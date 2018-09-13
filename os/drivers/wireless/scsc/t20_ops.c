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
#include "wlan_80211_utils.h"
#include "netif.h"

#define SLSI_FW_SCAN_DONE_TIMEOUT_MSEC (15 * 1000)
#define SLSI_MAX_CHAN_2G_BAND          14
#define FW_TX_POWER_LEVEL 4		/*Expressed in 0.25 dBm units */

#ifdef CONFIG_SCSC_ENABLE_P2P
static uint keep_alive_period = SLSI_P2PGO_KEEP_ALIVE_PERIOD_SEC;
#endif

#define RATE_LEGACY(_rate, _hw_value, _flags) { \
		.bitrate = (_rate), \
		.hw_value = (_hw_value), \
		.flags = (_flags), \
}

#define CHAN2G(_freq, _idx)  { \
		.band = SLSI_80211_BAND_2GHZ, \
		.center_freq = (_freq), \
		.hw_value = (_idx), \
}

#ifdef CONFIG_SCSC_ADV_FEATURE
#define CHAN5G(_freq, _idx)  { \
		.band = SLSI_80211_BAND_5GHZ, \
		.center_freq = (_freq), \
		.hw_value = (_idx), \
}
#endif

struct slsi_cm_ctx cm_ctx;

static struct slsi_80211_channel slsi_2ghz_channels[] = {
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

static struct slsi_80211_rate slsi_11g_rates[] = {
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

#ifdef CONFIG_SCSC_ADV_FEATURE
struct wireless_dev *slsi_add_virtual_intf(struct wiphy *wiphy, const char *name, enum slsi_80211_iftype type, u32 *flags, struct vif_params *params)
{
	struct slsi_dev *sdev = SDEV_FROM_WIPHY(wiphy);
	struct net_device *dev = NULL;
	struct netdev_vif *ndev_vif = NULL;
	int err = -EINVAL;
	int iface;

	SLSI_UNUSED_PARAMETER(flags);

	SLSI_DBG1(sdev, SLSI_T20_80211, "name:%s\n", name);

	iface = slsi_netif_add(sdev, name);
	if (iface < 0) {
		goto exit_with_error;
	}

	dev = slsi_get_netdev(sdev, iface);
	if (dev == NULL) {
		goto exit_with_error;
	}

	ndev_vif = netdev_priv(dev);

	err = slsi_netif_register_rtlnl_locked(sdev, dev);
	if (err) {
		goto exit_with_error;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	ndev_vif->iftype = type;
	dev->ieee80211_ptr->iftype = type;
	if (params) {
		dev->ieee80211_ptr->use_4addr = params->use_4addr;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return &ndev_vif->wdev;

exit_with_error:
	return ERR_PTR(-ENODEV);
}

int slsi_del_virtual_intf(struct wiphy *wiphy, struct wireless_dev *wdev)
{
	struct net_device *dev = wdev->netdev;
	struct slsi_dev *sdev = SDEV_FROM_WIPHY(wiphy);

	if (WARN_ON(!dev)) {
		return -EINVAL;
	}

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "\n");

	slsi_stop_net_dev(sdev, dev);
	slsi_netif_remove_rtlnl_locked(sdev, dev);

	return 0;
}

int slsi_change_virtual_intf(struct wiphy *wiphy, struct net_device *dev, enum slsi_80211_iftype type, u32 *flags, struct vif_params *params)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int r = 0;

	SLSI_UNUSED_PARAMETER(flags);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "type:%u\n", type);

	if (WARN_ON(ndev_vif->activated)) {
		r = -EINVAL;
		goto exit;
	}

	switch (type) {
	case SLSI_80211_IFTYPE_UNSPECIFIED:
	case SLSI_80211_IFTYPE_ADHOC:
	case SLSI_80211_IFTYPE_STATION:
	case SLSI_80211_IFTYPE_AP:
	case SLSI_80211_IFTYPE_P2P_CLIENT:
	case SLSI_80211_IFTYPE_P2P_GO:
		ndev_vif->iftype = type;
		dev->ieee80211_ptr->iftype = type;
		if (params) {
			dev->ieee80211_ptr->use_4addr = params->use_4addr;
		}
		break;
	default:
		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Operation not supported\n");
		r = -EINVAL;
		break;
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

#endif							/* CONFIG_SCSC_ADV_FEATURE */

static u32 wpa_alg_to_cipher_suite(enum wpa_alg alg, size_t key_len)
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
			return SLSI_WLAN_CIPHER_SUITE_WEP40;
		}
		return SLSI_WLAN_CIPHER_SUITE_WEP104;
	default:					/* SLSI_WPA_ALG_NONE/SLSI_WPA_ALG_PMK/SLSI_WPA_ALG_IGTK */
		SLSI_ERR_NODEV(" Unexpected encryption algorithm %d", alg);
		return 0;
	}
}

