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
 * This file provides APIs for BLE modules.
 */

#ifndef CA_LE_INTERFACE_H_
#define CA_LE_INTERFACE_H_

#include <stdbool.h>

#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Provide info about different mode of data transfer.
 *
 * This enum is used to differentiate between unicast and multicast
 * data transfer.
 */
typedef enum
{
    LE_MULTICAST,    /**< When this enum is selected, data will be updated to all OIC servers. */
    LE_UNICAST       /**< When this enum is selected, data will be updated to desired OIC Server. */
} CALETransferType_t;

/**
 * Stores the information of the Data to be sent from the queues.
 *
 * This structure will be pushed to the sender/receiver queue for
 * processing.
 */
typedef struct
{
    /// Remote endpoint contains the information of remote device.
    CAEndpoint_t *remoteEndpoint;

    /// Data to be transmitted over LE transport.
    uint8_t *data;

    /// Length of the data being transmitted.
    uint32_t dataLen;

    /// Sender information list
    u_arraylist_t * senderInfo;
} CALEData_t;

/**
 * The MTU supported for BLE adapter
 */
#define CA_DEFAULT_BLE_MTU_SIZE  20

/**
 * The MTU supported for BLE spec
 */
#define CA_SUPPORTED_BLE_MTU_SIZE  517

/**
 * The Header of the MTU supported for BLE spec
 */
#define CA_BLE_MTU_HEADER_SIZE  5

/**
 * This will be used to notify device status changes to the LE adapter layer.
 * @param[in]  adapter_state State of the adapter.
 */
typedef void (*CALEDeviceStateChangedCallback)(CAAdapterState_t adapter_state);

/**
 * This will be used to notify device connection state changes to the LE adapter layer.
 * @param[in]   adapter        Transport type information.
 * @param[in]   remoteAddress  Endpoint object from which the connection status is changed.
 * @param[in]   connected      State of connection.
 */
typedef void (*CALEConnectionStateChangedCallback)(CATransportAdapter_t adapter,
                                                   const char *remoteAddress, bool connected);

/**
 * Notify the adapter layer that a packet was received from the GATT
 * peer.
 *
 * @param[in]  remoteAddress  Remote endpoint Address.
 * @param[in]  data           Data received.
 * @param[in]  dataLength     Length of the data received.
 * @param[in]  sentLength     Length of the data sent.
 *
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
typedef CAResult_t (*CABLEDataReceivedCallback)(const char *remoteAddress,
                                                const uint8_t *data,
                                                uint32_t dataLength,
                                                uint32_t *sentLength);

/**
 * Initialize the LE adapter layer. This will be invoked from the CA
 * layer.
 *
 * @param[in] threadPool Thread pool handle
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAInitializeLEAdapter();

/**
 * Start the LE adapter layer.
 *
 * This function will be invoked from the CA layer when the LE
 * "network" is selected via @c CASelectNetwork().  It gives an
 * opportunity for LE adapter implementations to perform operations
 * before starting a GATT client or server.  Most LE adapter
 * implementations will simply implement this function as no-op.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 */
CAResult_t CAStartLEAdapter();

/**
 * Stop the LE adapter layer.
 *
 * This function will be invoked from the CA layer when the LE
 * "network" is unselected via @c CAUnselectNetwork().  It gives an
 * opportunity for LE adapter implementations to perform operations
 * after stopping a GATT client or server.  Most LE adapter
 * implementations will simply implement this function as no-op.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 */
CAResult_t CAStopLEAdapter();

/**
 * Used to get the current state of the LE adapter.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_ADAPTER_NOT_ENABLED  adapter not enabled
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAGetLEAdapterState();

/**
 * Initialize the network monitor layer of the LE adapter.  Mutex
 * variables required to operate in this layer and other parameters
 * can be initialized in this function.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAInitializeLENetworkMonitor();

/**
 * Terminate the network monitor layer of the LE adapter. The
 * variables initialized in @c CAInitializeLENetworkMonitor() must be
 * cleared in this function.
 */
void CATerminateLENetworkMonitor();

/**
 * Set the callback for the device state changes in the adapter.
 *
 * @param[in] callback Callback to notify the Device state change to
 *            the CA Layer
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CASetLEAdapterStateChangedCb(CALEDeviceStateChangedCallback callback);

/**
 * Set the callback for the device connection state changes.
 *
 * @param[in] callback Callback to notify the Device connection state change to
 *            the CA Layer
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CASetLENWConnectionStateChangedCb(CALEConnectionStateChangedCallback callback);

/**
 * Provides the MAC address of the local Bluetooth adapter.
 *
 * @param[out] local_address Pointer to the location where bd address
 *                           needs to be stored.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAGetLEAddress(char **local_address);

/**
 * Start Gatt Server thread for service creation and advertise BLE
 * service.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAStartLEGattServer();

/**
 * Stop BLE Gatt Service.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAStopLEGattServer();

/**
 * initialize gatt server
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAInitializeLEGattServer();

/**
 * Stop Gatt Server thread and remove service registration, stop
 * advertising.
 */
void CATerminateLEGattServer();

