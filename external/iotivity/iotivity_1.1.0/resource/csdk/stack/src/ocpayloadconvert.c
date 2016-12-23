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

#include "ocpayloadcbor.h"
#include "platform_features.h"
#include <stdlib.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocrandom.h"
#include "ocresourcehandler.h"
#include "cbor.h"
#include "rdpayload.h"

#define TAG "OIC_RI_PAYLOADCONVERT"

// Arbitrarily chosen size that seems to contain the majority of packages
#define INIT_SIZE (255)

// Discovery Links Map Length.
#define LINKS_MAP_LEN 4

// Functions all return either a CborError, or a negative version of the OC_STACK return values
static int64_t OCConvertPayloadHelper(OCPayload *payload, uint8_t *outPayload, size_t *size);
static int64_t OCConvertDiscoveryPayload(OCDiscoveryPayload *payload, uint8_t *outPayload,
                                         size_t *size);
static int64_t OCConvertDevicePayload(OCDevicePayload *payload, uint8_t *outPayload,
                                      size_t *size);
static int64_t OCConvertPlatformPayload(OCPlatformPayload *payload, uint8_t *outPayload,
                                        size_t *size);
static int64_t OCConvertRepPayload(OCRepPayload *payload, uint8_t *outPayload, size_t *size);
static int64_t OCConvertRepMap(CborEncoder *map, const OCRepPayload *payload);
static int64_t OCConvertPresencePayload(OCPresencePayload *payload, uint8_t *outPayload,
                                        size_t *size);
static int64_t OCConvertSecurityPayload(OCSecurityPayload *payload, uint8_t *outPayload,
                                        size_t *size);
static int64_t OCConvertSingleRepPayload(CborEncoder *parent, const OCRepPayload *payload);
static int64_t OCConvertArray(CborEncoder *parent, const OCRepPayloadValueArray *valArray);

static int64_t AddTextStringToMap(CborEncoder *map, const char *key, size_t keylen,
                                  const char *value);
static int64_t ConditionalAddTextStringToMap(CborEncoder *map, const char *key, size_t keylen,
                                             const char *value);

OCStackResult OCConvertPayload(OCPayload *payload, uint8_t **outPayload, size_t *size)
{
  // TinyCbor Version 47a78569c0 or better on master is required for the re-allocation
  // strategy to work.  If you receive the following assertion error, please do a git-pull
  // from the extlibs/tinycbor/tinycbor directory
#define CborNeedsUpdating  (CborErrorOutOfMemory < CborErrorDataTooLarge)
  OC_STATIC_ASSERT(!CborNeedsUpdating, "tinycbor needs to be updated to at least 47a78569c0");
#undef CborNeedsUpdating

  OCStackResult ret = OC_STACK_INVALID_PARAM;
  int64_t err;
  uint8_t *out = NULL;
  size_t curSize = INIT_SIZE;

  VERIFY_PARAM_NON_NULL(TAG, payload, "Input param, payload is NULL");
  VERIFY_PARAM_NON_NULL(TAG, outPayload, "OutPayload parameter is NULL");
  VERIFY_PARAM_NON_NULL(TAG, size, "size parameter is NULL");

  OIC_LOG_V(INFO, TAG, "Converting payload of type %d", payload->type);
  if (PAYLOAD_TYPE_SECURITY == payload->type)
  {
    size_t securityPayloadSize = ((OCSecurityPayload *)payload)->payloadSize;
    if (securityPayloadSize > 0)
    {
      out = (uint8_t *)OICCalloc(1, ((OCSecurityPayload *)payload)->payloadSize);
      VERIFY_PARAM_NON_NULL(TAG, out, "Failed to allocate security payload");
    }
  }
  if (out == NULL)
  {
    out = (uint8_t *)OICCalloc(1, curSize);
    VERIFY_PARAM_NON_NULL(TAG, out, "Failed to allocate payload");
  }
  err = OCConvertPayloadHelper(payload, out, &curSize);
  ret = OC_STACK_NO_MEMORY;

  if (err == CborErrorOutOfMemory)
  {
    // reallocate "out" and try again!
    uint8_t *out2 = (uint8_t *)OICRealloc(out, curSize);
    VERIFY_PARAM_NON_NULL(TAG, out2, "Failed to increase payload size");
    out = out2;
    err = OCConvertPayloadHelper(payload, out, &curSize);
    while (err == CborErrorOutOfMemory)
    {
      uint8_t *out2 = (uint8_t *)OICRealloc(out, curSize);
      VERIFY_PARAM_NON_NULL(TAG, out2, "Failed to increase payload size");
      out = out2;
      err = OCConvertPayloadHelper(payload, out, &curSize);
    }
  }

  if (err == CborNoError)
  {
    if (curSize < INIT_SIZE && PAYLOAD_TYPE_SECURITY != payload->type)
    {
      uint8_t *out2 = (uint8_t *)OICRealloc(out, curSize);
      VERIFY_PARAM_NON_NULL(TAG, out2, "Failed to increase payload size");
      out = out2;
    }

    *size = curSize;
    *outPayload = out;
    OIC_LOG_V(DEBUG, TAG, "Payload Size: %zd Payload : ", *size);
    OIC_LOG_BUFFER(DEBUG, TAG, *outPayload, *size);
    return OC_STACK_OK;
  }

  //TODO: Proper conversion from CborError to OCStackResult.
  ret = (OCStackResult) - err;

exit:
  OICFree(out);
  return ret;
}

