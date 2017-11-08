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
#include "debug_scsc.h"
#include "dev.h"
#include "mgt.h"
#include "mlme.h"
#include "ba.h"
#include "mib.h"
#include "wlan_80211_utils.h"
#include "wpa_driver_copy.h"
#include "t20_ops.h"

extern void l2_packet_receive(void *ctx, const u8 *src_addr, u8 *ptr, u16 len);

void slsi_rx_scan_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	u16 scan_id = fapi_get_u16(mbuf, u.mlme_scan_ind.scan_id);
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_NET_DBG3(dev, SLSI_MLME, "mlme_scan_ind(interface:%d, scan_id:%d (%#x)) bssid: " SLSI_MAC_FORMAT "\n", ndev_vif->ifnum, scan_id, scan_id, SLSI_MAC_STR(fapi_get_mgmt(mbuf)->bssid));

	scan_id = (scan_id & 0xFF);

	if (WARN_ON(scan_id >= SLSI_SCAN_MAX)) {
		slsi_kfree_mbuf(mbuf);
		return;
	}

	if ((fapi_get_vif(mbuf) != 0) && (fapi_get_u16(mbuf, u.mlme_scan_ind.scan_id) == SLSI_SCAN_HW_ID)) {
		SLSI_NET_DBG3(dev, SLSI_MLME, "Connect scan indication received, bssid: " SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(fapi_get_mgmt(mbuf)->bssid));
		slsi_kfree_mbuf(mbuf);
		return;
	}

	/* NOTE: No locking needed here. */
	slsi_mbuf_queue_tail(&ndev_vif->scan[scan_id].scan_results, mbuf);
}

void slsi_rx_scan_done_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	u16 scan_id = fapi_get_u16(mbuf, u.mlme_scan_done_ind.scan_id);
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_NET_DBG3(dev, SLSI_MLME, "slsi_rx_scan_done_ind\n");

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

	scan_id = (scan_id & 0xFF);

	if ((scan_id == SLSI_SCAN_HW_ID) && ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req) {
		work_cancel(SCSC_WORK, &ndev_vif->scan_timeout_work);
	}

	slsi_scan_complete(ndev_vif);

	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

int slsi_find_scan_freq(struct slsi_80211_mgmt *mgmt, size_t mgmt_len, u16 freq)
{
	int ielen = mgmt_len - (mgmt->u.beacon.variable - (u8 *)mgmt);
	const u8 *scan_ds = slsi_80211_find_ie(WLAN_EID_DS_PARAMS, mgmt->u.beacon.variable, ielen);
	const u8 *scan_ht = slsi_80211_find_ie(WLAN_EID_HT_OPERATION, mgmt->u.beacon.variable, ielen);
	u8 chan = 0;

	/* Use the DS or HT channel where possible as the Offchannel results mean the RX freq is not reliable */
	if (scan_ds) {
		chan = scan_ds[2];
	} else if (scan_ht) {
		chan = scan_ht[2];
	}

	if (chan) {
		enum slsi_80211_band band = SLSI_80211_BAND_2GHZ;

		if (chan > 14) {
			band = SLSI_80211_BAND_5GHZ;
		}
		freq = (u16)slsi_80211_channel_to_frequency(chan, band);
	}
	if (!freq) {
		return 0;
	}

	return freq;
}

void __slsi_rx_blockack_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer;

	SLSI_NET_DBG1(dev, SLSI_MLME, "ma_blockack_ind(vif:%d, peer_qsta_address:" SLSI_MAC_FORMAT ", parameter_set:%d, sequence_number:%d, reason_code:%d, direction:%d)\n", fapi_get_vif(mbuf), SLSI_MAC_STR(fapi_get_buff(mbuf, u.ma_blockack_ind.peer_qsta_address)), fapi_get_u16(mbuf, u.ma_blockack_ind.blockack_parameter_set), fapi_get_u16(mbuf, u.ma_blockack_ind.sequence_number), fapi_get_u16(mbuf, u.ma_blockack_ind.reason_code), fapi_get_u16(mbuf, u.ma_blockack_ind.direction));

	peer = slsi_get_peer_from_mac(sdev, dev, fapi_get_buff(mbuf, u.ma_blockack_ind.peer_qsta_address));
	WARN_ON(!peer);

	if (peer) {
		/* Buffering of frames before the mlme_connected_ind */
		if ((ndev_vif->vif_type == FAPI_VIFTYPE_AP) && (peer->connected_state == SLSI_STA_CONN_STATE_CONNECTING)) {
			SLSI_DBG3(sdev, SLSI_MLME, "Buffering MA-BlockAck.Indication\n");
			slsi_mbuf_queue_tail(&peer->buffered_frames, mbuf);
			return;
		}
		slsi_handle_blockack(dev, peer, fapi_get_vif(mbuf), fapi_get_buff(mbuf, u.ma_blockack_ind.peer_qsta_address), fapi_get_u16(mbuf, u.ma_blockack_ind.blockack_parameter_set), fapi_get_u16(mbuf, u.ma_blockack_ind.sequence_number), fapi_get_u16(mbuf, u.ma_blockack_ind.reason_code), fapi_get_u16(mbuf, u.ma_blockack_ind.direction)
							);
	}

	slsi_kfree_mbuf(mbuf);
}

