//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "utlist.h"
#include "payload_logging.h"
#include "psinterface.h"
#include "resourcemanager.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "doxmresource.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#ifdef __WITH_X509__
#include "crlresource.h"
#include "crl.h"
#endif /* __WITH_X509__ */

#define TAG  "SRM-CRL"



#define SEPARATOR                   ":"
#define SEPARATOR_LEN               (1)
#define CBOR_CRL_NAME               "\"CRL\""
#define CBOR_CRL_NAME_LEN           (5)
#define OIC_CBOR_CRL_NAME           "crl"
#define OIC_CBOR_CRL_ID             "CRLId"
#define OIC_CBOR_CRL_THIS_UPDATE    "ThisUpdate"
#define OIC_CBOR_CRL_DATA           "CRLData"
#define CRL_DEFAULT_CRL_ID          (1)
#define CRL_DEFAULT_THIS_UPDATE     "150101000000Z"
#define CRL_DEFAULT_CRL_DATA        "-"

static OCResourceHandle     gCrlHandle  = NULL;
static OicSecCrl_t         *gCrl        = NULL;

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching below max cbor size. */
static const uint16_t CBOR_SIZE = 1024;

// Max cbor size payload.
static const uint16_t CBOR_MAX_SIZE = 4400;

// PSTAT Map size - Number of mandatory items
static const uint8_t CRL_MAP_SIZE = 3;

void DeleteCrlBinData(OicSecCrl_t *crl)
{
  if (crl)
  {
    //Clean ThisUpdate
    OICFree(crl->ThisUpdate.data);

    //clean CrlData
    OICFree(crl->CrlData.data);

    //Clean crl itself
    OICFree(crl);
  }
}

OCStackResult CrlToCBORPayload(const OicSecCrl_t *crl, uint8_t **payload, size_t *size)
{
  if (NULL == crl || NULL == payload || NULL != *payload || NULL == size)
  {
    return OC_STACK_INVALID_PARAM;
  }

  size_t cborLen = *size;
  if (0 == cborLen)
  {
    cborLen = CBOR_SIZE;
  }

  *payload = NULL;
  *size = 0;

  OCStackResult ret = OC_STACK_ERROR;

  CborEncoder encoder;
  CborEncoder crlMap;

  CborError cborEncoderResult = CborNoError;

  uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
  VERIFY_NON_NULL(TAG, outPayload, ERROR);
  cbor_encoder_init(&encoder, outPayload, cborLen, 0);

  cborEncoderResult = cbor_encoder_create_map(&encoder, &crlMap, CRL_MAP_SIZE);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create CRL Map");

  //CRLId -- Mandatory
  cborEncoderResult = cbor_encode_text_string(&crlMap, OIC_CBOR_CRL_ID,
                                              strlen(OIC_CBOR_CRL_ID));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add CRL ID");
  cborEncoderResult = cbor_encode_int(&crlMap, crl->CrlId);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add CRL Id value");

  //ThisUpdate -- Mandatory
  cborEncoderResult = cbor_encode_text_string(&crlMap, OIC_CBOR_CRL_THIS_UPDATE,
                                              strlen(OIC_CBOR_CRL_THIS_UPDATE));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add Crl update");
  cborEncoderResult = cbor_encode_byte_string(&crlMap, crl->ThisUpdate.data,
                                              crl->ThisUpdate.len);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add Crl Update value");

  //CRLData -- Mandatory
  cborEncoderResult = cbor_encode_text_string(&crlMap, OIC_CBOR_CRL_DATA,
                                              strlen(OIC_CBOR_CRL_DATA));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add Crl data name");
  cborEncoderResult = cbor_encode_byte_string(&crlMap, crl->CrlData.data,
                                              crl->CrlData.len);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add Crl data value");

  cborEncoderResult = cbor_encoder_close_container(&encoder, &crlMap);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add close Crl map");

  *size = encoder.ptr - outPayload;
  *payload = outPayload;
  ret = OC_STACK_OK;

exit:
  if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
  {
    // reallocate and try again!
    OICFree(outPayload);
    // Since the allocated initial memory failed, double the memory.
    cborLen += encoder.ptr - encoder.end;
    cborEncoderResult = CborNoError;
    ret = CrlToCBORPayload(crl, payload, &cborLen);
  }

  if ((CborNoError != cborEncoderResult) || (OC_STACK_OK != ret))
  {
    OICFree(outPayload);
    outPayload = NULL;
    *payload = NULL;
    *size = 0;
    ret = OC_STACK_ERROR;
  }

  return ret;
}

