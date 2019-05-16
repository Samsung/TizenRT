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

/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you dont like!
 */
//#define CONFIG_PLATFOMR_CUSTOMER_RTOS
#if defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#define PLATFORM_CUSTOMER_RTOS 1
#define CONFIG_LWIP_LAYER 0
#else
#define PLATFORM_FREERTOS	1
#endif
#ifdef USE_SDIO_INTERFACE
#define CONFIG_SDIO_HCI
#else
#define CONFIG_GSPI_HCI
#endif

//#define CONFIG_IEEE80211W
#define CONFIG_FW_C2H_PKT
#define PHYDM_LINUX_CODING_STYLE 1

#if (PHYDM_LINUX_CODING_STYLE == 1)
#define PHYDM_NEW_INTERFACE 1
#else
#define PHYDM_NEW_INTERFACE 0
#endif

#ifndef CONFIG_INIC_EN
#define CONFIG_INIC_EN 0 //For iNIC project
#endif

#if CONFIG_INIC_EN
#define CONFIG_LWIP_LAYER    0
#endif

#ifndef CONFIG_WIFI_CRITICAL_CODE_SECTION
#define CONFIG_WIFI_CRITICAL_CODE_SECTION
#endif
#define CONFIG_LITTLE_ENDIAN
#define CONFIG_80211N_HT
//#define CONFIG_RECV_REORDERING_CTRL
#define RTW_NOTCH_FILTER 0
#define CONFIG_EMBEDDED_FWIMG
#define CONFIG_PHY_SETTING_WITH_ODM

#define HAL_MAC_ENABLE 1
#define HAL_BB_ENABLE 1
#define HAL_RF_ENABLE 1


//#define CONFIG_DEBUG
//#define CONFIG_DEBUG_RTL871X
#define CONFIG_MEM_MONITOR	MEM_MONITOR_SIMPLE
//#define CONFIG_TRACE_SKB
//#define WLAN_INTF_DBG

//#define CONFIG_DONT_CARE_TP
//#define CONFIG_HIGH_TP
//#define CONFIG_MEMORY_ACCESS_ALIGNED
#ifdef CONFIG_BT_EN
#define CONFIG_FTL_ENABLED
#endif

#define CONFIG_POWER_SAVING
#ifdef CONFIG_POWER_SAVING
	#define CONFIG_IPS
	#define CONFIG_LPS
	#ifdef CONFIG_LPS
		#define CONFIG_LPS_CHK_BY_TP
	#endif
	//#define CONFIG_LPS_LCLK
#ifdef CONFIG_LPS_LCLK
	#define CONFIG_DETECT_CPWM_BY_POLLING
	#define LPS_RPWM_WAIT_MS 300
#endif
	#define CONFIG_WAIT_PS_ACK
	#define CONFIG_FW_PSTIMEOUT
#endif

#define BAD_MIC_COUNTERMEASURE 1
#define DEFRAGMENTATION 1

#define WIFI_LOGO_CERTIFICATION 0
#if WIFI_LOGO_CERTIFICATION
    #define RX_AGGREGATION 1
	#define RX_AMSDU 1
#else
	#ifdef CONFIG_HIGH_TP_TEST
		#define RX_AGGREGATION 1
	#else
		#define RX_AGGREGATION 0
	#endif
	#define RX_AMSDU 0
#endif

#define CONFIG_XMIT_THREAD_MODE
//#define CONFIG_RECV_THREAD_MODE				/* Wlan IRQ Polling  Mode*/
//#define CONFIG_ISR_THREAD_MODE_POLLING		/* Wlan IRQ Polling  Mode*/

//1 Chris
#ifndef CONFIG_SDIO_HCI
#define CONFIG_ISR_THREAD_MODE_INTERRUPT	/* Wlan IRQ Interrupt  Mode*/
#endif

#if defined(CONFIG_ISR_THREAD_MODE_POLLING) && defined(CONFIG_ISR_THREAD_MODE_INTERRUPT)
#error "CONFIG_ISR_THREAD_MODE_POLLING and CONFIG_ISR_THREAD_MODE_INTERRUPT are mutually exclusive. "
#endif

//#define CONFIG_RECV_TASK_THREAD_MODE


