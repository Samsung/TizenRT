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

#ifndef WIFI_SEMAPHORE_H
#define WIFI_SEMAPHORE_H

#include <semaphore.h>
#include "wifi_common.h"

/**
 * @file wifi_semaphore.h
 * @brief semaphore API
 */

#ifdef DOXYGEN
/**
 * @brief wifi_semaphore struct
 */
typedef vendor_semapore wifi_semaphore;
#else
typedef sem_t wifi_semaphore;
#endif

/**
 * @brief  create semaphore
 *
 * @param[in]  semaphore   :  wifi semaphore
 * @param[in]  init_value  :  initial value of the semaphore (resource size)
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_semaphore_create(wifi_semaphore *semaphore, unsigned int init_value);

/**
 * @brief acquire semaphore
 *
 * @param[in]  semaphore  :  wifi semaphore
 * @param[in]  time_out   :  set time out, WIFI_UTILS_NO_WAIT, WIFI_UTILS_FOREVER, or wait milliseconds
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 * @return WIFI_UTILS_TIMEOUT       :  occur timeout
 */
wifi_utils_result_e wifi_semaphore_acquire(wifi_semaphore *semaphore, int time_out);

/**
 * @brief releases semaphore
 *
 * @param[in]  semaphore  :  wifi semaphore
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_semaphore_release(wifi_semaphore *semaphore);

/**
 * @brief destroy semaphore
 *
 * @param[in]  semaphore :  wifi semaphore
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_semaphore_destroy(wifi_semaphore *semaphore);

#endif							//WIFI_SEMAPHORE_H
