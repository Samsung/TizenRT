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

// #include "cmsis.h"
#include <stdint.h>
#include "mpu_config.h"
#include "os/os.h"
#include "common/bk_assert.h"
#include "armstar.h"

#include "components/log.h"
#include "components/system.h"

#define MPU_MAX_REGION 16

uint8_t mpu_entry_register[MPU_MAX_REGION];


void mpu_enable(void)
{
	MPU->CTRL |= (1 << MPU_CTRL_ENABLE_Pos); /* Enable the MPU */

	__DSB(); /* Force memory writes before continuing */
	__ISB(); /* Flush and refill pipeline with updated permissions */
}


void mpu_disable(void)
{
	/* Force any outstanding transfers to complete before disabling MPU */
	__DMB();

	MPU->CTRL &= ~(1 << MPU_CTRL_ENABLE_Pos);
}

/**
  * @brief    init mpu memory attribute to typical value.
  * @param  NA.
  * @note    typical value:
  *              - typical value if defined in @ref mpu_mem_attri_typical_define.
  * @retval   None
  */

void mpu_init(void)
{
	mpu_disable();

	MPU->CTRL = 0;

	/* Configure memory types */
	MPU->MAIR0 = ((MPU_MEM_ATTR0) << MPU_MAIR0_Attr0_Pos) | \
				 ((MPU_MEM_ATTR1) << MPU_MAIR0_Attr1_Pos) | \
				 ((MPU_MEM_ATTR2) << MPU_MAIR0_Attr2_Pos) | \
				 ((MPU_MEM_ATTR3) << MPU_MAIR0_Attr3_Pos);

	MPU->MAIR1 = ((MPU_MEM_ATTR4) << MPU_MAIR1_Attr4_Pos) | \
				 ((MPU_MEM_ATTR5) << MPU_MAIR1_Attr5_Pos) | \
				 ((MPU_MEM_ATTR6) << MPU_MAIR1_Attr6_Pos) | \
				 ((MPU_MEM_ATTR7) << MPU_MAIR1_Attr7_Pos);

	MPU->CTRL |= (MPU_INIT_CTRL_PRIVDEFENA << MPU_CTRL_PRIVDEFENA_Pos) | \
				 (MPU_INIT_CTRL_HFNMIENA << MPU_CTRL_HFNMIENA_Pos);

	mpu_enable();
}

/**
  * @brief    change mpu memory attribute.
  * @param  attr_idx: where x can be 0~7.
  * @param  mem_attr: memory attribute. @ref mpu_region_memory_attribute_define.
  * @note    NA:
  *              - NA.
  * @retval   None
  */

void mpu_set_mem_attr(uint8_t attr_idx, uint8_t mem_attr)
{
	uint32_t mair_mask;
	uint32_t bit_offset;

	BK_ASSERT(attr_idx < MPU_MAX_REGION);

	mpu_disable();

	bit_offset = (8 * (attr_idx & 0x03));
	mair_mask = 0xFF << bit_offset;

	if (attr_idx < 4) {
		MPU->MAIR0 &= ~mair_mask;
		MPU->MAIR0 |= mem_attr << bit_offset;
	} else {
		MPU->MAIR1 &= ~mair_mask;
		MPU->MAIR1 |= mem_attr << bit_offset;
	}

	mpu_enable();
}

/**
  * @brief    config mpu region memory attribute.
  * @param  region_num: where x can be 0~3.
  * @param  pmpu_cfg: pointer to an mpu_region_config structure which has been configured.
  * @note    NA:
  * @retval   None
  */

void mpu_region_cfg(uint8_t region_num, mpu_region_config *pmpu_cfg)
{
	u32 region_limit = 0;

	BK_ASSERT(region_num < MPU_MAX_REGION);

	if (pmpu_cfg->region_base & ~MPU_RBAR_BASE_Msk) {
		bk_printf("MPU: region_base should be 32 bytes aligned %08x\n",
				  pmpu_cfg->region_base);
		BK_ASSERT(0);
	}
	if (pmpu_cfg->region_size & ~MPU_RLAR_LIMIT_Msk) {
		bk_printf("MPU: region_size should be 32 bytes aligned %08x\n",
				  pmpu_cfg->region_size);
		BK_ASSERT(0);
	}
	region_limit = pmpu_cfg->region_base + pmpu_cfg->region_size;

	mpu_disable();

	MPU->RNR = region_num;

	MPU->RBAR = (pmpu_cfg->region_base & MPU_RBAR_BASE_Msk) | \
				pmpu_cfg->sh | \
				pmpu_cfg->ap | \
				pmpu_cfg->xn;

	MPU->RLAR = (region_limit & MPU_RLAR_LIMIT_Msk) | \
				((pmpu_cfg->attr_idx << MPU_RLAR_AttrIndx_Pos) & MPU_RLAR_AttrIndx_Msk) | \
				1;

	MPU->CTRL |= (1 << MPU_CTRL_ENABLE_Pos); /* Enable the MPU */

	mpu_enable();

    // bk_printf("=====================================\n");
	// bk_printf("mpu_region_cfg: pmpu_cfg->attr_idx:%x\n",
	// 		   pmpu_cfg->attr_idx);
	// bk_printf("mpu_region_cfg: RNR:%x RBAR:%x RLAR:%x \n",
	// 		   MPU->RNR, MPU->RBAR, MPU->RLAR);
	// bk_printf("mpu_region_cfg: MAIR0:%x MAIR1:%x CTRL:%x TYPE:%x\n",
	// 		   MPU->MAIR0, MPU->MAIR1, MPU->CTRL, MPU->TYPE);

}

/**
  * @brief  free mpu entry.
  * @param  entry_index: mpu entry index 0~7.
  * @retval   None
  */

void mpu_entry_free(uint32_t entry_index)
{
	mpu_entry_register[entry_index] = 0;
}

/**
  * @brief  alloc a free mpu entry.
  * @param  NA.
  * @retval value: mpu entry index  0~7 -1 is fail.
  */

char mpu_entry_alloc(void)
{
	u32 entry_index = 0;

	for (entry_index = 0; entry_index < MPU_MAX_REGION; entry_index++) {
		if (mpu_entry_register[entry_index] == 0) {
			mpu_entry_register[entry_index] = 1;
			return entry_index;
		}
	}

	return -1;
}