/* AES DMEM optimized mode comsume 10k less memory compare to 
     IMEM optimized mode AES_IMPLEMENTATION_IMEM_OPTIMIZED */
#define AES_IMPLEMENTATION_MODE AES_IMPLEMENTATION_DMEM_OPTIMIZED

#define USE_SKB_AS_XMITBUF	1
#define USE_XMIT_EXTBUFF	0
#define USE_MUTEX_FOR_SPINLOCK	1

// remove function to reduce code
#define NOT_SUPPORT_5G
#if !defined(CONFIG_HARDWARE_8192E)
#define NOT_SUPPORT_RF_MULTIPATH
#endif
#define NOT_SUPPORT_VHT
#define NOT_SUPPORT_40M
#define NOT_SUPPORT_80M
#ifdef CONFIG_HIGH_TP_TEST
	#undef NOT_SUPPORT_40M
#endif
#define NOT_SUPPORT_OLD_CHANNEL_PLAN
#define NOT_SUPPORT_BT

#define CONFIG_WIFI_SPEC	0
#define CONFIG_FAKE_EFUSE	0
#if CONFIG_FAKE_EFUSE
	#define FAKE_CHIPID		CHIPID_8710BN
#endif

#define CONFIG_AUTO_RECONNECT 1
#define ENABLE_HWPDN_PIN
#define SUPPORT_SCAN_BUF	1
#define BE_I_CUT			1

/* For WPA2 */
#define CONFIG_INCLUDE_WPA_PSK
#ifdef CONFIG_INCLUDE_WPA_PSK
#define CONFIG_MULTIPLE_WPA_STA
//#define CONFIG_WPA2_PREAUTH
#define PSK_SUPPORT_TKIP	1
#endif

/* For promiscuous mode */
#define CONFIG_PROMISC

#define PROMISC_DENY_PAIRWISE	0

/* For Simple Link */
#ifndef CONFIG_INCLUDE_SIMPLE_CONFIG
//#define CONFIG_INCLUDE_SIMPLE_CONFIG 1
#endif

// for probe request with custom vendor specific IE
#define CONFIG_CUSTOM_IE

/* For multicast */
#define CONFIG_MULTICAST

/* For STA+AP Concurrent MODE */
#define CONFIG_CONCURRENT_MODE
#ifdef CONFIG_CONCURRENT_MODE
//#define CONFIG_MCC_MODE
#if defined(CONFIG_HARDWARE_8723D) || defined(CONFIG_HARDWARE_8188F)
#define CONFIG_RUNTIME_PORT_SWITCH
#endif
#define NET_IF_NUM 2
#else
#define NET_IF_NUM 1
#endif


/****************** For EAP auth configurations *******************/
#define CONFIG_TLS	0
#define CONFIG_PEAP	0
#define CONFIG_TTLS	0

// DO NOT change the below config of EAP
#ifdef PRE_CONFIG_EAP
#undef CONFIG_TLS
#define CONFIG_TLS	1
#undef CONFIG_PEAP
#define CONFIG_PEAP	1
#undef CONFIG_TTLS
#define CONFIG_TTLS	1
#endif

// enable 1X code in lib_wlan as default (increase 380 bytes)
#define CONFIG_EAP

#if CONFIG_TLS || CONFIG_PEAP || CONFIG_TTLS
#define EAP_REMOVE_UNUSED_CODE 1
#endif	     
	     
#define EAP_SSL_VERIFY_SERVER

#if CONFIG_TLS
#define EAP_SSL_VERIFY_CLIENT
#endif

#if CONFIG_TTLS
#define EAP_MSCHAPv2
#define EAP_TTLS_MSCHAPv2
//#define EAP_TTLS_EAP
//#define EAP_TTLS_MSCHAP
//#define EAP_TTLS_PAP
//#define EAP_TTLS_CHAP
#endif
/****************** End of EAP configurations *******************/

/* For WPS and P2P */
#define CONFIG_WPS
#if 0
#define CONFIG_WPS_AP
#define CONFIG_P2P_NEW
#if (!defined(SUPPORT_SCAN_BUF)||!defined(CONFIG_WPS_AP)) && defined(CONFIG_P2P_NEW)
#error "If CONFIG_P2P_NEW, need to SUPPORT_SCAN_BUF"
#endif
#endif

