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

#include "dev.h"
#include "debug_scsc.h"
#include "mgt.h"
#include "mlme.h"
#include "netif.h"
#include "log_clients.h"
#include <tinyara/wdog.h>
#include "tinyara/wdog.h"
#include "hip4.h"

#ifdef CONFIG_SLSI_WLAN_STATS
static struct data_path_stats *dp_stats_g = NULL;

void slsi_init_dath_path_stats(struct slsi_dev *sdev)
{
	dp_stats_g = &sdev->dp_stats;
	dp_stats_g->magic_number = SLSI_WLAN_STATS_MAGIC_NUM;
	dp_stats_g->sdev = sdev;
}

void slsi_update_queue_stats(struct slsi_dev *sdev)
{
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	int i;

	sdev->dp_stats.ac_q_update_status = SLSI_PEER_INVALID;

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		return;
	}

	ndev_vif = netdev_priv(dev);
	if (!ndev_vif) {
		return;
	}

	peer = ndev_vif->peer_sta_record[0];
	if (!peer) {
		return;
	}

	if (peer->valid == true) {
		sdev->dp_stats.ac_q_update_status = SLSI_PEER_VALID;
		memcpy(sdev->dp_stats.peer_mac, peer->address, ETH_ALEN);
		for (i = 0; i < SLSI_NETIF_Q_PER_PEER; i++) {
			sdev->dp_stats.ac_q_stats_qmod[i] = peer->data_qs.ac_q[i].head.qmod;
			sdev->dp_stats.ac_q_stats_qcod[i] = peer->data_qs.ac_q[i].head.qcod;
		}
	}

	sdev->dp_stats.fw_triggered_panic = firmware_triggered_panic;
}

#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
void update_panic_stats(struct slsi_dev *sdev)
{
	int i;

	sdev->dp_stats.r4_panic_record_offset = __r4_panic_record_offset;
	for (i = 0; i < __r4_panic_record_offset; i++) {
		sdev->dp_stats.r4_panic_record_stats[i].panic_id = __r4_panic_record_stats[i].panic_id;
		sdev->dp_stats.r4_panic_record_stats[i].panic_arg = __r4_panic_record_stats[i].panic_arg;
	}
}
#endif

void *slsi_get_dath_path_stats(void)
{
	hip4_update_queue_stats(dp_stats_g->sdev);
	slsi_update_queue_stats(dp_stats_g->sdev);
	mbulk_pool_stats(dp_stats_g->sdev);
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	update_panic_stats(dp_stats_g->sdev);
#endif
	return (void *)dp_stats_g;
}

void slsi_reset_data_path_stats(void)
{
	memset(dp_stats_g, 0, sizeof(struct data_path_stats));
}
#endif

