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

#include "ocstack.h"

#include "NSUtils.h"
#include "NSSyncInfo.h"
#include "NSMessage.h"
#include "NSMediaContents.h"
#include "NSConsumerService.h"

#include "NSProviderServiceSimulator.h"

namespace
{
    NSProviderSimulator g_providerSimul;
    std::shared_ptr<OIC::Service::NSProvider> g_provider;

    std::atomic_bool g_isStartedStack(false);

    std::chrono::milliseconds g_waitForResponse(1000);

    std::condition_variable responseCon;
    std::mutex mutexForCondition;

    enum class NSSelector
    {
        NS_SELECTION_CONSUMER = 0,
        NS_SELECTION_PROVIDER = 1
    };

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

class NotificationServiceConsumerTest : public TestWithMock
{
    public:
        NotificationServiceConsumerTest() = default;
        ~NotificationServiceConsumerTest() = default;

        static void ProviderDiscoveredCallbackEmpty( std::shared_ptr<OIC::Service::NSProvider> )
        {
            std::cout << __func__ << std::endl;
        }

        static void NotificationReceivedCallbackEmpty( OIC::Service::NSMessage )
        {
            std::cout << __func__ << std::endl;
        }

        static void SyncCallbackEmpty(OIC::Service::NSSyncInfo)
        {
            std::cout << __func__ << std::endl;
        }

        static void FoundResourceEmpty(std::shared_ptr< OC::OCResource >)
        {
            std::cout << __func__ << std::endl;
        }

        static void ProviderChangedCallbackEmpty( OIC::Service::NSProviderState )
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
                    OC::QualityOfService::LowQos
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

TEST_F(NotificationServiceConsumerTest, StartConsumerPositive)
{
    OIC::Service::NSConsumerService::getInstance()->start(ProviderDiscoveredCallbackEmpty);
}

TEST_F(NotificationServiceConsumerTest, StopConsumerPositive)
{
    OIC::Service::NSConsumerService::getInstance()->stop();
}

TEST_F(NotificationServiceConsumerTest, DiscoverProviderWithNonAccepterWhenStartedConsumerFirst)
{
    mocks.ExpectCallFunc(ProviderDiscoveredCallbackEmpty).Do(
        [this]( std::shared_ptr<OIC::Service::NSProvider> provider)
    {
        std::cout << "Call Discovered" << std::endl;
        std::cout << provider->getProviderId() << std::endl;
        responseCon.notify_all();
    });

    OIC::Service::NSConsumerService::getInstance()->start(ProviderDiscoveredCallbackEmpty);

    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_CONSUMER);
    g_providerSimul.createNotificationResource();

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    OIC::Service::NSConsumerService::getInstance()->stop();
    g_providerSimul.deleteNotificationResource();
}

TEST_F(NotificationServiceConsumerTest, DiscoverProviderWithNonAccepterWhenStartedConsumerAfter)
{
    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_CONSUMER);
    g_providerSimul.createNotificationResource();
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    mocks.ExpectCallFunc(ProviderDiscoveredCallbackEmpty).Do(
        [this]( std::shared_ptr<OIC::Service::NSProvider> provider)
    {
        std::cout << "Call Discovered" << std::endl;
        g_provider = provider;
        std::cout << g_provider->getProviderId() << std::endl;
        responseCon.notify_all();
    });

    OIC::Service::NSConsumerService::getInstance()->start(ProviderDiscoveredCallbackEmpty);

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

}

TEST_F(NotificationServiceConsumerTest, DiscoverProviderWithNonAccepterWhenRescan)
{
    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_CONSUMER);
    mocks.OnCallFunc(ProviderDiscoveredCallbackEmpty).Do(
        [this]( std::shared_ptr<OIC::Service::NSProvider> provider)
    {
        std::cout << "Call Discovered" << std::endl;
        g_provider = provider;
        std::cout << g_provider->getProviderId() << std::endl;
        responseCon.notify_all();
    });

    OIC::Service::NSConsumerService::getInstance()->rescanProvider();

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationServiceConsumerTest, ExpectSubscribeSuccess)
{
    OIC::Service::NSProviderState revState = OIC::Service::NSProviderState::DENY;
    mocks.OnCallFunc(ProviderChangedCallbackEmpty).Do(
        [this, & revState](OIC::Service::NSProviderState state)
    {
        std::cout << "Income Accepted subscription : " << std::endl;
        revState = state;
        responseCon.notify_all();
    });

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    g_provider->setListener( (OIC::Service::NSProvider::ProviderStateCallback)
                             ProviderChangedCallbackEmpty,
                             (OIC::Service::NSProvider::MessageReceivedCallback)NotificationReceivedCallbackEmpty,
                             (OIC::Service::NSProvider::SyncInfoReceivedCallback)SyncCallbackEmpty);
    if (!g_provider->isSubscribed())
    {
        g_provider->subscribe();
    }
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
    EXPECT_EQ(OIC::Service::NSProviderState::ALLOW, revState);
}

