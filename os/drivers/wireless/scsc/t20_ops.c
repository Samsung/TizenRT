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

#include "t20_ops.h"
#include "debug_scsc.h"
#include "mgt.h"
#include "mlme.h"
#include "netif.h"
#include "unifiio.h"
#include "mib.h"
#include "netif.h"
#include "wpa_driver_copy.h"

#define SLSI_FW_SCAN_DONE_TIMEOUT_MSEC (15 * 1000)
#define SLSI_MAX_CHAN_2G_BAND          14
#define FW_TX_POWER_LEVEL 4		/*Expressed in 0.25 dBm units */
#define SLSI_FORCE_SCHD_ACT_FRAME_MSEC 100

#ifdef CONFIG_SLSI_WLAN_P2P
static uint keep_alive_period = SLSI_P2PGO_KEEP_ALIVE_PERIOD_SEC;
#endif

#define RATE_LEGACY(_rate, _hw_value, _flags) { \
		.bitrate = (_rate), \
		.hw_value = (_hw_value), \
		.flags = (_flags), \
}

#define CHAN2G(_freq, _idx)  { \
		.band = SLSI_WLAN_BAND_2GHZ, \
		.center_freq = (_freq), \
		.hw_value = (_idx), \
}

#ifdef CONFIG_SCSC_ADV_FEATURE
#define CHAN5G(_freq, _idx)  { \
		.band = SLSI_WLAN_BAND_5GHZ, \
		.center_freq = (_freq), \
		.hw_value = (_idx), \
}
#endif

static struct slsi_wlan_channel slsi_2ghz_channels[] = {
	CHAN2G(2412, 1),
	CHAN2G(2417, 2),
	CHAN2G(2422, 3),
	CHAN2G(2427, 4),
	CHAN2G(2432, 5),
	CHAN2G(2437, 6),
	CHAN2G(2442, 7),
	CHAN2G(2447, 8),
	CHAN2G(2452, 9),
	CHAN2G(2457, 10),
	CHAN2G(2462, 11),
	CHAN2G(2467, 12),
	CHAN2G(2472, 13),
	CHAN2G(2484, 14),
};

static struct slsi_wlan_rate slsi_11g_rates[] = {
	RATE_LEGACY(10, 1, 0),
	RATE_LEGACY(20, 2, 1),
	RATE_LEGACY(55, 3, 1),
	RATE_LEGACY(110, 6, 1),
	RATE_LEGACY(60, 4, 0),
	RATE_LEGACY(90, 5, 0),
	RATE_LEGACY(120, 7, 0),
	RATE_LEGACY(180, 8, 0),
	RATE_LEGACY(240, 9, 0),
	RATE_LEGACY(360, 10, 0),
	RATE_LEGACY(480, 11, 0),
	RATE_LEGACY(540, 12, 0),
};

static u32 wpa_alg_to_cipher_suite(enum slsi_wpa_alg alg, size_t key_len)
{
	switch (alg) {
	case SLSI_WPA_ALG_TKIP:
		return SLSI_WLAN_CIPHER_SUITE_TKIP;
	case SLSI_WPA_ALG_CCMP:
		return SLSI_WLAN_CIPHER_SUITE_CCMP;
	case SLSI_WPA_ALG_GCMP:
		return SLSI_WLAN_CIPHER_SUITE_GCMP;
	case SLSI_WPA_ALG_SMS4:
		return SLSI_WLAN_CIPHER_SUITE_SMS4;
	case SLSI_WPA_ALG_WEP:
		if (key_len == 5) {
			return SLSI_WLAN_CIPHER_SUITE_WEP_40;
		}
		return SLSI_WLAN_CIPHER_SUITE_WEP_104;
	default:					/* SLSI_WPA_ALG_NONE/SLSI_WPA_ALG_PMK/SLSI_WPA_ALG_IGTK */
		SLSI_ERR_NODEV(" Unexpected encryption algorithm %d", alg);
		return 0;
	}
}

int slsi_add_key(const char *ifname, void *priv, enum slsi_wpa_alg alg, const u8 *mac_addr, int key_index, int set_tx, const u8 *seq, size_t seq_len, const u8 *key, size_t key_len)
{
	struct slsi_drv_interface *drv_iface;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer = NULL;
	int r = 0;
	u16 key_type = FAPI_KEYTYPE_GROUP;
	struct slsi_key_params params;
	bool pairwise = FALSE;

	drv_iface = priv;
	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver interface not available\n");
		return -EINVAL;
	}

	dev = drv_iface->dev;
	sdev = drv_iface->sdev;

	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	if (WARN_ON(pairwise && !mac_addr)) {
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "(key_index:%d, set_tx :%d, address:" SLSI_MAC_FORMAT ", seq_len:0x%.8X, key_len:%d)\n", key_index, set_tx, SLSI_MAC_STR(mac_addr), seq_len, key_len);

	if (!ndev_vif->activated) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "vif not active\n");
		goto exit;
	}
	if (alg == SLSI_WPA_ALG_NONE) {
		goto exit;
	}
	params.key = key;
	params.key_len = key_len;
	params.seq = seq;
	params.seq_len = seq_len;
	params.cipher = wpa_alg_to_cipher_suite(alg, key_len);
	if (alg != SLSI_WPA_ALG_WEP && key_index && slsi_is_broadcast_addr(mac_addr)) {
		key_type = FAPI_KEYTYPE_GROUP;
	} else {
		key_type = FAPI_KEYTYPE_PAIRWISE;
		pairwise = TRUE;
	}

	if (params.cipher == WLAN_CIPHER_SUITE_PMK) {
		r = slsi_mlme_set_pmk(sdev, dev, params.key, params.key_len);
		goto exit;
	}

	if (mac_addr && pairwise) {
		/* All Pairwise Keys will have a peer record. */
		peer = slsi_get_peer_from_mac(sdev, dev, mac_addr);
		if (peer) {
			mac_addr = peer->address;
		}
	} else if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
		/* Sta Group Key will use the peer address */
		peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
		if (peer) {
			mac_addr = peer->address;
		}
	} else if (ndev_vif->vif_type == FAPI_VIFTYPE_AP && !pairwise) {
		/* AP Group Key will use the Interface address */
		mac_addr = dev->d_mac.ether_addr_octet;
	} else {
		r = -EINVAL;
		goto exit;
	}
	SLSI_NET_DBG2(dev, SLSI_T20_80211, "(key_index:%d, pairwise:%d, address:" SLSI_MAC_FORMAT ", cipher:0x%.8X, key_len:%d)\n", key_index, pairwise, SLSI_MAC_STR(mac_addr), params.cipher, params.key_len);

	/*Treat WEP key as pairwise key */
	if ((ndev_vif->vif_type == FAPI_VIFTYPE_STATION) && ((params.cipher == SLSI_WLAN_CIPHER_SUITE_WEP_40) || (params.cipher == SLSI_WLAN_CIPHER_SUITE_WEP_104)) && peer) {
		u8 bc_mac_addr[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

		SLSI_NET_DBG3(dev, SLSI_T20_80211, "WEP Key: store key\n");
		r = slsi_mlme_set_key(sdev, dev, key_index, FAPI_KEYTYPE_DEFAULT, bc_mac_addr, &params);
		if (r != FAPI_RESULTCODE_SUCCESS) {
			SLSI_NET_ERR(dev, "Error adding WEP key\n");
		}
		goto exit;
	}

	if (pairwise) {
		key_type = FAPI_KEYTYPE_PAIRWISE;
		if (WARN_ON(!peer)) {
			r = -EINVAL;
			goto exit;
		}
	} else if (params.cipher == SLSI_WLAN_CIPHER_SUITE_AES_CMAC) {
		key_type = FAPI_KEYTYPE_IGTK;
	}

	if (WARN(!mac_addr, "mac_addr not defined\n")) {
		r = -EINVAL;
		goto exit;
	}
	SLSI_NET_DBG2(dev, SLSI_T20_80211, "mac_addr = %x %x %x %x %x %x \n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
	if (!((ndev_vif->vif_type == FAPI_VIFTYPE_AP) && (key_index == 4))) {
		r = slsi_mlme_set_key(sdev, dev, key_index, key_type, mac_addr, &params);
		if (r) {
			SLSI_NET_ERR(dev, "error in adding key (key_type: %d)\n", key_type);
			goto exit;
		}
	}

	if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
		ndev_vif->sta.eap_hosttag = 0xFFFF;
	}

	if (key_type == FAPI_KEYTYPE_GROUP) {
		ndev_vif->sta.group_key_set = true;
		ndev_vif->ap.cipher = params.cipher;
	} else if (key_type == FAPI_KEYTYPE_PAIRWISE) {
		peer->pairwise_key_set = true;
	}

	if (peer) {
		if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
			if (pairwise) {
				if (params.cipher == SLSI_WLAN_CIPHER_SUITE_SMS4) {	/*WAPI */
					slsi_mlme_connect_resp(sdev, dev);
#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
					slsi_mlme_powermgt(sdev, dev, FAPI_POWERMANAGEMENTMODE_POWER_SAVE);
#endif
					slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_CONNECTED);
				}
				//slsi_set_packet_filters(sdev, dev);
			}

			if (ndev_vif->sta.gratuitous_arp_needed) {
				ndev_vif->sta.gratuitous_arp_needed = false;
				slsi_send_gratuitous_arp(sdev, dev);
			}
		} else if (ndev_vif->vif_type == FAPI_VIFTYPE_AP && pairwise) {
			slsi_mlme_connected_resp(sdev, dev, peer->aid);
			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_CONNECTED);
			peer->connected_state = SLSI_STA_CONN_STATE_CONNECTED;
#ifdef CONFIG_SLSI_WLAN_P2P
			if (ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GO) {
				ndev_vif->ap.p2p_gc_keys_set = true;
			}
#endif
		}
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

int slsi_get_ssid(void *priv, u8 *ssid)
{
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	struct slsi_drv_interface *drv_iface = priv;

	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver interface not available\n");
		return -EINVAL;
	}

	dev = drv_iface->dev;
	sdev = drv_iface->sdev;

	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	if ((ndev_vif->vif_type == FAPI_VIFTYPE_STATION) && (ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)) {
		peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
		if (peer == NULL) {
			return 0;
		}

		memcpy(ssid, peer->ssid, peer->ssid_len);
		return peer->ssid_len;
	}

	return 0;
}

int slsi_del_key(void *priv, struct netif *dev, u8 key_index, bool pairwise, const u8 *mac_addr)
{
	SLSI_UNUSED_PARAMETER(key_index);
	SLSI_UNUSED_PARAMETER(pairwise);
	SLSI_UNUSED_PARAMETER(mac_addr);

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_DELETEKEYS.request\n");
		return -EOPNOTSUPP;
	}

	/* Firmware does not require a del key */
	SLSI_NET_DBG3(dev, SLSI_T20_80211, "index: %d pairwise: %d " SLSI_MAC_FORMAT "\n", key_index, pairwise, SLSI_MAC_STR(mac_addr));
	return 0;
}

int slsi_get_key(const char *ifname, void *priv, const u8 *addr, int idx, u8 *seq)
{
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int r = 0, seq_len = 0;
	struct slsi_drv_interface *drv_iface = priv;

	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver interface not available\n");
		return -EINVAL;
	}

	dev = drv_iface->dev;
	sdev = drv_iface->sdev;

	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_GET_KEY_SEQUENCE.request\n");
		return -EOPNOTSUPP;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "(key_index:%d, pairwise:%d)\n", idx, (addr != NULL));

	if (!ndev_vif->activated) {
		SLSI_NET_ERR(dev, "vif not active\n");
		r = -EINVAL;
		goto exit;
	}

	/* The get_key call is expected only for AP vif with Group Key type */
	if (FAPI_VIFTYPE_AP != ndev_vif->vif_type) {
		SLSI_NET_ERR(dev, "Invalid vif type: %d\n", ndev_vif->vif_type);
		r = -EINVAL;
		goto exit;
	}
	if (addr) {
		SLSI_NET_ERR(dev, "Invalid key type\n");
		r = -EINVAL;
		goto exit;
	}

	if (!((ndev_vif->vif_type == FAPI_VIFTYPE_AP) && (idx == 4))) {
		r = slsi_mlme_get_key(sdev, dev, idx, FAPI_KEYTYPE_GROUP, seq, &seq_len);
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

static size_t slsi_strip_wsc_p2p_ie(const u8 *src_ie, size_t src_ie_len, u8 *dest_ie, bool strip_wsc, bool strip_p2p)
{
	const u8 *ie;
	const u8 *next_ie;
	size_t dest_ie_len = 0;

	if (!dest_ie || !(strip_p2p || strip_wsc)) {
		return dest_ie_len;
	}

	for (ie = src_ie; (ie - src_ie) < src_ie_len; ie = next_ie) {
		next_ie = ie + ie[1] + 2;

		if (ie[0] == SLSI_WLAN_EID_VENDOR_SPEC && ie[1] > 4) {
			int i;
			unsigned int oui = 0;

			for (i = 0; i < 4; i++) {
				oui = (oui << 8) | ie[5 - i];
			}

			if (strip_wsc && (oui == SLSI_WPS_OUI_PATTERN)) {
				continue;
			}
			if (strip_p2p && (oui == SLSI_P2P_OUI_PATTERN)) {
				continue;
			}
		}

		if (next_ie - src_ie <= src_ie_len) {
			memcpy(dest_ie + dest_ie_len, ie, ie[1] + 2);
			dest_ie_len += ie[1] + 2;
		}
	}

	return dest_ie_len;
}

const u8 *slsi_get_mac_addr(void *priv)
{
	struct slsi_drv_interface *drv_iface = priv;

	struct netif *dev;

	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver interface not available\n");
		return NULL;
	}

	dev = drv_iface->dev;

	SLSI_NET_INFO(dev, "MAC address: " SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(dev->d_mac.ether_addr_octet));
	return dev->d_mac.ether_addr_octet;
}

