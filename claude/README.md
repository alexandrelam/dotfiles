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

## Mods

`local-plugins/open-in-intellij` is a Claude desktop app mod: clicking a file link in the
transcript opens it in IntelliJ IDEA at its line instead of the in-app file pane. Stow
links it into `~/.claude/local-plugins/`, which also holds unpublished work plugins. Then
register it as a local marketplace and install it:

```
claude plugin marketplace add ~/.claude/local-plugins/open-in-intellij
claude plugin install open-in-intellij@local-mods
```

## Work-specific content

These commands grew out of a work codebase, so they are kept de-identified: no company or
product names, ticket ids, PR links, or colleague handles. Structural references that the
commands need in order to be useful — repo-relative paths, in-house ORM method names, DB
names — are still there.

Two per-project context dumps are gitignored instead, because they are made entirely of
ticket ids, PR links and branch names and there is no version of them worth publishing.
They stay on disk behind the symlink, so a fresh clone has two fewer commands than my
machine does.
