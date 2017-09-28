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

#include "NSUtil.h"

OCEntityHandlerRequest *NSCopyOCEntityHandlerRequest(OCEntityHandlerRequest *entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSCopyOCEntityHandlerRequest - IN");

    OCEntityHandlerRequest *copyOfRequest =
            (OCEntityHandlerRequest *)OICMalloc(sizeof(OCEntityHandlerRequest));

    if (copyOfRequest)
    {
        memcpy(copyOfRequest, entityHandlerRequest, sizeof(OCEntityHandlerRequest));
        copyOfRequest->payload = NULL;
        copyOfRequest->query = NULL;
        copyOfRequest->numRcvdVendorSpecificHeaderOptions = 0;
        copyOfRequest->rcvdVendorSpecificHeaderOptions = NULL;

        if (entityHandlerRequest->query)
        {
            copyOfRequest->query = OICStrdup(entityHandlerRequest->query);
            if (!copyOfRequest->query)
            {
                NS_LOG(ERROR, "Copy failed due to allocation failure");
                OICFree(copyOfRequest);
                return NULL;
            }
        }

        if (entityHandlerRequest->payload)
        {
            copyOfRequest->payload = (OCPayload *)
                    (OCRepPayloadClone ((OCRepPayload*) entityHandlerRequest->payload));
        }
    }

    if (copyOfRequest)
    {
        NS_LOG(DEBUG, "Copied client request");
    }
    else
    {
        NS_LOG(DEBUG, "Error copying client request");
    }

    NS_LOG(DEBUG, "NSCopyOCEntityHandlerRequest - OUT");
    return copyOfRequest;
}

NSResult NSFreeOCEntityHandlerRequest(OCEntityHandlerRequest * entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSFreeOCEntityHandlerRequest - IN");

    OICFree(entityHandlerRequest->query);
    OCPayloadDestroy(entityHandlerRequest->payload);
    OICFree(entityHandlerRequest);

    NS_LOG(DEBUG, "NSFreeOCEntityHandlerRequest - OUT");

    return NS_OK;
}

NSResult NSFreeMessage(NSMessage * obj)
{
    if (!obj)
    {
        return NS_ERROR;
    }

    obj->messageId = 0;
    (obj->providerId)[0] = '\0';

    NSFreeMalloc(&(obj->dateTime));
    obj->ttl = 0;
    NSFreeMalloc(&(obj->title));
    NSFreeMalloc(&(obj->contentText));
    NSFreeMalloc(&(obj->sourceName));
    NSFreeMalloc(&(obj->topic));
    NSFreeMediaContents(obj->mediaContents);
    OCRepPayloadDestroy(obj->extraInfo);
    OICFree(obj);

    return NS_OK;
}

NSMessage * NSDuplicateMessage(NSMessage * copyMsg)
{
    NSMessage * newMsg = NULL;

    if (copyMsg == NULL)
    {
        NS_LOG(ERROR, "Copy Msg is NULL");
        return NULL;
    }

    newMsg = NSInitializeMessage();
    NS_VERIFY_NOT_NULL(newMsg, NULL);

    newMsg->messageId = copyMsg->messageId;
    OICStrcpy(newMsg->providerId, UUID_STRING_SIZE, copyMsg->providerId);

    if (copyMsg->dateTime)
    {
        newMsg->dateTime = OICStrdup(copyMsg->dateTime);
    }

    newMsg->ttl = copyMsg->ttl;

    if (copyMsg->title)
    {
        newMsg->title = OICStrdup(copyMsg->title);
    }

    if (copyMsg->contentText)
    {
        newMsg->contentText = OICStrdup(copyMsg->contentText);
    }

    if (copyMsg->sourceName)
    {
       newMsg->sourceName = OICStrdup(copyMsg->sourceName);
    }

    if (copyMsg->mediaContents)
    {
       newMsg->mediaContents = NSDuplicateMediaContents(copyMsg->mediaContents);
    }

    if (copyMsg->topic)
    {
        newMsg->topic = OICStrdup(copyMsg->topic);
    }

    if (copyMsg->extraInfo)
    {
        newMsg->extraInfo = OCRepPayloadClone(copyMsg->extraInfo);
    }

    return newMsg;
}

