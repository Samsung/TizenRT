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
//



/**
 * @file
 *
 * This file contains the Internal include file used by lower layers of the OC stack
 *
 */

#ifndef OCSTACKINTERNAL_H_
#define OCSTACKINTERNAL_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdbool.h>
#include "ocstack.h"
#include "ocstackconfig.h"
#include "occlientcb.h"
#include <ocrandom.h>

#include "cacommon.h"
#include "cainterface.h"
#include "securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

/** Default device entity Handler.*/
extern OCDeviceEntityHandler defaultDeviceHandler;

/** Default Callback parameter.*/
extern void* defaultDeviceHandlerCallbackParameter;

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

/** The coap scheme */
#define OC_COAP_SCHEME "coap://"

/** the first outgoing sequence number will be 1*/
#define OC_OFFSET_SEQUENCE_NUMBER (0)

/**
 * This structure will be created in occoap and passed up the stack on the server side.
 */
typedef struct
{
    /** Observe option field.*/
    uint32_t observationOption;

    /** The REST method retrieved from received request PDU.*/
    OCMethod method;

    /** the requested payload format. */
    OCPayloadFormat acceptFormat;

    /** resourceUrl will be filled in occoap using the path options in received request PDU.*/
    char resourceUrl[MAX_URI_LENGTH];

    /** resource query send by client.*/
    char query[MAX_QUERY_LENGTH];

    /** reqJSON is retrieved from the payload of the received request PDU.*/
    uint8_t *payload;

    /** qos is indicating if the request is CON or NON.*/
    OCQualityOfService qos;

    /** Number of the received vendor specific header options.*/
    uint8_t numRcvdVendorSpecificHeaderOptions;

    /** Array of received vendor specific header option .*/
    OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];

    /** Remote end-point address **/
    OCDevAddr devAddr;

    /** Token for the observe request.*/
    CAToken_t requestToken;

    /** token length.*/
    uint8_t tokenLength;

    /** The ID of CoAP PDU.*/
    uint16_t coapID;

    /** For delayed Response.*/
    uint8_t delayedResNeeded;

    /** For More packet.*/
    uint8_t reqMorePacket;

    /** The number of requested packet.*/
    uint32_t reqPacketNum;

    /** The size of requested packet.*/
    uint16_t reqPacketSize;

    /** The number of responded packet.*/
    uint32_t resPacketNum;

    /** Responded packet size.*/
    uint16_t resPacketSize;

    /** The total size of requested packet.*/
    size_t reqTotalSize;
} OCServerProtocolRequest;

/**
 * This typedef is to represent our Server Instance identification.
 */
typedef uint8_t ServerID[16];

//-----------------------------------------------------------------------------
// Internal function prototypes
//-----------------------------------------------------------------------------


/**
 * Handler function for sending a response from multiple resources, such as a collection.
 * Aggregates responses from multiple resource until all responses are received then sends the
 * concatenated response
 *
 * TODO: Need to add a timeout in case a (remote?) resource does not respond
 *
 * @param token         Token to search for.
 * @param tokenLength   Length of token.
 * @param status        Feedback status.
 * @return
 *     ::OCStackResult
 */

OCStackResult OCStackFeedBack(CAToken_t token, uint8_t tokenLength, uint8_t status);


/**
 * Handler function to execute stack requests
 *
 * @param protocolRequest      Pointer to the protocol requests from server.
 *
 * @return
 *     ::OCStackResult
 */
OCStackResult HandleStackRequests(OCServerProtocolRequest * protocolRequest);

OCStackResult SendDirectStackResponse(const CAEndpoint_t* endPoint, const uint16_t coapID,
        const CAResponseResult_t responseResult, const CAMessageType_t type,
        const uint8_t numOptions, const CAHeaderOption_t *options,
        CAToken_t token, uint8_t tokenLength, const char *resourceUri,
        CADataType_t dataType);

#ifdef WITH_PRESENCE

/**
 * Notify Presence subscribers that a resource has been modified.
 *
 * @param resourceType    Handle to the resourceType linked list of resource that was modified.
 * @param trigger         The simplified reason this API was invoked.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult SendPresenceNotification(OCResourceType *resourceType,
        OCPresenceTrigger trigger);

/**
 * Send Stop Notification to Presence subscribers.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult SendStopNotification();
#endif // WITH_PRESENCE

/**
 * Function to parse the IPv4 address.
 *
 * @param ipAddrStr       Pointer to a string of IPv4 address.
 * @param ipAddr          pointer to IPv4 adress.
 * @param port            Port number.
 *
 * @return true on success, false upon failure.
 */
bool ParseIPv4Address(char * ipAddrStr, uint8_t * ipAddr, uint16_t * port);