int slsi_add_key(const char *ifname, void *priv, enum wpa_alg alg, const u8 *mac_addr, int key_index, int set_tx, const u8 *seq, size_t seq_len, const u8 *key, size_t key_len)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer = NULL;
	int r = 0;
	u16 key_type = FAPI_KEYTYPE_GROUP;
	struct slsi_key_params params;
	bool pairwise = FALSE;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
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
	if (alg != SLSI_WPA_ALG_WEP && key_index && is_broadcast_ether_addr(mac_addr)) {
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
	if ((ndev_vif->vif_type == FAPI_VIFTYPE_STATION) && ((params.cipher == SLSI_WLAN_CIPHER_SUITE_WEP40) || (params.cipher == SLSI_WLAN_CIPHER_SUITE_WEP104)) && peer) {
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
#ifdef CONFIG_SCSC_ENABLE_P2P
			if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO) {
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
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
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
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int r = 0, seq_len = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
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

		if (ie[0] == WLAN_EID_VENDOR_SPECIFIC && ie[1] > 4) {
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
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return NULL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return NULL;
	}

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return NULL;
	}

	SLSI_NET_INFO(dev, "MAC address: " SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(dev->d_mac.ether_addr_octet));
	return dev->d_mac.ether_addr_octet;
}

int slsi_hw_scan(void *priv, struct wpa_driver_scan_params *request)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	u16 scan_type = FAPI_SCANTYPE_FULL_SCAN;
	int r, i;

#ifdef CONFIG_SCSC_ENABLE_P2P
	u16 p2p_state;
#endif
	u8 *scan_ie = NULL;;
	size_t scan_ie_len;
	bool strip_wsc = false;
	bool strip_p2p = false;
	struct slsi_80211_channel *channels[15];
	int chan_count = 0;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return -EINVAL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return -EINVAL;
	}

#ifdef CONFIG_SCSC_ENABLE_P2P
	p2p_state = sdev->p2p_state;
#endif
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_WARN(dev, "Not supported in WlanLite mode\n");
		return -EOPNOTSUPP;
	}

	ndev_vif = netdev_priv(dev);

	/* To handle race conditions check driver is UP */
	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	if (!ndev_vif->is_available) {
		SLSI_ERR_NODEV("Driver is down\n");
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
		return -EINVAL;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

#ifdef CONFIG_SCSC_ENABLE_P2P
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

	/* Convert scan params freqs to slsi_80211_channel. Only 2.4 GHz channels considered for now.
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

#ifdef CONFIG_SCSC_ENABLE_P2P
	/* Update scan timing for P2P social channels scan.
	 * Checking for SSID and num_channel looks good enough, hence skip social channel check
	 */
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif) && (request->n_ssids == 1) && SLSI_IS_P2P_SSID(request->ssids[0].ssid, request->ssids[0].ssid_len)) {
		/* In supplicant during joining procedure the P2P GO scan
		 * with GO's operating channel comes on P2P device. Hence added the
		 * check for n_channels as 1
		 */
		if (request->n_channels == SLSI_P2P_SOCIAL_CHAN_COUNT || request->n_channels == 1) {
			p2p_state = P2P_SCANNING;
			scan_type = FAPI_SCANTYPE_P2P_SCAN_SOCIAL;
		} else if (request->n_channels > SLSI_P2P_SOCIAL_CHAN_COUNT) {
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

		ie = (u8 *)slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPS, request->extra_ies, request->extra_ies_len);
		if (ie && ie[1] > SLSI_WPS_REQUEST_TYPE_POS && ie[SLSI_WPS_REQUEST_TYPE_POS] == SLSI_WPS_REQUEST_TYPE_ENROLEE_INFO_ONLY) {
			strip_wsc = true;
		}

		ie = (u8 *)slsi_80211_find_vendor_ie(WLAN_OUI_WFA, WLAN_OUI_TYPE_WFA_P2P, request->extra_ies, request->extra_ies_len);
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
		scan_ie = (u8 *)request->extra_ies;
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
		work_queue(SCSC_WORK, &ndev_vif->scan_timeout_work, slsi_scan_ind_timeout_handle, (FAR void *)drv, MSEC2TICK(SLSI_FW_SCAN_DONE_TIMEOUT_MSEC));
#ifdef CONFIG_SCSC_ENABLE_P2P
		/* Update State only for scan in Device role */
		if (SLSI_IS_VIF_INDEX_P2P(ndev_vif) && (!SLSI_IS_P2P_GROUP_STATE(sdev))) {
			SLSI_P2P_STATE_CHANGE(sdev, p2p_state);
		} else if (SLSI_IS_VIF_INDEX_P2P_GROUP(ndev_vif) && (request->ie)) {
			/* Supplicant includes only WPS and P2P IE as of now. Hence a blind copy is done. */
			SLSI_NET_DBG3(dev, SLSI_T20_80211, "Scan IEs present with length = %zu\n", request->ie_len);
			sdev->gc_probe_req_ies = kmm_malloc(request->ie_len);
			if (sdev->gc_probe_req_ies == NULL) {	/* Don't fail, continue as it would still work */
				sdev->gc_probe_req_ie_len = 0;
			} else {
				sdev->gc_probe_req_ie_len = request->ie_len;
				memcpy(sdev->gc_probe_req_ies, request->ie, request->ie_len);
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
	struct slsi_80211_mgmt *frame = fapi_get_mgmt(scan);
	size_t frame_len = fapi_get_mgmtlen(scan);
	size_t ie_len = frame_len - (frame->u.beacon.variable - (u8 *)frame);
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
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct wpa_scan_results *results = NULL;
	struct max_buff *scan;
	int scan_id = SLSI_SCAN_HW_ID;
	int num_scan_ind;

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
		return NULL;
	}

	sdev = drv->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not available\n");
		return NULL;
	}
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	return results;
}

