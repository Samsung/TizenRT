/*****************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/
#include "dev.h"
#include "debug_scsc.h"
#include "mlme.h"
#include "mib.h"
#include "mgt.h"
#include "hydra.h"
#include "wlan_80211_utils.h"
#include "utils_scsc.h"

#define SLSI_SCAN_PRIVATE_IE_CHANNEL_LIST_HEADER_LEN  (7)
#define SLSI_SCAN_PRIVATE_IE_SSID_FILTER_HEADER_LEN   (7)
#define SLSI_SCAN_CHANNEL_DESCRIPTOR_SIZE             (3)
#define SLSI_CHANN_INFO_HT_SCB                        (0x0100)

#define SLSI_FW_API_RATE_HT_SELECTOR_FIELD  0xc000
#define SLSI_FW_API_RATE_NON_HT_SELECTED    0x4000
#define SLSI_FW_API_RATE_HT_SELECTED        0x8000
#define SLSI_FW_API_RATE_VHT_SELECTED       0xc000

#define SLSI_FW_API_RATE_MCS_FIELD          0x000F
#define SLSI_FW_API_RATE_INDEX_FIELD        0x1fff

#define SLSI_FW_API_RATE_BW_FIELD           0x0600
#define SLSI_FW_API_RATE_BW_40MHZ           0x0200
#define SLSI_FW_API_RATE_BW_20MHZ           0x0000

#define SLSI_FW_API_RATE_SGI                0x0100
#define SLSI_FW_API_RATE_GF                 0x0080

/* indices: 3= BW20->idx_0, BW40->idx_1, BW80->idx_2.
 *             2= noSGI->idx_0, SGI->idx_1
 *             10= mcs index
 * rate units 100kbps
 * This table for single stream Nss=1and does not include 160MHz BW and 80+80MHz BW.
 * For HT, it will use only 20MHz and 40MHz
 */
static const u16 slsi_mlme_ht_and_vht_rates_table[3][2][10] = {
	{							/* BW20 */
		{							/* no SGI */
			65, 130, 195, 260, 390, 520, 585, 650, 780, 0
		},
		{							/* SGI */
			72, 144, 217, 289, 433, 578, 650, 722, 867, 0
		}
	},
	{							/* BW40 */
		{							/* no SGI */
			135, 270, 405, 540, 810, 1080, 1215, 1350, 1620, 1800
		},
		{							/* SGI */
			150, 300, 450, 600, 900, 1200, 1350, 1500, 1800, 2000
		}
	},
	{							/* BW80 */
		{							/* no SGI */
			293, 585, 878, 1170, 1755, 2340, 2633, 2925, 3510, 3900
		},
		{							/* SGI */
			325, 650, 975, 1300, 1950, 2600, 2925, 3250, 3900, 4333
		}
	}
};

struct slsi_mlme_rsse {
	u8 group_cs_count;
	const u8 *group_cs;
	u8 pairwise_cs_count;
	const u8 *pairwise_cs;
	u8 akm_suite_count;
	const u8 *akm_suite;
	u8 pmkid_count;
	const u8 *pmkid;
	const u8 *group_mgmt_cs;	/* used for PMF */
};

static struct max_buff *slsi_mlme_wait_for_cfm(struct slsi_dev *sdev, struct slsi_sig_send *sig_wait)
{
	struct max_buff *cfm = NULL;
	int tm = 0;

	tm = wait_for_completion_timeout(&sig_wait->completion, SLSI_SIG_WAIT_CFM_TIMEOUT);
	SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);

	/* Confirm timed out? */
	if (!sig_wait->cfm) {
		SLSI_ERR(sdev, "No cfm(0x%.4X) for req(0x%04X) senderid=0x%x\n", sig_wait->cfm_id, sig_wait->req_id, sig_wait->process_id);
		if (tm == 0) {
			char reason[80];
			WARN(1, "Timeout - confirm 0x%04x not received from chip\n", sig_wait->cfm_id);

			snprintf(reason, sizeof(reason), "Timed out while waiting for the cfm(0x%.4x) for req(0x%04x)", sig_wait->cfm_id, sig_wait->req_id);

			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.host_triggered_panic);
			SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
			slsi_sm_service_failed(sdev, reason);
			SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);
		} else {
			WARN(1, "Confirm 0x%04x lost\n", sig_wait->cfm_id);
		}
	} else {
		WARN_ON(fapi_get_u16(sig_wait->cfm, receiver_pid) != sig_wait->process_id);
		WARN_ON(fapi_get_u16(sig_wait->cfm, id) != sig_wait->cfm_id);
	}

	sig_wait->cfm_id = 0;
	cfm = sig_wait->cfm;
	sig_wait->cfm = NULL;
	if (!cfm) {
		sig_wait->ind_id = 0;
	}

	SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);

	return cfm;
}

static struct max_buff *slsi_mlme_wait_for_ind(struct slsi_dev *sdev, struct netif *dev, struct slsi_sig_send *sig_wait, u16 ind_id)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *ind = NULL;
	int tm = 0;

	/* The indication and confirm may have been received in the same HIP read.
	 * The HIP receive buffer processes all received signals in one thread whilst the
	 * waiting process may not be scheduled even if the "complete" call is made.
	 * In this scenario, the complete() call has already been made for this object
	 * and the wait will return immediately.
	 */
	if (ind_id == MLME_SCAN_DONE_IND)
		/* To handle the coex scenario where BTscan has high priority increasing the wait time to 40 secs */
	{
		tm = wait_for_completion_timeout(&sig_wait->completion, SLSI_SCAN_DONE_IND_WAIT_TIMEOUT);
	} else if ((ind_id == MLME_DISCONNECT_IND) && (ndev_vif->vif_type == FAPI_VIFTYPE_AP)) {
		tm = wait_for_completion_timeout(&sig_wait->completion, sdev->device_config.ap_disconnect_ind_timeout);
	} else {
		tm = wait_for_completion_timeout(&sig_wait->completion, (SLSI_SIG_WAIT_CFM_TIMEOUT + 2000));
	}

	SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);

	/* Indication timed out? */
	if (!sig_wait->ind) {
		SLSI_ERR(sdev, "No ind(0x%.4X) for req(0x%04X) senderid=0x%x\n", sig_wait->ind_id, sig_wait->req_id, sig_wait->process_id);
		if (tm == 0) {
			char reason[80];

			WARN(1, "Timeout - indication 0x%04x not received from chip\n", sig_wait->ind_id);

			snprintf(reason, sizeof(reason), "Timed out while waiting for the ind(0x%.4x) for req(0x%04x)", sig_wait->ind_id, sig_wait->req_id);

			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.host_triggered_panic);
			SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);
			slsi_sm_service_failed(sdev, reason);
			SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);
		} else {
			WARN(1, "Indication 0x%04x lost\n", sig_wait->ind_id);
		}
	} else {
		WARN_ON(fapi_get_u16(sig_wait->ind, receiver_pid) != sig_wait->process_id);
		WARN_ON(fapi_get_u16(sig_wait->ind, id) != sig_wait->ind_id);
	}

	sig_wait->ind_id = 0;
	ind = sig_wait->ind;
	sig_wait->ind = NULL;

	SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);

	return ind;
}

/* mib_error: NULL when not required
 * ind: 0 when not required, if used validate_cfm_wait_ind MUST be supplied
 * validate_cfm_wait_ind: NULL when not required, if used ind MUS not be 0
 * NOTE: dev can be NULL!
 */
static struct max_buff *slsi_mlme_tx_rx(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 cfm_id, struct max_buff **mib_error, u16 ind_id, bool(*validate_cfm_wait_ind)(struct slsi_dev *sdev, struct netif *dev, struct max_buff *cfm))
{
	struct max_buff *rx = NULL;
	int err;
	u16 req_id = fapi_get_u16(mbuf, id);
	struct slsi_sig_send *sig_wait = &sdev->sig_wait;

	if (dev) {
		struct netdev_vif *ndev_vif = netdev_priv(dev);

		sig_wait = &ndev_vif->sig_wait;
	}

	if (sdev->mlme_blocked) {
		SLSI_DBG3(sdev, SLSI_MLME, "Rejected. mlme_blocked=%d", sdev->mlme_blocked);
		slsi_kfree_mbuf(mbuf);
		return NULL;
	}
	SLSI_MUTEX_LOCK(sig_wait->mutex);

	SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);
	if (++sig_wait->process_id > SLSI_TX_PROCESS_ID_MAX) {
		sig_wait->process_id = SLSI_TX_PROCESS_ID_MIN;
	}

	WARN_ON(sig_wait->cfm);
	WARN_ON(sig_wait->ind);
	slsi_kfree_mbuf(sig_wait->cfm);
	slsi_kfree_mbuf(sig_wait->ind);
	slsi_kfree_mbuf(sig_wait->mib_error);
	sig_wait->cfm = NULL;
	sig_wait->ind = NULL;
	sig_wait->mib_error = NULL;
	sig_wait->req_id = req_id;
	sig_wait->cfm_id = cfm_id;
	sig_wait->ind_id = ind_id;

	fapi_set_u16(mbuf, sender_pid, sig_wait->process_id);
	SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);

	/* Lock scheduler to ensure supplicant runs through to wait_for_completion_timeout
	 * before FW response is handled */
	sched_lock();
	err = slsi_tx_control(sdev, dev, mbuf, true);
	if (err != 0) {
		SLSI_ERR(sdev, "Failed to send mlme signal:0x%.4X, err=%d\n", req_id, err);
		slsi_kfree_mbuf(mbuf);

		/* Unlock scheduler at the earliest possible opportunity */
		sched_unlock();
		goto clean_exit;
	}

	if (cfm_id) {
		rx = slsi_mlme_wait_for_cfm(sdev, sig_wait);
		if (rx && ind_id) {
			/* The cfm mbuf is owned by the validate_cfm_wait_ind() function and MUST be freed or saved there */
			if (validate_cfm_wait_ind(sdev, dev, rx)) {
				if (sig_wait->ind == NULL) {
					rx = slsi_mlme_wait_for_ind(sdev, dev, sig_wait, ind_id);
				} else {
					rx = sig_wait->ind;
					sig_wait->ind_id = 0;
					sig_wait->ind = NULL;
				}
			} else {
				sig_wait->ind_id = 0;	/* Reset as there is no wait for indication */
				rx = NULL;
			}
		}
	} else if (ind_id) {
		rx = slsi_mlme_wait_for_ind(sdev, dev, sig_wait, ind_id);
	}

	/* Unlock scheduler at the earliest possible opportunity */
	sched_unlock();

	/* The cfm_id and ind_id should ALWAYS be 0 at this point */
	WARN_ON(sig_wait->cfm_id);
	WARN_ON(sig_wait->ind_id);
	WARN_ON(sig_wait->cfm);
	WARN_ON(sig_wait->ind);
clean_exit:

	SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);

	sig_wait->req_id = 0;
	sig_wait->cfm_id = 0;
	sig_wait->ind_id = 0;
	slsi_kfree_mbuf(sig_wait->cfm);
	slsi_kfree_mbuf(sig_wait->ind);
	sig_wait->cfm = NULL;
	sig_wait->ind = NULL;

	if (mib_error) {
		*mib_error = sig_wait->mib_error;
	} else {
		slsi_kfree_mbuf(sig_wait->mib_error);
	}
	sig_wait->mib_error = NULL;
	SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);

	SLSI_MUTEX_UNLOCK(sig_wait->mutex);

	return rx;
}

/**
 * NOTE: dev can be NULL!
 */
int slsi_mlme_req(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	int ret = 0;
	struct slsi_sig_send *sig_wait = &sdev->sig_wait;

	if (dev) {
		struct netdev_vif *ndev_vif = netdev_priv(dev);

		sig_wait = &ndev_vif->sig_wait;
	}
	SLSI_MUTEX_LOCK(sig_wait->send_signal_lock);
	if (++sig_wait->process_id > SLSI_TX_PROCESS_ID_MAX) {
		sig_wait->process_id = SLSI_TX_PROCESS_ID_MIN;
	}
	fapi_set_u16(mbuf, sender_pid, sig_wait->process_id);
	SLSI_MUTEX_UNLOCK(sig_wait->send_signal_lock);

	ret = slsi_tx_control(sdev, dev, mbuf, true);
	if (ret) {
		slsi_kfree_mbuf(mbuf);
	}
	return ret;
}

struct max_buff *slsi_mlme_req_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 ind_id)
{
	if (WARN_ON(!ind_id)) {
		goto err;
	}
	return slsi_mlme_tx_rx(sdev, dev, mbuf, 0, NULL, ind_id, NULL);
err:
	slsi_kfree_mbuf(mbuf);
	return NULL;
}

struct max_buff *slsi_mlme_req_no_cfm(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	return slsi_mlme_tx_rx(sdev, dev, mbuf, 0, NULL, 0, NULL);
}

struct max_buff *slsi_mlme_req_cfm(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 cfm_id)
{
	if (WARN_ON(!cfm_id)) {
		goto err;
	}
	return slsi_mlme_tx_rx(sdev, dev, mbuf, cfm_id, NULL, 0, NULL);
err:
	slsi_kfree_mbuf(mbuf);
	return NULL;
}

/* NOTE: dev can be NULL! */
static inline struct max_buff *slsi_mlme_req_cfm_mib(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 cfm_id, struct max_buff **mib_error)
{
	if (WARN_ON(!cfm_id)) {
		goto err;
	}
	if (WARN_ON(!mib_error)) {
		goto err;
	}
	return slsi_mlme_tx_rx(sdev, dev, mbuf, cfm_id, mib_error, 0, NULL);
err:
	slsi_kfree_mbuf(mbuf);
	return NULL;
}

