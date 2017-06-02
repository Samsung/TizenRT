/******************************************************************
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

#include "caleserver.h"
#include "cacommon.h"
#include "cacommonutil.h"
#include "octhread.h"
#include "caqueueingthread.h"
#include "cagattservice.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "caleutil.h"

/**
 * Logging tag for module name
 */
//#define TAG "OIC_CA_LE_SERVER_MCD"
#define TAG BLE_SERVER_MCD_TAG
/**
 * Initial buffer size for Gatt Server.
 */
#define CA_LE_INITIAL_BUF_SIZE 512

/**
 * The handle of the OIC server.
 */
static bt_gatt_server_h g_gattServer = NULL;

/**
 * The handle of the OIC service.
 */
static bt_gatt_h g_gattSvcPath = NULL;

/**
 * The handle of the OIC read characteristics.
 */
static bt_gatt_h g_gattReadCharPath = NULL;

/**
 * The handle of the OIC write characteristics.
 */
static bt_gatt_h g_gattWriteCharPath = NULL;

/**
 * The handle to control Bluetooth LE advertising.
 */
static bt_advertiser_h g_hAdvertiser = NULL;

/**
 * Callback register with LE adapter.  This callback is called on reception of any
 * data from the remote device.
 */
static CABLEDataReceivedCallback g_leServerDataReceivedCallback = NULL;

/**
 * Callback to notify any error in LE adapter.
 */
static CABLEErrorHandleCallback g_serverErrorCallback = NULL;

/**
 * To keep the state of GATT server if started or not.
 */
static bool g_isLEGattServerStarted = false;

/**
 * Mutex to synchronize the calls to start and stop server.
 */
static oc_mutex g_leServerStateMutex = NULL;

/**
 * Mutex to synchronize writing operations on the characteristics.
 */
static  oc_mutex g_leCharacteristicMutex = NULL;

/**
 * Mutex to synchronize to creation of OIC service.
 */
static  oc_mutex g_leServiceMutex = NULL;

/**
 * Mutex to synchronize access to the requestResponse callback to be called
 * when the data needs to be sent from GATTClient.
 */
static  oc_mutex g_leReqRespCbMutex = NULL;

/**
 * Mutex to synchronize the task to be pushed to thread pool.
 */
static oc_mutex g_leServerThreadPoolMutex = NULL;

/**
 * Reference to threadpool.
 */
static ca_thread_pool_t g_leServerThreadPool = NULL;

/**
 * GmainLoop to manage the threads to receive the callback from the platfrom.
 */
static GMainLoop *g_eventLoop = NULL;

/**
 * This contains the list of OIC clients connected to the server.
 */
static LEClientInfoList *g_LEClientList = NULL;

/**
 * Mutex to synchronize access to LE ClientList.
 */
static oc_mutex g_LEClientListMutex = NULL;

static const int samsung_code = 117;

static bool cutom_adv_flag = false;
static char *custom_adv_data = NULL;
static int custom_adv_data_length = 0;

static bool cutom_scanrsp_flag = false;
static char *custom_scanrsp_data = NULL;
static int custom_scanrsp_data_length = 0;

CAResult_t CAsetServerAdvertisementData(const char *data, int length)
{
    int res = 0;
    CAResult_t ret = CA_STATUS_OK;

    OIC_LOG(DEBUG, TAG, "IN");

    if(!data || strlen(data) <= 0 || length <=0 )
    {
        OIC_LOG(ERROR, TAG, "Invalid param is passed");
        return CA_STATUS_INVALID_PARAM;
    }

    oc_mutex_lock(g_leServerStateMutex);

    cutom_adv_flag = true;
    custom_adv_data = data;
    custom_adv_data_length = length;

    oc_mutex_unlock(g_leServerStateMutex);

    OIC_LOG_V(DEBUG, TAG, "Custom advertise value has set as [%s]", custom_adv_data);
    OIC_LOG(DEBUG, TAG, "OUT");

    return ret;
}

