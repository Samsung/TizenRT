/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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

/**
 * @file
 *
 * This file provides helper functions for EDR adapter.
 */

#include "caedrutils.h"

#include <bluetooth.h>

#include "logger.h"
#include "uarraylist.h"

/**
 * Pending devices list for data transfer.
 */
static u_arraylist_t *g_deviceStateList = NULL;

bool CAEDRIsServiceSupported(const char **serviceUUID, int32_t serviceCount,
                                 const char *matchService)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    if (NULL == serviceUUID || 0 == serviceCount || NULL == matchService)
    {
        OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Invalid input");
        return false;
    }

    for (int i = 0; i < serviceCount; i++)
    {
        if (!strcasecmp(serviceUUID[i], matchService))
        {
            OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Service found !");
            return true;
        }
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return false;
}

CAConnectedDeviceInfo_t *CAEDRGetDeviceInfoFromAddress(const char *remoteAddress)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "CAEDRGetDeviceInfoFromAddress");

    if (!remoteAddress)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "remoteAddress is null");
        return NULL;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    for (uint32_t index = 0; index < length; index++)
    {
        CAConnectedDeviceInfo_t* deviceInfo =
                (CAConnectedDeviceInfo_t*) u_arraylist_get(g_deviceStateList, index);
        if (!deviceInfo)
        {
            OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "deviceInfo object is null");
            continue;
        }

        if (!strcmp((const char*) deviceInfo->address, remoteAddress))
        {
            return deviceInfo;
        }
    }
    return NULL;
}

CAResult_t CAEDRAddDeviceInfoToList(const char *remoteAddress, CAConnectedDeviceInfo_t *deviceInfo)
{
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "remoteAddress is null");
        return CA_STATUS_INVALID_PARAM;
    }

    if (!deviceInfo)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "deviceInfo is null");
        return CA_STATUS_INVALID_PARAM;
    }

    for (int i = 0; i < CA_MACADDR_SIZE; i++)
    {
        deviceInfo->address[i] = (uint8_t) remoteAddress[i];
    }

    if (!g_deviceStateList)
    {
        g_deviceStateList = u_arraylist_create();
        if (!g_deviceStateList)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to create Device list");
            return CA_STATUS_FAILED;
        }
    }

    if (!u_arraylist_add(g_deviceStateList, (void *) deviceInfo))
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to Add Device to list");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}