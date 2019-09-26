/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <signal.h>
#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_uart.h>

#include "iotapi_sig_handler.h"

#define IOTAPI_SIG_THREAD_SIZE	2048
#define IOTAPI_THREAD_PRIORITY	120

static pthread_addr_t iotapi_sig_work(pthread_addr_t arg);
static pthread_t g_tid = -1;

static pthread_t _create_sig_thread(void *arg)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = 1;

	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		ibdbg("failed to set pthread init(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (ret != 0) {
		ibdbg("failed to set policy(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setstacksize(&attr, IOTAPI_SIG_THREAD_SIZE);
	if (ret != 0) {
		ibdbg("failed to set stack size(%d)\n", ret);
		return -ret;
	}

	sparam.sched_priority = IOTAPI_THREAD_PRIORITY;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != 0) {
		ibdbg("failed to set sched param(%d)\n", ret);
		return -ret;
	}

	ret = pthread_create(&tid, &attr, iotapi_sig_work, arg);
	if (ret != 0) {
		ibdbg("failed to create pthread(%d)\n", ret);
		return -ret;
	}

	pthread_setname_np(tid, "iotapi_handler");
	pthread_detach(tid);

	return tid;
}

static pthread_addr_t iotapi_sig_work(pthread_addr_t arg)
{
	void *ptr;
	int started = 1;

	sigset_t set;
	siginfo_t info;

	sigemptyset(&set);
	sigaddset(&set, SIG_IOTBUS_GPIO_FALLING | SIG_IOTBUS_GPIO_RISING);
	sigaddset(&set, SIG_IOTBUS_UART_TX_EMPTY | SIG_IOTBUS_UART_TX_RDY);
	sigaddset(&set, SIG_IOTBUS_UART_RECEIVED | SIG_IOTBUS_UART_RX_AVAIL);

	while (started) {
		sigwaitinfo(&set, &info);

#ifdef CONFIG_CAN_PASS_STRUCTS
		ptr = info.si_value.sival_ptr;
#else
		ptr = info.si_value;
#endif
		if (ptr == NULL) {
			continue;
		}
		switch (info.si_signo) {
		case SIG_IOTBUS_GPIO_FALLING:
		case SIG_IOTBUS_GPIO_RISING: {
			struct _iotbus_gpio_wrapper_s *dev = (struct _iotbus_gpio_wrapper_s *)ptr;
			void *sig_cb = iotbus_gpio_get_callback(dev);
			if (sig_cb != NULL) {
				((iotbus_gpio_cb)sig_cb)((iotbus_gpio_context_h)dev);
			}
		}	break;
		case SIG_IOTBUS_UART_TX_EMPTY:
			break;
		case SIG_IOTBUS_UART_TX_RDY:
			break;
		case SIG_IOTBUS_UART_RX_AVAIL:
			break;
		case SIG_IOTBUS_UART_RECEIVED:
			break;
		default:
			ibdbg("Fail to estimate interrupt type");
			started = 0;
		}
	}
	return NULL;
}

pid_t iotapi_get_pid()
{
	return g_tid;
}

void iotapi_sig_init()
{
	if (g_tid > 0) {
		return;
	}

	g_tid = _create_sig_thread(NULL);
	if (g_tid < 0) {
		ibdbg("Fail to create sig thread\n");
	}

	return;
}
