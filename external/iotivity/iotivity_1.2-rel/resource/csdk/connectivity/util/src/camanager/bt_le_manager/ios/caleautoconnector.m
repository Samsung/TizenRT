/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "cacommon.h"
#include "caleclient.h"
#include "camanagerleutil.h"
#include "camanagerdevice.h"
#include "caleautoconnector.h"
#include "cacommonutil.h"
#include "logger.h"
#include "octhread.h"

#define TAG "OIC_CA_LE_AUTO_CONN"

static const size_t MAX_RETRY_COUNT = 1;
static const size_t TIMEOUT = 30000000; //30sec
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

CAResult_t CAManagerStartAutoConnection(const char *remote_le_address)
{
    VERIFY_NON_NULL(remote_le_address, TAG, "remote_le_address is null");

    OIC_LOG(DEBUG, TAG, "IN - CAManagerStartAutoConnection");

    if (true == CAManagerGetAutoConnectionFlag(remote_le_address))
    {
        OIC_LOG(INFO, TAG, "auto connecting.");
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_connectRetryMutex);

    CBPeripheral *peripheral = CAManagerGetValueConnectedDeviceAddress(remote_le_address);
    if (!peripheral) {
        OIC_LOG_V(ERROR, TAG, "address : %s is not valid one", remote_le_address);
        oc_mutex_unlock(g_connectRetryMutex);
        return CA_STATUS_FAILED;
    }

    for (size_t retry_cnt = 0 ; retry_cnt < MAX_RETRY_COUNT ; retry_cnt++)
    {
        // there is retry logic 5 times when connectGatt call has failed
        // because BT adapter might be not ready yet.
        CAResult_t ret = CAManagerConnectGatt(remote_le_address);

        if (ret != CA_STATUS_OK) {
            OIC_LOG_V(ERROR, TAG, "CAManagerConnectGatt fail!!");
            oc_mutex_unlock(g_connectRetryMutex);
            return CA_STATUS_FAILED;
        }

        OIC_LOG_V(INFO, TAG, "retry will be started at least %d times after delay 30sec",
                  MAX_RETRY_COUNT - retry_cnt - 1);
        if (oc_cond_wait_for(g_connectRetryCond, g_connectRetryMutex, TIMEOUT) == 0)
        {
            OIC_LOG(INFO, TAG, "request to connect gatt is done");
            //CALEClientSetAutoConnectFlag(remote_le_address, false);
            oc_mutex_unlock(g_connectRetryMutex);
            return CA_STATUS_OK;
        }
        // time out. retry connection
        CALEClientConnectCancel(peripheral);
    }
    oc_mutex_unlock(g_connectRetryMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CAManagerStartAutoConnection");
    return CA_STATUS_OK;
}

CAResult_t CAManagerConnectGatt(const char *remote_le_address)
{
    VERIFY_NON_NULL_RET(remote_le_address, TAG, "remote_le_address", NULL);

    OIC_LOG(DEBUG, TAG, "IN - CAManagerConnectGatt");

    CBPeripheral *peripheral = CAManagerGetValueConnectedDeviceAddress(remote_le_address);

    if (!peripheral) {
        OIC_LOG_V(ERROR, TAG, "address : %s is not valid one", remote_le_address);
        return CA_STATUS_FAILED;
    }

    // request to connection with AutoConnection Flag
    OIC_LOG(INFO, TAG, "request to gatt connection for auto connection");
    CAResult_t res = CALEClientConnect(peripheral, true);
    if (CA_STATUS_FAILED == res)
    {
        OIC_LOG(INFO, TAG, "re-connection will be started");
        return CA_STATUS_FAILED;
    }

    // set flag auto connection is requested.
    CAManagerSetAutoConnectionFlag(remote_le_address, true);

    OIC_LOG(DEBUG, TAG, "OUT - CAManagerConnectGatt");
    return CA_STATUS_OK;
}

CAResult_t CAManagerProcessRecovery(uint16_t adapter_state)
{
    OIC_LOG(DEBUG, TAG, "IN - CAManagerProcessRecovery");

    oc_mutex_lock(g_recoveryMutex);
    CAResult_t res = CA_STATUS_OK;

    oc_mutex_unlock(g_recoveryMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CAManagerProcessRecovery");

    return res;
}

void CAManagerSignalToRetryCond(const char* address)
{
    OIC_LOG(DEBUG, TAG, "CAManagerSignalToRetryCond");
    if(true == CALEClientGetAutoConnectFlag(address))
    {
        if(g_connectRetryCond)
        {
            OIC_LOG(DEBUG, TAG, "signaling!");
            oc_mutex_lock(g_connectRetryMutex);
            oc_cond_signal(g_connectRetryCond);
            oc_mutex_unlock(g_connectRetryMutex);

        }
    }
}
