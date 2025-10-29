/**
 * \file bignum.h
 *
 * \brief Multi-precision integer library
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
#ifndef MBEDTLS_BIGNUM_WRAP_H
#define MBEDTLS_BIGNUM_WRAP_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <stddef.h>
#include <stdint.h>

#include "mbedtls/bignum.h"
#ifdef __cplusplus
extern "C" {
#endif

int mbedtls_mpi_get_bit_wrap( const mbedtls_mpi *X, size_t pos );
size_t mbedtls_mpi_bitlen_wrap( const mbedtls_mpi *X );

/**
 * \brief          Return the total size in bytes
 *
 * \param X        MPI to use
 */
size_t mbedtls_mpi_size_wrap( const mbedtls_mpi *X );

/**
 * \brief          Compare signed values
 *
 * \param X        Left-hand MPI
 * \param Y        Right-hand MPI
 *
 * \return         1 if X is greater than Y,
 *                -1 if X is lesser  than Y or
 *                 0 if X is equal to Y
 */
int mbedtls_mpi_cmp_mpi_wrap( const mbedtls_mpi *X, const mbedtls_mpi *Y );

/**
 * \brief          Compare signed values
 *
 * \param X        Left-hand MPI
 * \param z        The integer value to compare to
 *
 * \return         1 if X is greater than z,
 *                -1 if X is lesser  than z or
 *                 0 if X is equal to z
 */
int mbedtls_mpi_cmp_int_wrap( const mbedtls_mpi *X, mbedtls_mpi_sint z );

/**
 * \brief          Copy the contents of Y into X
 *
 * \param X        Destination MPI. It is enlarged if necessary.
 * \param Y        Source MPI.
 *
 * \return         0 if successful,
 *                 MBEDTLS_ERR_MPI_ALLOC_FAILED if memory allocation failed
 */
int mbedtls_mpi_copy_wrap( mbedtls_mpi *X, const mbedtls_mpi *Y );

/**
 * \brief          Import from an ASCII string
 *
 * \param X        Destination MPI
 * \param radix    Input numeric base
 * \param s        Null-terminated string buffer
 *
 * \return         0 if successful, or a MBEDTLS_ERR_MPI_XXX error code
 */
int mbedtls_mpi_read_string_wrap( mbedtls_mpi *X, int radix, const char *s );

#ifdef __cplusplus
}
#endif

#endif /* bignum_wrap.h */
