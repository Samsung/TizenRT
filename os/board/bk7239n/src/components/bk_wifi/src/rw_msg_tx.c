// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <common/bk_kernel_err.h>
#include "bk_list.h"
#include "fhost_msg.h"
#include "rw_msg_tx.h"
#include "rw_ieee80211.h"
#include "bk_rw.h"
#include "common.h"
//#include "mac_frame.h"
#include "bk_wifi_types.h"
#include "signal.h"
#include "rw_ieee80211.h"
#include "rwnx_params.h"
#include "rwnx_defs.h"
#include "bk_wifi.h"
#include "rwnx_misc.h"

#include "rwnx_err.h"
#include "rwnx_defs.h"
#include "bk_wifi_private.h"
//#include "reg_riu.h"
//#include "bk_wifi_prop_private.h"
#include "rwm_proto.h"
#include "modules/pm.h"
#include "components/ate.h"

#if (CONFIG_FULLY_HOSTED || CONFIG_SEMI_HOSTED)
#ifndef PP_HTONS
#define PP_HTONS(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))
#endif
#ifndef ENOMEM
#define ENOMEM 12
#endif
#endif

__INLINE int phy_freq_to_channel(uint8_t band, uint16_t freq)
{
	if ((band == PHY_BAND_2G4) && (freq >= 2412) && (freq <= 2484)) {
		if (freq == 2484)
			return 14;
		else
			return (freq - 2407) / 5;
	} else if ((band == PHY_BAND_5G) && (freq >= 5005) && (freq <= 5825))
		return (freq - PHY_FREQ_5G) / 5;

	return 0;
}

extern int bmsg_ioctl_sender(void *arg);
extern int bmsg_software_para_ioctl_sender(void *arg);
extern int bmsg_hardware_para_ioctl_sender(void *arg);
void phy_get_channel(struct phy_channel_info *info, uint8_t index);

int rw_msg_send(const void *msg_params, int reqcfm, uint16_t reqid, void *cfm)
{
	int ret;
	struct ke_msg *msg = ke_param2msg(msg_params);
	MSG_SND_NODE_PTR tx_msg = 0;
	int err = BK_OK;

	GLOBAL_INT_DECLARATION();

	// Need wait cfm from full mac
	if (reqcfm) {
		tx_msg = os_malloc(sizeof(MSG_SND_NODE_ST));
		if (!tx_msg)
			return BK_ERR_NO_MEM;

		tx_msg->msg = msg;
		tx_msg->reqid = reqid;
		tx_msg->cfm = cfm;

		ret = rtos_init_semaphore(&tx_msg->semaphore, 1);
		BK_ASSERT(0 == ret); /* ASSERT VERIFIED */

		GLOBAL_INT_DISABLE();
		co_list_push_back(&rw_msg_tx_head, &tx_msg->hdr);
		GLOBAL_INT_RESTORE();

	}

	///wakeup mac but do not config pwr bit
	///in the future, if the msg doesn't need to wakeup mac, can use bmsg_software_para_ioctl_sender()
	if ((ME_TRAFFIC_IND_REQ == msg->id) ||
		(ME_GET_BSS_INFO_REQ == msg->id) ||
		(MM_SET_LISTEN_INTERVAL_REQ == msg->id) ||
		(MM_SET_BCN_LOSS_INT_REQ == msg->id) ||
		(MM_SET_PSDEBUG_INTERVAL_REQ == msg->id) ||
		(ME_STA_ARP_UPDATE_IND == msg->id) ||
		(MM_SET_ARP_TX_RATE_REQ == msg->id)) {
		ret = bmsg_hardware_para_ioctl_sender((void *)msg_params);
	} else {
		ret = bmsg_ioctl_sender((void *)msg_params);
	}
	if (kNoErr != ret) {
		RWNX_LOGI("%s failed send %d\n", __FUNCTION__, msg->id);
		ke_msg_free(msg);
		err = RWNX_ERR_IOCTL_SEND;
		goto failed_or_timeout;
	} else if (reqcfm) {
		ret = rtos_get_semaphore(&tx_msg->semaphore, 5 * MICROSECONDS);
		if (0 != ret) {
			RWNX_LOGI("%s timeout for %d\n", __FUNCTION__, reqid);
			BK_ASSERT(0); /* ASSERT VERIFIED */
			GLOBAL_INT_DISABLE();
			co_list_extract(&rw_msg_tx_head, &tx_msg->hdr);
			GLOBAL_INT_RESTORE();
			err = RWNX_ERR_TIMEOUT;

			goto failed_or_timeout;
		}

		ret = rtos_deinit_semaphore(&tx_msg->semaphore);
		BK_ASSERT(0 == ret); /* ASSERT VERIFIED */

		os_free(tx_msg);
	}

	return BK_OK;

failed_or_timeout:
	if (reqcfm) {
		ret = rtos_deinit_semaphore(&tx_msg->semaphore);
		BK_ASSERT(0 == ret); /* ASSERT VERIFIED */
		os_free(tx_msg);
	}

	return err;
}

int rw_msg_send_reset(void)
{
	void *void_param;

	/* RESET REQ has no parameter */
	void_param = ke_msg_alloc(MM_RESET_REQ, TASK_MM, TASK_API, 0);
	if (!void_param)
		return -1;

	return rw_msg_send(void_param, 1, MM_RESET_CFM, NULL);
}

int rw_msg_send_start(void)
{
	struct mm_start_req *start_req_param;

	/* Build the START REQ message */
	start_req_param = ke_msg_alloc(MM_START_REQ, TASK_MM, TASK_API,
								   sizeof(struct mm_start_req));
	if (!start_req_param)
		return -1;

	/* Set parameters for the START message */
	start_req_param->phy_cfg.parameters[0] = 1;
	start_req_param->phy_cfg.parameters[1] = 0;
	start_req_param->uapsd_timeout = 300;
	start_req_param->rx_hostbuf_size = 8192;
#if CONFIG_PM_V2
	if (bk_pm_lpo_src_get() == PM_LPO_SRC_DIVD)
		start_req_param->lp_clk_accuracy = 20;
	else if (bk_pm_lpo_src_get() == PM_LPO_SRC_X32K)
		start_req_param->lp_clk_accuracy = 100;
	else
		start_req_param->lp_clk_accuracy = 1000;

#else
	start_req_param->lp_clk_accuracy = 20;
#endif
	/* Send the START REQ message to LMAC FW */
	return rw_msg_send(start_req_param, 1, MM_START_CFM, NULL);
}

int rw_msg_send_me_config_req(void)
{
	struct me_config_req *req;

#if CONFIG_WIFI4
	struct wiphy *wiphy = &g_wiphy;
	struct ieee80211_sta_erp_cap *erp_cap = &wiphy->bands[IEEE80211_BAND_2GHZ]->erp_cap;
	struct ieee80211_sta_ht_cap *ht_cap = &wiphy->bands[IEEE80211_BAND_2GHZ]->ht_cap;
	struct ieee80211_sta_vht_cap *vht_cap = &wiphy->bands[IEEE80211_BAND_5GHZ]->vht_cap;
	uint8_t *ht_mcs = (uint8_t *)&ht_cap->mcs;
	int i;
#endif // CONFIG_WIFI4
	struct ieee80211_sta_he_cap const *he_cap;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

	/* Build the ME_CONFIG_REQ message */
	req = ke_msg_alloc(ME_CONFIG_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_config_req));
	if (!req)
		return -1;

	/* Set parameters for the ME_CONFIG_REQ message */
#if CONFIG_WIFI4
	req->erp_supp = erp_cap->erp_supported;
	req->ht_supp = ht_cap->ht_supported;
	req->vht_supp = vht_cap->vht_supported;
	req->ht_cap.ht_capa_info = cpu_to_le16(ht_cap->cap);
	req->ht_cap.a_mpdu_param = ht_cap->ampdu_factor |
							   (ht_cap->ampdu_density <<
								IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);

	for (i = 0; i < sizeof(ht_cap->mcs); i++)
		req->ht_cap.mcs_rate[i] = ht_mcs[i];

	req->ht_cap.ht_extended_capa = 0;
	req->ht_cap.tx_beamforming_capa = 0;  //0x64000000 ? invalid bit[31:29] is reserved, bit[26:25]=0b10, 3 rows of CSI
	req->ht_cap.asel_capa = 0;  // Antenna Selection Capable: Not Supported

	req->vht_cap.vht_capa_info = cpu_to_le32(vht_cap->cap);
	req->vht_cap.rx_highest = cpu_to_le16(vht_cap->vht_mcs.rx_highest);
	req->vht_cap.rx_mcs_map = cpu_to_le16(vht_cap->vht_mcs.rx_mcs_map);
	req->vht_cap.tx_highest = cpu_to_le16(vht_cap->vht_mcs.tx_highest);
	req->vht_cap.tx_mcs_map = cpu_to_le16(vht_cap->vht_mcs.tx_mcs_map);
