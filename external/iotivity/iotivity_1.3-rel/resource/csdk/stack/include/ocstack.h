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
 * This file contains APIs for OIC Stack to be implemented.
 */

#ifndef OCSTACK_H_
#define OCSTACK_H_

#include <stdio.h>
#include <stdint.h>
#include "octypes.h"

#include "platform_features.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** Macro to use Random port.*/
#define USE_RANDOM_PORT (0)

/*
 * Function prototypes
 */

/**
 * This function Initializes the OC Stack.  Must be called prior to starting the stack.
 *
 * @param mode            OCMode Host device is client, server, or client-server.
 * @param serverFlags     OCTransportFlags Default server transport flags.
 * @param clientFlags     OCTransportFlags Default client transport flags.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCInit1(OCMode mode, OCTransportFlags serverFlags, OCTransportFlags clientFlags);

/**
 * This function Initializes the OC Stack.  Must be called prior to starting the stack.
 *
 * @param mode            OCMode Host device is client, server, or client-server.
 * @param serverFlags     OCTransportFlags Default server transport flags.
 * @param clientFlags     OCTransportFlags Default client transport flags.
 * @param transportType   OCTransportAdapter value to initialize.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCInit2(OCMode mode, OCTransportFlags serverFlags, OCTransportFlags clientFlags,
                      OCTransportAdapter transportType);

/**
 * This function Initializes the OC Stack.  Must be called prior to starting the stack.
 *
 * @param ipAddr      IP Address of host device. Deprecated parameter.
 * @param port        Port of host device. Deprecated parameter.
 * @param mode        OCMode Host device is client, server, or client-server.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCInit(const char *ipAddr, uint16_t port, OCMode mode);

#ifdef RA_ADAPTER
/**
 * @brief   Set Remote Access information for XMPP Client.
 * @param   raInfo            [IN] remote access info.
 *
 * @return  ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCSetRAInfo(const OCRAInfo_t *raInfo);
#endif

/**
 * This function Stops the OC stack.  Use for a controlled shutdown.
 *
 * @note: OCStop() performs operations similar to OCStopPresence(), as well as OCDeleteResource() on
 * all resources this server is hosting. OCDeleteResource() performs operations similar to
 * OCNotifyAllObservers() to notify all client observers that the respective resource is being
 * deleted.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCStop();

/**
 * This function starts responding to multicast /oic/res requests.  This can be
 * only called when stack is in OC_STACK_INITIALIZED state but device is not
 * receiving multicast traffic.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCStartMulticastServer();

/**
 * This function stops responding to multicast /oic/res requests.  This is to be
 * used for devices that uses other entity to push resources.
 *
 * Note that other multicast requests, such as those used during ownership
 * transfer, continue to be responded to.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCStopMulticastServer();

/**
 * This function is Called in main loop of OC client or server.
 * Allows low-level processing of stack services.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCProcess();

/**
 * This function discovers or Perform requests on a specified resource
 * (specified by that Resource's respective URI).
 *
 * @deprecated: Use OCDoRequest instead which does not free given payload.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param method            To perform on the resource.
 * @param requestUri        URI of the resource to interact with. (Address prefix is deprecated in
 *                          favor of destination.)
 * @param destination       Complete description of destination.
 * @param payload           Encoded request payload,
                            OCDoResource will free given payload when return OC_STATUS_OK.
 * @param connectivityType  Modifier flags when destination is not given.
 * @param qos               Quality of service. Note that if this API is called on a uri with the
 *                          well-known multicast IP address, the qos will be forced to ::OC_LOW_QOS
 *                          since it is impractical to send other QOS levels on such addresses.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          discovery or resource interaction is received. The discovery could be
 *                          related to filtered/scoped/particular resource. The callback is
 *                          generated for each response received.
 * @param options           The address of an array containing the vendor specific header options
 *                          to be sent with the request.
 * @param numOptions        Number of header options to be included.
 *
 * @note: Presence subscription amendments (i.e. adding additional resource type filters by calling
 * this API again) require the use of the same base URI as the original request to successfully
 * amend the presence filters.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCDoResource(OCDoHandle *handle,
                           OCMethod method,
                           const char *requestUri,
                           const OCDevAddr *destination,
                           OCPayload* payload,
                           OCConnectivityType connectivityType,
                           OCQualityOfService qos,
                           OCCallbackData *cbData,
                           OCHeaderOption *options,
                           uint8_t numOptions);

/**
 * This function discovers or Perform requests on a specified resource
 * (specified by that Resource's respective URI).
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param method            To perform on the resource.
 * @param requestUri        URI of the resource to interact with. (Address prefix is deprecated in
 *                          favor of destination.)
 * @param destination       Complete description of destination.
 * @param payload           Encoded request payload.
                            OCDoRequest does not free given payload.
 * @param connectivityType  Modifier flags when destination is not given.
 * @param qos               Quality of service. Note that if this API is called on a uri with the
 *                          well-known multicast IP address, the qos will be forced to ::OC_LOW_QOS
 *                          since it is impractical to send other QOS levels on such addresses.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          discovery or resource interaction is received. The discovery could be
 *                          related to filtered/scoped/particular resource. The callback is
 *                          generated for each response received.
 * @param options           The address of an array containing the vendor specific header options
 *                          to be sent with the request.
 * @param numOptions        Number of header options to be included.
 *
 * @note: Presence subscription amendments (i.e. adding additional resource type filters by calling
 * this API again) require the use of the same base URI as the original request to successfully
 * amend the presence filters.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCDoRequest(OCDoHandle *handle,
                          OCMethod method,
                          const char *requestUri,
                          const OCDevAddr *destination,
                          OCPayload* payload,
                          OCConnectivityType connectivityType,
                          OCQualityOfService qos,
                          OCCallbackData *cbData,
                          OCHeaderOption *options,
                          uint8_t numOptions);

/**
 * This function cancels a request associated with a specific @ref OCDoResource invocation.
 *
 * @param handle       Used to identify a specific OCDoResource invocation.
 * @param qos          Used to specify Quality of Service(read below).
 * @param options      Used to specify vendor specific header options when sending
 *                     explicit observe cancellation.
 * @param numOptions   Number of header options to be included.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCCancel(OCDoHandle handle,
                       OCQualityOfService qos,
                       OCHeaderOption * options,
                       uint8_t numOptions);

/**
 * Register Persistent storage callback.
 * @param   persistentStorageHandler  Pointers to open, read, write, close & unlink handlers.
 *
 * @return
 *     OC_STACK_OK                    No errors; Success.
 *     OC_STACK_INVALID_PARAM         Invalid parameter.
 */
