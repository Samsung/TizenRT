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

#include "NSProviderDiscovery.h"

NSResult NSStartPresence()
{
    NS_LOG(DEBUG, "NSStartPresence()");

    if (OCStartPresence(0) != OC_STACK_OK)
    {
        NS_LOG(DEBUG, "NSStartPresence() NS_ERROR");
        return NS_ERROR;
    }

    NS_LOG(DEBUG, "NSStartPresence() NS_OK");
    return NS_OK;
}

NSResult NSStopPresence()
{
    NS_LOG(DEBUG, "NSStopPresence()");

    if (OCStopPresence() != OC_STACK_OK)
    {
        NS_LOG(DEBUG, "NSStopPresence() NS_ERROR");
        return NS_ERROR;
    }

    NS_LOG(DEBUG, "NSStopPresence() NS_OK");
    return NS_OK;
}

void * NSDiscoverySchedule(void * ptr)
{
    if (ptr == NULL)
    {
        NS_LOG(DEBUG, "Create NSDiscoverySchedule");
    }

    while (NSIsRunning[DISCOVERY_SCHEDULER])
    {
        sem_wait(&NSSemaphore[DISCOVERY_SCHEDULER]);
        pthread_mutex_lock(&NSMutex[DISCOVERY_SCHEDULER]);

        if (NSHeadMsg[DISCOVERY_SCHEDULER] != NULL)
        {
            NSTask *node = NSHeadMsg[DISCOVERY_SCHEDULER];
            NSHeadMsg[DISCOVERY_SCHEDULER] = node->nextTask;

            switch (node->taskType)
            {
                case TASK_START_PRESENCE:
                    NS_LOG(DEBUG, "CASE TASK_START_PRESENCE : ");
                    NSStartPresence();
                    break;
                case TASK_STOP_PRESENCE:
                    NS_LOG(DEBUG, "CASE TASK_STOP_PRESENCE : ");
                    NSStopPresence();
                    break;
                case TASK_REGISTER_RESOURCE:
                    NS_LOG(DEBUG, "CASE TASK_REGISTER_RESOURCE : ");
                    NSRegisterResource();
                    break;
#if (defined WITH_CLOUD)
                case TASK_PUBLISH_RESOURCE:
                    NS_LOG(DEBUG, "CASE TASK_PUBLISH_PESOURCE : ");
                    NSPublishResourceToCloud((char*)node->taskData);
                    break;
#endif
                default:
                    break;
            }

            NSOICFree(node);
        }

        pthread_mutex_unlock(&NSMutex[DISCOVERY_SCHEDULER]);
    }

    NS_LOG(DEBUG, "Destroy NSDiscoverySchedule");
    return NULL;
}
