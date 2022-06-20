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
#ifndef WLANCONFIG_H
#define WLANCONFIG_H

/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you dont like!
 */
#include "platform_opts.h"

#define CONFIG_LITTLE_ENDIAN

#if defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBALITE)
#ifndef CONFIG_PLATFORM_AMEBA_X
#define CONFIG_PLATFORM_AMEBA_X 1
#endif
#else
#define CONFIG_PLATFORM_AMEBA_X 0
#endif

//#define CONFIG_HIGH_TP

#define WIFI_LOGO_CERTIFICATION 0
#define RX_AMSDU

/* no IOT chip supports 80M now, so close it in common */
#define NOT_SUPPORT_80M
#define CONFIG_AUTO_RECONNECT 1

/* For WPA3 */
#define CONFIG_IEEE80211W
#define CONFIG_SAE_SUPPORT
#ifdef CONFIG_SAE_SUPPORT
#define CONFIG_SAE_DH_SUPPORT 1
#define ALL_DH_GROUPS
#endif

//#define CONFIG_RTK_MESH

/* For promiscuous mode */
#define CONFIG_PROMISC

/* For STA+AP Concurrent MODE */
#define CONFIG_CONCURRENT_MODE
/****************** configurations for concurrent mode ************************/
#ifdef CONFIG_CONCURRENT_MODE
//#define CONFIG_MCC_MODE
//#define CONFIG_STA_MODE_SCAN_UNDER_AP_MODE
#define NET_IF_NUM ((CONFIG_ETHERNET) + (CONFIG_WLAN) + 1)
#define CONFIG_IFACE_NUMBER	2
#else
#define NET_IF_NUM ((CONFIG_ETHERNET) + (CONFIG_WLAN))
#define CONFIG_IFACE_NUMBER	1
#endif
/**************** configurations for concurrent mode end **********************/

/* For WPS and P2P */
#define CONFIG_WPS
#if 0
#define CONFIG_WPS_AP
#define CONFIG_P2P_NEW
#endif

/***************************** for Debug message ******************************/
#include "autoconf_debug.h"
/***************************** for Debug message ******************************/

/************************** config to support chip ****************************/
#define RTL8723B_SUPPORT 0
#define RTL8192E_SUPPORT 0
#define RTL8188E_SUPPORT 0
#define RTL8188F_SUPPORT 0
#define RTL8711B_SUPPORT 0
#define RTL8721D_SUPPORT 0
#define RTL8723D_SUPPORT 0
#define RTL8195B_SUPPORT 0
#define RTL8710C_SUPPORT 0
#define RTL8730A_SUPPORT 0
#define RTL8730E_SUPPORT 0
#define RTL8735B_SUPPORT 0
/************************ config to support chip end **************************/

/******************** Configurations for each platform ************************/
#if (CONFIG_PLATFORM_AMEBA_X == 1)
/******************* Ameba Series Common Configurations ***********************/
#if defined(CONFIG_HIGH_TP_TEST) || defined(CONFIG_INIC_IPC_HIGH_TP)
#define SKB_PRE_ALLOCATE_RX	1
#define EXCHANGE_LXBUS_RX_SKB 1
#else
#define SKB_PRE_ALLOCATE_RX	0
#define EXCHANGE_LXBUS_RX_SKB 0
#endif
//#define CONFIG_WLAN_HAL_RX_TASK

#define NOT_SUPPORT_RF_MULTIPATH
#define USE_XMIT_EXTBUFF 1

/*PHYDM version*/
#define OUTSRC	1
#define PHYDM	2
#define HALBBRF	3

#if defined(CONFIG_PLATFORM_8711B)
/******************************* AmebaZ (8711B) *******************************/
#include "autoconf_8711b.h"
#elif defined(CONFIG_PLATFORM_8721D)
/******************************* AmebaD (8721D) *******************************/
#include "autoconf_8721d.h"
#elif defined(CONFIG_PLATFORM_8195BHP)
/***************************** AmebaPro (8195BHP) *****************************/
#include "autoconf_8195b.h"
#elif defined(CONFIG_PLATFORM_8735B)
/***************************** AmebaPro2 (8735B) ******************************/
#include "autoconf_8735b.h"
#elif defined(CONFIG_PLATFORM_8710C)
/******************************* AmebaZ2 (8710C) ******************************/
#include "autoconf_8710c.h"
#elif defined(CONFIG_PLATFORM_AMEBAD2)
/******************************* AmebaD2 testchip(8730A)/ mpchip(8730E) ******************************/
#if defined(CONFIG_RLE1080_A_CUT)
#include "autoconf_8730a.h"
#else
#include "autoconf_8730e.h"
#endif
#elif defined(CONFIG_PLATFORM_AMEBALITE)
/***************************** AmebaLite (8720E) *****************************/
#include "autoconf_8720e.h"
#endif
/****************************** Ameba Series End ******************************/
#else /* (CONFIG_PLATFORM_AMEBA_X != 1) */
/*************************** Non Ameba Series Start ***************************/
#include "autoconf_nic.h"
/*************************** Non Ameba Series End *****************************/
#endif /* (CONFIG_PLATFORM_AMEBA_X == 1) */
/****************** Configurations for each platform end **********************/

/********************** For AP mode configurations ****************************/
/* to save memory, set AP_STA_NUM to 3. */
#ifndef AP_STA_NUM
#define AP_STA_NUM (3)
//Decrease STA due to memory limitation - Alex Fang
#define NUM_STA (2 + AP_STA_NUM)	//2 + supported clients
#endif /* not defined(AP_STA_NUM) */

//#define CONFIG_GK_REKEY
/******************** For AP mode configurations end **************************/

/************************ For EAP auth configurations *************************/
#include "autoconf_eap.h"
/************************ For EAP auth configurations *************************/

/* 80211 - K V R */
//#define CONFIG_IEEE80211K
//#define CONFIG_LAYER2_ROAMING
#ifdef CONFIG_LAYER2_ROAMING
//#define CONFIG_RTW_WNM
//#define CONFIG_IEEE80211R
#endif

/*************************** Config for MP_MODE *******************************/
//#define CONFIG_MP_INCLUDED
#ifdef CONFIG_MP_INCLUDED
#define MP_DRIVER 1
#undef CONFIG_ANTENNA_DIVERSITY
#undef CONFIG_BT_COEXIST_SOC
#undef CONFIG_REG_ENABLE_KFREE
#define CONFIG_REG_ENABLE_KFREE 1	 // 1: enable, 2: disable
#else /* undef CONFIG_MP_INCLUDED  */
#define MP_DRIVER 0
#endif /* #ifdef CONFIG_MP_INCLUDED */
/************************* Config for MP_MODE end *****************************/

/******************************** inic config *********************************/
#if defined(CONFIG_INIC_EN)
#ifndef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER 0
#endif /* defined(CONFIG_INIC_EN) */
#endif

#ifndef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER 1
#endif /* ndef CONFIG_LWIP_LAYER */
/****************************** inic config end *******************************/

#ifdef CONFIG_RTK_MESH
#define CONFIG_BEACON_PERIOD 1024
#else
#define CONFIG_BEACON_PERIOD 100
#endif

#define CONFIG_ACM_METHOD 0	// 0:By SW 1:By HW.

#endif //WLANCONFIG_H
