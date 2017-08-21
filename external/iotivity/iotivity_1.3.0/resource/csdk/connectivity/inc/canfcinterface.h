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
 * This file provides APIs NFC client/server/network monitor modules.
 */

#ifndef CA_NFC_INTERFACE_H_
#define CA_NFC_INTERFACE_H_

#include <stdbool.h>

#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * CAAdapterServerType_t.
 * Enum for defining different server types.
 */
typedef enum
{
    CA_NFC_SERVER = 0, /**< Listening Server */
} CAAdapterServerType_t;

/**
 * Callback to be notified on reception of any data from remote OIC devices.
 *
 * @param[in]  endpoint    network endpoint description
 * @param[in]  data        Data received from remote OIC device.
 * @param[in]  dataLength  Length of data in bytes.
 */
typedef void (*CANFCPacketReceivedCallback)(const CASecureEndpoint_t *endpoint, const void *data,
                                            uint32_t dataLength);

/**
  * Callback to notify error in the NFC adapter.
  *
  * @param[in]  endpoint     network endpoint description.
  * @param[in]  data         Data sent/received.
  * @param[in]  dataLength   Length of data in bytes.
  * @param[in]  result       result of request.
  */
typedef void (*CANFCErrorHandleCallback)(const CAEndpoint_t *endpoint, const void *data,
                                         size_t dataLength, CAResult_t result);

/**
 * Callback to be notified when exception occures on multicast/unicast server.
 * @param[in]  type    Type of server(::CAAdapterServerType_t).
 */
typedef void (*CANFCExceptionCallback)(CAAdapterServerType_t type);

/**
 * Initialize Nfc server.
 *
 * @return  ::CA_STATUS_OK, if success or Appropriate error code.
 */
CAResult_t CAInitializeNfcServer();

/**
 * Starts Nfc server.
 *
 * @return  ::CA_STATUS_OK, if success or Appropriate error code.
 */
CAResult_t CANFCStartServer();

/**
 * Stop Nfc server.
 */
void CANFCStopServer();

/**
 * Set this callback for receiving data packets from peer devices.
 *
 * @param[in]  callback   Callback to be notified when data received.
 */
void CANFCSetPacketReceiveCallback(CANFCPacketReceivedCallback callback);

/**
 * Set this callback for receiving exception notifications.
 *
 * @param[in]  callback    Callback to be notified in case of exception.
 */
void CANFCSetExceptionCallback(CANFCExceptionCallback callback);

/**
 * API to send Data.
 *
 * @param[in]  endpoint    Remote End point.
 * @param[in]  data        Data to be send.
 * @param[in]  dataLength  Length of data in bytes.
 */
void CANFCSendData(const CAEndpoint_t *endpoint, const void *data, uint32_t dataLength);

/**
 * Get a list of CAInterface_t items.
 * @param[in] desiredIndex    Get interface information for the given index.
 * @return  List of CAInterface_t items if success, NULL pointer if failed.
 */
u_arraylist_t *CANFCGetInterfaceInformation(int desiredIndex);

/**
 * Set callback for error handling.
 *
 * @param[in] nfcErrorCallback    callback to notify error to the nfcadapter.
 */
void CANFCSetErrorHandleCallback(CANFCErrorHandleCallback nfcErrorCallback);

#ifdef __cplusplus
}
#endif

#endif /* CA_NFC_INTERFACE_H_ */
