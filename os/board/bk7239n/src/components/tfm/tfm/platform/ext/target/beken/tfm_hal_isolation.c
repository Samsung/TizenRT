/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "array.h"
#include "cmsis.h"
#include "Driver_Common.h"
#include "mmio_defs.h"
#include "mpu_armv8m_drv.h"
#include "region.h"
#include "target_cfg.h"
#include "tfm_hal_isolation.h"
#include "tfm_peripherals_def.h"
#include "tfm_core_utils.h"

#ifdef TFM_PSA_API
#include "load/partition_defs.h"
#include "load/asset_defs.h"
#include "load/spm_load_api.h"
#endif

/* It can be retrieved from the MPU_TYPE register. */
#define MPU_REGION_NUM                  16

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
static uint32_t n_configured_regions = 0;
struct mpu_armv8m_dev_t dev_mpu_s = {MPU_BASE};

#ifdef CONFIG_TFM_PARTITION_META
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
#endif /* CONFIG_TFM_PARTITION_META */

#if TFM_LVL == 3
static uint32_t idx_boundary_handle = 0;
REGION_DECLARE(Image$$, PT_RO_START, $$Base);
REGION_DECLARE(Image$$, PT_RO_END, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_END, $$Base);

const static struct mpu_armv8m_region_cfg_t isolation_regions[] = {
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_RO_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_RO_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE,
    },
    /* For isolation Level 3, set up static isolation for privileged data.
     * Unprivileged data is dynamically set during Partition scheduling.
     */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_ONLY,
        MPU_ARMV8M_SH_NONE,
    },
#ifdef CONFIG_TFM_PARTITION_META
    /* TFM partition metadata pointer region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    }
#endif
};
#else /* TFM_LVL == 3 */

REGION_DECLARE(Image$$, ER_VENEER, $$Base);
REGION_DECLARE(Image$$, VENEER_ALIGN, $$Limit);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, ER_TFM_CODE_START, $$Base);
REGION_DECLARE(Image$$, ER_TFM_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);
REGION_DECLARE(Image$$, ER_TFM_DATA, $$ZI$$Base);
REGION_DECLARE(Image$$, ER_TFM_DATA, $$ZI$$Limit);
REGION_DECLARE(Image$$, ER_TFM_DATA, $$RW$$Base);
REGION_DECLARE(Image$$, ER_TFM_DATA, $$RW$$Limit);

