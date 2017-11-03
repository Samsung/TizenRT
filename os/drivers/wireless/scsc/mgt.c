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
#include "mgt.h"
#include "debug_scsc.h"
#include "mlme.h"
#include "netif.h"
#include "utils_scsc.h"
#include "udi.h"
#include "log_clients.h"
#include "hip.h"
#include "wpa_driver_copy.h"
#include "mib.h"
#include "hydra.h"
#include "unifiio.h"
#include "ba.h"
#include "scsc_wifi_fcq.h"
#include "t20_ops.h"

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#define CSR_WIFI_SME_MIB2_HOST_PSID_MASK    0x8000
#define SLSI_DEFAULT_HW_MAC_ADDR    "\x00\x00\x0F\x11\x22\x33"
#define MX_WLAN_FILE_PATH_LEN_MAX (128)
#define SLSI_MIB_REG_RULES_MAX (50)
#define SLSI_REG_PARAM_START_INDEX (1)

static int slsi_mib_download_file(struct slsi_dev *sdev, char *mib_file_name, bool optional);

static int slsi_mib_initial_get(struct slsi_dev *sdev);

bool is_multicast_ether_addr(const u8 *addr)
{
	u16 a = *(const u16 *)addr;

	return 0x01 & a;
}

void slsi_get_hw_mac_address(struct slsi_dev *sdev, u8 *addr)
{
	up_wlan_get_mac_addr(addr, ETH_ALEN);

	if ((addr[0] == 0) && (addr[1] == 0) && (addr[2] == 0) && (addr[3] == 0) && (addr[4] == 0) && (addr[5] == 0)) {
		SLSI_ETHER_COPY(addr, (u8 *)SLSI_DEFAULT_HW_MAC_ADDR);
		SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "Using default MAC ADDR: " SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(addr));
	}
}

static void slsi_disconnected(struct netdev_vif *ndev_vif, u16 reason, const u8 *ie, size_t ie_len)
{
	union wpa_event_data data;

	SLSI_NET_DBG1(dev, SLSI_MLME, "Disconnect event\n");

	memset(&data, 0, sizeof(data));
	if (reason) {
		data.deauth_info.reason_code = reason;
	}
	data.deauth_info.locally_generated = 1;
	wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_DEAUTH, &data);
}

static void slsi_sta_disconnected(struct netdev_vif *ndev_vif, u8 *addr, u16 reason)
{
	union wpa_event_data data;

	SLSI_INFO_NODEV("STA Disconnect event\n");

	memset(&data, 0, sizeof(data));

	data.disassoc_info.addr = addr;
	if (reason) {
		data.disassoc_info.reason_code = reason;
	} else {
		data.disassoc_info.locally_generated = 1;
	}

	wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_DISASSOC, &data);
}

int slsi_start(struct slsi_dev *sdev)
{
	int err = 0;
	u8 alpha2[3];

#ifdef CONFIG_SCSC_ADV_FEATURE
	u16 index;
#endif

	if (WARN_ON(sdev == NULL)) {
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);

	if (sdev->device_state != SLSI_DEVICE_STATE_STOPPED) {
		SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "Device already started: device_state:%d\n", sdev->device_state);
		goto done;
	}
	sdev->device_state = SLSI_DEVICE_STATE_STARTING;
	sdev->recovery_status = 0;

	reinit_completion(&sdev->sig_wait.completion);

	SLSI_INFO(sdev, "Step [1/2]: Start WLAN service\n");
	SLSI_EC_GOTO(slsi_sm_wlan_service_open(sdev), err, err_done);
	SLSI_EC_GOTO(slsi_sm_wlan_service_start(sdev), err, err_done);

	/**
	 * Download MIB data, if any.
	 * Get f/w capabilities and default configuration
	 * configure firmware
	 */
	SLSI_INFO(sdev, "Step [2/2]: Start MIB configuration\n");
	SLSI_EC_GOTO(slsi_mib_initial_get(sdev), err, err_hip_started);
	SLSI_INFO(sdev, "=== Version info from the [MIB] ===\n");
	SLSI_INFO(sdev, "HW Version : 0x%.4X (%u)\n", sdev->chip_info_mib.chip_version, sdev->chip_info_mib.chip_version);
	SLSI_INFO(sdev, "FW Build   : 0x%.8X (%u)\n", sdev->chip_info_mib.fw_build_id, sdev->chip_info_mib.fw_build_id);
	SLSI_INFO(sdev, "FW Patch   : 0x%.8X (%u)\n", sdev->chip_info_mib.fw_patch_id, sdev->chip_info_mib.fw_patch_id);
	slsi_hydra_get_chip_info(&sdev->chip_info);
	if (sdev->chip_info.populated) {
		SLSI_INFO(sdev, "=== Version info from [Hydra] ===\n");
		SLSI_INFO(sdev, "FW ROM     : 0x%.8X (%u)\n", sdev->chip_info.fw_rom_ver, sdev->chip_info.fw_rom_ver);
		SLSI_INFO(sdev, "FW Patch   : 0x%.8X (%u)\n", sdev->chip_info.fw_patch_ver, sdev->chip_info.fw_patch_ver);
		SLSI_INFO(sdev, "FW Version : %s\n", sdev->chip_info.ver_str);
	}

	/* Get UnifiCountryList */
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	slsi_read_unifi_countrylist(sdev, SLSI_PSID_UNIFI_COUNTRY_LIST);
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);

	/* Get unifiDefaultCountry  */
	slsi_read_default_country(sdev, &alpha2[0], SLSI_PSID_UNIFI_DEFAULT_COUNTRY);
	alpha2[2] = '\0';
#ifdef CONFIG_SCSC_ADV_FEATURE
	/* unifiDefaultCountry != world_domain */
	if (sdev->device_config.domain_info.alpha2 != NULL)
		if (memcmp(sdev->device_config.domain_info.regdomain->alpha2, alpha2, 2) != 0) {
			memcpy(sdev->device_config.domain_info.regdomain->alpha2, alpha2, 2);

			/* map country to index */
			index = slsi_country_to_index(sdev->device_config.domain_info, alpha2);
			if (index)
				/* Read the regulatory params for the country based on country index */
			{
				slsi_read_regulatory_rules(sdev, SLSI_PSID_UNIFI_REGULATORY_PARAMETERS, index);
			}
		}
#endif

	/* Disable TDLS */
	slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_TDLS_ACTIVATED, 0);

	/* Set max client(s) supported */
	slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_MAX_CLIENT, SLSI_AP_PEER_CONNECTIONS_MAX);

	/* Do nothing for unifiDefaultCountry == world_domain */

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "---Driver started successfully---\n");

#ifdef CONFIG_SCSC_ENABLE_FWFAULT_LOG
	/* Enable FW fault logging */
	slsi_set_uint_mib(sdev, NULL, SLSI_PSID_UNIFI_DEBUG_ENABLE, true);
#endif

	sdev->device_state = SLSI_DEVICE_STATE_STARTED;
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);

	return err;

err_hip_started:
	SLSI_MUTEX_LOCK(sdev->hip.hip_mutex);
	slsi_hip_stop(sdev);
	SLSI_MUTEX_UNLOCK(sdev->hip.hip_mutex);
	slsi_sm_wlan_service_stop(sdev);

err_done:
	sdev->device_state = SLSI_DEVICE_STATE_STOPPED;

done:
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
	return err;
}

static void slsi_stop_chip(struct slsi_dev *sdev)
{
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(sdev->start_stop_mutex));

	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "netdev_up_count:%d device_state:%d\n", sdev->netdev_up_count, sdev->device_state);

	if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
		return;
	}

	/* Only shutdown on the last device going down. */
	if (sdev->netdev_up_count) {
		return;
	}

	sdev->device_state = SLSI_DEVICE_STATE_STOPPING;

#ifdef CONFIG_SLSI_RX_PERFORMANCE_TEST
	slsi_rx_stop_performance_test_timer(sdev);
#endif
	SLSI_MUTEX_LOCK(sdev->hip.hip_mutex);
	slsi_hip_stop(sdev);
	SLSI_MUTEX_UNLOCK(sdev->hip.hip_mutex);

	slsi_sm_wlan_service_stop(sdev);
	sdev->device_state = SLSI_DEVICE_STATE_STOPPED;

	/* Cleanup the Channel Config */
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);
	slsi_kfree_mbuf(sdev->device_config.channel_config);
	sdev->device_config.channel_config = NULL;
	sdev->mlme_blocked = false;

	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
}

void slsi_vif_cleanup(struct slsi_dev *sdev, struct netif *dev, bool hw_available)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int i;

	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "clean VIF\n");
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (ndev_vif->activated) {
		for (i = 0; i < SLSI_AP_PEER_CONNECTIONS_MAX; i++) {
			struct slsi_peer *peer = ndev_vif->peer_sta_record[i];
			if (peer && peer->valid) {
				slsi_ps_port_control(sdev, dev, peer, SLSI_STA_CONN_STATE_DISCONNECTED);
			}
		}

		if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
			bool already_disconnected = false;

			SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "Station active: hw_available=%d\n", hw_available);
			netif_set_link_down(dev);
			if (hw_available) {
				struct slsi_peer *peer = ndev_vif->peer_sta_record[SLSI_STA_PEER_QUEUESET];

				if (peer && peer->valid) {
					slsi_mlme_disconnect(sdev, dev, peer->address, FAPI_REASONCODE_UNSPECIFIED_REASON, true);
					slsi_handle_disconnect(sdev, dev, peer->address, 0);
					already_disconnected = true;
				} else {
					slsi_mlme_del_vif(sdev, dev);
				}
			}
			if (!already_disconnected) {
				SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "Calling slsi_vif_deactivated\n");
				slsi_vif_deactivated(sdev, dev);
				if (!sdev->recovery_status) {
					slsi_disconnected(ndev_vif, FAPI_REASONCODE_UNSPECIFIED_REASON, NULL, 0);
				}
			}
		} else if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
			SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "AP active\n");
			netif_set_link_down(dev);
			if (hw_available) {
				int r = 0;

				r = slsi_mlme_disconnect(sdev, dev, NULL, WLAN_REASON_DEAUTH_LEAVING, true);
				if (r != 0) {
					SLSI_NET_ERR(dev, "Disconnection returned with CFM failure\n");
				}
				slsi_mlme_del_vif(sdev, dev);
			}
			SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "Calling slsi_vif_deactivated\n");
			slsi_vif_deactivated(sdev, dev);

#ifdef CONFIG_SCSC_ENABLE_P2P
			if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO) {
				SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_NO_VIF);
			}
#endif
		}
#ifdef CONFIG_SCSC_ENABLE_P2P
		if (ndev_vif->vif_type == FAPI_VIFTYPE_UNSYNCHRONISED) {
			SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "P2P active - Deactivate\n");
			slsi_p2p_vif_deactivate(sdev, dev, hw_available);
		}
#endif
	}
}

void slsi_scan_cleanup(struct slsi_dev *sdev, struct netif *dev, bool hw_available)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int i;

	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "clean scan_data\n");

	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);
	for (i = 0; i < SLSI_SCAN_MAX; i++)
		if (ndev_vif->scan[i].scan_req && i == SLSI_SCAN_HW_ID) {
			work_cancel(SCSC_WORK, &ndev_vif->scan_timeout_work);
			if (hw_available) {
				(void)slsi_mlme_del_scan(sdev, dev, ndev_vif->ifnum << 8 | SLSI_SCAN_HW_ID);
			}
			slsi_mbuf_queue_purge(&ndev_vif->scan[i].scan_results);
			ndev_vif->scan[i].scan_req = 0;
			slsi_scan_complete(ndev_vif);
		}
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
}

