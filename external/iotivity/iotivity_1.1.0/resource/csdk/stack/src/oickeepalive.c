/* ****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "oickeepalive.h"

#include <stdio.h>
#include <string.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "oic_time.h"
#include "ocrandom.h"
#include "uarraylist.h"
#include "ocstackinternal.h"
#include "ocpayloadcbor.h"
#include "ocpayload.h"
#include "ocresourcehandler.h"
#include "logger.h"

/**
 * Logging tag for module name.
 */
#define TAG "OIC_RI_KEEPALIVE"

static const uint64_t USECS_PER_SEC = 1000000;

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#define VERIFY_SUCCESS(op, successCode) { if ((op) != (successCode)) \
            {OIC_LOG_V(FATAL, TAG, "%s failed!!", #op); goto exit;} }

#define VERIFY_NON_NULL(arg, logLevel, retVal) { if (!(arg)) { OIC_LOG((logLevel), \
             TAG, #arg " is NULL"); return (retVal); } }

#define VERIFY_NON_NULL_NR(arg, logLevel) { if (!(arg)) { OIC_LOG((logLevel), \
             TAG, #arg " is NULL"); return; } }

#define VERIFY_NON_NULL_V(arg) { if (!arg) {OIC_LOG_V(FATAL, TAG, "%s is NULL", #arg);\
    goto exit;} }

/**
 * The KeepAlive table entries are removed
 * if it can't receive response message within 60 seconds.
 */
#define KEEPALIVE_RESPONSE_TIMEOUT_SEC 60

/**
 * The Min time interval value. (2 minutes)
 * start from 2 minutes and increases in multiples of 2 up to a maximum of 64minutes.
 */
#define KEEPALIVE_MIN_INTERVAL 2

/**
 * The Max time interval value. (64 minutes)
 */
#define KEEPALIVE_MAX_INTERVAL 64

/**
 * Default counts of interval value.
 */
#define DEFAULT_INTERVAL_COUNT  6

/**
 * KeepAlive key to parser Payload Table.
 */
static const char INTERVAL[] = "in";

/**
 * KeepAlive key to get interval values from Payload Table.
 */
static const char INTERVAL_ARRAY[] = "inarray";

/**
 * To check if KeepAlive is initialized.
 */
static bool g_isKeepAliveInitialized = false;

/**
 * Pointer to handle of the newly created KeepAlive resource.
 */
static OCResourceHandle g_keepAliveHandle = NULL;

/**
 * KeepAlive table which holds connection interval.
 */
static u_arraylist_t *g_keepAliveConnectionTable = NULL;

/**
 * KeepAlive table entries.
 */
typedef struct
{
  OCMode mode;                    /**< host Mode of Operation. */
  CAEndpoint_t remoteAddr;        /**< destination Address. */
  uint32_t interval;              /**< time interval for KeepAlive. in seconds.*/
  int32_t currIndex;              /**< current interval value index. */
  size_t intervalSize;            /**< total interval counts. */
  int64_t *intervalInfo;          /**< interval values for KeepAlive. */
  bool sentPingMsg;               /**< if oic client already sent ping message. */
  uint64_t timeStamp;             /**< last sent or received ping message. in microseconds. */
} KeepAliveEntry_t;

/**
 * Send disconnect message to remove connection.
 */
static OCStackResult SendDisconnectMessage(const KeepAliveEntry_t *entry);

/**
 * Send ping message to remote endpoint.
 */
static OCStackResult SendPingMessage(KeepAliveEntry_t *entry);

/**
 * Increase interval value to send next ping message.
 */
static void IncreaseInterval(KeepAliveEntry_t *entry);

/**
 * Ping Message callback registered with RI for KeepAlive Request.
 */
static OCStackApplicationResult PingRequestCallback(void *ctx, OCDoHandle handle,
    OCClientResponse *clientResponse);

