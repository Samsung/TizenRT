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

#include "NSCommon.h"
#include "NSConsumerInterface.h"

#include "NSProviderSimulator.h"

namespace
{
    NSProviderSimulator g_providerSimul;
    NSProvider * g_provider;

    std::atomic_bool g_isStartedStack(false);

    std::chrono::milliseconds g_waitForResponse(500);

    std::condition_variable responseCon;
    std::mutex mutexForCondition;

    enum class NSSelector
    {
        NS_SELECTION_CONSUMER = 0,
        NS_SELECTION_PROVIDER = 1
    };

    NSConsumerConfig cfg;

    NSProviderSimulator::NS_TopicStateList g_topicStateList;
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

class NotificationConsumerTest : public TestWithMock
{
public:
    NotificationConsumerTest() = default;
    ~NotificationConsumerTest() = default;

    static void NSNotificationReceivedCallbackEmpty(NSMessage *)
    {
        std::cout << __func__ << std::endl;
    }

    static void NSSyncCallbackEmpty(NSSyncInfo *)
    {
        std::cout << __func__ << std::endl;
    }

    static void NSFoundResourceEmpty(std::shared_ptr< OC::OCResource >)
    {
        std::cout << __func__ << std::endl;
    }

    static void NSProviderChangedCallback(NSProvider *,  NSProviderState)
    {
        std::cout << __func__ << std::endl;
    }

protected:

    void SetUp()
    {
        TestWithMock::SetUp();

        if (g_isStartedStack == false)
        {
            OC::PlatformConfig occfg
            {
                OC::ServiceType::InProc,
                OC::ModeType::Both,
                "0.0.0.0",
                0,
                OC::QualityOfService::LowQos
            };
            OC::OCPlatform::Configure(occfg);

            try
            {
                OC::OCPlatform::stopPresence();
            }
            catch (...)
            {

            }

            g_isStartedStack = true;

            cfg.changedCb = NSProviderChangedCallback;
            cfg.messageCb = NSNotificationReceivedCallbackEmpty;
            cfg.syncInfoCb = NSSyncCallbackEmpty;
        }

    }

    void TearDown()
    {
        TestWithMock::TearDown();
    }

};

TEST_F(NotificationConsumerTest, StartConsumerNegativeNonSetChangedCB)
{
    cfg.changedCb = NULL;
    cfg.messageCb = NSNotificationReceivedCallbackEmpty;
    cfg.syncInfoCb = NSSyncCallbackEmpty;

    EXPECT_EQ(NS_ERROR, NSStartConsumer(cfg));
    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationConsumerTest, StartConsumerNegativeNonSetNotiReceiveCB)
{
    cfg.changedCb = NSProviderChangedCallback;
    cfg.messageCb = NULL;
    cfg.syncInfoCb = NSSyncCallbackEmpty;

    EXPECT_EQ(NS_ERROR, NSStartConsumer(cfg));
    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationConsumerTest, StartConsumerNegativeNonSetSyncCB)
{
    cfg.changedCb = NSProviderChangedCallback;
    cfg.messageCb = NSNotificationReceivedCallbackEmpty;
    cfg.syncInfoCb = NULL;

    EXPECT_EQ(NS_ERROR, NSStartConsumer(cfg));
    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationConsumerTest, StartConsumerPositive)
{
    cfg.changedCb = NSProviderChangedCallback;
    cfg.messageCb = NSNotificationReceivedCallbackEmpty;
    cfg.syncInfoCb = NSSyncCallbackEmpty;

    EXPECT_EQ(NS_OK, NSStartConsumer(cfg));
    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);
}

TEST_F(NotificationConsumerTest, StopConsumerPositive)
{
    EXPECT_EQ(NSStopConsumer(), NS_OK);
}

TEST_F(NotificationConsumerTest, StopConsumerNegative)
{
    EXPECT_EQ(NSStopConsumer(), NS_ERROR);
}

TEST_F(NotificationConsumerTest, DiscoverProviderWithNonAccepterWhenStartedConsumerFirst)
{
    NSProviderState revState = NS_STOPPED;
    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [this, & revState](NSProvider *, NSProviderState state)
            {
                std::cout << "Call Discovered" << std::endl;
                revState = state;
                responseCon.notify_all();
            });

    NSStartConsumer(cfg);

    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_CONSUMER);
    g_providerSimul.createNotificationResource();

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    NSStopConsumer();
    g_providerSimul.deleteNotificationResource();

    EXPECT_EQ(NS_DISCOVERED, revState);
}

TEST_F(NotificationConsumerTest, DiscoverProviderWithNonAccepterWhenStartedConsumerAfter)
{
    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_CONSUMER);
    g_providerSimul.createNotificationResource();
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    NSProviderState revState = NS_STOPPED;
    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [this, & revState](NSProvider * provider, NSProviderState state)
            {
                std::cout << "Call Discovered" << std::endl;

                g_provider = provider;
                revState = state;
                responseCon.notify_all();
            });

    NSStartConsumer(cfg);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(NS_DISCOVERED, revState);

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";
}