void slsi_rx_blockack_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	__slsi_rx_blockack_ind(sdev, dev, mbuf);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
}

static bool get_wmm_ie_from_resp_ie(struct slsi_dev *sdev, struct netif *dev, u8 *resp_ie, size_t resp_ie_len, const u8 **wmm_elem, size_t *wmm_elem_len)
{
	struct slsi_80211_vendor_ie *ie;

	SLSI_UNUSED_PARAMETER(sdev);

	if (!resp_ie) {
		SLSI_NET_ERR(dev, "Received invalid pointer to the ie's of the association response\n");
		return false;
	}
	/* parse response ie elements and return the wmm ie */
	*wmm_elem = slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WMM, resp_ie, resp_ie_len);

	if (!(*wmm_elem)) {
		SLSI_NET_DBG2(dev, SLSI_MLME, "No WMM IE\n");
		return false;
	}
	ie = (struct slsi_80211_vendor_ie *)*wmm_elem;
	*wmm_elem_len = ie->len + 2;

	SLSI_NET_DBG3(dev, SLSI_MLME, "WMM IE received and parsed successfully\n");

	return true;
}

static bool sta_wmm_update_acm(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer, u8 *assoc_rsp_ie, size_t assoc_rsp_ie_len)
{
	size_t left;
	const u8 *pos;
	const u8 *wmm_elem = NULL;
	size_t wmm_elem_len = 0;

	if (!get_wmm_ie_from_resp_ie(sdev, dev, assoc_rsp_ie, assoc_rsp_ie_len, &wmm_elem, &wmm_elem_len)) {
		SLSI_NET_DBG1(dev, SLSI_MLME, "No WMM IE received\n");
		return false;
	}

	if (wmm_elem_len < 10 || wmm_elem[7] /* version */ != 1) {
		SLSI_NET_WARN(dev, "Invalid WMM IE: wmm_elem_len=%lu, wmm_elem[7]=%d\n", (unsigned long int)wmm_elem_len, (int)wmm_elem[7]);
		return false;
	}

	pos = wmm_elem + 10;
	left = wmm_elem_len - 10;

	for (; left >= 4; left -= 4, pos += 4) {
		int aci = (pos[0] >> 5) & 0x03;
		int acm = (pos[0] >> 4) & 0x01;

		switch (aci) {
		case 1:				/* AC_BK */
			if (acm) {
				peer->wmm_acm |= BIT(1) | BIT(2);    /* BK/- */
			}
			break;
		case 2:				/* AC_VI */
			if (acm) {
				peer->wmm_acm |= BIT(4) | BIT(5);    /* CL/VI */
			}
			break;
		case 3:				/* AC_VO */
			if (acm) {
				peer->wmm_acm |= BIT(6) | BIT(7);    /* VO/NC */
			}
			break;
		case 0:				/* AC_BE */
		default:
			if (acm) {
				peer->wmm_acm |= BIT(0) | BIT(3);    /* BE/EE */
			}
			break;
		}
	}

	SLSI_NET_DBG3(dev, SLSI_MLME, "ACM bitfield has been updated successfully\n");
	return true;
}

/* Retrieve any buffered frame before connected_ind and pass them up. */
void slsi_rx_buffered_frames(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *buff_frame = NULL;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));
	if (WARN(!peer, "Peer is NULL")) {
		return;
	}
	if (peer->connected_state == SLSI_STA_CONN_STATE_CONNECTING) {
		SLSI_NET_WARN(dev, "Wrong state");
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "Processing buffered RX frames received before mlme_connected_ind for (vif:%d, aid:%d)\n", ndev_vif->ifnum, peer->aid);
	buff_frame = slsi_mbuf_dequeue(&peer->buffered_frames);
	while (buff_frame) {
		switch (fapi_get_sigid(buff_frame)) {
		case MA_UNITDATA_IND:
			SLSI_NET_DBG2(dev, SLSI_MLME, "Transferring buffered MA_UNITDATA_IND frame");
			(void)slsi_rx_data(sdev, dev, buff_frame, false);
			break;
		case MA_BLOCKACK_IND:
			SLSI_NET_DBG2(dev, SLSI_MLME, "Transferring buffered MA_BLOCKACK_IND frame");
			__slsi_rx_blockack_ind(sdev, dev, buff_frame);
			break;
		default:
			SLSI_NET_WARN(dev, "Unexpected Data: 0x%.4x\n", fapi_get_sigid(buff_frame));
			slsi_kfree_mbuf(buff_frame);
			break;
		}
		buff_frame = slsi_mbuf_dequeue(&peer->buffered_frames);
		if (buff_frame == NULL) {
			return;
		}
	}
}

