/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef _ESP_ILI9341_H_
#define _ESP_ILI9341_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define LCD_VERTICAL_MAX                320
#define LCD_HORIZONTAL_MAX              240

#define ILI9341_DEVICE_CODE_READ_REG    0x00
#define ILI9341_SOFT_RESET_REG          0x01
#define ILI9341_IDENTINFO_R_REG         0x04
#define ILI9341_STATUS_R_REG            0x09
#define ILI9341_POWERMODE_R_REG         0x0A
#define ILI9341_MADCTL_R_REG            0x0B
#define ILI9341_PIXFORMAT_R_REG         0x0C
#define ILI9341_IMGFORMAT_R_REG         0x0D
#define ILI9341_SIGMODE_R_REG           0x0E
#define ILI9341_SD_RESULT_R_REG         0x0F
#define ILI9341_SLEEP_ENTER_REG         0x10
#define ILI9341_SLEEP_OUT_REG           0x11
#define ILI9341_PARTIALMODE_REG         0x12
#define ILI9341_NORDISPMODE_REG         0x13
#define ILI9341_INVERSIONOFF_REG        0x20
#define ILI9341_INVERSIONON_REG         0x21
#define ILI9341_GAMMASET_REG            0x26
#define ILI9341_DISPLAYOFF_REG          0x28
#define ILI9341_DISPLAYON_REG           0x29
#define ILI9341_COLADDRSET_REG          0x2A
#define ILI9341_PAGEADDRSET_REG         0x2B
#define ILI9341_MEMORYWRITE_REG         0x2C
#define ILI9341_COLORSET_REG            0x2D
#define ILI9341_MEMORYREAD_REG          0x2E
#define ILI9341_PARTIALAREA_REG         0x30
#define ILI9341_VERTSCROLL_REG          0x33
#define ILI9341_TEAREFFECTLINEOFF_REG   0x34
#define ILI9341_TEAREFFECTLINEON_REG    0x35
#define ILI9341_MEMACCESS_REG           0x36
#define ILI9341_VERSCRSRART_REG         0x37
#define ILI9341_IDLEMODEOFF_REG         0x38
#define ILI9341_IDLEMODEON_REG          0x39
#define ILI9341_PIXFORMATSET_REG        0x3A
#define ILI9341_WRITEMEMCONTINUE_REG    0x3C
#define ILI9341_READMEMCONTINUE_REG     0x3E
#define ILI9341_SETTEATSCAN_REG         0x44
#define ILI9341_GETSCANLINE_REG         0x45
#define ILI9341_WRITEBRIGHT_REG         0x51
#define ILI9341_READBRIGHT_REG          0x52
#define ILI9341_WRITECTRL_REG           0x53
#define ILI9341_READCTRL_REG            0x54
#define ILI9341_WRITECABC_REG           0x55
#define ILI9341_READCABC_REG            0x56
#define ILI9341_WRITECABCMB_REG         0x5E
#define ILI9341_READCABCMB_REG          0x5F
#define ILI9341_RGB_ISCTL_REG           0xB0
#define ILI9341_FRAMECTL_NOR_REG        0xB1
#define ILI9341_FRAMECTL_IDLE_REG       0xB2
#define ILI9341_FRAMECTL_PARTIAL_REG    0xB3
#define ILI9341_INVERCTL_REG            0xB4
#define ILI9341_BLANKPORCTL_REG         0xB5
#define ILI9341_FUNCTONCTL_REG          0xB6
#define ILI9341_ENTRYMODE_REG           0xB7
#define ILI9341_BLIGHTCTL1_REG          0xB8
#define ILI9341_BLIGHTCTL2_REG          0xB9
#define ILI9341_BLIGHTCTL3_REG          0xBA
#define ILI9341_BLIGHTCTL4_REG          0xBB
#define ILI9341_BLIGHTCTL5_REG          0xBC
#define ILI9341_BLIGHTCTL7_REG          0xBE
#define ILI9341_BLIGHTCTL8_REG          0xBF
#define ILI9341_POWERCTL1_REG           0xC0
#define ILI9341_POWERCTL2_REG           0xC1
#define ILI9341_VCOMCTL1_REG            0xC5
#define ILI9341_VCOMCTL2_REG            0xC7
#define ILI9341_POWERCTLA_REG           0xCB
#define ILI9341_POWERCTLB_REG           0xCF
#define ILI9341_NVMEMWRITE_REG          0xD0
#define ILI9341_NVMEMPROTECTKEY_REG     0xD1
#define ILI9341_NVMEMSTATUS_REG         0xD2
#define ILI9341_READID4_REG             0xD3
#define ILI9341_READID1_REG             0xDA
#define ILI9341_READID2_REG             0xDB
#define ILI9341_READID3_REG             0xDC
#define ILI9341_POSGAMMACORRECTION_REG  0xE0
#define ILI9341_NEGGAMMACORRECTION_REG  0xE1
#define ILI9341_DIGGAMCTL1_REG          0xE2
#define ILI9341_DIGGAMCTL2_REG          0xE3
#define ILI9341_DIVTIMCTL_A_REG         0xE8
#define ILI9341_DIVTIMCTL_B_REG         0xEA
#define ILI9341_POWONSEQCTL_REG         0xED
#define ILI9341_ENABLE_3G_REG           0xF2
#define ILI9341_INTERFCTL_REG           0xF6
#define ILI9341_PUMPRATIOCTL_REG        0xF7

