// Copyright 2020-2021 Beken
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

#ifdef __cplusplus
extern "C" {
#endif

void bk_wlan_dhcp_timeout(void);
uint32_t bk_sta_cipher_is_open(void);
uint8_t bk_wlan_ap_get_channel_config(void);
VOID bk_wlan_ap_set_channel_config(uint8_t channel);
uint8_t bk_wlan_has_role(uint8_t role);
void bk_wlan_reg_csa_cb_coexist_mode(void);
#ifdef CONFIG_P2P_GO
uint8_t bk_wlan_ap_get_default_channel(void);
void app_p2p_rw_event_func(void *new_evt);
void app_p2p_restart_thread(void);
#endif

/**
 * @brief  start the AP
 *
 * This function stop the AP first and then start the AP with new configuration.
 *
 * @param  inNetworkInitPara the configuration of the AP
 *
 * @return
 *      - kNoErr        : on success.
 *      - kGeneralErr   : fail
 */
__bk_deprecated
bk_err_t bk_wlan_start_ap(network_InitTypeDef_st *inNetworkInitPara);
void bk_wlan_ap_init_adv(network_InitTypeDef_ap_st *inNetworkInitParaAP);
monitor_cb_t bk_wlan_get_bcn_cb(void);
int bk_wlan_ps_disable_send_msg(void);
void bk_wlan_sta_init(network_InitTypeDef_st *inNetworkInitPara);
bk_err_t bk_wlan_start_sta(network_InitTypeDef_st *inNetworkInitPara);

int wlan_sta_set(const uint8_t *ssid, uint8_t ssid_len, const uint8_t *psk);
int wlan_sta_set_config(wlan_sta_config_t *config);
int wlan_sta_get_config(wlan_sta_config_t *config);
int wlan_sta_set_autoreconnect(wlan_auto_reconnect_t *config);
int wlan_sta_get_bss_size(uint32_t *size);
int wlan_sta_get_bss(wlan_sta_bss_info_t *bss_get);
int wlan_sta_set_bss(wlan_sta_bss_info_t *bss_set);
int wlan_sta_enable(void);
int wlan_sta_disable(void);
int wlan_sta_scan_once(void);
int wlan_sta_scan(wlan_sta_scan_param_t *param);
int wlan_sta_scan_result(ScanResult_adv *results);
int wlan_sta_scan_interval(int sec);
int wlan_sta_bss_max_count(uint8_t count);
int wlan_sta_bss_flush(int age);
int wlan_sta_connect(int chan);
int wlan_sta_disconnect(void);
int wlan_sta_state(wlan_sta_states_t *state);
int wlan_sta_twt_setup(uint8_t annouced,
					   uint16_t wake_interval_mantissa, uint8_t wake_interval_exp,
					   uint8_t nominal_min_wake_dur, bool wake_dur_unit);
int wlan_sta_twt_teardown(int flow_id);
int wlan_sta_ap_info(struct ApListStruct *ap);
int wlan_sta_gen_psk(wlan_gen_psk_param_t *param);
int wlan_sta_wps_pbc(void);
int wlan_sta_wps_pin_get(wlan_sta_wps_pin_t *wps);
int wlan_sta_wps_pin_set(wlan_sta_wps_pin_t *wps);
int wlan_sta_set_vendor_ie(wlan_sta_vsie_t *vsie);
int wlan_ap_set(uint8_t *ssid, uint8_t ssid_len, uint8_t *psk);
int wlan_ap_set_config(wlan_ap_config_t *config);
int wlan_ap_get_config(wlan_ap_config_t *config);
int wlan_ap_enable(void);
int wlan_ap_reload(void);
int wlan_ap_disable(void);
int wlan_ap_sta_num(int *num);
int wlan_ap_sta_info(wlan_ap_stas_t *stas);
int wlan_ap_set_beacon_vsie(wlan_ap_vsie_t *vsie);
int wlan_dhcp_done_ind(u8 vif_idx);
#if CONFIG_P2P
int wlan_p2p_listen(void);
int wlan_p2p_stop_find(void);
int wlan_p2p_connect(const uint8_t *mac, int method, int intent);
#endif
void bk_wifi_prepare_deepsleep(void);
void bk_trigger_mqttping(void);

#ifdef __cplusplus
}
#endif

