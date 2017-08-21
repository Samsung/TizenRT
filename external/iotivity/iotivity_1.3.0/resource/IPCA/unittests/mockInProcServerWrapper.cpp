/* *****************************************************************
 *
 * Copyright 2017 Microsoft
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

#include "OCPlatform_impl.h"
#include <OCPlatform.h>
#include <OCResource.h>
#include <string>
#include "ipcatestdata.h"

namespace OC
{
    // This is mocked because OCPlatform_impl instantiates the client and the server wrappers.
    // There is no implementation in this file since the mock directly links the client and server
    // apps.
    InProcServerWrapper::InProcServerWrapper(
            std::weak_ptr<std::recursive_mutex> csdkLock, PlatformConfig cfg)
         : m_threadRun(false), m_csdkLock(csdkLock),
           m_cfg { cfg }
    {
    }

    InProcServerWrapper::~InProcServerWrapper()
    {
    }

    OCStackResult InProcServerWrapper::start()
    {
        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::stop()
    {
        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::registerResource(
                    OCResourceHandle& resourceHandle,
                    std::string& resourceURI,
                    const std::string& resourceTypeName,
                    const std::string& resourceInterface,
                    EntityHandler& eHandler,
                    uint8_t resourceProperties)
    {
        OC_UNUSED(resourceHandle);
        OC_UNUSED(resourceURI);
        OC_UNUSED(resourceTypeName);
        OC_UNUSED(resourceInterface);
        OC_UNUSED(eHandler);
        OC_UNUSED(resourceProperties);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::registerDeviceInfo(const OCDeviceInfo deviceInfo)
    {
        OC_UNUSED(deviceInfo);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::registerPlatformInfo(const OCPlatformInfo platformInfo)
    {
        OC_UNUSED(platformInfo);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::unregisterResource(const OCResourceHandle& resourceHandle)
    {
        OC_UNUSED(resourceHandle);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::bindTypeToResource(const OCResourceHandle& resourceHandle,
                     const std::string& resourceTypeName)
    {
        OC_UNUSED(resourceHandle);
        OC_UNUSED(resourceTypeName);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::bindInterfaceToResource(
                     const OCResourceHandle& resourceHandle,
                     const std::string& resourceInterfaceName)
    {
        OC_UNUSED(resourceHandle);
        OC_UNUSED(resourceInterfaceName);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::startPresence(const unsigned int seconds)
    {
        OC_UNUSED(seconds);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::stopPresence()
    {
        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::setDefaultDeviceEntityHandler
                                        (EntityHandler entityHandler)
    {
        OC_UNUSED(entityHandler);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::sendResponse(
        const std::shared_ptr<OCResourceResponse> pResponse)
    {
        OC_UNUSED(pResponse);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::setPropertyValue(OCPayloadType type,
        const std::string& propName,
        const std::string& propValue)
    {
        OC_UNUSED(type);
        OC_UNUSED(propName);
        OC_UNUSED(propValue);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::getPropertyValue(OCPayloadType type,
        const std::string& propName, std::string& propValue)
    {
        OC_UNUSED(type);
        OC_UNUSED(propName);
        OC_UNUSED(propValue);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::getPropertyList(OCPayloadType type,
        const std::string& propName, std::vector<std::string>& propValue)
    {
        OC_UNUSED(type);
        OC_UNUSED(propName);
        OC_UNUSED(propValue);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::registerResourceWithTps(
                    OCResourceHandle& resourceHandle,
                    std::string& resourceURI,
                    const std::string& resourceTypeName,
                    const std::string& resourceInterface,
                    EntityHandler& eHandler,
                    uint8_t resourceProperties,
                    OCTpsSchemeFlags resourceTpsTypes)
    {
        OC_UNUSED(resourceHandle);
        OC_UNUSED(resourceURI);
        OC_UNUSED(resourceTypeName);
        OC_UNUSED(resourceInterface);
        OC_UNUSED(eHandler);
        OC_UNUSED(resourceProperties);
        OC_UNUSED(resourceTpsTypes);

        return OC_STACK_OK;
    }

    OCStackResult InProcServerWrapper::getSupportedTransportsInfo(OCTpsSchemeFlags& supportedTps)
    {
        OC_UNUSED(supportedTps);

        return OC_STACK_OK;
    }
}
