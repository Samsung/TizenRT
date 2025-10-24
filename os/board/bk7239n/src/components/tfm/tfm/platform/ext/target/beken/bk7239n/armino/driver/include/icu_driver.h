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

#include <common/bk_generic.h>
#include <components/log.h>
#include "icu_hal.h"

typedef struct {
	icu_hal_t hal;
} icu_driver_t;

#define ICU_TAG "icu"
#define ICU_LOGI(...) BK_LOGI(ICU_TAG, ##__VA_ARGS__)
#define ICU_LOGW(...) BK_LOGW(ICU_TAG, ##__VA_ARGS__)
#define ICU_LOGE(...) BK_LOGE(ICU_TAG, ##__VA_ARGS__)
#define ICU_LOGD(...) BK_LOGD(ICU_TAG, ##__VA_ARGS__)

extern icu_driver_t s_icu;

/**< global intterrupt  APIs */
#define icu_enable_fiq()	icu_hal_enable_fiq(&s_icu.hal)
#define icu_disable_fiq()	icu_hal_disable_fiq(&s_icu.hal)
#define icu_enable_irq()	icu_hal_enable_irq(&s_icu.hal)
#define icu_disable_irq()	icu_hal_disable_irq(&s_icu.hal)

bk_err_t icu_enable_global_interrupt(void);
bk_err_t icu_disable_global_interrupt(void);


uint32_t icu_get_global_int_status(void);
uint32_t icu_get_irq_int_status(void);
uint32_t icu_get_fiq_int_status(void);
uint32_t icu_get_uart_int_status(void);

#define icu_clear_global_int_status(status)	icu_hal_clear_global_int_status(&s_icu.hal, status)
#define icu_clear_irq_int_status(status)	icu_hal_clear_irq_int_status(&s_icu.hal, status)
#define icu_clear_fiq_int_status(status)	icu_hal_clear_fiq_int_status(&s_icu.hal, status)
#define icu_disable_all_interrupt()		icu_hal_disable_all_interrupt(&s_icu.hal);
#define icu_enable_interrupt(index)		icu_hal_enable_interrupt(&s_icu.hal, index);
#define icu_disable_interrupt(index)		icu_hal_disable_interrupt(&s_icu.hal, index);


/**< jtag select APIs */
#define icu_set_jtag_wr_arm()		icu_hal_set_jtag_wr_arm(&s_icu.hal)
#define icu_set_jtag_rd_arm()		icu_hal_set_jtag_rd_arm(&s_icu.hal)

#define icu_set_jtag_wr_tl410()		icu_hal_set_jtag_wr_tl410(&s_icu.hal)
#define icu_set_jtag_rd_tl410()		icu_hal_set_jtag_rd_tl410(&s_icu.hal)


/**< uart related APIs */
#define icu_enable_uart_interrupt(id)		icu_hal_enable_uart_interrupt(&s_icu.hal, id)
#define icu_disable_uart_interrupt(id)		icu_hal_disable_uart_interrupt(&s_icu.hal, id)
#define icu_enable_uart_arm_wakeup(id)		icu_hal_enable_uart_arm_wakeup(&s_icu.hal, id)
#define icu_disable_uart_arm_wakeup(id)		icu_hal_disable_uart_arm_wakeup(&s_icu.hal, id)

/**< i2c1 related APIs */
#define icu_enable_i2c_interrupt(id)	icu_hal_enable_i2c_interrupt(&s_icu.hal, id)
#define icu_disable_i2c_interrupt(id)	icu_hal_disable_i2c_interrupt(&s_icu.hal, id)
#define icu_enable_i2c_arm_wakeup(id)	icu_hal_enable_i2c_arm_wakeup(&s_icu.hal, id)
#define icu_disable_i2c_arm_wakeup(id)	icu_hal_disable_i2c_arm_wakeup(&s_icu.hal, id)

