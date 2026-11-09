---
name: readability-pass
description: Readability pass on a PR diff — from code flow and organization down to names, so a cold reader gets it at first read.
disable-model-invocation: true
---

Read the PR diff (`git diff $(git merge-base origin/<base> HEAD)`, base from the PR or `master`) as a **cold reader**: a teammate who has never seen the PR, the ticket or this conversation. Every spot where they'd pause, re-read, decode, or jump around to follow what happens is a finding.

Look at two levels:

- **Shape**: can the cold reader follow the flow top to bottom? Is each piece of logic where they'd look for it, at one level of abstraction, with one clear owner? Would a different split, order or data shape make the code obvious?
- **Lines**: names, expressions and test data that need decoding.

Fix the code, never add comments to explain it. Behaviour stays identical. Bugs and reuse are out of scope (`/code-review`, `/simplify`).

1. Read every changed function whole, and enough of its callers and callees to see the flow. Done when no hunk is read out of context.
2. Report a numbered list, shape findings first, then lines, highest reader impact first: `path:line`, why a cold reader stumbles, before/after snippet (a sketch of the new structure for shape findings). End with what's already clear and was left alone. Ask which to apply; edit nothing before the answer.
3. Apply the chosen items, then run only the formatter, linter, type checker and tests covering touched files. Mention anything the change left newly untested. Commit only when asked.
