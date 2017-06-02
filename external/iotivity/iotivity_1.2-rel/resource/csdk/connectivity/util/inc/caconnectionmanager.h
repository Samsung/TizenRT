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
 * Callback to send replaced data.
 * @param[in]   data    send data.
 */
typedef void (*CASendThreadFunc)(CAData_t *data);

/**
 * Callback to notify received data from the remote endpoint.
 * @param[in]   data    received data.
 */
typedef void (*CAReceiveThreadFunc)(CAData_t *data);

/**
 * Context of connection manager
 */
typedef struct
{
    /** send method for block data. **/
    CASendThreadFunc sendThreadFunc;

    /** callback function for received message. **/
    CAReceiveThreadFunc receivedThreadFunc;

    /** array list on which the thread is operating. **/
    u_arraylist_t *dataList;

    /** data list mutex for synchronization. **/
    oc_mutex dataListMutex;

    /** sender mutex for synchronization. **/
    oc_mutex dataSenderMutex;
} CAConnectionManagerContext_t;

/**
 * Initializes the connection manager context.
 * @param[in]  CASendThreadFunc    function point to add data in send queue thread.
 * @param[in]  CAReceiveThreadFunc function point to add data in receive queue thread.
 * @return ::CASTATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAInitializeConnectionManager(CASendThreadFunc blockSendMethod,
                                         CAReceiveThreadFunc receivedDataCallback);

/**
 * Terminate the connection manager context.
 */
void CATerminateConnectionManager();

/**
 * Initialize mutex.
 * @return ::CASTATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CAInitConnectionManagerMutexVariables();

/**
 * Terminate mutex.
 */
void CATerminateConnectionManagerMutexVariables();

/**
 * Get request/response message to send.
 */
CAData_t* CAGetConnectionManagerMessageData(CAData_t *data);

/**
 * Start connection manager.
 */
void CAStartConnectionManagerService(CMConfigureInfo_t info);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // CA_CONNECTIONMANAGER_H_
