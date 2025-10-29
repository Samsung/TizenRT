/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* NOTE: For the security of the protected storage system, the bootloader
 * rollback protection, and the protection of cryptographic material  it is
 * CRITICAL to use a internal (in-die) persistent memory for the implementation
 * of the OTP_NV_COUNTERS flash area (see flash_otp_nv_layout.c).
 */

#include "tfm_plat_otp.h"

#include "flash_layout.h"
#include "flash_otp_nv_counters_backend.h"
#include <string.h>
#include <stddef.h>

enum tfm_plat_err_t tfm_plat_otp_init(void)
{
    return init_otp_nv_counters_flash();
}

static enum tfm_plat_err_t write_to_output(enum tfm_otp_element_id_t id,
                                        uint32_t offset, size_t out_len,
                                        uint8_t *out)
{
    enum tfm_plat_err_t err = TFM_PLAT_ERR_SUCCESS;
    size_t value_size;
    size_t copy_size;

    err = tfm_plat_otp_get_size(id, &value_size);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    copy_size = out_len < value_size ? out_len : value_size;

    err = read_otp_nv_counters_flash(offset, out, copy_size);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_otp_read(enum tfm_otp_element_id_t id,
                                      size_t out_len, uint8_t *out)
{
    switch (id) {
        case PLAT_OTP_ID_HUK:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, huk), out_len, out);
        case PLAT_OTP_ID_IAK:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, iak), out_len, out);
        case PLAT_OTP_ID_IAK_LEN:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, iak_len), out_len, out);
        case PLAT_OTP_ID_IAK_TYPE:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, iak_type), out_len, out);
        case PLAT_OTP_ID_IAK_ID:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, iak_id), out_len, out);

        case PLAT_OTP_ID_BOOT_SEED:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, boot_seed), out_len, out);
        case PLAT_OTP_ID_LCS:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, lcs), out_len, out);
        case PLAT_OTP_ID_IMPLEMENTATION_ID:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, implementation_id), out_len, out);
        case PLAT_OTP_ID_HW_VERSION:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, hw_version), out_len, out);
        case PLAT_OTP_ID_VERIFICATION_SERVICE_URL:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, verification_service_url), out_len, out);
        case PLAT_OTP_ID_PROFILE_DEFINITION:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, profile_definition), out_len, out);

#ifdef BL2
        case PLAT_OTP_ID_BL2_ROTPK_0:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_rotpk_0), out_len, out);
        case PLAT_OTP_ID_BL2_ROTPK_1:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_rotpk_1), out_len, out);
        case PLAT_OTP_ID_BL2_ROTPK_2:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_rotpk_2), out_len, out);

        case PLAT_OTP_ID_NV_COUNTER_BL2_0:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_nv_counter_0), out_len, out);
        case PLAT_OTP_ID_NV_COUNTER_BL2_1:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_nv_counter_1), out_len, out);
        case PLAT_OTP_ID_NV_COUNTER_BL2_2:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_nv_counter_2), out_len, out);
#endif /* BL2 */

#ifdef BL1
        case PLAT_OTP_ID_BL1_ROTPK_0:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl1_rotpk_0), out_len, out);

        case PLAT_OTP_ID_NV_COUNTER_BL1_0:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, bl1_nv_counter_0), out_len, out);
#endif /* BL1 */

        case PLAT_OTP_ID_ENTROPY_SEED:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, entropy_seed), out_len, out);

        case PLAT_OTP_ID_SECURE_DEBUG_PK:
            return write_to_output(id, offsetof(struct flash_otp_nv_counters_region_t, secure_debug_pk), out_len, out);

        default:
            return TFM_PLAT_ERR_UNSUPPORTED;
    }
}

#if defined(OTP_WRITEABLE)
static enum tfm_plat_err_t read_from_input(enum tfm_otp_element_id_t id,
                                      uint32_t offset, size_t in_len,
                                      const uint8_t *in)
{
    enum tfm_plat_err_t err = TFM_PLAT_ERR_SUCCESS;
    size_t value_size;
    uint8_t buffer[in_len];
    size_t idx;

    err = tfm_plat_otp_get_size(id, &value_size);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    if (in_len > value_size) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = read_otp_nv_counters_flash(offset, buffer, in_len);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    for (idx = 0; idx < in_len; idx++) {
        if ((buffer[idx] | in[idx]) != in[idx]) {
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        buffer[idx] |= in[idx];
    }

    err = write_otp_nv_counters_flash(offset, buffer, in_len);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_otp_write(enum tfm_otp_element_id_t id,
                                       size_t in_len, const uint8_t *in)
{
    switch (id) {
    case PLAT_OTP_ID_HUK:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, huk), in_len, in);
    case PLAT_OTP_ID_IAK:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, iak), in_len, in);
    case PLAT_OTP_ID_IAK_LEN:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, iak_len), in_len, in);
    case PLAT_OTP_ID_IAK_TYPE:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, iak_type), in_len, in);
    case PLAT_OTP_ID_IAK_ID:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, iak_id), in_len, in);

    case PLAT_OTP_ID_BOOT_SEED:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, boot_seed), in_len, in);
    case PLAT_OTP_ID_LCS:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, lcs), in_len, in);
    case PLAT_OTP_ID_IMPLEMENTATION_ID:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, implementation_id), in_len, in);
    case PLAT_OTP_ID_HW_VERSION:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, hw_version), in_len, in);
    case PLAT_OTP_ID_VERIFICATION_SERVICE_URL:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, verification_service_url), in_len, in);
    case PLAT_OTP_ID_PROFILE_DEFINITION:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, profile_definition), in_len, in);

