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
#include "utils.h"

#include "logger.h"
#include <stddef.h>
#include <string.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "cacommonutil.h"

#include "ocpayload.h"
#include "payload_logging.h"
#include "doxmresource.h"
#include "pmutility.h"
#include "secureresourceprovider.h"

// headers required for mbed TLS
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pkcs12.h"
#include "mbedtls/ssl_internal.h"
#include "mbedtls/x509_csr.h"

#ifndef NDEBUG
#include "mbedtls/debug.h"
#include "mbedtls/version.h"
#endif

#include <unistd.h>
#include <fcntl.h>

#define TAG "OIC_CLOUD_CSR"

//TODO: is it required in CSR response?
static OCByteString g_privateKey = {0, 0};

#define MAX_URI_QUERY MAX_URI_LENGTH + MAX_QUERY_LENGTH

#define MAX_STRING_LEN 254

/**
 * @def SEED
 * @brief Seed for initialization RNG
 */

#define SEED "IOTIVITY_RND"

typedef struct
{
    char *subject;
    char *prKey;
    size_t prKeyLen;
    char *pubKey;
    size_t pubKeyLen;
} SignCred_t;

/**
 * Make CSR subject like:
 * Subject: C=KR, O=Samsung, OU=OCF Device, CN=uuid:1234567890
 * @param[in] subj              the subject
 */
static void CSRMakeSubject(char *subject, const char *countryCode, const char *organisation,
                    const char *organizationalUnitName, const char *deviceId)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    if (!deviceId)
    {
        OIC_LOG_V(ERROR, TAG, "%s: The device id is NULL",__func__);
        return;
    }
    snprintf(subject, MAX_STRING_LEN, "C=%s, O=%s, OU=%s, CN=uuid:%s", countryCode, organisation, organizationalUnitName, deviceId);

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);
}
/**
 * Generates elliptic keypair.
 *
 * @param[out]  pk    mbedtls public key container
 *
 * @return  0 on success or -1 on error
 */
static int ecdsaGenKeypair(mbedtls_pk_context * pk)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    VERIFY_NON_NULL_RET(pk, TAG, "Param pk is NULL", -1);

    // Entropy seeding
#ifdef __unix__
    unsigned char seed[sizeof(SEED)] = {0};
    int urandomFd = -2;
    urandomFd = open("/dev/urandom", O_RDONLY);
    if(urandomFd == -1)
    {
        OIC_LOG(ERROR, TAG, "Fails open /dev/urandom!");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    if(0 > read(urandomFd, seed, sizeof(seed)))
    {
        OIC_LOG(ERROR, TAG, "Fails read from /dev/urandom!");
        close(urandomFd);
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    close(urandomFd);

#else
    unsigned char * seed = (unsigned char*) SEED;
#endif
    // Initialize and seed DRBG context
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    if (0 != mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
                                   &entropy, seed, sizeof(SEED)))
    {
        OIC_LOG(ERROR, TAG, "Seed initialization failed!");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    mbedtls_ctr_drbg_set_prediction_resistance(&ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF);
    // Initialize l context
    mbedtls_pk_init(pk);
    if (0 > mbedtls_pk_setup(pk, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY)))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_pk_info_from_type error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    if (0 > mbedtls_ecp_group_load(&mbedtls_pk_ec(*pk)->grp, MBEDTLS_ECP_DP_SECP256R1))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_ecp_group_load error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    if (0 > mbedtls_ecp_gen_keypair(&mbedtls_pk_ec(*pk)->grp,
                                    &mbedtls_pk_ec(*pk)->d,
                                    &mbedtls_pk_ec(*pk)->Q,
                                    mbedtls_ctr_drbg_random, &ctr_drbg))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_ecp_gen_keypair error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return 0;
}



/**
 * Generates keypair and certificate signing request.
 *
 * @param[in]  subjectName    CSR Subject names should contain
 * a comma-separated list of OID types and values:
 * e.g. "C=UA,O=ABC,CN=uuid:32323232-3232-3232-3232-323232323232"
 * @param[out] prKey          private key in DER
 * @param[out] prKeyLen       private key buffer length
 * @param[out] pubKey         public key in DER
 * @param[out] pubKeyLen      public key buffer length
 * @param[out] CSR            certificate signing request in DER
 * @param[out] CSRLen         certificate signing request buffer length
 *
 * @return  0 on success or -1 on error
 */
