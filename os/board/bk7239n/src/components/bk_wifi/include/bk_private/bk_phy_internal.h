/* Copyright 2020-2025 Beken */

/*
 * All the APIs declared here are internal only APIs, it can only be used by
 * Beken internal modules. Beken customers are not recommended to use them.
 *
 * If someone really want to use specified APIs declared in here, please contact
 * Beken AE/developer to make sure you know the limitations or risk of the API,
 * otherwise you may get unexpected behavior!!!
 *
 */

#ifndef _BK_PHY_INTERNAL_H_
#define _BK_PHY_INTERNAL_H_


////for wifi use
int manual_cal_rfcali_status(void);
void rc_drv_set_rf_en(uint32_t value);
uint32_t rc_drv_get_rf_en(void);
int rwnx_cal_set_rfconfig_BTPLL(void);
int rwnx_cal_set_rfconfig_WIFIPLL(void);

int calibration_init(void);
unsigned int rwnx_sys_is_enable_hw_tpc(void);
UINT32 rwnx_sys_is_use_abs_power(void);
void rwnx_sys_set_use_abs_power(UINT32 use);
UINT32 rwnx_is_enable_pwr_change_by_rssi(void);
void tpc_auto_change_pwr_by_rssi(INT8 rssi, UINT8 rate, UINT8 *pwr_idx);
void tpc_change_pwr_by_media(UINT8 rate, UINT8 *pwr_idx, UINT8 level);
void tpc_set_media_pwr_level(UINT8 level);
UINT8 tpc_get_media_pwr_level(void);
void evm_via_mac_continue(void);
void evm_via_mac_evt(int dummy);

int do_evm(void *cmdtp, int flag, int argc, char *const argv[]);
int do_rx_sensitivity(void *cmdtp, int flag, int argc, char *const argv[]);

////for rf use
INT32 rwnx_cal_save_trx_rcbekn_reg_val(void);
void rc_drv_set_agc_manual_en(uint32_t value);
void rc_drv_set_rf_macbypass(uint32_t value);
void rc_drv_set_rx_mode_enrxsw(uint32_t value);
uint32_t rc_drv_get_rx_mode_enrxsw(void);
UINT32 rwnx_tpc_get_pwridx_by_rate(UINT32 rate, UINT32 format, UINT32 print_log);
UINT32 rwnx_setting_for_single_rate(INT32 rate);
void delay05us(INT32 num);
void rwnx_cal_recover_rf_setting(void);
void rwnx_cal_recover_wifi_setting(void);
void rwnx_cal_set_40M_setting(void);
void rwnx_cal_set_20M_setting(void);
void rwnx_cal_recover_rcbeken_reg_val(void);
void rwnx_cal_recover_trx_reg_val(void);
void rwnx_cal_set_channel(UINT32 freq);
void bk7011_cal_dpd(uint32_t freq, uint32_t connect_mode);
void bk7011_cal_pll(void);
void bk7011_update_by_rx(int8_t rssi, int8_t freq_offset);
INT32 rwnx_cal_load_trx_rcbekn_reg_val(void);
void bk7011_max_rxsens_setting(void);
void bk7011_default_rxsens_setting(void);
UINT32 tx_evm_rate_get(void);
UINT32 tx_evm_pwr_idx_get(void);
UINT32 tx_evm_modul_format_get(void);
UINT32 tx_evm_guard_i_tpye_get(void);
UINT32 tx_evm_bandwidth_get(void);
UINT8 manual_get_epa_flag(void);
UINT32 rxsens_start_flag_get(void);
UINT32 tx_evm_mode_get(void);
void bk_cal_get_txpwr_tabl(uint8_t *cal_array_ptr);

#endif
