// Copyright 2020-2025 Beken
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

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/media_types.h>
#include "modules/lcd_font.h"
#include "driver/lcd_qspi_types.h"

#define  USE_LCD_REGISTER_CALLBACKS  1

typedef void (*lcd_isr_t)(void);


#ifdef CONFIG_LCD_DMA2D_BLEND_FLASH_IMG
#define LOGO_MAX_W 40
#define LOGO_MAX_H 40
#define DMA2D_MALLOC_MAX  (LOGO_MAX_W * LOGO_MAX_H * 2)
#endif


typedef enum {
	LCD_DEVICE_UNKNOW,
	LCD_DEVICE_ST7282,  /**< 480X270 RGB  */
	LCD_DEVICE_HX8282,  /**< 1024X600 RGB  */
	LCD_DEVICE_GC9503V, /**< 480X800 RGB  */
	LCD_DEVICE_NT35510, /**< 480X854 RGB  */
	LCD_DEVICE_H050IWV, /**< 800X480 RGB  */
	LCD_DEVICE_MD0430R, /**< 800X480 RGB  */
	LCD_DEVICE_MD0700R, /**< 1024X600 RGB  */
	LCD_DEVICE_ST7701S_LY, /**< 480X480 RGB  */
	LCD_DEVICE_ST7701S, /**< 480X480 RGB  */
	LCD_DEVICE_ST7701SN, /**< 480X480 RGB  */
	LCD_DEVICE_AML01,   /**< 720X1280 RGB  */

	LCD_DEVICE_ST7796S, /**< 320X480 MCU  */
	LCD_DEVICE_NT35512, /**< 480X800 MCU  */
	LCD_DEVICE_NT35510_MCU, /**< 480X800 MCU  */
	LCD_DEVICE_ST7789V, /**< 170X320 MCU  */
	LCD_DEVICE_ST7796PI_MCU16, /**< 320X480 MCU 16bit  */

	LCD_DEVICE_SH8601A, /**< 454X454 QSPI  */
	LCD_DEVICE_ST77903_WX20114, /**< 400X400 QSPI  */
	LCD_DEVICE_ST77903_SAT61478M, /**< 400X400 QSPI  */
	LCD_DEVICE_ST77903_H0165Y008T, /**< 360X480 QSPI  */
	LCD_DEVICE_SPD2010, /**< 412X412 QSPI  */

	LCD_DEVICE_ST7796U, /**< 320X480 SPI */
} lcd_device_id_t;

typedef enum {
	LCD_TYPE_RGB,     /**< lcd hardware interface is parallel RGB interface */
	LCD_TYPE_RGB565,  /**< lcd device output data hardware interface is RGB565 format */
	LCD_TYPE_MCU8080, /**< lcd device output data hardware interface is MCU 8BIT format */
	LCD_TYPE_QSPI,    /**< lcd device hardware interface is QSPI interface */
	LCD_TYPE_SPI,     /**< lcd device hardware interface is SPI interface */
} lcd_type_t;

typedef enum {
	RGB_OUTPUT_EOF =1 << 5 ,	/**< reg end of frame int status*/
	RGB_OUTPUT_SOF =1 << 4, 	 /**< reg display output start of frame status*/
	I8080_OUTPUT_SOF =1 << 6,	/**< 8080 display output start of frame status */
	I8080_OUTPUT_EOF = 1 << 7,	 /**< 8080 display output end of frame status */
	DE_INT = 1 << 8,             /* de signal discontious interrupt status */
	FRAME_INTERVAL_INT = 1 <<10,
} lcd_int_type_t;

typedef enum {
	DCLK_UNIT = 0,
	HSYNC_UNIT,
	VSYNC_UNIT,
	NONE,
} frame_delay_unit_t;

/** rgb lcd clk select, infulence pfs, user should select according to lcd device spec*/
typedef enum {
	LCD_80M,
	LCD_64M,
	LCD_60M,
	LCD_54M,
	LCD_45M, //45.7M
	LCD_40M,
	LCD_35M, //35.5
	LCD_32M,
	LCD_30M,
	LCD_26M, //26.6M
	LCD_24M, //24.6M
	LCD_22M, //22.85M
	LCD_20M,
	LCD_17M, //17.1M
	LCD_15M,
	LCD_12M,
	LCD_10M,
	LCD_9M,  //9.2M
	LCD_8M,
	LCD_7M   //7.5M
} lcd_clk_t;

