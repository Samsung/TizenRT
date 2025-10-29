/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRYPTO_HW_H__
#define __CRYPTO_HW_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \brief Initialize the CC312 crypto accelerator
 *
 * \return 0 on success, non-zero otherwise
 */
int crypto_hw_accelerator_init(void);

/**
 * \brief Deallocate the CC312 crypto accelerator
 *
 * \return 0 on success, non-zero otherwise
 */
int crypto_hw_accelerator_finish(void);

/*
 * \brief  This function performs key derivation
 *
 * \param[in]  label         Label for KDF
 * \param[in]  label_size    Size of the label
 * \param[in]  context       Context for KDF
 * \param[in]  context_size  Size of the context
 * \param[out] key           Buffer to output the derived key material
 * \param[in]  key_size      Requested size of the derived key material and
 *                           minimum size of the key buffer
 *
 * \return 0 on success, non-zero otherwise
 */
int crypto_hw_accelerator_huk_derive_key(const uint8_t *label,
                                         size_t label_size,
                                         const uint8_t *context,
                                         size_t context_size,
                                         uint8_t *key,
                                         size_t key_size);
/**
 * \brief Apply permissions on debug signals
 *
 * \param[in]   permissions_mask   permission vector for debug signals
 *                                 vector bits interpretation is specific
 *                                 to a target and depends on the architecture
 * \param[in]   len                length of permission vector
 *
 * \return 0 on success, non-zero otherwise
 */
int crypto_hw_apply_debug_permissions(uint8_t *permissions_mask, uint32_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRYPTO_HW_H__ */
