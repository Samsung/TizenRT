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
#ifndef _IOTBUS_COMMON_H__
#define _IOTBUS_COMMON_H__

#include <tinyara/config.h>

#include <debug.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

typedef enum {
	IOTBUS_GPIO = 0,
	IOTBUS_PWM,
	IOTBUS_ADC,
	IOTBUS_UART,
	IOTBUS_I2C,
	IOTBUS_SPI,
	IOTBUS_PIN_MAX,
} iotbus_pin_e;

struct intr_attr {
	uint8_t priority;
	iotbus_pin_e pin;
	pid_t parent;
};

pthread_t create_intr_pthread(pthread_startroutine_t handler, pthread_addr_t arg, struct intr_attr *val);
void iotapi_sig_init(void);

#endif
