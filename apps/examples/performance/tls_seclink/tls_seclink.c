/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * examples/performance/tls_seclink/tls_seclink.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include "tls_common.h"

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tls_seclink_main(int argc, char *argv[])
#endif
{
	int ret;
	int srv_ret = -1;
	int cli_ret = -1;
	struct timespec ts_origin;
	struct timespec ts;

	clock_gettime(CLOCK_REALTIME, &ts_origin);

	ts.tv_sec = 1612483200; // 2021-02-05 00:00:00
	ts.tv_nsec = 0;

	clock_settime(CLOCK_REALTIME, &ts);

	printf("-- [TLS SECLINK TEST] START --\n");

	pthread_t tls_server;
	pthread_attr_t attr;
	struct sched_param sparam;
	int status;

	/* Initialize the attribute variable */
	status = pthread_attr_init(&attr);
	if (status != 0) {
		TLS_SECLINK_LOG("tls_seclink(): pthread_attr_init failed, status=%d\n", status);
		goto exit;
	}

	/* 1. set a priority */
	sparam.sched_priority = TLS_SERVER_PRI;
	status = pthread_attr_setschedparam(&attr, &sparam);
	if (status != 0) {
		TLS_SECLINK_LOG("tls_seclink(): pthread_attr_setschedparam failed, status=%d\n", status);
		goto exit;
	}

	/* 2. set a stacksize */
	status = pthread_attr_setstacksize(&attr, TLS_STACKSIZE);
	if (status != 0) {
		TLS_SECLINK_LOG("tls_seclink(): pthread_attr_setstacksize failed, status=%d\n", status);
		goto exit;
	}

	/* 3. set a sched policy */
	status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (status != 0) {
		TLS_SECLINK_LOG("tls_seclink(): pthread_attr_setschedpolicy failed, status=%d\n", status);
		goto exit;
	}

	/* 4. create pthread with entry function */
	status = pthread_create(&tls_server, &attr, tls_server_seclink_main, NULL);
	if (status != 0) {
		TLS_SECLINK_LOG("tls_seclink(): pthread_create failed, status=%d\n", status);
		goto exit;
	}

	status = pthread_setname_np(tls_server, "tls_seclink_server");
	if (status != 0) {
		TLS_SECLINK_LOG("failed to set name for pthread(%d)\n", status);
		goto exit;
	}

	/* To be clear until server thread working */
	sleep(1);

	cli_ret = (int)tls_client_seclink_main(NULL);
	TLS_SECLINK_LOG("[CLIENT] End : %d --\n", cli_ret);

	if (cli_ret != 0) {
		/*
		If cli_ret is not 0, the server thread might keep waiting connection from a client.
		Hence, to be clear that the server thread is stopped, making temporary connection with the server.
		*/
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = atoi(SERVER_PORT);
		server.sin_addr.s_addr = inet_addr(SERVER_ADDR);

		cli_ret = connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
		TLS_SECLINK_LOG("[SERVER] Cancel : %d --\n", cli_ret);
		close(sockfd);
	}

	/* Wait for the threads to stop */
	pthread_join(tls_server, (void *)&srv_ret);
	TLS_SECLINK_LOG("[SERVER] End : %d --\n", srv_ret);

exit:
	if (cli_ret == 0 && srv_ret == 0) {
		printf("-- [TLS SECLINK TEST] Success --\n");
		tls_print_test_log();
	} else {
		printf("-- [TLS SECLINK TEST] Fail --\n");
	}

	clock_settime(CLOCK_REALTIME, &ts_origin);
	printf("-- [TLS SECLINK TEST] DONE --\n");

	return 0;
}