/**< irda related APIs */
#define icu_enable_irda_interrupt()	icu_hal_enable_irda_interrupt(&s_icu.hal)
#define icu_disable_irda_interrupt()	icu_hal_disable_irda_interrupt(&s_icu.hal)
#define icu_enable_irda_arm_wakeup()	icu_hal_enable_irda_arm_wakeup(&s_icu.hal)
#define icu_disable_irda_arm_wakeup()	icu_hal_disable_irda_arm_wakeup(&s_icu.hal)

/**< i2s related APIs */
#define icu_enable_i2s_interrupt()	icu_hal_enable_i2s_interrupt(&s_icu.hal)
#define icu_disable_i2s_interrupt()	icu_hal_disable_i2s_interrupt(&s_icu.hal)
#define icu_enable_i2s_arm_wakeup()	icu_hal_enable_i2s_arm_wakeup(&s_icu.hal)
#define icu_disable_i2s_arm_wakeup()	icu_hal_disable_i2s_arm_wakeup(&s_icu.hal)


/**< spi related APIs */

#define icu_enable_spi_interrupt(id)	icu_hal_enable_spi_interrupt(&s_icu.hal, id)
#define icu_disable_spi_interrupt(id)	icu_hal_disable_spi_interrupt(&s_icu.hal, id)
#define icu_enable_spi_arm_wakeup()	icu_hal_enable_spi_arm_wakeup(&s_icu.hal)
#define icu_disable_spi_arm_wakeup()	icu_hal_disable_spi_arm_wakeup(&s_icu.hal)

/**< saradc related APIs */

#define icu_enable_saradc_interrupt()	icu_hal_enable_saradc_interrupt(&s_icu.hal)
#define icu_disable_saradc_interrupt()	icu_hal_disable_saradc_interrupt(&s_icu.hal)
#define icu_enable_saradc_arm_wakeup()	icu_hal_enable_saradc_arm_wakeup(&s_icu.hal)
#define icu_disable_saradc_arm_wakeup()	icu_hal_disable_saradc_arm_wakeup(&s_icu.hal)


/**< pwm related APIs */

#define icu_enable_pwm_interrupt()	icu_hal_enable_pwm_interrupt(&s_icu.hal)
#define icu_disable_pwm_interrupt()	icu_hal_disable_pwm_interrupt(&s_icu.hal)
#define icu_enable_pwm2_interrupt()	icu_hal_enable_pwm2_interrupt(&s_icu.hal)
#define icu_disable_pwm2_interrupt()	icu_hal_disable_pwm2_interrupt(&s_icu.hal)
bool icu_is_pwm_has_two_int(void);
uint32_t icu_get_pwm_2nd_int_num(void);
uint32_t icu_get_pwm_2nd_int_prio(void);


/**< timer related APIs */
#define icu_enable_timer_interrupt()	icu_hal_enable_timer_interrupt(&s_icu.hal)
#define icu_disable_timer_interrupt()	icu_hal_disable_timer_interrupt(&s_icu.hal)
#define icu_enable_timer_arm_wakeup()	icu_hal_enable_timer_arm_wakeup(&s_icu.hal)
#define icu_disable_timer_arm_wakeup()	icu_hal_disable_timer_arm_wakeup(&s_icu.hal)



/**< gpio related APIs */
#define icu_enable_gpio_interrupt()	icu_hal_enable_gpio_interrupt(&s_icu.hal)
#define icu_disable_gpio_interrupt()	icu_hal_disable_gpio_interrupt(&s_icu.hal)
#define icu_enable_gpio_arm_wakeup()	icu_hal_enable_gpio_arm_wakeup(&s_icu.hal)
#define icu_disable_gpio_arm_wakeup()	icu_hal_disable_gpio_arm_wakeup(&s_icu.hal)



/**< sdio related APIs */
#define icu_enable_sdio_interrupt()	icu_hal_enable_sdio_interrupt(&s_icu.hal)
#define icu_disable_sdio_interrupt()	icu_hal_disable_sdio_interrupt(&s_icu.hal)
#define icu_enable_sdio_arm_wakeup()	icu_hal_enable_sdio_arm_wakeup(&s_icu.hal)
#define icu_disable_sdio_arm_wakeup()	icu_hal_disable_sdio_arm_wakeup(&s_icu.hal)




