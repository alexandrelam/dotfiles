---
name: pr-internals
description: Explains the technical internals of a PR — code flow, mechanisms and data model — so the user understands how it works. Use when the user asks how a PR works under the hood, not for a review or a file-by-file walkthrough.
disable-model-invocation: true
argument-hint: "[PR number or URL, defaults to the current branch's PR]"
---

Explain how the given PR (or the current branch's, via `gh pr view` / `gh pr diff`) works technically. Explain, don't review. Read the diff, then the surrounding code it relies on (callers, models, schemas, tests) until you can explain it correctly.

Answer in plain English, citing `path:line` for every claim:

1. **Flow**: the path of one request or event from its entry point to its effects, step by step.
2. **Mechanisms**: the non-obvious parts, such as batching, transactions, caching, state machines, permissions, codegen and async work: what each one does and why it's needed.
3. **Data model**: the entities, fields, enums, relations and invariants that are added or used, and how they change. Draw a small diagram if it helps.
4. **Gotchas**: what someone changing this code later must know.

Keep it short. Skip what's obvious from reading the diff.
