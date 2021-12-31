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
#include <sys/types.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/ble.h>
#include <tinyara/net/if/wifi.h>

#define NUM_WIFI_LISTENER 3
#define NUM_BLE_LISTENER 2

static char g_buf[512] = {1,};

struct ls_msg {
	lwnl_dev_type type;
	uint32_t num_events;
};

struct ls_event_list {
	uint32_t evt;
	uint32_t buf_size;
};

struct ls_event_list g_ble_events[] = {
	{LWNL_EVT_BLE_CLIENT_CONNECT, 0},
	{LWNL_EVT_BLE_CLIENT_DISCONNECT, 246},
	{LWNL_EVT_BLE_SCAN_STATE, 0},
};

struct ls_event_list g_wifi_events[] = {
	{LWNL_EVT_STA_CONNECTED, 0},
	{LWNL_EVT_STA_CONNECT_FAILED, 0},
	{LWNL_EVT_STA_DISCONNECTED, 0},
	{LWNL_EVT_SOFTAP_STA_JOINED, 0},
	{LWNL_EVT_SOFTAP_STA_LEFT, 0},
	{LWNL_EVT_SCAN_DONE, 256},
	{LWNL_EVT_SCAN_FAILED, 0}
};

static void print_usage(void)
{
	printf("lwnl_sample [type] [option]");
	printf("type:\n");
	printf("\twifi: generate wi-fi event listener\n");
	printf("\tble: generate BLE event listener\n");
	printf("option: the number of event listener\n");
	printf("\tevent: generate random events\n");
}

/*
 * it's test code so it doesn't perform error handling strictly.
 */
static void *_wifi_event_listener(void *arg)
{
	int res = 0;
	int total = 0;
	if (arg) {
		total = *((int *)arg);
	}
	int recved = 0;

	printf("[WIFI][T%d] run event listener total (%d)\n", getpid(), total);
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		printf("[WIFI][T%d] fail to create socket\n", getpid());
		return NULL;
	}

	struct sockaddr_lwnl addr = {LWNL_DEV_WIFI};
	res = bind(fd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_lwnl));
	if (res < 0) {
		printf("[WIFI][T%d] bind fail errno(%d)\n", getpid(), errno);
		close(fd);
		return NULL;
	}

	fd_set rfds, ofds;
	FD_ZERO(&ofds);
	FD_SET(fd, &ofds);

	printf("[WIFI][T%d] run event loop\n", getpid());
	for (;;) {
		if (total != 0 && recved == total) {
			break;
		}
		rfds = ofds;
		res = select(fd + 1, &rfds, NULL, NULL, 0);
		if (res <= 0) {
			printf("[WIFI][T%d] select error res(%d) errno(%d))\n", getpid(), res, errno);
			return NULL;
		}

		if (FD_ISSET(fd, &rfds)) {
			#define LWNL_TEST_HEADER_LEN (sizeof(lwnl_cb_status) + sizeof(uint32_t))

			char buf[LWNL_TEST_HEADER_LEN] = {0, };
			lwnl_cb_status status;
			uint32_t len;
			int nbytes = read(fd, (char *)buf, LWNL_TEST_HEADER_LEN);
			if (nbytes < 0) {
				printf("[WIFI][T%d] read error bytes(%d) errno(%d))\n", getpid(), nbytes, errno);
				return NULL;
			}
			memcpy(&status, buf, sizeof(lwnl_cb_status));
			memcpy(&len, buf + sizeof(lwnl_cb_status), sizeof(uint32_t));
			// flush the event queue
			if (len > 0) {
				char *tmp = (char *)malloc(len);
				read(fd, tmp, len);
				free(tmp);
			}
			printf("[WIFI][T%d] dev type (%d) evt (%d) len(%d)\n",
				   getpid(), status.type, status.evt, len);
			if (status.type != LWNL_DEV_WIFI) {
				printf("[WIFI][T%d] drop packet\n", getpid());
			}
		}
		recved++;
	}
	printf("[WIFI][T%d] exit event listener\n", getpid());
	return NULL;
}