/**
 * This function creates KeepAlive resource.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult CreateKeepAliveResource();

/**
 * This function deletes KeepAlive resource.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult DeleteKeepAliveResource();

/**
 * API to handle the GET request received for a KeepAlive resource.
 * @param[in]   endPoint        RemoteEndpoint which sent the packet.
 * @param[in]   requestInfo     Received coap packet.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult HandleKeepAliveGETRequest(const CAEndpoint_t *endPoint,
                                               const CARequestInfo_t *requestInfo);

/**
 * API to handle the PUT request received for a KeepAlive resource.
 * @param[in]   endPoint        RemoteEndpoint which sent the packet.
 * @param[in]   requestInfo     Received coap packet.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult HandleKeepAlivePUTRequest(const CAEndpoint_t *endPoint,
                                               const CARequestInfo_t *requestInfo);

/**
 * API to handle the Response payload.
 * @param[in]   endpoint        RemoteEndpoint which sent the packet.
 * @param[in]   responseCode    Received reseponse code.
 * @param[in]   respPayload     Response payload.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult HandleKeepAliveResponse(const CAEndpoint_t *endPoint,
                                      OCStackResult responseCode,
                                      const OCRepPayload *respPayload);
/**
 * Gets keepalive entry.
 * @param[in]   endpoint    Remote Endpoint information (like ipaddress,
 *                          port, reference uri and transport type) to
 *                          which the ping message has to be sent.
 * @param[out]  index       index of array list.
 * @return  KeepAlive entry to send ping message.
 */
static KeepAliveEntry_t *GetEntryFromEndpoint(const CAEndpoint_t *endpoint, uint32_t *index);

/**
 * Add keepalive entry.
 * @param[in]   endpoint    Remote Endpoint information (like ipaddress,
 *                          port, reference uri and transport type).
 * @param[in]   mode        Whether it is OIC Server or OIC Client.
 * @param[in]   intervalArray   Received interval values from cloud server.
 * @return  The KeepAlive entry added in KeepAlive Table.
 */
KeepAliveEntry_t *AddKeepAliveEntry(const CAEndpoint_t *endpoint, OCMode mode,
                                    int64_t *intervalArray);

/**
 * Remove keepalive entry.
 * @param[in]   endpoint    Remote Endpoint information (like ipaddress,
 *                          port, reference uri and transport type).
 * @return  The KeepAlive entry removed in KeepAlive Table.
 */
static OCStackResult RemoveKeepAliveEntry(const CAEndpoint_t *endpoint);

OCStackResult InitializeKeepAlive(OCMode mode)
{
  OIC_LOG(DEBUG, TAG, "InitializeKeepAlive IN");
  if (g_isKeepAliveInitialized)
  {
    OIC_LOG(DEBUG, TAG, "KeepAlive already initialized");
    return OC_STACK_OK;
  }

  if (OC_CLIENT != mode)
  {
    // Create the KeepAlive Resource[/oic/ping].
    OCStackResult result = CreateKeepAliveResource();
    if (OC_STACK_OK != result)
    {
      OIC_LOG_V(ERROR, TAG, "CreateKeepAliveResource failed[%d]", result);
      return result;
    }
  }

  if (!g_keepAliveConnectionTable)
  {
    g_keepAliveConnectionTable = u_arraylist_create();
    if (NULL == g_keepAliveConnectionTable)
    {
      OIC_LOG(ERROR, TAG, "Creating KeepAlive Table failed");
      TerminateKeepAlive(mode);
      return OC_STACK_ERROR;
    }
  }

  g_isKeepAliveInitialized = true;

  OIC_LOG(DEBUG, TAG, "InitializeKeepAlive OUT");
  return OC_STACK_OK;
}

OCStackResult TerminateKeepAlive(OCMode mode)
{
  OIC_LOG(DEBUG, TAG, "TerminateKeepAlive IN");
  if (!g_isKeepAliveInitialized)
  {
    OIC_LOG(ERROR, TAG, "KeepAlive not initialized");
    return OC_STACK_ERROR;
  }

  if (OC_CLIENT != mode)
  {
    // Delete the KeepAlive Resource[/oic/ping].
    OCStackResult result = DeleteKeepAliveResource();
    if (OC_STACK_OK != result)
    {
      OIC_LOG_V(ERROR, TAG, "DeleteKeepAliveResource failed[%d]", result);
      return result;
    }
  }

  if (NULL != g_keepAliveConnectionTable)
  {
    u_arraylist_destroy(g_keepAliveConnectionTable);
    g_keepAliveConnectionTable = NULL;
  }

  g_isKeepAliveInitialized = false;

  OIC_LOG(DEBUG, TAG, "TerminateKeepAlive OUT");
  return OC_STACK_OK;
}

