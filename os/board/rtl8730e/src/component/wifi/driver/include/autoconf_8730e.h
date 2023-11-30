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
#ifndef AUTOCONF_8730E_H
#define AUTOCONF_8730E_H

#ifndef CONFIG_RTL8730E
#define CONFIG_RTL8730E
#endif
#undef RTL8730E_SUPPORT
#define RTL8730E_SUPPORT 1

/* For STA+AP Concurrent MODE */
/****************** configurations for concurrent mode ************************/
//#define CONFIG_MCC_MODE
//#define CONFIG_STA_MODE_SCAN_UNDER_AP_MODE
#define NET_IF_NUM 3
#define SUPPORT_ADAPTER_NUM	2
/**************** configurations for concurrent mode end **********************/

/************************* Default Values of User Configure *****************************/
/* Upper limit of STAs connected with SoftAP, more STAs connected will cost more heap*/
#define AP_STA_NUM	12
/************************* Default Values of User Configure End***************************/

//#define CONFIG_NAN
#ifdef CONFIG_NAN
#define CONFIG_TWT
#undef SUPPORT_ADAPTER_NUM
#define SUPPORT_ADAPTER_NUM	3
#undef AP_STA_NUM
#define AP_STA_NUM	6
#endif

/* Configure for bus */
#define CONFIG_AXI_HCI
/* For efuse or flash config start */
#define CONFIG_EFUSE_SEPARATE
#define CONFIG_EFUSE_RAW
/* For efuse or flash config end */
/* PHY layer band config */
#define CONFIG_DFS
#define NOT_SUPPORT_40M
/* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
* 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
* 0x21 means enable 2.4G 40MHz & 5G 80MHz */
#define  CONFIG_BW_MODE	0x00
#define SUPPORT_5G_CHANNEL
/* PHY layer band config end */

/* enable 1X code in lib_wlan as default (increase 380 bytes) */
#define CONFIG_EAP

/* For phydm configurations */
#define CONFIG_FW_C2H_PKT

#define CONFIG_BT_COEXIST

//#define CONFIG_ARP_KEEP_ALIVE

#if WIFI_LOGO_CERTIFICATION == 0
#define RX_SHORTCUT /*there's no reoder in rx short right now, wifi logo need ping 10k which needs reorder*/
#define TX_SHORTCUT /*there's no sw encrypt in tx short right now, wifi logo need htc which needs sw encrypt*/
#endif

#ifdef CONFIG_AS_INIC_NP
// customer requirement, send udp multicast frames around 1890 bytes without fragmentation
#endif

//#define CONFIG_SUPPORT_DYNAMIC_TXPWR   //rtw_phydm_fill_desc_dpt todo
#define RTL8730E_WL_TODO

#define NUM_STA (2 + AP_STA_NUM) /*one for bcmc, one for sta mode*/

/*enable ra mechanism start*/
#define CONFIG_WIFI_RA
/*enable ra mechanism end*/
/*enable dig mechanism start*/
#define CONFIG_WIFI_DIG
#define CONFIG_WIFI_TDMA_DIG /*for softap*/
/*enable dig mechanism end*/

/*halbb halrf config*/
#define PHYDM_VERSION	3 /*halbb halrf*/
#define DRV_BB_RUA_DISABLE
#define DRV_BB_LA_MODE_DISABLE
#define DRV_BB_PWR_CTRL_DISABLE
//#define DRV_BB_ENV_MNTR_DISABLE
#define DRV_BB_DFS_DISABLE
#define PHL_MAX_STA_NUM NUM_STA

#define CONFIG_80211AC_VHT
#ifdef CONFIG_80211AC_VHT
#define CONFIG_BEAMFORMEE_VHT
#define VHT_OPTION	1	/* 0:disable, 1:enable, 2:force auto enable */
/*00: support mcs0~7, 01: support mcs0~8,10: support mcs0~9, 11: not support*/
#define VHT_RX_MCS_MAP0 	(u8)(0x5555 & 0xFF)
#define VHT_RX_MCS_MAP1 	(u8)((0x5555 & 0xFF00) >> 8)
#endif

#define CONFIG_80211AX_HE
#ifdef CONFIG_80211AX_HE
//#define CONFIG_MBSSID_AX
//#define CONFIG_TWT
#endif

#if WIFI_LOGO_CERTIFICATION
#ifdef CONFIG_80211AX_HE
#define CONFIG_MBSSID_AX
#endif
#endif

/*Wifi verification*/
#if defined(CONFIG_WIFI_VERIFY_TRUEPHY) || defined(CONFIG_WIFI_VERIFY_PSPHY)
#define RTL8730E_WIFI_TEST	1//test code, should delete when use ASIC
#define DISABLE_FW
#define DISABLE_BB_RF
#endif
#ifdef CONFIG_WIFI_VERIFY_ASIC
#define RTL8730E_WIFI_TEST 1  // add wifi testcode for debug
#endif

//#define CONFIG_DFS_TEST  // add for dfs test

/* enable csi function */
#define CONFIG_CSI

#define CONFIG_REG_ENABLE_KFREE 0	// 0: Depend on efuse(flash), 1: enable, 2: disable
//#define DISABLE_BB_WATCHDOG
#define PLATFOM_IS_LITTLE_ENDIAN	1/*for halbb use*/

#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
#define DISABLE_FW
#define CONFIG_TWT
#define TWT_CSI_EN			0
#define TWT_TRIGGER_EN	1
#else
#define PHYSTS_WORK_AROUND
#endif
#define RTL8730E_WORK_AROUND

//#define RA_RX_ACK_RSSI
/*************************** Config for MP_MODE *******************************/
//#define CONFIG_MP_INCLUDED
#ifdef CONFIG_MP_INCLUDED
#define MP_DRIVER 1
#undef CONFIG_BT_COEXIST_SOC
#undef CONFIG_REG_ENABLE_KFREE
#define CONFIG_REG_ENABLE_KFREE 1	 // 1: enable, 2: disable
#define CONFIG_PHYDM_CMD  /*disable it in normal driver,lite can save 172KB code size, smart need check*/
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
#define SKB_CACHE_SZ	64/*max(AP_Core_Cache, NP_Core_Cache)*/
#define SKB_ALIGNMENT	__attribute__((aligned(SKB_CACHE_SZ)))
#define TXDESC_SIZE	40
#define RXDESC_SIZE	24
#define RXPHYSTS_SIZE 0
#define WLAN_HW_INFO_LEN	40/*max(TXDESC_SIZE, RXDESC_SIZE+RXPHYSTS_SIZE)*/
#define WLAN_MAX_ETHFRM_LEN	1904/*max payload size of wifi frame*/

/* debug log level */
#define RELEASE_WIFI
#ifdef RELEASE_WIFI
#define RTW_MSG_LEVEL    RTW_MSG_ERROR
#else
#define RTW_MSG_LEVEL    RTW_MSG_WARNING
#endif

#define CONFIG_IOT_RS 1
#endif /*#ifndef AUTOCONF_8730A_H */

