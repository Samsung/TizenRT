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
 * This file provides APIs of Notification Service for Provider.
 */

#ifndef _NS_PROVIDER_INTERFACE_H_
#define _NS_PROVIDER_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "NSCommon.h"
#include <stdbool.h>
#include <stdint.h>
/**
 * Invoked when provider receives the subscription request of consumer.
 * @param[in] consumer  Consumer who subscribes the notification message resource
 */
typedef void (*NSSubscribeRequestCallback)(NSConsumer *);

/**
 * Invoked when synchronization data which has notification message
 * read/deleted event from consumer is received.
 * @param[in] sync  Synchronization information of the notification message
 */
typedef void (*NSProviderSyncInfoCallback)(NSSyncInfo *);

/**
 *  Set provider service with the following configuration
 */
typedef struct
{
    /* Invoked when the subscription request from consumer is received */
    NSSubscribeRequestCallback subRequestCallback;
    /* Invoked when the synchronization data, read and deleted, is sent by consumer is received */
    NSProviderSyncInfoCallback syncInfoCallback;
    /* Set the policy for notification servcie which checks whether provider is capable of 
     * denying the subscription of notification message from consumer
     * and getting controllabliity to set consumer topic list.
     * If true, provider is able to control subscription request and consumer topic list.
     * Otherwise(policy is false), consumer can do the same.
     */
    bool subControllability;
    /* User defined information such as device friendly name */
    char * userInfo;
    /* Set on/off for secure resource channel setting */
    bool resourceSecurity;

} NSProviderConfig;

/**
 * Initialize notification service for provider
 * @param[in]  config   Refer to NSProviderConfig
 * @return ::NS_OK if the action is requested succesfully
 */
NSResult NSStartProvider(NSProviderConfig config);

/**
 * Terminate notification service for provider
 * @return ::NS_OK if the action is requested succesfully
 */
NSResult NSStopProvider();

/**
 * Request to publish resource using remote relay server
 * @param[in]  server address combined with IP address and port number using delimiter :
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if wrong parameter is set.
 */
NSResult NSProviderEnableRemoteService(char * serverAddress);

/**
 * Request to terminate remote service from relay server
 * @param[in]  server address combined with IP address and port number using delimiter :
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if wrong parameter is set.
 */
NSResult NSProviderDisableRemoteService(char * serverAddress);

#ifdef WITH_MQ
/**
 * Request to subscribe to remote MQ address as parameter.
 * @param[in] server address combined with IP address and port number and MQ broker uri using delimiter :
 * @param[in] topicName the interest Topic name for subscription.
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSProviderSubscribeMQService(const char * serverAddress, const char * topicName);
#endif

/**
 * Send notification message to all subscribers
 * @param[in]  message  Notification message including id, title, contentText
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if wrong parameter is set.
 */
NSResult NSSendMessage(NSMessage * msg);

/**
 * Send acceptance to consumer who subscribes the resource of notification message
 * This function is valid only when subControllability is set true.
 * @param[in]  consumer  Consumer who subscribes the resource
 * @param[in]  accepted  the result of acceptance; ALLOW or DENY
 * @return ::NS_OK if this function is requested succesfully
 * or NS_FAIL if subContollability is false.
 */
NSResult NSAcceptSubscription(const char * consumerId, bool accepted);

/**
 * Send synchronizad state of notificaion message to consumers
 * @param[in]  messageiId  ID of notification message
 * @param[in]  type  SyncType of the syncInfo message
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if wrong parameter is set.
 */
NSResult NSProviderSendSyncInfo(uint64_t messageId, NSSyncType type);

/**
 * Initialize NSMessage struct.
 * Service sets mandatory fields which message id and provider(device) id are filled with.
 * @return ::NSMessage *
 */
NSMessage * NSCreateMessage();

/**
 * Add topic to topic list which is located in provider service storage
 * @param[in]  topicName Topic name to add
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if wrong parameter is set.
 */
NSResult NSProviderRegisterTopic(const char * topicName);

/**
 * Delete topic from topic list
 * @param[in]  topicName Topic name to delete
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if wrong parameter is set.
 */
NSResult NSProviderUnregisterTopic(const char * topicName);

/**
 * Select a topic name for a consumer
 * @param[in]  consumerId  consumer id for which the user on provider selects a topic
 * @param[in]  topicName Topic name to select
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if subContollability is false
 */
NSResult NSProviderSetConsumerTopic(const char * consumerId, const char * topicName);

/**
 * Unselect a topic from the topic list for consumer
 * @param[in]  consumerId  consumer id for which the user on provider unselects a topic
 * @param[in]  topicName Topic name to unselect
 * @return ::NS_OK if the action is requested succesfully or NS_FAIL if subContollability is false
 */
NSResult NSProviderUnsetConsumerTopic(const char * consumerId, const char * topicName);

/**
 * Request topic list with selection state for the consumer
 * @param[in] consumerid  the id of consumer which topic list is subscribed for
 * @return :: Topic list
 */
NSTopicLL * NSProviderGetConsumerTopics(const char * consumerId);

/**
 * Request topics list already registered by provider user
 * @return :: Topic list
 */
NSTopicLL * NSProviderGetTopics();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _NS_PROVIDER_INTERFACE_H_ */

