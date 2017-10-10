//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
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
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define __STDC_LIMIT_MACROS

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

#include "cainterface.h"
#include "payload_logging.h"
#include "ocstack.h"
#include "ocrandom.h"
#include "base64.h"
#include "ocserverrequest.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "utlist.h"
#include "credresource.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "iotvticalendar.h"
#include "pbkdf2.h"
#include "resourcemanager.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "psinterface.h"
#include "pinoxmcommon.h"
#include "certhelpers.h"
#include "cacommon.h"
#include "secureresourcemanager.h"
#include "deviceonboardingstate.h"

#ifdef __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
#include <mbedtls/ssl_ciphersuites.h>
#include <mbedtls/pk.h>
#include <mbedtls/base64.h>
#include <mbedtls/pem.h>
#endif

#define TAG  "OIC_SRM_CREDL"

#ifdef HAVE_WINDOWS_H
#include <wincrypt.h>
#include <intsafe.h>
#endif

/** Max credential types number used for TLS */
#define MAX_TYPE 2
/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 2048;

/** CRED size - Number of mandatory items. */
static const uint8_t CRED_ROOT_MAP_SIZE = 4;
static const uint8_t CRED_EMPTY_ROOT_MAP_SIZE = 2;
static const uint8_t CRED_MAP_SIZE = 3;
static const uint8_t ROLEID_MAP_SIZE = 1;


static OicSecCred_t        *gCred = NULL;
static OCResourceHandle    gCredHandle = NULL;
static OicUuid_t           gRownerId = { .id = { 0 } };

typedef enum CredCompareResult{
    CRED_CMP_EQUAL = 0,
    CRED_CMP_NOT_EQUAL = 1,
    CRED_CMP_ERROR = 2
}CredCompareResult_t;

typedef struct CredIdList CredIdList_t;

struct CredIdList
{
    uint16_t credId;
    CredIdList_t *next;
};

static void DeleteCredIdList(CredIdList_t** list)
{
    if (list)
    {
        CredIdList_t *head = *list;

        while (head)
        {
            CredIdList_t *tmp = head->next;
            OICFree(head);
            head = tmp;
        }
    }
}

static bool ValueWithinBounds(uint64_t value, uint64_t maxValue)
{
    if (value > maxValue)
    {
        OIC_LOG_V(ERROR, TAG, "The value (%" PRId64 ") is greater than allowed maximum of %" PRId64 ".", value, maxValue);
        return false;
    }

    return true;
}

/**
 * Internal function to check a subject of SIGNED_ASYMMETRIC_KEY(Certificate).
 * If that subject is NULL or wildcard, set it to own deviceID.
 * @param cred credential on SVR DB file
 * @param deviceID own deviceuuid of doxm resource
 *
 * @return
 *     true successfully done
 *     false Invalid cred
 */

static bool CheckSubjectOfCertificate(OicSecCred_t* cred, OicUuid_t deviceID)
{
    OIC_LOG(DEBUG, TAG, "IN CheckSubjectOfCertificate");
    VERIFY_NOT_NULL(TAG, cred, ERROR);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    const OicUuid_t emptyUuid = { .id = { 0 } };

    if ( SIGNED_ASYMMETRIC_KEY == cred->credType)
    {
        if((0 == memcmp(cred->subject.id, emptyUuid.id, sizeof(cred->subject.id))) ||
            (0 == memcmp(cred->subject.id, &WILDCARD_SUBJECT_ID, sizeof(cred->subject.id))))
        {
            memcpy(cred->subject.id, deviceID.id, sizeof(deviceID.id));
        }
    }
#else
    OC_UNUSED(deviceID);
#endif

    OIC_LOG(DEBUG, TAG, "OUT CheckSubjectOfCertificate");
    return true;
exit:
    OIC_LOG(ERROR, TAG, "OUT CheckSubjectOfCertificate");
    return false;
}

/**
 * Internal function to check credential
 */
static bool IsValidCredential(const OicSecCred_t* cred)
{
    OicUuid_t emptyUuid = {.id={0}};


    OIC_LOG(DEBUG, TAG, "IN IsValidCredential");

    VERIFY_NOT_NULL(TAG, cred, ERROR);
    VERIFY_SUCCESS(TAG, 0 != cred->credId, ERROR);
    OIC_LOG_V(DEBUG, TAG, "Cred ID = %d", cred->credId);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    OIC_LOG_V(DEBUG, TAG, "Cred Type = %d", cred->credType);

    switch(cred->credType)
    {
        case SYMMETRIC_PAIR_WISE_KEY:
        case SYMMETRIC_GROUP_KEY:
        case PIN_PASSWORD:
        {
            VERIFY_NOT_NULL(TAG, cred->privateData.data, ERROR);
            VERIFY_SUCCESS(TAG, 0 != cred->privateData.len, ERROR);
            VERIFY_SUCCESS(TAG, \
                           (OIC_ENCODING_RAW == cred->privateData.encoding || \
                           OIC_ENCODING_BASE64 == cred->privateData.encoding), \
                           ERROR);
            break;
        }
        case ASYMMETRIC_KEY:
        {
            VERIFY_NOT_NULL(TAG, cred->publicData.data, ERROR);
            VERIFY_SUCCESS(TAG, 0 != cred->publicData.len, ERROR);
            VERIFY_SUCCESS(TAG, \
                           (OIC_ENCODING_UNKNOW < cred->publicData.encoding && \
                            OIC_ENCODING_DER >= cred->publicData.encoding),
                           ERROR);
            break;
        }
        case SIGNED_ASYMMETRIC_KEY:
        {
            VERIFY_SUCCESS(TAG, (NULL != cred->publicData.data ||NULL != cred->optionalData.data) , ERROR);
            VERIFY_SUCCESS(TAG, (0 != cred->publicData.len || 0 != cred->optionalData.len), ERROR);

            if(NULL != cred->optionalData.data)
            {
                VERIFY_SUCCESS(TAG, \
                               (OIC_ENCODING_UNKNOW < cred->optionalData.encoding && \
                                OIC_ENCODING_DER >= cred->optionalData.encoding),
                               ERROR);
            }
            break;
        }
        case ASYMMETRIC_ENCRYPTION_KEY:
        {
            VERIFY_NOT_NULL(TAG, cred->privateData.data, ERROR);
            VERIFY_SUCCESS(TAG, 0 != cred->privateData.len, ERROR);
            VERIFY_SUCCESS(TAG, \
                           (OIC_ENCODING_UNKNOW < cred->privateData.encoding && \
                            OIC_ENCODING_DER >= cred->privateData.encoding),
                           ERROR);
            break;
        }
        default:
        {
            OIC_LOG(WARNING, TAG, "Unknown credential type");
            return false;
        }
    }
#endif

    VERIFY_SUCCESS(TAG, 0 != memcmp(emptyUuid.id, cred->subject.id, sizeof(cred->subject.id)), ERROR);

    OIC_LOG(DEBUG, TAG, "OUT IsValidCredential");
    return true;
exit:
    OIC_LOG(WARNING, TAG, "OUT IsValidCredential : Invalid Credential detected.");
    return false;
}

static bool IsEmptyCred(const OicSecCred_t* cred)
{
    OicUuid_t emptyUuid = {.id={0}};

    VERIFY_SUCCESS(TAG, (0 == memcmp(cred->subject.id, emptyUuid.id, sizeof(emptyUuid))), ERROR);
    VERIFY_SUCCESS(TAG, !IsNonEmptyRole(&cred->roleId), ERROR);
    VERIFY_SUCCESS(TAG, (0 == cred->credId), ERROR);
    VERIFY_SUCCESS(TAG, (0 == cred->credType), ERROR);
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    VERIFY_SUCCESS(TAG, (NULL == cred->privateData.data), ERROR);
    VERIFY_SUCCESS(TAG, (NULL == cred->publicData.data), ERROR);
    VERIFY_SUCCESS(TAG, (NULL == cred->optionalData.data), ERROR);
    VERIFY_SUCCESS(TAG, (NULL == cred->credUsage), ERROR);
#endif
    return true;
exit:
    return false;
}

/**
 * This function frees OicSecCred_t object's fields and object itself.
 */
void FreeCred(OicSecCred_t *cred)
{
    if(NULL == cred)
    {
        OIC_LOG(ERROR, TAG, "Invalid Parameter");
        return;
    }
    //Note: Need further clarification on roleID data type
#if 0
    //Clean roleIds
    OICFree(cred->roleIds);
#endif

    //Clean PublicData/OptionalData/Credusage
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
     // TODO: Need to check credUsage.
    OICFree(cred->publicData.data);
    OICFree(cred->optionalData.data);
    OICFree(cred->credUsage);

#endif /* __WITH_DTLS__ ||  __WITH_TLS__*/

    //Clean PrivateData
    OICClearMemory(cred->privateData.data, cred->privateData.len);
    OICFree(cred->privateData.data);

    //Clean Period
    OICFree(cred->period);

#ifdef MULTIPLE_OWNER
    //Clean eowner
    OICFree(cred->eownerID);
#endif

    cred->next = NULL;

    //Clean Cred node itself
    OICFree(cred);
}

void DeleteCredList(OicSecCred_t* cred)
{
    if (cred)
    {
        OicSecCred_t *credTmp1 = NULL, *credTmp2 = NULL;
        LL_FOREACH_SAFE(cred, credTmp1, credTmp2)
        {
            LL_DELETE(cred, credTmp1);
            FreeCred(credTmp1);
        }
    }
}

size_t GetCredKeyDataSize(const OicSecCred_t* cred)
{
    size_t size = 0;
    if (cred)
    {
        OicSecCred_t *credPtr = NULL, *credTmp = NULL;
        LL_FOREACH_SAFE((OicSecCred_t*)cred, credPtr, credTmp)
        {
            if (credPtr->privateData.data && 0 < credPtr->privateData.len)
            {
                size += credPtr->privateData.len;
            }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            if (credPtr->publicData.data && 0 < credPtr->publicData.len)
            {
                size += credPtr->publicData.len;
            }
            if (credPtr->optionalData.data && 0 < credPtr->optionalData.len)
            {
                size += credPtr->optionalData.len;
            }
#endif
        }
    }
    OIC_LOG_V(DEBUG, TAG, "Cred Key Data Size : %u", (unsigned int) size);
    return size;
}

static size_t OicSecCredCount(const OicSecCred_t *secCred)
{
    size_t size = 0;
    for (const OicSecCred_t *cred = secCred; cred; cred = cred->next)
    {
        size++;
    }
    return size;
}

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

