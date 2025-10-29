/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_rsa_util.c
 *
 * This file contains the implementation of the internal utility functions used
 * to manipulate RSA types and key formats
 *
 */

#include "cc3xx_internal_rsa_util.h"
#include "cc3xx_internal_asn1_util.h"

#include "cc_common.h"
#include "cc_rnd_error.h"
#include "cc_rsa_build.h"
#include "cc_rsa_error.h"
#include "cc_rsa_types.h"
#include "psa/crypto.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

/* FixMe: Currently, some parts of the low-level driver are
 *        are not built at all based on the mbed TLS configuration,
 *        hence they can't be called from the interface code.
 *        Eventually, the low level driver should be made
 *        independent of the mbed TLS configuration and the
 *        interface layer should be the only part that should
 *        be configured through defines
 */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* FixMe: Temporary way of bridging mbed TLS based configuration
 *        with specific driver configuration defines
 */
#ifndef CC3XX_CONFIG_SUPPORT_RSA
#define CC3XX_CONFIG_SUPPORT_RSA
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
#ifndef MBEDTLS_RSA_C
#undef CC3XX_CONFIG_SUPPORT_RSA
#endif


/*  This function checks if the first byte of a pointer is zero and
 *  skips it when it is. We need this when we convert a PSA RSA key
 *  to the CryptoCell internal type because the PSA key will use the
 *  DER encoding to store its value. The DER encoding is using 2's
 *  complement representation when storing integeters which will add
 *  a leading zero byte when the MSB of a number is 1. CryptoCell
 *  does not allows this form so the leading zero needs to be removed.
 */
static void skip_leading_zero(uint8_t **ptr, size_t *ptr_size)
{
    if (ptr == NULL || ptr_size == NULL) {
        return;
    }

    if (*ptr == NULL) {
        return;
    }

    if ((*ptr)[0] == 0 && ((*ptr)[1] & 0x80)) {
        (*ptr)++;
        *ptr_size -= 1;
    }
}

/** \defgroup internal_rsa_util Internal RSA utility functions
 *
 *  Internal functions required to provide utilities for handling RSA type
 *  conversions
 *
 *  @{
 */
psa_status_t cc3xx_rsa_psa_pub_to_cc_pub(const uint8_t *psa_pub_key_buffer,
                                         size_t psa_pub_key_buffer_size,
                                         CCRsaUserPubKey_t *UserPubKey_ptr)

