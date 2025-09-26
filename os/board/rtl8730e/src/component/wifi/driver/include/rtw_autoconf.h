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
#include "platform_opts.h"

#define CONFIG_LITTLE_ENDIAN


//#define CONFIG_HIGH_TP

#define WIFI_LOGO_CERTIFICATION 0
#define RX_AMSDU

/* no IOT chip supports 80M now, so close it in common */
#define NOT_SUPPORT_80M
#define CONFIG_AUTO_RECONNECT 0

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
#elif defined(CONFIG_AMEBASMART)
/******************************* AmebaD2 (8730E) ******************************/
#include "autoconf_8730e.h"
#elif defined(CONFIG_AMEBALITE)
/***************************** AmebaLite (8720E) *****************************/
#include "autoconf_8720e.h"
#elif defined(CONFIG_AMEBAGREEN2) || defined(CONFIG_AMEBAPRO3)
/******************************* Amebalite2 (8720f) ******************************/
#include "autoconf_8720f.h"

#endif
/****************** Configurations for each platform end **********************/


/************************ For EAP auth configurations *************************/
#include "autoconf_eap.h"
/************************ For EAP auth configurations *************************/
/* KVR macro is default opened, but actually not working. To use it, need turn on the switch in menuconfig */
#define CONFIG_IEEE80211V
#define CONFIG_IEEE80211R
#define CONFIG_IEEE80211K
#if WIFI_LOGO_CERTIFICATION
/* 80211 - K MBO */
#define CONFIG_RTW_MBO
#endif

#define CONFIG_BEACON_PERIOD 100

#define CONFIG_ACM_METHOD 0	// 0:By SW 1:By HW.

/* for defrag in rom, remove after rom code freeze */
#if (defined(CONFIG_AMEBAGREEN2) && CONFIG_AMEBAGREEN2) || (defined(CONFIG_AMEBAPRO3) && CONFIG_AMEBAPRO3)
#define CONFIG_FRAME_DEFRAG // support frame defragmentaion
#else
//#define CONFIG_FRAME_DEFRAG // support frame defragmentaion
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

#ifdef CONFIG_AS_INIC_NP
#define INIC_SKIP_NP_MSG_TASK
#endif

#endif //WLANCONFIG_H
