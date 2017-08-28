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

#include "NSConsumerService.h"
#include <cstring>
#include "NSConsumerInterface.h"
#include "NSAcceptedProviders.h"
#include "NSMessage.h"
#include "NSCommon.h"
#include "NSConstants.h"
#include "oic_string.h"
#include "oic_malloc.h"

namespace OIC
{
    namespace Service
    {
        void NSConsumerService::onProviderStateReceived(::NSProvider *provider, ::NSProviderState state)
        {
            NS_LOG(DEBUG, "onNSProviderStateChanged - IN");
            NS_LOG_V(INFO_PRIVATE, "provider Id : %s", provider->providerId);
            NS_LOG_V(DEBUG, "state : %d", (int)state);

            std::string provId;
            provId.assign(provider->providerId, NS_UTILS_UUID_STRING_SIZE - 1);
            std::shared_ptr<NSProvider> oldProvider =
                NSConsumerService::getInstance()->getProvider(provId);

            if (oldProvider == nullptr)
            {
                //Callback received for First time from new Provider as there is no reference to
                //previous Provider with same ID in internal map.
                //States received during discovery of Provider : NS_DISCOVERED and NS_ALLOW
                std::shared_ptr<NSProvider> nsProvider = std::make_shared<NSProvider>(provider);
                NS_LOG(DEBUG, "Provider with same Id do not exist. updating Data for New Provider");
                auto discoveredCallback = NSConsumerService::getInstance()->getProviderDiscoveredCb();
                nsProvider->setProviderState((NSProviderState)state);
                auto topicLL = NSConsumerGetTopicList(provider->providerId);
                nsProvider->setTopicList(std::make_shared<NSTopicsList>(topicLL, false));
                NSConsumerService::getInstance()->getAcceptedProviders()->addProvider(nsProvider);
                if (state == NS_DISCOVERED)
                {
                    nsProvider->setProviderSubscribedState(NSProviderSubscribedState::DISCOVERED);
                    if (discoveredCallback != NULL)
                    {
                        NS_LOG(DEBUG,
                               "initiating the Discovered callback : NS_DISCOVERED, policy false, Accepter: Consumer");
                        discoveredCallback(nsProvider);
                    }
                }
                else if (state == NS_ALLOW)
                {
                    nsProvider->setProviderSubscribedState(NSProviderSubscribedState::SUBSCRIBED);
                    if (discoveredCallback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the Discovered callback : NS_ALLOW, policy true, Accepter: Provider");
                        discoveredCallback(nsProvider);
                    }
                }
            }
            else
            {
                //Callback received from already discovered and existing Provider in internal map.
                //States received Post discovery of Provider.
                NS_LOG(DEBUG, "Provider with same Id exists. updating the old Provider data");
                auto changeCallback = oldProvider->getProviderStateReceivedCb();
                auto prevState = oldProvider->getProviderState();
                oldProvider->setProviderState((NSProviderState)state);
                if (state == NS_ALLOW)
                {
                    oldProvider->setProviderSubscribedState(NSProviderSubscribedState::SUBSCRIBED);
                    if (changeCallback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the callback for Response : NS_ALLOW");
                        changeCallback((NSProviderState)state);
                    }
                    else
                    {
                        oldProvider->setProviderSubscribedState(NSProviderSubscribedState::SUBSCRIBED);
                        auto discoveredCallback = NSConsumerService::getInstance()->getProviderDiscoveredCb();
                        if (discoveredCallback != NULL)
                        {
                            discoveredCallback(oldProvider);
                        }
                        auto changeCallback = oldProvider->getProviderStateReceivedCb();
                        if (changeCallback != NULL)
                        {
                            changeCallback(prevState);
                        }
                    }
                }
                else if (state == NS_DENY)
                {
                    oldProvider->setProviderSubscribedState(NSProviderSubscribedState::DENY);
                    NSConsumerService::getInstance()->getAcceptedProviders()->removeProvider(
                        oldProvider->getProviderId());
                    if (changeCallback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the callback for Response : NS_DENY");
                        changeCallback((NSProviderState)state);
                    }
                }
                else if (state == NS_TOPIC)
                {
                    auto topicLL = NSConsumerGetTopicList(provider->providerId);
                    oldProvider->setTopicList(std::make_shared<NSTopicsList>(topicLL, false));
                    if (changeCallback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the callback for Response : NS_TOPIC");
                        changeCallback((NSProviderState)state);
                    }
                    if (topicLL)
                    {
                        NSTopicLL *iter = topicLL;
                        NSTopicLL *following = NULL;

                        while (iter)
                        {
                            following = iter->next;
                            if (iter)
                            {
                                NSOICFree(iter->topicName);
                                iter->next = NULL;
                                NSOICFree(iter);
                            }
                            iter = following;
                        }
                    }
                }
                else if (state == NS_STOPPED)
                {
                    oldProvider->setProviderSubscribedState(NSProviderSubscribedState::DENY);
                    oldProvider->getTopicList()->unsetModifiability();
                    NSConsumerService::getInstance()->getAcceptedProviders()->removeProvider(
                        oldProvider->getProviderId());
                    NS_LOG(DEBUG, "initiating the State callback : NS_STOPPED");
                    if (changeCallback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the callback for Response : NS_STOPPED");
                        changeCallback((NSProviderState)state);
                    }
                }
            }
            NS_LOG(DEBUG, "onNSProviderStateChanged - OUT");
        }

