/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

#ifndef LWIP_HDR_LWIPOPTS_H
#define LWIP_HDR_LWIPOPTS_H

#include "platform_autoconf.h"
#include "platform_stdlib.h"
#include "basic_types.h"
#include "rand.h" //use soc _rand, not lib rand & srand, because stack size
#define WIFI_LOGO_CERTIFICATION_CONFIG 0    //for ping 10k test buffer setting

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    1

/* Define LWIP_COMPAT_MUTEX if the port has no mutexes and binary semaphores
 should be used instead */
#define LWIP_COMPAT_MUTEX       1

#define ETHARP_TRUST_IP_MAC     0
#define IP_REASSEMBLY           1
#define IP_FRAG                 1
#define ARP_QUEUEING            0

#if defined(CONFIG_IP_NAT) && (CONFIG_IP_NAT == 1)
#define IP_FORWARD              1
#define IP_NAT                  1
#endif

#ifndef CONFIG_LWIP_DHCP_COARSE_TIMER
#define CONFIG_LWIP_DHCP_COARSE_TIMER		0
#endif
/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define NO_SYS                  0

#ifndef CONFIG_DYNAMIC_TICKLESS
#define CONFIG_DYNAMIC_TICKLESS 0
#endif

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#if WIFI_LOGO_CERTIFICATION_CONFIG
#define MEM_SIZE                (20*1024) //for ping 10k test
#elif defined(CONFIG_ETHERNET) && CONFIG_ETHERNET
#define MEM_SIZE                (6*1024)  //for iperf test
#elif defined(CONFIG_AMEBASMART)
#define MEM_SIZE                (6*TCP_MSS)
#elif defined(CONFIG_AMEBALITE)
#define MEM_SIZE                 (6*TCP_MSS)
#elif defined(CONFIG_AMEBADPLUS)
#define MEM_SIZE                 (7*1024)
#elif defined(CONFIG_AMEBAGREEN2)
#define MEM_SIZE                 (6*TCP_MSS)
#elif defined(ENABLE_AMAZON_COMMON)
#define MEM_SIZE                (10*1024)
#else
#define MEM_SIZE                (5*1024)
#endif

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#if defined(CONFIG_AMEBALITE)
#define MEMP_NUM_PBUF           50
#elif defined(CONFIG_AMEBAGREEN2)
#define MEMP_NUM_PBUF           50
#elif defined(CONFIG_AMEBASMART)
#define MEMP_NUM_PBUF           50
#else
#define MEMP_NUM_PBUF           100
#endif
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        6
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#if defined(CONFIG_AMEBALITE)
#define MEMP_NUM_TCP_PCB        6
#elif defined(CONFIG_AMEBAGREEN2)
#define MEMP_NUM_TCP_PCB        6
#else
#define MEMP_NUM_TCP_PCB        10
#endif
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 5
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        20
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    10

#define MEMP_NUM_NETCONN        8

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#if WIFI_LOGO_CERTIFICATION_CONFIG
#define PBUF_POOL_SIZE          60 //for ping 10k test
#define ARP_MAXAGE			720// modify 5min to 12min, for wifi6 logo 5.64.1 AP not response arp request in step10
#define IP_REASS_MAXAGE		1
#elif defined(ENABLE_AMAZON_COMMON)
#define PBUF_POOL_SIZE          30
#else
#define PBUF_POOL_SIZE          20
#endif

/* IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled.*/
#if WIFI_LOGO_CERTIFICATION_CONFIG
#define IP_REASS_MAX_PBUFS              60 //for ping 10k test
#elif defined(ENABLE_AMAZON_COMMON)
#define IP_REASS_MAX_PBUFS              30 //for ping 10k test
#else
#define IP_REASS_MAX_PBUFS              10
#endif

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE       508


/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         1

/* TCP Maximum segment size. */
#define TCP_MSS                 (1500 - 40)	  /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             (5*TCP_MSS)
/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */
#define TCP_SND_QUEUELEN        (4* TCP_SND_BUF/TCP_MSS)


