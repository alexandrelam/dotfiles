Query my local Postgres dev database.

## Connection

- Host/port: `localhost:5432`, no password.
- Auth: use my OS user (the default). The `postgres` role does NOT exist — never pass `-U postgres`.
- Databases: the main app db holds most tables, alongside `global`, `medical` and `postgres`.
- The main db is the "regional" db (Flyway `flyway-regional` migrations under the regional migrations directory).

## How to run

```bash
psql -h localhost -p 5432 -d <db> -P pager=off -c "SELECT ...;"
```

- `-P pager=off` keeps output inline.
- For anything that writes, wrap it in `BEGIN; ... COMMIT;` and add `-v ON_ERROR_STOP=1`.
- Multi-statement / migrations: put SQL in a file and use `-f file.sql`.

## Finding things

- Which db has a table: `psql -h localhost -p 5432 -d <db> -tAc "SELECT to_regclass('public.<table>');"`
- List a table's columns: query `information_schema.columns WHERE table_name='<table>'`.
- Schema source of truth for a table is its Flyway migration; jOOQ types live in the generated `jooq.generated.regional.public.*` package.

## Rules

- Read-only by default. Before any mutation, back up affected rows (e.g. `CREATE TABLE ..._backup AS SELECT ...`) and tell me what you'll change.
- Never run destructive DDL (`DROP`/`TRUNCATE`) without asking.

$ARGUMENTS
