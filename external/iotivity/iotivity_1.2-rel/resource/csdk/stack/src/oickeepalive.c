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
#include "oickeepaliveinternal.h"

#include <stdio.h>
#include <string.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "oic_time.h"
#include "ocrandom.h"
#include "octhread.h"
#include "uarraylist.h"
#include "ocstackinternal.h"
#include "ocpayloadcbor.h"
#include "ocpayload.h"
#include "ocresourcehandler.h"
#include "logger.h"
#include "cautilinterface.h"
#include <coap/utlist.h>

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
 * KeepAlive key to parser Payload Table.
 */
static const char INTERVAL[] = "in";

/**
 * KeepAlive key to get interval values from Payload Table.
 */
static const char INTERVAL_ARRAY[] = "inarray";

/**
 * Pointer to handle of the newly created KeepAlive resource.
 */
static OCResourceHandle g_keepAliveHandle = NULL;

/**
 * KeepAlive table which holds connection interval.
 */
static struct KeepAliveEntry_t *g_keepAliveConnectionTable = NULL;

/**
 * Mutex to synchronize device object list.
 */
static oc_mutex g_mutexObjectList = NULL;

/**
 * KeepAlive table entries.
 */
typedef struct KeepAliveEntry_t
{
    OCMode mode;                    /**< host Mode of Operation. */
    CAEndpoint_t remoteAddr;        /**< destination Address. */
    int64_t interval;               /**< time interval for KeepAlive. in seconds.*/
    int32_t currIndex;              /**< current interval value index. */
    bool sentPingMsg;               /**< if oic client already sent ping message. */
    uint64_t timeStamp;             /**< last sent or received ping message. in microseconds. */
    OCDoHandle handle;              /**< Invocation handle tied to original call to OCDoResource().*/
    struct KeepAliveEntry_t *next;  /**< Linked list; for multiple keepalive info list.*/
} KeepAliveEntry_t;

/**
 * Send disconnect message to remove connection.
 */
static OCStackResult OCSendDisconnectMessage(const KeepAliveEntry_t *entry);

/**
 * This function creates KeepAlive resource.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult OCCreateKeepAliveResource();

/**
 * This function deletes KeepAlive resource.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult OCDeleteKeepAliveResource();

/**
 * API to handle the GET request received for a KeepAlive resource.
 * @param[in]   request     Request Received.
 * @param[in]   resource    Resource handle used for sending the response.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCEntityHandlerResult OCHandleKeepAliveGETRequest(OCServerRequest *request,
                                                         const OCResource *resource);

/**
 * API to handle the PUT request received for a KeepAlive resource.
 * @param[in]   request     Request Received.
 * @param[in]   resource    Resource handle used for sending the response.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCEntityHandlerResult OCHandleKeepAlivePOSTRequest(OCServerRequest *request,
                                                          const OCResource *resource);

/**
 * Gets keepalive entry.
 * @param[in]   endpoint    Remote Endpoint information (like ipaddress,
 *                          port, reference uri and transport type) to
 *                          which the ping message has to be sent.
 * @return  KeepAlive entry to send ping message.
 */
static KeepAliveEntry_t *OCGetEntryFromEndpoint(const CAEndpoint_t *endpoint);

/**
 * Add keepalive entry.
 * @param[in]   endpoint    Remote Endpoint information (like ipaddress,
 *                          port, reference uri and transport type).
 * @param[in]   mode        Whether it is OIC Server or OIC Client.
 * @param[in]   interval    Sent interval value to remote device.
 * @return  The KeepAlive entry added in KeepAlive Table.
 */
KeepAliveEntry_t *OCAddKeepAliveEntry(const CAEndpoint_t *endpoint, OCMode mode,
                                      int64_t interval);

/**
 * Remove keepalive entry.
 * @param[in]   endpoint    Remote Endpoint information (like ipaddress,
 *                          port, reference uri and transport type).
 * @return  The KeepAlive entry removed in KeepAlive Table.
 */
