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

#include "bk_arm_arch.h"

#define PS_USE_GPIO_TRACE           0

#if (CONFIG_SOC_BK7231)
#define PS_MAC_DBG_GPIO                 (0x0802800 +(10*4))
#define PS_RF_DBG_GPIO                 (0x0802800 +(14*4))
#define PS_HWUP_DBG_GPIO                 (0x0802800 +(15*4))
#define PS_BCN_DBG_GPIO                (0x0802800 +(11*4))
#define PS_HWDOWN_DBG_GPIO               (0x0802800 +(17*4))
#define PS_TM_DBG_GPIO                (0x0802800 +(16*4))
#elif (CONFIG_SOC_BK7231U)
#define PS_MAC_DBG_GPIO                 (0x0802800 +(23*4))
#define PS_RF_DBG_GPIO                 (0x0802800 +(22*4))
#define PS_HWUP_DBG_GPIO                 (0x0802800 +(21*4))
#define PS_BCN_DBG_GPIO                (0x0802800 +(2*4))
#define PS_HWDOWN_DBG_GPIO               (0x0802800 +(20*4))
#define PS_TM_DBG_GPIO                (0x0802800 +(3*4))
#elif (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7256XX)
#define PS_MAC_DBG_GPIO                 (0x0802800 +(6*4))
#define PS_RF_DBG_GPIO                 (0x0802800 +(7*4))
#define PS_HWUP_DBG_GPIO                 (0x0802800 +(8*4))
#define PS_BCN_DBG_GPIO                (0x0802800 +(9*4))
#define PS_HWDOWN_DBG_GPIO               (0x0802800 +(20*4))
#define PS_TM_DBG_GPIO                (0x0802800 +(21*4))
#elif (CONFIG_SOC_BK7251) || (CONFIG_SOC_BK7271)
#define PS_MAC_DBG_GPIO                 (0x0802800 +(27*4))
#define PS_RF_DBG_GPIO                 (0x0802800 +(22*4))
#define PS_HWUP_DBG_GPIO                 (0x0802800 +(23*4))
#define PS_BCN_DBG_GPIO                (0x0802800 +(30*4))
#define PS_HWDOWN_DBG_GPIO               (0x0802800 +(20*4))
#define PS_TM_DBG_GPIO                (0x0802800 +(21*4))
#endif

#if PS_USE_GPIO_TRACE
#define PS_DEBUG_RF_OUT 	(REG_WRITE(PS_RF_DBG_GPIO, 0x00))
#define PS_DEBUG_HWUP_OUT 	(REG_WRITE(PS_HWUP_DBG_GPIO, 0x00))
#define PS_DEBUG_MAC_OUT 	(REG_WRITE(PS_MAC_DBG_GPIO, 0x00))
#define PS_DEBUG_BCN_OUT 	(REG_WRITE(PS_BCN_DBG_GPIO, 0x00))
#define PS_DEBUG_HWDOWN_OUT 	(REG_WRITE(PS_HWDOWN_DBG_GPIO, 0x00))
#define PS_DEBUG_TM_OUT 	(REG_WRITE(PS_TM_DBG_GPIO, 0x00))

#define PS_DEBUG_RF_TRIGER 	(REG_WRITE(PS_RF_DBG_GPIO,(REG_READ(PS_RF_DBG_GPIO))^(0x2)))
#define PS_DEBUG_BCN_TRIGER 	(REG_WRITE(PS_BCN_DBG_GPIO,(REG_READ(PS_BCN_DBG_GPIO))^(0x2)))
#define PS_DEBUG_MAC_TRIGER 	(REG_WRITE(PS_MAC_DBG_GPIO,(REG_READ(PS_MAC_DBG_GPIO))^(0x2)))
#define PS_DEBUG_HWUP_TRIGER 	(REG_WRITE(PS_HWUP_DBG_GPIO,(REG_READ(PS_HWUP_DBG_GPIO))^(0x2)))
#define PS_DEBUG_HWDOWN_TRIGER 	(REG_WRITE(PS_HWDOWN_DBG_GPIO,(REG_READ(PS_HWDOWN_DBG_GPIO))^(0x2)))
#define PS_DEBUG_TM_TRIGER 	(REG_WRITE(PS_TM_DBG_GPIO,(REG_READ(PS_TM_DBG_GPIO))^(0x2)))

#else
#define PS_DEBUG_RF_OUT
#define PS_DEBUG_HWUP_OUT
#define PS_DEBUG_MAC_OUT
#define PS_DEBUG_BCN_OUT
#define PS_DEBUG_HWDOWN_OUT
#define PS_DEBUG_TM_OUT
#define PS_DEBUG_RF_TRIGER
#define PS_DEBUG_BCN_TRIGER
#define PS_DEBUG_MAC_TRIGER
#define PS_DEBUG_HWUP_TRIGER
#define PS_DEBUG_HWDOWN_TRIGER
#define PS_DEBUG_TM_TRIGER
#endif

#ifdef __cplusplus
}
#endif
