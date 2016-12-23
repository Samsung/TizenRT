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

#ifndef PAYLOAD_LOGGING_H_
#define PAYLOAD_LOGGING_H_

#include "logger.h"
#ifdef __TIZEN__
#include <dlog.h>
#endif

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <inttypes.h>
#include "rdpayload.h"

#ifdef __cplusplus
extern "C"
{
#endif

// PL_TAG is made as generic predefined tag because of build problems in arduino for using logging
#define PL_TAG "PayloadLog"

#ifdef TB_LOG
#define OIC_LOG_PAYLOAD(level, payload) OCPayloadLog((level),(payload))
#define UUID_SIZE (16)
const char *convertTriggerEnumToString(OCPresenceTrigger trigger);
OCPresenceTrigger convertTriggerStringToEnum(const char *triggerStr);

static inline void OCPayloadLogRep(LogLevel level, OCRepPayload *payload)
{
  OIC_LOG(level, (PL_TAG), "Payload Type: Representation");
  OCRepPayload *rep = payload;
  int i = 1;
  while (rep)
  {
    OIC_LOG_V(level, PL_TAG, "\tResource #%d", i);
    OIC_LOG_V(level, PL_TAG, "\tURI:%s", rep->uri);
    OIC_LOG(level, PL_TAG, "\tResource Types:");
    OCStringLL *strll =  rep->types;
    while (strll)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
      strll = strll->next;
    }
    OIC_LOG(level, PL_TAG, "\tInterfaces:");
    strll =  rep->interfaces;
    while (strll)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
      strll = strll->next;
    }

    // TODO Finish Logging: Values
    OCRepPayloadValue *val = rep->values;

    OIC_LOG(level, PL_TAG, "\tValues:");

    while (val)
    {
      switch (val->type)
      {
        case OCREP_PROP_NULL:
          OIC_LOG_V(level, PL_TAG, "\t\t%s: NULL", val->name);
          break;
        case OCREP_PROP_INT:
          OIC_LOG_V(level, PL_TAG, "\t\t%s(int):%zd", val->name, val->i);
          break;
        case OCREP_PROP_DOUBLE:
          OIC_LOG_V(level, PL_TAG, "\t\t%s(double):%f", val->name, val->d);
          break;
        case OCREP_PROP_BOOL:
          OIC_LOG_V(level, PL_TAG, "\t\t%s(bool):%s", val->name, val->b ? "true" : "false");
          break;
        case OCREP_PROP_STRING:
          OIC_LOG_V(level, PL_TAG, "\t\t%s(string):%s", val->name, val->str);
          break;
        case OCREP_PROP_BYTE_STRING:
          OIC_LOG_V(level, PL_TAG, "\t\t%s(binary):", val->name);
          OIC_LOG_BUFFER(level, PL_TAG, val->ocByteStr.bytes, val->ocByteStr.len);
          break;
        case OCREP_PROP_OBJECT:
          // Note: Only prints the URI (if available), to print further, you'll
          // need to dig into the object better!
          OIC_LOG_V(level, PL_TAG, "\t\t%s(OCRep):%s", val->name, val->obj->uri);
          break;
        case OCREP_PROP_ARRAY:
          switch (val->arr.type)
          {
            case OCREP_PROP_INT:
              OIC_LOG_V(level, PL_TAG, "\t\t%s(int array):%zu x %zu x %zu",
                        val->name,
                        val->arr.dimensions[0], val->arr.dimensions[1],
                        val->arr.dimensions[2]);
              break;
            case OCREP_PROP_DOUBLE:
              OIC_LOG_V(level, PL_TAG, "\t\t%s(double array):%zu x %zu x %zu",
                        val->name,
                        val->arr.dimensions[0], val->arr.dimensions[1],
                        val->arr.dimensions[2]);
              break;
            case OCREP_PROP_BOOL:
              OIC_LOG_V(level, PL_TAG, "\t\t%s(bool array):%zu x %zu x %zu",
                        val->name,
                        val->arr.dimensions[0], val->arr.dimensions[1],
                        val->arr.dimensions[2]);
              break;
            case OCREP_PROP_STRING:
              OIC_LOG_V(level, PL_TAG, "\t\t%s(string array):%zu x %zu x %zu",
                        val->name,
                        val->arr.dimensions[0], val->arr.dimensions[1],
                        val->arr.dimensions[2]);
              break;
            case OCREP_PROP_BYTE_STRING:
              OIC_LOG_V(level, PL_TAG, "\t\t%s(byte array):%zu x %zu x %zu",
                        val->name,
                        val->arr.dimensions[0], val->arr.dimensions[1],
                        val->arr.dimensions[2]);
              break;
            case OCREP_PROP_OBJECT:
              OIC_LOG_V(level, PL_TAG, "\t\t%s(OCRep array):%zu x %zu x %zu",
                        val->name,
                        val->arr.dimensions[0], val->arr.dimensions[1],
                        val->arr.dimensions[2]);
              break;
            default:
              OIC_LOG_V(ERROR, PL_TAG, "\t\t%s <-- Unknown/unsupported array type!",
                        val->name);
              break;
          }
          break;
        default:
          OIC_LOG_V(ERROR, PL_TAG, "\t\t%s <-- Unknown type!", val->name);
          break;
      }
      val = val -> next;
    }

    ++i;
    rep = rep->next;
  }

}

