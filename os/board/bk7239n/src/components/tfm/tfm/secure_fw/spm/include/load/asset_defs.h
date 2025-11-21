/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __ASSET_DEFS_H__
#define __ASSET_DEFS_H__

#include <stdint.h>

/* Memory, MMIO and PPB asset attributes */
#define ASSET_ATTR_READ_ONLY        (1U << 0)  /* 1: Read-only MMIO         */
#define ASSET_ATTR_READ_WRITE       (1U << 1)  /* 1: Read-write MMIO        */
#define ASSET_ATTR_PPB              (1U << 2)  /* 1: PPB indicator          */

#define ASSET_ATTR_NAMED_MMIO       (1U << 3)  /* 1: Named mmio object       */
#define ASSET_ATTR_NUMBERED_MMIO    (1U << 4)  /* 1: Numbered mmio object    */
#define ASSET_ATTR_MMIO             (ASSET_ATTR_NAMED_MMIO | \
                                     ASSET_ATTR_NUMBERED_MMIO)

struct asset_desc_t {
    union {
        struct {                            /* Memory-based asset type  */
            uintptr_t start;
            uintptr_t limit;
        } mem;
        struct {                            /* Device asset type        */
            uintptr_t dev_ref;
            uintptr_t reserved;
        } dev;
    };
    uint32_t attr;                          /* Asset attributes         */
} __attribute__((aligned(4)));

#endif /* __ASSET_DEFS_H__ */