static OCStackResult OCRemoveKeepAliveEntry(const CAEndpoint_t *endpoint);

/**
 * Create KeepAlive paylaod to send message.
 * @param[in]   interval   The interval value to be sent.
 * @return  Created representation payload.
 */
static OCRepPayload *OCCreateKeepAlivePayload(int64_t interval);

/**
 * Send response to remote device.
 * @param[in]   request     Request Received.
 * @param[in]   result      Result to be sent.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult OCSendKeepAliveResponse(OCServerRequest *request,
                                           OCEntityHandlerResult result);

/**
 * Add resource type name to payload for GET request.
 * @param[in]   payload     Pointer to the payload to which byte string needs to be added.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult OCAddResourceTypeNameToPayload(OCRepPayload *payload);

/**
 * Add resource interface name to payload for GET request.
 * @param[in]   payload     Pointer to the payload to which byte string needs to be added.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
static OCStackResult OCAddResourceInterfaceNameToPayload(OCRepPayload *payload);

OCStackResult OCInitializeKeepAlive(OCMode mode)
{
    OIC_LOG(DEBUG, TAG, "OCInitializeKeepAlive IN");

    if (!g_mutexObjectList)
    {
        g_mutexObjectList = oc_mutex_new();
        if (!g_mutexObjectList)
        {
            OIC_LOG(ERROR, TAG, "Failed to create mutex!");
            return OC_STACK_ERROR;
        }
    }

    if (OC_CLIENT != mode)
    {
        // Create the KeepAlive Resource[/oic/ping].
        OCStackResult result = OCCreateKeepAliveResource();
        if (OC_STACK_OK != result)
        {
            OIC_LOG_V(ERROR, TAG, "OCCreateKeepAliveResource failed[%d]", result);
            return result;
        }
    }

    OIC_LOG(DEBUG, TAG, "OCInitializeKeepAlive OUT");
    return OC_STACK_OK;
}

OCStackResult OCTerminateKeepAlive(OCMode mode)
{
    OIC_LOG(DEBUG, TAG, "OCTerminateKeepAlive IN");

    if (g_mutexObjectList)
    {
        oc_mutex_free(g_mutexObjectList);
        g_mutexObjectList = NULL;
    }

    if (OC_CLIENT != mode)
    {
        // Delete the KeepAlive Resource[/oic/ping].
        OCStackResult result = OCDeleteKeepAliveResource();
        if (OC_STACK_OK != result)
        {
            OIC_LOG_V(ERROR, TAG, "OCDeleteKeepAliveResource failed[%d]", result);
            return result;
        }
    }

    OIC_LOG(DEBUG, TAG, "OCTerminateKeepAlive OUT");
    return OC_STACK_OK;
}

OCStackResult OCFindKeepAliveResource(OCDoHandle *handle, const char *remoteAddr,
                                      OCCallbackData *cbData)
{
    // Validate input parameters
    VERIFY_NON_NULL(remoteAddr, FATAL, OC_STACK_INVALID_CALLBACK);
    VERIFY_NON_NULL(cbData, FATAL, OC_STACK_INVALID_CALLBACK);
    VERIFY_NON_NULL(cbData->cb, FATAL, OC_STACK_INVALID_CALLBACK);

    // Send discover message to find ping resource
    char requestUri[MAX_QUERY_LENGTH] = { 0 };
    snprintf(requestUri, MAX_QUERY_LENGTH, "%s%s", remoteAddr, KEEPALIVE_RESOURCE_URI);
    OCStackResult result = OCDoResource(handle, OC_REST_DISCOVER, requestUri,
                                        NULL, NULL, CT_ADAPTER_TCP, OC_HIGH_QOS,
                                        cbData, NULL, 0);
    if (OC_STACK_OK != result)
    {
        OIC_LOG(ERROR, TAG, "OCDoResource has failed");
    }
    return result;
}

OCStackResult OCSendKeepAliveRequest(OCDoHandle *handle, const char *remoteAddr,
                                     OCPayload *payload, OCCallbackData *cbData)
{
    VERIFY_NON_NULL(remoteAddr, FATAL, OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL(cbData, FATAL, OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL(cbData->cb, FATAL, OC_STACK_INVALID_CALLBACK);
    VERIFY_NON_NULL(payload, FATAL, OC_STACK_INVALID_CALLBACK);

    OIC_LOG(DEBUG, TAG, "SendKeepAliveRequest IN");

    // Parse the remote device address to send ping message.
    OCDevAddr *devAddr = NULL;
    char requestUri[MAX_QUERY_LENGTH] = { 0 };
    snprintf(requestUri, MAX_QUERY_LENGTH, "%s%s", remoteAddr, KEEPALIVE_RESOURCE_URI);
    OCStackResult result = ParseRequestUri(requestUri, OC_DEFAULT_ADAPTER, OC_DEFAULT_FLAGS,
                                           &devAddr, NULL, NULL);
    if (result != OC_STACK_OK)
    {
        OIC_LOG_V(DEBUG, TAG, "Unable to parse uri: %s", remoteAddr);
        return OC_STACK_ERROR;
    }

    VERIFY_NON_NULL(devAddr, FATAL, OC_STACK_INVALID_PARAM);

    if (!(devAddr->adapter & OC_ADAPTER_TCP))
    {
        OIC_LOG_V(DEBUG, TAG, "Not supported connectivity type");
        OICFree(devAddr);
        return OC_STACK_ERROR;
    }

    // Get entry from KeepAlive table.
    CAEndpoint_t endpoint = { .adapter = CA_DEFAULT_ADAPTER };
    CopyDevAddrToEndpoint(devAddr, &endpoint);

    oc_mutex_lock(g_mutexObjectList);
    KeepAliveEntry_t *entry = OCGetEntryFromEndpoint(&endpoint);
    if (!entry)
    {
        OIC_LOG(DEBUG, TAG, "There is no connection info in KeepAlive table");

        entry = OCAddKeepAliveEntry(&endpoint, OC_CLIENT, 0);
        if (!entry)
        {
            oc_mutex_unlock(g_mutexObjectList);
            OIC_LOG(ERROR, TAG, "Failed to add new KeepAlive entry");
            OICFree(devAddr);
            return OC_STACK_ERROR;
        }
    }

    // Get "in" value from payload.
    int64_t interval = 0;
    bool findValue = OCRepPayloadGetPropInt((OCRepPayload *) payload, INTERVAL, &interval);
    if (findValue && interval)
    {
        if (entry->sentPingMsg)
        {
            oc_mutex_unlock(g_mutexObjectList);
            OIC_LOG(ERROR, TAG, "Already sent a ping request to remote device");
            OICFree(devAddr);
            return OC_STACK_ERROR;
        }
        entry->interval = interval;
        //OIC_LOG_V(DEBUG, TAG, "Send ping message with interval [%" PRId64 "]", entry->interval);
        OIC_LOG_V_INFO(INFO, TAG, "\e[3;30;43m Send ping message with  interval [%" PRId64 "] \e[m", entry->interval);
    }

    // Get "inarray" value from payload.
    int64_t *inArray = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
    OCRepPayloadGetIntArray((OCRepPayload *) payload, INTERVAL_ARRAY, &inArray, dimensions);
    if (inArray)
    {
        uint8_t len = 0;
        char newArray[MAX_URI_LENGTH] = { 0 };
        size_t inArraySize = calcDimTotal(dimensions);
        for (size_t i = 0; i < inArraySize; i++)
        {
            len += snprintf(newArray + len, MAX_URI_LENGTH, "%ld ", inArray[i]);
        }
        OICFree(inArray);
        OIC_LOG_V(DEBUG, TAG, "Send update interval message with inarray [ %s]", newArray);
    }

	/*Removed by DA*/
    //if(inArray != NULL) OICFree(inArray);

    // Send keepalive message.
    result = OCDoResource(&entry->handle, OC_REST_POST, KEEPALIVE_RESOURCE_URI,
                          devAddr, (OCPayload *) payload, CT_ADAPTER_TCP,
                          OC_HIGH_QOS, cbData, NULL, 0);
    if (OC_STACK_OK != result)
    {
        oc_mutex_unlock(g_mutexObjectList);
        OIC_LOG(ERROR, TAG, "OCDoResource has failed");
        OICFree(devAddr);
        return result;
    }

    // Update timeStamp with time sent ping message for next ping message.
    entry->timeStamp = OICGetCurrentTime(TIME_IN_US);
    entry->sentPingMsg = true;
    if (handle)
    {
        *handle = entry->handle;
    }

    oc_mutex_unlock(g_mutexObjectList);
    OIC_LOG(DEBUG, TAG, "SendKeepAliveRequest OUT");
    OICFree(devAddr);
    return result;
}

