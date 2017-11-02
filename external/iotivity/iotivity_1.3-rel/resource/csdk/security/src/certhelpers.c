/* *****************************************************************
 *
 * Copyright 2017 Microsoft. All Rights Reserved.
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

#include "iotivity_config.h"

#include "logger.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "cacommon.h"
#include "ocrandom.h"
#include "cacommonutil.h"

#include "ocpayload.h"
#include "payload_logging.h"
#include "pmutility.h"
#include "srmutility.h"

// headers required for mbed TLS
#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_csr.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/oid.h"

#ifndef NDEBUG
#include "mbedtls/debug.h"
#include "mbedtls/version.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>

#include "certhelpers.h"

#define TAG "OIC_CERTHELPERS"

/**
 * @def PERSONALIZATION_STRING
 * @brief Personalization string for the mbedtls RNG
 */
#define PERSONALIZATION_STRING "IOTIVITY_RND"

#define MAX_URI_QUERY MAX_URI_LENGTH + MAX_QUERY_LENGTH

#define MAX_STRING_LEN 254

#define MAX_ROLES_PER_CERT 10

/* OID for role certificates (1.3.6.1.4.1.44924.1.7) suitable for mbedTLS check */
static const char s_ekuRoleOid[] = MBEDTLS_OID_ISO_IDENTIFIED_ORG "\x06\x01\x04\x01\x82\xDE\x7C\x01\x07";

/**
 * Generates elliptic curve keypair.
 *
 * @param[out]  pk    mbedtls public key container
 *
 * @return  0 on success or <0 on error
 */
static int GenerateEccKeyPair(mbedtls_pk_context *pk)
{
    int ret = 0;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(pk, TAG, "Param pk is NULL", -1);

    // Initialize the DRBG context
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
                                &entropy, (const unsigned char *)PERSONALIZATION_STRING, sizeof(PERSONALIZATION_STRING));

    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "Seed initialization failed! %d", ret);
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        goto exit;
    }
    mbedtls_ctr_drbg_set_prediction_resistance(&ctr_drbg, MBEDTLS_CTR_DRBG_PR_ON);
    ret = mbedtls_pk_setup(pk, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "mbedtls_pk_setup error %d", ret);
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        goto exit;
    }
    ret = mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(*pk), mbedtls_ctr_drbg_random, &ctr_drbg);
    if (0 > ret)
    {
        OIC_LOG(ERROR, TAG, "mbedtls_ecp_gen_keypair error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        goto exit;
    }

exit:

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return 0;
}

/**
 * Generates certificate signing request for given key pair.
 *
 * @param[in]  subject        CSR Subject names should contain a comma-separated list of OID types and values:
 *                            e.g. "C=UA,O=ABC,CN=uuid:32323232-3232-3232-3232-323232323232"
 * @param[in]  key            Public/private key pair to generate for.
 * @param[in]  encoding       Encoding to use; valid values are OIC_ENCODING_PEM and OIC_ENCODING_DER
 * @param[out] csr            certificate signing request in the requested encoding
 *
 * @return  0 on success or <0 on error
 */
