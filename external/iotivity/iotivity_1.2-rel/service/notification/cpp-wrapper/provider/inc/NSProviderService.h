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

/**
 * @file
 *
 * This file provides C++ Wrapper APIs of Notification Service for Provider.
 */

#ifndef _NS_PROVIDER_SERVICE_H_
#define _NS_PROVIDER_SERVICE_H_

#include <string>
#include "NSConsumer.h"
#include "NSSyncInfo.h"
#include "NSMessage.h"
#include "NSUtils.h"
#include "NSTopicsList.h"

namespace OIC
{
    namespace Service
    {
        class NSAcceptedConsumers;
        /**
         * @class   NSProviderService
         * @brief   This class provides a set of C++APIs for Notification Provider.
         */
        class NSProviderService
        {
            public:
                /**
                      * Provider uses this callback function to receive subscription request of consumer
                      * @param[in] consumer  Consumer who subscribes the notification message resource
                      */
                typedef void (*ConsumerSubscribedCallback)(std::shared_ptr<NSConsumer> );

                /**
                      * Provider use this callback function to receive the status of the message
                      * synchronization
                      * @param[in] sync Synchronization information of the notification message
                      */
                typedef void (*MessageSynchronizedCallback)(NSSyncInfo );


                /**
                      * @struct   ProviderConfig
                      * @brief Provider sets this following configuration for registering callbacks and configs
                      *  Set the subControllability, for notification servcie refering to following
                      *  if subControllability, is true, provider decides to allow or deny for all the subscribing consumers.
                      *  Otherwise(subControllability, is false) consumer decides to request subscription to discovered providers.
                      */
                typedef struct
                {
                    /** m_subscribeRequestCb - ConsumerSubscribedCallback callback listener.*/
                    ConsumerSubscribedCallback m_subscribeRequestCb;
                    /** m_syncInfoCb - MessageSynchronizedCallback callback listener.*/
                    MessageSynchronizedCallback m_syncInfoCb;

                    /** subControllability - for setting the subscription controllability for Consumer */
                    bool subControllability;
                    /** userInfo - user defined information such as device friendly name */
                    std::string userInfo;
                    /* Set on/off for secure resource channel setting */
                    bool resourceSecurity;
                } ProviderConfig;

                /**
                      * API for starting the NS Provider
                      *
                      * @return NSProviderService Pointer to singleton instance created
                      */
                static NSProviderService *getInstance();

                /**
                      * Initialize notification service for provider
                      * @param[in]  config   ProviderConfig Callback function pointers to onConsumerSubscribed,
                      * and onMessageSynchronized function listeners
                      * @return :: result code of Provider Service
                      */
                NSResult start(ProviderConfig config);

                /**
                      * Terminate notification service for provider
                      * @return :: result code of Provider Service
                      */
                NSResult stop();

                /**
                      * Request to publish resource to cloud server
                      * @param[in]  serverAddress combined with IP address and port number using delimiter :
                      * @return  result code of Provider Service
                      */
                NSResult enableRemoteService(const std::string &serverAddress);

                /**
                      * Request to cancel remote service using cloud server
                      * @param[in]  serverAddress combined with IP address and port number using delimiter :
                      * @return  result code of Provider Service
                      */
                NSResult disableRemoteService(const std::string &serverAddress);

                /**
                      * Request to subscribe to remote MQ address as parameter.
                      * @param[in] server address combined with IP address and port number and MQ broker uri using delimiter :
                      * @param[in] topicName the interest Topic name for subscription.
                      * @return ::NS_OK or result code of NSResult
                      */
                NSResult subscribeMQService(const std::string &serverAddress, const std::string &topicName);

                /**
                      * Send notification message to all subscribers
                      * @param[in]  msg  Notification message including id, title, contentText
                      * @return :: result code of Provider Service
                      */
                NSResult sendMessage(const NSMessage &msg);


                /**
                      * Send read-check to provider in order to synchronize notification status with other consumers
                      * @param[in]  messageId  ID of Notification message to synchronize the status
                      * @param[in]  type  NotificationSyncType of the SyncInfo message
                      * @return :: OK or result code of NSResult
                      */
                NSResult sendSyncInfo(uint64_t messageId, NSSyncInfo::NSSyncType type);

                /**
                     * Initialize NSMessage class, This function is valid only when subControllability is set true.
                     * @return NSMessage *
                     */
                NSMessage createMessage();

                /**
                     * Add topic to topic list which is located in provider service storage
                     * @param[in]  topicName Topic name to add
                     * @return :: OK or result code of NSResult
                     */
                NSResult registerTopic(const std::string &topicName);

                /**
                     * Delete topic from topic list
                     * @param[in]  topicName Topic name to delete
                     * @return :: OK or result code of NSResult
                     */
                NSResult unregisterTopic(const std::string &topicName);

                /**
                     * Request topics list already registered by provider user
                     * @return :: Topic list
                     */
                std::shared_ptr<NSTopicsList> getRegisteredTopicList();

                /**
                      *  get Provider config values
                      * @return ProviderConfig callbaks set
                      */
                ProviderConfig getProviderConfig();

                /**
                      *  request to get NSConsumer pointer
                      * @param id -id as string
                      *
                      * @return shared pointer to NSConsumer
                      */
                std::shared_ptr<NSConsumer> getConsumer(const std::string &id);

                /**
                      *  get handle of Consumers accepted.
                      * @return m_acceptedConsumers -accepted Consumers
                      */
                NSAcceptedConsumers *getAcceptedConsumers();

            private :
                ProviderConfig m_config;
                NSAcceptedConsumers *m_acceptedConsumers;

            private:
                NSProviderService();
                ~NSProviderService();
                NSProviderService(const NSProviderService &) = delete;
                NSProviderService &operator=(const NSProviderService &) = delete;
                NSProviderService(const NSProviderService &&) = delete;
                NSProviderService &operator=(const NSProviderService && ) = delete;

                ::NSMessage *getNSMessage(const NSMessage &msg);
                static void onConsumerSubscribedCallback(::NSConsumer *consumer);
                static void onMessageSynchronizedCallback(::NSSyncInfo *syncInfo);
        };
    }
}
#endif /* _NS_PROVIDER_SERVICE_H_ */
