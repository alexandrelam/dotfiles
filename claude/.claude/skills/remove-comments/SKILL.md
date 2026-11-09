---
name: remove-comments
disable-model-invocation: true
---

Remove the comments added in the diff or pr. You can keep the one that were updated. But those that were added need to be removed. Ofc keep the ones related to @optin or eslint or things like that.

You must do this in a subagent: launch it with the Agent tool and `model: "sonnet"`, pass it these instructions, and don't edit the files yourself.
