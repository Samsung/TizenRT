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

#define ICU_FAILURE                (1)
#define ICU_SUCCESS                (0)

#define ICU_DEV_NAME                "icu"

#define ICU_CMD_MAGIC              (0xe220000)
enum
{
    CMD_ICU_CLKGATING_DISABLE = ICU_CMD_MAGIC + 1,
    CMD_ICU_CLKGATING_ENABLE,
    CMD_ICU_INT_DISABLE,
    CMD_ICU_INT_ENABLE,
	CMD_ICU_FIQ_DISABLE,
    CMD_ICU_FIQ_ENABLE,
    CMD_ICU_GLOBAL_INT_DISABLE,
    CMD_ICU_GLOBAL_INT_ENABLE,
    CMD_GET_INTR_STATUS,
    CMD_CLR_INTR_STATUS,
	CMD_GET_FIQ_REG_STATUS,
    CMD_CLR_FIQ_REG_STATUS,
    CMD_GET_INTR_RAW_STATUS,
    CMD_CLR_INTR_RAW_STATUS,
    CMD_CLK_PWR_DOWN,
    CMD_CLK_PWR_UP,
	CMD_FUNC_CLK_PWR_DOWN,
    CMD_FUNC_CLK_PWR_UP,
    CMD_CONF_PWM_PCLK,
    CMD_CONF_PWM_LPOCLK,
    CMD_TL410_CLK_PWR_DOWN,
    CMD_TL410_CLK_PWR_UP,
    CMD_CONF_PCLK_26M,
    CMD_CONF_PCLK_DCO,
    CMD_SET_JTAG_MODE,
    CMD_GET_JTAG_MODE,
    CMD_ARM_WAKEUP_DISABLE,
    CMD_ARM_WAKEUP_ENABLE,
    CMD_GET_ARM_WAKEUP,
    CMD_QSPI_CLK_SEL
};

/*CMD_CONF_PCLK*/
#define PCLK_POSI                            (0)
#if (CONFIG_SOC_BK7271)
#define PCLK_POSI_UART1                      (1 << 0)
#define PCLK_POSI_UART2                      (1 << 1)
#define PCLK_POSI_UART3                      (1 << 2)
#define PCLK_POSI_IRDA                       (1 << 3)
#define PCLK_POSI_FM_I2C                     (1 << 4)
#define PCLK_POSI_I2C1                       (1 << 5)
#define PCLK_POSI_I2C2                       (1 << 6)
#define PCLK_POSI_SPI1                       (1 << 7)
#define PCLK_POSI_SPI2                       (1 << 8)
#define PCLK_POSI_SPI3                       (1 << 9)
#define PCLK_POSI_PWMS                       (1 << 10)
#define PCLK_POSI_SDIO                       (1 << 11)
#define PCLK_POSI_EFUSE                      (1 << 12)
#define PCLK_POSI_CEC                        (1 << 13)
#else
#define PCLK_POSI_UART1                      (1 << 0)
#define PCLK_POSI_UART2                      (1 << 1)
#define PCLK_POSI_I2C1                       (1 << 2)
#define PCLK_POSI_IRDA                       (1 << 3)
#define PCLK_POSI_I2C2                       (1 << 4)
#define PCLK_POSI_SARADC                     (1 << 5)
#define PCLK_POSI_SPI                        (1 << 6)
#define PCLK_POSI_PWMS                       (1 << 7)
#define PCLK_POSI_SDIO                       (1 << 8)
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236XX)
#define PCLK_POSI_EFUSE                      (1 << 9)
#else
#define PCLK_POSI_SARADC_AUD                 (1 << 9)
#endif
#endif

#define PCLK_POSI_QSPI_DCO				 	 (0 << 16)
#define PCLK_POSI_QSPI_26M				 	 (1 << 16)
#define PCLK_POSI_QSPI_120M				 	 (2 << 16)

/*CMD_CONF_PWM_PCLK, CMD_CONF_PWM_LPOCLK*/
#define PWM_MUX_POSI                                (0)
#define PWM_MUX_LPO                                 (1)
#define PWM_MUX_PCLK                                (0)
// *param = channel_id

/*CMD_CLK_PWR_DOWN CMD_CLK_PWR_UP*/
#define PWD_QSPI_CLK_BIT					 (1 << 23)