static int64_t OCConvertPayloadHelper(OCPayload *payload, uint8_t *outPayload, size_t *size)
{
  switch (payload->type)
  {
    case PAYLOAD_TYPE_DISCOVERY:
      return OCConvertDiscoveryPayload((OCDiscoveryPayload *)payload, outPayload, size);
    case PAYLOAD_TYPE_DEVICE:
      return OCConvertDevicePayload((OCDevicePayload *)payload, outPayload, size);
    case PAYLOAD_TYPE_PLATFORM:
      return OCConvertPlatformPayload((OCPlatformPayload *)payload, outPayload, size);
    case PAYLOAD_TYPE_REPRESENTATION:
      return OCConvertRepPayload((OCRepPayload *)payload, outPayload, size);
    case PAYLOAD_TYPE_PRESENCE:
      return OCConvertPresencePayload((OCPresencePayload *)payload, outPayload, size);
    case PAYLOAD_TYPE_SECURITY:
      return OCConvertSecurityPayload((OCSecurityPayload *)payload, outPayload, size);
    case PAYLOAD_TYPE_RD:
      return OCRDPayloadToCbor((OCRDPayload *)payload, outPayload, size);
    default:
      OIC_LOG_V(INFO, TAG, "ConvertPayload default %d", payload->type);
      return CborErrorUnknownType;
  }
}

static int64_t checkError(int64_t err, CborEncoder *encoder, uint8_t *outPayload, size_t *size)
{
  if (err == CborErrorOutOfMemory)
  {
    *size += encoder->ptr - encoder->end;
    return err;
  }
  else if (err != CborNoError)
  {
    OIC_LOG_V(ERROR, TAG, "Convert Payload failed : %s", cbor_error_string(err));
    return err;
  }
  else
  {
    *size = encoder->ptr - outPayload;
    return err;
  }
}

static int64_t OCConvertSecurityPayload(OCSecurityPayload *payload, uint8_t *outPayload,
                                        size_t *size)
{
  memcpy(outPayload, payload->securityData, payload->payloadSize);
  *size = payload->payloadSize;

  return CborNoError;
}

static int64_t OCStringLLJoin(CborEncoder *map, char *type, OCStringLL *val)
{
  uint16_t count = 0;
  int64_t err = CborNoError;

  for (OCStringLL *temp = val; temp; temp = temp->next)
  {
    ++count;
  }
  if (count > 0)
  {
    CborEncoder array;
    err |= cbor_encode_text_string(map, type, strlen(type));
    err |= cbor_encoder_create_array(map, &array, count);
    while (val)
    {
      err |= cbor_encode_text_string(&array, val->value, strlen(val->value));
      val = val->next;
    }
    err |= cbor_encoder_close_container(map, &array);
  }

  return err;
}

