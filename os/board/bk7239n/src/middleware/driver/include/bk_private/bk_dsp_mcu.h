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

#include <common/sys_config.h>

#if (defined(CONFIG_SOC_BK7271))
#define D_SYS_CTRL_BASE_ADDR         0x08800000
#define D_PMU_BASE_ADDR              0x08800200
#define D_LCD_BASE_ADDR              0x08800280
#define D_GPIO_BASE_ADDR             0x08800300
#define D_SDIO_DMA_BASE_ADDR         0x08801000
#define D_APB_BUS_BASE_ADDR          0x08802000
#define D_ICU_BASE_ADDR              0x08802000
#define D_UART1_BASE_ADDR            0x08802100
#define D_UART2_BASE_ADDR            0x08802140
#define D_UART3_BASE_ADDR            0x08802180
#define D_FM_I2C_BASE_ADDR           0x08802200
#define D_I2C1_BASE_ADDR             0x08802240
#define D_I2C2_BASE_ADDR             0x08802280
#define D_SDIO_HOST_BASE_ADDR        0x08802300
#define D_SPI1_BASE_ADDR             0x08802500
#define D_SPI2_BASE_ADDR             0x08802540
#define D_SPI3_BASE_ADDR             0x08802580
#define D_WDT_BASE_ADDR              0x08802700
#define D_TRNG_BASE_ADDR             0x08802720
#define D_EFUSE_BASE_ADDR            0x08802740
#define D_IRDA_BASE_ADDR             0x08802760
#define D_TIMER1_BASE_ADDR           0x08802780
#define D_TIMER2_BASE_ADDR           0x088027C0
#define D_PWM1_BASE_ADDR             0x08802800
#define D_PWM2_BASE_ADDR             0x08802840
#define D_PWM3_BASE_ADDR             0x08802880
#define D_PWM4_BASE_ADDR             0x088028C0
#define D_SADC_BASE_ADDR             0x08802900
#define D_CEC_BASE_ADDR              0x08802A00
#define D_FLASH_BASE_ADDR            0x08803000
#define D_USB1_BASE_ADDR             0x08804000
#define D_USB2_BASE_ADDR             0x08804800
#define D_GENER_DMA_BASE_ADDR        0x08805000
#define D_SECURITY_BASE_ADDR         0x08806000
#define D_JPEG_BASE_ADDR             0x08807000
#define D_MDM_CFG_BASE_ADDR          0x08900000
#define D_MDM_STAT_BASE_ADDR         0x08900000
#define D_RC_BEKEN_BASE_ADDR         0x08950000
#define D_TRX_BEKEN_BASE_ADDR        0x08950080
#define D_MACPHY_BYPASS_BASE_ADDR    0x08960000
#define D_MAC_CORE_BASE_ADDR         0x08A00000
#define D_MAC_PL_BASE_ADDR           0x08A08000
#define D_YUV_MEM_BASE_ADDR          0x08B00000
#define D_WIFI_DTCM_512KB_BASE_ADDR  0x08400000
#define D_WIFI_ITCM_32KB_BASE_ADDR   0x083F8000
#define D_BUS_SMEM_128KB_BASE_ADDR   0x04000000
#define D_DSP_DMEM_64KB_BASE_ADDR    0x00000000
#define D_DSP_CPM_BASE_ADDR          0x00400000
#define D_DSP_SMEM_1_5MB_BASE_ADDR   0x00800000
#define D_PSRAM_BASE_ADDR            0x01000000
#define D_DMA_BASE_ADDR              0x02800000
#define D_FFT_BASE_ADDR              0x02810000
#define D_APBD_BUS_BASE_ADDR         0x028F0000
#define D_ICUD_BASE_ADDR             0x028F0000
#define D_AUDIO_BASE_ADDR            0x028F1000
#define D_SPDIF_BASE_ADDR            0x028F2000
#define D_EQ_BASE_ADDR               0x028F3000
#define D_I2S1_BASE_ADDR             0x028F4000
#define D_WDTD_BASE_ADDR             0x028F5000
#define D_RSV_BASE_ADDR              0x028F6000
#define D_DSP_CTRL_BASE_ADDR         0x028F7000
#define D_I2S2_BASE_ADDR             0x028F8000
#define D_I2S3_BASE_ADDR             0x028F9000
#define D_PSRAM_CTRL_BASE_ADDR       0x028FA000
#define D_BT_IMEM_384KB_BASE_ADDR    0x10000000
#define D_BT_DMEM_96KB_BASE_ADDR     0x10400000
#endif

#ifdef __cplusplus
}
#endif
