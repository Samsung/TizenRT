// Copyright 2022-2025 Beken
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

typedef struct {
	uint8_t dev;
	int8_t cfg_bit;
	int8_t reg;
	int8_t bit;
} prro_map_t;

#define PRRO_INVALID_REG  (-1)

#define PRRO_AP_MAP(ap_map) \
	const prro_map_t *ap_map = s_ap_map;

#define PRRO_AP_MAP_COUNT \
	(sizeof(s_ap_map) / sizeof(s_ap_map[0]))

#define PRRO_SEC_MAP(sec_map)\
	const prro_map_t *sec_map = s_sec_map;

#define PRRO_SEC_MAP_COUNT \
	(sizeof(s_sec_map) / sizeof(s_sec_map[0]))

#if CONFIG_TFM_BK7236_V5
#define PRRO_AP_MAP_ENTITY \
{\
	{ PRRO_DEV_AON,        8, 0x6, 8 },\
	{ PRRO_DEV_APB_WDT,        20, 0x6, 20 },\
	{ PRRO_DEV_AUD,        9, 0xa, 9 },\
	{ PRRO_DEV_BTDM,        2, 0xd, 2 },\
	{ PRRO_DEV_CAN,        8, 0x9, 8 },\
	{ PRRO_DEV_CKMN,        26, 0x6, 26 },\
	{ PRRO_DEV_DISP,        12, 0xb, 12 },\
	{ PRRO_DEV_DMA0,        4, 0x7, 4 },\
	{ PRRO_DEV_DMA1,        5, 0x7, 5 },\
	{ PRRO_DEV_DMAD,        13, 0xb, 13 },\
	{ PRRO_DEV_EFS,        24, 0x6, 24 },\
	{ PRRO_DEV_ENET,        12, 0x9, 12 },\
	{ PRRO_DEV_FLASH,        10, 0x6, 10 },\
	{ PRRO_DEV_H264,        20, 0xb, 20 },\
	{ PRRO_DEV_I2C0,        18, 0x8, 18 },\
	{ PRRO_DEV_I2C1,        19, 0x8, 19 },\
	{ PRRO_DEV_I2S0,        10, 0xa, 10 },\
	{ PRRO_DEV_I2S1,        11, 0xa, 11 },\
	{ PRRO_DEV_I2S2,        12, 0xa, 12 },\
	{ PRRO_DEV_IRDA,        23, 0x8, 23 },\
	{ PRRO_DEV_JPGD,        11, 0xb, 11 },\
	{ PRRO_DEV_JPGE,        9, 0xb, 9 },\
	{ PRRO_DEV_LA,        6, 0x7, 6 },\
	{ PRRO_DEV_LIN,        18, 0x9, 18 },\
	{ PRRO_DEV_MAC,        2, 0xc, 2 },\
	{ PRRO_DEV_MBOX_AHB,        1, 0xe, 1 },\
	{ PRRO_DEV_MCHECK,        25, 0x6, 25 },\
	{ PRRO_DEV_MOD,        3, 0xc, 3 },\
	{ PRRO_DEV_PPRO_MPC,        2, 0xe, 2 },\
	{ PRRO_DEV_PSRAM,        11, 0x9, 11 },\
	{ PRRO_DEV_PWM0,        22, 0x8, 22 },\
	{ PRRO_DEV_PWM1,        27, 0x8, 27 },\
	{ PRRO_DEV_QSPI0,        9, 0x9, 9 },\
	{ PRRO_DEV_QSPI1,        10, 0x9, 10 },\
	{ PRRO_DEV_ROTT,        14, 0xb, 14 },\
	{ PRRO_DEV_SADC,        21, 0x8, 21 },\
	{ PRRO_DEV_SBC,        2, 0xa, 2 },\
	{ PRRO_DEV_SCAL0,        15, 0xb, 15 },\
	{ PRRO_DEV_SCAL1,        16, 0xb, 16 },\
	{ PRRO_DEV_SCLD,        26, 0x8, 26 },\
	{ PRRO_DEV_SCR,        17, 0x9, 17 },\
	{ PRRO_DEV_SDIO,        25, 0x8, 25 },\
	{ PRRO_DEV_SDMADC,        15, 0x8, 15 },\
	{ PRRO_DEV_SPI0,        23, 0x6, 23 },\
	{ PRRO_DEV_SPI1,        20, 0x8, 20 },\
	{ PRRO_DEV_SYS,        9, 0x6, 9 },\
	{ PRRO_DEV_TIMER0,        21, 0x6, 21 },\
	{ PRRO_DEV_TIMER1,        14, 0x8, 14 },\
	{ PRRO_DEV_TRNG,        24, 0x8, 24 },\
	{ PRRO_DEV_UART0,        22, 0x6, 22 },\
	{ PRRO_DEV_UART1,        16, 0x8, 16 },\
	{ PRRO_DEV_UART2,        17, 0x8, 17 },\
	{ PRRO_DEV_USB,        7, 0x9, 7 },\
	{ PRRO_DEV_XVR,        6, 0xd, 6 },\
	{ PRRO_DEV_YUV,        10, 0xb, 10 },\
}

