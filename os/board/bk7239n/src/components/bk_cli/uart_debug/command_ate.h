#ifndef _COMMAND_ATE_H_
#define _COMMAND_ATE_H_

#include <common/bk_include.h>

extern void sctrl_cali_dpll(UINT8 flag);
extern void sctrl_cal_adc(const unsigned char *content, UINT8 *tx_buffer);
extern void ate_cali_adc(const unsigned char *content, UINT8 *tx_buffer);
extern void ate_cali_sdmadc(const unsigned char *content, UINT8 *tx_buffer);
extern int sctrl_check_dpll_unlock(UINT8 bandgap, UINT8 *tx_buffer);
extern int sctrl_set_vdddig_1voltage(void);
extern int sctrl_load_vdddig_from_efuse(void);
extern int sctrl_convert_vdddig_from_efuse_to_user(INT8 in_value, INT8 *out_value);
extern int sctrl_convert_vdddig_from_user_to_efuse(INT8 in_value, INT8 *out_value);
extern int sctrl_convert_vdddig_from_user_to_reg(INT8 in_value, UINT32 *out_vdddig, INT32 *out_bandgap);
extern int sctrl_set_bandgap_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer);
extern int sctrl_set_bandgap_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer);
extern int sctrl_set_vdddig_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer);
extern int sctrl_set_dia_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer);
extern int sctrl_set_dia_to_reg(const unsigned char *content, int cnt, UINT8 *tx_buffer);

extern void bk_ate_Delay(volatile uint32_t times);
extern void uart_send_byte_for_ate(UINT8 data);
extern void uart_send_bytes_for_ate(UINT8* pData, UINT8 cnt);
extern void uart_fast_init(void);

extern void audio_loop_test(uint32_t state);
extern void audio_adc_mic2_to_dac_test(uint32_t state);
extern void audio_ap_test_for_ate(UINT8 enable, UINT8 test_mode, UINT8 sample_rate, UINT8 dac_gain, UINT8 adc_dac_mode, UINT8 clk_mode);

extern void send_chip_id(void);
extern int cmd_save_memcheck(void);
extern void cmd_start_softreset(uint8_t vcore, uint8_t wdt_delay);
extern void cmd_start_memcheck(void);
extern int cmd_do_memcheck(void);
extern int cmd_do_miccheck(void);
extern void bk7011_rf_vco_verify(UINT32 start);

extern bk_err_t bk_usb_ate_bist_test(uint32_t state);
extern bk_err_t bk_usb_ate_voh_vol_test(uint32_t state);
extern bk_err_t bk_usb_ate_rterm_test(uint32_t state);
extern bk_err_t bk_usb_ate_rx_dc_input_test(uint32_t state);
#endif // _COMMAND_ATE_H_
