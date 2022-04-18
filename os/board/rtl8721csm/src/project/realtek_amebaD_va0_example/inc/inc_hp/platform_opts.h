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
 * For AT cmd Log service configurations
 */
#define SUPPORT_LOG_SERVICE	0
#if SUPPORT_LOG_SERVICE
#define LOG_SERVICE_BUFLEN     100 //can't larger than UART_LOG_CMD_BUFLEN(127)
#define CONFIG_LOG_HISTORY	0
#if CONFIG_LOG_HISTORY
#define LOG_HISTORY_LEN    5
#endif
#define CONFIG_ATCMD_MP				1 //support MP AT command
#define SUPPORT_INTERACTIVE_MODE		0//on/off wifi_interactive_mode
#define CONFIG_LOG_SERVICE_LOCK 0
#endif

/**
 * For interactive mode configurations, depents on log service
 */
#if defined(SUPPORT_INTERACTIVE_MODE) && (SUPPORT_INTERACTIVE_MODE==1)
#define CONFIG_INTERACTIVE_MODE     1
#define CONFIG_INTERACTIVE_EXT   0
#else
#define CONFIG_INTERACTIVE_MODE     0
#define CONFIG_INTERACTIVE_EXT   0
#endif

/**
 * For FreeRTOS tickless configurations
 */
#define FREERTOS_PMU_TICKLESS_SUSPEND_SDRAM  0   // In sleep mode, 1: suspend SDRAM, 0: no act

/******************************************************************************/

/**
* For common flash usage  
*/
#define AP_SETTING_SECTOR		0x000FE000
#define UART_SETTING_SECTOR		0x000FC000
#define SPI_SETTING_SECTOR		0x000FC000
#if defined(CONFIG_BAIDU_DUER) && CONFIG_BAIDU_DUER
#define FAST_RECONNECT_DATA 	0x1FF000
#else
#define FAST_RECONNECT_DATA 	0x105000
#endif
#define CONFIG_ENABLE_RDP		0

/**
 * For Wlan configurations
 */
#define CONFIG_WLAN	1
#if CONFIG_WLAN
#define CONFIG_INIT_NET		0 //init lwip layer when start up
#define CONFIG_WIFI_IND_USE_THREAD	0	// wifi indicate worker thread

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_WEBSERVER	0
#define CONFIG_OTA_UPDATE	0
#define CONFIG_BSD_TCP		0//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size
#define CONFIG_AIRKISS		0//on or off tencent airkiss
#define CONFIG_UART_SOCKET	0
#define CONFIG_JOYLINK			0//on or off for jdsmart or joylink
#define CONFIG_QQ_LINK		0//on or off for qqlink
#define CONFIG_UART_XMODEM	0//support uart xmodem upgrade or not
#define CONFIG_GOOGLE_NEST	0//on or off the at command control for google nest
#define CONFIG_TRANSPORT	0//on or off the at command for transport socket
#define CONFIG_ALINK			0//on or off for alibaba alink
#define CONFIG_HILINK			0//on or off for huawei hilink

//#define CONFIG_BAIDU_DUER	1
#define CONFIG_DMIC_SEL	0
/* For WPS and P2P */
#define CONFIG_ENABLE_WPS		0
#define CONFIG_ENABLE_P2P		0//on/off p2p cmd in log_service or interactive mode
#define CONFIG_ENABLE_WPS_DISCOVERY	0
#if CONFIG_ENABLE_P2P
#define CONFIG_ENABLE_WPS_AP		1
#undef CONFIG_WIFI_IND_USE_THREAD
#define CONFIG_WIFI_IND_USE_THREAD	1
#endif
#if (CONFIG_ENABLE_P2P && ((CONFIG_ENABLE_WPS_AP == 0) || (CONFIG_ENABLE_WPS == 0)))
#error "If CONFIG_ENABLE_P2P, need to define CONFIG_ENABLE_WPS_AP 1" 
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

/* For Simple Link */
#define CONFIG_INCLUDE_SIMPLE_CONFIG		0

/*For fast reconnection*/
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	0

/*For wowlan service settings*/
#define CONFIG_WOWLAN_SERVICE           			0

#define CONFIG_GAGENT			0
/*Disable CONFIG_EXAMPLE_WLAN_FAST_CONNECT when CONFIG_GAGENT is enabled,because
	reconnect to previous AP is not suitable when re-configuration. 
*/
#if CONFIG_GAGENT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#endif

#define CONFIG_JOINLINK    0

