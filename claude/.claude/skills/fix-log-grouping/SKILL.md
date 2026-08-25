---
name: fix-log-grouping
description: Given a Sentry log message or named-log id, move the high-cardinality values out of the log message into extraProps so Sentry groups it as one issue.
disable-model-invocation: true
---

The argument is a Sentry log — either a pasted message ("Timeout fetching problem list for record RecordId(value=eopb.t5g…)") or a named-log identifier (`FAILED_TO_PARSE_SAML_REQUEST_AS_XML`). It is exploding into one Sentry issue per distinct value. Move the varying values into `extraProps`, leaving a static message.

## Why this works

Sentry groups log-only events (no throwable) on `logentry.message`, which is meant to hold the log statement's *template*. Our loggers pre-interpolate, so it holds a rendering instead, and Sentry falls back to recovering a template by masking event-specific values. That masking covers UUIDs, numbers, timestamps and URLs — anything else lands in the grouping key.

Read the doc comment on `stripDurationsForGrouping` in the custom Sentry appender (grep for `CustomSentryAppender`) before deciding anything; it is the source of truth for this behaviour and for what the appender does with `extraProps`.

## Steps

### 1. Locate the emission site

Grep for the longest run of *literal* words in the message — the parts with no interpolation. For a named-log id, grep the id itself.

Landing on the call site is not the completion criterion: the same interpolated message often appears in sibling branches (a timeout path next to an error path, one per capability). Grep the static fragment across the whole repo and account for every hit, then read the enclosing function. Fix every sibling that has the same defect, not just the one that fired.

### 2. Classify each interpolated value

For every `$value` in the message, decide:

- **Move it** — opaque ids (external vendor ids, string ids), enum-like values with many members, free text, names, paths, hostnames, payloads. These are not masked and are what splits the group.
- **Leave it** — durations (`kotlin.time.Duration` renderings are already stripped by the appender; moving them is redundant churn), bare numbers, timestamps, URLs and UUIDs. Sentry masks these itself.
- **Drop it** — the value is already attached to every event as a `LogProperty` tag or context (check the `LogProperty` enum and the `sentryKind()` mapping in the appender). Re-adding it to `extraProps` duplicates it. The request-scoped ids and the organization/principal properties are the common cases.

A value in the **leave** bucket is a reason to narrow the change, not to skip the skill: if the only varying value is a duration or a UUID, the grouping is already fine — say so and stop rather than editing.

### 3. Rewrite the call

Keep the message a constant string with no interpolation, and pass the moved values via `extraProps`:

```kotlin
logger.warnLog(
    extraProps = mapOf(
        "external_record_id" to externalRecordId.value,
        "user_uuid" to user.uuid.toString(),
    ),
) { "Timeout fetching problem list. Returning empty list" }
```

Rules:

- `extraProps` keys are `snake_case`, and name the value rather than the variable.
- The map is `Map<String, String?>`; null values are dropped, so no need to filter them.
- When two or more sibling call sites move the same set of values, extract one private helper returning the map rather than repeating it. Below that, inline the `mapOf`.
- Unwrap value classes (`.value`, `.stringValue`) so the prop holds the id, not `RecordId(value=…)`.
- Leave the message readable on its own — "Timeout fetching problem list. Returning empty list", not a bare noun. It becomes the Sentry issue title.
- Do not add a `name =` to fix grouping. It goes to the app's custom Sentry context like any other prop and has no effect on the grouping key; add one only if the user wants the statement searchable by id.

### 4. Verify

Compile the owning Gradle module (`./gradlew :features:<feature>:impl:compileKotlin`) and let the pre-commit ktlint hook run at commit time. There is nothing to unit-test here.

Then report, per call site: which values moved, which were left to Sentry's own masking, and which were dropped as already-tagged. Flag explicitly that saved Sentry searches matching the old message text will break — the user may have alerts on them.
