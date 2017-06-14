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
 * This file contains common utility function for CA transport adaptors.
 */

#ifndef CA_INTERFACE_CONTROLLER_H_
#define CA_INTERFACE_CONTROLLER_H_

#include "caadapterinterface.h"
#include "cainterface.h"
#include "cautilinterface.h"

#ifndef SINGLE_THREAD
#include "cathreadpool.h" /* for thread pool */
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef SINGLE_THREAD
/**
 * Initializes different adapters based on the compilation flags.
 */
void CAInitializeAdapters();
#else
/**
 * Initializes different adapters based on the compilation flags.
 * @param[in]   handle           thread pool handle created by message handler
 *                               for different adapters.
 * @param[in]   transportType    transport type to initialize.
 */
void CAInitializeAdapters(ca_thread_pool_t handle, CATransportAdapter_t transportType);
#endif

/**
 * Set the received packets callback for message handler.
 * @param[in]   callback         message handler callback to receive packets
 *                               from different adapters.
 */
void CASetPacketReceivedCallback(CANetworkPacketReceivedCallback callback);

/**
 * Set the error handler callback for message handler.
 * @param[in]   errorCallback    error handler callback from adapters
 */
void CASetErrorHandleCallback(CAErrorHandleCallback errorCallback);

/**
 * Set the network status changed callback for CAUtil.
 * @param[in]   adapterCB       CAUtil callback to receive adapter status changes.
 * @param[in]   connCB          CAUtil callback to receive connection status changes.
 */
void CASetNetworkMonitorCallbacks(CAAdapterStateChangedCB adapterCB,
                                  CAConnectionStateChangedCB connCB);

/**
 * Unset the network status changed callback for CAUtil.
 * @param[in]   adapterCB       CAUtil callback to receive adapter status changes.
 * @param[in]   connCB          CAUtil callback to receive connection status changes.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAUnsetNetworkMonitorCallbacks(CAAdapterStateChangedCB adapterCB,
                                          CAConnectionStateChangedCB connCB);

/**
 * Starting different connectivity adapters based on the network selection.
 * @param[in]   transportType    interested network for starting.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAStartAdapter(CATransportAdapter_t transportType);

/**
 * Stopping different connectivity adapters based on the network un-selection.
 * @param[in]   transportType    network type that want to stop.
 */
void CAStopAdapter(CATransportAdapter_t transportType);

#ifdef RA_ADAPTER
/**
 * Set Remote Access information for XMPP Client.
 * @param[in]   caraInfo         remote access info..
 *
 * @return  CA_STATUS_OK
 */
CAResult_t CASetAdapterRAInfo(const CARAInfo_t *caraInfo);
#endif

/**
 * Get network information such as ipaddress and mac information.
 * @param[out]   info           connectivity information
 *                                  such as ipaddress and mac information.
 * @param[out]   size           number of connectivity information structures.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAGetNetworkInfo(CAEndpoint_t **info, uint32_t *size);

/**
 * Sends unicast data to the remote endpoint.
 * @param[in]   endpoint       endpoint information where the data has to be sent.
 * @param[in]   data           data that needs to be sent.
 * @param[in]   length         length of the data that needs to be sent.
 * @param[in]   dataType       Data type which is REQUEST or RESPONSE.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CASendUnicastData(const CAEndpoint_t *endpoint, const void *data,
                             uint32_t length, CADataType_t dataType);

/**
 * Sends multicast data to all endpoints in the network.
 * @param[in]   endpoint       endpoint information where the data has to be sent.
 * @param[in]   data           data that needs to be sent.
 * @param[in]   length         length of the data that needs to be sent.
 * @param[in]   dataType       Data type which is REQUEST or RESPONSE.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */

CAResult_t CASendMulticastData(const CAEndpoint_t *endpoint, const void *data,
                               uint32_t length, CADataType_t dataType);

/**
 * Start listening servers to receive search requests from clients.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAStartListeningServerAdapters();

/**
 * Stop listening servers to receive search requests from clients.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAStopListeningServerAdapters();

/**
 * Start discovery servers to receive advertisements from server.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAStartDiscoveryServerAdapters();

/**
 * Terminates the adapters which are initialized during the initialization.
 */
void CATerminateAdapters();

#ifdef SINGLE_THREAD
/**
 * Checks for available data and reads it.
 * @return   ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAReadData();
#endif

#ifdef IP_ADAPTER
/**
 * Set multicast time to live value to control the scope of the multicasts.
 * @param[in]  ttl         To be set to any value from 0 to 255.
 *
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CASetMulticastTTL(size_t ttl);

/**
 * Get multicast time to live value.
 * @param[out]  ttl        TTL pointer to get the stored multicast time to live.
 *
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAGetMulticastTTL(size_t *ttl);
#endif

#ifdef TCP_ADAPTER
/**
 * Disconnect TCP session.
 * When there is no transmission for a long time.
 * Some carrier vendor is blocking data.
 * Thur, TCP Session is cleaned through this function.
 * @param[in]   endpoint       endpoint information to disconnect.
 */
CAResult_t CADisconnectSession(const CAEndpoint_t *endpoint);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_INTERFACE_CONTROLLER_H_ */

