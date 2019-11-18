/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>
#include <debug.h>

#define DHCPD_MQ_NAME "dhcpd_queue"
#define DHCPD_MQ_LEN 11
#define DHCPD_MQ_MAX_LEN 20

typedef enum {
	DHCP_EVT_ACK,
	DHCP_EVT_NACK,
} dhcp_evt_type_e;

typedef struct dhcp_node {
	int ipaddr;
	char macaddr[6];
} dhcp_node_s;

void _dhcpd_join(dhcp_evt_type_e type, void *data)
{
	ndbg("dhcpd joined");

	struct mq_attr attr;
	attr.mq_maxmsg = DHCPD_MQ_MAX_LEN;
	attr.mq_msgsize = DHCPD_MQ_LEN;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;

	mqd_t md = mq_open(DHCPD_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (!md) {
		ndbg("mq open fail (errno %d)\n", errno);
		mq_close(md);
		return;
	}

	char msg[DHCPD_MQ_LEN];
	dhcp_node_s *node = (dhcp_node_s *)data;
	msg[0] = 1;
	memcpy(&msg[1], &node->ipaddr, 4);
	memcpy(&msg[5], &node->macaddr, 6);
	int mq_ret = mq_send(md, msg, DHCPD_MQ_LEN, 100);
	if (mq_ret < 0) {
		ndbg("send mq fail (errno %d)\n", errno);
	}

	mq_close(md);

	return;
}

