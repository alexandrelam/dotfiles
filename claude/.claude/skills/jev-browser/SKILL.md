---
name: jev-browser
description: How to drive the user's Chrome with the Jev MCP server (open-claude-in-chrome-jev) — jev_navigate for tasks, jev_assess for judging many pages at once. Use before the first jev_* call, when the user asks to do something in their browser, or to find, compare, filter or rank listings, search results or profiles on a site. Other skills that use Jev build on this one.
---

Jev is a fast, cheap decision model. It clicks, types and answers **your questions** with probabilities; it never writes text or invents a value. You plan, supply every piece of text and state the rules. Each of your turns costs far more than a Jev run, so **one call per job**, and put everything into it.

Start with `tabs_context_mcp`, then `tabs_create_mcp`: work in a new tab and never navigate the user's own tabs. Reuse that one tab for the rest of the session.

## Pick the tool

| Job | Tool |
|---|---|
| Do something: navigate, fill a form, change a setting | `jev_navigate` |
| Judge, filter or rank many items: ads, results, profiles, rows | `jev_assess` |
| One step Jev can't take | ordinary tools, by `ref` from `page_excerpt.interactive`, never screenshot coordinates; then back to Jev |

## Doing things: jev_navigate

- Every leg of the task goes in `subgoals`, all text in `values`, and `start_url` for where to begin. For legs you can't see yet, describe the outcome ("fill the form and save it"), not field names you'd be guessing.
- `success_criteria` names **where you are**, not page content: "the revision history is open", not "a list of revisions is shown". State that lives in the URL goes as key=value (`tab=billing in the URL`), which is checked exactly.
- Verification goes in `questions` (see Questions below), not screenshots or `javascript_tool`. Add `final_check` when a later leg could undo an earlier one.
- A leg can wait for slow content (a report being built, search results): say so in its goal ("wait until the results have loaded") and success_criteria, and raise `max_ms` to match. Don't poll with `javascript_tool`.
- `fill_defaults: true` only when any option will do (test data). `allow_sensitive: true` only when the user asked for that save, send, pay or delete.
- When it hands back, stay in Jev:

| Status | Next call |
|---|---|
| `needs_help` / `blocked` | fix-up legs for what `reason` and `blockers` name, then `remaining_subgoals` |
| `needs_value` | add the missing text to `values`, same call again |
| `limit_reached` | raise `max_ms` / `max_steps`, or split the task |
| `partial` | done; the optional legs listed in `reason` were skipped |

## Questions: how Jev judges

You write a set of questions once; Jev answers **every question for every page** in one request per page. It never replies in prose: each answer is a probability over the options you defined.

```js
questions: [
  { key: "real_laptop", type: "yes_no", question: "Is this ad selling a complete, working laptop?",
    yes: "a laptop for sale", no: "a part, accessory, broken unit or a wanted ad" },
  { key: "condition", type: "choice", question: "What condition does the seller describe?",
    options: { like_new: "new or like new", good: "used, works fine", worn: "visible wear or a flaw", faulty: "a defect" } },
  { key: "deal", type: "score", question: "How good is the price against the reference prices in context?",
    scale: ["Overpriced", "Fair", "Good", "Great"] },
]
```

Answers come back per page, each with `evidence`, the passage it rests on quoted verbatim:

```js
{ real_laptop: { yes: 0.97, evidence: "Vends mon Dell XPS 15…" },
  condition:   { choice: "good", p: 0.71, runner_up: { choice: "worn", p: 0.22 }, evidence: "…" },
  deal:        { score: 2.4, label: "Good", confidence: 0.8, evidence: "…" } }
```

- In **jev_assess** the questions are asked of every item; you get one row per item plus a `summary` per question (yes/no counts, counts per choice, mean score).
- In **jev_navigate**, `questions` are asked of the page the run ends on and come back under `answers`, with `evidence` quoted from the page. If a leg stops the run they are still answered, about the page it stopped on, and `answers_note` says so.
- A leg can carry its **own** `questions`, answered as soon as that leg is done and returned in `subgoals[i].answers`. This is how a test plan runs in **one call**: one leg per step, the check for that step on the leg.
- To only ask about the page as it is, call `jev_navigate` with `questions` and no `goal` or `subgoals`. Never write a "do nothing" leg for it.