TEST_F(NotificationServiceConsumerTest, ExpectReceiveNotification)
{
    uint64_t id = 10;
    std::string title = "title";
    std::string msg = "msg";

    mocks.ExpectCallFunc(NotificationReceivedCallbackEmpty).Do(
        [this]( OIC::Service::NSMessage message)
    {
        std::cout << "Income Notification : " << message.getMessageId() << std::endl;
        responseCon.notify_all();
    });

    g_providerSimul.notifyMessage(id, title, msg);

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    OIC::Service::NSConsumerService::getInstance()->stop();
}

TEST_F(NotificationServiceConsumerTest, DiscoverProviderWithAccepterisProvider)
{
    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_PROVIDER);

    mocks.ExpectCallFunc(ProviderDiscoveredCallbackEmpty).Do(
        [this]( std::shared_ptr<OIC::Service::NSProvider> provider)
    {
        std::cout << "Call Discovered" << std::endl;
        g_provider = provider;
        g_provider->setListener( (OIC::Service::NSProvider::ProviderStateCallback)
                                 ProviderChangedCallbackEmpty,
                                 (OIC::Service::NSProvider::MessageReceivedCallback)NotificationReceivedCallbackEmpty,
                                 (OIC::Service::NSProvider::SyncInfoReceivedCallback)SyncCallbackEmpty);
        if (!g_provider->isSubscribed())
        {
            g_provider->subscribe();
        }
        std::cout << g_provider->getProviderId() << std::endl;
        responseCon.notify_all();
    });


    OIC::Service::NSConsumerService::getInstance()->start(ProviderDiscoveredCallbackEmpty);
    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationServiceConsumerTest, ExpectReceiveNotificationWithAccepterisProvider)
{
    uint64_t id = 11;
    std::string title = "title";
    std::string msg = "msg";
    uint64_t revId = 1;

    mocks.OnCallFunc(NotificationReceivedCallbackEmpty).Do(
        [this, & id, & revId](OIC::Service::NSMessage message)
    {
        std::cout << "Income Notification : " << message.getMessageId() << std::endl;
        revId =  message.getMessageId();
        responseCon.notify_all();
    });

    g_providerSimul.notifyMessage(id, title, msg);

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(id, revId);
}

TEST_F(NotificationServiceConsumerTest, ExpectCallbackReadCheckWhenProviderNotifySync)
{
    uint64_t id = 12;
    std::string title = "title";
    std::string msg = "msg";
    OIC::Service::NSSyncInfo::NSSyncType type = OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED;

    mocks.OnCallFunc(NotificationReceivedCallbackEmpty).Do(
        [this]( OIC::Service::NSMessage message)
    {
        std::cout << "Income Notification : " << message.getMessageId() << std::endl;
    });

    mocks.OnCallFunc(SyncCallbackEmpty).Do(
        [& type, this](OIC::Service::NSSyncInfo sync)
    {
        std::cout << "Income SyncInfo : " << sync.getMessageId()
                  << ", State : " << (int) sync.getState() << std::endl;
        type = sync.getState();
        responseCon.notify_all();
    });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    g_providerSimul.sendRead(id);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ, type);
}

TEST_F(NotificationServiceConsumerTest, ExpectCallbackDismissCheckWhenProviderNotifySync)
{
    uint64_t id = 13;
    std::string title = "title";
    std::string msg = "msg";
    OIC::Service::NSSyncInfo::NSSyncType type = OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ;

    mocks.OnCallFunc(NotificationReceivedCallbackEmpty).Do(
        [this]( OIC::Service::NSMessage message)
    {
        std::cout << "Income Notification : " << message.getMessageId() << std::endl;
    });

    mocks.OnCallFunc(SyncCallbackEmpty).Do(
        [& type, this](OIC::Service::NSSyncInfo sync)
    {
        std::cout << "Income Notification : " << sync.getMessageId()
                  << ", State : " << (int) sync.getState() << std::endl;
        type = sync.getState();
        responseCon.notify_all();
    });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    g_providerSimul.sendDismiss(id);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED, type);
}

TEST_F(NotificationServiceConsumerTest, ExpectCallbackReadCheckWhenConsumerPostSync)
{
    uint64_t id = 14;
    std::string title = "title";
    std::string msg = "msg";
    OIC::Service::NSSyncInfo::NSSyncType type = OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED;

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    mocks.OnCallFunc(NotificationReceivedCallbackEmpty).Do(
        [this]( OIC::Service::NSMessage message)
    {
        std::cout << "Income Notification : " << message.getMessageId() << std::endl;
        g_provider->sendSyncInfo(message.getMessageId(),
                                 OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ);
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    });

    mocks.OnCallFunc(SyncCallbackEmpty).Do(
        [& type, this](OIC::Service::NSSyncInfo sync)
    {
        std::cout << "Income Notification : " << sync.getMessageId()
                  << ", State : " << (int) sync.getState() << std::endl;
        type = sync.getState();
        responseCon.notify_all();
    });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ, type);
}

