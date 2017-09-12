/* ****************************************************************
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
 * This file contains the APIs for KeepAlive Mechanism.
 * In order to ensure that the connection between an OIC Devices,
 * when using CoAP over TCP, OIC Device should send application layer
 * KeepAlive messages.
 */
#ifndef KEEP_ALIVE_H_
#define KEEP_ALIVE_H_

#include "octypes.h"
#include "ocserverrequest.h"
#include "ocresource.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Name of resource type.
 */
#define KEEPALIVE_RESOURCE_TYPE_NAME "oic.wk.ping"

/**
 * Name of resource interface.
 */
#define KEEPALIVE_RESOURCE_INTF_NAME OC_RSRVD_INTERFACE_READ_WRITE

/**
 * URI of the resource.
 */
#define KEEPALIVE_RESOURCE_URI "/oic/ping"

/**
 * Initialize the KeepAlive.
 * @param[in]   mode        Host mode of operation.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult InitializeKeepAlive(OCMode mode);

/**
 * Terminates the KeepAlive.
 * @param[in]   mode        Host mode of operation.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult TerminateKeepAlive(OCMode mode);

/**
 * Process the KeepAlive timer to send ping message to OIC Server.
 */
void ProcessKeepAlive();

/**
 * This API will be called from RI layer whenever there is a request for KeepAlive.
 * Virtual Resource.
 * @param[in]   request     Received request information.
 * @param[in]   resource    Resource handle used for sending the response.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult HandleKeepAliveRequest(OCServerRequest *request,
                                     const OCResource *resource);

/**
 * API to handle the connected device for KeepAlive.
 * @param[in]   endpoint        Remote endpoint information.
 * @param[in]   isConnected     Whether keepalive message needs to be sent.
 * @param[in]   isClient        Host Mode of Operation.
 */
void HandleKeepAliveConnCB(const CAEndpoint_t *endpoint, bool isConnected, bool isClient);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // KEEP_ALIVE_H_
