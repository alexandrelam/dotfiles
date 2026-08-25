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

## Work-specific files

Some commands and skills reference an employer's codebase, tickets and schema. They are
listed in the repo `.gitignore`, so they live in this directory on my machine and stay
reachable through the symlink, but never get published. Expect a fresh clone to have
fewer files here than `~/.claude` does.
