// Very much a work in progress. Very messy and bad. I will clean up once I've stabilized on something XD

#include QMK_KEYBOARD_H
#include "features/achordion.h"

enum layers {
    _DEF,
    _GA1,
    _GA2,
    _GA3,
    _MOU,
    _NUM,
    _FUN,
    _SYM,
    _NAV,
};

void eeconfig_init_user(void) {
    rgblight_enable();
    rgblight_sethsv(HSV_WHITE);
}

void oneshot_mods_changed_user(uint8_t mods) {
    if (mods) rgblight_sethsv(HSV_RED);
    else layer_state_set(layer_state);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_oneshot_mods()) return state;

    // TODO: Figure out how to set ranges on peripheral side
    switch (get_highest_layer(state)) {
        case _GA1:
            rgblight_sethsv(HSV_GREEN);
            break;
        case _GA2:
            rgblight_sethsv(HSV_RED);
            break;
        case _NAV:
            rgblight_sethsv(HSV_BLUE);
            break;
        case _NUM:
            rgblight_sethsv(HSV_CHARTREUSE);
            break;
        case _FUN:
            rgblight_sethsv(HSV_GOLD);
            break;
        case _SYM:
            rgblight_sethsv(HSV_MAGENTA);
            break;
        case _MOU:
            rgblight_sethsv(HSV_ORANGE);
            break;
        default:
            rgblight_sethsv(HSV_WHITE);
            break;
    }

    return state;
}

// Tap Dance
enum tap_dances {
    // Advanced tap dances
    TD_S_MEDIA,
    TD_GHLT2,

    // Intermediate tap dances
    TD_F1_,
    TD_F5_,
    TD_F9_,

    // Simple tap dances
    TD_HOME_END,
    TD_ENT_ESC,
};

enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,
    OS_CLR,
};

typedef enum {
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_TAP_INTERRUPTED,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_TAP_INTERRUPTED,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
    TD_TRIPLE_TAP_INTERRUPTED,
} tap_state_t;

static tap_state_t states[TD_GHLT2 + 1];

// TODO: Is this even needed. I should just check state manually?
tap_state_t current_dance(qk_tap_dance_state_t* state) {
    if (state->count == 1) {
        if (state->interrupted) return TD_SINGLE_TAP_INTERRUPTED;
        else if (state->pressed) return TD_SINGLE_HOLD;
        else return TD_SINGLE_TAP;
    } else if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_TAP_INTERRUPTED;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    } else if (state->count == 3) {
        if (state->interrupted) return TD_TRIPLE_TAP_INTERRUPTED;
        else if (state->pressed) return TD_TRIPLE_HOLD;
        else return TD_TRIPLE_TAP;
    } else return TD_UNKNOWN;
}

void finish_ghlt2(qk_tap_dance_state_t* state, void* user_data) {
    states[TD_GHLT2] = current_dance(state);
    switch (states[TD_GHLT2]) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            register_code(KC_LSFT); break;

        case TD_DOUBLE_TAP:
        case TD_DOUBLE_TAP_INTERRUPTED:
            tap_code(KC_ESC); break;

        case TD_DOUBLE_HOLD:
        case TD_SINGLE_TAP_INTERRUPTED:
            layer_on(_GA2); break;

        default: break;
    }
}
void reset_ghlt2(qk_tap_dance_state_t* state, void* user_data) {
    switch (states[TD_GHLT2]) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            unregister_code(KC_LSFT); break;

        case TD_DOUBLE_HOLD:
        case TD_SINGLE_TAP_INTERRUPTED:
            layer_off(_GA2); break;

        default: break;
    }
    states[TD_GHLT2] = TD_UNKNOWN;
}

