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
    int64_t interval;              /**< time interval for KeepAlive. in seconds.*/
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
static OCStackApplicationResult PingRequestCallback(void* ctx, OCDoHandle handle,
                                             OCClientResponse * clientResponse);

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
 * @param[in]   request     Request Received.
 * @param[in]   resource    Resource handle used for sending the response.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCEntityHandlerResult HandleKeepAliveGETRequest(OCServerRequest *request,
                                                       const OCResource *resource);

/**
 * API to handle the POST request received for a KeepAlive resource.
 * @param[in]   request     Request Received.
 * @param[in]   resource    Resource handle used for sending the response.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCEntityHandlerResult HandleKeepAlivePOSTRequest(OCServerRequest *request,
                                                        const OCResource *resource);

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
static KeepAliveEntry_t *GetEntryFromEndpoint(const CAEndpoint_t *endpoint, size_t *index);

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

/**
 * Create KeepAlive paylaod to send message.
 * @param[in]   interval   The interval value to be sent.
 * @return  Created representation payload.
 */
static OCRepPayload *CreateKeepAlivePayload(int64_t interval);

/**
 * Send response to remote device.
 * @param[in]   request     Request Received.
 * @param[in]   result      Result to be sent.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult SendKeepAliveResponse(OCServerRequest *request,
                                           OCEntityHandlerResult result);

/**
 * Add resource type name to payload for GET request.
 * @param[in]   payload     Pointer to the payload to which byte string needs to be added.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult AddResourceTypeNameToPayload(OCRepPayload *payload);

/**
 * Add resource interface name to payload for GET request.
 * @param[in]   payload     Pointer to the payload to which byte string needs to be added.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult AddResourceInterfaceNameToPayload(OCRepPayload *payload);

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
                                            OC_RSRVD_INTERFACE_DEFAULT,
                                            KEEPALIVE_RESOURCE_URI,
                                            NULL,
                                            NULL,
                                            OC_RES_PROP_NONE);
    if (OC_STACK_OK == result)
    {
        result = BindResourceInterfaceToResource((OCResource *) g_keepAliveHandle,
                                                 KEEPALIVE_RESOURCE_INTF_NAME);
    }

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

OCStackResult HandleKeepAliveRequest(OCServerRequest *request,
                                     const OCResource *resource)
{
    VERIFY_NON_NULL(request, FATAL, OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL(resource, FATAL, OC_STACK_INVALID_PARAM);

    OIC_LOG(DEBUG, TAG, "HandleKeepAliveRequest IN");

    OCEntityHandlerResult result = OC_EH_ERROR;
    if (OC_REST_GET == request->method)
    {
        switch ((OCObserveAction)request->observationOption)
        {
            case OC_OBSERVE_NO_OPTION:
            case OC_OBSERVE_REGISTER:
            case OC_OBSERVE_DEREGISTER:
                OIC_LOG(DEBUG, TAG, "Received GET request");
                result = HandleKeepAliveGETRequest(request, resource);
                break;
            default:
                OIC_LOG(DEBUG, TAG, "Not Supported by KeepAlive");
                result = OC_EH_UNAUTHORIZED_REQ;
        }
    }
    else if (OC_REST_PUT == request->method || OC_REST_POST == request->method)
    {
        OIC_LOG(DEBUG, TAG, "Received PUT/POST request");
        result = HandleKeepAlivePOSTRequest(request, resource);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Not Supported by KeepAlive");
        result = OC_EH_UNAUTHORIZED_REQ;
    }

    OCStackResult ret = SendKeepAliveResponse(request, result);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "SendKeepAliveResponse failed with result %u", ret);
    }

    OIC_LOG(DEBUG, TAG, "HandleKeepAliveRequest OUT");
    return ret;
}

OCStackResult SendKeepAliveResponse(OCServerRequest *request,
                                    OCEntityHandlerResult result)
{
    VERIFY_NON_NULL(request, FATAL, OC_STACK_INVALID_PARAM);

    OIC_LOG_V(DEBUG, TAG, "Send KeepAlive response with entity result[%d]", result);

    // Convert OCDevAddr to CAEndpoint_t.
    CAEndpoint_t endpoint = {.adapter = CA_DEFAULT_ADAPTER};
    CopyDevAddrToEndpoint(&request->devAddr, &endpoint);

    size_t index = 0;
    KeepAliveEntry_t *entry = GetEntryFromEndpoint(&endpoint, &index);
    int64_t interval = (entry) ? entry->interval : 0;

    // Create KeepAlive payload to send response message.
    OCRepPayload *payload = CreateKeepAlivePayload(interval);

    // Add resource type/interface name to payload for GET request.
    if (OC_REST_GET == request->method && OC_EH_OK == result)
    {
        AddResourceTypeNameToPayload(payload);
        AddResourceInterfaceNameToPayload(payload);
    }

    OCEntityHandlerResponse ehResponse = { .ehResult = result, 
                                           .payload = (OCPayload*) payload, 
                                           .requestHandle = request,
                                           .resourceHandle = g_keepAliveHandle };
    OICStrcpy(ehResponse.resourceUri, sizeof(ehResponse.resourceUri), KEEPALIVE_RESOURCE_URI);

    // Send response message.
    return OCDoResponse(&ehResponse);
}

OCEntityHandlerResult HandleKeepAliveGETRequest(OCServerRequest *request,
                                                const OCResource *resource)
{
    VERIFY_NON_NULL(request, FATAL, OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL(resource, FATAL, OC_STACK_INVALID_PARAM);

    OIC_LOG_V(DEBUG, TAG, "Find Ping resource [%s]", request->resourceUrl);

    OCResource *resourcePtr = FindResourceByUri(request->resourceUrl);
    if (!resourcePtr)
    {
        // Resource URL not specified
        OIC_LOG_V(DEBUG, TAG, "There is no Ping resource [%s]", request->resourceUrl);
        return OC_EH_RESOURCE_NOT_FOUND;
    }

    return OC_EH_OK;
}

OCEntityHandlerResult HandleKeepAlivePOSTRequest(OCServerRequest *request,
                                                 const OCResource *resource)
{
    VERIFY_NON_NULL(request, FATAL, OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL(resource, FATAL, OC_STACK_INVALID_PARAM);

    // Get entry from KeepAlive table.
    CAEndpoint_t endpoint = { .adapter = CA_DEFAULT_ADAPTER };
    CopyDevAddrToEndpoint(&request->devAddr, &endpoint);

    size_t index = 0;
    KeepAliveEntry_t *entry = GetEntryFromEndpoint(&endpoint, &index);
    if (!entry)
    {
        OIC_LOG(ERROR, TAG, "Received the first keepalive message from client");
        entry = AddKeepAliveEntry(&endpoint, OC_SERVER, NULL);
        if (!entry)
        {
            OIC_LOG(ERROR, TAG, "Failed to add new keepalive entry");
            return OC_EH_INTERNAL_SERVER_ERROR;
        }
    }

    OCPayload *ocPayload = NULL;
    OCParsePayload(&ocPayload, request->payloadFormat, PAYLOAD_TYPE_REPRESENTATION,
                   request->payload, request->payloadSize);
    OCRepPayload *repPayload = (OCRepPayload *)ocPayload;

    int64_t interval = 0;
    OCRepPayloadGetPropInt(repPayload, INTERVAL, &interval);
    entry->interval = interval;
    OIC_LOG_V(DEBUG, TAG, "Received interval is [%" PRId64 "]", entry->interval);
    entry->timeStamp = OICGetCurrentTime(TIME_IN_US);

    OCPayloadDestroy(ocPayload);

    return OC_EH_OK;
}

OCStackResult HandleKeepAliveResponse(const CAEndpoint_t *endPoint,
                                      OCStackResult responseCode,
                                      const OCRepPayload *respPayload)
{
    VERIFY_NON_NULL(endPoint, FATAL, OC_STACK_INVALID_PARAM);

    OIC_LOG(DEBUG, TAG, "HandleKeepAliveResponse IN");

    // Get entry from KeepAlive table.
    size_t index = 0;
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

        // Check the received interval value.
        int64_t interval = 0;
        OCRepPayloadGetPropInt(respPayload, INTERVAL, &interval);
        OIC_LOG_V(DEBUG, TAG, "Received interval is [%" PRId64 "]", entry->interval);
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

    size_t len = u_arraylist_length(g_keepAliveConnectionTable);

    for (size_t i = 0; i < len; i++)
    {
        KeepAliveEntry_t *entry = (KeepAliveEntry_t *)u_arraylist_get(g_keepAliveConnectionTable,
                                                                      i);
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

    OIC_LOG_V(DEBUG, TAG, "Total interval counts: %" PRIuPTR, entry->intervalSize);
    if (entry->intervalSize > (size_t)entry->currIndex + 1)
    {
        entry->currIndex++;
        entry->interval = entry->intervalInfo[entry->currIndex];
        OIC_LOG_V(DEBUG, TAG, "increase interval value [%" PRId64 "]", entry->interval);
    }
}

OCStackResult SendDisconnectMessage(const KeepAliveEntry_t *entry)
{
    VERIFY_NON_NULL(entry, FATAL, OC_STACK_INVALID_PARAM);

    /*
     * Send empty message to disconnect a connection.
     * If CA get the empty message from RI, CA will disconnect a connection.
     */

    OCStackResult result = RemoveKeepAliveEntry(&entry->remoteAddr);
    if (result != OC_STACK_OK)
    {
        return result;
    }

    CARequestInfo_t requestInfo = { .method = CA_POST };
    result = CASendRequest(&entry->remoteAddr, &requestInfo);
    return CAResultToOCResult(result);
}

