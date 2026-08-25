---
name: pr-walkthrough
description: Explain a pull request in plain English, step by step. Summarizes what the PR does and why, defines unfamiliar concepts, groups the changed files logically, and walks through one group at a time so the reader understands the whole change. Use when the user asks to walk through, explain, understand, or be onboarded to a PR or its diff, rather than to critique it.
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
- Linked tickets and issues. Branch names in this repo often carry a ticket ID (e.g. `lam/CLI-1112/...`); if a Linear or issue link is available, read it for the problem statement.
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

Lead with a single response containing:

**What this PR does** — two to four sentences in plain English. No file names, no function names.

**Why it exists** — the problem or limitation that existed before, and what is different after. If the PR description or ticket explains the motivation, use it; if the motivation is not stated anywhere, say that you are inferring it from the code.

**Concepts you need** — only the domain terms, acronyms, or internal abstractions required to follow the rest. Two or three sentences each. Skip anything a working engineer already knows; the goal is to explain this codebase's vocabulary, not programming.

**Map of the change** — a short table of the groups with a file count and a one-line purpose for each.

Then stop and offer the choice of where to go next, using the question tool: the first group, a specific group, or a straight-through pass over all groups.

### 5. Walk through one group at a time

For each group, in the order established by the map:

- **What this part of the system is** — enough orientation that the change makes sense, for a reader who has never opened these files.
- **What changed** — the substance, in prose. Describe behaviour, not lines. Group several similar edits into one statement instead of listing them.
- **Why it changed this way** — the reasoning, including alternatives that the code implies were rejected.
- **How it connects** — the link back to the contract and forward to whatever consumes it, so the groups form one story.
- **Worth noticing** — at most two or three genuinely non-obvious details: a subtle edge case, a behaviour change that is easy to miss, a backwards-compatibility decision.

Quote code only when a specific line is the point being made, and keep it to a few lines using the existing-code reference format. Never paste a whole hunk.

At the end of each group, pause and check whether the reader wants to dig in, ask questions, or continue to the next group. Do not steamroll through all groups after being asked for one.

### 6. Close the loop

After the last group, give a short end-to-end trace: follow one concrete scenario (a request, a user action, a migration run) through the changed code from entry point to result, naming the groups as it passes through them. This is what turns a list of file explanations into an understanding of the change.

Finish with:

- **Open questions** — anything you could not determine, and what you would need to read or ask to resolve it.
- **If you only remember three things** — the three points that matter most.

## Writing rules

- Plain English. Spell out every acronym on first use. Define internal jargon before relying on it.
- Complete sentences and normal prose. No arrow chains, no telegraphic fragments, no emoji.
- Explain intent and consequence, never narrate the diff. "Sites without a compatible encounter profile are now skipped instead of aborting the whole migration" beats "adds a filter to the site list".
- Never invent a rationale. Unverified intent must be marked as inference.
- Calibrate to the reader's questions: if they say they know an area, skip the orientation for it.

## Anti-patterns

- Restating file names and line counts as if that were an explanation.
- A wall of text covering every group at once when the reader asked for a walkthrough.
- Slipping into code review: severity labels, nitpicks, or suggested fixes belong in the `code-review` skill.
- Explaining language or framework basics instead of this codebase's specifics.
- Treating generated files as substantive changes.
