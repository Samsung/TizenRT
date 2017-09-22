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
//     OCPlatform_impl.cpp
//
// Description: Implementation of the OCPlatform functionality.  It contains
// a singleton interface that is used only by the OCPlatform namespace and is the
// central entrance to the stack.
//
//
//
//*********************************************************************

#include "OCPlatform_impl.h"

#include <random>
#include <utility>
#include <functional>

#include "ocstack.h"

#include "OCPlatform.h"
#include "OCApi.h"
#include "OCException.h"
#include "OCUtilities.h"
#include "ocpayload.h"
#include "iotivity_debug.h"

#include "logger.h"
#include "oc_logger.hpp"

#define TAG "OIC_PLATFORM"

namespace OC
{

    PlatformConfig& OCPlatform_impl::globalConfig()
    {
        static PlatformConfig s_config;
        return s_config;
    }

    void OCPlatform_impl::Configure(const PlatformConfig& config)
    {
        OCRegisterPersistentStorageHandler(config.ps);
        globalConfig() = config;
    }

    OCPlatform_impl& OCPlatform_impl::Instance()
    {
        static OCPlatform_impl platform(globalConfig());
        return platform;
    }

    OCStackResult OCPlatform_impl::start()
    {
        OIC_LOG(INFO, TAG, "start");

        std::lock_guard<std::mutex> lock(m_startCountLock);

        if (0 != m_startCount)
        {
            assert(UINT_MAX != m_startCount);
            m_startCount++;
            return OC_STACK_OK;
        }

        // Reload from the global configuration.
        m_cfg = globalConfig();

        // First caller gets to initialize the underlying objects and start the stack.
        OCStackResult res = init(m_cfg);
        if (OC_STACK_OK != res)
        {
            return res;
        }

        OCTransportFlags serverFlags =
                        static_cast<OCTransportFlags>(m_cfg.serverConnectivity & CT_MASK_FLAGS);
        OCTransportFlags clientFlags =
                        static_cast<OCTransportFlags>(m_cfg.clientConnectivity & CT_MASK_FLAGS);
        res = OCInit2(m_modeType, serverFlags, clientFlags, m_cfg.transportType);
        if (OC_STACK_OK != res)
        {
            return res;
        }

        switch(m_modeType)
        {
            case OC_CLIENT:
                if (OC_STACK_OK != checked_guard(m_client, &IClientWrapper::start))
                {
                    res = OC_STACK_ERROR;
                }
                break;

            case OC_SERVER:
                if (OC_STACK_OK != checked_guard(m_server, &IServerWrapper::start))
                {
                    res = OC_STACK_ERROR;
                }
                break;

            case OC_CLIENT_SERVER:
            case OC_GATEWAY:
                if (OC_STACK_OK != checked_guard(m_client, &IClientWrapper::start))
                {
                    res = OC_STACK_ERROR;
                }
                if (OC_STACK_OK != checked_guard(m_server, &IServerWrapper::start))
                {
                    res = OC_STACK_ERROR;
                }
                break;

            default:
                assert(!"Unknown modeType");
                res = OC_STACK_ERROR;
                break;
        }

        if (OC_STACK_OK != res)
        {
            // Stop the underlying stack.
            // The counterpart of this function, OCInit2(), is called once by start().
            OC_VERIFY(OC_STACK_OK == OCStop());
            return res;
        }

        assert(UINT_MAX != m_startCount);
        m_startCount++;
        return OC_STACK_OK;
    }

    OCStackResult OCPlatform_impl::stop()
    {
        OIC_LOG(INFO, TAG, "stop");

        OCStackResult res = OC_STACK_OK;

        std::lock_guard<std::mutex> lock(m_startCountLock);

        // Only the last call to stop() gets to do the real clean up work.
        if (m_startCount == 1)
        {
            if (OC_CLIENT == m_modeType)
            {
                if (OC_STACK_OK != checked_guard(m_client, &IClientWrapper::stop))
                {
                    res = OC_STACK_ERROR;
                }
            }
            else if (OC_SERVER == m_modeType)
            {
                if (OC_STACK_OK != checked_guard(m_server, &IServerWrapper::stop))
                {
                    res = OC_STACK_ERROR;
                }
            }
            else if (OC_CLIENT_SERVER == m_modeType)
            {
                if (OC_STACK_OK != checked_guard(m_client, &IClientWrapper::stop))
                {
                    res = OC_STACK_ERROR;
                }

                if (OC_STACK_OK != checked_guard(m_server, &IServerWrapper::stop))
                {
                    res = OC_STACK_ERROR;
                }
            }
            else
            {
                res = OC_STACK_ERROR;
            }

            if (OC_STACK_OK == res)
            {
                res = OCStop();
            }
        }

        if (OC_STACK_OK == res)
        {
            assert(0 != m_startCount);
            m_startCount--;
        }

        return res;
    }