const struct mpu_armv8m_region_cfg_t region_cfg[] = {
    /* Veneer region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* TFM Core unprivileged code region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* RO region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
	#if CONFIG_MPU_REGION_UPDATED
    /* RO region, AROT/unprivilege mode mpu issue */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, ER_TFM_CODE_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, ER_TFM_CODE_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
	#endif
    /* RW, ZI and stack as one region ------------------------------------------*/
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base),
        //(uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
 
 	#if CONFIG_MPU_REGION_UPDATED
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base),
        (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_ONLY,
        MPU_ARMV8M_SH_INNER
    },
    {
        0, /* will be updated before using  ------------------------------------------*/
        (uint32_t)&REGION_NAME(Image$$, ER_TFM_DATA, $$RW$$Base),
        (uint32_t)&REGION_NAME(Image$$, ER_TFM_DATA, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_INNER
    },
	#else
#if 0//def CONFIG_TFM_PARTITION_META
    /* TFM partition metadata pointer region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_INNER
    },
#endif
#endif
};
#endif /* TFM_LVL == 3 */
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

void flush_all_dcache(void);
void disable_scb_dcache(void);
void enable_scb_dcache(void);

enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
{
    /* Set up isolation boundaries between SPE and NSPE */
    sau_and_idau_cfg(); //Never failed
    if (mpc_init_cfg() != ARM_DRIVER_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    /* Set up static isolation boundaries inside SPE */
#if CONFIG_TFM_MPU
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
    struct mpu_armv8m_region_cfg_t localcfg;
    int32_t i;
	
	flush_all_dcache(); //Never failed
	disable_scb_dcache(); //Never failed
    mpu_armv8m_clean(&dev_mpu_s); //Never failed

#if TFM_LVL == 3
    /*
     * Update MPU region numbers. The numbers start from 0 and are continuous.
     * Under isolation level3, at lease one MPU region is reserved for private
     * data asset.
     */
    if (ARRAY_SIZE(isolation_regions) >= MPU_REGION_NUM) {
        return TFM_HAL_ERROR_GENERIC;
    }
    for (i = 0; i < ARRAY_SIZE(isolation_regions); i++) {
        spm_memcpy(&localcfg, &isolation_regions[i], sizeof(localcfg));
        /* Update region number */
        localcfg.region_nr = i;
        /* Enable regions */
        if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg) != MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }
    n_configured_regions = i;
#else /* TFM_LVL == 3 */
    if (ARRAY_SIZE(region_cfg) > MPU_REGION_NUM) {
        return TFM_HAL_ERROR_GENERIC;
    }
    for (i = 0; i < ARRAY_SIZE(region_cfg); i++) {
        spm_memcpy(&localcfg, &region_cfg[i], sizeof(localcfg));
        localcfg.region_nr = i;
        if (mpu_armv8m_region_enable(&dev_mpu_s,
            (struct mpu_armv8m_region_cfg_t *)&localcfg)
            != MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }
    n_configured_regions = i;
#endif /* TFM_LVL == 3 */

    /* Enable MPU */
    if (mpu_armv8m_enable(&dev_mpu_s,
                          PRIVILEGED_DEFAULT_ENABLE,
                          HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }
	enable_scb_dcache(); //Never failed
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */
#endif /* CONFIG_TFM_MPU */

    return TFM_HAL_SUCCESS;
}

#ifdef TFM_PSA_API
/*
 * Implementation of tfm_hal_bind_boundaries() on BK7236:
 *
 * The API encodes some attributes into a handle and returns it to SPM.
 * The attributes include isolation boundaries, privilege, and mmio information.
 * When scheduler switches running partitions, SPM compares the handle between
 * partitions to know if boundary update is necessary. If update is required,
 * SPM passes the handle to platform to do platform settings and update
 * isolation boundaries.
 *
 * The handle should be unique under isolation level 3. The implementation
 * encodes an index at the highest 8 bits to assure handle uniqueness. While
 * under isolation level 1/2, handles may not be unique.
 *
 * The encoding format assignment:
 * - For isolation level 3
 *      BIT | 31        24 | 23         20 | ... | 7           4 | 3        0 |
 *          | Unique Index | Region Attr 5 | ... | Region Attr 1 | Privileged |
 *
 *      In which the "Region Attr i" is:
 *      BIT |       3      | 2        0 |
 *          | 1: RW, 0: RO | MMIO Index |
 *
 * - For isolation level 1/2
 *      BIT | 31                           0 |
 *          | 1: privileged, 0: unprivileged |
 *
 * This is a reference implementation on BK7236, and may have some limitations.
 * 1. The maximum number of allowed MMIO regions is 5.
 * 2. Highest 8 bits are for index. It supports 256 unique handles at most.
 */
enum tfm_hal_status_t tfm_hal_bind_boundaries(
                                    const struct partition_load_info_t *p_ldinf,
                                    void **pp_boundaries)
{
    uint32_t i, j;
    bool privileged;
    const struct asset_desc_t *p_asset;
    struct platform_data_t *plat_data_ptr;
#if TFM_LVL == 2
    struct mpu_armv8m_region_cfg_t localcfg;
#elif TFM_LVL == 3
    uint32_t partition_attrs = 0;
#endif

    if (!p_ldinf || !pp_boundaries) {
        return TFM_HAL_ERROR_GENERIC;
    }

#if TFM_LVL == 1
    privileged = true;
#else
    privileged = IS_PARTITION_PSA_ROT(p_ldinf);
#endif

    p_asset = (const struct asset_desc_t *)LOAD_INFO_ASSET(p_ldinf);

    /*
     * Validate if the named MMIO of partition is allowed by the platform.
     * Otherwise, skip validation.
     *
     * NOTE: Need to add validation of numbered MMIO if platform requires.
     */
    for (i = 0; i < p_ldinf->nassets; i++) {
        if (!(p_asset[i].attr & ASSET_ATTR_NAMED_MMIO)) {
            continue;
        }
        for (j = 0; j < ARRAY_SIZE(partition_named_mmio_list); j++) {
            if (p_asset[i].dev.dev_ref == partition_named_mmio_list[j]) {
                break;
            }
        }

        if (j == ARRAY_SIZE(partition_named_mmio_list)) {
            /* The MMIO asset is not in the allowed list of platform. */
            return TFM_HAL_ERROR_GENERIC;
        }
        /* Assume PPC & MPC settings are required even under level 1 */
        plat_data_ptr = REFERENCE_TO_PTR(p_asset[i].dev.dev_ref,
                                         struct platform_data_t *);
        //TODO peter can add bk7236 configure here
#if TFM_LVL == 2
        /*
         * Static boundaries are set. Set up MPU region for MMIO.
         * Setup regions for unprivileged assets only.
         */
        if (!privileged) {
            localcfg.region_base = plat_data_ptr->periph_start;
            localcfg.region_limit = plat_data_ptr->periph_limit;
            localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
            localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
            localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
            localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
            localcfg.region_nr = n_configured_regions ++;

            if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg)
                != MPU_ARMV8M_OK) {
                return TFM_HAL_ERROR_GENERIC;
            }
        }
#elif TFM_LVL == 3
        /* Encode MMIO attributes into the "partition_attrs". */
        partition_attrs <<= HANDLE_PER_ATTR_BITS;
        partition_attrs |= ((j + 1) & HANDLE_ATTR_INDEX_MASK);
        if (p_asset[i].attr & ASSET_ATTR_READ_WRITE) {
            partition_attrs |= HANDLE_ATTR_RW_POS;
        }
#endif
    }

#if TFM_LVL == 3
    partition_attrs <<= HANDLE_PER_ATTR_BITS;
    partition_attrs |= ((uint8_t)privileged) & HANDLE_ATTR_PRIV_MASK;
    /*
     * Highest 8 bits are reserved for index, if they are non-zero, MMIO numbers
     * must have exceeded the limit of 5.
     */
    if (partition_attrs & HANDLE_INDEX_MASK) {
        return TFM_HAL_ERROR_GENERIC;
    }
    HANDLE_ENCODE_INDEX(partition_attrs, idx_boundary_handle);
    *pp_boundaries = (void *)partition_attrs;
#else
    *pp_boundaries = (void *)(((uint32_t)privileged) & HANDLE_ATTR_PRIV_MASK);
#endif

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_update_boundaries(
                             const struct partition_load_info_t *p_ldinf,
                             void *p_boundaries)
{
    CONTROL_Type ctrl;
    uint32_t local_handle = (uint32_t)p_boundaries;
    bool privileged = !!(local_handle & HANDLE_ATTR_PRIV_MASK);
#if TFM_LVL == 3
    struct mpu_armv8m_region_cfg_t localcfg;
    uint32_t i, mmio_index;
    struct platform_data_t *plat_data_ptr;
    struct asset_desc_t *rt_mem;
#endif

    /* Privileged level is required to be set always */
    ctrl.w = __get_CONTROL();
    ctrl.b.nPRIV = privileged ? 0 : 1;
    __set_CONTROL(ctrl.w);

#if TFM_LVL == 3
    if (!p_ldinf) {
        return TFM_HAL_ERROR_GENERIC;
    }

    /* Update regions, for unprivileged partitions only */
    if (privileged) {
        return TFM_HAL_SUCCESS;
    }

    /* Setup runtime memory first */
    localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
    localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    rt_mem = (struct asset_desc_t *)LOAD_INFO_ASSET(p_ldinf);
    /*
     * BK7236 shortcut: The first item is the only runtime memory asset.
     * Platforms with many memory assets please check this part.
     */
    for (i = 0;
         i < p_ldinf->nassets && !(rt_mem[i].attr & ASSET_ATTR_MMIO);
         i++) {
        localcfg.region_nr = n_configured_regions + i;
        localcfg.region_base = rt_mem[i].mem.start;
        localcfg.region_limit = rt_mem[i].mem.limit;

        if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg) != MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }

    /* Named MMIO part */
    local_handle = local_handle & (~HANDLE_INDEX_MASK);
    local_handle >>= HANDLE_PER_ATTR_BITS;
    mmio_index = local_handle & HANDLE_ATTR_INDEX_MASK;

    localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;

    i = n_configured_regions + i;
    while (mmio_index && i < MPU_REGION_NUM) {
        plat_data_ptr =
          (struct platform_data_t *)partition_named_mmio_list[mmio_index - 1];
        localcfg.region_nr = i++;
        localcfg.attr_access = (local_handle & HANDLE_ATTR_RW_POS)?
                            MPU_ARMV8M_AP_RW_PRIV_UNPRIV :
                            MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
        localcfg.region_base = plat_data_ptr->periph_start;
        localcfg.region_limit = plat_data_ptr->periph_limit;

        if (mpu_armv8m_region_enable(&dev_mpu_s, &localcfg) != MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }

        local_handle >>= HANDLE_PER_ATTR_BITS;
        mmio_index = local_handle & HANDLE_ATTR_INDEX_MASK;
    }

    /* Disable unused regions */
    while (i < MPU_REGION_NUM) {
        if (mpu_armv8m_region_disable(&dev_mpu_s, i++)!= MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }
#endif
    return TFM_HAL_SUCCESS;
}
#endif /* TFM_PSA_API */
