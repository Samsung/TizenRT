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

#include <gtest/gtest.h>
#include <HippoMocks/hippomocks.h>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <chrono>

#include "NSProviderService.h"
#include "NSConsumerServiceSimulator.h"
#include "NSUtils.h"
#include "NSSyncInfo.h"
#include "NSMessage.h"
#include "NSMediaContents.h"

namespace
{
    std::atomic_bool g_isStartedStack(false);

    std::chrono::milliseconds g_waitForResponse(500);

    std::condition_variable responseCon;
    std::mutex mutexForCondition;

    NSConsumerSimulator g_consumerSimul;
    std::shared_ptr<OIC::Service::NSConsumer> g_consumer;
}

class TestWithMock: public testing::Test
{
    public:
        MockRepository mocks;

    protected:
        virtual ~TestWithMock() noexcept(noexcept(std::declval<Test>().~Test()))
        {
        }

        virtual void TearDown()
        {
            try
            {
                mocks.VerifyAll();
            }
            catch (...)
            {
                mocks.reset();
                throw;
            }
        }
};

class NotificationProviderServiceTest : public TestWithMock
{
    public:
        NotificationProviderServiceTest() = default;
        ~NotificationProviderServiceTest() = default;

        static void ConsumerSubscribedCallbackEmpty(std::shared_ptr<OIC::Service::NSConsumer> )
        {
            std::cout << __func__ << std::endl;
        }

        static void MessageSynchronizedCallbackEmpty(OIC::Service::NSSyncInfo)
        {
            std::cout << __func__ << std::endl;
        }

        static void MessageCallbackFromConsumerEmpty(
            const int &, const std::string &, const std::string &, const std::string &)
        {
            std::cout << __func__ << std::endl;
        }

        static void SyncCallbackFromConsumerEmpty(int, int)
        {
            std::cout << __func__ << std::endl;
        }

    protected:

        void SetUp()
        {
            TestWithMock::SetUp();

            if (g_isStartedStack == false)
            {
                OC::PlatformConfig cfg
                {
                    OC::ServiceType::InProc,
                    OC::ModeType::Both,
                    "0.0.0.0",
                    0,
                    OC::QualityOfService::HighQos
                };
                OC::OCPlatform::Configure(cfg);

                try
                {
                    OC::OCPlatform::stopPresence();
                }
                catch (...)
                {

                }

                g_isStartedStack = true;
            }

        }

        void TearDown()
        {
            TestWithMock::TearDown();
        }

};

TEST_F(NotificationProviderServiceTest, StartProviderPositiveWithPolicyTrue)
{
    OIC::Service::NSProviderService::ProviderConfig config;
    config.m_subscribeRequestCb = ConsumerSubscribedCallbackEmpty;
    config.m_syncInfoCb = MessageSynchronizedCallbackEmpty;
    config.subControllability = true;

    OIC::Service::NSResult ret =  OIC::Service::NSProviderService::getInstance()->start(config);

    EXPECT_EQ(ret,  OIC::Service::NSResult::OK);
}

TEST_F(NotificationProviderServiceTest, StopProviderPositive)
{
    OIC::Service::NSResult ret =  OIC::Service::NSProviderService::getInstance()->stop();

    EXPECT_EQ(ret,  OIC::Service::NSResult::OK);
}

TEST_F(NotificationProviderServiceTest, StartProviderPositiveWithPolicyFalse)
{
    OIC::Service::NSProviderService::ProviderConfig  config;
    config.m_subscribeRequestCb = ConsumerSubscribedCallbackEmpty;
    config.m_syncInfoCb = MessageSynchronizedCallbackEmpty;
    config.subControllability = false;

    OIC::Service::NSResult ret =  OIC::Service::NSProviderService::getInstance()->start(config);

    EXPECT_EQ(ret,  OIC::Service::NSResult::OK);
    OIC::Service::NSProviderService::getInstance()->stop();
}

TEST_F(NotificationProviderServiceTest,
       ExpectCallbackWhenReceiveSubscribeRequestWithAccepterProvider)
{
    g_consumer = NULL;
    mocks.ExpectCallFunc(ConsumerSubscribedCallbackEmpty).Do(
        []( std::shared_ptr<OIC::Service::NSConsumer> consumer)
    {
        std::cout << "ConsumerSubscribedCallbackEmpty" << std::endl;
        g_consumer = consumer;
        responseCon.notify_all();
    });

    OIC::Service::NSProviderService::ProviderConfig  config;
    config.m_subscribeRequestCb = ConsumerSubscribedCallbackEmpty;
    config.m_syncInfoCb = MessageSynchronizedCallbackEmpty;
    config.subControllability = true;

    OIC::Service::NSProviderService::getInstance()->start(config);

    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    g_consumerSimul.setCallback(MessageCallbackFromConsumerEmpty,
                                SyncCallbackFromConsumerEmpty);
    g_consumerSimul.findProvider();

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    ASSERT_NE(nullptr, g_consumer) << "error: discovery failure";
}