static int slsi_tx_eapol(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer;
	u8 *eapol;
	u16 msg_type = 0;
	u16 proto = ntohs(eth_hdr(mbuf)->h_proto);
	int ret = 0;

	SLSI_MUTEX_LOCK(ndev_vif->peer_lock);
	peer = slsi_get_peer_from_mac(sdev, dev, eth_hdr(mbuf)->h_dest);
	if (!peer) {
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
		SLSI_WARN(sdev, "no peer record for" SLSI_MAC_FORMAT ", dropping EAP frame\n", SLSI_MAC_STR(eth_hdr(mbuf)->h_dest));
		return ERR_VAL;
	}

	switch (proto) {
	case ETH_P_PAE:
		/**
		 *  Detect if this is an EAPOL key frame. If so detect if it is an EAPOL-Key M4 packet
		 *
		 *  In M4 packet,
		 *   - MIC bit will be set in key info
		 *   - Key type bit will be set in key info ( pairwise =1, Group =0)
		 *   - Key Data Length would be 0
		 */
		eapol = slsi_mbuf_get_data(mbuf) + sizeof(struct ethhdr);
		if (eapol[SLSI_EAPOL_IEEE8021X_TYPE_POS] == SLSI_IEEE8021X_TYPE_EAP_PACKET) {
			msg_type = FAPI_MESSAGETYPE_EAP_MESSAGE;
		} else if (eapol[SLSI_EAPOL_IEEE8021X_TYPE_POS] == SLSI_IEEE8021X_TYPE_EAPOL_KEY) {
			msg_type = FAPI_MESSAGETYPE_EAPOL_KEY_M123;

			if ((eapol[SLSI_EAPOL_TYPE_POS] == SLSI_EAPOL_TYPE_RSN_KEY || eapol[SLSI_EAPOL_TYPE_POS] == SLSI_EAPOL_TYPE_WPA_KEY) && (eapol[SLSI_EAPOL_KEY_INFO_LOWER_BYTE_POS] & SLSI_EAPOL_KEY_INFO_KEY_TYPE_BIT_IN_LOWER_BYTE) && (eapol[SLSI_EAPOL_KEY_INFO_HIGHER_BYTE_POS] & SLSI_EAPOL_KEY_INFO_MIC_BIT_IN_HIGHER_BYTE) && (eapol[SLSI_EAPOL_KEY_DATA_LENGTH_HIGHER_BYTE_POS] == 0) && (eapol[SLSI_EAPOL_KEY_DATA_LENGTH_LOWER_BYTE_POS] == 0)) {
				SLSI_NET_DBG1(dev, SLSI_MLME, "message M4\n");
				msg_type = FAPI_MESSAGETYPE_EAPOL_KEY_M4;
			}
		}
		break;
	case ETH_P_WAI:
		SLSI_NET_DBG1(dev, SLSI_MLME, "WAI protocol frame\n");
		msg_type = FAPI_MESSAGETYPE_WAI_MESSAGE;
		break;
	default:
		SLSI_NET_DBG1(dev, SLSI_MLME, "unsupported protocol\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
		return ERR_VAL;
	}

	/* EAPOL frames are send via the MLME */
	ret = slsi_mlme_send_frame_data(sdev, dev, mbuf, msg_type);

	SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
	return ret;
}

void slsi_alloc_tx_mbuf(struct slsi_dev *sdev)
{
	sdev->tx_mbuf = mbuf_alloc(SLSI_TX_MBUF_SIZE);
	if (!sdev->tx_mbuf) {
		SLSI_ERR(sdev, "Failed to allocate TX mbuf\n");
	}
}

void slsi_free_tx_mbuf(struct slsi_dev *sdev)
{
	if (sdev->tx_mbuf) {
		slsi_kfree_mbuf(sdev->tx_mbuf);
	}

	sdev->tx_mbuf = NULL;
}

#define ETH_P_IP        0x0800	/* Internet Protocol packet     */
#define ETH_P_ARP       0x0806	/* Address Resolution packet    */

/**
 * This function deals with TX of data frames.
 * The data path re-uses the mbuf and so it is not freed here after transmission.
 * The caller of this function needs to free the mbuf.
 *
 * NOTE THAT currently ONLY the following set of err-codes will trigger
 * a REQUEUE and RETRY by upper layers in Kernel NetStack:
 *
 *  -ENOSPC
 */
int slsi_tx_data(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer;
	u16 len = mbuf->data_len;
	int ret = 0;

#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	enum slsi_traffic_q traffic_q;
#endif
	if (slsi_is_test_mode_enabled()) {
		/* This signals is in XML file because parts of the Firmware need the symbols defined by them
		 * but this is actually not handled in wlanlite firmware.
		 */
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_test_mode_enabled);
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MA_UNITDATA.request\n");

		return ERR_VAL;
	}

	SLSI_NET_DBG3(dev, SLSI_TX, "ac_queue:%d\n", mbuf->ac_queue);

	if (!ndev_vif->activated) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_vif_not_active);

		return ERR_CONN;
	}

	if ((ndev_vif->vif_type == FAPI_VIFTYPE_AP) && !ndev_vif->peer_sta_records) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_no_sta_connected);
		SLSI_NET_DBG3(dev, SLSI_TX, "AP/IBSS with no STAs associated, ignore TX frame\n");

		return ERR_CONN;
	}

	SLSI_NET_DBG3(dev, SLSI_TX, "h_proto:%x\n", ntohs(eth_hdr(mbuf)->h_proto));

	/* check if it is an high important frame? At the moment EAPOL, DHCP
	 * and ARP are treated as high important frame and are sent over
	 * MLME for applying special rules in transmission.
	 */
	if (mbuf->ac_queue == SLSI_NETIF_Q_PRIORITY) {
		int proto = ntohs(eth_hdr(mbuf)->h_proto);

		switch (proto) {
		default:
			/* Only EAP packets and IP frames with DHCP are stored in SLSI_NETIF_Q_PRIORITY */
			SLSI_NET_ERR(dev, "Bad h_proto=0x%x in SLSI_NETIF_Q_PRIORITY\n", proto);
			return ERR_VAL;
		case ETH_P_PAE:
		case ETH_P_WAI:
			SLSI_NET_DBG3(dev, SLSI_TX, "transmit EAP packet from SLSI_NETIF_Q_PRIORITY\n");
			ret = slsi_tx_eapol(sdev, dev, mbuf);
			if (ret < 0) {
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_eapol);
			}
			return ret;
		case ETH_P_ARP:
			SLSI_NET_DBG3(dev, SLSI_TX, "transmit ARP frame from SLSI_NETIF_Q_PRIORITY\n");
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_arp_dhcp_mlme);
			ret = slsi_mlme_send_frame_data(sdev, dev, mbuf, FAPI_MESSAGETYPE_ARP);
			if (ret < 0) {
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_arp);
			}
			return ret;
		case ETH_P_IP:
			if (slsi_is_dhcp_packet(slsi_mbuf_get_data(mbuf)) != SLSI_TX_IS_NOT_DHCP) {
				SLSI_NET_DBG2(dev, SLSI_MLME, "transmit DHCP packet from SLSI_NETIF_Q_PRIORITY\n");
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_arp_dhcp_mlme);
				ret = slsi_mlme_send_frame_data(sdev, dev, mbuf, FAPI_MESSAGETYPE_DHCP);
				if (ret < 0) {
					SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_dhcp);
				}
				return ret;
			}
			/* IP frame can have only DHCP packet in SLSI_NETIF_Q_PRIORITY */
			SLSI_NET_ERR(dev, "Bad IP frame in SLSI_NETIF_Q_PRIORITY\n");
			return ERR_VAL;
		}
	}

	if (mbuf_headroom(mbuf) < (fapi_sig_size(ma_unitdata_req) + 160)) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_headroom);
		SLSI_NET_WARN(dev, "Not enough headroom, packet dropped!\n");

		return ERR_VAL;
	}

	/* Align mac_header with mbuf->data */
	if (mbuf_headroom(mbuf) != mbuf->mac_header) {
		mbuf_pull(mbuf, mbuf->mac_header - mbuf_headroom(mbuf));
	}

	len = mbuf->data_len;

	(void)mbuf_push(mbuf, fapi_sig_size(ma_unitdata_req));
	fapi_set_u16(mbuf, id, MA_UNITDATA_REQ);
	fapi_set_u16(mbuf, receiver_pid, 0);
	fapi_set_u16(mbuf, sender_pid, SLSI_TX_PROCESS_ID_MIN);
	fapi_set_u32(mbuf, fw_reference, 0);
	fapi_set_u16(mbuf, u.ma_unitdata_req.vif, ndev_vif->ifnum);
	fapi_set_u16(mbuf, u.ma_unitdata_req.host_tag, slsi_tx_host_tag(sdev));
	fapi_set_u16(mbuf, u.ma_unitdata_req.peer_index, MAP_QS_TO_AID(slsi_netif_get_qs_from_queue(mbuf->ac_queue, slsi_frame_priority_to_ac_queue(mbuf->user_priority))));

	/* by default the priority is set to contention. It is overridden and set appropriate
	 * priority if peer supports QoS. The broadcast/multicast frames are sent in non-QoS except Oxygen.
	 * The broadcast/multicast frames are sent in QoS in case of Oxygen.
	 */
	fapi_set_u16(mbuf, u.ma_unitdata_req.priority, FAPI_PRIORITY_CONTENTION);
	fapi_set_u16(mbuf, u.ma_unitdata_req.data_unit_descriptor, FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME);

	mbuf->fapi.sig_length = fapi_sig_size(ma_unitdata_req);
	mbuf->fapi.data_length = mbuf->data_len;

	/* colour is defined as: */
	/* u16 register bits:
	 * 0      - do not use
	 * [2:1]  - vif
	 * [7:3]  - peer_index
	 * [10:8] - ac queue
	 */
	mbuf->colour = (slsi_frame_priority_to_ac_queue(mbuf->user_priority) << 8) | (fapi_get_u16(mbuf, u.ma_unitdata_req.peer_index) << 3) | ndev_vif->ifnum << 1;

	/* ACCESS POINT MODE */
	if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
		struct ethhdr *ehdr = eth_hdr(mbuf);

		if (is_multicast_ether_addr(ehdr->h_dest)) {
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
			ret = scsc_wifi_fcq_transmit_data(dev, &ndev_vif->ap.group_data_qs, slsi_frame_priority_to_ac_queue(mbuf->user_priority), true);
			if (ret < 0) {
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_flow_control_mcast);
				SLSI_NET_DBG3(dev, SLSI_TX, "no fcq for groupcast, dropping TX frame\n");

				return ret;
			}