static int GenerateCSRForKey(const char *subject, mbedtls_pk_context* key, OicEncodingType_t encoding, OCByteString *csr)
{
    int ret = 0;
    unsigned char buf[2048];
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_x509write_csr req;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(subject, TAG, "Param subject is NULL", -1);
    VERIFY_NON_NULL_RET(csr, TAG, "Param csr is NULL", -1);

    if ((OIC_ENCODING_PEM != encoding) && (OIC_ENCODING_DER != encoding))
    {
        OIC_LOG(ERROR, TAG, "Unsupported encoding requested");
        return -1;
    }

    // Initialize all data structures up front so the error path can always free them safely, no
    // matter where the error exit point.
    mbedtls_x509write_csr_init(&req);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    csr->bytes = NULL;

    // Initialize CSR context
    // Set up MD algorithm, key and subject to CSR
    mbedtls_x509write_csr_set_md_alg(&req, MBEDTLS_MD_SHA256);
    mbedtls_x509write_csr_set_key(&req, key);
    ret = mbedtls_x509write_csr_set_subject_name(&req, subject);
    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "mbedtls_x509write_csr_set_subject_name error %d", ret);
        goto exit;
    }

    // Initialize the DRBG context
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
                                &entropy, (const unsigned char *)PERSONALIZATION_STRING, sizeof(PERSONALIZATION_STRING));
    if (0 > ret)
    {
        OIC_LOG_V(ERROR, TAG, "Seed initialization failed! %d", ret);
        goto exit;
    }
    mbedtls_ctr_drbg_set_prediction_resistance(&ctr_drbg, MBEDTLS_CTR_DRBG_PR_ON);

    // Create CSR
    if (OIC_ENCODING_PEM == encoding)
    {
        ret = mbedtls_x509write_csr_pem(&req, buf, sizeof(buf),
                                        mbedtls_ctr_drbg_random, &ctr_drbg);

        // In the PEM case, success is always denoted by a zero return. We have to compute the length.
        if (0 != ret)
        {
            OIC_LOG_V(ERROR, TAG, "mbedtls_x509write_csr_pem error %d", ret);
            goto exit;
        }
        else
        {
            csr->len = strlen((const char *)buf) + 1;
            csr->bytes = (uint8_t *)OICMalloc(csr->len);
            if (NULL == csr->bytes)
            {
                OIC_LOG(ERROR, TAG, "OICMalloc returned NULL on csr allocation");
                ret = MBEDTLS_ERR_X509_ALLOC_FAILED;
                goto exit;
            }
            memcpy(csr->bytes, buf, csr->len);
        }
    }
    else // encoding == OIC_ENCODING_DER
    {
        ret = mbedtls_x509write_csr_der(&req, buf, sizeof(buf),
                                        mbedtls_ctr_drbg_random, &ctr_drbg);

        // In the DER case, ret is the number of bytes written.
        if (0 > ret)
        {
            OIC_LOG_V(ERROR, TAG, "mbedtls_x509write_csr_der error %d", ret);
            goto exit;
        }
        else
        {
            csr->len = ret;
            csr->bytes = (uint8_t *)OICMalloc(csr->len);
            if (NULL == csr->bytes)
            {
                OIC_LOG(ERROR, TAG, "OICMalloc returned NULL on csr allocation");
                ret = MBEDTLS_ERR_X509_ALLOC_FAILED;
                goto exit;
            }
            // DER data gets written to the end of the buffer.
            memcpy(csr->bytes, buf + sizeof(buf) - csr->len, csr->len);
            ret = 0;
        }
    }



exit:

    if (0 > ret)
    {
        OICFree(csr->bytes);
        csr->bytes = NULL;
        csr->len = 0;
    }

    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_x509write_csr_free(&req);

    OICClearMemory(buf, sizeof(buf));

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return ret;
}

int OCInternalCSRRequest(const char *subject, mbedtls_pk_context *keyPair, OicEncodingType_t encoding, OCByteString *csr)
{
    return GenerateCSRForKey(subject, keyPair, encoding, csr);
}

int OCInternalGenerateKeyPair(mbedtls_pk_context *keyPair)
{
    return GenerateEccKeyPair(keyPair);
}

