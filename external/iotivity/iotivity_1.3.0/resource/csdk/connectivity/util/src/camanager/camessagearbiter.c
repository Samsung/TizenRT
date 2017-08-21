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

#include "camessagearbiter.h"
#include "camanagerutil.h"
#include "capolicymanager.h"

#define TAG "OIC_CM_MSG_ARBITER"

#define CA_MSG_ARBITER_DI_URI_LENGTH 47

#define CA_MSG_ARBITER_PING_URI "/oic/ping"

/**
 * List for remote device information table.
 */
static CMDeviceInfoList_t *g_remoteDeviceInfoList;

/**
 * Mutex to synchronize the access to remote device info list.
 */
static oc_mutex g_remoteDeviceInfoListMutex = NULL;

/**
 * Initializes all required mutex variable for messge arbiter.
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
static CAResult_t CAMsgArbiterInitMutexVariables();

/**
 * Terminates all required mutex variable for messge arbiter.
 */
static void CAMsgArbiterTerminateMutexVariables();

CAResult_t CAMsgArbiterInitialize()
{
    OIC_LOG(DEBUG, TAG, "CAMsgArbiterInitialize");

    return CAMsgArbiterInitMutexVariables();
}

CAResult_t CAMsgArbiterTerminate()
{
    OIC_LOG(DEBUG, TAG, "CAMsgArbiterTerminate");

    if (g_remoteDeviceInfoList)
    {
        oc_mutex_lock(g_remoteDeviceInfoListMutex);
        CAMgrUtilDestroyDevInfoList(g_remoteDeviceInfoList);
        oc_mutex_unlock(g_remoteDeviceInfoListMutex);
    }
    CAMsgArbiterTerminateMutexVariables();
    CAPolicyMgrTerminate();

    return CA_STATUS_OK;
}

static CAResult_t CAMsgArbiterInitMutexVariables()
{
    if (NULL == g_remoteDeviceInfoListMutex)
    {
        g_remoteDeviceInfoListMutex = oc_mutex_new();
        if (NULL == g_remoteDeviceInfoListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }
    return CA_STATUS_OK;
}

static void CAMsgArbiterTerminateMutexVariables()
{
    if (g_remoteDeviceInfoListMutex)
    {
        oc_mutex_free(g_remoteDeviceInfoListMutex);
        g_remoteDeviceInfoListMutex = NULL;
    }
}

CAResult_t CAMsgArbiterGetMessageData(CAData_t *data)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(data, TAG, "data is NULL");

    CMDeviceInfo_t *targetInfo = NULL;

    if (NULL != data->requestInfo &&
            (NULL == data->requestInfo->info.resourceUri ||
            !strcmp(data->requestInfo->info.resourceUri, CA_MSG_ARBITER_PING_URI)))
    {
        OIC_LOG(DEBUG, TAG, "ping uri and null are ignored in messsage arbiter");
        return CA_STATUS_FAILED;
    }

    CAConnectUserPref_t connPrefer = CA_USER_PREF_LOCAL_TCP;
    CAResult_t ret = CAPolicyMgrGetConfigure(&connPrefer);

    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(DEBUG, TAG, "CAPolicyMgrGetConfigure is failed");
        return ret;
    }
    else
    {
        if (CA_USER_PREF_CLOUD != connPrefer && CA_REQUEST_DATA == data->dataType
            && NULL != data->remoteEndpoint && NULL != data->requestInfo)
        {
            char *deviceId = OICStrdup(data->remoteEndpoint->remoteId);

            char *di= CAMgrUtilGetDeviceId(data->requestInfo->info.resourceUri);
            //if deviceId is null && hasUUID, extract device id from uri.
            if ((NULL == deviceId || strcmp(deviceId, "") == 0) && NULL != di)
            {
                deviceId = di;
            }
            else if (NULL != di)
            {
                OICFree(di);
            }

            oc_mutex_lock(g_remoteDeviceInfoListMutex);
            ret = CAMgrUtilGetDevInfo(g_remoteDeviceInfoList, deviceId, &targetInfo);
            oc_mutex_unlock(g_remoteDeviceInfoListMutex);

            if (CA_STATUS_OK != ret)
            {
                OIC_LOG_V(DEBUG, TAG, "device id[%s] is not in connection switch info list",
                        deviceId);
                return ret;
            }

            if (NULL != targetInfo->d2dInfo.addr)
            {
                OICStrcpyPartial(data->remoteEndpoint->addr, sizeof(data->remoteEndpoint->addr),
                             targetInfo->d2dInfo.addr, strlen(targetInfo->d2dInfo.addr) + 1);
                if (CA_USER_PREF_LOCAL_TCP == connPrefer &&
                    (targetInfo->d2dInfo.adapter & CA_ADAPTER_TCP))
                {
                    data->remoteEndpoint->port = targetInfo->d2dInfo.tcpPort;
                    data->remoteEndpoint->adapter = CA_ADAPTER_TCP;
                }
                else if (targetInfo->d2dInfo.adapter & CA_ADAPTER_IP)
                {
                    data->remoteEndpoint->port = targetInfo->d2dInfo.udpPort;
                    data->remoteEndpoint->adapter = CA_ADAPTER_IP;
                }
                data->remoteEndpoint->flags = targetInfo->d2dInfo.flags;

                if (strlen(data->requestInfo->info.resourceUri) > CA_MSG_ARBITER_DI_URI_LENGTH)
                {
                    CAURI_t resourceUri = OICStrdup(data->requestInfo->info.resourceUri
                                                    + CA_MSG_ARBITER_DI_URI_LENGTH);
                    OICFree(data->requestInfo->info.resourceUri);
                    data->requestInfo->info.resourceUri = resourceUri;
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "device id[%s] received resourceUri[%s] too short!", deviceId,
                              data->requestInfo->info.resourceUri);
                }

                OIC_LOG_V(DEBUG, TAG, "device id[%s] switched to D2D connect", deviceId);
                OIC_LOG_V(DEBUG, TAG, "local addr[%s] local port[%d] adapter[%s]",
                          data->remoteEndpoint->addr, data->remoteEndpoint->port,
                          data->remoteEndpoint->adapter == CA_ADAPTER_IP ? "IP" : "TCP");
                OIC_LOG_V(DEBUG, TAG, "resource uri : %s", data->requestInfo->info.resourceUri);
            }
            else
            {
                OIC_LOG_V(DEBUG, TAG, "local address is not set.");
            }

            OICFree(deviceId);
            deviceId = NULL;
        }
        else
        {
            return CA_STATUS_FAILED;
        }
    }
    OIC_LOG(DEBUG, TAG, "OUT");
    return ret;
}

