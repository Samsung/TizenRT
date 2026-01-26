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


typedef volatile union {
	struct {
		uint32_t memchk_bps               :  1; /**<bit[0 : 0] */
		uint32_t fast_boot                :  1; /**<bit[1 : 1] */
		uint32_t dig_wake_en              :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_12        : 10; /**<bit[3 :12] */
		uint32_t saved_time               : 10; /**<bit[13:22] */
		uint32_t dlv_startup              :  1; /**<bit[23:23] */
		uint32_t reset_reason             :  7; /**<bit[24:30] */
		uint32_t gpio_sleep               :  1; /**<bit[31:31] */
	};
	uint32_t v;
} aon_pmu_r0_t;


typedef volatile union {
	struct {
		uint32_t wdt_rst_ana              :  1; /**<bit[0 : 0] */
		uint32_t wdt_rst_top              :  1; /**<bit[1 : 1] */
		uint32_t wdt_rst_aon              :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_7         :  5; /**<bit[3 : 7] */
		uint32_t rtc_value_samp_sel       :  1; /**<bit[8 : 8] */
		uint32_t reserved_bit_9_30        : 22; /**<bit[9 :30] */
		uint32_t otp_vdd_en               :  1; /**<bit[31:31] */
	};
	uint32_t v;
} aon_pmu_r2_t;


typedef volatile union {
	struct {
		uint32_t rsv                      : 32; /**<bit[0 :31] */
	};
	uint32_t v;
} aon_pmu_r25_t;


typedef volatile union {
	struct {
		uint32_t wake1_delay              :  4; /**<bit[0 : 3] */
		uint32_t wake2_delay              :  4; /**<bit[4 : 7] */
		uint32_t wake3_delay              :  4; /**<bit[8 :11] */
		uint32_t halt1_delay              :  4; /**<bit[12:15] */
		uint32_t halt2_delay              :  4; /**<bit[16:19] */
		uint32_t halt3_delay              :  4; /**<bit[20:23] */
		uint32_t halt_volt                :  1; /**<bit[24:24] */
		uint32_t halt_xtal                :  1; /**<bit[25:25] */
		uint32_t halt_core                :  1; /**<bit[26:26] */
		uint32_t halt_flash               :  1; /**<bit[27:27] */
		uint32_t halt_rosc                :  1; /**<bit[28:28] */
		uint32_t halt_resten              :  1; /**<bit[29:29] */
		uint32_t halt_isolat              :  1; /**<bit[30:30] */
		uint32_t halt_clkena              :  1; /**<bit[31:31] */
	};
	uint32_t v;
} aon_pmu_r40_t;


typedef volatile union {
	struct {
		uint32_t lpo_config               :  2; /**<bit[0 : 1] */
		uint32_t flshsck_iocap            :  2; /**<bit[2 : 3] */
		uint32_t wakeup_ena               :  6; /**<bit[4 : 9] */
		uint32_t gpio_clksel              :  1; /**<bit[10:10] */
		uint32_t reserved_bit_11_23       : 13; /**<bit[11:23] */
		uint32_t halt_lpo                 :  1; /**<bit[24:24] */
		uint32_t halt_cpu                 :  1; /**<bit[25:25] */
		uint32_t halt_anareg              :  1; /**<bit[26:26] */
		uint32_t dpll_lpen                :  1; /**<bit[27:27] */
		uint32_t reserved_bit_28_31       :  4; /**<bit[28:31] */
	};
	uint32_t v;
} aon_pmu_r41_t;


typedef volatile union {
	struct {
		uint32_t reserved_bit_0_19        : 20; /**<bit[0 :19] */
		uint32_t wakeup_source            :  2; /**<bit[20:21] */
		uint32_t abnormal_wakeup_source   :  2; /**<bit[22:23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24:31] */
	};
	uint32_t v;
} aon_pmu_r71_t;

typedef volatile union {
	struct {
		uint32_t finecnt_32k_samp         : 10; /**<bit[0 : 9]*/
		uint32_t ble_cnt_rvld             :  1; /**<bit[10:10]*/
		uint32_t reserved_bit_11_31       : 21; /**<bit[11:31]*/
	};
	uint32_t v;
} aon_pmu_r72_t;

