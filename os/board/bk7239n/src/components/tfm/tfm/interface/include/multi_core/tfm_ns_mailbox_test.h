/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* Data types and API definitions in test of NSPE mailbox */

#ifndef __TFM_NS_MAILBOX_TEST_H__
#define __TFM_NS_MAILBOX_TEST_H__

#include <stdint.h>

#include "tfm_ns_mailbox.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief The structure to hold the statistics result of NSPE mailbox
 */
struct ns_mailbox_stats_res_t {
    uint8_t avg_nr_slots;               /* The value before the decimal point
                                         * in the average number of NSPE
                                         * mailbox slots in use.
                                         */
    uint8_t avg_nr_slots_tenths;        /* The first digit value after the
                                         * decimal point in the average
                                         * number of NSPE mailbox slots in use.
                                         */
};

/**
 * \brief Initialize the statistics module in TF-M NSPE mailbox.
 *
 * \note This function is only available when multi-core tests are enabled.
 *
 * \param[in] ns_queue          The NSPE mailbox queue to be tracked.
 */
void tfm_ns_mailbox_tx_stats_init(struct ns_mailbox_queue_t *ns_queue);

/**
 * \brief Re-initialize the statistics module in TF-M NSPE mailbox.
 *        Clean up statistics data.
 *
 * \note This function is only available when multi-core tests are enabled.
 *
 * \return \ref MAILBOX_SUCCESS if the operation succeeded, or other return code
           in case of error
 */
int32_t tfm_ns_mailbox_tx_stats_reinit(void);

/**
 * \brief Update the statistics result of NSPE mailbox message transmission.
 *
 * \note This function is only available when multi-core tests are enabled.
 */
void tfm_ns_mailbox_tx_stats_update(void);

/**
 * \brief Calculate the average number of used NS mailbox queue slots each time
 *        NS task requires a queue slot to submit mailbox message, which is
 *        recorded in NS mailbox statisitics module.
 *
 * \note This function is only available when multi-core tests are enabled.
 *
 * \param[in] stats_res         The buffer to be written with
 *                              \ref ns_mailbox_stats_res_t.
 *
 * \return Return the calculation result.
 */
void tfm_ns_mailbox_stats_avg_slot(struct ns_mailbox_stats_res_t *stats_res);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_MAILBOX_TEST_H__ */