typedef enum {
    LCD_QSPI_80M = 80,
    LCD_QSPI_64M = 64,
    LCD_QSPI_60M = 60,
    LCD_QSPI_53M = 53, //53.3M
    LCD_QSPI_48M = 48,
    LCD_QSPI_40M = 40,
    LCD_QSPI_32M = 32,
    LCD_QSPI_30M = 30,
} lcd_qspi_clk_t;

/** rgb data output in clk rising or falling */
typedef enum {
	POSEDGE_OUTPUT = 0,    /**< output in clk falling*/
	NEGEDGE_OUTPUT,        /**< output in clk rising*/
} rgb_out_clk_edge_t;

typedef enum {
	ARGB8888 = 0, /**< ARGB8888 DMA2D color mode */
	RGB888,       /**< RGB888 DMA2D color mode   */
	RGB565,       /**< RGB565 DMA2D color mode   */
	YUYV,
} data_format_t;

typedef enum {
    LCD_BL_DEFAULT_CLOSE = 0,
    LCD_BL_DEFAULT_OPEN,
} lcd_backlight_default_ctrl_t;

typedef struct {
	unsigned char y;
	unsigned char u;
	unsigned char v;
} yuv_data_t;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
} lcd_rect_t;

typedef struct
{
    void *buffer;
    lcd_rect_t rect;
} lcd_disp_framebuf_t;


/** rgb interface config param */
typedef struct
{
	lcd_clk_t clk;                         /**< config lcd clk */
	rgb_out_clk_edge_t data_out_clk_edge;  /**< rgb data output in clk edge, should refer lcd device spec*/

	uint16_t hsync_back_porch;            /**< rang 0~0x3FF (0~1023), should refer lcd device spec*/
	uint16_t hsync_front_porch;           /**< rang 0~0x3FF (0~1023), should refer lcd device spec*/
	uint16_t vsync_back_porch;            /**< rang 0~0xFF (0~255), should refer lcd device spec*/
	uint16_t vsync_front_porch;           /**< rang 0~0xFF (0~255), should refer lcd device spec*/
	uint8_t hsync_pulse_width;            /**< rang 0~0x3F (0~7), should refer lcd device spec*/
	uint8_t vsync_pulse_width;            /**< rang 0~0x3F (0~7), should refer lcd device spec*/
} lcd_rgb_t;

/** mcu interface config param */
typedef struct
{
	lcd_clk_t clk; /**< config lcd clk */
	bk_err_t (*set_xy_swap)(bool swap_axes); 
	bk_err_t (*set_mirror)( bool mirror_x, bool mirror_y);
	void (*set_display_area)(uint16 xs, uint16 xe, uint16 ys, uint16 ye); 
	/**< if lcd size is smaller then image, and set api bk_lcd_pixel_config is image x y, should set partical display */

	void (*start_transform)(void); 
	void (*continue_transform)(void); 
} lcd_mcu_t;

/** qspi interface config param */
typedef struct
{
    lcd_qspi_clk_t clk;
	lcd_qspi_refresh_method_t refresh_method;
	uint8_t reg_write_cmd;
	uint8_t reg_read_cmd;
	lcd_qspi_write_config_t pixel_write_config;
	lcd_qspi_reg_read_config_t reg_read_config;
	const lcd_qspi_init_cmd_t *init_cmd;
	uint32_t device_init_cmd_len;

	lcd_qspi_refresh_config_by_line_t refresh_config;
	uint32_t frame_len;
} lcd_qspi_t;

/** spi interface config param */
typedef struct
{
	lcd_qspi_clk_t clk;
	const lcd_qspi_init_cmd_t *init_cmd;
	uint32_t device_init_cmd_len;
} lcd_spi_t;

typedef struct
{
	lcd_device_id_t id;  /**< lcd device type, user can add if you want to add another lcd device */
	char *name;          /**< lcd device name */
	lcd_type_t type;     /**< lcd device hw interface */
	media_ppi_t ppi;     /**< lcd device x y size */
	pixel_format_t src_fmt;  /**< source data format: input to display module data format(rgb565/rgb888/yuv)*/
	pixel_format_t out_fmt;   /**< display module output data format(rgb565/rgb666/rgb888), input to lcd device,*/
	union {
		const lcd_rgb_t *rgb;  /**< RGB interface lcd device config */
		const lcd_mcu_t *mcu;  /**< MCU interface lcd device config */
		const lcd_qspi_t *qspi;/**< QSPI interface lcd device config */
		const lcd_spi_t *spi;  /**< SPI interface lcd device config */
	};
	void (*init)(void);                   /**< lcd device initial function */
	bk_err_t (*lcd_off)(void);            /**< lcd off */
} lcd_device_t;


/*
 * @}
 */

#ifdef __cplusplus
}
#endif