/* TCP receive window. */
#if defined(CONFIG_AMEBASMART)
#define TCP_WND                 (5*TCP_MSS)
#elif defined(CONFIG_AMEBADPLUS)
#define TCP_WND                 (5*TCP_MSS)
#elif defined(CONFIG_AMEBALITE)
#define TCP_WND                 (5*TCP_MSS)
#elif defined(CONFIG_AMEBAGREEN2)
#define TCP_WND                 (5*TCP_MSS)
#elif defined(ENABLE_AMAZON_COMMON)
#define TCP_WND                 (4*TCP_MSS)
#else
#define TCP_WND                 (2*TCP_MSS)
#endif

/* ---------- ICMP options ---------- */
#define LWIP_ICMP                       1

/* ---------- ARP options ----------- */
#define LWIP_ARP                        1

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               1


/* ---------- UDP options ---------- */
#define LWIP_UDP                1
#define UDP_TTL                 255
/* ---------- DNS options ---------- */
#define LWIP_DNS                1

/* ---------- UPNP options --------- */
#define LWIP_UPNP               0

/* ---------- SO_SNDRCVTIMEO_NONSTANDARD options --------- */
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 1

/* ---------- SO_REUSE options --------- */
#define SO_REUSE                        1

/* Support Multicast */
#define LWIP_IGMP                   1
#define LWIP_RAND()                 _rand()
#ifdef __cplusplus
extern "C" unsigned int sys_now(void);
#else
extern unsigned int sys_now(void);
#endif
//#define LWIP_SRAND()                srand(sys_now())
#define LWIP_SRAND()

#define LWIP_MTU_ADJUST 		1

/* Support TCP Keepalive */
#define LWIP_TCP_KEEPALIVE				1

/*LWIP_UART_ADAPTER==1: Enable LWIP_UART_ADAPTER when CONFIG_GAGENT is enabled,
   because some GAGENT functions denpond on the following macro definitions.*/
#define LWIP_UART_ADAPTER                   0

#if LWIP_UART_ADAPTER || (defined(CONFIG_ETHERNET) && CONFIG_ETHERNET)
#undef  LWIP_SO_SNDTIMEO
#define LWIP_SO_SNDTIMEO                		1

#undef  SO_REUSE
#define SO_REUSE                        			1

#undef MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN                	10

#undef TCP_WND
#define TCP_WND                                       (4*TCP_MSS)

#define TCP_KEEPIDLE_DEFAULT			10000UL
#define TCP_KEEPINTVL_DEFAULT			1000UL
#define TCP_KEEPCNT_DEFAULT			10U
#endif

#if defined(CONFIG_HIGH_TP_TEST)
#if defined(CONFIG_AMEBALITE) && defined (CONFIG_AS_INIC_AP)
#undef MEM_SIZE
#define MEM_SIZE                (26*TCP_MSS)

#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE          64

#undef TCP_SND_BUF
#define TCP_SND_BUF             (24*TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG        (4* TCP_SND_BUF/TCP_MSS)

#undef TCP_WND
#define TCP_WND                 (16*TCP_MSS)

#undef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF           100
#elif defined(CONFIG_AMEBAGREEN2) && defined (CONFIG_AS_INIC_AP)
#undef MEM_SIZE
#define MEM_SIZE                (26*TCP_MSS)

#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE          64

#undef TCP_SND_BUF
#define TCP_SND_BUF             (24*TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG        (4* TCP_SND_BUF/TCP_MSS)

#undef TCP_WND
#define TCP_WND                 (16*TCP_MSS)

#undef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF           100
#elif defined(CONFIG_AMEBASMART) && defined (CONFIG_AS_INIC_AP)
#undef MEM_SIZE
#define MEM_SIZE                (26*TCP_MSS)

#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE          64

#undef TCP_SND_BUF
#define TCP_SND_BUF             (24*TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG        (4* TCP_SND_BUF/TCP_MSS)

#undef TCP_WND
#define TCP_WND                 (16*TCP_MSS)

#undef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF           100
#elif defined(CONFIG_AMEBADPLUS)
#undef MEM_SIZE
#define MEM_SIZE                (26*TCP_MSS)

#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE          64

#undef TCP_SND_BUF
#define TCP_SND_BUF             (24*TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG        (4* TCP_SND_BUF/TCP_MSS)

#undef TCP_WND
#define TCP_WND                 (16*TCP_MSS)

#undef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF           100
#else
#undef MEM_SIZE
#define MEM_SIZE                (23*1024)

#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE          60

#undef TCP_SND_BUF
#define TCP_SND_BUF             (10*TCP_MSS)