static CborError SerializeEncodingToCborInternal(CborEncoder *map, const OicSecKey_t *value)
{
    CborError cborEncoderResult = CborNoError;
    const char *encoding = EncodingValueToString(value->encoding);
    if (encoding)
    {
        cborEncoderResult = cbor_encode_text_string(map, OIC_JSON_ENCODING_NAME,
            strlen(OIC_JSON_ENCODING_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Encoding Tag.");
        cborEncoderResult = cbor_encode_text_string(map, encoding,
            strlen(encoding));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Encoding Value.");

        cborEncoderResult = cbor_encode_text_string(map, OIC_JSON_DATA_NAME,
            strlen(OIC_JSON_DATA_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Name Tag.");
        if (OIC_ENCODING_DER == value->encoding ||
            OIC_ENCODING_RAW == value->encoding)
        {
            cborEncoderResult = cbor_encode_byte_string(map,
                    value->data, value->len);
        }
        else
        {
            cborEncoderResult = cbor_encode_text_string(map,
                    (char*)value->data, value->len);
        }
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Name Value.");
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "%s: Unknown encoding type: %u.", __func__, value->encoding);
        return CborErrorUnknownType;
    }
    exit:
    return cborEncoderResult;
}

CborError SerializeEncodingToCbor(CborEncoder *rootMap, const char* tag, const OicSecKey_t *value)
{
    CborError cborEncoderResult = CborNoError;
    CborEncoder map;
    const size_t mapSize = 2;

    cborEncoderResult = cbor_encode_text_string(rootMap, tag, strlen(tag));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");

    cborEncoderResult = cbor_encoder_create_map(rootMap, &map, mapSize);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Map");

    cborEncoderResult = SerializeEncodingToCborInternal(&map, value);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding OicSecKey_t structure");

    cborEncoderResult = cbor_encoder_close_container(rootMap, &map);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing Map.");

    exit:
    return cborEncoderResult;
}

CborError SerializeSecOptToCbor(CborEncoder *rootMap, const char* tag, const OicSecOpt_t *value)
{
    CborError cborEncoderResult = CborNoError;
    CborEncoder map;
    const size_t mapSize = 3;

    cborEncoderResult = cbor_encode_text_string(rootMap, tag, strlen(tag));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");

    cborEncoderResult = cbor_encoder_create_map(rootMap, &map, mapSize);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Map");

    OicSecKey_t in;
    in.data = value->data;
    in.encoding = value->encoding;
    in.len = value->len;

    cborEncoderResult = SerializeEncodingToCborInternal(&map, &in);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding OicSecKey_t structure");

    cborEncoderResult = cbor_encode_text_string(&map, OIC_JSON_REVOCATION_STATUS_NAME,
        strlen(OIC_JSON_REVOCATION_STATUS_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding optional revstat Tag.");
    cborEncoderResult = cbor_encode_boolean(&map, value->revstat);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding optional revstat Value.");

    cborEncoderResult = cbor_encoder_close_container(rootMap, &map);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing Map.");

    exit:
    return cborEncoderResult;
}

static CborError DeserializeEncodingFromCborInternal(CborValue *map, char *name, OicSecKey_t *value)
{
    size_t len = 0;
    CborError cborFindResult = CborNoError;

    // data -- Mandatory
    if (strcmp(name, OIC_JSON_DATA_NAME) == 0)
    {
        if(cbor_value_is_byte_string(map))
        {
            cborFindResult = cbor_value_dup_byte_string(map, &value->data,
                &value->len, NULL);
        }
        else if(cbor_value_is_text_string(map))
        {
            cborFindResult = cbor_value_dup_text_string(map, (char**)(&value->data),
                &value->len, NULL);
        }
        else
        {
            cborFindResult = CborErrorUnknownType;
            OIC_LOG(ERROR, TAG, "Unknown type for private data.");
        }
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding PrivateData.");
    }

    // encoding -- Mandatory
    if (strcmp(name, OIC_JSON_ENCODING_NAME) == 0)
    {
        char* strEncoding = NULL;
        cborFindResult = cbor_value_dup_text_string(map, &strEncoding, &len, NULL);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding EncodingType");

        if(strcmp(strEncoding, OIC_SEC_ENCODING_RAW) == 0)
        {
            value->encoding = OIC_ENCODING_RAW;
        }
        else if(strcmp(strEncoding, OIC_SEC_ENCODING_BASE64) == 0)
        {
            value->encoding = OIC_ENCODING_BASE64;
        }
        else if(strcmp(strEncoding, OIC_SEC_ENCODING_DER) == 0)
        {
            value->encoding = OIC_ENCODING_DER;
        }
        else if(strcmp(strEncoding, OIC_SEC_ENCODING_PEM) == 0)
        {
            value->encoding = OIC_ENCODING_PEM;
        }
        else
        {
            //For unit test
            value->encoding = OIC_ENCODING_RAW;
            OIC_LOG_V(WARNING, TAG, "%s: Unknown encoding type detected.", __func__);
        }
        //Because cbor using malloc directly, it is required to use free() instead of OICFree
        free(strEncoding);
    }
    exit:
    return cborFindResult;
}

CborError DeserializeEncodingFromCbor(CborValue *rootMap, OicSecKey_t *value)
{
    CborValue map = { .parser = NULL };
    CborError cborFindResult = cbor_value_enter_container(rootMap, &map);
    size_t len = 0;

    while (cbor_value_is_valid(&map))
    {
        char* name = NULL;
        CborType type = cbor_value_get_type(&map);
        if (type == CborTextStringType && cbor_value_is_text_string(&map))
        {
            cborFindResult = cbor_value_dup_text_string(&map, &name, &len, NULL);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to get text");
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to advance value");
        }
        if (name)
        {
            cborFindResult = DeserializeEncodingFromCborInternal(&map, name, value);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read OicSecKey_t value");
        }
        if (cbor_value_is_valid(&map))
        {
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Map.");
        }
        //Because cbor using malloc directly, it is required to use free() instead of OICFree
        free(name);
    }
    exit:
    return cborFindResult;
}

CborError DeserializeSecOptFromCbor(CborValue *rootMap, OicSecOpt_t *value)
{
    CborValue map = { .parser = NULL };
    CborError cborFindResult = cbor_value_enter_container(rootMap, &map);
    size_t len = 0;
    value->revstat = false;

    while (cbor_value_is_valid(&map))
    {
        char* name = NULL;
        CborType type = cbor_value_get_type(&map);
        if (type == CborTextStringType && cbor_value_is_text_string(&map))
        {
            cborFindResult = cbor_value_dup_text_string(&map, &name, &len, NULL);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to get text");
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to advance value");
        }
        if (name)
        {
            // OptionalData::revstat -- Mandatory
            if (strcmp(name, OIC_JSON_REVOCATION_STATUS_NAME) == 0)
            {
                cborFindResult = cbor_value_get_boolean(&map, &value->revstat);
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding revstat Value.")
            }
            OicSecKey_t out;
            cborFindResult = DeserializeEncodingFromCborInternal(&map, name, &out);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read OicSecKey_t value");

            value->data = out.data;
            value->encoding = out.encoding;
            value->len = out.len;
        }
        if (cbor_value_is_valid(&map))
        {
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Map.");
        }
        //Because cbor using malloc directly, it is required to use free() instead of OICFree
        free(name);
    }
    exit:
    return cborFindResult;
}

/* Produce debugging output for all credentials, output metadata. */
static void logCredMetadata()
{
#if defined(TB_LOG)
    OicSecCred_t * temp = NULL;
    size_t count = 0;
    char uuidString[UUID_STRING_SIZE];
    OicUuid_t ownUuid;

    OIC_LOG_V(DEBUG, TAG, "IN %s:", __func__);

    if (GetDoxmDeviceID(&ownUuid) == OC_STACK_OK && OCConvertUuidToString(ownUuid.id, uuidString))
    {
        OIC_LOG_V(DEBUG, TAG, "Own UUID: %s", uuidString);
    }

    LL_FOREACH(gCred, temp)
    {
        count++;
        OIC_LOG(DEBUG, TAG, " ");
        OIC_LOG_V(DEBUG, TAG, "Cred ID: %d", temp->credId);
        if (OCConvertUuidToString(temp->subject.id, uuidString))
        {
            OIC_LOG_V(DEBUG, TAG, "Subject UUID: %s", uuidString);
        }
        if (IsNonEmptyRole(&temp->roleId))
        {
            OIC_LOG_V(DEBUG, TAG, "Role ID: %s", temp->roleId.id);
            OIC_LOG_V(DEBUG, TAG, "Role authority: %s", temp->roleId.authority);
        }
        OIC_LOG_V(DEBUG, TAG, "Cred Type: %d", temp->credType);
        OIC_LOG_V(DEBUG, TAG, "privateData length: %" PRIuPTR ", encoding: %d", temp->privateData.len, temp->privateData.encoding);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        OIC_LOG_V(DEBUG, TAG, "publicData length: %" PRIuPTR ", encoding: %d", temp->publicData.len, temp->publicData.encoding);
        if (temp->credUsage)
        {
            OIC_LOG_V(DEBUG, TAG, "credUsage: %s", temp->credUsage);
        }

        OIC_LOG_V(DEBUG, TAG, "optionalData length: %" PRIuPTR", encoding: %d" PRIuPTR, temp->optionalData.len, temp->optionalData.encoding);
#endif

    }

    OIC_LOG_V(DEBUG, TAG, "Found %" PRIuPTR " credentials.", count);

    OIC_LOG_V(DEBUG, TAG, "OUT %s:", __func__);
#endif
}

OCStackResult CredToCBORPayload(const OicSecCred_t *credS, uint8_t **cborPayload,
                                size_t *cborSize, int secureFlag)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s:", __func__);
    if (NULL == cborPayload || NULL != *cborPayload || NULL == cborSize)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;

    CborError cborEncoderResult = CborNoError;
    uint8_t *outPayload = NULL;
    size_t cborLen = *cborSize;
    *cborSize = 0;
    *cborPayload = NULL;
    const OicSecCred_t *cred = credS;
    CborEncoder encoder;
    CborEncoder credArray;
    CborEncoder credRootMap;
    CborEncoder roleIdMap;

    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    size_t credRootMapSize = CRED_ROOT_MAP_SIZE;

    // Create CRED Root Map (creds, rownerid)
    cborEncoderResult = cbor_encoder_create_map(&encoder, &credRootMap, credRootMapSize);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Root Map");

    // creds
    cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_CREDS_NAME,
        strlen(OIC_JSON_CREDS_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding creds Name Tag.");

    // creds array
    cborEncoderResult = cbor_encoder_create_array(&credRootMap, &credArray, OicSecCredCount(cred));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Array.");

    while (cred)
    {
        CborEncoder credMap;
        size_t mapSize = CRED_MAP_SIZE;
        size_t inLen = 0;
        if (cred->period)
        {
            mapSize++;
        }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#ifdef MULTIPLE_OWNER
        if(cred->eownerID)
        {
            mapSize++;
        }
#endif //MULTIPLE_OWNER

        if ((SIGNED_ASYMMETRIC_KEY == cred->credType || ASYMMETRIC_KEY == cred->credType)
            && cred->publicData.data)
        {
            mapSize++;
        }
        if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->optionalData.data)
        {
            mapSize++;
        }
        if (cred->credUsage)
        {
            mapSize++;
        }
        if (IsNonEmptyRole(&cred->roleId))
        {
            mapSize++;
        }
#endif /* __WITH_DTLS__ ||  __WITH_TLS__*/
        if (!secureFlag && cred->privateData.data)
        {
            mapSize++;
        }
        cborEncoderResult = cbor_encoder_create_map(&credArray, &credMap, mapSize);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Map");

        //CredID -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDID_NAME,
            strlen(OIC_JSON_CREDID_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Id Tag. ");
        cborEncoderResult = cbor_encode_int(&credMap, cred->credId);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Id Value.");

        //Subject -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_SUBJECTID_NAME,
            strlen(OIC_JSON_SUBJECTID_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Subject Tag.");
        inLen = (memcmp(&(cred->subject), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0) ?
            WILDCARD_SUBJECT_ID_LEN : sizeof(OicUuid_t);
        if(inLen == WILDCARD_SUBJECT_ID_LEN)
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, WILDCARD_RESOURCE_URI,
                strlen(WILDCARD_RESOURCE_URI));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding Subject Id wildcard Value.");
        }
        else
        {
            char *subject = NULL;
            ret = ConvertUuidToStr(&cred->subject, &subject);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&credMap, subject, strlen(subject));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding Subject Id Value.");
            OICFree(subject);
        }

        //Role ID -- optional
        if (IsNonEmptyRole(&cred->roleId))
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_ROLEID_NAME,
                strlen(OIC_JSON_ROLEID_NAME));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding role ID map tag");

            bool includeAuthority = (0 != memcmp(&cred->roleId.authority, &EMPTY_ROLE.authority, sizeof(EMPTY_ROLE.authority)));

            cborEncoderResult = cbor_encoder_create_map(&credMap, &roleIdMap, ROLEID_MAP_SIZE + (includeAuthority ? 1 : 0));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding role ID map");

            cborEncoderResult = cbor_encode_text_string(&roleIdMap, OIC_JSON_ROLE_NAME, strlen(OIC_JSON_ROLE_NAME));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding role tag");

            cborEncoderResult = cbor_encode_text_string(&roleIdMap, cred->roleId.id, strlen(cred->roleId.id));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding role value");

            if (includeAuthority)
            {
                cborEncoderResult = cbor_encode_text_string(&roleIdMap, OIC_JSON_AUTHORITY_NAME, strlen(OIC_JSON_AUTHORITY_NAME));
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding authority tag");

                cborEncoderResult = cbor_encode_text_string(&roleIdMap, cred->roleId.authority, strlen(cred->roleId.authority));
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding authority value");
            }

            cborEncoderResult = cbor_encoder_close_container(&credMap, &roleIdMap);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed closing role ID map");
        }

        //CredType -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDTYPE_NAME,
            strlen(OIC_JSON_CREDTYPE_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Type Tag.");
        cborEncoderResult = cbor_encode_int(&credMap, cred->credType);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Cred Type Value.");

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        //PublicData -- Not Mandatory
        if ((SIGNED_ASYMMETRIC_KEY == cred->credType || ASYMMETRIC_KEY == cred->credType)
            && cred->publicData.data)
        {
            cborEncoderResult = SerializeEncodingToCbor(&credMap,
                                         OIC_JSON_PUBLICDATA_NAME, &cred->publicData);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding PublicData Tag.");
        }
        //OptionalData -- Not Mandatory
        if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->optionalData.data)
        {
            cborEncoderResult = SerializeSecOptToCbor(&credMap,
                                         OIC_JSON_OPTDATA_NAME, &cred->optionalData);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding OptionalData Tag.");
        }
        //CredUsage -- Not Mandatory
        if(cred->credUsage)
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDUSAGE_NAME,
                strlen(OIC_JSON_CREDUSAGE_NAME));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Credusage Name Tag.");
            cborEncoderResult = cbor_encode_text_string(&credMap, cred->credUsage,
                strlen(cred->credUsage));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Credusage Name Value.");
        }
#endif /* __WITH_DTLS__ ||  __WITH_TLS__*/
        //PrivateData -- Not Mandatory
        if(!secureFlag && cred->privateData.data)
        {
            cborEncoderResult = SerializeEncodingToCbor(&credMap,
                                         OIC_JSON_PRIVATEDATA_NAME, &cred->privateData);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");
        }

        //Period -- Not Mandatory
        if(cred->period)
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_PERIOD_NAME,
                strlen(OIC_JSON_PERIOD_NAME));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Period Name Tag.");
            cborEncoderResult = cbor_encode_text_string(&credMap, cred->period,
                strlen(cred->period));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding Period Name Value.");
        }

#ifdef MULTIPLE_OWNER
        // Eownerid -- Not Mandatory
        if(cred->eownerID)
        {
            char *eowner = NULL;
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_EOWNERID_NAME,
                strlen(OIC_JSON_EOWNERID_NAME));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding eownerId Name Tag.");
            ret = ConvertUuidToStr(cred->eownerID, &eowner);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&credMap, eowner, strlen(eowner));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding eownerId Value.");
            OICFree(eowner);
        }
#endif //MULTIPLE_OWNER

        cborEncoderResult = cbor_encoder_close_container(&credArray, &credMap);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing Cred Map.");

        cred = cred->next;
    }
    cborEncoderResult = cbor_encoder_close_container(&credRootMap, &credArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing Cred Array.");

    cred = credS;

    // Rownerid
    {
        char *rowner = NULL;
        cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_ROWNERID_NAME,
            strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding rownerid Name.");
        ret = ConvertUuidToStr(&gRownerId, &rowner);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&credRootMap, rowner, strlen(rowner));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding rownerid Value.");
        OICFree(rowner);
    }

    {
        //RT -- Mandatory
        CborEncoder rtArray;
        cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_RT_NAME,
                strlen(OIC_JSON_RT_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
        cborEncoderResult = cbor_encoder_create_array(&credRootMap, &rtArray, 1);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Value.");
        for (size_t i = 0; i < 1; i++)
        {
            cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_CRED,
                    strlen(OIC_RSRC_TYPE_SEC_CRED));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding RT Value.");
        }
        cborEncoderResult = cbor_encoder_close_container(&credRootMap, &rtArray);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing RT.");

        //IF-- Mandatory
        CborEncoder ifArray;
        cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_IF_NAME,
                 strlen(OIC_JSON_IF_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
        cborEncoderResult = cbor_encoder_create_array(&credRootMap, &ifArray, 1);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Value.");
        for (size_t i = 0; i < 1; i++)
        {
            cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
                    strlen(OC_RSRVD_INTERFACE_DEFAULT));
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding IF Value.");
        }
        cborEncoderResult = cbor_encoder_close_container(&credRootMap, &ifArray);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing IF.");
    }

    // Close CRED Root Map
    cborEncoderResult = cbor_encoder_close_container(&encoder, &credRootMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing CRED Root Map.");

    if (CborNoError == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "CredToCBORPayload Successed");
        *cborPayload = outPayload;
        *cborSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
        ret = OC_STACK_OK;
    }
    OIC_LOG(DEBUG, TAG, "CredToCBORPayload OUT");
exit:
    if (CborErrorOutOfMemory == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "CredToCBORPayload:CborErrorOutOfMemory : retry with more memory");
        // reallocate and try again!
        OICFree(outPayload);
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        cborEncoderResult = CborNoError;
        ret = CredToCBORPayload(credS, cborPayload, &cborLen, secureFlag);
        *cborSize = cborLen;
    }

    if (CborNoError != cborEncoderResult)
    {
        OIC_LOG(ERROR, TAG, "Failed to CredToCBORPayload");
        OICFree(outPayload);
        outPayload = NULL;
        *cborSize = 0;
        *cborPayload = NULL;
        ret = OC_STACK_ERROR;
    }

    OIC_LOG_V(DEBUG, TAG, "OUT %s:", __func__);

    return ret;
}

