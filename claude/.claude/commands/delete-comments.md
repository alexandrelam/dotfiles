Check the changes made in my PR using `gh pr diff`.

Claude-generated code tends to over-comment. Go through every comment that was **added or modified** in the diff and decide whether it earns its place. Default to deletion — when in doubt, delete; a comment has to earn its keep, not the other way around.

- **Delete** comments that just restate what the code already says (e.g. `// increment counter` above `count++`, `// loop through users` above a `for` loop, comments repeating a well-named function/variable).
- **Delete narrative / design-tour comments** too, not just one-liners: block comments that describe the shape of a type, walk through what a function does step-by-step, or explain "how this is organized" when the code itself (good names, small functions, types) already makes that clear on read-through. These read well when freshly written but rot fast and are exactly the kind reviewers flag as unnecessary to maintain.
- **Keep** a comment only if it clears a high bar: a genuinely non-obvious *why*, a workaround for a specific bug/limitation (ideally with a link/ticket), or a subtle invariant that isn't visible from the code alone. If a comment's content could be inferred by a competent reader from the code plus type signatures, delete it even if it's "nice to have."
- If you're unsure whether a comment style is idiomatic here, check other parts of the same file or similar files in the repo for the prevailing convention, and match it rather than guessing.
- Don't touch comments outside the diff (pre-existing code you didn't add/modify), and don't touch non-comment code.

After cleaning up, show me a short summary of what was removed vs. kept (and why for anything kept).