void slsi_stop_net_dev_locked(struct slsi_dev *sdev, struct netif *dev, bool hw_available)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_NET_DBG1(dev, SLSI_INIT_DEINIT, "Stopping netdev_up_count=%d, hw_available = %d\n", sdev->netdev_up_count, hw_available);

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(sdev->start_stop_mutex));

	if (!ndev_vif->is_available) {
		SLSI_NET_DBG1(dev, SLSI_INIT_DEINIT, "Not Available\n");
		return;
	}

	if (WARN_ON(!sdev->netdev_up_count)) {
		SLSI_NET_DBG1(dev, SLSI_INIT_DEINIT, "sdev->netdev_up_count=%d\n", sdev->netdev_up_count);
		return;
	}

	if (!hw_available) {
		complete_all(&ndev_vif->sig_wait.completion);
	}

	slsi_scan_cleanup(sdev, dev, hw_available);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	slsi_vif_cleanup(sdev, dev, hw_available);
	ndev_vif->is_available = false;
	sdev->netdev_up_count--;
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);

	slsi_stop_chip(sdev);
}

/* Called when a net device wants to go DOWN */
void slsi_stop_net_dev(struct slsi_dev *sdev, struct netif *dev)
{
	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);
	slsi_stop_net_dev_locked(sdev, dev, sdev->recovery_status ? false : true);
	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
}

/* Called when we get sdio_removed */
void slsi_stop(struct slsi_dev *sdev)
{
	struct netif *dev;
	int i;

	SLSI_MUTEX_LOCK(sdev->start_stop_mutex);
	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "netdev_up_count:%d\n", sdev->netdev_up_count);

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	for (i = 1; i <= CONFIG_SCSC_WLAN_MAX_INTERFACES; i++) {
		dev = slsi_get_netdev_locked(sdev, i);
		if (dev) {
			slsi_stop_net_dev_locked(sdev, sdev->netdev[i], false);
		}
	}
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);

	SLSI_MUTEX_UNLOCK(sdev->start_stop_mutex);
}

/* MIB handling */
static u8 *slsi_mib_slice(struct slsi_dev *sdev, const u8 *data, u32 length, u32 *p_parsed_len, u32 *p_mib_slice_len)
{
	const u8 *p = data;
	u8 *mib_slice;
	u32 mib_slice_len = 0;

	SLSI_UNUSED_PARAMETER_NOT_DEBUG(sdev);

	if (!length) {
		return NULL;
	}

	mib_slice = kmm_malloc(length + 4);
	if (mib_slice == NULL) {
		return NULL;
	}

	while (length >= 4) {
		u16 psid = SLSI_BUFF_LE_TO_U16(p);
		u16 pslen = (u16)(4 + SLSI_BUFF_LE_TO_U16(&p[2]));

		if (pslen & 0x1) {
			pslen++;
		}

		if (psid & CSR_WIFI_SME_MIB2_HOST_PSID_MASK) {
			/* do nothing */
		} else {
			/* SLSI_ERR (sdev, "PSID=0x%04X : FW\n", psid); */
#define CSR_WIFI_HOSTIO_MIB_SET_MAX     (1800)
			if ((mib_slice_len + pslen) > CSR_WIFI_HOSTIO_MIB_SET_MAX) {
				break;
			}
			if (pslen > length + 4) {
				SLSI_ERR(sdev, "length %u read from MIB file > space %u - corrupt file?\n", pslen, length + 4);
				mib_slice_len = 0;
				break;
			}
			memcpy(&mib_slice[mib_slice_len], p, pslen);
			mib_slice_len += pslen;
		}
		p += pslen;
		length -= pslen;
	}

	*p_mib_slice_len = mib_slice_len;
	*p_parsed_len = (p - data);

	return mib_slice;
}

static int slsi_mib_download_file(struct slsi_dev *sdev, char *mib_file_name, bool optional)
{
	return 0;
}

static int slsi_mib_initial_get(struct slsi_dev *sdev)
{
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	int rxLength = 0;
	int r;
	static const struct slsi_mib_get_entry getValues[] = { { SLSI_PSID_UNIFI_FAST_POWER_SAVE_TIMEOUT, { 0, 0 } },
		{ SLSI_PSID_UNIFI_CHIP_VERSION, { 0, 0 } },
		{ SLSI_PSID_UNIFI_FIRMWARE_BUILD_ID, { 0, 0 } },
		{ SLSI_PSID_UNIFI_FIRMWARE_PATCH_BUILD_ID, { 0, 0 } },
	};

	r = slsi_mib_encode_get_list(&mibreq, sizeof(getValues) / sizeof(struct slsi_mib_get_entry), getValues);
	if (r != SLSI_MIB_STATUS_SUCCESS) {
		return -ENOMEM;
	}

	mibrsp.dataLength = 64;
	mibrsp.data = kmm_malloc(mibrsp.dataLength);
	if (mibrsp.data == NULL) {
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, NULL, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &rxLength);
	kmm_free(mibreq.data);

	if (r == 0) {
		struct slsi_mib_value *values;

		mibrsp.dataLength = (u32)rxLength;

		values = slsi_mib_decode_get_list(&mibrsp, sizeof(getValues) / sizeof(struct slsi_mib_get_entry), getValues);
		if (values == NULL) {
			SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "MIB : Mib decode failed\n");
			kmm_free(mibrsp.data);
			return -ENOMEM;
		}

		if (values[0].type != SLSI_MIB_TYPE_NONE) {
			WARN_ON(values[0].type != SLSI_MIB_TYPE_UINT);
			sdev->device_config.fast_power_save_timeout = values[0].u.uintValue / 1000;
			SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "MIB : FAST_POWER_SAVE_TIMEOUT = %d\n", sdev->device_config.fast_power_save_timeout);
		}

		if (values[1].type != SLSI_MIB_TYPE_NONE) {
			WARN_ON(values[1].type != SLSI_MIB_TYPE_UINT);
			sdev->chip_info_mib.chip_version = values[1].u.uintValue;
		}

		if (values[2].type != SLSI_MIB_TYPE_NONE) {
			WARN_ON(values[2].type != SLSI_MIB_TYPE_UINT);
			sdev->chip_info_mib.fw_build_id = values[2].u.uintValue;
		}

		if (values[3].type != SLSI_MIB_TYPE_NONE) {
			WARN_ON(values[3].type != SLSI_MIB_TYPE_UINT);
			sdev->chip_info_mib.fw_patch_id = values[3].u.uintValue;
		}
		kmm_free(values);
	}
	kmm_free(mibrsp.data);
	return r;
}

struct slsi_peer *slsi_peer_add(struct slsi_dev *sdev, struct netif *dev, u8 *peer_address, u16 aid)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer = NULL;
	u16 queueset = 0;

	if (WARN_ON(!aid)) {
		SLSI_NET_ERR(dev, "Invalid aid(0) received\n");
		return NULL;
	}
	SLSI_NET_DBG2(dev, SLSI_T20_80211, " aid received - %d\n", aid);
	queueset = MAP_AID_TO_QS(aid);

	/* MUST only be called from the control path that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));
	if (WARN_ON(!ndev_vif->activated)) {
		return NULL;
	}

	if (!peer_address) {
		SLSI_NET_WARN(dev, "Peer without address");
		return NULL;
	}

	peer = slsi_get_peer_from_mac(sdev, dev, peer_address);
	if (peer) {
		SLSI_NET_WARN(dev, "Peer (MAC:" SLSI_MAC_FORMAT ") already exists", SLSI_MAC_STR(peer_address));
		return NULL;
	}

	if (slsi_get_peer_from_qs(sdev, dev, queueset)) {
		SLSI_NET_WARN(dev, "Peer (queueset:%d) already exists", queueset);
		return NULL;
	}

	SLSI_NET_DBG2(dev, SLSI_T20_80211, SLSI_MAC_FORMAT ", aid:%d\n", SLSI_MAC_STR(peer_address), aid);

	peer = ndev_vif->peer_sta_record[queueset];
	if (peer == NULL) {
		/* If it reaches here, something has gone wrong */
		SLSI_NET_ERR(dev, "Peer (queueset:%d) is NULL", queueset);
		return NULL;
	}

	peer->aid = aid;
	peer->queueset = queueset;
	SLSI_ETHER_COPY(peer->address, peer_address);
	SLSI_NET_DBG1(dev, SLSI_MLME, " peer address trying to connect =%p \n", peer_address);
	peer->assoc_ie = NULL;
	peer->assoc_resp_ie = NULL;
	peer->connected_state = SLSI_STA_CONN_STATE_DISCONNECTED;
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	if (scsc_wifi_fcq_ctrl_q_init(&peer->ctrl_q) < 0) {
		SLSI_NET_ERR(dev, "scsc_wifi_fcq_ctrl_q_init failed\n");
		return NULL;
	}

	if (scsc_wifi_fcq_unicast_qset_init(&peer->data_qs, peer->queueset) < 0) {
		SLSI_NET_ERR(dev, "scsc_wifi_fcq_unicast_qset_init failed\n");
		scsc_wifi_fcq_ctrl_q_deinit(&peer->ctrl_q);
		return NULL;
	}
#endif

	/* A peer is only valid once all the data is initialised
	 * otherwise a process could check the flag and start to read
	 * uninitialised data.
	 */
	ndev_vif->peer_sta_records++;

	mbuf_queue_head_init(&peer->buffered_frames);

	peer->valid = true;

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "created station peer " SLSI_MAC_FORMAT " AID:%d\n", SLSI_MAC_STR(peer->address), aid);
	return peer;
}

static bool slsi_search_ies_for_qos_indicators(struct slsi_dev *sdev, u8 *ies, int ies_len)
{
	SLSI_UNUSED_PARAMETER_NOT_DEBUG(sdev);

	if (slsi_80211_find_ie(WLAN_EID_HT_CAPABILITY, ies, ies_len)) {
		SLSI_DBG1(sdev, SLSI_T20_80211, "QOS enabled due to WLAN_EID_HT_CAPABILITY\n");
		return true;
	}
	if (slsi_80211_find_ie(WLAN_EID_VHT_CAPABILITY, ies, ies_len)) {
		SLSI_DBG1(sdev, SLSI_T20_80211, "QOS enabled due to WLAN_EID_VHT_CAPABILITY\n");
		return true;
	}
	if (slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WMM, ies, ies_len)) {
		SLSI_DBG1(sdev, SLSI_T20_80211, "QOS enabled due to WLAN_OUI_TYPE_MICROSOFT_WMM\n");
		return true;
	}
	return false;
}