OCStackResult CreateKeepAliveResource()
{
  OIC_LOG(DEBUG, TAG, "InitKeepAliveResource IN");

  // Create a KeepAlive resource
  OCStackResult result = OCCreateResource(&g_keepAliveHandle,
                                          KEEPALIVE_RESOURCE_TYPE_NAME,
                                          KEEPALIVE_RESOURCE_INTF_NAME,
                                          KEEPALIVE_RESOURCE_URI,
                                          NULL,
                                          NULL,
                                          OC_DISCOVERABLE);

  if (OC_STACK_OK != result)
  {
    OIC_LOG_V(ERROR, TAG, "Create resource for KeepAlive failed[%d]", result);
  }

  OIC_LOG(DEBUG, TAG, "InitKeepAliveResource OUT");
  return result;
}

OCStackResult DeleteKeepAliveResource()
{
  OIC_LOG(DEBUG, TAG, "DeleteKeepAliveResource IN");

  // Create a KeepAlive resource
  OCStackResult result = OCDeleteResource(g_keepAliveHandle);

  if (OC_STACK_OK != result)
  {
    OIC_LOG_V(ERROR, TAG, "Delete resource for KeepAlive failed[%d]", result);
  }

  OIC_LOG(DEBUG, TAG, "DeleteKeepAliveResource OUT");
  return result;
}

OCStackResult HandleKeepAliveRequest(const CAEndpoint_t *endPoint,
                                     const CARequestInfo_t *requestInfo)
{
  VERIFY_NON_NULL(endPoint, FATAL, OC_STACK_INVALID_PARAM);
  VERIFY_NON_NULL(requestInfo, FATAL, OC_STACK_INVALID_PARAM);

  OIC_LOG(DEBUG, TAG, "HandleKeepAliveRequest IN");

  OCStackResult result = OC_STACK_OK;
  if (CA_PUT == requestInfo->method)
  {
    result = HandleKeepAlivePUTRequest(endPoint, requestInfo);
  }
  else if (CA_GET == requestInfo->method)
  {
    result = HandleKeepAliveGETRequest(endPoint, requestInfo);
  }

  OIC_LOG(DEBUG, TAG, "HandleKeepAliveRequest OUT");
  return result;
}

OCStackResult HandleKeepAliveGETRequest(const CAEndpoint_t *endPoint,
                                        const CARequestInfo_t *requestInfo)
{
  VERIFY_NON_NULL(endPoint, FATAL, OC_STACK_INVALID_PARAM);
  VERIFY_NON_NULL(requestInfo, FATAL, OC_STACK_INVALID_PARAM);

  OIC_LOG_V(DEBUG, TAG, "Find Ping resource [%s]", requestInfo->info.resourceUri);

  CAResponseResult_t result = CA_CONTENT;
  OCResource *resourcePtr = FindResourceByUri(requestInfo->info.resourceUri);
  if (!resourcePtr)
  {
    // Resource URL not specified
    OIC_LOG_V(DEBUG, TAG, "There is no Ping resource [%s]", requestInfo->info.resourceUri);
    result = CA_NOT_FOUND;
  }

  SendDirectStackResponse(endPoint, requestInfo->info.messageId, result, requestInfo->info.type,
                          requestInfo->info.numOptions, requestInfo->info.options,
                          requestInfo->info.token, requestInfo->info.tokenLength,
                          requestInfo->info.resourceUri);

  return OC_STACK_OK;
}

OCStackResult HandleKeepAlivePUTRequest(const CAEndpoint_t *endPoint,
                                        const CARequestInfo_t *requestInfo)
{
  VERIFY_NON_NULL(endPoint, FATAL, OC_STACK_INVALID_PARAM);
  VERIFY_NON_NULL(requestInfo, FATAL, OC_STACK_INVALID_PARAM);

  // Get entry from KeepAlive table.
  uint32_t index = 0;
  KeepAliveEntry_t *entry = GetEntryFromEndpoint(endPoint, &index);
  if (!entry)
  {
    OIC_LOG(ERROR, TAG, "Received the first keepalive message from client");
    entry = AddKeepAliveEntry(endPoint, OC_SERVER, NULL);
    if (!entry)
    {
      OIC_LOG(ERROR, TAG, "Failed to add new keepalive entry");
      return OC_STACK_ERROR;
    }
  }

  OCPayload *ocPayload = NULL;
  OCParsePayload(&ocPayload, PAYLOAD_TYPE_REPRESENTATION,
                 requestInfo->info.payload, requestInfo->info.payloadSize);
  OCRepPayload *repPayload = (OCRepPayload *)ocPayload;

  int64_t interval = 0;
  OCRepPayloadGetPropInt(repPayload, INTERVAL, &interval);
  entry->interval = interval;
  OIC_LOG_V(DEBUG, TAG, "Received interval is [%d]", entry->interval);
  entry->timeStamp = OICGetCurrentTime(TIME_IN_US);

  // Send response message.
  SendDirectStackResponse(endPoint, requestInfo->info.messageId, CA_VALID, requestInfo->info.type,
                          requestInfo->info.numOptions, requestInfo->info.options,
                          requestInfo->info.token, requestInfo->info.tokenLength,
                          requestInfo->info.resourceUri);

  return OC_STACK_OK;
}