/**< audio related APIs */
#define icu_enable_audio_interrupt()	icu_hal_enable_audio_interrupt(&s_icu.hal)
#define icu_disable_audio_interrupt()	icu_hal_disable_audio_interrupt(&s_icu.hal)
#define icu_enable_audio_arm_wakeup()	icu_hal_enable_audio_arm_wakeup(&s_icu.hal)
#define icu_disable_audio_arm_wakeup()	icu_hal_disable_audio_arm_wakeup(&s_icu.hal)




/**< usb related APIs */
#define icu_enable_usb_interrupt(id)	icu_hal_enable_usb_interrupt(&s_icu.hal, id)
#define icu_disable_usb_interrupt(id)	icu_hal_disable_usb_interrupt(&s_icu.hal, id)
#define icu_enable_usb_arm_wakeup(id)	icu_hal_enable_usb_arm_wakeup(&s_icu.hal, id)
#define icu_disable_usb_arm_wakeup(id)	icu_hal_disable_usb_arm_wakeup(&s_icu.hal, id)


/**< fft related APIs */
#define icu_enable_fft_interrupt()	icu_hal_enable_fft_interrupt(&s_icu.hal)
#define icu_disable_fft_interrupt()	icu_hal_disable_fft_interrupt(&s_icu.hal)
#define icu_enable_fft_arm_wakeup()	icu_hal_enable_fft_arm_wakeup(&s_icu.hal)
#define icu_disable_fft_arm_wakeup()	icu_hal_disable_fft_arm_wakeup(&s_icu.hal)



/**< dma related APIs */
#define icu_enable_dma_interrupt()	icu_hal_enable_dma_interrupt(&s_icu.hal)
#define icu_disable_dma_interrupt()	icu_hal_disable_dma_interrupt(&s_icu.hal)
#define icu_enable_dma_arm_wakeup()	icu_hal_enable_dma_arm_wakeup(&s_icu.hal)
#define icu_disable_dma_arm_wakeup()	icu_hal_disable_dma_arm_wakeup(&s_icu.hal)



/**< jpeg related APIs */

#define icu_enable_jpeg_interrupt()	icu_hal_enable_jpeg_interrupt(&s_icu.hal)
#define icu_disable_jpeg_interrupt()	icu_hal_disable_jpeg_interrupt(&s_icu.hal)
#define icu_enable_jpeg_arm_wakeup()	icu_hal_enable_jpeg_arm_wakeup(&s_icu.hal)
#define icu_disable_jpeg_arm_wakeup()	icu_hal_disable_jpeg_arm_wakeup(&s_icu.hal)




/**< qspi related APIs */

#define icu_enable_qspi_interrupt()	icu_hal_enable_psram_interrupt(&s_icu.hal)
#define icu_disable_qspi_interrupt()	icu_hal_disable_psram_interrupt(&s_icu.hal)
#define icu_enable_qspi_arm_wakeup()	icu_hal_enable_psram_arm_wakeup(&s_icu.hal)
#define icu_disable_qspi_arm_wakeup()	icu_hal_disable_psram_arm_wakeup(&s_icu.hal)



/**< modem related APIs */
#define icu_enable_modem_interrupt()	icu_hal_enable_modem_interrupt(&s_icu.hal)
#define icu_disable_modem_interrupt()	icu_hal_disable_modem_interrupt(&s_icu.hal)
#define icu_enable_modem_arm_wakeup()	icu_hal_enable_modem_arm_wakeup(&s_icu.hal)
#define icu_disable_modem_arm_wakeup()	icu_hal_disable_modem_arm_wakeup(&s_icu.hal)


/**< mac_txrx_timer related APIs */
#define icu_enable_mac_txrx_timer_interrupt()	icu_hal_enable_mac_txrx_timer_interrupt(&s_icu.hal)
#define icu_disable_mac_txrx_timer_interrupt()	icu_hal_disable_mac_txrx_timer_interrupt(&s_icu.hal)
#define icu_enable_mac_txrx_timer_arm_wakeup()	icu_hal_enable_mac_txrx_timer_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_txrx_timer_arm_wakeup()	icu_hal_disable_mac_txrx_timer_arm_wakeup(&s_icu.hal)



