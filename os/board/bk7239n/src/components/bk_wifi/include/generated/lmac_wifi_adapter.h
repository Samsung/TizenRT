#ifndef __LMAC_WIFI_ADAPTER_H_
#define __LMAC_WIFI_ADAPTER_H_

/* THIS FILE IS AUTO-GENERATED, DON'T MODIFY */

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
#define BK_WIFI_OS_ADAPTER_VERSION  0x00000001
#ifndef __BK_ADAPTER_H
#define __BK_ADAPTER_H

typedef enum {
	BK_PBUF_TRANSPORT,
	BK_PBUF_IP,
	BK_PBUF_LINK,
	BK_PBUF_RAW_TX,
	BK_PBUF_RAW
} bk_pbuf_layer;

typedef enum {
	BK_PBUF_RAM,
	BK_PBUF_RAM_RX,
	BK_PBUF_ROM,
	BK_PBUF_REF,
	BK_PBUF_POOL
} bk_pbuf_type;

typedef struct {
	int _version;
	int (*_manual_cal_rfcali)(void);
	void (*_rc_drv_set_rf_en)(uint32_t value);
	uint32_t (*_rc_drv_get_rf_en)(void);
	int (*_cal_set_rfconfig_BTPLL)(void);
	int (*_cal_set_rfconfig_WIFIPLL)(void);
	int (*_calibration_init)(void);
	UINT32 (*_sys_is_enable_hw_tpc)(void);
	UINT32 (*_sys_is_use_abs_power)(void);
	void (*_sys_set_use_abs_power)(UINT32 use);
	void (*_evm_via_mac_continue)(void);
	void (*_evm_via_mac_evt)(INT32 dummy);
	int (*_do_evm)(void *cmdtp, int flag, int argc, char *const argv[]);
	int (*_do_rx_sensitivity)(void *cmdtp, int flag, int argc, char *const argv[]);
	INT32 (*_rwnx_cal_save_trx_rcbekn_reg_val)(void);
	void (*_rc_drv_set_agc_manual_en)(uint32_t value);
	void (*_rc_drv_set_rx_mode_enrxsw)(uint32_t value);
	void (*_rc_drv_set_rf_macbypass)(uint32_t value);
	uint32_t (*_rc_drv_get_rx_mode_enrxsw)(void);
	UINT32 (*_rwnx_tpc_get_pwridx_by_rate)(UINT32 rate, UINT32 format, UINT32 print_log);
	UINT32 (*_rwnx_is_enable_pwr_change_by_rssi)(void);
	void (*_tpc_auto_change_pwr_by_rssi)(INT8 rssi, UINT8 rate, UINT8 *pwr_idx);
	void (*_bk7011_max_rxsens_setting)(void);
	void (*_bk7011_default_rxsens_setting)(void);
	UINT32 (*_tx_evm_rate_get)(void);
	UINT32 (*_tx_evm_pwr_idx_get)(void);
	UINT32 (*_tx_evm_modul_format_get)(void);
	UINT32 (*_tx_evm_guard_i_tpye_get)(void);
	UINT32 (*_tx_evm_bandwidth_get)(void);
	UINT32 (*_tx_evm_mode_get)(void);
	UINT32 (*_rwnx_setting_for_single_rate)(INT32 rate);
	void (*_delay05us)(INT32 num);
	void (*_rwnx_cal_recover_rf_setting)(void);
	void (*_rwnx_cal_recover_wifi_setting)(void);
	void (*_rwnx_cal_set_40M_setting)(void);
	void (*_rwnx_cal_set_20M_setting)(void);
	void (*_rwnx_cal_recover_rcbeken_reg_val)(void);
	void (*_rwnx_cal_recover_trx_reg_val)(void);
	void (*_rwnx_cal_set_channel)(UINT32 freq);
	void (*_bk7011_cal_dpd)(uint32_t freq, uint32_t connect_mode);
	void (*_bk7011_cal_pll)(void);
	void (*_bk7011_update_by_rx)(int8_t rssi, int8_t freq_offset);
	INT32 (*_rwnx_cal_load_trx_rcbekn_reg_val)(void);
	UINT8 (*_manual_get_epa_flag)(void);
	UINT32(*_rxsens_start_flag_get)(void);

	/////////////////////////

	void *(*_pbuf_alloc)(int layer, uint16_t length, int type);
	void (*_pbuf_free)(void *p);
	void (*_pbuf_ref)(void *p);
	void (*_pbuf_header)(void *p, int16_t len);
	void (*_pbuf_cat)(void *p, void *q);
	void *(*_pbuf_coalesce)(void *p);
	int (*_get_rx_pbuf_type)(void);
	int (*_get_pbuf_pool_size)(void);
	void *(*_get_netif_hostname)(void *netif);
	int (*_save_net_info)(int item, u8 *ptr0, u8 *ptr1, u8 *ptr2);
	int (*_get_net_info)(int item, u8 *ptr0, u8 *ptr1, u8 *ptr2);
	void *(*_inet_ntoa)(void *addr);
	uint32_t (*_lwip_ntohl)(uint32_t val);
	uint16_t (*_lwip_htons)(uint16_t val);
	void (*_sta_ip_start)(void);
	void (*_sta_ip_down)(void);
	void (*_uap_ip_start)(void);
	void (*_uap_ip_down)(void);
	int (*_net_wlan_add_netif)(void *mac);
	int (*_net_wlan_remove_netif)(void *mac);
	uint32_t (*_lookup_ipaddr)(void *addr);
	void (*_set_sta_status)(void *info);
	void (*_sta_ip_mode_set)(int dhcp);
	void (*_net_begin_send_arp_reply)(int is_send_arp, int is_allow_send_req);
	void (*_delay)(INT32 num);
	void (*_delay_us)(UINT32 us);
	uint32_t (*_sys_drv_modem_bus_clk_ctrl)(bool clk_en);
	uint32_t (*_sys_drv_modem_clk_ctrl)(bool clk_en);
	int32 (*_sys_drv_int_enable)(uint32 param);
	int32 (*_sys_drv_int_disable)(uint32 param);
	int32 (*_sys_drv_int_group2_enable)(uint32 param);
	int32 (*_sys_drv_int_group2_disable)(uint32 param);
	void (*_sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm)(uint32_t v);
	uint32_t (*_sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm)(void);
	uint32_t (*_sys_ll_get_cpu_device_clk_enable_mac_cken)(void);
	uint32_t (*_sys_ll_get_cpu_device_clk_enable_phy_cken)(void);
	int32 (*_sys_drv_module_power_state_get)(uint32_t module);
	void (*_power_save_delay_sleep_check)(void);
	void (*_power_save_wake_mac_rf_if_in_sleep)(void);
	void (*_power_save_wake_mac_rf_end_clr_flag)(void);
	UINT8 (*_power_save_if_ps_rf_dtim_enabled)(void);
	UINT16 (*_power_save_forbid_trace)(UINT16 forbid);
	bk_err_t (*_bk_pm_module_vote_sleep_ctrl)(uint32_t module, uint32_t sleep_state, uint32_t sleep_time);
	uint32_t (*_bk_pm_lpo_src_get)(void);
	int(*_ps_need_pre_process)( UINT32 arg );
	bool (*_power_save_rf_sleep_check)(void);
	int32 (*_bk_pm_module_power_state_get)(unsigned int module);
	bk_err_t (*_bk_pm_module_vote_power_ctrl)(unsigned int module, uint32_t power_state);
	bk_err_t (*_bk_pm_module_vote_cpu_freq)(uint32_t module, uint32_t cpu_freq);
	bool (*_bk_pm_phy_reinit_flag_get)(void);
	void (*_bk_pm_phy_reinit_flag_clear)(void);
	bk_err_t (*_bk_pm_sleep_register)(void *config_cb);
	bk_err_t (*_bk_pm_low_voltage_register)(void *enter_config_cb,void *exit_config_cb);
	void (*_bk_pm_wifi_rtc_set)(uint32_t tick, void *callback);
	void (*_bk_pm_wifi_rtc_clear)(void);
	void (* _wifi_vote_rf_ctrl)(uint8_t cmd);
	void (* _wifi_phy_clk_open)(uint8_t is_wifi);
	void (* _wifi_phy_clk_close)(uint8_t is_wifi);
	void (* _wifi_mac_clk_open)(void);
	void (* _wifi_mac_clk_close)(void);
	void (*_wifi_mac_phy_power_on)(void);
	void (*_mac_ps_exc32_cb_notify)(void);
	void (*_mac_ps_exc32_init)(void *cb);
	void (*_wifi_32k_src_switch_ready_notify)(void *cb);
	void (*_mac_ps_bcn_callback)(uint8_t *data, int len);
	UINT8 (*_mac_sleeped)(void);
	void (*_rwnx_cal_mac_sleep_rc_clr)(void);
	void (*_rwnx_cal_mac_sleep_rc_recover)(void);
	bk_err_t (*_bk_rosc_32k_ckest_prog)(uint32_t param);
	bk_err_t (*_bk_rosc_32k_get_ppm)(void);
	double (*_bk_rosc_32k_get_freq)(void);
	INT64 (*_bk_rosc_32k_get_time_diff)(void);

	void (*_mac_printf_encode)(char *txt, size_t maxlen, const u8 *data, size_t len);
	void (*_dbg_enable_debug_gpio)(void);
	bk_err_t (*_gpio_dev_unprotect_unmap)(uint32_t gpio_id);
	bk_err_t (*_gpio_dev_unprotect_map)(uint32_t gpio_id, uint32_t dev);
	UINT32 (*_mcu_ps_machw_cal)(void);
	UINT32 (*_mcu_ps_machw_reset)(void);
	UINT32 (*_mcu_ps_machw_init)(void);
	void (*_mcu_ps_bcn_callback)(uint8_t *data, int len);
	int (*_bk_feature_config_wifi_csi_enable)(void);
	int (*_bk_feature_receive_bcmc_enable)(void);
	int (*_bk_feature_bssid_connect_enable)(void);
	int (*_bk_feature_fast_connect_enable)(void);
	int (*_bk_feature_send_deauth_before_connect)(void);
	int (*_bk_feature_fast_dhcp_enable)(void);
	int (*_bk_feature_get_scan_speed_level)(void);
	int (*_bk_feature_not_check_ssid_enable)(void);
	int (*_bk_feature_config_cache_enable)(void);
	int (*_bk_feature_config_ckmn_enable)(void);
	int (*_bk_feature_sta_vsie_enable)(void);
	int (*_bk_feature_ap_statype_limit_enable)(void);
	int (*_bk_feature_close_coexist_csa)(void);
	int (*_bk_feature_network_found_event)(void);
	int (*_bk_feature_get_mac_sup_sta_max_num)(void);
	void (*_bk_feature_csi_out_cb)(void *data);
	void (*_flush_all_dcache)(void);
	unsigned long (*_bk_ms_to_ticks)(unsigned long ms);
	bk_err_t (*_dma_memcpy)(void *out, const void *in, uint32_t len);
	bk_err_t (*_bk_wifi_interrupt_init)(void);
	void (*_bk_wifi_stop_rf)(void);
	uint32_t (*_rtos_get_ms_per_tick)(void);
	uint32_t (*_rtos_disable_int)(void);
	void (*_rtos_enable_int)(uint32_t int_level);
	void (*_log)(int level, char *tag, const char *fmt, ...);
	void (*_log_raw)(int level, char * tag, const char *fmt, ...);
	uint32_t (*_rtos_get_time)(void);
	void *(*_os_malloc)(const char *func_name, int line, size_t size);
	void *(*_os_malloc_sram)(const char *func_name, int line, size_t size);
	void (*_os_free)(void *mem_ptr);
	void (*_rtos_assert)(uint32_t exp);
	INT32 (*_os_memcmp)(const void *s, const void *s1, UINT32 n);
	void *(*_os_memset)(void *b, int c, UINT32 len);
	void *(*_os_zalloc)(const char *func_name, int line, size_t size);
	void *(*_rtos_realloc)(void *ptr, size_t size);
	int (*_shell_assert_out)(bool bContinue, char * format, ...);
	void (*_bk_printf)(const char *fmt, ...);
	void (*_bk_null_printf)(const char *fmt, ...);
	bool (*_ate_is_enabled)(void);
	/// os rtos API function pointer
	bk_err_t (*_rtos_push_to_queue)( void** queue, void* message, uint32_t timeout_ms);
	bool (*_rtos_is_queue_full)(void** queue);
	bool (*_rtos_is_queue_empty)(void** queue);
	bk_err_t (*_rtos_pop_from_queue)( void** queue, void* message, uint32_t timeout_ms);
	bk_err_t (*_rtos_push_to_queue_front)(void** queue, void* message, uint32_t timeout_ms );
	uint64_t (*_bk_aon_rtc_get_current_tick)(uint32_t id);
	uint64_t (*_bk_aon_rtc_get_us)(void);
	bk_err_t (*_bk_int_isr_register)(uint32_t src, void* isr_callback, void*arg);
	float(*_rtc_get_ms_tick_cnt)(void);
	bk_err_t (*_rtos_init_semaphore)(void** semaphore, int maxCount);
	bk_err_t (*_rtos_get_semaphore)(void** semaphore, uint32_t timeout_ms);
	bk_err_t (*_rtos_deinit_semaphore)(void** semaphore);
	int (*_rtos_set_semaphore)(void** semaphore);
	bk_err_t (*_rtos_create_thread)(void **thread, uint8_t priority, const char *name, void *function, uint32_t stack_size, void *arg);
	bk_err_t (*_rtos_delete_thread)(void **thread);
	size_t (*_rtos_get_free_heap_size)(void);
	bk_err_t (*_rtos_deinit_queue)(void** queue);
	bk_err_t (*_rtos_init_queue)(void** queue, const char *name, uint32_t message_size, uint32_t number_of_messages);
	bk_err_t (*_rtos_deinit_mutex)(void** mutex);
	bk_err_t (*_rtos_init_mutex)(void** mutex);
	bk_err_t (*_rtos_lock_mutex)(void** mutex);
	bk_err_t (*_rtos_unlock_mutex)(void** mutex);
	bk_err_t (*_rtos_reload_timer)(void *timer);
	uint32_t (*_rtos_enter_critical)(void);
	void (*_rtos_exit_critical)(uint32_t flags);
	bool (*_rtos_is_timer_running)(void *timer);
	int (*_rtos_stop_timer)(void *timer);
	int(*_rtos_init_timer)(void *timer, uint32_t time_ms, void *function, void *arg);
	bool (*_rtos_is_current_thread)(void **thread);
	void (*_register_wifi_dump_hook)(void *wifi_func);
	/// os str API function pointer
	char *(*_os_strdup)(const char *s);
	INT32 (*_os_strcmp)(const char *s1, const char *s2);
	char *(*_os_strchr)(const char *s, int c);
	UINT32 (*_os_strlen)(const char *str);
	void *(*_os_memcpy)(void *out, const void *in, UINT32 n);
	void *(*_os_memmove)(void *out, const void *in, UINT32 n);
	unsigned int (*_os_strlcpy)(char *dest, const char *src, size_t siz);
	INT32 (*_os_strncmp)(const char *s1, const char *s2, const UINT32 n);
	char *(*_os_strcpy)(char *out, const char *in);
	int (*_rtos_delay_milliseconds)(uint32_t num_ms);
	INT32(*_os_snprintf)(char *buf, UINT32 size, const char *fmt, ...);
	//INT32(*_os_vsnprintf)(char *buf, UINT32 size, const char *fmt, va_list ap);
	char *(*_os_strncpy)(char *out, const char *in, const UINT32 n);
	UINT32 (*_os_strtoul)(const char *nptr, char **endptr, int base);
	int(*_os_strcasecmp)(const char *s1, const char *s2);
	int(*_os_strncasecmp)(const char *s1, const char *s2, size_t n);
	char *(*_os_strrchr)(const char *s, int c);
	char *(*_os_strstr)(const char *haystack, const char *needle);
	///
	UINT32 (*_rf_pll_ctrl)(UINT32 cmd, UINT32 param);
	void (*_send_udp_bc_pkt)(uint8_t data);
	void (*_tx_verify_test_call_back)(void);
	void (*_sys_hal_enter_low_analog)(void);
	void (*_sys_hal_exit_low_analog)(void);
	int (*_rw_msg_send)(const void *msg_params, int reqcfm, uint16_t reqid, void *cfm);
	void(*_tpc_change_pwr_by_media)(UINT8 rate, UINT8 *pwr_idx, UINT8 level);
	void(*_tpc_set_media_pwr_level)(UINT8 level);
	UINT8(*_tpc_get_media_pwr_level)(void);
	int (*_bk_otp_apb_read)(uint32_t item, uint8_t* buf, uint32_t size);
	int (*_bk_otp_apb_update)(uint32_t item, uint8_t* buf, uint32_t size);
	uint64_t (*_sys_hal_get_low_voltage_sleep_duration_us)(void);
	void (*_sys_hal_set_low_voltage_sleep_duration_us)(uint64_t sleep_duration);
	uint64_t (*_sys_hal_get_low_voltage_wakeup_time_us)(void);
	void (*_sys_hal_set_low_voltage_wakeup_time_us)(uint64_t wakeup_time);
	int (*_bk_feature_config_mac_pwd_enable)(void);
	int (*_bk_feature_config_wrls_pwd_enable)(void);
	int (*_bk_feature_config_cpu_pwd_enable)(void);
	int (*_bk_feature_config_mac_use_rtc_enable)(void);
	int (*_bk_feature_config_bk7239n_mp_enable)(void);
	int (*_cli_printf)(const char *fmt, ...);
	void (*_rf_force_set_wifi_mode)(uint8_t switch_mode);
	int (*_bk_feature_change_to_wifi_pll_enable)(void);
} wifi_os_funcs_t;
extern wifi_os_funcs_t g_wifi_os_funcs;