#if (CONFIG_SOC_BK7271)
#define PWD_CEC_CLK_BIT                      (1 << 19)
#define PWD_SARADC_CLK_BIT                   (1 << 18)
#define PWD_PWM3_CLK_BIT                     (1 << 17)
#define PWD_PWM2_CLK_BIT                     (1 << 16)
#define PWD_PWM1_CLK_BIT                     (1 << 15)
#define PWD_PWM0_CLK_BIT                     (1 << 14)
#define PWD_TIMER_32K_CLK_BIT                (1 << 13)
#define PWD_TIMER_26M_CLK_BIT                (1 << 12)
#define PWD_IRDA_CLK_BIT                     (1 << 11)
#define PWD_SPI3_CLK_BIT                     (1 << 10)
#define PWD_SPI2_CLK_BIT                     (1 <<  9)
#define PWD_SPI1_CLK_BIT                     (1 <<  8)
#define PWD_SDIO_CLK_BIT                     (1 <<  6)
#define PWD_I2C2_CLK_BIT                     (1 <<  5)
#define PWD_I2C1_CLK_BIT                     (1 <<  4)
#define PWD_FM_I2C_CLK_BIT                   (1 <<  3)
#define PWD_UART3_CLK_BIT                    (1 <<  2)
#define PWD_UART2_CLK_BIT                    (1 <<  1)
#define PWD_UART1_CLK_BIT                    (1 <<  0)

#define PWD_JEPG_CLK_BIT                     (1 << 3)
#define PWD_ARM_WATCHDOG_CLK_BIT             (1 << 0)
#else
#define PWD_JEPG_CLK_BIT                     (1 << 22)
#if (!CONFIG_SOC_BK7231)
#define PWD_TIMER_32K_CLK_BIT                (1 << 21)
#define PWD_TIMER_26M_CLK_BIT                (1 << 20)
#endif

#define PWD_QSPI_CLK_BIT					 (1 << 23)
#define PWD_FFT_CLK_BIT                      (1 << 19)
#define PWD_USB_CLK_BIT                      (1 << 18)
#define PWD_SDIO_CLK_BIT                     (1 << 17)
#define PWD_TL410_WATCHDOG_BIT               (1 << 16)
#define PWD_AUDIO_CLK_BIT                    (1 << 15)
#define PWD_PWM5_CLK_BIT                     (1 << 14)
#define PWD_PWM4_CLK_BIT                     (1 << 13)
#define PWD_PWM3_CLK_BIT                     (1 << 12)
#define PWD_PWM2_CLK_BIT                     (1 << 11)
#define PWD_PWM1_CLK_BIT                     (1 << 10)
#define PWD_PWM0_CLK_BIT                     (1 <<  9)
#define PWD_ARM_WATCHDOG_CLK_BIT             (1 <<  8)
#define PWD_SARADC_CLK_BIT                   (1 <<  7)
#define PWD_SPI_CLK_BIT                      (1 <<  6)
#define PWD_I2C2_CLK_BIT                     (1 <<  5)
#define PWD_I2S_PCM_CLK_BIT                  (1 <<  4)
#define PWD_IRDA_CLK_BIT                     (1 <<  3)
#define PWD_I2C1_CLK_BIT                     (1 <<  2)
#define PWD_UART2_CLK_BIT                    (1 <<  1)
#define PWD_UART1_CLK_BIT                    (1 <<  0)
#endif

/* CMD_ICU_CLKGATING_DISABLE CMD_ICU_CLKGATING_ENABLE*/
#define CLKGATE_MAC_AHB_BIT                      (1 << 16)
#define CLKGATE_FFT_AHB_BIT                      (1 << 15)
#define CLKGATE_USB_AHB_BIT                      (1 << 14)
#define CLKGATE_SDIO_AHB_BIT                     (1 << 13)
#define CLKGATE_SARADC_APB_BIT                   (1 << 12)
#define CLKGATE_AUDIO_APB_BIT                    (1 << 11)
#define CLKGATE_PWM_APB_BIT                      (1 << 10)
#define CLKGATE_WATCHDOG_APB_BIT                 (1 << 9)
#define CLKGATE_GPIO_APB_BIT                     (1 << 8)
#define CLKGATE_SPI_APB_BIT                      (1 << 7)
#define CLKGATE_I2C2_APB_BIT                     (1 << 6)
#define CLKGATE_I2S_PCM_APB_BIT                  (1 << 5)
#define CLKGATE_IRDA_APB_BIT                     (1 << 4)
#define CLKGATE_I2C1_APB_BIT                     (1 << 3)
#define CLKGATE_UART2_APB_BIT                    (1 << 2)
#define CLKGATE_UART1_APB_BIT                    (1 << 1)
#define CLKGATE_ICU_APB_BIT                      (1 << 0)

