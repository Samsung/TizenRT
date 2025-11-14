
/* this file is only used by command_ate.c
*******************************************/

//#if (CONFIG_ATE_TEST)


#ifndef _BK_PHY_ATE_H_
#define _BK_PHY_ATE_H_

void evm_stop_bypass_mac(void);
void evm_init_bypass_mac(void);

void rwnx_no_use_tpc_set_pwr(void);
UINT32 evm_get_auto_tx_len(UINT32 rate, UINT32 modul_format, UINT32 bandwidth, UINT32 test_mode);

UINT32 evm_bypass_mac_set_tx_data_length(UINT32 modul_format, UINT32 len, UINT32 rate, UINT32 bandwidth, UINT32 need_change);
UINT32 evm_bypass_mac_set_rate_mformat(UINT32 ppdu_rate, UINT32 m_format);

void evm_set_bandwidth(UINT32 bandwidth);
void evm_bypass_mac_set_guard_i_type(UINT32 gi_tpye);
void evm_bypass_mac_set_fec_coding(UINT32 fec_coding);
void rwnx_cal_en_extra_txpa(void);
void evm_start_bypass_mac(void);
void bk7011_stop_tx_pattern(void);
void rwnx_cal_set_40M_extra_setting(UINT8 val);
void rwnx_cal_set_txpwr(UINT32 pwr_gain, UINT32 grate);
void rwnx_cal_set_txpwr_by_rate(INT32 rate, UINT32 test_mode);

void rx_get_rx_result_end_for_ate(void *param1, void *param2);
void bk7011_cal_dpll(void);
UINT32 evm_translate_tx_rate(UINT32 rate);
void rc_drv_set_tx_pre_gain_2nd(uint32_t value);

void evm_bypass_set_single_carrier(UINT32 bk_rate, UINT32 rfband, UINT32 bandwidth, UINT32 channel);
void rx_get_rx_result_begin_for_ate(void);
void manual_cal_set_xtal(UINT32 xtal);
void evm_bypass_mac_test_for_ate(UINT32 frequency, UINT32 bandwidth);

void evm_start_bypass_mac_for_ate(void);

uint8_t evm_add_key(void);
void evm_del_key(uint8_t key_idx);

#endif 

//#endif
