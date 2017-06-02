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
 * This file contains the APIs related to the GATT Server functionalities.
 * Creation of the GattServer with the characteristics. Enabling the
 * advertisement and updating the characteristics for the response and
 * notifying the change of characteristcs for the request will be done here.
 * LE adapter will interact with this sub module.
 */

#ifndef TZ_BLE_SERVER_H_
#define TZ_BLE_SERVER_H_

#include <bluetooth.h>
#include <bluetooth_type.h>
#include <bluetooth_internal.h>
#ifdef BLE_MCD
#include <bluetooth_product.h>
#endif
#include "caadapterinterface.h"
#include "logger.h"
#include "cathreadpool.h"
#include "caleinterface.h"

#if defined(__TIZEN__) && defined(LE_ADAPTER) && defined(BLE_CUSTOM_ADVERTISE)
/**
 * Used to Start Server advertise send data
 */
CAResult_t CAsetServerAdvertisementData(const char *data, int length);


/**
 * Used to Start Server  send  response data
 */
CAResult_t CAsetServerSanResponseData(const char *data, int length);
#endif

/**
 * Used to initialize gatt server.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAInitLEGattServer();

/**
 * Used to de-initialize gatt service and destroy the GATT server.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CADeInitLEGattServer();

/**
 * Used to initialize all required mutex variables for GATT server implementation.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAInitGattServerMutexVariables();

/**
 * Used to start advertising.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CALEStartAdvertise();

/**
 * Used to start advertising with service UUID.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CALEStartAdvertiseImpl(const char *serviceUUID);

/**
 * Used to stop advertising.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CALEStopAdvertise();

/**
 * Used to terminate all required mutex variables for GATT server implementation.
 */
void CATerminateGattServerMutexVariables();

/**
 * Used to add new OIC service in GATT server using bt_gatt_add_service api and
 * internally store service path(outparam) in global variable.
 *
 * @param[in]  serviceUUID  unique identifier for BLE OIC service.
 *
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CAAddNewLEServiceInGattServer(const char *serviceUUID);

/**
 * Used to register the service in Gatt Server using bt_gatt_register_service api.
 *
 * @param[in] svcPath  unique identifier for BLE OIC service which is outparam of
 *                     bt_gatt_service_create api.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 * @retval  ::CA_STATUS_OK  Successful.
 * @retval  ::CA_STATUS_INVALID_PARAM  Invalid input arguments.
 * @retval  ::CA_STATUS_FAILED Operation failed.
 */
CAResult_t CARegisterLEServicewithGattServer(const bt_gatt_h svcPath);

/**
 * Used to add new characteristics(Read/Write) to the service in Gatt Server using
 * bt_gatt_add_characteristic api.
 * @param[in] svcPath         Service path to which this characteristic belongs to.
 * @param[in] charUUID        Gatt characteristic uuid.
 * @param[in] charValue       Gatt characteristic value.
 * @param[in] charValueLen    Characteristic value length.
 * @param[in] read            Boolean variable for checking whether read characteristics or
 *                            write characteristics.
 * @return  ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAAddNewCharacteristicsToGattServer(const bt_gatt_h svcPath, const char *charUUID,
                                               const char *charValue, int charValueLen, bool read);

/**
 * This is the callback which will be called when client update one of the characteristics
 * with data.
 * @param[in] remote_address The address of the remote device which requests a change
 * @param[in] server         The GATT server handle
 * @param[in] gatt_handle    The characteristic or descriptor's GATT handle which has an old value
 * @param[in] offset         The requested offset from where the @a gatt_handle value will be updated
 * @param[in] value          The new value
 * @param[in] len            The length of @a value
 * @param[in] user_data      The user data passed from the registration function
 */
#ifdef BLE_TIZEN_30
void CALEGattRemoteCharacteristicWriteCb(const char *remoteAddress, int request_id,
                                         bt_gatt_server_h server, bt_gatt_h gatt_handle,
                                         bool response_needed, int offset, const char *charValue,
                                         int charLen, void *userData);
#else
void CALEGattRemoteCharacteristicWriteCb(char *remoteAddress, bt_gatt_server_h server,
                                         bt_gatt_h gatt_handle, int offset, char *charValue,
                                         int charLen, void *userData);
#endif

/**
 * Setting the connection state changed callback.
 * @param[in] connStateCb      callback for receiving the changed network info.
 */
void CASetLEConnectionStateChangedCallback(CALEConnectionStateChangedCallback connStateCb);

/**
 * check connection status.
 * @param[in] address      the address of the remote device.
 * @return  true or false.
 */
bool CALEServerIsConnected(const char* address);

/**
 * get MTU size.
 * @param[in] address      the address of the remote device.
 * @return  mtu size negotiated from remote device.
 */
uint16_t CALEServerGetMtuSize(const char* address);

#endif /* TZ_BLE_SERVER_H_ */

