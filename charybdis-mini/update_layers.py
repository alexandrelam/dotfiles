"""Refresh the LAYERS data in layers.html from a Vial export.

Usage:
    python update_layers.py vial/layout.vil     # Vial: File > Save current layout

Vial uses long key names; they are mapped to the short names that layers.html knows.
"""
import json
import re
import sys
from pathlib import Path

HTML = Path(__file__).with_name("layers.html")

# Vial long name -> name used by layers.html
ALIASES = {
    "KC_DELETE": "KC_DEL", "KC_RIGHT": "KC_RGHT", "KC_ENTER": "KC_ENT", "KC_ESCAPE": "KC_ESC",
    "KC_BSPACE": "KC_BSPC", "KC_SPACE": "KC_SPC", "KC_MINUS": "KC_MINS", "KC_EQUAL": "KC_EQL",
    "KC_LBRACKET": "KC_LBRC", "KC_RBRACKET": "KC_RBRC", "KC_BSLASH": "KC_BSLS", "KC_SCOLON": "KC_SCLN",
    "KC_QUOTE": "KC_QUOT", "KC_COMMA": "KC_COMM", "KC_SLASH": "KC_SLSH", "KC_NONUS_BSLASH": "KC_NUBS",
    "KC_LCTRL": "KC_LCTL", "KC_LSHIFT": "KC_LSFT", "KC_RCTRL": "KC_RCTL", "KC_RSHIFT": "KC_RSFT", "KC_AUDIO_VOL_UP": "KC_VOLU", "KC_AUDIO_VOL_DOWN": "KC_VOLD",
    "KC_BTN1": "KC_MS_BTN1", "KC_BTN2": "KC_MS_BTN2", "KC_BTN3": "KC_MS_BTN3",
    "KC_TRANSPARENT": "KC_TRNS", "KC_NUMLOCK": "KC_NUM",
    "KC_KP_SLASH": "KC_PSLS", "KC_KP_ASTERISK": "KC_PAST", "KC_KP_MINUS": "KC_PMNS", "KC_KP_PLUS": "KC_PPLS",
    "KC_KP_ENTER": "KC_PENT", "KC_KP_DOT": "KC_PDOT",
    **{f"KC_KP_{n}": f"KC_P{n}" for n in range(10)},
}
LAYER_COUNT = 8
# Vial custom keycodes, in the order of vial/vial.json "customKeycodes"
FIRMWARE_KEYS = ["VOICE", "APP_SW", "DEL_WRD", "NAV_BCK", "NAV_FWD", "SCRNSHT",
                 "SPC_1", "SPC_2", "SPC_3", "SPC_4", "SPC_5"]
USER = {f"USER{n:02}": f"CUSTOM({n})" for n in range(8)}
USER.update({f"USER{n + 8:02}": name for n, name in enumerate(FIRMWARE_KEYS)})


def normalize(kc):
    if kc == -1 or kc is None:
        return "KC_NO"
    kc = str(kc)
    if kc in USER:
        return USER[kc]
    if re.fullmatch(r"M\d+", kc):
        return f"MACRO({kc[1:]})"
    return re.sub(r"KC_\w+", lambda m: ALIASES.get(m.group(0), m.group(0)), kc)


def load(path):
    data = json.loads(Path(path).read_text(encoding="utf-8"))
    return [[normalize(k) for row in layer for k in row] for layer in data["layout"]]  # layers -> rows -> cols


def main():
    layers = load(sys.argv[1])
    layers = layers[:LAYER_COUNT]
    if len(layers) < LAYER_COUNT or any(len(layer) != 48 for layer in layers):
        sys.exit(f"expected {LAYER_COUNT} layers of 8x6 keys")
    html = HTML.read_text(encoding="utf-8")
    line = "const LAYERS = " + json.dumps(layers, ensure_ascii=False) + ";"
    html, n = re.subn(r"^const LAYERS = .*;$", lambda _: line, html, flags=re.M)
    if n != 1:
        sys.exit("LAYERS line not found in layers.html")
    HTML.write_text(html, encoding="utf-8")
    print(f"layers.html updated from {sys.argv[1]}")


if __name__ == "__main__":
    main()
