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

/**
 * @file
 *
 * This file contains the declaration of classes and its members related to
 * OCPlatform.
 */

#ifndef OC_PLATFORM_H_
#define OC_PLATFORM_H_
#include <OCApi.h>
#include <OCPlatform_impl.h>
namespace OC
{
    /**
    * This namespace contains the main entrance/functionality of the product.
    * It may be used with OC::OCPlatform::functionName.  To set a custom configuration,
    * the implementer must make a call to OCPlatform::Configure before the first usage
    * of a function in this namespace.
    */
    namespace OCPlatform
    {
        /**
        * API for overwriting the default configuration of the OCPlatform object.
        * @note Any calls made to this AFTER the first call to OCPlatform::Instance
        * will have no affect
        */
        void Configure(const PlatformConfig& config);

        // typedef for handle to cancel presence info with
        typedef OCDoHandle OCPresenceHandle;

        /**
         * API for stop Base layer including resource and connectivity abstraction.
         * The API is reference counted. The calls to start and stop need to be balanced.
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult stop();

        /**
         * API for start Base layer including resource and connectivity abstraction.
         * OCInit will be invoked.
         * The API is reference counted. The calls to start and stop need to be balanced.
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult start();

        /**
         * API for notifying base that resource's attributes have changed.
         *
         * @param resourceHandle resource handle of the resource
         *
         * @return Returns ::OC_STACK_OK if success.
         * @note This API is for server side only.
         * @note OCResourceHandle is defined in ocstack.h
         * @note OCStackResult is defined in ocstack.h.
         * @see notifyAllObservers(OCResourceHandle, QualityOfService)
         */
        OCStackResult notifyAllObservers(OCResourceHandle resourceHandle);

        /**
         * @overload
         *
         * @param resourceHandle resource handle of the resource
         * @param QoS the quality of communication
         * @see notifyAllObservers(OCResourceHandle)
         */
        OCStackResult notifyAllObservers(OCResourceHandle resourceHandle, QualityOfService QoS);

        /**
         * API for notifying only specific clients that resource's attributes have changed.
         *
         * @param resourceHandle resource handle of the resource
         * @param observationIds std vector of observationIds. These set of ids are ones which
         * which will be notified upon resource change.
         * @param responsePtr OCResourceResponse pointer used by app to fill the response for this
         * resource change.
         *
         * @return Returns ::OC_STACK_OK if success.
         * @note This API is for server side only.
         * @note OCResourceHandle is defined in ocstack.h.
         * @note OCStackResult is defined in ocstack.h.
         * @see notifyListOfObservers(OCResourceHandle, ObservationIds&, const std::shared_ptr<OCResourceResponse>, QualityOfService)
         */
        OCStackResult notifyListOfObservers(
                    OCResourceHandle resourceHandle,
                    ObservationIds& observationIds,
                    const std::shared_ptr<OCResourceResponse> responsePtr);
        /**
         * @overload
         *
         * @param resourceHandle resource handle of the resource
         * @param observationIds std vector of observationIds. These set of ids are ones which
         * which will be notified upon resource change.
         * @param responsePtr OCResourceResponse pointer used by app to fill the response for this
         * resource change.
         * @param QoS the quality of communication
         * @see notifyListOfObservers(OCResourceHandle, ObservationIds&, const std::shared_ptr<OCResourceResponse>)
         */
        OCStackResult notifyListOfObservers(
                    OCResourceHandle resourceHandle,
                    ObservationIds& observationIds,
                    const std::shared_ptr<OCResourceResponse> responsePtr,
                    QualityOfService QoS);

