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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <eventloop/eventloop.h>

#include "tc_common.h"

#define EL_INVALID_EVENT -1

#define EL_TIMER_DATA            "timer"
#define EL_REPEAT_TIMER_DATA     "repeat_timer"
#define EL_THREAD_SAFE_DATA      "thread_safe"

#define EL_CB_DATA               "EVENT CB DATA"
#define EL_WIFI_ON_DATA          "HELLO WIFI-ON"
#define EL_WIFI_OFF_DATA         "HELLO WIFI-OFF"

#define EL_SEND_COUNT 5
#define EL_WIFI_ON_COUNT 3

static int el_timer_flag;
static int el_thread_safe_flag;
static bool loop_stop_flag;
static bool el_event_wifi_off_flag;

static int el_event_wifi_on_cnt;
static int send_cnt;

static el_timer_t *g_repeat_timer;

static void timer_cb(void *data)
{
	printf("Timer callback\n");
	if (strncmp((char *)data, EL_TIMER_DATA, strlen(EL_TIMER_DATA) + 1) == 0) {
		el_timer_flag++;
	}
}

static bool repeat_timer_cb(void *data)
{
	printf("Repeat timer callback\n");
	el_timer_flag++;
	return EVENTLOOP_CALLBACK_CONTINUE;
}

static void repeat_timer_stop_cb(void *data)
{
	int ret;

	ret = eventloop_delete_timer(g_repeat_timer);
	if (ret == OK) {
		printf("Stop the repeat timer SUCCESS!\n");
	} else {
		printf("Stop the repeat timer FAIL!\n");
	}
}

static bool repeat_timer_3times_cb(void *data)
{
	static int cnt = 0;

	printf("Repeat timer 3 times callback %d\n", ++cnt);
	if (strncmp((char *)data, EL_REPEAT_TIMER_DATA, strlen(EL_REPEAT_TIMER_DATA) + 1) == 0) {
		el_timer_flag++;
	}

	if (cnt >= 3) {
		printf("Calling count is %d, it will be stopped by itself.\n", cnt);
		cnt = 0;
		return EVENTLOOP_CALLBACK_STOP;
	}

	return EVENTLOOP_CALLBACK_CONTINUE;
}

static void utc_eventloop_loop_run_p(void)
{
	int ret;

	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);

	TC_SUCCESS_RESULT();
}

static void loop_stop_cb(void *data)
{
	int ret;

	printf("Loop will be stopped!\n");
	ret = eventloop_loop_stop();
	if (ret == OK) {
		loop_stop_flag = true;
	}
}

