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

#include <common/bk_typedef.h>
#include "../wpa_supplicant-2.10/src/common/wpa_ctrl.h"
//#include <os/mem.h>
//#include <os/str.h>
#include "../wpa_supplicant-2.10/src/common/defs.h"
#include "../wpa_supplicant-2.10/bk_patch/wpa_err.h"
#include "../wpa_supplicant-2.10/src/utils/common.h"
#include "../wpa_supplicant-2.10/src/utils/wpa_debug.h"
#include "bk_private/bk_wifi.h"
#include "bk_rw.h"
#include "../wpa_supplicant-2.10/src/common/wpa_psk_cache.h"
#include "bk_wifi_types.h"
#include "sa_ap.h"
#include "wifi_v2.h"
#include <common/sys_config.h>
//#include <components/system.h>
#include "wifi_config.h"
#ifdef CONFIG_P2P
#include "rw_tx_buffering.h"
#endif

#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "rw_ieee80211.h"
#include "rwnx_rx.h"

#include "wifi_init.h"
#include "rw_msg_tx.h"
//#if CONFIG_AP_PROBEREQ_CB//sr_get_scan_number
#include "rw_msg_rx.h"
//#endif

#include "bk_wifi.h"
#include "../wpa_supplicant-2.10/hostapd/main_none.h"
#if CONFIG_AP_STATYPE_LIMIT
#include "bk_vsie_cus.h"
#endif
#include "../wpa_supplicant-2.10/hostapd/hostapd_cfg.h"
#include "bk_wifi_prop_private.h"
//#include "net.h"
#include "bk_ps.h"
#include "bk_ps_time.h"
#include "bk_general_dma.h"
#include "modules/pm.h"
#include "bk_feature.h"
#include "bk_rf_internal.h"
#include "bk_phy_internal.h"
#include "bk_wifi_adapter.h"
#include "bk_phy_adapter.h"
#include "fhost_msg.h"
#include "../../../wpa_supplicant-2.10/wpa_supplicant/ctrl_iface.h"
#if CONFIG_EASY_FLASH_FAST_CONNECT
#if (CONFIG_EASY_FLASH && CONFIG_EASY_FLASH_V4)
#include "bk_ef.h"
#endif
#endif

__attribute__((section(".dtcm_sec_data "))) wifi_os_funcs_t *g_wifi_funcs = NULL;
__attribute__((section(".dtcm_sec_data "))) wifi_os_variable_t *g_wifi_vars = NULL;
extern struct scan_cfg_scan_param_tag scan_param_env;
extern struct scan_cfg_scan_param_tag scan_param_dump_env;

void rwnxl_reset_evt(int dummy);
static int wifi_sta_init_global_config(void);
static int wifi_scan_init_global_config(void);
static int wifi_sta_get_global_config(wifi_sta_config_t *config);
static int wifi_sta_set_bssid_wpa_config(const wifi_sta_config_t *config);
static int wifi_sta_set_wpa_config(const wifi_sta_config_t *config);
int wifi_monitor_register_cb(const wifi_monitor_cb_t monitor_cb);
void mm_hw_ap_disable(void);
int wifi_monitor_set_config(const wifi_monitor_config_t *monitor_config);
int wifi_monitor_get_config(wifi_monitor_config_t *monitor_config);
int wifi_monitor_register_cb(const wifi_monitor_cb_t monitor_cb);
wifi_monitor_cb_t wifi_monitor_get_cb(void);
wifi_monitor_cb_t rxu_cntrl_get_monitor_cb(void);
int wifi_monitor_set_enabled(bool enabled);
bool wifi_monitor_is_enabled(void);

int wifi_filter_set_config(const wifi_filter_config_t *filter_config);
int wifi_filter_get_config(wifi_filter_config_t *filter_config);
int wifi_filter_register_cb(const wifi_filter_cb_t filter_cb);
uint16_t chan_get_vif_frequency(void *vif);

extern void bmsg_ps_sender(uint8_t ioctl);
extern uint8_t phy_open_cca(void);
extern uint8_t phy_close_cca(void);
extern uint8_t phy_show_cca(void);
int ps_cmd_get(void);
extern void* mac_vif_tx_stats_get(uint8_t vif_idx);
extern uint8_t vif_mgmt_get_sta_vif_index(void);
extern uint8_t vif_mgmt_get_softap_vif_index(void);
/**
 * wifi_sec_type_string - Get the security name as a text string
 * @state: security
 * Returns: The security name as a printable text string
 */
const char *wifi_sec_type_string(wifi_security_t security)
{
	switch (security) {
	case WIFI_SECURITY_NONE:
		return "NONE";
	case WIFI_SECURITY_WEP:
		return "WEP";
	case WIFI_SECURITY_WPA_TKIP:
		return "WPA-TKIP";
	case WIFI_SECURITY_WPA_AES:
		return "WPA-AES";
	case WIFI_SECURITY_WPA_MIXED:
		return "WPA-MIX";
	case WIFI_SECURITY_WPA2_TKIP:
		return "WPA2-TKIP";
	case WIFI_SECURITY_WPA2_AES:
		return "WPA2-AES";
	case WIFI_SECURITY_WPA2_MIXED:
		return "WPA2-MIX";
	case WIFI_SECURITY_WPA3_SAE:
		return "WPA3-SAE";
	case WIFI_SECURITY_WPA3_WPA2_MIXED:
		return "WPA3-WPA2-MIX";
	case WIFI_SECURITY_EAP:
		return "EAP";
	case WIFI_SECURITY_OWE:
		return "OWE";
	case WIFI_SECURITY_AUTO:
		return "AUTO";
	default:
		return "UNKNOWN";
	}
}


/**
 * TODO
 *  - more protect, thread-safe
 *  - check return value of supplicant API
 *  - debug code
 */

#define WLAN_CHECK_ARG	0

#define WLAN_RETURN_NULL_POINTER(p)                  \
	do {                                        \
		if (p == NULL) {                        \
			WPA_LOGE("invalid param\n");        \
			return -1;                          \
		}                                       \
	} while (0)

/* copy from wifi_v1.c */
static monitor_cb_t g_bcn_cb = 0;
FUNC_1PARAM_PTR connection_status_cb = 0;
bool close_coexist_csa = false;

void bk_wlan_dhcp_timeout(void)
{
	void *p_vif_entry = mac_vif_mgmt_first_vif(); //vif_mgmt_first_used();
	DISCONNECT_PARAM_T disconnect_param = {0};
	disconnect_param.vif_idx = mac_vif_mgmt_get_index(p_vif_entry);
	disconnect_param.reason_code = WIFI_REASON_DHCP_TIMEOUT;

	while (p_vif_entry != NULL) {
		if (mac_vif_mgmt_get_type(p_vif_entry) == VIF_STA) {
#if CONFIG_LWIP
			sta_ip_down();
#endif
			disconnect_param.vif_idx = mac_vif_mgmt_get_index(p_vif_entry);
			rw_msg_send_sm_disconnect_req(&disconnect_param);
		}
		p_vif_entry = mac_vif_mgmt_next_vif(p_vif_entry);
	}
}

uint32_t bk_sta_cipher_is_open(void)
{
	BK_ASSERT(g_sta_param_ptr); /* ASSERT VERIFIED */
	return (BK_SECURITY_TYPE_NONE == g_sta_param_ptr->cipher_suite);
}

uint8_t bk_wlan_ap_get_channel_config(void)
{
	return g_ap_param_ptr->chann;
}

void bk_wlan_ap_set_channel_config(uint8_t channel)
{
	g_ap_param_ptr->chann = channel;
}

uint8_t bk_wlan_has_role(uint8_t role)
{
	void *vif_entry;
	uint32_t role_count = 0;

	vif_entry = (VIF_INF_PTR)rwm_mgmt_is_vif_first_used();
	while (vif_entry) {
		if (mac_vif_mgmt_get_type(vif_entry) == role)
			role_count ++ ;

		vif_entry = mac_vif_mgmt_next_vif(vif_entry);
	}

	return role_count;
}

void bk_wlan_set_coexist_at_init_phase(uint8_t current_role)
{
	uint32_t coexit = 0;

	switch (current_role) {
	case CONFIG_ROLE_AP:
		if (bk_wlan_has_role(VIF_STA))
			coexit = 1;
		break;

	case CONFIG_ROLE_STA:
		if (bk_wlan_has_role(VIF_AP))
			coexit = 1;
		break;

	case CONFIG_ROLE_NULL:
		if (bk_wlan_has_role(VIF_STA)
			&& bk_wlan_has_role(VIF_AP))
			coexit = 1;
		break;

	case CONFIG_ROLE_COEXIST:
		coexit = 1;
		BK_ASSERT(CONFIG_ROLE_COEXIST == g_wlan_general_param->role); /* ASSERT VERIFIED */
		break;

	default:
		break;
	}

	if (coexit)
		g_wlan_general_param->role = CONFIG_ROLE_COEXIST;
}

uint16_t bk_wlan_sta_get_frequency(void)
{
	uint16_t frequency = 0;
	uint32_t sta_flag = 0;
	VIF_INF_PTR vif_entry;

	vif_entry = (VIF_INF_PTR)rwm_mgmt_is_vif_first_used();
	while (vif_entry) {
		if (mac_vif_mgmt_get_type(vif_entry) == VIF_STA) {
			sta_flag = 1;
			break;
		}

		vif_entry = (VIF_INF_PTR)rwm_mgmt_next(vif_entry);
	}

	if (0 == sta_flag)
		goto get_exit;

	frequency = chan_get_vif_frequency(vif_entry);

get_exit:
	return frequency;
}

void bk_wlan_ap_csa_coexist_mode(void *arg, uint8_t dummy)
{
	int ret = 0;
	uint16_t frequency;

	if (0 == bk_wlan_has_role(VIF_AP))
		return;

	frequency = bk_wlan_sta_get_frequency();
	if (frequency) {
		WIFI_LOGD("hostapd_channel_switch\n");
		ret = hostapd_channel_switch(frequency);

		if (ret)
			WIFI_LOGW("csa_failed:%x\r\n", ret);
	}
}

void bk_wlan_reg_csa_cb_coexist_mode(void)
{
	/* the callback routine will be invoked at the timepoint of associating at station mode*/
	mhdr_connect_user_cb(bk_wlan_ap_csa_coexist_mode, 0);
}

uint8_t bk_wlan_sta_get_channel(void)
{
	uint8_t channel = 0;
	uint16_t frequency;

	frequency = bk_wlan_sta_get_frequency();
	if (frequency)
		channel = rw_ieee80211_get_chan_id(frequency);

	return channel;
}

uint8_t sys_channel = DEFAULT_CHANNEL_AP;
uint8_t bk_wlan_ap_get_default_channel(void)
{
	uint8_t channel;

	/* if ap and sta are coexist, ap channel shall match sta channel firstly*/
	channel = bk_wlan_sta_get_channel();
	if (0 == channel) {
		if (sys_channel == DEFAULT_CHANNEL_AP)
			channel = DEFAULT_CHANNEL_AP;
		else
			channel = sys_channel;
	}

	return channel;
}

void bk_wlan_ap_set_default_channel(uint8_t channel)
{
	sys_channel = channel;
}

/* FIXME: don't get scan result from umac */
unsigned char bk_wlan_get_scan_ap_result_numbers(void)
{
	return sr_get_scan_number();
}

//TODO move to better place
void bk_wlan_ap_init(network_InitTypeDef_st *inNetworkInitPara)
{
	WIFI_LOGD("Soft_AP_start\r\n");

	if (!g_ap_param_ptr) {
		g_ap_param_ptr = (ap_param_t *)os_zalloc(sizeof(ap_param_t));
		BK_ASSERT(g_ap_param_ptr); /* ASSERT VERIFIED */
	}

	os_memset(g_ap_param_ptr, 0x00, sizeof(*g_ap_param_ptr));

	if (is_zero_ether_addr((u8 *)&g_ap_param_ptr->bssid))
		bk_wifi_ap_get_mac((uint8_t*)(&g_ap_param_ptr->bssid));

	bk_wlan_ap_set_channel_config(bk_wlan_ap_get_default_channel());
	if (!g_wlan_general_param) {
		g_wlan_general_param = (general_param_t *)os_zalloc(sizeof(general_param_t));
		BK_ASSERT(g_wlan_general_param); /* ASSERT VERIFIED */
	}
	g_wlan_general_param->role = CONFIG_ROLE_AP;
	bk_wlan_set_coexist_at_init_phase(CONFIG_ROLE_AP);

	if (inNetworkInitPara) {
		g_ap_param_ptr->ssid.length = MIN(SSID_MAX_LEN, os_strlen(inNetworkInitPara->wifi_ssid));
		os_memcpy(g_ap_param_ptr->ssid.array, inNetworkInitPara->wifi_ssid, g_ap_param_ptr->ssid.length);
		g_ap_param_ptr->key_len = os_strlen(inNetworkInitPara->wifi_key);
		g_ap_param_ptr->hidden_ssid = inNetworkInitPara->hidden_ssid;
		if (g_ap_param_ptr->key_len < 8)
			g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_NONE;
		else {
#if CONFIG_SOFTAP_WPA3
			g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA3_WPA2_MIXED;
#else
			g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA2_AES;
#endif
			os_memcpy(g_ap_param_ptr->key, inNetworkInitPara->wifi_key, g_ap_param_ptr->key_len);
		}

#if 0//CONFIG_LWIP
		if (inNetworkInitPara->dhcp_mode == DHCP_SERVER)
			g_wlan_general_param->dhcp_enable = 1;
		else
			g_wlan_general_param->dhcp_enable = 0;
		inet_aton(inNetworkInitPara->local_ip_addr, &(g_wlan_general_param->ip_addr));
		inet_aton(inNetworkInitPara->net_mask, &(g_wlan_general_param->ip_mask));
		inet_aton(inNetworkInitPara->gateway_ip_addr, &(g_wlan_general_param->ip_gw));
#endif

	}

	sa_ap_init();
}

#if CONFIG_STA_PS
//static uint32_t wifi_ps_enabled = 0;//defined but not used
int bk_wlan_ps_enable(void)
{
	if (wifi_ap_is_started())
	{
		WIFI_LOGD("Softap is on,powersave can't use\n\r");
		return 0;
	}

	bmsg_ps_sender(PS_BMSG_IOCTL_PS_ENABLE);
	return 0;
}

int bk_wlan_ps_disable_send_msg(void)
{
	if (wifi_ap_is_started())
	{
		WIFI_LOGD("Softap is on,powersave can't use\n\r");
		return 0;
	}
	bmsg_ps_sender(PS_BMSG_IOCTL_PS_DISANABLE);
	return 0;
}

int bk_wlan_ps_disable(void)
{
	bk_wlan_ps_disable_send_msg();
	return 0;
}

int bk_wlan_ps_timer_start(void)
{
	bmsg_ps_sender(PS_BMSG_IOCTL_RF_PS_TIMER_INIT);
	return 0;
}

int bk_wlan_ps_timer_pause(void)
{
	bmsg_ps_sender(PS_BMSG_IOCTL_RF_PS_TIMER_DEINIT);
	return 0;
}
#endif

#if CONFIG_MCU_PS
static uint32_t mcu_ps_enabled = 0;
int bk_wlan_mcu_ps_mode_enable(void)
{
	mcu_ps_enabled = 1;
	bmsg_ps_sender(PS_BMSG_IOCTL_MCU_ENABLE);

	return 0;
}

int bk_wlan_mcu_ps_mode_disable(void)
{
	mcu_ps_enabled = 0;
	bmsg_ps_sender(PS_BMSG_IOCTL_MCU_DISANABLE);

	return 0;
}
#endif
bk_err_t bk_wlan_start_ap(network_InitTypeDef_st *inNetworkInitParaAP)
{
#if CONFIG_LWIP
	/* stop lwip netif */
	sta_ip_down();
#endif

	/* set AP parameter, ssid, akm, etc. */
	bk_wlan_ap_init(inNetworkInitParaAP);
	// enable hostapd
	wlan_ap_enable();

	// reload bss configuration
	wlan_ap_reload();

#if 0//CONFIG_LWIP
	/* now ap has started, set ip address to this interface */
	ip_address_set(BK_SOFT_AP,
				   inNetworkInitParaAP->dhcp_mode,
				   inNetworkInitParaAP->local_ip_addr,
				   inNetworkInitParaAP->net_mask,
				   inNetworkInitParaAP->gateway_ip_addr,
				   inNetworkInitParaAP->dns_server_ip_addr);

	/* restart lwip network */
	uap_ip_start();
#endif

	return kNoErr;
}

/* FIXME: don't get scan result from umac */
int bk_wlan_get_scan_ap_result(SCAN_RST_ITEM_PTR scan_rst_table, unsigned char get_scanu_num)
{
	struct scanu_rst_upload *scan_rst;
	unsigned char scanu_num = 0, i;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

	scan_rst = sr_get_scan_results();
	if (scan_rst) {
		scanu_num = (scan_rst->scanu_num) > get_scanu_num ? (get_scanu_num) : (scan_rst->scanu_num);

		for (i = 0; i < scanu_num; i++)
			scan_rst_table[i] = *(scan_rst->res[i]);
	}
	GLOBAL_INT_RESTORE();

	sr_release_scan_results(scan_rst);
	return scanu_num;
}

static void wlan_ui_bcn_callback(uint8_t *data, int len, wifi_link_info_t *info)
{
#if XXX // FIXME: dead code
#if CONFIG_STA_PS
	if (power_save_if_ps_rf_dtim_enabled())
		mac_ps_bcn_callback(data, len);
#endif
#if CONFIG_MCU_PS
	mcu_ps_bcn_callback(data, len);
#endif
#endif
}

static void bk_wlan_register_bcn_cb(monitor_cb_t fn)
{
	g_bcn_cb = fn;
}

void wifi_sta_reg_bcn_cb(void)
{
	bk_wlan_register_bcn_cb(wlan_ui_bcn_callback);
}

monitor_cb_t bk_wlan_get_bcn_cb(void)
{
	return g_bcn_cb;
}

void bk_wlan_status_register_cb(FUNC_1PARAM_PTR cb)
{
	connection_status_cb = cb;
}

FUNC_1PARAM_PTR bk_wlan_get_status_cb(void)
{
	return connection_status_cb;
}

int bk_wlan_send_80211_raw_frame(uint8_t *buffer, int len)
{
	return bk_wifi_send_raw(buffer, len);
}


#if 1//(CONFIG_ALIOS)
/**********************for alios*******************************/

bk_err_t bk_wlan_get_bssid_info(apinfo_adv_t *ap, uint8_t **key, int *key_len)
{
	wifi_link_status_t link_stat;

	if (wifi_ap_is_started())
		return kGeneralErr;

	ap->security = g_sta_param_ptr->cipher_suite;
	os_memset(&link_stat, 0, sizeof(link_stat));
	bk_wifi_sta_get_link_status(&link_stat);
	ap->channel = link_stat.channel;
	os_memcpy(ap->bssid, link_stat.bssid, 6);
	os_strcpy((char *)ap->ssid, (char *)link_stat.ssid);

	*key = g_sta_param_ptr->key;
	*key_len = g_sta_param_ptr->key_len;

	return 0;
}
#endif