/**< mac_txrx_misc related APIs */
#define icu_enable_mac_txrx_misc_interrupt()	icu_hal_enable_mac_txrx_misc_interrupt(&s_icu.hal)
#define icu_disable_mac_txrx_misc_interrupt()	icu_hal_disable_mac_txrx_misc_interrupt(&s_icu.hal)
#define icu_enable_mac_txrx_misc_arm_wakeup()	icu_hal_enable_mac_txrx_misc_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_txrx_misc_arm_wakeup()	icu_hal_disable_mac_txrx_misc_arm_wakeup(&s_icu.hal)


/**< mac_rx_trigger related APIs */
#define icu_enable_mac_rx_trigger_interrupt()	icu_hal_enable_mac_rx_trigger_interrupt(&s_icu.hal)
#define icu_disable_mac_rx_trigger_interrupt()	icu_hal_disable_mac_rx_trigger_interrupt(&s_icu.hal)
#define icu_enable_mac_rx_trigger_arm_wakeup()	icu_hal_enable_mac_rx_trigger_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_rx_trigger_arm_wakeup()	icu_hal_disable_mac_rx_trigger_arm_wakeup(&s_icu.hal)



/**< mac_tx_trigger related APIs */
#define icu_enable_mac_tx_trigger_interrupt()	icu_hal_enable_mac_tx_trigger_interrupt(&s_icu.hal)
#define icu_disable_mac_tx_trigger_interrupt()	icu_hal_disable_mac_tx_trigger_interrupt(&s_icu.hal)
#define icu_enable_mac_tx_trigger_arm_wakeup()	icu_hal_enable_mac_tx_trigger_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_tx_trigger_arm_wakeup()	icu_hal_disable_mac_tx_trigger_arm_wakeup(&s_icu.hal)



/**< mac_prot_trigger related APIs */
#define icu_enable_mac_prot_trigger_interrupt()		icu_hal_enable_mac_prot_trigger_interrupt(&s_icu.hal)
#define icu_disable_mac_prot_trigger_interrupt()	icu_hal_disable_mac_prot_trigger_interrupt(&s_icu.hal)
#define icu_enable_mac_prot_trigger_arm_wakeup()	icu_hal_enable_mac_prot_trigger_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_prot_trigger_arm_wakeup()	icu_hal_disable_mac_prot_trigger_arm_wakeup(&s_icu.hal)



/**< mac_general related APIs */
#define icu_enable_mac_general_interrupt()	icu_hal_enable_mac_general_interrupt(&s_icu.hal)
#define icu_disable_mac_general_interrupt()	icu_hal_disable_mac_general_interrupt(&s_icu.hal)
#define icu_enable_mac_general_arm_wakeup()	icu_hal_enable_mac_general_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_general_arm_wakeup()	icu_hal_disable_mac_general_arm_wakeup(&s_icu.hal)


/**< sdio_dma related APIs */
#define icu_enable_sdio_dma_interrupt()		icu_hal_enable_sdio_dma_interrupt(&s_icu.hal)
#define icu_disable_sdio_dma_interrupt()	icu_hal_disable_sdio_dma_interrupt(&s_icu.hal)
#define icu_enable_sdio_dma_arm_wakeup()	icu_hal_enable_sdio_dma_arm_wakeup(&s_icu.hal)
#define icu_disable_sdio_dma_arm_wakeup()	icu_hal_disable_sdio_dma_arm_wakeup(&s_icu.hal)


/**< usb_plug_inout related APIs */
#define icu_enable_usb_plug_inout_interrupt()	icu_hal_enable_usb_plug_interrupt(&s_icu.hal)
#define icu_disable_usb_plug_inout_interrupt()	icu_hal_disable_usb_plug_interrupt(&s_icu.hal)
#define icu_enable_usb_plug_inout_arm_wakeup()	icu_hal_enable_usb_plug_arm_wakeup(&s_icu.hal)
#define icu_disable_usb_plug_inout_arm_wakeup()	icu_hal_disable_usb_plug_arm_wakeup(&s_icu.hal)



