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

#include <tinyara/config.h>

#include <stdio.h>
#include <debug.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <wifi_manager/wifi_manager.h>
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include "wifi_manager_message.h"
#include "wifi_manager_log.h"

#define WIFIMGR_MSG_QUEUE_NAME "/dev/wifimgr_msg"
#define WIFIMGR_MSG_TAG_ERR "[ERR] "
#define MESSAGE_ERROR                                                        \
	do {                                                                     \
		WM_LOG_ERROR(WIFIMGR_MSG_TAG_ERR"%s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		WM_LOG_ERROR(WIFIMGR_MSG_TAG_ERR"err num(%d)\n", errno);                                \
	} while (0)

static inline int _send_message(int fd, void *buf, int buflen)
{
	int sent = 0;
	while (1) {
		int res = write(fd, (void *)buf + sent, buflen - sent);
		if (res < 0) {
			MESSAGE_ERROR;
			return -1;
		}
		sent += res;
		if (sent == buflen) {
			break;
		}
	}
	return 0;
}

static inline int _recv_message(int fd, void *buf, int buflen)
{
	int received = 0;
	while (1) {
		int res = read(fd, buf + received, buflen - received);
		if (res < 0) {
			MESSAGE_ERROR;
			return -1;
		}
		received += res;
		if (received == buflen) {
			break;
		}
	}
	return 0;
}

/*
 * Public functions
 */
int wifimgr_message_in(handler_msg *msg, handler_queue *queue)
{
	int fd = open(WIFIMGR_MSG_QUEUE_NAME, O_WRONLY);
	if (fd < 0) {
		MESSAGE_ERROR;
		return -1;
	}

	int res = _send_message(fd, (void *)msg, sizeof(handler_msg));
	if (res < 0) {
		MESSAGE_ERROR;
		close(fd);
		return -1;
	}

	close(fd);

	return 0;
}


// it's not thread-safe. in other words only one thread should call
// this function
int wifimgr_message_out(handler_msg *msg, handler_queue *queue)
{
	fd_set rfds = queue->rfds;
	int res = select(queue->max + 1, &rfds, NULL, NULL, NULL);
	if (res <= 0) {
		if (errno == EINTR) {
			return 1;
		}
		WM_LOG_ERROR("select error(%d)\n", res);
		MESSAGE_ERROR;
		return -1;
	}
	if (FD_ISSET(queue->fd, &rfds)) {
		res = _recv_message(queue->fd, (void *)msg, sizeof(handler_msg));
		if (res < 0) {
			MESSAGE_ERROR;
		}
		wifimgr_msg_s *wmsg = msg->msg;
		wmsg->result = wifimgr_handle_request(wmsg);
		if (msg->signal) {
			sem_post(msg->signal);
		}
	}
#ifdef CONFIG_LWNL80211
	if (FD_ISSET(queue->nd, &rfds)) {
		res = lwnl_fetch_event(queue->nd, (void *)msg, sizeof(handler_msg));
		if (res < 0) {
			MESSAGE_ERROR;
		}
		wifimgr_msg_s *wmsg = msg->msg;
		wmsg->result = wifimgr_handle_request(wmsg);
		if (msg->signal) {
			sem_post(msg->signal);
		}
	}
#endif
	return 0;
}

int wifimgr_create_msgqueue(handler_queue *queue)
{
	queue->fd = queue->nd = queue->max = 0;
	FD_ZERO(&queue->rfds);

	int res = mkfifo(WIFIMGR_MSG_QUEUE_NAME, 0666);
	if (res < 0 && res != -EEXIST) {
		MESSAGE_ERROR;
		return -1;
	}

	queue->fd = open(WIFIMGR_MSG_QUEUE_NAME, O_RDWR);
	if (queue->fd < 0) {
		MESSAGE_ERROR;
		unlink(WIFIMGR_MSG_QUEUE_NAME);
		return -1;
	}
	FD_SET(queue->fd, &queue->rfds);

#ifdef CONFIG_LWNL80211
	queue->nd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (queue->nd < 0) {
		close(queue->fd);
		MESSAGE_ERROR;
		return -1;
	}

	res = bind(queue->nd, NULL, 0);
	if (res < 0) {
		close(queue->fd);
		close(queue->nd);
		MESSAGE_ERROR;
		return -1;
	}
	FD_SET(queue->nd, &queue->rfds);
#endif
	queue->max = queue->fd > queue->nd ? queue->fd : queue->nd;

	return 0;
}