OCStackResult OC_CALL OCRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler);

#ifdef WITH_PRESENCE
/**
 * When operating in  OCServer or  OCClientServer mode,
 * this API will start sending out presence notifications to clients via multicast.
 * Once this API has been called with a success, clients may query for this server's presence and
 * this server's stack will respond via multicast.
 *
 * Server can call this function when it comes online for the first time, or when it comes back
 * online from offline mode, or when it re enters network.
 *
 * @param ttl         Time To Live in seconds.
 *                    @note: If ttl is '0', then the default stack value will be used (60 Seconds).
 *                    If ttl is greater than ::OC_MAX_PRESENCE_TTL_SECONDS, then the ttl will be
 *                    set to ::OC_MAX_PRESENCE_TTL_SECONDS.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCStartPresence(const uint32_t ttl);

/**
 * When operating in OCServer or OCClientServer mode, this API will stop sending
 * out presence notifications to clients via multicast.
 * Once this API has been called with a success this server's stack will not respond to clients
 * querying for this server's presence.
 *
 * Server can call this function when it is terminating, going offline, or when going
 * away from network.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */

OCStackResult OC_CALL OCStopPresence();
#endif


/**
 * This function sets default device entity handler.
 *
 * @param entityHandler      Entity handler function that is called by ocstack to handle requests
 *                           for any undefined resources or default actions.If NULL is passed it
 *                           removes the device default entity handler.
 * @param callbackParameter  Parameter passed back when entityHandler is called.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCSetDefaultDeviceEntityHandler(OCDeviceEntityHandler entityHandler,
                                              void* callbackParameter);

/**
 * This function sets device information.
 *
 * Upon call to OCInit, the default Device Type (i.e. "rt") has already been set to the default
 * Device Type "oic.wk.d". You do not have to specify "oic.wk.d" in the OCDeviceInfo.types linked
 * list. The default Device Type is mandatory and always specified by this Device as the first
 * Device Type.
 *
 * @deprecated Use OCSetPropertyValue instead.
 *
 * @param deviceInfo   Structure passed by the server application containing the device
 *                     information.
 *
 * @return
 *     ::OC_STACK_OK               no errors.
 *     ::OC_STACK_INVALID_PARAM    invalid parameter.
 *     ::OC_STACK_ERROR            stack process error.
 */
