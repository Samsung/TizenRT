/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/clock.h>
#include <tinyara/net/net.h>
#include <arpa/inet.h>
#include <net/lwip/netif/etharp.h>
#include <net/lwip/ethip6.h>
#include <net/lwip/igmp.h>

#include "debug_scsc.h"
#include "netif.h"
#include "dev.h"
#include "mgt.h"
#include "scsc_wifi_fcq.h"
#include "t20_ops.h"

#define IP4_OFFSET_TO_TOS_FIELD 1

/* Net Device callback operations */
static int slsi_net_open(struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	int err;

	SLSI_INFO(dev, "iface_num = %d\n", ndev_vif->ifnum);

	if (WARN_ON(ndev_vif->is_available)) {
		return -EINVAL;
	}

	if (sdev->mlme_blocked) {
		SLSI_NET_DBG2(dev, SLSI_NETDEV, "MLME Blocked. Reject net_open\n");
		return -EIO;
	}
	err = slsi_start(sdev);
	if (WARN_ON(err)) {
		return err;
	}

	if (!sdev->netdev_up_count) {
		slsi_get_hw_mac_address(sdev, sdev->hw_addr);
		SLSI_INFO(sdev, "Configure MAC address to [" SLSI_MAC_FORMAT "]\n", SLSI_MAC_STR(sdev->hw_addr));

		/* Assign Addresses */
		SLSI_ETHER_COPY(sdev->netdev_addresses[SLSI_NET_INDEX_WLAN], sdev->hw_addr);
#ifdef CONFIG_SCSC_ENABLE_P2P
		SLSI_ETHER_COPY(sdev->netdev_addresses[SLSI_NET_INDEX_P2P], sdev->hw_addr);
		sdev->netdev_addresses[SLSI_NET_INDEX_P2P][0] |= 0x02;	/* Set the local bit */

		SLSI_ETHER_COPY(sdev->netdev_addresses[SLSI_NET_INDEX_P2PX], sdev->hw_addr);
		sdev->netdev_addresses[SLSI_NET_INDEX_P2PX][0] |= 0x02;	/* Set the local bit */
		sdev->netdev_addresses[SLSI_NET_INDEX_P2PX][4] ^= 0x80;	/* EXOR 5th byte with 0x80 */
#endif
	}

	SLSI_ETHER_COPY(dev->d_mac.ether_addr_octet, sdev->netdev_addresses[SLSI_NET_INDEX_WLAN]);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ndev_vif->is_available = true;
	sdev->netdev_up_count++;

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	memcpy(dev->hwaddr, sdev->hw_addr, ETHARP_HWADDR_LEN);
	dev->flags |= NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_BROADCAST | NETIF_FLAG_IGMP;

	/* Put the network interface in the UP state */
	netif_set_up(dev);
	reinit_completion(&ndev_vif->sig_wait.completion);

	/* Send interface enabled event to supplicant if it is present. This could happen if ifdown-ifup is done
	 * explicilty while supplicant is running */
	if (sdev->drv != NULL) {
		wpa_supplicant_event_send(sdev->drv->ctx, EVENT_INTERFACE_ENABLED, NULL);
	}

	return 0;
}

static int slsi_net_stop(struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;

	SLSI_NET_DBG1(dev, SLSI_NETDEV, "\n");

	/* Put the network interface in the DOWN state */
	netif_set_down(dev);

	if (!ndev_vif->is_available) {
		/* May have been taken out by the Chip going down */
		SLSI_NET_DBG1(dev, SLSI_NETDEV, "netdev vif not available.\n");
		return 0;
	}

	slsi_stop_net_dev(sdev, dev);

	/* Send interface disabled event to supplicant if ifdown is not triggered by it */
	if ((sdev->is_supplicant_deinit != 1) && (sdev->drv != NULL)) {
		wpa_supplicant_event_send(sdev->drv->ctx, EVENT_INTERFACE_DISABLED, NULL);
	}

	/* Free country list allocated during slsi_start */
	slsi_regd_deinit(sdev);

	return 0;
}

