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
#include "caipadapter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "caipnwmonitor.h"
#include "caipinterface.h"
#include "caqueueingthread.h"
#include "caadapterutils.h"
#ifdef __WITH_DTLS__
#include "ca_adapter_net_ssl.h"
#endif
#include "octhread.h"
#include "uarraylist.h"
#include "caremotehandler.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "iotivity_debug.h"

/**
 * Logging tag for module name.
 */
#define TAG "OIC_CA_IP_ADAP"

#ifndef SINGLE_THREAD
/**
 * Holds inter thread ip data information.
 */
typedef struct
{
    CAEndpoint_t *remoteEndpoint;
    void *data;
    uint32_t dataLen;
    bool isMulticast;
} CAIPData_t;

/**
 * Queue handle for Send Data.
 */
static CAQueueingThread_t *g_sendQueueHandle = NULL;
#endif

/**
 * List of the endpoint that has a stack-owned IP address.
 */
static u_arraylist_t *g_ownIpEndpointList = NULL;

/**
 * Network Packet Received Callback to CA.
 */
static CANetworkPacketReceivedCallback g_networkPacketCallback = NULL;

/**
 * Network Changed Callback to CA.
 */
static CAAdapterChangeCallback g_networkChangeCallback = NULL;

/**
 * error Callback to CA adapter.
 */
static CAErrorHandleCallback g_errorCallback = NULL;

static void CAIPPacketReceivedCB(const CASecureEndpoint_t *endpoint,
                                 const void *data, size_t dataLength);
#ifdef __WITH_DTLS__
static ssize_t CAIPPacketSendCB(CAEndpoint_t *endpoint,
                                const void *data, size_t dataLength);
#endif

static void CAUpdateStoredIPAddressInfo(CANetworkStatus_t status);

#ifndef SINGLE_THREAD

static CAResult_t CAIPInitializeQueueHandles();

static void CAIPDeinitializeQueueHandles();

static void CAIPSendDataThread(void *threadData);

static CAIPData_t *CACreateIPData(const CAEndpoint_t *remoteEndpoint,
                                  const void *data, uint32_t dataLength,
                                  bool isMulticast);

void CAFreeIPData(CAIPData_t *ipData);

static void CADataDestroyer(void *data, uint32_t size);

CAResult_t CAIPInitializeQueueHandles()
{
    // Check if the message queue is already initialized
    if (g_sendQueueHandle)
    {
        OIC_LOG(DEBUG, TAG, "send queue handle is already initialized!");
        return CA_STATUS_OK;
    }

    g_ownIpEndpointList = u_arraylist_create();
    if (!g_ownIpEndpointList)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed! (g_ownIpEndpointList)");
        return CA_MEMORY_ALLOC_FAILED;
    }

    // Create send message queue
    g_sendQueueHandle = OICMalloc(sizeof(CAQueueingThread_t));
    if (!g_sendQueueHandle)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed! (g_sendQueueHandle)");
        u_arraylist_free(&g_ownIpEndpointList);
        g_ownIpEndpointList = NULL;
        return CA_MEMORY_ALLOC_FAILED;
    }

    if (CA_STATUS_OK != CAQueueingThreadInitialize(g_sendQueueHandle,
                                (const ca_thread_pool_t)caglobals.ip.threadpool,
                                CAIPSendDataThread, CADataDestroyer))
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize send queue thread");
        OICFree(g_sendQueueHandle);
        g_sendQueueHandle = NULL;
        u_arraylist_free(&g_ownIpEndpointList);
        g_ownIpEndpointList = NULL;
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

void CAIPDeinitializeQueueHandles()
{
    CAQueueingThreadDestroy(g_sendQueueHandle);
    OICFree(g_sendQueueHandle);
    g_sendQueueHandle = NULL;

    // Since the items in g_ownIpEndpointList are allocated once in a big chunk, we only need to
    // free the first item. Another location this is done is in the CA_INTERFACE_DOWN handler
    // in CAUpdateStoredIPAddressInfo().
    OICFree(u_arraylist_get(g_ownIpEndpointList, 0));
    u_arraylist_free(&g_ownIpEndpointList);
    g_ownIpEndpointList = NULL;
}

