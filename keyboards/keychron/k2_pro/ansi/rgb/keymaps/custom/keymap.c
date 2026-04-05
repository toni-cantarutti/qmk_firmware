/* Copyright 2022 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// clang-format off
enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN
};

enum custom_keycodes {
    NAV_SPC = SAFE_RANGE,
};

static bool nav_space_down = false;
static bool nav_space_used = false;

static bool nav_i_down    = false;
static bool nav_j_down    = false;
static bool nav_k_down    = false;
static bool nav_l_down    = false;
static bool nav_u_down    = false;
static bool nav_o_down    = false;
static bool nav_h_down    = false;
static bool nav_scln_down = false;

static bool handle_nav_hold(keyrecord_t *record, bool *is_down, uint16_t target_keycode) {
    if (nav_space_down || *is_down) {
        if (record->event.pressed) {
            nav_space_used = true;
            *is_down       = true;
            register_code16(target_keycode);
        } else {
            *is_down = false;
            unregister_code16(target_keycode);
        }
        return false;
    }

    return true;
}

static void tap_with_ralt(uint16_t keycode) {
    register_code(KC_RALT);
    tap_code16(keycode);
    unregister_code(KC_RALT);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case NAV_SPC:
            if (record->event.pressed) {
                nav_space_down = true;
                nav_space_used = false;
            } else {
                if (!nav_space_used) {
                    tap_code16(KC_SPC);
                }

                nav_space_down = false;
                nav_space_used = false;
            }
            return false;

        case KC_I:
            return handle_nav_hold(record, &nav_i_down, KC_UP);

        case KC_J:
            return handle_nav_hold(record, &nav_j_down, KC_LEFT);

        case KC_K:
            return handle_nav_hold(record, &nav_k_down, KC_DOWN);

        case KC_L:
            return handle_nav_hold(record, &nav_l_down, KC_RGHT);

        case KC_U:
            return handle_nav_hold(record, &nav_u_down, KC_HOME);

        case KC_O:
            return handle_nav_hold(record, &nav_o_down, KC_END);

        case KC_H:
            return handle_nav_hold(record, &nav_h_down, KC_BSPC);

        case KC_SCLN:
            return handle_nav_hold(record, &nav_scln_down, KC_DEL);

        case KC_E:
        case KC_D:
        case KC_2:
        case KC_3:
        case KC_4:
        case KC_W:
        case KC_Q:
        case KC_A:
        case KC_F:
            if (nav_space_down && record->event.pressed) {
                nav_space_used = true;
                tap_with_ralt(keycode);
                return false;
            }
            return true;

        case KC_C:
            if (nav_space_down && record->event.pressed) {
                nav_space_used = true;

                if ((get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_CTRL) {
                    return true;
                }

                tap_code16(C(A(KC_C)));
                return false;
            }
            return true;
    }

    if (nav_space_down && record->event.pressed) {
        nav_space_used = true;
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_84(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_F13,   KC_F14,   KC_DEL,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_8,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_I,
        KC_RALT,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_K,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_COMM,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    [MAC_FN] = LAYOUT_ansi_84(
        UG_TOGG,  KC_F15,   KC_F16,   KC_TASK,  KC_CALC,  KC_F17,   KC_F18,   KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_F19,   KC_F20,   KC_F21,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_F22,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_F23,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            KC_F24,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
    ),

    [WIN_BASE] = LAYOUT_ansi_84(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_F13,   KC_F14,   KC_DEL,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_8,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_I,
        KC_RALT,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_K,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_COMM,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                NAV_SPC,                                KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    [WIN_FN] = LAYOUT_ansi_84(
        UG_TOGG,  KC_F15,   KC_F16,   KC_TASK,  KC_CALC,  KC_F17,   KC_F18,   KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_F19,   KC_F20,   KC_F21,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_F22,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_F23,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            KC_F24,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
    )
};
