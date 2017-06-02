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

#include "caleclient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <gio/gio.h>

#include "octhread.h"
#include "uarraylist.h"
#include "caqueueingthread.h"
#include "caadapterutils.h"
#include "cagattservice.h"
#include "oic_string.h"
#include "oic_malloc.h"

/**
 * Logging tag for module name.
 */
#define TAG "OIC_CA_LE_CLIENT"

#define MICROSECS_PER_SEC 1000000
#define WAIT_TIME_WRITE_CHARACTERISTIC 10 * MICROSECS_PER_SEC

uint64_t const TIMEOUT = 30 * MICROSECS_PER_SEC;

/**
 * Flag to check if multicast is already in progress.
 */
static bool g_isMulticastInProgress = false;

/**
 * Pending multicast data list to be sent.
 */
static u_arraylist_t *g_multicastDataList = NULL;

/**
 * Mutex to synchronize the access to Pending multicast data list.
 */
static oc_mutex g_multicastDataListMutex = NULL;

/**
 * List of devices discovered.
 */
static u_arraylist_t *g_deviceDiscoveredList = NULL;

/**
 * Mutex to synchronize the access to discovered devices list.
 */
static oc_mutex g_deviceDiscoveredListMutex = NULL;

/**
 * Condition to start the timer for scanning.
 */
static oc_cond g_startTimerCond = NULL;

/**
 * Condition for scanning Time interval.
 */
static oc_cond g_scanningTimeCond = NULL;

/**
 * This contains the list of OIC services a client connect tot.
 */
static LEServerInfoList *g_LEServerList = NULL;

/**
 * Mutex to synchronize access to BleServiceList.
 */
static oc_mutex g_LEServerListMutex = NULL;

/**
 * Boolean variable to keep the state of the GATT Client.
 */
static bool g_isLEGattClientStarted = false;

/**
 * Mutex to synchronize access to the requestResponse callback to be called
 * when the data needs to be sent from GATTClient.
 */
static oc_mutex g_LEReqRespClientCbMutex = NULL;

/**
 * Mutex to synchronize access to the requestResponse callback to be called
 * when the data needs to be sent from GATTClient.
 */
static oc_mutex g_LEClientConnectMutex = NULL;

/**
 * Mutex to synchronize the calls to be done to the platform from GATTClient
 * interfaces from different threads.
 */
static oc_mutex g_LEClientStateMutex = NULL;

/**
 * Mutex to synchronize the task to be pushed to thread pool.
 */
static oc_mutex g_LEClientThreadPoolMutex = NULL;

/**
 * Mutex to synchronize the task to write characteristic one packet after another.
 */
static oc_mutex g_threadWriteCharacteristicMutex = NULL;

/**
 * Condition for Writing characteristic.
 */
static oc_cond g_threadWriteCharacteristicCond = NULL;

/**
 * Flag to check status of write characteristic.
 */
static bool g_isSignalSetFlag = false;

/**
 * Maintains the callback to be notified on receival of network packets from other
 *           BLE devices
 */
static CABLEDataReceivedCallback g_LEClientDataReceivedCallback = NULL;

/**
 * callback to update the error to le adapter
 */
static CABLEErrorHandleCallback g_clientErrorCallback;

/**
 * gmainLoop to manage the threads to receive the callback from the platfrom.
 */
static GMainLoop *g_eventLoop = NULL;

/**
 * Reference to threadpool
 */
static ca_thread_pool_t g_LEClientThreadPool = NULL;

bool CALEIsHaveService(bt_adapter_le_device_scan_result_info_s* scanInfo, char* service_uuid)
{
    bool ret = false;
    char **uuids = NULL;
    int count = 0;
    int result = 0;

    // For arduino servers, scan response will give the UUIDs advertised.
    result = bt_adapter_le_get_scan_result_service_uuids(scanInfo,
                                                         BT_ADAPTER_LE_PACKET_SCAN_RESPONSE,
                                                         &uuids, &count);
    if (result == BT_ERROR_NONE && NULL != uuids)
    {
        int i;
        for (i = 0; i < count; i++)
        {
            if (0 == strcasecmp(uuids[i], service_uuid))
            {
                OIC_LOG_V(DEBUG, TAG, "Service[%s] Found in %s",
                          uuids[i], scanInfo->remote_address);
                ret = true;
            }
            OICFree(uuids[i]);
        }
        OICFree(uuids);
    }

    // For android/tizen servers, advertising packet will give the UUIDs.
    result = bt_adapter_le_get_scan_result_service_uuids(scanInfo,
                                                         BT_ADAPTER_LE_PACKET_ADVERTISING,
                                                         &uuids, &count);
    if (result == BT_ERROR_NONE && NULL != uuids)
    {
        int i;
        for (i = 0; i < count; i++)
        {
            if (0 == strcasecmp(uuids[i], service_uuid))
            {
                OIC_LOG_V(DEBUG, TAG, "Service[%s] Found in %s",
                          uuids[i], scanInfo->remote_address);
                ret = true;
            }
            OICFree(uuids[i]);
        }
        OICFree(uuids);
    }

    return ret;
}

