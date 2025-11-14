/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "compiler_ext_defs.h"
#include "runtime_defs.h"
#include "sprt_partition_metadata_indicator.h"
#include "sprt_main.h"

extern void common_sfn_thread(void);

__used static uintptr_t runtime_init_c(void)
{
    struct runtime_metadata_t *p_rt_meta;

    p_rt_meta = PART_METADATA();

    if (p_rt_meta->n_sfn == 0) {
        /* IPC Partition */
        return p_rt_meta->entry;
    } else {
        /* SFN Partition */
        return (uintptr_t)common_sfn_thread;
    }
}

#if defined(__ICCARM__)
#pragma required = runtime_init_c
#endif

__naked void sprt_main(void)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified    \n"
#endif
        "bl runtime_init_c  \n"
        "bx r0              \n"
    );
}