typedef struct {
	uint32_t _sys_drv_clk_on;
	uint32_t _sys_sys_debug_config1_addr;
	uint32_t _ps_unallow;
	uint32_t _ps_allow;
	uint32_t _ps_event_key;
	uint32_t _ps_event_sta;
	uint32_t _ps_event_ap;
	uint32_t _ps_event_monitor;
	uint32_t _ps_event_phy;
	uint32_t _ps_event_scan;
	uint32_t _ps_forbid_in_doze;
	uint32_t _ps_forbid_txing;
	uint32_t _ps_forbid_hw_timer;
	uint32_t _ps_forbid_vif_prevent;
	uint32_t _ps_forbid_keevt_on;
	uint32_t _ps_bmsg_iotcl_rf_timer_init;
	uint32_t _ps_bmsg_ioctl_rf_ps_timer_deint;
	uint32_t _ps_bmsg_ioctl_rf_enable;
	uint32_t _ps_bmsg_ioctl_mcu_enable;
	uint32_t _ps_bmsg_ioctl_mcu_disable;
	uint32_t _ps_bmsg_ioctl_ps_enable;
	uint32_t _ps_bmsg_ioctl_ps_disable;
	uint32_t _ps_bmsg_ioctl_exc32k_start;
	uint32_t _ps_bmsg_ioctl_exc32k_stop;
	uint32_t _pm_mac_bit;
	uint32_t _pm_mcu_bit;
	uint32_t _pm_rf_bit;
	uint32_t _pm_lpo_src_divd;
	uint32_t _pm_lpo_src_x32k;
	uint32_t _pm_lpo_src_rosc;
	uint32_t _pm_power_module_name_btsp;
	uint32_t _pm_power_module_name_wifip_mac;
	uint32_t _pm_power_module_name_phy;
	uint32_t _pm_power_module_state_off;
	uint32_t _pm_power_module_state_on;
	uint32_t _low_power_xtal_dpll_stability_delay_time;
	uint32_t _low_power_delay_time_hardware;
	uint32_t _low_power_extern_32k_hardware;
	uint32_t _low_power_26m_stability_delay_time_hardware;
	uint32_t _low_power_bootup_restore_delay_time;
	uint32_t _pm_power_sub_module_name_phy_wifi;
	uint32_t _pm_sleep_module_name_wifip_mac;
	uint32_t _pm_dev_id_mac;
	uint32_t _pm_cpu_frq_60m;
	uint32_t _pm_cpu_frq_80m;
	uint32_t _pm_cpu_frq_120m;
	uint32_t _pm_cpu_frq_high;
	uint32_t _pm_cpu_frq_default;
	uint32_t _pm_32k_step_begin;
	uint32_t _pm_32k_step_finish;
	uint32_t _cmd_rf_wifipll_hold_bit_set;
	uint32_t _cmd_rf_wifipll_hold_bit_clr;
	uint32_t _rf_wifipll_hold_by_wifi_bit;
	uint32_t _wifi_modem_en;
	uint32_t _wifi_modem_rc_en;
	uint32_t _wifi_mac_tx_rx_timer_int_bit;
	uint32_t _wifi_mac_tx_rx_misc_int_bit;
	uint32_t _wifi_mac_rx_trigger_int_bit;
	uint32_t _wifi_mac_tx_trigger_int_bit;
	uint32_t _wifi_mac_port_trigger_int_bit;
	uint32_t _wifi_mac_gen_int_bit;
	uint32_t _wifi_hsu_interrupt_ctrl_bit;
	uint32_t _wifi_mac_wakeup_int_bit;
	uint32_t _int_src_mac_general;
	uint32_t _int_src_mac_rx_trigger;
	uint32_t _int_src_mac_txrx_timer;
	uint32_t _int_src_mac_prot_trigger;
	uint32_t _int_src_mac_tx_trigger;
	uint32_t _int_src_modem;
	uint32_t _int_src_modem_rc;
	#if (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX)
	int32_t  _pm_low_voltage_delta_wakeup_delay_in_us;
	#endif
	bool _improve_he_tb_enable;
    uint32_t _OTP_CHIP_RESERVED;
} wifi_os_variable_t;
extern wifi_os_variable_t g_wifi_os_variable;


bk_err_t bk_wifi_interrupt_init(void);
bk_err_t bk_pm_sleep_register_wrapper(void *config_cb);
bk_err_t bk_pm_low_voltage_register_wrapper(void *enter_config_cb,void *exit_config_cb);

#endif

#endif // __LMAC_WIFI_ADAPTER_H_