OCStackResult HandleKeepAliveResponse(const CAEndpoint_t *endPoint,
                                      OCStackResult responseCode,
                                      const OCRepPayload *respPayload)
{
  VERIFY_NON_NULL(endPoint, FATAL, OC_STACK_INVALID_PARAM);

  OIC_LOG(DEBUG, TAG, "HandleKeepAliveResponse IN");

  // Get entry from KeepAlive table.
  uint32_t index = 0;
  KeepAliveEntry_t *entry = GetEntryFromEndpoint(endPoint, &index);
  if (!entry)
  {
    // Receive response message about find /oic/ping request.
    OIC_LOG(ERROR, TAG, "There is no connection info in KeepAlive table");

    if (OC_STACK_NO_RESOURCE == responseCode)
    {
      OIC_LOG(ERROR, TAG, "Server doesn't have a ping resource");
      return OC_STACK_ERROR;
    }
    else if (OC_STACK_OK == responseCode)
    {
      int64_t *recvInterval = NULL;
      size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
      OCRepPayloadGetIntArray(respPayload, INTERVAL_ARRAY, &recvInterval, dimensions);
      size_t serverIntervalSize = calcDimTotal(dimensions);

      entry = AddKeepAliveEntry(endPoint, OC_CLIENT, recvInterval);
      if (!entry)
      {
        OIC_LOG(ERROR, TAG, "Failed to add new KeepAlive entry");
        return OC_STACK_ERROR;
      }

      if (serverIntervalSize)
      {
        // update interval size with received size of server.
        entry->intervalSize = serverIntervalSize;
      }

      // Send first ping message
      return SendPingMessage(entry);
    }
  }
  else
  {
    // Set sentPingMsg values with false.
    entry->sentPingMsg = false;
  }

  OIC_LOG(DEBUG, TAG, "HandleKeepAliveResponse OUT");
  return OC_STACK_OK;
}

void ProcessKeepAlive()
{
  if (!g_isKeepAliveInitialized)
  {
    OIC_LOG(ERROR, TAG, "KeepAlive not initialized");
    return;
  }

  uint32_t len = u_arraylist_length(g_keepAliveConnectionTable);

  for (uint32_t i = 0; i < len; i++)
  {
    KeepAliveEntry_t *entry = u_arraylist_get(g_keepAliveConnectionTable, i);
    if (NULL == entry)
    {
      continue;
    }

    uint64_t currentTime = OICGetCurrentTime(TIME_IN_US);
    if (OC_CLIENT == entry->mode)
    {
      if (entry->sentPingMsg)
      {
        /*
         * If an OIC Client does not receive the response within 1 minutes,
         * terminate the connection.
         * In this case the timeStamp means last time sent ping message.
         */
        if ((KEEPALIVE_RESPONSE_TIMEOUT_SEC * USECS_PER_SEC) <= currentTime - entry->timeStamp)
        {
          OIC_LOG(DEBUG, TAG, "Client does not receive the response within 1 minutes.");

          // Send message to disconnect session.
          SendDisconnectMessage(entry);
        }
      }
      else
      {
        if ((entry->interval * KEEPALIVE_RESPONSE_TIMEOUT_SEC * USECS_PER_SEC)
            <= currentTime - entry->timeStamp)
        {
          // Increase interval value.
          IncreaseInterval(entry);

          OCStackResult result = SendPingMessage(entry);
          if (OC_STACK_OK != result)
          {
            OIC_LOG(ERROR, TAG, "Failed to send ping request");
            continue;
          }
        }
      }
    }
    else if (OC_SERVER == entry->mode)
    {
      /*
       * If an OIC Server does not receive a PUT request to ping resource
       * within the specified interval time, terminate the connection.
       * In this case the timeStamp means last time received ping message.
       */
      if ((entry->interval * KEEPALIVE_RESPONSE_TIMEOUT_SEC * USECS_PER_SEC)
          <= currentTime - entry->timeStamp)
      {
        OIC_LOG(DEBUG, TAG, "Server does not receive a PUT request.");
        SendDisconnectMessage(entry);
      }
    }
  }
}

