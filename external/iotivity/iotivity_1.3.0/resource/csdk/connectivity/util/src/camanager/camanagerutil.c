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

#include "camanagerutil.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "caadapterutils.h"
#include "logger.h"
#include "ocrandom.h"
#include "oic_string.h"
#include "oic_malloc.h"

/**
 * Logging tag for module name
 */
#define TAG "OIC_CM_UTIL"

/**
 * This function is used to free DeviceInfo structure.
 *
 * @param[in] devInfo   Device info to be freed.
 */
static void CAMgrUtilDevInfoFree(CMDeviceInfo_t *devInfo);

/**
 * This function is used to print all components in list.
 *
 * @param[in]  devInfoList  Pointer to IP connection info table which contains
 *                          D2D,D2S ip related informaions.
 */
static void CAMgrUtilDevInfoListPrint(CMDeviceInfoList_t *devInfoList);

/**
 * This function is used to make DeviceInfo structure.
 *
 * @param[in] endpoint   remote Endpoint to be added in IP connection info table.
 *
 * @return :: new alloced CMDeviceInfo_t pointer structure or NULL.
 */
static CMDeviceInfo_t* CAMgrUtilDevInfoCreate(const CAEndpoint_t *endpoint);


CAResult_t CAMgrUtilAddDevInfoToList(CMDeviceInfoList_t **devInfoList,
                                     const CAEndpoint_t *endpoint)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(devInfoList, TAG, "devInfoList");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(endpoint->remoteId, TAG, "deviceId");
    VERIFY_NON_NULL(endpoint->addr, TAG, "address");

    CMDeviceInfo_t *node = NULL;
    CAResult_t ret = CAMgrUtilGetDevInfo(*devInfoList, endpoint->remoteId, &node);

    if (CA_STATUS_OK == ret)
    {
        OIC_LOG_V(ERROR, TAG, "deviceId[%s] is already added into list", endpoint->remoteId);
        return CA_STATUS_FAILED;
    }

    CMDeviceInfo_t *newDevInfo = CAMgrUtilDevInfoCreate(endpoint);
    if (NULL == newDevInfo)
    {
        OIC_LOG(ERROR, TAG, "CACMRemoteDeviceCreate failed!");
        return CA_MEMORY_ALLOC_FAILED;
    }

    CMDeviceInfoList_t *newNode = (CMDeviceInfoList_t *)OICCalloc(1, sizeof(CMDeviceInfoList_t));
    if (NULL == newNode)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed");
        return CA_MEMORY_ALLOC_FAILED;
    }

    newNode->deviceInfo = newDevInfo;
    newNode->next = NULL;

    if (*devInfoList != NULL)
    {
        newNode->next = *devInfoList;
    }
    *devInfoList = newNode;

    OIC_LOG_V(DEBUG, TAG, "device id[%s] added to list", newDevInfo->deviceId);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAMgrUtilGetDevInfo(CMDeviceInfoList_t *devInfoList, const char *deviceId,
                               CMDeviceInfo_t **devInfo)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(devInfoList, TAG, "devInfoList");
    VERIFY_NON_NULL(deviceId, TAG, "deviceId");

    CMDeviceInfoList_t *cur = devInfoList;
    *devInfo = NULL;
    while (cur != NULL)
    {
        if (NULL != cur->deviceInfo && !strcasecmp(cur->deviceInfo->deviceId, deviceId))
        {
            *devInfo = cur->deviceInfo;
            OIC_LOG(DEBUG, TAG, "OUT - found Device info");
            return CA_STATUS_OK;
        }
        cur = cur->next;
    }

    OIC_LOG(DEBUG, TAG, "OUT - can't find Device info");
    return CA_STATUS_FAILED;
}

CAResult_t CAMgrUtilUpdateDevInfo(CMDeviceInfoList_t *devInfoList, const CAEndpoint_t *endpoint)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(devInfoList, TAG, "devInfoList");
    VERIFY_NON_NULL(endpoint, TAG, "endpoint");
    VERIFY_NON_NULL(endpoint->remoteId, TAG, "deviceId");
    VERIFY_NON_NULL(endpoint->addr, TAG, "localAddr");

    CMDeviceInfo_t *node = NULL;
    CAResult_t ret = CAMgrUtilGetDevInfo(devInfoList, endpoint->remoteId, &node);

    if (CA_STATUS_OK != ret || NULL == node)
    {
        OIC_LOG_V(ERROR, TAG, "deviceId[%s] isn't included in list", endpoint->remoteId);
        return ret;
    }

    node->d2dInfo.adapter |= endpoint->adapter;
    node->d2dInfo.flags |= endpoint->flags;

    if (NULL != node->d2dInfo.addr)
    {
        OICFree(node->d2dInfo.addr);
    }
    node->d2dInfo.addr = OICStrdup(endpoint->addr);

    if (CA_ADAPTER_IP & endpoint->adapter)
    {
        node->d2dInfo.udpPort = endpoint->port;
    }
    else if (CA_ADAPTER_TCP & endpoint->adapter)
    {
        node->d2dInfo.tcpPort = endpoint->port;
    }

    CAMgrUtilDevInfoListPrint(devInfoList);
    OIC_LOG(DEBUG, TAG, "OUT");

    return CA_STATUS_OK;
}