/**< security related APIs */
#define icu_enable_security_interrupt()		icu_hal_enable_security_interrupt(&s_icu.hal)
#define icu_disable_security_interrupt()	icu_hal_disable_security_interrupt(&s_icu.hal)
#define icu_enable_security_arm_wakeup()	icu_hal_enable_security_arm_wakeup(&s_icu.hal)
#define icu_disable_security_arm_wakeup()	icu_hal_disable_security_arm_wakeup(&s_icu.hal)



/**< mac_wakeup related APIs */
#define icu_enable_mac_wakeup_interrupt()	icu_hal_enable_mac_wakeup_interrupt(&s_icu.hal)
#define icu_disable_mac_wakeup_interrupt()	icu_hal_disable_mac_wakeup_interrupt(&s_icu.hal)
#define icu_enable_mac_wakeup_arm_wakeup()	icu_hal_enable_mac_wakeup_arm_wakeup(&s_icu.hal)
#define icu_disable_mac_wakeup_arm_wakeup()	icu_hal_disable_mac_wakeup_arm_wakeup(&s_icu.hal)


/**< hsspi_slave related APIs */
#define icu_enable_hsspi_slave_interrupt()	icu_hal_enable_hsspi_slave_interrupt(&s_icu.hal)
#define icu_disable_hsspi_slave_interrupt()	icu_hal_disable_hsspi_slave_interrupt(&s_icu.hal)
#define icu_enable_hsspi_slave_arm_wakeup()	icu_hal_enable_hsspi_slave_arm_wakeup(&s_icu.hal)
#define icu_disable_hsspi_slave_arm_wakeup()	icu_hal_disable_hsspi_slave_arm_wakeup(&s_icu.hal)



/**< dpll_unlock related APIs */
#define icu_enable_dpll_unlock_interrupt()	icu_hal_enable_dpll_unlock_interrupt(&s_icu.hal)
#define icu_disable_dpll_unlock_interrupt()	icu_hal_disable_dpll_unlock_interrupt(&s_icu.hal)
#define icu_enable_dpll_unlock_arm_wakeup()	icu_hal_enable_dpll_unlock_arm_wakeup(&s_icu.hal)
#define icu_disable_dpll_unlock_arm_wakeup()	icu_hal_disable_dpll_unlock_arm_wakeup(&s_icu.hal)

/**< ble related APIs */
#define icu_enable_ble_interrupt()	icu_hal_enable_ble_interrupt(&s_icu.hal)
#define icu_disable_ble_interrupt()	icu_hal_disable_ble_interrupt(&s_icu.hal)
#define icu_enable_ble_arm_wakeup()	icu_hal_enable_ble_arm_wakeup(&s_icu.hal)
#define icu_disable_ble_arm_wakeup()	icu_hal_disable_ble_arm_wakeup(&s_icu.hal)


/**< la related APIs */
#define icu_enable_la_interrupt()	icu_hal_enable_la_interrupt(&s_icu.hal)
#define icu_disable_la_interrupt()	icu_hal_disable_la_interrupt(&s_icu.hal)
#define icu_enable_la_arm_wakeup()	icu_hal_enable_la_arm_wakeup(&s_icu.hal)
#define icu_disable_la_arm_wakeup()	icu_hal_disable_la_arm_wakeup(&s_icu.hal)

/**< btdm related APIs */
#define icu_enable_btdm_interrupt()	icu_hal_enable_btdm_interrupt(&s_icu.hal)
#define icu_disable_btdm_interrupt()	icu_hal_disable_btdm_interrupt(&s_icu.hal)
#define icu_enable_btdm_arm_wakeup()	icu_hal_enable_btdm_arm_wakeup(&s_icu.hal)
#define icu_disable_btdm_arm_wakeup()	icu_hal_disable_btdm_arm_wakeup(&s_icu.hal)

