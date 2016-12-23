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

// Defining _POSIX_C_SOURCE macro with 200112L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1-2001 base
// specification (excluding the XSI extension).
// For POSIX.1-2001 base specification,
// Refer http://pubs.opengroup.org/onlinepubs/009695399/
// Required for strok_r
#define _POSIX_C_SOURCE 200112L
#include <string.h>
#include <stdlib.h>
#include "oic_string.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "ocstackinternal.h"
#include "payload_logging.h"
#include "rdpayload.h"

#define TAG "OIC_RI_PAYLOADPARSE"

static OCStackResult OCParseDiscoveryPayload(OCPayload **outPayload, CborValue *arrayVal);
static OCStackResult OCParseDevicePayload(OCPayload **outPayload, CborValue *arrayVal);
static OCStackResult OCParsePlatformPayload(OCPayload **outPayload, CborValue *arrayVal);
static CborError OCParseSingleRepPayload(OCRepPayload **outPayload, CborValue *repParent, bool isRoot);
static OCStackResult OCParseRepPayload(OCPayload **outPayload, CborValue *arrayVal);
static OCStackResult OCParsePresencePayload(OCPayload **outPayload, CborValue *arrayVal);
static OCStackResult OCParseSecurityPayload(OCPayload **outPayload, const uint8_t *payload, size_t size);

OCStackResult OCParsePayload(OCPayload **outPayload, OCPayloadType payloadType,
                             const uint8_t *payload, size_t payloadSize)
{
  OCStackResult result = OC_STACK_MALFORMED_RESPONSE;
  CborError err;

  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Conversion of outPayload failed");
  VERIFY_PARAM_NON_NULL(TAG, payload, "Invalid cbor payload value");

  OIC_LOG_V(INFO, TAG, "CBOR Parsing size: %zu of Payload Type: %d, Payload:",
            payloadSize, payloadType);
  OIC_LOG_BUFFER(DEBUG, TAG, payload, payloadSize);

  CborParser parser;
  CborValue rootValue;

  err = cbor_parser_init(payload, payloadSize, 0, &parser, &rootValue);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed initializing init value")

  switch (payloadType)
  {
    case PAYLOAD_TYPE_DISCOVERY:
      result = OCParseDiscoveryPayload(outPayload, &rootValue);
      break;
    case PAYLOAD_TYPE_DEVICE:
      result = OCParseDevicePayload(outPayload, &rootValue);
      break;
    case PAYLOAD_TYPE_PLATFORM:
      result = OCParsePlatformPayload(outPayload, &rootValue);
      break;
    case PAYLOAD_TYPE_REPRESENTATION:
      result = OCParseRepPayload(outPayload, &rootValue);
      break;
    case PAYLOAD_TYPE_PRESENCE:
      result = OCParsePresencePayload(outPayload, &rootValue);
      break;
    case PAYLOAD_TYPE_SECURITY:
      result = OCParseSecurityPayload(outPayload, payload, payloadSize);
      break;
    case PAYLOAD_TYPE_RD:
      result = OCRDCborToPayload(&rootValue, outPayload);
      break;
    default:
      OIC_LOG_V(ERROR, TAG, "ParsePayload Type default: %d", payloadType);
      result = OC_STACK_INVALID_PARAM;
      break;
  }

  OIC_LOG_V(INFO, TAG, "Finished parse payload, result is %d", result);

exit:
  return result;
}

void OCFreeOCStringLL(OCStringLL *ll);

static OCStackResult OCParseSecurityPayload(OCPayload **outPayload, const uint8_t *payload,
                                            size_t size)
{
  if (size > 0)
  {
    *outPayload = (OCPayload *)OCSecurityPayloadCreate(payload, size);
  }
  else
  {
    *outPayload = NULL;
  }
  return OC_STACK_OK;
}

static char *InPlaceStringTrim(char *str)
{
  while (str[0] == ' ')
  {
    ++str;
  }

  size_t lastchar = strlen(str);

  while (str[lastchar] == ' ')
  {
    str[lastchar] = '\0';
    --lastchar;
  }

  return str;
}

