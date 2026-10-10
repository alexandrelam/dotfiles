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
| `charybdis-mini/vial/keymap.c` | Default layout compiled into the firmware. Only used after an EEPROM reset (see below). |
| `charybdis-mini/vial/config.h`, `rules.mk`, `vial.json` | Vial settings, unlock combo, layer count (8), custom keycodes. |
| `charybdis-mini/vial/layout.vil` | Backup of the live layout, saved from Vial (File > Save current layout). Source of truth for what is on the keyboard. |
| `charybdis-mini/layers.html` | Reference page. Its `LAYERS` line is generated; `META`, `NOTES`, `NAMED`, `SHORTCUTS` are written by hand. |
| `charybdis-mini/update_layers.py` | Rewrites the `LAYERS` line from a `.vil` (or the old VIA `.json`). |
| `charybdis-mini/via/charybdis_mini_alexandre.via.json` | Old VIA layout, to go back to stock VIA firmware. Don't edit. |
| `.github/workflows/charybdis-vial.yml` | CI build of the same firmware. |

Layers: 0 Mac base · 1 Linux base (also Windows) · 2 Mac symbols · 3 Mouse (`TG`) · 4 Function & nav · 5 Mac shortcuts · 6 Linux symbols · 7 Linux shortcuts. Custom keycodes: `QK_KB_0..7` = Charybdis pointer keys (`DPI_MOD`, `DPI_RMOD`, `S_D_MOD`, `S_D_RMOD`, `SNIPING`, `SNP_TOG`, `DRGSCRL`, `DRG_TOG`), `QK_KB_8` = `VOICE` (double-tap Right GUI). In `.vil` files they appear as `USER00`..`USER08`.

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
7. Optional but good: mirror the change into `vial/keymap.c` so a future EEPROM reset gives the same layout.
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
4. **Important — flashing does not replace the live layout.** Vial keeps the keymap stored on the keyboard, so `keymap.c` edits only show after an EEPROM reset. Easiest is bootmagic: unplug, hold the top-right outer key of the right half, plug in (this clears the EEPROM and opens the bootloader, so flash right after). An `EE_CLR` key also works. A reset also wipes Vial combos/tap dance/macros and DPI. Before resetting, have the user save a `.vil` so they can load it back (File > Load saved layout).

## Troubleshooting

- **Nothing appears on the PC** (no `RPI-RP2`, no `VID_A8F8`) but the red light is on: power arrives but no data. Check the cable is in this PC directly (not the hub/monitor), push the USB-C in firmly, flip the connector. The left half's port has been flaky before. Check with:
  `Get-PnpDevice -PresentOnly | ? { $_.InstanceId -match 'VID_2E8A|VID_A8F8' }` and the `Microsoft-Windows-Kernel-PnP/Configuration` event log.
- **Left half dead after flashing only the right half:** the halves run different firmware; flash the left half too.
- **Keyboard won't boot at all:** BastardKB hardware fallback — open the case, bridge the BOOT pads while plugging in. The RP2040 bootloader is in ROM and can't be broken.
- **Going back to stock VIA:** flash `bastardkb_charybdis_3x6_splinktegrated_rev1_vendor.uf2` from github.com/Bastardkb/qmk_userspace/releases (ask before downloading) on both halves, then load `via/charybdis_mini_alexandre.via.json` in usevia.app.
