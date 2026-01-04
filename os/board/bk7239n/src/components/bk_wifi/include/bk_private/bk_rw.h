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

#pragma once

#include <common/sys_config.h>
#include <os/os.h>
#include <bk_compiler.h>
#include "fhost_msg.h"
#include "fhost_mac.h"
#include "bk_wifi.h"
//#include "apm_task.h"
//#include "me_task.h"
//#include "mm_task.h"
//#include "mm.h"
//#include "twt_task.h"
#include "lwip/pbuf.h"
//#include "vif_mgmt.h"
//#include "rwnx_version.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cfg80211_connect_params {
	uint32_t flags;
	uint32_t vif_idx;
	uint8_t auth_type;
	struct mac_addr bssid;
	struct mac_ssid ssid;
	struct mac_chan_def chan;
	uint16_t ie_len;
	uint32_t ie_buf[64];
	uint16_t bcn_len;
	uint32_t bcn_buf[0];
} CONNECT_PARAM_T;

typedef struct cfg80211_auth_params {
	uint32_t vif_idx;
	uint8_t auth_type;
	struct mac_addr bssid;
	struct mac_ssid ssid;
	struct mac_chan_def chan;
	uint16_t ie_len;
	uint8_t ie_buf[128];
	uint16_t sae_data_len;
	uint8_t sae_data[0];
} AUTH_PARAM_T;

typedef struct cfg80211_external_auth_params {
	uint32_t vif_idx;
	uint16_t status;
} EXTERNAL_AUTH_PARAM_T;

typedef struct cfg80211_signal_monitor {
	uint32_t vif_idx;
	int threshold;
	int hysteresis;
} SIGNAL_MONITOR_PARAM_T;

typedef struct cfg80211_signal_info {
	u32 frequency;
	int current_signal;
	int avg_signal;
	int current_noise;
} SIGNAL_INFO_PARAM_T;

typedef struct cfg80211_update_ft_ies_params {
	u32 ft_ies[64];
	size_t ft_ies_len;
} UPDATE_FT_IES_PARAM_T;

typedef struct cfg80211_associate_params {
	uint32_t flags;
	uint32_t vif_idx;
	uint8_t auth_type;
	struct mac_addr bssid;
	struct mac_ssid ssid;
	struct mac_chan_def chan;
	uint16_t ie_len;
	uint32_t ie_buf[64];
	uint16_t bcn_len;
	uint32_t bcn_buf[0];
} ASSOC_PARAM_T;

typedef struct cfg80211_scan_params {
	uint8_t num_ssids;
	uint8_t vif_idx;
	struct mac_ssid ssids[SCAN_SSID_MAX];
	struct mac_addr bssid;
	int freqs[14 + 28];	// MAC_DOMAINCHANNEL_24G_MAX + MAC_DOMAINCHANNEL_5G_MAX, FIXME: BK7239 6E
	uint16_t extra_ies_len;
	uint8_t extra_ies[0];
} SCAN_PARAM_T;

typedef struct cfg80211_fast_scan_params {
	struct mac_ssid ssid;
	struct mac_addr bssid;

	uint16_t probe_delay;
	uint16_t min_ch_time;
	uint16_t max_ch_time;
	uint16_t ch_num;
} FAST_SCAN_PARAM_T;

typedef struct cfg80211_disconnect_params {
	uint16_t reason_code;
	uint8_t vif_idx;
} DISCONNECT_PARAM_T;

typedef struct cfg80211_set_operate_params {
	uint32_t flags;
	uint32_t vif_idx;
	int state;
} SET_OPERATE_PARAM_T;

typedef struct {
	char ssid[MAC_SSID_LEN];
	char ap_power;
} AP_INFO_T;

typedef  struct {
	uint16_t ap_num;
	uint16_t ap_max;

	AP_INFO_T *ap_list;
} SCAN_RESULT_SET;

typedef struct {
	FUNC_2PARAM_PTR cb;
	void *ctxt_arg;
} IND_CALLBACK_T;

typedef struct {
	int8_t rssi;
	uint8_t ssid[32];
	uint8_t bssid[6];
	uint16_t freq;
} BSS_INFO_T;

typedef struct {
	u32 frequency;
	int chanwidth;
	int sec_channel;
	int center_frq1;
	int center_frq2;
	u8 seg1_idx;
} PHY_CHAN_INFO_T;

enum nl80211_iftype {
	NL80211_IFTYPE_UNSPECIFIED,
	NL80211_IFTYPE_ADHOC,
	NL80211_IFTYPE_STATION,
	NL80211_IFTYPE_AP,
	NL80211_IFTYPE_AP_VLAN,
	NL80211_IFTYPE_WDS,
	NL80211_IFTYPE_MONITOR,
	NL80211_IFTYPE_MESH_POINT,
	NL80211_IFTYPE_P2P_CLIENT,
	NL80211_IFTYPE_P2P_GO,
	NL80211_IFTYPE_P2P_DEVICE,
	NL80211_IFTYPE_OCB,

