/* Includes ------------------------------------------------------------------*/
#include "lwip_netconf.h"
#include "wifi_intf_drv_to_upper.h"
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "ameba_pmu.h"
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "lwip_intf_tizenrt.h"

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif


/*Static IP ADDRESS FOR ETHERNET*/
#ifndef ETH_IP_ADDR0
#define ETH_IP_ADDR0 192
#define ETH_IP_ADDR1 168
#define ETH_IP_ADDR2 0
#define ETH_IP_ADDR3 80
#endif

/*NETMASK FOR ETHERNET*/
#ifndef ETH_NETMASK_ADDR0
#define ETH_NETMASK_ADDR0 255
#define ETH_NETMASK_ADDR1 255
#define ETH_NETMASK_ADDR2 255
#define ETH_NETMASK_ADDR3 0
#endif

/*Gateway address for ethernet*/
#ifndef ETH_GW_ADDR0
#define ETH_GW_ADDR0 192
#define ETH_GW_ADDR1 168
#define ETH_GW_ADDR2 0
#define ETH_GW_ADDR3 1
#endif

/* Private define ------------------------------------------------------------*/
#define MAX_DHCP_TRIES 5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef CONFIG_WHC_HOST
#include "whc_ipc.h"
#endif

#if (defined(CONFIG_WHC_DEV) && defined(CONFIG_WHC_DEV_TCPIP_KEEPALIVE)) || defined CONFIG_WHC_HOST || defined CONFIG_WHC_NONE
struct static_ip_config user_static_ip;

unsigned char ap_ip[4] = {192, 168, 43, 1}, ap_netmask[4] = {255, 255, 255, 0}, ap_gw[4] = {192, 168, 43, 1};
#endif


extern void (*p_wifi_join_info_free)(u8 iface_type);
struct netif xnetif[NET_IF_NUM]; /* network interface structure */
struct netif *pnetif_sta = &xnetif[NETIF_WLAN_STA_INDEX];
struct netif *pnetif_ap = &xnetif[NETIF_WLAN_AP_INDEX];
#if defined(CONFIG_NAN)
struct netif *pnetif_nan = &xnetif[NETIF_WLAN_NAN_INDEX];
#endif
#if defined(CONFIG_LWIP_ETHERNET)
struct netif *pnetif_eth = &xnetif[NETIF_ETH_INDEX];
#endif
#if defined(CONFIG_LWIP_USB_ETHERNET)
struct netif *pnetif_usb_eth = &xnetif[NETIF_USB_ETH_INDEX];
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */

int lwip_init_done = 0;

void LwIP_Init(void)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	int8_t idx = 0;
	u32 heap = rtos_mem_get_free_heap_size();

	/* Create tcp_ip stack thread */
	tcpip_init(NULL, NULL);

#ifdef LWIP_HOOK_TCP_ISN
	tcp_isn_init();
