---
name: pr-walkthrough
description: Explains a pull request in plain English — what it does and why — grouping the changed files logically and walking through one group at a time. Use when the user asks to understand or be walked through a PR, not to review it.
---

# PR Walkthrough

Teach the reader what a pull request does and why, in plain English. This is explanation, not review: do not hunt for bugs or grade the code. If a genuine correctness concern is unavoidable, mention it once at the end and move on.

## Process

### 1. Locate the PR

Default to the pull request for the currently checked-out branch: `gh pr view --json number,title,body,url,author,baseRefName,headRefName,additions,deletions,changedFiles,commits`. If the user supplies a PR number or URL, use that instead.

Then get the change itself with `gh pr diff`. Assume the branch is already checked out; never switch branches or create a worktree.

If there is no associated pull request, say so and ask whether to walk through the local branch changes against `master` instead. Do not silently fall back.

### 2. Build real understanding before writing anything

Read the diff in full first, then fill the gaps. Read as much surrounding code as you need to explain the intent correctly, prioritizing:

- The PR description and commit messages for the author's own framing.
- Linked tickets and issues. Branch names often carry a ticket ID (e.g. `<author>/<TICKET-ID>/...`); if a tracker or issue link is available, read it for the problem statement.
- The definitions of functions, types, and components the diff touches but does not show, so you can describe what they were doing before.
- Tests, which usually state the intended behaviour most explicitly.
- Callers of changed code, to know what is affected downstream.

Distinguish what you verified from what you inferred. When you are guessing at intent, label it as a guess.

### 3. Group the changed files

Group by the role a file plays in the change, not by directory. Common groupings: the data or API contract (schema, GraphQL, migrations, types), core business logic, callers and wiring, user-facing UI, tests, and configuration.

Guidelines:

- Order groups so understanding accumulates: the contract or data model first, then the logic that implements it, then callers and UI, then tests.
- Aim for two to six groups. A group of one important file is fine; a group of thirty trivial files should be collapsed into a single "noise" group.
- Put generated code, lockfiles, snapshots, and formatting-only churn in one final group and describe it in a sentence without walking through it.
- Give each group a name a human would use in conversation, not a path glob.

### 4. Deliver the overview

One short response, roughly 150 words:

**What this PR does and why** — three or four sentences covering the problem that existed before and what is different after. No file or function names. If the motivation is not stated in the description or ticket, say you are inferring it.

**Terms** — only the codebase-specific words needed to follow the rest, one line each, at most three. Skip the section entirely when nothing qualifies.

**Map of the change** — one table row per group: group name, file count, one-line purpose.

Then stop and offer, using the question tool, the first group, a specific group, or a straight pass through all of them.

### 5. Walk through one group at a time

Roughly 120 words per group. Two or three short paragraphs, no sub-headings, no bullet list of files:

- What this part of the system does, in one sentence, only if the reader would otherwise be lost.
- What changed and why, described as behaviour rather than edits. Collapse similar edits into one statement.
- How it connects to the group before or after it, when that link is not obvious.

Add at most one "worth noticing" sentence for a genuinely non-obvious detail: a subtle edge case, an easily missed behaviour change, a compatibility decision. Omit it when there is nothing to say.

Quote code only when one specific line is the point, a few lines at most, using the existing-code reference format. Never paste a hunk.

Pause after each group. Do not continue to the next group unasked, and do not preview what is coming.

### 6. Close the loop

After the last group, trace one concrete scenario (a request, a user action, a migration run) through the changed code in four or five sentences, naming the groups as it passes through them. This is what turns separate group explanations into one understanding.

Add open questions only if something material stayed unresolved. Do not write a recap; the trace is the summary.

## Length and density

Brevity comes from cutting content, not from compressing sentences into fragments. Say less, but say it in plain prose.

Cut on sight:

- Anything the reader can infer from the previous sentence.
- Orientation for parts of the system they already know.
- Restated file names, line counts, and diff statistics.
- Preamble, meta-commentary about the walkthrough, and closing recaps.
- Hedging and qualifiers that do not change the meaning.

If a group turns out to be trivial, one sentence is a complete answer. Detail is available on request; the reader will ask.

## Writing rules

- Plain English. Spell out every acronym on first use. Define codebase jargon before relying on it.
- Complete sentences and normal prose. No arrow chains, no telegraphic fragments, no emoji.
- Explain intent and consequence, never narrate the diff. "Sites without a compatible configuration are now skipped instead of aborting the whole migration" beats "adds a filter to the site list".
- Never invent a rationale. Unverified intent must be marked as inference.
- Calibrate to the reader: if they say they know an area, skip its orientation.

## Anti-patterns

- Padding a group to fill an expected shape when there is little to say.
- Restating file names and line counts as if that were an explanation.
- A wall of text covering every group at once when the reader asked for a walkthrough.
- Slipping into code review: severity labels, nitpicks, or suggested fixes belong in the `code-review` skill.
- Explaining language or framework basics instead of this codebase's specifics.
- Treating generated files as substantive changes.