	/* keep last */
	NUM_NL80211_IFTYPES,
	NL80211_IFTYPE_MAX = NUM_NL80211_IFTYPES - 1
};

typedef int (*rw_event_handler)(wifi_linkstate_reason_t info, void *data);

struct rw_evt_payload {
	uint8_t mac[6];
};

struct add_sta_st {
	u16 aid;
	u16 capability;
	void *sta_addr;
	u8 ap_vif_idx;
	u8 *supp_rates;
	size_t supp_rates_len;
	u32 flags;
} ;

typedef struct cfg80211_key_params {
	uint8_t cipher_suite;
	uint8_t sta_idx;
	uint8_t inst_nbr;
	uint8_t key_idx;
	uint8_t hw_key_idx;
	struct mac_sec_key key;
} KEY_PARAM_T;

typedef struct msg_send_node {
	struct co_list_hdr hdr;
	struct ke_msg *msg;
	void *cfm;
	void *semaphore;
	uint16_t reqid;
} MSG_SND_NODE_ST, *MSG_SND_NODE_PTR;

typedef struct bcn_param_st {
	u32 *bcn_ptr;
	u16 bcn_len;
	u16 tim_oft;

	u8 tim_len;
	u8 vif_idx;
	u16 flag;

	u8 csa_oft[BCN_MAX_CSA_CPT];
} BCN_PARAM_ST, *BCN_PARAM_PTR;

typedef void *VIF_INF_PTR;
typedef void *STA_INF_PTR;

extern struct co_list rw_msg_tx_head;
extern struct co_list rw_msg_rx_head;

#if CONFIG_AP_PROBEREQ_CB
typedef void (*apvif_probereq_frame_t)(unsigned char *data, unsigned int length);
#endif
struct scan_chan_duration_item {
	unsigned char ch_nb;
	unsigned int duration;   ///uS
};

struct scan_chan_duration_tag {
	unsigned char table_nm;
	struct scan_chan_duration_item *tb;
};

struct scan_cfg_scan_param_tag{
	u8 set_param;     /**< 1:indicates set scan param;0:not set scan para;*/
	u8 scan_type;     /**< passive scan:1, active scan:0*/
	u8 chan_cnt;     /**< scan channel cnt*/
	u8 chan_nb[WIFI_2BAND_MAX_CHAN_NUM];     /**< scan channel number 2.4g+5g*/
	u8 scan_band;     /**< scan band, 2.4G :1, 5G :2*/
	u32 duration;     /**< scan duration,us*/
};

typedef struct scanu_rst_upload {
	UINT16 ref;
	UINT16 scanu_num;
#ifdef CONFIG_MINIMUM_SCAN_RESULTS
	struct wpa_scan_res **res;
#else
	struct sta_scan_res **res;
#endif
} SCAN_RST_UPLOAD_T, *SCAN_RST_UPLOAD_PTR;

#ifdef CONFIG_IEEE80211R
extern int rw_msg_update_ft_ies(UPDATE_FT_IES_PARAM_T *ft_ies);
#endif

extern int rw_msg_get_signal_info(u8 vif_idx, SIGNAL_INFO_PARAM_T *signal_info_param);
extern int rw_msg_send_mm_signal_monitor(SIGNAL_MONITOR_PARAM_T *signal_monitor_param);
extern void mt_msg_dispatch(UINT16 cmd, void *param);
extern void mr_kmsg_flush(void);
extern UINT32 mr_kmsg_fwd(struct ke_msg *msg);
extern UINT32 mr_kmsg_fuzzy_handle(void);
extern UINT32 mr_kmsg_exact_handle(UINT16 rsp);
extern void mhdr_assoc_cfm_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_auth_cfm_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_scanu_reg_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_connect_user_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern UINT32 rw_ieee80211_init(void);
extern UINT32 rw_ieee80211_get_centre_frequency(UINT32 chan_id);
extern UINT8 rw_ieee80211_get_chan_id(UINT32 freq);
extern bool rw_ieee80211_5g_channel_supported(uint8_t chan);
extern void *sr_get_scan_results(void);
extern void sr_release_scan_results(SCAN_RST_UPLOAD_PTR ptr);
extern void *rwm_transfer_pre(UINT8 vif_idx, UINT8 *buf, UINT32 len);
extern UINT32 rwm_uploaded_data_handle(UINT8 *upper_buf, UINT32 len);
extern UINT32 rwm_get_rx_valid_node_len(void);
extern int rw_msg_send(const void *msg_params, int reqcfm, uint16_t reqid, void *cfm);
extern int rw_msg_send_reset(void);
extern int rw_msg_send_start(void);
extern int rw_msg_send_me_config_req(void);
extern int rw_msg_send_me_chan_config_req(void);
extern int rw_msg_send_add_if(const unsigned char *mac,
		enum nl80211_iftype iftype, bool p2p, struct mm_add_if_cfm *cfm);
