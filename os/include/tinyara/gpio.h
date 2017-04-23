/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * include/tinyara/gpio.h
 *
 *   Copyright (C) 2016 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: ByoungTae Cho <bt.cho@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_TINYARA_GPIO_H
#define __INCLUDE_TINYARA_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Maximum number of threads than can be waiting for POLL events */
#ifndef CONFIG_GPIO_NPOLLWAITERS
#define CONFIG_GPIO_NPOLLWAITERS 2
#endif

/* ioctl commands */
#define GPIOIOC_SET_DIRECTION	_GPIOIOC(0x0001)
#define GPIOIOC_SET_DRIVE		_GPIOIOC(0x0002)
#define GPIOIOC_POLLEVENTS		_GPIOIOC(0x0003)
#define GPIOIOC_REGISTER		_GPIOIOC(0x0004)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef enum {
	GPIO_EDGE_NONE,		/* No interrupt on GPIO */
	GPIO_EDGE_BOTH,		/* Interrupt occures on rising and falling edge */
	GPIO_EDGE_RISING,	/* Interrupt occurs on rising edge */
	GPIO_EDGE_FALLING,	/* Interrupt occurs on falling edge */
} gpio_edge_t;

typedef enum {
	GPIO_DIRECTION_NONE,
	GPIO_DIRECTION_OUT,
	GPIO_DIRECTION_IN
} gpio_direciton_t;

typedef enum {
	GPIO_DRIVE_NONE,
	GPIO_DRIVE_PULLUP,
	GPIO_DRIVE_PULLDOWN,
	GPIO_DRIVE_FLOAT,
	GPIO_DRIVE_PUSHPULL
} gpio_drive_t;

typedef enum {
	GPIO_CMD_SET_DIRECTION,
	GPIO_CMD_SET_DRIVE,
} gpio_cmd_t;

struct gpio_pollevents_s {
	bool gp_rising;
	bool gp_falling;
};

struct gpio_notify_s {
	bool gn_rising;
	bool gn_falling;
	uint8_t gn_signo;
};

struct gpio_upperhalf_s;
typedef CODE void (*gpio_handler_t)(FAR struct gpio_upperhalf_s *upper);

struct gpio_lowerhalf_s;
struct gpio_ops_s {
	CODE int  (*get)(FAR struct gpio_lowerhalf_s *lower);
	CODE void (*set)(FAR struct gpio_lowerhalf_s *lower,
					 FAR unsigned int value);
	CODE int  (*pull)(FAR struct gpio_lowerhalf_s *lower, unsigned long arg);
	CODE int  (*setdir)(FAR struct gpio_lowerhalf_s *lower, unsigned long arg);
	CODE int  (*enable)(FAR struct gpio_lowerhalf_s *lower,
						int falling, int rising, gpio_handler_t handler);
	CODE int  (*ioctl)(FAR struct gpio_lowerhalf_s *lower, FAR int cmd,
					   unsigned long args);
};

struct gpio_lowerhalf_s {
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpio_register
 *
 * Description:
 *   Register GPIO device.
 *
 ****************************************************************************/
int gpio_register(unsigned int minor, FAR struct gpio_lowerhalf_s *lower);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_TINYARA_GPIO_H */