#endif //end of #if CONFIG_WLAN
/*******************************************************************************/

/**
 * For Ethernet configurations
 */
#define CONFIG_ETHERNET 0
#if CONFIG_ETHERNET

#define CONFIG_INIT_NET         1 //init lwip layer when start up

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_BSD_TCP		0//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size

#endif

/**
 * For user to adjust SLEEP_INTERVAL
 */
#define CONFIG_DYNAMIC_TICKLESS  0
/*******************************************************************************/

/**
 * For iNIC configurations
 */
//#define CONFIG_INIC_EN 0//enable iNIC mode
#if defined(CONFIG_INIC_EN) && CONFIG_INIC_EN
#ifndef CONFIG_INIC_SDIO_HCI
#define CONFIG_INIC_SDIO_HCI	0 //for SDIO or USB iNIC
#endif
#ifndef CONFIG_INIC_CMD_RSP
#define CONFIG_INIC_CMD_RSP	0
#endif
#ifndef CONFIG_INIC_USB_HCI
#define CONFIG_INIC_USB_HCI	0
#endif
#endif
/******************End of iNIC configurations*******************/
/* For Amazon FreeRTOS SDK example */
#define CONFIG_EXAMPLE_AMAZON_FREERTOS   0

/* For aj_basic_example */
#define CONFIG_EXAMPLE_AJ_BASIC          0

/*For aj_ameba_led example*/
#define CONFIG_EXAMPLE_AJ_AMEBA_LED      0

/* For WIFI GET BEACON FRAME example */
#define CONFIG_EXAMPLE_GET_BEACON_FRAME  0

/* For WIFI MAC MONITOR example */
#define CONFIG_EXAMPLE_WIFI_MAC_MONITOR  0
   
/* For HTTP CLIENT example */
#define CONFIG_EXAMPLE_HTTP_CLIENT  0

/* For MQTT example */
#define CONFIG_EXAMPLE_MQTT				0

/* For WiGadget example */
#define CONFIG_EXAMPLE_WIGADGET			0

/*For google nest example*/
#define CONFIG_EXAMPLE_GOOGLE_NEST		0

/* For mDNS example */
#define CONFIG_EXAMPLE_MDNS				0

/* For multicast example */
#define CONFIG_EXAMPLE_MCAST			0

/* For XML example */
#define CONFIG_EXAMPLE_XML				0

/* For socket select example */
#define CONFIG_EXAMPLE_SOCKET_SELECT	0

/* For socket nonblocking connect example */
#define CONFIG_EXAMPLE_NONBLOCK_CONNECT	0

/* For socket TCP bidirectional transmission example */
#define CONFIG_EXAMPLE_SOCKET_TCP_TRX	0

/* For ssl download example */
#define CONFIG_EXAMPLE_SSL_DOWNLOAD      0

/* For http download example */
#define CONFIG_EXAMPLE_HTTP_DOWNLOAD	0

/* For http ota update example */
#define CONFIG_EXAMPLE_OTA_HTTP	0

/* For https ota update example */
#define CONFIG_EXAMPLE_OTA_HTTPS	0

/* For tcp keepalive example */
#define CONFIG_EXAMPLE_TCP_KEEPALIVE	0

/* For sd card ota update example */
#define CONFIG_EXAMPLE_OTA_SDCARD    0

/* For sntp show time example */
#define CONFIG_EXAMPLE_SNTP_SHOWTIME	0

/* For pppoe example */
#define CONFIG_EXAMPLE_PPPOE            0

/* For websocket client example */
#define CONFIG_EXAMPLE_WEBSOCKET		0

/*For promisc softap mode example */
#define CONFIG_EXAMPLE_PROMISC_SOFTAP_CONFIG 0

/* For wifi roaming examples */
#define CONFIG_EXAMPLE_WIFI_ROAMING_PLUS 0

/*For Audio example */
#define CONFIG_EXAMPLE_AUDIO			0

/* For dct example */
#define CONFIG_EXAMPLE_DCT			0

#if CONFIG_EXAMPLE_AUDIO
#define FATFS_DISK_SD 	1
#define CONFIG_EXAMPLE_CODEC_SGTL5000         0
#define CONFIG_EXAMPLE_CODEC_ALC5651	      1
#endif

#define CONFIG_EXAMPLE_AUDIO_MP3		0
#if CONFIG_EXAMPLE_AUDIO_MP3
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_AMR		0
#if CONFIG_EXAMPLE_AUDIO_AMR
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_AMR_FLASH 0