NSResult NSFreeSync(NSSyncInfo * obj)
{
    if (!obj)
    {
        return NS_ERROR;
    }

    OICFree(obj);

    return NS_OK;
}

NSSyncInfo* NSDuplicateSync(NSSyncInfo * copyMsg)
{
    NSSyncInfo * newMsg = NULL;

    if (!copyMsg)
    {
        NS_LOG(ERROR, "Copy Msg is NULL");
        return NULL;
    }

    newMsg = (NSSyncInfo *)OICMalloc(sizeof(NSSyncInfo));

    if (!newMsg)
    {
        NS_LOG(ERROR, "newMsg is NULL");
        return NULL;
    }

    newMsg->messageId = copyMsg->messageId;
    OICStrcpy(newMsg->providerId, UUID_STRING_SIZE, copyMsg->providerId);
    newMsg->state = copyMsg->state;

    return newMsg;
}

NSResult NSFreeConsumer(NSConsumer * obj)
{
    if (!obj)
    {
        return NS_ERROR;
    }

    (obj->consumerId)[0] = '\0';

    OICFree(obj);
    obj = NULL;

    return NS_OK;
}

NSConsumer* NSDuplicateConsumer(NSConsumer * copyMsg)
{
    NSConsumer * newMsg = NULL;

    if (copyMsg == NULL)
    {
        NS_LOG(ERROR, "Copy Msg is NULL");
        return NULL;
    }

    newMsg = (NSConsumer *)OICMalloc(sizeof(NSConsumer));

    if (!newMsg)
    {
        NS_LOG(ERROR, "newMsg is NULL");
        return NULL;
    }

    (newMsg->consumerId)[0] = '\0';

    OICStrcpy(newMsg->consumerId, UUID_STRING_SIZE, copyMsg->consumerId);

    return newMsg;
}

void NSDuplicateSetPropertyString(OCRepPayload** msgPayload, const char * name,
        const char * copyString)
{
    if (copyString)
    {
        OCRepPayloadSetPropString(*msgPayload, name, copyString);
    }
}

void NSDuplicateSetPropertyInt(OCRepPayload** msgPayload, const char * name,
        int64_t value)
{
    if (value)
    {
        OCRepPayloadSetPropInt(*msgPayload, name, value);
    }
}

NSSyncInfo * NSGetSyncInfo(OCPayload * payload)
{
    NS_LOG(DEBUG, "NSGetSyncInfo - IN");
    char * providerId = NULL;
    int64_t state = 0;

    if (!payload)
    {
        return NULL;
    }

    NSSyncInfo * retSync = (NSSyncInfo *)OICMalloc(sizeof(NSSyncInfo));

    if (!retSync)
    {
        return NULL;
    }

    retSync->messageId = 0;
    retSync->state = NS_SYNC_READ;
    OCRepPayload * repPayload = (OCRepPayload *)payload;

    if (!OCRepPayloadGetPropInt(repPayload, NS_ATTRIBUTE_MESSAGE_ID,
            (int64_t *)&retSync->messageId))
    {
        OICFree(retSync);
        return NULL;
    }

    if (!OCRepPayloadGetPropString(repPayload, NS_ATTRIBUTE_PROVIDER_ID, &providerId))
    {
        OICFree(retSync);
        return NULL;
    }

    if (!OCRepPayloadGetPropInt(repPayload, NS_ATTRIBUTE_STATE, &state))
    {
        OICFree(retSync);
        return NULL;
    }

    retSync->state = (NSSyncType) state;
    OICStrcpy(retSync->providerId, UUID_STRING_SIZE, providerId);
    OICFree(providerId);

    NS_LOG_V(INFO_PRIVATE, "Provider ID : %s", retSync->providerId);
    NS_LOG_V(DEBUG, "Sync ID : %lld", (long long int)retSync->messageId);
    NS_LOG_V(DEBUG, "Sync State : %d", (int) retSync->state);

    NS_LOG(DEBUG, "NSGetSyncInfo - OUT");

    return retSync;
}

NSResult NSGenerateUUIDStr(char uuidStr[UUID_STRING_SIZE])
{
    uint8_t uuid[UUID_SIZE] = { 0, };

    if (RAND_UUID_OK == OCGenerateUuid(uuid))
    {
        if (RAND_UUID_OK == OCConvertUuidToString(uuid, uuidStr))
        {
            return NS_OK;
        }
    }
    return NS_ERROR;
}