int slsi_get_ap_bssid(void *priv, u8 *bssid)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct slsi_peer *peer;
	u8 null_address[ETH_ALEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	if (!drv) {
		SLSI_ERR_NODEV("Driver not available\n");
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
	ndev_vif = netdev_priv(dev);

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
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	u8 device_address[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int r = 0;
	struct slsi_peer *peer;
	u16 prev_vif_type;
	u32 action_frame_bmap;
#ifdef CONFIG_SCSC_ENABLE_P2P
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

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "%.*s, " SLSI_MAC_FORMAT "\n", (int)request->ssid_len, request->ssid, SLSI_MAC_STR(request->bssid));
#ifdef CONFIG_SCSC_ENABLE_P2P
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		SLSI_NET_ERR(dev, "Connect requested on incorrect vif\n");
		goto exit_with_error;
	}
#endif
	if (WARN_ON(!request->ssid)) {
		goto exit_with_error;
	}

	if (WARN_ON(request->ssid_len > SLSI_80211_MAX_SSID_LEN)) {
		goto exit_with_error;
	}

	if (request->mode == 2) {
		if (ndev_vif->iftype != SLSI_80211_IFTYPE_UNSPECIFIED) {
			goto exit_with_error;
		} else {
			goto exit;
		}
	}

	if (WARN_ON(ndev_vif->activated)) {
		goto exit_with_error;
	}

	if (WARN_ON(ndev_vif->vif_type == FAPI_VIFTYPE_STATION && ndev_vif->sta.vif_status != SLSI_VIF_STATUS_UNSPECIFIED)) {
		SLSI_NET_ERR(dev, "VIF status: %d\n", ndev_vif->sta.vif_status);
		goto exit_with_error;
	}
	prev_vif_type = ndev_vif->vif_type;

	prev_vif_type = ndev_vif->vif_type;
	switch (ndev_vif->iftype) {
	case SLSI_80211_IFTYPE_UNSPECIFIED:
	case SLSI_80211_IFTYPE_STATION:
		ndev_vif->iftype = SLSI_80211_IFTYPE_STATION;
		action_frame_bmap = SLSI_ACTION_FRAME_WMM | SLSI_ACTION_FRAME_PUBLIC;
		break;
#ifdef CONFIG_SCSC_ENABLE_P2P
	case SLSI_80211_IFTYPE_P2P_CLIENT:
		slsi_p2p_group_start_remove_unsync_vif(sdev);
		p2p_dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2P);
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
		work_cancel(SCSC_WORK, &ndev_vif->scan_timeout_work);
		(void)slsi_mlme_del_scan(sdev, dev, ndev_vif->ifnum << 8 | SLSI_SCAN_HW_ID);
		slsi_mbuf_queue_purge(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		slsi_scan_complete(ndev_vif);
		ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req = 0;
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);

	ndev_vif->channel_type = SLSI_80211_CHAN_NO_HT;
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
#ifdef CONFIG_SCSC_ENABLE_P2P
	if (sdev->device_config.qos_info != 0) {
		r = slsi_set_uapsd_qos_info(sdev, dev, sdev->device_config.qos_info);
		if (r != 0) {
			SLSI_NET_ERR(dev, "qosInfo MIB write failed: %d\n", r);
			goto exit_with_vif;
		}
	}
	/* If P2P CLI, add_info_elements with Probe Req IEs. Proceed even if confirm fails for add_info as it would still work if the
	 * fw pre-join scan does not include the vendor IEs
	 */
	if ((ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_CLIENT) && (sdev->gc_probe_req_ies)) {
		if (sme->crypto.wpa_versions == 2) {
			sdev->delete_gc_probe_req_ies = true;    /* Stored P2P Probe Req can be deleted at vif deletion, after WPA2 association  */
		} else
			/* Retain stored P2P Probe Req at vif deletion until WPA2 connection to allow Probe request with P2P IE, after WSC */
		{
			sdev->delete_gc_probe_req_ies = false;
		}
		SLSI_NET_DBG3(dev, SLSI_T20_80211, "Setting Probe Req IEs for P2P CLI\n");
		(void)slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_REQUEST, sdev->gc_probe_req_ies, sdev->gc_probe_req_ie_len);
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

	peer = slsi_peer_add(sdev, dev, (u8 *)request->bssid, SLSI_STA_PEER_QUEUESET + 1);
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
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return r;
}

