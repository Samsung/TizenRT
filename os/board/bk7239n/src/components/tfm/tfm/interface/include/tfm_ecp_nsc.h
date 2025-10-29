// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/** This function performs a scalar multiplication of a point, provide hardware acceleration for non_security word
 *                  by an integer: \p R = \p m * \p P.    
 *
 *                  It is not thread-safe to use same group in multiple threads.
 *
 * \param id       The ECP curve to use.
 *                  This must be initialized and have group parameters
 *                  set, for example through mbedtls_ecp_group_load().
 * \param R         The point in which to store the result of the calculation.
 *                  This must be initialized.
 * \param m         The integer by which to multiply. This must be initialized.
 * \param P         The point to multiply. This must be initialized.
 
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_ECP_INVALID_KEY if \p m is not a valid private
 *                  key, or \p P is not a valid public key.
 * \return          #MBEDTLS_ERR_MPI_ALLOC_FAILED on memory-allocation failure.
 * \return          Another negative error code on other kinds of failure.
 */
typedef int32_t psa_status_t;

typedef struct {
	size_t olen;
	unsigned char buf[MBEDTLS_ECP_MAX_BYTES];
}mpi_binary_t;

typedef struct {
	size_t olen;
	unsigned char buf[MBEDTLS_ECP_MAX_PT_LEN];
}ecp_binary_t;

psa_status_t psa_mbedtls_ecp_mul_nsc(mbedtls_ecp_group_id id, ecp_binary_t *R, 
								mpi_binary_t *m, ecp_binary_t *P);

psa_status_t psa_ecp_mod_nsc( mbedtls_ecp_group_id id, mpi_binary_t *m );

#ifdef __cplusplus
}
#endif