/* NOTE: dev can be NULL! */
static inline struct max_buff *slsi_mlme_req_cfm_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 cfm_id, u16 ind_id, bool(*validate_cfm_wait_ind)(struct slsi_dev *sdev, struct netif *dev, struct max_buff *cfm))
{
	if (WARN_ON(!cfm_id)) {
		goto err;
	}
	if (WARN_ON(!ind_id)) {
		goto err;
	}
	if (WARN_ON(!validate_cfm_wait_ind)) {
		goto err;
	}

	return slsi_mlme_tx_rx(sdev, dev, mbuf, cfm_id, NULL, ind_id, validate_cfm_wait_ind);

err:
	slsi_kfree_mbuf(mbuf);
	return NULL;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
static struct ieee80211_reg_rule *slsi_get_reg_rule(u32 center_freq, struct slsi_802_11d_reg_domain *domain_info)
{
	struct ieee80211_reg_rule *rule;
	int i;

	for (i = 0; i < domain_info->regdomain->n_reg_rules; i++) {
		rule = &domain_info->regdomain->reg_rules[i];

		/* Consider 10Mhz on both side from the center frequency */
		if (((center_freq - MHZ_TO_KHZ(10)) >= rule->freq_range.start_freq_khz) && ((center_freq + MHZ_TO_KHZ(10)) <= rule->freq_range.end_freq_khz)) {
			return rule;
		}
	}

	return NULL;
}
#endif

u16 slsi_get_chann_info(struct slsi_dev *sdev, struct hostapd_freq_params *chandef)
{
	u16 chann_info;

	SLSI_UNUSED_PARAMETER_NOT_DEBUG(sdev);

	switch (chandef->bandwidth) {
	case 20:
		chann_info = 20;
		break;
	case 40:
		chann_info = 40;
		/* Check HT Minus */
		if (chandef->center_freq1 < chandef->freq) {
			chann_info |= SLSI_CHANN_INFO_HT_SCB;
		}
		break;
	default:
		SLSI_WARN(sdev, "Invalid chandef.width(0x%x)\n", chandef->bandwidth);
		chann_info = 0;
		break;
	}

	SLSI_DBG3(sdev, SLSI_MLME, "channel_width:%u, chann_info:0x%x\n", chandef->bandwidth, chann_info);
	return chann_info;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
int slsi_check_channelization(struct slsi_dev *sdev, struct slsi_80211_chan_def *chandef)
{
	u8 width;
	struct slsi_80211_reg_rule *rule = NULL;
	struct slsi_80211_channel *channel = NULL;

	switch (chandef->width) {
	case SLSI_80211_CHAN_WIDTH_20:
	case SLSI_80211_CHAN_WIDTH_20_NOHT:
		width = 20;
		break;
	case SLSI_80211_CHAN_WIDTH_40:
		width = 40;
		break;

	case SLSI_80211_CHAN_WIDTH_80:
		width = 80;
		break;

	default:
		SLSI_ERR(sdev, "Invalid chandef.width(0x%x)\n", chandef->width);
		return -EINVAL;
	}

	channel = ieee80211_get_channel(sdev->wiphy, chandef->chan->center_freq);
	if (!channel) {
		SLSI_ERR(sdev, "Invalid channel %d used to start AP. Channel not found\n", chandef->chan->center_freq);
		return -EINVAL;
	} else if (channel->flags & (IEEE80211_CHAN_DISABLED | IEEE80211_CHAN_RADAR | IEEE80211_CHAN_PASSIVE_SCAN)) {
		SLSI_ERR(sdev, "Invalid channel %d used to start AP\n", chandef->chan->center_freq);
		return -EINVAL;
	}

	rule = slsi_get_reg_rule(MHZ_TO_KHZ(chandef->center_freq1), &sdev->device_config.domain_info);
	if (!rule) {
		SLSI_ERR(sdev, "Invalid channel %d used to start AP. No reg rule found for this channel\n", chandef->chan->center_freq);
		return -EINVAL;
	}

	if (MHZ_TO_KHZ(width) <= rule->freq_range.max_bandwidth_khz) {
		u32 width_boundary1, width_boundary2;

		width_boundary1 = MHZ_TO_KHZ(chandef->center_freq1 - width / 2);
		width_boundary2 = MHZ_TO_KHZ(chandef->center_freq1 + width / 2);
		if ((width_boundary1 >= rule->freq_range.start_freq_khz) && (width_boundary2 <= rule->freq_range.end_freq_khz)) {
			return 0;
		}
		SLSI_ERR(sdev, "Invalid channel %d used to start AP. Channel not within frequency range of the reg rule\n", chandef->chan->center_freq);
		return -EINVAL;
	}
	return -EINVAL;
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

/* Called in the case of MIB SET errors.
 * Decode and print a MIB buffer to the log for debug purposes.
 */
#ifdef CONFIG_SCSC_ADV_FEATURE
static void mib_buffer_dump_to_log(struct slsi_dev *sdev, u8 *mib_buffer, unsigned int mib_buffer_len)
{
	size_t mib_decode_result;
	size_t offset = 0;
	struct slsi_mib_entry decoded_mib_value;
	struct slsi_mib_data mibdata;
	int error_out_len = mib_buffer_len * 3;
	int error_out_pos = 0;
	char *error_out;

	SLSI_UNUSED_PARAMETER(sdev);

	SLSI_ERR(sdev, "MIB buffer length: %u. MIB Error (decoded):", mib_buffer_len);

	if (mib_buffer == NULL) {
		SLSI_ERR(sdev, "MIB buffer pointer is NULL - can not decode MIB keys\n");
		return;
	}
	error_out = kmm_malloc(error_out_len);
	if (error_out == NULL) {
		SLSI_ERR(sdev, "Alloc failed - cannot decode\n");
		return;
	}

	while (offset < mib_buffer_len) {
		error_out_pos = 0;
		mibdata.data = &mib_buffer[offset];
		mibdata.dataLength = mib_buffer_len - offset;

		mib_decode_result = slsi_mib_decode(&mibdata, &decoded_mib_value);

		if (!mib_decode_result) {
			SLSI_ERR(sdev, "slsi_mib_decode() Failed to Decode:\n");
			break;
		}
		offset += mib_decode_result;
		/* Time for some eye candy - output the decoded MIB key at error level in the log */
		error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "%d", (int)(decoded_mib_value.psid));
		if (decoded_mib_value.index[0]) {
			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, ".%d", (int)(decoded_mib_value.index[0]));
			if (decoded_mib_value.index[1]) {
				error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, ".%d", (int)(decoded_mib_value.index[1]));
			}
		}

		switch (decoded_mib_value.value.type) {
		case SLSI_MIB_TYPE_BOOL:
			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "=%s\n", decoded_mib_value.value.u.boolValue ? "TRUE" : "FALSE");
			break;
		case SLSI_MIB_TYPE_UINT:
			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "=%d\n", (int)decoded_mib_value.value.u.uintValue);
			break;
		case SLSI_MIB_TYPE_INT:
			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "=%d\n", (int)decoded_mib_value.value.u.intValue);
			break;
		case SLSI_MIB_TYPE_OCTET: {
			u32 i;

			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "=[");
			for (i = 0; i < decoded_mib_value.value.u.octetValue.dataLength; i++) {
				error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "%.2X", (int)decoded_mib_value.value.u.octetValue.data[i]);
			}
			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "]\n");
			break;
		}
		default:
			error_out_pos += snprintf(error_out + error_out_pos, error_out_len - error_out_pos, "=Can not decode MIB key type\n");
			break;
		}
		SLSI_INFO_NODEV("%s", error_out);
	}
	kmm_free(error_out);
}
#endif
#ifndef CONFIG_SCSC_WLAN_BLOCK_IPV6

int slsi_mlme_set_ipv6_address(struct slsi_dev *sdev, struct netif *dev)
{
	struct max_buff *req;
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *cfm;
	int r = 0;
	u8 solicited_node_addr[ETH_ALEN] = { 0x33, 0x33, 0xff, 0x00, 0x00, 0x00 };

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_SET_IP_ADDRESS.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_MUTEX_LOCK(ndev_vif->ipv6addr_lock);
	memcpy(&solicited_node_addr[3], &ndev_vif->ipv6address.s6_addr[13], 3);
	SLSI_MUTEX_UNLOCK(ndev_vif->ipv6addr_lock);

	req = fapi_alloc(mlme_set_ip_address_req, MLME_SET_IP_ADDRESS_REQ, ndev_vif->ifnum, 16);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_set_ip_address_req.ip_version, 6);
	fapi_set_memcpy(req, u.mlme_set_ip_address_req.multicast_address, solicited_node_addr);
	fapi_append_data(req, ndev_vif->ipv6address.s6_addr, 16);

	SLSI_DBG2(sdev, SLSI_MLME, "mlme_set_ip_address_req(vif: %d, IP: %pI6)\n", ndev_vif->ifnum, &ndev_vif->ipv6address);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_IP_ADDRESS_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_ip_address_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_ip_address_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_set_ip_address_cfm.result_code));
		r = -EINVAL;
	}
	slsi_kfree_mbuf(cfm);
	return r;
}
#endif

int slsi_mlme_set(struct slsi_dev *sdev, struct netif *dev, u8 *mib, int mib_len)
{
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;
	u16 ifnum = 0;

	if (dev) {
		struct netdev_vif *ndev_vif = netdev_priv(dev);

		ifnum = ndev_vif->ifnum;
	}

	SLSI_DBG2(sdev, SLSI_MLME, "mlme_set_req(vif:%d, mib_len:%d)\n", ifnum, mib_len);
	req = fapi_alloc(mlme_set_req, MLME_SET_REQ, ifnum, mib_len);
	if (!req) {
		return -ENOMEM;
	}

	fapi_append_data(req, mib, mib_len);

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_CFM);
	if (!cfm) {
		return -EIO;
	}

	SLSI_DBG2(sdev, SLSI_MLME, "mlme_set_cfm(datalen:%u)\n", fapi_get_datalen(cfm));
	if (fapi_get_datalen(cfm)) {
#ifdef CONFIG_SCSC_ADV_FEATURE
		mib_buffer_dump_to_log(sdev, fapi_get_data(cfm), fapi_get_datalen(cfm));
#endif
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);

	return r;
}

int slsi_mlme_get(struct slsi_dev *sdev, struct netif *dev, u8 *mib, int mib_len, u8 *resp, int resp_buf_len, int *resp_len)
{
	struct max_buff *req;
	struct max_buff *err = NULL;
	struct max_buff *cfm;
	int r = 0;
	u16 ifnum = 0;

	SLSI_DBG3(sdev, SLSI_MLME, "mlme_get_req\n");

	if (dev) {
		struct netdev_vif *ndev_vif = netdev_priv(dev);

		ifnum = ndev_vif->ifnum;
	}
	req = fapi_alloc(mlme_get_req, MLME_GET_REQ, ifnum, mib_len);
	if (!req) {
		return -ENOMEM;
	}
	fapi_append_data(req, mib, mib_len);

	cfm = slsi_mlme_req_cfm_mib(sdev, dev, req, MLME_GET_CFM, &err);
	if (!cfm) {
		return -EIO;
	}

	SLSI_DBG3(sdev, SLSI_MLME, "mlme_get_cfm (datalen: %u)", fapi_get_datalen(cfm));
	if (err) {
		SLSI_DBG1(sdev, SLSI_MLME, "ERROR: mlme_get_cfm with mib error");
#ifdef CONFIG_SCSC_ADV_FEATURE
		mib_buffer_dump_to_log(sdev, fapi_get_data(err), fapi_get_datalen(err));
#endif
		if (fapi_get_datalen(cfm) > resp_buf_len) {
			SLSI_ERR(sdev, "Insufficient resp_buf_len(%d). mlme_get_cfm(%d)\n", resp_buf_len, fapi_get_datalen(cfm));
		}
		r = -EINVAL;
	}

	/* if host has requested for multiple PSIDs in same request, we can get a
	 *   combination of error and success
	 */
	if (fapi_get_datalen(cfm) <= resp_buf_len) {
		*resp_len = fapi_get_datalen(cfm);
		memcpy(resp, fapi_get_data(cfm), fapi_get_datalen(cfm));
		r = 0;
	} else {
		SLSI_WARN(sdev, "Insufficient length (%d) to read MIB values, expected =%d\n", resp_buf_len, fapi_get_datalen(cfm));
	}

	slsi_kfree_mbuf(err);
	slsi_kfree_mbuf(cfm);

	return r;
}

int slsi_mlme_add_vif(struct slsi_dev *sdev, struct netif *dev, u8 *interface_address, u8 *device_address)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_ADD_VIF.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_add_vif_req(vif:%d)\n", ndev_vif->ifnum);
	req = fapi_alloc(mlme_add_vif_req, MLME_ADD_VIF_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return -ENOMEM;
	}
	fapi_set_u16(req, u.mlme_add_vif_req.virtual_interface_type, ndev_vif->vif_type);
	fapi_set_memcpy(req, u.mlme_add_vif_req.interface_address, interface_address);
	fapi_set_memcpy(req, u.mlme_add_vif_req.device_address, device_address);

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_ADD_VIF_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_add_vif_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_add_vif_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_add_vif_cfm.result_code));
		r = -EINVAL;
	}
#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
	/* By default firmware vif will be in active mode */
	ndev_vif->power_mode = FAPI_POWERMANAGEMENTMODE_ACTIVE_MODE;
#endif

	slsi_kfree_mbuf(cfm);
	return r;
}

void slsi_mlme_del_vif(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_DEL_VIF.request\n");
		return;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_del_vif_req(vif:%d)\n", ndev_vif->ifnum);
	req = fapi_alloc(mlme_del_vif_req, MLME_DEL_VIF_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return;
	}

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_DEL_VIF_CFM);
	if (!cfm) {
		return;
	}

	if (fapi_get_u16(cfm, u.mlme_del_vif_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_del_vif_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_del_vif_cfm.result_code));
		r = -EINVAL;
	}
#ifdef CONFIG_SCSC_ENABLE_P2P
	if ((ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_CLIENT) && (sdev->delete_gc_probe_req_ies == true)) {
		kmm_free(sdev->gc_probe_req_ies);
		sdev->gc_probe_req_ies = NULL;
		sdev->gc_probe_req_ie_len = 0;
		sdev->delete_gc_probe_req_ies = false;
	}
