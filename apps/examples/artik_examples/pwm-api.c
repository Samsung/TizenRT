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
 * @file pwm-api.c
 */

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>
#include <artik_pwm.h>

#include "command.h"

static artik_pwm_handle pwm_handle;

static int pwm_start(int argc, char *argv[]);
static int pwm_stop(int argc, char *argv[]);

const struct command pwm_commands[] = {
	{ "start", "start <pin num> <period> <duty cycle> [invert]", pwm_start },
	{ "stop", "stop", pwm_stop},
	{"", "", NULL}
};

static int pwm_start(int argc, char *argv[])
{
	artik_pwm_module *pwm = NULL;
	int ret = 0;
	artik_error err = S_OK;

	artik_pwm_config config;
	char name[16] = "";

	if (argc < 6) {
		fprintf(stderr, "Wrong number of arguments\n");
		usage(argv[1], pwm_commands);
		return -1;
	}

	pwm = (artik_pwm_module *)artik_request_api_module("pwm");
	if (!pwm) {
		fprintf(stderr, "PWM module is not available\n");
		return -1;
	}

	if (pwm_handle) {
		fprintf(stderr, "PWM was already started\n");
		ret = -1;
		goto exit;
	}

	memset(&config, 0, sizeof(config));
	config.pin_num = atoi(argv[3]);
	config.period = atoi(argv[4]);
	config.duty_cycle = atoi(argv[5]);
	if ((argc > 6) && !strcmp(argv[6], "invert")) {
		config.polarity = ARTIK_PWM_POLR_INVERT;
	}

	snprintf(name, 16, "pwm%d", config.pin_num);
	config.name = name;

	err = pwm->request(&pwm_handle, &config);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request PWM %d (err=%d)\n",
				config.pin_num, err);
		ret = -1;
		goto exit;
	}

	err = pwm->enable(pwm_handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to enable PWM %d (err=%d)\n",
				config.pin_num, err);
		ret = -1;
		goto exit;
	}

exit:
	artik_release_api_module(pwm);
	return ret;
}

static int pwm_stop(int argc, char *argv[])
{
	int ret = 0;
	artik_pwm_module *pwm = (artik_pwm_module *)artik_request_api_module("pwm");
	artik_error err = S_OK;

	if (!pwm) {
		fprintf(stderr, "PWM module is not available\n");
		return -1;
	}

	if (!pwm_handle) {
		fprintf(stderr, "PWM was not started\n");
		ret = -1;
		goto exit;
	}

	err = pwm->release(pwm_handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to release PWM (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	pwm_handle = NULL;

exit:
	artik_release_api_module(pwm);
	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int pwm_main(int argc, char *argv[])
#endif
{
	return commands_parser(argc, argv, pwm_commands);
}
