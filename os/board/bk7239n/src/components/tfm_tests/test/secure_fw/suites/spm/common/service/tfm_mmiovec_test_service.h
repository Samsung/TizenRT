/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_MMIOVEC_TEST_SERVICE_H__
#define __TFM_MMIOVEC_TEST_SERVICE_H__

#include "psa/error.h"
#include "psa/service.h"

/**
 * \brief Mapping input vectors and unmapping them.
 *
 * \param[in] msg  Pointer to the PSA message body
 *
 * \return Returns \ref TFM_SUCCESS on success.
 */
psa_status_t test_service_mmiovec_invec(const psa_msg_t *msg);

/**
 * \brief Mapping output vectors and unmapping them.
 *
 * \param[in] msg  Pointer to the PSA message body
 *
 * \return Returns \ref TFM_SUCCESS on success.
 */
psa_status_t test_service_mmiovec_outvec(const psa_msg_t *msg);

/**
 * \brief Mapping input vectors and not unmapping them.
 *
 * \param[in] msg  Pointer to the PSA message body
 *
 * \return Returns \ref TFM_SUCCESS on success.
 */
psa_status_t test_service_outvec_not_unmap(const psa_msg_t *msg);

#endif /* __TFM_MMIOVEC_TEST_SERVICE_H__ */
