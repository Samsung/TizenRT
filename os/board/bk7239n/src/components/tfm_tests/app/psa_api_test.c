/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa_api_test.h"
#include "tfm_integ_test.h"

/**
 * \brief This symbol is the entry point provided by the PSA API compliance
 *        test libraries
 */
extern void val_entry(void);

__attribute__((noreturn))
void psa_api_test(void *arg)
{
    UNUSED_VARIABLE(arg);

    val_entry();

    for (;;) {
    }
}