        /**
         * API for Service and Resource Discovery.
         * @note This API applies to client side only.
         *
         * @param host Host IP Address of a service to direct resource discovery query. If null or
         *        empty, performs multicast resource discovery query
         * @param resourceURI name of the resource. If null or empty, performs search for all
         *       resource names
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param resourceHandler Handles callbacks, success states and failure states.
         *
         *        Four modes of discovery defined as follows:
         *        (NULL/Empty, NULL/Empty) - Performs ALL service discovery AND ALL resource
         *           discovery.
         *        (NULL/Empty, Not Empty) - Performs query for a filtered/scoped/particular
         *                                   resource(s) from ALL services.
         *        (Not Empty, NULL/Empty) - Performs ALL resource discovery on a particular service.
         *        (Not Empty, Not Empty) - Performs query for a filtered/scoped/particular
         *                                   resource(s)
         *                                  from a particular service.
         *
         * @return Returns ::OC_STACK_OK if success.
         * @note First parameter 'host' currently represents an IP address. This will change in
         * future and will refer to endpoint interface so that we can refer to other transports such
         * as BTH etc.
         * @note OCStackResult is defined in ocstack.h.
         * @see findResource(const std::string&, const std::string&, OCConnectivityType, FindCallback, QualityOfService)
         * @note To get the tcpPort information, user must call getAllHosts() from the OCResource
           object in FindCallback. To use a TCP endpoint that has been received by getAllHosts(),
           user must use the setHost() function.
         */
        OCStackResult findResource(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindCallback resourceHandler);
        /**
         * @overload
         *
         * @param host Host IP Address of a service to direct resource discovery query. If null or
         *        empty, performs multicast resource discovery query
         * @param resourceURI name of the resource. If null or empty, performs search for all
         *       resource names
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param resourceHandler Handles callbacks, success states and failure states.
         *
         *        Four modes of discovery defined as follows:
         *        (NULL/Empty, NULL/Empty) - Performs ALL service discovery AND ALL resource
         *           discovery.
         *        (NULL/Empty, Not Empty) - Performs query for a filtered/scoped/particular
         *                                   resource(s) from ALL services.
         *        (Not Empty, NULL/Empty) - Performs ALL resource discovery on a particular service.
         *        (Not Empty, Not Empty) - Performs query for a filtered/scoped/particular
         *                                   resource(s)
         *                                  from a particular service.
         * @param QoS QualityOfService the quality of communication
         * @see findResource(const std::string&, const std::string&, OCConnectivityType, FindCallback)
         * @note To get the tcpPort information, user must call getAllHosts() from the OCResource
           object in FindCallback. To use a TCP endpoint that has been received by getAllHosts(),
           user must use the setHost() function.
         */
        OCStackResult findResource(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindCallback resourceHandler,
                    QualityOfService QoS);

        OCStackResult findResource(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindCallback resourceHandler,
                    FindErrorCallback errorHandler);

        OCStackResult findResource(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindCallback resourceHandler,
                    FindErrorCallback errorHandler, QualityOfService QoS);

        OCStackResult findResourceList(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindResListCallback resourceHandler,
                    QualityOfService QoS = QualityOfService::LowQos);

        OCStackResult findResourceList(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindResListCallback resourceHandler,
                    FindErrorCallback errorHandler, QualityOfService QoS = QualityOfService::LowQos);

        OCStackResult setPropertyValue(OCPayloadType type, const std::string& tag,
                    const std::string& value);
        OCStackResult setPropertyValue(OCPayloadType type, const std::string& tag,
                    const std::vector<std::string>& value);
        OCStackResult getPropertyValue(OCPayloadType type, const std::string& tag,
                    std::string& value);
        OCStackResult getPropertyValue(OCPayloadType type, const std::string& tag,
                    std::vector<std::string>& value);
        /**
         * API for Device Discovery
         *
         *
         * @param host Host IP Address. If null or empty, Multicast is performed.
         * @param deviceURI Uri containing address to the virtual device in C Stack
                                ("/oic/d")
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param deviceInfoHandler device discovery callback
         *
         * @return Returns ::OC_STACK_OK if success.
         * @note OCStackResult is defined in ocstack.h.
         * @see getDeviceInfo(const std::string&, const std::string&, OCConnectivityType, FindDeviceCallback, QualityOfService)
         */
        OCStackResult getDeviceInfo(const std::string& host, const std::string& deviceURI,
                    OCConnectivityType connectivityType, FindDeviceCallback deviceInfoHandler);
        /**
         * @overload
         *
         * @param host Host IP Address. If null or empty, Multicast is performed.
         * @param deviceURI Uri containing address to the virtual device in C Stack
                                ("/oic/d")
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param deviceInfoHandler device discovery callback
         * @param QoS the quality of communication
         * @see getDeviceInfo(const std::string&, const std::string&, OCConnectivityType, FindDeviceCallback)
         */
        OCStackResult getDeviceInfo(const std::string& host, const std::string& deviceURI,
                    OCConnectivityType connectivityType, FindDeviceCallback deviceInfoHandler,
                    QualityOfService QoS);

