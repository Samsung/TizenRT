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
 * examples/ntpclient_test/ntpclient_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
/**
 * @file ntpclient_main.c
 * @brief the program for testing ntpclient
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <protocols/ntpclient.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define TIME_STRING_LEN         16
#define DATE_STRING_LEN         16

#ifndef CONFIG_EXAMPLES_NTPCLIENT_TEST_SERVER_PORT
#define CONFIG_EXAMPLES_NTPCLIENT_TEST_SERVER_PORT	123
#endif

/****************************************************************************
 * Enumeration
 ****************************************************************************/
enum timezone_e {
	TIMEZONE_UTC = 0,
	TIMEZONE_KST,
	TIMEZONE_END
};

/****************************************************************************
 * Structure
 ****************************************************************************/
struct timezone_info_s {
	char str[4];
	int offset;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct ntpc_server_conn_s g_server_conn[MAX_NTP_SERVER_NUM];

static char g_time_str[TIME_STRING_LEN + 1];
static char g_date_str[DATE_STRING_LEN + 1];
struct timezone_info_s g_timezone[TIMEZONE_END] = {
	{"UTC", 0},
	{"KST", 9}
};

/****************************************************************************
 * function prototype
 ****************************************************************************/
static void show_usage(FAR const char *progname);
static int ntpclient_show_date(unsigned int timezone);
static void ntpclient_show_daemon_status(unsigned int status);

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * static function
 ****************************************************************************/
static void show_usage(FAR const char *progname)
{
	printf("\nUsage: %s <command> [interval secs] [server's hostname] ... [server's hostname]\n", progname);
	printf("\nWhere:\n");
	printf("	<command>       command string (start | stop | status | date | link) \n");
	printf("	                 - start  : start ntpclient daemon \n");
	printf("	                 - stop   : terminate ntpclient daemon \n");
	printf("	                 - status : show ntpclient daemon's status \n");
	printf("	                 - date   : show current date and time \n");
	printf("	                 - link   : show ntpclient link status \n");
	printf("	[interval secs] ntp client poll interval seconds \n");
	printf("	[server's hostname] ntp server's hostname. \n");
	printf("\n");
}

static int ntpclient_show_date(unsigned int timezone)
{
	int result = -1;
	time_t now;
	struct tm *ptm;

	if (timezone >= TIMEZONE_END) {
		fprintf(stderr, "ERROR: invalid timezone: %d\n", timezone);
		goto done;
	}

	now = time(NULL);			/* UTC time */
	now += (g_timezone[timezone].offset * 3600);
	ptm = gmtime(&now);
	(void)strftime(g_time_str, TIME_STRING_LEN, "%H:%M:%S", ptm);
	(void)strftime(g_date_str, DATE_STRING_LEN, "%b %d, %Y", ptm);

	printf("current time: %s %s %s\n", g_date_str, g_time_str, g_timezone[timezone].str);

	/* result is success */
	result = 0;

done:
	return result;
}

static void ntpclient_show_daemon_status(unsigned int status)
{
	const int status_str_len = 16;
	char status_str[status_str_len];

	switch (status) {
	case NTP_NOT_RUNNING:
		snprintf(status_str, status_str_len, "NOT_RUNNING");
		break;
	case NTP_STARTED:
		snprintf(status_str, status_str_len, "STARTED");
		break;
	case NTP_RUNNING:
		snprintf(status_str, status_str_len, "RUNNING");
		break;
	case NTP_STOP_REQUESTED:
		snprintf(status_str, status_str_len, "STOP_REQUESTED");
		break;
	case NTP_STOPPED:
		snprintf(status_str, status_str_len, "STOPPED");
		break;
	default:
		fprintf(stderr, "ERROR: invalid daemon status : %d\n", status);
		return;
	}

	printf("ntpclient daemon status : %s\n", status_str);
}

static void ntp_link_error(void)
{
	printf("ntp_link_error() callback is called.\n");
}

static int test_ntpclient_start(struct ntpc_server_conn_s *server_conn, int interval_secs, int num_of_servers)
{
	if (ntpc_start(server_conn, num_of_servers, interval_secs, ntp_link_error) < 0) {
		fprintf(stderr, "ERROR: ntpc_start() failed.\n");
		return -1;
	}
	printf("ntpc_start() OK.\n");

	return 0;
}

static int test_ntpclient_stop(void)
{
	if (ntpc_stop() < 0) {
		fprintf(stderr, "ERROR: ntpc_stop() failed.\n");
		return -1;
	}
	printf("ntpc_stop() OK.\n");

	return 0;
}

static int test_ntpclient_get_status(void)
{
	int status = ntpc_get_status();

	switch (status) {
	case NTP_NOT_RUNNING:
	case NTP_STARTED:
	case NTP_RUNNING:
	case NTP_STOP_REQUESTED:
	case NTP_STOPPED:
		ntpclient_show_daemon_status(status);
		break;
	default:
		fprintf(stderr, "ERROR: ntpc_get_status() failed.\n");
		return -1;
	}

	return 0;
}

static int test_ntpclient_get_link(void)
{
	int link = ntpc_get_link_status();
	printf("ntpclient link status : ");
	switch (link) {
	case NTP_LINK_NOT_SET:
		printf("Searching NTP Server\n");
		break;
	case NTP_LINK_UP:
		printf("Link UP\n");
		break;
	case NTP_LINK_DOWN:
		printf("Link Down\n");
		break;
	default:
		printf("Unknown\n");
		return -1;
	}

	return 0;
}

/****************************************************************************
 * ntpclient_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ntpclient_main(int argc, char *argv[])
#endif
{
	if ((argc >= (3 + 1)) && (argc <= (3 + MAX_NTP_SERVER_NUM)) && !strcmp(argv[1], "start")) {
		int i;
		int interval_secs = atoi(argv[2]);
		int num_of_servers = argc - 3;
		memset(&g_server_conn, 0x0, sizeof(g_server_conn));
		for (i = 0; i < num_of_servers; i++) {
			g_server_conn[i].hostname = argv[3 + i];
			g_server_conn[i].port = CONFIG_EXAMPLES_NTPCLIENT_TEST_SERVER_PORT;
		}
		if (test_ntpclient_start(g_server_conn, interval_secs, num_of_servers) != 0) {
			goto errout;
		}
	} else if ((argc == 2) && !strcmp(argv[1], "stop")) {
		if (test_ntpclient_stop() != 0) {
			goto errout;
		}
	} else if ((argc == 2) && !strcmp(argv[1], "status")) {
		if (test_ntpclient_get_status() != 0) {
			goto errout;
		}
	} else if ((argc == 2) && !strcmp(argv[1], "date")) {
		ntpclient_show_date(TIMEZONE_KST);
	} else if ((argc == 2) && !strcmp(argv[1], "link")) {
		if (test_ntpclient_get_link() != 0) {
			goto errout;
		}
	} else {
		show_usage(argv[0]);
		goto errout;
	}

	return 0;

errout:
	return -1;
}
