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
#define __STDC_LIMIT_MACROS
#endif

#include <stdlib.h>
#include <string.h>

#include "cainterface.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "payload_logging.h"
#include "resourcemanager.h"
#include "secureresourcemanager.h"
#include "srmresourcestrings.h"
#include "srmutility.h"

#define TAG  "SRM-PSI"

//SVR database buffer block size
const size_t DB_FILE_SIZE_BLOCK = 1023;

/**
 * Gets the Secure Virtual Database size
 *
 * @param ps - pointer of OCPersistentStorage for the Secure Virtual Resource(s)
 *
 * @return size_t - total size of the SVR database
 */
static size_t GetSVRDatabaseSize(const OCPersistentStorage *ps)
{
  if (!ps)
  {
    return 0;
  }
  size_t size = 0;
  char buffer[DB_FILE_SIZE_BLOCK];  // can not initialize with declaration
  // but maybe not needed to initialize
  FILE *fp = ps->open(SVR_DB_DAT_FILE_NAME, "rb");
  if (fp)
  {
    size_t bytesRead = 0;
    do
    {
      bytesRead = ps->read(buffer, 1, DB_FILE_SIZE_BLOCK, fp);
      size += bytesRead;
    }
    while (bytesRead);
    ps->close(fp);
  }
  return size;
}

/**
 * Gets the Secure Virtual Database from the Persistent Storage
 *
 * @param rsrcName - pointer of character string for the SVR name (e.g. "acl")
 * @param data - pointer of the returned Secure Virtual Resource(s)
 * @param size - pointer of the returned size of Secure Virtual Resource(s)
 *
 * @return OCStackResult - result of getting Secure Virtual Resource(s)
 */
OCStackResult GetSecureVirtualDatabaseFromPS(const char *rsrcName, uint8_t **data, size_t *size)
{
  OIC_LOG(DEBUG, TAG, "GetSecureVirtualDatabaseFromPS IN");
  if (!data || *data || !size)
  {
    return OC_STACK_INVALID_PARAM;
  }

  FILE *fp = NULL;
  uint8_t *fsData = NULL;
  size_t fileSize = 0;
  OCStackResult ret = OC_STACK_ERROR;

  OCPersistentStorage *ps = SRMGetPersistentStorageHandler();
  VERIFY_NON_NULL(TAG, ps, ERROR);

  fileSize = GetSVRDatabaseSize(ps);
  OIC_LOG_V(DEBUG, TAG, "File Read Size: %zu", fileSize);
  if (fileSize)
  {
    fsData = (uint8_t *) OICCalloc(1, fileSize);
    VERIFY_NON_NULL(TAG, fsData, ERROR);

    fp = ps->open(SVR_DB_DAT_FILE_NAME, "rb");
    VERIFY_NON_NULL(TAG, fp, ERROR);
    if (ps->read(fsData, 1, fileSize, fp) == fileSize)
    {
      if (rsrcName)
      {
        CborParser parser;  // will be initialized in |cbor_parser_init|
        CborValue cbor;     // will be initialized in |cbor_parser_init|
        cbor_parser_init(fsData, fileSize, 0, &parser, &cbor);
        CborValue cborValue = {0};
        CborError cborFindResult = cbor_value_map_find_value(&cbor, rsrcName, &cborValue);
        if (CborNoError == cborFindResult && cbor_value_is_byte_string(&cborValue))
        {
          cborFindResult = cbor_value_dup_byte_string(&cborValue, data, size, NULL);
          VERIFY_SUCCESS(TAG, CborNoError == cborFindResult, ERROR);
          ret = OC_STACK_OK;
        }
        // in case of |else (...)|, svr_data not found
      }
      // return everything in case rsrcName is NULL
      else
      {
        *size = fileSize;
        *data = (uint8_t *) OICCalloc(1, fileSize);
        VERIFY_NON_NULL(TAG, *data, ERROR);
        memcpy(*data, fsData, fileSize);
        ret = OC_STACK_OK;
      }
    }
  }
  OIC_LOG(DEBUG, TAG, "GetSecureVirtualDatabaseFromPS OUT");

exit:
  if (fp)
  {
    ps->close(fp);
  }
  OICFree(fsData);
  return ret;
}