/**< bt related APIs */
#define icu_enable_bt_interrupt()	icu_hal_enable_bt_interrupt(&s_icu.hal)
#define icu_disable_bt_interrupt()	icu_hal_disable_bt_interrupt(&s_icu.hal)
#define icu_enable_bt_arm_wakeup()	icu_hal_enable_bt_arm_wakeup(&s_icu.hal)
#define icu_disable_bt_arm_wakeup()	icu_hal_disable_bt_arm_wakeup(&s_icu.hal)

/**< mailbox_dsp related APIs */
#define icu_enable_mailbox_dsp_interrupt()	icu_hal_enable_mailbox_dsp_interrupt(&s_icu.hal)
#define icu_disable_mailbox_dsp_interrupt()	icu_hal_disable_mailbox_dsp_interrupt(&s_icu.hal)
#define icu_enable_mailbox_dsp_arm_wakeup()	icu_hal_enable_mailbox_dsp_arm_wakeup(&s_icu.hal)
#define icu_disable_mailbox_dsp_arm_wakeup()	icu_hal_disable_mailbox_dsp_arm_wakeup(&s_icu.hal)

/**< mailbox_bt related APIs */
#define icu_enable_mailbox_bt_interrupt()	icu_hal_enable_mailbox_bt_interrupt(&s_icu.hal)
#define icu_disable_mailbox_bt_interrupt()	icu_hal_disable_mailbox_bt_interrupt(&s_icu.hal)
#define icu_enable_mailbox_bt_arm_wakeup()	icu_hal_enable_mailbox_bt_arm_wakeup(&s_icu.hal)
#define icu_disable_mailbox_bt_arm_wakeup()	icu_hal_disable_mailbox_bt_arm_wakeup(&s_icu.hal)

/**< cec related APIs */
#define icu_enable_cec_interrupt()	icu_hal_enable_cec_interrupt(&s_icu.hal)
#define icu_disable_cec_interrupt()	icu_hal_disable_cec_interrupt(&s_icu.hal)
#define icu_enable_cec_arm_wakeup()	icu_hal_enable_cec_arm_wakeup(&s_icu.hal)
#define icu_disable_cec_arm_wakeup()	icu_hal_disable_cec_arm_wakeup(&s_icu.hal)

/**< touch related APIs */
#define icu_enable_touch_interrupt()	icu_hal_enable_touch_interrupt(&s_icu.hal)
#define icu_disable_touch_interrupt()	icu_hal_disable_touch_interrupt(&s_icu.hal)
#define icu_enable_touch_arm_wakeup()	icu_hal_enable_touch_arm_wakeup(&s_icu.hal)
#define icu_disable_touch_arm_wakeup()	icu_hal_disable_touch_arm_wakeup(&s_icu.hal)

/**< rtc related APIs */
#define icu_enable_rtc_interrupt()	icu_hal_enable_rtc_interrupt(&s_icu.hal)
#define icu_disable_rtc_interrupt()	icu_hal_disable_rtc_interrupt(&s_icu.hal)
#define icu_enable_rtc_arm_wakeup()	icu_hal_enable_rtc_arm_wakeup(&s_icu.hal)
#define icu_disable_rtc_arm_wakeup()	icu_hal_disable_rtc_arm_wakeup(&s_icu.hal)

/**< dsp_wdt related APIs */
#define icu_enable_dsp_wdt_interrupt()	icu_hal_enable_dsp_wdt_interrupt(&s_icu.hal)
#define icu_disable_dsp_wdt_interrupt()	icu_hal_disable_dsp_wdt_interrupt(&s_icu.hal)
#define icu_enable_dsp_wdt_arm_wakeup()	icu_hal_enable_dsp_wdt_arm_wakeup(&s_icu.hal)
#define icu_disable_dsp_wdt_arm_wakeup()	icu_hal_disable_dsp_wdt_arm_wakeup(&s_icu.hal)

