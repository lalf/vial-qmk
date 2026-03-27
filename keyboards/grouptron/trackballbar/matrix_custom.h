/* Copyright 2026 Lalf/GroupTRON */
/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#if (MATRIX_COLS <= 8)
typedef uint8_t matrix_row_t;
#elif (MATRIX_COLS <= 16)
typedef uint16_t matrix_row_t;
#elif (MATRIX_COLS <= 32)
typedef uint32_t matrix_row_t;
#else
#    error "MATRIX_COLS: invalid value"
#endif

#define MATRIX_ROW_SHIFTER ((matrix_row_t)1)

#ifdef __cplusplus
extern "C" {
#endif

/* matrix.c */
void matrix_output_select_delay(void);
void matrix_output_unselect_delay(uint8_t line, bool key_pressed);

#ifdef __cplusplus
}

#endif