int slsi_disconnect(void *priv, const u8 *addr, int reason_code)
{
	struct slsi_t20_drv *drv = priv;
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

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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
		r = slsi_handle_disconnect(sdev, dev, peer->address, 0);

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
	struct slsi_t20_drv *drv = priv;
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
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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
	struct slsi_t20_drv *drv = priv;
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
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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
	if (((ndev_vif->iftype == SLSI_80211_IFTYPE_STATION && ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)
		 || ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_CLIENT)) {
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
	struct slsi_t20_drv *drv = priv;
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
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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
	struct slsi_t20_drv *drv = priv;
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
	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	SLSI_DBG1_NODEV(SLSI_MLME, "Setting TX power to: %d", dbm * FW_TX_POWER_LEVEL);

	return slsi_set_mib(sdev, dev, SLSI_PSID_UNIFI_USER_SET_TXPOWER_LEVEL, dbm * FW_TX_POWER_LEVEL);
}

int slsi_get_tx_power(void *priv)
{
	struct slsi_t20_drv *drv = priv;
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
	struct slsi_t20_drv *drv = priv;
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

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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
	} else if (is_broadcast_ether_addr(mac)) {
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
			r = slsi_handle_disconnect(sdev, dev, peer->address, WLAN_REASON_DEAUTH_LEAVING);
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
			r = slsi_handle_disconnect(sdev, dev, peer->address, reason);
		}
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return r;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
static void slsi_ap_start_obss_scan(struct slsi_dev *sdev, struct netif *dev, struct netdev_vif *ndev_vif)
{
	struct cfg80211_ssid ssids;
	struct ieee80211_channel *channel;
	int n_ssids = 1, n_channels = 1, i;

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "Perform scan before starting 20 MHz HT AP on channel %u\n", ndev_vif->chan->hw_value);

	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

	ssids.ssid_len = 0;
	for (i = 0; i < SLSI_80211_MAX_SSID_LEN; i++) {
		ssids.ssid[i] = 0x00;    /* Broadcast SSID */
	}

	channel = ieee80211_get_channel(sdev->wiphy, ndev_vif->chan->center_freq);

	(void)slsi_mlme_add_scan(sdev, dev, FAPI_SCANTYPE_OBSS_SCAN, FAPI_REPORTMODE_REAL_TIME, n_ssids, &ssids, n_channels, &channel, NULL, NULL,	/* No IEs */
							 0, true /* Wait for scan_done_ind */);

	slsi_ap_obss_scan_done_ind(dev, ndev_vif);
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

static int slsi_ap_start_validate(struct netif *dev, struct slsi_dev *sdev, struct wpa_driver_ap_params *settings)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

#ifdef CONFIG_SCSC_ENABLE_P2P
	if (SLSI_IS_VIF_INDEX_P2P(ndev_vif)) {
		SLSI_NET_ERR(dev, "AP start requested on incorrect vif\n");
		goto exit_with_error;
	}
#endif

	if (!settings->ssid_len || !settings->ssid) {
		SLSI_NET_ERR(dev, "SSID not provided\n");
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
		SLSI_NET_ERR(dev, "Configuration error: 40 MHz on 2.4 GHz is not supported. Channel_no: %d Channel_width: %d\n", settings->freq->channel, slsi_get_chann_info(sdev, settings->freq));
		goto exit_with_error;
	}

	ndev_vif->chandef = kmm_malloc(sizeof(struct hostapd_freq_params));
	if (ndev_vif->chandef == NULL) {
		SLSI_NET_ERR(dev, "chandef alloc failed\n");
		goto exit_with_error;
	}
	memcpy(ndev_vif->chandef, settings->freq, sizeof(struct hostapd_freq_params));

	if (WARN_ON(ndev_vif->activated)) {
		goto exit_with_error;
	}

	return 0;

exit_with_error:
	return -EINVAL;
}

