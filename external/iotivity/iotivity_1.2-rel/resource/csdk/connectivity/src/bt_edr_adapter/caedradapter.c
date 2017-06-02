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

/**
 * @file
 *
 * This file contains the APIs for EDR adapter.
 */

#include "caedradapter.h"

#include "caedrinterface.h"
#include "caadapterutils.h"
#include "logger.h"
#include "caqueueingthread.h"
#include "oic_malloc.h"
#include "caremotehandler.h"
#include <coap/pdu.h>

/**
 * Logging tag for module name.
 */
#define TAG "OIC_CA_EDR_ADAP"

/**
 * Reference to threadpool.
 */
static ca_thread_pool_t g_edrThreadPool = NULL;

/**
 * Queue handle for Send Data
 */
static CAQueueingThread_t *g_sendQueueHandle = NULL;

/**
 * Queue handle for Receive Data
 */
static CAQueueingThread_t *g_recvQueueHandle = NULL;

/**
 * Storing Adapter state information
 */
static bool g_adapterState = true;

/**
 * Maintains the callback to be notified on receival of network packets from other
 * Bluetooth devices.
 */
static CANetworkPacketReceivedCallback g_networkPacketReceivedCallback = NULL;

/**
 * Maintains the callback to be notified on local bluetooth adapter status change.
 */
static CAAdapterChangeCallback g_adapterChangeCallback = NULL;

/**
 * error Callback to CA adapter
 */
static CAErrorHandleCallback g_errorCallback = NULL;

/**
 * Information of local Bluetooth adapter.
 */
static CAEndpoint_t *g_localConnectivity = NULL;

/**
 * Storing Rfcommserver state information
 */
static bool g_serverState = false;

static CAResult_t CAStartServer();
static CAResult_t CAEDRInitializeQueueHandlers();
CAResult_t CAEDRInitializeSendHandler();
CAResult_t CAEDRInitializeReceiveHandler();
void CAAdapterTerminateQueues();
void CAAdapterDataSendHandler(void *context);
void CAAdapterDataReceiverHandler(void *context);
CAResult_t CAAdapterStopQueue();
CAResult_t CAAdapterRecvData(const char *remoteAddress, const uint8_t *data,
                             uint32_t dataLength, uint32_t *sentLength);
void CAEDRNotifyNetworkStatus(CANetworkStatus_t status);
void CAEDROnNetworkStatusChanged(void *context);
CAResult_t CAAdapterSendData(const char *remoteAddress, const char *serviceUUID,
                             const uint8_t *data, uint32_t dataLength, uint32_t *sentLength);
CAEDRNetworkEvent *CAEDRCreateNetworkEvent(CAEndpoint_t *connectivity, CANetworkStatus_t status);
CAResult_t CAEDRClientSendData(const char *remoteAddress, const uint8_t *data,
                               uint32_t dataLength);

/**
 * @fn CACreateEDRData
 * @brief Helper function to create CAEDRData
 */
static CAEDRData *CACreateEDRData(const CAEndpoint_t *remoteEndpoint, const uint8_t *data,
                                  uint32_t dataLength);

/**
 * @fn CAFreeEDRData
 * @brief Free the Created EDR data
 */
static void CAFreeEDRData(CAEDRData *edrData);

/**
 * @fn CAEDRFreeNetworkEvent
 * @brief Free the memory associated with @event.
 */
void CAEDRFreeNetworkEvent(CAEDRNetworkEvent *event);

static void CAEDRDataDestroyer(void *data, uint32_t size);

static void CAEDRErrorHandler(const char *remoteAddress, const uint8_t *data,
                              uint32_t dataLength, CAResult_t result);