        /**
         * API for Platform Discovery
         *
         *
         * @param host Host IP Address. If null or empty, Multicast is performed.
         * @param platformURI Uri containing address to the virtual platform in C Stack
                                ("/oic/p")
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param platformInfoHandler platform discovery callback
         *
         * @return Returns ::OC_STACK_OK if success.
         *
         * @note OCStackResult is defined in ocstack.h.
         * @see getPlatformInfo(const std::string&, const std::string&, OCConnectivityType, FindPlatformCallback, QualityOfService)
         */
        OCStackResult getPlatformInfo(const std::string& host, const std::string& platformURI,
                    OCConnectivityType connectivityType, FindPlatformCallback platformInfoHandler);
        /**
         * @overload
         *
         * @param host Host IP Address. If null or empty, Multicast is performed.
         * @param platformURI Uri containing address to the virtual platform in C Stack
                                ("/oic/p")
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param platformInfoHandler platform discovery callback
         * @param QoS the quality of communication
         * @see getPlatformInfo(const std::string&, const std::string&, OCConnectivityType, FindPlatformCallback)
         */
        OCStackResult getPlatformInfo(const std::string& host, const std::string& platformURI,
                    OCConnectivityType connectivityType, FindPlatformCallback platformInfoHandler,
                    QualityOfService QoS);

        /**
         * This function returns flags of supported endpoint TPS on stack.
         *
         * @param[out] supportedTps Bit combinations of supported OCTpsSchemeFlags.
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult getSupportedTransportsInfo(OCTpsSchemeFlags& supportedTps);

        /**
        * This API registers a resource with the server
        * @note This API applies to server side only.
        *
        * @param resourceHandle Upon successful registration, resourceHandle will be filled
        * @param resourceURI The URI of the resource. Example: "a/light". See NOTE below
        * @param resourceTypeName The resource type. Example: "core.light"
        * @param resourceInterface The resource interface (whether it is collection etc).
        * @param entityHandler entity handler callback.
        * @param resourceProperty indicates the property of the resource. Defined in octypes.h.
        * setting resourceProperty as OC_DISCOVERABLE will allow Discovery of this resource
        * setting resourceProperty as OC_OBSERVABLE will allow observation
        * setting resourceProperty as OC_DISCOVERABLE | OC_OBSERVABLE will allow both discovery
        * and observation
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note "a/light" is a relative reference to URI.
        * Above relative reference to URI will be prepended (by core) with a host IP
        * Therefore, fully qualified URI format would be
        * "CoAP(s)+protocol-URI-Scheme://HostIP-Address/relativeURI"
        * Example, a relative reference to URI: 'a/light' will result in a fully qualified URI:
        *   "coap://192.168.1.1:5246/a/light", "coaps://192.168.1.1:5246/a/light"
        * @note OCStackResult is defined in octypes.h.
        * @note entity handler callback :
        * When you set specific return value like OC_EH_CHANGED, OC_EH_CONTENT,
        * OC_EH_SLOW and etc in entity handler callback,
        * ocstack will be not send response automatically to client
        * except for error return value like OC_EH_ERROR
        * If you want to send response to client with specific result,
        * OCDoResponse API should be called with the result value.
        */
        OCStackResult registerResource(OCResourceHandle& resourceHandle,
                        std::string& resourceURI,
                        const std::string& resourceTypeName,
                        const std::string& resourceInterface,
                        EntityHandler entityHandler,
                        uint8_t resourceProperty);

