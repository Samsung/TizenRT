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
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caadapterutils.h"
#include "cainterface.h"
#include "camessagehandler.h"
#include "caremotehandler.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "octhread.h"
#include "logger.h"
#include "caadapterutils.h"

#include "caconnectionmanager.h"
#include "capolicymanager.h"

#define TAG "OIC_CM"

static oc_mutex g_threadCMConfigureMutex = NULL;

// context for connection manager
static CAConnectionManagerContext_t g_context = {.sendThreadFunc = NULL,
                                                 .receivedThreadFunc = NULL,
                                                 .dataList = NULL};

void CAStartConnectionManagerService(CMConfigureInfo_t info)
{
    OIC_LOG(DEBUG, TAG, "CAStartConnectionManagerService");

    oc_mutex_lock(g_threadCMConfigureMutex);
    CMSetConfigure(info);
    oc_mutex_unlock(g_threadCMConfigureMutex);
}

CAData_t* CAGetConnectionManagerMessageData(CAData_t *data)
{
    OIC_LOG(DEBUG, TAG, "CAGetConnectionManagerMessageData");

    VERIFY_NON_NULL_RET(data, TAG, "data is null", NULL);

    // TODO
    // decide specific reqeust/response message

    return data;
}

CAResult_t CAInitializeConnectionManager(CASendThreadFunc sendThreadFunc,
                                         CAReceiveThreadFunc receivedThreadFunc)
{
    OIC_LOG(DEBUG, TAG, "CAInitializeConnectionManager");

    if (!g_context.sendThreadFunc)
    {
        g_context.sendThreadFunc = sendThreadFunc;
    }

    if (!g_context.receivedThreadFunc)
    {
        g_context.receivedThreadFunc = receivedThreadFunc;
    }

    if (!g_context.dataList)
    {
        g_context.dataList = u_arraylist_create();
    }

    CAResult_t res = CAInitConnectionManagerMutexVariables();
    if (CA_STATUS_OK != res)
    {
        u_arraylist_free(&g_context.dataList);
        g_context.dataList = NULL;
        OIC_LOG(ERROR, TAG, "init has failed");
    }
    return res;
}

void CATerminateConnectionManager()
{
    OIC_LOG(DEBUG, TAG, "CATerminateConnectionManager");

    if (g_context.dataList)
    {
        // TODO
        // Remove all of management data();
        u_arraylist_free(&g_context.dataList);
    }
    CATerminateConnectionManagerMutexVariables();
}

CAResult_t CAInitConnectionManagerMutexVariables()
{
    if (!g_context.dataListMutex)
    {
        g_context.dataListMutex = oc_mutex_new();
        if (!g_context.dataListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (!g_context.dataSenderMutex)
    {
        g_context.dataSenderMutex = oc_mutex_new();
        if (!g_context.dataSenderMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            CATerminateConnectionManagerMutexVariables();
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadCMConfigureMutex)
    {
        g_threadCMConfigureMutex = oc_mutex_new();
        if (NULL == g_threadCMConfigureMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CATerminateConnectionManagerMutexVariables()
{
    if (g_context.dataListMutex)
    {
        oc_mutex_free(g_context.dataListMutex);
        g_context.dataListMutex = NULL;
    }

    if (g_context.dataSenderMutex)
    {
        oc_mutex_free(g_context.dataSenderMutex);
        g_context.dataSenderMutex = NULL;
    }

    if (g_threadCMConfigureMutex)
    {
        oc_mutex_free(g_threadCMConfigureMutex);
        g_threadCMConfigureMutex = NULL;
    }
}
