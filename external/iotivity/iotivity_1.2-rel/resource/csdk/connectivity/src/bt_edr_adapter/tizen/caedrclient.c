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
 * This file provides the APIs to establish RFCOMM connection with remote
 * bluetooth device.
 */

#include <string.h>
#include <bluetooth.h>
#include <bluetooth_internal.h>

#include "caedrinterface.h"
#include "octhread.h"
#include "caedrendpoint.h"
#include "caadapterutils.h"
#include "caedrutils.h"
#include "logger.h"
#include "cacommon.h"
#include "caedrdevicelist.h"

#define MICROSECS_PER_SEC 1000000

/**
 * Maximum CoAP over TCP header length
 * to know the total data length.
 */
#define EDR_MAX_HEADER_LEN  6

/**
 * Mutex to synchronize the access to Bluetooth device information list.
 */
static oc_mutex g_edrDeviceListMutex = NULL;

/**
 * Peer Bluetooth device information list.
 */
static EDRDeviceList *g_edrDeviceList = NULL;

/**
 * Maintains the callback to be notified when data received from remote
 * Bluetooth device.
 */
static CAEDRDataReceivedCallback g_edrPacketReceivedCallback = NULL;

/**
 * Error callback to update error in EDR.
 */
static CAEDRErrorHandleCallback g_edrErrorHandler = NULL;

/**
 * Pending multicast data list to be sent.
 */
static u_arraylist_t *g_multicastDataList = NULL;

/**
 * Mutex to synchronize the access to Pending multicast data list.
 */
static oc_mutex g_multicastDataListMutex = NULL;

/**
 * To Store Adapter Mode information
 */
static bool g_isDiscoveryServer = false;

/**
 * This function creates mutex.
 */
static CAResult_t CAEDRManagerInitializeMutex(void);

/**
 * This function frees mutex.
 */
static void CAEDRManagerTerminateMutex(void);

/**
 * This callback is registered to recieve data on any open RFCOMM connection.
 */
static void CAEDRDataRecvCallback(bt_socket_received_data_s *data, void *userData);

/**
 * This function stops any ongoing service sevice search.
 */
static CAResult_t CAEDRStopServiceSearch(void);

/**
 * This function stops device discovery.
 */
static CAResult_t CAEDRStopDeviceDiscovery(void);

/**
 * This function searches for OIC service for remote Bluetooth device.
 */
static CAResult_t CAEDRStartServiceSearch(const char *remoteAddress);

/**
 * This callback is registered to recieve all bluetooth nearby devices
 * when device scan is initiated.
 */
static void CAEDRDeviceDiscoveryCallback(int result,
                                         bt_adapter_device_discovery_state_e state,
                                         bt_adapter_device_discovery_info_s *discoveryInfo,
                                         void *userData);

/**
 * This callback is registered to recieve all the services remote
 * bluetooth device supports when service search initiated.
 */
static void CAEDRServiceSearchedCallback(int result, bt_device_sdp_info_s *sdpInfo,
                                        void *userData);

/**
 * This callback is registered to receive bluetooth RFCOMM connection
 * state changes.
 */
static void CAEDRSocketConnectionStateCallback(int result,
                                    bt_socket_connection_state_e state,
                                              bt_socket_connection_s *connection, void *userData);

/**
 * Establishes RFCOMM connection with remote bluetooth device.
 */
static CAResult_t CAEDRClientConnect(const char *remoteAddress, const char *serviceUUID);

/**
 * Disconnect RFCOMM client socket connection.
 */
static CAResult_t CAEDRClientDisconnect(const int32_t clientID);

void CAEDRSetPacketReceivedCallback(CAEDRDataReceivedCallback packetReceivedCallback)
{
    g_edrPacketReceivedCallback = packetReceivedCallback;
}

void CAEDRSetErrorHandler(CAEDRErrorHandleCallback errorHandleCallback)
{
    g_edrErrorHandler = errorHandleCallback;
}

