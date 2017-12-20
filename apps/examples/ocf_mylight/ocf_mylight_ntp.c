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
#include <time.h>
#include <apps/netutils/ntpclient.h>

#include "ocf_mylight.h"

static struct ntpc_server_conn_s _ntp_list[] = {
	{ "pool.ntp.org", 123 }
};

static void _ntp_link_error(void)
{
	printf("ntp_link_error() callback is called.\n");
}

int ocf_mylight_ntp_init(void)
{
	int ntp_pid;
	int status;
	int retry = 0;

	ntp_pid = ntpc_start(_ntp_list,
			sizeof(_ntp_list) / sizeof(struct ntpc_server_conn_s),
			MAX_NTP_POLL_INTERVAL_SECS, _ntp_link_error);
	if (ntp_pid < 0) {
		DBG("ERROR: ntpc_start() failed. (ret=%d)", ntp_pid);
		return -1;
	}

	printf("ntpc_start() success.\n");

	while (retry < 3) {
		status = ntpc_get_link_status();
		DBG("ntp state: %d", status);

		if (status == NTP_LINK_UP) {
			DBG("ntp time received");
			break;
		}

		sleep(1);
		retry++;
	}

	if (status != NTP_LINK_UP) {
		DBG("NTP time receive failed.");
		ntpc_stop();
		return -1;
	}

#ifdef CONFIG_LIBC_LOCALTIME
	ocf_mylight_tz_select(ocf_mylight_setup_get_timezone());
#endif

	ntpc_stop();

	return 0;
}
