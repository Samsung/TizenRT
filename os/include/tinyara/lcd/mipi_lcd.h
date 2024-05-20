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

#ifndef __DRIVERS_LCD_MIPI_H
#define __DRIVERS_LCD_MIPI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define REGFLAG_DELAY                       0xFC
#define REGFLAG_END_OF_TABLE                0xFD        // END OF REGISTERS MARKER

#define LCDC_IMG_BUF_OFFSET             (10 << 20) /*offset is 10M In DDR */
#define LCDC_IMG_BUF_OFFSET1            (13 << 20) /*offset is 13M In DDR */
#define LCDC_IMG_BUF_OFFSET2            (16 << 20) /*offset is 16M In DDR */
#define LCDC_IMG_BUF_OFFSET3            (19 << 20) /*offset is 19M In DDR */
#define LCDC_IMG_BUF_ALIGNED64B(x)      (((x) & ~0x3F) + 0x40)/*Start Addr shall aligned to 64Byte*/

#if defined(CONFIG_LCD_ST7785)
#define INIT_CMD_SIZE 28
#elif defined(CONFIG_LCD_ST7701)
#define INIT_CMD_SIZE 16
#else
#define INIT_CMD_SIZE 28
#endif

typedef struct lcm_setting_table {
        u8 cmd;
        u16 count;
        u8 para_list[INIT_CMD_SIZE];
} lcm_setting_table_t;

struct mipi_lcd_config_s {

	void (*init)();
	void (*reset)();
	void (*lcd_mode_switch)(bool mode);			//false is command mode, true is video mode
	void (*lcd_enable)();
	void (*lcd_layer_enable)(int layer, bool enable);
	void (*backlight)(u8 level);
	void (*lcd_put_area)(u8 *lcd_img_buffer, u32 x1, u32 y1, u32 x2, u32 y2);
};

#endif	/* __DRIVERS_LCD_MIPI_H */