TEST_F(NotificationServiceConsumerTest, ExpectCallbackDismissCheckWhenConsumerPostSync)
{
    uint64_t id = 15;
    std::string title = "title";
    std::string msg = "msg";
    OIC::Service::NSSyncInfo::NSSyncType type = OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ;

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    mocks.OnCallFunc(NotificationReceivedCallbackEmpty).Do(
        [this]( OIC::Service::NSMessage message)
    {
        std::cout << "Income Notification : " << message.getMessageId() << std::endl;
        g_provider->sendSyncInfo(message.getMessageId(),
                                 OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED);
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    });

    mocks.OnCallFunc(SyncCallbackEmpty).Do(
        [& type, this](OIC::Service::NSSyncInfo sync)
    {
        std::cout << "Income Notification : " << sync.getMessageId()
                  << ", State : " << (int) sync.getState() << std::endl;
        type = sync.getState();
        responseCon.notify_all();
    });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock { mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED, type);
}

TEST_F(NotificationServiceConsumerTest, ExpectGetProviderSuccessWithValidProviderId)
{
    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    std::shared_ptr<OIC::Service::NSProvider> provider =
        OIC::Service::NSConsumerService::getInstance()->getProvider(
            g_provider->getProviderId());
    int ret = strcmp(provider->getProviderId().c_str(), g_provider->getProviderId().c_str());
    EXPECT_EQ(0, ret);
}

TEST_F(NotificationServiceConsumerTest, ExpectGetProviderSuccessWithInvalidProviderId)
{
    std::shared_ptr<OIC::Service::NSProvider> provider =
        OIC::Service::NSConsumerService::getInstance()->getProvider(
            "123456789012345678901234567890123457");
    bool res = (provider == nullptr);
    EXPECT_EQ(res, 1);
}

TEST_F(NotificationServiceConsumerTest, ExpectCallbackTopicUpdated)
{
    OIC::Service::NSProviderState revState = OIC::Service::NSProviderState::STOPPED;
    mocks.OnCallFunc(ProviderChangedCallbackEmpty).Do(
        [this, & revState](OIC::Service::NSProviderState state)
    {
        std::cout << "Income Changed Callback : " << (int)state << std::endl;
        revState = state;
        responseCon.notify_all();
    });

    NSProviderSimulator::NS_TopicList topics;
    topics.push_back("1");
    topics.push_back("2");
    topics.push_back("3");

    g_providerSimul.setTopics(topics);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(OIC::Service::NSProviderState::TOPIC, revState);
}

TEST_F(NotificationServiceConsumerTest, ExpectEQTopicList)
{
    bool isSame = true;

    NSProviderSimulator::NS_TopicList topics;
    topics.push_back("1");
    topics.push_back("2");
    topics.push_back("3");

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    auto  retTopic = g_provider->getTopicList()->getTopicsList();
    auto it1 = retTopic.begin();
    auto it2 = topics.begin();
    while ( it1 != retTopic.end() || it2 != topics.end() )
    {
        if ((*it1).getTopicName() !=  *it2)
        {
            isSame = false; break;
        }
        it1++; it2++;
    }


    EXPECT_EQ(true, isSame);
}

TEST_F(NotificationServiceConsumerTest, ExpectFailUpdateTopicOnConsumer)
{
    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    auto retTopic = g_provider->getTopicList();
    for (auto it : retTopic->getTopicsList())
    {
        std::cout << "Topic Name: " << it.getTopicName() << std::endl;
        std::cout << "state : " << (int) it.getState() << std::endl;
        it.setState(OIC::Service::NSTopic::NSTopicState::SUBSCRIBED);
    }
    OIC::Service::NSResult ret = g_provider->updateTopicList(retTopic);
    std::cout << "ret : " << (int) ret << std::endl;

    EXPECT_EQ(OIC::Service::NSResult::ERROR, ret);
}


TEST_F(NotificationServiceConsumerTest, ExpectCallbackDeletedProvider)
{
    OIC::Service::NSProviderState type = OIC::Service::NSProviderState::ALLOW;
    mocks.OnCallFunc(ProviderChangedCallbackEmpty).Do(
        [& type, this](OIC::Service::NSProviderState state)
    {
        std::cout << "Income Changed Callback : " << std::endl;
        type = state;
    });

    g_providerSimul.deleteNotificationResource();

    std::unique_lock< std::mutex > lock { mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(type, OIC::Service::NSProviderState::STOPPED);
    OIC::Service::NSConsumerService::getInstance()->stop();
}
