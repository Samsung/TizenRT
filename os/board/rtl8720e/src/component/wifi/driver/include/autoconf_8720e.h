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
#ifndef AUTOCONF_8720E_H
#define AUTOCONF_8720E_H

#ifndef CONFIG_RTL8720E
#define CONFIG_RTL8720E
#endif

#ifndef CONFIG_RTL8720E
#define CONFIG_RTL8720E
#endif
#undef RTL8720E_SUPPORT
#define RTL8720E_SUPPORT 1

/* Configure for bus */
#define CONFIG_AXI_HCI

/* For efuse or flash config start */
#define CONFIG_EFUSE_SEPARATE
/* For efuse or flash config end */

/* PHY layer band config */
#define CONFIG_DFS
#define NOT_SUPPORT_40M
/* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
* 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
* 0x21 means enable 2.4G 40MHz & 5G 80MHz */
#define CONFIG_BW_MODE 0x00

/*Amebalite not support 5G*/
//#define SUPPORT_5G_CHANNEL

#define CONFIG_INIT_CHAN 1
/* PHY layer band config end */

/* For DM support */
#define RATE_ADAPTIVE_SUPPORT 0

/* Configurations for power saving */
#define CONFIG_WOWLAN
/* enable 1X code in lib_wlan as default (increase 380 bytes) */
#define CONFIG_EAP

#ifdef CONFIG_BT_EN
#define CONFIG_BT_COEXIST
#endif

//#define CONFIG_WIFI_MESH 0

//#define RX_SHORTCUT
//#define TX_SHORTCUT

/* For 11 ax function */
#define CONFIG_80211AX_HE
#ifdef CONFIG_80211AX_HE
//#define CONFIG_MBSSID_AX
#endif
//#define CONFIG_DEBUG_RTL871X

#if WIFI_LOGO_CERTIFICATION
#ifdef CONFIG_80211AX_HE
#define CONFIG_MBSSID_AX
#endif
#endif

#define TXBD_PRE_MALLOC
#define RTL8720E_SPECIFIC
//#define CONFIG_SUPPORT_DYNAMIC_TXPWR  // rtw_phydm_fill_desc_dpt -> todo

/* config for AP mode */
#define AP_STA_NUM (5)  // reduce ram size, revert this when ddr or psram read
// Decrease STA due to memory limitation - Alex Fang
#define NUM_STA (2 + AP_STA_NUM)  // 2 + supported clients

/*halbb halrf config*/
#define CONFIG_FW_C2H_PKT
#define PHYDM_VERSION	3/*halbb halrf*/
#define DRV_BB_DFS_DISABLE
#define DRV_BB_RUA_DISABLE
#define DRV_BB_LA_MODE_DISABLE
#define DRV_BB_DIG_MCC_DISABLE
#define PHL_MAX_STA_NUM NUM_STA
#define PLATFOM_IS_LITTLE_ENDIAN	1/*for halbb use*/
#define CONFIG_PHYDM_CMD  /*can save 172KB code size*/

/*Wifi verification*/
#if defined(CONFIG_WIFI_VERIFY_TRUEPHY) || defined(CONFIG_WIFI_VERIFY_PSPHY)
#define RTL8720E_WIFI_TEST	1//test code, should delete when use ASIC
#define DISABLE_FW
#define DISABLE_BB_RF
#endif

/* enable csi function */
#define CONFIG_CSI

/* For FPGA and PXP test code */
#if defined(CONFIG_WIFI_VERIFY)
#define RTL8720E_WIFI_TEST 1  // remove this when verification complete
#endif

#define RTL8720E_WL_TODO
#define DISABLE_BB_WATCHDOG

#define CONFIG_REG_ENABLE_KFREE 0  // 0: Depend on efuse(flash), 1: enable, 2: disable

#define PHYSTS_WORK_AROUND
#define RTL8720E_WORK_AROUND
#endif /*#ifndef AUTOCONF_8720E_H */

