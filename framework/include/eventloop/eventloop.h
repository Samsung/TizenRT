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
	EVENTLOOP_OPERATION_FAIL = -6,
	EVENTLOOP_INVALID_HANDLE = -7,
};

/**
 * @brief Event type used in Event Loop
 * @details Event loop uses these values as event types. \n
 * If user wants to use new events, those should be defined ahead.\n
 * An example of a pre-defined events are shown below.\n
 * \n
 * enum el_event_type_e {\n
 *		EL_EVENT_WIFI_ON = 1,\n
 *		EL_EVENT_WIFI_OFF = 2,\n
 *		EL_EVENT_PRE_DEFINE = 3,\n
 *		EL_EVENT_MAX,\n
 * };
 */
enum el_event_type_e {
	EL_EVENT_WIFI_ON = 1,
	EL_EVENT_WIFI_OFF = 2,
	EL_EVENT_MAX,
};

/**
 * @brief Return value of callback function for event
 * @details The callback function should return EVENTLOOP_CALLBACK_STOP(false) or EVENTLOOP_CALLBACK_CONTINUE(true).\n
 * It decides whether continuing to execute callback function for the event or not.\n
 * If it returns EVENTLOOP_CALLBACK_STOP, receiving registered events is stopped.\n
 * The registered handler won't be executed from now on although receiving the event.\n
 * Otherwise, a callback function is called continuously.
 */
#define EVENTLOOP_CALLBACK_STOP 0
#define EVENTLOOP_CALLBACK_CONTINUE 1

/**
 * @brief EventLoop Timer structure
 */
typedef uv_timer_t el_timer_t;

/**
 * @brief EventLoop Loop structure
 */
typedef uv_loop_t el_loop_t;

/**
 * @brief EventLoop Event structure
 */
typedef uv_signal_t el_event_t;

/**
 * @brief EventLoop Timeout Callback
 */
typedef bool (*timeout_callback)(void *data);

/**
 * @brief EventLoop Thread Safe Callback
 */
typedef void (*thread_safe_callback)(void *data);

/**
 * @brief EventLoop Event Callback
 * This is specific type for callback function used in eventloop_add_event_handler. \n
 * When some tasks send event, registered callback functions are called with some data. \n
 * The first parameter, registered_cb_data is function data registered in eventloop_add_event_handler. \n
 * The second parameter, received_event_data is data received from event sender. \n
 */
typedef bool (*event_callback)(void *registered_cb_data, void *received_event_data);

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Create timer to call the function in the given period of time
 * @details @b #include <eventloop/eventloop.h> \n
 * This API can create timer which is called once or repeated according to repeat flag. \n
 * The timer event is handled in the loop of its own task by calling eventloop_loop_run. \n
 * All resources created for timer at this call is freed when timer is stopped. \n
 * So you don't have to call eventloop_delete_timer after it is stopped. \n
 * In case of repeated timer, it can be stopped by calling eventloop_delete_timer or by returning EVENTLOOP_CALLBACK_STOP in callback function. \n
 * In other case, timer is stopped after calling callback function once regardless of return value in callback function. \n
 * @remarks User should NOT free the data passed to callback function before callback function is finished. \n
 *          It means that user should free them in callback function before it returns EVENTLOOP_CALLBACK_STOP or after eventloop_loop_run()
 * @param[in] timeout interval in milliseconds from the current time of loop for calling callback function
 * @param[in] repeat the value which represents whether timer runs repeatedly or not. \n
 *            You can stop the periodic timer by calling eventloop_delete_timer
 * @param[in] func the callback function to be called
 *            In case of repeated timer, it should return EVENTLOOP_CALLBACK_STOP(false) or EVENTLOOP_CALLBACK_CONTINUE(true).
 * @param[in] cb_data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned. On failure, NULL is returned
 * @since TizenRT v2.1 PRE
 */
el_timer_t *eventloop_add_timer(unsigned int timeout, bool repeat, timeout_callback func, void *cb_data);

