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
#ifndef AUTOCONF_8711B_H
#define AUTOCONF_8711B_H


#ifndef CONFIG_RTL8711B
#define CONFIG_RTL8711B
#endif
#undef RTL8711B_SUPPORT
#define RTL8711B_SUPPORT 1
/* Configure for bus */
#define CONFIG_LX_HCI
/* For efuse or flash config start */
#define CONFIG_EFUSE_SEPARATE
/* For efuse or flash config end */
/* PHY layer band config */
//#define SUPPORT_5G_CHANNEL
#define CONFIG_INIT_CHAN 1
#define NOT_SUPPORT_40M
/* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
* 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
* 0x21 means enable 2.4G 40MHz & 5G 80MHz */
#define  CONFIG_BW_MODE	0x00
/* PHY layer band config end */
#define CONFIG_FW_C2H_PKT
/*For phydm configurations*/
#define PHYDM_VERSION	1 /*outsrc*/
/* For DM support */
#define RATE_ADAPTIVE_SUPPORT 0
#define TX_CHECK_DSEC_ALWAYS 1
#define CONFIG_ODM_REFRESH_RAMASK
/* enable 1X code in lib_wlan as default (increase 380 bytes) */
#define CONFIG_EAP
#define CONFIG_8710B_MOVE_TO_ROM
#define CONFIG_MOVE_PSK_TO_ROM
/* Configurations for power saving */
#define CONFIG_WOWLAN
#define CONFIG_WOWLAN_SD1
#define CONFIG_FABVERSION_UMC 	1


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

/* Configurations for power saving */
#define TDMA_POWER_SAVING

#define CONFIG_REG_ENABLE_KFREE 1	 // 1: enable, 2: disable

#endif /* #ifndef AUTOCONF_8711B_H */

