/*
 * Copyright (c) 2001-2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef POLY_H
#define POLY_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include <stdint.h>
#include "cc_error.h"
#include "mbedtls_cc_poly.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! The POLY block size in 32-bit words */
#define CC_POLY_BLOCK_SIZE_IN_WORDS  4
#define CC_POLY_BLOCK_SIZE_IN_BYTES  (CC_POLY_BLOCK_SIZE_IN_WORDS*CC_32BIT_WORD_SIZE)

#define POLY_PRIME_SIZE_IN_BITS   130
#define POLY_PRIME_SIZE_IN_WORDS  CALC_FULL_32BIT_WORDS(POLY_PRIME_SIZE_IN_BITS)

/*! The POLY PKA registers size in 32-bit words */
#define CC_POLY_PKA_REG_SIZE_IN_PKA_WORDS  4
#define CC_POLY_PKA_REG_SIZE_IN_WORDS  (CC_POLY_PKA_REG_SIZE_IN_PKA_WORDS * (CALC_FULL_32BIT_WORDS(CC_PKA_WORD_SIZE_IN_BITS)))
#define CC_POLY_PKA_REG_SIZE_IN_BYTES  (CC_POLY_PKA_REG_SIZE_IN_WORDS*CC_32BIT_WORD_SIZE)

/**
 * PKA register contexts. Between multipart calls, the PKA engine needs to save
 * and restore the register context. It's composed of the clamped key pair
 * (r,s) 256 bit long and the value of the accumulator register which is mod P,
 * where P is 2^130-5, which in full words is 160 bit long, 5 32-bit words.
 */
typedef struct PolyPkaContext {
    uint32_t key[8]; /*!< (r,s) concatenated with r already clamped */
    uint32_t acc[5]; /*!< Value of the accumulator modulus P, i.e. [0,2^130-5)*/
} PolyPkaContext_t;

/**
 * State information required to support multipart APIs in AEAD for MAC
 * computation. As Poly1305 operates on CC_POLY_BLOCK_SIZE_IN BYTES of data
 * it needs to cache up to CC_POLY_BLOCK_SIZE_IN_BYTES-1 of the input. But
 * for practical reasons (i.e. working on 4-byte aligned buffers) we store an
 * entire block of 16 bytes that can be processed in one go without additional
 * copies
 */
typedef struct PolyState {
    uint32_t msg_state[CC_POLY_BLOCK_SIZE_IN_WORDS]; /*!< Equals 16 bytes of
                                                      *   data
                                                      */
    uint8_t msg_state_size;  /*!< Size of the message buffered in msg_state */
    PolyPkaContext_t context; /*!< PKA registers context (clamped key, acc) */
} PolyState_t;

/**
 * @brief Generates the POLY mac according to RFC 7539 section 2.5.1
 *
 * @param[in]  key            Pointer to 256 bits of KEY.
 * @param[in]  pAddData       Optional - pointer to additional data if any
 * @param[in]  addDataSize    The size of the additional data
 * @param[in]  pDataIn        Pointer to data buffer to calculate MAC on
 * @param[in]  dataInSize     The size of the additional data
 * @param[out] macRes         The calculated MAC
 * @param[in]  isPolyAeadMode Boolean indicating if the Poly MAC operation is
 *                            part of AEAD or just poly. In AEAD mode, the
 *                            last block of AddData and DataIn accumulation
 *                            will be padded with zeros up to 16 bytes as per
 *                            the AEAD spec construction on RFC7539.
 *
 * @return  CC_OK On success, otherwise indicates failure
 */
CCError_t PolyMacCalc(mbedtls_poly_key key,
                      const uint8_t *pAddData,
                      size_t addDataSize,
                      const uint8_t *pDataIn,
                      size_t dataInSize,
                      mbedtls_poly_mac macRes,
                      bool isPolyAeadMode);

/**
 * @brief Initialises a multipart authentication.
 *
 * @param[in,out] state    Pointer to the state associated to the operation.
 * @param[in,out] key      Buffer containing the (r,s) keypair.
 * @param[in]     key_size Size in bytes of the key. Must be 32 bytes.
 *
 * @return It returns CC_OK on success, or an error code otherwise
 */
CCError_t PolyInit(PolyState_t *state, const uint8_t *key, size_t key_size);

/**
 * @brief Updates a pre-initialised multipart authentication with a new
 *        chunk of data to be authenticated.
 *
 * @param[in,out] state      Pointer to the state associated to the operation.
 * @param[in]     data       Buffer containing the data to be authenticated.
 * @param[in]     data_size  Size in bytes of the data to be authenticated.
 * @param[in] isPolyAeadMode Boolean indicating if the update is the last
 *                           of an AEAD operation. In that case, it will pad
 *                           the last partial block with zeros to make a
 *                           full block as specified on RFC7539.
 *
 * @return It returns CC_OK on success, or an error code otherwise
 */
CCError_t PolyUpdate(PolyState_t *state,
                     const uint8_t *data,
                     size_t data_size,
                     bool isPolyAeadMode);

/**
 * @brief Finalises a pre-initialised multipart authentication outputting
 *        the 16 bytes that make the Poly1305 tag.
 *
 * @param[in,out] state    Pointer to the state associated to the operation.
 * @param[out]    tag      Buffer that will contain the 16 bytes of the tag.
 * @param[in]     tag_size Size in bytes ot the tag buffer. It needs to be
 *                         at least 16 bytes, i.e. CC_POLY_BLOCK_SIZE_IN_BYTES
 *
 * @return It returns CC_OK on success, or an error code otherwise
 */
CCError_t PolyFinish(PolyState_t *state, uint8_t *tag, size_t tag_size);

#ifdef __cplusplus
}
#endif

#endif  /* POLY_H */
