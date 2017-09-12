//******************************************************************
//
// Copyright 2017 Microsoft
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//******************************************************************

#include "iotivity_config.h"
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "oic_string.h"
#include "cainterface.h"
#include "payload_logging.h"
#include "ocstack.h"
#include "ocrandom.h"
#include "cacommon.h"
#include "srmresourcestrings.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "credresource.h"
#include "doxmresource.h"
#include "srmutility.h"
#include "certhelpers.h"
#include "csrresource.h"
#include "resourcemanager.h"

#define TAG  "OIC_SRM_CSR"

static const uint8_t CSR_MAP_SIZE = 4; // csr, encoding, RT, and IF

static OCResourceHandle    gCsrHandle = NULL;

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size.
 */
static const uint16_t CBOR_SIZE = 2048;

static const char* EncodingValueToString(OicEncodingType_t encoding)
{
    switch (encoding)
    {
    case OIC_ENCODING_RAW:    return OIC_SEC_ENCODING_RAW;
    case OIC_ENCODING_BASE64: return OIC_SEC_ENCODING_BASE64;
    case OIC_ENCODING_DER:    return OIC_SEC_ENCODING_DER;
    case OIC_ENCODING_PEM:    return OIC_SEC_ENCODING_PEM;
    default:                  return NULL;
    }
}


static OCStackResult StoreKeyPair(mbedtls_pk_context *keyPair, const OicUuid_t *myUuid)
{
    int ret = 0;
    OicSecCred_t *cred = NULL;
    OicSecKey_t publicData;
    OicSecKey_t privateData;
    uint8_t publicBuf[1024];
    uint8_t privateBuf[1024];

    /* These DER writing APIs write at the END of the buffers, hence the pointer arithmetic. See the API
     * documentation for mbedTLS.
     */

    ret = mbedtls_pk_write_pubkey_der(keyPair, publicBuf, sizeof(publicBuf));
    VERIFY_SUCCESS(TAG, 0 <= ret, ERROR);
    publicData.data = publicBuf + sizeof(publicBuf) - ret;
    publicData.len = ret;
    publicData.encoding = OIC_ENCODING_DER;

    ret = mbedtls_pk_write_key_der(keyPair, privateBuf, sizeof(privateBuf));
    VERIFY_SUCCESS(TAG, 0 <= ret, ERROR);
    privateData.data = privateBuf + sizeof(privateBuf) - ret;
    privateData.len = ret;
    privateData.encoding = OIC_ENCODING_DER;

    cred = GenerateCredential(myUuid, ASYMMETRIC_KEY, &publicData, &privateData, myUuid, NULL);
    VERIFY_NOT_NULL(TAG, cred, ERROR);
    cred->credUsage = OICStrdup(PRIMARY_CERT);
    VERIFY_NOT_NULL(TAG, cred->credUsage, ERROR);

    VERIFY_SUCCESS(TAG, OC_STACK_OK == AddCredential(cred), ERROR);

    return OC_STACK_OK;

exit:

    if (NULL != cred)
    {
        DeleteCredList(cred);
    }

    OICClearMemory(privateBuf, sizeof(privateBuf));

    return OC_STACK_ERROR;
}