extern int rw_msg_send_remove_if(u8 vif_index);
extern int rw_msg_send_config_monitor_req(struct mac_chan_op *chan,
		struct me_config_monitor_cfm *cfm);
extern int rwnx_monitor_open(void);
extern int rwnx_monitor_close(void);
extern int rwnx_msg_send_twt_setup(uint8_t setup_type, uint16_t mantissa, uint8_t min_twt);
extern int rwnx_msg_send_twt_teardown(void);
extern void rwnx_udpate_capability(uint32_t capa_id, uint32_t capa_val);
extern UINT32 rwm_transfer(UINT8 vif_idx, struct pbuf *p, UINT8 *buf, UINT32 len, void *cb, void *arg
#if CONFIG_SPECIAL_TX_TYPE
		 , bool is_special_tx
#endif
);
extern int rw_msg_send_twt_setup(uint8_t setup_type, uint8_t vif_idx, struct twt_conf_tag *twt_conf, struct twt_setup_cfm *cfm);
extern int rw_send_me_rc_set_rate(u8 sta_idx, u16 rate_cfg);
extern int rw_msg_send_twt_teardown(uint8_t vif_idx, uint8_t flow_id);
extern int rw_msg_get_machw_mib_req(uint32_t mib_bit, void *cfm);
extern int rw_send_apm_probe_req(uint8_t vif_index, uint8_t sta_idx, struct apm_probe_client_cfm *cfm);
extern int rw_send_me_traffic_ind(u8 sta_idx, bool uapsd, u8 tx_status);
extern int rw_msg_send_roc(u8 vif_index, unsigned int freq, uint32_t duration);
extern int rw_msg_send_cancel_roc(u8 vif_index);
extern int rw_msg_send_apm_start_req(u8 vif_index, u8 channel, struct apm_start_cfm *cfm);
extern int rw_msg_apm_send_null_frame(uint8_t sta_id, uint8_t vif_idx);
extern int rw_msg_send_bcn_change(void *bcn_param);
extern int rw_msg_send_me_sta_add(struct add_sta_st *param, struct me_sta_add_cfm *cfm);
extern int rw_msg_send_me_sta_del(u8 sta_idx, bool tdls_sta);
extern int rw_msg_me_set_control_port_req(bool opened, u8 sta_idx);
extern int rw_msg_send_key_add(KEY_PARAM_T *param, struct mm_key_add_cfm *cfm);
extern int rw_msg_send_key_del(u8 hw_key_idx);
extern int rw_msg_send_scanu_req(SCAN_PARAM_T *scan_param);
extern int rw_msg_send_connection_loss_ind(u8 vif_index);
extern int rw_msg_get_bss_info(u8 vif_idx, void *cfm);
extern int rw_msg_get_channel_info(u8 vif_idx, PHY_CHAN_INFO_T *channel_info);
extern int rw_msg_dhcp_done_ind(u8 vif_idx);
extern int rw_msg_send_listen_interval_req(uint8_t sta_idx, uint8_t interval);
extern int rw_msg_send_arp_set_rate_req(uint8_t vif_idx, uint16_t arp_tx_rate);
extern int rw_msg_get_listen_interval_req(void *cfm);
extern int rw_msg_send_bcn_loss_int_req(uint8_t vif_idx, uint8_t interval,uint8_t repeat_num);
extern int rw_msg_send_bcn_loss_time_req(uint8_t vif_idx, uint8_t wait_cnt,uint8_t wake_cnt);
extern int rw_msg_send_bcn_recv_win_req(uint8_t vif_idx, uint8_t default_win,uint8_t max_win, uint8_t step);
extern int rw_msg_send_bcn_miss_time_req(uint8_t vif_idx, uint8_t bcnmiss_time);
extern int rw_msg_send_conn_delay_time_ind(uint8_t vif_idx, uint8_t conn_delay_time);
extern void rw_msg_send_psdebug_req(uint8_t index);
extern int rw_msg_send_td_interval_req(uint8_t interval, uint8_t max_pnk);
extern int rw_msg_send_td_mask_req(void);
extern int rw_msg_send_mm_mqtt_info(bool mqtt_connect, bool is_send_pingreq, uint32_t period_s);
extern int rw_msg_get_channel(void *cfm);
extern int rw_msg_set_filter(uint32_t filter);
extern int rw_msg_set_app_filter(uint32_t filter);
extern int rw_msg_set_channel(uint32_t channel, uint32_t band_width, void *cfm);
extern int rw_msg_send_scan_cancel_req(void *cfm);
extern int rw_msg_send_sm_disconnect_req(DISCONNECT_PARAM_T *param);
extern int rw_msg_send_sm_connect_req(CONNECT_PARAM_T *sme, void *cfm);
extern int rw_msg_send_sm_assoc_req(ASSOC_PARAM_T *sme, void *cfm);
extern int rw_msg_send_tim_update(u8 vif_idx, u16 aid, u8 tx_status);
extern int rw_msg_send_apm_stop_req(u8 vif_index);
extern int rw_msg_set_power(u8 vif_idx, u8 power);
extern int rw_msg_send_sm_auth_req(AUTH_PARAM_T *auth_param);
extern int rw_msg_send_sm_external_auth_status(EXTERNAL_AUTH_PARAM_T *auth_param);
extern int rw_msg_send_sm_set_operstate_req(SET_OPERATE_PARAM_T *param);
extern int rw_msg_send_version_req(void);
#if CONFIG_WIFI_FTM
int rw_msg_send_ftm_start_req(uint8_t vif_idx, uint8_t ftm_per_burst, uint8_t nb_ftm_rsp, void *ind);
#endif
#if CONFIG_WIFI_CSI_EN
extern int rw_msg_send_csi_active_mode_req(wifi_csi_active_mode_config_t *config,struct csi_active_mode_cfm *cfm);
extern int rw_msg_send_csi_receive_mode_req(wifi_csi_receive_mode_config_t *config,struct csi_receive_mode_cfm *cfm);
#endif
extern int rw_msg_send_recycle_txlist_req(uint8_t sta_idx);
extern int rwnx_handle_dynparams(void);

