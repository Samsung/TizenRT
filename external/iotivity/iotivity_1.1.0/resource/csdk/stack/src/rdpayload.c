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
#include "rdpayload.h"

#include "oic_malloc.h"
#include "oic_string.h"
#include "octypes.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "payload_logging.h"

#define TAG "OIC_RI_RDPAYLOAD"

#define CBOR_ROOT_ARRAY_LENGTH 1

static CborError OCTagsPayloadToCbor(OCTagsPayload *tags, CborEncoder *setMap);
static CborError OCLinksPayloadToCbor(OCLinksPayload *rtPtr, CborEncoder *setMap);
static CborError OCTagsCborToPayload(CborValue *tagsMap, OCTagsPayload **tagsPayload);
static CborError OCLinksCborToPayload(CborValue *linksArray, OCLinksPayload **linksPayload);
static CborError FindStringInMap(const CborValue *map, const char *tags, char **value);
static CborError FindIntInMap(const CborValue *map, const char *tags, uint64_t *value);
static CborError FindStringLLInMap(const CborValue *linksMap, const char *tag, OCStringLL **links);
static CborError ConditionalAddTextStringToMap(CborEncoder *map, const char *key, const char *value);
static CborError ConditionalAddIntToMap(CborEncoder *map, const char *tags, const uint64_t *value);
static CborError AddStringLLToMap(CborEncoder *map, const char *tag, const OCStringLL *value);