void bk_wlan_sta_init(network_InitTypeDef_st *inNetworkInitPara)
{
	if (!g_sta_param_ptr) {
		g_sta_param_ptr = (sta_param_t *)os_zalloc(sizeof(sta_param_t));
		BK_ASSERT(g_sta_param_ptr); /* ASSERT VERIFIED */
	}

	bk_wifi_sta_get_mac((uint8_t*)(&g_sta_param_ptr->own_mac));
	if (!g_wlan_general_param) {
		g_wlan_general_param = (general_param_t *)os_zalloc(sizeof(general_param_t));
		BK_ASSERT(g_wlan_general_param); /* ASSERT VERIFIED */
	}
	g_wlan_general_param->role = CONFIG_ROLE_STA;
	bk_wlan_set_coexist_at_init_phase(CONFIG_ROLE_STA);

	if (inNetworkInitPara) {
		g_sta_param_ptr->ssid.length = MIN(SSID_MAX_LEN, os_strlen(inNetworkInitPara->wifi_ssid));
		os_memcpy(g_sta_param_ptr->ssid.array,
				  inNetworkInitPara->wifi_ssid,
				  g_sta_param_ptr->ssid.length);

		if(bk_feature_bssid_connect_enable()) {
			os_memcpy(g_sta_param_ptr->fast_connect.bssid, inNetworkInitPara->wifi_bssid, sizeof(inNetworkInitPara->wifi_bssid));
		}

		g_sta_param_ptr->key_len = os_strlen(inNetworkInitPara->wifi_key);
		os_memcpy(g_sta_param_ptr->key, inNetworkInitPara->wifi_key, g_sta_param_ptr->key_len);
		g_sta_param_ptr->key[g_sta_param_ptr->key_len] = 0;		/* append \0 */
	}

	if (!(bk_feature_close_coexist_csa() || close_coexist_csa))
		bk_wlan_reg_csa_cb_coexist_mode();
	sa_station_init();

	bk_wlan_register_bcn_cb(wlan_ui_bcn_callback);
}

bk_err_t bk_wlan_start_sta(network_InitTypeDef_st *inNetworkInitPara)
{
	size_t psk_len = 0;
	u8 *psk = 0;
	wifi_linkstate_reason_t info;
	int chan = 0;

	/* diconnect previous connection if may */
#if CONFIG_LWIP
	sta_ip_down();	// XXX: WLAN_DISCONNECT_EVENT may handle this
#endif
	wlan_sta_disconnect();

	info.state = WIFI_LINKSTATE_STA_CONNECTING;
	info.reason_code = WIFI_REASON_MAX;
	mhdr_set_station_status(info);

#if (CONFIG_RF_POLICY_WIFI_REQ)
	wifi_station_status_event_notice(0, WIFI_LINKSTATE_STA_CONNECTING);
#endif

#if (CONFIG_SOC_BK7231N)
	if (ate_is_enabled()) {
		// cunliang20210407 set blk_standby_cfg with blk_txen_cfg like txevm, qunshan confirmed
		rwnx_cal_en_extra_txpa();
	}
#endif
	bk_wlan_sta_init(inNetworkInitPara);

	if (bk_feature_fast_connect_enable()) {
		struct wlan_fast_connect_info fci = {0};
		int ssid_len, req_ssid_len;
#if CONFIG_EASY_FLASH_FAST_CONNECT
		bk_get_env_enhance("fast_connect_id", (void *)&fci, sizeof(struct wlan_fast_connect_info));
#endif

		ssid_len = os_strlen((char *)fci.ssid);
		if (ssid_len > SSID_MAX_LEN)
			ssid_len = SSID_MAX_LEN;

		req_ssid_len = os_strlen(inNetworkInitPara->wifi_ssid);
		if (req_ssid_len > SSID_MAX_LEN)
			req_ssid_len = SSID_MAX_LEN;

		if (ssid_len == req_ssid_len &&
			os_memcmp(inNetworkInitPara->wifi_ssid, fci.ssid, ssid_len) == 0 &&
			os_strcmp(inNetworkInitPara->wifi_key, (char *)fci.pwd) == 0) {
			chan = fci.channel;
			psk = fci.psk;
			psk_len = PMK_LEN * 2;

			WIFI_LOGD("fast_connect\n");
			if (os_strlen((char *)psk) == 0) {
				// no psk info, calcuate pmk
				psk = 0;
				psk_len = 0;
			}
		}
	}

	/*
	 * let wpa_psk_cal thread to caculate psk.
	 * XXX: If you know psk value, fill last two parameters of `wpa_psk_request()'.
	 */
	wpa_psk_request(g_sta_param_ptr->ssid.array, g_sta_param_ptr->ssid.length,
					(char *)g_sta_param_ptr->key, psk, psk_len);

	/* enable wpa_supplicant */
	if (wlan_sta_enable()) {
		WIFI_LOGW("%s wifi enable fail\n");
		return kGeneralErr;
	}

	/* set network parameters: ssid, passphase */
	wlan_sta_set((uint8_t *)inNetworkInitPara->wifi_ssid, os_strlen(inNetworkInitPara->wifi_ssid), (uint8_t *)inNetworkInitPara->wifi_key);

	if (bk_feature_bssid_connect_enable()) {
		/* set bssid */
		if (!is_zero_ether_addr((u8 *)inNetworkInitPara->wifi_bssid) &&
			!is_broadcast_ether_addr((u8 *)inNetworkInitPara->wifi_bssid)) {

			wlan_sta_config_t config;

			os_memset(&config, 0, sizeof(config));
			os_memcpy(config.u.bssid, inNetworkInitPara->wifi_bssid, ETH_ALEN);
			config.field = WLAN_STA_FIELD_BSSID;
			wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config);
		}
	}

	/* connect to AP */
	wlan_sta_connect(chan);

    return kNoErr;
}

#ifdef CONFIG_P2P
beken_queue_t  g_msg_queue;
void app_p2p_rw_event_func(void *new_evt)
{
	int ret;
	DRONE_MSG_T msg;
	wifi_link_state_t evt_type = *((wifi_link_state_t *)new_evt);

	msg.dmsg = evt_type;
	ret = rtos_push_to_queue(&g_msg_queue, &msg, BEKEN_NO_WAIT);
	if (ret)
		WIFI_LOGE("%s push err!\n", __func__);
}

int app_deinit(void)
{
	int ret = 0;
	DRONE_MSG_T msg;
	int status = 0;

	ret = rtos_init_queue(&g_msg_queue,
				"app_demo_p2p_cancel,queue",
                              sizeof(DRONE_MSG_T),
                              1);

	while(1) {
		ret = rtos_pop_from_queue(&g_msg_queue, &msg, BEKEN_WAIT_FOREVER);
		if (msg.dmsg == WIFI_LINKSTATE_AP_DISCONNECTED && status == 1) {
			rwnx_hw_reinit();
			uap_ip_down();
			wlan_p2p_cancel();
			wlan_p2p_find();
			status = 0;
		}
		else if ((msg.dmsg == WIFI_LINKSTATE_AP_CONNECTED ||
				msg.dmsg == WIFI_LINKSTATE_STA_GOT_IP) && status ==0) {
			status = 1;
		}
		else if ((msg.dmsg == WIFI_LINKSTATE_STA_DISCONNECTED ||
			 msg.dmsg == WIFI_LINKSTATE_STA_CONNECT_FAILED) && status == 1) {
			sta_ip_down();
			rwnx_hw_reinit();
			sys_msleep(2000);
			wlan_p2p_find();
			status = 0;
		}
	}

	return ret;
}

beken_thread_t p2p_restart_thread_hdl;
void app_p2p_restart_thread(void)
{
	 rtos_create_thread(&p2p_restart_thread_hdl,
                                 BEKEN_DEFAULT_WORKER_PRIORITY,
                                 "app_deinit_thread",
                                 (beken_thread_function_t)app_deinit,
                                 2048,
                                 (beken_thread_arg_t)NULL);
}

int bk_wlan_start_p2p(network_InitTypeDef_st *inNetworkInitPara)
{
//	size_t psk_len = 0;
//	u8 *psk = 0;

#if CFG_WPA_CTRL_IFACE

#if CFG_WLAN_FAST_CONNECT
	struct wlan_fast_connect_info fci = {0};
	int ssid_len, req_ssid_len;
#endif
//	int chan = 0;

	/* diconnect previous connection if may */
	sta_ip_down();	// XXX: WLAN_DISCONNECT_EVENT may handle this
	wlan_sta_disconnect();
#else /* !CFG_WPA_CTRL_IFACE */
	bk_wifi_sta_stop();
#endif

  //  mhdr_set_station_status(RW_EVT_STA_CONNECTING);

#if 0
#if (RF_USE_POLICY == BLE_DEFAULT_WIFI_REQUEST)
    wifi_station_status_event_notice(0,RW_EVT_STA_CONNECTING);
#endif
#endif

	bk_wlan_sta_init(inNetworkInitPara);
	if (wlan_sta_enable()) {
		WIFI_LOGE("%s wifi enable fail\n");
		return kGeneralErr;
	}

#if CONFIG_P2P
	wlan_p2p_find();
#endif

	return kNoErr;
}
#endif

bk_err_t bk_wlan_start(network_InitTypeDef_st *inNetworkInitPara)
{
	if (inNetworkInitPara->wifi_mode == BK_SOFT_AP)
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		bk_wlan_start_ap(inNetworkInitPara);
#pragma GCC diagnostic pop
	}
	else if (inNetworkInitPara->wifi_mode == BK_STATION)
		bk_wlan_start_sta(inNetworkInitPara);
#ifdef CONFIG_P2P
	else if (inNetworkInitPara->wifi_mode == BK_P2P)
		bk_wlan_start_p2p(inNetworkInitPara);
#endif
	return 0;
}

int bk_sta_cipher_type(void)
{
	return g_sta_param_ptr->cipher_suite;
}

void bk_wlan_phy_open_cca(void)
{
	phy_open_cca();
	WIFI_LOGD("bk_wlan cca opened\r\n");
}

void bk_wlan_phy_close_cca(void)
{
	phy_close_cca();
	WIFI_LOGD("bk_wlan cca closed\r\n");
}

void bk_wlan_phy_show_cca(void)
{
	phy_show_cca();
}

 /**
 * @brief Configure station in a convenient way to join a specified network
 * @param[in] ssid Network name, length is [1, 32]
 * @param[in] ssid_len The length of network name
 * @param[in] psk Network password, in one of the optional formats:
 *            - NULL or an empty string, to join an OPEN network
 *            - an ASCII string, length is [8, 63]
 *            - a hex string (two characters per octet of PSK), length is 64
 * @return 0 on success, -1 on failure
 *
 * @note This way is only adapted to join an OPEN, WPA-PSK, WPA2-PSK or
 *       WPA-PSK/WPA2-PSK network.
 */
int wlan_sta_set(const uint8_t *ssid, uint8_t ssid_len, const uint8_t *psk)
{
	if ((!bk_feature_bssid_connect_enable() && (ssid == NULL || ssid_len == 0)) || ssid_len > WLAN_SSID_MAX_LEN) {
		WPA_LOGE("invalid ssid (%p, %u)\n", ssid, ssid_len);
		return BK_ERR_PARAM;
	}

	wlan_sta_config_t config;
	os_memset(&config, 0, sizeof(config));

	/* ssid */
	config.field = WLAN_STA_FIELD_SSID;
	os_memcpy(config.u.ssid.ssid, ssid, ssid_len);
	config.u.ssid.ssid_len = ssid_len;
	BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));

	if ((psk == NULL) || (psk[0] == '\0')) {
		/* key_mgmt: NONE */
		config.field = WLAN_STA_FIELD_KEY_MGMT;
		config.u.key_mgmt = WPA_KEY_MGMT_NONE;
		BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));
	} else {
		/* psk */
		config.field = WLAN_STA_FIELD_PSK;
		os_strlcpy((char *)config.u.psk, (char *)psk, sizeof(config.u.psk));
		BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));

		/* key_mgmt: PSK */
		config.field = WLAN_STA_FIELD_KEY_MGMT;
		config.u.key_mgmt = WPA_KEY_MGMT_PSK;
#ifdef CONFIG_IEEE80211W
		config.u.key_mgmt |= WPA_KEY_MGMT_PSK_SHA256;
#endif
#ifdef CONFIG_SAE
		config.u.key_mgmt |= WPA_KEY_MGMT_SAE;
#endif
#ifdef CONFIG_WPA2_ENTERPRISE
		config.u.key_mgmt |= WPA_KEY_MGMT_IEEE8021X;
#endif
#ifdef CONFIG_OWE
		config.u.key_mgmt |= WPA_KEY_MGMT_OWE;
#endif

		BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));

		/* proto: WPA | RSN */
		config.field = WLAN_STA_FIELD_PROTO;
		config.u.proto = WPA_PROTO_WPA | WPA_PROTO_RSN;
		BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));

		/* pairwise: CCMP | TKIP */
		config.field = WLAN_STA_FIELD_PAIRWISE_CIPHER;
		config.u.pairwise_cipher = WPA_CIPHER_CCMP | WPA_CIPHER_TKIP
								   | WPA_CIPHER_CCMP_256;
		BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));

		/* group: CCMP | TKIP | WEP40 | WEP104 */
		config.field = WLAN_STA_FIELD_GROUP_CIPHER;
		config.u.pairwise_cipher = WPA_CIPHER_CCMP | WPA_CIPHER_TKIP
								   | WPA_CIPHER_WEP40 | WPA_CIPHER_WEP104;
		BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));
	}

	/* scan_ssid: 1 */
	config.field = WLAN_STA_FIELD_SCAN_SSID;
	config.u.scan_ssid = ssid && ssid_len;
	BK_RETURN_ON_ERR(wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config));

	return BK_OK;
}

/**
 * @brief Set station specified field configuration
 * @param[in] config Pointer to the configuration
 * @return 0 on success, -1 on failure
 */
int wlan_sta_set_config(wlan_sta_config_t *config)
{
	WLAN_RETURN_NULL_POINTER(config);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, config);
}

/**
 * @brief Get station specified field configuration
 * @param[in] config Pointer to the configuration
 * @return 0 on success, -1 on failure
 */
int wlan_sta_get_config(wlan_sta_config_t *config)
{
	WLAN_RETURN_NULL_POINTER(config);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_GET, config);
}

/**
 * @brief Set autoconnect after bss lost configuration
 * @param[in] config enable or disable autoconnect function
 * @return 0 on success, -1 on failure
 */
int wlan_sta_set_autoreconnect(wlan_auto_reconnect_t *config)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_AUTORECONNECT, config);
}

/**
 * @brief Get the assoc status code of disconnected AP
 * @param status_code auth/assoc status code
 *
 * @return 0 on success, -1 on failure
 */
int wlan_sta_get_assoc_status_code(int *status_code)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_GET_ASSOC_STATUS, status_code);
}

/**
 * @brief Get the information size of current bss
 * @param[in] config Pointer to the information size
 * @return 0 on success, -1 on failure
 */
int wlan_sta_get_bss_size(uint32_t *size)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_BSS_SIZE_GET, size);
}

/**
 * @brief Get the information of current bss
 * @param[in] config Pointer to the information
 * @return 0 on success, -1 on failure
 */
int wlan_sta_get_bss(wlan_sta_bss_info_t *bss_get)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_BSS_GET, bss_get);
}

/**
 * @brief Set the information of bss which will be used
 * @param[in] config Pointer to the information of bss
 * @return 0 on success, -1 on failure
 */
int wlan_sta_set_bss(wlan_sta_bss_info_t *bss_set)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_BSS_SET, bss_set);
}

/**
 * @brief Enable the station
 * @return 0 on success, -1 on failure
 */
int wlan_sta_enable(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_ENABLE, NULL);
}

/**
 * @brief Disable the station
 * @return 0 on success, -1 on failure
 */
int wlan_sta_disable(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_DISABLE, NULL);
}

/**
 * @brief Station scan once according to the default parameters
 * @return 0 on success, <0 on failure
 */
int wlan_sta_scan_once(void)
{
	return wlan_sta_scan(NULL);
}

static bool wlan_sta_scan_need_retry(int ret)
{
	return ((ret == WPA_ERR_SCAN_PENDING) || (ret == WPA_ERR_SCAN_SELECT_IN_PROGRESS)
			|| (ret == WPA_ERR_SCAN_SCHED_IN_PROGRESS) || (ret == WPA_ERR_SCAN_IN_PROGRESS)
			|| (ret == WPA_ERR_TRY_AGAIN));
}

#define WLAN_SCAN_MAX_RETRIES 3000
/**
 * @brief Station scan once according to the specified parameters
 * @return 0 on success, <0 on failure
 */
int wlan_sta_scan(wlan_sta_scan_param_t *param)
{
	int ret = 0;
	int max_tries = 0;

	while (1) {
		ret = wpa_ctrl_request(WPA_CTRL_CMD_STA_SCAN, param);
		if (wlan_sta_scan_need_retry(ret)) {
			/* previous scan in progress */
			if (++max_tries > WLAN_SCAN_MAX_RETRIES) {
				wpa_ctrl_request(WPA_CTRL_CMD_DEBUG_INFO_DUMP, (void *)WPA_CTRL_DEBUG_INFO_DUMP_SCAN);
				return ret;
			}

			rtos_delay_milliseconds(10);
			continue;
		} else
			break;
	}

	return ret;
}

/**
 * @brief Get station scan results
 * @param[in] results Pointer to the scan results
 * @return 0 on success, -1 on failure
 */
int wlan_sta_scan_result(ScanResult_adv *results)
{
	int ret;

	WLAN_RETURN_NULL_POINTER(results);

	ret = wpa_ctrl_request(WPA_CTRL_CMD_STA_SCAN_RESULTS, results);

#if CONFIG_FAST_FLUSH_SCAN_RESULTS
	/* flush scan results */
	wlan_sta_bss_flush(0);
#endif

	return ret;
}

/**
 * @brief Set station scan interval
 * @param[in] sec Scan interval in Seconds
 * @return 0 on success, -1 on failure
 */
int wlan_sta_scan_interval(int sec)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_SCAN_INTERVAL, (void *)sec);
}

/**
 * @brief Set maximum BSS entries to keep in memory
 * @param[in] count Maximum BSS entries to keep in memory
 * @return 0 on success, -1 on failure
 */
int wlan_sta_bss_max_count(uint8_t count)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_BSS_MAX_COUNT,
							(void *)(uint32_t)count);
}

/**
 * @brief Flush station old BSS entries
 * @param[in] age Maximum entry age in seconds
 * @return 0 on success, -1 on failure
 *
 * @note Remove BSS entries that have not been updated during the last @age
 * seconds.
 */
int wlan_sta_bss_flush(int age)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_BSS_FLUSH, (void *)age);
}

/**
 * @brief Request a new connection
 * @freq  AP's freq, specify 0 if unknown
 *
 * @return 0 on success, -1 on failure
 */
int wlan_sta_connect(int chan)
{
	//return wpa_ctrl_request(WPA_CTRL_CMD_STA_CONNECT, NULL);
	//int id = 0;
	return wpa_ctrl_request(WPA_CTRL_CMD_SELECT_NETWORK, (void *)chan);
}

/**
 * @brief Disconnect the current connection
 * @return 0 on success, -1 on failure
 */
int wlan_sta_disconnect(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_DISCONNECT, NULL);
}

/**
 * @brief Get station connection state
 * @param[in] state Pointer to the connection state
 * @return 0 on success, -1 on failure
 */
int wlan_sta_state(wlan_sta_states_t *state)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_STATE, state);
}

/**
 * @brief TWT Setup
 *   wake interval = mantissa << exp, and SP = nominal_min_wake_dur*256us if wake_dur_unit = 0
 *   else SP = nominal_min_wake_dur * 1TU.
 * @param annouced annouced twt
 * @param wake_interval_mantissa
 * @param wake_interval_exp
 * @param nominal_min_wake_dur
 * @param wake_dur_unit
 *
 * @return 0 on success, -1 on failure
 */
