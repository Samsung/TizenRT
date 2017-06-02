/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include "pkix_interface.h"
#include "credresource.h"
#include "crlresource.h"
#include "srmresourcestrings.h"
#include "casecurityinterface.h"
#include "logger.h"
#ifdef __TIZENRT__
#include "oic_malloc.h"
#endif

#define TAG "OIC_SRM_PKIX_INTERFACE"

static HWPkixContext_t gHwPkixCtx = {
    .getHwKeyContext = NULL,
    .freeHwKeyContext = NULL,
    .getOwnCertCb = NULL,
    .setupPkContextCb = NULL,
    .hwKeyCtx = NULL
};

/*
 * returns the number of removed zeros
 */
static size_t remove_useless_leading_zeros(uint8_t *asn_integer_buf, uint32_t size)
{
    /*
    */
    uint8_t buf[32 + 4];
    uint8_t tag = asn_integer_buf[0];
    uint8_t len = asn_integer_buf[1];
    uint8_t *p = asn_integer_buf + 2;
    uint32_t idx = 0;

    if (size < 2)
    {
        return 0;
    }

    if (tag != MBEDTLS_ASN1_INTEGER)
    {
        return 0;
    }

    if ((uint32_t)len + 2 != size || sizeof(buf) < size)
    {
        return 0;
    }

    for (idx = 0; idx < (uint32_t)len - 1; idx++)
    {
        if (p[idx] != 0)
        {
            break;
        }
        if (p[idx] == 0 && ((p[idx + 1] & 0x80) != 0)) // 00000000 1XXXXXXXX (bit)
        {
            break;
        }
    }

    if (idx == 0)
    {
        return 0;
    }

    len -= idx;
    asn_integer_buf[1] = len;

    memcpy(buf, p + idx, len);
    memcpy(p, buf, len);

    return idx;
}

