//#include <common/bk_include.h>
//#include <common/bk_err.h>
//#include <os/mem.h>
//#include <os/str.h>
//#include <components/log.h>
//#include <components/event.h>
//#include <modules/wifi.h>
//#include <components/system.h>

#include "hostapd_intf.h"
#include <../wpa_supplicant-2.10/bk_patch/sk_intf.h>
#include "../wpa_supplicant-2.10/bk_patch/fake_socket.h"
//#include "ieee802_11_defs.h"
#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "../wpa_supplicant-2.10/src/drivers/driver.h"
#include "../wpa_supplicant-2.10/src/drivers/driver_beken.h"
#include "../wpa_supplicant-2.10/hostapd/hostapd_cfg.h"
#include "bk_rw.h"
#include "bk_wifi_types.h"
#include "rw_msg_rx.h"

//#include "ieee802_11_common.h"
#include "../wpa_supplicant-2.10/src/common/ieee802_11_common.h"
#include "rw_msdu.h"
#include "rwnx_rx.h"
#include "bk_wifi_private.h"
//#include "wpa_ctrl.h"
#include "../wpa_supplicant-2.10/src/common/wpa_ctrl.h"
#if CONFIG_TX_BUFING
#include "rw_tx_buffering.h"
#endif
#include "rwnx_defs.h"
#include "../wpa_supplicant-2.10/wpa_supplicant/ctrl_iface.h"
#include "rwnx_intf.h"
#include "rwnx_params.h"

#define TAG "hitf"
#include "common.h"
#include "bk_wifi.h"
#include "bk_wifi_prop_private.h"
//#include "pbuf.h"
#include "lwip/pbuf.h"
#include "bk_feature.h"
#if CONFIG_EASY_FLASH_FAST_CONNECT
#include "bk_ef.h"
#endif
#include "bk_lwipif_tizenrt.h"
//#include "generated/lmac_msg.h"

#ifdef CONFIG_SME
extern int sa_station_send_associate_cmd(ASSOC_PARAM_T *assoc_param);
#else
extern int sa_station_send_associate_cmd(CONNECT_PARAM_T *connect_param);
#endif

/* forward declaration */
FUNC_1PARAM_PTR bk_wlan_get_status_cb(void);
void wpa_hostapd_release_scan_rst(void);

bk_ap_no_password_cb_t bk_ap_no_password_connected = NULL;

void bk_ap_no_password_connected_register_cb(bk_ap_no_password_cb_t func)
{
	bk_ap_no_password_connected = func;
}


struct scanu_rst_upload *s_scan_result_upload_ptr;

struct mm_bcn_change_req *hadp_intf_get_bcn_change_req(uint8_t vif_id, struct beacon_data *bcn_info)
{
	uint8_t tim_ie[BCN_TIM_IE_LEN] = {5, 4, 0, 2, 0, 0};
	uint8_t *beacon_ptr = 0;
	uint32_t bcn_len;
	struct mm_bcn_change_req *req = 0;

#ifdef ADD_BEACON_IES_ENABLE
	bcn_len = bcn_info->head_len + BCN_TIM_IE_LEN + bcn_info->tail_len
			  + bcn_info->beacon_ies_len;
#else
	bcn_len = bcn_info->head_len + BCN_TIM_IE_LEN + bcn_info->tail_len;
#endif

	beacon_ptr = (uint8_t *)os_malloc(bcn_len);
	if (!beacon_ptr)
		goto exit_get_failed;

	req = ke_msg_alloc(MM_BCN_CHANGE_REQ, TASK_MM, TASK_API,
					   sizeof(struct mm_bcn_change_req));
	if (!req)
		goto exit_get_failed;

	if (bcn_info->head && bcn_info->head_len)
		os_memcpy(beacon_ptr, bcn_info->head, bcn_info->head_len);

	os_memcpy((void *)((uint32_t)beacon_ptr + bcn_info->head_len), tim_ie, BCN_TIM_IE_LEN);

	if (bcn_info->tail && bcn_info->tail_len) {
		os_memcpy((void *)((uint32_t)beacon_ptr + bcn_info->head_len + BCN_TIM_IE_LEN),
				  bcn_info->tail, bcn_info->tail_len);
	}

#ifdef ADD_BEACON_IES_ENABLE
	if (bcn_info->beacon_ies && bcn_info->beacon_ies_len) {
		os_memcpy((void *)((uint32_t)beacon_ptr + bcn_info->head_len + bcn_info->tail_len),
				  bcn_info->beacon_ies, bcn_info->beacon_ies_len);
	}
#endif // ADD_BEACON_IES_ENABLE

	req->bcn_ptr = (uint32_t)(beacon_ptr);
	req->bcn_len = bcn_len;
	req->tim_len = BCN_TIM_IE_LEN;
	req->tim_oft = bcn_info->head_len;
	req->inst_nbr = vif_id;

	return req;

exit_get_failed:
	if (req)
		os_free(req);

	if (beacon_ptr)
		os_free(beacon_ptr);

	return NULL;
}

struct mm_bcn_change_req *hadp_intf_get_csa_bcn_req(struct csa_settings *settings,
		uint8_t vif_id, struct beacon_data *bcn_info)
{
	uint16_t csa_cnt_offset;
	struct mm_bcn_change_req *req;

	req = hadp_intf_get_bcn_change_req(vif_id, bcn_info);
	if (!req)
		return NULL;

	// TBD: csa & esca
	csa_cnt_offset = settings->counter_offset_beacon[0] + 6 + bcn_info->head_len; // tim_ie_len == 6
	req->csa_oft[0] =  csa_cnt_offset & 0xff;
	req->csa_oft[1] = (csa_cnt_offset & 0xff00) >> 8;

	return req;
}

struct mm_bcn_change_req *hadp_intf_get_csa_after_bcn_req(uint8_t vif_id, struct beacon_data *bcn_info)
{
	return hadp_intf_get_bcn_change_req(vif_id, bcn_info);
}


