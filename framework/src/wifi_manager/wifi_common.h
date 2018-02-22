/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef WIFI_COMMON_H
#define WIFI_COMMON_H


/**
 * @brief time out option (used by message queue, uart, semaphore, mutex)
 *
 */
typedef enum {
	WIFI_UTILS_NO_WAIT = 0,   /**<  no wait contant          */
	WIFI_UTILS_FOREVER = -1,  /**<  wait until job finished  */
} wifi_utils_timeout_option;

/**
 * @brief <b> wifi result type FAIL, SUCCESS, INVALID ARGS</b>
 */ 
typedef enum {
	WIFI_UTILS_FAIL = -1,
	WIFI_UTILS_SUCCESS,
	WIFI_UTILS_INVALID_ARGS,
	WIFI_UTILS_TIMEOUT,
	WIFI_UTILS_BUSY,
	WIFI_UTILS_FILE_ERROR,
	WIFI_UTILS_ALREADY_CONNECTED,
} wifi_utils_result_e;


#endif //WIFI_COMMON_H