void slsi_peer_update_assoc_req(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u16 id = fapi_get_u16(mbuf, id);

	/* MUST only be called from the control path that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	switch (id) {
	case MLME_CONNECTED_IND:
	case MLME_PROCEDURE_STARTED_IND:
		if (WARN_ON(ndev_vif->vif_type != FAPI_VIFTYPE_AP && ndev_vif->vif_type != FAPI_VIFTYPE_STATION)) {
			slsi_kfree_mbuf(mbuf);
			return;
		}
		break;
	default:
		slsi_kfree_mbuf(mbuf);
		WARN_ON(1);
		return;
	}

	slsi_kfree_mbuf(peer->assoc_ie);
	peer->assoc_ie = NULL;
	peer->capabilities = 0;

	if (fapi_get_datalen(mbuf)) {
		int mgmt_hdr_len;
		struct slsi_80211_mgmt *mgmt = fapi_get_mgmt(mbuf);
		/* Update the mbuf to just point to the frame */
		mbuf_pull(mbuf, fapi_get_siglen(mbuf));

		if (slsi_80211_is_assoc_req(mgmt->frame_control)) {
			mgmt_hdr_len = (mgmt->u.assoc_req.variable - (u8 *)mgmt);
			if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
				peer->capabilities = le16_to_cpu(mgmt->u.assoc_req.capab_info);
			}
		} else if (slsi_80211_is_reassoc_req(mgmt->frame_control)) {
			mgmt_hdr_len = (mgmt->u.reassoc_req.variable - (u8 *)mgmt);
			if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
				peer->capabilities = le16_to_cpu(mgmt->u.reassoc_req.capab_info);
			}
		} else {
			WARN_ON(1);
			slsi_kfree_mbuf(mbuf);
			return;
		}

		mbuf_pull(mbuf, mgmt_hdr_len);

		peer->assoc_ie = mbuf;
		peer->qos_enabled = slsi_search_ies_for_qos_indicators(sdev, slsi_mbuf_get_data(mbuf), mbuf->data_len);
	}
}

void slsi_peer_update_assoc_rsp(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer, struct max_buff *mbuf)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u16 id = fapi_get_u16(mbuf, id);

	SLSI_UNUSED_PARAMETER(sdev);

	/* MUST only be called from the control path that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (ndev_vif->vif_type != FAPI_VIFTYPE_STATION) {
		goto exit_with_warnon;
	}

	if (id != MLME_CONNECT_IND) {
		SLSI_NET_ERR(dev, "Unexpected id =0x%4x\n", id);
		goto exit_with_warnon;
	}

	slsi_kfree_mbuf(peer->assoc_resp_ie);
	peer->assoc_resp_ie = NULL;
	peer->capabilities = 0;
	if (fapi_get_datalen(mbuf)) {
		int mgmt_hdr_len;
		struct slsi_80211_mgmt *mgmt = fapi_get_mgmt(mbuf);

		/* Update the mbuf to just point to the frame */
		mbuf_pull(mbuf, fapi_get_siglen(mbuf));

		if (slsi_80211_is_assoc_resp(mgmt->frame_control)) {
			mgmt_hdr_len = (mgmt->u.assoc_resp.variable - (u8 *)mgmt);
			peer->capabilities = le16_to_cpu(mgmt->u.assoc_resp.capab_info);
		} else if (slsi_80211_is_reassoc_resp(mgmt->frame_control)) {
			mgmt_hdr_len = (mgmt->u.reassoc_resp.variable - (u8 *)mgmt);
			peer->capabilities = le16_to_cpu(mgmt->u.reassoc_resp.capab_info);
		} else {
			goto exit_with_warnon;
		}

		mbuf_pull(mbuf, mgmt_hdr_len);
		peer->assoc_resp_ie = mbuf;
	}
	return;

exit_with_warnon:
	WARN_ON(1);
	slsi_kfree_mbuf(mbuf);
}

int slsi_peer_remove(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *buff_frame;

	SLSI_UNUSED_PARAMETER(sdev);

	/* MUST only be called from the control path that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (!peer) {
		SLSI_NET_WARN(dev, "peer=NULL");
		return -EINVAL;
	}

	SLSI_NET_DBG2(dev, SLSI_T20_80211, SLSI_MAC_FORMAT "\n", SLSI_MAC_STR(peer->address));

	/* Take the peer lock to protect the transmit data path
	 * when accessing peer records.
	 */
	SLSI_MUTEX_LOCK(ndev_vif->peer_lock);

	/* The information is no longer valid so first update the flag to ensure that
	 * another process doesn't try to use it any more.
	 */
	peer->valid = false;
	peer->connected_state = SLSI_STA_CONN_STATE_DISCONNECTED;

	ndev_vif->peer_sta_records--;

	SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);

	buff_frame = slsi_mbuf_dequeue(&peer->buffered_frames);
	while (buff_frame) {
		SLSI_NET_DBG3(dev, SLSI_MLME, "FLUSHING BUFFERED FRAMES\n");
		slsi_kfree_mbuf(buff_frame);
		buff_frame = slsi_mbuf_dequeue(&peer->buffered_frames);
	}

	slsi_rx_ba_stop_all(dev, peer);

#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	scsc_wifi_fcq_qset_deinit(&peer->data_qs);
	scsc_wifi_fcq_ctrl_q_deinit(&peer->ctrl_q);
#endif

	slsi_kfree_mbuf(peer->assoc_ie);
	slsi_kfree_mbuf(peer->assoc_resp_ie);
	memset(peer, 0x00, sizeof(*peer));

	return 0;
}

int slsi_vif_activated(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_UNUSED_PARAMETER(sdev);

	/* MUST only be called from the control path that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	/* MUST have cleared any peer records previously */
	WARN_ON(ndev_vif->peer_sta_records);

	if (WARN_ON(ndev_vif->activated)) {
		return -EALREADY;
	}
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
		/* Enable the Multicast queue set for AP mode */
		if (scsc_wifi_fcq_multicast_qset_init(&ndev_vif->ap.group_data_qs) < 0) {
			return -EFAULT;
		}
	}
#endif

	ndev_vif->peer_sta_records = 0;
	ndev_vif->activated = true;
#ifdef CONFIG_SCSC_ENABLE_P2P
	ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
#endif
	return 0;
}

void slsi_vif_deactivated(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int i;

	/* MUST only be called from the control path that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	/**
	 * The station type VIF is deactivated when the AP connection is lost.
	 * Disable delayed ARP here as it will be enabled on connection.
	 */
	if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
		ndev_vif->sta.group_key_set = false;
		ndev_vif->sta.vif_status = SLSI_VIF_STATUS_UNSPECIFIED;
	}

	/* MUST be done first to ensure that other code doesn't treat the VIF as still active */
	ndev_vif->activated = false;

	for (i = 0; i < (SLSI_AP_PEER_CONNECTIONS_MAX); i++) {
		struct slsi_peer *peer = ndev_vif->peer_sta_record[i];

		if (peer && peer->valid) {
			slsi_peer_remove(sdev, dev, peer);
		}
	}
#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
		scsc_wifi_fcq_qset_deinit(&ndev_vif->ap.group_data_qs);
	}
#endif

	if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
		/* Free WPA and WMM IEs if present */
		slsi_clear_cached_ies(&ndev_vif->ap.cache_wpa_ie, &ndev_vif->ap.wpa_ie_len);
		slsi_clear_cached_ies(&ndev_vif->ap.cache_wmm_ie, &ndev_vif->ap.wmm_ie_len);

		if (ndev_vif->chandef) {
			kmm_free(ndev_vif->chandef);
		}
	}
#ifdef CONFIG_SCSC_ENABLE_P2P
	if ((ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_CLIENT) || (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO)) {
		SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_NO_VIF);
		sdev->p2p_group_exp_frame = SLSI_P2P_PA_INVALID;
	}
	ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
#endif

	/* MUST be done last as lots of code is dependent on checking the vif_type */
	ndev_vif->vif_type = SLSI_VIFTYPE_UNSPECIFIED;
	ndev_vif->iftype = SLSI_80211_IFTYPE_UNSPECIFIED;

	/* SHOULD have cleared any peer records */
	WARN(ndev_vif->peer_sta_records, "vif:%d, peer_sta_records:%d", ndev_vif->ifnum, ndev_vif->peer_sta_records);

	sdev->device_config.qos_info = 0;
}

int slsi_handle_disconnect(struct slsi_dev *sdev, struct netif *dev, u8 *peer_address, u16 reason)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer = NULL;

	if (WARN_ON(!dev)) {
		goto exit;
	}

	SLSI_NET_DBG3(dev, SLSI_MLME, "slsi_handle_disconnect(vif:%d, MAC:" SLSI_MAC_FORMAT ")\n", ndev_vif->ifnum, SLSI_MAC_STR(peer_address));

	/* MUST only be called from somewhere that has acquired the lock */
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (!ndev_vif->activated) {
		SLSI_NET_DBG1(dev, SLSI_MLME, "VIF not activated\n");
		goto exit;
	}

	peer = slsi_get_peer_from_mac(sdev, dev, peer_address);
	if (!peer) {
		SLSI_NET_DBG1(dev, SLSI_MLME, "peer NOT found by MAC address\n");
		goto exit;
	}

	switch (ndev_vif->vif_type) {
	case FAPI_VIFTYPE_STATION: {
		/* MLME-DISCONNECT-IND could indicate the completion of a MLME-DISCONNECT-REQ or
		 * the connection with the AP has been lost
		 */
		if (ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTING) {
			SLSI_NET_DBG1(dev, SLSI_MLME, "Unexpected mlme_disconnect_ind - whilst connecting\n");
		} else if (ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED || (ndev_vif->sta.vif_status == SLSI_VIF_STATUS_DISCONNECTING)) {
			/* Change keep alive and sync_loss reason code while sending to supplicant to a standard reason code */
			if (reason == FAPI_REASONCODE_KEEP_ALIVE_FAILURE || reason == FAPI_REASONCODE_SYNCHRONISATION_LOSS) {
				reason = WLAN_REASON_DEAUTH_LEAVING;
			}

			if (ndev_vif->sta.is_wps && ndev_vif->sta.vif_status == SLSI_VIF_STATUS_CONNECTED)
				/* Ignore sending deauth or disassoc event to cfg80211 during WPS session */
			{
				SLSI_NET_INFO(dev, "Ignoring Deauth notification to cfg80211 from the peer during WPS procedure\n");
			} else {
				slsi_disconnected(ndev_vif, reason, NULL, 0);
			}
			SLSI_NET_DBG3(dev, SLSI_MLME, "Completion of disconnect from AP\n");
		}

		ndev_vif->sta.is_wps = false;

		/* Delayed ARP only needs to run when connected. */

		netif_set_link_down(dev);
		slsi_mlme_del_vif(sdev, dev);
		slsi_vif_deactivated(sdev, dev);
		break;
	}
	case FAPI_VIFTYPE_AP: {
		u8 addr[ETH_ALEN];

		memcpy(addr, peer->address, (sizeof(u8) * ETH_ALEN));

		if ((peer->connected_state == SLSI_STA_CONN_STATE_CONNECTED)
			|| (peer->connected_state == SLSI_STA_CONN_STATE_DOING_KEY_CONFIG)) {
			slsi_peer_remove(sdev, dev, peer);
			slsi_sta_disconnected(ndev_vif, addr, reason);
		}

		break;
	}
	default:
		SLSI_NET_WARN(dev, "mlme_disconnect_ind(vif:%d, unexpected vif type:%d)\n", ndev_vif->ifnum, ndev_vif->vif_type);
		break;
	}
exit:
	return 0;
}

