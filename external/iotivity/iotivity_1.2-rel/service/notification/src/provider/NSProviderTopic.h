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

#ifndef _NS_PROVIDER_TOPIC_H_
#define _NS_PROVIDER_TOPIC_H_

#include "NSCommon.h"
#include "NSStructs.h"
#include "NSConstants.h"
#include "NSProviderScheduler.h"
#include "NSProviderResource.h"
#include "NSProviderSubscription.h"

typedef struct {
    pthread_cond_t * condition;
    pthread_mutex_t * mutex;
    char * consumerId;
    NSTopicLL * topics;
} NSTopicSync;

typedef struct {
    pthread_cond_t * condition;
    pthread_mutex_t * mutex;
    void * topicData;
    NSResult result;
} NSTopicSyncResult;

NSCacheList * consumerTopicList;
NSCacheList * registeredTopicList;

NSResult NSInitTopicList();
NSTopicList * NSGetTopics(char *consumerId);

bool NSIsTopicSubscribed(char *consumerId, char *topic);

#endif /* _NS_PROVIDER_TOPIC_H_ */
