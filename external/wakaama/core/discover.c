/*******************************************************************************
*
* Copyright (c) 2015 Intel Corporation and others.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* and Eclipse Distribution License v1.0 which accompany this distribution.
*
* The Eclipse Public License is available at
*    http://www.eclipse.org/legal/epl-v10.html
* The Eclipse Distribution License is available at
*    http://www.eclipse.org/org/documents/edl-v10.php.
*
* Contributors:
*    David Navarro, Intel Corporation - initial API and implementation
*
*******************************************************************************/


#include "internals.h"

#define PRV_LINK_BUFFER_SIZE  1024


#define PRV_CONCAT_STR(buf, len, index, str, str_len)    \
    {                                                    \
        if ((len)-(index) < (str_len)) return -1;        \
        memcpy((buf)+(index), (str), (str_len));         \
        (index) += (str_len);                            \
    }


#ifdef LWM2M_CLIENT_MODE
static int prv_serializeAttributes(lwm2m_context_t * contextP,
                                   lwm2m_uri_t * uriP,
                                   uint8_t * buffer,
                                   size_t uriLen,
                                   size_t bufferLen)
{
    lwm2m_observed_t * observedP;
    lwm2m_watcher_t * watcherP;
    int head;
    int res;

    if (contextP == NULL) return 0;

    observedP = observe_findByUri(contextP, uriP);
    if (observedP == NULL || observedP->watcherList == NULL) return 0;

    head = 0;
    for (watcherP = observedP->watcherList; watcherP != NULL; watcherP = watcherP->next)
    {
        lwm2m_attributes_t * paramP;

        paramP = watcherP->parameters;
        if (paramP == NULL || paramP->toSet == 0) continue;

        if (observedP->watcherList->next != NULL)
        {
            // multiple servers
            memcpy(buffer + head, buffer, uriLen);

            PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ATTR_SEPARATOR, LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, ATTR_SERVER_ID_STR, ATTR_SERVER_ID_LEN);

            res = utils_intToText(watcherP->server->shortID, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        else
        {
            head = uriLen;
        }

        if (paramP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ATTR_SEPARATOR, LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, ATTR_MIN_PERIOD_STR, ATTR_MIN_PERIOD_LEN);

            res = utils_intToText(paramP->minPeriod, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        if (paramP->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ATTR_SEPARATOR, LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, ATTR_MAX_PERIOD_STR, ATTR_MAX_PERIOD_LEN);

            res = utils_intToText(paramP->maxPeriod, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        if (paramP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ATTR_SEPARATOR, LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, ATTR_GREATER_THAN_STR, ATTR_GREATER_THAN_LEN);

            res = utils_floatToText(paramP->greaterThan, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        if (paramP->toSet & LWM2M_ATTR_FLAG_LESS_THAN)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ATTR_SEPARATOR, LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, ATTR_LESS_THAN_STR, ATTR_LESS_THAN_LEN);

            res = utils_floatToText(paramP->lessThan, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        if (paramP->toSet & LWM2M_ATTR_FLAG_STEP)
        {
            PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ATTR_SEPARATOR, LINK_ATTR_SEPARATOR_SIZE);
            PRV_CONCAT_STR(buffer, bufferLen, head, ATTR_STEP_STR, ATTR_STEP_LEN);

            res = utils_floatToText(paramP->step, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;
        }
        PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ITEM_ATTR_END, LINK_ITEM_ATTR_END_SIZE);
    }

    if (head > 0) head -= uriLen;

    return head;
}

static int prv_serializeLinkData(lwm2m_context_t * contextP,
                                 lwm2m_data_t * tlvP,
                                 lwm2m_uri_t * parentUriP,
                                 uint8_t * parentUriStr,
                                 size_t parentUriLen,
                                 uint8_t * buffer,
                                 size_t bufferLen)
{
    int head;
    int res;
    lwm2m_uri_t uri;

    head = 0;

    switch (tlvP->type)
    {
    case LWM2M_TYPE_UNDEFINED:
    case LWM2M_TYPE_STRING:
    case LWM2M_TYPE_OPAQUE:
    case LWM2M_TYPE_INTEGER:
    case LWM2M_TYPE_FLOAT:
    case LWM2M_TYPE_BOOLEAN:
    case LWM2M_TYPE_OBJECT_LINK:
    case LWM2M_TYPE_MULTIPLE_RESOURCE:
        if (bufferLen < LINK_ITEM_START_SIZE) return -1;
        memcpy(buffer + head, LINK_ITEM_START, LINK_ITEM_START_SIZE);
        head = LINK_ITEM_START_SIZE;

        if (parentUriLen > 0)
        {
            if (bufferLen - head < parentUriLen) return -1;
            memcpy(buffer + head, parentUriStr, parentUriLen);
            head += parentUriLen;
        }

        if (bufferLen - head < LINK_URI_SEPARATOR_SIZE) return -1;
        memcpy(buffer + head, LINK_URI_SEPARATOR, LINK_URI_SEPARATOR_SIZE);
        head += LINK_URI_SEPARATOR_SIZE;

        res = utils_intToText(tlvP->id, buffer + head, bufferLen - head);
        if (res <= 0) return -1;
        head += res;

        if (tlvP->type == LWM2M_TYPE_MULTIPLE_RESOURCE)
        {
            if (bufferLen - head < LINK_ITEM_DIM_START_SIZE) return -1;
            memcpy(buffer + head, LINK_ITEM_DIM_START, LINK_ITEM_DIM_START_SIZE);
            head += LINK_ITEM_DIM_START_SIZE;

            res = utils_intToText(tlvP->value.asChildren.count, buffer + head, bufferLen - head);
            if (res <= 0) return -1;
            head += res;

            if (bufferLen - head < LINK_ITEM_ATTR_END_SIZE) return -1;
            memcpy(buffer + head, LINK_ITEM_ATTR_END, LINK_ITEM_ATTR_END_SIZE);
            head += LINK_ITEM_ATTR_END_SIZE;
        }
        else
        {
            if (bufferLen - head < LINK_ITEM_END_SIZE) return -1;
            memcpy(buffer + head, LINK_ITEM_END, LINK_ITEM_END_SIZE);
            head += LINK_ITEM_END_SIZE;
        }

        memcpy(&uri, parentUriP, sizeof(lwm2m_uri_t));
        uri.resourceId = tlvP->id;
        uri.flag |= LWM2M_URI_FLAG_RESOURCE_ID;
        res = prv_serializeAttributes(contextP, &uri, buffer, head - 1, bufferLen);
        if (res < 0) return -1;    // careful, 0 is valid
        if (res > 0) head += res - 1;

        break;

    case LWM2M_TYPE_OBJECT_INSTANCE:
    {
        uint8_t uriStr[URI_MAX_STRING_LEN];
        size_t uriLen;
        size_t index;

        if (parentUriLen > 0)
        {
            if (URI_MAX_STRING_LEN < parentUriLen) return -1;
            memcpy(uriStr, parentUriStr, parentUriLen);
            uriLen = parentUriLen;
        }
        else
        {
            uriLen = 0;
        }

        if (URI_MAX_STRING_LEN - uriLen < LINK_URI_SEPARATOR_SIZE) return -1;
        memcpy(uriStr + uriLen, LINK_URI_SEPARATOR, LINK_URI_SEPARATOR_SIZE);
        uriLen += LINK_URI_SEPARATOR_SIZE;

        res = utils_intToText(tlvP->id, uriStr + uriLen, URI_MAX_STRING_LEN - uriLen);
        if (res <= 0) return -1;
        uriLen += res;

        memcpy(&uri, parentUriP, sizeof(lwm2m_uri_t));
        uri.instanceId = tlvP->id;
        uri.flag |= LWM2M_URI_FLAG_INSTANCE_ID;

        head = 0;
        PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ITEM_START, LINK_ITEM_START_SIZE);
        PRV_CONCAT_STR(buffer, bufferLen, head, uriStr, uriLen);
        PRV_CONCAT_STR(buffer, bufferLen, head, LINK_ITEM_END, LINK_ITEM_END_SIZE);
        res = prv_serializeAttributes(contextP, &uri, buffer, head - 1, bufferLen);
        if (res < 0) return -1;    // careful, 0 is valid
        if (res == 0) head = 0;    // rewind
        else head += res - 1;

        for (index = 0; index < tlvP->value.asChildren.count; index++)
        {
            res = prv_serializeLinkData(contextP, tlvP->value.asChildren.array + index, &uri, uriStr, uriLen, buffer + head, bufferLen - head);
            if (res < 0) return -1;
            head += res;
        }
    }
    break;

    case LWM2M_TYPE_OBJECT:
    default:
        return -1;
    }

    return head;
}

int discover_serialize(lwm2m_context_t * contextP,
                      lwm2m_uri_t * uriP,
                      int size,
                      lwm2m_data_t * dataP,
                      uint8_t ** bufferP)
{
    uint8_t bufferLink[PRV_LINK_BUFFER_SIZE];
    uint8_t baseUriStr[URI_MAX_STRING_LEN];
    int baseUriLen;
    int index;
    size_t head;
    int res;
    lwm2m_uri_t tempUri;

    LOG_ARG("size: %d", size);
    LOG_URI(uriP);
    baseUriLen = uri_toString(uriP, baseUriStr, URI_MAX_STRING_LEN, NULL);
    if (baseUriLen < 0) return -1;
    baseUriLen -= 1;

    head = 0;
    memset(&tempUri, 0, sizeof(lwm2m_uri_t));

    // get object level attributes
    PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, LINK_ITEM_START, LINK_ITEM_START_SIZE);
    PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, LINK_URI_SEPARATOR, LINK_URI_SEPARATOR_SIZE);
    res = utils_intToText(uriP->objectId, bufferLink + head, PRV_LINK_BUFFER_SIZE - head);
    if (res <= 0) return -1;
    head += res;
    PRV_CONCAT_STR(bufferLink, PRV_LINK_BUFFER_SIZE, head, LINK_ITEM_END, LINK_ITEM_END_SIZE);
    tempUri.objectId = uriP->objectId;
    res = prv_serializeAttributes(contextP, &tempUri, bufferLink, head - 1, PRV_LINK_BUFFER_SIZE);
    if (res < 0) return -1;    // careful, 0 is valid
    if (res == 0) head = 0;    // rewind
    else head += res - 1;
    if (LWM2M_URI_IS_SET_INSTANCE(uriP))
    {
        size_t subHead;

        // get object instance level attributes
        subHead = 0;
        PRV_CONCAT_STR(bufferLink + head, PRV_LINK_BUFFER_SIZE - head, subHead, LINK_ITEM_START, LINK_ITEM_START_SIZE);
        PRV_CONCAT_STR(bufferLink + head, PRV_LINK_BUFFER_SIZE - head, subHead, LINK_URI_SEPARATOR, LINK_URI_SEPARATOR_SIZE);
        res = utils_intToText(uriP->objectId, bufferLink + head + subHead, PRV_LINK_BUFFER_SIZE - head - subHead);
        if (res <= 0) return -1;
        subHead += res;
        PRV_CONCAT_STR(bufferLink + head, PRV_LINK_BUFFER_SIZE - head, subHead, LINK_URI_SEPARATOR, LINK_URI_SEPARATOR_SIZE);
        res = utils_intToText(uriP->instanceId, bufferLink + head + subHead, PRV_LINK_BUFFER_SIZE - head - subHead);
        if (res <= 0) return -1;
        subHead += res;
        PRV_CONCAT_STR(bufferLink + head, PRV_LINK_BUFFER_SIZE - head, subHead, LINK_ITEM_END, LINK_ITEM_END_SIZE);
        tempUri.instanceId = uriP->instanceId;
        tempUri.flag = LWM2M_URI_FLAG_INSTANCE_ID;
        res = prv_serializeAttributes(contextP, &tempUri, bufferLink + head, head + subHead - 1, PRV_LINK_BUFFER_SIZE);
        if (res < 0) return -1;    // careful, 0 is valid
        if (res == 0) subHead = 0;    // rewind
        else subHead += res - 1;

        head += subHead;
    }

    for (index = 0; index < size && head < PRV_LINK_BUFFER_SIZE; index++)
    {
        res = prv_serializeLinkData(contextP, dataP + index, uriP, baseUriStr, baseUriLen, bufferLink + head, PRV_LINK_BUFFER_SIZE - head);
        if (res < 0) return -1;
        head += res;
    }

    if (head > 0)
    {
        head -= 1;

        *bufferP = (uint8_t *)lwm2m_malloc(head);
        if (*bufferP == NULL) return 0;
        memcpy(*bufferP, bufferLink, head);
    }

    return (int)head;
}
#endif