void finish_s_media(qk_tap_dance_state_t* state, void* user_data) {
    states[TD_S_MEDIA] = current_dance(state);
    switch (states[TD_S_MEDIA]) {
        case TD_SINGLE_TAP: tap_code(KC_MPLY); break;
        case TD_SINGLE_HOLD: register_code(KC_MFFD); break;
        case TD_DOUBLE_TAP: tap_code(KC_MNXT); break;
        case TD_DOUBLE_HOLD: register_code(KC_MRWD); break;
        case TD_TRIPLE_TAP: tap_code(KC_MPRV); break;
        default: break;
    }
}
void reset_s_media(qk_tap_dance_state_t *state, void *user_data) {
    switch (states[TD_S_MEDIA]) {
        case TD_SINGLE_HOLD: unregister_code(KC_MFFD); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_MRWD); break;
        default: break;
    }
    states[TD_S_MEDIA] = TD_UNKNOWN;
}

void f1_(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1 ... 12: tap_code(KC_F1 + state->count - 1);
    }
    reset_tap_dance(state);
}

void f5_(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1 ... 8: tap_code(KC_F5 + state->count - 1);
    }
    reset_tap_dance(state);
}

void f9_(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1 ... 4: tap_code(KC_F9 + state->count - 1);
    }
    reset_tap_dance(state);
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_S_MEDIA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finish_s_media, reset_s_media),
    [TD_GHLT2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finish_ghlt2, reset_ghlt2),

    [TD_F1_] = ACTION_TAP_DANCE_FN(f1_),
    [TD_F5_] = ACTION_TAP_DANCE_FN(f5_),
    [TD_F9_] = ACTION_TAP_DANCE_FN(f9_),

    [TD_HOME_END] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
    [TD_ENT_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_ENT, KC_ESC),
};

#define SNIP G(S(KC_S))
#define LOCK G(KC_L)
#define TASKS C(S(KC_ESC))
#define CLOSE A(KC_F4)

#define F1_ TD(TD_F1_)
#define F5_ TD(TD_F5_)
#define F9_ TD(TD_F9_)
#define S_MEDIA TD(TD_S_MEDIA)
#define Z_UN LT(0, KC_Z)
#define X_CU LT(0, KC_X)
#define C_CO LT(0, KC_C)
#define V_PA LT(0, KC_V)
#define R_SN LT(0, KC_R)

#define HLT1 LT(_FUN, KC_SPC)
#define HLT2 LSFT_T(KC_ESC)
#define HLP WIN_T(KC_A)
#define HLR LALT_T(KC_S)
#define HLM LT(_NUM, KC_D)
#define HLI LCTL_T(KC_F)
#define HLX LSFT_T(KC_G)

#define HRT1 LT(_NAV, KC_BSPC)
#define HRT2 RSFT_T(KC_ENT)
#define HRP RWIN_T(KC_SCLN)
#define HRR RALT_T(KC_L)
#define HRM LT(_SYM, KC_K)
#define HRI RCTL_T(KC_J)
#define HRX RSFT_T(KC_H)

#define URM LT(_MOU, KC_I)

#define GHLT2 LT(_GA2, KC_ESC)

static bool is_alt_tab_active = false;
static uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM capsword[] = {HLT2, HRT2, COMBO_END};
const uint16_t PROGMEM tg_ga1[]   = {HLT2, KC_B, COMBO_END};
const uint16_t PROGMEM tab[]      = {HLT2, HLM, COMBO_END};
const uint16_t PROGMEM quote[]    = {HRT2, HRM, COMBO_END};
const uint16_t PROGMEM delete[]    = {HRT1, HRI, COMBO_END};
combo_t key_combos[] = {
    // Always available
    COMBO(capsword, CW_TOGG),
    COMBO(tg_ga1, TG(_GA1)),
    COMBO(quote, KC_QUOT),
    COMBO(delete, KC_DEL),

    // Disabled when gaming
    COMBO(tab, KC_TAB),

    // Gaming only
};
uint16_t COMBO_LEN = ARRAY_SIZE(key_combos);

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch (combo_index) {
        case 4 ... 4:
            return !IS_LAYER_ON(_GA1);
        /* case 4 ... 100: */
        /*     return IS_LAYER_ON(_GA1); */
        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_DEF] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_W    , KC_E    , R_SN    , KC_T    , _______ , /*    */ _______ , KC_Y    , KC_U    , URM     , KC_O    , _______ , XXXXXXX ,
    XXXXXXX , KC_Q    , HLR     , HLM     , HLI     , HLX     , _______ , /*    */ _______ , HRX     , HRI     , HRM     , HRR     , KC_P    , XXXXXXX ,
    XXXXXXX , HLP     , X_CU    , C_CO    , V_PA    , KC_B    , _______ , /*    */ _______ , KC_N    , KC_M    , KC_COMM , KC_DOT  , HRP     , XXXXXXX ,
    XXXXXXX , Z_UN    , _______ , _______ , _______ , HLT2    , HLT1    , /*    */ HRT1    , HRT2    , _______ , _______ , _______ , KC_SLSH , XXXXXXX
),

