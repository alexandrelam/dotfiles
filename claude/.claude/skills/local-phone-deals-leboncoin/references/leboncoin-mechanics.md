# Leboncoin mechanics

Every browser tool here is the Jev MCP server's (`mcp__open-claude-in-chrome-jev__*`), driving Brave.

Contents:
0. The call plan
A. Setting up the search with `jev_navigate`
B. The main `jev_assess` call
C. When Jev hands back
1. Search URL parameters
2–4. Leboncoin's data, for editing the scan
6. Failure modes

## 0. The call plan

Each tool call is a round trip back to Claude; time spent inside Jev is not. So the whole search is **two to four calls**, and nearly all the work happens inside one of them:

1. `tabs_context_mcp` + `tabs_create_mcp`, then prove it's Brave: `javascript_tool` on the new tab with `!!(navigator.brave && await navigator.brave.isBrave())` must return `true`. Otherwise `switch_browser` and retry (SKILL.md, Browser). Never continue in Chrome.
2. **Only if you can't build the search URL yourself** (a location other than Île-de-France, a filter you don't know the parameter for): **one** `jev_navigate` with every UI step as `subgoals` (§A). Its `final_url` is the search URL.
3. **One** `jev_assess` (§B). Its `items_script` loads every results page, fetches every ad body and every seller's ad count, and drops the obvious rejects. Jev then opens each shortlisted seller's profile, goes to the « Avis » tab, and answers every question about the ad *and* the seller in the same pass. One table comes back.
4. Write the Markdown file from that table.

Don't split step 3 into a `javascript_tool` scan followed by a judging call. Don't open ads or profiles one by one with `navigate` + `get_page_text`. And don't re-read a page to check an answer: read its `evidence` quote.

---

## A. Setting up the search with `jev_navigate`

Put the whole search setup in one call, one leg per UI action, so Jev does it all without coming back. Legs nothing depends on are `optional`. Example for a Galaxy S25 256 Go around Lyon, max 650 €:

```json
{
  "tabId": <your tab>,
  "start_url": "https://www.leboncoin.fr/",
  "subgoals": [
    { "goal": "Dismiss the cookie banner by continuing without accepting", "success_criteria": "No cookie banner covers the page", "optional": true },
    { "goal": "Type the search text into the main search box", "values": { "Rechercher": "galaxy s25 facture" }, "success_criteria": "The search box contains 'galaxy s25 facture'" },
    { "goal": "Pick the category « Téléphones & Objets connectés »", "success_criteria": "The category selector shows Téléphones & Objets connectés" },
    { "goal": "Open the location picker and choose Lyon", "values": { "Où": "Lyon" }, "success_criteria": "Lyon is selected as the location" },
    { "goal": "Set the search radius to 30 km if a radius control is offered", "success_criteria": "The radius shows 30 km", "optional": true },
    { "goal": "Run the search", "success_criteria": "A results page with ad cards is shown and /recherche is in the URL" },
    { "goal": "Open the price filter and set the maximum to 650", "values": { "Prix max": "650" }, "success_criteria": "price=-650 or price=0-650 in the URL" },
    { "goal": "Sort by most recent", "success_criteria": "sort=time in the URL", "optional": true }
  ],
  "questions": [
    { "key": "results", "type": "yes_no", "question": "Does the page list phone ads matching Galaxy S25?" }
  ],
  "final_check": "A leboncoin results page for 'galaxy s25 facture' in Téléphones & Objets connectés around Lyon, price at most 650 €",
  "max_ms": 180000
}
```

Write every leg you can foresee without seeing the page. Legs whose state lands in the URL should say so (`key=value in the URL`): that part is checked exactly. Never set `allow_sensitive`.

---

## B. The main `jev_assess` call

One call does extraction, prefiltering, profile browsing and judging:

```json
{
  "tabId": <your tab>,
  "items_script_url": "<search URL>",
  "items_script": "<the scan below, constants filled in>",
  "items_script_timeout_ms": 120000,
  "context": "<see below>",
  "questions": [ <see below> ],
  "where": [
    { "key": "invoice", "choice_in": ["solid", "claimed"] },
    { "key": "clean", "yes_above": 0.5 },
    { "key": "right_model", "yes_above": 0.5 }
  ],
  "return_chars": 600,
  "max_chars": 8000,
  "max_ms": 900000
}
```

### The scan (`items_script`)

