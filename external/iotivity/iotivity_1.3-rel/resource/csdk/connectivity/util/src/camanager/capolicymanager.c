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

#include "oic_string.h"
#include "logger.h"

#include "caconnectionmanager.h"

#define TAG "OIC_CM_POLICY"

/**
 * Mutex to synchronize the access to g_userConfigureMutex variable.
 */
static oc_mutex g_userConfigureMutex = NULL;

/**
 * User prefer variable to process connection manager logic.
 * default: Cloud TCP
 */
static CAConnectUserPref_t g_connectUserPrefer = CA_USER_PREF_CLOUD;

CAResult_t CAPolicyMgrSetConfiguration(CAConnectUserPref_t connPrefer)
{
    OIC_LOG(DEBUG, TAG, "CAPolicyMgrSetConfiguration");

    switch (connPrefer)
    {
        case CA_USER_PREF_CLOUD:
            OIC_LOG_V(DEBUG, TAG, "connPrefer: %d(CLOUD)", connPrefer);
            break;
        case CA_USER_PREF_LOCAL_UDP:
            OIC_LOG_V(DEBUG, TAG, "connPrefer: %d(LOCAL UDP)", connPrefer);
            break;
        case CA_USER_PREF_LOCAL_TCP:
            OIC_LOG_V(DEBUG, TAG, "connPrefer: %d(LOCAL TCP)", connPrefer);
            break;
        default:
            OIC_LOG_V(DEBUG, TAG, "Unsupported connPrefer type : %d", connPrefer);
            return CA_STATUS_FAILED;
    }

    if (NULL == g_userConfigureMutex)
    {
        g_userConfigureMutex = oc_mutex_new();
        if (NULL == g_userConfigureMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_MEMORY_ALLOC_FAILED;
        }
    }
    oc_mutex_lock(g_userConfigureMutex);
    g_connectUserPrefer = connPrefer;
    oc_mutex_unlock(g_userConfigureMutex);

    return CA_STATUS_OK;
}

CAResult_t CAPolicyMgrGetConfigure(CAConnectUserPref_t *connPrefer)
{
    if (NULL == g_userConfigureMutex)
    {
        g_userConfigureMutex = oc_mutex_new();
        if (NULL == g_userConfigureMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_MEMORY_ALLOC_FAILED;
        }
    }
    oc_mutex_lock(g_userConfigureMutex);
    *connPrefer = g_connectUserPrefer;
    oc_mutex_unlock(g_userConfigureMutex);
    return CA_STATUS_OK;
}

void CAPolicyMgrTerminate()
{
    if (g_userConfigureMutex)
    {
        oc_mutex_free(g_userConfigureMutex);
        g_userConfigureMutex = NULL;
    }
}
