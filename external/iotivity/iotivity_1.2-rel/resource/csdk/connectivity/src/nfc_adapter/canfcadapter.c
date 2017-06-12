/* *****************************************************************
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include "canfcadapter.h"

#include "canfcinterface.h"
#include "caqueueingthread.h"
#include "caadapterutils.h"
#include "octhread.h"
#include "uarraylist.h"
#include "caremotehandler.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"

/**
 * Logging tag for module name
 */
#define TAG "NFC_ADAP"

/**
 * Holds inter thread nfc data information.
 */

typedef struct
{
    CAEndpoint_t *remoteEndpoint;
    void *data;
    uint32_t dataLen;
} CANFCData;

/**
 * Reference to threadpool.
 */
static ca_thread_pool_t g_nfcThreadPool = NULL;

/**
 * Queue handle for Send Data
 */
static CAQueueingThread_t *g_sendQueueHandle = NULL;

/**
 * Network Packet Received Callback to CA
 */
static CANetworkPacketReceivedCallback g_packetReceivedCallback = NULL;

/**
 *  Adapter Changed Callback to CA
 */
static CAAdapterChangeCallback g_adapterStateCallback = NULL;

/**
 * error Callback to CA adapter
 */
static CAErrorHandleCallback g_errorCallback = NULL;

static CAResult_t CANFCPacketReceivedCB(const CASecureEndpoint_t *endpoint,
                                        const void *data, uint32_t dataLength);
#ifndef SINGLE_THREAD

static CAResult_t CANFCInitializeQueueHandles();

static void CANFCDeinitializeQueueHandles();

static void CANFCSendDataThread(void *threadData);

CANFCData *CACreateNFCData(const CAEndpoint_t *remoteEndpoint, const void *data,
                           uint32_t dataLength);

void CAFreeNFCData(CANFCData *ipData);

static void CANFCDataDestroyer(void *data, uint32_t size);

CAResult_t CANFCInitializeQueueHandles()
{
    OIC_LOG(DEBUG, TAG, "IN");

    // Check if the message queue is already initialized
    if (g_sendQueueHandle)
    {
        OIC_LOG(DEBUG, TAG, "send queue handle is already initialized!");
        return CA_STATUS_OK;
    }

    // Create send message queue
    g_sendQueueHandle = OICCalloc(1, sizeof(*g_sendQueueHandle));
    if (!g_sendQueueHandle)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return CA_MEMORY_ALLOC_FAILED;
    }

    if (CA_STATUS_OK
        != CAQueueingThreadInitialize(g_sendQueueHandle, (const ca_thread_pool_t) g_nfcThreadPool,
                                      CANFCSendDataThread, CANFCDataDestroyer))
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize send queue thread");
        OICFree(g_sendQueueHandle);
        g_sendQueueHandle = NULL;
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

void CANFCDeinitializeQueueHandles()
{
    OIC_LOG(DEBUG, TAG, "IN");

    CAQueueingThreadDestroy(g_sendQueueHandle);
    OICFree(g_sendQueueHandle);
    g_sendQueueHandle = NULL;

    OIC_LOG(DEBUG, TAG, "OUT");
}

#endif // SINGLE_THREAD
void CANFCConnectionStateCB(const char *nfcAddress, CANetworkStatus_t status)
{
    OIC_LOG(DEBUG, TAG, "Currently Not Supported");

    (void)nfcAddress;
    (void)status;
}

CAResult_t CANFCPacketReceivedCB(const CASecureEndpoint_t *sep, const void *data,
                                 uint32_t dataLength)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL(sep, TAG, "endpoint is NULL");
    VERIFY_NON_NULL(data, TAG, "data is NULL");

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, TAG, "Invalid call, data length is 0");
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG, "Address: %s, port:%d", sep->endpoint.addr, sep->endpoint.port);

    if (g_packetReceivedCallback)
    {
        g_packetReceivedCallback(sep, data, dataLength);
    }

    OIC_LOG(DEBUG, TAG, "OUT");

    return CA_STATUS_OK;
}

void CANFCErrorHandler(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength,
                       CAResult_t result)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");

    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");
    if (g_errorCallback)
    {
        g_errorCallback(endpoint, data, dataLength, result);
    }
    OIC_LOG(DEBUG, TAG, "OUT");
}

CAResult_t CAInitializeNFC(CARegisterConnectivityCallback registerCallback,
                           CANetworkPacketReceivedCallback packetReceivedCallback,
                           CAAdapterChangeCallback netCallback,
                           CAErrorHandleCallback errorCallback, ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "IN");
    VERIFY_NON_NULL(registerCallback, TAG, "registerCallback");
    VERIFY_NON_NULL(packetReceivedCallback, TAG, "packetReceivedCallback");
    VERIFY_NON_NULL(netCallback, TAG, "netCallback");
    VERIFY_NON_NULL(handle, TAG, "thread pool handle");

    g_adapterStateCallback = netCallback;
    g_packetReceivedCallback = packetReceivedCallback;
    g_errorCallback = errorCallback;

    g_nfcThreadPool = handle;

    CAResult_t result = CAInitializeNfcServer();
    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "CAInitializeNfcServer failed");
        return result;
    }

    CANFCSetPacketReceiveCallback(CANFCPacketReceivedCB);
    static const CAConnectivityHandler_t nfcHandler =
        {
            .startAdapter = CAStartNFC,
            .stopAdapter = CAStopNFC,
            .startListenServer = CAStartNFCListeningServer,
            .stopListenServer = CAStopNFCListeningServer,
            .startDiscoveryServer = CAStartNFCDiscoveryServer,
            .sendData = CASendNFCUnicastData,
            .sendDataToAll = CASendNFCMulticastData,
            .GetnetInfo = CAGetNFCInterfaceInformation,
            .readData = CAReadNFCData,
            .terminate = CATerminateNFC,
            .cType = CA_ADAPTER_NFC
        };
    registerCallback(nfcHandler);

    OIC_LOG(INFO, TAG, "OUT Intialize NFC is Success");
    return CA_STATUS_OK;
}