int rwnx_build_bcn(BCN_PARAM_ST *csa, uint8_t vif_idx, struct beacon_data *bcn_info, struct csa_settings *settings, bool csa_after)
{
	uint32_t bcn_len;
	uint8_t *beacon_ptr = 0;
	uint8_t tim_ie[BCN_TIM_IE_LEN] = {5, 4, 0, 2, 0, 0};

	bcn_len = bcn_info->head_len + BCN_TIM_IE_LEN + bcn_info->tail_len + bcn_info->beacon_ies_len;
	beacon_ptr = (uint8_t *)os_malloc(bcn_len);
	if (!beacon_ptr)
		goto failed;

	if (bcn_info->head && bcn_info->head_len)
		os_memcpy(beacon_ptr, bcn_info->head, bcn_info->head_len);

	os_memcpy((void *)((uint32_t)beacon_ptr + bcn_info->head_len), tim_ie, BCN_TIM_IE_LEN);

	if (bcn_info->tail && bcn_info->tail_len)
		os_memcpy((void *)((uint32_t)beacon_ptr + bcn_info->head_len + BCN_TIM_IE_LEN),
				  bcn_info->tail, bcn_info->tail_len);

	if (bcn_info->beacon_ies && bcn_info->beacon_ies_len)
		os_memcpy((void *)((uint32_t)beacon_ptr + bcn_info->head_len + bcn_info->tail_len),
				  bcn_info->beacon_ies, bcn_info->beacon_ies_len);

	csa->bcn_ptr = (u32 *)beacon_ptr;
	csa->bcn_len = bcn_len;
	csa->vif_idx = vif_idx;
	csa->tim_len = BCN_TIM_IE_LEN;
	csa->tim_oft = bcn_info->head_len;
	csa->csa_oft[0] = csa->csa_oft[1] = 0;

	// set csa/ecsa counter offset from beacon header
	if (!csa_after) {
		csa->csa_oft[0] = settings->counter_offset_beacon[0] + BCN_TIM_IE_LEN + bcn_info->head_len;
		csa->csa_oft[1] = settings->counter_offset_beacon[1] + BCN_TIM_IE_LEN + bcn_info->head_len;
	}

	//print_hex_dump("XXX BCN_BUILD: ", beacon_ptr, bcn_len);
	//BK_WIFI_LOGI(TAG, "%s %d: csa %d ecsa %d\n", __func__, __LINE__, csa->csa_oft[0], csa->csa_oft[1]);

	return 0;

failed:
	return -1;
}


// TODO:
// 1. release rwnx_hw->csa when exception occurs.
// 2. @settings may contain probe resp, will these set to umac?
int rwnx_cfg80211_channel_switch(uint8_t vif_idx, struct csa_settings *settings)
{
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
	BCN_PARAM_ST param;
	BCN_PARAM_ST *csa;

	if (rwnx_hw->csa) {
		BK_WIFI_LOGE(TAG, "CSA in progress\r\n");
		return -EBUSY;
	}

	// after csa
	csa = (BCN_PARAM_ST *)os_malloc(sizeof(*csa));
	if (!csa)
		return -ENOMEM;

	rwnx_hw->csa = csa;
	csa->vif_idx = vif_idx;

	// After CSA
	os_memset(rwnx_hw->csa, 0, sizeof(*rwnx_hw->csa));
	rwnx_build_bcn(rwnx_hw->csa, vif_idx, &settings->beacon_after, settings, true);
	os_memcpy(&rwnx_hw->freq_params, &settings->freq_params, sizeof(rwnx_hw->freq_params));

	// CSA
	os_memset(&param, 0, sizeof(param));
	rwnx_build_bcn(&param, vif_idx, &settings->beacon_csa, settings, false);
	rw_msg_send_bcn_change(&param);

	// cfg80211_ch_switch_started_notify
	wpa_ctrl_event_copy(WPA_CTRL_EVENT_CHAN_SWITCH_START_IND, &settings->freq_params,
			sizeof(settings->freq_params));

	os_free(param.bcn_ptr);

	return 0;
}

int wpa_intf_channel_switch(struct prism2_hostapd_param *param, int len)
{
	uint8_t vif_id;
	uint8_t chann;
	uint32_t freq;
	uint32_t csa_count;
	void *vif;
	struct csa_settings *settings;

	freq = param->u.chan_switch.freq;
	csa_count = param->u.chan_switch.csa_cnt;

	vif_id = param->vif_idx;
	vif = mac_vif_mgmt_get_entry(vif_id);

	ieee80211_freq_to_chan(freq, &chann);
	if (chann == bk_wlan_ap_get_channel_config()) {
		BK_WIFI_LOGI(TAG, "CSA over same channel\r\n");
		return BK_ERR_HITF_CSA_SAME_CHAN;
	}

	BK_WIFI_LOGI(TAG, "CSA vif=%d vif_id=%d csa_cnt=%d\r\n", vif, param->vif_idx, csa_count);
	settings = param->u.chan_switch.settings;

	rwnx_cfg80211_channel_switch(param->vif_idx, settings);

	return 0;
}

int hapd_intf_sta_add(struct prism2_hostapd_param *param, int len)
{
	int ret;
	struct add_sta_st add_sta;
	struct me_sta_add_cfm *cfm
		= (struct me_sta_add_cfm *)os_malloc(sizeof(struct me_sta_add_cfm));

	if (NULL == cfm) {
		BK_WIFI_LOGE(TAG, "sta_add buf_failed\r\n");
		return BK_ERR_NO_MEM;
	}

	add_sta.aid = param->u.add_sta.aid;
	add_sta.ap_vif_idx = param->vif_idx;
	add_sta.sta_addr = param->sta_addr;
	add_sta.capability = param->u.add_sta.capability;
	add_sta.flags = param->u.add_sta.flags;
	add_sta.supp_rates =  param->u.add_sta.supp_rates;
	add_sta.supp_rates_len = param->u.add_sta.supp_rates_len;

	ret = rw_msg_send_me_sta_add(&add_sta, cfm);
	if (!ret && (cfm->status == CO_OK)) {
		BK_WIFI_LOGI(TAG, "add sta, sta=%d, pm state=%d\r\n", cfm->sta_idx, cfm->pm_state);

#if CONFIG_TX_BUFING
		rwm_tx_bufing_end(cfm->sta_idx);
#endif
	} else if (!ret)
		ret = BK_ERR_HITF_ADD_STA;
	os_free(cfm);

	return ret;
}

void *sta_mgmt_get_sta(const uint8_t *mac);

int hapd_get_sta_info(struct prism2_hostapd_param *param, int len)
{
	void *entry;

	if (NULL == param)
		return BK_ERR_PARAM;

	param->u.get_info_sta.inactive_sec = 0;

	entry = sta_mgmt_get_sta(param->sta_addr);
	uint32_t mac_time = bk_wifi_get_mac_time();

	if (entry && sta_mgmt_get_last_active(entry)) {
		// TODO: bk7236 hal_machw_time may not accessable when enter PS.
		if (mac_time >= sta_mgmt_get_last_active(entry)) {
			param->u.get_info_sta.inactive_sec = (mac_time - sta_mgmt_get_last_active(entry)) / 1000000;
		} else {
			param->u.get_info_sta.inactive_sec = ((uint32_t)-1 - sta_mgmt_get_last_active(entry) + mac_time) / 1000000;
		}
	}

	return 0;
}

int hapd_intf_sta_del(struct prism2_hostapd_param *param, int len)
{
	bool tdls_sta = 0;
	u8 sta_idx;
	sta_idx = rwm_mgmt_sta_mac2idx(param->sta_addr);
	int ret;

	BK_WIFI_LOGI(TAG, "del sta %d, mac %pm, sta_idx %d\r\n",
		sta_idx, param->sta_addr, sta_idx);

	if (sta_idx == 0xff)
		return 0;

#if CONFIG_TX_BUFING
	rwm_tx_bufing_end(sta_idx);
#endif

	ret = rw_msg_send_me_sta_del(sta_idx, tdls_sta);

	if (!ret)
		// notify sta del status: drv_event_disassoc
		wpa_ctrl_event_copy(WPA_CTRL_EVENT_DEL_STATION, param->sta_addr, ETH_ALEN);

	return ret;
}