void slsi_rx_connected_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer = NULL;
	u16 aid = fapi_get_u16(mbuf, u.mlme_connected_ind.peer_index);

	/* For AP mode, peer_index value is equivalent to aid(association_index) value */

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_connected_ind(vif:%d, peer_index:%d)\n", fapi_get_vif(mbuf), aid);

	if (!ndev_vif->activated) {
		SLSI_NET_DBG1(dev, SLSI_MLME, "VIF not activated\n");
		goto exit_with_lock;
	}

	switch (ndev_vif->vif_type) {
	case FAPI_VIFTYPE_AP: {
		if (aid < SLSI_PEER_INDEX_MIN || aid > SLSI_PEER_INDEX_MAX) {
			SLSI_NET_ERR(dev, "Received incorrect peer_index: %d\n", aid);
			goto exit_with_lock;
		}

		peer = slsi_get_peer_from_qs(sdev, dev, aid - 1);
		if (!peer) {
			SLSI_NET_ERR(dev, "Peer (aid:%d) Not Found - Disconnect peer\n", aid);
			goto exit_with_lock;
		}
		slsi_connect_indication(ndev_vif, slsi_mbuf_get_data(peer->assoc_ie), peer->assoc_ie->data_len, NULL, 0, 0, ndev_vif->vif_type, peer->address);

		if (ndev_vif->ap.privacy) {
			peer->connected_state = SLSI_STA_CONN_STATE_DOING_KEY_CONFIG;

			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DOING_KEY_CONFIG);
		} else {
			peer->connected_state = SLSI_STA_CONN_STATE_CONNECTED;
			slsi_mlme_connected_resp(sdev, dev, aid);

			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_CONNECTED);
		}
		slsi_rx_buffered_frames(sdev, dev, peer);
		break;
	}
	default:
		SLSI_NET_WARN(dev, "mlme_connected_ind(vif:%d, unexpected vif type:%d)\n", fapi_get_vif(mbuf), ndev_vif->vif_type);
		break;
	}
exit_with_lock:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

void slsi_rx_connect_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	enum slsi_status_code status = WLAN_STATUS_SUCCESS;
	struct slsi_peer *peer = NULL;

	u8 *assoc_ie = NULL;
	int assoc_ie_len = 0;
	u8 *assoc_rsp_ie = NULL;
	int assoc_rsp_ie_len = 0;
	const u8 *ssid_ie;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_connect_ind(vif:%d, result:%d)\n", fapi_get_vif(mbuf), fapi_get_u16(mbuf, u.mlme_connect_ind.result_code));

	if (!ndev_vif->activated) {
		SLSI_NET_WARN(dev, "VIF not activated\n");
		goto exit_with_lock;
	}

	if (ndev_vif->vif_type != FAPI_VIFTYPE_STATION) {
		SLSI_NET_WARN(dev, "Not a Station VIF\n");
		goto exit_with_lock;
	}

	if (ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTING) {
		SLSI_NET_WARN(dev, "VIF not connecting\n");
		goto exit_with_lock;
	}

	if (fapi_get_u16(mbuf, u.mlme_connect_ind.result_code) != FAPI_RESULTCODE_SUCCESS) {
		status = WLAN_STATUS_UNSPECIFIED_FAILURE;
	} else {
		peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
		if (peer) {
			if (peer->assoc_ie) {
				assoc_ie = slsi_mbuf_get_data(peer->assoc_ie);
				assoc_ie_len = peer->assoc_ie->data_len;
			}

			slsi_peer_update_assoc_rsp(sdev, dev, peer, mbuf);
			/* mbuf is consumed by slsi_peer_update_assoc_rsp. So do not access this anymore. */
			mbuf = NULL;

			if (peer->assoc_resp_ie) {
				assoc_rsp_ie = slsi_mbuf_get_data(peer->assoc_resp_ie);
				assoc_rsp_ie_len = peer->assoc_resp_ie->data_len;
				ssid_ie = slsi_80211_find_ie(WLAN_EID_SSID, assoc_ie, assoc_ie_len);
				if (ssid_ie != NULL) {
					peer->ssid_len = ssid_ie[1];
					memcpy(peer->ssid, ssid_ie + 2, peer->ssid_len);
				}
			}

			/* this is the right place to initialize the bitmasks for
			 * acm bit and tspec establishment
			 */
			peer->wmm_acm = 0;
			peer->tspec_established = 0;
			peer->uapsd = 0;

			/* update the acm bitmask according to the acm bit values that
			 * are included in wmm ie element of association response
			 */
			if (!sta_wmm_update_acm(sdev, dev, peer, assoc_rsp_ie, assoc_rsp_ie_len)) {
				SLSI_NET_DBG1(dev, SLSI_MLME, "Fail to update WMM ACM\n");
			}

			WARN_ON(assoc_ie_len && !assoc_ie);
			WARN_ON(assoc_rsp_ie_len && !assoc_rsp_ie);
		} else {
			SLSI_NET_ERR(dev, "Peer not found\n");
			status = WLAN_STATUS_UNSPECIFIED_FAILURE;
		}
	}

	if (status == WLAN_STATUS_SUCCESS) {
		ndev_vif->sta.vif_status = SLSI_VIF_STATUS_CONNECTED;

		slsi_connect_indication(ndev_vif, assoc_ie, assoc_ie_len, assoc_rsp_ie, assoc_rsp_ie_len, status, ndev_vif->vif_type, NULL);

		/* For Open & WEP AP,set the power mode, send connect response and install the packet filters .
		 * For secured AP, all this would be done after handshake
		 */
		if ((peer->capabilities & WLAN_CAPABILITY_PRIVACY) && (slsi_80211_find_ie(WLAN_EID_RSN, assoc_ie, assoc_ie_len) || slsi_80211_find_ie(WLAN_EID_WAPI, assoc_ie, assoc_ie_len) || slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPA, assoc_ie, assoc_ie_len))) {
			/*secured AP */

			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DOING_KEY_CONFIG);

			ndev_vif->sta.resp_id = MLME_CONNECT_RES;
		} else {
			/*Open/WEP AP */
			slsi_mlme_connect_resp(sdev, dev);
			//slsi_set_packet_filters(sdev, dev);
#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
			slsi_mlme_powermgt(sdev, dev, FAPI_POWERMANAGEMENTMODE_POWER_SAVE);
#endif
			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_CONNECTED);
		}