OCStackResult OC_CALL OCSetDeviceInfo(OCDeviceInfo deviceInfo);

/**
 * This function sets platform information.
 *
 * @param platformInfo   Structure passed by the server application containing
 *                       the platform information.
 *
 *
 * @return
 *     ::OC_STACK_OK               no errors.
 *     ::OC_STACK_INVALID_PARAM    invalid parameter.
 *     ::OC_STACK_ERROR            stack process error.
 */
OCStackResult OC_CALL OCSetPlatformInfo(OCPlatformInfo platformInfo);

/**
 * This function creates a resource.
 *
 * @param handle                Pointer to handle to newly created resource. Set by ocstack and
 *                              used to refer to resource.
 * @param resourceTypeName      Name of resource type.  Example: "core.led".
 * @param resourceInterfaceName Name of resource interface.  Example: "core.rw".
 * @param uri                   URI of the resource.  Example:  "/a/led".
 * @param entityHandler         Entity handler function that is called by ocstack to handle
 *                              requests, etc.
 *                              NULL for default entity handler.
 * @param callbackParam     parameter passed back when entityHandler is called.
 * @param resourceProperties    Properties supported by resource.
 *                              Example: ::OC_DISCOVERABLE|::OC_OBSERVABLE.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCCreateResource(OCResourceHandle *handle,
                               const char *resourceTypeName,
                               const char *resourceInterfaceName,
                               const char *uri,
                               OCEntityHandler entityHandler,
                               void* callbackParam,
                               uint8_t resourceProperties);

/**
 * This function creates a resource.
 *
 * @param handle                Pointer to handle to newly created resource. Set by ocstack and
 *                              used to refer to resource.
 * @param resourceTypeName      Name of resource type.  Example: "core.led".
 * @param resourceInterfaceName Name of resource interface.  Example: "core.rw".
 * @param uri                   URI of the resource.  Example:  "/a/led".
 * @param entityHandler         Entity handler function that is called by ocstack to handle
 *                              requests, etc.
 *                              NULL for default entity handler.
 * @param callbackParam         parameter passed back when entityHandler is called.
 * @param resourceProperties    Properties supported by resource.
 *                              Example: ::OC_DISCOVERABLE|::OC_OBSERVABLE.
 * @param resourceTpsTypes      Transport Protocol Suites(TPS) types of resource for expose
                                resource to specific transport adapter (e.g., TCP, UDP)
                                with messaging protocol (e.g., COAP, COAPS).
                                Example: "OC_COAP | OC_COAP_TCP"
 *
 * @note Only supported TPS types on stack will be mapped to resource.
         It means "OC_COAPS" and "OC_COAPS_TCP" flags will be ignored if secure option
         not enabled on stack. Also "COAP_TCP" and "COAPS_TCP" flags will be ignored
         if stack does not support tcp mode.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCCreateResourceWithEp(OCResourceHandle *handle,
                                     const char *resourceTypeName,
                                     const char *resourceInterfaceName,
                                     const char *uri,
                                     OCEntityHandler entityHandler,
                                     void *callbackParam,
                                     uint8_t resourceProperties,
                                     OCTpsSchemeFlags resourceTpsTypes);
/*
 * This function returns flags of supported endpoint TPS on stack.
 *
 * @return Bit combinations of supported OCTpsSchemeFlags.
 */
OCTpsSchemeFlags OC_CALL OCGetSupportedEndpointTpsFlags();