CAResult_t CAInitializeEDR(CARegisterConnectivityCallback registerCallback,
                           CANetworkPacketReceivedCallback packetReceivedCallback,
                           CAAdapterChangeCallback netCallback,
                           CAConnectionChangeCallback connCallback,
                           CAErrorHandleCallback errorCallback, ca_thread_pool_t handle)
{
    // Input validation
    VERIFY_NON_NULL(registerCallback, TAG, "register callback is NULL");
    VERIFY_NON_NULL(packetReceivedCallback, TAG, "data receive callback is NULL");
    VERIFY_NON_NULL(netCallback, TAG, "adapter state change callback is NULL");
    VERIFY_NON_NULL(connCallback, TAG, "connection state change callback is NULL");
    VERIFY_NON_NULL(handle, TAG, "Thread pool handle is NULL");

    // Register the callbacks
    g_edrThreadPool = handle;
    g_networkPacketReceivedCallback = packetReceivedCallback;
    g_adapterChangeCallback = netCallback;
    g_errorCallback = errorCallback;

    // Initialize EDR Network Monitor
    CAResult_t res = CAEDRInitializeNetworkMonitor(handle);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "EDR N/w Monitor Initialize failed!, error number [%d]", res);
        return res;
    }

    CAEDRSetNetworkChangeCallback(CAEDRNotifyNetworkStatus);
    CAEDRSetPacketReceivedCallback(CAAdapterRecvData);
    CAEDRSetErrorHandler(CAEDRErrorHandler);
    res = CAEDRClientInitialize();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "EDR Client Initialize failed, error number [%d]", res);
        return res;
    }

    res = CAEDRServerInitialize(handle);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "EDR Server Initialize failed, error number [%d]", res);
        return res;
    }

    static const CAConnectivityHandler_t handler =
        {
            .startAdapter = CAStartEDR,
            .stopAdapter = CAStopEDR,
            .startListenServer = CAStartEDRListeningServer,
            .stopListenServer = CAStopEDRListeningServer,
            .startDiscoveryServer = CAStartEDRDiscoveryServer,
            .sendData = CASendEDRUnicastData,
            .sendDataToAll = CASendEDRMulticastData,
            .GetnetInfo = CAGetEDRInterfaceInformation,
            .readData = CAReadEDRData,
            .terminate = CATerminateEDR,
            .cType = CA_ADAPTER_RFCOMM_BTEDR
        };
    registerCallback(handler);

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAStartEDR()
{
    //Start Monitoring EDR Network
    CAResult_t ret = CAEDRStartNetworkMonitor();
    if (CA_STATUS_OK != ret)
    {
       OIC_LOG(ERROR, TAG, "Failed to Start n/w monitor");
    }

    // Get Bluetooth adapter state
    bool adapterState = false;
    if (CA_STATUS_OK != CAEDRGetAdapterEnableState(&adapterState))
    {
        OIC_LOG(ERROR, TAG, "Failed to get adapter enable state");
        return CA_STATUS_FAILED;
    }

    if (false == adapterState)
    {
        OIC_LOG(ERROR, TAG, "Bluetooth adapter is disabled!");
        g_adapterState = false;
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (CA_STATUS_OK != (ret = CAEDRClientSetCallbacks()))
    {
        OIC_LOG_V(ERROR, TAG, "Start Network Monitor failed!, error number [%d] ",
                  ret);
    }

    // Initialize Send/Receive data message queues
    if (CA_STATUS_OK != (ret = CAEDRInitializeQueueHandlers()))
    {
        OIC_LOG_V(ERROR, TAG,
                  "CAAdapterInitializeQueues failed!, error number [%d] ", ret);
        CATerminateEDR();
        return CA_STATUS_FAILED;
    }

    // Start Send/Receive data message queues
    if (CA_STATUS_OK != (ret = CAAdapterStartQueue()))
    {
        OIC_LOG_V(ERROR, TAG, "CAAdapterStartQueue failed!, error number [%d] ", ret);
    }

    return ret;
}

CAResult_t CAStartEDRListeningServer()
{
    return CAStartServer();
}

CAResult_t CAStopEDRListeningServer()
{
    return CAEDRServerStop();
}

CAResult_t CAStartEDRDiscoveryServer()
{
#ifdef __TIZEN__
    // Start device discovery
    CAResult_t result = CAEDRStartDeviceDiscovery();
    if(CA_STATUS_OK != result)
    {
        OIC_LOG(DEBUG, TAG, "Failed to Start Device discovery");
    }
#endif

    return CAStartServer();
}

int32_t CASendEDRUnicastData(const CAEndpoint_t *remoteEndpoint, const void *data,
                             uint32_t dataLength, CADataType_t dataType)
{
    // Input validation
    VERIFY_NON_NULL_RET(remoteEndpoint, TAG, "Remote endpoint is null", -1);
    VERIFY_NON_NULL_RET(data, TAG, "Data is null", -1);
    (void)dataType;

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, TAG, "Invalid input: data length is zero!");
        return -1;
    }

    if (0 == strlen(remoteEndpoint->addr))
    {
        OIC_LOG(ERROR, TAG, "Invalid input: EDR Address is empty!");
        return -1;
    }

    uint32_t sentLength = 0;
    const char *serviceUUID = OIC_EDR_SERVICE_ID;
    const char *address = remoteEndpoint->addr;
    CAResult_t err = CAAdapterSendData(address, serviceUUID, data, dataLength, &sentLength);
    if (CA_STATUS_OK != err)
    {
        OIC_LOG_V(ERROR, TAG, "Send unicast data failed!, error num [%d]", err);
        g_errorCallback(remoteEndpoint, data, dataLength, err);
        return -1;
    }

    return sentLength;
}

