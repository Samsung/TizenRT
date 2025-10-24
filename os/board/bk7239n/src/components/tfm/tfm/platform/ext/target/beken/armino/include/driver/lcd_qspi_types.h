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

typedef enum {
	LCD_QSPI_DEVICE_UNKNOW,
	LCD_QSPI_DEVICE_SH8601A,
	LCD_QSPI_DEVICE_ST77903,
} lcd_qspi_device_id_t;

typedef enum {
	LCD_QSPI_REFRESH_BY_LINE,
	LCD_QSPI_REFRESH_BY_FRAME,
	LCD_QSPI_REFRESH_INVALID,
} lcd_qspi_refresh_method_t;

typedef struct {
	uint8_t cmd;
	uint8_t data[16];
	uint8_t data_len;
} lcd_qspi_init_cmd_t;

typedef struct {
	uint8_t *cmd;
	uint8_t cmd_len;		// cmd is 1~8 bytes
} lcd_qspi_write_config_t;

typedef struct {
	uint8_t *cmd;
	uint8_t cmd_len;        // cmd is 1~8 bytes
	uint8_t *reg_rdata;
	uint8_t reg_rdata_len;  // rdata 0~64 bytes

	uint8_t data_line;
	uint8_t dummy_mode;
	uint8_t dummy_clk;
} lcd_qspi_reg_read_config_t;

typedef struct {
	uint8_t hsync_cmd;
	uint8_t vsync_cmd;
	uint8_t	vsw;
	uint16_t hfp;
	uint16_t hbp;
} lcd_qspi_refresh_config_by_line_t;

typedef struct {
	lcd_qspi_device_id_t id;
	char *name;
	media_ppi_t ppi;
	lcd_qspi_refresh_method_t refresh_method;
	uint8_t reg_write_cmd;
	lcd_qspi_write_config_t pixel_write_config;
	lcd_qspi_reg_read_config_t reg_read_config;
	const lcd_qspi_init_cmd_t *init_cmd;
	uint32_t device_init_cmd_len;
	bk_err_t (*backlight_init)(uint8_t);
	bk_err_t (*backlight_set)(uint8_t);
	bk_err_t (*backlight_deinit)(void);
	lcd_qspi_refresh_config_by_line_t refresh_config;
	uint32_t frame_len;
} lcd_qspi_device_t;

#ifdef __cplusplus
}
#endif


