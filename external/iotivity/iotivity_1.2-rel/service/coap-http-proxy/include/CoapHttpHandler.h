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
 * This file contains the functions to initiate request or response handling by CHP
 */

#ifndef COAP_HTTP_HANDLER_H_
#define COAP_HTTP_HANDLER_H_

#include "ocstack.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Initialize the CoAP HTTP Proxy.
 * @param[in]   secure      Set to advertise proxy as secured resource.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult CHPInitialize(bool secure);

/**
 * Terminate the CoAP HTTP Proxy.
 * @return  ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult CHPTerminate();

/**
 * API to check if CoAP-HTTP Proxy is initialized.
 * @return  true if initialized else false.
 */
bool CHPIsInitialized();

#ifdef __cplusplus
}
#endif

#endif