/* This is called after the WE handlers */
static int slsi_net_ioctl(struct netif *dev, struct ifreq *rq, int cmd)
{
	SLSI_NET_DBG1(dev, SLSI_NETDEV, "ioctl cmd:0x%.4x\n", cmd);

	if (cmd == 0x89f0 + 1) {
		return 0;
	}

	return -EOPNOTSUPP;
}

static u16 slsi_get_priority_from_tos(u8 *frame, u16 proto)
{
	if (WARN_ON(!frame)) {
		return FAPI_PRIORITY_QOS_UP0;
	}

	switch (proto) {
	case 0x0800:				/* IPv4 */
	case 0x814C:				/* SNMP */
	case 0x880C:				/* GSMP */
		return (u16)(((frame[IP4_OFFSET_TO_TOS_FIELD]) & 0xE0) >> 5);

	case 0x8100:				/* VLAN */
		return (u16)((*frame & 0xE0) >> 5);

	case 0x86DD:				/* IPv6 */
		return (u16)((*frame & 0x0E) >> 1);

	default:
		return FAPI_PRIORITY_QOS_UP0;
	}
}

static bool slsi_net_downgrade_ac(struct netif *dev, struct max_buff *mbuf)
{
	SLSI_UNUSED_PARAMETER(dev);

	switch (mbuf->user_priority) {
	case 6:
	case 7:
		mbuf->user_priority = FAPI_PRIORITY_QOS_UP5; /* VO -> VI */
		return true;
	case 4:
	case 5:
		mbuf->user_priority = FAPI_PRIORITY_QOS_UP3; /* VI -> BE */
		return true;
	case 0:
	case 3:
		mbuf->user_priority = FAPI_PRIORITY_QOS_UP2; /* BE -> BK */
		return true;
	default:
		return false;
	}
}

static u8 slsi_net_up_to_ac_mapping(u8 priority)
{
	switch (priority) {
	case FAPI_PRIORITY_QOS_UP6:
	case FAPI_PRIORITY_QOS_UP7:
		return BIT(FAPI_PRIORITY_QOS_UP6) | BIT(FAPI_PRIORITY_QOS_UP7);
	case FAPI_PRIORITY_QOS_UP4:
	case FAPI_PRIORITY_QOS_UP5:
		return BIT(FAPI_PRIORITY_QOS_UP4) | BIT(FAPI_PRIORITY_QOS_UP5);
	case FAPI_PRIORITY_QOS_UP0:
	case FAPI_PRIORITY_QOS_UP3:
		return BIT(FAPI_PRIORITY_QOS_UP0) | BIT(FAPI_PRIORITY_QOS_UP3);
	default:
		return BIT(FAPI_PRIORITY_QOS_UP1) | BIT(FAPI_PRIORITY_QOS_UP2);
	}
}

enum slsi_traffic_q slsi_frame_priority_to_ac_queue(u16 priority)
{
	switch (priority) {
	case FAPI_PRIORITY_QOS_UP0:
	case FAPI_PRIORITY_QOS_UP3:
		return SLSI_TRAFFIC_Q_BE;
	case FAPI_PRIORITY_QOS_UP1:
	case FAPI_PRIORITY_QOS_UP2:
		return SLSI_TRAFFIC_Q_BK;
	case FAPI_PRIORITY_QOS_UP4:
	case FAPI_PRIORITY_QOS_UP5:
		return SLSI_TRAFFIC_Q_VI;
	case FAPI_PRIORITY_QOS_UP6:
	case FAPI_PRIORITY_QOS_UP7:
		return SLSI_TRAFFIC_Q_VO;
	default:
		return SLSI_TRAFFIC_Q_BE;
	}
}