        /**
        * This API registers a resource with the server
        * @note This API applies to server side only.
        *
        * @param resourceHandle Upon successful registration, resourceHandle will be filled
        * @param resourceURI The URI of the resource. Example: "a/light". See NOTE below
        * @param resourceTypeName The resource type. Example: "core.light"
        * @param resourceInterface The resource interface (whether it is collection etc).
        * @param entityHandler Entity handler callback.
        * @param resourceProperty indicates the property of the resource. Defined in octypes.h.
        * @param resourceTpsTypes Transport Protocol Suites(TPS) types of resource for
                                  open resource to specific transport adapter (e.g., TCP, UDP)
                                  with messaging protocol(e.g., COAP, COAPS).
                                  Example: "OC_COAP | OC_COAP_TCP"
        * setting resourceProperty as OC_DISCOVERABLE will allow Discovery of this resource
        * setting resourceProperty as OC_OBSERVABLE will allow observation
        * setting resourceProperty as OC_DISCOVERABLE | OC_OBSERVABLE will allow both discovery
        * and observation
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note "a/light" is a relative reference to URI.
        * Above relative reference to URI will be prepended (by core) with a host IP
        * Therefore, fully qualified URI format would be
        * "CoAP(s)+protocol-URI-Scheme://HostIP-Address/relativeURI"
        * Example, a relative reference to URI: 'a/light' will result in a fully qualified URI:
        *   "coap://192.168.1.1:5246/a/light", "coaps://192.168.1.1:5246/a/light"
        * @note OCStackResult is defined in octypes.h.
        * @note entity handler callback :
        * When you set specific return value like OC_EH_CHANGED, OC_EH_CONTENT,
        * OC_EH_SLOW and etc in entity handler callback,
        * ocstack will be not send response automatically to client
        * except for error return value like OC_EH_ERROR
        * If you want to send response to client with specific result,
        * OCDoResponse API should be called with the result value.
        */
        OCStackResult registerResource(OCResourceHandle& resourceHandle,
                        std::string& resourceURI,
                        const std::string& resourceTypeName,
                        const std::string& resourceInterface,
                        EntityHandler entityHandler,
                        uint8_t resourceProperty,
                        OCTpsSchemeFlags resourceTpsTypes);

        /**
         * This API registers a resource with the server
         * @note This API applies to server & client side.
         *
         * @param resourceHandle Upon successful registration, resourceHandle will be filled
         * @param resource The instance of OCResource that all data filled.
         *
         * @return Returns ::OC_STACK_OK if success.
         * @note OCStackResult is defined in ocstack.h.
         */
        OCStackResult registerResource(OCResourceHandle& resourceHandle,
                        const std::shared_ptr< OCResource > resource);

        /**
        * Register Device Info
        *
        * @deprecated: Use setPropertyValue instead.
        *
        * @param deviceInfo structure containing all the device specific information
        * @return Returns ::OC_STACK_OK  if no errors and ::OC_STACK_ERROR in case of stack process error
        */
        OCStackResult registerDeviceInfo(const OCDeviceInfo deviceInfo);

        /**
        * Register Platform Info
        *
        * @param platformInfo structure containing all the platform specific information
        * @return Returns ::OC_STACK_OK if no errors and ::OC_STACK_ERROR in case of stack process error
        */
        OCStackResult registerPlatformInfo(const OCPlatformInfo platformInfo);

        /**
        * Set default device entity handler
        *
        * @param entityHandler entity handler to handle requests for
        *                      any undefined resources or default actions.
        *                      if NULL is passed it removes the device default entity handler.
        * @return Returns ::OC_STACK_OK  if no errors and ::OC_STACK_ERROR in case of stack process error
        * @note entity handler callback :
        * When you set specific return value like OC_EH_CHANGED, OC_EH_CONTENT,
        * OC_EH_SLOW and etc in entity handler callback,
        * ocstack will be not send response automatically to client
        * except for error return value like OC_EH_ERROR
        * If you want to send response to client with specific result,
        * sendResponse API should be called with the result value.
        */
        OCStackResult setDefaultDeviceEntityHandler(EntityHandler entityHandler);

        /**
        * This API unregisters a resource with the server
        * @note This API applies to server side only.
        *
        * @param resourceHandle This is the resource handle which we need to unregister from the
        * server
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note OCStackResult is defined in ocstack.h.
        */
        OCStackResult unregisterResource(const OCResourceHandle& resourceHandle);

