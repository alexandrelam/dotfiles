# Torrenting Stack

This folder contains a small Docker-based torrent stack with:

- `gluetun` for VPN networking
- `qBittorrent` for downloading
- `Prowlarr` for indexer management

## Files

- `docker-compose.yaml` -> stack definition
- `.env.example` -> required environment variables
- `.gitignore` -> prevents committing the live `.env`

Create a local `.env` from the example before starting the stack:

```bash
cp .env.example .env
```

Then fill in your real VPN credentials.

## Services

### `gluetun`

`gluetun` provides the VPN connection and exposes the network ports used by qBittorrent.

Published ports:

- `8080` -> qBittorrent Web UI
- `6881/tcp`
- `6881/udp`

### `qbittorrent`

`qbittorrent` shares `gluetun`'s network namespace:

```yaml
network_mode: "service:gluetun"
```

That routes qBittorrent traffic through the VPN and keeps qBittorrent from publishing its own ports directly.

Storage:

- `./config` -> qBittorrent config
- `./downloads` -> downloaded files

Web UI:

- `http://localhost:8080`

### `prowlarr`

`prowlarr` manages torrent/Usenet indexers in one place and can sync them to other `*arr` apps later.

Storage:

- `./config/prowlarr` -> Prowlarr config

Web UI:

- `http://localhost:9696`

## Start The Stack

```bash
docker compose up -d
```

Check status:

```bash
docker compose ps
```

View logs:

```bash
docker compose logs -f
```

## Current Layout

```text
host
 ├─ localhost:9696 -> prowlarr
 └─ localhost:8080 -> gluetun -> qbittorrent
```

Inside Docker:

- Prowlarr reaches qBittorrent at `gluetun:8080`

Do not use `localhost` inside Prowlarr's qBittorrent download client settings. In this Compose setup, `localhost` inside the Prowlarr container refers to the Prowlarr container itself.

## Configure qBittorrent In Prowlarr

In the Prowlarr UI:

1. Open `Settings`
2. Open `Download Clients`
3. Click `+`
4. Choose `qBittorrent`

Use:

- Name: `qBittorrent`
- Host: `gluetun`
- Port: `8080`
- Use SSL: `false`
- Username: your qBittorrent Web UI username
- Password: your qBittorrent Web UI password
- Category: optional, for example `prowlarr`

Then click `Test` and `Save`.

## Configure Indexers In Prowlarr

In the Prowlarr UI:

1. Open `Indexers`
2. Click `Add Indexer`
3. Add only indexers you actually use

Recommended first approach:

- 1 public test indexer
- 1 good general private tracker you already belong to
- 1 niche tracker only if you need one for anime, music, or books

## Security Note

Store secrets only in the local `.env`. Do not commit it.

If these VPN credentials were ever committed anywhere before, rotate them. Moving them to `.env` stops future exposure in the Compose file, but it does not remove them from old git history.
