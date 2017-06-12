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
 * @file caipinterface.h
 * This file provides APIs IP client/server/network monitor modules.
 */

#ifndef CA_IP_INTERFACE_H_
#define CA_IP_INTERFACE_H_

#include <stdbool.h>

#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Enum for defining different server types.
 */
typedef enum
{
    CA_UNICAST_SERVER = 0,      /**< Unicast Server */
    CA_MULTICAST_SERVER,        /**< Multicast Server */
    CA_SECURED_UNICAST_SERVER   /**< Secured Unicast Server */
} CAAdapterServerType_t;

/**
 * Callback to be notified on reception of any data from remote OIC devices.
 *
 * @param[in]  sep         network endpoint description.
 * @param[in]  data          Data received from remote OIC device.
 * @param[in]  dataLength    Length of data in bytes.
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 * @pre  Callback must be registered using CAIPSetPacketReceiveCallback().
 */
typedef CAResult_t (*CAIPPacketReceivedCallback)(const CASecureEndpoint_t *sep,
                                                 const void *data,
                                                 uint32_t dataLength);

/**
  * Callback to notify error in the IP adapter.
  *
  * @param[in]  endpoint       network endpoint description.
  * @param[in]  data          Data sent/received.
  * @param[in]  dataLength    Length of data in bytes.
  * @param[in]  result        result of request from R.I.
  * @pre  Callback must be registered using CAIPSetPacketReceiveCallback().
 */
typedef void (*CAIPErrorHandleCallback)(const CAEndpoint_t *endpoint, const void *data,
                                        uint32_t dataLength, CAResult_t result);

/**
 * Start IP server.
 *
 * @param   threadPool   Thread pool for managing Unicast/Multicast server threads.
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM Invalid input data.
 * @retval ::CA_STATUS_FAILED Initialization failed.
 */
#ifdef SINGLE_THREAD
CAResult_t CAIPStartServer();
#else
CAResult_t CAIPStartServer(const ca_thread_pool_t threadPool);
#endif

/**
 * Close IP socket.
 */
void CADeInitializeIPGlobals();

/**
 * Stop IP server.
 */
void CAIPStopServer();

/**
 * Starts receiving the multicast traffic.
 *
 * This will be used in case sleepy device wants to start back receiving the multicast
 * traffic.
 */
CAResult_t CAIPStartListenServer();

/**
 * Stops the multicast traffic.
 *
 * This is to be used by the sleeping device to stop receiving multicast traffic.
 * Once this is set no multicast traffic will be received. Device can still receive
 * the unicast traffic.
 */
CAResult_t CAIPStopListenServer();

/**
 * Set this callback for receiving data packets from peer devices.
 *
 * @param[in]  callback    Callback to be notified on reception of unicast/multicast data packets.
 */
void CAIPSetPacketReceiveCallback(CAIPPacketReceivedCallback callback);

/**
 * Set socket description for sending unicast UDP data.
 * Once the Unicast server is started,
 * the same socket descriptor is used for sending the Unicast UDP data.
 *
 * @param[in]  socketFD   Socket descriptor used for sending UDP data.
 */
void CAIPSetUnicastSocket(int socketFD);

/**
 * Set the port number for sending unicast UDP data.
 * @param[in]  port  Port number used for sending UDP data.
 */
void CAIPSetUnicastPort(uint16_t port);

/**
 * API to send unicast UDP data.
 *
 * @param[in]  endpoint          complete network address to send to.
 * @param[in]  data              Data to be send.
 * @param[in]  dataLength        Length of data in bytes.
 * @param[in]  isMulticast       Whether data needs to be sent to multicast ip.
 */
void CAIPSendData(CAEndpoint_t *endpoint,
                  const void *data,
                  uint32_t dataLength,
                  bool isMulticast);

/**
 * Get IP adapter connection state.
 *
 * @return  True if IP adapter is connected, otherwise false.
 */
bool CAIPIsConnected();

/**
 * Pull the Received Data.
 */
void CAIPPullData();

#define CA_COAP        5683
#define CA_SECURE_COAP 5684

/**
 * Let the network monitor update the polling interval.
 * @param   [in] current polling interval
 *
 * @return  desired polling interval
 */
int CAGetPollingInterval(int interval);

/**
 * Tell the IP server an interface has been added.
 */
void CAWakeUpForChange();

/**
 * Set callback for error handling.
 *
 * @param[in]  errorHandleCallback  callback to notify error to the ipadapter.
 */
void CAIPSetErrorHandler(CAIPErrorHandleCallback errorHandleCallback);

/**
 * Set udp multicast time to live value to control the scope of the multicasts.
 * @param[in]  ttl         To be set to any value from 0 to 255.
 *
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAIPSetMulticastTTL(size_t ttl);

/**
 * Get udp multicast time to live value.
 * @param[out]  ttl         TTL pointer to get the stored multicast time to live.
 *
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAIPGetMulticastTTL(size_t *ttl);
#ifdef __cplusplus
}
#endif

#endif /* CA_IP_INTERFACE_H_ */
