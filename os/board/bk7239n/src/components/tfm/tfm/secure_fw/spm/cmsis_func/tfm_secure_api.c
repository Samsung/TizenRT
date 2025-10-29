/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "spm_func.h"
#include "tfm_secure_api.h"

void tfm_secure_api_error_handler(void)
{
    SPMLOG_ERRMSG("Security violation when calling secure API\r\n");
    tfm_core_panic();
}

int32_t tfm_core_partition_request(uint32_t id, bool is_ns, void *fn,
            int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4)
{
    int32_t args[4] = {arg1, arg2, arg3, arg4};
    struct tfm_sfn_req_s desc, *desc_ptr = &desc;

    desc.sp_id = id;
    desc.sfn = (sfn_t) fn;
    desc.args = args;
    desc.ns_caller = is_ns;

    if (__get_active_exc_num() != EXC_NUM_THREAD_MODE) {
        /* The veneer of a secure service had been called from Handler mode.
         * This violates TF-M's programming model, and is considered an
         * unrecoverable error.
         */
        tfm_core_panic();
    } else {
        if (desc.ns_caller) {
            return tfm_core_sfn_request(desc_ptr);
        } else {
            return tfm_spm_sfn_request_thread_mode(desc_ptr);
        }
    }
    return TFM_ERROR_GENERIC;
}

enum tfm_status_e tfm_core_get_caller_client_id(int32_t *caller_client_id)
{
    if (__get_active_exc_num() != EXC_NUM_THREAD_MODE) {
        /* This API is available only in Thread mode and when called
         * from Handler mode is considered a violation of the
         * programming model.
         */
        tfm_core_panic();
    }

    const uint32_t running_partition_idx =
            tfm_spm_partition_get_running_partition_idx();
    const uint32_t running_partition_flags =
            tfm_spm_partition_get_flags(running_partition_idx);
    const struct spm_partition_runtime_data_t *curr_part_data =
            tfm_spm_partition_get_runtime_data(running_partition_idx);

    if (!(running_partition_flags & SPM_PART_FLAG_APP_ROT) ||
        curr_part_data->partition_state == SPM_PARTITION_STATE_HANDLING_IRQ ||
        curr_part_data->partition_state == SPM_PARTITION_STATE_SUSPENDED) {
        /* This API shouldn't be called from outside partition context.
         * Also if the current partition is handling IRQ, the caller partition
         * index might not be valid;
         * Partitions are only allowed to run while S domain is locked.
         */
        return TFM_ERROR_INVALID_PARAMETER;
    }

    int32_t res = tfm_spm_check_buffer_access(running_partition_idx,
                                      (void *)caller_client_id,
                                      sizeof(int32_t),
                                      2);
    if (res != TFM_SUCCESS) {
        return res;
    }

    *caller_client_id = curr_part_data->caller_client_id;

    return TFM_SUCCESS;
}
