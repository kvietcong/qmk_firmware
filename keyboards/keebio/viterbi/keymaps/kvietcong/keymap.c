// Very much a work in progress. Very messy and bad. I will clean up once I've stabilized on something XD

#include QMK_KEYBOARD_H

#define _DEF 0
#define _GAM 1
#define _MOR 2
#define _NUM 3
#define _FUN 4
#define _SYM 5
#define _NAV 6
#define _QMK 7

void eeconfig_init_user(void) {
    rgblight_enable();
    rgblight_sethsv(HSV_WHITE);
    rgblight_mode(RGBLIGHT_EFFECT_BREATHING + 1);
}

// TODO: Figure out how to get breathing to work with layers. I might have to manually do it ;.;
const rgblight_segment_t PROGMEM rgb_sym[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 4, HSV_BLUE},
    {9, 1, HSV_BLUE}
);
const rgblight_segment_t PROGMEM rgb_nav[] = RGBLIGHT_LAYER_SEGMENTS(
    {5, 4, HSV_BLUE},
    {4, 1, HSV_BLUE}
);
const rgblight_segment_t PROGMEM rgb_fun[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 5, HSV_RED}
);
const rgblight_segment_t PROGMEM rgb_num[] = RGBLIGHT_LAYER_SEGMENTS(
    {5, 5, HSV_RED}
);
const rgblight_segment_t PROGMEM rgb_gam[] = RGBLIGHT_LAYER_SEGMENTS({0, 10, HSV_GREEN});
const rgblight_segment_t PROGMEM rgb_col[] = RGBLIGHT_LAYER_SEGMENTS({0, 10, HSV_PURPLE});
const rgblight_segment_t PROGMEM rgb_qmk[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 3, HSV_GOLD},
    {6, 3, HSV_GOLD}
);
const rgblight_segment_t PROGMEM rgb_CYAN[] = RGBLIGHT_LAYER_SEGMENTS({0, 10, HSV_CYAN});

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    rgb_gam,
    rgb_col,
    rgb_num,
    rgb_fun,
    rgb_sym,
    rgb_nav,
    rgb_qmk,
    rgb_CYAN
);

void keyboard_post_init_user(void) {
    rgblight_layers = my_rgb_layers;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    for (int i = _GAM; i <= _QMK; i++)
        rgblight_set_layer_state(i-1, layer_state_cmp(state, i));
    return state;
}

// Tap Dance
enum {
    // Advanced tap dances
    TD_HYPR_MEDIA,
    TD_RHYPR_BASE,
    TD_LHYPR_BASE,

    // Simple tap dances
    TD_HOME_END,
    TD_ENT_ESC,
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

static tap_state_t states[TD_LHYPR_BASE + 1];

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

void finish_lhypr_base(qk_tap_dance_state_t* state, void* user_data) {
    states[TD_LHYPR_BASE] = current_dance(state);
    switch (states[TD_LHYPR_BASE]) {
        case TD_SINGLE_TAP:
            tap_code(KC_TAB); break;

        case TD_SINGLE_TAP_INTERRUPTED:
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_TAP:
            layer_on(_NUM); break;

        default: break;
    }
}
void reset_lhypr_base(qk_tap_dance_state_t* state, void* user_data) {
    switch (states[TD_LHYPR_BASE]) {
        case TD_SINGLE_TAP_INTERRUPTED:
        case TD_SINGLE_HOLD:
            layer_off(_NUM); break;

        default: break;
    }
    states[TD_LHYPR_BASE] = TD_UNKNOWN;
}

void finish_rhypr_base(qk_tap_dance_state_t* state, void* user_data) {
    states[TD_RHYPR_BASE] = current_dance(state);
    switch (states[TD_RHYPR_BASE]) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_TAP_INTERRUPTED:
            tap_code(KC_QUOT); break;

        case TD_SINGLE_HOLD:
        case TD_DOUBLE_TAP:
            layer_on(_FUN); break;

        default: break;
    }
}
void reset_rhypr_base(qk_tap_dance_state_t* state, void* user_data) {
    switch (states[TD_RHYPR_BASE]) {
        case TD_SINGLE_HOLD:
            layer_off(_FUN); break;

        default: break;
    }
    states[TD_RHYPR_BASE] = TD_UNKNOWN;
}