int32_t CASendEDRMulticastData(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength,
                               CADataType_t dataType)
{
    OIC_LOG(DEBUG, TAG, "IN - CASendEDRMulticastData");
    (void)dataType;

    // Input validation
    VERIFY_NON_NULL_RET(data, TAG, "Data is null", -1);

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, TAG, "Invalid input: data length is zero!");
        return -1;
    }

    uint32_t sentLen = 0;
    const char *serviceUUID = OIC_EDR_SERVICE_ID;
    CAResult_t err = CAAdapterSendData(NULL, serviceUUID, data, dataLength, &sentLen);
    if (CA_STATUS_OK != err)
    {
        OIC_LOG_V(ERROR, TAG, "Send multicast data failed!, error num [%d]", err);
        g_errorCallback(endpoint, data, dataLength, err);
        return -1;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CASendEDRMulticastData");
    return sentLen;
}

CAResult_t CAGetEDRInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
    VERIFY_NON_NULL(info, TAG, "LocalConnectivity info is null");

    CAResult_t err = CA_STATUS_OK;
    *size = 0;
    if (CA_STATUS_OK != (err = CAEDRGetInterfaceInformation(info)))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to get local interface information!, error num [%d]", err);
        return err;
    }

    *size = 1;
    return err;
}

CAResult_t CAReadEDRData()
{
    return CAEDRManagerReadData();
}

CAResult_t CAStopEDR()
{
    // Stop RFComm server if it is running
    CAEDRServerStop();

    // Stop the adapter
    CAEDRClientUnsetCallbacks();

    // Disconnect all the client connections
    CAEDRClientDisconnectAll();

    // Stop network monitor
    CAEDRStopNetworkMonitor();

    // Stop Send and receive Queue
    CAAdapterStopQueue();

    return CA_STATUS_OK;
}

void CATerminateEDR()
{
    // Terminate EDR Network Monitor
    CAEDRTerminateNetworkMonitor();

    // Terminate Send/Receive data messages queues
    CAAdapterTerminateQueues();

    g_networkPacketReceivedCallback = NULL;
    g_adapterChangeCallback = NULL;

    // Terminate thread pool
    g_edrThreadPool = NULL;

    // Terminate EDR Client
    CAEDRClientTerminate();

    // Terminate EDR Server
    CAEDRServerTerminate();

    // Free LocalConnectivity information
    CAFreeEndpoint(g_localConnectivity);
    g_localConnectivity = NULL;
}

