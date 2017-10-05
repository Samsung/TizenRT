//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef _NS_CONSUMER_SERVICE_SIMULATOR_H_
#define _NS_CONSUMER_SERVICE_SIMULATOR_H_

#include <iostream>

#include "OCPlatform.h"
#include "OCApi.h"

class NSConsumerSimulator
{
    private:
        std::function<void(const int &, const std::string &, const std::string &,
                           const std::string &)> m_messageFunc;
        std::function<void(const int &, const int &)> m_syncFunc;

        std::shared_ptr<OC::OCResource> m_syncResource;
        std::shared_ptr<OC::OCResource> m_msgResource;
        std::shared_ptr<OC::OCResource> m_topicResource;

    public:
        NSConsumerSimulator()
            : m_messageFunc(), m_syncFunc(),
              m_syncResource()
        {
        }
        ~NSConsumerSimulator() = default;

        NSConsumerSimulator(const NSConsumerSimulator &) = delete;
        NSConsumerSimulator &operator = (const NSConsumerSimulator &) = delete;

        NSConsumerSimulator(NSConsumerSimulator &&) = delete;
        NSConsumerSimulator &operator = (NSConsumerSimulator &&) = delete;

        void findProvider()
        {
            OC::OCPlatform::findResource("", std::string("/oic/res?rt=x.org.iotivity.notification"),
                                         OCConnectivityType::CT_DEFAULT,
                                         std::bind(&NSConsumerSimulator::findResultCallback, this, std::placeholders::_1),
                                         OC::QualityOfService::LowQos);
        }

        void syncToProvider(int &type, const int &id, const std::string &providerID)
        {
            if (m_syncResource == nullptr)
            {
                return;
            }

            OC::OCRepresentation rep;
            rep.setValue("x.org.iotivity.ns.providerid", providerID);
            rep.setValue("x.org.iotivity.ns.messageid", id);
            rep.setValue("x.org.iotivity.ns.state", type);

            m_syncResource->post(rep, OC::QueryParamsMap(), &onPost, OC::QualityOfService::LowQos);
        }

        bool cancelObserves()
        {
            if ((msgResourceCancelObserve(OC::QualityOfService::HighQos) == OC_STACK_OK) &&
                (syncResourceCancelObserve(OC::QualityOfService::HighQos) == OC_STACK_OK))
            {
                return true;
            }
            return false;
        }

        void setCallback(std::function<void(const int &, const std::string &,
                                            const std::string &, const std::string &)> messageFunc,
                         const std::function<void(const int &, const int &)> &syncFunc)
        {
            m_messageFunc = messageFunc;
            m_syncFunc = syncFunc;
        }

    private:
        static void onPost(const OC::HeaderOptions &/*headerOption*/,
                           const OC::OCRepresentation & /*rep*/ , const int /*eCode*/)
        {
        }
        void findResultCallback(std::shared_ptr<OC::OCResource> resource)
        {


            if (resource->uri() == "/notification")
            {
                resource->get(OC::QueryParamsMap(),
                              std::bind(&NSConsumerSimulator::onGet, this,
                                        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                        resource), OC::QualityOfService::LowQos);
            }
        }
        void onGet(const OC::HeaderOptions &/*headerOption*/,
                   const OC::OCRepresentation & /*rep*/ , const int /*eCode*/,
                   std::shared_ptr<OC::OCResource> resource)
        {

            OC::QueryParamsMap map;
            map.insert(std::pair<std::string, std::string>(std::string("x.org.iotivity.ns.consumerid"),
                       std::string("123456789012345678901234567890123456")));

            try
            {

                std::vector<std::string> rts{"x.org.iotivity.notification"};

                m_msgResource
                    = OC::OCPlatform::constructResourceObject(
                          std::string(resource->host()), std::string(resource->uri() + "/message"),
                          OCConnectivityType(resource->connectivityType()), true, rts,
                          std::vector<std::string>(resource->getResourceInterfaces()));

                m_msgResource->observe(OC::ObserveType::Observe, map,
                                       std::bind(&NSConsumerSimulator::onObserve, this,
                                                 std::placeholders::_1, std::placeholders::_2,
                                                 std::placeholders::_3, std::placeholders::_4, resource),
                                       OC::QualityOfService::LowQos);
            }
            catch (std::exception &e)
            {
                std::cout << "OC::ResoureInitException : " << e.what() << std::endl;
            }
            m_syncResource
                = OC::OCPlatform::constructResourceObject(resource->host(), resource->uri() + "/sync",
                        resource->connectivityType(), true, resource->getResourceTypes(),
                        resource->getResourceInterfaces());

            m_syncResource->observe(OC::ObserveType::Observe, map,
                                    std::bind(&NSConsumerSimulator::onObserve, this,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::placeholders::_3, std::placeholders::_4, resource),
                                    OC::QualityOfService::LowQos);

            m_topicResource
                = OC::OCPlatform::constructResourceObject(resource->host(), resource->uri() + "/topic",
                        resource->connectivityType(), true, resource->getResourceTypes(),
                        resource->getResourceInterfaces());
        }
        void onObserve(const OC::HeaderOptions &/*headerOption*/,
                       const OC::OCRepresentation &rep , const int & /*eCode*/, const int &,
                       std::shared_ptr<OC::OCResource> )
        {

            if (rep.getUri() == "/notification/message" && rep.hasAttribute("x.org.iotivity.ns.messageid")
                && rep.getValue<int>("x.org.iotivity.ns.messageid") != 1)
            {
                m_messageFunc(int(rep.getValue<int>("x.org.iotivity.ns.messageid")),
                              std::string(rep.getValueToString("x.org.iotivity.ns.title")),
                              std::string(rep.getValueToString("x.org.iotivity.ns.contenttext")),
                              std::string(rep.getValueToString(".x.org.iotivity.ns.source")));
                if (rep.getValue<int>("x.org.iotivity.ns.messageid") == 3)
                {
                    m_topicResource->get(OC::QueryParamsMap(),
                                         std::bind(&NSConsumerSimulator::onTopicGet, this, std::placeholders::_1,
                                                   std::placeholders::_2, std::placeholders::_3, m_topicResource),
                                         OC::QualityOfService::LowQos);
                }
            }
            else if (rep.getUri() == "/notification/sync")
            {
                m_syncFunc(int(rep.getValue<int>("x.org.iotivity.ns.state")), int(rep.getValue<int>("x.org.iotivity.ns.messageid")));
            }
        }
        void onTopicGet(const OC::HeaderOptions &/*headerOption*/,
                        const OC::OCRepresentation & /*rep*/ , const int /*eCode*/,
                        std::shared_ptr<OC::OCResource> /*resource*/)
        {
        }

        OCStackResult msgResourceCancelObserve(OC::QualityOfService qos)
        {
            return m_msgResource->cancelObserve(qos);
        }

        OCStackResult syncResourceCancelObserve(OC::QualityOfService qos)
        {
            return m_syncResource->cancelObserve(qos);
        }
};


#endif //_NS_CONSUMER_SERVICE_SIMULATOR_H_