#endif

	slsi_kfree_mbuf(cfm);
}

#ifdef CONFIG_SCSC_ENABLE_P2P
int slsi_mlme_set_channel(struct slsi_dev *sdev, struct netif *dev, struct ieee80211_channel *chan, u16 duration, u16 interval, u16 count)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "wlanlite does not support MLME_SET_CHANNEL.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_set_channel_req(freq:%u, duration:%u, interval:%u, count:%u)\n", chan->center_freq, duration, interval, count);

	req = fapi_alloc(mlme_set_channel_req, MLME_SET_CHANNEL_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_set_channel_req.availability_duration, duration);
	fapi_set_u16(req, u.mlme_set_channel_req.availability_interval, interval);
	fapi_set_u16(req, u.mlme_set_channel_req.count, count);
	fapi_set_u16(req, u.mlme_set_channel_req.channel_frequency, SLSI_FREQ_HOST_TO_FW(chan->center_freq));

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_CHANNEL_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_channel_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_channel_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_set_channel_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}
#endif

static inline bool is_channel_active(struct slsi_80211_channel *channel)
{
	/* Ignore disabled channels, radar channels are ALWAYS passive */
	if ((channel->flags & SLSI_80211_CHAN_DISABLED) || (channel->flags & SLSI_80211_CHAN_RADAR)) {
		return false;
	}

	if (!(channel->flags & SLSI_80211_CHAN_NO_IR)) {
		return true;
	}

	return false;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
void slsi_ap_obss_scan_done_ind(struct netif *dev, struct netdev_vif *ndev_vif)
{
	struct max_buff *scan_res;
	u16 scan_id = SLSI_SCAN_HW_ID;

	SLSI_UNUSED_PARAMETER(dev);

	SLSI_NET_DBG1(dev, SLSI_MLME, "Scan before AP start completed\n");

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->scan_mutex));

	scan_res = slsi_mbuf_dequeue(&ndev_vif->scan[scan_id].scan_results);
	while (scan_res) {
		struct ieee80211_mgmt *mgmt = fapi_get_mgmt(scan_res);
		size_t mgmt_len = fapi_get_mgmtlen(scan_res);
		size_t ie_len = mgmt_len - offsetof(struct ieee80211_mgmt, u.beacon.variable);	/* ieee80211_mgmt structure is similar for Probe Response and Beacons */

		SLSI_NET_DBG4(dev, SLSI_MLME, "OBSS scan result (scan_id:%d, %pM, freq:%d, rssi:%d, ie_len = %zu)\n", fapi_get_u16(scan_res, u.mlme_scan_ind.scan_id), fapi_get_mgmt(scan_res)->bssid, fapi_get_u16(scan_res, u.mlme_scan_ind.channel_frequency) / 2, fapi_get_s16(scan_res, u.mlme_scan_ind.rssi), ie_len);

		if (!slsi_80211_find_ie(WLAN_EID_HT_CAPABILITY, mgmt->u.beacon.variable, ie_len)) {
			SLSI_NET_DBG1(dev, SLSI_MLME, "Non HT BSS detected on primary channel\n");
			ndev_vif->ap.non_ht_bss_present = true;
		}

		slsi_kfree_mbuf(scan_res);
		scan_res = slsi_mbuf_dequeue(&ndev_vif->scan[scan_id].scan_results);
	}
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