int slsi_hw_scan(void *priv, struct wpa_driver_scan_params *request)
{
	struct slsi_drv_interface *drv_iface = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	u16 scan_type = FAPI_SCANTYPE_FULL_SCAN;
	int r, i;

#ifdef CONFIG_SLSI_WLAN_P2P
	u16 p2p_state;
#endif
	u8 *scan_ie = NULL;;
	size_t scan_ie_len;
	bool strip_wsc = false;
	bool strip_p2p = false;
	struct slsi_wlan_channel *channels[15];
	int chan_count = 0;
	drv_iface = priv;
	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver interface not available\n");
		return -EIO;
	}

	dev = drv_iface->dev;
	sdev = drv_iface->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_WARN(dev, "Not supported in WlanLite mode\n");
		return -EOPNOTSUPP;
	}

	ndev_vif = netdev_priv(dev);

#ifdef CONFIG_SLSI_WLAN_P2P
	p2p_state = sdev->p2p_state;
#endif
	/* To handle race conditions check driver is UP */
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->is_available) {
		SLSI_ERR_NODEV("Driver is down\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

#ifdef CONFIG_SLSI_WLAN_P2P
	/* Reject scan request if Group Formation is in progress */
	if (sdev->p2p_state == P2P_ACTION_FRAME_TX_RX) {
		SLSI_NET_INFO(dev, "Scan received in P2P Action Frame Tx/Rx state - Reject\n");
		return -EBUSY;
	}
#endif

	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

	if (ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req) {
		SLSI_NET_INFO(dev, "Rejecting scan request as last scan is still running\n");
		r = -EBUSY;
		goto exit;
	}

	/* Convert scan params freqs to slsi_wlan_channel. Only 2.4 GHz channels considered for now.
	 * The scan params freqs is of the form '2412 MHz' OR NULL for all frequencies
	 */
	if (request->freqs == NULL) {
		chan_count = ARRAY_SIZE(slsi_2ghz_channels);
		for (i = 0; i < chan_count; i++) {
			channels[i] = &slsi_2ghz_channels[i];
		}
	} else {
		int j = 0;
		for (i = 0; request->freqs[i]; i++) {
			int freq = request->freqs[i];
			if (freq >= 2412 && freq <= 2484) {
				int k = (freq - 2412) / 5;
				channels[j] = &slsi_2ghz_channels[k];
				j++;
			}
		}
		chan_count = j;
	}

	SLSI_NET_DBG3(dev, SLSI_T20_80211, "channels:%d, ssids:%d, ie_len:%d\n", chan_count, request->num_ssids, (int)request->extra_ies_len);

#ifdef CONFIG_SLSI_WLAN_P2P
	/* Update scan timing for P2P social channels scan.
	 * Checking for SSID and num_channel looks good enough, hence skip social channel check
	 */
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif) && (request->num_ssids == 1) && SLSI_IS_P2P_SSID(request->ssids[0].ssid, request->ssids[0].ssid_len)) {
		/* In supplicant during joining procedure the P2P GO scan
		 * with GO's operating channel comes on P2P device. Hence added the
		 * check for n_channels as 1
		 */
		if (chan_count == SLSI_P2P_SOCIAL_CHAN_COUNT || chan_count == 1) {
			p2p_state = P2P_SCANNING;
			scan_type = FAPI_SCANTYPE_P2P_SCAN_SOCIAL;
		} else if (chan_count > SLSI_P2P_SOCIAL_CHAN_COUNT) {
			scan_type = FAPI_SCANTYPE_P2P_SCAN_FULL;
		}
	}
#endif

	if (SLSI_IS_VIF_INDEX_WLAN(ndev_vif) && (request->extra_ies)) {
		const u8 *ie;

		/* check for interworking bit in extended capabilties */
		r = slsi_mlme_set_iw_ext_cap(sdev, dev, request->extra_ies, request->extra_ies_len);
		if (r) {
			goto exit;
		}

		/* Supplicant adds wsc and p2p in Station scan at the end of scan request ie.
		 * for non-wps case remove both wps and p2p IEs
		 * for wps case remove only p2p IE
		 */

		ie = (u8 *) slsi_wlan_find_vendor_spec_ie(SLSI_WLAN_VS_OUI_MICROSOFT, SLSI_WLAN_VS_OUI_TYPE_MS_WPS, request->extra_ies, request->extra_ies_len);
		if (ie && ie[1] > SLSI_WPS_REQUEST_TYPE_POS && ie[SLSI_WPS_REQUEST_TYPE_POS] == SLSI_WPS_REQUEST_TYPE_ENROLEE_INFO_ONLY) {
			strip_wsc = true;
		}

		ie = (u8 *) slsi_wlan_find_vendor_spec_ie(SLSI_WLAN_VS_OUI_WFA, SLSI_WLAN_VS_OUI_TYPE_WFA_P2P, request->extra_ies, request->extra_ies_len);
		if (ie) {
			strip_p2p = true;
		}
	}

	if (strip_wsc || strip_p2p) {
		scan_ie = kmm_malloc(request->extra_ies_len);
		if (!scan_ie) {
			SLSI_NET_INFO(dev, "Out of memory for scan IEs\n");
			r = -ENOMEM;
			goto exit;
		}
		scan_ie_len = slsi_strip_wsc_p2p_ie(request->extra_ies, request->extra_ies_len, scan_ie, strip_wsc, strip_p2p);
		slsi_mbuf_queue_purge(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		r = slsi_mlme_add_scan(sdev, dev, scan_type, FAPI_REPORTMODE_REAL_TIME, request->num_ssids, request->ssids, chan_count, channels, NULL, scan_ie, scan_ie_len, false);
		kmm_free(scan_ie);
	} else {
		scan_ie = (u8 *) request->extra_ies;
		scan_ie_len = request->extra_ies_len;
		slsi_mbuf_queue_purge(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		r = slsi_mlme_add_scan(sdev, dev, scan_type, FAPI_REPORTMODE_REAL_TIME, request->num_ssids, request->ssids, chan_count, channels, NULL, scan_ie, scan_ie_len, false);

	}

	if (r != 0) {
		if (r > 0) {
			SLSI_NET_DBG2(dev, SLSI_T20_80211, "Nothing to be done\n");
			slsi_scan_complete(ndev_vif);
			r = 0;
		} else {
			SLSI_NET_DBG2(dev, SLSI_T20_80211, "add_scan_req error: %d\n", r);
			r = -EIO;
		}
	} else {
		ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req = 1;
		work_queue(SCSC_WORK, &ndev_vif->scan_timeout_work, slsi_scan_ind_timeout_handle, (FAR void *)drv_iface, MSEC2TICK(SLSI_FW_SCAN_DONE_TIMEOUT_MSEC));
#ifdef CONFIG_SLSI_WLAN_P2P
		/* Update State only for scan in Device role */
		if (SLSI_IS_VIF_INDEX_P2P(ndev_vif) && (!SLSI_IS_P2P_GROUP_STATE(sdev))) {
			SLSI_P2P_STATE_CHANGE(sdev, p2p_state);
		} else if (SLSI_IS_VIF_INDEX_P2P_GROUP(ndev_vif) && (request->extra_ies)) {
			/* Supplicant includes only WPS and P2P IE as of now. Hence a blind copy is done. */
			SLSI_NET_DBG3(dev, SLSI_T20_80211, "Scan IEs present with length = %zu\n", request->extra_ies_len);
			sdev->gc_probe_req_ies = kmm_malloc(request->extra_ies_len);
			if (sdev->gc_probe_req_ies == NULL) {	/* Don't fail, continue as it would still work */
				sdev->gc_probe_req_ie_len = 0;
			} else {
				sdev->gc_probe_req_ie_len = request->extra_ies_len;
				sdev->delete_gc_probe_req_ies = true;
				memcpy(sdev->gc_probe_req_ies, request->extra_ies, request->extra_ies_len);
			}
		}
#endif
	}
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
	return r;
}

/**
 * slsi_add_wpa_scan_entry
 *
 * Description:
 *	Process the scan_ind frame (beacon/probe response) and store the results in
 *	wpa_scan_results struct format. The scan_ind data needs to be freed here.
 *	Allocations of wpa_scan_results structures and others within it is freed by supplicant.
 *
 * Parameters:
 *	results - Pointer to the wpa_scan_results to be sent to supplicant
 *	scan - mlme_scan_ind contents
 *	dev - netdev
 *
 * Returns:
 *	The wpa_scan_results is updated with the scan results.
 *	Also number of scan result entries is updated.
 */
static void slsi_add_wpa_scan_entry(struct wpa_scan_results *results, struct max_buff *scan, struct netif *dev)
{
	struct wpa_scan_res *scan_res;
	struct ieee80211_mgmt *frame = fapi_get_mgmt(scan);
	size_t frame_len = fapi_get_mgmtlen(scan);
	size_t ie_len = frame_len - (frame->u.beacon.variable - (u8 *) frame);
	u8 *scan_res_ies, *ies;

	scan_res = kmm_zalloc(sizeof(*scan_res) + ie_len);
	if (!scan_res) {
		SLSI_NET_ERR(dev, "Failed to allocate memory for scan entry (wpa_scan_res)\n");
		return;
	}

	memcpy(scan_res->bssid, frame->bssid, ETH_ALEN);
	scan_res->freq = slsi_find_scan_freq(frame, frame_len, SLSI_FREQ_FW_TO_HOST(fapi_get_u16(scan, u.mlme_scan_ind.channel_frequency)));
	scan_res->beacon_int = frame->u.beacon.beacon_int;
	scan_res->caps = frame->u.beacon.capab_info;
	scan_res->qual = fapi_get_s16(scan, u.mlme_scan_ind.rssi);
	scan_res->tsf = frame->u.beacon.timestamp;
	scan_res->ie_len = ie_len;

	scan_res_ies = (u8 *)(scan_res + 1);
	ies = &(frame->u.beacon.variable[0]);
	memcpy(scan_res_ies, ies, ie_len);
	results->res[results->num++] = scan_res;
	slsi_kfree_mbuf(scan);
}

/**
 * slsi_get_scan_results
 *
 * Description:
 *	Process the cached scan results of driver and send it to supplicant
 *	in the appropriate (struct wpa_scan_results) format.
 *
 * Parameters:
 *	priv - Pointer to the private driver data
 *
 * Returns: Scan results on Success, NULL on failure
 */
struct wpa_scan_results *slsi_get_scan_results(void *priv)
{
	struct slsi_drv_interface *drv_iface = priv;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct wpa_scan_results *results = NULL;
	struct max_buff *scan;
	int scan_id = SLSI_SCAN_HW_ID;
	int num_scan_ind;

	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver not available\n");
		return NULL;
	}

	dev = drv_iface->dev;

	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return NULL;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

	if (ndev_vif->scan[scan_id].scan_req != 1) {
		SLSI_NET_ERR(dev, "Scan results is not available\n");
		goto exit_with_lock;
	}

	num_scan_ind = ndev_vif->scan[scan_id].scan_results.queue_len;

	SLSI_NET_DBG3(dev, SLSI_T20_80211, "slsi_get_scan_results(interface:%d, scan_id:%d, num_scan_ind: %d)\n", ndev_vif->ifnum, scan_id, num_scan_ind);
	if (num_scan_ind == 0) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "No scan results acquired. Return\n");
		ndev_vif->scan[scan_id].scan_req = 0;
		goto exit_with_lock;
	}
	results = kmm_zalloc(sizeof(*results));
	if (results == NULL) {
		SLSI_NET_ERR(dev, "Failed to allocate memory for wpa_scan_results\n");
		goto exit_with_free;
	}

	results->res = kmm_zalloc(sizeof(*results->res) * num_scan_ind);
	if (results->res == NULL) {
		SLSI_NET_ERR(dev, "Failed to allocate memory for wpa_scan_res array\n");
		kmm_free(results);
		results = NULL;			//Need to return NULL in  this case
		goto exit_with_free;
	}

	if (ndev_vif->scan[scan_id].scan_req) {
		scan = slsi_mbuf_dequeue(&ndev_vif->scan[scan_id].scan_results);
		while (scan) {
			SLSI_NET_DBG3(dev, SLSI_T20_80211, "mlme_scan_ind(scan_id:%d, %pM, freq:%d, rssi:%d)\n", fapi_get_u16(scan, u.mlme_scan_ind.scan_id), fapi_get_mgmt(scan)->bssid, SLSI_FREQ_FW_TO_HOST(fapi_get_u16(scan, u.mlme_scan_ind.channel_frequency)), fapi_get_s16(scan, u.mlme_scan_ind.rssi));

			/* The scan result (scan_ind) would be freed inside slsi_add_wpa_scan_entry */
			slsi_add_wpa_scan_entry(results, scan, dev);
			scan = slsi_mbuf_dequeue(&ndev_vif->scan[scan_id].scan_results);
		}
		ndev_vif->scan[scan_id].scan_req = 0;
	}
	SLSI_NET_DBG3(dev, SLSI_T20_80211, "slsi_get_scan_results sent\n");

	/* Fetch time - Supplicant updates the timestamp if 0 is passed from driver */

	if (num_scan_ind != results->num) {
		SLSI_NET_ERR(dev, "Mismatch in count of num_scan_ind (%d) and scan result array entries (%d)\n", num_scan_ind, results->num);
	}

	goto exit_with_lock;