#endif // CONFIG_WIFI4

	if (wiphy->bands[IEEE80211_BAND_2GHZ]->iftype_data != NULL) {
		he_cap = &wiphy->bands[IEEE80211_BAND_2GHZ]->iftype_data->he_cap;

		req->he_supp = he_cap->has_he;
		for (i = 0; i < ARRAY_SIZE(he_cap->he_cap_elem.mac_cap_info); i++)
			req->he_cap.mac_cap_info[i] = he_cap->he_cap_elem.mac_cap_info[i];
		for (i = 0; i < ARRAY_SIZE(he_cap->he_cap_elem.phy_cap_info); i++)
			req->he_cap.phy_cap_info[i] = he_cap->he_cap_elem.phy_cap_info[i];
		req->he_cap.mcs_supp.rx_mcs_80 = cpu_to_le16(he_cap->he_mcs_nss_supp.rx_mcs_80);
		req->he_cap.mcs_supp.tx_mcs_80 = cpu_to_le16(he_cap->he_mcs_nss_supp.tx_mcs_80);
		req->he_cap.mcs_supp.rx_mcs_160 = cpu_to_le16(he_cap->he_mcs_nss_supp.rx_mcs_160);
		req->he_cap.mcs_supp.tx_mcs_160 = cpu_to_le16(he_cap->he_mcs_nss_supp.tx_mcs_160);
		req->he_cap.mcs_supp.rx_mcs_80p80 = cpu_to_le16(he_cap->he_mcs_nss_supp.rx_mcs_80p80);
		req->he_cap.mcs_supp.tx_mcs_80p80 = cpu_to_le16(he_cap->he_mcs_nss_supp.tx_mcs_80p80);
		for (i = 0; i < MAC_HE_PPE_THRES_MAX_LEN; i++)
			req->he_cap.ppe_thres[i] = he_cap->ppe_thres[i];
		req->he_ul_on = rwnx_hw->mod_params->he_ul_on;
	} else {
		req->he_supp = false;
		req->he_ul_on = false;
	}

	req->ps_on = rwnx_hw->mod_params->ps_on;		// TODO: BK7236 fix ps
	req->dpsm = rwnx_hw->mod_params->dpsm;			// TODO: BK7236 fix ps
	req->tx_lft = rwnx_hw->mod_params->tx_lft;
	req->ant_div_on = rwnx_hw->mod_params->ant_div;
	if (rwnx_hw->mod_params->use_80)
		req->phy_bw_max = PHY_CHNL_BW_80;
	else if (rwnx_hw->mod_params->use_2040)
		req->phy_bw_max = PHY_CHNL_BW_40;
	else
		req->phy_bw_max = PHY_CHNL_BW_20;

	RWNX_LOGI("HT supp %d, VHT supp %d, HE supp %d, bw %dM\n", req->ht_supp, req->vht_supp, req->he_supp, 20 * (1 + req->phy_bw_max));

#if CONFIG_PM_V2
	req->ps_on = 1;
#endif

	/* Send the ME_CONFIG_REQ message to LMAC FW */
	return rw_msg_send(req, 1, ME_CONFIG_CFM, NULL);
}

int find_ieee80211_channel_flags(int channel, struct ieee80211_channel *channels, int num_channels)
{
	for (int i = 0; i < num_channels; i++) {
		if (channel == channels[i].hw_value)
			return channels[i].flags;
	}
	return 0;
}

int find_ieee80211_freq_flags(int freq, struct ieee80211_channel *channels, int num_channels)
{
	for (int i = 0; i < num_channels; i++) {
		if (freq == channels[i].center_freq)
			return channels[i].flags;
	}
	return 0;
}

uint16_t get_chan_flags(uint32_t flags)
{
    uint16_t chan_flags = 0;
    if (flags & IEEE80211_CHAN_NO_IR)
        chan_flags |= CHAN_NO_IR;
    if (flags & IEEE80211_CHAN_RADAR)
        chan_flags |= CHAN_RADAR;
    return chan_flags;
}

int rw_msg_send_me_chan_config_req(void)
{
	struct me_chan_config_req *req;
	struct wiphy *wiphy = &g_wiphy;
	int i;

	/* Build the ME_CHAN_CONFIG_REQ message */
	req = ke_msg_alloc(ME_CHAN_CONFIG_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_chan_config_req));
	if (!req)
		return -1;

	req->chan2G4_cnt =  0;
	if (wiphy->bands[IEEE80211_BAND_2GHZ]->n_channels) {
		struct ieee80211_supported_band *b = wiphy->bands[IEEE80211_BAND_2GHZ];
		for (i = 0; i < b->n_channels; i++) {
			req->chan2G4[req->chan2G4_cnt].flags = 0;

			if (b->channels[i].flags & IEEE80211_CHAN_DISABLED)
				req->chan2G4[req->chan2G4_cnt].flags |= CHAN_DISABLED;

			req->chan2G4[req->chan2G4_cnt].flags |= get_chan_flags(b->channels[i].flags);
			req->chan2G4[req->chan2G4_cnt].band = IEEE80211_BAND_2GHZ;
			req->chan2G4[req->chan2G4_cnt].freq = b->channels[i].center_freq;
			req->chan2G4[req->chan2G4_cnt].tx_power = VIF_UNDEF_POWER;
			req->chan2G4_cnt++;

			if (req->chan2G4_cnt == SCAN_CHANNEL_2G4)
				break;
		}
	}

	req->chan5G_cnt = 0;
#if CONFIG_WIFI_BAND_5G
#if CONFIG_WIFI_REGDOMAIN
	if (wiphy->bands[IEEE80211_BAND_5GHZ]->n_channels) {
		struct ieee80211_supported_band *b = wiphy->bands[IEEE80211_BAND_5GHZ];
		for (i = 0; i < b->n_channels; i++) {
			req->chan5G[req->chan5G_cnt].flags = 0;

			if (b->channels[i].flags & IEEE80211_CHAN_DISABLED && !ate_is_enabled())
				req->chan5G[req->chan5G_cnt].flags |= CHAN_DISABLED;

			req->chan5G[req->chan5G_cnt].flags |= get_chan_flags(b->channels[i].flags);
			req->chan5G[req->chan5G_cnt].band = IEEE80211_BAND_5GHZ;
			req->chan5G[req->chan5G_cnt].freq = b->channels[i].center_freq;
			req->chan5G_cnt++;
			if (req->chan5G_cnt == SCAN_CHANNEL_5G)
				break;
		}
	}
#else // CONFIG_WIFI_REGDOMAIN
	if (wiphy->bands[IEEE80211_BAND_5GHZ]->n_channels) {
		if (ate_is_enabled()) {
			struct ieee80211_supported_band *b = wiphy->bands[IEEE80211_BAND_5GHZ];
			for (i = 0; i < b->n_channels; i++) {
				req->chan5G[req->chan5G_cnt].flags = 0;

				if (b->channels[i].flags & IEEE80211_CHAN_DISABLED)
					req->chan5G[req->chan5G_cnt].flags |= CHAN_DISABLED;

				req->chan5G[req->chan5G_cnt].flags |= get_chan_flags(b->channels[i].flags);
				req->chan5G[req->chan5G_cnt].band = IEEE80211_BAND_5GHZ;
				req->chan5G[req->chan5G_cnt].freq = b->channels[i].center_freq;
				req->chan5G_cnt++;
				if (req->chan5G_cnt == SCAN_CHANNEL_5G)
					break;
			}
		} else {
			int selected_channels_size = 0;
			struct ieee80211_supported_band *b = wiphy->bands[IEEE80211_BAND_5GHZ];
			extern int* rw_select_5g_channels_by_regulatory_domain(int *selected_channels_size);
			int *selected_channels_5g = rw_select_5g_channels_by_regulatory_domain(&selected_channels_size);
			for (i = 0; i < selected_channels_size; i++) {
				req->chan5G[req->chan5G_cnt].flags = 0;

				int flag = find_ieee80211_channel_flags(selected_channels_5g[i], b->channels, b->n_channels);

				if (flag & IEEE80211_CHAN_DISABLED)
					req->chan5G[req->chan5G_cnt].flags |= CHAN_DISABLED;

				req->chan5G[req->chan5G_cnt].flags |= get_chan_flags(b->channels[i].flags);
				req->chan5G[req->chan5G_cnt].band = IEEE80211_BAND_5GHZ;
				req->chan5G[req->chan5G_cnt].freq = (5000 + (5 *selected_channels_5g[i]));
				req->chan5G[req->chan5G_cnt].tx_power = VIF_UNDEF_POWER;
				req->chan5G_cnt++;
				if (req->chan5G_cnt == SCAN_CHANNEL_5G)
					break;
			}
		}
	}
#endif // CONFIG_WIFI_REGDOMAIN
#endif
	/* Send the ME_CHAN_CONFIG_REQ message to LMAC FW */
	return rw_msg_send(req, 1, ME_CHAN_CONFIG_CFM, NULL);
}

int rw_msg_send_add_if(const unsigned char *mac,
					   enum nl80211_iftype iftype,
					   bool p2p,
					   struct mm_add_if_cfm *cfm)
{
	struct mm_add_if_req *add_if_req_param;

	/* Build the ADD_IF_REQ message */
	add_if_req_param = ke_msg_alloc(MM_ADD_IF_REQ, TASK_MM, TASK_API,
									sizeof(struct mm_add_if_req));
	if (!add_if_req_param)
		return BK_ERR_NO_MEM;

#if CONFIG_P2P
	p2p = false;
	add_if_req_param->p2p_rec_probe_req = true;
#endif
	/* Set parameters for the ADD_IF_REQ message */
	os_memcpy(&(add_if_req_param->addr.array[0]), mac, ETH_ALEN);
	switch (iftype) {
#if CONFIG_P2P
	case NL80211_IFTYPE_P2P_CLIENT:
		p2p = true;
		/* fall through */
#endif
	case NL80211_IFTYPE_STATION:
		add_if_req_param->type = VIF_STA;
		break;

	case NL80211_IFTYPE_ADHOC:
		add_if_req_param->type = VIF_IBSS;
		break;

#if CONFIG_P2P
	case NL80211_IFTYPE_P2P_GO:
		p2p = true;
		/* fall through */
#endif
	case NL80211_IFTYPE_AP:
		add_if_req_param->type = VIF_AP;
		break;

	case NL80211_IFTYPE_MESH_POINT:
		add_if_req_param->type = VIF_MESH_POINT;
		break;

	case NL80211_IFTYPE_MONITOR:
		add_if_req_param->type = VIF_MONITOR;
		break;

	default:
		add_if_req_param->type = VIF_STA;
		break;
	}

