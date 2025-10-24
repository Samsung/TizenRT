/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_hal_sp_logdev.h"
#include "uart_stdout.h"
#ifdef TFM_PSA_API
#include "svc_num.h"
#else
#include "tfm_core_svc.h"
#endif /* TFM_PSA_API */
#include "tfm_hal_device_header.h"

__attribute__((naked))
static int tfm_output_unpriv_string(const unsigned char *str, size_t len)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_OUTPUT_UNPRIV_STRING));
}

int32_t tfm_hal_output_sp_log(const unsigned char *str, size_t len)
{
    /*
     * Peripheral based log function call the tfm_output_unpriv_string
     * directly.
     */
    return tfm_output_unpriv_string(str, len);
}
