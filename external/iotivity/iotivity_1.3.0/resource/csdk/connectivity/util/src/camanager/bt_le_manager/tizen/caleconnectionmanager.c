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

#include <bluetooth.h>
#include <bluetooth_type.h>
#include <bluetooth_internal.h>

#include "camanagerleinterface.h"
#include "cacommon.h"
#include "camessagehandler.h"
#include "caleserver.h"
#include "cagattservice.h"
#include "logger.h"

#define TAG "OIC_CA_MANAGER_TZ_LE"

static CAAdapterStateChangedCB g_adapterStateCB = NULL;
static CAConnectionStateChangedCB g_connStateCB = NULL;

static void CAManagerAdapterMonitorHandler(const CAEndpoint_t *info, CANetworkStatus_t status);
static void CAManagerConnectionMonitorHandler(CATransportAdapter_t adapter,
                                              const char *remoteAddress, bool connected);

void CASetLENetworkMonitorCallbacks(CAAdapterStateChangedCB adapterStateCB,
                                    CAConnectionStateChangedCB connStateCB)
{
    OIC_LOG(DEBUG, TAG, "CASetLENetworkMonitorCallbacks");

    g_adapterStateCB = adapterStateCB;
    CASetNetworkMonitorCallback(CAManagerAdapterMonitorHandler);

    g_connStateCB = connStateCB;
    CASetLEConnectionStateChangedCallback(CAManagerConnectionMonitorHandler);
}

CAResult_t CAManagerLEStartAdvertising()
{
    OIC_LOG(DEBUG, TAG, "CAManagerLEStartAdvertising");

    CAResult_t res = CALEStartAdvertise();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start le advertising [%d]", res);
        return res;
    }
    return res;
}

CAResult_t CAManagerLEStopAdvertising()
{
    OIC_LOG(DEBUG, TAG, "CAManagerLEStopAdvertising");

    CAResult_t res = CALEStopAdvertise();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to stop le advertising [%d]", res);
        return res;
    }
    return res;
}

CAResult_t CASetLEClientAutoConnectionDeviceInfo(const char * address)
{
    OIC_LOG(DEBUG, TAG, "CASetLEClientAutoConnectionDeviceInfo");
    (void)address;
    return CA_NOT_SUPPORTED;
}

CAResult_t CAUnsetLEClientAutoConnectionDeviceInfo(const char * address)
{
    OIC_LOG(DEBUG, TAG, "CAUnsetLEClientAutoConnectionDeviceInfo");
    (void)address;
    return CA_NOT_SUPPORTED;
}

static void CAManagerAdapterMonitorHandler(const CAEndpoint_t *info, CANetworkStatus_t status)
{
    if (CA_INTERFACE_DOWN == status)
    {
        if (info && g_adapterStateCB)
        {
            g_adapterStateCB(info->adapter, false);
            OIC_LOG(DEBUG, TAG, "Pass the disabled adapter state to upper layer");
        }
    }
    else if (CA_INTERFACE_UP == status)
    {
        if (info && g_adapterStateCB)
        {
            g_adapterStateCB(info->adapter, true);
            OIC_LOG(DEBUG, TAG, "Pass the enabled adapter state to upper layer");
        }
    }
}

static void CAManagerConnectionMonitorHandler(CATransportAdapter_t adapter,
                                              const char *remoteAddress, bool connected)
{
    (void)adapter;

    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is NULL");
        return;
    }

    if (connected)
    {
        if (g_connStateCB)
        {
            g_connStateCB(CA_ADAPTER_GATT_BTLE, remoteAddress, true);
            OIC_LOG(DEBUG, TAG, "Pass the connected device info to upper layer");

            // stop le advertising
            CAStopServerLEAdvertising();
        }
    }
    else
    {
        if (g_connStateCB)
        {
            g_connStateCB(CA_ADAPTER_GATT_BTLE, remoteAddress, false);
            OIC_LOG(DEBUG, TAG, "Pass the disconnected device info to upper layer");

            // start le advertising to receive new connection request.
            CAStartServerLEAdvertising();
        }
    }
}
