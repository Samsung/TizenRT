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
 * This file contains the APIs for NFC Adapter.
 */
#ifndef CA_NFC_ADAPTER_H_
#define CA_NFC_ADAPTER_H_

#include "cacommon.h"
#include "caadapterinterface.h"
#include "cathreadpool.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * API to initialize NFC Interface.
 * @param[in] registerCallback      Callback to register NFC interfaces to Connectivity
 *                                  Abstraction Layer
 * @param[in] networkPacketCallback Callback to notify request and response messages from server(s)
 *                                  started at Connectivity Abstraction Layer.
 * @param[in] netCallback           Callback to notify the network additions to Connectivity
 *                                  Abstraction Layer.
 * @param[in] errorCallback         Callback to notify the network errors to Connectivity
 *                                  Abstraction Layer
 * @param[in] handle                Threadpool Handle
 * @return  ::CA_STATUS_OK, if success or Appropriate error code
 */
CAResult_t CAInitializeNFC(CARegisterConnectivityCallback registerCallback,
                          CANetworkPacketReceivedCallback networkPacketCallback,
                          CAAdapterChangeCallback netCallback,
                          CAErrorHandleCallback errorCallback, ca_thread_pool_t handle);

/**
 * Start NFC Interface adapter.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code
 */
CAResult_t CAStartNFC();

/**
 * Start listening server for receiving multicast search requests.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code
 */
CAResult_t CAStartNFCListeningServer();

/**
 * Start listening server for receiving multicast search requests.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code
 */
CAResult_t CAStopNFCListeningServer();

/**
 * Start discovery servers for receiving multicast advertisements.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code.
 */
CAResult_t CAStartNFCDiscoveryServer();

/**
 * Sends data to the endpoint using the adapter connectivity.
 * @param[in]   endpoint    Remote Endpoint information.
 * @param[in]   data        data to be sent.
 * @param[in]   dataLen     Size of data to be sent.
 * @param[in]   dataType    Data type which is REQUEST or RESPONSE.
 * @return  number of bytes sent on the network. Return value equal to -1 indicates error.
 * @remark  dataLen must be > 0.
 */
int32_t CASendNFCUnicastData(const CAEndpoint_t *endpoint, const void *data,
                             uint32_t dataLen, CADataType_t dataType);

/**
 * Send Multicast data to the endpoint using the NFC connectivity.
 * @param[in]   endpoint    Remote Endpoint information.
 * @param[in]   data        Data which is required to be sent.
 * @param[in]   dataLen     Size of data to be sent.
   @param[in]   dataType    Data type which is REQUEST or RESPONSE.
 * @return  number of bytes sent on the network. Return value equal to -1 indicates error.
 * @remark  dataLen must be > 0.
 */
int32_t CASendNFCMulticastData(const CAEndpoint_t *endpoint, const void *data,
                               uint32_t dataLen, CADataType_t dataType);

/**
 * Get NFC Connectivity network information
 * @param[out]   info        Local connectivity information structures.
 * @param[out]   size        Number of local connectivity structures.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code.
 * @remarks info is allocated in this API and must be freed by the caller.
 */
CAResult_t CAGetNFCInterfaceInformation(CAEndpoint_t **info, size_t *size);

/**
 * Read Synchronous API callback.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code.
 */
CAResult_t CAReadNFCData();

/**
 * Stops stop servers and close the sockets.
 * @return  ::CA_STATUS_OK, if success or Appropriate error code.
 */
CAResult_t CAStopNFC();

/**
 * Terminate the NFC connectivity adapter.
 * Configuration information will be deleted from further use
 */
void CATerminateNFC();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* CA_NFC_ADAPTER_H_ */

