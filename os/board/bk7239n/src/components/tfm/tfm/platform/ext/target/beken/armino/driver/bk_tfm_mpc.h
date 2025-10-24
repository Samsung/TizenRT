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

#pragma once

#include "driver/mpc.h"
#include "flash_partition.h"

int bk_mpc_cfg(void);

#define MPC_ALLOC_SCHEME_ALL  (-1)
#define MPC_ALLOC_SCHEME_NONE (-2)
#define MPC_ALLOC_SCHEME_HALF (-3)



typedef struct {
    mpc_dev_t dev;
    int bytes_for_spe; // if bytes_for_spe < 0, means allocation scheme
} mpc_alloc_scheme_t;


#if CONFIG_TFM_S_JUMP_TO_CPU0_APP
#define MPC_ALLOC_SCHEMES \
		{\
			{MPC_DEV_PSRAM, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_QSPI0, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_QSPI1, MPC_ALLOC_SCHEME_NONE},\
		}
#else
#if (CONFIG_ENABLE_TEST_NS) || (CONFIG_ENABLE_TEST_S)
/*please check bk7236_ns.ld, half of ram is for nspe*/
#define MPC_ALLOC_SCHEMES \
		{\
			{MPC_DEV_PSRAM, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_QSPI0, MPC_ALLOC_SCHEME_ALL},\
			{MPC_DEV_QSPI1, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_OTP2,	MPC_ALLOC_SCHEME_HALF},\
			{MPC_DEV_FLASH, FLASH_PHY2VIRTUAL(CONFIG_PRIMARY_TFM_NS_PHY_PARTITION_OFFSET)},\
			{MPC_DEV_SMEM0, MPC_ALLOC_SCHEME_ALL},\
			{MPC_DEV_SMEM1, MPC_ALLOC_SCHEME_ALL},\
			{MPC_DEV_SMEM2, MPC_ALLOC_SCHEME_ALL},\
			{MPC_DEV_SMEM3, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_SMEM4, MPC_ALLOC_SCHEME_NONE},\
			{MPC_DEV_SMEM5, MPC_ALLOC_SCHEME_NONE},\
		}
#endif
#endif

int bk_mpc_enable_interrupt(void);
int bk_mpc_cfg(void);
// eof

