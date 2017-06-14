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

#include "NSConsumerScheduler.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "oic_malloc.h"
#include "oic_string.h"
#include "ocrandom.h"

#include "NSStructs.h"
#include "NSConstants.h"
#include "NSConsumerCommon.h"
#include "NSConsumerCommunication.h"

#include "NSThread.h"
#include "NSConsumerQueue.h"

#include "NSConsumerDiscovery.h"
#include "NSConsumerInternalTaskController.h"
#include "NSConsumerNetworkEventListener.h"
#include "NSConsumerSystem.h"

#ifdef WITH_MQ
#include "NSConsumerMQPlugin.h"
#endif

void * NSConsumerMsgHandleThreadFunc(void * handle);

void * NSConsumerMsgPushThreadFunc(void * data);

void NSConsumerTaskProcessing(NSTask * task);

NSConsumerThread ** NSGetMsgHandleThreadHandle()
{
    static NSConsumerThread * handle = NULL;
    return & handle;
}

void NSSetMsgHandleThreadHandle(NSConsumerThread * handle)
{
   *(NSGetMsgHandleThreadHandle()) = handle;
}

NSConsumerQueue ** NSGetMsgHandleQueue()
{
    static NSConsumerQueue * queue = NULL;
    return & queue;
}

void NSSetMsgHandleQueue(NSConsumerQueue * queue)
{
   *(NSGetMsgHandleQueue()) = queue;
}

NSResult NSConsumerMessageHandlerInit()
{
    NSConsumerThread * handle = NULL;
    NSConsumerQueue * queue = NULL;

    char * consumerUuid = (char *)OCGetServerInstanceIDString();
    NS_VERIFY_NOT_NULL(consumerUuid, NS_ERROR);

    NSSetConsumerId(consumerUuid);
    NS_LOG_V(INFO_PRIVATE, "Consumer ID : %s", *NSGetConsumerId());

    NS_LOG(DEBUG, "listener init");
    NSResult ret = NSConsumerListenerInit();
    NS_VERIFY_NOT_NULL(ret == NS_OK ? (void *) 1 : NULL, NS_ERROR);

    NS_LOG(DEBUG, "system init");
    ret = NSConsumerSystemInit();
    NS_VERIFY_NOT_NULL(ret == NS_OK ? (void *) 1 : NULL, NS_ERROR);

    NS_LOG(DEBUG, "create queue");
    queue = NSCreateQueue();
    NS_VERIFY_NOT_NULL(queue, NS_ERROR);
    NSSetMsgHandleQueue(queue);

    NS_LOG(DEBUG, "queue thread init");
#ifndef __TIZENRT__
    handle = NSThreadInit(NSConsumerMsgHandleThreadFunc, NULL);
#else
    handle = NSThreadInit(NSConsumerMsgHandleThreadFunc, NULL, "NSConsumerMsgHandleThreadFunc", 1024);
#endif
    NS_VERIFY_NOT_NULL(handle, NS_ERROR);
    NSSetMsgHandleThreadHandle(handle);

    return NS_OK;
}

NSResult NSConsumerPushEvent(NSTask * task)
{
#ifndef __TIZENRT__
    NSConsumerThread * thread = NSThreadInit(NSConsumerMsgPushThreadFunc, (void *) task);
#else
    NSConsumerThread * thread = NSThreadInit(NSConsumerMsgPushThreadFunc, (void *) task, "NSConsumerMsgPushThreadFunc", 1024);
#endif
    NS_VERIFY_NOT_NULL(thread, NS_ERROR);

    NSDestroyThreadHandle(thread);
    NSOICFree(thread);

    return NS_OK;
}

void NSConsumerMessageHandlerExit()
{

    NSConsumerListenerTermiate();
    NSCancelAllSubscription();

    NSConsumerThread * thread = *(NSGetMsgHandleThreadHandle());
    NSThreadStop(thread);
    NSSetMsgHandleThreadHandle(NULL);

    NSConsumerQueue * queue = *(NSGetMsgHandleQueue());
    NSDestroyQueue(queue);
    NSSetMsgHandleQueue(NULL);

    NSDestroyInternalCachedList();
}

void * NSConsumerMsgHandleThreadFunc(void * threadHandle)
{
    NSConsumerQueue * queue = *(NSGetMsgHandleQueue());;
    NSConsumerQueueObject * obj = NULL;

    NS_LOG(DEBUG, "create thread for consumer message handle");
    NSConsumerThread * queueHandleThread = (NSConsumerThread *) threadHandle;
    NS_VERIFY_NOT_NULL(queueHandleThread, NULL);

    while (true)
    {
        if (!queue)
        {
            queue = *(NSGetMsgHandleQueue());
            usleep(2000);
            continue;
        }

        if (!queueHandleThread->isStarted && NSIsQueueEmpty(queue))
        {
            NS_LOG(ERROR, "msg handler thread will be terminated");
            break;
        }

        if (NSIsQueueEmpty(queue))
        {
            usleep(2000);
            continue;
        }

        NSThreadLock(queueHandleThread);
        NS_LOG(DEBUG, "msg handler working");
        obj = NSPopQueue(queue);

        if (obj)
        {
            NSConsumerTaskProcessing((NSTask *)(obj->data));
            NSOICFree(obj);
        }

        NSThreadUnlock(queueHandleThread);

    }

    return NULL;
}