OCStackResult CBORPayloadToCred(const uint8_t *cborPayload, size_t size,
                                OicSecCred_t **secCred, OicUuid_t **rownerid)
{
    if (NULL == cborPayload || NULL == secCred || NULL != *secCred || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    char* tagName = NULL;
    char* roleIdTagName = NULL;
    char* name = NULL;
    OCStackResult ret = OC_STACK_ERROR;
    CborValue credCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;
    cbor_parser_init(cborPayload, size, 0, &parser, &credCbor);

    if (!cbor_value_is_container(&credCbor))
    {
        return OC_STACK_ERROR;
    }

    OicSecCred_t *headCred = NULL;

    // Enter CRED Root Map
    CborValue CredRootMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    cborFindResult = cbor_value_enter_container(&credCbor, &CredRootMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Entering CRED Root Map.");

    headCred = (OicSecCred_t *) OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NOT_NULL(TAG, headCred, ERROR);

    while (cbor_value_is_valid(&CredRootMap))
    {
        size_t len = 0;
        CborType type = cbor_value_get_type(&CredRootMap);
        if (type == CborTextStringType && cbor_value_is_text_string(&CredRootMap))
        {
            cborFindResult = cbor_value_dup_text_string(&CredRootMap, &tagName, &len, NULL);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Name in CRED Root Map.");
            cborFindResult = cbor_value_advance(&CredRootMap);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Value in CRED Root Map.");
        }
        if(tagName)
        {
            if (strcmp(tagName, OIC_JSON_CREDS_NAME)  == 0)
            {
                // Enter CREDS Array
                size_t tempLen = 0;
                int credCount = 0;
                CborValue credArray = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                cborFindResult = cbor_value_enter_container(&CredRootMap, &credArray);
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Cred Array.");

                while (cbor_value_is_valid(&credArray))
                {
                    credCount++;
                    //CredId -- Mandatory
                    CborValue credMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                    cborFindResult = cbor_value_enter_container(&credArray, &credMap);
                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Cred Map.");
                    OicSecCred_t *cred = NULL;

                    if(1 == credCount)
                    {
                        cred = headCred;
                    }
                    else
                    {
                        cred = (OicSecCred_t *) OICCalloc(1, sizeof(OicSecCred_t));
                        VERIFY_NOT_NULL(TAG, cred, ERROR);
                        OicSecCred_t *temp = headCred;
                        while (temp->next)
                        {
                            temp = temp->next;
                        }
                        temp->next = cred;
                    }

                    while(cbor_value_is_valid(&credMap) && cbor_value_is_text_string(&credMap))
                    {
                        CborType cmType = cbor_value_get_type(&credMap);
                        if (cmType == CborTextStringType)
                        {
                            cborFindResult = cbor_value_dup_text_string(&credMap, &name, &tempLen, NULL);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Name in CRED Map.");
                            cborFindResult = cbor_value_advance(&credMap);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Value in CRED Map.");
                        }
                        if(name)
                        {
                            //credid
                            if (strcmp(name, OIC_JSON_CREDID_NAME)  == 0)
                            {
                                uint64_t credId = 0;
                                cborFindResult = cbor_value_get_uint64(&credMap, &credId);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding CredId.");
                                cred->credId = (uint16_t)credId;
                            }
                            // subjectid
                            if (strcmp(name, OIC_JSON_SUBJECTID_NAME)  == 0)
                            {
                                char *subjectid = NULL;
                                cborFindResult = cbor_value_dup_text_string(&credMap, &subjectid, &tempLen, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding subjectid Value.");
                                if(strcmp(subjectid, WILDCARD_RESOURCE_URI) == 0)
                                {
                                    cred->subject.id[0] = '*';
                                }
                                else
                                {
                                    ret = ConvertStrToUuid(subjectid, &cred->subject);
                                    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                                }
                                //Because cbor using malloc directly
                                //It is required to use free() instead of OICFree
                                free(subjectid);
                            }
                            // roleid
                            if (strcmp(name, OIC_JSON_ROLEID_NAME) == 0)
                            {
                                /* Role subject */
                                size_t unusedLen = 0;
                                CborValue roleIdMap;
                                memset(&roleIdMap, 0, sizeof(roleIdMap));

                                cborFindResult = cbor_value_enter_container(&credMap, &roleIdMap);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed entering role ID map");

                                while (cbor_value_is_valid(&roleIdMap) && cbor_value_is_text_string(&roleIdMap))
                                {
                                    cborFindResult = cbor_value_dup_text_string(&roleIdMap, &roleIdTagName, &unusedLen, NULL);
                                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed getting role ID map tag name");
                                    cborFindResult = cbor_value_advance(&roleIdMap);
                                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing role ID map");

                                    if (NULL != roleIdTagName)
                                    {
                                        if (strcmp(roleIdTagName, OIC_JSON_ROLE_NAME) == 0)
                                        {
                                            char *roleId = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&roleIdMap, &roleId, &unusedLen, NULL);
                                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed getting role id value");
                                            if (strlen(roleId) >= sizeof(cred->roleId.id))
                                            {
                                                cborFindResult = CborUnknownError;
                                                free(roleId);
                                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Role ID is too long");
                                            }
                                            OICStrcpy(cred->roleId.id, sizeof(cred->roleId.id), roleId);
                                            free(roleId);
                                        }
                                        else if (strcmp(roleIdTagName, OIC_JSON_AUTHORITY_NAME) == 0)
                                        {
                                            char *authorityName = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&roleIdMap, &authorityName, &unusedLen, NULL);
                                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed getting role authority value");
                                            if (strlen(authorityName) >= sizeof(cred->roleId.authority))
                                            {
                                                cborFindResult = CborUnknownError;
                                                free(authorityName);
                                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Authority name is too long");
                                            }
                                            OICStrcpy(cred->roleId.authority, sizeof(cred->roleId.authority), authorityName);
                                            free(authorityName);
                                        }
                                        else
                                        {
                                            OIC_LOG_V(WARNING, TAG, "Unknown tag name in role ID map: %s", roleIdTagName);
                                        }

                                        free(roleIdTagName);
                                        roleIdTagName = NULL;
                                    }

                                    if (cbor_value_is_valid(&roleIdMap))
                                    {
                                        cborFindResult = cbor_value_advance(&roleIdMap);
                                        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing role map");
                                    }
                                }

                                /* Make sure at least the id is present. */
                                if ('\0' == cred->roleId.id[0])
                                {
                                    cborFindResult = CborUnknownError;
                                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "ID for role was not present in role map");
                                }
                            }
                            // credtype
                            if (strcmp(name, OIC_JSON_CREDTYPE_NAME)  == 0)
                            {
#ifdef __TIZENRT__
                                cborFindResult = cbor_value_get_int(&credMap, (int *) &cred->credType);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding CredType.");
#else
                                uint64_t credType = 0;
                                cborFindResult = cbor_value_get_uint64(&credMap, &credType);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding CredType.");
                                cred->credType = (OicSecCredType_t)credType;
#endif
                            }
                            // privatedata
                            if (strcmp(name, OIC_JSON_PRIVATEDATA_NAME)  == 0)
                            {
                                cborFindResult = DeserializeEncodingFromCbor(&credMap, &cred->privateData);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read privateData structure");
                            }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                            //PublicData -- Not Mandatory
                            if (strcmp(name, OIC_JSON_PUBLICDATA_NAME)  == 0)
                            {
                                cborFindResult = DeserializeEncodingFromCbor(&credMap, &cred->publicData);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read publicData structure");
                            }
                            //OptionalData -- Not Mandatory
                            if (strcmp(name, OIC_JSON_OPTDATA_NAME)  == 0)
                            {
                                cborFindResult = DeserializeSecOptFromCbor(&credMap, &cred->optionalData);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read optionalData structure");
                            }
                            //Credusage -- Not Mandatory
                            if (0 == strcmp(OIC_JSON_CREDUSAGE_NAME, name))
                            {
                                cborFindResult = cbor_value_dup_text_string(&credMap, &cred->credUsage, &tempLen, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Period.");
                            }
#endif  //__WITH_DTLS__ ||  __WITH_TLS__

                            if (0 == strcmp(OIC_JSON_PERIOD_NAME, name))
                            {
                                cborFindResult = cbor_value_dup_text_string(&credMap, &cred->period, &tempLen, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Period.");
                            }

#ifdef MULTIPLE_OWNER
                            // Eowner uuid -- Not Mandatory
                            if (strcmp(OIC_JSON_EOWNERID_NAME, name)  == 0 && cbor_value_is_text_string(&credMap))
                            {
                                char *eowner = NULL;
                                cborFindResult = cbor_value_dup_text_string(&credMap, &eowner, &tempLen, NULL);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding eownerId Value.");
                                if(NULL == cred->eownerID)
                                {
                                    cred->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
                                    VERIFY_NOT_NULL(TAG, cred->eownerID, ERROR);
                                }
                                ret = ConvertStrToUuid(eowner, cred->eownerID);
                                //Because cbor using malloc directly
                                //It is required to use free() instead of OICFree
                                free(eowner);
                                VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
                            }
#endif //MULTIPLE_OWNER

                            if (cbor_value_is_valid(&credMap))
                            {
                                cborFindResult = cbor_value_advance(&credMap);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing CRED Map.");
                            }
                            //Because cbor using malloc directly
                            //It is required to use free() instead of OICFree
                            free(name);
                            name = NULL;
                        }
                    }
                    cred->next = NULL;
                    if (cbor_value_is_valid(&credArray))
                    {
                        cborFindResult = cbor_value_advance(&credArray);
                        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing CRED Array.");
                    }
                }
            }

            //ROwner -- Mandatory
            if (strcmp(tagName, OIC_JSON_ROWNERID_NAME)  == 0 && cbor_value_is_text_string(&CredRootMap))
            {
                char *stRowner = NULL;
                cborFindResult = cbor_value_dup_text_string(&CredRootMap, &stRowner, &len, NULL);
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding Rownerid Value.");
                *rownerid = (OicUuid_t *) OICCalloc(1, sizeof(OicUuid_t));
                VERIFY_NOT_NULL(TAG, *rownerid, ERROR);
                ret = ConvertStrToUuid(stRowner, *rownerid);
                //Because cbor using malloc directly
                //It is required to use free() instead of OICFree
                free(stRowner);
                VERIFY_SUCCESS(TAG, (ret == OC_STACK_OK), ERROR);
            }
            //Because cbor using malloc directly
            //It is required to use free() instead of OICFree
            free(tagName);
            tagName = NULL;
        }
        if (cbor_value_is_valid(&CredRootMap))
        {
            cborFindResult = cbor_value_advance(&CredRootMap);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing CRED Root Map.");
        }
    }

    *secCred = headCred;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        DeleteCredList(headCred);
        headCred = NULL;
        *secCred = NULL;
        ret = OC_STACK_ERROR;
    }

    free(tagName);
    free(roleIdTagName);
    free(name);

    return ret;
}

#ifdef MULTIPLE_OWNER
bool IsValidCredentialAccessForSubOwner(const OicUuid_t* uuid, const uint8_t *cborPayload, size_t size)
{
    OicSecCred_t* cred = NULL;
    bool isValidCred = false;
    OicUuid_t *rownerId = NULL;

    OIC_LOG_BUFFER(DEBUG, TAG, cborPayload, size);

    VERIFY_NOT_NULL(TAG, uuid, ERROR);
    VERIFY_NOT_NULL(TAG, cborPayload, ERROR);
    VERIFY_SUCCESS(TAG, 0 != size, ERROR);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == CBORPayloadToCred(cborPayload, size, &cred, &rownerId), ERROR);
    VERIFY_NOT_NULL(TAG, cred, ERROR);
    VERIFY_NOT_NULL(TAG, cred->eownerID, ERROR);
    VERIFY_NOT_NULL(TAG, rownerId, ERROR);
    VERIFY_SUCCESS(TAG, (memcmp(cred->eownerID->id, uuid->id, sizeof(uuid->id)) == 0), ERROR);

    isValidCred = true;

exit:
    OICFree(rownerId);
    DeleteCredList(cred);

    return isValidCred;

}
#endif //MULTIPLE_OWNER

OicSecCred_t * GenerateCredential(const OicUuid_t * subject, OicSecCredType_t credType,
                                  const OicSecKey_t * publicData, const OicSecKey_t* privateData,
                                  const OicUuid_t * rownerID, const OicUuid_t * eownerID)
{
    OIC_LOG(DEBUG, TAG, "IN GenerateCredential");

    (void)publicData;
    OCStackResult ret = OC_STACK_ERROR;

    OicSecCred_t *cred = (OicSecCred_t *)OICCalloc(1, sizeof(*cred));
    VERIFY_NOT_NULL(TAG, cred, ERROR);
    cred->next = NULL;

    //CredId is assigned before appending new cred to the existing
    //credential list and updating svr database in AddCredential().
    cred->credId = 0;

    VERIFY_NOT_NULL(TAG, subject, ERROR);
    memcpy(cred->subject.id, subject->id , sizeof(cred->subject.id));

    VERIFY_SUCCESS(TAG, credType < (NO_SECURITY_MODE | SYMMETRIC_PAIR_WISE_KEY |
            SYMMETRIC_GROUP_KEY | ASYMMETRIC_KEY | SIGNED_ASYMMETRIC_KEY | PIN_PASSWORD), ERROR);
    cred->credType = credType;

#ifdef __WITH_DTLS__
    if (publicData && publicData->data)
    {
        cred->publicData.data = (uint8_t *)OICCalloc(1, publicData->len);
        VERIFY_NOT_NULL(TAG, cred->publicData.data, ERROR);
        memcpy(cred->publicData.data, publicData->data, publicData->len);
        cred->publicData.len = publicData->len;
        cred->publicData.encoding = publicData->encoding;
    }
#endif // __WITH_DTLS__

    if (privateData && privateData->data)
    {
        cred->privateData.data = (uint8_t *)OICCalloc(1, privateData->len);
        VERIFY_NOT_NULL(TAG, cred->privateData.data, ERROR);
        memcpy(cred->privateData.data, privateData->data, privateData->len);
        cred->privateData.len = privateData->len;
        cred->privateData.encoding = privateData->encoding;
    }

    VERIFY_NOT_NULL(TAG, rownerID, ERROR);
    memcpy(&cred->rownerID, rownerID, sizeof(OicUuid_t));

#ifdef MULTIPLE_OWNER
    if(eownerID)
    {
        cred->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
        VERIFY_NOT_NULL(TAG, cred->eownerID, ERROR);
        memcpy(cred->eownerID->id, eownerID->id, sizeof(eownerID->id));
    }
#else
    (void)(eownerID);
#endif //MULTIPLE_OWNER_

    ret = OC_STACK_OK;

    OIC_LOG_V(DEBUG, TAG, "GenerateCredential : result: %d", ret);
    OIC_LOG_V(DEBUG, TAG, "GenerateCredential : credId: %d", cred->credId);
    OIC_LOG_V(DEBUG, TAG, "GenerateCredential : credType: %d", cred->credType);
    OIC_LOG_BUFFER(DEBUG, TAG, cred->subject.id, sizeof(cred->subject.id));
    if (cred->privateData.data)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : privateData len: %"PRIuPTR, cred->privateData.len);
        OIC_LOG_BUFFER(DEBUG, TAG, cred->privateData.data, cred->privateData.len);
    }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    if(cred->credUsage)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : credUsage: %s", cred->credUsage);
    }
    if (cred->publicData.data)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : publicData len: %" PRIuPTR, cred->publicData.len);
        OIC_LOG_BUFFER(DEBUG, TAG, cred->publicData.data, cred->publicData.len);

    }
    if (cred->optionalData.data)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : optionalData len: %" PRIuPTR, cred->optionalData.len);
        OIC_LOG_BUFFER(DEBUG, TAG, cred->optionalData.data, cred->optionalData.len);
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : optionalData revstat: %d", cred->optionalData.revstat);
    }