#define PRRO_SEC_MAP_ENTITY \
{\
	{ PRRO_DEV_AON_WDT,        3, 0x6, 3 },\
	{ PRRO_DEV_APB_WDT,        13, 0x6, 13 },\
	{ PRRO_DEV_AUD,        5, 0xa, 5 },\
	{ PRRO_DEV_BTDM,        0, 0xd, 0 },\
	{ PRRO_DEV_BTDM_M,        5, 0xf, 5 },\
	{ PRRO_DEV_CAN,        1, 0x9, 1 },\
	{ PRRO_DEV_CKMN,        19, 0x6, 19 },\
	{ PRRO_DEV_DISP,        3, 0xb, 3 },\
	{ PRRO_DEV_DISP_M,        7, 0xf, 7 },\
	{ PRRO_DEV_DMA0,        0, 0x7, 0 },\
	{ PRRO_DEV_DMA1,        1, 0x7, 1 },\
	{ PRRO_DEV_DMAD,        4, 0xb, 4 },\
	{ PRRO_DEV_DMAD_M,        2, 0xf, 2 },\
	{ PRRO_DEV_EFS,        17, 0x6, 17 },\
	{ PRRO_DEV_ENET,        5, 0x9, 5 },\
	{ PRRO_DEV_ENET_M,        10, 0xf, 10 },\
	{ PRRO_DEV_FLASH,        6, 0x6, 6 },\
	{ PRRO_DEV_GPIOHIG,        2, 0x6, 2 },\
	{ PRRO_DEV_H264,        19, 0xb, 19 },\
	{ PRRO_DEV_HSU_M,        4, 0xf, 4 },\
	{ PRRO_DEV_I2C0,        4, 0x8, 4 },\
	{ PRRO_DEV_I2C1,        5, 0x8, 5 },\
	{ PRRO_DEV_I2S0,        6, 0xa, 6 },\
	{ PRRO_DEV_I2S1,        7, 0xa, 7 },\
	{ PRRO_DEV_I2S2,        8, 0xa, 8 },\
	{ PRRO_DEV_IRDA,        9, 0x8, 9 },\
	{ PRRO_DEV_JPDEC_M,        1, 0xf, 1 },\
	{ PRRO_DEV_JPENC_M,        0, 0xf, 0 },\
	{ PRRO_DEV_JPGD,        2, 0xb, 2 },\
	{ PRRO_DEV_JPGE,        0, 0xb, 0 },\
	{ PRRO_DEV_LA,        2, 0x7, 2 },\
	{ PRRO_DEV_LA_M,        6, 0xf, 6 },\
	{ PRRO_DEV_LIN,        16, 0x9, 16 },\
	{ PRRO_DEV_MAC,        0, 0xc, 0 },\
	{ PRRO_DEV_MAC_M,        3, 0xf, 3 },\
	{ PRRO_DEV_MBOX_AHB,        0, 0xe, 0 },\
	{ PRRO_DEV_MCHECK,        18, 0x6, 18 },\
	{ PRRO_DEV_MOD,        1, 0xc, 1 },\
	{ PRRO_DEV_PSRAM,        4, 0x9, 4 },\
	{ PRRO_DEV_PWM0,        8, 0x8, 8 },\
	{ PRRO_DEV_PWM1,        13, 0x8, 13 },\
	{ PRRO_DEV_QSPI0,        2, 0x9, 2 },\
	{ PRRO_DEV_QSPI1,        3, 0x9, 3 },\
	{ PRRO_DEV_REG,        0, 0x6, 0 },\
	{ PRRO_DEV_ROTT,        5, 0xb, 5 },\
	{ PRRO_DEV_ROTT_M,        8, 0xf, 8 },\
	{ PRRO_DEV_RTC,        1, 0x6, 1 },\
	{ PRRO_DEV_SADC,        7, 0x8, 7 },\
	{ PRRO_DEV_SBC,        0, 0xa, 0 },\
	{ PRRO_DEV_SCAL0,        6, 0xb, 6 },\
	{ PRRO_DEV_SCAL0_M,        11, 0xf, 11 },\
	{ PRRO_DEV_SCAL1,        7, 0xb, 7 },\
	{ PRRO_DEV_SCAL1_M,        12, 0xf, 12 },\
	{ PRRO_DEV_SCLD,        12, 0x8, 12 },\
	{ PRRO_DEV_SCR,        15, 0x9, 15 },\
	{ PRRO_DEV_SDIO,        11, 0x8, 11 },\
	{ PRRO_DEV_SDMADC,        1, 0x8, 1 },\
	{ PRRO_DEV_SPI0,        16, 0x6, 16 },\
	{ PRRO_DEV_SPI1,        6, 0x8, 6 },\
	{ PRRO_DEV_SYS,        5, 0x6, 5 },\
	{ PRRO_DEV_TIMER0,        14, 0x6, 14 },\
	{ PRRO_DEV_TIMER1,        0, 0x8, 0 },\
	{ PRRO_DEV_TRNG,        10, 0x8, 10 },\
	{ PRRO_DEV_UART0,        15, 0x6, 15 },\
	{ PRRO_DEV_UART1,        2, 0x8, 2 },\
	{ PRRO_DEV_UART2,        3, 0x8, 3 },\
	{ PRRO_DEV_USB,        0, 0x9, 0 },\
	{ PRRO_DEV_USB_M,        9, 0xf, 9 },\
	{ PRRO_DEV_XVR,        5, 0xd, 5 },\
	{ PRRO_DEV_YUV,        1, 0xb, 1 },\
	{PRRO_DEV_GPIO0,           0, 0x4, 0},\
	{PRRO_DEV_GPIO1,           1, 0x4, 1},\
	{PRRO_DEV_GPIO2,           2, 0x4, 2},\
	{PRRO_DEV_GPIO3,           3, 0x4, 3},\
	{PRRO_DEV_GPIO4,           4, 0x4, 4},\
	{PRRO_DEV_GPIO5,           5, 0x4, 5},\
	{PRRO_DEV_GPIO6,           6, 0x4, 6},\
	{PRRO_DEV_GPIO7,           7, 0x4, 7},\
	{PRRO_DEV_GPIO8,           8, 0x4, 8},\
	{PRRO_DEV_GPIO9,           9, 0x4, 9},\
	{PRRO_DEV_GPIO10,         10, 0x4, 10},\
	{PRRO_DEV_GPIO11,         11, 0x4, 11},\
	{PRRO_DEV_GPIO12,         12, 0x4, 12},\
	{PRRO_DEV_GPIO13,         13, 0x4, 13},\
	{PRRO_DEV_GPIO14,         14, 0x4, 14},\
	{PRRO_DEV_GPIO15,         15, 0x4, 15},\
	{PRRO_DEV_GPIO16,         16, 0x4, 16},\
	{PRRO_DEV_GPIO17,         17, 0x4, 17},\
	{PRRO_DEV_GPIO18,         18, 0x4, 18},\
	{PRRO_DEV_GPIO19,         19, 0x4, 19},\
	{PRRO_DEV_GPIO20,         20, 0x4, 20},\
	{PRRO_DEV_GPIO21,         21, 0x4, 21},\
	{PRRO_DEV_GPIO22,         22, 0x4, 22},\
	{PRRO_DEV_GPIO23,         23, 0x4, 23},\
	{PRRO_DEV_GPIO24,         24, 0x4, 24},\
	{PRRO_DEV_GPIO25,         25, 0x4, 25},\
	{PRRO_DEV_GPIO26,         26, 0x4, 26},\
	{PRRO_DEV_GPIO27,         27, 0x4, 27},\
	{PRRO_DEV_GPIO28,         28, 0x4, 28},\
	{PRRO_DEV_GPIO29,         29, 0x4, 29},\
	{PRRO_DEV_GPIO30,         30, 0x4, 30},\
	{PRRO_DEV_GPIO31,         31, 0x4, 31},\
\
	{PRRO_DEV_GPIO32,          0, 0x5, 0},\
	{PRRO_DEV_GPIO33,          1, 0x5, 1},\
	{PRRO_DEV_GPIO34,          2, 0x5, 2},\
	{PRRO_DEV_GPIO35,          3, 0x5, 3},\
	{PRRO_DEV_GPIO36,          4, 0x5, 4},\
	{PRRO_DEV_GPIO37,          5, 0x5, 5},\
	{PRRO_DEV_GPIO38,          6, 0x5, 6},\
	{PRRO_DEV_GPIO39,          7, 0x5, 7},\
	{PRRO_DEV_GPIO40,          8, 0x5, 8},\
	{PRRO_DEV_GPIO41,          9, 0x5, 9},\
	{PRRO_DEV_GPIO42,         10, 0x5, 10},\
	{PRRO_DEV_GPIO43,         11, 0x5, 11},\
	{PRRO_DEV_GPIO44,         12, 0x5, 12},\
	{PRRO_DEV_GPIO45,         13, 0x5, 13},\
	{PRRO_DEV_GPIO46,         14, 0x5, 14},\
	{PRRO_DEV_GPIO47,         15, 0x5, 15},\
	{PRRO_DEV_GPIO48,         16, 0x5, 16},\
	{PRRO_DEV_GPIO49,         17, 0x5, 17},\
	{PRRO_DEV_GPIO50,         18, 0x5, 18},\
	{PRRO_DEV_GPIO51,         19, 0x5, 19},\
	{PRRO_DEV_GPIO52,         20, 0x5, 20},\
	{PRRO_DEV_GPIO53,         21, 0x5, 21},\
	{PRRO_DEV_GPIO54,         22, 0x5, 22},\
	{PRRO_DEV_GPIO55,         23, 0x5, 23}\
}
#else
#define PRRO_AP_MAP_ENTITY \
{\
	{PRRO_DEV_AHB_AON,        0, 4,  0 },\
	{PRRO_DEV_AHB_SYSTEM,     1, 4,  1 },\
	{PRRO_DEV_AHB_FLASH,      2, 4,  2 },\
	{PRRO_DEV_AHB_GDMA,       3, 4,  3 },\
	{PRRO_DEV_AHB_LA,         4, 4,  4 },\
	{PRRO_DEV_AHB_USB,        5, 4,  5 },\
	{PRRO_DEV_AHB_CAN,        6, 4,  6 },\
	{PRRO_DEV_AHB_QSPI0,      7, 4,  7 },\
	{PRRO_DEV_AHB_QSPI1,      8, 4,  8 },\
	{PRRO_DEV_AHB_PSRAM,      9, 4,  9 },\
	{PRRO_DEV_AHB_FFT,        10, 4, 10},\
	{PRRO_DEV_AHB_SBC,        11, 4, 11},\
	{PRRO_DEV_AHB_JPEG_ENC,   12, 4, 12},\
	{PRRO_DEV_AHB_JPEG_DEC,   13, 4, 14},\
	{PRRO_DEV_AHB_YUV,        14, 4, 13},\
	{PRRO_DEV_AHB_LCD_DISP,   15, 4, 15},\
	{PRRO_DEV_AHB_DMA2D,      16, 4, 16},\
	{PRRO_DEV_AHB_ROTT,       17, 4, 17},\
	{PRRO_DEV_AHB_WIFI_MAC,   18, 4, 18},\
	{PRRO_DEV_AHB_WIFI_MODEM, 19, 4, 19},\
	{PRRO_DEV_AHB_BTDM_AHB,   20, 4, 20},\
	{PRRO_DEV_AHB_MBOX0,      21, 4, 21},\
	{PRRO_DEV_AHB_MBOX1,      22, 4, 22},\
	{PRRO_DEV_AHB_DMA1,       23, 4, 23},\
	{PRRO_DEV_AHB_MAX,        -1, PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_APB_WDT,        32, 7, 0 },\
	{PRRO_DEV_APB_EFUSE,      33, 7, 4 },\
	{PRRO_DEV_APB_MBIC,       34, 7, 5 },\
	{PRRO_DEV_APB_SDMADC,     35, 7, 7 },\
	{PRRO_DEV_APB_TIMER0,     36, 7, 1 },\
	{PRRO_DEV_APB_TIMER1,     37, 7, 6 },\
	{PRRO_DEV_APB_PWM0,       38, 7, 14 },\
	{PRRO_DEV_APB_PWM1,       39, 7, 19 },\
	{PRRO_DEV_APB_UART0,      40, 7, 2 },\
	{PRRO_DEV_APB_UART1,      41, 7, 8 },\
	{PRRO_DEV_APB_UART2,      42, 7, 9 },\
	{PRRO_DEV_APB_SPI0,       43, 7, 3},\
	{PRRO_DEV_APB_SPI1,       44, 7, 12},\
	{PRRO_DEV_APB_I2C0,       45, 7, 10},\
	{PRRO_DEV_APB_I2C1,       46, 7, 11},\
	{PRRO_DEV_APB_SARADC,     47, 7, 13},\
	{PRRO_DEV_APB_IRDA,       48, 7, 15},\
	{PRRO_DEV_APB_TRNG,       49, 7, 16},\
	{PRRO_DEV_APB_SDIO,       50, 7, 17},\
	{PRRO_DEV_APB_SLCD,       51, 7, 18},\
	{PRRO_DEV_APB_AUDIO,      52, 7, 20},\
	{PRRO_DEV_APB_I2S0,       53, 7, 21},\
	{PRRO_DEV_APB_I2S1,       54, 7, 22},\
	{PRRO_DEV_APB_I2S2,       55, 7, 23},\
	{PRRO_DEV_APB_VIDP,       56, 7, 24},\
	{PRRO_DEV_APB_BTDM_APB,   57, 7, 25},\
	{PRRO_DEV_APB_MAX,        -1,  PRRO_INVALID_REG, -1}\
}

