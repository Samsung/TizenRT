/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stddef.h>
#include "spm_ipc.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"

#define IDLE_SP_STACK_SIZE      (0x200)

struct partition_tfm_sp_idle_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
} __attribute__((aligned(4)));

/* Entrypoint function declaration */
extern void tfm_idle_thread(void);
/* Stack */
uint8_t idle_sp_stack[IDLE_SP_STACK_SIZE] __attribute__((aligned(8)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load"
__root
#endif
const struct partition_tfm_sp_idle_load_info_t
    tfm_sp_idle_load __attribute__((used, section(".part_load"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0101 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_IDLE_ID,
        .flags                      = PARTITION_PRI_LOWEST | PARTITION_MODEL_IPC
                                      | PARTITION_MODEL_PSA_ROT,
        .entry                      = ENTRY_TO_POSITION(tfm_idle_thread),
        .stack_size                 = IDLE_SP_STACK_SIZE,
        .heap_size                  = 0,
        .ndeps                      = 0,
        .nservices                  = 0,
        .nassets                    = 0,
    },
    .stack_addr                     = (uintptr_t)idle_sp_stack,
    .heap_addr                      = 0,
};

/* Placeholder for partition runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location = ".bss.part_runtime"
__root
#endif
static struct partition_t tfm_idle_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime")));
