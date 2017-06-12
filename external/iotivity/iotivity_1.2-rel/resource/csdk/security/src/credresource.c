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

#ifdef __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#define TAG  "OIC_SRM_CREDL"

/** Max credential types number used for TLS */
#define MAX_TYPE 2
/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 2048;

/** Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

/** CRED size - Number of mandatory items. */
static const uint8_t CRED_ROOT_MAP_SIZE = 4;
static const uint8_t CRED_MAP_SIZE = 3;

static OicSecCred_t        *gCred = NULL;
static OCResourceHandle    gCredHandle = NULL;

#ifdef MULTIPLE_OWNER
#define PRECONF_PIN_MIN_SIZE (8)
#endif

typedef enum CredCompareResult{
    CRED_CMP_EQUAL = 0,
    CRED_CMP_NOT_EQUAL = 1,
    CRED_CMP_ERROR = 2
}CredCompareResult_t;

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
    OicUuid_t emptyUuid = {.id={0}};
    OIC_LOG(DEBUG, TAG, "IN CheckSubjectOfCertificate");
    VERIFY_NON_NULL(TAG, cred, ERROR);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    if ( SIGNED_ASYMMETRIC_KEY == cred->credType)
    {
        if((0 == memcmp(cred->subject.id, emptyUuid.id, sizeof(cred->subject.id))) ||
            (0 == memcmp(cred->subject.id, &WILDCARD_SUBJECT_ID, sizeof(cred->subject.id))))
        {
            memcpy(cred->subject.id, deviceID.id, sizeof(deviceID.id));
        }
    }
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

    VERIFY_NON_NULL(TAG, cred, ERROR);
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
            VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);
            VERIFY_SUCCESS(TAG, 0 != cred->privateData.len, ERROR);
            VERIFY_SUCCESS(TAG, \
                           (OIC_ENCODING_RAW == cred->privateData.encoding || \
                           OIC_ENCODING_BASE64 == cred->privateData.encoding), \
                           ERROR);
            break;
        }
        case ASYMMETRIC_KEY:
        {
            VERIFY_NON_NULL(TAG, cred->publicData.data, ERROR);
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
            VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);
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
static void FreeCred(OicSecCred_t *cred)
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
    OIC_LOG_V(DEBUG, TAG, "Cred Key Data Size : %zd\n", size);
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

static char* EncodingValueToString(OicEncodingType_t encoding)
{
    char* str = NULL;
    switch (encoding)
    {
        case OIC_ENCODING_RAW:
            str = (char*)OIC_SEC_ENCODING_RAW;
            break;
        case OIC_ENCODING_BASE64:
            str = (char*)OIC_SEC_ENCODING_BASE64;
            break;
        case OIC_ENCODING_DER:
            str = (char*)OIC_SEC_ENCODING_DER;
            break;
        case OIC_ENCODING_PEM:
            str = (char*)OIC_SEC_ENCODING_PEM;
            break;
        default:
            break;
    }
    return str;
}

static CborError SerializeEncodingToCborInternal(CborEncoder *map, const OicSecKey_t *value)
{
    CborError cborEncoderResult = CborNoError;
    char *encoding = EncodingValueToString(value->encoding);
    if (encoding)
    {
        cborEncoderResult = cbor_encode_text_string(map, OIC_JSON_ENCODING_NAME,
            strlen(OIC_JSON_ENCODING_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Encoding Tag.");
        cborEncoderResult = cbor_encode_text_string(map, encoding,
            strlen(encoding));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Encoding Value.");

        cborEncoderResult = cbor_encode_text_string(map, OIC_JSON_DATA_NAME,
            strlen(OIC_JSON_DATA_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Name Tag.");
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
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Name Value.");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Unknown encoding type.");
        VERIFY_CBOR_SUCCESS(TAG, CborErrorUnknownType, "Failed Adding Encoding Value.");
    }
    exit:
    return cborEncoderResult;
}

static CborError SerializeEncodingToCbor(CborEncoder *rootMap, const char* tag, const OicSecKey_t *value)
{
    CborError cborEncoderResult = CborNoError;
    CborEncoder map;
    const size_t mapSize = 2;

    cborEncoderResult = cbor_encode_text_string(rootMap, tag, strlen(tag));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");

    cborEncoderResult = cbor_encoder_create_map(rootMap, &map, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Map");

    VERIFY_CBOR_SUCCESS(TAG, SerializeEncodingToCborInternal(&map, value),
                        "Failed adding OicSecKey_t structure");

    cborEncoderResult = cbor_encoder_close_container(rootMap, &map);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Map.");

    exit:
    return cborEncoderResult;
}

static CborError SerializeSecOptToCbor(CborEncoder *rootMap, const char* tag, const OicSecOpt_t *value)
{
    CborError cborEncoderResult = CborNoError;
    CborEncoder map;
    const size_t mapSize = 3;

    cborEncoderResult = cbor_encode_text_string(rootMap, tag, strlen(tag));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");

    cborEncoderResult = cbor_encoder_create_map(rootMap, &map, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Map");

    OicSecKey_t in;
    in.data = value->data;
    in.encoding = value->encoding;
    in.len = value->len;

    VERIFY_CBOR_SUCCESS(TAG, SerializeEncodingToCborInternal(&map, &in),
                        "Failed adding OicSecKey_t structure");

    cborEncoderResult = cbor_encode_text_string(&map, OIC_JSON_REVOCATION_STATUS_NAME,
        strlen(OIC_JSON_REVOCATION_STATUS_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding optional revstat Tag.");
    cborEncoderResult = cbor_encode_boolean(&map, value->revstat);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding optional revstat Value.");

    cborEncoderResult = cbor_encoder_close_container(rootMap, &map);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Map.");

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
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PrivateData.");
    }

    // encoding -- Mandatory
    if (strcmp(name, OIC_JSON_ENCODING_NAME) == 0)
    {
        char* strEncoding = NULL;
        cborFindResult = cbor_value_dup_text_string(map, &strEncoding, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding EncodingType");

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
            OIC_LOG(WARNING, TAG, "Unknown encoding type detected.");
        }
        OICFree(strEncoding);
    }
    exit:
    return cborFindResult;
}

static CborError DeserializeEncodingFromCbor(CborValue *rootMap, OicSecKey_t *value)
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
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get text");
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
        }
        if (name)
        {
            VERIFY_CBOR_SUCCESS(TAG, DeserializeEncodingFromCborInternal(&map, name, value),
                                "Failed to read OicSecKey_t value");
        }
        if (cbor_value_is_valid(&map))
        {
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Map.");
        }
        OICFree(name);
    }
    exit:
    return cborFindResult;
}

static CborError DeserializeSecOptFromCbor(CborValue *rootMap, OicSecOpt_t *value)
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
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get text");
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
        }
        if (name)
        {
            // OptionalData::revstat -- Mandatory
            if (strcmp(name, OIC_JSON_REVOCATION_STATUS_NAME) == 0)
            {
                cborFindResult = cbor_value_get_boolean(&map, &value->revstat);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding revstat Value.")
            }
            OicSecKey_t out;
            VERIFY_CBOR_SUCCESS(TAG, DeserializeEncodingFromCborInternal(&map, name, &out),
                                "Failed to read OicSecKey_t value");

            value->data = out.data;
            value->encoding = out.encoding;
            value->len = out.len;
        }
        if (cbor_value_is_valid(&map))
        {
            cborFindResult = cbor_value_advance(&map);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Map.");
        }
        OICFree(name);
    }
    exit:
    return cborFindResult;
}