static CborError OCParseStringLL(CborValue *map, char *type, OCStringLL **resource)
{
  CborValue val;
  CborError err = cbor_value_map_find_value(map, type, &val);
  VERIFY_CBOR_SUCCESS(TAG, err, "to find StringLL TAG");

  if (cbor_value_is_array(&val))
  {
    CborValue txtStr;
    err = cbor_value_enter_container(&val, &txtStr);
    VERIFY_CBOR_SUCCESS(TAG, err, "to enter container");
    while (cbor_value_is_text_string(&txtStr))
    {
      size_t len = 0;
      char *input = NULL;
      err = cbor_value_dup_text_string(&txtStr, &input, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find StringLL value.");
      if (input)
      {
        char *savePtr = NULL;
        char *curPtr = strtok_r(input, " ", &savePtr);
        while (curPtr)
        {
          char *trimmed = InPlaceStringTrim(curPtr);
          if (trimmed[0] != '\0')
          {
            if (!OCResourcePayloadAddStringLL(resource, trimmed))
            {
              return CborErrorOutOfMemory;
            }
          }
          curPtr = strtok_r(NULL, " ", &savePtr);
        }
        OICFree(input);
      }
      if (cbor_value_is_text_string(&txtStr))
      {
        err = cbor_value_advance(&txtStr);
        VERIFY_CBOR_SUCCESS(TAG, err, "to advance string value");
      }
    }
  }
exit:
  return err;
}

static OCStackResult OCParseDiscoveryPayload(OCPayload **outPayload, CborValue *rootValue)
{
  OCStackResult ret = OC_STACK_INVALID_PARAM;
  OCResourcePayload *resource = NULL;
  OCDiscoveryPayload *out = NULL;
  size_t len = 0;
  CborError err = CborNoError;
  *outPayload = NULL;

  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");
  VERIFY_PARAM_NON_NULL(TAG, rootValue, "Invalid Parameter rootValue");
  if (cbor_value_is_array(rootValue))
  {
    // Root value is already inside the main root array
    CborValue rootMap;
    ret = OC_STACK_NO_MEMORY;
    out = OCDiscoveryPayloadCreate();
    VERIFY_PARAM_NON_NULL(TAG, out, "Failed error initializing discovery payload");

    // Enter the main root map
    ret = OC_STACK_MALFORMED_RESPONSE;
    err = cbor_value_enter_container(rootValue, &rootMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "to enter root map container");

    // Look for DI
    CborValue curVal;
    if (!cbor_value_is_map(&rootMap))
    {
      OIC_LOG(ERROR, TAG, "Malformed packet!!");
      goto exit;
    }
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_DEVICE_ID, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find device id tag");
    if (cbor_value_is_valid(&curVal))
    {
      if (cbor_value_is_byte_string(&curVal))
      {
        err = cbor_value_dup_byte_string(&curVal, (uint8_t **) & (out->sid), &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, err, "to copy device id value");
      }
      else if (cbor_value_is_text_string(&curVal))
      {
        err = cbor_value_dup_text_string(&curVal, &(out->sid), &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, err, "to copy device id value");
      }
    }

    // BaseURI - Not a mandatory field
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_BASE_URI, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find uri tag");
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &(out->baseURI), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find base uri value");
    }

    // HREF - Not a mandatory field
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_HREF, &curVal);
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &(out->uri), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find uri value");
    }

    // RT - Not a mandatory field
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_RESOURCE_TYPE, &curVal);
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &(out->type), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find base uri value");
    }

    // IF - Not a mandatory field
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_INTERFACE, &curVal);
    if (cbor_value_is_valid(&curVal))
    {
      err =  OCParseStringLL(&rootMap, OC_RSRVD_INTERFACE, &out->interface);
    }
    if (!out->interface)
    {
      if (!OCResourcePayloadAddStringLL(&out->interface, OC_RSRVD_INTERFACE_LL))
      {
        err = CborErrorOutOfMemory;
      }
    }

    // Name - Not a mandatory field
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_DEVICE_NAME, &curVal);
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &out->name, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find device name");
    }

    // Look for Links which will have an array as the value
    CborValue linkMap;
    err = cbor_value_map_find_value(&rootMap, OC_RSRVD_LINKS, &linkMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find links tag");

    // Enter the links array and start iterating through the array processing
    // each resource which shows up as a map.
    CborValue resourceMap;
    err = cbor_value_enter_container(&linkMap, &resourceMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "to enter link map");

    while (cbor_value_is_map(&resourceMap))
    {
      resource = (OCResourcePayload *)OICCalloc(1, sizeof(OCResourcePayload));
      VERIFY_PARAM_NON_NULL(TAG, resource, "Failed allocating resource payload");

      // Uri
      err = cbor_value_map_find_value(&resourceMap, OC_RSRVD_HREF, &curVal);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find href tag");
      err = cbor_value_dup_text_string(&curVal, &(resource->uri), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find href value");

      // ResourceTypes
      err =  OCParseStringLL(&resourceMap, OC_RSRVD_RESOURCE_TYPE, &resource->types);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find resource type tag/value");

      // Interface Types
      err =  OCParseStringLL(&resourceMap, OC_RSRVD_INTERFACE, &resource->interfaces);
      if (CborNoError != err)
      {
        if (!OCResourcePayloadAddStringLL(&resource->interfaces, OC_RSRVD_INTERFACE_LL))
        {
          OIC_LOG(ERROR, TAG, "Failed to add string to StringLL");
          goto exit;
        }
      }

      // Policy
      CborValue policyMap;
      err = cbor_value_map_find_value(&resourceMap, OC_RSRVD_POLICY, &policyMap);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find policy tag");

      // Bitmap
      err = cbor_value_map_find_value(&policyMap, OC_RSRVD_BITMAP, &curVal);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find bitmap tag");
      err = cbor_value_get_int(&curVal, (int *)&resource->bitmap);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find bitmap value");

      // Secure Flag
      err = cbor_value_map_find_value(&policyMap, OC_RSRVD_SECURE, &curVal);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find secure tag");
      if (cbor_value_is_valid(&curVal))
      {
        err = cbor_value_get_boolean(&curVal, &(resource->secure));
        VERIFY_CBOR_SUCCESS(TAG, err, "to find secure value");
      }

      // Port
      err = cbor_value_map_find_value(&policyMap, OC_RSRVD_HOSTING_PORT, &curVal);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find port tag");
      if (cbor_value_is_valid(&curVal))
      {
        err = cbor_value_get_int(&curVal, (int *)&resource->port);
        VERIFY_CBOR_SUCCESS(TAG, err, "to find port value");
      }

      err = cbor_value_advance(&resourceMap);
      VERIFY_CBOR_SUCCESS(TAG, err, "to advance resource map");

      OCDiscoveryPayloadAddNewResource(out, resource);
    }

    err = cbor_value_leave_container(rootValue, &resourceMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "to advance resource map");
  }
  else
  {
    OIC_LOG(ERROR, TAG, "Malformed packet ");
    goto exit;
  }

  *outPayload = (OCPayload *)out;
  return OC_STACK_OK;

exit:
  OCDiscoveryResourceDestroy(resource);
  OCDiscoveryPayloadDestroy(out);
  return ret;
}

