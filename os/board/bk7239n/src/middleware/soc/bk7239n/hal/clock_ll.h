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

#include <soc/soc.h>
#include "icu_hw.h"


#ifdef __cplusplus
extern "C" {
#endif

#define ICU_LL_REG_BASE() (SOC_ICU_REG_BASE)

static inline void clock_ll_set_dco_clk_1_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_dco_clk_2_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_dco_clk_4_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_dco_clk_8_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_qspi_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_qspi_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_qspi_clk_120m(icu_hw_t *hw)
{
}
static inline void clock_ll_set_qspi_clk_80m(icu_hw_t *hw)
{

}


static inline void clock_ll_set_pwms_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_pwms_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_enable_pwm_lpo_clk(icu_hw_t *hw, uint32_t chan)
{
}

static inline void clock_ll_disable_pwm_lpo_clk(icu_hw_t *hw, uint32_t chan)
{
}

static inline void clock_ll_set_saradc_enable_audio_pll(icu_hw_t *hw)
{
}

static inline void clock_ll_set_saradc_disable_audio_pll(icu_hw_t *hw)
{
}

static inline void clock_ll_set_efuse_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_efuse_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_sdio_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_sdio_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_saradc_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_saradc_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_spi_clk_dco(icu_hw_t *hw, uint32_t id)
{
}

static inline void clock_ll_set_spi_clk_26m(icu_hw_t *hw, uint32_t id)
{
}

static inline void clock_ll_set_irda_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_irda_clk_26m(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2c_clk_dco(icu_hw_t *hw, uint32 id)
{
}

static inline void clock_ll_set_i2c_clk_26m(icu_hw_t *hw, uint32 id)
{
}

static inline void clock_ll_set_uart_clk_26m(icu_hw_t *hw, uint32_t id)
{
}

static inline void clock_ll_set_uart_clk_dco(icu_hw_t *hw, uint32_t id)
{
}

static inline void clock_ll_set_26m_clk_1_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_26m_clk_2_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_26m_clk_4_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_26m_clk_8_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_1_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_2_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_4_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_8_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_16_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_i2s_mclk_32_div(icu_hw_t *hw)
{
}

static inline void clock_ll_set_cec_clk_dco(icu_hw_t *hw)
{
}

static inline void clock_ll_set_cec_clk_26m(icu_hw_t *hw)
{
}

static inline uint32_t clock_ll_get_uart_clk(icu_hw_t *hw, uint32_t id)
{
	return 0;
}
#ifdef __cplusplus
}
#endif