	add_if_req_param->p2p = p2p;

	if (mac_vif_mgmt_get_used_cnt() > 0)
	{
#if CONFIG_CUS_MAC_MASK
	    add_if_req_param->mac_addr_hi_mask = 0;
	    add_if_req_param->mac_addr_low_mask = 0x2;
#else
	    add_if_req_param->mac_addr_low_mask = 0;
	    add_if_req_param->mac_addr_hi_mask = ((NX_VIRT_DEV_MAX - 1) << 8);
#endif
	}

	/* Send the ADD_IF_REQ message to LMAC FW */
	return rw_msg_send(add_if_req_param, 1, MM_ADD_IF_CFM, cfm);
}

int rw_msg_send_remove_if(u8 vif_index)
{
	struct ke_msg cfm;
	struct mm_remove_if_req *remove_if_req;

	/* Build the MM_REMOVE_IF_REQ message */
	remove_if_req = ke_msg_alloc(MM_REMOVE_IF_REQ, TASK_MM, TASK_API,
								 sizeof(struct mm_remove_if_req));
	if (!remove_if_req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the MM_REMOVE_IF_REQ message */
	remove_if_req->inst_nbr = vif_index;

	/* Send the MM_REMOVE_IF_REQ message to LMAC FW */
	return rw_msg_send(remove_if_req, 1, MM_REMOVE_IF_CFM, &cfm);
}

int rw_msg_send_version_req(void)
{
	void *void_param;
	struct mm_version_cfm *cfm = &g_rwnx_hw.version_cfm;

	/* VERSION REQ has no parameter */
	void_param = ke_msg_alloc(MM_VERSION_REQ, TASK_MM, TASK_API, 0);
	if (!void_param)
		return BK_ERR_NO_MEM;

	return rw_msg_send(void_param, 1, MM_VERSION_CFM, cfm);
}

// TODO: BK7236, call this function in wpa_s instead
int rw_msg_send_config_monitor_req(struct mac_chan_op *chan, struct me_config_monitor_cfm *cfm)
{
	struct me_config_monitor_req *req;

	//FHOST_DBG(FHOST_FN_ENTRY_STR);

	/* Build the ME_CONFIG_MONITOR_REQ message */
	req = ke_msg_alloc(ME_CONFIG_MONITOR_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_config_monitor_req));
	if (!req)
		return -1;

	if (chan) {
		req->chan_set = true;
		req->chan = *chan;
	} else
		req->chan_set = false;

	req->uf = rwnx_mod_params.uf;

	/* Send the ME_CONFIG_MONITOR_REQ message to FW */
	return rw_msg_send(req, 1, ME_CONFIG_MONITOR_CFM, cfm);
}

// TODO: BK7236, call this function in wpa_s instead
int rwnx_monitor_open(void)
{
	int                     ret = 0;
	struct mm_add_if_cfm    add_if_cfm;
	uint8_t                 mac_addr[ETH_ALEN];
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

	if (rwnx_hw->monitor_vif != RWNX_INVALID_VIF)
		return 0;

	bk_wifi_sta_get_mac((uint8_t *)mac_addr);

	if ((ret = rw_msg_send_add_if(mac_addr, NL80211_IFTYPE_MONITOR, 0, &add_if_cfm)))
		return ret;

	if (add_if_cfm.status != 0) {
		//dbg(D_ERR D_FHOST "FHOST: add virtual iface error(status=%d)!\r\n", add_if_cfm.status);

		return -1;
	}

	rwnx_hw->monitor_vif = add_if_cfm.inst_nbr;

	return ret;
}

// TODO: BK7236, call this function in wpa_s instead
int rwnx_monitor_close(void)
{
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
	int ret;

	if (rwnx_hw->monitor_vif == RWNX_INVALID_VIF)
		return 0;

	ret = rw_msg_send_remove_if(rwnx_hw->monitor_vif);
	if (!ret)
		// successfully removed the monitor interface
		rwnx_hw->monitor_vif = RWNX_INVALID_VIF;

	return ret;
}

#if NX_TWT
#ifdef CONFIG_WPA_TWT_TEST
int rwnx_msg_send_twt_setup(uint8_t setup_type, uint16_t mantissa, uint8_t min_twt)
{
	int ret = 0;
	unsigned char vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);
	struct twt_conf_tag twt_conf;
	struct twt_setup_cfm *twt_setup_cfm
		= (struct twt_setup_cfm *)os_malloc(sizeof(struct twt_setup_cfm));
	if (NULL == twt_setup_cfm) {
		RWNX_LOGI("twt setup failed: oom\r\n");
		return BK_ERR_NO_MEM;
	}

	os_memset(&twt_conf, 0, sizeof(twt_conf));
	twt_conf.flow_type = 1;
	twt_conf.wake_int_exp = 10;
	twt_conf.wake_dur_unit = 0;
	twt_conf.wake_int_mantissa = mantissa;
	twt_conf.min_twt_wake_dur = min_twt;

	RWNX_LOGI("TWT: Setup request, setup_command: %d  mantissa: %d  min_twt: %d\n",
		  setup_type, twt_conf.wake_int_mantissa, twt_conf.min_twt_wake_dur);

	ret = rw_msg_send_twt_setup(setup_type, vif_idx, &twt_conf, twt_setup_cfm);
	if (!ret && (twt_setup_cfm->status == CO_OK))
		RWNX_LOGI("set up success\r\n");
	else
		RWNX_LOGI("set up fail, ret = %d\r\n", ret);
	os_free(twt_setup_cfm);
	return ret;

}

int rwnx_msg_send_twt_teardown(void)
{
	int i = 0;
	int ret = 0;
	unsigned char vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

	for (i = 0; i < NX_TWT_FLOW_NB; i++)
	{
	 	// Check if flow is valid
		if (twt_flow_tab[i].control & TWT_CTRL_VALID) {
			RWNX_LOGI("TWT: teardown, vif_idx: %d  id: %d\n", vif_idx, twt_flow_tab[i].id);
			ret = rw_msg_send_twt_teardown(vif_idx, twt_flow_tab[i].id);
		}
	}

	if (!ret)
		RWNX_LOGI( "tear down success\r\n");
	else
		RWNX_LOGI( "tear down fail, ret = %d\r\n", ret);
	return ret;
}
#endif


int rw_msg_send_twt_setup(uint8_t setup_type, uint8_t vif_idx, struct twt_conf_tag *twt_conf, struct twt_setup_cfm *cfm)
{
	struct twt_setup_req *req;

	req = ke_msg_alloc(TWT_SETUP_REQ, TASK_TWT, TASK_API,
					   sizeof(struct twt_setup_req));
	if (!req)
		return -1;

	req->vif_idx = vif_idx;
	req->setup_type = setup_type;
	os_memcpy(&req->conf, twt_conf, sizeof(*twt_conf));

	return rw_msg_send(req, 1, TWT_SETUP_CFM, cfm);
}

int rw_msg_send_twt_teardown(uint8_t vif_idx, uint8_t flow_id)
{
	struct twt_teardown_req *twt_teardown;
	struct twt_teardown_cfm twt_teardown_cfm;

	twt_teardown = ke_msg_alloc(TWT_TEARDOWN_REQ, TASK_TWT, TASK_API,
								sizeof(struct twt_teardown_req));

	twt_teardown->id = flow_id;
	twt_teardown->neg_type = 0; // XXX MAC_TWT_CTRL_NEGOT_INDIV;
	twt_teardown->all_twt = 0;
	twt_teardown->vif_idx = vif_idx;

	return rw_msg_send(twt_teardown, 1, TWT_TEARDOWN_CFM, &twt_teardown_cfm);
}
#endif

