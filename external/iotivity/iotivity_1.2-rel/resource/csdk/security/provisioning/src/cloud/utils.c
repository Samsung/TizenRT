/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#include "utils.h"

#include "logger.h"
#include "payload_logging.h"
#include "oic_malloc.h"
#include "doxmresource.h"
#include "srmutility.h"
#include "pmutility.h"

#define TAG "OIC_CLOUD_UTILS"

typedef struct {
    void *ctx;
    OCCloudResponseCB cb;
    UserFunctionCB fn;
    void *params;
}ContextInfo_t;

char *DEFAULT_QUERY  = COAP_TCP_QUERY;
char *DEFAULT_PREFIX = COAP_TCP_PREFIX;

/**
 * This function sets required CoAP prefix depending on
 * secure or non-secure connection
 *
 * @param[in] secure   secure flag
 */
void setCoapPrefix(bool secure)
{
    if (secure)
    {
        DEFAULT_QUERY  = COAPS_TCP_QUERY;
        DEFAULT_PREFIX = COAPS_TCP_PREFIX;
    }
    else
    {
        DEFAULT_QUERY  = COAP_TCP_QUERY;
        DEFAULT_PREFIX = COAP_TCP_PREFIX;
    }
}

/**
 * This hepler function handles response from each request
 * During execution it calls proper callbacks
 *
 * @param[in] ctx             context
 * @param[in] handle          handle
 * @param[in] clientResponse  response from peer
 */
static OCStackApplicationResult handleResponse(void *ctx,
                                              OCDoHandle handle,
                                              OCClientResponse *clientResponse)
{
    OC_UNUSED(handle);

    if (NULL == clientResponse)
    {
        OIC_LOG_V(ERROR, TAG, "Received null response from client");
        return OC_STACK_DELETE_TRANSACTION;
    }

    OIC_LOG_V(DEBUG, TAG, "Received callback with response code: %d", clientResponse->result);

    if (clientResponse->payload)
    {
        OIC_LOG(DEBUG, TAG, "Payload received:");
        OIC_LOG_PAYLOAD(DEBUG, clientResponse->payload);
    }

    if (ctx)
    {
        OCStackResult result = clientResponse->result;
        void *data = NULL;

        ContextInfo_t *info = (ContextInfo_t *)ctx;

        if (info->fn)
        {
            result = ((UserFunctionCB)info->fn)(info->params, &data, clientResponse);
        }

        if (info->cb)
        {
            ((OCCloudResponseCB)info->cb)(info->ctx, result, data);
        }
    }

    return OC_STACK_DELETE_TRANSACTION;
}

static void deleteContextInfo(void *info)
{
    OICFree(info);
}

void fillCallbackData(OCCallbackData *cbData, void *ctx, OCCloudResponseCB cb,
                                              UserFunctionCB fn, void *params)
{
    memset(cbData, 0, sizeof(OCCallbackData));

    ContextInfo_t *info = OICCalloc(1, sizeof(ContextInfo_t));
    if (!info)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate memory for info");
        return;
    }

    info->ctx = ctx;
    info->cb  = cb;
    info->fn  = fn;
    info->params = params;

    cbData->cb = handleResponse;
    cbData->cd = deleteContextInfo;
    cbData->context = info;
}

char * getDeviceId()
{
    char *deviceId = NULL;
    OicUuid_t uuid;

    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    memset(&uuid, 0, sizeof(uuid));
    if (OC_STACK_OK != GetDoxmDeviceID(&uuid))
    {
        OIC_LOG(ERROR, TAG, "Cann't get the device id(GetDoxmDeviceID)");
        goto exit;
    }

    if (OC_STACK_OK != ConvertUuidToStr(&uuid, &deviceId))
    {
        OIC_LOG(ERROR, TAG, "Cann't get the device id(ConvertUuidToStr)");
        goto exit;
    }

    OIC_LOG_V(DEBUG, TAG, "device Id: %s", deviceId);
exit:
    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);
    return deviceId;
}

/**
 * Clear previously filled array of strings
 *
 * @param[out] list           array of strings structure to clear
 * @param[in] count           elements count
 */
void clearStringArray(stringArray_t *list)
{
    if (!list || !list->array)
    {
        return;
    }

    for (size_t i = 0; i < list->length; i++)
    {
        OICFree(list->array[i]);
    }
    OICFree(list->array);
}