Set `MODEL` (must match), `NEAR` (near-models to drop), `SINCE`, `MAX_PRICE` and `CAP` at the top. It returns one item per shortlisted ad. Each item opens the seller's profile, with a `goal` that moves to the « Avis » tab. The ad itself travels in `context`, since the profile page doesn't hold it. The `label` carries the facts you'll write in the file, including the invoice passage verbatim, because `evidence` quotes only come from the page Jev reads, which is the profile. Scan totals ride on the first label.

```js
const MODEL = /(i ?phone|ipone).{0,3}16/i, NEAR = /pro|max|plus|16 ?e\b/i;
const SINCE = '2026-08-20', MIN_PRICE = 400, MAX_PRICE = 600, CAP = 20;
function findAds(o,d){if(d>8||!o||typeof o!=='object')return null;if(Array.isArray(o)){if(o.length&&o[0]&&o[0].list_id&&o[0].subject)return o;for(const v of o){const r=findAds(v,d+1);if(r)return r;}return null;}for(const k in o){const r=findAds(o[k],d+1);if(r)return r;}return null;}
const nd = h => (h.match(/<script id="__NEXT_DATA__"[^>]*>([\s\S]*?)<\/script>/) || [])[1];
const pool = async (xs, n, f) => { let i = 0; await Promise.all(Array.from({length: n}, async () => { while (i < xs.length) { const k = i++; await f(xs[k]); } })); };
const base = location.href.replace(/&page=\d+/, ''), all = [], seen = new Set(); let total = 0;
for (let p = 1; p <= 15; p++) {
  const raw = nd(await fetch(base + '&page=' + p).then(r => r.text())); if (!raw) break;
  const ads = findAds(JSON.parse(raw), 0) || []; if (!ads.length) break; total += ads.length;
  for (const a of ads) {
    if (seen.has(a.list_id) || !MODEL.test(a.subject) || NEAR.test(a.subject)) continue; seen.add(a.list_id);
    const at = k => ((a.attributes || []).find(x => x.key === k) || {});
    all.push({ id: a.list_id, title: a.subject, price: a.price && a.price[0], city: a.location.city,
      date: a.first_publication_date.slice(0, 10), gb: at('phone_memory').value_label || '',
      ship: at('shipping_type').value || '', type: a.owner.type, uid: a.owner.user_id,
      idf: String(a.location.region_id) === '12' });
  }
}
await pool(all, 8, async r => { try { const raw = nd(await fetch('/ad/telephones_objets_connectes/' + r.id).then(x => x.text()));
  const m = raw && raw.match(/"body":"((?:[^"\\]|\\.)*)"/); r.body = m ? JSON.parse('"' + m[1] + '"').replace(/\s+/g, ' ') : ''; } catch (e) { r.body = 'ERR'; } });
const flags = r => { const l = (r.title + ' ' + r.body).toLowerCase(), f = [];
  if (!/factur/.test(l) || /pas (de|la) facture|sans (la )?facture|n'ai pas (la|de) facture|facture perdu/.test(l)) f.push('NOFACT');
  if (/reconditionn|refurb|grade [abc]\b|remis à neuf/.test(l)) f.push('RECOND');
  if (/blister|scell|jamais ouvert|non ouvert|pas ouvert|sous film d'origine/.test(l)) f.push('SEALED');
  if (/envoi uniquement|uniquement (par |en )?(envoi|livraison)|pas de (remise en )?main propre/.test(l)) f.push('SHIPONLY');
  if (/fissur|cass|rayé|ne s'allume|icloud|bloqué/.test(l)) f.push('DAMAGE?');
  if (/whats ?app|telegram/.test(l)) f.push('WA');
  const b = l.match(/batterie[^0-9]{0,20}(\d{2,3}) ?%/); if (b) f.push('bat' + b[1]); return f; };
window.__scan = all;
const short = all.map(r => ({ ...r, f: flags(r) }))
  .filter(r => !r.f.includes('NOFACT') && !r.f.includes('RECOND') && !r.f.includes('SEALED') && r.idf && r.type === 'private' && r.date >= SINCE && r.price >= MIN_PRICE && r.price <= MAX_PRICE)
  .sort((a, b) => a.price - b.price).slice(0, CAP);
const sellers = {};
await pool([...new Set(short.map(r => r.uid))], 8, async u => { try { const h = await fetch('/profil/' + u).then(x => x.text());
  sellers[u] = [(h.match(/"total_ads":(\d+)/) || [])[1] || '?', (h.match(/"registered_at":"(\d{4}-\d\d)/) || [])[1] || '?']; } catch (e) { sellers[u] = ['?', '?']; } });
window.__short = short;
const snip = b => { const i = b.toLowerCase().indexOf('factur'); return b.slice(Math.max(0, i - 70), i + 90); };
short.map((r, n) => ({
  url: 'https://www.leboncoin.fr/profil/' + r.uid,
  goal: 'Open the « Avis » tab of this seller profile so the reviews are listed. If the profile says it has no reviews, stay where you are.',
  success_criteria: 'The Avis tab is selected, or the profile shows it has no reviews',
  max_steps: 4,
  label: (n ? '' : `[scan: ${total} ads, ${all.length} model, ${short.length} kept] `) +
    `${r.id} | ${r.price}€ | ${r.gb} | ${r.title.slice(0, 50)} | ${r.city} | ${r.date} | ${r.ship} | ${r.type} | ${sellers[r.uid].join(' ads, since ')} | ${r.f.join(',')} | «${snip(r.body)}»`,
  context: `THE AD (not on this page): title "${r.title}", ${r.price} €, storage ${r.gb || 'not tagged'}, ${r.city}, published ${r.date}, shipping ${r.ship || 'unknown'}, seller type ${r.type}, seller has ${sellers[r.uid][0]} ads, member since ${sellers[r.uid][1]}. Ad text: ${r.body.slice(0, 2500)}`
}))
```