typedef volatile union {
	struct {
		uint32_t clkcnt_32k_samp          : 28; /**<bit[0 :27]*/
		uint32_t reserved_bit_28_31       :  4; /**<bit[28:31]*/
	};
	uint32_t v;
} aon_pmu_r73_t;

typedef volatile union {
	struct {
		uint32_t isocnt_32k_samp          : 32; /**<bit[0 :27]*/
	};
	uint32_t v;
} aon_pmu_r74_t;

typedef volatile union {
	struct {
		uint32_t ana_gpio_sta             : 24; /**<bit[0 :23]*/
		uint32_t L                        :  8; /**<bit[24:31]*/
	};
	uint32_t v;
} aon_pmu_r77_t;

typedef volatile union {
	struct {
		uint32_t rtc_tick_h               :  4; /**<bit[0 : 3]*/
		uint32_t reserved_bit_4_31        : 28; /**<bit[4 :31]*/
	};
	uint32_t v;
} aon_pmu_r78_t;

typedef volatile union {
	struct {
		uint32_t rtc_tick_l               : 32; /**<bit[0 :31]*/
	};
	uint32_t v;
} aon_pmu_r79_t;
typedef volatile union {
	struct {
		uint32_t rsv                      : 32; /**<bit[0 :31]*/
	};
	uint32_t v;
} aon_pmu_r7a_t;

typedef volatile union {
	struct {
		uint32_t rsv                      : 32; /**<bit[0 :31]*/
	};
	uint32_t v;
} aon_pmu_r7b_t;

typedef volatile union {
	struct {
		uint32_t id                       : 32; /**<bit[0 :31]*/
	};
	uint32_t v;
} aon_pmu_r7c_t;


typedef volatile union {
	struct {
		uint32_t adc_cal                  :  6; /**<bit[0 : 5]*/
		uint32_t bgcal                    :  6; /**<bit[6 :11]*/
		uint32_t dpll_unlockL             :  1; /**<bit[12:12]*/
		uint32_t dpll_unlockH             :  1; /**<bit[13:13]*/
		uint32_t dpll_band                :  7; /**<bit[14:20]*/
		uint32_t L                        : 11; /**<bit[21:31]*/
	};
	uint32_t v;
} aon_pmu_r7d_t;


typedef volatile union {
	struct {
		uint32_t cbcal                    :  5; /**<bit[0 : 4]*/
		uint32_t band_cal                 : 11; /**<bit[5 :15]*/
		uint32_t zcd_cal                  :  6; /**<bit[16:21]*/
		uint32_t L                        : 10; /**<bit[22:31]*/
	};
	uint32_t v;
} aon_pmu_r7e_t;

typedef volatile union {
	struct {
		uint32_t td_int                   :  1; /**<bit[0 : 0]*/
		uint32_t td_chen                  : 16; /**<bit[1 :16]*/
		uint32_t ad_state                 :  2; /**<bit[17:18]*/
		uint32_t td_do                    : 13; /**<bit[19:31]*/
	};
	uint32_t v;
} aon_pmu_r7f_t;

typedef volatile struct {
	volatile aon_pmu_r0_t r0;
	volatile uint32_t rsv_1_1[1];
	volatile aon_pmu_r2_t r2;
	volatile uint32_t rsv_3_24[34];
	volatile aon_pmu_r25_t r25;
	volatile uint32_t rsv_26_3f[26];
	volatile aon_pmu_r40_t r40;
	volatile aon_pmu_r41_t r41;
	volatile uint32_t rsv_42_70[47];
	volatile aon_pmu_r71_t r71;
	volatile aon_pmu_r72_t r72;
	volatile aon_pmu_r73_t r73;
	volatile uint32_t rsv_74_76[3];
	volatile aon_pmu_r77_t r77;
	volatile aon_pmu_r78_t r78;
	volatile aon_pmu_r79_t r79;
	volatile aon_pmu_r7a_t r7a;
	volatile aon_pmu_r7b_t r7b;
	volatile aon_pmu_r7c_t r7c;
	volatile aon_pmu_r7d_t r7d;
	volatile aon_pmu_r7e_t r7e;
	volatile aon_pmu_r7f_t r7f;
} aon_pmu_hw_t;

#ifdef __cplusplus
}
#endif