/* Null check for cfm done in caller function */
static bool slsi_scan_cfm_validate(struct slsi_dev *sdev, struct netif *dev, struct max_buff *cfm)
{
	bool r = true;

	if (fapi_get_u16(cfm, u.mlme_add_scan_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_ERR_NODEV("mlme_add_scan_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_add_scan_cfm.result_code));
		r = false;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

static int slsi_mlme_append_channel_list(struct slsi_dev *sdev, struct netif *dev, struct max_buff *req, u32 num_channels, struct slsi_80211_channel *channels[])
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int chann;
	u16 freq_fw_unit;
	u8 i;
	int n_valid_channels = 0;
	u8 *p;

	const u8 channels_list_ie_header[] = {
		0xDD,					/* Element ID: vendor specific */
		0x05,					/* Length: actual length will be updated later */
		0x00, 0x16, 0x32,		/* OUI: Samsung Electronics Co. */
		0x01,					/* OUI Type: scan parameters */
		0x02					/* OUI Subtype: channel list */
	};

	u8 *channels_list_ie = fapi_append_data(req, channels_list_ie_header, sizeof(channels_list_ie_header));

	if (!channels_list_ie) {
		SLSI_WARN(sdev, "channel list IE append failed\n");
		slsi_kfree_mbuf(req);
		return -EINVAL;
	}

	for (i = 0; i < num_channels; i++) {
		chann = channels[i]->hw_value & 0xFF;
		if (channels[i]->flags & SLSI_80211_CHAN_DISABLED) {
			continue;
		}

		if (sdev->device_config.supported_band) {
			if (channels[i]->band == SLSI_80211_BAND_2GHZ && sdev->device_config.supported_band != SLSI_FREQ_BAND_2GHZ) {
				continue;
			}
			if (channels[i]->band == SLSI_80211_BAND_5GHZ && sdev->device_config.supported_band != SLSI_FREQ_BAND_5GHZ) {
				continue;
			}
		}

		/* filter Passive channels */
		if (!is_channel_active(channels[i])) {
			continue;
		}

		//  SLSI_NET_DBG3(dev, SLSI_MLME, "c:%3d,b:%d,f:%d,f:%08x\n",
		//        chann, channels[i]->band, channels[i]->center_freq, channels[i]->flags);

		n_valid_channels++;
		p = fapi_append_data(req, NULL, SLSI_SCAN_CHANNEL_DESCRIPTOR_SIZE);
		if (!p) {
			SLSI_WARN(sdev, "scan channel descriptor append failed\n");
			slsi_kfree_mbuf(req);
			return -EINVAL;
		}
		freq_fw_unit = SLSI_FREQ_HOST_TO_FW(channels[i]->center_freq);
		freq_fw_unit = cpu_to_le16(freq_fw_unit);
		memcpy(p, &freq_fw_unit, sizeof(freq_fw_unit));

		if (ndev_vif->ifnum == SLSI_NET_INDEX_WLAN) {
			p[2] = FAPI_SCANPOLICY_ANY_RA;
		} else {
			p[2] = 0;
		}

		channels_list_ie[1] += SLSI_SCAN_CHANNEL_DESCRIPTOR_SIZE;
	}

	if (n_valid_channels == 0) {
		SLSI_NET_ERR(dev, "No valid channels to Scan\n");
		slsi_kfree_mbuf(req);
		return -EINVAL;
	}

	return 0;
}

static inline int slsi_set_scan_params(struct netif *dev, u16 scan_id, u16 scan_type, u16 report_mode, int num_ssids, struct wpa_driver_scan_ssid *ssids, struct max_buff *req)
{
	u8 *p = NULL;
	u8 i;
	struct wpa_driver_scan_ssid *pssid = ssids;

	fapi_set_u16(req, u.mlme_add_scan_req.scan_id, scan_id);
	fapi_set_u16(req, u.mlme_add_scan_req.scan_type, scan_type);
	fapi_set_u16(req, u.mlme_add_scan_req.report_mode_bitmap, report_mode);
	fapi_set_memcpy(req, u.mlme_add_scan_req.device_address, dev->d_mac.ether_addr_octet);

	for (i = 0; i < num_ssids; i++, pssid++) {
		p = fapi_append_data(req, NULL, 2 + pssid->ssid_len);
		if (!p) {
			slsi_kfree_mbuf(req);
			SLSI_NET_WARN(dev, "fail to append SSID element to scan request\n");
			return -EINVAL;
		}

		*p++ = WLAN_EID_SSID;
		*p++ = pssid->ssid_len;
		if (pssid->ssid_len) {
			memcpy(p, pssid->ssid, pssid->ssid_len);
			SLSI_NET_DBG2(dev, SLSI_MLME, "SSID: %.*s\n", pssid->ssid_len, pssid->ssid);
		} else {
			SLSI_NET_DBG2(dev, SLSI_MLME, "wild card SSID\n");
		}
	}
	return 0;
}

int slsi_mlme_add_scan(struct slsi_dev *sdev, struct netif *dev, u16 scan_type, u16 report_mode, u32 n_ssids, struct wpa_driver_scan_ssid *ssids, u32 n_channels, struct slsi_80211_channel *channels[], void *gscan, const u8 *ies, u16 ies_len, bool wait_for_ind)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *rx;
	int r = 0;
	size_t alloc_data_size = 0;
	u32 i;

	/* Scan Timing IE: default values */
	u8 scan_timing_ie[] = {
		0xdd,					/* Element ID: Vendor Specific */
		0x11,					/* Length */
		0x00, 0x16, 0x32,		/* OUI: Samsung Electronics Co. */
		0x01,					/* OUI Type: Scan parameters */
		0x01,					/* OUI Subtype: Scan timing */
		0x00, 0x00, 0x00, 0x00,	/* Min_Period:  filled later in the function */
		0x00, 0x00, 0x00, 0x00,	/* Max_Period:  filled later in the function */
		0x00,					/* Exponent */
		0x00,					/* Step count */
		0x00, 0x00				/* Skip first period: false */
	};

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_WARN(dev, "not supported in WlanLite mode\n");
		return -EOPNOTSUPP;
	}

	if (WARN_ON(!(dev->d_mac.ether_addr_octet))) {
		return -EINVAL;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->scan_mutex));
	SLSI_NET_DBG3(dev, SLSI_MLME, "id:0x%x, n_channels:%d\n", (ndev_vif->ifnum << 8 | SLSI_SCAN_HW_ID), n_channels);

	alloc_data_size += sizeof(scan_timing_ie) + ies_len + (SLSI_SCAN_PRIVATE_IE_CHANNEL_LIST_HEADER_LEN + (n_channels * SLSI_SCAN_CHANNEL_DESCRIPTOR_SIZE));

	for (i = 0; i < n_ssids; i++) {
		alloc_data_size += 2 + ssids[i].ssid_len;    /* 2: SSID EID + len */
	}

	req = fapi_alloc(mlme_add_scan_req, MLME_ADD_SCAN_REQ, 0, alloc_data_size);
	if (!req) {
		return -ENOMEM;
	}

	if (gscan == NULL) {
		r = slsi_set_scan_params(dev, (ndev_vif->ifnum << 8 | SLSI_SCAN_HW_ID), scan_type, report_mode, n_ssids, ssids, req);
		if (r) {
			return r;
		}

		fapi_append_data(req, scan_timing_ie, sizeof(scan_timing_ie));
		fapi_append_data(req, ies, ies_len);

		if (n_channels) {
			r = slsi_mlme_append_channel_list(sdev, dev, req, n_channels, channels);
			if (r) {
				return r;
			}
		}
	}

	if (wait_for_ind) {
		/* Use the Global sig_wait not the Interface specific for Scan Req */
		rx = slsi_mlme_req_cfm_ind(sdev, NULL, req, MLME_ADD_SCAN_CFM, MLME_SCAN_DONE_IND, slsi_scan_cfm_validate);
		if (!rx) {
			return -EIO;
		}
		SLSI_NET_DBG3(dev, SLSI_MLME, "mlme_scan_done_ind()\n");

		/* slsi_mlme_add_scan is a generic definition for multiple handlers
		 * Any added functionality, if not generic, should not be defined here.
		 * It should be a part of calling function.
		 */
	} else {
		/* Use the Global sig_wait not the Interface specific for Scan Req */
		rx = slsi_mlme_req_cfm(sdev, NULL, req, MLME_ADD_SCAN_CFM);
		if (!rx) {
			return -EIO;
		}
		if (fapi_get_u16(rx, u.mlme_add_scan_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
			SLSI_NET_ERR(dev, "mlme_add_scan_cfm(result:%u) ERROR\n", fapi_get_u16(rx, u.mlme_add_scan_cfm.result_code));
			r = -EINVAL;
		}
	}

	slsi_kfree_mbuf(rx);
	return r;
}

int slsi_mlme_del_scan(struct slsi_dev *sdev, struct netif *dev, u16 scan_id)
{
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_WARN(dev, "Not supported in WlanLite mode\n");
		return -EOPNOTSUPP;
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_del_scan_req(scan_id:%d)\n", scan_id);

	req = fapi_alloc(mlme_del_scan_req, MLME_DEL_SCAN_REQ, 0, 0);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_del_scan_req.scan_id, scan_id);

	/* Use the Global sig_wait not the Interface specific for Scan Req */
	cfm = slsi_mlme_req_cfm(sdev, NULL, req, MLME_DEL_SCAN_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_del_scan_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_del_scan_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_del_scan_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

static void slsi_ap_add_ext_capab_ie(struct max_buff *req, struct netdev_vif *ndev_vif)
{
	u8 ext_capa_ie[SLSI_AP_EXT_CAPAB_IE_LEN];
	int i;

	ext_capa_ie[0] = SLSI_WLAN_EID_EXT_CAPABILITY;
	ext_capa_ie[1] = SLSI_AP_EXT_CAPAB_IE_LEN - 1 - 1;

	for (i = 2; i < SLSI_AP_EXT_CAPAB_IE_LEN; i++) {
		ext_capa_ie[i] = 0x00;
	}

	/* For VHT, set the Operating Mode Notification field - Bit 62 (8th Octet) */
	ext_capa_ie[SLSI_AP_EXT_CAPAB_IE_LEN - 1] |= 0x40;

	fapi_append_data(req, &ext_capa_ie[0], SLSI_AP_EXT_CAPAB_IE_LEN);
}

#ifdef CONFIG_SCSC_ADV_FEATURE
static int slsi_prepare_country_ie(struct slsi_dev *sdev, u16 center_freq, u8 *country_ie, u8 **new_country_ie)
{
	struct ieee80211_supported_band band;
	struct ieee80211_reg_rule *rule;
	struct slsi_80211_channel *channels;
	u8 *ie;
	u8 offset = 0;
	int i;

	/* Select frequency band */
	if (center_freq < 5180) {
		band = slsi_band_2ghz;
	} else {
		band = slsi_band_5ghz;
	}

	/* Allocate memory for the new country IE - EID(1) + Len(1) + CountryString(3) + ChannelInfo (n * 3) */
	ie = kmm_malloc(5 + (band.n_channels * 3));
	if (!ie) {
		SLSI_ERR(sdev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	/* Preapre the new country IE */
	ie[offset++] = country_ie[0];	/* Element IE */
	ie[offset++] = 0;			/* IE Length - initialized at the end of this function */
	ie[offset++] = sdev->device_config.domain_info.regdomain->alpha2[0];	/* Country code */
	ie[offset++] = sdev->device_config.domain_info.regdomain->alpha2[1];	/* Country code */
	ie[offset++] = country_ie[4];	/* CountryString: 3rd octet */

	channels = band.channels;
	for (i = 0; i < band.n_channels; i++, channels++) {
		/* Get the regulatory rule for the channel */
		rule = slsi_get_reg_rule(MHZ_TO_KHZ(channels->center_freq), &sdev->device_config.domain_info);
		if (rule != NULL) {
			ie[offset++] = channels->hw_value;	/* Channel number */
			ie[offset++] = 1;	/* Number of channels */
			ie[offset++] = MBM_TO_DBM(rule->power_rule.max_eirp);	/* Max TX power */
		}
	}

	ie[1] = offset - 2;			/* Length of IE */
	*new_country_ie = ie;

	return 0;
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

int slsi_modify_ies(struct netif *dev, u8 eid, u8 *ies, int ies_len, u8 ie_index, u8 ie_value)
{
	u8 *ie;

	SLSI_NET_DBG1(dev, SLSI_MLME, "eid: %d, ie_value = 0x%x\n", eid, ie_value);

	ie = (u8 *)slsi_80211_find_ie_mod(eid, ies, ies_len);
	if (ie) {
		switch (eid) {
		case WLAN_EID_HT_OPERATION:
		case WLAN_EID_HT_CAPABILITY:
		case WLAN_EID_VHT_CAPABILITY:
			ie[ie_index] |= ie_value;
			break;
		default:
			SLSI_NET_WARN(dev, "slsi_modify_ies: IE type mismatch : %d\n", eid);
			return false;
		}
		return true;
	}
	SLSI_NET_WARN(dev, "slsi_modify_ies: IE not found : %d\n", eid);
	return false;
}

static void slsi_mlme_start_prepare_ies(struct max_buff *req, struct netif *dev, struct netdev_vif *ndev_vif, struct wpa_driver_ap_params *settings, const u8 *wpa_ie_pos, const u8 *wmm_ie_pos)
{
	const u8 *wps_ie, *vht_capab_ie, *tail_pos = NULL;
	size_t beacon_ie_len = 0, tail_length = 0;

#ifdef CONFIG_SCSC_ADV_FEATURE
	u8 *country_ie;
#endif
	const u8 *beacon_tail = (const u8 *)settings->tail;
	size_t beacon_tail_len = settings->tail_len;

#ifdef CONFIG_SCSC_ADV_FEATURE
	/**
	 * Channel list of Country IE prepared by hostapd is wrong, so driver needs remove the existing country IE and prepare correct one.
	 * Hostapd adds country IE at the beginning of the tail, beacon_tail is moved to the next IE to avoid the default county IE.
	 */
	country_ie = (u8 *)slsi_80211_find_ie(WLAN_EID_COUNTRY, beacon_tail, beacon_tail_len);
	if (country_ie) {
		u8 *new_country_ie = NULL;

		SLSI_DBG3(ndev_vif->sdev, SLSI_MLME, "Country IE found, length = %d", country_ie[1]);

		/* Prepare the new country IE */
		if (slsi_prepare_country_ie(ndev_vif->sdev, ndev_vif->chan->center_freq, country_ie, &new_country_ie) != 0) {
			SLSI_ERR(ndev_vif->sdev, "Failed to prepare country IE");
		}

		/* Add the new country IE */
		if (new_country_ie) {
			/* new_country_ie[1] ontains the length of IE */
			fapi_append_data(req, new_country_ie, (new_country_ie[1] + 2));

			/* Free the memory allocated for the new country IE */
			kmm_free(new_country_ie);

			/* Remove the default country IE from the beacon_tail */
			beacon_tail += (country_ie[1] + 2);
			beacon_tail_len -= (country_ie[1] + 2);
		}
	}
#endif

#ifdef CONFIG_SCSC_ADV_FEATURE
	/* Modify HT IE based on OBSS scan data */
	if (ndev_vif->ap.non_ht_bss_present) {
		u8 op_mode = 1;

		SLSI_NET_DBG1(dev, SLSI_MLME, "Modify Operating mode of BSS in HT IE\n");
		slsi_modify_ies(dev, WLAN_EID_HT_OPERATION, settings->tail, settings->tail_len, 4, op_mode);
		ndev_vif->ap.non_ht_bss_present = false;
	}
#endif

	/* Vendor IEs are excluded from start_req. Currently WPA IE, WMM IE, WPS IE and P2P IE need to be excluded.
	 * From hostapd, order of IEs are - WPA, WMM, WPS and P2P
	 * Of these the WMM, WPS and P2P IE are usually at the end.
	 * Note: There can be "eid_p2p_manage" and "eid_hs20" after WPS and P2P IE. Both of these are currently not supported.
	 */

	/* Exclude WMM or WPS IE */
	if (wmm_ie_pos) {			/* WMM IE is present. Remove from this position onwards, i.e. copy only till this data. WPS and P2P IE will also get removed. */
		beacon_ie_len = wmm_ie_pos - beacon_tail;
	} else {
		/* WMM IE is not present. Check for WPS IE (and thereby P2P IE) and exclude it */
		wps_ie = slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPS, beacon_tail, beacon_tail_len);
		if (wps_ie) {
			beacon_ie_len = wps_ie - beacon_tail;
		} else {
			beacon_ie_len = beacon_tail_len;
		}
	}

	/* Exclude WPA IE if present */
	if (wpa_ie_pos) {
		size_t len_before, len;

		len_before = wpa_ie_pos - beacon_tail;
		fapi_append_data(req, beacon_tail, len_before);

		len = len_before + ndev_vif->ap.wpa_ie_len;

		if (beacon_ie_len > len) {	/* More IEs to go */
			tail_length = beacon_ie_len - len;
			tail_pos = (beacon_tail + len);
		} else {				/* No more IEs, don't add Ext Capab IE as no HT/VHT */
			return;
		}
	} else {
		tail_length = beacon_ie_len;
		tail_pos = beacon_tail;
	}

	/* Add Ext Capab IE only for VHT mode for now */
	if (ndev_vif->chandef->bandwidth == 80) {
		/* Ext Capab should be before VHT IEs */
		vht_capab_ie = (slsi_80211_find_ie(WLAN_EID_VHT_CAPABILITY, tail_pos, tail_length));
		if (vht_capab_ie) {
			size_t len_before_vht;

			len_before_vht = vht_capab_ie - tail_pos;
			fapi_append_data(req, tail_pos, len_before_vht);	/* Append IEs till VHT Capab IE */
			slsi_ap_add_ext_capab_ie(req, ndev_vif);
			fapi_append_data(req, vht_capab_ie, tail_length - len_before_vht);	/* Append the VHT Capab IE onwards */
		} else {
			/* VHT IE is added later by driver for P2PGO case (for default 80 MHz) and so it may not be present as well
			 * Append Ext Capab at the end for such cases
			 */
			fapi_append_data(req, tail_pos, tail_length);
			slsi_ap_add_ext_capab_ie(req, ndev_vif);
		}
	} else {
		fapi_append_data(req, tail_pos, tail_length);
	}
}

/*EID + LEN + CAPABILITIES + MCS */
/* 1+1+4+8 */
#define SLSI_VHT_CAPABILITIES_IE_LEN 14

/* EID + LEN + WIDTH + SEG0 + SEG1 + MCS */
/* 1+1+1+1+1+2 */
#define SLSI_VHT_OPERATION_IE_LEN 7

#ifdef CONFIG_SCSC_ADV_FEATURE
static int slsi_prepare_vht_ies(struct netif *dev, u8 **vht_ie_capab, u8 **vht_ie_operation)
{
	u32 capabs;
	u16 mcs;
	u8 *p_cap, *p_oper;
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	*vht_ie_capab = kmm_malloc(SLSI_VHT_CAPABILITIES_IE_LEN);
	if (!(*vht_ie_capab)) {
		return -EINVAL;
	}
	*vht_ie_operation = kmm_malloc(SLSI_VHT_OPERATION_IE_LEN);
	if (!(*vht_ie_operation)) {
		kmm_free(*vht_ie_capab);
		return -EINVAL;
	}

	p_cap = *vht_ie_capab;
	p_oper = *vht_ie_operation;

	*p_cap++ = WLAN_EID_VHT_CAPABILITY;
	*p_cap++ = SLSI_VHT_CAPABILITIES_IE_LEN - 1 - 1;
	capabs = cpu_to_le32(slsi_vht_cap.cap);
	memcpy(p_cap, &capabs, sizeof(capabs));
	p_cap += sizeof(capabs);
	memcpy(p_cap, &slsi_vht_cap.vht_mcs, sizeof(slsi_vht_cap.vht_mcs));

	*p_oper++ = WLAN_EID_VHT_OPERATION;
	*p_oper++ = SLSI_VHT_OPERATION_IE_LEN - 1 - 1;
	*p_oper++ = IEEE80211_VHT_CHANWIDTH_80MHZ;
	*p_oper++ = ieee80211_frequency_to_channel(ndev_vif->chandef->center_freq1);
	*p_oper++ = 0;
	mcs = cpu_to_le16(0xfffc);
	memcpy(p_oper, &mcs, sizeof(mcs));

	return 0;
}
#endif

int slsi_mlme_start(struct slsi_dev *sdev, struct netif *dev, u8 *bssid, struct wpa_driver_ap_params *settings, const u8 *wpa_ie_pos, const u8 *wmm_ie_pos, bool append_vht_ies)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	struct ieee80211_mgmt *mgmt;
	int r = 0;
	u8 *p;
	enum slsi_80211_auth_type auth_type = SLSI_80211_AUTHTYPE_UNSUPPORTED;
	u16 beacon_ie_head_len;
	u16 chan_info;
	u16 fw_freq;
	u16 vht_ies_len = 0;
	u8 ext_capab_len = 0;

#ifdef CONFIG_SCSC_ADV_FEATURE
	u8 *vht_ie_capab, *vht_ie_operation;
#endif

	SLSI_UNUSED_PARAMETER(bssid);

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_START.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (WARN_ON(!settings->head_len || !settings->head)) {
		return -EINVAL;
	}
	if (WARN_ON(!settings->ssid_len || !settings->ssid)) {
		return -EINVAL;
	}
	if (WARN_ON(!settings->beacon_int)) {
		return -EINVAL;
	}

	mgmt = (struct ieee80211_mgmt *)settings->head;
	beacon_ie_head_len = settings->head_len - ((u8 *)mgmt->u.beacon.variable - (u8 *)mgmt);

	/* For port enabling, save the privacy bit used in assoc response or beacon */
	if (mgmt->u.beacon.capab_info & WLAN_CAPABILITY_PRIVACY) {
		ndev_vif->ap.privacy = 1;
	} else {
		ndev_vif->ap.privacy = 0;
	}

	if (settings->auth_algs & WPA_AUTH_ALG_OPEN) {
		auth_type = SLSI_80211_AUTHTYPE_OPEN_SYSTEM;
	} else if (settings->auth_algs & WPA_AUTH_ALG_SHARED) {
		auth_type = SLSI_80211_AUTHTYPE_SHARED_KEY;
	} else {
		SLSI_NET_ERR(dev, "Unsupported auth_type: %d\n", settings->auth_algs);
	}

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_start_req(vif:%u, bssid:" SLSI_MAC_FORMAT ", ssid:%.*s)\n", ndev_vif->ifnum, SLSI_MAC_STR(bssid), (int)settings->ssid_len, settings->ssid);

	if (append_vht_ies) {
		vht_ies_len = SLSI_VHT_CAPABILITIES_IE_LEN + SLSI_VHT_OPERATION_IE_LEN;
	}

	if (ndev_vif->chandef->bandwidth == 80) {
		/* Ext Capab are not advertised by driver and so the IE would not be sent by hostapd.
		 * Frame the IE in driver and set the required bit(s).
		 */
		SLSI_NET_DBG1(dev, SLSI_MLME, "VHT - Ext Capab IE to be included\n");
		ext_capab_len = SLSI_AP_EXT_CAPAB_IE_LEN;
	}

	if (append_vht_ies) {
		vht_ies_len = SLSI_VHT_CAPABILITIES_IE_LEN + SLSI_VHT_OPERATION_IE_LEN;
	}

	if (settings->hide_ssid == 1) {
		req = fapi_alloc(mlme_start_req, MLME_START_REQ, ndev_vif->ifnum, settings->ssid_len + beacon_ie_head_len + settings->tail_len + vht_ies_len + ext_capab_len);
		if (!req) {
			return -ENOMEM;
		}
	} else {
		req = fapi_alloc(mlme_start_req, MLME_START_REQ, ndev_vif->ifnum, beacon_ie_head_len + settings->tail_len + vht_ies_len + ext_capab_len);
		if (!req) {
			return -ENOMEM;
		}
	}

	fapi_set_memcpy(req, u.mlme_start_req.bssid, dev->d_mac.ether_addr_octet);
	fapi_set_u16(req, u.mlme_start_req.beacon_period, settings->beacon_int);
	fapi_set_u16(req, u.mlme_start_req.dtim_period, settings->dtim_period);
	fapi_set_u16(req, u.mlme_start_req.capability_information, le16_to_cpu(mgmt->u.beacon.capab_info));
	fapi_set_u16(req, u.mlme_start_req.authentication_type, auth_type);
	fapi_set_u16(req, u.mlme_start_req.hidden_ssid, settings->hide_ssid < 3 ? settings->hide_ssid : 0);

	fw_freq = ndev_vif->chandef->freq;

	chan_info = slsi_get_chann_info(sdev, ndev_vif->chandef);

	if ((chan_info & 20) != 20) {
		fw_freq = slsi_get_center_freq1(sdev, chan_info, fw_freq);
	}

	fapi_set_u16(req, u.mlme_start_req.channel_frequency, (2 * fw_freq));
	fapi_set_u16(req, u.mlme_start_req.channel_information, chan_info);

	/* Addition of SSID IE in mlme_start_req for hiddenSSID case */
	if (settings->hide_ssid != 0) {
		p = fapi_append_data(req, NULL, 2 + settings->ssid_len);
		if (!p) {
			slsi_kfree_mbuf(req);
			return -EINVAL;
		}
		*p++ = WLAN_EID_SSID;
		*p++ = settings->ssid_len;
		memcpy(p, settings->ssid, settings->ssid_len);
	}

	if (beacon_ie_head_len && settings->hide_ssid == 0) {
		fapi_append_data(req, mgmt->u.beacon.variable, beacon_ie_head_len);
	} else if (beacon_ie_head_len && settings->hide_ssid == 1) {
		fapi_append_data(req, mgmt->u.beacon.variable + 2, beacon_ie_head_len - 2);
	} else if (beacon_ie_head_len && settings->hide_ssid == 2) {
		fapi_append_data(req, mgmt->u.beacon.variable + 2 + settings->ssid_len, beacon_ie_head_len - (2 + settings->ssid_len));
	}

	if (settings->tail_len) {
		slsi_mlme_start_prepare_ies(req, dev, ndev_vif, settings, wpa_ie_pos, wmm_ie_pos);
	}

#ifdef CONFIG_SCSC_ADV_FEATURE
	if ((append_vht_ies) && !slsi_prepare_vht_ies(dev, &vht_ie_capab, &vht_ie_operation)) {
		fapi_append_data(req, vht_ie_capab, SLSI_VHT_CAPABILITIES_IE_LEN);
		fapi_append_data(req, vht_ie_operation, SLSI_VHT_OPERATION_IE_LEN);
		kmm_free(vht_ie_capab);
		kmm_free(vht_ie_operation);
	}
#endif

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_START_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_start_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_start_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_start_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

static void slsi_mlme_connect_rsse_copy(struct slsi_mlme_rsse *rsse, u8 *buf_ptr, int buf_ptr_len)
{
	u8 *rsse_buf_pos;
	u8 rsse_header[] = { 0xDD, 0x00,	/* VENDOR_SPECIFIC, LEN */
						 0x00, 0x16, 0x32,		/* OUI */
						 0x05 /* OUT Type: rsse */
					   };

	if (WARN_ON(buf_ptr == NULL || buf_ptr_len == 0)) {
		return;
	}

	rsse_header[1] = buf_ptr_len - 2;

	rsse_buf_pos = buf_ptr;
	rsse_buf_pos += sizeof(rsse_header);

	*rsse_buf_pos++ = rsse->group_cs_count;
	if (rsse->group_cs_count) {
		memcpy(rsse_buf_pos, rsse->group_cs, rsse->group_cs_count * 4);
		rsse_buf_pos += rsse->group_cs_count * 4;
	}

	*rsse_buf_pos++ = rsse->pairwise_cs_count;
	if (rsse->pairwise_cs_count) {
		memcpy(rsse_buf_pos, rsse->pairwise_cs, rsse->pairwise_cs_count * 4);
		rsse_buf_pos += rsse->pairwise_cs_count * 4;
	}

	*rsse_buf_pos++ = rsse->akm_suite_count;
	if (rsse->akm_suite_count) {
		memcpy(rsse_buf_pos, rsse->akm_suite, rsse->akm_suite_count * 4);
		rsse_buf_pos += rsse->akm_suite_count * 4;
	}

	*rsse_buf_pos++ = rsse->pmkid_count;
	if (rsse->pmkid_count) {
		memcpy(rsse_buf_pos, rsse->pmkid, rsse->pmkid_count * 16);
		rsse_buf_pos += rsse->pmkid_count * 16;
	}

	memcpy(buf_ptr, rsse_header, sizeof(rsse_header));
	if (rsse->group_mgmt_cs) {
		memcpy(rsse_buf_pos, rsse->group_mgmt_cs, 4);
	} else {
		memset(rsse_buf_pos, 0, 4);
	}
}

#ifdef CONFIG_SCSC_ADV_FEATURE
static int slsi_mlme_connect_wapi_rsse_prep(struct wpa_driver_associate_params *sme, struct slsi_mlme_rsse *rsse)
{
	const u8 *ptr = NULL;
	int ptr_len = 0;
	int index = 0;

//  const u8 *wapi_ie;

	memset(rsse, 0, sizeof(struct slsi_mlme_rsse));

	/* For WAPI IE , refer 7.3.2.9 WAPI information element in WAPI Independent IS Sample Draft Document Number: N13774 */

	wapi_ie = slsi_80211_find_ie(WLAN_EID_WAPI, sme->ie, sme->ie_len);
	if (wapi_ie) {
		u16 version = wapi_ie[3] << 8 | wapi_ie[2];

		if (version == 1) {
			ptr_len = wapi_ie[1] - 2;
			ptr = &wapi_ie[4];	/* points to start group cipher */
		}
	}
	if (ptr == NULL) {
		SLSI_ERR_NODEV("crypto.wpa_versions(%d) but no WAPI ie\n", sme->crypto.wpa_versions);
		return -EINVAL;
	}
	if (rsse != NULL) {
		rsse->akm_suite_count = ptr[index];
	} else {
		SLSI_ERR_NODEV("crypto.wpa_versions(%d) , rsse is NULL \n", sme->wpa_proto);
		return -EPERM;
	}

	index += 2;
	if (rsse->akm_suite_count) {
		rsse->akm_suite = &ptr[index];
		index += rsse->akm_suite_count * 4;
	}
	rsse->pairwise_cs_count = ptr[index];	/*Unicast Cipher Suite Count */
	index += 2;
	rsse->pairwise_cs = &ptr[index];	/*Unicast Cipher Suite */
	index += rsse->pairwise_cs_count * 4;

	rsse->group_cs_count = 1;
	rsse->group_cs = &ptr[index];	/*Multicast Cipher Suite */
	index += 4;

	index += 2;					/*WAPI Capablities */

	if (ptr_len > index) {
		rsse->pmkid_count = ptr[index];	/*BKID count */
		index += 2;
		if (rsse->pmkid_count) {
			rsse->pmkid = &ptr[index];
		}
	}

	/* group_mgmt_cs is not relevant in WAPI case */

	/* 6-> header, 4-> group_cs_count,pairwise_cs_count,akm_suite_count,pmkid_count
	 * last 4 -> group_mgmt_cs.
	 */
	return 6 + 4 + (rsse->group_cs_count + rsse->pairwise_cs_count + rsse->akm_suite_count) * 4 + rsse->pmkid_count * 16 + 4;
}
#endif

static int slsi_mlme_connect_rsse_prep(struct wpa_driver_associate_params *sme, struct slsi_mlme_rsse *rsse, u8 *mfp)
{
	const u8 *ptr = NULL;
	int ptr_len = 0;
	int index = 0;
	const u8 *ie;
	u8 version_index, oui_len = 0;
	u16 version;

	memset(rsse, 0, sizeof(struct slsi_mlme_rsse));

	/* For rsn indices refer "Figure 8-187RSNE format" in IEEE spec */
	if (sme->wpa_proto & WPA_PROTO_RSN) {
		ie = slsi_80211_find_ie(WLAN_EID_RSN, sme->wpa_ie, sme->wpa_ie_len);
		version_index = 2;
	} else {					/*(sme->crypto.wpa_versions == 1 */
		ie = slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPA, sme->wpa_ie, sme->wpa_ie_len);
		oui_len = 4;			/*3-> OUI,1- type */
		version_index = 6;
	}

	if (ie == NULL) {
		SLSI_ERR_NODEV("crypto.wpa_versions(%d) but no rsn/wpa ie\n", sme->wpa_proto);
		return -EINVAL;
	}

	version = ie[version_index + 1] << 8 | ie[version_index];
	if (version != 1) {
		SLSI_ERR_NODEV("Unexpected version (%d) in rsn/wpa ie\n", version);
		return -EINVAL;
	}

	ptr = &ie[version_index + 2];	/*  skip version, point to start of group cipher */
	ptr_len = ie[1] - (2 + oui_len);	/*version len + oui len(for wpa) */

	/* ptr point to -> [ group(4), pairwise_count(2){x}, pairwise_cs(x*4),
	 * akm_suite_count(2){y}, akm_suite(y*4), rsn_cap(2), pmkid_count(2){z},
	 * pmkid(z*16), group_mgmt_cs(4)]
	 * for wpa pts ends at akm_suite.
	 * the count fields are 2 bytes but values will be less than 256. So ignoring
	 * the higher byte for count.
	 */

	/* Group Data Cipher suite */
	rsse->group_cs_count = 1;
	rsse->group_cs = ptr;
	index += 4;

	/* Pairwise Cipher suite count */
	rsse->pairwise_cs_count = ptr[index];
	index += 2;

	/* if any nonzero length field is absent, then none of the susequent fields are
	 * included
	 */
	if (rsse->pairwise_cs_count == 0) {
		goto exit;
	}

	rsse->pairwise_cs = &ptr[index];
	index += rsse->pairwise_cs_count * 4;

	if (index >= ptr_len) {
		goto exit;
	}

	/* AKM suite count */
	rsse->akm_suite_count = ptr[index];
	index += 2;

	/* if any nonzero length field is absent, then none of the susequent fields are
	 * included
	 */
	if (rsse->akm_suite_count == 0) {
		goto exit;
	}

	rsse->akm_suite = &ptr[index];
	index += rsse->akm_suite_count * 4;

	if (index >= ptr_len) {
		goto exit;
	}

	if (sme->wpa_proto & WPA_PROTO_RSN) {
		/* RSN capabilities */

		/*get the PMF info from rsn capabilities */
		*mfp = FAPI_PMFCONTROL_DISABLED;

		if (ptr[index] & (1 << 7)) {
			*mfp = FAPI_PMFCONTROL_OPTIONAL;
		}
		if (ptr[index] & (1 << 6)) {
			*mfp = FAPI_PMFCONTROL_MANDATORY;
		}

		index += 2;
		if (index >= ptr_len) {
			goto exit;
		}

		/* PMKID count (only valid for wpa2) */
		rsse->pmkid_count = ptr[index];
		index += 2;

		if (rsse->pmkid_count != 0) {
			rsse->pmkid = &ptr[index];
			index += rsse->pmkid_count * 16;
		}

		if (index >= ptr_len) {
			goto exit;
		}
	}

	rsse->group_mgmt_cs = &ptr[index];

exit:
	/* 6-> header, 4-> group_cs_count,pairwise_cs_count,akm_suite_count,pmkid_count ,4 -> group_mgmt_cs.     */
	return 6 + 4 + (rsse->group_cs_count + rsse->pairwise_cs_count + rsse->akm_suite_count) * 4 + rsse->pmkid_count * 16 + 4;
}

/* If is_copy is true copy the required IEs from connect_ie to ie_dest. else calculate the required ie length */
static int slsi_mlme_connect_info_elems_ie_prep(const u8 *connect_ie, const size_t connect_ie_len, bool is_copy, u8 *ie_dest, int ie_dest_len)
{
	const u8 *ie_pos = NULL;
	const u8 *wpa_ie_pos = NULL;
	int info_elem_length = 0;
	u16 curr_ie_len;
	u16 wpa_ie_len;

	if (is_copy && (ie_dest == NULL || ie_dest_len == 0)) {
		return -EINVAL;
	}

	/* find interworking ie id:107 */
	ie_pos = slsi_80211_find_ie(SLSI_WLAN_EID_INTERWORKING, connect_ie, connect_ie_len);
	if (ie_pos != NULL) {
		curr_ie_len = *(ie_pos + 1) + 2;
		if (is_copy) {
			if (ie_dest_len >= curr_ie_len) {
				memcpy(ie_dest, ie_pos, curr_ie_len);
				ie_dest += curr_ie_len;
				ie_dest_len -= curr_ie_len;	/* free space available in ie_dest for next ie */
			} else {
				SLSI_ERR_NODEV("interwork ie extract error (ie_copy_l:%d, c_ie_l:%d):\n", ie_dest_len, curr_ie_len);
				return -EINVAL;
			}
		} else {
			info_elem_length = curr_ie_len;
		}
	}

	/* vendor specific IEs will be the last elements. */
	ie_pos = slsi_80211_find_ie(WLAN_EID_VENDOR_SPECIFIC, connect_ie, connect_ie_len);
	if (ie_pos != NULL) {
		curr_ie_len = connect_ie_len - (ie_pos - connect_ie);	/* length of all the vendor specific IEs */
		/* WPA ie will be the first vendor specific ie since wpa_supplicant adds it as first element */
		wpa_ie_pos = slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPA, connect_ie, connect_ie_len);
		if (wpa_ie_pos) {
			wpa_ie_len = wpa_ie_pos[1] + 2;	/* length of WPA vendor specific IE */
			curr_ie_len -= wpa_ie_len;
			ie_pos += wpa_ie_len;
		}
		if (is_copy) {
			if (ie_dest_len >= curr_ie_len) {
				memcpy(ie_dest, ie_pos, curr_ie_len);
				ie_dest += curr_ie_len;
				ie_dest_len -= curr_ie_len;
			} else {
				SLSI_ERR_NODEV("vendor ie extract error (ie_copy_l:%d, c_ie_l:%d):\n", ie_dest_len, curr_ie_len);
				return -EINVAL;
			}
		} else {
			info_elem_length += curr_ie_len;
		}
	}

	return info_elem_length;
}

static int slsi_mlme_connect_info_elements(struct slsi_dev *sdev, struct netif *dev, struct wpa_driver_associate_params *sme)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	int info_elem_length = 0;
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;
	u8 *p;

	info_elem_length = slsi_mlme_connect_info_elems_ie_prep(sme->wpa_ie, sme->wpa_ie_len, false, NULL, 0);

	/* NO IE required in MLME-ADD-INFO-ELEMENTS */
	if (info_elem_length <= 0) {
		return info_elem_length;
	}

	req = fapi_alloc(mlme_add_info_elements_req, MLME_ADD_INFO_ELEMENTS_REQ, ndev_vif->ifnum, info_elem_length);
	if (req == NULL) {
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_add_info_elements_req.purpose, FAPI_PURPOSE_ASSOCIATION_REQUEST);

	p = fapi_append_data(req, NULL, info_elem_length);
	if (p == NULL) {
		slsi_kfree_mbuf(req);
		return -EINVAL;
	}

	(void)slsi_mlme_connect_info_elems_ie_prep(sme->wpa_ie, sme->wpa_ie_len, true, p, info_elem_length);

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_add_info_elements_req(vif:%u)\n", ndev_vif->ifnum);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_ADD_INFO_ELEMENTS_CFM);
	if (cfm == NULL) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_add_info_elements_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_add_info_elements_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_connect_cfm.result_code));
		r = -EINVAL;
	}

	/*if (slsi_80211_find_vendor_ie(WLAN_OUI_MICROSOFT, WLAN_OUI_TYPE_MICROSOFT_WPS, sme->ie, sme->ie_len))
	 *      ndev_vif->sta.is_wps = true; */

	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_connect(struct slsi_dev *sdev, struct netif *dev, struct wpa_driver_associate_params *sme)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;
	u8 *p;
	enum slsi_80211_auth_type auth_type;
	struct slsi_mlme_rsse rsse;
	int rsse_len = 0;
	u8 mfp = FAPI_PMFCONTROL_DISABLED;
	u8 mac_addr[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	struct slsi_key_params slsi_key;

	memset(&slsi_key, 0, sizeof(slsi_key));

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_CONNECT.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (WARN_ON(!sme->ssid_len)) {
		return -EINVAL;
	}

	switch (sme->auth_alg) {
	case (WPA_AUTH_ALG_OPEN | WPA_AUTH_ALG_SHARED):	/* SLSI_80211_AUTHTYPE_AUTOMATIC */
		/* In case of WEP, need to try both open and shared.
		 * FW does this if auth is shared_key. So set it to shared.
		 */
		/* Supplicant maintains the auth_type as bitmap incase of WEP we need to
		 * try both open and shared and hence auth_type is set to 3, i.e OPEN and SHARED*/
		if ((sme->group_suite == WPA_CIPHER_WEP40 || sme->group_suite == WPA_CIPHER_WEP104)) {
			auth_type = SLSI_80211_AUTHTYPE_SHARED_KEY;
		} else {
			auth_type = SLSI_80211_AUTHTYPE_OPEN_SYSTEM;
		}
		break;
	case WPA_AUTH_ALG_OPEN:
		auth_type = SLSI_80211_AUTHTYPE_OPEN_SYSTEM;
		break;
	case WPA_AUTH_ALG_SHARED:
		auth_type = SLSI_80211_AUTHTYPE_SHARED_KEY;
		break;
	default:
		SLSI_NET_ERR(dev, "Unsupported auth_alg: %d\n", sme->auth_alg);
		return -EOPNOTSUPP;
	}

	/* We save the WEP key for shared authentication. */
	if ((auth_type == SLSI_80211_AUTHTYPE_SHARED_KEY) && ((sme->group_suite == WPA_CIPHER_WEP40) || (sme->group_suite == WPA_CIPHER_WEP104)) && (ndev_vif->vif_type == FAPI_VIFTYPE_STATION)) {
		SLSI_NET_DBG3(dev, SLSI_MLME, "key len (%d)\n", sme->wep_key_len[sme->wep_tx_keyidx]);
		slsi_key.key = (u8 *)sme->wep_key[sme->wep_tx_keyidx];
		if (slsi_key.key == NULL) {
			return -EINVAL;
		}
		slsi_key.key_len = sme->wep_key_len[sme->wep_tx_keyidx];
		slsi_key.seq_len = 0;
		if (sme->group_suite == WPA_CIPHER_WEP40) {
			slsi_key.cipher = SLSI_WLAN_CIPHER_SUITE_WEP40;
		} else {
			slsi_key.cipher = SLSI_WLAN_CIPHER_SUITE_WEP104;
		}

		r = slsi_mlme_set_key(sdev, dev, sme->wep_tx_keyidx, FAPI_KEYTYPE_DEFAULT, mac_addr, &slsi_key);
		if (r != 0) {
			SLSI_NET_ERR(dev, "Error Setting Shared key (%d)", r);
			return r;
		}
	}

	/*Do not check sme->ie as wpa_supplicant sends some invalid value in it even if ie_len is zero . */
	if (sme->wpa_ie_len) {
		r = slsi_mlme_connect_info_elements(sdev, dev, sme);
		if (r) {
			return r;
		}

		/* supplicant specific values: version 1 => wpa, version 2 => wpa2 */
		if (sme->wpa_proto & WPA_PROTO_WPA || sme->wpa_proto & WPA_PROTO_RSN) {
			rsse_len = slsi_mlme_connect_rsse_prep(sme, &rsse, &mfp);
		}
		/*else if ((sme->wpa_proto &WPA_PROTO_WAPI) && (sme->wpa_ie[0] == WLAN_EID_WAPI))
		   rsse_len = slsi_mlme_connect_wapi_rsse_prep(sme, &rsse); */
		if (rsse_len < 0) {
			SLSI_NET_ERR(dev, "ERROR preparing RSSE\n");
			return rsse_len;
		}
	}

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_connect_req(vif:%u, bssid:" SLSI_MAC_FORMAT ", ssid:%.*s)\n", ndev_vif->ifnum, SLSI_MAC_STR(sme->bssid), (int)sme->ssid_len, sme->ssid);
	req = fapi_alloc(mlme_connect_req, MLME_CONNECT_REQ, ndev_vif->ifnum, 2 + sme->ssid_len +	/*SSID IE */
					 rsse_len /*rsse */);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_memcpy(req, u.mlme_connect_req.bssid, sme->bssid);
	fapi_set_u16(req, u.mlme_connect_req.authentication_type, auth_type);
	fapi_set_u16(req, u.mlme_connect_req.channel_frequency, SLSI_FREQ_HOST_TO_FW(sme->freq_hint));
	fapi_set_u16(req, u.mlme_connect_req.pmf_control, mfp);

	p = fapi_append_data(req, NULL, 2 + sme->ssid_len + rsse_len);
	if (!p) {
		slsi_kfree_mbuf(req);
		return -EINVAL;
	}
	*p++ = WLAN_EID_SSID;
	*p++ = sme->ssid_len;
	memcpy(p, sme->ssid, sme->ssid_len);
	p += sme->ssid_len;

	if (rsse_len) {
		slsi_mlme_connect_rsse_copy(&rsse, p, rsse_len);
	}

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_CONNECT_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_connect_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_connect_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_connect_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

void slsi_mlme_connect_resp(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_CONNECT_RESP\n");
		return;
	}

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_connect_resp(vif:%u)\n", ndev_vif->ifnum);
	req = fapi_alloc(mlme_connect_res, MLME_CONNECT_RES, ndev_vif->ifnum, 0);
	if (!req) {
		return;
	}

	cfm = slsi_mlme_req_no_cfm(sdev, dev, req);
	WARN_ON(cfm);
	netif_set_link_up(dev);
}

