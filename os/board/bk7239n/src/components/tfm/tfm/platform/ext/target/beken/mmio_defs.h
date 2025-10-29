/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MMIO_DEFS_H__
#define __MMIO_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_peripherals_def.h"

/* Boundary handle binding macros. */
#define HANDLE_PER_ATTR_BITS            (0x4)
#define HANDLE_ATTR_PRIV_MASK           ((1 << HANDLE_PER_ATTR_BITS) - 1)
#if TFM_LVL == 3
#define HANDLE_ATTR_RW_POS              (1 << (HANDLE_PER_ATTR_BITS - 1))
#define HANDLE_ATTR_INDEX_MASK          (HANDLE_ATTR_RW_POS - 1)
#define HANDLE_INDEX_BITS               (0x8)
#define HANDLE_INDEX_MASK               (((1 << HANDLE_INDEX_BITS) -1) << 24)
#define HANDLE_ENCODE_INDEX(attr, idx)                              \
    do {                                                            \
        (attr) |= (((idx) << 24) & HANDLE_INDEX_MASK);              \
        (idx)++;                                                    \
    } while (0)
#endif

/* Allowed named MMIO of this platform */
const uintptr_t partition_named_mmio_list[] = {
    (uintptr_t)TFM_PERIPHERAL_TIMER0,
    (uintptr_t)TFM_PERIPHERAL_STD_UART,
#ifdef PSA_API_TEST_IPC
    (uintptr_t)FF_TEST_UART_REGION,
    (uintptr_t)FF_TEST_WATCHDOG_REGION,
    (uintptr_t)FF_TEST_NVMEM_REGION,
    (uintptr_t)FF_TEST_SERVER_PARTITION_MMIO,
    (uintptr_t)FF_TEST_DRIVER_PARTITION_MMIO,
#endif
};

/*
 * Platform BK7236 only has named MMIO.
 * If the platform has numbered MMIO, define them in another list.
 */

#ifdef __cplusplus
}
#endif

#endif /* __MMIO_DEFS_H__ */