/**
 * This function adds a resource to a collection resource.
 *
 * @param collectionHandle    Handle to the collection resource.
 * @param resourceHandle      Handle to resource to be added to the collection resource.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCBindResource(OCResourceHandle collectionHandle, OCResourceHandle resourceHandle);

/**
 * This function removes a resource from a collection resource.
 *
 * @param collectionHandle   Handle to the collection resource.
 * @param resourceHandle     Handle to resource to be removed from the collection resource.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCUnBindResource(OCResourceHandle collectionHandle, OCResourceHandle resourceHandle);

/**
 * This function binds a resource type to a resource.
 *
 * @param handle            Handle to the resource.
 * @param resourceTypeName  Name of resource type.  Example: "core.led".
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCBindResourceTypeToResource(OCResourceHandle handle,
                                           const char *resourceTypeName);
/**
 * This function binds a resource interface to a resource.
 *
 * @param handle                  Handle to the resource.
 * @param resourceInterfaceName   Name of resource interface.  Example: "core.rw".
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCBindResourceInterfaceToResource(OCResourceHandle handle,
                                                const char *resourceInterfaceName);

/**
 * This function binds an entity handler to the resource.
 *
 * @param handle            Handle to the resource that the contained resource is to be bound.
 * @param entityHandler     Entity handler function that is called by ocstack to handle requests.
 * @param callbackParameter Context parameter that will be passed to entityHandler.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCBindResourceHandler(OCResourceHandle handle,
                                    OCEntityHandler entityHandler,
                                    void *callbackParameter);

/**
 * This function gets the number of resources that have been created in the stack.
 *
 * @param numResources    Pointer to count variable.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCGetNumberOfResources(uint8_t *numResources);

/**
 * This function gets a resource handle by index.
 *
 * @param index   Index of resource, 0 to Count - 1.
 *
 * @return Found  resource handle or NULL if not found.
 */
OCResourceHandle OC_CALL OCGetResourceHandle(uint8_t index);

/**
 * This function deletes resource specified by handle.  Deletes resource and all
 * resource type and resource interface linked lists.
 *
 * @note: OCDeleteResource() performs operations similar to OCNotifyAllObservers() to notify all
 * client observers that "this" resource is being deleted.
 *
 * @param handle          Handle of resource to be deleted.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCDeleteResource(OCResourceHandle handle);

/**
 * Get a string representation the server instance ID.
 * The memory is managed internal to this function, so freeing externally will result
 * in a runtime error.
 * Note: This will NOT seed the RNG, so it must be called after the RNG is seeded.
 * This is done automatically during the OCInit process,
 * so ensure that this call is done after that.
 *
 * @return A string representation  the server instance ID.
 */
const char* OC_CALL OCGetServerInstanceIDString(void);

/**
 * This function gets the URI of the resource specified by handle.
 *
 * @param handle     Handle of resource.
 *
 * @return URI string if resource found or NULL if not found.
 */
const char *OC_CALL OCGetResourceUri(OCResourceHandle handle);

/**
 * This function gets the properties of the resource specified by handle.
 *
 * @param handle                Handle of resource.
 *
 * @return OCResourceProperty   Bitmask or -1 if resource is not found.
 *
 * @note that after a resource is created, the OC_ACTIVE property is set for the resource by the
 * stack.
 */
OCResourceProperty OC_CALL OCGetResourceProperties(OCResourceHandle handle);

/**
 * This function sets the properties of the resource specified by handle.
 *
 * @param handle                Handle of resource.
 * @param resourceProperties    Properties supported by resource.
 *                              Example: ::OC_DISCOVERABLE|::OC_OBSERVABLE.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCSetResourceProperties(OCResourceHandle handle, uint8_t resourceProperties);

/**
 * This function removes the properties of the resource specified by handle.
 *
 * @param handle                Handle of resource.
 * @param resourceProperties    Properties not supported by resource.
 *                              Example: ::OC_DISCOVERABLE|::OC_OBSERVABLE.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCClearResourceProperties(OCResourceHandle handle, uint8_t resourceProperties);

/**
 * This function gets the number of resource types of the resource.
 *
 * @param handle            Handle of resource.
 * @param numResourceTypes  Pointer to count variable.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCGetNumberOfResourceTypes(OCResourceHandle handle, uint8_t *numResourceTypes);

/**
 * This function gets name of resource type of the resource.
 *
 * @param handle       Handle of resource.
 * @param index        Index of resource, 0 to Count - 1.
 *
 * @return Resource type name if resource found or NULL if resource not found.
 */
const char *OC_CALL OCGetResourceTypeName(OCResourceHandle handle, uint8_t index);

