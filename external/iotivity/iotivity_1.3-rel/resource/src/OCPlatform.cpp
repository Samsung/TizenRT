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

//******************************************************************
// File name:
//     OCPlatform.cpp
//
// Description: Implementation of the OCPlatform.
//
//
//
//*********************************************************************
#include <OCPlatform.h>
namespace OC
{
    namespace OCPlatform
    {
        void Configure(const PlatformConfig& config)
        {
            OCPlatform_impl::Configure(config);
        }

        OCStackResult start()
        {
            return OCPlatform_impl::Instance().start();
        }

        OCStackResult stop()
        {
            return OCPlatform_impl::Instance().stop();
        }

        OCStackResult setDefaultDeviceEntityHandler(EntityHandler entityHandler)
        {
            return OCPlatform_impl::Instance().setDefaultDeviceEntityHandler(entityHandler);
        }

        OCStackResult notifyAllObservers(OCResourceHandle resourceHandle,
                                     QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().notifyAllObservers(resourceHandle, QoS);
        }

        OCStackResult notifyAllObservers(OCResourceHandle resourceHandle)
        {
            return OCPlatform_impl::Instance().notifyAllObservers(resourceHandle);
        }

        OCStackResult notifyListOfObservers(OCResourceHandle resourceHandle,
                                     ObservationIds& observationIds,
                                     const std::shared_ptr<OCResourceResponse> pResponse)
        {
            return OCPlatform_impl::Instance().notifyListOfObservers(resourceHandle,
                                     observationIds, pResponse);
        }

        OCStackResult notifyListOfObservers(OCResourceHandle resourceHandle,
                                     ObservationIds& observationIds,
                                     const std::shared_ptr<OCResourceResponse> pResponse,
                                     QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().notifyListOfObservers(resourceHandle,
                                     observationIds, pResponse, QoS);
        }

        OCResource::Ptr constructResourceObject(const std::string& host,
                                     const std::string& uri,
                                     OCConnectivityType connectivityType,
                                     bool isObservable,
                                     const std::vector<std::string>& resourceTypes,
                                     const std::vector<std::string>& interfaces)
        {
            return OCPlatform_impl::Instance().constructResourceObject(host,
                                     uri, connectivityType,
                                     isObservable,
                                     resourceTypes, interfaces);
        }

        OCStackResult findResource(const std::string& host,
                                 const std::string& resourceName,
                                 OCConnectivityType connectivityType,
                                 FindCallback resourceHandler)
        {
            return OCPlatform_impl::Instance().findResource(host, resourceName,
                                 connectivityType, resourceHandler);
        }

        OCStackResult findResource(const std::string& host,
                                 const std::string& resourceName,
                                 OCConnectivityType connectivityType,
                                 FindCallback resourceHandler,
                                 QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().findResource(host, resourceName,
                                 connectivityType, resourceHandler, QoS);
        }

        OCStackResult findResource(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindCallback resourceHandler,
                                            FindErrorCallback errorHandler)
        {
            return OCPlatform_impl::Instance().findResource(host, resourceName,
                                    connectivityType, resourceHandler, errorHandler);
        }

        OCStackResult findResource(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindCallback resourceHandler,
                                            FindErrorCallback errorHandler,
                                            QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().findResource(host, resourceName,
                                    connectivityType, resourceHandler, errorHandler, QoS);
        }

        OCStackResult findResourceList(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindResListCallback resourceHandler,
                    QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().findResourceList(host, resourceURI,
                                    connectivityType, resourceHandler, QoS);
        }

        OCStackResult findResourceList(const std::string& host, const std::string& resourceURI,
                    OCConnectivityType connectivityType, FindResListCallback resourceHandler,
                    FindErrorCallback errorHandler, QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().findResourceList(host, resourceURI,
                                    connectivityType, resourceHandler, errorHandler, QoS);
        }

        OCStackResult getDeviceInfo(const std::string& host,
                                 const std::string& deviceURI,
                                 OCConnectivityType connectivityType,
                                 FindDeviceCallback deviceInfoHandler)
        {
            return OCPlatform_impl::Instance().getDeviceInfo(host, deviceURI,
                                 connectivityType, deviceInfoHandler);
        }

