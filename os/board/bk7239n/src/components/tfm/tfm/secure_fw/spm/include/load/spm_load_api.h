/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* !! For SPM load usage, no including by components out of SPM !! */
#ifndef __SPM_LOAD_API_H__
#define __SPM_LOAD_API_H__

#include "asset_defs.h"
#include "interrupt_defs.h"
#include "partition_defs.h"
#include "service_defs.h"
#include "spm_ipc.h"

/* No more partition to be loaded */
#define NO_MORE_PARTITION        NULL

/* Length of extendable variables in partition load type */
#define LOAD_INFO_EXT_LENGTH                        (2)
/* Argument "pldinf" must be a "struct partition_load_info_t *". */
#define LOAD_INFSZ_BYTES(pldinf)                                       \
    (sizeof(*(pldinf)) + LOAD_INFO_EXT_LENGTH * sizeof(uintptr_t) +    \
     (pldinf)->ndeps * sizeof(uint32_t) +                              \
     (pldinf)->nservices * sizeof(struct service_load_info_t) +        \
     (pldinf)->nassets * sizeof(struct asset_desc_t) +                 \
     (pldinf)->nirqs * sizeof(struct irq_load_info_t))

/* 'Allocate' stack based on load info */
#define LOAD_ALLOCED_STACK_ADDR(pldinf)    (*((uintptr_t *)(pldinf + 1)))

#define LOAD_INFO_DEPS(pldinf)                                         \
    ((uintptr_t)(pldinf + 1) + LOAD_INFO_EXT_LENGTH * sizeof(uintptr_t))
#define LOAD_INFO_SERVICE(pldinf)                                      \
    ((uintptr_t)LOAD_INFO_DEPS(pldinf) + (pldinf)->ndeps * sizeof(uint32_t))
#define LOAD_INFO_ASSET(pldinf)                                        \
    ((uintptr_t)LOAD_INFO_SERVICE(pldinf) +                            \
     (pldinf)->nservices * sizeof(struct service_load_info_t))
#define LOAD_INFO_IRQ(pldinf)                                          \
    ((uintptr_t)LOAD_INFO_ASSET(pldinf) +                              \
     (pldinf)->nassets * sizeof(struct asset_desc_t))

/* Runtime partition struct list head node type */
struct partition_head_t {
    uint32_t reserved;                  /* Reserved             */
    struct partition_t *next;           /* Next partition node  */
};

/* Runtime service struct list head node type */
struct service_head_t {
    uint32_t reserved;                  /* Reserved             */
    struct service_t *next;             /* Next partition node  */
};

/*
 * Load a partition object to linked list and return if a load is successful.
 * An 'assuredly' function, return NO_MORE_PARTITION for no more partitions and
 * return a valid pointer if succeed. Other errors simply panic the system and
 * never return.
 */
struct partition_t *load_a_partition_assuredly(struct partition_head_t *head);

/*
 * Load numbers of service objects to linked list based on given partition.
 * It loads connection based services and stateless services that partition
 * contains.
 * As an 'assuredly' function, errors simply panic the system and never
 * return.
 * This function returns the service signal set in a 32 bit number. Return
 * ZERO if services are not represented by signals.
 */
uint32_t load_services_assuredly(struct partition_t *p_partition,
                                 struct service_head_t *services_listhead,
                                 struct service_t **stateless_services_ref_tbl,
                                 size_t ref_tbl_size);

/*
 * Append IRQ signals to Partition signals.
 * Set initial IRQ enabled status according to framework version.
 * And initialize the IRQ.
 * Any error within this API causes system to panic.
 */
void load_irqs_assuredly(struct partition_t *p_partition);

#endif /* __SPM_LOAD_API_H__ */
