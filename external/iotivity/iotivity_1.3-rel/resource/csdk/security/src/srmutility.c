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
#define _POSIX_C_SOURCE 200112L
#include <string.h>

#include "srmutility.h"
#include "srmresourcestrings.h"
#include "logger.h"
#include "oic_malloc.h"
#include "base64.h"
#include "ocrandom.h"
#include "doxmresource.h"

#define TAG  "OIC_SRM_UTILITY"

void ParseQueryIterInit(const unsigned char * query, OicParseQueryIter_t * parseIter)
{
    OIC_LOG(INFO, TAG, "Initializing coap iterator");
    if ((NULL == query) || (NULL == parseIter))
    {
        return;
    }

    parseIter->attrPos = NULL;
    parseIter->attrLen = 0;
    parseIter->valPos = NULL;
    parseIter->valLen = 0;
    coap_parse_iterator_init((unsigned char *)query, strlen((char *)query),
                             (unsigned char *)OIC_SEC_REST_QUERY_SEPARATOR,
                             (unsigned char *) "", 0, &parseIter->pi);
}

OicParseQueryIter_t * GetNextQuery(OicParseQueryIter_t * parseIter)
{
    OIC_LOG(INFO, TAG, "Getting Next Query");
    if (NULL == parseIter)
    {
        return NULL;
    }

    unsigned char * qrySeg = NULL;
    char * delimPos;

    // Get the next query. Querys are separated by OIC_SEC_REST_QUERY_SEPARATOR.
    qrySeg = coap_parse_next(&parseIter->pi);

    if (qrySeg)
    {
        delimPos = strchr((char *)qrySeg, OIC_SEC_REST_QUERY_DELIMETER);
        if (delimPos)
        {
            parseIter->attrPos = parseIter->pi.pos;
            parseIter->attrLen = (unsigned char *)delimPos - parseIter->pi.pos;
            parseIter->valPos  = (unsigned char *)delimPos + 1;
            parseIter->valLen  = &qrySeg[parseIter->pi.segment_length] - parseIter->valPos;
            return parseIter;
        }
    }
    return NULL;
}

/**
 * Function to getting string of ownership transfer method
 *
 * @prarm oxmType ownership transfer method
 *
 * @return string value of ownership transfer method
 */
const char* GetOxmString(OicSecOxm_t oxmType)
{
    switch(oxmType)
    {
        case OIC_JUST_WORKS:
            return OXM_JUST_WORKS;
        case OIC_RANDOM_DEVICE_PIN:
            return OXM_RANDOM_DEVICE_PIN;
        case OIC_MANUFACTURER_CERTIFICATE:
            return OXM_MANUFACTURER_CERTIFICATE;
#ifdef MULTIPLE_OWNER
        case OIC_PRECONFIG_PIN:
            return OXM_PRECONF_PIN;
#endif //MULTIPLE_OWNER
        case OIC_MV_JUST_WORKS:
            return OXM_MV_JUST_WORKS;
        case OIC_CON_MFG_CERT:
            return OXM_CON_MFG_CERT;
        default:
            return NULL;
    }
}

OCStackResult ConvertUuidToStr(const OicUuid_t* uuid, char** strUuid)
{
    if(NULL == uuid || NULL == strUuid || NULL != *strUuid)
    {
        OIC_LOG(ERROR, TAG, "ConvertUuidToStr : Invalid param");
        return OC_STACK_INVALID_PARAM;
    }

    const size_t urnBufSize = (UUID_LENGTH * 2) + 4 + 1;
    char* convertedUrn = (char*)OICCalloc(urnBufSize, sizeof(char));
    VERIFY_NOT_NULL(TAG, convertedUrn, ERROR);
    if(OCConvertUuidToString(uuid->id,convertedUrn))
    {
        *strUuid = convertedUrn;
        return OC_STACK_OK;
    }

    return OC_STACK_INVALID_PARAM;

exit:
    return OC_STACK_NO_MEMORY;
}

OCStackResult OC_CALL ConvertStrToUuid(const char* strUuid, OicUuid_t* uuid)
{
    bool result = true;
    size_t strUuidLen = strlen(strUuid);

    if (0 == strUuidLen)
    {
        OIC_LOG(INFO, TAG, "Converting empty UUID string to 00000000-0000-0000-0000-000000000000");
        memset(uuid->id, 0, sizeof(uuid->id));
    }
    else
    {
        result = OCConvertStringToUuid(strUuid, uuid->id);
    }

    if (!result)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Invalid parameter '%s'", __func__, strUuid);
        return OC_STACK_INVALID_PARAM;
    }

    return OC_STACK_OK;
}

#ifndef NDEBUG
/**
 * Log OicUuid_t structs.
 */
void LogUuid(const OicUuid_t* uuid)
{
    if(NULL == uuid)
    {
        OIC_LOG(ERROR, TAG, "ConvertUuidToStr : Invalid param");
        return;
    }

    const size_t urnBufSize = (UUID_LENGTH * 2) + 4 + 1;
    char* convertedUrn = (char*)OICCalloc(urnBufSize, sizeof(char));
    VERIFY_NOT_NULL(TAG, convertedUrn, ERROR);
    if(OCConvertUuidToString(uuid->id,convertedUrn))
    {
        OIC_LOG_V(DEBUG, TAG, "uuid: %s", convertedUrn);
    }
    OICFree(convertedUrn);
exit:
    return;
}
#endif

/**
 * Compares two OicUuid_t structs.
 *
 * @return true if the two OicUuid_t structs are equal, else false.
 */
bool UuidCmp(const OicUuid_t *firstId, const OicUuid_t *secondId)
{
    bool ret = false;
    VERIFY_NOT_NULL(TAG, firstId, ERROR);
    VERIFY_NOT_NULL(TAG, secondId, ERROR);

    if (0 == memcmp(firstId, secondId, sizeof(OicUuid_t)))
    {
        ret = true;
    }

exit:
    OIC_LOG_V(DEBUG, TAG, "%s: returning %s.", __func__, ret?"true":"false");
    return ret;
}

const OicUuid_t THE_NIL_UUID = {.id={0000000000000000}};

/**
 * Compares OicUuid_t to Nil UUID {.id={0000000000000000}}
 *
 * @return true if the OicUuid_t is the Nil UUID.
 */
bool IsNilUuid(const OicUuid_t *uuid)
{
#if !defined(NDEBUG)
    char *strUuid = NULL;
    ConvertUuidToStr(uuid, &strUuid);
    if (strUuid)
    {
        OIC_LOG_V(DEBUG, TAG, "%s: uuid: %s.", __func__, strUuid);
        OICFree(strUuid);
    }
#endif
    return UuidCmp(uuid, &THE_NIL_UUID);
}

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
OCStackResult OC_CALL SetDeviceIdSeed(const uint8_t* seed, size_t seedSize)
{
    return SetDoxmDeviceIDSeed(seed, seedSize);
}
#endif