CAResult_t CAStartServer()
{
    if (false == g_adapterState)
    {
        OIC_LOG(DEBUG, TAG, "Bluetooth adapter is disabled!");
        // Setting g_serverState for starting Rfcommserver when adapter starts
        g_serverState = true;
        return CA_STATUS_OK;
    }

    CAResult_t err = CA_STATUS_OK;
    if (CA_STATUS_OK != (err = CAEDRServerStart()))
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start RFCOMM server!, error num [%d]",
                  err);
        return err;
    }

    return err;
}

CAResult_t CAEDRInitializeQueueHandlers()
{
    if (CA_STATUS_OK == CAEDRInitializeSendHandler()
        && CA_STATUS_OK == CAEDRInitializeReceiveHandler())
    {
        OIC_LOG(DEBUG, TAG, "Queue is initialized!");
        return CA_STATUS_OK;
    }

    return CA_STATUS_FAILED;
}

CAResult_t CAEDRInitializeSendHandler()
{
    // Check if the message queue is already initialized
    if (g_sendQueueHandle)
    {
        OIC_LOG(DEBUG, TAG, "Already queue is initialized!");
        return CA_STATUS_OK;
    }

    g_sendQueueHandle = (CAQueueingThread_t *) OICMalloc(sizeof(CAQueueingThread_t));
    if (!g_sendQueueHandle)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return CA_MEMORY_ALLOC_FAILED;
    }

    if (CA_STATUS_OK != CAQueueingThreadInitialize(g_sendQueueHandle, g_edrThreadPool,
                                                   CAAdapterDataSendHandler, CAEDRDataDestroyer))
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize send queue thread");
        return CA_STATUS_FAILED;
    }
    return CA_STATUS_OK;
}

CAResult_t CAEDRInitializeReceiveHandler()
{
    // Check if the message queue is already initialized
    if (g_recvQueueHandle)
    {
        OIC_LOG(DEBUG, TAG, "Already queue is initialized!");
        return CA_STATUS_OK;
    }

    g_recvQueueHandle = (CAQueueingThread_t *) OICMalloc(sizeof(CAQueueingThread_t));
    if (!g_recvQueueHandle)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return CA_MEMORY_ALLOC_FAILED;
    }


    if (CA_STATUS_OK != CAQueueingThreadInitialize(g_recvQueueHandle, g_edrThreadPool,
                                                   CAAdapterDataReceiverHandler,
                                                   CAEDRDataDestroyer))
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize send queue thread");
        OICFree(g_recvQueueHandle);
        g_recvQueueHandle = NULL;
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

void CAAdapterTerminateQueues()
{
    if (g_sendQueueHandle)
    {
        CAQueueingThreadDestroy(g_sendQueueHandle);
        g_sendQueueHandle = NULL;
    }
    if (g_recvQueueHandle)
    {
        CAQueueingThreadDestroy(g_recvQueueHandle);
        g_recvQueueHandle = NULL;
    }
}