CborError OCRDPayloadToCbor(const OCRDPayload *rdPayload, uint8_t *outPayload, size_t *size)
{
  CborError cborEncoderResult = CborErrorIO;
  int flags = 0;
  CborEncoder encoder;
  VERIFY_PARAM_NON_NULL(TAG, rdPayload, "Invalid input parameter rdPayload");
  VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid input parameter outPayload");
  VERIFY_PARAM_NON_NULL(TAG, size, "Invalid input parameter size");

  cbor_encoder_init(&encoder, outPayload, *size, flags);

  if (rdPayload->rdDiscovery)
  {
    CborEncoder map;
    cborEncoderResult = cbor_encoder_create_map(&encoder, &map, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create discovery map");

    cborEncoderResult = ConditionalAddTextStringToMap(&map, OC_RSRVD_DEVICE_NAME,
                                                      rdPayload->rdDiscovery->n.deviceName);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_DEVICE_NAME in map");

    cborEncoderResult = ConditionalAddTextStringToMap(&map, OC_RSRVD_DEVICE_ID,
                                                      (char *)rdPayload->rdDiscovery->di.id);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_DEVICE_ID in map");

    {
      uint64_t value = rdPayload->rdDiscovery->sel;
      cborEncoderResult = ConditionalAddIntToMap(&map, OC_RSRVD_RD_DISCOVERY_SEL, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add RD_DISCOVERY_SEL in map");
    }
    cborEncoderResult = cbor_encoder_close_container(&encoder, &map);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing discovery map");
  }
  else if (rdPayload->rdPublish)
  {
    CborEncoder colArray;
    cborEncoderResult = cbor_encoder_create_array(&encoder, &colArray, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create collection array");

    OCResourceCollectionPayload *rdPublish = rdPayload->rdPublish;
    while (rdPublish)
    {
      cborEncoderResult = OCTagsPayloadToCbor(rdPublish->tags, &colArray);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding tags payload");
      cborEncoderResult = OCLinksPayloadToCbor(rdPublish->setLinks, &colArray);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding setLinks payload");
      rdPublish = rdPublish->next;
    }
    cborEncoderResult = cbor_encoder_close_container(&encoder, &colArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing collection array");
  }
  else
  {
    CborEncoder map;
    cborEncoderResult = cbor_encoder_create_map(&encoder, &map, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed entering discovery map");
    cborEncoderResult = cbor_encoder_close_container(&encoder, &map);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing discovery map");
  }
  *size = encoder.ptr - outPayload;

  return cborEncoderResult;

exit:
  OICFree(outPayload);
  return cborEncoderResult;
}

static CborError OCTagsPayloadToCbor(OCTagsPayload *tags, CborEncoder *setMap)
{
  CborEncoder tagsMap;
  CborError cborEncoderResult = cbor_encoder_create_map(setMap, &tagsMap, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create tags map");

  cborEncoderResult = ConditionalAddTextStringToMap(&tagsMap, OC_RSRVD_DEVICE_NAME, tags->n.deviceName);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_DEVICE_NAME in tags map");

  cborEncoderResult = ConditionalAddTextStringToMap(&tagsMap, OC_RSRVD_DEVICE_ID,
                                                    (char *)tags->di.id);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_DEVICE_ID in tags map");

  cborEncoderResult = ConditionalAddTextStringToMap(&tagsMap, OC_RSRVD_RTS, tags->rts);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_RTS in tags map");

  cborEncoderResult = ConditionalAddTextStringToMap(&tagsMap, OC_RSRVD_DREL, tags->drel);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_DREL in tags map");

  cborEncoderResult = ConditionalAddTextStringToMap(&tagsMap, OC_RSRVD_BASE_URI, tags->baseURI);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_BASE_URI in tags map");

  {
    uint64_t value = tags->bitmap;
    cborEncoderResult = ConditionalAddIntToMap(&tagsMap, OC_RSRVD_BITMAP, &value);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_BITMAP in tags map");

    value = tags->port;
    cborEncoderResult = ConditionalAddIntToMap(&tagsMap, OC_RSRVD_HOSTING_PORT, &value);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_HOSTING_PORT in tags map");

    value = tags->ins;
    cborEncoderResult = ConditionalAddIntToMap(&tagsMap, OC_RSRVD_INS, &value);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_INS in tags map");

    value = tags->ttl;
    cborEncoderResult = ConditionalAddIntToMap(&tagsMap, OC_RSRVD_TTL, &value);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_TTL in tags map");
  }

  cborEncoderResult = cbor_encoder_close_container(setMap, &tagsMap);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close container");

exit:
  return cborEncoderResult;
}

static CborError OCLinksPayloadToCbor(OCLinksPayload *rtPtr, CborEncoder *setMap)
{
  CborEncoder linksArray;
  CborError cborEncoderResult;

  cborEncoderResult = cbor_encoder_create_array(setMap, &linksArray, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create Links array");

  while (rtPtr)
  {
    CborEncoder linksMap;
    cborEncoderResult = cbor_encoder_create_map(&linksArray, &linksMap, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create Links map");

    cborEncoderResult = ConditionalAddTextStringToMap(&linksMap, OC_RSRVD_HREF, rtPtr->href);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_HREF in Links map");

    cborEncoderResult = ConditionalAddTextStringToMap(&linksMap, OC_RSRVD_REL, rtPtr->rel);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_REL in Links map");

    cborEncoderResult = ConditionalAddTextStringToMap(&linksMap, OC_RSRVD_TITLE, rtPtr->title);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_TITLE in Links map");

    cborEncoderResult = ConditionalAddTextStringToMap(&linksMap, OC_RSRVD_URI, rtPtr->uri);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_URI in Links map");

    cborEncoderResult = AddStringLLToMap(&linksMap, OC_RSRVD_RESOURCE_TYPE, rtPtr->rt);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_RT in Links map");

    cborEncoderResult = AddStringLLToMap(&linksMap, OC_RSRVD_INTERFACE, rtPtr->itf);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_ITF in Links map");

    cborEncoderResult = AddStringLLToMap(&linksMap, OC_RSRVD_MEDIA_TYPE, rtPtr->mt);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_MT in Links map");

    {
      uint64_t value = rtPtr->ins;
      cborEncoderResult = ConditionalAddIntToMap(&linksMap, OC_RSRVD_INS, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add OC_RSRVD_INS in Links map");
    }

    cborEncoderResult = cbor_encoder_close_container(&linksArray, &linksMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing Links map");

    rtPtr = rtPtr->next;
  }
  cborEncoderResult = cbor_encoder_close_container(setMap, &linksArray);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing links array");

exit:
  return cborEncoderResult;
}

OCStackResult OCRDCborToPayload(const CborValue *cborPayload, OCPayload **outPayload)
{
  CborValue *rdCBORPayload = (CborValue *)cborPayload;
  OCStackResult ret = OC_STACK_NO_MEMORY;
  CborError cborFindResult;

  OCRDPayload *rdPayload = OCRDPayloadCreate();
  VERIFY_PARAM_NON_NULL(TAG, rdPayload, "Failed allocating rdPayload");

  ret = OC_STACK_MALFORMED_RESPONSE;

  if (cbor_value_is_array(rdCBORPayload))
  {
    OCLinksPayload *linksPayload = NULL;
    OCTagsPayload *tagsPayload = NULL;

    while (cbor_value_is_container(rdCBORPayload))
    {
      // enter tags map
      CborValue tags;
      cborFindResult = cbor_value_enter_container(rdCBORPayload, &tags);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed entering tags container.");

      cborFindResult = OCTagsCborToPayload(&tags, &tagsPayload);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed parsing tags payload.");
      OCTagsLog(DEBUG, tagsPayload);

      cborFindResult = OCLinksCborToPayload(&tags, &linksPayload);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed parsing links payload.");
      OCLinksLog(DEBUG, linksPayload);

      // Move from tags payload to links array.
      cborFindResult = cbor_value_advance(rdCBORPayload);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing rdCborPayload.");
    }
    rdPayload->rdPublish = OCCopyCollectionResource(tagsPayload, linksPayload);
    VERIFY_PARAM_NON_NULL(TAG, rdPayload->rdPublish, "Failed allocating rdPayload->rdPublish");
  }
  else if (cbor_value_is_map(rdCBORPayload))
  {
    rdPayload->rdDiscovery = (OCRDDiscoveryPayload *)OICCalloc(1, sizeof(OCRDDiscoveryPayload));
    VERIFY_PARAM_NON_NULL(TAG, rdPayload->rdDiscovery, "Failed allocating discoveryPayload");

    cborFindResult = FindStringInMap(rdCBORPayload, OC_RSRVD_DEVICE_NAME,
                                     &rdPayload->rdDiscovery->n.deviceName);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding OC_RSRVD_DEVICE_NAME.");
    char *deviceId = NULL;
    cborFindResult = FindStringInMap(rdCBORPayload, OC_RSRVD_DEVICE_ID, &deviceId);
    if (deviceId)
    {
      memcpy(rdPayload->rdDiscovery->di.id, deviceId, strlen(deviceId));
      OICFree(deviceId);
    }
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding OC_RSRVD_DEVICE_ID.");

    {
      uint64_t value = 0;
      cborFindResult = FindIntInMap(rdCBORPayload, OC_RSRVD_RD_DISCOVERY_SEL, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding OC_RSRVD_RD_DISCOVERY_SEL.");
      rdPayload->rdDiscovery->sel = value;
    }

    cborFindResult =  cbor_value_advance(rdCBORPayload);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing rdCBORPayload.");
  }
  OIC_LOG_PAYLOAD(DEBUG, (OCPayload *) rdPayload);
  *outPayload = (OCPayload *)rdPayload;
  return OC_STACK_OK;

exit:
  OCRDPayloadDestroy(rdPayload);
  return ret;
}

static CborError FindStringInMap(const CborValue *map, const char *tags, char **value)
{
  CborValue curVal;
  CborError cborFindResult = cbor_value_map_find_value(map, tags, &curVal);
  if (CborNoError == cborFindResult && cbor_value_is_text_string(&curVal))
  {
    size_t len = 0;
    cborFindResult = cbor_value_dup_text_string(&curVal, value, &len, NULL);
  }
  return cborFindResult;
}

static CborError FindIntInMap(const CborValue *map, const char *tags, uint64_t *value)
{
  CborValue curVal;
  CborError cborFindResult = cbor_value_map_find_value(map, tags, &curVal);
  if (CborNoError == cborFindResult && cbor_value_is_unsigned_integer(&curVal))
  {
    cborFindResult = cbor_value_get_uint64(&curVal, value);
  }
  return cborFindResult;
}

static CborError FindStringLLInMap(const CborValue *linksMap, const char *tag, OCStringLL **links)
{
  size_t len;
  CborValue rtArray;
  OCStringLL *llPtr = *links;
  CborError cborFindResult = cbor_value_map_find_value(linksMap, tag, &rtArray);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding tag");

  CborValue rtVal;
  cborFindResult = cbor_value_enter_container(&rtArray, &rtVal);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed entering container");

  while (cbor_value_is_text_string(&rtVal))
  {
    if (llPtr == NULL)
    {
      llPtr = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
      VERIFY_PARAM_NON_NULL(TAG, llPtr, "Failed allocating OCStringLL");
      *links = llPtr;
    }
    else if (llPtr)
    {
      while (llPtr->next)
      {
        llPtr = llPtr->next;
      }
      llPtr->next = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
      VERIFY_PARAM_NON_NULL(TAG, llPtr->next, "Failed allocating OCStringLL->next");
    }
    cborFindResult = cbor_value_dup_text_string(&rtVal, &(llPtr->value), &len, NULL);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed duplicating value");
    cborFindResult = cbor_value_advance(&rtVal);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing OCStringLL");
  }

  cborFindResult = cbor_value_leave_container(&rtArray, &rtVal);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed leaving container");

exit:
  return cborFindResult;
}

static CborError OCTagsCborToPayload(CborValue *tagsMap, OCTagsPayload **tagsPayload)
{
  CborError cborFindResult = CborErrorOutOfMemory;
  OCTagsPayload *tags = (OCTagsPayload *)OICCalloc(1, sizeof(OCTagsPayload));
  VERIFY_PARAM_NON_NULL(TAG, tags, "Failed allocating tags");

  if (cbor_value_is_map(tagsMap))
  {
    cborFindResult = FindStringInMap(tagsMap, OC_RSRVD_DEVICE_NAME, &tags->n.deviceName);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding deviceName");
    cborFindResult = FindStringInMap(tagsMap, OC_RSRVD_DREL, &tags->drel);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding drel");
    cborFindResult = FindStringInMap(tagsMap, OC_RSRVD_RTS, &tags->rts);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding rts");
    cborFindResult = FindStringInMap(tagsMap, OC_RSRVD_BASE_URI, &tags->baseURI);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding baseURI");
    char *deviceId = NULL;
    cborFindResult = FindStringInMap(tagsMap, OC_RSRVD_DEVICE_ID, &deviceId);
    if (deviceId)
    {
      memcpy(tags->di.id, deviceId, strlen(deviceId));
      OICFree(deviceId);
    }
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding deviceId");
    {
      uint64_t value = 0;
      cborFindResult = FindIntInMap(tagsMap, OC_RSRVD_HOSTING_PORT, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding port");
      tags->port = value;
      value = 0;
      cborFindResult = FindIntInMap(tagsMap, OC_RSRVD_BITMAP, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding bitmap");
      tags->bitmap = value;
      value = 0;
      cborFindResult = FindIntInMap(tagsMap, OC_RSRVD_INS, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding ins");
      tags->ins = value;
      value = 0;
      cborFindResult = FindIntInMap(tagsMap, OC_RSRVD_TTL, &value);
      tags->ttl = value;
    }
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding ttl");
    cborFindResult = cbor_value_advance(tagsMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing bitmap");
  }
  *tagsPayload = tags;
  return cborFindResult;

exit:
  OCFreeTagsResource(tags);
  return cborFindResult;
}

static CborError OCLinksCborToPayload(CborValue *linksArray, OCLinksPayload **linksPayload)
{
  CborValue linksMap;
  OCLinksPayload *setLinks = NULL;
  CborError cborFindResult = cbor_value_enter_container(linksArray, &linksMap);
  VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed entering links map container");

  while (cbor_value_is_map(&linksMap))
  {
    setLinks = (OCLinksPayload *)OICCalloc(1, sizeof(OCLinksPayload));
    VERIFY_PARAM_NON_NULL(TAG, setLinks, "Failed allocating setLinks");

    cborFindResult = FindStringInMap(&linksMap, OC_RSRVD_HREF, &setLinks->href);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding href value");

    cborFindResult = FindStringInMap(&linksMap, OC_RSRVD_REL, &setLinks->rel);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding rel value");

    cborFindResult = FindStringInMap(&linksMap, OC_RSRVD_TITLE, &setLinks->title);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding title value");

    cborFindResult = FindStringInMap(&linksMap, OC_RSRVD_URI, &setLinks->uri);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding uri value");

    cborFindResult = FindStringLLInMap(&linksMap, OC_RSRVD_RESOURCE_TYPE, &setLinks->rt);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding rt value");

    cborFindResult = FindStringLLInMap(&linksMap, OC_RSRVD_INTERFACE, &setLinks->itf);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding itf value");

    cborFindResult = FindStringLLInMap(&linksMap, OC_RSRVD_MEDIA_TYPE, &setLinks->mt);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding mt value");

    {
      uint64_t value = 0;
      cborFindResult = FindIntInMap(&linksMap, OC_RSRVD_INS, &value);
      VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed finding ins value");
      setLinks->ins = value;
    }

    if (!*linksPayload)
    {
      *linksPayload = setLinks;
    }
    else
    {
      OCLinksPayload *temp = *linksPayload;
      while (temp->next)
      {
        temp = temp->next;
      }
      temp->next = setLinks;
    }
    cborFindResult = cbor_value_advance(&linksMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing links map");
  }

  return cborFindResult;

exit:
  OCFreeLinksResource(*linksPayload);
  OCFreeLinksResource(setLinks);
  return cborFindResult;
}

static CborError AddTextStringToMap(CborEncoder *map, const char *key, const char *value)
{
  CborError err = cbor_encode_text_string(map, key, strlen(key));
  VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting key value");
  err = cbor_encode_text_string(map, value, strlen(value));
exit:
  return err;
}

static CborError ConditionalAddTextStringToMap(CborEncoder *map, const char *key, const char *value)
{
  return value ? AddTextStringToMap(map, key, value) : CborNoError;
}

static CborError ConditionalAddIntToMap(CborEncoder *map, const char *tags, const uint64_t *value)
{
  CborError err = CborNoError;
  if (*value)
  {
    err = cbor_encode_text_string(map, tags, strlen(tags));
    VERIFY_CBOR_SUCCESS(TAG, err, "failed setting value");
    err = cbor_encode_uint(map, *value);
  }
exit:
  return err;
}

static CborError AddStringLLToMap(CborEncoder *map, const char *tag, const OCStringLL *strType)
{
  CborEncoder array;
  CborError cborEncoderResult;
  cborEncoderResult = cbor_encode_text_string(map, tag, strlen(tag));
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed encoding string tag name");
  cborEncoderResult = cbor_encoder_create_array(map, &array, CborIndefiniteLength);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating stringLL array");
  while (strType)
  {
    cborEncoderResult = cbor_encode_text_string(&array, strType->value, strlen(strType->value));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed encoding string value");
    strType = strType->next;
  }
  cborEncoderResult = cbor_encoder_close_container(map, &array);
  VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing string array");
exit:
  return cborEncoderResult;
}

OCRDPayload *OCRDPayloadCreate()
{
  OCRDPayload *rdPayload = (OCRDPayload *)OICCalloc(1, sizeof(OCRDPayload));
  VERIFY_PARAM_NON_NULL(TAG, rdPayload, "Failed allocating rdPayload");
  rdPayload->base.type = PAYLOAD_TYPE_RD;

exit:
  return rdPayload;
}

OCRDDiscoveryPayload *OCRDDiscoveryPayloadCreate(const char *deviceName, const char *id, int biasFactor)
{
  OCRDDiscoveryPayload *discoveryPayload = (OCRDDiscoveryPayload *)OICCalloc(1, sizeof(OCRDDiscoveryPayload));
  VERIFY_PARAM_NON_NULL(TAG, discoveryPayload, "Failed allocating memory for discovery payload");

  if (deviceName)
  {
    discoveryPayload->n.deviceName = OICStrdup(deviceName);
    VERIFY_PARAM_NON_NULL(TAG, discoveryPayload->n.deviceName,
                          "Failed allocating memory for discovery device name");
  }
  if (id)
  {
    OICStrcpy((char *)discoveryPayload->di.id, MAX_IDENTITY_SIZE, id);
  }

  discoveryPayload->sel = biasFactor;
  return discoveryPayload;
exit:
  OICFree(discoveryPayload);
  discoveryPayload = NULL;
  return discoveryPayload;
}

void OCRDPayloadDestroy(OCRDPayload *payload)
{
  if (!payload)
  {
    return;
  }

  if (payload->rdDiscovery)
  {
    if (payload->rdDiscovery->n.deviceName)
    {
      OICFree(payload->rdDiscovery->n.deviceName);
    }
    OICFree(payload->rdDiscovery);
  }

  if (payload->rdPublish)
  {
    for (OCResourceCollectionPayload *col = payload->rdPublish; col;)
    {
      if (col->setLinks)
      {
        OCFreeLinksResource(col->setLinks);
      }

      if (col->tags)
      {
        OCFreeTagsResource(col->tags);
      }
      OCResourceCollectionPayload *temp = col->next;
      OICFree(col);
      col = temp;
    }
  }

  OICFree(payload);
}

OCTagsPayload *OCCopyTagsResources(const char *deviceName, const unsigned char *id, const char *baseURI,
                                   uint8_t bitmap, uint16_t port, uint8_t ins, const char *rts, const  char *drel, uint32_t ttl)
{
  OCTagsPayload *tags = (OCTagsPayload *)OICCalloc(1, sizeof(OCTagsPayload));
  if (!tags)
  {
    return NULL;
  }
  if (deviceName)
  {
    tags->n.deviceName = OICStrdup(deviceName);
    if (!tags->n.deviceName)
    {
      goto memory_allocation_failed;
    }
  }
  if (id)
  {
    OICStrcpy((char *)tags->di.id, MAX_IDENTITY_SIZE, (char *)id);
    if (!tags->di.id)
    {
      goto memory_allocation_failed;
    }
  }
  if (baseURI)
  {
    tags->baseURI = OICStrdup(baseURI);
    if (!tags->baseURI)
    {
      goto memory_allocation_failed;
    }
  }
  tags->bitmap = bitmap;
  tags->port = port;
  tags->ins = ins;
  if (rts)
  {
    tags->rts = OICStrdup(rts);
    if (!tags->rts)
    {
      goto memory_allocation_failed;
    }
  }
  if (drel)
  {
    tags->drel = OICStrdup(drel);
    if (!tags->drel)
    {
      goto memory_allocation_failed;
    }
  }
  tags->ttl = ttl;
  return tags;

memory_allocation_failed:
  OIC_LOG(ERROR, TAG, "Memory allocation failed.");
  OCFreeTagsResource(tags);
  return NULL;
}

OCLinksPayload *OCCopyLinksResources(const char *href, OCStringLL *rt, OCStringLL *itf,
                                     const char *rel, bool obs, const char *title, const char *uri, uint8_t ins, OCStringLL *mt)
{
  OCLinksPayload *links = (OCLinksPayload *)OICCalloc(1, sizeof(OCLinksPayload));
  if (!links)
  {
    OIC_LOG(ERROR, TAG, "Failed allocating memory.");
    return NULL;
  }
  if (href)
  {
    links->href = OICStrdup(href);
    if (!links->href)
    {
      goto memory_allocation_failed;
    }
  }
  if (rt)
  {
    links->rt = CloneOCStringLL(rt);
    if (!links->rt)
    {
      goto memory_allocation_failed;
    }
  }
  if (itf)
  {
    links->itf = CloneOCStringLL(itf);
    if (!links->itf)
    {
      goto memory_allocation_failed;
    }
  }
  if (rel)
  {
    links->rel = OICStrdup(rel);
    if (!links->rel)
    {
      goto memory_allocation_failed;
    }
  }
  links->obs = obs;
  if (title)
  {
    links->title = OICStrdup(title);
    if (!links->title)
    {
      goto memory_allocation_failed;
    }
  }
  if (uri)
  {
    links->uri = OICStrdup(uri);
    if (!links->uri)
    {
      goto memory_allocation_failed;
    }
  }
  links->ins = ins;
  if (mt)
  {
    links->mt = CloneOCStringLL(mt);
    if (!links->mt)
    {
      goto memory_allocation_failed;
    }
  }
  links->next = NULL;
  return links;

memory_allocation_failed:
  OIC_LOG(ERROR, TAG, "Memory allocation failed.");
  OCFreeLinksResource(links);
  return NULL;
}

OCResourceCollectionPayload *OCCopyCollectionResource(OCTagsPayload *tags, OCLinksPayload *links)
{
  OCResourceCollectionPayload *pl =  NULL;
  VERIFY_PARAM_NON_NULL(TAG, tags, "Invalid param tags");
  VERIFY_PARAM_NON_NULL(TAG, links, "Invalid param links");

  pl = (OCResourceCollectionPayload *)OICCalloc(1, sizeof(OCResourceCollectionPayload));
  VERIFY_PARAM_NON_NULL(TAG, pl, "Failed allocating memory for the OCResourceCollectionPayload");

  pl->tags = tags;
  pl->setLinks = links;

exit:
  return pl;
}

void OCFreeLinksResource(OCLinksPayload *payload)
{
  if (!payload)
  {
    return;
  }
  OICFree(payload->href);
  OCFreeOCStringLL(payload->rt);
  OCFreeOCStringLL(payload->itf);
  OICFree(payload->rel);
  OICFree(payload->title);
  OICFree(payload->uri);
  OCFreeOCStringLL(payload->mt);
  OCFreeLinksResource(payload->next);
  OICFree(payload);
}

void OCFreeTagsResource(OCTagsPayload *payload)
{
  if (!payload)
  {
    return;
  }
  OICFree(payload->n.deviceName);
  OICFree(payload->baseURI);
  OICFree(payload->rts);
  OICFree(payload->drel);
  OICFree(payload);
}

void OCFreeCollectionResource(OCResourceCollectionPayload *payload)
{
  if (!payload)
  {
    return;
  }
  if (payload->tags)
  {
    OCFreeTagsResource(payload->tags);
  }
  if (payload->setLinks)
  {
    OCFreeLinksResource(payload->setLinks);
  }
  OCFreeCollectionResource(payload->next);
  OICFree(payload);
}

void OCTagsLog(const LogLevel level, const OCTagsPayload *tags)
{
  if (tags)
  {
    if (tags->n.deviceName)
    {
      OIC_LOG_V(level, TAG, " Device Name : %s ", tags->n.deviceName);
    }
    if (tags->baseURI)
    {
      OIC_LOG_V(level, TAG, " Base URI : %s ", tags->baseURI);
    }
    OIC_LOG_V(level, TAG, " Device ID : %s ", tags->di.id);
    OIC_LOG_V(level, TAG, " Bitmap : %d ", tags->bitmap);
    OIC_LOG_V(level, TAG, " Port : %d ", tags->port);
    OIC_LOG_V(level, TAG, " Ins : %d ", tags->ins);
    OIC_LOG_V(level, TAG, " Ttl : %d ", tags->ttl);

    if (tags->rts)
    {
      OIC_LOG_V(level, TAG, " RTS : %s ", tags->rts);
    }
    if (tags->drel)
    {
      OIC_LOG_V(level, TAG, " DREL : %s ", tags->drel);
    }
  }
  else
  {
    (void) level;
  }
}

void OCLinksLog(const LogLevel level, const OCLinksPayload *links)
{
  while (links)
  {
    if (links->href)
    {
      OIC_LOG_V(level, TAG, " href: %s ", links->href);
    }
    OIC_LOG(level, TAG, " RT: ");
    OCStringLL *rt = links->rt;
    while (rt)
    {
      if (rt->value)
      {
        OIC_LOG_V(level, TAG, "   %s", rt->value);
      }
      rt = rt->next;
    }
    OIC_LOG(level, TAG, " IF: ");
    OCStringLL *itf = links->itf;
    while (itf)
    {
      if (itf->value)
      {
        OIC_LOG_V(level, TAG, "   %s", itf->value);
      }
      itf = itf->next;
    }
    OIC_LOG(level, TAG, " MT: ");
    OCStringLL *mt = links->mt;
    while (mt)
    {
      if (mt->value)
      {
        OIC_LOG_V(level, TAG, "   %s", mt->value);
      }
      mt = mt->next;
    }
    OIC_LOG_V(level, TAG, " INS: %d", links->ins);
    OIC_LOG_V(level, TAG, " OBS: %d", links->obs);
    if (links->rel)
    {
      OIC_LOG_V(level, TAG, " REL: %s", links->rel);
    }
    if (links->title)
    {
      OIC_LOG_V(level, TAG, " TITLE: %s", links->title);
    }
    if (links->uri)
    {
      OIC_LOG_V(level, TAG, " URI: %s", links->uri);
    }
    links = links->next;
  }
  if (!links)
  {
    (void) level;
  }
}