static void utc_eventloop_loop_stop_p(void)
{
	int ret;
	el_timer_t *timer;

	loop_stop_flag = false;
	timer = eventloop_add_timer(3000, false, (timeout_callback)loop_stop_cb, NULL);
	TC_ASSERT_NEQ("eventloop_add_timer", timer, NULL);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_loop_stop", loop_stop_flag, true);

	/* Try to stop already stopped loop. It will be return EVENTLOOP_LOOP_FAIL. */
	ret = eventloop_loop_stop();
	TC_ASSERT_EQ("eventloop_loop_stop", ret, EVENTLOOP_LOOP_FAIL);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_add_timer_n(void)
{
	el_timer_t *timer;

	timer = eventloop_add_timer(1000, false, NULL, NULL);
	TC_ASSERT_EQ("eventloop_add_timer", timer, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_add_timer_p(void)
{
	int ret;
	el_timer_t *timer;
	el_timer_t *repeat_timer;
	el_timer_t *loop_stop_timer;

	el_timer_flag = 0;
	timer = eventloop_add_timer(0, false, (timeout_callback)timer_cb, EL_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer", timer, NULL);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_add_timer", el_timer_flag, 1);

	/* Add timer, it is stopped by returning EVENTLOOP_CALLBACK_STOP in callback function after 3 times. */
	el_timer_flag = 0;
	repeat_timer = eventloop_add_timer(1000, true, repeat_timer_3times_cb, EL_REPEAT_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer", repeat_timer, NULL);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_add_timer", el_timer_flag, 3);

	/* Add timer which repeats forever, it is stopped by calling eventloop_delete_timer. */
	el_timer_flag = 0;
	g_repeat_timer = eventloop_add_timer(1000, true, repeat_timer_cb, EL_REPEAT_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer", g_repeat_timer, NULL);
	/* Add timer which will stop above repeated timer, g_repeat_timer after 5s. */
	timer = eventloop_add_timer(5000, false, (timeout_callback)repeat_timer_stop_cb, EL_REPEAT_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer", timer, NULL);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_add_timer", el_timer_flag, 4);

	/* Add timer which repeats forever, it is stopped by calling eventloop_loop_stop. */
	el_timer_flag = 0;
	repeat_timer = eventloop_add_timer(2000, true, repeat_timer_cb, EL_REPEAT_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer", repeat_timer, NULL);
	loop_stop_timer = eventloop_add_timer(3000, false, (timeout_callback)loop_stop_cb, NULL);
	TC_ASSERT_NEQ("eventloop_add_timer", loop_stop_timer, NULL);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_add_timer", el_timer_flag, 1);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_delete_timer_n(void)
{
	int ret;

	ret = eventloop_delete_timer(NULL);
	TC_ASSERT_EQ("eventloop_delete_timer", ret, EVENTLOOP_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_delete_timer_p(void)
{
	el_timer_t *timer;
	int ret;

	timer = eventloop_add_timer(0, false, (timeout_callback)timer_cb, NULL);
	TC_ASSERT_NEQ("eventloop_add_timer", timer, NULL);
	ret = eventloop_delete_timer(timer);
	TC_ASSERT_EQ("eventloop_delete_timer", ret, OK);

	/* Try to delete already deleted timer, it will be return EVENTLOOP_INVALID_HANDLE. */
	ret = eventloop_delete_timer(timer);
	TC_ASSERT_EQ("eventloop_delete_timer", ret, EVENTLOOP_INVALID_HANDLE);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_add_timer_async_n(void)
{
	el_timer_t *timer;

	timer = eventloop_add_timer_async(1000, false, NULL, NULL);
	TC_ASSERT_EQ("eventloop_add_timer_async", timer, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_add_timer_async_p(void)
{
	int sleep_cnt;
	el_timer_t *timer;
	el_timer_t *repeat_timer;

	el_timer_flag = 0;
	timer = eventloop_add_timer_async(0, false, (timeout_callback)timer_cb, EL_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer_async", timer, NULL);
	sleep_cnt = 0;
	while (sleep_cnt < 5) {
		sleep(1);
		sleep_cnt++;
		if (el_timer_flag == 1) {
			break;
		}
	}
	TC_ASSERT_EQ("eventloop_add_timer_async", el_timer_flag, 1);

	el_timer_flag = 0;
	repeat_timer = eventloop_add_timer_async(1000, true, repeat_timer_3times_cb, EL_REPEAT_TIMER_DATA);
	TC_ASSERT_NEQ("eventloop_add_timer_async", repeat_timer, NULL);
	sleep_cnt = 0;
	while (sleep_cnt < 5) {
		sleep(1);
		sleep_cnt++;
		if (el_timer_flag == 3) {
			break;
		}
	}
	TC_ASSERT_EQ("eventloop_add_timer_async", el_timer_flag, 3);

	TC_SUCCESS_RESULT();
}

static int wifi_on_callback(void *cb_data, void *event_data)
{
	static int cnt = 0;

	printf("WIFI ON CALLBACK!! %d\n", ++cnt);

	if (cb_data == NULL && event_data != NULL && strncmp(event_data, EL_WIFI_ON_DATA, sizeof(EL_WIFI_ON_DATA)) == 0) {
		el_event_wifi_on_cnt++;
	}

	if (cnt >= EL_WIFI_ON_COUNT) {
		cnt = 0;
		return EVENTLOOP_CALLBACK_STOP;
	}

	return EVENTLOOP_CALLBACK_CONTINUE;
}

static int wifi_on_once_callback(void *cb_data, void *event_data)
{
	printf("WIFI ON ONCE CALLBACK!!\n");

	return EVENTLOOP_CALLBACK_STOP;
}

static void wifi_off_callback(void *cb_data, void *event_data)
{
	int ret;

	printf("WIFI OFF CALLBACK!!\n");

	if (cb_data != NULL && strncmp(cb_data, EL_CB_DATA, sizeof(EL_CB_DATA)) == 0 && event_data != NULL && strncmp(event_data, EL_WIFI_OFF_DATA, sizeof(EL_WIFI_OFF_DATA)) == 0) {
		printf("Callback data: %s\n", (char *)cb_data);
		el_event_wifi_off_flag = true;
	}

	ret = eventloop_loop_stop();
	if (ret == OK) {
		loop_stop_flag = true;
	}
}

static void utc_eventloop_add_event_handler_n(void)
{
	el_event_t *event_handle;

	event_handle = eventloop_add_event_handler(EL_INVALID_EVENT, (event_callback)wifi_on_callback, NULL);
	TC_ASSERT_EQ("eventloop_add_event_handler", event_handle, NULL);

	event_handle = eventloop_add_event_handler(EL_EVENT_WIFI_ON, NULL, NULL);
	TC_ASSERT_EQ("eventloop_add_event_handler", event_handle, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_add_event_handler_p(void)
{
	el_event_t *wifi_on_handle;
	el_event_t *wifi_off_handle;

	wifi_on_handle = eventloop_add_event_handler(EL_EVENT_WIFI_ON, (event_callback)wifi_on_callback, NULL);
	TC_ASSERT_NEQ("eventloop_add_event_handler", wifi_on_handle, NULL);

	wifi_off_handle = eventloop_add_event_handler(EL_EVENT_WIFI_OFF, (event_callback)wifi_off_callback, EL_CB_DATA);
	TC_ASSERT_NEQ("eventloop_add_event_handler", wifi_off_handle, NULL);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_del_event_handler_n(void)
{
	int ret;

	ret = eventloop_del_event_handler(NULL);
	TC_ASSERT_EQ("eventloop_del_event_handler", ret, EVENTLOOP_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_del_event_handler_p(void)
{
	int ret;
	el_event_t *event_handle;

	event_handle = eventloop_add_event_handler(EL_EVENT_WIFI_ON, (event_callback)wifi_on_callback, NULL);
	TC_ASSERT_NEQ("eventloop_add_event_handler", event_handle, NULL);
	ret = eventloop_del_event_handler(event_handle);
	TC_ASSERT_EQ("eventloop_del_event_handler", ret, OK);

	/* Try to delete already deleted event handler, it will be return EVENTLOOP_INVALID_HANDLE. */
	ret = eventloop_del_event_handler(event_handle);
	TC_ASSERT_EQ("eventloop_del_event_handler", ret, EVENTLOOP_INVALID_HANDLE);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_send_event_n(void)
{
	int ret;

	ret = eventloop_send_event(EL_INVALID_EVENT, NULL, 0);
	TC_ASSERT_EQ("eventloop_send_event", ret, EVENTLOOP_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static bool eventloop_send_event_func(void *data)
{
	static int cnt = 0;
	int ret;

	if (++cnt < EL_SEND_COUNT) {
		/* Send EL_EVENT_WIFI_ON event 4 times */
		ret = eventloop_send_event(EL_EVENT_WIFI_ON, EL_WIFI_ON_DATA, sizeof(EL_WIFI_ON_DATA));
		if (ret == OK) {
			send_cnt++;
		}
		return EVENTLOOP_CALLBACK_CONTINUE;
	}

	/* Send EL_EVENT_WIFI_OFF event and loop will be stopped in callback function. */
	ret = eventloop_send_event(EL_EVENT_WIFI_OFF, EL_WIFI_OFF_DATA, sizeof(EL_WIFI_OFF_DATA));
	if (ret == OK) {
		send_cnt++;
	}
	cnt = 0;

	return EVENTLOOP_CALLBACK_STOP;
}

static void utc_eventloop_send_event_p(void)
{
	int ret;
	el_timer_t *timer;
	el_event_t *event_handle;

	send_cnt = 0;
	el_event_wifi_on_cnt = 0;
	el_event_wifi_off_flag = false;
	loop_stop_flag = false;
	timer = eventloop_add_timer(2000, true, (timeout_callback)eventloop_send_event_func, NULL);
	TC_ASSERT_NEQ("eventloop_add_timer", timer, NULL);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_send_event", send_cnt, EL_SEND_COUNT);
	TC_ASSERT_EQ("eventloop_send_event", el_event_wifi_on_cnt, EL_WIFI_ON_COUNT);
	TC_ASSERT_EQ("eventloop_send_event", el_event_wifi_off_flag, true);
	TC_ASSERT_EQ("eventloop_send_event", loop_stop_flag, true);

	event_handle = eventloop_add_event_handler(EL_EVENT_WIFI_ON, (event_callback)wifi_on_once_callback, NULL);
	TC_ASSERT_NEQ("eventloop_add_event_handler", event_handle, NULL);
	ret = eventloop_send_event(EL_EVENT_WIFI_ON, NULL, 0);
	TC_ASSERT_EQ("eventloop_send_event", ret, OK);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);

	TC_SUCCESS_RESULT();
}

static void el_thread_safe_cb(void *data)
{
	if (strncmp((char *)data, EL_THREAD_SAFE_DATA, sizeof(EL_THREAD_SAFE_DATA)) == 0) {
		el_thread_safe_flag = true;
	}
}

static void utc_eventloop_thread_safe_function_call_n(void)
{
	int ret;

	ret = eventloop_thread_safe_function_call(NULL, NULL);
	TC_ASSERT_EQ("eventloop_thread_safe_function_call", ret, EVENTLOOP_INVALID_PARAM);

	TC_SUCCESS_RESULT();
}

static void utc_eventloop_thread_safe_function_call_p(void)
{
	int ret;

	el_thread_safe_flag = false;
	ret = eventloop_thread_safe_function_call(el_thread_safe_cb, EL_THREAD_SAFE_DATA);
	TC_ASSERT_EQ("eventloop_thread_safe_function_call", ret, OK);
	ret = eventloop_loop_run();
	TC_ASSERT_EQ("eventloop_loop_run", ret, OK);
	TC_ASSERT_EQ("eventloop_thread_safe_function_call", el_thread_safe_flag, true);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_eventloop_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Eventloop UTC") == ERROR) {
		return ERROR;
	}

	utc_eventloop_loop_run_p();

	utc_eventloop_add_timer_n();
	utc_eventloop_add_timer_p();

	utc_eventloop_delete_timer_n();
	utc_eventloop_delete_timer_p();

	utc_eventloop_add_timer_async_n();
	utc_eventloop_add_timer_async_p();

	utc_eventloop_add_event_handler_n();
	utc_eventloop_add_event_handler_p();

	utc_eventloop_del_event_handler_n();
	utc_eventloop_del_event_handler_p();

	utc_eventloop_send_event_n();
	utc_eventloop_send_event_p();

	utc_eventloop_thread_safe_function_call_n();
	utc_eventloop_thread_safe_function_call_p();

	utc_eventloop_loop_stop_p();

	(void)tc_handler(TC_END, "Eventloop UTC");

	return 0;
}