int slsi_ps_port_control(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer, enum slsi_sta_conn_state s)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_UNUSED_PARAMETER(sdev);

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	switch (s) {
	case SLSI_STA_CONN_STATE_DISCONNECTED:
		SLSI_NET_DBG1(dev, SLSI_TX, "STA disconnected, SET : FCQ - Disabled\n");
		peer->authorized = false;
		if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
			(void)scsc_wifi_fcq_8021x_port_state(dev, &ndev_vif->ap.group_data_qs, SCSC_WIFI_FCQ_8021x_STATE_BLOCKED);
		}
		return scsc_wifi_fcq_8021x_port_state(dev, &peer->data_qs, SCSC_WIFI_FCQ_8021x_STATE_BLOCKED);
		break;
	case SLSI_STA_CONN_STATE_DOING_KEY_CONFIG:
		SLSI_NET_DBG1(dev, SLSI_TX, "STA doing KEY config, SET : FCQ - Disabled\n");
		peer->authorized = false;
		if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
			(void)scsc_wifi_fcq_8021x_port_state(dev, &ndev_vif->ap.group_data_qs, SCSC_WIFI_FCQ_8021x_STATE_BLOCKED);
		}
		return scsc_wifi_fcq_8021x_port_state(dev, &peer->data_qs, SCSC_WIFI_FCQ_8021x_STATE_BLOCKED);
		break;
	case SLSI_STA_CONN_STATE_CONNECTED:
		SLSI_NET_DBG1(dev, SLSI_TX, "STA connected, SET : FCQ - Enabled\n");
		peer->authorized = true;
		if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
			(void)scsc_wifi_fcq_8021x_port_state(dev, &ndev_vif->ap.group_data_qs, SCSC_WIFI_FCQ_8021x_STATE_OPEN);
		}
		return scsc_wifi_fcq_8021x_port_state(dev, &peer->data_qs, SCSC_WIFI_FCQ_8021x_STATE_OPEN);
		break;
	default:
		SLSI_NET_DBG1(dev, SLSI_TX, "SET : FCQ - Disabled\n");
		peer->authorized = false;
		if (ndev_vif->vif_type == FAPI_VIFTYPE_AP) {
			(void)scsc_wifi_fcq_8021x_port_state(dev, &ndev_vif->ap.group_data_qs, SCSC_WIFI_FCQ_8021x_STATE_BLOCKED);
		}
		return scsc_wifi_fcq_8021x_port_state(dev, &peer->data_qs, SCSC_WIFI_FCQ_8021x_STATE_BLOCKED);
		break;
	}

	return 0;
}

int slsi_set_uint_mib(struct slsi_dev *sdev, struct netif *dev, u16 psid, int value)
{
	struct slsi_mib_data mib_data = { 0, NULL };
	int r = 0;

	SLSI_DBG2(sdev, SLSI_MLME, "UINT MIB Set Request (PSID = 0x%04X, Value = %d)\n", psid, value);

	if (slsi_mib_encode_uint(&mib_data, psid, value, 0) == SLSI_MIB_STATUS_SUCCESS)
		if (mib_data.dataLength) {
			r = slsi_mlme_set(sdev, dev, mib_data.data, mib_data.dataLength);
			if (r != 0) {
				SLSI_ERR(sdev, "MIB (PSID = 0x%04X) set error = %d\n", psid, r);
			}
			kmm_free(mib_data.data);
		}

	return r;
}

const u8 addr_mask[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static void slsi_create_packet_filter_element(u8 filterid, u8 pkt_filter_mode, u8 num_pattern_desc, struct slsi_mlme_pattern_desc *pattern_desc, struct slsi_mlme_pkt_filter_elem *pkt_filter_elem, u8 *pkt_filters_len)
{
	u8 pkt_filter_hdr[SLSI_PKT_FILTER_ELEM_HDR_LEN] = { 0xdd,	/* vendor ie */
														0x00,					/*Length to be filled */
														0x00, 0x16, 0x32,		/*oui */
														0x02,
														filterid,				/*filter id to be filled */
														pkt_filter_mode			/* pkt filter mode to be filled */
													  };
	u8 i, pattern_desc_len = 0;

	WARN_ON(num_pattern_desc > SLSI_MAX_PATTERN_DESC);

	memcpy(pkt_filter_elem->header, pkt_filter_hdr, SLSI_PKT_FILTER_ELEM_HDR_LEN);
	pkt_filter_elem->num_pattern_desc = num_pattern_desc;

	for (i = 0; i < num_pattern_desc; i++) {
		memcpy(&pkt_filter_elem->pattern_desc[i], &pattern_desc[i], sizeof(struct slsi_mlme_pattern_desc));
		pattern_desc_len += SLSI_PKT_DESC_FIXED_LEN + (2 * pattern_desc[i].mask_length);
	}

	/*Update the length in the header */
	pkt_filter_elem->header[1] = SLSI_PKT_FILTER_ELEM_FIXED_LEN + pattern_desc_len;
	*pkt_filters_len += (SLSI_PKT_FILTER_ELEM_HDR_LEN + pattern_desc_len);

	SLSI_DBG3_NODEV(SLSI_MLME, "filterid=0x%x,pkt_filter_mode=0x%x,num_pattern_desc=0x%x\n", filterid, pkt_filter_mode, num_pattern_desc);
}

int slsi_set_multicast_packet_filters(struct slsi_dev *sdev, struct netif *dev, const u8 *addr)
{
	struct slsi_mlme_pattern_desc pattern_desc;
	u8 pkt_filters_len = 0, num_filters = 0;
	int ret = 0;
	struct slsi_mlme_pkt_filter_elem *pkt_filter_elem = NULL;
	u8 mdns_mask[6] = { 0x01, 0x00, 0x5E, 0x00, 0x00, 0xFB };

	pkt_filter_elem = kmm_malloc(2 * sizeof(struct slsi_mlme_pkt_filter_elem));
	if (!pkt_filter_elem) {
		SLSI_NET_ERR(dev, "ERROR Memory allocation failure\n");
		return -ENOMEM;
	}
	/*Opt in the multicast packet from addr */
	pattern_desc.offset = 0;
	pattern_desc.mask_length = ETH_ALEN;
	SLSI_ETHER_COPY(pattern_desc.mask, addr_mask);
	SLSI_ETHER_COPY(pattern_desc.pattern, addr);

	slsi_create_packet_filter_element(SLSI_REGD_MC_FILTER_ID, FAPI_PACKETFILTERMODE_OPT_IN, 1, &pattern_desc, &pkt_filter_elem[num_filters], &pkt_filters_len);
	num_filters++;
	/*Opt out all IPv4 multicast */
	pattern_desc.offset = 0;
	pattern_desc.mask_length = 3;	/* prefix len */
	memcpy(pattern_desc.mask, addr_mask, 3);
	memcpy(pattern_desc.pattern, mdns_mask, 3);	/*prefix for multicast is same as that for mDns */

	slsi_create_packet_filter_element(SLSI_ALL_IPv4_MC_FILTER_ID, FAPI_PACKETFILTERMODE_OPT_OUT, 1, &pattern_desc, &pkt_filter_elem[num_filters], &pkt_filters_len);
	num_filters++;

	ret = slsi_mlme_set_packet_filter(sdev, dev, pkt_filters_len, num_filters, pkt_filter_elem);
	kmm_free(pkt_filter_elem);

	return ret;
}

static int slsi_clear_packet_filters(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer = slsi_get_peer_from_qs(sdev, dev, SLSI_STA_PEER_QUEUESET);
	u8 pkt_filters_len = 0;
	int num_filters = 0;
	int ret = 0;
	struct slsi_mlme_pkt_filter_elem *pkt_filter_elem;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (WARN_ON(ndev_vif->vif_type != FAPI_VIFTYPE_STATION)) {
		return -EINVAL;
	}

	if (WARN_ON(!peer)) {
		return -EINVAL;
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "Clear filters");

	/*Number of filters packet from addr & IPv4 multicast */
	pkt_filter_elem = kmm_malloc((2 * sizeof(struct slsi_mlme_pkt_filter_elem)));
	if (!pkt_filter_elem) {
		SLSI_NET_ERR(dev, "ERROR Memory allocation failure");
		return -ENOMEM;
	}

	slsi_create_packet_filter_element(SLSI_REGD_MC_FILTER_ID, 0, 0, NULL, &pkt_filter_elem[num_filters], &pkt_filters_len);
	num_filters++;
	slsi_create_packet_filter_element(SLSI_ALL_IPv4_MC_FILTER_ID, 0, 0, NULL, &pkt_filter_elem[num_filters], &pkt_filters_len);
	num_filters++;

	ret = slsi_mlme_set_packet_filter(sdev, dev, pkt_filters_len, num_filters, pkt_filter_elem);
	kmm_free(pkt_filter_elem);
	return ret;
}

/* The mbuf needs to be freed after transmit (success or failure) */
int slsi_send_gratuitous_arp(struct slsi_dev *sdev, struct netif *dev)
{
	int ret = 0;
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *arp;
	struct ethhdr *ehdr;
	static const u8 arp_hdr[] = { 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01 };
	int arp_size = sizeof(arp_hdr) + ETH_ALEN + sizeof(ndev_vif->ipaddress) + ETH_ALEN + sizeof(ndev_vif->ipaddress);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "\n");

	if (!ndev_vif->ipaddress) {
		return 0;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (WARN_ON(!ndev_vif->activated)) {
		return -EINVAL;
	}
	if (WARN_ON(ndev_vif->vif_type != FAPI_VIFTYPE_STATION)) {
		return -EINVAL;
	}
	if (WARN_ON(ndev_vif->sta.vif_status != SLSI_VIF_STATUS_CONNECTED)) {
		return -EINVAL;
	}

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "IP:%pI4\n", &ndev_vif->ipaddress);

	arp = slsi_mbuf_alloc(sizeof(struct ethhdr) + arp_size);
	if (WARN_ON(!arp)) {
		return -ENOMEM;
	}

	/* The Ethernet header is accessed in the stack. */
	mbuf_set_mac_header(arp, 0);

	/* Ethernet Header */
	ehdr = (struct ethhdr *)mbuf_put(arp, sizeof(struct ethhdr));
	memset(ehdr->h_dest, 0xFF, ETH_ALEN);
	SLSI_ETHER_COPY(ehdr->h_source, dev->d_mac.ether_addr_octet);
	ehdr->h_proto = cpu_to_be16(ETH_P_ARP);

	/* Arp Data */
	memcpy(mbuf_put(arp, sizeof(arp_hdr)), arp_hdr, sizeof(arp_hdr));
	SLSI_ETHER_COPY(mbuf_put(arp, ETH_ALEN), dev->d_mac.ether_addr_octet);
	memcpy(mbuf_put(arp, sizeof(ndev_vif->ipaddress)), &ndev_vif->ipaddress, sizeof(ndev_vif->ipaddress));
	memset(mbuf_put(arp, ETH_ALEN), 0xFF, ETH_ALEN);
	memcpy(mbuf_put(arp, sizeof(ndev_vif->ipaddress)), &ndev_vif->ipaddress, sizeof(ndev_vif->ipaddress));
	arp->ac_queue = slsi_netif_get_peer_queue(0, 0); /* Queueset 0 AC 0 */

	ret = slsi_tx_data(sdev, dev, arp);
	slsi_kfree_mbuf(arp);

	return ret;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
#define SLSI_AP_AUTO_CHANLS_LIST_FROM_HOSTAPD_MAX 3

int slsi_auto_chan_select_scan(struct slsi_dev *sdev, int n_channels, struct slsi_80211_channel *channels[])
{
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	struct max_buff_head unique_scan_results;
	int scan_result_count[SLSI_AP_AUTO_CHANLS_LIST_FROM_HOSTAPD_MAX] = { 0, 0, 0 };
	int i, j;
	int r = 0;
	int selected_index = 0;
	int min_index = 0;
	u32 freqdiff = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_WARN(sdev, "not supported in WlanLite mode\n");
		return -EOPNOTSUPP;
	}

	mbuf_queue_head_init(&unique_scan_results);

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);	/* use the main VIF */
	if (!dev) {
		r = -EINVAL;
		return r;
	}

	SLSI_NET_DBG1(dev, SLSI_T20_80211, "number of Channels %d\n", n_channels);

	ndev_vif = netdev_priv(dev);
	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

	if (ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req) {
		r = -EBUSY;
		goto exit_with_vif;
	}
	r = slsi_mlme_add_scan(sdev, dev, FAPI_SCANTYPE_AP_AUTO_CHANNEL_SELECTION, FAPI_REPORTMODE_REAL_TIME, 0,	/* n_ssids */
						   NULL,	/* ssids */
						   n_channels, channels, NULL, NULL,	/* ie */
						   0,	/* ie_len */
						   true);

	if (r == 0) {
		struct max_buff *unique_scan;
		struct max_buff *scan = slsi_mbuf_dequeue(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);

		while (scan) {
			struct slsi_80211_mgmt *mgmt = fapi_get_mgmt(scan);
			struct slsi_80211_channel *channel;

			/* make sure this BSSID has not already been used */
			mbuf_queue_walk(&unique_scan_results, unique_scan) {
				struct slsi_80211_mgmt *unique_mgmt = fapi_get_mgmt(unique_scan);

				if (compare_ether_addr(mgmt->bssid, unique_mgmt->bssid) == 0) {
					slsi_kfree_mbuf(scan);
					goto next_scan;
				}
			}

			slsi_mbuf_queue_head(&unique_scan_results, scan);

			channel = slsi_find_scan_channel(sdev, mgmt, fapi_get_mgmtlen(scan), fapi_get_u16(scan, u.mlme_scan_ind.channel_frequency) / 2);
			if (!channel) {
				goto next_scan;
			}

			/* check for interfering channels for 1, 6 and 11 */
			for (i = 0, j = 0; i < SLSI_AP_AUTO_CHANLS_LIST_FROM_HOSTAPD_MAX && channels[j]; i++, j = j + 5) {
				if (channel->center_freq == channels[j]->center_freq) {
					SLSI_NET_DBG3(dev, SLSI_T20_80211, "exact match:%d\n", i);
					scan_result_count[i] += 5;
					goto next_scan;
				}
				freqdiff = abs((int)channel->center_freq - (channels[j]->center_freq));
				if (freqdiff <= 20) {
					SLSI_NET_DBG3(dev, SLSI_T20_80211, "overlapping:%d, freqdiff:%d\n", i, freqdiff);
					scan_result_count[i] += (5 - (freqdiff / 5));
				}
			}

next_scan:
			scan = slsi_mbuf_dequeue(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		}
		/* Select the channel to use */
		for (i = 0, j = 0; i < SLSI_AP_AUTO_CHANLS_LIST_FROM_HOSTAPD_MAX; i++, j = j + 5) {
			SLSI_NET_DBG3(dev, SLSI_T20_80211, "score[%d]:%d\n", i, scan_result_count[i]);
			if (scan_result_count[i] <= scan_result_count[min_index]) {
				min_index = i;
				selected_index = j;
			}
		}
		SLSI_NET_DBG3(dev, SLSI_T20_80211, "selected:%d with score:%d\n", selected_index, scan_result_count[min_index]);

		SLSI_MUTEX_LOCK(sdev->device_config_mutex);
		sdev->device_config.ap_auto_chan = channels[selected_index]->hw_value & 0xFF;
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	}
	slsi_mbuf_queue_purge(&unique_scan_results);

exit_with_vif:
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
	return r;
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

#ifdef CONFIG_SCSC_ENABLE_P2P
/**
 * Work to be done when ROC retention duration expires:
 * Send ROC expired event to cfg80211 and queue work to delete unsync vif after retention timeout.
 */
static void slsi_p2p_roc_duration_expiry_work(struct work_struct *work)
{
	struct netdev_vif *ndev_vif = container_of((struct delayed_work *)work, struct netdev_vif, unsync.roc_expiry_work);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	/* There can be a race condition of this work function waiting for ndev_vif->vif_mutex and meanwhile the vif is deleted (due to net_stop).
	 * In such cases ndev_vif->chan would have been cleared.
	 */
	if (ndev_vif->sdev->p2p_state == P2P_IDLE_NO_VIF) {
		SLSI_NET_DBG1(ndev_vif->wdev.netdev, SLSI_T20_80211, "P2P unsync vif is not present\n");
		goto exit;
	}

	SLSI_NET_DBG3(ndev_vif->wdev.netdev, SLSI_T20_80211, "Send ROC expired event\n");

	/* If action frame tx is in progress don't schedule work to delete vif */
	if (ndev_vif->sdev->p2p_state != P2P_ACTION_FRAME_TX_RX) {
		slsi_p2p_queue_unsync_vif_del_work(ndev_vif, SLSI_P2P_UNSYNC_VIF_EXTRA_MSEC);
		SLSI_P2P_STATE_CHANGE(ndev_vif->sdev, P2P_IDLE_VIF_ACTIVE);
	}

	cfg80211_remain_on_channel_expired(&ndev_vif->wdev, ndev_vif->unsync.roc_cookie, ndev_vif->chan);

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
}

/**
 * Work to be done when unsync vif retention duration expires:
 * Delete the unsync vif.
 */
static void slsi_p2p_unsync_vif_delete_work(struct work_struct *work)
{
	struct netdev_vif *ndev_vif = container_of((struct delayed_work *)work, struct netdev_vif, unsync.del_vif_work);

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	SLSI_NET_DBG1(ndev_vif->wdev.netdev, SLSI_T20_80211, "Delete vif duration expired - Deactivate unsync vif\n");
	slsi_p2p_vif_deactivate(ndev_vif->sdev, ndev_vif->wdev.netdev, true);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
}

/* Initializations for P2P - Change vif type to unsync, create workqueue and init work */
int slsi_p2p_init(struct slsi_dev *sdev, struct netdev_vif *ndev_vif)
{
	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "Initialize P2P - Init P2P state to P2P_IDLE_NO_VIF\n");
	sdev->p2p_state = P2P_IDLE_NO_VIF;
	sdev->delete_gc_probe_req_ies = false;
	sdev->gc_probe_req_ies = NULL;
	sdev->gc_probe_req_ie_len = 0;
	sdev->p2p_group_exp_frame = SLSI_P2P_PA_INVALID;

	ndev_vif->vif_type = FAPI_VIFTYPE_UNSYNCHRONISED;

	//INIT_DELAYED_WORK(&ndev_vif->unsync.roc_expiry_work, slsi_p2p_roc_duration_expiry_work);
	//INIT_DELAYED_WORK(&ndev_vif->unsync.del_vif_work, slsi_p2p_unsync_vif_delete_work);

	return 0;
}

