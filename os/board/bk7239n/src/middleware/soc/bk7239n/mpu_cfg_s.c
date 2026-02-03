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

#include "bk7239xx.h"
#include "os/os.h"
#include "common/bk_assert.h"

#ifdef CONFIG_ARMV8M_MPU
#include "up_mpuinit.h"
#include "mpu_config.h"
#endif

#ifdef CONFIG_XIP_ELF
#include "partitions_gen_ns.h"
#endif


uint32_t g_mpu_nregion_allocated;

#if CONFIG_KMM_REGIONS == 1
extern uint32_t RAM_KREGION1_START;
extern uint32_t RAM_KREGION1_SIZE;
#define PSRAM_HEAP_REGION_START (uint32_t)&RAM_KREGION1_START
#define PSRAM_HEAP_REGION_SIZE (uint32_t)&RAM_KREGION1_SIZE
#else
extern uint32_t RAM_KREGION0_START;
extern uint32_t RAM_KREGION0_SIZE;
#define PSRAM_HEAP_REGION_START (uint32_t)&RAM_KREGION0_START
#define PSRAM_HEAP_REGION_SIZE (uint32_t)&RAM_KREGION0_SIZE
#endif

/*----------------------------------------------------------------------------
  MPU configuration
 *----------------------------------------------------------------------------*/
void bk_mpu_init(void)
{
    mpu_region_config mpu_cfg;
	uint32_t  mpu_entry = 0;

     /* MPU region 1, RO-code/RO-data.
    	Flash_s:  0x0200 0000-----------0x02FF FFFF
    	Flash_ns: 0x1200 0000-----------0x12FF FFFF
        Flash, for RO-code/RO-data. WT-RA 
     */
    mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x12000000UL;
 #if defined(CONFIG_XIP_ELF)
    /* For XIP_ELF, kernel only configures its own Flash region to avoid
	 * MPU region conflicts with common and app Flash regions which are
	 * configured later during binary loading.
	 */
	mpu_cfg.region_size = CONFIG_COMMON_PHY_PARTITION_OFFSET -32;
#else
    /* For non-XIP_ELF, configure entire Flash space */
	mpu_cfg.region_size = 0xFFFFE0;
#endif
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_PRIV_W;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_WT_T_RA;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 2
    	iram_s:  0x0800 0000-----------0x0809 FFFF
    	iram_ns: 0x1800 0000-----------0x1809 FFFF
        SRAM, for RO-code/RO-data. WT-RA
     */
    mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x18000000UL;
	mpu_cfg.region_size = 0x9FFE0;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_PRIV_W;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 3
        shared memory(smem0) 0x2800 0000-----------0x2800 FFFF   0x3800 0000-----------0x3800 FFFF 
        shared memory(smem1) 0x2801 0000-----------0x2801 FFFF   0x3801 0000-----------0x3801 FFFF
        shared memory(smem2) 0x2802 0000-----------0x2803 FFFF   0x3802 0000-----------0x3803 FFFF
        shared memory(smem3) 0x2804 0000-----------0x2805 FFFF   0x3804 0000-----------0x3805 FFFF
        shared memory(smem4) 0x2806 0000-----------0x2807 FFFF   0x3806 0000-----------0x3807 FFFF
        shared memory(smem4) 0x2808 0000-----------0x2809 FFFF   0x3808 0000-----------0x3809 FFFF 
     */
#if defined(CONFIG_CACHE_ENABLE) 
    mpu_entry = mpu_entry_alloc();
    mpu_cfg.region_base = 0x38000000UL;
    mpu_cfg.region_size = 0x7FFE0;
    mpu_cfg.xn = MPU_EXEC_ALLOW;
    mpu_cfg.ap = MPU_UN_PRIV_RW;
    mpu_cfg.sh = MPU_NON_SHAREABLE;
    mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_WT_T_RA;
    mpu_region_cfg(mpu_entry, &mpu_cfg);
#else
    mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x38000000UL;
	mpu_cfg.region_size = 0x7FFE0;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);
#endif

    /* MPU region 4 periphral, device memory
        device memory is shareable, and must not be cached.
	 */
    mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x50000000;
	mpu_cfg.region_size = 0x1FFFFFE0;
	mpu_cfg.xn = MPU_EXEC_NEVER;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_INR_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_DEVICE;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

   /* MPU region 5 psram data */
    mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = CONFIG_BK_PSRAM_DATA_BASE;
	mpu_cfg.region_size = CONFIG_BK_PSRAM_DATA_SIZE - 32;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 6 psram code*/
	mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = CONFIG_BK_PSRAM_CODE_BASE;
	mpu_cfg.region_size = CONFIG_BK_PSRAM_CODE_SIZE - 32;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_WT_T_RA;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 7 psram heap*/
    mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = PSRAM_HEAP_REGION_START;
	mpu_cfg.region_size = PSRAM_HEAP_REGION_SIZE - 32;
	mpu_cfg.xn = MPU_EXEC_NEVER;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
#if defined(CONFIG_BK_TIZEN_IPERF_PROJECT) && (CONFIG_BK_TIZEN_IPERF_PROJECT == 1)
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
#else
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_WB_T_RWA;
#endif
	mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 8 qspi0 */
    // mpu_entry = mpu_entry_alloc();
	// mpu_cfg.region_base = 0x74000000;
	// mpu_cfg.region_size = 0x3FFFFE0;
	// mpu_cfg.xn = MPU_EXEC_NEVER;
	// mpu_cfg.ap = MPU_UN_PRIV_RW;
	// mpu_cfg.sh = MPU_NON_SHAREABLE;
	// mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	// mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 8 qspi1 */
    // mpu_entry = mpu_entry_alloc();
	// mpu_cfg.region_base = 0x78000000;
	// mpu_cfg.region_size = 0x3FFFFE0;
	// mpu_cfg.xn = MPU_EXEC_NEVER;
	// mpu_cfg.ap = MPU_UN_PRIV_RW;
	// mpu_cfg.sh = MPU_NON_SHAREABLE;
	// mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_WB_T_RWA;
	// mpu_region_cfg(mpu_entry, &mpu_cfg);

    /* MPU region 11 ppb and other */
    // mpu_entry = mpu_entry_alloc();
	// mpu_cfg.region_base = 0x80000000;
	// mpu_cfg.region_size = 0x6FFFFFE0;
	// mpu_cfg.xn = MPU_EXEC_NEVER;
	// mpu_cfg.ap = MPU_UN_PRIV_RW;
	// mpu_cfg.sh = MPU_NON_SHAREABLE;
	// mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_DEVICE;
	// mpu_region_cfg(mpu_entry, &mpu_cfg);


    g_mpu_nregion_allocated = mpu_entry + 1;

}

void mpu_dump(void)
{
    BK_DUMP_OUT("******************** Dump mpu config begin ********************\r\n");
    BK_DUMP_OUT("MPU->TYPE: 0x%08X.\r\n", MPU->TYPE);
    BK_DUMP_OUT("MPU->CTRL: 0x%08X.\r\n", MPU->CTRL);
    for (uint32_t i = 0; i < 16; i++) {
        MPU->RNR = i;
        BK_DUMP_OUT("MPU->RNR: %d.\r\n", MPU->RNR);
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