#ifdef BL2
    case PLAT_OTP_ID_BL2_ROTPK_0:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_rotpk_0), in_len, in);
    case PLAT_OTP_ID_BL2_ROTPK_1:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_rotpk_1), in_len, in);
    case PLAT_OTP_ID_BL2_ROTPK_2:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_rotpk_2), in_len, in);

    case PLAT_OTP_ID_NV_COUNTER_BL2_0:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_nv_counter_0), in_len, in);
    case PLAT_OTP_ID_NV_COUNTER_BL2_1:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_nv_counter_1), in_len, in);
    case PLAT_OTP_ID_NV_COUNTER_BL2_2:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl2_nv_counter_2), in_len, in);
#endif /* Bl2 */

#ifdef BL1
    case PLAT_OTP_ID_BL1_ROTPK_0:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl1_rotpk_0), in_len, in);

    case PLAT_OTP_ID_NV_COUNTER_BL1_0:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, bl1_nv_counter_0), in_len, in);
#endif /* BL1 */

    case PLAT_OTP_ID_ENTROPY_SEED:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, entropy_seed), in_len, in);

    case PLAT_OTP_ID_SECURE_DEBUG_PK:
        return read_from_input(id, offsetof(struct flash_otp_nv_counters_region_t, secure_debug_pk), in_len, in);

    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }
}
#else
enum tfm_plat_err_t tfm_plat_otp_write(enum tfm_otp_element_id_t id,
                                       size_t in_len, const uint8_t *in)
{
    (void)id;
    (void)in_len;
    (void)in;
    return TFM_PLAT_ERR_UNSUPPORTED;
}
#endif

enum tfm_plat_err_t tfm_plat_otp_get_size(enum tfm_otp_element_id_t id,
                                          size_t *size)
{
    switch (id) {
        case PLAT_OTP_ID_HUK:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->huk);
            break;
        case PLAT_OTP_ID_IAK:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->iak);
            break;
        case PLAT_OTP_ID_IAK_LEN:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->iak_len);
            break;
        case PLAT_OTP_ID_IAK_TYPE:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->iak_type);
            break;
        case PLAT_OTP_ID_IAK_ID:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->iak_id);
            break;

        case PLAT_OTP_ID_BOOT_SEED:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->boot_seed);
            break;
        case PLAT_OTP_ID_LCS:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->lcs);
            break;
        case PLAT_OTP_ID_IMPLEMENTATION_ID:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->implementation_id);
            break;
        case PLAT_OTP_ID_HW_VERSION:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->hw_version);
            break;
        case PLAT_OTP_ID_VERIFICATION_SERVICE_URL:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->verification_service_url);
            break;
        case PLAT_OTP_ID_PROFILE_DEFINITION:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->profile_definition);
            break;

#ifdef BL2
        case PLAT_OTP_ID_BL2_ROTPK_0:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl2_rotpk_0);
            break;
        case PLAT_OTP_ID_BL2_ROTPK_1:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl2_rotpk_1);
            break;
        case PLAT_OTP_ID_BL2_ROTPK_2:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl2_rotpk_2);
            break;

        case PLAT_OTP_ID_NV_COUNTER_BL2_0:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl2_nv_counter_0);
            break;
        case PLAT_OTP_ID_NV_COUNTER_BL2_1:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl2_nv_counter_1);
            break;
        case PLAT_OTP_ID_NV_COUNTER_BL2_2:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl2_nv_counter_2);
            break;
#endif /* BL2 */

#ifdef BL1
        case PLAT_OTP_ID_BL1_ROTPK_0:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl1_rotpk_0);
            break;

        case PLAT_OTP_ID_NV_COUNTER_BL1_0:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->bl1_nv_counter_0);
            break;
#endif /* BL1 */

        case PLAT_OTP_ID_ENTROPY_SEED:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->entropy_seed);
            break;

        case PLAT_OTP_ID_SECURE_DEBUG_PK:
            *size = sizeof(((struct flash_otp_nv_counters_region_t*)0)->secure_debug_pk);
            break;

        default:
            return TFM_PLAT_ERR_UNSUPPORTED;
    }

    return TFM_PLAT_ERR_SUCCESS;
}