#endif //defined(__WITH_DTLS__) || defined(__WITH_TLS__)

exit:
    if (OC_STACK_OK != ret)
    {
        DeleteCredList(cred);
        cred = NULL;
    }
    OIC_LOG(DEBUG, TAG, "OUT GenerateCredential");
    return cred;
}

#ifdef HAVE_WINDOWS_H
/* Helper for UpdatePersistentStorage. */
static OCStackResult CopyPayload(uint8_t **payload, size_t *payloadSize, const DATA_BLOB *source)
{
    OCStackResult res = OC_STACK_OK;

    if (source->cbData > *payloadSize)
    {
        /* Need more memory to copy encrypted payload out. */
        OICClearMemory(*payload, *payloadSize);
        OICFree(*payload);
        *payload = OICMalloc(source->cbData);

        if (NULL == *payload)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to OICMalloc for encrypted payload: %u", GetLastError());
            res = OC_STACK_NO_MEMORY;
        }
    }
    else if (source->cbData < *payloadSize)
    {
        /* Zero portion of payload we won't overwrite with the encrypted version. The
         * later call to OICClearMemory won't cover this part of the buffer.
         */
        OICClearMemory(*payload + source->cbData, *payloadSize - source->cbData);
    }

    if (OC_STACK_OK == res)
    {
        memcpy(*payload, source->pbData, source->cbData);
        *payloadSize = source->cbData;
    }

    return res;
}
#endif

static bool UpdatePersistentStorage(const OicSecCred_t *cred)
{
    bool ret = false;
    OIC_LOG(DEBUG, TAG, "IN Cred UpdatePersistentStorage");

    // Convert Cred data into JSON for update to persistent storage
    if (cred)
    {
        uint8_t *payload = NULL;
        // This added '512' is arbitrary value that is added to cover the name of the resource, map addition and ending
        size_t size = GetCredKeyDataSize(cred);
        size += (512 * OicSecCredCount(cred));
        OIC_LOG_V(DEBUG, TAG, "cred size: %zu", size);

        int secureFlag = 0;
        OCStackResult res = CredToCBORPayload(cred, &payload, &size, secureFlag);
#ifdef HAVE_WINDOWS_H
        /* On Windows, keep the credential resource encrypted on disk to protect symmetric and private keys. Only the
         * current user on this system will be able to decrypt it later, to help prevent credential theft.
         */
        DWORD dwordSize;

        if (FAILED(SizeTToDWord(size, &dwordSize)))
        {
            OIC_LOG(DEBUG, TAG, "Cred size too large.");
            res = OC_STACK_ERROR;
            ret = false;
        }

        if ((OC_STACK_OK == res) && payload)
        {
            DATA_BLOB decryptedPayload;
            DATA_BLOB encryptedPayload;
            memset(&decryptedPayload, 0, sizeof(decryptedPayload));
            memset(&encryptedPayload, 0, sizeof(encryptedPayload));
            decryptedPayload.cbData = dwordSize;
            decryptedPayload.pbData = payload;

            if (CryptProtectData(
                &decryptedPayload,
                NULL,
                NULL,
                NULL,
                NULL,
                CRYPTPROTECT_UI_FORBIDDEN,
                &encryptedPayload))
            {
                res = CopyPayload(&payload, &size, &encryptedPayload);
                ret = (OC_STACK_OK == res);

                /* For the returned data from CryptProtectData, LocalFree must be used to free. Don't use OICFree. */
                if (NULL != LocalFree(encryptedPayload.pbData))
                {
                    OIC_LOG_V(ERROR, TAG, "LocalFree failed on output from CryptProtectData; memory may be corrupted or leaked. Last error: %u.", GetLastError());
                    assert(!"LocalFree failed");
                }
            }
            else
            {
                OIC_LOG_V(ERROR, TAG, "Failed to CryptProtectData cred resource: %u", GetLastError());
                res = OC_STACK_ERROR;
                ret = false;
            }
        }
#endif

        if ((OC_STACK_OK == res) && payload)
        {
            if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_CRED_NAME, payload, size))
            {
                ret = true;
            }
        }

        OICClearMemory(payload, size);
        OICFree(payload);
    }
    else //Empty cred list
    {
        if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_CRED_NAME, NULL, 0))
        {
            ret = true;
        }
    }

    OIC_LOG(DEBUG, TAG, "OUT Cred UpdatePersistentStorage");

    logCredMetadata();

    return ret;
}

/**
 * Compare function used LL_SORT for sorting credentials.
 *
 * @param first pointer to OicSecCred_t struct.
 * @param second  pointer to OicSecCred_t struct.
 *
 *@return -1, if credId of first is less than credId of second.
 * 0, if credId of first is equal to credId of second.
 * 1, if credId of first is greater than credId of second.
 */
static int CmpCredId(const OicSecCred_t * first, const OicSecCred_t *second)
{
    if (first->credId < second->credId)
    {
        return -1;
    }
    else if (first->credId > second->credId)
    {
        return 1;
    }
    else
        return 0;
}

/**
 * GetCredId goes through the cred list and returns the next
 * available credId. The next credId could be the credId that is
 * available due deletion of OicSecCred_t object or one more than
 * credId of last credential in the list.
 *
 * @return next available credId if successful, else 0 for error.
 */
static uint16_t GetCredId()
{
    //Sorts credential list in incremental order of credId
    /** @todo: Remove pragma for VS2013 warning; Investigate fixing LL_SORT macro */
#ifdef _MSC_VER
#pragma warning(suppress:4133)
    LL_SORT(gCred, CmpCredId);
#else
    LL_SORT(gCred, CmpCredId);
#endif

    OicSecCred_t *currentCred = NULL, *credTmp = NULL;
    uint16_t nextCredId = 1;

    LL_FOREACH_SAFE(gCred, currentCred, credTmp)
    {
        if (currentCred->credId == nextCredId)
        {
            nextCredId += 1;
        }
        else
        {
            break;
        }
    }

    VERIFY_SUCCESS(TAG, nextCredId < UINT16_MAX, ERROR);
    return nextCredId;

exit:
    return 0;
}

/**
 * Get the default value.
 *
 * @return  NULL for now.
 */
static OicSecCred_t* GetCredDefault()
{
    // TODO:Update it when we finalize the default info.
    return NULL;
}

bool IsSameSecOpt(const OicSecOpt_t* sk1, const OicSecOpt_t* sk2)
{
    VERIFY_NOT_NULL(TAG, sk1, WARNING);
    VERIFY_NOT_NULL(TAG, sk2, WARNING);

    VERIFY_SUCCESS(TAG, (sk1->len == sk2->len), INFO);
    VERIFY_SUCCESS(TAG, (sk1->encoding == sk2->encoding), INFO);
    VERIFY_SUCCESS(TAG, (0 == memcmp(sk1->data, sk2->data, sk1->len)), INFO);
    return true;
exit:
    return false;
}

bool IsSameSecKey(const OicSecKey_t* sk1, const OicSecKey_t* sk2)
{
    VERIFY_NOT_NULL(TAG, sk1, WARNING);
    VERIFY_NOT_NULL(TAG, sk2, WARNING);

    VERIFY_SUCCESS(TAG, (sk1->len == sk2->len), INFO);
    VERIFY_SUCCESS(TAG, (sk1->encoding == sk2->encoding), INFO);
    VERIFY_SUCCESS(TAG, (0 == memcmp(sk1->data, sk2->data, sk1->len)), INFO);
    return true;
exit:
    return false;
}

#if ((defined(__WITH_DTLS__) || defined(__WITH_TLS__)) && defined(MULTIPLE_OWNER))
static bool IsNewPreconfigPinCredential(OicSecCred_t * oldCred, OicSecCred_t * newCred)
{
    if (oldCred->credUsage &&
        newCred->credUsage &&
        (0 == strcmp(PRECONFIG_PIN_CRED, oldCred->credUsage)) &&
        (0 == strcmp(PRECONFIG_PIN_CRED, newCred->credUsage)))
    {
        return true;
    }

    return false;
}
#endif //(__WITH_DTLS__ or __WITH_TLS__) and MULTIPLE_OWNER

OCStackResult AddCredential(OicSecCred_t * newCred)
{
    OCStackResult result = OC_STACK_ERROR;

    OicSecCred_t* cred = NULL;
    OicSecCred_t* tempCred = NULL;

    bool found = false;

    OicSecDostype_t dos;

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDos(&dos), ERROR);
    if ((DOS_RESET == dos.state) ||
        (DOS_RFNOP == dos.state))
    {
        OIC_LOG_V(ERROR, TAG, "%s /cred resource is read-only in RESET and RFNOP.", __func__);
        result = OC_EH_NOT_ACCEPTABLE;
        goto exit;
    }

    //leave IOT-1936 fix for preconfig pin
#if ((defined(__WITH_DTLS__) || defined(__WITH_TLS__)) && defined(MULTIPLE_OWNER))
    LL_FOREACH_SAFE(gCred, cred, tempCred)
    {
        if (IsNewPreconfigPinCredential(cred, newCred))
        {
            //devices can only have one Preconfigured Pin credential at any given time. Check
            //if the new credential is an update to an existing Preconfigured Pin credential
            //if so, just update existing one.

            //save old credid so act like an update
            newCred->credId = cred->credId;

            LL_DELETE(gCred, cred);
            LL_PREPEND(gCred, newCred);

            FreeCred(cred);
            found = true;
            break;
        }
    }
#endif //(__WITH_DTLS__ or __WITH_TLS__) and MULTIPLE_OWNER

    if (found)
    {
        goto saveToDB;
    }

    if (newCred->credId != 0)
    {
        //check if cred with such cred id exists
        LL_FOREACH_SAFE(gCred, cred, tempCred)
        {
            if (cred->credId == newCred->credId)
            {
                //remove old cred with same cred id
                LL_DELETE(gCred, cred);
                FreeCred(cred);
                break;
            }
        }
    }
    else
    {
        newCred->credId = GetCredId();
    }

    OIC_LOG(DEBUG, TAG, "Adding New Cred");
    LL_APPEND(gCred, newCred);


    OicUuid_t emptyOwner = { .id = {0} };
    if (memcmp(&(newCred->rownerID), &emptyOwner, sizeof(OicUuid_t)) != 0)
    {
        memcpy(&(gRownerId), &(newCred->rownerID), sizeof(OicUuid_t));
    }

saveToDB:

    if (UpdatePersistentStorage(gCred))
    {
        result = OC_STACK_OK;
    }
exit:
    return result;
}

OCStackResult RemoveCredential(const OicUuid_t *subject)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicSecCred_t *cred = NULL;
    OicSecCred_t *tempCred = NULL;
    bool deleteFlag = false;

    LL_FOREACH_SAFE(gCred, cred, tempCred)
    {
        if (memcmp(cred->subject.id, subject->id, sizeof(subject->id)) == 0)
        {
            LL_DELETE(gCred, cred);
            FreeCred(cred);
            deleteFlag = 1;
        }
    }

    if (deleteFlag)
    {
        if (UpdatePersistentStorage(gCred))
        {
            ret = OC_STACK_RESOURCE_DELETED;
        }
    }
    return ret;
}

OCStackResult RemoveCredentialByCredId(uint16_t credId)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicSecCred_t *cred = NULL;
    OicSecCred_t *tempCred = NULL;
    bool deleteFlag = false;

    OIC_LOG(INFO, TAG, "IN RemoveCredentialByCredId");

    if ( 0 == credId)
    {
        return OC_STACK_INVALID_PARAM;
    }


    LL_FOREACH_SAFE(gCred, cred, tempCred)
    {
        if (cred->credId == credId)
        {
            OIC_LOG_V(DEBUG, TAG, "Credential(ID=%d) will be removed.", credId);

            LL_DELETE(gCred, cred);
            FreeCred(cred);
            deleteFlag = true;
        }
    }

    if (deleteFlag)
    {
        if (UpdatePersistentStorage(gCred))
        {
            ret = OC_STACK_RESOURCE_DELETED;
        }
    }
    OIC_LOG(INFO, TAG, "OUT RemoveCredentialByCredId");

    return ret;
}

/**
 * This method removes cred's corresponding to given list of credid's from the Cred
 *
 * @param credid's of the Cred
 *
 * @return
 *     ::OC_STACK_RESOURCE_DELETED on success
 *     ::OC_STACK_NO_RESOURCE on failure to find the appropriate Cred
 *     ::OC_STACK_INVALID_PARAM on invalid parameter
 */
static OCStackResult RemoveCredentialByCredIds(CredIdList_t *credIdList)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicSecCred_t *cred = NULL;
    OicSecCred_t *tempCred = NULL;
    CredIdList_t *credIdElem = NULL;
    bool deleteFlag = false;

    OIC_LOG(INFO, TAG, "IN RemoveCredentialByCredIds");

    LL_FOREACH(credIdList, credIdElem)
    {
        LL_FOREACH_SAFE(gCred, cred, tempCred)
        {
            if (cred->credId == credIdElem->credId)
            {
                OIC_LOG_V(DEBUG, TAG, "Credential(ID=%d) will be removed.", cred->credId);

                LL_DELETE(gCred, cred);
                FreeCred(cred);
                deleteFlag = true;
                //TODO: add break when cred's will have unique credid (during IOT-2464 fix)
            }
        }
    }

    if (deleteFlag)
    {
        if (UpdatePersistentStorage(gCred))
        {
            ret = OC_STACK_RESOURCE_DELETED;
        }
    }
    OIC_LOG(INFO, TAG, "OUT RemoveCredentialByCredIds");

    return ret;
}

/**
 * This method parses the query string received for REST requests and
 * retrieves the 'credid' field.
 *
 * @param query querystring passed in REST request
 * @param credid cred id parsed from query string
 *
 * @return true if query parsed successfully and found 'credid', else false.
 */
static bool GetCredIdsFromQueryString(const char *query, CredIdList_t **credid)
{
    bool found = false;
    OicParseQueryIter_t parseIter = { .attrPos = NULL };

    ParseQueryIterInit((unsigned char *) query, &parseIter);

    while (GetNextQuery (&parseIter))
    {
        if (strncasecmp((char *) parseIter.attrPos, OIC_JSON_CREDID_NAME, parseIter.attrLen) == 0)
        {
            VERIFY_SUCCESS(TAG, 0 != parseIter.valLen, ERROR);

            //parse credid array value in format "1,2,3"
            unsigned char *str = parseIter.valPos;

            //remember last symbol (may be '\0' or '&')
            unsigned char tmp = str[parseIter.valLen];
            //set last symbol to '\0' to use strtok_r
            str[parseIter.valLen] = '\0';

            char *saveptr = NULL;

            for (; ; str = NULL)
            {
                char *token = strtok_r((char *)str, ",", &saveptr);
                if (NULL == token)
                {
                    break;
                }

                CredIdList_t *newCredIdElem = (CredIdList_t *) OICCalloc(1, sizeof(CredIdList_t));
                if (NULL == newCredIdElem)
                {
                    OIC_LOG(ERROR, TAG, "Failed to allocate CredIdList_t element");
                    break;
                }
                newCredIdElem->credId = (uint16_t)atoi(token);

                LL_PREPEND(*credid, newCredIdElem);
                found = true;
            }

            //restore last symbol
            parseIter.valPos[parseIter.valLen] = tmp;
        }
    }

exit:
    return found;
}

