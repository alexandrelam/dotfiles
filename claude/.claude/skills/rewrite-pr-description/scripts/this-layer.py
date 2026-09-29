#!/usr/bin/env python3
"""Print this pull request's layer-only commits, body, and diff.

Parent is the gh-stack branch below (or the trunk at the bottom). If the current
branch is not in a stack, parent is the GitHub PR base.

When GitHub's PR base is that parent, the diff comes from `gh pr diff` (what
reviewers see). When they disagree — stacked PRs that still target trunk —
the diff is `git diff parent...HEAD` so parent-layer work stays out.
"""

from __future__ import annotations

import json
import os
import subprocess
import sys

ENV = {
    **os.environ,
    "GH_PAGER": "cat",
    "GIT_PAGER": "cat",
    "PAGER": "cat",
}


def run(args: list[str], *, check: bool = True) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        args,
        check=check,
        text=True,
        capture_output=True,
        env=ENV,
    )


def die(message: str, proc: subprocess.CompletedProcess[str] | None = None) -> None:
    print(message, file=sys.stderr)
    if proc is not None:
        err = (proc.stderr or proc.stdout or "").strip()
        if err:
            print(err, file=sys.stderr)
    sys.exit(1)


def gh_pr() -> dict:
    proc = run(
        [
            "gh",
            "pr",
            "view",
            "--json",
            "number,title,body,url,baseRefName,headRefName,commits",
        ],
        check=False,
    )
    if proc.returncode != 0:
        die("No pull request for the current branch.", proc)
    return json.loads(proc.stdout)


def gh_stack() -> dict | None:
    proc = run(["gh", "stack", "view", "--json"], check=False)
    if proc.returncode == 0:
        return json.loads(proc.stdout)
    # 2 = not in a stack. Anything else (not installed, auth, lock) is ignored
    # so a missing extension still falls back to the GitHub PR base.
    return None


def stack_parent(stack: dict) -> tuple[str, int | None]:
    """Return (parent_branch, parent_pr_number_or_None) for the current layer."""
    branches: list[dict] = stack["branches"]
    current = next((b for b in branches if b.get("isCurrent")), None)
    if current is None:
        name = stack.get("currentBranch")
        current = next((b for b in branches if b.get("name") == name), None)
    if current is None:
        die("gh stack view succeeded but no current branch was marked.")

    idx = branches.index(current)
    if idx == 0:
        return stack["trunk"], None

    parent = branches[idx - 1]
    pr = parent.get("pr") or {}
    number = pr.get("number")
    return parent["name"], int(number) if number is not None else None


def resolve_ref(name: str) -> str:
    for candidate in (name, f"origin/{name}"):
        proc = run(["git", "rev-parse", "--verify", candidate], check=False)
        if proc.returncode == 0:
            return candidate
    die(
        f"Cannot resolve parent ref {name!r}. Fetch it "
        f"(`git fetch origin {name}`) and rerun."
    )
    raise AssertionError


def github_commits(pr: dict) -> str:
    lines = []
    for commit in pr.get("commits") or []:
        oid = commit.get("oid") or ""
        headline = commit.get("messageHeadline") or ""
        lines.append(f"{oid[:12]} {headline}".strip())
    return "\n".join(lines)


def main() -> None:
    pr = gh_pr()
    stack = gh_stack()
    github_base: str = pr["baseRefName"]

    if stack is not None:
        parent, parent_pr = stack_parent(stack)
        in_stack = True
    else:
        parent, parent_pr = github_base, None
        in_stack = False

    base_matches = github_base == parent

    if base_matches:
        diff_proc = run(["gh", "pr", "diff", str(pr["number"])], check=False)
        if diff_proc.returncode != 0:
            die("gh pr diff failed.", diff_proc)
        diff_text = diff_proc.stdout
        commits_text = github_commits(pr)
        parent_ref = github_base
    else:
        parent_ref = resolve_ref(parent)
        commits_proc = run(
            ["git", "log", "--format=%h %s", f"{parent_ref}..HEAD"],
            check=False,
        )
        if commits_proc.returncode != 0:
            die("git log failed.", commits_proc)
        diff_proc = run(
            ["git", "diff", "--no-color", "--find-renames", f"{parent_ref}...HEAD"],
            check=False,
        )
        if diff_proc.returncode != 0:
            die("git diff failed.", diff_proc)
        diff_text = diff_proc.stdout
        commits_text = (commits_proc.stdout or "").rstrip()

    if not (diff_text or "").strip():
        die(
            f"Empty diff against {parent}. This layer has no changes to describe."
        )

    print("pr:", pr["number"])
    print("url:", pr["url"])
    print("title:", pr["title"])
    print("head:", pr["headRefName"])
    print("in_stack:", "true" if in_stack else "false")
    print("parent:", parent)
    print("parent_ref:", parent_ref)
    print("parent_pr:", parent_pr if parent_pr is not None else "")
    print("github_base:", github_base)
    print("github_base_matches_parent:", "true" if base_matches else "false")
    print("diff_source:", "gh pr diff" if base_matches else "git diff parent...HEAD")
    print()
    print("## commits")
    print(commits_text or "(none)")
    print()
    print("## existing body")
    print(pr.get("body") or "")
    print()
    print("## diff")
    print(diff_text, end="" if diff_text.endswith("\n") else "\n")


if __name__ == "__main__":
    main()
