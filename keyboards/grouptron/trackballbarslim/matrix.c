/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar
Copyright 2025 Lalf/GroupTRON

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "atomic_util.h"

#include "keymap_introspection.h"
#include "action_layer.h"

#ifdef VIAL_ENABLE
#include "vial.h"
#endif

#ifdef SPLIT_KEYBOARD
#    include "split_common/split_util.h"
#    include "split_common/transactions.h"

#    define ROWS_PER_HAND (MATRIX_ROWS / 2)
#else
#    define ROWS_PER_HAND (MATRIX_ROWS)
#endif

#if 1
#    define SPLIT_MUTABLE
#    define SPLIT_MUTABLE_ROW
#    define SPLIT_MUTABLE_COL
#else
#ifdef DIRECT_PINS_RIGHT
#    define SPLIT_MUTABLE
#else
#    define SPLIT_MUTABLE const
#endif
#ifdef MATRIX_ROW_PINS_RIGHT
#    define SPLIT_MUTABLE_ROW
#else
#    define SPLIT_MUTABLE_ROW const
#endif
#ifdef MATRIX_COL_PINS_RIGHT
#    define SPLIT_MUTABLE_COL
#else
#    define SPLIT_MUTABLE_COL const
#endif
#endif

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

#ifdef DIRECT_PINS
static SPLIT_MUTABLE pin_t direct_pins[ROWS_PER_HAND][MATRIX_COLS] = DIRECT_PINS;
#elif (DIODE_DIRECTION == ROW2COL) || (DIODE_DIRECTION == COL2ROW)
#    ifdef MATRIX_ROW_PINS
static SPLIT_MUTABLE_ROW pin_t row_pins[ROWS_PER_HAND] = MATRIX_ROW_PINS;
#    endif // MATRIX_ROW_PINS
#    ifdef MATRIX_COL_PINS
static SPLIT_MUTABLE_COL pin_t col_pins[MATRIX_COLS]   = MATRIX_COL_PINS;
#    endif // MATRIX_COL_PINS
#endif

/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
extern matrix_row_t matrix[MATRIX_ROWS];     // debounced values

#ifdef SPLIT_KEYBOARD
// row offsets for each hand
extern uint8_t thisHand, thatHand;
#endif

// user-defined overridable functions
__attribute__((weak)) void matrix_init_pins(void);
__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter);

static inline void gpio_atomic_set_pin_output_low(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_output(pin);
        gpio_write_pin_low(pin);
    }
}

static inline void gpio_atomic_set_pin_output_high(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_output(pin);
        gpio_write_pin_high(pin);
    }
}

static inline void gpio_atomic_set_pin_input_high(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_input_high(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (gpio_read_pin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
    } else {
        return 1;
    }
}

// matrix code

#ifdef DIRECT_PINS

__attribute__((weak)) void matrix_init_pins(void) {
    for (int row = 0; row < ROWS_PER_HAND; row++) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            pin_t pin = direct_pins[row][col];
            if (pin != NO_PIN) {
                gpio_set_pin_input_high(pin);
            }
        }
    }
}

__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        pin_t pin = direct_pins[current_row][col_index];
        current_row_value |= readMatrixPin(pin) ? 0 : row_shifter;
    }

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

#elif defined(DIODE_DIRECTION)
#    if defined(MATRIX_ROW_PINS) && defined(MATRIX_COL_PINS)
#        if (DIODE_DIRECTION == COL2ROW)

static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        gpio_atomic_set_pin_output_low(pin);
        return true;
    }
    return false;
}

static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_atomic_set_pin_output_high(pin);
#            else
        gpio_atomic_set_pin_input_high(pin);
#            endif
    }
}

static void unselect_rows(void) {
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        unselect_row(x);
    }
}

__attribute__((weak)) void matrix_init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            gpio_atomic_set_pin_input_high(col_pins[x]);
        }
    }
}

__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_output_select_delay();

    // For each col...
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        uint8_t pin_state = readMatrixPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : row_shifter;
    }

    // Unselect row
    unselect_row(current_row);
    matrix_output_unselect_delay(current_row, current_row_value != 0); // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

#        elif (DIODE_DIRECTION == ROW2COL)

static bool select_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
        gpio_atomic_set_pin_output_low(pin);
        return true;
    }
    return false;
}

static void unselect_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_atomic_set_pin_output_high(pin);
#            else
        gpio_atomic_set_pin_input_high(pin);
#            endif
    }
}

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        unselect_col(x);
    }
}