OCStackResult CBORPayloadToCrl(const uint8_t *cborPayload, const size_t size,
                               OicSecCrl_t **secCrl)
{
  if (NULL == cborPayload || NULL == secCrl || NULL != *secCrl || 0 == size)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OCStackResult ret = OC_STACK_ERROR;
  *secCrl = NULL;

  CborValue crlCbor = {.parser = NULL};
  CborParser parser = {.end = NULL};
  CborError cborFindResult = CborNoError;

  cbor_parser_init(cborPayload, size, 0, &parser, &crlCbor);
  CborValue crlMap = { .parser = NULL};
  OicSecCrl_t *crl = NULL;
  size_t outLen = 0;
  cborFindResult = cbor_value_enter_container(&crlCbor, &crlMap);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter Crl map");

  crl = (OicSecCrl_t *)OICCalloc(1, sizeof(OicSecCrl_t));
  VERIFY_NON_NULL(TAG, crl, ERROR);

  cborFindResult = cbor_value_map_find_value(&crlCbor, OIC_CBOR_CRL_ID, &crlMap);
  if (CborNoError == cborFindResult && cbor_value_is_integer(&crlMap))
  {
    cborFindResult = cbor_value_get_int(&crlMap, (int *) &crl->CrlId);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CrlId.");
  }

  cborFindResult = cbor_value_map_find_value(&crlCbor, OIC_CBOR_CRL_THIS_UPDATE, &crlMap);
  if (CborNoError == cborFindResult && cbor_value_is_byte_string(&crlMap))
  {
    cborFindResult = cbor_value_dup_byte_string(&crlMap,
                                                &crl->ThisUpdate.data, &crl->ThisUpdate.len, NULL);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Byte Array.");
  }
  cborFindResult = cbor_value_map_find_value(&crlCbor, OIC_CBOR_CRL_DATA, &crlMap);
  if (CborNoError == cborFindResult && cbor_value_is_byte_string(&crlMap))
  {
    cborFindResult = cbor_value_dup_byte_string(&crlMap,
                                                &crl->CrlData.data, &crl->CrlData.len, NULL);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Byte Array.");
  }

  *secCrl = crl;
  ret = OC_STACK_OK;
exit:
  if (CborNoError != cborFindResult)
  {
    // PUT/POST CBOR may not have mandatory values set default values.
    if (gCrl)
    {
      OIC_LOG(DEBUG, TAG, "Set default values");
      crl->CrlId = gCrl->CrlId;
      if (crl->ThisUpdate.data)
      {
        OICFree(crl->ThisUpdate.data);
      }
      outLen = gCrl->ThisUpdate.len;
      crl->ThisUpdate.data = (uint8_t *) OICMalloc(outLen);
      if (crl->ThisUpdate.data)
      {
        memcpy(crl->ThisUpdate.data, gCrl->ThisUpdate.data, outLen);
        crl->ThisUpdate.len = outLen;
      }
      else
      {
        crl->ThisUpdate.len = 0;
        OIC_LOG(ERROR, TAG, "Set default failed");
      }
      if (crl->CrlData.data)
      {
        OICFree(crl->CrlData.data);
      }
      outLen = gCrl->CrlData.len;
      crl->CrlData.data = (uint8_t *) OICMalloc(outLen);
      if (crl->CrlData.data && gCrl->CrlData.data)
      {
        memcpy(crl->CrlData.data, gCrl->CrlData.data, outLen);
        crl->CrlData.len = outLen;
      }
      else
      {
        crl->CrlData.len = 0;
        OIC_LOG(ERROR, TAG, "Set default failed");
      }

      *secCrl = crl;
      ret = OC_STACK_OK;
    }
    else
    {
      OIC_LOG(ERROR, TAG, "CBORPayloadToCrl failed");
      DeleteCrlBinData(crl);
      crl = NULL;
      ret = OC_STACK_ERROR;
    }
  }
  return ret;
}