    OCStackResult OCPlatform_impl::init(const PlatformConfig& config)
    {
        OIC_LOG(INFO, TAG, "init");

        OCStackResult result = OC_STACK_NOTIMPL;
        switch(config.mode)
        {
            case ModeType::Server:
                m_server = m_WrapperInstance->CreateServerWrapper(m_csdkLock, config, &result);
                m_modeType = OC_SERVER;
                break;

            case ModeType::Client:
                m_client = m_WrapperInstance->CreateClientWrapper(m_csdkLock, config, &result);
                m_modeType = OC_CLIENT;
                break;

            case ModeType::Both:
            case ModeType::Gateway:
                m_server = m_WrapperInstance->CreateServerWrapper(m_csdkLock, config, &result);
                m_client = m_WrapperInstance->CreateClientWrapper(m_csdkLock, config, &result);
                m_modeType = config.mode == ModeType::Gateway ? OC_GATEWAY : OC_CLIENT_SERVER;
                break;
        }

        if (OC_STACK_OK != result)
        {
            m_server.reset();
            m_client.reset();
        }
        return result;
    }

    OCPlatform_impl::OCPlatform_impl(const PlatformConfig& config)
     : m_cfg             { config },
       m_WrapperInstance { make_unique<WrapperFactory>() },
       m_csdkLock        { std::make_shared<std::recursive_mutex>() },
       m_startCount(0)
    {
        if (m_cfg.useLegacyCleanup)
        {
            start();
        }
    }

    OCPlatform_impl::~OCPlatform_impl(void)
    {
        if (m_cfg.useLegacyCleanup)
        {
            stop();
        }
    }

    OCStackResult OCPlatform_impl::setDefaultDeviceEntityHandler(EntityHandler entityHandler)
    {
        return checked_guard(m_server, &IServerWrapper::setDefaultDeviceEntityHandler,
                             entityHandler);
    }

    OCStackResult OCPlatform_impl::notifyAllObservers(OCResourceHandle resourceHandle,
                                                QualityOfService QoS)
    {
        return result_guard(OCNotifyAllObservers(resourceHandle,
                    static_cast<OCQualityOfService>(QoS)));
    }

    OCStackResult OCPlatform_impl::notifyAllObservers(OCResourceHandle resourceHandle)
    {
        return notifyAllObservers(resourceHandle, m_cfg.QoS);
    }

    OCStackResult OCPlatform_impl::notifyListOfObservers(OCResourceHandle resourceHandle,
                                       ObservationIds& observationIds,
                                       const std::shared_ptr<OCResourceResponse> pResponse)
    {
        return notifyListOfObservers(resourceHandle, observationIds, pResponse, m_cfg.QoS);
    }

    OCStackResult OCPlatform_impl::notifyListOfObservers(OCResourceHandle resourceHandle,
                                       ObservationIds& observationIds,
                                       const std::shared_ptr<OCResourceResponse> pResponse,
                                       QualityOfService QoS)
    {
        if(!pResponse || (observationIds.size() > UINT8_MAX))
        {
         return result_guard(OC_STACK_ERROR);
        }

        OCRepPayload* pl = pResponse->getResourceRepresentation().getPayload();
        OCStackResult result =
                   OCNotifyListOfObservers(resourceHandle,
                            &observationIds[0], (uint8_t)observationIds.size(),
                            pl,
                            static_cast<OCQualityOfService>(QoS));
        OCRepPayloadDestroy(pl);
        return result_guard(result);
    }

