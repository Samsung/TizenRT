/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/// @file see/see_internal.h
/// @brief SEE api for using in TLS internal.

#ifndef __SEE_INTERNAL_H
#define __SEE_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "mbedtls/see_api.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SEE_MAX_ENCRYPTED_KEY_SIZE  (1280)
#define SEE_MAX_ECP_KEY_SIZE  (68)

#define DHM_1024 (128)
#define DHM_2048 (256)

/****************************************************************************
 * Public types
****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#if defined(CONFIG_HW_ECDH_PARAM)
int see_generate_key_internal(unsigned int key_type, unsigned char *key_buf,
                              unsigned int key_len, unsigned int pukey_e);

int see_get_ecc_publickey_internal(struct sECC_KEY *ecc_pub, unsigned char *key_buf,
                                   unsigned int object_id);

int see_compute_ecdh_param_internal(struct sECC_KEY *ecc_pub, unsigned char *key_buf,
                                    unsigned char *output, unsigned int *olen);
#endif /* CONFIG_HW_ECDH_PARAM */

#if defined(CONFIG_HW_DH_PARAM)
int see_supported_dhm_size(int size);

int see_generate_dhm_params_internal(struct sDH_PARAM *d_param, unsigned char *key_buf);


int see_compute_dhm_param_internal(struct sDH_PARAM *d_param, unsigned char *key_buf,
                                   unsigned char *output, unsigned int *olen);
#endif /* CONFIG_HW_DH_PARAM */

#if defined(CONFIG_HW_ECDSA_VERIFICATION)
int see_setup_key_internal(unsigned char *key_der, unsigned int key_len,
                           unsigned int key_type, unsigned char *key_buf);

int see_verify_ecdsa_signature_internal(struct sECC_SIGN *ecc_sign,
                                        unsigned char *hash, unsigned int hash_len,
                                        unsigned char *key_buf);
#endif /* CONFIG_HW_ECDSA_VERIFICATION */
#endif /* __SEE_INTERNAL_H */
