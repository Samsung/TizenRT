// Copyright 2020-2021 Beken
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

/* This header mainly for configuring the HAL debug options.
 * Normally the HAL debug option should be disabled to reduce the image size.
 **/

//TODO disable all of them
#if CONFIG_DEBUG_FIRMWARE
#define CFG_HAL_DEBUG_ICU        1
#define CFG_HAL_DEBUG_PWM        1
#define CFG_HAL_DEBUG_GPIO       1
#define CFG_HAL_DEBUG_TIMER      1
#define CFG_HAL_DEBUG_DMA        1
#define CFG_HAL_DEBUG_UART       1
#define CFG_HAL_DEBUG_WDT        1
#define CFG_HAL_DEBUG_TRNG       1
#define CFG_HAL_DEBUG_EFUSE      1
#define CFG_HAL_DEBUG_ADC        1
#define CFG_HAL_DEBUG_SDMADC     1
#define CFG_HAL_DEBUG_SPI        1
#define CFG_HAL_DEBUG_QSPI       1
#define CFG_HAL_DEBUG_AON_RTC    1
#define CFG_HAL_DEBUG_I2C        1
#define CFG_HAL_DEBUG_AUD        1
#define CFG_HAL_DEBUG_JPEG       1
#define CFG_HAL_DEBUG_FFT        1
#define CFG_HAL_DEBUG_LCD_DISP   1
#define CFG_HAL_DEBUG_I2S        1
#define CFG_HAL_DEBUG_CALENDAR   1
#define CFG_HAL_DEBUG_FLASH      1
#define CFG_HAL_DEBUG_SDIO_HOST  1
#define CFG_HAL_DEBUG_AON_WDT    1
#define CFG_HAL_DEBUG_DMA2D      1
#define CFG_HAL_DEBUG_JPEG_DEC   1
#define CFG_HAL_DEBUG_YUV_BUF    1
#define CFG_HAL_DEBUG_SYS_CLOCK  1
#define CFG_HAL_DEBUG_CKMN       1
#else
#define CFG_HAL_DEBUG_ICU        0
#define CFG_HAL_DEBUG_PWM        0
#define CFG_HAL_DEBUG_GPIO       0
#define CFG_HAL_DEBUG_TIMER      0
#define CFG_HAL_DEBUG_DMA        0
#define CFG_HAL_DEBUG_UART       0
#define CFG_HAL_DEBUG_WDT        0
#define CFG_HAL_DEBUG_TRNG       0
#define CFG_HAL_DEBUG_EFUSE      0
#define CFG_HAL_DEBUG_ADC        0
#define CFG_HAL_DEBUG_SDMADC     0
#define CFG_HAL_DEBUG_SPI        0
#define CFG_HAL_DEBUG_QSPI       0
#define CFG_HAL_DEBUG_AON_RTC    0
#define CFG_HAL_DEBUG_I2C        0
#define CFG_HAL_DEBUG_AUD        0
#define CFG_HAL_DEBUG_JPEG       0
#define CFG_HAL_DEBUG_FFT        0
#define CFG_HAL_DEBUG_LCD_DISP   0
#define CFG_HAL_DEBUG_I2S        0
#define CFG_HAL_DEBUG_CALENDAR   0
#define CFG_HAL_DEBUG_FLASH      0
#define CFG_HAL_DEBUG_SDIO_HOST  0
#define CFG_HAL_DEBUG_AON_WDT    0
#define CFG_HAL_DEBUG_DMA2D      0
#define CFG_HAL_DEBUG_JPEG_DEC   0
#define CFG_HAL_DEBUG_YUV_BUF    0
#define CFG_HAL_DEBUG_CKMN       0
#endif
#ifdef __cplusplus
}
#endif
