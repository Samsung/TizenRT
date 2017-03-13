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
/************************************************************************************
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
 ******************************************************************************/

#ifndef __INCLUDE_TINYARA_GPIO_H
#define __INCLUDE_TINYARA_GPIO_H

/******************************************************************************
 * Included Files
 ******************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <semaphore.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>

#ifndef CONFIG_DISABLE_POLL
#include<tinyara/serial/serial.h>
#endif
/******************************************************************************
 * Pre-processor Definitions
 ******************************************************************************/

/* Maximum number of threads than can be waiting for POLL events */

#ifndef CONFIG_GPIO_NPOLLWAITERS
#define CONFIG_GPIO_NPOLLWAITERS 2
#endif

/* vtable access helpers */

#define gpio_open(dev)          dev->ops->open(dev)
#define gpio_close(dev)         dev->ops->close(dev)
#define gpio_get(dev)           dev->ops->get(dev)
#define gpio_set(dev, s)        dev->ops->set(dev, s)
#define gpio_ctrl(dev, s, v)    dev->ops->ctrl(dev, s, v)

/******************************************************************************
 * Public Types
 ******************************************************************************/
typedef void (*GPIO_CB_FUNC)(void);

typedef enum {
	GPIO_EDGE_NONE,				/* No interrupt on GPIO */
	GPIO_EDGE_BOTH,				/* Interrupt occures on rising and falling edge */
	GPIO_EDGE_RISING,			/* Interrupt occurs on rising edge */
	GPIO_EDGE_FALLING,			/* Interrupt occurs on falling edge */

	GPIO_LEVEL_LOW,				/* Not support in SystemIO */
	GPIO_LEVEL_HIGH,			/* Not support in SystemIO */
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
	GPIO_CMD_GET_DIRECTION,
	GPIO_CMD_SET_EDGE,
	GPIO_CMD_GET_EDGE,
	GPIO_CMD_SET_DRIVE,
	GPIO_CMD_GET_DRIVE,
	GPIO_CMD_SET_CALLBACK
} gpio_cmd_t;

struct gpio_dev_s;
struct gpio_ops_s {
	CODE int (*open)(FAR struct gpio_dev_s *dev);
	CODE int (*close)(FAR struct gpio_dev_s *dev);
	CODE int (*get)(FAR struct gpio_dev_s *dev);
	CODE void (*set)(FAR struct gpio_dev_s *dev, FAR unsigned int value);
	CODE int (*ctrl)(FAR struct gpio_dev_s *dev, FAR int cmd, unsigned long args);
};

struct gpio_dev_s {
	uint8_t open_count;			/* Number of times the device has been opened */
	FAR const struct gpio_ops_s *ops;	/* Arch-specific operations */
	FAR void *priv;				/* Used by the arch-specific logic */

#ifndef CONFIG_DISABLE_POLL
	struct pollfd *fds[CONFIG_SERIAL_NPOLLWAITERS];
	sem_t pollsem;				/* Manages exclusive access to fds[] */
#endif
	sem_t closesem;				/* Locks out new open while close is in progress */

#if defined(CONFIG_ARCH_CHIP_S5JT200)
	CODE void (*callback)(void);
	WDOG_ID wdog;
#if defined(CONFIG_SCHED_WORKQUEUE)
	struct work_s work;			/* Supports the interrupt handling "bottom half" */
#endif
#endif
};

/******************************************************************************
 * Public Data
 ******************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/******************************************************************************
 * Public Functions
 ******************************************************************************/

/*******************************************************************************
 * Name: gpio_notify
 *
 * Description:
 *   This routine is called from irq handler. If you want to handle with
 *   async call or event, you have to register your fd with poll().
 *
 ******************************************************************************/

void gpio_notify(FAR struct gpio_dev_s *dev);

/******************************************************************************
 * Name: gpio_register
 *
 * Description:
 *   Register GPIO device.
 *
 ******************************************************************************/

int gpio_register(FAR const char *path, FAR struct gpio_dev_s *dev);

/******************************************************************************
 * Name: gpio_export_init
 *
 * Description:
 *   register export-file in filesystem
 *
 ******************************************************************************/

#ifdef CONFIG_GPIO_EXPORT
int gpio_export_init();
#endif

/******************************************************************************
 * Name: gpio_unexport_init
 *
 * Description:
 *   register unexport-file in filesystem
 *
 ******************************************************************************/

#ifdef CONFIG_GPIO_EXPORT
int gpio_unexport_init();
#endif

/*******************************************************************************
 * Name: up_destroy_gpio
 *
 * Description:
 *  Destroy device.
 *
 ******************************************************************************/

int up_destroy_gpio(int32_t idx);

/*******************************************************************************
 * Name: up_create_gpio
 *
 * Description:
 * Create device. Userspace may ask the kernel to export control of
 * a GPIO to userspace by writing its number to file (gpio_export).
 *
 ******************************************************************************/

int up_create_gpio(int32_t idx);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_TINYARA_GPIO_H */
