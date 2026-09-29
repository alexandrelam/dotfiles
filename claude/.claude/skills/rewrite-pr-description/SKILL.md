---
name: rewrite-pr-description
description: Rewrite the current PR body from this layer's diff — short, simple English, behaviour only.
disable-model-invocation: true
---

Rewrite the GitHub pull-request body so a reviewer grasps the product change without dropping off. Keep it concise and in simple English. The goal is not to make the reader dropoff because there is too much text. Focus on the behaviour / product changes. No test section.

## 1. Read this layer only

Run:

```bash
python3 "$HOME/.claude/skills/rewrite-pr-description/scripts/this-layer.py"
```

Treat that output as the whole change set. The script diffs this branch against its gh-stack parent (the branch below, or the trunk at the bottom). If the branch is not in a stack, it diffs against the GitHub PR base.

Completion: every later sentence maps to this layer's diff. Parent-layer work stays out, even when GitHub's Files tab shows it (common when stacked PRs all target the default branch).

If the script exits non-zero, stop and say why. Do not switch branches or create a worktree.

If `github_base_matches_parent` is false, say so in the chat reply only — not in the PR body.

## 2. Write the new body

Put these at the top, and nothing else from the old template:

- Ticket lines: keep existing `Fixes` / `Closes` / `Related to` / `Parent` lines. If they are missing, add them from the PR title, branch name, or Linear IDs already in the old body. Link each as `[ABC-1234](https://linear.app/<workspace>/issue/ABC-1234)`, taking `<workspace>` from Linear links already in this PR or in the repo's recent PRs. Prefer `Related to` unless the old body already used `Fixes` or `Closes`. Never drop ticket links to shorten the body.
- `Depends on: #N` when `parent_pr` is set

Then the description. No headings unless there is a warning.

**Opening** — 1 to 3 short sentences. What someone can do or will see after this lands, that they could not before. No file names, no "this PR", no implementation.

**Bullets** — only when there are two or more distinct behaviours. Maximum five. One line each. Drop any bullet that repeats the opening.

**Warning** — add one only if it is really really necessary: irreversible data change, broken existing workflow, or a behaviour reviewers will miss and ship by accident. Prefix `Warning:`. Skip otherwise.

Keep screenshots already in the body, after the text. Keep a trailing stack-navigation table if one is already there.

Cut on sight: test plans, file lists, implementation notes, restated commit messages, hedging, "additional context", generated-file noise, anything a reader can skip without missing a behaviour.

Example of a finished body:

```
- Related to: [ABC-1234](https://linear.app/acme/issue/ABC-1234)
- Depends on: #12345

Users can export a report as a PDF from the report page.

- Export sits next to Share.
- The file uses the same layout as the printed note.

Warning: the PDF includes sections that are hidden on screen.
```

Skip the warning and the extra bullets when they are not earned. Skip `Depends on` when there is no parent PR. Never skip the ticket line when a Linear ID is known.

Completion: a busy reader finishes the body in a few seconds and knows the product change.

## 3. Apply

```bash
gh pr edit <number> --body-file <path>
```

Do not change the title. Reply with the PR URL and the new body.