int slsi_start_ap(void *priv, struct wpa_driver_ap_params *settings)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	u8 device_address[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int r = 0;
	const u8 *wpa_ie_pos = NULL;
	size_t wpa_ie_len = 0;
	const u8 *wmm_ie_pos = NULL;
	size_t wmm_ie_len = 0;

#ifdef CONFIG_SCSC_ADV_FEATURE
	UNUSED const u8 *country_ie = NULL;
	UNUSED char alpha2[SLSI_COUNTRY_CODE_LEN];
#endif
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

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return -EINVAL;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "Iftype: %d\n", ndev_vif->iftype);

	r = slsi_ap_start_validate(dev, sdev, settings);
	if (r != 0) {
		goto exit_with_vif_mutex;
	}

	memset(&ndev_vif->ap, 0, sizeof(ndev_vif->ap));
	/* Initialise all allocated peer structures to remove old data. */
	/*slsi_netif_init_all_peers(sdev, dev); */
#ifdef CONFIG_SCSC_ADV_FEATURE
	/* Reg domain changes */
	country_ie = slsi_80211_find_ie(WLAN_EID_COUNTRY, settings->tail, settings->tail_len);
	if (country_ie) {
		country_ie += 2;
		memcpy(alpha2, country_ie, SLSI_COUNTRY_CODE_LEN);
		if (memcmp(sdev->device_config.domain_info.regdomain->alpha2, alpha2, SLSI_COUNTRY_CODE_LEN - 1) != 0) {
			if (slsi_set_country_update_regd(sdev, alpha2, SLSI_COUNTRY_CODE_LEN) != 0) {
				r = -EINVAL;
				goto exit_with_vif_mutex;
			}

			if (slsi_check_channelization(sdev, ndev_vif->chandef) != 0) {
				r = -EINVAL;
				goto exit_with_vif_mutex;
			}
		}
	}
#endif

#ifdef CONFIG_SCSC_ENABLE_P2P
	if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO) {
		slsi_p2p_group_start_remove_unsync_vif(sdev);
		SLSI_ETHER_COPY(device_address, sdev->netdev_addresses[SLSI_NET_INDEX_P2P]);
		if (keep_alive_period != SLSI_P2PGO_KEEP_ALIVE_PERIOD_SEC)
			if (slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_GO_KEEP_ALIVE_PERIOD, keep_alive_period) != 0) {
				SLSI_NET_ERR(dev, "P2PGO Keep Alive MIB set failed");
				r = -EINVAL;
				goto exit_with_vif_mutex;
			}