void CAAdapterDataSendHandler(void *context)
{
    OIC_LOG(DEBUG, TAG, "IN - CAAdapterDataSendHandler");

    CAEDRData *message = (CAEDRData *) context;
    if (!message)
    {
        OIC_LOG(ERROR, TAG, "Failed to get message!");
        return;
    }

    if (!message->remoteEndpoint)
    {
        OIC_LOG(DEBUG, TAG, "remoteEndpoint is not available");
        return;
    }

    const char *remoteAddress = message->remoteEndpoint->addr;
    if(!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "EDR Send Message error");
        //Error cannot be sent if remote address is NULL
        return;
    }

    CAResult_t result = CAEDRClientSendData(remoteAddress, message->data, message->dataLen);
    if(CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CAEDRClientSendData API failed");
        CAEDRErrorHandler(remoteAddress, message->data, message->dataLen, result);
        return;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAEDRClientSendData(const char *remoteAddress, const uint8_t *data,
                               uint32_t dataLength)
{
    CAResult_t result = CA_SEND_FAILED;

    // Send the first segment with the header.
    if ((NULL != remoteAddress) && (0 < strlen(remoteAddress))) //Unicast data
    {
        result = CAEDRClientSendUnicastData(remoteAddress, data, dataLength);
        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, TAG, "Failed to send unicast data !");
            return result;
        }
    }
    else
    {
        OIC_LOG_V(DEBUG, TAG, "sending multicast data : %s", data);
        result = CAEDRClientSendMulticastData(data, dataLength);

        if (CA_STATUS_OK != result)
        {
            OIC_LOG(ERROR, TAG, "Failed to send multicast data !");
            return result;
        }
    }
    return result;
}

void CAAdapterDataReceiverHandler(void *context)
{
    OIC_LOG(DEBUG, TAG, "IN_CAAdapterDataReceiverHandler");

    if (NULL == g_networkPacketReceivedCallback)
    {
        OIC_LOG(ERROR, TAG, "g_networkPacketReceivedCallback is NULL");
        return;
    }

    CAEDRData *message = (CAEDRData *) context;
    if (NULL == message || NULL == message->remoteEndpoint)
    {
        OIC_LOG(ERROR, TAG, "Failed to get message!");
        return;
    }

    CAEndpoint_t *remoteEndpoint = CACreateEndpointObject(CA_DEFAULT_FLAGS,
                                                          CA_ADAPTER_RFCOMM_BTEDR,
                                                          message->remoteEndpoint->addr,
                                                          0);

    if (!remoteEndpoint)
    {
        OIC_LOG(ERROR, TAG, "remoteEndpoint is NULL");
        return;
    }

    OIC_LOG(DEBUG, TAG, "Sending data up !");

    const CASecureEndpoint_t sep = { .endpoint = *remoteEndpoint };

    g_networkPacketReceivedCallback(&sep, message->data, message->dataLen);

    CAFreeEndpoint(remoteEndpoint);

    OIC_LOG(DEBUG, TAG, "OUT_CAAdapterDataReceiverHandler");
}