/* CMD ICU_TL410_CLK_PWD*/
#define PWD_TL410_CLK_BIT                    (1 <<  0)
#define PWD_BLE_CLK_BIT                      (1 <<  1)

/* ICU_R_JTAG_SELECT */
#define JTAG_ARM_MODE                            0
#if (CONFIG_SOC_BK7271)
#define JTAG_BT_MODE                             1
#define JTAG_TL410_MODE                          2
#else
#define JTAG_TL410_MODE                          1
#endif

#if (CONFIG_SOC_BK7271)
#define FIQ_MAC_WAKEUP_BIT                        (1 << 31) /* IRQ actually, compatible with legacy name */
#define FIQ_MAC_GENERAL_BIT                       (1 << 30)
#define FIQ_MAC_PROT_TRIGGER_BIT                  (1 << 29)
#define FIQ_MAC_TX_TRIGGER_BIT                    (1 << 28)
#define FIQ_MAC_RX_TRIGGER_BIT                    (1 << 27)
#define FIQ_MAC_TX_RX_MISC_BIT                    (1 << 26)
#define FIQ_MAC_TX_RX_TIMER_BIT                   (1 << 25)
#define FIQ_MODEM_BIT                             (1 << 24)
#define FIQ_SECURITY_BIT                          (1 << 23) /* IRQ actually, compatible with legacy name */
#define IRQ_MAILBOX2_BIT                          (1 << 22)
#define IRQ_MAILBOX1_BIT                          (1 << 21)
#define IRQ_GDMA_BIT                              (1 << 20)
#define FIQ_SDIO_DMA_BIT                          (1 << 19) /* IRQ actually, compatible with legacy name */
#define IRQ_USB2_BIT                              (1 << 18)
#define IRQ_QSPI_BIT                              (1 << 17)
#define IRQ_SARADC_BIT                            (1 << 16)
#define IRQ_PWM2_BIT                              (1 << 15)
#define IRQ_PWM_BIT                               (1 << 14)
#define IRQ_TIMER_BIT                             (1 << 13)
#define IRQ_IRDA_BIT                              (1 << 12)
#define IRQ_GPIO_BIT                              (1 << 11)
#define IRQ_SPI3_BIT                              (1 << 10)
#define IRQ_SPI2_BIT                              (1 << 9)
#define IRQ_SPI1_BIT                              (1 << 8)
#define IRQ_USB1_BIT                              (1 << 7)
#define IRQ_SDIO_BIT                              (1 << 6)
#define IRQ_I2C2_BIT                              (1 << 5)
#define IRQ_I2C1_BIT                              (1 << 4)
#define IRQ_FM_I2C_BIT                            (1 << 3)
#define IRQ_UART3_BIT                             (1 << 2)
#define IRQ_UART2_BIT                             (1 << 1)
#define IRQ_UART1_BIT                             (1 << 0)

