/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __CONFIG_IMPL_H__
#define __CONFIG_IMPL_H__

/* Backends */
#define CONFIG_TFM_SPM_BACKEND_IPC                               1
#define CONFIG_TFM_SPM_BACKEND_SFN                               0

/* API calls */
#define CONFIG_TFM_PSA_API_SFN_CALL                              0
#define CONFIG_TFM_PSA_API_CROSS_CALL                            0
#define CONFIG_TFM_PSA_API_SUPERVISOR_CALL                       1

#define CONFIG_TFM_CONNECTION_BASED_SERVICE_API                  1
#define CONFIG_TFM_FLIH_API                                      0
#define CONFIG_TFM_SLIH_API                                      0

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
/* Trustzone NS agent working stack size. */
#define CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE                        1024

/* SPM re-uses Trustzone NS agent stack. */
#define CONFIG_TFM_SPM_THREAD_STACK_SIZE                             \
            CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE                       

#elif CONFIG_TFM_SPM_BACKEND_SFN == 1
/*
 * In isolation level 1 SFN model, all subsequent components work on NS agent
 * stack. It is observed that half of the sum of all partition stack sizes is
 * enough for working. Define a divisor factor
 * CONFIG_TFM_NS_AGENT_TZ_STK_SIZE_SHIFT_FACTOR for reference, and allow
 * modification of the factor based on application situation. The stack size
 * value is aligned to 8 bytes.
 * The minimum value is 0x400 to satisfy the SPM functional requirement.
 * Manifest tool will assure this.
 */
#define CONFIG_TFM_NS_AGENT_TZ_STK_SIZE_SHIFT_FACTOR             1
#define CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE                         \
    (((0x4a00 >> CONFIG_TFM_NS_AGENT_TZ_STK_SIZE_SHIFT_FACTOR) + 0x7) & (~0x7))

#endif /* CONFIG_TFM_SPM_BACKEND_IPC == 1 */

#if (CONFIG_TFM_FP > 0) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
#error "FP is not supported for SFN model."
#endif

#endif /* __CONFIG_IMPL_H__ */
