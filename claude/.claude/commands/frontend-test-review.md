## PR context

- Changed files: !`gh pr diff --name-only`
- Full diff: !`gh pr diff`

## Testing strategy of this codebase (the rules you review against)

1. **E2E tests** are Playwright tests living in the webapp's `e2e` directory. They cover user-facing flows and component behavior in the browser.
2. **Unit tests** exist for `.ts` files: pure logic, utils, hooks-extracted logic, data transforms, etc.
3. **We do NOT write RTL (React Testing Library) component tests.** Component rendering/interaction is covered by e2e; component logic should live in plain `.ts` files where it can be unit tested.

Any test in the diff that doesn't fit this strategy is a finding.

## Your task

You are reviewing ONLY the changes in this PR, from a testing perspective. This is a read-only review — do not modify files. You may read files outside the diff (existing tests, tested modules) to judge coverage and redundancy.

### 1. Coverage: is the changed code adequately tested?

Build a mental map: for each meaningful production change in the diff, where is it tested?

- **New or changed `.ts` logic** (utils, transforms, validation, non-trivial branching): is there a new/updated unit test exercising it? Flag untested logic, and untested *branches* of tested functions (error paths, edge cases like empty arrays, null/undefined inputs, boundary values) — but only edge cases that are actually reachable given the types and call sites.
- **New or changed user-facing behavior** (new flow, changed interaction, new page/dialog, changed form validation feedback): is there a new/updated Playwright test in the webapp's `e2e` directory covering the happy path? For critical flows, is the main failure path covered too?
- **Bug fixes**: is there a regression test that would have failed before the fix? A bug fix without a test pinning the behavior is a finding.
- **Deleted/refactored code**: were the corresponding tests updated or removed? Flag orphaned tests that now test nothing meaningful, and tests weakened during the refactor (assertions removed or loosened to make them pass).
- If logic worth unit testing is trapped inside a `.tsx` component (and thus untestable under our strategy), suggest extracting it to a `.ts` file + unit test rather than suggesting an RTL test.

Missing coverage is only a finding when the code is worth testing: skip trivial passthroughs, type-only changes, constants, and pure styling.

### 2. Strategy compliance

- Flag any RTL / `@testing-library/react` test added in the diff → propose the replacement: extract logic to `.ts` + unit test, and/or an e2e test if the behavior is user-facing.
- Flag e2e tests placed outside the webapp's `e2e` directory, and unit-style tests written as e2e (a Playwright test asserting pure logic that a unit test would cover in milliseconds).
- Flag the reverse too: unit tests trying to simulate DOM/browser behavior with heavy mocking — that's e2e territory.

### 3. Test bloat: too many useless tests

Generated code tends to over-test. Hunt for:

- **Redundant cases**: several tests exercising the same code path with cosmetically different inputs (same branch, same assertions). Recommendation: keep one, or collapse into a parameterized test (`test.each` / `it.each` in the unit runner; loop over a cases array in Playwright) — name each case so failures stay readable.
- **Tests that can't fail meaningfully**: asserting mocks return what they were mocked to return, testing that a constant equals itself, snapshot tests of trivial output, "renders without crashing"-style tests.
- **Testing the framework or the library**: verifying that React/Playwright/lodash/zod behaves as documented instead of verifying our code.
- **Tautological tests**: reimplementing the production logic inside the test to compute the expected value.
- **Over-mocked unit tests**: so much is mocked that the test only verifies call wiring, not behavior.
- **Redundant e2e**: multiple Playwright tests re-walking the same navigation/setup to assert minor variations — suggest merging into one test with several assertions, or parameterizing.
- **Duplicate coverage across layers**: the exact same behavior asserted both in a unit test and an e2e test with no added value at either layer — keep the unit test for logic, keep e2e only for the user-visible part.

For each bloat finding, give a concrete recommendation: **delete** (and why nothing is lost) or **parameterize** (with a short sketch of the `test.each` table or cases array).

### 4. Test quality nits (only for tests touched in the diff)

- Test names that don't describe behavior ("works", "test 2", "should handle data").
- Assertions that are too loose (`toBeTruthy`/`toBeDefined` where an exact value is checkable; asserting only array length, not content).
- Playwright: arbitrary `waitForTimeout` instead of web-first assertions/auto-waiting; brittle selectors (deep CSS/XPath) instead of role/test-id based locators; tests depending on execution order or shared mutable state.
- Unit: missing `await` on async assertions; shared state leaking between tests; `beforeEach` setup that only some tests use.

## Output format

Structure the review as:

1. **Coverage map** — a short table: changed area → how it's tested (unit / e2e / ❌ untested) → verdict.
2. 🔴 **Missing coverage** — untested changes that matter, with the specific test to add (file location + a 3–5 line sketch of the test).
3. 🟠 **Strategy violations** — RTL tests, misplaced tests, wrong layer.
4. 🟡 **Bloat** — tests to delete or parameterize, with the concrete refactor.
5. ⚪ **Nits** — quality issues in touched tests.

For every finding: `file:line`, why it matters, concrete fix. End with a verdict: is the test suite for this PR right-sized — under-tested, over-tested, or about right? If it's about right, say so plainly; do not invent findings.
