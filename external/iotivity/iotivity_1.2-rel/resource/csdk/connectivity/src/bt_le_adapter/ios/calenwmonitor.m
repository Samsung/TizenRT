/******************************************************************
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

#include <stdio.h>
#include "logger.h"
#include "calenwmonitor.h"
#include "caleclient.h"
#include "caleserver.h"
#include "caleutils.h"
#include "caleinterface.h"
#include "caadapterutils.h"

#include "octhread.h"

#define TAG PCF("OIC_CA_LE_MONITOR")

static oc_cond g_deviceStateCbCond = NULL;
static oc_mutex g_deviceStateCbMutex = NULL;

/**
 * @var g_bleDeviceStateChangedCallback
 * @brief Maintains the callback to be notified on device state changed.
 */
static CALEDeviceStateChangedCallback g_bleDeviceStateChangedCallback = NULL;

/**
 * @var g_bleConnectionStateChangedCallback
 * @brief Maintains the callback to be notified on device state changed.
 */
static CALEConnectionStateChangedCallback g_bleConnectionStateChangedCallback = NULL;

/**
 * @var g_bleDeviceStateChangedCbMutex
 * @brief Mutex to synchronize access to the deviceStateChanged Callback when the state
 *           of the LE adapter gets change.
 */
static oc_mutex g_bleDeviceStateChangedCbMutex = NULL;

/**
 * @var g_bleConnectionStateChangedCbMutex
 * @brief Mutex to synchronize access to the LE ConnectionStateChanged Callback when the state
 *           of the LE adapter gets change.
 */
static oc_mutex g_bleConnectionStateChangedCbMutex = NULL;

void CALESetAdapterStateCallback(CALEDeviceStateChangedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "CALESetAdapterStateCallback");
    g_bleDeviceStateChangedCallback = callback;

    oc_cond_signal(g_deviceStateCbCond);
}

CAResult_t CAInitializeLEAdapter(const ca_thread_pool_t threadPool)
{
    OIC_LOG(DEBUG, TAG, "IN - CAInitializeLEAdapter");
    (void)threadPool;
    OIC_LOG(DEBUG, TAG, "OUT - CAInitializeLEAdapter");
    return CA_STATUS_OK;
}

CAResult_t CAStartLEAdapter()
{
    // Nothing to do.

    return CA_STATUS_OK;
}

CAResult_t CAStopLEAdapter()
{
    // Nothing to do.

    return CA_STATUS_OK;
}