/**< bt_wdt related APIs */
#define icu_enable_bt_wdt_interrupt()	icu_hal_enable_bt_wdt_interrupt(&s_icu.hal)
#define icu_disable_bt_wdt_interrupt()	icu_hal_disable_bt_wdt_interrupt(&s_icu.hal)
#define icu_enable_bt_wdt_arm_wakeup()	icu_hal_enable_bt_wdt_arm_wakeup(&s_icu.hal)
#define icu_disable_bt_wdt_arm_wakeup()	icu_hal_disable_bt_wdt_arm_wakeup(&s_icu.hal)

#if (CONFIG_SOC_BK7236A)
#define icu_enable_modem_rc_interrupt()	icu_hal_enable_modem_rc_interrupt(&s_icu.hal)
#define icu_disable_modem_rc_interrupt()  icu_hal_disable_modem_rc_interrupt(&s_icu.hal)
#define icu_enable_mac_hsu_interrupt()	icu_hal_enable_mac_hsu_interrupt(&s_icu.hal)
#define icu_disable_mac_hsu_interrupt()  icu_hal_disable_mac_hsu_interrupt(&s_icu.hal)
#endif

#if (CONFIG_SOC_BK7271)
typedef enum {
       ICU_GPIO_SDIO_MAP_MODE0 = 0,		/**<GPIO8~GPIO10  GPIO34~GPIO36 is used for SDIO   HOST/DMA */
       ICU_GPIO_SDIO_MAP_MODE1,			/**<GPIO34~GPIO39 is used for SDIO HOST/DMA */
       ICU_GPIO_SDIO_MAP_MODE_MAX,		/**< Invalid sdio mode */
} icu_gpio_sdio_map_mode_t;


typedef enum {
       ICU_GPIO_SPI_MAP_MODE0 = 0,		/**<GPIO30~GPIO33 is used for SPI module */
       ICU_GPIO_SPI_MAP_MODE1,			/**<GPIO36~GPIO39 is used for SPI module*/
       ICU_GPIO_SPI_MAP_MODE_MAX ,		/**< Invalid spi mode */
} icu_gpio_spi3_map_mode_t;


typedef enum {
       ICU_GPIO_UART2_MAP_MODE0 = 0,		/**<GPIO6~GPIO7 is used for UART2 */
       ICU_GPIO_UART2_MAP_MODE1,		/**<GPIO16~GPIO17 is used for UART2*/
       ICU_GPIO_UART2_MAP_MODE_MAX,		/**< Invalid mode*/
} icu_gpio_uart2_map_mode_t;

typedef enum {
       ICU_GPIO_PWMS_MAP_MODE0 = 0,		/**<GPIO18/19/20/21 is used for PWM6/7/8/9 */
       ICU_GPIO_PWMS_MAP_MODE1,			/**<GPIO26/27/28/29 is used for PWM6/7/8/9 */
       ICU_GPIO_PWMS_MAP_MODE2,			/**<GPIO30/31/32/33 is used for PWM6/7/8/9 */
       ICU_GPIO_PWMS_MAP_MODE_MAX,		/**< Invalid mode*/
} icu_gpio_pwms_map_mode_t;

typedef enum {
       ICU_GPIO_SPDIF_MAP_MODE0 = 0,		/**<GPIO11is used for SPDIF */
       ICU_GPIO_SPDIF_MAP_MODE1,		/**<GPIO12 is used for SPDIF */
       ICU_GPIO_SPDIF_MAP_MODE2,		/**<GPIO13 is used for SPDIF */
       ICU_GPIO_SPDIF_MAP_MODE_MAX,		/**< Invalid mode*/
} icu_gpio_spdif_map_mode_t;

void icu_set_gpio_perial_mode(gpio_id_t id, uint32_t mode);
uint32_t icu_get_gpio_perial_mode(gpio_id_t id);
void icu_uart2_gpio_sel(icu_gpio_uart2_map_mode_t mode);
void icu_spi3_gpio_sel(icu_gpio_spi3_map_mode_t mode);
void icu_sdio_gpio_sel(icu_gpio_sdio_map_mode_t mode);
uint32_t icu_pwms_gpio_sel(uint32 pwm_chan,icu_gpio_pwms_map_mode_t mode);
void icu_sdio_host_enable(void);
void icu_sdio_dma_enable(void);
#endif