exit_with_free:
	/* Free the cached scan results from driver */
	scan = slsi_mbuf_dequeue(&ndev_vif->scan[scan_id].scan_results);
	while (scan) {
		slsi_kfree_mbuf(scan);
		scan = slsi_mbuf_dequeue(&ndev_vif->scan[scan_id].scan_results);
	}

exit_with_lock:

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);

	return results;
}

int slsi_get_ap_bssid(void *priv, u8 *bssid)
{
	struct slsi_drv_interface *drv_iface = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	u8 null_address[ETH_ALEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	if (!drv_iface) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	dev = drv_iface->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	sdev = drv_iface->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	if ((ndev_vif->vif_type == FAPI_VIFTYPE_STATION) && (ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)) {
		peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
		SLSI_ETHER_COPY(bssid, peer->address);
		return 0;
	} else {
		SLSI_ETHER_COPY(bssid, null_address);
		return -1;
	}
}

int slsi_connect(void *priv, struct wpa_driver_associate_params *request)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	u8 device_address[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int r = 0;
	struct slsi_peer *peer;
	u16 prev_vif_type;
	u32 action_frame_bmap;
#ifdef CONFIG_SLSI_WLAN_P2P
	struct netif *p2p_dev;
#endif

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_CONNECT.request\n");
		return -EOPNOTSUPP;
	}

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);
	if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
		SLSI_WARN(sdev, "device not started yet (device_state:%d)\n", sdev->device_state);
		SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "%.*s, " SLSI_MAC_FORMAT "\n", (int)request->ssid_len, request->ssid, SLSI_MAC_STR(request->bssid));
#ifdef CONFIG_SLSI_WLAN_P2P
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		SLSI_NET_ERR(dev, "Connect requested on incorrect vif\n");
		goto exit_with_error;
	}
#endif
	if (WARN_ON(!request->ssid)) {
		goto exit_with_error;
	}

	if (WARN_ON(request->ssid_len > SLSI_WLAN_MAX_SSID_LEN)) {
		goto exit_with_error;
	}

	if (request->mode == IEEE80211_MODE_AP) {
		if (ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GROUP) {
			ndev_vif->iftype = SLSI_WLAN_IFTYPE_P2P_GO;
		}
		goto exit;
	}

	if (WARN_ON(ndev_vif->activated)) {
		goto exit_with_error;
	}

	if (WARN_ON(ndev_vif->vif_type == FAPI_VIFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_UNSPECIFIED)) {
		SLSI_NET_ERR(dev, "VIF status: %d\n", ndev_vif->sta.vif_status);
		goto exit_with_error;
	}
	prev_vif_type = ndev_vif->vif_type;

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "iftype: %d\n", ndev_vif->iftype);

	switch (ndev_vif->iftype) {
	case SLSI_WLAN_IFTYPE_UNSPECIFIED:
	case SLSI_WLAN_IFTYPE_STATION:
		ndev_vif->iftype = SLSI_WLAN_IFTYPE_STATION;
		action_frame_bmap = SLSI_ACTION_FRAME_WMM | SLSI_ACTION_FRAME_PUBLIC;
		break;
#ifdef CONFIG_SLSI_WLAN_P2P
	/**
	 * For P2P iftype is initialized in slsi_add_virtual_intf()
	 * For negotiated group the iftype will be SLSI_WLAN_IFTYPE_P2P_GROUP.
	 */
	case SLSI_WLAN_IFTYPE_P2P_CLIENT:
	case SLSI_WLAN_IFTYPE_P2P_GROUP:
		ndev_vif->vif_type = SLSI_WLAN_IFTYPE_P2P_CLIENT;
		slsi_p2p_group_start_remove_unsync_vif(sdev);
		p2p_dev = slsi_get_netdev_locked(sdev, SLSI_NET_INDEX_P2P);
		if (p2p_dev) {
			SLSI_ETHER_COPY(device_address, p2p_dev->d_mac.ether_addr_octet);
		}
		action_frame_bmap = SLSI_ACTION_FRAME_PUBLIC;
		break;
#endif
	default:
		SLSI_NET_ERR(dev, "Invalid Device Type: %d\n", ndev_vif->iftype);
		goto exit_with_error;
	}

	ndev_vif->vif_type = FAPI_VIFTYPE_STATION;
	/* Abort on-going scans (if any) */
	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);
	if (ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Abort on-going scan\n");
		if (slsi_is_work_pending(&ndev_vif->scan_timeout_work)) {
			work_cancel(SCSC_WORK, &ndev_vif->scan_timeout_work);
		}
		(void)slsi_mlme_del_scan(sdev, dev, ndev_vif->ifnum << 8 | SLSI_SCAN_HW_ID);
		slsi_mbuf_queue_purge(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		slsi_scan_complete(ndev_vif);
		ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req = 0;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);

	ndev_vif->center_freq = request->freq_hint;
	if (slsi_mlme_add_vif(sdev, dev, dev->d_mac.ether_addr_octet, device_address) != 0) {
		SLSI_NET_ERR(dev, "slsi_mlme_add_vif failed\n");
		goto exit_with_error;
	}

	if (slsi_vif_activated(sdev, dev) != 0) {
		SLSI_NET_ERR(dev, "slsi_vif_activated failed\n");
		goto exit_with_vif;
	}

	if (slsi_mlme_register_action_frame(sdev, dev, action_frame_bmap, action_frame_bmap) != 0) {
		SLSI_NET_ERR(dev, "Action frame registration failed for bitmap value %d\n", action_frame_bmap);
		goto exit_with_vif;
	}
#ifdef CONFIG_SLSI_WLAN_P2P
#ifdef CONFIG_SCSC_ADV_FEATURE
	if (sdev->device_config.qos_info != 0) {
		r = slsi_set_uapsd_qos_info(sdev, dev, sdev->device_config.qos_info);
		if (r != 0) {
			SLSI_NET_ERR(dev, "qosInfo MIB write failed: %d\n", r);
			goto exit_with_vif;
		}
	}
#endif
	/* If P2P CLI, add_info_elements with Probe Req IEs. Proceed even if confirm fails for add_info as it would still work if the
	 * fw pre-join scan does not include the vendor IEs
	 */
	if ((ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_CLIENT) && (sdev->gc_probe_req_ies)) {
#ifdef CONFIG_SCSC_ADV_FEATURE
		if (sme->crypto.wpa_versions == 2) {
			sdev->delete_gc_probe_req_ies = true;    /* Stored P2P Probe Req can be deleted at vif deletion, after WPA2 association  */
		} else
#endif
			/* Retain stored P2P Probe Req at vif deletion until WPA2 connection to allow Probe request with P2P IE, after WSC */
			sdev->delete_gc_probe_req_ies = false;
		SLSI_NET_DBG3(dev, SLSI_T20_80211, "Setting Probe Req IEs for P2P CLI\n");
		if (slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_REQUEST, sdev->gc_probe_req_ies, sdev->gc_probe_req_ie_len) != 0) {
			SLSI_NET_ERR(dev, "Failed to add information elments\n");
		}
	}
#endif

	ndev_vif->sta.vif_status = SLSI_VIF_STATUS_CONNECTING;
	r = slsi_mlme_connect(sdev, dev, request);
	if (r != 0) {
		ndev_vif->sta.is_wps = false;
		SLSI_NET_ERR(dev, "connect failed: %d\n", r);
		goto exit_with_vif;
	}
	memcpy(ndev_vif->sta.mac_addr, request->bssid, ETH_ALEN);

	peer = slsi_peer_add(sdev, dev, (u8 *) request->bssid, SLSI_STA_PEER_QUEUESET + 1);
	ndev_vif->sta.resp_id = 0;

	if (!peer) {
		goto exit_with_error;
	}

	goto exit;

exit_with_vif:
	slsi_mlme_del_vif(sdev, dev);
	slsi_vif_deactivated(sdev, dev);
exit_with_error:
	r = -EINVAL;
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return r;
}

int slsi_disconnect(void *priv, const u8 *addr, int reason_code)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	int r = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "reason: %d\n", reason_code);

	/* Assuming that the time it takes the firmware to disconnect is not significant
	 * as this function holds the locks until the MLME-DISCONNECT-IND comes back.
	 * Unless the MLME-DISCONNECT-CFM fails.
	 */
	if (!ndev_vif->activated) {
		r = -EINVAL;
		goto exit;
	}

	peer = ndev_vif->peer_sta_record[SLSI_STA_PEER_QUEUESET];

	switch (ndev_vif->vif_type) {
	case FAPI_VIFTYPE_STATION: {
		/* Disconnecting spans several host firmware interactions so track the status
		 * so that the Host can ignore connect related signaling eg. MLME-CONNECT-IND
		 * now that it has triggered a disconnect.
		 */
		ndev_vif->sta.vif_status = SLSI_VIF_STATUS_DISCONNECTING;

		if (peer->valid) {
			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DISCONNECTED);
		}

		/* MLME-DISCONNECT_CFM only means that the firmware has accepted the request it has not yet
		 * disconnected. Completion of the disconnect is indicated by MLME-DISCONNECT-IND, so have
		 * to wait for that before deleting the VIF. Also any new activities eg. connect can not yet
		 * be started on the VIF until the disconnection is completed. So the MLME function also handles
		 * waiting for the MLME-DISCONNECT-IND (if the CFM is successful)
		 */

		r = slsi_mlme_disconnect(sdev, dev, peer->address, reason_code, true);
		if (r != 0) {
			SLSI_NET_ERR(dev, "Disconnection returned with failure\n");
		}
		/* Even if we fail to disconnect cleanly, tidy up. */
		r = slsi_handle_disconnect(sdev, dev, peer->address, 0, 1);

		break;
	}
	default:
		SLSI_NET_WARN(dev, "Invalid - vif type:%d, device type:%d)\n", ndev_vif->vif_type, ndev_vif->iftype);
		r = -EINVAL;
		break;
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_NET_DBG3(dev, SLSI_T20_80211, "Completed disconnect\n");
	return r;
}

int slsi_set_rts(void *priv, int rts)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	int r = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}
	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	SLSI_DBG1(sdev, SLSI_T20_80211, "slsi_set_rts");

	SLSI_DBG1(sdev, SLSI_T20_80211, "RTS Threshold = %d\n", rts);
	r = slsi_set_uint_mib(sdev, NULL, SLSI_PSID_DOT11_RTS_THRESHOLD, rts);
	if (r != 0) {
		SLSI_ERR(sdev, "Setting RTS_THRESHOLD failed\n");
		return r;
	}
	return r;
}

int slsi_get_signal_poll(void *priv, struct wpa_signal_info *si)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int res = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->activated) {
		SLSI_ERR_NODEV("STA Not Connected\n");
		res = -EINVAL;
		goto exit;
	}
	memset(si, 0, sizeof(*si));
	if (((ndev_vif->iftype == SLSI_WLAN_IFTYPE_STATION && ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)
		 || ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_CLIENT)) {
		/*Read MIB and fill into the peer.sinfo */
		res = slsi_mlme_get_sinfo_mib(sdev, dev, si);
		if (res) {
			SLSI_NET_DBG1(dev, SLSI_T20_80211, "failed to read Station Info Error:%d\n", res);
			goto exit;
		}
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return res;
}

int slsi_set_frag_threshold(void *priv, int frag_threshold)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	int r = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}
	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	SLSI_DBG1(sdev, SLSI_T20_80211, "slsi_set_frag_threshold");

	SLSI_DBG1(sdev, SLSI_T20_80211, "Frag Threshold = %d\n", frag_threshold);
	r = slsi_set_uint_mib(sdev, NULL, SLSI_PSID_DOT11_FRAGMENTATION_THRESHOLD, frag_threshold);
	if (r != 0) {
		SLSI_ERR(sdev, "Setting FRAG_THRESHOLD failed\n");
		return r;
	}

	return r;
}

int slsi_set_tx_power(void *priv, int dbm)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}
	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	SLSI_DBG1_NODEV(SLSI_MLME, "Setting TX power to: %d", dbm * FW_TX_POWER_LEVEL);

	return slsi_set_mib(sdev, dev, SLSI_PSID_UNIFI_USER_SET_TXPOWER_LEVEL, dbm * FW_TX_POWER_LEVEL);
}

int slsi_get_tx_power(void *priv)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev = NULL;
	int mib_value = 0;
	int res = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}
	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	res = slsi_get_mib(sdev, SLSI_PSID_UNIFI_USER_SET_TXPOWER_LEVEL, &mib_value);
	if (res) {
		return res;
	}

	return mib_value / FW_TX_POWER_LEVEL;
}

