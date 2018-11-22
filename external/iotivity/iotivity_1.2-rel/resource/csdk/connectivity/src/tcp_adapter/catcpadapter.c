/* ****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include <stdint.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include "cainterface.h"
#include "caipnwmonitor.h"
#include "catcpadapter.h"
#include "catcpinterface.h"
#include "caqueueingthread.h"
#include "caadapterutils.h"
#include "octhread.h"
#include "uarraylist.h"
#include "caremotehandler.h"
#include "logger.h"
#include "oic_malloc.h"
#ifdef __WITH_TLS__
#include "ca_adapter_net_ssl.h"
#endif

/**
 * Logging tag for module name.
 */
#define TAG "OIC_CA_TCP_ADAP"

/**
 * Holds internal thread TCP data information.
 */
typedef struct
{
    CAEndpoint_t *remoteEndpoint;
    void *data;
    size_t dataLen;
    bool isMulticast;
} CATCPData;

#define CA_TCP_LISTEN_BACKLOG  3

#define CA_TCP_SELECT_TIMEOUT 10

/**
 * Queue handle for Send Data.
 */
static CAQueueingThread_t *g_sendQueueHandle = NULL;

/**
 * Network Packet Received Callback to CA.
 */
static CANetworkPacketReceivedCallback g_networkPacketCallback = NULL;

/**
 * Adapter Changed Callback to CA.
 */
static CAAdapterChangeCallback g_networkChangeCallback = NULL;

/**
 * Connection Changed Callback to CA.
 */
static CAConnectionChangeCallback g_connectionChangeCallback = NULL;

/**
 * error Callback to CA adapter.
 */
static CAErrorHandleCallback g_errorCallback = NULL;

/**
 * KeepAlive Connected or Disconnected Callback to CA adapter.
 */
static CAKeepAliveConnectionCallback g_connKeepAliveCallback = NULL;

static CAResult_t CATCPPacketReceivedCB(const CASecureEndpoint_t *sep,
                                        const void *data, uint32_t dataLength);

static void CATCPErrorHandler(const CAEndpoint_t *endpoint, const void *data,
                              size_t dataLength, CAResult_t result);

static CAResult_t CATCPInitializeQueueHandles();

static void CATCPDeinitializeQueueHandles();

static void CATCPSendDataThread(void *threadData);

static CATCPData *CACreateTCPData(const CAEndpoint_t *remoteEndpoint,
                                  const void *data, size_t dataLength,
                                  bool isMulticast);
void CAFreeTCPData(CATCPData *ipData);

static void CADataDestroyer(void *data, uint32_t size);

CAResult_t CATCPInitializeQueueHandles()
{
    // Check if the message queue is already initialized
    if (g_sendQueueHandle)
    {
        OIC_LOG(DEBUG, TAG, "send queue handle is already initialized!");
        return CA_STATUS_OK;
    }

    // Create send message queue
    g_sendQueueHandle = OICMalloc(sizeof(CAQueueingThread_t));
    if (!g_sendQueueHandle)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return CA_MEMORY_ALLOC_FAILED;
    }

    if (CA_STATUS_OK != CAQueueingThreadInitialize(g_sendQueueHandle,
                                (const ca_thread_pool_t)caglobals.tcp.threadpool,
                                CATCPSendDataThread, CADataDestroyer))
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize send queue thread");
        OICFree(g_sendQueueHandle);
        g_sendQueueHandle = NULL;
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

void CATCPDeinitializeQueueHandles()
{
    CAQueueingThreadDestroy(g_sendQueueHandle);
    OICFree(g_sendQueueHandle);
    g_sendQueueHandle = NULL;
}

void CATCPConnectionStateCB(const char *ipAddress, CANetworkStatus_t status)
{
    (void)ipAddress;
    (void)status;
}

CAResult_t CATCPPacketReceivedCB(const CASecureEndpoint_t *sep, const void *data,
                                 uint32_t dataLength)
{
    VERIFY_NON_NULL(sep, TAG, "sep is NULL");
    VERIFY_NON_NULL(data, TAG, "data is NULL");

    OIC_LOG_V(DEBUG, TAG, "Address: %s, port:%d", sep->endpoint.addr, sep->endpoint.port);

    CAResult_t res = CA_STATUS_OK;
#ifdef SINGLE_THREAD
    if (g_networkPacketCallback)
    {
        res = g_networkPacketCallback(sep, data, dataLength);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "Error parsing CoAP data");
        }
    }