TEST_F(NotificationProviderServiceTest, NeverCallNotifyOnConsumerByAcceptIsFalse)
{
    bool expectTrue = true;
    int msgID = 0;

    mocks.OnCallFunc(MessageCallbackFromConsumerEmpty).Do(
        [& expectTrue, &msgID](const int &id, const std::string &, const std::string &,
                               const std::string &)
    {
        if (id == msgID)
        {
            std::cout << "This function never call" << std::endl;
            expectTrue = false;
        }
        responseCon.notify_all();
    });

    ASSERT_NE(nullptr, g_consumer) << "error: discovery failure";

    g_consumer->acceptSubscription(false);
    OIC::Service::NSMessage msg =  OIC::Service::NSProviderService::getInstance()->createMessage();
    msgID = (int)msg.getMessageId();
    msg.setTitle(std::string("Title"));
    msg.setContentText(std::string("ContentText"));
    msg.setSourceName(std::string("OCF"));

    OIC::Service::NSProviderService::getInstance()->sendMessage(msg);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(expectTrue, true);
}

TEST_F(NotificationProviderServiceTest, ExpectCallNotifyOnConsumerByAcceptIsTrue)
{
    int msgID = 0;

    ASSERT_NE(nullptr, g_consumer) << "error: discovery failure";

    mocks.ExpectCallFunc(MessageCallbackFromConsumerEmpty).Do(
        [&msgID](const int &id, const std::string &, const std::string &, const std::string &)
    {
        if (id == msgID)
        {
            std::cout << "ExpectCallNotifyOnConsumerByAcceptIsTrue" << std::endl;
            responseCon.notify_all();
        }
    });

    g_consumer->acceptSubscription(true);

    OIC::Service::NSMessage msg =  OIC::Service::NSProviderService::getInstance()->createMessage();
    msgID = (int)msg.getMessageId();
    msg.setTitle(std::string("Title"));
    msg.setContentText(std::string("ContentText"));
    msg.setSourceName(std::string("OCF"));

    OIC::Service::NSProviderService::getInstance()->sendMessage(msg);
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

}

TEST_F(NotificationProviderServiceTest, ExpectCallbackSyncOnReadToConsumer)
{
    int id = 0;

    mocks.ExpectCallFunc(SyncCallbackFromConsumerEmpty).Do(
        [& id](int &type, int &syncId)
    {
        std::cout << "MessageSynchronizedCallbackEmpty" << std::endl;
        if (syncId == id &&
            type == (int)OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ)
        {
            std::cout << "ExpectCallbackSyncOnReadFromConsumer" << std::endl;
            responseCon.notify_all();
        }
    });

    OIC::Service::NSMessage msg =  OIC::Service::NSProviderService::getInstance()->createMessage();
    id = (int)msg.getMessageId();
    msg.setTitle(std::string("Title"));
    msg.setContentText(std::string("ContentText"));
    msg.setSourceName(std::string("OCF"));

    OIC::Service::NSProviderService::getInstance()->sendSyncInfo(msg.getMessageId(),
            OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ);
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationProviderServiceTest, ExpectCallbackSyncOnReadFromConsumer)
{
    int type = (int)OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ;
    int id = 0;
    mocks.ExpectCallFunc(MessageSynchronizedCallbackEmpty).Do(
        [& id](OIC::Service::NSSyncInfo sync)
    {
        std::cout << "MessageSynchronizedCallbackEmpty" << std::endl;
        if ((int)sync.getMessageId() == id
            && sync.getState() == OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ)
        {
            std::cout << "ExpectCallbackSyncOnReadFromConsumer" << std::endl;
            responseCon.notify_all();
        }
    });

    OIC::Service::NSMessage msg =  OIC::Service::NSProviderService::getInstance()->createMessage();
    id = (int)msg.getMessageId();
    msg.setTitle(std::string("Title"));
    msg.setContentText(std::string("ContentText"));
    msg.setSourceName(std::string("OCF"));
    g_consumerSimul.syncToProvider(type, id, msg.getProviderId());
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait(lock);
}

