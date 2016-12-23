//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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
#define _POSIX_C_SOURCE 200112L

#if defined(WITH_ARDUINO) || defined(__TINYARA__)
#include <string.h>
#else
#include <strings.h>
#endif

#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocobserve.h"
#include "occollection.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "cJSON.h"
#include "ocpayload.h"
#include "secureresourcemanager.h"
#include "cacommon.h"
#include "cainterface.h"
#include "rdpayload.h"
#include "ocpayload.h"

#ifdef WITH_RD
#include "rd_server.h"
#endif

#ifdef ROUTING_GATEWAY
#include "routingmanager.h"
#endif

/// Module Name
#define TAG "OIC_RI_RESOURCE"

#define VERIFY_SUCCESS(op, successCode) { if (op != successCode) \
            {OIC_LOG_V(FATAL, TAG, "%s failed!!", #op); goto exit;} }

#define VERIFY_NON_NULL(arg, logLevel, retVal) { if (!(arg)) { OIC_LOG((logLevel), \
             TAG, #arg " is NULL"); return (retVal); } }

extern OCResource *headResource;
static OCPlatformInfo savedPlatformInfo = {0};
static OCDeviceInfo savedDeviceInfo = {0};

/**
 * Prepares a Payload for response.
 */
static OCStackResult BuildVirtualResourceResponse(const OCResource *resourcePtr,
                                                  OCDiscoveryPayload *payload,
                                                  OCDevAddr *endpoint,
                                                  bool rdResponse);

//-----------------------------------------------------------------------------
// Default resource entity handler function
//-----------------------------------------------------------------------------
OCEntityHandlerResult defaultResourceEHandler(OCEntityHandlerFlag flag,
                                              OCEntityHandlerRequest *request, void *callbackParam)
{
  //TODO ("Implement me!!!!");
  // TODO:  remove silence unused param warnings
  (void) flag;
  (void) request;
  (void) callbackParam;
  return  OC_EH_OK; // Making sure that the Default EH and the Vendor EH have matching signatures
}

/* This method will retrieve the port at which the secure resource is hosted */
static OCStackResult GetSecurePortInfo(OCDevAddr *endpoint, uint16_t *port)
{
  uint16_t p = 0;

  if (endpoint->adapter == OC_ADAPTER_IP)
  {
    if (endpoint->flags & OC_IP_USE_V6)
    {
      p = caglobals.ip.u6s.port;
    }
    else if (endpoint->flags & OC_IP_USE_V4)
    {
      p = caglobals.ip.u4s.port;
    }
  }

  *port = p;
  return OC_STACK_OK;
}

/*
 * Function will extract 0, 1 or 2 filters from query.
 * More than 2 filters or unsupported filters will result in error.
 * If both filters are of the same supported type, the 2nd one will be picked.
 * Resource and device filters in the SAME query are NOT validated
 * and resources will likely not clear filters.
 */
static OCStackResult ExtractFiltersFromQuery(char *query, char **filterOne, char **filterTwo)
{

  char *key = NULL;
  char *value = NULL;
  char *restOfQuery = NULL;
  int numKeyValuePairsParsed = 0;

  *filterOne = NULL;
  *filterTwo = NULL;

  OIC_LOG_V(INFO, TAG, "Extracting params from %s", query);

  char *keyValuePair = strtok_r(query, OC_QUERY_SEPARATOR, &restOfQuery);

  while (keyValuePair)
  {
    if (numKeyValuePairsParsed >= 2)
    {
      OIC_LOG(ERROR, TAG, "More than 2 queries params in URI.");
      return OC_STACK_INVALID_QUERY;
    }

    key = strtok_r(keyValuePair, OC_KEY_VALUE_DELIMITER, &value);

    if (!key || !value)
    {
      return OC_STACK_INVALID_QUERY;
    }
    else if (strncasecmp(key, OC_RSRVD_INTERFACE, sizeof(OC_RSRVD_INTERFACE) - 1) == 0)
    {
      *filterOne = value;     // if
    }
    else if (strncasecmp(key, OC_RSRVD_RESOURCE_TYPE, sizeof(OC_RSRVD_INTERFACE) - 1) == 0)
    {
      *filterTwo = value;     // rt
    }
    else
    {
      OIC_LOG_V(ERROR, TAG, "Unsupported query key: %s", key);
      return OC_STACK_INVALID_QUERY;
    }
    ++numKeyValuePairsParsed;

    keyValuePair = strtok_r(NULL, OC_QUERY_SEPARATOR, &restOfQuery);
  }

  OIC_LOG_V(INFO, TAG, "Extracted params if: %s and rt: %s.", *filterOne, *filterTwo);
  return OC_STACK_OK;
}

static OCVirtualResources GetTypeOfVirtualURI(const char *uriInRequest)
{
  if (strcmp(uriInRequest, OC_RSRVD_WELL_KNOWN_URI) == 0)
  {
    return OC_WELL_KNOWN_URI;
  }
  else if (strcmp(uriInRequest, OC_RSRVD_DEVICE_URI) == 0)
  {
    return OC_DEVICE_URI;
  }
  else if (strcmp(uriInRequest, OC_RSRVD_PLATFORM_URI) == 0)
  {
    return OC_PLATFORM_URI;
  }
  else if (strcmp(uriInRequest, OC_RSRVD_RESOURCE_TYPES_URI) == 0)
  {
    return OC_RESOURCE_TYPES_URI;
  }
#ifdef ROUTING_GATEWAY
  else if (0 == strcmp(uriInRequest, OC_RSRVD_GATEWAY_URI))
  {
    return OC_GATEWAY_URI;
  }
#endif
#ifdef WITH_PRESENCE
  else if (strcmp(uriInRequest, OC_RSRVD_PRESENCE_URI) == 0)
  {
    return OC_PRESENCE;
  }
#endif //WITH_PRESENCE
  return OC_UNKNOWN_URI;
}

static OCStackResult getQueryParamsForFiltering(OCVirtualResources uri, char *query,
                                                char **filterOne, char **filterTwo)
{
  if (!filterOne || !filterTwo)
  {
    return OC_STACK_INVALID_PARAM;
  }

  *filterOne = NULL;
  *filterTwo = NULL;

#ifdef WITH_PRESENCE
  if (uri == OC_PRESENCE)
  {
    //Nothing needs to be done, except for pass a OC_PRESENCE query through as OC_STACK_OK.
    OIC_LOG(INFO, TAG, "OC_PRESENCE Request for virtual resource.");
    return OC_STACK_OK;
  }
#endif

  OCStackResult result = OC_STACK_OK;

  if (query && *query)
  {
    result = ExtractFiltersFromQuery(query, filterOne, filterTwo);
  }

  return result;
}

OCStackResult BuildResponseRepresentation(const OCResource *resourcePtr,
                                          OCRepPayload **payload)
{
  OCRepPayload *tempPayload = OCRepPayloadCreate();

  if (!resourcePtr)
  {
    OCRepPayloadDestroy(tempPayload);
    return OC_STACK_INVALID_PARAM;
  }

  if (!tempPayload)
  {
    return OC_STACK_NO_MEMORY;
  }

  OCRepPayloadSetUri(tempPayload, resourcePtr->uri);

  OCResourceType *resType = resourcePtr->rsrcType;
  while (resType)
  {
    OCRepPayloadAddResourceType(tempPayload, resType->resourcetypename);
    resType = resType->next;
  }

  OCResourceInterface *resInterface = resourcePtr->rsrcInterface;
  while (resInterface)
  {
    OCRepPayloadAddInterface(tempPayload, resInterface->name);
    resInterface = resInterface->next;
  }

  OCAttribute *resAttrib = resourcePtr->rsrcAttributes;
  while (resAttrib)
  {
    OCRepPayloadSetPropString(tempPayload, resAttrib->attrName,
                              resAttrib->attrValue);
    resAttrib = resAttrib->next;
  }

  if (!*payload)
  {
    *payload = tempPayload;
  }
  else
  {
    OCRepPayloadAppend(*payload, tempPayload);
  }

  return OC_STACK_OK;
}

OCStackResult BuildVirtualResourceResponse(const OCResource *resourcePtr,
                                           OCDiscoveryPayload *payload, OCDevAddr *devAddr, bool rdResponse)
{
  if (!resourcePtr || !payload)
  {
    return OC_STACK_INVALID_PARAM;
  }
  uint16_t port = 0;
  if (resourcePtr->resourceProperties & OC_SECURE)
  {
    if (GetSecurePortInfo(devAddr, &port) != OC_STACK_OK)
    {
      port = 0;
    }
  }

  if (rdResponse)
  {
    port = devAddr->port;
  }

  OCDiscoveryPayloadAddResource(payload, resourcePtr, port);
  return OC_STACK_OK;
}

uint8_t IsCollectionResource(OCResource *resource)
{
  if (!resource)
  {
    return 0;
  }

  if (resource->rsrcChildResourcesHead != NULL)
  {
    return 1;
  }

  return 0;
}

OCResource *FindResourceByUri(const char *resourceUri)
{
  if (!resourceUri)
  {
    return NULL;
  }

  OCResource *pointer = headResource;
  while (pointer)
  {
    if (strcmp(resourceUri, pointer->uri) == 0)
    {
      return pointer;
    }
    pointer = pointer->next;
  }
  OIC_LOG_V(INFO, TAG, "Resource %s not found", resourceUri);
  return NULL;
}


OCStackResult DetermineResourceHandling(const OCServerRequest *request,
                                        ResourceHandling *handling,
                                        OCResource **resource)
{
  if (!request || !handling || !resource)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OIC_LOG_V(INFO, TAG, "DetermineResourceHandling for %s", request->resourceUrl);

  // Check if virtual resource
  if (GetTypeOfVirtualURI(request->resourceUrl) != OC_UNKNOWN_URI)
  {
    OIC_LOG_V(INFO, TAG, "%s is virtual", request->resourceUrl);
    *handling = OC_RESOURCE_VIRTUAL;
    *resource = headResource;
    return OC_STACK_OK;
  }
  if (strlen((const char *)(request->resourceUrl)) == 0)
  {
    // Resource URL not specified
    *handling = OC_RESOURCE_NOT_SPECIFIED;
    return OC_STACK_NO_RESOURCE;
  }
  else
  {
    OCResource *resourcePtr = FindResourceByUri((const char *)request->resourceUrl);
    *resource = resourcePtr;
    if (!resourcePtr)
    {
      if (defaultDeviceHandler)
      {
        *handling = OC_RESOURCE_DEFAULT_DEVICE_ENTITYHANDLER;
        return OC_STACK_OK;
      }

      // Resource does not exist
      // and default device handler does not exist
      *handling = OC_RESOURCE_NOT_SPECIFIED;
      return OC_STACK_NO_RESOURCE;
    }

    if (IsCollectionResource(resourcePtr))
    {
      // Collection resource
      if (resourcePtr->entityHandler != defaultResourceEHandler)
      {
        *handling = OC_RESOURCE_COLLECTION_WITH_ENTITYHANDLER;
        return OC_STACK_OK;
      }
      else
      {
        *handling = OC_RESOURCE_COLLECTION_DEFAULT_ENTITYHANDLER;
        return OC_STACK_OK;
      }
    }
    else
    {
      // Resource not a collection
      if (resourcePtr->entityHandler != defaultResourceEHandler)
      {
        *handling = OC_RESOURCE_NOT_COLLECTION_WITH_ENTITYHANDLER;
        return OC_STACK_OK;
      }
      else
      {
        *handling = OC_RESOURCE_NOT_COLLECTION_DEFAULT_ENTITYHANDLER;
        return OC_STACK_OK;
      }
    }
  }
}

OCStackResult EntityHandlerCodeToOCStackCode(OCEntityHandlerResult ehResult)
{
  OCStackResult result;

  switch (ehResult)
  {
    case OC_EH_OK:
      result = OC_STACK_OK;
      break;
    case OC_EH_SLOW:
      result = OC_STACK_SLOW_RESOURCE;
      break;
    case OC_EH_ERROR:
      result = OC_STACK_ERROR;
      break;
    case OC_EH_FORBIDDEN:
      result = OC_STACK_RESOURCE_ERROR;
      break;
    case OC_EH_RESOURCE_CREATED:
      result = OC_STACK_RESOURCE_CREATED;
      break;
    case OC_EH_RESOURCE_DELETED:
      result = OC_STACK_RESOURCE_DELETED;
      break;
    case OC_EH_RESOURCE_NOT_FOUND:
      result = OC_STACK_NO_RESOURCE;
      break;
    default:
      result = OC_STACK_ERROR;
  }

  return result;
}

static bool resourceMatchesRTFilter(OCResource *resource, char *resourceTypeFilter)
{
  if (!resource)
  {
    return false;
  }

  // Null or empty is analogous to no filter.
  if (resourceTypeFilter == NULL || *resourceTypeFilter == 0)
  {
    return true;
  }

  OCResourceType *resourceTypePtr = resource->rsrcType;

  while (resourceTypePtr)
  {
    if (strcmp(resourceTypePtr->resourcetypename, resourceTypeFilter) == 0)
    {
      return true;
    }
    resourceTypePtr = resourceTypePtr->next;
  }

  OIC_LOG_V(INFO, TAG, "%s does not contain rt=%s.", resource->uri, resourceTypeFilter);
  return false;
}

static bool resourceMatchesIFFilter(OCResource *resource, char *interfaceFilter)
{
  if (!resource)
  {
    return false;
  }

  // Null or empty is analogous to no filter.
  if (interfaceFilter == NULL || *interfaceFilter == 0)
  {
    return true;
  }

  OCResourceInterface *interfacePtr = resource->rsrcInterface;

  while (interfacePtr)
  {
    if ((strcmp(interfacePtr->name, interfaceFilter) == 0) &&
        (strcmp(OC_RSRVD_INTERFACE_LL, interfaceFilter) == 0 ||
         strcmp(OC_RSRVD_INTERFACE_DEFAULT, interfaceFilter) == 0))
    {
      return true;
    }
    interfacePtr = interfacePtr->next;
  }

  OIC_LOG_V(INFO, TAG, "%s does not contain if=%s.", resource->uri, interfaceFilter);
  return false;
}

/*
 * If the filters are null, they will be assumed to NOT be present
 * and the resource will not be matched against them.
 * Function will return true if all non null AND non empty filters passed in find a match.
 */
static bool includeThisResourceInResponse(OCResource *resource,
                                          char *interfaceFilter,
                                          char *resourceTypeFilter)
{
  if (!resource)
  {
    OIC_LOG(ERROR, TAG, "Invalid resource");
    return false;
  }

  if (resource->resourceProperties & OC_EXPLICIT_DISCOVERABLE)
  {
    /*
     * At least one valid filter should be available to
     * include the resource in discovery response
     */
    if (!(resourceTypeFilter && *resourceTypeFilter))
    {
      OIC_LOG_V(INFO, TAG, "%s no query string for EXPLICIT_DISCOVERABLE \
                resource", resource->uri);
      return false;
    }
  }
  else if (!(resource->resourceProperties & OC_ACTIVE) ||
           !(resource->resourceProperties & OC_DISCOVERABLE))
  {
    OIC_LOG_V(INFO, TAG, "%s not ACTIVE or DISCOVERABLE", resource->uri);
    return false;
  }

  return resourceMatchesIFFilter(resource, interfaceFilter) &&
         resourceMatchesRTFilter(resource, resourceTypeFilter);

}

OCStackResult SendNonPersistantDiscoveryResponse(OCServerRequest *request, OCResource *resource,
                                                 OCPayload *discoveryPayload, OCEntityHandlerResult ehResult)
{
  OCEntityHandlerResponse response = {0};

  response.ehResult = ehResult;
  response.payload = discoveryPayload;
  response.persistentBufferFlag = 0;
  response.requestHandle = (OCRequestHandle) request;
  response.resourceHandle = (OCResourceHandle) resource;

  return OCDoResponse(&response);
}

#ifdef WITH_RD
static OCStackResult checkResourceExistsAtRD(const char *interfaceType, const char *resourceType,
                                             OCResource **payload, OCDevAddr *devAddr)
{
  OCResourceCollectionPayload *repPayload;
  if (!payload)
  {
    return OC_STACK_ERROR;
  }
  if (OCRDCheckPublishedResource(interfaceType, resourceType, &repPayload, devAddr) == OC_STACK_OK)
  {
    if (!repPayload)
    {
      return OC_STACK_ERROR;
    }
    OCResource *ptr = ((OCResource *) OICCalloc(1, sizeof(OCResource)));
    if (!ptr)
    {
      return OC_STACK_NO_MEMORY;
    }

    ptr->uri = OICStrdup(repPayload->setLinks->href);
    if (!ptr->uri)
    {
      return OC_STACK_NO_MEMORY;
    }
    OCStringLL *rt = repPayload->setLinks->rt;
    while (rt)
    {
      OCResourceType *temp = (OCResourceType *) OICCalloc(1, sizeof(OCResourceType));
      if (!temp)
      {
        OICFree(ptr->uri);
        return OC_STACK_NO_MEMORY;
      }
      temp->next = NULL;
      temp->resourcetypename = OICStrdup(rt->value);
      if (!ptr->rsrcType)
      {
        ptr->rsrcType = temp;
      }
      else
      {
        OCResourceType *type = ptr->rsrcType;
        while (type->next)
        {
          type = type->next;
        }
        type->next = temp;
      }
      rt = rt->next;
    }

    OCStringLL *itf = repPayload->setLinks->itf;
    while (itf)
    {
      OCResourceInterface *temp = (OCResourceInterface *) OICCalloc(1, sizeof(OCResourceInterface));
      if (!temp)
      {
        OICFree(ptr->uri);

        return OC_STACK_NO_MEMORY;
      }
      temp->next = NULL;
      temp->name = OICStrdup(itf->value);
      if (!ptr->rsrcInterface)
      {
        ptr->rsrcInterface = temp;
      }
      else
      {
        OCResourceInterface *type = ptr->rsrcInterface;
        while (type->next)
        {
          type = type->next;
        }
        type->next = temp;
      }
      itf = itf->next;
    }

    ptr->resourceProperties = (OCResourceProperty) repPayload->tags->bitmap;

    OCFreeCollectionResource(repPayload);
    *payload = ptr;
    return OC_STACK_OK;
  }
  else
  {
    OIC_LOG_V(ERROR, TAG, "The resource type or interface type doe not exist \
                             on the resource directory");
  }
  return OC_STACK_ERROR;
}
#endif

static OCStackResult HandleVirtualResource(OCServerRequest *request, OCResource *resource)
{
  if (!request || !resource)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OCStackResult discoveryResult = OC_STACK_ERROR;

  bool bMulticast    = false;     // Was the discovery request a multicast request?
  OCPayload *payload = NULL;

  OIC_LOG(INFO, TAG, "Entering HandleVirtualResource");

  OCVirtualResources virtualUriInRequest = GetTypeOfVirtualURI(request->resourceUrl);

  // Step 1: Generate the response to discovery request
  if (virtualUriInRequest == OC_WELL_KNOWN_URI)
  {
    if (request->method == OC_REST_PUT || request->method == OC_REST_POST || request->method == OC_REST_DELETE)
    {
      OIC_LOG_V(ERROR, TAG, "Resource : %s not permitted for method: %d", request->resourceUrl, request->method);
      return OC_STACK_UNAUTHORIZED_REQ;
    }

    char *interfaceQuery = NULL;
    char *resourceTypeQuery = NULL;

    discoveryResult = getQueryParamsForFiltering(virtualUriInRequest, request->query,
                                                 &interfaceQuery, &resourceTypeQuery);
    bool interfaceQueryAllocated = false;
    if (!interfaceQuery && !resourceTypeQuery)
    {
      interfaceQueryAllocated = true;
      interfaceQuery = OICStrdup(OC_RSRVD_INTERFACE_LL);
    }

    if (discoveryResult == OC_STACK_OK)
    {
      payload = (OCPayload *)OCDiscoveryPayloadCreate();

      if (payload)
      {
        OCDiscoveryPayload *discPayload = (OCDiscoveryPayload *)payload;
        discPayload->sid = (char *)OICCalloc(1, UUID_STRING_SIZE);
        VERIFY_NON_NULL(discPayload->sid, ERROR, OC_STACK_NO_MEMORY);
        const char *uid = OCGetServerInstanceIDString();
        if (uid)
        {
          memcpy(discPayload->sid, uid, UUID_STRING_SIZE);
        }

        if (!resourceTypeQuery && interfaceQuery && (0 == strcmp(interfaceQuery, OC_RSRVD_INTERFACE_LL)))
        {
          for (; resource && discoveryResult == OC_STACK_OK; resource = resource->next)
          {
            bool result = false;
            if (resource->resourceProperties & OC_EXPLICIT_DISCOVERABLE)
            {
              if (resourceTypeQuery && resourceMatchesRTFilter(resource, resourceTypeQuery))
              {
                result = true;
              }
            }
            if (resource->resourceProperties & OC_DISCOVERABLE)
            {
              result = true;
            }

            if (result)
            {
              discoveryResult = BuildVirtualResourceResponse(resource,
                                                             discPayload, &request->devAddr, false);
            }
          }
        }
        else
        {
          if ((interfaceQuery && (0 != strcmp(interfaceQuery, OC_RSRVD_INTERFACE_LL))) ||
              !interfaceQuery)
          {
            discPayload->uri = OICStrdup(OC_RSRVD_WELL_KNOWN_URI);
            VERIFY_NON_NULL(discPayload->uri, ERROR, OC_STACK_NO_MEMORY);
            if (savedDeviceInfo.deviceName)
            {
              discPayload->name = OICStrdup(savedDeviceInfo.deviceName);
              VERIFY_NON_NULL(discPayload->name, ERROR, OC_STACK_NO_MEMORY);
            }
            discPayload->type = OICStrdup(OC_RSRVD_RESOURCE_TYPE_RES);
            VERIFY_NON_NULL(discPayload->type, ERROR, OC_STACK_NO_MEMORY);
            OCResourcePayloadAddStringLL(&discPayload->interface, OC_RSRVD_INTERFACE_LL);
            OCResourcePayloadAddStringLL(&discPayload->interface, OC_RSRVD_INTERFACE_DEFAULT);
            VERIFY_NON_NULL(discPayload->interface, ERROR, OC_STACK_NO_MEMORY);
          }
          bool foundResourceAtRD = false;
          for (; resource && discoveryResult == OC_STACK_OK; resource = resource->next)
          {
#ifdef WITH_RD
            if (strcmp(resource->uri, OC_RSRVD_RD_URI) == 0)
            {
              OCResource *resource1 = NULL;
              OCDevAddr devAddr;
              discoveryResult = checkResourceExistsAtRD(interfaceQuery,
                                                        resourceTypeQuery, &resource1, &devAddr);
              if (discoveryResult != OC_STACK_OK)
              {
                break;
              }
              discoveryResult = BuildVirtualResourceResponse(resource1,
                                                             discPayload, &devAddr, true);
              if (payload)
              {
                discPayload->baseURI = OICStrdup(devAddr.addr);
              }
              OICFree(resource1->uri);
              for (OCResourceType *rsrcRt = resource1->rsrcType, *rsrcRtNext = NULL; rsrcRt;)
              {
                rsrcRtNext = rsrcRt->next;
                OICFree(rsrcRt->resourcetypename);
                OICFree(rsrcRt);
                rsrcRt = rsrcRtNext;
              }

              for (OCResourceInterface *rsrcPtr = resource1->rsrcInterface, *rsrcNext = NULL; rsrcPtr;)
              {
                rsrcNext = rsrcPtr->next;
                OICFree(rsrcPtr->name);
                OICFree(rsrcPtr);
                rsrcPtr = rsrcNext;
              }
              foundResourceAtRD = true;
            }
#endif
            if (!foundResourceAtRD && includeThisResourceInResponse(resource, interfaceQuery, resourceTypeQuery))
            {
              discoveryResult = BuildVirtualResourceResponse(resource,
                                                             discPayload, &request->devAddr, false);
            }
          }
          // Set discoveryResult appropriately if no 'valid' resources are available
          if (discPayload->resources == NULL && !foundResourceAtRD)
          {
            discoveryResult = OC_STACK_NO_RESOURCE;
          }
        }
      }
      else
      {
        discoveryResult = OC_STACK_NO_MEMORY;
      }
    }
    else
    {
      OIC_LOG_V(ERROR, TAG, "Error (%d) parsing query.", discoveryResult);
    }
    if (interfaceQueryAllocated)
    {
      OICFree(interfaceQuery);
    }
  }
  else if (virtualUriInRequest == OC_DEVICE_URI)
  {
    if (request->method == OC_REST_PUT || request->method == OC_REST_POST || request->method == OC_REST_DELETE)
    {
      OIC_LOG_V(ERROR, TAG, "Resource : %s not permitted for method: %d", request->resourceUrl, request->method);
      return OC_STACK_UNAUTHORIZED_REQ;
    }

    const char *deviceId = OCGetServerInstanceIDString();
    if (!deviceId)
    {
      discoveryResult = OC_STACK_ERROR;
    }
    else
    {
      payload = (OCPayload *) OCDevicePayloadCreate(deviceId, savedDeviceInfo.deviceName,
                                                    savedDeviceInfo.types, OC_SPEC_VERSION, OC_DATA_MODEL_VERSION);
      if (!payload)
      {
        discoveryResult = OC_STACK_NO_MEMORY;
      }
      else
      {
        discoveryResult = OC_STACK_OK;
      }
    }
  }
  else if (virtualUriInRequest == OC_PLATFORM_URI)
  {
    if (request->method == OC_REST_PUT || request->method == OC_REST_POST || request->method == OC_REST_DELETE)
    {
      OIC_LOG_V(ERROR, TAG, "Resource : %s not permitted for method: %d", request->resourceUrl, request->method);
      return OC_STACK_UNAUTHORIZED_REQ;
    }

    payload = (OCPayload *)OCPlatformPayloadCreate(&savedPlatformInfo);
    if (!payload)
    {
      discoveryResult = OC_STACK_NO_MEMORY;
    }
    else
    {
      discoveryResult = OC_STACK_OK;
    }
  }
#ifdef ROUTING_GATEWAY
  else if (OC_GATEWAY_URI == virtualUriInRequest)
  {
    // Received request for a gateway
    OIC_LOG(INFO, TAG, "Request is for Gateway Virtual Request");
    discoveryResult = RMHandleGatewayRequest(request, resource);

  }
#endif

  /**
   * Step 2: Send the discovery response
   *
   * Iotivity should respond to discovery requests in below manner:
   * 1)If query filter matching fails and discovery request is multicast,
   *   it should NOT send any response.
   * 2)If query filter matching fails and discovery request is unicast,
   *   it should send an error(RESOURCE_NOT_FOUND - 404) response.
   * 3)If Server does not have any 'DISCOVERABLE' resources and discovery
   *   request is multicast, it should NOT send any response.
   * 4)If Server does not have any 'DISCOVERABLE' resources and discovery
   *   request is unicast, it should send an error(RESOURCE_NOT_FOUND - 404) response.
   */

#ifdef WITH_PRESENCE
  if ((virtualUriInRequest == OC_PRESENCE) &&
      (resource->resourceProperties & OC_ACTIVE))
  {
    // Presence uses observer notification api to respond via SendPresenceNotification.
    SendPresenceNotification(resource->rsrcType, OC_PRESENCE_TRIGGER_CHANGE);
  }
  else
#endif
#ifdef ROUTING_GATEWAY
    // Gateway uses the RMHandleGatewayRequest to respond to the request.
    if (OC_GATEWAY_URI != virtualUriInRequest)
#endif
    {
      if (discoveryResult == OC_STACK_OK)
      {
        SendNonPersistantDiscoveryResponse(request, resource, payload, OC_EH_OK);
      }
      else if (bMulticast == false && (request->devAddr.adapter != OC_ADAPTER_RFCOMM_BTEDR) &&
               (request->devAddr.adapter != OC_ADAPTER_GATT_BTLE))
      {
        OIC_LOG_V(ERROR, TAG, "Sending a (%d) error to (%d)  \
                discovery request", discoveryResult, virtualUriInRequest);
        SendNonPersistantDiscoveryResponse(request, resource, NULL,
                                           (discoveryResult == OC_STACK_NO_RESOURCE) ? OC_EH_RESOURCE_NOT_FOUND : OC_EH_ERROR);
      }
      else
      {
        // Ignoring the discovery request as per RFC 7252, Section #8.2
        OIC_LOG(INFO, TAG, "Silently ignoring the request since device does not have \
                any useful data to send");
      }
    }

  OCPayloadDestroy(payload);

  return OC_STACK_OK;
}

static OCStackResult
HandleDefaultDeviceEntityHandler(OCServerRequest *request)
{
  if (!request)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OCStackResult result = OC_STACK_OK;
  OCEntityHandlerResult ehResult = OC_EH_ERROR;
  OCEntityHandlerRequest ehRequest = {0};

  OIC_LOG(INFO, TAG, "Entering HandleResourceWithDefaultDeviceEntityHandler");
  result = FormOCEntityHandlerRequest(&ehRequest,
                                      (OCRequestHandle) request,
                                      request->method,
                                      &request->devAddr,
                                      (OCResourceHandle) NULL, request->query,
                                      PAYLOAD_TYPE_REPRESENTATION,
                                      request->payload,
                                      request->payloadSize,
                                      request->numRcvdVendorSpecificHeaderOptions,
                                      request->rcvdVendorSpecificHeaderOptions,
                                      (OCObserveAction)request->observationOption,
                                      (OCObservationId)0);
  VERIFY_SUCCESS(result, OC_STACK_OK);

  // At this point we know for sure that defaultDeviceHandler exists
  ehResult = defaultDeviceHandler(OC_REQUEST_FLAG, &ehRequest,
                                  (char *) request->resourceUrl, defaultDeviceHandlerCallbackParameter);
  if (ehResult == OC_EH_SLOW)
  {
    OIC_LOG(INFO, TAG, "This is a slow resource");
    request->slowFlag = 1;
  }
  else if (ehResult == OC_EH_ERROR)
  {
    FindAndDeleteServerRequest(request);
  }
  result = EntityHandlerCodeToOCStackCode(ehResult);
exit:
  OCPayloadDestroy(ehRequest.payload);
  return result;
}

static OCStackResult
HandleResourceWithEntityHandler(OCServerRequest *request,
                                OCResource *resource,
                                uint8_t collectionResource)
{
  if (!request || ! resource)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OCStackResult result = OC_STACK_ERROR;
  OCEntityHandlerResult ehResult = OC_EH_ERROR;
  OCEntityHandlerFlag ehFlag = OC_REQUEST_FLAG;
  ResourceObserver *resObs = NULL;

  OCEntityHandlerRequest ehRequest = {0};

  OIC_LOG(INFO, TAG, "Entering HandleResourceWithEntityHandler");
  OCPayloadType type = PAYLOAD_TYPE_REPRESENTATION;
  // check the security resource
  if (request && request->resourceUrl && SRMIsSecurityResourceURI(request->resourceUrl))
  {
    type = PAYLOAD_TYPE_SECURITY;

  }

  if (request && strcmp(request->resourceUrl, OC_RSRVD_RD_URI) == 0)
  {
    type = PAYLOAD_TYPE_RD;
  }

  result = FormOCEntityHandlerRequest(&ehRequest,
                                      (OCRequestHandle)request,
                                      request->method,
                                      &request->devAddr,
                                      (OCResourceHandle)resource,
                                      request->query,
                                      type,
                                      request->payload,
                                      request->payloadSize,
                                      request->numRcvdVendorSpecificHeaderOptions,
                                      request->rcvdVendorSpecificHeaderOptions,
                                      (OCObserveAction)request->observationOption,
                                      0);
  VERIFY_SUCCESS(result, OC_STACK_OK);

  if (ehRequest.obsInfo.action == OC_OBSERVE_NO_OPTION)
  {
    OIC_LOG(INFO, TAG, "No observation requested");
    ehFlag = OC_REQUEST_FLAG;
  }
  else if (ehRequest.obsInfo.action == OC_OBSERVE_REGISTER && !collectionResource)
  {
    OIC_LOG(INFO, TAG, "Observation registration requested");

    ResourceObserver *obs = GetObserverUsingToken(request->requestToken,
                                                  request->tokenLength);

    if (obs)
    {
      OIC_LOG(INFO, TAG, "Observer with this token already present");
      OIC_LOG(INFO, TAG, "Possibly re-transmitted CON OBS request");
      OIC_LOG(INFO, TAG, "Not adding observer. Not responding to client");
      OIC_LOG(INFO, TAG, "The first request for this token is already ACKED.");

      // server requests are usually free'd when the response is sent out
      // for the request in ocserverrequest.c : HandleSingleResponse()
      // Since we are making an early return and not responding, the server request
      // needs to be deleted.
      FindAndDeleteServerRequest(request);
      return OC_STACK_OK;
    }

    result = GenerateObserverId(&ehRequest.obsInfo.obsId);
    VERIFY_SUCCESS(result, OC_STACK_OK);

    result = AddObserver((const char *)(request->resourceUrl),
                         (const char *)(request->query),
                         ehRequest.obsInfo.obsId, request->requestToken, request->tokenLength,
                         resource, request->qos, request->acceptFormat,
                         &request->devAddr);

    if (result == OC_STACK_OK)
    {
      OIC_LOG(INFO, TAG, "Added observer successfully");
      request->observeResult = OC_STACK_OK;
      ehFlag = (OCEntityHandlerFlag)(OC_REQUEST_FLAG | OC_OBSERVE_FLAG);
    }
    else
    {
      result = OC_STACK_OK;

      // The error in observeResult for the request will be used when responding to this
      // request by omitting the observation option/sequence number.
      request->observeResult = OC_STACK_ERROR;
      OIC_LOG(ERROR, TAG, "Observer Addition failed");
      ehFlag = OC_REQUEST_FLAG;
    }

  }
  else if (ehRequest.obsInfo.action == OC_OBSERVE_DEREGISTER &&
           !collectionResource)
  {
    OIC_LOG(INFO, TAG, "Deregistering observation requested");

    resObs = GetObserverUsingToken(request->requestToken, request->tokenLength);

    if (NULL == resObs)
    {
      // Stack does not contain this observation request
      // Either token is incorrect or observation list is corrupted
      result = OC_STACK_ERROR;
      goto exit;
    }
    ehRequest.obsInfo.obsId = resObs->observeId;
    ehFlag = (OCEntityHandlerFlag)(ehFlag | OC_OBSERVE_FLAG);

    result = DeleteObserverUsingToken(request->requestToken, request->tokenLength);

    if (result == OC_STACK_OK)
    {
      OIC_LOG(INFO, TAG, "Removed observer successfully");
      request->observeResult = OC_STACK_OK;
    }
    else
    {
      result = OC_STACK_OK;
      request->observeResult = OC_STACK_ERROR;
      OIC_LOG(ERROR, TAG, "Observer Removal failed");
    }
  }
  else
  {
    result = OC_STACK_ERROR;
    goto exit;
  }

  ehResult = resource->entityHandler(ehFlag, &ehRequest, resource->entityHandlerCallbackParam);
  if (ehResult == OC_EH_SLOW)
  {
    OIC_LOG(INFO, TAG, "This is a slow resource");
    request->slowFlag = 1;
  }
  else if (ehResult == OC_EH_ERROR)
  {
    FindAndDeleteServerRequest(request);
  }
  result = EntityHandlerCodeToOCStackCode(ehResult);
exit:
  OCPayloadDestroy(ehRequest.payload);
  return result;
}

static OCStackResult
HandleCollectionResourceDefaultEntityHandler(OCServerRequest *request,
                                             OCResource *resource)
{
  if (!request || !resource)
  {
    return OC_STACK_INVALID_PARAM;
  }

  OCStackResult result = OC_STACK_ERROR;
  OCEntityHandlerRequest ehRequest = {0};

  result = FormOCEntityHandlerRequest(&ehRequest,
                                      (OCRequestHandle)request,
                                      request->method,
                                      &request->devAddr,
                                      (OCResourceHandle)resource,
                                      request->query,
                                      PAYLOAD_TYPE_REPRESENTATION,
                                      request->payload,
                                      request->payloadSize,
                                      request->numRcvdVendorSpecificHeaderOptions,
                                      request->rcvdVendorSpecificHeaderOptions,
                                      (OCObserveAction)request->observationOption,
                                      (OCObservationId)0);
  if (result == OC_STACK_OK)
  {
    result = DefaultCollectionEntityHandler(OC_REQUEST_FLAG, &ehRequest);
  }

  OCPayloadDestroy(ehRequest.payload);
  return result;
}

OCStackResult
ProcessRequest(ResourceHandling resHandling, OCResource *resource, OCServerRequest *request)
{
  OCStackResult ret = OC_STACK_OK;

  switch (resHandling)
  {
    case OC_RESOURCE_VIRTUAL:
    {
      ret = HandleVirtualResource(request, resource);
      break;
    }
    case OC_RESOURCE_DEFAULT_DEVICE_ENTITYHANDLER:
    {
      ret = HandleDefaultDeviceEntityHandler(request);
      break;
    }
    case OC_RESOURCE_NOT_COLLECTION_DEFAULT_ENTITYHANDLER:
    {
      OIC_LOG(INFO, TAG, "OC_RESOURCE_NOT_COLLECTION_DEFAULT_ENTITYHANDLER");
      return OC_STACK_ERROR;
    }
    case OC_RESOURCE_NOT_COLLECTION_WITH_ENTITYHANDLER:
    {
      ret = HandleResourceWithEntityHandler(request, resource, 0);
      break;
    }
    case OC_RESOURCE_COLLECTION_WITH_ENTITYHANDLER:
    {
      ret = HandleResourceWithEntityHandler(request, resource, 1);
      break;
    }
    case OC_RESOURCE_COLLECTION_DEFAULT_ENTITYHANDLER:
    {
      ret = HandleCollectionResourceDefaultEntityHandler(request, resource);
      break;
    }
    case OC_RESOURCE_NOT_SPECIFIED:
    {
      ret = OC_STACK_NO_RESOURCE;
      break;
    }
    default:
    {
      OIC_LOG(INFO, TAG, "Invalid Resource Determination");
      return OC_STACK_ERROR;
    }
  }
  return ret;
}

void DeletePlatformInfo()
{
  OIC_LOG(INFO, TAG, "Deleting platform info.");

  OICFree(savedPlatformInfo.platformID);
  savedPlatformInfo.platformID = NULL;

  OICFree(savedPlatformInfo.manufacturerName);
  savedPlatformInfo.manufacturerName = NULL;

  OICFree(savedPlatformInfo.manufacturerUrl);
  savedPlatformInfo.manufacturerUrl = NULL;

  OICFree(savedPlatformInfo.modelNumber);
  savedPlatformInfo.modelNumber = NULL;

  OICFree(savedPlatformInfo.dateOfManufacture);
  savedPlatformInfo.dateOfManufacture = NULL;

  OICFree(savedPlatformInfo.platformVersion);
  savedPlatformInfo.platformVersion = NULL;

  OICFree(savedPlatformInfo.operatingSystemVersion);
  savedPlatformInfo.operatingSystemVersion = NULL;

  OICFree(savedPlatformInfo.hardwareVersion);
  savedPlatformInfo.hardwareVersion = NULL;

  OICFree(savedPlatformInfo.firmwareVersion);
  savedPlatformInfo.firmwareVersion = NULL;

  OICFree(savedPlatformInfo.supportUrl);
  savedPlatformInfo.supportUrl = NULL;

  OICFree(savedPlatformInfo.systemTime);
  savedPlatformInfo.systemTime = NULL;
}

static OCStackResult DeepCopyPlatFormInfo(OCPlatformInfo info)
{
  savedPlatformInfo.platformID = OICStrdup(info.platformID);
  savedPlatformInfo.manufacturerName = OICStrdup(info.manufacturerName);
  savedPlatformInfo.manufacturerUrl = OICStrdup(info.manufacturerUrl);
  savedPlatformInfo.modelNumber = OICStrdup(info.modelNumber);
  savedPlatformInfo.dateOfManufacture = OICStrdup(info.dateOfManufacture);
  savedPlatformInfo.platformVersion = OICStrdup(info.platformVersion);
  savedPlatformInfo.operatingSystemVersion = OICStrdup(info.operatingSystemVersion);
  savedPlatformInfo.hardwareVersion = OICStrdup(info.hardwareVersion);
  savedPlatformInfo.firmwareVersion = OICStrdup(info.firmwareVersion);
  savedPlatformInfo.supportUrl = OICStrdup(info.supportUrl);
  savedPlatformInfo.systemTime = OICStrdup(info.systemTime);

  if ((!savedPlatformInfo.platformID && info.platformID) ||
      (!savedPlatformInfo.manufacturerName && info.manufacturerName) ||
      (!savedPlatformInfo.manufacturerUrl && info.manufacturerUrl) ||
      (!savedPlatformInfo.modelNumber && info.modelNumber) ||
      (!savedPlatformInfo.dateOfManufacture && info.dateOfManufacture) ||
      (!savedPlatformInfo.platformVersion && info.platformVersion) ||
      (!savedPlatformInfo.operatingSystemVersion && info.operatingSystemVersion) ||
      (!savedPlatformInfo.hardwareVersion && info.hardwareVersion) ||
      (!savedPlatformInfo.firmwareVersion && info.firmwareVersion) ||
      (!savedPlatformInfo.supportUrl && info.supportUrl) ||
      (!savedPlatformInfo.systemTime && info.systemTime))
  {
    DeletePlatformInfo();
    return OC_STACK_INVALID_PARAM;
  }

  return OC_STACK_OK;

}

OCStackResult SavePlatformInfo(OCPlatformInfo info)
{
  DeletePlatformInfo();

  OCStackResult res = DeepCopyPlatFormInfo(info);

  if (res != OC_STACK_OK)
  {
    OIC_LOG_V(ERROR, TAG, "Failed to save platform info. errno(%d)", res);
  }
  else
  {
    OIC_LOG(INFO, TAG, "Platform info saved.");
  }

  return res;
}

void DeleteDeviceInfo()
{
  OIC_LOG(INFO, TAG, "Deleting device info.");

  OICFree(savedDeviceInfo.deviceName);
  OCFreeOCStringLL(savedDeviceInfo.types);
  savedDeviceInfo.deviceName = NULL;

}

static OCStackResult DeepCopyDeviceInfo(OCDeviceInfo info)
{
  savedDeviceInfo.deviceName = OICStrdup(info.deviceName);

  if (!savedDeviceInfo.deviceName && info.deviceName)
  {
    DeleteDeviceInfo();
    return OC_STACK_NO_MEMORY;
  }

  if (info.types)
  {
    savedDeviceInfo.types = CloneOCStringLL(info.types);
    if (!savedDeviceInfo.types && info.types)
    {
      DeleteDeviceInfo();
      return OC_STACK_NO_MEMORY;
    }
  }
  return OC_STACK_OK;
}

OCStackResult SaveDeviceInfo(OCDeviceInfo info)
{
  OCStackResult res = OC_STACK_OK;

  DeleteDeviceInfo();

  res = DeepCopyDeviceInfo(info);

  VERIFY_SUCCESS(res, OC_STACK_OK);

  if (OCGetServerInstanceIDString() == NULL)
  {
    OIC_LOG(INFO, TAG, "Device ID generation failed");
    res =  OC_STACK_ERROR;
    goto exit;
  }

  OIC_LOG(INFO, TAG, "Device initialized successfully.");
  return OC_STACK_OK;

exit:
  DeleteDeviceInfo();
  return res;
}