#else
    unsigned char *buffer = (unsigned char*)data;
    size_t bufferLen = dataLength;
    size_t index = 0;

    //get remote device information from file descriptor.
    CATCPSessionInfo_t *svritem = CAGetTCPSessionInfoFromEndpoint(&sep->endpoint, &index);
    if (!svritem)
    {
        OIC_LOG(ERROR, TAG, "there is no connection information in list");
        return CA_STATUS_INVALID_PARAM;
    }
    if (UNKNOWN == svritem->protocol)
    {
        OIC_LOG(ERROR, TAG, "invalid protocol type");
        return CA_STATUS_INVALID_PARAM;
    }

    //totalLen filled only when header fully read and parsed
    while (0 != bufferLen)
    {
        res = CAConstructCoAP(svritem, &buffer, &bufferLen);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "CAConstructCoAP return error : %d", res);
            return res;
        }

        //when successfully read all required data - pass them to upper layer.
        if (svritem->len == svritem->totalLen)
        {
            if (g_networkPacketCallback)
            {
                res = g_networkPacketCallback(sep, svritem->data, svritem->totalLen);
                if (CA_STATUS_OK != res)
                {
                    OIC_LOG(ERROR, TAG, "Error parsing CoAP data");
                    return res;
                }
            }
            CACleanData(svritem);
        }
        else
        {
            OIC_LOG_V(DEBUG, TAG, "%u bytes required for complete CoAP",
                                svritem->totalLen - svritem->len);
        }
    }
#endif
    return res;
}

#ifdef __WITH_TLS__
static ssize_t CATCPPacketSendCB(CAEndpoint_t *endpoint, const void *data, size_t dataLength)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    VERIFY_NON_NULL_RET(endpoint, TAG, "endpoint is NULL", -1);
    VERIFY_NON_NULL_RET(data, TAG, "data is NULL", -1);

    OIC_LOG_V(DEBUG, TAG, "Address: %s, port:%d", endpoint->addr, endpoint->port);
    OIC_LOG_BUFFER(DEBUG, TAG, data, dataLength);

    ssize_t ret = CATCPSendData(endpoint, data, dataLength);
    OIC_LOG_V(DEBUG, TAG, "Out %s : %d bytes sent", __func__, ret);
    return ret;
}
#endif

static void CATCPErrorHandler(const CAEndpoint_t *endpoint, const void *data,
                              size_t dataLength, CAResult_t result)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

    if (g_errorCallback)
    {
        g_errorCallback(endpoint, data, dataLength, result);
    }
}

static void CATCPConnectionHandler(const CAEndpoint_t *endpoint, bool isConnected, bool isClient)
{
    // Pass the changed connection status to RI Layer for keepalive.
    if (g_connKeepAliveCallback)
    {
        g_connKeepAliveCallback(endpoint, isConnected, isClient);
    }

    // Pass the changed connection status to CAUtil.
    if (g_connectionChangeCallback)
    {
        g_connectionChangeCallback(endpoint, isConnected);
    }
}

void CATCPSetKeepAliveCallbacks(CAKeepAliveConnectionCallback ConnHandler)
{
    g_connKeepAliveCallback = ConnHandler;
}

void CATCPAdapterHandler(CATransportAdapter_t adapter, CANetworkStatus_t status)
{
    if (g_networkChangeCallback)
    {
        g_networkChangeCallback(adapter, status);
    }

    if (CA_INTERFACE_DOWN == status)
    {
        OIC_LOG(INFO, TAG, "Network status is down, close all session");
        CATCPStopServer();
    }
    else if (CA_INTERFACE_UP == status)
    {
        OIC_LOG(INFO, TAG, "Network status is up, create new socket for listening");

        CAResult_t ret = CA_STATUS_FAILED;
#ifndef SINGLE_THREAD
        ret = CATCPStartServer((const ca_thread_pool_t)caglobals.tcp.threadpool);
#else
        ret = CATCPStartServer();
#endif
        if (CA_STATUS_OK != ret)
        {
            OIC_LOG_V(DEBUG, TAG, "CATCPStartServer failed[%d]", ret);
        }
    }
}

