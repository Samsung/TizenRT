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

#include "NSProviderResource.h"
#include <string.h>
#include "rd_client.h"

NSNotificationResource NotificationResource;
NSMessageResource NotificationMessageResource;
NSSyncResource NotificationSyncResource;
NSTopicResource NotificationTopicResource;

#if (defined WITH_CLOUD)
#define DEFAULT_CONTEXT_VALUE 0x99

OCStackApplicationResult NSHandlePublishCb(void *ctx, OCDoHandle handle,
    OCClientResponse *clientResponse)
{
    (void) handle;
    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        NS_LOG(DEBUG, "Invalid Publish callback received");
    }

    NS_LOG_V(DEBUG, "Publish resource response received code: %d", clientResponse->result);

    return OC_STACK_KEEP_TRANSACTION;
}

NSResult NSPublishResourceToCloud(char *serverAddress)
{

    NS_LOG(DEBUG, "NSPublishResourceToCloud - IN");
    NS_LOG_V(INFO_PRIVATE, "Remote Server Address: %s", serverAddress);

    OCCallbackData cbData;
    cbData.cb = NSHandlePublishCb;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCResourceHandle resourceHandles[1] = { NotificationResource.handle };
    OCStackResult res = OCRDPublish(NULL, serverAddress, CT_ADAPTER_TCP, resourceHandles, 1,
            &cbData, OC_LOW_QOS);

    if (res != OC_STACK_OK)
    {
        NS_LOG_V(DEBUG, "Unable to publish resources to cloud: %d", res);
    }

    NS_LOG(DEBUG, "NSPublishResourceToCloud - OUT");
    return NS_OK;
}
#endif

NSResult NSCreateResource(char *uri)
{
    NS_LOG(DEBUG, "NSCreateResource - IN");

    if (!uri)
    {
        NS_LOG(ERROR, "Resource URI cannot be NULL");
        return NS_ERROR;
    }

    uint8_t resourceProperties;

    if (strcmp(uri, NS_ROOT_URI) == 0)
    {
        NotificationResource.policy = true;
        (NotificationResource.providerId)[0] = '\0';
        NotificationResource.message_uri = NS_COLLECTION_MESSAGE_URI;
        NotificationResource.sync_uri = NS_COLLECTION_SYNC_URI;
        NotificationResource.topic_uri = NS_COLLECTION_TOPIC_URI;
        NotificationResource.version = VERSION;
        NotificationResource.handle = NULL;

        if (NSGetResourceSecurity())
        {
            NS_LOG(DEBUG, "Create secured resource");
            resourceProperties = OC_DISCOVERABLE | OC_SECURE;
        }
        else
        {
            resourceProperties = OC_DISCOVERABLE;
        }

        if (OCCreateResource(&NotificationResource.handle, NS_ROOT_TYPE, NS_INTERFACE_BASELINE,
                NS_ROOT_URI, NSEntityHandlerNotificationCb, NULL,
                resourceProperties) != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to Create Notification Resource");
            return NS_ERROR;
        }

        if (OCBindResourceInterfaceToResource(NotificationResource.handle, NS_INTERFACE_READ)
            != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to bind Notification Resource Type");
            return NS_ERROR;
        }
    }
    else if (strcmp(uri, NS_COLLECTION_MESSAGE_URI) == 0)
    {

        NotificationMessageResource.messageId = 0;

        (NotificationMessageResource.providerId)[0] = '\0';
        NotificationMessageResource.type = 0;
        NotificationMessageResource.dateTime = NULL;
        NotificationMessageResource.ttl = 0;
        NotificationMessageResource.title = NULL;
        NotificationMessageResource.contentText = NULL;
        NotificationMessageResource.sourceName = NULL;
        NotificationMessageResource.topicName = NULL;
        NotificationMessageResource.mediaContents = NULL;

        if (NSGetResourceSecurity())
        {
            NS_LOG(DEBUG, "Create secured resource");
            resourceProperties = OC_OBSERVABLE | OC_SECURE;
        }
        else
        {
            resourceProperties = OC_OBSERVABLE;
        }

        if (OCCreateResource(&NotificationMessageResource.handle, NS_COLLECTION_MESSAGE_TYPE,
                NS_INTERFACE_BASELINE, NS_COLLECTION_MESSAGE_URI, NSEntityHandlerMessageCb, NULL,
                resourceProperties) != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to Create Notification Message Resource");
            return NS_ERROR;
        }

        if (OCBindResourceInterfaceToResource(NotificationMessageResource.handle, NS_INTERFACE_READ)
            != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to bind Notification Message Resource Type");
            return NS_ERROR;
        }
    }
    else if (strcmp(uri, NS_COLLECTION_SYNC_URI) == 0)
    {
        NotificationSyncResource.messageId = 0;
        (NotificationSyncResource.providerId)[0] = '\0';
        NotificationSyncResource.state = NULL;
        NotificationSyncResource.handle = NULL;

        if (NSGetResourceSecurity())
        {
            NS_LOG(DEBUG, "Create secured resource");
            resourceProperties = OC_OBSERVABLE | OC_SECURE;
        }
        else
        {
            resourceProperties = OC_OBSERVABLE;
        }

        if (OCCreateResource(&(NotificationSyncResource.handle), NS_COLLECTION_SYNC_TYPE,
                NS_INTERFACE_BASELINE, NS_COLLECTION_SYNC_URI, NSEntityHandlerSyncCb, NULL,
                resourceProperties) != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to Create Notification Sync Resource");
            return NS_ERROR;
        }

        if (OCBindResourceInterfaceToResource(NotificationSyncResource.handle,
                NS_INTERFACE_READWRITE)
            != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to bind Notification Sync Resource Type");
            return NS_ERROR;
        }
    }
    else if (strcmp(uri, NS_COLLECTION_TOPIC_URI) == 0)
    {
        (NotificationTopicResource.providerId)[0] = '\0';
        (NotificationTopicResource.consumerId)[0] = '\0';
        NotificationTopicResource.TopicList = NULL;
        NotificationTopicResource.handle = NULL;

        if (NSGetResourceSecurity())
        {
            NS_LOG(DEBUG, "Create secured resource");
            resourceProperties = OC_RES_PROP_NONE | OC_SECURE;
        }
        else
        {
            resourceProperties = OC_RES_PROP_NONE;
        }

        if (OCCreateResource(&(NotificationTopicResource.handle), NS_COLLECTION_TOPIC_TYPE,
                NS_INTERFACE_BASELINE, NS_COLLECTION_TOPIC_URI, NSEntityHandlerTopicCb, NULL,
                resourceProperties) != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to Create Notification Sync Resource");
            return NS_ERROR;
        }

        if (OCBindResourceInterfaceToResource(NotificationTopicResource.handle,
                NS_INTERFACE_READWRITE)
            != OC_STACK_OK)
        {
            NS_LOG(ERROR, "Fail to bind Notification Topic Resource Type");
            return NS_ERROR;
        }
    }
    else
    {
        NS_LOG(ERROR, "Fail to create resource with invalid URI");
        return NS_ERROR;
    }

    NS_LOG(DEBUG, "NSCreateResource - OUT");
    return NS_OK;
}