static int64_t OCConvertDiscoveryPayload(OCDiscoveryPayload *payload, uint8_t *outPayload,
                                         size_t *size)
{
  CborEncoder encoder;
  int64_t err = CborNoError;

  cbor_encoder_init(&encoder, outPayload, *size, 0);

  if (payload->resources)
  {
    /*
    The format for the payload is "modelled" as JSON.

    [                                                       // rootArray
        {                                                   // rootMap
            "di" : UUID,                                    // device ID
            "href": "/oic/res"
            "rt": "oic.wk.res"
            "n":"MyDevice"
            "if":"oic.if.ll oic.if.baseline"
            "di": "0685B960-736F-46F7-BEC0-9E6CBD61ADC1",
            links :[                                        // linksArray contains maps of resources
                        {
                            href, rt, if, policy            // Resource 1
                        },
                        {
                            href, rt, if, policy            // Resource 2
                        },
                        .
                        .
                        .
                    ]
        }
    ]
    */
    // Open the main root array
    CborEncoder rootArray;
    size_t resourceCount =  OCDiscoveryPayloadGetResourceCount(payload);
    err |= cbor_encoder_create_array(&encoder, &rootArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery root array");

    // Open the root map in the root array
    CborEncoder rootMap;
    err |= cbor_encoder_create_map(&rootArray, &rootMap, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery map");

    // Insert Name
    err |= ConditionalAddTextStringToMap(&rootMap, OC_RSRVD_DEVICE_NAME,
                                         sizeof(OC_RSRVD_DEVICE_NAME) - 1, payload->name);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting name");

    // Insert URI
    err |= ConditionalAddTextStringToMap(&rootMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                         payload->uri);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting href");

    // Insert Device ID into the root map
    err |= AddTextStringToMap(&rootMap, OC_RSRVD_DEVICE_ID, sizeof(OC_RSRVD_DEVICE_ID) - 1,
                              payload->sid);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting device id");

    // Insert Resource Type
    err |= ConditionalAddTextStringToMap(&rootMap, OC_RSRVD_RESOURCE_TYPE,
                                         sizeof(OC_RSRVD_RESOURCE_TYPE) - 1, payload->type);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting RT");

    // Insert interfaces
    if (payload->interface)
    {
      err |= OCStringLLJoin(&rootMap, OC_RSRVD_INTERFACE, payload->interface);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding interface types tag/value");
    }

    // Insert baseURI if present
    err |= ConditionalAddTextStringToMap(&rootMap, OC_RSRVD_BASE_URI,
                                         sizeof(OC_RSRVD_BASE_URI) - 1,
                                         payload->baseURI);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting baseURI");

    // Insert Links into the root map.
    CborEncoder linkArray;
    err |= cbor_encode_text_string(&rootMap, OC_RSRVD_LINKS, sizeof(OC_RSRVD_LINKS) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array tag");
    err |= cbor_encoder_create_array(&rootMap, &linkArray, resourceCount);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array");

    for (size_t i = 0; i < resourceCount; ++i)
    {
      CborEncoder linkMap;
      OCResourcePayload *resource = OCDiscoveryPayloadGetResource(payload, i);
      VERIFY_PARAM_NON_NULL(TAG, resource, "Failed retrieving resource");

      // resource map inside the links array.
      err |= cbor_encoder_create_map(&linkArray, &linkMap, LINKS_MAP_LEN);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating links map");

      // Below are insertions of the resource properties into the map.
      // Uri
      err |= AddTextStringToMap(&linkMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                resource->uri);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding uri to links map");

      // Resource Type
      if (resource->types)
      {
        err |= OCStringLLJoin(&linkMap, OC_RSRVD_RESOURCE_TYPE, resource->types);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resourceType tag/value to links map");
      }
      // Interface Types
      if (resource->interfaces)
      {
        err |= OCStringLLJoin(&linkMap, OC_RSRVD_INTERFACE, resource->interfaces);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding interfaces tag/value to links map");
      }

      // Policy
      CborEncoder policyMap;
      err |= cbor_encode_text_string(&linkMap, OC_RSRVD_POLICY, sizeof(OC_RSRVD_POLICY) - 1);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding policy tag to links map");
      err |= cbor_encoder_create_map(&linkMap, &policyMap, CborIndefiniteLength);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding policy map to links map");

      // Bitmap
      err |=  cbor_encode_text_string(&policyMap, OC_RSRVD_BITMAP, sizeof(OC_RSRVD_BITMAP) - 1);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding bitmap tag to policy map");
      err |= cbor_encode_uint(&policyMap, resource->bitmap);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding bitmap value to policy map");

      if (resource->secure)
      {
        err |= cbor_encode_text_string(&policyMap, OC_RSRVD_SECURE,
                                       sizeof(OC_RSRVD_SECURE) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure tag to policy map");
        err |= cbor_encode_boolean(&policyMap, OC_RESOURCE_SECURE);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure value to policy map");
      }
      if ((resource->secure && resource->port != 0) || payload->baseURI)
      {
        err |= cbor_encode_text_string(&policyMap, OC_RSRVD_HOSTING_PORT,
                                       sizeof(OC_RSRVD_HOSTING_PORT) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure port tag");
        err |= cbor_encode_uint(&policyMap, resource->port);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure port value");
      }

      err |= cbor_encoder_close_container(&linkMap, &policyMap);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing policy map");

      // Finsihed encoding a resource, close the map.
      err |= cbor_encoder_close_container(&linkArray, &linkMap);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing link map");
    }
    // Close links array inside the root map.
    err |= cbor_encoder_close_container(&rootMap, &linkArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing link array");
    // close root map inside the root array.
    err |= cbor_encoder_close_container(&rootArray, &rootMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root map");
    // Close the final root array.
    err |= cbor_encoder_close_container(&encoder, &rootArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root array");
  }

exit:
  return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertDevicePayload(OCDevicePayload *payload, uint8_t *outPayload,
                                      size_t *size)
{
  if (!payload)
  {
    return CborUnknownError;
  }
  int64_t err = CborNoError;
  CborEncoder encoder;

  cbor_encoder_init(&encoder, outPayload, *size, 0);
  CborEncoder repMap;
  err |= cbor_encoder_create_map(&encoder, &repMap, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating device map");

  // Resource Type
  if (payload->types)
  {
    OIC_LOG(INFO, TAG, "Payload has types");
    err |= OCStringLLJoin(&repMap, OC_RSRVD_RESOURCE_TYPE, payload->types);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resource type tag/value.");
  }

  if (payload->interfaces)
  {
    OIC_LOG(INFO, TAG, "Payload has interface");
    err |= OCStringLLJoin(&repMap, OC_RSRVD_INTERFACE, payload->interfaces);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding interface type tag/value.");
  }

  // Device ID
  err |= AddTextStringToMap(&repMap, OC_RSRVD_DEVICE_ID, sizeof(OC_RSRVD_DEVICE_ID) - 1 , payload->sid);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding device id");

  // Device Name
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_DEVICE_NAME,
                                       sizeof(OC_RSRVD_DEVICE_NAME) - 1, payload->deviceName);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding device name");

  // Device Spec Version
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_SPEC_VERSION,
                                       sizeof(OC_RSRVD_SPEC_VERSION) - 1, payload->specVersion);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding data spec version");

  // Device data Model Version
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_DATA_MODEL_VERSION,
                                       sizeof(OC_RSRVD_DATA_MODEL_VERSION) - 1, payload->dataModelVersion);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding data model version");

  err |= cbor_encoder_close_container(&encoder, &repMap);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing device map");

exit:
  return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertPlatformPayload(OCPlatformPayload *payload, uint8_t *outPayload,
                                        size_t *size)
{
  int64_t err = CborNoError;
  CborEncoder encoder;

  cbor_encoder_init(&encoder, outPayload, *size, 0);

  CborEncoder repMap;
  err |= cbor_encoder_create_map(&encoder, &repMap, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating platform map");

  // Platform ID
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_PLATFORM_ID,
                                       sizeof(OC_RSRVD_PLATFORM_ID) - 1, payload->info.platformID);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding platform id");

  // MFG Name
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_MFG_NAME,
                                       sizeof(OC_RSRVD_MFG_NAME) - 1, payload->info.manufacturerName);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding mfg name");

  // MFG Url
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_MFG_URL,
                                       sizeof(OC_RSRVD_MFG_URL) - 1, payload->info.manufacturerUrl);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding mfg url");

  // Model Num
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_MODEL_NUM,
                                       sizeof(OC_RSRVD_MODEL_NUM) - 1, payload->info.modelNumber);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding model num");

  // Date of Mfg
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_MFG_DATE,
                                       sizeof(OC_RSRVD_MFG_DATE) - 1, payload->info.dateOfManufacture);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding mfg date");

  // Platform Version
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_PLATFORM_VERSION,
                                       sizeof(OC_RSRVD_PLATFORM_VERSION) - 1, payload->info.platformVersion);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding platform version");

  // OS Version
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_OS_VERSION,
                                       sizeof(OC_RSRVD_OS_VERSION) - 1, payload->info.operatingSystemVersion);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding OS version");

  // Hardware Version
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_HARDWARE_VERSION,
                                       sizeof(OC_RSRVD_HARDWARE_VERSION) - 1, payload->info.hardwareVersion);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding HW version");

  // Firmware Version
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_FIRMWARE_VERSION,
                                       sizeof(OC_RSRVD_FIRMWARE_VERSION) - 1, payload->info.firmwareVersion);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding firmware version");

  // Support URL
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_SUPPORT_URL,
                                       sizeof(OC_RSRVD_SUPPORT_URL) - 1, payload->info.supportUrl);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding support url");

  // System Time
  err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_SYSTEM_TIME,
                                       sizeof(OC_RSRVD_SYSTEM_TIME) - 1, payload->info.systemTime);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding system time");

  // Resource type
  if (payload->rt)
  {
    err |= ConditionalAddTextStringToMap(&repMap, OC_RSRVD_RESOURCE_TYPE,
                                         sizeof(OC_RSRVD_RESOURCE_TYPE) - 1, payload->rt);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resource type.");
  }

  // Resource interfaces
  if (payload->interfaces)
  {
    err |= OCStringLLJoin(&repMap, OC_RSRVD_INTERFACE, payload->interfaces);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding platform interface type.");
  }

  // Close Map
  err |= cbor_encoder_close_container(&encoder, &repMap);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep map");

