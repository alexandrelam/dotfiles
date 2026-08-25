# gh stack — command reference

Detailed flags, behavior, and output per command. See `SKILL.md` for the non-interactive rules, workflows, and exit codes. All commands assume a non-interactive terminal.

## `init` — create a stack

```
gh stack init [-b|--base <branch>] <branches...>
```

- Requires at least one branch argument (no args → prompt).
- Creates missing branches from the trunk; adopts existing ones automatically. Names are verbatim (slashes kept).
- `--base` sets the trunk (default: repo default branch).
- Checks out the last branch listed. Enables `git rerere`.

## `add` — add a branch on top

```
gh stack add [-m <msg> [-A|-u]] <branch>
```

- Must be run from the topmost branch (or trunk on an empty stack); otherwise exit 5 (`can only add branches on top of the stack`) — `gh stack top` first.
- `-m` commits with the message; `-A` stages all incl. untracked, `-u` stages tracked only (mutually exclusive, each needs `-m`).
- With `-m` but no branch name, the name is auto-generated as `MM-DD-slug`.
- On a branch with no commits (e.g. right after `init`), `add -Am` commits on the current branch instead of creating one.
- Without `-Am`, uncommitted working-tree changes carry over to the new branch (standard git). Commit/stash first for a clean start.

## `push` — push branches, no PRs

```
gh stack push [--remote <name>]
```

- Pushes all active (non-merged, non-queued) branches in one non-atomic multi-ref push with per-branch `--force-with-lease`. If one ref is rejected, others may still update — fix and rerun.
- Does not touch PRs. Output: `Pushed N branches`.

## `submit` — push + create/update PRs

```
gh stack submit --auto [--open] [--remote <name>]
```

- `--auto` (required): auto-titles new PRs. Single commit → subject as title, body as body; multiple → humanized branch name.
- `--open`: mark new and existing PRs ready for review (default is draft).
- Pushes each active branch sequentially (`--force-with-lease`, non-atomic); on rejection, earlier pushes/PR updates persist — fix and rerun.
- Creates a PR per branch lacking one (base = first non-merged ancestor), then links them as a GitHub **Stack**.
- If the whole stack is already merged, forks unmerged branches into a new stack on the trunk.
- Exit 9 if stacked PRs aren't enabled on the repo.
- Output: `Created PR #N for <branch>`, `PR #N for <branch> is up to date`, `Pushed and synced N branches`.

## `sync` — fetch, rebase, push, sync state (routine command)

```
gh stack sync [--remote <name>] [--prune]
```

In order: fetch → reconcile the remote stack (pull down branches added to the GitHub stack; abort if diverged) → fast-forward trunk → cascade-rebase onto updated parents (only if trunk moved; auto-handles merged PRs; on conflict restores all branches and exits 3) → atomic push → sync PR state → link open PRs into the GitHub stack (2+ PRs, additive only, never opens PRs) → prune (only with `--prune` non-interactively).

Key output lines: `✓ Fetched…`, `✓ Trunk <b> fast-forwarded`/`already up to date`, `✓ Rebased <b> onto <base>`, `✓ Pushed N branches`, `✓ PR #N (<b>) — Open`, `Merged: #N`, `✓ Stack created/updated on GitHub with N PRs`, `✓ Pruned <b> (merged)`, `ℹ Sync aborted — no changes were made` (divergence).

## `rebase` — cascade-rebase (finer control than sync)

```
gh stack rebase [--upstack|--downstack|--no-trunk] [--continue|--abort] [--remote <name>] [branch]
```

- Default: rebase the whole stack. `--upstack` = current→top; `--downstack` = trunk→current; `--no-trunk` = branches onto each other only, no fetch/trunk rebase.
- `--continue` after `git add`-ing resolved files; `--abort` restores all branches.
- Detects merged PRs and uses `--onto` to replay correctly. `rerere` auto-resolves previously-seen conflicts.
- `[branch]` targets a branch (default: current).

## `view` — inspect the stack

```
gh stack view --json
```

`--json` (required) prints to stdout:

```json
{
  "trunk": "main",
  "currentBranch": "api",
  "branches": [
    {
      "name": "auth", "head": "abc…", "base": "def…",
      "isCurrent": false, "isMerged": true, "isQueued": false, "needsRebase": false,
      "pr": { "number": 42, "url": "https://…/pull/42", "state": "MERGED" }
    }
  ]
}
```

Per branch: `name`, `head` (HEAD SHA), `base` (parent HEAD at last sync), `isCurrent`, `isMerged`, `isQueued` (in merge queue), `needsRebase` (base not an ancestor), `pr` (omitted if none; `state` = `OPEN`|`MERGED`|`QUEUED`).

## Navigation

```
gh stack up [n] | down [n] | top | bottom | trunk
```

`up` = away from trunk, `down` = toward it. Clamps to bounds; skips merged branches when navigating from active ones. `bottom` = first non-merged branch above trunk. Fully non-interactive.

## `checkout` — switch to a stack

```
gh stack checkout <stack-number | pr-number | pr-url | branch>
```

- A bare number resolves as **stack number first**, then PR number, then branch name.
- Stack/PR number or URL → fetches from GitHub, pulls branches, sets up the stack locally; if it already matches locally, just switches.
- Branch name → resolves against locally tracked stacks only (always safe).
- If local and remote stacks differ in composition → unbypassable prompt. Run `gh stack unstack --local` first, then retry.

## `merge` — merge stacked PRs (`gh pr merge` won't work)

```
gh stack merge [<pr-number>|<stack-number>] --yes [--squash|--rebase|--merge|--merge-method <m>]
```

- `--yes` (required non-interactively). Merges the whole stack bottom-to-top, **atomically** — if any PR can't merge, none do, and the reason is reported.
- Scope with a PR number (up to and including that PR) or a stack number (no local checkout needed).
- Method: `--squash`/`--rebase`/`--merge` or `--merge-method`; omitted → last-used method.
- Only open + not-draft is checked; merge requirements can't be bypassed for stacks.
- If the base uses a **merge queue**, the stack is added to the queue instead: the queue picks the method (yours is ignored with a warning), and PRs may land in separate groups.

## `link` — link PRs into a GitHub stack, no local state

For branches managed by other tools (jj, Sapling, git-town).

```
gh stack link [--base <branch>] [--open] [--remote <name>] <stack-number | branch-or-pr> <branch-or-pr> [...]
```

- Args in stack order, bottom to top. Each is tried as a PR number first, else a branch name.
- First arg = an existing stack number → remaining args appended to that stack's top (args already in it skipped; args in a different stack rejected).
- Branch args are pushed (non-force, atomic). Missing PRs are created with auto-titles and correct base chaining; wrong base branches on existing PRs are corrected.
- Creates the stack if none exists, else updates additively (never removes PRs). Touches no local state.
- Output: `Pushing N branches…`, `Found PR #N…`, `Created PR #N for <b> (base: <base>)`, `Updated base branch for PR #N…`, `Created/Updated stack…`.

## `unstack` — tear down a stack (never deletes PRs/branches)

```
gh stack unstack [<stack-number>] [--local]
```

- No arg → the active stack (containing the current branch): unstacks on GitHub + removes local tracking.
- `<stack-number>` → unstacks that stack on GitHub via API from anywhere (tracked or not); removes local tracking too if present. Unknown number → exit 2 (`not found on GitHub`).
- `--local` → remove local tracking only, never contacts GitHub. Combining `--local` with an untracked number is an error.

## `modify`

Not used by this skill. If a repo is left in `modify` recovery (exit 10), run `gh stack modify --abort` to restore the pre-modify state.
