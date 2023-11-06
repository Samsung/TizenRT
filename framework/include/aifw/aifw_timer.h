/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <time.h>
#include <semaphore.h>

/**
 * @brief This is a callback function called by timer everytime it expires. Interval at which timer expires is set in 'interval' field of structure timer.
 * Internally, it calls Collect Raw Data listener of application to collect raw data and pass it for inference.
*/
typedef void *(*aifw_timer_callback)(void *);

/**
 * @brief function result constant
 */
typedef enum {
	AIFW_TIMER_FAIL = -1,		/**<  fail				*/
	AIFW_TIMER_SUCCESS = 0,		/**<  success				*/
	AIFW_TIMER_INVALID_ARGS = 1,		/**<  invalid parameter (argument)	*/
} aifw_timer_result;

/**
 * @brief This structure defines member fields to store attributes of timer.
 * id: Timer ID returned by 'timer_create' API.
 * function: Callback function to be called when the timer expires.
 * function_args: Arguments of callback function.
 * interval: Time interval in milliseconds at which timer expires.
 * enable: Flag which tells whether timer has enabled(or started).
 * semaphore: Semaphore associated with timer.
*/
struct aifw_timer{
    timer_t id;
    aifw_timer_callback function;
    void *function_args;
    unsigned int interval;
    bool enable;
    sem_t semaphore;
    sem_t exitSemaphore;
    pthread_t timerThread;
};

/**
 * @brief initializes member fields of aifw_timer structure object pointed by parameter 'timer'
 * It doesnot auto-start the timer.
 *
 * @param[in] timer	  :  pointer to a aifw_timer structure object
 * @param[in] timer_function  :  callback function to call when the timer expires.
 * @param[in] function_args   :  callback function arguments
 * @param[in] interval        :  time interval in milliseconds at which timer will expire
 *
 * @return AIFW_TIMER_SUCCESS       :  success
 * @return AIFW_TIMER_FAIL          :  fail
 * @return AIFW_TIMER_INVALID_ARGS  :  input parameter invalid
*/
aifw_timer_result aifw_timer_create(aifw_timer *timer, void *timer_function, void *function_args, unsigned int interval);

/**
 * @brief starts the timer
 *
 * @param[in] timer  :  pointer to a aifw_timer structure object
 *
 * @return AIFW_TIMER_SUCCESS       :  success
 * @return AIFW_TIMER_FAIL          :  fail
 * @return AIFW_TIMER_INVALID_ARGS  :  input parameter invalid
*/
aifw_timer_result aifw_timer_start(aifw_timer *timer);

/**
 * @brief change timer interval
 *
 * regardless of timer status (start, stop)
 *
 * @param[in] timer     :  pointer to a aifw_timer structure objectaifw_
 * @param[in] interval  :  set interval time milliseconds
 *
 * @return AIFW_TIMER_SUCCESS       :  success
 * @return AIFW_TIMER_FAIL          :  fail
 * @return AIFW_TIMER_INVALID_ARGS  :  input parameter invalid
*/
aifw_timer_result aifw_timer_change_interval(aifw_timer *timer, unsigned int interval);

/**
 * @brief stop timer
 *
 * @param[in] timer  :  pointer to a aifw_timer structure object
 *
 * @return AIFW_TIMER_SUCCESS       :  successaifw_
 * @return AIFW_TIMER_FAIL          :  fail
 * @return AIFW_TIMER_INVALID_ARGS  :  input parameter invalid
*/
aifw_timer_result aifw_timer_stop(aifw_timer *timer);

/**
 * @brief destroy timer
 *
 * @param[in] timer	  :  pointer to a aifw_timer structure object
 *
 * @return AIFW_TIMER_SUCCESS       :  success
 * @return AIFW_TIMER_FAIL          :  fail
 * @return AIFW_TIMER_INVALID_ARGS  :  input parameter invalid
*/
aifw_timer_result aifw_timer_destroy(aifw_timer *timer);

