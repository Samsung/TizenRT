/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_FWU_REQ_MNGR_H__
#define __TFM_FWU_REQ_MNGR_H__

#include <stddef.h>

#include "psa/client.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The maxinum of images that TF-M support to update concurrently. */
#define TFM_FWU_MAX_IMAGES    4

/**
 * \brief Handles the write request.
 *
 * \param[in]  in_vec  Pointer to the input vector which contains the input
 *                     parameters.
 * \param[in]  in_len  Number of input parameters in the input vector.
 * \param[out] out_vec Pointer to the output vector which contains the output
 *                     parameters.
 * \param[in]  out_len Number of output parameters in the output vector.
 *
 * \return A status indicating the success/failure of the operation as
 *         specified in \ref psa_status_t
 */
psa_status_t tfm_fwu_write_req(psa_invec *in_vec, size_t in_len,
                               psa_outvec *out_vec, size_t out_len);

/**
 * \brief Handles the install request.
 *
 * \param[in]  in_vec  Pointer to the input vector which contains the input
 *                     parameters.
 * \param[in]  in_len  Number of input parameters in the input vector.
 * \param[out] out_vec Pointer to the output vector which contains the output
 *                     parameters.
 * \param[in]  out_len Number of output parameters in the output vector.
 *
 * \return A status indicating the success/failure of the operation as
 *         specified in \ref psa_status_t
 */
psa_status_t tfm_fwu_install_req(psa_invec *in_vec, size_t in_len,
                               psa_outvec *out_vec, size_t out_len);

/**
 * \brief Handles the abort request.
 *
 * \param[in]  in_vec  Pointer to the input vector which contains the input
 *                     parameters.
 * \param[in]  in_len  Number of input parameters in the input vector.
 * \param[out] out_vec Pointer to the output vector which contains the output
 *                     parameters.
 * \param[in]  out_len Number of output parameters in the output vector.
 *
 * \return A status indicating the success/failure of the operation as
 *         specified in \ref psa_status_t
 */
psa_status_t tfm_fwu_abort_req(psa_invec *in_vec, size_t in_len,
                               psa_outvec *out_vec, size_t out_len);

/**
 * \brief Query the infor of the primary slot.
 *
 * \param[in]  in_vec  Pointer to the input vector which contains the input
 *                     parameters.
 * \param[in]  in_len  Number of input parameters in the input vector.
 * \param[out] out_vec Pointer to the output vector which contains the output
 *                     parameters.
 * \param[in]  out_len Number of output parameters in the output vector.
 *
 * \return A status indicating the success/failure of the operation as
 *         specified in \ref psa_status_t
 */

psa_status_t tfm_fwu_query_req(psa_invec *in_vec, size_t in_len,
                               psa_outvec *out_vec, size_t out_len);

psa_status_t tfm_fwu_request_reboot_req(psa_invec *in_vec, size_t in_len,
                               psa_outvec *out_vec, size_t out_len);

#ifdef __cplusplus
}
#endif
#endif /* __TFM_FWU_REQ_MNGR_H__ */