/* De-initializations for P2P - Reset vif type, cancel work and destroy workqueue */
void slsi_p2p_deinit(struct slsi_dev *sdev, struct netdev_vif *ndev_vif)
{
	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "De-initialize P2P\n");

	ndev_vif->vif_type = SLSI_VIFTYPE_UNSPECIFIED;

	/* Work should have been cleaned up by now */
	/*if (WARN_ON(delayed_work_pending(&ndev_vif->unsync.del_vif_work)))
	   cancel_delayed_work(&ndev_vif->unsync.del_vif_work);

	   if (WARN_ON(delayed_work_pending(&ndev_vif->unsync.roc_expiry_work)))
	   cancel_delayed_work(&ndev_vif->unsync.roc_expiry_work); */

	kmm_free(sdev->gc_probe_req_ies);
	sdev->gc_probe_req_ies = NULL;
	sdev->gc_probe_req_ie_len = 0;
	sdev->delete_gc_probe_req_ies = false;
}

/**
 * P2P vif activation:
 * Add unsync vif, register for action frames, configure Probe Rsp IEs if required and set channel
 */
int slsi_p2p_vif_activate(struct slsi_dev *sdev, struct netif *dev, struct slsi_80211_channel *chan, u16 duration, bool set_probe_rsp_ies)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u32 af_bmap_active = SLSI_ACTION_FRAME_PUBLIC;
	u32 af_bmap_suspended = SLSI_ACTION_FRAME_PUBLIC;
	int r = 0;

	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "Activate P2P unsync vif\n");

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	/* Interface address and device address are same for P2P unsync vif */
	if (slsi_mlme_add_vif(sdev, dev, dev->dev_addr, dev->dev_addr) != 0) {
		SLSI_NET_ERR(dev, "slsi_mlme_add_vif failed for unsync vif\n");
		goto exit_with_error;
	}

	ndev_vif->activated = true;
	SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_VIF_ACTIVE);

	if (slsi_mlme_register_action_frame(sdev, dev, af_bmap_active, af_bmap_suspended) != 0) {
		SLSI_NET_ERR(dev, "Action frame registration failed for unsync vif\n");
		goto exit_with_vif;
	}

	if (set_probe_rsp_ies) {
		u16 purpose = FAPI_PURPOSE_PROBE_RESPONSE;

		if (ndev_vif->unsync.probe_rsp_ies == NULL) {
			SLSI_NET_ERR(dev, "Probe Response IEs not available for ROC\n");
			goto exit_with_vif;
		}

		if (slsi_mlme_add_info_elements(sdev, dev, purpose, ndev_vif->unsync.probe_rsp_ies, ndev_vif->unsync.probe_rsp_ies_len) != 0) {
			SLSI_NET_ERR(dev, "Setting Probe Response IEs for unsync vif failed\n");
			goto exit_with_vif;
		}
		ndev_vif->unsync.ies_changed = false;
	}

	if (slsi_mlme_set_channel(sdev, dev, chan, SLSI_FW_CHANNEL_DURATION_UNSPECIFIED, 0, 0) != 0) {
		SLSI_NET_ERR(dev, "Set channel failed for unsync vif\n");
		goto exit_with_vif;
	} else {
		ndev_vif->chan = chan;
	}

	ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
	goto exit;

exit_with_vif:
	slsi_p2p_vif_deactivate(sdev, dev, true);
exit_with_error:
	r = -EINVAL;
exit:
	return r;
}

/* Delete unsync vif - DON'T update the vif type */
void slsi_p2p_vif_deactivate(struct slsi_dev *sdev, struct netif *dev, bool hw_available)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	SLSI_NET_DBG1(dev, SLSI_INIT_DEINIT, "De-activate P2P unsync vif\n");

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (sdev->p2p_state == P2P_IDLE_NO_VIF) {
		SLSI_NET_DBG1(dev, SLSI_INIT_DEINIT, "P2P unsync vif already deactivated\n");
		return;
	}

	/* Indicate failure using cfg80211_mgmt_tx_status() if frame TX is not completed during VIF delete */
	if (ndev_vif->mgmt_tx_data.exp_frame != SLSI_P2P_PA_INVALID) {
		ndev_vif->mgmt_tx_data.exp_frame = SLSI_P2P_PA_INVALID;
		cfg80211_mgmt_tx_status(&ndev_vif->wdev, ndev_vif->mgmt_tx_data.cookie, ndev_vif->mgmt_tx_data.buf, ndev_vif->mgmt_tx_data.buf_len, false);
	}
	//cancel_delayed_work(&ndev_vif->unsync.del_vif_work);
	//cancel_delayed_work(&ndev_vif->unsync.roc_expiry_work);

	if (hw_available) {
		slsi_mlme_del_vif(sdev, dev);
	}

	SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_NO_VIF);

	/* slsi_vif_deactivated is not used here after del_vif as it modifies vif type as well */

	ndev_vif->activated = false;
	ndev_vif->chan = NULL;

	if (WARN_ON(ndev_vif->unsync.listen_offload)) {
		ndev_vif->unsync.listen_offload = false;
	}

	slsi_unsync_vif_set_probe_rsp_ie(ndev_vif, NULL, 0);
	(void)slsi_set_mgmt_tx_data(ndev_vif, 0, 0, NULL, 0);

	SLSI_NET_DBG2(dev, SLSI_INIT_DEINIT, "P2P unsync vif deactivated\n");
}

