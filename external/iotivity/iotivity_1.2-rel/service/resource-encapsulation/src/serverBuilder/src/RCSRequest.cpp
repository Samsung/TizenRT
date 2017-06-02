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

#include "RCSRequest.h"

#include "OCResourceRequest.h"

namespace OIC
{
    namespace Service
    {

        RCSRequest::RCSRequest(const std::shared_ptr< RCSResourceObject >& resourceObject,
                const std::shared_ptr< OC::OCResourceRequest >& ocRequest) :
                m_resourceObject{ resourceObject },
                m_ocRequest{ ocRequest }
        {
        }

        RCSRequest::RCSRequest(const std::string& resourceUri) :
                m_ocRequest{ std::make_shared< OC::OCResourceRequest >() }
        {
            m_ocRequest->setResourceUri(resourceUri);
        }


        std::weak_ptr< RCSResourceObject > RCSRequest::getResourceObject() const noexcept
        {
            return m_resourceObject;
        }

        std::string RCSRequest::getResourceUri() const
        {
            return m_ocRequest->getResourceUri();
        }

        std::shared_ptr< OC::OCResourceRequest > RCSRequest::getOCRequest() const noexcept
        {
            return m_ocRequest;
        }

        const std::map< std::string, std::string >& RCSRequest::getQueryParams() const
        {
            return m_ocRequest->getQueryParameters();
        }

        std::string RCSRequest::getInterface() const
        {
            const auto& params = m_ocRequest->getQueryParameters();

            auto it = params.find(OC::Key::INTERFACESKEY);

            if (it == params.end()) return "";

            return it->second;
        }

    }
}
