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
 * This file contains common function for connection manager arbiter
 */

#ifndef CA_CONNECTIONMANAGER_ARBITER_H_
#define CA_CONNECTIONMANAGER_ARBITER_H_

#include "cacommon.h"
#include "camanagerutil.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Initializes the Message Arbiter
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAMsgArbiterInitialize();

/**
 * Terminate the Message Arbiter
 */
CAResult_t CAMsgArbiterTerminate();

/**
 * Update Device Info. in managed data
 * @param[in]  endpoint       network address.
 * @param[in]  isCloud        with cloud or not .
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAMsgArbiterUpdateDeviceInfo(const CAEndpoint_t *endpoint, bool isCloud);

/**
 * Reset Device Info in managed data
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAMsgArbiterResetDeviceInfo();

/**
 * Get request/response message to send.
 * @param[in, out]  data     data to request or response.
 * @return ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAMsgArbiterGetMessageData(CAData_t *data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // CA_CONNECTIONMANAGER_ARBITER_H_


