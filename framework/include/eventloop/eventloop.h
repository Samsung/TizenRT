/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/**
 * @defgroup Eventloop Eventloop
 * @ingroup Eventloop
 * @brief Provides APIs for Eventloop
 * @{
 */
/**
 * @file eventloop/eventloop.h
 */
#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <libtuv/uv.h>
#include <stdbool.h>

/**
 * @brief Error Type of Result Value returned from Event Loop
 * @details If operation is failed, these defined values will be returned.
 */
enum el_result_error_e {
	EVENTLOOP_INVALID_PARAM = -1,
	EVENTLOOP_BUSY = -2,
	EVENTLOOP_OUT_OF_MEMORY = -3,
};

/**
 * @brief EventLoop Timer structure
 */
typedef uv_timer_t el_timer_t;

/**
 * @brief EventLoop Timeout Callback
 */
typedef void (*timeout_callback)(void *);

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Create timer to call the function in the given period of time
 * @details @b #include <eventloop/eventloop.h> \n
 * This API can create timer which is called once or repeated according to repeat flag
 * @param[in] timeout interval in milliseconds from the current time of loop for calling callback function
 * @param[in] repeat the value which represents whether timer runs repeatly or not. \n
 *            You can stop the periodic timer by calling eventloop_delete_timer
 * @param[in] func the callback function to be called
 * @param[in] data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned if 'repeat' is true. \n
           You should free used timer resources after using timer by calling eventloop_delete_timer with returned pointer. \n
           On the other hand, NULL is returned if 'repeat' is false. \n
           So you don't have to do anything because all resources are freed internally after callback execution. \n
 *         On failure, NULL is returned
 * @since TizenRT v2.0 PRE
 */
el_timer_t *eventloop_add_timer(unsigned int timeout, bool repeat, timeout_callback func, void *data);

/**
 * @brief Delete specific timer
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] timer A pointer of timer to be deleted
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.0 PRE
 */
int eventloop_delete_timer(el_timer_t *timer);

/**
 * @brief Run default loop
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] None
 * @return None
 * @since TizenRT v2.0 PRE
 */
void eventloop_loop_run(void);

/**
 * @brief Stop default loop
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] None
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.0 PRE
 */
int eventloop_loop_stop(void);
#endif
/**
 * @}
 */