int wlan_sta_twt_setup(uint8_t annouced,
					   uint16_t wake_interval_mantissa, uint8_t wake_interval_exp,
					   uint8_t nominal_min_wake_dur, bool wake_dur_unit)
{
	struct twt_conf_tag twt_conf;

	twt_conf.flow_type = !!annouced;
	twt_conf.min_twt_wake_dur = nominal_min_wake_dur;
	twt_conf.wake_dur_unit = wake_dur_unit;
	twt_conf.wake_int_mantissa = wake_interval_mantissa;
	twt_conf.wake_int_exp = wake_interval_exp;

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_TWT_SETUP, (void *)&twt_conf);
}

/**
 * @brief TWT Teardown
 * @param flow_id flow identification
 *
 * @return 0 on success, -1 on failure
 */
int wlan_sta_twt_teardown(int flow_id)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_TWT_TEARDOWN, (void *)flow_id);
}

/**
 * @brief Get the information of connected AP
 * @param[in] ap Pointer to the AP information
 * @return 0 on success, -1 on failure
 */
int wlan_sta_ap_info(struct ApListStruct *ap)
{
	WLAN_RETURN_NULL_POINTER(ap);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_AP, ap);
}

/**
 * @brief Generate WPA PSK based on passphrase and SSID
 * @param[in] param Pointer to wlan_gen_psk_param_t structure
 * @return 0 on success, -1 on failure
 */
int wlan_sta_gen_psk(wlan_gen_psk_param_t *param)
{
	WLAN_RETURN_NULL_POINTER(param);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_GEN_PSK, param);
}

/**
 * @brief Start the WPS negotiation with PBC method
 * @return 0 on success, -1 on failure
 *
 * @note WPS will be turned off automatically after two minutes.
 */
int wlan_sta_wps_pbc(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_STA_WPS_PBC, NULL);
}

/**
 * @brief Get a random valid WPS PIN
 * @param[in] wps Pointer to the WPS pin
 * @return 0 on success, -1 on failure
 */
int wlan_sta_wps_pin_get(wlan_sta_wps_pin_t *wps)
{
	WLAN_RETURN_NULL_POINTER(wps);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_WPS_GET_PIN, wps);
}

/**
 * @brief Start the WPS negotiation with PIN method
 * @param[in] wps Pointer to the WPS pin
 * @return 0 on success, -1 on failure
 *
 * @note WPS will be turned off automatically after two minutes.
 */
int wlan_sta_wps_pin_set(wlan_sta_wps_pin_t *wps)
{
	WLAN_RETURN_NULL_POINTER(wps);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_WPS_SET_PIN, wps);
}

/**
 * @brief set vendor specific IEs for frame
 * @param[in] vsie
 * @return 0 on success, -1 on failure
 */
int wlan_sta_set_vendor_ie(wlan_sta_vsie_t *vsie)
{
	WLAN_RETURN_NULL_POINTER(vsie);

	return wpa_ctrl_request(WPA_CTRL_CMD_STA_SET_VSIE, vsie);
}

/**
 * @brief Configure AP in a convenient way to build a specified network
 * @param[in] ssid Network name, length is [1, 32]
 * @param[in] ssid_len The length of network name
 * @param[in] psk Network password, in one of the optional formats:
 *            - NULL or an empty string, to build an OPEN network
 *            - an ASCII string, length is [8, 63]
 *            - a hex string (two characters per octet of PSK), length is 64
 * @return 0 on success, -1 on failure
 *
 * @note This way is only adapted to build an OPEN or WPA-PSK/WPA2-PSK network.
 */
int wlan_ap_set(uint8_t *ssid, uint8_t ssid_len, uint8_t *psk)
{
	if ((ssid == NULL) || (ssid_len == 0) || (ssid_len > WLAN_SSID_MAX_LEN)) {
		WPA_LOGE("invalid ssid (%p, %u)\n", ssid, ssid_len);
		return -1;
	}

	wlan_ap_config_t config;
	os_memset(&config, 0, sizeof(config));

	/* ssid */
	config.field = WLAN_AP_FIELD_SSID;
	os_memcpy(config.u.ssid.ssid, ssid, ssid_len);
	config.u.ssid.ssid_len = ssid_len;
	if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
		return -1;

	/* auth_alg: OPEN */
	config.field = WLAN_AP_FIELD_AUTH_ALG;
	config.u.auth_alg = WPA_AUTH_ALG_OPEN;
	if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
		return -1;

	if ((psk == NULL) || (psk[0] == '\0')) {
		/* proto: 0 */
		config.field = WLAN_AP_FIELD_PROTO;
		config.u.proto = 0;
		if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
			return -1;

		/* key_mgmt: NONE */
		config.field = WLAN_AP_FIELD_KEY_MGMT;
		config.u.key_mgmt = WPA_KEY_MGMT_NONE;
		if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
			return -1;
	} else {
		/* psk */
		config.field = WLAN_AP_FIELD_PSK;
		os_strlcpy((char *)config.u.psk, (char *)psk, sizeof(config.u.psk));
		if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
			return -1;

		/* proto: WPA | RSN */
		config.field = WLAN_AP_FIELD_PROTO;
		config.u.proto = WPA_PROTO_WPA | WPA_PROTO_RSN;
		if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
			return -1;

		/* key_mgmt: PSK */
		config.field = WLAN_AP_FIELD_KEY_MGMT;
		config.u.key_mgmt = WPA_KEY_MGMT_PSK;
		if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
			return -1;
	}

	/* wpa_cipher: TKIP */
	config.field = WLAN_AP_FIELD_WPA_CIPHER;
	config.u.wpa_cipher = WPA_CIPHER_TKIP;
	if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
		return -1;

	/* rsn_cipher: CCMP */
	config.field = WLAN_AP_FIELD_RSN_CIPHER;
	config.u.rsn_cipher = WPA_CIPHER_CCMP;
	if (wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, &config) != 0)
		return -1;

	return 0;
}

/**
 * @brief Set AP specified field configuration
 * @param[in] config Pointer to the configuration
 * @return 0 on success, -1 on failure
 */
int wlan_ap_set_config(wlan_ap_config_t *config)
{
	WLAN_RETURN_NULL_POINTER(config);

	return wpa_ctrl_request(WPA_CTRL_CMD_AP_SET, config);
}

/**
 * @brief Get AP specified field configuration
 * @param[in] config Pointer to the configuration
 * @return 0 on success, -1 on failure
 */
int wlan_ap_get_config(wlan_ap_config_t *config)
{
	WLAN_RETURN_NULL_POINTER(config);

	return wpa_ctrl_request(WPA_CTRL_CMD_AP_GET, config);
}

/**
 * @brief Enable the AP
 * @return 0 on success, -1 on failure
 */
int wlan_ap_enable(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_AP_ENABLE, NULL);
}

/**
 * @brief Reload AP configuration
 * @return 0 on success, -1 on failure
 */
int wlan_ap_reload(void)
{
	int ret;
	while (1) {
		ret = wpa_ctrl_request(WPA_CTRL_CMD_AP_RELOAD, NULL);
		if (ret == -2) {
			// busy, csa in progress
			rtos_delay_milliseconds(50);
		} else
			return ret;
	}
}

/**
 * @brief Disable the AP
 * @return 0 on success, -1 on failure
 */
int wlan_ap_disable(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_AP_DISABLE, NULL);
}

/**
 * @brief Get the number of connected stations
 * @param[in] num Pointer to the number
 * @return 0 on success, -1 on failure
 */
int wlan_ap_sta_num(int *num)
{
	WLAN_RETURN_NULL_POINTER(num);

	return wpa_ctrl_request(WPA_CTRL_CMD_AP_STA_NUM, num);
}

/**
 * @brief Get the information of connected stations
 * @param[in] stas Pointer to the stations information
 * @return 0 on success, -1 on failure
 */
int wlan_ap_sta_info(wlan_ap_stas_t *stas)
{
	WLAN_RETURN_NULL_POINTER(stas);

	return wpa_ctrl_request(WPA_CTRL_CMD_AP_STA_INFO, stas);
}

/**
 * @brief Set Beacon/Probe Response Vendor IE
 * @param[in] vendor IE and its len
 * @return 0 on success, -1 on failure
 */
int wlan_ap_set_beacon_vsie(wlan_ap_vsie_t *vsie)
{
	WLAN_RETURN_NULL_POINTER(vsie);

	return wpa_ctrl_request(WPA_CTRL_CMD_UPDATE_BEACON_VSIE, vsie);
}

/**
 * @brief Set Beacon Hidden IE
 * @return 0 on success, -1 on failure
 */
int wlan_ap_set_beacon_hidden_cap(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_UPDATE_BEACON, NULL);
}

int wlan_dhcp_done_ind(u8 vif_idx)
{
	return rw_msg_dhcp_done_ind(vif_idx);
}

#if CONFIG_P2P
int wlan_p2p_listen(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_P2P_LISTEN, NULL);
}

int wlan_p2p_find(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_P2P_FIND, NULL);
}

int wlan_p2p_stop_find(void)
{
	return wpa_ctrl_request(WPA_CTRL_CMD_P2P_STOP_FIND, NULL);
}

int wlan_p2p_cancel(void)
{
        return wpa_ctrl_request(WPA_CTRL_CMD_P2P_CANCEL, NULL);
}

int wlan_p2p_connect(const uint8_t *mac, int method, int intent)
{
	struct wlan_p2p_connect_param param;

	os_memcpy(param.addr, mac, 6);
	param.method = method;
	param.intent = intent;

	return wpa_ctrl_request(WPA_CTRL_CMD_P2P_CONNECT, &param);
}

void wlan_hw_reinit(void) {
	rwnx_hw_reinit();
}
#endif


static uint16_t s_wifi_state_bits = 0;

static inline void wifi_set_state_bit(uint16_t state_bit)
{
	wifi_lock();
	s_wifi_state_bits |= state_bit;
	wifi_unlock();
}

static inline void wifi_clear_state_bit(uint16_t state_bit)
{
	wifi_lock();
	s_wifi_state_bits &= ~state_bit;
	wifi_unlock();
}

static inline bool wifi_is_inited(void)
{
	return (s_wifi_state_bits & WIFI_INIT_BIT);
}

bool wifi_sta_is_started(void)
{
	return (s_wifi_state_bits & WIFI_STA_STARTED_BIT);
}

static inline bool wifi_sta_is_connected(void)
{
	return (s_wifi_state_bits & WIFI_STA_CONNECTED_BIT);
}

bool wifi_ap_is_started(void)
{
	return (s_wifi_state_bits & WIFI_AP_STARTED_BIT);
}

static inline bool wifi_sta_is_configured(void)
{
	return (s_wifi_state_bits & WIFI_STA_CONFIGURED_BIT);
}

bool wifi_sta_is_started_or_connected(void)
{
	return (s_wifi_state_bits & (WIFI_STA_STARTED_BIT | WIFI_STA_CONNECTED_BIT));
}

static inline bool wifi_ap_is_configured(void)
{
	return (s_wifi_state_bits & WIFI_AP_CONFIGURED_BIT);
}

static inline bool wifi_monitor_is_started(void)
{
	return (s_wifi_state_bits & WIFI_MONITOR_STARTED_BIT);
}

static inline bool wifi_pure_scan_is_started(void)
{
	return (s_wifi_state_bits & WIFI_PURE_SCAN_STARTED_BIT);
}

static int wifi_init_validate_config(const wifi_init_config_t *config)
{
	if (config) {
		//TODO validate init config
	} else {
		//Use default init configuration
	}

	return BK_OK;
}


bool g_is_deepsleep;
static int wifi_deepsleep_enter_cb(uint64_t expected_time_ms, void *args)
{
	g_is_deepsleep = true;
	bk_wifi_ap_stop();
	bk_wifi_prepare_deepsleep();
	bk_wifi_sta_stop();

	rf_module_vote_ctrl(RF_CLOSE,RF_BY_WIFI_BIT);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_PHY_WIFI, PM_POWER_MODULE_STATE_OFF);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC, PM_POWER_MODULE_STATE_OFF);

	return 0;
}


static int wifi_lowvol_enter_cb(uint64_t expected_time_ms, void *args)
{
	rwnx_lowvol_enter_callback();
	return 0;
}

static int wifi_lowvol_exit_cb(uint64_t expected_time_ms, void *args)
{
	if (bk_pm_exit_low_vol_wakeup_source_get() == PM_WAKEUP_SOURCE_INT_GPIO)
		bk_wifi_media_dtim();
	rwnx_lowvol_exit_callback();
	return 0;
}

bk_err_t bk_wifi_adapter_init(const wifi_init_config_t *config)
{
	int ret = BK_OK;

	if (config->os_funcs == NULL) {
		//WIFI_LOGI("wifi func cb not be registered!\n");
		return BK_FAIL;
	} else {
		g_wifi_funcs = config->os_funcs;
		g_wifi_vars = config->os_val;
	}

	BK_RETURN_ON_ERR(wifi_init_validate_config(config));

    return ret;
}

bk_err_t bk_wifi_init(const wifi_init_config_t *config)
{
	int ret = BK_OK;

#if 0
	if (config->os_funcs == NULL) {
		//WIFI_LOGI("wifi func cb not be registered!\n");
		return BK_FAIL;
	} else {
		g_wifi_funcs = config->os_funcs;
		g_wifi_vars = config->os_val;
	}
#endif
	bk_wifi_adapter_init(config);

	bk_wifi_interrupt_init();

	WIFI_LOGD("wifi initing\n");
	//BK_RETURN_ON_ERR(wifi_init_validate_config(config));

	if (wifi_is_inited())
		WIFI_LOGD("wifi already init, reinit anyway!\n");

	ret = wifi_init(config);

	//TODO set the init flag according to the return value!
	wifi_set_state_bit(WIFI_INIT_BIT);
	WIFI_LOGD("wifi inited(%x) ret(%x)\n", s_wifi_state_bits, ret);

	if (bk_pm_sleep_register_wrapper(wifi_deepsleep_enter_cb))
		WIFI_LOGW("register wifi pm sleep cb fail!\n");

	if (bk_pm_low_voltage_register_wrapper(wifi_lowvol_enter_cb,wifi_lowvol_exit_cb))
		WIFI_LOGI("register wifi pm low voltage cb fail!\n");

	return BK_OK;
}

//TODO
// 1. Free all WiFi resource
// 2. Delete all WiFi related task gracefully
bk_err_t bk_wifi_deinit(void)
{
	int ret = BK_OK;
	WIFI_LOGD("wifi deiniting\n");
	ret = wifi_deinit();

	//TODO set the init flag according to the return value!
	wifi_clear_state_bit(WIFI_INIT_BIT);
	WIFI_LOGD("wifi deinited(%x) ret(%x)\n", s_wifi_state_bits, ret);
	return BK_ERR_NOT_SUPPORT;
}

static inline int wifi_sta_init_rw_driver(void)
{
	//TODO
	// 1. sa_station_init and sa_ap_init can merge
	// 2. sa_station_init() should be called in supplicant
	sa_station_init(); //TODO change to better name, refactor SA module
	return BK_OK;
}

static inline int wifi_ap_init_rw_driver(void)
{
	BK_RETURN_ON_ERR(sa_ap_init());
	return BK_OK;
}

static inline void wifi_sta_init_callback(void)
{
	if (!(bk_feature_close_coexist_csa() || close_coexist_csa))
		bk_wlan_reg_csa_cb_coexist_mode(); //TODO why need this???
	wifi_sta_reg_bcn_cb();
}

static inline int wifi_supplicant_start(void)
{
	//TODO
	return wlan_sta_enable();
}

void _wifi_sta_exit(void)
{
	wifi_linkstate_reason_t info;
	//TODO why STA needs to care about this???
#if CONFIG_AP_IDLE
	if (bk_wlan_has_role(VIF_AP) && ap_ps_enable_get())
		stop_global_ap_bcn_timer();
#endif
	info.state = WIFI_LINKSTATE_STA_IDLE;
	info.reason_code = WIFI_REASON_MAX;
	mhdr_set_station_status(info);

	//bk_wifi_sta_disconnect();
#if CONFIG_LWIP
	//sta_ip_down();
#endif

	wlan_sta_disable();

	//TODO why STA needs to care about this???
	if (bk_wlan_has_role(VIF_AP))
		g_wlan_general_param->role = CONFIG_ROLE_AP;

	return;
}

#if defined(CONFIG_FAST_CONNECT_INFO_ENC_METHOD) && (CONFIG_FAST_CONNECT_INFO_ENC_METHOD == ENC_METHOD_XOR)
static void fc_info_enc(struct wlan_fast_connect_info *fci)
{
	struct wlan_fast_connect_info tmp_out;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	xor_enc(fci, &tmp_out, sizeof(tmp_out));
	os_memcpy(fci, &tmp_out, sizeof(tmp_out));
	GLOBAL_INT_RESTORE();
}

static void fc_info_dec(struct wlan_fast_connect_info *fci)
{
	struct wlan_fast_connect_info tmp_out;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	xor_dec(fci, &tmp_out, sizeof(tmp_out));
	os_memcpy(fci, &tmp_out, sizeof(tmp_out));
	GLOBAL_INT_RESTORE();
}
#elif defined(CONFIG_FAST_CONNECT_INFO_ENC_METHOD) && (CONFIG_FAST_CONNECT_INFO_ENC_METHOD == ENC_METHOD_AES)
static void fc_info_enc(struct wlan_fast_connect_info *fci)
{
	struct wlan_fast_connect_info tmp_out;
	weeny_aes_context ctx;
	uint8_t iv[BK_AES_IV_LEN + 1];
	uint8_t private_key[BK_AES_KEY_LEN + 1];
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	memcpy(iv, BK_TINY_AES_IV, strlen(BK_TINY_AES_IV));
	iv[sizeof(iv) - 1] = '\0';
	memcpy(private_key, BK_TINY_AES_KEY, strlen(BK_TINY_AES_KEY));
	private_key[sizeof(private_key) - 1] = '\0';

	memset(&tmp_out, 0x0, sizeof(tmp_out));
	weeny_aes_setkey_enc(&ctx, (uint8_t *) private_key, 256);
	weeny_aes_crypt_cbc(&ctx, AES_ENCRYPT, sizeof(tmp_out), iv, (unsigned char *)fci, (unsigned char *)&tmp_out);
	os_memcpy(fci, &tmp_out, sizeof(tmp_out));
	GLOBAL_INT_RESTORE();
}

static void fc_info_dec(struct wlan_fast_connect_info *fci)
{
	struct wlan_fast_connect_info tmp_out;
	weeny_aes_context ctx;
	uint8_t iv[BK_AES_IV_LEN + 1];
	uint8_t private_key[BK_AES_KEY_LEN + 1];
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	memcpy(iv, BK_TINY_AES_IV, strlen(BK_TINY_AES_IV));
	iv[sizeof(iv) - 1] = '\0';
	memcpy(private_key, BK_TINY_AES_KEY, strlen(BK_TINY_AES_KEY));
	private_key[sizeof(private_key) - 1] = '\0';

	memset(&tmp_out, 0x0, sizeof(tmp_out));
	weeny_aes_setkey_dec(&ctx, (uint8_t *) private_key, 256);
	weeny_aes_crypt_cbc(&ctx, AES_DECRYPT, sizeof(tmp_out), iv, (unsigned char *)fci, (unsigned char *)&tmp_out);
	os_memcpy(fci, &tmp_out, sizeof(tmp_out));
	GLOBAL_INT_RESTORE();
}
#endif
bool g_is_get_fci_from_flash = true;
bool g_is_use_fci_from_flash = true;
struct wlan_fast_connect_info g_fci;
void wlan_read_fast_connect_info(struct wlan_fast_connect_info *fci)
{
	/* read fast connect info from flash */
	if(g_is_get_fci_from_flash && g_is_use_fci_from_flash)
		get_net_info(FAST_CONNECT_ITEM, (UINT8 *)fci, NULL, NULL);
	else
		os_memcpy((UINT8 *)fci, &g_fci, sizeof(struct wlan_fast_connect_info));


#if defined(CONFIG_FAST_CONNECT_INFO_ENC_METHOD) && (CONFIG_FAST_CONNECT_INFO_ENC_METHOD != ENC_METHOD_NULL)
	/* decrypt fast connect info */
	fc_info_dec(fci);
#endif

	WIFI_LOGD("read flash successfully\n");
}