OCStackResult OCInternalIsValidRoleCertificate(const uint8_t *buf, size_t bufLen,
                                               uint8_t **pubKey, size_t *pubKeyLen)
{
    OCStackResult res = OC_STACK_ERROR;
    mbedtls_x509_crt parsedCert;

    OIC_LOG(DEBUG, TAG, "OCInternalIsValidRoleCertificate IN");

    mbedtls_x509_crt_init(&parsedCert);
    int mbedRet = mbedtls_x509_crt_parse(&parsedCert, buf, bufLen);
    if (0 > mbedRet)
    {
        OIC_LOG(ERROR, TAG, "Could not parse cert chain");
        goto exit;
    }

    /* We opt to require an EKU extension to be present; all-purposes certs are not allowed.
     * mbedtls_x509_crt_check_extended_key_usage will return success if the EKU extension is absent,
     * so we check this separately first.
     */
    if ((parsedCert.ext_types & MBEDTLS_X509_EXT_EXTENDED_KEY_USAGE) == 0)
    {
        OIC_LOG(ERROR, TAG, "EKU extension is absent. We require it to be present.");
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    /* The subject alternative name extension must also be present. */
    if ((parsedCert.ext_types & MBEDTLS_X509_EXT_SUBJECT_ALT_NAME) == 0)
    {
        OIC_LOG(ERROR, TAG, "Subject alternative name is absent, and is required in a role certificate.");
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    mbedRet = mbedtls_x509_crt_check_extended_key_usage(&parsedCert, s_ekuRoleOid, MBEDTLS_OID_SIZE(s_ekuRoleOid));
    if (0 > mbedRet)
    {
        OIC_LOG_V(ERROR, TAG, "Role EKU is absent: %d", mbedRet);
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    bool valid = false;
    /* Check for at least one subjAltName with a role in it. */
    for (const mbedtls_x509_general_names *nameCur = &parsedCert.subject_alt_names;
            NULL != nameCur;
            nameCur = nameCur->next)
    {
        if (MBEDTLS_X509_GENERALNAME_DIRECTORYNAME == nameCur->general_name.name_type)
        {
            /* Name must contain a CN component. OU is optional. Anything else is ignored
             * but not grounds for rejection.
             */
            for (const mbedtls_x509_name *dirName = nameCur->general_name.directory_name;
                    NULL != dirName;
                    dirName = dirName->next)
            {
                if ((MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN) == dirName->oid.len) &&
                    (0 == memcmp(MBEDTLS_OID_AT_CN, dirName->oid.p, MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN))))
                {
                    if (dirName->val.len >= ROLEID_LENGTH)
                    {
                        OIC_LOG_V(ERROR, TAG, "Certificate has role id that is too long: %" PRIuPTR, dirName->val.len);
                        res = OC_STACK_INVALID_PARAM;
                        goto exit;
                    }

                    valid = true;
                }
                else if ((MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_ORG_UNIT) == dirName->oid.len) &&
                            (0 == memcmp(MBEDTLS_OID_AT_ORG_UNIT, dirName->oid.p, MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_ORG_UNIT))))
                {
                    if (dirName->val.len >= ROLEAUTHORITY_LENGTH)
                    {
                        OIC_LOG_V(ERROR, TAG, "Certificate has role authority that is too long: %" PRIuPTR, dirName->val.len);
                        res = OC_STACK_INVALID_PARAM;
                        goto exit;
                    }
                    /* Presence of OU does not affect validity, so don't set valid flag. */
                }
                else
                {
                    OIC_LOG(WARNING, TAG, "Directory name has attribute that isn't CN or OU; ignoring");
                }
            }
        }

        if (valid)
        {
            break;
        }
    }

    if (!valid)
    {
        OIC_LOG(ERROR, TAG, "Could not find valid role encoded in a subject alternative name");
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    /* If the public key is requested, extract it. */
    if (NULL != pubKey)
    {
        unsigned char tmp[1024] = { 0 };
        mbedRet = mbedtls_pk_write_pubkey_der(&parsedCert.pk, tmp, sizeof(tmp));
        if (0 > mbedRet)
        {
            OIC_LOG_V(ERROR, TAG, "Could not write public key as DER: %d", mbedRet);
            res = OC_STACK_ERROR;
            goto exit;
        }

        /* mbedRet is the amount of data written, and it's written at the END of the buffer. */
        *pubKey = (uint8_t *)OICCalloc(1, mbedRet);
        if (NULL == *pubKey)
        {
            OIC_LOG(ERROR, TAG, "No memory allocating pubKey");
            res = OC_STACK_NO_MEMORY;
            goto exit;
        }
        memcpy(*pubKey, tmp + sizeof(tmp) - mbedRet, mbedRet);
        *pubKeyLen = mbedRet;
    }

    res = OC_STACK_OK;

exit:

    mbedtls_x509_crt_free(&parsedCert);

    OIC_LOG_V(DEBUG, TAG, "OCInternalIsValidRoleCertificate OUT; returning %d", res);
    return res;
}

OCStackResult OCInternalIsValidCertChain(const uint8_t *buf, size_t bufLen)
{
    OCStackResult res = OC_STACK_ERROR;
    int mbedRet;
    mbedtls_x509_crt parsedCert;

    OIC_LOG(DEBUG, TAG, "OCInternalIsValidCertChain IN");

    mbedtls_x509_crt_init(&parsedCert);
    mbedRet = mbedtls_x509_crt_parse(&parsedCert, buf, bufLen);
    if (0 > mbedRet)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to parse certificate chain: %d", mbedRet);
        res = OC_STACK_INVALID_PARAM;
    }
    else
    {
        res = OC_STACK_OK;
        if (0 < mbedRet)
        {
            OIC_LOG_V(WARNING, TAG, "Partly successful: mbedTLS returned %d", mbedRet);
        }
    }

    mbedtls_x509_crt_free(&parsedCert);

    OIC_LOG_V(DEBUG, TAG, "OCInternalIsValidCertChain OUT; returning %d", res);

    return res;
}

