# Guide format

Reference example: the most recent multi-chapter guide already in `Learn/`.

## Folder

```
Learn/<topic-slug>/
  README.md
  01-<why-it-exists>.md
  02-<the-big-picture>.md
  ...
  NN-file-map.md
```

## README.md

In this order:

1. `# <Title>` and one sentence saying what the guide covers.
2. `Written YYYY-MM-DD, based on \`<default-branch>\` at \`<sha>\`.`
3. `## Read in this order` — numbered list of `**[[NN-slug]]** — one-line promise`.
4. `## One-paragraph summary` — the whole system in ~120 words, plain English, following one piece of data from start to end.
5. `## Glossary (quick)` — table `| Word | Meaning in one line |`, every term a newcomer meets.

## Chapter menu

Pick the chapters the system earns, keep the order, and rename to fit. 8–12 chapters is typical.

| Chapter | Holds |
| --- | --- |
| Why it exists | The problem, what the system does, who uses it, a concrete example scenario |
| The big picture | One mermaid flowchart of the main flow, then the steps in words, then "why this design" |
| Core concepts | Each domain object: what, why, relations, where it lives. A mermaid diagram of relations |
| The main screen / API | What you see and can do; a sequence diagram of the main action |
| Secondary workflows | Batch jobs, reports, admin features |
| Data pipeline | Queues, warehouses, transformation layers, schedules, tests, schema rules |
| Storage and read paths | Where data is read from, switches between sources, troubleshooting "I can't find X" |
| Changing it | End-to-end checklist for the common change, with a real worked example, merge order, CI checks, "things people forget" |
| Architecture and local dev | Stack, codebase particularities, auth, deploy, local setup commands |
| Gotchas | Numbered traps, each one bold sentence + why + what to do |
| File map | Tables of `What | Path`, grouped by layer, paths from repo root |

## Chapter template

```markdown
# NN — Title

<1–3 sentence intro: what this chapter answers>

## <Sections>
...

Next: [[NN+1-slug]]
```

The file map ends with `Back to [[README]]`.

## Style

- Simple English. Short sentences. Define a term the first time it appears.
- Explain **why**, not only what: every design choice gets its reason.
- Concrete example scenarios, labelled as examples.
- Mermaid for flows (`flowchart LR`) and interactions (`sequenceDiagram`); keep node labels short.
- Tables for enumerable facts; prose for explanations.
- Short code excerpts (≤ 10 lines) only where they make a concept click (a config block, a one-line switch).
- Name files and symbols in backticks, with paths from the repo root.
- Link PRs as markdown links.