void CAEDRSocketConnectionStateCallback(int result, bt_socket_connection_state_e state,
                                       bt_socket_connection_s *connection, void *userData)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    EDRDevice *device = NULL;

    if (BT_ERROR_NONE != result || NULL == connection)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Invalid connection state!, error num [%x]",
                  result);
        return;
    }

    switch (state)
    {
        case BT_SOCKET_CONNECTED:
            {
                oc_mutex_lock(g_edrDeviceListMutex);
                CAResult_t res = CAGetEDRDevice(g_edrDeviceList, connection->remote_address,
                                                   &device);
                if (CA_STATUS_OK != res)
                {
                    // Create the deviceinfo and add to list
                    res = CACreateAndAddToDeviceList(&g_edrDeviceList,
                            connection->remote_address, OIC_EDR_SERVICE_ID, &device);
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed add device to list ret[%d]", res);
                        oc_mutex_unlock(g_edrDeviceListMutex);
                        return;
                    }

                    if(!device)
                    {
                        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "EDRDevice is null!");
                        oc_mutex_unlock(g_edrDeviceListMutex);
                        return;
                    }

                    device->socketFD = connection->socket_fd;
                    oc_mutex_unlock(g_edrDeviceListMutex);
                    return;
                }

                if(!device)
                {
                    OIC_LOG(ERROR, EDR_ADAPTER_TAG, "EDRDevice is null!");
                    oc_mutex_unlock(g_edrDeviceListMutex);
                    return;
                }
                device->socketFD = connection->socket_fd;
                while (device->pendingDataList)
                {
                    EDRData *edrData = device->pendingDataList->data;
                    res = CAEDRSendData(device->socketFD, device->remoteAddress,
                                        edrData->data, edrData->dataLength);
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed to send pending data [%s]",
                                  device->remoteAddress);

                        // Remove all the data from pending list
                        CADestroyEDRDataList(&device->pendingDataList);
                        break;
                    }

                    // Remove the data which send from pending list
                    CARemoveEDRDataFromList(&device->pendingDataList);
                }
                oc_mutex_unlock(g_edrDeviceListMutex);
            }
            break;

        case BT_SOCKET_DISCONNECTED:
            {
                oc_mutex_lock(g_edrDeviceListMutex);
                CARemoveEDRDeviceFromList(&g_edrDeviceList, connection->remote_address);
                oc_mutex_unlock(g_edrDeviceListMutex);
            }
            break;

        default:
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Unknown Bluetooth Socket connection state");
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}