CAResult_t CAAdapterStartQueue()
{
    // Start send queue thread
    if (CA_STATUS_OK != CAQueueingThreadStart(g_sendQueueHandle))
    {
        OIC_LOG(ERROR, TAG, "Failed to Start Send Data Thread");
        CAEDRClientUnsetCallbacks();
        //Disconnect all the client connections
        CAEDRClientDisconnectAll();
        return CA_STATUS_FAILED;
    }

    // Start receive queue thread
    if (CA_STATUS_OK != CAQueueingThreadStart(g_recvQueueHandle))
    {
        OIC_LOG(ERROR, TAG, "Failed to Start Receive Data Thread");
        CAEDRClientUnsetCallbacks();
        //Disconnect all the client connections
        CAEDRClientDisconnectAll();
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

CAResult_t CAAdapterStopQueue()
{
    //Stop send queue thread
    CAQueueingThreadStop(g_sendQueueHandle);

    //Stop receive queue thread
    CAQueueingThreadStop(g_recvQueueHandle);

    return CA_STATUS_OK;
}

CAResult_t CAAdapterRecvData(const char *remoteAddress, const uint8_t *data,
                             uint32_t dataLength, uint32_t *sentLength)
{
    if (false == g_adapterState)
    {
        OIC_LOG_V(ERROR, TAG, "Bluetooth adapter is disabled!");
        *sentLength = 0;
        return CA_ADAPTER_NOT_ENABLED;
    }

    // Input validation
    VERIFY_NON_NULL(data, TAG, "Data is null");
    VERIFY_NON_NULL(sentLength, TAG, "Sent data length holder is null");

    // Create remote endpoint
    CAEndpoint_t *remoteEndpoint = CACreateEndpointObject(CA_DEFAULT_FLAGS,
                                                          CA_ADAPTER_RFCOMM_BTEDR,
                                                          remoteAddress, 0);
    if (NULL == remoteEndpoint)
    {
        OIC_LOG(ERROR, TAG, "Failed to create remote endpoint !");
        return CA_STATUS_FAILED;
    }

    // Add message to data queue
    CAEDRData *edrData =  CACreateEDRData(remoteEndpoint, data, dataLength);
    CAQueueingThreadAddData(g_recvQueueHandle, edrData, sizeof(CAEDRData));
    *sentLength = dataLength;

    // Free remote endpoint
    CAFreeEndpoint(remoteEndpoint);

    return CA_STATUS_OK;
}

void CAEDRErrorHandler(const char *remoteAddress, const uint8_t *data,
                       uint32_t dataLength, CAResult_t result)
{
    // Input validation
    VERIFY_NON_NULL_VOID(data, TAG, "Data is null");

    // Create remote endpoint
    CAEndpoint_t *remoteEndpoint = CACreateEndpointObject(0, CA_ADAPTER_RFCOMM_BTEDR,
                                                           remoteAddress, 0);
    if (!remoteEndpoint)
    {
        OIC_LOG(ERROR, TAG, "Failed to create remote endpoint !");
        return;
    }

    g_errorCallback(remoteEndpoint, data, dataLength, result);

    // Free remote endpoint
    CAFreeEndpoint(remoteEndpoint);
}

CAResult_t CAAdapterSendData(const char *remoteAddress, const char *serviceUUID, const uint8_t *data,
                             uint32_t dataLength, uint32_t *sentLength)
{
    OIC_LOG(DEBUG, TAG, "IN - CAAdapterSendData");

    if (false == g_adapterState)
    {
        OIC_LOG(ERROR, TAG, "Bluetooth adapter is disabled!");
        *sentLength = 0;
        return CA_ADAPTER_NOT_ENABLED;
    }
    // Input validation
    VERIFY_NON_NULL(serviceUUID, TAG, "service UUID is null");
    VERIFY_NON_NULL(data, TAG, "Data is null");
    VERIFY_NON_NULL(sentLength, TAG, "Sent data length holder is null");

    // Create remote endpoint
    CAEndpoint_t *remoteEndpoint = CACreateEndpointObject(CA_DEFAULT_FLAGS,
                                                          CA_ADAPTER_RFCOMM_BTEDR,
                                                          remoteAddress, 0);
    if (NULL == remoteEndpoint)
    {
        OIC_LOG(ERROR, TAG, "Failed to create remote endpoint !");
        return CA_MEMORY_ALLOC_FAILED;
    }

    // Add message to data queue
    CAEDRData *edrData =  CACreateEDRData(remoteEndpoint, data, dataLength);
    CAQueueingThreadAddData(g_sendQueueHandle, edrData, sizeof (CAEDRData));
    *sentLength = dataLength;

    // Free remote endpoint
    CAFreeEndpoint(remoteEndpoint);

    OIC_LOG(DEBUG, TAG, "OUT - CAAdapterSendData");
    return CA_STATUS_OK;
}

void CAEDRNotifyNetworkStatus(CANetworkStatus_t status)
{
    // Create localconnectivity
    if (NULL == g_localConnectivity)
    {
        CAEDRGetInterfaceInformation(&g_localConnectivity);
    }

    if (CA_INTERFACE_UP == status)
    {
        if (false == g_adapterState)
        {
            // Get Bluetooth adapter state
            bool adapterState = false;
            if (CA_STATUS_OK != CAEDRGetAdapterEnableState(&adapterState))
            {
                OIC_LOG(ERROR, TAG, "Failed to get adapter enable state");
                return;
            }

            if (false == adapterState)
            {
                OIC_LOG(ERROR, TAG, "Bluetooth adapter is disabled!");
                g_adapterState = false;
                return;
            }
            CAEDRClientSetCallbacks();
            g_adapterState = true;
            CAAdapterStartQueue();
            // starting RFCommServer
            if (true == g_serverState)
            {
                CAStartServer();
                g_serverState = false;
            }
        }
    }
    else
    {
        g_adapterState = false;
    }

    // Notify to upper layer
    if (g_adapterChangeCallback && g_localConnectivity && g_edrThreadPool)
    {
        // Add notification task to thread pool
        CAEDRNetworkEvent *event = CAEDRCreateNetworkEvent(g_localConnectivity, status);
        if (NULL != event)
        {
            if (CA_STATUS_OK != ca_thread_pool_add_task(g_edrThreadPool,
                                                        CAEDROnNetworkStatusChanged, event, NULL))
            {
                OIC_LOG(ERROR, TAG, "Failed to create threadpool!");
                return;
            }
        }
    }
}

void CAEDROnNetworkStatusChanged(void *context)
{
    if (NULL == context)
    {
        OIC_LOG(ERROR, TAG, "context is NULL!");
        return;
    }

    CAEDRNetworkEvent *networkEvent = (CAEDRNetworkEvent *) context;

    // Notify to upper layer
    if (g_adapterChangeCallback)
    {
        g_adapterChangeCallback(networkEvent->info->adapter, networkEvent->status);
    }

    // Free the created Network event
    CAEDRFreeNetworkEvent(networkEvent);
}

CAEDRNetworkEvent *CAEDRCreateNetworkEvent(CAEndpoint_t *connectivity,
                                           CANetworkStatus_t status)
{
    VERIFY_NON_NULL_RET(connectivity, TAG, "connectivity is NULL", NULL);

    // Create CAEDRNetworkEvent
    CAEDRNetworkEvent *event = (CAEDRNetworkEvent *) OICMalloc(sizeof(CAEDRNetworkEvent));
    if (NULL == event)
    {
        OIC_LOG(ERROR, TAG, "Failed to allocate memory to network event!");
        return NULL;
    }

    // Create duplicate of Local connectivity
    event->info = CACloneEndpoint(connectivity);
    event->status = status;
    return event;
}

void CAEDRFreeNetworkEvent(CAEDRNetworkEvent *event)
{
    if (event)
    {
        CAFreeEndpoint(event->info);
        OICFree(event);
    }
}

CAEDRData *CACreateEDRData(const CAEndpoint_t *remoteEndpoint,
                           const uint8_t *data, uint32_t dataLength)
{
    CAEDRData *edrData = (CAEDRData *) OICCalloc(1, sizeof(*edrData));
    if (!edrData)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return NULL;
    }

    edrData->remoteEndpoint = CACloneEndpoint(remoteEndpoint);

    edrData->data = OICMalloc(dataLength);
    if (NULL == edrData->data)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        CAFreeEDRData(edrData);
        return NULL;
    }
    memcpy(edrData->data, data, dataLength);
    edrData->dataLen = dataLength;

    return edrData;
}

void CAFreeEDRData(CAEDRData *edrData)
{
    VERIFY_NON_NULL_VOID(edrData, TAG, "edrData is NULL");

    CAFreeEndpoint(edrData->remoteEndpoint);
    OICFree(edrData->data);
    OICFree(edrData);
}

void CAEDRDataDestroyer(void *data, uint32_t size)
{
    if ((size_t)size < sizeof(CAEDRData))
    {
        OIC_LOG_V(ERROR, TAG, "Destroy data too small %p %d", data, size);
    }
    CAEDRData *edrdata = (CAEDRData *) data;

    CAFreeEDRData(edrdata);
}