void slsi_mlme_connected_resp(struct slsi_dev *sdev, struct netif *dev, u16 peer_index)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_CONNECT_RESP\n");
		return;
	}

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_connected_resp(vif:%u, peer_index:%d)\n", ndev_vif->ifnum, peer_index);
	req = fapi_alloc(mlme_connected_res, MLME_CONNECTED_RES, ndev_vif->ifnum, 0);
	if (!req) {
		SLSI_NET_ERR(dev, "mlme-connected-response :: memory allocation failed\n");
		return;
	}

	fapi_set_u16(req, u.mlme_connected_res.peer_index, peer_index);
	slsi_mlme_req_no_cfm(sdev, dev, req);
}

/* Null check for cfm done in caller function */
bool slsi_disconnect_cfm_validate(struct slsi_dev *sdev, struct netif *dev, struct max_buff *cfm)
{
	int result = fapi_get_u16(cfm, u.mlme_disconnect_cfm.result_code);
	bool r = false;

	SLSI_UNUSED_PARAMETER(sdev);

	if (WARN_ON(!dev)) {
		goto exit;
	}

	if (result == FAPI_RESULTCODE_SUCCESS) {
		r = true;
	}
	/* Not present code would mean peer is already disconnected and hence no ind (could be race scenario), don't log as error */
	else if (result != FAPI_RESULTCODE_NOT_PRESENT) {
		SLSI_NET_ERR(dev, "mlme_disconnect_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_disconnect_cfm.result_code));
	}

exit:
	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_disconnect(struct slsi_dev *sdev, struct netif *dev, u8 *mac, u16 reason_code, bool wait_ind)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *rx;
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_DISCONNECT.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_disconnect_req(vif:%u, bssid:" SLSI_MAC_FORMAT ", reason:%d)\n", ndev_vif->ifnum, SLSI_MAC_STR(mac), reason_code);

	/* No data reference required */
	req = fapi_alloc(mlme_disconnect_req, MLME_DISCONNECT_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return -ENOMEM;
	}
	fapi_set_u16(req, u.mlme_disconnect_req.reason_code, reason_code);
	if (mac) {
		fapi_set_memcpy(req, u.mlme_disconnect_req.peer_sta_address, mac);
	} else {
		fapi_set_memset(req, u.mlme_disconnect_req.peer_sta_address, 0);
	}
	if (wait_ind) {
		rx = slsi_mlme_req_cfm_ind(sdev, dev, req, MLME_DISCONNECT_CFM, MLME_DISCONNECT_IND, slsi_disconnect_cfm_validate);
		if (!rx) {
			SLSI_NET_ERR(dev, "mlme_disconnect_cfm() ERROR\n");
			r = -EINVAL;
		}
	} else {
		rx = slsi_mlme_req_cfm(sdev, dev, req, MLME_DISCONNECT_CFM);
		if (rx) {
			if (fapi_get_u16(rx, u.mlme_disconnect_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
				SLSI_NET_ERR(dev, "mlme_disconnect_cfm(result:%u) ERROR\n", fapi_get_u16(rx, u.mlme_disconnect_cfm.result_code));
				r = -EINVAL;
			}
		} else {
			r = -EIO;
		}
	}

	slsi_kfree_mbuf(rx);
	return r;
}

int slsi_mlme_set_key(struct slsi_dev *sdev, struct netif *dev, u16 key_id, u16 key_type, const u8 *address, struct slsi_key_params *key)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	if (slsi_is_test_mode_enabled()) {
		SLSI_NET_INFO(dev, "Skip sending signal, WlanLite FW does not support MLME_SETKEYS.request\n");
		return -EOPNOTSUPP;
	}

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_setkeys_req(key_id:%d, key_type:%d, address:" SLSI_MAC_FORMAT ", length:%d, cipher:0x%.8X)\n", key_id, key_type, SLSI_MAC_STR(address), key->key_len, key->cipher);
	req = fapi_alloc(mlme_setkeys_req, MLME_SETKEYS_REQ, ndev_vif->ifnum, key->key_len + 1);	/* + 1 for the wep key index */
	if (!req) {
		return -ENOMEM;
	}
	fapi_set_u16(req, u.mlme_setkeys_req.length, key->key_len * 8);
	fapi_set_u16(req, u.mlme_setkeys_req.key_id, key_id);
	fapi_set_u16(req, u.mlme_setkeys_req.key_type, key_type);
	fapi_set_memcpy(req, u.mlme_setkeys_req.address, address);
	fapi_set_memset(req, u.mlme_setkeys_req.sequence_number, 0x00);

	if (key->seq_len && key->seq) {
		int i;
		u16 temp_seq;

		SLSI_NET_DBG3(dev, SLSI_MLME, "mlme_setkeys_req(key->seq_len:%d)\n", key->seq_len);

		/* Sequence would be in little endian format (sent as part of get_key_sequence).
		 * If sequence is say 0x01 0x02 0x03 0x04 with 0x01 as MSB and 0x04 as LSB then
		 * it would be encoded as: 0x0304 0x0102 for firmware
		 */
		for (i = 0; i < key->seq_len; i += 2) {
			temp_seq = (u16)(key->seq[i + 1] << 8) | (u16)(key->seq[i]);
			fapi_set_u16(req, u.mlme_setkeys_req.sequence_number[i], temp_seq);
		}
	}

	fapi_set_u32(req, u.mlme_setkeys_req.cipher_suite_selector, key->cipher);

	if (key->cipher == SLSI_WLAN_CIPHER_SUITE_WEP40 || key->cipher == SLSI_WLAN_CIPHER_SUITE_WEP104) {
		u8 wep_key_id = (u8)key_id;

		if (key_id > 3) {
			SLSI_NET_WARN(dev, "Key ID is greater than 3");
		}
		fapi_append_data(req, &wep_key_id, 1);
	}
	fapi_append_data(req, key->key, key->key_len);

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SETKEYS_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_setkeys_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_setkeys_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_setkeys_cfm.result_code));
		r = -EINVAL;
	}
	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_get_key(struct slsi_dev *sdev, struct netif *dev, u16 key_id, u16 key_type, u8 *seq, int *seq_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_get_key_sequence_req(key_id:%d, key_type:%d)\n", key_id, key_type);
	req = fapi_alloc(mlme_get_key_sequence_req, MLME_GET_KEY_SEQUENCE_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return -ENOMEM;
	}
	fapi_set_u16(req, u.mlme_get_key_sequence_req.key_id, key_id);
	fapi_set_u16(req, u.mlme_get_key_sequence_req.key_type, key_type);

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_GET_KEY_SEQUENCE_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_get_key_sequence_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_get_key_sequence_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_get_key_sequence_cfm.result_code));
		r = -ENOENT;
	} else {
		int i;
		u16 temp_seq;

		/* For WPA2 Key RSC - 8 octets. For WPAI, it would be 16 octets (code would need to be updated)
		 * Length is not available in cfm but even if max length 8 is assigned, it should be ok as other octets
		 * would be padded with 0s
		 */
		*seq_len = 8;

		/* Sequence from firmware is of a[8] type u16 (16 octets) and only 8 octets are required for WPA/WPA2.
		 * If sequence is say 0x01 0x02 0x03 0x04 with 0x01 as MSB and 0x04 as LSB then
		 * it would be encoded as: 0x0304 0x0102 by firmware.
		 * Sequence is expected to be returned in little endian
		 */

		for (i = 0; i < *seq_len / 2; i++) {
			temp_seq = fapi_get_u16(cfm, u.mlme_get_key_sequence_cfm.sequence_number[i]);
			*seq = (u8)(temp_seq & 0xFF);
			*(seq + 1) = (u8)((temp_seq >> 8) & 0xFF);

			seq += 2;
		}
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

static int slsi_mlme_fw_tx_rate_calc(u16 fw_rate)
{
	const int fw_rate_idx_to_80211_rate[] = { 0, 10, 20, 55, 60, 90, 110, 120, 180, 240, 360, 480, 540 };
	int chan_bw_idx;
	int gi_idx;
	int mcs_idx;

	int channel_txrate = 0;

	if ((fw_rate & SLSI_FW_API_RATE_HT_SELECTOR_FIELD) == SLSI_FW_API_RATE_NON_HT_SELECTED) {
		u16 fw_rate_idx = fw_rate & SLSI_FW_API_RATE_INDEX_FIELD;

		if (fw_rate > 0 && fw_rate_idx < sizeof(fw_rate_idx_to_80211_rate) / sizeof(fw_rate_idx_to_80211_rate[0])) {
			channel_txrate = fw_rate_idx_to_80211_rate[fw_rate_idx];
		}
	} else if ((fw_rate & SLSI_FW_API_RATE_HT_SELECTOR_FIELD) == SLSI_FW_API_RATE_HT_SELECTED) {
		mcs_idx = SLSI_FW_API_RATE_MCS_FIELD & fw_rate;
		chan_bw_idx = (fw_rate & SLSI_FW_API_RATE_BW_FIELD) >> 9;
		gi_idx = ((fw_rate & SLSI_FW_API_RATE_SGI) == SLSI_FW_API_RATE_SGI) ? 1 : 0;
		if ((chan_bw_idx <= 2) && (mcs_idx <= 7)) {
			channel_txrate = slsi_mlme_ht_and_vht_rates_table[chan_bw_idx][gi_idx][mcs_idx];
		} else {
			SLSI_WARN_NODEV(" FW DATA RATE decode error fw_rate:%x, bw:%x, mcs_idx:%x\n", fw_rate, chan_bw_idx, mcs_idx);
		}
	} else if ((fw_rate & SLSI_FW_API_RATE_HT_SELECTOR_FIELD) == SLSI_FW_API_RATE_VHT_SELECTED) {
		/* report vht rate in legacy units and not as mcs index. reason: upper layers may still be not
		 * updated with vht msc table.
		 */
		chan_bw_idx = (fw_rate & SLSI_FW_API_RATE_BW_FIELD) >> 9;
		gi_idx = ((fw_rate & SLSI_FW_API_RATE_SGI) == SLSI_FW_API_RATE_SGI) ? 1 : 0;
		/* ignore Nss. Condor supports only one stream */
		mcs_idx = SLSI_FW_API_RATE_MCS_FIELD & fw_rate;
		if (WARN_ON((chan_bw_idx > 3) || (mcs_idx > 9))) {
			SLSI_WARN_NODEV(" FW DATA RATE decode error fw_rate:%x, bw:%x, mcs_idx:%x\n", fw_rate, chan_bw_idx, mcs_idx);
		} else {
			if ((chan_bw_idx <= 2) && (mcs_idx <= 9)) {
				channel_txrate = slsi_mlme_ht_and_vht_rates_table[chan_bw_idx][gi_idx][mcs_idx];
			}
		}
	}

	return channel_txrate;
}

int slsi_mlme_get_sinfo_mib(struct slsi_dev *sdev, struct netif *dev, struct wpa_signal_info *si)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	struct slsi_mib_value *values = NULL;
	int data_length = 0;
	int r = 0;
	static const struct slsi_mib_get_entry getValues[] = {
		{ SLSI_PSID_UNIFI_TX_DATA_RATE, { 0, 0 } },	/* to get STATION_INFO_TX_BITRATE */
		{ SLSI_PSID_UNIFI_RSSI, { 0, 0 } },			/* to get STATION_INFO_SIGNAL_AVG */
	};

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));
	r = slsi_mib_encode_get_list(&mibreq, (sizeof(getValues) / sizeof(struct slsi_mib_get_entry)), getValues);
	if (r != SLSI_MIB_STATUS_SUCCESS) {
		return -ENOMEM;
	}

	/* Fixed fields len (5) : 2 bytes(PSID) + 2 bytes (Len) + 1 byte (VLDATA header )  [10 for 2 PSIDs]
	 * Data : 3 bytes for SLSI_PSID_UNIFI_TX_DATA_RATE , 1 byte for SLSI_PSID_UNIFI_RSSI
	 */
	mibrsp.dataLength = 14;
	mibrsp.data = kmm_malloc(mibrsp.dataLength);

	if (mibrsp.data == NULL) {
		SLSI_NET_DBG1(dev, SLSI_MLME, "failed to allocate memory\n");
		kmm_free(mibreq.data);
		return -ENOMEM;
	}

	r = slsi_mlme_get(sdev, dev, mibreq.data, mibreq.dataLength, mibrsp.data, mibrsp.dataLength, &data_length);
	kmm_free(mibreq.data);

	if (r == 0) {
		mibrsp.dataLength = (u32)data_length;
		values = slsi_mib_decode_get_list(&mibrsp, (sizeof(getValues) / sizeof(struct slsi_mib_get_entry)), getValues);
		if (values == NULL) {
			SLSI_NET_DBG1(dev, SLSI_MLME, "mib decode list failed\n");
			kmm_free(mibrsp.data);
			return -ENOMEM;
		}
		if (values[0].type != SLSI_MIB_TYPE_NONE) {
			WARN_ON(values[0].type != SLSI_MIB_TYPE_UINT);
			//supplicant actually doing -  si.current_txrate / 1000, to compensate the same we are multiplying with 1000
			si->current_txrate = slsi_mlme_fw_tx_rate_calc((u16)values[0].u.uintValue) * 1000;
			SLSI_DBG3(sdev, SLSI_MLME, "SLSI_PSID_UNIFI_TX_DATA_RATE = %d\n", values[0].u.uintValue);
		}

		if (values[1].type != SLSI_MIB_TYPE_NONE) {
			WARN_ON(values[1].type != SLSI_MIB_TYPE_INT);
			si->current_signal = (s8)values[1].u.intValue;
			SLSI_DBG3(sdev, SLSI_MLME, "SLSI_PSID_UNIFI_RSSI = %d\n", values[1].u.intValue);
		}
	} else {
		SLSI_NET_DBG1(dev, SLSI_MLME, "mlme_get_req failed(result:%u)\n", r);
	}

	kmm_free(mibrsp.data);
	kmm_free(values);
	return r;
}