void CheckInvalidDERSignature(uint8_t *crtBuf, size_t *crtBufLen)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == crtBuf)
    {
        OIC_LOG(ERROR, TAG, "Param buf is NULL");
        return;
    }
    if (NULL == crtBufLen)
    {
        OIC_LOG(ERROR, TAG, "Param bufLen is NULL");
        return;
    }

    mbedtls_x509_crt deviceCert;
    int ret = 0;

    // check only first(i.e., device) certificate
    if (crtBuf[0] == 0x30 && crtBuf[1] == 0x82)
    {
        uint8_t *sign_ptr = NULL;
        /**
        * structure of r_buf & s_buf
        * +---------------+-------------+----------------------------+
        * | tag (INTEGER) | length (1B) | value (r or s in integer)  |
        * +---------------+-------------+----------------------------+
        */
        uint8_t r_buf[32 + 4]; // for ECC 256 sign
        uint8_t s_buf[32 + 4];
        uint32_t r_len = 0;
        uint32_t s_len = 0;
        size_t sign_len = 0;
        uint32_t removed = 0;
        uint32_t removed_total = 0;
        size_t org_len = 0;

        mbedtls_x509_crt_init(&deviceCert);

        unsigned char * tmp = (unsigned char *)crtBuf + 1;
        if ( 0 != mbedtls_asn1_get_len(&tmp, crtBuf + *crtBufLen, &org_len))
        {
            OIC_LOG(ERROR, TAG, "Invalid parsed length");
            goto exit;
        }

        if (org_len < *crtBufLen)
        {
            ret = mbedtls_x509_crt_parse_der(&deviceCert, crtBuf, org_len + 4);
            if (0 != ret)
            {
                OIC_LOG_V(ERROR, TAG, "mbedtls_x509_crt_parse_der returned -0x%04x", -(ret));
                goto exit;
            }

            if (NULL == deviceCert.sig.p)
            {
                OIC_LOG(ERROR, TAG, "Cert's signature is NULL");
                goto exit;
            }
            sign_ptr = (uint8_t*)deviceCert.sig.p;
            ret = mbedtls_asn1_get_tag(&sign_ptr, deviceCert.sig.p + deviceCert.sig.len, &sign_len,
                MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE);
            if (0 != ret)
            {
                OIC_LOG_V(ERROR, TAG, "mbedtls_asn1_get_tag error : %d", ret);
                goto exit;
            }
            if (sign_ptr)
            {
                r_len = sign_ptr[1] + 2; // including header itself
            }
            if (r_len > deviceCert.sig.len)
            {
                OIC_LOG_V(ERROR, TAG, "signature length check error #1 : %d", ret);
                goto exit;
            }
            OIC_LOG_V(DEBUG, TAG, "r_len = %d", r_len);
            memcpy(r_buf, sign_ptr, r_len);
            removed = remove_useless_leading_zeros(r_buf, r_len);

            /* Do not change order */
            sign_ptr += r_len;
            r_len -= removed;
            removed_total += removed;
            if (sign_ptr)
            {
                s_len = sign_ptr[1] + 2; // including header itself
            }
            if (s_len + r_len > deviceCert.sig.len)
            {
                OIC_LOG_V(ERROR, TAG, "signature length check error #2 : %d", ret);
                goto exit;
            }
            OIC_LOG_V(DEBUG, TAG, "s_len = %d", s_len);
            memcpy(s_buf, sign_ptr, s_len);
            removed = remove_useless_leading_zeros(s_buf, s_len);
            s_len -= removed;
            removed_total += removed;

            if (removed_total > 0)
            {
                // if length of signature is incorrect.
                OIC_LOG_V(INFO, TAG, "Cert Length (Before) : %lu", *crtBufLen);
                OIC_LOG(INFO, TAG, "Invalid length of signature is dectected.");
                OIC_LOG(INFO, TAG, "Update signature...");

                uint16_t crt_len = 0;
                /* change bitstring length */
                {
                    uint8_t *sign_len_ptr = deviceCert.sig.p + 1;
                    *sign_len_ptr -= removed_total;
                }
                /* change signature length */
                {
                    uint8_t *sign_len_ptr = deviceCert.sig.p - 1;
                    if (*sign_len_ptr == 0)
                    {
                        sign_len_ptr--;
                    }
                    *sign_len_ptr -= removed_total;
                }
                /* change signature */
                {
                    memcpy(deviceCert.sig.p + 2, r_buf, r_len);
                    memcpy(deviceCert.sig.p + 2 + r_len, s_buf, s_len);
                }
                /* change certificate length */
                {
                    uint8_t *crt_len_ptr = deviceCert.raw.p + 2;
                    crt_len = (crt_len_ptr[0] << 8) & 0x0000FF00;
                    crt_len |= (crt_len_ptr[1] & 0x000000FF);
                    crt_len -= removed_total;
                    crt_len_ptr[0] = (uint8_t)(crt_len >> 8);
                    crt_len_ptr[1] = (uint8_t)(crt_len);
                }
                crt_len += 4; // include header and length field
                /* change raw data and crt parse cert again */
                {
                    mbedtls_x509_crt crt_cpy;
                    mbedtls_x509_crt_init( &crt_cpy );
                    if ((ret = mbedtls_x509_crt_parse_der(&crt_cpy, deviceCert.raw.p, (size_t)crt_len)) != 0)
                    {
                        OIC_LOG_V(ERROR, TAG, "mbedtls_x509_crt_parse error : %d", ret);
                        mbedtls_x509_crt_free(&crt_cpy);
                        goto exit;
                    }

                    org_len += 4; // include header and length field
                    size_t remained_len = (*crtBufLen - org_len);
                    memcpy(crtBuf, deviceCert.raw.p, crt_len);
                    memcpy(crtBuf + crt_len, crtBuf + org_len, remained_len);
                    *crtBufLen = (size_t)crt_len + remained_len;

                    mbedtls_x509_crt_free(&crt_cpy);
                    OIC_LOG_V(INFO, TAG, "Dev cert : %lu -> %lu", org_len, crt_len);
                    OIC_LOG_V(INFO, TAG, "Remained chain : %lu", remained_len);
                    OIC_LOG_V(INFO, TAG, "Cert Length (After) : %lu", *crtBufLen);
                }
            }
            else
            {
                // if length of signature is correct.
                OIC_LOG(INFO, TAG, "Don't need to update signature...");
            }

        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "PEM or invalid cert format detected. finish to check");
    }

exit:
    mbedtls_x509_crt_free(&deviceCert);
    OIC_LOG_V(DEBUG, TAG, "Cert chain length = %d", *crtBufLen);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

/**
 * Internal API to load the key/cert from HW PKI
 *
 * @return true on HW PKI is available
 */
