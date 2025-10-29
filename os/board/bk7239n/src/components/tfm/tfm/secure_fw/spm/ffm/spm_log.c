/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_spm_log.h"

#define MAX_DIGIT_BITS 12  /* 8 char for number, 2 for '0x' and 2 for '\r\n' */
const static char HEX_TABLE[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/**
 * \brief Convert digit number into HEX format string, the string have '0x'
 *        prefix and leading zeros are not stripped.
 *
 * \param[in]  value  A value need to be converted.
 * \param[in]  msg    A string message that the value converted to.
 *
 */

static void to_hex(uint32_t value, char msg[])
{
    int i = MAX_DIGIT_BITS - 1;

    msg[i--] = '\n';
    msg[i--] = '\r';
    for (; i > 1; i--, value >>= 4) {
        msg[i] = HEX_TABLE[value & 0xF];
    }
    msg[i--] = 'x';
    msg[i--] = '0';
}

int32_t spm_log_msgval(const char *msg, size_t len, uint32_t value)
{
    int32_t result_msg = 0, result_val;
    char value_str[MAX_DIGIT_BITS];

    if (msg && len) {
        result_msg = tfm_hal_output_spm_log(msg, len);
        if (result_msg < TFM_HAL_SUCCESS) {
            return result_msg;
        }
    }

    to_hex(value, value_str);

    result_val = tfm_hal_output_spm_log(value_str,
                                        MAX_DIGIT_BITS);
    if (result_val < TFM_HAL_SUCCESS) {
        return result_val;
    }
    return (result_msg + result_val);
}
