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
} i2s_smb_devid_t;


typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} i2s_smb_verid_t;


typedef volatile union {
	struct {
		uint32_t soft_reset               :  1; /**<bit[0 : 0] */
		uint32_t clkg_bypass              :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} i2s_smb_clkrst_t;


typedef volatile union {
	struct {
		uint32_t devstatus                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} i2s_smb_state_t;


typedef volatile union {
	struct {
		uint32_t bitratio                 :  8; /**<bit[0 : 7] */
		uint32_t smpratio                 :  5; /**<bit[8 : 12] */
		uint32_t pcm_dlen                 :  3; /**<bit[13 : 15] */
		uint32_t datalen                  :  5; /**<bit[16 : 20] */
		uint32_t synclen                  :  3; /**<bit[21 : 23] */
		uint32_t lsbfirst                 :  1; /**<bit[24 : 24] */
		uint32_t sclkinv                  :  1; /**<bit[25 : 25] */
		uint32_t lrckrp                   :  1; /**<bit[26 : 26] */
		uint32_t modesel                  :  3; /**<bit[27 : 29] */
		uint32_t msten                    :  1; /**<bit[30 : 30] */
		uint32_t i2spcmen                 :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} i2s_pcm_cfg_t;


typedef volatile union {
	struct {
		uint32_t rxint_en                 :  1; /**<bit[0 : 0] */
		uint32_t txint_en                 :  1; /**<bit[1 : 1] */
		uint32_t rxovf_en                 :  1; /**<bit[2 : 2] */
		uint32_t txudf_en                 :  1; /**<bit[3 : 3] */
		uint32_t rxint_level              :  2; /**<bit[4 : 5] */
		uint32_t txint_level              :  2; /**<bit[6 : 7] */
		uint32_t txfifo_clr               :  1; /**<bit[8 : 8] */
		uint32_t rxfifo_clr               :  1; /**<bit[9 : 9] */
		uint32_t smpratio_h2b             :  2; /**<bit[10 : 11] */
		uint32_t bitratio_h4b             :  4; /**<bit[12 : 15] */
		uint32_t lrcom_store              :  1; /**<bit[16 : 16] */
		uint32_t parallel_en              :  1; /**<bit[17 : 17] */
		uint32_t reserved_18_31           : 14; /**<bit[18 : 31] */
	};
	uint32_t v;
} i2s_pcm_cn_t;


typedef volatile union {
	struct {
		uint32_t rxint                    :  1; /**<bit[0 : 0] */
		uint32_t txint                    :  1; /**<bit[1 : 1] */
		uint32_t rxovf                    :  1; /**<bit[2 : 2] */
		uint32_t txudf                    :  1; /**<bit[3 : 3] */
		uint32_t rxfifo_rd_ready          :  1; /**<bit[4 : 4] */
		uint32_t txfifo_wr_ready          :  1; /**<bit[5 : 5] */
		uint32_t reserved_6_31            : 26; /**<bit[6 : 31] */
	};
	uint32_t v;
} i2s_pcm_stat_t;


typedef volatile union {
	struct {
		uint32_t i2s_dat                  : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} i2s_pcm_dat_t;


typedef volatile struct {
	volatile i2s_smb_devid_t smb_devid;
	volatile i2s_smb_verid_t smb_verid;
	volatile i2s_smb_clkrst_t smb_clkrst;
	volatile i2s_smb_state_t smb_state;
	volatile i2s_pcm_cfg_t pcm_cfg;
	volatile i2s_pcm_cn_t pcm_cn;
	volatile i2s_pcm_stat_t pcm_stat;
	volatile i2s_pcm_dat_t pcm_dat;
} i2s_hw_t;

#ifdef __cplusplus
}
#endif