static void CAInitializeTCPGlobals()
{
    caglobals.tcp.ipv4.fd = -1;
    caglobals.tcp.ipv4s.fd = -1;
    caglobals.tcp.ipv6.fd = -1;
    caglobals.tcp.ipv6s.fd = -1;

    // Set the port number received from application.
    caglobals.tcp.ipv4.port = caglobals.ports.tcp.u4;
    caglobals.tcp.ipv4s.port = caglobals.ports.tcp.u4s;
    caglobals.tcp.ipv6.port = caglobals.ports.tcp.u6;
    caglobals.tcp.ipv6s.port = caglobals.ports.tcp.u6s;

    caglobals.tcp.selectTimeout = CA_TCP_SELECT_TIMEOUT;
    caglobals.tcp.listenBacklog = CA_TCP_LISTEN_BACKLOG;
    caglobals.tcp.svrlist = NULL;

    CATransportFlags_t flags = 0;
    if (caglobals.client)
    {
        flags |= caglobals.clientFlags;
    }

#ifndef DISABLE_TCP_SERVER
    if (caglobals.server)
    {
        flags |= caglobals.serverFlags;
    }
#endif

    caglobals.tcp.ipv4tcpenabled = flags & CA_IPV4;
    caglobals.tcp.ipv6tcpenabled = flags & CA_IPV6;
}

CAResult_t CAInitializeTCP(CARegisterConnectivityCallback registerCallback,
                           CANetworkPacketReceivedCallback networkPacketCallback,
                           CAAdapterChangeCallback netCallback,
                           CAConnectionChangeCallback connCallback,
                           CAErrorHandleCallback errorCallback, ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "IN");
    VERIFY_NON_NULL(registerCallback, TAG, "registerCallback");
    VERIFY_NON_NULL(networkPacketCallback, TAG, "networkPacketCallback");
    VERIFY_NON_NULL(netCallback, TAG, "netCallback");
#ifndef SINGLE_THREAD
    VERIFY_NON_NULL(handle, TAG, "thread pool handle");
#endif

    g_networkChangeCallback = netCallback;
    g_connectionChangeCallback = connCallback;
    g_networkPacketCallback = networkPacketCallback;
    g_errorCallback = errorCallback;

    CAInitializeTCPGlobals();
#ifndef SINGLE_THREAD
    caglobals.tcp.threadpool = handle;
#endif

    CATCPSetConnectionChangedCallback(CATCPConnectionHandler);
    CATCPSetPacketReceiveCallback(CATCPPacketReceivedCB);
    CATCPSetErrorHandler(CATCPErrorHandler);

#ifdef __WITH_TLS__
    CAsetSslAdapterCallbacks(CATCPPacketReceivedCB, CATCPPacketSendCB, CA_ADAPTER_TCP);
#endif

    CAConnectivityHandler_t tcpHandler = {
        .startAdapter = CAStartTCP,
        .startListenServer = CAStartTCPListeningServer,
        .stopListenServer = CAStopTCPListeningServer,
        .startDiscoveryServer = CAStartTCPDiscoveryServer,
        .sendData = CASendTCPUnicastData,
        .sendDataToAll = CASendTCPMulticastData,
        .GetnetInfo = CAGetTCPInterfaceInformation,
        .readData = CAReadTCPData,
        .stopAdapter = CAStopTCP,
        .terminate = CATerminateTCP,
        .cType = CA_ADAPTER_TCP};

    registerCallback(tcpHandler);

    OIC_LOG(INFO, TAG, "OUT IntializeTCP is Success");
    return CA_STATUS_OK;
}

CAResult_t CAStartTCP()
{
    OIC_LOG(DEBUG, TAG, "IN");

    // Start network monitoring to receive adapter status changes.
    CAIPStartNetworkMonitor(CATCPAdapterHandler, CA_ADAPTER_TCP);

#ifndef SINGLE_THREAD
    if (CA_STATUS_OK != CATCPInitializeQueueHandles())
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize Queue Handle");
        CATerminateTCP();
        return CA_STATUS_FAILED;
    }

    // Start send queue thread
#ifndef __TIZENRT__
    if (CA_STATUS_OK != CAQueueingThreadStart(g_sendQueueHandle))
#else
    if (CA_STATUS_OK != CAQueueingThreadStart(g_sendQueueHandle, "IoT_TCPSendQueue"))
#endif
    {
        OIC_LOG(ERROR, TAG, "Failed to Start Send Data Thread");
        return CA_STATUS_FAILED;
    }
#else
    CAResult_t ret = CATCPStartServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(DEBUG, TAG, "CATCPStartServer failed[%d]", ret);
        return ret;
    }
#endif

    return CA_STATUS_OK;
}

CAResult_t CATCPDisconnectSession(const CAEndpoint_t *endpoint)
{
    CAResult_t res = CA_STATUS_OK;
#ifdef __WITH_TLS__
    res = CAcloseSslConnection(endpoint);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "failed to close TLS session");
        return res;
    }
#endif
    res = CASearchAndDeleteTCPSession(endpoint);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "failed to close TCP session");
    }
    return res;
}

