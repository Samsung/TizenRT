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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Error Type of Result Value returned from Event Loop
 * @details If operation is failed, these defined values will be returned.
 */
enum el_result_error_e {
	EVENTLOOP_INVALID_PARAM = -1,
	EVENTLOOP_BUSY = -2,
	EVENTLOOP_OUT_OF_MEMORY = -3,
	EVENTLOOP_LOOP_FAIL = -4,
	EVENTLOOP_NOT_FINISHED = -5,
};

/**
 * @brief EventLoop Timer structure
 */
typedef uv_timer_t el_timer_t;

/**
 * @brief EventLoop Loop structure
 */
typedef uv_loop_t el_loop_t;

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
 * This API can create timer which is called once or repeated according to repeat flag. \n
 * The timer event is handled in the loop of its own task by calling eventloop_loop_run. \n
 * When eventloop_delete_timer is called, the timer is stopped if it is active and then used resources are freed
 * @remarks You should free used timer resources after using timer by calling @eventloop_delete_timer with returned pointer
 * @param[in] timeout interval in milliseconds from the current time of loop for calling callback function
 * @param[in] repeat the value which represents whether timer runs repeatly or not. \n
 *            You can stop the periodic timer by calling eventloop_delete_timer
 * @param[in] func the callback function to be called
 * @param[in] data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned. On failure, NULL is returned
 * @since TizenRT v2.0 PRE
 */
el_timer_t *eventloop_add_timer(unsigned int timeout, bool repeat, timeout_callback func, void *data);

/**
 * @brief Delete specific timer
 * @details @b #include <eventloop/eventloop.h> \n
 * The timer is stopped if it is active and all used resources for timer are freed
 * @param[in] timer A pointer of timer to be deleted
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.0 PRE
 */
int eventloop_delete_timer(el_timer_t *timer);

/**
 * @brief Create timer to call the function in the given period of time asynchronously
 * @details @b #include <eventloop/eventloop.h> \n
 * This API is same as eventloop_add_timer(), but it yields the event to evnet loop task.\n
 * So you don't need to call eventloop_loop_run() to run loop. \n
 * In other words, the timer event is added to a loop handled by eventloop task, \n
 * and then registered callback is executed in the context of eventloop task asynchronously, not task which created timer. \n
 * When eventloop_delete_timer is called, the timer is stopped if it is active and then used resources are freed
 * @remarks You should free used timer resources after using timer by calling @eventloop_delete_timer with returned pointer
 * @param[in] timeout interval in milliseconds from the current time of loop for calling callback function
 * @param[in] repeat the value which represents whether timer runs repeatly or not. \n
 *            You can stop the periodic timer by calling eventloop_delete_timer
 * @param[in] func the callback function to be called
 * @param[in] data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned. On failure, NULL is returned
 * @since TizenRT v2.0 PRE
 */
el_timer_t *eventloop_add_timer_async(unsigned int timeout, bool repeat, timeout_callback func, void *data);

/**
 * @brief Run the loop of its own task
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] None
 * @return On success, OK is returned. On failure, defined negative value is returned. \n
 *         If eventloop_loop_stop() was called and there are still active works, EVENTLOOP_NOT_FINISHED is returned
 * @since TizenRT v2.0 PRE
 */
int eventloop_loop_run(void);

/**
 * @brief Stop the loop of its own task
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] None
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.0 PRE
 */
int eventloop_loop_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