{
#ifndef CC3XX_CONFIG_SUPPORT_RSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint8_t *pub_key_buffer_start_pnt = (uint8_t *)psa_pub_key_buffer;
    uint8_t **pub_key_buffer_start = &pub_key_buffer_start_pnt;
    uint8_t *pub_key_buffer_end =
        (uint8_t *)psa_pub_key_buffer + psa_pub_key_buffer_size;
    uint8_t *n_ptr;
    size_t n_len;
    uint8_t *e_ptr;
    size_t e_len;
    size_t len;
    int ret;
    CCError_t cc_err = CC_FAIL;

    /* Move the pointer after the sequence */
    ret = cc3xx_asn1_get_tag(pub_key_buffer_start, pub_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_CONSTRUCTED |
                                 CC3XX_TAG_ASN1_SEQUENCE);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    /* Get the modulus n */
    ret = cc3xx_asn1_get_tag(pub_key_buffer_start, pub_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    n_ptr = *pub_key_buffer_start;
    n_len = len;
    skip_leading_zero(&n_ptr, &n_len);

    *pub_key_buffer_start += len;

    /* Get the exponent e */
    ret = cc3xx_asn1_get_tag(pub_key_buffer_start, pub_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    e_ptr = *pub_key_buffer_start;
    e_len = len;
    skip_leading_zero(&e_ptr, &e_len);

    *pub_key_buffer_start += len;

    cc_err = CC_RsaPubKeyBuild(UserPubKey_ptr, e_ptr, e_len, n_ptr, n_len);

end:
    return cc3xx_rsa_cc_error_to_psa_error(cc_err);
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
}

psa_status_t cc3xx_rsa_psa_priv_to_cc_pub(const uint8_t *psa_priv_key_buffer,
                                          size_t psa_priv_key_buffer_size,
                                          CCRsaUserPubKey_t *UserPubKey_ptr)

{
#ifndef CC3XX_CONFIG_SUPPORT_RSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint8_t *priv_key_buffer_start_pnt = (uint8_t *)psa_priv_key_buffer;
    uint8_t **priv_key_buffer_start = &priv_key_buffer_start_pnt;
    uint8_t *priv_key_buffer_end =
        (uint8_t *)psa_priv_key_buffer + psa_priv_key_buffer_size;
    uint8_t *n_ptr;
    size_t n_len;
    uint8_t *e_ptr;
    size_t e_len;
    size_t len;
    int dummy;
    int ret;
    CCError_t cc_err = CC_FAIL;

    /* Move the pointer after the sequence */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_CONSTRUCTED |
                                 CC3XX_TAG_ASN1_SEQUENCE);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    /* Move the pointer after the version */
    ret = cc3xx_asn1_get_int(priv_key_buffer_start, priv_key_buffer_end, &dummy);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    /* Get the modulus n */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    n_ptr = *priv_key_buffer_start;
    n_len = len;
    skip_leading_zero(&n_ptr, &n_len);

    *priv_key_buffer_start += len;

    /* Get the exponent e */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    e_ptr = *priv_key_buffer_start;
    e_len = len;
    skip_leading_zero(&e_ptr, &e_len);

    *priv_key_buffer_start += len;

    cc_err = CC_RsaPubKeyBuild(UserPubKey_ptr, e_ptr, e_len, n_ptr, n_len);

end:
    return cc3xx_rsa_cc_error_to_psa_error(cc_err);
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
}

psa_status_t cc3xx_rsa_psa_priv_to_cc_priv(const uint8_t *psa_priv_key_buffer,
                                           size_t psa_priv_key_buffer_size,
                                           CCRsaUserPrivKey_t *UserPrivKey_ptr)

{
#ifndef CC3XX_CONFIG_SUPPORT_RSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint8_t *p_ptr;
    size_t p_len;
    uint8_t *q_ptr;
    size_t q_len;
    uint8_t *dP_ptr;
    size_t dP_len;
    uint8_t *dQ_ptr;
    size_t dQ_len;
    uint8_t *qInv_ptr;
    size_t qInv_len;
    int ret;
    size_t len;
    int dummy;
    CCError_t cc_err = CC_FAIL;

    uint8_t *priv_key_buffer_start_pnt = (uint8_t *)psa_priv_key_buffer;
    uint8_t **priv_key_buffer_start = &priv_key_buffer_start_pnt;
    uint8_t *priv_key_buffer_end =
        (uint8_t *)psa_priv_key_buffer + psa_priv_key_buffer_size;

    /* Move the pointer after the sequence */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_CONSTRUCTED |
                                 CC3XX_TAG_ASN1_SEQUENCE);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    /* Move the pointer after the version */
    ret =
        cc3xx_asn1_get_int(priv_key_buffer_start, priv_key_buffer_end, &dummy);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    /* Move the pointer after the modulus n */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }
    *priv_key_buffer_start += len;

    /* Move the pointer after the publicExponent e*/
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }
    *priv_key_buffer_start += len;

    /* Move the pointer after the privateExponent d */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }
    *priv_key_buffer_start += len;

    /* Get the P */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    p_ptr = *priv_key_buffer_start;
    p_len = len;
    skip_leading_zero(&p_ptr, &p_len);

    *priv_key_buffer_start += len;

    /* Get the Q */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    q_ptr = *priv_key_buffer_start;
    q_len = len;
    skip_leading_zero(&q_ptr, &q_len);

    *priv_key_buffer_start += len;

    /* Get the dP */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    dP_ptr = *priv_key_buffer_start;
    dP_len = len;
    skip_leading_zero(&dP_ptr, &dP_len);

    *priv_key_buffer_start += len;

    /* Get the dQ */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    dQ_ptr = *priv_key_buffer_start;
    dQ_len = len;
    skip_leading_zero(&dQ_ptr, &dQ_len);

    *priv_key_buffer_start += len;

    /* Get the dInv */
    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    qInv_ptr = *priv_key_buffer_start;
    qInv_len = len;
    skip_leading_zero(&qInv_ptr, &qInv_len);

    *priv_key_buffer_start += len;

    cc_err = CC_RsaPrivKeyCrtBuild(UserPrivKey_ptr, p_ptr, p_len, q_ptr, q_len,
                                   dP_ptr, dP_len, dQ_ptr, dQ_len, qInv_ptr,
                                   qInv_len);