#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
/**
 * The slsi_mlme_powermgt_unlocked() must be called from a context that is synchronised
 * with ndev_vif. if called without the ndev_vif mutex already taken, other mechanisms
 * must ensure that ndev_vif will exist for the duration of the function.
 */
int slsi_mlme_powermgt_unlocked(struct slsi_dev *sdev, struct netif *dev, u16 ps_mode)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *rx;
	int r = 0;

	SLSI_MUTEX_LOCK(ndev_vif->powermgt_lock);

	if (WARN_ON(!ndev_vif->activated)) {
		SLSI_MUTEX_UNLOCK(ndev_vif->powermgt_lock);
		return -EINVAL;
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_powermgt_req(vif:%d, current mode:%d, new mode: %d)\n", ndev_vif->ifnum, ndev_vif->power_mode, ps_mode);

	if (ndev_vif->power_mode == ps_mode) {
		SLSI_MUTEX_UNLOCK(ndev_vif->powermgt_lock);
		SLSI_NET_DBG3(dev, SLSI_MLME, "power management mode is same as requested. No changes done\n");
		return 0;
	}

	req = fapi_alloc(mlme_powermgt_req, MLME_POWERMGT_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		SLSI_MUTEX_UNLOCK(ndev_vif->powermgt_lock);
		return -ENOMEM;
	}
	fapi_set_u16(req, u.mlme_powermgt_req.power_management_mode, ps_mode);

	rx = slsi_mlme_req_cfm(sdev, dev, req, MLME_POWERMGT_CFM);
	if (!rx) {
		SLSI_MUTEX_UNLOCK(ndev_vif->powermgt_lock);
		return -EIO;
	}

	if (fapi_get_u16(rx, u.mlme_powermgt_cfm.result_code) == FAPI_RESULTCODE_SUCCESS) {
		ndev_vif->power_mode = ps_mode;
	} else {
		SLSI_NET_ERR(dev, "mlme_powermgt_cfm(result:%u) ERROR\n", fapi_get_u16(rx, u.mlme_powermgt_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(rx);
	SLSI_MUTEX_UNLOCK(ndev_vif->powermgt_lock);
	return r;
}

int slsi_mlme_powermgt(struct slsi_dev *sdev, struct netif *dev, u16 ps_mode)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	return slsi_mlme_powermgt_unlocked(sdev, dev, ps_mode);
}
#endif

int slsi_mlme_register_action_frame(struct slsi_dev *sdev, struct netif *dev, u32 af_bitmap_active, u32 af_bitmap_suspended)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	req = fapi_alloc(mlme_register_action_frame_req, MLME_REGISTER_ACTION_FRAME_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_u32(req, u.mlme_register_action_frame_req.action_frame_category_bitmap_active, af_bitmap_active);
	fapi_set_u32(req, u.mlme_register_action_frame_req.action_frame_category_bitmap_suspended, af_bitmap_suspended);

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_register_action_frame(vif:%d, active:%d, suspended:%d)\n", ndev_vif->ifnum, af_bitmap_active, af_bitmap_suspended);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_REGISTER_ACTION_FRAME_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_register_action_frame_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_register_action_frame_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_register_action_frame_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_channel_switch(struct slsi_dev *sdev, struct netif *dev, u16 center_freq, u16 chan_info)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_channel_switch_req(vif:%d, freq: %d, channel info: 0x%x)\n", ndev_vif->ifnum, center_freq, chan_info);
	req = fapi_alloc(mlme_channel_switch_req, MLME_CHANNEL_SWITCH_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_channel_switch_req.channel_frequency, SLSI_FREQ_HOST_TO_FW(center_freq));
	fapi_set_u16(req, u.mlme_channel_switch_req.channel_information, chan_info);

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_CHANNEL_SWITCH_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_channel_switch_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_channel_switch_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_channel_switch_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_add_info_elements(struct slsi_dev *sdev, struct netif *dev, u16 purpose, const u8 *ies, const u16 ies_len)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;
	u8 *p;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	req = fapi_alloc(mlme_add_info_elements_req, MLME_ADD_INFO_ELEMENTS_REQ, ndev_vif->ifnum, ies_len);
	if (!req) {
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_add_info_elements_req.purpose, purpose);

	if (ies_len != 0) {
		p = fapi_append_data(req, ies, ies_len);
		if (!p) {
			slsi_kfree_mbuf(req);
			return -EINVAL;
		}
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_add_info_elements_req(vif:%d, ies_len:%d)\n", ndev_vif->ifnum, ies_len);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_ADD_INFO_ELEMENTS_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_add_info_elements_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_add_info_elements_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_add_info_elements_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_send_frame_data(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 msg_type)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	u16 host_tag = slsi_tx_host_tag(sdev);
	u16 len = mbuf->len;
	int ret;

#ifdef CONFIG_SCSC_ENABLE_PORT_CONTROL
	struct ethhdr *ehdr;
	struct slsi_peer *peer;
	struct scsc_wifi_fcq_data_qset *qs = NULL;
	int proto = 0;

	ehdr = eth_hdr(mbuf);
	proto = ntohs(ehdr->h_proto);

	/* Port control check */
	if (proto != ETH_P_PAE) {
		if ((ndev_vif->vif_type == FAPI_VIFTYPE_AP) && is_multicast_ether_addr(ehdr->h_dest)) {
			qs = &ndev_vif->ap.group_data_qs;
		} else {
			SLSI_MUTEX_LOCK(ndev_vif->peer_lock);
			peer = slsi_get_peer_from_mac(sdev, dev, ehdr->h_dest);
			SLSI_MUTEX_UNLOCK(ndev_vif->peer_lock);
			if (!peer) {
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_no_peer_record);
				return ERR_CONN;
			}
			qs = &peer->data_qs;
		}

		if (slsi_is_port_blocked(dev, qs)) {
			SLSI_NET_DBG1(dev, SLSI_TX, "Packet dropped, port is blocked\n");
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_port_blocked);
			return ERR_CONN;
		}
	}
#endif

	len = mbuf->len;
	(void)mbuf_push(mbuf, (fapi_sig_size(mlme_send_frame_req)));

	/* fill the signal header */
	fapi_set_u16(mbuf, id, MLME_SEND_FRAME_REQ);
	fapi_set_u16(mbuf, receiver_pid, 0);
	fapi_set_u16(mbuf, sender_pid, SLSI_TX_PROCESS_ID_MIN);
	fapi_set_u16(mbuf, fw_reference, 0);

	/* fill in signal parameters */
	fapi_set_u16(mbuf, u.mlme_send_frame_req.vif, ndev_vif->ifnum);
	fapi_set_u16(mbuf, u.mlme_send_frame_req.host_tag, host_tag);
	fapi_set_u16(mbuf, u.mlme_send_frame_req.data_unit_descriptor, FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME);
	fapi_set_u16(mbuf, u.mlme_send_frame_req.message_type, msg_type);
	fapi_set_u16(mbuf, u.mlme_send_frame_req.channel_frequency, 0);
	fapi_set_u32(mbuf, u.mlme_send_frame_req.dwell_time, 0);
	fapi_set_u32(mbuf, u.mlme_send_frame_req.period, 0);

	SLSI_DBG2(sdev, SLSI_MLME, "mlme_send_frame_req(vif:%d, message_type:%d, host_tag:%d)\n", ndev_vif->ifnum, msg_type, host_tag);

	ret = slsi_tx_control(sdev, dev, mbuf, false);
	if (ret != 0) {
		SLSI_WARN(sdev, "failed to send MLME signal(err=%d)\n", ret);
		/* Data path mbuf is re-used and hence shouldn't be freed here */
		goto exit;
	}

	/* as the frame is queued to HIP for transmission, store the host tag of the frames
	 * to validate the transmission status in MLME-Frame-Transmission.indication.
	 * Take necessary action based on the type of frame and status of it's transmission
	 */
	if (msg_type == FAPI_MESSAGETYPE_EAPOL_KEY_M4) {
		ndev_vif->sta.m4_host_tag = host_tag;
		SLSI_NET_DBG1(dev, SLSI_MLME, "EAPOL-Key M4 frame (host_tag:%d)\n", ndev_vif->sta.m4_host_tag);
	} else if (msg_type == FAPI_MESSAGETYPE_EAP_MESSAGE) {
		if (!ndev_vif->sta.is_wps && (ndev_vif->iftype == SLSI_80211_IFTYPE_STATION)) {
			/* In case of non-P2P station and Enterprise security store the host_tag.
			 * If transmission of such frame fails, inform supplicant to disconnect.
			 */
			ndev_vif->sta.eap_hosttag = host_tag;
			SLSI_NET_DBG1(dev, SLSI_MLME, "EAP frame (host_tag:%d)\n", ndev_vif->sta.eap_hosttag);
		}
	}

exit:
	return ret;
}

