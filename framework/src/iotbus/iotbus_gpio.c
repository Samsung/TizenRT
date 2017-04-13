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

/**
 * @file iotbus_gpio.c
 * @brief Iotbus APIs for Gpio
 */

#include <tinyara/config.h>
#include <tinyara/gpio.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>

#include <include/iotbus/iotbus_gpio.h>
#include <include/iotbus/iotbus_error.h>
#include "iotapi_evt_handler.h"

#define zdbg printf

/**
 * @brief Struct for iotbus_gpio_s
 */
struct _iotbus_gpio_s {
	int pin;
	iotbus_gpio_drive_e drive;
	iotbus_gpio_direction_e dir;
	iotbus_gpio_edge_e edge;
	int fd;
	gpio_isr_cb isr_cb;
	void *ud;
};

#ifdef __cplusplus
extern "C" {
#else
#endif

void gpio_async_handler(void *data)
{
	struct _iotbus_gpio_s *item = (struct _iotbus_gpio_s *)data;
	item->isr_cb(item->ud);

	return;
}

/**
 * @brief Initializes gpio_context, based on Gpio pin.
 */
iotbus_gpio_context_h iotbus_gpio_open(int gpiopin)
{
	struct _iotbus_gpio_s *dev = (struct _iotbus_gpio_s *)malloc(sizeof(struct _iotbus_gpio_s));

	char gpio_dev[32] = { 0, };
	snprintf(gpio_dev, 32, "/dev/gpio%d", gpiopin);

	dev->fd = open(gpio_dev, O_RDWR);
	if (dev->fd < 0) {
		zdbg("open %s failed: %d\n", gpio_dev, errno);
		free(dev);
		return NULL;
	}
	dev->pin = gpiopin;
	dev->drive = IOTBUS_GPIO_DRIVE_PULLUP;
	dev->dir = IOTBUS_GPIO_DIRECTION_OUT;
	dev->edge = IOTBUS_GPIO_EDGE_NONE;
	dev->isr_cb = NULL;

	return dev;
}

/**
 * @brief Closes the gpio_context.
 */
int iotbus_gpio_close(iotbus_gpio_context_h dev)
{
	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (dev->isr_cb != NULL) {
		int ret = iotbus_gpio_unregister_cb(dev);
		if (ret != IOTBUS_ERROR_NONE)
			return ret;
	}

	close(dev->fd);
	free(dev);
	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Sets Gpio direction.
 */
int iotbus_gpio_set_direction(iotbus_gpio_context_h dev, iotbus_gpio_direction_e dir)
{
	int ret = -1;

	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	dev->dir = dir;
	switch (dev->dir) {
	case IOTBUS_GPIO_DIRECTION_NONE:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DIRECTION, (unsigned long)GPIO_DIRECTION_NONE);
			break;
		}
	case IOTBUS_GPIO_DIRECTION_IN:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DIRECTION, (unsigned long)GPIO_DIRECTION_IN);
			break;
		}
	case IOTBUS_GPIO_DIRECTION_OUT:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DIRECTION, (unsigned long)GPIO_DIRECTION_OUT);
			break;
		}
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (ret != 0) {
		switch (errno) {
		case EPERM:
			zdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			zdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			zdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Sets the edge mode on the Gpio.
 */
int iotbus_gpio_set_edge_mode(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge)
{
	int ret = -1;

	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	switch (edge) {
	case IOTBUS_GPIO_EDGE_NONE:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_EDGE, (unsigned long)GPIO_EDGE_NONE);
			break;
		}
	case IOTBUS_GPIO_EDGE_BOTH:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_EDGE, (unsigned long)GPIO_EDGE_BOTH);
			break;
		}
	case IOTBUS_GPIO_EDGE_RISING:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_EDGE, (unsigned long)GPIO_EDGE_RISING);
			break;
		}
	case IOTBUS_GPIO_EDGE_FALLING:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_EDGE, (unsigned long)GPIO_EDGE_FALLING);
			break;
		}
	default:{
			return IOTBUS_ERROR_INVALID_PARAMETER;

		}
	}

	if (ret != 0) {
		switch (errno) {
		case EPERM:
			zdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			zdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			zdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	dev->edge = edge;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Sets Gpio output mode.
 */
int iotbus_gpio_set_drive_mode(iotbus_gpio_context_h dev, iotbus_gpio_drive_e drive)
{
	int ret = -1;

	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	dev->drive = drive;

	switch (dev->drive) {
	case IOTBUS_GPIO_DRIVE_NONE:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DRIVE, (unsigned long)GPIO_DRIVE_NONE);
			break;
		}
	case IOTBUS_GPIO_DRIVE_PULLUP:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DRIVE, (unsigned long)GPIO_DRIVE_PULLUP);
			break;
		}
	case IOTBUS_GPIO_DRIVE_PULLDOWN:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DRIVE, (unsigned long)GPIO_DRIVE_PULLDOWN);
			break;
		}
	case IOTBUS_GPIO_DRIVE_FLOAT:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DRIVE, (unsigned long)GPIO_DRIVE_FLOAT);
			break;
		}
	case IOTBUS_GPIO_DRIVE_PUSHPULL:{
			ret = ioctl(dev->fd, GPIO_CMD_SET_DRIVE, (unsigned long)GPIO_DRIVE_PUSHPULL);
			break;
		}
	}

	if (ret != 0) {
		switch (errno) {
		case EPERM:
			zdbg("unsupported command \n");
			return IOTBUS_ERROR_NOT_SUPPORTED;
		case EINVAL:
			zdbg("invalid parameter \n");
			return IOTBUS_ERROR_INVALID_PARAMETER;
		default:
			zdbg("ioctl failed \n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Registers event handler callback for interrupt.
 */
int iotbus_gpio_register_cb(iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge, gpio_isr_cb isr_cb, void *user_data)
{
	int ret = iotbus_gpio_set_edge_mode(dev, edge);
	if (ret != IOTBUS_ERROR_NONE)
		return ret;
	if (isr_cb == NULL || user_data == NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	struct _iotbus_gpio_s *item = (struct _iotbus_gpio_s *)dev;
	iotapi_elem elm;

	item->ud = user_data;
	item->isr_cb = isr_cb;
	elm.fd = dev->fd;
	elm.data = item;
	elm.func = gpio_async_handler;

	iotapi_insert(&elm);

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Unregisters event handler callback for interrupt.
 */
int iotbus_gpio_unregister_cb(iotbus_gpio_context_h dev)
{
	if (dev == NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;
	iotapi_elem elm;
	struct _iotbus_gpio_s *item = (struct _iotbus_gpio_s *)dev;
	elm.fd = item->fd;

	iotapi_remove(&elm);

	item->isr_cb = NULL;
	item->ud = NULL;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Reads the gpio value.
 */
int iotbus_gpio_read(iotbus_gpio_context_h dev)
{
	int state = 0;

	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	int ret = read(dev->fd, &state, sizeof(int));
	if (ret < 0)
		return IOTBUS_ERROR_UNKNOWN;
	ret = lseek(dev->fd, 0, SEEK_SET);
	if (ret < 0)
		return IOTBUS_ERROR_UNKNOWN;

	return state;
}

/**
 * @brief Writes to the gpio value.
 */
int iotbus_gpio_write(iotbus_gpio_context_h dev, int value)
{
	int ret;

	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (value != 0 && value != 1)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	ret = write(dev->fd, &value, sizeof(int));

	if (ret < 0)
		return IOTBUS_ERROR_UNKNOWN;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Gets a direction of the Gpio.
 */
int iotbus_gpio_get_direction(iotbus_gpio_context_h dev, iotbus_gpio_direction_e * dir)
{
	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	*dir = dev->dir;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Gets a pin number of the Gpio.
 */
int iotbus_gpio_get_pin(iotbus_gpio_context_h dev)
{
	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	return dev->pin;
}

/**
 * @brief Gets a edge mode of the Gpio.
 */
int iotbus_gpio_get_edge_mode(iotbus_gpio_context_h dev, iotbus_gpio_edge_e * edge)
{
	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	*edge = dev->edge;

	return IOTBUS_ERROR_NONE;
}

/**
 * @brief Gets a drive mode of the Gpio.
 */
int iotbus_gpio_get_drive_mode(iotbus_gpio_context_h dev, iotbus_gpio_drive_e * drive)
{
	if (!dev)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	*drive = dev->drive;

	return IOTBUS_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif
