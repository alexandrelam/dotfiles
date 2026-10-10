#pragma once

#define VIAL_KEYBOARD_UID {0xDC, 0x14, 0xCA, 0x71, 0xCB, 0xDB, 0x2C, 0xEF}

// Keys to hold when Vial asks to unlock the keyboard: the second key of the
// top row on each half (KC_Q and KC_P, which type A and P on AZERTY).
#define VIAL_UNLOCK_COMBO_ROWS {0, 4}
#define VIAL_UNLOCK_COMBO_COLS {1, 1}

// The board is a Splinktegrated rev1, built with the splinky_3 target (same pins).
// BastardKB's own firmware for it also sets the USB power sense pin.
#define USB_VBUS_PIN GP19

#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 8
