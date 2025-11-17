/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform_opts.h"
#include "rtw_wifi_constants.h"
#include "lwipconf.h"
#include "lwip/netifapi.h"
// macros
/* Give default value if not defined */
#define IP_ADDR_INVALID 0x00000000

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	DHCP_START = 0,
	DHCP_WAIT_ADDRESS,
	DHCP_ADDRESS_ASSIGNED,
	DHCP_RELEASE_IP,
	DHCP_STOP,
	DHCP_TIMEOUT
} DHCP_State_TypeDef;

struct static_ip_config {
	uint8_t  use_static_ip;
	uint32_t addr;
	uint32_t netmask;
	uint32_t gw;
};

/* Extern functions ------------------------------------------------------------*/
#ifndef CONFIG_AS_INIC_AP
void rtw_wakelock_timeout(uint32_t timeoutms);
#endif
/** Release a DHCP lease. */
err_t dhcp_release_unicast(struct netif *netif);


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LwIP_Init(void);
void LwIP_ReleaseIP(uint8_t idx);
uint8_t LwIP_DHCP(uint8_t idx, uint8_t dhcp_state);
void LwIP_DHCP_stop(uint8_t idx);
s8_t LwIP_etharp_find_addr(uint8_t idx, const ip4_addr_t *ipaddr,
						   struct eth_addr **eth_ret, const ip4_addr_t **ip_ret);
void LwIP_etharp_request(uint8_t idx, const ip4_addr_t *ipaddr);
void LwIP_netif_set_up(uint8_t idx);
void LwIP_netif_set_down(uint8_t idx);
void LwIP_netif_set_link_up(uint8_t idx);
void LwIP_netif_set_link_down(uint8_t idx);
uint8_t *LwIP_GetMAC(uint8_t idx);
unsigned char *LwIP_GetIP(uint8_t idx);
unsigned char *LwIP_GetGW(uint8_t idx);
uint8_t *LwIP_GetMASK(uint8_t idx);
void LwIP_wlan_set_netif_info(int idx_wlan, void *dev, unsigned char *dev_addr);
void LwIP_ethernetif_recv(uint8_t idx, int total_len);
void LwIP_ethernetif_recv_inic(uint8_t idx, struct pbuf *p_buf);
int LwIP_netif_is_valid_IP(int idx, unsigned char *ip_dest);
uint8_t *LwIP_GetBC(uint8_t idx);
#if LWIP_DNS
void LwIP_GetDNS(struct ip_addr *dns);
void LwIP_SetDNS(struct ip_addr *dns);
#endif
void LwIP_SetIP(uint8_t idx, u32_t addr, u32_t netmask, u32_t gw);
#if LWIP_AUTOIP
void LwIP_AUTOIP(uint8_t idx);
void LwIP_AUTOIP_STOP(uint8_t idx);
#endif
#if LWIP_IPV6
void LwIP_AUTOIP_IPv6(struct netif *pnetif);
#endif
uint32_t LWIP_Get_Dynamic_Sleep_Interval(void);
int netif_get_idx(struct netif *pnetif);
uint32_t LwIP_GetXID(uint8_t idx);
uint32_t LwIP_GetLEASETIME(uint8_t idx);
uint32_t LwIP_GetRENEWTIME(uint8_t idx);
#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