char * NSGetValueFromQuery(char *query, char * compareKey)
{
    char *key = NULL;
    char *value = NULL;
    char *restOfQuery = NULL;
    int numKeyValuePairsParsed = 0;

    NS_LOG_V(INFO, "NS Query Params = %s", query);

    if (!query || query[0] == '\0' || !strlen(query))
    {
        NS_LOG(ERROR, "query is null or \\0 or size is 0");
        return NULL;
    }

    char *keyValuePair = strtok_r (query, NS_QUERY_SEPARATOR, &restOfQuery);

    while(keyValuePair)
    {
        if (numKeyValuePairsParsed >= 2)
        {
            NS_LOG(ERROR, "More than 2 queries params in URI.");
            return NULL;
        }

        key = strtok_r(keyValuePair, NS_KEY_VALUE_DELIMITER, &value);

        if (!key || !value)
        {
            NS_LOG(ERROR, "More than 2 queries params in URI.");
            return NULL;
        }

        if (strcmp(key, compareKey) == 0)
        {
            NS_LOG_V(DEBUG, "found Key : [%s] - Value : [%s] = ", key, value);
            return value;
        }

        ++numKeyValuePairsParsed;

        keyValuePair = strtok_r(NULL, NS_QUERY_SEPARATOR, &restOfQuery);
    }

    return NULL;
}

NSResult NSFreeMalloc(char ** obj)
{
    if (*obj)
    {
        OICFree(*obj);
        *obj = NULL;
        return NS_OK;
    }

    return NS_FAIL;
}

NSMediaContents * NSDuplicateMediaContents(NSMediaContents * copyObj)
{
    if (!copyObj)
    {
        return NULL;
    }

    NSMediaContents * newObj = (NSMediaContents *)OICMalloc(sizeof(NSMediaContents));

    if (!newObj)
    {
        NS_LOG(ERROR, "contents newObj is NULL");
        return NULL;
    }

    if (copyObj->iconImage)
    {
        newObj->iconImage = OICStrdup(copyObj->iconImage);
    }

    return newObj;
}

NSResult NSFreeMediaContents(NSMediaContents * obj)
{
    if (!obj)
    {
        return NS_FAIL;
    }

    NSFreeMalloc(&(obj->iconImage));
    OICFree(obj);

    return NS_OK;
}

NSMessage * NSInitializeMessage()
{
    NSMessage * msg = (NSMessage *)OICMalloc(sizeof(NSMessage));

    if (!msg)
    {
        NS_LOG(ERROR, "Msg is NULL");
        return NULL;
    }

    msg->messageId = OICGetCurrentTime(TIME_IN_MS);
    msg->messageId = msg->messageId & 0x000000007FFFFFFF;
    (msg->providerId)[0] = '\0';
    msg->type = 0;
    msg->dateTime = NULL;
    msg->ttl = 0;
    msg->title = NULL;
    msg->contentText = NULL;
    msg->sourceName = NULL;
    msg->mediaContents = NULL;
    msg->topic = NULL;
    msg->extraInfo = NULL;

    return msg;
}

OCRepPayloadValue* NSPayloadFindValue(const OCRepPayload* payload, const char* name)
{
    if (!payload || !name)
    {
        return NULL;
    }

    OCRepPayloadValue* val = payload->values;

    while (val)
    {
        if (0 == strcmp(val->name, name))
        {
            return val;
        }
        val = val->next;
    }

    return NULL;
}

NSTopicList * NSInitializeTopicList()
{
    NSTopicList * topicList = (NSTopicList *)OICMalloc(sizeof(NSTopicList));

    if (!topicList)
    {
        NS_LOG(ERROR, "topicList is NULL");
        return NULL;
    }

    (topicList->consumerId)[0] = '\0';
    topicList->head = NULL;
    topicList->tail = NULL;

    return topicList;
}