/**
 * Remove all credential data on credential resource and persistent storage
 *
 * @retval
 *     OC_STACK_OK              - no errors
 *     OC_STACK_ERROR           - stack process error
 */
static OCStackResult RemoveAllCredentials()
{
    DeleteCredList(gCred);
    gCred = GetCredDefault();

    if (!UpdatePersistentStorage(gCred))
    {
        return OC_STACK_ERROR;
    }
    return OC_STACK_OK;
}

/**
 * This method parses the query string received for REST requests and
 * retrieves the 'subjectuuid' field.
 *
 * @param query querystring passed in REST request
 * @param subject subject UUID parsed from query string
 *
 * @return true if query parsed successfully and found 'subject', else false.
 */
static bool GetSubjectFromQueryString(const char *query, OicUuid_t *subject)
{
    OicParseQueryIter_t parseIter = { .attrPos = NULL };

    ParseQueryIterInit((unsigned char *) query, &parseIter);

    while (GetNextQuery (&parseIter))
    {
        if (strncasecmp((char *) parseIter.attrPos, OIC_JSON_SUBJECTID_NAME, parseIter.attrLen) == 0)
        {
            VERIFY_SUCCESS(TAG, 0 != parseIter.valLen, ERROR);
            //temporary set '\0' symbol instead of copy string
            char tmp = parseIter.valPos[parseIter.valLen];
            parseIter.valPos[parseIter.valLen] = '\0';

            OCStackResult res = ConvertStrToUuid((const char*)parseIter.valPos, subject);
            parseIter.valPos[parseIter.valLen] = tmp;
            VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);
            return true;
        }
    }

exit:
    return false;
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * Internal function to fill private data of owner PSK.
 *
 * @param receviedCred recevied owner credential from OBT(PT)
 * @param ownerAdd address of OBT(PT)
 * @param doxm current device's doxm resource
 *
 * @return
 *     true successfully done and valid ower psk information
 *     false Invalid owner psk information or failed to owner psk generation
 */
static bool FillPrivateDataOfOwnerPSK(OicSecCred_t* receviedCred, const CAEndpoint_t* ownerAddr,
                           const OicSecDoxm_t* doxm)
{
    //Derive OwnerPSK locally
    const char* oxmLabel = GetOxmString(doxm->oxmSel);
    char* b64Buf = NULL;
    size_t b64BufSize = 0;
    VERIFY_NOT_NULL(TAG, oxmLabel, ERROR);

    uint8_t ownerPSK[OWNER_PSK_LENGTH_128] = {0};
    CAResult_t pskRet = CAGenerateOwnerPSK(ownerAddr,
        (uint8_t*)oxmLabel, strlen(oxmLabel),
        doxm->owner.id, sizeof(doxm->owner.id),
        doxm->deviceID.id, sizeof(doxm->deviceID.id),
        ownerPSK, OWNER_PSK_LENGTH_128);
    VERIFY_SUCCESS(TAG, pskRet == CA_STATUS_OK, ERROR);

    OIC_LOG(DEBUG, TAG, "OwnerPSK dump :");
    OIC_LOG_BUFFER(DEBUG, TAG, ownerPSK, OWNER_PSK_LENGTH_128);

    //Generate owner credential based on recevied credential information

    // TODO: Added as workaround, will be replaced soon.
    if(OIC_ENCODING_RAW == receviedCred->privateData.encoding)
    {
#ifndef __TIZENRT__
        receviedCred->privateData.data = (uint8_t *)OICCalloc(1, OWNER_PSK_LENGTH_128);
#else
        receviedCred->privateData.data = (uint8_t *)OICRealloc(receviedCred->privateData.data, OWNER_PSK_LENGTH_128);
#endif
        VERIFY_NOT_NULL(TAG, receviedCred->privateData.data, ERROR);
        receviedCred->privateData.len = OWNER_PSK_LENGTH_128;
        memcpy(receviedCred->privateData.data, ownerPSK, OWNER_PSK_LENGTH_128);
    }
    else if(OIC_ENCODING_BASE64 == receviedCred->privateData.encoding)
    {
        B64Result b64res = B64_OK;
        size_t b64OutSize = 0;
        b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        b64Buf = OICCalloc(1, b64BufSize);
        VERIFY_NOT_NULL(TAG, b64Buf, ERROR);

        b64res = b64Encode(ownerPSK, OWNER_PSK_LENGTH_128, b64Buf, b64BufSize, &b64OutSize);
        VERIFY_SUCCESS(TAG, B64_OK == b64res, ERROR);
#ifndef __TIZENRT__
        receviedCred->privateData.data = (uint8_t *)OICCalloc(1, b64OutSize + 1);
#else
        receviedCred->privateData.data = (uint8_t *)OICRealloc(receviedCred->privateData.data, b64OutSize + 1);
#endif
        VERIFY_NOT_NULL(TAG, receviedCred->privateData.data, ERROR);
        receviedCred->privateData.len = b64OutSize;
        strncpy((char*)receviedCred->privateData.data, b64Buf, b64OutSize);
        receviedCred->privateData.data[b64OutSize] = '\0';
        OICClearMemory(b64Buf, b64BufSize);
        OICFree(b64Buf);
        b64Buf = NULL;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Unknown credential encoding type: %u.", receviedCred->privateData.encoding);
        goto exit;
    }

    OIC_LOG(INFO, TAG, "PrivateData of OwnerPSK was calculated successfully");

    OICClearMemory(ownerPSK, sizeof(ownerPSK));

    //Verify OwnerPSK information
    return (memcmp(&(receviedCred->subject), &(doxm->owner), sizeof(OicUuid_t)) == 0 &&
            receviedCred->credType == SYMMETRIC_PAIR_WISE_KEY);
exit:
    //receviedCred->privateData.data will be deallocated when deleting credential.
    OICClearMemory(ownerPSK, sizeof(ownerPSK));
    OICClearMemory(b64Buf, b64BufSize);
    OICFree(b64Buf);
    return false;
}


#ifdef MULTIPLE_OWNER
/**
 * Internal function to fill private data of SubOwner PSK.
 *
 * @param receviedCred recevied owner credential from SubOwner
 * @param ownerAdd address of SubOwner
 * @param doxm current device's doxm resource
 *
 * @return
 *     true successfully done and valid subower psk information
 *     false Invalid subowner psk information or failed to subowner psk generation
 */
static bool FillPrivateDataOfSubOwnerPSK(OicSecCred_t* receivedCred, const CAEndpoint_t* ownerAddr,
                           const OicSecDoxm_t* doxm, const OicUuid_t* subOwner)
{
    char* b64Buf = NULL;
    //Derive OwnerPSK locally
    const char* oxmLabel = GetOxmString(doxm->oxmSel);
    VERIFY_NOT_NULL(TAG, oxmLabel, ERROR);

    uint8_t subOwnerPSK[OWNER_PSK_LENGTH_128] = {0};
    CAResult_t pskRet = CAGenerateOwnerPSK(ownerAddr,
        (uint8_t*)oxmLabel, strlen(oxmLabel),
        subOwner->id, sizeof(subOwner->id),
        doxm->deviceID.id, sizeof(doxm->deviceID.id),
        subOwnerPSK, OWNER_PSK_LENGTH_128);
    VERIFY_SUCCESS(TAG, pskRet == CA_STATUS_OK, ERROR);

    OIC_LOG(DEBUG, TAG, "SubOwnerPSK dump :");
    OIC_LOG_BUFFER(DEBUG, TAG, subOwnerPSK, OWNER_PSK_LENGTH_128);

    //Generate owner credential based on received credential information

    if(OIC_ENCODING_RAW == receivedCred->privateData.encoding)
    {
        receivedCred->privateData.data = (uint8_t *)OICCalloc(1, OWNER_PSK_LENGTH_128);
        VERIFY_NOT_NULL(TAG, receivedCred->privateData.data, ERROR);
        receivedCred->privateData.len = OWNER_PSK_LENGTH_128;
        memcpy(receivedCred->privateData.data, subOwnerPSK, OWNER_PSK_LENGTH_128);
    }
    else if(OIC_ENCODING_BASE64 == receivedCred->privateData.encoding)
    {
        size_t b64OutSize = 0;
        size_t b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        b64Buf = OICCalloc(1, b64BufSize);
        VERIFY_NOT_NULL(TAG, b64Buf, ERROR);

        VERIFY_SUCCESS(TAG, \
                       B64_OK == b64Encode(subOwnerPSK, OWNER_PSK_LENGTH_128, b64Buf, b64BufSize, &b64OutSize), \
                       ERROR);

        receivedCred->privateData.data = (uint8_t *)OICCalloc(1, b64OutSize + 1);
        VERIFY_NOT_NULL(TAG, receivedCred->privateData.data, ERROR);
        receivedCred->privateData.len = b64OutSize;
        strncpy((char*)receivedCred->privateData.data, b64Buf, b64OutSize);
        receivedCred->privateData.data[b64OutSize] = '\0';
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "Unknown credential encoding type: %u.", receivedCred->privateData.encoding);
        goto exit;
    }

    OIC_LOG(INFO, TAG, "PrivateData of SubOwnerPSK was calculated successfully");
    OICFree(b64Buf);
    return true;
exit:
    //receivedCred->privateData.data will be deallocated when deleting credential.
    OICFree(b64Buf);
    return false;
}
#endif //MULTIPLE_OWNER
#endif // __WITH_DTLS__ or __WITH_TLS__


static OCEntityHandlerResult HandleNewCredential(OCEntityHandlerRequest *ehRequest, OicSecCred_t *cred, uint16_t previousMsgId)
{
    OCEntityHandlerResult ret = OC_EH_INTERNAL_SERVER_ERROR;

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    OicUuid_t emptyUuid = {.id={0}};
    const OicSecDoxm_t *doxm = GetDoxmResourceData();
    if( NO_SECURITY_MODE != cred->credType
        && doxm
        && false == doxm->owned
        && memcmp(&(doxm->owner), &emptyUuid, sizeof(OicUuid_t)) != 0
        && memcmp(&(doxm->owner), &cred->subject, sizeof(OicUuid_t)) == 0 )
    {
        //in case of owner PSK
        switch(cred->credType)
        {
            case SYMMETRIC_PAIR_WISE_KEY:
            {
                OCServerRequest *request = (OCServerRequest *)ehRequest->requestHandle;
#ifdef __TIZENRT__
                if (NULL == request)
                {
                    OIC_LOG(ERROR, TAG, "Failed to get a server request information.");
                    ret = OC_EH_ERROR;
                    break;
                }

                CAEndpoint_t *ep_addr = (CAEndpoint_t *)malloc(sizeof(CAEndpoint_t));
                if(!ep_addr)
                {
                    ret = OC_STACK_NO_MEMORY;
                    break;
                }
                ep_addr->adapter=   request->devAddr.adapter;
                ep_addr->flags=   request->devAddr.flags;
                ep_addr->port  =   request->devAddr.port;
                memcpy(ep_addr->addr,request->devAddr.addr,MAX_ADDR_STR_SIZE_CA);
                ep_addr->ifindex  =   request->devAddr.ifindex;
#if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
                memcpy(ep_addr->routeData,request->devAddr.routeData,MAX_ADDR_STR_SIZE_CA);
#endif
                if(FillPrivateDataOfOwnerPSK(cred, ep_addr, doxm))
#else
                if(FillPrivateDataOfOwnerPSK(cred, (CAEndpoint_t *)&request->devAddr, doxm))
#endif
                {
                    if(OC_STACK_RESOURCE_DELETED == RemoveCredential(&cred->subject))
                    {
                        OIC_LOG(WARNING, TAG, "The credential with the same subject ID was detected!");
                    }

                    OIC_LOG(ERROR, TAG, "OwnerPSK was generated successfully.");
                    if(OC_STACK_OK == AddCredential(cred))
                    {
                        ret = OC_EH_CHANGED;
                    }
                    else
                    {
                        OIC_LOG(ERROR, TAG, "Failed to save the OwnerPSK as cred resource");
                        ret = OC_EH_ERROR;
                    }
                }
                else
                {
                    OIC_LOG(ERROR, TAG, "Failed to verify receviced OwnerPKS.");
                    ret = OC_EH_ERROR;
                }
#ifdef __TIZENRT__
                free(ep_addr);
#endif
                if(OC_EH_CHANGED == ret)
                {
                    /**
                        * in case of random PIN based OxM,
                        * revert get_psk_info callback of tinyDTLS to use owner credential.
                        */
                    if(OIC_RANDOM_DEVICE_PIN == doxm->oxmSel)
                    {
                        SetUuidForPinBasedOxm(&emptyUuid);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                        if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskCredentials))
                        {
                            OIC_LOG(ERROR, TAG, "Failed to revert TLS credential handler.");
                            ret = OC_EH_ERROR;
                            break;
                        }
#endif // __WITH_DTLS__ or __WITH_TLS__
                    }

                    //Select cipher suite to use owner PSK
                    if(CA_STATUS_OK != CAEnableAnonECDHCipherSuite(false))
                    {
                        OIC_LOG(ERROR, TAG, "Failed to disable anonymous cipher suite");
                        ret = OC_EH_ERROR;
                    }
                    else
                    {
                        OIC_LOG(INFO, TAG, "Anonymous cipher suite is DISABLED");
                    }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                    if(CA_STATUS_OK != CASelectCipherSuite(
                                MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, CA_ADAPTER_IP))
                    {
                        OIC_LOG(ERROR, TAG, "Failed to enable PSK cipher suite");
                        ret = OC_EH_ERROR;
                    }
                    else
                    {
                        OIC_LOG(INFO, TAG, "PSK cipher suite is ENABLED");
                    }
#endif // __WITH_DTLS__ or __WITH_TLS__
                }

                break;
            }
            case SYMMETRIC_GROUP_KEY:
            case ASYMMETRIC_KEY:
            case SIGNED_ASYMMETRIC_KEY:
            case PIN_PASSWORD:
            case ASYMMETRIC_ENCRYPTION_KEY:
            {
                OIC_LOG(WARNING, TAG, "Unsupported credential type for owner credential.");
                ret = OC_EH_ERROR;
                break;
            }
            default:
            {
                OIC_LOG(WARNING, TAG, "Unknown credential type for owner credential.");
                ret = OC_EH_ERROR;
                break;
            }
        }

        if(OC_EH_CHANGED != ret)
        {
            /*
                * If some error is occured while ownership transfer,
                * ownership transfer related resource should be revert back to initial status.
                */
            const OicSecDoxm_t *ownershipDoxm =  GetDoxmResourceData();
            if(ownershipDoxm)
            {
                if(!ownershipDoxm->owned)
                {
                    OIC_LOG(WARNING, TAG, "The operation failed during handle DOXM request");

                    if((OC_ADAPTER_IP == ehRequest->devAddr.adapter && previousMsgId != ehRequest->messageID)
                        || OC_ADAPTER_TCP == ehRequest->devAddr.adapter)
                    {
                        RestoreDoxmToInitState();
                        RestorePstatToInitState();
                        OIC_LOG(WARNING, TAG, "DOXM will be reverted.");
                    }
                }
            }
            else
            {
                OIC_LOG(ERROR, TAG, "Invalid DOXM resource");
            }
        }
    }