OCStackResult UpdateCRLResource(const OicSecCrl_t *crl)
{
  uint8_t *payload = NULL;
  size_t size = 0;

  OCStackResult res = CrlToCBORPayload((OicSecCrl_t *) crl, &payload, &size);
  if (OC_STACK_OK != res)
  {
    return res;
  }

  return UpdateSecureResourceInPS(OIC_CBOR_CRL_NAME, payload, size);
}

static OCEntityHandlerResult HandleCRLPostRequest(const OCEntityHandlerRequest *ehRequest)
{
  OCEntityHandlerResult ehRet = OC_EH_ERROR;
  OicSecCrl_t *crl = NULL;
  uint8_t *payload = ((OCSecurityPayload *)ehRequest->payload)->securityData;
  size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;

  if (payload)
  {
    OIC_LOG(INFO, TAG, "UpdateSVRDB...");
    CBORPayloadToCrl(payload, size, &crl);
    VERIFY_NON_NULL(TAG, crl, ERROR);

    gCrl->CrlId = crl->CrlId;

    OICFree(gCrl->ThisUpdate.data);
    gCrl->ThisUpdate.data = NULL;
    gCrl->ThisUpdate.data = OICMalloc(crl->ThisUpdate.len);
    VERIFY_NON_NULL(TAG, gCrl->ThisUpdate.data, ERROR);
    memcpy(gCrl->ThisUpdate.data, crl->ThisUpdate.data, crl->ThisUpdate.len);
    gCrl->ThisUpdate.len = crl->ThisUpdate.len;

    OICFree(gCrl->CrlData.data);
    gCrl->CrlData.data = OICMalloc(crl->CrlData.len);
    VERIFY_NON_NULL(TAG, gCrl->CrlData.data, ERROR);
    memcpy(gCrl->CrlData.data, crl->CrlData.data, crl->CrlData.len);
    gCrl->CrlData.len = crl->CrlData.len;

    if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_CBOR_CRL_NAME, payload, size))
    {
      ehRet = OC_EH_RESOURCE_CREATED;
    }

    DeleteCrlBinData(crl);
  }

exit:
  // Send payload to request originator
  if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
  {
    ehRet = OC_EH_ERROR;
    OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleCRLPostRequest");
  }

  OIC_LOG_V(INFO, TAG, "%s RetVal %d", __func__, ehRet);
  return ehRet;
}


/**
 * This internal method is the entity handler for CRL resource and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
static OCEntityHandlerResult CRLEntityHandler(OCEntityHandlerFlag flag,
                                              OCEntityHandlerRequest *ehRequest,
                                              void *callbackParameter)
{
  OCEntityHandlerResult ehRet = OC_EH_ERROR;
  (void)callbackParameter;

  if (!ehRequest)
  {
    return ehRet;
  }

  OIC_LOG(INFO, TAG, "Handle CRL resource");

  if (flag & OC_REQUEST_FLAG)
  {
    // TODO :  Handle PUT and DEL methods
    OIC_LOG(INFO, TAG, "Flag includes OC_REQUEST_FLAG");
    switch (ehRequest->method)
    {
      case OC_REST_GET:
        OIC_LOG(INFO, TAG, "Not implemented request method.");
        //ehRet = HandleCRLGetRequest(ehRequest);
        break;

      case OC_REST_POST:
        ehRet = HandleCRLPostRequest(ehRequest);
        break;

      default:
        ehRet = OC_EH_ERROR;
        SendSRMResponse(ehRequest, ehRet, NULL, 0);
    }
  }

  return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/crl' resource.
 */
static OCStackResult CreateCRLResource()
{
  OCStackResult ret = OCCreateResource(&gCrlHandle,
                                       OIC_RSRC_TYPE_SEC_CRL,
                                       OIC_MI_DEF,
                                       OIC_RSRC_CRL_URI,
                                       CRLEntityHandler,
                                       NULL,
                                       OC_OBSERVABLE);

  if (OC_STACK_OK != ret)
  {
    OIC_LOG(FATAL, TAG, "Unable to instantiate CRL resource");
    DeInitCRLResource();
  }
  return ret;
}

/**
 * Get the default value.
 * @return defaultCrl for now.
 */