```js
subgoals: [
  { goal: "Click the Details tab", success_criteria: "the Details tab is selected",
    questions: [{ key: "orders_listed", type: "yes_no", question: "Is the list of orders shown?" }] },
  { goal: "Type the note in the Comment box, without sending it", success_criteria: "the Comment box holds the note",
    values: { "Comment": "Please gift wrap it" }, optional: true },
  { goal: "Click Summary, then Details again", success_criteria: "the Details tab is selected",
    questions: [{ key: "comment_kept", type: "yes_no", question: "Does the Comment box still hold 'Please gift wrap it'?" }] },
]
```

## Judging many items: jev_assess

### Build the items with fetch, not tabs

`url` items are opened one by one in the tab; `text` items are judged in parallel with nothing opened. So let `items_script` do the reading: it runs in the page with the user's session, fetches every result page and every item page, and returns `text` items. A few hundred items take seconds instead of many minutes.

```js
// items_script — runs on items_script_url; the last expression is the item array
const get = async (u) => new DOMParser().parseFromString(await (await fetch(u)).text(), "text/html");
const pool = async (xs, n, f) => { const out = []; let i = 0;
  await Promise.all(Array.from({ length: n }, async () => { while (i < xs.length) { const k = i++; out[k] = await f(xs[k]).catch(() => null); } }));
  return out; };

const pageUrl = (p) => { const u = new URL(location.href); u.searchParams.set("page", p); return u.href; };
const pages = await pool([1, 2, 3, 4, 5].map(pageUrl), 6, get);
const links = [...new Set(pages.flatMap((d) => d ? [...d.querySelectorAll("a[href*='/ad/']")].map((a) => new URL(a.getAttribute("href"), location.href).href) : []))];
const items = await pool(links, 6, async (u) => {
  const d = await get(u);
  // Adapt this part to the site. Prefer its embedded data to the markup:
  // JSON-LD, or Next.js <script id="__NEXT_DATA__"> (pick the fields, never dump it whole).
  const ld = [...d.querySelectorAll('script[type="application/ld+json"]')].map((s) => s.textContent).join("\n");
  const text = ld || (d.querySelector("main") ?? d.body).textContent.replace(/\s+/g, " ");
  return { text: text.slice(0, 2500), label: `${d.title} | ${u}` };
});
items.filter(Boolean);
```

- Pool of **5–8 fetches** at a time. Sites with anti-bot protection (Leboncoin uses DataDome) can captcha or block the user's browser if you burst hundreds at once.
- Set `items_script_url` (the listing), `items_script_timeout_ms` up to 120000 for long lists, and **`return_chars: 0`**.
- Keep each item's text to what the questions need, about 2–3k characters: title, price, place, condition, description. Put the item's URL in `label` so it comes back in the rows.
- Drop what the questions would reject anyway (wrong category, pro sellers, duplicates) inside the script, before judging.
- Judge **the page that holds the answer**. When an aggregator truncates the text and links to a source site, resolve the source URL in the script and fetch that.
- Use `url` items only when a page needs rendering or a click to show its content.

### Write the questions once

- **Every question in the first pass.** A second call on the same items redoes all the work. Include the ones whose `evidence` you'll want later: "does the ad name the nearest station?" as a yes_no returns the sentence that names it.
- Put what a page can't know in `context`: today's reference prices (look them up on the web first; don't trust your own price memory), the user's criteria, what counts as good. Facts about one item go in that item's `context`.
- **Rank with a `score`, not a strict yes_no.** A yes_no "is this a good deal?" with a tight rule can leave 1 of 86 items; a `score` scale such as Overpriced / Fair / Good / Great lets you read the top of the list and decide the cutoff yourself.
- State each question fully (Jev sees nothing else of your intent), one fact per question, and for yes_no say what counts as yes and no. A `choice` needs at least 2 `options`, a `score` an ordered `scale`. Keys are short identifiers, not `verified`, `model`, `usage` or `id`.
- On long lists pass `where` (e.g. `[{key: "relevant", yes_above: 0.5}]`) to get back only the matching rows. The summary still counts everything.

### Read the answers

- Each answer carries `evidence`, a passage quoted verbatim. Read it before opening any page. Doubt is a yes_no near 0.5 or a choice with a close `runner_up`.
- A result too large for the reply is saved to a file. Read it with `jq` rather than paging through it.
- For an exact value to reuse (an ID, a URL), read it with `get_page_text`; Jev's answers are probabilities, not text.
- Before recommending, check the top rows yourself against the evidence: Jev scores what the ad says, and a price that is too good is a scam signal as often as a deal.

## Reporting

Answer in chat: a short table of the best items with links, then "also worth a look" and "skip" with the reason for each. Say how many items were scanned and how the reference prices were sourced.