exit:
  return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertArrayItem(CborEncoder *array, const OCRepPayloadValueArray *valArray,
                                  size_t index)
{
  int64_t err = CborNoError;
  switch (valArray->type)
  {
    case OCREP_PROP_NULL:
      OIC_LOG(ERROR, TAG, "ConvertArray Invalid NULL");
      err = CborUnknownError;
      break;
    case OCREP_PROP_INT:
      err |= cbor_encode_int(array, valArray->iArray[index]);
      break;
    case OCREP_PROP_DOUBLE:
      err |= cbor_encode_double(array, valArray->dArray[index]);
      break;
    case OCREP_PROP_BOOL:
      err |= cbor_encode_boolean(array, valArray->bArray[index]);
      break;
    case OCREP_PROP_STRING:
      if (!valArray->strArray[index])
      {
        err |= cbor_encode_null(array);
      }
      else
      {
        err |= cbor_encode_text_string(array, valArray->strArray[index],
                                       strlen(valArray->strArray[index]));
      }
      break;
    case OCREP_PROP_BYTE_STRING:
      if (!valArray->strArray[index])
      {
        err |= cbor_encode_null(array);
      }
      else
      {
        err |= cbor_encode_byte_string(array, valArray->ocByteStrArray[index].bytes,
                                       valArray->ocByteStrArray[index].len);
      }
      break;
    case OCREP_PROP_OBJECT:
      if (!valArray->objArray[index])
      {
        err |= cbor_encode_null(array);
      }
      else
      {
        err |= OCConvertRepMap(array, valArray->objArray[index]);
      }
      break;
    case OCREP_PROP_ARRAY:
      OIC_LOG(ERROR, TAG, "ConvertArray Invalid child array");
      err = CborUnknownError;
      break;
  }

  return err;
}