OCStackResult OCHandleKeepAliveResponse(const CAEndpoint_t *endPoint, const OCPayload *payload)
{
    (void) payload;
    VERIFY_NON_NULL(endPoint, FATAL, OC_STACK_INVALID_PARAM);

    // Receive response message about post /oic/ping request.
    OIC_LOG(DEBUG, TAG, "OCHandleKeepAliveResponse IN");

    // Get entry from KeepAlive table.
    oc_mutex_lock(g_mutexObjectList);
    KeepAliveEntry_t *entry = OCGetEntryFromEndpoint(endPoint);
    if (entry)
    {
        //OIC_LOG_V(DEBUG, TAG, "Received response about interval [%" PRId64 "]", entry->interval);
        OIC_LOG_V_INFO(INFO, TAG, "\e[3;30;43m Received response about interval [%" PRId64 "] \e[m", entry->interval);

        // Set sentPingMsg values with false.
        entry->sentPingMsg = false;
        entry->handle = NULL;
    }

    oc_mutex_unlock(g_mutexObjectList);
    OIC_LOG(DEBUG, TAG, "OCHandleKeepAliveResponse OUT");
    return OC_STACK_OK;
}

OCStackResult OCCreateKeepAliveResource()
{
    OIC_LOG(DEBUG, TAG, "OCCreateKeepAliveResource IN");

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

    OIC_LOG(DEBUG, TAG, "OCCreateKeepAliveResource OUT");
    return result;
}

