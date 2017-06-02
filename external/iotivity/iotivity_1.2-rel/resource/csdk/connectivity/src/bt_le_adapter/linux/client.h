/******************************************************************
 *
 * Copyright 2015 Intel Corporation All Rights Reserved.
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

#ifndef CA_BLE_LINUX_CLIENT_H
#define CA_BLE_LINUX_CLIENT_H

#include "context.h"


/**
 * Initialize the GATT client.
 *
 * @param[in] context
 *
 * @return @c CA_STATUS_OK on success, @c CA_STATUS_FAILED otherwise.
 */
CAResult_t CAGattClientInitialize(CALEContext * context);

/**
 * Destroy the GATT client.
 */
void CAGattClientDestroy();

/**
 * Send request data through a single user-specified BLE connection.
 *
 * @param[in] address  MAC address of the BLE peripheral running the
 *                     OIC Transport Profile GATT server to which the
 *                     data will be sent.
 * @param[in] data     Octet array of request data to be sent.
 * @param[in] length   Length of the @a data octet array.
 * @param[in] context  Object containing mutexes and error reporting
 *                     callback used on failure to send.
 *
 * @return @c CA_STATUS_OK on success, @c CA_STATUS_FAILED otherwise.
 */
CAResult_t CAGattClientSendData(char const * address,
                                uint8_t const * data,
                                size_t length,
                                CALEContext * context);

/**
 * Send request data through all BLE connections.
 *
 * Send the @a data to the GATT server found in all discovered LE
 * peripherals.
 *
 * @param[in] data    Octet array of request data to be sent.
 * @param[in] length  Length of the @a data octet array.
 * @param[in] context Object GATT request characteristic map.
 *
 * @return @c CA_STATUS_OK on success, @c CA_STATUS_FAILED otherwise.
 */
CAResult_t CAGattClientSendDataToAll(uint8_t const * data,
                                     size_t length,
                                     CALEContext * context);


#endif  /* CA_BLE_LINUX_CLIENT_H */