#ifdef CONFIG_SCSC_ENABLE_P2P
		/* For P2PCLI, set the Connection Timeout (beacon miss) mib to 10 seconds */
		if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_CLIENT) {
			if (slsi_set_uint_mib(sdev, dev, SLSI_PSID_UNIFI_MLME_CONNECTION_TIMEOUT, SLSI_P2PGC_CONN_TIMEOUT_MSEC) != 0) {
				SLSI_NET_ERR(dev, "P2PGC Connection Timeout MIB set failed");
				goto exit_with_vif;
			}
			SLSI_P2P_STATE_CHANGE(sdev, P2P_GROUP_FORMED_CLI);
		}
#endif
		goto exit_with_lock;
	}
#ifdef CONFIG_SCSC_ENABLE_P2P
exit_with_vif:
	slsi_mlme_del_vif(sdev, dev);
	slsi_vif_deactivated(sdev, dev);
#endif

exit_with_lock:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

void slsi_rx_disconnect_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_disconnect_ind(vif:%d, tx_status:%d, MAC:" SLSI_MAC_FORMAT ")\n", fapi_get_vif(mbuf), fapi_get_u16(mbuf, u.mlme_disconnect_ind.transmission_status), SLSI_MAC_STR(fapi_get_buff(mbuf, u.mlme_disconnect_ind.peer_sta_address)));

	slsi_handle_disconnect(sdev, dev, fapi_get_buff(mbuf, u.mlme_disconnect_ind.peer_sta_address), 0);

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

void slsi_rx_disconnected_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "mlme_disconnected_ind(vif:%d, reason:%d, MAC:" SLSI_MAC_FORMAT ")\n", fapi_get_vif(mbuf), fapi_get_u16(mbuf, u.mlme_disconnected_ind.reason_code), SLSI_MAC_STR(fapi_get_buff(mbuf, u.mlme_disconnected_ind.peer_sta_address)));

	if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
		struct slsi_peer *peer = NULL;

		peer = slsi_get_peer_from_mac(sdev, dev, fapi_get_buff(mbuf, u.mlme_disconnected_ind.peer_sta_address));
		if (!peer) {
			SLSI_NET_WARN(dev, "PEER NOT FOUND");
		} else {
			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DISCONNECTED);
		}
	}
	slsi_handle_disconnect(sdev, dev, fapi_get_buff(mbuf, u.mlme_disconnected_ind.peer_sta_address), fapi_get_u16(mbuf, u.mlme_disconnected_ind.reason_code));

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

