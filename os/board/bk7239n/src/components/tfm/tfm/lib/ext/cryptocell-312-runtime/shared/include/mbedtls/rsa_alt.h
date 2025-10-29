/*
 * Copyright (c) 2001-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_RSA_ALT_H
#define MBEDTLS_RSA_ALT_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#endif

#if defined (MBEDTLS_RSA_ALT)

// Regular implementation
//

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   The RSA context structure.
 *
 * \note    Direct manipulation of the members of this structure
 *          is deprecated. All manipulation should instead be done through
 *          the public interface functions.
 */
typedef struct
{
    int MBEDTLS_PRIVATE(ver);                    /*!<  always 0          */
    size_t MBEDTLS_PRIVATE(len);                 /*!<  size(N) in chars  */

    mbedtls_mpi MBEDTLS_PRIVATE(N);                      /*!<  public modulus    */
    mbedtls_mpi MBEDTLS_PRIVATE(E);                      /*!<  public exponent   */

    mbedtls_mpi MBEDTLS_PRIVATE(D);                      /*!<  private exponent  */
    mbedtls_mpi MBEDTLS_PRIVATE(P);                      /*!<  1st prime factor  */
    mbedtls_mpi MBEDTLS_PRIVATE(Q);                      /*!<  2nd prime factor  */

    mbedtls_mpi MBEDTLS_PRIVATE(DP);                     /*!<  D % (P - 1)       */
    mbedtls_mpi MBEDTLS_PRIVATE(DQ);                     /*!<  D % (Q - 1)       */
    mbedtls_mpi MBEDTLS_PRIVATE(QP);                     /*!<  1 / (Q % P)       */

    mbedtls_mpi MBEDTLS_PRIVATE(RN);                     /*!<  cached R^2 mod N  */

    mbedtls_mpi MBEDTLS_PRIVATE(RP);                     /*!<  cached R^2 mod P  */
    mbedtls_mpi MBEDTLS_PRIVATE(RQ);                     /*!<  cached R^2 mod Q  */

    mbedtls_mpi MBEDTLS_PRIVATE(Vi);                     /*!<  cached blinding value     */
    mbedtls_mpi MBEDTLS_PRIVATE(Vf);                     /*!<  cached un-blinding value  */

    mbedtls_mpi MBEDTLS_PRIVATE(NP);                     /*!< Barrett mod N tag NP for N-modulus */
    mbedtls_mpi MBEDTLS_PRIVATE(BQP);                    /*!< Barrett mod Q tag QP for Q-factor  */
    mbedtls_mpi MBEDTLS_PRIVATE(BPP);                    /*!< Barrett mod P tag PP for P-factor  */

    int MBEDTLS_PRIVATE(padding);                /*!<  MBEDTLS_RSA_PKCS_V15 for 1.5 padding and
                                      MBEDTLS_RSA_PKCS_v21 for OAEP/PSS         */
    int MBEDTLS_PRIVATE(hash_id);                /*!<  Hash identifier of mbedtls_md_type_t as
                                      specified in the mbedtls_md.h header file
                                      for the EME-OAEP and EMSA-PSS
                                      encoding                          */
#if defined(MBEDTLS_THREADING_C)
    mbedtls_threading_mutex_t MBEDTLS_PRIVATE(mutex);    /*!<  Thread-safety mutex       */
#endif
}
mbedtls_rsa_context;

#ifdef __cplusplus
}
#endif

#endif /*  MBEDTLS_RSA_ALT  */

#endif /*  MBEDTLS_RSA_ALT_H  */