int slsi_del_station(void *priv, const u8 *addr, int reason)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	int r = 0;
	const u8 *mac = addr;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_T20_80211, SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(mac));

	if (!ndev_vif->activated) {
		goto exit;
	}

	if (FAPI_VIFTYPE_AP != ndev_vif->vif_type) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Invalid Device Type: %d\n", ndev_vif->iftype);
		r = -EINVAL;
		goto exit;
	}
	/* MAC with NULL value will come in case of flushing VLANS . Ignore this. */
	if (!mac) {
		goto exit;
	} else if (slsi_is_broadcast_addr(mac)) {
		int i = 0;
		bool peer_connected = false;

		while (i < SLSI_PEER_INDEX_MAX) {
			peer = ndev_vif->peer_sta_record[i];
			if (peer && peer->valid) {
				slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DISCONNECTED);
				peer_connected = true;
			}
			++i;
		}
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "peer_connected: %d\n", peer_connected);
		if (peer_connected) {
			if (slsi_mlme_disconnect(sdev, dev, peer->address, WLAN_REASON_DEAUTH_LEAVING, true) != 0) {
				SLSI_NET_ERR(dev, "Disconnection for peermac=00:00:00:00:00:00 returned with CFM failure\n");
			}
			r = slsi_handle_disconnect(sdev, dev, peer->address, WLAN_REASON_DISASSOC_STA_HAS_LEFT, 1);
		}

		/* Note AP :: mlme_disconnect_request with broadcast mac address is
		 * not required. Other third party devices don't support this. Conclusively,
		 * BIP support is not present with AP
		 */
	} else {
		peer = slsi_get_peer_from_mac(sdev, dev, mac);
		if (peer) {				/* To handle race condition when disconnect_req is sent before procedure_strted_ind and before mlme-connected_ind */
			if (peer->connected_state == SLSI_STA_CONN_STATE_CONNECTING) {
				SLSI_NET_DBG1(dev, SLSI_T20_80211, "SLSI_STA_CONN_STATE_CONNECTING : mlme-disconnect-req dropped at driver\n");
				goto exit;
			}
			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DISCONNECTED);

			if (reason == 0) {	/* sta_remove ops doesn't send the reason */
				reason = WLAN_REASON_DEAUTH_LEAVING;
			}

			r = slsi_mlme_disconnect(sdev, dev, peer->address, reason, true);
			if (r != 0) {
				SLSI_NET_ERR(dev, "Disconnection returned with failure\n");
			}
			/* Even if we fail to disconnect cleanly, tidy up. */
			r = slsi_handle_disconnect(sdev, dev, peer->address, WLAN_REASON_DISASSOC_STA_HAS_LEFT, 1);
		}
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

#ifdef CONFIG_SLSI_WLAN_P2P
int slsi_add_virtual_intf(void *priv, enum wpa_driver_if_type type, const char *ifname, const u8 *addr, void *bss_ctx, void **drv_priv, char *force_ifname, u8 *if_addr, const char *bridge, int use_existing)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	SLSI_DBG1(sdev, SLSI_T20_80211, "name:%s, iftype: %d\n", ifname, type);
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX);
	if (dev == NULL) {
		goto exit_with_error;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	switch (type) {
	case WPA_IF_P2P_GO:
		ndev_vif->iftype = SLSI_WLAN_IFTYPE_P2P_GO;
		break;
	case WPA_IF_P2P_CLIENT:
		ndev_vif->iftype = SLSI_WLAN_IFTYPE_P2P_CLIENT;
		break;
	case WPA_IF_P2P_GROUP:
		ndev_vif->iftype = SLSI_WLAN_IFTYPE_P2P_GROUP;
		break;
	default:
		SLSI_ERR_NODEV("Unknown iftype on wl3 p2p interface: %d\n", type);
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		goto exit_with_error;
	}
	SLSI_ETHER_COPY(dev->d_mac.ether_addr_octet, ndev_vif->sdev->netdev_addresses[SLSI_NET_INDEX_P2PX]);
	memcpy(dev->hwaddr, sdev->netdev_addresses[SLSI_NET_INDEX_P2PX], ETH_ALEN);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_ETHER_COPY(if_addr, dev->d_mac.ether_addr_octet);
	return 0;

exit_with_error:
	return -ENODEV;
}

int slsi_del_virtual_intf(void *priv, enum wpa_driver_if_type type, const char *ifname)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}
	ndev_vif = netdev_priv(dev);

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "\n");
	if (ndev_vif->is_registered) {
		slsi_stop_net_dev(sdev, dev);
	}
	netif_set_down(dev);
	return 0;
}

int slsi_remain_on_channel(void *priv, unsigned int freq, unsigned int duration)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int r = 0;
	union wpa_event_data data;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);
	memset(&data, 0, sizeof(data));
	data.remain_on_channel.freq = freq;
	data.remain_on_channel.duration = duration;

	if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
		SLSI_WARN(sdev, "device not started yet (device_state:%d)\n", sdev->device_state);
		goto exit_with_error;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "channel freq = %d, duration = %d\n", freq, duration);

	if (!SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		SLSI_NET_ERR(dev, "Invalid vif type\n");
		goto exit_with_error;
	}

	if (SLSI_IS_P2P_GROUP_STATE(sdev)) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "ROC requested in Group Role (%s) - Allow but do nothing\n", ((sdev->p2p_state == P2P_GROUP_FORMED_GO) ? "P2P GO" : "P2P CLI"));

		wpa_supplicant_event_send(ndev_vif->ctx, EVENT_REMAIN_ON_CHANNEL, &data);
		wpa_supplicant_event_send(ndev_vif->ctx, EVENT_CANCEL_REMAIN_ON_CHANNEL, &data);
		goto exit;
	}

	/* Unsync vif will be required, cancel any pending work of its deletion */
	if (slsi_is_work_pending(&ndev_vif->unsync.del_vif_work)) {
		work_cancel(SCSC_WORK, &ndev_vif->unsync.del_vif_work);
	}

	/* Ideally, there should not be any ROC work pending. However, supplicant can send back to back ROC in a race scenario as below.
	 * If action frame is received while P2P social scan, the response frame tx is delayed till scan completes. After scan completion,
	 * frame tx is done and ROC is started. Upon frame tx status, supplicant sends another ROC without cancelling the previous one.
	 */
	if (slsi_is_work_pending(&ndev_vif->unsync.roc_expiry_work)) {
		work_cancel(SCSC_WORK, &ndev_vif->unsync.roc_expiry_work);
	}

	/* If action frame tx is in progress and ROC comes, then it would mean action frame tx was done in ROC and
	 * frame tx ind is awaited, don't change state. Also allow back to back ROC in case it comes.
	 */
	if ((sdev->p2p_state == P2P_ACTION_FRAME_TX_RX) || (sdev->p2p_state == P2P_LISTENING)) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Allow ROC requested in %s state\n", slsi_p2p_state_text(sdev->p2p_state));
		goto exit_with_roc;
	}

	/* Unsync vif activation: Possible P2P state at this point is P2P_IDLE_NO_VIF or P2P_IDLE_VIF_ACTIVE */
	if (sdev->p2p_state == P2P_IDLE_NO_VIF) {
		if (slsi_p2p_vif_activate(sdev, dev, freq, duration, false) != 0) {
			goto exit_with_error;
		}
	} else if (sdev->p2p_state == P2P_IDLE_VIF_ACTIVE) {
		/* Configure Probe Response IEs in firmware if they have changed */
		if (ndev_vif->unsync.ies_changed) {
			u16 purpose = FAPI_PURPOSE_PROBE_RESPONSE;

			if (slsi_mlme_add_info_elements(sdev, dev, purpose, ndev_vif->unsync.probe_rsp_ies, ndev_vif->unsync.probe_rsp_ies_len) != 0) {
				SLSI_NET_ERR(dev, "Probe Rsp IEs setting failed\n");
				goto exit_with_vif;
			}
			ndev_vif->unsync.ies_changed = false;
		}
		/* Channel Setting - Don't set if already on same channel */
		if (slsi_mlme_set_channel(sdev, dev, freq, SLSI_FW_CHANNEL_DURATION_UNSPECIFIED, 0, 0) != 0) {
			SLSI_NET_ERR(dev, "Channel setting failed\n");
			goto exit_with_vif;
		} else {
			ndev_vif->center_freq = freq;
		}
	} else {
		SLSI_NET_ERR(dev, "Driver in incorrect P2P state (%s)", slsi_p2p_state_text(sdev->p2p_state));
		goto exit_with_error;
	}

	SLSI_P2P_STATE_CHANGE(sdev, P2P_LISTENING);

exit_with_roc:
	work_queue(SCSC_WORK, &ndev_vif->unsync.roc_expiry_work, slsi_p2p_roc_duration_expiry_work, (FAR void *)ndev_vif, MSEC2TICK(duration + SLSI_P2P_ROC_EXTRA_MSEC));

	wpa_supplicant_event_send(ndev_vif->ctx, EVENT_REMAIN_ON_CHANNEL, &data);

	goto exit;

exit_with_vif:
	slsi_p2p_vif_deactivate(sdev, dev, true);
exit_with_error:
	r = -EINVAL;
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return r;
}

int slsi_cancel_remain_on_channel(void *priv)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int r = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (!SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		SLSI_NET_ERR(dev, "Invalid vif type\n");
		r = -EINVAL;
		goto exit;
	}

	if (!((sdev->p2p_state == P2P_LISTENING) || (sdev->p2p_state == P2P_ACTION_FRAME_TX_RX))) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Driver not in listen - Ignore Cancel ROC\n");
		goto exit;
	}

	if (sdev->p2p_state == P2P_ACTION_FRAME_TX_RX && ndev_vif->mgmt_tx_data.exp_frame != SLSI_P2P_PA_INVALID) {
		/* Reset the expected action frame as procedure got completed */
		SLSI_INFO(sdev, "Action frame (%s) was not received\n", slsi_p2p_pa_subtype_text(ndev_vif->mgmt_tx_data.exp_frame));
		ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
	}

	if (slsi_is_work_pending(&ndev_vif->unsync.roc_expiry_work)) {
		work_cancel(SCSC_WORK, &ndev_vif->unsync.roc_expiry_work);
	}

	/* Supplicant has stopped FIND/LISTEN. Clear Probe Response IEs in firmware and driver */
	if (slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_RESPONSE, NULL, 0) != 0) {
		SLSI_NET_ERR(dev, "Clearing Probe Response IEs failed for unsync vif\n");
	}
	slsi_unsync_vif_set_probe_rsp_ie(ndev_vif, NULL, 0);
	wpa_supplicant_event_send(ndev_vif->ctx, EVENT_CANCEL_REMAIN_ON_CHANNEL, NULL);
	ndev_vif->center_freq = 0;
	/* Queue work to delete unsync vif */
	slsi_p2p_queue_unsync_vif_del_work(ndev_vif, SLSI_P2P_UNSYNC_VIF_EXTRA_MSEC);
	SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_VIF_ACTIVE);

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

static int slsi_p2p_group_mgmt_tx(struct slsi_dev *sdev, struct ieee80211_mgmt *mgmt, struct netif *dev, unsigned int freq, unsigned int wait, const u8 *buf, size_t len)
{
	struct netdev_vif *ndev_vif;
	int subtype = slsi_p2p_get_public_action_subtype(mgmt);
	int r = 0;
	u32 host_tag = slsi_tx_host_tag(sdev);
	u32 dwell_time = SLSI_FORCE_SCHD_ACT_FRAME_MSEC;
	u16 data_unit_desc = FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME;

	dev = sdev->netdev[SLSI_NET_INDEX_P2PX];
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);
	if (WARN_ON(sdev->p2p_group_exp_frame != SLSI_P2P_PA_INVALID)) {
		return -EINVAL;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	if (!((ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GO) || (ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_CLIENT))) {
		goto exit_with_error;
	}

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "Sending Action frame (%s) on p2p group vif (%d)\n", slsi_p2p_pa_subtype_text(subtype), ndev_vif->activated);

	if (ndev_vif->center_freq != freq) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Off-channel TX: channel freq = %d, dwell time = %dms\n", freq, wait);
		freq = SLSI_FREQ_HOST_TO_FW(freq);
		dwell_time = wait;
	}

	sdev->p2p_group_exp_frame = slsi_p2p_get_exp_peer_frame_subtype(subtype);
	SLSI_NET_DBG1(dev, SLSI_T20_80211, "Expected action frame is (%s)\n", slsi_p2p_pa_subtype_text(sdev->p2p_group_exp_frame));

	r = slsi_mlme_send_frame_mgmt(sdev, dev, buf, len, data_unit_desc, FAPI_MESSAGETYPE_IEEE80211_ACTION, host_tag, freq, dwell_time * 1000, 0);
	if (r) {
		goto exit_with_error;
	}
	r = slsi_set_mgmt_tx_data(ndev_vif, host_tag, buf, len);	/* If error then it is returned in exit */
	goto exit_with_lock;

exit_with_error:
	r = -EINVAL;
exit_with_lock:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

