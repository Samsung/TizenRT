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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <tinyara/lwnl/lwnl.h>

/*
 * it's test code so it doesn't perform error handling strictly.
 */
static void *event_listener(void *arg)
{
	int res = 0;
	int idx = *((int *)arg);
	printf("[%d] run event listener\n", idx);
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		printf("[%d] fail to create socket\n", idx);
		return NULL;
	}

	res = bind(fd, NULL, 0);
	if (res < 0) {
		printf("[%d] bind fail errno(%d)\n", idx, errno);
		close(fd);
		return NULL;
	}

	fd_set rfds, ofds;
	FD_ZERO(&ofds);
	FD_SET(fd, &ofds);

	printf("[%d] run event loop\n", idx);
	for (;;) {
		rfds = ofds;
		res = select(fd + 1, &rfds, NULL, NULL, 0);
		if (res <= 0) {
			printf("[%d] select error res(%d) errno(%d))\n", idx, res, errno);
			return NULL;
		}

		if (FD_ISSET(fd, &rfds)) {
			char buf[8] = {0, };
			lwnl_cb_status status;
			uint32_t len;
			int nbytes = read(fd, (char *)buf, 8);
			if (nbytes < 0) {
				printf("[%d] read error bytes(%d) errno(%d))\n", idx, nbytes, errno);
				return NULL;
			}
			memcpy(&status, buf, sizeof(lwnl_cb_status));
			memcpy(&len, buf + sizeof(lwnl_cb_status), sizeof(uint32_t));
			printf("[%d] status (%d) len(%d)\n", idx, status, len);

			// flush the event queue
			if (len > 0) {
				char *tmp = (char *)malloc(len);
				read(fd, tmp, len);
				free(tmp);
			}
		}
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
	int lwnl_sample_main(int argc, char *argv[])
#endif
{
	pthread_t *pid = NULL;
	int num_threads = 3;
	if (argc == 2) {
		/* the number of listener is dependent to lwnl queue size.
		 * please check LWNL_NPOLLWAITERS in lwnl_evt_queue.h.
		 */
		num_threads = atoi(argv[1]);
		pid = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
		if (!pid) {
			printf("allocate thread pid array fail\n");
			return -1;
		}
	} else if (argc != 1) {
		printf("invalid usage\n");
		printf("usage: lwnl_sample [number of threads]\n");
	}

	/*  run event listener thread */
	printf("Run %d listener threads\n", num_threads);
	for (int i = 0; i < num_threads; i++) {
		pthread_create((pid + i), NULL, event_listener, (void *)&i);
	}

	printf("Run wi-fi manager to generate events\n");
	/*  it's test program so it doesn't consider termination */
	free(pid);

	return 0;
}
