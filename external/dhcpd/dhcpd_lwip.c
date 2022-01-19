/****************************************************************************
 * netutils/dhcpd/dhcpd.c
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
 * netutils/dhcpd/dhcpd.c
 *
 *   Copyright (C) 2007-2009, 2011-2014 Gregory Nutt. All rights reserved.
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


#include <tinyara/config.h>		/* TinyAra configuration */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <debug.h>				/* For printf, vdbg */
#include <tinyara/compiler.h>	/* For CONFIG_CPP_HAVE_WARNING */
#include <tinyara/netmgr/netctl.h>
#include <protocols/dhcpd.h>	/* Advertised DHCPD APIs */
#include <netutils/netlib.h>

/****************************************************************************
 * Global Data
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/
#define DHCP_SERVER_PORT         67
#define DHCP_CLIENT_PORT         68

static pthread_t g_dhcpd_tid = -1;

struct dhcp_join_data {
	char *intf;
	dhcp_sta_joined_cb fn;
};
typedef struct dhcp_join_data dhcp_join_data_s;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#define DHCPD_MQ_NAME "dhcpd_queue"
#define DHCPD_MQ_LEN 11
#define DHCPD_MQ_MAX_LEN 20

static int _send_mqmsg(mqd_t mq, char *msg, size_t msglen, int prio)
{
	printf("dhcps send dhcps msg size %d\n", msglen);
	while (1) {
		int mq_ret = mq_send(mq, msg, msglen, prio);
		if (mq_ret == ERROR) {
			int err_no = get_errno();
			if (err_no == EINTR || err_no == EAGAIN) {
				continue;
			}
			printf("dhcps _send_mqmsg error %d\n", err_no);
			return -1;
		}
		printf("dhcps sent res %d sent %d\n", mq_ret, msglen);
		break;
	}
	return 0;
}

static int _recv_mqmsg(mqd_t mq, char *msg, size_t msglen, int *prio)
{
	printf("dhcps receive dhcps msg size %d\n", msglen);
	while (1) {
		int mq_ret = mq_receive(mq, msg, msglen, prio);
		if (mq_ret == -1) {
			int err_no = get_errno();
			if (err_no == EINTR || err_no == EAGAIN) {
				continue;
			}
			printf("dhcps error %d\n", err_no);
			return -1;
		}
		if (mq_ret == msglen) {
			printf("dhcps recv message size (%d)\n", mq_ret);
			break;
		} else {
			printf("dhcps critical error %d %d\n", mq_ret, msglen);
			assert(0);
		}
	}
	return 0;
}

static void _empty_mq(mqd_t mq)
{
	printf("dhcps empty dhcps message queue\n");
	char msg[DHCPD_MQ_LEN];

	while (1) {
		struct mq_attr mq_stat;
		mq_getattr(mq, &mq_stat);
		if (mq_stat.mq_curmsgs == 0) {
			printf("dhcps dhcps msg queue is empty\n");
			return;
		}

		struct timespec ts;
		while (1) {
			int status = clock_gettime(CLOCK_REALTIME, &ts);
			if (status == 0) {
				break;
			}
			printf("fail to get time %d\n", errno);
			usleep(1000); // 1ms sleep
			continue;
		}
		ts.tv_nsec += 5000000; // 5ms
		ts.tv_sec = 0;
		int nbytes = mq_timedreceive(mq, msg, DHCPD_MQ_LEN, 0, &ts);
		if (nbytes == -1) {
			int err_no = get_errno();
			printf("dhcps empty dhcps message queue errno %d\n", err_no);
			if (err_no == ETIMEDOUT) {
				break;
			} else if (err_no == EINTR || err_no == EAGAIN) {
				continue;
			} else {
				printf("dhcps critical error errno %d\n", err_no);
				assert(0);
			}
		}
		printf("received msg length(%d) msg(%d)\n", nbytes, msg[0]);
	}
	printf("dhcps empty mq\n");
}

static void _create_mq(mqd_t *mq)
{
	printf("dhcps create queue\n");
	struct mq_attr attr;
	attr.mq_maxmsg = DHCPD_MQ_MAX_LEN;
	attr.mq_msgsize = DHCPD_MQ_LEN;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	*mq = mq_open(DHCPD_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (*mq == (mqd_t)ERROR) {
		printf("dhcps Failed to open mq errno(%d)\n", errno);
		assert(0);
		return;
	}
	printf("dhcps create mq\n");
}

static void _delete_mq(mqd_t mq)
{
	printf("dhcps delete mq\n");
	if (mq_close(mq) != 0) {
		printf("dhcps fail to delete mq\n");
		return;
	}
}

void *_dhcpd_join_handler(void *arg)
{
	printf("dhcps dhcps start join handler (app)\n");
	dhcp_join_data_s *data = (dhcp_join_data_s *)arg;
	int ret = ERROR;
	int sockfd = 0;
	struct req_lwip_data req;
	mqd_t md = (mqd_t)-1;

	_create_mq(&md);

	_empty_mq(md);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("dhcps socket() failed with errno: %d\n", errno);
		goto go_out;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPDSTART;
	req.msg.dhcp.intf = data->intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("dhcps ioctl() failed with errno: %d\n", errno);
		goto go_out;
	}

	while (1) {
		char msg[DHCPD_MQ_LEN];
		memset(msg, 0, DHCPD_MQ_LEN);
		int prio = 0;
		int nbytes = _recv_mqmsg(md, msg, DHCPD_MQ_LEN, &prio);
		if (nbytes < 0) {
			printf("dhcps mq receive none\n");
			break;
		} else if (msg[0] == 0) {// terminate thread
			break;
		} else if (msg[0] == 1) {
			dhcp_node_s node;
			memcpy(&node.ipaddr, &msg[1], 4);
			memcpy(&node.macaddr, &msg[5], 6);
			data->fn(DHCP_ACK_EVT, &node);
		}
	}

go_out:
	free(data->intf);
	free(data);
	_delete_mq(md);
	if (sockfd > 0) {
		close(sockfd);
	}
	printf("exit dhcps join handler (app)\n");
	return NULL;
}

/****************************************************************************
 * Global Functions
 ****************************************************************************/