Keep `SINCE`, `MAX_PRICE` and `CAP` tight enough that about 20 items or fewer survive. Each item opens a page and runs a short navigation, so they're sequential, roughly 15–30 s each. Everything the script dropped stays on `window.__scan` if you need it later.

### `context` (shared)

State the rules once. Jev applies them to every item:

> You are judging a phone ad on leboncoin for a buyer who wants **<model> <storage>**, with a purchase invoice, and not reconditioned. The ad's own text is in each item's context under THE AD; the page you are reading is the seller's profile (their other ads, rating, reviews). Judge ad questions from THE AD and seller questions from the page.
> Invoice: *solid* = names the shop and ties the invoice to a warranty date; *claimed* = "facture disponible" and nothing more; *absent* = no invoice, or "pas de facture".
> Reconditioned wording: reconditionné, refurb, grade A/B/C, remis à neuf, écran changé/remplacé par nos soins.
> Seller patterns: household clear-out = many unrelated items; hobby flipper = a handful of varied phones over months; hidden reseller = several of the same recent model at once, template text, prices clustered just under retail; hidden pro = "Cette page professionnelle n'est pour l'instant pas accessible".
> Today is <date>. An ad older than 4 weeks is stale.

### `questions`

Ask everything in this one pass, since the page and context are already loaded:

```json
[
  { "key": "invoice", "type": "choice", "question": "How strong is THE AD's invoice claim?",
    "options": { "solid": "names the shop and a warranty date", "claimed": "mentions an invoice, nothing more", "absent": "no invoice, or says there is none" } },
  { "key": "clean", "type": "yes_no", "question": "Is THE AD free of any reconditioned or refurbished wording?" },
  { "key": "right_model", "type": "yes_no", "question": "Is THE AD really for exactly <model> (not an FE, Plus, Pro, Max, Ultra or other variant), and do any specs it lists match that model?" },
  { "key": "damage", "type": "yes_no", "question": "Does THE AD mention any damage, crack, repair, locked account or fault?" },
  { "key": "condition", "type": "choice", "question": "What condition does THE AD describe?",
    "options": { "like_new": "neuf / comme neuf / jamais utilisé", "good": "très bon or bon état, light marks", "worn": "visible wear", "unknown": "not stated" } },
  { "key": "accessories", "type": "choice", "question": "What comes with the phone per THE AD?",
    "options": { "box_charger": "box and charger or cable", "box": "box only", "none": "phone only", "unknown": "not stated" } },
  { "key": "warranty", "type": "choice", "question": "How much manufacturer warranty remains per THE AD?",
    "options": { "over12": "more than 12 months", "6to12": "6 to 12 months", "under6": "under 6 months", "none": "expired", "unknown": "not stated" } },
  { "key": "battery", "type": "choice", "question": "What battery health does THE AD state?",
    "options": { "high": "90% or more", "mid": "80 to 89%", "low": "under 80%", "unknown": "not stated" } },
  { "key": "off_platform", "type": "yes_no", "question": "Does THE AD push WhatsApp, Telegram, email, direct payment or a deposit?" },
  { "key": "theatre", "type": "yes_no", "question": "Does THE AD use sales theatre: emoji titles, 'prix imbattable', 'vs X € en boutique', urgency?" },
  { "key": "stale", "type": "yes_no", "question": "Was THE AD published more than 4 weeks before today?" },
  { "key": "pattern", "type": "choice", "question": "Which pattern does the seller's profile match?",
    "options": { "household": "household clear-out", "flipper": "hobby flipper", "reseller": "hidden reseller", "hidden_pro": "pro profile hidden", "single": "only this ad" } },
  { "key": "reviews_ok", "type": "yes_no", "question": "Does the seller have at least 3 reviews averaging 4.5/5 or better?" },
  { "key": "red_review", "type": "yes_no", "question": "Does any review mention a problem, a non-conforming item, a no-show or a cancelled sale?" },
  { "key": "new_profile", "type": "yes_no", "question": "Is the profile under 6 months old with at most 2 reviews?" }
]
```

