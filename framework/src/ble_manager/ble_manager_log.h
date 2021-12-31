/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#pragma once

#include <errno.h>
/**
 * Logging
 */
#define BLE_TAG "[LE]"

#define BLE_LOG_VERBOSE printf
#define BLE_LOG_INFO printf
#define BLE_LOG_DEBUG(...)
#define BLE_LOG_ERROR printf

#define BLE_ERR												\
	do {													\
		BLE_LOG_ERROR(BLE_TAG"[ERR] %s: %d line err(%s)\n",	\
					 __FILE__, __LINE__, strerror(errno));	\
	} while (0)
