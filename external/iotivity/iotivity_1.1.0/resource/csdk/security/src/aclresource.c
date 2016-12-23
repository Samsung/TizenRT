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

#ifdef WITH_ARDUINO
#include <string.h>
#else
#include <strings.h>
#endif
#include <stdlib.h>

#include "ocstack.h"
#include "ocserverrequest.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocrandom.h"
#include "ocpayload.h"
#include "utlist.h"
#include "payload_logging.h"
#include "srmresourcestrings.h"
#include "aclresource.h"
#include "doxmresource.h"
#include "resourcemanager.h"
#include "srmutility.h"
#include "psinterface.h"

#include "security_internals.h"

#define TAG  "SRM-ACL"
#define NUMBER_OF_SEC_PROV_RSCS 4
#define NUMBER_OF_DEFAULT_SEC_RSCS 2

static const uint8_t ACL_MAP_SIZE = 2;
static const uint8_t ACL_ACLIST_MAP_SIZE = 1;
static const uint8_t ACL_ACES_MAP_SIZE = 3;
static const uint8_t ACL_RESOURCE_MAP_SIZE = 4;


// CborSize is the default cbor payload size being used.
static const uint16_t CBOR_SIZE = 2048;

static OicSecAcl_t *gAcl = NULL;
static OCResourceHandle gAclHandle = NULL;

/**
 * This function frees OicSecAcl_t object's fields and object itself.
 */
static void FreeACE(OicSecAcl_t *ace)
{
  size_t i;
  if (NULL == ace)
  {
    OIC_LOG(ERROR, TAG, "Invalid Parameter");
    return;
  }

  // Clean Resources
  for (i = 0; i < ace->resourcesLen; i++)
  {
    OICFree(ace->resources[i]);
  }
  OICFree(ace->resources);

  //Clean Period
  if (ace->periods)
  {
    for (i = 0; i < ace->prdRecrLen; i++)
    {
      OICFree(ace->periods[i]);
    }
    OICFree(ace->periods);
  }

  //Clean Recurrence
  if (ace->recurrences)
  {
    for (i = 0; i < ace->prdRecrLen; i++)
    {
      OICFree(ace->recurrences[i]);
    }
    OICFree(ace->recurrences);
  }

  // Clean ACL node itself
  OICFree(ace);
}

void DeleteACLList(OicSecAcl_t *acl)
{
  if (acl)
  {
    OicSecAcl_t *aclTmp1 = NULL;
    OicSecAcl_t *aclTmp2 = NULL;
    LL_FOREACH_SAFE(acl, aclTmp1, aclTmp2)
    {
      LL_DELETE(acl, aclTmp1);
      FreeACE(aclTmp1);
    }
  }
}

static size_t OicSecAclSize(const OicSecAcl_t *secAcl)
{
  if (!secAcl)
  {
    return 0;
  }
  OicSecAcl_t *acl = (OicSecAcl_t *)secAcl;
  size_t size = 0;
  while (acl)
  {
    size++;
    acl = acl->next;
  }
  return size;
}

