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
 * \file dhm_alt.h
 *
 * \brief Diffie-Hellman-Merkle key exchange.
 *
 * <em>RFC-3526: More Modular Exponential (MODP) Diffie-Hellman groups for
 * Internet Key Exchange (IKE)</em> defines a number of standardized
 * Diffie-Hellman groups for IKE.
 *
 * <em>RFC-5114: Additional Diffie-Hellman Groups for Use with IETF
 * Standards</em> defines a number of standardized Diffie-Hellman
 * groups that can be used.
 *
 * \warning  The security of the DHM key exchange relies on the proper choice
 *           of prime modulus - optimally, it should be a safe prime. The usage
 *           of non-safe primes both decreases the difficulty of the underlying
 *           discrete logarithm problem and can lead to small subgroup attacks
 *           leaking private exponent bits when invalid public keys are used
 *           and not detected. This is especially relevant if the same DHM
 *           parameters are reused for multiple key exchanges as in static DHM,
 *           while the criticality of small-subgroup attacks is lower for
 *           ephemeral DHM.
 *
 * \warning  For performance reasons, the code does neither perform primality
 *           nor safe primality tests, nor the expensive checks for invalid
 *           subgroups. Moreover, even if these were performed, non-standardized
 *           primes cannot be trusted because of the possibility of backdoors
 *           that can't be effectively checked for.
 *
 * \warning  Diffie-Hellman-Merkle is therefore a security risk when not using
 *           standardized primes generated using a trustworthy ("nothing up
 *           my sleeve") method, such as the RFC 3526 / 7919 primes. In the TLS
 *           protocol, DH parameters need to be negotiated, so using the default
 *           primes systematically is not always an option. If possible, use
 *           Elliptic Curve Diffie-Hellman (ECDH), which has better performance,
 *           and for which the TLS protocol mandates the use of standard
 *           parameters.
 *
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

#ifndef MBEDTLS_DHM_ALT_H
#define MBEDTLS_DHM_ALT_H


#include "mbedtls/build_info.h"
#include "../bignum.h"
#include "common.h"

/*
 * DHM Error codes
 */
#define MBEDTLS_ERR_DHM_HW_ACCEL_FAILED                   -0x3500  /**< DHM hardware accelerator failed. */

#define MBEDTLS_DHM_KEY_INDEX_ALT 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          The DHM context structure.
 */
typedef struct {
	size_t len;					/*!<  The size of \p P in Bytes. */
	mbedtls_mpi P;				/*!<  The prime modulus. */
	mbedtls_mpi G;				/*!<  The generator. */
	mbedtls_mpi X;				/*!<  Our secret value. */
	mbedtls_mpi GX;				/*!<  Our public key = \c G^X mod \c P. */
	mbedtls_mpi GY;				/*!<  The public key of the peer = \c G^Y mod \c P. */
	mbedtls_mpi K;				/*!<  The shared secret = \c G^(XY) mod \c P. */
	mbedtls_mpi RP;				/*!<  The cached value = \c R^2 mod \c P. */
	mbedtls_mpi Vi;				/*!<  The blinding value. */
	mbedtls_mpi Vf;				/*!<  The unblinding value. */
	mbedtls_mpi pX;				/*!<  The previous \c X. */
	unsigned int key_index;
} mbedtls_dhm_context;

#ifdef __cplusplus
}
#endif
#endif							/* dhm_alt.h */