static const mbedtls_x509_crt_profile s_certProfile = {
    MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA256),            /* MD algorithms */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_PK_ECKEY) |            /* Allowed key types */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_PK_ECDSA),
    MBEDTLS_X509_ID_FLAG(MBEDTLS_ECP_DP_SECP256R1),     /* EC curves */
    0                                                   /* RSA minimum key length - not used because we only use EC key pairs */
};

OCStackResult OCInternalVerifyRoleCertificate(const OicSecKey_t *certificateChain, const uint8_t *trustedCaCerts,
                                              size_t trustedCaCertsLength, OicSecRole_t **roles,
                                              size_t *rolesLength, struct tm *notValidAfter)
{
    bool freeData = false;
    uint8_t *data = certificateChain->data;
    size_t dataLength = certificateChain->len;

    VERIFY_NOT_NULL_RETURN(TAG, certificateChain, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, trustedCaCerts, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, roles, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, rolesLength, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, notValidAfter, ERROR, OC_STACK_INVALID_PARAM);

    OCStackResult res = OC_STACK_ERROR;
    int mbedRet;
    uint32_t flags = 0;
    mbedtls_x509_crt certChain, trustedCas;

    OicSecRole_t rolesTmp[MAX_ROLES_PER_CERT];
    memset(rolesTmp, 0, sizeof(rolesTmp));
    size_t rolesTmpCount = 0;

    OIC_LOG(DEBUG, TAG, "OCInternalVerifyRoleCertificate IN");

    if ((dataLength > 0) && (data[dataLength - 1] != 0))
    {
        /* mbedtls_x509_crt_parse requires null terminator */
        data = OICMalloc(dataLength + 1);

        if (data == NULL)
        {
            OIC_LOG_V(ERROR, TAG, "%s: OICMalloc failed", __func__);
            res = OC_STACK_NO_MEMORY;
            goto exit;
        }

        OIC_LOG(DEBUG, TAG, "Adding null terminator");
        memcpy(data, certificateChain->data, dataLength);
        data[dataLength] = 0;
        dataLength++;
        freeData = true;
    }

    mbedtls_x509_crt_init(&certChain);
    mbedtls_x509_crt_init(&trustedCas);

    res = OCInternalIsValidRoleCertificate(data, dataLength, NULL, NULL);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Certificate is not valid as a role certificate: %d", res);
        goto exit;
    }

    mbedRet = mbedtls_x509_crt_parse(&certChain, data, dataLength);
    if (0 > mbedRet)
    {
        OIC_LOG_V(ERROR, TAG, "Could not parse certificates: %d", mbedRet);
        res = OC_STACK_ERROR;
        goto exit;
    }

    mbedRet = mbedtls_x509_crt_parse(&trustedCas, trustedCaCerts, trustedCaCertsLength);
    if (0 > mbedRet)
    {
        OIC_LOG_V(ERROR, TAG, "Could not parse trusted CA certs: %d", mbedRet);
        res = OC_STACK_ERROR;
        goto exit;
    }

    mbedRet = mbedtls_x509_crt_verify_with_profile(
        &certChain,
        &trustedCas,
        NULL,
        &s_certProfile,
        NULL,
        &flags,
        NULL,
        NULL);
    if (0 > mbedRet)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to verify certificate: ret = %d, flags = %u", mbedRet, flags);
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    /* Certificate is verified. If requested, extract the list of roles and return.
     * The first certificate in the certChain list is the leaf, since we parsed it first.
     */
    if (NULL != roles)
    {
        for (const mbedtls_x509_general_names *nameCur = &certChain.subject_alt_names;
             (NULL != nameCur) && (rolesTmpCount < MAX_ROLES_PER_CERT);
             nameCur = nameCur->next)
        {
            if (MBEDTLS_X509_GENERALNAME_DIRECTORYNAME == nameCur->general_name.name_type)
            {
                bool advanceCount = false;
                bool addAuthority = true;
                for (const mbedtls_x509_name *dirName = nameCur->general_name.directory_name;
                     NULL != dirName;
                     dirName = dirName->next)
                {
                    if ((MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN) == dirName->oid.len) &&
                        (0 == memcmp(MBEDTLS_OID_AT_CN, dirName->oid.p, MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN))) &&
                        (rolesTmp[rolesTmpCount].id[0] == '\0'))
                    {
                        /* When checking validity above, we made sure the role ID and authority were not too
                         * long to fit in an OicSecRole_t. Here we only assert, but don't check again in release code.
                         * id was also initialized as all zeroes, so the string will automatically be null-terminated.
                         */
                        assert(dirName->val.len < ROLEID_LENGTH);
                        memcpy(rolesTmp[rolesTmpCount].id, dirName->val.p, dirName->val.len);
                        advanceCount = true;
                    }
                    else if ((MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_ORG_UNIT) == dirName->oid.len) &&
                             (0 == memcmp(MBEDTLS_OID_AT_ORG_UNIT, dirName->oid.p, MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_ORG_UNIT)))&&
                             (rolesTmp[rolesTmpCount].authority[0] == '\0'))
                    {
                        assert(dirName->val.len < ROLEID_LENGTH);
                        memcpy(rolesTmp[rolesTmpCount].authority, dirName->val.p, dirName->val.len);
                        addAuthority = false;
                    }
                }

                if (advanceCount)
                {
                    /* If the authority was absent in the subject alternative name we know that the certificate
                     * issuer defined the role. We add the issuer as the authority here so that access checks
                     * function properly when invoked.
                     */
                    if (addAuthority)
                    {
                        for (const mbedtls_x509_name *issuerName = &certChain.issuer;
                             NULL != issuerName;
                             issuerName = issuerName->next)
                        {
                            if ((MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN) == issuerName->oid.len) &&
                                (0 == memcmp(MBEDTLS_OID_AT_CN, issuerName->oid.p, MBEDTLS_OID_SIZE(MBEDTLS_OID_AT_CN))))
                            {
                                assert(issuerName->val.len < ROLEID_LENGTH);
                                memcpy(rolesTmp[rolesTmpCount].authority, issuerName->val.p, issuerName->val.len);
                                OIC_LOG_V(DEBUG, TAG, "Adding authority %s to role", rolesTmp[rolesTmpCount].authority);
                            }
                        }
                    }

                    rolesTmpCount++;
                }
            }
        }

        if (MAX_ROLES_PER_CERT <= rolesTmpCount)
        {
            OIC_LOG(ERROR, TAG, "More roles than supported in a single certificate");
            res = OC_STACK_ERROR;
            goto exit;
        }
        else if (0 == rolesTmpCount)
        {
            OIC_LOG(ERROR, TAG, "No roles in the certificate");
            res = OC_STACK_ERROR;
            goto exit;
        }

        *roles = (OicSecRole_t *)OICCalloc(1, sizeof(OicSecRole_t) * rolesTmpCount);
        if (NULL == *roles)
        {
            OIC_LOG(ERROR, TAG, "No memory allocating roles array");
            res = OC_STACK_NO_MEMORY;
            goto exit;
        }

        memcpy(*roles, rolesTmp, sizeof(rolesTmp[0]) * rolesTmpCount);
        *rolesLength = rolesTmpCount;
    }

    memset(notValidAfter, 0, sizeof(*notValidAfter));

    notValidAfter->tm_year = certChain.valid_to.year - 1900;
    notValidAfter->tm_mon = certChain.valid_to.mon - 1;
    notValidAfter->tm_mday = certChain.valid_to.day;
    notValidAfter->tm_hour = certChain.valid_to.hour;
    notValidAfter->tm_min = certChain.valid_to.min;
    notValidAfter->tm_sec = certChain.valid_to.sec;

    res = OC_STACK_OK;

exit:

    mbedtls_x509_crt_free(&trustedCas);
    mbedtls_x509_crt_free(&certChain);

    if (freeData) {
        OICFree(data);
    }

    OIC_LOG_V(DEBUG, TAG, "OCInternalVerifyRoleCertificate out: %d", res);

    return res;
}