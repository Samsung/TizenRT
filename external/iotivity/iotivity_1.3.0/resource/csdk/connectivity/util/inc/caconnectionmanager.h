/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * This file contains common function for connection manager
 */

#ifndef CA_CONNECTIONMANAGER_H_
#define CA_CONNECTIONMANAGER_H_

#include <stdint.h>

#include <coap/coap.h>
#include "cathreadpool.h"
#include "octhread.h"
#include "logger.h"
#include "uarraylist.h"
#include "cacommon.h"
#include "caprotocolmessage.h"
#include "camessagehandler.h"
#include "cautilinterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Initializes the connection manager context.
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CACMInitialize();

/**
 * Terminate the connection manager context.
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CACMTerminate();

/**
 * Get request/response message to send.
 * @param[in,out]  data     data to request or response.
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CACMGetMessageData(CAData_t *data);

/**
 * Set CM User Configuration
 * @param[in]  connPriority     value of CAConnectUserPref_t
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CACMSetConnUserConfig(CAConnectUserPref_t connPrefer);

/**
 * Get CM User Configuration
 * @param[out]  connPriority     value of CAConnectUserPref_t
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CACMGetConnUserConfig(CAConnectUserPref_t *connPrefer);

/**
 * Update Remote Device Info.
 * @param[in]  endpoint       network address.
 * @param[in]  isCloud        with cloud or not .
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CACMUpdateRemoteDeviceInfo(const CAEndpoint_t *endpoint, bool isCloud);

/**
 * Reset Remote Device Info.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CACMResetRemoteDeviceInfo();

/**
 * Set callback to handle connection status
 * @param[in]  callback       Callback function type for adapter status changes delivery.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CACMSetIPConnectionCallback(CAAdapterStateChangedCB callback);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // CA_CONNECTIONMANAGER_H_
