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

typedef enum {
	INT_SRC_DMA0_NSEC        = 0,
	INT_SRC_ENC_SEC          = 1,
	INT_SRC_ENC_NSEC         = 2,
	INT_SRC_TIMER            = 3,
	INT_SRC_UART0            = 4,
	INT_SRC_PWM              = 5,
	INT_SRC_I2C0             = 6,
	INT_SRC_SPI0             = 7,
	INT_SRC_SARADC           = 8,
	INT_SRC_UART3            = 9,
	INT_SRC_SDIO             = 10,
	INT_SRC_GDMA             = 11,
	INT_SRC_LA               = 12,
	INT_SRC_TIMER1           = 13,
	INT_SRC_I2C1             = 14,
	INT_SRC_UART1            = 15,
	INT_SRC_UART2            = 16,
	INT_SRC_SPI1             = 17,
	INT_SRC_LED              = 18,
	INT_SRC_TAMPER           = 19,
	INT_SRC_CKMN             = 21,
	INT_SRC_I2S0             = 24,

	/* wifi */
	INT_SRC_MODEM            = 29, // phy_mbp_int
	INT_SRC_MODEM_RC         = 30, // phy_riu_int
	INT_SRC_MAC_TXRX_TIMER   = 31,
	INT_SRC_MAC_TXRX_MISC    = 32,
	INT_SRC_MAC_RX_TRIGGER   = 33,
	INT_SRC_MAC_TX_TRIGGER   = 34,
	INT_SRC_MAC_PROT_TRIGGER = 35,
	INT_SRC_MAC_GENERAL      = 36,
	INT_SRC_MAC_HSU          = 37,
	INT_SRC_MAC_WAKEUP       = 38,

	/* btdm */
	INT_SRC_BTDM             = 39,
	INT_SRC_BLE              = 40,
	INT_SRC_BT               = 41,

	INT_SRC_QSPI0            = 42,
	INT_SRC_THREAD           = 48,
	INT_SRC_OTP              = 50,
	INT_SRC_PLL_UNLOCK       = 51,
	INT_SRC_GPIO             = 55, // GPIO_S
	INT_SRC_GPIO_NS          = 56,

	INT_SRC_ANA_GPIO         = 58,
	INT_SRC_ANA_RTC          = 59,
	INT_SRC_ABNORMAL_GPIO    = 60,
	INT_SRC_ABNORMAL_RTC     = 61,

	INT_SRC_NONE,
} icu_int_src_t;

#ifdef __cplusplus
}
#endif
