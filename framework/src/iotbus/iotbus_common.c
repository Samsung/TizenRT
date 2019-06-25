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
#include <iotbus/iotbus_common.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#define INTR_THREAD_STACK_SIZE		1024

char *iotbus_pin_str[IOTBUS_PIN_MAX] = {
	"GPIO",
	"PWM",
	"ADC",
	"UART",
	"I2C",
	"SPI"
};

pthread_t create_intr_pthread(pthread_startroutine_t handler, pthread_addr_t arg, struct intr_attr *val)
{
	ibdbg("Start To Create Thread !\n");
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	char pname[32] = { 0, };
	int ret = OK;

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

	ret = pthread_attr_setstacksize(&attr, INTR_THREAD_STACK_SIZE);
	if (ret != 0) {
		ibdbg("failed to set stack size(%d)\n", ret);
		return -ret;
	}

	sparam.sched_priority = val->priority;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != 0) {
		ibdbg("failed to set sched param(%d)\n", ret);
		return -ret;
	}

	ret = pthread_create(&tid, &attr, handler, arg);
	if (ret != 0) {
		ibdbg("failed to create pthread(%d)\n", ret);
		return -ret;
	}

	sprintf(pname, "%s_intr_handler[%d]", iotbus_pin_str[val->pin], val->parent);
	pthread_setname_np(tid, pname);
	pthread_detach(tid);

	ibdbg("[%d] Thread Created!\n", tid);

	return tid;
}

void iotapi_sig_init(void)
{
	return;
}