OCStackResult OCDeleteKeepAliveResource()
{
    OIC_LOG(DEBUG, TAG, "OCDeleteKeepAliveResource IN");

    // Create a KeepAlive resource
    OCStackResult result = OCDeleteResource(g_keepAliveHandle);

    if (OC_STACK_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "Delete resource for KeepAlive failed[%d]", result);
    }

    OIC_LOG(DEBUG, TAG, "OCDeleteKeepAliveResource OUT");
    return result;
}

OCStackResult OCHandleKeepAliveRequest(OCServerRequest *request,
                                       const OCResource *resource)
{
    VERIFY_NON_NULL(request, FATAL, OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL(resource, FATAL, OC_STACK_INVALID_PARAM);

    OIC_LOG(DEBUG, TAG, "OCHandleKeepAliveRequest IN");

    OCEntityHandlerResult result = OC_EH_ERROR;
    if (OC_REST_GET == request->method)
    {
        switch ((OCObserveAction)request->observationOption)
        {
            case OC_OBSERVE_NO_OPTION:
            case OC_OBSERVE_REGISTER:
            case OC_OBSERVE_DEREGISTER:
                OIC_LOG(DEBUG, TAG, "Received GET request");
                result = OCHandleKeepAliveGETRequest(request, resource);
                break;
            default:
                OIC_LOG(DEBUG, TAG, "Not Supported by KeepAlive");
                result = OC_EH_UNAUTHORIZED_REQ;
        }
    }
    else if (OC_REST_PUT == request->method || OC_REST_POST == request->method)
    {
        OIC_LOG(DEBUG, TAG, "Received PUT/POST request");
        result = OCHandleKeepAlivePOSTRequest(request, resource);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Not Supported by KeepAlive");
        result = OC_EH_UNAUTHORIZED_REQ;
    }

    OCStackResult ret = OCSendKeepAliveResponse(request, result);
    if (OC_STACK_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "SendKeepAliveResponse failed with result %u", ret);
    }

    OIC_LOG(DEBUG, TAG, "OCHandleKeepAliveRequest OUT");
    return ret;
}