int rw_msg_get_machw_mib_req(uint32_t mib_bit, void *cfm)
{
	struct mm_get_machw_mib_req *req;
	/* Build the MM_GET_MACHW_MIB_REQ message */
	req = ke_msg_alloc(MM_GET_MACHW_MIB_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_get_machw_mib_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->mib_bit = mib_bit;

	/* Send the MM_GET_MACHW_MIB_CFM message to FW */
	return rw_msg_send(req, 1, MM_GET_MACHW_MIB_CFM, cfm);
}

int rw_send_me_rc_set_rate(u8 sta_idx, u16 rate_cfg)
{
	struct me_rc_set_rate_req *req;

	/* Build the ME_RC_SET_RATE_REQ message */
	req = ke_msg_alloc(ME_RC_SET_RATE_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_rc_set_rate_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters for the ME_RC_SET_RATE_REQ message */
	req->sta_idx = sta_idx;
	req->fixed_rate_cfg = rate_cfg;

	/* Send the ME_RC_SET_RATE_REQ message to FW */
	return rw_msg_send(req, 0, 0, NULL);
}

int rw_send_me_traffic_ind(u8 sta_idx, bool uapsd, u8 tx_status)
{
    struct me_traffic_ind_req *req;

    /* Build the ME_UTRAFFIC_IND_REQ message */
    req = ke_msg_alloc(ME_TRAFFIC_IND_REQ, TASK_ME, TASK_API,
                          sizeof(struct me_traffic_ind_req));
    if (!req)
        return BK_ERR_NO_MEM;

    /* Set parameters for the ME_TRAFFIC_IND_REQ message */
    req->sta_idx = sta_idx;
    req->tx_avail = tx_status;
    req->uapsd = uapsd;

    /* Send the ME_TRAFFIC_IND_REQ to UMAC FW */
    return rw_msg_send(req, 0, ME_TRAFFIC_IND_CFM, NULL);
}

int rw_send_apm_probe_req(uint8_t vif_index, uint8_t sta_idx,
			struct apm_probe_client_cfm *cfm)
{
	struct apm_probe_client_req *req;

	req = ke_msg_alloc(APM_PROBE_CLIENT_REQ, TASK_APM, TASK_API,
					   sizeof(struct apm_probe_client_req));
	if (!req)
		return -ENOMEM;

	req->vif_idx = vif_index;
	req->sta_idx = sta_idx;

	/* Send the APM_PROBE_CLIENT_REQ message to UMAC FW */
	return rw_msg_send(req, 1, APM_PROBE_CLIENT_CFM, cfm);
}

#if NX_VERSION < NX_VERSION_PACK(6, 22, 0, 0)
int rw_msg_set_app_filter(uint32_t filter)
{
	struct mm_set_filter_req *set_filter_req_param;
	//uint32_t rx_filter = 0;

	/* Build the MM_SET_FILTER_REQ message */
	set_filter_req_param =
		ke_msg_alloc(MM_SET_APP_FILTER_REQ, TASK_MM, TASK_API,
					 sizeof(struct mm_set_filter_req));
	if (!set_filter_req_param)
		return BK_ERR_NO_MEM;

	/* Now copy all the flags into the message parameter */
	set_filter_req_param->filter = filter;

	/* Send the MM_SET_FILTER_REQ message to LMAC FW */
	return rw_msg_send(set_filter_req_param, 1, MM_SET_APP_FILTER_CFM, NULL);
}
#endif

u8_t supp_multi_proto = 0;

void set_multicast_flag(void) {
	supp_multi_proto = 1;
}

u8_t get_multicast_flag(void) {
	return supp_multi_proto;
}

int rw_msg_dhcp_done_ind(u8 vif_idx)
{
	struct me_dhcp_done_ind *ind = ke_msg_alloc(ME_DHCP_DONE_IND, TASK_ME, TASK_API,
					sizeof(struct me_dhcp_done_ind));

	if (!ind)
		return BK_ERR_NO_MEM;

	ind->vif_idx = vif_idx;
	ind->multi_pro_flag = get_multicast_flag();

#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
	rwnx_dhcp_done_handler(vif_idx);  // FIXME: move to right place.
#endif

	return rw_msg_send(ind, 0, 0, NULL);
}

// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
int rw_msg_mfp_connect_deauth(u8 vif_idx,u8 *bssid,bool encrypt,
											u8 *pn,u16 seq,struct mac_sec_key *key, bool is_first_connect)
{
	struct me_mfp_connect_deauth_ind *ind = ke_msg_alloc(ME_MFP_CONNECT_DEAUTH_IND, TASK_ME, TASK_API,
					sizeof(struct me_mfp_connect_deauth_ind));

	if (!ind)
		return BK_ERR_NO_MEM;

	ind->vif_idx = vif_idx;
	ind->is_mfp_support = encrypt;
	ind->seq = seq;
	ind->is_first_connect = is_first_connect;

	os_memcpy(&ind->bssid,bssid,sizeof(ind->bssid));
	os_memcpy(ind->pn, pn, sizeof(ind->pn));
	os_memcpy(&ind->key, key, sizeof(ind->key));

	return rw_msg_send(ind, 0, 0, NULL);
}
// #endif

int rw_msg_send_apm_start_req(u8 vif_index, u8 channel,
							  struct apm_start_cfm *cfm)
{
	struct apm_start_req *req;

	/* Build the APM_START_REQ message */
	req = ke_msg_alloc(APM_START_REQ, TASK_APM, TASK_API,
					   sizeof(struct apm_start_req) + sizeof(beacon));
	if (!req)
		return -1;


	req->chan.band = 0;
	req->chan.flags = 0;
	req->chan.prim20_freq = req->chan.center1_freq = rw_ieee80211_get_centre_frequency(channel);
	req->chan.center2_freq = 0;

#define MAC_BASIC_RATE                  0x80
	// FIXME: BK7239 6E channel starts with channel 1
	if (channel <= 14) {
		req->chan.band = PHY_BAND_2G4;

		// 2.4G Basic rate
		req->basic_rates.length = 4;
		req->basic_rates.array[0] = 2 | MAC_BASIC_RATE; // 1M
		req->basic_rates.array[1] = 4 | MAC_BASIC_RATE; // 2M
		req->basic_rates.array[2] = 11 | MAC_BASIC_RATE; // 5.5M
		req->basic_rates.array[3] = 22 | MAC_BASIC_RATE; // 11M
	} else {
		// 5G basic rate
		req->basic_rates.length = 3;
		req->basic_rates.array[0] = 12 | MAC_BASIC_RATE;  // 6M
		req->basic_rates.array[1] = 24 | MAC_BASIC_RATE;  // 12M
		req->basic_rates.array[2] = 48 | MAC_BASIC_RATE;  // 24M
		req->chan.band = PHY_BAND_5G;
	}
#undef MAC_BASIC_RATE

	req->chan.type = PHY_CHNL_BW_20;

	req->bcn_addr = (UINT32)(req + 1);
	req->bcn_len = sizeof(beacon);
	os_memcpy(req + 1, beacon, sizeof(beacon));
	req->tim_oft = 56;
	req->tim_len = 6;
	req->bcn_int = BEACON_INTERVAL;

	if (g_ap_param_ptr->cipher_suite > BK_SECURITY_TYPE_WEP) {
		req->flags = USE_PAIRWISE_KEY;
		req->flags |= USE_PRIVACY;
		req->flags |= CONTROL_PORT_HOST;
	} else {
		req->flags = 0;
	}

#ifdef CONFIG_AP_PS
	req->flags |= VIF_AP_PS;
#endif

	req->ctrl_port_ethertype = PP_HTONS(ETH_P_PAE);
	req->vif_idx = vif_index;

	RWNX_LOGD("apm start with vif:%d\r\n", vif_index);

	return rw_msg_send(req, 1, APM_START_CFM, cfm);
}

int rw_msg_send_apm_stop_req(u8 vif_index)
{
	struct apm_stop_req *req;

	/* Build the APM_STOP_REQ message */
	req = ke_msg_alloc(APM_STOP_REQ, TASK_APM, TASK_API,
					   sizeof(struct apm_stop_req));
	if (!req)
		return -1;

	/* Set parameters for the APM_STOP_REQ message */
	req->vif_idx = vif_index;
	RWNX_LOGI("send apm stop req, vif=%d\n", vif_index);

	/* Send the APM_STOP_REQ message to LMAC FW */
	return rw_msg_send(req, 1, APM_STOP_CFM, NULL);
}

int rw_msg_send_bcn_change(void *bcn_param)
{
	struct mm_bcn_change_req *req;
	BCN_PARAM_PTR param = (BCN_PARAM_PTR)bcn_param;

	/* Build the MM_BCN_CHANGE_REQ message */
	req = ke_msg_alloc(MM_BCN_CHANGE_REQ, TASK_MM, TASK_API,
					   sizeof(struct mm_bcn_change_req) + param->bcn_len);
	if (!req)
		return -1;

	/* Set parameters for the MM_BCN_CHANGE_REQ message */
	req->bcn_ptr = (u32)(req + 1);
	req->bcn_len = param->bcn_len;
	os_memcpy(req + 1, param->bcn_ptr, req->bcn_len);
	req->tim_len = param->tim_len;
	req->tim_oft = param->tim_oft;
	req->inst_nbr = param->vif_idx;

	for (int i = 0; i < BCN_MAX_CSA_CPT; i++)
		req->csa_oft[i] = param->csa_oft[i];

#if CONFIG_AP_IDLE
	if (ap_ps_enable_get() &&
		!bk_wlan_has_role(VIF_STA) && bk_wlan_has_role(VIF_AP))
		start_global_ap_bcn_timer();
#endif

	/* Send the MM_BCN_CHANGE_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_BCN_CHANGE_CFM, NULL);
}


int rw_msg_send_me_sta_add(struct add_sta_st *param,
						   struct me_sta_add_cfm *cfm)
{
	struct me_sta_add_req *req;

	/* Build the MM_STA_ADD_REQ message */
	req = ke_msg_alloc(ME_STA_ADD_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_sta_add_req));
	if (!req)
		return BK_ERR_NO_MEM;

#if CONFIG_AP_IDLE
	ap_idle_stop();
#endif
	/* Set parameters for the MM_STA_ADD_REQ message */
	os_memcpy(&(req->mac_addr.array[0]), param->sta_addr, ETH_ALEN);

	req->vif_idx = param->ap_vif_idx;
	req->aid = param->aid;
	req->flags = param->flags; // 1:STA_QOS_CAPA 2: STA_HT_CAPA BIT(3)STA_MFP_CAPA

	RWNX_LOGI("hapd_intf_sta_add:%d, vif:%d\r\n", req->aid, req->vif_idx);
	if(param->supp_rates_len > MAC_RATESET_LEN)
		RWNX_LOGW("hapd_intf_sta_add: supp_rates_len:%d\r\n", param->supp_rates_len);
	req->rate_set.length = param->supp_rates_len <= MAC_RATESET_LEN ? param->supp_rates_len : MAC_RATESET_LEN;
	for(uint8_t i = 0; i < req->rate_set.length; i++)
		req->rate_set.array[i] = param->supp_rates[i];

	req->ht_cap.mcs_rate[0] = 255;

	/* Send the ME_STA_ADD_REQ message to LMAC FW */
	return rw_msg_send(req, 1, ME_STA_ADD_CFM, cfm);
}

int rw_msg_send_me_sta_del(u8 sta_idx, bool tdls_sta)
{
	struct me_sta_del_req *req;

	/* Build the MM_STA_DEL_REQ message */
	req = ke_msg_alloc(ME_STA_DEL_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_sta_del_req));
	if (!req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the MM_STA_DEL_REQ message */
	req->sta_idx = sta_idx;
	req->tdls_sta = tdls_sta;

	/* Send the ME_STA_DEL_REQ message to LMAC FW */
	return rw_msg_send(req, 1, ME_STA_DEL_CFM, NULL);
}

int rw_msg_me_set_control_port_req(bool opened, u8 sta_idx)
{
	struct me_set_control_port_req *req;

	/* Build the ME_SET_CONTROL_PORT_REQ message */
	req = ke_msg_alloc(ME_SET_CONTROL_PORT_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_set_control_port_req));
	if (!req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the ME_SET_CONTROL_PORT_REQ message */
	req->sta_idx = sta_idx;
	req->control_port_open = opened;

	/* Send the ME_SET_CONTROL_PORT_REQ message to LMAC FW */
	return rw_msg_send(req, 1, ME_SET_CONTROL_PORT_CFM, NULL);
}

int rw_msg_send_key_add(KEY_PARAM_T *param, struct mm_key_add_cfm *cfm)
{
	struct mm_key_add_req *key_add_req;

	/* Build the MM_KEY_ADD_REQ message */
	key_add_req = ke_msg_alloc(MM_KEY_ADD_REQ, TASK_MM, TASK_API,
							   sizeof(struct mm_key_add_req));
	if (!key_add_req) {
		RWNX_LOGI("rw_msg_send_key_add NULL\r\n");
		return BK_ERR_NO_MEM;
	}

	/* Set parameters for the MM_KEY_ADD_REQ message */
	if (param->sta_idx != 0xFF) {
		/* Pairwise key */
		key_add_req->sta_idx = param->sta_idx;
	} else {
		/* Default key */
		key_add_req->sta_idx = param->sta_idx;
		key_add_req->key_idx = param->key_idx; /* only useful for default keys */
	}

	key_add_req->spp = 0;
	key_add_req->pairwise = 0;
	key_add_req->inst_nbr = param->inst_nbr;
	key_add_req->key.length = param->key.length;
	os_memcpy(&(key_add_req->key.array[0]), &(param->key.array[0]), param->key.length);

	key_add_req->cipher_suite = param->cipher_suite;

	/* Send the MM_KEY_ADD_REQ message to LMAC FW */
	return rw_msg_send(key_add_req, 1, MM_KEY_ADD_CFM, cfm);
}

int rw_msg_send_key_del(u8 hw_key_idx)
{
	struct mm_key_del_req *key_del_req;

	/* Build the MM_KEY_DEL_REQ message */
	key_del_req = ke_msg_alloc(MM_KEY_DEL_REQ, TASK_MM, TASK_API,
							   sizeof(struct mm_key_del_req));
	if (!key_del_req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the MM_KEY_DEL_REQ message */
	key_del_req->hw_key_idx = hw_key_idx;

	/* Send the MM_KEY_DEL_REQ message to LMAC FW */
	return rw_msg_send(key_del_req, 1, MM_KEY_DEL_CFM, NULL);
}

#if defined(CONFIG_WIFI_SCAN_CH_TIME) && CONFIG_WIFI_SCAN_CH_TIME
struct scan_chan_duration_tag scancd_env = {0};

void set_scan_chan_duration_param(unsigned char chnb, struct scan_chan_duration_item *param)
{
	scancd_env.table_nm = chnb;
	scancd_env.tb = param;
}
#endif

struct scan_cfg_scan_param_tag scan_param_env = {0};
struct scan_cfg_scan_param_tag scan_param_dump_env = {0};


int rw_msg_send_scanu_req(SCAN_PARAM_T *scan_param)
{
	int i;
	struct scanu_start_req *req;
	uint8_t *extra_ies;
	#if CONFIG_WIFI_REGDOMAIN
	struct wiphy *wiphy = &g_wiphy;
	uint32_t flags;
	#else
	uint16_t scan_freq = 0;
	#endif // CONFIG_WIFI_REGDOMAIN
	int band = IEEE80211_BAND_2GHZ;
	int chan_cnt = 0;

	/* Build the SCANU_START_REQ message */
	req = ke_msg_alloc(SCANU_START_REQ, TASK_SCANU, TASK_API,
					   sizeof(struct scanu_start_req) + scan_param->extra_ies_len);
	if (!req) {
		RWNX_LOGE("scanu req alloc failed\r\n");
		return BK_ERR_NO_MEM;
	}

	extra_ies = (uint8_t *)(req + 1);

	/* Set parameters */
	req->vif_idx = scan_param->vif_idx;
	req->no_cck = 0;

	os_memcpy(&req->bssid, &scan_param->bssid, sizeof(req->bssid));
	req->ssid_cnt = scan_param->num_ssids;
	for (i = 0; i < req->ssid_cnt; i++) {
		req->ssid[i].length = scan_param->ssids[i].length;
		os_memcpy(req->ssid[i].array, scan_param->ssids[i].array, req->ssid[i].length);
	}

	int *freqs = scan_param->freqs;
	if (!freqs[0]) {
		/* no specified freq, set to all freqs supported */
		rw_ieee80211_init_scan_chan(req);
	} else {
		/* specified freqs: XXX 5g band */
		for (i = 0; i < ARRAY_SIZE(scan_param->freqs); i++, freqs++) {
			if (!*freqs)
				break;
			if (*freqs >= 5925) {
				band = IEEE80211_BAND_6GHZ;   /// FIXME: BK7239 6E
			} else if (*freqs >= 4900) {
				band = IEEE80211_BAND_5GHZ;
			} else {
				band = IEEE80211_BAND_2GHZ;
			}

			flags = find_ieee80211_freq_flags(*freqs, wiphy->bands[band]->channels,wiphy->bands[band]->n_channels);
			if((flags & IEEE80211_CHAN_DISABLED) && (!(ate_is_enabled() || rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid))))
			{
				RWNX_LOGW("rw_msg_send_scanu_req:freq %d,CHAN_DISABLED\r\n ",*freqs);
				continue;
			}

			req->chan[chan_cnt].freq = *freqs;
			req->chan[chan_cnt].band = band;
			req->chan[chan_cnt].flags = 0;
			req->chan[chan_cnt].tx_power = VIF_UNDEF_POWER;

			#if CONFIG_WIFI_REGDOMAIN
			if(!(ate_is_enabled() || rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid)))
			{
				flags = find_ieee80211_freq_flags(req->chan[chan_cnt].freq, wiphy->bands[req->chan[chan_cnt].band]->channels,
							wiphy->bands[req->chan[chan_cnt].band]->n_channels);
				req->chan[chan_cnt].flags = get_chan_flags(flags);
			}
			else
			{
				req->chan[chan_cnt].flags = 0;
				//os_printf("rw_msg_send_scanu_req:freq %d ,flags 0x%x\r\n ",req->chan[i].freq,req->chan[i].flags);
			}
			#else
			req->chan[chan_cnt].flags = 0;

			#if CONFIG_WIFI_AUTO_COUNTRY_CODE
			// If auto mode, disable 12/13/14,52/56/60/64a ctive scan
			if (country_code_policy_is_auto() &&
				((req->chan[chan_cnt].freq == 2467 || req->chan[chan_cnt].freq == 2472 || req->chan[chan_cnt].freq == 2484)
					||(req->chan[chan_cnt].freq == 5260 || req->chan[chan_cnt].freq == 5280 || req->chan[chan_cnt].freq == 5300 || req->chan[chan_cnt].freq == 5320))) {
					req->chan[chan_cnt].flags |= CHAN_NO_IR;
					// os_printf("XXX disable IR chan for %d\n", req->chan[i].freq);
			}
			#else
			scan_freq = req->chan[chan_cnt].freq;
			//disable 12/13/14,52/56/60/64 active scan
			if (scan_freq == 2467 || scan_freq == 2472 || scan_freq == 2484)
				req->chan[chan_cnt].flags |= CHAN_NO_IR;
			
			#if CONFIG_WIFI_BAND_5G
			if (scan_freq == 5260 || scan_freq == 5280 || scan_freq == 5300 || scan_freq == 5320)
				req->chan[chan_cnt].flags |= CHAN_NO_IR;
			#endif
			#endif // CONFIG_WIFI_AUTO_COUNTRY_CODE
			#endif // CONFIG_WIFI_REGDOMAIN
			chan_cnt++;
		}
		req->chan_cnt = chan_cnt;
		// RWNX_LOGI("XXX Using specified freqs, chan_cnt %d\n", req->chan_cnt);
	}

#if defined(CONFIG_WIFI_SCAN_CH_TIME) && CONFIG_WIFI_SCAN_CH_TIME
	if (scancd_env.table_nm) {
		int j;
		uint16_t freq;
		if (scancd_env.tb != NULL) {
			for (i = 0; i < scancd_env.table_nm; i++) {
				freq = rw_ieee80211_get_centre_frequency(scancd_env.tb[i].ch_nb);
				if (scancd_env.tb[i].ch_nb == 0 || freq == 0) {
					RWNX_LOGI("channel_number error\r\n");
					break;
				}

				for (j = 0; j < req->chan_cnt; j++) {
					if (req->chan[j].freq == freq) {
						req->duration[j] = scancd_env.tb[i].duration;
						break;
					}
				}
			}
		}
	}
#endif

	if (scan_param_env.set_param) {
		if(1 == scan_param_env.scan_band) {
			rw_ieee80211_init_scan_chan_band(req,IEEE80211_BAND_2GHZ);
		}
		else if(2 == scan_param_env.scan_band) {
			rw_ieee80211_init_scan_chan_band(req,IEEE80211_BAND_5GHZ);
		}
		else if(0 != scan_param_env.chan_cnt) {
			chan_cnt = 0;
			for (i = 0; i < scan_param_env.chan_cnt; i++) {
				uint16_t freq;
				freq = rw_ieee80211_get_centre_frequency(scan_param_env.chan_nb[i]);
				if (freq == 0) {
					RWNX_LOGI("rw_msg_send_scanu_req:channel_number error\r\n");
					break;
				}

				if (freq >= 5925) {
					band = IEEE80211_BAND_6GHZ;
				} else if (freq >= 4900) {
					band = IEEE80211_BAND_5GHZ;
				} else {
					band = IEEE80211_BAND_2GHZ;
				}

				flags = find_ieee80211_freq_flags(freq, wiphy->bands[band]->channels,wiphy->bands[band]->n_channels);
				if((flags & IEEE80211_CHAN_DISABLED) && (!(ate_is_enabled() || rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid))))
				{
					RWNX_LOGW("rw_msg_send_scanu_req:freq %d,CHAN_DISABLED\r\n ",freq);
				    continue;
				}

				req->chan[chan_cnt].freq = freq;
				req->chan[chan_cnt].band = band;
				req->chan[chan_cnt].tx_power = VIF_UNDEF_POWER;
				if(1 == scan_param_env.scan_type) {
					req->chan[chan_cnt].flags |= CHAN_NO_IR;
				}
				else {
					#if CONFIG_WIFI_REGDOMAIN
					if(!(ate_is_enabled() || rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid)))
					{
						flags = find_ieee80211_freq_flags(req->chan[chan_cnt].freq, wiphy->bands[req->chan[chan_cnt].band]->channels,
									wiphy->bands[req->chan[chan_cnt].band]->n_channels);
						req->chan[chan_cnt].flags = get_chan_flags(flags);
					}
					else
					{
						req->chan[chan_cnt].flags = 0;
						//os_printf("rw_msg_send_scanu_req:freq %d ,flags 0x%x\r\n ",req->chan[i].freq,req->chan[i].flags);
					}
					#else
					req->chan[chan_cnt].flags = 0;
					scan_freq = req->chan[chan_cnt].freq;
					//disable 12/13/14,52/56/60/64 active scan
					if (scan_freq == 2467 || scan_freq == 2472 || scan_freq == 2484)
						req->chan[chan_cnt].flags |= CHAN_NO_IR;

					#if CONFIG_WIFI_BAND_5G
					if (scan_freq == 5260 || scan_freq == 5280 || scan_freq == 5300 || scan_freq == 5320)
						req->chan[chan_cnt].flags |= CHAN_NO_IR;
					#endif
					#endif // CONFIG_WIFI_REGDOMAIN
				}
				chan_cnt++;
			}
			req->chan_cnt = chan_cnt;
			RWNX_LOGW("Using specified freqs\n");
		}

		if(0 != scan_param_env.scan_type) {
			for (i = 0; i < req->chan_cnt; i++) {
				req->chan[i].flags |= CHAN_NO_IR;
			}
		} 
		req->duration = scan_param_env.duration;

		os_memcpy(&scan_param_dump_env, &scan_param_env, sizeof(scan_param_env));
		scan_param_env.set_param = 0;

	}

	req->add_ie_len = scan_param->extra_ies_len;
	os_memcpy(extra_ies, scan_param->extra_ies, req->add_ie_len);
	req->add_ies = (uint32_t)extra_ies;

	/* Send the SCANU_START_REQ message to LMAC FW */
	return rw_msg_send(req, 0, SCANU_START_CFM, NULL);
}