void wlan_write_fast_connect_info(struct wlan_fast_connect_info *fci)
{
	struct wlan_fast_connect_info pre_fci;

	/* obtain the previous bssid info*/
	wlan_read_fast_connect_info(&pre_fci);

	/* if different, save the latest information about fast connection*/
	if (!os_memcmp(&pre_fci, fci, sizeof(*fci)))
		goto wr_exit;

#if defined(CONFIG_FAST_CONNECT_INFO_ENC_METHOD) && (CONFIG_FAST_CONNECT_INFO_ENC_METHOD != ENC_METHOD_NULL)
	/* encrypt fast connect info */
	fc_info_enc(fci);
#endif

	/* save encrypted or plain fast connect info to flash */
	if(g_is_get_fci_from_flash && g_is_use_fci_from_flash)
		save_net_info(FAST_CONNECT_ITEM, (UINT8 *)fci, NULL, NULL);
	else
		os_memcpy(&g_fci, (UINT8 *)fci, sizeof(struct wlan_fast_connect_info));

	WIFI_LOGD("writed fci to flash\n");
wr_exit:
	return;
}

void wlan_clear_fast_connect_info(struct wlan_fast_connect_info *fci)
{
	g_sta_param_ptr->fast_connect_set = 0;
	if(fci){
		if(g_is_get_fci_from_flash && g_is_use_fci_from_flash)
		{
			os_memset(fci, 0, sizeof(struct wlan_fast_connect_info));
			save_net_info(FAST_CONNECT_ITEM, (UINT8 *)fci, NULL, NULL);
		}
		else
		{
			os_memset(&g_fci, 0, sizeof(struct wlan_fast_connect_info));
		}
		WIFI_LOGD("clear fast connect info\n");
	}
	if (bk_feature_fast_dhcp_enable()) {
		sta_ip_mode_set(1);
	}
}


// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
/**
* Send key to mac for sending encrypted deauth frame to AP after reboot or disconnect without deauth exchange.
*
* @encrypt   deauth frame needs to be encrypted or not
* @bssid     BSSID
* @tk        TK if frame needs to be encrypted
* @is_sta_start        station type start
*/
static void wlan_mfp_connect_deauth(bool encrypt, uint8_t *bssid, uint8_t *tk, bool is_sta_start)
{
	static bool is_first_sta_after_reboot = false;
	uint8_t pn[8] = {0};
	struct mac_sec_key key;
	uint8_t vif_idx = INVALID_VIF_IDX;
	struct vif_info_tag *vif;
	bool is_first_start = false;

	// Is first station start after reboot
	if(is_sta_start && !is_first_sta_after_reboot) {
		is_first_start = true;
		is_first_sta_after_reboot = true;
	}

	// Find first vif index that frame is going to send to.
	for(uint8_t i = 0;i<NX_VIRT_DEV_MAX;i++) {
		vif = mac_vif_mgmt_get_entry(i);
		if (mac_vif_mgmt_get_type(vif) == VIF_STA) {
			vif_idx = mac_vif_mgmt_get_index(vif);
			break;
		}
	}

	// NO invalid STA VIF
	if (vif_idx == INVALID_VIF_IDX) {
		return;
	}

	if (encrypt) {
		// Large enough PN
		os_memset(pn, 0xFF, sizeof(pn));
		pn[3] = 0x20;   // eiv, key idx = 0

		// Set Pairwise key (only CCMP)
		os_memcpy(key.array, tk, 16);
		key.length = 16;
	}
	else {
		// If not support mfp, do not need send msg to mac
		return;
	}

	extern int rw_msg_mfp_connect_deauth(u8 vif_idx,u8 *bssid,bool encrypt,u8 *pn,u16 seq,
												struct mac_sec_key *key, bool is_first_connect);
	// send key to mac
	rw_msg_mfp_connect_deauth(vif_idx,bssid,encrypt,pn,0xfff,&key,is_first_start);
}
// #endif

void bk_wlan_ap_init_adv(network_InitTypeDef_ap_st *inNetworkInitParaAP)
{
	if (!g_ap_param_ptr) {
		g_ap_param_ptr = (ap_param_t *)os_zalloc(sizeof(ap_param_t));
		BK_ASSERT(g_ap_param_ptr); /* ASSERT VERIFIED */
	}

	if (is_zero_ether_addr((u8 *)&g_ap_param_ptr->bssid))
		bk_wifi_ap_get_mac((uint8_t *)&g_ap_param_ptr->bssid);

	if (!g_wlan_general_param) {
		g_wlan_general_param = (general_param_t *)os_zalloc(sizeof(general_param_t));
		BK_ASSERT(g_wlan_general_param); /* ASSERT VERIFIED */
	}
	g_wlan_general_param->role = CONFIG_ROLE_AP;
	bk_wlan_set_coexist_at_init_phase(CONFIG_ROLE_AP);

	if (inNetworkInitParaAP) {
		if (0 == inNetworkInitParaAP->channel)
			g_ap_param_ptr->chann = bk_wlan_ap_get_default_channel();
		else
			g_ap_param_ptr->chann = inNetworkInitParaAP->channel;

		g_ap_param_ptr->ssid.length = MIN(SSID_MAX_LEN, os_strlen(inNetworkInitParaAP->wifi_ssid));
		os_memcpy(g_ap_param_ptr->ssid.array, inNetworkInitParaAP->wifi_ssid, g_ap_param_ptr->ssid.length);
		g_ap_param_ptr->key_len = os_strlen(inNetworkInitParaAP->wifi_key);
		os_memcpy(g_ap_param_ptr->key, inNetworkInitParaAP->wifi_key, g_ap_param_ptr->key_len);

		g_ap_param_ptr->cipher_suite = inNetworkInitParaAP->security;
	}

	sa_ap_init();
}

wifi_connect_tick_t sta_tick = {0};
//TODO check return value
bk_err_t bk_wifi_sta_start(void)
{
	wifi_sta_config_t sta_config = { 0 };
	size_t psk_len = 0;
	u8 *psk = 0;
	int ret = 0;

#if CONFIG_STA_AUTO_RECONNECT
	wlan_auto_reconnect_t ar;
#endif
	bool fast_connect __maybe_unused = false;
	struct wlan_fast_connect_info fci;

	WIFI_LOGD("sta starting\n");
	sta_tick.sta_start_tick = rtos_get_time();
	if (wifi_monitor_is_started()) {
		WIFI_LOGD("sta start fail, monitor in progress\n");
		return BK_ERR_WIFI_MONITOR_IP;
	}
	if (g_is_deepsleep)
	{
		WIFI_LOGD("sta start fail, deepsleep in progress\n");
		return BK_ERR_WIFI_NOT_INIT;
	}

	if (!wifi_sta_is_configured()) {
		WIFI_LOGD("sta start fail, sta not configured\n");
		return BK_ERR_WIFI_STA_NOT_CONFIG;
	}

	wifi_sta_init_global_config();

	// enable softap's channel to follow STA's channel
	wifi_sta_init_callback();

	wifi_sta_get_global_config(&sta_config);

#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	if (sta_config.psk_calculated) {
		psk_len = sta_config.psk_len;
		psk = sta_config.psk;
	}
#endif

	if (bk_feature_fast_connect_enable()) {
		int ssid_len, req_ssid_len;

		os_memset(&fci, 0, sizeof(fci));
#if CONFIG_EASY_FLASH_FAST_CONNECT
		bk_get_env_enhance("fast_connect_id", (void *)&fci, sizeof(struct wlan_fast_connect_info));
#endif

		ssid_len = os_strlen((char *)fci.ssid);
		if (ssid_len > SSID_MAX_LEN)
			ssid_len = SSID_MAX_LEN;

		req_ssid_len = os_strlen(sta_config.ssid);
		if (req_ssid_len > SSID_MAX_LEN)
			req_ssid_len = SSID_MAX_LEN;

		if (((ssid_len == req_ssid_len &&
			os_memcmp(sta_config.ssid, fci.ssid, ssid_len) == 0) ||
			(os_memcmp(sta_config.bssid, fci.bssid, 6) == 0)) &&
			((os_strcmp(sta_config.password, (char *)fci.pwd) == 0) ||
			(os_strcmp(sta_config.password, (char *)fci.psk) == 0))) {
			fast_connect = true;
			psk = fci.psk;
			psk_len = PMK_LEN * 2;
#if CONFIG_FAST_CONNECT_RESTORE_CHAN
			g_sta_param_ptr->fast_connect.chann = fci.channel;
#endif
			g_sta_param_ptr->fast_connect_set = 1;
			WIFI_LOGD("fast_connect\n");
			if (os_strlen((char *)psk) == 0) {
				/* no psk info, calcuate pmk */
				psk = 0;
				psk_len = 0;
			}
#if CONFIG_LWIP
			if (bk_feature_fast_dhcp_enable()) {
				if ((fci.ip_addr[0] != 0) && (fci.ip_addr[0] != 0xFF))
#if CONFIG_STA_USE_STATIC_IP
					sta_ip_mode_set(0);
#else
					sta_ip_mode_set(2);
#endif
				else
					sta_ip_mode_set(1);
			}
#endif
			// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
#ifndef CONFIG_FUZZ_TEST
			/* Send deauth frames to AP, if sta disconnect with the AP without Deauth or Disconnect.*/
			wlan_mfp_connect_deauth(!!(fci.pmf == MGMT_FRAME_PROTECTION_REQUIRED),fci.bssid, fci.tk,false);
#endif
			// #endif
		} else {
#if CONFIG_LWIP
			sta_ip_mode_set(1);
#endif
		}
	}

	if (wifi_sta_is_started()) {
		WIFI_LOGD("sta already started, ignored!\n");
		return BK_OK;
	}

#if CONFIG_STA_AUTO_RECONNECT
	/*
	 * let supplicant know we will reconnect after disconnect, so supplicant will not post
	 * DISCONNECT EVENT if we are connecting to another ssid(can be the same ssid).
	 */
	wpa_ctrl_request(WPA_CTRL_CMD_WPAS_SET, (void *)true);
#endif
	bk_wifi_sta_disconnect();
#if CONFIG_STA_AUTO_RECONNECT
	wpa_ctrl_request(WPA_CTRL_CMD_WPAS_SET, (void *)false);
#endif

	wifi_sta_init_rw_driver();
	if (wifi_supplicant_start()) {
		WIFI_LOGE("%s wifi enable fail\n");
		return BK_OK;
	}

#if CONFIG_STA_VSIE
	if (bk_feature_sta_vsie_enable()) {
		for (int i = 0; i < NUM_WIFI_VENDOR_ELEM_FRAMES; i++) {
			if (g_sta_param_ptr->vsies[i] ) {
				if (g_sta_param_ptr->vsies[i]->len > 0)
					bk_wifi_sta_add_vendor_ie(i, g_sta_param_ptr->vsies[i]->buf,
						g_sta_param_ptr->vsies[i]->len);
			}
		}
	}
#endif

// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
#ifndef CONFIG_FUZZ_TEST
	/* Send deauth frames to AP, if sta disconnect with the AP without Deauth or Disconnect.*/
	if (fast_connect){
		wlan_mfp_connect_deauth(!!(fci.pmf == MGMT_FRAME_PROTECTION_REQUIRED),fci.bssid, fci.tk,true);
	}
#endif
// #endif

	/* set network parameters: ssid, passphase */
	ret = wlan_sta_set((uint8_t *)sta_config.ssid, os_strlen(sta_config.ssid), (uint8_t *)sta_config.password);
	if (ret != BK_OK) {
		WIFI_LOGE("wlan_sta_set fail, ret = %d\n", ret);
		return BK_FAIL;
	}

	/*
	 * let wpa_psk_cal thread to caculate psk.
	 * XXX: If you know psk value, fill last two parameters of `wpa_psk_request()'.
	 */
	wpa_psk_request(g_sta_param_ptr->ssid.array, g_sta_param_ptr->ssid.length,
						(char *)g_sta_param_ptr->key, psk, psk_len);

#if CONFIG_STA_AUTO_RECONNECT
	/* set auto reconnect parameters */
	ar.max_count = g_sta_param_ptr->auto_reconnect_count;
	ar.timeout = g_sta_param_ptr->auto_reconnect_timeout;
	ar.disable_reconnect_when_disconnect = g_sta_param_ptr->disable_auto_reconnect_after_disconnect;
	wlan_sta_set_autoreconnect(&ar);
#endif

	wifi_set_state_bit(WIFI_STA_STARTED_BIT);
	WIFI_LOGD("sta started(%x)\n", s_wifi_state_bits);

	/* always connect the AP automatically */
	bk_wifi_sta_connect();

	return BK_OK;
}

bk_err_t bk_wifi_sta_stop(void)
{
	WIFI_LOGD("sta stopping\n");

	if (!wifi_sta_is_started()) {
		WIFI_LOGD("sta stop, already stopped\n");
		return BK_OK;
	}
	_wifi_sta_exit();

	wifi_clear_state_bit(WIFI_STA_STARTED_BIT);
	WIFI_LOGD("sta stopped(%x)\n", s_wifi_state_bits);
	return BK_OK;
}

void bk_wifi_set_video_transfer_state(bool enable)
{
	if (enable)
		wifi_set_state_bit(WIFI_VIDEO_TRANSFER_STARTED_BIT);
	else
		wifi_clear_state_bit(WIFI_VIDEO_TRANSFER_STARTED_BIT);
}

void rwnxl_prepare_deepsleep(void);
void bk_wifi_prepare_deepsleep(void)
{
	rwnxl_prepare_deepsleep();
	if (!wifi_sta_is_started()) {
	}
}

static inline bool wifi_video_transfer_is_started(void)
{
	return (bool)(s_wifi_state_bits & WIFI_VIDEO_TRANSFER_STARTED_BIT);
}

bk_err_t bk_wifi_scan_start(const wifi_scan_config_t *config)
{
	int ret;

	WIFI_LOGD("scaning\n");

	if (!wifi_is_inited()) {
		WIFI_LOGD("start scan fail, wifi not init\n");
		return BK_ERR_WIFI_NOT_INIT;
	}

	if (wifi_video_transfer_is_started()) {
		WIFI_LOGD("scan refused, video transfer in progress\n");
		return BK_ERR_BUSY;
	}

	if (wifi_monitor_is_started()) {
		WIFI_LOGD("scan refused, monitor in progress\n");
		return BK_ERR_WIFI_MONITOR_IP;
	}

	wifi_scan_init_global_config();
	wifi_sta_init_rw_driver();
	if (wifi_supplicant_start()) {
		WIFI_LOGE("%s wifi enable fail\n");
		return BK_OK;
	}

	wifi_set_state_bit(WIFI_PURE_SCAN_STARTED_BIT);

	/* flush old scan results */
	wlan_sta_bss_flush(0);

	wlan_sta_scan_param_t scan_param = {0};
	os_memset(&scan_param, 0, sizeof(wlan_sta_scan_param_t));
	scan_param.id = (uint32_t)rtos_get_current_thread();

	if (config) {
		scan_param.scan_cc = !!(config->flag & SCAN_TYPE_CC);

		if((0 != config->scan_type) ||(0 != config->chan_cnt) ||(0 != config->duration) || (0 != config->scan_band)) {
			scan_param_env.set_param = 1;
			scan_param_env.scan_type = config->scan_type;
			scan_param_env.scan_band = config->scan_band;
			scan_param_env.chan_cnt = config->chan_cnt;
			if(WIFI_MAX_SCAN_CHAN_DUR < config->duration) {
				WIFI_LOGW("scan duration is too long %dus,need less than 200ms\r\n",config->duration);
				scan_param_env.duration = WIFI_MAX_SCAN_CHAN_DUR * 1000;
			} else
				scan_param_env.duration = config->duration * 1000;
			os_memcpy(scan_param_env.chan_nb, config->chan_nb, config->chan_cnt);
		}
	}

	if (0 == config->ssid_cnt) {
		WIFI_LOGI("scan all APs\n");
		scan_param.num_ssids = 1;
		scan_param.ssids[0].ssid_len = 0;
	} else {
		WIFI_LOGI("scan_cnt %d, ssid %s\n", config->ssid_cnt, config->ssid[0]);
		scan_param.num_ssids = config->ssid_cnt;
		for (int i =0; i < scan_param.num_ssids; i ++)
		{
			scan_param.ssids[i].ssid_len = MIN(WLAN_SSID_MAX_LEN, os_strlen((char *)config->ssid[i]));
			os_memcpy(scan_param.ssids[i].ssid, config->ssid[i], scan_param.ssids[i].ssid_len);
		}
	}
	ret = wlan_sta_scan(&scan_param);

	if (ret != BK_OK) {
		if (wlan_sta_scan_need_retry(ret)) {
			WIFI_LOGW("scan failed(%x), just need retry\r\n", ret);
		} else {
			WIFI_LOGE("scan failed(%x)!\r\n", ret);
			return ret;
		}
	}

	return BK_OK;
}

bk_err_t bk_wifi_scan_stop(void)
{
	WIFI_LOGD("scan stopping\n");

	if (!wifi_pure_scan_is_started()) {
		WIFI_LOGD("sta stop, already stopped\n");
		return BK_OK;
	}

	_wifi_sta_exit();

	wifi_clear_state_bit(WIFI_PURE_SCAN_STARTED_BIT);
	WIFI_LOGD("sta stopped(%x)\n", s_wifi_state_bits);
	return BK_OK;
}

static bool is_reserved_byte_init(const uint8_t *reserved_bytes, uint32_t len)
{
	for (int i = 0; i < len; i++) {
		if (reserved_bytes[i] != WIFI_RESERVED_BYTE_VALUE) {
			WIFI_LOGD("reserved byte not correct, %x\n", reserved_bytes[i]);
			return false;
		}
	}

	return true;
}

static int wifi_sta_validate_config(const wifi_sta_config_t *config)
{
	if (!config) {
		WIFI_LOGD("sta config fail, null config\n");
		return BK_ERR_NULL_PARAM;
	}

	if (is_reserved_byte_init(config->reserved, sizeof(config->reserved)) == false)
		return BK_ERR_WIFI_RESERVED_FIELD;

	if(config->is_user_fast_connect)
	{
		g_is_get_fci_from_flash = false;
		os_memcpy(g_fci.ssid, config->ssid, WIFI_SSID_STR_LEN);
		os_memcpy(g_fci.bssid, config->bssid, WIFI_BSSID_LEN);
		os_memcpy(g_fci.pwd, config->password, WIFI_PASSWORD_LEN);
		os_memcpy(g_fci.psk, config->psk, WIFI_PASSWORD_LEN);
		os_memcpy(g_fci.ip_addr, config->ip_addr, 4);
		os_memcpy(g_fci.netmask, config->netmask, 4);
		os_memcpy(g_fci.gw, config->gw, 4);
		os_memcpy(g_fci.dns1, config->dns1, 4);
		os_memcpy(g_fci.tk, config->tk, 16);
		g_fci.channel = config->channel;
		g_fci.pmf = config->pmf;
		g_fci.security = config->security;
	}
	else
	{
		g_is_get_fci_from_flash = true;
	}

	if(config->is_not_support_auto_fci)
	{
		g_is_use_fci_from_flash = false;
		if(g_is_get_fci_from_flash)
			os_memset(&g_fci, 0, sizeof(struct wlan_fast_connect_info));
	}
	else
	{
		g_is_use_fci_from_flash = true;
	}
	//TODO more check
	return BK_OK;
}

