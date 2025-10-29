/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PLATFORM_SP_H__
#define __PLATFORM_SP_H__

#include "tfm_platform_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initializes the secure partition.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t platform_sp_init(void);

/*!
 * \brief Resets the system.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t platform_sp_system_reset(void);

/*!
 * \brief Performs pin services of the platform
 *
 * \param[in]     in_vec     Pointer to in_vec array, which contains input
 *                           arguments for the pin service
 * \param[in]     num_invec  Number of elements in in_vec array
 * \param[in,out] out_vec    Pointer out_vec array, which contains output data
 *                           of the pin service
 * \param[in]     num_outvec Number of elements in out_vec array
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t
platform_sp_pin_service(const psa_invec  *in_vec,  uint32_t num_invec,
                        const psa_outvec *out_vec, uint32_t num_outvec);

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_SP_H__ */