#define PRRO_SEC_MAP_ENTITY \
{\
	{PRRO_DEV_AHB_AON,        0, 5,  0 },\
	{PRRO_DEV_AHB_SYSTEM,     1, 5,  1 },\
	{PRRO_DEV_AHB_FLASH,      2, 5,  2 },\
	{PRRO_DEV_AHB_GDMA,       3, 5,  3 },\
	{PRRO_DEV_AHB_LA,         4, 5,  4 },\
	{PRRO_DEV_AHB_USB,        5, 5,  5 },\
	{PRRO_DEV_AHB_CAN,        6, 5,  6 },\
	{PRRO_DEV_AHB_QSPI0,      7, 5,  7 },\
	{PRRO_DEV_AHB_QSPI1,      8, 5,  8 },\
	{PRRO_DEV_AHB_PSRAM,      9, 5,  9 },\
	{PRRO_DEV_AHB_FFT,        10, 5, 10},\
	{PRRO_DEV_AHB_SBC,        11, 5, 11},\
	{PRRO_DEV_AHB_JPEG_ENC,   12, 5, 12},\
	{PRRO_DEV_AHB_JPEG_DEC,   13, 5, 14},\
	{PRRO_DEV_AHB_YUV,        14, 5, 13},\
	{PRRO_DEV_AHB_LCD_DISP,   15, 5, 15},\
	{PRRO_DEV_AHB_DMA2D,      16, 5, 16},\
	{PRRO_DEV_AHB_ROTT,       17, 5, 17},\
	{PRRO_DEV_AHB_WIFI_MAC,   18, 5, 18},\
	{PRRO_DEV_AHB_WIFI_MODEM, 19, 5, 19},\
	{PRRO_DEV_AHB_BTDM_AHB,   20, 5, 20},\
	{PRRO_DEV_AHB_MBOX0,      21, 5, 21},\
	{PRRO_DEV_AHB_MBOX1,      22, 5, 22},\
	{PRRO_DEV_AHB_DMA1,       23, 5, 23},\
	{PRRO_DEV_AHB_MAX,        -1,  PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_APB_WDT,        0, 8, 0 },\
	{PRRO_DEV_APB_EFUSE,      1, 8, 4 },\
	{PRRO_DEV_APB_MBIC,       2, 8, 5 },\
	{PRRO_DEV_APB_SDMADC,     3, 8, 7 },\
	{PRRO_DEV_APB_TIMER0,     4, 8, 1 },\
	{PRRO_DEV_APB_TIMER1,     5, 8, 6 },\
	{PRRO_DEV_APB_PWM0,       6, 8, 14 },\
	{PRRO_DEV_APB_PWM1,       7, 8, 19 },\
	{PRRO_DEV_APB_UART0,      8, 8, 2 },\
	{PRRO_DEV_APB_UART1,      9, 8, 8 },\
	{PRRO_DEV_APB_UART2,      10, 8, 9 },\
	{PRRO_DEV_APB_SPI0,       11, 8, 3},\
	{PRRO_DEV_APB_SPI1,       12, 8, 12},\
	{PRRO_DEV_APB_I2C0,       13, 8, 10},\
	{PRRO_DEV_APB_I2C1,       14, 8, 11},\
	{PRRO_DEV_APB_SARADC,     15, 8, 13},\
	{PRRO_DEV_APB_IRDA,       16, 8, 15},\
	{PRRO_DEV_APB_TRNG,       17, 8, 16},\
	{PRRO_DEV_APB_SDIO,       18, 8, 17},\
	{PRRO_DEV_APB_SLCD,       19, 8, 18},\
	{PRRO_DEV_APB_AUDIO,      20, 8, 20},\
	{PRRO_DEV_APB_I2S0,       21, 8, 21},\
	{PRRO_DEV_APB_I2S1,       22, 8, 22},\
	{PRRO_DEV_APB_I2S2,       23, 8, 23},\
	{PRRO_DEV_APB_VIDP,       24, 8, 24},\
	{PRRO_DEV_APB_BTDM_APB,   25, 8, 25},\
	{PRRO_DEV_APB_MAX,        -1,  PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_AON_PMU,  0, 0xa, 0},\
	{PRRO_DEV_AON_RTC,  1, 0xa, 1},\
	{PRRO_DEV_AON_GPIO, 2, 0xa, 2},\
	{PRRO_DEV_AON_WDT,  3, 0xa, 3},\
	{PRRO_DEV_AON_MAX, -1,  PRRO_INVALID_REG, -1},\
\
	{PRRO_DEV_JPEC_ENC_HNONSEC, 0, 0xd, 0},\
	{PRRO_DEV_JPEC_DEC_HNONSEC, 1, 0xe, 0},\
	{PRRO_DEV_DMA2D_HNONSEC,    2, 0xf, 0},\
	{PRRO_DEV_WIFI_MAC_HNONSEC, 3, 0x10, 0},\
	{PRRO_DEV_HSU_HNONSEC,      4, 0x11, 0},\
	{PRRO_DEV_BTDM_HNONSEC,     5, 0x12, 0},\
	{PRRO_DEV_LA_HNONSEC,       6, 0x13, 0},\
	{PRRO_DEV_LCD_DISP_HNONSEC, 7, 0x14, 0},\
	{PRRO_DEV_ENC_HNONSEC,      8, 0x15, 0},\
	{PRRO_DEV_MAX_HNONSEC,      -1, PRRO_INVALID_REG,    -1}\
}

#endif
