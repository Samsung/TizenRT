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
		uint32_t deviceid                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg0_t;


typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg1_t;


typedef volatile union {
	struct {
		uint32_t soft_reset               :  1; /**<bit[0 : 0] */
		uint32_t bps_clkgate              :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} pwm_reg2_t;


typedef volatile union {
	struct {
		uint32_t pwm_o                    : 12; /**<bit[0 : 11] */
		uint32_t reserved_12_31           : 20; /**<bit[12 : 31] */
	};
	uint32_t v;
} pwm_reg3_t;


typedef volatile union {
	struct {
		uint32_t cen1                     :  1; /**<bit[0 : 0] */
		uint32_t cen2                     :  1; /**<bit[1 : 1] */
		uint32_t cen3                     :  1; /**<bit[2 : 2] */
		uint32_t cen4                     :  1; /**<bit[3 : 3] */
		uint32_t cen5                     :  1; /**<bit[4 : 4] */
		uint32_t cen6                     :  1; /**<bit[5 : 5] */
		uint32_t cen7                     :  1; /**<bit[6 : 6] */
		uint32_t cen8                     :  1; /**<bit[7 : 7] */
		uint32_t cen9                     :  1; /**<bit[8 : 8] */
		uint32_t cen10                    :  1; /**<bit[9 : 9] */
		uint32_t cen11                    :  1; /**<bit[10 : 10] */
		uint32_t cen12                    :  1; /**<bit[11 : 11] */
		uint32_t sync_all                 :  1; /**<bit[12 : 12] */
		uint32_t sync_all_en              :  1; /**<bit[13 : 13] */
		uint32_t reserved_14_31           : 18; /**<bit[14 : 31] */
	};
	uint32_t v;
} pwm_reg4_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_0_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg10_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_1_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg11_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_2_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg12_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_3_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg13_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_4_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg14_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_5_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg15_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_6_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg16_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_7_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg17_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_8_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg18_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_9_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg19_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_10_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg1a_t;


typedef volatile union {
	struct {
		uint32_t pwm_o_11_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg1b_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_0_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg20_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_1_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg21_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_2_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg22_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_3_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg23_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_4_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg24_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_5_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg25_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_6_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg26_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_7_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg27_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_8_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg28_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_9_allocate         : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg29_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_10_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg2a_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_11_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg2b_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_12_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg2c_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_13_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg2d_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_14_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg2e_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_15_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg2f_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_16_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg30_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_17_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg31_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_18_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg32_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_19_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg33_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_20_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg34_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_21_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg35_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_22_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg36_t;


typedef volatile union {
	struct {
		uint32_t pwm_i_23_allocate        : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg37_t;


typedef volatile union {
	struct {
		uint32_t ocm3pe                   :  1; /**<bit[0 : 0] */
		uint32_t ocm2pe                   :  1; /**<bit[1 : 1] */
		uint32_t ocm1pe                   :  1; /**<bit[2 : 2] */
		uint32_t arpe3                    :  1; /**<bit[3 : 3] */
		uint32_t arpe2                    :  1; /**<bit[4 : 4] */
		uint32_t arpe1                    :  1; /**<bit[5 : 5] */
		uint32_t oc3pe                    :  1; /**<bit[6 : 6] */
		uint32_t oc2pe                    :  1; /**<bit[7 : 7] */
		uint32_t oc1pe                    :  1; /**<bit[8 : 8] */
		uint32_t reserved_9_17            :  9; /**<bit[9 : 17] */
		uint32_t cms_tim3                 :  2; /**<bit[18 : 19] */
		uint32_t cms_tim2                 :  2; /**<bit[20 : 21] */
		uint32_t cms_tim1                 :  2; /**<bit[22 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t urs3                     :  1; /**<bit[26 : 26] */
		uint32_t urs2                     :  1; /**<bit[27 : 27] */
		uint32_t urs1                     :  1; /**<bit[28 : 28] */
		uint32_t dir_tim3                 :  1; /**<bit[29 : 29] */
		uint32_t dir_tim2                 :  1; /**<bit[30 : 30] */
		uint32_t dir_tim1                 :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} pwm_reg40_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_7             :  8; /**<bit[0 : 7] */
		uint32_t com_mode                 :  1; /**<bit[8 : 8] */
		uint32_t ccpc                     :  1; /**<bit[9 : 9] */
		uint32_t trios3                   :  3; /**<bit[10 : 12] */
		uint32_t adcs3                    :  3; /**<bit[13 : 15] */
		uint32_t reserved_16_16           :  1; /**<bit[16 : 16] */
		uint32_t ccus                     :  1; /**<bit[17 : 17] */
		uint32_t trios2                   :  3; /**<bit[18 : 20] */
		uint32_t adcs2                    :  3; /**<bit[21 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t trios1                   :  3; /**<bit[26 : 28] */
		uint32_t adcs1                    :  3; /**<bit[29 : 31] */
	};
	uint32_t v;
} pwm_reg41_t;


typedef volatile union {
	struct {
		uint32_t sms1                     :  4; /**<bit[0 : 3] */
		uint32_t sms2                     :  4; /**<bit[4 : 7] */
		uint32_t sms3                     :  4; /**<bit[8 : 11] */
		uint32_t ts1                      :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_15           :  1; /**<bit[15 : 15] */
		uint32_t ts2                      :  3; /**<bit[16 : 18] */
		uint32_t ts3                      :  3; /**<bit[19 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_reg42_t;


typedef volatile union {
	struct {
		uint32_t cc1ie                    :  1; /**<bit[0 : 0] */
		uint32_t cc2ie                    :  1; /**<bit[1 : 1] */
		uint32_t cc3ie                    :  1; /**<bit[2 : 2] */
		uint32_t cc4ie                    :  1; /**<bit[3 : 3] */
		uint32_t cc5ie                    :  1; /**<bit[4 : 4] */
		uint32_t cc6ie                    :  1; /**<bit[5 : 5] */
		uint32_t cc7ie                    :  1; /**<bit[6 : 6] */
		uint32_t cc8ie                    :  1; /**<bit[7 : 7] */
		uint32_t cc9ie                    :  1; /**<bit[8 : 8] */
		uint32_t uie1                     :  1; /**<bit[9 : 9] */
		uint32_t uie2                     :  1; /**<bit[10 : 10] */
		uint32_t uie3                     :  1; /**<bit[11 : 11] */
		uint32_t tie1                     :  1; /**<bit[12 : 12] */
		uint32_t tie2                     :  1; /**<bit[13 : 13] */
		uint32_t tie3                     :  1; /**<bit[14 : 14] */
		uint32_t comie                    :  1; /**<bit[15 : 15] */
		uint32_t reserved_16_19           :  4; /**<bit[16 : 19] */
		uint32_t df_tim1                  :  4; /**<bit[20 : 23] */
		uint32_t df_tim2                  :  4; /**<bit[24 : 27] */
		uint32_t df_tim3                  :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} pwm_reg43_t;


typedef volatile union {
	struct {
		uint32_t cc1if                    :  1; /**<bit[0 : 0] */
		uint32_t cc2if                    :  1; /**<bit[1 : 1] */
		uint32_t cc3if                    :  1; /**<bit[2 : 2] */
		uint32_t cc4if                    :  1; /**<bit[3 : 3] */
		uint32_t cc5if                    :  1; /**<bit[4 : 4] */
		uint32_t cc6if                    :  1; /**<bit[5 : 5] */
		uint32_t cc7if                    :  1; /**<bit[6 : 6] */
		uint32_t cc8if                    :  1; /**<bit[7 : 7] */
		uint32_t cc9if                    :  1; /**<bit[8 : 8] */
		uint32_t uif1                     :  1; /**<bit[9 : 9] */
		uint32_t uif2                     :  1; /**<bit[10 : 10] */
		uint32_t uif3                     :  1; /**<bit[11 : 11] */
		uint32_t tif1                     :  1; /**<bit[12 : 12] */
		uint32_t tif2                     :  1; /**<bit[13 : 13] */
		uint32_t tif3                     :  1; /**<bit[14 : 14] */
		uint32_t comif                    :  1; /**<bit[15 : 15] */
		uint32_t ch1eif                   :  1; /**<bit[16 : 16] */
		uint32_t ch2eif                   :  1; /**<bit[17 : 17] */
		uint32_t ch3eif                   :  1; /**<bit[18 : 18] */
		uint32_t ch4eif                   :  1; /**<bit[19 : 19] */
		uint32_t ch5eif                   :  1; /**<bit[20 : 20] */
		uint32_t ch6eif                   :  1; /**<bit[21 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_reg44_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_8             :  9; /**<bit[0 : 8] */
		uint32_t ug1                      :  1; /**<bit[9 : 9] */
		uint32_t ug2                      :  1; /**<bit[10 : 10] */
		uint32_t ug3                      :  1; /**<bit[11 : 11] */
		uint32_t tg1                      :  1; /**<bit[12 : 12] */
		uint32_t tg2                      :  1; /**<bit[13 : 13] */
		uint32_t tg3                      :  1; /**<bit[14 : 14] */
		uint32_t comg                     :  1; /**<bit[15 : 15] */
		uint32_t dtm1                     :  2; /**<bit[16 : 17] */
		uint32_t dtm2                     :  2; /**<bit[18 : 19] */
		uint32_t dtm3                     :  2; /**<bit[20 : 21] */
		uint32_t init_level1              :  1; /**<bit[22 : 22] */
		uint32_t init_level2              :  1; /**<bit[23 : 23] */
		uint32_t init_level3              :  1; /**<bit[24 : 24] */
		uint32_t reserved_25_31           :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} pwm_reg45_t;


typedef volatile union {
	struct {
		uint32_t ch1p                     :  2; /**<bit[0 : 1] */
		uint32_t ch2p                     :  2; /**<bit[2 : 3] */
		uint32_t ch3p                     :  2; /**<bit[4 : 5] */
		uint32_t ch4p                     :  2; /**<bit[6 : 7] */
		uint32_t ch5p                     :  2; /**<bit[8 : 9] */
		uint32_t ch6p                     :  2; /**<bit[10 : 11] */
		uint32_t ch1e                     :  1; /**<bit[12 : 12] */
		uint32_t ch2e                     :  1; /**<bit[13 : 13] */
		uint32_t ch3e                     :  1; /**<bit[14 : 14] */
		uint32_t ch4e                     :  1; /**<bit[15 : 15] */
		uint32_t ch5e                     :  1; /**<bit[16 : 16] */
		uint32_t ch6e                     :  1; /**<bit[17 : 17] */
		uint32_t tim1ccm                  :  1; /**<bit[18 : 18] */
		uint32_t tim2ccm                  :  1; /**<bit[19 : 19] */
		uint32_t tim3ccm                  :  1; /**<bit[20 : 20] */
		uint32_t oc1m                     :  3; /**<bit[21 : 23] */
		uint32_t oc2m                     :  3; /**<bit[24 : 26] */
		uint32_t oc3m                     :  3; /**<bit[27 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_reg46_t;


typedef volatile union {
	struct {
		uint32_t tim1_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg47_t;


typedef volatile union {
	struct {
		uint32_t tim2_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg48_t;


typedef volatile union {
	struct {
		uint32_t tim3_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg49_t;


typedef volatile union {
	struct {
		uint32_t psc1                     :  8; /**<bit[0 : 7] */
		uint32_t psc2                     :  8; /**<bit[8 : 15] */
		uint32_t psc3                     :  8; /**<bit[16 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} pwm_reg4a_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg4b_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg4c_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg4d_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg4e_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg4f_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg50_t;


typedef volatile union {
	struct {
		uint32_t ccr1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg51_t;


typedef volatile union {
	struct {
		uint32_t ccr2                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg52_t;


typedef volatile union {
	struct {
		uint32_t ccr3                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg53_t;


typedef volatile union {
	struct {
		uint32_t ccr4                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg54_t;


typedef volatile union {
	struct {
		uint32_t ccr5                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg55_t;


typedef volatile union {
	struct {
		uint32_t ccr6                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg56_t;


typedef volatile union {
	struct {
		uint32_t ccr7                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg57_t;


typedef volatile union {
	struct {
		uint32_t ccr8                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg58_t;


typedef volatile union {
	struct {
		uint32_t ccr9                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg59_t;


typedef volatile union {
	struct {
		uint32_t dt1                      : 10; /**<bit[0 : 9] */
		uint32_t dt2                      : 10; /**<bit[10 : 19] */
		uint32_t dt3                      : 10; /**<bit[20 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_reg5a_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg5b_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg5c_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg5d_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg5e_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg5f_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg60_t;


typedef volatile union {
	struct {
		uint32_t ccr1_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg61_t;


typedef volatile union {
	struct {
		uint32_t ccr2_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg62_t;


typedef volatile union {
	struct {
		uint32_t ccr3_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg63_t;


typedef volatile union {
	struct {
		uint32_t ccr4_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg64_t;


typedef volatile union {
	struct {
		uint32_t ccr5_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg65_t;


typedef volatile union {
	struct {
		uint32_t ccr6_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg66_t;


typedef volatile union {
	struct {
		uint32_t ccr7_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg67_t;


typedef volatile union {
	struct {
		uint32_t ccr8_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg68_t;


typedef volatile union {
	struct {
		uint32_t ccr9_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg69_t;


typedef volatile union {
	struct {
		uint32_t ch6e_shad                :  1; /**<bit[0 : 0] */
		uint32_t ch5e_shad                :  1; /**<bit[1 : 1] */
		uint32_t ch4e_shad                :  1; /**<bit[2 : 2] */
		uint32_t ch3e_shad                :  1; /**<bit[3 : 3] */
		uint32_t ch2e_shad                :  1; /**<bit[4 : 4] */
		uint32_t ch1e_shad                :  1; /**<bit[5 : 5] */
		uint32_t oc1m_shad                :  3; /**<bit[6 : 8] */
		uint32_t oc2m_shad                :  3; /**<bit[9 : 11] */
		uint32_t oc3m_shad                :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_31           : 17; /**<bit[15 : 31] */
	};
	uint32_t v;
} pwm_reg6a_t;


typedef volatile union {
	struct {
		uint32_t tim1_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim1_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim1_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim1_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim1_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim1_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim1_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reg6b_t;


typedef volatile union {
	struct {
		uint32_t tim2_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim2_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim2_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim2_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim2_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim2_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim2_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reg6c_t;


typedef volatile union {
	struct {
		uint32_t tim3_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim3_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim3_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim3_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim3_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim3_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim3_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reg6d_t;


typedef volatile union {
	struct {
		uint32_t ocm3pe                   :  1; /**<bit[0 : 0] */
		uint32_t ocm2pe                   :  1; /**<bit[1 : 1] */
		uint32_t ocm1pe                   :  1; /**<bit[2 : 2] */
		uint32_t arpe3                    :  1; /**<bit[3 : 3] */
		uint32_t arpe2                    :  1; /**<bit[4 : 4] */
		uint32_t arpe1                    :  1; /**<bit[5 : 5] */
		uint32_t oc3pe                    :  1; /**<bit[6 : 6] */
		uint32_t oc2pe                    :  1; /**<bit[7 : 7] */
		uint32_t oc1pe                    :  1; /**<bit[8 : 8] */
		uint32_t reserved_9_17            :  9; /**<bit[9 : 17] */
		uint32_t cms_tim3                 :  2; /**<bit[18 : 19] */
		uint32_t cms_tim2                 :  2; /**<bit[20 : 21] */
		uint32_t cms_tim1                 :  2; /**<bit[22 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t urs3                     :  1; /**<bit[26 : 26] */
		uint32_t urs2                     :  1; /**<bit[27 : 27] */
		uint32_t urs1                     :  1; /**<bit[28 : 28] */
		uint32_t dir_tim3                 :  1; /**<bit[29 : 29] */
		uint32_t dir_tim2                 :  1; /**<bit[30 : 30] */
		uint32_t dir_tim1                 :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} pwm_reg80_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_7             :  8; /**<bit[0 : 7] */
		uint32_t com_mode                 :  1; /**<bit[8 : 8] */
		uint32_t ccpc                     :  1; /**<bit[9 : 9] */
		uint32_t trios3                   :  3; /**<bit[10 : 12] */
		uint32_t adcs3                    :  3; /**<bit[13 : 15] */
		uint32_t reserved_16_16           :  1; /**<bit[16 : 16] */
		uint32_t ccus                     :  1; /**<bit[17 : 17] */
		uint32_t trios2                   :  3; /**<bit[18 : 20] */
		uint32_t adcs2                    :  3; /**<bit[21 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t trios1                   :  3; /**<bit[26 : 28] */
		uint32_t adcs1                    :  3; /**<bit[29 : 31] */
	};
	uint32_t v;
} pwm_reg81_t;


typedef volatile union {
	struct {
		uint32_t sms1                     :  4; /**<bit[0 : 3] */
		uint32_t sms2                     :  4; /**<bit[4 : 7] */
		uint32_t sms3                     :  4; /**<bit[8 : 11] */
		uint32_t ts1                      :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_15           :  1; /**<bit[15 : 15] */
		uint32_t ts2                      :  3; /**<bit[16 : 18] */
		uint32_t ts3                      :  3; /**<bit[19 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_reg82_t;


typedef volatile union {
	struct {
		uint32_t cc1ie                    :  1; /**<bit[0 : 0] */
		uint32_t cc2ie                    :  1; /**<bit[1 : 1] */
		uint32_t cc3ie                    :  1; /**<bit[2 : 2] */
		uint32_t cc4ie                    :  1; /**<bit[3 : 3] */
		uint32_t cc5ie                    :  1; /**<bit[4 : 4] */
		uint32_t cc6ie                    :  1; /**<bit[5 : 5] */
		uint32_t cc7ie                    :  1; /**<bit[6 : 6] */
		uint32_t cc8ie                    :  1; /**<bit[7 : 7] */
		uint32_t cc9ie                    :  1; /**<bit[8 : 8] */
		uint32_t uie1                     :  1; /**<bit[9 : 9] */
		uint32_t uie2                     :  1; /**<bit[10 : 10] */
		uint32_t uie3                     :  1; /**<bit[11 : 11] */
		uint32_t tie1                     :  1; /**<bit[12 : 12] */
		uint32_t tie2                     :  1; /**<bit[13 : 13] */
		uint32_t tie3                     :  1; /**<bit[14 : 14] */
		uint32_t comie                    :  1; /**<bit[15 : 15] */
		uint32_t reserved_16_19           :  4; /**<bit[16 : 19] */
		uint32_t df_tim1                  :  4; /**<bit[20 : 23] */
		uint32_t df_tim2                  :  4; /**<bit[24 : 27] */
		uint32_t df_tim3                  :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} pwm_reg83_t;


typedef volatile union {
	struct {
		uint32_t cc1if                    :  1; /**<bit[0 : 0] */
		uint32_t cc2if                    :  1; /**<bit[1 : 1] */
		uint32_t cc3if                    :  1; /**<bit[2 : 2] */
		uint32_t cc4if                    :  1; /**<bit[3 : 3] */
		uint32_t cc5if                    :  1; /**<bit[4 : 4] */
		uint32_t cc6if                    :  1; /**<bit[5 : 5] */
		uint32_t cc7if                    :  1; /**<bit[6 : 6] */
		uint32_t cc8if                    :  1; /**<bit[7 : 7] */
		uint32_t cc9if                    :  1; /**<bit[8 : 8] */
		uint32_t uif1                     :  1; /**<bit[9 : 9] */
		uint32_t uif2                     :  1; /**<bit[10 : 10] */
		uint32_t uif3                     :  1; /**<bit[11 : 11] */
		uint32_t tif1                     :  1; /**<bit[12 : 12] */
		uint32_t tif2                     :  1; /**<bit[13 : 13] */
		uint32_t tif3                     :  1; /**<bit[14 : 14] */
		uint32_t comif                    :  1; /**<bit[15 : 15] */
		uint32_t ch1eif                   :  1; /**<bit[16 : 16] */
		uint32_t ch2eif                   :  1; /**<bit[17 : 17] */
		uint32_t ch3eif                   :  1; /**<bit[18 : 18] */
		uint32_t ch4eif                   :  1; /**<bit[19 : 19] */
		uint32_t ch5eif                   :  1; /**<bit[20 : 20] */
		uint32_t ch6eif                   :  1; /**<bit[21 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_reg84_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_8             :  9; /**<bit[0 : 8] */
		uint32_t ug1                      :  1; /**<bit[9 : 9] */
		uint32_t ug2                      :  1; /**<bit[10 : 10] */
		uint32_t ug3                      :  1; /**<bit[11 : 11] */
		uint32_t tg1                      :  1; /**<bit[12 : 12] */
		uint32_t tg2                      :  1; /**<bit[13 : 13] */
		uint32_t tg3                      :  1; /**<bit[14 : 14] */
		uint32_t comg                     :  1; /**<bit[15 : 15] */
		uint32_t dtm1                     :  2; /**<bit[16 : 17] */
		uint32_t dtm2                     :  2; /**<bit[18 : 19] */
		uint32_t dtm3                     :  2; /**<bit[20 : 21] */
		uint32_t init_level1              :  1; /**<bit[22 : 22] */
		uint32_t init_level2              :  1; /**<bit[23 : 23] */
		uint32_t init_level3              :  1; /**<bit[24 : 24] */
		uint32_t reserved_25_31           :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} pwm_reg85_t;


typedef volatile union {
	struct {
		uint32_t ch1p                     :  2; /**<bit[0 : 1] */
		uint32_t ch2p                     :  2; /**<bit[2 : 3] */
		uint32_t ch3p                     :  2; /**<bit[4 : 5] */
		uint32_t ch4p                     :  2; /**<bit[6 : 7] */
		uint32_t ch5p                     :  2; /**<bit[8 : 9] */
		uint32_t ch6p                     :  2; /**<bit[10 : 11] */
		uint32_t ch1e                     :  1; /**<bit[12 : 12] */
		uint32_t ch2e                     :  1; /**<bit[13 : 13] */
		uint32_t ch3e                     :  1; /**<bit[14 : 14] */
		uint32_t ch4e                     :  1; /**<bit[15 : 15] */
		uint32_t ch5e                     :  1; /**<bit[16 : 16] */
		uint32_t ch6e                     :  1; /**<bit[17 : 17] */
		uint32_t tim1ccm                  :  1; /**<bit[18 : 18] */
		uint32_t tim2ccm                  :  1; /**<bit[19 : 19] */
		uint32_t tim3ccm                  :  1; /**<bit[20 : 20] */
		uint32_t oc1m                     :  3; /**<bit[21 : 23] */
		uint32_t oc2m                     :  3; /**<bit[24 : 26] */
		uint32_t oc3m                     :  3; /**<bit[27 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_reg86_t;


typedef volatile union {
	struct {
		uint32_t tim1_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg87_t;


typedef volatile union {
	struct {
		uint32_t tim2_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg88_t;


typedef volatile union {
	struct {
		uint32_t tim3_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg89_t;


typedef volatile union {
	struct {
		uint32_t psc1                     :  8; /**<bit[0 : 7] */
		uint32_t psc2                     :  8; /**<bit[8 : 15] */
		uint32_t psc3                     :  8; /**<bit[16 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} pwm_reg8a_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg8b_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg8c_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg8d_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg8e_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg8f_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg90_t;


typedef volatile union {
	struct {
		uint32_t ccr1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg91_t;


typedef volatile union {
	struct {
		uint32_t ccr2                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg92_t;


typedef volatile union {
	struct {
		uint32_t ccr3                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg93_t;


typedef volatile union {
	struct {
		uint32_t ccr4                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg94_t;


typedef volatile union {
	struct {
		uint32_t ccr5                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg95_t;


typedef volatile union {
	struct {
		uint32_t ccr6                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg96_t;


typedef volatile union {
	struct {
		uint32_t ccr7                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg97_t;


typedef volatile union {
	struct {
		uint32_t ccr8                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg98_t;


typedef volatile union {
	struct {
		uint32_t ccr9                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg99_t;


typedef volatile union {
	struct {
		uint32_t dt1                      : 10; /**<bit[0 : 9] */
		uint32_t dt2                      : 10; /**<bit[10 : 19] */
		uint32_t dt3                      : 10; /**<bit[20 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_reg9a_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg9b_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg9c_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg9d_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg9e_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg9f_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega0_t;


typedef volatile union {
	struct {
		uint32_t ccr1_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega1_t;


typedef volatile union {
	struct {
		uint32_t ccr2_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega2_t;


typedef volatile union {
	struct {
		uint32_t ccr3_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega3_t;


typedef volatile union {
	struct {
		uint32_t ccr4_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega4_t;


typedef volatile union {
	struct {
		uint32_t ccr5_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega5_t;


typedef volatile union {
	struct {
		uint32_t ccr6_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega6_t;


typedef volatile union {
	struct {
		uint32_t ccr7_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega7_t;


typedef volatile union {
	struct {
		uint32_t ccr8_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega8_t;


typedef volatile union {
	struct {
		uint32_t ccr9_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rega9_t;


typedef volatile union {
	struct {
		uint32_t ch6e_shad                :  1; /**<bit[0 : 0] */
		uint32_t ch5e_shad                :  1; /**<bit[1 : 1] */
		uint32_t ch4e_shad                :  1; /**<bit[2 : 2] */
		uint32_t ch3e_shad                :  1; /**<bit[3 : 3] */
		uint32_t ch2e_shad                :  1; /**<bit[4 : 4] */
		uint32_t ch1e_shad                :  1; /**<bit[5 : 5] */
		uint32_t oc1m_shad                :  3; /**<bit[6 : 8] */
		uint32_t oc2m_shad                :  3; /**<bit[9 : 11] */
		uint32_t oc3m_shad                :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_31           : 17; /**<bit[15 : 31] */
	};
	uint32_t v;
} pwm_regaa_t;


typedef volatile union {
	struct {
		uint32_t tim1_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim1_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim1_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim1_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim1_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim1_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim1_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_regab_t;


typedef volatile union {
	struct {
		uint32_t tim2_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim2_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim2_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim2_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim2_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim2_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim2_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_regac_t;


typedef volatile union {
	struct {
		uint32_t tim3_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim3_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim3_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim3_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim3_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim3_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim3_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_regad_t;


typedef volatile union {
	struct {
		uint32_t ocm3pe                   :  1; /**<bit[0 : 0] */
		uint32_t ocm2pe                   :  1; /**<bit[1 : 1] */
		uint32_t ocm1pe                   :  1; /**<bit[2 : 2] */
		uint32_t arpe3                    :  1; /**<bit[3 : 3] */
		uint32_t arpe2                    :  1; /**<bit[4 : 4] */
		uint32_t arpe1                    :  1; /**<bit[5 : 5] */
		uint32_t oc3pe                    :  1; /**<bit[6 : 6] */
		uint32_t oc2pe                    :  1; /**<bit[7 : 7] */
		uint32_t oc1pe                    :  1; /**<bit[8 : 8] */
		uint32_t reserved_9_17            :  9; /**<bit[9 : 17] */
		uint32_t cms_tim3                 :  2; /**<bit[18 : 19] */
		uint32_t cms_tim2                 :  2; /**<bit[20 : 21] */
		uint32_t cms_tim1                 :  2; /**<bit[22 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t urs3                     :  1; /**<bit[26 : 26] */
		uint32_t urs2                     :  1; /**<bit[27 : 27] */
		uint32_t urs1                     :  1; /**<bit[28 : 28] */
		uint32_t dir_tim3                 :  1; /**<bit[29 : 29] */
		uint32_t dir_tim2                 :  1; /**<bit[30 : 30] */
		uint32_t dir_tim1                 :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} pwm_regc0_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_7             :  8; /**<bit[0 : 7] */
		uint32_t com_mode                 :  1; /**<bit[8 : 8] */
		uint32_t ccpc                     :  1; /**<bit[9 : 9] */
		uint32_t trios3                   :  3; /**<bit[10 : 12] */
		uint32_t adcs3                    :  3; /**<bit[13 : 15] */
		uint32_t reserved_16_16           :  1; /**<bit[16 : 16] */
		uint32_t ccus                     :  1; /**<bit[17 : 17] */
		uint32_t trios2                   :  3; /**<bit[18 : 20] */
		uint32_t adcs2                    :  3; /**<bit[21 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t trios1                   :  3; /**<bit[26 : 28] */
		uint32_t adcs1                    :  3; /**<bit[29 : 31] */
	};
	uint32_t v;
} pwm_regc1_t;


typedef volatile union {
	struct {
		uint32_t sms1                     :  4; /**<bit[0 : 3] */
		uint32_t sms2                     :  4; /**<bit[4 : 7] */
		uint32_t sms3                     :  4; /**<bit[8 : 11] */
		uint32_t ts1                      :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_15           :  1; /**<bit[15 : 15] */
		uint32_t ts2                      :  3; /**<bit[16 : 18] */
		uint32_t ts3                      :  3; /**<bit[19 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_regc2_t;


typedef volatile union {
	struct {
		uint32_t cc1ie                    :  1; /**<bit[0 : 0] */
		uint32_t cc2ie                    :  1; /**<bit[1 : 1] */
		uint32_t cc3ie                    :  1; /**<bit[2 : 2] */
		uint32_t cc4ie                    :  1; /**<bit[3 : 3] */
		uint32_t cc5ie                    :  1; /**<bit[4 : 4] */
		uint32_t cc6ie                    :  1; /**<bit[5 : 5] */
		uint32_t cc7ie                    :  1; /**<bit[6 : 6] */
		uint32_t cc8ie                    :  1; /**<bit[7 : 7] */
		uint32_t cc9ie                    :  1; /**<bit[8 : 8] */
		uint32_t uie1                     :  1; /**<bit[9 : 9] */
		uint32_t uie2                     :  1; /**<bit[10 : 10] */
		uint32_t uie3                     :  1; /**<bit[11 : 11] */
		uint32_t tie1                     :  1; /**<bit[12 : 12] */
		uint32_t tie2                     :  1; /**<bit[13 : 13] */
		uint32_t tie3                     :  1; /**<bit[14 : 14] */
		uint32_t comie                    :  1; /**<bit[15 : 15] */
		uint32_t reserved_16_19           :  4; /**<bit[16 : 19] */
		uint32_t df_tim1                  :  4; /**<bit[20 : 23] */
		uint32_t df_tim2                  :  4; /**<bit[24 : 27] */
		uint32_t df_tim3                  :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} pwm_regc3_t;


typedef volatile union {
	struct {
		uint32_t cc1if                    :  1; /**<bit[0 : 0] */
		uint32_t cc2if                    :  1; /**<bit[1 : 1] */
		uint32_t cc3if                    :  1; /**<bit[2 : 2] */
		uint32_t cc4if                    :  1; /**<bit[3 : 3] */
		uint32_t cc5if                    :  1; /**<bit[4 : 4] */
		uint32_t cc6if                    :  1; /**<bit[5 : 5] */
		uint32_t cc7if                    :  1; /**<bit[6 : 6] */
		uint32_t cc8if                    :  1; /**<bit[7 : 7] */
		uint32_t cc9if                    :  1; /**<bit[8 : 8] */
		uint32_t uif1                     :  1; /**<bit[9 : 9] */
		uint32_t uif2                     :  1; /**<bit[10 : 10] */
		uint32_t uif3                     :  1; /**<bit[11 : 11] */
		uint32_t tif1                     :  1; /**<bit[12 : 12] */
		uint32_t tif2                     :  1; /**<bit[13 : 13] */
		uint32_t tif3                     :  1; /**<bit[14 : 14] */
		uint32_t comif                    :  1; /**<bit[15 : 15] */
		uint32_t ch1eif                   :  1; /**<bit[16 : 16] */
		uint32_t ch2eif                   :  1; /**<bit[17 : 17] */
		uint32_t ch3eif                   :  1; /**<bit[18 : 18] */
		uint32_t ch4eif                   :  1; /**<bit[19 : 19] */
		uint32_t ch5eif                   :  1; /**<bit[20 : 20] */
		uint32_t ch6eif                   :  1; /**<bit[21 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_regc4_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_8             :  9; /**<bit[0 : 8] */
		uint32_t ug1                      :  1; /**<bit[9 : 9] */
		uint32_t ug2                      :  1; /**<bit[10 : 10] */
		uint32_t ug3                      :  1; /**<bit[11 : 11] */
		uint32_t tg1                      :  1; /**<bit[12 : 12] */
		uint32_t tg2                      :  1; /**<bit[13 : 13] */
		uint32_t tg3                      :  1; /**<bit[14 : 14] */
		uint32_t comg                     :  1; /**<bit[15 : 15] */
		uint32_t dtm1                     :  2; /**<bit[16 : 17] */
		uint32_t dtm2                     :  2; /**<bit[18 : 19] */
		uint32_t dtm3                     :  2; /**<bit[20 : 21] */
		uint32_t init_level1              :  1; /**<bit[22 : 22] */
		uint32_t init_level2              :  1; /**<bit[23 : 23] */
		uint32_t init_level3              :  1; /**<bit[24 : 24] */
		uint32_t reserved_25_31           :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} pwm_regc5_t;


typedef volatile union {
	struct {
		uint32_t ch1p                     :  2; /**<bit[0 : 1] */
		uint32_t ch2p                     :  2; /**<bit[2 : 3] */
		uint32_t ch3p                     :  2; /**<bit[4 : 5] */
		uint32_t ch4p                     :  2; /**<bit[6 : 7] */
		uint32_t ch5p                     :  2; /**<bit[8 : 9] */
		uint32_t ch6p                     :  2; /**<bit[10 : 11] */
		uint32_t ch1e                     :  1; /**<bit[12 : 12] */
		uint32_t ch2e                     :  1; /**<bit[13 : 13] */
		uint32_t ch3e                     :  1; /**<bit[14 : 14] */
		uint32_t ch4e                     :  1; /**<bit[15 : 15] */
		uint32_t ch5e                     :  1; /**<bit[16 : 16] */
		uint32_t ch6e                     :  1; /**<bit[17 : 17] */
		uint32_t tim1ccm                  :  1; /**<bit[18 : 18] */
		uint32_t tim2ccm                  :  1; /**<bit[19 : 19] */
		uint32_t tim3ccm                  :  1; /**<bit[20 : 20] */
		uint32_t oc1m                     :  3; /**<bit[21 : 23] */
		uint32_t oc2m                     :  3; /**<bit[24 : 26] */
		uint32_t oc3m                     :  3; /**<bit[27 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_regc6_t;


typedef volatile union {
	struct {
		uint32_t tim1_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regc7_t;


typedef volatile union {
	struct {
		uint32_t tim2_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regc8_t;


typedef volatile union {
	struct {
		uint32_t tim3_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regc9_t;


typedef volatile union {
	struct {
		uint32_t psc1                     :  8; /**<bit[0 : 7] */
		uint32_t psc2                     :  8; /**<bit[8 : 15] */
		uint32_t psc3                     :  8; /**<bit[16 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} pwm_regca_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regcb_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regcc_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regcd_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regce_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regcf_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd0_t;


typedef volatile union {
	struct {
		uint32_t ccr1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd1_t;


typedef volatile union {
	struct {
		uint32_t ccr2                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd2_t;


typedef volatile union {
	struct {
		uint32_t ccr3                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd3_t;


typedef volatile union {
	struct {
		uint32_t ccr4                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd4_t;


typedef volatile union {
	struct {
		uint32_t ccr5                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd5_t;


typedef volatile union {
	struct {
		uint32_t ccr6                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd6_t;


typedef volatile union {
	struct {
		uint32_t ccr7                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd7_t;


typedef volatile union {
	struct {
		uint32_t ccr8                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd8_t;


typedef volatile union {
	struct {
		uint32_t ccr9                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regd9_t;


typedef volatile union {
	struct {
		uint32_t dt1                      : 10; /**<bit[0 : 9] */
		uint32_t dt2                      : 10; /**<bit[10 : 19] */
		uint32_t dt3                      : 10; /**<bit[20 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_regda_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regdb_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regdc_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regdd_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regde_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_regdf_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege0_t;


typedef volatile union {
	struct {
		uint32_t ccr1_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege1_t;


typedef volatile union {
	struct {
		uint32_t ccr2_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege2_t;


typedef volatile union {
	struct {
		uint32_t ccr3_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege3_t;


typedef volatile union {
	struct {
		uint32_t ccr4_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege4_t;


typedef volatile union {
	struct {
		uint32_t ccr5_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege5_t;


typedef volatile union {
	struct {
		uint32_t ccr6_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege6_t;


typedef volatile union {
	struct {
		uint32_t ccr7_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege7_t;


typedef volatile union {
	struct {
		uint32_t ccr8_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege8_t;


typedef volatile union {
	struct {
		uint32_t ccr9_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_rege9_t;


typedef volatile union {
	struct {
		uint32_t ch6e_shad                :  1; /**<bit[0 : 0] */
		uint32_t ch5e_shad                :  1; /**<bit[1 : 1] */
		uint32_t ch4e_shad                :  1; /**<bit[2 : 2] */
		uint32_t ch3e_shad                :  1; /**<bit[3 : 3] */
		uint32_t ch2e_shad                :  1; /**<bit[4 : 4] */
		uint32_t ch1e_shad                :  1; /**<bit[5 : 5] */
		uint32_t oc1m_shad                :  3; /**<bit[6 : 8] */
		uint32_t oc2m_shad                :  3; /**<bit[9 : 11] */
		uint32_t oc3m_shad                :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_31           : 17; /**<bit[15 : 31] */
	};
	uint32_t v;
} pwm_regea_t;


typedef volatile union {
	struct {
		uint32_t tim1_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim1_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim1_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim1_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim1_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim1_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim1_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_regeb_t;


typedef volatile union {
	struct {
		uint32_t tim2_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim2_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim2_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim2_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim2_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim2_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim2_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_regec_t;


typedef volatile union {
	struct {
		uint32_t tim3_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim3_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim3_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim3_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim3_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim3_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim3_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reged_t;


typedef volatile union {
	struct {
		uint32_t ocm3pe                   :  1; /**<bit[0 : 0] */
		uint32_t ocm2pe                   :  1; /**<bit[1 : 1] */
		uint32_t ocm1pe                   :  1; /**<bit[2 : 2] */
		uint32_t arpe3                    :  1; /**<bit[3 : 3] */
		uint32_t arpe2                    :  1; /**<bit[4 : 4] */
		uint32_t arpe1                    :  1; /**<bit[5 : 5] */
		uint32_t oc3pe                    :  1; /**<bit[6 : 6] */
		uint32_t oc2pe                    :  1; /**<bit[7 : 7] */
		uint32_t oc1pe                    :  1; /**<bit[8 : 8] */
		uint32_t reserved_9_17            :  9; /**<bit[9 : 17] */
		uint32_t cms_tim3                 :  2; /**<bit[18 : 19] */
		uint32_t cms_tim2                 :  2; /**<bit[20 : 21] */
		uint32_t cms_tim1                 :  2; /**<bit[22 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t urs3                     :  1; /**<bit[26 : 26] */
		uint32_t urs2                     :  1; /**<bit[27 : 27] */
		uint32_t urs1                     :  1; /**<bit[28 : 28] */
		uint32_t dir_tim3                 :  1; /**<bit[29 : 29] */
		uint32_t dir_tim2                 :  1; /**<bit[30 : 30] */
		uint32_t dir_tim1                 :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} pwm_reg100_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_7             :  8; /**<bit[0 : 7] */
		uint32_t com_mode                 :  1; /**<bit[8 : 8] */
		uint32_t ccpc                     :  1; /**<bit[9 : 9] */
		uint32_t trios3                   :  3; /**<bit[10 : 12] */
		uint32_t adcs3                    :  3; /**<bit[13 : 15] */
		uint32_t reserved_16_16           :  1; /**<bit[16 : 16] */
		uint32_t ccus                     :  1; /**<bit[17 : 17] */
		uint32_t trios2                   :  3; /**<bit[18 : 20] */
		uint32_t adcs2                    :  3; /**<bit[21 : 23] */
		uint32_t reserved_24_25           :  2; /**<bit[24 : 25] */
		uint32_t trios1                   :  3; /**<bit[26 : 28] */
		uint32_t adcs1                    :  3; /**<bit[29 : 31] */
	};
	uint32_t v;
} pwm_reg101_t;


typedef volatile union {
	struct {
		uint32_t sms1                     :  4; /**<bit[0 : 3] */
		uint32_t sms2                     :  4; /**<bit[4 : 7] */
		uint32_t sms3                     :  4; /**<bit[8 : 11] */
		uint32_t ts1                      :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_15           :  1; /**<bit[15 : 15] */
		uint32_t ts2                      :  3; /**<bit[16 : 18] */
		uint32_t ts3                      :  3; /**<bit[19 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_reg102_t;


typedef volatile union {
	struct {
		uint32_t cc1ie                    :  1; /**<bit[0 : 0] */
		uint32_t cc2ie                    :  1; /**<bit[1 : 1] */
		uint32_t cc3ie                    :  1; /**<bit[2 : 2] */
		uint32_t cc4ie                    :  1; /**<bit[3 : 3] */
		uint32_t cc5ie                    :  1; /**<bit[4 : 4] */
		uint32_t cc6ie                    :  1; /**<bit[5 : 5] */
		uint32_t cc7ie                    :  1; /**<bit[6 : 6] */
		uint32_t cc8ie                    :  1; /**<bit[7 : 7] */
		uint32_t cc9ie                    :  1; /**<bit[8 : 8] */
		uint32_t uie1                     :  1; /**<bit[9 : 9] */
		uint32_t uie2                     :  1; /**<bit[10 : 10] */
		uint32_t uie3                     :  1; /**<bit[11 : 11] */
		uint32_t tie1                     :  1; /**<bit[12 : 12] */
		uint32_t tie2                     :  1; /**<bit[13 : 13] */
		uint32_t tie3                     :  1; /**<bit[14 : 14] */
		uint32_t comie                    :  1; /**<bit[15 : 15] */
		uint32_t reserved_16_19           :  4; /**<bit[16 : 19] */
		uint32_t df_tim1                  :  4; /**<bit[20 : 23] */
		uint32_t df_tim2                  :  4; /**<bit[24 : 27] */
		uint32_t df_tim3                  :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} pwm_reg103_t;


typedef volatile union {
	struct {
		uint32_t cc1if                    :  1; /**<bit[0 : 0] */
		uint32_t cc2if                    :  1; /**<bit[1 : 1] */
		uint32_t cc3if                    :  1; /**<bit[2 : 2] */
		uint32_t cc4if                    :  1; /**<bit[3 : 3] */
		uint32_t cc5if                    :  1; /**<bit[4 : 4] */
		uint32_t cc6if                    :  1; /**<bit[5 : 5] */
		uint32_t cc7if                    :  1; /**<bit[6 : 6] */
		uint32_t cc8if                    :  1; /**<bit[7 : 7] */
		uint32_t cc9if                    :  1; /**<bit[8 : 8] */
		uint32_t uif1                     :  1; /**<bit[9 : 9] */
		uint32_t uif2                     :  1; /**<bit[10 : 10] */
		uint32_t uif3                     :  1; /**<bit[11 : 11] */
		uint32_t tif1                     :  1; /**<bit[12 : 12] */
		uint32_t tif2                     :  1; /**<bit[13 : 13] */
		uint32_t tif3                     :  1; /**<bit[14 : 14] */
		uint32_t comif                    :  1; /**<bit[15 : 15] */
		uint32_t ch1eif                   :  1; /**<bit[16 : 16] */
		uint32_t ch2eif                   :  1; /**<bit[17 : 17] */
		uint32_t ch3eif                   :  1; /**<bit[18 : 18] */
		uint32_t ch4eif                   :  1; /**<bit[19 : 19] */
		uint32_t ch5eif                   :  1; /**<bit[20 : 20] */
		uint32_t ch6eif                   :  1; /**<bit[21 : 21] */
		uint32_t reserved_22_31           : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} pwm_reg104_t;


typedef volatile union {
	struct {
		uint32_t reserved_0_8             :  9; /**<bit[0 : 8] */
		uint32_t ug1                      :  1; /**<bit[9 : 9] */
		uint32_t ug2                      :  1; /**<bit[10 : 10] */
		uint32_t ug3                      :  1; /**<bit[11 : 11] */
		uint32_t tg1                      :  1; /**<bit[12 : 12] */
		uint32_t tg2                      :  1; /**<bit[13 : 13] */
		uint32_t tg3                      :  1; /**<bit[14 : 14] */
		uint32_t comg                     :  1; /**<bit[15 : 15] */
		uint32_t dtm1                     :  2; /**<bit[16 : 17] */
		uint32_t dtm2                     :  2; /**<bit[18 : 19] */
		uint32_t dtm3                     :  2; /**<bit[20 : 21] */
		uint32_t init_level1              :  1; /**<bit[22 : 22] */
		uint32_t init_level2              :  1; /**<bit[23 : 23] */
		uint32_t init_level3              :  1; /**<bit[24 : 24] */
		uint32_t reserved_25_31           :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} pwm_reg105_t;


typedef volatile union {
	struct {
		uint32_t ch1p                     :  2; /**<bit[0 : 1] */
		uint32_t ch2p                     :  2; /**<bit[2 : 3] */
		uint32_t ch3p                     :  2; /**<bit[4 : 5] */
		uint32_t ch4p                     :  2; /**<bit[6 : 7] */
		uint32_t ch5p                     :  2; /**<bit[8 : 9] */
		uint32_t ch6p                     :  2; /**<bit[10 : 11] */
		uint32_t ch1e                     :  1; /**<bit[12 : 12] */
		uint32_t ch2e                     :  1; /**<bit[13 : 13] */
		uint32_t ch3e                     :  1; /**<bit[14 : 14] */
		uint32_t ch4e                     :  1; /**<bit[15 : 15] */
		uint32_t ch5e                     :  1; /**<bit[16 : 16] */
		uint32_t ch6e                     :  1; /**<bit[17 : 17] */
		uint32_t tim1ccm                  :  1; /**<bit[18 : 18] */
		uint32_t tim2ccm                  :  1; /**<bit[19 : 19] */
		uint32_t tim3ccm                  :  1; /**<bit[20 : 20] */
		uint32_t oc1m                     :  3; /**<bit[21 : 23] */
		uint32_t oc2m                     :  3; /**<bit[24 : 26] */
		uint32_t oc3m                     :  3; /**<bit[27 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_reg106_t;


typedef volatile union {
	struct {
		uint32_t tim1_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg107_t;


typedef volatile union {
	struct {
		uint32_t tim2_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg108_t;


typedef volatile union {
	struct {
		uint32_t tim3_cnt                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg109_t;


typedef volatile union {
	struct {
		uint32_t psc1                     :  8; /**<bit[0 : 7] */
		uint32_t psc2                     :  8; /**<bit[8 : 15] */
		uint32_t psc3                     :  8; /**<bit[16 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} pwm_reg10a_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg10b_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg10c_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg10d_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg10e_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg10f_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg110_t;


typedef volatile union {
	struct {
		uint32_t ccr1                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg111_t;


typedef volatile union {
	struct {
		uint32_t ccr2                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg112_t;


typedef volatile union {
	struct {
		uint32_t ccr3                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg113_t;


typedef volatile union {
	struct {
		uint32_t ccr4                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg114_t;


typedef volatile union {
	struct {
		uint32_t ccr5                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg115_t;


typedef volatile union {
	struct {
		uint32_t ccr6                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg116_t;


typedef volatile union {
	struct {
		uint32_t ccr7                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg117_t;


typedef volatile union {
	struct {
		uint32_t ccr8                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg118_t;


typedef volatile union {
	struct {
		uint32_t ccr9                     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg119_t;


typedef volatile union {
	struct {
		uint32_t dt1                      : 10; /**<bit[0 : 9] */
		uint32_t dt2                      : 10; /**<bit[10 : 19] */
		uint32_t dt3                      : 10; /**<bit[20 : 29] */
		uint32_t reserved_30_31           :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} pwm_reg11a_t;


typedef volatile union {
	struct {
		uint32_t tim1_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg11b_t;


typedef volatile union {
	struct {
		uint32_t tim2_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg11c_t;


typedef volatile union {
	struct {
		uint32_t tim3_arr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg11d_t;


typedef volatile union {
	struct {
		uint32_t tim1_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg11e_t;


typedef volatile union {
	struct {
		uint32_t tim2_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg11f_t;


typedef volatile union {
	struct {
		uint32_t tim3_rcr_shad            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg120_t;


typedef volatile union {
	struct {
		uint32_t ccr1_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg121_t;


typedef volatile union {
	struct {
		uint32_t ccr2_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg122_t;


typedef volatile union {
	struct {
		uint32_t ccr3_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg123_t;


typedef volatile union {
	struct {
		uint32_t ccr4_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg124_t;


typedef volatile union {
	struct {
		uint32_t ccr5_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg125_t;


typedef volatile union {
	struct {
		uint32_t ccr6_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg126_t;


typedef volatile union {
	struct {
		uint32_t ccr7_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg127_t;


typedef volatile union {
	struct {
		uint32_t ccr8_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg128_t;


typedef volatile union {
	struct {
		uint32_t ccr9_shad                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} pwm_reg129_t;


typedef volatile union {
	struct {
		uint32_t ch6e_shad                :  1; /**<bit[0 : 0] */
		uint32_t ch5e_shad                :  1; /**<bit[1 : 1] */
		uint32_t ch4e_shad                :  1; /**<bit[2 : 2] */
		uint32_t ch3e_shad                :  1; /**<bit[3 : 3] */
		uint32_t ch2e_shad                :  1; /**<bit[4 : 4] */
		uint32_t ch1e_shad                :  1; /**<bit[5 : 5] */
		uint32_t oc1m_shad                :  3; /**<bit[6 : 8] */
		uint32_t oc2m_shad                :  3; /**<bit[9 : 11] */
		uint32_t oc3m_shad                :  3; /**<bit[12 : 14] */
		uint32_t reserved_15_31           : 17; /**<bit[15 : 31] */
	};
	uint32_t v;
} pwm_reg12a_t;


typedef volatile union {
	struct {
		uint32_t tim1_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim1_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim1_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim1_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim1_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim1_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim1_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reg12b_t;


typedef volatile union {
	struct {
		uint32_t tim2_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim2_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim2_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim2_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim2_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim2_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim2_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reg12c_t;


typedef volatile union {
	struct {
		uint32_t tim3_fade_num            : 10; /**<bit[0 : 9] */
		uint32_t tim3_fade_intval_cyc     : 10; /**<bit[10 : 19] */
		uint32_t tim3_fade_scale          :  8; /**<bit[20 : 27] */
		uint32_t tim3_fade_end_if         :  1; /**<bit[28] */
		uint32_t tim3_fade_end_int_en     :  1; /**<bit[29] */
		uint32_t tim3_fade_inc_dec        :  1; /**<bit[30] */
		uint32_t tim3_duty_fading_en      :  1; /**<bit[31] */
	};
	uint32_t v;
} pwm_reg12d_t;


typedef volatile struct {
	volatile pwm_reg0_t reg0;
	volatile pwm_reg1_t reg1;
	volatile pwm_reg2_t reg2;
	volatile pwm_reg3_t reg3;
	volatile pwm_reg4_t reg4;
	volatile uint32_t rsv_5_f[11];
	volatile pwm_reg10_t reg10;
	volatile pwm_reg11_t reg11;
	volatile pwm_reg12_t reg12;
	volatile pwm_reg13_t reg13;
	volatile pwm_reg14_t reg14;
	volatile pwm_reg15_t reg15;
	volatile pwm_reg16_t reg16;
	volatile pwm_reg17_t reg17;
	volatile pwm_reg18_t reg18;
	volatile pwm_reg19_t reg19;
	volatile pwm_reg1a_t reg1a;
	volatile pwm_reg1b_t reg1b;
	volatile uint32_t rsv_1c_1f[4];
	volatile pwm_reg20_t reg20;
	volatile pwm_reg21_t reg21;
	volatile pwm_reg22_t reg22;
	volatile pwm_reg23_t reg23;
	volatile pwm_reg24_t reg24;
	volatile pwm_reg25_t reg25;
	volatile pwm_reg26_t reg26;
	volatile pwm_reg27_t reg27;
	volatile pwm_reg28_t reg28;
	volatile pwm_reg29_t reg29;
	volatile pwm_reg2a_t reg2a;
	volatile pwm_reg2b_t reg2b;
	volatile pwm_reg2c_t reg2c;
	volatile pwm_reg2d_t reg2d;
	volatile pwm_reg2e_t reg2e;
	volatile pwm_reg2f_t reg2f;
	volatile pwm_reg30_t reg30;
	volatile pwm_reg31_t reg31;
	volatile pwm_reg32_t reg32;
	volatile pwm_reg33_t reg33;
	volatile pwm_reg34_t reg34;
	volatile pwm_reg35_t reg35;
	volatile pwm_reg36_t reg36;
	volatile pwm_reg37_t reg37;
	volatile uint32_t rsv_38_3f[8];
	volatile pwm_reg40_t reg40;
	volatile pwm_reg41_t reg41;
	volatile pwm_reg42_t reg42;
	volatile pwm_reg43_t reg43;
	volatile pwm_reg44_t reg44;
	volatile pwm_reg45_t reg45;
	volatile pwm_reg46_t reg46;
	volatile pwm_reg47_t reg47;
	volatile pwm_reg48_t reg48;
	volatile pwm_reg49_t reg49;
	volatile pwm_reg4a_t reg4a;
	volatile pwm_reg4b_t reg4b;
	volatile pwm_reg4c_t reg4c;
	volatile pwm_reg4d_t reg4d;
	volatile pwm_reg4e_t reg4e;
	volatile pwm_reg4f_t reg4f;
	volatile pwm_reg50_t reg50;
	volatile pwm_reg51_t reg51;
	volatile pwm_reg52_t reg52;
	volatile pwm_reg53_t reg53;
	volatile pwm_reg54_t reg54;
	volatile pwm_reg55_t reg55;
	volatile pwm_reg56_t reg56;
	volatile pwm_reg57_t reg57;
	volatile pwm_reg58_t reg58;
	volatile pwm_reg59_t reg59;
	volatile pwm_reg5a_t reg5a;
	volatile pwm_reg5b_t reg5b;
	volatile pwm_reg5c_t reg5c;
	volatile pwm_reg5d_t reg5d;
	volatile pwm_reg5e_t reg5e;
	volatile pwm_reg5f_t reg5f;
	volatile pwm_reg60_t reg60;
	volatile pwm_reg61_t reg61;
	volatile pwm_reg62_t reg62;
	volatile pwm_reg63_t reg63;
	volatile pwm_reg64_t reg64;
	volatile pwm_reg65_t reg65;
	volatile pwm_reg66_t reg66;
	volatile pwm_reg67_t reg67;
	volatile pwm_reg68_t reg68;
	volatile pwm_reg69_t reg69;
	volatile pwm_reg6a_t reg6a;
	volatile pwm_reg6b_t reg6b;
	volatile pwm_reg6c_t reg6c;
	volatile pwm_reg6d_t reg6d;
	volatile uint32_t rsv_6e_7f[18];
	volatile pwm_reg80_t reg80;
	volatile pwm_reg81_t reg81;
	volatile pwm_reg82_t reg82;
	volatile pwm_reg83_t reg83;
	volatile pwm_reg84_t reg84;
	volatile pwm_reg85_t reg85;
	volatile pwm_reg86_t reg86;
	volatile pwm_reg87_t reg87;
	volatile pwm_reg88_t reg88;
	volatile pwm_reg89_t reg89;
	volatile pwm_reg8a_t reg8a;
	volatile pwm_reg8b_t reg8b;
	volatile pwm_reg8c_t reg8c;
	volatile pwm_reg8d_t reg8d;
	volatile pwm_reg8e_t reg8e;
	volatile pwm_reg8f_t reg8f;
	volatile pwm_reg90_t reg90;
	volatile pwm_reg91_t reg91;
	volatile pwm_reg92_t reg92;
	volatile pwm_reg93_t reg93;
	volatile pwm_reg94_t reg94;
	volatile pwm_reg95_t reg95;
	volatile pwm_reg96_t reg96;
	volatile pwm_reg97_t reg97;
	volatile pwm_reg98_t reg98;
	volatile pwm_reg99_t reg99;
	volatile pwm_reg9a_t reg9a;
	volatile pwm_reg9b_t reg9b;
	volatile pwm_reg9c_t reg9c;
	volatile pwm_reg9d_t reg9d;
	volatile pwm_reg9e_t reg9e;
	volatile pwm_reg9f_t reg9f;
	volatile pwm_rega0_t rega0;
	volatile pwm_rega1_t rega1;
	volatile pwm_rega2_t rega2;
	volatile pwm_rega3_t rega3;
	volatile pwm_rega4_t rega4;
	volatile pwm_rega5_t rega5;
	volatile pwm_rega6_t rega6;
	volatile pwm_rega7_t rega7;
	volatile pwm_rega8_t rega8;
	volatile pwm_rega9_t rega9;
	volatile pwm_regaa_t regaa;
	volatile pwm_regab_t regab;
	volatile pwm_regac_t regac;
	volatile pwm_regad_t regad;
	volatile uint32_t rsv_ae_bf[18];
	volatile pwm_regc0_t regc0;
	volatile pwm_regc1_t regc1;
	volatile pwm_regc2_t regc2;
	volatile pwm_regc3_t regc3;
	volatile pwm_regc4_t regc4;
	volatile pwm_regc5_t regc5;
	volatile pwm_regc6_t regc6;
	volatile pwm_regc7_t regc7;
	volatile pwm_regc8_t regc8;
	volatile pwm_regc9_t regc9;
	volatile pwm_regca_t regca;
	volatile pwm_regcb_t regcb;
	volatile pwm_regcc_t regcc;
	volatile pwm_regcd_t regcd;
	volatile pwm_regce_t regce;
	volatile pwm_regcf_t regcf;
	volatile pwm_regd0_t regd0;
	volatile pwm_regd1_t regd1;
	volatile pwm_regd2_t regd2;
	volatile pwm_regd3_t regd3;
	volatile pwm_regd4_t regd4;
	volatile pwm_regd5_t regd5;
	volatile pwm_regd6_t regd6;
	volatile pwm_regd7_t regd7;
	volatile pwm_regd8_t regd8;
	volatile pwm_regd9_t regd9;
	volatile pwm_regda_t regda;
	volatile pwm_regdb_t regdb;
	volatile pwm_regdc_t regdc;
	volatile pwm_regdd_t regdd;
	volatile pwm_regde_t regde;
	volatile pwm_regdf_t regdf;
	volatile pwm_rege0_t rege0;
	volatile pwm_rege1_t rege1;
	volatile pwm_rege2_t rege2;
	volatile pwm_rege3_t rege3;
	volatile pwm_rege4_t rege4;
	volatile pwm_rege5_t rege5;
	volatile pwm_rege6_t rege6;
	volatile pwm_rege7_t rege7;
	volatile pwm_rege8_t rege8;
	volatile pwm_rege9_t rege9;
	volatile pwm_regea_t regea;
	volatile pwm_regeb_t regeb;
	volatile pwm_regec_t regec;
	volatile pwm_reged_t reged;
	volatile uint32_t rsv_ee_ff[18];
	volatile pwm_reg100_t reg100;
	volatile pwm_reg101_t reg101;
	volatile pwm_reg102_t reg102;
	volatile pwm_reg103_t reg103;
	volatile pwm_reg104_t reg104;
	volatile pwm_reg105_t reg105;
	volatile pwm_reg106_t reg106;
	volatile pwm_reg107_t reg107;
	volatile pwm_reg108_t reg108;
	volatile pwm_reg109_t reg109;
	volatile pwm_reg10a_t reg10a;
	volatile pwm_reg10b_t reg10b;
	volatile pwm_reg10c_t reg10c;
	volatile pwm_reg10d_t reg10d;
	volatile pwm_reg10e_t reg10e;
	volatile pwm_reg10f_t reg10f;
	volatile pwm_reg110_t reg110;
	volatile pwm_reg111_t reg111;
	volatile pwm_reg112_t reg112;
	volatile pwm_reg113_t reg113;
	volatile pwm_reg114_t reg114;
	volatile pwm_reg115_t reg115;
	volatile pwm_reg116_t reg116;
	volatile pwm_reg117_t reg117;
	volatile pwm_reg118_t reg118;
	volatile pwm_reg119_t reg119;
	volatile pwm_reg11a_t reg11a;
	volatile pwm_reg11b_t reg11b;
	volatile pwm_reg11c_t reg11c;
	volatile pwm_reg11d_t reg11d;
	volatile pwm_reg11e_t reg11e;
	volatile pwm_reg11f_t reg11f;
	volatile pwm_reg120_t reg120;
	volatile pwm_reg121_t reg121;
	volatile pwm_reg122_t reg122;
	volatile pwm_reg123_t reg123;
	volatile pwm_reg124_t reg124;
	volatile pwm_reg125_t reg125;
	volatile pwm_reg126_t reg126;
	volatile pwm_reg127_t reg127;
	volatile pwm_reg128_t reg128;
	volatile pwm_reg129_t reg129;
	volatile pwm_reg12a_t reg12a;
	volatile pwm_reg12b_t reg12b;
	volatile pwm_reg12c_t reg12c;
	volatile pwm_reg12d_t reg12d;
} pwm_hw_t;

#ifdef __cplusplus
}
#endif