//TODO optimize param_config.c
//Init global STA configurations
static int wifi_sta_init_global_config(void)
{
	BK_ASSERT(g_sta_param_ptr); /* ASSERT VERIFIED */
	BK_ASSERT(g_wlan_general_param); /* ASSERT VERIFIED */

	bk_wifi_sta_get_mac((uint8_t *)(&g_sta_param_ptr->own_mac));
	g_wlan_general_param->role = CONFIG_ROLE_STA;
	bk_wlan_set_coexist_at_init_phase(CONFIG_ROLE_STA); //TODO optimize it
	return BK_OK;
}

static int wifi_scan_init_global_config(void)
{
	BK_ASSERT(g_sta_param_ptr); /* ASSERT VERIFIED */
	BK_ASSERT(g_wlan_general_param); /* ASSERT VERIFIED */

	bk_wifi_sta_get_mac((uint8_t *)(&g_sta_param_ptr->own_mac));

	return BK_OK;
}

//TODO param_config.c
//Set STA configuration to global configuration
static int wifi_sta_set_global_config(const wifi_sta_config_t *config)
{
	g_sta_param_ptr->ssid.length = MIN(SSID_MAX_LEN, os_strlen(config->ssid));
	os_memcpy(g_sta_param_ptr->ssid.array, config->ssid, g_sta_param_ptr->ssid.length);

	if (bk_feature_bssid_connect_enable()) {
		os_memcpy(g_sta_param_ptr->fast_connect.bssid, config->bssid, sizeof(config->bssid));
		g_sta_param_ptr->fast_connect.chann = config->channel;
	}

	g_sta_param_ptr->key_len = os_strlen(config->password);
	os_memcpy(g_sta_param_ptr->key, config->password, g_sta_param_ptr->key_len);
	g_sta_param_ptr->key[g_sta_param_ptr->key_len] = 0;

#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	g_sta_param_ptr->psk_len = config->psk_len;
	g_sta_param_ptr->psk_calculated = config->psk_calculated;
	os_strlcpy((char *)g_sta_param_ptr->psk, (char *)config->psk, sizeof(g_sta_param_ptr->psk));
#endif

#if CONFIG_STA_VSIE
	if (bk_feature_sta_vsie_enable()) {
		for (int i = 0; i < NUM_WIFI_VENDOR_ELEM_FRAMES; i++) {
			if (g_sta_param_ptr->vsies[i] && config->vsies[i])
				os_memcpy(g_sta_param_ptr->vsies[i], config->vsies[i], sizeof(struct bk_vise));
		}
	}
#endif

#if CONFIG_WPA2_ENTERPRISE
	os_strlcpy(g_sta_param_ptr->eap, config->eap, sizeof(g_sta_param_ptr->eap));
	os_strlcpy(g_sta_param_ptr->identity, config->identity, sizeof(g_sta_param_ptr->identity));
	os_strlcpy(g_sta_param_ptr->ca, config->ca, sizeof(g_sta_param_ptr->ca));
	os_strlcpy(g_sta_param_ptr->client_cert, config->client_cert, sizeof(g_sta_param_ptr->client_cert));
	os_strlcpy(g_sta_param_ptr->private_key, config->private_key, sizeof(g_sta_param_ptr->private_key));
	os_strlcpy(g_sta_param_ptr->private_key_passwd, config->private_key_passwd,
			sizeof(g_sta_param_ptr->private_key_passwd));
	os_strlcpy(g_sta_param_ptr->phase1, config->phase1, sizeof(g_sta_param_ptr->phase1));
#endif

#if CONFIG_STA_AUTO_RECONNECT
	g_sta_param_ptr->auto_reconnect_count = config->auto_reconnect_count;
	g_sta_param_ptr->auto_reconnect_timeout = config->auto_reconnect_timeout;
	g_sta_param_ptr->disable_auto_reconnect_after_disconnect =
			config->disable_auto_reconnect_after_disconnect;
#endif

	WIFI_LOGD("sta config, ssid=%s password=%s security=%d\n",
			  config->ssid, config->password, config->security);
	return BK_OK;
}

static int wifi_sta_get_global_config(wifi_sta_config_t *sta_config)
{
	if (!sta_config)
		return BK_ERR_NULL_PARAM;

	os_memset(sta_config, 0, sizeof(sta_config));
	os_memcpy(sta_config->ssid, g_sta_param_ptr->ssid.array, g_sta_param_ptr->ssid.length);

#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	sta_config->psk_len = g_sta_param_ptr->psk_len;
	sta_config->psk_calculated = g_sta_param_ptr->psk_calculated;
	os_strlcpy((char *)sta_config->psk, (char *)g_sta_param_ptr->psk, sizeof(sta_config->psk));
#endif

	if (bk_feature_bssid_connect_enable()) {
		os_memcpy(sta_config->bssid, g_sta_param_ptr->fast_connect.bssid, sizeof(sta_config->bssid));
		sta_config->channel = g_sta_param_ptr->fast_connect.chann;
	}

	os_memcpy(sta_config->password, g_sta_param_ptr->key, g_sta_param_ptr->key_len);

#if CONFIG_STA_VSIE
	if (bk_feature_sta_vsie_enable()) {
		for (int i = 0; i < NUM_WIFI_VENDOR_ELEM_FRAMES; i++) {
			if (g_sta_param_ptr->vsies[i] && sta_config->vsies[i])
				os_memcpy(sta_config->vsies[i], g_sta_param_ptr->vsies[i], sizeof(struct bk_vise));
		}
	}
#endif

#if CONFIG_WPA2_ENTERPRISE
	os_strlcpy(sta_config->eap, g_sta_param_ptr->eap, sizeof(sta_config->eap));
	os_strlcpy(sta_config->identity, g_sta_param_ptr->identity, sizeof(sta_config->identity));
	os_strlcpy(sta_config->ca, g_sta_param_ptr->ca, sizeof(sta_config->ca));
	os_strlcpy(sta_config->client_cert, g_sta_param_ptr->client_cert, sizeof(sta_config->client_cert));
	os_strlcpy(sta_config->private_key, g_sta_param_ptr->private_key, sizeof(sta_config->private_key));
	os_strlcpy(sta_config->private_key_passwd, g_sta_param_ptr->private_key_passwd,
			sizeof(sta_config->private_key_passwd));
	os_strlcpy(sta_config->phase1, g_sta_param_ptr->phase1, sizeof(sta_config->phase1));
#endif


	WIFI_LOGD("sta get sta_config, ssid=%s password=%s security=%d\n",
			  sta_config->ssid, sta_config->password, sta_config->security);

	return BK_OK;
}

static bool bssid_is_initialized(const uint8_t *bssid)
{
	for (int i = 0; i < WIFI_BSSID_LEN; i++) {
		if (bssid[i] != 0)
			return true;
	}

	return false;
}

static int wifi_sta_set_bssid_wpa_config(const wifi_sta_config_t *config)
{
	wlan_sta_config_t wpa_config = {0};
	WPA_LOGD("fast connect, bssid=%pm channel=%d\n",
			  config->bssid, config->channel);
	if (bssid_is_initialized(config->bssid)) {
		/* set fast connect bssid */
		os_memset(&wpa_config, 0, sizeof(wpa_config));
		os_memcpy(wpa_config.u.bssid, config->bssid, ETH_ALEN);
		wpa_config.field = WLAN_STA_FIELD_BSSID;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}
	if (config->channel != 0) {
		/* set fast connect freq */
		os_memset(&wpa_config, 0, sizeof(wpa_config));
		wpa_config.u.channel = config->channel;
		wpa_config.field = WLAN_STA_FIELD_FREQ;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}
	return BK_OK;
}

static int wifi_sta_set_wpa_config(const wifi_sta_config_t *config)
{
#if  CONFIG_WPA2_ENTERPRISE
	wlan_sta_config_t wpa_config = {0};
#endif
#if CONFIG_STA_VSIE
	int i;
#endif

#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	if (config->psk_calculated == true) {
		u8 *psk = (u8 *)config->psk;
		wpa_psk_request(g_sta_param_ptr->ssid.array, g_sta_param_ptr->ssid.length,
								(char *)g_sta_param_ptr->key, psk, config->psk_len);
	}
	else
#endif
	wpa_psk_request(g_sta_param_ptr->ssid.array, g_sta_param_ptr->ssid.length,
						(char *)g_sta_param_ptr->key, NULL, 0);

	wlan_sta_set((uint8_t *)config->ssid, os_strlen(config->ssid), (uint8_t *)config->password);

#if CONFIG_STA_VSIE
	if (bk_feature_sta_vsie_enable()) {
		for (i = 0; i < NUM_WIFI_VENDOR_ELEM_FRAMES; i++) {
			if (config->vsies[i] == NULL)
				continue;
			if (config->vsies[i]->len) {
				wlan_sta_vsie_t vsie;

				/* set STA vendor IE via ctrl iface */
				switch (i) {
				case WIFI_VENDOR_ELEM_ASSOC_REQ:
					vsie.frame = VENDOR_ELEM_ASSOC_REQ;
					break;
				case WIFI_VENDOR_ELEM_PROBE_REQ:
					vsie.frame = VENDOR_ELEM_PROBE_REQ;
					break;
				default:
					continue;
				}
				vsie.len = config->vsies[i]->len;
				os_memcpy(vsie.vsie, config->vsies[i]->buf, vsie.len);
				wlan_sta_set_vendor_ie(&vsie);
			}
		}
	}
#endif

#if CONFIG_WPA2_ENTERPRISE
	if (config->ca[0]) {
		/* set CA certificate */
		os_strcpy(wpa_config.u.ca_cert, config->ca);
		wpa_config.field = WLAN_STA_FIELD_CA_CERT;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}

	if (config->identity[0]) {
		os_strcpy(wpa_config.u.identity, config->identity);
		wpa_config.field = WLAN_STA_FIELD_IDENTITY;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}

	if (config->eap[0]) {
		os_strcpy(wpa_config.u.eap, config->eap);
		wpa_config.field = WLAN_STA_FIELD_EAP;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}

	if (config->client_cert[0]) {
		os_strcpy(wpa_config.u.client_cert, config->client_cert);
		wpa_config.field = WLAN_STA_FIELD_CLIENT_CERT;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}

	if (config->private_key[0]) {
		os_strcpy(wpa_config.u.private_key, config->private_key);
		wpa_config.field = WLAN_STA_FIELD_PRIVATE_KEY;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}

	if (config->private_key_passwd[0]) {
		os_strcpy(wpa_config.u.private_key_passwd, config->private_key_passwd);
		wpa_config.field = WLAN_STA_FIELD_PRIVATE_KEY_PASSWD;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}
	if (config->phase1[0]) {
		os_strcpy(wpa_config.u.phase1, config->phase1);
		wpa_config.field = WLAN_STA_FIELD_PHASE1;
		wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
	}
#endif

	if (bk_feature_bssid_connect_enable()) {
		wifi_sta_set_bssid_wpa_config(config);
	}
	return BK_OK;
}

bk_err_t bk_wifi_sta_set_config(const wifi_sta_config_t *config)
{
	bool need_reconnect = false;
	int ret = BK_OK;

	WIFI_LOGD("sta configuring\n");

	if (!wifi_is_inited()) {
		WIFI_LOGD("set sta config fail, wifi not init\n");
		return BK_ERR_WIFI_NOT_INIT;
	}

	ret = wifi_sta_validate_config(config);
	if (ret != BK_OK) {
		WIFI_LOGD("set config fail, invalid param\n");
		return ret;
	}

	if (wifi_sta_is_connected()) {
#if CONFIG_STA_AUTO_RECONNECT
		/*
		 * let supplicant know we will reconnect after disconnect, so supplicant will not post
		 * DISCONNECT EVENT if we are connecting to another ssid(can be the same ssid).
		 */
		wpa_ctrl_request(WPA_CTRL_CMD_WPAS_SET, (void *)true);
#endif
		bk_wifi_sta_disconnect();
#if CONFIG_STA_AUTO_RECONNECT
		wpa_ctrl_request(WPA_CTRL_CMD_WPAS_SET, (void *)false);
#endif
		need_reconnect = true;
	}

	wifi_sta_set_global_config(config);
	if (wifi_sta_is_started())
		wifi_sta_set_wpa_config(config);

	wifi_set_state_bit(WIFI_STA_CONFIGURED_BIT);
	WIFI_LOGD("sta configured(%x)\n", s_wifi_state_bits);

	if (need_reconnect) {
#if CONFIG_STA_AUTO_RECONNECT
		wlan_auto_reconnect_t ar = {0};
		/* set auto reconnect parameters */
		ar.max_count = g_sta_param_ptr->auto_reconnect_count;
		ar.timeout = g_sta_param_ptr->auto_reconnect_timeout;
		ar.disable_reconnect_when_disconnect = g_sta_param_ptr->disable_auto_reconnect_after_disconnect;
		wlan_sta_set_autoreconnect(&ar);
#endif
		bk_wifi_sta_connect();
	}

	return BK_OK;
}

bk_err_t bk_wifi_sta_get_config(wifi_sta_config_t *config)
{
	if (!config) {
		WIFI_LOGD("get sta config fail, null config");
		return BK_ERR_NULL_PARAM;
	}

	os_memset(config, 0, sizeof(config));

	if (!wifi_sta_is_configured())
		return BK_ERR_WIFI_STA_NOT_CONFIG;

	os_memcpy(config->ssid, g_sta_param_ptr->ssid.array, g_sta_param_ptr->ssid.length);

	if (bk_feature_bssid_connect_enable()) {
		os_memcpy(config->bssid, g_sta_param_ptr->fast_connect.bssid, sizeof(config->bssid));
	}

	os_memcpy(config->password, g_sta_param_ptr->key, g_sta_param_ptr->key_len);
	config->password[g_sta_param_ptr->key_len] = 0;

	if(bk_feature_fast_connect_enable()){
		os_memcpy(config->bssid, g_fci.bssid, WIFI_BSSID_LEN);
		os_memcpy(config->psk, g_fci.psk, WIFI_PASSWORD_LEN);
		os_memcpy(config->ip_addr, g_fci.ip_addr, 4);
		os_memcpy(config->netmask, g_fci.netmask, 4);
		os_memcpy(config->gw, g_fci.gw, 4);
		os_memcpy(config->dns1, g_fci.dns1, 4);
		os_memcpy(config->tk, g_fci.tk, 16);
		config->channel = g_fci.channel;
		config->pmf = g_fci.pmf;
		config->security = g_fci.security;
	}

	//TODO get channel and security type
	return BK_OK;
}

bk_err_t bk_wifi_sta_get_linkstate_with_reason(wifi_linkstate_reason_t *info)
{
	wifi_linkstate_reason_t val = mhdr_get_station_status();

	if (!info)
		return BK_ERR_NULL_PARAM;

	info->state = val.state;
	info->reason_code = val.reason_code;

	return BK_OK;
}

bk_err_t bk_wifi_sta_get_link_status(wifi_link_status_t *link_status)
{
	int val, ret = BK_OK;
	u8 vif_idx = 0, ssid_len;
	struct me_get_bss_info_cfm *cfm = NULL;
	wifi_linkstate_reason_t info = mhdr_get_station_status();

	WIFI_LOGD("get sta link status\n");

	if (!link_status)
		return BK_ERR_NULL_PARAM;

	os_memset(link_status, 0, sizeof(link_status));
	link_status->aid = -1;
	link_status->rssi = -128;
	if (!wifi_sta_is_connected()) {
		link_status->state = WIFI_LINKSTATE_STA_DISCONNECTED;
		return BK_OK;
	}

	//TODO need to convert the link status of status code!!!
	link_status->state = info.state;

	vif_idx = rwm_mgmt_vif_mac2idx((void *)&g_sta_param_ptr->own_mac);
	if (INVALID_VIF_IDX == vif_idx) {
		ret = BK_ERR_WIFI_DRIVER;
		goto get_exit;
	}

	cfm = os_malloc(sizeof(struct me_get_bss_info_cfm));
	if (!cfm) {
		ret = BK_ERR_NO_MEM;
		goto get_exit;
	}

	val = rw_msg_get_bss_info(vif_idx, (void *)cfm);
	if (val) {
		ret = BK_ERR_WIFI_DRIVER;
		goto get_exit;
	}

	link_status->aid = cfm->aid;
	link_status->rssi = cfm->rssi;
	link_status->channel = rw_ieee80211_get_chan_id(cfm->freq);
	link_status->security = g_sta_param_ptr->cipher_suite;

	os_memcpy(link_status->bssid, cfm->bssid, WIFI_BSSID_LEN);
	ssid_len = MIN(SSID_MAX_LEN, os_strlen((char *)(cfm->ssid)));
	os_memcpy(link_status->ssid, cfm->ssid, ssid_len);

get_exit:
	if (cfm)
		os_free(cfm);

	return ret;
}

bk_err_t bk_wifi_sta_add_vendor_ie(uint8_t frame, uint8_t *vsie, uint8_t len)
{
	wlan_sta_vsie_t ie;

	/* frame id invalid, or len !=0 but vsie is null */
	if ((frame >= NUM_WIFI_VENDOR_ELEM_FRAMES) || (len && !vsie))
		return BK_FAIL;

	ie.len = len;
	switch (frame) {
	case WIFI_VENDOR_ELEM_ASSOC_REQ:
		ie.frame = VENDOR_ELEM_ASSOC_REQ;
		break;
	case WIFI_VENDOR_ELEM_PROBE_REQ:
		ie.frame = VENDOR_ELEM_PROBE_REQ;
		break;
	}
	if (len)
		os_memcpy(ie.vsie, vsie, len);

	return wlan_sta_set_vendor_ie(&ie);
}

bk_err_t bk_wifi_sta_update_vendor_ie(uint8_t frame, uint8_t *vsie, uint8_t len)
{
	return bk_wifi_sta_add_vendor_ie(frame, vsie, len);
}

bk_err_t bk_wifi_sta_del_vendor_ie(uint8_t frame)
{
	return bk_wifi_sta_add_vendor_ie(frame, NULL, 0);
}

/*
 * Add Beacon/ProbeResponse Vendor Specific IEs
 */
bk_err_t bk_wifi_ap_add_vendor_ie(void *buf, uint8_t len)
{
	WLAN_RETURN_NULL_POINTER(buf);

	wlan_ap_vsie_t vsie;

	vsie.len = len;
	os_memcpy(vsie.vsie, buf, len);

	return wlan_ap_set_beacon_vsie(&vsie);
}

/*
 * Delete Beacon/ProbeResponse Vendor Specific IEs
 */
bk_err_t bk_wifi_ap_del_vendor_ie(void)
{
	wlan_ap_vsie_t vsie;

	vsie.len = 0;
	return wlan_ap_set_beacon_vsie(&vsie);
}

/*
 * Update Hidden Ssid Capability Of Softap Mode
 */
bk_err_t bk_wifi_ap_update_hidden_cap(bool flag)
{
	g_ap_param_ptr->hidden_ssid = flag;
	return wlan_ap_set_beacon_hidden_cap();
}

