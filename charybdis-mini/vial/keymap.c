/*
 * Charybdis Mini (3x6) Vial keymap.
 *
 * One set of layers for macOS and Windows. The keyboard detects the OS when it
 * is plugged in:
 *   - macOS: keys work as written below (LGUI = Cmd).
 *   - Windows (and Linux): left Ctrl and left GUI are swapped, so every
 *     LGUI(...) shortcut sends Ctrl instead (Cmd+C -> Ctrl+C) and the one-shot
 *     Ctrl/Cmd on the symbols layer trade places.
 * Right-hand mods (KC_RCTL, KC_RALT) are never swapped, so the thumb Ctrl is
 * the real Ctrl on both systems. Shortcuts a swap can't translate are the
 * OS-aware keys below.
 *
 * This is only the layout the keyboard starts with after an EEPROM reset.
 * Live edits made in Vial are stored on the keyboard, not here.
 */
#include QMK_KEYBOARD_H
#include "qmk_settings.h"
#include "dynamic_keymap.h"

enum layers {
    BASE,
    SYM,
    MOUSE,
    FUNC,
    SHORT,
    NUM,
};

// QK_KB_0..7 are the Charybdis pointer keycodes (DPI, sniping, drag-scroll).
// Keep this order in sync with "customKeycodes" in vial.json.
enum custom_keycodes {
    VOICE = QK_KB_8, // Mac: double-tap Right Cmd (voice capture). Windows: nothing.
    APP_SW,          // Cmd+Tab        / Alt+Tab
    DEL_WRD,         // Alt+Backspace  / Ctrl+Backspace
    NAV_BCK,         // Cmd+Left       / Alt+Left
    NAV_FWD,         // Cmd+Right      / Alt+Right
    SCRNSHT,         // Cmd+Shift+4    / Win+Shift+S
    SPC_1,           // Alt+1..5 (yabai spaces) / Windows: -, Win, Tab, Alt, -
    SPC_2,
    SPC_3,
    SPC_4,
    SPC_5,
};

// Space on tap, right click on hold. LT() to the base layer is only a tap-hold
// shell: process_record_user turns the hold into the mouse button.
#define SPC_RCL LT(BASE, KC_SPC)

// Mouse Back on tap, Forward on double tap, right click on hold. Vial tap dance
// slot 0, filled in by apply_default_tap_dances().
#define MS_NAV TD(0)

// The top row letters are Vial tap dances 1-10 (A Z E R T on the left, Y U I O P
// on the right): see apply_default_tap_dances().

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        MS_NAV,           TD(1),            TD(2),            TD(3),            TD(4),            TD(5),              TD(6),            TD(7),            TD(8),            TD(9),            TD(10),           MS_NAV,
        DRGSCRL,          LSFT_T(KC_A),     LCTL_T(KC_S),     LALT_T(KC_D),     LGUI_T(KC_F),     KC_G,               KC_H,             LGUI_T(KC_J),     LALT_T(KC_K),     LCTL_T(KC_L),     LSFT_T(KC_SCLN),  DRGSCRL,
        MS_BTN1,          KC_Z,             KC_X,             KC_C,             KC_V,             KC_B,               KC_N,             KC_M,             LSFT(KC_COMM),    KC_DOT,           KC_SLSH,          MS_BTN1,
                                                              LT(SYM, MS_BTN1), SPC_RCL,          TG(MOUSE),          MO(FUNC),         MO(SHORT)
    ),
    [SYM] = LAYOUT(
        _______,       KC_1,          KC_2,          KC_3,          KC_4,          KC_5,            KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), KC_ENT,          KC_MINS,       KC_LBRC,       KC_RBRC,       KC_BSLS,       KC_EQL,        _______,
        _______,       SPC_1,         SPC_2,         SPC_3,         SPC_4,         SPC_5,           KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     _______,       KC_RCTL,       VOICE,           KC_LSFT,       KC_RALT
    ),
    [MOUSE] = LAYOUT(
        _______,    LGUI(KC_Z), NAV_BCK,    NAV_FWD,    SGUI(KC_T), LGUI(KC_T),   DPI_MOD,    DPI_RMOD,   S_D_MOD,    S_D_RMOD,   KC_BRID,    KC_BRIU,
        _______,    KC_LSFT,    MS_BTN2,    DRGSCRL,    MS_BTN1,    DRGSCRL,      VOICE,      MS_BTN1,    DRGSCRL,    MS_BTN2,    KC_VOLD,    KC_VOLU,
        _______,    SPC_1,      SPC_2,      SPC_3,      SPC_4,      SPC_5,        DRG_TOG,    SNP_TOG,    MS_BTN3,    XXXXXXX,    XXXXXXX,    _______,
                                            MS_BTN1,    DRGSCRL,    _______,      _______,    _______
    ),
    [FUNC] = LAYOUT(
        _______,       KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,           KC_5,          KC_MINS,       KC_UP,         KC_VOLU,       KC_VOLD,       _______,
        _______,       KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,          RALT(KC_5),    KC_LEFT,       KC_DOWN,       KC_RGHT,       KC_BRID,       _______,
        _______,       KC_F11,        KC_F12,        XXXXXXX,       XXXXXXX,       XXXXXXX,         RALT(KC_4),    RALT(KC_EQL),  RALT(KC_MINS), KC_BRIU,       KC_NUBS,       _______,
                                                     KC_LSFT,       KC_RCTL,       _______,         _______,       _______
    ),
    [SHORT] = LAYOUT(
        _______,       LGUI(KC_Q),    LGUI(KC_W),    LSFT(KC_Q),    SGUI(KC_F),    LGUI(KC_T),      KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       APP_SW,        LGUI(KC_S),    LGUI(KC_D),    LGUI(KC_F),    SCRNSHT,         KC_TAB,        KC_BSPC,       DEL_WRD,       KC_ESC,        KC_ENT,        _______,
        _______,       LGUI(KC_Z),    LGUI(KC_X),    LGUI(KC_C),    LGUI(KC_V),    LSFT(KC_I),      KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     KC_LSFT,       KC_RCTL,       _______,         KC_LGUI,       _______
    ),
    [NUM] = LAYOUT(
        _______, _______, _______, _______, _______, _______,   KC_PSLS, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_NUM,
        _______, _______, _______, _______, _______, _______,   KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_BSPC,
        _______, _______, _______, _______, _______, _______,   KC_PDOT, KC_P7,   KC_P8,   KC_P9,   KC_PENT, XXXXXXX,
                                   _______, _______, _______,   KC_P0,   _______
    ),
};
// clang-format on

