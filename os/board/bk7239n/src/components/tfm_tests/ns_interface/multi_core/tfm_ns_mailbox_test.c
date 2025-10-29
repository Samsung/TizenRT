/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_ns_mailbox.h"
#include "tfm_ns_mailbox_test.h"

static struct ns_mailbox_queue_t *stats_queue_ptr = NULL;

void tfm_ns_mailbox_tx_stats_init(struct ns_mailbox_queue_t *ns_queue)
{
    if (!ns_queue) {
        return;
    }

    ns_queue->nr_tx = 0;
    ns_queue->nr_used_slots = 0;

    stats_queue_ptr = ns_queue;
}

int32_t tfm_ns_mailbox_tx_stats_reinit(void)
{
    if (!stats_queue_ptr) {
        return MAILBOX_INVAL_PARAMS;
    }

    stats_queue_ptr->nr_tx = 0;
    stats_queue_ptr->nr_used_slots = 0;

    return MAILBOX_SUCCESS;
}

void tfm_ns_mailbox_tx_stats_update(void)
{
    mailbox_queue_status_t empty_status;
    uint8_t idx, nr_empty = 0;

    if (!stats_queue_ptr) {
        return;
    }

    tfm_ns_mailbox_os_spin_lock();
    /* Count the number of used slots when this tx arrives */
    empty_status = stats_queue_ptr->empty_slots;
    tfm_ns_mailbox_os_spin_unlock();

    if (empty_status) {
        for (idx = 0; idx < NUM_MAILBOX_QUEUE_SLOT; idx++) {
            if (empty_status & (0x1UL << idx)) {
                nr_empty++;
            }
        }
    }

    tfm_ns_mailbox_os_spin_lock();
    stats_queue_ptr->nr_used_slots += (NUM_MAILBOX_QUEUE_SLOT - nr_empty);
    stats_queue_ptr->nr_tx++;
    tfm_ns_mailbox_os_spin_unlock();
}

void tfm_ns_mailbox_stats_avg_slot(struct ns_mailbox_stats_res_t *stats_res)
{
    uint32_t nr_used_slots, nr_tx;

    if (!stats_queue_ptr || !stats_res) {
        return;
    }

    nr_used_slots = stats_queue_ptr->nr_used_slots;
    nr_tx = stats_queue_ptr->nr_tx;

    stats_res->avg_nr_slots = nr_used_slots / nr_tx;
    nr_used_slots %= nr_tx;
    stats_res->avg_nr_slots_tenths = nr_used_slots * 10 / nr_tx;
}
