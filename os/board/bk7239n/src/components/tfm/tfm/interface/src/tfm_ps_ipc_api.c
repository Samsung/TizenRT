/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/client.h"
#include "psa/protected_storage.h"
#include "psa_manifest/sid.h"
#include "tfm_ns_interface.h"
#include "tfm_ps_defs.h"

psa_status_t psa_ps_set(psa_storage_uid_t uid,
                        size_t data_length,
                        const void *p_data,
                        psa_storage_create_flags_t create_flags)
{
    psa_status_t status;

    psa_invec in_vec[] = {
        { .base = &uid,   .len = sizeof(uid) },
        { .base = p_data, .len = data_length },
        { .base = &create_flags, .len = sizeof(create_flags) }
    };

    status = psa_call(TFM_PROTECTED_STORAGE_SERVICE_HANDLE, TFM_PS_SET, in_vec,
                      IOVEC_LEN(in_vec), NULL, 0);

    return status;
}

psa_status_t psa_ps_get(psa_storage_uid_t uid,
                        size_t data_offset,
                        size_t data_size,
                        void *p_data,
                        size_t *p_data_length)
{
    psa_status_t status;

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) },
        { .base = &data_offset, .len = sizeof(data_offset) }
    };

    psa_outvec out_vec[] = {
        { .base = p_data, .len = data_size }
    };

    if (p_data_length == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    status = psa_call(TFM_PROTECTED_STORAGE_SERVICE_HANDLE, TFM_PS_GET, in_vec,
                      IOVEC_LEN(in_vec), out_vec, IOVEC_LEN(out_vec));

    *p_data_length = out_vec[0].len;

    return status;
}

psa_status_t psa_ps_get_info(psa_storage_uid_t uid,
                             struct psa_storage_info_t *p_info)
{
    psa_status_t status;

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) }
    };

    psa_outvec out_vec[] = {
        { .base = p_info, .len = sizeof(*p_info) }
    };

    status = psa_call(TFM_PROTECTED_STORAGE_SERVICE_HANDLE, TFM_PS_GET_INFO,
                      in_vec, IOVEC_LEN(in_vec), out_vec, IOVEC_LEN(out_vec));

    return status;
}

psa_status_t psa_ps_remove(psa_storage_uid_t uid)
{
    psa_status_t status;

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) }
    };

    status = psa_call(TFM_PROTECTED_STORAGE_SERVICE_HANDLE, TFM_PS_REMOVE,
                      in_vec, IOVEC_LEN(in_vec), NULL, 0);

    return status;
}

psa_status_t psa_ps_create(psa_storage_uid_t uid, size_t size,
                           psa_storage_create_flags_t create_flags)
{
    (void)uid;
    (void)size;
    (void)create_flags;

    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_ps_set_extended(psa_storage_uid_t uid, size_t data_offset,
                                 size_t data_length, const void *p_data)
{
    (void)uid;
    (void)data_offset;
    (void)data_length;
    (void)p_data;

    return PSA_ERROR_NOT_SUPPORTED;
}

uint32_t psa_ps_get_support(void)
{
    /* Initialise support_flags to a sensible default, to avoid returning an
     * uninitialised value in case the secure function fails.
     */
    uint32_t support_flags = 0;

    psa_outvec out_vec[] = {
        { .base = &support_flags, .len = sizeof(support_flags) }
    };

    /* The PSA API does not return an error, so any error from TF-M is
     * ignored.
     */
    (void)psa_call(TFM_PROTECTED_STORAGE_SERVICE_HANDLE, TFM_PS_GET_SUPPORT,
                   NULL, 0, out_vec, IOVEC_LEN(out_vec));

    return support_flags;
}
