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

#include <components/log.h>
#include "clock_hal.h"

#if (CONFIG_SYSTEM_CTRL)
#include "sys_hal.h"

#define clk_set_uart_clk_26m(id)    sys_hal_uart_select_clock(id,UART_SCLK_XTAL_26M)

#define clk_set_spi_clk_26m(id)     //sys_hal_set_clksel_spi(SPI_CLK_SRC_XTAL)
#define clk_set_spi_clk_dco(id)     //sys_hal_set_clksel_spi(SPI_CLK_SRC_UNKNOW)//spi don't support DCO clock source

#define clk_enable_saradc_audio_pll()   //do nothing
#define clk_disable_saradc_audio_pll()  //do nothing
#define clk_set_saradc_clk_26m()	sys_hal_set_cksel_sadc(0)
#define clk_set_saradc_clk_dco()	sys_hal_set_cksel_sadc(1)

#define clk_set_pwms_clk_26m()		sys_hal_set_cksel_pwm(1)
#define clk_set_pwms_clk_dco()		//pwm don't support DCO clock source
#define clk_enable_pwm_clk_lpo(chan)	//pwm don't support lpo clock source
#define clk_disable_pwm_clk_lpo(chan)	//pwm don't support lpo clock source

#define clk_set_qspi_clk_26m()      //do nothing
#define clk_set_qspi_clk_dco()      //do nothing
#define clk_set_qspi_clk_120m()     //do nothing
#define clk_set_qspi_clk_80m()      //do nothing

#define clk_get_uart_clk(id)        sys_hal_uart_select_clock_get(id)
#else
#include "icu_driver.h"

extern icu_driver_t s_icu;

/**< clock source division APIs: 26M XTAL division only for gpio output clock*/
#define clk_set_dco_1_div()	clock_hal_set_dco_1_div(&s_icu.hal)
#define clk_set_dco_2_div()	clock_hal_set_dco_2_div(&s_icu.hal)
#define clk_set_dco_4_div()	clock_hal_set_dco_4_div(&s_icu.hal)
#define clk_set_dco_8_div()	clock_hal_set_dco_8_div(&s_icu.hal)


#define clk_set_26m_1_div()	clock_hal_set_26m_1_div(&s_icu.hal)
#define clk_set_26m_2_div()	clock_hal_set_26m_2_div(&s_icu.hal)
#define clk_set_26m_4_div()	clock_hal_set_26m_4_div(&s_icu.hal)
#define clk_set_26m_8_div()	clock_hal_set_26m_8_div(&s_icu.hal)

#define clk_set_uart_clk_26m(id)	clock_hal_uart_set_clk_26m(&s_icu.hal, id)
#define clk_set_uart_clk_dco(id)	clock_hal_uart_set_clk_dco(&s_icu.hal, id)

#define clk_set_i2c_clk_26m(id)		clock_hal_i2c_set_clk_26m(&s_icu.hal, id)
#define clk_set_i2c_clk_dco(id)		clock_hal_i2c_set_clk_dco(&s_icu.hal, id)

#define clk_set_irda_clk_26m()		clock_hal_irda_set_clk_26m(&s_icu.hal)
#define clk_set_irda_clk_dco()		clock_hal_irda_set_clk_dco(&s_icu.hal)

#define clk_set_spi_clk_26m(id)		clock_hal_spi_set_clk_26m(&s_icu.hal, id)
#define clk_set_spi_clk_dco(id)		clock_hal_spi_set_clk_dco(&s_icu.hal, id)

#define clk_enable_saradc_audio_pll()	clock_hal_saradc_enable_audio_pll(&s_icu.hal)
#define clk_disable_saradc_audio_pll()	clock_hal_saradc_disable_audio_pll(&s_icu.hal)
#define clk_set_saradc_clk_26m()	clock_hal_saradc_set_clk_26m(&s_icu.hal)
#define clk_set_saradc_clk_dco()	clock_hal_saradc_set_clk_dco(&s_icu.hal)

#define clk_set_pwms_clk_26m()		clock_hal_pwms_set_clk_26m(&s_icu.hal)
#define clk_set_pwms_clk_dco()		clock_hal_pwms_set_clk_dco(&s_icu.hal)
#define clk_enable_pwm_clk_lpo(chan)	clock_hal_enable_pwm_lpo_clk(&s_icu.hal, chan)
#define clk_disable_pwm_clk_lpo(chan)	clock_hal_disable_pwm_lpo_clk(&s_icu.hal, chan)

#define clk_set_sdio_clk_26m()		clock_hal_sdio_set_clk_26m(&s_icu.hal)
#define clk_set_sdio_clk_dco()		clock_hal_sdio_set_clk_dco(&s_icu.hal)

#define clk_set_qspi_clk_26m()		clock_hal_qspi_set_clk_26m(&s_icu.hal)
#define clk_set_qspi_clk_dco()		clock_hal_qspi_set_clk_dco(&s_icu.hal)
#define clk_set_qspi_clk_120m()		clock_hal_qspi_set_clk_120m(&s_icu.hal)
#define clk_set_qspi_clk_80m()		clock_hal_qspi_set_clk_80m(&s_icu.hal)

#define clk_set_efuse_clk_26m()		clock_hal_efuse_set_clk_26m(&s_icu.hal)
#define clk_set_efuse_clk_dco()		clock_hal_efuse_set_clk_dco(&s_icu.hal)

#define clk_set_cec_clk_26m()		clock_hal_cec_set_clk_26m(&s_icu.hal)
#define clk_set_cec_clk_dco()		clock_hal_cec_set_clk_dco(&s_icu.hal)

#define clk_set_i2s_1_div()		clock_hal_set_i2s_1_div(&s_icu.hal)
#define clk_set_i2s_2_div()		clock_hal_set_i2s_2_div(&s_icu.hal)
#define clk_set_i2s_4_div()		clock_hal_set_i2s_4_div(&s_icu.hal)
#define clk_set_i2s_8_div()		clock_hal_set_i2s_8_div(&s_icu.hal)
#define clk_set_i2s_16_div()		clock_hal_set_i2s_16_div(&s_icu.hal)
#define clk_set_i2s_32_div()		clock_hal_set_i2s_32_div(&s_icu.hal)

#define clk_get_uart_clk(id)		clock_hal_get_uart_clk(&s_icu.hal, id)

#if CONFIG_SOC_BK7236A
#define clock_jpeg_set_96m		clock_hal_jpeg_set_96m(&s_icu.hal)
#define clock_jpeg_set_120m		clock_hal_jpeg_set_120m(&s_icu.hal)
#define clock_jpeg_set_160m		clock_hal_jpeg_set_160m(&s_icu.hal)
#define clock_jpeg_set_240m		clock_hal_jpeg_set_240m(&s_icu.hal)
#endif

#endif