bool CALEIsDeviceDiscovered(const char * address)
{
    if (g_deviceDiscoveredList)
    {
        oc_mutex_lock(g_deviceDiscoveredListMutex);
        uint32_t arrayLength = u_arraylist_length(g_deviceDiscoveredList);
        for (int i = 0; i < arrayLength; i++)
        {
            char *deviceAddr = u_arraylist_get(g_deviceDiscoveredList, i);
            if (0 == strcasecmp(deviceAddr, address))
            {
                oc_mutex_unlock(g_deviceDiscoveredListMutex);
                return true;
            }

        }
        oc_mutex_unlock(g_deviceDiscoveredListMutex);
    }
    return false;
}

void CALEGattCharacteristicChangedCb(bt_gatt_h characteristic,
                                     char *value,
                                     int valueLen, void *userData)
{
    OIC_LOG(DEBUG, TAG, "IN");
    OIC_LOG_V(DEBUG, TAG, "Changed characteristic value length [%d]", valueLen);

    oc_mutex_lock(g_LEReqRespClientCbMutex);
    if (NULL == g_LEClientDataReceivedCallback)
    {
        OIC_LOG(ERROR, TAG, "Request response callback is not set");
        oc_mutex_unlock(g_LEReqRespClientCbMutex);
        return;
    }

    uint32_t sentLength = 0;
    g_LEClientDataReceivedCallback(userData, (uint8_t *)value, valueLen, &sentLength);

    OIC_LOG_V(DEBUG, TAG, "Recv data Length is %d", sentLength);

    oc_mutex_unlock(g_LEReqRespClientCbMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CALEGattCharacteristicWriteCb(int result, bt_gatt_h reqHandle, void *userData)
{
    OIC_LOG(DEBUG, TAG, "IN ");

    if (BT_ERROR_NONE != result)
    {
        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, "", 0, -1,
                           false, "writeChar failure");

        OIC_LOG(ERROR, TAG, "Write failed Need Retry ");
        //Need to Implement retry mechanism
    }
    else
    {
        oc_mutex_lock(g_threadWriteCharacteristicMutex);
        OIC_LOG(DEBUG, TAG, "g_isSignalSetFlag is set true and signal");
        g_isSignalSetFlag = true;
        oc_cond_signal(g_threadWriteCharacteristicCond);
        oc_mutex_unlock(g_threadWriteCharacteristicMutex);

        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, "", 0, -1,
                           true, "writeChar success");
    }

    OIC_LOG(DEBUG, TAG, "OUT ");
}

void CALEGattConnectionStateChanged(bool connected, const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN ");

    VERIFY_NON_NULL_VOID(remoteAddress, TAG, "remote address is NULL");

    // Start the scanning.
    CAResult_t ret = CALEGattStartDeviceScanning();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEGattStartDeviceDiscovery Failed");
    }
    // Signal the start timer.
    oc_cond_signal(g_scanningTimeCond);

    if (!connected)
    {
        OIC_LOG_V(DEBUG, TAG, "DisConnected from [%s] ", remoteAddress);
        oc_mutex_lock(g_LEServerListMutex);
        CARemoveLEServerInfoFromList(&g_LEServerList, remoteAddress);
        oc_mutex_unlock(g_LEServerListMutex);
    }
    else
    {
        OIC_LOG_V(DEBUG, TAG, "Connected to [%s] ", remoteAddress);

        char *addr = OICStrdup(remoteAddress);
        if (NULL == addr)
        {
            OIC_LOG(ERROR, TAG, "addr is NULL");
            return;
        }

        oc_mutex_lock(g_LEClientThreadPoolMutex);
        if (NULL == g_LEClientThreadPool)
        {
            OIC_LOG(ERROR, TAG, "g_LEClientThreadPool is NULL");
            OICFree(addr);
            oc_mutex_unlock(g_LEClientThreadPoolMutex);
            return;
        }

        ret = ca_thread_pool_add_task(g_LEClientThreadPool, CADiscoverLEServicesThread,
                                      addr, NULL);
        if (CA_STATUS_OK != ret)
        {
            OIC_LOG_V(ERROR, TAG, "ca_thread_pool_add_task failed with ret [%d]", ret);
            OICFree(addr);
        }
        oc_mutex_unlock(g_LEClientThreadPoolMutex);
    }
    OIC_LOG(DEBUG, TAG, "OUT");
}