void CAEDRDeviceDiscoveryCallback(int result, bt_adapter_device_discovery_state_e state,
                                 bt_adapter_device_discovery_info_s *discoveryInfo, void *userData)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    EDRDevice *device = NULL;

    if (BT_ERROR_NONE != result)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Received bad state!, error num [%x]",
                  result);
        return;
    }

    switch (state)
    {
        case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
            {
                OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Discovery started!");
            }
            break;

        case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
            {
                OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Discovery finished!");
                oc_mutex_lock(g_multicastDataListMutex);
                u_arraylist_destroy(g_multicastDataList);
                g_multicastDataList = NULL;
                oc_mutex_unlock(g_multicastDataListMutex);
            }
            break;

        case BT_ADAPTER_DEVICE_DISCOVERY_FOUND:
            {
                OIC_LOG_V(DEBUG, EDR_ADAPTER_TAG, "Device discovered [%s]!",
                          discoveryInfo->remote_name);
                if (true == CAEDRIsServiceSupported((const char **)discoveryInfo->service_uuid,
                                                        discoveryInfo->service_count,
                                                        OIC_EDR_SERVICE_ID))
                {
                    // Check if the deivce is already in the list
                    oc_mutex_lock(g_edrDeviceListMutex);
                    if (CA_STATUS_OK == CAGetEDRDevice(g_edrDeviceList,
                                                discoveryInfo->remote_address, &device))
                    {
                        if(!device)
                        {
                            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "EDRDevice is null!");
                            oc_mutex_unlock(g_edrDeviceListMutex);
                            return;
                        }
                        device->serviceSearched = true;
                        oc_mutex_unlock(g_edrDeviceListMutex);
                        return;
                    }

                    // Create the deviceinfo and add to list
                    CAResult_t res = CACreateAndAddToDeviceList(&g_edrDeviceList,
                            discoveryInfo->remote_address, OIC_EDR_SERVICE_ID, &device);
                    if (CA_STATUS_OK != res)
                    {
                        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to add device to list!");
                        oc_mutex_unlock(g_edrDeviceListMutex);
                        return;
                    }

                    if(!device)
                    {
                        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "EDRDevice is null!");
                        oc_mutex_unlock(g_edrDeviceListMutex);
                        return;
                    }

                    int lengthData = u_arraylist_length(g_multicastDataList);
                    for(int len = 0; len < lengthData; len++)
                    {
                        // Adding to pending list
                        EDRData *multicastData =
                            (EDRData *)u_arraylist_get(g_multicastDataList, len);
                        if (NULL == multicastData)
                        {
                            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "multicastData is NULL");
                            continue;
                        }
                        result = CAAddEDRDataToList(&device->pendingDataList, multicastData->data,
                                                    multicastData->dataLength);
                        if (CA_STATUS_OK != result)
                        {
                            OIC_LOG_V(ERROR, EDR_ADAPTER_TAG,
                                      "Failed to add data to pending list[%d]", result);
                            continue;
                        }
                    }
                    if (lengthData)
                    {
                        result = CAEDRClientConnect(device->remoteAddress, device->serviceUUID);
                        if (CA_STATUS_OK != result)
                        {
                            OIC_LOG_V(ERROR, EDR_ADAPTER_TAG,
                                      "Failed to make RFCOMM connection[%d]", result);

                            //Remove the data which added to pending list
                            CARemoveEDRDataFromList(&device->pendingDataList);
                        }
                    }
                    device->serviceSearched = true;
                    oc_mutex_unlock(g_edrDeviceListMutex);
                }
                else
                {
                    OIC_LOG(INFO, EDR_ADAPTER_TAG, "Device does not support OIC service!");
                }
            }
            break;

        default:
            OIC_LOG(INFO, EDR_ADAPTER_TAG, "Unknown Bluetooth Adapter device discovery state");
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}

void CAEDRServiceSearchedCallback(int32_t result,
                bt_device_sdp_info_s *sdpInfo,void *userData)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    if (NULL == sdpInfo)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "SDP info is null!");
        return;
    }

    oc_mutex_lock(g_edrDeviceListMutex);

    EDRDevice *device = NULL;
    CAResult_t res = CAGetEDRDevice(g_edrDeviceList, sdpInfo->remote_address, &device);
    if (CA_STATUS_OK == res && NULL != device)
    {
        if (device->serviceSearched)
        {
            OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Service is already searched for this device!");
            oc_mutex_unlock(g_edrDeviceListMutex);
            return;
        }

        if (true == CAEDRIsServiceSupported((const char **)sdpInfo->service_uuid,
                                           sdpInfo->service_count, OIC_EDR_SERVICE_ID))
        {
            device->serviceSearched = true;
            res = CAEDRClientConnect(sdpInfo->remote_address, OIC_EDR_SERVICE_ID);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to make rfcomm connection!");

                // Remove the device from device list
                CARemoveEDRDeviceFromList(&g_edrDeviceList, sdpInfo->remote_address);
            }
        }
        else
        {
            OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Device does not contain OIC service!");

            // Remove device from list as it does not support OIC service
            CARemoveEDRDeviceFromList(&g_edrDeviceList, sdpInfo->remote_address);
        }
    }

    oc_mutex_unlock(g_edrDeviceListMutex);

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}

