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

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "octypes.h"
#include "ownershiptransfermanager.h"
#include "utlist.h"
#include "otmcontextlist.h"

#define TAG "OIC_OTM_CTX"

/**
 * List for saving the OTMContext to be used while ownership transfer.
 */
static OTMContextItem_t* g_otmCtxList = NULL;

void RemoveOTMContext(const char* addr, uint16_t port)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    size_t ctxCnt = 0;

    if (NULL != addr && 0 != port)
    {
        OTMContextItem_t* item = NULL;
        OTMContextItem_t* temp = NULL;

        LL_FOREACH_SAFE(g_otmCtxList, item, temp)
        {
            ctxCnt++;
            if (strncmp(addr, item->endpoint.addr, sizeof(item->endpoint.addr)) == 0 &&
                port == item->endpoint.port)
            {
                OIC_LOG_V(DEBUG, TAG, "Remove [%s:%d]'s context from OTMContext list", addr, port);
                item->otmCtx = NULL;
                LL_DELETE(g_otmCtxList, item);
                OICFree(item);
                ctxCnt--;
                break;
            }
        }

        if (0 == ctxCnt)
        {
            g_otmCtxList = NULL;
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

void DeleteOTMContextList()
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    OTMContextItem_t* item = NULL;
    OTMContextItem_t* temp = NULL;
    int index = 0;

    LL_FOREACH_SAFE(g_otmCtxList, item, temp)
    {
        LL_DELETE(g_otmCtxList, item);
        if (item)
        {
            OIC_LOG_V(DEBUG, TAG, "Found OTM Context Item #%d in List", index++);
            if (item->otmCtx)
            {
                OIC_LOG(DEBUG, TAG, "Found OTM Context is exist.");
                if (item->otmCtx->selectedDeviceInfo)
                {
                    OIC_LOG_V(DEBUG, TAG, "Remove [%s:%d]'s context from OTMContext list",
                              item->otmCtx->selectedDeviceInfo->endpoint.addr,
                              item->otmCtx->selectedDeviceInfo->securePort);
                }
                if (item->otmCtx->ctxResultArray)
                {
                    OIC_LOG(DEBUG, TAG, "Found Result Array is exist in OTM Context.");
                    OICFree(item->otmCtx->ctxResultArray);
                }
                OICFree(item->otmCtx);
            }
            OICFree(item);
        }
    }
    g_otmCtxList = NULL;
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}

OCStackResult AddOTMContext(OTMContext_t* ctx, const char* addr, uint16_t port)
{
    OTMContextItem_t* item = NULL;
    OTMContextItem_t* temp = NULL;
    OTMContextItem_t* newItem = NULL;

    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (NULL == ctx || NULL == addr || 0 == strlen(addr) || 0 == port)
    {
        return OC_STACK_INVALID_PARAM;
    }

    LL_FOREACH_SAFE(g_otmCtxList, item, temp)
    {
            if (strncmp(addr, item->endpoint.addr, sizeof(item->endpoint.addr)) == 0 &&
                port == item->endpoint.port)
            {
                //if OTM Context already exists, just return OC_STACK_OK.
                OIC_LOG(DEBUG, TAG, "Same OTMContext already exists.");
                item->otmCtx->ctxResultCallback = ctx->ctxResultCallback;
                return OC_STACK_OK;
            }
    }

    newItem = (OTMContextItem_t*)OICCalloc(1, sizeof(OTMContextItem_t));
    if (NULL == newItem)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory.");
        return OC_STACK_NO_MEMORY;
    }

    OIC_LOG_V(DEBUG, TAG, "Add [%s:%d]'s context to OTMContext list", addr, port);

    newItem->otmCtx = ctx;
    OICStrcpy(newItem->endpoint.addr, sizeof(newItem->endpoint.addr), addr);
    newItem->endpoint.port = port;
    LL_APPEND(g_otmCtxList, newItem);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return OC_STACK_OK;
}

OTMContext_t* GetOTMContext(const char* addr, uint16_t port)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (NULL != addr && 0 != port)
    {
        OTMContextItem_t* item = NULL;
        OTMContextItem_t* temp = NULL;

        LL_FOREACH_SAFE(g_otmCtxList, item, temp)
        {
            if (strncmp(addr, item->endpoint.addr, sizeof(item->endpoint.addr)) == 0 &&
               port == item->endpoint.port)
            {
                OIC_LOG_V(DEBUG, TAG, "Found the OTMContext for [%s:%d]", addr, port);
                return item->otmCtx;
            }
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return NULL;
}

