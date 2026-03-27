/* Copyright 2025-2026 Lalf/GroupTRON */
/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "keymap_japanese.h"
#include "keymap_introspection.h"

typedef union {
	uint32_t raw;
	struct {
		uint8_t dpi;	// 1/100
		uint8_t dpi_abs;	// 1/100
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
	KC_TB_CHGANGLE,
	KC_TB_WHEEL,
	KC_TB_LOCKX,
	KC_TB_LOCKY,
	KC_TB_TOGABS,
	KC_TB_SWAPPOS,
	KC_TB_ABSMOUSE1,
	KC_TB_ABSMOUSE2,
	KC_XY_480,
	KC_XY_600,
	KC_XY_640,
	KC_XY_720,
	KC_XY_768,
	KC_XY_800,
	KC_XY_900,
	KC_XY_1024,
	KC_XY_1080,
	KC_XY_1200,
	KC_XY_1280,
	KC_XY_1440,
	KC_XY_1600,
	KC_XY_1920,
	KC_XY_2160,
	KC_XY_2560,
	KC_XY_2880,
	KC_XY_3840,
	KC_XY_4096,
	KC_XY_4320,
	KC_XY_5120,
	KC_XY_7680,
	KC_XY_8192,
	KC_POS_2,
	KC_POS_3,
	KC_POS_4,
	KC_POS_5,
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
	 *         +----+ +----+ +----+               +----+
	 *      B5 |5,0 | |5,1 | |5,2 |               |5,5 |
	 *         |SCRX| |SCRY| |POSN|               |    |
	 *         +----+ +----+ +----+               +----+
     */

	[0] = {
/*	button		B0						B1						B2 					B3						B4					B5 */
/*	B0 */	{	KC_TB_ABSMOUSE1,		KC_NO,					KC_TB_CHGANGLE,		KC_TB_SWAPPOS,			KC_NO,				KC_NO			},
/*	B1 */	{	XXX,					KC_TB_ABSMOUSE1,		KC_TB_CHGDPI,		KC_NO,					KC_TB_TOGABS,		KC_NO			},
/*	B2 */	{	XXX,					XXX,					KC_TB_HALFDPI,		KC_NO,					KC_NO,				KC_NO			},
/*	B3 */	{	XXX,					XXX,					XXX,				KC_TB_ABSMOUSE2,		KC_NO,				KC_NO,			},
/*	B4 */	{	XXX,					XXX,					XXX,				XXX,					KC_TB_ABSMOUSE2,	KC_TB_SHOWDPI,	},
/*	B5 */	{	KC_XY_1920,				KC_XY_1080,				KC_POS_2,			XXX,					XXX,				MO(7)			}
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

// absolute mode
#define POS_MAX 5
#define MOUSEKEY_REPEATDELAY	10
#define MOUSEKEY_ACCELCOUNT		100
#define MOUSEKEY_MAXDELTA		(SCR_MULTIPLIER * 10)
static bool abs_mode = false;
static bool abs_force_report = true;
static bool abs_tip = false;
static bool abs_tip_last = false;
static bool abs_barrel = false;
static bool abs_barrel_last = false;
static int scr_x, scr_y;
static int pos_x[POS_MAX], pos_y[POS_MAX];
static int8_t pos_mode = 0;
static bool mousekey_left, mousekey_right, mousekey_up, mousekey_down,
			mousekey_button[8];
static int mousekey_counter, mousekey_repeatcounter, mousekey_repeatdelay, mousekey_delta, mousekey_resetparam = true;

// wheel mode
#define WHEEL_COUNT		100
static bool wheel_mode = false;
static int16_t wheel_x, wheel_y;

// lock axis
static bool lock_x = false, lock_y = false;

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

#define SCR_MULTIPLIER	4

static void set_screensize(void)
{
	uint16_t keycode;
	int size;

	for (int8_t xy = 0; xy < 2; xy++) {
		if (xy == 0)
			keycode = keycode_at_keymap_location(0, 5, 0);
		else
			keycode = keycode_at_keymap_location(0, 5, 1);
		switch (keycode) {
		case KC_XY_480:
			size = 480 * SCR_MULTIPLIER;
			break;
		case KC_XY_600:
			size = 600 * SCR_MULTIPLIER;
			break;
		case KC_XY_640:
			size = 640 * SCR_MULTIPLIER;
			break;
		case KC_XY_720:
			size = 720 * SCR_MULTIPLIER;
			break;
		case KC_XY_768:
			size = 768 * SCR_MULTIPLIER;
			break;
		case KC_XY_800:
			size = 800 * SCR_MULTIPLIER;
			break;
		case KC_XY_900:
			size = 900 * SCR_MULTIPLIER;
			break;
		case KC_XY_1024:
			size = 1024 * SCR_MULTIPLIER;
			break;
		case KC_XY_1080:
			size = 1080 * SCR_MULTIPLIER;
			break;
		case KC_XY_1200:
			size = 1200 * SCR_MULTIPLIER;
			break;
		case KC_XY_1280:
			size = 1280 * SCR_MULTIPLIER;
			break;
		case KC_XY_1440:
			size = 1440 * SCR_MULTIPLIER;
			break;
		case KC_XY_1600:
			size = 1600 * SCR_MULTIPLIER;
			break;
		case KC_XY_1920:
			size = 1920 * SCR_MULTIPLIER;
			break;
		case KC_XY_2160:
			size = 2160 * SCR_MULTIPLIER;
			break;
		case KC_XY_2560:
			size = 2560 * SCR_MULTIPLIER;
			break;
		case KC_XY_2880:
			size = 2880 * SCR_MULTIPLIER;
			break;
		case KC_XY_3840:
			size = 3840 * SCR_MULTIPLIER;
			break;
		case KC_XY_4096:
			size = 4096 * SCR_MULTIPLIER;
			break;
		case KC_XY_4320:
			size = 4320 * SCR_MULTIPLIER;
			break;
		case KC_XY_5120:
			size = 5120 * SCR_MULTIPLIER;
			break;
		case KC_XY_7680:
			size = 7680 * SCR_MULTIPLIER;
			break;
		case KC_XY_8192:
			size = 8192 * SCR_MULTIPLIER;
			break;
		default:
			if (xy == 0)
				size = 1920 * SCR_MULTIPLIER;
			else
				size = 1080 * SCR_MULTIPLIER;
			break;
		}
		if (xy == 0)
			scr_x = size;
		else
			scr_y = size;
	}
}

static int8_t get_pos_no(void)
{
	uint16_t keycode = keycode_at_keymap_location(0, 5, 2);
	int8_t pos = 2;

	switch (keycode) {
	case KC_POS_2:
		pos = 2;
		break;
	case KC_POS_3:
		pos = 3;
		break;
	case KC_POS_4:
		pos = 4;
		break;
	case KC_POS_5:
		pos = 5;
		break;
	}
	return pos;
}

static void absolute_move_cursor(int x, int y)
{
	set_screensize();
	digitizer_state.in_range = true;
	digitizer_state.x = ((float) x / (float)(scr_x - 1));
	digitizer_state.y = ((float) y / (float)(scr_y - 1));
	digitizer_state.tip = abs_tip;
	digitizer_state.barrel = abs_barrel;
	digitizer_state.dirty = true;
	digitizer_flush();
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report)
{
	mouse_xy_report_t swap;
	bool report = false;

	// Angle
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

	// Lock XY Axis
	if (lock_x == true)
		mouse_report.x = 0;
	if (lock_y == true)
		mouse_report.y = 0;

	// Wheel
	if (wheel_mode == true) {
		wheel_x += mouse_report.x;
		wheel_y -= mouse_report.y;
		mouse_report.h += (wheel_x / WHEEL_COUNT);
		mouse_report.v += (wheel_y / WHEEL_COUNT);
		wheel_x %= WHEEL_COUNT;
		wheel_y %= WHEEL_COUNT;
		mouse_report.x = 0;
		mouse_report.y = 0;
	}

	// Absolute mode
	if (abs_mode == true) {
		if (mousekey_left == true || mousekey_right == true || mousekey_up == true || mousekey_down == true) {
			if (mousekey_counter == 0) {
				if (mousekey_left == true)
					pos_x[pos_mode] -= mousekey_delta;
				if (mousekey_right == true)
					pos_x[pos_mode] += mousekey_delta;
				if (mousekey_up == true)
					pos_y[pos_mode] -= mousekey_delta;
				if (mousekey_down == true)
					pos_y[pos_mode] += mousekey_delta;
				mousekey_counter = mousekey_repeatdelay;
				report = true;
			} else
				mousekey_counter--;
			if (mousekey_repeatcounter == 0) {
				mousekey_repeatcounter = MOUSEKEY_ACCELCOUNT;
				if (mousekey_repeatdelay >= 1)
					mousekey_repeatdelay--;
				else if (mousekey_delta < MOUSEKEY_MAXDELTA)
						mousekey_delta += SCR_MULTIPLIER;
			} else
				mousekey_repeatcounter--;
		} else
			mousekey_resetparam = true;

		if (mouse_report.x != 0 || mouse_report.y != 0)
			report = true;
		if (abs_tip != abs_tip_last) {
			abs_tip_last = abs_tip;
			report = true;
		}
		if (abs_barrel != abs_barrel_last) {
			abs_barrel_last = abs_barrel;
			report = true;
		}

		if (abs_force_report == true) {
			abs_force_report = false;
			report = true;
		}

		if (report == true) {
			pos_x[pos_mode] += mouse_report.x;
			if (pos_x[pos_mode] < 0)
				pos_x[pos_mode] = 0;
			else if (pos_x[pos_mode] >= scr_x)
				pos_x[pos_mode] = scr_x - 1;

			pos_y[pos_mode] += mouse_report.y;
			if (pos_y[pos_mode] < 0)
				pos_y[pos_mode] = 0;
			else if (pos_y[pos_mode] >= scr_y)
				pos_y[pos_mode] = scr_y - 1;

			absolute_move_cursor(pos_x[pos_mode], pos_y[pos_mode]);
			mouse_report.x = 0;
			mouse_report.y = 0;
		}
	}
	for (int8_t button = 0; button < 8; button++) {
		if (mousekey_button[button] == true)
			mouse_report.buttons |= (1 << button);
		else
			mouse_report.buttons &= ~(1 << button);
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
		layer_move(0);
	}
	pmw33xx_set_cpi(0, (int)user_config.dpi * 100);
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
	pmw33xx_set_cpi(0, (int)user_config.dpi * 100);
}
#endif

static void tb_setdpi(uint8_t dpi, bool change_eeprom, bool led)
{
	pmw33xx_set_cpi(0, (int)dpi * 100);
	if (change_eeprom == true) {
		if (abs_mode == false)
			user_config.dpi = dpi;
		else
			user_config.dpi_abs = dpi;
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
	uint8_t dpi;

	if (abs_mode == false)
		dpi = user_config.dpi;
	else
		dpi = user_config.dpi_abs;

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
	user_config.dpi_abs = 10;
	user_config.angle = 0;
	eeconfig_update_user(user_config.raw);
}

static void tb_showdpi(uint8_t count)
{
	if (count / 10 > 0) {
		led_color_blink(LED_GREEN, 250, count / 10, false);
	}
	if (count % 10 > 0) {
		led_color_blink(LED_BLUE, 250, count % 10, true);
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	const int color[POS_MAX] = { LED_WHITE, LED_GREEN, LED_BLUE, LED_CYAN, LED_YELLOW };
	bool result = true;
	uint8_t dpi;
	int8_t pos_max;

	if (abs_mode == false)
		dpi = user_config.dpi;
	else
		dpi = user_config.dpi_abs;

	switch (keycode) {
	case KC_TB_CHGDPI:
		chgdpi = record->event.pressed;
		result = false;
		break;
	case KC_TB_SHOWDPI:
		if (record->event.pressed == true) {
			tb_showdpi(dpi);
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
	case KC_TB_WHEEL:
		if (record->event.pressed == true) {
			if (wheel_mode == false) {
				wheel_mode = true;
				wheel_x = 0;
				wheel_y = 0;
			}
		} else {
			wheel_mode = false;
		}
		break;
	case KC_TB_LOCKX:
		if (record->event.pressed == true) {
			if (lock_x == false) {
				lock_x = true;
				led_color_blink(LED_RED, 100, 1, true);
			} else {
				lock_x = false;
				led_color_blink(LED_BLUE, 100, 1, true);
			}
		}
		break;
	case KC_TB_LOCKY:
		if (record->event.pressed == true) {
			if (lock_y == false) {
				lock_y = true;
				led_color_blink(LED_RED, 100, 1, true);
			} else {
				lock_y = false;
				led_color_blink(LED_BLUE, 100, 1, true);
			}
		}
		break;
	case KC_TB_TOGABS:
		if (record->event.pressed == true) {
			if (abs_mode == false) {
				abs_mode = true;
				digitizer_in_range_on();
				led_color_blink(LED_RED, 100, 1, true);
				dpi = user_config.dpi_abs;
			} else {
				abs_mode = false;
				digitizer_in_range_off();
				led_color_blink(LED_BLUE, 100, 1, true);
				dpi = user_config.dpi;
				abs_tip = false;
				abs_barrel = false;
			}
			tb_setdpi(dpi, false, false);
		}
		result = false;
		break;
	case KC_TB_SWAPPOS:
		if (record->event.pressed == true) {
			pos_max = get_pos_no();
			if (abs_mode == false) {
				abs_mode = true;
				digitizer_in_range_on();
				dpi = user_config.dpi_abs;
				tb_setdpi(dpi, false, false);
			} else {
				pos_mode++;
				if (pos_mode >= pos_max)
					pos_mode = 0;
			}
			abs_force_report = true;
			led_color_blink(color[pos_mode], 100, 1, true);
		}
		result = false;
		break;
	case QK_MOUSE_CURSOR_UP:
	case QK_MOUSE_CURSOR_DOWN:
	case QK_MOUSE_CURSOR_LEFT:
	case QK_MOUSE_CURSOR_RIGHT:
		if (abs_mode == true) {
			if (record->event.pressed == true) {
				if (mousekey_resetparam == true) {
					mousekey_repeatdelay = MOUSEKEY_REPEATDELAY;
					mousekey_repeatcounter = MOUSEKEY_ACCELCOUNT;
					mousekey_delta = SCR_MULTIPLIER;
					mousekey_counter = 0;
					mousekey_resetparam = false;
				}
				switch (keycode) {
				case QK_MOUSE_CURSOR_UP:
					mousekey_up = true;
					break;
				case QK_MOUSE_CURSOR_DOWN:
					mousekey_down = true;
					break;
				case QK_MOUSE_CURSOR_LEFT:
					mousekey_left = true;
					break;
				case QK_MOUSE_CURSOR_RIGHT:
					mousekey_right = true;
					break;
				}
			} else {
				switch (keycode) {
				case QK_MOUSE_CURSOR_UP:
					mousekey_up = false;
					break;
				case QK_MOUSE_CURSOR_DOWN:
					mousekey_down = false;
					break;
				case QK_MOUSE_CURSOR_LEFT:
					mousekey_left = false;
					break;
				case QK_MOUSE_CURSOR_RIGHT:
					mousekey_right = false;
					break;
				}
			}
			result = false;
		}
		break;
	case QK_MOUSE_BUTTON_1:
	case QK_MOUSE_BUTTON_2:
	case QK_MOUSE_BUTTON_3:
	case QK_MOUSE_BUTTON_4:
	case QK_MOUSE_BUTTON_5:
	case QK_MOUSE_BUTTON_6:
	case QK_MOUSE_BUTTON_7:
	case QK_MOUSE_BUTTON_8:
		if (record->event.pressed == true) {
			mousekey_button[keycode - QK_MOUSE_BUTTON_1] = true;
		} else
			mousekey_button[keycode - QK_MOUSE_BUTTON_1] = false;
		result = false;
		break;
	case KC_TB_ABSMOUSE1:
		if (record->event.pressed == true) {
			if (abs_mode == true) {
				abs_tip = true;
			} else {
				mousekey_button[0] = true;
			}
		} else {
			if (abs_mode == true) {
				abs_tip = false;
			} else {
				mousekey_button[0] = false;
			}
		}
		result = false;
		break;
	case KC_TB_ABSMOUSE2:
		if (record->event.pressed == true) {
			if (abs_mode == true) {
				abs_tip = true;
				abs_barrel = true;
			} else {
				mousekey_button[1] = true;
			}
		} else {
			if (abs_mode == true) {
				abs_tip = false;
				abs_barrel = false;
			} else {
				mousekey_button[1] = false;
			}
		}
		result = false;
		break;
	}
	return result;
}
