/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_asn1_util.c
 *
 * This file contains the implementation of the internal functions and
 * utilities to perform parsing of ASN-1 encoded key buffers
 *
 */

#include "cc3xx_internal_asn1_util.h"
#include "psa/crypto.h"

#define CC3XX_ASN1_CHK_ADD(g, f)                                               \
    do {                                                                       \
        if ((ret = (f)) < 0)                                                   \
            return (ret);                                                      \
        else                                                                   \
            (g) += ret;                                                        \
    } while (0)

/**
 * \ingroup internal_asn1_util
 */
int cc3xx_asn1_write_len(unsigned char **p, const unsigned char *start,
                         size_t len)
{
    if (len < 0x80) {
        if (*p - start < 1) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }

        *--(*p) = (unsigned char)len;
        return (1);
    }

    if (len <= 0xFF) {
        if (*p - start < 2) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }

        *--(*p) = (unsigned char)len;
        *--(*p) = 0x81;
        return (2);
    }

    if (len <= 0xFFFF) {
        if (*p - start < 3) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }

        *--(*p) = (len)&0xFF;
        *--(*p) = (len >> 8) & 0xFF;
        *--(*p) = 0x82;
        return (3);
    }

    if (len <= 0xFFFFFF) {
        if (*p - start < 4) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }

        *--(*p) = (len)&0xFF;
        *--(*p) = (len >> 8) & 0xFF;
        *--(*p) = (len >> 16) & 0xFF;
        *--(*p) = 0x83;
        return (4);
    }

    return PSA_ERROR_GENERIC_ERROR;
}

/**
 * \ingroup internal_asn1_util
 */
int cc3xx_asn1_write_tag(unsigned char **p, const unsigned char *start,
                         unsigned char tag)
{
    if (*p - start < 1) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    *--(*p) = tag;

    return (1);
}

/**
 * \ingroup internal_asn1_util
 */
int cc3xx_asn1_write_big_integer(unsigned char **p, const unsigned char *start,
                                 uint8_t *data, size_t data_size)
{
    int ret = PSA_ERROR_GENERIC_ERROR;
    size_t len = data_size;

    if (*p < start || (size_t)(*p - start) < len) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    (*p) -= len;
    CC_PalMemCopy(*p, data, data_size);

    /* DER format assumes 2s complement for numbers, so the leftmost bit
     * should be 0 for positive numbers and 1 for negative numbers.
     *
     * FIXME: Check if there is any chance that we will use negative numbers. If
     * so this should be done (was X->s == 1)
     */
    if (**p & 0x80) {
        if (*p - start < 1) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }

        *--(*p) = 0x00;
        len += 1;
    }

    CC3XX_ASN1_CHK_ADD(len, cc3xx_asn1_write_len(p, start, len));
    CC3XX_ASN1_CHK_ADD(len,
                       cc3xx_asn1_write_tag(p, start, CC3XX_TAG_ASN1_INTEGER));

    ret = (int)len;

    return (ret);
}

static int asn1_write_tagged_int(unsigned char **p, unsigned char *start,
                                 int val, int tag)
{
    int ret; /* This is used by the CC3XX_ASN1_CHK_ADD macro */
    size_t len = 0;

    do {
        if (*p - start < 1) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }
        len += 1;
        *--(*p) = val & 0xff;
        val >>= 8;
    } while (val > 0);

    if (**p & 0x80) {
        if (*p - start < 1) {
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }
        *--(*p) = 0x00;
        len += 1;
    }

    CC3XX_ASN1_CHK_ADD(len, cc3xx_asn1_write_len(p, start, len));
    CC3XX_ASN1_CHK_ADD(len, cc3xx_asn1_write_tag(p, start, tag));

    return (len);
}

static int asn1_get_tagged_int(unsigned char **p, const unsigned char *end,
                               int tag, int *val)
{
    int ret = PSA_ERROR_CORRUPTION_DETECTED;
    size_t len;

    if ((ret = cc3xx_asn1_get_tag(p, end, &len, tag)) != 0) {
        return (ret);
    }

    /*
     * len==0 is malformed (0 must be represented as 020100 for INTEGER,
     * or 0A0100 for ENUMERATED tags
     */
    if (len == 0) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    if ((**p & 0x80) != 0) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    /* Skip leading zeros. */
    while (len > 0 && **p == 0) {
        ++(*p);
        --len;
    }

    /* Reject integers that don't fit in an int. This code assumes that
     * the int type has no padding bit. */
    if (len > sizeof(int)) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }
    if (len == sizeof(int) && (**p & 0x80) != 0) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    *val = 0;
    while (len-- > 0) {
        *val = (*val << 8) | **p;
        (*p)++;
    }

    return (PSA_SUCCESS);
}

/** \defgroup internal_asn1_util Internal ASN-1 parsing functions
 *
 *  Internal functions used by the driver to parse objets in ASN-1 format
 *
 *  @{
 */
psa_status_t cc3xx_asn1_get_tag(unsigned char **p, const unsigned char *end,
                                size_t *len, int tag)
{
    if ((end - *p) < 1) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    if (**p != tag) {
        return (PSA_ERROR_GENERIC_ERROR);
    }

    (*p)++;

    return (cc3xx_asn1_get_len(p, end, len));
}

psa_status_t cc3xx_asn1_get_len(unsigned char **p, const unsigned char *end,
                                size_t *len)
{
    if ((end - *p) < 1) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    if ((**p & 0x80) == 0) {
        *len = *(*p)++;
    } else {
        switch (**p & 0x7F) {
        case 1:
            if ((end - *p) < 2) {
                return (PSA_ERROR_BUFFER_TOO_SMALL);
            }

            *len = (*p)[1];
            (*p) += 2;
            break;

        case 2:
            if ((end - *p) < 3) {
                return (PSA_ERROR_BUFFER_TOO_SMALL);
            }

            *len = ((size_t)(*p)[1] << 8) | (*p)[2];
            (*p) += 3;
            break;

        case 3:
            if ((end - *p) < 4) {
                return (PSA_ERROR_BUFFER_TOO_SMALL);
            }

            *len = ((size_t)(*p)[1] << 16) | ((size_t)(*p)[2] << 8) | (*p)[3];
            (*p) += 4;
            break;

        case 4:
            if ((end - *p) < 5) {
                return (PSA_ERROR_BUFFER_TOO_SMALL);
            }

            *len = ((size_t)(*p)[1] << 24) | ((size_t)(*p)[2] << 16) |
                   ((size_t)(*p)[3] << 8) | (*p)[4];
            (*p) += 5;
            break;

        default:
            return (PSA_ERROR_BUFFER_TOO_SMALL);
        }
    }

    if (*len > (size_t)(end - *p)) {
        return (PSA_ERROR_BUFFER_TOO_SMALL);
    }

    return (PSA_SUCCESS);
}

psa_status_t cc3xx_asn1_get_int(unsigned char **p, const unsigned char *end,
                                int *val)
{
    return (asn1_get_tagged_int(p, end, CC3XX_TAG_ASN1_INTEGER, val));
}

int cc3xx_asn1_write_int(unsigned char **p, unsigned char *start, int val)
{
    return (asn1_write_tagged_int(p, start, val, CC3XX_TAG_ASN1_INTEGER));
}
/** @} */ // end of internal_asn1_util
