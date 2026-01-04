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

/* TODO:
 * 1. bk_wifi.h is used to define the new beken WiFi APIs
 * 2. Move bk_wifi.h from private to public API after all these
 *    APIs are passed beken test
 * */
#pragma once
#include <common/bk_include.h>
#include <common/bk_err.h>
#include <modules/wifi.h>
#include <components/netif.h>
#include <stdint.h>
#include <stdbool.h>
#include "bk_private/bk_wifi_types.h"
#include "modules/wifi_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* wlan_ui_pub */
#define __bk_deprecated                 __attribute__((deprecated))

enum ip_ver{
	IP4 = 1<<4,
	IP6 = 1<<6,
};

/**
 * @brief WiFi APIs Version 1.0
 * @defgroup bk_api_wifi_v1 Old WiFi API group
 * @{
 */

/* TODO add general description about legacy WiFi API
 *
 */

/** @brief  Connect or establish a Wi-Fi network in normal mode (station or soft ap mode).
 *
 *  This function can establish a Wi-Fi connection as a station or create
 *  a soft AP that other staions can connect (4 stations Max). In station mode,
 *  _BK_ first scan all of the supported Wi-Fi channels to find a wlan that
 *  matchs the input SSID, and read the security mode. Then try to connect
 *  to the target wlan. If any error occurs in the connection procedure or
 *  disconnected after a successful connection, _BK_ start the reconnection
 *  procedure in backgound after a time interval defined in inNetworkInitPara.
 *  Call this function twice when setup coexistence mode (staion + soft ap).
 *  This function retruns immediately in station mode, and the connection will
 *  be executed in background.
 *
 *  @param  inNetworkInitPara: Specifies wlan parameters.
 *
 *  @return In station mode, allways retrurn kWlanNoErr.
 *          In soft ap mode, return kWlanXXXErr
 */
__bk_deprecated
bk_err_t bk_wlan_start(network_InitTypeDef_st* inNetworkInitPara);

/**
 * @brief  Get the number of found APs
 *
 * In scan callback, the application can get the number of found APs by calling this API.
 *
 * @return return the number of found APs
 */
__bk_deprecated
unsigned char bk_wlan_get_scan_ap_result_numbers(void);

/**
 * @brief  Get all APs found by this scan
 *
 * In scan callback, the application can get all found by calling this API.
 *
 * @param scan_result_table store the found APs
 * @param ap_num the maxiumu AP number that scan_result_table can store
 *
 * @return return the actual number of returned APs
 */
__bk_deprecated
int bk_wlan_get_scan_ap_result(SCAN_RST_ITEM_PTR scan_result_table,unsigned char ap_num);

/**
 * @brief Send raw 802.11 frame
 *
 * @attention 1. This API can be used in WiFi station, softap, or monitor mode.
 * @attention 2. Only support to send non-QoS frame.
 * @attention 3. The frame sequence will be overwritten by WiFi driver.
 * @attention 4. The API doesn't check the correctness of the raw frame, the
 *               caller need to guarantee the correctness of the frame.
 *
 * @param buffer raw 802.11 frame
 * @param len the length of the 802.11 frame
 *
 * @return
 *    - kNoErr: success
 *    - otherwise: fail
 */
__bk_deprecated
int bk_wlan_send_80211_raw_frame(uint8_t *buffer, int len);

/**
 * @brief check whether the filter_type is set or not
 *
 * This function is used internally by SDK, it's used to check whether the
 * specified filter_type is set or not.
 *
 * @param filter_type filter type
 *
 *  @return
 *      - true : if filter_type is set
 *      - false : if filter_type is not set
 */
__bk_deprecated
int bk_wlan_monitor_filter(unsigned char filter_type);

/**
 * @brief  set AP default channel
 *
 * TODO -  precondition to use this API?
 *
 * @param  channel AP channel
 */
__bk_deprecated
void bk_wlan_ap_set_default_channel(uint8_t channel);

