#undef LOCKING_RESYNC_ENABLE
#undef LOCKING_SUPPORT_ENABLE

#define LAYER_STATE_8BIT
#define LAYER_TIMEOUT 30000
#define DYNAMIC_KEYMAP_LAYER_COUNT 8

#define TAPPING_TERM 225
#define TAPPING_TOGGLE 2
#define TAPPING_TERM_PER_KEY

#undef ONESHOT_TAP_TOGGLE
#define ONESHOT_TIMEOUT 2500

#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS
#define PERMISSIVE_HOLD_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_FORCE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

#define COMBO_TERM 60
#define COMBO_SHOULD_TRIGGER
#define COMBO_ONLY_FROM_LAYER 0

#define CAPS_WORD_IDLE_TIMEOUT 5000
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

#define MOUSEKEY_MOVE_DELTA 4
#define MOUSEKEY_WHEEL_MAX_SPEED 5

#undef DEBOUNCE
#define DEBOUNCE 16
#define FORCE_NKRO
#define USB_POLLING_INTERVAL_MS 1

#undef RGBLED_NUM
#define RGBLED_NUM 10
#undef RGBLED_SPLIT
#define RGBLED_SPLIT { 5, 5 }
#define RGBLIGHT_TIMEOUT 300000

#define LEADER_TIMEOUT 500
#define LEADER_PER_KEY_TIMING
#define LEADER_KEY_STRICT_KEY_PROCESSING