// Chordal Hold "hands": rows 0-3 are the left half, 4-7 the right half.
// Thumbs are exempt, so the Symbols thumb works with keys on its own hand.
char chordal_hold_handedness(keypos_t key) {
    if (key.row == 3 || key.row == 7) {
        return '*';
    }
    return key.row < MATRIX_ROWS / 2 ? 'L' : 'R';
}

// Tap-hold settings for the home row mods and the click/Symbols thumb. They live
// in Vial's "QMK Settings" tab, which every flash resets to all-off. Apply these
// only while the settings are still at that factory state, so changes made in
// Vial are kept.
static void apply_tap_hold_defaults(void) {
    if (QS.tapping_v2 != 0 || QS.flow_tap_term != 0) {
        return;
    }
    const uint8_t  on            = 1;
    const uint16_t flow_tap_term = 150; // fast typing never triggers a home row mod
    qmk_settings_set(22, &on, sizeof(on)); // Permissive Hold: hold + tap another key = hold
    qmk_settings_set(26, &on, sizeof(on)); // Chordal Hold: home row mod + same-hand key = letters
    qmk_settings_set(27, &flow_tap_term, sizeof(flow_tap_term));
}

// Default combos, using base layer keycodes (letters as typed on AZERTY):
//   drag-scroll + Q (left home row, outer two keys) toggles Caps Lock
//   back/right click + A (left top row, outer two keys) toggles the numpad layer;
//   both keys are transparent on the numpad layer, so the combo also turns it off
// Vial stores combos on the keyboard and every flash clears them, so add these
// only when no combo is set. Edit or delete them in Vial's Combos tab.
static void apply_default_combos(void) {
    for (uint8_t i = 0; i < VIAL_COMBO_ENTRIES; i++) {
        vial_combo_entry_t entry;
        if (dynamic_keymap_get_combo(i, &entry) != 0 || entry.output != KC_NO) {
            return;
        }
    }
    const vial_combo_entry_t caps_lock = {.input = {DRGSCRL, LSFT_T(KC_A), KC_NO, KC_NO}, .output = KC_CAPS};
    const vial_combo_entry_t numpad    = {.input = {MS_NAV, TD(1), KC_NO, KC_NO}, .output = TG(NUM)};
    dynamic_keymap_set_combo(0, &caps_lock);
    dynamic_keymap_set_combo(1, &numpad);
    vial_init(); // reload combos from the keyboard memory
}

// Default tap dances, same idea as the combos: Vial keeps them on the keyboard and
// every flash clears them, so fill them in only when none is set.
// Vial uses each tap dance's own tapping term (0 would make every press a hold).
#define TD_TAPPING_TERM 200

