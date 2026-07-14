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

#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdev_mgr_internal.h>

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

static struct netdev *rtk_get_netdev(int idx)
{
	if (idx == 1)
	    return ameba_nm_dev_wlan1;
	else
	    return ameba_nm_dev_wlan0;
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

void *rltk_pbuf_wrapper(struct sk_buff *skb, uint16_t *len)
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
#ifndef CONFIG_ENABLE_HOMELYNK
    idx_wlan = 0;
#endif //#ifndef CONFIG_ENABLE_HOMELYNK
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