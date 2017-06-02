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

#include "NSConsumerQueue.h"

#include "NSConstants.h"
#include "oic_malloc.h"
#include "NSConsumerCommon.h"

NSConsumerQueue * NSCreateQueue()
{
    NSConsumerQueue * newQueue = (NSConsumerQueue *)OICMalloc(sizeof(NSConsumerQueue));
    NS_VERIFY_NOT_NULL(newQueue, NULL);

    newQueue->size = 0;
    newQueue->head = NULL;
    newQueue->tail = NULL;

    return newQueue;
}

void NSDestroyQueue(NSConsumerQueue * queue)
{
    NS_VERIFY_NOT_NULL_V(queue);

    NSConsumerQueueObject * node = NSPopQueue(queue);
    while(node)
    {
        NSOICFree(node->data);
        NSOICFree(node);

        node = NSPopQueue(queue);
    }
}

bool NSPushConsumerQueue(NSConsumerQueue * queue, NSConsumerQueueObject * object)
{
    NS_VERIFY_NOT_NULL(queue, false);
    NS_VERIFY_NOT_NULL(object, false);

    if (!(queue->head))
    {
        queue->head = object;
    }
    else
    {
        (queue->tail)->next = object;
    }

    queue->tail = object;
    queue->size++;

    return true;
}

NSConsumerQueueObject * NSPopQueue(NSConsumerQueue * queue)
{
    NSConsumerQueueObject * retObject = NULL;

    NS_VERIFY_NOT_NULL(queue, NULL);
    NS_VERIFY_NOT_NULL(queue->head, NULL);

    if (queue->size <= 0)
    {
        return NULL;
    }

    retObject = queue->head;

    queue->head = (NSConsumerQueueObject *)(retObject->next);
    if (!(queue->head))
    {
        queue->tail = NULL;
    }
    retObject->next = NULL;
    queue->size--;

    return retObject;
}

int NSGetQueueSize(NSConsumerQueue * queue)
{
    return queue->size;
}

bool NSIsQueueEmpty(NSConsumerQueue * queue)
{
    return (queue->size <= 0);
}