#endif // SINGLE_THREAD

void CAIPAdapterHandler(CATransportAdapter_t adapter, CANetworkStatus_t status)
{
    CAUpdateStoredIPAddressInfo(status);

    if (g_networkChangeCallback)
    {
        g_networkChangeCallback(adapter, status);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "g_networkChangeCallback is NULL");
    }

    if (CA_INTERFACE_DOWN == status)
    {
        OIC_LOG(DEBUG, TAG, "Network status for IP is down");
#ifdef __WITH_DTLS__
        OIC_LOG(DEBUG, TAG, "close all ssl session");
        CAcloseSslConnectionAll(CA_ADAPTER_IP);
#endif
    }
}

static void CAUpdateStoredIPAddressInfo(CANetworkStatus_t status)
{
    if (CA_INTERFACE_UP == status)
    {
        OIC_LOG(DEBUG, TAG, "IP adapter status is on. Store the own IP address info");

        CAEndpoint_t *eps = NULL;
        size_t numOfEps = 0;

        CAResult_t res = CAGetIPInterfaceInformation(&eps, &numOfEps);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CAGetIPInterfaceInformation failed");
            return;
        }

        for (size_t i = 0; i < numOfEps; i++)
        {
            u_arraylist_add(g_ownIpEndpointList, (void *)&eps[i]);
        }
    }
    else // CA_INTERFACE_DOWN
    {
        OIC_LOG(DEBUG, TAG, "IP adapter status is off. Remove the own IP address info");

        CAEndpoint_t *headEp = u_arraylist_get(g_ownIpEndpointList, 0);
        OICFree(headEp);
        headEp = NULL;

        size_t len = u_arraylist_length(g_ownIpEndpointList);
        for (size_t i = len; i > 0; i--)
        {
            u_arraylist_remove(g_ownIpEndpointList, i - 1);
        }
    }
}

#ifdef __WITH_DTLS__
static ssize_t CAIPPacketSendCB(CAEndpoint_t *endpoint, const void *data, size_t dataLength)
{
    VERIFY_NON_NULL_RET(endpoint, TAG, "endpoint is NULL", -1);
    VERIFY_NON_NULL_RET(data, TAG, "data is NULL", -1);

    CAIPSendData(endpoint, data, dataLength, false);
    return dataLength;
}
#endif

void CAIPPacketReceivedCB(const CASecureEndpoint_t *sep, const void *data,
                          size_t dataLength)
{
    VERIFY_NON_NULL_VOID(sep, TAG, "sep is NULL");
    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

    OIC_LOG_V(DEBUG, TAG, "Address: %s, port:%d", sep->endpoint.addr, sep->endpoint.port);

    if (g_networkPacketCallback)
    {
        g_networkPacketCallback(sep, data, dataLength);
    }
}

bool CAIPIsLocalEndpoint(const CAEndpoint_t *ep)
{
    char addr[MAX_ADDR_STR_SIZE_CA];
    OICStrcpy(addr, MAX_ADDR_STR_SIZE_CA, ep->addr);

    // drop the zone ID if the address of endpoint is IPv6. ifindex will be checked instead.
    if ((ep->flags & CA_IPV6) && strchr(addr, '%'))
    {
        strtok(addr, "%");
    }

    size_t len = u_arraylist_length(g_ownIpEndpointList);
    for (size_t i = 0; i < len; i++)
    {
        CAEndpoint_t *ownIpEp = u_arraylist_get(g_ownIpEndpointList, i);
        if (!strcmp(addr, ownIpEp->addr) && ep->port == ownIpEp->port
                                         && ep->ifindex == ownIpEp->ifindex)
        {
            return true;
        }
    }

    return false;
}

void CAIPErrorHandler(const CAEndpoint_t *endpoint, const void *data,
                      size_t dataLength, CAResult_t result)
{
    VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
    VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

    if (g_errorCallback)
    {
        g_errorCallback(endpoint, data, dataLength, result);
    }
}