OCStackResult OCSendKeepAliveResponse(OCServerRequest *request,
                                      OCEntityHandlerResult result)
{
    VERIFY_NON_NULL(request, FATAL, OC_STACK_INVALID_PARAM);

    OIC_LOG_V(DEBUG, TAG, "Send KeepAlive response with entity result[%d]", result);

    // Convert OCDevAddr to CAEndpoint_t.
    CAEndpoint_t endpoint = {.adapter = CA_DEFAULT_ADAPTER};
    CopyDevAddrToEndpoint(&request->devAddr, &endpoint);

    oc_mutex_lock(g_mutexObjectList);
    KeepAliveEntry_t *entry = OCGetEntryFromEndpoint(&endpoint);
    int64_t interval = (entry) ? entry->interval : 0;
    oc_mutex_unlock(g_mutexObjectList);

    // Create KeepAlive payload to send response message.
    OCRepPayload *payload = OCCreateKeepAlivePayload(interval);

    // Add resource type/interface name to payload for GET request.
    if (OC_REST_GET == request->method && OC_EH_OK == result)
    {
        OCAddResourceTypeNameToPayload(payload);
        OCAddResourceInterfaceNameToPayload(payload);
    }

    OCEntityHandlerResponse ehResponse = { .ehResult = result,
                                           .payload = (OCPayload*) payload,
                                           .requestHandle = request->requestId,
                                           .resourceHandle = g_keepAliveHandle };
    OICStrcpy(ehResponse.resourceUri, sizeof(ehResponse.resourceUri), KEEPALIVE_RESOURCE_URI);

    // Send response message.
    return OCDoResponse(&ehResponse);
}

