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

#include "hal_config.h"
#include "power_ll.h"


//power up/down each module
#define power_hal_uart_pwr_up(hal, id) power_ll_pwr_up_uart((hal)->hw, (id))
#define power_hal_uart_pwr_down(hal, id) power_ll_pwr_down_uart((hal)->hw, (id))

#define power_hal_i2c_pwr_up(hal, id) power_ll_pwr_up_i2c((hal)->hw, id)
#define power_hal_i2c_pwr_down(hal, id) power_ll_pwr_down_i2c((hal)->hw, id)

#define power_hal_irda_pwr_up(hal) power_ll_pwr_up_irda((hal)->hw)
#define power_hal_irda_pwr_down(hal) power_ll_pwr_down_irda((hal)->hw)

#define power_hal_i2s_pcm_pwr_up(hal) power_ll_pwr_up_i2s_pcm((hal)->hw)
#define power_hal_i2s_pcm_pwr_down(hal) power_ll_pwr_down_i2s_pcm((hal)->hw)

#define power_hal_pwr_up_spi(hal, id) power_ll_pwr_up_spi((hal)->hw, id)
#define power_hal_pwr_down_spi(hal, id) power_ll_pwr_down_spi((hal)->hw, id)

#define power_hal_saradc_pwr_up(hal) power_ll_pwr_up_saradc((hal)->hw)
#define power_hal_saradc_pwr_down(hal) power_ll_pwr_down_saradc((hal)->hw)

#define power_hal_arm_wdt_pwr_up(hal) power_ll_pwr_up_arm_wdt((hal)->hw)
#define power_hal_arm_wdt_pwr_down(hal) power_ll_pwr_down_arm_wdt((hal)->hw)

#define power_hal_pwm_pwr_up(hal, chan) power_ll_pwm_pwr_up((hal)->hw, (chan))
#define power_hal_pwm_pwr_down(hal, chan) power_ll_pwm_pwr_down((hal)->hw, (chan))

#define power_hal_audio_pwr_up(hal) power_ll_pwr_up_audio((hal)->hw)
#define power_hal_audio_pwr_down(hal) power_ll_pwr_down_audio((hal)->hw)

#define power_hal_tl410_wdt_pwr_up(hal) power_ll_pwr_up_tl410_wdt((hal)->hw)
#define power_hal_tl410_wdt_pwr_down(hal) power_ll_pwr_down_tl410_wdt((hal)->hw)

#define power_hal_sdio_pwr_up(hal) power_ll_pwr_up_sdio((hal)->hw)
#define power_hal_sdio_pwr_down(hal) power_ll_pwr_down_sdio((hal)->hw)

#define power_hal_usb_pwr_up(hal, id) power_ll_pwr_up_usb((hal)->hw, id)
#define power_hal_usb_pwr_down(hal, id) power_ll_pwr_down_usb((hal)->hw, id)

#define power_hal_fft_pwr_up(hal) power_ll_pwr_up_fft((hal)->hw)
#define power_hal_fft_pwr_down(hal) power_ll_pwr_down_fft((hal)->hw)

#define power_hal_pwr_up_timer(hal, timer_id) power_ll_pwr_up_timer((hal)->hw, (timer_id))
#define power_hal_pwr_down_timer(hal, timer_id) power_ll_pwr_down_timer((hal)->hw, (timer_id))

#define power_hal_jpeg_pwr_up(hal) power_ll_pwr_up_jpeg((hal)->hw)
#define power_hal_jpeg_pwr_down(hal) power_ll_pwr_down_jpeg((hal)->hw)

#define power_hal_qspi_pwr_up(hal) power_ll_pwr_up_qspi((hal)->hw)
#define power_hal_qspi_pwr_down(hal) power_ll_pwr_down_qspi((hal)->hw)

#define power_hal_ble_pwr_up(hal) power_ll_pwr_up_ble((hal)->hw)
#define power_hal_ble_pwr_down(hal) power_ll_pwr_down_ble((hal)->hw)

#define power_hal_tl410_pwr_up(hal) power_ll_pwr_up_tl410((hal)->hw)
#define power_hal_tl410_pwr_down(hal) power_ll_pwr_down_tl410((hal)->hw)

#define power_hal_security_pwr_up(hal) power_ll_pwr_up_security((hal)->hw)
#define power_hal_security_pwr_down(hal) power_ll_pwr_down_security((hal)->hw)

#define power_hal_cec_pwr_up(hal) power_ll_pwr_up_cec((hal)->hw)
#define power_hal_cec_pwr_down(hal) power_ll_pwr_down_cec((hal)->hw)

#define power_hal_sdio_dma_pwr_up(hal) power_ll_pwr_up_sdio_dma((hal)->hw)
#define power_hal_sdio_dma_pwr_down(hal) power_ll_pwr_down_sdio_dma((hal)->hw)

#define power_hal_pwr_up_wdt(hal) power_ll_pwr_up_wdt((hal)->hw)
#define power_hal_pwr_down_wdt(hal) power_ll_pwr_down_wdt((hal)->hw)

#ifdef __cplusplus
}
#endif
