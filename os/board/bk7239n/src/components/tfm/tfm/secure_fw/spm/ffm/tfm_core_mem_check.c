/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>
#include <stdbool.h>
#include "region_defs.h"
#include "tfm_arch.h"
#include "tfm_spm_hal.h"
#include "tfm_api.h"
#include "spm_func.h"

/**
 * \brief Check whether the current partition has access to a memory range
 *
 * This function assumes, that the current MPU configuration is set for the
 * partition to be checked. The flags should contain information of the
 * execution mode of the partition code (priv/unpriv), and access type
 * (read/write) as specified in "ARMv8-M Security Extensions: Requirements on
 * Development Tools" chapter "Address range check intrinsic"
 *
 * \param[in] p      The start address of the range to check
 * \param[in] s      The size of the range to check
 * \param[in] flags  The flags to pass to the cmse_check_address_range func
 *
 * \return TFM_SUCCESS if the partition has access to the memory range,
 *         TFM_ERROR_GENERIC otherwise.
 */
static enum tfm_status_e has_access_to_region(const void *p, size_t s,
                                              int flags)
{
    int32_t range_access_allowed_by_mpu;

    /* Use the TT instruction to check access to the partition's regions*/
    range_access_allowed_by_mpu =
                          cmse_check_address_range((void *)p, s, flags) != NULL;

#if !defined(__SAUREGION_PRESENT) || (__SAUREGION_PRESENT == 0)
    if (!range_access_allowed_by_mpu) {
        /*
         * Verification failure may be due to address range crossing
         * one or multiple IDAU boundaries. In this case request a
         * platform-specific check for access permissions.
         */
        cmse_address_info_t addr_info_base = cmse_TT((void *)p);
        cmse_address_info_t addr_info_last = cmse_TT((void *)((uint32_t)p + s - 1));

        if ((addr_info_base.flags.idau_region_valid != 0) &&
            (addr_info_last.flags.idau_region_valid != 0) &&
            (addr_info_base.flags.idau_region != addr_info_last.flags.idau_region)) {
                range_access_allowed_by_mpu =
                    tfm_spm_hal_has_access_to_region(p, s, flags);
            }
    }
#endif

    if (range_access_allowed_by_mpu) {
        return TFM_SUCCESS;
    }

    return TFM_ERROR_GENERIC;
}

enum tfm_status_e tfm_core_has_read_access_to_region(const void *p, size_t s,
                                                     bool ns_caller,
                                                     uint32_t privileged)
{
    int flags = CMSE_MPU_READ;

    /* In case of NS caller, only force unprivileged check, if the non secure
     * Thread mode is unprivileged
     */
    if (ns_caller) {
        CONTROL_Type ctrl;

        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1) {
            privileged = TFM_PARTITION_UNPRIVILEGED_MODE;
        } else {
            privileged = TFM_PARTITION_PRIVILEGED_MODE;
        }
    }

    if (privileged == TFM_PARTITION_UNPRIVILEGED_MODE) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if (ns_caller) {
        flags |= CMSE_NONSECURE;
    }

    return has_access_to_region(p, s, flags);
}

enum tfm_status_e tfm_core_has_write_access_to_region(const void *p, size_t s,
                                                      bool ns_caller,
                                                      uint32_t privileged)
{
    int flags = CMSE_MPU_READWRITE;

    /* In case of NS caller, only force unprivileged check, if the non secure
     * Thread mode is unprivileged
     */
    if (ns_caller) {
        CONTROL_Type ctrl;

        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1) {
            privileged = TFM_PARTITION_UNPRIVILEGED_MODE;
        } else {
            privileged = TFM_PARTITION_PRIVILEGED_MODE;
        }
    }

    if (privileged == TFM_PARTITION_UNPRIVILEGED_MODE) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if (ns_caller) {
        flags |= CMSE_NONSECURE;
    }

    return has_access_to_region(p, s, flags);
}