static inline void OCPayloadLogDiscovery(LogLevel level, OCDiscoveryPayload *payload)
{
  OIC_LOG(level, PL_TAG, "Payload Type: Discovery");
  int i = 1;

  if (!payload->resources)
  {
    OIC_LOG(level, PL_TAG, "\tNO Resources");
    return;
  }
  OIC_LOG_V(level, PL_TAG, "\tSID: %s", payload->sid);
  if (payload->baseURI)
  {
    OIC_LOG_V(level, PL_TAG, "\tBase URI:%s", payload->baseURI);
  }
  if (payload->name)
  {
    OIC_LOG_V(level, PL_TAG, "\tNAME: %s", payload->name);
  }
  if (payload->uri)
  {
    OIC_LOG_V(level, PL_TAG, "\tURI: %s", payload->uri);
  }
  if (payload->type)
  {
    OIC_LOG_V(level, PL_TAG, "\tResource Type: %s", payload->type);
  }
  OIC_LOG(level, PL_TAG, "\tInterface:");
  for (OCStringLL *itf = payload->interface; itf; itf = itf->next)
  {
    OIC_LOG_V(level, PL_TAG, "\t\t%s", itf->value);
  }

  OCResourcePayload *res = payload->resources;

  while (res)
  {
    OIC_LOG_V(level, PL_TAG, "\tResource #%d", i);
    OIC_LOG_V(level, PL_TAG, "\tURI:%s", res->uri);
    OIC_LOG(level, PL_TAG, "\tResource Types:");
    OCStringLL *strll =  res->types;
    while (strll)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
      strll = strll->next;
    }
    OIC_LOG(level, PL_TAG, "\tInterfaces:");
    strll =  res->interfaces;
    while (strll)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
      strll = strll->next;
    }

    OIC_LOG_V(level, PL_TAG, "\tBitmap: %u", res->bitmap);
    OIC_LOG_V(level, PL_TAG, "\tSecure?: %s", res->secure ? "true" : "false");
    OIC_LOG_V(level, PL_TAG, "\tPort: %u", res->port);
    OIC_LOG(level, PL_TAG, "");
    res = res->next;
    ++i;
  }
}

static inline void OCPayloadLogDevice(LogLevel level, OCDevicePayload *payload)
{
  OIC_LOG(level, PL_TAG, "Payload Type: Device");
  OIC_LOG_V(level, PL_TAG, "\tSID:%s", payload->sid);
  OIC_LOG_V(level, PL_TAG, "\tDevice Name:%s", payload->deviceName);
  OIC_LOG_V(level, PL_TAG, "\tSpec Version%s", payload->specVersion);
  OIC_LOG_V(level, PL_TAG, "\tData Model Version:%s", payload->dataModelVersion);
  if (payload->types)
  {
    OIC_LOG(level, PL_TAG, "\tResource Type:");
    for (OCStringLL *strll = payload->types; strll; strll = strll->next)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
    }
  }
  if (payload->interfaces)
  {
    OIC_LOG(level, PL_TAG, "\tInterface:");
    for (OCStringLL *strll = payload->interfaces; strll; strll = strll->next)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
    }
  }
}