#ifdef MULTIPLE_OWNER
    // In case SubOwner Credential
    else if(doxm && doxm->owned && doxm->mom &&
            OIC_MULTIPLE_OWNER_DISABLE != doxm->mom->mode &&
            0 == cred->privateData.len &&
            0 == cred->optionalData.len &&
            0 == cred->publicData.len )
    {
        switch(cred->credType)
        {
            case SYMMETRIC_PAIR_WISE_KEY:
            {
                OCServerRequest *request = (OCServerRequest *)ehRequest->requestHandle;
                if(FillPrivateDataOfSubOwnerPSK(cred, (CAEndpoint_t *)&request->devAddr, doxm, &cred->subject))
                {
                    if(OC_STACK_RESOURCE_DELETED == RemoveCredential(&cred->subject))
                    {
                        OIC_LOG(WARNING, TAG, "The credential with the same subject ID was detected!");
                    }

                    OIC_LOG(ERROR, TAG, "SubOwnerPSK was generated successfully.");
                    if(OC_STACK_OK == AddCredential(cred))
                    {
                        ret = OC_EH_CHANGED;
                    }
                    else
                    {
                        OIC_LOG(ERROR, TAG, "Failed to save the SubOwnerPSK as cred resource");
                        ret = OC_EH_ERROR;
                    }
                }
                else
                {
                    OIC_LOG(ERROR, TAG, "Failed to verify receviced SubOwner PSK.");
                    ret = OC_EH_ERROR;
                }
            }
            break;

            case SYMMETRIC_GROUP_KEY:
            case ASYMMETRIC_KEY:
            case SIGNED_ASYMMETRIC_KEY:
            case PIN_PASSWORD:
            case ASYMMETRIC_ENCRYPTION_KEY:
            {
                OIC_LOG(WARNING, TAG, "Unsupported credential type for SubOwner credential.");
                ret = OC_EH_ERROR;
                break;
            }
            default:
            {
                OIC_LOG(WARNING, TAG, "Unknown credential type for SubOwner credential.");
                ret = OC_EH_ERROR;
                break;
            }
        }
    }
#endif //MULTIPLE_OWNER
    else
    {
        if(IsEmptyCred(cred))
        {
                    if (UpdatePersistentStorage(gCred))
                    {
                        ret = OC_EH_CHANGED;
                    }
                    else
                    {
                        ret = OC_EH_ERROR;
                    }
        }
        else
        {
            /*
                * If the post request credential has credId, it will be
                * discarded and the next available credId will be assigned
                * to it before getting appended to the existing credential
                * list and updating svr database.
                */
            ret = (OC_STACK_OK == AddCredential(cred))? OC_EH_CHANGED : OC_EH_ERROR;
        }
    }
#else //not __WITH_DTLS__
    /*
        * If the post request credential has credId, it will be
        * discarded and the next available credId will be assigned
        * to it before getting appended to the existing credential
        * list and updating svr database.
        */
    ret = (OC_STACK_OK == AddCredential(cred))? OC_EH_CHANGED : OC_EH_ERROR;
    OC_UNUSED(previousMsgId);
    OC_UNUSED(ehRequest);
#endif//__WITH_DTLS__

    return ret;
}

static OCEntityHandlerResult HandlePostRequest(OCEntityHandlerRequest* ehRequest)
{
    OCEntityHandlerResult ret = OC_EH_INTERNAL_SERVER_ERROR;
    OIC_LOG(DEBUG, TAG, "HandleCREDPostRequest IN");

    OicSecDostype_t dos;
    static uint16_t previousMsgId = 0;
    // Get binary representation of cbor
    OicSecCred_t *cred = NULL;
    OicUuid_t     *rownerId = NULL;
    uint8_t *payload = (((OCSecurityPayload*)ehRequest->payload)->securityData);
    size_t size = (((OCSecurityPayload*)ehRequest->payload)->payloadSize);

    OCStackResult res = OC_STACK_OK;

    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDos(&dos), ERROR);
    if ((DOS_RESET == dos.state) ||
        (DOS_RFNOP == dos.state))
    {
        OIC_LOG_V(WARNING, TAG, "%s /cred resource is read-only in RESET and RFNOP.", __func__);
        ret = OC_EH_NOT_ACCEPTABLE;
        goto exit;
    }

    res = CBORPayloadToCred(payload, size, &cred, &rownerId);

    if (OC_STACK_OK == res)
    {
        OicSecCred_t *newCred = NULL;
        OicSecCred_t *newCredTemp = NULL;

        LL_FOREACH_SAFE(cred, newCred, newCredTemp)
        {
            ret = HandleNewCredential(ehRequest, newCred, previousMsgId);

            if (OC_EH_CHANGED != ret)
            {
                // Remove the credentials added so far.
                OicSecCred_t *removedCred = NULL;
                OicSecCred_t *removedCredTemp = NULL;
                LL_FOREACH_SAFE(cred, removedCred, removedCredTemp)
                {
                    if (removedCred == newCred)
                    {
                        break;
                    }

                    if (RemoveCredential(&cred->subject) != OC_STACK_OK)
                    {
                        OIC_LOG(WARNING, TAG, "Failed to remove credential");
                    }
                }

                break;
            }
        }
        if (OC_EH_CHANGED == ret && NULL != rownerId)
        {
            memcpy(&gRownerId, rownerId, sizeof(OicUuid_t));
        }
    }

