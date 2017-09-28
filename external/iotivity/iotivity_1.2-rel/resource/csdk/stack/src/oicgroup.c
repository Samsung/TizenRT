//******************************************************************
//
// Copyright 2014 Samsung Electronics All Rights Reserved.
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

#define _POSIX_C_SOURCE 200112L

#include "iotivity_config.h"

#include <string.h>

#include "oicgroup.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "cbor.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "occollection.h"
#include "logger.h"
#include "timer.h"

#define TAG "OIC_RI_GROUP"

#define DESC_DELIMITER          "\""
#define ACTION_DELIMITER        "*"
#define ATTR_DELIMITER          "|"
#define ATTR_ASSIGN             "="

// Definitions for operations related to actions
#define DO_ACTION               "DoAction"
#define GET_ACTIONSET           "GetActionSet"
#define ACTIONSET               "ActionSet"
#define CANCEL_ACTIONSET        "CancelAction"
#define DELETE_ACTIONSET        "DelActionSet"

#define DEFAULT_CONTEXT_VALUE 0x99

#define VARIFY_POINTER_NULL(pointer, result, toExit) \
    if(pointer == NULL) \
    {\
        result = OC_STACK_NO_MEMORY;\
        goto toExit;\
    }
#define VARIFY_PARAM_NULL(pointer, result, toExit) \
    if(pointer == NULL)\
    {\
        result = OC_STACK_INVALID_PARAM;\
        goto exit;\
    }

#define OCFREE(pointer) \
    { \
        OICFree(pointer); \
        pointer = NULL; \
    }

// Mutex implementation macros
#if defined(HAVE_PTHREAD_H)

 #include <pthread.h>
 pthread_mutex_t g_scheduledResourceLock;
 #define MUTEX_LOCK(ARG_NAME)       { pthread_mutex_lock(ARG_NAME); }
 #define MUTEX_UNLOCK(ARG_NAME)     { pthread_mutex_unlock(ARG_NAME); }
 #define MUTEX_INITIALIZE(ARG_NAME) { }
 #define MUTEX_TERMINATE(ARG_NAME)  { }

#elif defined(HAVE_WINDOWS_H)

 #include <windows.h>
 CRITICAL_SECTION g_scheduledResourceLock;
 bool g_initializedScheduledResourceLock = false;
 #define MUTEX_LOCK(ARG_NAME)       { EnterCriticalSection(ARG_NAME); }
 #define MUTEX_UNLOCK(ARG_NAME)     { LeaveCriticalSection(ARG_NAME); }
 #define MUTEX_INITIALIZE(ARG_NAME) { assert(!g_initializedScheduledResourceLock); \
                                      InitializeCriticalSection(ARG_NAME); \
                                      g_initializedScheduledResourceLock = true; \
                                    }
 #define MUTEX_TERMINATE(ARG_NAME)  { if (g_initializedScheduledResourceLock) \
                                      { \
                                          DeleteCriticalSection(ARG_NAME); \
                                          g_initializedScheduledResourceLock = false; \
                                      } \
                                    }

#elif defined(WITH_ARDUINO)

 #define MUTEX_LOCK(ARG_NAME)       { }
 #define MUTEX_UNLOCK(ARG_NAME)     { }
 #define MUTEX_INITIALIZE(ARG_NAME) { }
 #define MUTEX_TERMINATE(ARG_NAME)  { }

#else

 ERROR Need mutex implementation on this platform

#endif

enum ACTION_TYPE
{
    NONE = 0, SCHEDULED, RECURSIVE
};

typedef struct scheduledresourceinfo
{
    OCResource *resource;
    OCActionSet *actionset;

    int timer_id;

    OCServerRequest *ehRequest;

    time_t time;
    struct scheduledresourceinfo* next;
} ScheduledResourceInfo;

ScheduledResourceInfo *g_scheduleResourceList = NULL;