TEST_F(NotificationConsumerTest, DiscoverProviderWithNonAccepterWhenRescan)
{
    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_CONSUMER);
    NSProviderState revState = NS_STOPPED;
    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [this, &revState](NSProvider * provider, NSProviderState state)
            {
                std::cout << "Call Discovered" << std::endl;
                revState = state;
                g_provider = provider;
                std::cout << g_provider->providerId << std::endl;
                responseCon.notify_all();
            });

    NSRescanProvider();

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(NS_DISCOVERED, revState);
}

TEST_F(NotificationConsumerTest, ExpectSubscribeSuccess)
{
    NSProviderState revState = NS_DENY;

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [this, & revState](NSProvider * , NSProviderState state)
            {
                std::cout << "Income Changed Callback : " << state << std::endl;
                revState = state;
                responseCon.notify_all();
            });

    NSResult ret = NSSubscribe(g_provider->providerId);
    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(NS_ALLOW, revState);
    EXPECT_EQ(NS_OK, ret);
}

TEST_F(NotificationConsumerTest, ExpectReceiveNotification)
{
    uint64_t id = 10;
    std::string title = "title";
    std::string msg = "msg";
    uint64_t revId = 0;

    mocks.OnCallFunc(NSNotificationReceivedCallbackEmpty).Do(
            [this, & revId](NSMessage * message)
            {
                std::cout << "Income Notification : " << message->messageId << std::endl;
                revId = message->messageId;
                responseCon.notify_all();
            });

    g_providerSimul.notifyMessage(id, title, msg);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(id, revId);
    NSStopConsumer();
    g_providerSimul.deleteNotificationResource();
}

TEST_F(NotificationConsumerTest, ExpectReceiveSubAllowWithAccepterisProvider)
{
    g_providerSimul.setAccepter((int)NSSelector::NS_SELECTION_PROVIDER);
    NSProviderState revState = NS_DENY;
    g_providerSimul.createNotificationResource();
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [this, & revState](NSProvider * provider, NSProviderState state)
            {
                std::cout << "Income Changed Callback : " << state << std::endl;
                revState = state;
                g_provider = provider;
                responseCon.notify_all();
            });
    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [this, & revState](NSProvider *, NSProviderState state)
            {
                std::cout << "Income Changed Callback : " << state << std::endl;
                revState = state;
                responseCon.notify_all();
            });

    NSStartConsumer(cfg);
    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(NS_ALLOW, revState);
}

TEST_F(NotificationConsumerTest, ExpectReceiveNotificationWithAccepterisProvider)
{
    uint64_t id = 11;
    std::string title = "title";
    std::string msg = "msg";
    uint64_t revId = 1;

    mocks.OnCallFunc(NSNotificationReceivedCallbackEmpty).Do(
            [this, & id, & revId](NSMessage * message)
            {
                std::cout << "Income Notification : " << message->messageId << std::endl;
                revId = message->messageId;
                responseCon.notify_all();
            });

    g_providerSimul.notifyMessage(id, title, msg);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(id, revId);
}

TEST_F(NotificationConsumerTest, ExpectCallbackReadCheckWhenProviderNotifySync)
{
    uint64_t id = 12;
    std::string title = "title";
    std::string msg = "msg";
    NSSyncType type = NS_SYNC_DELETED;

    mocks.OnCallFunc(NSNotificationReceivedCallbackEmpty).Do(
            [this](NSMessage * message)
            {
                std::cout << "Income Notification : " << message->messageId << std::endl;
                responseCon.notify_all();
            });

    mocks.OnCallFunc(NSSyncCallbackEmpty).Do(
            [& type, this](NSSyncInfo * sync)
            {
                std::cout << "Income SyncInfo : " << sync->messageId
                        << ", State : " << sync->state << std::endl;
                type = sync->state;
                responseCon.notify_all();
            });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    g_providerSimul.sendRead(id);
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(NS_SYNC_READ, type);
}

TEST_F(NotificationConsumerTest, ExpectCallbackDismissCheckWhenProviderNotifySync)
{
    uint64_t id = 13;
    std::string title = "title";
    std::string msg = "msg";
    NSSyncType type = NS_SYNC_READ;

    mocks.OnCallFunc(NSNotificationReceivedCallbackEmpty).Do(
            [this](NSMessage * message)
            {
                std::cout << "Income Notification : " << message->messageId << std::endl;
                responseCon.notify_all();
            });

    mocks.OnCallFunc(NSSyncCallbackEmpty).Do(
            [& type, this](NSSyncInfo * sync)
            {
                std::cout << "Income Notification : " << sync->messageId
                        << ", State : " << sync->state << std::endl;
                type = sync->state;
                responseCon.notify_all();
            });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    g_providerSimul.sendDismiss(id);
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(NS_SYNC_DELETED, type);
}