#define CONFIG_NEW_SIGNAL_STAT_PROCESS
#define CONFIG_SKIP_SIGNAL_SCALE_MAPPING

/* For AP_MODE */
#define CONFIG_AP_MODE
extern unsigned char g_user_ap_sta_num;
#define USER_AP_STA_NUM g_user_ap_sta_num
extern unsigned int g_ap_sta_num;
#define AP_STA_NUM 3//g_ap_sta_num
#ifdef CONFIG_AP_MODE
	#define CONFIG_NATIVEAP_MLME
	#ifdef CONFIG_INTERRUPT_BASED_TXBCN
		//#define CONFIG_INTERRUPT_BASED_TXBCN_EARLY_INT
		#define CONFIG_INTERRUPT_BASED_TXBCN_BCN_OK_ERR
	#endif
//	#define CONFIG_GK_REKEY
	#define USE_DEDICATED_BCN_TX	1
#if CONFIG_INIC_EN
//	#define REPORT_STA_EVENT //useless
#endif
#else
	#define USE_DEDICATED_BCN_TX	0
#endif

#if defined(CONFIG_AP_MODE) && defined(CONFIG_GK_REKEY) && !defined(CONFIG_MULTIPLE_WPA_STA)
#error "If CONFIG_GK_REKEY when CONFIG_AP_MODE, need to CONFIG_MULTIPLE_WPA_STA"
#endif

#if !defined(CONFIG_AP_MODE) && defined(CONFIG_CONCURRENT_MODE)
#error "If CONFIG_CONCURRENT_MODEE, need to CONFIG_AP_MODE"
#endif

/* For MP_MODE */
//#define CONFIG_MP_INCLUDED
#ifdef CONFIG_MP_INCLUDED
	#define MP_DRIVER		1
	#define CONFIG_MP_IWPRIV_SUPPORT
//	#define HAL_EFUSE_MEMORY
#else
	#define MP_DRIVER		0
#endif // #ifdef CONFIG_MP_INCLUDED

// for Debug message
#define DBG 0

#if defined(CONFIG_HARDWARE_8188F)
#define CONFIG_RTL8188F
#elif defined(CONFIG_HARDWARE_8192E)
#define CONFIG_RTL8192E
#elif defined(CONFIG_HARDWARE_8821C)
#define CONFIG_RTL8821C
#elif defined(CONFIG_HARDWARE_8723D)
#define CONFIG_RTL8723D
#elif defined(CONFIG_HARDWARE_8723B)
#define CONFIG_RTL8723B
#elif defined(CONFIG_HARDWARE_8703B)
#define CONFIG_RTL8703B
#elif defined(CONFIG_HARDWARE_8188E)
#define CONFIG_RTL8188E
#else
#define CONFIG_RTL8188E
#endif

#define RTL8192E_SUPPORT 0
#define RTL8812A_SUPPORT 0
#define RTL8821A_SUPPORT 0
#define RTL8723B_SUPPORT 0
#define RTL8188E_SUPPORT 0
#define RTL8188F_SUPPORT 0
#define RTL8821C_SUPPORT 0
#define RTL8723D_SUPPORT 0
#if defined(CONFIG_HARDWARE_8188F)
#undef	RTL8188F_SUPPORT
#define RTL8188F_SUPPORT 1
#elif defined(CONFIG_HARDWARE_8192E)
#undef RTL8192E_SUPPORT
#define RTL8192E_SUPPORT 1
#elif defined(CONFIG_HARDWARE_8821C)
#undef	RTL8821C_SUPPORT
#define RTL8821C_SUPPORT 1
#elif defined(CONFIG_HARDWARE_8723D)
#undef	RTL8723D_SUPPORT
#define RTL8723D_SUPPORT 1
#elif defined(CONFIG_HARDWARE_8723B)
#undef	RTL8723B_SUPPORT
#define RTL8723B_SUPPORT 1
#elif defined(CONFIG_HARDWARE_8703B)
#undef	RTL8703B_SUPPORT
#define RTL8703B_SUPPORT 1
#elif defined(CONFIG_HARDWARE_8188E)
#undef RTL8188E_SUPPORT
#define RTL8188E_SUPPORT 1
#else
#undef RTL8188E_SUPPORT
#define RTL8188E_SUPPORT 1
#endif

