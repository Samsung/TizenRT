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

#include <stdlib.h>
#ifdef WITH_ARDUINO
#include <string.h>
#else
#include <strings.h>
#endif
#include <stdint.h>

#include "cainterface.h"
#include "payload_logging.h"
#include "ocstack.h"
#include "ocrandom.h"
#include "base64.h"
#include "ocserverrequest.h"
#include "oic_malloc.h"
#include "ocpayload.h"
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

#ifdef __WITH_DTLS__
#include "global.h"
#endif

#define TAG  "SRM-CREDL"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 2048;

/** Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

/** CRED size - Number of mandatory items. */
static const uint8_t CRED_ROOT_MAP_SIZE = 2;
static const uint8_t CRED_MAP_SIZE = 3;


static OicSecCred_t        *gCred = NULL;
static OCResourceHandle    gCredHandle = NULL;

/**
 * This function frees OicSecCred_t object's fields and object itself.
 */
static void FreeCred(OicSecCred_t *cred)
{
  if (NULL == cred)
  {
    OIC_LOG(ERROR, TAG, "Invalid Parameter");
    return;
  }
  //Note: Need further clarification on roleID data type
#if 0
  //Clean roleIds
  OICFree(cred->roleIds);
#endif

  //Clean PublicData
#ifdef __WITH_X509__
  OICFree(cred->publicData.data);
#endif

  //Clean PrivateData
  OICFree(cred->privateData.data);

  //Clean Period
  OICFree(cred->period);

  //Clean Cred node itself
  OICFree(cred);
}

void DeleteCredList(OicSecCred_t *cred)
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

static size_t OicSecCredCount(const OicSecCred_t *secCred)
{
  size_t size = 0;
  for (const OicSecCred_t *cred = secCred; cred; cred = cred->next)
  {
    size++;
  }
  return size;
}

OCStackResult CredToCBORPayload(const OicSecCred_t *credS, uint8_t **cborPayload,
                                size_t *cborSize)
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
    char *subject = NULL;
    if (cred->period)
    {
      mapSize++;
    }
#ifdef __WITH_X509__
    if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->publicData.data)
    {
      mapSize++;
    }
