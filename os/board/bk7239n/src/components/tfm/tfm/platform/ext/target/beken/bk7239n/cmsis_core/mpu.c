// Copyright 2023-2025 Beken
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

#include "platform_irq.h"
#include "STAR_SE.h"
#include "core_star.h"
#include "sdkconfig.h"
#include <common/bk_assert.h>

#define MPU_MAX_NUM_REGIONS     (8UL)                   /* 支持的最大区域数量 */
#define MPU_MAX_NUM_ATTRS       (8UL)                   /* 支持的最大区域数量 */

/*
* #define ARM_MPU_RBAR(BASE, SH, RO, NP, XN)
*
* \brief Region Base Address Register value
* \param BASE The base address bits [31:5] of a memory region. The value is zero extended. Effective address gets 32 byte aligned.
* \param SH Defines the Shareability domain for this memory region.
* \param RO Read-Only: Set to 1 for a read-only memory region.
* \param NP Non-Privileged: Set to 1 for a non-privileged memory region.
* \oaram XN eXecute Never: Set to 1 for a non-executable memory region.
*/
static const ARM_MPU_Region_t mpu_regions[] = {
    /* MPU region 0，
    	itcm_s:  0x0000 0000-----------0x0000 3FFF
    	itcm_ns: 0x1000 0000-----------0x1000 3FFF
    	dtcm_s:  0x2000 0000-----------0x2000 3FFF
    	dtcm_ns: 0x3000 0000-----------0x3000 3FFF
     */
    { ARM_MPU_RBAR(0x00000000UL, ARM_MPU_SH_NON, 0, 1, 0),
      ARM_MPU_RLAR(0x27FFFFE0UL, 0) },

    /* MPU region 1，
        shared memory(smem0) 0x2800 0000-----------0x2800 FFFF   0x3800 0000-----------0x3800 FFFF 
        shared memory(smem1) 0x2801 0000-----------0x2801 FFFF   0x3801 0000-----------0x3801 FFFF
        shared memory(smem2) 0x2802 0000-----------0x2802 FFFF   0x3802 0000-----------0x3802 FFFF
        shared memory(smem3) 0x2804 0000-----------0x2804 FFFF   0x3804 0000-----------0x3804 FFFF
        shared memory(smem4) 0x2806 0000-----------0x2806 FFFF   0x3806 0000-----------0x3806 FFFF
     */
    { ARM_MPU_RBAR(0x28000000UL, ARM_MPU_SH_INNER, 0, 1, 0),
      ARM_MPU_RLAR(0x3FFFFFE0UL, 1) },
    /* MPU region 2，periphral, device memory
        device memory is shareable, and must not be cached.
        please refer to the document:star_user_guide_reference_material.pdf page50
	 */
    { ARM_MPU_RBAR(0x40000000UL, ARM_MPU_SH_INNER, 0, 1, 1),
      ARM_MPU_RLAR(0x5FFFFFE0UL, 2) },

    /* MPU region 3，psram*/
    { ARM_MPU_RBAR(0x60000000UL, ARM_MPU_SH_NON, 0, 1, 1),
      ARM_MPU_RLAR(0x63FFFFE0UL, 3) },

    /* MPU region 4，qspi0*/
    { ARM_MPU_RBAR(0x64000000UL, ARM_MPU_SH_NON, 0, 1, 1),
      ARM_MPU_RLAR(0x67FFFFE0UL, 3) },

     /* MPU region 5，qspi1*/
     { ARM_MPU_RBAR(0x68000000UL, ARM_MPU_SH_NON, 0, 1, 1),
       ARM_MPU_RLAR(0x6BFFFFE0UL, 3) },

     /* MPU region 6， ppb and other */
     { ARM_MPU_RBAR(0x6C000000UL, ARM_MPU_SH_NON, 0, 1, 1),
       ARM_MPU_RLAR(0x6FFFFFE0UL, 2) }
};

