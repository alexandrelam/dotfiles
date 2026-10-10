# Charybdis Mini

My board: **Charybdis Mini (3x6)** with a **Splinktegrated rev1** controller (RP2040) in each half.
It reports itself as "Charybdis Mini (3x6) Splinky" over USB, because BastardKB uses that name for every 3x6 build.
Vial has no Splinktegrated target, but the `splinky_3` target uses the same pins as BastardKB's `splinktegrated_rev1`.
The OS keyboard layout is French AZERTY (PC), so the keycodes and the printed letters differ (KC_Q types A).

| Path | What it is |
| --- | --- |
| [layers.html](layers.html) | Reference page that shows every layer. |
| [via/charybdis_mini_alexandre.via.json](via/charybdis_mini_alexandre.via.json) | Old layout for the VIA firmware. Load it in [usevia.app](https://usevia.app). |
| [vial/](vial/) | QMK keymap for the Vial firmware: `keymap.c` is the layout the keyboard starts with. |
| [update_layers.py](update_layers.py) | Refreshes `layers.html` from a Vial `.vil` (or VIA `.json`) export. |

## Vial firmware

Vial is VIA plus more QMK features that you can change live, without building again:
combos, tap dance, key overrides, and timing settings (tapping term and others in "QMK Settings").

The Vial keymap is the same as the VIA layout, with two fixes:

- The base layer keys on the function layer are `PDF(0)` / `PDF(1)`, so the keyboard remembers Mac or Linux after unplugging.
- Voice capture is a firmware key (`VOICE`), not a macro.

### Get the firmware

Build it locally. On Windows, open **QMK MSYS** (not PowerShell) and run:

```bash
cd /c/Users/alexa/Documents/vial-qmk && rm -rf keyboards/bastardkb/charybdis/3x6/keymaps/alexandre && cp -r /c/Users/alexa/Documents/dotfiles/charybdis-mini/vial keyboards/bastardkb/charybdis/3x6/keymaps/alexandre && make bastardkb/charybdis/3x6/v2/splinky_3:alexandre
```

If the keyboard does not work after flashing, the hardware fallback in the [BastardKB flashing guide](https://docs.bastardkb.com/fw/flashing.html) always works: hold the BOOT pads while plugging in.

### Flash it

Flash **both halves** with the same `.uf2` file, one half at a time:

1. Plug in one half only (USB cable in that half).
2. Put it in bootloader mode: **press the reset button twice quickly** (it is the same as the BastardKB flashing guide).
3. A USB drive called `RPI-RP2` appears. Copy the `.uf2` file onto it. The drive closes by itself when it is done.
4. Do the same for the other half, then connect the halves and plug in the right half as usual.

The first start resets the keyboard memory, so it loads the layout from `keymap.c`. DPI settings go back to default too.

### Change keys

Open [vial.rocks](https://vial.rocks) in Chrome or Edge, or the Vial desktop app.
Some changes ask you to **unlock** the keyboard: hold the second key of the top row on both halves (A and P on AZERTY).

After you change the layout, save it (File > Save current layout) as `vial/layout.vil` and refresh the page:

```bash
python update_layers.py vial/layout.vil
```

### Go back to VIA

Flash the BastardKB VIA firmware for "Charybdis Mini 3x6 Splinky" again, then load `via/charybdis_mini_alexandre.via.json` in usevia.app.
