/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "array.h"
#include "tfm_boot_status.h"
#include "region_defs.h"
#include "tfm_memory_utils.h"
#include "tfm_api.h"
#include "tfm_core_utils.h"
#include "psa_manifest/pid.h"
#ifdef TFM_PSA_API
#include "internal_errors.h"
#include "utilities.h"
#include "psa/service.h"
#include "thread.h"
#include "tfm_spm_hal.h"
#include "spm_ipc.h"
#include "load/partition_defs.h"
#else
#include "spm_func.h"
#endif

/*!
 * \def BOOT_DATA_VALID
 *
 * \brief Indicates that shared data between bootloader and runtime firmware was
 *        passed the sanity check with success.
 */
#define BOOT_DATA_VALID (1u)

/*!
 * \def BOOT_DATA_INVALID
 *
 * \brief Indicates that shared data between bootloader and runtime firmware was
 *        failed on sanity check.
 */
#define BOOT_DATA_INVALID (0u)

/*!
 * \var is_boot_data_valid
 *
 * \brief Indicates the status of shared data between bootloader and runtime
 *        firmware
 */
static uint32_t is_boot_data_valid = BOOT_DATA_INVALID;

/*!
 * \struct boot_data_access_policy
 *
 * \brief Defines the access policy of secure partitions to data items in shared
 *        data area (between bootloader and runtime firmware).
 */
struct boot_data_access_policy {
    int32_t partition_id;
    uint32_t major_type;
};

/*!
 * \var access_policy_table
 *
 * \brief Contains the partition_id and major_type assignments. This describes
 *        which secure partition is allowed to access which data item
 *        (identified by major_type).
 */
static const struct boot_data_access_policy access_policy_table[] = {
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    {TFM_SP_INITIAL_ATTESTATION, TLV_MAJOR_IAS},
#endif
#ifdef TFM_PARTITION_FIRMWARE_UPDATE
    {TFM_SP_FWU, TLV_MAJOR_FWU},
#endif
};

/*!
 * \brief Verify the access right of the active secure partition to the
 *        specified data type in the shared data area.
 *
 * \param[in]  major_type  Data type identifier.
 *
 * \return  Returns 0 in case of success, otherwise -1.
 */
static int32_t tfm_core_check_boot_data_access_policy(uint8_t major_type)
{
    int32_t partition_id;
    uint32_t i;
    int32_t rc = -1;
    const uint32_t array_size = ARRAY_SIZE(access_policy_table);

#ifndef TFM_PSA_API
    uint32_t partition_idx = tfm_spm_partition_get_running_partition_idx();

    partition_id = tfm_spm_partition_get_partition_id(partition_idx);
#else
    partition_id = tfm_spm_partition_get_running_partition_id();
#endif

    for (i = 0; i < array_size; ++i) {
        if (partition_id == access_policy_table[i].partition_id) {
            if (major_type == access_policy_table[i].major_type) {
                rc = 0;
                break;
            }
        }
    }

    return rc;
}

/* Compile time check to verify that shared data region is not overlapping with
 * non-secure data area.
 */
#if ((BOOT_TFM_SHARED_DATA_BASE  >= NS_DATA_START && \
      BOOT_TFM_SHARED_DATA_BASE  <= NS_DATA_LIMIT) || \
     (BOOT_TFM_SHARED_DATA_LIMIT >= NS_DATA_START && \
      BOOT_TFM_SHARED_DATA_LIMIT <= NS_DATA_LIMIT))
#error "Shared data area and non-secure data area is overlapping"
#endif

void tfm_core_validate_boot_data(void)
{
#ifdef BOOT_DATA_AVAILABLE
    struct tfm_boot_data *boot_data;

    boot_data = (struct tfm_boot_data *)BOOT_TFM_SHARED_DATA_BASE;

    if (boot_data->header.tlv_magic == SHARED_DATA_TLV_INFO_MAGIC) {
        is_boot_data_valid = BOOT_DATA_VALID;
    }
#else
    is_boot_data_valid = BOOT_DATA_VALID;
#endif /* BOOT_DATA_AVAILABLE */
}

