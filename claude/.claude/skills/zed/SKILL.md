---
name: zed
description: Open the current worktree in Zed so the user can read the diff.
---

Run `zed <git root of the current worktree>`; if the changes to review are only in local unpushed commits you made this session, first `git reset --soft` them and `git restore --staged .` so they show in Zed's git diff panel.