/* Handle mgmt_tx callback for P2P modes */
static int slsi_p2p_mgmt_tx(struct slsi_dev *sdev, struct ieee80211_mgmt *mgmt, struct netif *dev, struct netdev_vif *ndev_vif, unsigned int freq, unsigned int wait, const u8 *buf, size_t len)
{
	int ret = 0;
	union wpa_event_data event;

	if (SLSI_IS_MGMT_FRAME_ACTION(mgmt->frame_control)) {
		u16 host_tag = slsi_tx_host_tag(sdev);
		int subtype = slsi_p2p_get_public_action_subtype(mgmt);
		u8 exp_peer_frame;
		u32 dwell_time = 0;

		SLSI_NET_DBG2(dev, SLSI_T20_80211, "Action frame (%s), unsync_vif_active (%d)\n", slsi_p2p_pa_subtype_text(subtype), ndev_vif->activated);

		if (subtype == SLSI_P2P_PA_INVALID) {
			SLSI_NET_ERR(dev, "Invalid Action frame subtype\n");
			goto exit_with_error;
		}

		/* Check if unsync vif is available */
		if (sdev->p2p_state == P2P_IDLE_NO_VIF)
			if (slsi_p2p_vif_activate(sdev, dev, freq, wait, false) != 0) {
				goto exit_with_error;
			}

		/* Vif might be present but frame tx could be requested on a different channel */
		if (ndev_vif->center_freq != freq) {
			/* Clear Probe Response IEs if vif was already present with a different channel */
			if (slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_RESPONSE, NULL, 0) != 0) {
				SLSI_NET_ERR(dev, "Clearing Probe Response IEs failed for unsync vif\n");
				slsi_unsync_vif_set_probe_rsp_ie(ndev_vif, NULL, 0);
			}
			if (slsi_mlme_set_channel(sdev, dev, freq, SLSI_FW_CHANNEL_DURATION_UNSPECIFIED, 0, 0) != 0) {
				goto exit_with_vif;
			} else {
				ndev_vif->center_freq = freq;
			}
		}

		/* Check if peer frame response is expected */
		exp_peer_frame = slsi_p2p_get_exp_peer_frame_subtype(subtype);

		if (exp_peer_frame != SLSI_P2P_PA_INVALID) {
			if ((subtype == SLSI_P2P_PA_GO_NEG_RSP) && (slsi_p2p_get_go_neg_rsp_status(dev, (struct ieee80211_mgmt *)mgmt) != SLSI_P2P_STATUS_CODE_SUCCESS)) {
				SLSI_NET_DBG1(dev, SLSI_T20_80211, "GO_NEG_RSP Tx, peer response not expected\n");
				exp_peer_frame = SLSI_P2P_PA_INVALID;
			} else {
				SLSI_NET_DBG1(dev, SLSI_T20_80211, "Peer response expected with action frame (%s)\n", slsi_p2p_pa_subtype_text(exp_peer_frame));

				if (ndev_vif->mgmt_tx_data.exp_frame != SLSI_P2P_PA_INVALID) {
					(void)slsi_set_mgmt_tx_data(ndev_vif, 0, NULL, 0);
				}

				/* Change Force Schedule Duration as peer response is expected */
				dwell_time = SLSI_FORCE_SCHD_ACT_FRAME_MSEC;
			}
		}

		/* Send the action frame, transmission status indication would be received later */
		if (slsi_mlme_send_frame_mgmt(sdev, dev, buf, len, FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME, FAPI_MESSAGETYPE_IEEE80211_ACTION, host_tag, 0, dwell_time * 1000, 0) != 0) {
			goto exit_with_vif;
		}

		/* If multiple frames are requested for tx, only the info of first frame would be stored */
		if (ndev_vif->mgmt_tx_data.host_tag == 0) {
			unsigned int n_wait = 0;

			ret = slsi_set_mgmt_tx_data(ndev_vif, host_tag, buf, len);
			if (ret != 0) {
				goto exit_with_vif;
			}
			ndev_vif->mgmt_tx_data.exp_frame = exp_peer_frame;

			SLSI_P2P_STATE_CHANGE(sdev, P2P_ACTION_FRAME_TX_RX);
			if ((exp_peer_frame == SLSI_P2P_PA_GO_NEG_RSP) || (exp_peer_frame == SLSI_P2P_PA_GO_NEG_CFM))
				/* Retain vif for larger duration that wpa_supplicant asks to wait,
				 * during GO-Negotiation to allow peer to retry GO neg in bad radio condition.
				 * Some of phones retry GO-Negotiation after 2 seconds
				 */
			{
				n_wait = SLSI_P2P_NEG_PROC_UNSYNC_VIF_RETAIN_DURATION;
			} else if (exp_peer_frame != SLSI_P2P_PA_INVALID)
				/* If a peer response is expected queue work to retain vif till wait time else the work will be handled in mgmt_tx_cancel_wait */
			{
				n_wait = wait + SLSI_P2P_MGMT_TX_EXTRA_MSEC;
			}
			if (n_wait) {
				SLSI_NET_DBG2(dev, SLSI_T20_80211, "retain unsync vif for duration (%d) msec\n", n_wait);
				slsi_p2p_queue_unsync_vif_del_work(ndev_vif, n_wait);
			}
		} else {
			/* Already a frame Tx is in progress, send immediate tx_status as success. Sending immediate tx status should be ok
			 * as supplicant is in another procedure and so these frames would be mostly only response frames.
			 */
			WARN_ON(sdev->p2p_state != P2P_ACTION_FRAME_TX_RX);
			SLSI_NET_DBG1(dev, SLSI_T20_80211, "Received another frame Tx while already in Tx\n");
			memset(&event, 0, sizeof(event));
			event.tx_status.type = WLAN_FC_GET_TYPE(mgmt->frame_control);
			event.tx_status.stype = WLAN_FC_GET_STYPE(mgmt->frame_control);
			event.tx_status.dst = mgmt->da;
			event.tx_status.data = buf;
			event.tx_status.data_len = len;
			wpa_supplicant_event_send(ndev_vif->ctx, EVENT_TX_STATUS, &event);
		}
		goto exit;
	}

	/* Else send failure for unexpected management frame */
	SLSI_NET_ERR(dev, "Drop Tx frame: Unexpected Management frame\n");
	goto exit_with_error;

exit_with_vif:
	if (sdev->p2p_state != P2P_LISTENING) {
		slsi_p2p_vif_deactivate(sdev, dev, true);
	}
exit_with_error:
	ret = -EINVAL;
exit:
	return ret;
}

int slsi_mgmt_tx(void *priv, unsigned int freq, unsigned int wait, const u8 *dst, const u8 *src, const u8 *bssid, const u8 *data, size_t data_len, int no_cck)
{

	/* Note to explore for AP ::All public action frames which come to host should be handled properly
	 * Additionally, if PMF is negotiated over the link, the host shall not issue "mlme-send-frame.request"
	 * primitive  for action frames before the pairwise keys have been installed in F/W. Presently, for
	 * SoftAP with PMF support, there is no scenario in which slsi_mlme_send_frame will be called for
	 * action frames for VIF TYPE = AP.
	 */
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct ieee80211_mgmt *mgmt;
	int r = 0;
	union wpa_event_data event;
	u16 host_tag = 0;
	u8 *buf;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	buf = malloc(data_len + SLSI_WLAN_HEADER_LENGTH);
	memcpy(buf + SLSI_WLAN_HEADER_LENGTH, data, data_len);
	mgmt = (struct ieee80211_mgmt *)buf;
	mgmt->frame_control = SLSI_MGMT_SUB_TYPE_ACTION;
	memcpy(mgmt->da, dst, ETH_ALEN);
	memcpy(mgmt->sa, src, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	data_len = data_len + SLSI_WLAN_HEADER_LENGTH;

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	SLSI_NET_DBG2(dev, SLSI_T20_80211, "Mgmt Frame Tx: iface_num = %d, freq = %d, wait = %d\n", ndev_vif->ifnum, freq, wait);
	if (!(slsi_is_mgmt_frame(mgmt->frame_control))) {
		SLSI_NET_ERR(dev, "Drop Tx frame: Not a Management frame\n");
		r = -EINVAL;
		goto exit;
	}

	/*P2P */
	/* Drop Probe Responses which can come in P2P Device and P2P Group role */
	if (SLSI_IS_MGMT_FRAME_PROBE_RSP(mgmt->frame_control)) {
		SLSI_NET_DBG3(dev, SLSI_T20_80211, "Drop Probe Response from supplicant");
		/* Ideally supplicant doesn't expect Tx status for Probe Rsp. Send tx status just in case it requests ack */
		memset(&event, 0, sizeof(event));
		event.tx_status.type = WLAN_FC_GET_TYPE(mgmt->frame_control);
		event.tx_status.stype = WLAN_FC_GET_STYPE(mgmt->frame_control);
		event.tx_status.dst = mgmt->da;
		event.tx_status.data = buf;
		event.tx_status.data_len = data_len;
		event.tx_status.ack = no_cck;
		wpa_supplicant_event_send(ndev_vif->ctx, EVENT_TX_STATUS, &event);
		goto exit;
	}

	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		/* Check whether STA scan is running or not. If yes, then abort the STA scan */
		/*TODO:  Do we need this as only 1 interface can be active */
		//slsi_abort_sta_scan(sdev);
		if (SLSI_IS_P2P_GROUP_STATE(sdev)) {
			r = slsi_p2p_group_mgmt_tx(sdev, mgmt, dev, freq, wait, buf, data_len);
		} else {
			r = slsi_p2p_mgmt_tx(sdev, mgmt, dev, ndev_vif, freq, wait, buf, data_len);
		}
	} else if (ndev_vif->ifnum == SLSI_NET_INDEX_P2PX) {
		if (freq == ndev_vif->center_freq) {
			host_tag = slsi_tx_host_tag(sdev);
			SLSI_NET_DBG2(dev, SLSI_T20_80211, "Mgmt TX received on group vif\n");
			r = slsi_mlme_send_frame_mgmt(sdev, dev, buf, data_len, FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME, FAPI_MESSAGETYPE_IEEE80211_ACTION, host_tag, 0, 0, 0);
			if (r) {
				SLSI_NET_ERR(dev, "Failed to send action frame, r = %d\n", r);
				goto exit;
			}

			r = slsi_set_mgmt_tx_data(ndev_vif, host_tag, buf, data_len);
		}
	}
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return r;
}

void slsi_mgmt_tx_cancel_wait(void *priv)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return;
	}

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "iface_num = %d\n", ndev_vif->ifnum);

	/* If device was in frame tx_rx state, clear mgmt tx data and change state */
	if (sdev->p2p_state == P2P_ACTION_FRAME_TX_RX) {

		if (ndev_vif->mgmt_tx_data.exp_frame != SLSI_P2P_PA_INVALID) {
			(void)slsi_mlme_reset_dwell_time(sdev, dev);
		}

		(void)slsi_set_mgmt_tx_data(ndev_vif, 0, NULL, 0);
		ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;

		if (slsi_is_work_pending(&ndev_vif->unsync.roc_expiry_work)) {
			SLSI_P2P_STATE_CHANGE(sdev, P2P_LISTENING);
		} else {
			slsi_p2p_queue_unsync_vif_del_work(ndev_vif, SLSI_P2P_UNSYNC_VIF_EXTRA_MSEC);
			SLSI_P2P_STATE_CHANGE(ndev_vif->sdev, P2P_IDLE_VIF_ACTIVE);
		}
	} else if ((SLSI_IS_P2P_GROUP_STATE(sdev)) && (sdev->p2p_group_exp_frame != SLSI_P2P_PA_INVALID)) {
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "Reset P2P group action frame data\n");
		/* acquire mutex lock if it is not group net dev */
		slsi_clear_offchannel_data(sdev, (ndev_vif->ifnum != SLSI_NET_INDEX_P2PX) ? true : false);
	}

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
}

int slsi_set_p2p_powersave(void *priv, int legacy_ps, int opp_ps, int ctwindow)
{
	int result = 0;
#ifdef CONFIG_SCSC_ADV_FEATURE
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_mib_data mib_data = { 0, NULL };
	unsigned int ct_param = 0;
	int error = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	/* The NOA param shall be added only after P2P-VIF is active */
	if ((!ndev_vif->activated) || (ndev_vif->iftype != SLSI_WLAN_IFTYPE_P2P_GO)) {
		SLSI_ERR_NODEV("P2P GO vif not activated\n");
		result = -EINVAL;
		goto exit;
	}
	if (ct_param == 1) {
		SLSI_WARN(sdev, "p2p oppps = %d, Ignore this command as p2p oppps already active\n", ct_param);
		goto exit;
	} else if (ct_param < (unsigned int)ndev_vif->ap.beacon_interval) {
		SLSI_DBG1(sdev, SLSI_T20_80211, "p2p ct window = %d\n", ct_param);
		error = slsi_mib_encode_uint(&mib_data, SLSI_PSID_UNIFI_CT_WINDOW, ct_param, 0);
	} else {
		SLSI_DBG1(sdev, SLSI_T20_80211, "p2p ct window = %d is out of range for beacon interval(%d)\n", ct_param, ndev_vif->ap.beacon_interval);
		goto exit;
	}
	if ((error != SLSI_MIB_STATUS_SUCCESS) || (mib_data.dataLength == 0)) {
		result = -ENOMEM;
		goto exit;
	}

	result = slsi_mlme_set(sdev, dev, mib_data.data, mib_data.dataLength);
	kmm_free(mib_data.data);

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
#endif
	return result;

}

