/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LWIPCONF_H
#define __LWIPCONF_H

#include "lwip/api.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/stats.h"

#include "lwip/inet.h"
#include "lwip/inet_chksum.h"

#include "lwip/netif.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/debug.h"

#include "lwip/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/igmp.h"
#include "lwip/icmp.h"

#include "lwip/raw.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"

#include "lwip/pbuf.h"
#include "lwip/err.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"

#include "lwip/ethip6.h" /* Add for ipv6 */

#include "lwip/ip_addr.h" 
#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
#include "lwip/lwip_timers.h"

#include "lwip/apps/sntp.h"
#include "lwip/apps/sntp_opts.h"

#include "netif/loopif.h"
#include "netif/etharp.h"
#include "netif/ppp/ppp_impl.h"
#include "netif/ppp/pppapi.h"
#include "netif/ppp/pppoe.h"

#include "ethernetif.h"
#else
#include "ethernetif_tizenrt.h"
#endif

#endif //__LWIPCONF_H