    OCResource::Ptr OCPlatform_impl::constructResourceObject(const std::string& host,
                                                const std::string& uri,
                                                OCConnectivityType connectivityType,
                                                bool isObservable,
                                                const std::vector<std::string>& resourceTypes,
                                                const std::vector<std::string>& interfaces)
    {
        if(!m_client)
        {
            return std::shared_ptr<OCResource>();
        }

        uint8_t resourceProperty = 0;
        if (isObservable)
        {
            resourceProperty = (resourceProperty | OC_OBSERVABLE);
        }
        return std::shared_ptr<OCResource>(new OCResource(m_client,
                                            host,
                                            uri, "", connectivityType,
                                            resourceProperty,
                                            resourceTypes,
                                            interfaces));
    }

    OCStackResult OCPlatform_impl::findResource(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindCallback resourceHandler)
    {
        return findResource(host, resourceName, connectivityType, resourceHandler, m_cfg.QoS);
    }

    OCStackResult OCPlatform_impl::findResource(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindCallback resourceHandler,
                                            QualityOfService QoS)
    {
        return checked_guard(m_client, &IClientWrapper::ListenForResource,
                             host, resourceName, connectivityType, resourceHandler, QoS);
    }

    OCStackResult OCPlatform_impl::findResource(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindCallback resourceHandler,
                                            FindErrorCallback errorHandler)
    {
        return findResource(host, resourceName, connectivityType, resourceHandler,
                            errorHandler, m_cfg.QoS);
    }

    OCStackResult OCPlatform_impl::findResource(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindCallback resourceHandler,
                                            FindErrorCallback errorHandler,
                                            QualityOfService QoS)
    {
        return checked_guard(m_client, &IClientWrapper::ListenErrorForResource,
                             host, resourceName, connectivityType, resourceHandler,
                             errorHandler, QoS);
    }

    OCStackResult OCPlatform_impl::findResourceList(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindResListCallback resourceHandler,
                                            QualityOfService QoS)
    {
        return checked_guard(m_client, &IClientWrapper::ListenForResourceList,
                             host, resourceName, connectivityType, resourceHandler, QoS);
    }

    OCStackResult OCPlatform_impl::findResourceList(const std::string& host,
                                            const std::string& resourceName,
                                            OCConnectivityType connectivityType,
                                            FindResListCallback resourceHandler,
                                            FindErrorCallback errorHandler,
                                            QualityOfService Qos)
    {
        return checked_guard(m_client, &IClientWrapper::ListenForResourceListWithError,
                             host, resourceName, connectivityType, resourceHandler,
                             errorHandler, Qos);
    }

    OCStackResult OCPlatform_impl::getDeviceInfo(const std::string& host,
                                            const std::string& deviceURI,
                                            OCConnectivityType connectivityType,
                                            FindDeviceCallback deviceInfoHandler)
    {
        return result_guard(getDeviceInfo(host, deviceURI, connectivityType,
               deviceInfoHandler, m_cfg.QoS));
    }

    OCStackResult OCPlatform_impl::getDeviceInfo(const std::string& host,
                                            const std::string& deviceURI,
                                            OCConnectivityType connectivityType,
                                            FindDeviceCallback deviceInfoHandler,
                                            QualityOfService QoS)
    {
        return checked_guard(m_client, &IClientWrapper::ListenForDevice,
                             host, deviceURI, connectivityType, deviceInfoHandler, QoS);
    }

    OCStackResult OCPlatform_impl::getPlatformInfo(const std::string& host,
                                            const std::string& platformURI,
                                            OCConnectivityType connectivityType,
                                            FindPlatformCallback platformInfoHandler)
    {
        return result_guard(getPlatformInfo(host, platformURI, connectivityType,
               platformInfoHandler, m_cfg.QoS));
    }

    OCStackResult OCPlatform_impl::getPlatformInfo(const std::string& host,
                                            const std::string& platformURI,
                                            OCConnectivityType connectivityType,
                                            FindPlatformCallback platformInfoHandler,
                                            QualityOfService QoS)
    {
        return checked_guard(m_client, &IClientWrapper::ListenForDevice,
                             host, platformURI, connectivityType, platformInfoHandler, QoS);
    }

    OCStackResult OCPlatform_impl::getSupportedTransportsInfo(OCTpsSchemeFlags& supportedTps)
    {
        return checked_guard(m_server, &IServerWrapper::getSupportedTransportsInfo, supportedTps);
    }

