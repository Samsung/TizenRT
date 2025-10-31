/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_protected_storage.h"
#include "ps_object_system.h"
#include "tfm_ps_defs.h"

psa_status_t tfm_ps_init(void)
{
    psa_status_t err;

    err = ps_system_prepare();
#ifdef PS_CREATE_FLASH_LAYOUT
    /* If PS_CREATE_FLASH_LAYOUT is set, it indicates that it is required to
     * create a PS flash layout. PS service will generate an empty and valid
     * PS flash layout to store assets. It will erase all data located in the
     * assigned PS memory area before generating the PS layout.
     * This flag is required to be set if the PS memory area is located in
     * non-persistent memory.
     * This flag can be set if the PS memory area is located in persistent
     * memory without a previous valid PS flash layout in it. That is the case
     * when it is the first time in the device life that the PS service is
     * executed.
     */
    if (err != PSA_SUCCESS) {
        /* Remove all data in the PS memory area and create a valid PS flash
         * layout in that area.
         */
        err = ps_system_wipe_all();
        if (err != PSA_SUCCESS) {
            return err;
        }

        /* Attempt to initialise again */
        err = ps_system_prepare();
    }
#endif /* PS_CREATE_FLASH_LAYOUT */

    return err;
}

psa_status_t tfm_ps_set(int32_t client_id,
                        psa_storage_uid_t uid,
                        uint32_t data_length,
                        psa_storage_create_flags_t create_flags)
{
    /* Check that the UID is valid */
    if (uid == TFM_PS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Check that the create_flags does not contain any unsupported flags */
    if (create_flags & ~(PSA_STORAGE_FLAG_WRITE_ONCE |
                         PSA_STORAGE_FLAG_NO_CONFIDENTIALITY |
                         PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    /* Create the object in the object system */
    return ps_object_create(uid, client_id, create_flags, data_length);
}

psa_status_t tfm_ps_get(int32_t client_id,
                        psa_storage_uid_t uid,
                        uint32_t data_offset,
                        uint32_t data_size,
                        size_t *p_data_length)
{
    /* Check that the UID is valid */
    if (uid == TFM_PS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Read the object data from the object system */
    return ps_object_read(uid, client_id, data_offset, data_size,
                          p_data_length);
}

psa_status_t tfm_ps_get_info(int32_t client_id, psa_storage_uid_t uid,
                             struct psa_storage_info_t *p_info)
{
    /* Check that the UID is valid */
    if (uid == TFM_PS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Get the info struct data from the object system */
    return ps_object_get_info(uid, client_id, p_info);
}

psa_status_t tfm_ps_remove(int32_t client_id, psa_storage_uid_t uid)
{
    psa_status_t err;

    /* Check that the UID is valid */
    if (uid == TFM_PS_INVALID_UID) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* Delete the object from the object system */
    err = ps_object_delete(uid, client_id);

    /* PSA_ERROR_INVALID_SIGNATURE is not supported by psa_ps_remove
     * specification. So, this function returns TFM_PS_ERR_OPERATION_FAILED
     * instead.
     */
    if (err == PSA_ERROR_INVALID_SIGNATURE) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return err;
}

uint32_t tfm_ps_get_support(void)
{
    /*
     * This function returns a bitmask with flags set for all of the optional
     * features supported by the PS service implementation.
     *
     * PS service does not support the optional extended PSA PS API yet.
     */

    return 0;
}