Evidence quotes come from the profile page, so they back the seller answers (`pattern`, `reviews_ok`, `red_review`). For the ad answers, check them against the `«…»` passage in the label.

`where` returns only the rows with an invoice, no refurb wording and the right model. Dropped labels are listed under `filtered_out`, which is enough for the file's "Skip" section. If the result is saved to a file because it's large, read it with `jq`.

---

## C. When Jev hands back

Stay in Jev.

- `jev_navigate` returns `needs_help`, `needs_value` or `blocked`: read `reason` and `blockers`, then call `jev_navigate` again with a fix-up leg followed by `remaining_subgoals`. If one step really needs the ordinary tools, act by `ref` from `page_excerpt.interactive` (never screenshot coordinates), then go back to Jev for the rest.
- `jev_assess` rows with status `skipped` ("time limit reached"): call again with the same arguments, but add a filter to the end of `items_script` that keeps only the skipped ids. `window.__short` is still on the tab, so the script can end with a filtered `window.__short.map(...)` instead of re-scanning. Text items are unaffected.
- `items_script failed`: run the scan once through `javascript_tool` (`timeout_ms: 120000`) to see the error, fix it, then retry the `jev_assess`.
- A row with navigation `partial`: the page was read anyway, usually on the right tab. Trust the answers and evidence over the status.
- `blocked` by domain rules: `leboncoin.fr` is missing from `jev.allowed_domains` in the Jev config. Tell the user; don't route around it.

---

## 1. Search URL parameters

Build the URL and navigate to it. Base: `https://www.leboncoin.fr/recherche?`

| Parameter | Meaning | Example |
|---|---|---|
| `text` | Free text, matched against title **and body** | `text=galaxy%20s25%20facture` |
| `category` | Category id. `17` = Téléphones & objets connectés | `category=17` |
| `locations` | Region/department/city code. `r_12` = Île-de-France | `locations=r_12` |
| `price` | Range in euros, `min-max`; open-ended with `min-` | `price=400-600` |
| `owner_type` | `private` or `pro`. Omit for both | `owner_type=private` |
| `sort` / `order` | e.g. `sort=time&order=desc` for newest first | |
| `shippable` | `1` restricts to ads that can be shipped | |

The `facture` trick: because `text` matches ad bodies, appending it filters to sellers who mention an invoice. In one real run this took 59 ads down to 12. It is a keyword match — it catches "pas de facture" too, which is why bodies still get read individually.

Region codes other than Île-de-France are easiest to obtain by setting the location once in the UI and reading the resulting URL.

---

---

## 2. Extracting search results

Search pages are server-rendered Next.js. Everything is in `window.__NEXT_DATA__`. This is what the scan's parsing relies on. Run via `javascript_tool` only when debugging the scan:

```js
function findAds(o, d) {
  if (d > 8 || !o || typeof o !== 'object') return null;
  if (Array.isArray(o)) {
    if (o.length && o[0] && o[0].list_id && o[0].subject) return o;
    for (const v of o) { const r = findAds(v, d + 1); if (r) return r; }
    return null;
  }
  for (const k in o) { const r = findAds(o[k], d + 1); if (r) return r; }
  return null;
}
const ads = findAds(window.__NEXT_DATA__, 0);
JSON.stringify({ n: ads.length, list: ads.map(a => [
  a.list_id, a.subject, a.price && a.price[0],
  a.location && a.location.city, a.first_publication_date.slice(0, 10),
  (a.attributes || []).map(x => x.value_label).join('/')
])}, null, 0)
```

Useful fields on each ad: `list_id`, `subject`, `price[0]`, `location.city`, `first_publication_date`, `index_date`, `attributes[]` (état, modèle, capacité, couleur), `owner`, `status`.

