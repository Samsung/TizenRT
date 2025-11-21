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
#define B_SYS_CTRL_BASE_ADDR            0x08800000
#define B_PMU_BASE_ADDR                 0x08800200
#define B_LCD_BASE_ADDR                 0x08800280
#define B_GPIO_BASE_ADDR                0x08800300
#define B_SDIO_DMA_BASE_ADDR            0x08801000
#define B_APB_BUS_BASE_ADDR             0x08802000
#define B_ICU_BASE_ADDR                 0x08802000
#define B_UART1_BASE_ADDR               0x08802100
#define B_UART2_BASE_ADDR               0x08802140
#define B_UART3_BASE_ADDR               0x08802180
#define B_FM_I2C_BASE_ADDR              0x08802200
#define B_I2C1_BASE_ADDR                0x08802240
#define B_I2C2_BASE_ADDR                0x08802280
#define B_SDIO_HOST_BASE_ADDR           0x08802300
#define B_SPI1_BASE_ADDR                0x08802500
#define B_SPI2_BASE_ADDR                0x08802540
#define B_SPI3_BASE_ADDR                0x08802580
#define B_WDT_BASE_ADDR                 0x08802700
#define B_TRNG_BASE_ADDR                0x08802720
#define B_EFUSE_BASE_ADDR               0x08802740
#define B_IRDA_BASE_ADDR                0x08802760
#define B_TIMER1_BASE_ADDR              0x08802780
#define B_TIMER2_BASE_ADDR              0x088027C0
#define B_PWM1_BASE_ADDR                0x08802800
#define B_PWM2_BASE_ADDR                0x08802840
#define B_PWM3_BASE_ADDR                0x08802880
#define B_PWM4_BASE_ADDR                0x088028C0
#define B_SADC_BASE_ADDR                0x08802900
#define B_CEC_BASE_ADDR                 0x08802A00
#define B_FLASH_BASE_ADDR               0x08803000
#define B_USB1_BASE_ADDR                0x08804000
#define B_USB2_BASE_ADDR                0x08804800
#define B_GENER_DMA_BASE_ADDR           0x08805000
#define B_SECURITY_BASE_ADDR            0x08806000
#define B_JPEG_BASE_ADDR                0x08807000
#define B_MDM_CFG_BASE_ADDR             0x08900000
#define B_MDM_STAT_BASE_ADDR            0x08900000
#define B_RC_BEKEN_BASE_ADDR            0x08950000
#define B_TRX_BEKEN_BASE_ADDR           0x08950080
#define B_MACPHY_BYPASS_BASE_ADDR       0x08960000
#define B_MAC_CORE_BASE_ADDR            0x08A00000
#define B_MAC_PL_BASE_ADDR              0x08A08000
#define B_YUV_MEM_BASE_ADDR             0x08B00000
#define B_WIFI_DTCM_512KB_BASE_ADDR     0x08400000
#define B_WIFI_ITCM_32KB_BASE_ADDR      0x083F8000
#define B_BUS_SMEM_128KB_BASE_ADDR      0x04000000
#define B_DSP_DMEM_64KB_BASE_ADDR       0x0C000000
#define B_DSP_CPM_BASE_ADDR             0x0C400000
#define B_DSP_SMEM_1_5MB_BASE_ADDR      0x0C800000
#define B_PSRAM_BASE_ADDR               0x0D000000
#define B_DMA_BASE_ADDR                 0x0E800000
#define B_FFT_BASE_ADDR                 0x0E810000
#define B_APBD_BUS_BASE_ADDR            0x0E8F0000
#define B_ICUD_BASE_ADDR                0x0E8F0000
#define B_AUDIO_BASE_ADDR               0x0E8F1000
#define B_SPDIF_BASE_ADDR               0x0E8F2000
#define B_EQ_BASE_ADDR                  0x0E8F3000
#define B_I2S1_BASE_ADDR                0x0E8F4000
#define B_WDTD_BASE_ADDR                0x0E8F5000
#define B_RSV_BASE_ADDR                 0x0E8F6000
#define B_DSP_CTRL_BASE_ADDR            0x0E8F7000
#define B_I2S2_BASE_ADDR                0x0E8F8000
#define B_I2S3_BASE_ADDR                0x0E8F9000
#define B_PSRAM_CTRL_BASE_ADDR          0x0E8FA000
#define B_BT_IMEM_384KB_BASE_ADDR       0x00000000
#define B_BT_DMEM_96KB_BASE_ADDR        0x00400000
#endif

#ifdef __cplusplus
}
#endif