bk_err_t bk_wifi_sta_connect(void)
{
	wifi_linkstate_reason_t info;

	WIFI_LOGD("sta connecting\n");

	if (!wifi_sta_is_started()) {
		WIFI_LOGD("sta connect fail, sta not start\n");
		return BK_ERR_WIFI_STA_NOT_STARTED;
	}

#if CONFIG_STA_AUTO_RECONNECT
	/*
	 * let supplicant know we will reconnect after disconnect, so supplicant will not post
	 * DISCONNECT EVENT if we are connecting to another ssid(can be the same ssid).
	 */
	wpa_ctrl_request(WPA_CTRL_CMD_WPAS_SET, (void *)true);
#endif
	bk_wifi_sta_disconnect();
#if CONFIG_STA_AUTO_RECONNECT
	wpa_ctrl_request(WPA_CTRL_CMD_WPAS_SET, (void *)false);
#endif

	//TODO - optimize event module
	info.state = WIFI_LINKSTATE_STA_CONNECTING;
	info.reason_code = WIFI_REASON_MAX;
	mhdr_set_station_status(info);

	//TODO - what's the meaning of this?????
#if (CONFIG_RF_POLICY_WIFI_REQ)
	wifi_station_status_event_notice(0, WIFI_LINKSTATE_STA_CONNECTING);
#endif
	//TODO

	if (bk_feature_bssid_connect_enable()) {
		wlan_sta_config_t wpa_config = {0};
		if (bssid_is_initialized(g_sta_param_ptr->fast_connect.bssid)) {
			/* set fast connect bssid */
			os_memset(&wpa_config, 0, sizeof(wpa_config));
			os_memcpy(wpa_config.u.bssid, g_sta_param_ptr->fast_connect.bssid, ETH_ALEN);
			wpa_config.field = WLAN_STA_FIELD_BSSID;
			wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &wpa_config);
		}
	}

#if CONFIG_WIFI_OCV
		if (g_sta_param_ptr->ocv) {
			wlan_sta_config_t config;
			os_memset(&config, 0, sizeof(config));
			config.field = WLAN_STA_FIELD_OCV;
			config.u.ocv = true;
			if (wpa_ctrl_request(WPA_CTRL_CMD_STA_SET, &config) != 0)
				return -1;
		}
#endif
	wlan_sta_connect(g_sta_param_ptr->fast_connect_set ? g_sta_param_ptr->fast_connect.chann : 0);
	wifi_set_state_bit(WIFI_STA_CONNECTED_BIT);
	WIFI_LOGD("sta connected(%x)\n", s_wifi_state_bits);
	return BK_OK;
}

bk_err_t bk_wifi_sta_disconnect(void)
{
	WIFI_LOGD("sta disconnecting\n");

	if (wifi_sta_is_connected()) {
#if CONFIG_LWIP
		sta_ip_down();
#endif
		wlan_sta_disconnect();

		//TODO do we need to post the disconnect event?
		wifi_clear_state_bit(WIFI_STA_CONNECTED_BIT);
	}

	WIFI_LOGD("sta disconnected(%x)\n", s_wifi_state_bits);
	return BK_OK;
}

bk_err_t bk_wifi_scan_get_result(wifi_scan_result_t *scan_result)
{
	ScanResult_adv ap_list = {0};
	wifi_scan_ap_info_t *ap;
	int ret = BK_OK;
	int j = 0;

	if (!scan_result)
		return BK_ERR_NULL_PARAM;

	ap_list.ApList = NULL;
	ret = wlan_sta_scan_result(&ap_list);
	if (BK_OK != ret) {
		WIFI_LOGW("scan get result fail, ret=%d\n", ret);
		goto _free_and_exit;
	}

	if (ap_list.ApNum == 0)
		goto _free_and_exit;

	scan_result->ap_num = 0;
	scan_result->aps = os_zalloc(sizeof(wifi_scan_ap_info_t) * ap_list.ApNum);
	if (!scan_result->aps) {
		ret = BK_ERR_NO_MEM;
		WIFI_LOGW("scan get result fail, ap_num=%u oom\n", ap_list.ApNum);
		goto _free_and_exit;
	}

	for (int i = 0; i < ap_list.ApNum; i++) {
		if ((1 == scan_param_dump_env.set_param) && (0 != scan_param_dump_env.chan_cnt)
			&& (rw_ieee80211_get_scan_default_chan_num() > scan_param_dump_env.chan_cnt)) {
			for (j = 0; j < scan_param_dump_env.chan_cnt; j++) {
				if(ap_list.ApList[i].channel == scan_param_dump_env.chan_nb[j])
					break;
			}

			if(j == scan_param_dump_env.chan_cnt) {
				WIFI_LOGD("scan get result , not print chan %d\n", ap_list.ApList[i].channel);
				continue;
			}
		}

		ap = &scan_result->aps[scan_result->ap_num];
		strncpy(ap->ssid, ap_list.ApList[i].ssid, WIFI_SSID_STR_LEN);
		os_memcpy(ap->bssid, ap_list.ApList[i].bssid, WIFI_BSSID_LEN);
		ap->channel = ap_list.ApList[i].channel;
		ap->security = ap_list.ApList[i].security;
		ap->rssi = ap_list.ApList[i].ApPower;

		scan_result->ap_num++;
	}

_free_and_exit:
	os_free(ap_list.ApList);
	return BK_OK;
}

static void wifi_scan_dump_ap(const wifi_scan_ap_info_t *ap)
{
	const char *security_str = wifi_sec_type_string(ap->security);
#if (CONFIG_SHELL_ASYNCLOG)
	shell_cmd_ind_out("%-32s " BK_MAC_FORMAT "   %4d %2d %s\r\n",
			   ap->ssid, BK_MAC_STR(ap->bssid), (int8_t)ap->rssi, ap->channel, security_str);
#else
	WIFI_LOG_RAW("%-32s " BK_MAC_FORMAT "   %4d %2d %s\n",
			   ap->ssid, BK_MAC_STR(ap->bssid), (int8_t)ap->rssi, ap->channel, security_str);
#endif
}

bk_err_t bk_wifi_scan_dump_result(const wifi_scan_result_t *scan_result)
{
	int i;

	if (!scan_result) {
#if (CONFIG_SHELL_ASYNCLOG)
		shell_cmd_ind_out("scan doesn't found AP\n");
#else
		WIFI_LOGI("scan doesn't found AP\n");
#endif
		return BK_OK;
	}

	if ((scan_result->ap_num > 0) && (!scan_result->aps)) {
		WIFI_LOGE("scan number is %d, but AP info is NULL\n", scan_result->ap_num);
		return BK_ERR_PARAM;
	}
#if (CONFIG_SHELL_ASYNCLOG)
	shell_cmd_ind_out("scan found %d AP\r\n", scan_result->ap_num);
	shell_cmd_ind_out("%32s %17s   %4s %4s %s\r\n", "              SSID              ",
			   "      BSSID      ", "RSSI", "chan", "security");
	shell_cmd_ind_out("%32s %17s   %4s %4s %s\r\n", "--------------------------------",
			   "-----------------", "----", "----", "---------\n");
#else
	WIFI_LOGI("scan found %d AP\n", scan_result->ap_num);
	WIFI_LOG_RAW("%32s %17s   %4s %4s %s\n", "              SSID              ",
			   "      BSSID      ", "RSSI", "chan", "security");
	WIFI_LOG_RAW("%32s %17s   %4s %4s %s\n", "--------------------------------",
			   "-----------------", "----", "----", "---------\n");
#endif
	for (i = 0; i < scan_result->ap_num; i++) {
		wifi_scan_dump_ap(&scan_result->aps[i]);
		rtos_delay_milliseconds(10);
	}

	//WIFI_LOG_RAW("\n");

	return BK_OK;
}

void bk_wifi_scan_free_result(wifi_scan_result_t *scan_result)
{
	if (scan_result) {
		os_free(scan_result->aps);
		scan_result->aps = 0;
		scan_result->ap_num = 0;
	}
	os_memset(&scan_param_dump_env, 0, sizeof(scan_param_dump_env));
	WIFI_LOGD("scan free result\n");
}

#if CONFIG_BRIDGE
extern uint8 bridge_is_enabled;
#endif

bk_err_t bk_wifi_ap_start(void)
{
	WIFI_LOGD("ap starting\n");

	if (!wifi_ap_is_configured()) {
		WIFI_LOGD("start ap failed, ap not configured\n");
		return BK_ERR_WIFI_AP_NOT_CONFIG;
	}

	if (wifi_ap_is_started()) {
		WIFI_LOGD("start ap, already started, ignored\n");
		return BK_OK;
	}

#if CONFIG_LWIP
	WIFI_LOGD("ap start, ip down\n");
	//TODO move to event handler
	uap_ip_down();
#endif

	WIFI_LOGD("ap start, ap start rf\n");
	if(wifi_ap_init_rw_driver()) {
		WIFI_LOGE("ap init rw driver fail!\n");
		return BK_ERR_WIFI_AP_NOT_STARTED;
	}
	//TODO return value
	if(wlan_ap_enable()) {
		WIFI_LOGE("ap enable fail!\n");
		return BK_ERR_WIFI_AP_NOT_STARTED;
	}
	if(wlan_ap_reload()) {
		WIFI_LOGE("ap reload fail!\n");
		return BK_ERR_WIFI_AP_NOT_STARTED;
	}
#if CONFIG_LWIP
	//TODO move to event handler
#if CONFIG_BRIDGE
	if (!bridge_is_enabled)
#endif
		uap_ip_start();
#endif

#if CONFIG_AP_STATYPE_LIMIT
	if (bk_feature_ap_statype_limit_enable())
		bk_vsie_cus_init();
#endif

	WIFI_LOGD("ap started\n");
	wifi_set_state_bit(WIFI_AP_STARTED_BIT);
	return BK_OK;
}

bk_err_t wifi_ap_validate_config(const wifi_ap_config_t *ap_config)
{
	if (!ap_config)
		return BK_ERR_NULL_PARAM;
#if (CONFIG_SOC_BK7239XX) && CONFIG_WIFI_BAND_5G
	if (!ate_is_enabled() && ap_config->channel >= 36 && ap_config->channel <= 177) {

#if CONFIG_WIFI_REGDOMAIN
		if (check_non_radar_channel_available(ap_config->channel))
			return BK_OK;
#else
		//check if configured channel is avaliable channel and no need for radat detection
		int selected_channels_size = 0;
		extern int* rw_select_5g_non_radar_avaliable_channels(int *selected_channels_size);
		int *non_radar_avaliable_channels = rw_select_5g_non_radar_avaliable_channels(&selected_channels_size);

		for (int i = 0; i < selected_channels_size; i++) {
			if (non_radar_avaliable_channels[i] == ap_config->channel)
				return BK_OK;
		}
#endif

		//TODO more parameter checking
		WIFI_LOGE("[%s]configured unavaliable or dfs channel\r\n",__FUNCTION__);
		return BK_ERR_NOT_FOUND;
	}
#endif
	return BK_OK;
}
static bk_err_t wifi_ap_set_config(const wifi_ap_config_t *ap_config)
{
	BK_ASSERT(g_ap_param_ptr);          /* ASSERT VERIFIED */
	BK_ASSERT(g_wlan_general_param);    /* ASSERT VERIFIED */

	uint8_t ap_channel = ap_config->channel;

	if (is_zero_ether_addr((u8 *)&g_ap_param_ptr->bssid))
		bk_wifi_ap_get_mac((uint8_t *)(&g_ap_param_ptr->bssid));

	// If STA and AP coexistense is enabled, use STA's channel if possible even
	// if user specified the channel.
	if (!bk_feature_close_coexist_csa() && !close_coexist_csa) {
		uint8_t sta_channel;

		// if ap and sta are coexist, ap channel shall match sta channel firstly
		sta_channel = bk_wlan_sta_get_channel();

		// If user specified softap's channel, prompt to user.
		if (ap_channel)
			WIFI_LOGI("Use STA channel %d for softap, user specified channel %d\n", sta_channel, ap_channel);

		if (sta_channel)
			ap_channel = sta_channel;
	}

	if ((ap_channel >= 1 && ap_channel <= 14)
#if CONFIG_WIFI_BAND_5G
		|| (ap_channel >= 36 && ap_channel <= 177)
#endif
		) {
		bk_wlan_ap_set_channel_config(ap_channel);
	} else if (ap_channel == 0) {
		bk_wlan_ap_set_channel_config(bk_wlan_ap_get_default_channel());
	} else {
		WIFI_LOGD("invalid channel\r\n");
		return BK_ERR_WIFI_CHAN_NUMBER;
	}

	if (ap_config->max_con == 0 || ap_config->max_con > CFG_SUPPORTED_MAX_STA_NUM) {
		if (ap_config->max_con == 0)
			WIFI_LOGW("the max conn num is zero, set it is default\n");
		if (ap_config->max_con > CFG_SUPPORTED_MAX_STA_NUM)
			WIFI_LOGW("the max conn num is more than CFG_SUPPORTED_MAX_STA_NUM, set it is default\n");
		g_ap_param_ptr->max_con = CFG_SUPPORTED_MAX_STA_NUM;
	} else {
		g_ap_param_ptr->max_con = ap_config->max_con;
	}
	g_wlan_general_param->role = CONFIG_ROLE_AP;
	//TODO why need this???
	bk_wlan_set_coexist_at_init_phase(CONFIG_ROLE_AP);

	g_ap_param_ptr->ssid.length = MIN(SSID_MAX_LEN, os_strlen(ap_config->ssid));
	os_memcpy(g_ap_param_ptr->ssid.array, ap_config->ssid, g_ap_param_ptr->ssid.length);
	g_ap_param_ptr->key_len = os_strlen(ap_config->password);
	g_ap_param_ptr->hidden_ssid = ap_config->hidden;
	if (g_ap_param_ptr->key_len < 8) {
		g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_NONE;
	} else {
		switch(ap_config->security) {
			case WIFI_SECURITY_WPA_TKIP:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA_TKIP;
				break;
			case WIFI_SECURITY_WPA_AES:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA_AES;
				break;
			case WIFI_SECURITY_WPA_MIXED:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA_MIXED;
				break;
			case WIFI_SECURITY_WPA2_TKIP:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA2_TKIP;
				break;
			case WIFI_SECURITY_WPA2_AES:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA2_AES;
				break;
			case WIFI_SECURITY_WPA2_MIXED:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA2_MIXED;
				break;
#if CONFIG_SOFTAP_WPA3
			case WIFI_SECURITY_WPA3_SAE:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA3_SAE;
				break;
			case WIFI_SECURITY_WPA3_WPA2_MIXED:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA3_WPA2_MIXED;
				break;
#endif
			default:
				g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_WPA2_AES;
				break;

		}
		os_memset(g_ap_param_ptr->key, 0, sizeof(g_ap_param_ptr->key));
		os_memcpy(g_ap_param_ptr->key, ap_config->password, g_ap_param_ptr->key_len);
	}
#if CONFIG_AP_VSIE
	g_ap_param_ptr->vsie_len = ap_config->vsie_len;
	if (ap_config->vsie_len)
		os_memcpy(g_ap_param_ptr->vsie, ap_config->vsie, g_ap_param_ptr->vsie_len);
#endif

	g_wlan_general_param->dhcp_enable = 1;
	g_ap_param_ptr->hidden_ssid = ap_config->hidden;
#if CONFIG_AP_STATYPE_LIMIT
	if (bk_feature_ap_statype_limit_enable()) {
		os_memcpy(g_ap_param_ptr->max_statype_num, ap_config->max_statype_num, 4);
		if (g_ap_param_ptr->max_statype_num[0] == 0)
			g_ap_param_ptr->max_statype_num[0] = CFG_SUPPORTED_MAX_STA_NUM;
	}
#endif

	return BK_OK;
}

bk_err_t bk_wifi_ap_set_config(const wifi_ap_config_t *ap_config)
{
	int ret = BK_OK;

	WIFI_LOGD("ap configuring\n");

	if (!wifi_is_inited()) {
		WIFI_LOGD("set ap config fail, wifi not init\n");
		return BK_ERR_WIFI_NOT_INIT;
	}

	ret = wifi_ap_validate_config(ap_config);
	if (ret != BK_OK)
		return ret;

	ret = wifi_ap_set_config(ap_config);
	if (ret != BK_OK)
		return ret;

	wifi_set_state_bit(WIFI_AP_CONFIGURED_BIT);
	WIFI_LOGD("ap configured\n");

	if (wifi_ap_is_started()) {
		BK_LOG_ON_ERR(bk_wifi_ap_stop());
		BK_LOG_ON_ERR(bk_wifi_ap_start());
	}
	return BK_OK;
}

bk_err_t bk_wifi_ap_get_config(wifi_ap_config_t *ap_config)
{
	if (!ap_config)
		return BK_ERR_NULL_PARAM;

	os_memcpy(ap_config->ssid, g_ap_param_ptr->ssid.array, g_ap_param_ptr->ssid.length);
	os_memcpy(ap_config->password, g_ap_param_ptr->key, g_ap_param_ptr->key_len);
	ap_config->channel = g_ap_param_ptr->chann;
	ap_config->security = g_ap_param_ptr->cipher_suite;

	return BK_OK;
}

void sm_build_broadcast_deauthenticate(void);
bk_err_t bk_wifi_ap_stop(void)
{
	if (!wifi_ap_is_started()) {
		WIFI_LOGD("ap stop: already stopped\n");
		return BK_OK;
	}

	//TODO why this??
#if CONFIG_AP_IDLE
	if (bk_wlan_has_role(VIF_AP) && ap_ps_enable_get())
		stop_global_ap_bcn_timer();
#endif

	sm_build_broadcast_deauthenticate();
	rtos_delay_milliseconds(10);
	mm_hw_ap_disable();
	rtos_delay_milliseconds(10);
	if(wlan_ap_disable()) {
		WIFI_LOGE("ap disable fail!\n");
		return BK_FAIL;
	}

#if CONFIG_AP_STATYPE_LIMIT
	if (bk_feature_ap_statype_limit_enable())
		bk_vsie_cus_deinit();
#endif

	//TODO optimize it
	if (bk_wlan_has_role(VIF_STA))
		g_wlan_general_param->role = CONFIG_ROLE_STA;

	rtos_delay_milliseconds(200);

	WIFI_LOGD("ap stopped\n");
	wifi_clear_state_bit(WIFI_AP_STARTED_BIT);
	return BK_OK;
}

bk_err_t bk_wifi_ap_get_sta_list(wlan_ap_stas_t *stas)
{
	return wlan_ap_sta_info(stas);
}

bk_err_t bk_wifi_calculate_pmk(const char *ssid, const char *pwd, char *pmk)
{
	uint8_t tmp_pmk[32] = {0};
	int ret;

	if(os_strlen(ssid) == 0 || os_strlen(ssid) > 32) {
		return BK_ERR_PARAM;
	}

	if(os_strlen(pwd) == 0 || os_strlen(pwd) > 64) {
		return BK_ERR_PARAM;
	}

	ret = wpas_calculate_ap_pmk(pwd, (uint8_t *)ssid, tmp_pmk);
	sprintf(pmk, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
				 "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
				 "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		tmp_pmk[0], tmp_pmk[1], tmp_pmk[2], tmp_pmk[3], tmp_pmk[4],
		tmp_pmk[5], tmp_pmk[6], tmp_pmk[7], tmp_pmk[8], tmp_pmk[9],
		tmp_pmk[10], tmp_pmk[11], tmp_pmk[12], tmp_pmk[13], tmp_pmk[14],
		tmp_pmk[15], tmp_pmk[16], tmp_pmk[17], tmp_pmk[18], tmp_pmk[19],
		tmp_pmk[20], tmp_pmk[21], tmp_pmk[22], tmp_pmk[23], tmp_pmk[24],
		tmp_pmk[25], tmp_pmk[26], tmp_pmk[27], tmp_pmk[28], tmp_pmk[29],
		tmp_pmk[30], tmp_pmk[31]);
	return ret;
}

