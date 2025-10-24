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
#include "icu_hw.h"
#include "icu_ll.h"
//#include "interrupt_base.h"

typedef struct {
	icu_hw_t *hw;
} icu_hal_t;

bk_err_t icu_hal_init(icu_hal_t *hal);


//set/get global int
#define icu_hal_enable_fiq(hal) icu_ll_enable_fiq((hal)->hw)
#define icu_hal_disable_fiq(hal) icu_ll_disable_fiq((hal)->hw)

#define icu_hal_enable_irq(hal) icu_ll_enable_irq((hal)->hw)
#define icu_hal_disable_irq(hal) icu_ll_disable_irq((hal)->hw)

#define icu_hal_get_global_int_status(hal) icu_ll_get_int_status((hal)->hw)
#define icu_hal_clear_global_int_status(hal, status) icu_ll_clear_int_status((hal)->hw,status)
#define icu_hal_get_irq_int_status(hal) icu_ll_get_irq_int_status((hal)->hw)
#define icu_hal_clear_irq_int_status(hal, status) icu_ll_clear_irq_int_status((hal)->hw,status)
#define icu_hal_get_fiq_int_status(hal) icu_ll_get_fiq_int_status((hal)->hw)
#define icu_hal_clear_fiq_int_status(hal, status) icu_ll_clear_fiq_int_status((hal)->hw,status)
#define icu_hal_disable_all_interrupt(hal) icu_ll_disable_all_interrupt((hal)->hw)
#define icu_hal_enable_interrupt(hal, index) icu_ll_enable_interrupt((hal)->hw, index)
#define icu_hal_disable_interrupt(hal, index) icu_ll_disable_interrupt((hal)->hw, index)
#define icu_hal_get_uart_int_status(hal) icu_ll_get_uart_int_status((hal)->hw)

//enable/disable intterrupt
#define icu_hal_enable_uart_interrupt(hal, id) icu_ll_enable_uart_interrupt((hal)->hw, id)
#define icu_hal_disable_uart_interrupt(hal, id) icu_ll_disable_uart_interrupt((hal)->hw, id)

#define icu_hal_enable_i2c_interrupt(hal, id) icu_ll_enable_i2c_interrupt((hal)->hw, id)
#define icu_hal_disable_i2c_interrupt(hal, id) icu_ll_disable_i2c_interrupt((hal)->hw, id)

#define icu_hal_enable_irda_interrupt(hal) icu_ll_enable_irda_interrupt((hal)->hw)
#define icu_hal_disable_irda_interrupt(hal) icu_ll_disable_irda_interrupt((hal)->hw)

#define icu_hal_enable_i2s_interrupt(hal) icu_ll_enable_i2s_interrupt((hal)->hw)
#define icu_hal_disable_i2s_interrupt(hal) icu_ll_disable_i2s_interrupt((hal)->hw)

#define icu_hal_enable_spi_interrupt(hal, id) icu_ll_enable_spi_interrupt((hal)->hw, id)
#define icu_hal_disable_spi_interrupt(hal, id) icu_ll_disable_spi_interrupt((hal)->hw, id)

#define icu_hal_enable_gpio_interrupt(hal) icu_ll_enable_gpio_interrupt((hal)->hw)
#define icu_hal_disable_gpio_interrupt(hal) icu_ll_disable_gpio_interrupt((hal)->hw)

#define icu_hal_enable_timer_interrupt(hal) icu_ll_enable_timer_interrupt((hal)->hw)
#define icu_hal_disable_timer_interrupt(hal) icu_ll_disable_timer_interrupt((hal)->hw)

#define icu_hal_enable_pwm_interrupt(hal) icu_ll_enable_pwm_interrupt((hal)->hw)
#define icu_hal_disable_pwm_interrupt(hal) icu_ll_disable_pwm_interrupt((hal)->hw)
#define icu_hal_enable_pwm2_interrupt(hal) icu_ll_enable_pwm2_interrupt((hal)->hw)
#define icu_hal_disable_pwm2_interrupt(hal) icu_ll_disable_pwm2_interrupt((hal)->hw)

#define icu_hal_enable_audio_interrupt(hal) icu_ll_enable_audio_interrupt((hal)->hw)
#define icu_hal_disable_audio_interrupt(hal) icu_ll_disable_audio_interrupt((hal)->hw)