end:
    return cc3xx_rsa_cc_error_to_psa_error(cc_err);
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
}

psa_status_t cc3xx_rsa_psa_priv_to_psa_publ(uint8_t *priv_key_buffer,
                                            size_t priv_key_buffer_size,
                                            uint8_t *publ_key_buffer,
                                            size_t publ_key_buffer_size,
                                            size_t *publ_key_buffer_length)
{
#ifndef CC3XX_CONFIG_SUPPORT_RSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint8_t *priv_key_buffer_start_pnt = priv_key_buffer;
    uint8_t **priv_key_buffer_start = &priv_key_buffer_start_pnt;
    uint8_t *priv_key_buffer_end = priv_key_buffer + priv_key_buffer_size;
    uint8_t *n_pnt;
    size_t n_len;
    uint8_t *e_pnt;
    size_t e_len;

    uint8_t *pub_key_buffer_end_pnt = publ_key_buffer + publ_key_buffer_size;
    uint8_t **pub_key_buffer_end = &pub_key_buffer_end_pnt;

    CCError_t cc_err = CC_FAIL;
    int bytes_written;
    size_t buffer_used;
    size_t len;
    int ret;
    int dummy;

    /* Initialise the return value to 0 */
    *publ_key_buffer_length = 0;

    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                        CC3XX_TAG_ASN1_CONSTRUCTED | CC3XX_TAG_ASN1_SEQUENCE);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    ret = cc3xx_asn1_get_int(priv_key_buffer_start, priv_key_buffer_end,
                             &dummy);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    n_pnt = *priv_key_buffer_start;
    n_len = len;

    *priv_key_buffer_start += len;

    ret = cc3xx_asn1_get_tag(priv_key_buffer_start, priv_key_buffer_end, &len,
                             CC3XX_TAG_ASN1_INTEGER);
    if (ret < 0) {
        cc_err = CC_FAIL;
        goto end;
    }

    e_pnt = *priv_key_buffer_start;
    e_len = len;

    bytes_written = cc3xx_asn1_write_big_integer(pub_key_buffer_end,
                                                 publ_key_buffer, e_pnt, e_len);
    buffer_used = bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(pub_key_buffer_end,
                                                 publ_key_buffer, n_pnt, n_len);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    bytes_written = cc3xx_asn1_write_len(pub_key_buffer_end, publ_key_buffer,
                                         buffer_used);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    bytes_written = cc3xx_asn1_write_tag(pub_key_buffer_end, publ_key_buffer,
                        CC3XX_TAG_ASN1_SEQUENCE | CC3XX_TAG_ASN1_CONSTRUCTED);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    /* The asn1 functions write to the end of the buffer.
     * Move the data to the beginning and erase remaining data
     * at the original location.
     */
    if (2 * buffer_used <= publ_key_buffer_size) {
        CC_PalMemMove(publ_key_buffer,
                      publ_key_buffer + publ_key_buffer_size - buffer_used,
                      buffer_used);
        CC_PalMemSetZero(publ_key_buffer + publ_key_buffer_size - buffer_used,
                         buffer_used);
        cc_err = CC_OK;
    } else if ((size_t)buffer_used < publ_key_buffer_size) {
        CC_PalMemMove(publ_key_buffer,
                      publ_key_buffer + publ_key_buffer_size - buffer_used,
                      buffer_used);
        CC_PalMemSetZero(publ_key_buffer + buffer_used,
                         publ_key_buffer_size - buffer_used);
        cc_err = CC_OK;
    } else {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    *publ_key_buffer_length = buffer_used;

end:
    return cc3xx_rsa_cc_error_to_psa_error(cc_err);
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
}

