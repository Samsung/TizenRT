/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PARTITION_DEFS_H__
#define __PARTITION_DEFS_H__

#include <stddef.h>
#include <stdint.h>

/* TF-M internal partition ID */
#define TFM_SP_NON_SECURE_ID                    (0)
#define TFM_SP_IDLE_ID                          (1)
#define INVALID_PARTITION_ID                    (~0U)

/* Encode a magic number into version for validating partition info */
#define PARTITION_INFO_VERSION_MASK             (0x0000FFFF)
#define PARTITION_INFO_MAGIC_MASK               (0xFFFF0000)
#define PARTITION_INFO_MAGIC                    (0x5F5F0000)

/*
 * Partition load data - flags
 * bit 7-0: priority
 * bit 8: 1 - PSA_ROT, 0 - APP_ROT
 * bit 9: 1 - IPC model, 0 - SFN model
 */
#define PARTITION_PRI_HIGHEST                   (0x0)
#define PARTITION_PRI_HIGH                      (0xF)
#define PARTITION_PRI_NORMAL                    (0x1F)
#define PARTITION_PRI_LOW                       (0x7F)
#define PARTITION_PRI_LOWEST                    (0xFF)
#define PARTITION_PRI_MASK                      (0xFF)

#define PARTITION_MODEL_PSA_ROT                 (1U << 8)
#define PARTITION_MODEL_IPC                     (1U << 9)

#define PARTITION_PRIORITY(flag)                ((flag) & PARTITION_PRI_MASK)
#define TO_THREAD_PRIORITY(x)                   (x)

#define ENTRY_TO_POSITION(x)                    (uintptr_t)(x)
#define POSITION_TO_ENTRY(x, t)                 (t)(x)

#define PTR_TO_REFERENCE(x)                     (uintptr_t)(x)
#define REFERENCE_TO_PTR(x, t)                  (t)(x)

#define IS_PARTITION_PSA_ROT(pldi)              (!!((pldi)->flags \
                                                     & PARTITION_MODEL_PSA_ROT))
#define IS_PARTITION_IPC_MODEL(pldi)            (!!((pldi)->flags \
                                                         & PARTITION_MODEL_IPC))

/*
 * Common partition structure type, the extendable data is right after it.
 * Extendable data has different size for each partition, and must be 4-byte
 * aligned. It includes: stack and heap position, dependencies, services and
 * assets data.
 */
struct partition_load_info_t {
    uint32_t        psa_ff_ver;         /* Encode the version with magic    */
    int32_t         pid;                /* Partition ID                     */
    char *          name;
    uint32_t        flags;              /* ARoT/PRoT, SFN/IPC, priority     */
    uintptr_t       entry;              /* Entry point                      */
    size_t          stack_size;         /* Stack size                       */
    size_t          heap_size;          /* Heap size                        */
    uint32_t        ndeps;              /* Dependency number                */
    uint32_t        nservices;          /* Service number                   */
    uint32_t        nassets;            /* Asset numbers                    */
    uint32_t        nirqs;              /* Number of IRQ owned by Partition */
} __attribute__((aligned(4)));

#endif /* __PARTITION_DEFS_H__ */