void slsi_rx_procedure_started_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer = NULL;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_MLME, "(vif:%d, type:%d, peer_index:%d)\n", fapi_get_vif(mbuf), fapi_get_u16(mbuf, u.mlme_procedure_started_ind.procedure_type), fapi_get_u16(mbuf, u.mlme_procedure_started_ind.peer_index));

	if (!ndev_vif->activated) {
		SLSI_NET_DBG1(dev, SLSI_MLME, "VIF not activated\n");
		goto exit_with_lock;
	}

	switch (fapi_get_u16(mbuf, u.mlme_procedure_started_ind.procedure_type)) {
	case FAPI_PROCEDURETYPE_CONNECTION_STARTED:
		switch (ndev_vif->vif_type) {
		case FAPI_VIFTYPE_AP: {
			u16 peer_index = fapi_get_u16(mbuf, u.mlme_procedure_started_ind.peer_index);

			SLSI_NET_DBG1(dev, SLSI_MLME, "STA MAC: " SLSI_MAC_FORMAT "\n", SLSI_MAC_STR((fapi_get_mgmt(mbuf))->sa));

			/* Check for MAX client */
			if ((ndev_vif->peer_sta_records + 1) > SLSI_AP_PEER_CONNECTIONS_MAX) {
				SLSI_NET_ERR(dev, "MAX Station limit reached. Ignore ind for peer_index:%d\n", peer_index);
				goto exit_with_lock;
			}

			if (peer_index < SLSI_PEER_INDEX_MIN || peer_index > SLSI_PEER_INDEX_MAX) {
				SLSI_NET_ERR(dev, "Received incorrect peer_index: %d\n", peer_index);
				goto exit_with_lock;
			}

			peer = slsi_peer_add(sdev, dev, (fapi_get_mgmt(mbuf))->sa, peer_index);
			if (!peer) {
				SLSI_NET_ERR(dev, "Peer NOT Created\n");
				goto exit_with_lock;
			}
			slsi_peer_update_assoc_req(sdev, dev, peer, mbuf);
			/* mbuf is consumed by slsi_peer_update_assoc_req. So do not access this anymore. */
			mbuf = NULL;
			peer->connected_state = SLSI_STA_CONN_STATE_CONNECTING;

			break;
		}
		case FAPI_VIFTYPE_STATION: {
			peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
			if (WARN_ON(!peer)) {
				SLSI_NET_ERR(dev, "Peer NOT FOUND\n");
				goto exit_with_lock;
			}
			slsi_peer_update_assoc_req(sdev, dev, peer, mbuf);
			/* mbuf is consumed by slsi_peer_update_assoc_rsp. So do not access this anymore. */
			mbuf = NULL;
			break;
		}
		default:
			SLSI_NET_ERR(dev, "Incorrect vif type for proceduretype_connection_started\n");
			break;
		}
		break;
#ifdef CONFIG_SCSC_ENABLE_P2P
	case FAPI_PROCEDURETYPE_DEVICE_DISCOVERED:
		/* Expected only in P2P Device and P2P GO role */
		if (WARN_ON(!SLSI_IS_VIF_INDEX_P2P(ndev_vif) && (ndev_vif->iftype != SLSI_80211_IFTYPE_P2P_GO))) {
			break;
		}

		/* Send probe request to supplicant only if in listening state. Issues were seen earlier if
		 * Probe request was sent to supplicant while waiting for GO Neg Req from peer.
		 * Send Probe request to supplicant if received in GO mode
		 */
		if ((sdev->p2p_state == P2P_LISTENING) || (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO)) {
			slsi_rx_p2p_device_discovered_ind(sdev, dev, mbuf);
		}
		break;

#endif
	default:
		SLSI_NET_DBG1(dev, SLSI_MLME, "Unknown Procedure: %d\n", fapi_get_u16(mbuf, u.mlme_procedure_started_ind.procedure_type));
		break;
	}

exit_with_lock:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

#ifdef CONFIG_SCSC_ENABLE_P2P
/* Handle Procedure Started (Type = Device Discovered) indication for P2P */
static void slsi_rx_p2p_device_discovered_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int mgmt_len;

	SLSI_UNUSED_PARAMETER(sdev);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "Freq = %d\n", ndev_vif->chan->center_freq);

	/* Only Probe Request is expected as of now */
	mgmt_len = fapi_get_mgmtlen(mbuf);
	if (mgmt_len) {
		struct slsi_80211_mgmt *mgmt = fapi_get_mgmt(mbuf);

		if (slsi_80211_is_mgmt(mgmt->frame_control)) {
			if (slsi_80211_is_probe_req(mgmt->frame_control)) {
				SLSI_NET_DBG3(dev, SLSI_T20_80211, "Received Probe Request\n");
				cfg80211_rx_mgmt(&ndev_vif->wdev, ndev_vif->chan->center_freq, 0, (const u8 *)mgmt, mgmt_len);
			} else {
				SLSI_NET_ERR(dev, "Ignore Indication - Not Probe Request frame\n");
			}
		} else {
			SLSI_NET_ERR(dev, "Ignore Indication - Not Management frame\n");
		}
	}
}
#endif

void slsi_rx_frame_transmission_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u16 host_tag = fapi_get_u16(mbuf, u.mlme_frame_transmission_ind.host_tag);
	u16 tx_status = fapi_get_u16(mbuf, u.mlme_frame_transmission_ind.transmission_status);

//  bool              ack = true;

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_frame_transmission_ind(vif:%d, host_tag:%d, transmission_status:%d)\n", fapi_get_vif(mbuf), host_tag, tx_status);

