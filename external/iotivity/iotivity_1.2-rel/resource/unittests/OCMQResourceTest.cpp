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

    //Helper method
    OCResource::Ptr ConstructResourceObject(std::string host, std::string uri)
    {
        OCConnectivityType connectivityType = CT_DEFAULT;
        std::vector<std::string> types = {"oic.ps"};
        std::vector<std::string> ifaces = {DEFAULT_INTERFACE};

        auto ret = OCPlatform::constructResourceObject(host, uri,
                connectivityType, false, types, ifaces);

        if (!ret)
        {
            ADD_FAILURE() << "ConstructResourceObject result was null";
            return nullptr;
        }

        return ret;
    }

    // Message Queue Test
    TEST(MessageQueueTest, DiscoveryMQTopicsValid)
    {
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/resource");
        EXPECT_TRUE(resource != NULL);
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->discoveryMQTopics(query, &foundResource,
                                                           QualityOfService::LowQos));
    }

    TEST(MessageQueueTest, CreateMQTopicValid)
    {
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/resource");
        EXPECT_TRUE(resource != NULL);
        OCRepresentation rep;
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->createMQTopic(rep, "/lightTopic", query, &createdTopic,
                                                       QualityOfService::LowQos));
    }

#ifdef MQ_PUBLISHER
    TEST(MessageQueueTest, PublishMQTopicValid)
    {
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/resource");
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
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/resource");
        EXPECT_TRUE(resource != NULL);
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->subscribeMQTopic(ObserveType::Observe, query,
                                                          &onSubscribe, QualityOfService::LowQos));
    }

    TEST(MessageQueueTest, RequestMQPublishValid)
    {
        OCResource::Ptr resource = ConstructResourceObject("coap://192.168.1.2:5000", "/resource");
        EXPECT_TRUE(resource != NULL);
        QueryParamsMap query = {};
        EXPECT_EQ(OC_STACK_OK, resource->requestMQPublish(query, &onReqPub,
                                                          QualityOfService::LowQos));
    }
#endif
}

