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
/*******************************************************************************
 * Included Files
 ******************************************************************************/

#include <stdio.h>
#include <debug.h>

#include <tinyara/gpio.h>

/*******************************************************************************
 * Pre-processor Definitions
 ******************************************************************************/

/******************************************************************************
 * Private Types
 ******************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
static ssize_t gpio_exportfs(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static ssize_t gpio_unexportfs(FAR struct file *filep, FAR const char *buffer, size_t buflen);

/******************************************************************************
 * Private Variables
 ******************************************************************************/

static const struct file_operations g_exportops = {
	NULL,						/* open  */
	NULL,						/* close */
	NULL,						/* read  */
	gpio_exportfs,				/* write */
	NULL,						/* seek  */
	NULL,						/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL						/* poll  */
#endif
};

static const struct file_operations g_unexportops = {
	NULL,						/* open  */
	NULL,						/* close */
	NULL,						/* read  */
	gpio_unexportfs,			/* write */
	NULL,						/* seek  */
	NULL,						/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL						/* poll  */
#endif
};

/******************************************************************************
 * Private Functions
 ******************************************************************************/

/******************************************************************************
 * Name: gpio_exportfs
 *
 * Description:
 *    Make GPIO pin. If it is succeed, you can be found /dev/gpioXX in FS
 *
 ******************************************************************************/

static ssize_t gpio_exportfs(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	int32_t value;
	int32_t ret;

	sscanf(buffer, "%d", &value);

	dbg("export GPIO(%d)\n", value);

	ret = up_create_gpio(value);

	return (ret < 0) ? ret : buflen;
}

/*******************************************************************************
 * Name: gpio_unexport
 *
 * Description:
 *    This function is called when you want to close opening GPIO.
 *
 ******************************************************************************/

static ssize_t gpio_unexportfs(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	int32_t value;
	int32_t ret;

	sscanf(buffer, "%d", &value);

	dbg("unexport GPIO(%d)\n", value);

	ret = up_destroy_gpio(value);

	return (ret < 0) ? ret : buflen;
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/

/******************************************************************************
 * Name: gpio_export_init
 *
 * Description:
 *   register export-file in filesystem
 *
 ******************************************************************************/

int gpio_export_init()
{
	int ret;
	const char *path = "/dev/gpio_export";
	dev_t dev;

	ret = register_driver(path, &g_exportops, 0666, &dev);

	return ret;
}

/******************************************************************************
 * Name: gpio_unexport_init
 *
 * Description:
 *   register unexport-file in filesystem
 *
 ******************************************************************************/

int gpio_unexport_init()
{
	int ret;
	const char *path = "/dev/gpio_unexport";
	dev_t dev;

	ret = register_driver(path, &g_unexportops, 0666, &dev);

	return ret;
}