#ifdef CONFIG_SCSC_ENABLE_P2P_VHT
		/* Default 11ac configuration */
		if (!(slsi_80211_find_ie(WLAN_EID_VHT_CAPABILITY, settings->tail, settings->tail_len)) && (ndev_vif->chandef->chan->hw_value >= 36)) {
			u16 oper_chan = ndev_vif->chandef->chan->hw_value;

			ndev_vif->chandef->width = CHAN_WIDTH_80;
			if ((oper_chan >= 36) && (oper_chan <= 48)) {
				ndev_vif->chandef->center_freq1 = ieee80211_channel_to_frequency(42, SLSI_80211_BAND_5GHZ);
			} else if ((oper_chan >= 149) && (oper_chan <= 161)) {
				ndev_vif->chandef->center_freq1 = ieee80211_channel_to_frequency(155, SLSI_80211_BAND_5GHZ);
			}
			if (cfg80211_chandef_valid(ndev_vif->chandef)) {
				u8 *ht_operation_ie;
				u8 sec_chan_offset = 0;

				append_vht_ies = true;
				ht_operation_ie = (u8 *)slsi_80211_find_ie(WLAN_EID_HT_OPERATION, settings->beacon.tail, settings->beacon.tail_len);
				if (!ht_operation_ie) {
					SLSI_NET_ERR(dev, "HT Operation IE is not passed by wpa_supplicant");
					r = -EINVAL;
					goto exit_with_vif_mutex;
				}

				if (oper_chan == 36 || oper_chan == 44 || oper_chan == 149 || oper_chan == 157) {
					sec_chan_offset = SLSI_80211_HT_PARAM_CHA_SEC_ABOVE;
				} else {
					sec_chan_offset = SLSI_80211_HT_PARAM_CHA_SEC_BELOW;
				}
				/* Change HT Information IE subset 1 */
				ht_operation_ie += 3;
				*(ht_operation_ie) |= sec_chan_offset;
				*(ht_operation_ie) |= SLSI_80211_HT_PARAM_CHAN_WIDTH_ANY;
			}
		}
		/* End */
#else
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
					ht_info_value = SLSI_80211_HT_PARAM_CHAN_WIDTH_ANY | SLSI_80211_HT_PARAM_CHA_SEC_BELOW;
					ndev_vif->chandef->center_freq1 = ndev_vif->chandef->chan->center_freq - 10;
					offset_above = false;
					break;
				}

			if (offset_above) {
				ht_info_value = SLSI_80211_HT_PARAM_CHAN_WIDTH_ANY | SLSI_80211_HT_PARAM_CHA_SEC_ABOVE;
				ndev_vif->chandef->center_freq1 = ndev_vif->chandef->chan->center_freq + 10;
			}

			if (cfg80211_chandef_valid(ndev_vif->chandef)) {
				slsi_modify_ies(dev, WLAN_EID_HT_OPERATION, (u8 *)settings->beacon.tail, settings->beacon.tail_len, 3, ht_info_value);
				slsi_modify_ies(dev, WLAN_EID_HT_CAPABILITY, (u8 *)settings->beacon.tail, settings->beacon.tail_len, 2, SLSI_80211_HT_CAP_SUP_WIDTH_20_40);
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
		if (slsi_80211_find_ie(WLAN_EID_HT_CAPABILITY, settings->tail, settings->tail_len)) {
			u8 mod_value = SLSI_80211_HT_CAP_SGI_20;

			if (ndev_vif->chandef->bandwidth == 40) {
				mod_value |= SLSI_80211_HT_CAP_SGI_40;
			}

			slsi_modify_ies(dev, WLAN_EID_HT_CAPABILITY, (u8 *)settings->tail, settings->tail_len, 2, mod_value);
		}
#ifdef CONFIG_SCSC_ADV_FEATURE
		if (slsi_80211_find_ie(WLAN_EID_VHT_CAPABILITY, settings->tail, settings->tail_len)) {
			slsi_modify_ies(dev, WLAN_EID_VHT_CAPABILITY, (u8 *)settings->tail, settings->tail_len, 2, SLSI_80211_VHT_CAP_SHORT_GI_80);
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
	wpa_ie_pos = slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPA, settings->tail, settings->tail_len);
	if (wpa_ie_pos) {
		wpa_ie_len = *(wpa_ie_pos + 1) + 2;	/* For 0xdd (1) and Tag Length (1) */
		SLSI_NET_DBG2(dev, SLSI_T20_80211, "WPA IE found: Length = %zu\n", wpa_ie_len);
		SLSI_EC_GOTO(slsi_cache_ies(wpa_ie_pos, wpa_ie_len, &ndev_vif->ap.cache_wpa_ie, &ndev_vif->ap.wpa_ie_len), r, exit_with_vif);
	}

	wmm_ie_pos = slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WMM, settings->tail, settings->tail_len);
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
#ifdef CONFIG_SCSC_ENABLE_P2P
	if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO) {
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
#ifdef CONFIG_SCSC_ENABLE_P2P
	} else if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO) {
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
exit_with_start_stop_mutex:
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return r;
}