static void *_ble_event_listener(void *arg)
{
	int res = 0;
	int total = 0;
	if (arg) {
		total = *((int *)arg);
	}
	int recved = 0;
	printf("[BLE][T%d] run event listener total %d\n", getpid(), total);
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		printf("[BLE][T%d] fail to create socket\n", getpid());
		return NULL;
	}

	struct sockaddr_lwnl addr = {LWNL_DEV_BLE};
	res = bind(fd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_lwnl));
	if (res < 0) {
		printf("[BLE][T%d] bind fail errno(%d)\n", getpid(), errno);
		close(fd);
		return NULL;
	}

	fd_set rfds, ofds;
	FD_ZERO(&ofds);
	FD_SET(fd, &ofds);

	printf("[BLE][T%d] run event loop\n", getpid());
	for (;;) {
		if (total != 0 && total == recved) {
			break;
		}
		rfds = ofds;
		res = select(fd + 1, &rfds, NULL, NULL, 0);
		if (res <= 0) {
			printf("[BLE][T%d] select error res(%d) errno(%d))\n", getpid(), res, errno);
			return NULL;
		}

		if (FD_ISSET(fd, &rfds)) {
			#define LWNL_TEST_HEADER_LEN (sizeof(lwnl_cb_status) + sizeof(uint32_t))

			char buf[LWNL_TEST_HEADER_LEN] = {0, };
			lwnl_cb_status status;
			uint32_t len;
			printf("[T%d] -->%s %d\n", getpid(), __FUNCTION__, __LINE__);
			int nbytes = read(fd, (char *)buf, LWNL_TEST_HEADER_LEN);
			printf("[T%d] <--%s %d\n", getpid(), __FUNCTION__, __LINE__);
			if (nbytes < 0) {
				printf("[BLE][T%d] read error bytes(%d) errno(%d))\n", getpid(), nbytes, errno);
				return NULL;
			}
			memcpy(&status, buf, sizeof(lwnl_cb_status));
			memcpy(&len, buf + sizeof(lwnl_cb_status), sizeof(uint32_t));
			// flush the event queue
			if (len > 0) {
				char *tmp = (char *)malloc(len);
				read(fd, tmp, len);
				free(tmp);
			}
			printf("[BLE][T%d] dev type (%d) evt (%d) len(%d)\n",
				   getpid(), status.type, status.evt, len);
			if (status.type != LWNL_DEV_BLE) {
				printf("[BLE][T%d] drop packet\n", getpid());
			}
		}
		recved++;
	}
	printf("[BLE][T%d] exit event listener\n", getpid());
	return NULL;
}

static void *_generate_random_event(void *arg)
{
	lwnl_postmsg(LWNL_DEV_BLE, LWNL_EVT_BLE_CLIENT_CONNECT, NULL, 0);
	lwnl_postmsg(LWNL_DEV_WIFI, LWNL_EVT_STA_CONNECTED, NULL, 0);

	lwnl_postmsg(LWNL_DEV_WIFI, LWNL_EVT_SCAN_DONE, g_buf, 24);

	return NULL;
}

static void *_generate_event(void *arg)
{
	struct ls_msg *msg = (struct ls_msg *)arg;
	lwnl_dev_type type = msg->type;
	uint32_t num_events = msg->num_events;
	printf("[LS] generate events type(%d) num(%d)\n", type, num_events);
	uint32_t evt_list_size = sizeof(g_ble_events)/sizeof(struct ls_event_list);
	struct ls_event_list *list = g_ble_events;

	if (type == LWNL_DEV_WIFI) {
		evt_list_size = sizeof(g_wifi_events)/sizeof(struct ls_event_list);
		list = g_wifi_events;
	}

	for (int i = 0; i < num_events; i++) {
		int idx = i % evt_list_size;
		char *buf = NULL;
		if (list[idx].buf_size != 0) {
			buf = g_buf;
		}
		printf("[LS] generate evt dev %d type %d size %d\n",
			   type, list[idx].evt, list[idx].buf_size);
		lwnl_postmsg(type, list[idx].evt, buf, list[idx].buf_size);
	}
	printf("[LS] exit event generator\n");
	return NULL;
}