#ifdef CONFIG_SCSC_ENABLE_P2P
	if (ndev_vif->mgmt_tx_data.host_tag == host_tag) {
		//  struct netdev_vif *ndev_vif_to_cfg = ndev_vif;
		/* If frame tx failed allow del_vif work to take care of vif deletion.
		 * This work would be queued as part of frame_tx with the wait duration
		 */

		if (tx_status != FAPI_TRANSMISSIONSTATUS_SUCCESSFUL) {
			ack = false;
			if (sdev->p2p_group_exp_frame != SLSI_P2P_PA_INVALID) {
				slsi_clear_offchannel_data(sdev, (ndev_vif->ifnum != SLSI_NET_INDEX_P2PX) ? true : false);
			} else if (ndev_vif->mgmt_tx_data.exp_frame != SLSI_P2P_PA_INVALID) {
				(void)slsi_mlme_reset_dwell_time(sdev, dev);
			}
			ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
		}

		/* Change state if frame tx was in Listen as peer response is not expected */
		if (SLSI_IS_VIF_INDEX_P2P(ndev_vif) && (ndev_vif->mgmt_tx_data.exp_frame == SLSI_P2P_PA_INVALID)) {
			//if (delayed_work_pending(&ndev_vif->unsync.roc_expiry_work))
			SLSI_P2P_STATE_CHANGE(sdev, P2P_LISTENING);
			else {
				SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_VIF_ACTIVE);
			}
		} else if (SLSI_IS_VIF_INDEX_P2P_GROUP(ndev_vif)) {
			const struct slsi_80211_mgmt *mgmt = (const struct ieee80211_mgmt *)ndev_vif->mgmt_tx_data.buf;

			/* If frame transmission was initiated on P2P device vif by supplicant, then use the net_dev of that vif (i.e. p2p0) */
			if ((mgmt) && (memcmp(mgmt->sa, dev->dev_addr, ETH_ALEN) != 0)) {
				struct netif *ndev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2P);

				SLSI_NET_DBG2(dev, SLSI_MLME, "Frame Tx was requested with device address - Change ndev_vif for tx_status\n");

				ndev_vif_to_cfg = netdev_priv(ndev);
				if (!ndev_vif_to_cfg) {
					SLSI_NET_ERR(dev, "Getting P2P Index netdev failed\n");
					ndev_vif_to_cfg = ndev_vif;
				}
			}
		}
		/*
					cfg80211_mgmt_tx_status(&ndev_vif_to_cfg->wdev, ndev_vif->mgmt_tx_data.cookie, ndev_vif->mgmt_tx_data.buf, ndev_vif->mgmt_tx_data.buf_len, ack);
		 */
		(void)slsi_set_mgmt_tx_data(ndev_vif, 0, 0, NULL, 0);
	}
#endif

	if (tx_status == FAPI_TRANSMISSIONSTATUS_SUCCESSFUL) {
		if ((ndev_vif->vif_type == FAPI_VIFTYPE_STATION) && (ndev_vif->sta.m4_host_tag == host_tag)) {
			struct slsi_peer *peer = NULL;

			peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
			if (peer == NULL) {
				/* Unlikely but if it reaches here no point in sending connect_resp */
				SLSI_NET_ERR(dev, "Peer not found\n");
				goto exit;
			}

			switch (ndev_vif->sta.resp_id) {
			case MLME_CONNECT_RES:
				slsi_mlme_connect_resp(sdev, dev);
				//slsi_set_packet_filters(sdev, dev);
#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
				slsi_mlme_powermgt(sdev, dev, FAPI_POWERMANAGEMENTMODE_POWER_SAVE);
#endif
				/* Open port after sending connect_resp to have some mbulk buffers available.
				 * Resources are limited and there could be scarcity if DHCP/ARP are pumped in quick succession. */
				slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_CONNECTED);

				break;
			default:
				break;
			}
			ndev_vif->sta.m4_host_tag = 0;
			ndev_vif->sta.resp_id = 0;
		}
	} else {
		if ((tx_status == FAPI_TRANSMISSIONSTATUS_RETRY_LIMIT) && (ndev_vif->iftype == SLSI_80211_IFTYPE_STATION) && (ndev_vif->sta.eap_hosttag == host_tag)) {
			SLSI_NET_WARN(dev, "Disconnect as EAP frame transmission failed\n");

			slsi_mlme_disconnect(sdev, dev, ndev_vif->sta.mac_addr, FAPI_REASONCODE_UNSPECIFIED_REASON, false);
		}
	}
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

void slsi_rx_received_frame_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u16 data_unit_descriptor = fapi_get_u16(mbuf, u.mlme_received_frame_ind.data_unit_descriptor);

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_received_frame_ind(vif:%d, data descriptor:%d, freq:%d)\n", fapi_get_vif(mbuf), data_unit_descriptor, SLSI_FREQ_FW_TO_HOST(fapi_get_u16(mbuf, u.mlme_received_frame_ind.channel_frequency)));

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

