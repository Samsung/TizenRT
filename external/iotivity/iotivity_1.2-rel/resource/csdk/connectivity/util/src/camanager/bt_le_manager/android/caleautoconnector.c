/* ****************************************************************
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

#include <jni.h>
#include "cacommon.h"
#include "caleclient.h"
#include "camanagerleutil.h"
#include "camanagerdevice.h"
#include "caleautoconnector.h"
#include "cacommonutil.h"
#include "logger.h"
#include "octhread.h"

#define TAG "OIC_CA_LE_AUTO_CONN"

static const size_t MAX_RETRY_COUNT = 5;
static const size_t TIMEOUT = 1000000;
static const size_t WAITING_TIME = 500000;

static oc_mutex g_connectRetryMutex = NULL;
static oc_cond g_connectRetryCond = NULL;

static oc_mutex g_recoveryMutex = NULL;
static oc_cond g_recoveryCond = NULL;

CAResult_t CAManagerInitLEAutoConnection()
{
    if (NULL == g_connectRetryMutex)
    {
        g_connectRetryMutex = oc_mutex_new();
        if (NULL == g_connectRetryMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_connectRetryCond)
    {
        g_connectRetryCond = oc_cond_new();
        if (NULL == g_connectRetryCond)
        {
            OIC_LOG(ERROR, TAG, "oc_cond_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_recoveryMutex)
    {
        g_recoveryMutex = oc_mutex_new();
        if (NULL == g_recoveryMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_recoveryCond)
    {
        g_recoveryCond = oc_cond_new();
        if (NULL == g_recoveryCond)
        {
            OIC_LOG(ERROR, TAG, "oc_cond_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CAManagerTerminateLEAutoConnection()
{
    if (g_connectRetryCond)
    {
        oc_cond_signal(g_connectRetryCond);
        oc_cond_free(g_connectRetryCond);
        g_connectRetryCond = NULL;
    }

    if (g_connectRetryMutex)
    {
        oc_mutex_free(g_connectRetryMutex);
        g_connectRetryMutex = NULL;
    }

    if (g_recoveryCond)
    {
        oc_cond_signal(g_recoveryCond);
        oc_cond_free(g_recoveryCond);
        g_recoveryCond = NULL;
    }

    if (g_recoveryMutex)
    {
        oc_mutex_free(g_recoveryMutex);
        g_recoveryMutex = NULL;
    }
}

CAResult_t CAManagerStartAutoConnection(JNIEnv *env, jstring remote_le_address)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(remote_le_address, TAG, "remote_le_address is null");

    OIC_LOG(DEBUG, TAG, "IN - CAManagerStartAutoConnection");
    oc_mutex_lock(g_connectRetryMutex);

    bool isAutoConnecting = false;
    if (CA_STATUS_OK != CAManagerGetAutoConnectingFlag(env, remote_le_address, &isAutoConnecting))
    {
        OIC_LOG(DEBUG, TAG, "CAManagerIsAutoConnecting has failed");
        oc_mutex_unlock(g_connectRetryMutex);
        return CA_STATUS_FAILED;
    }

    if (isAutoConnecting)
    {
        OIC_LOG(INFO, TAG, "connection has been already in progress or completed");
        oc_mutex_unlock(g_connectRetryMutex);
        return CA_STATUS_FAILED;
    }

    CAResult_t res = CA_STATUS_OK;
    for (size_t retry_cnt = 0 ; retry_cnt < MAX_RETRY_COUNT ; retry_cnt++)
    {
        // there is retry logic 5 times when connectGatt call has failed
        // because BT adapter might be not ready yet.
        res = CAManagerConnectGatt(env, remote_le_address);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(INFO, TAG, "retry will be started at least %d times after delay 1sec",
                      MAX_RETRY_COUNT - retry_cnt - 1);
            if (oc_cond_wait_for(g_connectRetryCond, g_connectRetryMutex, TIMEOUT) == 0)
            {
                oc_mutex_unlock(g_connectRetryMutex);
                OIC_LOG(INFO, TAG, "request to connect gatt was canceled");
                return CA_STATUS_OK;
            }
            // time out. retry connection
        }
        else
        {
            OIC_LOG(INFO, TAG, "ConnectGatt has called successfully");
            break;
        }
    }
    oc_mutex_unlock(g_connectRetryMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CAManagerStartAutoConnection");
    return res;
}

CAResult_t CAManagerConnectGatt(JNIEnv *env, jstring remote_le_address)
{
    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(remote_le_address, TAG, "remote_le_address");

    OIC_LOG(DEBUG, TAG, "IN - CAManagerConnectGatt");

    jobject jni_bluetooth = CAManagerGetRemoteDevice(env, remote_le_address);
    if (!jni_bluetooth)
    {
        OIC_LOG(ERROR, TAG, "jni_bluetooth is null");
        return CA_STATUS_FAILED;
    }

    if (!CAManagerIsDeviceBonded(env, jni_bluetooth))
    {
        OIC_LOG(INFO, TAG, "device is BONDED_NONE");
    }

    // request to connection with AutoConnection Flag
    OIC_LOG(INFO, TAG, "request to gatt connection for auto connection");
    CAResult_t res = CALEClientDirectConnect(env, jni_bluetooth, JNI_TRUE);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(INFO, TAG, "re-connection will be started");
        return res;
    }

    // set flag auto connection is requested.
    CAManagerSetAutoConnectingFlag(env, remote_le_address, true);

    OIC_LOG(DEBUG, TAG, "OUT - CAManagerConnectGatt");
    return CA_STATUS_OK;
}

CAResult_t CAManagerProcessRecovery(JNIEnv *env, uint16_t adapter_state)
{
    VERIFY_NON_NULL(env, TAG, "env");
    OIC_LOG(DEBUG, TAG, "IN - CAManagerProcessRecovery");

    oc_mutex_lock(g_recoveryMutex);
    CAResult_t res = CA_STATUS_OK;

    switch(adapter_state)
    {
        case STATE_OFF:
            // adapter will be enabled automatically after WAITING_TIME.
            if (oc_cond_wait_for(g_recoveryCond, g_recoveryMutex, WAITING_TIME) == 0)
            {
                OIC_LOG(INFO, TAG, "BT recovery was canceled");
            }
            else
            {
                // enabled
                if (!CAManagerControlAdapter(env, true))
                {
                    OIC_LOG(ERROR, TAG, "BT recovery(enable) failure");
                    res = CA_STATUS_FAILED;
                }
            }
            CAManagerSetBTRecovery(false);
            break;
        case START_RECOVERY:
            if (!CAManagerControlAdapter(env, false))
            {
                OIC_LOG(ERROR, TAG, "BT recovery(disable) failure");
                res = CA_STATUS_FAILED;
            }
            CAManagerSetBTRecovery(true);
            break;
        default:
            break;
    }

    oc_mutex_unlock(g_recoveryMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CAManagerProcessRecovery");

    return res;
}