psa_status_t cc3xx_rsa_save_der_priv_key(uint8_t *key_buffer,
                                         size_t key_buffer_size, uint32_t *n,
                                         uint32_t *e, uint32_t *d, uint32_t *p,
                                         uint32_t *q, uint32_t *dP, uint32_t *dQ,
                                         uint32_t *qInv, size_t d_size_bytes,
                                         size_t *key_buffer_length)
{
#ifndef CC3XX_CONFIG_SUPPORT_RSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint8_t *key_buffer_end_pnt = key_buffer + key_buffer_size;
    uint8_t **key_buffer_end = &key_buffer_end_pnt;
    CCError_t cc_err = CC_FAIL;
    uint8_t *temp_buff = NULL;
    int bytes_written;
    size_t buffer_used;

    /* Initialise the return value to 0 */
    *key_buffer_length = 0;

    temp_buff = (uint8_t *)mbedtls_calloc(1, d_size_bytes);
    if (temp_buff == NULL) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      qInv, d_size_bytes / 2);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes / 2);
    buffer_used = bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      dQ, d_size_bytes / 2);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes / 2);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      dP, d_size_bytes / 2);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes / 2);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      q, d_size_bytes / 2);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes / 2);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      p, d_size_bytes / 2);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes / 2);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      d, d_size_bytes);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    CC_PalMemCopy(temp_buff, (uint8_t *)e, 3);

    bytes_written =
        cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer, temp_buff, 3);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    cc_err = CC_CommonConvertLswMswWordsToMsbLsbBytes(temp_buff, d_size_bytes,
                                                      n, d_size_bytes);
    if (cc_err != CC_OK) {
        goto end;
    }

    bytes_written = cc3xx_asn1_write_big_integer(key_buffer_end, key_buffer,
                                                 temp_buff, d_size_bytes);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    bytes_written = cc3xx_asn1_write_int(key_buffer_end, key_buffer, 0);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    bytes_written =
        cc3xx_asn1_write_len(key_buffer_end, key_buffer, buffer_used);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    bytes_written = cc3xx_asn1_write_tag(key_buffer_end, key_buffer,
                                         CC3XX_TAG_ASN1_SEQUENCE |
                                             CC3XX_TAG_ASN1_CONSTRUCTED);
    buffer_used += bytes_written;
    if (bytes_written < 0) {
        cc_err = CC_OUT_OF_RESOURCE_ERROR;
        goto end;
    }

    /* The asn1 functions write to the end of the buffer.
     * Move the data to the beginning and erase remaining data
     * at the original location. */

    if (2 * buffer_used <= key_buffer_size) {
        CC_PalMemMove(key_buffer, key_buffer + key_buffer_size - buffer_used,
                      buffer_used);
        CC_PalMemSetZero(key_buffer + key_buffer_size - buffer_used,
                         buffer_used);
    } else if ((size_t)buffer_used < key_buffer_size) {
        CC_PalMemMove(key_buffer, key_buffer + key_buffer_size - buffer_used,
                      buffer_used);
        CC_PalMemSetZero(key_buffer + buffer_used,
                         key_buffer_size - buffer_used);
    }

    /* Reports as output the size in byte of the DER key just written */
    *key_buffer_length = buffer_used;

end:
    if (temp_buff) {
        /* zeroing temp buffers  */
        CC_PalMemSetZero(temp_buff, d_size_bytes);
        mbedtls_free(temp_buff);
    }

    return cc3xx_rsa_cc_error_to_psa_error(cc_err);
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
}