#define icu_hal_enable_saradc_interrupt(hal) icu_ll_enable_saradc_interrupt((hal)->hw)
#define icu_hal_disable_saradc_interrupt(hal) icu_ll_disable_saradc_interrupt((hal)->hw)

#define icu_hal_enable_sdio_interrupt(hal) icu_ll_enable_sdio_interrupt((hal)->hw)
#define icu_hal_disable_sdio_interrupt(hal) icu_ll_disable_sdio_interrupt((hal)->hw)

#define icu_hal_enable_usb_interrupt(hal, id) icu_ll_enable_usb_interrupt((hal)->hw, id)
#define icu_hal_disable_usb_interrupt(hal, id) icu_ll_disable_usb_interrupt((hal)->hw, id)

#define icu_hal_enable_fft_interrupt(hal) icu_ll_enable_fft_interrupt((hal)->hw)
#define icu_hal_disable_fft_interrupt(hal) icu_ll_disable_fft_interrupt((hal)->hw)

#define icu_hal_enable_dma_interrupt(hal) icu_ll_enable_dma_interrupt((hal)->hw)
#define icu_hal_disable_dma_interrupt(hal) icu_ll_disable_dma_interrupt((hal)->hw)

#define icu_hal_enable_modem_interrupt(hal) icu_ll_enable_modem_interrupt((hal)->hw)
#define icu_hal_disable_modem_interrupt(hal) icu_ll_disable_modem_interrupt((hal)->hw)

#define icu_hal_enable_mac_txrx_timer_interrupt(hal) icu_ll_enable_mac_txrx_timer_interrupt((hal)->hw)
#define icu_hal_disable_mac_txrx_timer_interrupt(hal) icu_ll_disable_mac_txrx_timer_interrupt((hal)->hw)

#define icu_hal_enable_mac_txrx_misc_interrupt(hal) icu_ll_enable_mac_txrx_misc_interrupt((hal)->hw)
#define icu_hal_disable_mac_txrx_misc_interrupt(hal) icu_ll_disable_mac_txrx_misc_interrupt((hal)->hw)

#define icu_hal_enable_mac_rx_trigger_interrupt(hal) icu_ll_enable_mac_rx_trigger_interrupt((hal)->hw)
#define icu_hal_disable_mac_rx_trigger_interrupt(hal) icu_ll_disable_mac_rx_trigger_interrupt((hal)->hw)
#define icu_hal_enable_mac_tx_trigger_interrupt(hal) icu_ll_enable_mac_tx_trigger_interrupt((hal)->hw)
#define icu_hal_disable_mac_tx_trigger_interrupt(hal) icu_ll_disable_mac_tx_trigger_interrupt((hal)->hw)

#define icu_hal_enable_mac_prot_trigger_interrupt(hal) icu_ll_enable_mac_prot_trigger_interrupt((hal)->hw)
#define icu_hal_disable_mac_prot_trigger_interrupt(hal) icu_ll_disable_mac_prot_trigger_interrupt((hal)->hw)

#define icu_hal_enable_mac_general_interrupt(hal) icu_ll_enable_mac_general_interrupt((hal)->hw)
#define icu_hal_disable_mac_general_interrupt(hal) icu_ll_disable_mac_general_interrupt((hal)->hw)

#define icu_hal_enable_sdio_dma_interrupt(hal) icu_ll_enable_sdio_dma_interrupt((hal)->hw)
#define icu_hal_disable_sdio_dma_interrupt(hal) icu_ll_disable_sdio_dma_interrupt((hal)->hw)

#define icu_hal_enable_usb_plug_interrupt(hal) icu_ll_enable_usb_plug_interrupt((hal)->hw)
#define icu_hal_disable_usb_plug_interrupt(hal) icu_ll_disable_usb_plug_interrupt((hal)->hw)

#define icu_hal_enable_security_interrupt(hal) icu_ll_enable_security_interrupt((hal)->hw)
#define icu_hal_disable_security_interrupt(hal) icu_ll_disable_security_interrupt((hal)->hw)

#define icu_hal_enable_mac_wakeup_interrupt(hal) icu_ll_enable_mac_wakeup_interrupt((hal)->hw)
#define icu_hal_disable_mac_wakeup_interrupt(hal) icu_ll_disable_mac_wakeup_interrupt((hal)->hw)

