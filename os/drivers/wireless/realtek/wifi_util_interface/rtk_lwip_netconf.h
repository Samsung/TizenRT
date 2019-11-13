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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/tcpip.h"
#include "lwip/init.h" //for lwip version control
/* Includes ------------------------------------------------------------------*/
//#include <platform/platform_stdlib.h>
//#include "platform_opts.h"
//#include "autoconf.h"

#include "lwip/err.h"
// macros
/* Give default value if not defined */

#define CONFIG_WLAN 1

#ifndef NET_IF_NUM
#ifdef CONFIG_CONCURRENT_MODE
#define NET_IF_NUM ((CONFIG_WLAN) + 1)
#else
#define NET_IF_NUM (CONFIG_WLAN)
#endif // end of CONFIG_CONCURRENT_MODE
#endif // end of NET_IF_NUM

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	DHCP_START = 0,
	DHCP_WAIT_ADDRESS,
	DHCP_ADDRESS_ASSIGNED,
	DHCP_RELEASE_IP,
	DHCP_STOP,
	DHCP_TIMEOUT
} DHCP_State_TypeDef;

/* Extern functions ------------------------------------------------------------*/
void wifi_rx_beacon_hdl(char *buf, int buf_len, int flags, void *userdata);
void rtw_wakelock_timeout(u32 timeoutms);
/** Release a DHCP lease. */
err_t dhcp_release_unicast(struct netif *netif);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int LwIP_Is_Init(void);
void LwIP_Init_If(void);
void LwIP_ReleaseIP(uint8_t idx);
unsigned char *LwIP_GetMAC(struct netif *pnetif);
unsigned char *LwIP_GetIP(struct netif *pnetif);
unsigned char *LwIP_GetGW(struct netif *pnetif);
uint8_t *LwIP_GetMASK(struct netif *pnetif);
uint8_t *LwIP_GetBC(struct netif *pnetif);
#if LWIP_DNS
void LwIP_GetDNS(ip_addr_t *dns);
void LwIP_SetDNS(ip_addr_t *dns);
#endif
void LwIP_UseStaticIP(struct netif *pnetif);
#if LWIP_AUTOIP
void LwIP_AUTOIP(struct netif *pnetif);
#endif
#if LWIP_IPV6
void LwIP_AUTOIP_IPv6(struct netif *pnetif);
#endif
uint32_t LWIP_Get_Dynamic_Sleep_Interval(void);
#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