/**
 * @brief  The callback function of 802.11 frame receiving
 *
 * When WiFi driver receives 802.11 frame that should be filtered to application,
 * the registered callback function will be called. For the filter condition,
 * refer to specific monitor register API, such as bk_wlan_register_monitor_cb etc.
 *
 * @param  data the 802.11 frame received by WiFi driver
 * @param  len the length of data
 * @param  info link info, such as rssi etc
 *
 */
typedef void (*monitor_cb_t)(uint8_t *data, int len, wifi_link_info_t *info);

/**
 * @brief  Enable DTIM power save
 *
 * DTIM power save will enable/disable WiFi RF dynamically according to
 * WiFi power management protocol.
 *
 * TODO add more detail about DTIM power save, such as:
 *  - precondition to use it
 *  - how about AP/STA coexist?
 *  - how about WIFI/BLE coexist?
 *  - how about Monitor mode?
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_ps_enable(void );

/**
 * @brief  Disable DTIM power save
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_ps_disable(void);

/**
 * @brief  Enable DTIM power save timer
 *
 * TODO why we need this API? for keepalive?
 *
 * @return
 *      - kNoErr  : on success.
 *      - oterwise: fail
 */
__bk_deprecated
int bk_wlan_ps_timer_start(void);

/**
 * @brief  Disable DTIM power save timer
 *
 * TODO add details
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_ps_timer_pause(void);

/**
 * @brief  Enable MCU power save
 *
 * When MCU power save is enabled, the power management model low the MCU
 * frequency to save power if it detects there is no activity; and resume
 * the MCU if specific activities happen, such as interrupt, timer etc.
 *
 * TODO add details.
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_mcu_ps_mode_enable(void);

/**
 * @brief  Disable MCU power save
 *
 * TODO add details.
 *
 * @return
 *      - kNoErr   : on success.
 *      - otherwise: fail
 */
__bk_deprecated
int bk_wlan_mcu_ps_mode_disable(void);

/**
 * @brief  Register WiFi status change callback
 *
 * If WiFi status is changed, the callback is called by WiFi driver.
 *
 * TODO add details.
 *
 * @param cb status change callback function
 */
__bk_deprecated
void bk_wlan_status_register_cb(FUNC_1PARAM_PTR cb);

/**
 * @brief  Get WiFi status change callback
 *
 * @return return status callback
 */
__bk_deprecated
FUNC_1PARAM_PTR bk_wlan_get_status_cb(void);

/**
 * @brief  AP no password callback
 */
typedef void (*bk_ap_no_password_cb_t)(void);

/**
 * @brief  Register AP no password callback
 *
 * @attention Current this API has no effect at all, in other words the
 * WiFi driver never calls the callback.
 *
 * @return return status callback
 */
__bk_deprecated
void bk_ap_no_password_connected_register_cb(bk_ap_no_password_cb_t func);

/**
 * @brief  Get AP configurations
 *
 * @param ap store ap basic configurations
 * @param key store the pointer of the global key of AP
 * @param key_len store the key length
 *
 * @return
 *    - kNoErr: success
 *    - others: otherwise
 */
__bk_deprecated
bk_err_t bk_wlan_get_bssid_info(apinfo_adv_t *ap, uint8_t **key, int *key_len);

#ifdef CONFIG_AOS_MESH

/**
 * @brief  Register mesh monitor callback
 *
 * If mesh monitor callback is registered, the WiFi driver filters all received
 * 802.11 data frame to the callback.
 *
 * @param fn mesh monitor callback
 */
void wlan_register_mesh_monitor_cb(monitor_cb_t fn);

/**
 * @brief  Set mesh BSSID
 *
 * @param bssid the BSSID of mesh
 *
 * @return
 *     - kNoErr: success
 *     - others: otherwise
 */
int wlan_set_mesh_bssid(uint8_t *bssid);

/**
 * @brief  Get mesh BSSID
 *
 * This function return the global mesh BSSID
 */
uint8_t *wlan_get_mesh_bssid(void);
#endif

/**
 * @brief  Set Wi-Fi state
 *
 * This function set Wi-Fi state to stop active scan, to ensure
 *	softap video_transfer
 */
void bk_wifi_set_video_transfer_state(bool enable);