int rw_msg_send_scanu_fast_req(FAST_SCAN_PARAM_T *fscan_param)
{
	struct scanu_fast_req *req;

	/* Build the SCANU_START_REQ message */
	req = ke_msg_alloc(SCANU_FAST_REQ, TASK_SCANU, TASK_API,
					   sizeof(struct scanu_fast_req));

	req->bssid = fscan_param->bssid;
	req->ch_nbr = fscan_param->ch_num;
	req->maxch_time = fscan_param->max_ch_time;
	req->minch_time = fscan_param->min_ch_time;
	req->probe_delay = fscan_param->probe_delay;
	req->ssid = fscan_param->ssid;

	return rw_msg_send(req, 0, SCANU_FAST_CFM, NULL);
}

int rw_msg_send_connection_loss_ind(u8 vif_index)
{
	struct mm_connection_loss_ind *ind = ke_msg_alloc(MM_CONNECTION_LOSS_IND,
										 TASK_SM, TASK_API, sizeof(struct mm_connection_loss_ind));

	// Fill-in the indication message parameters
	ind->inst_nbr = vif_index;

	// Send the indication to the upper layers
	return rw_msg_send(ind, 0, 0, NULL);
}

int rw_msg_get_bss_info(u8 vif_idx, void *cfm)
{
	struct me_get_bss_info_req *req = NULL;

	if (vif_idx >= NX_VIRT_DEV_MAX)
		return -1;

	req = ke_msg_alloc(ME_GET_BSS_INFO_REQ, TASK_ME, TASK_API,
					   sizeof(struct me_get_bss_info_req));

	if (!req)
		return BK_ERR_NO_MEM;

	req->vif_idx = vif_idx;

	return rw_msg_send(req, 1, ME_GET_BSS_INFO_CFM, cfm);
}