static void CAInitializeIPGlobals()
{
    caglobals.ip.u6.fd  = OC_INVALID_SOCKET;
    caglobals.ip.u6s.fd = OC_INVALID_SOCKET;
    caglobals.ip.u4.fd  = OC_INVALID_SOCKET;
    caglobals.ip.u4s.fd = OC_INVALID_SOCKET;
    caglobals.ip.m6.fd  = OC_INVALID_SOCKET;
    caglobals.ip.m6s.fd = OC_INVALID_SOCKET;
    caglobals.ip.m4.fd  = OC_INVALID_SOCKET;
    caglobals.ip.m4s.fd = OC_INVALID_SOCKET;
    caglobals.ip.u6.port  = 0;
    caglobals.ip.u6s.port = 0;
    caglobals.ip.u4.port  = 0;
    caglobals.ip.u4s.port = 0;
    caglobals.ip.m6.port  = CA_COAP;
    caglobals.ip.m6s.port = CA_SECURE_COAP;
    caglobals.ip.m4.port  = CA_COAP;
    caglobals.ip.m4s.port = CA_SECURE_COAP;

    CATransportFlags_t flags = 0;
    if (caglobals.client)
    {
        flags |= caglobals.clientFlags;
    }
    if (caglobals.server)
    {
        flags |= caglobals.serverFlags;
    }
    caglobals.ip.ipv6enabled = flags & CA_IPV6;
    caglobals.ip.ipv4enabled = flags & CA_IPV4;
    caglobals.ip.dualstack = caglobals.ip.ipv6enabled && caglobals.ip.ipv4enabled;
}

CAResult_t CAInitializeIP(CARegisterConnectivityCallback registerCallback,
                          CANetworkPacketReceivedCallback networkPacketCallback,
                          CAAdapterChangeCallback netCallback,
                          CAErrorHandleCallback errorCallback, ca_thread_pool_t handle)
{
    OIC_LOG(DEBUG, TAG, "IN");
    VERIFY_NON_NULL(registerCallback, TAG, "registerCallback");
    VERIFY_NON_NULL(networkPacketCallback, TAG, "networkPacketCallback");
    VERIFY_NON_NULL(netCallback, TAG, "netCallback");
#ifndef SINGLE_THREAD
    VERIFY_NON_NULL(handle, TAG, "thread pool handle");
#endif

#ifdef WSA_WAIT_EVENT_0
    // Windows-specific initialization.
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData = {.wVersion = 0};
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (0 != err)
    {
        OIC_LOG_V(ERROR, TAG, "%s: WSAStartup failed: %i", __func__, err);
        return CA_STATUS_FAILED;
    }
    OIC_LOG(DEBUG, TAG, "WSAStartup Succeeded");
#endif

    g_networkChangeCallback = netCallback;
    g_networkPacketCallback = networkPacketCallback;
    g_errorCallback = errorCallback;

    CAInitializeIPGlobals();
    caglobals.ip.threadpool = handle;

    CAIPSetErrorHandler(CAIPErrorHandler);
    CAIPSetPacketReceiveCallback(CAIPPacketReceivedCB);

#ifdef __WITH_DTLS__
    if (CA_STATUS_OK != CAinitSslAdapter())
    {
        OIC_LOG(ERROR, TAG, "Failed to init SSL adapter");
    }
    else
    {
        CAsetSslAdapterCallbacks(CAIPPacketReceivedCB, CAIPPacketSendCB, CAIPErrorHandler, CA_ADAPTER_IP);
    }
#endif

    static const CAConnectivityHandler_t ipHandler =
        {
            .startAdapter = CAStartIP,
            .stopAdapter = CAStopIP,
            .startListenServer = CAStartIPListeningServer,
            .stopListenServer = CAStopIPListeningServer,
            .startDiscoveryServer = CAStartIPDiscoveryServer,
            .sendData = CASendIPUnicastData,
            .sendDataToAll = CASendIPMulticastData,
            .GetnetInfo = CAGetIPInterfaceInformation,
            .readData = CAReadIPData,
            .terminate = CATerminateIP,
            .cType = CA_ADAPTER_IP
        };
    registerCallback(ipHandler);

    OIC_LOG(INFO, TAG, "OUT IntializeIP is Success");
    return CA_STATUS_OK;
}