[_GA1] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______ , _______ , _______  , _______ , /*    */ _______ , _______ , _______  , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_W    , KC_E    , KC_R    , KC_T     , _______ , /*    */ _______ , KC_Y    , KC_U     , KC_I    , KC_O    , _______ , XXXXXXX ,
    XXXXXXX , KC_Q    , KC_S    , KC_D    , KC_F    , KC_G     , _______ , /*    */ _______ , KC_H    , KC_J     , KC_K    , KC_L    , KC_P    , XXXXXXX ,
    XXXXXXX , KC_A    , KC_X    , KC_C    , KC_V    , KC_B     , _______ , /*    */ _______ , KC_N    , KC_M     , KC_COMM , KC_DOT  , KC_SCLN , XXXXXXX ,
    XXXXXXX , KC_Z    , _______ , _______ , KC_LCTL , GHLT2    , KC_SPC  , /*    */ _______ , _______ , TG(_GA1) , _______ , _______ , KC_SLSH , XXXXXXX
),

[_GA2] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______   , _______ , _______ , _______   , /*    */ _______  , _______  , _______  , _______  , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_1    , _______   , KC_2    , KC_TAB  , _______   , /*    */ _______  , _______  , _______  , _______  , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_LCTL , _______ , _______   , _______ , KC_3    , _______   , /*    */ _______  , _______  , _______  , MO(_SYM) , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_LSFT , KC_5    , OSL(_GA3) , KC_4    , KC_GRV  , _______   , /*    */ _______  , _______  , _______  , _______  , _______ , KC_LGUI , XXXXXXX ,
    XXXXXXX , KC_LALT , _______ , _______   , OS_CLR  , KC_ESC  , OSL(_GA3) , /*    */ MO(_NUM) , MO(_FUN) , TG(_GA2) , _______  , _______ , _______ , XXXXXXX
),

[_GA3] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______  , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______  , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , S(KC_F2) , _______ , KC_F2   , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______  , KC_LGUI , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______  , _______ , OS_CLR  , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX
),

[_MOU] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_WH_L , KC_MS_U , KC_WH_R , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_MS_L , KC_MS_D , KC_MS_R , KC_WH_U , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_BTN1 , _______ , _______ , _______ , KC_WH_D , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_BTN2 , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX
),

[_NUM] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , KC_PPLS , KC_7    , KC_8    , KC_9    , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , KC_EQL  , KC_4    , KC_5    , KC_6    , KC_PAST , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , KC_MINS , KC_1    , KC_2    , KC_3    , KC_DOT  , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , KC_0    , _______ , _______ , _______ , KC_SLSH , XXXXXXX
),

[_FUN] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , ALT_TAB , _______ , /*    */ _______ , _______ , _______ , _______ , CLOSE   , _______ , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , F1_     , F5_     , F9_     , KC_VOLU , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , SNIP    , LOCK    , TASKS   , S_MEDIA , XXXXXXX ,
    XXXXXXX , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , KC_VOLD , XXXXXXX
),