#endif
			ret = scsc_wifi_transmit_frame(&sdev->hip4_inst, false, mbuf);
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
			if (ret != ERR_OK) {
				/* scsc_wifi_transmit_frame failed, decrement BoT counters */
				scsc_wifi_fcq_receive_data(dev, &ndev_vif->ap.group_data_qs, slsi_frame_priority_to_ac_queue(mbuf->user_priority));
			}
#endif
			return ret;
		}
	}

	SLSI_MUTEX_LOCK(ndev_vif->peer_lock);
	peer = slsi_get_peer_from_mac(sdev, dev, eth_hdr(mbuf)->h_dest);
	if (!peer) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_no_peer_record);
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);

		return ERR_CONN;
	}

	/**
	 * mbuf->user_priority will contain the priority obtained from the IP Diff/Serv field.
	 * The mbuf->user_priority field is defined in terms of the FAPI_PRIORITY_* defintions.
	 * For QoS enabled associations, this is the tid and is the value required in
	 * the ma_unitdata_req.priority field. For non-QoS assocations, the ma_unitdata_req.
	 * priority field requires FAPI_PRIORITY_CONTENTION.
	 */
	if (peer->qos_enabled) {
		fapi_set_u16(mbuf, u.ma_unitdata_req.priority, mbuf->user_priority);
	}
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	traffic_q = slsi_frame_priority_to_ac_queue(mbuf->user_priority);
	if (scsc_wifi_fcq_transmit_data(dev, &peer->data_qs, traffic_q, false) < 0) {
#ifdef CONFIG_SLSI_WLAN_STATS
		switch (traffic_q) {
		case SLSI_TRAFFIC_Q_BE:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_flow_control_be);
			break;
		case SLSI_TRAFFIC_Q_BK:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_flow_control_bk);
			break;
		case SLSI_TRAFFIC_Q_VI:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_flow_control_vi);
			break;
		case SLSI_TRAFFIC_Q_VO:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_flow_control_vo);
			break;
		}