#endif /* __WITH_X509__ */
    if (cred->privateData.data)
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
    ret = ConvertUuidToStr(&cred->subject, &subject);
    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
    cborEncoderResult = cbor_encode_text_string(&credMap, subject, strlen(subject));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject Id Value.");
    OICFree(subject);

    //CredType -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_CREDTYPE_NAME,
                                                strlen(OIC_JSON_CREDTYPE_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Type Tag.");
    cborEncoderResult = cbor_encode_int(&credMap, cred->credType);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Type Value.");

#ifdef __WITH_X509__
    //PublicData -- Not Mandatory
    if (SIGNED_ASYMMETRIC_KEY == cred->credType && cred->publicData.data)
    {
      CborEncoder publicMap;
      const size_t publicMapSize = 2;

      cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_PUBLICDATA_NAME,
                                                  strlen(OIC_JSON_PUBLICDATA_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PublicData Tag.");

      cborEncoderResult = cbor_encoder_create_map(&credMap, &publicMap, publicMapSize);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PublicData Map");

      cborEncoderResult = cbor_encode_text_string(&publicMap, OIC_JSON_DATA_NAME,
                                                  strlen(OIC_JSON_DATA_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Pub Data Tag.");
      cborEncoderResult = cbor_encode_byte_string(&publicMap, cred->publicData.data,
                                                  cred->publicData.len);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Pub Value.");

      // TODO: Need to data strucure modification for OicSecCert_t.
      cborEncoderResult = cbor_encode_text_string(&publicMap, OIC_JSON_ENCODING_NAME,
                                                  strlen(OIC_JSON_ENCODING_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Public Encoding Tag.");
      cborEncoderResult = cbor_encode_text_string(&publicMap, OIC_SEC_ENCODING_RAW,
                                                  strlen(OIC_SEC_ENCODING_RAW));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Public Encoding Value.");

      cborEncoderResult = cbor_encoder_close_container(&credMap, &publicMap);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing PublicData Map.");
    }
#endif /*__WITH_X509__*/
    //PrivateData -- Not Mandatory
    if (cred->privateData.data)
    {
      CborEncoder privateMap;
      const size_t privateMapSize = 2;

      cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_PRIVATEDATA_NAME,
                                                  strlen(OIC_JSON_PRIVATEDATA_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PrivateData Tag.");

      cborEncoderResult = cbor_encoder_create_map(&credMap, &privateMap, privateMapSize);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PrivateData Map");

      cborEncoderResult = cbor_encode_text_string(&privateMap, OIC_JSON_DATA_NAME,
                                                  strlen(OIC_JSON_DATA_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Priv Tag.");
      cborEncoderResult = cbor_encode_byte_string(&privateMap, cred->privateData.data,
                                                  cred->privateData.len);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Priv Value.");

      // TODO: Need to data strucure modification for OicSecKey_t.
      cborEncoderResult = cbor_encode_text_string(&privateMap, OIC_JSON_ENCODING_NAME,
                                                  strlen(OIC_JSON_ENCODING_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Private Encoding Tag.");
      cborEncoderResult = cbor_encode_text_string(&privateMap, OIC_SEC_ENCODING_RAW,
                                                  strlen(OIC_SEC_ENCODING_RAW));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Private Encoding Value.");

      cborEncoderResult = cbor_encoder_close_container(&credMap, &privateMap);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing PrivateData Map.");
    }

    //Period -- Not Mandatory
    if (cred->period)
    {
      cborEncoderResult = cbor_encode_text_string(&credMap, OIC_JSON_PERIOD_NAME,
                                                  strlen(OIC_JSON_PERIOD_NAME));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Name Tag.");
      cborEncoderResult = cbor_encode_text_string(&credMap, cred->period,
                                                  strlen(cred->period));
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Name Value.");
    }


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

  // Close CRED Root Map
  cborEncoderResult = cbor_encoder_close_container(&encoder, &credRootMap);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing CRED Root Map.");

  if (CborNoError == cborEncoderResult)
  {
    OIC_LOG(DEBUG, TAG, "CredToCBORPayload Successed");
    *cborPayload = outPayload;
    *cborSize = encoder.ptr - outPayload;
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
    cborLen += encoder.ptr - encoder.end;
    cborEncoderResult = CborNoError;
    ret = CredToCBORPayload(credS, cborPayload, &cborLen);
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

  // Enter CRED Root Map
  CborValue CredRootMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
  cborFindResult = cbor_value_enter_container(&credCbor, &CredRootMap);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering CRED Root Map.");

  while (cbor_value_is_valid(&CredRootMap))
  {
    char *tagName = NULL;
    size_t len = 0;
    CborType type = cbor_value_get_type(&CredRootMap);
    if (type == CborTextStringType)
    {
      cborFindResult = cbor_value_dup_text_string(&CredRootMap, &tagName, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in CRED Root Map.");
      cborFindResult = cbor_value_advance(&CredRootMap);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in CRED Root Map.");
    }
    if (tagName)
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

          if (1 == credCount)
          {
            cred = headCred;
          }
          else
          {
            cred = (OicSecCred_t *) OICCalloc(1, sizeof(OicSecCred_t));
            OicSecCred_t *temp = headCred;
            while (temp->next)
            {
              temp = temp->next;
            }
            temp->next = cred;
          }

          VERIFY_NON_NULL(TAG, cred, ERROR);

          while (cbor_value_is_valid(&credMap))
          {
            char *name = NULL;
            CborType type = cbor_value_get_type(&credMap);
            if (type == CborTextStringType)
            {
              cborFindResult = cbor_value_dup_text_string(&credMap, &name, &len, NULL);
              VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in CRED Map.");
              cborFindResult = cbor_value_advance(&credMap);
              VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in CRED Map.");
            }
            if (name)
            {
              //credid
              if (strcmp(name, OIC_JSON_CREDID_NAME)  == 0)
              {
                cborFindResult = cbor_value_get_uint64(&credMap, (uint64_t *) &cred->credId);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CredId.");
              }
              // subjectid
              if (strcmp(name, OIC_JSON_SUBJECTID_NAME)  == 0)
              {
                char *subjectid = NULL;
                cborFindResult = cbor_value_dup_text_string(&credMap, &subjectid, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subjectid Value.");
                ret = ConvertStrToUuid(subjectid, &cred->subject);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                OICFree(subjectid);
              }
              // credtype
              if (strcmp(name, OIC_JSON_CREDTYPE_NAME)  == 0)
              {
#ifdef __TINYARA__
                /* uint64_t temp_credType=0;
                  cborFindResult = cbor_value_get_uint64(&credMap, &temp_credType);
                cred->credType = temp_credType;        */
                cborFindResult = cbor_value_get_int(&credMap, (int *) &cred->credType);
#else
                cborFindResult = cbor_value_get_uint64(&credMap, (uint64_t *) &cred->credType);
#endif
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CredType.");
              }
              // privatedata
              if (strcmp(name, OIC_JSON_PRIVATEDATA_NAME)  == 0)
              {
                CborValue privateMap = { .parser = NULL };
                cborFindResult = cbor_value_enter_container(&credMap, &privateMap);

                while (cbor_value_is_valid(&privateMap))
                {
                  char *privname = NULL;
                  CborType type = cbor_value_get_type(&privateMap);
                  if (type == CborTextStringType)
                  {
                    cborFindResult = cbor_value_dup_text_string(&privateMap, &privname,
                                                                &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get text");
                    cborFindResult = cbor_value_advance(&privateMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
                  }
                  if (privname)
                  {
                    // PrivateData::privdata -- Mandatory
                    if (strcmp(privname, OIC_JSON_DATA_NAME) == 0)
                    {
                      cborFindResult = cbor_value_dup_byte_string(&privateMap, &cred->privateData.data,
                                                                  &cred->privateData.len, NULL);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PrivateData.");
                    }
                    // PrivateData::encoding -- Mandatory
                    if (strcmp(privname, OIC_JSON_ENCODING_NAME) == 0)
                    {
                      // TODO: Need to update data structure, just ignore encoding value now.
                    }
                  }
                  if (cbor_value_is_valid(&privateMap))
                  {
                    cborFindResult = cbor_value_advance(&privateMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing privatedata Map.");
                  }
                  OICFree(privname);
                }

              }
#ifdef __WITH_X509__
              if (strcmp(name, OIC_JSON_PUBLICDATA_NAME)  == 0)
              {
                CborValue pubMap = { .parser = NULL };
                cborFindResult = cbor_value_enter_container(&credMap, &pubMap);

                while (cbor_value_is_valid(&pubMap))
                {
                  char *pubname = NULL;
                  CborType type = cbor_value_get_type(&pubMap);
                  if (type == CborTextStringType)
                  {
                    cborFindResult = cbor_value_dup_text_string(&pubMap, &pubname,
                                                                &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get text");
                    cborFindResult = cbor_value_advance(&pubMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
                  }
                  if (pubname)
                  {
                    // PrivateData::privdata -- Mandatory
                    if (strcmp(pubname, OIC_JSON_DATA_NAME) == 0)
                    {
                      cborFindResult = cbor_value_dup_byte_string(&pubMap, &cred->publicData.data,
                                                                  &cred->publicData.len, NULL);
                      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PubData.");
                    }
                    // PublicData::encoding -- Mandatory
                    if (strcmp(pubname, OIC_JSON_ENCODING_NAME) == 0)
                    {
                      // TODO: Need to update data structure, just ignore encoding value now.
                    }
                  }
                  if (cbor_value_is_valid(&pubMap))
                  {
                    cborFindResult = cbor_value_advance(&pubMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing publicdata Map.");
                  }
                  OICFree(pubname);
                }
              }
#endif  //__WITH_X509__

              if (0 == strcmp(OIC_JSON_PERIOD_NAME, name))
              {
                cborFindResult = cbor_value_dup_text_string(&credMap, &cred->period, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Period.");
              }

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
      if (strcmp(tagName, OIC_JSON_ROWNERID_NAME)  == 0)
      {
        char *stRowner = NULL;
        cborFindResult = cbor_value_dup_text_string(&CredRootMap, &stRowner, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Rownerid Value.");

        ret = ConvertStrToUuid(stRowner, &headCred->rownerID);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        OICFree(stRowner);
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

OicSecCred_t *GenerateCredential(const OicUuid_t *subject, OicSecCredType_t credType,
                                 const OicSecCert_t *publicData, const OicSecKey_t *privateData,
                                 const OicUuid_t *rownerID)
{
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

#ifdef __WITH_X509__
  if (publicData && publicData->data)
  {
    cred->publicData.data = (uint8_t *)OICCalloc(1, publicData->len);
    VERIFY_NON_NULL(TAG, cred->publicData.data, ERROR);
    memcpy(cred->publicData.data, publicData->data, publicData->len);
    cred->publicData.len = publicData->len;
  }
#endif // __WITH_X509__

  if (privateData && privateData->data)
  {
    cred->privateData.data = (uint8_t *)OICCalloc(1, privateData->len);
    VERIFY_NON_NULL(TAG, cred->privateData.data, ERROR);
    memcpy(cred->privateData.data, privateData->data, privateData->len);
    cred->privateData.len = privateData->len;
  }

  VERIFY_NON_NULL(TAG, rownerID, ERROR);
  memcpy(&cred->rownerID, rownerID, sizeof(OicUuid_t));

  ret = OC_STACK_OK;
exit:
  if (OC_STACK_OK != ret)
  {
    DeleteCredList(cred);
    cred = NULL;
  }
  return cred;
}

static bool UpdatePersistentStorage(const OicSecCred_t *cred)
{
  bool ret = false;

  // Convert Cred data into JSON for update to persistent storage
  if (cred)
  {
    uint8_t *payload = NULL;
    size_t size = 0;
    OCStackResult res = CredToCBORPayload(cred, &payload, &size);
    if ((OC_STACK_OK == res) && payload)
    {
      if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_CRED_NAME, payload, size))
      {
        ret = true;
      }
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
static int CmpCredId(const OicSecCred_t *first, const OicSecCred_t *second)
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
static OicSecCred_t *GetCredDefault()
{
  // TODO:Update it when we finalize the default info.
  return NULL;
}

OCStackResult AddCredential(OicSecCred_t *newCred)
{
  OCStackResult ret = OC_STACK_ERROR;
  VERIFY_SUCCESS(TAG, NULL != newCred, ERROR);

  //Assigning credId to the newCred
  newCred->credId = GetCredId();
  VERIFY_SUCCESS(TAG, newCred->credId != 0, ERROR);

  //Append the new Cred to existing list
  LL_APPEND(gCred, newCred);

  if (UpdatePersistentStorage(gCred))
  {
    ret = OC_STACK_OK;
  }

exit:
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

#ifdef __WITH_DTLS__
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
static bool FillPrivateDataOfOwnerPSK(OicSecCred_t *receviedCred, const CAEndpoint_t *ownerAddr,
                                      const OicSecDoxm_t *doxm)
{
  //Derive OwnerPSK locally
  const char *oxmLabel = GetOxmString(doxm->oxmSel);
  VERIFY_NON_NULL(TAG, oxmLabel, ERROR);

  uint8_t ownerPSK[OWNER_PSK_LENGTH_128] = {0};
  CAResult_t pskRet = CAGenerateOwnerPSK(ownerAddr,
                                         (uint8_t *)oxmLabel, strlen(oxmLabel),
                                         doxm->owner.id, sizeof(doxm->owner.id),
                                         doxm->deviceID.id, sizeof(doxm->deviceID.id),
                                         ownerPSK, OWNER_PSK_LENGTH_128);
  VERIFY_SUCCESS(TAG, pskRet == CA_STATUS_OK, ERROR);

  OIC_LOG(DEBUG, TAG, "OwnerPSK dump :");
  OIC_LOG_BUFFER(DEBUG, TAG, ownerPSK, OWNER_PSK_LENGTH_128);

  //Generate owner credential based on recevied credential information
  receviedCred->privateData.data = (uint8_t *)OICCalloc(1, OWNER_PSK_LENGTH_128);
  VERIFY_NON_NULL(TAG, receviedCred->privateData.data, ERROR);
  receviedCred->privateData.len = OWNER_PSK_LENGTH_128;
  memcpy(receviedCred->privateData.data, ownerPSK, OWNER_PSK_LENGTH_128);

  OIC_LOG(INFO, TAG, "PrivateData of OwnerPSK was calculated successfully");

  //Verify OwnerPSK information
  return (memcmp(&(receviedCred->subject), &(doxm->owner), sizeof(OicUuid_t)) == 0 &&
          receviedCred->credType == SYMMETRIC_PAIR_WISE_KEY);
exit:
  //receviedCred->privateData.data will be deallocated when deleting credential.
  return false;
}

#endif //__WITH_DTLS__

static OCEntityHandlerResult HandlePutRequest(const OCEntityHandlerRequest *ehRequest)
{
  OCEntityHandlerResult ret = OC_EH_ERROR;
  OIC_LOG(DEBUG, TAG, "HandleCREDPutRequest IN");

  //Get binary representation of cbor
  OicSecCred_t *cred  = NULL;
  uint8_t *payload = (((OCSecurityPayload *)ehRequest->payload)->securityData);
  size_t size = (((OCSecurityPayload *)ehRequest->payload)->payloadSize);
  OCStackResult res = CBORPayloadToCred(payload, size, &cred);
  if (res == OC_STACK_OK)
  {
#ifdef __WITH_DTLS__
    OicUuid_t emptyUuid = {.id = {0}};
    const OicSecDoxm_t *doxm = GetDoxmResourceData();
    if (false == doxm->owned && memcmp(&(doxm->owner), &emptyUuid, sizeof(OicUuid_t)) != 0)
    {
      //in case of owner PSK
      switch (cred->credType)
      {
        case SYMMETRIC_PAIR_WISE_KEY:
        {
          OCServerRequest *request = (OCServerRequest *)ehRequest->requestHandle;
#ifdef __TINYARA__
          /*
              OCTransportAdapter      adapter;
              OCTransportFlags        flags;
              uint16_t                port;
              char                    addr[MAX_ADDR_STR_SIZE];
              uint32_t                interface;
          #if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
              char                    routeData[MAX_ADDR_STR_SIZE]; //destination GatewayID:ClientId
          #endif
          */
          CAEndpoint_t *ep_addr = (CAEndpoint_t *)malloc(sizeof(CAEndpoint_t));
          ep_addr->adapter =   request->devAddr.adapter;
          ep_addr->flags =   request->devAddr.flags;
          ep_addr->port  =   request->devAddr.port;
          memcpy(ep_addr->addr, request->devAddr.addr, MAX_ADDR_STR_SIZE_CA);
          ep_addr->interface  =   request->devAddr.interface;
#if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
          memcpy(ep_addr->routeData, request->devAddr.routeData, MAX_ADDR_STR_SIZE_CA);
#endif
          if (FillPrivateDataOfOwnerPSK(cred, ep_addr, doxm))
#else
          if (FillPrivateDataOfOwnerPSK(cred, (CAEndpoint_t *)&request->devAddr, doxm))
#endif
          {
            if (OC_STACK_RESOURCE_DELETED == RemoveCredential(&cred->subject))
            {
              OIC_LOG(WARNING, TAG, "The credential with the same subject ID was detected!");
            }

            OIC_LOG(ERROR, TAG, "OwnerPSK was generated successfully.");
            if (OC_STACK_OK == AddCredential(cred))
            {
              ret = OC_EH_RESOURCE_CREATED;
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

          if (OC_EH_RESOURCE_CREATED == ret)
          {
            /**
             * in case of random PIN based OxM,
             * revert get_psk_info callback of tinyDTLS to use owner credential.
             */
            if (OIC_RANDOM_DEVICE_PIN == doxm->oxmSel)
            {
              OicUuid_t emptyUuid = { .id = {0}};
              SetUuidForRandomPinOxm(&emptyUuid);

              if (CA_STATUS_OK != CARegisterDTLSCredentialsHandler(GetDtlsPskCredentials))
              {
                OIC_LOG(ERROR, TAG, "Failed to revert DTLS credential handler.");
                ret = OC_EH_ERROR;
                break;
              }
            }

            //Select cipher suite to use owner PSK
            if (CA_STATUS_OK != CAEnableAnonECDHCipherSuite(false))
            {
              OIC_LOG(ERROR, TAG, "Failed to disable anonymous cipher suite");
              ret = OC_EH_ERROR;
            }
            else
            {
              OIC_LOG(INFO, TAG, "Anonymous cipher suite is DISABLED");
            }

            if (CA_STATUS_OK !=
                CASelectCipherSuite(TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA_256))
            {
              OIC_LOG(ERROR, TAG, "Failed to select cipher suite");
              ret = OC_EH_ERROR;
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
          OIC_LOG(WARNING, TAG, "Unknow credential type for owner credential.");
          ret = OC_EH_ERROR;
          break;
        }
      }

      if (OC_EH_RESOURCE_CREATED != ret)
      {
        /*
          * If some error is occured while ownership transfer,
          * ownership transfer related resource should be revert back to initial status.
          */
        RestoreDoxmToInitState();
        RestorePstatToInitState();
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
      ret = (OC_STACK_OK == AddCredential(cred)) ? OC_EH_RESOURCE_CREATED : OC_EH_ERROR;
    }
#else //not __WITH_DTLS__
    /*
     * If the post request credential has credId, it will be
     * discarded and the next available credId will be assigned
     * to it before getting appended to the existing credential
     * list and updating svr database.
     */
    ret = (OC_STACK_OK == AddCredential(cred)) ? OC_EH_RESOURCE_CREATED : OC_EH_ERROR;
#endif//__WITH_DTLS__
  }

  if (OC_EH_RESOURCE_CREATED != ret)
  {
    if (OC_STACK_OK != RemoveCredential(&cred->subject))
    {
      OIC_LOG(WARNING, TAG, "Failed to remove the invalid credential");
    }
    FreeCred(cred);
  }
  OIC_LOG(DEBUG, TAG, "HandleCREDPutRequest OUT");
  return ret;
}

static OCEntityHandlerResult HandlePostRequest(const OCEntityHandlerRequest *ehRequest)
{
  OCEntityHandlerResult ret = OC_EH_ERROR;

  //Get binary representation of CBOR
  OicSecCred_t *cred  = NULL;
  uint8_t *payload = ((OCSecurityPayload *)ehRequest->payload)->securityData;
  size_t size = ((OCSecurityPayload *)ehRequest->payload)->payloadSize;
  OCStackResult res = CBORPayloadToCred(payload, size, &cred);
  if ((OC_STACK_OK == res) && cred)
  {
    //If the Post request credential has credId, it will be
    //discarded and the next available credId will be assigned
    //to it before getting appended to the existing credential
    //list and updating svr database.
    ret = (OC_STACK_OK == AddCredential(cred)) ? OC_EH_RESOURCE_CREATED : OC_EH_ERROR;
  }

  return ret;
}

static OCEntityHandlerResult HandleDeleteRequest(const OCEntityHandlerRequest *ehRequest)
{
  OIC_LOG(DEBUG, TAG, "Processing CredDeleteRequest");

  OCEntityHandlerResult ehRet = OC_EH_ERROR;

  if (NULL == ehRequest->query)
  {
    return ehRet;
  }

  OicParseQueryIter_t parseIter = { .attrPos = NULL };
  OicUuid_t subject = {.id = {0}};

  //Parsing REST query to get the subject
  ParseQueryIterInit((unsigned char *)ehRequest->query, &parseIter);
  while (GetNextQuery(&parseIter))
  {
    if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_SUBJECTID_NAME,
                    parseIter.attrLen) == 0)
    {
      OCStackResult ret = ConvertStrToUuid((const char *)parseIter.valPos, &subject);
      VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
    }
  }

  if (OC_STACK_RESOURCE_DELETED == RemoveCredential(&subject))
  {
    ehRet = OC_EH_RESOURCE_DELETED;
  }

exit:
  return ehRet;
}

OCEntityHandlerResult CredEntityHandler(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest *ehRequest,
                                        void *callbackParameter)
{
  (void)callbackParameter;
  OCEntityHandlerResult ret = OC_EH_ERROR;

  if (!ehRequest)
  {
    return OC_EH_ERROR;
  }
  if (flag & OC_REQUEST_FLAG)
  {
    OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
    //TODO :  Handle PUT/DEL methods
    switch (ehRequest->method)
    {
      case OC_REST_GET:
        ret = OC_EH_FORBIDDEN;
        break;
      case OC_REST_PUT:
        ret = HandlePutRequest(ehRequest);
        break;
      case OC_REST_POST:
        ret = HandlePostRequest(ehRequest);
        break;
      case OC_REST_DELETE:
        ret = HandleDeleteRequest(ehRequest);
        break;
      default:
        ret = OC_EH_ERROR;
        break;
    }
  }

  //Send payload to request originator
  ret = (SendSRMResponse(ehRequest, ret, NULL, 0) == OC_STACK_OK) ?
        ret : OC_EH_ERROR;

  return ret;
}

OCStackResult CreateCredResource()
{
  OCStackResult ret = OCCreateResource(&gCredHandle,
                                       OIC_RSRC_TYPE_SEC_CRED,
                                       OIC_MI_DEF,
                                       OIC_RSRC_CRED_URI,
                                       CredEntityHandler,
                                       NULL,
                                       OC_RES_PROP_NONE);

  if (OC_STACK_OK != ret)
  {
    OIC_LOG(FATAL, TAG, "Unable to instantiate Cred resource");
    DeInitCredResource();
  }
  return ret;
}

OCStackResult InitCredResource()
{
  OCStackResult ret = OC_STACK_ERROR;

  //Read Cred resource from PS
  uint8_t *data = NULL;
  size_t size = 0;
  ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_CRED_NAME, &data, &size);
  // If database read failed
  if (ret != OC_STACK_OK)
  {
    OIC_LOG(DEBUG, TAG, "ReadSVDataFromPS failed");
  }
  if (data)
  {
    // Read ACL resource from PS
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
  //Instantiate 'oic.sec.cred'
  ret = CreateCredResource();
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

const OicSecCred_t *GetCredResourceData(const OicUuid_t *subject)
{
  OicSecCred_t *cred = NULL;

  if (NULL == subject)
  {
    return NULL;
  }

  LL_FOREACH(gCred, cred)
  {
    if (memcmp(cred->subject.id, subject->id, sizeof(subject->id)) == 0)
    {
      return cred;
    }
  }
  return NULL;
}


#if defined(__WITH_DTLS__)
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
      OicUuid_t deviceID = {.id = {}};
      // Retrieve Device ID from doxm resource
      if (OC_STACK_OK != GetDoxmDeviceID(&deviceID))
      {
        OIC_LOG(ERROR, TAG, "Unable to retrieve doxm Device ID");
        return ret;
      }

      if (result_length < sizeof(deviceID.id))
      {
        OIC_LOG(ERROR, TAG, "Wrong value for result_length");
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
            if (IOTVTICAL_VALID_ACCESS != IsRequestWithinValidTime(cred->period, NULL))
            {
              OIC_LOG(INFO, TAG, "Credentials are expired.");
              ret = -1;
              return ret;
            }
          }

          // Copy PSK.
          result_length = cred->privateData.len;
          memcpy(result, cred->privateData.data, result_length);
          return result_length;
        }
      }
    }
    break;

    default:
    {
      OIC_LOG(ERROR, TAG, "Wrong value passed for CADtlsPskCredType_t.");
      ret = -1;
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
OCStackResult AddTmpPskWithPIN(const OicUuid_t *tmpSubject, OicSecCredType_t credType,
                               const char *pin, size_t pinSize,
                               const OicUuid_t *rownerID, OicUuid_t *tmpCredSubject)
{
  OCStackResult ret = OC_STACK_ERROR;
  OIC_LOG(DEBUG, TAG, "AddTmpPskWithPIN IN");

  if (NULL == tmpSubject || NULL == pin || 0 == pinSize || NULL == tmpCredSubject)
  {
    return OC_STACK_INVALID_PARAM;
  }

  uint8_t privData[OWNER_PSK_LENGTH_128] = {0,};
  OicSecKey_t privKey = {privData, OWNER_PSK_LENGTH_128};
  OicSecCred_t *cred = NULL;
  int dtlsRes = DeriveCryptoKeyFromPassword((const unsigned char *)pin, pinSize, rownerID->id,
                                            UUID_LENGTH, PBKDF_ITERATIONS,
                                            OWNER_PSK_LENGTH_128, privData);
  VERIFY_SUCCESS(TAG, (0 == dtlsRes) , ERROR);

  cred = GenerateCredential(tmpSubject, credType, NULL,
                            &privKey, rownerID);
  if (NULL == cred)
  {
    OIC_LOG(ERROR, TAG, "GeneratePskWithPIN() : Failed to generate credential");
    return OC_STACK_ERROR;
  }

  memcpy(tmpCredSubject->id, cred->subject.id, UUID_LENGTH);

  ret = AddCredential(cred);
  if (OC_STACK_OK != ret)
  {
    RemoveCredential(tmpSubject);
    OIC_LOG(ERROR, TAG, "GeneratePskWithPIN() : Failed to add credential");
  }
  OIC_LOG(DEBUG, TAG, "AddTmpPskWithPIN OUT");

exit:
  return ret;
}

#endif /* __WITH_DTLS__ */
#ifdef __WITH_X509__
#define CERT_LEN_PREFIX (3)
#define BYTE_SIZE (8) //bits
#define PUB_KEY_X_COORD ("x")
#define PUB_KEY_Y_COORD ("y")
#define CERTIFICATE ("x5c")
#define PRIVATE_KEY ("d")

static uint32_t parseCertPrefix(uint8_t *prefix)
{
  uint32_t res = 0;
  if (NULL != prefix)
  {
    for (int i = 0; i < CERT_LEN_PREFIX; ++i)
    {
      res |= (((uint32_t) prefix[i]) << ((CERT_LEN_PREFIX - 1 - i) * BYTE_SIZE));
    }
  }
  return res;
}

static OCStackResult GetCAPublicKeyData(CADtlsX509Creds_t *credInfo)
{
  OCStackResult ret = OC_STACK_ERROR;
  uint8_t *ccPtr = credInfo->certificateChain;
  for (uint8_t i = 0; i < credInfo->chainLen - 1; ++i)
  {
    ccPtr += CERT_LEN_PREFIX + parseCertPrefix(ccPtr);
  }

  ByteArray cert = { .data = ccPtr + CERT_LEN_PREFIX, .len = parseCertPrefix(ccPtr) };
  CertificateX509 certStruct;

  VERIFY_SUCCESS(TAG, PKI_SUCCESS == DecodeCertificate(cert, &certStruct), ERROR);

  INC_BYTE_ARRAY(certStruct.pubKey, 2);

  memcpy(credInfo->rootPublicKeyX, certStruct.pubKey.data, PUBLIC_KEY_SIZE / 2);
  memcpy(credInfo->rootPublicKeyY, certStruct.pubKey.data + PUBLIC_KEY_SIZE / 2, PUBLIC_KEY_SIZE / 2);

  ret = OC_STACK_OK;
exit:
  return ret;
}

int GetDtlsX509Credentials(CADtlsX509Creds_t *credInfo)
{
  int ret = 1;
  VERIFY_NON_NULL(TAG, credInfo, ERROR);
  if (NULL == gCred)
  {
    VERIFY_SUCCESS(TAG, OC_STACK_OK == InitCredResource(), ERROR);
  }

  OicSecCred_t *cred = NULL;
  LL_SEARCH_SCALAR(gCred, cred, credType, SIGNED_ASYMMETRIC_KEY);
  VERIFY_NON_NULL(TAG, cred, ERROR);

  if (cred->publicData.len > MAX_CERT_MESSAGE_LEN || cred->privateData.len > PRIVATE_KEY_SIZE)
  {
    goto exit;
  }
  credInfo->chainLen = 2;
  memcpy(credInfo->certificateChain, cred->publicData.data, cred->publicData.len);
  memcpy(credInfo->devicePrivateKey, cred->privateData.data, cred->privateData.len);
  credInfo->certificateChainLen = cred->publicData.len;
  GetCAPublicKeyData(credInfo);
  ret = 0;

exit:

  return ret;
}
#undef CERT_LEN_PREFIX
#endif /* __WITH_X509__ */

OCStackResult SetCredRownerId(const OicUuid_t *newROwner)
{
  OCStackResult ret = OC_STACK_ERROR;
  uint8_t *cborPayload = NULL;
  size_t size = 0;
  OicUuid_t prevId = {.id = {0}};

  if (NULL == newROwner)
  {
    ret = OC_STACK_INVALID_PARAM;
  }
  if (NULL == gCred)
  {
    ret = OC_STACK_NO_RESOURCE;
  }

  if (newROwner && gCred)
  {
    memcpy(prevId.id, gCred->rownerID.id, sizeof(prevId.id));
    memcpy(gCred->rownerID.id, newROwner->id, sizeof(newROwner->id));

    ret = CredToCBORPayload(gCred, &cborPayload, &size);
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
