/**
 ******************************************************************************
 *This file contains general configurations for ameba platform
 ******************************************************************************
*/

#ifndef __PLATFORM_OPTS_H__
#define __PLATFORM_OPTS_H__

#include "platform_autoconf.h"
#if defined CONFIG_BT && CONFIG_BT
#include "platform_opts_bt.h"
#endif
/*For MP mode setting*/
//#define SUPPORT_MP_MODE		1

/**
* For common flash usage
*/
#define AP_SETTING_SECTOR		0x000FE000
#define UART_SETTING_SECTOR	0x000FC000
#define SPI_SETTING_SECTOR		0x000FC000
#define FAST_RECONNECT_DATA 	0x0001E000
#define CONFIG_ENABLE_RDP		0

/**
 * For Wlan configurations
 */
#ifdef CONFIG_WLAN
#define CONFIG_LWIP_LAYER	1
#define CONFIG_INIT_NET		1 //init lwip layer when start up

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_WEBSERVER	0
#define CONFIG_OTA_UPDATE	0
#define CONFIG_BSD_TCP		1//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size
#define CONFIG_AIRKISS		0//on or off tencent airkiss
#define CONFIG_UART_SOCKET	0
#define CONFIG_UART_XMODEM	0//support uart xmodem upgrade or not
#define CONFIG_GOOGLE_NEST	0//on or off the at command control for google nest
#define CONFIG_TRANSPORT	0//on or off the at command for transport socket

#define CONFIG_DMIC_SEL	0
/* For WPS and P2P */
#define CONFIG_ENABLE_WPS		1
#define CONFIG_ENABLE_P2P		0//on/off p2p cmd in log_service or interactive mode
#define CONFIG_ENABLE_WPS_DISCOVERY	0

/* For WPS configurations*/
#if CONFIG_ENABLE_WPS
#define WPS_CONNECT_RETRY_COUNT		4
#define WPS_CONNECT_RETRY_INTERVAL	5000 // in ms
#endif

/* For SSL/TLS */
#define CONFIG_USE_POLARSSL     0
#define CONFIG_USE_MBEDTLS      1
#if ((CONFIG_USE_POLARSSL == 0) && (CONFIG_USE_MBEDTLS == 0)) || ((CONFIG_USE_POLARSSL == 1) && (CONFIG_USE_MBEDTLS == 1))
#undef CONFIG_USE_POLARSSL
#define CONFIG_USE_POLARSSL 1
#undef CONFIG_USE_MBEDTLS
#define CONFIG_USE_MBEDTLS 0
#endif
#define CONFIG_SSL_CLIENT_PRIVATE_IN_TZ 0

/* For fast DHCP when fast connect*/
#define CONFIG_FAST_DHCP 0
#define CONFIG_GAGENT            0

#endif //end of #if CONFIG_WLAN
/*******************************************************************************/

#if defined (CONFIG_AS_INIC_NP)
#undef SUPPORT_LOG_SERVICE
#undef CONFIG_ATCMD_MP
#define CONFIG_ATCMD_MP				0 //support MP AT command
#undef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER	0
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE				0
#endif

#if defined (CONFIG_SINGLE_CORE_WIFI)
#undef SUPPORT_LOG_SERVICE
#define SUPPORT_LOG_SERVICE 1
#endif

/**
 * For AT cmd Log service configurations
 */
#ifdef SUPPORT_LOG_SERVICE
#define LOG_SERVICE_BUFLEN     100 //can't larger than UART_LOG_CMD_BUFLEN(127)
#define CONFIG_ATCMD_MP				1 //support MP AT command
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

/******************************************************************************/

/**
 * For Ethernet configurations
 */
#define CONFIG_ETHERNET 0
#if CONFIG_ETHERNET

#define CONFIG_LWIP_LAYER	1
#define CONFIG_INIT_NET         1 //init lwip layer when start up

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_BSD_TCP		0//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size

#endif

/**
 * For user to adjust SLEEP_INTERVAL
 */
#define CONFIG_DYNAMIC_TICKLESS  1

/*******************************************************************************/

/**
 * For EAP configurations
 */

// on/off specified eap method
#define CONFIG_ENABLE_PEAP	0
#define CONFIG_ENABLE_TLS	0
#define CONFIG_ENABLE_TTLS	0

// optional feature: whether to verify the cert of radius server
#define ENABLE_EAP_SSL_VERIFY_SERVER	0

#if CONFIG_ENABLE_PEAP || CONFIG_ENABLE_TLS || CONFIG_ENABLE_TTLS
#define CONFIG_ENABLE_EAP
#endif

#if CONFIG_ENABLE_TLS
#define ENABLE_EAP_SSL_VERIFY_CLIENT	1
#else
#define ENABLE_EAP_SSL_VERIFY_CLIENT	0
#endif

/*******************************************************************************/

#endif //__PLATFORM_OPTS_H__
