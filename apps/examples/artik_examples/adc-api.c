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
 * @file adc_api.c
 */

#include <stdio.h>
#include <string.h>

#include <shell/tash.h>

#include <artik_module.h>
#include <artik_error.h>
#include <artik_adc.h>

#include "command.h"

static int adc_read(int argc, char *argv[]);

const struct command adc_commands[] = {
	{ "read", "read <pin num>", adc_read },
	{ "", "", NULL }
};

static int adc_read(int argc, char *argv[])
{
	artik_adc_module *adc = NULL;
	artik_adc_handle handle = NULL;
	artik_adc_config config;
	artik_error err = S_OK;
	int val = 0;
	char name[16] = "";
	int ret = 0;

	if (argc < 4) {
		fprintf(stderr, "Wrong arguments\n");
		usage(argv[1], adc_commands);
		return -1;
	}

	adc = (artik_adc_module *)artik_request_api_module("adc");
	if (!adc) {
		fprintf(stderr, "ADC module is not available\n");
		return -1;
	}

	memset(&config, 0, sizeof(config));
	config.pin_num = atoi(argv[3]);
	snprintf(name, 16, "adc%d", config.pin_num);
	config.name = name;

	err = adc->request(&handle, &config);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request ADC %d (%d)\n", config.pin_num, err);
		ret = -1;
		goto exit;
	}

	err = adc->get_value(handle, &val);
	if (err != S_OK) {
		fprintf(stderr, "Failed to read ADC %d value (%d)\n", config.pin_num, err);
		adc->release(handle);
		ret = -1;
		goto exit;
	}

	fprintf(stdout, "ADC%d=%d\n", config.pin_num, val);
	adc->release(handle);

exit:
	artik_release_api_module(adc);
	return ret;
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int adc_main(int argc, char *argv[])
#endif
{
	return commands_parser(argc, argv, adc_commands);
}
