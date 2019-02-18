/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
/**
 * \file pk.h
 *
 * \brief Public Key abstraction layer
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#ifndef MBEDTLS_PK_ALT_H
#define MBEDTLS_PK_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "../config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "../md.h"

#if defined(MBEDTLS_RSA_C)
#include "../rsa.h"
#endif

#if defined(MBEDTLS_ECP_C)
#include "../ecp.h"
#endif

#if defined(MBEDTLS_ECDSA_C)
#include "../ecdsa.h"
#endif

#include "common.h"

#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && \
    !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif


#ifdef __cplusplus
extern "C" {
#endif

int mbedtls_get_ecdsa_signature_alt( struct mbedtls_sECC_SIGN *ecc_sign, unsigned char *hash,
				unsigned int hash_len, unsigned int key_index );

int mbedtls_get_rsa_signature_alt( struct mbedtls_sRSA_SIGN *rsa_sign, unsigned char *hash,
				unsigned int hash_len, unsigned int key_index );

int mbedtls_rsa_decryption_alt( unsigned int key_index, unsigned int pad_type,
				unsigned char *output, unsigned int *outlen,
				unsigned char *input, unsigned int inlen );

#if defined(MBEDTLS_PK_RSA_VERIFY_ALT)
int mbedtls_verify_rsa_signature_alt( struct mbedtls_sRSA_SIGN *rsa_sign,
				unsigned char *hash, unsigned int hash_len,
				unsigned char *key_buf );
#endif

#if defined(MBEDTLS_PK_RSA_ENCRYPT_ALT)
int mbedtls_rsa_encryption_alt( unsigned char *key_buf, unsigned int pad_type,
				unsigned char *output, unsigned int *outlen,
				unsigned char *input, unsigned int inlen );
#endif

#if defined(MBEDTLS_PK_ECDSA_VERIFY_ALT)
int mbedtls_verify_ecdsa_signature_alt( struct mbedtls_sECC_SIGN *ecc_sign,
                                        unsigned char *hash, unsigned int hash_len,
                                        unsigned char *key_buf );
#endif

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_PK_ALT_H */
