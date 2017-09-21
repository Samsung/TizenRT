/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * @file gpio-api.c
 */

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>
#include <artik_gpio.h>

#include "command.h"

static int gpio_read(int argc, char *argv[]);
static int gpio_write(int argc, char *argv[]);
static int gpio_watch(int argc, char *argv[]);
static int gpio_unwatch(int argc, char *argv[]);

const struct command gpio_commands[] = {
	{ "read", "read <num>", gpio_read },
	{ "write", "write <num> <0|1>", gpio_write },
	{ "watch", "watch <num>", gpio_watch },
	{ "unwatch", "unwatch <num>", gpio_unwatch },
	{ "", "", NULL}
};

struct gpio_node {
	artik_list node;
	artik_gpio_handle handle;
};

static artik_list *requested_node;

static void gpio_event(void *user_data, int value)
{
	unsigned int id = (unsigned int)user_data;

	fprintf(stdout, "GPIO id %u event: %d\n", id, value);
}

static int gpio_io(artik_gpio_dir_t dir, artik_gpio_id id, int new_value)
{
	artik_gpio_module *gpio;
	artik_gpio_config config;
	artik_gpio_handle handle;
	char name[16] = "";
	int ret = 0;

	gpio = (artik_gpio_module *)artik_request_api_module("gpio");
	if (!gpio) {
		fprintf(stderr, "GPIO module is not available\n");
		return -1;
	}

	memset(&config, 0, sizeof(config));
	config.dir = dir;
	config.id = id;
	snprintf(name, 16, "gpio%d", config.id);
	config.name = name;

	if (gpio->request(&handle, &config) != S_OK) {
		fprintf(stderr, "Failed to request GPIO %d\n", config.id);
		ret = -1;
		goto exit;
	}

	if (dir == GPIO_IN) {
		ret = gpio->write(handle, new_value);
		if (ret != S_OK) {
			fprintf(stderr, "Failed to write GPIO %d [err %d]\n", config.id, ret);
		} else {
			fprintf(stdout, "Write %d to GPIO %d\n", new_value, config.id);
		}
	} else {
		ret = gpio->read(handle);
		if (ret < 0) {
			fprintf(stderr, "Failed to read GPIO %d [err %d]\n", config.id, ret);
		} else {
			fprintf(stdout, "The value read in GPIO %d is %d\n", config.id, ret);
		}
	}

	gpio->release(handle);

exit:
	artik_release_api_module(gpio);
	return ret;
}

static int gpio_read(int argc, char *argv[])
{
	if (argc < 4) {
		usage(argv[1], gpio_commands);
		return -1;
	}

	return gpio_io(GPIO_OUT, atoi(argv[3]), 0);
}

static int gpio_write(int argc, char *argv[])
{
	if (argc < 5) {
		usage(argv[1], gpio_commands);
		return -1;
	}

	return gpio_io(GPIO_IN, atoi(argv[3]), atoi(argv[4]));
}

static int gpio_watch(int argc, char *argv[])
{
	if (argc < 4) {
		usage(argv[1], gpio_commands);
		return -1;
	}

	artik_gpio_module *gpio = (artik_gpio_module *)
					artik_request_api_module("gpio");

	artik_gpio_config config;
	artik_gpio_handle handle;
	char name[16] = "";
	int ret = 0;

	memset(&config, 0, sizeof(config));
	snprintf(name, 16, "gpio%d", config.id);
	config.name = name;
	config.id = atoi(argv[3]);
	config.dir = GPIO_IN;
	config.edge = GPIO_EDGE_BOTH;
	config.initial_value = 0;

	ret = gpio->request(&handle, &config);
	if (ret != S_OK) {
		fprintf(stderr, "GPIO id %u already on watching or wrong GPIO id\n", config.id);
		goto exit;
	}

	struct gpio_node *node = (struct gpio_node *)artik_list_add(&requested_node,
			(ARTIK_LIST_HANDLE)config.id, sizeof(struct gpio_node));
	if (!node)
		return E_NO_MEM;

	node->handle = handle;

	ret = gpio->set_change_callback(handle, gpio_event, (void *)config.id);
	if (ret != S_OK) {
		fprintf(stderr, "Failed, could not set GPIO change callback (%d)\n",
				ret);
		goto exit;
	}

	fprintf(stdout, "GPIO id %u on watching\n", config.id);

exit:
	artik_release_api_module(gpio);

	return ret;
}

static int gpio_unwatch(int argc, char *argv[])
{
	if (argc < 4) {
		usage(argv[1], gpio_commands);
		return -1;
	}

	artik_gpio_module *gpio = (artik_gpio_module *)
					artik_request_api_module("gpio");

	artik_gpio_config config;

	config.id = atoi(argv[3]);

	struct gpio_node *node = (struct gpio_node *)artik_list_get_by_handle(
			requested_node, (ARTIK_LIST_HANDLE)config.id);

	if (!node) {
		fprintf(stderr, "No watch on GPIO id %u\n", config.id);
		return E_BAD_ARGS;
	}

	int ret = 0;

	gpio->unset_change_callback(node->handle);

	ret = gpio->release(node->handle);

	if (ret != S_OK) {
		fprintf(stderr, "Failed to release (%d)\n", ret);
		goto exit;
	}

	fprintf(stdout, "GPIO id %u unwatched\n", config.id);

	artik_list_delete_node(&requested_node, (artik_list *) node);

exit:
	artik_release_api_module(gpio);

	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int gpio_main(int argc, char *argv[])
#endif
{
	return commands_parser(argc, argv, gpio_commands);
}