/*****************************************************/
#define LCD_TFTWIDTH  240
#define LCD_TFTHEIGHT 320

#define LCD_INVOFF    0x20
#define LCD_INVON     0x21

#define LCD_CASET   0x2A
#define LCD_PASET   0x2B
#define LCD_RAMWR   0x2C
#define LCD_MADCTL  0x36

// Color definitions
#define COLOR_BLACK       0x0000      /*   0,   0,   0 */
#define COLOR_NAVY        0x000F      /*   0,   0, 128 */
#define COLOR_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define COLOR_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define COLOR_MAROON      0x7800      /* 128,   0,   0 */
#define COLOR_PURPLE      0x780F      /* 128,   0, 128 */
#define COLOR_OLIVE       0x7BE0      /* 128, 128,   0 */
#define COLOR_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define COLOR_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define COLOR_BLUE        0x001F      /*   0,   0, 255 */
#define COLOR_GREEN       0x07E0      /*   0, 255,   0 */
#define COLOR_CYAN        0x07FF      /*   0, 255, 255 */
#define COLOR_RED         0xF800      /* 255,   0,   0 */
#define COLOR_MAGENTA     0xF81F      /* 255,   0, 255 */
#define COLOR_YELLOW      0xFFE0      /* 255, 255,   0 */
#define COLOR_WHITE       0xFFFF      /* 255, 255, 255 */
#define COLOR_ORANGE      0xFD20      /* 255, 165,   0 */
#define COLOR_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define COLOR_PINK        0xF81F
#define COLOR_SILVER      0xC618
#define COLOR_GRAY        0x8410
#define COLOR_LIME        0x07E0
#define COLOR_TEAL        0x0410
#define COLOR_FUCHSIA     0xF81F
#define COLOR_ESP_BKGD    0xD185

/**
 * @brief  initialize the LCD device on ESP wrover kit
 *
 */
void up_lcdinitialize(void);
/**
 * @brief  set orientation of the LCD
 *
 * @param orientation value should be 0/90/180/270 directly
 *
 */
void esp_lcd_set_orientation(uint16_t orientation);
/**
 * @brief  flush UI buffer to the LCD display
 *
 * @param x1, y1, x2, y2 are the position of the render area
 * 		   color_p is the UI buffer of each pixels
 */
void esp_lcd_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t *color_p);
/**
 * @brief  fill same color of area
 *
 * @param x1, y1, x2, y2 are the position of the render area
 *		   color is the color value for the area
 */
void esp_lcd_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color);

#ifdef __cplusplus
}
#endif
#endif /* _ILI9341_H_ */

