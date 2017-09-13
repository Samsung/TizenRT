/**
 * @copyright
 * DAwit
 *
 * This program is subject to copyright protection in accordance with the
 * applicable law. It must not, except where allowed by law, by any means or
 * in any form be reproduced, distributed or lent. Moreover, no part of the
 * program may be used, viewed, printed, disassembled or otherwise interfered
 * with in any form, except where allowed by law, without the express written
 * consent of the copyright holder.@n
 * Copyright (C) 2017 Samsung Electronics Co., Ltd.
 * All Rights Reserved.
 *
 */

/**
 * @file     oicdartosapis.c
 * @brief    For extra functions
 * @author   Nikhil Doifode (nikhil.pd@samsung.com)
 * @date     14-Mar-2017
 */

#include <pthread.h>

#include "thingsapi.h"
#include "oicdef.h"
#include "thingslogger.h"
//#include "notification_manager.h"
#include "oicreqhandler.h"
#include "octypes.h"
#include "ocpayload.h"
#include "thingsmalloc.h"
#include "oic_string.h"
#include "oicserverbuilder.h"

#ifdef OCF_RTOS

#define TAG "[thingsstacknew]"

static pthread_mutex_t gNSmutexNew      = PTHREAD_MUTEX_INITIALIZER;
extern OICRequestHandler*  gReqHandler;
extern OICServerBuilder*   gServerBuilder;

typedef enum
{
    OIC_THINGS_API_NONE_DA = 0,
    OIC_THINGS_API_OBSERVE_OK_DA = 1,
    OIC_THINGS_API_NOTI_OK_DA = 2,
    OIC_THINGS_API_PUSH_OK_DA = 4
} OICDANotiResult;

#if 0
int OICNotificationForPush(const char* uri,
                           const char* query,
                           OICNotiMessageInfo* msgInfo,
                           OCRepPayload* convertedData)
{
    pthread_mutex_lock(&gNSmutexNew);

    int ret = OIC_THINGS_API_NONE_DA;
    OICNotiMessage* nsMessage = NULL; // Notification Message
    OCRepPayload* extra = NULL;

    if(msgInfo == NULL) {
      // 1. Notify to observers
      if(gReqHandler->NotifyOicObservers(uri, query)) {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Observer Response Success");
        ret |= OIC_THINGS_API_OBSERVE_OK_DA;
      }
      else {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Observer Response Failed");
      }

      if(NULL == msgInfo) {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "No need to notify & push message");
        goto EXIT;
      }
    }

#ifdef ENABLE_PUSH_NOTY
    /* 2. Send Notification Service Message */
    if(msgInfo->p & 1) {

        nsMessage = CreateOICNotiMessage();
        nsMessage->extraInfo = (OCRepPayload*) OCRepPayloadCreate();
        extra = OCRepPayloadCreate();

        if(NULL == nsMessage || NULL == nsMessage->extraInfo || NULL == extra || NULL == convertedData) {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Can not create notification message.");
            goto SEND_PUSHSERVICE;
        }

        nsMessage->sourceName = (char*) DAMalloc (sizeof(char) * strlen(uri) + 1);
        OICStrcpy(nsMessage->sourceName, strlen(uri) + 1, uri);

        // Get resource type
        if(NULL != strstr(uri, URI_ALARMS)) {
            THINGS_LOG_D(THINGS_INFO, TAG, "[%s] : [%s]", "Default Resource Type", SEC_RTYPE_ALARM);
            OCRepPayloadSetPropString(extra, SEC_ATTRIBUTE_CONTENTS_RT, SEC_RTYPE_ALARM);
        }
        else {
            const char* resType = GetResType((gServerBuilder->GetResource(gServerBuilder, uri)), 0);
            if(NULL == resType) {
                THINGS_LOG_D(THINGS_INFO, TAG, "Can not get resource type, Send Notification failed");
                goto SEND_PUSHSERVICE;
            }

            OCRepPayloadSetPropString(extra, SEC_ATTRIBUTE_CONTENTS_RT, resType);
            THINGS_LOG_D(THINGS_INFO, TAG, "[%s] : [%s]", "Resource Type", resType);
        }

        OCRepPayloadSetPropObject(extra, KEY_EXTRA, convertedData);
        OCRepPayloadSetPropObject(nsMessage->extraInfo, KEY_MESSAGE, extra);

        //notification service
        if(OICSendNotification(nsMessage)) {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Notification Response Success");
            ret |= OIC_THINGS_API_NOTI_OK_DA;
        }
        else {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Notification Response Failed");
        }
    }
#endif

SEND_PUSHSERVICE:
    // 3. Send Third party push
    if(msgInfo->p & 2) {
        // D2S [IoT Cloud] - /service/pushservice
        if(OICSendThirdPartyPush(msgInfo)) {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Third Party Push Success!");
            ret |= OIC_THINGS_API_PUSH_OK_DA;
        }
        else {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Third Party Push Failed!");
        }
    }

EXIT:
    FreeOICNotiMessage(nsMessage);
    OCRepPayloadDestroy(extra);
    OCRepPayloadDestroy(convertedData);

    pthread_mutex_unlock(&gNSmutexNew);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return ret;
}



int OICNotify(const char* uri, const char* query)
{
    pthread_mutex_lock(&gNSmutexNew);

    int ret = OIC_THINGS_API_NONE_DA;

    // 1. Notify to observers
    if(gReqHandler->NotifyOicObservers(uri, query)) {
      THINGS_LOG_D(THINGS_DEBUG, TAG, "Observer Response Success");
      ret |= OIC_THINGS_API_OBSERVE_OK_DA;
    }
    else {
      THINGS_LOG_D(THINGS_DEBUG, TAG, "Observer Response Failed");
    }

    pthread_mutex_unlock(&gNSmutexNew);

    return ret;
}
#endif
#endif