static void _run_procedure(void)
{
	int res = 0;
	int num_wifi_events = 30;
	int num_ble_events = 20;

	pthread_t wpid[NUM_WIFI_LISTENER] = {0,};
	pthread_t bpid[NUM_BLE_LISTENER] = {0,};
	pthread_t evt_pid[2] = {0,};
	for (int i = 0; i < NUM_WIFI_LISTENER; i++) {
		res = pthread_create(&wpid[i], NULL, _wifi_event_listener, (void *)&num_wifi_events);
		if (res != 0) {
			printf("wifi pthread create fail %d\n", res);
		}
	}
	for (int i = 0; i < NUM_BLE_LISTENER; i++) {
		res = pthread_create(&bpid[i], NULL, _ble_event_listener, (void *)&num_ble_events);
		if (res != 0) {
			printf("ble pthread create fail %d\n", res);
		}
	}

	struct ls_msg msg = {LWNL_DEV_BLE, num_ble_events};

	res = pthread_create(&evt_pid[0], NULL, _generate_event, (void *)&msg);
	if (res != 0) {
		printf("ble event generate fail %d\n", res);
	}

	msg.type = LWNL_DEV_WIFI;
	msg.num_events = num_wifi_events;
	res = pthread_create(&evt_pid[1], NULL, _generate_event, (void *)&msg);
	if (res != 0) {
		printf("wifi event generate fail %d\n", res);
	}

	for (int i = 0; i < 2; i++) {
		res = pthread_join(evt_pid[i], NULL);
		if (res != 0) {
			printf("join %d event generate thread fail %d\n", i, res);
		}
	}

	for (int i = 0; i < NUM_BLE_LISTENER; i++) {
		res = pthread_join(bpid[i], NULL);
		if (res != 0) {
			printf("join ble event listener fail %d %d\n", i, res);
		}
	}

	for (int i = 0; i < NUM_WIFI_LISTENER; i++) {
		res = pthread_join(wpid[i], NULL);
		if (res != 0) {
			printf("join wifi event listener fail %d %d\n", i, res);
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
	pthread_startroutine_t func = NULL;
	if (argc != 3 && argc != 2) {
		print_usage();
		return -1;
	}
	if (argc == 3) {
		/* the number of listener is dependent to lwnl queue size.
		 * please check LWNL_NPOLLWAITERS in lwnl_evt_queue.h.
		 */
		if (strncmp(argv[1], "wifi", strlen("wifi") + 1) == 0) {
			func = _wifi_event_listener;
		} else if (strncmp(argv[1], "ble", strlen("ble") + 1) == 0) {
			func = _ble_event_listener;
		} else {
			print_usage();
			return -1;
		}
		num_threads = atoi(argv[2]);
		pid = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
		if (!pid) {
			printf("allocate thread pid array fail\n");
			return -1;
		}
		printf("Run %d listener threads\n", num_threads);
		for (int i = 0; i < num_threads; i++) {
			pthread_create((pid + i), NULL, func, (void *)&i);
			pthread_detach(*(pid + i));
		}
	} else if (argc == 2 && strncmp(argv[1], "event", strlen("event") + 1) == 0) {
		pthread_t eid;
		pthread_create(&eid, NULL, _generate_random_event, NULL);
		pthread_detach(eid);
	} else if (argc == 2 && strncmp(argv[1], "full", strlen("full") + 1) == 0) {
		_run_procedure();
	}

	return 0;
}