OCStackResult SendPingMessage(KeepAliveEntry_t *entry)
{
    VERIFY_NON_NULL(entry, FATAL, OC_STACK_INVALID_PARAM);

    // Send ping message.
    OCCallbackData pingData = { .context = NULL, .cb = PingRequestCallback };
    OCDevAddr devAddr = { .adapter = OC_ADAPTER_TCP };
    CopyEndpointToDevAddr(&(entry->remoteAddr), &devAddr);

    OCRepPayload *payload = CreateKeepAlivePayload(entry->interval);
    OCStackResult result = OCDoResource(NULL, OC_REST_POST, KEEPALIVE_RESOURCE_URI, &devAddr,
                                        (OCPayload *) payload, CT_ADAPTER_TCP, OC_LOW_QOS,
                                        &pingData, NULL, 0);
    if (OC_STACK_OK != result)
    {
        OIC_LOG(ERROR, TAG, "OCDoResource has failed");
        return result;
    }

    // Update timeStamp with time sent ping message for next ping message.
    entry->timeStamp = OICGetCurrentTime(TIME_IN_US);
    entry->sentPingMsg = true;

    OIC_LOG_V(DEBUG, TAG, "Client sent ping message, interval [%" PRId64 "]", entry->interval);

    return OC_STACK_OK;
}

