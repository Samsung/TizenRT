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
 * This file contains common function for policy manager
 */

#ifndef CA_POLICY_MANAGER_H_
#define CA_POLICY_MANAGER_H_

#include <stdint.h>

#include "cathreadpool.h"
#include "octhread.h"
#include "uarraylist.h"
#include "cacommon.h"
#include "caconnectionmanager.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Set connection manager configure.
 * Target address and connectivity priority and speed level can be set in this method.
 * @param[in] info  configuration data for connection manager policy
 */
void CMSetConfigure(CMConfigureInfo_t info);

/**
 * get target address.
 * @return address
 */
const char* CMGetTargetAddress();

/**
 * get Adapter Type of current priority.
 * @return ::CATransportAdapter_t value.
 */
CATransportAdapter_t CMGetAdapterType();

/**
 * get speed of response level.
 * @return ::CMSpeedLevel_t value.
 */
CMSpeedLevel_t CMGetSpeedOfResponseLevel();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // CA_POLICY_MANAGER_H_