void AddScheduledResource(ScheduledResourceInfo **head,
        ScheduledResourceInfo* add)
{
    OIC_LOG(INFO, TAG, "AddScheduledResource Entering...");

    MUTEX_LOCK(&g_scheduledResourceLock);
    ScheduledResourceInfo *tmp = NULL;

    if (*head != NULL)
    {
        tmp = *head;

        while (tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = add;
    }
    else
    {
        *head = add;
    }
    MUTEX_UNLOCK(&g_scheduledResourceLock);
}

ScheduledResourceInfo* GetScheduledResource(ScheduledResourceInfo *head)
{
    OIC_LOG(INFO, TAG, "GetScheduledResource Entering...");

    MUTEX_LOCK(&g_scheduledResourceLock);

    time_t t_now;

    ScheduledResourceInfo *tmp = NULL;
    tmp = head;

#if !defined(WITH_ARDUINO)
    time(&t_now);
#else
    t_now = now();
#endif

    if (tmp)
    {
        while (tmp)
        {
            time_t diffTm = 0;
            diffTm = timespec_diff(tmp->time, t_now);

            if (diffTm <= (time_t) 0)
            {
                OIC_LOG(INFO, TAG, "return Call INFO.");
                goto exit;
            }

            tmp = tmp->next;
        }
    }

    exit:

    MUTEX_UNLOCK(&g_scheduledResourceLock);

    if (tmp == NULL)
    {
        OIC_LOG(INFO, TAG, "Cannot Find Call Info.");
    }
    return tmp;
}

ScheduledResourceInfo* GetScheduledResourceByActionSetName(ScheduledResourceInfo *head, char *setName)
{
    OIC_LOG(INFO, TAG, "GetScheduledResourceByActionSetName Entering...");

    MUTEX_LOCK(&g_scheduledResourceLock);

    ScheduledResourceInfo *tmp = NULL;
    tmp = head;

    if (tmp)
    {
        while (tmp)
        {
            if (strcmp(tmp->actionset->actionsetName, setName) == 0)
            {
                OIC_LOG(INFO, TAG, "return Call INFO.");
                goto exit;
            }
            tmp = tmp->next;
        }
    }

exit:

    MUTEX_UNLOCK(&g_scheduledResourceLock);

    if (tmp == NULL)
    {
        OIC_LOG(INFO, TAG, "Cannot Find Call Info.");
    }
    return tmp;
}

void RemoveScheduledResource(ScheduledResourceInfo **head,
        ScheduledResourceInfo* del)
{

    MUTEX_LOCK(&g_scheduledResourceLock);

    OIC_LOG(INFO, TAG, "RemoveScheduledResource Entering...");
    ScheduledResourceInfo *tmp = NULL;

    if (del == NULL)
    {

        MUTEX_UNLOCK(&g_scheduledResourceLock);

        return;
    }

    if (*head == del)
    {
        *head = (*head)->next;
    }
    else
    {
        tmp = *head;
        while (tmp->next && (tmp->next != del))
        {
            tmp = tmp->next;
        }
        if (tmp->next)
        {
            tmp->next = del->next;
        }
    }

    OCFREE(del)

    MUTEX_UNLOCK(&g_scheduledResourceLock);
}

typedef struct aggregatehandleinfo
{
    OCServerRequest *ehRequest;
    OCDoHandle required;
    OCResource *collResource;

    struct aggregatehandleinfo *next;
} ClientRequestInfo;

ClientRequestInfo *clientRequstList = NULL;

void AddClientRequestInfo(ClientRequestInfo **head, ClientRequestInfo* add)
{
    ClientRequestInfo *tmp = NULL;

    if (*head != NULL)
    {
        tmp = *head;

        while (tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = add;
    }
    else
    {
        *head = add;
    }
}

ClientRequestInfo* GetClientRequestInfo(ClientRequestInfo *head,
        OCDoHandle handle)
{
    ClientRequestInfo *tmp = NULL;

    tmp = head;

    if (tmp)
    {
        while (tmp)
        {
            if (tmp->required == handle)
            {
                break;
            }

            tmp = tmp->next;
        }

        return tmp;
    }
    return NULL;
}

void RemoveClientRequestInfo(ClientRequestInfo **head, ClientRequestInfo* del)
{
    ClientRequestInfo *tmp = NULL;

    if (del == NULL)
        return;

    if (*head == del)
    {
        *head = (*head)->next;
    }
    else
    {
        tmp = *head;
        while (tmp->next && (tmp->next != del))
        {
            tmp = tmp->next;
        }
        if (tmp->next)
        {
            tmp->next = del->next;
        }
    }
}

void AddCapability(OCCapability** head, OCCapability* node)
{
    OCCapability *pointer = *head;
    if (NULL == pointer)
    {
        *head = node;
    }
    else
    {
        while (pointer->next != NULL)
        {
            pointer = pointer->next;
        }

        pointer->next = node;
    }
}

void AddAction(OCAction** head, OCAction* node)
{
    OCAction *pointer = *head;
    if (NULL == pointer)
    {
        *head = node;
    }
    else
    {

        while (pointer->next != NULL)
        {
            pointer = pointer->next;
        }

        pointer->next = node;
    }
}

OCStackResult AddActionSet(OCActionSet **head, OCActionSet* node)
{
    OCActionSet *pointer = *head;
    OCActionSet *prev = NULL;
    if(node == NULL)
    {
        return OC_STACK_ERROR;
    }
    if (NULL == pointer)
    {
        *head = node;
    }
    else
    {
        prev = pointer;
        while (pointer != NULL)
        {
            // check the uniqeness of actionsetname.
            if (strcmp(pointer->actionsetName, node->actionsetName) == 0)
            {
                return OC_STACK_ERROR;
            }

            prev = pointer;
            pointer = pointer->next;
        }

        prev->next = node;
    }

    return OC_STACK_OK;
}

void DeleteCapability(OCCapability *del)
{
    OCFREE(del->capability)
    del->capability = NULL;
    OCFREE(del->status)
    del->status = NULL;
    OCFREE(del)
}

void DeleteAction(OCAction** action)
{
    OCCapability* pointer = (*action)->head;
    OCCapability* pDel = NULL;

    while (pointer)
    {
        pDel = pointer;
        pointer = pointer->next;

        DeleteCapability(pDel);
    }
    OCFREE((*action)->resourceUri)
    (*action)->next = NULL;
    OCFREE(*action)
}

void DeleteActionSet(OCActionSet** actionset)
{
    OCAction* pointer = NULL;
    OCAction* pDel = NULL;

    if(*actionset == NULL)
        return;

    pointer = (*actionset)->head;

    while (pointer)
    {
        pDel = pointer;
        pointer = pointer->next;

        DeleteAction(&pDel);
    }
    //    (*actionset)->head = NULL;
    OCFREE((*actionset)->actionsetName)
    OCFREE(*actionset)
}

OCStackResult FindAndDeleteActionSet(OCResource **resource,
        const char * actionsetName)
{
    if (*resource != NULL)
    {
        OCActionSet *pointer = NULL;
        OCActionSet *pDel = NULL;

        pointer = (*resource)->actionsetHead;

        if (pointer == NULL)
        {
            return OC_STACK_ERROR;
        }
        else
        {
            if (strcmp(pointer->actionsetName, actionsetName) == 0)
            {
                if (pointer->next != NULL)
                    (*resource)->actionsetHead = pointer->next;
                else
                    (*resource)->actionsetHead = NULL;

                DeleteActionSet(&pointer);
            }
            else if (pointer->next != NULL)
            {
                while (pointer)
                {
                    if (pointer->next != NULL)
                    {
                        if (strcmp(pointer->next->actionsetName, actionsetName)
                                == 0)
                        {
                            pDel = pointer->next;
                            pointer->next = pointer->next->next;

                            DeleteActionSet(&pDel);
                        }
                    }
                    pointer = pointer->next;
                }
            }

            return OC_STACK_OK;
        }

    }
    return OC_STACK_ERROR;
}

OCStackResult GetActionSet(const char *actionName, OCActionSet *head,
        OCActionSet** actionset)
{
    OCActionSet *pointer = head;

    while (pointer)
    {
        if (strcmp(pointer->actionsetName, actionName) == 0)
        {
            *actionset = pointer;
            return OC_STACK_OK;
        }

        pointer = pointer->next;
    }

    return OC_STACK_ERROR;

}

OCStackResult ExtractKeyValueFromRequest(OCEntityHandlerRequest *ehRequest,
                                        char **key, char **value)
{
    OCStackResult result = OC_STACK_OK;

    char *actionSetStr = NULL;

    if( NULL == ehRequest->payload )
    {
        result = OC_STACK_ERROR;
        goto exit;
    }

    OCRepPayload* input;

    input = (OCRepPayload*)(ehRequest->payload);

    if(OCRepPayloadGetPropString(input, ACTIONSET, &actionSetStr))
    {
        *key = OICStrdup(ACTIONSET);
        VARIFY_POINTER_NULL(*key, result, exit);

        *value = OICStrdup(actionSetStr);
        VARIFY_POINTER_NULL(*value, result, exit);
    }
    else if(OCRepPayloadGetPropString(input, DO_ACTION, &actionSetStr))
    {
        *key = OICStrdup(DO_ACTION);
        VARIFY_POINTER_NULL(*key, result, exit);

        *value = OICStrdup(actionSetStr);
        VARIFY_POINTER_NULL(*value, result, exit);
    }
    else if(OCRepPayloadGetPropString(input, GET_ACTIONSET, &actionSetStr))
    {
        *key = OICStrdup(GET_ACTIONSET);
        VARIFY_POINTER_NULL(*key, result, exit);

        *value = OICStrdup(actionSetStr);
        VARIFY_POINTER_NULL(*value, result, exit);
    }
    else if(OCRepPayloadGetPropString(input, DELETE_ACTIONSET, &actionSetStr))
    {
        *key = OICStrdup(DELETE_ACTIONSET);
        VARIFY_POINTER_NULL(*key, result, exit);

        *value = OICStrdup(actionSetStr);
        VARIFY_POINTER_NULL(*value, result, exit);
    }
    else if(OCRepPayloadGetPropString(input, CANCEL_ACTIONSET, &actionSetStr))
    {
        *key = OICStrdup(CANCEL_ACTIONSET);
        VARIFY_POINTER_NULL(*key, result, exit);

        *value = OICStrdup(actionSetStr);
        VARIFY_POINTER_NULL(*value, result, exit);
    }
    else
    {
        result = OC_STACK_ERROR;
    }

exit:
    if (result != OC_STACK_OK)
    {
        OCFREE(*key)
        OCFREE(*value)
    }

    OCFREE(actionSetStr);

    return result;
}

OCStackResult ExtractActionSetNameAndDelaytime(char *pChar, char **setName,
        long int *pa)
{
    char *token = NULL, *tokenPtr = NULL;
    OCStackResult result = OC_STACK_OK;

    token = (char*) strtok_r(pChar, ACTION_DELIMITER, &tokenPtr);
    VARIFY_POINTER_NULL(token, result, exit)

    *setName = (char *) OICMalloc(strlen(token) + 1);
    VARIFY_POINTER_NULL(*setName, result, exit)
    VARIFY_PARAM_NULL(token, result, exit)
    strncpy(*setName, token, strlen(token) + 1);

    token = strtok_r(NULL, ACTION_DELIMITER, &tokenPtr);
    VARIFY_POINTER_NULL(pa, result, exit)
    VARIFY_PARAM_NULL(token, result, exit)
    *pa = atoi(token);

    return OC_STACK_OK;

exit:
    OCFREE(*setName);
    return result;
}

OCStackResult BuildActionSetFromString(OCActionSet **set, char* actiondesc)
{
    OCStackResult result = OC_STACK_OK;

    char *iterToken = NULL, *iterTokenPtr = NULL;
    char *descIterToken = NULL, *descIterTokenPtr = NULL;
    char *attrIterToken = NULL, *attrIterTokenPtr = NULL;
    char *desc = NULL, *attr = NULL;
    char *key = NULL, *value = NULL;

    OCAction *action = NULL;
    OCCapability *capa = NULL;

    OIC_LOG(INFO, TAG, "Build ActionSet Instance.");

    *set = (OCActionSet*) OICMalloc(sizeof(OCActionSet));
    VARIFY_POINTER_NULL(*set, result, exit)

    iterToken = (char *) strtok_r(actiondesc, ACTION_DELIMITER, &iterTokenPtr);
    VARIFY_POINTER_NULL(iterToken, result, exit);

    // ActionSet Name
    memset(*set, 0, sizeof(OCActionSet));
    (*set)->actionsetName = (char *) OICMalloc(strlen(iterToken) + 1);
    VARIFY_POINTER_NULL((*set)->actionsetName, result, exit)
    VARIFY_PARAM_NULL(iterToken, result, exit)
    strncpy((*set)->actionsetName, iterToken, strlen(iterToken) + 1);

    // Time info. for Scheduled/Recursive Group action.
    // d is meant Day of the week.
    // T is meant ActionType.
    // yyyy-mm-dd hh:mm:ss d
    iterToken = (char *) strtok_r(NULL, ACTION_DELIMITER, &iterTokenPtr);
    VARIFY_PARAM_NULL(iterToken, result, exit)
#if !defined(WITH_ARDUINO)
    if( 2 != sscanf(iterToken, "%ld %u", &(*set)->timesteps, &(*set)->type) )
    {
        // If the return value should be 2, the number of items in the argument. Otherwise, it fails.
        goto exit;
    }
#endif

    OIC_LOG_V(INFO, TAG, "ActionSet Name : %s", (*set)->actionsetName);

    iterToken = (char *) strtok_r(NULL, ACTION_DELIMITER, &iterTokenPtr);
    while (iterToken)
    {
        desc = (char *) OICMalloc(strlen(iterToken) + 1);
        VARIFY_POINTER_NULL(desc, result, exit)
        VARIFY_PARAM_NULL(desc, result, exit)
        strncpy(desc, iterToken, strlen(iterToken) + 1);
        descIterToken = (char *) strtok_r(desc, ATTR_DELIMITER,
                &descIterTokenPtr);
        while (descIterToken)
        {
            attr = (char *) OICMalloc(strlen(descIterToken) + 1);
            VARIFY_POINTER_NULL(attr, result, exit)
            VARIFY_PARAM_NULL(descIterToken, result, exit)
            strncpy(attr, descIterToken, strlen(descIterToken) + 1);

            attrIterToken = (char *) strtok_r(attr, ATTR_ASSIGN,
                    &attrIterTokenPtr);
            VARIFY_POINTER_NULL(attrIterToken, result, exit);

            key = (char *) OICMalloc(strlen(attrIterToken) + 1);
            VARIFY_POINTER_NULL(key, result, exit)
            VARIFY_PARAM_NULL(attrIterToken, result, exit)
            strncpy(key, attrIterToken, strlen(attrIterToken) + 1);

            attrIterToken = (char *) strtok_r(NULL, ATTR_ASSIGN,
                    &attrIterTokenPtr);
            VARIFY_POINTER_NULL(attrIterToken, result, exit);
            value = (char *) OICMalloc(strlen(attrIterToken) + 1);
            VARIFY_POINTER_NULL(value, result, exit)
            VARIFY_PARAM_NULL(attrIterToken, result, exit)
            strncpy(value, attrIterToken, strlen(attrIterToken) + 1);

            if (strcmp(key, "uri") == 0)
            {
                OIC_LOG(INFO, TAG, "Build OCAction Instance.");

                action = (OCAction*) OICMalloc(sizeof(OCAction));
                VARIFY_POINTER_NULL(action, result, exit)
                memset(action, 0, sizeof(OCAction));
                action->resourceUri = (char *) OICMalloc(strlen(value) + 1);
                VARIFY_POINTER_NULL(action->resourceUri, result, exit)
                VARIFY_PARAM_NULL(value, result, exit)
                strncpy(action->resourceUri, value, strlen(value) + 1);
            }
            else
            {
                if ((key != NULL) && (value != NULL))
                {
                    OIC_LOG(INFO, TAG, "Build OCCapability Instance.");

                    capa = (OCCapability*) OICMalloc(sizeof(OCCapability));
                    VARIFY_POINTER_NULL(capa, result, exit)
                    memset(capa, 0, sizeof(OCCapability));

                    capa->capability = (char *) OICMalloc(strlen(key) + 1);
                    VARIFY_POINTER_NULL(capa->capability, result, exit)
                    VARIFY_PARAM_NULL(key, result, exit)
                    strncpy(capa->capability, key, strlen(key) + 1);

                    capa->status = (char *) OICMalloc(strlen(value) + 1);
                    VARIFY_POINTER_NULL(capa->status, result, exit)
                    VARIFY_PARAM_NULL(value, result, exit)
                    strncpy(capa->status, value, strlen(value) + 1);

                    VARIFY_POINTER_NULL(action, result, exit)

                    AddCapability(&action->head, capa);
                }
            }

            OCFREE(key)
            OCFREE(value)
            OCFREE(attr)

            descIterToken = (char *) strtok_r(NULL, ATTR_DELIMITER,
                    &descIterTokenPtr);
        }

        AddAction(&(*set)->head, action);
        iterToken = (char *) strtok_r(NULL, ACTION_DELIMITER, &iterTokenPtr);
        OCFREE(desc);
    }

    return OC_STACK_OK;
exit:
    OCFREE(attr)
    OCFREE(desc)
    OCFREE(capa)
    OCFREE(action)
    OCFREE((*set)->actionsetName)
    OCFREE(*set)
    OCFREE(key)
    OCFREE(value)
    OCFREE(attr)

    return result;
}

OCStackResult BuildStringFromActionSet(OCActionSet* actionset, char** desc)
{
    // Can't use the macros here as they are hardcoded to 'exit' and will
    // result in dereferencing a null pointer.
    if (!actionset || !desc)
    {
        return OC_STACK_INVALID_PARAM;
    }
    char temp[1024] = { 0 };
    size_t remaining = sizeof(temp) - 1;
    OCStackResult res = OC_STACK_ERROR;
    char* actionTypeStr = NULL;

    OCAction *action = actionset->head;

    if (remaining >= strlen(actionset->actionsetName) + 1)
    {
        strncat(temp, actionset->actionsetName, strlen(actionset->actionsetName));
        remaining -= strlen(actionset->actionsetName);
        strncat(temp, ACTION_DELIMITER, strlen(ACTION_DELIMITER));
        remaining--;
    }
    else
    {
        res = OC_STACK_ERROR;
        goto exit;
    }

    actionTypeStr = (char *)OICMalloc(1024);
    if(actionTypeStr != NULL)
    {
        sprintf(actionTypeStr, "%ld %u", actionset->timesteps, actionset->type);
        if(remaining >= strlen(actionTypeStr) + strlen(ACTION_DELIMITER) + 1)
        {
            strncat(temp, actionTypeStr, strlen(actionTypeStr));
            remaining -= strlen(actionTypeStr);
            strncat(temp, ACTION_DELIMITER, strlen(ACTION_DELIMITER));
            remaining -= strlen(ACTION_DELIMITER);
            OICFree(actionTypeStr);
        }
        else
        {
            OICFree(actionTypeStr);
            res = OC_STACK_ERROR;
            goto exit;
        }
    }
    else
    {
        res = OC_STACK_ERROR;
        goto exit;
    }

    while (action != NULL)
    {
        if (remaining < (strlen("uri=") + strlen(action->resourceUri) + 1))
        {
            res = OC_STACK_ERROR;
            goto exit;
        }

        strcat(temp, "uri=");
        remaining -= strlen("uri=");
        strcat(temp, action->resourceUri);
        remaining -= strlen(action->resourceUri);
        strcat(temp, "|");
        remaining--;

        OCCapability *capas = action->head;
        while (capas != NULL)
        {
            if (remaining < (strlen(capas->capability)
                             + 1 + strlen(capas->status)))
            {
                res = OC_STACK_ERROR;
                goto exit;
            }

            strcat(temp, capas->capability);
            remaining -= strlen(capas->capability);
            strcat(temp, "=");
            remaining--;
            strcat(temp, capas->status);
            remaining -= strlen(capas->status);

            capas = capas->next;
            if (capas != NULL)
            {
                if (remaining < 1)
                {
                    res = OC_STACK_ERROR;
                    goto exit;
                }
                strcat(temp, "|");
                remaining --;
            }
        }

        action = action->next;
        if (action != NULL)
        {
            if (remaining < strlen(ACTION_DELIMITER))
            {
                res = OC_STACK_ERROR;
                goto exit;
            }
            strcat(temp, ACTION_DELIMITER);
            remaining--;
        }
    }

    *desc = OICStrdup(temp);
    VARIFY_POINTER_NULL(*desc, res, exit);

    return OC_STACK_OK;

exit:
    OCFREE(*desc);
    return res;
}

OCStackApplicationResult ActionSetCB(void* context, OCDoHandle handle,
        OCClientResponse* clientResponse)
{
    (void)context;
    (void)clientResponse;
    OIC_LOG(INFO, TAG, "Entering ActionSetCB");

    ClientRequestInfo *info = GetClientRequestInfo(clientRequstList, handle);

    if (info)
    {
        OCEntityHandlerResponse response = { 0 };

        response.ehResult = OC_EH_OK;

        if(NULL == clientResponse->payload)
        {
            OIC_LOG(ERROR, TAG, "Error sending response");
            return OC_STACK_DELETE_TRANSACTION;
        }

        // Format the response.  Note this requires some info about the request
        response.requestHandle = info->ehRequest->requestId;
        response.resourceHandle = info->collResource;
        response.payload = clientResponse->payload;
        response.numSendVendorSpecificHeaderOptions = 0;
        memset(response.sendVendorSpecificHeaderOptions, 0,
                sizeof response.sendVendorSpecificHeaderOptions);
        memset(response.resourceUri, 0, sizeof response.resourceUri);
        // Indicate that response is NOT in a persistent buffer
        response.persistentBufferFlag = 0;

        // Send the response
        if (OCDoResponse(&response) != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "Error sending response");
            return OC_STACK_DELETE_TRANSACTION;
        }

        RemoveClientRequestInfo(&clientRequstList, info);
        OCFREE(info)
    }

    return OC_STACK_KEEP_TRANSACTION;
}

void ActionSetCD(void *context)
{
    (void)context;
}

OCStackResult BuildActionJSON(OCAction* action, unsigned char* bufferPtr,
        uint16_t *remaining)
{
    OCStackResult ret = OC_STACK_ERROR;
    cJSON *json;
    cJSON *body;

    char *jsonStr;
    uint16_t jsonLen;

    OIC_LOG(INFO, TAG, "Entering BuildActionJSON");
    json = cJSON_CreateObject();

    cJSON_AddItemToObject(json, "rep", body = cJSON_CreateObject());

    OCCapability* pointerCapa = action->head;
    while (pointerCapa)
    {
        cJSON_AddStringToObject(body, pointerCapa->capability,
                pointerCapa->status);
        pointerCapa = pointerCapa->next;
    }

    jsonStr = cJSON_PrintUnformatted(json);

    jsonLen = strlen(jsonStr);
    if (jsonLen < *remaining)
    {
        strncat((char*) bufferPtr, jsonStr, jsonLen);
        *remaining -= jsonLen;
        bufferPtr += jsonLen;
        ret = OC_STACK_OK;
    }

    cJSON_Delete(json);
    free(jsonStr);

    return ret;
}

OCPayload* BuildActionCBOR(OCAction* action)
{
    OCRepPayload* payload = OCRepPayloadCreate();

    if (!payload)
    {
        OIC_LOG(INFO, TAG, "Failed to create put payload object");
        return NULL;
    }

    OCCapability* pointerCapa = action->head;
    while (pointerCapa)
    {
        OCRepPayloadSetPropString(payload, pointerCapa->capability, pointerCapa->status);
        pointerCapa = pointerCapa->next;
    }

    return (OCPayload*) payload;
}

unsigned int GetNumOfTargetResource(OCAction *actionset)
{
    int numOfResource = 0;

    OCAction *pointerAction = actionset;

    while (pointerAction != NULL)
    {
        numOfResource++;
        pointerAction = pointerAction->next;
    }

    return numOfResource;
}

OCStackResult SendAction(OCDoHandle *handle, OCServerRequest* requestHandle, const char *targetUri,
        OCPayload *payload)
{

    OCCallbackData cbData;
    cbData.cb = &ActionSetCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    return OCDoResource(handle, OC_REST_PUT, targetUri, &requestHandle->devAddr,
                       payload, CT_ADAPTER_IP, OC_NA_QOS, &cbData, NULL, 0);
}

OCStackResult DoAction(OCResource* resource, OCActionSet* actionset,
        OCServerRequest* requestHandle)
{
    OCStackResult result = OC_STACK_ERROR;

    if( NULL == actionset->head)
    {
        return result;
    }

    OCAction *pointerAction = actionset->head;

    while (pointerAction != NULL)
    {
        OCPayload* payload;
        payload = BuildActionCBOR(pointerAction);

        if(payload == NULL)
        {
            return result;
        }

        ClientRequestInfo *info = (ClientRequestInfo *) OICMalloc(
                sizeof(ClientRequestInfo));

        if( info == NULL )
        {
            OCFREE(payload);
            return OC_STACK_NO_MEMORY;
        }

        memset(info, 0, sizeof(ClientRequestInfo));

        info->collResource = resource;
        info->ehRequest = requestHandle;

        result = SendAction(&info->required, info->ehRequest, pointerAction->resourceUri,
                payload);

        if (result != OC_STACK_OK)
        {
            OICFree(info);
            return result;
        }

        AddClientRequestInfo(&clientRequstList, info);

        pointerAction = pointerAction->next;
    }

    return result;
}

void DoScheduledGroupAction()
{
    OIC_LOG(INFO, TAG, "DoScheduledGroupAction Entering...");
    ScheduledResourceInfo* info = GetScheduledResource(g_scheduleResourceList);

    if (info == NULL)
    {
        OIC_LOG(INFO, TAG, "Target resource is NULL");
        goto exit;
    }
    else if (info->resource == NULL)
    {
        OIC_LOG(INFO, TAG, "Target resource is NULL");
        goto exit;
    }
    else if (info->actionset == NULL)
    {
        OIC_LOG(INFO, TAG, "Target ActionSet is NULL");
        goto exit;
    }
    else if (info->ehRequest == NULL)
    {
        OIC_LOG(INFO, TAG, "Target ActionSet is NULL");
        goto exit;
    }

    MUTEX_LOCK(&g_scheduledResourceLock);

    DoAction(info->resource, info->actionset, info->ehRequest);

    MUTEX_UNLOCK(&g_scheduledResourceLock);


    if (info->actionset->type == RECURSIVE)
    {
        ScheduledResourceInfo *schedule;
        schedule = (ScheduledResourceInfo *) OICMalloc(
                sizeof(ScheduledResourceInfo));

        if (schedule)
        {
            OIC_LOG(INFO, TAG, "Building New Call Info.");
            memset(schedule, 0, sizeof(ScheduledResourceInfo));

            if (info->actionset->timesteps > 0)
            {
                MUTEX_LOCK(&g_scheduledResourceLock);
                schedule->resource = info->resource;
                schedule->actionset = info->actionset;
                schedule->ehRequest = info->ehRequest;

                schedule->time = registerTimer(info->actionset->timesteps,
                        &schedule->timer_id,
                        &DoScheduledGroupAction);

                OIC_LOG(INFO, TAG, "Reregistration.");
                MUTEX_UNLOCK(&g_scheduledResourceLock);
                AddScheduledResource(&g_scheduleResourceList, schedule);
            }
            else
            {
                OICFree(schedule);
            }
        }
    }

    RemoveScheduledResource(&g_scheduleResourceList, info);

    exit:

    return;
}

OCStackResult BuildCollectionGroupActionCBORResponse(
        OCMethod method/*OCEntityHandlerFlag flag*/, OCResource *resource,
        OCEntityHandlerRequest *ehRequest)
{
    OCStackResult stackRet = OC_STACK_ERROR;

    OIC_LOG(INFO, TAG, "Group Action is requested.");

    char *doWhat = NULL;
    char *details = NULL;

    stackRet = ExtractKeyValueFromRequest(ehRequest, &doWhat, &details);

    if(stackRet != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "ExtractKeyValueFromRequest failed: %d", stackRet);
        return stackRet;
    }

    stackRet = OC_STACK_ERROR;

    if (method == OC_REST_PUT)
    {
        OIC_LOG(INFO, TAG, "Group Action[PUT].");

        if (strcmp(doWhat, ACTIONSET) == 0)
        {
            OCActionSet *actionSet = NULL;
            stackRet = BuildActionSetFromString(&actionSet, details);

            if(stackRet == OC_STACK_OK)
            {
                if (actionSet != NULL)
                {
                    stackRet = AddActionSet(&resource->actionsetHead,
                            actionSet);
                    if (stackRet == OC_STACK_ERROR)
                    {
                        if(actionSet != NULL)
                        {
                            DeleteActionSet( &actionSet );
                        }
                        OIC_LOG(INFO, TAG, "Duplicated ActionSet ");
                    }
                }
                else
                {
                    stackRet = OC_STACK_ERROR;
                    goto exit;
                }
            }
            else
            {
                stackRet = OC_STACK_ERROR;
            }

        }
        else if (strcmp(doWhat, DELETE_ACTIONSET) == 0)
        {
            if (FindAndDeleteActionSet(&resource, details) == OC_STACK_OK)
            {
                stackRet = OC_STACK_OK;
            }
            else
            {
                stackRet = OC_STACK_ERROR;
            }
        }

        OCRepPayload* payload = OCRepPayloadCreate();

        if(!payload)
        {
            OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
            stackRet = OC_STACK_ERROR;
        }
        else
        {
            OCEntityHandlerResponse response = { 0 };

            if(stackRet == OC_STACK_OK)
                response.ehResult = OC_EH_OK;
            else
                response.ehResult = OC_EH_ERROR;

            // Format the response.  Note this requires some info about the request
            response.requestHandle = ehRequest->requestHandle;
            response.resourceHandle = ehRequest->resource;
            response.payload = (OCPayload*) payload;
            response.numSendVendorSpecificHeaderOptions = 0;
            memset(response.sendVendorSpecificHeaderOptions, 0,
                    sizeof response.sendVendorSpecificHeaderOptions);
            memset(response.resourceUri, 0, sizeof response. resourceUri);
            // Indicate that response is NOT in a persistent buffer
            response.persistentBufferFlag = 0;
            response.ehResult = (stackRet == OC_STACK_OK)?OC_EH_OK:OC_EH_ERROR;

            // Send the response
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                OIC_LOG(ERROR, TAG, "Error sending response");
                stackRet = OC_STACK_ERROR;
            }
        }
        OCRepPayloadDestroy(payload);
    }
    else if (method == OC_REST_POST)
    {
        OCActionSet *actionset = NULL;

        OCRepPayload* payload = OCRepPayloadCreate();
        OCRepPayloadSetUri(payload, resource->uri);

        if ((strcmp(doWhat, DO_ACTION) == 0)
                || (strcmp(doWhat, "DoScheduledAction") == 0))
        {
            char *pActionsetName = NULL;
            long int delay = -1;

            if (strcmp(doWhat, "DoScheduledAction") == 0)
            {
                stackRet = ExtractActionSetNameAndDelaytime(details,
                        &pActionsetName, &delay);

                OCFREE(details)
                details = pActionsetName;
            }
            else
            {
                stackRet = OC_STACK_OK;
            }

            if (stackRet == OC_STACK_OK)
            {
                if (GetActionSet(details, resource->actionsetHead,
                        &actionset) != OC_STACK_OK)
                {
                    OIC_LOG(INFO, TAG, "ERROR");
                    stackRet = OC_STACK_ERROR;
                }

                if (actionset == NULL)
                {
                    OIC_LOG(INFO, TAG, "Cannot Find ActionSet");
                    stackRet = OC_STACK_ERROR;
                }
                else
                {
                    OIC_LOG(INFO, TAG, "Group Action[POST].");
                    OCServerRequest *request =
                            GetServerRequestUsingHandle(ehRequest->requestHandle);
                    if (NULL == request)
                    {
                        stackRet = OC_STACK_ERROR;
                        goto exit;
                    }

                    if (actionset->type == NONE)
                    {
                        OIC_LOG_V(INFO, TAG, "Execute ActionSet : %s",
                                actionset->actionsetName);
                        unsigned int num = GetNumOfTargetResource(
                                actionset->head);

                        request->ehResponseHandler = HandleAggregateResponse;
                        request->numResponses = num + 1;

                        DoAction(resource, actionset, request);

                        stackRet = OC_STACK_OK;
                    }
                    else
                    {
                        OIC_LOG_V(INFO, TAG, "Execute Scheduled ActionSet : %s",
                                actionset->actionsetName);

                        delay =
                                (delay == -1 ? actionset->timesteps : delay);

                        ScheduledResourceInfo *schedule;
                        schedule = (ScheduledResourceInfo *) OICMalloc(
                                sizeof(ScheduledResourceInfo));

                        if (schedule)
                        {
                            OIC_LOG(INFO, TAG, "Building New Call Info.");
                            memset(schedule, 0,
                                    sizeof(ScheduledResourceInfo));
                            MUTEX_LOCK(&g_scheduledResourceLock);
                            schedule->resource = resource;
                            schedule->actionset = actionset;
                            schedule->ehRequest = request;

                            MUTEX_UNLOCK(&g_scheduledResourceLock);
                            if (delay > 0)
                            {
                                OIC_LOG_V(INFO, TAG, "delay_time is %ld seconds.",
                                        actionset->timesteps);
                                MUTEX_LOCK(&g_scheduledResourceLock);
                                schedule->time = registerTimer(delay,
                                        &schedule->timer_id,
                                        &DoScheduledGroupAction);
                                MUTEX_UNLOCK(&g_scheduledResourceLock);
                                AddScheduledResource(&g_scheduleResourceList,
                                        schedule);
                                stackRet = OC_STACK_OK;
                            }
                            else
                            {
                                stackRet = OC_STACK_ERROR;
                            }
                        }
                    }
                }
            }
        }
        else if (strcmp(doWhat, "CancelAction") == 0)
        {
            ScheduledResourceInfo *info =
                    GetScheduledResourceByActionSetName(g_scheduleResourceList, details);

            if(info != NULL)
            {
                MUTEX_LOCK(&g_scheduledResourceLock);
                unregisterTimer(info->timer_id);
                MUTEX_UNLOCK(&g_scheduledResourceLock);

                RemoveScheduledResource(&g_scheduleResourceList, info);
                stackRet = OC_STACK_OK;
            }
            else
            {
                stackRet = OC_STACK_ERROR;
            }
        }

        else if (strcmp(doWhat, GET_ACTIONSET) == 0)
        {
            char *plainText = NULL;
            OCActionSet *actionset = NULL;

            GetActionSet(details, resource->actionsetHead, &actionset);
            if (actionset != NULL)
            {
                BuildStringFromActionSet(actionset, &plainText);

                if (plainText != NULL)
                {
                    OCRepPayloadSetPropString(payload, ACTIONSET, plainText);
                }
                OICFree(plainText);
                stackRet = OC_STACK_OK;
            }
        }

        if(!payload)
        {
            OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
            stackRet = OC_STACK_ERROR;
        }
        else
        {
            OCEntityHandlerResponse response = { 0 };
            if(stackRet == OC_STACK_OK)
                response.ehResult = OC_EH_OK;
            else
                response.ehResult = OC_EH_ERROR;

            // Format the response.  Note this requires some info about the request
            response.requestHandle = ehRequest->requestHandle;
            response.resourceHandle = ehRequest->resource;
            response.payload = (OCPayload*) payload;
            response.numSendVendorSpecificHeaderOptions = 0;
            memset(response.sendVendorSpecificHeaderOptions, 0,
                    sizeof response.sendVendorSpecificHeaderOptions);
            memset(response.resourceUri, 0, sizeof response.resourceUri);
            // Indicate that response is NOT in a persistent buffer
            response.persistentBufferFlag = 0;
            response.ehResult = (stackRet == OC_STACK_OK)?OC_EH_OK:OC_EH_ERROR;

            // Send the response
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                OIC_LOG(ERROR, TAG, "Error sending response");
                stackRet = OC_STACK_ERROR;
            }
        }
        OCRepPayloadDestroy(payload);
    }

exit:

    OCFREE(doWhat)
    OCFREE(details)

    return stackRet;
}

OCStackResult InitializeScheduleResourceList()
{
    MUTEX_INITIALIZE(&g_scheduledResourceLock);

    g_scheduleResourceList = NULL;
    return OC_STACK_OK;
}

void TerminateScheduleResourceList()
{
    assert(g_scheduleResourceList == NULL);

    MUTEX_TERMINATE(&g_scheduledResourceLock);
}
