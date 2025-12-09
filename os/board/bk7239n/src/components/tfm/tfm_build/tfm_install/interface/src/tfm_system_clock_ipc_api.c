/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/client.h"
#include "psa_manifest/sid.h"
#include "tfm_api.h"
#include "tfm_system_clock_api.h"

psa_status_t psa_system_power_ctrl(uint32_t module_id, uint32_t power_state)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &module_id, .len = sizeof(module_id) },
        { .base = &power_state, .len = sizeof(power_state) },
    };

    handle = psa_connect(TFM_SYSTEM_POWER_CTRL_SID, TFM_SYSTEM_POWER_CTRL_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), NULL, 0);

    psa_close(handle);
    if (status == (psa_status_t)TFM_ERROR_INVALID_PARAMETER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}

psa_status_t psa_system_clock_set(uint32_t module_id, uint32_t clk_src, uint32_t clk_div)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &module_id, .len = sizeof(module_id) },
        { .base = &clk_src, .len = sizeof(clk_src) },
        { .base = &clk_div, .len = sizeof(clk_div) },
    };

    handle = psa_connect(TFM_SYSTEM_CLOCK_SET_SID, TFM_SYSTEM_CLOCK_SET_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), NULL, 0);

    psa_close(handle);
    if (status == (psa_status_t)TFM_ERROR_INVALID_PARAMETER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}

psa_status_t psa_system_core_set(uint32_t system_operation_id)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &system_operation_id, .len = sizeof(system_operation_id) },
    };

    handle = psa_connect(TFM_SYSTEM_CORE_SET_SID, TFM_SYSTEM_CORE_SET_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), NULL, 0);

    psa_close(handle);
    if (status == (psa_status_t)TFM_ERROR_INVALID_PARAMETER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}