#endif

	/* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
	        struct ip_addr *netmask, struct ip_addr *gw,
	        void *state, err_t (* init)(struct netif *netif),
	        err_t (* input)(struct pbuf *p, struct netif *netif))

	Adds your network interface to the netif_list. Allocate a struct
	netif and pass a pointer to this structure as the first argument.
	Give pointers to cleared ip_addr structures when using DHCP,
	or fill them with sane numbers otherwise. The state pointer may be NULL.

	The init function pointer must point to a initialization function for
	your ethernet netif interface. The following code illustrates it's use.*/
	//RTK_LOGS(NOTAG, RTK_LOG_INFO, "NET_IF_NUM:%d\n\r",NET_IF_NUM);
	for (idx = 0; idx < NET_IF_NUM; idx++) {
		if (idx == 0) {
			IP4_ADDR(ip_2_ip4(&ipaddr), IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
			IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
			IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		}
#if defined(CONFIG_LWIP_USB_ETHERNET)
		else if (idx == NETIF_USB_ETH_INDEX) {
			IP4_ADDR(ip_2_ip4(&ipaddr), ETH_IP_ADDR0, ETH_IP_ADDR1, ETH_IP_ADDR2, ETH_IP_ADDR3);
			IP4_ADDR(ip_2_ip4(&netmask), ETH_NETMASK_ADDR0, ETH_NETMASK_ADDR1, ETH_NETMASK_ADDR2, ETH_NETMASK_ADDR3);
			IP4_ADDR(ip_2_ip4(&gw), ETH_GW_ADDR0, ETH_GW_ADDR1, ETH_GW_ADDR2, ETH_GW_ADDR3);
		}
#endif
#if defined(CONFIG_LWIP_ETHERNET)
		else if (idx == NETIF_ETH_INDEX) {
			IP4_ADDR(ip_2_ip4(&ipaddr), ETH_IP_ADDR0, ETH_IP_ADDR1, ETH_IP_ADDR2, ETH_IP_ADDR3);
			IP4_ADDR(ip_2_ip4(&netmask), ETH_NETMASK_ADDR0, ETH_NETMASK_ADDR1, ETH_NETMASK_ADDR2, ETH_NETMASK_ADDR3);
			IP4_ADDR(ip_2_ip4(&gw), ETH_GW_ADDR0, ETH_GW_ADDR1, ETH_GW_ADDR2, ETH_GW_ADDR3);
		}
#endif
		else {
			IP4_ADDR(ip_2_ip4(&ipaddr), AP_IP_ADDR0, AP_IP_ADDR1, AP_IP_ADDR2, AP_IP_ADDR3);
			IP4_ADDR(ip_2_ip4(&netmask), AP_NETMASK_ADDR0, AP_NETMASK_ADDR1, AP_NETMASK_ADDR2, AP_NETMASK_ADDR3);
			IP4_ADDR(ip_2_ip4(&gw), AP_GW_ADDR0, AP_GW_ADDR1, AP_GW_ADDR2, AP_GW_ADDR3);
		}

		xnetif[idx].name[0] = 'r';
		xnetif[idx].name[1] = '0' + idx;

		netifapi_netif_add(&xnetif[idx], ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw), NULL, &ethernetif_init, &tcpip_input);

		RTK_LOGS(NOTAG, RTK_LOG_INFO, "interface %d is initialized\n", idx);

	}

	/*  Registers the default network interface. */
	netifapi_netif_set_default(pnetif_sta);

	/*move these operations to wifi_on/wifi_off*/
#if 0
	/*  When the netif is fully configured this function must be called.*/
	for (idx = 0; idx < NET_IF_NUM; idx++) {
		netif_set_up(&xnetif[idx]);
	}
#endif

	lwip_init_done = 1;
#ifdef CONFIG_STANDARD_TICKLESS
	pmu_register_sleep_callback(PMU_LWIP_STACK, (PSM_HOOK_FUN)lwip_rm_unneeded_tmr, NULL, (PSM_HOOK_FUN)lwip_update_internal_counter, NULL);
#endif
	RTK_LOGS(TAG_WLAN_DRV, RTK_LOG_INFO, "LWIP consume heap %d\n", heap - rtos_mem_get_free_heap_size() - 4 * TCPIP_THREAD_STACKSIZE);
}

#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
extern uint32_t offer_ip;
extern uint32_t server_ip;

#endif
/**
  * @brief  LwIP_DHCP_Process_Handle
  * @param  None
  * @retval None
  */
uint8_t LwIP_DHCP(uint8_t idx, uint8_t dhcp_state)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	uint32_t IPaddress;
	uint8_t iptab[4];
	uint8_t DHCP_state;
	struct dhcp *dhcp = NULL;
	uint8_t ret = 0;
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return 0;
	}

	DHCP_state = dhcp_state;

#if defined(CONFIG_LWIP_USB_ETHERNET_BRIDGE) && CONFIG_LWIP_USB_ETHERNET_BRIDGE
	RTK_LOGS(NOTAG, RTK_LOG_INFO, "skip DHCP !!\n");
	return 0;
#endif

	extern struct static_ip_config user_static_ip;
	if (user_static_ip.use_static_ip) {
		LwIP_SetIP(NETIF_WLAN_STA_INDEX, user_static_ip.addr, user_static_ip.netmask, user_static_ip.gw);
		iptab[3] = (uint8_t)(user_static_ip.addr >> 24);
		iptab[2] = (uint8_t)(user_static_ip.addr >> 16);
		iptab[1] = (uint8_t)(user_static_ip.addr >> 8);
		iptab[0] = (uint8_t)(user_static_ip.addr);
		RTK_LOGS(NOTAG, RTK_LOG_INFO, "\n\rSet Interface %d static IP : %d.%d.%d.%d\n", idx, iptab[3], iptab[2], iptab[1], iptab[0]);
		return 0;
	}

	if (DHCP_state == 0) {
		ip_addr_set_zero(&pnetif->ip_addr);
		ip_addr_set_zero(&pnetif->netmask);
		ip_addr_set_zero(&pnetif->gw);
	}

	dhcp = ((struct dhcp *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP));
	if (!netif_is_up(pnetif)) { // netif should be set up before doing dhcp request (in lwip v2.0.0)
		netifapi_netif_set_up(pnetif);
	}

