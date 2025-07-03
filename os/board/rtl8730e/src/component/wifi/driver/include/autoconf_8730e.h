/**
  ******************************************************************************
  * @file    autoconf_8730e.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef AUTOCONF_8730E_H
#define AUTOCONF_8730E_H

/* For STA+AP Concurrent MODE */
/****************** configurations for concurrent mode ************************/
//#define CONFIG_NAN

#if defined(CONFIG_FULLMAC) && defined(CONFIG_NAN)
#define CONFIG_MCC_MODE
#endif

#ifdef CONFIG_NAN
#define NET_IF_NUM	3
#define SUPPORT_ADAPTER_NUM	3
#else
#define NET_IF_NUM 2
#define SUPPORT_ADAPTER_NUM	2
#endif
/**************** configurations for concurrent mode end **********************/

/************************* Default Values of User Configure *****************************/
/* TizenRT customization: scan multiple SSIDs */
#ifdef CONFIG_PLATFORM_TIZENRT_OS
/* Upper limit of multiple SSID scan */
#define SSID_SCAN_NUM 6
#endif //CONFIG_PLATFORM_TIZENRT_OS

/* Upper limit of STAs connected with SoftAP, more STAs connected will cost more heap*/
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define AP_STA_NUM	3
#else
#define AP_STA_NUM	12
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#define MACID_HW_MAX_NUM		16
/************************* Default Values of User Configure End***************************/

#ifdef CONFIG_NAN
#define CONFIG_NAN_PAIRING
#define CONFIG_TWT
#endif

#ifdef CONFIG_MCC_MODE
#define CONFIG_TWT
#endif

/* Configure for bus */
#define CONFIG_AXI_HCI
/* For efuse or flash config start */
#define CONFIG_EFUSE_SEPARATE
#define CONFIG_EFUSE_RAW
/* For efuse or flash config end */
/* PHY layer band config */
#define CONFIG_DFS

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

#if WIFI_LOGO_CERTIFICATION == 0
#define RX_SHORTCUT /*there's no reoder in rx short right now, wifi logo need ping 10k which needs reorder*/
#define TX_SHORTCUT /*there's no sw encrypt in tx short right now, wifi logo need htc which needs sw encrypt*/
#endif

//#define SW_WEP_TKIP

#ifdef CONFIG_AS_INIC_NP
// customer requirement, send udp multicast frames around 1890 bytes without fragmentation
#endif

//#define CONFIG_SUPPORT_DYNAMIC_TXPWR
#define RTL8730E_WL_TODO

#define NUM_STA (2 + AP_STA_NUM) /*one for bcmc, one for sta mode*/
/*enable dynamic mechanism for driver*/
#define CONFIG_WIFI_RA
#define CONFIG_WIFI_DIG
#define CONFIG_WIFI_TDMA_DIG /*for softap*/
#define CONFIG_WIFI_EDCCA
#define CONFIG_WIFI_ANTDIV

/*halbb halrf config*/
#define PHYDM_VERSION	3 /*halbb halrf*/
#define DRV_BB_RUA_DISABLE
#define DRV_BB_LA_MODE_DISABLE
#define DRV_BB_PWR_CTRL_DISABLE
#define DRV_BB_CFO_TRK_DISABLE
//#define DRV_BB_ENV_MNTR_DISABLE
#define DRV_BB_DFS_DISABLE

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
#define WIFI_TEST 1
#define DISABLE_FW
#define DISABLE_BB_RF
#endif
#ifdef CONFIG_WIFI_VERIFY_ASIC
#define WIFI_TEST 1
#endif

//#define CONFIG_DFS_TEST  // add for dfs test

/* enable csi function */
#define CONFIG_CSI

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

//#define CONFIG_P2P
/*************************** Config for MP_MODE *******************************/
#ifdef CONFIG_MP_INCLUDED
#undef RX_SHORTCUT
#undef TX_SHORTCUT


#define DRV_BB_DBG_TRACE_DISABLE
#define DRV_BB_PMAC_TX_DISABLE
#define DRV_BB_CMN_RPT_DISABLE
#define DRV_BB_STATISTICS_DISABLE
#define DRV_BB_DGB_SUPPORT_DISABLE
#define DRV_RF_DBG_TRACE_DISABLE
#define DRV_BB_CH_INFO_DISABLE
#define DRV_BB_ENV_MNTR_DISABLE
#define DRV_BB_PHYSTS_PARSING_DISABLE
#define DRV_BB_TIMER_SUPPORT_DISABLE
#endif

#ifdef CONFIG_MP_SHRINK
#undef CONFIG_TWT
#undef CONFIG_MCC_MODE
#undef CONFIG_DFS
#undef CONFIG_FW_C2H_PKT
#undef CONFIG_IEEE80211W
#undef CONFIG_WIFI_RA
#undef CONFIG_WIFI_DIG
#undef CONFIG_WIFI_TDMA_DIG /*for softap*/
#undef CONFIG_WIFI_EDCCA
#undef CONFIG_WIFI_ANTDIV
#define DISABLE_FW
#endif
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

