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
 * This file provides APIs of Notification Service for Consumer.
 */

#ifndef _NS_CONSUMER_INTERFACE_H_
#define _NS_CONSUMER_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "NSCommon.h"

/**
 * Invoked when the provider state is changed
 * @param[in] provider  Provider which has the notification resource
 * @param[in] state  Response which has the provider state
 */
typedef void (* NSProviderStateCallback)(NSProvider *, NSProviderState);

/**
 * Invoked when the notification message from provider is received
 * synchronization
 * @param[in] message  Notification message
 */
typedef void (* NSMessageReceivedCallback)(NSMessage *);

/**
 * Invoked when the synchronization data which has notification message
 * read/delete event from provider/consumer is received
 * synchronization
 * @param[in] syncInfo  Synchronization information of the notification message
 */
typedef void (* NSSyncInfoReceivedCallback)(NSSyncInfo *);

typedef struct
{
    NSProviderStateCallback changedCb;
    NSMessageReceivedCallback messageCb;
    NSSyncInfoReceivedCallback syncInfoCb;

} NSConsumerConfig;

/**
 * Initialize notification service for consumer
 * @param[in] config  NSConsumerconfig structure of callback functions
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSStartConsumer(NSConsumerConfig config);

/**
 * Terminate notification service for consumer
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSStopConsumer();

/**
 * Request to discover to remote address as parameter.
 * @param[in] server address combined with IP address and port number using delimiter :
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSConsumerEnableRemoteService(const char * serverAddress);

#ifdef WITH_MQ
/**
 * Request to subscribe to remote MQ address as parameter.
 * @param[in] server address combined with IP address and port number and MQ broker uri using delimiter :
 * @param[in] topicName the interest MQ Topic name for subscription.
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSConsumerSubscribeMQService(const char * serverAddress, const char * topicName);
#endif

/**
 * Request discovery manually
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSRescanProvider();

/**
 * Request to subscribe notification message resource of provider
 * @param[in] providerId the Id of Provider who send the notification message
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSSubscribe(const char * providerId);

/**
 * Request to unsubscribe in order not to receive notification message from provider
 * @param[in]  providerId  the Id of Provider who send the notification message
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSUnsubscribe(const char * providerId);

/**
 * Send sync type to provider in order to synchronize notification status with other consumers
 * when consumer consumes the notification such as READ, DELETE
 * @param[in] providerId Provider id of the Notification message
 * @param[in] messageId Notification message id to synchronize the status
 * @param[in] type changed notification status from NSSyncType
 * @return ::NS_OK or result code of NSResult
 */
NSResult NSConsumerSendSyncInfo(
        const char * providerId, uint64_t messageId, NSSyncType type);

/**
 * Request NSProvider that is matched by provider id
 * @param[in] providerId the id of provider that user wants to get
 * @return NSProvider
 */
NSProvider * NSConsumerGetProvider(const char * providerId);

/**
 * Request NSTopic list that is subscribed from provider
 * @param[in] providerId the Id of provider that user wants to get
 * @return NSResult
 */
NSTopicLL * NSConsumerGetTopicList(const char * providerId);

/**
 * Select Topic list that is wanted to subscribe from provider
 * @param[in] providerId the Id of provider that user wants to set
 * @param[in] topics the topic list that user wants to set
 * @return NSResult
 */
NSResult NSConsumerUpdateTopicList(const char * providerId, NSTopicLL * topics);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _NS_CONSUMER_INTERFACE_H_
