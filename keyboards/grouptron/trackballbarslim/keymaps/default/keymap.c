/* Copyright 2025 Lalf/GroupTRON */
/* SPDX-License-Identifier: GPL-2.0-or-later */


#include QMK_KEYBOARD_H

typedef union {
	uint32_t raw;
	struct {
		uint8_t dpi;	// 1/100
		uint8_t angle;	// 0.90 1.180 2.270 3.0
	};
} user_config_t;

static user_config_t user_config;
static bool chgdpi = false;

enum custom_keycodes {
	KC_TB_CHGDPI = QK_KB_0,
	KC_TB_SHOWDPI,
	KC_TB_HALFDPI,
	KC_TB_DOUBLEDPI,
	KC_TB_SETDPI300,
	KC_TB_SETDPI500,
	KC_TB_SETDPI1000,
	KC_TB_SETDPI1500,
	KC_TB_SETDPI3000,
	KC_TB_CHGANGLE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/*  col-0(GP9)		row-0(GP14)
	 *  col-1(GP3)		row-1(GP13)
	 *  col-2(GP4)		row-2(GP5)
	 *					row-3(GP6)
	 *
	 *  re1(GP5)
	 *  re2(GP6)
	 *
	 *  real matrix
	 *   +----+        +----+ +----+
	 *   |0,0 |        |2,1 | |2,2 |
	 *   |B0  |        |B1  | |B2  |
	 *   +----+        +----+ +----+
	 *   +----+        +----+ +----+
	 *   |1,0 |        |3,1 | |3,2 |
	 *   |B3  |        |B4  | |B5  |
	 *   +----+        +----+ +----+
	 *
	 * keymap matrix
	 *           B0     B1     B2     B3     B4     B5
	 *         +----+ +----+ +----+ +----+ +----+ +----+
	 *      B0 |0,0 | |0,1 | |0,2 | |0,3 | |0,4 | |0,5 |
	 *         |    | |    | |    | |    | |    | |    |
	 *         +----+ +----+ +----+ +----+ +----+ +----+
	 *                +----+ +----+ +----+ +----+ +----+
	 *      B1        |1,1 | |1,2 | |1,3 | |1,4 | |1,5 |
	 *                |    | |    | |    | |    | |    |
	 *                +----+ +----+ +----+ +----+ +----+
	 *                       +----+ +----+ +----+ +----+
	 *      B2               |2,2 | |2,3 | |2,4 | |2,5 |
	 *                       |    | |    | |    | |    |
	 *                       +----+ +----+ +----+ +----+
	 *                              +----+ +----+ +----+
	 *      B3                      |3,3 | |3,4 | |3,5 |
	 *                              |    | |    | |    |
	 *                              +----+ +----+ +----+
	 *                                     +----+ +----+
	 *      B4                             |4,4 | |4,5 |
	 *                                     |    | |    |
	 *                                     +----+ +----+
	 *                                            +----+
	 *      B5                                    |5,5 |
	 *                                            |    |
	 *                                            +----+
     */

	[0] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	QK_MOUSE_BUTTON_1,		KC_NO,					KC_TB_CHGANGLE,		KC_NO,					KC_NO,				KC_NO			},
/*	B1 */	{	XXX,					QK_MOUSE_BUTTON_1,		KC_TB_CHGDPI,		KC_NO,					KC_NO,				KC_NO			},
/*	B2 */	{	XXX,					XXX,					KC_TB_HALFDPI,		KC_NO,					KC_NO,				KC_NO			},
/*	B3 */	{	XXX,					XXX,					XXX,				QK_MOUSE_BUTTON_2,		KC_NO,				KC_NO,			},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					QK_MOUSE_BUTTON_2,	KC_TB_SHOWDPI,	},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				MO(7)			}
	},

	[1] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	},

	[2] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	},

	[3] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	},

	[4] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	},

	[5] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	},

	[6] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	},

	[7] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TRNS,				KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B1 */	{	XXX,					KC_TRNS,				KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B2 */	{	XXX,					XXX,					KC_TRNS,			KC_TRNS,				KC_TRNS,			KC_TRNS			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TRNS,				KC_TRNS,			KC_TRNS,		},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TRNS,			KC_TRNS,		},
/*	B5 */	{	XXX,					XXX,					XXX,				XXX,					XXX,				KC_TRNS			}
	}
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
		[0] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[1] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[2] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[3] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[4] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[5] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[6] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_DOWN,QK_MOUSE_WHEEL_UP)
		},

		[7] = { 
			ENCODER_CCW_CW(QK_MOUSE_WHEEL_RIGHT,QK_MOUSE_WHEEL_LEFT)
		}
};
#endif

