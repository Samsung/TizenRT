/**
  ******************************************************************************
  * @file    rtw_autoconf.h
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

#ifndef WLANCONFIG_H
#define WLANCONFIG_H

/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you dont like!
 */
#include "platform_autoconf.h"

#define CONFIG_LITTLE_ENDIAN


//#define CONFIG_HIGH_TP

#define WIFI_LOGO_CERTIFICATION 0
#define RX_AMSDU

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define CONFIG_AUTO_RECONNECT 0
#else
#define CONFIG_AUTO_RECONNECT 1
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

/* For WPA3 */
#define CONFIG_IEEE80211W
#define CONFIG_OWE_SUPPORT
#define CONFIG_SAE_SUPPORT
#ifdef CONFIG_SAE_SUPPORT
#define CONFIG_SAE_DH_SUPPORT 1
#endif

#define CONFIG_PS_EN

/* For promiscuous mode */
// #define CONFIG_PROMISC

/* For WPS and P2P */
#define CONFIG_WPS

/******************* Ameba Series Common Configurations ***********************/
/*PHYDM version*/
#define OUTSRC	1
#define PHYDM	2
#define HALBBRF	3

#if defined(CONFIG_AMEBADPLUS)
/******************************* AmebaDPLUS (8721DA) *******************************/
#include "autoconf_8721da.h"
#elif defined(CONFIG_AMEBAD)
/******************************* AmebaD2 (8730E) ******************************/
#include "autoconf_8721d.h"
#elif defined(CONFIG_AMEBASMART)
/******************************* AmebaD2 (8730E) ******************************/
#include "autoconf_8730e.h"
#elif defined(CONFIG_AMEBALITE)
/***************************** AmebaLite (8720E) *****************************/
#include "autoconf_8720e.h"
#elif defined(CONFIG_AMEBAGREEN2)
/******************************* Amebalite2 (8721f) ******************************/
#include "autoconf_8721f.h"
#elif defined(CONFIG_AMEBAPRO3)
/******************************* AmebaPro3 (8735c) ******************************/
#include "autoconf_8721f.h"
#elif defined(CONFIG_AMEBAL2)
/******************************* AmebaL2 (6955) ******************************/
#include "autoconf_amebax.h"
#elif defined(CONFIG_RTL8720F)
/******************************* Ameba (8720F) ******************************/
#include "autoconf_8720f.h"
#endif
/****************** Configurations for each platform end **********************/


/************************ For EAP auth configurations *************************/
/* wpa_supplicant_std handles EAP configuration in its own config.
 * Do not include autoconf_eap.h to avoid configuration conflicts.
 */
#ifndef CONFIG_WPA_STD
#include "autoconf_eap.h"
#endif
/************************ For EAP auth configurations *************************/
/* KVR macro is default opened, but actually not working. To use it, need turn on the switch in menuconfig */
#ifndef CONFIG_PLATFORM_TIZENRT_OS
// Currently not used in TizenRT
#define CONFIG_IEEE80211V
#define CONFIG_IEEE80211R
#define CONFIG_IEEE80211K
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if WIFI_LOGO_CERTIFICATION
/* 80211 - K MBO */
#define CONFIG_RTW_MBO
#endif

#define CONFIG_BEACON_PERIOD 100

#define CONFIG_ACM_METHOD 0	// 0:By SW 1:By HW.

#if !defined(CONFIG_AMEBAL2) && !defined(CONFIG_RTL8720F)
#define CONFIG_FRAME_DEFRAG // support frame defragmentaion
#endif

#ifdef CONFIG_MP_INCLUDED
#endif

#ifdef CONFIG_MP_SHRINK
#undef CONFIG_PS_EN
#undef CONFIG_AUTO_RECONNECT
#undef CONFIG_IEEE80211W
#undef CONFIG_OWE_SUPPORT
#undef CONFIG_SAE_SUPPORT
#undef CONFIG_SAE_DH_SUPPORT
#undef CONFIG_IEEE80211V
#undef CONFIG_IEEE80211R
#undef CONFIG_IEEE80211K
#undef CONFIG_RTW_MBO

#define CONFIG_AUTO_RECONNECT 0
#endif

#ifdef CONFIG_WHC_DEV
#define WHC_SKIP_NP_MSG_TASK
#endif

#ifdef NAN_CUSTOMER_NANDOW
#define MAX_NANDOW_PARA_LEN 2600
#endif

/* When using supplicant SME, 11R is supported by default, instead of reuse the path of RTOS 11R */
#ifdef CONFIG_SUPPLICANT_SME
#undef CONFIG_IEEE80211R
#endif

#endif //WLANCONFIG_H
