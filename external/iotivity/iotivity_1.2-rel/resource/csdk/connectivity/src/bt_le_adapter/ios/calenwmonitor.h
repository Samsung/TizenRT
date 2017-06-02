/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
* This file contains the APIs for BT LE communications.
*/
#ifndef CA_LENWMONITOR_H_
#define CA_LENWMONITOR_H_

#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"
#include "caleinterface.h"
#include "caleutils.h"

#ifdef __cplusplus
extern "C"
{
#endif

void CALESetAdapterStateCallback(CALEDeviceStateChangedCallback callback);

void CALEClientNWStateChangeCallback(CALE_STATE_t status);

void CALEServerDisconnectCallback(const char *address);
void CALEClientDisconnectCallback(const char *address);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_LENWMONITOR_H_ */

