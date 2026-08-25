# Claude Code commands and skills

Slash commands (`~/.claude/commands`) and skills (`~/.claude/skills`) for Claude Code.

## Install

`~/.claude` holds a lot of generated state (sessions, history, caches), so only the
two directories below are linked, not the whole folder:

```
cd dotfiles
stow claude
```

If `~/.claude/commands` or `~/.claude/skills` already exist as real directories, move
them aside first — stow refuses to overwrite them.

## Work-specific content

These commands grew out of a work codebase, so they are kept de-identified: no company or
product names, ticket ids, PR links, or colleague handles. Structural references that the
commands need in order to be useful — repo-relative paths, in-house ORM method names, DB
names — are still there.

Two per-project context dumps are gitignored instead, because they are made entirely of
ticket ids, PR links and branch names and there is no version of them worth publishing.
They stay on disk behind the symlink, so a fresh clone has two fewer commands than my
machine does.