TEST_F(NotificationProviderServiceTest, ExpectEqualAddedTopicsAndRegisteredTopics)
{
    std::string str1("TEST1");
    std::string str2("TEST2");
    OIC::Service::NSProviderService::getInstance()->registerTopic(str1);
    OIC::Service::NSProviderService::getInstance()->registerTopic(str2);
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
    bool isSame = false;
    auto topicList =
        OIC::Service::NSProviderService::getInstance()->getRegisteredTopicList();
    if (!topicList)
    {
        printf("topic is NULL\n");
        isSame = false;
    }
    else
    {
        std::string compString[10];
        int i = 0;
        for (auto itr : topicList->getTopicsList())
        {
            compString[i] = itr.getTopicName(); i++;
        }
        std::cout << compString[0] << std::endl;
        std::cout << compString[1] << std::endl;
        if (str1.compare(compString[0]) == 0 && str2.compare(compString[1]) == 0)
        {
            isSame = true;
        }
    }
    EXPECT_EQ(isSame, true);

    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str1);
    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str2);
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationProviderServiceTest, ExpectEqualUnregisteredTopicsAndRegisteredTopics)
{
    std::string str1("TEST1");
    std::string str2("TEST2");
    OIC::Service::NSProviderService::getInstance()->registerTopic(str1);
    OIC::Service::NSProviderService::getInstance()->registerTopic(str2);
    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str2);
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
    bool isSame = false;
    auto topicList =
        OIC::Service::NSProviderService::getInstance()->getRegisteredTopicList();
    if (!topicList)
    {
        printf("topic is NULL\n");
        isSame = false;
    }
    else
    {
        auto topic = topicList->getTopicsList();
        auto it = topic.begin();
        std::string compStr = (*it).getTopicName() ;
        std::cout << compStr << std::endl;
        if (str1.compare(compStr) == 0 )
        {
            isSame = true;
        }
    }
    EXPECT_EQ(isSame, true);

    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str1);
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationProviderServiceTest, ExpectEqualSetConsumerTopicsAndGetConsumerTopics)
{
    std::string str1("TEST1");
    std::string str2("TEST2");
    OIC::Service::NSProviderService::getInstance()->registerTopic(str1);
    OIC::Service::NSProviderService::getInstance()->registerTopic(str2);

    ASSERT_NE(nullptr, g_consumer) << "error: discovery failure";

    g_consumer->setTopic(str1);

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    bool isSame = false;
    auto topicList =  g_consumer->getConsumerTopicList();

    if (!topicList)
    {
        printf("topic is NULL\n");
        isSame = false;
    }
    else
    {
        std::string compString[10];
        int i = 0, state[10] = {0};
        for (auto itr : topicList->getTopicsList())
        {
            compString[i] = itr.getTopicName();
            state[i++] = (int) itr.getState();
        }
        std::cout << compString[0] << std::endl;
        std::cout << compString[1] << std::endl;
        if (str1.compare(compString[0]) == 0 && str2.compare(compString[1]) == 0
            && state[0] == 1 &&  state[1] == 0)
        {
            isSame = true;
        }
    }

    EXPECT_EQ(isSame, true);

    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str1);
    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str2);
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationProviderServiceTest, ExpectEqualUnSetConsumerTopicsAndGetConsumerTopics)
{
    std::string str1("TEST1");
    std::string str2("TEST2");
    OIC::Service::NSProviderService::getInstance()->registerTopic(str1);
    OIC::Service::NSProviderService::getInstance()->registerTopic(str2);

    ASSERT_NE(nullptr, g_consumer) << "error: discovery failure";

    g_consumer->setTopic(str1);
    g_consumer->setTopic(str2);
    g_consumer->unsetTopic(str1);

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    bool isSame = false;
    auto topicList =  g_consumer->getConsumerTopicList();

    if (!topicList)
    {
        printf("topic is NULL\n");
        isSame = false;
    }
    else
    {
        std::string compString[10];
        int i = 0, state[10] = {0};
        for (auto itr : topicList->getTopicsList())
        {
            compString[i] = itr.getTopicName();
            state[i++] = (int) itr.getState();
        }
        std::cout << compString[0] << std::endl;
        std::cout << compString[1] << std::endl;
        if (str1.compare(compString[0]) == 0 && str2.compare(compString[1]) == 0
            && state[0] == 0 &&  state[1] == 1)
        {
            isSame = true;
        }
    }

    EXPECT_EQ(isSame, true);

    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str1);
    OIC::Service::NSProviderService::getInstance()->unregisterTopic(str2);
    responseCon.wait_for(lock, g_waitForResponse);
}


TEST_F(NotificationProviderServiceTest, CancelObserves)
{
    bool ret = g_consumerSimul.cancelObserves();

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(ret, true);
}