OCStackResult CredToCBORPayload(const OicSecCred_t *credS, uint8_t **cborPayload,
                                size_t *cborSize, int secureFlag)
{
    if (NULL == credS || NULL == cborPayload || NULL != *cborPayload || NULL == cborSize)
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

    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create CRED Root Map (creds, rownerid)
    cborEncoderResult = cbor_encoder_create_map(&encoder, &credRootMap, CRED_ROOT_MAP_SIZE);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Root Map");

    // creds
    cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_CREDS_NAME,
        strlen(OIC_JSON_CREDS_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding creds Name Tag.");

    // creds array
    cborEncoderResult = cbor_encoder_create_array(&credRootMap, &credArray, OicSecCredCount(cred));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Array.");

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

        if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->publicData.data)
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
#endif /* __WITH_DTLS__ ||  __WITH_TLS__*/
        if (!secureFlag && cred->privateData.data)
        {
            mapSize++;
        }
        cborEncoderResult = cbor_encoder_create_map(&credArray, &credMap, mapSize);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Map");

        //CredID -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDID_NAME,
            strlen(OIC_JSON_CREDID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Id Tag. ");
        cborEncoderResult = cbor_encode_int(&credMap, cred->credId);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Id Value.");

        //Subject -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_SUBJECTID_NAME,
            strlen(OIC_JSON_SUBJECTID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Subject Tag.");
        inLen = (memcmp(&(cred->subject), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0) ?
            WILDCARD_SUBJECT_ID_LEN : sizeof(OicUuid_t);
        if(inLen == WILDCARD_SUBJECT_ID_LEN)
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, WILDCARD_RESOURCE_URI,
                strlen(WILDCARD_RESOURCE_URI));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject Id wildcard Value.");
        }
        else
        {
            char *subject = NULL;
            ret = ConvertUuidToStr(&cred->subject, &subject);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&credMap, subject, strlen(subject));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject Id Value.");
            OICFree(subject);
        }

        //CredType -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDTYPE_NAME,
            strlen(OIC_JSON_CREDTYPE_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Type Tag.");
        cborEncoderResult = cbor_encode_int(&credMap, cred->credType);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Type Value.");

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        //PublicData -- Not Mandatory
        if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->publicData.data)
        {
            cborEncoderResult = SerializeEncodingToCbor(&credMap,
                                         OIC_JSON_PUBLICDATA_NAME, &cred->publicData);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PublicData Tag.");
        }
        //OptionalData -- Not Mandatory
        if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->optionalData.data)
        {
            cborEncoderResult = SerializeSecOptToCbor(&credMap,
                                         OIC_JSON_OPTDATA_NAME, &cred->optionalData);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding OptionalData Tag.");
        }
        //CredUsage -- Not Mandatory
        if(cred->credUsage)
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDUSAGE_NAME,
                strlen(OIC_JSON_CREDUSAGE_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Credusage Name Tag.");
            cborEncoderResult = cbor_encode_text_string(&credMap, cred->credUsage,
                strlen(cred->credUsage));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Credusage Name Value.");
        }