#if CONFIG_RWNX_PROTO_DEBUG
/** @brief  enable or diasble proto debug of Wi-Fi driver
 *
 * @param flag indicate enable or diasble proto debug
 *
 */
bk_err_t bk_wifi_enable_proto_debug(uint8_t flag);

/**
 * @brief  enable or diasble proto debug of Wi-Fi driver
 *
 * @param flag indicate enable or diasble proto debug
 *
 */
bk_err_t bk_wifi_disable_proto_debug(uint8_t flag);
#endif

/**
 * @brief  Get sta state
 *
 * This function get Wi-Fi station state
 * 1: started or connect
 * 0: others
 */
bool wifi_sta_is_started_or_connected(void);

/* If SSID blacklist is enabled, the SSID is added into the
  * blacklist if the STA fails to connect it; the blacklist will be
  * cleared once the STA successfully connects one SSID or when
  * STA tries all found SSIDs.
  *
  * This feature only impacts the connect behavior when STA finds
  * several APs with the same SSIDs,
  *
  * e.g.  STA find AP1/AP2/AP3 ... with same SSID xxx. STA puts
  * AP1 into blacklist if it fails to connect AP1 and then tries AP2, STA
  * puts AP2 into blacklist if it fails to connect AP2 and then tries AP3 ...
  * the STA repeats this process until it successfully connects one AP or
  * fails to connect all APs, then the blacklist is cleared.
  *
  * Please be notified that the blacklist feature may consume more time
  * before it can successfully connect to the AP if it finds more than one AP
  * with same SSID, especially if only one AP has correct password but has
  * very low RSSI.
  *
  * Generally speaking, if you want the STA to iterate all the scanned APs (with
  * same SSID) one by one, you should enable the blacklist feature.
  *
  * The SSID blacklist is enabled by default.
  */
int wlan_sta_enable_ssid_blacklist(void);
int wlan_sta_disable_ssid_blacklist(void);
int wlan_p2p_find(void);
int wlan_p2p_cancel(void);
int wlan_dhcp_done_ind(u8 vif_idx);

int http_ota_download(const char *uri);
int bmsg_tx_raw_sender(uint8_t *payload, uint16_t length);
int bmsg_tx_raw_ex_sender(wifi_raw_tx_info_t *raw_tx, void *cb, void *param);
int bmsg_tx_raw_cb_sender(uint8_t *buffer, int length, void *cb, void *param);
void wlan_write_fast_connect_info(struct wlan_fast_connect_info *fci);
void wlan_read_fast_connect_info(struct wlan_fast_connect_info *fci);

int bk_wifi_sta_set_power(int power);
int bk_wifi_ap_set_power(int power);
uint32_t bk_wifi_get_mac_time(void);
int bk_wifi_get_beacon_rssi(void);
void bk_wifi_dump_ps_regs(void);
void bk_wifi_udelay(uint32_t us);
void wlan_hw_reinit(void);
void wlan_set_multicast_flag(void);

void bk_wifi_rc_config(uint8_t sta_idx, uint16_t rate_cfg);
void bk_wifi_ps_config(uint8_t ps_id, uint8_t ps_val, uint8_t ps_val1);
int bk_wlan_mcu_suppress_and_sleep(UINT32);
int bk_wlan_is_monitor_mode(void);
monitor_cb_t bk_wlan_get_monitor_cb(void);

#if CONFIG_WIFI_CSI_EN
void bk_wifi_csi_info_cb(void * data);
#endif
bk_err_t bk_wifi_set_csa_coexist_mode_flag(bool is_close);

#ifdef CONFIG_AOS_MESH
monitor_cb_t wlan_get_mesh_monitor_cb(void);
int wlan_is_mesh_monitor_mode(void);
#endif
int bk_sta_cipher_type(void);

/* bk_wifi_netif */
// TODO
// Wrappers to hide internal details of WiFi stack, such as WiFi Driver etc.
// We should finally remove this wrapper and use the event instead.