int slsi_stop_ap(void *priv)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netdev_vif *ndev_vif;
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

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
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

	netif_set_link_down(dev);
	/* All STA related packets and info should already have been flushed */
	slsi_mlme_del_vif(sdev, dev);
	slsi_vif_deactivated(sdev, dev);
	ndev_vif->ipaddress = 0;

#ifdef CONFIG_SCSC_ENABLE_P2P
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
	struct slsi_t20_drv *drv = priv;
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

void *slsi_t20_init(void *ctx, const char *ifname, void *global_priv)
{
	struct slsi_t20_drv *drv;
	struct slsi_dev *sdev;
	struct netif *dev;
	int strlen = 0;

	SLSI_DBG1_NODEV(SLSI_T20_80211, "Init SLSI driver\n");

	drv = kmm_zalloc(sizeof(struct slsi_t20_drv));
	if (!drv) {
		SLSI_ERR_NODEV("Failed to allocate drv\n");
		return NULL;
	}

	drv->global = global_priv;
	drv->ctx = ctx;
	drv->sdev = cm_ctx.sdev;
	sdev = cm_ctx.sdev;
	for (strlen = 0; strlen < IFNAMSIZ + 1; strlen++) {
		drv->ifname[strlen] = ifname[strlen];
	}
	drv->ifname[IFNAMSIZ] = '\0';
	drv->capa.flags = sdev->flags;
	drv->capa.key_mgmt = WPA_DRIVER_CAPA_KEY_MGMT_WPA | WPA_DRIVER_CAPA_KEY_MGMT_WPA_PSK | WPA_DRIVER_CAPA_KEY_MGMT_WPA2 | WPA_DRIVER_CAPA_KEY_MGMT_WPA2_PSK;

	drv->capa.auth = WPA_DRIVER_AUTH_OPEN | WPA_DRIVER_AUTH_SHARED;

	drv->capa.flags |= WPA_DRIVER_FLAGS_SANE_ERROR_CODES;
	drv->capa.flags |= WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC_DONE;
	drv->capa.flags |= WPA_DRIVER_FLAGS_EAPOL_TX_STATUS;

	drv->capa.enc = WPA_DRIVER_CAPA_ENC_WEP40 | WPA_DRIVER_CAPA_ENC_WEP104 | WPA_DRIVER_CAPA_ENC_TKIP | WPA_DRIVER_CAPA_ENC_CCMP | WPA_DRIVER_CAPA_ENC_WEP128;

	drv->capa.max_remain_on_chan = 5000;
	drv->capa.wmm_ac_supported = 1;

	drv->capa.flags |= WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT;
	drv->capa.max_scan_ssids = sdev->max_scan_ssids;
	drv->capa.max_sched_scan_ssids = sdev->max_sched_scan_ssids;
	drv->capa.sched_scan_supported = sdev->sched_scan_supported;
	drv->capa.max_match_sets = sdev->max_match_sets;
	drv->capa.max_remain_on_chan = sdev->max_remain_on_chan;
	drv->capa.max_stations = sdev->max_stations;
	drv->capa.probe_resp_offloads = sdev->probe_resp_offloads;
	drv->capa.max_acl_mac_addrs = sdev->max_acl_mac_addrs;

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		kmm_free(drv);
		return NULL;
	}

	netdev_ifup(dev);

	/* Free drv context if driver start was unsuccessful (interface is not up) */
	if ((dev->d_flags & IFF_UP) == 0) {
		kmm_free(drv);
		return NULL;
	}

	/* Update sdev with driver context sent to supplicant */
	sdev->drv = drv;
	sdev->is_supplicant_deinit = 0;

	return drv;
}

int slsi_t20_deinit(void *priv)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;

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

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return 0;
	}

	/* Set flag to indicate driver that ifdown is triggered by supplicant */
	sdev->is_supplicant_deinit = 1;

	/* The drv context in supplicant is freed by ifdown handler */
	netdev_ifdown(dev);

	/* Free the drv context allocated during init */
	kmm_free(drv);
	drv = NULL;
	sdev->drv = NULL;

	/* Reset flag */
	sdev->is_supplicant_deinit = 0;

	return 0;
}

int slsi_t20_set_param(void *priv, const char *param)
{
	return 0;
}

struct hostapd_hw_modes *slsi_get_hw_feature_data(void *priv, u16 *num_modes, u16 *flags)
{
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct slsi_80211_supported_band *supp_band;
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
	struct slsi_t20_drv *drv = priv;