CAResult_t CAMsgArbiterUpdateDeviceInfo(const CAEndpoint_t *endpoint, bool isCloud)
{
    OIC_LOG(DEBUG, TAG, "IN - CAMsgArbiterUpdateDeviceInfo");

    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(endpoint->remoteId, TAG, "remoteId");

    CAResult_t ret = CA_STATUS_FAILED;

    if (isCloud)
    {
        oc_mutex_lock(g_remoteDeviceInfoListMutex);
        ret = CAMgrUtilAddDevInfoToList(&g_remoteDeviceInfoList, endpoint);
        oc_mutex_unlock(g_remoteDeviceInfoListMutex);

        if (CA_STATUS_OK != ret)
        {
            OIC_LOG(ERROR, TAG, "AddToList failed!");
            return ret;
        }

        OIC_LOG_V(DEBUG, TAG, "device id[%s] is successfully added into list!",
                  endpoint->remoteId);
    }
    else
    {
        oc_mutex_lock(g_remoteDeviceInfoListMutex);
        ret = CAMgrUtilUpdateDevInfo(g_remoteDeviceInfoList, endpoint);
        oc_mutex_unlock(g_remoteDeviceInfoListMutex);

        if (CA_STATUS_OK != ret)
        {
            OIC_LOG_V(ERROR, TAG, "device id[%s] is not included in list", endpoint->remoteId);
            return ret;
        }

        OIC_LOG_V(DEBUG, TAG, "device id[%s] is successfully updated into list!",
                  endpoint->remoteId);
    }

    OIC_LOG(DEBUG, TAG, "OUT - CAMsgArbiterUpdateDeviceInfo");
    return ret;
}

CAResult_t CAMsgArbiterResetDeviceInfo()
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_remoteDeviceInfoListMutex);
    CAResult_t ret = CAMgrUtilResetDevInfo(g_remoteDeviceInfoList);
    oc_mutex_unlock(g_remoteDeviceInfoListMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return ret;
}