OCStackApplicationResult PingRequestCallback(void* ctx, OCDoHandle handle,
                                             OCClientResponse *clientResponse)
{
    (void) ctx;
    (void) handle;
    if (NULL == clientResponse)
    {
        OIC_LOG(ERROR, TAG, "clientResponse is NULL");
        return OC_STACK_DELETE_TRANSACTION;
    }

    CAEndpoint_t endpoint = { .adapter = CA_ADAPTER_TCP };
    CopyDevAddrToEndpoint(&(clientResponse->devAddr), &endpoint);

    HandleKeepAliveResponse(&endpoint, clientResponse->result,
                            (OCRepPayload *)clientResponse->payload);

    return OC_STACK_DELETE_TRANSACTION;
}

KeepAliveEntry_t *GetEntryFromEndpoint(const CAEndpoint_t *endpoint, size_t *index)
{
    if (!g_keepAliveConnectionTable)
    {
        OIC_LOG(ERROR, TAG, "KeepAlive Table was not Created.");
        return NULL;
    }

    size_t len = u_arraylist_length(g_keepAliveConnectionTable);

    for (size_t i = 0; i < len; i++)
    {
        KeepAliveEntry_t *entry = (KeepAliveEntry_t *)u_arraylist_get(g_keepAliveConnectionTable,
                                                                      i);
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
    entry->remoteAddr = *endpoint;
    entry->intervalSize = DEFAULT_INTERVAL_COUNT;
    entry->intervalInfo = intervalInfo;
    if (!entry->intervalInfo)
    {
        entry->intervalInfo = (int64_t*) OICMalloc(entry->intervalSize * sizeof(int64_t));
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

    size_t index = 0;
    KeepAliveEntry_t *entry = GetEntryFromEndpoint(endpoint, &index);
    if (!entry)
    {
        OIC_LOG(ERROR, TAG, "There is no entry in keepalive table.");
        return OC_STACK_ERROR;
    }

    KeepAliveEntry_t *removedEntry = (KeepAliveEntry_t *)
                                        u_arraylist_remove(g_keepAliveConnectionTable, index);
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

void HandleKeepAliveConnCB(const CAEndpoint_t *endpoint, bool isConnected, bool isClient)
{
    VERIFY_NON_NULL_NR(endpoint, FATAL);

    if (isConnected)
    {
        OIC_LOG(DEBUG, TAG, "Received the connected device information from CA");

        if (isClient)
        {
            // Send discover message to find ping resource
            OCCallbackData pingData = {.context = NULL, .cb = PingRequestCallback };
            OCDevAddr devAddr = { .adapter = OC_ADAPTER_TCP };
            CopyEndpointToDevAddr(endpoint, &devAddr);

            OCStackResult result = OCDoResource(NULL, OC_REST_DISCOVER, KEEPALIVE_RESOURCE_URI,
                                                &devAddr, NULL, CT_ADAPTER_TCP, OC_HIGH_QOS,
                                                &pingData, NULL, 0);
            if (OC_STACK_OK != result)
            {
                OIC_LOG(ERROR, TAG, "OCDoResource has failed");
                return;
            }
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Received the disconnected device information from CA");

        OCStackResult result = RemoveKeepAliveEntry(endpoint);
        if(result != OC_STACK_OK)
        {
            return;
        }
    }
}

OCRepPayload *CreateKeepAlivePayload(int64_t interval)
{
    OIC_LOG_V(DEBUG, TAG, "Create KeepAlive Payload, interval is [%" PRId64 "]", interval);

    OCRepPayload *payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
        return NULL;
    }
    payload->base.type = PAYLOAD_TYPE_REPRESENTATION;
    OCRepPayloadSetPropInt(payload, INTERVAL, interval);

    return payload;
}

OCStackResult AddResourceTypeNameToPayload(OCRepPayload *payload)
{
    uint8_t numElement = 0;
    OCStackResult res = OCGetNumberOfResourceTypes(g_keepAliveHandle, &numElement);
    if (OC_STACK_OK == res)
    {
        size_t rtDim[MAX_REP_ARRAY_DEPTH] = {numElement, 0, 0};
        char **rt = (char **)OICMalloc(sizeof(char *) * numElement);
        if (!rt)
        {
            OIC_LOG(ERROR, TAG, "Could not allocate memory for rf");
            return OC_STACK_NO_MEMORY;
        }
        for (uint8_t i = 0; i < numElement; ++i)
        {
            const char *value = OCGetResourceTypeName(g_keepAliveHandle, i);
            OIC_LOG_V(DEBUG, TAG, "value: %s", value);
            rt[i] = OICStrdup(value);
            if (NULL == rt[i])
            {
                OIC_LOG_V(ERROR, TAG, "Creating duplicate string for rt failed!");
                for (uint8_t j = 0; j < i; ++j)
                {
                    OICFree(rt[j]);
                }
                OICFree(rt);
                return OC_STACK_NO_MEMORY;
            }
        }
        OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **) rt, rtDim);
        for (uint8_t i = 0; i < numElement; ++i)
        {
            OICFree(rt[i]);
        }
        OICFree(rt);
    }

    return res;
}

OCStackResult AddResourceInterfaceNameToPayload(OCRepPayload *payload)
{
    uint8_t numElement = 0;
    OCStackResult res = OCGetNumberOfResourceInterfaces(g_keepAliveHandle, &numElement);
    if (OC_STACK_OK == res)
    {
        size_t ifDim[MAX_REP_ARRAY_DEPTH] = {numElement, 0, 0};
        char **itf = (char **)OICMalloc(sizeof(char *) * numElement);
        if (!itf)
        {
            OIC_LOG(ERROR, TAG, "Could not allocate memory for itf");
            return OC_STACK_NO_MEMORY;
        }

        for (uint8_t i = 0; i < numElement; ++i)
        {
            const char *value = OCGetResourceInterfaceName(g_keepAliveHandle, i);
            OIC_LOG_V(DEBUG, TAG, "value: %s", value);
            itf[i] = OICStrdup(value);
            if (NULL == itf[i])
            {
                OIC_LOG_V(ERROR, TAG, "Creating duplicate string for itf failed!");
                for (uint8_t j = 0; j < i; ++j)
                {
                    OICFree(itf[j]);
                }
                OICFree(itf);
                return OC_STACK_NO_MEMORY;
            }
        }
        OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **) itf, ifDim);
        for (uint8_t i = 0; i < numElement; ++i)
        {
            OICFree(itf[i]);
        }
        OICFree(itf);
    }

    return res;
}