CAResult_t CAStartTCPListeningServer()
{
#if !defined(SINGLE_THREAD) && !defined(DISABLE_TCP_SERVER)
    if (!caglobals.server)
    {
        caglobals.server = true;    // only needed to run CA tests
    }

    CAResult_t ret = CATCPStartServer((const ca_thread_pool_t)caglobals.tcp.threadpool);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start listening server![%d]", ret);
        return ret;
    }
#endif

    return CA_STATUS_OK;
}

CAResult_t CAStopTCPListeningServer()
{
    return CA_STATUS_OK;
}

CAResult_t CAStartTCPDiscoveryServer()
{
    if (!caglobals.client)
    {
        caglobals.client = true;    // only needed to run CA tests
    }

    CAResult_t ret = CATCPStartServer((const ca_thread_pool_t)caglobals.tcp.threadpool);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start discovery server![%d]", ret);
        return ret;
    }

    return CA_STATUS_OK;
}

static size_t CAQueueTCPData(bool isMulticast, const CAEndpoint_t *endpoint,
                             const void *data, size_t dataLength)
{
    VERIFY_NON_NULL_RET(endpoint, TAG, "endpoint", -1);
    VERIFY_NON_NULL_RET(data, TAG, "data", -1);

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, TAG, "Invalid Data Length");
        return -1;
    }

    VERIFY_NON_NULL_RET(g_sendQueueHandle, TAG, "sendQueueHandle", -1);

    // Create TCPData to add to queue
    CATCPData *tcpData = CACreateTCPData(endpoint, data, dataLength, isMulticast);
    if (!tcpData)
    {
        OIC_LOG(ERROR, TAG, "Failed to create ipData!");
        return -1;
    }
    // Add message to send queue
    CAQueueingThreadAddData(g_sendQueueHandle, tcpData, sizeof(CATCPData));

    return dataLength;
}

int32_t CASendTCPUnicastData(const CAEndpoint_t *endpoint,
                             const void *data, uint32_t dataLength,
                             CADataType_t dataType)
{
    OIC_LOG(DEBUG, TAG, "IN");
    (void)dataType;
#ifndef SINGLE_THREAD
    return CAQueueTCPData(false, endpoint, data, dataLength);
#else
    return CATCPSendData(endpoint, data, dataLength);
#endif
}

int32_t CASendTCPMulticastData(const CAEndpoint_t *endpoint,
                               const void *data, uint32_t dataLength,
                               CADataType_t dataType)
{
    (void)dataType;
    return CAQueueTCPData(true, endpoint, data, dataLength);
}

CAResult_t CAReadTCPData()
{
    OIC_LOG(DEBUG, TAG, "IN");
#ifdef SINGLE_THREAD
    CATCPPullData();
#endif
    return CA_STATUS_OK;
}

CAResult_t CAStopTCP()
{
    CAIPStopNetworkMonitor(CA_ADAPTER_TCP);

#ifndef SINGLE_THREAD
    if (g_sendQueueHandle && g_sendQueueHandle->threadMutex)
    {
        CAQueueingThreadStop(g_sendQueueHandle);
    }
    CATCPDeinitializeQueueHandles();
#endif

    CATCPStopServer();

    //Re-initializing the Globals to start them again
    CAInitializeTCPGlobals();

    return CA_STATUS_OK;
}

void CATerminateTCP()
{
    CATCPSetPacketReceiveCallback(NULL);
}

void CATCPSendDataThread(void *threadData)
{
    CATCPData *tcpData = (CATCPData *) threadData;
    if (!tcpData)
    {
        OIC_LOG(DEBUG, TAG, "Invalid TCP data!");
        return;
    }

    if (caglobals.tcp.terminate)
    {
        OIC_LOG(DEBUG, TAG, "Adapter is not enabled");
        CATCPErrorHandler(tcpData->remoteEndpoint, tcpData->data, tcpData->dataLen,
                          CA_SEND_FAILED);
        return;
    }

    if (tcpData->isMulticast)
    {
        //Processing for sending multicast
        OIC_LOG(DEBUG, TAG, "Send Multicast Data is called, not supported");
        return;
    }
    else
    {
#ifdef __WITH_TLS__
        if (tcpData->remoteEndpoint && tcpData->remoteEndpoint->flags & CA_SECURE)
        {
            CAResult_t result = CA_STATUS_OK;
            OIC_LOG(DEBUG, TAG, "CAencryptSsl called!");
            result = CAencryptSsl(tcpData->remoteEndpoint, tcpData->data, tcpData->dataLen);

            if (CA_STATUS_OK != result)
            {
                OIC_LOG(ERROR, TAG, "CAAdapterNetDtlsEncrypt failed!");
                CASearchAndDeleteTCPSession(tcpData->remoteEndpoint);
                CATCPErrorHandler(tcpData->remoteEndpoint, tcpData->data, tcpData->dataLen,
                                  CA_SEND_FAILED);
            }
            OIC_LOG_V(DEBUG, TAG,
                      "CAAdapterNetDtlsEncrypt returned with result[%d]", result);
            return;
        }
#endif
        //Processing for sending unicast
         ssize_t dlen = CATCPSendData(tcpData->remoteEndpoint, tcpData->data, tcpData->dataLen);
         if (-1 == dlen)
         {
             OIC_LOG(ERROR, TAG, "CATCPSendData failed");
             CASearchAndDeleteTCPSession(tcpData->remoteEndpoint);
             CATCPErrorHandler(tcpData->remoteEndpoint, tcpData->data, tcpData->dataLen,
                               CA_SEND_FAILED);
         }
    }
}