static OCStackResult OCParseDevicePayload(OCPayload **outPayload, CborValue *rootValue)
{
  OCStackResult ret = OC_STACK_INVALID_PARAM;
  CborError err = CborNoError;
  OCDevicePayload *out = NULL;
  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid param outPayload");
  VERIFY_PARAM_NON_NULL(TAG, rootValue, "Invalid param rootValue");

  *outPayload = NULL;

  out = (OCDevicePayload *)OICCalloc(1, sizeof(OCDevicePayload));
  VERIFY_PARAM_NON_NULL(TAG, out, "Failed allocating device payload")
  out->base.type = PAYLOAD_TYPE_DEVICE;
  ret = OC_STACK_MALFORMED_RESPONSE;

  if (cbor_value_is_map(rootValue))
  {
    CborValue curVal;
    // Resource Type
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_RESOURCE_TYPE, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find resource type tag");
    if (cbor_value_is_valid(&curVal))
    {
      err =  OCParseStringLL(rootValue, OC_RSRVD_RESOURCE_TYPE, &out->types);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find rt type tag/value");
    }

    err = cbor_value_map_find_value(rootValue, OC_RSRVD_INTERFACE, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find interface tag");
    if (cbor_value_is_valid(&curVal))
    {
      err =  OCParseStringLL(rootValue, OC_RSRVD_INTERFACE, &out->interfaces);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find interfaces tag/value");
    }
    // Device ID
    size_t len = 0;
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_DEVICE_ID, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find device id tag");
    if (cbor_value_is_valid(&curVal))
    {
      if (cbor_value_is_byte_string(&curVal))
      {
        err = cbor_value_dup_byte_string(&curVal, (uint8_t **)&out->sid, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, err, "to find device id in device payload");
      }
      else if (cbor_value_is_text_string(&curVal))
      {
        err = cbor_value_dup_text_string(&curVal, &out->sid, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, err, "to find device id in device payload");
      }
    }
    // Device Name
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_DEVICE_NAME, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find device name tag");
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &out->deviceName, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find device name in device payload");
    }
    // Device Spec Version
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_SPEC_VERSION, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find spec ver tag");
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &out->specVersion, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find spec version in device payload");
    }
    // Data Model Version
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_DATA_MODEL_VERSION, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find data model ver tag");
    if (cbor_value_is_valid(&curVal))
    {
      err = cbor_value_dup_text_string(&curVal, &out->dataModelVersion, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find data model version in device payload");
    }
    err = cbor_value_advance(rootValue);
    VERIFY_CBOR_SUCCESS(TAG, err, "to advance device payload");

    *outPayload = (OCPayload *)out;
    return OC_STACK_OK;
  }

exit:
  OCDevicePayloadDestroy(out);
  return ret;
}