#undef TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN        (6* TCP_SND_BUF/TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG        60

#undef TCP_WND
#define TCP_WND                 (8*TCP_MSS)
#endif
#endif

#if defined(CONFIG_EXAMPLE_AMAZON_ALEXA) && CONFIG_EXAMPLE_AMAZON_ALEXA

#undef TCP_WND
#define TCP_WND                                       	(4*TCP_MSS)

#undef TCP_SND_BUF
#define TCP_SND_BUF             (10*TCP_MSS)

#undef TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN        (4* TCP_SND_BUF/TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG        40

#endif

#if defined(CONFIG_VIDEO_APPLICATION) && CONFIG_VIDEO_APPLICATION
#undef	LWIP_WND_SCALE
#define	LWIP_WND_SCALE                  1

#undef	TCP_RCV_SCALE
#define	TCP_RCV_SCALE                   1

#undef MEM_SIZE
#define MEM_SIZE (512*1024)

#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE 880

#undef MEMP_NUM_NETBUF
#define MEMP_NUM_NETBUF 60

#undef IP_REASS_MAX_PBUFS
#define IP_REASS_MAX_PBUFS 40

#undef TCP_SND_BUF
#define TCP_SND_BUF (80*TCP_MSS)

#undef TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN (6*TCP_SND_BUF/TCP_MSS)

#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG TCP_SND_QUEUELEN

#undef TCP_WND
#define TCP_WND (80*TCP_MSS)

#undef MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN        64

#undef MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB        MEMP_NUM_NETCONN

#undef MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB        MEMP_NUM_NETCONN

#undef MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN MEMP_NUM_NETCONN

#undef LWIP_SO_SNDTIMEO
#define LWIP_SO_SNDTIMEO                		1

#endif

/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_ERRNO_STDINCLUDE

/*
In IAR, errno define in toolchain (arm\inc\c\errno.h) will not be used by compiler because the toolchain include path is lower priority (in the end of search path list).
so we define errno here if it have not been defined.
*/
#ifdef __ICCARM__
#ifndef errno
extern __ATTRIBUTES int volatile *__aeabi_errno_addr(void);
#define errno (* (int *) __aeabi_errno_addr())
#endif
#endif


/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/*
Certain platform allows computing and verifying the IP, UDP, TCP and ICMP checksums by hardware:
 - To use this feature let the following define uncommented.
 - To disable it and process by CPU comment the  the checksum.
*/
//Do checksum by lwip - WLAN nic does not support Checksum offload
//#define CHECKSUM_BY_HARDWARE


#ifdef CHECKSUM_BY_HARDWARE
/* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
#define CHECKSUM_GEN_IP                 0
/* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP                0
/* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP                0
/* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
#define CHECKSUM_CHECK_IP               0
/* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP              0
/* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP              0
#else
/* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
#define CHECKSUM_GEN_IP                 1
/* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP                1
/* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP                1
/* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
#define CHECKSUM_CHECK_IP               1
/* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP              1
/* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP              1
#endif


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    1

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     1

/*
   -----------------------------------
   ---------- DEBUG options ----------
   -----------------------------------
*/
#ifdef CONFIG_LWIP_DEBUG
#define LWIP_DEBUG                      1
#endif

/*
   ---------------------------------
   ---------- OS options ----------
   ---------------------------------
*/

#define TCPIP_THREAD_STACKSIZE          1000
#if defined(CONFIG_VIDEO_APPLICATION) && CONFIG_VIDEO_APPLICATION
#define TCPIP_MBOX_SIZE                 600
#define DEFAULT_UDP_RECVMBOX_SIZE       600
#define DEFAULT_TCP_RECVMBOX_SIZE       600
#define DEFAULT_RAW_RECVMBOX_SIZE       600
#define DEFAULT_ACCEPTMBOX_SIZE         600
#else
#define TCPIP_MBOX_SIZE                 6
#define DEFAULT_UDP_RECVMBOX_SIZE       6
#define DEFAULT_TCP_RECVMBOX_SIZE       6
#define DEFAULT_RAW_RECVMBOX_SIZE       6
#define DEFAULT_ACCEPTMBOX_SIZE         6
#endif
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (RTOS_TASK_MAX_PRIORITIES - 2)