#ifdef RGBLIGHT_LAYERS
enum {
	LED_WHITE = 0,
	LED_GREEN,
	LED_BLUE,
	LED_CYAN,
	LED_YELLOW,
	LED_ORANGE,
	LED_PINK,
	LED_RED
};

const uint8_t led_color[8][3] = {
// HSV_WHITE    0,   0, 255 -> 25%   0,   0,  63
	{   0,   0,  63 },
// HSV_GREEN   85, 255, 255 -> 25%  85, 255,  63
	{  85, 255,  63 },
// HSV_BLUE   170, 255, 255 -> 25% 170, 255,  63
	{ 170, 255,  63 },
// HSV_CYAN   128, 255, 255 -> 25% 128, 255,  63
	{ 128, 255,  63 },
// HSV_YELLOW  43, 255, 255 -> 25%  43, 255,  63
	{  43, 255,  63 },
// HSV_ORANGE  21, 255, 255 -> 25%  21, 255,  63
	{  21, 255,  63 },
// HSV_PINK   234, 128, 255 -> 25% 234, 128,  63
	{ 234, 128,  63 },
// HSV_RED      0, 255, 255 -> 25%   0, 255,  63
	{   0, 255,  63 }
};

// HSV_WHITE
const rgblight_segment_t PROGMEM rgb_layer_1[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1, led_color[0][0], led_color[0][1], led_color[0][2] } );
// HSV_GREEN
const rgblight_segment_t PROGMEM rgb_layer_2[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1, led_color[1][0], led_color[1][1], led_color[1][2] } );
// HSV_BLUE
const rgblight_segment_t PROGMEM rgb_layer_3[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1, 170, 255,  63} );
// HSV_CYAN
const rgblight_segment_t PROGMEM rgb_layer_4[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1, 128, 255,  63} );
// HSV_YELLOW
const rgblight_segment_t PROGMEM rgb_layer_5[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1,  43, 255,  63} );
// HSV_ORANGE
const rgblight_segment_t PROGMEM rgb_layer_6[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1,  21, 255,  63} );
// HSV_PINK
const rgblight_segment_t PROGMEM rgb_layer_7[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1, 234, 128,  63} );
// HSV_RED
const rgblight_segment_t PROGMEM rgb_layer_8[] = RGBLIGHT_LAYER_SEGMENTS( {0, 1,   0, 255,  63} );

const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
	rgb_layer_1, rgb_layer_2, rgb_layer_3, rgb_layer_4,
	rgb_layer_5, rgb_layer_6, rgb_layer_7, rgb_layer_8
);

static void led_color_blink(int color, int ms, int count, bool lastblink)
{
	int layer = 0;

	for (int l = 0; l < 8; l++) {
		if (rgblight_get_layer_state(l) == true) {
			layer = l;
			rgblight_set_layer_state(l, false);
			break;
		}
	}

	for (int i = 0; i < count; i++) {
		rgblight_disable_noeeprom();
		wait_ms(ms);
		rgblight_enable_noeeprom();
		rgblight_sethsv_noeeprom(led_color[color][0],led_color[color][1],led_color[color][2]);
		wait_ms(ms);
	}

	if (lastblink == true) {
		rgblight_disable_noeeprom();
		wait_ms(ms);
		rgblight_enable_noeeprom();
	}
	rgblight_set_layer_state(layer, true);
}

static void tb_setangle(uint8_t angle, bool eeprom, bool led)
{
	int color[4] = { LED_WHITE, LED_GREEN, LED_BLUE, LED_RED };
	if (eeprom == true) {
		user_config.angle = angle;
		eeconfig_update_user(user_config.raw);
	}
	if (led == true) {
		
		led_color_blink(color[angle], 100, 1, true);
	}
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report)
{
	mouse_xy_report_t swap;
	switch (user_config.angle) {
	case 0:
		swap = mouse_report.x;
		mouse_report.x = mouse_report.y;
		mouse_report.y = swap;
		break;
	case 1:
		mouse_report.x = -mouse_report.x;
		break;
	case 2:
		swap = mouse_report.x;
		mouse_report.x = -mouse_report.y;
		mouse_report.y = -swap;
		break;
	case 3:
		mouse_report.y = -mouse_report.y;
		break;
	}
	return mouse_report;
}