#ifdef CONFIG_SCSC_ENABLE_P2P
	if (data_unit_descriptor == FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME) {
		struct slsi_80211_mgmt *mgmt;
		int mgmt_len;

		mgmt_len = fapi_get_mgmtlen(mbuf);
		if (!mgmt_len) {
			goto exit;
		}
		mgmt = fapi_get_mgmt(mbuf);
		if (WARN_ON(!(slsi_80211_is_action(mgmt->frame_control)))) {
			goto exit;
		}

		/* HS2 code removed, restructure P2P code below */
		{
			int subtype = slsi_p2p_get_public_action_subtype(mgmt);

			SLSI_NET_DBG2(dev, SLSI_T20_80211, "Received action frame (%s)\n", slsi_p2p_pa_subtype_text(subtype));

			if (SLSI_IS_P2P_UNSYNC_VIF(ndev_vif) && (ndev_vif->mgmt_tx_data.exp_frame != SLSI_P2P_PA_INVALID) && (subtype == ndev_vif->mgmt_tx_data.exp_frame)) {
				if (sdev->p2p_state == P2P_LISTENING) {
					SLSI_NET_WARN(dev, "Driver in incorrect P2P state (P2P_LISTENING)");
				}

				//cancel_delayed_work(&ndev_vif->unsync.del_vif_work);

				ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
				(void)slsi_mlme_reset_dwell_time(sdev, dev);
				//if (delayed_work_pending(&ndev_vif->unsync.roc_expiry_work)) {
				SLSI_P2P_STATE_CHANGE(sdev, P2P_LISTENING);
			} else {
				/*queue_delayed_work(sdev->device_wq, &ndev_vif->unsync.del_vif_work,
				   SLSI_P2P_UNSYNC_VIF_EXTRA_MSEC); */
				SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_VIF_ACTIVE);
			}
		} else if ((sdev->p2p_group_exp_frame != SLSI_P2P_PA_INVALID) && (sdev->p2p_group_exp_frame == subtype)) {
			SLSI_NET_DBG2(dev, SLSI_MLME, "Expected action frame (%s) received on Group VIF\n", slsi_p2p_pa_subtype_text(subtype));
			slsi_clear_offchannel_data(sdev, (ndev_vif->ifnum != SLSI_NET_INDEX_P2PX) ? true : false);
		}

	}

	/*  if (mgmt->u.action.category == WLAN_CATEGORY_WMM)
	   cac_rx_wmm_action(sdev, dev, mgmt, mgmt_len);

	   cfg80211_rx_mgmt(&ndev_vif->wdev, frequency, 0, (const u8 *)mgmt, mgmt_len);
	 */
}

else
#endif

	if (data_unit_descriptor == FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME) {
		struct slsi_peer *peer = NULL;
		struct ethhdr *ehdr = (struct ethhdr *)fapi_get_data(mbuf);

		peer = slsi_get_peer_from_mac(sdev, dev, ehdr->h_source);
		if (!peer) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_peer_not_present);
			SLSI_DBG1(sdev, SLSI_RX, "drop packet as No peer found\n");
			goto exit;
		}

		/* strip signal and any signal/bulk roundings/offsets */
		mbuf_pull(mbuf, fapi_get_siglen(mbuf));

		/* test for an overlength frame */
		if (mbuf->data_len > (dev->mtu + ETH_HLEN)) {
			/* A bogus length ethfrm has been encap'd. */
			/* Is someone trying an oflow attack? */
			SLSI_WARN(sdev, "oversize frame (%d > %d)\n", mbuf->data_len, dev->mtu + ETH_HLEN);
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_large_frame);
			goto exit;
		}

		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		SLSI_DBG3(sdev, SLSI_RX, "pass %u bytes to local stack\n", mbuf->data_len);
		dev->d_buf = slsi_mbuf_get_data(mbuf);
		dev->d_len = mbuf->data_len;

		if (ntohs(ehdr->h_proto) == ETH_P_PAE) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_eapol);
			if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
				SLSI_NET_DBG2(dev, SLSI_MLME, "Received eapol from AP\n");
				mbuf_pull(mbuf, sizeof(*ehdr));
				l2_packet_receive(ndev_vif->sdev->drv->ctx, ehdr->h_source, slsi_mbuf_get_data(mbuf), mbuf->data_len);
			}
			if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
				union wpa_event_data event;
				memset(&event, 0, sizeof(event));
				SLSI_NET_DBG2(dev, SLSI_MLME, "Received eapol from STA\n");
				mbuf_pull(mbuf, sizeof(*ehdr));
				event.eapol_rx.data = slsi_mbuf_get_data(mbuf);
				event.eapol_rx.data_len = mbuf->data_len;
				event.eapol_rx.src = ehdr->h_source;
				wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_EAPOL_RX, &event);
			}
		} else {
			SLSI_NET_DBG2(dev, SLSI_MLME, "sending non eapol\n");
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_mlme_dhcp);
			slsi_ethernetif_input(dev, slsi_mbuf_get_data(mbuf), mbuf->data_len);
		}
		slsi_kfree_mbuf(mbuf);

		return;
	}