__attribute__((weak)) void matrix_init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        if (row_pins[x] != NO_PIN) {
            gpio_atomic_set_pin_input_high(row_pins[x]);
        }
    }
}

__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    bool key_pressed = false;

    // Select col
    if (!select_col(current_col)) { // select col
        return;                     // skip NO_PIN col
    }
    matrix_output_select_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < ROWS_PER_HAND; row_index++) {
        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
            key_pressed = true;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed); // wait for all Row signals to go HIGH
}

#        else
#            error DIODE_DIRECTION must be one of COL2ROW or ROW2COL!
#        endif
#    endif // defined(MATRIX_ROW_PINS) && defined(MATRIX_COL_PINS)
#else
#    error DIODE_DIRECTION is not defined!
#endif

#if 0
void matrix_init(void) {
#ifdef SPLIT_KEYBOARD
    // Set pinout for right half if pinout for that half is defined
    if (!isLeftHand) {
#    ifdef DIRECT_PINS_RIGHT
        const pin_t direct_pins_right[ROWS_PER_HAND][MATRIX_COLS] = DIRECT_PINS_RIGHT;
        for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
            for (uint8_t j = 0; j < MATRIX_COLS; j++) {
                direct_pins[i][j] = direct_pins_right[i][j];
            }
        }
#    endif
#    ifdef MATRIX_ROW_PINS_RIGHT
        const pin_t row_pins_right[ROWS_PER_HAND] = MATRIX_ROW_PINS_RIGHT;
        for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
            row_pins[i] = row_pins_right[i];
        }
#    endif
#    ifdef MATRIX_COL_PINS_RIGHT
        const pin_t col_pins_right[MATRIX_COLS] = MATRIX_COL_PINS_RIGHT;
        for (uint8_t i = 0; i < MATRIX_COLS; i++) {
            col_pins[i] = col_pins_right[i];
        }
#    endif
    }

    thisHand = isLeftHand ? 0 : (ROWS_PER_HAND);
    thatHand = ROWS_PER_HAND - thisHand;
#endif

    // initialize key pins
    matrix_init_pins();

    // initialize matrix state: all keys off
    memset(matrix, 0, sizeof(matrix));
    memset(raw_matrix, 0, sizeof(raw_matrix));

    debounce_init(ROWS_PER_HAND);

    matrix_init_kb();
}

#ifdef SPLIT_KEYBOARD
// Fallback implementation for keyboards not using the standard split_util.c
__attribute__((weak)) bool transport_master_if_connected(matrix_row_t master_matrix[], matrix_row_t slave_matrix[]) {
    transport_master(master_matrix, slave_matrix);
    return true; // Treat the transport as always connected
}
#endif

uint8_t matrix_scan(void) {
    matrix_row_t curr_matrix[MATRIX_ROWS] = {0};

#if defined(DIRECT_PINS) || (DIODE_DIRECTION == COL2ROW)
    // Set row, read cols
    for (uint8_t current_row = 0; current_row < ROWS_PER_HAND; current_row++) {
        matrix_read_cols_on_row(curr_matrix, current_row);
    }
#elif (DIODE_DIRECTION == ROW2COL)
    // Set col, read rows
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(curr_matrix, current_col, row_shifter);
    }
#endif

    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));

#ifdef SPLIT_KEYBOARD
    changed = debounce(raw_matrix, matrix + thisHand, ROWS_PER_HAND, changed) | matrix_post_scan();
#else
    changed = debounce(raw_matrix, matrix, ROWS_PER_HAND, changed);
    matrix_scan_kb();
#endif
    return (uint8_t)changed;
}
#endif

// ----------------------------------------------------------------------------------
// for TrackBallBar Slim

#define MATRIX_REAL_COLS	3
#define MATRIX_REAL_ROWS	4
#define BUTTONS				6
#ifndef COMBO_TERM
#define COMBO_TERM			200
#endif

void matrix_init_custom(void)
{
	matrix_init_pins();
}