int slsi_ac_to_tids(enum slsi_traffic_q ac, int *tids)
{
	switch (ac) {
	case SLSI_TRAFFIC_Q_BE:
		tids[0] = FAPI_PRIORITY_QOS_UP0;
		tids[1] = FAPI_PRIORITY_QOS_UP3;
		break;

	case SLSI_TRAFFIC_Q_BK:
		tids[0] = FAPI_PRIORITY_QOS_UP1;
		tids[1] = FAPI_PRIORITY_QOS_UP2;
		break;

	case SLSI_TRAFFIC_Q_VI:
		tids[0] = FAPI_PRIORITY_QOS_UP4;
		tids[1] = FAPI_PRIORITY_QOS_UP5;
		break;

	case SLSI_TRAFFIC_Q_VO:
		tids[0] = FAPI_PRIORITY_QOS_UP6;
		tids[1] = FAPI_PRIORITY_QOS_UP7;
		break;
	}

	return 0;
}

static void slsi_net_downgrade_pri(struct netif *dev, struct slsi_peer *peer, struct max_buff *mbuf)
{
	/* in case we are a client downgrade the ac if acm is
	 * set and tspec is not established
	 */
	while ((peer->wmm_acm & BIT(mbuf->user_priority)) && !(peer->tspec_established & slsi_net_up_to_ac_mapping(mbuf->user_priority))) {
		SLSI_NET_DBG3(dev, SLSI_TX, "Downgrading from UP:%d\n", mbuf->user_priority);
		if (!slsi_net_downgrade_ac(dev, mbuf)) {
			break;
		}
	}
	SLSI_NET_DBG3(dev, SLSI_TX, "To UP:%d\n", mbuf->user_priority);
}

static u16 slsi_net_select_queue(struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	u16 netif_q = 0;
	struct ethhdr *ehdr = (struct ethhdr *)slsi_mbuf_get_data(mbuf);
	int proto = 0;
	struct slsi_peer *peer;

	proto = ntohs(eth_hdr(mbuf)->h_proto);
	switch (proto) {
	default:
		/* SLSI_NETIF_Q_PRIORITY is used only for EAP, ARP and IP frames with DHCP */
		break;
	case ETH_P_PAE:
	case ETH_P_WAI:
		SLSI_NET_DBG3(dev, SLSI_TX, "EAP packet. Priority Queue Selected\n");
		return SLSI_NETIF_Q_PRIORITY;
	case ETH_P_ARP:
		SLSI_NET_DBG3(dev, SLSI_TX, "ARP frame. Priority Queue Selected\n");
		return SLSI_NETIF_Q_PRIORITY;
	case ETH_P_IP:
		if (slsi_is_dhcp_packet(slsi_mbuf_get_data(mbuf)) == SLSI_TX_IS_NOT_DHCP) {
			break;
		}
		SLSI_NET_DBG3(dev, SLSI_TX, "DHCP packet. Priority Queue Selected\n");
		return SLSI_NETIF_Q_PRIORITY;
	}

	if (ndev_vif->vif_type == FAPI_VIFTYPE_AP)
		/* MULTICAST/BROADCAST Queue is only used for AP */
		if (is_multicast_ether_addr(ehdr->h_dest)) {
			SLSI_NET_DBG3(dev, SLSI_TX, "Multicast AC queue will be selected\n");
			mbuf->user_priority = slsi_get_priority_from_tos(slsi_mbuf_get_data(mbuf) + ETH_HLEN, proto);
			return slsi_netif_get_multicast_queue(slsi_frame_priority_to_ac_queue(mbuf->user_priority));
		}

	SLSI_MUTEX_LOCK(ndev_vif->peer_lock);
	peer = slsi_get_peer_from_mac(sdev, dev, ehdr->h_dest);
	if (!peer) {
		SLSI_NET_DBG3(dev, SLSI_TX, "Peer NOT found : " SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(ehdr->h_dest));
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
		return SLSI_NETIF_Q_DISCARD;
	}

	if (peer->qos_enabled) {
		mbuf->user_priority = slsi_get_priority_from_tos(slsi_mbuf_get_data(mbuf) + ETH_HLEN, proto);
	} else {
		mbuf->user_priority = FAPI_PRIORITY_QOS_UP0;
	}

	/* Downgrade the priority if acm bit is set and tspec is not established */
	slsi_net_downgrade_pri(dev, peer, mbuf);

	netif_q = slsi_netif_get_peer_queue(peer->queueset, slsi_frame_priority_to_ac_queue(mbuf->user_priority));
	SLSI_NET_DBG3(dev, SLSI_TX, "%u Queue Selected\n", netif_q);
	SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
	return netif_q;
}