CAResult_t CAStartIP()
{
    //Initializing the Globals
    CAInitializeIPGlobals();

    // Specific the port number received from application.
    caglobals.ip.u6.port  = caglobals.ports.udp.u6;
    caglobals.ip.u6s.port = caglobals.ports.udp.u6s;
    caglobals.ip.u4.port  = caglobals.ports.udp.u4;
    caglobals.ip.u4s.port = caglobals.ports.udp.u4s;

    CAIPStartNetworkMonitor(CAIPAdapterHandler, CA_ADAPTER_IP);
#ifdef SINGLE_THREAD
    uint16_t unicastPort = 55555;
    // Address is hardcoded as we are using Single Interface
    CAResult_t ret = CAIPStartServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(DEBUG, TAG, "CAIPStartServer failed[%d]", ret);
        return ret;
    }
#else
    if (CA_STATUS_OK != CAIPInitializeQueueHandles())
    {
        OIC_LOG(ERROR, TAG, "Failed to Initialize Queue Handle");
        CATerminateIP();
        return CA_STATUS_FAILED;
    }

    // Start send queue thread
#ifndef __TIZENRT__
    if (CA_STATUS_OK != CAQueueingThreadStart(g_sendQueueHandle))
#else
    if (CA_STATUS_OK != CAQueueingThreadStart(g_sendQueueHandle, "IoT_IPSendQueue"))
#endif
    {
        OIC_LOG(ERROR, TAG, "Failed to Start Send Data Thread");
        return CA_STATUS_FAILED;
    }

    CAResult_t ret = CAIPStartServer((const ca_thread_pool_t)caglobals.ip.threadpool);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start server![%d]", ret);
        return ret;
    }

#endif

    return CA_STATUS_OK;
}

CAResult_t CAStartIPListeningServer()
{
    CAResult_t ret = CAIPStartListenServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to start listening server![%d]", ret);
        return ret;
    }

    return CA_STATUS_OK;
}

CAResult_t CAStopIPListeningServer()
{
    CAResult_t ret = CAIPStopListenServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to stop listening server![%d]", ret);
    }

    return ret;
}

CAResult_t CAStartIPDiscoveryServer()
{
    return CAStartIPListeningServer();
}

static int32_t CAQueueIPData(bool isMulticast, const CAEndpoint_t *endpoint,
                             const void *data, uint32_t dataLength)
{
    VERIFY_NON_NULL_RET(endpoint, TAG, "remoteEndpoint", -1);
    VERIFY_NON_NULL_RET(data, TAG, "data", -1);

    if (0 == dataLength)
    {
        OIC_LOG(ERROR, TAG, "Invalid Data Length");
        return -1;
    }

#ifdef SINGLE_THREAD

    CAIPSendData(endpoint, data, dataLength, isMulticast);
    return dataLength;

#else

    VERIFY_NON_NULL_RET(g_sendQueueHandle, TAG, "sendQueueHandle", -1);
    // Create IPData to add to queue
    CAIPData_t *ipData = CACreateIPData(endpoint, data, dataLength, isMulticast);
    if (!ipData)
    {
        OIC_LOG(ERROR, TAG, "Failed to create ipData!");
        return -1;
    }
    // Add message to send queue
    CAQueueingThreadAddData(g_sendQueueHandle, ipData, sizeof(CAIPData_t));

#endif // SINGLE_THREAD

    return dataLength;
}

int32_t CASendIPUnicastData(const CAEndpoint_t *endpoint,
                            const void *data, uint32_t dataLength,
                            CADataType_t dataType)
{
    (void)dataType;
    return CAQueueIPData(false, endpoint, data, dataLength);
}

int32_t CASendIPMulticastData(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength,
                              CADataType_t dataType)
{
    (void)dataType;
    return CAQueueIPData(true, endpoint, data, dataLength);
}

CAResult_t CAReadIPData()
{
    CAIPPullData();
    return CA_STATUS_OK;
}

