 /* Copyright 2024 Efthimis Iosifidis <iosifidise@gmail.com>
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

enum layers {
    _BASIC,
    _VIM,
    _FN
};


enum custom_keycodes {
        SAVE_EXIT = SAFE_RANGE,
        DD,
        YY
};

#define KC_X0 LT(_FN, KC_ESC)

static char current_alpha_oled [12] = "None";

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* _BASIC Layer
 * ,-------------------------------------.
 * |         |         |        |         |
 * | Select  | New Tab |  Undo  | Search  |
 * | All     | New Term|        |         |
 * |---------+---------+--------+---------+
 * |         |         |        |   TO    |
 * |  Cut    |   Copy  |  Paste | _BASIC  |
 * |         |         |        |         |
 *  `-------------------------------------'
 */ 
 [_BASIC] = LAYOUT_ortho_2x4(
     LCTL(KC_A), LCTL(KC_T), LCTL(KC_Z), LCTL(KC_F),
     LCTL(KC_X), LCTL(KC_C), LCTL(KC_V),  TO(_VIM)
  ),
/* _VIM Layer
 * ,-------------------------------------.
 * |         | Save    |        |         |
 * |    F2   | & Exit  |  Undo  |  Redo   |
 * |         |         |        |         |
 * |---------+---------+--------+---------+
 * |         |         |        |   TO    |
 * |    dd   |   yy    |  Paste | _BASIC  |
 * |         |         |        |         |
 *  `-------------------------------------'
 * 
 * for the F2 to toggle between insert and 
 * command mode you need to include
 * the following two lines in .vimrc: 
 * 
 * " Toggle between insert and command mode
 * nnoremap <F2> i
 * inoremap <F2> <Esc>
 *  
 */ 
 [_VIM] = LAYOUT_ortho_2x4(
        KC_F2, SAVE_EXIT, KC_U, LCTL(KC_R),
        DD, YY, KC_P, TO(_FN) 
    ),
/* _FN Layer
 * ,--------------------------------------------.
 * | RGB       |  RGB      |  RGB      | RGB     |
 * | Toggle    |  Mode     |  Mode     | Snake   |
 * |           |  Forward  | Reverse   | Mode    |
 * |-----------+-----------+-----------+---------+
 * |           |  Cycle    |  Toggle   |  TO     |
 * | BackLight | BackLight | BackLight |  _BASIC |
 * |  Toggle   |   Levels  | Breathing |         |
 *  `--------------------------------------------'
 */ 
  [_FN] = LAYOUT_ortho_2x4(
     RGB_TOG, RGB_MOD, RGB_M_R, RGB_M_SN,
     BL_TOGG, BL_STEP, BL_BRTG, TO(_BASIC)
  )
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        char string [33];
        switch(keycode)
        {
              //First Layer with Basic Keys
            case LCTL(KC_X):
                strncpy(current_alpha_oled, "Cut", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_F):
                strncpy(current_alpha_oled, "Search", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_Z):
                strncpy(current_alpha_oled, "Undo", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_C):
                strncpy(current_alpha_oled, "Copy", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_V):
                strncpy(current_alpha_oled, "Paste", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_T):
                strncpy(current_alpha_oled, "NewTab", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_A):
                strncpy(current_alpha_oled, "Sel. All", sizeof(current_alpha_oled));
                break;
            //Second Layer with Vim Keys
            case KC_F2:
                strncpy(current_alpha_oled, "Toggle", sizeof(current_alpha_oled));
                break;
            case SAVE_EXIT:
            	if (record->event.pressed) {
                    SEND_STRING(":wq!\n");
                }
                strncpy(current_alpha_oled, "Exit", sizeof(current_alpha_oled));
                break;
            case KC_U:
                strncpy(current_alpha_oled, "Undo", sizeof(current_alpha_oled));
                break;
            case LCTL(KC_R):
                strncpy(current_alpha_oled, "Redo", sizeof(current_alpha_oled));
                break;
            case DD:
            	if (record->event.pressed) {
                    SEND_STRING("dd");
                }
                strncpy(current_alpha_oled, "dd", sizeof(current_alpha_oled));
                break;
            case YY:
            	if (record->event.pressed) {
                    SEND_STRING("yy");
                }
                strncpy(current_alpha_oled, "yy", sizeof(current_alpha_oled));
                break;
            case KC_P:
                strncpy(current_alpha_oled, "p", sizeof(current_alpha_oled));
                break;
            // FN Layer keys
            case RGB_TOG:
                strncpy(current_alpha_oled, "RGB Toggle", sizeof(current_alpha_oled));
                break;
            case RGB_MOD:
                strncpy(current_alpha_oled, "RGB Fwd", sizeof(current_alpha_oled));
                break;
            case RGB_M_R:
                strncpy(current_alpha_oled, "RGB Rev", sizeof(current_alpha_oled));
                break;
            case RGB_MODE_SNAKE:
                strncpy(current_alpha_oled, "RGB Snk", sizeof(current_alpha_oled));
                break;
            case BL_TOGG:
                strncpy(current_alpha_oled, "BkLgt Tog", sizeof(current_alpha_oled));
                break;
            case BL_STEP:
                strncpy(current_alpha_oled, "BkLgt Lvl", sizeof(current_alpha_oled));
                break;
            case BL_BRTG:
                strncpy(current_alpha_oled, "BkLgt Brth", sizeof(current_alpha_oled));
                break;
            //FN Key keycodes
            case TO(_BASIC)...TO(_FN):
               strncpy(current_alpha_oled, "Switcher", sizeof(current_alpha_oled));
               break;
            default:
               strncpy(current_alpha_oled, itoa(keycode, string, 10), sizeof(current_alpha_oled));
               break;
        }
    }
    return true;
}

bool oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_ln_P(PSTR("ANAVI Macro Pad 8"), false);
  oled_write_P(PSTR("Active layer: "), false);

  switch (get_highest_layer(layer_state)) {
    case _BASIC:
      oled_write_ln_P(PSTR("Basic"), false);
      break;
    case _VIM:
      oled_write_ln_P(PSTR("Vim"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR("FN"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR("N/A"), false);
  }


  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(PSTR("Num Lock: "), false);
  oled_write_ln_P(led_state.num_lock ? PSTR("On") : PSTR("Off"), false);
  oled_write_P(PSTR("Caps Lock: "), false);
  oled_write_ln_P(led_state.caps_lock ? PSTR("On") : PSTR("Off"), false);
  oled_write_P(PSTR("Backlit: "), false);
  oled_write_ln_P(is_backlight_enabled() ? PSTR("On") : PSTR("Off"), false);
  oled_write_P(PSTR("Last Key: "), false);
  oled_write_ln(current_alpha_oled, false);
#ifdef RGBLIGHT_ENABLE
  static char rgbStatusLine1[26] = {0};
  snprintf(rgbStatusLine1, sizeof(rgbStatusLine1), "RGB Mode: %d", rgblight_get_mode());
  oled_write_ln(rgbStatusLine1, false);
  static char rgbStatusLine2[26] = {0};
  snprintf(rgbStatusLine2, sizeof(rgbStatusLine2), "h:%d s:%d v:%d", rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
  oled_write_ln(rgbStatusLine2, false);
#endif
    return false;
}
#endif