TEST_F(NotificationConsumerTest, ExpectCallbackReadCheckWhenConsumerPostSync)
{
    uint64_t id = 14;
    std::string title = "title";
    std::string msg = "msg";
    NSSyncType type = NS_SYNC_DELETED;

    mocks.OnCallFunc(NSNotificationReceivedCallbackEmpty).Do(
            [this](NSMessage * message)
            {
                std::cout << "Income Notification : " << message->messageId << std::endl;
                NSConsumerSendSyncInfo(message->providerId, message->messageId, NS_SYNC_READ);
                std::unique_lock< std::mutex > lock{ mutexForCondition };
                responseCon.wait_for(lock, g_waitForResponse);
            });

    mocks.OnCallFunc(NSSyncCallbackEmpty).Do(
            [& type, this](NSSyncInfo * sync)
            {
                std::cout << "Income Notification : " << sync->messageId
                        << ", State : " << sync->state << std::endl;
                type = sync->state;
                responseCon.notify_all();
            });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(NS_SYNC_READ, type);
}

TEST_F(NotificationConsumerTest, ExpectCallbackDismissCheckWhenConsumerPostSync)
{
    uint64_t id = 15;
    std::string title = "title";
    std::string msg = "msg";
    NSSyncType state = NS_SYNC_READ;

    mocks.OnCallFunc(NSNotificationReceivedCallbackEmpty).Do(
            [this](NSMessage * message)
            {
                std::cout << "Income Notification : " << message->messageId << std::endl;
                NSConsumerSendSyncInfo(message->providerId, message->messageId, NS_SYNC_DELETED);
                std::unique_lock< std::mutex > lock{ mutexForCondition };
                responseCon.wait_for(lock, g_waitForResponse);
            });

    mocks.OnCallFunc(NSSyncCallbackEmpty).Do(
            [& state, this](NSSyncInfo * sync)
            {
                std::cout << "Income Notification : " << sync->messageId
                        << ", State : " << sync->state << std::endl;
                state = sync->state;
                responseCon.notify_all();
            });

    g_providerSimul.notifyMessage(id, title, msg);
    {
        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

    EXPECT_EQ(NS_SYNC_DELETED, state);
}

TEST_F(NotificationConsumerTest, ExpectGetProviderSuccessWithValidProviderId)
{
    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    NSProvider * provider = NSConsumerGetProvider(g_provider->providerId);
    int ret = strcmp(provider->providerId, g_provider->providerId);
    EXPECT_EQ(0, ret);
    free(provider);
}

TEST_F(NotificationConsumerTest, ExpectGetProviderSuccessWithInvalidProviderId)
{
    NSProvider * provider = NSConsumerGetProvider("123456789012345678901234567890123457");
    EXPECT_EQ(provider, (void*)NULL);
}

TEST_F(NotificationConsumerTest, ExpectGetProviderSuccessWithNULL)
{
    NSProvider * provider = NSConsumerGetProvider(NULL);
    EXPECT_EQ(provider, (void*)NULL);
}

TEST_F(NotificationConsumerTest, ExpectGetTopicListIsNULL)
{
    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    NSTopicLL * currentTopics = NSConsumerGetTopicList(g_provider->providerId);
    EXPECT_EQ(NULL, currentTopics);
}

TEST_F(NotificationConsumerTest, ExpectCallbackTopicUpdated)
{
    NSProviderState revState = NS_STOPPED;
    mocks.OnCallFunc(NSProviderChangedCallback).Do(
        [this, & revState](NSProvider * , NSProviderState state)
        {
            std::cout << "Income Changed Callback : " << state << std::endl;
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

    EXPECT_EQ(NS_TOPIC, revState);
}

TEST_F(NotificationConsumerTest, ExpectEQTopicList)
{
    bool isSame = false;

    NSProviderSimulator::NS_TopicList topics;
    topics.push_back("1");
    topics.push_back("2");
    topics.push_back("3");

    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    NSTopicLL * retTopic = NSConsumerGetTopicList(g_provider->providerId);
    std::for_each (topics.begin(), topics.end(),
            [this, & retTopic, & isSame](const std::string & str)
            {
                isSame = (str == std::string(retTopic->topicName));
                retTopic = retTopic->next;
            });

    EXPECT_EQ(true, isSame);
}

TEST_F(NotificationConsumerTest, ExpectFailUpdateTopicOnConsumer)
{
    ASSERT_NE(nullptr, g_provider) << "error: discovery failure";

    NSTopicLL * retTopic = NSConsumerGetTopicList(g_provider->providerId);
    for (; retTopic; retTopic = retTopic->next)
    {
        retTopic->state = NS_TOPIC_SUBSCRIBED;
    }
    NSResult ret = NSConsumerUpdateTopicList(g_provider->providerId, retTopic);

    EXPECT_EQ(NS_ERROR, ret);
}

TEST_F(NotificationConsumerTest, ExpectCallbackDeletedProvider)
{
    NSProviderState type = NS_ALLOW;
    mocks.OnCallFunc(NSProviderChangedCallback).Do(
            [& type, this](NSProvider * , NSProviderState state)
            {
                std::cout << "Income Changed Callback : " << state << std::endl;
                type = state;
                responseCon.notify_all();
            });

    g_providerSimul.deleteNotificationResource();

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, std::chrono::milliseconds(2000));

    EXPECT_EQ(type, NS_STOPPED);
    NSStopConsumer();
}
