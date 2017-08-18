//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "InterfaceHandler.h"

#include "OCResourceRequest.h"

#include "RCSRepresentation.h"
#include "RCSRequest.h"
#include "RCSResourceObject.h"
#include "RCSResourceAttributes.h"
#include "ResourceAttributesConverter.h"

#include <unordered_map>

namespace
{
    using namespace OIC::Service;

    RCSRepresentation toRepresentation(const RCSResourceObject& resource)
    {
        RCSResourceObject::LockGuard lock{ resource, RCSResourceObject::AutoNotifyPolicy::NEVER };

        return RCSRepresentation{ resource.getUri(), resource.getInterfaces(), resource.getTypes(),
            resource.getAttributes() };
    }

    RCSRepresentation buildGetBaselineResponse(const RCSRequest&, const RCSResourceObject& resource)
    {
        return toRepresentation(resource);
    }

    RCSRepresentation buildSetBaselineResponse(const RCSRequest& rcsRequest,
            const RCSResourceObject& resource)
    {
        return buildGetBaselineResponse(rcsRequest, resource);
    }

    RCSRepresentation buildGetRequestResponse(const RCSRequest&, const RCSResourceObject& resource)
    {
        RCSResourceObject::LockGuard lock{ resource, RCSResourceObject::AutoNotifyPolicy::NEVER };

        return RCSRepresentation{ resource.getAttributes() };
    }

    RCSRepresentation buildSetRequestResponse(const RCSRequest& rcsRequest,
            const RCSResourceObject& resource)
    {
        auto requestAttr = ResourceAttributesConverter::fromOCRepresentation(
                rcsRequest.getOCRequest()->getResourceRepresentation());

        RCSResourceObject::LockGuard lock{ resource, RCSResourceObject::AutoNotifyPolicy::NEVER };

        const RCSResourceAttributes& updatedAttr = resource.getAttributes();

        for (auto it = requestAttr.begin(); it != requestAttr.end();)
        {
            if(updatedAttr.contains(it->key()))
            {
                it->value() = updatedAttr.at(it->key());
                ++it;
            }
            else
            {
                it = requestAttr.erase(it);
            }
        }

        return RCSRepresentation{ requestAttr };
    }

    RCSRepresentation buildGetBatchResponse(RCSRequest, const RCSResourceObject& resource)
    {
        RCSRepresentation rcsRep;

        RCSResourceObject::LockGuard lock{ resource, RCSResourceObject::AutoNotifyPolicy::NEVER };
        rcsRep.setAttributes(resource.getAttributes());

        for (const auto& bound : resource.getBoundResources())
        {
            rcsRep.addChild(toRepresentation(*bound));
        }

        return rcsRep;
    }

    const std::unordered_map< std::string, InterfaceHandler > g_defaultHandlers {

            { BASELINE_INTERFACE,
                InterfaceHandler(buildGetBaselineResponse, buildSetBaselineResponse) },

            { ACTUATOR_INTERFACE,
                InterfaceHandler(buildGetRequestResponse, buildSetRequestResponse) },

            { SENSOR_INTERFACE,
                InterfaceHandler(buildGetRequestResponse, nullptr) },

            { BATCH_INTERFACE,
                InterfaceHandler(buildGetBatchResponse, buildSetBaselineResponse) }
    };
}

namespace OIC
{
    namespace Service
    {

        InterfaceHandler::InterfaceHandler(GetResponseBuilder getBuilder,
                SetResponseBuilder setBuilder) :
                m_getBuilder{ std::move(getBuilder) },
                m_setBuilder{ std::move(setBuilder) }
        {
        }

        bool InterfaceHandler::isGetSupported() const
        {
            return m_getBuilder != nullptr;
        }

        bool InterfaceHandler::isSetSupported() const
        {
            return m_setBuilder != nullptr;
        }

        InterfaceHandler::GetResponseBuilder InterfaceHandler::getGetResponseBuilder() const
        {
            return m_getBuilder;
        }

        InterfaceHandler::SetResponseBuilder InterfaceHandler::getSetResponseBuilder() const
        {
            return m_setBuilder;
        }

        InterfaceHandler getDefaultInterfaceHandler(const std::string& interfaceName,
                const std::string& defaultInterfaceName)
        {
            auto it = g_defaultHandlers.find(interfaceName);
            if (it != g_defaultHandlers.end()) return it->second;

            it = g_defaultHandlers.find(defaultInterfaceName);
            if (it != g_defaultHandlers.end()) return it->second;

            return g_defaultHandlers.find(OIC::Service::BASELINE_INTERFACE)->second;
        }
    }
}
