/**
 ******************************************************************************
 *This file contains general configurations for ameba platform
 ******************************************************************************
*/

#ifndef __PLATFORM_OPTS_AP_H__
#define __PLATFORM_OPTS_AP_H__

/*For MP mode setting*/
//#define SUPPORT_MP_MODE		1

/**
 * For Wlan configurations
 */
#ifdef CONFIG_WLAN
#define CONFIG_LWIP_LAYER	1

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_OTA_UPDATE	0
#define CONFIG_BSD_TCP		1//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size
#define CONFIG_TRANSPORT	0//on or off the at command for transport socket

/* For WPS and P2P */
#define CONFIG_ENABLE_WPS		1
#define CONFIG_ENABLE_P2P		0//on/off p2p cmd in log_service or interactive mode
#define CONFIG_ENABLE_WPS_DISCOVERY	0
#if CONFIG_ENABLE_WPS
#define WPS_CONNECT_RETRY_COUNT		4
#define WPS_CONNECT_RETRY_INTERVAL	5000 // in ms
#endif

#endif //end of #if CONFIG_WLAN
/*******************************************************************************/

/* AT command */
#define ATVER_1 1 // For First AT command
#define ATVER_2 2 // For UART Module AT command

/**
 * For AT cmd Log service configurations
 */
#if defined (CONFIG_AS_INIC_AP)
#undef SUPPORT_LOG_SERVICE
#define SUPPORT_LOG_SERVICE 1
#define CONFIG_INIC_IPC_TODO 1
#define CONFIG_STDLIB_TODO 1
#endif

#if defined (CONFIG_SINGLE_CORE_WIFI)
#undef SUPPORT_LOG_SERVICE
#define SUPPORT_LOG_SERVICE 1
#endif

#ifdef SUPPORT_LOG_SERVICE
#define LOG_SERVICE_BUFLEN     100 //can't larger than UART_LOG_CMD_BUFLEN(127)
#define CONFIG_LOG_SERVICE_LOCK 0
#endif

/* For UART Module AT command example */
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

#define ATCMD_VER ATVER_2
#else
#define ATCMD_VER ATVER_1
#endif

/*******************************************************************************/

#define CONFIG_ATCMD_MP 1  //support MP AT command

#endif

