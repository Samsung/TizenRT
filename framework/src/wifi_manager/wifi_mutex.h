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

#ifndef WIFI_MUTEX_H
#define WIFI_MUTEX_H
#include <semaphore.h>
#include "wifi_common.h"
typedef sem_t wifi_mutex;

/**
 * @brief create mutex
 *
 * @param[in]  mutex  :  wifi_mutex
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_mutex_create(wifi_mutex *mutex);

/**
 * @brief acquire the lock a mutex
 *
 * @param[in]  mutex     :  wifi_mutex
 * @param[in]  time_out  :  set time out, WIFI_UTILS_NO_WAIT, WIFI_UTILS_FOREVER, or wait milliseconds
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 * @return WIFI_UTILS_TIMEOUT       :  occur timeout
  */
wifi_utils_result_e wifi_mutex_acquire(wifi_mutex *mutex, int time_out);

/**
 * @brief release the lock on a mutex
 *
 * @param[in]  mutex  :  wifi_mutex
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
  */
wifi_utils_result_e wifi_mutex_release(wifi_mutex *mutex);

/**
 * @brief destroy a mutex
 *
 * @param[in]  mutex  :  wifi_mutex
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_mutex_destroy(wifi_mutex *mutex);

#endif							//WIFI_MUTEX_H
