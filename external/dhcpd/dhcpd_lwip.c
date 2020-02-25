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
#include <debug.h>				/* For ndbg, vdbg */
#include <tinyara/compiler.h>	/* For CONFIG_CPP_HAVE_WARNING */
#include <protocols/dhcpd.h>	/* Advertised DHCPD APIs */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <net/if.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <pthread.h>
#include <netutils/netlib.h>
#include <mqueue.h>
#include <sys/ioctl.h>
#include <netdb.h>

/****************************************************************************
 * Global Data
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/
#define DHCP_SERVER_PORT         67
#define DHCP_CLIENT_PORT         68

static pthread_t g_dhcpd_tid = -1;
static int g_dhcpd_term = 0;

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

void *_dhcpd_join_handler(void *arg)
{
	dhcp_join_data_s *data = (dhcp_join_data_s *)arg;
	int ret = ERROR;
	int sockfd = 0;
	struct req_lwip_data req;

	struct mq_attr attr;
	attr.mq_maxmsg = DHCPD_MQ_MAX_LEN;
	attr.mq_msgsize = DHCPD_MQ_LEN;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;


	mqd_t md = mq_open(DHCPD_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (md == (mqd_t)ERROR) {
		ndbg("Failed to open mq\n");
		goto go_out;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		ndbg("socket() failed with errno: %d\n", errno);
		goto go_out;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPDSTART;
	req.intf = data->intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		ndbg("ioctl() failed with errno: %d\n", errno);
		goto go_out;
	}

	while (1) {
		char msg[DHCPD_MQ_LEN];
		memset(msg, 0, DHCPD_MQ_LEN);
		int prio = 0;
		int nbytes = mq_receive(md, msg, DHCPD_MQ_LEN, &prio);

		if (nbytes <= 0) {
			if (errno != EAGAIN) {
				ndbg("mq receive none (errno %d)\n", errno);
				break;
			}
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
	if (md > 0) {
		mq_close(md);
	}
	if (sockfd > 0) {
		close(sockfd);
	}
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
		ndbg("socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPDSTATUS;
	req.intf = intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		ndbg("ioctl() failed with errno: %d\n", errno);
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
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = ERROR;
	dhcp_join_data_s *data = (dhcp_join_data_s *)malloc(sizeof(dhcp_join_data_s));
	if (!data) {
		ndbg("Failed to alloc mem for data\n");
		return ERROR;
	}
	data->intf = (char *)malloc(strlen(intf) + 1);
	if (!data->intf) {
		free(data);
		ndbg("Failed to alloc mem for data->intf\n");
		return ERROR;
	}
	memcpy(data->intf, intf, strlen(intf) + 1);
	data->fn = dhcp_join_cb;

	g_dhcpd_term = 0;
	if ((ret = pthread_attr_init(&attr)) != 0) {
		ndbg("Failed to init attr\n");
		free(data->intf);
		free(data);
		return ERROR;
	}
	sparam.sched_priority = 110;
	if ((ret = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		ndbg("Failed to set attr\n");
		free(data->intf);
		free(data);
		return ERROR;
	}
	ret = pthread_create(&g_dhcpd_tid, &attr, _dhcpd_join_handler, (void *)data);
	if (ret < 0) {
		free(data->intf);
		free(data);
		ndbg("create iotapi handler fail(%d) errno %d\n", ret, errno);
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
	int ret = ERROR;
	struct req_lwip_data req;

	g_dhcpd_term = 1;
	sleep(1);

	struct mq_attr attr;
	attr.mq_maxmsg = DHCPD_MQ_MAX_LEN;
	attr.mq_msgsize = DHCPD_MQ_LEN;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	mqd_t md = mq_open(DHCPD_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (md == (mqd_t)ERROR) {
		ndbg("Failed to open mq\n");
	}
	char msg[2] = {0, 0}; // terminate thread message
	int mq_ret = mq_send(md, msg, 2, 100);
	if (mq_ret == 0) {
		pthread_join(g_dhcpd_tid, NULL);
	} else {
		ndbg("send mq fail (errno %d)\n", errno);
	}

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		ndbg("socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPDSTOP;
	req.intf = intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		ndbg("ioctl() failed with errno: %d\n", errno);
		close(sockfd);
		return ret;
	}

	ret = req.req_res;
	close(sockfd);

	return ret;
}
