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

#include <stdbool.h>
#include <tinyara/wdog.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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
 * @brief Type of Handle Mode in Event Loop
 * @details User can set the mode of handle. \n
 * If a task or pthread add handle with EL_MODE_WAIT_DONE, it doesn't exit until they added handles are processed.
 */
enum el_handle_mode_e {
	EL_MODE_DEFAULT = 0,
	EL_MODE_WAIT_DONE = 1,
	EL_MODE_MAX,
};

/**
 * @brief EventLoop Timeout Callback
 */
typedef bool (*timeout_callback)(void *data);

/**
 * @brief EventLoop Event Callback
 * This is specific type for callback function used in eventloop_add_event_handler. \n
 * When some tasks send event, registered callback functions are called with some data. \n
 * The first parameter, registered_cb_data is function data registered in eventloop_add_event_handler. \n
 * The second parameter, received_event_data is data received from event sender. \n
 */
typedef bool (*event_callback)(void *cb_data, void *received_event_data);

/**
 * @brief EventLoop Timer structure
 */
struct el_timer_s {
	uint32_t timeout;
	bool repeat;
	timeout_callback cb_func;
	void *cb_data;
	struct wdog_s *wdog;
};
typedef struct el_timer_s el_timer_t;

/**
 * @brief EventLoop Event structure
 */
struct el_event_s {
	int event_type;
	event_callback cb_func;
	void *cb_data;
	void *event_data;
};
typedef struct el_event_s el_event_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/**
 * @brief Create timer to call the function in the given period of time
 * @details @b #include <eventloop/eventloop.h> \n
 * This API can create timer which is called once or repeated according to repeat flag. \n
 * Timer event is notified to task and callback function is executed in the context of task which created timer. \n
 * All resources created for timer at this call is freed when timer is stopped. \n
 * So you don't have to call eventloop_delete_timer after it is stopped. \n
 * In case of repeated timer, it can be stopped by calling eventloop_delete_timer or by returning EVENTLOOP_CALLBACK_STOP in callback function. \n
 * In other case, timer is stopped after calling callback function once regardless of return value in callback function. \n
 * @remarks User should NOT free the data passed to callback function before callback function is finished. \n
 *          It means that user should free them in callback function before it returns EVENTLOOP_CALLBACK_STOP or after eventloop_loop_run()
 * @param[in] timeout interval in milliseconds from the current time of loop for calling callback function
 * @param[in] repeat the value which represents whether timer runs repeatedly or not. \n
 *            You can stop the periodic timer by calling eventloop_delete_timer or returning EVENTLOOP_CALLBACK_STOP in callback function.
 * @param[in] func the callback function to be called
 *            In case of repeated timer, it should return EVENTLOOP_CALLBACK_STOP(false)
 * @param[in] cb_data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned. On failure, NULL is returned
 * @since TizenRT v2.1 PRE
 */
el_timer_t *eventloop_add_timer(uint32_t timeout, bool repeat, timeout_callback func, void *data);

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
 * Timer event is notified to eventloop task, and then registered callback is executed in the context of eventloop task asynchronously, not task which created timer. \n
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
 *            In case of repeated timer, it should return EVENTLOOP_CALLBACK_STOP(false) or EVENTLOOP_CALLBACK_CONTINUE(true).
 * @param[in] cb_data data to pass to func when func is called
 * @return On success, A pointer of created timer is returned. On failure, NULL is returned
 * @since TizenRT v2.1 PRE
 */
el_timer_t *eventloop_add_timer_async(unsigned int timeout, bool repeat, timeout_callback func, void *cb_data);

/**
 * @brief Set mode of timer
 * @details @b #include <eventloop/eventloop.h> \n
 * The mode of all timer is EL_MODE_DEFAULT. \n
 * A timer with EL_MODE_DEFAULT mode is valid when task/thread which registered this timer is alive. \n
 * In case of EL_MODE_WAIT_DONE, task/thread which registered this timer doesn't exit, and it waits for registered timers and processes them. \n
 * Finally it terminates when processing all registered events is done.
 * @param[in] timer a pointer of timer
 * @param[in] mode type of mode which is defined at enum el_handle_mode_e
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_set_timer_mode(el_timer_t *timer, int mode);

/**
 * @brief Set event callback which will be called a certain event occur
 * @details @b #include <eventloop/eventloop.h> \n
 * The Event is notified to task and callback function is executed in the context of task which registered event handler. \n
 * If someone sends events, it is notified to the tasks which registered handler and callback functions are executed in the each context. \n
 * And you can continue or stop to execution of callback function depending on return value of callback function. \n
 * Please refer to a description of EVENTLOOP_CALLBACK_STOP and EVENTLOOP_CALLBACK_CONTINUE. \n
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
 * @param[in] event_handler a pointer of event handle to be deleted
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_del_event_handler(el_event_t *event_handler);

/**
 * @brief Send an event
 * @details @b #include <eventloop/eventloop.h> \n
 * This API is almost similar to task_manager_broadcast.\n
 * The event will be sent with some data to tasks which registed handler for this event.\n
 * And then registered callback functions will be executed in a context of each task.
 * @param[in] type a value of event type
 * @param[in] event_data data to be passed to registered handler together
 * @param[in] data_size size of data
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_send_event(int type, void *event_data, int data_size);

/**
 * @brief Set mode of event handler
 * @details @b #include <eventloop/eventloop.h> \n
 * The mode of all event handler is EL_MODE_DEFAULT. \n
 * A event handler with EL_MODE_DEFAULT mode is valid when task/thread which registered this handler is alive. \n
 * In case of EL_MODE_WAIT_DONE, task/thread which registered this handler doesn't exit, waits for registered events and processes them. \n
 * Finally it terminates when processing all registered events is done.
 * @param[in] event_handler a pointer of event handle
 * @param[in] mode type of mode which is defined at enum el_handle_mode_e
 * @return On success, OK is returned. On failure, defined negative value is returned
 * @since TizenRT v2.1 PRE
 */
int eventloop_set_event_handler_mode(el_event_t *event_handler, int mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
