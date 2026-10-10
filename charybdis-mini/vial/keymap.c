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

enum layers {
    BASE,
    SYM,
    MOUSE,
    FUNC,
    SHORT,
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
    OS_MAC, // force macOS mode until the next plug-in
    OS_WIN, // force Windows mode until the next plug-in
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        MS_BTN2,       KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,            KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,          MS_BTN2,
        MS_BTN1,       KC_A,          KC_S,          KC_D,          KC_F,          KC_G,            KC_H,          KC_J,          KC_K,          KC_L,          KC_SCLN,       MS_BTN1,
        DRGSCRL,       KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,            KC_N,          KC_M,          LSFT(KC_COMM), KC_DOT,        KC_SLSH,       DRGSCRL,
                                                     MO(SYM),       KC_SPC,        TG(MOUSE),       MO(FUNC),      MO(SHORT)
    ),
    [SYM] = LAYOUT(
        _______,       KC_1,          KC_2,          KC_3,          KC_4,          KC_5,            KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), KC_ENT,          KC_MINS,       KC_LBRC,       KC_RBRC,       KC_BSLS,       KC_EQL,        _______,
        _______,       SPC_1,         SPC_2,         SPC_3,         SPC_4,         SPC_5,           KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     _______,       KC_RCTL,       VOICE,           KC_LSFT,       KC_RALT
    ),
    [MOUSE] = LAYOUT(
        _______,    LGUI(KC_Z), NAV_BCK,    NAV_FWD,    SGUI(KC_T), LGUI(KC_T),   DPI_MOD,    DPI_RMOD,   S_D_MOD,    S_D_RMOD,   KC_VOLD,    KC_VOLU,
        _______,    KC_LSFT,    MS_BTN2,    DRGSCRL,    MS_BTN1,    DRGSCRL,      VOICE,      MS_BTN1,    DRGSCRL,    MS_BTN2,    KC_BRID,    KC_BRIU,
        _______,    SPC_1,      SPC_2,      SPC_3,      SPC_4,      SPC_5,        DRG_TOG,    SNP_TOG,    MS_BTN3,    XXXXXXX,    XXXXXXX,    _______,
                                            DRGSCRL,    MS_BTN1,    _______,      _______,    _______
    ),
    [FUNC] = LAYOUT(
        _______,       KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,           KC_5,          KC_MINS,       KC_UP,         KC_VOLU,       KC_VOLD,       _______,
        _______,       KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,          RALT(KC_5),    KC_LEFT,       KC_DOWN,       KC_RGHT,       KC_BRID,       _______,
        _______,       KC_F11,        KC_F12,        OS_MAC,        OS_WIN,        XXXXXXX,         RALT(KC_4),    RALT(KC_EQL),  RALT(KC_MINS), KC_BRIU,       KC_NUBS,       _______,
                                                     KC_LSFT,       KC_RCTL,       _______,         _______,       _______
    ),
    [SHORT] = LAYOUT(
        _______,       LGUI(KC_Q),    LGUI(KC_W),    LSFT(KC_Q),    SGUI(KC_F),    LGUI(KC_T),      KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       APP_SW,        LGUI(KC_S),    LGUI(KC_D),    LGUI(KC_F),    SCRNSHT,         KC_TAB,        KC_BSPC,       DEL_WRD,       KC_ESC,        KC_ENT,        _______,
        _______,       LGUI(KC_Z),    LGUI(KC_X),    LGUI(KC_C),    LGUI(KC_V),    LSFT(KC_I),      KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     KC_LSFT,       KC_RCTL,       _______,         KC_LGUI,       _______
    ),
};
// clang-format on

// The old Mac/Linux layout saved its default layer with PDF(); there is only
// one base layer now, so always start on it and clear any old saved value.
void keyboard_post_init_user(void) {
    const layer_state_t base = (layer_state_t)1 << BASE;
    if (eeconfig_read_default_layer() != base) {
        eeconfig_update_default_layer(base);
    }
    default_layer_set(base);
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
        case VOICE:
            if (record->event.pressed && !is_windows()) {
                tap_code_delay(KC_RGUI, 30);
                wait_ms(100);
                tap_code_delay(KC_RGUI, 30);
            }
            return false;
        case OS_MAC:
        case OS_WIN:
            if (record->event.pressed) {
                set_windows(keycode == OS_WIN);
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
