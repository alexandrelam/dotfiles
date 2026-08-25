---
name: code-review
description: Review the code quality of the current pull request. Finds correctness, maintainability, security, performance, and testing issues in pull-request changes. Use when the user asks to review a PR or its diff.
---

Review the current pull request's diff, focusing only on actionable code-quality problems introduced by the change. Assume the user has already checked out the PR branch; do not switch branches or create a worktree.

## Process

### 1. Get the pull-request diff

Run `gh pr diff` to obtain the diff for the pull request associated with the current branch. Do not ask the user for a branch, fixed point, or merge-base.

Before going further, confirm the command succeeds and the diff is non-empty. Stop with a clear explanation if the current branch has no associated pull request or there are no changes to review.

### 2. Review the diff first

Start by reading only the pull-request diff. Do not proactively read repository instructions, entire changed files, nearby modules, tests, call sites, or other supporting files. The diff is often sufficient for a useful review, and minimizing file reads keeps the review fast and inexpensive.

Review the diff for:

- **Correctness** — broken logic, invalid assumptions, edge cases, error paths, state inconsistencies, race conditions, and resource leaks.
- **Security and privacy** — unsafe input handling, authorization mistakes, injection, secret exposure, and sensitive-data leakage.
- **Reliability** — failure handling, retries, idempotency, cleanup, concurrency, and compatibility concerns.
- **Maintainability** — unnecessary complexity, unclear ownership, excessive coupling, misleading names, duplication, and hard-to-change designs.
- **Performance** — avoidable hot-path work, unbounded operations, inefficient I/O or queries, and memory growth. Report only plausible material impact.
- **Tests** — missing or weak coverage for meaningful new behavior, regressions, boundary cases, and failure modes.

Use code smells as heuristics, not automatic findings:

- mysterious names
- duplicated logic
- feature envy or misplaced responsibilities
- data clumps and primitive obsession
- repeated conditionals
- shotgun surgery and divergent change
- speculative generality
- message chains and needless middle layers
- inappropriate inheritance

Do not report a smell unless it creates a concrete maintenance or correctness cost in this change.

### 3. Resolve uncertainty selectively

Only read beyond the diff when a specific candidate finding cannot be confirmed from the diff alone, or when essential context is clearly missing. Read the smallest relevant section of the fewest files needed to answer that question, then stop exploring.

For a broad or unfamiliar change, you may spawn a focused sub-agent with a small model only when it is likely to resolve a concrete uncertainty more cheaply than exploring directly. Give it one narrow question and limit its investigation to the relevant area. Use its output as research, then independently validate any finding before reporting it.

Do not explore the codebase merely to look for additional issues, learn the architecture, or gather general context.

### 4. Validate findings

For every candidate finding:

1. Confirm it is introduced by or materially affected by the diff.
2. If the diff is insufficient, inspect only the minimum context needed to confirm or reject it.
3. Check whether visible tests, types, validation, or framework guarantees already prevent it.
4. State the concrete failure mode or maintenance cost.
5. Give a focused remediation.

Discard speculative, cosmetic, preference-only, or tooling-enforced comments. Do not require unrelated cleanup.

### 5. Report

Lead with findings, ordered by severity:

- **Critical** — likely security incident, data loss, or broadly broken production behavior; must fix before merge.
- **High** — clear bug or serious reliability issue in a common path; should fix before merge.
- **Medium** — real defect or maintainability problem with bounded impact.
- **Low** — worthwhile improvement with concrete but limited impact.

For each finding include:

- severity and concise title
- file and line or hunk
- why it matters, including the concrete failure mode
- a focused fix

Keep findings concise and avoid large code excerpts. If no actionable issues remain after validation, say so explicitly.

End with a short assessment of:

- overall code quality
- test coverage risks
- whether the change appears merge-ready from a code-quality perspective