int wifi_netif_mac_to_vifid(uint8_t *mac);
void* wifi_netif_vifid_to_vif(int vifid);
void* wifi_netif_mac_to_vif(uint8_t *mac);
netif_if_t wifi_netif_vif_to_netif_type(void *vif);
netif_if_t wifi_netif_mac_to_netif_type(uint8_t *mac);
void wifi_netif_set_vif_private_data(void *vif, void *data);
void* wifi_netif_get_vif_private_data(void *vif);
void wifi_netif_notify_sta_got_ip(enum ip_ver ver);
void wifi_netif_notify_sta_dhcp_timeout(void);
void wifi_netif_call_status_cb_when_sta_got_ip(void);
void wifi_netif_call_status_cb_when_sta_dhcp_timeout(void);
bool wifi_netif_sta_is_connected(void);
bool wifi_netif_sta_is_got_ip(void);
void wifi_netif_notify_sta_disconnect(void);

uint8_t* wifi_netif_vif_to_mac(void *vif);
int wifi_netif_vif_to_vifid(void *vif);

/* bk_wifi_rw */
int bk_wifi_sta_set_power(int power);
int bk_wifi_ap_set_power(int power);
uint32_t bk_wifi_get_mac_time(void);
int bk_wifi_get_beacon_rssi(void);
void bk_wifi_dump_ps_regs(void);
void bk_wifi_set_wait_num(uint16_t trial_wait_num);
void bk_wifi_set_stats_max_tot(uint32_t max_tot_ms);
void bk_wifi_set_stats_tx_count(uint32_t tx_cnt);
void bk_wifi_set_idx_min(uint8_t rc_idx_min);
void bk_wifi_rc_set_he_gi(uint8_t he_gi);
void bk_wifi_rc_set_ht_gi(uint8_t ht_gi);
void bk_wifi_rc_set_type(uint8_t type);
void bk_wifi_set_rc_rssi_offset(int8_t rssi_offset);
uint32_t bk_wifi_get_la_base_address(void);
uint32_t bk_wifi_get_mac_base_address(void);
uint32_t bk_wifi_get_monotonic_counter_2_lo(void);
void bk_wifi_debug_port_sel_pack(uint8_t debugportsel2, uint8_t debugportsel1);
void bk_wifi_phy_wakeup_rf_reinit(void);
void bk_wifi_phy_wakeup_wifi_reinit(void);
void bk_wifi_get_mac_trx_status(bool reset_status);
uint8_t bk_wifi_get_bandw(void);
void bk_wifi_get_rx_statistic(uint32_t *RxSuccessCount);
void bk_wifi_get_tx_statistic(uint32_t *TxSuccessCount, uint32_t *TxRetryCount, uint32_t *TxFailCount);
void bk_wifi_get_wifi_config(void);
void bk_wifi_test_hw_diag(void);
void bk_wifi_set_hw_diag(uint8_t diag_type, uint16_t diag_no);

/* bk_wifi_wpa_cmd*/
int cmd_wlan_sta_exec(char *cmd);
int cmd_wlan_ap_exec(char *cmd);
int cmd_wlan_p2p_exec(char *cmd);
int cmd_wlan_get_ps_status(void);

/* bk_wifi_wpa */
// TODO
// Wrappers to hide internal details of Supplicant
// We should finally remove this wrapper and use the event instead.


/* bk_wifi_wrapper */
#define WLAN_DEFAULT_IP         "192.168.188.1"
#define WLAN_DEFAULT_GW         "192.168.188.1"
#define WLAN_DEFAULT_MASK       "255.255.255.0"

#if CONFIG_BRIDGE
#define WLAN_ANY_IP         "0.0.0.0"
int demo_bridge_softap_init(char *ap_ssid, char *ap_key, wifi_security_t security);
#endif