void * NSConsumerMsgPushThreadFunc(void * data)
{
    NSConsumerQueueObject * obj = NULL;
    NSConsumerQueue * queue = NULL;

    NS_LOG(DEBUG, "get queueThread handle");
    NSConsumerThread * msgHandleThread = *(NSGetMsgHandleThreadHandle());
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(msgHandleThread, NULL, NSOICFree(data));

    NS_LOG(DEBUG, "create queue object");
    obj = (NSConsumerQueueObject *)OICMalloc(sizeof(NSConsumerQueueObject));
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(obj, NULL, NSOICFree(data));

    obj->data = data;
    obj->next = NULL;

    NSThreadLock(msgHandleThread);

    queue = *(NSGetMsgHandleQueue());
    if (!queue)
    {
        NS_LOG(ERROR, "NSQueue is null. can not insert to queue");
        NSOICFree(data);
        NSOICFree(obj);
    }
    else
    {
        NSPushConsumerQueue(queue, obj);
    }

    NSThreadUnlock(msgHandleThread);

    return NULL;
}

void NSProviderDeletedPostClean(
        NSTask * task, NSProvider_internal * prov1, NSProvider_internal * prov2)
{
    if (task && task->taskData)
    {
        if (task->taskType == TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL)
        {
            NSRemoveProvider((NSProvider *) task->taskData);
        }
        else if (task->taskType == TASK_CONSUMER_PROVIDER_DELETED)
        {
            NSOICFree(task->taskData);
        }
        NSOICFree(task);
    }

    if (prov1)
    {
        NSRemoveProvider_internal(prov1);
    }

    if (prov2)
    {
        NSRemoveProvider_internal(prov2);
    }
}

void NSConsumerTaskProcessing(NSTask * task)
{
    switch (task->taskType)
    {
        case TASK_EVENT_CONNECTED:
        case TASK_EVENT_CONNECTED_TCP:
        case TASK_CONSUMER_REQ_DISCOVER:
        {
            NSConsumerDiscoveryTaskProcessing(task);
            break;
        }
        case TASK_CONSUMER_REQ_SUBSCRIBE:
        {
            NSProvider_internal * prov =
                    NSConsumerFindNSProvider(((NSProvider *)task->taskData)->providerId);
            NS_VERIFY_NOT_NULL_V(prov);
            NSTask * subTask = NSMakeTask(TASK_CONSUMER_REQ_SUBSCRIBE, prov);
            NS_VERIFY_NOT_NULL_V(subTask);
            NSConsumerCommunicationTaskProcessing(subTask);

            NSRemoveProvider((NSProvider *)task->taskData);
            NSOICFree(task);
            break;
        }
        case TASK_SEND_SYNCINFO:
        case TASK_CONSUMER_REQ_TOPIC_LIST:
        case TASK_CONSUMER_SELECT_TOPIC_LIST:
        {
            NSConsumerCommunicationTaskProcessing(task);
            break;
        }
        case TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL:
        case TASK_CONSUMER_PROVIDER_DELETED:
        {
            NSProvider_internal * data = NULL;

            if (task->taskType == TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL)
            {
                data = NSConsumerFindNSProvider(((NSProvider *) task->taskData)->providerId);
                NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(
                        data, NSProviderDeletedPostClean(task, NULL, NULL));
            }
            else if (task->taskType == TASK_CONSUMER_PROVIDER_DELETED)
            {
                data = NSFindProviderFromAddr((OCDevAddr *) task->taskData);
                NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(
                        data, NSProviderDeletedPostClean(task, NULL, NULL));
            }

            NSProvider_internal * data2 = NSCopyProvider_internal(data);
            NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(
                        data2, NSProviderDeletedPostClean(task, data, NULL));

            NSTask * conTask = NSMakeTask(TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL, data);
            NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(
                        conTask, NSProviderDeletedPostClean(task, data, data2));
            NSConsumerCommunicationTaskProcessing(conTask);

            NSTask * conTask2 = NSMakeTask(TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL, data2);
            NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(
                        conTask, NSProviderDeletedPostClean(task, NULL, data2));
            NSConsumerInternalTaskProcessing(conTask2);

            NSProviderDeletedPostClean(task, NULL, NULL);
            break;
        }
        case TASK_RECV_SYNCINFO:
        case TASK_CONSUMER_RECV_MESSAGE:
        case TASK_CONSUMER_SENT_REQ_OBSERVE:
        case TASK_CONSUMER_RECV_PROVIDER_CHANGED:
        case TASK_MAKE_SYNCINFO:
        case TASK_CONSUMER_REQ_TOPIC_URI:
        case TASK_CONSUMER_RECV_TOPIC_LIST:
        {
            NSConsumerInternalTaskProcessing(task);
            break;
        }
        case TASK_CONSUMER_PROVIDER_DISCOVERED:
        {
            NSTask * getTopicTask = (NSTask *)OICMalloc(sizeof(NSTask));
            NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(getTopicTask,
                        NSRemoveProvider_internal((void *) task->taskData));
            getTopicTask->nextTask = NULL;
            getTopicTask->taskData =
                    (void *) NSCopyProvider_internal((NSProvider_internal *) task->taskData);
            getTopicTask->taskType = TASK_CONSUMER_REQ_TOPIC_LIST;
            NSConsumerCommunicationTaskProcessing(getTopicTask);
            NSConsumerInternalTaskProcessing(task);
            break;
        }
#ifdef WITH_MQ
        case TASK_MQ_REQ_SUBSCRIBE:
        {
            NSConsumerMQTaskProcessing(task);
            break;
        }
#endif
        default:
        {
            NS_LOG(ERROR, "Unknown type of task");
            break;
        }
    }
}

NSProvider_internal * NSConsumerFindNSProvider(const char * providerId)
{
    NS_VERIFY_NOT_NULL(providerId, NULL);

    return NSProviderCacheFind(providerId);
}
