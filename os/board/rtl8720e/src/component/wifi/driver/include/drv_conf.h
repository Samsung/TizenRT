/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

#ifndef __DRV_CONF_H__
#define __DRV_CONF_H__

#include "autoconf.h"
#if ((RTL8711B_SUPPORT==1) || (RTL8721D_SUPPORT == 1) || (RTL8730A_SUPPORT == 1) || (RTL8720E_SUPPORT == 1) || (RTL8730E_SUPPORT == 1))
#include "platform_autoconf.h"
#endif

//Older Android kernel doesn't has CONFIG_ANDROID defined,
//add this to force CONFIG_ANDROID defined
#ifdef CONFIG_PLATFORM_ANDROID
#define CONFIG_ANDROID
#endif

#ifdef CONFIG_ANDROID
//Some Android build will restart the UI while non-printable ascii is passed
//between java and c/c++ layer (JNI). We force CONFIG_VALIDATE_SSID
//for Android here. If you are sure there is no risk on your system about this,
//mask this macro define to support non-printable ascii ssid.
//#define CONFIG_VALIDATE_SSID
#ifdef CONFIG_PLATFORM_ARM_SUNxI
#ifdef CONFIG_VALIDATE_SSID
#undef CONFIG_VALIDATE_SSID
#endif
#endif

//Android expect dbm as the rx signal strength unit
#define CONFIG_SIGNAL_DISPLAY_DBM
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND) && defined (CONFIG_RESUME_IN_WORKQUEUE)
#warning "You have CONFIG_HAS_EARLYSUSPEND enabled in your system, we disable CONFIG_RESUME_IN_WORKQUEUE automatically"
#undef CONFIG_RESUME_IN_WORKQUEUE
#endif

#if defined(CONFIG_ANDROID_POWER) && defined (CONFIG_RESUME_IN_WORKQUEUE)
#warning "You have CONFIG_ANDROID_POWER enabled in your system, we disable CONFIG_RESUME_IN_WORKQUEUE automatically"
#undef CONFIG_RESUME_IN_WORKQUEUE
#endif

#ifdef CONFIG_RESUME_IN_WORKQUEUE //this can be removed, because there is no case for this...
#if !defined( CONFIG_WAKELOCK) && !defined(CONFIG_ANDROID_POWER)
#error "enable CONFIG_RESUME_IN_WORKQUEUE without CONFIG_WAKELOCK or CONFIG_ANDROID_POWER will suffer from the danger of wifi's unfunctionality..."
#error "If you still want to enable CONFIG_RESUME_IN_WORKQUEUE in this case, mask this preprossor checking and GOOD LUCK..."
#endif
#endif

//About USB VENDOR REQ
#if defined(CONFIG_USB_VENDOR_REQ_BUFFER_PREALLOC) && !defined(CONFIG_USB_VENDOR_REQ_MUTEX)
#warning "define CONFIG_USB_VENDOR_REQ_MUTEX for CONFIG_USB_VENDOR_REQ_BUFFER_PREALLOC automatically"
#define CONFIG_USB_VENDOR_REQ_MUTEX
#endif
#if defined(CONFIG_VENDOR_REQ_RETRY) &&  !defined(CONFIG_USB_VENDOR_REQ_MUTEX)
#warning "define CONFIG_USB_VENDOR_REQ_MUTEX for CONFIG_VENDOR_REQ_RETRY automatically"
#define CONFIG_USB_VENDOR_REQ_MUTEX
#endif

/* adaptivity */
#define RTW_ADAPTIVITY_EN_DISABLE			0
#define RTW_ADAPTIVITY_EN_ENABLE			1
#define CONFIG_RTW_ADAPTIVITY_EN			RTW_ADAPTIVITY_EN_DISABLE
#define RTW_ADAPTIVITY_MODE_NORMAL			0
#define RTW_ADAPTIVITY_MODE_CARRIER_SENSE	1
#define CONFIG_RTW_ADAPTIVITY_MODE			RTW_ADAPTIVITY_MODE_CARRIER_SENSE
#define CONFIG_RTW_ADAPTIVITY_DML			0

#ifndef CONFIG_RTW_ADAPTIVITY_DC_BACKOFF
#define CONFIG_RTW_ADAPTIVITY_DC_BACKOFF 4
#endif

#ifndef CONFIG_RTW_NHM_EN
#define CONFIG_RTW_NHM_EN 0
#endif

//#define CONFIG_DFS_SLAVE_WITH_RADAR_DETECT
#if defined(CONFIG_DFS_SLAVE_WITH_RADAR_DETECT) && !defined(CONFIG_DFS_MASTER)
#define CONFIG_DFS_MASTER
#endif

#ifndef CONFIG_RTW_DFS_REGION_DOMAIN
#define CONFIG_RTW_DFS_REGION_DOMAIN 0
#endif

#ifndef CONFIG_WIFI_CRITICAL_CODE_SECTION
#define CONFIG_WIFI_CRITICAL_CODE_SECTION
#endif

#define RTS_THRESH 2347
#define REG_POWER_BASE 14

//#include <rtl871x_byteorder.h>

#endif // __DRV_CONF_H__