void keyboard_post_init_user(void)
{
	rgblight_sethsv(127, 255, 100);	// CYAN
	rgblight_layers = rgb_layers;
	user_config.raw = eeconfig_read_user();
	if (user_config.dpi == 0) {
		eeconfig_init_user();
	}
	pmw33xx_set_cpi(0, user_config.dpi * 100);
}

layer_state_t layer_state_set_user(layer_state_t state)
{
	for (int i = 0; i < 8; i++) {
		rgblight_set_layer_state(i, layer_state_cmp(state, i));
	}
	return state;
}
#else
void keyboard_post_init_user(void)
{
	rgblight_sethsv(127, 255, 100);	// CYAN
	user_config.raw = eeconfig_read_user();
	if (user_config.dpi == 0) {
		eeconfig_init_user();
	}
	pmw33xx_set_cpi(0, user_config.dpi * 100);
}
#endif

static void tb_setdpi(int dpi, bool change_eeprom, bool led)
{
	pmw33xx_set_cpi(0, dpi * 100);
	if (change_eeprom == true) {
		user_config.dpi = dpi;
		eeconfig_update_user(user_config.raw);
	}

	if (led == true) {
		led_color_blink(LED_WHITE, 100, 1, true);
	}
}

static void led_error(void)
{
	led_color_blink(LED_RED, 100, 3, true);
}

bool encoder_hook_user(uint8_t index, bool clockwise)
{
	bool change = false;
	uint8_t dpi = user_config.dpi;

	if (index == 0) {
		if (chgdpi == true) {
			if (user_config.angle == 0 || user_config.angle == 1) {
				if (clockwise) {
					// down
					if (dpi < 30) {
						dpi++;
						change = true;
					}
				} else {
					// up
					if (dpi > 3) {
						dpi--;
						change = true;
					}
				}
			} else {
				if (clockwise) {
					// down
					if (dpi > 3) {
						dpi--;
						change = true;
					}
				} else {
					// up
					if (dpi < 30) {
						dpi++;
						change = true;
					}
				}
			}
			
			if (change == true) {
				tb_setdpi(dpi, true, true);
			}
			else {
				led_error();
			}
			return false;
		}
	}
	return true;
}

void eeconfig_init_user(void)
{
	debug_enable = true;
	debug_matrix = true;
	user_config.raw = 0;
	user_config.dpi = 10;
	user_config.angle = 0;
	eeconfig_update_user(user_config.raw);
}

static void tb_showdpi(void)
{
	int count = user_config.dpi;

	if (count / 10 > 0) {
		led_color_blink(LED_GREEN, 250, count / 10, false);
	}
	if (count % 10 > 0) {
		led_color_blink(LED_BLUE, 250, count % 10, true);
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	bool result = true;
	int dpi = (int)user_config.dpi;

	switch (keycode) {
	case KC_TB_CHGDPI:
		chgdpi = record->event.pressed;
		result = false;
		break;
	case KC_TB_SHOWDPI:
		if (record->event.pressed == true) {
			tb_showdpi();
		}
		result = false;
		break;
	case KC_TB_HALFDPI:
		if (record->event.pressed == true) {
			dpi /= 2;
			if (dpi < 3) {
				dpi = 3;
			}
		}
		tb_setdpi(dpi, false, false);
		result = false;
		break;
	case KC_TB_DOUBLEDPI:
		if (record->event.pressed == true) {
			dpi *= 2;
			if (dpi > 30) {
				dpi = 30;
			}
		}
		tb_setdpi(dpi, false, false);
		result = false;
		break;
	case KC_TB_SETDPI300:
		if (record->event.pressed == true) {
			tb_setdpi(3, true, true);
		}
		result = false;
		break;
	case KC_TB_SETDPI500:
		if (record->event.pressed == true) {
			tb_setdpi(5, true, true);
		}
		result = false;
		break;
	case KC_TB_SETDPI1000:
		if (record->event.pressed == true) {
			tb_setdpi(10, true, true);
		}
		result = false;
		break;
	case KC_TB_SETDPI1500:
		if (record->event.pressed == true) {
			tb_setdpi(15, true, true);
		}
		result = false;
		break;
	case KC_TB_SETDPI3000:
		if (record->event.pressed == true) {
			tb_setdpi(30, true, true);
		}
		result = false;
		break;
	case KC_TB_CHGANGLE:
		if (record->event.pressed == true) {
			tb_setangle((user_config.angle + 1) % 4, true, true);
		}
		result = false;
		break;
	}
	return result;
}