void finish_hypr_media(qk_tap_dance_state_t* state, void* user_data) {
    states[TD_HYPR_MEDIA] = current_dance(state);
    switch (states[TD_HYPR_MEDIA]) {
        case TD_SINGLE_TAP: tap_code(KC_MPLY); break;
        case TD_SINGLE_HOLD: register_code(KC_MFFD); break;
        case TD_DOUBLE_TAP: tap_code(KC_MNXT); break;
        case TD_DOUBLE_HOLD: register_code(KC_MRWD); break;
        case TD_TRIPLE_TAP: tap_code(KC_MPRV); break;
        default: break;
    }
}
void reset_hypr_media(qk_tap_dance_state_t *state, void *user_data) {
    switch (states[TD_HYPR_MEDIA]) {
        case TD_SINGLE_HOLD: unregister_code(KC_MFFD); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_MRWD); break;
        default: break;
    }
    states[TD_HYPR_MEDIA] = TD_UNKNOWN;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_HYPR_MEDIA] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finish_hypr_media, reset_hypr_media),
    [TD_RHYPR_BASE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finish_rhypr_base, reset_rhypr_base),
    [TD_LHYPR_BASE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finish_lhypr_base, reset_lhypr_base),

    [TD_HOME_END] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
    [TD_ENT_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_ENT, KC_ESC),
};

const uint16_t ENT_T_ESC  = TD(TD_ENT_ESC);
const uint16_t HOME_T_END = TD(TD_HOME_END);

const uint16_t LHYPR   = TD(TD_LHYPR_BASE);
const uint16_t RHYPR   = TD(TD_RHYPR_BASE);
const uint16_t HYPR_MEDIA = TD(TD_HYPR_MEDIA);

#define RC(key) RCTL_T(key)
#define LC(key) LCTL_T(key)
#define RA(key) RALT_T(key)
#define LA(key) LALT_T(key)
#define RW(key) RWIN_T(key)
#define LW(key) WIN_T(key)
#define RS(key) RSFT_T(key)
#define LS(key) LSFT_T(key)
#define SPC_OR(layer) LT(layer, KC_SPC)
#define ENT_OR(layer) LT(layer, KC_ENT)
#define MOR(key) LT(_MOR, key)

const key_override_t **key_overrides = (const key_override_t *[]){
    &ko_make_basic(MOD_MASK_CTRL | MOD_MASK_SHIFT, RS(KC_BSPC), KC_DEL),
    &ko_make_basic(MOD_MASK_CTRL | MOD_MASK_SHIFT, ENT_OR(_SYM), KC_INS),
    &ko_make_basic(MOD_MASK_CTRL, KC_VOLU, KC_BRIU),
    &ko_make_basic(MOD_MASK_CTRL, KC_VOLD, KC_BRID),
    NULL
};