static ssize_t slsi_p2p_go_vendor_ies_write(struct slsi_dev *sdev, struct netif *dev, u8 *ie, size_t ie_len, u16 purpose)
{
	u8 *go_vendor_ie = NULL;
	int result = 0;
	struct netdev_vif *ndev_vif;

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	/* During AP start before mlme_start_req, supplicant calls set_ap_wps_ie() to send the vendor IEs for each
	 * beacon, probe response and association response. As we get all of them in mlme_start_req, ignoring the
	 * same which comes before adding GO VIF
	 */
	if (!ndev_vif->activated) {
		SLSI_DBG1(sdev, SLSI_T20_80211, "P2P GO vif not activated\n");
		result = 0;
		goto exit;
	}

	go_vendor_ie = kmm_malloc(ie_len);
	if (go_vendor_ie == NULL) {
		SLSI_ERR(sdev, "kmalloc failed\n");
		result = -ENOMEM;
		goto exit;
	}
	memcpy(go_vendor_ie, ie, ie_len);

	slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	result = slsi_ap_prepare_add_info_ies(ndev_vif, go_vendor_ie, ie_len);

	if (result == 0) {
		result = slsi_mlme_add_info_elements(sdev, dev, purpose, ndev_vif->ap.add_info_ies, ndev_vif->ap.add_info_ies_len);
	}

	slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	kmm_free(go_vendor_ie);

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return result;
}

int slsi_set_ap_wps_ie(void *priv, const struct wpabuf *beacon, const struct wpabuf *proberesp, const struct wpabuf *assocresp)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int r = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	if (ndev_vif->ifnum == SLSI_NET_INDEX_WLAN) {
		return 0;
	}

	/* check the net device interface type */

	SLSI_NET_DBG2(dev, SLSI_NETDEV, "slsi_set_ap_wps_ie\n");

	/* check the net device interface type */
	if ((ndev_vif->ifnum == SLSI_NET_INDEX_P2P) && (proberesp)) {
		u8 *probe_resp_ie = NULL;
		probe_resp_ie = kmm_malloc(proberesp->used);
		if (probe_resp_ie == NULL) {
			SLSI_ERR(sdev, "Malloc for IEs failed size = %d \n", proberesp->used);
			return -ENOMEM;
		}
		memcpy(probe_resp_ie, proberesp->buf, proberesp->used);
		SLSI_NET_DBG2(dev, SLSI_NETDEV, "P2P Device: probe_resp_ie is NOT NULL\n");
		return slsi_p2p_dev_probe_rsp_ie(sdev, dev, probe_resp_ie, proberesp->size);
	} else if (ndev_vif->ifnum == SLSI_NET_INDEX_P2PX) {
		SLSI_NET_DBG2(dev, SLSI_NETDEV, "P2P GO case");

		if (beacon) {
			SLSI_DBG1(sdev, SLSI_MLME, "P2P GO beacon IEs update\n");
			r = slsi_p2p_go_vendor_ies_write(sdev, dev, beacon->buf, beacon->size, FAPI_PURPOSE_BEACON);
			if (r != 0) {
				goto exit;
			}
		}
		if (proberesp) {
			SLSI_DBG1(sdev, SLSI_MLME, "P2P GO proberesp IEs update\n");
			r = slsi_p2p_go_vendor_ies_write(sdev, dev, proberesp->buf, proberesp->size, FAPI_PURPOSE_PROBE_RESPONSE);
			if (r != 0) {
				goto exit;
			}
		}

		if (assocresp) {
			SLSI_DBG1(sdev, SLSI_MLME, "P2P GO Association Response IEs update\n");
			r = slsi_p2p_go_vendor_ies_write(sdev, dev, assocresp->buf, assocresp->size, FAPI_PURPOSE_ASSOCIATION_RESPONSE);
		}
	}
exit:

	return r;
}
#endif							/*CONFIG_SLSI_WLAN_P2P */

#ifdef CONFIG_SCSC_ADV_FEATURE
static void slsi_ap_start_obss_scan(struct slsi_dev *sdev, struct netif *dev, struct netdev_vif *ndev_vif)
{
	struct cfg80211_ssid ssids;
	struct ieee80211_channel *channel;
	int n_ssids = 1, n_channels = 1, i;

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "Perform scan before starting 20 MHz HT AP on channel %u\n", ndev_vif->chan->hw_value);

	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

	ssids.ssid_len = 0;
	for (i = 0; i < SLSI_WLAN_MAX_SSID_LEN; i++) {
		ssids.ssid[i] = 0x00;    /* Broadcast SSID */
	}

	channel = ieee80211_get_channel(sdev->wiphy, ndev_vif->chan->center_freq);

	(void)slsi_mlme_add_scan(sdev, dev, FAPI_SCANTYPE_OBSS_SCAN, FAPI_REPORTMODE_REAL_TIME, n_ssids, &ssids, n_channels, &channel, NULL, NULL,	/* No IEs */
							 0, true /* Wait for scan_done_ind */);

	slsi_ap_obss_scan_done_ind(dev, ndev_vif);
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

int slsi_ap_start_validate(struct netdev_vif *ndev_vif, struct slsi_dev *sdev, struct wpa_driver_ap_params *settings)
{
#ifdef CONFIG_SLSI_WLAN_P2P
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		SLSI_ERR(sdev, "AP start requested on incorrect vif\n");
		goto exit_with_error;
	}
#endif
	if (ndev_vif->activated) {
		if (SLSI_IS_VIF_INDEX_WLAN(ndev_vif)) {
			WARN_ON(1);
			goto exit_with_error;
		} else {
			/* In P2P GO scenario StartAP is called multiple times from supplicant when there is a change in the beacon content.
			   It is taken care in Add info elements.
			 */
			return EBUSY;
		}
	}

	if (!settings->ssid_len || !settings->ssid) {
		SLSI_ERR(sdev, "SSID not provided\n");
		goto exit_with_error;
	}

	if (!((settings->freq->mode == HOSTAPD_MODE_IEEE80211B)
		  || (settings->freq->mode == HOSTAPD_MODE_IEEE80211G)
		  || (settings->freq->mode == HOSTAPD_MODE_IEEE80211A))) {
		goto exit_with_error;
	}

	if (!((settings->freq->bandwidth == SLSI_CHANNEL_BANDWIDTH_20MHZ)
		  || (settings->freq->bandwidth == SLSI_CHANNEL_BANDWIDTH_40MHZ))) {
		goto exit_with_error;
	}

	if ((settings->freq->channel <= 14)
		&& (settings->freq->bandwidth == SLSI_CHANNEL_BANDWIDTH_40MHZ)) {
		SLSI_ERR(sdev, "Configuration error: 40 MHz on 2.4 GHz is not supported. Channel_no: %d Channel_width: %d\n", settings->freq->channel, slsi_get_chann_info(sdev, settings->freq));
		goto exit_with_error;
	}

	if (ndev_vif->chandef == NULL) {
		ndev_vif->chandef = kmm_malloc(sizeof(struct hostapd_freq_params));
		if (ndev_vif->chandef == NULL) {
			SLSI_ERR(sdev, "chandef alloc failed\n");
			goto exit_with_error;
		}
		memcpy(ndev_vif->chandef, settings->freq, sizeof(struct hostapd_freq_params));
	}

	return 0;

exit_with_error:
	return -EINVAL;
}

int slsi_start_ap(void *priv, struct wpa_driver_ap_params *settings)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	u8 device_address[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int r = 0;
	const u8 *wpa_ie_pos = NULL;
	size_t wpa_ie_len = 0;
	const u8 *wmm_ie_pos = NULL;
	size_t wmm_ie_len = 0;
	const u8 *country_ie = NULL;
	char alpha2[SLSI_COUNTRY_CODE_LEN];
	bool append_vht_ies = false;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);
	if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
		SLSI_WARN(sdev, "device not started yet (device_state:%d)\n", sdev->device_state);
		r = -EINVAL;
		goto exit_with_start_stop_mutex;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "iftype: %d\n", ndev_vif->iftype);

	r = slsi_ap_start_validate(ndev_vif, sdev, settings);
	if (r != 0) {
		/* In P2P GO scenario StartAP is called multiple times from supplicant when there is a change in the beacon content. It is taken care in Add info elements.
		   When this function is called to set the beacon params after AP start,  slsi_ap_start_validate will return EBUSY error code.
		 */
		if (r == EBUSY) {
			r = 0;
		}
		goto exit_with_vif_mutex;
	}
	memset(&ndev_vif->ap, 0, sizeof(ndev_vif->ap));
	/* Initialise all allocated peer structures to remove old data. */
	/*slsi_netif_init_all_peers(sdev, dev); */

	/* Reg domain changes */
	country_ie = slsi_wlan_find_ie(SLSI_WLAN_EID_COUNTRY, settings->tail, settings->tail_len);
	if (country_ie) {
		country_ie += 2;
		memcpy(alpha2, country_ie, SLSI_COUNTRY_CODE_LEN);
		if (memcmp(sdev->device_config.domain_info->alpha2, alpha2, SLSI_COUNTRY_CODE_LEN - 1) != 0) {
			if (slsi_set_country_update_regd(sdev, alpha2, SLSI_COUNTRY_CODE_LEN) != 0) {
				r = -EINVAL;
				goto exit_with_vif_mutex;
			}
#ifdef CONFIG_SCSC_ADV_FEATURE
			if (slsi_check_channelization(sdev, ndev_vif->chandef) != 0) {
				r = -EINVAL;
				goto exit_with_vif_mutex;
			}
#endif
		}
	}
#ifdef CONFIG_SLSI_WLAN_P2P
	/* For P2P iftype is initialized in slsi_add_virtual_intf(). For negotiated group the iftype will be SLSI_WLAN_IFTYPE_P2P_GROUP */
	if ((ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GO) || (ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GROUP)) {
		slsi_p2p_group_start_remove_unsync_vif(sdev);
		SLSI_ETHER_COPY(device_address, sdev->netdev_addresses[SLSI_NET_INDEX_P2P]);
		if (keep_alive_period != SLSI_P2PGO_KEEP_ALIVE_PERIOD_SEC)
			if (slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_GO_KEEP_ALIVE_PERIOD, keep_alive_period) != 0) {
				SLSI_NET_ERR(dev, "P2PGO Keep Alive MIB set failed");
				r = -EINVAL;
				goto exit_with_vif_mutex;
			}
#ifdef CONFIG_SLSI_WLAN_P2P_VHT
		/* Default 11ac configuration */
		if (!(slsi_wlan_find_ie(SLSI_WLAN_EID_VHT_CAPAB, settings->tail, settings->tail_len)) && (ndev_vif->chandef->chan->hw_value >= 36)) {
			u16 oper_chan = ndev_vif->chandef->chan->hw_value;

			ndev_vif->chandef->width = CHAN_WIDTH_80;
			if ((oper_chan >= 36) && (oper_chan <= 48)) {
				ndev_vif->chandef->center_freq1 = ieee80211_channel_to_frequency(42, SLSI_WLAN_BAND_5GHZ);
			} else if ((oper_chan >= 149) && (oper_chan <= 161)) {
				ndev_vif->chandef->center_freq1 = ieee80211_channel_to_frequency(155, SLSI_WLAN_BAND_5GHZ);
			}
			if (cfg80211_chandef_valid(ndev_vif->chandef)) {
				u8 *ht_operation_ie;
				u8 sec_chan_offset = 0;

				append_vht_ies = true;
				ht_operation_ie = (u8 *) slsi_wlan_find_ie(WLAN_EID_HT_OPERATION, settings->beacon.tail, settings->beacon.tail_len);
				if (!ht_operation_ie) {
					SLSI_NET_ERR(dev, "HT Operation IE is not passed by wpa_supplicant");
					r = -EINVAL;
					goto exit_with_vif_mutex;
				}

				if (oper_chan == 36 || oper_chan == 44 || oper_chan == 149 || oper_chan == 157) {
					sec_chan_offset = SLSI_WLAN_HT_OPER_SEC_CHAN_ABOVE;
				} else {
					sec_chan_offset = SLSI_WLAN_HT_OPER_SEC_CHAN_BELOW;
				}
				/* Change HT Information IE subset 1 */
				ht_operation_ie += 3;
				*(ht_operation_ie) |= sec_chan_offset;
				*(ht_operation_ie) |= SLSI_WLAN_HT_OPER_CHAN_WIDTH_ANY;
			}
		}
		/* End */
		/* Default HT40 configuration */
		if ((ndev_vif->chandef->chan->hw_value >= 36) && (ndev_vif->chandef->width == SLSI_80211_CHAN_WIDTH_20)) {
			u16 oper_chan = ndev_vif->chandef->chan->hw_value;
			u8 ht_info_value = 0;
			u8 bw_40_minus_channels[4] = { 40, 48, 153, 161 };
			u8 ch;
			bool offset_above = true;

			ndev_vif->chandef->width = SLSI_80211_CHAN_WIDTH_40;

			for (ch = 0; ch < 4; ch++)
				if (oper_chan == bw_40_minus_channels[ch]) {
					ht_info_value = SLSI_WLAN_HT_OPER_CHAN_WIDTH_ANY | SLSI_WLAN_HT_OPER_SEC_CHAN_BELOW;
					ndev_vif->chandef->center_freq1 = ndev_vif->chandef->chan->center_freq - 10;
					offset_above = false;
					break;
				}

			if (offset_above) {
				ht_info_value = SLSI_WLAN_HT_OPER_CHAN_WIDTH_ANY | SLSI_WLAN_HT_OPER_SEC_CHAN_ABOVE;
				ndev_vif->chandef->center_freq1 = ndev_vif->chandef->chan->center_freq + 10;
			}

			if (cfg80211_chandef_valid(ndev_vif->chandef)) {
				slsi_modify_ies(dev, WLAN_EID_HT_OPERATION, (u8 *) settings->beacon_ies->buf, settings->beacon_ies->size, 3, ht_info_value);
				slsi_modify_ies(dev, SLSI_WLAN_EID_HT_CAPAB, (u8 *) settings->beacon_ies->buf, settings->beacon_ies->size, 2, SLSI_HT_CAPAB_SUP_WIDTH_20_40);
			} else {
				SLSI_NET_WARN(dev, "chandef is not valid");
			}
		}