/*
 For the star processor, only two combinations of these attributes are valid:Device-nGnRnE/Device-nGnRE
 please refer to the document:star_user_guide_reference_material.pdf page50
 */
static const uint8_t mpu_attrs[] = {
    ARM_MPU_ATTR(0xa, 0xa), // Normal memory, cacheable write through, read allocate, write allocate
    ARM_MPU_ATTR(0x4, 0x4), // Normal memory, non-cacheable
    ARM_MPU_ATTR(0x0, 0x0), // Device memory, bit[3:4]:nGnRnE-00,nGnRE-01
    ARM_MPU_ATTR(0xf, 0xf)  // Normal memory, cacheable write back, read allocate, write allocate
};

void mpu_clear(uint32_t rnr)
{
    ARM_MPU_Disable();
    ARM_MPU_ClrRegion(rnr);
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
}

void mpu_cfg(uint32_t rnr, uint32_t rbar, uint32_t rlar)
{
    ARM_MPU_Disable();
    ARM_MPU_SetRegion(rnr, rbar, rlar);
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
    SCB_CleanInvalidateDCache();
}

void mpu_enable(void)
{
    uint32_t region_num = sizeof(mpu_regions)/sizeof(mpu_regions[0]);
    uint32_t attr_num = sizeof(mpu_attrs)/sizeof(mpu_attrs[0]);

    ARM_MPU_Disable();

    if (region_num > MPU_MAX_NUM_REGIONS){
        region_num = MPU_MAX_NUM_REGIONS;
    }

    for (int i = 0; i < region_num; i++) {
        ARM_MPU_SetRegion(i, mpu_regions[i].RBAR, mpu_regions[i].RLAR);
    }

    for (int j = 0; j < attr_num; j++) {
        ARM_MPU_SetMemAttr(j, mpu_attrs[j]);
    }

    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
}

void mpu_disable(void)
{
    ARM_MPU_Disable();
    for (uint32_t i = 0; i < MPU_MAX_NUM_REGIONS; i++) {
      ARM_MPU_ClrRegion(i);
    }
}

void mpu_dump(void)
{
    BK_DUMP_OUT("******************** Dump mpu config begin ********************\r\n");
    BK_DUMP_OUT("MPU->TYPE: 0x%08X.\r\n", MPU->TYPE);
    BK_DUMP_OUT("MPU->CTRL: 0x%08X.\r\n", MPU->CTRL);
    for (uint32_t i = 0; i < MPU_MAX_NUM_REGIONS; i++) {
        MPU->RNR = i;
        BK_DUMP_OUT("MPU->RNR: %u.\r\n", MPU->RNR);
        BK_DUMP_OUT("MPU->RBAR: 0x%08X.\r\n", MPU->RBAR);
        BK_DUMP_OUT("MPU->RLAR: 0x%08X.\r\n", MPU->RLAR);
        BK_DUMP_OUT("MPU->RBAR_A1: 0x%08X.\r\n", MPU->RBAR_A1);
        BK_DUMP_OUT("MPU->RLAR_A1: 0x%08X.\r\n", MPU->RLAR_A1);
        BK_DUMP_OUT("MPU->RBAR_A2: 0x%08X.\r\n", MPU->RBAR_A2);
        BK_DUMP_OUT("MPU->RLAR_A2: 0x%08X.\r\n", MPU->RLAR_A2);
        BK_DUMP_OUT("MPU->RBAR_A3: 0x%08X.\r\n", MPU->RBAR_A3);
        BK_DUMP_OUT("MPU->RLAR_A3: 0x%08X.\r\n", MPU->RLAR_A3);
        BK_DUMP_OUT("MPU->MAIR0: 0x%08X.\r\n", MPU->MAIR0);
        BK_DUMP_OUT("MPU->MAIR1: 0x%08X.\r\n", MPU->MAIR1);
    }
    BK_DUMP_OUT("******************** Dump mpu config begin ********************\r\n");
}

