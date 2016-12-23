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
#define KEEPALIVE_RESOURCE_INTF_NAME "oic.if.rw"

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
 * @param[in]   endPoint        RemoteEndpoint which sent the packet.
 * @param[in]   requestInfo     Received coap packet.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult HandleKeepAliveRequest(const CAEndpoint_t *endPoint,
                                     const CARequestInfo_t *requestInfo);

/**
 * API to handle the connected device for KeepAlive.
 * @return  Current Time.
 */
void HandleKeepAliveConnCB(const CAEndpoint_t *endpoint);

/**
 * API to handle the disconnected device for KeepAlive.
 * @return  Current Time.
 */
void HandleKeepAliveDisconnCB(const CAEndpoint_t *endpoint);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // KEEP_ALIVE_H_