#endif
	} else
#endif
	{							/* Legacy AP */
#ifdef CONFIG_SCSC_ADV_FEATURE
		if (ndev_vif->chandef->bandwidth == 20) {
			slsi_ap_start_obss_scan(sdev, dev, ndev_vif);
		}
#endif

		/* Enable SGI by default */
		if (slsi_wlan_find_ie(SLSI_WLAN_EID_HT_CAPAB, settings->tail, settings->tail_len)) {
			u8 mod_value = SLSI_WLAN_HT_CAPAB_SGI_20;

			if (ndev_vif->chandef->bandwidth == 40) {
				mod_value |= SLSI_WLAN_HT_CAPAB_SGI_40;
			}

			slsi_modify_ies(dev, SLSI_WLAN_EID_HT_CAPAB, (u8 *) settings->tail, settings->tail_len, 2, mod_value);
		}
#ifdef CONFIG_SCSC_ADV_FEATURE
		if (slsi_wlan_find_ie(SLSI_WLAN_EID_VHT_CAPAB, settings->tail, settings->tail_len)) {
			slsi_modify_ies(dev, SLSI_WLAN_EID_VHT_CAPAB, (u8 *) settings->tail, settings->tail_len, 2, SLSI_80211_VHT_CAP_SHORT_GI_80);
		}
#endif
	}

	ndev_vif->vif_type = FAPI_VIFTYPE_AP;
	if (slsi_mlme_add_vif(sdev, dev, dev->d_mac.ether_addr_octet, device_address) != 0) {
		SLSI_NET_ERR(dev, "slsi_mlme_add_vif failed\n");
		r = -EINVAL;
		goto exit_with_vif_mutex;
	}

	if (slsi_vif_activated(sdev, dev) != 0) {
		SLSI_NET_ERR(dev, "slsi_vif_activated failed\n");
		goto exit_with_vif;
	}

	/* Extract the WMM and WPA IEs from settings->beacon.tail - This is sent in add_info_elements and shouldn't be included in start_req
	 * Cache IEs to be used in later add_info_elements_req. The IEs would be freed during AP stop
	 */
	wpa_ie_pos = slsi_wlan_find_vendor_spec_ie(SLSI_WLAN_VS_OUI_MICROSOFT, SLSI_WLAN_VS_OUI_TYPE_MS_WPA, settings->tail, settings->tail_len);
	if (wpa_ie_pos) {
		wpa_ie_len = *(wpa_ie_pos + 1) + 2;	/* For 0xdd (1) and Tag Length (1) */
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "WPA IE found: Length = %zu\n", wpa_ie_len);
		SLSI_EC_GOTO(slsi_cache_ies(wpa_ie_pos, wpa_ie_len, &ndev_vif->ap.cache_wpa_ie, &ndev_vif->ap.wpa_ie_len), r, exit_with_vif);
	}

	wmm_ie_pos = slsi_wlan_find_vendor_spec_ie(SLSI_WLAN_VS_OUI_MICROSOFT, SLSI_WLAN_VS_OUI_TYPE_MS_WMM, settings->tail, settings->tail_len);
	if (wmm_ie_pos) {
		wmm_ie_len = *(wmm_ie_pos + 1) + 2;
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "WMM IE found: Length = %zu\n", wmm_ie_len);
		SLSI_EC_GOTO(slsi_cache_ies(wmm_ie_pos, wmm_ie_len, &ndev_vif->ap.cache_wmm_ie, &ndev_vif->ap.wmm_ie_len), r, exit_with_vif);
	}

	slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);

	/* Set Vendor specific IEs (WPA, WMM, WPS, P2P) for Beacon, Probe Response and Association Response
	 * The Beacon and Assoc Rsp IEs can include Extended Capability (WLAN_EID_EXT_CAPAB) IE when supported.
	 * Some other IEs (like internetworking, etc) can also come if supported.
	 * The add_info should include only vendor specific IEs and other IEs should be removed if supported in future.
	 */
	if ((wmm_ie_pos) || (wpa_ie_pos) || settings->beacon_ies) {
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "Add info elements for beacon\n");
		SLSI_EC_GOTO(slsi_ap_prepare_add_info_ies(ndev_vif, settings->beacon_ies->buf, settings->beacon_ies->used), r, exit_with_vif);
		SLSI_EC_GOTO(slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_BEACON, ndev_vif->ap.add_info_ies, ndev_vif->ap.add_info_ies_len), r, exit_with_vif);
		slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	}

	if ((wmm_ie_pos) || (wpa_ie_pos) || settings->proberesp_ies) {
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "Add info elements for probe response\n");
		SLSI_EC_GOTO(slsi_ap_prepare_add_info_ies(ndev_vif, settings->proberesp_ies->buf, settings->proberesp_ies->used), r, exit_with_vif);
		SLSI_EC_GOTO(slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_RESPONSE, ndev_vif->ap.add_info_ies, ndev_vif->ap.add_info_ies_len), r, exit_with_vif);
		slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	}

	if ((wmm_ie_pos) || (wpa_ie_pos) || settings->assocresp_ies) {
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "Add info elements for assoc response\n");
		SLSI_EC_GOTO(slsi_ap_prepare_add_info_ies(ndev_vif, settings->assocresp_ies->buf, settings->assocresp_ies->used), r, exit_with_vif);
		SLSI_EC_GOTO(slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_ASSOCIATION_RESPONSE, ndev_vif->ap.add_info_ies, ndev_vif->ap.add_info_ies_len), r, exit_with_vif);
		slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	}
#ifdef CONFIG_SLSI_WLAN_P2P
	if (ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GO) {
		u32 af_bmap_active = SLSI_ACTION_FRAME_PUBLIC;
		u32 af_bmap_suspended = SLSI_ACTION_FRAME_PUBLIC;

		r = slsi_mlme_register_action_frame(sdev, dev, af_bmap_active, af_bmap_suspended);
		if (r != 0) {
			SLSI_NET_ERR(dev, "slsi_mlme_register_action_frame failed: resultcode = %d\n", r);
			goto exit_with_vif;
		}
	}
#endif

	r = slsi_mlme_start(sdev, dev, dev->d_mac.ether_addr_octet, settings, wpa_ie_pos, wmm_ie_pos, append_vht_ies);
	if (r != 0) {
		SLSI_NET_ERR(dev, "Start ap failed: resultcode = %d\n", r);
		goto exit_with_vif;
#ifdef CONFIG_SLSI_WLAN_P2P
	} else if (ndev_vif->iftype == SLSI_WLAN_IFTYPE_P2P_GO) {
		SLSI_P2P_STATE_CHANGE(sdev, P2P_GROUP_FORMED_GO);
#endif
	}

	ndev_vif->ap.beacon_interval = settings->beacon_int;

	r = slsi_read_disconnect_ind_timeout(sdev, SLSI_PSID_UNIFI_DISCONNECT_TIMEOUT);
	if (r != 0) {
		sdev->device_config.ap_disconnect_ind_timeout = SLSI_SIG_WAIT_CFM_TIMEOUT + 2000;
	}

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "slsi_read_disconnect_ind_timeout: timeout = %d", sdev->device_config.ap_disconnect_ind_timeout);

	netif_set_link_up(dev);
	goto exit_with_vif_mutex;

exit_with_vif:
	slsi_clear_cached_ies(&ndev_vif->ap.add_info_ies, &ndev_vif->ap.add_info_ies_len);
	slsi_mlme_del_vif(sdev, dev);
	slsi_vif_deactivated(sdev, dev);
	r = -EINVAL;
exit_with_vif_mutex:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
exit_with_start_stop_mutex:
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return r;
}

int slsi_stop_ap(void *priv)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netdev_vif *ndev_vif;
	struct netif *dev;
	int r = 0;
	int i = 0;
	struct slsi_peer *peer;
	bool peer_connected = false;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "\n");

	if (!ndev_vif->activated) {
		r = -EINVAL;
		goto exit;
	}

	if (FAPI_VIFTYPE_AP != ndev_vif->vif_type) {
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Invalid Device Type: %d\n", ndev_vif->iftype);
		r = -EINVAL;
		goto exit;
	}
	while (i < SLSI_PEER_INDEX_MAX) {
		peer = ndev_vif->peer_sta_record[i];
		if (peer && peer->valid) {
			slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DISCONNECTED);
			peer_connected = true;
		}
		++i;
	}
	SLSI_NET_DBG1(dev, SLSI_T20_80211, "peer_connected: %d\n", peer_connected);
	if (peer_connected) {
		if (slsi_mlme_disconnect(sdev, dev, peer->address, WLAN_REASON_DEAUTH_LEAVING, true) != 0) {
			SLSI_NET_ERR(dev, "Disconnection for peermac=00:00:00:00:00:00 returned with CFM failure\n");
		}
		r = slsi_handle_disconnect(sdev, dev, peer->address, WLAN_REASON_DISASSOC_STA_HAS_LEFT, 1);
	}

	netif_set_link_down(dev);
	/* All STA related packets and info should already have been flushed */
	slsi_mlme_del_vif(sdev, dev);
	slsi_vif_deactivated(sdev, dev);
	ndev_vif->ipaddress = 0;

#ifdef CONFIG_SLSI_WLAN_P2P
	if (ndev_vif->ap.p2p_gc_keys_set) {
		ndev_vif->ap.p2p_gc_keys_set = false;
	}

#endif

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

int slsi_set_panic(void *priv)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	int r = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.force_panic);
	scsc_service_force_panic(sdev->service);

	return r;
}

int slsi_mlme_send_ap_eapol(void *priv, const u8 *addr, const u8 *data, size_t data_len, int encrypt, const u8 *own_addr, u32 flags)
{
	if (!priv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	UNUSED(encrypt);
	UNUSED(flags);

	eth_send_eapol(own_addr, addr, data, data_len, ETH_P_PAE);
	return 0;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
int slsi_set_mac_acl(struct wiphy *wiphy, struct net_device *dev, const struct cfg80211_acl_data *params)
{
	struct slsi_dev *sdev = SDEV_FROM_WIPHY(wiphy);
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_SET_ACL.request\n");
		return -EOPNOTSUPP;
	}
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (FAPI_VIFTYPE_AP != ndev_vif->vif_type) {
		SLSI_NET_ERR(dev, "Invalid vif type: %d\n", ndev_vif->vif_type);
		r = -EINVAL;
		goto exit;
	}
	SLSI_NET_DBG2(dev, SLSI_T20_80211, "ACL:: Policy: %d  Number of stations: %d\n", params->acl_policy, params->n_acl_entries);
	r = slsi_mlme_set_acl(sdev, dev, params);
	if (r != 0) {
		SLSI_NET_ERR(dev, "mlme_set_acl_req returned with CFM failure\n");
	}
exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

/*
 * Handler to update the drv_interface fields and the supplicant context in the
 * corresponding netdev_vif.
 */
static void slsi_update_supp_iface_fields(u16 ifnum, void *ctx, struct netif *dev, struct slsi_dev *sdev, struct slsi_t20_drv *drv)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	ndev_vif->ctx = ctx;
	drv->supp_iface[ifnum].dev = dev;
	drv->supp_iface[ifnum].sdev = sdev;
}

void *slsi_t20_init(void *ctx, const char *ifname, void *global_priv)
{
	struct slsi_t20_drv *drv;
	struct slsi_dev *sdev;
	struct netif *dev;
	int str_len = 0;

	SLSI_DBG1_NODEV(SLSI_T20_80211, "Init SLSI driver, ifname = %s\n", ifname);

	sdev = cm_ctx.sdev;
	drv = sdev->drv;
	if (strncmp(ifname, "wl1", strlen(ifname)) == 0) {
		for (str_len = 0; str_len < IFNAMSIZ + 1; str_len++) {
			drv->supp_iface[SLSI_NET_INDEX_WLAN].ifname[str_len] = ifname[str_len];
		}
		drv->supp_iface[SLSI_NET_INDEX_WLAN].ifname[IFNAMSIZ] = '\0';

		dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
		if (!dev) {
			SLSI_ERR_NODEV("dev not available\n");
			return NULL;
		}

		slsi_update_supp_iface_fields(SLSI_NET_INDEX_WLAN, ctx, dev, sdev, drv);
		netdev_ifup(dev);
		return &(drv->supp_iface[SLSI_NET_INDEX_WLAN]);
	}
#ifdef CONFIG_SLSI_WLAN_P2P
	else if (strncmp(ifname, "wl2", strlen(ifname)) == 0) {
		dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2P);
		if (!dev) {
			SLSI_ERR_NODEV("p2pdev not available\n");
			return NULL;
		}

		slsi_update_supp_iface_fields(SLSI_NET_INDEX_P2P, ctx, dev, sdev, drv);
		netdev_ifup(dev);
		return &(drv->supp_iface[SLSI_NET_INDEX_P2P]);
	} else if (strncmp(ifname, "wl3", strlen(ifname)) == 0) {
		dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX);
		if (!dev) {
			SLSI_ERR_NODEV("p2pdev not available\n");
			return NULL;
		}

		slsi_update_supp_iface_fields(SLSI_NET_INDEX_P2PX, ctx, dev, sdev, drv);
		netdev_ifup(dev);
		return &(drv->supp_iface[SLSI_NET_INDEX_P2PX]);
	}