CAResult_t CAStartNFC()
{

    OIC_LOG(DEBUG, TAG, "IN");
    CAResult_t res = CANFCInitializeQueueHandles();

    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize Queue Handle");
        return res;
    }

    res = CAQueueingThreadStart(g_sendQueueHandle);
    // Start send queue thread
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "Failed to Start Send Data Thread");
        return res;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAStartNFCListeningServer()
{
    OIC_LOG(DEBUG, TAG, "IN");

    CAResult_t ret = CANFCStartServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start server![%d]", ret);
        return ret;
    }

    OIC_LOG(DEBUG, TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAStopNFCListeningServer()
{
    CANFCStopServer();
    return CA_STATUS_OK;
}

CAResult_t CAStartNFCDiscoveryServer()
{
    return CAStartNFCListeningServer();
}

static int32_t CAQueueNFCData(const CAEndpoint_t *endpoint, const void *data,
                              uint32_t dataLength)
{
    OIC_LOG(DEBUG, TAG, "IN");

    VERIFY_NON_NULL_RET(endpoint, TAG, "remoteEndpoint", -1);
    VERIFY_NON_NULL_RET(data, TAG, "data", -1);

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, TAG, "Invalid Data Length");
        return -1;
    }

    VERIFY_NON_NULL_RET(g_sendQueueHandle, TAG, "sendQueueHandle", -1);
    // Create NFCData to add to queue
    CANFCData *nfcData = CACreateNFCData(endpoint, data, dataLength);
    if (!nfcData)
    {
        OIC_LOG(ERROR, TAG, "Failed to create ipData!");
        return -1;
    }
    // Add message to send queue
    CAQueueingThreadAddData(g_sendQueueHandle, nfcData, sizeof(CANFCData));

    OIC_LOG(DEBUG, TAG, "OUT");
    return dataLength;

}

int32_t CASendNFCUnicastData(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength,
                             CADataType_t dataType)
{
    (void)dataType;
    return CAQueueNFCData(endpoint, data, dataLength);
}

int32_t CASendNFCMulticastData(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength,
                               CADataType_t dataType)
{
    (void)dataType;
    return CAQueueNFCData(endpoint, data, dataLength);
}

CAResult_t CAReadNFCData()
{
#if 0
    OIC_LOG(DEBUG, TAG, "IN");
    CANFCPullData();
    OIC_LOG(DEBUG, TAG, "OUT");
#endif
    return CA_STATUS_OK;
}

CAResult_t CAStopNFC()
{
    OIC_LOG(DEBUG, TAG, "IN");

    if (g_sendQueueHandle && g_sendQueueHandle->threadMutex)
    {
        CAQueueingThreadStop(g_sendQueueHandle);
    }

    CANFCDeinitializeQueueHandles();

    CANFCStopServer();

    OIC_LOG(DEBUG, TAG, "OUT");

    return CA_STATUS_OK;
}

void CATerminateNFC()
{

    OIC_LOG(DEBUG, TAG, "IN");
    CANFCSetPacketReceiveCallback(NULL);
    CANFCDeinitializeQueueHandles();

    OIC_LOG(DEBUG, TAG, "OUT");
}

void CANFCSendDataThread(void *threadData)
{
    OIC_LOG(DEBUG, TAG, "IN");
    CANFCData *nfcData = (CANFCData *) threadData;
    if (!nfcData)
    {
        OIC_LOG(ERROR, TAG, "Invalid ip data!");
        return;
    }

    CANFCSendData(nfcData->remoteEndpoint, nfcData->data, nfcData->dataLen);
    OIC_LOG(DEBUG, TAG, "OUT");
}

CANFCData *CACreateNFCData(const CAEndpoint_t *remoteEndpoint, const void *data,
                           uint32_t dataLength)
{
    VERIFY_NON_NULL_RET(data, TAG, "NFCData is NULL", NULL);

    CANFCData *nfcData = (CANFCData *) OICCalloc(1,sizeof(*nfcData));
    if (!nfcData)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return NULL;
    }

    nfcData->remoteEndpoint = CACloneEndpoint(remoteEndpoint);
    nfcData->data = (void *) OICMalloc(dataLength);
    if (!nfcData->data)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        CAFreeNFCData(nfcData);
        return NULL;
    }

    memcpy(nfcData->data, data, dataLength);
    nfcData->dataLen = dataLength;

    return nfcData;

}

void CAFreeNFCData(CANFCData *nfcData)
{
    if (nfcData)
    {
        CAFreeEndpoint(nfcData->remoteEndpoint);
        OICFree(nfcData->data);
        OICFree(nfcData);
    }
}

void CANFCDataDestroyer(void *data, uint32_t size)
{
    CANFCData *nfcData = (CANFCData *) data;

    CAFreeNFCData(nfcData);
    (void)size;
}

CAResult_t CAGetNFCInterfaceInformation(CAEndpoint_t **info, uint32_t *size)
{
    OIC_LOG(DEBUG, TAG, "CAGetNFCInterfaceInformation not supported");
    if (!info || !size)
    {
        OIC_LOG(ERROR, TAG, "CAGetNFCInterfaceInformation : Invalid Input");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}