int hapd_intf_add_key(struct prism2_hostapd_param *param, int len)
{
	int ret;
	KEY_PARAM_T key_param;
	struct mm_key_add_cfm *cfm
		= (struct mm_key_add_cfm *)os_malloc(sizeof(struct mm_key_add_cfm));

	if (NULL == cfm) {
		BK_WIFI_LOGW(TAG, "add key failed: oom\r\n");
		return BK_ERR_NO_MEM;
	}

	if (os_strcmp((char *)param->u.crypt.alg, "WEP40") == 0) {
		BK_WIFI_LOGD(TAG, "add WEP40\r\n");
		key_param.cipher_suite = MAC_CIPHER_WEP40;
	} else if (os_strcmp((char *)param->u.crypt.alg, "WEP104") == 0) {
		BK_WIFI_LOGD(TAG, "add WEP104\r\n");
		key_param.cipher_suite = MAC_CIPHER_WEP104;
	} else if (os_strcmp((char *)param->u.crypt.alg, "TKIP") == 0) {
		BK_WIFI_LOGD(TAG, "add TKIP\r\n");
		key_param.cipher_suite = MAC_CIPHER_TKIP;
	} else if (os_strcmp((char *)param->u.crypt.alg, "CCMP") == 0) {
		BK_WIFI_LOGD(TAG, "add CCMP\r\n");
		key_param.cipher_suite = MAC_CIPHER_CCMP;
	}
    else if(os_strcmp((char *)param->u.crypt.alg, "CCMP-256") == 0)
    {
        BK_WIFI_LOGI(TAG, "add CCMP-256\r\n");
        key_param.cipher_suite = MAC_CIPHER_CCMP_256;
    }
#if CONFIG_PMF
	else if (os_strcmp((char *)param->u.crypt.alg, "BIP") == 0) {
		BK_WIFI_LOGD(TAG, "add BIP\r\n");
		key_param.cipher_suite = MAC_CIPHER_BIP_CMAC_128;
	}
#endif

	if (is_broadcast_ether_addr(param->sta_addr)) {
		BK_WIFI_LOGD(TAG, "add is_broadcast_ether_addr\r\n");
		key_param.sta_idx = 0xFF;
		key_param.inst_nbr = param->vif_idx;
	} else {
		BK_WIFI_LOGD(TAG, "add sta_mgmt_get_sta\r\n");
		key_param.sta_idx = rwm_mgmt_sta_mac2idx(param->sta_addr);
		key_param.inst_nbr = param->vif_idx;
	}

	key_param.key_idx = param->u.crypt.idx;
	key_param.key.length = param->u.crypt.key_len;
	os_memcpy((u8 *) & (key_param.key.array[0]), (u8 *)&param[1], key_param.key.length);

	BK_WIFI_LOGD(TAG, "add key, sta=%d, vif=%d, key=%d\r\n",
			key_param.sta_idx, key_param.inst_nbr, key_param.key_idx);

	ret = rw_msg_send_key_add(&key_param, cfm);
	if (!ret && (cfm->status == CO_OK))
		BK_WIFI_LOGD(TAG, "add hw key idx=%d\r\n", cfm->hw_key_idx);
	else if (!ret)
		ret = BK_ERR_HITF_ADD_KEY;

	os_free(cfm);

	return ret;
}


extern int max_mm_sec_key_nbr(void);
int hapd_intf_del_key(struct prism2_hostapd_param *param, int len)
{
	u8 hw_key_idx = 0;

	if ((param->sta_addr == NULL) || is_broadcast_ether_addr(param->sta_addr))
		hw_key_idx = rwm_mgmt_get_hwkeyidx(param->vif_idx, 0xff);
	else {
		u8 staid = rwm_mgmt_sta_mac2idx(param->sta_addr);
		if (staid == 0xff)
			return BK_ERR_HITF_STA_NOT_EXIST;
		hw_key_idx = rwm_mgmt_get_hwkeyidx(param->vif_idx, staid);
	}

	if (hw_key_idx > max_mm_sec_key_nbr())
		return BK_ERR_HITF_HW_KEY_ID;

	BK_WIFI_LOGD(TAG, "del hw key idx=%d\r\n", hw_key_idx);

	return rw_msg_send_key_del(hw_key_idx);
}

int hapd_intf_add_vif(struct prism2_hostapd_param *param, int len)
{
	int ret;
	struct mm_add_if_cfm *cfm
		= (struct mm_add_if_cfm *)os_malloc(sizeof(struct mm_add_if_cfm));

	if (NULL == cfm) {
		BK_WIFI_LOGE(TAG, "vif add failed\r\n");
		return BK_ERR_NO_MEM;
	}

	ret = rw_msg_send_add_if((const unsigned char *)&param->sta_addr,
							 param->u.add_if.type, 0, cfm);

	if (ret || (cfm->status != CO_OK)) {
		BK_WIFI_LOGE(TAG, "add vif failed, %x!\r\n", cfm->status);
		os_free(cfm);
		return BK_ERR_HITF_ADD_VIF;
	}

	BK_WIFI_LOGD(TAG, "add vif%d, type=%d, status=%d\r\n",
			cfm->inst_nbr, param->u.add_if.type, cfm->status);

	if (cfm->inst_nbr >= NX_VIRT_DEV_MAX)
		return BK_ERR_HITF_VIF_ID;

	*(param->u.add_if.indx_ptr) = cfm->inst_nbr;

	os_free(cfm);

	return 0;
}

int hapd_intf_remove_vif(struct prism2_hostapd_param *param, int len)
{
	u8 vif_index = param->vif_idx;
	int ret = 0;

	if (vif_index >= NX_VIRT_DEV_MAX)
		return BK_ERR_HITF_VIF_ID;

	ret = rw_msg_send_remove_if(vif_index);
	BK_WIFI_LOGD(TAG, "del vif%d ret=%d\r\n", vif_index, ret);
	return ret;
}

int hapd_intf_start_apm(struct prism2_hostapd_param *param, int len)
{
	int ret;
	struct apm_start_cfm *cfm
		= (struct apm_start_cfm *)os_malloc(sizeof(struct apm_start_cfm));

	if (NULL == cfm) {
		BK_WIFI_LOGE(TAG, "start apm failed: oom\r\n");
		return BK_ERR_NO_MEM;
	}

	ret = rw_msg_send_apm_start_req(param->vif_idx, g_ap_param_ptr->chann, cfm);
	if (ret) {
		BK_WIFI_LOGE(TAG, "start apm failed, ret=%x!\r\n", ret);
		os_free(cfm);
		return BK_ERR_HITF_START_APM;
	}

	if (cfm->status == CO_OK) {
		BK_WIFI_LOGD(TAG, "start apm success, vif%d, channel%d, bcmc%d\r\n", cfm->vif_idx,
				cfm->ch_idx, cfm->bcmc_idx);
	}

	os_free(cfm);

	return 0;
}

