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

#include "power_hal.h"
#include "icu_driver.h"
extern icu_driver_t s_icu;

/**< uart related APIs */
#define power_uart_pwr_up(id)		power_hal_uart_pwr_up(&s_icu.hal, id)
#define power_uart_pwr_down(id)		power_hal_uart_pwr_down(&s_icu.hal, id)

/**< i2c related APIs */
#define power_i2c_pwr_up(id)		power_hal_i2c_pwr_up(&s_icu.hal, id)
#define power_i2c_pwr_down(id)		power_hal_i2c_pwr_down(&s_icu.hal, id)

/**< irda related APIs */
#define power_irda_pwr_up()		power_hal_irda_pwr_up(&s_icu.hal)
#define power_irda_pwr_down()		power_hal_irda_pwr_down(&s_icu.hal)

/**< i2s related APIs */
#define power_i2s_pwr_up()		power_hal_i2s_pcm_pwr_up(&s_icu.hal)
#define power_i2s_pwr_down()		power_hal_i2s_pcm_pwr_down(&s_icu.hal)

/**< spi related APIs */
#define power_up_spi(id)		power_hal_pwr_up_spi(&s_icu.hal, id)
#define power_down_spi(id)		power_hal_pwr_down_spi(&s_icu.hal, id)

/**< saradc related APIs */
#define power_saradc_pwr_up()		power_hal_saradc_pwr_up(&s_icu.hal)
#define power_saradc_pwr_down()		power_hal_saradc_pwr_down(&s_icu.hal)

/**<pwm related APIs */
#define power_pwm_pwr_up(chan)		power_hal_pwm_pwr_up(&s_icu.hal, chan)
#define power_pwm_pwr_down(chan)	power_hal_pwm_pwr_down(&s_icu.hal, chan)

/**< timer related APIs */
#define power_pwr_up_timer(chan)	power_hal_pwr_up_timer(&s_icu.hal, chan)
#define power_pwr_down_timer(chan)	power_hal_pwr_down_timer(&s_icu.hal, chan)

/**< sdio host related APIs */
#define power_sdio_pwr_up()		power_hal_sdio_pwr_up(&s_icu.hal)
#define power_sdio_pwr_down()		power_hal_sdio_pwr_down(&s_icu.hal)

/**< arm wdt related APIs */
#define power_arm_wdt_pwr_up()		power_hal_arm_wdt_pwr_up(&s_icu.hal)
#define power_arm_wdt_pwr_down()	power_hal_arm_wdt_pwr_down(&s_icu.hal)

/**< tl410 wdt related APIs */
#define power_tl410_pwr_up()		power_hal_tl410_pwr_up(&s_icu.hal)
#define power_tl410_pwr_down()		power_hal_tl410_pwr_down(&s_icu.hal)
#define power_tl410_wdt_pwr_up()	power_hal_tl410_wdt_pwr_up(&s_icu.hal)
#define power_tl410_wdt_pwr_down()	power_hal_tl410_wdt_pwr_down(&s_icu.hal)

#define power_audio_pwr_up()		power_hal_audio_pwr_up(&s_icu.hal)
#define power_audio_pwr_down()		power_hal_audio_pwr_down(&s_icu.hal)

/**< usb related APIs */
#define power_usb_pwr_up(id)		power_hal_usb_pwr_up(&s_icu.hal, id)
#define power_usb_pwr_down(id)		power_hal_usb_pwr_down(&s_icu.hal, id)

/**< fft related APIs */
#define power_fft_pwr_up()		power_hal_fft_pwr_up(&s_icu.hal)
#define power_fft_pwr_down()		power_hal_fft_pwr_down(&s_icu.hal)

/**< jpeg related APIs */
#define power_jpeg_pwr_up()		power_hal_jpeg_pwr_up(&s_icu.hal)
#define power_jpeg_pwr_down()		power_hal_jpeg_pwr_down(&s_icu.hal)

#define power_qspi_pwr_up()		power_hal_qspi_pwr_up(&s_icu.hal)
#define power_qspi_pwr_down()		power_hal_qspi_pwr_down(&s_icu.hal)

/**< ble related APIs */
#define power_ble_pwr_up()		power_hal_ble_pwr_up(&s_icu.hal)
#define power_ble_pwr_down()		power_hal_ble_pwr_down(&s_icu.hal)

/**< security related APIs */
#define power_security_pwr_up()		power_hal_security_pwr_up(&s_icu.hal)
#define power_security_pwr_down()	power_hal_security_pwr_down(&s_icu.hal)

/**< cec related APIs */
#define power_cec_pwr_up()		power_hal_cec_pwr_up(&s_icu.hal)
#define power_cec_pwr_down()		power_hal_cec_pwr_down(&s_icu.hal)

/**< sdio_dma related APIs */
#define power_sdio_dma_pwr_up()		power_hal_sdio_dma_pwr_up(&s_icu.hal)
#define power_sdio_dma_pwr_down()	power_hal_sdio_dma_pwr_down(&s_icu.hal)

/**< wdt related APIs */
#define power_up_wdt() power_hal_pwr_up_wdt(&s_icu.hal)
#define power_down_wdt() power_hal_pwr_down_wdt(&s_icu.hal)