static OCStackResult OCParsePlatformPayload(OCPayload **outPayload, CborValue *rootValue)
{
  OCStackResult ret = OC_STACK_INVALID_PARAM;
  CborError err = CborNoError;
  OCPlatformInfo info = {0};
  char *rt = NULL;
  OCStringLL *interfaces = NULL;
  OCPlatformPayload *out = NULL;

  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");

  if (cbor_value_is_map(rootValue))
  {
    CborValue repVal;
    size_t len = 0;
    ret = OC_STACK_MALFORMED_RESPONSE;

    // Platform ID
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_PLATFORM_ID, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find platform id tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.platformID), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find platformID in the platform payload");
    }
    // MFG Name
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_MFG_NAME, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find mfg name tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.manufacturerName), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find manufactureName in the platform payload");
    }
    // MFG URL
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_MFG_URL, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find mfg url tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.manufacturerUrl), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find manufactureUrl in the platform payload");
    }
    // Model Num
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_MODEL_NUM, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find model num tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.modelNumber), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find modelNumber in the platform payload");
    }
    // Date of Mfg
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_MFG_DATE, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find mfg date tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.dateOfManufacture), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find dateOfManufacture in the platform payload");
    }
    // Platform Version
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_PLATFORM_VERSION, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find platform ver tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.platformVersion), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find platformVersion in the platform payload");
    }
    // OS Version
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_OS_VERSION, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find os ver tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.operatingSystemVersion), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find OSVersion in the platform payload");
    }
    // Hardware Version
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_HARDWARE_VERSION, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find hw ver tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.hardwareVersion), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find HWVersion in the platform payload");
    }
    // Firmware Version
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_FIRMWARE_VERSION, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find fw ver tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.firmwareVersion), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find firmwareVersion in the platform payload");
    }
    // Support URL
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_SUPPORT_URL, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find support url tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.supportUrl), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find supportUrl in the platform payload");
    }
    // System Time
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_SYSTEM_TIME, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find sys time tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &(info.systemTime), &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find systemTume in the platform payload");
    }

    // Resource type
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_RESOURCE_TYPE, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find resource type tag");
    if (cbor_value_is_valid(&repVal))
    {
      err = cbor_value_dup_text_string(&repVal, &rt, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find resource type in the platform payload");
    }

    // Interface Types
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_INTERFACE, &repVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find interface tag");
    if (cbor_value_is_valid(&repVal))
    {
      err =  OCParseStringLL(rootValue, OC_RSRVD_INTERFACE, &interfaces);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find interfaces tag/value");
    }

    err = cbor_value_advance(rootValue);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find supportUrl in the platform payload");

    out = (OCPlatformPayload *)OCPlatformPayloadCreateAsOwner(&info);
    out->rt = rt;
    out->interfaces = interfaces;
    *outPayload = (OCPayload *)out;
    OIC_LOG_PAYLOAD(DEBUG, *outPayload);
    return OC_STACK_OK;
  }

exit:
  OCPlatformInfoDestroy(&info);
  OIC_LOG(ERROR, TAG, "CBOR error In ParsePlatformPayload");
  return ret;
}