        /**
        * Add a resource to a collection resource.
        *
        * @param collectionHandle handle to the collection resource
        * @param resourceHandle handle to resource to be added to the collection resource
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note OCStackResult is defined in ocstack.h.
        * @note bindResource must be used only after the both collection resource and
        * resource to add under a collections are created and respective handles obtained
        *
        * @par Example:
        * -# registerResource(homeResourceHandle, "a/home", "home", Link_Interface,
        *   entityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# registerResource(kitchenResourceHandle, "a/kitchen", "kitchen", Link_Interface,
        *   entityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# bindResource(homeResourceHandle, kitchenResourceHandle);
        * @par
        * At the end of Step 3, resource "a/home" will contain a reference to "a/kitchen".
        */
        OCStackResult bindResource(const OCResourceHandle collectionHandle,
                const OCResourceHandle resourceHandle);

        /**
        * Add multiple resources to a collection resource.
        *
        * @param collectionHandle handle to the collection resource
        * @param addedResourceHandleList reference to list of resource handles to be added to the
        *   collection resource
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note OCStackResult is defined in ocstack.h.
        * @note bindResources must be used only after the both collection resource and
        * list of resources to add under a collection are created and respective handles
        * obtained.
        *
        * @par Example:
        * -# registerResource(homeResourceHandle, "a/home", "home", Link_Interface,
        *   homeEntityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# registerResource(kitchenResourceHandle, "a/kitchen", "kitchen", Link_Interface,
        *   kitchenEntityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# registerResource(roomResourceHandle, "a/room", "room", Link_Interface,
        *   roomEntityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# std::vector<OCResourceHandle> rList; rList.push_back(kitchenResourceHandle);
        *   rList.push_back(roomResourceHandle);
        * -# bindResource(homeResourceHandle, rList);
        * @par
        * At the end of Step 5, resource "a/home" will contain a references to "a/kitchen" and
        *   "a/room"
        */
        OCStackResult bindResources(const OCResourceHandle collectionHandle,
                const std::vector<OCResourceHandle>& addedResourceHandleList);

        /**
        * Unbind a resource from a collection resource.
        *
        * @param collectionHandle handle to the collection resource
        * @param resourceHandle resource handle to be unbound from the collection resource
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note OCStackResult is defined in ocstack.h.
        * @note unbindResource must be used only after the both collection resource and
        * resource to unbind from a collection are created and respective handles obtained
        *
        * @par Example:
        * -# registerResource(homeResourceHandle, "a/home", "home", Link_Interface,
        *   entityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# registerResource(kitchenResourceHandle, "a/kitchen", "kitchen", Link_Interface,
        *   entityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# bindResource(homeResourceHandle, kitchenResourceHandle);
        * -# unbindResource(homeResourceHandle, kitchenResourceHandle);
        * @par
        * At the end of Step 4, resource "a/home" will no longer reference "a/kitchen".
        */
        OCStackResult unbindResource(const OCResourceHandle collectionHandle,
                    const OCResourceHandle resourceHandle);

        /**
        * Unbind resources from a collection resource.
        *
        * @param collectionHandle handle to the collection resource
        * @param resourceHandleList List of resource handles to be unbound from the collection
        *   resource
        *
        * @return Returns ::OC_STACK_OK if success.
        * @note OCStackResult is defined in ocstack.h.
        * @note unbindResources must be used only after the both collection resource and
        * list of resources resource to unbind from a collection are created and respective handles
        *   obtained.
        *
        * @par Example:
        * -# registerResource(homeResourceHandle, "a/home", "home", Link_Interface,
        *   homeEntityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# registerResource(kitchenResourceHandle, "a/kitchen", "kitchen", Link_Interface,
        *   kitchenEntityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# registerResource(roomResourceHandle, "a/room", "room", Link_Interface,
        *   roomEntityHandler, OC_DISCOVERABLE | OC_OBSERVABLE);
        * -# std::vector<OCResourceHandle> rList; rList.push_back(kitchenResourceHandle);
        *   rList.push_back(roomResourceHandle);
        * -# bindResource(homeResourceHandle, rList);
        * -# unbindResources(homeResourceHandle, rList);
        * @par
        * At the end of Step 6, resource "a/home" will no longer reference to "a/kitchen" and
        *   "a/room"
        */
        OCStackResult unbindResources(const OCResourceHandle collectionHandle,
                        const std::vector<OCResourceHandle>& resourceHandleList);