void IncreaseInterval(KeepAliveEntry_t *entry)
{
  VERIFY_NON_NULL_NR(entry, FATAL);

  OIC_LOG_V(DEBUG, TAG, "Total interval counts: %d", entry->intervalSize);
  if (entry->intervalSize > entry->currIndex + 1)
  {
    entry->currIndex++;
    entry->interval = entry->intervalInfo[entry->currIndex];
    OIC_LOG_V(DEBUG, TAG, "increase interval value [%d]", entry->interval);
  }
}

OCStackResult SendDisconnectMessage(const KeepAliveEntry_t *entry)
{
  VERIFY_NON_NULL(entry, FATAL, OC_STACK_INVALID_PARAM);

  /*
   * Send empty message to disconnect a connection.
   * If CA get the empty message from RI, CA will disconnect a connection.
   */
  CARequestInfo_t requestInfo = { .method = CA_PUT };
  return CASendRequest(&entry->remoteAddr, &requestInfo);
}

OCStackResult SendPingMessage(KeepAliveEntry_t *entry)
{
  VERIFY_NON_NULL(entry, FATAL, OC_STACK_INVALID_PARAM);

  // Send ping message.
  OCCallbackData pingData = { .cb = PingRequestCallback };
  OCDevAddr devAddr = { .adapter = OC_ADAPTER_TCP };
  CopyEndpointToDevAddr(&(entry->remoteAddr), &devAddr);

  OCRepPayload *payload = OCRepPayloadCreate();
  if (!payload)
  {
    OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
    return OC_STACK_ERROR;
  }
  payload->base.type = PAYLOAD_TYPE_REPRESENTATION;
  OCRepPayloadSetPropInt(payload, INTERVAL, entry->interval);

  OCDoResource(NULL, OC_REST_PUT, KEEPALIVE_RESOURCE_URI, &devAddr,
               (OCPayload *) payload, CT_ADAPTER_TCP, OC_LOW_QOS, &pingData, NULL, 0);

  // Update timeStamp with time sent ping message for next ping message.
  entry->timeStamp = OICGetCurrentTime(TIME_IN_US);
  entry->sentPingMsg = true;

  OIC_LOG_V(DEBUG, TAG, "Client sent ping message, interval [%d]", entry->interval);

  return OC_STACK_OK;
}

OCStackApplicationResult PingRequestCallback(void *ctx, OCDoHandle handle,
                                             OCClientResponse *clientResponse)
{
  OIC_LOG(DEBUG, TAG, "PingRequestCallback IN");
  (void) ctx;
  (void) handle;
  if (NULL == clientResponse)
  {
    OIC_LOG(ERROR, TAG, "clientResponse is NULL");
    return OC_STACK_KEEP_TRANSACTION;
  }

  CAEndpoint_t endpoint = { .adapter = CA_ADAPTER_TCP };
  CopyDevAddrToEndpoint(&(clientResponse->devAddr), &endpoint);

  HandleKeepAliveResponse(&endpoint, clientResponse->result,
                          (OCRepPayload *)clientResponse->payload);

  OIC_LOG(DEBUG, TAG, "PingRequestCallback OUT");
  return OC_STACK_KEEP_TRANSACTION;
}

KeepAliveEntry_t *GetEntryFromEndpoint(const CAEndpoint_t *endpoint, uint32_t *index)
{
  if (!g_keepAliveConnectionTable)
  {
    OIC_LOG(ERROR, TAG, "KeepAlive Table was not Created.");
    return NULL;
  }

  uint32_t len = u_arraylist_length(g_keepAliveConnectionTable);

  for (uint32_t i = 0; i < len; i++)
  {
    KeepAliveEntry_t *entry = u_arraylist_get(g_keepAliveConnectionTable, i);
    if (NULL == entry)
    {
      continue;
    }

    if (!strncmp(entry->remoteAddr.addr, endpoint->addr, sizeof(entry->remoteAddr.addr))
        && (entry->remoteAddr.port == endpoint->port))
    {
      OIC_LOG(DEBUG, TAG, "Connection Info found in KeepAlive table");
      *index = i;
      return entry;
    }
  }

  return NULL;
}