/**
 * This function gets the number of resource interfaces of the resource.
 *
 * @param handle                 Handle of resource.
 * @param numResourceInterfaces  Pointer to count variable.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCGetNumberOfResourceInterfaces(OCResourceHandle handle,
        uint8_t *numResourceInterfaces);

/**
 * This function gets name of resource interface of the resource.
 *
 * @param handle      Handle of resource.
 * @param index       Index of resource, 0 to Count - 1.
 *
 * @return Resource interface name if resource found or NULL if resource not found.
 */
const char *OC_CALL OCGetResourceInterfaceName(OCResourceHandle handle, uint8_t index);

/**
 * This function gets resource handle from the collection resource by index.
 *
 * @param collectionHandle   Handle of collection resource.
 * @param index              Index of contained resource, 0 to Count - 1.
 *
 * @return Handle to contained resource if resource found or NULL if resource not found.
 */
OCResourceHandle OC_CALL OCGetResourceHandleFromCollection(OCResourceHandle collectionHandle,
        uint8_t index);

/**
 * This function gets the entity handler for a resource.
 *
 * @param handle            Handle of resource.
 *
 * @return Entity handler if resource found or NULL resource not found.
 */
OCEntityHandler OC_CALL OCGetResourceHandler(OCResourceHandle handle);

/**
 * This function notify all registered observers that the resource representation has
 * changed. If observation includes a query the client is notified only if the query is valid after
 * the resource representation has changed.
 *
 * @param handle   Handle of resource.
 * @param qos      Desired quality of service for the observation notifications.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCNotifyAllObservers(OCResourceHandle handle, OCQualityOfService qos);

/**
 * Notify specific observers with updated value of representation.
 * Before this API is invoked by entity handler it has finished processing
 * queries for the associated observers.
 *
 * @param handle                    Handle of resource.
 * @param obsIdList                 List of observation IDs that need to be notified.
 * @param numberOfIds               Number of observation IDs included in obsIdList.
 * @param payload                   Object representing the notification
 * @param qos                       Desired quality of service of the observation notifications.
 *
 * @note: The memory for obsIdList and payload is managed by the entity invoking the API.
 * The maximum size of the notification is 1015 bytes for non-Arduino platforms. For Arduino
 * the maximum size is 247 bytes.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCNotifyListOfObservers (OCResourceHandle handle,
                                       OCObservationId  *obsIdList,
                                       uint8_t          numberOfIds,
                                       const OCRepPayload *payload,
                                       OCQualityOfService qos);

/**
 * This function sends a response to a request.
 * The response can be a normal, slow, or block (i.e. a response that
 * is too large to be sent in a single PDU and must span multiple transmissions).
 *
 * @param response   Pointer to structure that contains response parameters.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCDoResponse(OCEntityHandlerResponse *response);

/**
 * This function sets uri being used for proxy.
 *
 * @param uri            NULL terminated resource uri for CoAP-HTTP Proxy.
 */
OCStackResult OC_CALL OCSetProxyURI(const char *uri);

#if defined(RD_CLIENT) || defined(RD_SERVER)
/**
 * This function binds an resource unique id to the resource.
 *
 * @param handle            Handle to the resource that the contained resource is to be bound.
 * @param ins               Unique ID for resource.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCBindResourceInsToResource(OCResourceHandle handle, int64_t ins);

/**
 * This function gets the resource unique id for a resource.
 *
 * @param handle            Handle of resource.
 * @param ins               Unique ID for resource.
 *
 * @return Ins if resource found or 0 resource not found.
 */
OCStackResult OC_CALL OCGetResourceIns(OCResourceHandle handle, int64_t *ins);

#ifdef RD_SERVER
/**
 * Sets the filename to be used for database persistent storage.
 * @param   filename            [IN] the filename.
 *
 * @return  ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCRDDatabaseSetStorageFilename(const char *filename);

/**
 * Returns the filename to be used for database persistent storage.
 *
 * @return the filename
 */
const char *OC_CALL OCRDDatabaseGetStorageFilename();

/**
* Search the RD database for queries.
*
* @param interfaceType is the interface type that is queried.
* @param resourceType is the resource type that is queried.
* @param discPayload NULL if no resource found or else OCDiscoveryPayload with the details
* about the resources.
*
* @return ::OC_STACK_OK in case of success or else other value.
*/
OCStackResult OC_CALL OCRDDatabaseDiscoveryPayloadCreate(const char *interfaceType,
                                                 const char *resourceType,
                                                 OCDiscoveryPayload **discPayload);

