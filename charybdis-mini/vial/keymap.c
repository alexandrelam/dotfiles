/*
 * Charybdis Mini (3x6) Vial keymap.
 *
 * Port of ../via/charybdis_mini_alexandre.via.json, with two changes:
 *   - DF() is now PDF(), so the chosen base layer survives unplugging.
 *   - Voice capture (VIA macro 0) is a firmware keycode, so a Vial macro
 *     reset can't lose it.
 *
 * This is only the layout the keyboard starts with after flashing. Live edits
 * made in Vial are stored on the keyboard, not here.
 */
#include QMK_KEYBOARD_H

enum layers {
    MAC_BASE,
    LNX_BASE,
    MAC_SYM,
    MOUSE,
    FUNC,
    MAC_SHORT,
    LNX_SYM,
    LNX_SHORT,
};

// QK_KB_0..7 are the Charybdis pointer keycodes (DPI, sniping, drag-scroll).
enum custom_keycodes {
    VOICE = QK_KB_8, // double-tap Right GUI to start voice capture
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT(
        MS_BTN2,       KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,            KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,          MS_BTN2,
        MS_BTN1,       KC_A,          KC_S,          KC_D,          KC_F,          KC_G,            KC_H,          KC_J,          KC_K,          KC_L,          KC_SCLN,       MS_BTN1,
        DRGSCRL,       KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,            KC_N,          KC_M,          LSFT(KC_COMM), KC_DOT,        KC_SLSH,       DRGSCRL,
                                                     MO(MAC_SYM),   KC_SPC,        TG(MOUSE),       MO(FUNC),      MO(MAC_SHORT)
    ),
    [LNX_BASE] = LAYOUT(
        MS_BTN2,       KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,            KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,          MS_BTN2,
        MS_BTN1,       KC_A,          KC_S,          KC_D,          KC_F,          KC_G,            KC_H,          KC_J,          KC_K,          KC_L,          KC_SCLN,       MS_BTN1,
        DRGSCRL,       KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,            KC_N,          KC_M,          LSFT(KC_COMM), KC_DOT,        KC_SLSH,       DRGSCRL,
                                                     MO(LNX_SYM),   KC_SPC,        TG(MOUSE),       MO(FUNC),      MO(LNX_SHORT)
    ),
    [MAC_SYM] = LAYOUT(
        _______,       KC_1,          KC_2,          KC_3,          KC_4,          KC_5,            KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), KC_ENT,          KC_MINS,       KC_LBRC,       KC_RBRC,       KC_BSLS,       KC_EQL,        _______,
        _______,       LALT(KC_1),    LALT(KC_2),    LALT(KC_3),    LALT(KC_4),    LALT(KC_5),      KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     _______,       KC_LCTL,       VOICE,           KC_LSFT,       KC_RALT
    ),
    [MOUSE] = LAYOUT(
        _______,       LGUI(KC_Z),    LGUI(KC_LEFT), LGUI(KC_RGHT), SGUI(KC_T),    LGUI(KC_T),      DPI_MOD,       DPI_RMOD,      S_D_MOD,       S_D_RMOD,      KC_VOLD,       KC_VOLU,
        _______,       KC_LSFT,       MS_BTN2,       DRGSCRL,       MS_BTN1,       DRGSCRL,         VOICE,         MS_BTN1,       DRGSCRL,       MS_BTN2,       KC_BRID,       KC_BRIU,
        _______,       LALT(KC_1),    LALT(KC_2),    LALT(KC_3),    LALT(KC_4),    LALT(KC_5),      DRG_TOG,       SNP_TOG,       MS_BTN3,       XXXXXXX,       XXXXXXX,       _______,
                                                     DRGSCRL,       MS_BTN1,       _______,         _______,       _______
    ),
    [FUNC] = LAYOUT(
        _______,       KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,           KC_5,          KC_MINS,       KC_UP,         KC_VOLU,       KC_VOLD,       _______,
        _______,       KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,          RALT(KC_5),    KC_LEFT,       KC_DOWN,       KC_RGHT,       KC_BRID,       _______,
        _______,       KC_F11,        KC_F12,        PDF(MAC_BASE), PDF(LNX_BASE), XXXXXXX,         RALT(KC_4),    RALT(KC_EQL),  RALT(KC_MINS), KC_BRIU,       KC_NUBS,       _______,
                                                     KC_LSFT,       KC_LCTL,       _______,         _______,       _______
    ),
    [MAC_SHORT] = LAYOUT(
        _______,       LGUI(KC_Q),    LGUI(KC_W),    LSFT(KC_Q),    SGUI(KC_F),    LGUI(KC_T),      KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       LGUI(KC_TAB),  LGUI(KC_S),    LGUI(KC_D),    LGUI(KC_F),    SGUI(KC_4),      KC_TAB,        KC_BSPC,       LALT(KC_BSPC), KC_ESC,        KC_ENT,        _______,
        _______,       LGUI(KC_Z),    LGUI(KC_X),    LGUI(KC_C),    LGUI(KC_V),    LSFT(KC_I),      KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     KC_LSFT,       KC_LCTL,       _______,         KC_LGUI,       _______
    ),
    [LNX_SYM] = LAYOUT(
        _______,       KC_1,          KC_2,          KC_3,          KC_4,          KC_5,            KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       OSM(MOD_LSFT), OSM(MOD_LGUI), OSM(MOD_LALT), OSM(MOD_LCTL), KC_ENT,          KC_MINS,       KC_LBRC,       KC_RBRC,       KC_BSLS,       KC_EQL,        _______,
        _______,       XXXXXXX,       KC_LGUI,       KC_TAB,        KC_LALT,       XXXXXXX,         KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     _______,       _______,       _______,         KC_LSFT,       KC_RALT
    ),
    [LNX_SHORT] = LAYOUT(
        _______,       LCTL(KC_Q),    LCTL(KC_W),    LSFT(KC_Q),    LCTL(KC_R),    LCTL(KC_T),      KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          _______,
        _______,       LALT(KC_TAB),  LCTL(KC_S),    LCTL(KC_D),    LCTL(KC_F),    XXXXXXX,         KC_TAB,        KC_BSPC,       LCTL(KC_BSPC), KC_ESC,        KC_ENT,        _______,
        _______,       LCTL(KC_Z),    LCTL(KC_X),    LCTL(KC_C),    LCTL(KC_V),    LSFT(KC_I),      KC_QUOT,       LSFT(KC_M),    KC_COMM,       LSFT(KC_DOT),  LSFT(KC_SLSH), _______,
                                                     KC_LALT,       KC_LCTL,       _______,         KC_LGUI,       _______
    ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == VOICE) {
        if (record->event.pressed) {
            tap_code_delay(KC_RGUI, 30);
            wait_ms(100);
            tap_code_delay(KC_RGUI, 30);
        }
        return false;
    }
    return true;
}
