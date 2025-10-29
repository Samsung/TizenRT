/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_LOG_H
#define BL1_LOG_H

#include "uart_stdout.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_BL1_LOGGING
#define BL1_LOG(str) do { \
    const unsigned char msg[] = str; \
    stdio_output_string(msg, sizeof(msg)); \
} while (0)
#else
#define BL1_LOG(str)
#endif /* TFM_BL1_LOGGING */

#ifdef __cplusplus
}
#endif

#endif /* BL1_LOG_H */
