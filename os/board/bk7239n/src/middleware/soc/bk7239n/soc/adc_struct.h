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
} adc_reg0_t;


typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} adc_reg1_t;


typedef volatile union {
	struct {
		uint32_t soft_rst                 :  1; /**<bit[0 : 0] */
		uint32_t bypass_ckg               :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} adc_reg2_t;


typedef volatile union {
	struct {
		uint32_t gbstatus                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} adc_reg3_t;


typedef volatile union {
	struct {
		uint32_t enable                   :  1; /**<bit[0 : 0] */
		uint32_t calib_mode               :  1; /**<bit[1 : 1] */
		uint32_t calib_done               :  1; /**<bit[2 : 2] */
		uint32_t samp_sel                 :  1; /**<bit[3 : 3] */
		uint32_t adc_chan                 :  4; /**<bit[4 : 7] */
		uint32_t adc_mode                 :  1; /**<bit[8 : 8] */
		uint32_t adc_dump_num             :  3; /**<bit[9 : 11] */
		uint32_t adc_sclk_div             :  4; /**<bit[12 : 15] */
		uint32_t adc_sraw                 :  1; /**<bit[16 : 16] */
		uint32_t reserved_bit_17_31       : 15; /**<bit[17 : 31] */
	};
	uint32_t v;
} adc_reg4_t;


typedef volatile union {
	struct {
		uint32_t co_gain                  :  8; /**<bit[0 : 7] */
		uint32_t reserved_bit_8_31        : 24; /**<bit[8 : 31] */
	};
	uint32_t v;
} adc_reg5_t;


typedef volatile union {
	struct {
		uint32_t co_offset                : 16; /**<bit[0 : 15] */
		uint32_t reserved_bit_16_31       : 16; /**<bit[16 : 31] */
	};
	uint32_t v;
} adc_reg6_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal00                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg7_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal01                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg8_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal02                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg9_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal03                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_rega_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal04                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_regb_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal05                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_regc_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal06                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_regd_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal07                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_rege_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal08                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_regf_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal09                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg10_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal0a                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg11_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal0b                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg12_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal0c                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg13_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal0d                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg14_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal0e                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg15_t;


typedef volatile union {
	struct {
		uint32_t cwt_cal0f                : 24; /**<bit[0 : 23] */
		uint32_t reserved_bit_24_31       :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} adc_reg16_t;


typedef volatile union {
	struct {
		uint32_t adc_int_en               :  1; /**<bit[0 : 0] */
		uint32_t adc_int_level            :  6; /**<bit[1 : 6] */
		uint32_t reserved_bit_7_31        : 25; /**<bit[7 : 31] */
	};
	uint32_t v;
} adc_reg17_t;


typedef volatile union {
	struct {
		uint32_t fifo_rdata               : 16; /**<bit[0 : 15] */
		uint32_t reserved_bit_16_31       : 16; /**<bit[16 : 31] */
	};
	uint32_t v;
} adc_reg18_t;


typedef volatile union {
	struct {
		uint32_t fifo_full                :  1; /**<bit[0 : 0] */
		uint32_t fifo_empty               :  1; /**<bit[1 : 1] */
		uint32_t int_state                :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_31        : 29; /**<bit[3 : 31] */
	};
	uint32_t v;
} adc_reg19_t;

typedef volatile struct {
	volatile adc_reg0_t reg0;
	volatile adc_reg1_t reg1;
	volatile adc_reg2_t reg2;
	volatile adc_reg3_t reg3;
	volatile adc_reg4_t reg4;
	volatile adc_reg5_t reg5;
	volatile adc_reg6_t reg6;
	volatile adc_reg7_t reg7;
	volatile adc_reg8_t reg8;
	volatile adc_reg9_t reg9;
	volatile adc_rega_t rega;
	volatile adc_regb_t regb;
	volatile adc_regc_t regc;
	volatile adc_regd_t regd;
	volatile adc_rege_t rege;
	volatile adc_regf_t regf;
	volatile adc_reg10_t reg10;
	volatile adc_reg11_t reg11;
	volatile adc_reg12_t reg12;
	volatile adc_reg13_t reg13;
	volatile adc_reg14_t reg14;
	volatile adc_reg15_t reg15;
	volatile adc_reg16_t reg16;
	volatile adc_reg17_t reg17;
	volatile adc_reg18_t reg18;
	volatile adc_reg19_t reg19;
} adc_hw_t;

#ifdef __cplusplus
}
#endif
