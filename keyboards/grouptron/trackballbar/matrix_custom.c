/* Copyright 2026 Lalf/GroupTRON */
/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "matrix_custom.h"
#include "atomic_util.h"
#include "keymap_introspection.h"

#ifdef VIAL_ENABLE
#include "vial.h"
#endif

// [matrix]
#define MATRIX_TB_COLS		2
#define MATRIX_TB_ROWS		3
#define MATRIX_TB_COL_PINS	{ GP26, GP27 }
#define MATRIX_TB_ROW_PINS	{ GP28, GP25, GP29 }

#define MATRIX_KEY_COLS		14
#define MATRIX_KEY_ROWS		14
#define MATRIX_KEY_COL_PINS	{ GP0,  GP1,  GP2, GP3,  GP4,  GP5,  GP6,  GP7,  GP8,  GP9,  GP10, GP11, GP12, GP13 }
#define MATRIX_KEY_ROW_PINS	{ GP0,  GP1,  GP2, GP3,  GP4,  GP5,  GP6,  GP7,  GP8,  GP9,  GP10, GP11, GP12, GP13 }

#define MATRIX_TB_ROW		MATRIX_KEY_ROWS

static const pin_t tb_col_pins[MATRIX_TB_COLS] = MATRIX_TB_COL_PINS;
static const pin_t tb_row_pins[MATRIX_TB_ROWS] = MATRIX_TB_ROW_PINS;

static const pin_t key_col_pins[MATRIX_KEY_COLS] = MATRIX_KEY_COL_PINS;
static const pin_t key_row_pins[MATRIX_KEY_ROWS] = MATRIX_KEY_ROW_PINS;

// [combo]
#define TB_BUTTONS			6
#ifndef COMBO_TERM
#define COMBO_TERM			200
#endif
#define PUSH_TERM			50

// from matrix.c (COL2ROW) --------------------------------------------------
#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

static inline void gpio_atomic_set_pin_output_low(pin_t pin)
{
	ATOMIC_BLOCK_FORCEON {
		gpio_set_pin_output(pin);
		gpio_write_pin_low(pin);
	}
}

static inline void gpio_atomic_set_pin_output_high(pin_t pin)
{
	ATOMIC_BLOCK_FORCEON {
		gpio_set_pin_output(pin);
		gpio_write_pin_high(pin);
	}
}

static inline void gpio_atomic_set_pin_input_high(pin_t pin)
{
	ATOMIC_BLOCK_FORCEON {
		gpio_set_pin_input_high(pin);
	}
}
// --------------------------------------------------------------------------
void matrix_init_custom(void)
{
	for (uint8_t row = 0; row < MATRIX_KEY_ROWS; row++) {
		#ifdef MATRIX_UNSELECT_DRIVE_HIGH
			gpio_atomic_set_pin_output_high(key_row_pins[row]);
		else
			gpio_atomic_set_pin_input_high(key_row_pins[row]);
		#endif
	}
	for (uint8_t col = 0; col < MATRIX_KEY_COLS; col++) {
		gpio_atomic_set_pin_input_high(key_col_pins[col]);
	}
	for (uint8_t row = 0; row < MATRIX_TB_ROWS; row++) {
		#ifdef MATRIX_UNSELECT_DRIVE_HIGH
			gpio_atomic_set_pin_output_high(tb_row_pins[row]);
		else
			gpio_atomic_set_pin_input_high(tb_row_pins[row]);
		#endif
	}
	for (uint8_t col = 0; col < MATRIX_TB_COLS; col++) {
		gpio_atomic_set_pin_input_high(tb_col_pins[col]);
	}
}

