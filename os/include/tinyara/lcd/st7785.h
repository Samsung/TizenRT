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

#if defined(CONFIG_LCD_PORTRAIT) || defined(CONFIG_LCD_RPORTRAIT)
#define LCD_XRES CONFIG_LCD_XRES
#define LCD_YRES CONFIG_LCD_YRES
#define LCD_CASET 0x00EF
#define LCD_RASET 0x013F
#else
#define LCD_XRES CONFIG_LCD_YRES
#define LCD_YRES CONFIG_LCD_XRES
#define LCD_CASET 0x013F
#define LCD_RASET 0x00EF
#endif

#if defined(CONFIG_LCD_PORTRAIT)
#define LCD_ORIENTATION 0x00
#elif defined(CONFIG_LCD_RPORTRAIT)
#define LCD_ORIENTATION 0xc0
#elif defined(CONFIG_LCD_RLANDSCAPE)
#define LCD_ORIENTATION 0xa0
#else
#define LCD_ORIENTATION 0x70
#endif

#define LCDC_IMG_BUF_SIZE               LCDC_IMG_BUF_ALIGNED64B(LCD_XRES * LCD_YRES * 2)

static const lcm_setting_table_t lcd_init_cmd_g[] = { 
        {0x11, 0, {0x00}},
        {REGFLAG_DELAY, 120, {}},       /* Delayms (120) */
        {0x36, 2, {0x00, LCD_ORIENTATION}},
        {0x3A, 2, {0x00, 0x55}},// changed from 66 to 55 because 55 is for rgb565 and 66 is for rgb888
        {0xB0, 2, {0x00, 0x10}},//changed 0x10 to 0x12
        {0xB2, 10, {0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x33, 0x00, 0x33}},
        {0xB7, 2, {0x00, 0x51}},
        {0xBB, 2, {0x00, 0x22}},
        {0xC0, 2, {0x00, 0x2C}},
        {0xC2, 2, {0x00, 0x01}},
        {0xC3, 2, {0x00, 0x13}},
        {0xC6, 2, {0x00, 0x0F}},
        {0xD0, 2, {0x00, 0xA7}},
        {0xD0, 4, {0x00, 0xA4, 0x00, 0xA1}}, //removed 0x00 and 0xA1
        {0xE0, 28, {0x00, 0xF0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x03, 0x00, 0x05, 0x00, 0x04, 0x00, 0x2E, 0x00, 0x44, 0x00, 0x45, 0x00, 0x39, 0x00, 0x14, 0x00, 0x14, 0x00, 0x2D, 0x00, 0x35}},
        {0xE1, 28, {0x00, 0xF0, 0x00, 0x0A, 0x00, 0x0E, 0x00, 0x0F, 0x00, 0x0B, 0x00, 0x26, 0x00, 0x2E, 0x00, 0x43, 0x00, 0x45, 0x00, 0x36, 0x00, 0x12, 0x00, 0x12, 0x00, 0x2A, 0x00, 0x32}},
        {0x2A, 6, {0x00, 0x00, 0x00, 0x00, LCD_CASET && 0xFF00, LCD_CASET && 0x00FF}},//for landscape it is 0x13F and portrait , it is 0xEF
        {0x2B, 6, {0x00, 0x00, 0x00, 0x00, LCD_RASET && 0xFF00, LCD_RASET && 0x00FF}},//EF changed to 13F as given in datasheet
        {0x21, 0, {0x00}},
        {0x29, 0, {0x00}},
        {REGFLAG_END_OF_TABLE, 0, {}},
};

#endif	/* __DRIVERS_LCD_ST7785_H */