#define icu_hal_enable_hsspi_slave_interrupt(hal) icu_ll_enable_hsspi_slave_interrupt((hal)->hw)
#define icu_hal_disable_hsspi_slave_interrupt(hal) icu_ll_disable_hsspi_slave_interrupt((hal)->hw)

#define icu_hal_enable_dpll_unlock_interrupt(hal) icu_ll_enable_pll_unlock_interrupt((hal)->hw)
#define icu_hal_disable_dpll_unlock_interrupt(hal) icu_ll_disable_pll_unlock_interrupt((hal)->hw)

#define icu_hal_enable_jpeg_interrupt(hal) icu_ll_enable_jpeg_interrupt((hal)->hw)
#define icu_hal_disable_jpeg_interrupt(hal) icu_ll_disable_jpeg_interrupt((hal)->hw)

#define icu_hal_enable_ble_interrupt(hal) icu_ll_enable_ble_interrupt((hal)->hw)
#define icu_hal_disable_ble_interrupt(hal) icu_ll_disable_ble_interrupt((hal)->hw)

#define icu_hal_enable_psram_interrupt(hal) icu_ll_enable_psram_interrupt((hal)->hw)
#define icu_hal_disable_psram_interrupt(hal) icu_ll_disable_psram_interrupt((hal)->hw)

#define icu_hal_enable_la_interrupt(hal) icu_ll_enable_la_interrupt((hal)->hw)
#define icu_hal_disable_la_interrupt(hal) icu_ll_disable_la_interrupt((hal)->hw)
#define icu_hal_enable_btdm_interrupt(hal) icu_ll_enable_btdm_interrupt((hal)->hw)
#define icu_hal_disable_btdm_interrupt(hal) icu_ll_disable_btdm_interrupt((hal)->hw)
#define icu_hal_enable_bt_interrupt(hal) icu_ll_enable_bt_interrupt((hal)->hw)
#define icu_hal_disable_bt_interrupt(hal) icu_ll_disable_bt_interrupt((hal)->hw)

#define icu_hal_enable_mailbox_dsp_interrupt(hal) icu_ll_enable_mailbox_dsp_interrupt((hal)->hw)
#define icu_hal_disable_mailbox_dsp_interrupt(hal) icu_ll_disable_mailbox_dsp_interrupt((hal)->hw)
#define icu_hal_enable_mailbox_bt_interrupt(hal) icu_ll_enable_mailbox_bt_interrupt((hal)->hw)
#define icu_hal_disable_mailbox_bt_interrupt(hal) icu_ll_disable_mailbox_bt_interrupt((hal)->hw)
#define icu_hal_enable_cec_interrupt(hal) icu_ll_enable_cec_interrupt((hal)->hw)
#define icu_hal_disable_cec_interrupt(hal) icu_ll_disable_cec_interrupt((hal)->hw)
#define icu_hal_enable_touch_interrupt(hal) icu_ll_enable_touch_interrupt((hal)->hw)
#define icu_hal_disable_touch_interrupt(hal) icu_ll_disable_touch_interrupt((hal)->hw)
#define icu_hal_enable_rtc_interrupt(hal) icu_ll_enable_rtc_interrupt((hal)->hw)
#define icu_hal_disable_rtc_interrupt(hal) icu_ll_disable_rtc_interrupt((hal)->hw)
#define icu_hal_enable_dsp_wdt_interrupt(hal) icu_ll_enable_dsp_wdt_interrupt((hal)->hw)
#define icu_hal_disable_dsp_wdt_interrupt(hal) icu_ll_disable_dsp_wdt_interrupt((hal)->hw)
#define icu_hal_enable_bt_wdt_interrupt(hal) icu_ll_enable_bt_wdt_interrupt((hal)->hw)
#define icu_hal_disable_bt_wdt_interrupt(hal) icu_ll_disable_bt_wdt_interrupt((hal)->hw)


//get peripheral intterrupt bit in icu and priority
#define icu_hal_is_pwm_has_two_int(hal) icu_ll_is_pwm_has_two_int((hal)->hw)


//enable/disable arm wakeup
#define icu_hal_enable_uart_arm_wakeup(hal, id) icu_ll_enable_uart_arm_wakeup((hal)->hw, id)
#define icu_hal_disable_uart_arm_wakeup(hal, id) icu_ll_disable_uart_arm_wakeup((hal)->hw. id)

