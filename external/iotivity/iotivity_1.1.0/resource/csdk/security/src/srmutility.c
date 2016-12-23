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

#define TAG  "SRM-UTILITY"

void ParseQueryIterInit(const unsigned char *query, OicParseQueryIter_t *parseIter)
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

OicParseQueryIter_t *GetNextQuery(OicParseQueryIter_t *parseIter)
{
  OIC_LOG(INFO, TAG, "Getting Next Query");
  if (NULL == parseIter)
  {
    return NULL;
  }

  unsigned char *qrySeg = NULL;
  char *delimPos;

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

// TODO This functionality is replicated in all SVR's and therefore we need
// to encapsulate it in a common method. However, this may not be the right
// file for this method.
OCStackResult AddUuidArray(const cJSON *jsonRoot, const char *arrayItem,
                           size_t *numUuids, OicUuid_t **uuids)
{
  size_t idxx = 0;
  cJSON *jsonObj = cJSON_GetObjectItem((cJSON *)jsonRoot, arrayItem);
  VERIFY_NON_NULL(TAG, jsonObj, ERROR);
  VERIFY_SUCCESS(TAG, cJSON_Array == jsonObj->type, ERROR);

  *numUuids = (size_t)cJSON_GetArraySize(jsonObj);
  VERIFY_SUCCESS(TAG, *numUuids > 0, ERROR);
  *uuids = (OicUuid_t *)OICCalloc(*numUuids, sizeof(OicUuid_t));
  VERIFY_NON_NULL(TAG, *uuids, ERROR);

  do
  {
    unsigned char base64Buff[sizeof(((OicUuid_t *)0)->id)] = {};
    uint32_t outLen = 0;
    B64Result b64Ret = B64_OK;

    cJSON *jsonOwnr = cJSON_GetArrayItem(jsonObj, idxx);
    VERIFY_NON_NULL(TAG, jsonOwnr, ERROR);
    VERIFY_SUCCESS(TAG, cJSON_String == jsonOwnr->type, ERROR);

    outLen = 0;
    b64Ret = b64Decode(jsonOwnr->valuestring, strlen(jsonOwnr->valuestring), base64Buff,
                       sizeof(base64Buff), &outLen);

    VERIFY_SUCCESS(TAG, (b64Ret == B64_OK && outLen <= sizeof((*uuids)[idxx].id)),
                   ERROR);
    memcpy((*uuids)[idxx].id, base64Buff, outLen);
  }
  while (++idxx < *numUuids);

  return OC_STACK_OK;

exit:
  return OC_STACK_ERROR;

}

/**
 * Function to getting string of ownership transfer method
 *
 * @prarm oxmType ownership transfer method
 *
 * @return string value of ownership transfer method
 */
const char *GetOxmString(OicSecOxm_t oxmType)
{
  switch (oxmType)
  {
    case OIC_JUST_WORKS:
      return OXM_JUST_WORKS;
    case OIC_RANDOM_DEVICE_PIN:
      return OXM_RANDOM_DEVICE_PIN;
    case OIC_MANUFACTURER_CERTIFICATE:
      return OXM_MANUFACTURER_CERTIFICATE;
    default:
      return NULL;
  }
}

OCStackResult ConvertUuidToStr(const OicUuid_t *uuid, char **strUuid)
{
  if (NULL == uuid || NULL == strUuid || NULL != *strUuid)
  {
    OIC_LOG(ERROR, TAG, "ConvertUuidToStr : Invalid param");
    return OC_STACK_INVALID_PARAM;
  }

  size_t uuidIdx = 0;
  size_t urnIdx = 0;
  const size_t urnBufSize = (UUID_LENGTH * 2) + 4 + 1;
  char *convertedUrn = (char *)OICCalloc(urnBufSize, sizeof(char));
  VERIFY_NON_NULL(TAG, convertedUrn, ERROR);

  for (uuidIdx = 0, urnIdx = 0;  uuidIdx < UUID_LENGTH && urnIdx < urnBufSize; uuidIdx++, urnIdx += 2)
  {
    // canonical format for UUID has '8-4-4-4-12'
    if (uuidIdx == 4 || uuidIdx == 6 || uuidIdx == 8 || uuidIdx == 10)
    {
      snprintf(convertedUrn + urnIdx, 2, "%c", '-');
      urnIdx++;
    }
    snprintf(convertedUrn + urnIdx, 3, "%02x", (uint8_t)(uuid->id[uuidIdx]));
  }
  convertedUrn[urnBufSize - 1] = '\0';

  *strUuid = convertedUrn;
  return OC_STACK_OK;

exit:
  return OC_STACK_NO_MEMORY;
}

OCStackResult ConvertStrToUuid(const char *strUuid, OicUuid_t *uuid)
{
  if (NULL == strUuid || NULL == uuid)
  {
    OIC_LOG(ERROR, TAG, "ConvertStrToUuid : Invalid param");
    return OC_STACK_INVALID_PARAM;
  }

  size_t urnIdx = 0;
  size_t uuidIdx = 0;
  size_t strUuidLen = 0;
  char convertedUuid[UUID_LENGTH * 2] = {0};

  strUuidLen = strlen(strUuid);
  if (0 == strUuidLen)
  {
    OIC_LOG(INFO, TAG, "The empty string detected, The UUID will be converted to "\
            "\"00000000-0000-0000-0000-000000000000\"");
  }
  else if (UUID_LENGTH * 2 + 4 == strUuidLen)
  {
    for (uuidIdx = 0, urnIdx = 0; uuidIdx < UUID_LENGTH ; uuidIdx++, urnIdx += 2)
    {
      if (*(strUuid + urnIdx) == '-')
      {
        urnIdx++;
      }
      sscanf(strUuid + urnIdx, "%2hhx", &convertedUuid[uuidIdx]);
    }
  }
  else
  {
    OIC_LOG(ERROR, TAG, "Invalid string uuid format, Please set the uuid as correct format");
    OIC_LOG(ERROR, TAG, "e.g) \"72616E64-5069-6E44-6576-557569643030\" (4-2-2-2-6)");
    OIC_LOG(ERROR, TAG, "e.g) \"\"");

    return OC_STACK_INVALID_PARAM;
  }

  memcpy(uuid->id, convertedUuid, UUID_LENGTH);

  return OC_STACK_OK;
}