/**
 * Updates the Secure Virtual Resource(s) into the Persistent Storage.
 * This function stores cbor-payload of each resource by appending resource name,
 * and empty payload implies deleting the value
 *
 * @param rsrcName - pointer of character string for the SVR name (e.g. "acl")
 * @param psPayload - pointer of the updated Secure Virtual Resource(s)
 * @param psSize - the updated size of Secure Virtual Resource(s)
 *
 * @return OCStackResult - result of updating Secure Virtual Resource(s)
 */
OCStackResult UpdateSecureResourceInPS(const char *rsrcName, const uint8_t *psPayload, size_t psSize)
{
  OIC_LOG(DEBUG, TAG, "UpdateSecureResourceInPS IN");
  if (!rsrcName)
  {
    return OC_STACK_INVALID_PARAM;
  }

  size_t dbSize = 0;
  size_t outSize = 0;
  uint8_t *dbData = NULL;
  uint8_t *outPayload = NULL;

  uint8_t *aclCbor = NULL;
  uint8_t *pstatCbor = NULL;
  uint8_t *doxmCbor = NULL;
  uint8_t *amaclCbor = NULL;
  uint8_t *svcCbor = NULL;
  uint8_t *credCbor = NULL;
  uint8_t *pconfCbor = NULL;

  int64_t cborEncoderResult = CborNoError;
  OCStackResult ret = GetSecureVirtualDatabaseFromPS(NULL, &dbData, &dbSize);
  if (dbData && dbSize)
  {
    size_t aclCborLen = 0;
    size_t pstatCborLen = 0;
    size_t doxmCborLen = 0;
    size_t amaclCborLen = 0;
    size_t svcCborLen = 0;
    size_t credCborLen = 0;
    size_t pconfCborLen = 0;

    // Gets each secure virtual resource from persistent storage
    // this local scoping intended, for destroying large cbor instances after use
    {
      CborParser parser;  // will be initialized in |cbor_parser_init|
      CborValue cbor;     // will be initialized in |cbor_parser_init|
      cbor_parser_init(dbData, dbSize, 0, &parser, &cbor);
      CborValue curVal = {0};
      CborError cborFindResult = CborNoError;

      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_ACL_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &aclCbor, &aclCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ACL Name Value.");
      }
      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_PSTAT_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &pstatCbor, &pstatCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PSTAT Name Value.");
      }
      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_DOXM_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &doxmCbor, &doxmCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult,  "Failed Finding DOXM Name Value.");
      }
      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_AMACL_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &amaclCbor, &amaclCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding AMACL Name Value.");
      }
      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_SVC_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &svcCbor, &svcCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding SVC Name Value.");
      }
      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_CRED_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &credCbor, &credCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CRED Name Value.");
      }
      cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_PCONF_NAME, &curVal);
      if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
      {
        cborFindResult = cbor_value_dup_byte_string(&curVal, &pconfCbor, &pconfCborLen, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PCONF Name Value.");
      }
    }

    // Updates the added |psPayload| with the existing secure virtual resource(s)
    // this local scoping intended, for destroying large cbor instances after use
    {
      size_t size = aclCborLen + pstatCborLen + doxmCborLen + amaclCborLen
                    + svcCborLen + credCborLen + pconfCborLen + psSize + 255;
      // This added '255' is arbitrary value that is added to cover the name of the resource, map addition and ending

      outPayload = (uint8_t *) OICCalloc(1, size);
      VERIFY_NON_NULL(TAG, outPayload, ERROR);
      CborEncoder encoder;  // will be initialized in |cbor_parser_init|
      cbor_encoder_init(&encoder, outPayload, size, 0);
      CborEncoder secRsrc;  // will be initialized in |cbor_encoder_create_map|
      cborEncoderResult |= cbor_encoder_create_map(&encoder, &secRsrc, CborIndefiniteLength);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PS Map.");

      if (psPayload && psSize)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, rsrcName, strlen(rsrcName));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value Tag");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, psPayload, psSize);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value.");
      }
      if (strcmp(OIC_JSON_ACL_NAME, rsrcName) && aclCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_ACL_NAME, strlen(OIC_JSON_ACL_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, aclCbor, aclCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Value.");
      }
      if (strcmp(OIC_JSON_PSTAT_NAME, rsrcName) && pstatCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_PSTAT_NAME, strlen(OIC_JSON_PSTAT_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, pstatCbor, pstatCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Value.");
      }
      if (strcmp(OIC_JSON_DOXM_NAME, rsrcName) && doxmCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_DOXM_NAME, strlen(OIC_JSON_DOXM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, doxmCbor, doxmCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Value.");
      }
      if (strcmp(OIC_JSON_AMACL_NAME, rsrcName) && amaclCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_AMACL_NAME, strlen(OIC_JSON_AMACL_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Amacl Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, amaclCbor, amaclCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Amacl Value.");
      }
      if (strcmp(OIC_JSON_SVC_NAME, rsrcName) && svcCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_SVC_NAME, strlen(OIC_JSON_SVC_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SVC Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, svcCbor, svcCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SVC Value.");
      }
      if (strcmp(OIC_JSON_CRED_NAME, rsrcName) && credCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_CRED_NAME, strlen(OIC_JSON_CRED_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, credCbor, credCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Value.");
      }
      if (strcmp(OIC_JSON_PCONF_NAME, rsrcName) && pconfCborLen)
      {
        cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_PCONF_NAME, strlen(OIC_JSON_PCONF_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Pconf Name.");
        cborEncoderResult |= cbor_encode_byte_string(&secRsrc, pconfCbor, pconfCborLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Pconf Value.");
      }

      cborEncoderResult |= cbor_encoder_close_container(&encoder, &secRsrc);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Array.");
      outSize = encoder.ptr - outPayload;
    }
  }
  else if (psPayload && psSize)
  {
    size_t size = psSize + 255;
    // This added '255' is arbitrary value that is added to cover the name of the resource, map addition and ending

    outPayload = (uint8_t *) OICCalloc(1, size);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    CborEncoder encoder;  // will be initialized in |cbor_parser_init|
    cbor_encoder_init(&encoder, outPayload, size, 0);
    CborEncoder secRsrc;  // will be initialized in |cbor_encoder_create_map|
    cborEncoderResult |= cbor_encoder_create_map(&encoder, &secRsrc, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PS Map.");

    cborEncoderResult |= cbor_encode_text_string(&secRsrc, rsrcName, strlen(rsrcName));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value Tag");
    cborEncoderResult |= cbor_encode_byte_string(&secRsrc, psPayload, psSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value.");

    cborEncoderResult |= cbor_encoder_close_container(&encoder, &secRsrc);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Array.");
    outSize = encoder.ptr - outPayload;
  }

  if (outPayload && outSize)
  {
    OIC_LOG_V(DEBUG, TAG, "Writting in the file: %zu", outSize);
    OCPersistentStorage *ps = SRMGetPersistentStorageHandler();
    if (ps)
    {
      FILE *fp = ps->open(SVR_DB_DAT_FILE_NAME, "wb");
      if (fp)
      {
        size_t numberItems = ps->write(outPayload, 1, outSize, fp);
        if (outSize == numberItems)
        {
          OIC_LOG_V(DEBUG, TAG, "Written %zu bytes into SVR database file", outSize);
          ret = OC_STACK_OK;
        }
        else
        {
          OIC_LOG_V(ERROR, TAG, "Failed writing %zu in the database", numberItems);
        }
        ps->close(fp);
      }
      else
      {
        OIC_LOG(ERROR, TAG, "File open failed.");
      }
    }
  }

  OIC_LOG(DEBUG, TAG, "UpdateSecureResourceInPS OUT");

exit:
  OICFree(dbData);
  OICFree(outPayload);
  OICFree(aclCbor);
  OICFree(pstatCbor);
  OICFree(doxmCbor);
  OICFree(amaclCbor);
  OICFree(svcCbor);
  OICFree(credCbor);
  OICFree(pconfCbor);
  return ret;
}