CAResult_t CAsetServerSanResponseData(const char *data, int length)
{
    int res = 0;
    CAResult_t ret = CA_STATUS_OK;

    OIC_LOG(DEBUG, TAG, "IN");

    if(!data || strlen(data) <= 0 || length <=0 )
    {
        OIC_LOG(ERROR, TAG, "Invalid param is passed");
        return CA_STATUS_INVALID_PARAM;
    }

    oc_mutex_lock(g_leServerStateMutex);

    cutom_scanrsp_flag = true;
    custom_scanrsp_data = data;
    custom_scanrsp_data_length = length;

    oc_mutex_unlock(g_leServerStateMutex);

    OIC_LOG_V(DEBUG, TAG, "Custom scan response value has set as [%s]", custom_scanrsp_data);
    OIC_LOG(DEBUG, TAG, "OUT");

    return ret;
}

void CALEGattServerConnectionStateChanged(bool connected, const char *remoteAddress)
{
    VERIFY_NON_NULL_VOID(remoteAddress, TAG, "remote address");

    CAResult_t res = CA_STATUS_OK;
    if (connected)
    {
        OIC_LOG_V(DEBUG, TAG, "Connected to [%s]", remoteAddress);
        char *addr = OICStrdup(remoteAddress);
        oc_mutex_lock(g_LEClientListMutex);
        res  = CAAddLEClientInfoToList(&g_LEClientList, addr);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CAAddLEClientInfoToList failed");
            oc_mutex_unlock(g_LEClientListMutex);
            OICFree(addr);
            return;
        }
        oc_mutex_unlock(g_LEClientListMutex);

        res = CALEStopAdvertise();
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to stop advertising [%d]", res);
            return;
        }
    }
    else
    {
        OIC_LOG_V(DEBUG, TAG, "Disconnected from [%s]", remoteAddress);
        oc_mutex_lock(g_LEClientListMutex);
        CARemoveLEClientInfoFromList(&g_LEClientList, remoteAddress);
        oc_mutex_unlock(g_LEClientListMutex);

        res = CALEStartAdvertise();
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to start advertising [%d]", res);
            return;
        }
    }
}

#ifdef BLE_TIZEN_30
void CALEServerNotificationSentCB(int result, const char *remote_address, bt_gatt_server_h server,
                                  bt_gatt_h characteristic, bool completed, void *user_data)
#else
void CALEServerNotificationSentCB(int result, char *remote_address, bt_gatt_server_h server,
                                  bt_gatt_h characteristic, bool completed, void *user_data)
#endif
{
    OIC_LOG_V(DEBUG, TAG, "Notification to the device[%s] result[%d]", remote_address, result);
}

