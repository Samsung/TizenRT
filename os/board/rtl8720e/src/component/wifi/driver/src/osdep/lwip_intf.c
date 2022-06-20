/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

//#define _LWIP_INTF_C_
#include <autoconf.h>
#include <lwip_intf.h>
#if !defined(CONFIG_MBED_ENABLED)
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#endif
#endif
#include <osdep_service.h>
#include <wlan_intf.h>
#ifdef WIFI_PERFORMANCE_MONITOR
#include <wifi_performance_monitor.h>
#else
#define WIFI_MONITOR_TIMER_START(x)
#define WIFI_MONITOR_TIMER_END(x, len)
#endif

#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
#include "inic_ipc_dev_trx.h"
#include "inic_ipc_api.h"
#endif

/**
 *      rltk_wlan_set_netif_info - set netif hw address and register dev pointer to netif device
 *      @idx_wlan: netif index
 *			    0 for STA only or SoftAP only or STA in STA+SoftAP concurrent mode,
 *			    1 for SoftAP in STA+SoftAP concurrent mode
 *      @dev: register netdev pointer to LWIP. Reserved.
 *      @dev_addr: set netif hw address
 *
 *      Return Value: None
 */
void rltk_wlan_set_netif_info(int idx_wlan, void *dev, unsigned char *dev_addr)
{
#if (CONFIG_LWIP_LAYER == 1)
#if defined(CONFIG_MBED_ENABLED)
	//rtw_memcpy(xnetif[idx_wlan]->hwaddr, dev_addr, 6);
	//set netif hwaddr later
#else
	LwIP_wlan_set_netif_info(idx_wlan, dev, dev_addr);
#endif
#else
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	inic_ipc_dev_set_netif_info(idx_wlan, dev_addr);
#endif
#endif
}

/**
 *      rltk_wlan_send - send IP packets to WLAN. Called by low_level_output().
 *      @idx: netif index
 *      @sg_list: data buffer list
 *      @sg_len: size of each data buffer
 *      @total_len: total data len
 *
 *      Return Value: None
 */
int rltk_wlan_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len)
{
#if (CONFIG_LWIP_LAYER == 1)
	struct eth_drv_sg *last_sg;
	struct sk_buff *skb = NULL;
	int ret = 0;

	WIFI_MONITOR_TIMER_START(wifi_time_test.wlan_send_time);
	if (idx == -1) {
		DBG_ERR("netif is DOWN");
		return -1;
	}
	DBG_TRACE("%s is called", __FUNCTION__);

	save_and_cli();
	if (rltk_wlan_check_isup(idx)) {
		rltk_wlan_tx_inc(idx);
	} else {
		DBG_ERR("netif is DOWN");
		restore_flags();
		return -1;
	}
	restore_flags();

	WIFI_MONITOR_TIMER_START(wifi_time_test.wlan_send_time1);
	skb = rltk_wlan_alloc_skb(total_len);
	WIFI_MONITOR_TIMER_END(wifi_time_test.wlan_send_time1, total_len);
	if (skb == NULL) {
		//DBG_ERR("rltk_wlan_alloc_skb() for data len=%d failed!", total_len);
		ret = -1;
		goto exit;
	}
	WIFI_MONITOR_TIMER_START(wifi_time_test.wlan_send_time2);
	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		rtw_memcpy(skb->tail, (void *)(sg_list->buf), sg_list->len);
		skb_put(skb,  sg_list->len);
	}
	WIFI_MONITOR_TIMER_END(wifi_time_test.wlan_send_time2, total_len);

	WIFI_MONITOR_TIMER_START(wifi_time_test.wlan_send_skb_time);
	rltk_wlan_send_skb(idx, skb);
	WIFI_MONITOR_TIMER_END(wifi_time_test.wlan_send_skb_time, total_len);
	WIFI_MONITOR_TIMER_END(wifi_time_test.wlan_send_time, total_len);

exit:
	save_and_cli();
	rltk_wlan_tx_dec(idx);
	restore_flags();
	return ret;