int hapd_intf_stop_apm(struct prism2_hostapd_param *param, int len)
{
	int ret;
	ret = rw_msg_send_apm_stop_req(param->vif_idx);
	if (ret) {
		BK_WIFI_LOGE(TAG, "stop apm failed, %d!\r\n", ret);
		return BK_ERR_HITF_STOP_APM;
	}
	return 0;
}

int hapd_intf_set_ap_bcn(struct prism2_hostapd_param *param, int len)
{
	char *bcn_buf;
	BCN_PARAM_ST bcn_param;
	int ret;

	if (param->u.bcn_change.bcn_len == 0) {
		BK_WIFI_LOGE(TAG, "bcn len = 0\r\n");
		return BK_ERR_PARAM;
	}

	bcn_buf = (char *)os_malloc(param->u.bcn_change.bcn_len);
	if (!bcn_buf) {
		BK_WIFI_LOGW(TAG, "set bcn no buffer\r\n");
		return BK_ERR_NO_MEM;
	}

	os_memcpy(bcn_buf, param->u.bcn_change.beacon, param->u.bcn_change.bcn_len);
	os_memset(&bcn_param, 0, sizeof(BCN_PARAM_ST));

	bcn_param.bcn_ptr = (u32 *)bcn_buf;
	bcn_param.bcn_len = param->u.bcn_change.bcn_len;
	bcn_param.tim_len = param->u.bcn_change.tim_len;
	bcn_param.tim_oft = param->u.bcn_change.head_len;
	bcn_param.vif_idx = param->vif_idx;

	ret = rw_msg_send_bcn_change((void *)&bcn_param);
	os_free(bcn_buf);

	return ret;
}

void wpa_buffer_scan_results(void)
{
	s_scan_result_upload_ptr = sr_get_scan_results();
}

void wpa_clear_scan_results(void)
{
	s_scan_result_upload_ptr = NULL;
}

int wpa_send_scan_req(struct prism2_hostapd_param *param, int len)
{
	UINT8 i;
	int ret;
	SCAN_PARAM_T *scan_param;

	BK_WIFI_LOGD(TAG, "send scan req to driver\r\n");

	scan_param = (SCAN_PARAM_T *)(os_zalloc(sizeof(*scan_param) + param->u.scan_req.extra_ies_len));
	if (!scan_param) {
		BK_WIFI_LOGI(TAG, "send scan req OOM\n");
		return BK_ERR_NO_MEM;
	}
	scan_param->num_ssids = param->u.scan_req.ssids_num;
	os_memcpy(&scan_param->bssid, param->u.scan_req.bssid, ETH_ALEN);
	for (i = 0; i < scan_param->num_ssids; i++) {
		scan_param->ssids[i].length = param->u.scan_req.ssids[i].ssid_len;
		os_memcpy(scan_param->ssids[i].array, param->u.scan_req.ssids[i].ssid, scan_param->ssids[i].length);
	}
	scan_param->vif_idx = param->vif_idx;
	os_memcpy(scan_param->freqs, param->u.scan_req.freqs, sizeof(scan_param->freqs));
	os_memcpy(scan_param->extra_ies, param->u.scan_req.extra_ies, param->u.scan_req.extra_ies_len);
	scan_param->extra_ies_len = param->u.scan_req.extra_ies_len;

	ret = rw_msg_send_scanu_req(scan_param);
	if (!ret)
		wpa_ctrl_event(WPA_CTRL_EVENT_SCAN_STARTED, NULL);

	os_free(scan_param);

	return ret;
}

/* extract neccary ies for wpa_s */
int wlan_get_bss_beacon_ies(struct wpabuf *buf, const u8 *bcn_ie, int ie_len)
{
	const u8 *ie;
	const u8 *vsie;
	u8 ies[] = {WLAN_EID_SSID, WLAN_EID_RSN};

	buf->used = 0;

	// IEs
	for (int i = 0; i < ARRAY_SIZE(ies); i++) {
		ie = get_ie(bcn_ie, ie_len, ies[i]);
		if (ie) {
			if (wpabuf_tailroom(buf) >= ie[1] + 2)
				wpabuf_put_data(buf, ie, ie[1] + 2);
		}
	}

	// VS IE
	vsie = get_vendor_ie(bcn_ie, ie_len, WPA_IE_VENDOR_TYPE);
	if (vsie) {
		if (wpabuf_tailroom(buf) >= vsie[1] + 2)
			wpabuf_put_data(buf, vsie, vsie[1] + 2);
	}

	return 0;
}

int wpa_get_scan_rst(struct prism2_hostapd_param *param, int len)
{
	struct wpa_scan_results *results = param->u.scan_rst;
	#if CONFIG_MINIMUM_SCAN_RESULTS
	struct wpa_scan_res *scan_rst_ptr;
	#else
	struct sta_scan_res *scan_rst_ptr;
	struct wpa_scan_res *r;
	#endif
	int i, ret = 0;
	u32 ie_len;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	wpa_buffer_scan_results();

	if (NULL == s_scan_result_upload_ptr) {
		GLOBAL_INT_RESTORE();
		BK_WIFI_LOGW(TAG, "get scan result: empty\n");
		/* Don't fail get scan result, but allow empty scan result */
		return 0;
	}

	BK_WIFI_LOGI(TAG, "get scan result:%d\r\n", s_scan_result_upload_ptr->scanu_num);

	/* IE filtering is done at lower layer (rw_msg_rx.c) when CONFIG_MINIMIZE_SCAN_RESULT_IE is enabled */
	for (i = 0; s_scan_result_upload_ptr && i < s_scan_result_upload_ptr->scanu_num; i++) {
		scan_rst_ptr = s_scan_result_upload_ptr->res[i];
		ie_len = scan_rst_ptr->ie_len;

#if CONFIG_MINIMUM_SCAN_RESULTS
		scan_rst_ptr->freq = rw_ieee80211_get_centre_frequency(scan_rst_ptr->channel);
		scan_rst_ptr->ie_len = ie_len;
		results->res[results->num++] = scan_rst_ptr;

		if (bk_feature_bssid_connect_enable()) {
			if(os_memcmp(g_sta_param_ptr->fast_connect.bssid, scan_rst_ptr->bssid, ETH_ALEN) == 0) {
				BK_WIFI_LOGI(TAG, "match specified BSSID " MACSTR  "\r\n", MAC2STR(scan_rst_ptr->bssid));
				os_memset(g_sta_param_ptr->fast_connect.bssid, 0, ETH_ALEN);
			}
		}
#else
		r = os_zalloc(sizeof(*r) + ie_len);
		if (r == NULL) {
			uint32_t total_size,free_size,mini_size;
			#if CONFIG_MEM_DEBUG && CONFIG_FREERTOS
			os_dump_memory_stats(0, 0, NULL);
			#endif
			os_show_memory_config_info();

			total_size = rtos_get_total_heap_size();
			free_size  = rtos_get_free_heap_size();
			mini_size  = rtos_get_minimum_free_heap_size();
			BK_WIFI_LOGI(TAG, "%-5s   %-5s   %-5s   %-5s   %-5s\r\n","name", "total", "free", "minimum", "peak");
			BK_WIFI_LOGI(TAG, "heap\t%d\t%d\t%d\t%d\r\n",total_size,free_size,mini_size,total_size-mini_size);

			ret = BK_ERR_NO_MEM;
			BK_WIFI_LOGE(TAG, "get scan result fail: oom\n");
			break;
		}

		os_memcpy(r->bssid, scan_rst_ptr->bssid, ETH_ALEN);
		r->level = scan_rst_ptr->level;
		r->freq = rw_ieee80211_get_centre_frequency(scan_rst_ptr->channel);
		r->beacon_int = scan_rst_ptr->beacon_int;
		r->caps = scan_rst_ptr->caps;
		r->tsf = WPA_GET_BE64(scan_rst_ptr->tsf);
		r->ie_len = ie_len;
		os_memcpy(r + 1, scan_rst_ptr + 1, scan_rst_ptr->ie_len);

		results->res[results->num++] = r;

		if (bk_feature_bssid_connect_enable()) {
			if(os_memcmp(g_sta_param_ptr->fast_connect.bssid, r->bssid, ETH_ALEN) == 0) {
				BK_WIFI_LOGI(TAG, "match specified BSSID " MACSTR  "\r\n", MAC2STR(r->bssid));
				os_memset(g_sta_param_ptr->fast_connect.bssid, 0, ETH_ALEN);
			}
		}
#endif
	}

	GLOBAL_INT_RESTORE();

	#if !CONFIG_MINIMUM_SCAN_RESULTS
	/* doesn't need to keep, all info are stored in wpas */
	wpa_hostapd_release_scan_rst();
	#endif
	return ret;
}