CAResult_t CAStartLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_leServerStateMutex);
    if (true == g_isLEGattServerStarted)
    {
        OIC_LOG(ERROR, TAG, "Gatt Server is already running");
        oc_mutex_unlock(g_leServerStateMutex);
        return CA_STATUS_OK;
    }

    CAResult_t ret = CAInitLEGattServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "CAInitLEGattServer failed[%d]", ret);
        oc_mutex_unlock(g_leServerStateMutex);
        CATerminateLEGattServer();
        return CA_STATUS_FAILED;
    }

    ret  = CAAddNewLEServiceInGattServer(CA_GATT_SERVICE_UUID);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "CAAddNewLEServiceInGattServer failed[%d]", ret);
        oc_mutex_unlock(g_leServerStateMutex);
        CATerminateLEGattServer();
        return CA_STATUS_FAILED;
    }

    static const char charReadUUID[] = CA_GATT_RESPONSE_CHRC_UUID;
    char charReadValue[] = {33, 44, 55, 66}; // These are initial random values

    // For Read Characteristics.
    ret = CAAddNewCharacteristicsToGattServer(g_gattSvcPath, charReadUUID, charReadValue,
                                              CA_LE_INITIAL_BUF_SIZE, true);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "CAAddNewCharacteristicsToGattServer failed[%d]", ret);
        oc_mutex_unlock(g_leServerStateMutex);
        CATerminateLEGattServer();
        return CA_STATUS_FAILED;
    }

    static const char charWriteUUID[] = CA_GATT_REQUEST_CHRC_UUID;
    char charWriteValue[] = {33, 44, 55, 66}; // These are initial random values


    ret = CAAddNewCharacteristicsToGattServer(g_gattSvcPath, charWriteUUID, charWriteValue,
            CA_LE_INITIAL_BUF_SIZE, false); // For Write Characteristics.
    if (CA_STATUS_OK != ret )
    {
        OIC_LOG_V(ERROR, TAG, "CAAddNewCharacteristicsToGattServer failed[%d]", ret);
        oc_mutex_unlock(g_leServerStateMutex);
        CATerminateLEGattServer();
        return CA_STATUS_FAILED;
    }

    ret = CARegisterLEServicewithGattServer(g_gattSvcPath);
    if (CA_STATUS_OK != ret )
    {
        OIC_LOG_V(ERROR, TAG, "CARegisterLEServicewithGattServer failed[%d]", ret);
        oc_mutex_unlock(g_leServerStateMutex);
        CATerminateLEGattServer();
        return CA_STATUS_FAILED;
    }

    ret = CALEStartAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "CALEStartAdvertise failed[%d]", ret);
        oc_mutex_unlock(g_leServerStateMutex);
        CATerminateLEGattServer();
        return CA_STATUS_FAILED;
    }

    g_isLEGattServerStarted = true;

    oc_mutex_unlock(g_leServerStateMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CALENotificationCb(bool notify, bt_gatt_server_h server, bt_gatt_h gatt_handle,
                        void *user_data)
{
    OIC_LOG(DEBUG, TAG, "IN");
    if (notify)
    {
        OIC_LOG(DEBUG, TAG, "Notification is subscribed by the client");
        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, "", 0, -1,
                           true, "notifyChar success");
    }
    else
    {
        CALogSendStateInfo(CA_ADAPTER_GATT_BTLE, "", 0, -1,
                           false, "notifyChar failure");
    }
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CALEStartAdvertise()
{
    OIC_LOG(DEBUG, TAG, "IN");

    CAResult_t res = CALEStartAdvertiseImpl(CA_GATT_SERVICE_UUID);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "CALEStartAdvertiseImpl failed[%d]", res);
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return res;
}