// LAYOUT
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEF] = LAYOUT_ortho_5x14(
            XXXXXXX , XXXXXXX      , KC_1     , KC_2      , KC_3     , KC_4       , KC_5         , /*    */ KC_6         , KC_7        , KC_8     , KC_9      , KC_0        , TG(_GAM) , XXXXXXX ,
            XXXXXXX , XXXXXXX      , KC_Q     , KC_W      , KC_E     , KC_R       , KC_T         , /*    */ KC_Y         , KC_U        , KC_I     , KC_O      , KC_P        , XXXXXXX  , XXXXXXX ,
            XXXXXXX , C(A(KC_DEL)) , LW(KC_A) , MOR(KC_S) , LA(KC_D) , LC(KC_F)   , LS(KC_G)     , /*    */ RS(KC_H)     , RC(KC_J)    , RA(KC_K) , MOR(KC_L) , LW(KC_SCLN) , XXXXXXX  , XXXXXXX ,
            XXXXXXX , G(KC_L)      , KC_Z     , KC_X      , KC_C     , KC_V       , KC_B         , /*    */ KC_N         , KC_M        , KC_COMM  , KC_DOT    , KC_SLSH     , KC_PSCR  , XXXXXXX ,
            XXXXXXX , KC_LCTL      , XXXXXXX  , XXXXXXX   , LHYPR    , LS(KC_ESC) , SPC_OR(_NAV) , /*    */ ENT_OR(_SYM) , RS(KC_BSPC) , RHYPR    , XXXXXXX   , XXXXXXX     , MO(_QMK) , XXXXXXX
            ),

    [_GAM] = LAYOUT_ortho_5x14(
            KC_ESC  , KC_GRV  , KC_1     , KC_2    , KC_3         , KC_4    , KC_5    , /*    */ KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , TG(_GAM) , KC_BSPC    ,
            KC_INS  , KC_TAB  , _______  , _______ , _______      , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , KC_LBRC  , KC_RBRC    ,
            KC_DEL  , KC_ESC  , KC_A     , KC_S    , KC_D         , KC_F    , KC_G    , /*    */ KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN , KC_QUOT  , KC_ENT     ,
            KC_F2   , KC_LSFT , _______  , _______ , _______      , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , KC_RSFT  , HOME_T_END ,
            KC_LCTL , _______ , KC_LCTL  , KC_LGUI , LALT_T(KC_Y) , _______ , KC_SPC  , /*    */ _______ , _______ , _______ , _______ , _______ , _______  , _______
            ),

    [_MOR] = LAYOUT_ortho_5x14(
            _______ , _______ , _______ , _______ , _______ , _______  , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , KC_HOME , KC_UP   , KC_END   , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , KC_LEFT , KC_DOWN , KC_RIGHT , KC_PGUP , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______  , KC_PGDN , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , KC_BTN1  , KC_BTN2 , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______
            ),

    [_NUM] = LAYOUT_ortho_5x14(
            _______ , _______ , _______ , _______ , _______  , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______  , _______ , _______ , /*    */ KC_PPLS , KC_7    , KC_8    , KC_9    , KC_PAST , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______  , _______ , _______ , /*    */ KC_EQL  , KC_4    , KC_5    , KC_6    , KC_DOT  , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______  , _______ , _______ , /*    */ KC_MINS , KC_1    , KC_2    , KC_3    , KC_SLSH , _______ , _______ ,
            _______ , _______ , _______ , _______ , TG(_NUM) , _______ , _______ , /*    */ _______ , KC_0    , _______  , _______ , _______ , _______ , _______
            ),

    [_FUN] = LAYOUT_ortho_5x14(
            _______ , _______ , _______ , _______ , _______ , _______ , _______    , /*    */ _______ , _______ , _______  , _______ , _______ , _______ , _______ ,
            _______ , _______ , KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_VOLU    , /*    */ _______ , _______ , _______  , _______ , _______ , _______ , _______ ,
            _______ , _______ , KC_F5   , KC_F6   , KC_F7   , KC_F8   , HYPR_MEDIA , /*    */ _______ , _______ , _______  , _______ , _______ , _______ , _______ ,
            _______ , _______ , KC_F9   , KC_F10  , KC_F11  , KC_F12  , KC_VOLD    , /*    */ _______ , _______ , _______  , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______    , /*    */ _______ , _______ , TG(_FUN) , _______ , _______ , _______ , _______
            ),

    [_SYM] = LAYOUT_ortho_5x14(
            _______ , _______ , _______ , _______    , _______    , _______    , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , S(KC_BSLS) , KC_LBRC    , KC_RBRC    , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , KC_GRV  , S(KC_MINS) , S(KC_9)    , S(KC_0)    , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , KC_BSLS    , S(KC_LBRC) , S(KC_RBRC) , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______    , _______    , _______    , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______
            ),

    // I gotta move the mouse movement keys into their own layer.
    [_NAV] = LAYOUT_ortho_5x14(
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ KC_HOME , KC_PGDN , KC_PGUP , KC_END  , _______ , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ KC_LEFT , KC_DOWN , KC_UP   , KC_RGHT , KC_BTN2 , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ KC_MS_L , KC_MS_D , KC_MS_U , KC_MS_R , KC_BTN1 , _______ , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______ , _______
            ),

    [_QMK] = LAYOUT_ortho_5x14(
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______  , _______ ,
            _______ , RGB_TOG , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______  , _______ ,
            _______ , RGB_MOD , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______  , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , _______  , _______ ,
            _______ , _______ , _______ , _______ , _______ , _______ , _______ , /*    */ _______ , _______ , _______ , _______ , _______ , TG(_QMK) , _______
            ),
};

// Leader Keys
void matrix_scan_user(void) {
    const uint32_t time_since_last_input = last_input_activity_elapsed();
    if (time_since_last_input > LAYER_TIMEOUT && !IS_LAYER_ON(_GAM)) layer_clear();

    if (time_since_last_input > RGBLIGHT_TIMEOUT) rgblight_disable_noeeprom();
    else if(!rgblight_is_enabled()) rgblight_enable_noeeprom();
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LW(KC_A):
        case RW(KC_SCLN):
            return TAPPING_TERM * 1.4;

        case LS(KC_G):
        case RS(KC_H):
        case LC(KC_F):
        case RC(KC_J):
        case LA(KC_D):
        case RA(KC_K):
        case MOR(KC_S):
        case MOR(KC_L):
            return TAPPING_TERM * 1.2;

        case LS(KC_ESC):
        case RS(KC_BSPC):
            return TAPPING_TERM;
    }
    return TAPPING_TERM;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LW(KC_A):
        case RW(KC_SCLN):
        case LS(KC_G):
        case RS(KC_H):
        case LC(KC_F):
        case RC(KC_J):
        case LA(KC_D):
        case RA(KC_K):
        case MOR(KC_S):
        case MOR(KC_L):
            return false;
    }
    return true;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RS(KC_BSPC):
        case LS(KC_ESC):
            return true;
    }
    return false;
}

bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_1 ... KC_0:
        case KC_QUOT ... KC_SLASH:
        case KC_MINUS ... KC_BACKSLASH:
            return true;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    }
    return true;
}
