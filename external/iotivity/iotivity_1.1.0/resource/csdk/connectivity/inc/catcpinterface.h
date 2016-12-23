/* *****************************************************************
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

/**
 * @file
 * This file provides APIs TCP client/server/network monitor modules.
 */

#ifndef CA_TCP_INTERFACE_H_
#define CA_TCP_INTERFACE_H_

#include <stdbool.h>

#include "cacommon.h"
#include "catcpadapter.h"
#include "cathreadpool.h"
#include "uarraylist.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Callback to be notified on reception of any data from remote OIC devices.
 *
 * @param[in]  endpoint      network endpoint description.
 * @param[in]  data          Data received from remote OIC device.
 * @param[in]  dataLength    Length of data in bytes.
 * @pre  Callback must be registered using CAIPSetPacketReceiveCallback().
 */
typedef void (*CATCPPacketReceivedCallback)(const CASecureEndpoint_t *endpoint,
                                            const void *data,
                                            uint32_t dataLength);

/**
  * Callback to notify error in the TCP adapter.
  *
  * @param[in]  endpoint      network endpoint description.
  * @param[in]  data          Data sent/received.
  * @param[in]  dataLength    Length of data in bytes.
  * @param[in]  result        result of request from R.I.
  * @pre  Callback must be registered using CAIPSetPacketReceiveCallback().
 */
typedef void (*CATCPErrorHandleCallback)(const CAEndpoint_t *endpoint, const void *data,
                                         uint32_t dataLength, CAResult_t result);

/**
  * Callback to notify connection information in the TCP adapter.
  *
  * @param[in]  addr    connected device address.
  * @param[in]  port    connected port info.
  * @param[in]  isConnected    Whether keepalive message needs to be sent.
  * @see  Callback must be registered using CATCPSetKeepAliveCallback().
 */
typedef void (*CATCPKeepAliveHandleCallback)(const char *addr, uint16_t port, bool isConnected);

/**
 * set error callback to notify error in TCP adapter.
 *
 * @param[in]  errorHandleCallback Callback function to notify the error
 * in the TCP adapter.
 */
void CATCPSetErrorHandler(CATCPErrorHandleCallback errorHandleCallback);

/**
 * set keepalive callback to notify connection information in TCP adapter.
 *
 * @param[in]  keepaliveHandler Callback function to notify the connection information.
 * in the TCP adapter.
 */
void CATCPSetKeepAliveCallback(CATCPKeepAliveHandleCallback keepaliveHandler);

/**
 * Start TCP server.
 *
 * @param   threadPool   Thread pool for managing Unicast server threads.
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM Invalid input data.
 * @retval ::CA_STATUS_FAILED Initialization failed.
 */
CAResult_t CATCPStartServer(const ca_thread_pool_t threadPool);

/**
 * Stop TCP server.
 */
void CATCPStopServer();

/**
 * Set this callback for receiving data packets from peer devices.
 *
 * @param[in]  callback    Callback to be notified on reception of unicast data packets.
 */
void CATCPSetPacketReceiveCallback(CATCPPacketReceivedCallback callback);

/**
 * API to send unicast TCP data.
 *
 * @param[in]  endpoint          complete network address to send to.
 * @param[in]  data              Data to be send.
 * @param[in]  dataLength        Length of data in bytes.
 * @param[in]  isMulticast       Whether data needs to be sent to multicast ip.
 */
void CATCPSendData(CAEndpoint_t *endpoint, const void *data, uint32_t dataLength,
                   bool isMulticast);

/**
 * Get a list of CAInterface_t items.
 *
 * @return  List of CAInterface_t items.
 */
u_arraylist_t *CATCPGetInterfaceInformation(int desiredIndex);

/**
 * Connect to TCP Server.
 *
 * @param[in]   endpoint    remote endpoint information.
 * @return  TCP Session Information structure.
 */
CATCPSessionInfo_t *CAConnectTCPSession(const CAEndpoint_t *endpoint);

/**
 * Disconnect from TCP Server.
 *
 * @param[in]   svritem     TCP session information.
 * @param[in]   index       current session index in list.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CADisconnectTCPSession(CATCPSessionInfo_t *svritem, size_t index);

/**
 * Disconnect all connection from TCP Server.
 */
void CATCPDisconnectAll();

/**
 * Get TCP connection information from list.
 *
 * @param[in]   endpoint    remote endpoint information.
 * @param[out]  index   index of array list.
 * @return  TCP Session Information structure.
 */
CATCPSessionInfo_t *CAGetTCPSessionInfoFromEndpoint(const CAEndpoint_t *endpoint,
    size_t *index);

/**
 * Get total length from CoAP over TCP header.
 *
 * @param[in]   recvBuffer    received header data.
 * @return  total data length
 */
size_t CAGetTotalLengthFromHeader(const unsigned char *recvBuffer);

/**
 * Get session information from file descriptor index.
 *
 * @param[in]   fd      file descriptor.
 * @param[out]  index   index of array list
 * @return  TCP Server Information structure.
 */
CATCPSessionInfo_t *CAGetSessionInfoFromFD(int fd, size_t *index);

#ifdef __cplusplus
}
#endif

#endif /* CA_TCP_INTERFACE_H_ */
