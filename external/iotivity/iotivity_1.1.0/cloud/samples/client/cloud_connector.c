//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include "cloud_connector.h"

#include <stdarg.h>
#include <stdlib.h>

#include "oic_string.h"
#include "oic_malloc.h"

#include "ocpayload.h"

#include "rdpayload.h"

#define OC_RD_PUBLISH_TTL 86400
#define DEFAULT_CONTEXT_VALUE 0x99

#define DEFAULT_COAP_TCP_HOST "coap+tcp://"
#define DEFAULT_COAP_TCP_PORT 5683

#define DEFAULT_COAP_TCP_SECURE_HOST "coaps+tcp://"
#define DEFAULT_COAP_TCP_SECURE_PORT 5864

#define DEFAULT_AUTH_REGISTER_LOGIN "/oic/auth/?reqtype=register"
#define DEFAULT_AUTH_LOGIN "/oic/auth/?reqtype=login"
#define DEFAULT_AUTH_LOGOUT "/oic/auth/?reqtype=logout"

static OCStackResult createStringLL(uint8_t numElements, OCResourceHandle handle,
                                    const char * (*getValue)(OCResourceHandle handle, uint8_t i), OCStringLL **stringLL)
{
  for (uint8_t i = 0; i < numElements; ++i)
  {
    const char *value = getValue(handle, i);
    if (!*stringLL)
    {
      *stringLL = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
      if (!*stringLL)
      {
        return OC_STACK_NO_MEMORY;
      }
      (*stringLL)->value = OICStrdup(value);
      if (!(*stringLL)->value)
      {
        return OC_STACK_NO_MEMORY;
      }
    }
    else
    {
      OCStringLL *cur = *stringLL;
      while (cur->next)
      {
        cur = cur->next;
      }
      cur->next = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
      if (!cur->next)
      {
        return OC_STACK_NO_MEMORY;
      }
      cur->next->value = OICStrdup(value);
      if (!cur->next->value)
      {
        return OC_STACK_NO_MEMORY;
      }
    }
  }
  return OC_STACK_OK;
}

OCStackResult OCCloudRegisterLogin(const char *host, const char *auth_provider,
                                   const char *auth_code, OCClientResponseHandler response)
{
  char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
  snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, DEFAULT_AUTH_REGISTER_LOGIN);

  OCCallbackData cbData;
  memset(&cbData, 0, sizeof(OCCallbackData));
  cbData.cb = response;
  cbData.cd = NULL;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

  OCRepPayload *registerPayload = OCRepPayloadCreate();
  if (!registerPayload)
  {
    goto no_memory;
  }

  OCRepPayloadSetPropString(registerPayload, "authprovider", auth_provider);
  OCRepPayloadSetPropString(registerPayload, "authcode", auth_code);

  return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)registerPayload,
                      CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
  OCRepPayloadDestroy(registerPayload);
  return OC_STACK_NO_MEMORY;
}

OCStackResult OCCloudLoginout(const char *host, const char *query, const char *auth_session,
                              OCClientResponseHandler response)
{
  char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
  snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

  OCCallbackData cbData;
  memset(&cbData, 0, sizeof(OCCallbackData));
  cbData.cb = response;
  cbData.cd = NULL;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

  OCRepPayload *loginoutPayload = OCRepPayloadCreate();
  if (!loginoutPayload)
  {
    goto no_memory;
  }

  OCRepPayloadSetPropString(loginoutPayload, "session", auth_session);

  return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)loginoutPayload,
                      CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
  OCRepPayloadDestroy(loginoutPayload);
  return OC_STACK_NO_MEMORY;
}


OCStackResult OCCloudLogin(const char *host, const char *auth_session,
                           OCClientResponseHandler response)
{
  return OCCloudLoginout(host, DEFAULT_AUTH_LOGIN, auth_session, response);
}

OCStackResult OCCloudLogout(const char *host, const char *auth_session,
                            OCClientResponseHandler response)
{
  return OCCloudLoginout(host, DEFAULT_AUTH_LOGOUT, auth_session, response);
}

OCStackResult OCCloudPublish(const char *host, const char *query,
                             OCClientResponseHandler response, int numArg, ...)
{
  char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
  snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

  // Gather all resources locally and do publish
  OCCallbackData cbData;
  memset(&cbData, 0, sizeof(OCCallbackData));
  cbData.cb = response;
  cbData.cd = NULL;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

  OCTagsPayload *tagsPayload = NULL;
  OCLinksPayload *linksPayload = NULL;
  OCStringLL *rt = NULL;
  OCStringLL *itf = NULL;
  OCStringLL *mt = NULL;

  OCRDPayload *rdPayload = OCRDPayloadCreate();
  if (!rdPayload)
  {
    goto no_memory;
  }

  const unsigned char *id = (unsigned char *)OCGetServerInstanceIDString();
  tagsPayload = OCCopyTagsResources(NULL, id,
                                    NULL, OC_DISCOVERABLE, 0, 0, NULL, NULL, OC_RD_PUBLISH_TTL);
  if (!tagsPayload)
  {
    goto no_memory;
  }

  va_list arguments;
  va_start(arguments, numArg);

  for (int j = 0; j < numArg; j++)
  {
    OCResourceHandle handle = va_arg(arguments, OCResourceHandle);
    if (handle)
    {
      rt = itf = mt = NULL;
      const char *uri = OCGetResourceUri(handle);
      uint8_t numElement;
      if (OC_STACK_OK == OCGetNumberOfResourceTypes(handle, &numElement))
      {
        OCStackResult res = createStringLL(numElement, handle, OCGetResourceTypeName, &rt);
        if (res != OC_STACK_OK || !rt)
        {
          goto no_memory;
        }
      }

      if (OC_STACK_OK == OCGetNumberOfResourceInterfaces(handle, &numElement))
      {
        OCStackResult res = createStringLL(numElement, handle, OCGetResourceInterfaceName, &itf);
        if (res != OC_STACK_OK || !itf)
        {
          goto no_memory;
        }
      }

      mt = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
      if (!mt)
      {
        goto no_memory;
      }
      mt->value = OICStrdup("application/cbor");
      if (!mt->value)
      {
        goto no_memory;
      }

      if (!linksPayload)
      {
        linksPayload = OCCopyLinksResources(uri, rt, itf, NULL, 0, NULL,
                                            NULL, j, mt);;
        if (!linksPayload)
        {
          goto no_memory;
        }
      }
      else
      {
        OCLinksPayload *temp = linksPayload;
        while (temp->next)
        {
          temp = temp->next;
        }
        temp->next = OCCopyLinksResources(uri, rt, itf, NULL, 0, NULL,
                                          NULL, j, mt);
        if (!temp->next)
        {
          goto no_memory;
        }
      }
      OCFreeOCStringLL(rt);
      OCFreeOCStringLL(itf);
      OCFreeOCStringLL(mt);
    }
  }
  va_end(arguments);

  rdPayload->rdPublish = OCCopyCollectionResource(tagsPayload, linksPayload);
  if (!rdPayload->rdPublish)
  {
    goto no_memory;
  }

  return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)rdPayload,
                      CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
  va_end(arguments);
  if (rt)
  {
    OCFreeOCStringLL(rt);
  }
  if (itf)
  {
    OCFreeOCStringLL(itf);
  }
  if (mt)
  {
    OCFreeOCStringLL(mt);
  }
  if (tagsPayload)
  {
    OCFreeTagsResource(tagsPayload);
  }
  if (linksPayload)
  {
    OCFreeLinksResource(linksPayload);
  }
  OCRDPayloadDestroy(rdPayload);
  return OC_STACK_NO_MEMORY;
}
