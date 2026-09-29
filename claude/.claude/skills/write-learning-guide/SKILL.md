---
name: write-learning-guide
description: Explore a codebase area and write a multi-chapter learning guide into the Obsidian Learn folder, then verify-and-improve it in passes.
disable-model-invocation: true
argument-hint: "Which system or feature should the guide explain?"
---

Write a **guide**: a folder of short chapters in simple English that lets a newcomer understand one system — why it exists, how data flows through it, the concepts inside it, and how to change it safely. Every sentence in the guide is a **claim** about the code; the job is to make every claim true.

Guides live in `~/Projects/work-obsidian/Learn/<topic-slug>/`, whatever the current working directory. Folder shape, chapter menu and style rules are in [FORMAT.md](FORMAT.md) — read it before step 4.

## 1. Scope

- Pick a kebab-case `<topic-slug>`. If `Learn/` already has a folder for this topic, update that one instead of creating a second.
- From the repo, record the default branch and the short SHA of its remote head (fall back to `HEAD`) for the "Written on … based on …" line.
- Read the area's own `README.md`, `AGENTS.md` / `CLAUDE.md`, and any README of the systems it depends on (data pipelines, infra modules).
- Read one existing guide in `Learn/` to match the house style.

Done when: slug chosen, SHA recorded, the area's docs read.

## 2. Explore

Launch 3 `explore` subagents **in parallel**, background, "very thorough", each told not to modify files and to return a markdown report with file paths and flagged uncertainties:

1. **Flow** — where the data or request originates, every hop it takes (services, queues, databases, pipelines, schedules), and why it's built that way.
2. **Concepts** — every domain object and page/feature: what it is, why it exists, how it relates to the others, where it lives.
3. **Architecture** — stack, code layout and conventions, auth, how it talks to other systems, environments and deploy, local dev, documented gotchas.

Tailor each prompt with the paths from step 1. While they run, read the core files yourself (main models, main entry points) so you can judge their reports.

Done when: all three reports are back.

## 3. Spot-check

Subagent reports contain plausible errors. Before drafting, verify in code the **load-bearing** claims: the ones the guide's big-picture diagram and summary depend on (which path is primary, what triggers what, schedules, who runs what). Correct or drop anything that fails.

Done when: every claim in the planned summary paragraph and big-picture diagram has been read in code.

## 4. Draft

Write the README and chapters per [FORMAT.md](FORMAT.md). If you worked on this area in the current conversation (a ticket, a PR), use it as the worked example in the "changing it" chapter.

Done when: every chapter in the chosen menu exists, and every chapter links to the next.

## 5. Verify-and-improve passes

Run these passes in order. Each pass edits the guide; tell the user what each pass changed.

1. **Accuracy.** List every claim you did not read directly in code (meanings of fields, flag semantics, example channels/teams, counts, "X is only used for Y"). Check each with Grep/Read. Fix it, soften it to what the code shows ("I found no code that enforces this"), or delete it. Invented specifics (a Slack channel, a team name, an example that looks real) go.
2. **Completeness.** Answer the newcomer's questions and add what's missing: *What happens when I click the main action?* (sequence diagram) — *Why can't I find my X?* (troubleshooting checklist) — *How do I add a field / change it safely?* — *What breaks if I get the order wrong?* — *How do I run it locally?*
3. **Readability.** Reread as someone new to the company: expand jargon on first use, replace arrow-chains and fragments with sentences, cut paragraphs a reader can skip, make sure examples are labelled as examples.

Repeat pass 1 on any text added by passes 2–3.

Done when: a full accuracy pass over the final text finds nothing to change.

## 6. Check and report

- Every `[[wikilink]]` resolves to a file in the folder.
- Every file path mentioned exists in the repo (spot-check the file map with `ls`/Glob).

Reply with: the folder path, the chapter list with one line each, what each pass changed, and any claim you kept but could not fully verify.
