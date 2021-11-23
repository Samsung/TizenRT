/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "wm_test_log.h"
#include "wm_test_network.h"

#define WT_TARGET_ADDR CONFIG_LWIP_DHCPS_SERVER_IP
#define TAG "[NET]"
#define WT_BUF_SIZE 512

static int _send_data(int fd, int size)
{
	char buf[WT_BUF_SIZE];
	int remain = size;

	while (remain > 0) {
		int send_size = remain > WT_BUF_SIZE ? WT_BUF_SIZE : remain;
		int nbytes = send(fd, buf, send_size, 0);
		if (nbytes == 0) {
			WT_LOGE(TAG, "connection closed\n");
			return -1;
		} else if (nbytes < 0) {
			if (errno == EWOULDBLOCK) {
				continue;
			} else {
				WT_LOGE(TAG, "connection error %d\n", errno);
				return -1;
			}
		}
		remain -= nbytes;
	}
	return 0;
}

static int _recv_data(int fd, int size)
{
	char buf[WT_BUF_SIZE];
	int remain = size;

	while (remain > 0) {
		int read_size = remain > WT_BUF_SIZE ? WT_BUF_SIZE : remain;
		int nbytes = recv(fd, buf, read_size, 0);
		if (nbytes == 0) {
			WT_LOGE(TAG, "connection closed\n");
			return -1;
		} else if (nbytes < 0) {
			if (errno == EWOULDBLOCK) {
				continue;
			} else {
				WT_LOGE(TAG, "connection error %d\n", errno);
				return -1;
			}
		}
		remain -= nbytes;
	}
	return 0;
}

int wt_receive_dummy(int size)
{
	struct sockaddr_in saddr;
	struct sockaddr_in caddr;
	int listenfd = 0;
	int connfd = 0;
	int ret = 0;

	WT_LOG(TAG, "create socket");

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		WT_LOGE(TAG, "create socket fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "set reuseaddr");

	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		WT_LOGE(TAG, "set reuse addr fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "bind INADDR_ANY port:%d", WT_NET_PORT);

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = PF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(WT_NET_PORT);

	ret = bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0) {
		WT_LOGE(TAG, "bind fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "listen");

	ret = listen(listenfd, 1024);
	if (ret < 0) {
		WT_LOGE(TAG, "listen fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "wait connection");

	socklen_t clilen = sizeof(caddr);
	connfd = accept(listenfd, (struct sockaddr *)&caddr, &clilen);
	if (connfd < 0) {
		WT_LOGE(TAG, "accept fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "receive data size %d", size);

	ret = _recv_data(connfd, size);
	if (ret < 0) {
		WT_LOGE(TAG, "recv fail size %d ret %d code %d\n", size, ret, errno);
		return -1;
	}

	WT_LOG(TAG, "send ACK data size %d", WT_ACK_SIZE);

	ret = _send_data(connfd, WT_ACK_SIZE);
	if (ret < 0) {
		WT_LOGE(TAG, "send fail size %d ret %d code %d\n", WT_ACK_SIZE, ret, errno);
		return -1;
	}

	close(connfd);
	close(listenfd);

	WT_LOG(TAG, "terminate receive");

	return 0;
}

int wt_send_dummy(int size)
{
	struct sockaddr_in target_addr;
	int ret = 0;
	int sockfd = 0;

	WT_LOG(TAG, "create socket");

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		WT_LOGE(TAG, "create socket fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "connect to %s:%d", WT_TARGET_ADDR, WT_NET_PORT);

	/* Connect the socket to the server */
	target_addr.sin_family = AF_INET;
	target_addr.sin_port = HTONS(WT_NET_PORT);
	inet_pton(AF_INET, WT_TARGET_ADDR, &(target_addr.sin_addr));

	int addrlen = sizeof(struct sockaddr_in);
	if (connect(sockfd, (struct sockaddr *)&target_addr, addrlen) < 0) {
		WT_LOGE(TAG, "connect fail %d\n", errno);
		return -1;
	}

	WT_LOG(TAG, "send data size %d", size);

	ret = _send_data(sockfd, size);
	if (ret < 0) {
		WT_LOGE(TAG, "send fail size %d ret %d code %d\n", size, ret, errno);
		return -1;
	}

	WT_LOG(TAG, "recv ack size %d", WT_ACK_SIZE);

	ret = _recv_data(sockfd, WT_ACK_SIZE);
	if (ret < 0) {
		WT_LOGE(TAG, "receive fail size %d ret %d code %d\n", WT_ACK_SIZE, ret, errno);
		return -1;
	}
	close(sockfd);

	WT_LOG(TAG, "terminate sending");

	return 0;
}