        OCStackResult getDeviceInfo(const std::string& host,
                                 const std::string& deviceURI,
                                 OCConnectivityType connectivityType,
                                 FindDeviceCallback deviceInfoHandler,
                                 QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().getDeviceInfo(host, deviceURI, connectivityType,
                                 deviceInfoHandler, QoS);
        }

        OCStackResult getPlatformInfo(const std::string& host,
                                 const std::string& platformURI,
                                 OCConnectivityType connectivityType,
                                 FindPlatformCallback platformInfoHandler)
        {
            return OCPlatform_impl::Instance().getPlatformInfo(host, platformURI,
                                 connectivityType, platformInfoHandler);
        }

        OCStackResult getPlatformInfo(const std::string& host,
                                 const std::string& platformURI,
                                 OCConnectivityType connectivityType,
                                 FindPlatformCallback platformInfoHandler,
                                 QualityOfService QoS)
        {
            return OCPlatform_impl::Instance().getPlatformInfo(host, platformURI, connectivityType,
                                 platformInfoHandler, QoS);
        }

        OCStackResult getSupportedTransportsInfo(OCTpsSchemeFlags& supportedTps)
        {
            return OCPlatform_impl::Instance().getSupportedTransportsInfo(supportedTps);
        }

        OCStackResult registerResource(OCResourceHandle& resourceHandle,
                                 std::string& resourceURI,
                                 const std::string& resourceTypeName,
                                 const std::string& resourceInterface,
                                 EntityHandler entityHandler,
                                 uint8_t resourceProperty
                                 )
        {
            return OCPlatform_impl::Instance().registerResource(resourceHandle, resourceURI,
                                 resourceTypeName, resourceInterface,
                                 entityHandler, resourceProperty);
        }

        OCStackResult registerResource(OCResourceHandle& resourceHandle,
                                 std::string& resourceURI,
                                 const std::string& resourceTypeName,
                                 const std::string& resourceInterface,
                                 EntityHandler entityHandler,
                                 uint8_t resourceProperty,
                                 OCTpsSchemeFlags resourceTpsTypes)
        {
            return OCPlatform_impl::Instance().registerResource(resourceHandle, resourceURI,
                                 resourceTypeName, resourceInterface,
                                 entityHandler, resourceProperty,
                                 resourceTpsTypes);
        }

        OCStackResult registerResource(OCResourceHandle& resourceHandle,
                                 const std::shared_ptr< OCResource > resource)
        {
            return OCPlatform_impl::Instance().registerResource(resourceHandle, resource);
        }

        OCStackResult registerDeviceInfo(const OCDeviceInfo deviceInfo)
        {
            return OCPlatform_impl::Instance().registerDeviceInfo(deviceInfo);
        }

        OCStackResult registerPlatformInfo(const OCPlatformInfo platformInfo)
        {
            return OCPlatform_impl::Instance().registerPlatformInfo(platformInfo);
        }

        OCStackResult setPropertyValue(OCPayloadType type, const std::string& tag,
                                       const std::string& value)
        {
            return OCPlatform_impl::Instance().setPropertyValue(type, tag, value);
        }

        OCStackResult setPropertyValue(OCPayloadType type, const std::string& tag,
                                       const std::vector<std::string>& value)
        {
            return OCPlatform_impl::Instance().setPropertyValue(type, tag, value);
        }

        OCStackResult getPropertyValue(OCPayloadType type, const std::string& tag,
                                       std::string& value)
        {
            return OCPlatform_impl::Instance().getPropertyValue(type, tag, value);
        }

        OCStackResult getPropertyValue(OCPayloadType type, const std::string& tag,
                                       std::vector<std::string>& value)
        {
            return OCPlatform_impl::Instance().getPropertyList(type, tag, value);
        }

        OCStackResult unregisterResource(const OCResourceHandle& resourceHandle)
        {
            return OCPlatform_impl::Instance().unregisterResource(resourceHandle);
        }