bk_err_t bk_wifi_monitor_register_cb(const wifi_monitor_cb_t monitor_cb)
{
	return wifi_monitor_register_cb(monitor_cb);
}

bk_err_t bk_wifi_monitor_set_config(const wifi_monitor_config_t *monitor_config)
{
	int ret = BK_OK;

	if (!monitor_config)
		return BK_ERR_NULL_PARAM;

	if (monitor_config->reserved != 0)
		return BK_ERR_WIFI_RESERVED_FIELD;

	ret = wifi_monitor_set_config(monitor_config);
	WIFI_LOGD("monitor config, mcast=%d dup=%d\n", monitor_config->rx_mcast_frame, monitor_config->rx_dup_frame);
	return ret;
}

bk_err_t bk_wifi_monitor_get_config(wifi_monitor_config_t *monitor_config)
{
	if (!monitor_config)
		return BK_ERR_NULL_PARAM;

	wifi_monitor_get_config(monitor_config);
	return BK_OK;
}

#ifdef CONFIG_WPA_TWT_TEST
bk_err_t bk_wifi_twt_setup(uint8_t setup_type, uint16_t mantissa, uint8_t min_twt)
{
	#if NX_TWT
	return rwnx_msg_send_twt_setup(setup_type, mantissa, min_twt);
	#else
	return BK_ERR_NOT_FOUND;
	#endif
}

bk_err_t bk_wifi_twt_teardown(void)
{
	#if NX_TWT
	return rwnx_msg_send_twt_teardown();
	#else
	return BK_ERR_NOT_FOUND;
	#endif
}
#endif
bk_err_t bk_wifi_filter_set_config(const wifi_filter_config_t *filter_config)
{
	if (!filter_config)
		return BK_ERR_NULL_PARAM;

	if (filter_config->reserved != 0)
		return BK_ERR_WIFI_RESERVED_FIELD;

	WIFI_LOGD("filter configured(%x)\n", *(uint32_t *)filter_config);
	return wifi_filter_set_config(filter_config);
}

bk_err_t bk_wifi_filter_get_config(wifi_filter_config_t *filter_config)
{
	if (!filter_config)
		return BK_ERR_NULL_PARAM;

	return wifi_filter_get_config(filter_config);
}

bk_err_t bk_wifi_filter_register_cb(const wifi_filter_cb_t filter_cb)
{
	if (filter_cb)
		WIFI_LOGD("filter started\n");
	else
		WIFI_LOGD("filter stopped\n");

	return wifi_filter_register_cb(filter_cb);
}

static bk_err_t wifi_monitor_validate_channel(const wifi_channel_t *chan)
{
	uint8_t primary;

	if (!chan)
		return BK_ERR_NULL_PARAM;

	if (chan->second != 0)
		WIFI_LOGD("curently only support HT20, second channel is ignored");

	primary = chan->primary;
	if (primary <= WIFI_MAX_CHAN_NUM) {
		if (primary < WIFI_MIN_CHAN_NUM) {
			WIFI_LOGD("invalid channel number <%d, %d>\n", primary, chan->second);
			return BK_ERR_WIFI_CHAN_NUMBER;
		}
	} else {
#if CONFIG_WIFI_BAND_5G
		if (!rw_ieee80211_5g_channel_supported(chan->primary)) {
			WIFI_LOGD("invalid channel number <%d, %d>\n", primary, chan->second);
			return BK_ERR_WIFI_CHAN_NUMBER;
		}
#else
		// 2.4G channel number larger than 14.
		WIFI_LOGD("invalid channel number <%d, %d>\n", primary, chan->second);
		return BK_ERR_WIFI_CHAN_NUMBER;
#endif
	}

	if (wifi_sta_is_started() || wifi_ap_is_started()) {
		wifi_country_t country = {0};
		WIFI_LOGD("monitor change channel when STA or AP is started\n");
		rw_ieee80211_get_country(&country);
		if (primary <= 14 && (primary < country.schan || primary > country.nchan))
			return BK_ERR_WIFI_CHAN_NUMBER;
	}

	return BK_OK;
}

bk_err_t bk_wifi_monitor_set_channel(const wifi_channel_t *chan)
{
	int ret = BK_OK;

	ret = wifi_monitor_validate_channel(chan);
	if (ret != BK_OK)
		return ret;

	rwnxl_reset_evt(0);

	//always set to HT20
	rw_msg_set_channel(chan->primary, PHY_CHNL_BW_20, NULL);

	WIFI_LOGD("monitor set channel<%d, %d>\n", chan->primary, chan->second);
	return BK_OK;
}

/* TODO - monitor is independent as STA/AP, should decouple it
 * TODO - optimize monitor!!!
 *
 */
bk_err_t bk_wifi_monitor_start(void)
{
	WIFI_LOGD("monitor starting\n");

	bk_wifi_scan_stop();
	bk_wifi_sta_pm_disable();
	wifi_sta_init_rw_driver();

	// add/open monitor interface
	rwnx_monitor_open();

	wifi_monitor_set_enabled(true);
	wifi_set_state_bit(WIFI_MONITOR_STARTED_BIT);
	WIFI_LOGD("monitor started(%x)\n", s_wifi_state_bits);
	return BK_OK;
}

bk_err_t bk_wifi_monitor_stop(void)
{
	WIFI_LOGD("monitor stopping\n");

	if (wifi_monitor_is_started()) {
		rwnx_monitor_close();
		bk_wifi_sta_pm_enable();
	}

	wifi_monitor_set_enabled(false);
	wifi_clear_state_bit(WIFI_MONITOR_STARTED_BIT);
	WIFI_LOGD("monitor stopped(%x)\n", s_wifi_state_bits);
	return BK_OK;
}

////function for resume monitor :MAC can not sleep
bk_err_t bk_wifi_monitor_resume(void)
{
	//WIFI_LOGI("monitor resume start\n");

	////vote monitor disbale mac ps
	bk_wifi_sta_pm_disable();

	//WIFI_LOGI("monitor resume end\n");
	return BK_OK;
}

extern void ps_reset_prevent(void);
////function for suspend monitor : MAC can sleep
bk_err_t bk_wifi_monitor_suspend(void)
{
	//WIFI_LOGI("monitor suspend start\n");

	///clean prevent sleep flag
	ps_reset_prevent();

	///clean monitor for mac can sleep
	bk_wifi_sta_pm_enable();

	//WIFI_LOGI("monitor suspend end\n");
	return BK_OK;
}

bk_err_t bk_wifi_set_country(const wifi_country_t *country)
{
	//TODO add init/start precondition check
	return rw_ieee80211_set_country(country);
}

bk_err_t bk_wifi_get_country(wifi_country_t *country)
{
	//TODO add init/start precondition check
	return rw_ieee80211_get_country(country);
}

bk_err_t bk_wifi_send_raw(uint8_t *buffer, int len)
{
	uint8_t *pkt;
	int ret;

	//TODO add init/start precondition check

	pkt = os_malloc(len);
	if (pkt == NULL) {
		WIFI_LOGD("raw send fail, oom\n");
		return BK_ERR_NO_MEM;
	}

	os_memcpy(pkt, buffer, len);
	ret = bmsg_tx_raw_sender(pkt, len);
	return ret;
}
bk_err_t bk_wifi_set_tx_raw_rate_power(uint32_t rate, uint32_t power)
{
	return rwm_set_tx_raw_rate_power(rate, power);
}

bk_err_t bk_wifi_send_raw_ex(wifi_raw_tx_info_t *raw_tx, void *cb, void *param)
{
	uint8_t *raw_tx_copy;

	raw_tx_copy = os_malloc(sizeof(wifi_raw_tx_info_t) + raw_tx->len);
	if (raw_tx_copy == NULL) {
		WIFI_LOGD("raw ex send fail, alloc PKT failed\n");
		return BK_ERR_NO_MEM;
	}

	dma_memcpy(raw_tx_copy, raw_tx, sizeof(wifi_raw_tx_info_t));
	dma_memcpy(raw_tx_copy + sizeof(wifi_raw_tx_info_t), raw_tx->pkt, raw_tx->len);
	return bmsg_tx_raw_ex_sender((wifi_raw_tx_info_t *)raw_tx_copy, cb, param);
}

bk_err_t bk_wifi_send_tx_raw_ex(wifi_raw_tx_info_t *raw_tx, void *cb, void *param)
{
	return bmsg_tx_raw_ex_sender((wifi_raw_tx_info_t *)raw_tx, cb, param);
}

bk_err_t bk_wifi_set_tx_raw_ac(uint8_t ac)
{
	g_wifi_mac_config.raw_tx_ac = ac;

	return BK_OK;
}

bk_err_t bk_wifi_set_tx_raw_timeout(uint16_t timeout_ms)
{
	g_wifi_mac_config.raw_tx_timeout_ms = timeout_ms;

	return BK_OK;
}

bk_err_t bk_wifi_set_wifi_media_mode(bool flag)
{
	g_wifi_mac_config.media_flag = flag;

	return BK_OK;
}

bk_err_t bk_wifi_set_video_quality(uint8_t quality)
{
	g_wifi_mac_config.media_quality = quality;

	return BK_OK;
}

bk_err_t bk_wifi_get_video_quality_config(uint8_t *quality)
{
	if (!quality)
		return BK_ERR_NULL_PARAM;

	*quality = g_wifi_mac_config.media_quality;

	return BK_OK;
}

bk_err_t bk_wifi_get_wifi_media_mode_config(bool *flag)
{
	if (!flag)
		return BK_ERR_NULL_PARAM;

	*flag = g_wifi_mac_config.media_flag;

	return BK_OK;
}

bk_err_t bk_wifi_get_tx_raw_ac(uint8_t *ac)
{
	if (!ac)
		return BK_ERR_NULL_PARAM;

	*ac = g_wifi_mac_config.raw_tx_ac;

	return BK_OK;
}

bk_err_t bk_wifi_get_tx_raw_timeout(uint16_t *time)
{
    if (!time)
		return BK_ERR_NULL_PARAM;

	*time = g_wifi_mac_config.raw_tx_timeout_ms;

	return BK_OK;
}

bk_err_t bk_wifi_set_min_rsv_mem(uint16_t mem_bytes)
{
	if (mem_bytes > WIFI_CFG_MAX_RSV_MEM) {
		WIFI_LOGD("set minimum reserve memory failed, mem_bytes is greater than:%d\n", WIFI_CFG_MAX_RSV_MEM);
		return BK_ERR_PARAM;
	}

	g_wifi_mac_config.min_rsv_mem = mem_bytes;

	return BK_OK;
}
bk_err_t bk_wifi_get_min_rsv_mem(uint16_t *mem_bytes)
{
    if (!mem_bytes)
		return BK_ERR_NULL_PARAM;

	*mem_bytes = g_wifi_mac_config.min_rsv_mem;

	return BK_OK;
}
bk_err_t bk_wifi_set_tx_max_msdu_cnt(uint16_t max_cnt)
{
	g_wifi_mac_config.tx_max_msdu_cnt = max_cnt;

	return BK_OK;
}
bk_err_t bk_wifi_get_tx_max_msdu_cnt(uint16_t *max_cnt)
{
    if (!max_cnt)
		return BK_ERR_NULL_PARAM;

	*max_cnt = g_wifi_mac_config.tx_max_msdu_cnt;

	return BK_OK;
}

bk_err_t  bk_wifi_sta_get_mac(uint8_t *mac)
{
	if (!mac)
		return BK_ERR_NULL_PARAM;

	bk_get_mac(mac, MAC_TYPE_STA);
	return BK_OK;
}

bk_err_t bk_wifi_ap_get_mac(uint8_t *mac)
{
	if (!mac)
		return BK_ERR_NULL_PARAM;

	bk_get_mac(mac, MAC_TYPE_AP);
	return BK_OK;
}

bk_err_t bk_wifi_sta_pm_enable(void)
{
	bk_err_t result = BK_FAIL;
#if CONFIG_STA_PS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	result = bk_wlan_ps_enable();
#pragma GCC diagnostic pop
#endif
	return result;
}

bk_err_t bk_wifi_sta_pm_disable(void)
{
	bk_err_t result = BK_FAIL;
#if CONFIG_STA_PS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	result = bk_wlan_ps_disable();
#pragma GCC diagnostic pop
#endif
	return result;
}

bk_err_t bk_wifi_get_channel(void)
{
	int channel, freq;

	freq = rw_msg_get_channel(NULL);
	channel = rw_ieee80211_get_chan_id(freq);

	return channel;
}

bk_err_t bk_wifi_set_mac_address(char *mac)
{
	return bk_set_base_mac((uint8_t *)mac);
}


bk_err_t bk_wifi_manual_cal_rfcali_status(void)
{
	if (manual_cal_rfcali_status())
		return BK_OK;
	return BK_FAIL;
}


void bk_wifi_rc_config(uint8_t sta_idx, uint16_t rate_cfg)
{
    rw_send_me_rc_set_rate(sta_idx, rate_cfg);
}
bk_err_t bk_wifi_capa_config(wifi_capability_t capa_id, uint32_t capa_val)
{
	if (capa_id >= WIFI_CAPA_ID_MAX) {
		return BK_ERR_PARAM;
	}
	rwnx_udpate_capability(capa_id, capa_val);
	return BK_OK;
}

bk_err_t bk_wifi_ap_vif_probe_req_frame_cb_register(void *cb) {
#if CONFIG_AP_PROBEREQ_CB
	ap_vif_probe_req_frame_cb_register((apvif_probereq_frame_t) cb);
	return BK_OK;
#else
	return BK_ERR_NOT_SUPPORT;
#endif
}

#if CONFIG_RWNX_PROTO_DEBUG
bk_err_t bk_wifi_enable_proto_debug(uint8_t flag) {
	return rwm_set_proto_debug(flag);
}

bk_err_t bk_wifi_disable_proto_debug(uint8_t flag) {
	return rwm_set_proto_debug(flag);
}
#endif

bk_err_t bk_wifi_send_arp_set_rate_req(uint16_t arp_tx_rate) {

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

	return rw_msg_send_arp_set_rate_req(vif_idx, arp_tx_rate);
}

bk_err_t bk_wifi_send_listen_interval_req(uint8_t interval) {
	if(!((1 == interval) || (3 == interval) || (10 == interval)))
	{
		WIFI_LOGE("set listen_int %d fail\r\n",interval);
	}

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

#if NX_P2P
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(vif_idx);
	if (vif->p2p)
		return BK_FAIL;
#endif

	return rw_msg_send_listen_interval_req(vif_idx, interval);
}

bk_err_t bk_wifi_get_listen_interval(uint8_t *listen_interval)
{
	bk_err_t get_result = BK_FAIL;
	struct mm_get_listen_interval_cfm *cfm = NULL;

	WIFI_LOGD("bk_wifi_get_listen_interval\n");

	cfm = os_malloc(sizeof(struct mm_get_listen_interval_cfm));
	if (!cfm) {
		WIFI_LOGE("get_listen_interval,malloc fail\r\n");
		goto get_exit;
	}

	if (BK_OK != rw_msg_get_listen_interval_req((void *)cfm)) {
		WIFI_LOGE("get_listen_interval,send fail\r\n");
		goto get_exit;
	}

	*listen_interval = cfm->listen_intv_dyna;
	get_result = BK_OK;

get_exit:
	if (cfm)
		os_free(cfm);

	return get_result;
}

bk_err_t bk_wifi_send_bcn_loss_int_req(uint8_t interval,uint8_t repeat_num) {

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

#if NX_P2P
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(vif_idx);
	if (vif->p2p)
		return BK_FAIL;
#endif
        WIFI_LOGD("bcn_loss_int %d,repeat %d\r\n",interval,repeat_num);

	return rw_msg_send_bcn_loss_int_req(vif_idx, interval, repeat_num);
}

bk_err_t bk_wifi_set_bcn_loss_time(uint8_t wait_cnt, uint8_t wake_cnt) {

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

#if NX_P2P
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(vif_idx);
	if (vif->p2p)
		return BK_FAIL;
#endif
	WIFI_LOGD("bcn_loss_time wait %d,wake %d\r\n",wait_cnt,wake_cnt);

	return rw_msg_send_bcn_loss_time_req(vif_idx, wait_cnt, wake_cnt);
}

bk_err_t bk_wifi_set_bcn_recv_win(uint8_t default_win, uint8_t max_win, uint8_t step) {

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

#if NX_P2P
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(vif_idx);
	if (vif->p2p)
		return BK_FAIL;
#endif
	WIFI_LOGD("bcn_recv_win default %d,max %d,step %d\r\n",default_win,max_win,step);

	return rw_msg_send_bcn_recv_win_req(vif_idx, default_win, max_win, step);
}

bk_err_t bk_wifi_set_bcn_miss_time(uint8_t bcnmiss_time) {

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

#if NX_P2P
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(vif_idx);
	if (vif->p2p)
		return BK_FAIL;
#endif
	WIFI_LOGD("bcn_miss_time %d\r\n",bcnmiss_time);

	return rw_msg_send_bcn_miss_time_req(vif_idx, bcnmiss_time);
}

bk_err_t bk_wifi_set_conn_delay_time(uint8_t conn_delay_time)
{

	uint8_t vif_idx = 0;
	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);

#if NX_P2P
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(vif_idx);
	if (vif->p2p)
		return BK_FAIL;
#endif
	WIFI_LOGI("conn_delay_time %d\r\n",conn_delay_time);

	return rw_msg_send_conn_delay_time_ind(vif_idx, conn_delay_time);
}

bk_err_t bk_wifi_get_rx_crc_error(uint32_t *ulRxCRCErrorCount)
{
	int ret;
	uint32_t mib = 0;
	struct mm_get_machw_mib_cfm get_mib_cfm;
	mib |= CO_BIT(1);
	ret = rw_msg_get_machw_mib_req(mib, &get_mib_cfm);
	if(!ret)
		*ulRxCRCErrorCount = get_mib_cfm.crc_error_count;
	return ret;
}

bk_err_t bk_wifi_get_statistic_info(wifi_statistics_info_t *wifiinfo)
{
	int ret;
	wifiinfo->noise = -128;
	wifiinfo->idle_time_per = 0;
	wifiinfo->rxrate = 0;
	wifiinfo->mic_error_count = 0;
	wifiinfo->phyrate = 0;
	wifiinfo->txrate = 0;

	WIFI_LOGD("unavilable cNo:%d,IdleT:%d,TxRate:%d,RxRate:%d,PhyRate:%d,MICE:%d\n",
		wifiinfo->noise, wifiinfo->idle_time_per, wifiinfo->txrate, wifiinfo->rxrate, wifiinfo->phyrate,
		wifiinfo->mic_error_count);

	wifiinfo->bandwidth = bk_wifi_get_bandw();
	wifiinfo->rssi = bk_wifi_get_beacon_rssi();
	WIFI_LOGD("Bandw:%d,Rssi:%d\n",
		 wifiinfo->bandwidth, wifiinfo->rssi);

	bk_wifi_get_rx_statistic(&wifiinfo->rx_success_count);
	bk_wifi_get_tx_statistic(&wifiinfo->tx_success_count, &wifiinfo->tx_retry_count, &wifiinfo->tx_fail_count);
	WIFI_LOGE("RxS:%d TxS:%d TxRe:%d TxF:%d\n",
		wifiinfo->rx_success_count, wifiinfo->tx_success_count, wifiinfo->tx_retry_count, wifiinfo->tx_fail_count);

	ret = bk_wifi_get_rx_crc_error(&wifiinfo->rx_crcerror_count);
	WIFI_LOGD("CRCE:%d\n", wifiinfo->rx_crcerror_count);
	return ret;
}


