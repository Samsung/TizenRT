/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <limits.h>
#include <stdint.h>
#include "config_impl.h"
#include "lists.h"
#include "region.h"
#include "region_defs.h"
#include "spm_ipc.h"
#include "tfm_hal_interrupt.h"
#include "tfm_plat_defs.h"
#include "tfm_spm_hal.h"
#include "utilities.h"
#include "load/partition_defs.h"
#include "load/spm_load_api.h"
#include "load/service_defs.h"
#include "psa/client.h"

/* Partition load data region */
REGION_DECLARE(Image$$, TFM_SP_LOAD_LIST, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_SP_LOAD_LIST, $$RO$$Limit);

/* Partition and service runtime pool region */
REGION_DECLARE(Image$$, ER_PART_RT_POOL, $$ZI$$Base);
REGION_DECLARE(Image$$, ER_PART_RT_POOL, $$ZI$$Limit);
REGION_DECLARE(Image$$, ER_SERV_RT_POOL, $$ZI$$Base);
REGION_DECLARE(Image$$, ER_SERV_RT_POOL, $$ZI$$Limit);

static uintptr_t ldinf_sa = PART_REGION_ADDR(TFM_SP_LOAD_LIST, $$RO$$Base);
static uintptr_t ldinf_ea = PART_REGION_ADDR(TFM_SP_LOAD_LIST, $$RO$$Limit);
static uintptr_t part_pool_sa = PART_REGION_ADDR(ER_PART_RT_POOL, $$ZI$$Base);
static uintptr_t part_pool_ea = PART_REGION_ADDR(ER_PART_RT_POOL, $$ZI$$Limit);
static uintptr_t serv_pool_sa = PART_REGION_ADDR(ER_SERV_RT_POOL, $$ZI$$Base);
static uintptr_t serv_pool_ea = PART_REGION_ADDR(ER_SERV_RT_POOL, $$ZI$$Limit);

/* Allocate runtime space for partition. Panic if pool runs out. */
static struct partition_t *tfm_allocate_partition_assuredly(void)
{
    struct partition_t *p_part_allocated = (struct partition_t *)part_pool_sa;

    part_pool_sa += sizeof(struct partition_t);
    if (part_pool_sa > part_pool_ea) {
        tfm_core_panic();
    }

    return p_part_allocated;
}

/* Allocate runtime space for services. Panic if pool runs out. */
static struct service_t *tfm_allocate_service_assuredly(uint32_t service_count)
{
    struct service_t *p_serv_allocated = (struct service_t *)serv_pool_sa;

    if (service_count == 0) {
        return NULL;
    }

    serv_pool_sa += service_count * sizeof(struct service_t);
    if (serv_pool_sa > serv_pool_ea) {
        tfm_core_panic();
    }

    return p_serv_allocated;
}

struct partition_t *load_a_partition_assuredly(struct partition_head_t *head)
{
    struct partition_load_info_t *p_ptldinf;
    struct partition_t           *partition;

    if (!head) {
        tfm_core_panic();
        return NO_MORE_PARTITION;
    }

    if ((UINTPTR_MAX - ldinf_sa < sizeof(struct partition_load_info_t)) ||
        (ldinf_sa + sizeof(struct partition_load_info_t) >= ldinf_ea)) {
        return NO_MORE_PARTITION;
    }

    p_ptldinf = (struct partition_load_info_t *)ldinf_sa;

    if ((UINTPTR_MAX - ldinf_sa < LOAD_INFSZ_BYTES(p_ptldinf)) ||
        (ldinf_sa + LOAD_INFSZ_BYTES(p_ptldinf) > ldinf_ea))   {
        tfm_core_panic();
    }

    /* Magic ensures data integrity */
    if ((p_ptldinf->psa_ff_ver & PARTITION_INFO_MAGIC_MASK)
        != PARTITION_INFO_MAGIC) {
        tfm_core_panic();
    }

    if ((p_ptldinf->psa_ff_ver & PARTITION_INFO_VERSION_MASK)
        > PSA_FRAMEWORK_VERSION) {
        tfm_core_panic();
    }

    if (p_ptldinf->pid < 0) {
        /* 0 is the internal NS Agent, besides the normal positive PIDs */
        tfm_core_panic();
    }

    partition = tfm_allocate_partition_assuredly();
    partition->p_ldinf = p_ptldinf;

    ldinf_sa += LOAD_INFSZ_BYTES(p_ptldinf);

    UNI_LIST_INSERT_AFTER(head, partition, next);

    return partition;
}

uint32_t load_services_assuredly(struct partition_t *p_partition,
                                 struct service_head_t *services_listhead,
                                 struct service_t **stateless_services_ref_tbl,
                                 size_t ref_tbl_size)
{
    uint32_t i, serv_ldflags, hidx, service_setting = 0;
    struct service_t *services;
    const struct partition_load_info_t *p_ptldinf;
    const struct service_load_info_t *p_servldinf;

    if (!p_partition || !services_listhead) {
        tfm_core_panic();
        return 0;
    }

    p_ptldinf = p_partition->p_ldinf;
    p_servldinf = (struct service_load_info_t *)LOAD_INFO_SERVICE(p_ptldinf);

    /*
     * 'services' CAN be NULL when no services, which is a rational result.
     * The loop won't go in the NULL case.
     */
    services = tfm_allocate_service_assuredly(p_ptldinf->nservices);
    for (i = 0; i < p_ptldinf->nservices && services; i++) {
        services[i].p_ldinf = &p_servldinf[i];
        services[i].partition = p_partition;
        services[i].next = NULL;

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
        service_setting |= p_servldinf[i].signal;
#endif

        /* Populate the stateless service reference table */
        serv_ldflags = p_servldinf[i].flags;
        if (SERVICE_IS_STATELESS(serv_ldflags)) {
            if ((stateless_services_ref_tbl == NULL) ||
                (ref_tbl_size == 0) ||
                (ref_tbl_size !=
                 STATIC_HANDLE_NUM_LIMIT * sizeof(struct service_t *))) {
                tfm_core_panic();
                return 0;
            }

            hidx = SERVICE_GET_STATELESS_HINDEX(serv_ldflags);

            if ((hidx >= STATIC_HANDLE_NUM_LIMIT) ||
                stateless_services_ref_tbl[hidx]) {
                tfm_core_panic();
                return 0;
            }
            stateless_services_ref_tbl[hidx] = &services[i];
        }

        UNI_LIST_INSERT_AFTER(services_listhead, &services[i], next);
    }

    return service_setting;
}

void load_irqs_assuredly(struct partition_t *p_partition)
{
    struct irq_load_info_t *p_irq_info;
    const struct partition_load_info_t *p_ldinf;
    uint32_t i;

    if (!p_partition) {
        tfm_core_panic();
        return;
    }

    p_ldinf = p_partition->p_ldinf;
    p_irq_info = (struct irq_load_info_t *)LOAD_INFO_IRQ(p_ldinf);

    for (i = 0; i < p_ldinf->nirqs; i++) {
        p_partition->signals_allowed |= p_irq_info->signal;

        if (p_irq_info->init(p_partition, p_irq_info) != TFM_HAL_SUCCESS) {
            tfm_core_panic();
        }

        if ((p_ldinf->psa_ff_ver & PARTITION_INFO_VERSION_MASK) == 0x0100) {
            tfm_hal_irq_enable(p_irq_info->source);
        } else if ((p_ldinf->psa_ff_ver & PARTITION_INFO_VERSION_MASK)
                                                                    == 0x0101) {
            tfm_hal_irq_disable(p_irq_info->source);
        }
        p_irq_info++;
    }
}