static void apply_default_tap_dances(void) {
    for (uint8_t i = 0; i < VIAL_TAP_DANCE_ENTRIES; i++) {
        vial_tap_dance_entry_t entry;
        if (dynamic_keymap_get_tap_dance(i, &entry) != 0 || entry.on_tap || entry.on_hold || entry.on_double_tap || entry.on_tap_hold) {
            return;
        }
    }
    const vial_tap_dance_entry_t ms_nav = {.on_tap = MS_BTN4, .on_hold = MS_BTN2, .on_double_tap = MS_BTN5, .on_tap_hold = KC_NO, .custom_tapping_term = TD_TAPPING_TERM};
    dynamic_keymap_set_tap_dance(0, &ms_nav);

    // Top row, slots 1-10: tap = letter, double tap = the AZERTY number-row
    // character (& é " ' ( - è _ ç à), hold = the digit (Shift + number row).
    static const uint16_t letters[10] = {KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P};
    static const uint16_t numbers[10] = {KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0};
    for (uint8_t i = 0; i < 10; i++) {
        const vial_tap_dance_entry_t top = {.on_tap = letters[i], .on_hold = LSFT(numbers[i]), .on_double_tap = numbers[i], .on_tap_hold = KC_NO, .custom_tapping_term = TD_TAPPING_TERM};
        dynamic_keymap_set_tap_dance(1 + i, &top);
    }
    vial_init(); // reload tap dances from the keyboard memory
}

void keyboard_post_init_user(void) {
    // The old Mac/Linux layout saved its default layer with PDF(); there is only
    // one base layer now, so always start on it and clear any old saved value.
    const layer_state_t base = (layer_state_t)1 << BASE;
    if (eeconfig_read_default_layer() != base) {
        eeconfig_update_default_layer(base);
    }
    default_layer_set(base);

    apply_tap_hold_defaults();
    apply_default_tap_dances();
    apply_default_combos();
}

static bool is_windows(void) {
    return keymap_config.swap_lctl_lgui;
}

static void set_windows(bool on) {
    if (keymap_config.swap_lctl_lgui == on) {
        return;
    }
    clear_keyboard();
    keymap_config.swap_lctl_lgui = on;
    eeconfig_update_keymap(&keymap_config);
}

bool process_detected_host_os_user(os_variant_t os) {
    if (os != OS_UNSURE) {
        set_windows(os != OS_MACOS && os != OS_IOS);
    }
    return true;
}

// What an OS-aware key sends. Sent with register_code16, which is not swapped.
static uint16_t os_key(uint16_t keycode) {
    bool win = is_windows();
    switch (keycode) {
        case APP_SW:  return win ? LALT(KC_TAB) : LGUI(KC_TAB);
        case DEL_WRD: return win ? LCTL(KC_BSPC) : LALT(KC_BSPC);
        case NAV_BCK: return win ? LALT(KC_LEFT) : LGUI(KC_LEFT);
        case NAV_FWD: return win ? LALT(KC_RGHT) : LGUI(KC_RGHT);
        case SCRNSHT: return win ? SGUI(KC_S) : SGUI(KC_4);
        case SPC_1:   return win ? KC_NO : LALT(KC_1);
        case SPC_2:   return win ? KC_LGUI : LALT(KC_2);
        case SPC_3:   return win ? KC_TAB : LALT(KC_3);
        case SPC_4:   return win ? KC_LALT : LALT(KC_4);
        case SPC_5:   return win ? KC_NO : LALT(KC_5);
    }
    return KC_NO;
}

// Remember what each OS-aware key pressed, so the release matches even if the mode changed.
static uint16_t os_key_held[SPC_5 - APP_SW + 1];

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SPC_RCL:
            if (!record->tap.count) {
                if (record->event.pressed) {
                    register_code(MS_BTN2);
                } else {
                    unregister_code(MS_BTN2);
                }
                return false;
            }
            return true;
        case VOICE:
            if (record->event.pressed && !is_windows()) {
                tap_code_delay(KC_RGUI, 30);
                wait_ms(100);
                tap_code_delay(KC_RGUI, 30);
            }
            return false;
        case APP_SW ... SPC_5: {
            uint16_t *held = &os_key_held[keycode - APP_SW];
            if (record->event.pressed) {
                *held = os_key(keycode);
                if (*held != KC_NO) {
                    register_code16(*held);
                }
            } else if (*held != KC_NO) {
                unregister_code16(*held);
                *held = KC_NO;
            }
            return false;
        }
    }
    return true;
}