KeepAliveEntry_t *AddKeepAliveEntry(const CAEndpoint_t *endpoint, OCMode mode,
                                    int64_t *intervalInfo)
{
  if (!endpoint)
  {
    OIC_LOG(ERROR, TAG, "endpoint is NULL");
    return NULL;
  }

  if (!g_keepAliveConnectionTable)
  {
    OIC_LOG(ERROR, TAG, "KeepAlive Table was not Created.");
    return NULL;
  }

  KeepAliveEntry_t *entry = (KeepAliveEntry_t *) OICCalloc(1, sizeof(KeepAliveEntry_t));
  if (NULL == entry)
  {
    OIC_LOG(ERROR, TAG, "Failed to Calloc KeepAlive Entry");
    return NULL;
  }

  entry->mode = mode;
  entry->timeStamp = OICGetCurrentTime(TIME_IN_US);
  entry->remoteAddr.adapter = endpoint->adapter;
  entry->remoteAddr.flags = endpoint->flags;
  entry->remoteAddr.interface = endpoint->interface;
  entry->remoteAddr.port = endpoint->port;
  strncpy(entry->remoteAddr.addr, endpoint->addr, sizeof(entry->remoteAddr.addr));

  entry->intervalSize = DEFAULT_INTERVAL_COUNT;
  entry->intervalInfo = intervalInfo;
  if (!entry->intervalInfo)
  {
    entry->intervalInfo = (int64_t *) OICMalloc(entry->intervalSize * sizeof(int64_t));
    for (size_t i = 0; i < entry->intervalSize; i++)
    {
      entry->intervalInfo[i] = KEEPALIVE_MIN_INTERVAL << i;
    }
  }
  entry->interval = entry->intervalInfo[0];

  bool result = u_arraylist_add(g_keepAliveConnectionTable, (void *)entry);
  if (!result)
  {
    OIC_LOG(ERROR, TAG, "Adding node to head failed");
    OICFree(entry->intervalInfo);
    OICFree(entry);
    return NULL;
  }

  return entry;
}

OCStackResult RemoveKeepAliveEntry(const CAEndpoint_t *endpoint)
{
  VERIFY_NON_NULL(endpoint, FATAL, OC_STACK_INVALID_PARAM);

  uint32_t index = 0;
  KeepAliveEntry_t *entry = GetEntryFromEndpoint(endpoint, &index);
  if (!entry)
  {
    OIC_LOG(ERROR, TAG, "There is no entry in keepalive table.");
    return OC_STACK_ERROR;
  }

  KeepAliveEntry_t *removedEntry = u_arraylist_remove(g_keepAliveConnectionTable, index);
  if (NULL == removedEntry)
  {
    OIC_LOG(ERROR, TAG, "Removed Entry is NULL");
    return OC_STACK_ERROR;
  }

  OIC_LOG_V(DEBUG, TAG, "Remove Connection Info from KeepAlive table, "
            "remote addr=%s port:%d", removedEntry->remoteAddr.addr,
            removedEntry->remoteAddr.port);

  OICFree(entry->intervalInfo);
  OICFree(removedEntry);

  return OC_STACK_OK;
}

void HandleKeepAliveConnCB(const CAEndpoint_t *endpoint)
{
  VERIFY_NON_NULL_NR(endpoint, FATAL);

  OIC_LOG(DEBUG, TAG, "Received the connected device information from CA");

  // Send discover message to find ping resource
  OCCallbackData pingData = { .cb = PingRequestCallback };
  OCDevAddr devAddr = { .adapter = OC_ADAPTER_TCP };
  CopyEndpointToDevAddr(endpoint, &devAddr);

  OCDoResource(NULL, OC_REST_DISCOVER, KEEPALIVE_RESOURCE_URI, &devAddr, NULL,
               OC_ADAPTER_TCP, OC_HIGH_QOS, &pingData, NULL, 0);
}

void HandleKeepAliveDisconnCB(const CAEndpoint_t *endpoint)
{
  VERIFY_NON_NULL_NR(endpoint, FATAL);

  OIC_LOG(DEBUG, TAG, "Received the disconnected device information from CA");

  OCStackResult result = RemoveKeepAliveEntry(endpoint);
  if (result != OC_STACK_OK)
  {
    OIC_LOG(ERROR, TAG, "Failed to remove entry");
    return;
  }
}