CAResult_t CAMgrUtilResetDevInfo(CMDeviceInfoList_t *devInfoList)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(devInfoList, TAG, "devInfoList");

    CMDeviceInfoList_t *cur = devInfoList;
    CMDeviceInfo_t *devInfo = NULL;

    while (cur != NULL)
    {
        devInfo = cur->deviceInfo;

        if (NULL != devInfo && NULL != devInfo->d2dInfo.addr)
        {
            devInfo->d2dInfo.adapter = 0;
            devInfo->d2dInfo.flags = 0;
            OICFree(devInfo->d2dInfo.addr);
            devInfo->d2dInfo.addr = NULL;
            devInfo->d2dInfo.udpPort = 0;
            devInfo->d2dInfo.tcpPort = 0;
        }

        cur = cur->next;
    }

    CAMgrUtilDevInfoListPrint(devInfoList);
    OIC_LOG(DEBUG, TAG, "OUT");

    return CA_STATUS_OK;
}

void CAMgrUtilDestroyDevInfoList(CMDeviceInfoList_t *devInfoList)
{
    OIC_LOG(DEBUG, TAG, "IN");

    while (devInfoList != NULL)
    {
        CMDeviceInfoList_t *temp = devInfoList;
        devInfoList = devInfoList->next;
        CAMgrUtilDevInfoFree(temp->deviceInfo);
        OICFree(temp);
        temp = NULL;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
}

static CMDeviceInfo_t* CAMgrUtilDevInfoCreate(const CAEndpoint_t *endpoint)
{
    OIC_LOG(DEBUG, TAG, "IN");

    assert(endpoint);
    assert(endpoint->remoteId);
    assert(endpoint->addr);

    CMDeviceInfo_t *node = (CMDeviceInfo_t *)OICCalloc(1, sizeof(CMDeviceInfo_t));
    if (NULL == node)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed");
        return NULL;
    }

    node->deviceId = OICStrdup(endpoint->remoteId);

    node->d2sInfo.adapter = endpoint->adapter;
    node->d2sInfo.flags = endpoint->flags;
    node->d2sInfo.addr = OICStrdup(endpoint->addr);
    node->d2sInfo.udpPort = 0;
    node->d2sInfo.tcpPort = endpoint->port;

    node->d2dInfo.adapter = 0;
    node->d2dInfo.flags = 0;
    node->d2dInfo.addr = NULL;
    node->d2dInfo.udpPort = 0;
    node->d2dInfo.tcpPort = 0;

    OIC_LOG_V(DEBUG, TAG, "remoteId[%s] cloudAddr[%s] cloudPort[%d]",
              node->deviceId, node->d2sInfo.addr, node->d2sInfo.tcpPort);

    OIC_LOG(DEBUG, TAG, "OUT");

    return node;
}

static void CAMgrUtilDevInfoFree(CMDeviceInfo_t *devInfo)
{
    if (NULL != devInfo)
    {
        if (NULL != devInfo->deviceId)
        {
            OICFree(devInfo->deviceId);
        }
        if (NULL != devInfo->d2dInfo.addr)
        {
            OICFree(devInfo->d2dInfo.addr);
        }
        if (NULL != devInfo->d2sInfo.addr)
        {
            OICFree(devInfo->d2sInfo.addr);
        }

        OICFree(devInfo);
    }
}

static void CAMgrUtilDevInfoListPrint(CMDeviceInfoList_t *devInfoList)
{
    size_t index = 1;
    CMDeviceInfoList_t *cur = devInfoList;
    CMDeviceInfo_t *devInfo = NULL;

    while (cur != NULL)
    {
        devInfo = cur->deviceInfo;
        OIC_LOG(DEBUG, TAG, "---------------------------------------------------------------");
        if (NULL != devInfo->deviceId)
        {
            OIC_LOG_V(DEBUG, TAG, "     %zu. device ID : %s", index, devInfo->deviceId);
        }
        OIC_LOG_V(DEBUG, TAG, "         local address : %s", devInfo->d2dInfo.addr);
        OIC_LOG_V(DEBUG, TAG, "         local udp port : %d", devInfo->d2dInfo.udpPort);
        OIC_LOG_V(DEBUG, TAG, "         local tcp port : %d", devInfo->d2dInfo.tcpPort);
        OIC_LOG_V(DEBUG, TAG, "         cloud address : %s", devInfo->d2sInfo.addr);
        OIC_LOG_V(DEBUG, TAG, "         cloud tcp port : %d", devInfo->d2sInfo.tcpPort);
        OIC_LOG(DEBUG, TAG, "---------------------------------------------------------------");
        cur = cur->next;
        index++;
    }
    OIC_LOG_V(DEBUG, TAG, "Done.");
}

char *CAMgrUtilGetDeviceId(char *uri)
{
    VERIFY_NON_NULL_RET(uri, TAG, "uri", NULL);

    char *result = NULL;
    char *copyUri = OICStrdup(uri);

    size_t tokSize = 0;
    char *str = copyUri;

    char *savePtr = NULL;
    char *tok = strtok_r(str, "/", &savePtr);
    while (tok != NULL)
    {
        tokSize = strlen(tok);
        if (tokSize == CA_MAX_IDENTITY_SIZE - 1)
        {
            if (OCIsUUID(tok))
            {
                OIC_LOG_V(DEBUG, TAG, "[%s] has uuid", uri);
                result = OICStrdup(tok);
            }
        }
        tok = strtok_r(NULL, "/", &savePtr);
    }
    OICFree(copyUri);
    copyUri = NULL;

    return result;
}