/**
 * Frame Eapol header and send eapol packet
 */
int eth_send_eapol(const u8 *src, const u8 *dst, const u8 *buf, u16 len, u16 proto)
{
	struct max_buff *mbuf = NULL;
	int alloc_size;
	struct slsi_dev *sdev = cm_ctx.sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);

	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);
	alloc_size = len + (fapi_sig_size(ma_unitdata_req) + 160) + sizeof(struct ethhdr);

	mbuf = mbuf_alloc(alloc_size);
	if (!mbuf) {
		SLSI_NET_ERR(dev, "Failed to allocate memory, alloc_size = %d\n", alloc_size);
		return -ENOMEM;
	}

	mbuf_reserve_headroom(mbuf, (fapi_sig_size(ma_unitdata_req) + 160));
	fapi_append_data(mbuf, dst, ETH_ALEN);
	fapi_append_data(mbuf, src, ETH_ALEN);
	proto = htons(proto);
	fapi_append_data(mbuf, (u8 *)&proto, 2);
	fapi_append_data(mbuf, buf, len);

	mbuf->ac_queue = SLSI_NETIF_Q_PRIORITY;
	mbuf_set_mac_header(mbuf, 0);
	slsi_tx_data(sdev, dev, mbuf);
	slsi_kfree_mbuf(mbuf);

	return ERR_OK;
}

void slsi_ethernetif_input(struct netif *dev, u8_t *frame_ptr, u16_t len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;

	SLSI_MUTEX_LOCK(sdev->rx_data_mutex);

	dev->d_buf = frame_ptr;
	dev->d_len = len;

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_packets_given_to_lwip);
	ethernetif_input(dev);

	SLSI_MUTEX_UNLOCK(sdev->rx_data_mutex);
}

static err_t slsi_linkoutput(struct netif *dev, struct pbuf *buf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct max_buff *mbuf = NULL;
	struct pbuf *next_buf;
	int offset = 0;
	int ret = ERR_OK;
	u8 *mbuf_data;

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_linkoutput_packets);

	if (!ndev_vif->is_available) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_vif_not_available);
		SLSI_NET_ERR(dev, "netdev vif not available\n");

		ret = ERR_CONN;
		goto exit;
	}

	if ((buf == NULL) || (buf->tot_len == 0) || (buf->len == 0)) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_invalid_pbuf);
		SLSI_NET_ERR(dev, "Invalid pbuf\n");

		ret = ERR_VAL;
		goto exit;
	}

	SLSI_NET_DBG3(dev, SLSI_TX, "tot_len: %d, len: %d\n", buf->tot_len, buf->len);

	if (sdev->tx_mbuf == NULL) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_null_mbuf);
		SLSI_NET_ERR(dev, "TX mbuf is NULL\n");

		ret = ERR_MEM;
		goto exit;
	}

	if (buf->tot_len + (fapi_sig_size(ma_unitdata_req) + 160) > SLSI_TX_MBUF_SIZE) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_large_pbuf);
		SLSI_NET_ERR(dev, "Large packet: %d\n", buf->tot_len);

		ret = ERR_VAL;
		goto exit;
	}

	mbuf = sdev->tx_mbuf;
	mbuf_reset(mbuf);
	mbuf_reserve_headroom(mbuf, (fapi_sig_size(ma_unitdata_req) + 160));
	mbuf_put(mbuf, buf->tot_len);

	mbuf_data = slsi_mbuf_get_data(mbuf);
	/* Copy the data from multiple buffer */
	next_buf = buf;
	while (next_buf) {
		if (offset >= buf->tot_len) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_wrong_length);
			SLSI_NET_ERR(dev, "buffer has more data than tol_len. offset:%d, tot_len: %d\n", offset, buf->tot_len);

			ret = ERR_VAL;
			goto exit;
		}
		memcpy(&mbuf_data[offset], next_buf->payload, next_buf->len);
		offset += next_buf->len;
		next_buf = next_buf->next;
	}

	mbuf_set_mac_header(mbuf, 0);
	mbuf->ac_queue = slsi_net_select_queue(dev, mbuf);

	if (mbuf->ac_queue == SLSI_NETIF_Q_DISCARD) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_discard_queue);

		ret = ERR_CONN;
		goto exit;
	}

	ret = slsi_tx_data(ndev_vif->sdev, dev, mbuf);
	if (ret != 0) {
		ret = ERR_MEM;
		goto exit;
	}

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_success_packet);

	return ERR_OK;