Ad URLs are `https://www.leboncoin.fr/ad/telephones_objets_connectes/<list_id>`.

Return arrays rather than objects with keys — the key names repeated across 12 ads waste a surprising amount of the response budget and push you into truncation.

---

## 3. Fetching ad descriptions in bulk

The search payload does **not** include ad bodies (`body` is empty there). Fetch each ad page same-origin from the search tab and pull the body out of its `__NEXT_DATA__`:

```js
const res = [];
for (const a of ads) {
  const url = 'https://www.leboncoin.fr/ad/telephones_objets_connectes/' + a.list_id;
  let body = '';
  try {
    const html = await fetch(url, { credentials: 'same-origin' }).then(r => r.text());
    const m = html.match(/<script id="__NEXT_DATA__"[^>]*>([\s\S]*?)<\/script>/);
    if (m) {
      const bm = m[1].match(/"body":"((?:[^"\\]|\\.)*)"/);
      if (bm) body = JSON.parse('"' + bm[1] + '"');
    }
  } catch (e) { body = 'ERR'; }
  body = body.replace(/\s+/g, ' ');
  const i = body.toLowerCase().indexOf('factur');
  res.push([a.list_id, a.subject, a.price && a.price[0],
            i >= 0 ? body.slice(Math.max(0, i - 90), i + 110) : 'NO-FACTURE']);
}
window.__res = res;                 // keep it, print in slices
JSON.stringify(res.slice(0, 6), null, 0)
```

The scan in §B does this; this is the standalone version, for debugging. `javascript_tool` returns the last expression and allows top-level `await`. Pass `timeout_ms` (up to 120000) when the script fetches many pages: without it, it gives up after 20 s. On an extension build older than the `replMode` fix, top-level `await` throws "missing ) after argument list" or "await is only valid in async functions". In that case, wrap the script as `(async()=>{ ...; return x })()`.

Extracting the body with a regex on the raw JSON string, rather than `JSON.parse` + tree walk, is deliberate: the ad payload is large and the regex is both faster and less likely to blow the response limit.

---

## 4. Reading the seller object

Take the owner from the ad's own object, not from the first `user_id` found in the page — "similar ads" blocks contain other sellers and will mislead you.

```js
function findAd(o, d, id) {
  if (d > 10 || !o || typeof o !== 'object') return null;
  if (!Array.isArray(o) && String(o.list_id) === String(id) && o.subject) return o;
  for (const k in o) { const r = findAd(o[k], d + 1, id); if (r) return r; }
  return null;
}
```

`ad.owner` looks like:

```json
{"store_id":"2112855","user_id":"9a87...","type":"private","name":"Dadou88","no_salesmen":true,"activity_sector":""}
```

`type` is self-declared. A `private` seller with a `store_id` is normal and means nothing on its own — the profile check in §B is what actually tells you.

---

## 6. Failure modes

- **`javascript_tool` returns `[BLOCKED: Cookie/query string data]`** — something in the output tripped a filter. Re-run returning fewer fields; dropping postcodes and image URLs usually clears it.
- **Truncated JSON** — print in slices from a `window` variable rather than shortening the analysis.
- **Empty `subject`/`price` on DOM cards** — the search list lazy-loads; use `__NEXT_DATA__` instead of scraping cards on search pages.
- **`get_page_text` on a search page returns the shipping carousel** — that's a promo block, not the results. Same fix: use `__NEXT_DATA__`.
- **`items_script` or `CDP Runtime.evaluate` timed out**: the scan ran longer than its timeout. Set `items_script_timeout_ms: 120000`, or lower `CAP` and narrow the search. The first run usually finished in the page anyway, so check `window.__scan` / `window.__short` before re-fetching.
- **"Cette page professionnelle n'est pour l'instant pas accessible"** on a profile: a pro account with its profile hidden. Treat it as a reseller that doesn't want to be looked at, and check whether its ad text matches other listings.
- **Unsupported params** — leboncoin silently ignores parameters it doesn't know (`user_id=` on `/recherche` returns unfiltered global results). If a filtered query returns implausibly many or wildly off-topic results, assume the parameter was dropped.
- **Old extension builds sent Jev each ad title three times**, which made single-ad sellers look like resellers. Builds with `collapseRepeats` in `host/jev/assess.ts` fix this. On an old build, trust the excerpt over the `pattern` answer.
- **`total_ads` vs cards**: `total_ads` counts everything, while the profile DOM holds only the first page, so a profile can report 46 ads and render 35 cards. Use the count for scale and the cards for composition.