        OCStackResult unbindResource(OCResourceHandle collectionHandle,
                                 OCResourceHandle resourceHandle)
        {
            return OCPlatform_impl::Instance().unbindResource(collectionHandle, resourceHandle);
        }

        OCStackResult unbindResources(const OCResourceHandle collectionHandle,
                                 const std::vector<OCResourceHandle>& resourceHandles)
        {
            return OCPlatform_impl::Instance().unbindResources(collectionHandle, resourceHandles);
        }

        OCStackResult bindResource(const OCResourceHandle collectionHandle,
                                 const OCResourceHandle resourceHandle)
        {
            return OCPlatform_impl::Instance().bindResource(collectionHandle, resourceHandle);
        }

        OCStackResult bindResources(const OCResourceHandle collectionHandle,
                                 const std::vector<OCResourceHandle>& resourceHandles
                                 )
        {
            return OCPlatform_impl::Instance().bindResources(collectionHandle, resourceHandles);
        }

        OCStackResult bindTypeToResource(const OCResourceHandle& resourceHandle,
                                 const std::string& resourceTypeName)
        {
            return OCPlatform_impl::Instance().bindTypeToResource(resourceHandle,resourceTypeName);
        }

        OCStackResult bindInterfaceToResource(const OCResourceHandle& resourceHandle,
                                 const std::string& resourceInterfaceName)
        {
            return OCPlatform_impl::Instance().bindInterfaceToResource(resourceHandle,
                                                             resourceInterfaceName);
        }

        OCStackResult startPresence(const unsigned int announceDurationSeconds)
        {
            return OCPlatform_impl::Instance().startPresence(announceDurationSeconds);
        }

        OCStackResult stopPresence()
        {
            return OCPlatform_impl::Instance().stopPresence();
        }

        OCStackResult subscribePresence(OCPresenceHandle& presenceHandle,
                                     const std::string& host,
                                     OCConnectivityType connectivityType,
                                     SubscribeCallback presenceHandler)
        {
            return OCPlatform_impl::Instance().subscribePresence(presenceHandle, host,
                                                             connectivityType, presenceHandler);
        }

        OCStackResult subscribePresence(OCPresenceHandle& presenceHandle,
                                     const std::string& host,
                                     const std::string& resourceType,
                                     OCConnectivityType connectivityType,
                                     SubscribeCallback presenceHandler)
        {
            return OCPlatform_impl::Instance().subscribePresence(presenceHandle, host,
                                             resourceType, connectivityType, presenceHandler);
        }

        OCStackResult unsubscribePresence(OCPresenceHandle presenceHandle)
        {
            return OCPlatform_impl::Instance().unsubscribePresence(presenceHandle);
        }

#ifdef WITH_CLOUD
        OCStackResult subscribeDevicePresence(OCPresenceHandle& presenceHandle,
                                              const std::string& host,
                                              const std::vector<std::string>& di,
                                              OCConnectivityType connectivityType,
                                              ObserveCallback callback)
        {
            return OCPlatform_impl::Instance().subscribeDevicePresence(presenceHandle,
                                                                       host,
                                                                       di,
                                                                       connectivityType,
                                                                       callback);
        }
#endif

        OCStackResult sendResponse(const std::shared_ptr<OCResourceResponse> pResponse)
        {
            return OCPlatform_impl::Instance().sendResponse(pResponse);
        }

#ifdef WITH_CLOUD
        OCAccountManager::Ptr constructAccountManagerObject(const std::string& host,
                                                            OCConnectivityType connectivityType)
        {
            return OCPlatform_impl::Instance().constructAccountManagerObject(host,
                                                                             connectivityType);
        }
#endif // WITH_CLOUD

        OCStackResult getDeviceId(OCUUIdentity *deviceId)
        {
            return OCPlatform_impl::Instance().getDeviceId(deviceId);
        }

        OCStackResult setDeviceId(const OCUUIdentity *deviceId)
        {
            return OCPlatform_impl::Instance().setDeviceId(deviceId);
        }
    } // namespace OCPlatform
} //namespace OC