void CALEAdapterScanResultCb(int result, bt_adapter_le_device_scan_result_info_s *scanInfo,
                             void *userData)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_VOID(scanInfo, TAG, "scanInfo");
    VERIFY_NON_NULL_VOID(scanInfo->remote_address, TAG, "scanInfo->remote_address");

    OIC_LOG_V(DEBUG, TAG, "Remote Address [%s]", scanInfo->remote_address);
    OIC_LOG_V(DEBUG, TAG, "Scan Result [%d]", result);
    OIC_LOG_V(DEBUG, TAG,
              " Adv data len [%d] Scan data len[%d]RSSI [%d] Addr_type [%d] ",
              scanInfo->adv_data_len, scanInfo->scan_data_len, scanInfo->rssi,
              scanInfo->address_type);

    // Check if device is already discovered.
    if (CALEIsDeviceDiscovered(scanInfo->remote_address))
    {
        OIC_LOG_V(INFO, TAG, "Device[%s] is already discovered", scanInfo->remote_address);
        return;
    }

    if (!CALEIsHaveService(scanInfo, CA_GATT_SERVICE_UUID))
    {
        oc_mutex_lock(g_deviceDiscoveredListMutex);
        // Add the the device Discovered list.
        if (NULL == g_deviceDiscoveredList)
        {
            g_deviceDiscoveredList = u_arraylist_create();
        }
        char *deviceAddr = OICStrdup(scanInfo->remote_address);
        if (NULL == deviceAddr)
        {
            OIC_LOG_V(ERROR, TAG, "Device address is NULL");
            oc_mutex_unlock(g_deviceDiscoveredListMutex);
            return;
        }

        u_arraylist_add(g_deviceDiscoveredList, (void *) deviceAddr);
        oc_mutex_unlock(g_deviceDiscoveredListMutex);
        OIC_LOG_V(INFO, TAG, "Device[%s] is don't have service", scanInfo->remote_address);
        return;
    }

    // Stop the scan before invoking bt_gatt_connect().
    CALEGattStopDeviceScanning();

    size_t len = strlen(scanInfo->remote_address);

    char *addr = (char *)OICMalloc(sizeof(char) * (len + 1));
    VERIFY_NON_NULL_VOID(addr, TAG, "Malloc failed");

    strncpy(addr, scanInfo->remote_address, len + 1);

    OIC_LOG_V(DEBUG, TAG,
              "Trying to do Gatt connection to [%s]", addr);

    oc_mutex_lock(g_LEClientThreadPoolMutex);
    if (NULL == g_LEClientThreadPool)
    {
        OIC_LOG(ERROR, TAG, "g_LEClientThreadPool is NULL");
        OICFree(addr);
        oc_mutex_unlock(g_LEClientThreadPoolMutex);
        return;
    }

    CAResult_t res = ca_thread_pool_add_task(g_LEClientThreadPool, CAGattConnectThread, addr, NULL);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG,
                  "ca_thread_pool_add_task failed with ret [%d]", res);
        OICFree(addr);
    }
    oc_mutex_unlock(g_LEClientThreadPoolMutex);
    OIC_LOG(DEBUG, TAG, "OUT");
}

