/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_PROXY_SHARED_MEM_MNGR_H__
#define __PSA_PROXY_SHARED_MEM_MNGR_H__

#include "tfm_mailbox.h"
#include "psa/error.h"
#include "psa/service.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Returns the NS mailbox
 *
 * \return Returns a pointer to the NS mailbox
 */
struct ns_mailbox_queue_t * psa_proxy_get_ns_mailbox_queue(void);

/*!
 * \brief Puts message into the shared memory
 *
 * \param[in]  msg              PSA message to be forwarded
 * \param[out] forward_params   PSA client parameters to be forwarded (pointers
 *                              of the shared input and output vectors shall be
 *                              written back to this structure.
 *
 * \return Returns values as specified by the \ref psa_status_t
 */
psa_status_t psa_proxy_put_msg_into_shared_mem(
        const psa_msg_t *msg,
        struct psa_client_params_t *forward_params);

/*!
 * \brief Writes back the results of the forwarded PSA message
 *
 * \param[in]  msg  Original PSA message was already forwarded
 */
void psa_proxy_write_back_results_from_shared_mem(const psa_msg_t *msg);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_PROXY_SHARED_MEM_MNGR_H__ */
