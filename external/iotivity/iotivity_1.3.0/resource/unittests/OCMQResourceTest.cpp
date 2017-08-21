/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
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


#include <OCPlatform.h>
#include <OCApi.h>
#include <gtest/gtest.h>
#include <string>
#include <map>

namespace OCMQResourceTest
{
    using namespace OC;

#ifdef MQ_SUBSCRIBER
    void onSubscribe(const HeaderOptions, const OCRepresentation&, const int&, const int&)
    {
    }

    void onReqPub(const HeaderOptions&, const OCRepresentation&, const int)
    {
    }
#endif

#ifdef MQ_PUBLISHER
    void onPublish(const HeaderOptions&, const OCRepresentation&, const int)
    {
    }
#endif

    void foundResource(const int, const std::string, std::shared_ptr<OCResource>)
    {
    }

    void createdTopic(const int, const std::string, std::shared_ptr<OCResource>)
    {
    }

    class Framework
        {
        public:
            Framework(ServiceType serviceType = OC::ServiceType::InProc,
                      ModeType mode = OC::ModeType::Server,
                      OCPersistentStorage *ps = nullptr)
                      : m_started(false)
            {
                PlatformConfig cfg(serviceType, mode, ps);
                OCPlatform::Configure(cfg);
            }
            ~Framework()
            {
                if (m_started)
                {
                    OC_VERIFY(OC_STACK_OK == OCPlatform::stop());
                    m_started = false;
                }
            }
            OCStackResult start()
            {
                OCStackResult result = OCPlatform::start();
                if (OC_STACK_OK == result)
                {
                    m_started = true;
                }
                return result;
            }

        private:
            bool m_started;
        };

    //Helper method
    OCResource::Ptr ConstructResourceObject(std::string host, std::string uri)
    {
        auto ret = OCPlatform::constructResourceObject(host, uri,
                   static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4), false,
                   { std::string("oic.wk.ps") }, { std::string(DEFAULT_INTERFACE) });

        if(!ret)
        {
            ADD_FAILURE() << "ConstructResourceObject result was null";
            throw std::runtime_error("ConstructResourceObject result was null");
        }

        return ret;
    }

    // Message Queue Test
    TEST(MessageQueueTest, DiscoveryMQTopicsValid)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Client, NULL);
        ASSERT_TRUE(OC_STACK_OK == framework.start());
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/oic/ps");
        EXPECT_TRUE(resource != NULL);
        QueryParamsMap query;
        EXPECT_EQ(OC_STACK_OK, resource->discoveryMQTopics(query, &foundResource, QualityOfService::LowQos));
    }

    TEST(MessageQueueTest, CreateMQTopicValid)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Client, NULL);
        ASSERT_TRUE(OC_STACK_OK == framework.start());
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/oic/ps");
        EXPECT_TRUE(resource != NULL);
        OCRepresentation rep;
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->createMQTopic(rep, "/lightTopic", query, &createdTopic,
                                                       QualityOfService::LowQos));
    }

#ifdef MQ_PUBLISHER
    TEST(MessageQueueTest, PublishMQTopicValid)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Client, NULL);
        ASSERT_TRUE(OC_STACK_OK == framework.start());
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/oic/ps");
        EXPECT_TRUE(resource != NULL);
        OCRepresentation rep;
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->publishMQTopic(rep, query, &onPublish,
                                                        QualityOfService::LowQos));
    }
#endif

#ifdef MQ_SUBSCRIBER
    TEST(MessageQueueTest, SubscribeMQTopicValid)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Client, NULL);
        ASSERT_TRUE(OC_STACK_OK == framework.start());
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/oic/ps");
        EXPECT_TRUE(resource != NULL);
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->subscribeMQTopic(ObserveType::Observe, query,
                                                          &onSubscribe, QualityOfService::LowQos));
    }

    TEST(MessageQueueTest, RequestMQPublishValid)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Client, NULL);
        ASSERT_TRUE(OC_STACK_OK == framework.start());
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/oic/ps");
        EXPECT_TRUE(resource != NULL);
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->requestMQPublish(query, &onReqPub,
                                                          QualityOfService::LowQos));
    }
#endif
}