/* For DM support */
#if defined(CONFIG_RTL8188F) 
#define RATE_ADAPTIVE_SUPPORT 0
#elif defined(CONFIG_RTL8821C)
#define RATE_ADAPTIVE_SUPPORT 0
#elif defined(CONFIG_RTL8192E)
#define RATE_ADAPTIVE_SUPPORT 0
#elif defined(CONFIG_RTL8723D)
#define RATE_ADAPTIVE_SUPPORT 0
#elif defined(CONFIG_RTL8723B)
#define RATE_ADAPTIVE_SUPPORT 0
#elif defined(CONFIG_RTL8703B)
#define RATE_ADAPTIVE_SUPPORT 0
#define CONFIG_BT_COEXIST
#elif defined(CONFIG_PLATFORM_8711B)
#define RATE_ADAPTIVE_SUPPORT 0
#define CONFIG_ODM_REFRESH_RAMASK
#elif defined(CONFIG_PLATFORM_8721D)
#define RATE_ADAPTIVE_SUPPORT 0
//#define CONFIG_ODM_REFRESH_RAMASK
#elif defined(CONFIG_PLATFORM_8710C)
#define RATE_ADAPTIVE_SUPPORT 0
//#define CONFIG_ODM_REFRESH_RAMASK
#else
#define RATE_ADAPTIVE_SUPPORT 1
#endif

//#define CONFIG_BT_COEXIST
#ifdef CONFIG_BT_COEXIST
#undef NOT_SUPPORT_BT
#define CONFIG_BT_MAILBOX
#define CONFIG_BT_EFUSE
//#define CONFIG_BT_TWO_ANTENNA
#if defined(CONFIG_RTL8723D) || defined(CONFIG_RTL8821C) || defined(CONFIG_RTL8703B)
#define HAL_EFUSE_MEMORY
#endif
#endif

// adaptivity
#define RTW_ADAPTIVITY_EN_DISABLE			0
#define RTW_ADAPTIVITY_EN_ENABLE			1
#define CONFIG_RTW_ADAPTIVITY_EN			RTW_ADAPTIVITY_EN_DISABLE
#define RTW_ADAPTIVITY_MODE_NORMAL			0
#define RTW_ADAPTIVITY_MODE_CARRIER_SENSE	1
#define CONFIG_RTW_ADAPTIVITY_MODE			RTW_ADAPTIVITY_MODE_CARRIER_SENSE
#define CONFIG_RTW_ADAPTIVITY_DML			0

#define POWER_BY_RATE_SUPPORT 0

#ifndef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER 1
#endif
#define CONFIG_MAC_ADDRESS 0
//fast reconnection
//#define CONFIG_FAST_RECONNECTION 1
#if defined(CONFIG_INIC_EN)&&(CONFIG_INIC_EN==1)
#define CONFIG_RECV_REORDERING_CTRL //enable reordering for iNIC high throughput
#undef RX_AGGREGATION
#define RX_AGGREGATION 1
#undef NOT_SUPPORT_40M
#undef CONFIG_CONCURRENT_MODE
#endif

#if defined(CONFIG_HARDWARE_8821C)
#define FW_IQK
#define RTW_HALMAC
#define LOAD_FW_HEADER_FROM_DRIVER
#define RTW_HALMAC_SIZE_OPTIMIZATION 1
//#define CONFIG_NO_FW
#ifdef NOT_SUPPORT_5G
#undef NOT_SUPPORT_5G
#define SUPPORT_5G_CHANNEL	1
#endif
#endif

//#define CONFIG_ADDRESS_ALIGNMENT
#ifdef CONFIG_ADDRESS_ALIGNMENT
#define ALIGNMENT_SIZE 32
#endif

#define CONFIG_DFS
//#define CONFIG_EMPTY_EFUSE_PG_ENABLE

#define WLAN_WRAPPER_VERSION 1

#define TIME_THRES	20

//#define CONFIG_WOWLAN
//#define CONFIG_SUSPEND
//#define CONFIG_GPIO_WAKEUP

//#define CONFIG_AMBARELLA_REQUEST

