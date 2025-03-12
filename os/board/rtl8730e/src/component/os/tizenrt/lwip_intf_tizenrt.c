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
#include <lwip/pbuf.h>
#include <os_wrapper.h>
#include "rtw_autoconf.h"
//#include "rtw_adapter.h"

#include <tinyara/netmgr/netdev_mgr.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdev_mgr_internal.h>
#include "rtw_wifi_constants.h"
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

//static
struct netdev *rtk_get_netdev(int idx)
{
	if (idx == 1)
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
		rtw_printf("[rltk_wlan_set_netif_info] get dev fail\n");
		return;
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

	unsigned int irq_flags = save_and_cli();
	/*if (rltk_wlan_check_isup(idx)) {
		wifi_if_tx_inc(idx);
	} else {
		DBG_ERR("netif is DOWN");
		restore_flags(irq_flags);
		return -1;
	}*/
	restore_flags(irq_flags);

#ifdef CONFIG_TX_ZERO_COPY
	data = rtw_malloc(1640);
	if (data == NULL) {
		ndbg("\r\n###########ERR_MEM at %s\r\n", __func__);
		ret = -1;
		goto exit;
	}

	Mac_data = data + 80;

	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		memcpy(Mac_data, (void *)(sg_list->buf), sg_list->len);
		Mac_data += sg_list->len;
	}

	skb = (struct sk_buff *)wifi_if_alloc_skb_0copy();
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
	skb = wifi_if_alloc_skb(total_len);
#endif

	if (skb == NULL) {
		ret = -1;
		goto exit;
	}

#ifndef CONFIG_TX_ZERO_COPY
	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		memcpy(skb->tail, (void *)(sg_list->buf), sg_list->len);
		skb_put(skb, sg_list->len);
	}
#endif

	wifi_if_send_skb(idx, skb);

exit:
	irq_flags = save_and_cli();
	//wifi_if_tx_dec(idx);
	restore_flags(irq_flags);
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
	skb = wifi_if_get_recv_skb(idx);
	DBG_ASSERT(skb, "No pending rx skb");

	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		if (sg_list->buf != 0) {
			memcpy((void *)(sg_list->buf), skb->data, sg_list->len);
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
	if (!strcmp(WLAN0_NAME, dev->ifname))
		return 0;
	else if(!strcmp(WLAN1_NAME, dev->ifname))
		return 1;
	else
		return -1;
}
#endif
#if !defined(CONFIG_MBED_ENABLED)
#if 0
int netif_get_idx(struct netif *pnetif)
{
	return -1;
}
#endif

int netif_get_hwaddr(int idx_wlan, uint8_t *dev_addr)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx_wlan);

	if (!dev_tmp) {
		rtw_printf("[netif_get_hwaddr] get dev fail\n");
		return -1;
	}
	if (netdev_get_hwaddr(dev_tmp, dev_addr, (unsigned char*)IFHWADDRLEN) == 0)
		return 0;
	else
		return -1;
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
//wlan driver has included this function, won't go to here
void netif_rx(int idx, unsigned int len)
{
#ifdef CONFIG_INIC_EN
#if defined(CONFIG_AS_INIC_AP)
	inic_ipc_dev_recv(idx);
#else
	inic_netif_rx(idx, len);
#endif
#endif

#if 0
	struct sk_buff *skb;
	skb = rltk_wlan_get_recv_skb(idx);

	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);

	netdev_input(dev_tmp, skb->data, skb->len);

#if (CONFIG_INIC_EN == 1)
	inic_netif_rx(idx, len);
#endif
#endif
}

void netif_post_sleep_processing(void)
{
}

void netif_pre_sleep_processing(void)
{
}

unsigned char *rltk_wlan_get_ip(int idx)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);

	if (!dev_tmp) {
		rtw_printf("[rltk_wlan_get_ip] get dev fail\n");
		return NULL;
	}

	struct netif *ni = (struct netif *)(((struct netdev_ops *)(dev_tmp)->ops)->nic);

	return (uint8_t *) &(ni->ip_addr);
}
unsigned char *rltk_wlan_get_gw(int idx)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);

	if (!dev_tmp) {
		rtw_printf("[rltk_wlan_get_ip] get dev fail\n");
		return NULL;
	}

	struct netif *ni = (struct netif *)(((struct netdev_ops *)(dev_tmp)->ops)->nic);
	return (uint8_t *) &(ni->gw);
}

unsigned char *rltk_wlan_get_gwmask(int idx)
{
	struct netdev *dev_tmp = NULL;

	dev_tmp = rtk_get_netdev(idx);

	if (!dev_tmp) {
		rtw_printf("[rltk_wlan_get_ip] get dev fail\n");
		return NULL;
	}

	struct netif *ni = (struct netif *)(((struct netdev_ops *)(dev_tmp)->ops)->nic);
	return (uint8_t *) &(ni->netmask);
}

void *rltk_pbuf_wrapper(struct sk_buff *skb, u16_t *len)
{
    struct pbuf *p_buf = NULL, *temp_buf = NULL;
	p_buf = pbuf_alloc(PBUF_RAW, skb->len, PBUF_POOL);
	if (p_buf == NULL) {
        return NULL;
	}

	/* copy data from skb(ipc data) to pbuf(ether net data) */
	temp_buf = p_buf;
	while (temp_buf) {
		/* If tot_len > PBUF_POOL_BUFSIZE_ALIGNED, the skb will be
		 * divided into several pbufs. Therefore, there is a while to
		 * use to assign data to pbufs.
		 */
		memcpy(temp_buf->payload, skb->data, temp_buf->len);
		skb_pull(skb, temp_buf->len);
		temp_buf = temp_buf->next;
	}
    if (len != NULL){
        *len = p_buf->len;
    }
    return (void*)p_buf;
}

void rltk_wlan_indicate_lwip(int idx_wlan, void *p_buf)
{
    /* TizenRT gets netif from netdev */
    /* Currently TizenRT only uses idx 0, remove below line if TizenRT supports concurrent */
    idx_wlan = 0;
    struct netdev *dev_tmp = NULL;
    dev_tmp = (struct netdev *)rtk_get_netdev(idx_wlan);
    struct netif *netif = GET_NETIF_FROM_NETDEV(dev_tmp);
    if (netif->input((struct pbuf *)p_buf, netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("input processing error\n"));
        LINK_STATS_INC(link.err);
        pbuf_free((struct pbuf *)p_buf);
    } else {
        LINK_STATS_INC(link.recv);
    }
}