/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_asn1_util.h
 *
 * This file contains the declaration of the internal functions and
 * utilities to perform parsing of ASN-1 encoded key buffers
 *
 */

#ifndef CC3XX_INTERNAL_ASN1_UTIL_H
#define CC3XX_INTERNAL_ASN1_UTIL_H

#include "psa/crypto.h"

#include "cc_common.h"
#include "cc_ecc_internal.h"
#include "cc_ecpki_error.h"
#include "cc_pal_abort.h"
#include "cc_pal_mem.h"
#include "cc_pal_types.h"
#include "mbedtls_cc_ec_mont_edw_error.h"

#include "cc_ecpki_build.h"
#include "cc_ecpki_domain.h"
#include "cc_ecpki_ecdsa.h"
#include "cc_ecpki_kg.h"
#include "cc_ecpki_local.h"
#include "pka_ec_wrst.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name DER constants
 * These constants comply with the DER encoded ASN.1 type tags.
 * DER encoding uses hexadecimal representation.
 * An example DER sequence is:\n
 * - 0x02 -- tag indicating INTEGER
 * - 0x01 -- length in octets
 * - 0x05 -- value
 * Such sequences are typically read into \c ::mbedtls_x509_buf.
 */
#define CC3XX_TAG_ASN1_INTEGER 0x02
#define CC3XX_TAG_ASN1_SEQUENCE 0x10
#define CC3XX_TAG_ASN1_CONSTRUCTED 0x20

/**
 * \brief           Write a length field in ASN.1 format.
 *
 * \note            This function works backwards in data buffer.
 *
 * \param p         The reference to the current position pointer.
 * \param start     The start of the buffer, for bounds-checking.
 * \param len       The length value to write.
 *
 * \return          The number of bytes written to \p p on success.
 * \return          PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 * \return          PSA_ERROR_GENERIC_ERROR if any other error occured.
 */
int cc3xx_asn1_write_len(unsigned char **p, const unsigned char *start, size_t len);

/**
 * \brief           Write an ASN.1 tag in ASN.1 format.
 *
 * \note            This function works backwards in data buffer.
 *
 * \param p         The reference to the current position pointer.
 * \param start     The start of the buffer, for bounds-checking.
 * \param tag       The tag to write.
 *
 * \return          The number of bytes written to \p p on success.
 * \return          PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 */
int cc3xx_asn1_write_tag(unsigned char **p, const unsigned char *start,
                         unsigned char tag);

/**
 * \brief           Write an int tag and value in ASN.1 format represented as a uint8_t array.
 *
 * \note            This function works backwards in data buffer.
 *
 * \param p         The reference to the current position pointer.
 * \param start     The start of the buffer, for bounds-checking.
 * \param data      Buffer containing the integer to be written.
 * \param data_size Size in bytes of the integer.
 *
 * \return          The number of bytes written to \p p on success.
 * \return          PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 * \return          PSA_ERROR_GENERIC_ERROR if any other error occured.
 */
int cc3xx_asn1_write_big_integer(unsigned char **p, const unsigned char *start,
                                 uint8_t *data, size_t data_size);

/**
 * \brief           Write an int tag and value in ASN.1 format.
 *
 * \note            This function works backwards in data buffer.
 *
 * \param p         The reference to the current position pointer.
 * \param start     The start of the buffer, for bounds-checking.
 * \param val       The integer value to write.
 *                  It must be non-negative.
 *
 * \return          The number of bytes written to \p p on success.
 * \return          PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 * \return          PSA_ERROR_GENERIC_ERROR if any other error occured.
 */
int cc3xx_asn1_write_int(unsigned char **p, unsigned char *start, int val);

/**
 * \brief       Get the tag and length of the element.
 *              Check for the requested tag.
 *              Updates the pointer to immediately behind the tag and length.
 *
 * \param p     On entry, \c *p points to the start of the ASN.1 element.
 *              On successful completion, \c *p points to the first byte
 *              after the length, i.e. the first byte of the content.
 *              On error, the value of \c *p is undefined.
 * \param end   End of data.
 * \param len   On successful completion, \c *len contains the length
 *              read from the ASN.1 input.
 * \param tag   The expected tag.
 *
 * \return      PSA_SUCCESS if successful.
 * \return      PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 * \return      PSA_ERROR_GENERIC_ERROR if any other error occured.
 */
psa_status_t cc3xx_asn1_get_tag(unsigned char **p, const unsigned char *end, size_t *len,
                       int tag);

/**
 * \brief       Get the length of an ASN.1 element.
 *              Updates the pointer to immediately behind the length.
 *
 * \param p     On entry, \c *p points to the first byte of the length,
 *              i.e. immediately after the tag.
 *              On successful completion, \c *p points to the first byte
 *              after the length, i.e. the first byte of the content.
 *              On error, the value of \c *p is undefined.
 * \param end   End of data.
 * \param len   On successful completion, \c *len contains the length
 *              read from the ASN.1 input.
 *
 * \return      PSA_SUCCESS if successful.
 * \return      PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 * \return      PSA_ERROR_GENERIC_ERROR if any other error occured.
 */
psa_status_t cc3xx_asn1_get_len(unsigned char **p, const unsigned char *end,
                       size_t *len);

/**
 * \brief       Retrieve an integer ASN.1 tag and its value.
 *              Updates the pointer to immediately behind the full tag.
 *
 * \param p     On entry, \c *p points to the start of the ASN.1 element.
 *              On successful completion, \c *p points to the first byte
 *              beyond the ASN.1 element.
 *              On error, the value of \c *p is undefined.
 * \param end   End of data.
 * \param val   On success, the parsed value.
 *
 * \return      PSA_SUCCESS if successful.
 * \return      PSA_ERROR_BUFFER_TOO_SMALL if the buffer is too small.
 * \return      PSA_ERROR_GENERIC_ERROR if any other error occured.
 */
psa_status_t cc3xx_asn1_get_int(unsigned char **p, const unsigned char *end, int *val);

#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_ASN1_UTIL_H */

