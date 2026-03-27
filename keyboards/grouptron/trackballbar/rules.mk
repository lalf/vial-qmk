# Add trackball support
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3360

# Disable Features
AUTO_SHIFT_ENABLE = no
AUTOCORRECT_ENABLE = no
CAPS_WORD_ENABLE = no

# Custom matrix
CUSTOM_MATRIX = lite
SRC += matrix_custom.c 

# EEPROM
 EEPROM_DRIVER = wear_leveling
 WEAR_LEVELING_DRIVER = rp2040_flash