/**
 * Delete unsync vif when group role is being started.
 * For such cases the netif during the call would be of the group interface (called from ap_start/connect).
 * Hence get the netif using P2P Index. Take the mutex lock and call slsi_p2p_vif_deactivate.
 */
void slsi_p2p_group_start_remove_unsync_vif(struct slsi_dev *sdev)
{
	struct netif *dev = NULL;
	struct netdev_vif *ndev_vif = NULL;

	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "Starting P2P Group - Remove unsync vif\n");

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2P);
	if (!dev) {
		SLSI_ERR(sdev, "Failed to deactivate p2p vif as dev is not found\n");
		return;
	}

	ndev_vif = netdev_priv(dev);

	if (WARN_ON(!(SLSI_IS_P2P_UNSYNC_VIF(ndev_vif)))) {
		return;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	slsi_p2p_vif_deactivate(sdev, dev, true);
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
}

/**
 * Called only for P2P Device mode (p2p0 interface) to store the Probe Response IEs
 * which would be used in Listen (ROC) state.
 * If the IEs are received in Listen Offload mode, then configure the IEs in firmware.
 */
int slsi_p2p_dev_probe_rsp_ie(struct slsi_dev *sdev, struct netif *dev, u8 *probe_rsp_ie, size_t probe_rsp_ie_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int ret = 0;

	SLSI_UNUSED_PARAMETER(sdev);

	if (!SLSI_IS_P2P_UNSYNC_VIF(ndev_vif)) {
		SLSI_NET_ERR(dev, "Incorrect vif type - Not unsync vif\n");
		kmm_free(probe_rsp_ie);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG2(dev, SLSI_T20_80211, "Received Probe Rsp IE len = %zu, Current IE len = %zu\n", probe_rsp_ie_len, ndev_vif->unsync.probe_rsp_ies_len);

	if (!ndev_vif->unsync.listen_offload) {	/* ROC */
		/* Store the IEs. Upon receiving it on subsequent occassions, store only if IEs have changed */
		if (ndev_vif->unsync.probe_rsp_ies_len != probe_rsp_ie_len) {	/* Check if IE length changed */
			ndev_vif->unsync.ies_changed = true;
		} else if (memcmp(ndev_vif->unsync.probe_rsp_ies, probe_rsp_ie, probe_rsp_ie_len) != 0) {	/* Check if IEs changed */
			ndev_vif->unsync.ies_changed = true;
		} else {				/* No change in IEs */
			kmm_free(probe_rsp_ie);
			goto exit;
		}

		slsi_unsync_vif_set_probe_rsp_ie(ndev_vif, probe_rsp_ie, probe_rsp_ie_len);
	} else {					/* P2P Listen Offloading */
		if (sdev->p2p_state == P2P_LISTENING) {
			ret = slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_RESPONSE, probe_rsp_ie, probe_rsp_ie_len);
			if (ret != 0) {
				SLSI_NET_ERR(dev, "Listen Offloading: Setting Probe Response IEs for unsync vif failed\n");
				ndev_vif->unsync.listen_offload = false;
				slsi_p2p_vif_deactivate(sdev, dev, true);
			}
		}
	}

exit:
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return ret;
}

/**
 * This should be called only for P2P Device mode (p2p0 interface). NULL IEs to clear Probe Response IEs are not updated
 * in driver to avoid configuring the Probe Response IEs to firmware on every ROC.
 * Use this call as a cue to stop any ongoing P2P scan as there is no API from user space for cancelling scan.
 * If ROC was in progress as part of P2P_FIND then Cancel ROC will be received.
 */
int slsi_p2p_dev_null_ies(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	if (!SLSI_IS_P2P_UNSYNC_VIF(ndev_vif)) {
		SLSI_NET_ERR(dev, "Incorrect vif type - Not unsync vif\n");
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

	SLSI_NET_DBG3(dev, SLSI_T20_80211, "Probe Rsp NULL IEs\n");

	if (sdev->p2p_state == P2P_SCANNING) {
		struct max_buff *scan_result;

		SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);

		SLSI_NET_DBG1(dev, SLSI_T20_80211, "Stop Find - Abort ongoing P2P scan\n");

		(void)slsi_mlme_del_scan(sdev, dev, ((ndev_vif->ifnum << 8) | SLSI_SCAN_HW_ID));

		scan_result = slsi_mbuf_dequeue(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		while (scan_result) {
			slsi_rx_scan_pass_to_cfg80211(sdev, dev, scan_result);
			scan_result = slsi_mbuf_dequeue(&ndev_vif->scan[SLSI_SCAN_HW_ID].scan_results);
		}

		WARN_ON(ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req == NULL);

		if (ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req) {
			cfg80211_scan_done(ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req, true);
		}

		ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req = NULL;

		SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);

		if (ndev_vif->activated) {
			/* Supplicant has stopped FIND. Also clear Probe Response IEs in firmware and driver
			 * as Cancel ROC will not be sent as driver was not in Listen
			 */
			SLSI_NET_DBG1(dev, SLSI_T20_80211, "Stop Find - Clear Probe Response IEs in firmware\n");
			if (slsi_mlme_add_info_elements(sdev, dev, FAPI_PURPOSE_PROBE_RESPONSE, NULL, 0) != 0) {
				SLSI_NET_ERR(dev, "Clearing Probe Response IEs failed for unsync vif\n");
			}
			slsi_unsync_vif_set_probe_rsp_ie(ndev_vif, NULL, 0);

			SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_VIF_ACTIVE);
		} else {
			SLSI_P2P_STATE_CHANGE(sdev, P2P_IDLE_NO_VIF);
		}
	}

	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	return 0;
}

/**
 * Returns the P2P public action frame subtype.
 * Returns SLSI_P2P_PA_INVALID if it is not a P2P public action frame.
 */
int slsi_p2p_get_public_action_subtype(const struct slsi_80211_mgmt *mgmt)
{
	int subtype = SLSI_P2P_PA_INVALID;
	/* Vendor specific Public Action (0x09), P2P OUI (0x50, 0x6f, 0x9a), P2P Subtype (0x09) */
	u8 p2p_pa_frame[5] = { 0x09, 0x50, 0x6f, 0x9a, 0x09 };
	u8 *action = (u8 *)&mgmt->u.action.u;

	if (memcmp(&action[0], p2p_pa_frame, 5) == 0) {
		subtype = action[5];
	} else {
		/* For service discovery action frames dummy subtype is used */
		switch (action[0]) {
		case SLSI_PA_GAS_INITIAL_REQ:
		case SLSI_PA_GAS_INITIAL_RSP:
		case SLSI_PA_GAS_COMEBACK_REQ:
		case SLSI_PA_GAS_COMEBACK_RSP:
			subtype = (action[0] | SLSI_PA_GAS_DUMMY_SUBTYPE_MASK);
			break;
		}
	}

	return subtype;
}

/**
 * Returns the P2P status code of Status attribute of the GO Neg Rsp frame.
 * Returns -1 if status attribute is NOT found.
 */
int slsi_p2p_get_go_neg_rsp_status(struct netif *dev, const struct slsi_80211_mgmt *mgmt)
{
	int status = -1;
	u8 p2p_oui_type[4] = { 0x50, 0x6f, 0x9a, 0x09 };
	u8 *action = (u8 *)&mgmt->u.action.u;
	u8 *vendor_ie = &action[7];	/* 1 (0x09), 4 (0x50, 0x6f, 0x9a, 0x09), 1 (0x01), 1 (Dialog Token) */
	u8 ie_length, elem_idx;
	u16 attr_length;

	while (vendor_ie && (*vendor_ie == SLSI_WLAN_EID_VENDOR_SPECIFIC)) {
		ie_length = vendor_ie[1];

		if (memcmp(&vendor_ie[2], p2p_oui_type, 4) == 0) {
			elem_idx = 6;		/* 1 (Id - 0xdd) + 1 (Length) + 4 (OUI and Type) */

			while (ie_length > elem_idx) {
				attr_length = ((vendor_ie[elem_idx + 1]) | (vendor_ie[elem_idx + 2] << 8));

				if (vendor_ie[elem_idx] == SLSI_P2P_STATUS_ATTR_ID) {
					SLSI_NET_DBG3(dev, SLSI_T20_80211, "Status Attribute Found, attr_length = %d, value (%u %u %u %u)\n", attr_length, vendor_ie[elem_idx], vendor_ie[elem_idx + 1], vendor_ie[elem_idx + 2], vendor_ie[elem_idx + 3]);
					status = vendor_ie[elem_idx + 3];
					break;
				}
				elem_idx += 3 + attr_length;
			}

			break;
		}
		vendor_ie += 2 + ie_length;
	}

	SLSI_UNUSED_PARAMETER(dev);

	return status;
}

/**
 * Returns the next expected P2P public action frame subtype for input subtype.
 * Returns SLSI_P2P_PA_INVALID if no frame is expected.
 */
u8 slsi_p2p_get_exp_peer_frame_subtype(u8 subtype)
{
	switch (subtype) {
	/* Peer response is expected for following frames */
	case SLSI_P2P_PA_GO_NEG_REQ:
	case SLSI_P2P_PA_GO_NEG_RSP:
	case SLSI_P2P_PA_INV_REQ:
	case SLSI_P2P_PA_DEV_DISC_REQ:
	case SLSI_P2P_PA_PROV_DISC_REQ:
	case SLSI_PA_GAS_INITIAL_REQ_SUBTYPE:
	case SLSI_PA_GAS_COMEBACK_REQ_SUBTYPE:
		return subtype + 1;
	default:
		return SLSI_P2P_PA_INVALID;
	}
}
#endif							/* CONFIG_SCSC_ENABLE_P2P */

/**
 * Returns a slsi_dhcp_tx enum value after verifying whether the 802.11 packet in mbuf
 * is a DHCP packet (identified by UDP port numbers)
 */
int slsi_is_dhcp_packet(u8 *data)
{
	u8 *p;
	int ret = SLSI_TX_IS_NOT_DHCP;

	p = data + SLSI_IP_TYPE_OFFSET;

	if (*p == SLSI_IP_TYPE_UDP) {
		u16 source_port, dest_port;

		p = data + SLSI_IP_SOURCE_PORT_OFFSET;
		source_port = p[0] << 8 | p[1];
		p = data + SLSI_IP_DEST_PORT_OFFSET;
		dest_port = p[0] << 8 | p[1];
		if ((source_port == SLSI_DHCP_CLIENT_PORT) && (dest_port == SLSI_DHCP_SERVER_PORT)) {
			ret = SLSI_TX_IS_DHCP_CLIENT;
		} else if ((source_port == SLSI_DHCP_SERVER_PORT) && (dest_port == SLSI_DHCP_CLIENT_PORT)) {
			ret = SLSI_TX_IS_DHCP_SERVER;
		}
	}

	return ret;
}