void tfm_core_get_boot_data_handler(uint32_t args[])
{
    uint8_t  tlv_major = (uint8_t)args[0];
    uint8_t *buf_start = (uint8_t *)args[1];
    uint16_t buf_size  = (uint16_t)args[2];
    struct tfm_boot_data *boot_data;
#ifdef BOOT_DATA_AVAILABLE
    uint8_t *ptr;
    struct shared_data_tlv_entry tlv_entry = {0};
    uintptr_t tlv_end, offset;
    size_t next_tlv_offset;
#endif /* BOOT_DATA_AVAILABLE */
#ifndef TFM_PSA_API
    uint32_t running_partition_idx =
                tfm_spm_partition_get_running_partition_idx();
    uint32_t res;
#else
    uint32_t privileged;
#endif

#ifndef TFM_PSA_API
    /*
     * Make sure that the output pointer points to a memory area that is owned
     * by the partition. And check 4 bytes alignment.
     */
    res = tfm_spm_check_buffer_access(running_partition_idx,
                                      (void *)buf_start,
                                      buf_size,
                                      2);
    if (res != TFM_SUCCESS) {
        /* Not in accessible range, return error */
        args[0] = (uint32_t)res;
        return;
    }
#else
    privileged = GET_CURRENT_PARTITION_PRIVILEGED_MODE();

    if (tfm_memory_check(buf_start, buf_size, false, TFM_MEMORY_ACCESS_RW,
        privileged) != SPM_SUCCESS) {
        /* Not in accessible range, return error */
        args[0] = (uint32_t)TFM_ERROR_INVALID_PARAMETER;
        return;
    }
#endif

    if (is_boot_data_valid != BOOT_DATA_VALID) {
        args[0] = (uint32_t)TFM_ERROR_INVALID_PARAMETER;
        return;
    }

    /* Check whether caller has access right to given tlv_major_type */
    if (tfm_core_check_boot_data_access_policy(tlv_major)) {
        args[0] = (uint32_t)TFM_ERROR_INVALID_PARAMETER;
        return;
    }

#ifdef BOOT_DATA_AVAILABLE
    /* Get the boundaries of TLV section */
    boot_data = (struct tfm_boot_data *)BOOT_TFM_SHARED_DATA_BASE;
    tlv_end = BOOT_TFM_SHARED_DATA_BASE + boot_data->header.tlv_tot_len;
    offset  = BOOT_TFM_SHARED_DATA_BASE + SHARED_DATA_HEADER_SIZE;
#endif /* BOOT_DATA_AVAILABLE */

    /* Add header to output buffer as well */
    if (buf_size < SHARED_DATA_HEADER_SIZE) {
        args[0] = (uint32_t)TFM_ERROR_INVALID_PARAMETER;
        return;
    } else {
        boot_data = (struct tfm_boot_data *)buf_start;
        boot_data->header.tlv_magic   = SHARED_DATA_TLV_INFO_MAGIC;
        boot_data->header.tlv_tot_len = SHARED_DATA_HEADER_SIZE;
    }

#ifdef BOOT_DATA_AVAILABLE
    ptr = boot_data->data;
    /* Iterates over the TLV section and copy TLVs with requested major
     * type to the provided buffer.
     */
    for (; offset < tlv_end; offset += next_tlv_offset) {
        /* Create local copy to avoid unaligned access */
        (void)spm_memcpy(&tlv_entry, (const void *)offset,
                         SHARED_DATA_ENTRY_HEADER_SIZE);

        next_tlv_offset = SHARED_DATA_ENTRY_HEADER_SIZE + tlv_entry.tlv_len;

        if (GET_MAJOR(tlv_entry.tlv_type) == tlv_major) {
            /* Check buffer overflow */
            if (((ptr - buf_start) + next_tlv_offset) > buf_size) {
                args[0] = (uint32_t)TFM_ERROR_INVALID_PARAMETER;
                return;
            }

            (void)spm_memcpy(ptr, (const void *)offset, next_tlv_offset);
            ptr += next_tlv_offset;
            boot_data->header.tlv_tot_len += next_tlv_offset;
        }
    }
#endif /* BOOT_DATA_AVAILABLE */

    args[0] = (uint32_t)TFM_SUCCESS;
    return;
}
