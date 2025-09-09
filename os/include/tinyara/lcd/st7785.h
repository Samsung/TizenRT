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

#ifndef __DRIVERS_LCD_ST7785_H
#define __DRIVERS_LCD_ST7785_H

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

#define MIPI_DSI_RTNI           4
#define MIPI_DSI_HSA            30
#define MIPI_DSI_HBP            30
#define MIPI_DSI_HFP            60
#define MIPI_DSI_VSA            4
#define MIPI_DSI_VBP            4
#define MIPI_DSI_VFP            8
#define MIPI_LCD_LIMIT		250
#define MIPI_FRAME_RATE         60
#define MIPI_LANE_NUMBER        1

#define LCD_CASET1 0x00		/* MV = 0, Column = 0x00EF = (240 - 1), Row = 0x013F = (320 - 1) */
#define LCD_CASET2 0xEF
#define LCD_RASET1 0x01
#define LCD_RASET2 0x3F
#define LCD_ID_LENGTH 3
#define LCD_HEIGHT_MM 57.6
#define LCD_WIDTH_MM 43.2

#if defined(CONFIG_LCD_SW_ROTATION) || defined(CONFIG_LCD_PORTRAIT)
#define LCD_ORIENTATION 0x00
#elif defined(CONFIG_LCD_RPORTRAIT)
#define LCD_ORIENTATION 0xD0
#else
#error Not support
#endif

#define LCDC_IMG_BUF_SIZE               LCDC_IMG_BUF_ALIGNED64B(LCD_XRES * LCD_YRES * 2)

#define ST7785_COMMON_INIT {0x3A, 2, {0x00, 0x55}},														/* Interface Pixel format 0x55 for RGB565 */ \
						   {0xB0, 2, {0x00, 0x11}},														/* RAM control */                            \
						   {0xB2, 10, {0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x33, 0x00, 0x33}},	/* Porch Setting */                          \
						   {0xB7, 2, {0x00, 0x51}},														/* Gate Control */                           \
						   {0xBB, 2, {0x00, 0x22}},														/* VCOMS Setting */                          \
						   {0xC0, 2, {0x00, 0x2C}},														/* LCM Control */                            \
						   {0xC2, 2, {0x00, 0x01}},														/* VRH Command Enable */                     \
						   {0xC3, 2, {0x00, 0x13}},														/* VRH Set */                                \
						   {0xC6, 2, {0x00, 0x0F}},														/* Frame Rate Control in Normal Mode */      \
						   {0xD0, 2, {0x00, 0xA7}},														/* Power Control 1 */                        \
						   {0xD0, 4, {0x00, 0xA4, 0x00, 0xA1}},											/* Power Control 1 */                        \
						   {0xE0, 28, {0x00, 0xF0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x03, 0x00, 0x05, 0x00, 0x04, 0x00, 0x2E, 0x00, 0x44, 0x00, 0x45, 0x00, 0x39, 0x00, 0x14, 0x00, 0x14, 0x00, 0x2D, 0x00, 0x35}}, /* Positive Voltage Gamma Control */         \
						   {0xE1, 28, {0x00, 0xF0, 0x00, 0x0A, 0x00, 0x0E, 0x00, 0x0F, 0x00, 0x0B, 0x00, 0x26, 0x00, 0x2E, 0x00, 0x43, 0x00, 0x45, 0x00, 0x36, 0x00, 0x12, 0x00, 0x12, 0x00, 0x2A, 0x00, 0x32}}, /* Negative Voltage Gamma Control */         \
						   {0x2A, 8, {0x00, 0x00, 0x00, 0x00, 0x00, LCD_CASET1, 0x00, LCD_CASET2}},		/* Column Address Set */                     \
						   {0x2B, 8, {0x00, 0x00, 0x00, 0x00, 0x00, LCD_RASET1, 0x00, LCD_RASET2}},		/* Row Address Set */                        \
						   {0x21, 0, {0x00}},																																									 /* Display Inversion On */

#ifdef CONFIG_LCD_INIT_AVD_2701
static const lcm_setting_table_t lcd_init_cmd_g_avd_2701[] = {
	{0x11, 0, {0x00}},					 /* Sleep out */
	{REGFLAG_DELAY, 120, {}},			 /* Delayms (120) */
	{0x36, 2, {0x00, LCD_ORIENTATION}},	 /* Memory Data Access Control */
	ST7785_COMMON_INIT{0x29, 0, {0x00}}, /* Display On */
	{REGFLAG_END_OF_TABLE, 0, {}},
};
#endif

