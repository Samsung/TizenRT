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

#ifndef CONFIG_PLATFORM_AMEBA_X
#define CONFIG_PLATFORM_AMEBA_X 1
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
#endif


/* For promiscuous mode */
// #define CONFIG_PROMISC

/* For WPS and P2P */
#define CONFIG_WPS

/************************** config to support chip ****************************/
#define RTL8723B_SUPPORT 0
#define RTL8192E_SUPPORT 0
#define RTL8188E_SUPPORT 0
#define RTL8188F_SUPPORT 0
#define RTL8720E_SUPPORT 0
#define RTL8720F_SUPPORT 0
#define RTL8721D_SUPPORT 0
#define RTL8723D_SUPPORT 0
#define RTL8195B_SUPPORT 0
#define RTL8710C_SUPPORT 0
#define RTL8730A_SUPPORT 0
#define RTL8730E_SUPPORT 0
#define RTL8721F_SUPPORT 0
#define RTL8735B_SUPPORT 0
/************************ config to support chip end **************************/

/******************** Configurations for each platform ************************/
#if (CONFIG_PLATFORM_AMEBA_X == 1)
/******************* Ameba Series Common Configurations ***********************/
/*PHYDM version*/
#define OUTSRC	1
#define PHYDM	2
#define HALBBRF	3

#if defined(CONFIG_PLATFORM_8721D)
/******************************* AmebaD (8721D) *******************************/
#include "autoconf_8721d.h"
#elif defined(CONFIG_PLATFORM_AMEBADPLUS)
/******************************* AmebaDPLUS (8721F) *******************************/
#include "autoconf_8721f.h"
#elif defined(CONFIG_PLATFORM_8735B)
/***************************** AmebaPro2 (8735B) ******************************/
#include "autoconf_8735b.h"
#elif defined(CONFIG_PLATFORM_AMEBAD2)
/******************************* AmebaD2 (8730E) ******************************/
#include "autoconf_8730e.h"
#elif defined(CONFIG_PLATFORM_AMEBALITE)
/***************************** AmebaLite (8720E) *****************************/
#include "autoconf_8720e.h"
#elif defined(CONFIG_PLATFORM_AMEBAZ6)	/* TODO_claire, just for compile costdown IC */
#include "autoconf_amebaz6.h"

#elif defined(CONFIG_PLATFORM_RTL8720F)
/******************************* Amebalite2 (8720f) ******************************/
#include "autoconf_8720f.h"

#endif

/****************************** Ameba Series End ******************************/
#endif /* (CONFIG_PLATFORM_AMEBA_X == 1) */
/****************** Configurations for each platform end **********************/


/************************ For EAP auth configurations *************************/
#include "autoconf_eap.h"
/************************ For EAP auth configurations *************************/

#if WIFI_LOGO_CERTIFICATION
#undef WLAN_MAX_ETHFRM_LEN
#define WLAN_MAX_ETHFRM_LEN	4000
/* 80211 - K MBO */
#define CONFIG_RTW_MBO
#define CONFIG_IEEE80211K
#ifndef CONFIG_LINUX_FW_EN
#define CONFIG_LAYER2_ROAMING
#endif
#endif

/* 80211 - V R */
#ifdef CONFIG_LAYER2_ROAMING
#define CONFIG_RTW_WNM
#define CONFIG_IEEE80211R
#endif

#define CONFIG_BEACON_PERIOD 100

#define CONFIG_ACM_METHOD 0	// 0:By SW 1:By HW.

#endif //WLANCONFIG_H
