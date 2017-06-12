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
 * This file provides APIs of Notification Service for common functions.
 */

#ifndef _NS_COMMON_H_
#define _NS_COMMON_H_

#include <stdint.h>
#include <octypes.h>

#define NS_UUID_STRING_SIZE 37
//#define WITH_MQ
/**
 * Result code of notification service
 */
typedef enum eResult
{
    NS_OK = 100,
    NS_ERROR = 200,
    NS_SUCCESS = 300,
    NS_FAIL = 400,

} NSResult;

/**
 * Provider state of notification consumer service
 */
typedef enum
{
    NS_ALLOW = 1,
    NS_DENY = 2,
    NS_TOPIC = 3,
    NS_DISCOVERED = 11,
    NS_STOPPED = 12
} NSProviderState;

/**
 * Notification message status to synchronize
 */
typedef enum
{
    NS_SYNC_UNREAD = 0,
    NS_SYNC_READ = 1,
    NS_SYNC_DELETED = 2,

} NSSyncType;

/**
 * Notification Message Type
 * Alert mean is High / critical
 * Notice mean is low / critical
 * Event mean is High / Normal
 * Information mean is Low / Normal
 */
typedef enum
{
    NS_MESSAGE_ALERT = 1,
    NS_MESSAGE_NOTICE = 2,
    NS_MESSAGE_EVENT = 3,
    NS_MESSAGE_INFO = 4,
    NS_MESSAGE_WARNING = 5,
    NS_MESSAGE_READ = 11,
    NS_MESSAGE_DELETED = 12

} NSMessageType;

/**
 *  Notification topic state
 */
typedef enum
{
    NS_TOPIC_UNSUBSCRIBED = 0,
    NS_TOPIC_SUBSCRIBED = 1,

} NSTopicState;

/**
 * Topic linked list
 */
typedef struct _nsTopic
{
    char * topicName;
    NSTopicState state;
    struct _nsTopic * next;

} NSTopicLL;

/**
 *  Consumer information
 */
typedef struct
{
    char consumerId[NS_UUID_STRING_SIZE];

} NSConsumer;

/**
 *  Provider information
 */
typedef struct
{
    char providerId[NS_UUID_STRING_SIZE];

} NSProvider;

/**
 *  Media Contents of Notification Message (Optional)
 */
typedef struct
{
    char * iconImage;

} NSMediaContents;

/**
 *  Notification Message
 */
typedef struct
{
    //Mandatory
    uint64_t messageId;
    char providerId[NS_UUID_STRING_SIZE];

    //optional
    NSMessageType type;
    char * dateTime;
    uint64_t ttl;
    char * title;
    char * contentText;
    char * sourceName;
    NSMediaContents * mediaContents;
    char * topic;
    OCRepPayload * extraInfo;

} NSMessage;

/**
 *  Synchronization information of the notification message
 */
typedef struct
{
    uint64_t messageId;
    char providerId[NS_UUID_STRING_SIZE];
    NSSyncType state;

} NSSyncInfo;

#endif /* _NS_COMMON_H_ */