exit:
	return ret;
}

#define MULTICAST_IP_TO_MAC(ip) { (u8)0x01, \
				  (u8)0x00, \
				  (u8)0x5e, \
				  (u8)((ip)[1] & 0x7F), \
				  (u8)(ip)[2], \
				  (u8)(ip)[3] \
}
static err_t slsi_set_multicast_list(struct netif *dev, const ip4_addr_t *group, u8_t action)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	u8 addr[ETH_ALEN] = MULTICAST_IP_TO_MAC((u8 *)group);

	if (ndev_vif->vif_type != FAPI_VIFTYPE_STATION) {
		return 0;
	}

	if (!ndev_vif->is_available) {
		SLSI_NET_DBG1(dev, SLSI_NETDEV, "Not available\n");
		return 0;
	}

	if (IGMP_ADD_MAC_FILTER) {
		SLSI_NET_DBG3(dev, SLSI_NETDEV, "Add filter for mac address = %pM\n", addr);
		return slsi_set_multicast_packet_filters(sdev, dev, addr);
	} else {
		SLSI_NET_DBG3(dev, SLSI_NETDEV, "Clear filter for mac address = %pM\n", addr);
		return slsi_clear_packet_filters(sdev, dev);
	}
}

static struct netif *slsi_alloc_netdev(int sizeof_priv)
{
	struct netif *dev;
	void *priv;

	SLSI_INFO_NODEV("slsi_alloc_netdev\n");

	dev = kmm_zalloc(sizeof(struct netif));
	if (dev == NULL) {
		return NULL;
	}

	priv = kmm_zalloc(sizeof_priv);
	if (priv == NULL) {
		kmm_free(dev);
		return NULL;
	}

	/* Initialize the driver structure */
	dev->d_private = priv;
	dev->d_ifup = slsi_net_open;
	dev->d_ifdown = slsi_net_stop;
	dev->linkoutput = slsi_linkoutput;
	dev->output = etharp_output;
#if LWIP_IPV6
	dev->output_ip6 = ethip6_output;
#endif
	dev->igmp_mac_filter = slsi_set_multicast_list;
#ifdef CONFIG_NETDEV_PHY_IOCTL
	dev->d_ioctl = slsi_net_ioctl;
#endif
	dev->mtu = CONFIG_NET_ETH_MTU;
	dev->hwaddr_len = IFHWADDRLEN;

	return dev;
}

static void slsi_free_netdev(struct netif *dev)
{
	if (dev->d_private) {
		kmm_free(dev->d_private);
	}

	kmm_free(dev);
}