#ifndef CONFIG_STANDARD_TICKLESS
	/*acqurie wakelock to guarantee dhcp*/
	pmu_acquire_wakelock(PMU_DHCP_PROCESS);
#endif
	for (;;) {
		//RTK_LOGS(NOTAG, RTK_LOG_INFO, "\n\r ========DHCP_state:%d============\n\r",DHCP_state);
		switch (DHCP_state) {
		case DHCP_START: {

#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
			if (check_is_the_same_ap()) {
				if ((offer_ip != 0 && offer_ip != 0xFFFFFFFF) || (dhcp != NULL)) {
					if (dhcp == NULL) {
						dhcp = (struct dhcp *)mem_malloc(sizeof(struct dhcp));
						if (dhcp == NULL) {
							RTK_LOGS(NOTAG, RTK_LOG_ERROR, "dhcp_start(): could not allocate dhcp\n");
							ret = DHCP_STOP;
							goto exit;
						}
					}
					memset(dhcp, 0, sizeof(struct dhcp));
					dhcp->offered_ip_addr.addr = (u32_t)offer_ip;

					ip_addr_set_ip4_u32(&dhcp->server_ip_addr, (u32_t)server_ip);

					netif_set_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP, dhcp);
				}
			} else {
				if (dhcp != NULL) {
					memset(dhcp, 0, sizeof(struct dhcp));
				}
			}

#endif
			netifapi_dhcp_start(pnetif);
			dhcp = ((struct dhcp *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP));
			IPaddress = 0;
			DHCP_state = DHCP_WAIT_ADDRESS;
		}
		break;

		case DHCP_WAIT_ADDRESS: {
			/* If DHCP stopped by wifi_disconn_hdl*/
			if ((dhcp_state_enum_t)dhcp->state == DHCP_STATE_OFF) {
				DHCP_state = DHCP_STOP;
				IP4_ADDR(ip_2_ip4(&ipaddr), IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
				IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
				IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
				netifapi_netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
				RTK_LOGS(NOTAG, RTK_LOG_INFO, "\n\rLwIP_DHCP: dhcp stop.");
				wifi_indication(RTW_EVENT_DHCP_STATUS, &DHCP_state, sizeof(uint8_t));
				ret = DHCP_STOP;
				goto exit;
			}

			/* Read the new IP address */
			IPaddress = ip_addr_get_ip4_u32(netif_ip_addr4(pnetif));

			if (IPaddress != 0) {
#if LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS
				tcp_randomize_local_port();
				udp_randomize_local_port();
#endif
				DHCP_state = DHCP_ADDRESS_ASSIGNED;

				/* Stop DHCP */
				// dhcp_stop(pnetif);  /* can not stop, need to renew, Robbie*/

				iptab[0] = (uint8_t)(IPaddress >> 24);
				iptab[1] = (uint8_t)(IPaddress >> 16);
				iptab[2] = (uint8_t)(IPaddress >> 8);
				iptab[3] = (uint8_t)(IPaddress);

				at_printf_indicate("wifi got ip:\"%d.%d.%d.%d\"\r\n", iptab[3], iptab[2], iptab[1], iptab[0]);
				wifi_dhcp_success_indicate();

#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
				dhcp = ((struct dhcp *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP));

				if (p_store_fast_connect_info) {
					p_store_fast_connect_info((uint32_t)dhcp->offered_ip_addr.addr, (uint32_t)ip_addr_get_ip4_u32(&dhcp->server_ip_addr));
				}
				if (p_wifi_join_info_free) {
					/* free key here after join success */
					p_wifi_join_info_free(IFACE_PORT0);
				}
#endif
				wifi_indication(RTW_EVENT_DHCP_STATUS, &DHCP_state, sizeof(uint8_t));
				ret = DHCP_ADDRESS_ASSIGNED;
				goto exit;
			} else {
				/* DHCP timeout */
				if (dhcp->tries > MAX_DHCP_TRIES) {
					DHCP_state = DHCP_TIMEOUT;
					/* Stop DHCP */
					netifapi_dhcp_stop(pnetif);

					at_printf_indicate("wifi got ip timeout\r\n");

#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP
					if (p_store_fast_connect_info) {
						p_store_fast_connect_info((uint32_t)dhcp->offered_ip_addr.addr, (uint32_t)ip_addr_get_ip4_u32(&dhcp->server_ip_addr));
					}
					if (p_wifi_join_info_free) {
						/* free key here after join success */
						p_wifi_join_info_free(IFACE_PORT0);
					}
#endif
					wifi_indication(RTW_EVENT_DHCP_STATUS, &DHCP_state, sizeof(uint8_t));
					ret = DHCP_TIMEOUT;
					goto exit;
				}
			}
		}
		break;
		case DHCP_RELEASE_IP:
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "\n\rLwIP_DHCP: Release ip");
			netifapi_dhcp_release(pnetif);
			ret = DHCP_RELEASE_IP;
			goto exit;
		case DHCP_STOP:
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "\n\rLwIP_DHCP: dhcp stop.");
			LwIP_DHCP_stop(idx);
			ret = DHCP_STOP;
			goto exit;
		default:
			RTK_LOGS(NOTAG, RTK_LOG_ERROR, "\n\rLwIP_DHCP: invalid dhcp state\n");
			ret = DHCP_STOP;
			goto exit;
		}
		/* wait 10 ms */
		rtos_time_delay_ms(10);
	}