/**
 * @brief Delete specific timer
 * @details @b #include <eventloop/eventloop.h> \n
 * The timer is stopped if it is active and all used resources for timer are freed
 * @param[in] timer A pointer of timer to be deleted
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_delete_timer(el_timer_t *timer);

/**
 * @brief Create timer to call the function in the given period of time asynchronously
 * @details @b #include <eventloop/eventloop.h> \n
 * This API is same as eventloop_add_timer(), but it yields the event to event loop task.\n
 * So you don't need to call eventloop_loop_run() to run loop. \n
 * In other words, the timer event is added to a loop handled by eventloop task, \n
 * and then registered callback is executed in the context of eventloop task asynchronously, not task which created timer. \n
 * All resources created for timer at this call is freed when timer is stopped. \n
 * So you don't have to call eventloop_delete_timer after it is stopped. \n
 * In case of repeated timer, it can be stopped by calling eventloop_delete_timer or by returning EVENTLOOP_CALLBACK_STOP in callback function. \n
 * In other case, timer is stopped after calling callback function once. regardless of return value in callback function.
 * @remarks User should NOT free the data passed to callback function before callback function is finished. \n
 *          Therefore, user should free them in callback function before it returns EVENTLOOP_CALLBACK_STOP.
 * @param[in] timeout interval in milliseconds from the current time of loop for calling callback function
 * @param[in] repeat the value which represents whether timer runs repeatedly or not. \n
 *            You can stop the periodic timer by calling eventloop_delete_timer
 * @param[in] func the callback function to be called
 * @param[in] cb_data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned. On failure, NULL is returned
 * @since TizenRT v2.1 PRE
 */
el_timer_t *eventloop_add_timer_async(unsigned int timeout, bool repeat, timeout_callback func, void *cb_data);

/**
 * @brief Set event callback which will be called a certain event occur
 * @details @b #include <eventloop/eventloop.h> \n
 * This API is almost similar to task_manager_set_broadcast_cb. The difference between them is a context when callback function is executed. \n
 * In case of task manager, the callback function is executed by task manager if someone sends some events. \n
 * On the other hands, the callback functions of eventloop are called by polling the events in loop by itself. \n
 * So you should run loop by calling eventloop_loop_run to receive event after calling this API. \n
 * And you can continue or stop to execution of callback function depending on return value of callback function. \n
 * Please refer to a description of EVENTLOOP_CALLBACK_STOP and EVENTLOOP_CALLBACK_CONTINUE. \n
 * Finally when returned handle is not needed anymore or all works you want are done, you should call eventloop_del_event_handler. \n
 * Because some resources for event handler are allocated internally, you should free them.
 * @remarks User should NOT free the data passed to callback function before callback function is finished. \n
 *          It means that user should free them in callback function before it returns EVENTLOOP_CALLBACK_STOP or after eventloop_loop_run()
 * @param[in] type a value of event type
 * @param[in] func the callback function to be called \n
 *            It has specific type, event_callback which has two types of data as parameter. \n
 *            The first parameter is data registered in this call and the second parameter is data received from sender. \n
 *            And, it should return EVENTLOOP_CALLBACK_STOP(false) or EVENTLOOP_CALLBACK_CONTINUE(true).
 * @param[in] cb_data data to pass to func when func is called
 * @return On success,  A pointer of created event handle is returned. On failure, NULL is returned
 * @since TizenRT v2.1 PRE
 */
el_event_t *eventloop_add_event_handler(int type, event_callback func, void *cb_data);

/**
 * @brief Delete registered handler for event
 * @details @b #include <eventloop/eventloop.h> \n
 * The registered event handler is deleted and all used resources for event handler are freed.
 * @param[in] handle a pointer of event handle to be deleted
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_del_event_handler(el_event_t *handle);

/**
 * @brief Send an event
 * @details @b #include <eventloop/eventloop.h> \n
 * This API is almost similar to task_manager_broadcast.\n
 * The event will be sent with some data to tasks which registed handler for this event.\n
 * And then registered callback functions will be executed when they are polling events.
 * @param[in] type a value of event type
 * @param[in] event_data data to be passed to registered handler together
 * @param[in] data_size size of data
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_send_event(int type, void *event_data, int data_size);

/**
 * @brief Run the loop of its own task
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] None
 * @return On success, OK is returned. On failure, defined negative value is returned. \n
 *         If eventloop_loop_stop() was called and there are still active works, EVENTLOOP_NOT_FINISHED is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_loop_run(void);

/**
 * @brief Stop the loop of its own task
 * @details @b #include <eventloop/eventloop.h>
 * @param[in] None
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_loop_stop(void);

/**
 * @brief Callback is added at the each eventloop to be called thread safely.\n
 * @details @b #include <eventloop/eventloop.h>
 * @remarks User should NOT free the data passed to callback function before callback function is finished. \n
 *          It means that user should free them in callback function or free them after eventloop_loop_run()
 * @param[in] func the callback function to be called
 * @param[in] cb_data data to pass to func when func is called
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_thread_safe_function_call(thread_safe_callback func, void *cb_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