int rw_msg_get_signal_info(u8 vif_idx, SIGNAL_INFO_PARAM_T *signal_info_param)
{
#if (NX_MDM_VER >= 20)
	uint8_t read_counter = 10;
#endif //(NX_MDM_VER >= 20)
	void *vif;
	struct phy_channel_info info;

	/* get channel frequency */
	phy_get_channel(&info, 0);
	signal_info_param->frequency = (info.info1 >> 16) & 0xffff;

	/* get link noise */
	signal_info_param->current_noise = 0;
#if (NX_MDM_VER >= 20)
		// If read noise value is 0, value is not valid because of re-sync between AGC clock
		// and AHB clock. Register value is updated once AGC is in listen state.
	while (read_counter--)
	{
		// During scan, config used is 1x1, noise has to be read to antenna 0
		int reg_noise = (int)bk_wifi_get_noise();
		if (reg_noise != 0)
		{
			signal_info_param->current_noise = reg_noise;
			break;
		}
	}
#endif //(NX_MDM_VER >= 20)

	/* get link signal */
	vif = mac_vif_mgmt_get_entry(vif_idx);
	signal_info_param->current_signal = mac_vif_mgmt_get_u_sta_rssi(vif);
	signal_info_param->avg_signal = mac_vif_mgmt_get_u_sta_smooth_rssi(vif);

	return 0;
}

int rw_msg_send_mm_signal_monitor(SIGNAL_MONITOR_PARAM_T *signal_monitor_param)
{
	struct mm_cfg_rssi_req *req;

	/* Build the MM_CFG_RSSI_REQ message */
	req = ke_msg_alloc(MM_CFG_RSSI_REQ, TASK_MM, TASK_API,
						sizeof(struct mm_cfg_rssi_req));
	if (!req)
		return BK_ERR_NO_MEM;

	req->vif_index = signal_monitor_param->vif_idx;
	req->rssi_thold = signal_monitor_param->threshold;
	req->rssi_hyst = signal_monitor_param->hysteresis;

	/* Send the MM_CFG_RSSI_REQ message to LMAC FW */
	return rw_msg_send(req, 0, 0/*DUMMY*/, NULL);
}

int rw_msg_get_channel_info(u8 vif_idx, PHY_CHAN_INFO_T *channel_info)
{
	struct phy_channel_info info;
	int channel;
	uint8_t band;

	/* get phy channel */
	phy_get_channel(&info, 0);

	band = info.info1 & 0xff;
	channel_info->frequency = (info.info1 >> 16) & 0xffff;
	channel_info->chanwidth = (info.info1 >> 8) & 0xff;
	channel_info->center_frq1 = info.info2 & 0xffff;
	channel_info->center_frq2 = (info.info2 >> 16) & 0xffff;

	/* frequency transfer into channel */
	channel = phy_freq_to_channel(band, channel_info->frequency);

	/* get seg1_idx */
	if (channel_info->center_frq2)
		channel_info->seg1_idx = 36 + (channel_info->center_frq2 - 5180) / 5;

	/* get sec_channel */
	if (channel_info->chanwidth == PHY_CHNL_BW_20)
		channel_info->sec_channel = 0;
	else if (channel_info->chanwidth == PHY_CHNL_BW_40) // FIXME ??
		channel_info->sec_channel = (((channel - 1) / 4) % 2) ? -1 : 1;

	return 0;
}

int rw_msg_get_channel(void *cfm)
{
	struct phy_channel_info info;

	phy_get_channel(&info, 0);

	return info.info2;
}

int rw_msg_set_filter(uint32_t filter)
{
	struct mm_set_filter_req *set_filter_req_param;
	uint32_t rx_filter = 0;

	/* Build the MM_SET_FILTER_REQ message */
	set_filter_req_param =
		ke_msg_alloc(MM_SET_FILTER_REQ, TASK_MM, TASK_API,
					 sizeof(struct mm_set_filter_req));
	if (!set_filter_req_param)
		return BK_ERR_NO_MEM;

	/* Now copy all the flags into the message parameter */
	set_filter_req_param->filter = rx_filter;


	/* Send the MM_SET_FILTER_REQ message to LMAC FW */
	return rw_msg_send(set_filter_req_param, 1, MM_SET_FILTER_CFM, NULL);
}