static int slsi_netif_add_locked(struct slsi_dev *sdev, int ifnum)
{
	struct netif *dev = NULL;
	struct netdev_vif *ndev_vif;
	int alloc_size, ret;
	int i;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(sdev->netdev_add_remove_mutex));

	if (WARN_ON(!sdev || ifnum > CONFIG_SCSC_WLAN_MAX_INTERFACES || sdev->netdev[ifnum])) {
		return -EINVAL;
	}

	SLSI_INFO_NODEV("Add:" SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(sdev->netdev_addresses[ifnum]));

	alloc_size = sizeof(struct netdev_vif);

	dev = slsi_alloc_netdev(alloc_size);
	if (dev == NULL) {
		SLSI_ERR(sdev, "Failed to allocate private data for netdev\n");
		return -ENOMEM;
	}

	ndev_vif = netdev_priv(dev);
	memset(ndev_vif, 0x00, sizeof(*ndev_vif));
	SLSI_MUTEX_INIT(ndev_vif->vif_mutex);
	SLSI_MUTEX_INIT(ndev_vif->scan_mutex);
	mbuf_queue_head_init(&ndev_vif->ba_complete);
	slsi_sig_send_init(&ndev_vif->sig_wait);
	ndev_vif->sdev = sdev;
	ndev_vif->ifnum = ifnum;
	ndev_vif->vif_type = SLSI_VIFTYPE_UNSPECIFIED;
#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
	SLSI_MUTEX_INIT(ndev_vif->powermgt_lock);
#endif
#ifndef CONFIG_SCSC_WLAN_BLOCK_IPV6
	SLSI_MUTEX_INIT(ndev_vif->ipv6addr_lock);
#endif
	pthread_mutex_init(&ndev_vif->peer_lock, NULL);
	ndev_vif->ba_flush = 0;

	/* Reserve memory for the peer database - Not required for p2p0 interface */
	if (!SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		int queueset;

		for (queueset = 0; queueset < SLSI_AP_PEER_CONNECTIONS_MAX; queueset++) {
			ndev_vif->peer_sta_record[queueset] = kmm_zalloc(sizeof(*ndev_vif->peer_sta_record[queueset]));

			if (ndev_vif->peer_sta_record[queueset] == NULL) {
#if SLSI_AP_PEER_CONNECTIONS_MAX > 1
				int j;

				/* Free previously allocated peer database memory till current queueset */
				for (j = 0; j < queueset; j++) {
					kmm_free(ndev_vif->peer_sta_record[j]);
					ndev_vif->peer_sta_record[j] = NULL;
				}
#endif
				SLSI_NET_ERR(dev, "Could not allocate memory for peer entry (queueset:%d)\n", queueset);
				ret = -ENOMEM;
				goto exit_with_error;
			}
		}
	}

	SLSI_INFO_NODEV("ifnum=%d\n", ndev_vif->ifnum);

#ifdef CONFIG_SCSC_ENABLE_P2P
	/* For p2p0 interface */
	else if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		ret = slsi_p2p_init(sdev, ndev_vif);
		if (ret) {
			goto exit_with_error;
		}
	}
#endif

	ret = slsi_mbuf_work_init(sdev, dev, &ndev_vif->rx_data, "slsi_wlan_rx_data", slsi_rx_netdev_data_work);
	if (ret) {
		goto exit_with_error;
	}
	ret = slsi_mbuf_work_init(sdev, dev, &ndev_vif->rx_mlme, "slsi_wlan_rx_mlme", slsi_rx_netdev_mlme_work);
	if (ret) {
		slsi_mbuf_work_deinit(&ndev_vif->rx_data);
		goto exit_with_error;
	}

	for (i = 0; i < SLSI_SCAN_MAX; i++) {
		mbuf_queue_head_init(&ndev_vif->scan[i].scan_results);
	}

	if (ifnum < CONFIG_SCSC_WLAN_MAX_INTERFACES + 1) {
		SLSI_ETHER_COPY(dev->d_mac.ether_addr_octet, sdev->netdev_addresses[ifnum]);
		sdev->netdev[ifnum] = dev;
	} else {
		goto exit_with_error;
	}

	return 0;

exit_with_error:
	slsi_free_netdev(dev);
	return ret;
}

int slsi_netif_add(struct slsi_dev *sdev, const char *name)
{
	int index = -EINVAL;
	int i;
	int err;

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	for (i = 1; i <= CONFIG_SCSC_WLAN_MAX_INTERFACES; i++)
		if (!sdev->netdev[i]) {
			index = i;
			break;
		}
	if (index > 0) {
		err = slsi_netif_add_locked(sdev, index);
		if (err != 0) {
			index = err;
		}
	}
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
	return index;
}