#ifdef CONFIG_SME
int wpa_send_auth_req(struct prism2_hostapd_param *param, int len)
{
	AUTH_PARAM_T *auth_param;
	int ret = 0;

	auth_param = os_zalloc(sizeof(*auth_param) /*+ param->u.authen_req.ie_len*/ + param->u.authen_req.sae_data_len);
	if (!auth_param) {
		BK_WIFI_LOGE(TAG, "send auth req failed, oom\n");
		return BK_ERR_NO_MEM;
	}

	os_memcpy((UINT8 *)&auth_param->bssid, param->u.authen_req.bssid, ETH_ALEN);
	BK_WIFI_LOGI(TAG, "send auth req fail: %d, %d\n",
			param->u.authen_req.ie_len, param->u.authen_req.sae_data_len);

	auth_param->vif_idx = param->vif_idx;
	auth_param->ssid.length = param->u.authen_req.ssid_len;
	os_memcpy(auth_param->ssid.array, param->u.authen_req.ssid, auth_param->ssid.length);

	auth_param->ie_len = param->u.authen_req.ie_len;
	//auth_param->ie_buf = (uint8_t *)(auth_param + 1);
	if (auth_param->ie_len)
		os_memcpy((UINT8 *)auth_param->ie_buf, (UINT8 *)param->u.authen_req.ie, param->u.authen_req.ie_len);

	auth_param->sae_data_len = param->u.authen_req.sae_data_len;
	//auth_param->sae_data = auth_param->ie_buf + param->u.authen_req.sae_data_len;
	if (auth_param->sae_data_len)
		os_memcpy((UINT8 *)auth_param->sae_data, (UINT8 *)param->u.authen_req.sae_data, param->u.authen_req.sae_data_len);

	switch (param->u.authen_req.auth_alg) {
	case WPA_AUTH_ALG_OPEN:
		auth_param->auth_type = MAC_AUTH_ALGO_OPEN;
		break;
	case WPA_AUTH_ALG_SAE:
		auth_param->auth_type = MAC_AUTH_ALGO_SAE;
		break;
	case WPA_AUTH_ALG_SHARED:
		auth_param->auth_type = MAC_AUTH_ALGO_SHARED;
		break;
	}

	ret = rw_msg_send_sm_auth_req(auth_param);
	os_free(auth_param);

	return ret;
}

int wpa_send_assoc_req(struct prism2_hostapd_param *param, int len)
{
	ASSOC_PARAM_T *assoc_param;
	int ret = 0;

	assoc_param = os_zalloc(sizeof(*assoc_param) + param->u.assoc_req.bcn_len);
	if (!assoc_param) {
		BK_WIFI_LOGE(TAG, "send assoc req fail\n");
		return BK_ERR_NO_MEM;
	}

	os_memcpy((UINT8 *)&assoc_param->bssid, param->u.assoc_req.bssid, ETH_ALEN);
	assoc_param->flags = CONTROL_PORT_HOST;
	/* if group cipher is not wep, assume wpa2 in use */
	if (!(param->u.assoc_req.group_suite & (WPA_CIPHER_WEP40 | WPA_CIPHER_WEP104)))
		assoc_param->flags |= WPA_WPA2_IN_USE;

	if (param->u.assoc_req.mfp == MGMT_FRAME_PROTECTION_REQUIRED)
		assoc_param->flags |= MFP_IN_USE;

	assoc_param->vif_idx = param->vif_idx;
	assoc_param->ssid.length = param->u.assoc_req.ssid_len;
	os_memcpy(assoc_param->ssid.array, param->u.assoc_req.ssid, assoc_param->ssid.length);
	assoc_param->ie_len = param->u.assoc_req.ie_len;
	os_memcpy((UINT8 *)assoc_param->ie_buf, (UINT8 *)param->u.assoc_req.ie_buf, assoc_param->ie_len);
	assoc_param->bcn_len = param->u.assoc_req.bcn_len;
	if (assoc_param->bcn_len) {
		os_memcpy(assoc_param->bcn_buf, param->u.assoc_req.bcn_buf, assoc_param->bcn_len);
	}

	assoc_param->auth_type = param->u.assoc_req.auth_alg;
	ret = sa_station_send_associate_cmd(assoc_param);

	os_free(assoc_param);

	if (s_scan_result_upload_ptr) {
		sr_release_scan_results(s_scan_result_upload_ptr);
		s_scan_result_upload_ptr = NULL;
	}

	return ret;
}

#else /* !CONFIG_SME */

