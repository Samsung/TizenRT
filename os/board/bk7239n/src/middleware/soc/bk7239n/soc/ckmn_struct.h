// Copyright 2022-2023 Beken
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

typedef volatile struct {

	uint32_t deviceid; 								/**<bit[0 : 31] */

	uint32_t versionid; 							/**<bit[0 : 31] */

	union {
		struct {
			uint32_t soft_reset               :  1; /**<bit[0 : 0] */
			uint32_t ckg_bypass               :  1; /**<bit[1 : 1] */
			uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
		};
		uint32_t v;
	} global_ctrl;

	uint32_t devicestatus; 							/**<bit[0 : 31] */

	union {
		struct {
			uint32_t ckes_count               : 10; /**<bit[0 : 9] */
			uint32_t reserved_10_31           : 22; /**<bit[10 : 31] */
		};
		uint32_t v;
	} rc32k_count;

	union {
		struct {
			uint32_t ckes_enable              :  1; /**<bit[0 : 0] */
			uint32_t ckes_intr_enable         :  1; /**<bit[1 : 1] */
			uint32_t reserved_2_31            : 30; /**<bit[2 : 31] */
		};
		uint32_t v;
	} rc32k_ctrl;

	union {
		struct {
			uint32_t ckes_number              : 20; /**<bit[0 : 19] */
			uint32_t reserved_bit_20_31       : 12; /**<bit[20 : 31] */
		};
		uint32_t v;
	} rc26m_count;

 	union {
		struct {
			uint32_t corr_26m_target          :  2; /**<bit[0 : 1] */
			uint32_t autosw_26m_enable        :  1; /**<bit[2 : 2] */
			uint32_t corr_26m_enable          :  1; /**<bit[3 : 3] */
			uint32_t corr_32k_target          :  2; /**<bit[4 : 5] */
			uint32_t autosw_32k_enable        :  1; /**<bit[6 : 6] */
			uint32_t corr_32k_enable          :  1; /**<bit[7 : 7] */
			uint32_t cor26m_intr_enable       :  1; /**<bit[8 : 8] */
			uint32_t cor32k_intr_enable       :  1; /**<bit[9 : 9] */
			uint32_t reserved_bit_10_31       : 22; /**<bit[10 : 31] */
		};
		uint32_t v;
	} corr_cfg;

	union {
		struct {
			uint32_t ckest_intr_status        :  1; /**<bit[0 : 0] */
			uint32_t cor26m_intr_status       :  1; /**<bit[1 : 1] */
			uint32_t cor32k_intr_status       :  1; /**<bit[2 : 2] */
			uint32_t reserved_bit_3_31        : 29; /**<bit[3 : 31] */
		};
		uint32_t v;
	} intr;
} ckmn_hw_t;

#ifdef __cplusplus
}
#endif