static int64_t OCConvertArray(CborEncoder *parent, const OCRepPayloadValueArray *valArray)
{
  int64_t err = CborNoError;
  CborEncoder array;
  err |= cbor_encoder_create_array(parent, &array, valArray->dimensions[0]);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep array");

  for (size_t i = 0; i < valArray->dimensions[0]; ++i)
  {
    if (0 != valArray->dimensions[1])
    {
      CborEncoder array2;
      err |= cbor_encoder_create_array(&array, &array2, valArray->dimensions[1]);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep array2");

      for (size_t j = 0; j < valArray->dimensions[1]; ++j)
      {
        if (0 != valArray->dimensions[2])
        {
          CborEncoder array3;
          err |= cbor_encoder_create_array(&array2, &array3, valArray->dimensions[2]);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep array3");

          for (size_t k = 0; k < valArray->dimensions[2]; ++k)
          {
            err |= OCConvertArrayItem(&array3, valArray,
                                      j * valArray->dimensions[2] +
                                      i * valArray->dimensions[2] * valArray->dimensions[1] +
                                      k);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep array3 value");
          }
          err |= cbor_encoder_close_container(&array2, &array3);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep array3");
        }
        else
        {
          err |= OCConvertArrayItem(&array2, valArray, i * valArray->dimensions[1] + j);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep array2 value");
        }
      }
      err |= cbor_encoder_close_container(&array, &array2);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep array2");
    }
    else
    {
      err |= OCConvertArrayItem(&array, valArray, i);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep array value");
    }
  }
  err |= cbor_encoder_close_container(parent, &array);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep array");

exit:
  return err;
}

