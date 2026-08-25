Set up my local env to view backend trace spans in Jaeger.

The backend exports OTLP traces to a **local Jaeger** in dev: the app's own SDK exports over OTLP gRPC to `localhost:4317`, and Jaeger's UI is `localhost:16686`. There is no committed automation for this — this command is it.

## Setup (do these in order; each is idempotent)

1. **Docker runtime up.** Jaeger runs in Docker via Colima. Check `colima status`; if it's not running, `colima start`.

2. **Jaeger container.** Check `docker ps` for a `jaeger` container.
   - Running → done.
   - Exists but stopped → `docker start jaeger`.
   - Absent → start it with OTLP enabled:
     ```bash
     docker run -d --name jaeger -e COLLECTOR_OTLP_ENABLED=true \
       -p 16686:16686 -p 4317:4317 -p 4318:4318 \
       jaegertracing/all-in-one:latest
     ```
   Done when `http://localhost:16686` responds.

3. **Enable exports.** In `server/configuration/src/main/resources/application.user.conf` (gitignored, per-dev), ensure:
   ```
   telemetry {
     enabled = true
     disableExports = false
   }
   ```
   The dev default is `disableExports = true` (spans stay in-process), so this override is what makes traces leave the process.

## Run & view

- Run the server in dev: the `server [run]` IntelliJ config, or `./gradlew :app:run` from `server/`.
- Trigger a request, then open `http://localhost:16686` and select the backend service.

## Notes

- Dev samples every request (`otel.traces.sampler.arg=1.0` in `server/buildSrc/src/main/kotlin/JvmArgs.kt`), so no sampling tweak is needed.
- The same flow is documented in the "Open Telemetry traces" section of `server/README.md`.
- Edits to `application.user.conf` only take effect after restarting the server.

$ARGUMENTS