static OCStackResult CSRToCBORPayload(const uint8_t *csr, size_t csrLen, OicEncodingType_t encoding, uint8_t **cborPayload, size_t *cborSize)
{
    OCStackResult ret = OC_STACK_ERROR;

    CborError cborEncoderResult = CborNoError;
    uint8_t *outPayload = NULL;
    size_t cborLen = *cborSize;
    CborEncoder encoder;
    CborEncoder csrRootMap;
    const char *strEncoding = NULL;

    if ((NULL == csr) || (0 == csrLen) || (NULL == cborPayload) || (NULL == cborSize))
    {
        return OC_STACK_INVALID_PARAM;
    }

    if ((OIC_ENCODING_DER != encoding) && (OIC_ENCODING_PEM != encoding))
    {
        return OC_STACK_INVALID_PARAM;
    }

    *cborSize = 0;
    *cborPayload = NULL;

    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_NO_MEMORY);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create CSR Root Map (csr)
    cborEncoderResult = cbor_encoder_create_map(&encoder, &csrRootMap, CSR_MAP_SIZE);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CSR Root Map");

    // Create CSR string entry
    cborEncoderResult = cbor_encode_text_string(&csrRootMap, OIC_JSON_CSR_NAME, strlen(OIC_JSON_CSR_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding CSR name.");
    cborEncoderResult = cbor_encode_byte_string(&csrRootMap, csr, csrLen);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding CSR value.");

    // Create encoding entry
    cborEncoderResult = cbor_encode_text_string(&csrRootMap, OIC_JSON_ENCODING_NAME, strlen(OIC_JSON_ENCODING_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding encoding name.");
    strEncoding = EncodingValueToString(encoding);
    assert(strEncoding != NULL);
    cborEncoderResult = cbor_encode_text_string(&csrRootMap, strEncoding, strlen(strEncoding));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding encoding value.");

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&csrRootMap, OIC_JSON_RT_NAME,
        strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&csrRootMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_CSR,
            strlen(OIC_RSRC_TYPE_SEC_CSR));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&csrRootMap, &rtArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing RT.");

    //IF-- Mandatory
    CborEncoder ifArray;
    cborEncoderResult = cbor_encode_text_string(&csrRootMap, OIC_JSON_IF_NAME,
        strlen(OIC_JSON_IF_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&csrRootMap, &ifArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
            strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&csrRootMap, &ifArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing IF.");

    // Close CSR Map
    cborEncoderResult = cbor_encoder_close_container(&encoder, &csrRootMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing CSR root Map.");

    if (CborNoError == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "CSRToCBORPayload Succeeded");
        *cborSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
        *cborPayload = outPayload;
        ret = OC_STACK_OK;
    }
exit:
    if (CborErrorOutOfMemory == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "CSRToCBORPayload:CborErrorOutOfMemory : retry with more memory");

        // reallocate and try again!
        OICFree(outPayload);
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        cborEncoderResult = CborNoError;
        ret = CSRToCBORPayload(csr, csrLen, encoding, cborPayload, &cborLen);
        *cborSize = cborLen;
    }
    else if (cborEncoderResult != CborNoError)
    {
        OIC_LOG(ERROR, TAG, "Failed to CSRToCBORPayload");
        OICFree(outPayload);
        outPayload = NULL;
        *cborSize = 0;
        *cborPayload = NULL;
        ret = OC_STACK_ERROR;
    }

    return ret;
}

OCStackResult CBORPayloadToCSR(const uint8_t *cborPayload, size_t size, uint8_t **csr, size_t *csrLen, OicEncodingType_t *encoding)
{
    if (NULL == cborPayload || 0 == size || NULL == csr || NULL == csrLen)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    CborValue csrCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;
    uint8_t* cborCsr = NULL;
    size_t cborCsrLen = 0;
    char* tagName = NULL;
    size_t len = 0;

    cborFindResult = cbor_parser_init(cborPayload, size, 0, &parser, &csrCbor);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to initialize parser.");

    if (!cbor_value_is_container(&csrCbor))
    {
        return OC_STACK_ERROR;
    }

    // Enter CSR Root Map
    CborValue csrRootMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    cborFindResult = cbor_value_enter_container(&csrCbor, &csrRootMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering CSR Root Map.");

    while (cbor_value_is_valid(&csrRootMap))
    {
        if (NULL != tagName)
        {
            free(tagName);
            tagName = NULL;
        }
        len = 0;
        CborType type = cbor_value_get_type(&csrRootMap);
        if (type == CborTextStringType && cbor_value_is_text_string(&csrRootMap))
        {
            cborFindResult = cbor_value_dup_text_string(&csrRootMap, &tagName, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding name in CSR Root Map.");
            cborFindResult = cbor_value_advance(&csrRootMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing value in CSR Root Map.");
        }
        if (NULL != tagName)
        {
            if (strcmp(tagName, OIC_JSON_CSR_NAME) == 0 && cbor_value_is_byte_string(&csrRootMap))
            {
                cborFindResult = cbor_value_dup_byte_string(&csrRootMap, &cborCsr, &cborCsrLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding name in CSR Map");
                // Use our own heap allocator and copy the result in so callers can use OICFree.
                *csr = (uint8_t *)OICCalloc(1, cborCsrLen);
                VERIFY_NOT_NULL(TAG, *csr, ERROR);
                memcpy(*csr, cborCsr, cborCsrLen);
                free(cborCsr);
                cborCsr = NULL;
                *csrLen = cborCsrLen;
                cborCsrLen = 0;
            }
            else if (strcmp(tagName, OIC_JSON_ENCODING_NAME) == 0 && cbor_value_is_text_string(&csrRootMap))
            {
                char *strEncoding = NULL;
                cborFindResult = cbor_value_dup_text_string(&csrRootMap, &strEncoding, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding encoding type");

                if (strcmp(strEncoding, OIC_SEC_ENCODING_DER) == 0)
                {
                    *encoding = OIC_ENCODING_DER;
                }
                else if (strcmp(strEncoding, OIC_SEC_ENCODING_PEM) == 0)
                {
                    *encoding = OIC_ENCODING_PEM;
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "Invalid encoding type %s", strEncoding);
                    ret = OC_STACK_ERROR;
                    free(strEncoding);
                    goto exit;
                }

                free(strEncoding);
            }
            else
            {
                // Ignore any other tag type for now.
                OIC_LOG_V(WARNING, TAG, "Unknown tag %s", tagName);
            }

        }
        if (cbor_value_is_valid(&csrRootMap))
        {
            cborFindResult = cbor_value_advance(&csrRootMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing CSR Root Map");
        }
    }

    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        if (NULL != *csr)
        {
            OICFreeAndSetToNull((void **)csr);
        }

        *csrLen = 0;

        ret = OC_STACK_ERROR;
    }

    // tagName and cborCsr are both allocated internally by tinycbor, which uses malloc, not OICMalloc.
    // Therefore, they must be freed with free, not OICFree.
    if (NULL != tagName)
    {
        free(tagName);
    }
    if (NULL != cborCsr)
    {
        free(cborCsr);
    }

    return ret;

}

static OCEntityHandlerResult HandleCsrGetRequest(OCEntityHandlerRequest * ehRequest)
{
    OCStackResult res = OC_STACK_OK;
    OCEntityHandlerResult ehRet = OC_EH_OK;
    int ret = 0;
    mbedtls_pk_context keyPair;
    OicUuid_t myUuid;
    char *myUuidStr = NULL;
    char *myDNStr = NULL;
    size_t myDNStrLen = 0;
    ByteArray_t keyData = { .data = NULL, .len = 0 };
    OCByteString csr = { .bytes = NULL, .len = 0 };
    size_t size = 0;
    uint8_t *payload = NULL;

    OIC_LOG(INFO, TAG, "HandleCsrGetRequest  processing GET request");

    res = GetDoxmDeviceID(&myUuid);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);
    res = ConvertUuidToStr(&myUuid, &myUuidStr);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);

    // Retrieve the key from our current certificate if present, otherwise create a new key
    GetDerKey(&keyData, PRIMARY_CERT);
    mbedtls_pk_init(&keyPair);
    if (0 < keyData.len)
    {
        ret = mbedtls_pk_parse_key(&keyPair, keyData.data, keyData.len, NULL, 0);
        if (0 > ret)
        {
            res = OC_STACK_ERROR;
            // Because we need to set res before failing out, this is a redundant check of ret, but gives better logging.
            VERIFY_SUCCESS(TAG, 0 <= ret, ERROR);
        }
    }
    else
    {
        ret = OCInternalGenerateKeyPair(&keyPair);
        if (0 > ret)
        {
            res = OC_STACK_ERROR;
            VERIFY_SUCCESS(TAG, 0 <= ret, ERROR);
        }
        VERIFY_SUCCESS(TAG, OC_STACK_OK == (res = StoreKeyPair(&keyPair, &myUuid)), ERROR);
    }

    // Generate CSR
    myDNStrLen = strlen(myUuidStr) + sizeof(SUBJECT_PREFIX); // sizeof(SUBJECT_PREFIX) will also count the byte for terminating null.
    myDNStr = (char *)OICMalloc(myDNStrLen);
    VERIFY_NOT_NULL(TAG, myDNStr, ERROR);
    ret = snprintf(myDNStr, myDNStrLen, SUBJECT_PREFIX "%s", myUuidStr);
    VERIFY_SUCCESS(TAG, 0 <= ret, ERROR);
    /* ret >= myDNStrLen means there was truncation; assert because this means a code bug. */
    assert((size_t)ret < myDNStrLen);
    ret = OCInternalCSRRequest(myDNStr, &keyPair, OIC_ENCODING_DER, &csr);
    if (0 > ret)
    {
        res = OC_STACK_ERROR;
        VERIFY_SUCCESS(TAG, 0 <= ret, ERROR);
    }

    // Convert CSR data into CBOR for transmission
    res = CSRToCBORPayload(csr.bytes, csr.len, OIC_ENCODING_DER, &payload, &size);

exit:

    ehRet = (OC_STACK_OK == res) ? OC_EH_OK : OC_EH_ERROR;

    //Send payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, payload, size)) == OC_STACK_OK) ?
                       OC_EH_OK : OC_EH_ERROR;

    mbedtls_pk_free(&keyPair);
    OICFree(myDNStr);
    OICFree(myUuidStr);
    OICFree(keyData.data);
    OICFree(csr.bytes);
    OICFree(payload);
    return ehRet;
}

static OCEntityHandlerResult CredCsrEntityHandler(OCEntityHandlerFlag flag,
                                                  OCEntityHandlerRequest * ehRequest,
                                                  void* callbackParameter)
{
    OC_UNUSED(callbackParameter);
    OCEntityHandlerResult ret = OC_EH_ERROR;

    if (!ehRequest)
    {
        return OC_EH_ERROR;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        if (ehRequest->method == OC_REST_GET)
        {
            ret = HandleCsrGetRequest(ehRequest);
        }
        else
        {
            /* Only GET supported on CSR resource. */
            ret = OC_EH_ERROR;
        }
    }

    return ret;
}

OCStackResult InitCSRResource()
{
    OCStackResult ret = OCCreateResource(&gCsrHandle,
        OIC_RSRC_TYPE_SEC_CSR,
        OC_RSRVD_INTERFACE_DEFAULT,
        OIC_RSRC_CSR_URI,
        CredCsrEntityHandler,
        NULL,
        OC_SECURE |
        OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG(FATAL, TAG, "Unable to instantiate CSR resource");
    }

    return ret;
}

OCStackResult DeInitCSRResource()
{
    OCStackResult res = OCDeleteResource(gCsrHandle);

    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to delete CSR resource: %d", res);
    }

    gCsrHandle = NULL;

    return res;
}