static bool GetPkixInfoFromHw(PkiInfo_t * inf)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (!gHwPkixCtx.hwKeyCtx && gHwPkixCtx.getHwKeyContext)
    {
        gHwPkixCtx.hwKeyCtx = gHwPkixCtx.getHwKeyContext(
                                                        HWKEY_SVC_IOTIVITY,
                                                        HWKEY_USAGE_PRIMARY, NULL);
        if (!gHwPkixCtx.hwKeyCtx)
        {
            OIC_LOG(WARNING, TAG, "gHwPkixCtx.getHwKeyContext return null");
        }
    }

    if (gHwPkixCtx.hwKeyCtx && gHwPkixCtx.getOwnCertCb)
    {
        int ret = gHwPkixCtx.getOwnCertCb(gHwPkixCtx.hwKeyCtx, &inf->crt.data, &inf->crt.len);
        if (0 != ret)
        {
            OIC_LOG_V(ERROR, TAG, "gHwPkixCtx.getOwnCertCb error : %d", ret);
            OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
            return false;
        }

        // check and fix invalid cert signature
        CheckInvalidDERSignature(inf->crt.data, &inf->crt.len);

        OIC_LOG_V(DEBUG, TAG, "Cert chain length = %d", inf->crt.len);
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return true;
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return false;
}

void GetPkixInfo(PkiInfo_t * inf)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == inf)
    {
        OIC_LOG(ERROR, TAG, "NULL passed");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }

    if (GetPkixInfoFromHw(inf))
    {
        OIC_LOG(INFO, TAG, "H/W based PKI will be used.");
    }
    else
    {
        GetDerOwnCert(&inf->crt, PRIMARY_CERT);
        GetDerKey(&inf->key, PRIMARY_CERT);
    }
    GetDerCaCert(&inf->ca, TRUST_CA);
    GetDerCrl(&inf->crl);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

void GetManufacturerPkixInfo(PkiInfo_t * inf)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == inf)
    {
        OIC_LOG(ERROR, TAG, "NULL passed");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }

    if (GetPkixInfoFromHw(inf))
    {
        OIC_LOG(INFO, TAG, "H/W based PKI will be used.");
    }
    else
    {
        GetDerOwnCert(&inf->crt, MF_PRIMARY_CERT);
        GetDerKey(&inf->key, MF_PRIMARY_CERT);
    }
    GetDerCaCert(&inf->ca, MF_TRUST_CA);
    // CRL not provided
#ifdef __TIZENRT__
    OICFree(inf->crl.data);
#endif
    inf->crl.data = NULL;
    inf->crl.len = 0;
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

void InitCipherSuiteList(bool * list)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == list)
    {
        OIC_LOG(ERROR, TAG, "NULL passed");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    InitCipherSuiteListInternal(list, TRUST_CA);
    if (gHwPkixCtx.getOwnCertCb)
    {
        list[1] = true;
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

void InitManufacturerCipherSuiteList(bool * list)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == list)
    {
        OIC_LOG(ERROR, TAG, "NULL passed");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    InitCipherSuiteListInternal(list, MF_TRUST_CA);
    if (gHwPkixCtx.getOwnCertCb)
    {
        list[1] = true;
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

int SetupHwPkContext(mbedtls_pk_context* pkCtx)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == pkCtx)
    {
        OIC_LOG(ERROR, TAG, "NULL passed");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }

    if (gHwPkixCtx.setupPkContextCb)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return gHwPkixCtx.setupPkContextCb(pkCtx, gHwPkixCtx.hwKeyCtx);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "gHwPkixCallbacks.setupPkContextCb is NULL");
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return -1;
}

int SetHwPkixCallbacks(GetHwKeyContext getHwKeyContext,
                        FreeHwKeyContext freeHwKeyContext,
                        GetOwnCertFromHwCallback getOwnCertCb,
                        SetupPkContextFromHwCallback setupPkContextCb)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);

    if (NULL == getHwKeyContext || NULL == freeHwKeyContext
        || NULL == getOwnCertCb || NULL == setupPkContextCb)
    {
        OIC_LOG(ERROR, TAG, "NULL Passed");
        OIC_LOG(ERROR, TAG, "Callback function parameters can not be null");
        return -1;
    }

    gHwPkixCtx.getHwKeyContext = getHwKeyContext;
    gHwPkixCtx.freeHwKeyContext = freeHwKeyContext;
    gHwPkixCtx.getOwnCertCb = getOwnCertCb;
    gHwPkixCtx.setupPkContextCb = setupPkContextCb;

    if (gHwPkixCtx.hwKeyCtx)
    {
        gHwPkixCtx.freeHwKeyContext(gHwPkixCtx.hwKeyCtx);
    }
    gHwPkixCtx.hwKeyCtx = NULL;

    // setup pkcontext handler
    CAregisterSetupPkContextHandler(SetupHwPkContext);

    OIC_LOG_V(INFO, TAG, "Out %s", __func__);
    return 0;
}

