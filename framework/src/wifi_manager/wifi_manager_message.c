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
#include <sys/types.h>
#include <semaphore.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <wifi_manager/wifi_manager.h>
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include "wifi_manager_message.h"
#include <tinyara/net/netlog.h>

#define WIFIMGR_MSG_QUEUE_NAME "/dev/wifimgr_msg"
#define TAG "[WM]"
static int message_in_fd;

static inline int _send_message(int fd, void *buf, int buflen)
{
	int sent = 0;
	NET_LOGE(TAG, "0 _send_message  %d\n", sent);
	while (1) {
		int res = write(fd, (void *)buf + sent, buflen - sent);
		if (res < 0) {
			int err_no = get_errno();
			if (err_no == EAGAIN || err_no == EINTR) {
				NET_LOGE(TAG, "1 _send_message  %d\n", err_no);
				continue;
			}
			NET_LOGE(TAG, "write error %d\n", err_no);
			return -1;
		}
		sent += res;
		if (sent == buflen) {
			NET_LOGE(TAG, "1 _send_message  %d  \n", sent);
			break;
		}
		NET_LOGE(TAG, "2 _send_message  %d  -  %d\n", buflen, sent);
	}
	NET_LOGE(TAG, "3 _send_message  %d\n", sent);
	return 0;
}

static inline int _recv_message(int fd, void *buf, int buflen)
{
	int received = 0;
	while (1) {
		int res = read(fd, buf + received, buflen - received);
		NET_LOGE(TAG, "0 _recv_message %d\n", res);
		if (res < 0) {
			int err_no = get_errno();
			if (err_no == EAGAIN || err_no == EINTR) {
				NET_LOGE(TAG, "1 _recv_message %d\n", err_no);
				continue;
			}
			NET_LOGE(TAG, "read error %d\n", err_no);
			return -1;
		}
		received += res;
		if (received == buflen) {
			NET_LOGE(TAG, "2 _recv_message %d\n", received);
			break;
		}
		NET_LOGE(TAG, "3 _recv_message %d  - %d\n", buflen, received);
	}
	return 0;
}

/*
 * Public functions
 */
int wifimgr_message_in(handler_msg *msg, handler_queue *queue)
{
	NET_LOGE(TAG, "0 wifimgr_message_in in  %d\n", 0);
	// int fd = open(WIFIMGR_MSG_QUEUE_NAME, 1 << 1);
	int fd = message_in_fd;
	if (fd < 0) {
		NET_LOGE(TAG, "open error %d\n", errno);
		return -1;
	}

	int res = _send_message(fd, (void *)msg, sizeof(handler_msg));
	NET_LOGE(TAG, "1 wifimgr_message_in  %d\n", res);
	// close(fd);
	NET_LOGE(TAG, "2 wifimgr_message_in close  %d\n", res);
	if (res < 0) {
		return -1;
	}
	return 0;
}


// it's not thread-safe. in other words only one thread should call
// this function
int wifimgr_message_out(handler_msg *msg, handler_queue *queue)
{
	
	fd_set rfds = queue->rfds;
	NET_LOGE(TAG, "1 message out  \n");
	int res = select(queue->max + 1, &rfds, NULL, NULL, NULL);
	NET_LOGE(TAG, "2 message out select  %d\n", res);
	if (res <= 0) {
		int err_no = get_errno();
		if (err_no == EINTR) {
			return 1;
		}
		NET_LOGE(TAG, "select error(%d)\n", err_no);
		return -1;
	}
	if (FD_ISSET(queue->fd, &rfds)) {
		res = _recv_message(queue->fd, (void *)msg, sizeof(handler_msg));
		if (res < 0) {
			NET_LOGE(TAG, "critical error%d\n",1);
		} else {
			wifimgr_msg_s *wmsg = msg->msg;
			NET_LOGE(TAG, "3 message out select  %d\n", res);
			wmsg->result = wifimgr_handle_request(wmsg);
			NET_LOGE(TAG, "4 message out select  %d\n", res);
			if (msg->signal) {
				sem_post(msg->signal);
				NET_LOGE(TAG, "5 message out select  %d\n", res);
			}
		}
	}
#ifdef CONFIG_LWNL80211
	if (FD_ISSET(queue->nd, &rfds)) {
		res = lwnl_fetch_event(queue->nd, (void *)msg, sizeof(handler_msg));
		if (res < 0) {
			NET_LOGE(TAG, "critical error\n");
		} else {
			wifimgr_msg_s *wmsg = msg->msg;
			if (wmsg) {
				wmsg->result = wifimgr_handle_request(wmsg);
			}
			if (msg->signal) {
				sem_post(msg->signal);
			}
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
		NET_LOGE(TAG, "create wifimgr msg queue fail %d\n", errno);
		return -1;
	}

	queue->fd = open(WIFIMGR_MSG_QUEUE_NAME, 1 << 0 | 1 << 1);
	if (queue->fd < 0) {
		NET_LOGE(TAG, "open wifimgr msg queue fail %d\n", errno);
		unlink(WIFIMGR_MSG_QUEUE_NAME);
		return -1;
	}
	FD_SET(queue->fd, &queue->rfds);
	message_in_fd = open(WIFIMGR_MSG_QUEUE_NAME, 1 << 1);

#ifdef CONFIG_LWNL80211
	queue->nd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (queue->nd < 0) {
		close(queue->fd);
		NET_LOGE(TAG, "socket open fail %d\n", errno);
		return -1;
	}

	struct sockaddr_lwnl addr = {LWNL_DEV_WIFI};
	res = bind(queue->nd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_lwnl));
	if (res < 0) {
		close(queue->fd);
		close(queue->nd);
		NET_LOGE(TAG, "bind fail %d\n", errno);
		return -1;
	}
	FD_SET(queue->nd, &queue->rfds);
#endif
	queue->max = queue->fd > queue->nd ? queue->fd : queue->nd;

	return 0;
}
