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
} iomx_dev_id_t;

typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} iomx_ver_id_t;

typedef volatile union {
	struct {
		uint32_t soft_reset               :  1; /**<bit[0 : 0] */
		uint32_t clkg_bypass              :  1; /**<bit[1 : 1] */
		uint32_t gpio_atpg_rd_en          :  1; /**<bit[2 : 2] */
		uint32_t reserved_bit_3_31        : 29; /**<bit[3 : 31] */
	};
	uint32_t v;
} iomx_clkg_reset_t;

typedef volatile union {
	struct {
		uint32_t devstatus                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} iomx_dev_status_t;

typedef volatile union {
	struct {
		uint32_t gpio_input               :  1; /**<bit[0 : 0] */
		uint32_t gpio_output              :  1; /**<bit[1 : 1] */
		uint32_t resv4                    :  1; /**<bit[2 : 2] */
		uint32_t resv3                    :  1; /**<bit[3 : 3] */
		uint32_t gpio_pull_mode           :  1; /**<bit[4 : 4] */
		uint32_t gpio_pull_ena            :  1; /**<bit[5 : 5] */
		uint32_t resv2                    :  1; /**<bit[6 : 6] */
		uint32_t input_monitor            :  1; /**<bit[7 : 7] */
		uint32_t gpio_capacity            :  2; /**<bit[8 : 9] */
		uint32_t gpio_int_type            :  2; /**<bit[10 : 11] */
		uint32_t gpio_int_ena             :  1; /**<bit[12 : 12] */
		uint32_t gpio_int_clear           :  1; /**<bit[13 : 13] */
		uint32_t resv1                    :  2; /**<bit[14 : 15] */
		uint32_t gpio_state               :  5; /**<bit[16 : 20] */
		uint32_t resv0                    :  3; /**<bit[21 : 23] */
		uint32_t gpio_func_sel            :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_cfg_t;

typedef volatile union {
	struct {
		uint32_t gpio_intsta              : 24; /**<bit[0 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_intsta_t;


typedef volatile union {
	struct {
		uint32_t gpio_input               : 24; /**<bit[0 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_input_t;


typedef volatile union {
	struct {
		uint32_t gpio_output              : 24; /**<bit[0 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_output_t;


typedef volatile union {
	struct {
		uint32_t gpio_func_o_atpg         : 24; /**<bit[0 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_func_o_atpg_t;


typedef volatile union {
	struct {
		uint32_t gpio_func_ie_atpg        : 24; /**<bit[0 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_func_ie_atpg_t;


typedef volatile union {
	struct {
		uint32_t gpio_func_oen_atpg       : 24; /**<bit[0 : 23] */
		uint32_t reserved_24_31           :  8; /**<bit[24 : 31] */
	};
	uint32_t v;
} iomx_gpio_func_oen_atpg_t;

typedef volatile struct {
	volatile iomx_dev_id_t dev_id;
	volatile iomx_ver_id_t ver_id;
	volatile iomx_clkg_reset_t clkg_reset;
	volatile iomx_dev_status_t dev_status;
	volatile uint32_t rsv_4_3f[60];
	volatile iomx_gpio_cfg_t gpio_cfg[SOC_GPIO_NUM];
	volatile uint32_t reserved[0xc0 - SOC_GPIO_NUM - 1];
	volatile iomx_gpio_intsta_t gpio_intsta;
	volatile uint32_t rsv_c1_c3[3];
	volatile iomx_gpio_input_t gpio_input;
	volatile uint32_t rsv_c5_c7[3];
	volatile iomx_gpio_output_t gpio_output;
	volatile uint32_t rsv_c9_cb[3];
	volatile iomx_gpio_func_o_atpg_t gpio_func_o_atpg;
	volatile uint32_t rsv_cd_cf[3];
	volatile iomx_gpio_func_ie_atpg_t gpio_func_ie_atpg;
	volatile uint32_t rsv_d1_d3[3];
	volatile iomx_gpio_func_oen_atpg_t gpio_func_oen_atpg;
} iomx_hw_t;

#ifdef __cplusplus
}
#endif
