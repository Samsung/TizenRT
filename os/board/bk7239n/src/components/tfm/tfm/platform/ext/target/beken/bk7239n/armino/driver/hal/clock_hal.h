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
#include "clock_ll.h"


//set dco clock division
#define clock_hal_set_dco_1_div(hal) clock_ll_set_dco_clk_1_div((hal)->hw)
#define clock_hal_set_dco_2_div(hal) clock_ll_set_dco_clk_2_div((hal)->hw)
#define clock_hal_set_dco_4_div(hal) clock_ll_set_dco_clk_4_div((hal)->hw)
#define clock_hal_set_dco_8_div(hal) clock_ll_set_dco_clk_8_div((hal)->hw)

#define clock_hal_set_26m_1_div(hal) clock_ll_set_26m_clk_1_div((hal)->hw)
#define clock_hal_set_26m_2_div(hal) clock_ll_set_26m_clk_2_div((hal)->hw)
#define clock_hal_set_26m_4_div(hal) clock_ll_set_26m_clk_4_div((hal)->hw)
#define clock_hal_set_26m_8_div(hal) clock_ll_set_26m_clk_8_div((hal)->hw)



//set peripheral clock source
#define clock_hal_uart_set_clk_26m(hal, id) clock_ll_set_uart_clk_26m((hal)->hw, id)
#define clock_hal_uart_set_clk_dco(hal, id) clock_ll_set_uart_clk_dco((hal)->hw, id)

#define clock_hal_i2c_set_clk_26m(hal, id) clock_ll_set_i2c_clk_26m((hal)->hw, id)
#define clock_hal_i2c_set_clk_dco(hal, id) clock_ll_set_i2c_clk_dco((hal)->hw, id)

#define clock_hal_irda_set_clk_26m(hal) clock_ll_set_irda_clk_26m((hal)->hw)
#define clock_hal_irda_set_clk_dco(hal) clock_ll_set_irda_clk_dco((hal)->hw)

#define clock_hal_spi_set_clk_26m(hal, id) clock_ll_set_spi_clk_26m((hal)->hw, id)
#define clock_hal_spi_set_clk_dco(hal, id) clock_ll_set_spi_clk_dco((hal)->hw, id)

#define clock_hal_saradc_set_clk_26m(hal) clock_ll_set_saradc_clk_26m((hal)->hw)
#define clock_hal_saradc_set_clk_dco(hal) clock_ll_set_saradc_clk_dco((hal)->hw)

#define clock_hal_pwms_set_clk_26m(hal) clock_ll_set_pwms_clk_26m((hal)->hw)
#define clock_hal_pwms_set_clk_dco(hal) clock_ll_set_pwms_clk_dco((hal)->hw)

#define clock_hal_enable_pwm_lpo_clk(hal, chan) clock_ll_enable_pwm_lpo_clk((hal)->hw, chan)
#define clock_hal_disable_pwm_lpo_clk(hal, chan) clock_ll_disable_pwm_lpo_clk((hal)->hw, chan)

#define clock_hal_sdio_set_clk_26m(hal) clock_ll_set_sdio_clk_26m((hal)->hw)
#define clock_hal_sdio_set_clk_dco(hal) clock_ll_set_sdio_clk_dco((hal)->hw)

#define clock_hal_saradc_enable_audio_pll(hal) clock_ll_set_saradc_enable_audio_pll((hal)->hw)
#define clock_hal_saradc_disable_audio_pll(hal) clock_ll_set_saradc_disable_audio_pll((hal)->hw)

#define clock_hal_qspi_set_clk_26m(hal) clock_ll_set_qspi_clk_26m((hal)->hw)
#define clock_hal_qspi_set_clk_dco(hal) clock_ll_set_qspi_clk_dco((hal)->hw)
#define clock_hal_qspi_set_clk_120m(hal) clock_ll_set_qspi_clk_120m((hal)->hw)
#define clock_hal_qspi_set_clk_80m(hal) clock_ll_set_qspi_clk_80m((hal)->hw)

#define clock_hal_efuse_set_clk_26m(hal) clock_ll_set_efuse_clk_26m((hal)->hw)
#define clock_hal_efuse_set_clk_dco(hal) clock_ll_set_efuse_clk_dco((hal)->hw)

#define clock_hal_cec_set_clk_26m(hal) clock_ll_set_cec_clk_26m((hal)->hw)
#define clock_hal_cec_set_clk_dco(hal) clock_ll_set_cec_clk_dco((hal)->hw)

#define clock_hal_set_i2s_1_div(hal) clock_ll_set_i2s_mclk_1_div((hal)->hw)
#define clock_hal_set_i2s_2_div(hal) clock_ll_set_i2s_mclk_2_div((hal)->hw)
#define clock_hal_set_i2s_4_div(hal) clock_ll_set_i2s_mclk_4_div((hal)->hw)
#define clock_hal_set_i2s_8_div(hal) clock_ll_set_i2s_mclk_8_div((hal)->hw)
#define clock_hal_set_i2s_16_div(hal) clock_ll_set_i2s_mclk_16_div((hal)->hw)
#define clock_hal_set_i2s_32_div(hal) clock_ll_set_i2s_mclk_32_div((hal)->hw)

#define clock_hal_get_uart_clk(hal ,id) clock_ll_get_uart_clk((hal)->hw, id)

#if CONFIG_SOC_BK7236A
#define clock_hal_jpeg_set_96m(hal) icu_ll_jpeg_set_96m((hal)->hw)
#define clock_hal_jpeg_set_120m(hal) icu_ll_jpeg_set_120m((hal)->hw)
#define clock_hal_jpeg_set_160m(hal) icu_ll_jpeg_set_160m((hal)->hw)
#define clock_hal_jpeg_set_240m(hal) icu_ll_jpeg_set_240m((hal)->hw)
#endif

#ifdef __cplusplus
}
#endif
