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


#define CKMN_DEVICEID_ADDR (SOC_CKMN_REG_BASE + (0x0 << 2))

#define CKMN_DEVICEID_POS (0)
#define CKMN_DEVICEID_MASK (0xffffffff)

#define CKMN_VERSIONID_ADDR (SOC_CKMN_REG_BASE + (0x1 << 2))

#define CKMN_VERSIONID_POS (0)
#define CKMN_VERSIONID_MASK (0xffffffff)

#define CKMN_CTRL_ADDR (SOC_CKMN_REG_BASE + (0x2 << 2))

#define CKMN_CTRL_SOFT_RESET_POS (0)
#define CKMN_CTRL_SOFT_RESET_MASK (0x1)

#define CKMN_CTRL_CKG_BYPASS_POS (1)
#define CKMN_CTRL_CKG_BYPASS_MASK (0x1)

#define CKMN_CTRL_RESERVED_BIT_2_31_POS (2)
#define CKMN_CTRL_RESERVED_BIT_2_31_MASK (0x3fffffff)

#define CKMN_DEVICESTATUS_ADDR (SOC_CKMN_REG_BASE + (0x3 << 2))

#define CKMN_DEVICESTATUS_POS (0)
#define CKMN_DEVICESTATUS_MASK (0xffffffff)

#define CKMN_RC32K_ADDR (SOC_CKMN_REG_BASE + (0x4 << 2))

#define CKMN_RC32K_COUNT_POS (0)
#define CKMN_RC32K_COUNT_MASK (0x3ff)

#define CKMN_RC32K_RESERVED_10_31_POS (10)
#define CKMN_RC32K_RESERVED_10_31_MASK (0x3fffff)

#define CKMN_RC32K_CTRL_ADDR (SOC_CKMN_REG_BASE + (0x5 << 2))

#define CKMN_RC32K_CTRL_ENABLE_POS (0)
#define CKMN_RC32K_CTRL_ENABLE_MASK (0x1)

#define CKMN_RC32K_CTRL_INTR_ENABLE_POS (1)
#define CKMN_RC32K_CTRL_INTR_ENABLE_MASK (0x1)

#define CKMN_RC32K_CTRL_RESERVED_2_31_POS (2)
#define CKMN_RC32K_CTRL_RESERVED_2_31_MASK (0x3fffffff)

#define CKMN_RC26M_ADDR (SOC_CKMN_REG_BASE + (0x6 << 2))

#define CKMN_RC26M_COUNT_POS (0)
#define CKMN_RC26M_COUNT_MASK (0xfffff)

#define CKMN_RC26M_RESERVED_BIT_20_31_POS (20)
#define CKMN_RC26M_RESERVED_BIT_20_31_MASK (0xfff)

#define CKMN_CORR_CFG_ADDR (SOC_CKMN_REG_BASE + (0x7 << 2))

#define CKMN_26M_CORR_TARGET_POS (0)
#define CKMN_26M_CORR_TARGET_MASK (0x3)

#define CKMN_26M_AUTOSW_ENABLE_POS (2)
#define CKMN_26M_AUTOSW_ENABLE_MASK (0x1)

#define CKMN_26M_CORR_ENABLE_POS (3)
#define CKMN_26M_CORR_ENABLE_MASK (0x1)

#define CKMN_32K_CORR_TARGET_POS (4)
#define CKMN_32K_CORR_TARGET_MASK (0x3)

#define CKMN_32K_AUTOSW_ENABLE_POS (6)
#define CKMN_32K_AUTOSW_ENABLE_MASK (0x1)

#define CKMN_32K_CORR_ENABLE_POS (7)
#define CKMN_32K_CORR_ENABLE_MASK (0x1)

#define CKMN_COR26M_INTR_ENABLE_POS (8)
#define CKMN_COR26M_INTR_ENABLE_MASK (0x1)

#define CKMN_COR32K_INTR_ENABLE_POS (9)
#define CKMN_COR32K_INTR_ENABLE_MASK (0x1)

#define CKMN_CORR_CFG_RESERVED_BIT_10_31_POS (10)
#define CKMN_CORR_CFG_RESERVED_BIT_10_31_MASK (0x3fffff)

#define CKMN_INTR_ADDR (SOC_CKMN_REG_BASE + (0x8 << 2))

#define CKMN_CKEST_INTR_STATUS_POS (0)
#define CKMN_CKEST_INTR_STATUS_MASK (0x1)

#define CKMN_COR26M_INTR_STATUS_POS (1)
#define CKMN_COR26M_INTR_STATUS_MASK (0x1)

#define CKMN_COR32K_INTR_STATUS_POS (2)
#define CKMN_COR32K_INTR_STATUS_MASK (0x1)

#define CKMN_INTR_RESERVED_BIT_3_31_POS (3)
#define CKMN_INTR_RESERVED_BIT_3_31_MASK (0x1fffffff)

#ifdef __cplusplus
}
#endif
