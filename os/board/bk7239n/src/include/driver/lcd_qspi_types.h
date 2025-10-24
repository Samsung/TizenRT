// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <driver/hal/hal_qspi_types.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_QSPI_ID     QSPI_ID_0

typedef enum {
	LCD_QSPI_REFRESH_BY_LINE,
	LCD_QSPI_REFRESH_BY_FRAME,
	LCD_QSPI_REFRESH_INVALID,
} lcd_qspi_refresh_method_t;

typedef enum {
	LCD_QSPI_NO_INSERT_DUMMMY_CLK,
	LCD_QSPI_CMD1_DUMMY_CLK_CMD2,
	LCD_QSPI_CMD2_DUMMY_CLK_CMD3,
	LCD_QSPI_CMD3_DUMMY_CLK_CMD4,
	LCD_QSPI_CMD4_DUMMY_CLK_CMD5,
	LCD_QSPI_CMD5_DUMMY_CLK_CMD6,
	LCD_QSPI_CMD6_DUMMY_CLK_CMD7,
	LCD_QSPI_CMD7_DUMMY_CLK_CMD8,
} lcd_qspi_dummy_mode_t;

typedef enum {
    LCD_QSPI_ACT_DISP_IDLE,
    LCD_QSPI_ACT_DISP_FRAME,
    LCD_QSPI_ACT_DISP_CLOSE,
} lcd_qspi_act_option_t;

typedef struct {
	uint8_t cmd;
	uint8_t data[32];
	uint8_t data_len;
} lcd_qspi_init_cmd_t;

typedef struct {
	uint8_t *cmd;
	uint8_t cmd_len;        // cmd is 1 ~ 8 bytes
} lcd_qspi_write_config_t;

typedef struct {
	uint8_t dummy_clk;      // 1 ~ 127
	lcd_qspi_dummy_mode_t dummy_mode;
} lcd_qspi_reg_read_config_t;

typedef struct {
	uint8_t hsync_cmd;
	uint8_t vsync_cmd;
	uint8_t vsw;
	uint8_t hfp;
	uint8_t hbp;
    uint16_t line_len;
} lcd_qspi_refresh_config_by_line_t;


#ifdef __cplusplus
}
#endif