#ifdef CONFIG_LCD_INIT_TEC_181A
static const lcm_setting_table_t lcd_init_cmd_g_tec_181a[] = {
	{0x36, 2, {0x00, LCD_ORIENTATION}},	 /* Memory Data Access Control */
	ST7785_COMMON_INIT{0x11, 0, {0x00}}, /* Sleep out */
	{REGFLAG_DELAY, 120, {}},			 /* Delayms (120) */
	{0x29, 0, {0x00}},					 /* Display On */
	{REGFLAG_DELAY, 10, {}},			 /* Delayms (10) */
	{REGFLAG_END_OF_TABLE, 0, {}},
};
#endif

#ifdef CONFIG_LCD_INIT_AVD_NB01
static const lcm_setting_table_t lcd_init_cmd_g_avd_nb01[] = {
	{0x11, 0, {0x00}},															/* Sleep out */
	{REGFLAG_DELAY, 120, {}},													/* Delayms (120) */
	{0x36, 2, {0x00, LCD_ORIENTATION}},											/* Memory Data Access Control */
	{0x3A, 2, {0x00, 0x55}},													/* Interface Pixel format 0x55 for RGB565 */
	{0xB0, 2, {0x00, 0x11}},													/* RAM control */
	{0xB2, 10, {0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x33, 0x00, 0x33}},	/* Porch Setting */
	{0xB7, 2, {0x00, 0x57}},													/* Gate Control */
	{0xBB, 2, {0x00, 0x1A}},													/* VCOMS Setting */
	{0xC0, 2, {0x00, 0x2C}},													/* LCM Control */
	{0xC2, 2, {0x00, 0x01}},													/* VRH Command Enable */
	{0xC3, 2, {0x00, 0x0F}},													/* VRH Set */
	{0xC6, 2, {0x00, 0x0F}},													/* Frame Rate Control in Normal Mode */
	{0xD0, 2, {0x00, 0xA7}},													/* Power Control 1 */
	{0xD0, 4, {0x00, 0xA4, 0x00, 0xA1}},										/* Power Control 1 */
	{0xE0, 28, {0x00, 0xF0, 0x00, 0x01, 0x00, 0x08, 0x00, 0x05, 0x00, 0x07, 0x00, 0x15, 0x00, 0x32, 0x00, 0x44, 0x00, 0x49, 0x00, 0x37, 0x00, 0x12, 0x00, 0x11, 0x00, 0x2E, 0x00, 0x33}}, /* Positive Voltage Gamma Control */
	{0xE1, 28, {0x00, 0xF0, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x0E, 0x00, 0x0C, 0x00, 0x06, 0x00, 0x32, 0x00, 0x33, 0x00, 0x49, 0x00, 0x35, 0x00, 0x10, 0x00, 0x12, 0x00, 0x2C, 0x00, 0x34}}, /* Negative Voltage Gamma Control */
	{0x2A, 8, {0x00, 0x00, 0x00, 0x00, 0x00, LCD_CASET1, 0x00, LCD_CASET2}},	/* Column Address Set */
	{0x2B, 8, {0x00, 0x00, 0x00, 0x00, 0x00, LCD_RASET1, 0x00, LCD_RASET2}},	/* Row Address Set */
	{0x21, 0, {0x00}},															/* Display Inversion On */
	{0x29, 0, {0x00}},															/* Display On */
	{REGFLAG_END_OF_TABLE, 0, {}},
};
#endif

