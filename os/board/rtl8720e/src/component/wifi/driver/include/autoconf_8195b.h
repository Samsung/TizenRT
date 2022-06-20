/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef AUTOCONF_8195B_H
#define AUTOCONF_8195B_H

#define CONFIG_RTL8195B
#undef RTL8195B_SUPPORT
#define RTL8195B_SUPPORT 1
/* Configure for bus */
#define CONFIG_AXI_HCI
/* PHY layer band config */
#define CONFIG_DFS
#define NOT_SUPPORT_40M
/* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
* 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
* 0x21 means enable 2.4G 40MHz & 5G 80MHz */
#define  CONFIG_BW_MODE	0x00
#define SUPPORT_5G_CHANNEL
#define CONFIG_80211AC_VHT
#define VHT_OPTION	1	/* 0:disable, 1:enable, 2:force auto enable */
#define VHT_RX_MCS_MAP0 	(u8)(0xaaaa & 0xFF)
#define VHT_RX_MCS_MAP1 	(u8)((0xaaaa & 0xFF00) >> 8)

#define CONFIG_INIT_CHAN 36
/* PHY layer band config end */
/* For DM support */
#define RATE_ADAPTIVE_SUPPORT 1
#define TX_CHECK_DSEC_ALWAYS 1
//		#define CONFIG_NO_FW
#define CONFIG_EX_FW_BIN
/* Configurations for power saving */
#define CONFIG_WOWLAN
#define CONFIG_WOWLAN_SD1
#define CONFIG_WOWLAN_HW_CAM
#define CONFIG_WOWLAN_CUSTOM_PATTERN
#ifdef CONFIG_WOWLAN
#define CONFIG_WOWLAN_TCP_KEEP_ALIVE
#endif
/* For phydm configurations */
#define CONFIG_FW_C2H_PKT
#define PHYDM_VERSION	2 /*phydm trunk*/
/* config in concurrent mode */
#ifdef CONFIG_CONCURRENT_MODE
#define CONFIG_RUNTIME_PORT_SWITCH
#endif

#define FW_IQK
#define RTW_HALMAC		/* Use HALMAC architecture */

#define CONFIG_MAC_LOOPBACK_DRIVER_RTL8195B 0

#if defined(CONFIG_TWO_MAC_DRIVER) || defined(CONFIG_MAC_LOOPBACK_DRIVER_AMEBA)
#define CONFIG_SUDO_PHY_SETTING
#define HAL_MAC_ENABLE 1
#define DISABLE_BB_RF 1
#else
#define HAL_MAC_ENABLE 1
#define HAL_BB_ENABLE 1
#define HAL_RF_ENABLE 1
#define DISABLE_BB_RF 0
#endif

#define CONFIG_REG_ENABLE_KFREE 0	// 0: Depend on efuse(flash), 1: enable, 2: disable

#endif /* #ifndef AUTOCONF_8195B_H */
