/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <protocols/ntpclient.h>
#include "rreport_config.h"


static void ntp_link_error(void)
{
	printf("ntp_link_error() callback is called.\n");
}

int rr_sync_time(void)
{
	struct ntpc_server_conn_s server;
	server.hostname = RR_NTPC_SERVER;
	server.port = RR_NTPC_PORT;
	if (ntpc_start(&server, 1, RR_NTPC_INTERVAL_SEC, ntp_link_error) <= 0) {
		printf("error start ntpc\n");
		return -1;
	}
	// wait until ntpc receive time
	sleep(1);

	return 0;
}

int rr_get_time(char *buf, int size)
{
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64];

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec + 9 * 3600; // add additional number to support timezone
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
	snprintf(buf, size, "%s.%06ld", tmbuf, tv.tv_usec);

	return 0;
}