/****************************************************************************
 * Name: dhcps_server_status
 ****************************************************************************/
int dhcp_server_status(char *intf)
{
	int ret = ERROR;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("dhcps socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPDSTATUS;
	req.msg.dhcp.intf = intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("dhcps ioctl() failed with errno: %d\n", errno);
		close(sockfd);
		return ret;
	}

	ret = req.req_res;
	close(sockfd);
	return ret;
}

/****************************************************************************
 * Name: dhcps_server_start
 ****************************************************************************/
int dhcp_server_start(char *intf, dhcp_sta_joined_cb dhcp_join_cb)
{
	printf("dhcps start dhcp server\n");
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = ERROR;
	dhcp_join_data_s *data = (dhcp_join_data_s *)malloc(sizeof(dhcp_join_data_s));
	if (!data) {
		printf("dhcps Failed to alloc mem for data\n");
		return ERROR;
	}
	data->intf = (char *)malloc(strlen(intf) + 1);
	if (!data->intf) {
		free(data);
		printf("dhcps Failed to alloc mem for data->intf\n");
		return ERROR;
	}
	memcpy(data->intf, intf, strlen(intf) + 1);
	data->fn = dhcp_join_cb;

	if ((ret = pthread_attr_init(&attr)) != 0) {
		printf("dhcps Failed to init attr\n");
		free(data->intf);
		free(data);
		return ERROR;
	}
	sparam.sched_priority = 110;
	if ((ret = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("dhcps Failed to set attr\n");
		free(data->intf);
		free(data);
		return ERROR;
	}
	ret = pthread_create(&g_dhcpd_tid, &attr, _dhcpd_join_handler, (void *)data);
	if (ret != OK) {
		free(data->intf);
		free(data);
		printf("dhcps create dhcp handler fail(%d) errno %d\n", ret, errno);
		return ERROR;
	}
	pthread_setname_np(g_dhcpd_tid, "dhcpd cb handler");
	return ret;
}

/****************************************************************************
 * Name: dhcps_server_stop
 ****************************************************************************/
int dhcp_server_stop(char *intf)
{
	printf("dhcps stop dhcp server\n");
	int ret = ERROR;
	struct req_lwip_data req;
	mqd_t md = (mqd_t)-1;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("dhcps socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPDSTOP;
	req.msg.dhcp.intf = intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("dhcps ioctl() failed with errno: %d\n", errno);
		close(sockfd);
		return ret;
	}

	ret = req.req_res;
	close(sockfd);

	_create_mq(&md);

	printf("dhcps send terminate msg\n");
	char msg[DHCPD_MQ_LEN] = {0}; // terminate thread message
	int mq_ret = _send_mqmsg(md, msg, DHCPD_MQ_LEN, 100);
	if (mq_ret == 0) {
		printf("dhcps send success\n");
		pthread_join(g_dhcpd_tid, NULL);
	} else {
		printf("dhcps send mq critical error\n");
		assert(0);
	}

	_delete_mq(md);

	return ret;
}
