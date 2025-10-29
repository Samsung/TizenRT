/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***** WARNING: This file SHOULD BE CHANGED according to storage template *****/

#include <stdint.h>
#include <stddef.h>
#include "compiler_ext_defs.h"
#include "config_impl.h"
#include "spm_ipc.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"

#define TFM_SP_NS_AGENT_NDEPS                                   (0)
#define TFM_SP_NS_AGENT_NSERVS                                  (0)
#if TFM_LVL == 3
#define TFM_SP_NS_AGENT_NASSETS                                 (1)
#endif

/* Entrypoint function declaration */
extern void ns_agent_tz_main(void);

/* Stack */
uint8_t ns_agent_tz_stack[CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE] __aligned(0x20);

struct partition_tfm_sp_ns_agent_tz_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
#if TFM_LVL == 3
    struct asset_desc_t             assets[TFM_SP_NS_AGENT_NASSETS];
#endif
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load"
__root
#endif
const struct partition_tfm_sp_ns_agent_tz_load_info_t
    tfm_sp_ns_agent_tz_load __attribute__((used, section(".part_load"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0100 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_NON_SECURE_ID,
        .flags                      = (PARTITION_PRI_LOWEST - 1)
                                    | PARTITION_MODEL_IPC
                                    | PARTITION_MODEL_PSA_ROT,
        .entry                      = ENTRY_TO_POSITION(ns_agent_tz_main),
        .stack_size                 = CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE,
        .heap_size                  = 0,
        .ndeps                      = TFM_SP_NS_AGENT_NDEPS,
        .nservices                  = TFM_SP_NS_AGENT_NSERVS,
#if TFM_LVL == 3
        .nassets                    = TFM_SP_NS_AGENT_NASSETS,
#endif
    },
    .stack_addr                     = (uintptr_t)ns_agent_tz_stack,
    .heap_addr                      = 0,
#if TFM_LVL == 3
    .assets                         = {
        {
            .mem.start              = (uintptr_t)ns_agent_tz_stack,

            .mem.limit              =
               (uintptr_t)&ns_agent_tz_stack[CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE],
            .attr                   = ASSET_ATTR_READ_WRITE,
        },
    },
#endif
};
#if defined(__ICCARM__)
#pragma location = ".bss.part_runtime"
__root
#endif
/* Placeholder for partition runtime space. Do not reference it. */
static struct partition_t tfm_sp_ns_agent_tz_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime")));