void wlan_set_multicast_flag(void) {
	set_multicast_flag();
}

typedef void (*mqtt_pingreq_timeout_t)(void);
static mqtt_pingreq_timeout_t net_arp_mqtt_cb = NULL;
void bk_net_arp_register_cb(mqtt_pingreq_timeout_t mqtt_timeout)
{
	net_arp_mqtt_cb = mqtt_timeout;
}

void bk_net_mqtt_update_cb(bool send_pingreq, uint32_t period_s)
{
	int ret = 0;
	if(net_arp_mqtt_cb){
		ret = rw_msg_send_mm_mqtt_info(true, send_pingreq, period_s);
	}
	else{
		ret = rw_msg_send_mm_mqtt_info(false, send_pingreq, 0);
	}

	if (ret)
		WIFI_LOGE("%s: fail\n", __func__);
}

void bk_trigger_mqttping(void)
{
	/* forward to mqtt */
	if(net_arp_mqtt_cb) {
		(*net_arp_mqtt_cb)();
	}
}

bk_err_t bk_dbg_init(void)
{
    #if BK_MEM_DYNA_APPLY_EN
    trace_mem_apply();
    dbg_mem_apply();
    hal_machw_mem_apply();
    #endif
    return 0;
}

extern uint32_t rwnxl_get_video_transfer_flag(void);
/**
 * Re-calculate TCP RTO according to congestion avoidance/control and
 * number of retransmissions
 */
int16_t bk_recalc_tcp_rto(int16_t rto, uint8_t nrtx, int16_t sa, int16_t sv)
{
    // decrease RTO to get better TCP letancy when video transfer is ongoing
    if (rwnxl_get_video_transfer_flag())
    {
        return WIFI_VIDEO_TRANSFER_TCP_RTO;
    }

    // nrtx/sa/sa is reserved to be used

    return rto;
}
/**
 * Re-calculate TCP RTO according to congestion avoidance/control and
 * number of retransmissions
 */
uint32_t bk_recalc_tcp_cwnd(uint32_t cwnd, uint32_t snd_wnd, uint32_t mss)
{
    // increse cwnd to get better TCP letancy when video transfer is ongoing
    if (rwnxl_get_video_transfer_flag())
    {
        if (mss<<1 > snd_wnd)
        {
            return cwnd;
        }
        else
        {
            return (mss<<1 > cwnd) ? mss<<1:cwnd;
        }
    }

    return cwnd;
}

int wlan_sta_enable_ssid_blacklist(void)
{
#if CONFIG_WPA_SUPPLICANT_2_9
	return wpa_ctrl_request(WPA_CTRL_CMD_SSID_BLACKLIST_ENABLE, NULL);
#else
	return 0;
#endif
}

int wlan_sta_disable_ssid_blacklist(void)
{
#if CONFIG_WPA_SUPPLICANT_2_9
	return wpa_ctrl_request(WPA_CTRL_CMD_SSID_BLACKLIST_DISABLE, NULL);
#else
	return 0;
#endif
}

void bk_wifi_ps_cmd_open(void)
{
	if (wifi_ap_is_started())
	{
		WIFI_LOGD("Softap is on,powersave can't use\n\r");
		return;
	}

	bmsg_ps_sender(PS_BMSG_IOCTL_PS_ENABLE);
	return;
}

void bk_wifi_ps_cmd_close(void)
{
	if (wifi_ap_is_started())
	{
		WIFI_LOGD("Softap is on,powersave can't use\n\r");
		return;
	}

	bmsg_ps_sender(PS_BMSG_IOCTL_PS_DISANABLE);
	return;
}

uint8_t bk_wifi_get_ps_status(void)
{
	//0 - Power save Mode
	//1 -Active Mode
	int ret = 0;
	ret = !(ps_cmd_get());

	return ret;
}

void bk_wifi_ps_cmd_debug_printf(uint8_t ps_val)
{
	rw_msg_send_psdebug_req(ps_val);
	return;
}

void bk_wifi_mask_td_info(void)
{
	rw_msg_send_td_mask_req();
	return;
}

void bk_wifi_set_td_info(uint8_t interval, uint8_t max_pnk)
{
	rw_msg_send_td_interval_req(interval,max_pnk);
	return;
}

void bk_wifi_set_td_active_percent(uint8_t active_percent)
{
	if(active_percent < 10 || active_percent > 100)
	{
		WIFI_LOGE("Invalid active_percent config\r\n");
		return;
	}

	bk_wifi_set_td_info(active_percent,200);
	return;
}

void bk_wifi_mask_td_active_percent()
{
	bk_wifi_mask_td_info();
	return;
}

void bk_wifi_ps_config(uint8_t ps_id, uint8_t ps_val, uint8_t ps_val1)
{
	switch (ps_id)
	{
		case WIFI_PS_CMD_OPEN:
			bk_wifi_ps_cmd_open();
			break;
		case WIFI_PS_CMD_CLOSE:
			bk_wifi_ps_cmd_close();
			break;
		case WIFI_PS_CMD_DEBUG_PRINTF:
			bk_wifi_ps_cmd_debug_printf(ps_val);
			break;
		case WIFI_PS_CMD_TD_INFO:
			bk_wifi_set_td_info(ps_val,ps_val1);
			break;
		case WIFI_PS_CMD_TD_MASK:
			bk_wifi_mask_td_info();
			break;
		default:
			break;
	}
	return;
}

FUNC_1PARAM_PTR ap_power_cb = NULL;
void bk_wifi_ap_power_register_cb(FUNC_1PARAM_PTR cb)
{
	ap_power_cb = cb;
}

FUNC_1PARAM_PTR bk_wifi_get_ap_power_cb(void)
{
	return ap_power_cb;
}

bk_err_t bk_wifi_set_csa_coexist_mode_flag(bool is_close)
{
	if (s_wifi_state_bits & WIFI_STA_STARTED_BIT)
	{
		WIFI_LOGW("Set csa coxist mode fail, 0x%x\r\n",s_wifi_state_bits);
		return BK_ERR_STATE;
	}

	close_coexist_csa = is_close;
	return BK_OK;
}


#if CONFIG_WIFI_FTM
bk_err_t bk_wifi_ftm_start(const wifi_ftm_config_t *config, wifi_ftm_results_t *ftm_results)
{
	int val, ret = BK_OK;
	uint8_t vif_idx = 0;
	struct ftm_done_ind *ind = NULL;

	WIFI_LOGD("FTM starting\n");
	WIFI_LOGD("ftm config: ftm_per_burst %d, nb_ftm_rsp %d \n", config->ftm_per_burst, config->nb_ftm_rsp);

	if (!wifi_is_inited()) {
		WIFI_LOGE("start ftm fail, wifi not init\n");
		return BK_ERR_WIFI_NOT_INIT;
	}

	if (!wifi_sta_is_started()) {
		WIFI_LOGE"start ftm fail, sta not start\n");
		return BK_ERR_WIFI_STA_NOT_STARTED;
	}

	vif_idx = rwm_mgmt_vif_mac2idx((void *)&g_sta_param_ptr->own_mac);
	if (INVALID_VIF_IDX == vif_idx) {
		ret = BK_ERR_WIFI_DRIVER;
		goto get_exit;
	}

	ind = os_malloc(sizeof(struct ftm_done_ind));
	if (!ind) {
		ret = BK_ERR_NO_MEM;
		goto get_exit;
	}

	val = rw_msg_send_ftm_start_req(vif_idx, config->ftm_per_burst, config->nb_ftm_rsp, (void *)ind);
	if (val) {
		ret = BK_ERR_WIFI_DRIVER;
		goto get_exit;
	}

	ftm_results->nb_ftm_rsp = ind->results.nb_ftm_rsp;
	ftm_results->rsp = os_zalloc(sizeof(wifi_ftm_rsp_info_t) * ftm_results->nb_ftm_rsp);
	for (int i = 0; i < ftm_results->nb_ftm_rsp; i++)
	{
		os_memcpy(ftm_results->rsp[i].bssid, (uint8_t *)&ind->results.meas[i].addr, ETH_ALEN);
		ftm_results->rsp[i].rtt = ind->results.meas[i].rtt / 1000;
		ftm_results->rsp[i].distance = (float)(ftm_results->rsp[i].rtt)*0.15;
	}

get_exit:
	if (ind)
		os_free(ind);

	return ret;
}

bk_err_t bk_wifi_ftm_dump_result(const wifi_ftm_results_t *ftm_results)
{
	if (!ftm_results) {
		WIFI_LOGW("ftm doesn't found responser\n");
		return BK_OK;
	}

	if ((ftm_results->nb_ftm_rsp > 0) && (!ftm_results->rsp)) {
		WIFI_LOGE("ftm responser number is %d, but responser info is NULL\n", ftm_results->nb_ftm_rsp);
		return BK_ERR_PARAM;
	}

	WIFI_LOGI("ftm found %d responser\n", ftm_results->nb_ftm_rsp);

	for (int i = 0; i < ftm_results->nb_ftm_rsp; i++) {
		BK_WIFI_LOGI("The distance to " WIFI_MAC_FORMAT " is %.2f meters, rtt is %d nSec \n",
			WIFI_MAC_STR(ftm_results->rsp[i].bssid), ftm_results->rsp[i].distance, ftm_results->rsp[i].rtt);
		rtos_delay_milliseconds(10);
	}

	WIFI_LOG_RAW("\n");

	return BK_OK;
}

void bk_wifi_ftm_free_result(wifi_ftm_results_t *ftm_results)
{
	if (ftm_results) {
		os_free(ftm_results->rsp);
		ftm_results->rsp = 0;
		ftm_results->nb_ftm_rsp = 0;
	}
	WIFI_LOGD("ftm free result\n");
}
#endif //CONFIG_WIFI_FTM

#if CONFIG_WIFI_CSI_EN
bk_err_t bk_wifi_csi_active_mode_req(wifi_csi_active_mode_config_t *config)
{
	struct csi_active_mode_cfm cfm;
	rw_msg_send_csi_active_mode_req(config, &cfm);
	if(cfm.status != BK_OK)
	{
		WIFI_LOGE("csi active mode req failed, status: %d\r\n", cfm.status);
		return BK_FAIL;
	}
	return BK_OK;
}

bk_err_t bk_wifi_csi_receive_mode_req(wifi_csi_receive_mode_config_t *config)
{
	struct csi_receive_mode_cfm cfm;
	rw_msg_send_csi_receive_mode_req(config, &cfm);
	if(cfm.status != BK_OK)
	{
		WIFI_LOGE("csi receive mode req failed, status: %d\r\n", cfm.status);
		return BK_FAIL;
	}
	return BK_OK;
}
#endif //CONFIG_WIFI_CSI_EN

wifi_csi_cb_t g_wifi_csi_info_handler = NULL;
void bk_wifi_csi_info_cb_register(wifi_csi_cb_t cb)
{
	g_wifi_csi_info_handler = cb;
}
void bk_wifi_csi_info_cb(void * data)
{
	if(g_wifi_csi_info_handler)
		g_wifi_csi_info_handler((struct wifi_csi_info_t *)data);
}

bk_err_t bk_wifi_get_tx_stats(uint8_t mode,struct tx_stats_t* tx_stats)
{
	struct tx_stats_t* tx_stats_tmp;
	static struct tx_stats_t tx_stats_prev = {0};
	uint8_t vif_idx;
	if(tx_stats == NULL) return BK_ERR_PARAM;
	if(mode == NETIF_IF_STA)
	{
		vif_idx = vif_mgmt_get_sta_vif_index();
	}
	else if(mode == NETIF_IF_AP)
	{
		vif_idx = vif_mgmt_get_softap_vif_index();
	}
	else
	{
		return BK_ERR_PARAM;
	}
	tx_stats_tmp = (struct tx_stats_t*)mac_vif_tx_stats_get(vif_idx);
	if(tx_stats_tmp != NULL)
	{
		tx_stats->tx_agg_fail = tx_stats_tmp->tx_agg_fail - tx_stats_prev.tx_agg_fail;
		tx_stats->tx_agg_total = tx_stats_tmp->tx_agg_total - tx_stats_prev.tx_agg_total;
		tx_stats->tx_single_retry = tx_stats_tmp->tx_single_retry - tx_stats_prev.tx_single_retry;
		tx_stats->tx_single_total = tx_stats_tmp->tx_single_total - tx_stats_prev.tx_single_total;
		
		memcpy(&tx_stats_prev,tx_stats_tmp,sizeof(struct tx_stats_t));

		tx_stats->chan = rw_ieee80211_get_chan_id(tx_stats_tmp->chan);
	}
	else
	{
		return BK_ERR_PARAM;
	}
/*  per = (tx_agg_fail + tx_singel_retry)/(tx_agg_total + tx_single_total*SINGLE_RETRY_CNT)  */

	return BK_OK;
}
bk_err_t bk_wifi_set_data_rty_num(uint8_t rty_num)
{
	g_wifi_mac_config.data_rty_num = ((uint16_t)rty_num << 8)|(uint16_t)rty_num;

	return BK_OK;
}
bk_err_t bk_wifi_get_data_rty_num(uint32_t* rty_num)
{
	if(rty_num == NULL)
	{
		 return BK_ERR_NULL_PARAM;
	}
	*rty_num = (uint32_t)g_wifi_mac_config.data_rty_num;

	return BK_OK;
}
bk_err_t bk_wifi_set_mgmt_rty_num(uint8_t rty_num)
{
	g_wifi_mac_config.mgmt_rty_num = ((uint16_t)rty_num << 8)|(uint16_t)rty_num;

	return BK_OK;
}
bk_err_t bk_wifi_get_mgmt_rty_num(uint32_t* rty_num)
{
	if(rty_num == NULL)
	{
		 return BK_ERR_NULL_PARAM;
	}
	*rty_num = (uint32_t)g_wifi_mac_config.mgmt_rty_num;

	return BK_OK;
}

bk_err_t bk_wifi_set_block_bcmc_en(uint8_t config)
{
	g_wifi_mac_config.block_bcmc_en = config;
	return BK_OK;
}

bool bk_wifi_get_block_bcmc_en(void)
{
	return g_wifi_mac_config.block_bcmc_en;
}

void bk_wifi_set_ap_channel(uint8_t channel)
{
	int ret = 0;
	uint16_t frequency;

	if (0 == bk_wlan_has_role(VIF_AP))
		return;

	if (((channel < 1) || (channel > 14))
#if CONFIG_SOC_BK7239XX
		&& (channel < 36 || channel > 177)
#endif
		){
		WIFI_LOGI("please input the valid param\n");
		return;
	}

	frequency = rw_ieee80211_get_centre_frequency(channel);
	if (frequency) {
		WIFI_LOGD("hostapd_channel_switch\n");
		ret = hostapd_channel_switch(frequency);

		if (ret)
			WIFI_LOGW("hostapd_channel_switch failed:%x\r\n", ret);
	}
}
bk_err_t bk_wifi_set_ani_en(bool enable)
{
	g_wifi_mac_config.ani_en = enable;
	return BK_OK;
}
bool bk_wifi_get_ani_en(void)
{
	return g_wifi_mac_config.ani_en;
}

bool rwnxl_get_status_in_doze(void);

int cmd_wlan_get_ps_status(void) {
	/* 0: the status of ps is not ok
	 * 1: the status of ps is ok
	 */
	int ret = 0;
	ret = rwnxl_get_status_in_doze()? 1 : 0;
	return ret;
}



#define CAL_DATA_NUM  (82)       ///11b: 14 channel   11g: 14 channel   11n: 14 channel  ble 40 channel , so cal array length is 14+14+14+40 = 82

bool  bk_wifi_get_cal_2g()
{
	bool is_cal_2g = false;

	uint8_t cal_array[CAL_DATA_NUM] ={0,};
	uint8_t cal_data;
	uint32_t i;
	is_cal_2g = (manual_cal_rfcali_status() == 1) ? true : false;    
	if (is_cal_2g) 
	{
		bk_cal_get_txpwr_tabl(cal_array);
	        for (i = 0; i < CAL_DATA_NUM; i++) 
		{
			cal_data = (uint8_t)cal_array[i];
			#if CONFIG_CALI_DATA_PRINT
			WIFI_LOGE("cal_2g_data[%d]: %u\r\n", i, cal_data);
			#endif
			if( i < 14 ) ///for 11b
			{
				if (cal_data > 23 || cal_data < 3 )
				{
					WIFI_LOGE("cal_2g_data[%d] 11b range error! cal_2g_data: %u\n", i, cal_data);
					is_cal_2g = false;
					break;
				}
			}
			else if(( i >= 14) && (i < 42))  //for 11g and 11n , 11g index is 14-27, 11n is 28-41
			{
				if (cal_data > 53 || cal_data < 13 )
				{
					WIFI_LOGE("cal_2g_data[%d] 11g/n range error! cal_2g_data: %u\n", i, cal_data);
					is_cal_2g = false;
					break;
				}
			}
		}
	}
	else
	{
		WIFI_LOGE("Fail to read cal data \r\n");
	}
    return is_cal_2g;
}

void bk_wifi_get_noise_floor(uint8_t *noise_floor)
{
    ////noise floor is not implemented 
}

#if CONFIG_WIFI_SCAN_COUNTRY_CODE
struct bk_scan_cc_st
{
	beken_semaphore_t cc_wait;
	uint8_t *cc_ptr;
	uint8_t cc_len;
};

static bk_err_t bk_scan_country_code_callback(void *ctxt, uint8_t *cc, uint8_t cc_len)
{
	struct bk_scan_cc_st *bk_scan_ptr = NULL;

	if(ctxt) {
		bk_scan_ptr = (struct bk_scan_cc_st *)ctxt;
		if((cc) && (cc_len != 0)) {
			if(bk_scan_ptr->cc_ptr) {
				os_memcpy(bk_scan_ptr->cc_ptr, cc, MAC_COUNTRY_STRING_LEN);
			}
			bk_scan_ptr->cc_len = MAC_COUNTRY_STRING_LEN;
		}
		bk_wifi_bcn_cc_rxed_register_cb(NULL, NULL);
		rtos_set_semaphore(&bk_scan_ptr->cc_wait);
	}

	return 0;
}

int bk_scan_country_code(uint8_t *country_code)
{
	struct bk_scan_cc_st bk_scan = {0};
	bk_err_t err = kNoErr;

	err = rtos_init_semaphore(&bk_scan.cc_wait, 1);
	if (err == kNoErr) {
		bk_scan.cc_len = 0;
		if(country_code)
			bk_scan.cc_ptr = country_code;
		else
			bk_scan.cc_ptr = NULL;

		bk_wifi_bcn_cc_rxed_register_cb(bk_scan_country_code_callback, &bk_scan);
		err = cc_scan_start();
		if (err == kNoErr) {
			err = rtos_get_semaphore(&bk_scan.cc_wait, 4000);
			cc_scan_stop();
			if (err == kNoErr) {
				rtos_deinit_semaphore(&bk_scan.cc_wait);
				return bk_scan.cc_len;
			}
		}
		bk_wifi_bcn_cc_rxed_register_cb(NULL, NULL);
		rtos_deinit_semaphore(&bk_scan.cc_wait);
	}

	return 0;
}
#endif //CONFIG_WIFI_SCAN_COUNTRY_CODE
