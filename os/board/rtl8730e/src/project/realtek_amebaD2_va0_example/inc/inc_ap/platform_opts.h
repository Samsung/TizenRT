/**
 ******************************************************************************
 *This file contains general configurations for ameba platform
 ******************************************************************************
*/

#ifndef __PLATFORM_OPTS_H__
#define __PLATFORM_OPTS_H__

#include "platform_autoconf.h"
/*For MP mode setting*/
//#define SUPPORT_MP_MODE		1

/******************************************************************************/

/**
* For common flash usage
*/
#define AP_SETTING_SECTOR		0x000FE000
#define UART_SETTING_SECTOR		0x000FC000
#define SPI_SETTING_SECTOR		0x000FC000
#define FAST_RECONNECT_DATA 	0x0001E000

#define CONFIG_ENABLE_RDP		0


/*******************************************************************************/

/**
 * For AT cmd Log service configurations
 */

#if defined (CONFIG_AS_INIC_AP)
#undef SUPPORT_LOG_SERVICE
#define SUPPORT_LOG_SERVICE 1
#define CONFIG_INIC_IPC_TODO 1
#define CONFIG_STDLIB_TODO 1
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE		0
#endif

#ifdef SUPPORT_LOG_SERVICE
#define LOG_SERVICE_BUFLEN     100 //can't larger than UART_LOG_CMD_BUFLEN(127)
#define CONFIG_ATCMD_MP			1 //support MP AT command
#define CONFIG_LOG_SERVICE_LOCK 0
#endif

/* For UART Module AT command */
#define SUPPORT_UART_LOG_SERVICE	0
#if SUPPORT_UART_LOG_SERVICE
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE 1
#undef CONFIG_TRANSPORT
#define CONFIG_TRANSPORT 1
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN 1600
#undef CONFIG_LOG_SERVICE_LOCK
#define CONFIG_LOG_SERVICE_LOCK 1
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define ATVER_1 1 // For First AT command
#define ATVER_2 2 // For UART Module AT command
#if SUPPORT_UART_LOG_SERVICE
#define ATCMD_VER ATVER_2
#else
#define ATCMD_VER ATVER_1
#endif
#endif

/*******************************************************************************/

/**
 * For user to adjust SLEEP_INTERVAL
 */
#define CONFIG_DYNAMIC_TICKLESS  1

/*******************************************************************************/

/*******************************************************************************/
/**
 *For uvc configurations
 */

#ifdef CONFIG_UVC
/*for uvc_FATFS feature*/
#define CONFIG_UVC_SD_EN			0
#endif

/*******************************************************************************/

#endif