/**
 * Bind a resource interface to a resource.
 *
 * @param resource Target resource.
 * @param resourceInterfaceName Resource interface.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult BindResourceInterfaceToResource(OCResource* resource,
                                            const char *resourceInterfaceName);
/**
 * Bind a resource type to a resource.
 *
 * @param resource Target resource.
 * @param resourceTypeName Name of resource type.
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult BindResourceTypeToResource(OCResource* resource,
                                            const char *resourceTypeName);

/**
 * Convert OCStackResult to CAResponseResult_t.
 *
 * @param ocCode OCStackResult code.
 * @param method OCMethod method the return code replies to.
 * @return ::CA_CONTENT on OK, some other value upon failure.
 */
CAResponseResult_t OCToCAStackResult(OCStackResult ocCode, OCMethod method);

/**
 * Converts a CAResult_t type to a OCStackResult type.
 *
 * @param caResult CAResult_t value to convert.
 * @return OCStackResult that was converted from the input CAResult_t value.
 */
OCStackResult CAResultToOCResult(CAResult_t caResult);

/**
 * Converts a OCStackResult type to a bool type.
 *
 * @param ocResult OCStackResult value to convert.
 * @return true on success, false upon failure.
 */
bool OCResultToSuccess(OCStackResult ocResult);

/**
 * Map OCQualityOfService to CAMessageType.
 *
 * @param qos Input qos.
 *
 * @return CA message type for a given qos.
 */
CAMessageType_t qualityOfServiceToMessageType(OCQualityOfService qos);

#ifdef WITH_PRESENCE
/**
 * Enable/disable a resource property.
 *
 * @param inputProperty             Pointer to resource property.
 * @param resourceProperties        Property to be enabled/disabled.
 * @param enable                    0:disable, 1:enable.
 *
 * @return OCStackResult that was converted from the input CAResult_t value.
 */
//TODO: should the following function be public?
OCStackResult OCChangeResourceProperty(OCResourceProperty * inputProperty,
        OCResourceProperty resourceProperties, uint8_t enable);
#endif

const char *convertTriggerEnumToString(OCPresenceTrigger trigger);

OCPresenceTrigger convertTriggerStringToEnum(const char * triggerStr);

void CopyEndpointToDevAddr(const CAEndpoint_t *in, OCDevAddr *out);

void CopyDevAddrToEndpoint(const OCDevAddr *in, CAEndpoint_t *out);

/**
 * Get the CoAP ticks after the specified number of milli-seconds.
 *
 * @param milliSeconds Milli-seconds.
 * @return CoAP ticks
 */
uint32_t GetTicks(uint32_t milliSeconds);

/**
 * Extract interface and resource type from the query.
 *
 * @param query is the request received from the client
 * @param filterOne will include result if the interface is included in the query.
 * @param filterTwo will include result if the resource type is included in the query.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure
 */
OCStackResult ExtractFiltersFromQuery(const char *query, char **filterOne, char **filterTwo);

#if defined(RD_CLIENT) || defined(RD_SERVER)
/**
 * This function binds an resource unique ins value to the resource. This can be only called
 * when stack is received response from resource-directory.
 *
 * @param requestUri URI of the resource.
 * @param response Response from queries to remote servers.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OCUpdateResourceInsWithResponse(const char *requestUri,
                                              const OCClientResponse *response);
#endif

/**
 * Get the CoAP ticks after the specified number of milli-seconds.
 *
 * @param milliSeconds Milli-seconds.
 * @return CoAP ticks
 */
uint32_t GetTicks(uint32_t milliSeconds);

/**
 * Delete all of the dynamically allocated elements that were created for the resource attributes.
 *
 * @param resourceAttr Specified resource attribute.
 */
void OCDeleteResourceAttributes(OCAttribute *rsrcAttributes);

/**
 *  A request uri consists of the following components in order:
 *                              example
 *  optionally one of
 *      CoAP over UDP prefix    "coap://"
 *      CoAP over TCP prefix    "coap+tcp://"
 *      CoAP over DTLS prefix   "coaps://"
 *      CoAP over TLS prefix    "coaps+tcp://"
 *  optionally one of
 *      IPv6 address            "[1234::5678]"
 *      IPv4 address            "192.168.1.1"
 *  optional port               ":5683"
 *  resource uri                "/oc/core..."
 *
 *  for PRESENCE requests, extract resource type.
 *
 *  @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult ParseRequestUri(const char *fullUri,
                              OCTransportAdapter adapter,
                              OCTransportFlags flags,
                              OCDevAddr **devAddr,
                              char **resourceUri,
                              char **resourceType);

/**
 * Fix up client response data.
 *
 * @param cr Response from queries to remote servers.
 */
void FixUpClientResponse(OCClientResponse *cr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* OCSTACKINTERNAL_H_ */