#define icu_hal_enable_i2c_arm_wakeup(hal, id) icu_ll_enable_i2c_arm_wakeup((hal)->hw, id)
#define icu_hal_disable_i2c_arm_wakeup(hal, id) icu_ll_disable_i2c_arm_wakeup((hal)->hw. id)

#define icu_hal_enable_irda_arm_wakeup(hal) icu_ll_enable_irda_arm_wakeup((hal)->hw)
#define icu_hal_disable_irda_arm_wakeup(hal) icu_ll_disable_irda_arm_wakeup((hal)->hw)

#define icu_hal_enable_i2s_arm_wakeup(hal) icu_ll_enable_i2s_arm_wakeup((hal)->hw)
#define icu_hal_disable_i2s_arm_wakeup(hal) icu_ll_disable_i2s_arm_wakeup((hal)->hw)

#define icu_hal_enable_spi_arm_wakeup(hal, id) icu_ll_enable_spi_arm_wakeup((hal)->hw, id)
#define icu_hal_disable_spi_arm_wakeup(hal, id) icu_ll_disable_spi_arm_wakeup((hal)->hw, id)

#define icu_hal_enable_gpio_arm_wakeup(hal) icu_ll_enable_gpio_arm_wakeup((hal)->hw)
#define icu_hal_disable_gpio_arm_wakeup(hal) icu_ll_disable_gpio_arm_wakeup((hal)->hw)

#define icu_hal_enable_timer_arm_wakeup(hal) icu_ll_enable_timer_arm_wakeup((hal)->hw)
#define icu_hal_disable_timer_arm_wakeup(hal) icu_ll_disable_timer_arm_wakeup((hal)->hw)

#define icu_hal_enable_pwm_arm_wakeup(hal) icu_ll_enable_pwm_arm_wakeup((hal)->hw)
#define icu_hal_disable_pwm_arm_wakeup(hal) icu_ll_disable_pwm_arm_wakeup((hal)->hw)


#define icu_hal_enable_audio_arm_wakeup(hal) icu_ll_enable_audio_arm_wakeup((hal)->hw)
#define icu_hal_disable_audio_arm_wakeup(hal) icu_ll_disable_audio_arm_wakeup((hal)->hw)

#define icu_hal_enable_saradc_arm_wakeup(hal) icu_ll_enable_saradc_arm_wakeup((hal)->hw)
#define icu_hal_disable_saradc_arm_wakeup(hal) icu_ll_disable_saradc_arm_wakeup((hal)->hw)

#define icu_hal_enable_sdio_arm_wakeup(hal) icu_ll_enable_sdio_arm_wakeup((hal)->hw)
#define icu_hal_disable_sdio_arm_wakeup(hal) icu_ll_disable_sdio_arm_wakeup((hal)->hw)

#define icu_hal_enable_usb_arm_wakeup(hal, id) icu_ll_enable_usb_arm_wakeup((hal)->hw, id)
#define icu_hal_disable_usb_arm_wakeup(hal, id) icu_ll_disable_usb_arm_wakeup((hal)->hw. id)

#define icu_hal_enable_fft_arm_wakeup(hal) icu_ll_enable_fft_arm_wakeup((hal)->hw)
#define icu_hal_disable_fft_arm_wakeup(hal) icu_ll_disable_fft_arm_wakeup((hal)->hw)

#define icu_hal_enable_dma_arm_wakeup(hal) icu_ll_enable_dma_arm_wakeup((hal)->hw)
#define icu_hal_disable_dma_arm_wakeup(hal) icu_ll_disable_dma_arm_wakeup((hal)->hw)

#define icu_hal_enable_modem_arm_wakeup(hal) icu_ll_enable_modem_arm_wakeup((hal)->hw)
#define icu_hal_disable_modem_arm_wakeup(hal) icu_ll_disable_modem_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_txrx_timer_arm_wakeup(hal) icu_ll_enable_mac_txrx_timer_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_txrx_timer_arm_wakeup(hal) icu_ll_disable_mac_txrx_timer_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_txrx_misc_arm_wakeup(hal) icu_ll_enable_mac_txrx_misc_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_txrx_misc_arm_wakeup(hal) icu_ll_disable_mac_txrx_misc_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_rx_trigger_arm_wakeup(hal) icu_ll_enable_mac_rx_trigger_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_rx_trigger_arm_wakeup(hal) icu_ll_disable_mac_rx_trigger_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_tx_trigger_arm_wakeup(hal) icu_ll_enable_mac_tx_trigger_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_tx_trigger_arm_wakeup(hal) icu_ll_disable_mac_tx_trigger_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_prot_trigger_arm_wakeup(hal) icu_ll_enable_mac_prot_trigger_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_prot_trigger_arm_wakeup(hal) icu_ll_disable_mac_prot_trigger_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_general_arm_wakeup(hal) icu_ll_enable_mac_general_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_general_arm_wakeup(hal) icu_ll_disable_mac_general_arm_wakeup((hal)->hw)