#endif
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
		return ERR_MEM;
	}
#endif

	ret = scsc_wifi_transmit_frame(&sdev->hip4_inst, false, mbuf);
	if (ret != 0) {
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
		/* scsc_wifi_transmit_frame failed, decrement BoT counters */
		scsc_wifi_fcq_receive_data(dev, &peer->data_qs, traffic_q);
#endif
	}

	SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);

	return ret;
}

int slsi_tx_data_lower(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	u16 vif;
	u8 *dest;
	int ret;

	vif = fapi_get_vif(mbuf);

	switch (fapi_get_u16(mbuf, u.ma_unitdata_req.data_unit_descriptor)) {
	case FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME:
		if (ntohs(eth_hdr(mbuf)->h_proto) == ETH_P_PAE || ntohs(eth_hdr(mbuf)->h_proto) == ETH_P_WAI) {
			return slsi_tx_control(sdev, NULL, mbuf, true);
		}
		dest = eth_hdr(mbuf)->h_dest;
		break;

	case FAPI_DATAUNITDESCRIPTOR_AMSDU:
		/* The AMSDU frame type is an AMSDU payload ready to be prepended by
		 * an 802.11 frame header by the firmware. The AMSDU subframe header
		 * is identical to an Ethernet header in terms of addressing, so it
		 * is safe to access the destination address through the ethernet
		 * structure.
		 */
		dest = eth_hdr(mbuf)->h_dest;
		break;
	case FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME:
		dest = slsi_80211_get_DA((struct slsi_80211_hdr *)fapi_get_data(mbuf));
		break;
	default:
		SLSI_ERR(sdev, "data_unit_descriptor incorrectly set (0x%02x), dropping TX frame\n", fapi_get_u16(mbuf, u.ma_unitdata_req.data_unit_descriptor));
		return -EINVAL;
	}

	dev = slsi_get_netdev(sdev, vif);
	if (!dev) {
		SLSI_ERR(sdev, "netdev(%d) No longer exists\n", vif);
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	if (is_multicast_ether_addr(dest) && (ndev_vif->vif_type == FAPI_VIFTYPE_AP)) {
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
		if (scsc_wifi_fcq_transmit_data(dev, &ndev_vif->ap.group_data_qs, slsi_frame_priority_to_ac_queue(mbuf->user_priority), true) < 0) {
			SLSI_NET_DBG3(dev, SLSI_TX, "no fcq for groupcast, dropping TX frame\n");
			return ERR_MEM;
		}
#endif
		ret = scsc_wifi_transmit_frame(&sdev->hip4_inst, false, mbuf);
		if (ret == 0) {
			return ret;
		}
		/**
		 * This should be NEVER RETRIED/REQUEUED and its' handled
		 * by the caller in UDI cdev_write
		 */
		if (ret == -ENOSPC) {
			SLSI_NET_DBG1(dev, SLSI_TX, "TX_LOWER...Queue Full... BUT Dropping packet\n");
		} else {
			SLSI_NET_DBG1(dev, SLSI_TX, "TX_LOWER...Generic Error...Dropping packet\n");
		}

		/* scsc_wifi_transmit_frame failed, decrement BoT counters */
		scsc_wifi_fcq_receive_data(dev, &ndev_vif->ap.group_data_qs, slsi_frame_priority_to_ac_queue(mbuf->user_priority));
		return ret;
	}

	SLSI_MUTEX_LOCK(ndev_vif->peer_lock);
	peer = slsi_get_peer_from_mac(sdev, dev, dest);
	if (!peer) {
		SLSI_ERR(sdev, "no peer record for %02x:%02x:%02x:%02x:%02x:%02x, dropping TX frame\n", dest[0], dest[1], dest[2], dest[3], dest[4], dest[5]);
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
		return -EINVAL;
	}

	if (fapi_get_u16(mbuf, u.ma_unitdata_req.priority) == FAPI_PRIORITY_CONTENTION) {
		mbuf->user_priority = FAPI_PRIORITY_QOS_UP0;
	} else {
		mbuf->user_priority = fapi_get_u16(mbuf, u.ma_unitdata_req.priority);
	}

	ret = scsc_wifi_transmit_frame(&sdev->hip4_inst, false, mbuf);
	if (ret < 0) {
		if (ret == -ENOSPC) {
			SLSI_NET_DBG1(dev, SLSI_TX, "TX_LOWER...Queue Full...BUT Dropping packet\n");
		} else {
			SLSI_NET_DBG1(dev, SLSI_TX, "TX_LOWER...Generic Error...Dropping packet\n");
		}
		SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
		return ret;
	}

	SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
	return 0;
}

/**
 * NOTE:
 * 1. dev can be NULL
 * 2. On error the MBUF is NOT freed, NOR retried (ENOSPC dropped).
 * Callers should take care to free the MBUF eventually.
 */
int slsi_tx_control(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, bool free_buf)
{
	int res = 0;
	struct fapi_signal_header *hdr;

	if (WARN_ON(!mbuf)) {
		res = ERR_VAL;
		goto exit;
	}

	/**
	 * Sanity check of the mbuf - if it's not an MLME, MA, debug or test
	 * signal it will be discarded.
	 * Skip if test mode (wlanlite) is enabled.
	 */
	if (!slsi_is_test_mode_enabled())
		if (!fapi_is_mlme(mbuf) && !fapi_is_ma(mbuf) && !fapi_is_debug(mbuf) && !fapi_is_test(mbuf)) {
			SLSI_NET_WARN(dev, "Discarding mbuf because it has type: 0x%04X\n", fapi_get_sigid(mbuf));
			res = ERR_VAL;
			goto exit;
		}

	mbuf->fapi.sig_length = fapi_get_expected_size(mbuf);
	mbuf->fapi.data_length = mbuf->data_len;

	/* F/w will panic if fw_reference is not zero. */
	hdr = (struct fapi_signal_header *)slsi_mbuf_get_data(mbuf);
	hdr->fw_reference = 0;

	res = scsc_wifi_transmit_frame(&sdev->hip4_inst, true, mbuf);
	if (res == 0) {
		/* Free the mbuf after successful transmit if asked for.
		 * All the control path mbufs are normally freed here except DHCP/ARP called from data path. */
		if (free_buf) {
			slsi_kfree_mbuf(mbuf);
		}
	}
exit:
	return res;
}