int slsi_mlme_send_frame_mgmt(struct slsi_dev *sdev, struct netif *dev, const u8 *frame, int frame_len, u16 data_desc, u16 msg_type, u16 host_tag, u16 freq, u32 dwell_time, u32 period)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;
	u8 *p;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	req = fapi_alloc(mlme_send_frame_req, MLME_SEND_FRAME_REQ, ndev_vif->ifnum, frame_len);
	if (!req) {
		SLSI_WARN(sdev, "failed to alloc memory\n");
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_send_frame_req.host_tag, host_tag);
	fapi_set_u16(req, u.mlme_send_frame_req.data_unit_descriptor, data_desc);
	fapi_set_u16(req, u.mlme_send_frame_req.message_type, msg_type);
	fapi_set_u16(req, u.mlme_send_frame_req.channel_frequency, freq);
	fapi_set_u32(req, u.mlme_send_frame_req.dwell_time, dwell_time);
	fapi_set_u32(req, u.mlme_send_frame_req.period, period);

	p = fapi_append_data(req, frame, frame_len);
	if (!p) {
		slsi_kfree_mbuf(req);
		SLSI_WARN(sdev, "failed to append data\n");
		return -EINVAL;
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_send_frame_req(vif:%d, host_tag:%d)\n", ndev_vif->ifnum, host_tag);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SEND_FRAME_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_send_frame_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_send_frame_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_send_frame_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

#ifdef CONFIG_SCSC_ENABLE_P2P
int slsi_mlme_reset_dwell_time(struct slsi_dev *sdev, struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_reset_dwell_time_req (vif:%d)\n", ndev_vif->ifnum);

	req = fapi_alloc(mlme_reset_dwell_time_req, MLME_RESET_DWELL_TIME_REQ, ndev_vif->ifnum, 0);

	if (!req) {
		return -ENOMEM;
	}

	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_RESET_DWELL_TIME_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_reset_dwell_time_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_reset_dwell_time_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_reset_dwell_time_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}
#endif

int slsi_mlme_set_packet_filter(struct slsi_dev *sdev, struct netif *dev, int pkt_filter_len, u8 num_filters, struct slsi_mlme_pkt_filter_elem *pkt_filter_elems)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0, i = 0, j = 0;
	u8 *p;
	u8 index = 0;

	if (WARN_ON(!ndev_vif->activated)) {
		return -EINVAL;
	}

	if (WARN_ON(!num_filters)) {
		return -EINVAL;
	}

	req = fapi_alloc(mlme_set_packet_filter_req, MLME_SET_PACKET_FILTER_REQ, ndev_vif->ifnum, pkt_filter_len);
	if (!req) {
		return -ENOMEM;
	}

	p = fapi_append_data(req, NULL, pkt_filter_len);
	if (!p) {
		slsi_kfree_mbuf(req);
		return -EINVAL;
	}

	for (i = 0; i < num_filters; i++) {
		struct slsi_mlme_pkt_filter_elem pkt_filter_elem = pkt_filter_elems[i];

		memcpy(&p[index], pkt_filter_elem.header, SLSI_PKT_FILTER_ELEM_HDR_LEN);
		index += SLSI_PKT_FILTER_ELEM_HDR_LEN;

		for (j = 0; j < pkt_filter_elem.num_pattern_desc; j++) {
			p[index++] = pkt_filter_elem.pattern_desc[j].offset;
			p[index++] = pkt_filter_elem.pattern_desc[j].mask_length;
			memcpy(&p[index], pkt_filter_elem.pattern_desc[j].mask, pkt_filter_elem.pattern_desc[j].mask_length);
			index += pkt_filter_elem.pattern_desc[j].mask_length;
			memcpy(&p[index], pkt_filter_elem.pattern_desc[j].pattern, pkt_filter_elem.pattern_desc[j].mask_length);
			index += pkt_filter_elem.pattern_desc[j].mask_length;
		}
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_set_packet_filter_req(vif:%d, num_filters:%d)\n", ndev_vif->ifnum, num_filters);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_PACKET_FILTER_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_packet_filter_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_packet_filter_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_set_packet_filter_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}

int slsi_mlme_set_pmk(struct slsi_dev *sdev, struct netif *dev, const u8 *pmk, u16 pmklen)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (WARN_ON(!ndev_vif->activated)) {
		return -EINVAL;
	}
	if (pmk) {
		req = fapi_alloc(mlme_set_pmk_req, MLME_SET_PMK_REQ, ndev_vif->ifnum, pmklen);
	} else {
		req = fapi_alloc(mlme_set_pmk_req, MLME_SET_PMK_REQ, ndev_vif->ifnum, 0);
	}

	if (!req) {
		return -ENOMEM;
	}
	if (pmk) {
		fapi_append_data(req, pmk, pmklen);
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_set_pmk_req(vif:%u, pmklen:%d)\n", ndev_vif->ifnum, pmklen);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_PMK_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_pmk_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_pmk_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_set_pmk_cfm.result_code));
		r = -EINVAL;
	}
	slsi_kfree_mbuf(cfm);
	return r;
}

#ifdef CONFIG_SCSC_ADV_FEATURE
int slsi_mlme_set_acl(struct slsi_dev *sdev, struct netif *dev, const struct cfg80211_acl_data *params)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	size_t mac_acl_size = 0;
	int i, r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	mac_acl_size = sizeof((params->mac_addrs[0])) * (params->n_acl_entries);
	req = fapi_alloc(mlme_set_acl_req, MLME_SET_ACL_REQ, ndev_vif->ifnum, mac_acl_size);
	if (!req) {
		SLSI_NET_ERR(dev, "fapi alloc failure\n");
		return -ENOMEM;
	}
	fapi_set_u16(req, u.mlme_set_acl_req.entries, params->n_acl_entries);
	fapi_set_u16(req, u.mlme_set_acl_req.acl_policy, params->acl_policy);

	for (i = 0; i < params->n_acl_entries; i++) {
		fapi_append_data(req, params->mac_addrs[i].addr, sizeof((params->mac_addrs[i])));
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_set_acl_req(vif:%u, n_acl_entries:%d, mac_addrs[0]:%pM)\n", ndev_vif->ifnum, params->n_acl_entries, params->mac_addrs[0].addr);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_SET_ACL_CFM);
	if (!cfm) {
		return -EIO;
	}

	if (fapi_get_u16(cfm, u.mlme_set_acl_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_set_acl_cfm(result:%u) ERROR\n", fapi_get_u16(cfm, u.mlme_set_acl_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}
#endif							/* CONFIG_SCSC_ADV_FEATURE */

int slsi_mlme_set_ext_capab(struct slsi_dev *sdev, struct netif *dev, struct slsi_mib_value *mib_val)
{
	struct slsi_mib_data mib_data = { 0, NULL };
	int error = 0;

	error = slsi_mib_encode_octet(&mib_data, SLSI_PSID_UNIFI_EXTENDED_CAPABILITIES, mib_val->u.octetValue.dataLength, mib_val->u.octetValue.data, 0);
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

	if (!error) {
		return 0;
	}

exit:
	SLSI_ERR(sdev, "Error in setting ext capab. error = %d\n", error);
	return error;
}

int slsi_mlme_set_iw_ext_cap(struct slsi_dev *sdev, struct netif *dev, const u8 *ies, int ie_len)
{
	struct slsi_mib_entry mib_entry;
	struct slsi_mib_data mibreq = { 0, NULL };
	struct slsi_mib_data mibrsp = { 0, NULL };
	const u8 *ext_capab_ie;
	int r = 0;
	int rxLength = 0;
	int len = 0;

	slsi_mib_encode_get(&mibreq, SLSI_PSID_UNIFI_EXTENDED_CAPABILITIES, 0);

	/*  5 (header) + 9 (data)  + 2 (mlme expects 16 (??)) */
	mibrsp.dataLength = 16;
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
		len = slsi_mib_decode(&mibrsp, &mib_entry);
		if (len == 0) {
			SLSI_ERR(sdev, "Mib decode error\n");
			r = -EINVAL;
			goto exit;
		}
	} else {
		SLSI_ERR(sdev, "Mib read failed (error: %d)\n", r);
		goto exit;
	}

	ext_capab_ie = slsi_80211_find_ie(SLSI_WLAN_EID_EXT_CAPABILITY, ies, ie_len);

	/*interworking bit is bit 31 ,ie length must be >= 4 */
	if ((ext_capab_ie) && (ext_capab_ie[1] >= 4) && (ext_capab_ie[5] & 0x80)) {
		if ((mib_entry.value.u.octetValue.data[3] & 0x80) == 0) {
			mib_entry.value.u.octetValue.data[3] |= 0x80;
			r = slsi_mlme_set_ext_capab(sdev, dev, &mib_entry.value);
		}
	} else if ((mib_entry.value.u.octetValue.data[3] & 0x80)) {
		/*reset iw bit in ext capabilities if iw is not enabled */
		mib_entry.value.u.octetValue.data[3] &= 0x7F;
		r = slsi_mlme_set_ext_capab(sdev, dev, &mib_entry.value);
	}

exit:
	kmm_free(mibrsp.data);
	return r;
}

int slsi_mlme_blockack_control_req(struct slsi_dev *sdev, struct netif *dev, u16 blockack_control_bitmap, u16 direction, const u8 *peer_sta_address)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *req;
	struct max_buff *cfm;
	int r = 0;

	WARN_ON(!SLSI_MUTEX_IS_LOCKED(ndev_vif->vif_mutex));

	if (!peer_sta_address) {
		SLSI_NET_WARN(dev, "INVALID PEER ADDRESS");
		return -EINVAL;
	}

	req = fapi_alloc(mlme_blockack_control_req, MLME_BLOCKACK_CONTROL_REQ, ndev_vif->ifnum, 0);
	if (!req) {
		SLSI_NET_ERR(dev, "memory allocation failed for blockack control request\n");
		return -ENOMEM;
	}

	fapi_set_u16(req, u.mlme_blockack_control_req.blockack_control_bitmap, blockack_control_bitmap);
	fapi_set_u16(req, u.mlme_blockack_control_req.direction, direction);
	fapi_set_memcpy(req, u.mlme_blockack_control_req.peer_sta_address, peer_sta_address);

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_blockack_control_req(vif:%u, direction:%d, blockack_control_bitmap:%d)\n", ndev_vif->ifnum, direction, blockack_control_bitmap);
	cfm = slsi_mlme_req_cfm(sdev, dev, req, MLME_BLOCKACK_CONTROL_CFM);
	if (!cfm) {
		return -EIO;
	}

	SLSI_NET_DBG2(dev, SLSI_MLME, "mlme_blockack_control_cfm: peer=" SLSI_MAC_FORMAT " up=%X dir=%X\n", SLSI_MAC_STR(peer_sta_address), blockack_control_bitmap, direction);

	if (fapi_get_u16(cfm, u.mlme_blockack_control_cfm.result_code) != FAPI_RESULTCODE_SUCCESS) {
		SLSI_NET_ERR(dev, "mlme_blockack_control_cfm (result: %u) ERROR\n", fapi_get_u16(cfm, u.mlme_blockack_control_cfm.result_code));
		r = -EINVAL;
	}

	slsi_kfree_mbuf(cfm);
	return r;
}