static OicSecCrl_t *GetCrlDefault()
{
  OicSecCrl_t *defaultCrl = (OicSecCrl_t *)OICCalloc(1, sizeof(OicSecCrl_t));
  if (NULL == defaultCrl)
  {
    return NULL;
  }
  defaultCrl->CrlId = CRL_DEFAULT_CRL_ID;

  defaultCrl->CrlData.len = strlen(CRL_DEFAULT_CRL_DATA);
  defaultCrl->CrlData.data = (uint8_t *) OICMalloc(defaultCrl->CrlData.len);
  if (defaultCrl->CrlData.data)
  {
    memcpy(defaultCrl->CrlData.data, CRL_DEFAULT_CRL_DATA, defaultCrl->CrlData.len);
  }
  else
  {
    defaultCrl->CrlData.len = 0;
  }

  defaultCrl->ThisUpdate.len = strlen(CRL_DEFAULT_THIS_UPDATE);
  defaultCrl->ThisUpdate.data = (uint8_t *) OICMalloc(defaultCrl->ThisUpdate.len);
  if (defaultCrl->ThisUpdate.data)
  {
    memcpy(defaultCrl->ThisUpdate.data, CRL_DEFAULT_THIS_UPDATE, defaultCrl->ThisUpdate.len);
  }
  else
  {
    defaultCrl->ThisUpdate.len = 0;
  }

  return defaultCrl;
}

/**
 * Initialize CRL resource by loading data from persistent storage.
 *
 * @retval
 *     OC_STACK_OK    - no errors
 *     OC_STACK_ERROR - stack process error
 */
OCStackResult InitCRLResource()
{
  OCStackResult ret = OC_STACK_ERROR;
  // Read Crl resource from PS
  uint8_t *data = NULL;
  size_t size = 0;
  ret = GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size);
  // If database read failed
  if (OC_STACK_OK != ret)
  {
    OIC_LOG(DEBUG, TAG, "ReadSVDataFromPS failed");
  }
  if (data)
  {
    // Read ACL resource from PS
    ret = CBORPayloadToCrl(data, size, &gCrl);
  }

  /*
   * If SVR database in persistent storage got corrupted or
   * is not available for some reason, a default CrlResource is created
   * which allows user to initiate CrlResource provisioning again.
   */
  if ((OC_STACK_OK != ret) || !data || !gCrl)
  {
    gCrl = GetCrlDefault();
  }

  ret = CreateCRLResource();
  OICFree(data);
  return ret;
}

/**
 * Perform cleanup for ACL resources.
 */
void DeInitCRLResource()
{
  OCDeleteResource(gCrlHandle);
  gCrlHandle = NULL;
  DeleteCrlBinData(gCrl);
  gCrl = NULL;
}

OicSecCrl_t *GetCRLResource()
{
  OicSecCrl_t *crl =  NULL;

  //Read CRL resource from PS
  uint8_t *data = NULL;
  size_t size = 0;
  OCStackResult ret = GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size);
  if (data)
  {
    //Convert CBOR CRL into binary format
    ret = CBORPayloadToCrl(data, size, &crl);
  }
  /*
   * If SVR database in persistent storage got corrupted or
   * is not available for some reason, a default CrlResource is created
   * which allows user to initiate CrlResource provisioning again.
   */
  if ((OC_STACK_OK != ret) || !data || !crl)
  {
    crl = GetCrlDefault();
  }
  OICFree(data);

  return crl;
}

uint8_t *GetCrl()
{
  uint8_t *data = NULL;
  size_t size = 0;
  OicSecCrl_t *crl = NULL;
  if (OC_STACK_OK == GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size) && data &&
      OC_STACK_OK == CBORPayloadToCrl(data, size, &crl))
  {
    return crl->CrlData.data;
  }
  return NULL;
}

void  GetDerCrl(ByteArray crlArray)
{
  OicSecCrl_t *crlRes = GetCRLResource();
  if (crlRes && crlRes->CrlData.len <= crlArray.len)
  {
    memcpy(crlArray.data, crlRes->CrlData.data, crlRes->CrlData.len);
    crlArray.len = crlRes->CrlData.len;
  }
  else
  {
    crlArray.len = 0;
  }
  DeleteCrlBinData(crlRes);
}


