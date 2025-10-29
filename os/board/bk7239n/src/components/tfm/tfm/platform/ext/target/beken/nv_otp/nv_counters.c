/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* NOTE: For the security of the protected storage system, the bootloader
 * rollback protection, and the protection of cryptographic material  it is
 * CRITICAL to use a internal (in-die) persistent memory for the implementation
 * of the OTP_NV_COUNTERS flash area (see flash_otp_nv_layout.c).
 */

#include "tfm_plat_nv_counters.h"

#include <limits.h>
#include "Driver_Flash.h"
#include "flash_layout.h"
#include "tfm_plat_otp.h"
#include "cmsis_compiler.h"

#include <string.h>
#include "nv_otp_log.h"

#define OTP_COUNTER_MAX_SIZE    128u
#define NV_COUNTER_SIZE         4

enum flash_nv_counter_id_t {
    FLASH_NV_COUNTER_ID_PS_0 = 0,
    FLASH_NV_COUNTER_ID_PS_1,
    FLASH_NV_COUNTER_ID_PS_2,
    FLASH_NV_COUNTER_ID_MAX,
};

enum tfm_plat_err_t tfm_plat_init_nv_counter(void)
{
    return TFM_PLAT_ERR_SUCCESS;
}

#if 0
static enum tfm_plat_err_t read_nv_counter_otp(enum tfm_otp_element_id_t id,
                                               uint32_t size, uint8_t *val)
{
    size_t counter_size;
    enum tfm_plat_err_t err;
    size_t byte_idx;
    uint8_t bit_idx;
    uint8_t counter_value[OTP_COUNTER_MAX_SIZE];
    uint32_t count;

    OTP_LOGI("read nv counter otp: id=%d size=%d\r\n", id, size);

    err = tfm_plat_otp_get_size(id, &counter_size);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    counter_size = counter_size > OTP_COUNTER_MAX_SIZE ? OTP_COUNTER_MAX_SIZE : counter_size;

    err = tfm_plat_otp_read(id, counter_size, counter_value);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    count = 0;
    for (byte_idx = 0; byte_idx < counter_size; byte_idx++) {
        for (bit_idx = 0; bit_idx < 8; bit_idx++) {
            count += (counter_value[byte_idx] >> bit_idx) & 1;
        }
    }

    memcpy(val, &count, NV_COUNTER_SIZE);

    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t set_nv_counter_otp(enum tfm_otp_element_id_t id,
                                              uint32_t value)
{
    size_t counter_size;
    enum tfm_plat_err_t err;
    size_t byte_idx;
    uint8_t counter_value[OTP_COUNTER_MAX_SIZE];

    OTP_LOGI("set nv counter otp: id=%d size=%d\r\n", id, value);

    err = tfm_plat_otp_get_size(id, &counter_size);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    counter_size = counter_size > OTP_COUNTER_MAX_SIZE ? OTP_COUNTER_MAX_SIZE : counter_size;

    if (value > (8 * counter_size)) {
        return TFM_PLAT_ERR_MAX_VALUE;
    }

    memset(counter_value, 0, OTP_COUNTER_MAX_SIZE);
    for (byte_idx = 0; byte_idx < (value / 8); byte_idx++) {
        counter_value[byte_idx] = UINT8_MAX;
    }
    counter_value[byte_idx] = (1 << (value % 8)) - 1;

    err = tfm_plat_otp_write(id, counter_size, counter_value);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    return TFM_PLAT_ERR_SUCCESS;
}
#endif

enum tfm_plat_err_t tfm_plat_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                             uint32_t size, uint8_t *val)
{
    OTP_LOGI("read nv counter: id=%d size=%d\r\n", counter_id, size);

    if (size != NV_COUNTER_SIZE) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    switch(counter_id) {
    case (PLAT_NV_COUNTER_PS_0):
    case (PLAT_NV_COUNTER_PS_1):
    case (PLAT_NV_COUNTER_PS_2):
        return TFM_PLAT_ERR_SUCCESS;
    case (PLAT_NV_COUNTER_BL2_0):
        return TFM_PLAT_ERR_SUCCESS;
        //return read_nv_counter_otp(PLAT_OTP_ID_NV_COUNTER_BL2_0, size, val);//to optimize codesize
    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }
}

enum tfm_plat_err_t tfm_plat_set_nv_counter(enum tfm_nv_counter_t counter_id,
                                            uint32_t value)
{
    OTP_LOGI("set nv counter: id=%d size=%d\r\n", counter_id, value);
    switch(counter_id) {
    case (PLAT_NV_COUNTER_PS_0):
    case (PLAT_NV_COUNTER_PS_1):
    case (PLAT_NV_COUNTER_PS_2):
        return TFM_PLAT_ERR_SUCCESS;
    case (PLAT_NV_COUNTER_BL2_0):
        return TFM_PLAT_ERR_SUCCESS;
        //return set_nv_counter_otp(PLAT_OTP_ID_NV_COUNTER_BL2_0, value); //to optimize codesize
    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }
}

enum tfm_plat_err_t tfm_plat_increment_nv_counter(
                                           enum tfm_nv_counter_t counter_id)
{
    uint32_t security_cnt = 0;
    enum tfm_plat_err_t err;

    OTP_LOGI("increment nv counter: id=%d\r\n", counter_id);

    err = tfm_plat_read_nv_counter(counter_id,
                                   sizeof(security_cnt),
                                   (uint8_t *)&security_cnt);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    if (security_cnt == UINT32_MAX) {
        return TFM_PLAT_ERR_MAX_VALUE;
    }

    return tfm_plat_set_nv_counter(counter_id, security_cnt + 1u);
}
