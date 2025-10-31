/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_UTILS_H__
#define __TFM_UTILS_H__

#include <stdbool.h>
#include <stdio.h>
#include "tfm_spm_log.h"

/*
 * CPU spin here.
 * Note: this function is used to handle PROGRAMMER ERROR.
 */
void tfm_core_panic(void);

/* Core assert and spin */
#ifndef NDEBUG
#define TFM_CORE_ASSERT(cond)                                       \
            do {                                                    \
                if (!(cond)) {                                      \
                    while (1)                                       \
                        ;                                           \
                }                                                   \
            } while (0)
#else
#define TFM_CORE_ASSERT(cond)
#endif

/* Get container structure start address from member */
#define TO_CONTAINER(ptr, type, member) \
    (type *)((unsigned long)(ptr) - offsetof(type, member))

/* FixMe: Replace ERROR_MSG() in platform code with a suitable API */
#define ERROR_MSG(msg) SPMLOG_ERRMSG(msg "\r\n")

/* Stringify preprocessors, no leading underscore. ('STRINGIFY') */
#define STRINGIFY_EXPAND(x) #x
#define M2S(m) STRINGIFY_EXPAND(m)

#endif /* __TFM_UTILS_H__ */