int wpa_send_assoc_req(struct prism2_hostapd_param *param, int len)
{
	CONNECT_PARAM_T *connect_param;
	int ret = 0;

	connect_param = os_zalloc(sizeof(*connect_param) + param->u.assoc_req.bcn_len);
	if (!connect_param)
		return BK_ERR_NO_MEM;

	os_memcpy((UINT8 *)&connect_param->bssid, param->u.assoc_req.bssid, ETH_ALEN);
#if defined(CONFIG_IEEE80211R) || defined(CONFIG_WNM)
	connect_param->flags = param->u.assoc_req.flags;
	connect_param->flags |= CONTROL_PORT_HOST;
#else
	connect_param->flags = CONTROL_PORT_HOST;
#endif
	if (param->u.assoc_req.proto & (WPA_PROTO_WPA | WPA_PROTO_RSN))
		connect_param->flags |= USE_PAIRWISE_KEY;

	if (param->u.assoc_req.pairwise_suite & (WPA_CIPHER_TKIP | WPA_CIPHER_WEP104 | WPA_CIPHER_WEP40))
		connect_param->flags |= DISABLE_HT;

	if (param->u.assoc_req.mfp == MGMT_FRAME_PROTECTION_REQUIRED)
		connect_param->flags |= MFP_IN_USE;

	if (param->u.assoc_req.group_suite != WPA_CIPHER_NONE)
		connect_param->flags |= USE_PRIVACY;

	if (rwnx_mod_params.amsdu_require_spp)
		connect_param->flags |= REQUIRE_SPP_AMSDU;

	connect_param->vif_idx = param->vif_idx;
	connect_param->ssid.length = param->u.assoc_req.ssid_len;
	os_memcpy(connect_param->ssid.array, param->u.assoc_req.ssid, connect_param->ssid.length);
	connect_param->ie_len = param->u.assoc_req.ie_len;
	os_memcpy((UINT8 *)connect_param->ie_buf, (UINT8 *)param->u.assoc_req.ie_buf, connect_param->ie_len);
	connect_param->bcn_len = param->u.assoc_req.bcn_len;
	if (connect_param->bcn_len)
		os_memcpy((UINT8 *)connect_param->bcn_buf, (UINT8 *)param->u.assoc_req.bcn_buf, connect_param->bcn_len);

#if CFG_WLAN_FAST_CONNECT_WITHOUT_SCAN
	connect_param->rssi = param->u.assoc_req.rssi;
	connect_param->cap_info = param->u.assoc_req.cap_info;
	connect_param->beacon_period = param->u.assoc_req.beacon_period;
#endif

	connect_param->auth_type = param->u.assoc_req.auth_alg;
#if CFG_WIFI_P2P || CFG_WLAN_FAST_CONNECT_WITHOUT_SCAN
	if (param->u.assoc_req.freq >= 5925) {
		connect_param->chan.band = IEEE80211_BAND_6GHZ;
	} else if (param->u.assoc_req.freq >= 4900) {
		connect_param->chan.band = IEEE80211_BAND_5GHZ;
	} else {
		connect_param->chan.band = IEEE80211_BAND_2GHZ;
	}
	connect_param->chan.freq = param->u.assoc_req.freq;
#endif
	ret = sa_station_send_associate_cmd(connect_param);

	os_free(connect_param);
	if (s_scan_result_upload_ptr) {
		sr_release_scan_results(s_scan_result_upload_ptr);
		s_scan_result_upload_ptr = NULL;
	}

	return ret;
}

#endif /* CONFIG_SME */

int wpa_send_disconnect_req(struct prism2_hostapd_param *param, int len)
{
	DISCONNECT_PARAM_T disconnect_param = {0};
	disconnect_param.vif_idx = param->vif_idx;
	disconnect_param.reason_code = param->u.disconnect_req.reason;

#if 0
	// supplicant will trigger state change, set mac status and post event there.
	wifi_linkstate_reason_t info;
#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
	FUNC_1PARAM_PTR fn;
	u32 val;

	fn = bk_wlan_get_status_cb();
	if (fn) {
		val = WIFI_LINKSTATE_STA_DISCONNECTED;
		(*fn)(&val);
	}
#endif
	info.state = WIFI_LINKSTATE_STA_DISCONNECTED;
	info.reason_code = WIFI_REASON_DISCONNECT_BY_APP;
	mhdr_set_station_status(info);

	wifi_event_sta_disconnected_t sta_disconnected = {0};
	sta_disconnected.disconnect_reason = info.reason_code;
	sta_disconnected.local_generated = true;
	BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_STA_DISCONNECTED,
					&sta_disconnected, sizeof(sta_disconnected), BEKEN_NEVER_TIMEOUT));
#endif

	return rw_msg_send_sm_disconnect_req(&disconnect_param);
}

#ifdef CONFIG_SME
int wpa_send_set_operate(struct prism2_hostapd_param *param, int len)
{
	SET_OPERATE_PARAM_T oper_param = {0};

	oper_param.vif_idx = param->vif_idx;
	oper_param.state = param->u.oper_state.state;

	return rw_msg_send_sm_set_operstate_req(&oper_param);
}
#endif

#ifdef CONFIG_SAE_EXTERNAL
int wpa_send_external_auth_status(struct prism2_hostapd_param *param, int len)
{
	EXTERNAL_AUTH_PARAM_T oper_param = {0};

	oper_param.vif_idx = param->vif_idx;
	oper_param.status = param->u.external_auth_status.status;

	return rw_msg_send_sm_external_auth_status(&oper_param);
}
#endif

int wpa_send_signal_monitor(struct prism2_hostapd_param *param, int len)
{
	SIGNAL_MONITOR_PARAM_T signal_monitor_param = {0};

	signal_monitor_param.vif_idx = param->vif_idx;
	signal_monitor_param.threshold = param->u.signal_monitor.threshold;
	signal_monitor_param.hysteresis = param->u.signal_monitor.hysteresis;

	return rw_msg_send_mm_signal_monitor(&signal_monitor_param);
}

int wpa_send_signal_poll(struct prism2_hostapd_param *param, int len)
{
	int ret = 0;
	SIGNAL_INFO_PARAM_T signal_info_param = {0};

	ret = rw_msg_get_signal_info(param->vif_idx, &signal_info_param);

	param->u.signal_info.frequency = signal_info_param.frequency;
	param->u.signal_info.current_signal = signal_info_param.current_signal;
	param->u.signal_info.avg_signal = signal_info_param.avg_signal;
	param->u.signal_info.current_noise = signal_info_param.current_noise;

	return ret;
}

#ifdef CONFIG_IEEE80211R
int wpa_update_ft_ies(struct prism2_hostapd_param *param, int len)
{
	UPDATE_FT_IES_PARAM_T ft_ies_params = {0};

	ft_ies_params.ft_ies_len = param->u.assoc_req.ie_len;
	os_memcpy((UINT8 *)ft_ies_params.ft_ies, (UINT8 *)param->u.assoc_req.ie_buf, param->u.assoc_req.ie_len);

	return rw_msg_update_ft_ies(&ft_ies_params);
}
#endif

int wpa_get_channel_info(struct prism2_hostapd_param *param, int len)
{
	 int ret = 0;
	 PHY_CHAN_INFO_T channel_info = {0};

	 ret = rw_msg_get_channel_info(param->vif_idx, &channel_info);

	 param->u.channel_info.seg1_idx = channel_info.seg1_idx;
	 param->u.channel_info.chanwidth = channel_info.chanwidth;
	 param->u.channel_info.frequency = channel_info.frequency;
	 param->u.channel_info.sec_channel = channel_info.sec_channel;
	 param->u.channel_info.center_frq1 = channel_info.center_frq1;
	 param->u.channel_info.center_frq2 = channel_info.center_frq2;

	 return ret;
}