int slsi_ap_prepare_add_info_ies(struct netdev_vif *ndev_vif, const u8 *ies, size_t ies_len)
{
	const u8 *wps_p2p_ies = NULL;
	size_t wps_p2p_ie_len = 0;

	/* The ies may contain Extended Capability followed by WPS IE. The Extended capability IE needs to be excluded. */
	wps_p2p_ies = slsi_80211_find_ie(SLSI_WLAN_EID_VENDOR_SPECIFIC, ies, ies_len);
	if (wps_p2p_ies) {
		size_t temp_len = wps_p2p_ies - ies;

		wps_p2p_ie_len = ies_len - temp_len;
	}

	SLSI_NET_DBG2(ndev_vif->wdev.netdev, SLSI_MLME, "WPA IE len = %zu, WMM IE len = %zu, IEs len = %zu, WPS_P2P IEs len = %zu\n", ndev_vif->ap.wpa_ie_len, ndev_vif->ap.wmm_ie_len, ies_len, wps_p2p_ie_len);

	ndev_vif->ap.add_info_ies_len = ndev_vif->ap.wpa_ie_len + ndev_vif->ap.wmm_ie_len + wps_p2p_ie_len;
	ndev_vif->ap.add_info_ies = kmm_malloc(ndev_vif->ap.add_info_ies_len);	/* Caller needs to free this */

	if (ndev_vif->ap.add_info_ies == NULL) {
		SLSI_NET_DBG1(ndev_vif->wdev.netdev, SLSI_MLME, "Failed to allocate memory for IEs\n");
		ndev_vif->ap.add_info_ies_len = 0;
		return -ENOMEM;
	}

	if (ndev_vif->ap.cache_wpa_ie) {
		memcpy(ndev_vif->ap.add_info_ies, ndev_vif->ap.cache_wpa_ie, ndev_vif->ap.wpa_ie_len);
		ndev_vif->ap.add_info_ies += ndev_vif->ap.wpa_ie_len;
	}

	if (ndev_vif->ap.cache_wmm_ie) {
		memcpy(ndev_vif->ap.add_info_ies, ndev_vif->ap.cache_wmm_ie, ndev_vif->ap.wmm_ie_len);
		ndev_vif->ap.add_info_ies += ndev_vif->ap.wmm_ie_len;
	}

	if (wps_p2p_ies) {
		memcpy(ndev_vif->ap.add_info_ies, wps_p2p_ies, wps_p2p_ie_len);
		ndev_vif->ap.add_info_ies += wps_p2p_ie_len;
	}

	ndev_vif->ap.add_info_ies -= ndev_vif->ap.add_info_ies_len;

	return 0;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
static u32 slsi_remap_reg_rule_flags(u8 flags)
{
	u32 remapped_flags = 0;

	if (flags & SLSI_REGULATORY_DFS) {
		remapped_flags |= SLSI_80211_RRF_DFS;
	}
	if (flags & SLSI_REGULATORY_NO_OFDM) {
		remapped_flags |= SLSI_80211_RRF_NO_OFDM;
	}
	if (flags & SLSI_REGULATORY_NO_IR) {
		remapped_flags |= SLSI_80211_RRF_PASSIVE_SCAN | SLSI_80211_RRF_NO_IBSS;
	}
	if (flags & SLSI_REGULATORY_NO_INDOOR) {
		remapped_flags |= SLSI_80211_RRF_NO_INDOOR;
	}
	if (flags & SLSI_REGULATORY_NO_OUTDOOR) {
		remapped_flags |= SLSI_80211_RRF_NO_OUTDOOR;
	}

	return remapped_flags;
}
#endif

static void slsi_reg_mib_to_regd(struct slsi_mib_data *mib, struct slsi_802_11d_reg_domain domain_info)
{
#ifdef CONFIG_SCSC_ADV_FEATURE
	int i = 0;
	int num_rules = 0;
	u16 freq;
	u8 byte_val;
	struct ieee80211_reg_rule *reg_rule;

	SLSI_DBG3_NODEV(SLSI_MLME, "slsi_reg_mib_to_regd");

	domain_info.regdomain->alpha2[0] = *(u8 *)(&mib->data[i]);
	i++;

	domain_info.regdomain->alpha2[1] = *(u8 *)(&mib->data[i]);
	i++;

	domain_info.regdomain->dfs_region = *(u8 *)(&mib->data[i]);
	i++;

	while (i < mib->dataLength) {
		reg_rule = &domain_info.regdomain->reg_rules[num_rules];

		/* start freq 2 bytes */
		freq = __le16_to_cpu(*(u16 *)(&mib->data[i]));
		reg_rule->freq_range.start_freq_khz = MHZ_TO_KHZ(freq);

		/* end freq 2 bytes */
		freq = __le16_to_cpu(*(u16 *)(&mib->data[i + 2]));
		reg_rule->freq_range.end_freq_khz = MHZ_TO_KHZ(freq);

		/* Max Bandwidth 1 byte */
		byte_val = *(u8 *)(&mib->data[i + 4]);
		reg_rule->freq_range.max_bandwidth_khz = MHZ_TO_KHZ(byte_val);

		/* max_antenna_gain is obsolute now. */
		reg_rule->power_rule.max_antenna_gain = 0;

		/* Max Power 1 byte */
		byte_val = *(u8 *)(&mib->data[i + 5]);
		reg_rule->power_rule.max_eirp = DBM_TO_MBM(byte_val);

		/* Flags 1 byte */
		reg_rule->flags = slsi_remap_reg_rule_flags(*(u8 *)(&mib->data[i + 6]));

		i += 7;

		num_rules++;			/* Num of reg rules */

		SLSI_DBG3_NODEV(SLSI_MLME, "reg_rule:[%d] sf[%x] ef[%x] mg[%x] bw[%x] cn[%x]\n", num_rules, reg_rule->freq_range.start_freq_khz, reg_rule->freq_range.end_freq_khz, reg_rule->power_rule.max_antenna_gain, reg_rule->freq_range.max_bandwidth_khz, byte_val);
	}

	domain_info.regdomain->n_reg_rules = num_rules;
#endif
}

void slsi_reset_channel_flags(struct slsi_dev *sdev)
{
#ifdef CONFIG_SCSC_ADV_FEATURE
	enum ieee80211_band band;
	struct slsi_80211_channel *chan;
	int i;
	struct wiphy *wiphy = sdev->wiphy;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		if (!wiphy->bands[band]) {
			continue;
		}
		for (i = 0; i < wiphy->bands[band]->n_channels; i++) {
			chan = &wiphy->bands[band]->channels[i];
			chan->flags = 0;
		}
	}
#endif
}

int slsi_read_regulatory_rules(struct slsi_dev *sdev, u16 psid, u16 index)
{
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	struct slsi_mib_entry mib_val;
	int r = 0;
	int rxLength = 0;
	int len = 0;

	SLSI_DBG3(sdev, SLSI_MLME, "\n");

	slsi_mib_encode_get(&mibreq, psid, index);

	/* Max of 6 regulatory constraints.
	 * each constraint start_freq(2 byte), end_freq(2 byte), Band width(1 byte), Max power(1 byte),
	 * rules flag (1 byte)
	 * firmware can have a max of 6 rules for a country.
	 */
	/* PSID header (5 bytes) + ((3 bytes) alpha2 code + dfs) + (max of 50 regulatory rules * 7 bytes each row) + MIB status(1) */
	mibrsp.dataLength = 5 + 3 + (SLSI_MIB_REG_RULES_MAX * 7) + 1;
	mibrsp.data = kmm_malloc(mibrsp.dataLength);

	if (mibrsp.data == NULL) {
		SLSI_ERR(sdev, "Failed to alloc for Mib response\n");
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, NULL, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &rxLength);
	kmm_free(mibreq.data);

	if (r == 0) {
		mibrsp.dataLength = rxLength;

		len = slsi_mib_decode(&mibrsp, &mib_val);

		if (len == 0) {
			kmm_free(mibrsp.data);
			SLSI_ERR(sdev, "Mib decode error\n");
			return -EINVAL;
		}
		slsi_reg_mib_to_regd(&mib_val.value.u.octetValue, sdev->device_config.domain_info);
		slsi_reset_channel_flags(sdev);
		//wiphy_apply_custom_regulatory(sdev->wiphy, sdev->device_config.domain_info.regdomain);
	} else {
		SLSI_ERR(sdev, "Mib read failed (error: %d)\n", r);
	}

	kmm_free(mibrsp.data);
	return r;
}

u16 slsi_country_to_index(struct slsi_802_11d_reg_domain domain_info, const char *alpha2)
{
	u16 index = 0;

	SLSI_DBG3_NODEV(SLSI_MLME, "\n");

	if (domain_info.countrylist != NULL) {
		for (index = 0; index < domain_info.country_len; index += 2)
			if (memcmp(&domain_info.countrylist[index], alpha2, 2) == 0) {
				break;
			}

		return (index / 2) + 1;
	}

	return 0;
}

/* Set the new country code and read the regulatory parameters of updated country. */
int slsi_set_country_update_regd(struct slsi_dev *sdev, const char *alpha2_code, int size)
{
	struct slsi_mib_data mib_data = { 0, NULL };
	u8 alpha2[4];
	int error = 0;
	u16 index = 0;

	SLSI_DBG2(sdev, SLSI_MLME, "Set country code: %c%c\n", alpha2_code[0], alpha2_code[1]);

	if (size == 4) {
		memcpy(alpha2, alpha2_code, 4);
	} else {
		memcpy(alpha2, alpha2_code, 3);
		alpha2[3] = '\0';
	}
#ifdef CONFIG_SCSC_ADV_FEATURE
	if (memcmp(alpha2, sdev->device_config.domain_info.regdomain->alpha2, 2) == 0) {
		SLSI_DBG3(sdev, SLSI_MLME, "Country is already set to the requested country code\n");
		return 0;
	}
#endif
	SLSI_MUTEX_LOCK(sdev->device_config_mutex);

	error = slsi_mib_encode_octet(&mib_data, SLSI_PSID_UNIFI_DEFAULT_COUNTRY, 3, &alpha2[0], 0);
	if (error != SLSI_MIB_STATUS_SUCCESS) {
		error = -ENOMEM;
		goto exit;
	}

	if (WARN_ON(mib_data.dataLength == 0)) {
		error = -EINVAL;
		goto exit;
	}
	error = slsi_mlme_set(sdev, NULL, mib_data.data, mib_data.dataLength);

	kmm_free(mib_data.data);

	if (error) {
		SLSI_ERR(sdev, "Err setting country. error = %d\n", error);
		SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
		return -1;
	}

	/* Read the regulatory params for the country based country index */
	index = slsi_country_to_index(sdev->device_config.domain_info, alpha2_code);
	if (index) {
		slsi_read_regulatory_rules(sdev, SLSI_PSID_UNIFI_REGULATORY_PARAMETERS, index);
	}

exit:
	SLSI_MUTEX_UNLOCK(sdev->device_config_mutex);
	return error;
}

/* Read unifiDisconnectTimeOut MIB */
int slsi_read_disconnect_ind_timeout(struct slsi_dev *sdev, u16 psid)
{
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	struct slsi_mib_entry mib_val;
	int r = 0;
	int rxLength = 0;
	int len = 0;

	SLSI_DBG3(sdev, SLSI_MLME, "\n");

	slsi_mib_encode_get(&mibreq, psid, 0);

	mibrsp.dataLength = 10;		/* PSID header(5) + uint 4 bytes + status(1) */
	mibrsp.data = kmm_malloc(mibrsp.dataLength);

	if (mibrsp.data == NULL) {
		SLSI_ERR(sdev, "Failed to alloc for Mib response\n");
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, NULL, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &rxLength);
	kmm_free(mibreq.data);

	if (r == 0) {
		mibrsp.dataLength = rxLength;
		len = slsi_mib_decode(&mibrsp, &mib_val);

		if (len == 0) {
			kmm_free(mibrsp.data);
			SLSI_ERR(sdev, "Mib decode error\n");
			return -EINVAL;
		}
		/* Add additional 1 sec delay */
		sdev->device_config.ap_disconnect_ind_timeout = ((mib_val.value.u.uintValue + 1) * 1000);
	} else {
		SLSI_ERR(sdev, "Mib read failed (error: %d)\n", r);
	}

	kmm_free(mibrsp.data);
	return r;
}

