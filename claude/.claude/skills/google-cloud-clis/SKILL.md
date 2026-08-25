---
name: google-cloud-clis
description: Google Cloud CLIs on this machine — gcloud, bq, gsutil, kubectl. Use when a gcloud/bq/gsutil command segfaults or exits 139, when a gcloud alpha or beta command is unavailable, when authentication or reauthentication fails, or when reaching GCP from the shell.
---

# Google Cloud CLIs

Everything comes from one SDK at `~/google-cloud-sdk`: `gcloud`, `bq`, `gsutil`, `kubectl`
(plus pinned `kubectl.1.30`–`1.36`), `gke-gcloud-auth-plugin`, `docker-credential-gcloud`,
`git-credential-gcloud.sh`, `gcloud-crc32c`, `ecp`, `dev_appserver.py`. Run
`gcloud components list` for live state.

There is no `firebase`, `cbt`, `cloud-sql-proxy`, `skaffold`, `istioctl`, or standalone Gemini CLI.

## Segfault on every network call

Symptom: `Segmentation fault: 11`, or exit 139 with **no output whatsoever** — `gcloud projects
list` fails silently that way. Only local commands (`gcloud config list`, `gcloud auth list`,
`bq version`) survive, which makes it look like one tool is broken rather than all of them.

Cause: this machine opts into mTLS client certificates (`GOOGLE_API_USE_CLIENT_CERTIFICATE=1` in
`~/.zprofile`, `context_aware/use_client_certificate=true`), so the TLS handshake loads the Go
`libecp.dylib` to sign with a macOS keychain cert. `cryptography>=47` dies inside that path.

Fix — pin it below 47 in the venv `CLOUDSDK_PYTHON` points at:

```bash
uv pip install --python ~/.gcloud-python/bin/python3 "cryptography<47" pyOpenSSL
```

Anything that later upgrades `cryptography` there reintroduces the crash. The Python version is
not the trigger — 3.13 and 3.14 both crash on `cryptography>=47`, both work below it.

## alpha and beta are absent

`gcloud alpha` and `gcloud beta` fail until `gcloud components install alpha beta`. Reach for this
early when a newer GCP feature seems to have no command.

## Reauthentication needs the user

"Reauthentication failed. cannot prompt during non-interactive execution" is unfixable from an
agent shell. Ask the user to run `gcloud_auth` (`gcloud auth login --update-adc`) in their own
terminal instead of retrying.

Application-default credentials are separate and often still valid, so prefer the Python client
when the CLI is blocked:

```bash
uv run --with google-cloud-bigquery python -c "..."
```

## BigQuery

For the product-analytics tables — region pinning, required partition filters, pricing and
latency models — follow the `query-bigquery-analytics` skill in the work repo.