        /**
        * Binds a type to a particular resource
        * @param resourceHandle handle to the resource
        * @param resourceTypeName new typename to bind to the resource
        *
        * @return Returns ::OC_STACK_OK if success.
        */
        OCStackResult bindTypeToResource(const OCResourceHandle& resourceHandle,
                        const std::string& resourceTypeName);

        /**
        * Binds an interface to a particular resource
        * @param resourceHandle handle to the resource
        * @param resourceInterfaceName new interface  to bind to the resource
        *
        * @return Returns ::OC_STACK_OK if success.
        */
        OCStackResult bindInterfaceToResource(const OCResourceHandle& resourceHandle,
                        const std::string& resourceInterfaceName);


        /**
        * Start Presence announcements.
        *
        * @param ttl time to live
        * @par
        * If ttl is '0', then the default stack value will be used (60 Seconds).
        * If ttl is greater than OC_MAX_PRESENCE_TTL_SECONDS, then the ttl will be set to
        * OC_MAX_PRESENCE_TTL_SECONDS.
        * @par
        * @return Returns ::OC_STACK_OK if success.
        *
        * Server can call this function when it comes online for the
        * first time, or when it comes back online from offline mode,
        * or when it re enters network.
        *
        */
        OCStackResult startPresence(const unsigned int ttl);

        /**
        * Stop Presence announcements.
        *
        * @return Returns ::OC_STACK_OK if success.
        *
        * Server can call this function when it is terminating,
        * going offline, or when going away from network.
        *
        */
        OCStackResult stopPresence();

        /**
         * subscribes to a server's presence change events.  By making this subscription,
         * every time a server adds/removes/alters a resource, starts or is intentionally
         * stopped (potentially more to be added later).
         *
         * @param presenceHandle a handle object that can be used to identify this subscription
         *               request.  It can be used to unsubscribe from these events in the future.
         *               It will be set upon successful return of this method.
         * @param host The IP address/addressable name of the server to subscribe to.
         *               This should be in the format coap://address:port
         *               If empty, the multicast subscribe presence is performed.
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param presenceHandler callback function that will receive notifications/subscription
         *               events
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult subscribePresence(OCPresenceHandle& presenceHandle, const std::string& host,
                        OCConnectivityType connectivityType, SubscribeCallback presenceHandler);
        /**
         * @overload
         *
         * subscribes to a server's presence change events.  By making this subscription,
         * every time a server adds/removes/alters a resource, starts or is intentionally
         * stopped (potentially more to be added later).
         *
         * @param presenceHandle a handle object that can be used to identify this subscription
         *               request.  It can be used to unsubscribe from these events in the future.
         *               It will be set upon successful return of this method.
         * @param host The IP address/addressable name of the server to subscribe to.
         *               This should be in the format coap://address:port
         *               If empty, the multicast subscribe presence is performed.
         * @param resourceType a resource type specified as a filter for subscription callbacks.
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param presenceHandler callback function that will receive notifications/subscription
         *               events
         *
         * @see subscribePresence(OCPresenceHandle&, const std::string&, OCConnectivityType, SubscribeCallback)
         */
        OCStackResult subscribePresence(OCPresenceHandle& presenceHandle, const std::string& host,
                        const std::string& resourceType, OCConnectivityType connectivityType,
                        SubscribeCallback presenceHandler);

