/******************************************************************
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

// std
#include <iostream>
#include <stdlib.h>
#include <cstdint>
#include <limits>

// ns
#include "NSCommon.h"
#include "NSProviderService.h"
#include "NSUtils.h"
#include "NSTopicsList.h"

// base
#include "logger.h"
#include "octypes.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "ocstack.h"

// external
#include "pthread.h"

#define TAG "NotiProviderWrapperExample"
using namespace std;
using namespace OIC::Service;
std::vector<std::string> discoveredConsumers;
uint64_t mainMessageId = 0;

extern char *strdup(const char *s);

bool isExit = false;

int id = 0;
std::string REMOTE_SERVER_ADDRESS;

void *OCProcessThread(void *ptr)
{
    (void) ptr;
    while (!isExit)
    {
        usleep(2000);
        if (OCProcess() != OC_STACK_OK)
        {
            std::cout << "OCStack process error" << std::endl;
            return NULL;
        }
    }

    return NULL;
}

void subscribeRequestCallback(std::shared_ptr<OIC::Service::NSConsumer> consumer)
{
    std::cout << "consumer requested to subscribe" << std::endl;

    std::cout << "Consumer Device ID: " << consumer->getConsumerId() << std::endl;
    discoveredConsumers.push_back(consumer->getConsumerId());
    consumer->acceptSubscription(true);
}

void syncCallback(OIC::Service::NSSyncInfo sync)
{
    std::cout << "SyncInfo Received " << std::endl;
    std::cout << "Sync ID : " <<  sync.getMessageId() << std::endl;
    std::cout << "Provider ID : " <<  sync.getProviderId() << std::endl;
    std::cout << "Sync State: " << (int) sync.getState() << std::endl;
}

std::shared_ptr<OIC::Service::NSConsumer> printAvailableConsumers()
{
    std::cout << "Choose the Consumer ID for operation" << std::endl;
    int pos = 1;
    unsigned int option = 0;
    for (auto it : discoveredConsumers)
    {
        std::cout << pos << ". " << it << std::endl;
        pos++;
    }
    while (!(std::cin >> option))
    {
        std::cout << "Bad value!" << std::endl;;
        std::cin.clear();
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    option--;
    if (option > discoveredConsumers.size())
    {
        return NULL;
    }
    std::string consumerId = discoveredConsumers[option];
    std::shared_ptr<OIC::Service::NSConsumer> consumer =
        NSProviderService::getInstance()->getConsumer(consumerId);
    return consumer;
}

int main()
{
    int num = 0;
    pthread_t processThread = 0;

    std::cout << "start Iotivity" << std::endl;


    if (OCInit(NULL, 0, OC_CLIENT_SERVER) != OC_STACK_OK)
    {
        std::cout << "OCStack init error" << std::endl;
        return 0;
    }

    pthread_create(&processThread, NULL, OCProcessThread, NULL);

    while (!isExit)
    {
        std::cout << "1. Start the Notification Provider(Accepter: Provider)" << std::endl;
        std::cout << "2. Start the Notification Provider(Accepter: Consumer)" << std::endl;
        std::cout << "3. Allow Subscription" << std::endl;
        std::cout << "4. Deny Subscription" << std::endl;
        std::cout << "5. SendMessage " << std::endl;
        std::cout << "6. SendSyncInfo" << std::endl;

        std::cout << "7. RegisterTopic" << std::endl;
        std::cout << "8. UnregisterTopic" << std::endl;
        std::cout << "9. SetTopic" << std::endl;
        std::cout << "10. UnsetTopic" << std::endl;
        std::cout << "11. GetConsumerTopicList" << std::endl;
        std::cout << "12. GetRegisteredTopicList" << std::endl;
#ifdef WITH_CLOUD
        std::cout << "13. Enable NS Provider RemoteService" << std::endl;
        std::cout << "14. Disable NS Provider RemoteService" << std::endl;
#endif
        std::cout << "15. Stop the Notification Provider" << std::endl;
        std::cout << "16. Exit()" << std::endl;

        std::cout << "input : ";

        std::cin >> num;

        switch (num)
        {
            case 1:
                {
                    std::cout << "Start (Accepter: Provider)" << std::endl;
                    NSProviderService::ProviderConfig cfg;
                    cfg.m_subscribeRequestCb = subscribeRequestCallback;
                    cfg.m_syncInfoCb = syncCallback;
                    cfg.subControllability = true;

                    NSProviderService::getInstance()->start(cfg);
                    break;
                }
            case 2:
                {
                    std::cout << "Start (Accepter: Consumer)" << std::endl;
                    NSProviderService::ProviderConfig cfg;
                    cfg.m_subscribeRequestCb = subscribeRequestCallback;
                    cfg.m_syncInfoCb = syncCallback;
                    cfg.subControllability = false;

                    NSProviderService::getInstance()->start(cfg);
                    break;
                }
            case 3:
                {
                    std::cout << "Allow Subscription" << std::endl;
                    std::shared_ptr<OIC::Service::NSConsumer> consumer = printAvailableConsumers();
                    if (consumer != nullptr)
                    {
                        std::cout << "ALLOW" << std::endl;
                        consumer->acceptSubscription(true);
                    }
                    break;
                }
            case 4:
                {
                    std::cout << "Deny Subscription" << std::endl;
                    std::shared_ptr<OIC::Service::NSConsumer>consumer = printAvailableConsumers();
                    if (consumer != nullptr)
                    {
                        std::cout << "DENY" << std::endl;
                        consumer->acceptSubscription(false);
                    }
                    break;
                }
            case 5:
                {
                    std::cout << "------------------------------------" << std::endl;
                    std::cout << "SendMessage" << std::endl;
                    std::cout << "------------------------------------" << std::endl;

                    std::string dummy;
                    std::string title;
                    std::string body;
                    std::string topic;

                    std::cout << "id : " << ++id << std::endl;
                    std::cout << "title : ";

                    std::getline(std::cin, dummy);
                    std::getline(std::cin, title);

                    std::cout << "body : ";
                    std::getline(std::cin, body);

                    std::cout << "topic : ";
                    std::getline(std::cin, topic);

                    std::cout << "app - mTitle : " << title << std::endl;
                    std::cout << "app - mContentText : " << body << std::endl;
                    std::cout << "app - mTopic : " << topic << std::endl;

                    OIC::Service::NSMessage msg = NSProviderService::getInstance()->createMessage();

                    msg.setType(OIC::Service::NSMessage::NSMessageType::NS_MESSAGE_INFO);
                    msg.setTitle(title.c_str());
                    msg.setContentText(body.c_str());
                    msg.setSourceName("OCF");
                    msg.setTopic(topic);
                    msg.setTTL(40);
                    msg.setTime("12:30");
                    OIC::Service::NSMediaContents *mediaContents =
                        new OIC::Service::NSMediaContents("iconImage");
                    msg.setMediaContents(mediaContents);

                    OC::OCRepresentation rep;
                    rep.setValue("Key1", "Value1");
                    rep.setValue("Key2", "Value2");
                    msg.setExtraInfo(rep);

                    mainMessageId = msg.getMessageId();
                    std::cout << "ProviderID for Message : " << msg.getProviderId() << std::endl;
                    std::cout << "MessageID for Message : " << msg.getMessageId() << std::endl;

                    NSProviderService::getInstance()->sendMessage(msg);
                    break;
                }
            case 6:
                {
                    std::cout << "------------------------------------" << std::endl;
                    std::cout <<  "SendSyncInfo" << std::endl;
                    std::cout << "------------------------------------" << std::endl;
                    if (!mainMessageId)
                    {
                        std::cout <<  "Message ID is empty" << std::endl;
                        break;
                    }
                    std::cout << "1. Send Read Sync" << std::endl;
                    std::cout << "2. Send Delete Sync" << std::endl;
                    int syn = 0;
                    while (!(std::cin >> syn))
                    {
                        std::cout << "Bad value!" << std::endl;;
                        std::cin.clear();
                        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    switch (syn)
                    {
                        case 1:
                            {
                                std::cout << "Sending Read Sync" << std::endl;
                                NSProviderService::getInstance()->sendSyncInfo(mainMessageId,
                                        OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ);
                            }
                            break;
                        case 2:
                            {
                                std::cout << "Sending Delete Sync" << std::endl;
                                NSProviderService::getInstance()->sendSyncInfo(mainMessageId,
                                        OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED);
                            }
                            break;
                        default:
                            {
                                cout << "Invalid Input!. sending default Read Sync";
                                NSProviderService::getInstance()->sendSyncInfo(mainMessageId,
                                        OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ);
                                std::cin.clear();
                                std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                break;
                            }
                    }
                    break;
                }

            case 7:
                {
                    std::cout <<  "RegisterTopic" << std::endl;
                    NSProviderService::getInstance()->registerTopic("OCF_TOPIC1");
                    NSProviderService::getInstance()->registerTopic("OCF_TOPIC2");
                    NSProviderService::getInstance()->registerTopic("OCF_TOPIC3");
                    NSProviderService::getInstance()->registerTopic("OCF_TOPIC4");
                    break;
                }
            case 8:
                {
                    std::cout <<  "UnregisterTopic" << std::endl;
                    NSProviderService::getInstance()->unregisterTopic("OCF_TOPIC2");
                    break;
                }
            case 9:
                {
                    std::cout <<  "SetTopic" << std::endl;
                    std::shared_ptr<OIC::Service::NSConsumer> consumer = printAvailableConsumers();
                    if (consumer != nullptr)
                    {
                        consumer->setTopic("OCF_TOPIC1");
                        consumer->setTopic("OCF_TOPIC2");
                        consumer->setTopic("OCF_TOPIC3");
                        std::cout <<  "SelectTopic completed" << std::endl;
                    }
                    break;
                }
            case 10:
                {
                    std::cout <<  "UnsetTopic" << std::endl;
                    std::shared_ptr<OIC::Service::NSConsumer> consumer = printAvailableConsumers();
                    if (consumer != nullptr)
                    {
                        consumer->unsetTopic("OCF_TOPIC1");
                        std::cout <<  "UnSelectTopic completed" << std::endl;
                    }
                    break;
                }

            case 11:
                {
                    std::cout <<  "GetConsumerTopicList" << std::endl;
                    std::shared_ptr<OIC::Service::NSConsumer> consumer = printAvailableConsumers();
                    if (consumer != nullptr)
                    {
                        auto nsTopics = consumer->getConsumerTopicList();
                        if (nsTopics != nullptr)
                        {
                            for (auto it : nsTopics->getTopicsList())
                            {

                                std::cout << it.getTopicName() << std::endl;
                                std::cout << (int) it.getState() << std::endl;
                            }
                        }
                        std::cout <<  "GetConsumerTopicList completed" << std::endl;
                    }
                    break;
                }

            case 12:
                {
                    std::cout <<  "GetRegisteredTopicList" << std::endl;
                    auto nsTopics = NSProviderService::getInstance()->getRegisteredTopicList();
                    for (auto it : nsTopics->getTopicsList())
                    {

                        std::cout << it.getTopicName() << std::endl;
                        std::cout << (int) it.getState() << std::endl;
                    }
                    break;
                }
#ifdef WITH_CLOUD
            case 13:
                {
                    std::cout << "Enable NS Provider RemoteService" << std::endl;
                    std::cout << "Input the Server Address :";
                    std::cin >> REMOTE_SERVER_ADDRESS;
                    NSProviderService::getInstance()->enableRemoteService(REMOTE_SERVER_ADDRESS);
                    break;
                }
            case 14:
                {
                    std::cout << "Disable NS Provider RemoteService" << std::endl;
                    std::cout << "Input the Server Address :";
                    NSProviderService::getInstance()->disableRemoteService(REMOTE_SERVER_ADDRESS);
                    break;
                }
#endif
            case 15:
                {
                    std::cout << "Stop the Notification Provider" << std::endl;
                    NSProviderService::getInstance()->stop();
                    break;
                }
            case 16:
                {
                    std::cout << "Exit()" << std::endl;
                    NSProviderService::getInstance()->stop();
                    isExit = true;
                    break;
                }
            default:
                {
                    std::cout << "Under Construction" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
        }

        std::cout << std::endl;
    }

    return 0;
}