CAResult_t CAEDRStartDeviceDiscovery(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    bool isDiscoveryStarted = false;

    // Check the device discovery state
    bt_error_e err = bt_adapter_is_discovering(&isDiscoveryStarted);
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed to get discovery state!, error num [%x]",
                  err);
        return CA_STATUS_FAILED;
    }

    //Start device discovery if its not started
    if (false == isDiscoveryStarted)
    {
        err = bt_adapter_start_device_discovery();
        if (BT_ERROR_NONE != err)
        {
            OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Device discovery failed!, error num [%x]",
                      err);
            return CA_STATUS_FAILED;
        }
    }

    g_isDiscoveryServer = true;

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRStopServiceSearch(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    bt_error_e err = bt_device_cancel_service_search();
    // Stop ongoing service search
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Get bonded device failed!, error num [%x]",
                  err);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRStopDeviceDiscovery(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    bool isDiscoveryStarted = false;
    bt_error_e err = bt_adapter_is_discovering(&isDiscoveryStarted);
    // Check the device discovery state
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed to get discovery state!, error num [%x]",
                  err);
        return CA_STATUS_FAILED;
    }

    //stop the device discovery process
    if (true == isDiscoveryStarted)
    {
        OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Stopping the device search process");
        if (BT_ERROR_NONE != (err = bt_adapter_stop_device_discovery()))
        {
            OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed to stop discovery!, error num [%x]",
                      err);
        }
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRStartServiceSearch(const char *remoteAddress)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Input validation
    VERIFY_NON_NULL(remoteAddress, EDR_ADAPTER_TAG, "Remote address is null");
    if (!remoteAddress[0])
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Remote address is empty!");
        return CA_STATUS_INVALID_PARAM;
    }

    bt_error_e err = bt_device_start_service_search(remoteAddress);
    // Start searching for OIC service
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Get bonded device failed!, error num [%x]",
                  err);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRClientSetCallbacks(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Register for discovery and rfcomm socket connection callbacks
    bt_adapter_set_device_discovery_state_changed_cb(CAEDRDeviceDiscoveryCallback, NULL);
    bt_device_set_service_searched_cb(CAEDRServiceSearchedCallback, NULL);
    bt_socket_set_connection_state_changed_cb(CAEDRSocketConnectionStateCallback, NULL);
    bt_socket_set_data_received_cb(CAEDRDataRecvCallback, NULL);

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}


void CAEDRClientUnsetCallbacks(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Stop service search
    CAEDRStopServiceSearch();

    // Stop the device discovery process
    CAEDRStopDeviceDiscovery();

    // reset bluetooth adapter callbacks
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Resetting the callbacks");
    bt_adapter_unset_device_discovery_state_changed_cb();
    bt_device_unset_service_searched_cb();
    bt_socket_unset_connection_state_changed_cb();
    bt_socket_unset_data_received_cb();

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}

CAResult_t CAEDRManagerInitializeMutex(void)
{
    CAResult_t result = CA_STATUS_OK;
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    if (!g_edrDeviceListMutex)
    {
        g_edrDeviceListMutex = oc_mutex_new();
    }

    if (!g_multicastDataListMutex)
    {
        g_multicastDataListMutex = oc_mutex_new();
    }

    if (!g_edrDeviceListMutex || !g_multicastDataListMutex)
    {
        result = CA_STATUS_NOT_INITIALIZED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");

    return result;
}

void CAEDRManagerTerminateMutex(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    if (g_edrDeviceListMutex)
    {
        oc_mutex_free(g_edrDeviceListMutex);
        g_edrDeviceListMutex = NULL;
    }

    if (g_multicastDataListMutex)
    {
        oc_mutex_free(g_multicastDataListMutex);
        g_multicastDataListMutex = NULL;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}

CAResult_t CAEDRClientInitialize()
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");
    CAResult_t result = CAEDRManagerInitializeMutex();
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return result;
}

void CAEDRClientTerminate()
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Free EDRDevices list
    if (g_edrDeviceListMutex)
    {
        oc_mutex_lock(g_edrDeviceListMutex);
        CADestroyEDRDeviceList(&g_edrDeviceList);
        oc_mutex_unlock(g_edrDeviceListMutex);
    }

    if (g_multicastDataListMutex)
    {
        oc_mutex_lock(g_multicastDataListMutex);
        u_arraylist_destroy(g_multicastDataList);
        g_multicastDataList = NULL;
        oc_mutex_unlock(g_multicastDataListMutex);
    }

    // Free the mutex
    CAEDRManagerTerminateMutex();
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}