CAResult_t CAInitLENwkMonitorMutexVaraibles()
{
    OIC_LOG(DEBUG, TAG, "IN - CAInitLENwkMonitorMutexVaraibles");
    if (NULL == g_deviceStateCbMutex)
    {
        g_deviceStateCbMutex = oc_mutex_new();
        if (NULL == g_deviceStateCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleDeviceStateChangedCbMutex)
    {
        g_bleDeviceStateChangedCbMutex = oc_mutex_new();
        if (NULL == g_bleDeviceStateChangedCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleConnectionStateChangedCbMutex)
    {
        g_bleConnectionStateChangedCbMutex = oc_mutex_new();
        if (NULL == g_bleConnectionStateChangedCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            oc_mutex_free(g_bleConnectionStateChangedCbMutex);
            return CA_STATUS_FAILED;
        }
    }

    OIC_LOG(DEBUG, TAG, "OUT - CAInitLENwkMonitorMutexVaraibles");
    return CA_STATUS_OK;
}

void CATerminateLENwkMonitorMutexVaraibles()
{
    OIC_LOG(DEBUG, TAG, "IN - CATerminateLENwkMonitorMutexVaraibles");

    oc_mutex_free(g_deviceStateCbMutex);
    g_deviceStateCbMutex = NULL;

    oc_mutex_free(g_bleDeviceStateChangedCbMutex);
    g_bleDeviceStateChangedCbMutex = NULL;

    oc_mutex_free(g_bleConnectionStateChangedCbMutex);
    g_bleConnectionStateChangedCbMutex = NULL;

    OIC_LOG(DEBUG, TAG, "OUT - CATerminateLENwkMonitorMutexVaraibles");
}

CAResult_t CAGetLEAdapterState()
{
    OIC_LOG(DEBUG, TAG, "IN - CAGetLEAdapterState");

    if (!CALEClientIsEnableBTAdapter() || !CALEServerIsEnableBTAdapter())
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
    }

    OIC_LOG(DEBUG, TAG, "OUT - CAGetLEAdapterState");
    return CA_STATUS_OK;
}

CAResult_t CAInitializeLENetworkMonitor()
{
    OIC_LOG(DEBUG, TAG, "IN - CAInitializeLENetworkMonitor");

    if (NULL == g_deviceStateCbCond){
        g_deviceStateCbCond = oc_cond_new();
    }

    CAResult_t res = CAInitLENwkMonitorMutexVaraibles();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAInitLENwkMonitorMutexVaraibles has failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT- CAInitializeLENetworkMonitor");

    return CA_STATUS_OK;

}

void CATerminateLENetworkMonitor()
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_cond_free(g_deviceStateCbCond);
    g_deviceStateCbCond = NULL;

    CATerminateLENwkMonitorMutexVaraibles();

    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CASetLEAdapterStateChangedCb(CALEDeviceStateChangedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");

    OIC_LOG(DEBUG, TAG, "Setting CALEDeviceStateChangedCallback");

    oc_mutex_lock(g_bleDeviceStateChangedCbMutex);
    CALESetAdapterStateCallback(callback);
    oc_mutex_unlock(g_bleDeviceStateChangedCbMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAUnSetLEAdapterStateChangedCb()
{
    OIC_LOG(DEBUG, TAG, "it is not required in this platform");
    return CA_STATUS_OK;
}

CAResult_t CASetLENWConnectionStateChangedCb(CALEConnectionStateChangedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");
    oc_mutex_lock(g_bleConnectionStateChangedCbMutex);
    g_bleConnectionStateChangedCallback = callback;
    oc_mutex_unlock(g_bleConnectionStateChangedCbMutex);
    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAUnsetLENWConnectionStateChangedCb()
{
    OIC_LOG(DEBUG, TAG, "IN");
    oc_mutex_lock(g_bleConnectionStateChangedCbMutex);
    g_bleConnectionStateChangedCallback = NULL;
    oc_mutex_unlock(g_bleConnectionStateChangedCbMutex);
    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CALEClientNWStateChangeCallback(CALE_STATE_t state){
    OIC_LOG_V(DEBUG, TAG, "[Network State Changed]: state(%s)",
            state == 1 ? "STATE_ON": (state == 0 ? "STATE_OFF" : "TURN_OFF"));

    if (!g_bleDeviceStateChangedCallback)
    {
        OIC_LOG(ERROR, TAG, "gNetworkChangeCb is null --> wait~");
        if (NULL == g_deviceStateCbMutex){
            g_deviceStateCbMutex = oc_mutex_new();
        }
        if (NULL == g_deviceStateCbCond){
            g_deviceStateCbCond = oc_cond_new();
        }
        oc_mutex_lock(g_deviceStateCbMutex);
        oc_cond_wait(g_deviceStateCbCond, g_deviceStateCbMutex);
        oc_mutex_unlock(g_deviceStateCbMutex);
    }

    CAAdapterState_t newStatus = CA_ADAPTER_ENABLED;
    CAResult_t res = CA_STATUS_OK;
    switch(state){
        case CALE_STATE_ON:
            newStatus = CA_ADAPTER_ENABLED;
            CALEClientCreateDeviceList();
            CALEServerCreateCachedDeviceList();

            g_bleDeviceStateChangedCallback(newStatus);
            break;
        case CALE_STATE_TURNING_OFF:
            CAStopLEGattClient();
            break;
        default:
        case CALE_STATE_OFF:
            res = CALEClientRemoveAllGattObjs();
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientRemoveAllGattObjs has failed");
            }

            res = CALEClientResetDeviceStateForAll();
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEClientResetDeviceStateForAll has failed");
            }
            res = CALEServerRemoveAllDevices();
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CALEServerRemoveAllDevices has failed");
            }
            CALEClientSetScanFlag(false);

            newStatus = CA_ADAPTER_DISABLED;
            g_bleDeviceStateChangedCallback(newStatus);
            break;
    }
}

void CALEClientDisconnectCallabck(const char *address){
    OIC_LOG(DEBUG, TAG, "[CALEClientDisconnectCallabck] Central: disconnect");
    if (g_bleConnectionStateChangedCallback)
    {
        g_bleConnectionStateChangedCallback(CA_ADAPTER_GATT_BTLE, address, false);
    }
}

void CALEServerDisconnectCallback(const char *address){
    OIC_LOG(DEBUG, TAG, "[CALEServerDisconnectCallback] Peripheral: disconnect");
    if (g_bleConnectionStateChangedCallback)
    {
        g_bleConnectionStateChangedCallback(CA_ADAPTER_GATT_BTLE, address, false);
    }
}