bool matrix_scan_custom(matrix_row_t* matrix)
{
	bool changed = false;
	matrix_row_t bak_matrix[MATRIX_ROWS] = {0},
	             tb_matrix[MATRIX_TB_ROWS] = {0},
	             tb_row_state, row_state, col_bit;
	pin_t pin_col, pin_row;
	uint8_t pin_state;
	bool button[TB_BUTTONS] = {false}, set[TB_BUTTONS] = {false};
	static bool combo_button[TB_BUTTONS] = {false}, combo_matrix[TB_BUTTONS][TB_BUTTONS] = {false};
	static int count[TB_BUTTONS] = {0}, push_count[TB_BUTTONS] = {0};
	static uint8_t before_layer = 255;
	uint8_t layer, row, col;
	uint16_t keycode = KC_NO;
	/* combomap[TB_BUTTONS][TB_BUTTONS][row/col] */
	const int8_t combomap[TB_BUTTONS][TB_BUTTONS][2] = {
		{
			/* b1,b1 */ { -1, -1 },
			/* b1,b2 */ {  0,  0 },
			/* b1,b3 */ {  1,  1 },
			/* b1,b4 */ {  2,  2 },
			/* b1,b5 */ {  3,  3 },
			/* b1,b6 */ {  4,  4 }
		},
		{
			/* b2,b1 */ { -1, -1 },
			/* b2,b2 */ { -1, -1 },
			/* b2,b3 */ {  5,  5 },
			/* b2,b4 */ {  6,  6 },
			/* b2,b5 */ {  7,  7 },
			/* b2,b6 */ {  8,  8 }
		},
		{
			/* b3,b1 */ { -1, -1 },
			/* b3,b2 */ { -1, -1 },
			/* b3,b3 */ { -1, -1 },
			/* b3,b4 */ {  9,  9 },
			/* b3,b5 */ { 10, 10 },
			/* b3,b6 */ { 11, 11 }
		},
		{
			/* b4,b1 */ { -1, -1 },
			/* b4,b2 */ { -1, -1 },
			/* b4,b3 */ { -1, -1 },
			/* b4,b4 */ { -1, -1 },
			/* b4,b5 */ { 12, 12 },
			/* b4,b6 */ { 13, 13 }
		},
		{
			/* b5,b1 */ { -1, -1 },
			/* b5,b2 */ { -1, -1 },
			/* b5,b3 */ { -1, -1 },
			/* b5,b4 */ { -1, -1 },
			/* b5,b5 */ { -1, -1 },
			/* b5,b6 */ { 14, 13 }
		},
		{
			/* b6,b1 */ { -1, -1 },
			/* b6,b2 */ { -1, -1 },
			/* b6,b3 */ { -1, -1 },
			/* b6,b4 */ { -1, -1 },
			/* b6,b5 */ { -1, -1 },
			/* b6,b6 */ { -1, -1 },
		}
	};

	// copy current matrix
	memcpy(bak_matrix, matrix, sizeof(bak_matrix));

	// read key matrix
	for (row = 0; row < MATRIX_KEY_ROWS; row++) {
		row_state = 0;
		// row pin set low
		pin_row = key_row_pins[row];
		gpio_atomic_set_pin_output_low(pin_row);
		matrix_output_select_delay();

		// read col pin
		col_bit = MATRIX_ROW_SHIFTER;
		for (col = 0; col < MATRIX_KEY_COLS; col++, col_bit <<= 1) {
			pin_col = key_col_pins[col];
			if (pin_col != pin_row) {
				pin_state = gpio_read_pin(pin_col);
				if (pin_state == MATRIX_INPUT_PRESSED_STATE)
					row_state |= col_bit;
			}
		}

		// row pin unset
		#ifdef MATRIX_UNSELECT_DRIVE_HIGH
		gpio_atomic_set_pin_output_high(pin_row);
		#else
		gpio_atomic_set_pin_input_high(pin_row);
		#endif
		matrix_output_unselect_delay(pin_row, row_state != 0);

		matrix[row] = row_state;
	}

	// read trackball matrix
	for (row = 0; row < MATRIX_TB_ROWS; row++) {
		row_state = 0;
		// row pin set low
		pin_row = tb_row_pins[row];
		gpio_atomic_set_pin_output_low(pin_row);
		matrix_output_select_delay();

		// read col pin
		col_bit = MATRIX_ROW_SHIFTER;
		for (col = 0; col < MATRIX_TB_COLS; col++, col_bit <<= 1) {
			pin_state = gpio_read_pin(tb_col_pins[col]);
			if (pin_state == MATRIX_INPUT_PRESSED_STATE)
				row_state |= col_bit;
		}

		// row pin unset
		#ifdef MATRIX_UNSELECT_DRIVE_HIGH
		gpio_atomic_set_pin_output_high(pin_row);
		#else
		gpio_atomic_set_pin_input_high(pin_row);
		#endif
		matrix_output_unselect_delay(row, row_state != 0);

		tb_matrix[row] = row_state;
	}

	// set trackball row
	tb_row_state = (tb_matrix[2] << 4) | (tb_matrix[1] << 2) | tb_matrix[0];
	matrix[MATRIX_TB_ROW] = 0;

	// combo ======================================================================
	layer = get_highest_layer(layer_state | default_layer_state);

	// layer change -> waiting button on
	if (before_layer != layer) {
		before_layer = layer;
		for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++) {
			if (count[b1] > 1) {
				set[b1] = true;
				count[b1] = 0;
			}
		}
		// search combo button
		for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++) {
			combo_button[b1] = false;
			for (uint8_t b2 = b1 + 1; b2 < TB_BUTTONS; b2++) {
				combo_matrix[b1][b2] = false;
			}
		}

		for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++) {
			for (uint8_t b2 = b1 + 1; b2 < TB_BUTTONS; b2++) {
				keycode = KC_NO;
				row = combomap[b1][b2][0];
				col = combomap[b1][b2][1];
				for (uint8_t l = layer; l >= 0; l--) {
					uint16_t keycode2 = keycode_at_keymap_location(l, row, col);
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

	// set button flag
	for (uint8_t col = 0; col < TB_BUTTONS; col++, tb_row_state >>= 1) {
		if ((tb_row_state & 1) != 0) {
			button[col] = true;
			set[col] = true;
		}
	}

	#ifdef VIAL_ENABLE
	// disable combo when vial unlock in progress
	if (vial_unlock_in_progress == 0) {
	#endif
		// counter progress
		for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++) {
			if (combo_button[b1] == true) {
				if (button[b1] == true) {
					if (count[b1] != 1) {
						set[b1] = false;

						if (count[b1] == 0) {
							count[b1] = COMBO_TERM;
							push_count[b1] = 0;
						} else {
							count[b1]--;
						}
					} else {
						set[b1] = true;
					}
				} else {
					if (count[b1] > 1) {
						push_count[b1] = PUSH_TERM;
					}
					count[b1] = 0;
				}
			}
		}

		// combo check
		for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++) {
			for (uint8_t b2 = b1 + 1; b2 < TB_BUTTONS; b2++) {
				if (combo_matrix[b1][b2] == true) {
					row = combomap[b1][b2][0];
					col = combomap[b1][b2][1];
					// button1:on button2:on -> combo set
					if (button[b1] == true && button[b2] == true) {
						count[b1] = 1;
						count[b2] = 1;
						set[b1] = false;
						set[b2] = false;
						matrix[row] |= (MATRIX_ROW_SHIFTER << col);
					// when combo on
					} else if ((bak_matrix[row] & (MATRIX_ROW_SHIFTER << col)) != 0) {
						// button1:on or button2:on -> combo set
						if (button[b1] == true || button[b2] == true) {
							matrix[row] |= (MATRIX_ROW_SHIFTER << col);
							set[b1] = false;
							set[b2] = false;
						}
					}
				}
			}
		}

		// push counter progress
		for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++) {
			if (push_count[b1] > 0) {
				push_count[b1]--;
				set[b1] = true;
			}
		}
	#ifdef VIAL_ENABLE
	}
	#endif

	// set track ball button
	col_bit = MATRIX_ROW_SHIFTER;
	for (uint8_t b1 = 0; b1 < TB_BUTTONS; b1++, col_bit <<= 1) {
		if (set[b1] == true) {
			matrix[MATRIX_TB_ROW] |= col_bit;
		}
	}

	// set changed flag
	if (memcmp(bak_matrix, matrix, sizeof(bak_matrix)) != 0) {
		changed = true;
	}

	return changed;
}
