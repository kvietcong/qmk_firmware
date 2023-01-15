#define NO_ACTION_ONESHOT
#undef LOCKING_RESYNC_ENABLE
#undef LOCKING_SUPPORT_ENABLE

#define LAYER_STATE_16BIT

#define DYNAMIC_KEYMAP_LAYER_COUNT 16 // For Via

#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

#define LAYER_TIMEOUT 30000

#define IGNORE_MOD_TAP_INTERRUPT

#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY

#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

#define COMBO_TERM 75

#define AUTO_SHIFT_TIMEOUT 175

#define CAPS_WORD_IDLE_TIMEOUT 3000
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

#define MOUSEKEY_MOVE_DELTA 5
#define MOUSEKEY_WHEEL_MAX_SPEED 5

#define RGBLIGHT_LAYERS
#define RGBLIGHT_MAX_LAYERS 16
#define RGBLIGHT_TIMEOUT 300000
#define RGBLIGHT_EFFECT_BREATHING

#undef DEBOUNCE
#define DEBOUNCE 16
#define FORCE_NKRO
#define USB_POLLING_INTERVAL_MS 1

#undef RGBLED_NUM
#define RGBLED_NUM 10
#undef RGBLED_SPLIT
#define RGBLED_SPLIT { 5, 5 }