exit:
#ifndef CONFIG_STANDARD_TICKLESS
	pmu_release_wakelock(PMU_DHCP_PROCESS);
#endif
	return ret;
}

void LwIP_ReleaseIP(uint8_t idx)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}

	IP4_ADDR(ip_2_ip4(&ipaddr), 0, 0, 0, 0);
	IP4_ADDR(ip_2_ip4(&netmask), 255, 255, 255, 0);
	IP4_ADDR(ip_2_ip4(&gw), 0, 0, 0, 0);
	netifapi_netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
}

void LwIP_DHCP_stop(uint8_t idx)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}

	netifapi_dhcp_stop(pnetif);

	IP4_ADDR(ip_2_ip4(&ipaddr), IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
	IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
	netifapi_netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
}
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
int LwIP_netif_get_idx(struct netif *pnetif)
{
#if defined(CONFIG_LWIP_LAYER) && (CONFIG_LWIP_LAYER == 1)
	int idx = pnetif - xnetif;
	return idx < NET_IF_NUM ? idx : -1;
#else
	UNUSED(pnetif);
	return -1;
#endif
}

struct netif *LwIP_idx_get_netif(uint8_t idx)
{
#if defined(CONFIG_LWIP_LAYER) && (CONFIG_LWIP_LAYER == 1)
	if (idx < NET_IF_NUM) {
		return &xnetif[idx];
	} else {
		RTK_LOGS(NOTAG, RTK_LOG_ERROR, "idx is invalid !!\n");
		return NULL;
	}
#else
	UNUSED(idx);
	return NULL;
#endif
}

void LwIP_netif_set_up(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	netifapi_netif_set_up(pnetif);
}

void LwIP_netif_set_down(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	netifapi_netif_set_down(pnetif);
}

void LwIP_netif_set_link_up(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	netifapi_netif_set_link_up(pnetif);
	if (idx == NETIF_WLAN_STA_INDEX) {
		netifapi_netif_set_default(pnetif_sta);
	} else if ((idx == NETIF_WLAN_AP_INDEX) && (!(xnetif[NETIF_WLAN_STA_INDEX].flags & NETIF_FLAG_LINK_UP))) {
		/*default netif is on sta when sta and softap both up*/
		netifapi_netif_set_default(pnetif_ap);
	}
}

