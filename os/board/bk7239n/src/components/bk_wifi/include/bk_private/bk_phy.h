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

//TODO more optimization for APIs in this headers

#ifdef __cplusplus
extern "C" {
#endif

uint8_t phy_open_cca(void);
uint8_t phy_close_cca(void);
uint8_t phy_show_cca(void);

void cmd_rfcali_cfg_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_get_cfg_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_rate_dist(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_tssi_g(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_tssi_b(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_get_cfg_tssi(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_show_data(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_show_result(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_to_flash(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_cali(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_auto_pwr_change_by_rssi(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_flag_in_otp2(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rf_set_pwr_idx(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rf_get_pwr_idx(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if CONFIG_PSRAM
void cmd_psram(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

void tx_evm_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void rx_sens_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int bkreg_run_command(const char *content, int cnt);

//TODO move to better place
void bk7011_cal_bias(void);

int manual_cal_get_macaddr_from_flash(uint8_t *mac_ptr);
int manual_cal_write_macaddr_to_flash(uint8_t *mac_ptr);
UINT8 manual_cal_read_otp_flash(UINT32 addr, UINT32 len, UINT8 *buf);
UINT8 manual_cal_wirte_otp_flash(UINT32 addr, UINT32 len, UINT8 *buf);
void manual_cal_temp_pwr_unint(void);

int do_evm(void *cmdtp, int flag, int argc, char *const argv[]);
int do_rx_sensitivity(void *cmdtp, int flag, int argc, char *const argv[]);

#define LA_SAMPLE_TEST 1    
void cmd_la_sample_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

UINT32 bk7011_set_sample_mac_out_for_ate(UINT32 sample_len);
void bk7011_clear_sample_mac_out_for_ate(void);
UINT32 bk7011_get_sample_mac_out_for_ate(UINT8 **buf, uint32_t *len);

#ifdef __cplusplus
}
#endif