        void NSConsumerService::onNSMessageReceived(::NSMessage *message)
        {
            NS_LOG(DEBUG, "onNSMessageReceived - IN");
            NS_LOG_V(INFO_PRIVATE, "message->providerId : %s", message->providerId);

            NSMessage nsMessage(message);

            if (NSConsumerService::getInstance()->getAcceptedProviders()->isAccepted(
                    nsMessage.getProviderId()))
            {
                auto provider = NSConsumerService::getInstance()->getProvider(
                                    nsMessage.getProviderId());
                if (provider != nullptr)
                {
                    NS_LOG(DEBUG, "Found Provider with given ID");
                    auto callback = provider->getMessageReceivedCb();
                    if (callback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the callback for messageReceived");
                        callback(nsMessage);
                    }
                }
            }
            NS_LOG(DEBUG, "onNSMessageReceived - OUT");
        }

        void NSConsumerService::onNSSyncInfoReceived(::NSSyncInfo *syncInfo)
        {
            NS_LOG(DEBUG, "onNSSyncInfoReceived - IN");
            NS_LOG_V(INFO_PRIVATE, "syncInfo->providerId : %s", syncInfo->providerId);

            NSSyncInfo nsSyncInfo(syncInfo);

            if (NSConsumerService::getInstance()->getAcceptedProviders()->isAccepted(
                    nsSyncInfo.getProviderId()))
            {
                auto provider = NSConsumerService::getInstance()->getProvider(
                                    nsSyncInfo.getProviderId());
                if (provider != nullptr)
                {
                    NS_LOG(DEBUG, "Found Provider with given ID");
                    auto callback = provider->getSyncInfoReceivedCb();
                    if (callback != NULL)
                    {
                        NS_LOG(DEBUG, "initiating the callback for SyncInfoReceived");
                        callback(nsSyncInfo);
                    }
                }
            }
            NS_LOG(DEBUG, "onNSSyncInfoReceived - OUT");
        }

        NSConsumerService::NSConsumerService()
        {
            m_providerDiscoveredCb = NULL;
            m_acceptedProviders = new NSAcceptedProviders();
        }

        NSConsumerService::~NSConsumerService()
        {
            m_acceptedProviders->removeProviders();
            delete m_acceptedProviders;
        }

        NSConsumerService *NSConsumerService::getInstance()
        {
            static  NSConsumerService s_instance;
            return &s_instance;
        }

        NSResult NSConsumerService::start(NSConsumerService::ProviderDiscoveredCallback providerDiscovered)
        {
            NS_LOG(DEBUG, "start - IN");
            m_acceptedProviders->removeProviders();

            m_providerDiscoveredCb = providerDiscovered;
            NSConsumerConfig nsConfig;
            nsConfig.changedCb = onProviderStateReceived;
            nsConfig.messageCb = onNSMessageReceived;
            nsConfig.syncInfoCb = onNSSyncInfoReceived;

            NSResult result = (NSResult) NSStartConsumer(nsConfig);
            NS_LOG(DEBUG, "start - OUT");
            return result;
        }

        NSResult NSConsumerService::stop()
        {
            NS_LOG(DEBUG, "stop - IN");
            m_providerDiscoveredCb = NULL;
            m_acceptedProviders->removeProviders();

            NSResult result = (NSResult) NSStopConsumer();
            NS_LOG(DEBUG, "stop - OUT");
            return result;
        }

        NSResult NSConsumerService::enableRemoteService(const std::string &serverAddress)
        {
            NS_LOG(DEBUG, "enableRemoteService - IN");
            NS_LOG_V(INFO_PRIVATE, "Server Address : %s", serverAddress.c_str());
            NSResult result = NSResult::ERROR;
#ifdef WITH_CLOUD
            result = (NSResult) NSConsumerEnableRemoteService(OICStrdup(serverAddress.c_str()));
#else
            NS_LOG(ERROR, "Remote Services feature is not enabled in the Build");
            (void) serverAddress;
#endif
            NS_LOG(DEBUG, "enableRemoteService - OUT");
            return result;
        }

        NSResult NSConsumerService::subscribeMQService(const std::string &serverAddress,
                const std::string &topicName)
        {
            NS_LOG(DEBUG, "subscribeMQService - IN");
            NS_LOG_V(INFO_PRIVATE, "Server Address : %s", serverAddress.c_str());
            NSResult result = NSResult::ERROR;
#ifdef WITH_MQ
            result = (NSResult) NSConsumerSubscribeMQService(
                         serverAddress.c_str(), topicName.c_str());
#else
            NS_LOG(ERROR, "MQ Services feature is not enabled in the Build");
            (void) serverAddress;
            (void) topicName;
#endif
            NS_LOG(DEBUG, "subscribeMQService - OUT");
            return result;
        }

        NSResult NSConsumerService::rescanProvider()
        {
            NS_LOG(DEBUG, "rescanProvider - IN");
            NSResult result = (NSResult) NSRescanProvider();
            NS_LOG(DEBUG, "rescanProvider - OUT");
            return result;
        }

        NSConsumerService::ProviderDiscoveredCallback NSConsumerService::getProviderDiscoveredCb()
        {
            return m_providerDiscoveredCb;
        }

        std::shared_ptr<NSProvider> NSConsumerService::getProvider(const std::string &id)
        {
            return m_acceptedProviders->getProvider(id);
        }

        NSAcceptedProviders *NSConsumerService::getAcceptedProviders()
        {
            return m_acceptedProviders;
        }
    }
}
