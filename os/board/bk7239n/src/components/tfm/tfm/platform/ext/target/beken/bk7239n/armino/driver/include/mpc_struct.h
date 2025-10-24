// Copyright 2020-2021 Beken
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

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	/* REG_0x00*/
	union {
		struct {
			uint32_t reserved0:   4;
			uint32_t cfg_sec_rsp: 1;
			uint32_t reserved1:   1;
			uint32_t gating_req:  1;
			uint32_t gating_ack:  1;
			uint32_t auto_inc:    1;
			uint32_t reserved2:  22;
			uint32_t sec_lock:    1;
		};
		uint32_t v;
	} ctrl;

	uint32_t reserved[3];

	/* REG_0x04 */
	uint32_t block_index_max;

	/* REG_0x05 */
	union {
		struct {
			uint32_t block_size: 4;
			uint32_t reserved:  28;
		};
		uint32_t v;
	} block_size;

	/* REG_0x06 */
	uint32_t block_index;

	/* REG_0x07 */
	uint32_t block_lut;
} mpc_hw_t;

#ifdef __cplusplus
}
#endif