void CAEDRClientDisconnectAll(void)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    oc_mutex_lock(g_edrDeviceListMutex);

    EDRDeviceList *cur = g_edrDeviceList;
    while (cur != NULL)
    {
        EDRDevice *device = cur->device;
        cur = cur->next;

        if (device && 0 <= device->socketFD)
        {
            CAResult_t result = CAEDRClientDisconnect(device->socketFD);
            if (CA_STATUS_OK != result)
            {
                OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed to disconnect with client :%s",
                          device->remoteAddress);
            }

            device->socketFD = -1;
        }
    }

    oc_mutex_unlock(g_edrDeviceListMutex);

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}


CAResult_t CAEDRClientSendUnicastData(const char *remoteAddress,
                                      const uint8_t *data,
                                      uint32_t dataLength)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    EDRDevice *device = NULL;

    // Input validation
    VERIFY_NON_NULL(remoteAddress, EDR_ADAPTER_TAG, "Remote address is null");
    VERIFY_NON_NULL(data, EDR_ADAPTER_TAG, "Data is null");

    if (0 >= dataLength)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Invalid input: Negative data length!");
        return CA_STATUS_INVALID_PARAM;
    }

    // Check the connection existence with remote device
    oc_mutex_lock(g_edrDeviceListMutex);
    CAResult_t result = CAGetEDRDevice(g_edrDeviceList, remoteAddress, &device);
    if (CA_STATUS_OK != result)
    {
        // Create new device and add to list
        result = CACreateAndAddToDeviceList(&g_edrDeviceList, remoteAddress,
                                            OIC_EDR_SERVICE_ID, &device);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed create device and add to list!");

            oc_mutex_unlock(g_edrDeviceListMutex);
            return CA_STATUS_FAILED;
        }

        // Start the OIC service search newly created device
        result = CAEDRStartServiceSearch(remoteAddress);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to initiate service search!");

            // Remove device from list
            CARemoveEDRDeviceFromList(&g_edrDeviceList, remoteAddress);

            oc_mutex_unlock(g_edrDeviceListMutex);
            return CA_STATUS_FAILED;
        }
    }

    if(!device)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "EDRDevice is null!");
        // Remove device from list
        CARemoveEDRDeviceFromList(&g_edrDeviceList, remoteAddress);

        oc_mutex_unlock(g_edrDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    oc_mutex_unlock(g_edrDeviceListMutex);

    if (-1 == device->socketFD)
    {
        // Adding to pending list
        result = CAAddEDRDataToList(&device->pendingDataList, data,
                                              dataLength);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to add data to pending list!");

            //Remove device from list
            CARemoveEDRDeviceFromList(&g_edrDeviceList, remoteAddress);
            return CA_STATUS_FAILED;
        }

        // Make a rfcomm connection with remote BT Device
        if (device->serviceSearched &&
            CA_STATUS_OK != CAEDRClientConnect(remoteAddress, OIC_EDR_SERVICE_ID))
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to make RFCOMM connection!");

            //Remove device from list
            CARemoveEDRDeviceFromList(&g_edrDeviceList, remoteAddress);
            return CA_STATUS_FAILED;
        }
    }
    else
    {
        result = CAEDRSendData(device->socketFD, device->remoteAddress, data, dataLength);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to send data!");
            return CA_STATUS_FAILED;
        }
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRClientSendMulticastData(const uint8_t *data,
                                        uint32_t dataLength)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Input validation
    VERIFY_NON_NULL(data, EDR_ADAPTER_TAG, "Data is null");

    if (0 >= dataLength)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Invalid input: Negative data length!");
        return CA_STATUS_INVALID_PARAM;
    }

    // Send the packet to all OIC devices
    oc_mutex_lock(g_edrDeviceListMutex);

    EDRDeviceList *curList = g_edrDeviceList;
    CAResult_t result = CA_STATUS_FAILED;
    while (curList != NULL)
    {
        EDRDevice *device = curList->device;
        curList = curList->next;

        if (!device)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "There is no device!");
            break;
        }

        if (-1 == device->socketFD)
        {
            // Check if the device service search is finished
            if (false == device->serviceSearched)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Device services are still unknown!");
                continue;
            }

            // Adding to pendding list
            result = CAAddEDRDataToList(&device->pendingDataList, data,
                                                  dataLength);
            if (CA_STATUS_OK != result)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to add data to pending list !");
                continue;
            }

            // Make a rfcomm connection with remote BT Device
            result = CAEDRClientConnect(device->remoteAddress, device->serviceUUID);
            if (CA_STATUS_OK != result)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to make RFCOMM connection !");

                //Remove the data which added to pending list
                CARemoveEDRDataFromList(&device->pendingDataList);
                continue;
            }
        }
        else
        {
            result = CAEDRSendData(device->socketFD, device->remoteAddress ,data, dataLength);
            if (CA_STATUS_OK != result)
            {
                OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed to send data to [%s] !",
                          device->remoteAddress);
            }
        }
    }

    oc_mutex_unlock(g_edrDeviceListMutex);

    if(g_isDiscoveryServer)
    {
        // Start the device Discovery.
        result = CAEDRStartDeviceDiscovery();
        if (CA_STATUS_OK == result)
        {
            OIC_LOG(INFO, EDR_ADAPTER_TAG, "Add the data to the multicast data list");

            EDRData *multicastData = (EDRData *)OICCalloc(1, sizeof(EDRData));
            if (NULL == multicastData)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Malloc failed");
                goto exit;
            }
            multicastData->data = OICCalloc(1, dataLength);
            if (NULL == multicastData->data)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Malloc failed");
                goto exit;
            }
            memcpy(multicastData->data, data, dataLength);
            multicastData->dataLength = dataLength;

            // Add the data to pending multicast data list.
            oc_mutex_lock(g_multicastDataListMutex);
            if (NULL == g_multicastDataList)
            {
                g_multicastDataList = u_arraylist_create();
            }
            u_arraylist_add(g_multicastDataList, (void *)multicastData);
            oc_mutex_unlock(g_multicastDataListMutex);
        }
    }