exit:
SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
slsi_kfree_mbuf(mbuf);
}

void slsi_rx_mic_failure_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u8 *mac_addr;
	u16 key_type, key_id;
	union wpa_event_data event;

	SLSI_UNUSED_PARAMETER(sdev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	memset(&event, 0, sizeof(event));

	mac_addr = fapi_get_buff(mbuf, u.mlme_disconnected_ind.peer_sta_address);
	key_type = fapi_get_u16(mbuf, u.mlme_mic_failure_ind.key_type);
	key_id = fapi_get_u16(mbuf, u.mlme_mic_failure_ind.key_id);

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_mic_failure_ind(vif:%d, MAC:%pM, key_type:%d, key_id:%d)\n", fapi_get_vif(mbuf), mac_addr, key_type, key_id);

	if (WARN_ON((key_type != FAPI_KEYTYPE_GROUP) && (key_type != FAPI_KEYTYPE_PAIRWISE))) {
		goto exit;
	}

	event.michael_mic_failure.src = mac_addr;
	if (key_type == SLSI_80211_KEYTYPE_PAIRWISE) {
		event.michael_mic_failure.unicast = 1;
	}
	wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_MICHAEL_MIC_FAILURE, &event);
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	slsi_kfree_mbuf(mbuf);
}

int slsi_rx_blocking_signals(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	u16 pid, id;
	struct slsi_sig_send *sig_wait;
	u16 vif = fapi_get_vif(mbuf);

	sig_wait = &sdev->sig_wait;
	id = fapi_get_sigid(mbuf);
	pid = fapi_get_u16(mbuf, receiver_pid);

	/* ALL mlme cfm signals MUST have blocking call waiting for it (Per Vif or Global) */
	if (fapi_is_cfm(mbuf)) {
		struct netif *dev;
		struct netdev_vif *ndev_vif;
		dev = sdev->netdev[vif];
		if (dev) {
			ndev_vif = netdev_priv(dev);
			sig_wait = &ndev_vif->sig_wait;
		}
		SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);
		SLSI_DBG3(sdev, SLSI_MLME, "rx  cfm(0x%.4x, pid:0x%.4x)\n", id, pid);
		SLSI_DBG3(sdev, SLSI_MLME, "exp cfm(0x%.4x, pid:0x%.4x)\n", sig_wait->cfm_id, sig_wait->process_id);
		if (id == sig_wait->cfm_id && pid == sig_wait->process_id) {
			if (WARN_ON(sig_wait->cfm)) {
				slsi_kfree_mbuf(sig_wait->cfm);
			}
			sig_wait->cfm = mbuf;
			SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
			complete(&sig_wait->completion);
			return 0;
		}
		/**
		 * Important data frames such as EAPOL, ARP, DHCP are send
		 * over MLME. For these frames driver does not block on confirms.
		 * So there can be unexpected confirms here for such data frames.
		 * These confirms are treated as normal and is silently dropped
		 * here
		 */
		if (id == MLME_SEND_FRAME_CFM) {
			SLSI_DBG3(sdev, SLSI_MLME, "cfm(0x%.4x, pid:0x%.4x, vif:%d)\n", id, pid, vif);
			slsi_kfree_mbuf(mbuf);
			SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
			return 0;
		}

		SLSI_DBG1(sdev, SLSI_MLME, "cfm received for unblocking req (0x%.4x, pid:0x%.4x, vif:%d)\n", id, pid, vif);

		SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
		return 0;
	}
	/* Some mlme ind signals have a blocking call waiting (Per Vif or Global) */
	if (fapi_is_ind(mbuf)) {
		struct netif *dev;
		struct netdev_vif *ndev_vif;

		dev = slsi_get_netdev(sdev, vif);
		if (dev) {
			ndev_vif = netdev_priv(dev);
			sig_wait = &ndev_vif->sig_wait;
		}
		SLSI_DBG3(sdev, SLSI_MLME, "rx ind(0x%.4x, pid:0x%.4x)\n", id, pid);
		if (sig_wait->ind_id) {
			SLSI_DBG3(sdev, SLSI_MLME, "exp ind(0x%.4x, pid:0x%.4x)\n", sig_wait->ind_id, sig_wait->process_id);
		}
		SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);
		if (id == sig_wait->ind_id && pid == sig_wait->process_id) {
			if (WARN_ON(sig_wait->ind)) {
				slsi_kfree_mbuf(sig_wait->ind);
			}
			sig_wait->ind = mbuf;
			SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
			complete(&sig_wait->completion);
			return 0;
		}
		SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
	}
	return -EINVAL;
}