    OCStackResult OCPlatform_impl::registerResource(OCResourceHandle& resourceHandle,
                                            std::string& resourceURI,
                                            const std::string& resourceTypeName,
                                            const std::string& resourceInterface,
                                            EntityHandler entityHandler,
                                            uint8_t resourceProperty)
    {
        return checked_guard(m_server, &IServerWrapper::registerResource,
                             std::ref(resourceHandle), resourceURI, resourceTypeName,
                             resourceInterface, entityHandler, resourceProperty);
    }

    OCStackResult OCPlatform_impl::registerResource(OCResourceHandle& resourceHandle,
                                            std::string& resourceURI,
                                            const std::string& resourceTypeName,
                                            const std::string& resourceInterface,
                                            EntityHandler entityHandler,
                                            uint8_t resourceProperty,
                                            OCTpsSchemeFlags resourceTpsTypes)
    {
        return checked_guard(m_server, &IServerWrapper::registerResourceWithTps,
                             std::ref(resourceHandle), resourceURI, resourceTypeName,
                             resourceInterface, entityHandler, resourceProperty,
                             resourceTpsTypes);
    }

    OCStackResult OCPlatform_impl::registerDeviceInfo(const OCDeviceInfo deviceInfo)
    {
        return checked_guard(m_server, &IServerWrapper::registerDeviceInfo, deviceInfo);
    }

    OCStackResult OCPlatform_impl::registerPlatformInfo(const OCPlatformInfo platformInfo)
    {
        return checked_guard(m_server, &IServerWrapper::registerPlatformInfo, platformInfo);
    }

    OCStackResult OCPlatform_impl::setPropertyValue(OCPayloadType type, const std::string& tag,
                                                    const std::string& value)
    {
        return checked_guard(m_server, &IServerWrapper::setPropertyValue, type, tag, value);
    }

    OCStackResult OCPlatform_impl::setPropertyValue(OCPayloadType type, const std::string& tag,
                                                    const std::vector<std::string>& value)
    {
        std::string concatString = "";
        for (const auto& h : value)
        {
            if (std::string::npos == h.find(","))
            {
                concatString += h + ",";
            }
            else
            {
                return OC_STACK_INVALID_PARAM;
            }
        }

        return checked_guard(m_server, &IServerWrapper::setPropertyValue, type, tag, concatString);
    }

    OCStackResult OCPlatform_impl::getPropertyValue(OCPayloadType type, const std::string& tag,
                                                    std::string& value)
    {
        return checked_guard(m_server, &IServerWrapper::getPropertyValue, type, tag, value);
    }

    OCStackResult OCPlatform_impl::getPropertyList(OCPayloadType type, const std::string& tag,
                                                    std::vector<std::string>& value)
    {
        return checked_guard(m_server, &IServerWrapper::getPropertyList, type, tag, value);
    }

    OCStackResult OCPlatform_impl::registerResource(OCResourceHandle& resourceHandle,
                                            const std::shared_ptr< OCResource > resource)
    {
        uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
        std::vector<std::string> resourceTypes = resource->getResourceTypes();

        return checked_guard(m_server, &IServerWrapper::registerResource,
                std::ref(resourceHandle), resource->host() + resource->uri(),
                resourceTypes[0]/*"core.remote"*/, DEFAULT_INTERFACE,
                (EntityHandler) nullptr, resourceProperty);
    }

    OCStackResult OCPlatform_impl::unregisterResource(const OCResourceHandle& resourceHandle) const
    {
        return checked_guard(m_server, &IServerWrapper::unregisterResource,
                             resourceHandle);
    }

    OCStackResult OCPlatform_impl::unbindResource(OCResourceHandle collectionHandle,
                                            OCResourceHandle resourceHandle)
    {
        return result_guard(OCUnBindResource(std::ref(collectionHandle), std::ref(resourceHandle)));
    }

    OCStackResult OCPlatform_impl::unbindResources(const OCResourceHandle collectionHandle,
                                            const std::vector<OCResourceHandle>& resourceHandles)
    {
        for(const auto& h : resourceHandles)
        {
           OCStackResult r;

           if(OC_STACK_OK != (r = result_guard(OCUnBindResource(collectionHandle, h))))
           {
               return r;
           }
        }

        return OC_STACK_OK;
    }

