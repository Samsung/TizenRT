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

#ifndef __DRIVERS_LCD_ST7701_H
#define __DRIVERS_LCD_ST7701_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/lcd/mipi_lcd.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* System Function Command Table 1 */

#define LCD_XRES CONFIG_LCD_XRES
#define LCD_YRES CONFIG_LCD_YRES
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

#define LCDC_IMG_BUF_SIZE               LCDC_IMG_BUF_ALIGNED64B(LCD_XRES * LCD_YRES * 2)
/*#define INIT_CMD_SIZE 16

typedef struct lcm_setting_table {
        u8 cmd;
        u16 count;
        u8 para_list[INIT_CMD_SIZE];
} lcm_setting_table_t;
*/
static const lcm_setting_table_t lcd_init_cmd_g[] = {   /* DCS Write Long */
        /* ST7701S Reset Sequence */
        /* LCD_Nreset (1); Delayms (1); */
        /* LCD_Nreset (0); Delayms (1); */
        /* LCD_Nreset (1); Delayms (120); */
        {0x11, 0, {0x00}},
        {REGFLAG_DELAY, 120, {}},       /* Delayms (120) */

        /* Bank0 Setting */
        /* Display Control setting */
        {0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x10}},
        {0xC0, 2, {0x63, 0x00}},
        {0xC1, 2, {0x0C, 0x02}},
        {0xC2, 2, {0x31, 0x08}},
        {0xCC, 1, {0x10}},

        /* Gamma Cluster Setting */
        {0xB0, 16, {0x00, 0x11, 0x19, 0x0C, 0x10, 0x06, 0x07, 0x0A, 0x09, 0x22, 0x04, 0x10, 0x0E, 0x28, 0x30, 0x1C}},
        {0xB1, 16, {0x00, 0x12, 0x19, 0x0D, 0x10, 0x04, 0x06, 0x07, 0x08, 0x23, 0x04, 0x12, 0x11, 0x28, 0x30, 0x1C}},

        /* End Gamma Setting */
        /* End Display Control setting */
        /* End Bank0 Setting */

        /* Bank1 Setting */
        /* Power Control Registers Initial */
        {0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x11}},
        {0xB0, 1, {0x4D}},

        /* Vcom Setting */
        {0xB1, 1, {0x3E}},                      /* VCOM */
        /* End End Vcom Setting */

	{0xB2, 1, {0x09}},                      /* 07 */
	{0xB3, 1, {0x80}},
        {0xB5, 1, {0x4B}},                      /* 47 4A */
        {0xB7, 1, {0x85}},
        {0xB8, 1, {0x21}},
        {0xB9, 1, {0x10}},
        {0xC1, 1, {0x78}},
        {0xC2, 1, {0x78}},
        {0xD0, 1, {0x88}},
        /* End Power Control Registers Initial */
        {REGFLAG_DELAY, 100, {}},       /* Delayms (100) */
        /* GIP Setting */
        {0xE0, 3, {0x00, 0x00, 0x02}},
        {0xE1, 11, {0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20}},
        {0xE2, 13, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0xE3, 4, {0x00, 0x00, 0x33, 0x33}},
        {0xE4, 2, {0x22, 0x00}},
        {0xE5, 16, {0x04, 0x34, 0xAA, 0xAA, 0x06, 0x34, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0xE6, 4, {0x00, 0x00, 0x33, 0x00}},
        {0xE7, 2, {0x22, 0x00}},
        {0xE8, 16, {0x05, 0x34, 0xAA, 0xAA, 0x07, 0x34, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        //{0xEA, 16, {0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00}},
        {0xEB, 7, {0x02, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00}},
        {0xEC, 2, {0x00, 0x00}},
        {0xED, 16, {0xFA, 0x45, 0x0B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB0, 0x54, 0xAF}},
        // {0xEF, 12, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
        {0xEF, 6, {0x08, 0x08, 0x08, 0x44, 0x3F, 0x54}},
        /* End GIP Setting */

        {0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x00}},
        {0x29, 0, {0x00}},
        {REGFLAG_END_OF_TABLE, 0x00, {}},
};

#endif	/* __DRIVERS_LCD_ST7701_H */