static int64_t OCConvertRepMap(CborEncoder *map, const OCRepPayload *payload)
{
  int64_t err = CborNoError;
  CborEncoder repMap;
  err |= cbor_encoder_create_map(map, &repMap, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep map");
  err |= OCConvertSingleRepPayload(&repMap, payload);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed converting single rep payload");
  err |= cbor_encoder_close_container(map, &repMap);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep map");
exit:
  return err;
}

static int64_t OCConvertSingleRepPayload(CborEncoder *repMap, const OCRepPayload *payload)
{
  int64_t err = CborNoError;
  OCRepPayloadValue *value = payload->values;
  while (value)
  {
    err |= cbor_encode_text_string(repMap, value->name, strlen(value->name));
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding tag name");

    switch (value->type)
    {
      case OCREP_PROP_NULL:
        err |= cbor_encode_null(repMap);
        break;
      case OCREP_PROP_INT:
        err |= cbor_encode_int(repMap, value->i);
        break;
      case OCREP_PROP_DOUBLE:
        err |= cbor_encode_double(repMap, value->d);
        break;
      case OCREP_PROP_BOOL:
        err |= cbor_encode_boolean(repMap, value->b);
        break;
      case OCREP_PROP_STRING:
        err |= cbor_encode_text_string(repMap, value->str, strlen(value->str));
        break;
      case OCREP_PROP_BYTE_STRING:
        err |= cbor_encode_byte_string(repMap, value->ocByteStr.bytes, value->ocByteStr.len);
        break;
      case OCREP_PROP_OBJECT:
        err |= OCConvertRepMap(repMap, value->obj);
        break;
      case OCREP_PROP_ARRAY:
        err |= OCConvertArray(repMap, &value->arr);
        break;
      default:
        OIC_LOG_V(ERROR, TAG, "Invalid Prop type: %d", value->type);
        break;
    }
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding single rep value");
    value = value->next;
  }

exit:
  return err;
}

static int64_t OCConvertRepPayload(OCRepPayload *payload, uint8_t *outPayload, size_t *size)
{
  CborEncoder encoder;
  int64_t err = CborNoError;

  cbor_encoder_init(&encoder, outPayload, *size, 0);

  size_t arrayCount = 0;
  for (OCRepPayload *temp = payload; temp; temp = temp->next)
  {
    arrayCount++;
  }
  CborEncoder rootArray;
  if (arrayCount > 1)
  {
    err |= cbor_encoder_create_array(&encoder, &rootArray, arrayCount);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep root map");
  }

  while (payload != NULL && (err == CborNoError))
  {
    CborEncoder rootMap;
    err |= cbor_encoder_create_map(((arrayCount == 1) ? &encoder : &rootArray),
                                   &rootMap, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating root map");

    // Only in case of collection href is included.
    if (arrayCount > 1 && payload->uri && strlen(payload->uri) > 0)
    {
      OIC_LOG(INFO, TAG, "Payload has uri");
      err |= cbor_encode_text_string(&rootMap, OC_RSRVD_HREF, strlen(OC_RSRVD_HREF));
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep href tag");
      err |= cbor_encode_text_string(&rootMap, payload->uri, strlen(payload->uri));
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep href value");
    }
    if (payload->types)
    {
      OIC_LOG(INFO, TAG, "Payload has types");
      err |= OCStringLLJoin(&rootMap, OC_RSRVD_RESOURCE_TYPE, payload->types);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resource type.");
    }
    if (payload->interfaces)
    {
      OIC_LOG(INFO, TAG, "Payload has interfaces");
      err |= OCStringLLJoin(&rootMap, OC_RSRVD_INTERFACE, payload->interfaces);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding platform interface type.");
    }

    err |= OCConvertSingleRepPayload(&rootMap, payload);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting rep payload");

    // Close main array
    err |= cbor_encoder_close_container(((arrayCount == 1) ? &encoder : &rootArray),
                                        &rootMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root map");
    payload = payload->next;
  }
  if (arrayCount > 1)
  {
    err |= cbor_encoder_close_container(&encoder, &rootArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root array");
  }

exit:
  return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertPresencePayload(OCPresencePayload *payload, uint8_t *outPayload,
                                        size_t *size)
{
  int64_t err = CborNoError;
  CborEncoder encoder;

  cbor_encoder_init(&encoder, outPayload, *size, 0);
  CborEncoder map;
  err |= cbor_encoder_create_map(&encoder, &map, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating presence map");

  // Sequence Number
  err |= cbor_encode_text_string(&map, OC_RSRVD_NONCE, sizeof(OC_RSRVD_NONCE) - 1);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding nonce tag to presence map");
  err |= cbor_encode_uint(&map, payload->sequenceNumber);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding nonce value to presence map");

  // Max Age
  err |= cbor_encode_text_string(&map, OC_RSRVD_TTL, sizeof(OC_RSRVD_TTL) - 1);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding ttl tag to presence map");
  err |= cbor_encode_uint(&map, payload->maxAge);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding ttl value to presence map");

  // Trigger
  err |= cbor_encode_text_string(&map, OC_RSRVD_TRIGGER, sizeof(OC_RSRVD_TRIGGER) - 1);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding trigger tag to presence map");
  err |= cbor_encode_simple_value(&map, payload->trigger);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding trigger value to presence map");

  // Resource type name
  if (payload->trigger != OC_PRESENCE_TRIGGER_DELETE)
  {
    err |= ConditionalAddTextStringToMap(&map, OC_RSRVD_RESOURCE_TYPE,
                                         sizeof(OC_RSRVD_RESOURCE_TYPE) - 1, payload->resourceType);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resource type to presence map");
  }

  // Close Map
  err |= cbor_encoder_close_container(&encoder, &map);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing presence map");

exit:
  return checkError(err, &encoder, outPayload, size);
}

static int64_t AddTextStringToMap(CborEncoder *map, const char *key, size_t keylen,
                                  const char *value)
{
  int64_t err = cbor_encode_text_string(map, key, keylen);
  if (CborNoError != err)
  {
    return err;
  }
  return cbor_encode_text_string(map, value, strlen(value));
}

static int64_t ConditionalAddTextStringToMap(CborEncoder *map, const char *key, size_t keylen,
                                             const char *value)
{
  return value ? AddTextStringToMap(map, key, keylen, value) : 0;
}
