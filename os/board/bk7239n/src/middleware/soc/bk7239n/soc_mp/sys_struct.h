// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*reg0x00*/
typedef volatile union {
	struct {
		uint32_t deviceid                         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} system_device_id_t;

/*reg0x01*/
typedef volatile union {
	struct {
		uint32_t versionid                        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} system_version_id_t;

/*reg0x02*/
typedef volatile union {
	struct {
		uint32_t boot_mode                        :  1; /**<bit[0 : 0] */
		uint32_t reserved0                        :  3; /**<bit[1 : 3] */
		uint32_t rf_switch_manual_en              :  1; /**<bit[4 : 4] */
		uint32_t rf_source                        :  2; /**<bit[5 : 6] */
		uint32_t reserved1                        :  2; /**<bit[7 : 8] */
		uint32_t flash_sel                        :  1; /**<bit[9 : 9] */
		uint32_t fem_bps_txen                     :  1; /**<bit[10 : 10] */
		uint32_t reserved2                        : 21; /**<bit[11 : 31] */
	};
	uint32_t v;
} system_cpu_storage_connect_op_select_t;

/*reg0x03*/
typedef volatile union {
	struct {
		uint32_t core0_halted                     :  1; /**<bit[0 : 0] */
		uint32_t reserved_bit_1_1                 :  1; /**<bit[1 : 1] */
		uint32_t reserved0                        :  2; /**<bit[2 : 3] */
		uint32_t cpu0_sw_reset                    :  1; /**<bit[4 : 4] */
		uint32_t reserved1                        :  3; /**<bit[5 : 7] */
		uint32_t cpu0_pwr_dw_state                :  1; /**<bit[8 : 8] */
		uint32_t reserved2                        : 23; /**<bit[9 : 31] */
	};
	uint32_t v;
} system_cpu_current_run_status_t;

/*reg0x04*/
typedef volatile union {
	struct {
		uint32_t cpu0_sw_rst                      :  1; /**<bit[0 : 0] */
		uint32_t cpu0_pwr_dw                      :  1; /**<bit[1 : 1] */
		uint32_t cpu0_int_mask                    :  1; /**<bit[2 : 2] */
		uint32_t cpu0_halt                        :  1; /**<bit[3 : 3] */
		uint32_t cpu0_speed                       :  1; /**<bit[4 : 4] */
		uint32_t cpu0_rxevt_sel                   :  1; /**<bit[5 : 5] */
		uint32_t reserved_6_7                     :  2; /**<bit[6 : 7] */
		uint32_t cpu0_offset                      : 24; /**<bit[8 : 31] */
	};
	uint32_t v;
} system_cpu0_int_halt_clk_op_t;

/*reg0x08*/
typedef volatile union {
	struct {
		uint32_t clkdiv_core                      :  4; /**<bit[0 : 3] */
		uint32_t cksel_core                       :  1; /**<bit[4 : 4] */
		uint32_t reserved_bit_5_17                : 13; /**<bit[5 : 17] */
		uint32_t cksel_pwm0                       :  1; /**<bit[18 : 18] */
		uint32_t reserved_bit_19_19               :  1; /**<bit[19 : 19] */
		uint32_t cksel_tim0                       :  1; /**<bit[20 : 20] */
		uint32_t cksel_tim1                       :  1; /**<bit[21 : 21] */
		uint32_t reserved_bit_22_31               : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} system_cpu_clk_div_mode1_t;

/*reg0x09*/
typedef volatile union {
	struct {
		uint32_t reserved_0_3                     :  4; /**<bit[0 : 3] */
		uint32_t ckdiv_psram                      :  1; /**<bit[4 : 4] */
		uint32_t cksel_psram                      :  1; /**<bit[5 : 5] */
		uint32_t reserved_bit_6_13                :  8; /**<bit[6 : 13] */
		uint32_t ckdiv_sdio                       :  2; /**<bit[14 : 15] */
		uint32_t cksel_sdio                       :  2; /**<bit[16 : 17] */
		uint32_t ckdiv_auxs                       :  4; /**<bit[18 : 21] */
		uint32_t cksel_auxs                       :  2; /**<bit[22 : 23] */
		uint32_t cksel_flash                      :  2; /**<bit[24 : 25] */
		uint32_t ckdiv_flash                      :  2; /**<bit[26 : 27] */
		uint32_t reserved_bit_28_30               :  3; /**<bit[28 : 30] */
		uint32_t clkdiv_auxs                      :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu_clk_div_mode2_t;

/*reg0x0a*/
typedef volatile union {
	struct {
		uint32_t ckdiv_xtal                        :  2; /**<bit[0 : 1] */
		uint32_t ckdiv_wdt                        :  2; /**<bit[2 : 3] */
		uint32_t clksel_spi0                      :  2; /**<bit[4 : 5] */
		uint32_t clksel_spi1                      :  2; /**<bit[6 : 7] */
		uint32_t cksel_qspi1                      :  1; /**<bit[8 : 8] */
		uint32_t reserved_9_9                     :  1; /**<bit[9 : 9] */
		uint32_t cksel_i2c0                      :   1; /**<bit[10 : 10] */
		uint32_t cksel_i2c1                      :   1; /**<bit[11 : 11] */
		uint32_t cksel_uart0                      :  1; /**<bit[12 : 12] */
		uint32_t cksel_uart1                      :  1; /**<bit[13 : 13] */
		uint32_t cksel_uart2                      :  1; /**<bit[14 : 14] */
		uint32_t cksel_uart3                      :  1; /**<bit[15 : 15] */
		uint32_t ckdiv_qspi1                      :  2; /**<bit[16 : 17] */
		uint32_t reserved_18_31                   : 14; /**<bit[18 : 31] */
	};
	uint32_t v;
} system_cpu_26m_wdt_clk_div_t;

/*reg0x0b*/
typedef volatile union {
	struct {
		uint32_t anaspi_freq                      :  6; /**<bit[0 : 5] */
		uint32_t reserved_6_31                    : 26; /**<bit[6 : 31] */
	};
	uint32_t v;
} system_cpu_anaspi_freq_t;

/*reg0x0c*/
typedef volatile union {
	struct {
		uint32_t i2c0_cken                        :  1; /**<bit[0 : 0] */
		uint32_t spi0_cken                        :  1; /**<bit[1 : 1] */
		uint32_t uart0_cken                       :  1; /**<bit[2 : 2] */
		uint32_t pwm0_cken                        :  1; /**<bit[3 : 3] */
		uint32_t tim0_cken                        :  1; /**<bit[4 : 4] */
		uint32_t sadc_cken                        :  1; /**<bit[5 : 5] */
		uint32_t uart3_cken                       :  1; /**<bit[6 : 6] */
		uint32_t gpio_cken                        :  1; /**<bit[7 : 7] */
		uint32_t i2c1_cken                        :  1; /**<bit[8 : 8] */
		uint32_t spi1_cken                        :  1; /**<bit[9 : 9] */
		uint32_t uart1_cken                       :  1; /**<bit[10 : 10] */
		uint32_t uart2_cken                       :  1; /**<bit[11 : 11] */
		uint32_t reserved_bit_12_12               :  1; /**<bit[12 : 12] */
		uint32_t tim1_cken                        :  1; /**<bit[13 : 13] */
		uint32_t reserved_bit_14_14               :  1; /**<bit[14 : 14] */
		uint32_t otp_cken                         :  1; /**<bit[15 : 15] */
		uint32_t i2s_cken                         :  1; /**<bit[16 : 16] */
		uint32_t led_cken                         :  1; /**<bit[17 : 17] */
		uint32_t tamp_cken                        :  1; /**<bit[18 : 18] */
		uint32_t psram_cken                       :  1; /**<bit[19 : 19] */
		uint32_t dplldiv_cken                     :  1; /**<bit[20 : 20] */
		uint32_t qspi1_cken                       :  1; /**<bit[21 : 21] */
		uint32_t sdio_cken                        :  1; /**<bit[22 : 22] */
		uint32_t auxs_cken                        :  1; /**<bit[23 : 23] */
		uint32_t btdm_cken                        :  1; /**<bit[24 : 24] */
		uint32_t xvr_cken                         :  1; /**<bit[25 : 25] */
		uint32_t mac_cken                         :  1; /**<bit[26 : 26] */
		uint32_t phy_cken                         :  1; /**<bit[27 : 27] */
		uint32_t thread_cken                      :  1; /**<bit[28 : 28] */
		uint32_t rf_cken                          :  1; /**<bit[29 : 29] */
		uint32_t reserved_bit_30_30               :  1; /**<bit[30 : 30] */
		uint32_t wdt_cken                         :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu_device_clk_enable_t;

/*reg0x0f*/
typedef volatile union {
	struct {
		uint32_t uart1_ds                         :  1; /**<bit[0 : 0] */
		uint32_t uart2_ds                         :  1; /**<bit[1 : 1] */
		uint32_t spi1_ds                          :  1; /**<bit[2 : 2] */
		uint32_t sdio_ds                          :  1; /**<bit[3 : 3] */
		uint32_t uart3_ds                         :  1; /**<bit[4 : 4] */
		uint32_t led_ds                           :  1; /**<bit[5 : 5] */
		uint32_t reserved_6_6                     :  1; /**<bit[6 : 6] */
		uint32_t qspi1_ds                         :  1; /**<bit[7 : 7] */
		uint32_t pram_ds                          :  1; /**<bit[8 : 8] */
		uint32_t reserved0                        :  3; /**<bit[9 : 11] */
		uint32_t i2s0_ds                          :  1; /**<bit[12 : 12] */
		uint32_t reserved_13_20                   :  8; /**<bit[13 : 20] */
		uint32_t mac_ds                           :  1; /**<bit[21 : 21] */
		uint32_t phy_ds                           :  1; /**<bit[22 : 22] */
		uint32_t xvr_ds                           :  1; /**<bit[23 : 23] */
		uint32_t reserved_bit_24_24               :  1; /**<bit[24 : 24] */
		uint32_t la_ds                            :  1; /**<bit[25 : 25] */
		uint32_t flsh_ds                          :  1; /**<bit[26 : 26] */
		uint32_t uart_ds                          :  1; /**<bit[27 : 27] */
		uint32_t spi0_ds                          :  1; /**<bit[28 : 28] */
		uint32_t enc_ds                           :  1; /**<bit[29 : 29] */
		uint32_t dma0_ds                          :  1; /**<bit[30 : 30] */
		uint32_t reserved1                        :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu_device_mem_ctrl2_t;

/*reg0x10*/
typedef volatile union {
	struct {
		uint32_t pwd_mem1                         :  1; /**<bit[0 : 0] */
		uint32_t pwd_mem2                         :  1; /**<bit[1 : 1] */
		uint32_t pwd_mem3                         :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_3                 :  1; /**<bit[3 : 3] */
		uint32_t pwd_bakp                         :  1; /**<bit[4 : 4] */
		uint32_t reserved_bit_5_7                 :  3; /**<bit[5 : 7] */
		uint32_t pwd_btsp                         :  1; /**<bit[8 : 8] */
		uint32_t pwd_wifp_mac                     :  1; /**<bit[9 : 9] */
		uint32_t pwd_wifp_phy                     :  1; /**<bit[10 : 10] */
		uint32_t pwd_mem0                         :  1; /**<bit[11 : 11] */
		uint32_t pwd_mem4                         :  1; /**<bit[12 : 12] */
		uint32_t pwd_ofdm                         :  1; /**<bit[13 : 13] */
		uint32_t reserved_bit_14_14               :  1; /**<bit[14 : 14] */
		uint32_t rom_pgen                         :  1; /**<bit[15 : 15] */
		uint32_t sleep_en_need_flash_idle         :  1; /**<bit[16 : 16] */
		uint32_t reserved_bit_17_17               :  1; /**<bit[17 : 17] */
		uint32_t sleep_en_need_cpu0_wfi           :  1; /**<bit[18 : 18] */
		uint32_t sleep_en_global                  :  1; /**<bit[19 : 19] */
		uint32_t sleep_bus_idle_bypass            :  1; /**<bit[20 : 20] */
		uint32_t reserved_bit_21_21               :  1; /**<bit[21 : 21] */
		uint32_t bts_soft_wakeup_req              :  1; /**<bit[22 : 22] */
		uint32_t rom_rd_disable                   :  1; /**<bit[23 : 23] */
		uint32_t otp_rd_disable                   :  1; /**<bit[24 : 24] */
		uint32_t tcm0_pgen                        :  1; /**<bit[25 : 25] */
		uint32_t cpu0_subpwdm_en                  :  1; /**<bit[26 : 26] */
		uint32_t te200_clkgat_bps                 :  1; /**<bit[27 : 27] */
		uint32_t share_mem_clkgating_disable      :  1; /**<bit[28 : 28] */
		uint32_t cpu0_ticktimer_32k_enable        :  1; /**<bit[29 : 29] */
		uint32_t reserved_30_30                   :  1; /**<bit[30 : 30] */
		uint32_t busmatrix_busy                   :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu_power_sleep_wakeup_t;

/*reg0x11*/
typedef volatile union {
	struct {
		uint32_t cpu0_cache_ret_en                :  1; /**<bit[0 : 0] */
		uint32_t cpu0_cache_sleeppwd_en           :  1; /**<bit[1 : 1] */
		uint32_t reserved_2_31                    : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} system_cpu0_lv_sleep_cfg_t;

/*reg0x12*/
typedef volatile union {
	struct {
		uint32_t ram0_ds                          :  1; /**<bit[0 : 0] */
		uint32_t ram1_ds                          :  1; /**<bit[1 : 1] */
		uint32_t ram2_ds                          :  1; /**<bit[2 : 2] */
		uint32_t ram3_ds                          :  1; /**<bit[3 : 3] */
		uint32_t ram4_ds                          :  1; /**<bit[4 : 4] */
		uint32_t reserved0                        : 11; /**<bit[5 : 15] */
		uint32_t ram_config                       : 10; /**<bit[16 : 25] */
		uint32_t reserved1                        :  6; /**<bit[26 : 31] */
	};
	uint32_t v;
} system_cpu_device_mem_ctrl3_t;

/*reg0x20*/
typedef volatile union {
	struct {
		uint32_t cpu0_dma_nsec_intr_en            :  1; /**<bit[0 : 0] */
		uint32_t cpu0_encp_sec_intr_en            :  1; /**<bit[1 : 1] */
		uint32_t cpu0_encp_nsec_intr_en           :  1; /**<bit[2 : 2] */
		uint32_t cpu0_timer_int_en                :  1; /**<bit[3 : 3] */
		uint32_t cpu0_uart0_int_en                :  1; /**<bit[4 : 4] */
		uint32_t cpu0_pwm0_int_en                 :  1; /**<bit[5 : 5] */
		uint32_t cpu0_i2c0_int_en                 :  1; /**<bit[6 : 6] */
		uint32_t cpu0_spi0_int_en                 :  1; /**<bit[7 : 7] */
		uint32_t cpu0_sadc_int_en                 :  1; /**<bit[8 : 8] */
		uint32_t cpu0_uart3_int_en                :  1; /**<bit[9 : 9] */
		uint32_t cpu0_sdio_int_en                 :  1; /**<bit[10 : 10] */
		uint32_t cpu0_gdma_int_en                 :  1; /**<bit[11 : 11] */
		uint32_t cpu0_la_int_en                   :  1; /**<bit[12 : 12] */
		uint32_t cpu0_timer1_int_en               :  1; /**<bit[13 : 13] */
		uint32_t cpu0_i2c1_int_en                 :  1; /**<bit[14 : 14] */
		uint32_t cpu0_uart1_int_en                :  1; /**<bit[15 : 15] */
		uint32_t cpu0_uart2_int_en                :  1; /**<bit[16 : 16] */
		uint32_t cpu0_spi1_int_en                 :  1; /**<bit[17 : 17] */
		uint32_t cpu0_led_int_en                  :  1; /**<bit[18 : 18] */
		uint32_t cpu0_resv0_int_en                :  1; /**<bit[19 : 19] */
		uint32_t cpu0_resv1_int_en                :  1; /**<bit[20 : 20] */
		uint32_t cpu0_ckmn_int_en                 :  1; /**<bit[21 : 21] */
		uint32_t cpu0_resv2_int_en                :  1; /**<bit[22 : 22] */
		uint32_t cpu0_resv3_int_en                :  1; /**<bit[23 : 23] */
		uint32_t cpu0_i2s0_int_en                 :  1; /**<bit[24 : 24] */
		uint32_t cpu0_resv4_int_en                :  1; /**<bit[25 : 25] */
		uint32_t cpu0_resv5_int_en                :  1; /**<bit[26 : 26] */
		uint32_t cpu0_resv6_int_en                :  1; /**<bit[27 : 27] */
		uint32_t cpu0_resv7_int_en                :  1; /**<bit[28 : 28] */
		uint32_t cpu0_int_mac_wakeup_int_en       :  1; /**<bit[29 : 29] */
		uint32_t cpu0_hsu_irq_int_en              :  1; /**<bit[30 : 30] */
		uint32_t cpu0_mac_int_gen_int_en          :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu0_int_0_31_en_t;

/*reg0x21*/
typedef volatile union {
	struct {
		uint32_t cpu0_mac_int_port_trigger_int_en :  1; /**<bit[0 : 0] */
		uint32_t cpu0_mac_int_tx_trigger_int_en   :  1; /**<bit[1 : 1] */
		uint32_t cpu0_mac_int_rx_trigger_int_en   :  1; /**<bit[2 : 2] */
		uint32_t cpu0_mac_int_tx_rx_misc_int_en   :  1; /**<bit[3 : 3] */
		uint32_t cpu0_mac_int_tx_rx_timer_int_en  :  1; /**<bit[4 : 4] */
		uint32_t cpu0_phy_riu_int_int_en          :  1; /**<bit[5 : 5] */
		uint32_t cpu0_phy_mbp_int_int_en          :  1; /**<bit[6 : 6] */
		uint32_t cpu0_bt_irq_int_en               :  1; /**<bit[7 : 7] */
		uint32_t cpu0_ble_irq_int_en              :  1; /**<bit[8 : 8] */
		uint32_t cpu0_dm_irq_int_en               :  1; /**<bit[9 : 9] */
		uint32_t cpu0_qspi0_int_en                :  1; /**<bit[10 : 10] */
		uint32_t cpu0_resv8_int_en                :  1; /**<bit[11 : 11] */
		uint32_t cpu0_resv9_int_en                :  1; /**<bit[12 : 12] */
		uint32_t cpu0_resv10_int_en               :  1; /**<bit[13 : 13] */
		uint32_t cpu0_resv11_int_en               :  1; /**<bit[14 : 14] */
		uint32_t cpu0_resv12_int_en               :  1; /**<bit[15 : 15] */
		uint32_t cpu0_thread_int_en               :  1; /**<bit[16 : 16] */
		uint32_t cpu0_resv13_int_en               :  1; /**<bit[17 : 17] */
		uint32_t cpu0_otp_int_en                  :  1; /**<bit[18 : 18] */
		uint32_t cpu0_dpllunlock_int_en           :  1; /**<bit[19 : 19] */
		uint32_t cpu0_resv14_int_en               :  1; /**<bit[20 : 20] */
		uint32_t cpu0_resv15_int_en               :  1; /**<bit[21 : 21] */
		uint32_t cpu0_resv16_int_en               :  1; /**<bit[22 : 22] */
		uint32_t cpu0_gpio_s_int_en               :  1; /**<bit[23 : 23] */
		uint32_t cpu0_gpio_ns_int_en              :  1; /**<bit[24 : 24] */
		uint32_t cpu0_resv17_int_en               :  1; /**<bit[25 : 25] */
		uint32_t cpu0_ana_gpio_int_en             :  1; /**<bit[26 : 26] */
		uint32_t cpu0_ana_rtc_int_en              :  1; /**<bit[27 : 27] */
		uint32_t cpu0_abnormal_gpio_int_en        :  1; /**<bit[28 : 28] */
		uint32_t cpu0_abnormal_rtc_int_en         :  1; /**<bit[29 : 29] */
		uint32_t cpu0_resv18_int_en               :  1; /**<bit[30 : 30] */
		uint32_t cpu0_resv19_int_en               :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu0_int_32_63_en_t;

/*reg0x28*/
typedef volatile union {
	struct {
		uint32_t cpu0_dma_nsec_intr_st            :  1; /**<bit[0 : 0] */
		uint32_t cpu0_stcp_sec_intr_st            :  1; /**<bit[1 : 1] */
		uint32_t cpu0_stcp_nsec_intr_st           :  1; /**<bit[2 : 2] */
		uint32_t cpu0_timer_int_st                :  1; /**<bit[3 : 3] */
		uint32_t cpu0_uart0_int_st                :  1; /**<bit[4 : 4] */
		uint32_t cpu0_pwm0_int_st                 :  1; /**<bit[5 : 5] */
		uint32_t cpu0_i2c0_int_st                 :  1; /**<bit[6 : 6] */
		uint32_t cpu0_spi0_int_st                 :  1; /**<bit[7 : 7] */
		uint32_t cpu0_sadc_int_st                 :  1; /**<bit[8 : 8] */
		uint32_t cpu0_uart3_int_st                :  1; /**<bit[9 : 9] */
		uint32_t cpu0_sdio_int_st                 :  1; /**<bit[10 : 10] */
		uint32_t cpu0_gdma_int_st                 :  1; /**<bit[11 : 11] */
		uint32_t cpu0_la_int_st                   :  1; /**<bit[12 : 12] */
		uint32_t cpu0_timer1_int_st               :  1; /**<bit[13 : 13] */
		uint32_t cpu0_i2c1_int_st                 :  1; /**<bit[14 : 14] */
		uint32_t cpu0_uart1_int_st                :  1; /**<bit[15 : 15] */
		uint32_t cpu0_uart2_int_st                :  1; /**<bit[16 : 16] */
		uint32_t cpu0_spi1_int_st                 :  1; /**<bit[17 : 17] */
		uint32_t cpu0_led_int_st                  :  1; /**<bit[18 : 18] */
		uint32_t cpu0_resv0_int_st                :  1; /**<bit[19 : 19] */
		uint32_t cpu0_resv1_int_st                :  1; /**<bit[20 : 20] */
		uint32_t cpu0_ckmn_int_st                 :  1; /**<bit[21 : 21] */
		uint32_t cpu0_resv2_int_st                :  1; /**<bit[22 : 22] */
		uint32_t cpu0_resv3_int_st                :  1; /**<bit[23 : 23] */
		uint32_t cpu0_i2s0_int_st                 :  1; /**<bit[24 : 24] */
		uint32_t cpu0_resv4_int_st                :  1; /**<bit[25 : 25] */
		uint32_t cpu0_resv5_int_st                :  1; /**<bit[26 : 26] */
		uint32_t cpu0_resv6_int_st                :  1; /**<bit[27 : 27] */
		uint32_t cpu0_resv7_int_st                :  1; /**<bit[28 : 28] */
		uint32_t cpu0_int_mac_wakeup_int_st       :  1; /**<bit[29 : 29] */
		uint32_t cpu0_hsu_irq_int_st              :  1; /**<bit[30 : 30] */
		uint32_t cpu0_mac_int_gst_int_st          :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu0_int_0_31_status_t;

/*reg0x29*/
typedef volatile union {
	struct {
		uint32_t cpu0_mac_int_port_trigger_int_st :  1; /**<bit[0 : 0] */
		uint32_t cpu0_mac_int_tx_trigger_int_st   :  1; /**<bit[1 : 1] */
		uint32_t cpu0_mac_int_rx_trigger_int_st   :  1; /**<bit[2 : 2] */
		uint32_t cpu0_mac_int_tx_rx_misc_int_st   :  1; /**<bit[3 : 3] */
		uint32_t cpu0_mac_int_tx_rx_timer_int_st  :  1; /**<bit[4 : 4] */
		uint32_t cpu0_phy_riu_int_int_st          :  1; /**<bit[5 : 5] */
		uint32_t cpu0_phy_mbp_int_int_st          :  1; /**<bit[6 : 6] */
		uint32_t cpu0_bt_irq_int_st               :  1; /**<bit[7 : 7] */
		uint32_t cpu0_ble_irq_int_st              :  1; /**<bit[8 : 8] */
		uint32_t cpu0_dm_irq_int_st               :  1; /**<bit[9 : 9] */
		uint32_t cpu0_qspi0_int_st                :  1; /**<bit[10 : 10] */
		uint32_t cpu0_resv8_int_st                :  1; /**<bit[11 : 11] */
		uint32_t cpu0_resv9_int_st                :  1; /**<bit[12 : 12] */
		uint32_t cpu0_resv10_int_st               :  1; /**<bit[13 : 13] */
		uint32_t cpu0_resv11_int_st               :  1; /**<bit[14 : 14] */
		uint32_t cpu0_resv12_int_st               :  1; /**<bit[15 : 15] */
		uint32_t cpu0_thread_int_st               :  1; /**<bit[16 : 16] */
		uint32_t cpu0_resv13_int_st               :  1; /**<bit[17 : 17] */
		uint32_t cpu0_otp_int_st                  :  1; /**<bit[18 : 18] */
		uint32_t cpu0_dpllunlock_int_st           :  1; /**<bit[19 : 19] */
		uint32_t cpu0_resv14_int_st               :  1; /**<bit[20 : 20] */
		uint32_t cpu0_resv15_int_st               :  1; /**<bit[21 : 21] */
		uint32_t cpu0_resv16_int_st               :  1; /**<bit[22 : 22] */
		uint32_t cpu0_gpio_s_int_st               :  1; /**<bit[23 : 23] */
		uint32_t cpu0_gpio_ns_int_st              :  1; /**<bit[24 : 24] */
		uint32_t cpu0_resv17_int_st               :  1; /**<bit[25 : 25] */
		uint32_t cpu0_ana_gpio_int_st             :  1; /**<bit[26 : 26] */
		uint32_t cpu0_ana_rtc_int_st              :  1; /**<bit[27 : 27] */
		uint32_t cpu0_abnormal_gpio_int_st        :  1; /**<bit[28 : 28] */
		uint32_t cpu0_abnormal_rtc_int_st         :  1; /**<bit[29 : 29] */
		uint32_t cpu0_resv18_int_st               :  1; /**<bit[30 : 30] */
		uint32_t cpu0_resv19_int_st               :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} system_cpu0_int_32_63_status_t;

/*reg0x30*/
typedef volatile union {
	struct {
		uint32_t gpio_config0                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} system_gpio_config0_t;

/*reg0x31*/
typedef volatile union {
	struct {
		uint32_t gpio_config1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} system_gpio_config1_t;

/*reg0x32*/
typedef volatile union {
	struct {
		uint32_t gpio_config2                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} system_gpio_config2_t;

/*reg0x38*/
typedef volatile union {
	struct {
		uint32_t dbug_msg                         : 16; /**<bit[0 : 15] */
		uint32_t dbug_mux                         :  4; /**<bit[16 : 19] */
		uint32_t reserved_20_31                   : 12; /**<bit[20 : 31] */
	};
	uint32_t v;
} system_dbug_config0_t;

/*reg0x3a*/
typedef volatile union {
	struct {
		uint32_t anareg_stat                      : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} system_anareg_stat_t;

typedef volatile struct {
	volatile system_device_id_t device_id;
	volatile system_version_id_t version_id;
	volatile system_cpu_storage_connect_op_select_t cpu_storage_connect_op_select;
	volatile system_cpu_current_run_status_t cpu_current_run_status;
	volatile system_cpu0_int_halt_clk_op_t cpu0_int_halt_clk_op;
	volatile uint32_t rsv_5_7[3];
	volatile system_cpu_clk_div_mode1_t cpu_clk_div_mode1;
	volatile system_cpu_clk_div_mode2_t cpu_clk_div_mode2;
	volatile system_cpu_26m_wdt_clk_div_t cpu_26m_wdt_clk_div;
	volatile system_cpu_anaspi_freq_t cpu_anaspi_freq;
	volatile system_cpu_device_clk_enable_t cpu_device_clk_enable;
	volatile uint32_t rsv_d_e[2];
	volatile system_cpu_device_mem_ctrl2_t cpu_device_mem_ctrl2;
	volatile system_cpu_power_sleep_wakeup_t cpu_power_sleep_wakeup;
	volatile system_cpu0_lv_sleep_cfg_t cpu0_lv_sleep_cfg;
	volatile system_cpu_device_mem_ctrl3_t cpu_device_mem_ctrl3;
	volatile uint32_t rsv_13_1f[13];
	volatile system_cpu0_int_0_31_en_t cpu0_int_0_31_en;
	volatile system_cpu0_int_32_63_en_t cpu0_int_32_63_en;
	volatile uint32_t rsv_22_27[6];
	volatile system_cpu0_int_0_31_status_t cpu0_int_0_31_status;
	volatile system_cpu0_int_32_63_status_t cpu0_int_32_63_status;
	volatile uint32_t rsv_2a_2f[6];
	volatile system_gpio_config0_t gpio_config0;
	volatile system_gpio_config1_t gpio_config1;
	volatile system_gpio_config2_t gpio_config2;
	volatile uint32_t rsv_33_37[5];
	volatile system_dbug_config0_t dbug_config0;
	volatile uint32_t rsv_39_39[1];
	volatile system_anareg_stat_t anareg_stat;
} sys_hw_t;

#ifdef __cplusplus
}
#endif
