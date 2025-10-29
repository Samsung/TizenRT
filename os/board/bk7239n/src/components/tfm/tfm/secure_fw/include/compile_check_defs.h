/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __COMPILE_CHECK_DEFS_H__
#define __COMPILE_CHECK_DEFS_H__

#include "load/spm_load_api.h"
#include "thread.h"

#if TO_THREAD_PRIORITY(PARTITION_PRI_HIGHEST) != THRD_PRIOR_HIGHEST ||  \
    TO_THREAD_PRIORITY(PARTITION_PRI_HIGH) != THRD_PRIOR_HIGH ||        \
    TO_THREAD_PRIORITY(PARTITION_PRI_NORMAL) != THRD_PRIOR_MEDIUM ||    \
    TO_THREAD_PRIORITY(PARTITION_PRI_LOW) != THRD_PRIOR_LOW ||          \
    TO_THREAD_PRIORITY(PARTITION_PRI_LOWEST) != THRD_PRIOR_LOWEST
#error "Partition priority converting to thread priority error!"
#endif

#endif /* __COMPILE_CHECK_DEFS_H__ */