bool matrix_scan_custom(matrix_row_t* current_matrix)
{
	bool changed = false;
	matrix_row_t raw_matrix[MATRIX_REAL_ROWS] = {0}, bak_matrix[MATRIX_ROWS] = {0};
	bool button[BUTTONS] = {false}, set[BUTTONS] = {false};
	static bool combo_button[BUTTONS] = {false}, combo_matrix[BUTTONS][BUTTONS] = {false};
	static int count[BUTTONS] = {0};
	static uint8_t before_layer = 255;
	uint8_t layer;
	uint16_t keycode = KC_NO;

	memcpy(bak_matrix, current_matrix, sizeof(bak_matrix));

	layer = get_highest_layer(layer_state | default_layer_state);

	// layer change -> waiting button on
	if (before_layer != layer) {
		before_layer = layer;
		for (uint8_t b1 = 0; b1 < BUTTONS; b1++) {
			if (count[b1] > 1) {
				current_matrix[b1] |= (1 << b1);
				count[b1] = 0;
			}
		}
		// search combo button
		for (uint8_t b1 = 0; b1 < BUTTONS; b1++) {
			combo_button[b1] = false;
			for (uint8_t b2 = b1 + 1; b2 < BUTTONS; b2++) {
				combo_matrix[b1][b2] = false;
			}
		}
		
		for (uint8_t b1 = 0; b1 < BUTTONS; b1++) {
			combo_button[b1] = false;
			for (uint8_t b2 = b1 + 1; b2 < BUTTONS; b2++) {
				for (uint8_t l = layer; l >= 0; l--) {
					uint16_t keycode2 = keycode_at_keymap_location(l, b1, b2);
					if (keycode2 != KC_TRANSPARENT) {
						keycode = keycode2;
						break;
					}
				}
				if (keycode != KC_NO) {
					combo_button[b1] = true;
					combo_button[b2] = true;
					combo_matrix[b1][b2] = true;
				}
			}
		}
	}

	// read raw matrix
	for (uint8_t current_row = 0; current_row < MATRIX_REAL_ROWS; current_row++) {
		matrix_read_cols_on_row(raw_matrix, current_row);
	}

	// B0 0,0
	if ((raw_matrix[0] & 1) != 0) {
		button[0] = true;
		set[0] = true;
	}
	// B1 2,1
	if ((raw_matrix[2] & (1 << 1)) != 0) {
		button[1] = true;
		set[1] = true;
	}
	// B2 2,2
	if ((raw_matrix[2] & (1 << 2)) != 0) {
		button[2] = true;
		set[2] = true;
	}
	// B3 1,0
	if ((raw_matrix[1] & 1) != 0) {
		button[3] = true;
		set[3] = true;
	}
	// B4 3,1
	if ((raw_matrix[3] & (1 << 1)) != 0) {
		button[4] = true;
		set[4] = true;
	}
	// B5 3,2
	if ((raw_matrix[3] & (1 << 2)) != 0) {
		button[5] = true;
		set[5] = true;
	}

	#ifdef VIAL_ENABLE
	// disable combo when vial unlock in progress
	if (vial_unlock_in_progress == 0) {
	#endif
		// counter progress
		for (uint8_t b1 = 0; b1 < BUTTONS; b1++) {
			if (combo_button[b1] == true) {
				if (button[b1] == true) {
					if (count[b1] != 1) {
						set[b1] = false;

						if (count[b1] == 0) {
							count[b1] = COMBO_TERM;
						} else {
							count[b1]--;
						}
					} else {
						set[b1] = true;
					}
				} else {
					if (count[b1] > 1) {
						set[b1] = true;
					}
					count[b1] = 0;
				}
			}
		}

		// combo check
		for (uint8_t b1 = 0; b1 < BUTTONS; b1++) {
			for (uint8_t b2 = b1 + 1; b2 < BUTTONS; b2++) {
				if (combo_matrix[b1][b2] == true) {
					// button1:on button2:on -> combo set
					if (button[b1] == true && button[b2] == true) {
						count[b1] = 1;
						count[b2] = 1;
						set[b1] = false;
						set[b2] = false;
						current_matrix[b1] |= (1 << b2);
					// when combo on
					} else if ((current_matrix[b1] & (1 << b2)) != 0) {
						// button1:off button2:off -> combo reset
						if (button[b1] == false && button[b2] == false) {
							current_matrix[b1] &= ~(1 << b2);
						// button1:on button2:off or button1:off button2:on -> button1:off button2:off
						} else {
							set[b1] = false;
							set[b2] = false;
						}
					}
				}
			}
		}
	#ifdef VIAL_ENABLE
	}
	#endif

	for (uint8_t b1 = 0; b1 < BUTTONS; b1++) {
		if (set[b1] == true) {
			current_matrix[b1] |= (1 << b1);
		} else {
			current_matrix[b1] &= ~(1 << b1);
		}
	}

	if (memcmp(bak_matrix, current_matrix, sizeof(bak_matrix)) != 0) {
		changed = true;
	}
	return changed;
}
