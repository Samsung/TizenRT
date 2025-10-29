/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_ASSERT_H__
#define __TFM_ASSERT_H__

#include "tfm_spm_log.h"

/* Functions and macros in this file is for 'thread mode' usage. */

#define TFM_ASSERT(cond)                                                    \
            do {                                                            \
                if (!(cond)) {                                              \
                    SPMLOG_INFMSG("Assert:");                               \
                    SPMLOG_INFMSG(__FUNCTION__);                            \
                    SPMLOG_INFMSGVAL(",", __LINE__);                        \
                    while (1)                                               \
                        ;                                                   \
                }                                                           \
            } while (0)

#endif /* __TFM_ASSERT_H__ */
