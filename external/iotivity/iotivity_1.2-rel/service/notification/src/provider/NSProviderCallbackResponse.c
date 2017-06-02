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

#include "NSProviderInterface.h"
#include "NSProviderScheduler.h"
#include "NSProviderListener.h"
#include "NSProviderSubscription.h"
#include "NSProviderNotification.h"
#include "NSProviderMemoryCache.h"
#include "NSProviderCallbackResponse.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cautilinterface.h"
#include "NSProviderSystem.h"
#include "oic_time.h"

static NSSubscribeRequestCallback g_subscribeRequestCb = NULL;
static NSProviderSyncInfoCallback g_syncCb = NULL;

pthread_mutex_t nsInitMutex;

void NSRegisterSubscribeRequestCb(NSSubscribeRequestCallback subscribeRequestCb)
{
    NS_LOG(DEBUG, "NSRegisterSubscribeRequestCb - IN");
    g_subscribeRequestCb = subscribeRequestCb;
    NS_LOG(DEBUG, "NSRegisterSubscribeRequestCb - OUT");
}

void NSRegisterSyncCb(NSProviderSyncInfoCallback syncCb)
{
    NS_LOG(DEBUG, "NSRegisterSyncCb - IN");
    g_syncCb = syncCb;
    NS_LOG(DEBUG, "NSRegisterSyncCb - OUT");
}

void NSSubscribeRequestCb(NSConsumer *consumer)
{
    NS_LOG(DEBUG, "NSSubscribeRequestCb - IN");
    g_subscribeRequestCb(consumer);
    NS_LOG(DEBUG, "NSSubscribeRequestCb - OUT");
}

void NSSyncCb(NSSyncInfo *sync)
{
    NS_LOG(DEBUG, "NSSyncCb - IN");
    g_syncCb(sync);
    NS_LOG(DEBUG, "NSSyncCb - OUT");
}

void * NSCallbackResponseSchedule(void * ptr)
{
    if (ptr == NULL)
    {
        NS_LOG(DEBUG, "Create NSReponseSchedule");
    }

    while (NSIsRunning[CALLBACK_RESPONSE_SCHEDULER])
    {
        sem_wait(&NSSemaphore[CALLBACK_RESPONSE_SCHEDULER]);
        pthread_mutex_lock(&NSMutex[CALLBACK_RESPONSE_SCHEDULER]);

        if (NSHeadMsg[CALLBACK_RESPONSE_SCHEDULER] != NULL)
        {
            NSTask *node = NSHeadMsg[CALLBACK_RESPONSE_SCHEDULER];
            NSHeadMsg[CALLBACK_RESPONSE_SCHEDULER] = node->nextTask;

            switch (node->taskType)
            {
                case TASK_CB_SUBSCRIPTION:
                {
                    NS_LOG(DEBUG, "CASE TASK_CB_SUBSCRIPTION : ");

                    OCEntityHandlerRequest * request = (OCEntityHandlerRequest*)node->taskData;
                    NSConsumer * consumer = (NSConsumer *)OICMalloc(sizeof(NSConsumer));

                    char * copyQuery = OICStrdup(request->query);
                    char * consumerId = NSGetValueFromQuery(copyQuery, NS_QUERY_CONSUMER_ID);

                    if (consumerId)
                    {
                        OICStrcpy(consumer->consumerId, UUID_STRING_SIZE, consumerId);
                        NSSubscribeRequestCb(consumer);
                    }

                    NSOICFree(copyQuery);
                    NSFreeConsumer(consumer);
                    NSFreeOCEntityHandlerRequest(request);

                    break;
                }
                case TASK_CB_SYNC:
                {
                    NS_LOG(DEBUG, "CASE TASK_CB_SYNC : ");
                    NSSyncInfo * sync = (NSSyncInfo*)node->taskData;
                    NSSyncCb(NSDuplicateSync(sync));
                    NSFreeSync(sync);
                    break;
                }
                default:
                    NS_LOG(DEBUG, "No Task Type");
                    break;
            }
            NSOICFree(node);
        }

        pthread_mutex_unlock(&NSMutex[CALLBACK_RESPONSE_SCHEDULER]);
    }

    NS_LOG(DEBUG, "Destroy NSResponseSchedule");
    return NULL;
}

