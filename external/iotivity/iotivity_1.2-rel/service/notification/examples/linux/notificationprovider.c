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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "NSProviderInterface.h"
#include "NSCommon.h"
#include "logger.h"
#include "octypes.h"
#include "pthread.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "ocstack.h"

#define TAG "NSProviderExample"

#ifdef WITH_CLOUD
#include "cloud_connector.h"

// Input the following values to publish resource to cloud
char REMOTE_SERVER_ADDRESS[50] = {'\0',};
char AUTH_PROVIDER[50] = {'\0',};
char AUTH_CODE[50] = {'\0',};
char UID[50] = {'\0',};
char ACCESS_TOKEN[50] = {'\0',};
#endif

#ifdef WITH_MQ
char CLOUD_ADDRESS[100] = "10.113.64.52:5686";
char CLOUD_TOPIC[100] = "/oic/ps/notification";
#endif

char mainConsumer[37] = {'\0',};

extern char *strdup(const char *s);

bool isExit = false;

int id;

void* OCProcessThread(void * ptr)
{
    (void) ptr;
    while (!isExit)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            printf("OCStack process error");
            return NULL;
        }
    }

    return NULL;
}

void subscribeRequestCallback(NSConsumer *consumer)
{
    printf("consumer requested to subscribe");

    printf("NS_APP Consumer Device ID: %s\n", consumer->consumerId);

    if(mainConsumer[0] == '\0')
    {
        OICStrcpy(mainConsumer, 37, consumer->consumerId);
    }

    NSAcceptSubscription(consumer->consumerId, true);
}

void syncCallback(NSSyncInfo *sync)
{
    printf("sync requested");

    printf("NS_APP Sync State: %d\n", sync->state);
}

FILE* server_fopen(const char *path, const char *mode)
{
    (void)path;
    return fopen("oic_ns_provider_db.dat", mode);
}

void printTopics(NSTopicLL * topics)
{
    if(!topics)
    {
        printf("topics is null\n");
        return;
    }

    NSTopicLL * iter = topics;

    while(iter)
    {
        printf("tName = %s, tState = %d\n", iter->topicName, (int)iter->state);
        iter = iter->next;
    }
}

void removeTopics(NSTopicLL * topics)
{
    if(!topics)
    {
        printf("topics is null\n");
        return;
    }

    NSTopicLL * iter = topics;

    while(iter)
    {
        NSTopicLL * del = iter;
        if(del->topicName)
        {
            OICFree(del->topicName);
        }
        iter = iter->next;

        OICFree(del);
    }
}

void input(char * buffer)
{
    char ch;
    int i = 0;

    while( (ch = getchar()) != '\n' && i < 100)
        buffer[i++] = ch;

    buffer[i] = '\0';
}