void CASetLEClientThreadPoolHandle(ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_LEClientThreadPoolMutex);
    g_LEClientThreadPool = handle;
    oc_mutex_unlock(g_LEClientThreadPoolMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CASetLEReqRespClientCallback(CABLEDataReceivedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_LEReqRespClientCbMutex);

    g_LEClientDataReceivedCallback = callback;

    oc_mutex_unlock(g_LEReqRespClientCbMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CASetBLEClientErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    g_clientErrorCallback = callback;
}

CAResult_t CAStartLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_LEClientStateMutex);
    if (true  == g_isLEGattClientStarted)
    {
        OIC_LOG(ERROR, TAG, "Gatt Client is already running!!");
        oc_mutex_unlock(g_LEClientStateMutex);
        return CA_STATUS_FAILED;
    }

    CAResult_t  result = CALEGattSetCallbacks();
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CABleGattSetCallbacks Failed");
        oc_mutex_unlock(g_LEClientStateMutex);
        CATerminateLEGattClient();
        return CA_STATUS_FAILED;
    }

    g_isLEGattClientStarted = true;
    oc_mutex_unlock(g_LEClientStateMutex);

    oc_mutex_lock(g_LEClientThreadPoolMutex);
    if (NULL == g_LEClientThreadPool)
    {
        OIC_LOG(ERROR, TAG, "gBleServerThreadPool is NULL");
        CATerminateGattClientMutexVariables();
        oc_mutex_unlock(g_LEClientThreadPoolMutex);
        return CA_STATUS_FAILED;
    }

    result = ca_thread_pool_add_task(g_LEClientThreadPool, CAStartTimerThread,
                                     NULL, NULL);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "ca_thread_pool_add_task failed");
        CATerminateGattClientMutexVariables();
        oc_mutex_unlock(g_LEClientThreadPoolMutex);
        return CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_LEClientThreadPoolMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CAStartTimerThread(void *data)
{
    OIC_LOG(DEBUG, TAG, "IN");
    while (g_isLEGattClientStarted)
    {
        oc_mutex_lock(g_multicastDataListMutex);
        if (!g_isMulticastInProgress)
        {
            OIC_LOG(DEBUG, TAG, "waiting....");
            oc_cond_wait(g_startTimerCond, g_multicastDataListMutex);
            OIC_LOG(DEBUG, TAG, "Wake up");
            g_isMulticastInProgress = true;

            if (!g_isLEGattClientStarted)
            {
            	break;
            }
        }

        // Timed conditional wait for stopping the scan.
        OCWaitResult_t ret = oc_cond_wait_for(g_scanningTimeCond, g_multicastDataListMutex,
                                              TIMEOUT);
        if (OC_WAIT_TIMEDOUT == ret)
        {
            OIC_LOG(DEBUG, TAG, "Scan is timed Out");
            // Call stop scan.
            CALEGattStopDeviceScanning();

            // Clear the data list and device list.
            u_arraylist_destroy(g_multicastDataList);
            g_multicastDataList = NULL;

            oc_mutex_lock(g_deviceDiscoveredListMutex);
            u_arraylist_destroy(g_deviceDiscoveredList);
            g_deviceDiscoveredList = NULL;
            oc_mutex_unlock(g_deviceDiscoveredListMutex);

            g_isMulticastInProgress = false;
        }
        oc_mutex_unlock(g_multicastDataListMutex);
    }

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CAStopLEGattClient()
{
    OIC_LOG(DEBUG,  TAG, "IN");

    oc_mutex_lock(g_LEClientStateMutex);

    if (false == g_isLEGattClientStarted)
    {
        OIC_LOG(ERROR, TAG, "Gatt Client is not running to stop");
        oc_mutex_unlock(g_LEClientStateMutex);
        return;
    }

    CALEGattUnSetCallbacks();

    CALEGattStopDeviceScanning();

    // this flag should be set before signal g_startTimerCond.
    // since scan thread can be stopped through this flag.
    g_isLEGattClientStarted = false;

    // Signal the conditions waiting in Start timer.
    oc_cond_signal(g_startTimerCond);
    oc_cond_signal(g_scanningTimeCond);

    // Destroy the multicast data list and device list if not empty.
    if (NULL != g_multicastDataList)
    {
        oc_mutex_lock(g_multicastDataListMutex);
        u_arraylist_destroy(g_multicastDataList);
        g_multicastDataList = NULL;
        oc_mutex_unlock(g_multicastDataListMutex);
    }

    if (NULL != g_deviceDiscoveredList)
    {
        oc_mutex_lock(g_deviceDiscoveredListMutex);
        u_arraylist_destroy(g_deviceDiscoveredList);
        g_deviceDiscoveredList = NULL;
        oc_mutex_unlock(g_deviceDiscoveredListMutex);
    }

    oc_mutex_lock(g_LEServerListMutex);
    CAFreeLEServerList(g_LEServerList);
    g_LEServerList = NULL;
    oc_mutex_unlock(g_LEServerListMutex);

    oc_mutex_lock(g_threadWriteCharacteristicMutex);
    oc_cond_signal(g_threadWriteCharacteristicCond);
    oc_mutex_unlock(g_threadWriteCharacteristicMutex);

    CAResetRegisteredServiceCount();

    GMainContext  *context_event_loop = NULL;
    // Required for waking up the thread which is running in gmain loop
    if (NULL != g_eventLoop)
    {
        context_event_loop = g_main_loop_get_context(g_eventLoop);
    }
    if (context_event_loop)
    {
        OIC_LOG_V(DEBUG,  TAG, "g_eventLoop context %x", context_event_loop);
        g_main_context_wakeup(context_event_loop);

        // Kill g main loops and kill threads.
        g_main_loop_quit(g_eventLoop);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "g_eventLoop context is NULL");
    }

    oc_mutex_unlock(g_LEClientStateMutex);

    OIC_LOG(DEBUG,  TAG, "OUT");
}

CAResult_t CAInitializeLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "Initialize GATT Client");
    CAResult_t res = CAInitGattClientMutexVariables();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAInitGattClientMutexVariables failed!");
        CATerminateGattClientMutexVariables();
        return CA_STATUS_FAILED;
    }
    return res;
}

void CATerminateLEGattClient()
{
    OIC_LOG(DEBUG,  TAG, "IN");

    CATerminateGattClientMutexVariables();

    OIC_LOG(DEBUG,  TAG, "OUT");
}