#define CONFIG_EXAMPLE_AUDIO_AC3		0
#ifdef CONFIG_EXAMPLE_AUDIO_AC3	
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_HELIX_AAC	0

#define CONFIG_EXAMPLE_AUDIO_HELIX_MP3	0

#define CONFIG_EXAMPLE_AUDIO_PCM_UPLOAD	0

#define CONFIG_EXAMPLE_AUDIO_HLS		0
#if CONFIG_EXAMPLE_AUDIO_HLS
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_RECORDER	0
#if CONFIG_EXAMPLE_AUDIO_RECORDER
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_M4A_SELFPARSE		0
#if CONFIG_EXAMPLE_AUDIO_M4A_SELFPARSE
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_M4A			0
#if CONFIG_EXAMPLE_AUDIO_M4A
#define FATFS_DISK_SD	1
#endif

#define CONFIG_EXAMPLE_AUDIO_FLAC		0
#if CONFIG_EXAMPLE_AUDIO_FLAC
#define FATFS_DISK_SD	1
#endif

#define AUDIO_SIGNAL_GENERATE       0

#define CONFIG_EXAMPLE_EKHO  0

#define CONFIG_EXAMPLE_AUDIO_TTS        0

#define CONFIG_EXAMPLE_TTS_CYBERON  0

#define CONFIG_EXAMPLE_AUDIO_OPUS_DECODE 0

#define CONFIG_EXAMPLE_AUDIO_OPUS_ENCODE 0
/* For UART Module AT command example */
#define CONFIG_EXAMPLE_UART_ATCMD	0
#if CONFIG_EXAMPLE_UART_ATCMD
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE 1
#undef CONFIG_TRANSPORT
#define CONFIG_TRANSPORT 1
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN 1600
#undef CONFIG_LOG_SERVICE_LOCK
#define CONFIG_LOG_SERVICE_LOCK 1
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT  0
#endif

/* For SPI Module AT command example */
#define CONFIG_EXAMPLE_SPI_ATCMD 0

#if CONFIG_EXAMPLE_SPI_ATCMD
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE 1
#undef CONFIG_TRANSPORT
#define CONFIG_TRANSPORT 1
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN 1600
#undef CONFIG_LOG_SERVICE_LOCK
#define CONFIG_LOG_SERVICE_LOCK 1
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT  0
#endif

/* For uvc example */
#ifdef CONFIG_UVC
/*for uvc_FATFS feature*/
#define CONFIG_UVC_SD_EN			0
#if CONFIG_UVC_SD_EN
#define CONFIG_FATFS_EN	1
#if CONFIG_FATFS_EN
// fatfs version
#define FATFS_R_10C
// fatfs disk interface
#define FATFS_DISK_USB	0
#define FATFS_DISK_SD 	1
#endif
#endif
#endif

#define CONFIG_EXAMPLE_MEDIA_FRAMEWORK					        0

#define CONFIG_EXAMPLE_MEDIA_SS 				0
#define CONFIG_EXAMPLE_MEDIA_MS					0
#define CONFIG_EXAMPLE_MEDIA_GEO_RTP                            0
// Use media source/sink example
#if (CONFIG_EXAMPLE_MEDIA_SS==1) || (CONFIG_EXAMPLE_MEDIA_MS==1)
#undef CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG		0
#define CONFIG_ENABLE_WPS	0
#endif  

/* For Mjpeg capture example*/
#define CONFIG_EXAMPLE_MJPEG_CAPTURE		0
#if CONFIG_EXAMPLE_MJPEG_CAPTURE
#define FATFS_DISK_SD 	1
#endif

/****************** For EAP method example *******************/
#define CONFIG_EXAMPLE_EAP	1

// on/off specified eap method
#define CONFIG_ENABLE_PEAP	1
#define CONFIG_ENABLE_TLS	1
#define CONFIG_ENABLE_TTLS	1
#define CONFIG_ENABLE_FAST	0

// optional feature: whether to verify the cert of radius server
#define ENABLE_EAP_SSL_VERIFY_SERVER	0

#if CONFIG_ENABLE_PEAP || CONFIG_ENABLE_TLS || CONFIG_ENABLE_TTLS || CONFIG_ENABLE_FAST
#define CONFIG_ENABLE_EAP
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#endif

#if CONFIG_ENABLE_TLS
#define ENABLE_EAP_SSL_VERIFY_CLIENT	1
#else
#define ENABLE_EAP_SSL_VERIFY_CLIENT	0
#endif
/******************End of EAP configurations*******************/