    OCStackResult OCPlatform_impl::bindResource(const OCResourceHandle collectionHandle,
                                            const OCResourceHandle resourceHandle)
    {
        return result_guard(OCBindResource(collectionHandle, resourceHandle));
    }

    OCStackResult OCPlatform_impl::bindResources(const OCResourceHandle collectionHandle,
                                            const std::vector<OCResourceHandle>& resourceHandles)
    {
        for(const auto& h : resourceHandles)
        {
           OCStackResult r;

           if(OC_STACK_OK != (r = result_guard(OCBindResource(collectionHandle, h))))
           {
               return r;
           }
        }

        return OC_STACK_OK;
    }

    OCStackResult OCPlatform_impl::bindTypeToResource(const OCResourceHandle& resourceHandle,
                                             const std::string& resourceTypeName) const
    {
        return checked_guard(m_server, &IServerWrapper::bindTypeToResource,
                             resourceHandle, resourceTypeName);
    }

    OCStackResult OCPlatform_impl::bindInterfaceToResource(const OCResourceHandle& resourceHandle,
                                             const std::string& resourceInterfaceName) const
    {
        return checked_guard(m_server, &IServerWrapper::bindInterfaceToResource,
                             resourceHandle, resourceInterfaceName);
    }

    OCStackResult OCPlatform_impl::startPresence(const unsigned int announceDurationSeconds)
    {
        return checked_guard(m_server, &IServerWrapper::startPresence,
                             announceDurationSeconds);
    }

    OCStackResult OCPlatform_impl::stopPresence()
    {
        return checked_guard(m_server, &IServerWrapper::stopPresence);
    }

    OCStackResult OCPlatform_impl::subscribePresence(OCPresenceHandle& presenceHandle,
                                            const std::string& host,
                                            OCConnectivityType connectivityType,
                                            SubscribeCallback presenceHandler)
    {
        return subscribePresence(presenceHandle, host, "", connectivityType, presenceHandler);
    }


    OCStackResult OCPlatform_impl::subscribePresence(OCPresenceHandle& presenceHandle,
                                            const std::string& host,
                                            const std::string& resourceType,
                                            OCConnectivityType connectivityType,
                                            SubscribeCallback presenceHandler)
    {
        return checked_guard(m_client, &IClientWrapper::SubscribePresence,
                             &presenceHandle, host, resourceType, connectivityType,
                             presenceHandler);
    }

    OCStackResult OCPlatform_impl::unsubscribePresence(OCPresenceHandle presenceHandle)
    {
        return checked_guard(m_client, &IClientWrapper::UnsubscribePresence,
                             std::ref(presenceHandle));
    }

#ifdef WITH_CLOUD
    OCStackResult OCPlatform_impl::subscribeDevicePresence(OCPresenceHandle& presenceHandle,
                                                           const std::string& host,
                                                           const std::vector<std::string>& di,
                                                           OCConnectivityType connectivityType,
                                                           ObserveCallback callback)
    {
        return checked_guard(m_client, &IClientWrapper::SubscribeDevicePresence,
                             &presenceHandle, host, di, connectivityType, callback);
    }
#endif

    OCStackResult OCPlatform_impl::sendResponse(const std::shared_ptr<OCResourceResponse> pResponse)
    {
        return checked_guard(m_server, &IServerWrapper::sendResponse,
                             pResponse);
    }
    std::weak_ptr<std::recursive_mutex> OCPlatform_impl::csdkLock()
    {
        return m_csdkLock;
    }

#ifdef WITH_CLOUD
    OCAccountManager::Ptr OCPlatform_impl::constructAccountManagerObject(const std::string& host,
                                                            OCConnectivityType connectivityType)
    {
        if (!m_client)
        {
            return std::shared_ptr<OCAccountManager>();
        }

        return std::shared_ptr<OCAccountManager>(new OCAccountManager(m_client,
                                                                      host,
                                                                      connectivityType));
    }
#endif // WITH_CLOUD

    OCStackResult OCPlatform_impl::getDeviceId(OCUUIdentity *myUuid)
    {
        return OCGetDeviceId(myUuid);
    }

    OCStackResult OCPlatform_impl::setDeviceId(const OCUUIdentity *myUuid)
    {
        return OCSetDeviceId(myUuid);
    }
} //namespace OC
