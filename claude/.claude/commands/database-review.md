## PR context

- Changed files: !`gh pr diff --name-only`
- Full diff: !`gh pr diff`

## Your task

You are a meticulous senior reviewer of database changes on this stack: Flyway SQL migrations and the hand-written Kotlin Entity/Companion code on top of the ORM (`server/orm/`, docs in `server/orm/docs/`). Review ONLY the changes in the diff above (read surrounding code and the migration history for context, but do not comment on pre-existing code the PR didn't touch). Do NOT modify any files — this is a read-only review.

**In scope:** `.sql` migrations under `server/jooq/migrations/{regional,global,medical}/src/main/resources/flyway-*/`, and hand-written `Entity`/`Companion` `.kt` files.

**Out of scope — skip entirely:** generated jOOQ under any `/generated/` or `/build/` path (`*Record.kt`, table constants). These are code-generated from the live schema and never hand-edited; a diff touching them by hand is itself a 🔴 finding.

The one idea everything hangs on: **the DB leads, the code follows.** The flow is always schema → codegen → entity, never the reverse. A migration defines truth; jOOQ regenerates; only then does the Kotlin entity change to match.

### 1. Migration safety — migrations run automatically on startup against a live, in-use database

Every DDL statement must be **online-safe**: it must not hold a long `ACCESS EXCLUSIVE` lock or block reads/writes on a hot table. This is the highest-severity category — an unsafe migration is a production incident, not a style nit.

- **Forward-only, immutable once applied.** No `down`/rollback. Flag ANY edit to a migration file that is already merged/applied (it has run in other environments — changing it makes code disagree with the live schema). A bad migration is fixed by a *new* migration.
- **File naming**: `V<YYYY.MM.DD.HH.MM.SS>__Short_description.sql`. The full timestamp (to the second) is what keeps branches from colliding on version order — flag truncated or non-timestamp version prefixes.
- **Adding an index → `CREATE INDEX CONCURRENTLY`, in its own migration that runs outside a transaction.** A plain `CREATE INDEX` locks out writes for the whole build. Match the repo convention (see `..._backfill_idx` migrations).
- **Adding a `CHECK` or `FOREIGN KEY` constraint → add it `NOT VALID`, then `VALIDATE CONSTRAINT` in a *separate later* migration.** Adding a validated constraint directly takes `ACCESS EXCLUSIVE` and full-table-scans; `VALIDATE` alone takes only `SHARE UPDATE EXCLUSIVE` and doesn't block reads/writes. Flag a constraint added-and-validated in one shot on a non-trivial table.
- **Validate/backfill migrations should bound their locks** with `SET LOCAL lock_timeout` / `SET LOCAL statement_timeout` so a stuck migration fails fast instead of stalling the deploy (see the `validate_shadow_*` migrations).
- **New NOT NULL column on an existing table → expand/contract.** Add the column nullable (+ default), backfill asynchronously (batched, behind a partial index on the not-yet-backfilled rows), then set `NOT NULL` and drop the backfill index in a follow-up *contract* deploy. Flag a bare `ADD COLUMN ... NOT NULL` (no default) on a populated table.
- **Rewrites & type changes**: flag `ALTER COLUMN ... TYPE`, `ADD COLUMN ... DEFAULT <volatile>`, and similar full-table-rewrite operations on large tables; call for the safe multi-step alternative.
- **Regional/global parity.** Several tables are mirrored across the `regional` and `global` DBs and kept in sync by a dedicated sync component, with a parity test asserting the two schemas match. A schema change to one side almost always needs the mirror migration on the other — flag a one-sided change to a mirrored table and point at the parity test.
- **Invariants that span columns** belong in a DB `CHECK` (and, where a write should auto-correct, a `BEFORE` trigger), not only in Kotlin — mirror the repo's `iff(...)` both-null-or-both-set pattern where relevant.

### 2. Tenancy — a new org-scoped table is not done until it is siloed

Cross-org isolation ("siloing") is enforced by PostgreSQL Row-Level Security, applied to *every* statement — not by the ORM rewriting queries.

- **A new per-organization regional table MUST, in the same migration**, `ENABLE ROW LEVEL SECURITY` and define its `organization_scoping` policy (`... USING (current_organization_matches(organization_id))`). A test asserts every regional table has RLS on (with an explicit exceptions list) — a new table without it fails that test. Flag its absence as 🔴.
- **Base class ↔ `organization_id` column must agree**: `Organization*` base ⇔ NOT NULL `organization_id`; `MaybeOrganization*` ⇔ nullable; `Unscoped*` ⇔ no column. A mismatch is a bug.
- **`organization_id` is stamped automatically on create** from the creation context — flag code that sets it by hand. Check the creation context matches the tenancy: `org.asScopedEntityCreationContext()`, `ScopedEntityCreationContext.Global`, or `null` for unscoped.
- **Manual `AND organization_id = ?` in ordinary request code is redundant** (RLS already does it) — flag it as a smell. It is correct *only* on paths that sometimes run as an almighty/back-office (RLS-bypassing) context and want exactly one org.
- **Sensitive medical data belongs in the `MEDICAL` DB**, not regional.

### 3. Entities & columns

- **`val`/`var` must track write timing.** Never written → `val`. Written once at insert then frozen → `var ... @ShouldNotChangeAfterInsertion set` (flag a missing annotation on an insert-only field). Mutated after insert → plain `var`.
- **Helper nullability must match the schema.** `column(FIELD)` ⇒ non-null `T`; `nullableColumn(FIELD)` ⇒ `T?`. A startup assertion checks this against the real DB nullability — a mismatch fails boot. Same for declared `defaultValue`: it must equal the DB-side `DEFAULT`.
- **A nullable DB column that a `CHECK` constraint guarantees non-null** should be exposed as non-null via `nullableColumn(...).guaranteedNotNullBy(CONSTRAINT)` — and the `CONSTRAINT` name passed to `checkConstraint("...")` must match a constraint that actually exists in a migration. Flag a name that doesn't resolve to a real constraint.
- **FK columns should be typed IDs** (`.asNonNullableIDColumn()` / `.asNullableIDColumn()`), not bare `Int`/`Long` — a `BookId` must not be assignable to an `AuthorId`.
- **Reading a non-null column before insert throws.** Flag reads of a non-null property on a not-yet-inserted entity outside its `create { }` block, unless the column declares a `defaultValue`.
- Naming: Entity = singular noun; Companion = `<Entity>Companion`; public interface = `I<Entity>`. Every Companion implements the one-line `of(state)` factory.

### 4. Reading & writing

- **Prefer the ORM's read helpers over hand-built jOOQ DSL queries.** Reach for `getById`/`getAllWhere`/`getAtMostOneWhere`/references/companion helpers first; drop to the raw jOOQ DSL (`DSL.*`, `selectQuery()`, joins) only when no helper expresses the query. Raw DSL loses the ORM's batching, identity-map, signals, and cache invalidation, and joins silently break the row→Entity mapping — flag a hand-rolled DSL query that a companion helper or a `.with(ref)` batch load would cover, and show the helper form.
- **`update()` is explicit — no autosave.** Every column mutation must be followed by a persist (`update()` / `store()`); a mutation with no persist silently discards the change. This is a top correctness check — trace each field assignment to its write.
- **Transactions: as few as possible.** A `transaction { }` is justified only for (1) atomicity across several writes where an intermediate state is invalid (e.g. delete-then-reinsert a list, or insert-plus-reorder), or (2) coordinating a DB write with an external system. Flag a transaction wrapped reflexively around a single write — leaked/long transactions cause "idle in transaction" and pool starvation. The transaction rides the coroutine context implicitly; nested DB calls join it automatically.
- **Pagination**: user-facing lists use the cursor/keyset `OpaquePaginator`. Flag `FixedPaginator` (LIMIT/OFFSET) unless a forced-offset external API (e.g. SCIM) demands it, and flag `getAll()` feeding a user-facing list.
- **Read variant must match the missing/duplicate semantics**: `getById` (throws) vs `getByIdOrNull`; `getAtMostOneWhere` (throws if >1) vs `getExactlyOneWhere` (throws if ≠1). Flag a mismatch.
- **Concurrent updates** to the same row need optimistic locking via `storeIfLastVersion()` (requires a `record_version` column).

### 5. Relationships & N+1

Relationships are batch-loaded **references**. This is the single most common regression to catch.

- **Flag `list.map { it.someRef.get() }`** (one query per element) → the fix is `list.with(someRef)` / `list.nullableWith(someRef)`, which batches into one query. Query-count tests (`countDirectDBQueries`) exist precisely to pin this — a reference change should keep or add one.
- **Reference builder must match FK nullability**: `referenceTo` for a NOT NULL FK, `nullableReferenceTo` for a nullable FK.
- **Declare-once-on-Companion, bind-per-Entity**: the loader (`val fooRef = referenceTo(...)`) lives on the Companion; the Entity exposes `val fooRef get() = companion.fooRef.toGetter(this)`.
- **`@OptIn(NotBatched::class)` in a loop or a GraphQL resolver is a red flag** for reintroduced N+1 — scrutinize the justification.
- Prefer narrowing an existing relationship (`.adaptQuery { }` on `referenceFrom`/`referenceVia`) over a hand-rolled `genericReference` when the result is just a filtered subset.

### 6. Signals & caching

- **Batch writes skip per-row `Before*` signals.** `updateAllWhere` / `deleteAllWhere` run a single statement and never load each row, so `BeforeUpdate`/`BeforeDelete` logic does NOT run (matching `After*` fires once). If the code relies on those hooks (validation, cascade, derived columns), it must load and update/delete entities individually (`deleteAll(entities)` *does* fire `BeforeDeletes`). Flag a batch write where a `Before*` hook is silently bypassed.
- **Raw jOOQ (`DSL.*`, raw SQL) bypasses signals AND cache invalidation entirely** — loader caches and the identity map go stale for the rest of the request. Flag raw writes to an entity-backed table; require going through the Entity/Companion, or manual invalidation if raw is unavoidable.
- **`Before*` handlers must be synchronous and may mutate the entity** (the change is persisted by the same statement); **`After*` must not mutate** (won't be persisted). Flag an `After*` handler that mutates-and-expects-persistence.
- **Subscribe to the narrowest layer that fits** (`Inserts/Updates/Deletes` < `Stores` < `AnyChanges`), and use an **async `After*`** for side effects that must only happen post-commit — never rely on async for consistency-critical logic.
- **Stale entity across scopes.** The single-instance-per-row guarantee holds only *within* one cache scope; an entity carried across a transaction/scope boundary can be stale — flag missing `refresh()` where freshness matters. Background work touching entities needs `withOrmCache { }` / `withOrmCacheAndAlmighty { }`.

### 7. Access control

Distinct from siloing: **access verifiers are app-level, per-row, intra-org**, and run only at two moments — the checked `getByUuid(...)` load, and when data crosses the GraphQL boundary. Plain `getById`/`getAllWhere` in the middle of logic do NOT run them (by design).

- **Every Companion declares an `accessVerifier`** — scrutinize its correctness; this is the authorization boundary. `alwaysYesAccessVerifier()` is only for genuinely global/public config; `alwaysNoAccessVerifier()` blocks all ORM reads (e.g. sensitive audit logs). `viaRefAccessVerifier(ref, companion)` delegates through a reference — check the reference actually gates access.
- **`getByUuidUnchecked...` skips the verifier** — flag every use and confirm it's justified.

## Output format

Group findings by severity, most severe first:

1. 🔴 **Blocker** — unsafe migrations (locking DDL, edited-after-merge, missing RLS on a new org table), data-correctness bugs, dropped `update()`/writes, tenancy/access holes.
2. 🟡 **Should fix** — N+1 reintroductions, needless transactions, bypassed signals, nullability/annotation mismatches, wrong read/pagination variant.
3. ⚪ **Nit** — naming, migration comment quality, minor conventions.

For each finding give: `file:line`, a one-sentence *why it matters*, and a concrete fix (SQL or Kotlin snippet when the fix isn't obvious). For a migration-safety finding, name the lock it would take and the safe multi-step alternative.

End with a one-paragraph verdict: is this safe to merge and deploy as-is, or what must change first? Pay special attention to whether the schema→codegen→entity flow was followed and whether any migration could stall a live deploy. If everything is sound, say so plainly — do not invent findings to seem thorough.

$ARGUMENTS