#if defined(CONFIG_HIGH_TP_TEST)
#if defined(CONFIG_AMEBALITE) && defined (CONFIG_AS_INIC_AP)
#undef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE                 20
#undef DEFAULT_UDP_RECVMBOX_SIZE
#define DEFAULT_UDP_RECVMBOX_SIZE       16
#undef DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE       16
#undef MEMP_NUM_TCPIP_MSG_INPKT
#define MEMP_NUM_TCPIP_MSG_INPKT        16
#elif defined(CONFIG_AMEBAGREEN2) && defined (CONFIG_AS_INIC_AP)
#undef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE                 20
#undef DEFAULT_UDP_RECVMBOX_SIZE
#define DEFAULT_UDP_RECVMBOX_SIZE       16
#undef DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE       16
#undef MEMP_NUM_TCPIP_MSG_INPKT
#define MEMP_NUM_TCPIP_MSG_INPKT        16
#elif defined(CONFIG_AMEBASMART) && defined (CONFIG_AS_INIC_AP)
#undef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE                 30
#undef DEFAULT_UDP_RECVMBOX_SIZE
#define DEFAULT_UDP_RECVMBOX_SIZE       18
#undef DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE       18
#undef MEMP_NUM_TCPIP_MSG_INPKT
#define MEMP_NUM_TCPIP_MSG_INPKT        20
#elif defined(CONFIG_AMEBAGREEN2) && defined (CONFIG_AS_INIC_AP)
#undef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE                 20
#undef DEFAULT_UDP_RECVMBOX_SIZE
#define DEFAULT_UDP_RECVMBOX_SIZE       16
#undef DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE       16
#undef MEMP_NUM_TCPIP_MSG_INPKT
#define MEMP_NUM_TCPIP_MSG_INPKT        16
#elif defined(CONFIG_AMEBADPLUS)
#undef TCPIP_MBOX_SIZE
#define TCPIP_MBOX_SIZE                 20
#undef DEFAULT_UDP_RECVMBOX_SIZE
#define DEFAULT_UDP_RECVMBOX_SIZE       16
#undef DEFAULT_TCP_RECVMBOX_SIZE
#define DEFAULT_TCP_RECVMBOX_SIZE       16
#undef MEMP_NUM_TCPIP_MSG_INPKT
#define MEMP_NUM_TCPIP_MSG_INPKT        16
#endif
#endif

/* Added by Realtek. For DHCP server reply unicast DHCP packets before the ip actually assigned. */
#define ETHARP_SUPPORT_STATIC_ENTRIES   1

/* Added by Realtek start */
#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS 1
#define LWIP_DNS_LEGACY_SUPPORT 0
#define LWIP_ICMP_SUPPRESS 0
#define LWIP_ICMP_SUPPRESS_INTERVAL 900 // allow one icmp per second with tolerance of 100 ms
/* Added by Realtek end */

/* Extra options for lwip_v2.0.2 which should not affect lwip_v1.4.1 */
#define LWIP_TCPIP_CORE_LOCKING         0
#define LWIP_TCPIP_TIMEOUT              1
#define LWIP_SO_SNDTIMEO                1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SOCKET_SET_ERRNO           0
#define LWIP_RAW                        1
#define LWIP_AUTOIP                     1
#define TCPIP_THREAD_NAME              "TCP_IP"
#define LWIP_NETIF_API                  1
#define LWIP_IPV6                       0
#if LWIP_IPV6
#undef  MEMP_NUM_SYS_TIMEOUT
#define MEMP_NUM_SYS_TIMEOUT            13
#endif

#ifndef CONFIG_EXAMPLE_COAP_SERVER
#define CONFIG_EXAMPLE_COAP_SERVER 0
#endif

#ifndef CONFIG_EXAMPLE_COAP_CLIENT
#define CONFIG_EXAMPLE_COAP_CLIENT 0
#endif

/*CONFIG_LIBCOAP_ON is defined to 1 in the lib_coap project options preprocessor defined symbol
 *CONFIG_EXAMPLE_COAP_SERVER and CONFIG_EXAMPLE_COAP_CLIENT is defined in platform_opts.h
 */