exit:
    OICFree(rownerId);
    if (OC_EH_CHANGED != ret)
    {
        if (NULL != cred)
        {
            DeleteCredList(cred);
        }
    }
    else
    {
        if (OC_ADAPTER_IP == ehRequest->devAddr.adapter)
        {
            previousMsgId = ehRequest->messageID++;
        }
    }

    // Send response to request originator
    ret = ((SendSRMResponse(ehRequest, ret, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    OIC_LOG(DEBUG, TAG, "HandleCREDPostRequest OUT");
    return ret;
}

/**
 * The entity handler determines how to process a GET request.
 */
static OCEntityHandlerResult HandleGetRequest (const OCEntityHandlerRequest * ehRequest)
{
    OIC_LOG(INFO, TAG, "HandleGetRequest  processing GET request");

    // Convert Cred data into CBOR for transmission
    size_t size = 0;
    uint8_t *payload = NULL;
    int secureFlag = 1;

    const OicSecCred_t *cred = gCred;

    // This added '256' is arbitrary value that is added to cover the name of the resource, map addition and ending
    size = GetCredKeyDataSize(cred);
    size += (256 * OicSecCredCount(cred));
    OCStackResult res = CredToCBORPayload(cred, &payload, &size, secureFlag);

    // A device should always have a default cred. Therefore, payload should never be NULL.
    OCEntityHandlerResult ehRet = (res == OC_STACK_OK) ? OC_EH_OK : OC_EH_ERROR;

    //Send payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, payload, size)) == OC_STACK_OK) ?
                       OC_EH_OK : OC_EH_ERROR;
    OICClearMemory(payload, size);
    OICFree(payload);
    return ehRet;
}

static OCEntityHandlerResult HandleDeleteRequest(const OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(DEBUG, TAG, "Processing CredDeleteRequest");

    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    CredIdList_t *credIdList = NULL;
    OicUuid_t subject = { .id= { 0 } };

    if (NULL == ehRequest->query)
    {
        return ehRet;
    }

    if (GetCredIdsFromQueryString(ehRequest->query, &credIdList))
    {
        if (OC_STACK_RESOURCE_DELETED == RemoveCredentialByCredIds(credIdList))
        {
            ehRet = OC_EH_RESOURCE_DELETED;
        }
        DeleteCredIdList(&credIdList);
    }
    else if (GetSubjectFromQueryString(ehRequest->query, &subject))
    {
        if (OC_STACK_RESOURCE_DELETED == RemoveCredential(&subject))
        {
            ehRet = OC_EH_RESOURCE_DELETED;
        }
    }
    else
    {
        if (OC_STACK_OK == RemoveAllCredentials())
        {
            ehRet = OC_EH_RESOURCE_DELETED;
        }
    }

    //Send response to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;
    return ehRet;
}

OCEntityHandlerResult CredEntityHandler(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParameter)
{
    (void)callbackParameter;
    OCEntityHandlerResult ret = OC_EH_ERROR;

    if (!ehRequest)
    {
        return OC_EH_ERROR;
    }
    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG (DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        //TODO :  Remove Handle PUT methods once CTT have changed to POST on OTM
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ret = HandleGetRequest(ehRequest);
                break;
            case OC_REST_PUT:
            case OC_REST_POST:
                ret = HandlePostRequest(ehRequest);
                break;
            case OC_REST_DELETE:
                ret = HandleDeleteRequest(ehRequest);
                break;
            default:
                ret = ((SendSRMResponse(ehRequest, ret, NULL, 0)) == OC_STACK_OK) ?
                               OC_EH_OK : OC_EH_ERROR;
                break;
        }
    }
    return ret;
}

OCStackResult CreateCredResource()
{
    OCStackResult ret = OCCreateResource(&gCredHandle,
                                         OIC_RSRC_TYPE_SEC_CRED,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_CRED_URI,
                                         CredEntityHandler,
                                         NULL,
                                         OC_SECURE |
                                         OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG (FATAL, TAG, "Unable to instantiate Cred resource");
        DeInitCredResource();
    }
    return ret;
}

OCStackResult InitCredResource()
{
    OCStackResult ret = OC_STACK_ERROR;
    OicSecCred_t* cred = NULL;
    OicUuid_t   *rownerId = NULL;

    //Read Cred resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_CRED_NAME, &data, &size);
    // If database read failed
    if (ret != OC_STACK_OK)
    {
        OIC_LOG (DEBUG, TAG, "ReadSVDataFromPS failed");
    }

    if ((ret == OC_STACK_OK) && data)
    {
        // Read Cred resource from PS
        ret = CBORPayloadToCred(data, size, &gCred, &rownerId);

#ifdef HAVE_WINDOWS_H
        /* On Windows, if the credential payload isn't cleartext CBOR, it is encrypted. Decrypt and retry. */
        DWORD dwordSize;

        if (FAILED(SizeTToDWord(size, &dwordSize)))
        {
            OIC_LOG(DEBUG, TAG, "Cred size too large.");
            ret = OC_STACK_ERROR;
        }

        if (ret != OC_STACK_OK)
        {
            DATA_BLOB decryptedPayload;
            DATA_BLOB encryptedPayload;
            memset(&decryptedPayload, 0, sizeof(decryptedPayload));
            memset(&encryptedPayload, 0, sizeof(encryptedPayload));
            encryptedPayload.cbData = dwordSize;
            encryptedPayload.pbData = data;

            if (CryptUnprotectData(
                &encryptedPayload,
                NULL,
                NULL,
                NULL,
                NULL,
                CRYPTPROTECT_UI_FORBIDDEN,
                &decryptedPayload))
            {
                ret = CBORPayloadToCred(decryptedPayload.pbData, decryptedPayload.cbData, &gCred, &rownerId);

                /* For the returned data from CryptUnprotectData, LocalFree must be used to free. Don't use OICFree. */
                OICClearMemory(decryptedPayload.pbData, decryptedPayload.cbData);
                if (NULL != LocalFree(decryptedPayload.pbData))
                {
                    OIC_LOG_V(ERROR, TAG, "LocalFree failed on output from CryptUnprotectData; memory may be corrupted or leaked. Last error: %u.", GetLastError());
                    assert(!"LocalFree failed");
                }
            }
            else
            {
                /* Credential resource is corrupted, or we no longer have access to the encryption key to decrypt it. */
                OIC_LOG_V(ERROR, TAG, "Failed to CryptUnprotectData cred resource: %u", GetLastError());
                ret = OC_STACK_ERROR;
            }
        }
#endif
    }

    /*
     * If SVR database in persistent storage got corrupted or
     * is not available for some reason, a default Cred is created
     * which allows user to initiate Cred provisioning again.
     */
    if (ret != OC_STACK_OK || !data || !gCred)
    {
        gCred = GetCredDefault();
    }

    if (gCred)
    {
        OicUuid_t deviceID;
        OicUuid_t emptyUuid = {.id={0}};

        ret = GetDoxmDeviceID(&deviceID);
        if (ret != OC_STACK_OK)
        {
            OIC_LOG_V(WARNING, TAG, "%s: GetDoxmDeviceID failed, error %d", __func__, ret);
            //Unit tests expect error code OC_STACK_INVALID_PARAM.
            ret = OC_STACK_INVALID_PARAM;
            goto exit;
        }

        //Add a log to track the invalid credential.
        LL_FOREACH(gCred, cred)
        {
            if (false == CheckSubjectOfCertificate(cred, deviceID))
            {
                OIC_LOG(WARNING, TAG, "Check subject of Certificate was failed while InitCredResource");
            }
            if (false == IsValidCredential(cred))
            {
                OIC_LOG(WARNING, TAG, "Invalid credential data was dectected while InitCredResource");
                OIC_LOG_V(WARNING, TAG, "Invalid credential ID = %d", cred->credId);
            }
        }

        if (NULL == rownerId || 0 == memcmp(rownerId, &emptyUuid, sizeof(OicUuid_t)))
        {
            memcpy(&gRownerId, &deviceID, sizeof(OicUuid_t));
        }
        else
        {
            memcpy(&gRownerId, rownerId, sizeof(OicUuid_t));
        }

        if (!UpdatePersistentStorage(gCred))
        {
            OIC_LOG(FATAL, TAG, "UpdatePersistentStorage failed!");
        }
    }
    //Instantiate 'oic.sec.cred'
    ret = CreateCredResource();

exit:
    OIC_LOG(DEBUG, TAG, "OUT InitCredResource.");
    OICClearMemory(data, size);
    OICFree(data);
    OICFree(rownerId);
    return ret;
}

OCStackResult DeInitCredResource()
{
    OCStackResult result = OCDeleteResource(gCredHandle);
    DeleteCredList(gCred);
    gCred = NULL;
    return result;
}

OicSecCred_t* GetCredResourceData(const OicUuid_t* subject)
{
    OicSecCred_t *cred = NULL;

   if ( NULL == subject)
    {
       return NULL;
    }

    LL_FOREACH(gCred, cred)
    {
        if(memcmp(cred->subject.id, subject->id, sizeof(subject->id)) == 0)
        {
            return cred;
        }
    }
    return NULL;
}

const OicSecCred_t* GetCredList()
{
    return gCred;
}

OicSecCred_t* GetCredEntryByCredId(const uint16_t credId)
{
    OicSecCred_t *cred = NULL;
    OicSecCred_t *tmpCred = NULL;

   if ( 1 > credId)
    {
       return NULL;
    }

    LL_FOREACH(gCred, tmpCred)
    {
        if(tmpCred->credId == credId)
        {
            cred = (OicSecCred_t*)OICCalloc(1, sizeof(OicSecCred_t));
            VERIFY_NOT_NULL(TAG, cred, ERROR);

            // common
            cred->next = NULL;
            cred->credId = tmpCred->credId;
            cred->credType = tmpCred->credType;
            memcpy(cred->subject.id, tmpCred->subject.id , sizeof(cred->subject.id));
            if (tmpCred->period)
            {
                cred->period = OICStrdup(tmpCred->period);
            }

            // key data
            if (tmpCred->privateData.data)
            {
                cred->privateData.data = (uint8_t *)OICCalloc(1, tmpCred->privateData.len);
                VERIFY_NOT_NULL(TAG, cred->privateData.data, ERROR);

                memcpy(cred->privateData.data, tmpCred->privateData.data, tmpCred->privateData.len);
                cred->privateData.len = tmpCred->privateData.len;
                cred->privateData.encoding = tmpCred->privateData.encoding;
            }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            if (tmpCred->publicData.data)
            {
                cred->publicData.data = (uint8_t *)OICCalloc(1, tmpCred->publicData.len);
                VERIFY_NOT_NULL(TAG, cred->publicData.data, ERROR);

                memcpy(cred->publicData.data, tmpCred->publicData.data, tmpCred->publicData.len);
                cred->publicData.len = tmpCred->publicData.len;
                cred->publicData.encoding = tmpCred->publicData.encoding;
            }
            if (tmpCred->optionalData.data)
            {
                cred->optionalData.data = (uint8_t *)OICCalloc(1, tmpCred->optionalData.len);
                VERIFY_NOT_NULL(TAG, cred->optionalData.data, ERROR);

                memcpy(cred->optionalData.data, tmpCred->optionalData.data, tmpCred->optionalData.len);
                cred->optionalData.len = tmpCred->optionalData.len;
                cred->optionalData.encoding = tmpCred->optionalData.encoding;
                cred->optionalData.revstat= tmpCred->optionalData.revstat;
            }
            if (tmpCred->credUsage)
            {
                cred->credUsage = OICStrdup(tmpCred->credUsage);
            }
#endif /* __WITH_DTLS__  or __WITH_TLS__*/

            return cred;
        }
    }

exit:
    FreeCred(cred);
    return NULL;
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
int32_t GetDtlsPskCredentials(CADtlsPskCredType_t type,
              const uint8_t *desc, size_t desc_len,
              uint8_t *result, size_t result_length)
{
    int32_t ret = -1;

    if (NULL == result)
    {
        return ret;
    }

    switch (type)
    {
        case CA_DTLS_PSK_HINT:
        case CA_DTLS_PSK_IDENTITY:
            {
                OicUuid_t deviceID = {.id={0}};
                // Retrieve Device ID from doxm resource
                if ( OC_STACK_OK != GetDoxmDeviceID(&deviceID) )
                {
                    OIC_LOG (ERROR, TAG, "Unable to retrieve doxm Device ID");
                    return ret;
                }

                if (result_length < sizeof(deviceID.id))
                {
                    OIC_LOG (ERROR, TAG, "Wrong value for result_length");
                    return ret;
                }
                memcpy(result, deviceID.id, sizeof(deviceID.id));
                return (sizeof(deviceID.id));
            }
            break;

        case CA_DTLS_PSK_KEY:
            {
                OicSecCred_t *cred = NULL;
                LL_FOREACH(gCred, cred)
                {
                    if (cred->credType != SYMMETRIC_PAIR_WISE_KEY)
                    {
                        continue;
                    }

                    if ((desc_len == sizeof(cred->subject.id)) &&
                        (memcmp(desc, cred->subject.id, sizeof(cred->subject.id)) == 0))
                    {
                        /*
                         * If the credentials are valid for limited time,
                         * check their expiry.
                         */
                        if (cred->period)
                        {
                            if(IOTVTICAL_VALID_ACCESS != IsRequestWithinValidTime(cred->period, NULL))
                            {
                                OIC_LOG (INFO, TAG, "Credentials are expired.");
                                return ret;
                            }
                        }

                        // Copy PSK.
                        // TODO: Added as workaround. Will be replaced soon.
                        if(OIC_ENCODING_RAW == cred->privateData.encoding)
                        {
                            if (ValueWithinBounds(cred->privateData.len, INT32_MAX))
                            {
                                size_t len = cred->privateData.len;
                                if (result_length < len)
                                {
                                    OIC_LOG (ERROR, TAG, "Wrong value for result_length");
                                    return ret;
                                }
                                memcpy(result, cred->privateData.data, len);
                                ret = (int32_t)len;
                            }
                        }
                        else if(OIC_ENCODING_BASE64 == cred->privateData.encoding)
                        {
                            size_t outBufSize = B64DECODE_OUT_SAFESIZE((cred->privateData.len + 1));
                            uint8_t* outKey = OICCalloc(1, outBufSize);
                            size_t outKeySize;
                            if(NULL == outKey)
                            {
                                OIC_LOG (ERROR, TAG, "Failed to allocate memory.");
                                return ret;
                            }

                            if(B64_OK == b64Decode((char*)cred->privateData.data, cred->privateData.len, outKey, outBufSize, &outKeySize))
                            {
                                if (ValueWithinBounds(outKeySize, INT32_MAX))
                                {
                                    if (result_length < outKeySize)
                                    {
                                        OIC_LOG (ERROR, TAG, "Wrong value for result_length");
                                        return ret;
                                    }
                                    memcpy(result, outKey, outKeySize);
                                    ret = (int32_t)outKeySize;
                                }
                            }
                            else
                            {
                                OIC_LOG (ERROR, TAG, "Failed base64 decoding.");
                            }

                            OICFree(outKey);
                        }

                        if (OC_STACK_OK != RegisterSymmetricCredentialRole(cred))
                        {
                            OIC_LOG(WARNING, TAG, "Couldn't RegisterRoleForSubject");
                        }

                        return ret;
                    }
                }
                OIC_LOG(DEBUG, TAG, "Can not find subject matched credential.");

#ifdef MULTIPLE_OWNER
                const OicSecDoxm_t* doxm = GetDoxmResourceData();
                if(doxm && doxm->mom && OIC_MULTIPLE_OWNER_DISABLE != doxm->mom->mode)
                {
                    // in case of multiple owner transfer authentication
                    if(OIC_PRECONFIG_PIN == doxm->oxmSel)
                    {
                        OicSecCred_t* wildCardCred = GetCredResourceData(&WILDCARD_SUBJECT_ID);
                        if(wildCardCred)
                        {
                            OIC_LOG(DEBUG, TAG, "Detected wildcard credential.");
                            if(PIN_PASSWORD == wildCardCred->credType)
                            {
                                //Read PIN/PW
                                char* pinBuffer = NULL;
                                size_t pinLength = 0;
                                if(OIC_ENCODING_RAW == wildCardCred->privateData.encoding)
                                {
                                    pinBuffer = OICCalloc(1, wildCardCred->privateData.len + 1);
                                    if(NULL == pinBuffer)
                                    {
                                        OIC_LOG (ERROR, TAG, "Failed to allocate memory.");
                                        return ret;
                                    }
                                    pinLength = wildCardCred->privateData.len;
                                    memcpy(pinBuffer, wildCardCred->privateData.data, pinLength);
                                }
                                else if(OIC_ENCODING_BASE64 == wildCardCred->privateData.encoding)
                                {
                                    size_t pinBufSize = B64DECODE_OUT_SAFESIZE((wildCardCred->privateData.len + 1));
                                    pinBuffer = OICCalloc(1, pinBufSize);
                                    if(NULL == pinBuffer)
                                    {
                                        OIC_LOG (ERROR, TAG, "Failed to allocate memory.");
                                        return ret;
                                    }

                                    if(B64_OK != b64Decode((char*)wildCardCred->privateData.data, wildCardCred->privateData.len, (uint8_t*)pinBuffer, pinBufSize, &pinLength))
                                    {
                                        OIC_LOG (ERROR, TAG, "Failed to base64 decoding.");
                                        return ret;
                                    }
                                }
                                else
                                {
                                    OIC_LOG(ERROR, TAG, "Unknown encoding type of PIN/PW credential.");
                                    return ret;
                                }

                                //Set the PIN/PW to derive PSK
                                if (OC_STACK_OK != SetPreconfigPin(pinBuffer, pinLength))
                                {
                                    OICFree(pinBuffer);
                                    OIC_LOG(ERROR, TAG, "Failed to load PIN data.");
                                    return ret;
                                }
                                OICFree(pinBuffer);

                                OicUuid_t myUuid;
                                if(OC_STACK_OK != GetDoxmDeviceID(&myUuid))
                                {
                                    OIC_LOG(ERROR, TAG, "Failed to read device ID");
                                    return ret;
                                }
                                SetUuidForPinBasedOxm(&myUuid);

                                //Calculate PSK using PIN/PW
                                if(0 == DerivePSKUsingPIN((uint8_t*)result))
                                {
                                    ret = OWNER_PSK_LENGTH_128;
                                }
                                else
                                {
                                    OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN");
                                }

                                if(CA_STATUS_OK != CAregisterSslHandshakeCallback(MultipleOwnerDTLSHandshakeCB))
                                {
                                    OIC_LOG(WARNING, TAG, "Error while bind the DTLS Handshake Callback.");
                                }
                            }
                        }
                    }
                    else if(OIC_RANDOM_DEVICE_PIN == doxm->oxmSel)
                    {
                        if(0 == DerivePSKUsingPIN((uint8_t*)result))
                        {
                            ret = OWNER_PSK_LENGTH_128;
                        }
                        else
                        {
                            OIC_LOG_V(ERROR, TAG, "Failed to derive crypto key from PIN : result");
                            ret = -1;
                        }
                    }
                }
#endif //MULTIPLE_OWNER
            }
            break;
    }

    return ret;
}

/**
 * Add temporal PSK to PIN based OxM
 *
 * @param[in] tmpSubject UUID of target device
 * @param[in] credType Type of credential to be added
 * @param[in] pin numeric characters
 * @param[in] pinSize length of 'pin'
 * @param[in] rownerID Resource owner's UUID
 * @param[out] tmpCredSubject Generated credential's subject.
 *
 * @return OC_STACK_OK for success and errorcode otherwise.
 */
OCStackResult AddTmpPskWithPIN(const OicUuid_t* tmpSubject, OicSecCredType_t credType,
                            const char * pin, size_t pinSize,
                            const OicUuid_t * rownerID, OicUuid_t* tmpCredSubject)
{
    OCStackResult ret = OC_STACK_ERROR;
    OIC_LOG(DEBUG, TAG, "AddTmpPskWithPIN IN");

    if(NULL == tmpSubject || NULL == pin || 0 == pinSize || NULL == tmpCredSubject)
    {
        return OC_STACK_INVALID_PARAM;
    }

    uint8_t privData[OWNER_PSK_LENGTH_128] = {0,};
    OicSecKey_t privKey;
    memset(&privKey, 0, sizeof(privKey));
    privKey.data = privData;
    privKey.len = OWNER_PSK_LENGTH_128;
    privKey.encoding = OIC_ENCODING_RAW;

    OicSecCred_t* cred = NULL;
    int dtlsRes = DeriveCryptoKeyFromPassword((const unsigned char *)pin, pinSize, rownerID->id,
                                              UUID_LENGTH, PBKDF_ITERATIONS,
                                              OWNER_PSK_LENGTH_128, privData);
    VERIFY_SUCCESS(TAG, (0 == dtlsRes) , ERROR);

    cred = GenerateCredential(tmpSubject, credType, NULL,
                              &privKey, rownerID, NULL);
    OICClearMemory(privData, sizeof(privData));
    if(NULL == cred)
    {
        OIC_LOG(ERROR, TAG, "GeneratePskWithPIN() : Failed to generate credential");
        return OC_STACK_ERROR;
    }

    memcpy(tmpCredSubject->id, cred->subject.id, UUID_LENGTH);

    ret = AddCredential(cred);
    if( OC_STACK_OK != ret)
    {
        RemoveCredential(tmpSubject);
        OIC_LOG(ERROR, TAG, "GeneratePskWithPIN() : Failed to add credential");
    }
    OIC_LOG(DEBUG, TAG, "AddTmpPskWithPIN OUT");

exit:
    return ret;
}

#endif /* __WITH_DTLS__ */

OCStackResult SetCredRownerId(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicUuid_t prevId = {.id={0}};

    if(NULL == newROwner)
    {
        ret = OC_STACK_INVALID_PARAM;
    }

    if (newROwner)
    {
        memcpy(prevId.id, gRownerId.id, sizeof(prevId.id));
        memcpy(gRownerId.id, newROwner->id, sizeof(newROwner->id));

        VERIFY_SUCCESS(TAG, UpdatePersistentStorage(gCred), ERROR);

        ret = OC_STACK_OK;
    }

    return ret;

exit:
    memcpy(gRownerId.id, prevId.id, sizeof(prevId.id));
    return ret;
}

OCStackResult GetCredRownerId(OicUuid_t *rowneruuid)
{
    if (rowneruuid)
    {
        memcpy(&(rowneruuid->id), &(gRownerId.id), sizeof(rowneruuid->id));
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
/* Caller must call OICFree on *der when finished. */
static int ConvertPemCertToDer(const char *pem, size_t pemLen, uint8_t** der, size_t* derLen)
{
    const char* pemHeader = "-----BEGIN CERTIFICATE-----"; /* no newlines allowed here */
    const char* pemFooter = "-----END CERTIFICATE-----";

    mbedtls_pem_context ctx;
    int ret;

    OC_UNUSED(pemLen);

    mbedtls_pem_init(&ctx);
    size_t usedLen;
    ret = mbedtls_pem_read_buffer(&ctx, pemHeader, pemFooter, (const uint8_t*) pem, NULL, 0, &usedLen);
    if (ret != 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s: failed reading PEM cert", __func__);
        goto exit;
    }

    uint8_t *buf = OICCalloc(1, ctx.buflen);
    if (NULL == buf)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory");
        ret = -1;
        goto exit;
    }

    memcpy(buf, ctx.buf, ctx.buflen);

    *der = buf;
    *derLen = ctx.buflen;

exit:
    mbedtls_pem_free(&ctx);

    return ret;
}

/* Caller must call OICFree on *pem when finished. */
static int ConvertDerCertToPem(const uint8_t* der, size_t derLen, uint8_t** pem)
{
    const char* pemHeader = "-----BEGIN CERTIFICATE-----\n";
    const char* pemFooter = "-----END CERTIFICATE-----\n";

    /* Get the length required for output */
    size_t pemLen;
    int ret = mbedtls_pem_write_buffer(pemHeader,
        pemFooter,
        der,
        derLen,
        NULL,
        0,
        &pemLen);
    if (ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL)
    {
        OIC_LOG_V(ERROR, TAG, "Couldn't convert cert into PEM, failed getting required length: %d", ret);
        return ret;
    }

    *pem = OICCalloc(1, pemLen + 1);
    if (*pem == NULL)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory for PEM cert");
        return ret;
    }

    /* Try the conversion */
    ret = mbedtls_pem_write_buffer(pemHeader,
        pemFooter,
        der,
        derLen,
        *pem,
        pemLen,
        &pemLen);
    if (ret < 0)
    {
        OIC_LOG_V(ERROR, TAG, "Couldn't convert cert into PEM, failed writing PEM: %d", ret);
        OICFreeAndSetToNull((void **) pem);
        return ret;
    }

    return 0;
}

static OCStackResult GetCaCert(ByteArray_t * crt, const char * usage, OicEncodingType_t desiredEncoding)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == crt || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return OC_STACK_INVALID_PARAM;
    }

    switch (desiredEncoding)
    {
    case OIC_ENCODING_PEM:
    case OIC_ENCODING_DER:
    case OIC_ENCODING_BASE64:
        break;
    default:
        OIC_LOG_V(ERROR, TAG, "%s: Unsupported encoding %d", __func__, desiredEncoding);
        return OC_STACK_INVALID_PARAM;
    }

    crt->len = 0;
    OicSecCred_t* temp = NULL;

    LL_FOREACH(gCred, temp)
    {
        if ((SIGNED_ASYMMETRIC_KEY == temp->credType) &&
            (temp->credUsage != NULL) &&
            (0 == strcmp(temp->credUsage, usage)) && (false == temp->optionalData.revstat))
        {

            if ((OIC_ENCODING_BASE64 != temp->publicData.encoding) &&
                (OIC_ENCODING_PEM != temp->publicData.encoding) &&
                (OIC_ENCODING_DER != temp->publicData.encoding))
            {
                OIC_LOG_V(WARNING, TAG, "%s: Unknown encoding type", __func__);
                continue;
            }

            if (OIC_ENCODING_DER == desiredEncoding)
            {
                if ((OIC_ENCODING_BASE64 == temp->publicData.encoding) ||
                    (OIC_ENCODING_PEM == temp->publicData.encoding))
                {
                    uint8_t* buf = NULL;
                    size_t outSize = 0;
                    int ret = ConvertPemCertToDer((const char*)temp->publicData.data, temp->publicData.len, &buf, &outSize);
                    if (0 > ret)
                    {
                        OIC_LOG(ERROR, TAG, "Could not convert PEM cert to DER");
                        return OC_STACK_ERROR;
                    }

                    uint8_t *savePtr = crt->data;
                    crt->data = OICRealloc(crt->data, crt->len + outSize);
                    if (NULL == crt->data)
                    {
                        OIC_LOG(ERROR, TAG, "No memory reallocating crt->data");
                        OICFree(savePtr);
                        OICFree(buf);
                        return OC_STACK_NO_MEMORY;
                    }
                    memcpy(crt->data + crt->len, buf, outSize);
                    crt->len += outSize;
                    OICFree(buf);
                }
                else
                {
                    uint8_t *savePtr = crt->data;
                    crt->data = OICRealloc(crt->data, crt->len + temp->publicData.len);
                    if (NULL == crt->data)
                    {
                        OIC_LOG(ERROR, TAG, "No memory reallocating crt->data");
                        OICFree(savePtr);
                        return OC_STACK_NO_MEMORY;
                    }
                    memcpy(crt->data + crt->len, temp->publicData.data, temp->publicData.len);
                    crt->len += temp->publicData.len;
                }
                OIC_LOG_V(DEBUG, TAG, "%s found", usage);
            }
            else
            {
                /* PEM/Base64 */
                uint8_t *pem = NULL;
                size_t pemLen = 0;
                if ((OIC_ENCODING_BASE64 == temp->publicData.encoding) ||
                    (OIC_ENCODING_PEM == temp->publicData.encoding))
                {
                    pem = temp->publicData.data;
                    pemLen = temp->publicData.len;
                }
                else
                {
                    int ret = ConvertDerCertToPem(temp->publicData.data, temp->publicData.len, &pem);
                    if (0 > ret)
                    {
                        OIC_LOG_V(ERROR, TAG, "Failed converting DER cert to PEM: %d", ret);
                        return OC_STACK_ERROR;
                    }
                    pemLen = strlen((char *)pem) + 1;
                }

                uint8_t *oldData = crt->data;
                crt->data = OICRealloc(crt->data, crt->len + pemLen);
                if (NULL == crt->data)
                {
                    OIC_LOG(ERROR, TAG, "No memory reallocating crt->data");
                    OICFree(oldData);
                    return OC_STACK_NO_MEMORY;
                }
                memcpy(crt->data + crt->len, pem, pemLen);
                crt->len += pemLen;
            }
        }
    }
    if(0 == crt->len)
    {
        OIC_LOG_V(WARNING, TAG, "%s not found", usage);
        return OC_STACK_NO_RESOURCE;
    }

    if (OIC_ENCODING_PEM == desiredEncoding)
    {
        /* mbedtls_x509_crt_parse requires a null terminator to determine that the format is PEM */
        size_t crtLength = crt->len;
        bool addNull = (crt->data[crtLength - 1] != 0);

        if (addNull)
        {
            OIC_LOG_V(DEBUG, TAG, "%s: adding null terminator at the end of the cert", __func__);
            uint8_t *oldData = crt->data;
            crt->data = OICRealloc(crt->data, crtLength + 1);
            if (NULL == crt->data)
            {
                OIC_LOG(ERROR, TAG, "No memory reallocating crt->data");
                OICFree(oldData);
                return OC_STACK_NO_MEMORY;
            }
            crt->data[crtLength] = 0;
            crt->len = crtLength + 1;
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_OK;
}

OCStackResult GetPemCaCert(ByteArray_t * crt, const char * usage)
{
    return GetCaCert(crt, usage, OIC_ENCODING_PEM);
}

static int cloneSecKey(OicSecKey_t * dst, OicSecKey_t * src)
{
    if ((src == NULL) || (dst == NULL))
    {
        return -1;
    }

    if (src->len > 0)
    {
        dst->data = OICCalloc(src->len, 1);
        if (dst->data == NULL)
        {
            OIC_LOG_V(ERROR, TAG, "%s memory allocation failed", __func__);
            return -1;
        }
        memcpy(dst->data, src->data, src->len);
    }
    else
    {
        dst->data = NULL;
    }

    dst->len = src->len;
    dst->encoding = src->encoding;

    return 0;
}

/* Caller must call FreeRoleCertChainList on roleEntries when finished. */
OCStackResult GetAllRoleCerts(RoleCertChain_t ** output)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    *output = NULL;

    OicSecCred_t * temp = NULL;
    LL_FOREACH(gCred, temp)
    {
        if ((SIGNED_ASYMMETRIC_KEY == temp->credType) &&
            (temp->credUsage != NULL) &&
            (0 == strcmp(temp->credUsage, ROLE_CERT)))
        {
            if (temp->publicData.data == NULL)
            {
                OIC_LOG_V(WARNING, TAG, "%s missing certificate data in role certificate", __func__);
                continue;
            }

            RoleCertChain_t * add = (RoleCertChain_t *) OICCalloc(1, sizeof(RoleCertChain_t));
            if (add == NULL)
            {
                OIC_LOG_V(ERROR, TAG, "%s Failed to allocate memory", __func__);
                goto error;
            }
            LL_APPEND(*output, add);
            add->credId = temp->credId;
            if (cloneSecKey(&add->certificate, &temp->publicData) != 0)
            {
                OIC_LOG_V(ERROR, TAG, "%s failed to copy certificate data", __func__);
                goto error;
            }
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_OK;

error:
    FreeRoleCertChainList(*output);
    *output = NULL;
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_ERROR;
}

void GetPemOwnCert(ByteArray_t * crt, const char * usage)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == crt || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    crt->len = 0;
    OicSecCred_t * temp = NULL;
    LL_FOREACH(gCred, temp)
    {
        if (SIGNED_ASYMMETRIC_KEY == temp->credType &&
            temp->credUsage != NULL &&
            0 == strcmp(temp->credUsage, usage))
        {
            uint8_t *p = NULL;
            int mbedRet = 0;
            uint8_t *pem = NULL;
            size_t pemLen = 0;
            bool mustFreePem = false;
            bool mustAddNull = true;

            switch (temp->publicData.encoding)
            {
            case OIC_ENCODING_DER:
            case OIC_ENCODING_RAW:
                mbedRet = ConvertDerCertToPem(temp->publicData.data, temp->publicData.len, &pem);
                if (0 > mbedRet)
                {
                    OIC_LOG_V(ERROR, TAG, "Failed to ConvertDerCertToPem: %d", mbedRet);
                    return;
                }
                mustFreePem = true;
                mustAddNull = false; /* mbedTLS always NULL-terminates. */
                pemLen = strlen((char *)pem) + 1;
                break;

            case OIC_ENCODING_PEM:
            case OIC_ENCODING_BASE64:
                pem = temp->publicData.data;
                pemLen = temp->publicData.len;

                /* Make sure the buffer has a terminating NULL. If not, make sure we add one later. */
                for (size_t i = pemLen - 1; i > 0; i--)
                {
                    if ('\0' == (char)pem[i])
                    {
                        mustAddNull = false;
                        break;
                    }
                }
                break;

            default:
                OIC_LOG_V(ERROR, TAG, "Unsupported encoding %d", temp->publicData.encoding);
                return;
            }

            p = crt->data;
            crt->data = OICRealloc(crt->data, crt->len + pemLen + (mustAddNull ? 1 : 0));
            if (NULL == crt->data)
            {
                OIC_LOG(ERROR, TAG, "No memory reallocating crt->data");
                OICFree(p);
                if (mustFreePem)
                {
                    OICFree(pem);
                }
                return;
            }

            /* If we're appending, subtract one from crt->len below so we overwrite the current terminating
             * NULL with the beginning of the new data.
             */
            if (0 < crt->len)
            {
                assert(crt->data[crt->len - 1] == '\0');
                memcpy(crt->data + crt->len - 1, pem, pemLen);
                crt->len += pemLen - 1;
            }
            else
            {
                memcpy(crt->data, pem, pemLen);
                crt->len = pemLen;
            }

            /* If pem doesn't contain a terminating NULL, add one. */
            if (mustAddNull)
            {
                assert(crt->data[crt->len - 1] != '\0');
                crt->data[crt->len] = '\0';
                crt->len += 1;
            }

            if (mustFreePem)
            {
                OICFree(pem);
            }

            OIC_LOG_V(DEBUG, TAG, "%s found", usage);
        }
    }
    if(0 == crt->len)
    {
        OIC_LOG_V(WARNING, TAG, "%s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return;
}

void GetDerKey(ByteArray_t * key, const char * usage)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == key || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }

    OicSecCred_t * temp = NULL;
    key->len = 0;
    LL_FOREACH(gCred, temp)
    {
        if ((SIGNED_ASYMMETRIC_KEY == temp->credType || ASYMMETRIC_KEY == temp->credType) &&
            temp->privateData.len > 0 &&
            NULL != temp->credUsage &&
            0 == strcmp(temp->credUsage, usage))
        {

            if (temp->privateData.encoding == OIC_ENCODING_PEM)
            {
                /* Convert PEM to DER */
                const char* pemHeader = "-----BEGIN EC PRIVATE KEY-----"; /* no newlines allowed here */
                const char* pemFooter = "-----END EC PRIVATE KEY-----";
                uint8_t *data = temp->privateData.data;
                size_t length = temp->privateData.len;
                bool freeData = false;

                if (data[length - 1] != 0)
                {
                    /* Add a null terminator, because mbedtls_pem_read_buffer requires it */
                    OIC_LOG_V(DEBUG, TAG, "%s: adding null terminator to privateData", __func__);

                    data = OICMalloc(length + 1);
                    if (NULL == data)
                    {
                        OIC_LOG(ERROR, TAG, "Failed to allocate memory");
                        return;
                    }

                    memcpy(data, temp->privateData.data, length);
                    data[length] = 0;
                    freeData = true;
                }

                mbedtls_pem_context ctx;
                int ret;
                size_t usedLen;

                mbedtls_pem_init(&ctx);
                ret = mbedtls_pem_read_buffer(&ctx, pemHeader, pemFooter, data, NULL, 0, &usedLen);

                if (freeData)
                {
                    OICFree(data);
                }

                if (ret != 0)
                {
                    OIC_LOG_V(ERROR, TAG, "%s: failed reading PEM key", __func__);
                    mbedtls_pem_free(&ctx);
                    return;
                }

                uint8_t *tmp = OICRealloc(key->data, ctx.buflen);
                if (NULL == tmp)
                {
                    OICFree(key->data);
                    key->data = NULL;
                    OIC_LOG(ERROR, TAG, "Failed to allocate memory");
                    mbedtls_pem_free(&ctx);
                    return;
                }
                key->data = tmp;

                memcpy(key->data, ctx.buf, ctx.buflen);
                key->len = ctx.buflen;
                mbedtls_pem_free(&ctx);
                break;
            }
            else if(temp->privateData.encoding == OIC_ENCODING_DER || temp->privateData.encoding == OIC_ENCODING_RAW)
            {
                uint8_t *tmp = OICRealloc(key->data, key->len + temp->privateData.len);
                if (NULL == tmp)
                {
                    OICFree(key->data);
                    key->data = NULL;
                    OIC_LOG(ERROR, TAG, "Failed to allocate memory");
                    return;
                }
                key->data = tmp;
                memcpy(key->data + key->len, temp->privateData.data, temp->privateData.len);
                key->len += temp->privateData.len;
                OIC_LOG_V(DEBUG, TAG, "Key for %s found", usage);
                break;
            }
            else
            {
                OIC_LOG_V(WARNING, TAG, "Key for %s found, but it has an unknown encoding (%d)", usage, temp->privateData.encoding);
            }
        }
    }
    if(0 == key->len)
    {
        OIC_LOG_V(WARNING, TAG, "Key for %s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

void InitCipherSuiteListInternal(bool * list, const char * usage, const char *deviceId)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == list || NULL == usage)
    {
        OIC_LOG(DEBUG, TAG, "NULL passed");
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    OicSecCred_t * temp = NULL;
    LL_FOREACH(gCred, temp)
    {
        switch (temp->credType)
        {
            case PIN_PASSWORD:
            {
                list[0] = true;
                OIC_LOG(DEBUG, TAG, "PIN_PASSWORD found");
                break;
            }
            case SYMMETRIC_PAIR_WISE_KEY:
            {
                OicUuid_t uuid;

                if (NULL == deviceId || deviceId[0] == '\0' ||
                    OC_STACK_OK != ConvertStrToUuid(deviceId, &uuid) ||
                    0 == memcmp(uuid.id, temp->subject.id, sizeof(uuid.id)))
                {
                    list[0] = true;
                    OIC_LOG(DEBUG, TAG, "SYMMETRIC_PAIR_WISE_KEY found");
                }
                break;
            }
            case SIGNED_ASYMMETRIC_KEY:
            {
                if (NULL != temp->credUsage && 0 == strcmp(temp->credUsage, usage))
                {
                    list[1] = true;
                    OIC_LOG_V(DEBUG, TAG, "SIGNED_ASYMMETRIC_KEY found for %s", usage);
                }
                break;
            }
            case SYMMETRIC_GROUP_KEY:
            case ASYMMETRIC_KEY:
            case ASYMMETRIC_ENCRYPTION_KEY:
            {
                OIC_LOG(WARNING, TAG, "Unsupported credential type for TLS.");
                break;
            }
            default:
            {
                OIC_LOG(WARNING, TAG, "Unknown credential type for TLS.");
                break;
            }
        }
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}
#endif