#endif
}

/**
 *      rltk_wlan_recv - indicate packets to LWIP. Called by ethernetif_recv().
 *      @idx: netif index
 *      @sg_list: data buffer list
 *      @sg_len: size of each data buffer
 *
 *      Return Value: None
 */
void rltk_wlan_recv(int idx, struct eth_drv_sg *sg_list, int sg_len)
{
#if (CONFIG_LWIP_LAYER == 1)
	struct eth_drv_sg *last_sg;
	struct sk_buff *skb;

	DBG_TRACE("%s is called", __FUNCTION__);
	if (idx == -1) {
		DBG_ERR("skb is NULL");
		return;
	}
	skb = rltk_wlan_get_recv_skb(idx);
	DBG_ASSERT(skb, "No pending rx skb");

	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		if (sg_list->buf != 0) {
			rtw_memcpy((void *)(sg_list->buf), skb->data, sg_list->len);
			skb_pull(skb, sg_list->len);
		}
	}
#endif
}

int netif_is_valid_IP(int idx, unsigned char *ip_dest)
{
#if defined(CONFIG_MBED_ENABLED)
	return 1;
#elif (CONFIG_LWIP_LAYER == 1)
	return LwIP_netif_is_valid_IP(idx, ip_dest);
#elif defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	return inic_ipc_get_lwip_info(IPC_WLAN_IS_VALID_IP, ip_dest, idx);
#else
	return 1;
#endif
}

#if !defined(CONFIG_MBED_ENABLED)
unsigned char *netif_get_hwaddr(int idx_wlan)
{
#if (CONFIG_LWIP_LAYER == 1)
	return LwIP_GetMAC(idx_wlan);
#elif defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	return (unsigned char *)inic_ipc_get_lwip_info(IPC_WLAN_GET_HW_ADDR, NULL, idx_wlan);
#else
	return NULL;
#endif
}
#endif

#if defined(CONFIG_MBED_ENABLED)
emac_callback emac_callback_func = NULL;
void *emac_callback_data = NULL;
void set_callback_func(emac_callback p, void *data)
{
	emac_callback_func = p;
	emac_callback_data = data;
}
#endif

void netif_rx(int idx, unsigned int len)
{
	WIFI_MONITOR_TIMER_START(wifi_time_test.netif_rx_time);
#if (CONFIG_LWIP_LAYER == 1)
#if defined(CONFIG_MBED_ENABLED)
	emac_callback_func(emac_callback_data, NULL, len);
#else
	LwIP_ethernetif_recv(idx, len);
#endif
#endif

#ifdef CONFIG_INIC_EN
#if (CONFIG_INIC_IPC)
	inic_ipc_dev_recv(idx);
#else
	inic_netif_rx(idx, len);
#endif
#endif
	WIFI_MONITOR_TIMER_END(wifi_time_test.netif_rx_time, len);
}

#ifdef CONFIG_WOWLAN
unsigned char *rltk_wlan_get_ip(int idx)
{
#if (CONFIG_LWIP_LAYER == 1)
	return LwIP_GetIP(idx);
#elif defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	return (unsigned char *)inic_ipc_get_lwip_info(IPC_WLAN_GET_IP, NULL, idx);
#else
	return NULL;
#endif
}
unsigned char *rltk_wlan_get_gw(int idx)
{
#if (CONFIG_LWIP_LAYER == 1)
	return LwIP_GetGW(idx);
#elif defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	return (unsigned char *)inic_ipc_get_lwip_info(IPC_WLAN_GET_GW, NULL, idx);
#else
	return NULL;
#endif
}

unsigned char *rltk_wlan_get_gwmask(int idx)
{
#if (CONFIG_LWIP_LAYER == 1)
	return LwIP_GetMASK(idx);
#elif defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	return (unsigned char *)inic_ipc_get_lwip_info(IPC_WLAN_GET_GWMSK, NULL, idx);
#else
	return NULL;
#endif
}
#endif