CAResult_t CAStopIP()
{
#ifdef __WITH_DTLS__
    CAdeinitSslAdapter();
#endif

#ifndef SINGLE_THREAD
    if (g_sendQueueHandle && g_sendQueueHandle->threadMutex)
    {
        CAQueueingThreadStop(g_sendQueueHandle);
    }
#endif

    CAIPStopNetworkMonitor(CA_ADAPTER_IP);
    CAIPStopServer();

    return CA_STATUS_OK;
}

void CATerminateIP()
{
#ifdef __WITH_DTLS__
    CAsetSslAdapterCallbacks(NULL, NULL, NULL, CA_ADAPTER_IP);
#endif

    CAIPSetPacketReceiveCallback(NULL);

#ifndef SINGLE_THREAD
    CAIPDeinitializeQueueHandles();
#endif

#ifdef WSA_WAIT_EVENT_0
    // Windows-specific clean-up.
    OC_VERIFY(0 == WSACleanup());
#endif
}

#ifndef SINGLE_THREAD

void CAIPSendDataThread(void *threadData)
{
    CAIPData_t *ipData = (CAIPData_t *) threadData;
    if (!ipData)
    {
        OIC_LOG(DEBUG, TAG, "Invalid ip data!");
        return;
    }

    if (ipData->isMulticast)
    {
        //Processing for sending multicast
        OIC_LOG(DEBUG, TAG, "Send Multicast Data is called");
        CAIPSendData(ipData->remoteEndpoint, ipData->data, ipData->dataLen, true);
    }
    else
    {
        //Processing for sending unicast
#ifdef __WITH_DTLS__
        if (ipData->remoteEndpoint && ipData->remoteEndpoint->flags & CA_SECURE)
        {
            OIC_LOG(DEBUG, TAG, "DTLS encrypt called");
            CAResult_t result = CAencryptSsl(ipData->remoteEndpoint, ipData->data, ipData->dataLen);
            if (CA_STATUS_OK != result)
            {
                OIC_LOG(ERROR, TAG, "CAencryptSsl failed!");
            }
            OIC_LOG_V(DEBUG, TAG, "CAencryptSsl returned with result[%d]", result);
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "Send Unicast Data is called");
            CAIPSendData(ipData->remoteEndpoint, ipData->data, ipData->dataLen, false);
        }
#else
        CAIPSendData(ipData->remoteEndpoint, ipData->data, ipData->dataLen, false);
#endif
    }
}

#endif

#ifndef SINGLE_THREAD
CAIPData_t *CACreateIPData(const CAEndpoint_t *remoteEndpoint, const void *data,
                           uint32_t dataLength, bool isMulticast)
{
    VERIFY_NON_NULL_RET(remoteEndpoint, TAG, "remoteEndpoint is NULL", NULL);
    VERIFY_NON_NULL_RET(data, TAG, "IPData is NULL", NULL);

    CAIPData_t *ipData = (CAIPData_t *) OICMalloc(sizeof(*ipData));
    if (!ipData)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return NULL;
    }

    ipData->remoteEndpoint = CACloneEndpoint(remoteEndpoint);
    ipData->data = (void *) OICMalloc(dataLength);
    if (!ipData->data)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        CAFreeIPData(ipData);
        return NULL;
    }

    memcpy(ipData->data, data, dataLength);
    ipData->dataLen = dataLength;

    ipData->isMulticast = isMulticast;

    return ipData;
}

void CAFreeIPData(CAIPData_t *ipData)
{
    VERIFY_NON_NULL_VOID(ipData, TAG, "ipData is NULL");

    CAFreeEndpoint(ipData->remoteEndpoint);
    OICFree(ipData->data);
    OICFree(ipData);
}

void CADataDestroyer(void *data, uint32_t size)
{
    if (size < sizeof(CAIPData_t))
    {
        OIC_LOG_V(ERROR, TAG, "Destroy data too small %p %d", data, size);
    }
    CAIPData_t *etdata = (CAIPData_t *) data;

    CAFreeIPData(etdata);
}

#endif // SINGLE_THREAD