VIF_INF_PTR rwm_mgmt_vif_idx2ptr(UINT8 vif_idx);
VIF_INF_PTR rwm_mgmt_vif_type2ptr(UINT8 vif_type);
STA_INF_PTR rwm_mgmt_sta_idx2ptr(UINT8 staid);
STA_INF_PTR rwm_mgmt_sta_mac2ptr(void *mac);
#if CONFIG_RWNX_PROTO_DEBUG
UINT8 rwm_set_proto_debug(uint8_t flag);
#endif
UINT8 rwm_mgmt_sta_mac2idx(void *mac);
UINT8 rwm_mgmt_vif_mac2idx(void *mac);
UINT8 rwm_mgmt_vif_name2idx(char *name);
UINT8 rwm_mgmt_get_hwkeyidx(UINT8 vif_idx, UINT8 staid);
#if CONFIG_NO_HOSTED
struct netif *rwm_mgmt_get_vif2netif(UINT8 vif_idx);
UINT8 rwm_mgmt_get_netif2vif(struct netif *netif);
#endif
UINT8 rwm_mgmt_tx_get_staidx(UINT8 vif_idx, void *dstmac);
UINT8 rwm_get_monitor_vif_idx(void);
UINT8 rwm_first_vif_idx(void);
u8 rwn_mgmt_is_only_sta_role_add(void);
void rwm_msdu_init(void);
void rwm_flush_txing_list(UINT8 sta_idx);
void rwm_msdu_ps_change_ind_handler(void *msg) ;
void rwm_p2p_ps_change_ind_handler(void *msg);
void rwm_msdu_send_txing_node(UINT8 sta_idx);
//void rwm_msdu_twt_ps_change_ind_handler(void *msg);

void rwm_msdu_twt_ps_change_ind_handler(void *msg);
void rwm_get_mac_trx_status(bool reset_status);
void rwm_test_hw_diag(void);
void rwm_set_hw_diag_cfg(uint8_t diag_type, uint16_t diag_no);

__INLINE void *rwm_mgmt_is_vif_first_used(void)
{
	return mac_vif_mgmt_first_vif();
}

__INLINE void *rwm_mgmt_next(VIF_INF_PTR vif_entry)
{
	return mac_vif_mgmt_next_vif(vif_entry);
}

extern UINT8 beacon[149];

UINT8 rw_ieee80211_init_scan_chan(struct scanu_start_req *req);
UINT8 rw_ieee80211_init_scan_chan_band(struct scanu_start_req *req, UINT8 band);
UINT8 rw_ieee80211_is_scan_rst_in_countrycode(UINT8 freq);
#if CONFIG_WIFI4
void rw_ieee80211_set_ht_cap(UINT8 ht_supp);
#endif

#if CONFIG_AP_PROBEREQ_CB
extern void ap_vif_probe_req_frame_cb_register(apvif_probereq_frame_t cb);
#endif

int rw_ieee80211_set_country(const wifi_country_t *country);
int rw_ieee80211_get_country(wifi_country_t *country);

FUNC_1PARAM_PTR bk_wifi_get_ap_power_cb(void);
#ifdef __cplusplus
}
#endif

