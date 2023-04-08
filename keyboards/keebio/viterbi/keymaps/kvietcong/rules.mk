SRC += features/achordion.c

# VIA just stopped updating my keymaps without clearing EEPROM. I guess it wasn't working properly before.
# VIA_ENABLE = yes
LTO_ENABLE = yes
COMBO_ENABLE = yes
RGBLIGHT_ENABLE = yes
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes
DEBOUNCE_TYPE = asym_eager_defer_pk

AVR_USE_MINIMAL_PRINTF = yes
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