int slsi_netif_init(struct slsi_dev *sdev)
{
	SLSI_DBG3(sdev, SLSI_NETDEV, "Init\n");

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);

	sdev->netdev[SLSI_NET_INDEX_WLAN] = NULL;

	if (slsi_netif_add_locked(sdev, SLSI_NET_INDEX_WLAN) != 0) {
		SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
		return -EINVAL;
	}
#ifdef CONFIG_SCSC_ENABLE_P2P
	if (slsi_netif_add_locked(sdev, SLSI_NET_INDEX_P2P) != 0) {
		slsi_netif_remove(sdev, sdev->netdev[SLSI_NET_INDEX_WLAN]);
		SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
		return -EINVAL;
	}
#endif

	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
	return 0;
}

void up_wlan_init(struct netif *dev);

static int slsi_netif_register_locked(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int err = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(sdev->netdev_add_remove_mutex));
	SLSI_INFO_NODEV("Register:" SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(dev->d_mac.ether_addr_octet));
	if (ndev_vif->is_registered) {
		SLSI_INFO_NODEV("Register:" SLSI_MAC_FORMAT " Failed: Already registered\n", SLSI_MAC_STR(dev->d_mac.ether_addr_octet));
		return 0;
	}

	up_wlan_init(dev);

	return err;
}

int slsi_netif_register(struct slsi_dev *sdev, struct netif *dev)
{
	int err;

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	err = slsi_netif_register_locked(sdev, dev);
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
	return err;
}

static void slsi_netif_remove_locked(struct slsi_dev *sdev, struct netif *dev)
{
	int i;
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_NET_DBG1(dev, SLSI_NETDEV, "Unregister:" SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(dev->d_mac.ether_addr_octet));

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(sdev->netdev_add_remove_mutex));

	if (ndev_vif->is_registered) {
		slsi_stop_net_dev(sdev, dev);
	}

	sdev->netdev[ndev_vif->ifnum] = NULL;

	/* Free memory of the peer database - Not required for p2p0 interface */
	if (!SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		int queueset;

		for (queueset = 0; queueset < SLSI_AP_PEER_CONNECTIONS_MAX; queueset++) {
			kmm_free(ndev_vif->peer_sta_record[queueset]);
			ndev_vif->peer_sta_record[queueset] = NULL;
		}
	}
#ifdef CONFIG_SCSC_ENABLE_P2P
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		slsi_p2p_deinit(sdev, ndev_vif);
	}
#endif
	if (SLSI_IS_VIF_INDEX_WLAN(ndev_vif)) {
		ndev_vif->vif_type = SLSI_VIFTYPE_UNSPECIFIED;
	}

	work_cancel(SCSC_WORK, &ndev_vif->scan_timeout_work);

	slsi_mbuf_work_deinit(&ndev_vif->rx_data);
	slsi_mbuf_work_deinit(&ndev_vif->rx_mlme);

	for (i = 0; i < SLSI_SCAN_MAX; i++) {
		slsi_mbuf_queue_purge(&ndev_vif->scan[i].scan_results);
	}

	/* Free the netdev */
	slsi_free_netdev(dev);
}

void slsi_netif_remove(struct slsi_dev *sdev, struct netif *dev)
{
	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	slsi_netif_remove_locked(sdev, dev);
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
}

void slsi_netif_remove_all(struct slsi_dev *sdev)
{
	int i;

	SLSI_DBG1(sdev, SLSI_NETDEV, "\n");
	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	for (i = 1; i <= CONFIG_SCSC_WLAN_MAX_INTERFACES; i++)
		if (sdev->netdev[i]) {
			slsi_netif_remove_locked(sdev, sdev->netdev[i]);
		}
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
}

void slsi_netif_deinit(struct slsi_dev *sdev)
{
	SLSI_DBG1(sdev, SLSI_NETDEV, "\n");
	slsi_netif_remove_all(sdev);
}