int main()
{
    int num;
    pthread_t processThread;

    printf("NSStartProvider()\n\n");

    // open oic_db
    static OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};
    OCRegisterPersistentStorageHandler(&ps);

    if (OCInit(NULL, 0, OC_CLIENT_SERVER) != OC_STACK_OK)
    {
        printf("OCStack init error");
        return 0;
    }

    pthread_create(&processThread, NULL, OCProcessThread, unlink);

    while (!isExit)
    {
        char dummy;

        printf("==============================================\n");
        printf("1.  NSStartProvider(Accepter: Provider) \n");
        printf("2.  NSStartProvider(Accepter: Consumer) \n");
        printf("3.  NSSendNotification() \n");
        printf("4.  NSRead() \n");
        printf("5.  NSProviderAddTopic(); \n");
        printf("6.  NSProviderDeleteTopic(); \n");
        printf("7.  NSProviderSelectTopic(); \n");
        printf("8.  NSProviderUnselectTopic(); \n");
        printf("9.  NSProviderGetConsumerTopics(); \n");
        printf("10. NSProviderGetTopics(); \n");
        printf("11. NSProviderSubscribeMQService() \n");
        printf("0. NSStopProvider() \n");
#ifdef WITH_CLOUD
        printf("21. NSProviderEnableRemoteService (after login) \n");
        printf("22. NSProviderDisableRemoteService (after login) \n");
        printf("31. Cloud Signup \n");
        printf("32. Cloud Login \n");
        printf("33. Cloud Logout \n");
#endif
        printf("0. Exit() \n");
        printf("==============================================\n");

        printf("input : ");

        if(scanf("%d", &num) > 0)
        {
            if(scanf("%c", &dummy) > 0)
            {
                fflush(stdin);
                printf("\n");
            }
        }
        else
        {
            printf("invalid input \n");
            num = 0;
        }

        switch (num)
        {
            case 1:
            {
                printf("NSStartProvider(Accepter: Provider)");
                NSProviderConfig config;
                config.subControllability = true;
                config.subRequestCallback = subscribeRequestCallback;
                config.syncInfoCallback = syncCallback;
                config.userInfo = OICStrdup("OCF_NOTIFICATION");
                NSStartProvider(config);
            }
                break;

            case 2:
            {
                printf("NSStartProvider(Accepter: Consumer)");
                NSProviderConfig config;
                config.subControllability = false;
                config.subRequestCallback = subscribeRequestCallback;
                config.syncInfoCallback = syncCallback;
                config.userInfo = OICStrdup("OCF_NOTIFICATION");
                NSStartProvider(config);
            }
                break;

            case 3:
            {
                printf("NSSendNotification()");
                char title[100] = {'\0',};
                char body[100] = {'\0',};
                char topic[100] = {'\0',};

                printf("id : %d\n", ++id);
                printf("title : ");
                input(title);

                printf("body : ");
                input(body);

                printf("topic : ");

                input(topic);

                printf("app - mTitle : %s \n", title);
                printf("app - mContentText : %s \n", body);
                printf("app - topic : %s \n", topic);

                NSMessage * msg = NSCreateMessage();
                if(msg)
                {

                    msg->title = OICStrdup(title);
                    msg->contentText = OICStrdup(body);
                    msg->sourceName = OICStrdup("OCF");

                    if(topic[0] != '\0')
                    {
                        msg->topic = OICStrdup(topic);
                    }

                    NSSendMessage(msg);
                }
            }
                break;

            case 4:
                printf("NSRead\n");
                break;

            case 5:
                printf("NSProviderAddTopic\n");
                NSProviderRegisterTopic("OCF_TOPIC1");
                NSProviderRegisterTopic("OCF_TOPIC2");
                NSProviderRegisterTopic("OCF_TOPIC3");
                NSProviderRegisterTopic("OCF_TOPIC4");
                break;

            case 6:
                printf("NSProviderDeleteTopic\n");
                NSProviderUnregisterTopic("OCF_TOPIC2");
                break;

            case 7:
                printf("NSProviderSelectTopic\n");
                NSProviderSetConsumerTopic(mainConsumer, "OCF_TOPIC1");
                NSProviderSetConsumerTopic(mainConsumer, "OCF_TOPIC2");
                NSProviderSetConsumerTopic(mainConsumer, "OCF_TOPIC3");
                NSProviderSetConsumerTopic(mainConsumer, "OCF_TOPIC4");
                break;

            case 8:
                printf("NSProviderUnSelectTopic\n");
                NSProviderUnsetConsumerTopic(mainConsumer, "OCF_TOPIC1");
                break;

            case 9:
                printf("NSProviderGetConsumerTopics\n");
                {
                    NSTopicLL * topics = NSProviderGetConsumerTopics(mainConsumer);
                    printTopics(topics);
                    removeTopics(topics);
                }
                break;

            case 10:
                printf("NSProviderGetTopics\n");
                {
                    NSTopicLL * topics = NSProviderGetTopics();
                    printTopics(topics);
                    removeTopics(topics);
                }
                break;

#if WITH_MQ
            case 11:
                printf("11. Subscribe MQ Service\n");

                printf("Remote Server Address: ");
                //input(CLOUD_ADDRESS);

                printf("Topic[notificationtest]: ");
                //input(CLOUD_TOPIC);

                NSProviderSubscribeMQService(CLOUD_ADDRESS, CLOUD_TOPIC);
                break;
#endif

#ifdef WITH_CLOUD
            case 21:
                printf("Enable Remote Service\n");
                if(!IsCloudLoggedin())
                {
                    printf("Login required\n");
                    break;
                }
                NSProviderEnableRemoteService(REMOTE_SERVER_ADDRESS);
                break;

            case 22:
                printf("Disable Remote Service\n");
                if(!IsCloudLoggedin())
                {
                    printf("Login required\n");
                    break;
                }
                NSProviderDisableRemoteService(REMOTE_SERVER_ADDRESS);
                break;

            case 31:
                printf("Remote Server Address: ");
                input(REMOTE_SERVER_ADDRESS);

                printf("Auth Provider(eg. github): ");
                input(AUTH_PROVIDER);

                printf("Auth Code: ");
                input(AUTH_CODE);

                OCCloudSignup(REMOTE_SERVER_ADDRESS, OCGetServerInstanceIDString(),
                    AUTH_PROVIDER, AUTH_CODE, CloudSignupCallback);
                printf("OCCloudSignup requested");
                break;
            case 32:
                printf("Remote Server Address: ");
                input(REMOTE_SERVER_ADDRESS);

                printf("UID: ");
                input(UID);

                printf("ACCESS_TOKEN: ");
                input(ACCESS_TOKEN);

                OCCloudLogin(REMOTE_SERVER_ADDRESS, UID, OCGetServerInstanceIDString(),
                    ACCESS_TOKEN, CloudLoginoutCallback);
                printf("OCCloudLogin requested\n");
                break;
            case 33:
                OCCloudLogout(REMOTE_SERVER_ADDRESS, CloudLoginoutCallback);
                printf("OCCloudLogin requested\n");
                break;
#endif
            case 0:
                NSStopProvider();
                isExit = true;
                break;
            default:
                break;
        }

        printf("\n");
    }

    return 0;
}