        /**
         * unsubscribes from a previously subscribed server's presence events. Note that
         * you may for a short time still receive events from the server since it may take time
         * for the unsubscribe to take effect.
         *
         * @param presenceHandle the handle object provided by the subscribePresence call that
         *               identifies this subscription.
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult unsubscribePresence(OCPresenceHandle presenceHandle);

#ifdef WITH_CLOUD
        /**
         * Subscribes to a server's device presence change events.
         *
         * @param presenceHandle a handle object that can be used to identify this subscription
         *               request.  It can be used to unsubscribe from these events in the future.
         *               It will be set upon successful return of this method.
         * @param host The IP address/addressable name of the server to subscribe to.
         *               This should be in the format coap://address:port
         * @param di Vector which can have the devices id.
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         * @param observeHandler handles callback
         *        The callback function will be invoked with a map of attribute name and values.
         *        The callback function will also have the result from this observe operation
         *        This will have error codes
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult subscribeDevicePresence(OCPresenceHandle& presenceHandle,
                                              const std::string& host,
                                              const std::vector<std::string>& di,
                                              OCConnectivityType connectivityType,
                                              ObserveCallback callback);
#endif

        /**
         * Creates a resource proxy object so that get/put/observe functionality
         * can be used without discovering the object in advance.  Note that the
         * consumer of this method needs to provide all of the details required to
         * correctly contact and observe the object. If the consumer lacks any of
         * this information, they should discover the resource object normally.
         * Additionally, you can only create this object if OCPlatform was initialized
         * to be a Client or Client/Server.  Otherwise, this will return an empty
         * shared ptr.
         *
         * @param host a string containing a resolvable "coap(s)", "coap(s)+protocol" uri scheme
         *        of the server holding the resource.
         *        Currently this should be in the format coap(s)://address:port or
         *        coap(s)+protocol://address:port, though in the future, we expect this to
         *        change to //address:port
         *
         * @param uri the rest of the resource's URI that will permit messages to be
         *           properly routed.  Example: /a/light
         *
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           transport method and IP address scope.
         *                           Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         *                           if you want to use a specific Flag like IPv4,
         *                           you should apply OR operation for the flag in here.
         *                           Example: static_cast<OCConnectivityType>(CT_ADAPTER_TCP
         *                                                                    | OC_IP_USE_V4)
         *
         * @param isObservable a boolean containing whether the resource supports observation
         *
         * @param resourceTypes a collection of resource types implemented by the resource
         *
         * @param interfaces a collection of interfaces that the resource supports/implements
         * @return OCResource::Ptr a shared pointer to the new resource object
         */
        OCResource::Ptr constructResourceObject(const std::string& host,
                        const std::string& uri,
                        OCConnectivityType connectivityType, bool isObservable,
                        const std::vector<std::string>& resourceTypes,
                        const std::vector<std::string>& interfaces);

        /**
         * Allows application entity handler to send response to an incoming request.
         *
         * @param pResponse OCResourceResponse pointer that will permit to set values related
         * to resource response.
         *
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult sendResponse(const std::shared_ptr<OCResourceResponse> pResponse);

#ifdef WITH_CLOUD
        /**
         * Create an account manager object that can be used for doing request to account server.
         * You can only create this object if OCPlatform was initialized to be a Client or
         * Client/Server. Otherwise, this will return an empty shared ptr.
         *
         * @note For now, OCPlatform SHOULD be initialized to be a Client/Server(Both) for the
         *       methods of this object to work since device id is not generated on Client mode.
         *
         * @param host Host IP Address of a account server.
         * @param connectivityType ::OCConnectivityType type of connectivity indicating the
         *                           interface. Example: CT_DEFAULT, CT_ADAPTER_IP, CT_ADAPTER_TCP.
         *                           if you want to use a specific Flag like IPv4,
         *                           you should apply OR operation for the flag in here.
         *                           Example: static_cast<OCConnectivityType>(CT_ADAPTER_TCP
         *                                                                    | OC_IP_USE_V4)
         *
         * @return OCAccountManager::Ptr a shared pointer to the new account manager object
         */
        OCAccountManager::Ptr constructAccountManagerObject(const std::string& host,
                                                            OCConnectivityType connectivityType);
#endif // WITH_CLOUD

        /**
         * gets the deviceId of the client
         *
         * @param deviceId pointer.
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult getDeviceId(OCUUIdentity *deviceId);

        /**
         * sets the deviceId of the client
         *
         * @param deviceId pointer.
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult setDeviceId(const OCUUIdentity *deviceId);
    }
}

#endif // OC_PLATFORM_H_
