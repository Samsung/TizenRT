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

#undef RTL8720E_SUPPORT
#define RTL8720E_SUPPORT 1

/************************* Default Values of User Configure *****************************/
/* Upper limit of STAs connected with SoftAP, more STAs connected will cost more heap*/
#define AP_STA_NUM	5
/*wifi driver's trx buffer number, each skb cost about 2240 bytes of heap*/
#ifdef CONFIG_HIGH_TP_TEST
#define SKB_NUM_NP	10
#define SKB_NUM_AP	5
#else
#define SKB_NUM_NP	6
#define SKB_NUM_AP	4
#endif
/************************* Default Values of User Configure End***************************/

/* Configure for bus */
#define CONFIG_AXI_HCI

/* For efuse or flash config start */
#define CONFIG_EFUSE_SEPARATE
#define CONFIG_EFUSE_RAW
/* For efuse or flash config end */

/* PHY layer band config */
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
// #define CONFIG_EAP

#define CONFIG_BT_COEXIST

//#define CONFIG_ARP_KEEP_ALIVE

//#define CONFIG_WIFI_MESH 0
#if WIFI_LOGO_CERTIFICATION == 0
#define RX_SHORTCUT /*there's no reoder in rx short right now, wifi logo need ping 10k which needs reorder*/
#define TX_SHORTCUT /*there's no sw encrypt in tx short right now, wifi logo need htc which needs sw encrypt*/
#endif

/* For 11 ax function */
#define CONFIG_80211AX_HE
#ifdef CONFIG_80211AX_HE
//#define CONFIG_MBSSID_AX
//#define CONFIG_TWT
#endif
//#define CONFIG_DEBUG_RTL871X

#if WIFI_LOGO_CERTIFICATION
#ifdef CONFIG_80211AX_HE
#define CONFIG_MBSSID_AX
#endif
/*HW AES encrypt frame with HTC will be wrong,but wifi6 logo need HTC and require high level of TP,
so use SW AES encrypt with IPSEC.  */
#define SW_ENCRYPT_HTC_PKT
#endif

//#define CONFIG_SUPPORT_DYNAMIC_TXPWR  // rtw_phydm_fill_desc_dpt -> todo

#define NUM_STA (2 + AP_STA_NUM)  // 2 + supported clients

/*halbb halrf config*/
#define CONFIG_FW_C2H_PKT
#define PHYDM_VERSION	3/*halbb halrf*/
#define DRV_BB_DFS_DISABLE
#define DRV_BB_RUA_DISABLE
#define DRV_BB_LA_MODE_DISABLE
#define DRV_BB_TDMADIG_DISABLE
#define DRV_BB_DIG_MCC_DISABLE
#define DRV_BB_PWR_CTRL_DISABLE
#define PHL_MAX_STA_NUM NUM_STA
#define PLATFOM_IS_LITTLE_ENDIAN	1/*for halbb use*/

/*Wifi verification*/
#if defined(CONFIG_WIFI_VERIFY_TRUEPHY) || defined(CONFIG_WIFI_VERIFY_PSPHY)
#define RTL8720E_WIFI_TEST	1//test code, should delete when use ASIC
#define DISABLE_FW
#define DISABLE_BB_RF
#endif
#ifdef CONFIG_WIFI_VERIFY_ASIC
#define RTL8720E_WIFI_TEST 1  // add wifi testcode for debug
#endif

//#define CONFIG_DFS_TEST  // add for dfs test

/* enable csi function */
#define CONFIG_CSI

#define RTL8720E_WL_TODO

#define CONFIG_REG_ENABLE_KFREE 0  // 0: Depend on efuse(flash), 1: enable, 2: disable

#define PHYSTS_WORK_AROUND
#define RTL8720E_WORK_AROUND

/*************************** Config for MP_MODE *******************************/
//#define CONFIG_MP_INCLUDED
#ifdef CONFIG_MP_INCLUDED
#define MP_DRIVER 1
#undef CONFIG_ANTENNA_DIVERSITY
#undef CONFIG_BT_COEXIST_SOC
#undef CONFIG_REG_ENABLE_KFREE
#define CONFIG_REG_ENABLE_KFREE 1	 // 1: enable, 2: disable
#define CONFIG_PHYDM_CMD  /*disable it in normal driver,can save 172KB code size*/
#else /* undef CONFIG_MP_INCLUDED  */
#define MP_DRIVER 0
#endif /* #ifdef CONFIG_MP_INCLUDED */
/************************* Config for MP_MODE end *****************************/

#ifndef CONFIG_PHYDM_CMD
#define DRV_BB_DBG_TRACE_DISABLE
#define DRV_BB_PMAC_TX_DISABLE
#define DRV_BB_CMN_RPT_DISABLE
#define DRV_BB_STATISTICS_DISABLE
#define DRV_BB_DGB_SUPPORT_DISABLE
#define DRV_RF_DBG_TRACE_DISABLE
#endif

/*Config for SKB Size*/
#define SKB_CACHE_SZ	32/*max(AP_Core_Cache, NP_Core_Cache)*/
#define SKB_ALIGNMENT	__attribute__((aligned(SKB_CACHE_SZ)))
#define TXDESC_SIZE	40
#define RXDESC_SIZE	24
#define RXPHYSTS_SIZE 0
#define WLAN_HW_INFO_LEN	40/*max(TXDESC_SIZE, RXDESC_SIZE+RXPHYSTS_SIZE)*/
#define WLAN_MAX_ETHFRM_LEN	1514/*max payload size of wifi frame*/

/* debug log level */
#define RELEASE_WIFI
#ifdef RELEASE_WIFI
#define RTW_MSG_LEVEL    RTW_MSG_ERROR
#else
#define RTW_MSG_LEVEL    RTW_MSG_WARNING
#endif

#define CONFIG_DOSCAN_IN_BUSYTRAFFIC
#define CONFIG_IOT_RS 1
#endif /*#ifndef AUTOCONF_8720E_H */

