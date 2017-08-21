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

using namespace OC;

    // OCPlatform_impl is friend class of OCResource.
    // Every OCResource object is created via this function.

    IClientWrapper::Ptr g_mockClientWrapper = std::shared_ptr<IClientWrapper>();
    extern PlatformConfig g_platformConfig;
    OCResource::Ptr OCPlatform_impl::constructResourceObject(
                                                 const std::string& host,
                                                 const std::string& uri,
                                                 OCConnectivityType connectivityType,
                                                 bool isObservable,
                                                 const std::vector<std::string>& resourceTypes,
                                                 const std::vector<std::string>& interfaces)
    {
        uint8_t resourceProperty = 0;

        if (isObservable)
        {
            resourceProperty = (resourceProperty | OC_OBSERVABLE);
        }

        return std::shared_ptr<OCResource>(new OCResource(
                                                 g_mockClientWrapper,
                                                 host,
                                                 uri,
                                                 "",
                                                 connectivityType,
                                                 resourceProperty,
                                                 resourceTypes,
                                                 interfaces));
    }

    OCPlatform_impl::OCPlatform_impl(const PlatformConfig& config) :
        m_cfg             { config },
        m_WrapperInstance { make_unique<WrapperFactory>() },
        m_csdkLock        { std::make_shared<std::recursive_mutex>() }
    {
    }

    OCPlatform_impl::~OCPlatform_impl(void)
    {
    }

    OCPlatform_impl& OCPlatform_impl::Instance()
    {
        static OCPlatform_impl platform(g_platformConfig);
        return platform;
    }

    OCResource::Ptr OCPlatform::constructResourceObject(const std::string& host,
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