OCStackResult AclToCBORPayload(const OicSecAcl_t *secAcl, uint8_t **payload, size_t *size)
{
  if (NULL == secAcl || NULL == payload || NULL != *payload || NULL == size)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OCStackResult ret = OC_STACK_ERROR;
  CborError cborEncoderResult = CborNoError;
  OicSecAcl_t *acl = (OicSecAcl_t *)secAcl;
  CborEncoder encoder;
  CborEncoder aclMap;
  CborEncoder aclListMap;
  CborEncoder acesArray;
  uint8_t *outPayload = NULL;
  size_t cborLen = *size;
  *size = 0;
  *payload = NULL;

  if (cborLen == 0)
  {
    cborLen = CBOR_SIZE;
  }

  outPayload = (uint8_t *)OICCalloc(1, cborLen);
  VERIFY_NON_NULL(TAG, outPayload, ERROR);
  cbor_encoder_init(&encoder, outPayload, cborLen, 0);

  // Create ACL Map (aclist, rownerid)
  cborEncoderResult = cbor_encoder_create_map(&encoder, &aclMap, ACL_MAP_SIZE);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACL Map.");

  cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ACLIST_NAME,
                                              strlen(OIC_JSON_ACLIST_NAME));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding aclist Name Tag.");

  // Create ACLIST Map (aces)
  cborEncoderResult = cbor_encoder_create_map(&aclMap, &aclListMap, ACL_ACLIST_MAP_SIZE);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACLIST Map.");

  cborEncoderResult = cbor_encode_text_string(&aclListMap, OIC_JSON_ACES_NAME,
                                              strlen(OIC_JSON_ACES_NAME));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACES Name Tag.");

  // Create ACES Array
  cborEncoderResult = cbor_encoder_create_array(&aclListMap, &acesArray, OicSecAclSize(secAcl));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACES Array.");

  while (acl)
  {
    CborEncoder oicSecAclMap;
    // ACL Map size - Number of mandatory items
    uint8_t aclMapSize = ACL_ACES_MAP_SIZE;
    size_t inLen = 0;

    // Create ACL Map
    if (acl->periods)
    {
      ++aclMapSize;
    }
    if (acl->recurrences)
    {
      ++aclMapSize;
    }

    cborEncoderResult = cbor_encoder_create_map(&acesArray, &oicSecAclMap, aclMapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACES Map");

    // Subject -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_SUBJECTID_NAME,
                                                strlen(OIC_JSON_SUBJECTID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Subject Name Tag.");
    inLen = (memcmp(&(acl->subject), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0) ?
            WILDCARD_SUBJECT_ID_LEN : sizeof(OicUuid_t);
    if (inLen == WILDCARD_SUBJECT_ID_LEN)
    {
      cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, WILDCARD_RESOURCE_URI,
                                                  strlen(WILDCARD_RESOURCE_URI));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject Id wildcard Value.");
    }
    else
    {
      char *subject = NULL;
      ret = ConvertUuidToStr(&acl->subject, &subject);
      VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
      cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, subject, strlen(subject));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject Id Value.");
      OICFree(subject);
    }

    // Resources
    {
      CborEncoder resources;
      cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_RESOURCES_NAME,
                                                  strlen(OIC_JSON_RESOURCES_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Resource Name Tag.");

      cborEncoderResult = cbor_encoder_create_array(&oicSecAclMap, &resources, acl->resourcesLen);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Resource Name Array.");

      for (size_t i = 0; i < acl->resourcesLen; i++)
      {

        CborEncoder rMap;
        cborEncoderResult = cbor_encoder_create_map(&resources, &rMap, ACL_RESOURCE_MAP_SIZE);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Resource Map.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_HREF_NAME,
                                                    strlen(OIC_JSON_HREF_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding HREF Name Tag.");
        cborEncoderResult = cbor_encode_text_string(&rMap, acl->resources[i],
                                                    strlen(acl->resources[i]));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding HREF Value in Map.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_REL_NAME,
                                                    strlen(OIC_JSON_REL_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding REL Name Tag.");

        // TODO : Need to assign real value of REL
        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                                                    strlen(OIC_JSON_EMPTY_STRING));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding REL Value.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_RT_NAME,
                                                    strlen(OIC_JSON_RT_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");

        // TODO : Need to assign real value of RT
        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                                                    strlen(OIC_JSON_EMPTY_STRING));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");

        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_IF_NAME,
                                                    strlen(OIC_JSON_IF_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");

        // TODO : Need to assign real value of IF
        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                                                    strlen(OIC_JSON_EMPTY_STRING));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");


        cborEncoderResult = cbor_encoder_close_container(&resources, &rMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Map.");

      }
      cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &resources);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Name Array.");
    }


    // Permissions -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_PERMISSION_NAME,
                                                strlen(OIC_JSON_PERMISSION_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Permission Name Tag.");
    cborEncoderResult = cbor_encode_int(&oicSecAclMap, acl->permission);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Permission Name Value.");

    // Period -- Not Mandatory
    if (acl->periods)
    {

      CborEncoder period;
      cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_PERIOD_NAME,
                                                  strlen(OIC_JSON_PERIOD_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Tag.");
      cborEncoderResult = cbor_encoder_create_array(&oicSecAclMap, &period, acl->prdRecrLen);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Array.");
      for (size_t i = 0; i < acl->prdRecrLen; i++)
      {
        cborEncoderResult = cbor_encode_text_string(&period, acl->periods[i],
                                                    strlen(acl->periods[i]));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Value in Array.");
      }
      cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &period);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Period Array.");
    }

    // Recurrence -- Not Mandatory
    if (acl->recurrences)
    {
      CborEncoder recurrences;
      cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_RECURRENCES_NAME,
                                                  strlen(OIC_JSON_RECURRENCES_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Recurrence Tag.");
      cborEncoderResult = cbor_encoder_create_array(&oicSecAclMap, &recurrences, acl->prdRecrLen);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Recurrence Array.");

      for (size_t i = 0; i < acl->prdRecrLen; i++)
      {
        cborEncoderResult = cbor_encode_text_string(&recurrences, acl->recurrences[i],
                                                    strlen(acl->recurrences[i]));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Recurrence Array Value.");
      }
      cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &recurrences);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Recurrence Array");

    }


    cborEncoderResult = cbor_encoder_close_container(&acesArray, &oicSecAclMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACES Map.");
    acl = acl->next;
  }

  // Close ACES Array
  cborEncoderResult = cbor_encoder_close_container(&aclListMap, &acesArray);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACES Array.");

  // Close ACLIST Map
  cborEncoderResult = cbor_encoder_close_container(&aclMap, &aclListMap);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACLIST Map.");

  // Rownerid
  {
    char *rowner = NULL;
    cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ROWNERID_NAME,
                                                strlen(OIC_JSON_ROWNERID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding rownerid Name.");
    ret = ConvertUuidToStr(&secAcl->rownerID, &rowner);
    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
    cborEncoderResult = cbor_encode_text_string(&aclMap, rowner, strlen(rowner));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding rownerid Value.");
    OICFree(rowner);
  }

  // Close ACL Map
  cborEncoderResult = cbor_encoder_close_container(&encoder, &aclMap);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACL Map.");

  if (CborNoError == cborEncoderResult)
  {
    OIC_LOG(DEBUG, TAG, "AclToCBORPayload Successed");
    *size = encoder.ptr - outPayload;
    *payload = outPayload;
    ret = OC_STACK_OK;
  }
exit:
  if (CborErrorOutOfMemory == cborEncoderResult)
  {
    OIC_LOG(DEBUG, TAG, "AclToCBORPayload:CborErrorOutOfMemory : retry with more memory");

    // reallocate and try again!
    OICFree(outPayload);
    // Since the allocated initial memory failed, double the memory.
    cborLen += encoder.ptr - encoder.end;
    cborEncoderResult = CborNoError;
    ret = AclToCBORPayload(secAcl, payload, &cborLen);
    *size = cborLen;
  }
  else if (cborEncoderResult != CborNoError)
  {
    OIC_LOG(ERROR, TAG, "Failed to AclToCBORPayload");
    OICFree(outPayload);
    outPayload = NULL;
    *size = 0;
    *payload = NULL;
    ret = OC_STACK_ERROR;
  }

  return ret;
}

// This function converts CBOR format to ACL data.
// Caller needs to invoke 'free' when done using
// note: This function is used in unit test hence not declared static,
OicSecAcl_t *CBORPayloadToAcl(const uint8_t *cborPayload, const size_t size)
{
  if (NULL == cborPayload || 0 == size)
  {
    return NULL;
  }
  OCStackResult ret = OC_STACK_ERROR;
  CborValue aclCbor = { .parser = NULL };
  CborParser parser = { .end = NULL };
  CborError cborFindResult = CborNoError;
  cbor_parser_init(cborPayload, size, 0, &parser, &aclCbor);

  OicSecAcl_t *headAcl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));

  // Enter ACL Map
  CborValue aclMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
  cborFindResult = cbor_value_enter_container(&aclCbor, &aclMap);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACL Map.");

  while (cbor_value_is_valid(&aclMap))
  {
    char *tagName = NULL;
    size_t len = 0;
    CborType type = cbor_value_get_type(&aclMap);
    if (type == CborTextStringType)
    {
      cborFindResult = cbor_value_dup_text_string(&aclMap, &tagName, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACL Map.");
      cborFindResult = cbor_value_advance(&aclMap);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACL Map.");
    }
    if (tagName)
    {
      if (strcmp(tagName, OIC_JSON_ACLIST_NAME)  == 0)
      {
        // Enter ACLIST Map
        CborValue aclistMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
        cborFindResult = cbor_value_enter_container(&aclMap, &aclistMap);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACLIST Map.");


        while (cbor_value_is_valid(&aclistMap))
        {
          char *acName = NULL;
          size_t acLen = 0;
          CborType acType = cbor_value_get_type(&aclistMap);
          if (acType == CborTextStringType)
          {
            cborFindResult = cbor_value_dup_text_string(&aclistMap, &acName, &acLen, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACLIST Map.");
            cborFindResult = cbor_value_advance(&aclistMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACLIST Map.");
          }
          if (acName)
          {
            if (strcmp(acName, OIC_JSON_ACES_NAME)  == 0)
            {

              // Enter ACES Array
              CborValue aclArray = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
              cborFindResult = cbor_value_enter_container(&aclistMap, &aclArray);
              VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACL Array.");

              int acesCount = 0;
              while (cbor_value_is_valid(&aclArray))
              {
                acesCount++;

                CborValue aclMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                cborFindResult = cbor_value_enter_container(&aclArray, &aclMap);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACL Map.");
                OicSecAcl_t *acl = NULL;

                if (acesCount == 1)
                {
                  acl = headAcl;
                }
                else
                {
                  acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
                  OicSecAcl_t *temp = headAcl;
                  while (temp->next)
                  {
                    temp = temp->next;
                  }
                  temp->next = acl;
                }
                VERIFY_NON_NULL(TAG, acl, ERROR);

                while (cbor_value_is_valid(&aclMap))
                {
                  char *name = NULL;
                  size_t len = 0;
                  CborType type = cbor_value_get_type(&aclMap);
                  if (type == CborTextStringType)
                  {
                    cborFindResult = cbor_value_dup_text_string(&aclMap, &name, &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACL Map.");
                    cborFindResult = cbor_value_advance(&aclMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACL Map.");
                  }
                  if (name)
                  {
                    // Subject -- Mandatory
                    if (strcmp(name, OIC_JSON_SUBJECTID_NAME)  == 0)
                    {
                      char *subject = NULL;
                      cborFindResult = cbor_value_dup_text_string(&aclMap, &subject, &len, NULL);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subject Value.");
                      if (strcmp(subject, WILDCARD_RESOURCE_URI) == 0)
                      {
                        acl->subject.id[0] = '*';
                      }
                      else
                      {
                        ret = ConvertStrToUuid(subject, &acl->subject);
                        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                      }
                      OICFree(subject);
                    }

                    // Resources -- Mandatory
                    if (strcmp(name, OIC_JSON_RESOURCES_NAME) == 0)
                    {
                      CborValue resources = { .parser = NULL };
                      cborFindResult = cbor_value_get_array_length(&aclMap, &acl->resourcesLen);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Resource Array Len Value.");
                      cborFindResult = cbor_value_enter_container(&aclMap, &resources);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering a Resource Array.");

                      acl->resources = (char **) OICMalloc(acl->resourcesLen * sizeof(char *));
                      VERIFY_NON_NULL(TAG, acl->resources, ERROR);
                      int i = 0;
                      while (cbor_value_is_valid(&resources))
                      {
                        // rMap
                        CborValue rMap = { .parser = NULL  };
                        cborFindResult = cbor_value_enter_container(&resources, &rMap);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering Resource Map");


                        while (cbor_value_is_valid(&rMap))
                        {
                          char *rMapName = NULL;
                          size_t rMapNameLen = 0;
                          cborFindResult = cbor_value_dup_text_string(&rMap, &rMapName, &rMapNameLen, NULL);
                          VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RMap Data Name Tag.");
                          cborFindResult = cbor_value_advance(&rMap);
                          VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RMap Data Value.");

                          // "href"
                          if (0 == strcmp(OIC_JSON_HREF_NAME, rMapName))
                          {
                            // TODO : Need to check data structure of OicSecAcl_t based on RAML spec.
                            cborFindResult = cbor_value_dup_text_string(&rMap, &acl->resources[i++], &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Href Value.");
                          }
                          // "rel"
                          if (0 == strcmp(OIC_JSON_REL_NAME, rMapName))
                          {
                            // TODO : Need to check data structure of OicSecAcl_t based on RAML spec.
                            char *relData = NULL;
                            cborFindResult = cbor_value_dup_text_string(&rMap, &relData, &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding REL Value.");
                            OICFree(relData);
                          }
                          // "rt"
                          if (0 == strcmp(OIC_JSON_RT_NAME, rMapName))
                          {
                            // TODO : Need to check data structure of OicSecAcl_t and assign based on RAML spec.
                            char *rtData = NULL;
                            cborFindResult = cbor_value_dup_text_string(&rMap, &rtData, &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RT Value.");
                            OICFree(rtData);
                          }

                          // "if"
                          if (0 == strcmp(OIC_JSON_IF_NAME, rMapName))
                          {
                            // TODO : Need to check data structure of OicSecAcl_t and assign based on RAML spec.
                            char *ifData = NULL;
                            cborFindResult = cbor_value_dup_text_string(&rMap, &ifData, &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF Value.");
                            OICFree(ifData);
                          }

                          if (cbor_value_is_valid(&rMap))
                          {
                            cborFindResult = cbor_value_advance(&rMap);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Rlist Map.");
                          }
                          OICFree(rMapName);
                        }

                        if (cbor_value_is_valid(&resources))
                        {
                          cborFindResult = cbor_value_advance(&resources);
                          VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Resource Array.");
                        }
                      }
                    }

                    // Permissions -- Mandatory
                    if (strcmp(name, OIC_JSON_PERMISSION_NAME) == 0)
                    {
                      cborFindResult = cbor_value_get_uint64(&aclMap, (uint64_t *) &acl->permission);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a PERM Value.");
                    }

                    // Period -- Not mandatory
                    if (strcmp(name, OIC_JSON_PERIOD_NAME) == 0)
                    {
                      CborValue period = { .parser = NULL };
                      cborFindResult = cbor_value_get_array_length(&aclMap, &acl->prdRecrLen);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period Array Len.");
                      cborFindResult = cbor_value_enter_container(&aclMap, &period);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period Array Map.");
                      acl->periods = (char **)OICCalloc(acl->prdRecrLen, sizeof(char *));
                      VERIFY_NON_NULL(TAG, acl->periods, ERROR);
                      int i = 0;
                      while (cbor_value_is_text_string(&period))
                      {
                        cborFindResult = cbor_value_dup_text_string(&period, &acl->periods[i++],
                                                                    &len, NULL);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period Array Value.");
                        cborFindResult = cbor_value_advance(&period);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing a Period Array.");
                      }
                    }

                    // Recurrence -- Not mandatory
                    if (strcmp(name, OIC_JSON_RECURRENCES_NAME) == 0)
                    {
                      CborValue recurrences = { .parser = NULL };
                      cborFindResult = cbor_value_enter_container(&aclMap, &recurrences);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Adding Recurrence Array.");
                      acl->recurrences = (char **)OICCalloc(acl->prdRecrLen, sizeof(char *));
                      VERIFY_NON_NULL(TAG, acl->recurrences, ERROR);
                      int i = 0;
                      while (cbor_value_is_text_string(&recurrences))
                      {
                        cborFindResult = cbor_value_dup_text_string(&recurrences,
                                                                    &acl->recurrences[i++], &len, NULL);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Adding Recurrence Array Value.");
                        cborFindResult = cbor_value_advance(&recurrences);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Closing Recurrence Array.");
                      }
                    }

                    OICFree(name);
                  }

                  if (type != CborMapType && cbor_value_is_valid(&aclMap))
                  {
                    cborFindResult = cbor_value_advance(&aclMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing the Array.");
                  }
                }

                acl->next = NULL;

                if (cbor_value_is_valid(&aclArray))
                {
                  cborFindResult = cbor_value_advance(&aclArray);
                  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Array.");
                }
              }
            }
            OICFree(acName);
          }

          if (cbor_value_is_valid(&aclistMap))
          {
            cborFindResult = cbor_value_advance(&aclistMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACLIST Map.");
          }
        }
      }

      // TODO : Need to modify headAcl->owners[0].id to headAcl->rowner based on RAML spec.
      if (strcmp(tagName, OIC_JSON_ROWNERID_NAME)  == 0)
      {
        char *stRowner = NULL;
        cborFindResult = cbor_value_dup_text_string(&aclMap, &stRowner, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Rownerid Value.");
        ret = ConvertStrToUuid(stRowner, &headAcl->rownerID);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        OICFree(stRowner);
      }
      OICFree(tagName);
    }
    if (cbor_value_is_valid(&aclMap))
    {
      cborFindResult = cbor_value_advance(&aclMap);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Map.");
    }
  }

exit:
  if (cborFindResult != CborNoError)
  {
    OIC_LOG(ERROR, TAG, "Failed to CBORPayloadToAcl");
    DeleteACLList(headAcl);
    headAcl = NULL;
  }
  return headAcl;
}

/**
 * This method removes ACE for the subject and resource from the ACL
 *
 * @param subject of the ACE
 * @param resource of the ACE
 *
 * @return
 *     ::OC_STACK_RESOURCE_DELETED on success
 *     ::OC_STACK_NO_RESOURCE on failure to find the appropriate ACE
 *     ::OC_STACK_INVALID_PARAM on invalid parameter
 */
static OCStackResult RemoveACE(const OicUuid_t *subject, const char *resource)
{
  OIC_LOG(DEBUG, TAG, "IN RemoveACE");

  OicSecAcl_t *acl = NULL;
  OicSecAcl_t *tempAcl = NULL;
  bool deleteFlag = false;
  OCStackResult ret = OC_STACK_NO_RESOURCE;

  if (memcmp(subject->id, &WILDCARD_SUBJECT_ID, sizeof(subject->id)) == 0)
  {
    OIC_LOG_V(ERROR, TAG, "%s received invalid parameter", __func__);
    return  OC_STACK_INVALID_PARAM;
  }

  //If resource is NULL then delete all the ACE for the subject.
  if (NULL == resource || resource[0] == '\0')
  {
    LL_FOREACH_SAFE(gAcl, acl, tempAcl)
    {
      if (memcmp(acl->subject.id, subject->id, sizeof(subject->id)) == 0)
      {
        LL_DELETE(gAcl, acl);
        FreeACE(acl);
        deleteFlag = true;
      }
    }
  }
  else
  {
    //Looping through ACL to find the right ACE to delete. If the required resource is the only
    //resource in the ACE for the subject then delete the whole ACE. If there are more resources
    //than the required resource in the ACE, for the subject then just delete the resource from
    //the resource array
    LL_FOREACH_SAFE(gAcl, acl, tempAcl)
    {
      if (memcmp(acl->subject.id, subject->id, sizeof(subject->id)) == 0)
      {
        if (1 == acl->resourcesLen && strcmp(acl->resources[0], resource) == 0)
        {
          LL_DELETE(gAcl, acl);
          FreeACE(acl);
          deleteFlag = true;
          break;
        }
        else
        {
          size_t resPos = -1;
          size_t i;
          for (i = 0; i < acl->resourcesLen; i++)
          {
            if (strcmp(acl->resources[i], resource) == 0)
            {
              resPos = i;
              break;
            }
          }
          if (0 <= (int) resPos)
          {
            OICFree(acl->resources[resPos]);
            acl->resources[resPos] = NULL;
            acl->resourcesLen -= 1;
            for (i = resPos; i < acl->resourcesLen; i++)
            {
              acl->resources[i] = acl->resources[i + 1];
            }
            deleteFlag = true;
            break;
          }
        }
      }
    }
  }

  if (deleteFlag)
  {
    // In case of unit test do not update persistant storage.
    if (memcmp(subject->id, &WILDCARD_SUBJECT_B64_ID, sizeof(subject->id)) == 0)
    {
      ret = OC_STACK_RESOURCE_DELETED;
    }
    else
    {
      uint8_t *payload = NULL;
      size_t size = 0;
      if (OC_STACK_OK == AclToCBORPayload(gAcl, &payload, &size))
      {
        if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size))
        {
          ret = OC_STACK_RESOURCE_DELETED;
        }
        OICFree(payload);
      }
    }
  }
  return ret;
}

/**
 * This method parses the query string received for REST requests and
 * retrieves the 'subject' field.
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

  while (GetNextQuery(&parseIter))
  {
    if (strncasecmp((char *) parseIter.attrPos, OIC_JSON_SUBJECTID_NAME, parseIter.attrLen) == 0)
    {
      VERIFY_SUCCESS(TAG, 0 != parseIter.valLen, ERROR);
      memcpy(subject->id, parseIter.valPos, parseIter.valLen);
      return true;
    }
  }

exit:
  return false;
}

/**
 * This method parses the query string received for REST requests and
 * retrieves the 'resource' field.
 *
 * @param query querystring passed in REST request
 * @param resource resource parsed from query string
 * @param resourceSize size of the memory pointed to resource
 *
 * @return true if query parsed successfully and found 'resource', else false.
 */
static bool GetResourceFromQueryString(const char *query, char *resource, size_t resourceSize)
{
  OicParseQueryIter_t parseIter = { .attrPos = NULL };

  ParseQueryIterInit((unsigned char *) query, &parseIter);

  while (GetNextQuery(&parseIter))
  {
    if (strncasecmp((char *) parseIter.attrPos, OIC_JSON_RESOURCES_NAME, parseIter.attrLen)
        == 0)
    {
      VERIFY_SUCCESS(TAG, 0 != parseIter.valLen, ERROR);
      OICStrcpy(resource, resourceSize, (char *) parseIter.valPos);

      return true;
    }
  }

exit:
  return false;
}

static OCEntityHandlerResult HandleACLGetRequest(const OCEntityHandlerRequest *ehRequest)
{
  OIC_LOG(INFO, TAG, "HandleACLGetRequest processing the request");
  uint8_t *payload = NULL;
  size_t size = 0;
  OCEntityHandlerResult ehRet;

  // Process the REST querystring parameters
  if (ehRequest->query)
  {
    OIC_LOG(DEBUG, TAG, "HandleACLGetRequest processing query");

    OicUuid_t subject = {.id = { 0 } };
    char resource[MAX_URI_LENGTH] = { 0 };

    OicSecAcl_t *savePtr = NULL;
    const OicSecAcl_t *currentAce = NULL;

    // 'Subject' field is MUST for processing a querystring in REST request.
    VERIFY_SUCCESS(TAG, true == GetSubjectFromQueryString(ehRequest->query, &subject), ERROR);

    GetResourceFromQueryString(ehRequest->query, resource, sizeof(resource));

    /*
     * TODO : Currently, this code only provides one ACE for a Subject.
     * Below code needs to be updated for scenarios when Subject have
     * multiple ACE's in ACL resource.
     */
    while ((currentAce = GetACLResourceData(&subject, &savePtr)))
    {
      /*
       * If REST querystring contains a specific resource, we need
       * to search for that resource in ACE.
       */
      if (resource[0] != '\0')
      {
        for (size_t n = 0; n < currentAce->resourcesLen; n++)
        {
          if ((currentAce->resources[n])
              && (0 == strcmp(resource, currentAce->resources[n])
                  || 0 == strcmp(WILDCARD_RESOURCE_URI, currentAce->resources[n])))
          {
            // Convert ACL data into CBOR format for transmission
            if (OC_STACK_OK != AclToCBORPayload(currentAce, &payload, &size))
            {
              ehRet = OC_EH_ERROR;
            }
            goto exit;
          }
        }
      }
      else
      {
        // Convert ACL data into CBOR format for transmission
        if (OC_STACK_OK != AclToCBORPayload(currentAce, &payload, &size))
        {
          ehRet = OC_EH_ERROR;
        }
        goto exit;
      }
    }
  }
  else
  {
    // Convert ACL data into CBOR format for transmission.
    if (OC_STACK_OK != AclToCBORPayload(gAcl, &payload, &size))
    {
      ehRet = OC_EH_ERROR;
    }
  }
exit:
  // A device should always have a default acl. Therefore, payload should never be NULL.
  ehRet = (payload ? OC_EH_OK : OC_EH_ERROR);

  // Send response payload to request originator
  if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, payload, size))
  {
    ehRet = OC_EH_ERROR;
    OIC_LOG(ERROR, TAG, "SendSRMResponse failed for HandleACLGetRequest");
  }
  OICFree(payload);

  OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__, ehRet);
  return ehRet;
}

static OCEntityHandlerResult HandleACLPostRequest(const OCEntityHandlerRequest *ehRequest)
{
  OIC_LOG(INFO, TAG, "HandleACLPostRequest processing the request");
  OCEntityHandlerResult ehRet = OC_EH_ERROR;

  // Convert CBOR into ACL data and update to SVR buffers. This will also validate the ACL data received.
  uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
  size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;
  if (payload)
  {
    OicSecAcl_t *newAcl = CBORPayloadToAcl(payload, size);
    if (newAcl)
    {
      // Append the new ACL to existing ACL
      LL_APPEND(gAcl, newAcl);
      size_t size = 0;
      // In case of unit test do not update persistant storage.
      if (memcmp(newAcl->subject.id, &WILDCARD_SUBJECT_ID, sizeof(newAcl->subject.id)) == 0
          || memcmp(newAcl->subject.id, &WILDCARD_SUBJECT_B64_ID, sizeof(newAcl->subject.id)) == 0)
      {
        ehRet = OC_EH_RESOURCE_CREATED;
      }
      else
      {
        uint8_t *cborPayload = NULL;
        if (OC_STACK_OK == AclToCBORPayload(gAcl, &cborPayload, &size))
        {
          if (UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, cborPayload, size) == OC_STACK_OK)
          {
            ehRet = OC_EH_RESOURCE_CREATED;
          }
          OICFree(cborPayload);
        }
      }
    }
  }

  // Send payload to request originator
  if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
  {
    ehRet = OC_EH_ERROR;
    OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleACLPostRequest");
  }

  OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__, ehRet);
  return ehRet;
}

static OCEntityHandlerResult HandleACLDeleteRequest(const OCEntityHandlerRequest *ehRequest)
{
  OIC_LOG(DEBUG, TAG, "Processing ACLDeleteRequest");
  OCEntityHandlerResult ehRet = OC_EH_ERROR;
  OicUuid_t subject = { .id = { 0 } };
  char resource[MAX_URI_LENGTH] = { 0 };

  VERIFY_NON_NULL(TAG, ehRequest->query, ERROR);

  // 'Subject' field is MUST for processing a querystring in REST request.
  VERIFY_SUCCESS(TAG, true == GetSubjectFromQueryString(ehRequest->query, &subject), ERROR);

  GetResourceFromQueryString(ehRequest->query, resource, sizeof(resource));

  if (OC_STACK_RESOURCE_DELETED == RemoveACE(&subject, resource))
  {
    ehRet = OC_EH_RESOURCE_DELETED;
  }

exit:
  // Send payload to request originator
  if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
  {
    ehRet = OC_EH_ERROR;
    OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleACLDeleteRequest");
  }

  return ehRet;
}

OCEntityHandlerResult ACLEntityHandler(OCEntityHandlerFlag flag, OCEntityHandlerRequest *ehRequest,
                                       void *callbackParameter)
{
  OIC_LOG(DEBUG, TAG, "Received request ACLEntityHandler");
  (void)callbackParameter;
  OCEntityHandlerResult ehRet = OC_EH_ERROR;

  if (!ehRequest)
  {
    return ehRet;
  }

  if (flag & OC_REQUEST_FLAG)
  {
    // TODO :  Handle PUT method
    OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
    switch (ehRequest->method)
    {
      case OC_REST_GET:
        ehRet = HandleACLGetRequest(ehRequest);
        break;

      case OC_REST_POST:
        ehRet = HandleACLPostRequest(ehRequest);
        break;

      case OC_REST_DELETE:
        ehRet = HandleACLDeleteRequest(ehRequest);
        break;

      default:
        ehRet = OC_EH_ERROR;
        SendSRMResponse(ehRequest, ehRet, NULL, 0);
    }
  }

  return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/acl' resource.
 */
static OCStackResult CreateACLResource()
{
  OCStackResult ret;

  ret = OCCreateResource(&gAclHandle,
                         OIC_RSRC_TYPE_SEC_ACL,
                         OIC_MI_DEF,
                         OIC_RSRC_ACL_URI,
                         ACLEntityHandler,
                         NULL,
                         OC_OBSERVABLE | OC_SECURE | OC_EXPLICIT_DISCOVERABLE);

  if (OC_STACK_OK != ret)
  {
    OIC_LOG(FATAL, TAG, "Unable to instantiate ACL resource");
    DeInitACLResource();
  }
  return ret;
}

// This function sets the default ACL and is defined for the unit test only.
OCStackResult SetDefaultACL(OicSecAcl_t *acl)
{
  gAcl = acl;
  return OC_STACK_OK;
}

OCStackResult GetDefaultACL(OicSecAcl_t **defaultAcl)
{
  OCStackResult ret = OC_STACK_ERROR;

  OicUuid_t ownerId = { .id = { 0 } };

  /*
   * TODO In future, when new virtual resources will be added in OIC
   * specification, Iotivity stack should be able to add them in
   * existing SVR database. To support this, we need to add 'versioning'
   * mechanism in SVR database.
   */

  const char *rsrcs[] =
  {
    OC_RSRVD_WELL_KNOWN_URI,
    OC_RSRVD_DEVICE_URI,
    OC_RSRVD_PLATFORM_URI,
    OC_RSRVD_RESOURCE_TYPES_URI,
#ifdef WITH_PRESENCE
    OC_RSRVD_PRESENCE_URI,
#endif //WITH_PRESENCE
    OIC_RSRC_ACL_URI,
    OIC_RSRC_DOXM_URI,
    OIC_RSRC_PSTAT_URI,
  };

  if (!defaultAcl)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
  VERIFY_NON_NULL(TAG, acl, ERROR);

  // Subject -- Mandatory
  memcpy(&(acl->subject), &WILDCARD_SUBJECT_ID, sizeof(acl->subject));

  // Resources -- Mandatory
  acl->resourcesLen = sizeof(rsrcs) / sizeof(rsrcs[0]);

  acl->resources = (char **) OICCalloc(acl->resourcesLen, sizeof(char *));
  VERIFY_NON_NULL(TAG, (acl->resources), ERROR);

  for (size_t i = 0; i < acl->resourcesLen; i++)
  {
    size_t len = strlen(rsrcs[i]) + 1;
    acl->resources[i] = (char *) OICMalloc(len * sizeof(char));
    VERIFY_NON_NULL(TAG, (acl->resources[i]), ERROR);
    OICStrcpy(acl->resources[i], len, rsrcs[i]);
  }

  acl->permission = PERMISSION_READ;
  acl->prdRecrLen = 0;
  acl->periods = NULL;
  acl->recurrences = NULL;

  // Device ID is the owner of this default ACL
  if (GetDoxmResourceData() != NULL)
  {
    ret = GetDoxmDeviceID(&ownerId);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, FATAL);
  }
  else
  {
    OCRandomUuidResult rdm = OCGenerateUuid(ownerId.id);
    VERIFY_SUCCESS(TAG, RAND_UUID_OK == rdm, FATAL);
  }

  memcpy(&acl->rownerID, &ownerId, sizeof(OicUuid_t));

  acl->next = NULL;

  *defaultAcl = acl;
  ret = OC_STACK_OK;

exit:

  if (ret != OC_STACK_OK)
  {
    DeleteACLList(acl);
    acl = NULL;
  }

  return ret;
}

OCStackResult InitACLResource()
{
  OCStackResult ret = OC_STACK_ERROR;

  uint8_t *data = NULL;
  size_t size = 0;
  ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_ACL_NAME, &data, &size);
  // If database read failed
  if (ret != OC_STACK_OK)
  {
    OIC_LOG(DEBUG, TAG, "ReadSVDataFromPS failed");
  }
  if (data)
  {
    // Read ACL resource from PS
    gAcl = CBORPayloadToAcl(data, size);
  }
  /*
   * If SVR database in persistent storage got corrupted or
   * is not available for some reason, a default ACL is created
   * which allows user to initiate ACL provisioning again.
   */
  if (!gAcl)
  {
    GetDefaultACL(&gAcl);
    // TODO Needs to update persistent storage
  }
  VERIFY_NON_NULL(TAG, gAcl, FATAL);

  // Instantiate 'oic.sec.acl'
  ret = CreateACLResource();

exit:
  if (OC_STACK_OK != ret)
  {
    DeInitACLResource();
  }
  return ret;
}

OCStackResult DeInitACLResource()
{
  OCStackResult ret =  OCDeleteResource(gAclHandle);
  gAclHandle = NULL;

  if (gAcl)
  {
    DeleteACLList(gAcl);
    gAcl = NULL;
  }
  return ret;
}

const OicSecAcl_t *GetACLResourceData(const OicUuid_t *subjectId, OicSecAcl_t **savePtr)
{
  OicSecAcl_t *acl = NULL;
  OicSecAcl_t *begin = NULL;

  if (NULL == subjectId)
  {
    return NULL;
  }

  /*
   * savePtr MUST point to NULL if this is the 'first' call to retrieve ACL for
   * subjectID.
   */
  if (NULL == *savePtr)
  {
    begin = gAcl;
  }
  else
  {
    /*
     * If this is a 'successive' call, search for location pointed by
     * savePtr and assign 'begin' to the next ACL after it in the linked
     * list and start searching from there.
     */
    LL_FOREACH(gAcl, acl)
    {
      if (acl == *savePtr)
      {
        begin = acl->next;
      }
    }
  }

  // Find the next ACL corresponding to the 'subjectID' and return it.
  LL_FOREACH(begin, acl)
  {
    if (memcmp(&(acl->subject), subjectId, sizeof(OicUuid_t)) == 0)
    {
      *savePtr = acl;
      return acl;
    }
  }

  // Cleanup in case no ACL is found
  *savePtr = NULL;
  return NULL;
}

OCStackResult InstallNewACL(const uint8_t *cborPayload, const size_t size)
{
  OCStackResult ret = OC_STACK_ERROR;

  // Convert CBOR format to ACL data. This will also validate the ACL data received.
  OicSecAcl_t *newAcl = CBORPayloadToAcl(cborPayload, size);

  if (newAcl)
  {
    // Append the new ACL to existing ACL
    LL_APPEND(gAcl, newAcl);

    // Update persistent storage only if it is not WILDCARD_SUBJECT_ID
    if (memcmp(newAcl->subject.id, &WILDCARD_SUBJECT_ID, sizeof(newAcl->subject.id)) == 0
        || memcmp(newAcl->subject.id, &WILDCARD_SUBJECT_B64_ID, sizeof(newAcl->subject.id)) == 0)
    {
      ret = OC_STACK_OK;
    }
    else
    {
      size_t size = 0;
      uint8_t *payload = NULL;
      if (OC_STACK_OK == AclToCBORPayload(gAcl, &payload, &size))
      {
        if (UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size) == OC_STACK_OK)
        {
          ret = OC_STACK_OK;
        }
        OICFree(payload);
      }
    }
  }

  return ret;
}

/**
 * This function generates default ACL for security resource in case of owned status.
 *
 * @return Default ACL for security resource.
 */
static OicSecAcl_t *GetSecDefaultACL()
{
  const char *sec_rsrcs[] =
  {
    OIC_RSRC_DOXM_URI,
    OIC_RSRC_PSTAT_URI
  };
  OicUuid_t ownerId = {.id = {0}};
  OCStackResult res = OC_STACK_ERROR;
  OicSecAcl_t *newDefaultAcl = (OicSecAcl_t *)OICCalloc(1, sizeof(OicSecAcl_t));
  VERIFY_NON_NULL(TAG, newDefaultAcl, ERROR);

  // Subject -- Mandatory
  memcpy(&(newDefaultAcl->subject), &WILDCARD_SUBJECT_ID, WILDCARD_SUBJECT_ID_LEN);

  // Resources -- Mandatory
  newDefaultAcl->resourcesLen = NUMBER_OF_DEFAULT_SEC_RSCS;
  newDefaultAcl->resources = (char **)OICCalloc(NUMBER_OF_DEFAULT_SEC_RSCS, sizeof(char *));
  VERIFY_NON_NULL(TAG, (newDefaultAcl->resources), ERROR);

  for (size_t i = 0; i <  NUMBER_OF_DEFAULT_SEC_RSCS; i++)
  {
    size_t len = strlen(sec_rsrcs[i]) + 1;
    newDefaultAcl->resources[i] = (char *)OICMalloc(len * sizeof(char));
    VERIFY_NON_NULL(TAG, (newDefaultAcl->resources[i]), ERROR);
    OICStrcpy(newDefaultAcl->resources[i], len, sec_rsrcs[i]);
  }

  // Permissions -- Mandatory
  newDefaultAcl->permission = PERMISSION_READ;

  //Period -- Not Mandatory
  newDefaultAcl->prdRecrLen = 0;
  newDefaultAcl->periods = NULL;

  //Recurrence -- Not Mandatory
  newDefaultAcl->recurrences = NULL;

  // Device ID is the owner of this default ACL
  res = GetDoxmDeviceID(&ownerId);
  VERIFY_SUCCESS(TAG, OC_STACK_OK == res, FATAL);

  // Owners -- Mandatory
  memcpy(&newDefaultAcl->rownerID, &ownerId, sizeof(OicUuid_t));

  return newDefaultAcl;
exit:
  DeleteACLList(newDefaultAcl);
  return NULL;

}

OCStackResult UpdateDefaultSecProvACL()
{
  OCStackResult ret = OC_STACK_OK;
  OicSecAcl_t *acl = NULL;
  OicSecAcl_t *tmp = NULL;

  if (gAcl)
  {
    int matchedRsrc = 0;
    bool isRemoved = false;

    LL_FOREACH_SAFE(gAcl, acl, tmp)
    {
      //Find default security resource ACL
      if (memcmp(&acl->subject, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0 &&
          ((PERMISSION_READ | PERMISSION_WRITE) == acl->permission))
      {
        matchedRsrc = 0;

        for (size_t i = 0; i < acl->resourcesLen; i++)
        {
          if (strncmp(acl->resources[i], OIC_RSRC_DOXM_URI,
                      strlen(OIC_RSRC_DOXM_URI) + 1) == 0 ||
              strncmp(acl->resources[i], OIC_RSRC_CRED_URI,
                      strlen(OIC_RSRC_CRED_URI) + 1) == 0 ||
              strncmp(acl->resources[i], OIC_RSRC_ACL_URI,
                      strlen(OIC_RSRC_ACL_URI) + 1) == 0 ||
              strncmp(acl->resources[i], OIC_RSRC_PSTAT_URI,
                      strlen(OIC_RSRC_PSTAT_URI) + 1) == 0)
          {
            matchedRsrc++;
          }
        }

        //If default security resource ACL is detected, delete it.
        if (NUMBER_OF_SEC_PROV_RSCS == matchedRsrc)
        {
          LL_DELETE(gAcl, acl);
          FreeACE(acl);
          isRemoved = true;
        }
      }
    }

    if (isRemoved)
    {
      /*
       * Generate new security resource ACL as follows :
       *      subject : "*"
       *      resources :  '/oic/sec/doxm', '/oic/sec/pstat'
       *      permission : READ
       */
      OicSecAcl_t *newDefaultAcl = GetSecDefaultACL();
      if (newDefaultAcl)
      {
        LL_APPEND(gAcl, newDefaultAcl);

        size_t size = 0;
        uint8_t *payload = NULL;
        if (OC_STACK_OK == AclToCBORPayload(gAcl, &payload, &size))
        {
          if (UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size) == OC_STACK_OK)
          {
            ret = OC_STACK_OK;
          }
          OICFree(payload);
        }
      }
    }
  }

  return ret;
}

OCStackResult SetAclRownerId(const OicUuid_t *newROwner)
{
  OCStackResult ret = OC_STACK_ERROR;
  uint8_t *cborPayload = NULL;
  size_t size = 0;
  OicUuid_t prevId = {.id = {0}};

  if (NULL == newROwner)
  {
    ret = OC_STACK_INVALID_PARAM;
  }
  if (NULL == gAcl)
  {
    ret = OC_STACK_NO_RESOURCE;
  }

  if (newROwner && gAcl)
  {
    memcpy(prevId.id, gAcl->rownerID.id, sizeof(prevId.id));
    memcpy(gAcl->rownerID.id, newROwner->id, sizeof(newROwner->id));

    ret = AclToCBORPayload(gAcl, &cborPayload, &size);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

    ret = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, cborPayload, size);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

    OICFree(cborPayload);
  }

  return ret;

exit:
  OICFree(cborPayload);
  memcpy(gAcl->rownerID.id, prevId.id, sizeof(prevId.id));
  return ret;
}

OCStackResult GetAclRownerId(OicUuid_t *rowneruuid)
{
  OCStackResult retVal = OC_STACK_ERROR;
  if (gAcl)
  {
    *rowneruuid = gAcl->rownerID;
    retVal = OC_STACK_OK;
  }
  return retVal;
}