OCEntityHandlerResult OCHandleKeepAliveGETRequest(OCServerRequest *request,
                                                  const OCResource *resource)
{
    VERIFY_NON_NULL(request, FATAL, OC_EH_ERROR);
    VERIFY_NON_NULL(resource, FATAL, OC_EH_ERROR);

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

OCEntityHandlerResult OCHandleKeepAlivePOSTRequest(OCServerRequest *request,
                                                   const OCResource *resource)
{
    VERIFY_NON_NULL(request, FATAL, OC_EH_ERROR);
    VERIFY_NON_NULL(resource, FATAL, OC_EH_ERROR);

    int64_t interval = 0;
    OCPayload *ocPayload = NULL;
    OCStackResult result = OCParsePayload(&ocPayload, PAYLOAD_TYPE_REPRESENTATION,
                                          request->payload, request->payloadSize);
    if (OC_STACK_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Representation parse failed");
        return OC_EH_ERROR;
    }
    bool findValue = OCRepPayloadGetPropInt((OCRepPayload *) ocPayload, INTERVAL, &interval);
    if (!findValue)
    {
        OIC_LOG(ERROR, TAG, "Can't find the time interval property");
        OCPayloadDestroy(ocPayload);
        return OC_EH_BAD_REQ;
    }

    // Get entry from KeepAlive table.
    CAEndpoint_t endpoint = { .adapter = OC_ADAPTER_TCP };
    CopyDevAddrToEndpoint(&request->devAddr, &endpoint);

    oc_mutex_lock(g_mutexObjectList);
    KeepAliveEntry_t *entry = OCGetEntryFromEndpoint(&endpoint);
    if (!entry)
    {
        OIC_LOG(ERROR, TAG, "Received the first keepalive message from client");
        entry = OCAddKeepAliveEntry(&endpoint, OC_SERVER, 0);
        if (!entry)
        {
            oc_mutex_unlock(g_mutexObjectList);
            OIC_LOG(ERROR, TAG, "Failed to add new keepalive entry");
            OCPayloadDestroy(ocPayload);
            return OC_EH_INTERNAL_SERVER_ERROR;
        }
    }

    entry->interval = interval;
    OIC_LOG_V(DEBUG, TAG, "Received interval is [%" PRId64 "]", entry->interval);
    entry->timeStamp = OICGetCurrentTime(TIME_IN_US);
    oc_mutex_unlock(g_mutexObjectList);

    OCPayloadDestroy(ocPayload);
    return OC_EH_OK;
}

void OCProcessKeepAlive()
{
    oc_mutex_lock(g_mutexObjectList);
    KeepAliveEntry_t *entry = NULL;
    KeepAliveEntry_t *tmp = NULL;
    LL_FOREACH_SAFE(g_keepAliveConnectionTable, entry, tmp)
    {
        if (entry)
        {
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
                        OCSendDisconnectMessage(entry);
                    }
                }
            }
            else if (OC_SERVER == entry->mode)
            {
                /*
                 * If an OIC Server does not receive a PUT/POST request to ping resource
                 * within the specified interval time, terminate the connection.
                 * In this case the timeStamp means last time received ping message.
                 */
                if ((entry->interval * KEEPALIVE_RESPONSE_TIMEOUT_SEC * USECS_PER_SEC)
                        <= currentTime - entry->timeStamp)
                {
                    OIC_LOG(DEBUG, TAG, "Server does not receive a PUT/POST request.");
                    OCSendDisconnectMessage(entry);
                }
            }
        }
    }
    oc_mutex_unlock(g_mutexObjectList);
}

OCStackResult OCSendDisconnectMessage(const KeepAliveEntry_t *entry)
{
    VERIFY_NON_NULL(entry, FATAL, OC_STACK_INVALID_PARAM);

    // notify application that a client does not receive a response.
    ClientCB *cbNode = GetClientCB(NULL, 0, entry->handle, NULL);
    if (cbNode)
    {
        OCClientResponse response = { .devAddr = { .adapter = OC_DEFAULT_ADAPTER } };
        CopyEndpointToDevAddr(&entry->remoteAddr, &response.devAddr);
        FixUpClientResponse(&response);
        response.resourceUri = cbNode->requestUri;
        response.result = OC_STACK_TIMEOUT;

        cbNode->callBack(cbNode->context, cbNode->handle, &response);
        FindAndDeleteClientCB(cbNode);
    }

    CAEndpoint_t endpoint = entry->remoteAddr;
    if (OC_STACK_OK != OCRemoveKeepAliveEntry(&endpoint))
    {
        OIC_LOG(ERROR, TAG, "OCRemoveKeepAliveEntry is failed");
        return OC_STACK_ERROR;
    }

    /*
     * Send empty message to disconnect a connection.
     * If CA get the empty message from RI, CA will disconnect a connection.
     */
    CAResult_t result = CAUtilTCPDisconnectSession(endpoint.addr, endpoint.port, endpoint.flags);
    if (OC_STACK_OK != CAResultToOCResult(result))
    {
        OIC_LOG(ERROR, TAG, "Failed to disconnect session");
    }

    return CAResultToOCResult(result);
}

