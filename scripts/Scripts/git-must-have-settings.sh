#!/bin/bash

echo "🚀 Optimizing your global Git configuration..."

# --- Performance ---
# Use the built-in filesystem monitor to speed up git status
git config --global core.fsmonitor true
# Enable untracked cache for faster file detection
git config --global core.untrackedCache true

# --- Workflow ---
# Automatically set upstream branch on push (no more 'git push -u origin...')
git config --global push.autoSetupRemote true
# Always rebase on pull to keep history clean (linear history)
git config --global pull.rebase true
# Auto-stash changes before rebasing/pulling, then pop them after
git config --global rebase.autoStash true

# --- Code & Conflicts ---
# Use the 'histogram' algorithm for smarter, more readable diffs
git config --global diff.algorithm histogram
# 'Reuse Recorded Resolution': Auto-solve repeated merge conflicts
git config --global rerere.enabled true

# --- Quality of Life ---
# Auto-correct typos (e.g., 'git stats' -> 'git status') after 1 second
git config --global help.autocorrect 10

echo "✅ Done! Git is now faster, smarter, and ready to go."