#endif /* __WITH_DTLS__ ||  __WITH_TLS__*/
        //PrivateData -- Not Mandatory
        if(!secureFlag && cred->privateData.data)
        {
            cborEncoderResult = SerializeEncodingToCbor(&credMap,
                                         OIC_JSON_PRIVATEDATA_NAME, &cred->privateData);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");
        }

        //Period -- Not Mandatory
        if(cred->period)
        {
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_PERIOD_NAME,
                strlen(OIC_JSON_PERIOD_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Name Tag.");
            cborEncoderResult = cbor_encode_text_string(&credMap, cred->period,
                strlen(cred->period));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Name Value.");
        }

#ifdef MULTIPLE_OWNER
        // Eownerid -- Not Mandatory
        if(cred->eownerID)
        {
            char *eowner = NULL;
            cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_EOWNERID_NAME,
                strlen(OIC_JSON_EOWNERID_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding eownerId Name Tag.");
            ret = ConvertUuidToStr(cred->eownerID, &eowner);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&credMap, eowner, strlen(eowner));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding eownerId Value.");
            OICFree(eowner);
        }
#endif //MULTIPLE_OWNER

        cborEncoderResult = cbor_encoder_close_container(&credArray, &credMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Cred Map.");

        cred = cred->next;
    }
    cborEncoderResult = cbor_encoder_close_container(&credRootMap, &credArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Cred Array.");

    cred = credS;

    // Rownerid
    {
        char *rowner = NULL;
        cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_ROWNERID_NAME,
            strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding rownerid Name.");
        ret = ConvertUuidToStr(&cred->rownerID, &rowner);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&credRootMap, rowner, strlen(rowner));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding rownerid Value.");
        OICFree(rowner);
    }

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_RT_NAME,
            strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&credRootMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_CRED,
                strlen(OIC_RSRC_TYPE_SEC_CRED));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&credRootMap, &rtArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing RT.");

    //IF-- Mandatory
    CborEncoder ifArray;
    cborEncoderResult = cbor_encode_text_string(&credRootMap, OIC_JSON_IF_NAME,
             strlen(OIC_JSON_IF_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&credRootMap, &ifArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
                strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&credRootMap, &ifArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing IF.");


    // Close CRED Root Map
    cborEncoderResult = cbor_encoder_close_container(&encoder, &credRootMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing CRED Root Map.");

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

    return ret;
}

OCStackResult CBORPayloadToCred(const uint8_t *cborPayload, size_t size,
                                OicSecCred_t **secCred)
{
    if (NULL == cborPayload || NULL == secCred || NULL != *secCred || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    CborValue credCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;
    cbor_parser_init(cborPayload, size, 0, &parser, &credCbor);

    OicSecCred_t *headCred = (OicSecCred_t *) OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NON_NULL(TAG, headCred, ERROR);

    // Enter CRED Root Map
    CborValue CredRootMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    cborFindResult = cbor_value_enter_container(&credCbor, &CredRootMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering CRED Root Map.");

    while (cbor_value_is_valid(&CredRootMap))
    {
        char* tagName = NULL;
        size_t len = 0;
        CborType type = cbor_value_get_type(&CredRootMap);
        if (type == CborTextStringType && cbor_value_is_text_string(&CredRootMap))
        {
            cborFindResult = cbor_value_dup_text_string(&CredRootMap, &tagName, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in CRED Root Map.");
            cborFindResult = cbor_value_advance(&CredRootMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in CRED Root Map.");
        }
        if(tagName)
        {
            if (strcmp(tagName, OIC_JSON_CREDS_NAME)  == 0)
            {
                // Enter CREDS Array
                size_t len = 0;
                int credCount = 0;
                CborValue credArray = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                cborFindResult = cbor_value_enter_container(&CredRootMap, &credArray);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Cred Array.");

                while (cbor_value_is_valid(&credArray))
                {
                    credCount++;
                    //CredId -- Mandatory
                    CborValue credMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                    cborFindResult = cbor_value_enter_container(&credArray, &credMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Cred Map.");
                    OicSecCred_t *cred = NULL;

                    if(1 == credCount)
                    {
                        cred = headCred;
                    }
                    else
                    {
                        cred = (OicSecCred_t *) OICCalloc(1, sizeof(OicSecCred_t));
                        VERIFY_NON_NULL(TAG, cred, ERROR);
                        OicSecCred_t *temp = headCred;
                        while (temp->next)
                        {
                            temp = temp->next;
                        }
                        temp->next = cred;
                    }

                    while(cbor_value_is_valid(&credMap) && cbor_value_is_text_string(&credMap))
                    {
                        char* name = NULL;
                        CborType type = cbor_value_get_type(&credMap);
                        if (type == CborTextStringType)
                        {
                            cborFindResult = cbor_value_dup_text_string(&credMap, &name, &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in CRED Map.");
                            cborFindResult = cbor_value_advance(&credMap);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in CRED Map.");
                        }
                        if(name)
                        {
                            //credid
                            if (strcmp(name, OIC_JSON_CREDID_NAME)  == 0)
                            {
                                uint64_t credId = 0;
                                cborFindResult = cbor_value_get_uint64(&credMap, &credId);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CredId.");
                                cred->credId = (uint16_t)credId;
                            }
                            // subjectid
                            if (strcmp(name, OIC_JSON_SUBJECTID_NAME)  == 0)
                            {
                                char *subjectid = NULL;
                                cborFindResult = cbor_value_dup_text_string(&credMap, &subjectid, &len, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subjectid Value.");
                                if(strcmp(subjectid, WILDCARD_RESOURCE_URI) == 0)
                                {
                                    cred->subject.id[0] = '*';
                                }
                                else
                                {
                                    ret = ConvertStrToUuid(subjectid, &cred->subject);
                                    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                                }
                                OICFree(subjectid);
                            }
                            // credtype
                            if (strcmp(name, OIC_JSON_CREDTYPE_NAME)  == 0)
                            {
#ifdef __TIZENRT__
                                cborFindResult = cbor_value_get_int(&credMap, (int *) &cred->credType);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CredType.");
#else
                                uint64_t credType = 0;
                                cborFindResult = cbor_value_get_uint64(&credMap, &credType);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CredType.");
                                cred->credType = (OicSecCredType_t)credType;
#endif
                            }
                            // privatedata
                            if (strcmp(name, OIC_JSON_PRIVATEDATA_NAME)  == 0)
                            {
                                cborFindResult = DeserializeEncodingFromCbor(&credMap, &cred->privateData);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to read privateData structure");

                                OicEncodingType_t encoding = cred->privateData.encoding;
                                if (OIC_ENCODING_DER == encoding || OIC_ENCODING_PEM == encoding)
                                {
                                    //For unit test
                                    cred->privateData.encoding = OIC_ENCODING_RAW;
                                    OIC_LOG(WARNING, TAG, "Unknown encoding type detected for private data.");
                                }
                            }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                            //PublicData -- Not Mandatory
                            if (strcmp(name, OIC_JSON_PUBLICDATA_NAME)  == 0)
                            {
                                cborFindResult = DeserializeEncodingFromCbor(&credMap, &cred->publicData);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to read publicData structure");
                            }
                            //OptionalData -- Not Mandatory
                            if (strcmp(name, OIC_JSON_OPTDATA_NAME)  == 0)
                            {
                                cborFindResult = DeserializeSecOptFromCbor(&credMap, &cred->optionalData);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to read optionalData structure");
                            }
                            //Credusage -- Not Mandatory
                            if (0 == strcmp(OIC_JSON_CREDUSAGE_NAME, name))
                            {
                                cborFindResult = cbor_value_dup_text_string(&credMap, &cred->credUsage, &len, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Period.");
                            }
#endif  //__WITH_DTLS__ ||  __WITH_TLS__

                            if (0 == strcmp(OIC_JSON_PERIOD_NAME, name))
                            {
                                cborFindResult = cbor_value_dup_text_string(&credMap, &cred->period, &len, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Period.");
                            }

#ifdef MULTIPLE_OWNER
                            // Eowner uuid -- Not Mandatory
                            if (strcmp(OIC_JSON_EOWNERID_NAME, name)  == 0 && cbor_value_is_text_string(&credMap))
                            {
                                char *eowner = NULL;
                                cborFindResult = cbor_value_dup_text_string(&credMap, &eowner, &len, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding eownerId Value.");
                                if(NULL == cred->eownerID)
                                {
                                    cred->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
                                    VERIFY_NON_NULL(TAG, cred->eownerID, ERROR);
                                }
                                ret = ConvertStrToUuid(eowner, cred->eownerID);
                                OICFree(eowner);
                                VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
                            }
#endif //MULTIPLE_OWNER

                            if (cbor_value_is_valid(&credMap))
                            {
                                cborFindResult = cbor_value_advance(&credMap);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing CRED Map.");
                            }
                            OICFree(name);
                        }
                    }
                    cred->next = NULL;
                    if (cbor_value_is_valid(&credArray))
                    {
                        cborFindResult = cbor_value_advance(&credArray);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing CRED Array.");
                    }
                }
            }

            //ROwner -- Mandatory
            if (strcmp(tagName, OIC_JSON_ROWNERID_NAME)  == 0 && cbor_value_is_text_string(&CredRootMap))
            {
                char *stRowner = NULL;
                cborFindResult = cbor_value_dup_text_string(&CredRootMap, &stRowner, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Rownerid Value.");

                ret = ConvertStrToUuid(stRowner, &headCred->rownerID);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                OICFree(stRowner);
            }
            else if (NULL != gCred)
            {
                memcpy(&(headCred->rownerID), &(gCred->rownerID), sizeof(OicUuid_t));
            }
            OICFree(tagName);
        }
        if (cbor_value_is_valid(&CredRootMap))
        {
            cborFindResult = cbor_value_advance(&CredRootMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing CRED Root Map.");
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

    return ret;
}

#ifdef MULTIPLE_OWNER
bool IsValidCredentialAccessForSubOwner(const OicUuid_t* uuid, const uint8_t *cborPayload, size_t size)
{
    OicSecCred_t* cred = NULL;
    bool isValidCred = false;

    OIC_LOG_BUFFER(DEBUG, TAG, cborPayload, size);

    VERIFY_NON_NULL(TAG, uuid, ERROR);
    VERIFY_NON_NULL(TAG, cborPayload, ERROR);
    VERIFY_SUCCESS(TAG, 0 != size, ERROR);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == CBORPayloadToCred(cborPayload, size, &cred), ERROR);
    VERIFY_NON_NULL(TAG, cred, ERROR);
    VERIFY_NON_NULL(TAG, cred->eownerID, ERROR);
    VERIFY_SUCCESS(TAG, (memcmp(cred->eownerID->id, uuid->id, sizeof(uuid->id)) == 0), ERROR);

    isValidCred = true;

exit:
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
    VERIFY_NON_NULL(TAG, cred, ERROR);

    //CredId is assigned before appending new cred to the existing
    //credential list and updating svr database in AddCredential().
    cred->credId = 0;

    VERIFY_NON_NULL(TAG, subject, ERROR);
    memcpy(cred->subject.id, subject->id , sizeof(cred->subject.id));

    VERIFY_SUCCESS(TAG, credType < (NO_SECURITY_MODE | SYMMETRIC_PAIR_WISE_KEY |
            SYMMETRIC_GROUP_KEY | ASYMMETRIC_KEY | SIGNED_ASYMMETRIC_KEY | PIN_PASSWORD), ERROR);
    cred->credType = credType;

#ifdef __WITH_DTLS__
    if (publicData && publicData->data)
    {
        cred->publicData.data = (uint8_t *)OICCalloc(1, publicData->len);
        VERIFY_NON_NULL(TAG, cred->publicData.data, ERROR);
        memcpy(cred->publicData.data, publicData->data, publicData->len);
        cred->publicData.len = publicData->len;
    }
#endif // __WITH_DTLS__

    if (privateData && privateData->data)
    {
        cred->privateData.data = (uint8_t *)OICCalloc(1, privateData->len);
        VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);
        memcpy(cred->privateData.data, privateData->data, privateData->len);
        cred->privateData.len = privateData->len;
        cred->privateData.encoding = OIC_ENCODING_RAW;
    }

    VERIFY_NON_NULL(TAG, rownerID, ERROR);
    memcpy(&cred->rownerID, rownerID, sizeof(OicUuid_t));

#ifdef MULTIPLE_OWNER
    if(eownerID)
    {
        cred->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
        VERIFY_NON_NULL(TAG, cred->eownerID, ERROR);
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
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : privateData len: %d", cred->privateData.len);
        OIC_LOG_BUFFER(DEBUG, TAG, cred->privateData.data, cred->privateData.len);
    }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
    if(cred->credUsage)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : credUsage: %s", cred->credUsage);
    }
    if (cred->publicData.data)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : publicData len: %d", cred->publicData.len);
        OIC_LOG_BUFFER(DEBUG, TAG, cred->publicData.data, cred->publicData.len);

    }
    if (cred->optionalData.data)
    {
        OIC_LOG_V(DEBUG, TAG, "GenerateCredential : optionalData len: %d", cred->optionalData.len);
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
        if ((OC_STACK_OK == res) && payload)
        {
            if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_CRED_NAME, payload, size))
            {
                ret = true;
            }
            OICClearMemory(payload, size);
            OICFree(payload);
        }
    }
    else //Empty cred list
    {
        if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_CRED_NAME, NULL, 0))
        {
            ret = true;
        }
    }
    OIC_LOG(DEBUG, TAG, "OUT Cred UpdatePersistentStorage");
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
    LL_SORT(gCred, CmpCredId);

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

static bool IsSameSecOpt(const OicSecOpt_t* sk1, const OicSecOpt_t* sk2)
{
    VERIFY_NON_NULL(TAG, sk1, WARNING);
    VERIFY_NON_NULL(TAG, sk2, WARNING);

    VERIFY_SUCCESS(TAG, (sk1->len == sk2->len), INFO);
    VERIFY_SUCCESS(TAG, (sk1->encoding == sk2->encoding), INFO);
    VERIFY_SUCCESS(TAG, (0 == memcmp(sk1->data, sk2->data, sk1->len)), INFO);
    return true;
exit:
    return false;
}

static bool IsSameSecKey(const OicSecKey_t* sk1, const OicSecKey_t* sk2)
{
    VERIFY_NON_NULL(TAG, sk1, WARNING);
    VERIFY_NON_NULL(TAG, sk2, WARNING);

    VERIFY_SUCCESS(TAG, (sk1->len == sk2->len), INFO);
    VERIFY_SUCCESS(TAG, (sk1->encoding == sk2->encoding), INFO);
    VERIFY_SUCCESS(TAG, (0 == memcmp(sk1->data, sk2->data, sk1->len)), INFO);
    return true;
exit:
    return false;
}

/**
 * Compares credential
 *
 * @return CRED_CMP_EQUAL if credentials are equal
 *         CRED_CMP_NOT_EQUAL if not equal
 *         otherwise error.
 */

static CredCompareResult_t CompareCredential(const OicSecCred_t * l, const OicSecCred_t * r)
{
    CredCompareResult_t cmpResult = CRED_CMP_ERROR;
    bool isCompared = false;
    OIC_LOG(DEBUG, TAG, "IN CompareCredetial");

    VERIFY_NON_NULL(TAG, l, ERROR);
    VERIFY_NON_NULL(TAG, r, ERROR);

    cmpResult = CRED_CMP_NOT_EQUAL;

    VERIFY_SUCCESS(TAG, (l->credType == r->credType), INFO);
    VERIFY_SUCCESS(TAG, (0 == memcmp(l->subject.id, r->subject.id, sizeof(l->subject.id))), INFO);

    switch(l->credType)
    {
        case SYMMETRIC_PAIR_WISE_KEY:
        case SYMMETRIC_GROUP_KEY:
        case PIN_PASSWORD:
        {
            if(l->privateData.data && r->privateData.data)
            {
                VERIFY_SUCCESS(TAG, IsSameSecKey(&l->privateData, &r->privateData), INFO);
                isCompared = true;
            }
            break;
        }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        case ASYMMETRIC_KEY:
        case SIGNED_ASYMMETRIC_KEY:
        {
            if(l->publicData.data && r->publicData.data)
            {
                VERIFY_SUCCESS(TAG, IsSameSecKey(&l->publicData, &r->publicData), INFO);
                isCompared = true;
            }

            if(l->optionalData.data && r->optionalData.data)
            {
                VERIFY_SUCCESS(TAG, IsSameSecOpt(&l->optionalData, &r->optionalData), INFO);
                isCompared = true;
            }

            if(l->credUsage && r->credUsage)
            {
                VERIFY_SUCCESS(TAG, (strlen(l->credUsage) == strlen(r->credUsage)), INFO);
                VERIFY_SUCCESS(TAG, (0 == strncmp(l->credUsage, r->credUsage, strlen(l->credUsage))), INFO);
                isCompared = true;
            }
            break;
        }
        case ASYMMETRIC_ENCRYPTION_KEY:
        {
            if(l->privateData.data && r->privateData.data)
            {
                VERIFY_SUCCESS(TAG, IsSameSecKey(&l->privateData, &r->privateData), INFO);
                isCompared = true;
            }

            if(l->publicData.data && r->publicData.data)
            {
                VERIFY_SUCCESS(TAG, IsSameSecKey(&l->publicData, &r->publicData), INFO);
                isCompared = true;
            }

            if(l->optionalData.data && r->optionalData.data)
            {
                VERIFY_SUCCESS(TAG, IsSameSecOpt(&l->optionalData, &r->optionalData), INFO);
                isCompared = true;
            }

            break;
        }
#endif //__WITH_DTLS__ or __WITH_TLS__
        default:
        {
            cmpResult = CRED_CMP_ERROR;
            break;
        }
    }

    if(isCompared)
    {
        OIC_LOG(DEBUG, TAG, "Same Credentials");
        cmpResult = CRED_CMP_EQUAL;
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Can not find the key data in credential");
        cmpResult = CRED_CMP_ERROR;
    }
exit:
    OIC_LOG(DEBUG, TAG, "OUT CompareCredetial");

    return cmpResult;
}

OCStackResult AddCredential(OicSecCred_t * newCred)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicSecCred_t * temp = NULL;
    bool validFlag = true;
    OicUuid_t emptyOwner = { .id = {0} };

    OIC_LOG(DEBUG, TAG, "IN AddCredential");

    VERIFY_SUCCESS(TAG, NULL != newCred, ERROR);
    //Assigning credId to the newCred
    newCred->credId = GetCredId();
    VERIFY_SUCCESS(TAG, true == IsValidCredential(newCred), ERROR);

    //the newCred is not valid if it is empty

    if (memcmp(&(newCred->subject), &emptyOwner, sizeof(OicUuid_t)) == 0)
    {
        validFlag = false;
    }
    else
    {
        LL_FOREACH(gCred, temp)
        {
            CredCompareResult_t cmpRes = CompareCredential(temp, newCred);
            if(CRED_CMP_EQUAL == cmpRes)
            {
                OIC_LOG_V(WARNING, TAG, "Detected same credential ID(%d)" \
                          "new credential's ID will be replaced.", temp->credId);
                newCred->credId = temp->credId;
                ret = OC_STACK_OK;
                validFlag = false;
                break;
            }

            if (CRED_CMP_ERROR == cmpRes)
            {
                OIC_LOG_V(WARNING, TAG, "Credential skipped : %d", cmpRes);
                ret = OC_STACK_ERROR;
                validFlag = false;
                break;
            }
        }
    }

    //Append the new Cred to existing list if new Cred is valid
    if (validFlag)
    {
        LL_APPEND(gCred, newCred);
    }
    if (memcmp(&(newCred->rownerID), &emptyOwner, sizeof(OicUuid_t)) != 0)
    {
        memcpy(&(gCred->rownerID), &(newCred->rownerID), sizeof(OicUuid_t));
    }
    if (UpdatePersistentStorage(gCred))
    {
        ret = OC_STACK_OK;
    }

exit:
    OIC_LOG(DEBUG, TAG, "OUT AddCredential");
    return ret;
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
 * Remove all credential data on credential resource and persistent storage
 *
 * @retval
 *     OC_STACK_OK              - no errors
 *     OC_STACK_ERROR           - stack process error
 */
OCStackResult RemoveAllCredentials(void)
{
    DeleteCredList(gCred);
    gCred = GetCredDefault();

    if (!UpdatePersistentStorage(gCred))
    {
        return OC_STACK_ERROR;
    }
    return OC_STACK_OK;
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
    VERIFY_NON_NULL(TAG, oxmLabel, ERROR);

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
        VERIFY_NON_NULL(TAG, receviedCred->privateData.data, ERROR);
        receviedCred->privateData.len = OWNER_PSK_LENGTH_128;
        memcpy(receviedCred->privateData.data, ownerPSK, OWNER_PSK_LENGTH_128);
    }
    else if(OIC_ENCODING_BASE64 == receviedCred->privateData.encoding)
    {
        B64Result b64res = B64_OK;
        uint32_t b64OutSize = 0;
        b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        b64Buf = OICCalloc(1, b64BufSize);
        VERIFY_NON_NULL(TAG, b64Buf, ERROR);

        b64res = b64Encode(ownerPSK, OWNER_PSK_LENGTH_128, b64Buf, b64BufSize, &b64OutSize);
        VERIFY_SUCCESS(TAG, B64_OK == b64res, ERROR);
#ifndef __TIZENRT__
        receviedCred->privateData.data = (uint8_t *)OICCalloc(1, b64OutSize + 1);
#else
        receviedCred->privateData.data = (uint8_t *)OICRealloc(receviedCred->privateData.data, b64OutSize + 1);
#endif
        VERIFY_NON_NULL(TAG, receviedCred->privateData.data, ERROR);
        receviedCred->privateData.len = b64OutSize;
        strncpy((char*)receviedCred->privateData.data, b64Buf, b64OutSize);
        receviedCred->privateData.data[b64OutSize] = '\0';
        OICClearMemory(b64Buf, b64BufSize);
        OICFree(b64Buf);
        b64Buf = NULL;
    }
    else
    {
        VERIFY_SUCCESS(TAG, OIC_ENCODING_UNKNOW, ERROR);
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
    VERIFY_NON_NULL(TAG, oxmLabel, ERROR);

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
        VERIFY_NON_NULL(TAG, receivedCred->privateData.data, ERROR);
        receivedCred->privateData.len = OWNER_PSK_LENGTH_128;
        memcpy(receivedCred->privateData.data, subOwnerPSK, OWNER_PSK_LENGTH_128);
    }
    else if(OIC_ENCODING_BASE64 == receivedCred->privateData.encoding)
    {
        uint32_t b64OutSize = 0;
        size_t b64BufSize = B64ENCODE_OUT_SAFESIZE((OWNER_PSK_LENGTH_128 + 1));
        b64Buf = OICCalloc(1, b64BufSize);
        VERIFY_NON_NULL(TAG, b64Buf, ERROR);

        VERIFY_SUCCESS(TAG, \
                       B64_OK == b64Encode(subOwnerPSK, OWNER_PSK_LENGTH_128, b64Buf, b64BufSize, &b64OutSize), \
                       ERROR);

        receivedCred->privateData.data = (uint8_t *)OICCalloc(1, b64OutSize + 1);
        VERIFY_NON_NULL(TAG, receivedCred->privateData.data, ERROR);
        receivedCred->privateData.len = b64OutSize;
        strncpy((char*)receivedCred->privateData.data, b64Buf, b64OutSize);
        receivedCred->privateData.data[b64OutSize] = '\0';
    }
    else
    {
        OIC_LOG(INFO, TAG, "Unknown credential encoding type.");
        VERIFY_SUCCESS(TAG, OIC_ENCODING_UNKNOW, ERROR);
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


OCStackResult AddPreconfPinCredential(const char* preconfPin)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

#ifdef MULTIPLE_OWNER
    OCStackResult res = OC_STACK_INVALID_PARAM;
    OicSecCred_t* cred = NULL;
    OicSecCred_t* pinCred = NULL;
    VERIFY_NON_NULL(TAG, preconfPin, ERROR);
    VERIFY_SUCCESS(TAG, (strlen(preconfPin) >= PRECONF_PIN_MIN_SIZE), ERROR);

    OIC_LOG(DEBUG, TAG, "Finding previous preconfigured PIN...");
    //Find the previous PIN
    LL_FOREACH(gCred, cred)
    {
        if(memcmp(cred->subject.id, WILDCARD_SUBJECT_ID.id, sizeof(cred->subject.id)) == 0 &&
            PIN_PASSWORD == cred->credType)
        {
            break;
        }
    }

    //If previous PIN is exist, remove it.
    if (cred)
    {
        OIC_LOG_V(DEBUG, TAG, "Preconfigured PIN already exist.");
        OIC_LOG_V(DEBUG, TAG, "Previous preconfigured PIN will be removed.");

        res = RemoveCredentialByCredId(cred->credId);
        if (OC_STACK_RESOURCE_DELETED != res)
        {
            OIC_LOG_V(ERROR, TAG, "RemoveCredentialByCredId error : %d", res);
            cred = NULL;
            goto exit;
        }
    }

    OIC_LOG(DEBUG, TAG, "Adding preconfigured PIN...");
    //Add preconfig PIN
    res = OC_STACK_NO_MEMORY;
    //Generate PIN based credential
    size_t preconfPinLen = strlen(preconfPin);
    pinCred = (OicSecCred_t*)OICCalloc(1, sizeof(OicSecCred_t));
    VERIFY_NON_NULL(TAG, pinCred, ERROR);

    pinCred->privateData.data = (uint8_t*)OICMalloc(preconfPinLen + 1);
    VERIFY_NON_NULL(TAG, pinCred->privateData.data, ERROR);

    memcpy(pinCred->privateData.data, preconfPin, preconfPinLen);
    pinCred->privateData.data[preconfPinLen] = '\0';
    pinCred->privateData.len = preconfPinLen;
    pinCred->privateData.encoding = OIC_ENCODING_RAW;
    pinCred->credType = PIN_PASSWORD;
    memcpy(pinCred->subject.id, WILDCARD_SUBJECT_ID.id, sizeof(pinCred->subject.id));

    res = AddCredential(pinCred);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "AddCredential error : %d", res);
        goto exit;
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_OK;
exit:
    if (cred)
    {
        FreeCred(cred);
    }
    if (pinCred)
    {
        FreeCred(pinCred);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return res;
#else
    OIC_LOG(DEBUG, TAG, "Multiple Owner is not enabled.");
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_ERROR;
#endif //MULTIPLE_OWNER
}


#endif // __WITH_DTLS__ or __WITH_TLS__

static OCEntityHandlerResult HandlePostRequest(OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ret = OC_EH_ERROR;
    OIC_LOG(DEBUG, TAG, "HandleCREDPostRequest IN");

    static uint16_t previousMsgId = 0;
    //Get binary representation of cbor
    OicSecCred_t *cred  = NULL;
    uint8_t *payload = (((OCSecurityPayload*)ehRequest->payload)->securityData);
    size_t size = (((OCSecurityPayload*)ehRequest->payload)->payloadSize);

    OCStackResult res = CBORPayloadToCred(payload, size, &cred);
    if (res == OC_STACK_OK)
    {
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        OicUuid_t emptyUuid = {.id={0}};
        const OicSecDoxm_t* doxm = GetDoxmResourceData();
        if(doxm && false == doxm->owned && memcmp(&(doxm->owner), &emptyUuid, sizeof(OicUuid_t)) != 0)
        {
            //in case of owner PSK
            switch(cred->credType)
            {
                case SYMMETRIC_PAIR_WISE_KEY:
                {
                    OCServerRequest *request = GetServerRequestUsingHandle(ehRequest->requestHandle);
                    if (NULL == request)
                    {
                        OIC_LOG(ERROR, TAG, "Failed to get a server request information.");
                        ret = OC_EH_ERROR;
                        break;
                    }
#ifdef __TIZENRT__
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

                        if (CA_STATUS_OK != CASelectCipherSuite(
                                    MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, CA_ADAPTER_IP))
                        {
                            OIC_LOG(ERROR, TAG, "Failed to enable PSK cipher suite");
                            ret = OC_EH_ERROR;
                        }
                        else
                        {
                            OIC_LOG(INFO, TAG, "PSK cipher suite is ENABLED");
                        }

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
                const OicSecDoxm_t* doxm =  GetDoxmResourceData();
                if(doxm)
                {
                    if(!doxm->owned)
                    {
                        OIC_LOG(WARNING, TAG, "The operation failed during handle DOXM request");

                        if((OC_ADAPTER_IP == ehRequest->devAddr.adapter && previousMsgId != ehRequest->messageID)
                           || OC_ADAPTER_TCP == ehRequest->devAddr.adapter)
                        {
#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
                            InvokeOtmEventHandler(ehRequest->devAddr.addr, ehRequest->devAddr.port,
                                                  NULL, OIC_OTM_ERROR);
#endif
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
                    OCServerRequest *request = GetServerRequestUsingHandle(ehRequest->requestHandle);
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
                OicUuid_t emptyUuid = {.id={0}};
                if(memcmp(cred->rownerID.id, emptyUuid.id, sizeof(emptyUuid.id)) != 0)
                {
                    OIC_LOG(INFO, TAG, "CRED's rowner will be updated.");
                    memcpy(gCred->rownerID.id, cred->rownerID.id, sizeof(cred->rownerID.id));
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
#endif//__WITH_DTLS__
    }

    if (OC_EH_CHANGED != ret)
    {
        if(OC_STACK_OK != RemoveCredential(&cred->subject))
        {
            OIC_LOG(WARNING, TAG, "Failed to remove the invalid credential");
        }
        FreeCred(cred);
    }
    else
    {
        if(OC_ADAPTER_IP == ehRequest->devAddr.adapter)
        {
            previousMsgId = ehRequest->messageID++;
        }
    }
    //Send response to request originator
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

    if (NULL == ehRequest->query)
    {
        return ehRet;
    }

    OicParseQueryIter_t parseIter = { .attrPos=NULL };
    OicUuid_t subject = {.id={0}};

    //Parsing REST query to get the subject
    ParseQueryIterInit((unsigned char *)ehRequest->query, &parseIter);
    while (GetNextQuery(&parseIter))
    {
        if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_SUBJECTID_NAME,
                parseIter.attrLen) == 0)
        {
            OCStackResult ret = ConvertStrToUuid((const char*)parseIter.valPos, &subject);
            VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        }
    }

    if (OC_STACK_RESOURCE_DELETED == RemoveCredential(&subject))
    {
        ehRet = OC_EH_RESOURCE_DELETED;
    }
    //Send response to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;
exit:
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
                ret = HandleGetRequest(ehRequest);;
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
                                         OC_SECURE);

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

    //Read Cred resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_CRED_NAME, &data, &size);
    // If database read failed
    if (ret != OC_STACK_OK)
    {
        OIC_LOG (DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
        // Read Cred resource from PS
        ret = CBORPayloadToCred(data, size, &gCred);
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

        if (0 == memcmp(&gCred->rownerID, &emptyUuid, sizeof(OicUuid_t)))
        {
            memcpy(&gCred->rownerID, &deviceID, sizeof(OicUuid_t));
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
            VERIFY_NON_NULL(TAG, cred, ERROR);

            // common
            cred->next = NULL;
            cred->credId = tmpCred->credId;
            cred->credType = tmpCred->credType;
            memcpy(cred->subject.id, tmpCred->subject.id , sizeof(cred->subject.id));
            memcpy(cred->rownerID.id, tmpCred->rownerID.id , sizeof(cred->rownerID.id));
            if (tmpCred->period)
            {
                cred->period = OICStrdup(tmpCred->period);
            }

            // key data
            if (tmpCred->privateData.data)
            {
                cred->privateData.data = (uint8_t *)OICCalloc(1, tmpCred->privateData.len);
                VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);

                memcpy(cred->privateData.data, tmpCred->privateData.data, tmpCred->privateData.len);
                cred->privateData.len = tmpCred->privateData.len;
                cred->privateData.encoding = tmpCred->privateData.encoding;
            }
#if defined(__WITH_X509__) || defined(__WITH_TLS__)
            else if (tmpCred->publicData.data)
            {
                cred->publicData.data = (uint8_t *)OICCalloc(1, tmpCred->publicData.len);
                VERIFY_NON_NULL(TAG, cred->publicData.data, ERROR);

                memcpy(cred->publicData.data, tmpCred->publicData.data, tmpCred->publicData.len);
                cred->publicData.len = tmpCred->publicData.len;
            }
            else if (tmpCred->optionalData.data)
            {
                cred->optionalData.data = (uint8_t *)OICCalloc(1, tmpCred->optionalData.len);
                VERIFY_NON_NULL(TAG, cred->optionalData.data, ERROR);

                memcpy(cred->optionalData.data, tmpCred->optionalData.data, tmpCred->optionalData.len);
                cred->optionalData.len = tmpCred->optionalData.len;
                cred->optionalData.encoding = tmpCred->optionalData.encoding;
                cred->optionalData.revstat= tmpCred->optionalData.revstat;
            }

            if (tmpCred->credUsage)
            {
                cred->credUsage = OICStrdup(tmpCred->credUsage);
            }
#endif /* __WITH_X509__  or __WITH_TLS__*/

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
                            ret = cred->privateData.len;
                            memcpy(result, cred->privateData.data, ret);
                        }
                        else if(OIC_ENCODING_BASE64 == cred->privateData.encoding)
                        {
                            size_t outBufSize = B64DECODE_OUT_SAFESIZE((cred->privateData.len + 1));
                            uint8_t* outKey = OICCalloc(1, outBufSize);
                            uint32_t outKeySize;
                            if(NULL == outKey)
                            {
                                OIC_LOG (ERROR, TAG, "Failed to allocate memory.");
                                return ret;
                            }

                            if(B64_OK == b64Decode((char*)cred->privateData.data, cred->privateData.len, outKey, outBufSize, &outKeySize))
                            {
                                memcpy(result, outKey, outKeySize);
                                ret = outKeySize;
                            }
                            else
                            {
                                OIC_LOG (ERROR, TAG, "Failed to base64 decoding.");
                            }

                            OICFree(outKey);
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
                                uint32_t pinLength = 0;
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

                                    if(B64_OK != b64Decode((char*)wildCardCred->privateData.data, wildCardCred->privateData.len, pinBuffer, pinBufSize, &pinLength))
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
    OicSecKey_t privKey = {privData, OWNER_PSK_LENGTH_128, OIC_ENCODING_RAW};
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
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    int secureFlag = 0;
    OicUuid_t prevId = {.id={0}};

    if(NULL == newROwner)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    if(NULL == gCred)
    {
        ret = OC_STACK_NO_RESOURCE;
    }

    if(newROwner && gCred)
    {
        memcpy(prevId.id, gCred->rownerID.id, sizeof(prevId.id));
        memcpy(gCred->rownerID.id, newROwner->id, sizeof(newROwner->id));

        // This added '256' is arbitrary value that is added to cover the name of the resource, map addition and ending
        size = GetCredKeyDataSize(gCred);
        size += (256 * OicSecCredCount(gCred));
        ret = CredToCBORPayload(gCred, &cborPayload, &size, secureFlag);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_CRED_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    memcpy(gCred->rownerID.id, prevId.id, sizeof(prevId.id));
    return ret;
}

OCStackResult GetCredRownerId(OicUuid_t *rowneruuid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gCred)
    {
        *rowneruuid = gCred->rownerID;
        retVal = OC_STACK_OK;
    }
    return retVal;
}

#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
void GetDerCaCert(ByteArray_t * crt, const char * usage)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == crt || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    crt->len = 0;
    OicSecCred_t* temp = NULL;

    LL_FOREACH(gCred, temp)
    {
        if ((SIGNED_ASYMMETRIC_KEY == temp->credType) &&
            (0 == strcmp(temp->credUsage, usage)) && (false == temp->optionalData.revstat))
        {
            if(OIC_ENCODING_BASE64 == temp->optionalData.encoding)
            {
                size_t bufSize = B64DECODE_OUT_SAFESIZE((temp->optionalData.len + 1));
                uint8_t * buf = OICCalloc(1, bufSize);
                if(NULL == buf)
                {
                    OIC_LOG(ERROR, TAG, "Failed to allocate memory");
                    return;
                }
                uint32_t outSize;
                if(B64_OK != b64Decode((char*)(temp->optionalData.data),
                                       temp->optionalData.len, buf, bufSize, &outSize))
                {
                    OICFree(buf);
                    OIC_LOG(ERROR, TAG, "Failed to decode base64 data");
                    return;
                }
                crt->data = OICRealloc(crt->data, crt->len + outSize);
                memcpy(crt->data + crt->len, buf, outSize);
                crt->len += outSize;
                OICFree(buf);
            }
            else
            {
                crt->data = OICRealloc(crt->data, crt->len + temp->optionalData.len);
                memcpy(crt->data + crt->len, temp->optionalData.data, temp->optionalData.len);
                crt->len += temp->optionalData.len;
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

void GetDerOwnCert(ByteArray_t * crt, const char * usage)
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
            0 == strcmp(temp->credUsage, usage))
        {
            crt->data = OICRealloc(crt->data, crt->len + temp->publicData.len);
            memcpy(crt->data + crt->len, temp->publicData.data, temp->publicData.len);
            crt->len += temp->publicData.len;
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
        if (SIGNED_ASYMMETRIC_KEY == temp->credType &&
            0 == strcmp(temp->credUsage, usage))
        {
            key->data = OICRealloc(key->data, key->len + temp->privateData.len);
            memcpy(key->data + key->len, temp->privateData.data, temp->privateData.len);
            key->len += temp->privateData.len;
            OIC_LOG_V(DEBUG, TAG, "Key for %s found", usage);
        }
    }
    if(0 == key->len)
    {
        OIC_LOG_V(WARNING, TAG, "Key for %s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

void InitCipherSuiteListInternal(bool * list, const char * usage)
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
                list[0] = true;
                OIC_LOG(DEBUG, TAG, "SYMMETRIC_PAIR_WISE_KEY found");
                break;
            }
            case SIGNED_ASYMMETRIC_KEY:
            {
                if (0 == strcmp(temp->credUsage, usage))
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


//Added as workaround by Chul Lee
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
OCStackResult CredSaveTrustCertChain(const OicUuid_t* subject, uint8_t *trustCertChain, size_t chainSize,
                                            OicEncodingType_t encodingType,  const char* usage, uint16_t *credId)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    if(NULL == trustCertChain || NULL == credId || NULL == usage || NULL == subject)
    {
        OIC_LOG_V(ERROR, TAG, "Invaild param");
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult res = OC_STACK_ERROR;
    OicSecCred_t *cred = (OicSecCred_t*)OICCalloc(1, sizeof(OicSecCred_t));
    if(NULL == cred)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    cred->credId = 0;
    memcpy(cred->subject.id, subject->id, sizeof(cred->subject.id));

    cred->credUsage = OICStrdup(usage);
    if (NULL == cred->credUsage)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    cred->credType = SIGNED_ASYMMETRIC_KEY;

    if (encodingType == OIC_ENCODING_PEM)
    {
        cred->optionalData.data = (uint8_t *)OICCalloc(1, chainSize + 1);
        if(NULL == cred->optionalData.data)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
            res = OC_STACK_NO_MEMORY;
            goto error;
        }
        cred->optionalData.len = chainSize + 1;
    }
    else
    {
        cred->optionalData.data = (uint8_t *)OICCalloc(1, chainSize);
        if(NULL == cred->optionalData.data)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
            res = OC_STACK_NO_MEMORY;
            goto error;
        }
        cred->optionalData.len = chainSize;
    }
    memcpy(cred->optionalData.data, trustCertChain, chainSize);
    cred->optionalData.encoding = encodingType;
    cred->optionalData.revstat = false;

    res = AddCredential(cred);
    if(res != OC_STACK_OK)
    {
        goto error;
    }
    *credId = cred->credId;

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return res;

error:
    DeleteCredList(cred);
    OIC_LOG_V(ERROR, TAG, "OUT %s : error = %d", __func__, (int)res);
    return res;
}

OCStackResult CredSaveOwnCert(const OicUuid_t* subject, OicSecKey_t * cert, OicSecKey_t * key,
                                    const char* usage, uint16_t *credId)
{
    OIC_LOG_V(DEBUG, TAG, "IN %s", __func__);

    if(NULL == cert || NULL == cert->data || NULL == key || NULL == key->data ||
       NULL == credId || NULL == usage || NULL == subject)
    {
        OIC_LOG_V(ERROR, TAG, "Invalid param");
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult res = OC_STACK_ERROR;
    OicSecCred_t *cred = (OicSecCred_t *)OICCalloc(1, sizeof(OicSecCred_t));
    if(NULL == cred)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    cred->credId = 0;
    memcpy(cred->subject.id, subject->id, sizeof(cred->subject.id));

    cred->credUsage = OICStrdup(usage);
    if (NULL == cred->credUsage)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }

    cred->credType = SIGNED_ASYMMETRIC_KEY;

    OicSecKey_t *publicData = &cred->publicData;
    publicData->data = (uint8_t *)OICCalloc(1, cert->len);
    if(NULL == publicData->data)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    memcpy(publicData->data, cert->data, cert->len);
    publicData->encoding = cert->encoding;
    publicData->len = cert->len;

    OicSecKey_t *privateData = &cred->privateData;
    privateData->data = (uint8_t *)OICCalloc(1, key->len);
    if(NULL == privateData->data)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to allocate memory");
        res = OC_STACK_NO_MEMORY;
        goto error;
    }
    memcpy(privateData->data, key->data, key->len);
    privateData->len = key->len;
    privateData->encoding = key->encoding;

    res = AddCredential(cred);
    if(res != OC_STACK_OK)
    {
        goto error;
    }
    *credId = cred->credId;

    OIC_LOG_V(DEBUG, TAG, "OUT %s", __func__);

    return res;

error:
    DeleteCredList(cred);
    OIC_LOG_V(ERROR, TAG, "OUT %s : error = %d", __func__, (int)res);
    return res;
}

#endif defined(__WITH_DTLS__) || defined(__WITH_TLS__)
