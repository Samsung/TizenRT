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

#include <StringConstants.h>
#include <sstream>
#include <cstring>
#include "ocpayload.h"
#include "ocrandom.h"
#include "oic_string.h"

namespace OC
{
    static const char EP_ADDR_SPLIT[]   = "://";
    static const char EP_PORT_SPLIT[]   = ":";
    static const char EP_BRAKET_START[] = "[";
    static const char EP_BRAKET_END[]   = "]";

    class ListenOCContainer
    {
        private:
            static std::vector<std::string> StringLLToVector(OCStringLL* ll)
            {
                std::vector<std::string> strs;
                while(ll)
                {
                    strs.push_back(ll->value);
                    ll = ll->next;
                }
                return strs;
            }

            static std::vector<std::string> EpsLLToVector(OCEndpointPayload* head)
            {
                std::vector<std::string> strs;
                while (head)
                {
                    std::ostringstream endpoint;
                    endpoint << head->tps << EP_ADDR_SPLIT;

                    if (OC_DEFAULT_FLAGS == head->family)
                    {
                        // mac
                        endpoint << head->addr;
                    }
                    else if (head->family & OC_IP_USE_V4)
                    {
                        endpoint << head->addr << EP_PORT_SPLIT << head->port;
                    }
                    else if (head->family & OC_IP_USE_V6)
                    {
                        endpoint << EP_BRAKET_START << head->addr << EP_BRAKET_END
                                 << EP_PORT_SPLIT << head->port;
                    }
                    else
                    {
                        head = head->next;
                        continue;
                    }

                    strs.push_back(endpoint.str());
                    head = head->next;
                }
                return strs;
            }

        public:
            ListenOCContainer(std::weak_ptr<IClientWrapper> cw,
                    OCDevAddr& devAddr,
                    HeaderOptions serverHeaderOption, OCDiscoveryPayload* payload)
            {
                OCDevAddr currentDevAddr = devAddr;
                while (payload)
                {
                    OCResourcePayload* res = payload->resources;
                    while (res)
                    {

                        currentDevAddr.flags = res->secure ?
                                (OCTransportFlags)(OC_FLAG_SECURE | devAddr.flags) :
                                devAddr.flags;

                        currentDevAddr.port = (res->port != 0) ? res->port : devAddr.port;

                        OCEndpointPayload* eps = res->eps;
                        std::vector<std::string> epsVector;
                        if (eps)
                        {
                            //parsing eps from payload
                            epsVector = EpsLLToVector(eps);
                        }

                        m_resources.push_back(std::shared_ptr<OC::OCResource>(
                                new OC::OCResource(cw, currentDevAddr,
                                    std::string(res->uri),
                                    std::string(payload->sid),
                                    res->bitmap,
                                    StringLLToVector(res->types),
                                    StringLLToVector(res->interfaces),
                                    epsVector,
                                    serverHeaderOption
                                    )));

#ifdef TCP_ADAPTER
                        if (res->tcpPort != 0)
                        {
                            OCDevAddr tcpDevAddr = currentDevAddr;
                            tcpDevAddr.port = res->tcpPort;
                            tcpDevAddr.adapter = OC_ADAPTER_TCP;
                            m_resources.push_back(std::shared_ptr<OC::OCResource>(
                                        new OC::OCResource(cw, tcpDevAddr,
                                            std::string(res->uri),
                                            std::string(payload->sid),
                                            res->bitmap,
                                            StringLLToVector(res->types),
                                            StringLLToVector(res->interfaces),
                                            epsVector,
                                            serverHeaderOption
                                            )));
                        }
#endif
                        res = res->next;
                    }
                    payload = payload->next;
                }
            }

#ifdef WITH_MQ
            ListenOCContainer(std::weak_ptr<IClientWrapper> cw,
                                OCDevAddr& devAddr, OCRepPayload* payload)
            {
                if (payload)
                {
                    char**topicList = nullptr;
                    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
                    OCRepPayloadGetStringArray(payload, "topiclist", &topicList, dimensions);

                    for(size_t idx = 0; idx < dimensions[0]; idx++)
                    {
                        m_resources.push_back(std::shared_ptr<OC::OCResource>(
                                new OC::OCResource(cw, devAddr,
                                                   std::string(topicList[idx]),
                                                   "",
                                                   OC_OBSERVABLE,
                                                   {OC_RSRVD_RESOURCE_TYPE_MQ_TOPIC},
                                                   {DEFAULT_INTERFACE})));
                    }
                }
            }

            ListenOCContainer(std::weak_ptr<IClientWrapper> cw,
                              OCDevAddr& devAddr, const std::string& topicUri)
            {
                    m_resources.push_back(std::shared_ptr<OC::OCResource>(
                            new OC::OCResource(cw, devAddr,
                                               topicUri,
                                               "",
                                               OC_OBSERVABLE,
                                               {OC_RSRVD_RESOURCE_TYPE_MQ_TOPIC},
                                               {DEFAULT_INTERFACE})));
            }
#endif

            const std::vector<std::shared_ptr<OCResource>>& Resources() const
            {
                return m_resources;
            }
        private:
            std::vector<std::shared_ptr<OC::OCResource>> m_resources;
    };
}