int wpa_get_bss_info(struct prism2_hostapd_param *param, int len)
{
	int ret, ssid_len;
	struct me_get_bss_info_cfm *cfm;

	cfm = os_malloc(sizeof(struct me_get_bss_info_cfm));

	if (NULL == cfm) {
		BK_WIFI_LOGE(TAG, "get bss info fail\r\n");
		return BK_ERR_NO_MEM;
	}

	ret = rw_msg_get_bss_info(param->vif_idx, (void *)cfm);
	if (ret)
		return -1;

	os_memcpy(param->u.bss_info.bssid, cfm->bssid, ETH_ALEN);
	ssid_len = MIN(SSID_MAX_LEN, os_strlen((char *)cfm->ssid));
	os_memcpy(param->u.bss_info.ssid, cfm->ssid, ssid_len);
	os_free(cfm);

	return 0;
}
extern UINT8 rwm_mgmt_sta_mac2port(void *mac);
extern uint32_t vif_mgmt_get_first_ap_index(void);

uint32_t wpa_hostapd_no_password_connected(const uint8_t *addr)
{
	uint32_t sta_id;
	uint32_t ap_index;
	uint32_t connected_flag = 0;

	if (!cfg_ap_is_open_system())
		goto is_connected_exit;

	BK_WIFI_LOGD(TAG, "no password connected\r\n");
	ap_index = vif_mgmt_get_first_ap_index();
	if (INVALID_VIF_IDX == ap_index)
		goto is_connected_exit;

	BK_WIFI_LOGD(TAG, "ap_index:%d\r\n", ap_index);

	sta_id = sta_mgmt_get_staid(ap_index, addr);
	BK_WIFI_LOGD(TAG, "sta_id:%d\r\n", sta_id);
	if (INVALID_STA_IDX != sta_id)
		connected_flag = 1;

is_connected_exit:
	return connected_flag;
}

int wpa_hostapd_set_sta_flag(struct prism2_hostapd_param *param, int len)
{
	u32 set_flag = param->u.set_flags_sta.flags_or;
	u32 mask = param->u.set_flags_sta.flags_and;
	u32 flag = 0;
	bool opened = 0;
	u8 sta_idx;

	flag |= set_flag;
	flag &= mask;

	if (!flag)
		return 0;

	if (!(flag & WPA_STA_AUTHENTICATED))
		return 0;

	// Port already open
	if (rwm_mgmt_sta_mac2port(param->sta_addr)) {
		BK_WIFI_LOGD(TAG, "STA " MACSTR "already opened\n", MAC2STR(param->sta_addr));
		return 0;
	}

	sta_idx = rwm_mgmt_sta_mac2idx(param->sta_addr);
	if (sta_idx == 0xff)
		return 0;

	opened = 1;

	return rw_msg_me_set_control_port_req(opened, sta_idx);
}


void wpa_hostapd_release_scan_rst(void)
{
	SCAN_RST_UPLOAD_T *rst;

	if (s_scan_result_upload_ptr) {
		sr_release_scan_results(s_scan_result_upload_ptr);
		s_scan_result_upload_ptr = NULL;
	} else {
		rst = sr_get_scan_results();
		sr_release_scan_results(rst);
	}
}

int hapd_poll_null_frame(struct prism2_hostapd_param *param, int len)
{
	uint32_t sta_idx;
	struct apm_probe_client_cfm cfm;
	void *sta;
	void *vif;

	sta_idx = sta_mgmt_get_sta_id(param->u.poll_null_data.sta_addr);
	if (INVALID_STA_IDX == sta_idx)
		return 1;

	sta = sta_mgmt_get_entry(sta_idx);
	if (sta) {
		vif = mac_vif_mgmt_get_entry(mac_sta_mgmt_get_inst_nbr(sta));
		if (vif) {
			rw_send_apm_probe_req(mac_vif_mgmt_get_index(vif), sta_idx, &cfm);
			return cfm.status;
		}
	}
	return -1;
}

int hapd_intf_ioctl(unsigned long arg)
{
	u32 cmd = 0;
	int len, ret = BK_OK;
	struct iwreq *iwr_ptr;
	struct prism2_hostapd_param *param;
	BOOL display_err = true;

	if (0 == arg) {
		ret = BK_ERR_PARAM;
		goto ioctl_exit;
	}

	iwr_ptr = (struct iwreq *)arg;
	len = iwr_ptr->u.data.length;
	param = iwr_ptr->u.data.pointer;

	cmd = param->cmd;
	switch (cmd) {
	case PRISM2_HOSTAPD_POLL_CLIENT_NULL_DATA:
		ret = hapd_poll_null_frame(param, len);
		break;

	case PRISM2_HOSTAPD_CHANNEL_SWITCH:
		ret = wpa_intf_channel_switch(param, len);
		break;

	case PRISM2_HOSTAPD_WPA_INIT_VIF:
		ret = hapd_intf_add_vif(param, len);
		break;

	case PRISM2_HOSTAPD_SET_FLAGS_STA:
		ret = wpa_hostapd_set_sta_flag(param, len);
		break;

	case PRISM2_HOSTAPD_START_APM:
		ret = hapd_intf_start_apm(param, len);
		break;

	case PRISM2_HOSTAPD_STOP_APM:
		ret = hapd_intf_stop_apm(param, len);
		break;

	case PRISM2_HOSTAPD_SET_AP_BCN:
		ret = hapd_intf_set_ap_bcn(param, len);
		break;

	case PRISM2_SET_ENCRYPTION:
		if (os_strcmp((const char *)param->u.crypt.alg, "NONE") == 0) {
			ret = hapd_intf_del_key(param, len);
			display_err = false;

		} else
			ret = hapd_intf_add_key(param, len);
		break;

	case PRISM2_GET_ENCRYPTION:
		break;

	case PRISM2_HOSTAPD_FLUSH:
		break;

	case PRISM2_HOSTAPD_ADD_STA:
		ret = hapd_intf_sta_add(param, len);
		break;

	case PRISM2_HOSTAPD_REMOVE_STA:
		ret = hapd_intf_sta_del(param, len);
		break;

	case PRISM2_HOSTAPD_GET_INFO_STA:
		ret = hapd_get_sta_info(param, len);
		break;

	case PRISM2_HOSTAPD_STA_CLEAR_STATS:
		break;

	case PRISM2_HOSTAPD_SET_GENERIC_ELEMENT:
		break;

	case PRISM2_HOSTAPD_SCAN_REQ:
		ret = wpa_send_scan_req(param, len);
		break;

	case PRISM2_HOSTAPD_GET_SCAN_RESULT:
		ret = wpa_get_scan_rst(param, len);
		break;

#ifdef CONFIG_SME
	case PRISM2_HOSTAPD_AUTHENTICATE:
		ret = wpa_send_auth_req(param, len);
		break;

	case PRISM2_HOSTAPD_SET_OPER_STATE:
		ret = wpa_send_set_operate(param, len);
		break;
#endif

	case PRISM2_HOSTAPD_ASSOC_REQ:
		ret = wpa_send_assoc_req(param, len);
		break;

	case PRISM2_HOSTAPD_ASSOC_ACK:
		break;

	case PRISM2_HOSTAPD_REG_APP_START:
		break;

	case PRISM2_HOSTAPD_DISCONN_REQ:
		ret = wpa_send_disconnect_req(param, len);
		break;

	case PRISM2_HOSTAPD_WPA_DEINIT_VIF:
		ret = hapd_intf_remove_vif(param, len);
		break;

	case PRISM2_HOSTAPD_GET_BSS_INFO:
		ret = wpa_get_bss_info(param, len);
		break;

#ifdef CONFIG_OCV
	case PRISM2_HOSTAPD_GET_CHANNEL_REQ:
		ret = wpa_get_channel_info(param, len);
		break;
#endif

#ifdef CONFIG_SAE_EXTERNAL
	case PRISM2_HOSTAPD_EXTERNAL_AUTH_STATUS:
		ret = wpa_send_external_auth_status(param, len);
		break;
#endif

	case PRISM2_HOSTAPD_SIGNAL_MONITOR:
		ret = wpa_send_signal_monitor(param, len);
		break;

	case PRISM2_HOSTAPD_GET_SIGNAL_POLL:
		ret = wpa_send_signal_poll(param, len);
		break;

#ifdef CONFIG_IEEE80211R
	case PRISM2_HOSTAPD_UPDATE_FT_IES:
		ret = wpa_update_ft_ies(param, len);
		break;
#endif

#if CONFIG_P2P
	case PRISM2_HOSTAPD_ROC:
		ret = rw_msg_send_roc(param->vif_idx, param->u.roc.freq,
			param->u.roc.duration);

		break;

	case PRISM2_HOSTAPD_CANCEL_ROC: {
		ret = rw_msg_send_cancel_roc(param->vif_idx);
	}	break;
#endif

	default:
		ret = BK_ERR_HITF_COMMAND;
		break;
	}

ioctl_exit:
	if ((ret != BK_OK) && display_err)
		BK_WIFI_LOGE(TAG, "ioctl fail, cmd=%d ret=%d\n", cmd, ret);
	return ret;
}

