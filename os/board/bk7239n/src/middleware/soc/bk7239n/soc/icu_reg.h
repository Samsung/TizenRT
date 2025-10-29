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

#include <soc/soc.h>

//TODO place all auto-generated Registers here!!!

#define ICU_R_BASE                     (SOC_ICU_REG_BASE)

#define ICU_R_CLK_MUX                  (ICU_R_BASE)

#define ICU_F_PWMS_CLK_MUX             (BIT(7))
#define ICU_F_PWMS_CLK_MUX_M           (BIT(7))
#define ICU_F_PWMS_CLK_MUX_V           0x1
#define ICU_F_PWMS_CLK_MUX_S           7
#define ICU_V_PWMS_CLK_MUX_26M         1
#define ICU_V_PWMS_CLK_MUX_DCO         0

#define ICU_R_PWM_CHAN_CLK_MUX         (ICU_R_BASE + 4 * 1)

#define ICU_F_PWM_CHAN_CLK_MUX         (BIT(0))
#define ICU_F_PWM_CHAN_CLK_MUX_M       (BIT(0))
#define ICU_F_PWM_CHAN_CLK_MUX_V       0x1
#define ICU_F_PWM_CHAN_CLK_MUX_S       0
#define ICU_F_PWM_CHAN_CLK_MUX_MS(_ch) (ICU_F_PWM_CHAN_CLK_MUX_S + 1 * (_ch))
#define ICU_V_PWM_CHAN_CLK_MUX_26M     1
#define ICU_V_PWM_CHAN_CLK_MUX_GLOBAL  0

#define ICU_R_PWM_CHAN_PWR             (ICU_R_BASE + 4 * 2)

#define ICU_F_PWM_CHAN_PWR             (BIT(9))
#define ICU_F_PWM_CHAN_PWR_M           (BIT(9))
#define ICU_F_PWM_CHAN_PWR_V           0x1
#define ICU_F_PWM_CHAN_PWR_S           9
#define ICU_F_PWM_CHAN_PWR_MS(_ch)     (ICU_F_PWM_CHAN_PWR_S + 1 * (_ch))
#define ICU_V_PWM_CHAN_PWR_UP          0
#define ICU_V_PWM_CHAN_PWR_DOWN        1

#define ICU_R_TIMER_CHAN_PWR           (ICU_R_BASE + 4 * 2)

#define ICU_F_TIMER_CHAN_PWR           (BIT(20))
#define ICU_F_TIMER_CHAN_PWR_M         (BIT(20))
#define ICU_F_TIMER_CHAN_PWR_V         0x1
#define ICU_F_TIMER_CHAN_PWR_S         20
#define ICU_F_TIMER_CHAN_PWR_MS(_ch)   (ICU_F_TIMER_CHAN_PWR_S + (_ch) / 3)
#define ICU_V_TIMER_CHAN_PWR_UP        0
#define ICU_V_TIMER_CHAN_PWR_DOWN      1

#define ICU_V_CLK_MUX_26M              1
#define ICU_V_CLK_MUX_DCO              0

#define	ICU_R_JTAG_SELECT              (ICU_R_BASE + 4 * 0x6)
#define ICU_V_JTAG_SEL_WR_ARM          (0x7111E968)
#define ICU_V_JTAG_SEL_WR_TL4          (0x7111E410)
#define ICU_V_JTAG_SEL_RD_ARM          (0x00000000)
#define ICU_V_JTAG_SEL_RD_TL4          (0x00000001)

#define	ICU_R_INT_EN                   (ICU_R_BASE + 4 * 0x10)
#define	ICU_R_GLOBAL_INT_EN            (ICU_R_BASE + 4 * 0x11)
#define	ICU_R_INT_RAW_STATUS           (ICU_R_BASE + 4 * 0x12)
#define	ICU_R_INT_STATUS               (ICU_R_BASE + 4 * 0x13)
#define ICU_R_ARM_WAKEUP_EN            (ICU_R_BASE + 4 * 0x14)

#define MAX_INT_GROUP_NUM		2
#define FIQ_MAX_COUNT			16
#define IRQ_MAX_COUNT			16
#define FIQ_STATIS_COUNT		FIQ_MAX_COUNT
#define IRQ_STATIS_COUNT		IRQ_MAX_COUNT
#define FIQ_START_COUNT_BIT		IRQ_MAX_COUNT
#define INTC_MAX_COUNT            (IRQ_MAX_COUNT + FIQ_MAX_COUNT)

#define ICU_IRQ_PWM				(9)

#define ICU_PRI_FIQ_BT                           (7)
#define ICU_PRI_FIQ_BLE                          (8)
#define ICU_PRI_FIQ_BTDM                         (13)
#define ICU_PRI_FIQ_DPLL_UNLOCK                  (29)
#define ICU_PRI_FIQ_SPI_DMA                      (7)
#define ICU_PRI_FIQ_MAC_WAKEUP                   (9)
#define ICU_PRI_FIQ_SECURITY                     (12)
#define ICU_PRI_FIQ_MAC_GENERAL                  (1)
#define ICU_PRI_FIQ_MAC_PROT_TRIGGER             (6)
#define ICU_PRI_FIQ_MAC_TX_TRIGGER               (3)
#define ICU_PRI_FIQ_MAC_RX_TRIGGER               (4)
#define ICU_PRI_FIQ_MAC_TX_RX_MISC               (5)
#define ICU_PRI_FIQ_MAC_TX_RX_TIMER              (2)
#define ICU_PRI_FIQ_MODEM                        (10)

#define ICU_PRI_IRQ_GENERDMA                     (28)
#define ICU_PRI_IRQ_LA                           (29)
#define ICU_PRI_IRQ_SEC                          (21)
#define ICU_PRI_IRQ_SDIO                         (22)
#define ICU_PRI_IRQ_SARADC                       (16)
#define ICU_PRI_IRQ_PWM                          (17)
#define ICU_PRI_IRQ_TIMER                        (18)
#define ICU_PRI_IRQ_GPIO                         (19)
#define ICU_PRI_IRQ_SPI                          (20)
#define ICU_PRI_IRQ_I2C1                         (21)
#define ICU_PRI_IRQ_I2S_PCM                      (22)
#define ICU_PRI_IRQ_IRDA                         (23)
#define ICU_PRI_IRQ_I2C0                         (24)
#define ICU_PRI_IRQ_UART2                        (25)
#define ICU_PRI_IRQ_UART1                        (26)
#define ICU_PRI_IRQ_QSPI                         (27)

#define ICU_PRI_FIQ_HSU                          (11)
#define ICU_PRI_FIQ_RC                           (12)
#define ICU_PRI_JPEG_ENCODER                     (20)

#define IQR_PRI_DEFAULT                          (6)

#ifdef __cplusplus
}
#endif
