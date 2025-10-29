/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "cmsis.h"
#include "tfm_ns_ctx.h"
#include "tfm_nspm.h"

/*
 * NS context. Initialized to 0.
 * All contexts are not used as the reference counter is 0 in initialization.
 */
static struct tfm_ns_ctx_t ns_ctx_data[TFM_NS_CONTEXT_MAX] = {0};

/* Current active NS context index. Default is invalid index */
static uint8_t active_ns_ctx_index = TFM_NS_CONTEXT_MAX;

bool init_ns_ctx(void)
{
    uint32_t i;

    for (i = 0; i < TFM_NS_CONTEXT_MAX; i++) {
        /* Only need to ensure the reference counter is 0 */
        ns_ctx_data[i].ref_cnt = 0;
    }

    active_ns_ctx_index = TFM_NS_CONTEXT_MAX;
    return true;
}

bool acquire_ns_ctx(uint8_t gid, uint8_t *idx)
{
    /* Only support one context slot for now */
    uint32_t i;
    uint32_t empty_ctx_idx = TFM_NS_CONTEXT_MAX; /* default is invalid */

    __disable_irq();
    /*
     * Go through all context slots to get the context for the given group ID.
     * It will not take a long time as the context number should be limited.
     */
    for (i = 0; i < TFM_NS_CONTEXT_MAX; i++) {
        if (ns_ctx_data[i].ref_cnt > 0) {  /* This context has been taken */
            if (ns_ctx_data[i].gid == gid) {
                /*
                 * Found the context associated with the input group ID.
                 * Check if the thread number reached the limit.
                 */
                if (ns_ctx_data[i].ref_cnt < TFM_NS_CONTEXT_MAX_TID) {
                    /* Reuse this context and increase the reference number */
                    ns_ctx_data[i].ref_cnt++;
                    *idx = i;
                    __enable_irq();
                    return true;
                } else {
                    /* No more thread for this group */
                    __enable_irq();
                    return false;
                }
            }
        } else {    /* Found an empty context slot */
            /* Save the first unused context index and continue */
            if (empty_ctx_idx == TFM_NS_CONTEXT_MAX) {
                empty_ctx_idx = i;
            }
            /*
             * Still need to go through the remaining slots to check if there
             * is an existing context assigned to the given group ID.
             */
        }
    }

    /* No existing context for the group ID, use the first free context */
    if (empty_ctx_idx < TFM_NS_CONTEXT_MAX) {
        ns_ctx_data[empty_ctx_idx].ref_cnt++;
        ns_ctx_data[empty_ctx_idx].gid = gid;
        *idx = empty_ctx_idx;
        __enable_irq();
        return true;
    } else {
        __enable_irq();
        return false;   /* No available context */
    }
}

bool release_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx)
{
    /* Check if the index is in range */
    if (idx >= TFM_NS_CONTEXT_MAX) {
        return false;
    }

    __disable_irq();

    /* Check if the context belongs to that group  */
    if (ns_ctx_data[idx].gid != gid) {
        __enable_irq();
        return false;
    }

    /*
     * If it is to release the current active context, then set active context
     * to invalid.
     * Otherwise, just de-reference the context
     */
    if (idx == active_ns_ctx_index) {
        if (ns_ctx_data[idx].tid == tid) {
            /* Release the currrent active thread */
            if (ns_ctx_data[idx].ref_cnt > 0) {
                ns_ctx_data[idx].ref_cnt--;
            }
            active_ns_ctx_index = TFM_NS_CONTEXT_MAX;
        } else {
            /*
             * Release for another thread in the active context
             * As there's an active thread ongoing, so the ref_counter should
             * be at least 1 after the release.
             */
            if (ns_ctx_data[idx].ref_cnt > 1) {
                ns_ctx_data[idx].ref_cnt--;
            }
        }
    } else {
        /* Release in the non-active context */
        if (ns_ctx_data[idx].ref_cnt > 0) {
            ns_ctx_data[idx].ref_cnt--;
        }
    }

    __enable_irq();
    return true;
}

bool load_ns_ctx(uint8_t gid, uint8_t tid, int32_t nsid, uint8_t idx)
{
    /* Check if the index is in range */
    if (idx >= TFM_NS_CONTEXT_MAX) {
        return false;
    }

    __disable_irq();

    /* Check group ID and reference counter */
    if ((ns_ctx_data[idx].gid != gid) || (ns_ctx_data[idx].ref_cnt == 0)) {
        __enable_irq();
        return false;
    }

    ns_ctx_data[idx].tid = tid;
    ns_ctx_data[idx].nsid = nsid;
    active_ns_ctx_index = idx;
    __enable_irq();
    return true;
}

bool save_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx)
{
    __disable_irq();
    /* Check if the given index is valid */
    if ((idx != active_ns_ctx_index) || (idx >= TFM_NS_CONTEXT_MAX)) {
        __enable_irq();
        return false;
    }

    /* Check group, thread ID and reference counter */
    if ((ns_ctx_data[idx].gid != gid)
        || (ns_ctx_data[idx].tid != tid)
        || (ns_ctx_data[idx].ref_cnt == 0)) {
        __enable_irq();
        return false;
    }

    /* Set active context index to invalid */
    active_ns_ctx_index = TFM_NS_CONTEXT_MAX;
    __enable_irq();
    return true;
}

int32_t get_nsid_from_active_ns_ctx(void)
{
    int32_t ret = TFM_NS_CLIENT_INVALID_ID;

    __disable_irq();

    if (active_ns_ctx_index < TFM_NS_CONTEXT_MAX) {
        ret = ns_ctx_data[active_ns_ctx_index].nsid;
    }

    __enable_irq();
    return ret;
}