CAResult_t CAInitGattClientMutexVariables()
{
    OIC_LOG(DEBUG,  TAG, "IN");
    if (NULL == g_LEClientStateMutex)
    {
        g_LEClientStateMutex = oc_mutex_new();
        if (NULL == g_LEClientStateMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_LEServerListMutex)
    {
        g_LEServerListMutex = oc_mutex_new();
        if (NULL == g_LEServerListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_LEReqRespClientCbMutex)
    {
        g_LEReqRespClientCbMutex = oc_mutex_new();
        if (NULL == g_LEReqRespClientCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_LEClientThreadPoolMutex)
    {
        g_LEClientThreadPoolMutex = oc_mutex_new();
        if (NULL == g_LEClientThreadPoolMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_LEClientConnectMutex)
    {
        g_LEClientConnectMutex = oc_mutex_new();
        if (NULL == g_LEClientConnectMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_multicastDataListMutex)
    {
        g_multicastDataListMutex = oc_mutex_new();
        if (NULL == g_multicastDataListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_deviceDiscoveredListMutex)
    {
        g_deviceDiscoveredListMutex = oc_mutex_new();
        if (NULL == g_deviceDiscoveredListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadWriteCharacteristicMutex)
    {
        g_threadWriteCharacteristicMutex = oc_mutex_new();
        if (NULL == g_threadWriteCharacteristicMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_startTimerCond)
    {
        g_startTimerCond = oc_cond_new();
        if (NULL == g_startTimerCond)
        {
            OIC_LOG(ERROR, TAG, "oc_cond_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_scanningTimeCond)
    {
        g_scanningTimeCond = oc_cond_new();
        if (NULL == g_scanningTimeCond)
        {
            OIC_LOG(ERROR, TAG, "oc_cond_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadWriteCharacteristicCond)
    {
        g_threadWriteCharacteristicCond = oc_cond_new();
        if (NULL == g_threadWriteCharacteristicCond)
        {
            OIC_LOG(ERROR, TAG, "oc_cond_new failed");
            return CA_STATUS_FAILED;
        }
    }

    OIC_LOG(DEBUG,  TAG, "OUT");
    return CA_STATUS_OK;
}

void CATerminateGattClientMutexVariables()
{
    OIC_LOG(DEBUG,  TAG, "IN");

    oc_mutex_free(g_LEClientStateMutex);
    g_LEClientStateMutex = NULL;

    oc_mutex_free(g_LEServerListMutex);
    g_LEServerListMutex = NULL;

    oc_mutex_free(g_LEReqRespClientCbMutex);
    g_LEReqRespClientCbMutex = NULL;

    oc_mutex_free(g_LEClientConnectMutex);
    g_LEClientConnectMutex = NULL;

    oc_mutex_free(g_LEClientThreadPoolMutex);
    g_LEClientThreadPoolMutex = NULL;

    oc_mutex_free(g_multicastDataListMutex);
    g_multicastDataListMutex = NULL;

    oc_mutex_free(g_deviceDiscoveredListMutex);
    g_deviceDiscoveredListMutex = NULL;

    oc_mutex_free(g_threadWriteCharacteristicMutex);
    g_threadWriteCharacteristicMutex = NULL;

    oc_cond_free(g_startTimerCond);
    g_startTimerCond = NULL;

    oc_cond_free(g_scanningTimeCond);
    g_scanningTimeCond = NULL;

    oc_cond_free(g_threadWriteCharacteristicCond);
    g_threadWriteCharacteristicCond = NULL;
    g_isSignalSetFlag = false;

    OIC_LOG(DEBUG,  TAG, "OUT");
}

CAResult_t CALEGattSetCallbacks()
{
    OIC_LOG(DEBUG, TAG, "IN");

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CALEGattUnSetCallbacks()
{
    OIC_LOG(DEBUG, TAG, "IN");

    bt_gatt_unset_connection_state_changed_cb();

    int numOfServersConnected = CAGetRegisteredServiceCount();
    LEServerInfo *leServerInfo = NULL;

    for (int32_t index = 0; index < numOfServersConnected; index++)
    {
        CAGetLEServerInfoByPosition(g_LEServerList, index, &leServerInfo);
        bt_gatt_client_unset_characteristic_value_changed_cb(leServerInfo->readChar);
    }
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CALEGattStartDeviceScanning()
{
    OIC_LOG(DEBUG, TAG, "IN");

    int ret = bt_adapter_le_start_scan(CALEAdapterScanResultCb, NULL);
    if(BT_ERROR_NONE != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_adapter_le_start_scan failed[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CALEGattStopDeviceScanning()
{
    OIC_LOG(DEBUG, TAG, "IN");

    int ret = bt_adapter_le_stop_scan();
    if (BT_ERROR_NONE != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_adapter_le_stop_scan failed[%s]",
                  CALEGetErrorMsg(ret));
    }

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CAGattConnectThread (void *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN ");

    VERIFY_NON_NULL_VOID(remoteAddress, TAG, "remote address is NULL");

    char *address  = (char *)remoteAddress;

    OIC_LOG_V(DEBUG, TAG, "remote address is [%s]", address);

    CAResult_t result = CALEGattConnect(address);

    if (CA_STATUS_OK != result)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_connect failed for [%s]", address);
    }

    OICFree(address);

    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CALEGattConnect(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_RET(remoteAddress, TAG,
                        "remote address is NULL", CA_STATUS_FAILED);

    oc_mutex_lock(g_LEClientConnectMutex);
    bool isConnected = false;
    int ret = bt_device_is_profile_connected(remoteAddress, BT_PROFILE_GATT, &isConnected);
    if (BT_ERROR_NONE != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_device_is_profile_connected Failed with ret value [%s] ",
                  CALEGetErrorMsg(ret));
        oc_mutex_unlock(g_LEClientConnectMutex);
        return CA_STATUS_FAILED;
    }

    CAResult_t result = CA_STATUS_OK;
    if (!isConnected)
    {
        ret = bt_gatt_connect(remoteAddress, true);

        if (BT_ERROR_NONE != ret)
        {
            OIC_LOG_V(ERROR, TAG, "bt_gatt_connect Failed with ret value [%s] ",
                      CALEGetErrorMsg(ret));
            oc_mutex_unlock(g_LEClientConnectMutex);
            return CA_STATUS_FAILED;
        }
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "Remote address[%s] is already connected",
                  remoteAddress);
        char *addr = OICStrdup(remoteAddress);
        if (NULL == addr)
        {
            OIC_LOG(ERROR, TAG, "addr is NULL");
            oc_mutex_unlock(g_LEClientConnectMutex);
            return CA_STATUS_FAILED;
        }

        oc_mutex_lock(g_LEClientThreadPoolMutex);
        if (NULL == g_LEClientThreadPool)
        {
            OIC_LOG(ERROR, TAG, "g_LEClientThreadPool is NULL");
            OICFree(addr);
            oc_mutex_unlock(g_LEClientThreadPoolMutex);
            oc_mutex_unlock(g_LEClientConnectMutex);
            return CA_STATUS_FAILED;
        }

        result = ca_thread_pool_add_task(g_LEClientThreadPool, CADiscoverLEServicesThread,
                                         addr, NULL);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG_V(ERROR, TAG, "ca_thread_pool_add_task failed with ret [%d]", result);
            OICFree(addr);
        }
        oc_mutex_unlock(g_LEClientThreadPoolMutex);
    }
    oc_mutex_unlock(g_LEClientConnectMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return result;
}

CAResult_t CALEGattDisConnect(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_RET(remoteAddress, TAG,
                        "remote address is NULL", CA_STATUS_FAILED);

    int ret = bt_gatt_disconnect(remoteAddress);

    if (BT_ERROR_NONE != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_disconnect Failed with ret value [%d] ",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CADiscoverLEServicesThread (void *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_VOID(remoteAddress, TAG, "remote address is NULL");

    char *address  = (char *)remoteAddress;

    CAResult_t result = CALEGattDiscoverServices(address);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CALEGattDiscoverServices failed");
    }

    OICFree(address);
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CALEGattDiscoverServices(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_RET(remoteAddress, TAG,
                        "remote address is NULL", CA_STATUS_FAILED);

    LEServerInfo *leServerInfo = NULL;
    CAResult_t result =  CA_STATUS_FAILED;

    oc_mutex_lock(g_LEServerListMutex);
    result = CAGetLEServerInfo(g_LEServerList, remoteAddress, &leServerInfo);
    oc_mutex_unlock(g_LEServerListMutex);

    if (CA_STATUS_OK == result)
    {
        OIC_LOG_V(INFO, TAG, "Device[%s] is already discovered", leServerInfo->remoteAddress);

        // Send the data of pending multicast data list if any.
        if (g_multicastDataList)
        {
            oc_mutex_lock(g_multicastDataListMutex);
            uint32_t arrayLength = u_arraylist_length(g_multicastDataList);
            for (int i = 0; i < arrayLength; i++)
            {
                CALEData_t *multicastData = u_arraylist_get(g_multicastDataList, i);
                if (NULL == multicastData)
                {
                    OIC_LOG(DEBUG, TAG, "multicastData is NULL");
                    continue;
                }
                CAUpdateCharacteristicsToGattServer(remoteAddress, multicastData->data,
                                                    multicastData->dataLen, LE_UNICAST, 0);
            }
            oc_mutex_unlock(g_multicastDataListMutex);
        }

        OIC_LOG(DEBUG, TAG, "OUT");
        return CA_STATUS_OK;
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "CAGetLEServerInfo [%s] is failed, %d", remoteAddress, result);
    }

    bt_gatt_client_h clientHandle = NULL;
    int32_t ret = bt_gatt_client_create(remoteAddress, &clientHandle);
    if (BT_ERROR_NONE != ret || NULL == clientHandle)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_client_create Failed with ret value [%s] ", CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    bt_gatt_h serviceHandle = NULL;
    ret = bt_gatt_client_get_service(clientHandle, CA_GATT_SERVICE_UUID, &serviceHandle);
    if (BT_ERROR_NONE != ret || NULL == serviceHandle)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_client_get_service Failed with ret value [%s] ", CALEGetErrorMsg(ret));
        bt_gatt_client_destroy(clientHandle);
        return CA_STATUS_FAILED;
    }

    // Server will read data on this characteristic.
    bt_gatt_h writeChrHandle = NULL;
    ret = bt_gatt_service_get_characteristic(serviceHandle, CA_GATT_REQUEST_CHRC_UUID,
                                             &writeChrHandle);
    if (BT_ERROR_NONE != ret || NULL == writeChrHandle)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_service_get_characteristic Failed with ret value [%s] ",
                  CALEGetErrorMsg(ret));
        bt_gatt_client_destroy(clientHandle);
        CALEGattDisConnect(remoteAddress);
        return CA_STATUS_FAILED;
    }

    // Server will notify data on this characteristic.
    bt_gatt_h readChrHandle = NULL;
    ret = bt_gatt_service_get_characteristic(serviceHandle, CA_GATT_RESPONSE_CHRC_UUID,
                                             &readChrHandle);
    if (BT_ERROR_NONE != ret || NULL == readChrHandle)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_service_get_characteristic Failed with ret value [%s] ",
                  CALEGetErrorMsg(ret));
        bt_gatt_client_destroy(clientHandle);
        CALEGattDisConnect(remoteAddress);
        return CA_STATUS_FAILED;
    }


    //TODO: This data has to be freed while unsetting the callback.
    char *addr = OICStrdup(remoteAddress);
    if (NULL == addr)
    {
        OIC_LOG(ERROR, TAG, "addr is NULL");
        bt_gatt_client_destroy(clientHandle);
        CALEGattDisConnect(remoteAddress);
        return CA_STATUS_FAILED;
    }

    ret = bt_gatt_client_set_characteristic_value_changed_cb(readChrHandle,
                                                             CALEGattCharacteristicChangedCb,
                                                             (void *)addr);
    if (BT_ERROR_NONE != ret)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_client_set_characteristic_value_changed_cb Failed with ret value [%s]",
                  CALEGetErrorMsg(ret));
        bt_gatt_client_destroy(clientHandle);
        CALEGattDisConnect(remoteAddress);
        return CA_STATUS_FAILED;
    }

    LEServerInfo *serverInfo = (LEServerInfo *)OICCalloc(1, sizeof(LEServerInfo));
    if (NULL == serverInfo)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed");
        CALEGattDisConnect(remoteAddress);
        return CA_MEMORY_ALLOC_FAILED;
    }
    serverInfo->clientHandle = clientHandle;
    serverInfo->serviceHandle = serviceHandle;
    serverInfo->readChar = readChrHandle;
    serverInfo->writeChar = writeChrHandle;
    serverInfo->remoteAddress = OICStrdup(remoteAddress);

    oc_mutex_lock(g_LEServerListMutex);
    result = CAAddLEServerInfoToList(&g_LEServerList, serverInfo);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CAAddLEServerInfoToList failed");
        bt_gatt_client_destroy(clientHandle);
        CALEGattDisConnect(remoteAddress);
        return CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_LEServerListMutex);

    oc_mutex_lock(g_deviceDiscoveredListMutex);
    // Add the the device Discovered list.
    if (NULL == g_deviceDiscoveredList)
    {
        g_deviceDiscoveredList = u_arraylist_create();
    }
    char *deviceAddr = OICStrdup(remoteAddress);
    if (NULL == deviceAddr)
    {
        OIC_LOG_V(ERROR, TAG, "Device address is NULL");
        oc_mutex_unlock(g_deviceDiscoveredListMutex);
        return CA_STATUS_FAILED;
    }
    u_arraylist_add(g_deviceDiscoveredList, (void *) deviceAddr);
    oc_mutex_unlock(g_deviceDiscoveredListMutex);

    // Send the data of pending multicast data list if any.
    if (g_multicastDataList)
    {
        oc_mutex_lock(g_multicastDataListMutex);
        uint32_t arrayLength = u_arraylist_length(g_multicastDataList);
        for (int i = 0; i < arrayLength; i++)
        {
            CALEData_t *multicastData = u_arraylist_get(g_multicastDataList, i);
            if (NULL == multicastData)
            {
                OIC_LOG(DEBUG, TAG, "multicastData is NULL");
                continue;
            }
            CAUpdateCharacteristicsToGattServer(remoteAddress, multicastData->data,
                                                multicastData->dataLen, LE_UNICAST, 0);
        }
        oc_mutex_unlock(g_multicastDataListMutex);
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t  CAUpdateCharacteristicsToGattServer(const char *remoteAddress,
                                                const uint8_t *data, const uint32_t dataLen,
                                                CALETransferType_t type, const int32_t position)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(data, TAG, "data is NULL");

    if (0 >= dataLen)
    {
        OIC_LOG(ERROR, TAG, "dataLen is less than or equal zero. Invalid input!");
        return CA_STATUS_INVALID_PARAM;
    }

    LEServerInfo *leServerInfo = NULL;
    CAResult_t ret =  CA_STATUS_FAILED;

    oc_mutex_lock(g_LEServerListMutex);
    if (LE_UNICAST == type)
    {
        ret = CAGetLEServerInfo(g_LEServerList, remoteAddress, &leServerInfo);
    }
    else if (LE_MULTICAST == type)
    {
        ret = CAGetLEServerInfoByPosition(g_LEServerList, position, &leServerInfo);
    }
    oc_mutex_unlock(g_LEServerListMutex);

    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAGetBLEServiceInfoByPosition is failed");
        return CA_STATUS_FAILED;
    }

    VERIFY_NON_NULL(leServerInfo, TAG, "bleServiceInfo is NULL");

    OIC_LOG_V(DEBUG, TAG, "Updating the data of length [%d] to [%s] ", dataLen,
              leServerInfo->remoteAddress);

    int result = bt_gatt_set_value(leServerInfo->writeChar, (char *)data, dataLen);

    if (BT_ERROR_NONE != result)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_set_value Failed with return val [%s]",
                  CALEGetErrorMsg(result));
        return CA_STATUS_FAILED;
    }

    result = bt_gatt_client_write_value(leServerInfo->writeChar, CALEGattCharacteristicWriteCb,
                                        NULL);
    if (BT_ERROR_NONE != result)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_client_write_value Failed with return val [%s]",
                  CALEGetErrorMsg(result));
        return CA_STATUS_FAILED;
    }

    // wait for callback for write Characteristic with success to sent data
    OIC_LOG_V(DEBUG, TAG, "callback flag is %d", g_isSignalSetFlag);
    oc_mutex_lock(g_threadWriteCharacteristicMutex);
    if (!g_isSignalSetFlag)
    {
        OIC_LOG(DEBUG, TAG, "wait for callback to notify writeCharacteristic is success");
        if (OC_WAIT_SUCCESS != oc_cond_wait_for(g_threadWriteCharacteristicCond,
                                  g_threadWriteCharacteristicMutex,
                                  WAIT_TIME_WRITE_CHARACTERISTIC))
        {
            OIC_LOG(ERROR, TAG, "there is no response. write has failed");
            g_isSignalSetFlag = false;
            oc_mutex_unlock(g_threadWriteCharacteristicMutex);
            return CA_SEND_FAILED;
        }
    }
    // reset flag set by writeCharacteristic Callback
    g_isSignalSetFlag = false;
    oc_mutex_unlock(g_threadWriteCharacteristicMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAUpdateCharacteristicsToAllGattServers(const uint8_t *data, uint32_t dataLen)
{
    OIC_LOG(DEBUG,  TAG, "IN");

    VERIFY_NON_NULL(data, TAG, "data is NULL");

    if (0 >= dataLen)
    {
        OIC_LOG(ERROR, TAG, "dataLen is less than or equal zero. Invalid input !");
        return CA_STATUS_INVALID_PARAM;
    }

    int numOfServersConnected = CAGetRegisteredServiceCount();

    // Send data to already connected devices.
    for (int32_t pos = 0; pos < numOfServersConnected; pos++)
    {
        /*remoteAddress will be NULL.
          Since we have to send to all destinations. pos will be used for getting remote address.
         */
        int32_t ret = CAUpdateCharacteristicsToGattServer(NULL, data, dataLen, LE_MULTICAST, pos);

        if (CA_STATUS_OK != ret)
        {
            OIC_LOG_V(ERROR, TAG,
                      "CAUpdateCharacteristicsToGattServer Failed with return val [%d] ", ret);
            g_clientErrorCallback(NULL, data, dataLen, ret);
        }
    }

    // Add the data to pending list.
    CALEData_t *multicastData = (CALEData_t *)OICCalloc(1, sizeof(CALEData_t));
    if (NULL == multicastData)
    {
        OIC_LOG(ERROR, TAG, "Calloc failed");
        goto exit;
    }
    multicastData->data = OICCalloc(1, dataLen);
    if (NULL == multicastData->data)
    {
        OIC_LOG(ERROR, TAG, "Calloc failed");
        goto exit;
    }
    memcpy(multicastData->data, data, dataLen);
    multicastData->dataLen = dataLen;

    oc_mutex_lock(g_multicastDataListMutex);
    if (NULL == g_multicastDataList)
    {
        g_multicastDataList = u_arraylist_create();
    }
    u_arraylist_add(g_multicastDataList, (void *)multicastData);
    oc_mutex_unlock(g_multicastDataListMutex);

    // Start the scanning.
    CAResult_t result = CALEGattStartDeviceScanning();
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CALEGattStartDeviceDiscovery Failed");
        goto exit;
    }

    // Start the timer by signalling it.
    oc_cond_signal(g_startTimerCond);

exit:
    OIC_LOG(DEBUG, TAG, "OUT ");
    return CA_STATUS_OK;
}

bool CALEClientIsConnected(const char* address)
{
    //@Todo
    return true;
}

uint16_t CALEClientGetMtuSize(const char* address)
{
    VERIFY_NON_NULL_RET(address, TAG, "address is null", CA_DEFAULT_BLE_MTU_SIZE);
    //@Todo
    //it should be implemented after update Tizen 3.0
    return CA_DEFAULT_BLE_MTU_SIZE;
}

CAResult_t CALEClientSetMtuSize(const char* address, uint16_t mtuSize)
{
    VERIFY_NON_NULL(address, TAG, "address is null");
    //@Todo
    //it should be implemented after update Tizen 3.0
    return CA_NOT_SUPPORTED;
}

CAResult_t CALEClientSendNegotiationMessage(const char* address)
{
    OIC_LOG_V(DEBUG, TAG, "CALEClientSendNegotiationMessage(%s)", address);
    //@Todo
    //it will be implemented when tizen public 3.0 is released.
    return CA_NOT_SUPPORTED;
}
