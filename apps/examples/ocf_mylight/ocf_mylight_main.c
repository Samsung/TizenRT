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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/boardctl.h>

#include "ocf_mylight.h"

static int gQuitFlag;

static const char *tpl_usage = "\n"
		"Usage: %s [command]\n"
		"Commands:\n"
		" recovery\t\tReset SVR DB using JSON to DB converter\n"
		" scan\t\t\tScan wifi network\n"
		" wifi {ssid} {sec} [pw]\tSave wifi information to file\n"
		" \t\t\t- sec: security hex number (e.g. 0x40)\n"
		" net\t\t\tTest wifi connection and network setup\n"
		" factory\t\tFactory reset (reset configuration info)\n"
		" time [time_t]\t\tSet/Get failback time\n"
#ifdef CONFIG_LIBC_LOCALTIME
		" tz [zone]\t\tSet/Get timezone (e.g. KST, PST, ..)\n"
#endif
		"\n";

static void _time_measurement(struct timespec *delay)
{
	int i;
	struct timeval tv;

	printf("time measurement: ");
	gettimeofday(&tv, NULL);
	printf("%zd.%06zd\n", tv.tv_sec, tv.tv_usec);

	for (i = 0; i < 10; i++)
		nanosleep(delay, NULL);

	printf("time measurement: ");
	gettimeofday(&tv, NULL);
	printf("%zd.%06zd\n", tv.tv_sec, tv.tv_usec);
}

static int cmds(char *argv[], int argc, int arg_idx)
{
	const char *prog = argv[arg_idx];
	const char *opt = argv[arg_idx + 1];
	const char *value = NULL;

	if (argc >= arg_idx + 2)
		value = argv[arg_idx + 2];

	if (!strcmp(opt, "recovery")) {
		printf("Recovery json to db\n");
		ocf_mylight_remove_db();
		ocf_mylight_recovery_default();
	} else if (!strcmp(opt, "factory")) {
		ocf_mylight_remove_db();
		ocf_mylight_recovery_default();
		ocf_mylight_setup_reset();
		return ocf_mylight_setup_save();
	} else if (!strcmp(opt, "wifi")) {
		const char *pwd = NULL;
		int sec = 0;

		if (argc >= arg_idx + 3)
			sec = strtol(argv[arg_idx + 3], NULL, 0);
		if (argc >= arg_idx + 4)
			pwd = argv[arg_idx + 4];

		ocf_mylight_setup_set_wifi_ssid(value, strlen(value));
		ocf_mylight_setup_set_wifi_security(sec);

		if (!pwd)
			ocf_mylight_setup_set_wifi_pwd(NULL, 0);
		else
			ocf_mylight_setup_set_wifi_pwd(pwd, strlen(pwd));

		return ocf_mylight_setup_save();
	} else if (!strcmp(opt, "scan")) {
		ocf_mylight_wifi_scan();
	} else if (!strcmp(opt, "net")) {
		ocf_mylight_wifi_connect();
	} else if (!strcmp(opt, "time")) {
		if (!value) {
			printf("failback time: %zd\n",
					ocf_mylight_setup_get_failback_time());
		} else {
			ocf_mylight_setup_set_failback_time(atoi(value));
			return ocf_mylight_setup_save();
		}
#ifdef CONFIG_LIBC_LOCALTIME
	} else if (!strcmp(opt, "tz")) {
		if (!value) {
			printf("timezone: %s\n",
					ocf_mylight_setup_get_timezone());
		} else {
			ocf_mylight_setup_set_timezone(value, strlen(value));
			return ocf_mylight_setup_save();
		}
#endif
	} else {
		printf(tpl_usage, prog);
		return 0;
	}

	MSG("program exited");
	return 0;
}

static int _failback_time_setup(void)
{
	struct timespec tp;

#ifdef CONFIG_LIBC_LOCALTIME
	ocf_mylight_tz_select(ocf_mylight_setup_get_timezone());
#endif

	tp.tv_sec = ocf_mylight_setup_get_failback_time();
	tp.tv_nsec = 0;

	if (tp.tv_sec == 0) {
		DBG("failback_time is 0. program exit");
		return -1;
	}

	clock_settime(CLOCK_REALTIME, &tp);

	DBG("set time to %zd seconds", tp.tv_sec);

	return 0;
}

static void _display_time(void)
{
	struct tm tmdata;
	time_t now;
	char buf[32];

	now = time(NULL);

	gmtime_r(&now, &tmdata);
	strftime(buf, 32, "%Y-%m-%d %H:%M:%S", &tmdata);
	printf("      GMT: %s\n", buf);

	localtime_r(&now, &tmdata);
	strftime(buf, 32, "%Y-%m-%d %H:%M:%S", &tmdata);
	printf("LOCALTIME: %s\n", buf);
}

static int server_cb(int argc, char *argv[])
{
	struct timespec timeout;

	MSG("\nOCF Light IoTivity(%s) Server", IOTIVITY_VERSION);

	if (ocf_mylight_wifi_init() < 0) {
		MSG("wifi initialize failed.");
		return -1;
	}

	if (ocf_mylight_setup_load() < 0) {
		MSG("Can't load setup information");
		return -1;
	}

	/**
	 * argv[0] = task name ('ocf_mylight_server')
	 * argv[1] = original argv[0]
	 * argv[2] = original argv[1]
	 */
	if (argc > 2)
		return cmds(argv, argc, 1);

	_failback_time_setup();
	_display_time();

	if (ocf_mylight_wifi_connect() < 0) {
		MSG("wifi initialize failed.");
		return 0;
	}

	if (ocf_mylight_ntp_init() < 0) {
		MSG("ntp initialize failed. use failback time");
	}

	MSG("Start IoTivity stack...");

	ocf_mylight_security_init();

	if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK) {
		DBG("OCStack init error");
		return 0;
	}

	MSG("Supported Endpoint Transport Protocol Suites: 0x%X",
			OCGetSupportedEndpointTpsFlags());

	ocf_mylight_playform_init();
	ocf_mylight_device_init();
	ocf_mylight_light_init();
#ifdef CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER
	ocf_mylight_dimm_init();
#endif
	ocf_mylight_notify_init();
	ocf_mylight_userinput_init();

	_display_time();

	MSG("Entering ocserver main loop...");

#ifdef LOOPDELAY_0_1_SECS
	timeout.tv_sec = 0;
	timeout.tv_nsec = 100000000L; /* 0.1 sec */
#else
	timeout.tv_sec = 0;
	timeout.tv_nsec = 10000000L; /* 0.01 sec */
#endif

	_time_measurement(&timeout);

	while (!gQuitFlag) {
		if (OCProcess() != OC_STACK_OK) {
			DBG("OCStack process error");
			return 0;
		}
		nanosleep(&timeout, NULL);
	}

	ocf_mylight_light_exit();
	ocf_mylight_notify_exit();

	MSG("Exiting ocserver main loop...");

	if (OCStop() != OC_STACK_OK)
		DBG("OCStack process error");

	ocf_mylight_security_free();

	if (gQuitFlag == 2) {
		MSG("Reboot board");
		boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
	}

	return 0;
}

int ocf_mylight_get_quit_flag(void)
{
	return gQuitFlag;
}

void ocf_mylight_set_quit_flag(int with_reboot)
{
	if (with_reboot)
		gQuitFlag = 2;
	else
		gQuitFlag = 1;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int ocf_mylight_main(int argc, char *argv[])
#endif
{
	task_create("ocf_mylight_server", 100, 16384, server_cb,
			(FAR char * const *)argv);
	return 0;
}
