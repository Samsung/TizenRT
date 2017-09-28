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

#include <iostream>
#include <limits>

#include <unistd.h>
#include "NSConsumerService.h"
#include "NSMessage.h"
#include "NSProvider.h"
#include "NSTopicsList.h"
#include "ocstack.h"

#define TAG "NotiConsumerWrapperExample"
using namespace std;
using namespace OIC::Service;

bool isExit = false;
std::string REMOTE_SERVER_ADDRESS;
std::string mainProvider;
uint64_t mainMessageId = 0;

#ifndef OC_SECURITY_DB_DAT_FILE_NAME
#define OC_SECURITY_DB_DAT_FILE_NAME "oic_svr_db.dat"
#endif

FILE *server_fopen(const char *path, const char *mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen("oic_ns_provider_db.dat", mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

void onNotificationPostedCb(OIC::Service::NSMessage notification)
{
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Message Received " << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "id : " << notification.getMessageId() << std::endl;
    std::cout << "title : " << notification.getTitle() << std::endl;
    std::cout << "content : " <<  notification.getContentText() << std::endl;
    std::cout << "source : " <<  notification.getSourceName() << std::endl;
    std::cout << "topic : " <<  notification.getTopic() << std::endl;
    std::cout << "type : " <<  (int) notification.getType() << std::endl;
    std::cout << "TTL : " <<  notification.getTTL() << std::endl;
    std::cout << "time : " <<  notification.getTime() << std::endl;
    if (notification.getMediaContents() != nullptr)
    {
        std::cout << "MediaContents IconImage : " <<  notification.getMediaContents()->getIconImage()
                  << std::endl;
    }
    std::cout << "ExtraInfo " << std::endl;
    OC::OCRepresentation rep = notification.getExtraInfo();
    for (auto it : rep.getResourceTypes())
    {
        std::cout << "resourceType : " << it << std::endl;
    }
    for (auto it : rep.getResourceInterfaces())
    {
        std::cout << "Interface : " << it << std::endl;
    }
    for (auto it : rep.getValues())
    {
        std::cout << "Key : " << it.first << std::endl;
    }
    mainMessageId = notification.getMessageId();
}

void onNotificationSyncCb(OIC::Service::NSSyncInfo sync)
{
    std::cout << "------------------------------------" << std::endl;
    std::cout << "SyncInfo Received " << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Sync ID : " <<  sync.getMessageId() << std::endl;
    std::cout << "Provider ID : " <<  sync.getProviderId() << std::endl;
    std::cout << "Sync STATE : " << (int) sync.getState() << std::endl;
}

void onProviderStateChangedCb(OIC::Service::NSProviderState state)
{
    std::cout << "onProviderStateChangedCb" << std::endl;
    if (state == OIC::Service::NSProviderState::ALLOW)
    {
        std::cout << "Provider Subscription Accepted" << std::endl;
    }
    else if (state == OIC::Service::NSProviderState::DENY)
    {
        std::cout << "Provider Subscription Denied" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
    else if (state == OIC::Service::NSProviderState::TOPIC)
    {
        std::shared_ptr<OIC::Service::NSProvider> provider =
            NSConsumerService::getInstance()->getProvider(mainProvider);
        if (provider != nullptr)
        {
            auto topicList = provider->getTopicList();
            if (topicList != nullptr)
            {
                for (auto it : topicList->getTopicsList())
                {
                    std::cout << "Topic Name: " << it.getTopicName() << std::endl;
                    std::cout << "Topic state: " << (int) it.getState() << std::endl;
                }
            }
        }
    }
    else if (state == OIC::Service::NSProviderState::STOPPED)
    {
        std::cout << "Provider Stopped" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
}

void onDiscoverNotificationCb(std::shared_ptr<OIC::Service::NSProvider> provider)
{
    std::cout << "Notification Resource Discovered" << std::endl;
    std::cout << "SetListeners for callbacks" << std::endl;
    std::cout << "ProviderID : " << provider->getProviderId() << std::endl;
    provider->setListener(onProviderStateChangedCb, onNotificationPostedCb, onNotificationSyncCb);
    if (!provider->isSubscribed())
    {
        std::cout << "startSubscribing" << std::endl;
        provider->subscribe();
    }
    if (mainProvider.empty())
    {
        mainProvider = provider->getProviderId();
    }
}

void *OCProcessThread(void *ptr)
{
    (void) ptr;

    while (!isExit)
    {
        usleep(2000);
        if (OCProcess() != OC_STACK_OK)
        {
            OCStop();
            break;
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t OCThread = 0;

    std::cout << "start Iotivity" << std::endl;

    // open oic_db
    static OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};
    OCRegisterPersistentStorageHandler(&ps);

    if (OCInit1(OC_CLIENT_SERVER, OC_DEFAULT_FLAGS, OC_DEFAULT_FLAGS) != OC_STACK_OK)
    {
        std::cout << "OCInit fail" << std::endl;
        return 0;
    }

    pthread_create(&OCThread, NULL, OCProcessThread, NULL);

    std::cout << "Start notification consumer service" << std::endl;
    while (!isExit)
    {
        int num = 0;

        std::cout << "1. Start Consumer" << std::endl;
        std::cout << "2. Stop Consumer" << std::endl;
        std::cout << "3. SendSyncInfo" << std::endl;
        std::cout << "4. GetTopicList" << std::endl;
        std::cout << "5. UpdateTopicList" << std::endl;
        std::cout << "6. Subscribe provider" << std::endl;
        std::cout << "7. UnSubscribe provider" << std::endl;
        std::cout << "8. Rescan provider" << std::endl;
#ifdef WITH_CLOUD
        std::cout << "9. Enable  NS Consumer RemoteService" << std::endl;
#endif
        std::cout << "10. Exit" << std::endl;

        std::cout << "Input: " << std::endl;
        std::cin >> num;
        switch (num)
        {
            case 1:
                {
                    std::cout << "Start the Notification Consumer" << std::endl;
                    NSConsumerService::getInstance()->start(onDiscoverNotificationCb);
                    break;
                }
            case 2:
                {
                    std::cout << "Stop the Notification Consumer" << std::endl;
                    NSConsumerService::getInstance()->stop();
                    break;
                }
            case 3:
                {
                    std::cout <<  "SendSyncInfo" << std::endl;
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
                                auto provider = NSConsumerService::getInstance()->getProvider(
                                                    mainProvider);
                                if (provider != nullptr)
                                {
                                    provider->sendSyncInfo(mainMessageId,
                                                           OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ);
                                }
                            }
                            break;
                        case 2:
                            {
                                std::cout << "Sending Delete Sync" << std::endl;
                                auto provider = NSConsumerService::getInstance()->getProvider(
                                                    mainProvider);
                                if (provider != nullptr)
                                {
                                    provider->sendSyncInfo(mainMessageId,
                                                           OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_DELETED);
                                }
                            }
                            break;
                        default:
                            {
                                cout << "Invalid Input!. sending default Read Sync";
                                auto provider = NSConsumerService::getInstance()->getProvider(
                                                    mainProvider);
                                if (provider != nullptr)
                                {
                                    provider->sendSyncInfo(mainMessageId,
                                                           OIC::Service::NSSyncInfo::NSSyncType::NS_SYNC_READ);
                                }
                                std::cin.clear();
                                std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                break;
                            }
                    }
                    break;
                }
            case 4:
                {
                    std::cout <<  "GetTopicList" << std::endl;
                    std::shared_ptr<OIC::Service::NSProvider> provider =
                        NSConsumerService::getInstance()->getProvider(mainProvider);
                    if (provider != nullptr)
                    {
                        auto topicList = provider->getTopicList();
                        if (topicList != nullptr)
                        {
                            for (auto it : topicList->getTopicsList())
                            {
                                std::cout << "Topic Name: " << it.getTopicName() << std::endl;
                                std::cout << "Topic state: " << (int) it.getState() << std::endl;
                            }
                        }
                    }
                    break;
                }
            case 5:
                {
                    std::cout <<  "UpdateTopicList" << std::endl;
                    std::shared_ptr<OIC::Service::NSProvider> provider =
                        NSConsumerService::getInstance()->getProvider(mainProvider);
                    if (provider != nullptr)
                    {
                        std::shared_ptr<NSTopicsList> topicList = std::make_shared<NSTopicsList>();
                        topicList->addTopic("OCF_TOPIC1", NSTopic::NSTopicState::SUBSCRIBED);
                        topicList->addTopic("OCF_TOPIC2", NSTopic::NSTopicState::SUBSCRIBED);
                        topicList->addTopic("OCF_TOPIC3", NSTopic::NSTopicState::UNSUBSCRIBED);

                        provider->updateTopicList(topicList);
                    }
                    break;
                }
            case 6:
                {
                    std::cout << "Subscribe provider" << std::endl;
                    if (!mainProvider.empty())
                    {
                        std::shared_ptr<OIC::Service::NSProvider> provider =
                            NSConsumerService::getInstance()->getProvider(mainProvider);
                        if (provider != nullptr )
                        {
                            std::cout << "calling Subscribe on discovered mainProvider" << std::endl;
                            if (!provider->isSubscribed())
                            {
                                std::cout << "start Subscribing" << std::endl;
                                provider->subscribe();
                            }
                        }
                    }
                    break;
                }
            case 7:
                {
                    std::cout << "UnSubscribe provider" << std::endl;
                    if (!mainProvider.empty())
                    {
                        std::shared_ptr<OIC::Service::NSProvider> provider =
                            NSConsumerService::getInstance()->getProvider(mainProvider);
                        if (provider != nullptr )
                        {
                            std::cout << "calling UnSubscribe on discovered mainProvider" << std::endl;
                            if (provider->isSubscribed())
                            {
                                std::cout << "start UnSubscribing" << std::endl;
                                provider->unsubscribe();
                            }
                        }
                    }
                    break;
                }
            case 8:
                {
                    std::cout << "Rescan Provider" << std::endl;
                    NSConsumerService::getInstance()->rescanProvider();
                    break;
                }

#ifdef WITH_CLOUD
            case 9:
                {
                    std::cout << "Enable NS Consumer RemoteService" << std::endl;
                    std::cout << "Input the Server Address :";
                    std::cin >> REMOTE_SERVER_ADDRESS;
                    NSConsumerService::getInstance()->enableRemoteService(REMOTE_SERVER_ADDRESS);
                    break;
                }
#endif
            case 10:
                {
                    std::cout << "Exit" << std::endl;
                    NSConsumerService::getInstance()->stop();
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
    }

    return 0;
}
