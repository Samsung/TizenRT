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
 * \file ecdh_alt.h
 *
 * \brief The Elliptic Curve Diffie-Hellman (ECDH) protocol APIs.
 *
 * ECDH is an anonymous key agreement protocol allowing two parties to
 * establish a shared secret over an insecure channel. Each party must have an
 * elliptic-curve public–private key pair.
 *
 * For more information, see <em>NIST SP 800-56A Rev. 2: Recommendation for
 * Pair-Wise Key Establishment Schemes Using Discrete Logarithm
 * Cryptography</em>.
 */
/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
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
 *  This file is part of Mbed TLS (https://tls.mbed.org)
 */

#ifndef MBEDTLS_ECDH_ALT_H
#define MBEDTLS_ECDH_ALT_H

#include "ecp.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT) || defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
#include "alt/common.h"

/*
 * ECDH Error codes
 */
#define MBEDTLS_ERR_ECDH_BAD_INPUT_DATA                    -0x4080  /**< Bad input parameters. */
#define MBEDTLS_ERR_ECDH_HW_ACCEL_FAILED                   -0x4500  /**< ECDH hardware accelerator failed. */

#endif

#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
int mbedtls_ecp_gen_keypair_alt( mbedtls_ecp_group *grp, mbedtls_mpi *d, 
							mbedtls_ecp_point *Q );
int mbedtls_generate_key_alt( unsigned int key_type, unsigned char *key_buf,
							unsigned int key_len, unsigned int pukey_e );
int mbedtls_get_ecc_publickey_alt( struct sECC_KEY *ecc_pub, unsigned char *key_buf,
							unsigned int object_id );
#endif

#if defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
int mbedtls_ecdh_compute_shared_alt( mbedtls_ecp_group *grp,
							mbedtls_mpi *z, const mbedtls_ecp_point *Q );
int mbedtls_compute_ecdh_param_alt( struct sECC_KEY *ecc_pub, unsigned char *key_buf,
							unsigned char *output, unsigned int *olen );
#endif

#ifdef __cplusplus
}
#endif

#endif /* ecdh_alt.h */