void demo_scan_app_init(void);
void demo_scan_opt_app_init(int scantype, int duration);
void demo_scan_adv_app_init(uint8_t *oob_ssid);
int demo_softap_app_init(char *ap_ssid, char *ap_key, char *ap_channel);
#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
int demo_sta_app_init(char *oob_ssid, u8 *psk, char *connect_key);
#else
int demo_sta_app_init(char *oob_ssid, char *connect_key);
#endif
void demo_sta_bssid_app_init(uint8_t *bssid, char *connect_key);
void demo_sta_adv_app_init(char *oob_ssid,char *connect_key);
void demo_wlan_app_init(VIF_ADDCFG_PTR cfg);
int demo_state_app_init(void);
void demo_ip_app_init(void);
void demo_wifi_iplog_init(char *iplogmode, char *iplogtype);
void demo_wifi_ipdbg_init(uint32_t ipdbg_func, uint16_t ipdbg_value);
void demo_wifi_mem_apply_init(uint8_t module, uint8_t value);

/* bk_wifi_rw  */
void mhdr_set_station_status(wifi_linkstate_reason_t info);
wifi_linkstate_reason_t mhdr_get_station_status(void);
void rwm_mgmt_set_vif_netif(uint8_t *mac, void *netif);
int rw_msg_send_arp_msg(u8 vif_idx);

/* bk_net_param */
UINT32 test_get_whole_tbl(UINT8 *ptr);
UINT32 save_info_item(NET_INFO_ITEM item, UINT8 *ptr0, UINT8 *ptr1, UINT8 *ptr2);
UINT32 get_info_item(NET_INFO_ITEM item, UINT8 *ptr0, UINT8 *ptr1, UINT8 *ptr2);
UINT32 save_net_info(NET_INFO_ITEM item, UINT8 *ptr0, UINT8 *ptr1, UINT8 *ptr2);
UINT32 get_net_info(NET_INFO_ITEM item, UINT8 *ptr0, UINT8 *ptr1, UINT8 *ptr2);

/* bk_wifi_debug */

/* bk_wifi_frame */
uint32_t bk_wifi_find_ie(uint32_t addr, uint16_t buflen, uint8_t ie_id);

/* param_config */
extern general_param_t *g_wlan_general_param;
extern ap_param_t *g_ap_param_ptr;
extern sta_param_t *g_sta_param_ptr;

uint32_t cfg_param_init(void);


void bk_phy_set_nv_reg_hook(void *hook);


void me_init(void);
void mm_init(void);
void mm_active(void);

void phy_vif_mgmt_register(const void *mac_addr, uint8_t vif_type, bool p2p, uint8_t *vif_idx);


void tx_evm_set_chan_ctxt_pop(void *chan_info);



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



////for rf use
void phy_enter_dsss_only(void);
void phy_exit_dsss_only(void);




void * mpb_reg_api(void);
void * crm_reg_api(void);
void * riu_reg_api(void);
void * mix_funcs(void);


void rs_init(UINT32 channel, INT32 band, UINT32 mode);
void rs_bypass_mac_init(UINT32 channel, INT32 band, UINT32 mode);
void rs_deinit(void);
void evm_phy_init(UINT32 channel, INT32 band, UINT32 bandwidth);
void evm_bypass_mac_init(UINT32 frequency, INT32 band, UINT32 bandwidth);
void evm_clear_ke_evt_mac_bit(void);
void evm_set_ke_evt_mac_bit(void);
void manual_cal_set_cc_backoff_flag(UINT8 enable);



uint32_t evm_req_tx_for_ate(uint32_t frame_len);
uint32_t hal_machw_frame_duration_ate(uint8_t bw, uint8_t modf, uint8_t rate, uint8_t long_preamble, uint8_t short_gi, uint16_t len);

void mdm_set_scramblerctrl(uint32_t value);
void bk_wifi_print_regdomain(void);

//Optimize it
__attribute__((weak)) uint32_t cfg_ap_is_open_system(void);
uint8_t bk_wifi_get_ps_status(void);

/**
 * @brief     This API initializes the WiFi adapter driver
 *
 * Usage example:
 *
 *      wifi_init_config_t init_config = WIFI_DEFAULT_INIT_CONFIG();
 *      BK_LOG_ON_ERR(bk_wifi_adapter_init(&init_config);
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_wifi_adapter_init(const wifi_init_config_t *config);


#ifdef __cplusplus
}
#endif