[_SYM] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______    , _______    , _______    , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_QUOT    , KC_LBRC    , KC_RBRC    , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , S(KC_MINS) , S(KC_9)    , S(KC_0)    , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_GRV  , S(KC_QUOT) , S(KC_LBRC) , S(KC_RBRC) , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_BSLS , _______    , _______    , _______    , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX
),

[_NAV] = LAYOUT_ortho_5x14(
    XXXXXXX , _______ , _______ , _______ , _______  , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , _______ , KC_HOME , KC_UP   , KC_END   , ALT_TAB , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_INS  , KC_LEFT , KC_DOWN , KC_RIGHT , KC_PGUP , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_TAB  , _______ , _______ , _______  , KC_PGDN , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX ,
    XXXXXXX , KC_DEL  , _______ , _______ , _______  , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , XXXXXXX
),

};

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
    switch (tap_hold_keycode) {
        case HLT1:
        case HRT1:
        case HLT2:
        case HRT2:

        case Z_UN:
        case X_CU:
        case C_CO:
        case V_PA:
        case R_SN:

        case GHLT2:
            return true;
    }
    return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    switch (tap_hold_keycode) {
        case Z_UN:
        case X_CU:
        case C_CO:
        case V_PA:
        case R_SN:
            return 0;

        case HLP:
        case HRP:
            return TAPPING_TERM * 1.5;

        case GHLT2:
            return TAPPING_TERM / 2;
    }
    return TAPPING_TERM * 2;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        case MOD_LCTL:
        case MOD_RCTL:
            return true;

        default:
            return false;
    }
}

void matrix_scan_user(void) {
    achordion_task();

    const uint32_t time_since_last_input = last_input_activity_elapsed();
    if (time_since_last_input > LAYER_TIMEOUT && !IS_LAYER_ON(_GA1)) layer_clear();

    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 500) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }

    if (time_since_last_input > RGBLIGHT_TIMEOUT) rgblight_disable_noeeprom();
    else if(!rgblight_is_enabled()) rgblight_enable_noeeprom();
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    uint16_t tapping_term = TAPPING_TERM;
    switch (keycode) {
        case HLP:
        case HLR:
        case HLM:
        case HLI:
        case HRP:
        case HRR:
        case HRM:
        case HRI:

        case Z_UN:
        case X_CU:
        case C_CO:
        case V_PA:
        case R_SN:
            return tapping_term * 1.1;

        case GHLT2:
            return tapping_term * 0.5;
    }
    return tapping_term;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HLT1:
        /* case HLT2: */
        case HLP:
        /* case HLR: */
        /* case HLM: */
        /* case HLI: */
        /* case HLX: */
        /* case HRT1: */
        /* case HRT2: */
        case HRP:
        /* case HRR: */
        /* case HRM: */
        /* case HRI: */
        /* case HRX: */
        /* case GHLP: */
        case Z_UN:
        case X_CU:
        case C_CO:
        case V_PA:
        case R_SN:
            return false;
    }
    return true;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HLT2:
        case HRT1:
        case HRT2:
        case GHLT2:
            return true;
    }
    return false;
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HLT2:
        case HRT2:
        case GHLT2:
            return true;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_achordion(keycode, record)) return false;

    switch (keycode) {
        case ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        case OS_CLR:
            clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            clear_oneshot_mods();
            break;
    }

    // Handle mod tap holds
    if (!record->tap.count && record->event.pressed) {
        switch (keycode) {
            case Z_UN:
                tap_code16(C(KC_Z));
                return false;
            case X_CU:
                tap_code16(C(KC_X));
                return false;
            case C_CO:
                tap_code16(C(KC_C));
                return false;
            case V_PA:
                tap_code16(C(KC_V));
                return false;
            case R_SN:
                tap_code16(G(S(KC_S)));
                return false;
        }
    }

    return true;
}

