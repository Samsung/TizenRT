/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_secure_client_service_api.h"
#include "tfm_veneers.h"

int32_t tfm_secure_client_run_tests(void)
{
#ifndef TFM_PSA_API
    tfm_secure_client_service_sfn_run_tests_veneer(NULL, 0, NULL, 0);
#endif

    return 0;
}