CATCPData *CACreateTCPData(const CAEndpoint_t *remoteEndpoint, const void *data,
                           size_t dataLength, bool isMulticast)
{
    VERIFY_NON_NULL_RET(remoteEndpoint, TAG, "remoteEndpoint is NULL", NULL);
    VERIFY_NON_NULL_RET(data, TAG, "data is NULL", NULL);

    CATCPData *tcpData = (CATCPData *) OICCalloc(1, sizeof(*tcpData));
    if (!tcpData)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return NULL;
    }

    tcpData->remoteEndpoint = CACloneEndpoint(remoteEndpoint);
    tcpData->data = (void *) OICMalloc(dataLength);
    if (!tcpData->data)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        CAFreeTCPData(tcpData);
        return NULL;
    }

    memcpy(tcpData->data, data, dataLength);
    tcpData->dataLen = dataLength;

    tcpData->isMulticast = isMulticast;

    return tcpData;
}

void CAFreeTCPData(CATCPData *tcpData)
{
    VERIFY_NON_NULL_VOID(tcpData, TAG, "tcpData is NULL");

    CAFreeEndpoint(tcpData->remoteEndpoint);
    OICFree(tcpData->data);
    OICFree(tcpData);
}

void CADataDestroyer(void *data, uint32_t size)
{
    if (size < sizeof(CATCPData))
    {
#ifndef __TIZENRT__
        OIC_LOG_V(ERROR, TAG, "Destroy data too small %p %" PRIu32, data, size);
#endif
    }
    CATCPData *TCPData = (CATCPData *) data;

    CAFreeTCPData(TCPData);
}

#ifdef SINGLE_THREAD
size_t CAGetTotalLengthFromPacketHeader(const unsigned char *recvBuffer, size_t size)
{
    OIC_LOG(DEBUG, TAG, "IN - CAGetTotalLengthFromHeader");

    if (NULL == recvBuffer || !size)
    {
        OIC_LOG(ERROR, TAG, "recvBuffer is NULL");
        return 0;
    }

    coap_transport_t transport = coap_get_tcp_header_type_from_initbyte(
            ((unsigned char *)recvBuffer)[0] >> 4);
    size_t optPaylaodLen = coap_get_length_from_header((unsigned char *)recvBuffer,
                                                        transport);
    size_t headerLen = coap_get_tcp_header_length((unsigned char *)recvBuffer);

    OIC_LOG_V(DEBUG, TAG, "option/paylaod length [%d]", optPaylaodLen);
    OIC_LOG_V(DEBUG, TAG, "header length [%d]", headerLen);
    OIC_LOG_V(DEBUG, TAG, "total data length [%d]", headerLen + optPaylaodLen);

    OIC_LOG(DEBUG, TAG, "OUT - CAGetTotalLengthFromHeader");
    return headerLen + optPaylaodLen;
}

void CAGetTCPHeaderDetails(unsigned char* recvBuffer, coap_transport_t *transport,
                           size_t *headerlen)
{
    if (NULL == recvBuffer)
    {
        OIC_LOG(ERROR, TAG, "recvBuffer is NULL");
        return;
    }

    if (NULL == transport)
    {
        OIC_LOG(ERROR, TAG, "transport is NULL");
        return;
    }

    if (NULL == headerlen)
    {
        OIC_LOG(ERROR, TAG, "headerlen is NULL");
        return;
    }

    *transport = coap_get_tcp_header_type_from_initbyte(
        ((unsigned char *)recvBuffer)[0] >> 4);
    *headerlen = coap_get_tcp_header_length_for_transport(*transport);
}
#endif
