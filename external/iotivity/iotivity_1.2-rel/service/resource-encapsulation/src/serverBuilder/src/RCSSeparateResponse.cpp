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

#include "RCSSeparateResponse.h"

#include "RCSRequest.h"
#include "RCSResourceObject.h"
#include "RCSRepresentation.h"
#include "AssertUtils.h"

#include "OCPlatform.h"
#include "OCResourceResponse.h"
#include "OCResourceRequest.h"

namespace OIC
{
    namespace Service
    {

        namespace
        {
            void validateRequest(const RCSRequest& request)
            {
                if (!request.getOCRequest() || request.getResourceObject().expired())
                {
                    throw RCSInvalidParameterException{
                        "The request is incomplete. The resource for the request might be destroyed." };
                }
            }
        }

        RCSSeparateResponse::RCSSeparateResponse(const RCSRequest& request) :
                m_request{ request },
                m_done{ false }
        {
            validateRequest(m_request);
        }

        RCSSeparateResponse::RCSSeparateResponse(RCSRequest&& request) :
                m_request{ std::move(request) },
                m_done{ false }
        {
            validateRequest(m_request);
        }

        void RCSSeparateResponse::set()
        {
            if (!m_request.getOCRequest())
            {
                throw RCSBadRequestException{ "The state of this object is invalid!" };
            }

            auto resObj = m_request.getResourceObject().lock();
            if (!resObj)
            {
                throw RCSBadRequestException{ "ResourceObject is unspecified(or destroyed)!" };
            }

            if (m_done) throw RCSBadRequestException{ "The response is already set!" };

            auto ocRequest = m_request.getOCRequest();
            auto response = std::make_shared< OC::OCResourceResponse >();

            response->setRequestHandle(ocRequest->getRequestHandle());
            response->setResourceHandle(ocRequest->getResourceHandle());

            response->setResponseResult(OC_EH_OK);

            response->setResourceRepresentation(
                    RCSRepresentation::toOCRepresentation(resObj->getRepresentation(m_request)));

            invokeOCFunc(OC::OCPlatform::sendResponse, response);

            m_done = true;
        }

    }
}