/* For usb mass storage example */
#define CONFIG_EXAMPLE_USB_MASS_STORAGE		0

/* For usb device isoc example */
#define CONFIG_EXAMPLE_USB_ISOC_DEVICE			0
/* For usb host vendor specific  example */
#define CONFIG_EXAMPLE_USB_VENDOR_SPECIFIC		0

/* For FATFS example*/
#define CONFIG_EXAMPLE_FATFS			0
#if CONFIG_EXAMPLE_FATFS
#define CONFIG_FATFS_EN	1
#if CONFIG_FATFS_EN
// fatfs version
#define FATFS_R_10C
// fatfs disk interface
#define FATFS_DISK_USB	0
#define FATFS_DISK_SD 	1
#define FATFS_DISK_FLASH 	0
#endif
#endif

/* For iNIC host example*/
#ifdef CONFIG_INIC_GSPI_HOST //this flag is defined in IAR project
#define	CONFIG_EXAMPLE_INIC_GSPI_HOST	1
#if CONFIG_EXAMPLE_INIC_GSPI_HOST

#define CONFIG_INIC_HOST	1

#undef CONFIG_WLAN
#define CONFIG_WLAN		0
#undef	CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG	0
#undef	CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	0
#undef	CONFIG_BSD_TCP
#define CONFIG_BSD_TCP		1

#endif
#endif

/*For uart update example*/
#define CONFIG_UART_UPDATE				0
#if CONFIG_UART_UPDATE
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#endif

/*For arduino wifi shield example */
#define CONFIG_EXAMPLE_ARDUINO_WIFI	0
#if CONFIG_EXAMPLE_ARDUINO_WIFI
#undef CONFIG_WIFI_NORMAL
#endif

/* For uart adapter example */
/* Please also configure LWIP_UART_ADAPTER to 1 
in lwip_opt.h for support uart adapter*/
#define CONFIG_EXAMPLE_UART_ADAPTER	0
#if CONFIG_EXAMPLE_UART_ADAPTER
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	1
#undef CONFIG_EXAMPLE_MDNS
#define CONFIG_EXAMPLE_MDNS	1
#endif

#if CONFIG_EXAMPLE_PROMISC_SOFTAP_CONFIG
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	1
#endif

/* For wifi scenarios example (Wi-Fi, WPS enrollee, P2P GO) */
// also need to enable WPS and P2P
#define CONFIG_EXAMPLE_WLAN_SCENARIO	0
	 
/* For broadcast example */
#define CONFIG_EXAMPLE_BCAST			0

/* For high-load memory use case memory usage */
#define CONFIG_EXAMPLE_HIGH_LOAD_MEMORY_USE		0

/* For rarp example */
#define CONFIG_EXAMPLE_RARP				0

/* For ssl server example */
#define CONFIG_EXAMPLE_SSL_SERVER		0

#if CONFIG_QQ_LINK
#define FATFS_R_10C
#define FATFS_DISK_USB	0
#define FATFS_DISK_SD 	1
#endif

#if CONFIG_ENABLE_WPS
#define WPS_CONNECT_RETRY_COUNT		4
#define WPS_CONNECT_RETRY_INTERVAL	5000 // in ms
#endif 

#define AUTO_RECONNECT_COUNT	8
#define AUTO_RECONNECT_INTERVAL	5 // in sec

#if defined(CONFIG_INIC_EN) && CONFIG_INIC_EN
#undef CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG	0
#define SUPPORT_INTERACTIVE_MODE		0
#define CONFIG_INTERACTIVE_MODE		0
#define CONFIG_INTERACTIVE_EXT			0
#define CONFIG_OTA_UPDATE				0
#endif

#if defined(CONFIG_USBD_AUDIO)
#define CONFIG_EXAMPLE_USBD_AUDIO	1
//#define CONFIG_EXAMPLE_COMPETITIVE_HEADPHONES_DONGLE	1
#endif

#if defined(CONFIG_USBD_MSC)
#define CONFIG_EXAMPLE_USBD_MSC         1
#endif

#if defined(CONFIG_USBD_CDC_ACM)
#define CONFIG_EXAMPLE_USBD_CDC_ACM     1
#endif

//#define CONFIG_EXAMPLE_COMPETITIVE_HEADPHONES		1

/* For fast DHCP */
#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_FAST_DHCP 1
#else
#define CONFIG_FAST_DHCP 0
#endif

#endif