void LwIP_netif_set_link_down(uint8_t idx)
{
	struct netif *pnetif;
	if (idx < NET_IF_NUM) {
		pnetif = &xnetif[idx];
	} else {
		return;
	}
	netifapi_netif_set_link_down(pnetif);
	if (idx == NETIF_WLAN_AP_INDEX) {
		netifapi_netif_set_default(pnetif_sta);
	} else if (idx == NETIF_WLAN_STA_INDEX) {
		netifapi_netif_set_default(pnetif_ap);
	}
}

uint8_t *LwIP_GetMAC(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return NULL;
	}
	return (uint8_t *)(pnetif->hwaddr);
}

uint8_t *LwIP_GetIP(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return NULL;
	}
	return (uint8_t *) & (pnetif->ip_addr);
}

uint8_t *LwIP_GetGW(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return NULL;
	}
	return (uint8_t *) & (pnetif->gw);
}

uint8_t *LwIP_GetMASK(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return NULL;
	}
	return (uint8_t *) & (pnetif->netmask);
}

void LwIP_wlan_set_netif_info(uint8_t idx, void *dev, unsigned char *dev_addr)
{
	struct netif *pnetif;
	if (idx < NET_IF_NUM) {
		pnetif = &xnetif[idx];
	} else {
		return;
	}
	memcpy(pnetif->hwaddr, dev_addr, 6);
	pnetif->state = dev;
}
#ifndef CONFIG_PLATFORM_TIZENRT_OS
void LwIP_ethernetif_recv(uint8_t idx, int total_len)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	ethernetif_recv(pnetif, total_len);
}

SRAM_WLAN_CRITICAL_CODE_SECTION
void LwIP_ethernetif_recv_inic(uint8_t idx, struct pbuf *p_buf)
{
	err_enum_t error = ERR_OK;
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	error = pnetif->input(p_buf, pnetif);
	if (error != ERR_OK) {
		RTK_LOGS(TAG_WLAN_INIC, RTK_LOG_ERROR, "lwip input err (%d)\n", error);
		pbuf_free(p_buf);
	}
}
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
int LwIP_netif_is_valid_IP(uint8_t idx, unsigned char *ip_dest)
{
#if defined(CONFIG_LWIP_LAYER) && (CONFIG_LWIP_LAYER == 1)
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return 0;
	}

	ip_addr_t addr = { 0 };
	u32_t ip_dest_addr = { 0 };

	memcpy(&ip_dest_addr, ip_dest, 4);

	ip_addr_set_ip4_u32(&addr, ip_dest_addr);

	if ((ip_addr_get_ip4_u32(netif_ip_addr4(pnetif))) == 0) {
		return 1;
	}

	if (ip_addr_ismulticast(&addr) || ip_addr_isbroadcast(&addr, pnetif)) {
		return 1;
	}

	//if(ip_addr_netcmp(&(pnetif->ip_addr), &addr, &(pnetif->netmask))) //addr&netmask
	//	return 1;

	if (ip_addr_cmp(&(pnetif->ip_addr), &addr)) {
		return 1;
	}

	//RTK_LOGS(NOTAG, RTK_LOG_INFO, "invalid IP: %d.%d.%d.%d ",ip_dest[0],ip_dest[1],ip_dest[2],ip_dest[3]);
#endif
	UNUSED(idx);
	UNUSED(ip_dest);
	return 0;
}

#if LWIP_DNS
void LwIP_GetDNS(struct ip_addr *dns)
{
	struct ip_addr *tmp = (struct ip_addr *)dns_getserver(0);
	*dns = *tmp;
}

void LwIP_SetDNS(struct ip_addr *dns)
{
	dns_setserver(0, dns);
}
#endif

void LwIP_SetIP(uint8_t idx, u32_t addr, u32_t netmask_addr, u32_t gw_addr)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}

#if CONFIG_WLAN || (defined(CONFIG_LWIP_USB_ETHERNET) && CONFIG_LWIP_USB_ETHERNET) || (defined(CONFIG_LWIP_ETHERNET) && CONFIG_LWIP_ETHERNET)
	ip_2_ip4(&ipaddr)->addr = PP_HTONL(addr);
	ip_2_ip4(&netmask)->addr = PP_HTONL(netmask_addr);
	ip_2_ip4(&gw)->addr = PP_HTONL(gw_addr);
	netifapi_netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
#endif
}

#if LWIP_AUTOIP
#include <lwip/autoip.h>
#include <lwip/prot/autoip.h>

