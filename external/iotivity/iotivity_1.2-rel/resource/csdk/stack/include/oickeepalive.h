/* ****************************************************************
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
 * This file contains the APIs for KeepAlive Mechanism.
 * In order to ensure that the connection between an OIC Devices,
 * when using CoAP over TCP, OIC Device should send application layer
 * KeepAlive messages.
 */
#ifndef KEEP_ALIVE_H_
#define KEEP_ALIVE_H_

#include "octypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * This function discovers on a /oic/ping resource.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param remoteAddr        The target device address to discovery a resource.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          discovery or resource interaction is received. The discovery could be
 *                          related to filtered/scoped/particular resource. The callback is
 *                          generated for each response received.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OCFindKeepAliveResource(OCDoHandle *handle, const char *remoteAddr,
                                      OCCallbackData *cbData);

/**
 * Send ping message to remote endpoint.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param remoteAddr        The target device address to discovery a resource.
 * @param payload           Encoded request payload.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          discovery or resource interaction is received. The discovery could be
 *                          related to filtered/scoped/particular resource. The callback is
 *                          generated for each response received.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OCSendKeepAliveRequest(OCDoHandle *handle, const char *remoteAddr,
                                     OCPayload *payload, OCCallbackData *cbData);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // KEEP_ALIVE_H_
