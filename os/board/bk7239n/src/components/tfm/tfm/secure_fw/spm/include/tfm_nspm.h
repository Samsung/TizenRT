/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_NSPM_H__
#define __TFM_NSPM_H__

#include <stdint.h>

#define TFM_NS_CLIENT_INVALID_ID            ((int32_t)0)

#if !defined(TFM_MULTI_CORE_TOPOLOGY)
/*
 * The macro cmse_nsfptr_create defined in the gcc library uses the non-standard
 * gcc C lanuage extension 'typeof'. TF-M is built with '-std=c99' so typeof
 * cannot be used in the code. As a workaround cmse_nsfptr_create is redefined
 * here to use only standard language elements.
 */
#undef cmse_nsfptr_create
#define cmse_nsfptr_create(p) ((intptr_t) (p) & ~1)

/*!
 * \def __tfm_nspm_secure_gateway_attributes__
 *
 * \brief Attributes for secure gateway functions for NSPM
 */
#if !defined(__ARMCC_VERSION) && !defined(__ICCARM__)
/*
 * GNUARM requires noclone attribute to protect gateway function symbol from
 * being renamed and cloned
 */
#define __tfm_nspm_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, noclone))
#else
#define __tfm_nspm_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry))
#endif /* !__ARMCC_VERSION */
#endif /* __GNUC__ && !TFM_MULTI_CORE_TOPOLOGY */

/**
 * \brief initialise the NS context database
 */
void tfm_nspm_ctx_init(void);

/**
 * \brief Get the client ID of the current NS client
 *
 * \return The client id of the current NS client. 0 (invalid client id) is
 *         returned in case of error.
 */
int32_t tfm_nspm_get_current_client_id(void);

#endif /* __TFM_NSPM_H__ */