/**
* Search the RD database for queries.
*
* @param interfaceType is the interface type that is queried.
* @param resourceType is the resource type that is queried.
* @param endpoint is the requesting endpoint to filter created eps value against.
* @param discPayload NULL if no resource found or else OCDiscoveryPayload with the details
* about the resources.
*
* @return ::OC_STACK_OK in case of success or else other value.
*/
OCStackResult OC_CALL OCRDDatabaseDiscoveryPayloadCreateWithEp(const char *interfaceType,
                                                 const char *resourceType,
                                                 OCDevAddr *endpoint,
                                                 OCDiscoveryPayload **discPayload);
#endif // RD_SERVER
#endif // RD_CLIENT || RD_SERVER

/**
* This function gets a resource handle by resource uri.
*
* @param uri   Uri of Resource to get Resource handle.
*
* @return Found  resource handle or NULL if not found.
*/
OCResourceHandle OC_CALL OCGetResourceHandleAtUri(const char *uri);

/**
 *  Add a header option to the given header option array.
 *
 * @param ocHdrOpt            Pointer to existing options.
 * @param numOptions          Number of existing options.
 * @param optionID            COAP option ID.
 * @param optionData          Option data value.
 * @param optionDataLength    Size of Option data value.
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult OC_CALL OCSetHeaderOption(OCHeaderOption* ocHdrOpt,
                                size_t* numOptions,
                                uint16_t optionID,
                                void* optionData,
                                size_t optionDataLength);

/**
 *  Get data value of the option with specified option ID from given header option array.
 *
 * @param ocHdrOpt            Pointer to existing options.
 * @param numOptions          Number of existing options.
 * @param optionID            COAP option ID.
 * @param optionData          Pointer to option data.
 * @param optionDataLength    Size of option data value.
 * @param receivedDatalLength Pointer to the actual length of received data.
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult OC_CALL OCGetHeaderOption(OCHeaderOption* ocHdrOpt,
                                size_t numOptions,
                                uint16_t optionID,
                                void* optionData,
                                size_t optionDataLength,
                                uint16_t* receivedDatalLength);

/**
 * gets the deviceId of the client
 *
 * @param deviceId pointer.
 * @return Returns ::OC_STACK_OK if success.
 */
OCStackResult OC_CALL OCGetDeviceId(OCUUIdentity *deviceId);

/**
 * sets the deviceId of the client
 *
 * @param deviceId pointer.
 * @return Returns ::OC_STACK_OK if success.
 */
OCStackResult OC_CALL OCSetDeviceId(const OCUUIdentity *deviceId);

 /**
 * Gets the bool state of "isOwned" property on the doxm resource.
 *
 * @param isOwned a pointer to be assigned to isOwned property
 * @return Returns ::OC_STACK_OK if success.
 */
OCStackResult OC_CALL OCGetDeviceOwnedState(bool *isOwned);