static inline void OCPayloadLogPlatform(LogLevel level, OCPlatformPayload *payload)
{
  OIC_LOG(level, PL_TAG, "Payload Type: Platform");
  OIC_LOG_V(level, PL_TAG, "\tURI:%s", payload->uri);
  OIC_LOG_V(level, PL_TAG, "\tPlatform ID:%s", payload->info.platformID);
  OIC_LOG_V(level, PL_TAG, "\tMfg Name:%s", payload->info.manufacturerName);
  OIC_LOG_V(level, PL_TAG, "\tMfg URL:%s", payload->info.manufacturerUrl);
  OIC_LOG_V(level, PL_TAG, "\tModel Number:%s", payload->info.modelNumber);
  OIC_LOG_V(level, PL_TAG, "\tDate of Mfg:%s", payload->info.dateOfManufacture);
  OIC_LOG_V(level, PL_TAG, "\tPlatform Version:%s", payload->info.platformVersion);
  OIC_LOG_V(level, PL_TAG, "\tOS Version:%s", payload->info.operatingSystemVersion);
  OIC_LOG_V(level, PL_TAG, "\tHardware Version:%s", payload->info.hardwareVersion);
  OIC_LOG_V(level, PL_TAG, "\tFirmware Version:%s", payload->info.firmwareVersion);
  OIC_LOG_V(level, PL_TAG, "\tSupport URL:%s", payload->info.supportUrl);
  OIC_LOG_V(level, PL_TAG, "\tSystem Time:%s", payload->info.systemTime);

  if (payload->rt)
  {
    OIC_LOG(level, PL_TAG, "\tResource Types:");
    OIC_LOG_V(level, PL_TAG, "\t\t%s", payload->rt);
  }
  if (payload->interfaces)
  {
    OIC_LOG(level, PL_TAG, "\tResource Interfaces:");
    for (OCStringLL *strll = payload->interfaces; strll; strll = strll->next)
    {
      OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
    }
  }
}

static inline void OCPayloadLogPresence(LogLevel level, OCPresencePayload *payload)
{
  OIC_LOG(level, PL_TAG, "Payload Type: Presence");
  OIC_LOG_V(level, PL_TAG, "\tSequence Number:%u", payload->sequenceNumber);
  OIC_LOG_V(level, PL_TAG, "\tMax Age:%d", payload->maxAge);
  OIC_LOG_V(level, PL_TAG, "\tTrigger:%s", convertTriggerEnumToString(payload->trigger));
  OIC_LOG_V(level, PL_TAG, "\tResource Type:%s", payload->resourceType);
}

static inline void OCPayloadLogSecurity(LogLevel level, OCSecurityPayload *payload)
{
  OIC_LOG(level, PL_TAG, "Payload Type: Security");
  OIC_LOG_V(level, PL_TAG, "\tSecurity Data: %s", payload->securityData);
}

static inline void OCRDPayloadLog(const LogLevel level, const OCRDPayload *payload)
{
  if (!payload)
  {
    return;
  }

  if (payload->rdDiscovery)
  {
    OIC_LOG(level, PL_TAG, "RD Discovery");
    OIC_LOG_V(level, PL_TAG, "  Device Name : %s", payload->rdDiscovery->n.deviceName);
    OIC_LOG_V(level, PL_TAG, "  Device Identity : %s", payload->rdDiscovery->di.id);
    OIC_LOG_V(level, PL_TAG, "  Bias: %d", payload->rdDiscovery->sel);
  }
  if (payload->rdPublish)
  {
    OIC_LOG(level, PL_TAG, "RD Publish");
    OCResourceCollectionPayload *rdPublish = payload->rdPublish;
    OCTagsLog(level, rdPublish->tags);
    OCLinksLog(level, rdPublish->setLinks);
  }
}

static inline void OCPayloadLog(LogLevel level, OCPayload *payload)
{
  if (!payload)
  {
    OIC_LOG(level, PL_TAG, "NULL Payload");
    return;
  }
  switch (payload->type)
  {
    case PAYLOAD_TYPE_REPRESENTATION:
      OCPayloadLogRep(level, (OCRepPayload *)payload);
      break;
    case PAYLOAD_TYPE_DISCOVERY:
      OCPayloadLogDiscovery(level, (OCDiscoveryPayload *)payload);
      break;
    case PAYLOAD_TYPE_DEVICE:
      OCPayloadLogDevice(level, (OCDevicePayload *)payload);
      break;
    case PAYLOAD_TYPE_PLATFORM:
      OCPayloadLogPlatform(level, (OCPlatformPayload *)payload);
      break;
    case PAYLOAD_TYPE_PRESENCE:
      OCPayloadLogPresence(level, (OCPresencePayload *)payload);
      break;
    case PAYLOAD_TYPE_SECURITY:
      OCPayloadLogSecurity(level, (OCSecurityPayload *)payload);
      break;
    case PAYLOAD_TYPE_RD:
      OCRDPayloadLog(level, (OCRDPayload *)payload);
      break;
    default:
      OIC_LOG_V(level, PL_TAG, "Unknown Payload Type: %d", payload->type);
      break;
  }
}
#else
#define OIC_LOG_PAYLOAD(level, payload)
#endif

#ifdef __cplusplus
}
#endif

#endif