int rw_msg_set_channel(uint32_t channel, uint32_t band_width, void *cfm)
{
	struct mm_set_channel_req *set_chnl_par;

	set_chnl_par = ke_msg_alloc(MM_SET_CHANNEL_REQ, TASK_MM, TASK_API,
								sizeof(struct mm_set_channel_req));
	if (!set_chnl_par)
		return BK_ERR_NO_MEM;

	// FIXME: BK7239 6E channel starts with channel 1
	if (channel <= 14) {
		set_chnl_par->chan.band = PHY_BAND_2G4;
	} else {
		set_chnl_par->chan.band = PHY_BAND_5G;
	}
	set_chnl_par->chan.type = band_width;//PHY_CHNL_BW_20;
	set_chnl_par->chan.prim20_freq = set_chnl_par->chan.center1_freq =
										 rw_ieee80211_get_centre_frequency(channel);
	set_chnl_par->chan.center2_freq = 0;
	set_chnl_par->chan.tx_power = VIF_UNDEF_POWER;
	set_chnl_par->index = PHY_PRIM;

	/* Send the MM_SET_CHANNEL_REQ REQ message to LMAC FW */
	return rw_msg_send(set_chnl_par, 1, MM_SET_CHANNEL_CFM, cfm);
}

int rw_msg_send_scan_cancel_req(void *cfm)
{
	struct scan_cancel_req *req;

	/* Build the SCAN_CANCEL_REQ message */
	req = ke_msg_alloc(SCAN_CANCEL_REQ, TASK_SCAN, TASK_API,
					   sizeof(struct scan_cancel_req));
	if (!req)
		return BK_ERR_NO_MEM;

	/* Send the SCAN_CANCEL_REQ message to LMAC FW */
	return rw_msg_send(req, 1, SCAN_CANCEL_CFM, cfm);
}

