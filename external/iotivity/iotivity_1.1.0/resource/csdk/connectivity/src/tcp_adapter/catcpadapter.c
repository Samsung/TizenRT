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
#include "catcpadapter.h"
#include "catcpinterface.h"
#include "caqueueingthread.h"
#include "caadapterutils.h"
#include "camutex.h"
#include "uarraylist.h"
#include "caremotehandler.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"

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
 * Network Changed Callback to CA.
 */
static CANetworkChangeCallback g_networkChangeCallback = NULL;

/**
 * error Callback to CA adapter.
 */
static CAErrorHandleCallback g_errorCallback = NULL;

static void CATCPPacketReceivedCB(const CASecureEndpoint_t *sep,
                                  const void *data, uint32_t dataLength);

/**
 * KeepAlive Connected Callback to CA adapter.
 */
static CAKeepAliveConnectedCallback g_connCallback = NULL;

/**
 * KeepAlive Disconnected Callback to CA adapter.
 */
static CAKeepAliveDisconnectedCallback g_disconnCallback = NULL;

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

void CATCPPacketReceivedCB(const CASecureEndpoint_t *sep, const void *data,
                           uint32_t dataLength)
{
  VERIFY_NON_NULL_VOID(sep, TAG, "sep is NULL");
  VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

  OIC_LOG_V(DEBUG, TAG, "Address: %s, port:%d", sep->endpoint.addr, sep->endpoint.port);

  if (g_networkPacketCallback)
  {
    g_networkPacketCallback(sep, data, dataLength);
  }
}

void CATCPErrorHandler(const CAEndpoint_t *endpoint, const void *data,
                       uint32_t dataLength, CAResult_t result)
{
  VERIFY_NON_NULL_VOID(endpoint, TAG, "endpoint is NULL");
  VERIFY_NON_NULL_VOID(data, TAG, "data is NULL");

  if (g_errorCallback)
  {
    g_errorCallback(endpoint, data, dataLength, result);
  }
}

static void CATCPKeepAliveHandler(const char *addr, uint16_t port, bool isConnected)
{
  CAEndpoint_t endpoint = { .adapter =  CA_ADAPTER_TCP,
                            .port = port
                          };
  OICStrcpy(endpoint.addr, sizeof(endpoint.addr), addr);

  if (isConnected)
  {
    g_connCallback(&endpoint);
  }
  else
  {
    g_disconnCallback(&endpoint);
  }
}

void CATCPSetKeepAliveCallbacks(CAKeepAliveConnectedCallback ConnHandler,
                                CAKeepAliveDisconnectedCallback DisconnHandler)
{
  g_connCallback = ConnHandler;
  g_disconnCallback = DisconnHandler;

  CATCPSetKeepAliveCallback(CATCPKeepAliveHandler);
}

static void CAInitializeTCPGlobals()
{
  caglobals.tcp.selectTimeout = CA_TCP_SELECT_TIMEOUT;
  caglobals.tcp.listenBacklog = CA_TCP_LISTEN_BACKLOG;
  caglobals.tcp.svrlist = NULL;

  CATransportFlags_t flags = 0;
  if (caglobals.client)
  {
    flags |= caglobals.clientFlags;
  }
  if (caglobals.server)
  {
    flags |= caglobals.serverFlags;
  }

  caglobals.tcp.ipv4tcpenabled = flags & CA_IPV4;
}

CAResult_t CAInitializeTCP(CARegisterConnectivityCallback registerCallback,
                           CANetworkPacketReceivedCallback networkPacketCallback,
                           CANetworkChangeCallback netCallback,
                           CAErrorHandleCallback errorCallback, ca_thread_pool_t handle)
{
  OIC_LOG(DEBUG, TAG, "IN");
  VERIFY_NON_NULL(registerCallback, TAG, "registerCallback");
  VERIFY_NON_NULL(networkPacketCallback, TAG, "networkPacketCallback");
  VERIFY_NON_NULL(netCallback, TAG, "netCallback");
  VERIFY_NON_NULL(handle, TAG, "thread pool handle");

  g_networkChangeCallback = netCallback;
  g_networkPacketCallback = networkPacketCallback;
  g_errorCallback = errorCallback;

  CAInitializeTCPGlobals();
  caglobals.tcp.threadpool = handle;

  CATCPSetPacketReceiveCallback(CATCPPacketReceivedCB);
  CATCPSetErrorHandler(CATCPErrorHandler);

  CAConnectivityHandler_t tcpHandler =
  {
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
    .cType = CA_ADAPTER_TCP
  };

  registerCallback(tcpHandler);

  OIC_LOG(INFO, TAG, "OUT IntializeTCP is Success");
  return CA_STATUS_OK;
}

CAResult_t CAStartTCP()
{
  if (CA_STATUS_OK != CATCPInitializeQueueHandles())
  {
    OIC_LOG(ERROR, TAG, "Failed to Initialize Queue Handle");
    CATerminateTCP();
    return CA_STATUS_FAILED;
  }

  // Start send queue thread
  if (CA_STATUS_OK != CAQueueingThreadStart(g_sendQueueHandle))
  {
    OIC_LOG(ERROR, TAG, "Failed to Start Send Data Thread");
    return CA_STATUS_FAILED;
  }

  return CA_STATUS_OK;
}

CAResult_t CAStartTCPListeningServer()
{
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
                             const void *data, uint32_t dataLength)
{
  return CAQueueTCPData(false, endpoint, data, dataLength);
}

int32_t CASendTCPMulticastData(const CAEndpoint_t *endpoint,
                               const void *data, uint32_t dataLength)
{
  return CAQueueTCPData(true, endpoint, data, dataLength);
}

CAResult_t CAReadTCPData()
{
  return CA_STATUS_OK;
}

CAResult_t CAStopTCP()
{
  if (g_sendQueueHandle && g_sendQueueHandle->threadMutex)
  {
    CAQueueingThreadStop(g_sendQueueHandle);
  }

  CATCPStopServer();

  //Re-initializing the Globals to start them again
  CAInitializeTCPGlobals();

  return CA_STATUS_OK;
}

void CATerminateTCP()
{
  CATCPSetPacketReceiveCallback(NULL);

  CATCPDeinitializeQueueHandles();
}

void CATCPSendDataThread(void *threadData)
{
  CATCPData *tcpData = (CATCPData *) threadData;
  if (!tcpData)
  {
    OIC_LOG(DEBUG, TAG, "Invalid TCP data!");
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
    //Processing for sending unicast
    CATCPSendData(tcpData->remoteEndpoint, tcpData->data, tcpData->dataLen, false);
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
    //OIC_LOG_V(ERROR, TAG, "Destroy data too small %p %" PRIu32, data, size);
  }
  CATCPData *TCPData = (CATCPData *) data;

  CAFreeTCPData(TCPData);
}