CAResult_t CALEStartAdvertiseImpl(const char *serviceUUID)
{
    OIC_LOG(DEBUG, TAG, "IN");

    int length;

    int res = bt_adapter_le_create_advertiser(&g_hAdvertiser);
    if (NULL == g_hAdvertiser || BT_ERROR_NONE != res)
    {
        OIC_LOG_V(ERROR, TAG, "g_hAdvertiser is NULL/ Result is %d", res);
        return CA_STATUS_FAILED;
    }

    if (cutom_adv_flag == false) // Advertise with Default Service UUID
    {
        OIC_LOG(DEBUG, TAG, "Advertise with default Service UUID");

        res = bt_adapter_le_add_advertising_service_uuid(g_hAdvertiser,
                                                         BT_ADAPTER_LE_PACKET_ADVERTISING,
                                                         serviceUUID);
        if (BT_ERROR_NONE != res)
        {
            OIC_LOG_V(ERROR, TAG, "bt_adapter_le_add_advertising_service_uuid failed with ret[%s]",
                      CALEGetErrorMsg(res));
            return CA_STATUS_FAILED;
        }
    }
    else // Advertise with custom advertise data
    {
        OIC_LOG(DEBUG, TAG, "Advertise with custom advertise data");

        res = bt_adapter_le_add_advertising_manufacturer_data(g_hAdvertiser,BT_ADAPTER_LE_PACKET_ADVERTISING, samsung_code, custom_adv_data, custom_adv_data_length);
        if (BT_ERROR_NONE != res)
        {
            OIC_LOG_V(ERROR, TAG, "bt_adapter_le_add_advertising_manufacturer_data failed(BT_ADAPTER_LE_PACKET_ADVERTISING) with ret[%s]",
                      CALEGetErrorMsg(res));
            return CA_STATUS_FAILED;
        }

    }

    if(cutom_scanrsp_flag == false)
    {
        OIC_LOG(DEBUG, TAG, "Advertise with default scan response data");

        res = bt_adapter_le_set_advertising_device_name(g_hAdvertiser,
                                                        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE, true);
        if (BT_ERROR_NONE != res)
        {
            OIC_LOG_V(ERROR, TAG, "bt_adapter_le_set_advertising_device_name failed with ret[%s]",
                      CALEGetErrorMsg(res));
            return CA_STATUS_FAILED;
        }
    }
    else // Advertise with custom advertise data
    {
        OIC_LOG(DEBUG, TAG, "Advertise with custom scan response data");

        res = bt_adapter_le_add_advertising_manufacturer_data(g_hAdvertiser,BT_ADAPTER_LE_PACKET_SCAN_RESPONSE, samsung_code, custom_scanrsp_data, custom_scanrsp_data_length);
        if (BT_ERROR_NONE != res)
        {
            OIC_LOG_V(ERROR, TAG, "bt_adapter_le_add_advertising_manufacturer_data(BT_ADAPTER_LE_PACKET_SCAN_RESPONSE) failed with ret[%s]",
                      CALEGetErrorMsg(res));
            return CA_STATUS_FAILED;
        }

    }

    res = bt_adapter_le_start_advertising_new(g_hAdvertiser, NULL, NULL);
    if (BT_ERROR_NONE != res)
    {
        OIC_LOG_V(ERROR, TAG, "bt_adapter_le_start_advertising_new failed with ret[%s]",
                  CALEGetErrorMsg(res));
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CALEStopAdvertise()
{
    OIC_LOG(DEBUG, TAG, "IN");
    if (NULL != g_hAdvertiser)
    {
        int ret  = bt_adapter_le_stop_advertising(g_hAdvertiser);
        if (0 != ret)
        {
            OIC_LOG_V(ERROR, TAG,
                      "bt_adapter_le_stop_advertising failed with ret[%s]", CALEGetErrorMsg(ret));
        }

        ret = bt_adapter_le_destroy_advertiser(g_hAdvertiser);
        if (0 != ret)
        {
            OIC_LOG_V(ERROR, TAG,
                      "bt_adapter_le_destroy_advertiser failed with ret[%s]", CALEGetErrorMsg(ret));
        }
        g_hAdvertiser = NULL;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Advertising is not running");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAStopLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_leServerStateMutex);

    if (false == g_isLEGattServerStarted)
    {
        OIC_LOG(ERROR, TAG, "Gatt Server is not running to stop");
        oc_mutex_unlock(g_leServerStateMutex);
        return CA_STATUS_OK;
    }

    oc_mutex_lock(g_leServerStateMutex);
    g_isLEGattServerStarted = false;
    cutom_adv_flag = false;
    custom_adv_data = NULL;
    custom_adv_data_length = 0;
    cutom_scanrsp_flag = false;
    custom_scanrsp_data = NULL;
    custom_scanrsp_data_length = 0;
    oc_mutex_unlock(g_leServerStateMutex);

    oc_mutex_lock(g_LEClientListMutex);
    CADisconnectAllClient(g_LEClientList);
    g_LEClientList = NULL;
    oc_mutex_unlock(g_LEClientListMutex);

    CAResult_t res = CALEStopAdvertise();
    {
        OIC_LOG_V(ERROR, TAG, "CALEStopAdvertise failed with ret[%d]", res);
    }

    res = CADeInitLEGattServer();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "CADeInitLEGattService failed with ret[%d]", res);
    }

    GMainContext  *context_event_loop = NULL;
    // Required for waking up the thread which is running in gmain loop
    if (NULL != g_eventLoop)
    {
        context_event_loop = g_main_loop_get_context(g_eventLoop);

        if (context_event_loop)
        {
            OIC_LOG_V(DEBUG,  TAG, "g_eventLoop context %x", context_event_loop);
            g_main_context_wakeup(context_event_loop);

            // Kill g main loops and kill threads
            g_main_loop_quit(g_eventLoop);
        }
        g_eventLoop = NULL;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "g_eventLoop context is NULL");
    }

    oc_mutex_unlock(g_leServerStateMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAInitializeLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    CAResult_t ret = CAInitGattServerMutexVariables();
    if (CA_STATUS_OK != ret )
    {
        OIC_LOG(ERROR, TAG, "CAInitGattServerMutexVariables failed!");
        CATerminateGattServerMutexVariables();
        return CA_SERVER_NOT_STARTED;
    }
    OIC_LOG(DEBUG, TAG, "OUT");
    return ret;
}

void CATerminateLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    // Service and characteristics path will be freed by the platform.
    oc_mutex_lock(g_leServiceMutex);
    g_gattSvcPath = NULL;
    oc_mutex_unlock(g_leServiceMutex);

    oc_mutex_lock(g_leCharacteristicMutex);
    g_gattReadCharPath = NULL;
    g_gattWriteCharPath = NULL;
    oc_mutex_unlock(g_leCharacteristicMutex);

    oc_mutex_lock(g_leServerThreadPoolMutex);
    g_leServerThreadPool = NULL;
    oc_mutex_unlock(g_leServerThreadPoolMutex);

    // Terminating all mutex variables.
    CATerminateGattServerMutexVariables();
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAInitGattServerMutexVariables()
{
    OIC_LOG(DEBUG, TAG, "IN");
    if (NULL == g_leServerStateMutex)
    {
        g_leServerStateMutex = oc_mutex_new();
        if (NULL == g_leServerStateMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_leServiceMutex)
    {
        g_leServiceMutex = oc_mutex_new();
        if (NULL == g_leServiceMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_leCharacteristicMutex)
    {
        g_leCharacteristicMutex = oc_mutex_new();
        if (NULL == g_leCharacteristicMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_leReqRespCbMutex)
    {
        g_leReqRespCbMutex = oc_mutex_new();
        if (NULL == g_leReqRespCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_leServerThreadPoolMutex)
    {
        g_leServerThreadPoolMutex = oc_mutex_new();
        if (NULL == g_leServerThreadPoolMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_LEClientListMutex)
    {
        g_LEClientListMutex = oc_mutex_new();
        if (NULL == g_LEClientListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new failed");
            return CA_STATUS_FAILED;
        }
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CATerminateGattServerMutexVariables()
{
    OIC_LOG(DEBUG, TAG, "IN");
    oc_mutex_free(g_leServerStateMutex);
    g_leServerStateMutex = NULL;

    oc_mutex_free(g_leServiceMutex);
    g_leServiceMutex = NULL;

    oc_mutex_free(g_leCharacteristicMutex);
    g_leCharacteristicMutex = NULL;

    oc_mutex_free(g_leReqRespCbMutex);
    g_leReqRespCbMutex = NULL;

    oc_mutex_free(g_leServerThreadPoolMutex);
    g_leServerThreadPoolMutex = NULL;

    oc_mutex_free(g_LEClientListMutex);
    g_LEClientListMutex = NULL;

    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAInitLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    int ret =  bt_gatt_server_initialize();
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_initialize failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    if (!g_gattServer)
    {
        OIC_LOG(DEBUG, TAG, "g_gattServer is NULL. create gatt server..");
        ret = bt_gatt_server_create(&g_gattServer);
        if (0 != ret)
        {
            OIC_LOG_V(ERROR, TAG, "bt_gatt_server_create failed with ret[%s]",
                      CALEGetErrorMsg(ret));
            return CA_STATUS_FAILED;
        }
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CADeInitLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    int ret = bt_gatt_server_unregister_all_services(g_gattServer);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_unregister_all_services failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    ret = bt_gatt_server_destroy(g_gattServer);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_destroy failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }
    g_gattServer = NULL;

    ret =  bt_gatt_server_deinitialize();
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_deinitialize failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CASetLEServerThreadPoolHandle(ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "IN");
    oc_mutex_lock(g_leServerThreadPoolMutex);
    g_leServerThreadPool = handle;
    oc_mutex_unlock(g_leServerThreadPoolMutex);
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAAddNewLEServiceInGattServer(const char *serviceUUID)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(serviceUUID, TAG, "serviceUUID");

    OIC_LOG_V(DEBUG, TAG, "service uuid %s", serviceUUID);

    bt_gatt_service_type_e type = BT_GATT_SERVICE_TYPE_PRIMARY;

    oc_mutex_lock(g_leServiceMutex);
    int ret = bt_gatt_service_create(serviceUUID, type, &g_gattSvcPath);
    if (0 != ret)
    {
        oc_mutex_unlock(g_leServiceMutex);
        OIC_LOG_V(ERROR, TAG, "bt_gatt_service_create failed with ret [%s]",
                    CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }
    oc_mutex_unlock(g_leServiceMutex);

    if (g_gattSvcPath)
    {
        OIC_LOG_V(DEBUG, TAG, "ServicePath obtained is %s", (char *)g_gattSvcPath);
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

#ifdef BLE_TIZEN_30
void CALEGattRemoteCharacteristicWriteCb(const char *remoteAddress, int request_id,
                                         bt_gatt_server_h server, bt_gatt_h gatt_handle,
                                         bool response_needed, int offset, const char *charValue,
                                         int charLen, void *userData)
#else
void CALEGattRemoteCharacteristicWriteCb(char *remoteAddress, bt_gatt_server_h server,
                                         bt_gatt_h gatt_handle, int offset, char *charValue,
                                         int charLen, void *userData)
#endif
{
    OIC_LOG(INFO, TAG, "IN - WriteCharCB");

    if (NULL == charValue || NULL == remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "Param callback values are NULL");
        return;
    }

    OIC_LOG_V(DEBUG, TAG, "len [%d]", charLen);

    uint8_t *data = OICMalloc(charLen);
    if (NULL == data)
    {
        OIC_LOG(ERROR, TAG, "Malloc failed!");
        return;
    }

    memcpy(data, charValue, charLen);

    oc_mutex_lock(g_leReqRespCbMutex);
    if (NULL == g_leServerDataReceivedCallback)
    {
        OIC_LOG(ERROR, TAG, "gReqRespCallback is NULL!");
        oc_mutex_unlock(g_leReqRespCbMutex);
        OICFree(data);
        return;
    }

    OIC_LOG(INFO, TAG, "Sending data up !");
    uint32_t sentLength = 0;
    g_leServerDataReceivedCallback(remoteAddress, data, charLen,
                                    &sentLength);
    oc_mutex_unlock(g_leReqRespCbMutex);
    OICFree(data);

#ifdef BLE_TIZEN_30
    //response
    OIC_LOG_V(INFO, TAG, "response_needed flag : %d", response_needed);
    if (response_needed)
    {
        OIC_LOG(INFO, TAG, "send response to remote client");
        bt_gatt_server_send_response(request_id,
                BT_GATT_REQUEST_TYPE_WRITE, offset,
                BT_ERROR_NONE, NULL, 0);
    }
#endif

    OIC_LOG(INFO, TAG, "OUT - WriteCharCB");
}

CAResult_t CARegisterLEServicewithGattServer(const bt_gatt_h svcPath)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(svcPath, TAG, "svcPath");

    OIC_LOG_V(DEBUG, TAG, "svcPath:%s", svcPath);

    int ret = bt_gatt_server_register_service(g_gattServer, svcPath);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_register_service failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    ret = bt_gatt_server_start();
    if(0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_start failed with ret[%s]",
                CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

#ifdef BLE_TIZEN_30
    ret = bt_gatt_server_set_write_value_requested_cb(g_gattWriteCharPath,
#else
    ret = bt_gatt_server_set_value_changed_cb(g_gattWriteCharPath,
#endif
                                              CALEGattRemoteCharacteristicWriteCb, NULL);

    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_server_set_value_changed_cb failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAAddNewCharacteristicsToGattServer(const bt_gatt_h svcPath, const char *charUUID,
                                               const char *charValue, int charValueLen, bool read)
{

    OIC_LOG(DEBUG, TAG, "IN");

    int permissions = BT_GATT_PERMISSION_READ | BT_GATT_PERMISSION_WRITE;
    int properties;
    if (read)
    {
        properties = BT_GATT_PROPERTY_INDICATE;
    }
    else
    {
        properties = BT_GATT_PROPERTY_WRITE;
    }

    bt_gatt_h charPath = NULL;

    int ret = bt_gatt_characteristic_create(charUUID, permissions, properties, charValue,
                                            charValueLen, &charPath);

    if (0 != ret || NULL == charPath)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_add_characteristic  failed with ret [%s]", CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG,
              "bt_gatt_characteristic_create charPath obtained: %s", (char *)charPath);

    if (read)
    {
#ifdef BLE_TIZEN_30
        ret = bt_gatt_server_set_characteristic_notification_state_change_cb(charPath,
                                                                             CALENotificationCb,
                                                                             NULL);
#else
        ret = bt_gatt_server_set_notification_state_change_cb(charPath, CALENotificationCb, NULL);
#endif
        if (0 != ret)
        {
            OIC_LOG_V(ERROR, TAG,
#ifdef BLE_TIZEN_30
                      "bt_gatt_server_set_characteristic_notification_state_change_cb failed with ret[%s]",
#else
                      "bt_gatt_server_set_notification_state_change_cb  failed with ret[%s]",
#endif
                      CALEGetErrorMsg(ret));
            return CA_STATUS_FAILED;
        }
    }

    ret =  bt_gatt_service_add_characteristic(svcPath, charPath);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_service_add_characteristic  failed with ret[%s]",
                  CALEGetErrorMsg(ret));
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_leCharacteristicMutex);

    if (read)
    {
        char desc_value[2] = {1, 0};  // Notification enabled.
        bt_gatt_h descriptor = NULL;
        permissions = BT_GATT_PERMISSION_READ | BT_GATT_PERMISSION_WRITE;
        ret = bt_gatt_descriptor_create(CA_GATT_CONFIGURATION_DESC_UUID, permissions,
                                        desc_value, sizeof(desc_value),
                                        &descriptor);
        if (0 != ret)
        {
            oc_mutex_unlock(g_leCharacteristicMutex);
            OIC_LOG_V(ERROR, TAG,
                      "bt_gatt_descriptor_create  failed with ret[%s]",
                      CALEGetErrorMsg(ret));
            return CA_STATUS_FAILED;
        }

        ret = bt_gatt_characteristic_add_descriptor(charPath, descriptor);
        if (0 != ret)
        {
            oc_mutex_unlock(g_leCharacteristicMutex);
            OIC_LOG_V(ERROR, TAG,
                      "bt_gatt_characteristic_add_descriptor  failed with ret[%s]",
                      CALEGetErrorMsg(ret));
            return CA_STATUS_FAILED;
        }

        g_gattReadCharPath = charPath;
    }
    else
    {
        g_gattWriteCharPath = charPath;
    }

    oc_mutex_unlock(g_leCharacteristicMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAUpdateCharacteristicsToGattClient(const char *address, const uint8_t *charValue,
                                               uint32_t charValueLen)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(charValue, TAG, "charValue");
    VERIFY_NON_NULL(address, TAG, "address");

    OIC_LOG_V(DEBUG, TAG, "Client's Unicast address for sending data [%s]", address);

    oc_mutex_lock(g_leCharacteristicMutex);

    if (NULL  == g_gattReadCharPath)
    {
        OIC_LOG(ERROR, TAG, "g_gattReadCharPath is NULL");
        oc_mutex_unlock(g_leCharacteristicMutex);
        return CA_STATUS_FAILED;
    }

    int ret = bt_gatt_set_value(g_gattReadCharPath, (char *)charValue, charValueLen);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_gatt_set_value failed with return [%s]", CALEGetErrorMsg(ret));
        oc_mutex_unlock(g_leCharacteristicMutex);
        return CA_STATUS_FAILED;
    }

#ifdef BLE_TIZEN_30
    ret = bt_gatt_server_notify_characteristic_changed_value(g_gattReadCharPath,
                                                             CALEServerNotificationSentCB,
                                                             address, NULL);
#else
    ret = bt_gatt_server_notify(g_gattReadCharPath, false, CALEServerNotificationSentCB,
                                address, NULL);
#endif

    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG,
#ifdef BLE_TIZEN_30
                  "bt_gatt_server_notify_characteristic_changed_value failed with return [%s]",
#else
                  "bt_gatt_server_notify failed with return [%s]",
#endif
                  CALEGetErrorMsg(ret));
        oc_mutex_unlock(g_leCharacteristicMutex);
        return CA_STATUS_FAILED;
    }

    oc_mutex_unlock(g_leCharacteristicMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAUpdateCharacteristicsToAllGattClients(const uint8_t *charValue, uint32_t charValueLen)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(charValue, TAG, "charValue");

    oc_mutex_lock(g_leCharacteristicMutex);

    if (NULL  == g_gattReadCharPath)
    {
        OIC_LOG(ERROR, TAG, "g_gattReadCharPath is NULL");
        oc_mutex_unlock(g_leCharacteristicMutex);
        return CA_STATUS_FAILED;
    }

    int ret = bt_gatt_set_value(g_gattReadCharPath, (char *)charValue, charValueLen);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG, "bt_gatt_set_value failed with return[%s]", CALEGetErrorMsg(ret));
        oc_mutex_unlock(g_leCharacteristicMutex);
        return CA_STATUS_FAILED;
    }

#ifdef BLE_TIZEN_30
    ret = bt_gatt_server_notify_characteristic_changed_value(g_gattReadCharPath,
                                                             CALEServerNotificationSentCB,
                                                             NULL, NULL);
#else
    ret = bt_gatt_server_notify(g_gattReadCharPath, false, CALEServerNotificationSentCB,
                                NULL, NULL);
#endif
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG,
#ifdef BLE_TIZEN_30
                  "bt_gatt_server_notify_characteristic_changed_value failed with return[%s]",
#else
                  "bt_gatt_server_notify failed with return[%s]",
#endif
                  CALEGetErrorMsg(ret));
        oc_mutex_unlock(g_leCharacteristicMutex);
        return CA_STATUS_FAILED;
    }

    oc_mutex_unlock(g_leCharacteristicMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CASetLEReqRespServerCallback(CABLEDataReceivedCallback callback)
{
    OIC_LOG(DEBUG, TAG, "IN");

    oc_mutex_lock(g_leReqRespCbMutex);
    g_leServerDataReceivedCallback = callback;
    oc_mutex_unlock(g_leReqRespCbMutex);

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CASetBLEServerErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    g_serverErrorCallback = callback;
}

bool CALEServerIsConnected(const char* address)
{
    //@Todo
    return true;
}

uint16_t CALEServerGetMtuSize(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN");
    VERIFY_NON_NULL_RET(address, TAG, "address is null", CA_DEFAULT_BLE_MTU_SIZE);

    unsigned int mtu;
    int ret = bt_device_get_att_mtu(address, &mtu);
    if (0 != ret)
    {
        OIC_LOG_V(ERROR, TAG,
                  "bt_device_get_att_mtu failed with return [%s]", CALEGetErrorMsg(ret));
        return CA_DEFAULT_BLE_MTU_SIZE;
    }
    OIC_LOG_V(INFO, TAG, "mtu size(including header) from bt_device_get_att_mtu is %d", mtu);
    OIC_LOG(DEBUG, TAG, "OUT");
    return mtu - CA_BLE_MTU_HEADER_SIZE;
}
