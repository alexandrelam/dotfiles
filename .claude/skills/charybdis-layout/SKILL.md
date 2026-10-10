---
name: charybdis-layout
description: Change the layout of Alexandre's Charybdis Mini keyboard (Vial firmware) and keep the dotfiles in sync — remap keys, add combos/tap dance/key overrides, change layers, refresh layers.html, and when code changes are needed, build and flash the firmware. Use when the user asks to change, move, add or remove a key, layer, combo or shortcut on the Charybdis / keyboard / Vial, to update layers.html, or to rebuild or flash the keyboard.
---

# Update the Charybdis Mini layout

The user writes in simple English (not their first language). Answer in simple English, short sentences.

## Facts about this keyboard

- **Board:** Charybdis Mini 3x6, **Splinktegrated rev1** (RP2040) in each half. Over USB it reports as "Charybdis Mini (3x6) Splinky", VID `A8F8`, PID `1834`, serial `VIAL:…` when running Vial.
- **Firmware:** Vial, built from `vial-kb/vial-qmk` with target `bastardkb/charybdis/3x6/v2/splinky_3:alexandre`. The `splinky_3` pins match BastardKB's `splinktegrated_rev1`. `vial/config.h` adds `USB_VBUS_PIN GP19` like BastardKB does. Do not use `splinky_2`.
- **OS layout is French AZERTY (PC).** Keycodes are QWERTY names: `KC_Q` types A, `KC_W` types Z, `KC_A` types Q, `KC_Z` types W, `KC_SCLN` types M, `KC_M` types `,`. Always think about what the user will *see typed*, and check the `AZ` table in `charybdis-mini/layers.html`.
- **The USB cable goes in the right half** (master). Never plug or unplug the cable between the halves while USB is connected.

## Files

| Path | Role |
| --- | --- |
| `charybdis-mini/vial/keymap.c` | Layout compiled into the firmware. Every flash of a new build loads it onto the keyboard (see below). |
| `charybdis-mini/vial/config.h`, `rules.mk`, `vial.json` | Vial settings, unlock combo, layer count (5), OS detection, custom keycodes. |
| `charybdis-mini/vial/layout.vil` | Backup of the live layout, saved from Vial (File > Save current layout). Source of truth for what is on the keyboard. |
| `charybdis-mini/layers.html` | Reference page with a macOS / Windows switch. Its `LAYERS` line is generated; `META`, `NOTES` (per OS), `NAMED`, `BY_OS`, `SHORTCUTS` are written by hand. |
| `charybdis-mini/update_layers.py` | Rewrites the `LAYERS` line (first 5 layers) from a `.vil`. |
| `charybdis-mini/via/charybdis_mini_alexandre.via.json` | Old VIA layout, to go back to stock VIA firmware. Don't edit. |

Layers (one set for both systems): 0 Base · 1 Symbols (hold left outer thumb) · 2 Mouse (`TG`, left inner thumb) · 3 Function & nav (hold right inner thumb) · 4 Shortcuts (hold right outer thumb). Vial has exactly 5 layer slots (`DYNAMIC_KEYMAP_LAYER_COUNT` in `config.h`); raise it to add a layer.

**macOS / Windows:** QMK OS detection runs on plug-in (`process_detected_host_os_user` in `keymap.c`). Windows/Linux mode = `keymap_config.swap_lctl_lgui` on, so every *left* `LCTL`/`LGUI` (plain, in `LGUI(kc)` shortcuts, in `OSM`) swaps. Right mods (`KC_RCTL`, `KC_RALT`) never swap: use `KC_RCTL` for a Ctrl that must stay Ctrl on both systems (the thumb Ctrls do). Design layouts in Mac terms (Cmd = `LGUI`); Windows follows. For a shortcut a swap can't translate, add an OS-aware key in `os_key()` instead of a separate layer.

Custom keycodes (order must match `vial.json` "customKeycodes"; in `.vil` they are `USER00`.., and `update_layers.py` maps them back):
`QK_KB_0..7` Charybdis pointer keys (`DPI_MOD`, `DPI_RMOD`, `S_D_MOD`, `S_D_RMOD`, `SNIPING`, `SNP_TOG`, `DRGSCRL`, `DRG_TOG`) · `VOICE` (Mac: double-tap Right Cmd; Windows: nothing) · `APP_SW` (Cmd+Tab / Alt+Tab) · `DEL_WRD` (Alt+Bksp / Ctrl+Bksp) · `NAV_BCK` `NAV_FWD` (Cmd+←/→ / Alt+←/→) · `SCRNSHT` (Cmd+Shift+4 / Win+Shift+S) · `SPC_1..5` (Alt+1..5 for yabai / Windows: –, Win, Tab, Alt, –) · `OS_MAC` `OS_WIN` (force a mode until next plug-in). New custom keys go at the end of the enum, `vial.json`, `FIRMWARE_KEYS` in `update_layers.py`, and `BY_OS`/`NAMED` in `layers.html`.

