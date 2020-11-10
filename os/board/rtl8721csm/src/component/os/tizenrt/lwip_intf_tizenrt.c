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

#include <lwip_intf_tizenrt.h>
#include <lwip/init.h>
#include <lwip/netif.h>
#include <osdep_service.h>
#include "autoconf.h"

#include <tinyara/netmgr/netdev_mgr.h>
#include <sys/socket.h>
#include <netdev_mgr_internal.h>
#include "wifi_constants.h"
#include <net/if.h>

#ifndef WLAN0_NAME
#define WLAN0_NAME "wlan0"
#endif
#ifndef WLAN1_NAME
#define WLAN1_NAME "wlan1"
#endif

#ifndef GET_NETIF_FROM_NETDEV
#define GET_NETIF_FROM_NETDEV(dev) (struct netif *)(((struct netdev_ops *)(dev)->ops)->nic)
#endif

extern struct netdev *ameba_nm_dev_wlan0;
extern struct netdev *ameba_nm_dev_wlan1;

//----- ------------------------------------------------------------------
// External Reference
//----- ------------------------------------------------------------------

extern rtw_mode_t wifi_mode;

static struct netdev *rtk_get_netdev(int idx)
{
	if (wifi_mode == RTW_MODE_STA_AP && idx == 1)
	    return ameba_nm_dev_wlan1;
	else
	    return ameba_nm_dev_wlan0;
}

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
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx_wlan);

	if (!dev_tmp) {
		DiagPrintf("[rltk_wlan_set_netif_info] get dev fail\n");
	}
	netdev_set_hwaddr(dev_tmp, dev_addr, IFHWADDRLEN);
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
#ifdef CONFIG_NET_NETMGR
	struct eth_drv_sg *last_sg;
	struct sk_buff *skb = NULL;
	int ret = 0;

#ifdef CONFIG_TX_ZERO_COPY
	u8 *data = NULL;
	u8 *Mac_data = NULL;
#endif

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

#ifdef CONFIG_TX_ZERO_COPY
	data = rtw_malloc(1640);
	if (data == NULL) {
		ndbg("\r\n###########ERR_MEM at %s\r\n", __func__);
		ret = -1;
		goto exit;
	}

	Mac_data = data + 80;

	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		rtw_memcpy(Mac_data, (void *)(sg_list->buf), sg_list->len);
		Mac_data += sg_list->len;
	}

	skb = (struct sk_buff *)rltk_wlan_alloc_skb_0copy();
	if (skb == NULL) {
		ndbg("\r\nrltk_wlan_alloc_skb()failed!\r\n");
		goto exit;
	}
	skb->data = data + 80;
	skb->head = data;
	skb->end = data + 1640;
	skb->tail = data + 80 + total_len;
	skb->len = total_len;
	ret = 0;
#else
	skb = rltk_wlan_alloc_skb(total_len);
#endif

	if (skb == NULL) {
		ret = -1;
		goto exit;
	}

#ifndef CONFIG_TX_ZERO_COPY
	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		rtw_memcpy(skb->tail, (void *)(sg_list->buf), sg_list->len);
		skb_put(skb, sg_list->len);
	}
#endif

	rltk_wlan_send_skb(idx, skb);

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
#ifdef CONFIG_NET_NETMGR
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
	return 1;//For net manager, always Valid
}

#ifdef CONFIG_NET_NETMGR
int get_idx_from_dev(struct netdev *dev)
{
	switch (wifi_mode) {
	case RTW_MODE_STA_AP:
		if (!strcmp(WLAN0_NAME, dev->ifname))
			return 0;
		else if(!strcmp(WLAN1_NAME, dev->ifname))
			return 1;
		else
			return -1;
	default:
		if (!strcmp(WLAN0_NAME, dev->ifname))
			return 0;
		else
			return -1;
	}
}
#endif
#if !defined(CONFIG_MBED_ENABLED)
int netif_get_idx(struct netif *pnetif)
{
	return -1;
}

unsigned char *netif_get_hwaddr(int idx_wlan)
{
	struct netdev *dev_tmp = NULL;
	unsigned char *dev_addr;

	dev_tmp = rtk_get_netdev(idx_wlan);

	if (!dev_tmp) {
		DiagPrintf("[netif_get_hwaddr] get dev fail\n");
	}
	netdev_get_hwaddr(dev_tmp, dev_addr, (unsigned char*)IFHWADDRLEN);
	return dev_addr;
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
	struct sk_buff *skb;
	skb = rltk_wlan_get_recv_skb(idx);

	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);

	netdev_input(dev_tmp, skb->data, skb->len);

#if (CONFIG_INIC_EN == 1)
	inic_netif_rx(idx, len);
#endif
}

void netif_post_sleep_processing(void)
{
}

void netif_pre_sleep_processing(void)
{
}

#ifdef CONFIG_WOWLAN
unsigned char *rltk_wlan_get_ip(int idx)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);
	
	if (!dev_tmp) {
		DiagPrintf("[rltk_wlan_get_ip] get dev fail\n");
	}

	struct netif *ni = (struct netif *)(((struct netdev_ops *)(dev_tmp)->ops)->nic);

	return (uint8_t *) &(ni->ip_addr);
}
unsigned char *rltk_wlan_get_gw(int idx)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);
	
	if (!dev_tmp) {
		DiagPrintf("[rltk_wlan_get_ip] get dev fail\n");
	}

	struct netif *ni = (struct netif *)(((struct netdev_ops *)(dev_tmp)->ops)->nic);
	return (uint8_t *) &(ni->gw);
}

unsigned char *rltk_wlan_get_gwmask(int idx)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);
	
	if (!dev_tmp) {
		DiagPrintf("[rltk_wlan_get_ip] get dev fail\n");
	}

	struct netif *ni = (struct netif *)(((struct netdev_ops *)(dev_tmp)->ops)->nic);
	return (uint8_t *) &(ni->netmask);
}
#endif