#define FIQ_JPEG_DECODER_BIT                      (1 << 7)
#define FIQ_CEC_BIT                               (1 << 6)
#define FIQ_TOUCH_BIT                             (1 << 5)
#define FIQ_RTC_BIT                               (1 << 4)
#define FIQ_DSP_WATCHDOG_BIT                      (1 << 3)
#define FIQ_BT_WATCHDOG_BIT                       (1 << 2)
#define FIQ_USB_PLUG_INOUT_BIT                    (1 << 1)
#define FIQ_DPLL_UNLOCK_BIT                       (1 << 0)
#else
/*CMD_ICU_INT_DISABLE CMD_ICU_INT_ENABLE*/
#if (!CONFIG_SOC_BK7236A) && (!CONFIG_SOC_BK7236XX)
#define FIQ_JPEG_DECODER_BIT                 (1 << 29)
#endif
#define FIQ_DPLL_UNLOCK_BIT                  (1 << 28)
#define FIQ_SPI_DMA_BIT                      (1 << 27)
#define FIQ_MAC_WAKEUP_BIT                   (1 << 26)
#if (CONFIG_SOC_BK7251)
#define FIQ_SECURITY_BIT                     (1 << 25)
#define FIQ_USB_PLUG_INOUT_BIT               (1 << 24)
#else
#define FIQ_MAILBOX1_BIT                     (1 << 25)
#define FIQ_MAILBOX0_BIT                     (1 << 24)
#endif // (CONFIG_SOC_BK7251)
#define FIQ_PSRAM_BIT						 (1 << 31)
#define FIQ_SDIO_DMA_BIT                     (1 << 23)
#define FIQ_MAC_GENERAL_BIT                  (1 << 22)
#define FIQ_MAC_PROT_TRIGGER_BIT             (1 << 21)
#define FIQ_MAC_TX_TRIGGER_BIT               (1 << 20)
#define FIQ_MAC_RX_TRIGGER_BIT               (1 << 19)
#define FIQ_MAC_TX_RX_MISC_BIT               (1 << 18)
#define FIQ_MAC_TX_RX_TIMER_BIT              (1 << 17)
#define FIQ_MODEM_BIT                        (1 << 16)
#define IRQ_GDMA_BIT                         (1 << 15)
#define IRQ_FFT_BIT                          (1 << 14)
#define IRQ_USB_BIT                          (1 << 13)
#define IRQ_SDIO_BIT                         (1 << 12)
#define IRQ_SARADC_BIT                       (1 << 11)
#if (!CONFIG_SOC_BK7236A) && (!CONFIG_SOC_BK7236XX)
#define IRQ_AUDIO_BIT                        (1 << 10)
#else
#define IRQ_JPEG_DECODER_BIT                 (1 << 10)
#endif
#define IRQ_PWM_BIT                          (1 << 9)
#define IRQ_TL410_WATCHDOG_BIT               (1 << 8)
#define IRQ_GPIO_BIT                         (1 << 7)
#define IRQ_SPI_BIT                          (1 << 6)
#define IRQ_I2C2_BIT                         (1 << 5)
#define IRQ_I2S_PCM_BIT                      (1 << 4)
#define IRQ_IRDA_BIT                         (1 << 3)
#define IRQ_I2C1_BIT                         (1 << 2)
#define IRQ_UART2_BIT                        (1 << 1)
#define IRQ_UART1_BIT                        (1 << 0)
#endif

/* CMD_ICU_GLOBAL_INT_DISABLE CMD_ICU_GLOBAL_INT_ENABLE*/
#define GINTR_FIQ_BIT                            (1 << 1)
#define GINTR_IRQ_BIT                            (1 << 0)

/* CMD_ARM_WAKEUP */
#if (CONFIG_SOC_BK7231)
#define TL410_WATCHDOG_ARM_WAKEUP_EN_BIT                (1 << 8)
#elif (CONFIG_SOC_BK7271)
#define TIMER_ARM_WAKEUP_EN_BIT                         (1 << 13)
#else
#define TIMER_ARM_WAKEUP_EN_BIT                         (1 << 8)
#endif
#define BLE_ARM_WAKEUP_EN_BIT                       	 (1 << 30)

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236XX)
#define BTDM_ARM_WAKEUP_EN_BIT                       	 (1 << 29)
#endif

#if (CONFIG_SOC_BK7271)
#define MAC_ARM_WAKEUP_EN_BIT                       	 (1 << 31)
#define GPIO_ARM_WAKEUP_EN_BIT                           (1 << 11)
#else
#define MAC_ARM_WAKEUP_EN_BIT                       	 (1 << 26)
#define GPIO_ARM_WAKEUP_EN_BIT                           (1 << 7)
#endif

#define AUDIO_ARM_WAKEUP_EN_BIT                         (1 << 10)
#define PWM_ARM_WAKEUP_EN_BIT                           (1 << 9)
#define UART2_ARM_WAKEUP_EN_BIT                         (1 << 1)
#define UART1_ARM_WAKEUP_EN_BIT                         (1 << 0)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void icu_init(void);
void icu_exit(void);
UINT32 icu_ctrl(UINT32 cmd, void *param);


/**< wdt related APIs */
void icu_pwr_up_wdt(void);
void icu_pwr_down_wdt(void);

#ifdef __cplusplus
}
#endif
