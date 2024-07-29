/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __DRIVERS_LCD_ST7701SN_H
#define __DRIVERS_LCD_ST7701SN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
#include <tinyara/lcd/mipi_lcd.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* System Function Command Table 1 */
#define MIPI_DSI_RTNI           2
#define MIPI_DSI_HSA            4
#define MIPI_DSI_HBP            30
#define MIPI_DSI_HFP            30
#define MIPI_DSI_VSA            5
#define MIPI_DSI_VBP            20
#define MIPI_DSI_VFP            15
#define MIPI_LCD_LIMIT		500
#define MIPI_FRAME_RATE         60
#define MIPI_LANE_NUMBER        2
/* Support for screen rotation is not present in hardware. LCD will work in portrait mode only. */
#define LCD_XRES CONFIG_LCD_XRES
#define LCD_YRES CONFIG_LCD_YRES
#define LCDC_IMG_BUF_SIZE	LCDC_IMG_BUF_ALIGNED64B(LCD_XRES * LCD_YRES * 2)

static const lcm_setting_table_t lcd_init_cmd_g[] = {
	{0x11, 0, {0x00}},				/* Sleep out */
	{REGFLAG_DELAY, 120, {}},			/* Delayms (120) */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x13}},	/* Command2 BKx Selection */
	{0xEF, 1, {0x08}},
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x10}},	/* Command2 BKx Selection */
	{0xC0, 2, {0x63, 0x00}},			/* Display Line Setting */
	{0xC1, 2, {0x1C, 0x08}},			/* Porch Control */
	{0xC2, 2, {0x17, 0x02}},			/* Inversion selection & Frame Rate Control */
	{0xCC, 1, {0x10}},				/* Vop Amplitude setting */
	{0xB0, 16, {0x00, 0x0D, 0x14, 0x0D, 0x10, 0x06, 0x05, 0x08, 0x08, 0x20, 0x04, 0x11, 0x0F, 0x26, 0x2F, 0x1B}},	/* Positive Voltage Gamma Control */
	{0xB1, 16, {0x00, 0x0D, 0x14, 0x0D, 0x11, 0x06, 0x05, 0x08, 0x08, 0x20, 0x04, 0x11, 0x0F, 0x26, 0x2F, 0x1B}},	/* Negative Voltage Gamma Control */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x11}},	/* Command2 BKx Selection */
	{0xB0, 1, {0x5E}},	/* Positive Voltage Gamma*/
	{0xB1, 1, {0x2B}},	/* Negative Voltage Gamma*/
	{0xB2, 1, {0x81}},	/* VGH Voltage set*/
	{0xB3, 1, {0x80}},	/* TEST Command Set*/
	{0xB5, 1, {0x4E}},	/* VGL Voltage set*/
	{0xB7, 1, {0x87}},	/* Power Control 1 */
	{0xB8, 1, {0x32}},	/* Power Control 2 */
	{0xBB, 1, {0x03}},	/* Power pumping clk selection 2 */
	{0xC1, 1, {0x08}},	/* Source pre_drive timing set1 */
	{0xC2, 1, {0x08}},	/* Source EQ2 Set*/
	{0xD0, 1, {0x88}},	/* MIPI Setting 1 */
	{0xE0, 3, {0x00, 0x00, 0x02}},	/* Sunlight Readable Enhancement */
	{0xE1, 11, {0x06, 0x28, 0x08, 0x28, 0x05, 0x28, 0x07, 0x28, 0x0E, 0x33, 0x33}},		/* Noise Reduce*/
	{0xE2, 12, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},	/* Sharpness*/
	{0xE3, 4, {0x00, 0x00, 0x33, 0x33}},							/* Color Calibration*/
	{0xE4, 2, {0x44, 0x44}},								/* Skin Tone Preservation Control */
	{0xE5, 16, {0x11, 0x37, 0x2C, 0x8C, 0x13, 0x39, 0x2C, 0x8C, 0x0D, 0x33, 0x2C, 0x8C, 0x0F, 0x35, 0x2C, 0x8C}},
	{0xE6, 4, {0x00, 0x00, 0x33, 0x33}},
	{0xE7, 2, {0x44, 0x44}},
	{0xE8, 16, {0x10, 0x36, 0x2C, 0x8C, 0x12, 0x38, 0x2C, 0x8C, 0x0C, 0x32, 0x2C, 0x8C, 0x0E, 0x34, 0x2C, 0x8C}},
	{0xE9, 2, {0x36, 0x00}},
	{0xEB, 7, {0x02, 0x01, 0xE4, 0xE4, 0x44, 0x88, 0x40}},
	{0xED, 16, {0xBF, 0xCA, 0x76, 0x54, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x10, 0x45, 0x67, 0xAC, 0xFB}},
	{0xEF, 6, {0x08, 0x08, 0x08, 0x45, 0x3F, 0x54}},
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x13}},	/* Command2 BKx Selection */
	{0xE8, 2, {0x00, 0x0E}},
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x13}},	/* Command2 BKx Selection */
	{0xE8, 2, {0x00, 0x0C}},
	{REGFLAG_DELAY, 10, {}},			/* Delayms (10) */
	{0xE8, 2, {0x00, 0x00}},
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x00}},	/* Command2 BKx Selection */
	{0x36, 1, {0x00}},				/* Display data access control */
	{0x3A, 1, {0x55}},				/* Interface Pixel Format */
	{0x29, 0, {0x00}},				/* Display ON */
	{REGFLAG_DELAY, 20, {}},			/* Delayms (20) */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x10}},	/* Command2 BKx Selection */
	{0xE5, 1, {0x00}},
	{0xFF, 0, {0x00}},				/* Command2 BKx Selection */
	{REGFLAG_END_OF_TABLE, 0x00, {}},
};

#endif	/* __DRIVERS_LCD_ST7701SN_H */