/* Transmit mgmt frame callback */
static void hapd_intf_mgmt_tx_cb(void *arg, bool acked)
{
	struct sk_buff *skb = arg;
	uint8_t *mpdu = skb->msdu_ptr;
	struct ieee80211_mgmt *hdr = (struct ieee80211_mgmt *)mpdu;

	hdr->frame_control &= ~MAC_FCTRL_PROTOCOLVERSION_MASK;

	/* protocol version 2 is reserved for indicating ACKed frame (TX
	 * callbacks), and version 1 for indicating failed frame (no ACK, TX
	 * callbacks) */
	if (acked)
		hdr->frame_control |= 2;
	else
		hdr->frame_control |= 1;

	/* send to hostapd / wpa supplicant */
	ke_mgmt_packet_tx(mpdu, skb->len, skb->vif_idx);
}

int hapd_intf_ke_rx_handle(int dummy)
{
	int payload_size;
	S_TYPE_PTR type_ptr = (S_TYPE_PTR)dummy;

	if (type_ptr->type == HOSTAPD_MGMT || type_ptr->type == HOSTAPD_MGMT_ROBUST) {
#if !CONFIG_RWNX_SW_TXQ
		MSDU_NODE_T *node;

		// get the payload size that want to send to umac
		payload_size = ke_mgmt_peek_rxed_next_payload_size(type_ptr->vif_index);
		if (!payload_size) {
			BK_WIFI_LOGE(TAG, "payload_size is 0\r\n");
			goto exit;
		}

		// allocate memory for tx
		node = rwm_tx_mgmt_node_alloc(payload_size, PBUF_RAW);
		if (!node) {
			BK_WIFI_LOGE(TAG, "node alloc fail, %d\r\n", payload_size);
			goto exit;
		}

		// TODO: BK7236, avoid memcpy. copy payload from socket to MSDU
		ke_mgmt_packet_rx(rwm_get_mpdu_content_ptr(node), payload_size, type_ptr->vif_index);

		node->sta_idx = 0xFF;  // FIXME BK7236
		node->vif_idx = type_ptr->vif_index;
		node->cb = hapd_intf_mgmt_tx_cb;
		node->args = node;
		rwm_transfer_mgmt_node(node);
#else /* CONFIG_RWNX_SW_TXQ */
		struct sk_buff *skb;

		// get the payload size that want to send to umac
		payload_size = ke_mgmt_peek_rxed_next_payload_size(type_ptr->vif_index);
		if (!payload_size) {
			BK_WIFI_LOGE(TAG, "payload_size is 0\r\n");
			goto exit;
		}

		// allocate memory for tx
		skb = alloc_skb_for_tx_mgmt(payload_size);
		if (!skb) {
			BK_WIFI_LOGE(TAG, "skb malloc fail, %d\r\n", payload_size);
			goto exit;
		}

		// TODO: BK7236, avoid memcpy. copy payload from socket to MSDU
		ke_mgmt_packet_rx(skb->msdu_ptr, payload_size, type_ptr->vif_index);

		skb->sta_idx = 0xFF;  // FIXME BK7236
		skb->vif_idx = type_ptr->vif_index;
		skb->cb = hapd_intf_mgmt_tx_cb;
		skb->args = skb; // cb args

		// start to send the skb
		extern void rwnx_start_xmit_mgmt(struct sk_buff *skb);
		rwnx_start_xmit_mgmt(skb);
#endif /* CONFIG_RWNX_SW_TXQ */
	} else if ((type_ptr->type == HOSTAPD_DATA) || (type_ptr->type == HOSTAPD_DATA_SPECIAL)) {
		struct pbuf *p;

		payload_size = ke_data_peek_rxed_next_payload_size(type_ptr->vif_index);
		#if BK_SS_WIFI_DP
			p = bk_wlan_buf_alloc(payload_size); 
		#else
			p = pbuf_alloc(PBUF_RAW_TX, payload_size, PBUF_RAM);
		#endif
		if (!p) {
			BK_WIFI_LOGE(TAG, "hapd_intf_tx_error\r\n");
			goto exit;
		}

		ke_l2_packet_rx(p->payload, payload_size, type_ptr->vif_index);

#if CONFIG_RWNX_SW_TXQ
		BUS_MSG_T msg = {0};
		msg.arg = (uint32_t)p;
		msg.len = type_ptr->vif_index;
		msg.sema = NULL;
		msg.cb = type_ptr->cb;
		msg.param = type_ptr->args;

		if(type_ptr->type == HOSTAPD_DATA) {
		    // send to core thread
			msg.type = BMSG_TX_TYPE;
		}
		else if (type_ptr->type == HOSTAPD_DATA_SPECIAL) {
			msg.type = BMSG_SPECIAL_TX_TYPE;
		}

		extern void bmsg_tx_handler(BUS_MSG_T *msg);
		bmsg_tx_handler(&msg);

		#if CONFIG_RWNX_SW_TXQ
		rwnx_hwq_process_all(0);
		#endif
#else
		rwm_transfer(type_ptr->vif_index, p, p->payload, payload_size, type_ptr->cb, type_ptr->args
		#if CONFIG_SPECIAL_TX_TYPE
		,0
		#endif
		);
#endif
	}

	os_free(type_ptr);
	return kNoErr;

exit:
	return kNoMemoryErr;
}
// eof

