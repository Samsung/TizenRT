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
	/* REG_0x0 */
	uint32_t device_id;

	/* REG_0x1 */
	uint32_t version_id;

	/* REG_0x2 */
	union {
		struct {
			uint32_t soft_reset: 1;
			uint32_t bps_clk_gate: 1;
			uint32_t prio_mode: 1;
			uint32_t reserved: 29;
		};
		uint32_t v;
	} prio_mode;

	uint32_t reg_gap_0;

	/* REG_0x4 */
	union {
		struct {
			uint32_t attr: 12;
			uint32_t reserved: 20;
		};
		uint32_t v;
	} secure_attr;

	/* REG_0x5 */
	union {
		struct {
			uint32_t attr: 12;
			uint32_t reserved: 20;
		};
		uint32_t v;
	} privileged_attr;

	/* REG_0x6 */
	union {
		struct {
			uint32_t status: 12;
			uint32_t reserved: 20;
		};
		uint32_t v;
	} int_status_sec;

	/* REG_0x7 */
	union {
		struct {
			uint32_t status: 12;
			uint32_t reserved: 20;
		};
		uint32_t v;
	} int_status_nonsec;

	/* REG_0x8 */
	union {
		struct {
			uint32_t reserved: 32;
		};
		uint32_t v;
	} int_status2;
	/* REG_0x9 */
	union {
		struct {
			uint32_t reserved: 32;
		};
		uint32_t v;
	} int_status3;
	/* REG_0xA */
	union {
		struct {
			uint32_t status: 16;
			uint32_t reserved: 16;
		};
		uint32_t v;
	} int_allocate;

	uint32_t reg_gap_1[5];

	/* REG_CHANN(x) */
	struct {
		/* REG_0x10 */
		union {
			struct {
				uint32_t enable: 1;
				uint32_t finish_int_en: 1;
				uint32_t half_finish_int_en: 1;
				uint32_t mode: 1;
				uint32_t src_data_width: 2;
				uint32_t dest_data_width: 2;
				uint32_t src_addr_inc_en: 1;
				uint32_t dest_addr_inc_en: 1;
				uint32_t src_addr_loop_en: 1;
				uint32_t dest_addr_loop_en: 1;
				uint32_t chan_prio: 3;
				uint32_t reserved: 1;
				uint32_t transfer_len: 16;
			};
			uint32_t v;
		} ctrl;

		/* REG_0x11 */
		uint32_t dest_start_addr;

		/* REG_0x12 */
		uint32_t src_start_addr;

		/* REG_0x13 */
		uint32_t dest_loop_end_addr;

		/* REG_0x14 */
		uint32_t dest_loop_start_addr;

		/* REG_0x15 */
		uint32_t src_loop_end_addr;

		/* REG_0x16 */
		uint32_t src_loop_start_addr;

		/* REG_0x17 */
		union {
			struct {
				uint32_t src_req_mux: 5;
				uint32_t dest_req_mux: 5;
				uint32_t reserved0: 2;
				uint32_t src_read_interval: 4;
				uint32_t dest_write_interval: 4;
				uint32_t src_sec_attr: 1;
				uint32_t dest_sec_attr: 1;
				uint32_t bus_err_int_en: 1;
				uint32_t reserved1: 1;
				uint32_t src_burst_len: 2;
				uint32_t dtst_burst_len: 2;
				uint32_t pixel_trans_type: 2;
				uint32_t reserved2: 2;
			};
			uint32_t v;
		} req_mux;

		/* REG_0x18 */
		uint32_t src_pause_addr;

		/* REG_0x19 */
		uint32_t dest_pause_addr;

		/* REG_0x1A */
		uint32_t src_rd_addr;

		/* REG_0x1B */
		uint32_t dest_wr_addr;

		/* REG_0x1C */
		union {
			struct {
				uint32_t remain_len: 17;
				uint32_t flush_src_buff: 1;
				uint32_t finish_int: 1;
				uint32_t half_finish_int: 1;
				uint32_t bus_err_int: 1;
				uint32_t reserved: 1;
				uint32_t repeat_wr_pause: 1;
				uint32_t repeat_rd_pause: 1;
				uint32_t finish_int_counter: 4;
				uint32_t half_finish_int_counter: 4;
			};
			uint32_t v;
		} status;

		uint32_t reg_gap_0[3];
	} config_group[SOC_DMA_CHAN_NUM_PER_UNIT];
} dma_hw_t;

#ifdef __cplusplus
}
#endif