int rw_msg_send_sm_disconnect_req(DISCONNECT_PARAM_T *param)
{
	struct ke_msg cfm;
	struct sm_disconnect_req *req;
	int ret;

	/* Build the SM_DISCONNECT_REQ message */
	req = ke_msg_alloc(SM_DISCONNECT_REQ, TASK_SM, TASK_API,
					   sizeof(struct sm_disconnect_req));
	if (!req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the SM_DISCONNECT_REQ message */
	req->reason_code = param->reason_code;
	req->vif_idx = param->vif_idx;

	ret=rw_msg_send(req, 1, SM_DISCONNECT_CFM, &cfm);
	return ret;
}

int rw_msg_send_sm_connect_req(CONNECT_PARAM_T *sme, void *cfm)
{
	struct sm_connect_req *req;

	/* Build the SM_CONNECT_REQ message */
	req = ke_msg_alloc(SM_CONNECT_REQ, TASK_SM, TASK_API,
					   sizeof(struct sm_connect_req) + sme->bcn_len);
	if (!req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the SM_CONNECT_REQ message */
	req->ssid.length = sme->ssid.length;
	os_memcpy(req->ssid.array, sme->ssid.array, sme->ssid.length);
	os_memcpy(&req->bssid, &sme->bssid, sizeof(sme->bssid));

	req->vif_idx = sme->vif_idx;
	req->chan.band = sme->chan.band;
	req->chan.flags = sme->chan.flags;
	req->chan.freq = sme->chan.freq;
	req->chan.tx_power = sme->chan.tx_power;
	req->flags = sme->flags;
	req->ctrl_port_ethertype = PP_HTONS(ETH_P_PAE);
	req->ie_len = sme->ie_len;
	req->auth_type = sme->auth_type;

	BK_ASSERT(req->ie_len < sizeof(req->ie_buf));

	os_memcpy((UINT8 *)req->ie_buf, (UINT8 *)sme->ie_buf, req->ie_len);

	req->bcn_len = sme->bcn_len;

	if (req->bcn_len)
		os_memcpy((UINT8 *)req->bcn_buf, (UINT8 *)sme->bcn_buf, req->bcn_len);


#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
	rwnx_connecting_handler(req->vif_idx); // FIXME: move to right place
#endif

	/* Send the SM_CONNECT_REQ message to LMAC FW */
	return rw_msg_send(req, 1, SM_CONNECT_CFM, cfm);
}

#ifdef CONFIG_IEEE80211R
int rw_msg_update_ft_ies(UPDATE_FT_IES_PARAM_T *ft_ies)
{
	struct sm_connect_req *req;

	req = ke_msg_alloc(SM_FT_AUTH_RSP, TASK_SM, TASK_API, sizeof(struct sm_connect_req));
	if (!req)
		return BK_ERR_NO_MEM;

	req->ie_len = ft_ies->ft_ies_len;
	os_memcpy(req->ie_buf, ft_ies->ft_ies, ft_ies->ft_ies_len);

	return rw_msg_send(req, 0, 0, NULL);
}
#endif

#ifdef CONFIG_SAE_EXTERNAL
int rw_msg_send_sm_external_auth_status(EXTERNAL_AUTH_PARAM_T *auth_param)
{
	struct sm_external_auth_required_rsp *req;

	/* Build the SM_CONNECT_REQ message */
	req = ke_msg_alloc(SM_EXTERNAL_AUTH_REQUIRED_RSP, TASK_SM, TASK_API,
					   sizeof(struct sm_external_auth_required_rsp));
	if (!req)
		return BK_ERR_NO_MEM;

	req->vif_idx = auth_param->vif_idx;
	req->status = auth_param->status;

	/* Send the SM_CONNECT_REQ message to LMAC FW */
	return rw_msg_send(req, 0, 0/*DUMMY*/, NULL);
}
#endif

int rw_msg_send_arp_set_rate_req(uint8_t vif_idx, uint16_t arp_rate)
{
	struct mm_set_arp_tx_rate_req *req;

	/* Build the ARP TX RATE REQ message */
	req = ke_msg_alloc(MM_SET_ARP_TX_RATE_REQ, TASK_MM, TASK_API,
                       sizeof(struct mm_set_arp_tx_rate_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_index = vif_idx;
	req->arp_tx_rate = arp_rate;

	/* Send the ARP TX RATE REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_ARP_TX_RATE_CFM, NULL);
}

int rw_msg_send_listen_interval_req(uint8_t vif_idx, uint8_t interval)
{
	struct mm_set_listen_interval_req *req;
	/* Build the MM_BA_DEL_REQ message */
	req = ke_msg_alloc(MM_SET_LISTEN_INTERVAL_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_set_listen_interval_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_index = vif_idx;
	req->listen_interval = interval;

	/* Send the MM_BA_DEL_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_LISTEN_INTERVAL_CFM, NULL);
}

int rw_msg_get_listen_interval_req(void *cfm)
{
	struct mm_get_listen_interval_req *req = NULL;

	req = ke_msg_alloc(MM_GET_LISTEN_INTERVAL_REQ, TASK_MM, TASK_API,
					   sizeof(struct mm_get_listen_interval_req));

	if (!req)
		return BK_ERR_NO_MEM;

	return rw_msg_send(req, 1, MM_GET_LISTEN_INTERVAL_CFM, cfm);
}

int rw_msg_send_bcn_loss_int_req(uint8_t vif_idx, uint8_t interval,uint8_t repeat_num)
{
	struct mm_set_bcn_loss_int_req *req;
	/* Build the MM_BA_DEL_REQ message */
	req = ke_msg_alloc(MM_SET_BCN_LOSS_INT_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_set_bcn_loss_int_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_index = vif_idx;
	req->listen_interval = interval;
	req->repeat_num = repeat_num;

	/* Send the MM_BA_DEL_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_BCN_LOSS_INT_CFM, NULL);
}

int rw_msg_send_bcn_loss_time_req(uint8_t vif_idx, uint8_t wait_cnt,uint8_t wake_cnt)
{
	struct mm_set_bcn_loss_time_req *req;
	/* Build the MM_SET_BCN_LOSS_TIME_REQ message */
	req = ke_msg_alloc(MM_SET_BCN_LOSS_TIME_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_set_bcn_loss_time_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_index = vif_idx;
	req->wait_cnt = wait_cnt;
	req->wake_cnt = wake_cnt;

	/* Send the MM_SET_BCN_LOSS_TIME_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_BCN_LOSS_TIME_CFM, NULL);
}

int rw_msg_send_bcn_recv_win_req(uint8_t vif_idx, uint8_t default_win,uint8_t max_win, uint8_t step)
{
	struct mm_set_bcn_recv_win_req *req;
	/* Build the MM_SET_BCN_RECV_WIN_REQ message */
	req = ke_msg_alloc(MM_SET_BCN_RECV_WIN_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_set_bcn_recv_win_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_index = vif_idx;
	req->default_win= default_win;
	req->max_win = max_win;
	req->step = step;

	/* Send the MM_SET_BCN_RECV_WIN_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_BCN_RECV_WIN_CFM, NULL);
}

int rw_msg_send_bcn_miss_time_req(uint8_t vif_idx, uint8_t bcnmiss_time)
{
	struct mm_set_bcn_miss_time_req *req;
	/* Build the MM_SET_BCN_LOSS_TIME_REQ message */
	req = ke_msg_alloc(MM_SET_BCN_MISS_TIME_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_set_bcn_miss_time_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_index = vif_idx;
	req->bcnmiss_time = bcnmiss_time;

	/* Send the MM_SET_BCN_LOSS_TIME_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_BCN_MISS_TIME_CFM, NULL);
}

int rw_msg_send_conn_delay_time_ind(uint8_t vif_idx, uint8_t conn_delay_time)
{
	struct sm_conn_delay_time_ind *ind;
	/* Build the SM_CONN_DELAY_TIME_IND message */
	ind = ke_msg_alloc(SM_CONN_DELAY_TIME_IND, TASK_SM, TASK_API,
                          sizeof(struct sm_conn_delay_time_ind));
	if (!ind)
		return -ENOMEM;

	/* Set parameters */
	ind->vif_idx = vif_idx;
	ind->conn_delay_time = conn_delay_time;

	/* Send the SM_CONN_DELAY_TIME_IND message to LMAC FW */
	return rw_msg_send(ind, 0, 0, NULL);
}

int rw_msg_send_psdebug_interval_req(uint8_t interval)
{
    struct mm_set_psdebug_interval_req *req;

    req = ke_msg_alloc(MM_SET_PSDEBUG_INTERVAL_REQ, TASK_MM, TASK_API,
                          sizeof(struct mm_set_psdebug_interval_req));
    if (!req)
        return -ENOMEM;

    /* Set parameters */
    req->psdebug_interval = interval;
    return rw_msg_send(req, 0, 0, NULL);
}

void rw_msg_send_psdebug_req(uint8_t index)
{
	extern void ps_set_debug_info(uint8_t index);
	ps_set_debug_info(index);
}

int rw_msg_send_td_interval_req(uint8_t interval, uint8_t max_pnk)
{
	extern void ps_set_td_info(uint8_t interval,uint8_t max_pnk);
	ps_set_td_info(interval,max_pnk);
	return 0;
}

int rw_msg_send_td_mask_req(void)
{
	extern void ps_mask_td_info(void);
	ps_mask_td_info();
	return 0;
}

int rw_msg_send_mm_mqtt_info(bool mqtt_connect, bool is_send_pingreq, uint32_t period_s)
{
	struct me_mqtt_keepalive_info_ind *ind;

	/* Build the ME_MQTT_KEEPALIVE_INFO_IND message */
	ind = ke_msg_alloc(ME_MQTT_KEEPALIVE_INFO_IND, TASK_ME, TASK_API,
					   sizeof(struct me_mqtt_keepalive_info_ind));
	if (!ind)
		return BK_ERR_NO_MEM;

	ind->mqtt_conn = mqtt_connect;
	ind->send_pingreq = is_send_pingreq;
	ind->period_s = period_s;

	/* Send the ME_MQTT_KEEPALIVE_INFO_IND message to LMAC FW */
	return rw_msg_send(ind, 0, 0, NULL);
}

int rw_msg_send_tim_update(u8 vif_idx, u16 aid, u8 tx_status)
{
	struct mm_tim_update_req *req;

	/* Build the MM_TIM_UPDATE_REQ message */
	req = ke_msg_alloc(MM_TIM_UPDATE_REQ, TASK_MM, TASK_API,
					   sizeof(struct mm_tim_update_req));
	if (!req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the MM_TIM_UPDATE_REQ message */
	req->aid = aid;
	req->tx_avail = tx_status;
	req->inst_nbr = vif_idx;

	/* Send the MM_TIM_UPDATE_REQ message to LMAC FW */
	return rw_msg_send(req, 0, MM_TIM_UPDATE_CFM, NULL);
}

int rw_msg_set_power(u8 vif_idx, u8 power)
{
	struct mm_set_power_req *req;

	/* Build the MM_SET_POWER_REQ message */
	req = ke_msg_alloc(MM_SET_POWER_REQ, TASK_MM, TASK_NONE,
					   sizeof(struct mm_set_power_req));
	if (!req)
		return BK_ERR_NO_MEM;

	/* Set parameters for the MM_SET_POWER_REQ message */
	req->inst_nbr = vif_idx;
	req->power = power;

	/* Send the MM_SET_POWER_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_SET_POWER_CFM, NULL);
}

#if CONFIG_P2P
int rw_msg_send_roc(u8 vif_index, unsigned int freq, uint32_t duration)
{
	struct mm_remain_on_channel_req *req;
	struct mm_remain_on_channel_cfm cfm;
	int ret;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
	struct rwnx_roc_elem *roc_elem;

	/* FIXME: LOCK */
    /* Check that no other RoC procedure has been launched */
	//RWNX_LOGI("req:%d %d %d\n", freq, duration, !!rwnx_hw->roc_elem);
	if (rwnx_hw->roc_elem) {
		return BK_ERR_BUSY;
	}

	roc_elem = os_zalloc(sizeof(struct rwnx_roc_elem));
	if (!roc_elem)
		return BK_ERR_NO_MEM;

	roc_elem->freq = freq;
	roc_elem->duration = duration;

	req = ke_msg_alloc(MM_REMAIN_ON_CHANNEL_REQ, TASK_MM, TASK_API,
					   sizeof(struct mm_remain_on_channel_req));
	if (!req) {
		os_free(roc_elem);
		return BK_ERR_NO_MEM;
	}

	req->op_code      = MM_ROC_OP_START;
	req->vif_index    = vif_index;

	if (freq >= 5925) {
		// FIXME: BK7239 6E
	} else if (freq >= 4900) {
		req->chan.band = PHY_BAND_5G;
	} else {
		req->chan.band = PHY_BAND_2G4;
	}
	req->chan.type = PHY_CHNL_BW_20;
	req->chan.prim20_freq = freq;
	req->chan.center1_freq = freq;
	req->duration_ms  = duration;

	/* Send the MM_REMAIN_ON_CHANNEL_REQ message to LMAC FW */
	ret = rw_msg_send(req, 1, MM_REMAIN_ON_CHANNEL_CFM, &cfm);
	if (ret || cfm.status) {
		RWNX_LOGE("%s: failed ret %d, cfm.status %d\n", __func__, ret, cfm.status);
		os_free(roc_elem);
		return -1;
	} else {
		rwnx_hw->roc_elem = roc_elem;
	}

	return ret;
}

int rw_msg_send_cancel_roc(u8 vif_index)
{
	struct mm_remain_on_channel_req *req;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

	if (!rwnx_hw->roc_elem)
		return BK_ERR_PARAM;

	req = ke_msg_alloc(MM_REMAIN_ON_CHANNEL_REQ, TASK_MM, TASK_API,
					   sizeof(struct mm_remain_on_channel_req));
	if (!req)
		return BK_ERR_NO_MEM;

	req->vif_index = vif_index;
	req->op_code      = MM_ROC_OP_CANCEL;

	/* Send the MM_REMAIN_ON_CHANNEL_REQ message to LMAC FW */
	return rw_msg_send(req, 1, MM_REMAIN_ON_CHANNEL_CFM, NULL);
}

#endif // CONFIG_P2P

int rw_msg_send_arp_msg(u8 vif_idx)
{
	struct me_sta_arp_update_ind *ind;

	/* Build the ME_STA_ARP_UPDATE_IND message */
	ind = ke_msg_alloc(ME_STA_ARP_UPDATE_IND, TASK_ME, TASK_NONE,
					   sizeof(struct me_sta_arp_update_ind));
	if (!ind)
		return BK_ERR_NO_MEM;

	/* Set parameters for the ME_STA_ARP_UPDATE_IND message */
	ind->vif_idx = vif_idx;

	/* Send the ME_STA_ARP_UPDATE_IND message to LMAC FW */
	return rw_msg_send(ind, 0, 0, NULL);
}

#if CONFIG_WIFI_FTM
int rw_msg_send_ftm_start_req(uint8_t vif_idx, uint8_t ftm_per_burst, uint8_t nb_ftm_rsp, void *ind)
{
	struct ftm_start_req *req;
	/* Build the FTM_START_REQ message */
	req = ke_msg_alloc(FTM_START_REQ, TASK_FTM, TASK_API,
						  sizeof(struct ftm_start_req));
	if (!req)
		return -ENOMEM;

	/* Set parameters */
	req->vif_idx = vif_idx;
	req->ftm_per_burst = ftm_per_burst;
	req->nb_ftm_rsp = nb_ftm_rsp;

	/* Send the FTM_START_REQ message to LMAC FW */
	return rw_msg_send(req, 1, FTM_DONE_IND, ind);
}
#endif
int rw_msg_send_recycle_txlist_req(uint8_t sta_idx)
{
	struct mm_recycle_txlist_req *param;

	/* RESET REQ has no parameter */
	param = ke_msg_alloc(MM_RECYCLE_TXLIST_REQ, TASK_MM, TASK_API,
								sizeof(struct mm_recycle_txlist_req));
	if (!param)
		return -1;

	param->sta_idx = sta_idx;

	return rw_msg_send(param, 0, 0, NULL);

}

#if CONFIG_WIFI_CSI_EN
int rw_msg_send_csi_active_mode_req(wifi_csi_active_mode_config_t *config,struct csi_active_mode_cfm *cfm)
{
	struct csi_active_mode_req *req;

	/* Build the CSI_ACTIVE_MODE_REQ message */
	req = ke_msg_alloc(CSI_ACTIVE_MODE_REQ, TASK_CSI, TASK_API,
					   sizeof(struct csi_active_mode_req));
	if (!req)
		return BK_ERR_NO_MEM;

	req->enable = config->enable;
	req->interval = config->interval;

	/* Send the CSI_ACTIVE_MODE_REQ message to LMAC FW */
	return rw_msg_send(req, 1, CSI_ACTIVE_MODE_CFM, (void *)cfm);
}

int rw_msg_send_csi_receive_mode_req(wifi_csi_receive_mode_config_t *config,struct csi_receive_mode_cfm *cfm)
{
	struct csi_receive_mode_req *req;

	/* Build the CSI_RECEIVE_MODE_REQ message */
	req = ke_msg_alloc(CSI_RECEIVE_MODE_REQ, TASK_CSI, TASK_API,
					   sizeof(struct csi_receive_mode_req));
	if (!req)
		return BK_ERR_NO_MEM;

	req->enable = config->enable;
	os_memcpy(&req->filter_config, &config->filter_config, sizeof(wifi_csi_filter_config_t));

	/* Send the CSI_RECEIVE_MODE_REQ message to LMAC FW */
	return rw_msg_send(req, 1, CSI_RECEIVE_MODE_CFM, (void *)cfm);
}
#endif // CONFIG_WIFI_CSI_EN