NSResult NSRegisterResource()
{
    NS_LOG(DEBUG, "NSRegisterResource - IN");

    NS_CREATE_RESOURCE(
            NSCreateResource(NS_COLLECTION_TOPIC_URI), "Fail to register Topic Resource");
    NS_CREATE_RESOURCE(
            NSCreateResource(NS_COLLECTION_SYNC_URI), "Fail to register Sync Resource");
    NS_CREATE_RESOURCE(
            NSCreateResource(NS_COLLECTION_MESSAGE_URI), "Fail to register Message Resource");
    NS_CREATE_RESOURCE(
            NSCreateResource(NS_ROOT_URI), "Fail to register Notification Resource");

    NS_LOG(DEBUG, "NSRegisterResource - OUT");
    return NS_OK;
}

NSResult NSUnRegisterResource()
{
    NS_LOG(DEBUG, "NSUnRegisterResource - IN");

    NS_DELETE_RESOURCE(
            OCDeleteResource(NotificationResource.handle), "Fail to Delete Notification Resource");
    NS_DELETE_RESOURCE(OCDeleteResource(NotificationMessageResource.handle),
            "Fail to Delete Notification Message Resource");
    NS_DELETE_RESOURCE(OCDeleteResource(NotificationSyncResource.handle),
            "Fail to Delete Notification Sync Resource");
    NS_DELETE_RESOURCE(OCDeleteResource(NotificationTopicResource.handle),
            "Fail to Delete Notification Topic Resource");

    NotificationResource.handle = NULL;
    NotificationMessageResource.handle = NULL;
    NotificationSyncResource.handle = NULL;
    NotificationTopicResource.handle = NULL;

    NS_LOG(DEBUG, "NSUnRegisterResource - OUT");
    return NS_OK;
}

NSResult NSPutNotificationResource(bool policy, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutNotificationResource - IN");

    NotificationResource.policy = policy;
    OICStrcpy(NotificationResource.providerId, UUID_STRING_SIZE,
        NSGetProviderInfo()->providerId);
    NotificationResource.message_uri = NS_COLLECTION_MESSAGE_URI;
    NotificationResource.sync_uri = NS_COLLECTION_SYNC_URI;
    NotificationResource.topic_uri = NS_COLLECTION_TOPIC_URI;
    NotificationResource.version = VERSION;

    *handle = NotificationResource.handle;

    NS_LOG(DEBUG, "NSPutNotificationResource - OUT");
    return NS_OK;
}

NSResult NSPutMessageResource(NSMessage *msg, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutMessageResource - IN");

    if(msg != NULL)
    {
        NS_LOG(DEBUG, "NSMessage is valid");

        NotificationMessageResource.messageId = msg->messageId;
        OICStrcpy(NotificationMessageResource.providerId, UUID_STRING_SIZE, msg->providerId);
        NotificationMessageResource.type = msg->type;
        NotificationMessageResource.dateTime = msg->dateTime;
        NotificationMessageResource.ttl = msg->ttl;
        NotificationMessageResource.title = msg->title;
        NotificationMessageResource.contentText = msg->contentText;
        NotificationMessageResource.sourceName = msg->sourceName;
        NotificationMessageResource.topicName = msg->topic;
        NotificationMessageResource.mediaContents = msg->mediaContents;
    }
    else
    {
        NS_LOG(ERROR, "NSMessage is NULL");
    }

    *handle = NotificationMessageResource.handle;

    NS_LOG(DEBUG, "NSPutMessageResource - OUT");
    return NS_OK;
}

NSResult NSPutSyncResource(NSSyncInfo *sync, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutSyncResource - IN");

    (void) sync;

    *handle = NotificationSyncResource.handle;

    NS_LOG(DEBUG, "NSPutSyncResource - OUT");
    return NS_OK;
}

NSResult NSPutTopicResource(NSTopicList *topicList, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutTopicResource - IN");

    (void) topicList;

    *handle = NotificationTopicResource.handle;

    NS_LOG(DEBUG, "NSPutTopicResource - OUT");
    return NS_OK;
}