void LwIP_AUTOIP(uint8_t idx)
{
	uint8_t *ip = LwIP_GetIP(idx);
	struct autoip *autoip = NULL;
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}

	autoip = ((struct autoip *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_AUTOIP));
	if (autoip && (autoip->tried_llipaddr >= MAX_CONFLICTS)) { // before autoip_start(), autoip may be NULL
		autoip->tried_llipaddr = 0;
	}

	netifapi_autoip_start(pnetif);

	autoip = ((struct autoip *)netif_get_client_data(pnetif, LWIP_NETIF_CLIENT_DATA_INDEX_AUTOIP));

	while ((autoip->state == AUTOIP_STATE_PROBING) || (autoip->state == AUTOIP_STATE_ANNOUNCING)) {
		rtos_time_delay_ms(1000);
	}

	if (*((uint32_t *) ip) == 0) {
		struct ip_addr ipaddr;
		struct ip_addr netmask;
		struct ip_addr gw;

		RTK_LOGS(NOTAG, RTK_LOG_INFO, "AUTOIP timeout\n");

		/* Static address used */
		IP4_ADDR(ip_2_ip4(&ipaddr), STATIC_IP_ADDR0, STATIC_IP_ADDR1, STATIC_IP_ADDR2, STATIC_IP_ADDR3);
		IP4_ADDR(ip_2_ip4(&netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
		IP4_ADDR(ip_2_ip4(&gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
		netifapi_netif_set_addr(pnetif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
		RTK_LOGS(NOTAG, RTK_LOG_INFO, "Static IP address : %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
	} else {
		RTK_LOGS(NOTAG, RTK_LOG_INFO, "\nLink-local address: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
	}
}

void LwIP_AUTOIP_STOP(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	netifapi_autoip_stop(pnetif);
}
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if LWIP_IPV6
/* Get IPv6 address with lwip 1.5.0 */
void LwIP_AUTOIP_IPv6(uint8_t idx)
{
	struct netif *pnetif = LwIP_idx_get_netif(idx);
	if (pnetif == NULL) {
		return;
	}
	netif_create_ip6_linklocal_address(pnetif, 1);
	RTK_LOGS(NOTAG, RTK_LOG_INFO, "\nIPv6 link-local address: ");
	LwIP_DUMP_IPV6_ADDRESS(netif_ip6_addr(pnetif, 0)->addr);
}
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

//To check successful WiFi connection and obtain of an IP address
int LwIP_Check_Connectivity(uint8_t idx)
{
	if (idx == NETIF_WLAN_STA_INDEX) {
		u8 join_status = RTW_JOINSTATUS_UNKNOWN;
		if (!((wifi_get_join_status(&join_status) == RTK_SUCCESS)
			  && (join_status == RTW_JOINSTATUS_SUCCESS)
#if defined(CONFIG_LWIP_USB_ETHERNET_BRIDGE) && CONFIG_LWIP_USB_ETHERNET_BRIDGE
			 )) {
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "Wait for WiFi Connect Success...\n");
#else
			  &&(*(u32 *)LwIP_GetIP(NETIF_WLAN_STA_INDEX) != IP_ADDR_INVALID))) {
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "Wait for WiFi and DHCP Connect Success...\n");
#endif
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "Please use AT+WLCONN to connect AP first time\n");
			return CONNECTION_INVALID;
		} else {
			return CONNECTION_VALID;
		}
	} else if (idx == NETIF_WLAN_AP_INDEX) {
		if (wifi_is_running(SOFTAP_WLAN_INDEX) == FALSE) {
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "SoftAP is not running...\n");
			RTK_LOGS(NOTAG, RTK_LOG_INFO, "Please use AT+WLSTARTAP to start AP first time\n");
			return CONNECTION_INVALID;
		} else {
			return CONNECTION_VALID;
		}
	} else {
		return CONNECTION_INVALID;
	}
}

/**
  * @brief  For sta get ipv4(dhcp) and ipv6 address
  * @param  None
  * @retval -1 for failed
  */

uint8_t LwIP_IP_Address_Request(uint8_t idx)
{
	uint8_t ret = -1;
#if LWIP_IPV6
	LwIP_AUTOIP_IPv6(idx);
#endif
#if LWIP_IPV4
	ret = LwIP_DHCP(idx, DHCP_START);
#endif
	return ret;
}