#define icu_hal_enable_sdio_dma_arm_wakeup(hal) icu_ll_enable_sdio_dma_arm_wakeup((hal)->hw)
#define icu_hal_disable_sdio_dma_arm_wakeup(hal) icu_ll_disable_sdio_dma_arm_wakeup((hal)->hw)

#define icu_hal_enable_usb_plug_arm_wakeup(hal) icu_ll_enable_usb_plug_arm_wakeup((hal)->hw)
#define icu_hal_disable_usb_plug_arm_wakeup(hal) icu_ll_disable_usb_plug_arm_wakeup((hal)->hw)

#define icu_hal_enable_security_arm_wakeup(hal) icu_ll_enable_security_arm_wakeup((hal)->hw)
#define icu_hal_disable_security_arm_wakeup(hal) icu_ll_disable_security_arm_wakeup((hal)->hw)

#define icu_hal_enable_mac_wakeup_arm_wakeup(hal) icu_ll_enable_mac_arm_wakeup((hal)->hw)
#define icu_hal_disable_mac_wakeup_arm_wakeup(hal) icu_ll_disable_mac_arm_wakeup((hal)->hw)

#define icu_hal_enable_hsspi_slave_arm_wakeup(hal) icu_ll_enable_hsspi_slave_arm_wakeup((hal)->hw)
#define icu_hal_disable_hsspi_slave_arm_wakeup(hal) icu_ll_disable_hsspi_slave_arm_wakeup((hal)->hw)

#define icu_hal_enable_jpeg_arm_wakeup(hal) icu_ll_enable_jpeg_arm_wakeup((hal)->hw)
#define icu_hal_disable_jpeg_arm_wakeup(hal) icu_ll_disable_jpeg_arm_wakeup((hal)->hw)

#define icu_hal_enable_ble_arm_wakeup(hal) icu_ll_enable_ble_arm_wakeup((hal)->hw)
#define icu_hal_disable_ble_arm_wakeup(hal) icu_ll_disable_ble_arm_wakeup((hal)->hw)

#define icu_hal_enable_psram_arm_wakeup(hal) icu_ll_enable_psram_arm_wakeup((hal)->hw)
#define icu_hal_disable_psram_arm_wakeup(hal) icu_ll_disable_psram_arm_wakeup((hal)->hw)

#define icu_hal_enable_la_arm_wakeup(hal) icu_ll_enable_la_arm_wakeup((hal)->hw)
#define icu_hal_disable_la_arm_wakeup(hal) icu_ll_disable_la_arm_wakeup((hal)->hw)
#define icu_hal_enable_dpll_unlock_arm_wakeup(hal) icu_ll_enable_pll_unlock_arm_wakeup((hal)->hw)
#define icu_hal_disable_dpll_unlock_arm_wakeup(hal) icu_ll_disable_pll_unlock_arm_wakeup((hal)->hw)
#define icu_hal_enable_btdm_arm_wakeup(hal) icu_ll_enable_btdm_arm_wakeup((hal)->hw)
#define icu_hal_disable_btdm_arm_wakeup(hal) icu_ll_disable_btdm_arm_wakeup((hal)->hw)
#define icu_hal_enable_bt_arm_wakeup(hal) icu_ll_enable_bt_arm_wakeup((hal)->hw)
#define icu_hal_disable_bt_arm_wakeup(hal) icu_ll_disable_bt_arm_wakeup((hal)->hw)

