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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KB(x) ((x) << 10)

typedef enum {
	MPC_DEV_PSRAM = 0, /**< MPC device PSRAM */
	MPC_DEV_QSPI1,     /**< MPC device QSPI1 */
	MPC_DEV_OTP2,       /**< MPC device OTP */
	MPC_DEV_FLASH,     /**< MPC device FLASH */
	MPC_DEV_SMEM0,     /**< MPC device SMEM0 */
	MPC_DEV_SMEM1,     /**< MPC device SMEM1 */
	MPC_DEV_SMEM2,     /**< MPC device SMEM2 */
	MPC_DEV_SMEM3,     /**< MPC device SMEM3 */
	MPC_DEV_SMEM4,     /**< MPC device SMEM4 */
	MPC_DEV_MAX,       /**< MPC max device id */
} mpc_dev_t;

#define MPC_ALLOC_SCHEMES \
		{\
			{MPC_DEV_PSRAM, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_QSPI1, MPC_ALLOC_SCHEME_NONE},\
		}

extern uint32_t ram_size[];

#ifdef __cplusplus
}
#endif