exit:
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRClientConnect(const char *remoteAddress, const char *serviceUUID)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    VERIFY_NON_NULL(remoteAddress, EDR_ADAPTER_TAG, "Remote address is null");
    VERIFY_NON_NULL(serviceUUID, EDR_ADAPTER_TAG, "Service UUID is null");

    size_t addressLen = strlen(remoteAddress);
    if (0 == addressLen || CA_MACADDR_SIZE - 1 != addressLen)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Invalid input: Invalid remote address");
        return  CA_STATUS_INVALID_PARAM;
    }

    if (!serviceUUID[0])
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Invalid input: Empty service uuid");
        return  CA_STATUS_INVALID_PARAM;
    }

    bt_error_e err = bt_socket_connect_rfcomm(remoteAddress, serviceUUID);
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG,
                  "Failed to connect!, address [%s] error num [%x]",
                  remoteAddress, err);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRClientDisconnect(const int32_t clientID)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Input validation
    if (0 > clientID)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Invalid input: negative client id");
        return CA_STATUS_INVALID_PARAM;
    }

    bt_error_e err = bt_socket_disconnect_rfcomm(clientID);
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Failed close rfcomm client socket!, error num [%x]",
                  err);
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

void CAEDRDataRecvCallback(bt_socket_received_data_s *data, void *userData)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    EDRDevice *device = NULL;

    if (NULL == data || 0 >= data->data_size)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Data is null!");
        return;
    }

    // Get EDR device from list
    oc_mutex_lock(g_edrDeviceListMutex);
    CAResult_t result = CAGetEDRDeviceBySocketId(g_edrDeviceList, data->socket_fd, &device);
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Could not find the device!");

        oc_mutex_unlock(g_edrDeviceListMutex);
        return;
    }
    oc_mutex_unlock(g_edrDeviceListMutex);

    //: TODO Need to check if 'check required for socket still connected or not'
    if (!device)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "There is no device!");
        return;
    }

    CAConnectedDeviceInfo_t *deviceInfo =
        (CAConnectedDeviceInfo_t *) CAEDRGetDeviceInfoFromAddress(device->remoteAddress);

    if (!deviceInfo)
    {
        OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Received Data from new device");
        deviceInfo = (CAConnectedDeviceInfo_t *) OICCalloc(1, sizeof(*deviceInfo));
        if (!deviceInfo)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Out of memory");
            return;
        }

        deviceInfo->state = STATE_CONNECTED;
        deviceInfo->recvData = NULL;
        deviceInfo->recvDataLen = 0;
        deviceInfo->totalDataLen = 0;
        result = CAEDRAddDeviceInfoToList(device->remoteAddress, deviceInfo);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Could not add device info to list!");
            OICFree(deviceInfo);
            return;
        }
    }

    if (!deviceInfo->recvData)
    {
        OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "Callocing deviceInfo->recvData");
        deviceInfo->recvData = OICCalloc(data->data_size, sizeof(uint8_t));
        if (!deviceInfo->recvData)
        {
            OIC_LOG(ERROR, EDR_ADAPTER_TAG, "out of memory");
            return;
        }
    }

    memcpy(deviceInfo->recvData + deviceInfo->recvDataLen, (const char*)data->data,
           data->data_size);
    deviceInfo->recvDataLen += data->data_size;

    if (!deviceInfo->totalDataLen)
    {
        coap_transport_t transport = coap_get_tcp_header_type_from_initbyte(
                ((unsigned char *)deviceInfo->recvData)[0] >> 4);
        size_t headerLen = coap_get_tcp_header_length_for_transport(transport);

        if (deviceInfo->recvDataLen >= headerLen)
        {
            // get actual data length from coap over tcp header
            deviceInfo->totalDataLen = coap_get_total_message_length(deviceInfo->recvData,
                                                                     deviceInfo->recvDataLen);
            OIC_LOG_V(DEBUG, EDR_ADAPTER_TAG, "total data length [%d] bytes", deviceInfo->totalDataLen);

            uint8_t *newBuf = OICRealloc(deviceInfo->recvData, deviceInfo->totalDataLen);
            if (!newBuf)
            {
                OIC_LOG(ERROR, EDR_ADAPTER_TAG, "out of memory");
                //Memory free
                return;
            }
            deviceInfo->recvData = newBuf;
        }
    }

    if (deviceInfo->totalDataLen == deviceInfo->recvDataLen)
    {
        if (g_edrPacketReceivedCallback)
        {
            OIC_LOG_V(DEBUG, EDR_ADAPTER_TAG,"data will be sent to callback routine: %s, %d",
                      deviceInfo->recvData, deviceInfo->recvDataLen);

            uint32_t sentLength = 0;
            g_edrPacketReceivedCallback(device->remoteAddress, (void*) deviceInfo->recvData,
                                        deviceInfo->recvDataLen, &sentLength);

            OICFree(deviceInfo->recvData);
            deviceInfo->recvData = NULL;
            deviceInfo->recvDataLen = 0;
            deviceInfo->totalDataLen = 0;
        }
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}