#define icu_hal_enable_mailbox_dsp_arm_wakeup(hal) icu_ll_enable_mailbox_dsp_arm_wakeup((hal)->hw)
#define icu_hal_disable_mailbox_dsp_arm_wakeup(hal) icu_ll_disable_mailbox_dsp_arm_wakeup((hal)->hw)
#define icu_hal_enable_mailbox_bt_arm_wakeup(hal) icu_ll_enable_mailbox_bt_arm_wakeup((hal)->hw)
#define icu_hal_disable_mailbox_bt_arm_wakeup(hal) icu_ll_disable_mailbox_bt_arm_wakeup((hal)->hw)
#define icu_hal_enable_cec_arm_wakeup(hal) icu_ll_enable_cec_arm_wakeup((hal)->hw)
#define icu_hal_disable_cec_arm_wakeup(hal) icu_ll_disable_cec_arm_wakeup((hal)->hw)
#define icu_hal_enable_touch_arm_wakeup(hal) icu_ll_enable_touch_arm_wakeup((hal)->hw)
#define icu_hal_disable_touch_arm_wakeup(hal) icu_ll_disable_touch_arm_wakeup((hal)->hw)
#define icu_hal_enable_rtc_arm_wakeup(hal) icu_ll_enable_rtc_arm_wakeup((hal)->hw)
#define icu_hal_disable_rtc_arm_wakeup(hal) icu_ll_disable_rtc_arm_wakeup((hal)->hw)
#define icu_hal_enable_dsp_wdt_arm_wakeup(hal) icu_ll_enable_dsp_wdt_arm_wakeup((hal)->hw)
#define icu_hal_disable_dsp_wdt_arm_wakeup(hal) icu_ll_disable_dsp_wdt_arm_wakeup((hal)->hw)
#define icu_hal_enable_bt_wdt_arm_wakeup(hal) icu_ll_enable_bt_wdt_arm_wakeup((hal)->hw)
#define icu_hal_disable_bt_wdt_arm_wakeup(hal) icu_ll_disable_bt_wdt_arm_wakeup((hal)->hw)

#if (CONFIG_SOC_BK7271)
#define icu_hal_set_gpio_perial_mode(hal, index, mode) icu_ll_set_gpio_perial_mode((hal)->hw, index, mode)
#define icu_hal_get_gpio_perial_mode(hal, index) icu_ll_get_gpio_perial_mode((hal)->hw, index)
#define icu_hal_uart2_gpio_sel(hal, mode) icu_ll_uart2_gpio_sel((hal)->hw,mode)
#define icu_hal_spi3_gpio_sel(hal, mode) icu_ll_spi3_gpio_sel((hal)->hw,mode)
#define icu_hal_sdio_gpio_sel(hal, mode) icu_ll_sdio_gpio_sel((hal)->hw,mode)
#define icu_hal_sdio_host_enable(hal) icu_ll_sdio_host_enable((hal)->hw)
#define icu_hal_sdio_dma_enable(hal) icu_ll_sdio_dma_enable((hal)->hw)
#define icu_hal_pwms_gpio_sel(hal, channel, mode) icu_ll_pwms_gpio_sel((hal)->hw, channel, mode)
#define icu_hal_spdif_gpio_sel(hal, mode) icu_ll_spdif_gpio_sel((hal)->hw,mode)
#endif

#define icu_hal_set_jtag_wr_arm(hal)	icu_ll_set_jtag_wr_arm((hal)->hw)
#define icu_hal_set_jtag_rd_arm(hal)	icu_ll_set_jtag_rd_arm((hal)->hw)

#define icu_hal_set_jtag_wr_tl410(hal)	icu_ll_set_jtag_wr_tl410((hal)->hw)
#define icu_hal_set_jtag_rd_tl410(hal)	icu_ll_set_jtag_rd_tl410((hal)->hw)

#if CONFIG_SOC_BK7236A
#define icu_hal_enable_modem_rc_interrupt(hal) icu_ll_enable_modem_rc_interrupt((hal)->hw)
#define icu_hal_disable_modem_rc_interrupt(hal) icu_ll_disable_modem_rc_interrupt((hal)->hw)
#define icu_hal_enable_mac_hsu_interrupt(hal) icu_ll_enable_mac_hsu_interrupt((hal)->hw)
#define icu_hal_disable_mac_hsu_interrupt(hal) icu_ll_disable_mac_hsu_interrupt((hal)->hw)
#endif

/**< wdt */
#define icu_hal_pwr_up_wdt(hal) icu_ll_pwr_up_wdt((hal)->hw)
#define icu_hal_pwr_down_wdt(hal) icu_ll_pwr_down_wdt((hal)->hw)

#if CFG_HAL_DEBUG_ICU
void icu_struct_dump(void);
#else
#define icu_struct_dump()
#endif

#ifdef __cplusplus
}
#endif