#if CONFIG_EXAMPLE_COAP_SERVER || CONFIG_EXAMPLE_COAP_CLIENT || (defined(CONFIG_LIBCOAP_ON) && (CONFIG_LIBCOAP_ON))
#undef SO_REUSE
#define SO_REUSE                        1
#undef MEMP_NUM_NETCONN
#define MEMP_NUM_NETCONN                20
#define MEMP_USE_CUSTOM_POOLS           1
#undef LWIP_IPV6
#define LWIP_IPV6                       1
#define ERRNO                           1

#if defined(LWIP_IPV6) && (LWIP_IPV6==1)
#undef MEMP_NUM_SYS_TIMEOUT
#define MEMP_NUM_SYS_TIMEOUT            20
#ifndef xchar
#define xchar(i)                ((i) < 10 ? '0' + (i) : 'A' + (i) - 10)
#endif
#endif
#endif

#if defined(ENABLE_AMAZON_COMMON)
#define LWIP_COMPAT_MUTEX_ALLOWED
#define ERRNO                           1
#define LWIP_SO_SNDTIMEO                1
#define SYS_LIGHTWEIGHT_PROT            1
#define SO_REUSE                        1
#undef LWIP_TCPIP_CORE_LOCKING
#undef LWIP_SOCKET_SET_ERRNO
#define LWIP_TCPIP_CORE_LOCKING         1
#define LWIP_SOCKET_SET_ERRNO           1
#define SOCKETS_DEBUG                   LWIP_DBG_ON
#endif

#if defined(CONFIG_EXAMPLE_AZURE_IOT_HUB) && CONFIG_EXAMPLE_AZURE_IOT_HUB
#define ERRNO                           1
#endif

#include "lwip/init.h"                  //for version control

#if defined(CONFIG_VIDEO_APPLICATION) && CONFIG_VIDEO_APPLICATION
#undef ERRNO
#define ERRNO                           1
#undef LWIP_SOCKET_SET_ERRNO
#define LWIP_SOCKET_SET_ERRNO           1
#endif

#define LWIP_MDNS_RESPONDER 	1
#define LWIP_NUM_NETIF_CLIENT_DATA 1

#define LWIP_NETIF_LOOPBACK             1

#ifdef CONFIG_802154_THREAD_BORDER_ROUTER_EN
#undef LWIP_IPV6
#undef LWIP_IPV6_FORWARD
#undef IP_FORWARD
#undef LWIP_NETIF_STATUS_CALLBACK
#undef LWIP_IPV6_NUM_ADDRESSES
#undef LWIP_ND6_NUM_PREFIXES
#undef LWIP_ND6_NUM_ROUTERS
#undef MEMP_NUM_UDP_PCB
#undef MEMP_NUM_MLD6_GROUP
#undef LWIP_MULTICAST_PING
#undef LWIP_HOOK_ND6_GET_GW
#undef LWIP_HOOK_IP6_ROUTE
#undef LWIP_HOOK_IP6_INPUT
#undef LWIP_HOOK_IP4_INPUT
#undef LWIP_NETIF_EXT_STATUS_CALLBACK
#undef MDNS_RESP_USENETIF_EXTCALLBACK
#undef MDNS_MAX_SERVICES
#undef MEMP_NUM_SYS_TIMEOUT

#define LWIP_IPV6                       1
#define LWIP_IPV6_FORWARD               1
#define IP_FORWARD                      1
#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_IPV6_NUM_ADDRESSES         10
#define LWIP_ND6_NUM_PREFIXES           10
#define LWIP_ND6_NUM_ROUTERS            5
#define MEMP_NUM_UDP_PCB        		12
#define MEMP_NUM_MLD6_GROUP     		20
#define LWIP_MULTICAST_PING             1
#define LWIP_HOOK_ND6_GET_GW rtk_otbr_lwip_hook_nd6_get_gw
#define LWIP_HOOK_IP6_ROUTE rtk_otbr_lwip_hook_ip6_route
#define LWIP_HOOK_IP6_INPUT rtk_otbr_lwip_hook_ip6_input
#define LWIP_HOOK_IP4_INPUT rtk_otbr_lwip_hook_ip4_input
#define LWIP_NETIF_EXT_STATUS_CALLBACK  1
#define MDNS_RESP_USENETIF_EXTCALLBACK  1
#define MDNS_MAX_SERVICES               10
#define MEMP_NUM_SYS_TIMEOUT            13
#endif

#define MEM_LIBC_MALLOC                 1

#endif /* LWIP_HDR_LWIPOPTS_H */