#endif
	return NULL;
}

int slsi_t20_deinit(void *priv)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	SLSI_DBG1_NODEV(SLSI_T20_80211, "Deinit SLSI driver\n");
	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	dev = drv->dev;
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return 0;
	}

	/* Set flag to indicate driver that ifdown is triggered by supplicant */
	sdev->is_supplicant_deinit = 1;

	/* The drv context in supplicant is freed by ifdown handler */
	netdev_ifdown(dev);

	/* Reset flag */
	sdev->is_supplicant_deinit = 0;

	/* Reset supplicant context */
	/* TODO: It would be good to call slsi_update_supp_iface_fields with all NULL to reset
	 * the supp_iface fields. This would require the ifnum here. */
	ndev_vif = netdev_priv(dev);
	if (ndev_vif) {
		ndev_vif->ctx = NULL;
	}

	return 0;
}

int slsi_t20_set_param(void *priv, const char *param)
{
	return 0;
}

struct hostapd_hw_modes *slsi_get_hw_feature_data(void *priv, u16 *num_modes, u16 *flags)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct slsi_wlan_supported_band *supp_band;
	struct hostapd_hw_modes *hw_modes, *mode;
	int i;

	if (!drv) {
		SLSI_ERR_NODEV("Driver context not available\n");
		return NULL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return NULL;
	}
	supp_band = sdev->device_config.band_2G;

	*num_modes = 2;

	hw_modes = kmm_zalloc(*num_modes * sizeof(struct hostapd_hw_modes));

	if (hw_modes == NULL) {
		SLSI_ERR_NODEV("hw_modes is NULL\n");
		return NULL;
	}
	/* Only 2.4 G */
	/* 80211G mode */
	mode = &hw_modes[0];
	mode->mode = HOSTAPD_MODE_IEEE80211G;
	mode->num_channels = supp_band->n_channels;
	mode->channels = kmm_zalloc(supp_band->n_channels * sizeof(struct hostapd_channel_data));
	if (mode->channels == NULL) {
		SLSI_ERR_NODEV("mode channels are NULL\n");
		return NULL;
	}
	for (i = 0; i < supp_band->n_channels; i++) {
		mode->channels[i].chan = supp_band->channels[i].hw_value;
		mode->channels[i].freq = supp_band->channels[i].center_freq;
		/* mode->channels[i].flag = supp_band->channels[i].flags; */
		/* hw_modes->channels other settings : leaving out for now */
	}
	mode->num_rates = supp_band->n_bitrates;
	mode->rates = kmm_zalloc(supp_band->n_bitrates * sizeof(int));
	if (mode->rates == NULL) {
		SLSI_ERR_NODEV("mode rates are NULL\n");
		return NULL;
	}

	for (i = 0; i < supp_band->n_bitrates; i++) {
		mode->rates[i] = supp_band->bitrates[i].bitrate;
	}
	mode->ht_capab = supp_band->ht_cap.cap;
	/* mcs rate: only the first index (of 16) is filled */
	mode->mcs_set[0] = supp_band->ht_cap.mcs.rx_mask[0];
	mode->a_mpdu_params |= supp_band->ht_cap.ampdu_factor & 0x03;
	mode->a_mpdu_params |= supp_band->ht_cap.ampdu_density << 2;
	/* vht_capab & vht_mcs_set: leaving out for now */
	mode->flags = HOSTAPD_MODE_FLAG_HT_INFO_KNOWN | HOSTAPD_MODE_FLAG_VHT_INFO_KNOWN;

	/* 80211B mode */
	mode = &hw_modes[1];
	mode->mode = HOSTAPD_MODE_IEEE80211B;
	mode->num_channels = supp_band->n_channels;
	mode->channels = kmm_zalloc(supp_band->n_channels * sizeof(struct hostapd_channel_data));
	if (mode->channels == NULL) {
		SLSI_ERR_NODEV("mode channels are NULL\n");
		return NULL;
	}
	for (i = 0; i < supp_band->n_channels; i++) {
		mode->channels[i].chan = supp_band->channels[i].hw_value;
		mode->channels[i].freq = supp_band->channels[i].center_freq;
		/* mode->channels[i].flag = supp_band->channels[i].flags; */
		/* hw_modes->channels other settings : leaving out for now */
	}
	mode->rates = kmm_zalloc(supp_band->n_bitrates * sizeof(int));
	if (mode->rates == NULL) {
		SLSI_ERR_NODEV("mode rates are NULL\n");
		return NULL;
	}
	for (i = 0; i < supp_band->n_bitrates; i++) {
		if (supp_band->bitrates[i].bitrate != 10 && supp_band->bitrates[i].bitrate != 20 && supp_band->bitrates[i].bitrate != 55 && supp_band->bitrates[i].bitrate != 110) {
			continue;
		}
		mode->rates[mode->num_rates] = supp_band->bitrates[i].bitrate;
		mode->num_rates++;
		if (mode->num_rates == 4) {
			break;
		}
	}
	return hw_modes;
}

int slsi_t20_get_capa(void *priv, struct wpa_driver_capa *capa)
{
	struct slsi_drv_interface *drv_iface = priv;
	struct slsi_t20_drv *drv;
	SLSI_DBG1_NODEV(SLSI_T20_80211, "slsi_t20_get_capa SLSI driver\n");
	drv = drv_iface->sdev->drv;
	memcpy(capa, &drv->capa, sizeof(*capa));
	if (drv->capa.extended_capa && drv->capa.extended_capa_mask) {
		capa->extended_capa = drv->capa.extended_capa;
		capa->extended_capa_mask = drv->capa.extended_capa_mask;
		capa->extended_capa_len = drv->capa.extended_capa_len;
	}

	return 0;
}

static struct slsi_wlan_sta_ht_cap slsi_ht_cap_2ghz = {
	.ht_supported = true,
	.cap = SLSI_WLAN_HT_CAPAB_RX_STBC | SLSI_WLAN_HT_CAPAB_SGI_20,
	.ampdu_factor = SLSI_WLAN_HT_MAX_AMPDU_64K,
	.ampdu_density = SLSI_WLAN_HT_MPDU_MIN_SPACING_16,
	.mcs = {
		.rx_mask = {0xff, 0,},
		.rx_highest = cpu_to_le16(0),
		.tx_params = 0,
	},
};

struct slsi_wlan_supported_band slsi_band_2ghz = {
	.channels = slsi_2ghz_channels,
	.band = SLSI_WLAN_BAND_2GHZ,
	.n_channels = ARRAY_SIZE(slsi_2ghz_channels),
	.bitrates = slsi_11g_rates,
	.n_bitrates = ARRAY_SIZE(slsi_11g_rates),
};

#ifdef CONFIG_SCSC_ADV_FEATURE
static struct slsi_wlan_channel slsi_5ghz_channels[] = {
	/* _We_ call this UNII 1 */
	CHAN5G(5180, 36),
	CHAN5G(5200, 40),
	CHAN5G(5220, 44),
	CHAN5G(5240, 48),
	/* UNII 2 */
	CHAN5G(5260, 52),
	CHAN5G(5280, 56),
	CHAN5G(5300, 60),
	CHAN5G(5320, 64),
	/* "Middle band" */
	CHAN5G(5500, 100),
	CHAN5G(5520, 104),
	CHAN5G(5540, 108),
	CHAN5G(5560, 112),
	CHAN5G(5580, 116),
	CHAN5G(5600, 120),
	CHAN5G(5620, 124),
	CHAN5G(5640, 128),
	CHAN5G(5660, 132),
	CHAN5G(5680, 136),
	CHAN5G(5700, 140),
	/* UNII 3 */
	CHAN5G(5745, 149),
	CHAN5G(5765, 153),
	CHAN5G(5785, 157),
	CHAN5G(5805, 161),
	CHAN5G(5825, 165),
};

/* note fw_rate_idx_to_host_11a_idx[] below must change if this table changes */

static struct slsi_wlan_rate wifi_11a_rates[] = {
	RATE_LEGACY(60, 4, 0),
	RATE_LEGACY(90, 5, 0),
	RATE_LEGACY(120, 7, 0),
	RATE_LEGACY(180, 8, 0),
	RATE_LEGACY(240, 9, 0),
	RATE_LEGACY(360, 10, 0),
	RATE_LEGACY(480, 11, 0),
	RATE_LEGACY(540, 12, 0),
};

struct slsi_wlan_supported_band slsi_band_5ghz = {
	.channels = slsi_5ghz_channels,
	.band = SLSI_WLAN_BAND_5GHZ,
	.n_channels = ARRAY_SIZE(slsi_5ghz_channels),
	.bitrates = wifi_11a_rates,
	.n_bitrates = ARRAY_SIZE(wifi_11a_rates),
};
#endif /* CONFIG_SCSC_ADV_FEATURE */

static struct slsi_802_11d_reg_domain slsi_regdomain = {
	.reg_rules = {
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
		SLSI_REG_RULE(0, 0, 0, 0, 0, 0),
	}
};

struct slsi_dev *slsi_new(void)
{
	struct slsi_dev *sdev = NULL;

	SLSI_INFO_NODEV("slsi_new()\n");
	sdev = kmm_zalloc(sizeof(struct slsi_dev));
	if (!sdev) {
		SLSI_ERR_NODEV("Failed to allocate sdev\n");
		return NULL;
	}

	sdev->flags = WPA_DRIVER_FLAGS_AP;
	sdev->flags |= WPA_DRIVER_FLAGS_PROBE_RESP_OFFLOAD;

#ifndef CONFIG_SCSC_WLAN_STA_ONLY
	sdev->flags |= WPA_DRIVER_FLAGS_AP_UAPSD;
	sdev->flags |= WPA_DRIVER_FLAGS_INACTIVITY_TIMER;
#ifdef CONFIG_SLSI_WLAN_P2P
	sdev->flags |= WPA_DRIVER_FLAGS_P2P_MGMT_AND_NON_P2P;
	sdev->flags |= WPA_DRIVER_FLAGS_P2P_CAPABLE;
	sdev->flags |= WPA_DRIVER_FLAGS_OFFCHANNEL_TX;
#endif
#endif

	slsi_band_2ghz.ht_cap = slsi_ht_cap_2ghz;
#ifdef SCSC_WLAN_ENABLE_5GHZ
	slsi_band_5ghz.ht_cap = slsi_ht_cap;
#endif

	/* Disable 5Ghz in device configuration for single band support */
#ifdef SCSC_WLAN_ENABLE_5GHZ
	sdev->device_config.band_5G = &slsi_band_5ghz;
#else
	sdev->device_config.band_5G = NULL;
#endif
	sdev->device_config.band_2G = &slsi_band_2ghz;

	sdev->device_config.domain_info = &slsi_regdomain;

	sdev->max_remain_on_chan = 5000;	/* 5000 msec */

	/* Basic scan parameters */
	sdev->max_scan_ssids = 10;

#ifndef CONFIG_SCSC_WLAN_STA_ONLY
	/* P2P flags */
#ifdef CONFIG_SLSI_WLAN_P2P
	/* Enable Probe response offloading w.r.t WPS and P2P */
	sdev->probe_resp_offloads = WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS2 | WPA_DRIVER_PROBE_RESP_OFFLOAD_P2P;
#endif
#endif
	sdev->max_acl_mac_addrs = 10;
	sdev->max_match_sets = 16;
	sdev->sched_scan_supported = 1;

	sdev->max_sched_scan_ssids = 16;

	sdev->max_stations = SLSI_AP_PEER_CONNECTIONS_MAX;

	return sdev;
}

ssize_t slsi_set_country(void *priv, const char *country_code)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	char alpha2_rev[4];
	int status = 0;
#ifdef CONFIG_SLSI_WLAN_P2P
	struct netif *p2p_dev;
	struct netdev_vif *p2p_ndev_vif;
#endif

	if (!drv) {
		SLSI_ERR_NODEV("Driver context not available\n");
		return -EINVAL;
	}
	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	if (!country_code) {
		return -EINVAL;
	}

	memcpy(alpha2_rev, country_code, 2);
	alpha2_rev[2] = 0x0;
	alpha2_rev[3] = '\0';

	status = slsi_set_country_update_regd(sdev, alpha2_rev, 4);

	ndev_vif = netdev_priv(dev);
	slsi_channel_list_changed(ndev_vif, country_code);

#ifdef CONFIG_SLSI_WLAN_P2P
	/* Send channel list change event to supplicant */
	p2p_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2P);
	p2p_ndev_vif = netdev_priv(p2p_dev);
	slsi_channel_list_changed(p2p_ndev_vif, country_code);
#endif
	return status;
}

ssize_t slsi_get_country(void *priv, char *country_code)
{
	struct slsi_drv_interface *drv = priv;
	struct slsi_dev *sdev;
	u8 buf[4];
	int len = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver context not available\n");
		return -EINVAL;
	}
	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

	memset(buf, 0, sizeof(buf));

	memcpy(buf, sdev->device_config.domain_info->alpha2, 3);
	len += 3;
	buf[3] = '\0';
	len++;

	memcpy(country_code, buf, len);

	return 1;
}