static OCRepPayloadPropType DecodeCborType(CborType type)
{
  switch (type)
  {
    case CborNullType:
      return OCREP_PROP_NULL;
    case CborIntegerType:
      return OCREP_PROP_INT;
    case CborDoubleType:
    case CborFloatType:
      return OCREP_PROP_DOUBLE;
    case CborBooleanType:
      return OCREP_PROP_BOOL;
    case CborTextStringType:
      return OCREP_PROP_STRING;
    case CborByteStringType:
      return OCREP_PROP_BYTE_STRING;
    case CborMapType:
      return OCREP_PROP_OBJECT;
    case CborArrayType:
      return OCREP_PROP_ARRAY;
    default:
      return OCREP_PROP_NULL;
  }
}
static CborError OCParseArrayFindDimensionsAndType(const CborValue *parent,
    size_t dimensions[MAX_REP_ARRAY_DEPTH], OCRepPayloadPropType *type)
{
  CborValue insideArray;
  *type = OCREP_PROP_NULL;
  dimensions[0] = dimensions[1] = dimensions[2] = 0;

  CborError err = cbor_value_enter_container(parent, &insideArray);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed to enter container");

  while (cbor_value_is_valid(&insideArray))
  {
    OCRepPayloadPropType tempType = DecodeCborType(cbor_value_get_type(&insideArray));

    if (tempType == OCREP_PROP_ARRAY)
    {
      size_t subdim[MAX_REP_ARRAY_DEPTH];
      tempType = OCREP_PROP_NULL;
      err = OCParseArrayFindDimensionsAndType(&insideArray, subdim, &tempType);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to parse array");

      if (subdim[2] != 0)
      {
        OIC_LOG(ERROR, TAG, "Parse array helper, sub-array too deep");
      }

      dimensions[1] = dimensions[1] >= subdim[0] ? dimensions[1] : subdim[0];
      dimensions[2] = dimensions[2] >= subdim[1] ? dimensions[2] : subdim[1];

      if (*type != OCREP_PROP_NULL && tempType != OCREP_PROP_NULL && *type != tempType)
      {
        OIC_LOG(ERROR, TAG, "Array parse failed, mixed arrays not allowed (subtype)");
        return CborUnknownError;
      }
      else if (*type == OCREP_PROP_NULL)
      {
        // We don't know the type of this array yet, so the assignment is OK
        *type = tempType;
      }
    }
    else if (*type == OCREP_PROP_NULL)
    {
      // We don't know the type of this array yet, so the assignment is OK
      *type = tempType;
    }
    // tempType is allowed to be NULL, since it might now know the answer yet
    else if (tempType != OCREP_PROP_NULL && *type != tempType)
    {
      // this is an invalid situation!
      OIC_LOG(ERROR, TAG, "Array parse failed, mixed arrays not allowed");
      return CborUnknownError;
    }

    ++dimensions[0];
    err = cbor_value_advance(&insideArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed to advance array");
  }

exit:
  return err;
}

static size_t getAllocSize(OCRepPayloadPropType type)
{
  switch (type)
  {
    case OCREP_PROP_INT:
      return sizeof(int64_t);
    case OCREP_PROP_DOUBLE:
      return sizeof(double);
    case OCREP_PROP_BOOL:
      return sizeof(bool);
    case OCREP_PROP_STRING:
      return sizeof(char *);
    case OCREP_PROP_BYTE_STRING:
      return sizeof(OCByteString);
    case OCREP_PROP_OBJECT:
      return sizeof(OCRepPayload *);
    default:
      return 0;
  }
}

static size_t arrayStep(size_t dimensions[MAX_REP_ARRAY_DEPTH], size_t elementNum)
{
  return
    (dimensions[1] == 0 ? 1 : dimensions[1]) *
    (dimensions[2] == 0 ? 1 : dimensions[2]) *
    elementNum;
}

static CborError OCParseArrayFillArray(const CborValue *parent,
                                       size_t dimensions[MAX_REP_ARRAY_DEPTH], OCRepPayloadPropType type, void *targetArray)
{
  CborValue insideArray;

  size_t i = 0;
  char *tempStr = NULL;
  OCByteString ocByteStr = { .bytes = NULL, .len = 0};
  size_t tempLen = 0;
  OCRepPayload *tempPl = NULL;

  size_t newdim[MAX_REP_ARRAY_DEPTH];
  newdim[0] = dimensions[1];
  newdim[1] = dimensions[2];
  newdim[2] = 0;

  CborError err = cbor_value_enter_container(parent, &insideArray);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed to enter container");

  while (!err && i < dimensions[0] && cbor_value_is_valid(&insideArray))
  {
    bool noAdvance = false;
    if (cbor_value_get_type(&insideArray) != CborNullType)
    {
      switch (type)
      {
        case OCREP_PROP_INT:
          if (dimensions[1] == 0)
          {
            err = cbor_value_get_int64(&insideArray, &(((int64_t *)targetArray)[i]));
          }
          else
          {
            err = OCParseArrayFillArray(&insideArray, newdim, type,
                                        &(((int64_t *)targetArray)[arrayStep(dimensions, i)]));
          }
          break;
        case OCREP_PROP_DOUBLE:
          if (dimensions[1] == 0)
          {
            double *d = &(((double *)targetArray)[i]);
            if (cbor_value_get_type(&insideArray) == CborDoubleType)
            {
              err = cbor_value_get_double(&insideArray, d);
            }
            else
            {
              /* must be float */
              float f;
              err = cbor_value_get_float(&insideArray, &f);
              if (!err)
                *d = f;
            }
          }
          else
          {
            err = OCParseArrayFillArray(&insideArray, newdim, type,
                                        &(((double *)targetArray)[arrayStep(dimensions, i)]));
          }
          break;
        case OCREP_PROP_BOOL:
          if (dimensions[1] == 0)
          {
            err = cbor_value_get_boolean(&insideArray, &(((bool *)targetArray)[i]));
          }
          else
          {
            err = OCParseArrayFillArray(&insideArray, newdim, type,
                                        &(((bool *)targetArray)[arrayStep(dimensions, i)]));
          }
          break;
        case OCREP_PROP_STRING:
          if (dimensions[1] == 0)
          {
            err = cbor_value_dup_text_string(&insideArray, &tempStr, &tempLen, NULL);
            ((char **)targetArray)[i] = tempStr;
            tempStr = NULL;
          }
          else
          {
            err = OCParseArrayFillArray(&insideArray, newdim, type,
                                        &(((char **)targetArray)[arrayStep(dimensions, i)]));
          }
          break;
        case OCREP_PROP_BYTE_STRING:
          if (dimensions[1] == 0)
          {
            err = cbor_value_dup_byte_string(&insideArray, &(ocByteStr.bytes),
                                             &(ocByteStr.len), NULL);
            ((OCByteString *)targetArray)[i] = ocByteStr;
          }
          else
          {
            err = OCParseArrayFillArray(&insideArray, newdim, type,
                                        &(((OCByteString *)targetArray)[arrayStep(dimensions, i)]));
          }
          break;
        case OCREP_PROP_OBJECT:
          if (dimensions[1] == 0)
          {
            err = OCParseSingleRepPayload(&tempPl, &insideArray, false);
            ((OCRepPayload **)targetArray)[i] = tempPl;
            tempPl = NULL;
            noAdvance = true;
          }
          else
          {
            err = OCParseArrayFillArray(&insideArray, newdim, type,
                                        &(((OCRepPayload **)targetArray)[arrayStep(dimensions, i)]));
          }
          break;
        default:
          OIC_LOG(ERROR, TAG, "Invalid Array type in Parse Array");
          err = CborErrorUnknownType;
          break;
      }
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting repPayload");
    }
    ++i;
    if (!noAdvance && cbor_value_is_valid(&insideArray))
    {
      err = cbor_value_advance(&insideArray);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed advnce insideArray");
    }
  }

exit:
  return err;
}

static CborError OCParseArray(OCRepPayload *out, const char *name, CborValue *container)
{
  void *arr = NULL;
  OCRepPayloadPropType type;
  size_t dimensions[MAX_REP_ARRAY_DEPTH];
  size_t dimTotal = 0;
  size_t allocSize = 0;
  bool res = true;
  CborError err = OCParseArrayFindDimensionsAndType(container, dimensions, &type);
  VERIFY_CBOR_SUCCESS(TAG, err, "Array details weren't clear");

  if (type == OCREP_PROP_NULL)
  {
    res = OCRepPayloadSetNull(out, name);
    err = (CborError) !res;
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting value");
    err = cbor_value_advance(container);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed advancing container");
    return err;
  }

  dimTotal = calcDimTotal(dimensions);
  allocSize = getAllocSize(type);
  arr = OICCalloc(dimTotal, allocSize);
  VERIFY_PARAM_NON_NULL(TAG, arr, "Array Parse allocation failed");

  res = OCParseArrayFillArray(container, dimensions, type, arr);
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed parse array");

  switch (type)
  {
    case OCREP_PROP_INT:
      res = OCRepPayloadSetIntArrayAsOwner(out, name, (int64_t *)arr, dimensions);
      break;
    case OCREP_PROP_DOUBLE:
      res = OCRepPayloadSetDoubleArrayAsOwner(out, name, (double *)arr, dimensions);
      break;
    case OCREP_PROP_BOOL:
      res = OCRepPayloadSetBoolArrayAsOwner(out, name, (bool *)arr, dimensions);
      break;
    case OCREP_PROP_STRING:
      res = OCRepPayloadSetStringArrayAsOwner(out, name, (char **)arr, dimensions);
      break;
    case OCREP_PROP_BYTE_STRING:
      res = OCRepPayloadSetByteStringArrayAsOwner(out, name, (OCByteString *)arr, dimensions);
      break;
    case OCREP_PROP_OBJECT:
      res = OCRepPayloadSetPropObjectArrayAsOwner(out, name, (OCRepPayload **)arr, dimensions);
      break;
    default:
      OIC_LOG(ERROR, TAG, "Invalid Array type in Parse Array");
      break;
  }
  err = (CborError) !res;
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting array parameter");
  return CborNoError;
exit:
  if (type == OCREP_PROP_STRING)
  {
    for (size_t i = 0; i < dimTotal; ++i)
    {
      OICFree(((char **)arr)[i]);
    }
  }
  if (type == OCREP_PROP_BYTE_STRING)
  {
    for (size_t i = 0; i < dimTotal; ++i)
    {
      OICFree(((OCByteString *)arr)[i].bytes);
    }
  }
  if (type == OCREP_PROP_OBJECT)
  {
    for (size_t i = 0; i < dimTotal; ++i)
    {
      OCRepPayloadDestroy(((OCRepPayload **)arr)[i]);
    }
  }
  OICFree(arr);
  return err;
}

static CborError OCParseSingleRepPayload(OCRepPayload **outPayload, CborValue *objMap, bool isRoot)
{
  CborError err = CborUnknownError;
  char *name = NULL;
  bool res = false;
  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");
  VERIFY_PARAM_NON_NULL(TAG, objMap, "Invalid Parameter objMap");

  if (cbor_value_is_map(objMap))
  {
    if (!*outPayload)
    {
      *outPayload = OCRepPayloadCreate();
      if (!*outPayload)
      {
        return CborErrorOutOfMemory;
      }
    }

    OCRepPayload *curPayload = *outPayload;

    size_t len = 0;
    CborValue repMap;
    err = cbor_value_enter_container(objMap, &repMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed entering repMap");

    while (!err && cbor_value_is_valid(&repMap))
    {
      if (cbor_value_is_text_string(&repMap))
      {
        err = cbor_value_dup_text_string(&repMap, &name, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding tag name in the map");
        err = cbor_value_advance(&repMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed advancing rootMap");
        if (name &&
            isRoot &&
            ((0 == strcmp(OC_RSRVD_HREF, name)) ||
             (0 == strcmp(OC_RSRVD_RESOURCE_TYPE, name)) ||
             (0 == strcmp(OC_RSRVD_INTERFACE, name))))
        {
          err = cbor_value_advance(&repMap);
          OICFree(name);
          continue;
        }
      }
      CborType type = cbor_value_get_type(&repMap);
      switch (type)
      {
        case CborNullType:
          res = OCRepPayloadSetNull(curPayload, name);
          break;
        case CborIntegerType:
        {
          int64_t intval = 0;
          err = cbor_value_get_int64(&repMap, &intval);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting int value");
          res = OCRepPayloadSetPropInt(curPayload, name, intval);
        }
        break;
        case CborDoubleType:
        {
          double doubleval = 0;
          err = cbor_value_get_double(&repMap, &doubleval);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting double value");
          res = OCRepPayloadSetPropDouble(curPayload, name, doubleval);
        }
        break;
        case CborBooleanType:
        {
          bool boolval = false;
          err = cbor_value_get_boolean(&repMap, &boolval);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting boolean value");
          res = OCRepPayloadSetPropBool(curPayload, name, boolval);
        }
        break;
        case CborTextStringType:
        {
          char *strval = NULL;
          err = cbor_value_dup_text_string(&repMap, &strval, &len, NULL);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting string value");
          res = OCRepPayloadSetPropStringAsOwner(curPayload, name, strval);
        }
        break;
        case CborByteStringType:
        {
          uint8_t *bytestrval = NULL;
          err = cbor_value_dup_byte_string(&repMap, &bytestrval, &len, NULL);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting byte string value");
          OCByteString tmp = {.bytes = bytestrval, .len = len};
          res = OCRepPayloadSetPropByteStringAsOwner(curPayload, name, &tmp);
        }
        break;
        case CborMapType:
        {
          OCRepPayload *pl = NULL;
          err = OCParseSingleRepPayload(&pl, &repMap, false);
          VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting parse single rep");
          res = OCRepPayloadSetPropObjectAsOwner(curPayload, name, pl);
        }
        break;
        case CborArrayType:
          err = OCParseArray(curPayload, name, &repMap);
          break;
        default:
          OIC_LOG_V(ERROR, TAG, "Parsing rep property, unknown type %d", repMap.type);
          res = false;
      }
      if (type != CborArrayType)
      {
        err = (CborError) !res;
      }
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting value");

      if (type != CborMapType && cbor_value_is_valid(&repMap))
      {
        err = cbor_value_advance(&repMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed advance repMap");
      }
      OICFree(name);
      name = NULL;
    }
    if (cbor_value_is_container(objMap))
    {
      err = cbor_value_leave_container(objMap, &repMap);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to leave container");
    }
    return err;
  }

exit:
  OICFree(name);
  OCRepPayloadDestroy(*outPayload);
  *outPayload = NULL;
  return err;
}

static OCStackResult OCParseRepPayload(OCPayload **outPayload, CborValue *root)
{
  OCStackResult ret = OC_STACK_INVALID_PARAM;
  CborError err;
  OCRepPayload *temp = NULL;
  OCRepPayload *rootPayload = NULL;
  OCRepPayload *curPayload = NULL;
  CborValue rootMap = *root;
  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");
  VERIFY_PARAM_NON_NULL(TAG, root, "Invalid Parameter root");

  *outPayload = NULL;
  if (cbor_value_is_array(root))
  {
    err = cbor_value_enter_container(root, &rootMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed entering repMap");
  }
  while (cbor_value_is_valid(&rootMap))
  {
    temp = OCRepPayloadCreate();
    ret = OC_STACK_NO_MEMORY;
    VERIFY_PARAM_NON_NULL(TAG, temp, "Failed allocating memory");

    CborValue curVal;
    ret = OC_STACK_MALFORMED_RESPONSE;

    // temporary fix to check for malformed cbor payload
    if (!cbor_value_is_map(&rootMap) && !cbor_value_is_array(&rootMap))
    {
      goto exit;
    }

    if (cbor_value_is_map(&rootMap))
    {
      err = cbor_value_map_find_value(&rootMap, OC_RSRVD_HREF, &curVal);
      VERIFY_CBOR_SUCCESS(TAG, err, "to find href tag");
      if (cbor_value_is_valid(&curVal))
      {
        size_t len = 0;
        err = cbor_value_dup_text_string(&curVal, &temp->uri, &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find uri");
      }
    }

    // Resource types
    if (cbor_value_is_map(&rootMap))
    {
      if (CborNoError == cbor_value_map_find_value(&rootMap, OC_RSRVD_RESOURCE_TYPE, &curVal))
      {
        err =  OCParseStringLL(&rootMap, OC_RSRVD_RESOURCE_TYPE, &temp->types);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find rt type tag/value");
      }
    }

    // Interface Types
    if (cbor_value_is_map(&rootMap))
    {
      if (CborNoError == cbor_value_map_find_value(&rootMap, OC_RSRVD_INTERFACE, &curVal))
      {
        err =  OCParseStringLL(&rootMap, OC_RSRVD_INTERFACE, &temp->interfaces);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find interfaces tag/value");
      }
    }

    if (cbor_value_is_map(&rootMap))
    {
      err = OCParseSingleRepPayload(&temp, &rootMap, true);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to parse single rep payload");
    }

    if (rootPayload == NULL)
    {
      rootPayload = temp;
      curPayload = temp;
    }
    else
    {
      curPayload->next = temp;
      curPayload = curPayload->next;
    }

    if (cbor_value_is_array(&rootMap))
    {
      err = cbor_value_advance(&rootMap);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed to advance single rep payload");
    }
  }
  *outPayload = (OCPayload *)rootPayload;
  return OC_STACK_OK;

exit:
  OCRepPayloadDestroy(temp);
  OCRepPayloadDestroy(rootPayload);
  OIC_LOG(ERROR, TAG, "CBOR error in ParseRepPayload");
  return ret;
}

static OCStackResult OCParsePresencePayload(OCPayload **outPayload, CborValue *rootValue)
{
  OCStackResult ret = OC_STACK_INVALID_PARAM;
  OCPresencePayload *payload = NULL;
  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");

  *outPayload = NULL;

  payload = (OCPresencePayload *)OICCalloc(1, sizeof(OCPresencePayload));
  ret = OC_STACK_NO_MEMORY;
  VERIFY_PARAM_NON_NULL(TAG, payload, "Failed allocating presence payload");
  payload->base.type = PAYLOAD_TYPE_PRESENCE;
  ret = OC_STACK_MALFORMED_RESPONSE;

  if (cbor_value_is_map(rootValue))
  {
    CborValue curVal;

    // Sequence Number
    CborError err = cbor_value_map_find_value(rootValue, OC_RSRVD_NONCE, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding nonce tag");
    err = cbor_value_get_uint64(&curVal, (uint64_t *)&payload->sequenceNumber);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding nonce value");

    // Max Age
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_TTL, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding ttl tag");
    err = cbor_value_get_uint64(&curVal, (uint64_t *)&payload->maxAge);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding ttl value");

    // Trigger
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_TRIGGER, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding trigger tag");
    err = cbor_value_get_simple_type(&curVal, (uint8_t *)&payload->trigger);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding trigger value");

    // Resource type name
    err = cbor_value_map_find_value(rootValue, OC_RSRVD_RESOURCE_TYPE, &curVal);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find res type tag");
    if (cbor_value_is_valid(&curVal))
    {
      size_t len = 0;
      err = cbor_value_dup_text_string(&curVal, &payload->resourceType, &len, NULL);
      VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding resource type value");
    }

    err = cbor_value_advance(rootValue);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed advancing root value");

    *outPayload = (OCPayload *)payload;
    return OC_STACK_OK;
  }
exit:
  OIC_LOG(ERROR, TAG, "CBOR error Parse Presence Payload");
  OCPresencePayloadDestroy(payload);
  return ret;
}