#ifdef CONFIG_LCD_INIT_HLT_A196
static const lcm_setting_table_t lcd_init_cmd_g_hlt_a196[] = {
	{0xB2, 10, {0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x33, 0x00, 0x33}},	/* Porch Setting */
	{0x35, 2, {0x00, 0x00}},													/* Tearing Effect Line On 0x00 for The Tearing Effect output line consists of V-Blanking information only */
	{0x36, 2, {0x00, LCD_ORIENTATION}},											/* Memory Data Access Control */
	{0x3A, 2, {0x00, 0x55}},													/* Interface Pixel format 0x55 for RGB565 */
	{0xB0, 2, {0x00, 0x11}},													/* RAM control 0x11 for Ram access from RGB and MIPI interface, Display SYNC: RGB Interface DOTCLK SYNC mode */
	{0xB7, 2, {0x00, 0x52}},													/* Gate Control 0x52 for VGH 14.06V, VGL -8.23V */
	{0xBB, 2, {0x00, 0x17}},													/* VCOMS Setting 0x17 for VCOMS 0.675V */
	{0xC0, 2, {0x00, 0x2C}},													/* LCM Control 0x2C for XOR RGB setting in command 36h, XOR MX setting in command 36h, can reverse source output order and only support for RGB interface without RAM mode */
	{0xC3, 2, {0x00, 0x0F}},													/* VRH Set for 0x0F VRH 4.3V */
	{0xC6, 2, {0x00, 0x0F}},													/* Frame Rate Control in Normal Mode 0x0F for 60Hz FR in normal mode */
	{0xD0, 2, {0x00, 0xA7}},													/* Power Control 1 0xA7 for startup current of charge pump circuits high power mode */
	{0xD0, 4, {0x00, 0xA4, 0x00, 0xA1}},										/* Power Control 1 0xA4 0xA1 for startup current of charge pump circuits normar mode, AVD 6.6V, AVCL -4.8V, VDDS 2.3V */
	{0xD6, 2, {0x00, 0xA1}},													/* Gate Output Selection in Sleep In Mode 0xA1 for GND in sleep in mode */
	{0xE0, 28, {0x00, 0xF0, 0x00, 0x03, 0x00, 0x0A, 0x00, 0x06, 0x00, 0x07, 0x00, 0x14, 0x00, 0x32, 0x00, 0x43, 0x00, 0x49, 0x00, 0x37, 0x00, 0x12, 0x00, 0x11, 0x00, 0x2E, 0x00, 0x35}}, /* Positive Voltage Gamma Control */
	{0xE1, 28, {0x00, 0xF0, 0x00, 0x09, 0x00, 0x0D, 0x00, 0x0C, 0x00, 0x0A, 0x00, 0x06, 0x00, 0x32, 0x00, 0x33, 0x00, 0x49, 0x00, 0x34, 0x00, 0x10, 0x00, 0x12, 0x00, 0x2C, 0x00, 0x33}}, /* Negative Voltage Gamma Control */
	{0x21, 0, {0x00}},															/* Display Inversion On */
	{0x11, 0, {0x00}},															/* Sleep out */
	{REGFLAG_DELAY, 120, {}},													/* Delayms (120) */
	{0x29, 0, {0x00}},															/* Display On */
	{REGFLAG_DELAY, 10, {}},													/* Delayms (10) */
	{0x2C, 0, {0x00}},															/* Memory Write */
	{REGFLAG_END_OF_TABLE, 0, {}},
};
#endif

// Vendor mapping table
typedef struct {
	uint32_t id;
	lcm_setting_table_t *init_cmd;
} lcd_vendor_map_t;

static const lcd_vendor_map_t g_lcd_vendors[] = {
#ifdef CONFIG_LCD_INIT_AVD_NB01
	{0x8A8A85, lcd_init_cmd_g_avd_nb01},
#endif
#ifdef CONFIG_LCD_INIT_AVD_2701
	{0x8A8A01, lcd_init_cmd_g_avd_2701},
	{0x010000, lcd_init_cmd_g_avd_2701},
#endif
#ifdef CONFIG_LCD_INIT_HLT_A196
	{0x484C54, lcd_init_cmd_g_hlt_a196},
	{0x020000, lcd_init_cmd_g_hlt_a196},
#endif
#ifdef CONFIG_LCD_INIT_TEC_181A
	{0x030000, lcd_init_cmd_g_tec_181a},
#endif
};

#define NUM_LCD_VENDORS_ID (sizeof(g_lcd_vendors) / sizeof(g_lcd_vendors[0]))

#ifdef CONFIG_LCD_INIT_AVD_NB01
#define AVD_NB01 1
#else
#define AVD_NB01 0
#endif

#ifdef CONFIG_LCD_INIT_AVD_2701
#define AVD_2701 1
#else
#define AVD_2701 0
#endif

#ifdef CONFIG_LCD_INIT_HLT_A196
#define HLT_A196 1
#else
#define HLT_A196 0
#endif

#ifdef CONFIG_LCD_INIT_TEC_181A
#define TEC_181A 1
#else
#define TEC_181A 0
#endif

#define NUM_LCD_VENDERS (AVD_NB01 + AVD_2701 + HLT_A196 + TEC_181A)

#define LCD_BACKLIGHT_PWM_FREQ_KHZ 20

#endif	/* __DRIVERS_LCD_ST7785_H */