static int GenerateCSR(char *subject, OCByteString *csr)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(subject, TAG, "Param subject is NULL", -1);
    VERIFY_NON_NULL_RET(csr, TAG, "Param csr is NULL", -1);

    int ret = 0;
    int bufsize = 1024;
    unsigned char * buf = NULL;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_pk_context * key = NULL;
    mbedtls_x509write_csr req;

    // Initialize keypair context
    key = (mbedtls_pk_context *)OICMalloc(sizeof(mbedtls_pk_context));
    if (NULL == key)
    {
        OIC_LOG_V(ERROR, TAG, "OICMalloc returned NULL on key allocation");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    // Generate keypair
    if (0 > ecdsaGenKeypair(key))
    {
        OIC_LOG(ERROR, TAG, "ecdsaGenKeypair error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }

    // Initialize CSR context
    mbedtls_x509write_csr_init(&req);
    // Set up MD algorithm, key and subject to CSR
    mbedtls_x509write_csr_set_md_alg(&req, MBEDTLS_MD_SHA256);
    mbedtls_x509write_csr_set_key(&req, key);
    if (0 > mbedtls_x509write_csr_set_subject_name(&req, subject))
    {
        OIC_LOG(ERROR, TAG, "mbedtls_x509write_csr_set_subject_name error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }

    // Entropy seeding
#ifdef __unix__
    unsigned char seed[sizeof(SEED)] = {0};
    int urandomFd = -2;
    urandomFd = open("/dev/urandom", O_RDONLY);
    if(urandomFd == -1)
    {
        OIC_LOG(ERROR, TAG, "Fails open /dev/urandom!");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    if(0 > read(urandomFd, seed, sizeof(seed)))
    {
        OIC_LOG(ERROR, TAG, "Fails read from /dev/urandom!");
        close(urandomFd);
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    close(urandomFd);

#else
    unsigned char * seed = (unsigned char *) SEED;
#endif
    // Initialize and seed DRBG context
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    if (0 != mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
                                   &entropy, seed, sizeof(SEED)))
    {
        OIC_LOG(ERROR, TAG, "Seed initialization failed!");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    mbedtls_ctr_drbg_set_prediction_resistance(&ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF);

    // Create CSR
    buf = (unsigned char *)OICMalloc(bufsize * sizeof(unsigned char));
    if (NULL == buf)
    {
        OIC_LOG(ERROR, TAG, "OICMalloc returned NULL on buf allocation");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    ret = mbedtls_x509write_csr_der(&req, buf, bufsize,
                                    mbedtls_ctr_drbg_random, &ctr_drbg);
    if (ret < 0)
    {
        OIC_LOG(ERROR, TAG, "mbedtls_x509write_csr_der error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }

    // CSR to output
    csr->bytes = (uint8_t *)OICMalloc(ret * sizeof(uint8_t));
    if (NULL == csr->bytes)
    {
        OIC_LOG(ERROR, TAG, "OICMalloc returned NULL on csr allocation");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    memcpy(csr->bytes, buf + bufsize - ret, ret * sizeof(uint8_t));
    csr->len = ret;
    // Private key to output
    ret = mbedtls_pk_write_key_der(key, buf, bufsize);
    if (ret < 0)
    {
        OIC_LOG(ERROR, TAG, "mbedtls_pk_write_key_der error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    g_privateKey.bytes = (uint8_t *)OICMalloc(ret * sizeof(char));
    if (NULL == g_privateKey.bytes)
    {
        OIC_LOG(ERROR, TAG, "OICMalloc returned NULL on g_privateKey.bytes allocation");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    memcpy(g_privateKey.bytes, buf + bufsize - ret, ret * sizeof(uint8_t));
    g_privateKey.len = ret;
    // Public key to output
    ret = mbedtls_pk_write_pubkey_der(key, buf, bufsize);
    if (ret < 0)
    {
        OIC_LOG(ERROR, TAG, "mbedtls_pk_write_pubkey_der error");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    //leave this, may be public key will be required in future
    OCByteString publicKey;
    publicKey.bytes = (uint8_t *)OICMalloc(ret * sizeof(char));
    if (NULL == publicKey.bytes)
    {
        OIC_LOG(ERROR, TAG, "OICMalloc returned NULL on pubKey allocation");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return -1;
    }
    memcpy(publicKey.bytes, buf + bufsize - ret, ret * sizeof(uint8_t));
    publicKey.len = ret;
    OICFree(publicKey.bytes);

    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_x509write_csr_free(&req);
    OICFree(key);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return 0;
}

/**
 * Cloud CSR Sign response handler

 * @param[in] ctx       context
 * @param[out] data     data required to external application
 * @param[in] response  peer response
 * @return  OCStackResult application result
 */
static OCStackResult HandleCertificateIssueRequest(void *ctx, void **data, OCClientResponse *response)
{
    OCStackResult result = OC_STACK_OK;
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    OC_UNUSED(ctx);
    OC_UNUSED(data);

    if (!response)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Client response is null",__func__);
        return OC_STACK_INVALID_PARAM;
    }

    if (response->result < 4 && response->payload)
    {
        OIC_LOG_V(ERROR, TAG, "CSR sign error: result: %d, payload null: %s",
                  response->result, response->payload ? "no" : "yes");
        OIC_LOG_PAYLOAD(DEBUG, response->payload);
        return OC_STACK_ERROR;
    }

    char *deviceId = 0;

    if (!OCRepPayloadGetPropString((OCRepPayload *)response->payload,
                                   OC_RSRVD_DEVICE_ID, &deviceId))
    {
        OIC_LOG(ERROR, TAG, "Can't get: Device Id");
        result = OC_STACK_ERROR;
    }

    OicSecKey_t cert;
    if (!OCRepPayloadGetPropPubDataType((OCRepPayload *)response->payload,
                                   OC_RSRVD_CERT, &cert))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_CERT);
        result = OC_STACK_ERROR;
    }
    else
    {
        OicSecKey_t key =
        {
            g_privateKey.bytes,
            g_privateKey.len,
            OIC_ENCODING_DER
        };

        uint16_t credId;
        result = SRPSaveOwnCertChain(&cert, &key, &credId);
        if (result != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Can't add cert");
        }
    }

    //get cacert
    OicSecKey_t caCert;
    if (!OCRepPayloadGetPropPubDataType((OCRepPayload *)response->payload,
                                   OC_RSRVD_CACERT, &caCert))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_CACERT);
        result = OC_STACK_ERROR;
    }
    else
    {
        uint16_t credId;
        result = SRPSaveTrustCertChain(caCert.data, caCert.len, caCert.encoding, &credId);
        if (result != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Can't insert CA cert");
        }
    }

    OICClearMemory(g_privateKey.bytes, g_privateKey.len);
    OICFree(g_privateKey.bytes);
    g_privateKey.bytes = NULL;
    g_privateKey.len   = 0;

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);

    return result;
}

/**
 * Certificate-Issue request function
 *
 * @param[in] endPoint          cloud host and port
 * @return  OCStackResult application result
 */
OCStackResult OCCloudCertificateIssueRequest(void* ctx,
                                             const OCDevAddr *endPoint,
                                             OCCloudResponseCB callback)
{
    OCStackResult ret = OC_STACK_OK;

    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    if (NULL == endPoint)
    {
        OIC_LOG(ERROR, TAG, "Input parameter endpoint is NULL");
        return OC_STACK_INVALID_PARAM;
    }

    char *deviceId = getDeviceId();

    char subject[MAX_STRING_LEN] = { 0 };
    CSRMakeSubject(subject, "KR", "Samsung", "OCF Device", deviceId);

    OIC_LOG_V(DEBUG, TAG, "Certificate Request subject: %s", subject);

    OCByteString request;
    if (0 != GenerateCSR(subject, &request))
    {
        OIC_LOG(ERROR, TAG, "Cann't get the sertificate request");
        ret = OC_STACK_ERROR;
        goto exit;
    }
    OIC_LOG(DEBUG, TAG, "Certificate Request:");
    OIC_LOG_BUFFER(DEBUG, TAG, request.bytes, request.len);

    OIC_LOG(DEBUG, TAG, "Private Key:");
    OIC_LOG_BUFFER(DEBUG, TAG, g_privateKey.bytes, g_privateKey.len);

    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, TAG, "Failed to memory allocation");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_DEVICE_ID, deviceId);

    OicSecKey_t csr = {.data = request.bytes, .len = request.len, .encoding = OIC_ENCODING_DER};

    OCRepPayloadSetPropPubDataType(payload, OC_RSRVD_CSR, &csr);

    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *)payload);

    char uri[MAX_URI_QUERY] = { 0 };
    snprintf(uri, MAX_URI_QUERY, DEFAULT_QUERY,
             endPoint->addr, endPoint->port,
             OC_RSRVD_PROV_CERT_URI);
    OIC_LOG_V(DEBUG, TAG, "Certificate Request Query: %s", uri);

    OCCallbackData cbData;
    fillCallbackData(&cbData, ctx, callback, HandleCertificateIssueRequest, NULL);

    ret = OCDoResource(NULL, OC_REST_POST, uri, NULL,
                       (OCPayload *)payload,
                       CT_ADAPTER_TCP,
                       OC_LOW_QOS, &cbData, NULL, 0);

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);
exit:
    OICFree(request.bytes);
    return ret;
}
