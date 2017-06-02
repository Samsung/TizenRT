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

#include "RequestHandler.h"

#include "ResourceAttributesConverter.h"
#include "RCSResourceObject.h"
#include "ResourceAttributesUtils.h"

namespace
{
    using namespace OIC::Service;

    AttrKeyValuePairs applyAcceptMethod(RCSResourceObject& resource,
            const RCSResourceAttributes& requestAttrs)
    {
        RCSResourceObject::LockGuard lock(resource, RCSResourceObject::AutoNotifyPolicy::NEVER);

        return replaceAttributes(resource.getAttributes(), requestAttrs);
    }

    AttrKeyValuePairs applyDefaultMethod(RCSResourceObject& resource,
            const RCSResourceAttributes& requestAttrs)
    {
        RCSResourceObject::LockGuard lock(resource, RCSResourceObject::AutoNotifyPolicy::NEVER);

        if (resource.getSetRequestHandlerPolicy()
            != RCSResourceObject::SetRequestHandlerPolicy::ACCEPTANCE
            && !acceptableAttributes(resource.getAttributes(), requestAttrs))
        {
            return AttrKeyValuePairs{ };
        }

        return replaceAttributes(resource.getAttributes(), requestAttrs);
    }

    AttrKeyValuePairs applyIgnoreMethod(RCSResourceObject&, const RCSResourceAttributes&)
    {
        return AttrKeyValuePairs();
    }

    auto getApplyAcceptanceFunc(RCSSetResponse::AcceptanceMethod method) ->
            std::function<AttrKeyValuePairs(RCSResourceObject&, const RCSResourceAttributes&)>
    {
        switch (method)
        {
            case RCSSetResponse::AcceptanceMethod::DEFAULT:
                return applyDefaultMethod;

            case RCSSetResponse::AcceptanceMethod::ACCEPT:
                return applyAcceptMethod;

            case RCSSetResponse::AcceptanceMethod::IGNORE:
                return applyIgnoreMethod;
        }

        return applyIgnoreMethod;
    }

} // unnamed namespace

namespace OIC
{
    namespace Service
    {
        constexpr int RequestHandler::DEFAULT_ERROR_CODE;

        RequestHandler::RequestHandler() :
                m_errorCode{ DEFAULT_ERROR_CODE },
                m_customRep{ false },
                m_ocRep{ }
        {
        }

        RequestHandler::RequestHandler(int errorCode) :
                m_errorCode{ errorCode },
                m_customRep{ false },
                m_ocRep{ }

        {
        }

        RequestHandler::RequestHandler(const RCSResourceAttributes& attrs, int errorCode) :
                m_errorCode{ errorCode },
                m_customRep{ true },
                m_ocRep{ ResourceAttributesConverter::toOCRepresentation(attrs) }
        {
        }

        RequestHandler::RequestHandler(RCSResourceAttributes&& attrs, int errorCode) :
                m_errorCode{ errorCode },
                m_customRep{ true },
                m_ocRep{ ResourceAttributesConverter::toOCRepresentation(std::move(attrs)) }
        {
        }

        int RequestHandler::getErrorCode() const
        {
            return m_errorCode;
        }

        bool RequestHandler::hasCustomRepresentation() const
        {
            return m_customRep;
        }

        OC::OCRepresentation RequestHandler::getRepresentation() const
        {
            return m_ocRep;
        }

        SetRequestHandler::SetRequestHandler() :
                RequestHandler{ }
        {
        }

        SetRequestHandler::SetRequestHandler(int errorCode) :
                RequestHandler{ errorCode }
        {
        }


        SetRequestHandler::SetRequestHandler(const RCSResourceAttributes& attrs, int errorCode) :
                RequestHandler{ attrs, errorCode }
        {
        }

        SetRequestHandler::SetRequestHandler(RCSResourceAttributes&& attrs,  int errorCode) :
                RequestHandler{ std::move(attrs), errorCode }
        {
        }

        AttrKeyValuePairs SetRequestHandler::applyAcceptanceMethod(
                RCSSetResponse::AcceptanceMethod method, RCSResourceObject& resource,
                const RCSResourceAttributes& requestAttrs) const
        {
            return getApplyAcceptanceFunc(method)(resource, requestAttrs);
        }

    }
}