	memcpy(capa, &drv->capa, sizeof(*capa));
	if (drv->capa.extended_capa && drv->capa.extended_capa_mask) {
		capa->extended_capa = drv->capa.extended_capa;
		capa->extended_capa_mask = drv->capa.extended_capa_mask;
		capa->extended_capa_len = drv->capa.extended_capa_len;
	}

	return 0;
}

static struct slsi_80211_sta_ht_cap slsi_ht_cap_2ghz = {
	.ht_supported = true,
	.cap = SLSI_80211_HT_CAP_RX_STBC | SLSI_80211_HT_CAP_SGI_20,
	.ampdu_factor = SLSI_80211_HT_MAX_AMPDU_64K,
	.ampdu_density = SLSI_80211_HT_MPDU_DENSITY_16,
	.mcs = {
		.rx_mask = {0xff, 0,},
		.rx_highest = cpu_to_le16(0),
		.tx_params = 0,
	},
};

struct slsi_80211_supported_band slsi_band_2ghz = {
	.channels = slsi_2ghz_channels,
	.band = SLSI_80211_BAND_2GHZ,
	.n_channels = ARRAY_SIZE(slsi_2ghz_channels),
	.bitrates = slsi_11g_rates,
	.n_bitrates = ARRAY_SIZE(slsi_11g_rates),
};

#ifdef CONFIG_SCSC_ADV_FEATURE
static struct slsi_80211_channel slsi_5ghz_channels[] = {
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

static struct slsi_80211_channel wifi_11a_rates[] = {
	RATE_LEGACY(60, 4, 0),
	RATE_LEGACY(90, 5, 0),
	RATE_LEGACY(120, 7, 0),
	RATE_LEGACY(180, 8, 0),
	RATE_LEGACY(240, 9, 0),
	RATE_LEGACY(360, 10, 0),
	RATE_LEGACY(480, 11, 0),
	RATE_LEGACY(540, 12, 0),
};

struct slsi_80211_supported_band slsi_band_5ghz = {
	.channels = slsi_5ghz_channels,
	.band = SLSI_80211_BAND_5GHZ,
	.n_channels = ARRAY_SIZE(slsi_5ghz_channels),
	.bitrates = wifi_11a_rates,
	.n_bitrates = ARRAY_SIZE(wifi_11a_rates),
};

static struct ieee80211_regdomain slsi_regdomain = {
	.reg_rules = {
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
		REG_RULE(0, 0, 0, 0, 0, 0),
	}
};
#endif							/* CONFIG_SCSC_ADV_FEATURE */

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
//  sdev->flags |= WPA_DRIVER_FLAGS_BSS_SELECTION;
	sdev->flags |= WPA_DRIVER_FLAGS_PROBE_RESP_OFFLOAD;

#ifndef CONFIG_SCSC_WLAN_STA_ONLY
	sdev->flags |= WPA_DRIVER_FLAGS_AP_UAPSD;
	sdev->flags |= WPA_DRIVER_FLAGS_INACTIVITY_TIMER;
#ifdef CONFIG_SCSC_ENABLE_P2P
	sdev->flags |= WPA_DRIVER_FLAGS_DEDICATED_P2P_DEVICE;
	sdev->flags |= WPA_DRIVER_FLAGS_P2P_CAPABLE;
	sdev->flags |= WPA_DRIVER_FLAGS_P2P_CONCURRENT;
	sdev->flags |= WPA_DRIVER_FLAGS_P2P_DEDICATED_INTERFACE;
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

//  sdev->device_config.domain_info.regdomain = &slsi_regdomain;

	sdev->max_remain_on_chan = 5000;	/* 5000 msec */

	/* Basic scan parameters */
	sdev->max_scan_ssids = 10;

#ifndef CONFIG_SCSC_WLAN_STA_ONLY
	/* P2P flags */
#ifdef CONFIG_SCSC_ENABLE_P2P
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
	struct slsi_t20_drv *drv = priv;
	struct slsi_dev *sdev;
	struct netif *dev;
	char alpha2_rev[4];
	int status = 0;

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

	return status;
}

ssize_t slsi_get_country(void *priv, char *country_code)
{
	struct slsi_t20_drv *drv = priv;
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

	memcpy(buf, sdev->device_config.domain_info.alpha2, 3);
	len += 3;
	buf[3] = '\0';
	len++;

	memcpy(country_code, buf, len);

	return 1;
}