Matrix: 8 rows × 6 cols. Rows 0–3 left half, 4–7 right half; col 0 is the outer (pinky) column on both halves. Thumbs: left `[3,3]` outer (layer hold), `[3,4]` middle (Space), `[3,1]` inner (TG mouse); right `[7,1]` inner (MO func), `[7,3]` outer (MO shortcuts). In `keymap.c` the `LAYOUT()` order per row is left col 0→5, then right col 5→0, then thumbs `L33, L34, L31, R31, R33`.

## Pick the path

**Most changes need no flashing** — keys, layers, combos, tap dance, key overrides, macros, tapping term. These are done live in Vial and stored on the keyboard. Go to path A.

Only code changes need a build and flash: OS detection, new custom keycodes, anything in `process_record_user`, `config.h`/`rules.mk` options. Go to path B.

### Path A — live change in Vial (default)

You can't drive Vial yourself; guide the user:

1. Tell the user exactly what to set, in Vial terms: which layer tab, which key (describe the position, e.g. "left half, home row, 4th key from the outside"), and which keycode to pick (use Vial's tab names: Basic, Layers, Quantum, User, Macro, Tap Dance…). For combos/tap dance/key overrides, give the exact fields to fill.
2. Some actions need **unlock**: hold the second key of the top row on both halves (`KC_Q` and `KC_P`, they type A and P) until Vial says unlocked.
3. Ask the user to save the layout: File > Save current layout → `C:\Users\alexa\Documents\dotfiles\charybdis-mini\vial\layout.vil`.
4. Refresh the page: `python charybdis-mini/update_layers.py charybdis-mini/vial/layout.vil`.
5. If the meaning of a layer or key changed, also update the hand-written parts of `layers.html` (`META` reach text, `NOTES`, and add labels to `NAMED`/`SHORTCUTS` for new keycodes so they don't render as raw codes). If `update_layers.py` meets a new Vial long name (e.g. `KC_AUDIO_MUTE`), add it to `ALIASES`.
6. Check the page renders (open `charybdis-mini/layers.html` in the browser pane, read the keys you changed).
7. Mirror the change into `vial/keymap.c` (and `layers.html` data) — the next flash replaces the live layout with `keymap.c`, so anything only in Vial is lost then.
8. Commit only when the user asks.

### Path B — code change, build and flash

1. Edit `charybdis-mini/vial/` files. Keep `keymap.c` style (enum names for layers, `_______`/`XXXXXXX`, aligned columns).
2. **Build** (QMK MSYS is installed at `C:\QMK_MSYS`; vial-qmk is at `C:\Users\alexa\Documents\vial-qmk`). From PowerShell — run a script file, inline `-lc '...'` breaks on quoting:
   ```powershell
   $env:MSYSTEM='MINGW64'; $env:CHERE_INVOKING='1'; & C:\QMK_MSYS\usr\bin\bash.exe -l /c/Users/alexa/Documents/dotfiles/.claude/skills/charybdis-layout/scripts/build.sh
   ```
   If keycodes like `PDF` or `MS_BTN1` are "undeclared", vial-qmk is outdated: `git -C C:/Users/alexa/Documents/vial-qmk pull --ff-only origin vial` then `git submodule update --init --recursive`.
3. **Flash.** Start the watcher in the background, then tell the user what to do:
   ```powershell
   & C:\Users\alexa\Documents\dotfiles\.claude\skills\charybdis-layout\scripts\flash.ps1
   ```
   User steps: USB in the right half → press its reset button twice quickly (hole in the bottom plate) → red light → the watcher copies the file. Usually only the right half needs flashing. Flash both halves (same file, one at a time, USB in the half being flashed, cable between halves unplugged first) when the vial-qmk version changed or the halves act strange.
4. **Important — every flash resets the live Vial layout.** vial-qmk gives each build a random `BUILD_ID` (`util/build_id.py`); when the keyboard sees a new one it reloads the keymap from `keymap.c` and clears Vial combos, tap dance, key overrides and macros. So before flashing: make sure every live Vial change is already in `keymap.c`, or have the user save a `.vil` first and load it back afterwards (File > Load saved layout). The Ctrl/Cmd swap and DPI are stored elsewhere and survive.

## Troubleshooting

- **Nothing appears on the PC** (no `RPI-RP2`, no `VID_A8F8`) but the red light is on: power arrives but no data. Check the cable is in this PC directly (not the hub/monitor), push the USB-C in firmly, flip the connector. The left half's port has been flaky before. Check with:
  `Get-PnpDevice -PresentOnly | ? { $_.InstanceId -match 'VID_2E8A|VID_A8F8' }` and the `Microsoft-Windows-Kernel-PnP/Configuration` event log.
- **Left half dead after flashing only the right half:** the halves run different firmware; flash the left half too.
- **Keyboard won't boot at all:** BastardKB hardware fallback — open the case, bridge the BOOT pads while plugging in. The RP2040 bootloader is in ROM and can't be broken.
- **Going back to stock VIA:** flash `bastardkb_charybdis_3x6_splinktegrated_rev1_vendor.uf2` from github.com/Bastardkb/qmk_userspace/releases (ask before downloading) on both halves, then load `via/charybdis_mini_alexandre.via.json` in usevia.app.
