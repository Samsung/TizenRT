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
#include "platform_autoconf.h"
#include "lwipconf.h"
#if defined(CONFIG_WLAN)
#include "wifi_api_types.h"
#endif

extern struct netif xnetif[];
extern struct netif *pnetif_sta;
extern struct netif *pnetif_ap;
#if defined(CONFIG_LWIP_ETHERNET)
extern struct netif *pnetif_eth;
#endif
#if defined(CONFIG_LWIP_USB_ETHERNET)
extern struct netif *pnetif_usb_eth;
#endif
#if LWIP_IPV6
#define LwIP_DUMP_IPV6_ADDRESS(addr) do { \
	uint8_t *ipv6 = (uint8_t *)addr; \
	RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\r\n", \
		ipv6[0], ipv6[1],  ipv6[2],  ipv6[3],  ipv6[4],  ipv6[5],  ipv6[6], ipv6[7], ipv6[8], ipv6[9], ipv6[10], ipv6[11], ipv6[12], ipv6[13], ipv6[14], ipv6[15]); \
} while(0)
#endif

extern unsigned char ap_ip[4], ap_netmask[4], ap_gw[4];
extern struct static_ip_config user_static_ip;
// macros
/* Give default value if not defined */
#define IP_ADDR_INVALID 0x00000000

/*Initial IP ADDRESS*/
#define IP_ADDR0   0
#define IP_ADDR1   0
#define IP_ADDR2   0
#define IP_ADDR3   0

/*Static IP ADDRESS*/
#define STATIC_IP_ADDR0   192
#define STATIC_IP_ADDR1   168
#define STATIC_IP_ADDR2   1
#define STATIC_IP_ADDR3   80

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   0
#define GW_ADDR1   0
#define GW_ADDR2   0
#define GW_ADDR3   0

/*******************************************/

/*Static IP ADDRESS*/
#define AP_IP_ADDR0   ap_ip[0]
#define AP_IP_ADDR1   ap_ip[1]
#define AP_IP_ADDR2   ap_ip[2]
#define AP_IP_ADDR3   ap_ip[3]

/*NETMASK*/
#define AP_NETMASK_ADDR0   ap_netmask[0]
#define AP_NETMASK_ADDR1   ap_netmask[1]
#define AP_NETMASK_ADDR2   ap_netmask[2]
#define AP_NETMASK_ADDR3   ap_netmask[3]

/*Gateway Address*/
#define AP_GW_ADDR0   ap_gw[0]
#define AP_GW_ADDR1   ap_gw[1]
#define AP_GW_ADDR2   ap_gw[2]
#define AP_GW_ADDR3   ap_gw[3]

#define CONNECTION_VALID 0
#define CONNECTION_INVALID 1

enum {
#if defined(CONFIG_WLAN)
	NETIF_WLAN_STA_INDEX = STA_WLAN_INDEX,
	NETIF_WLAN_AP_INDEX = SOFTAP_WLAN_INDEX,
#endif
#if defined(CONFIG_NAN)
	NETIF_WLAN_NAN_INDEX,
#endif
#if defined(CONFIG_LWIP_USB_ETHERNET)
	NETIF_USB_ETH_INDEX,
#endif
#if defined(CONFIG_LWIP_ETHERNET)
	NETIF_ETH_INDEX,
#endif
	NET_IF_NUM
};

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


/* Exported types ------------------------------------------------------------*/
extern struct netif *pnetif_eth;
extern int lwip_init_done;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LwIP_Init(void);
void LwIP_ReleaseIP(uint8_t idx);
uint8_t LwIP_DHCP(uint8_t idx, uint8_t dhcp_state);
void LwIP_DHCP_stop(uint8_t idx);
void LwIP_netif_set_up(uint8_t idx);
void LwIP_netif_set_down(uint8_t idx);
void LwIP_netif_set_link_up(uint8_t idx);
void LwIP_netif_set_link_down(uint8_t idx);
uint8_t *LwIP_GetMAC(uint8_t idx);
unsigned char *LwIP_GetIP(uint8_t idx);
unsigned char *LwIP_GetGW(uint8_t idx);
uint8_t *LwIP_GetMASK(uint8_t idx);
void LwIP_wlan_set_netif_info(uint8_t idx, void *dev, unsigned char *dev_addr);
void LwIP_ethernetif_recv(uint8_t idx, int total_len);
void LwIP_ethernetif_recv_inic(uint8_t idx, struct pbuf *p_buf);
int LwIP_netif_is_valid_IP(uint8_t idx, unsigned char *ip_dest);
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
void LwIP_AUTOIP_IPv6(uint8_t idx);
#endif
int LwIP_netif_get_idx(struct netif *pnetif);
struct netif *LwIP_idx_get_netif(uint8_t idx);
int LwIP_Check_Connectivity(uint8_t idx);
uint8_t LwIP_IP_Address_Request(uint8_t idx);

#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