/**
 * Encode an address string to match RFC 6874.
 *
 * @param outputAddress    a char array to be written with the encoded string.
 * @param outputSize       size of outputAddress buffer.
 * @param inputAddress     a char array of size <= CA_MAX_URI_LENGTH
 *                         containing a valid IPv6 address string.
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult OC_CALL OCEncodeAddressForRFC6874(char* outputAddress,
                                        size_t outputSize,
                                        const char* inputAddress);

/**
 * Decode an address string according to RFC 6874.
 *
 * @param outputAddress    a char array to be written with the decoded string.
 * @param outputSize       size of outputAddress buffer.
 * @param inputAddress     a valid percent-encoded address string.
 * @param end              NULL if the entire entire inputAddress is a null-terminated percent-
 *                         encoded address string.  Otherwise, a pointer to the first byte that
 *                         is not part of the address string (e.g., ']' in a URI).
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult OC_CALL OCDecodeAddressForRFC6874(char* outputAddress,
                                        size_t outputSize,
                                        const char* inputAddress,
                                        const char* end);

/**
 * Set the value of /oic/d and /oic/p properties. This function is a generic function that sets for
 * all OCF defined properties.
 *
 * For PAYLOAD_TYPE_DEVICE and OC_RSRVD_DATA_MODEL_VERSION, value must be a const char* CSV string.
 * For PAYLOAD_TYPE_DEVICE and OC_RSRVD_DEVICE_DESCRIPTION, or OC_RSRVD_DEVICE_MFG_NAME, value must
 * be an OCStringLL* with pairs of language tag and value elements.  For all other
 * PAYLOAD_TYPE_DEVICE properties, value must be a const char* string.
 *
 * @param type the payload type for device and platform as defined in @ref OCPayloadType.
 * @param propName the pre-defined property as per OCF spec.
 * @param value the value of the property to be set.
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult OC_CALL OCSetPropertyValue(OCPayloadType type, const char *propName, const void *value);

/**
 * Get the value of /oic/d and /oic/p properties. This function is a generic function that gets the
 * value for all OCF defined properties.
 *
 * For PAYLOAD_TYPE_DEVICE and OC_RSRVD_RESOURCE_TYPE, OC_RSRVD_INTERFACE,
 * OC_RSRVD_DATA_MODEL_VERSION, OC_RSRVD_DEVICE_DESCRIPTION, or OC_RSRVD_DEVICE_MFG_NAME, the value
 * will be an OCStringLL*. For all other PAYLOAD_TYPE_DEVICE properties, value will be a char*.
 *
 * When the returned value is non-NULL, the memory must be OCFreeOCStringLL()'d or OICFree()'d by
 * the caller (depending on the underlying type returned).
 *
 * @param type the payload type for device and platform as defined in @ref OCPayloadType.
 * @param propName the pre-defined as per OCF spec.
 * @param value this holds the return value.  In case of error will be set to NULL.
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult OC_CALL OCGetPropertyValue(OCPayloadType type, const char *propName, void **value);

/**
* Get the registered persistent storage handler. All modules must use this to obtain access to
* persistent storage.
*
* @return pointer to OCPersistentStorage structure on success and NULL otherwise.
*/
OCPersistentStorage *OC_CALL OCGetPersistentStorageHandler();

/**
* This function return link local zone id related from ifindex.
*
* @param[in] ifindex     interface index.
* @param[out] zoneId     pointer of zoneId string, caller should free
*                        zoneId using OICFree() when it returned CA_STATUS_OK.
* @return ::OC_STACK_OK if successful.
*/
OCStackResult OC_CALL OCGetLinkLocalZoneId(uint32_t ifindex, char **zoneId);

/**
 * Select the cipher suite for dtls handshake.
 *
 * @param[in] cipher      cipher suite (Note : Make sure endianness).
 *                          TLS_RSA_WITH_AES_256_CBC_SHA256          0x3D
 *                          TLS_RSA_WITH_AES_128_GCM_SHA256          0x009C
 *                          TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256  0xC02B
 *                          TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8       0xC0AE
 *                          TLS_ECDHE_ECDSA_WITH_AES_128_CCM         0xC0AC
 *                          TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256  0xC023
 *                          TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  0xC024
 *                          TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384  0xC02C
 *                          TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256    0xC037
 *                          TLS_ECDH_anon_WITH_AES_128_CBC_SHA       0xC018
 * @param[in] adapterType transport adapter (TCP/IP/BLE)
 *
 * @return ::OC_STACK_OK if successful.
 */
OCStackResult OC_CALL OCSelectCipherSuite(uint16_t cipher, OCTransportAdapter adapterType);

/**
 * Return the scope level of a given IP address.
 *
 * @param[in] addr          remote IP address.
 * @param[out] scope        scope level of given IP address.
 *
 * @return ::OC_STACK_OK if successful.
 */
OCStackResult OC_CALL OCGetIpv6AddrScope(const char *addr, OCTransportFlags *scope);

/**
* Return the Content Format from Entity Handler Request.
*
* @param[in] ehRequest          client request on entity handler
* @param[out] pContentFormat    media content format enum OCPayloadFormat
* @param[out] pAcceptVersion    accept version (e.g: 2048 for "1.0.0", NULL input is accepted)
*
* @return ::OC_STACK_OK if successful.
*/
OCStackResult OC_CALL OCGetRequestPayloadVersion(OCEntityHandlerRequest *ehRequest,
                                  OCPayloadFormat* pContentFormat, uint16_t* pAcceptVersion);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* OCSTACK_H_ */