KeepAliveEntry_t *OCGetEntryFromEndpoint(const CAEndpoint_t *endpoint)
{
    KeepAliveEntry_t *entry = NULL;
    LL_FOREACH(g_keepAliveConnectionTable, entry)
    {
        if (entry)
        {
            if (!strncmp(entry->remoteAddr.addr, endpoint->addr, sizeof(entry->remoteAddr.addr))
                    && (entry->remoteAddr.port == endpoint->port))
            {
                OIC_LOG(DEBUG, TAG, "Connection Info found in KeepAlive table");
                return entry;
            }
        }
    }

    return NULL;
}

KeepAliveEntry_t *OCAddKeepAliveEntry(const CAEndpoint_t *endpoint, OCMode mode,
                                      int64_t interval)
{
    if (!endpoint)
    {
        OIC_LOG(ERROR, TAG, "endpoint is NULL");
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
    entry->remoteAddr.ifindex = endpoint->ifindex;
    entry->remoteAddr.port = endpoint->port;
    strncpy(entry->remoteAddr.addr, endpoint->addr, sizeof(entry->remoteAddr.addr));
    entry->interval = interval;

    LL_APPEND(g_keepAliveConnectionTable, entry);
    return entry;
}

OCStackResult OCRemoveKeepAliveEntry(const CAEndpoint_t *endpoint)
{
    OIC_LOG(DEBUG, TAG, "OCRemoveKeepAliveEntry IN");

    VERIFY_NON_NULL(endpoint, FATAL, OC_STACK_INVALID_PARAM);

    KeepAliveEntry_t *entry = NULL;
    KeepAliveEntry_t *tmp = NULL;
    LL_FOREACH_SAFE(g_keepAliveConnectionTable, entry, tmp)
    {
        if (entry)
        {
            if (!strncmp(entry->remoteAddr.addr, endpoint->addr, sizeof(entry->remoteAddr.addr))
                    && (entry->remoteAddr.port == endpoint->port))
            {
                OIC_LOG_V(DEBUG, TAG, "Remove Connection Info from KeepAlive table, "
                          "remote addr=%s port:%d", entry->remoteAddr.addr,
                          entry->remoteAddr.port);

                LL_DELETE(g_keepAliveConnectionTable, entry);
                OICFree(entry);
            }
        }
    }

    OIC_LOG(DEBUG, TAG, "OCRemoveKeepAliveEntry OUT");
    return OC_STACK_OK;
}

void OCHandleKeepAliveConnCB(const CAEndpoint_t *endpoint, bool isConnected, bool isClient)
{
    (void) isClient;
    VERIFY_NON_NULL_NR(endpoint, FATAL);

    if (isConnected)
    {
        OIC_LOG(DEBUG, TAG, "Received the connected device information from CA");
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "Received the disconnected device information from CA");

        // Do nothing because callback will be removed after one min.
        /*
        // notify application that the session is disconnected.
        oc_mutex_lock(g_mutexObjectList);
        KeepAliveEntry_t *entry = OCGetEntryFromEndpoint(endpoint);
        if (entry)
        {
            ClientCB *cbNode = GetClientCB(NULL, 0, entry->handle, NULL);
            if (cbNode)
            {
                OCClientResponse response = { .devAddr = { .adapter = OC_ADAPTER_TCP } };
                CopyEndpointToDevAddr(&entry->remoteAddr, &response.devAddr);
                FixUpClientResponse(&response);
                response.resourceUri = cbNode->requestUri;
                response.result = OC_STACK_COMM_ERROR;

                cbNode->callBack(cbNode->context, cbNode->handle, &response);
                FindAndDeleteClientCB(cbNode);
            }
            OCRemoveKeepAliveEntry(endpoint);
        }
        oc_mutex_unlock(g_mutexObjectList);
        */
    }
}

OCRepPayload *OCCreateKeepAlivePayload(int64_t interval)
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

OCStackResult OCAddResourceTypeNameToPayload(OCRepPayload *payload)
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

OCStackResult OCAddResourceInterfaceNameToPayload(OCRepPayload *payload)
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