/**
 * Used to store upper layer callback locally which will be used to
 * send the data to application.
 *
 * @param[in] callback Callback function to pass the data to CA layer.
 */
void CASetLEReqRespServerCallback(CABLEDataReceivedCallback callback);

/**
 * Update characteristics(Read/Write) value that we want to send to
 * particular client.
 *
 * @param[in] address  BD address of Gatt client
 * @param[in] value    Data that we want to send to client(unicast)
 * @param[in] valueLen Length of the data.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAUpdateCharacteristicsToGattClient(const char *address,
                                               const uint8_t *value,
                                               uint32_t valueLen);

/**
 * Update characteristics(Read/Write) value that we want to multicast
 * to all clients.
 *
 * @param[in] value    Data that we want to send to clients(multicast)
 * @param[in] valueLen Length of the data.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAUpdateCharacteristicsToAllGattClients(const uint8_t *value,
                                                   uint32_t valueLen);

/**
 * Start @c CAStartBleGattClientThread for initializing Gatt Client.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAStartLEGattClient();

/**
 * Stop Gatt client gracefully.  In turn it will call the
 * @c CATerminateBLEGattClient function.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
void CAStopLEGattClient();

/**
 * initialize Client
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAInitializeLEGattClient();

/**
 * Unset all the callbacks and stop service discovery
 */
void CATerminateLEGattClient();

/**
 * Read the data from characteristics and invoke notify callback.
 */
void CACheckLEData();

/**
 * Set the value of characteristic and update the value to
 * GATTServer (unicast).
 *
 * @param[in] remoteAddress The address of the remote device
 * @param[in] data          The value of characteristic (byte array)
 * @param[in] dataLen       The length of value
 * @param[in] type          Type of the transfer(::CALETransferType_t)
 * @param[in] position      The unique index of each ble server. Used
 *                          for multicast feature.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t  CAUpdateCharacteristicsToGattServer(const char *remoteAddress,
                                                const uint8_t *data,
                                                uint32_t dataLen,
                                                CALETransferType_t type,
                                                int32_t position);

/**
 * Set the value of characteristic and update the value to all
 * registered GATTServer (multicast).
 *
 * @param[in]  data    The value of characteristic (byte array)
 * @param[in]  dataLen The length of value
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
CAResult_t CAUpdateCharacteristicsToAllGattServers(const uint8_t *data,
                                                   uint32_t dataLen);

/**
 * Store upper layer callback locally which will be used to send the
 * data to application.
 *
 * @param[in] callback Callback function to pass the data to CA layer.
 */
void CASetLEReqRespClientCallback(CABLEDataReceivedCallback callback);

/**
 * Set the server thread pool handle which is required for spawning
 * new thread.
 *
 * @param[in] handle Thread pool handle which is given by above layer
 *                   for using thread creation task.
 *
 * @return ::CA_STATUS_OK or Appropriate error code
 * @retval ::CA_STATUS_OK  Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED Operation failed
 */
void CASetLEServerThreadPoolHandle(ca_thread_pool_t handle);

/**
* Set the client thread pool handle which is required for spawning new
* thread.
*
* @param[in] handle Thread pool handle which is given by above layer
*                   for using thread creation task.
*/
void CASetLEClientThreadPoolHandle(ca_thread_pool_t handle);

/**
 * Unset the callback of adapter state change.
 *
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAUnSetLEAdapterStateChangedCb();

/**
 * Unset the callback of adapter connection state change.
 *
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAUnSetLENWConnectionStateChangedCb();

/**
 * This will be used to notify errors in BLE adapter.
 *
 * @param[in] remoteAddress Remote endpoint Address
 * @param[in] data          Data received
 * @param[in] dataLength    Length of the data received
 * @param[in] result        error code as per CAResult_t
 */
typedef void (*CABLEErrorHandleCallback)(const char *remoteAddress,
                                         const uint8_t *data,
                                         uint32_t dataLength,
                                         CAResult_t result);
/**
 * Set the client error handler callback.
 *
 * @param[in] callback Callback function to update error to the
 *                     adapter.
 */
void CASetBLEClientErrorHandleCallback(CABLEErrorHandleCallback callback);


/**
 * Set the server error handler callback.
 *
 * @param[in] callback Callback function to update error to the
 *                     adapter.
 */
void CASetBLEServerErrorHandleCallback(CABLEErrorHandleCallback callback);

/**
 * This is the callback which will be called whenever there is change in gatt connection
 * with Client(Connected/Disconnected).
 *
 * @param[in]  connected      State of connection.
 * @param[in]  remoteAddress  Mac address of the remote device in which we made connection.
 */
void CALEGattServerConnectionStateChanged(bool connected, const char *remoteAddress);

/**
 * This is the callback which will be called whenever there is change in gatt connection
 * with server(Connected/Disconnected)
 *
 * @param[in]  connected     State of connection
 * @param[in]  remoteAddress Mac address of the remote device in which we made connection.
 */
void CALEGattConnectionStateChanged(bool connected, const char *remoteAddress);

#ifdef __cplusplus
}
#endif

#endif /* CA_LE_INTERFACE_H_ */