OCDevAddr * NSChangeAddress(const char * inputaddress)
{
    NS_VERIFY_NOT_NULL(inputaddress, NULL);

    char * address = (char *)inputaddress;
    char * schema = strstr(inputaddress, "//");
    if (schema)
    {
        address = schema + 2;
    }
    size_t prefixLen = schema - inputaddress;
    if (prefixLen <= 0)
    {
        NS_LOG(ERROR, "Invalid Input address.");
        return NULL;
    }

    OCTransportFlags flags = OC_DEFAULT_FLAGS;
    OCTransportAdapter adapter = OC_ADAPTER_IP;
    if (strstr(inputaddress, "coap+tcp://"))
    {
        NS_LOG(DEBUG, "address : TCP");
        adapter = OC_ADAPTER_TCP;
    }
    else if (strstr(inputaddress, "coaps://"))
    {
        NS_LOG(DEBUG, "address : UDP + SECURED");
        flags |= OC_FLAG_SECURE;
    }
    else if (strstr(inputaddress, "coaps+tcp://"))
    {
        NS_LOG(DEBUG, "address : TCP + SECURED");
        flags |= OC_FLAG_SECURE;
        adapter = OC_ADAPTER_TCP;
    }
    else if (strstr(inputaddress, "coap://"))
    {
        NS_LOG(DEBUG, "address : UDP");
    }
    else
    {
        NS_LOG(ERROR, "Invalid CoAP Schema.");
        return NULL;
    }

    OCDevAddr * retAddr = NULL;
    retAddr = (OCDevAddr *) OICMalloc(sizeof(OCDevAddr));
    NS_VERIFY_NOT_NULL(retAddr, NULL);

    char * start = address;
    char * end = address;
    if (address[0] == '[')
    {
        flags |= OC_IP_USE_V6;
        end = strchr(++address, ']');
        if (!end || end <= start)
        {
            NS_LOG(ERROR, "Invalid Input Address - IPv6.");
            NSOICFree(retAddr);
            return NULL;
        }
        memset(retAddr->addr, 0, (size_t)MAX_ADDR_STR_SIZE);
        OICStrcpy(retAddr->addr, (size_t)(end-start), address);
    }
    else
    {
        flags |= OC_IP_USE_V4;
        end = strchr(address, ':');
        if (!end || end <= start)
        {
            NS_LOG(ERROR, "Invalid Input Address - IPv4.");
            NSOICFree(retAddr);
            return NULL;
        }
        char * end2 = strchr(end + 1, ':');
        if (end2)
        {
            NS_LOG(ERROR, "Invalid Input Address - IPv4.");
            NSOICFree(retAddr);
            return NULL;
        }
        memset(retAddr->addr, 0, (size_t)MAX_ADDR_STR_SIZE);
        OICStrcpy(retAddr->addr, (size_t)(end-start)+1, address);
    }

    retAddr->adapter = adapter;
    retAddr->flags = flags;

    address = end + 1;
    int tmp = 0;
    if (flags & OC_IP_USE_V6)
    {
        address++;
    }
    uint16_t port = address[tmp++] - '0';

    while(true)
    {
        if (address[tmp] == '\0' || address[tmp] > '9' || address[tmp] < '0')
        {
            break;
        }
        if (tmp >= 5 || (port >= 6553 && (address[tmp] -'0') >= 6))
        {
            NS_LOG_V(ERROR, "Invalid Input Address - Port. %d", tmp+1);
            NSOICFree(retAddr);
            return NULL;
        }
        port *= 10;
        port += address[tmp++] - '0';
    }

    retAddr->port = port;

    NS_LOG(DEBUG, "Change Address for TCP request");
    NS_LOG_V(INFO_PRIVATE, "Origin : %s", inputaddress);
    NS_LOG_V(INFO_PRIVATE, "Changed Addr : %s", retAddr->addr);
    NS_LOG_V(INFO_PRIVATE, "Changed Port : %d", retAddr->port);

    return retAddr;
}

bool NSOCResultToSuccess(OCStackResult ret)
{
    switch (ret)
    {
        case OC_STACK_OK:
        case OC_STACK_RESOURCE_CREATED:
        case OC_STACK_RESOURCE_DELETED:
        case OC_STACK_PRESENCE_STOPPED:
        case OC_STACK_CONTINUE:
        case OC_STACK_RESOURCE_CHANGED:
            return true;
        default:
            NS_LOG_V(DEBUG, "OCStackResult : %d", (int)ret);
            return false;
    }
}