psa_status_t cc3xx_rsa_cc_error_to_psa_error(CCError_t cc_error)
{
#ifndef CC3XX_CONFIG_SUPPORT_RSA
    return PSA_ERROR_NOT_SUPPORTED;
#else
#ifndef CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION
    /* Limited coverage error code conversion routine */
    switch (cc_error) {
    case CC_SUCCESS:
        return PSA_SUCCESS;
    case CC_RSA_INVALID_SIGNATURE_BUFFER_SIZE:
        return PSA_ERROR_BUFFER_TOO_SMALL;
    case CC_RSA_ERROR_VER15_INCONSISTENT_VERIFY:
    case CC_RSA_ERROR_PSS_INCONSISTENT_VERIFY:
    case CC_RSA_INVALID_SIGNATURE_BUFFER_POINTER:
        return PSA_ERROR_INVALID_SIGNATURE;
    default:
        return PSA_ERROR_HARDWARE_FAILURE;
    }
#else /* CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION */
    psa_status_t err;
    /* Full coverage error code conversion routine */
    switch (cc_error) {
    case CC_SUCCESS:
        return PSA_SUCCESS;
        break;

    case CC_RSA_BASE_MGF_MASK_TOO_LONG:
    case CC_RSA_BASE_OAEP_DECODE_MESSAGE_TOO_LONG:
    case CC_RSA_BASE_OAEP_DECODE_PARAMETER_STRING_TOO_LONG:
    case CC_RSA_BASE_OAEP_ENCODE_MESSAGE_TOO_LONG:
    case CC_RSA_BASE_OAEP_ENCODE_PARAMETER_STRING_TOO_LONG:
    case CC_RSA_CONV_TO_CRT_INVALID_TEMP_BUFF_POINTER_ERROR:
    case CC_RSA_DATA_POINTER_INVALID_ERROR:
    case CC_RSA_DECRYPT_INVALID_OUTPUT_SIZE:
    case CC_RSA_DECRYPT_OUTPUT_SIZE_POINTER_ERROR:
    case CC_RSA_ENCODE_15_MSG_OUT_OF_RANGE:
    case CC_RSA_GET_DER_HASH_MODE_ILLEGAL:
    case CC_RSA_HASH_ILLEGAL_OPERATION_MODE_ERROR:
    case CC_RSA_ILLEGAL_PARAMS_ACCORDING_TO_PRIV_ERROR:
    case CC_RSA_INVALID_CRT_COEFFICIENT_PTR_ERROR:
    case CC_RSA_INVALID_CRT_COEFFICIENT_SIZE_ERROR:
    case CC_RSA_INVALID_CRT_COEFFICIENT_SIZE_PTR_ERROR:
    case CC_RSA_INVALID_CRT_COEFF_VAL:
    case CC_RSA_INVALID_CRT_FIRST_AND_SECOND_FACTOR_SIZE:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_EXPONENT_VAL:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_EXP_PTR_ERROR:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_EXP_SIZE_ERROR:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_EXP_SIZE_PTR_ERROR:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_POINTER_ERROR:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_SIZE:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_SIZE_ERROR:
    case CC_RSA_INVALID_CRT_FIRST_FACTOR_SIZE_POINTER_ERROR:
    case CC_RSA_INVALID_CRT_PARAMETR_SIZE_ERROR:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_EXPONENT_VAL:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_EXP_PTR_ERROR:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_EXP_SIZE_ERROR:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_EXP_SIZE_PTR_ERROR:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_POINTER_ERROR:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_SIZE:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_SIZE_ERROR:
    case CC_RSA_INVALID_CRT_SECOND_FACTOR_SIZE_POINTER_ERROR:
    case CC_RSA_INVALID_DECRYPRION_MODE_ERROR:
    case CC_RSA_INVALID_EXPONENT_POINTER_ERROR:
    case CC_RSA_INVALID_EXPONENT_SIZE:
    case CC_RSA_INVALID_EXPONENT_VAL:
    case CC_RSA_INVALID_EXP_BUFFER_SIZE_POINTER:
    case CC_RSA_INVALID_MESSAGE_BUFFER_SIZE:
    case CC_RSA_INVALID_MESSAGE_DATA_SIZE:
    case CC_RSA_INVALID_MODULUS_ERROR:
    case CC_RSA_INVALID_MODULUS_POINTER_ERROR:
    case CC_RSA_INVALID_MODULUS_SIZE:
    case CC_RSA_INVALID_MOD_BUFFER_SIZE_POINTER:
    case CC_RSA_INVALID_OUTPUT_POINTER_ERROR:
    case CC_RSA_INVALID_OUTPUT_SIZE_POINTER_ERROR:
    case CC_RSA_INVALID_PRIV_KEY_STRUCT_POINTER_ERROR:
    case CC_RSA_INVALID_PTR_ERROR:
    case CC_RSA_INVALID_PUB_KEY_STRUCT_POINTER_ERROR:
    case CC_RSA_INVALID_SIGNATURE_BUFFER_POINTER:
    case CC_RSA_INVALID_USER_CONTEXT_POINTER_ERROR:
    case CC_RSA_KEY_GEN_DATA_STRUCT_POINTER_INVALID:
    case CC_RSA_MGF_ILLEGAL_ARG_ERROR:
    case CC_RSA_MODULUS_EVEN_ERROR:
    case CC_RSA_PKCS1_VER_ARG_ERROR:
    case CC_RSA_PRIM_DATA_STRUCT_POINTER_INVALID:
    case CC_RSA_PRIV_KEY_VALIDATION_TAG_ERROR:
    case CC_RSA_PSS_ENCODING_MODULUS_HASH_SALT_LENGTHS_ERROR:
    case CC_RSA_PUB_KEY_VALIDATION_TAG_ERROR:
    case CC_RSA_USER_CONTEXT_VALIDATION_TAG_ERROR:
    case CC_RSA_WRONG_PRIVATE_KEY_TYPE:
    case CC_RSA_INVALID_MESSAGE_VAL:
    case CC_RSA_INVALID_MESSAGE_DATA_SIZE_IN_SSL_CASE:
    case CC_RSA_ERROR_IN_DECRYPTED_BLOCK_PARSING:
    case CC_RSA_OAEP_DECODE_ERROR:
        err = PSA_ERROR_INVALID_ARGUMENT;
        break;

    case CC_RSA_INVALID_SIGNATURE_BUFFER_SIZE:
    case CC_RSA_15_ERROR_IN_DECRYPTED_DATA_SIZE:
        err = PSA_ERROR_BUFFER_TOO_SMALL;
        break;

    case CC_RSA_KEY_GEN_CONDITIONAL_TEST_FAIL_ERROR:
    case CC_RSA_GENERATED_PRIV_KEY_IS_TOO_LOW:
    case CC_RSA_KEY_GENERATION_FAILURE_ERROR:
        err = PSA_ERROR_GENERIC_ERROR;
        break;

    case CC_RSA_CAN_NOT_GENERATE_RAND_IN_RANGE:
    case CC_RSA_ERROR_IN_RANDOM_OPERATION_FOR_ENCODE:
    case CC_RND_STATE_PTR_INVALID_ERROR:
    case CC_RND_GEN_VECTOR_FUNC_ERROR:
        err = PSA_ERROR_GENERIC_ERROR;
        break;

    case CC_RSA_ERROR_VER15_INCONSISTENT_VERIFY:
    case CC_RSA_ERROR_PSS_INCONSISTENT_VERIFY:
        err = PSA_ERROR_INVALID_SIGNATURE;
        break;

    /* For now, there is no better error code for malloc failure, both in CC and
     * PSA status
     */
    case CC_OUT_OF_RESOURCE_ERROR:
        err = PSA_ERROR_INSUFFICIENT_MEMORY;
        break;

    default:
        CC_PAL_LOG_ERR("Unknown CC_ERROR %d\r\n", cc_error);
        err = PSA_ERROR_GENERIC_ERROR;
        break;
    }

    CC_PAL_LOG_DEBUG("Converted CC_ERROR %d (0x%08x) to PSA_ERROR %d\r\n",
                     cc_error, cc_error, err);
    return err;
#endif /* CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION */
#endif /* CC3XX_CONFIG_SUPPORT_RSA */
}
/** @} */ // end of internal_rsa_util
