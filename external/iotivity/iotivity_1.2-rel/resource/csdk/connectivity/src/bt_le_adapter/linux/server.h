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

#ifndef CA_BLE_LINUX_SERVER_H
#define CA_BLE_LINUX_SERVER_H

#include "cacommon.h"

#include <stdint.h>
#include <sys/types.h>


/**
 * Send response/notification to the GATT client.
 *
 * Data will be sent to the client through the given response
 * @a characteristic proxy as a GATT characteristic notification.
 *
 * @param[in] characteristic The D-Bus proxy for the response
 *                           characteristic through which the
 *                           notification will be sent.
 * @param[in] data           The byte array to be sent.
 * @param[in] length         The number of elements in the byte
 *                           array.
 *
 * @return ::CA_STATUS_OK on success, ::CA_STATUS_FAILED otherwise.
 */
CAResult_t CAGattServerSendResponseNotification(
    char const * address,
    uint8_t const * data,
    size_t length);


/**
 * Send notification to all connected GATT clients.
 *
 * Data will be sent to the client through the given response
 * @a characteristic proxy as a GATT characteristic notification.
 *
 * @param[in] data    The byte array to be sent.
 * @param[in] length  The number of elements in the byte array.
 * @param[in] context Object containing mutexes and error reporting
 *                    callback used on failure to send.
 *
 * @return ::CA_STATUS_OK on success, ::CA_STATUS_FAILED otherwise.
 */
CAResult_t CAGattServerSendResponseNotificationToAll(
    uint8_t const * data,
    size_t length);


#endif  /* CA_BLE_LINUX_SERVER_H */