/* Read unifiDefaultCountry MIB */
int slsi_read_default_country(struct slsi_dev *sdev, u8 *alpha2, u16 psid)
{
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	struct slsi_mib_entry mib_val;
	int r = 0;
	int rxLength = 0;
	int len = 0;

	SLSI_DBG3(sdev, SLSI_MLME, "\n");

	slsi_mib_encode_get(&mibreq, psid, 0);

	mibrsp.dataLength = 10;		/* PSID header(5) + country code alpha2 2 bytes + status(1) */
	mibrsp.data = kmm_malloc(mibrsp.dataLength);

	if (mibrsp.data == NULL) {
		SLSI_ERR(sdev, "Failed to alloc for Mib response\n");
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, NULL, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &rxLength);

	kmm_free(mibreq.data);

	if (r == 0) {
		mibrsp.dataLength = rxLength;
		len = slsi_mib_decode(&mibrsp, &mib_val);

		if (len == 0) {
			kmm_free(mibrsp.data);
			SLSI_ERR(sdev, "Mib decode error\n");
			return -EINVAL;
		}
		memcpy(alpha2, mib_val.value.u.octetValue.data, 2);
	} else {
		SLSI_ERR(sdev, "Mib read failed (error: %d)\n", r);
	}

	kmm_free(mibrsp.data);
	return r;
}

int slsi_copy_country_table(struct slsi_dev *sdev, struct slsi_mib_data *mib, int len)
{
	SLSI_DBG3(sdev, SLSI_MLME, "\n");

	sdev->device_config.domain_info.countrylist = kmm_malloc(len);
	if (sdev->device_config.domain_info.countrylist == NULL) {
		SLSI_ERR(sdev, "kmm_malloc failed\n");
		return -EINVAL;
	}

	sdev->device_config.domain_info.country_len = len;

	memcpy(sdev->device_config.domain_info.countrylist, mib->data, len);

	return 0;
}

/* Read unifi country list */
int slsi_read_unifi_countrylist(struct slsi_dev *sdev, u16 psid)
{
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	struct slsi_mib_entry mib_val;
	int r = 0;
	int rxLength = 0;
	int len = 0;

	SLSI_DBG3(sdev, SLSI_MLME, "\n");

	slsi_mib_encode_get(&mibreq, psid, 0);

	/* Fixed fields len (5) : 2 bytes(PSID) + 2 bytes (Len) + 1 byte (status)
	 * Data : 148 countries??? for SLSI_PSID_UNIFI_COUNTRY_LIST
	 */
	mibrsp.dataLength = 5 + (NUM_COUNTRY * 2);
	mibrsp.data = kmm_malloc(mibrsp.dataLength);

	if (mibrsp.data == NULL) {
		SLSI_ERR(sdev, "Failed to alloc for Mib response\n");
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, NULL, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &rxLength);

	kmm_free(mibreq.data);

	if (r == 0) {
		mibrsp.dataLength = rxLength;
		len = slsi_mib_decode(&mibrsp, &mib_val);

		if (len == 0) {
			kmm_free(mibrsp.data);
			return -EINVAL;
		}
		slsi_copy_country_table(sdev, &mib_val.value.u.octetValue, len);
	} else {
		SLSI_ERR(sdev, "Mib read failed (error: %d)\n", r);
	}

	kmm_free(mibrsp.data);
	return r;
}

void slsi_regd_deinit(struct slsi_dev *sdev)
{
	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "slsi_regd_deinit\n");
	kmm_free(sdev->device_config.domain_info.countrylist);
}

#ifdef CONFIG_SCSC_ENABLE_P2P
void slsi_clear_offchannel_data(struct slsi_dev *sdev, bool acquire_lock)
{
	struct netif *dev = NULL;
	struct netdev_vif *ndev_vif = NULL;

	dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2PX);
	if (WARN_ON(!dev)) {
		SLSI_ERR(sdev, "No Group net dev found\n");
		return;
	}
	ndev_vif = netdev_priv(dev);

	if (acquire_lock) {
		SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
	}

	/* Reset dwell time should be sent on group vif */
	(void)slsi_mlme_reset_dwell_time(sdev, dev);

	(void)slsi_set_mgmt_tx_data(ndev_vif, 0, 0, NULL, 0);

	if (acquire_lock) {
		SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
	}

	sdev->p2p_group_exp_frame = SLSI_P2P_PA_INVALID;
}
#endif

/**
 * slsi_scan_ind_timeout_handle
 *
 * Description:
 *	Work to be performed on scan timeout.
 *	Delete ongoing scan and Indicate scan completion to upper layer.
 *
 * Parameters:
 *	arg - Pointer to the private driver data
 *
 * Return: None
 */
void slsi_scan_ind_timeout_handle(FAR void *arg)
{
	struct slsi_t20_drv *drv = (struct slsi_t20_drv *)arg;
	struct slsi_dev *sdev = drv->sdev;
	struct netif *dev = slsi_get_netdev(sdev, SLSI_NET_INDEX_WLAN);
	struct netdev_vif *ndev_vif;

	if (!dev) {
		SLSI_ERR_NODEV("dev not available\n");
		return;
	}

	ndev_vif = netdev_priv(dev);

	SLSI_MUTEX_LOCK(ndev_vif->scan_mutex);
	if (ndev_vif->scan[SLSI_SCAN_HW_ID].scan_req) {
		SLSI_WARN(sdev, "mlme_scan_ind not received");
		(void)slsi_mlme_del_scan(sdev, dev, ndev_vif->ifnum << 8 | SLSI_SCAN_HW_ID);
		slsi_scan_complete(ndev_vif);
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->scan_mutex);
}

/**
 * slsi_scan_complete
 *
 * Description:
 *	Indicate scan completion to supplicant with EVENT_SCAN_RESULTS
 *
 * Parameters:
 *	ndev_vif - netdev vif structure
 *
 * Return: None
 */
void slsi_scan_complete(struct netdev_vif *ndev_vif)
{
	/* The scan_info params are not filled for now and sent as NULL. It appears to
	 * be optional and supplicant also doesn't seem to be not using it significantly.
	 * Other drivers also appear to do this.
	 */

	wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_SCAN_RESULTS, NULL);
}

void slsi_connect_indication(struct netdev_vif *ndev_vif, const u8 *req_ie, size_t req_ie_len, const u8 *resp_ie, size_t resp_ie_len, u16 status, u16 vif_type, const u8 *addr)
{
	union wpa_event_data event;

	memset(&event, 0, sizeof(event));

	if (vif_type == FAPI_VIFTYPE_STATION) {
		if (status != WLAN_STATUS_SUCCESS) {
			if (resp_ie) {
				event.assoc_reject.resp_ies = resp_ie;
				event.assoc_reject.resp_ies_len = resp_ie_len;
			}
			event.assoc_reject.status_code = status;
			wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_ASSOC_REJECT, &event);
			return;
		}
		if (resp_ie) {
			event.assoc_info.resp_ies = resp_ie;
			event.assoc_info.resp_ies_len = resp_ie_len;
		}
		event.assoc_info.freq = ndev_vif->center_freq;
	} else if (vif_type == FAPI_VIFTYPE_AP) {
		event.assoc_info.addr = addr;
		event.assoc_info.freq = ndev_vif->chandef->freq;
	}

	if (req_ie) {
		event.assoc_info.req_ies = req_ie;
		event.assoc_info.req_ies_len = req_ie_len;
	}

	wpa_supplicant_event_send(ndev_vif->sdev->drv->ctx, EVENT_ASSOC, &event);
}

int slsi_get_mib(struct slsi_dev *sdev, u16 psid, int *mib_value)
{
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	int rxLength = 0;
	int r;
	struct slsi_mib_get_entry getValues[] = { {psid, { 0, 0 } } };

	r = slsi_mib_encode_get_list(&mibreq, sizeof(getValues) / sizeof(struct slsi_mib_get_entry), getValues);
	if (r != SLSI_MIB_STATUS_SUCCESS) {
		return -ENOMEM;
	}

	mibrsp.dataLength = 64;
	mibrsp.data = kmm_malloc(mibrsp.dataLength);
	if (mibrsp.data == NULL) {
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, NULL, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &rxLength);
	kmm_free(mibreq.data);

	if (r == 0) {
		struct slsi_mib_value *values;

		mibrsp.dataLength = (u32)rxLength;

		values = slsi_mib_decode_get_list(&mibrsp, sizeof(getValues) / sizeof(struct slsi_mib_get_entry), getValues);
		if (values == NULL) {
			SLSI_DBG1(sdev, SLSI_MLME, "Failed to decode MIB\n");
			kmm_free(mibrsp.data);
			return -ENOMEM;
		}

		WARN_ON(values[0].type == SLSI_MIB_TYPE_OCTET || values[0].type == SLSI_MIB_TYPE_NONE);

		if (values[0].type == SLSI_MIB_TYPE_INT) {
			*mib_value = (int)(values->u.intValue);
		} else if (values[0].type == SLSI_MIB_TYPE_UINT) {
			*mib_value = (int)(values->u.uintValue);
		} else if (values[0].type == SLSI_MIB_TYPE_BOOL) {
			*mib_value = (int)(values->u.boolValue);
		}

		SLSI_DBG2(sdev, SLSI_MLME, "MIB value = %d\n", *mib_value);
		kmm_free(values);
	} else {
		SLSI_ERR(sdev, "Mib read failed (error: %d)\n", r);
	}

	kmm_free(mibrsp.data);
	return r;
}

int slsi_set_mib(struct slsi_dev *dev, struct netif *ndev, u16 psid, int value)
{
	struct slsi_mib_data mib_data = { 0, NULL };
	int error = SLSI_MIB_STATUS_FAILURE;

	if (slsi_mib_encode_int(&mib_data, psid, value, 0) == SLSI_MIB_STATUS_SUCCESS)
		if (mib_data.dataLength) {
			error = slsi_mlme_set(dev, ndev, mib_data.data, mib_data.dataLength);
			if (error) {
				SLSI_ERR(dev, "Err Setting MIB failed. error = %d\n", error);
			}
			kmm_free(mib_data.data);
		}

	return error;
}

enum slsi_wlan_vendor_attr_hanged_event {
	SLSI_WLAN_VENDOR_ATTR_HANGED_EVENT_PANIC_CODE = 1,
	SLSI_WLAN_VENDOR_ATTR_HANGED_EVENT_MAX
};

int slsi_send_hanged_vendor_event(struct slsi_dev *sdev, u16 scsc_panic_code)
{
	union wpa_event_data data;

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.hanged_event);

	SLSI_INFO(sdev, "Sending EVENT_HANGED, reason =0x%2x\n", scsc_panic_code);

	memset(&data, 0, sizeof(data));
	data.hanged_event.panic_code = scsc_panic_code;
	wpa_supplicant_event_send(sdev->drv->ctx, EVENT_HANGED, &data);
	return 0;
}
