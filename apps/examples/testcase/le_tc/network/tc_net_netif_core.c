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

/// @file tc_net_core.c
/// @brief Test Case Example for api() API
#include <net/lwip/stats.h>
#include <net/lwip/tcpip.h>
#include <net/lwip/arch/sys_arch.h>
#include <net/lwip/ipv4/ip.h>
#include <net/lwip/tcp.h>
#include <net/lwip/netif/etharp.h>
#include <net/lwip/tcp_impl.h>
#include <net/lwip/pbuf.h>

#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include "tc_internal.h"

#define PORTNUM 5555
#define MAXRCVLEN 20

struct netif loop_netif;
struct netif test_netif;
static ip_addr_t test_ipaddr, test_netmask, test_gw;
static int linkoutput_ctr;
extern err_t netif_loopif_init(struct netif *netif);

/**
* @testcase				: tc_net_netif_remove_p
* @brief				:
* @scenario				:
* @apicovered			: netif_remove(), netif_add()
* @precondition			:
* @postcondition		:
*/
static void tc_net_netif_remove_p(void)
{
	struct netif *ret;

	ip_addr_t loop_ipaddr, loop_netmask, loop_gw;
	IP4_ADDR(&loop_gw, 127, 0, 0, 1);
	IP4_ADDR(&loop_ipaddr, 127, 0, 0, 1);
	IP4_ADDR(&loop_netmask, 255, 0, 0, 0);

	ret = netif_add(&loop_netif, &loop_ipaddr, &loop_netmask, &loop_gw, NULL, netif_loopif_init, ip_input);
	netif_remove(ret);

	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_get_at_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_get_at()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_get_at_p(void)
{
	struct pbuf *p;
	u16_t offset = 0;
	u8_t ret;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->len = 6;
	p->payload = "packet";

	ret = pbuf_get_at(p, offset);
	memp_free(MEMP_PBUF_POOL, p);
	TC_ASSERT_NEQ("tc_net_pbuf_get_at_p", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: create_packet
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
void create_packet(struct pbuf **head, char *data, u16_t len, u16_t ref)
{
	struct pbuf *node;
	struct pbuf *q;

	node = (struct pbuf *)memp_malloc(sizeof(struct pbuf));

	node->payload = data;
	node->next = NULL;
	node->flags = 0;
	node->len = len;
	node->tot_len = 16;
	node->type = PBUF_RAM;
	node->ref = ref;

	if (*head == NULL) {
		*head = node;
	} else {
		for (q = *head; q->next != NULL; q = q->next) ;
		q->next = node;
	}
}

/**
* @testcase				: tc_net_pbuf_memcmp_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_memcmp()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_memcmp_p(void)
{
	u16_t ret;
	struct pbuf *head = NULL;
	int pk_count;
	u16_t offset = 6;
	const void *s2 = "is";
	u16_t n = 2;

	for (pk_count = 0; pk_count < 2; pk_count++) {
		switch (pk_count) {
		case 0:
			create_packet(&head, "packet", strlen("packet"), 1);
			break;
		case 1:
			create_packet(&head, "is", (strlen("is") + strlen("packet")), 2);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_memcmp(head, offset, s2, n);

	TC_ASSERT_EQ("tc_net_pbuf_memcmp_p", ret, 0xffff);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_memfind_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_memfind()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_memfind_p(void)
{
	u16_t ret;
	struct pbuf *p = (struct pbuf *)"packet not found";
	const void *mem = "packet";
	u16_t mem_len = 1;
	u16_t start_offset = 0;
	p->tot_len = 16;
	p->len = 16;

	ret = pbuf_memfind(p, mem, mem_len, start_offset);

	TC_ASSERT_NEQ("tc_net_pbuf_memfind_p", ret, 0xFFFF);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_pbuf_strstr_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_strstr()
* @precondition			:
* @postcondition		:
*/
void tc_net_pbuf_strstr_p(void)
{
	struct pbuf *head = NULL;
	int pk_count;
	const char *substr = "found";
	u16_t ret;

	for (pk_count = 0; pk_count < 4; pk_count++) {
		switch (pk_count) {
		case 0:
			create_packet(&head, "packet", strlen("packet"), 1);
			break;
		case 1:
			create_packet(&head, "is", strlen("is"), 2);
			break;
		case 2:
			create_packet(&head, "found", strlen("found"), 3);
			break;
		case 3:
			create_packet(&head, "end", strlen("end"), 4);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_strstr(head, substr);

	TC_ASSERT_NEQ("tc_net_pbuf_strstr_p", ret, 0xFFFF);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_get_socket_struct_n
* @brief				:
* @scenario				:
* @apicovered			: get_socket_struct()
* @precondition			:
* @postcondition		:
*/
static void tc_net_get_socket_struct_n(void)
{
	struct socket *ret;
	int s = NEG_VAL;

	ret = (struct socket *)get_socket_struct(s);

	TC_ASSERT_EQ("tc_net_get_socket_struct_n", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_get_socket_struct_p
* @brief				:
* @scenario				:
* @apicovered			: get_socket_struct()
* @precondition			:
* @postcondition		:
*/
static void tc_net_get_socket_struct_p(void)
{
	struct socket *ret;
	int s = socket(AF_INET, SOCK_STREAM, 0);

	ret = (struct socket *)get_socket_struct(s);

	TC_ASSERT_EQ("tc_net_get_socket_struct_p", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_tcp_rexmit_fast_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_rexmit_fast()
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_rexmit_fast_n(void)
{
	struct tcp_pcb *pcb;
	pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);

	pcb->unacked = NULL;
	pcb->flags = ((u8_t) 0x04);

	tcp_rexmit_fast(pcb);
	memp_free(MEMP_TCP_PCB, pcb);

	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_tcp_zero_window_probe_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_zero_window_probe()
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_zero_window_probe_n(void)
{
	struct tcp_pcb *pcb;
	pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);

	pcb->unacked = NULL;
	pcb->unsent = NULL;

	tcp_zero_window_probe(pcb);
	memp_free(MEMP_TCP_PCB, pcb);
	pcb = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_netif_remove_n
* @brief				:
* @scenario				:
* @apicovered			: netif_remove()
* @precondition			:
* @postcondition		:
*/
static void tc_net_netif_remove_n(void)
{
	struct netif *netif = NULL;

	netif_remove(netif);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_alloced_custom_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_alloced_custom()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_alloced_custom_n(void)
{
	struct pbuf *ret;
	pbuf_layer l = 4;
	u16_t length = 2;
	pbuf_type type = 4;
	struct pbuf_custom *p = NULL;
	void *payload_mem = NULL;
	u16_t payload_mem_len = 1;

	ret = pbuf_alloced_custom(l, length, type, p, payload_mem, payload_mem_len);

	TC_ASSERT_EQ("tc_net_pbuf_alloced_custom_n", ret, NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_pbuf_alloced_custom_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_alloced_custom()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_alloced_custom_p(void)
{
	struct pbuf *ret;
	struct pbuf_custom *p;
	pbuf_layer l = 0;
	u16_t length = 5;
	pbuf_type type = 0;
	void *payload_mem = "payload big";
	u16_t payload_mem_len = 11;

	p = (struct pbuf_custom *)memp_malloc(sizeof(struct pbuf_custom));
	int pbuf_layer = 0;
	do {
		switch (l) {
		case 0:
			ret = pbuf_alloced_custom(0, length, type, p, payload_mem, payload_mem_len);
			break;
		case 1:
			ret = pbuf_alloced_custom(1, length, type, p, payload_mem, payload_mem_len);
			break;
		case 2:
			ret = pbuf_alloced_custom(2, length, type, p, payload_mem, payload_mem_len);
			break;
		case 3:
			ret = pbuf_alloced_custom(3, length, type, p, payload_mem, payload_mem_len);
			break;
		}
		pbuf_layer++;
	} while (pbuf_layer <= 3);

	TC_ASSERT_EQ("tc_net_pbuf_alloced_custom_p", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_dechain_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_dechain()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_dechain_n(void)
{
	struct pbuf *ret;
	struct pbuf *p;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->next = NULL;

	ret = pbuf_dechain(p);
	memp_free(MEMP_PBUF_POOL, p);
	TC_ASSERT_EQ("tc_net_pbuf_dechain_n", ret, NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: create_chain
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
void create_chain(struct pbuf **head, char *data, u16_t len, u16_t ref)
{
	struct pbuf *node;
	struct pbuf *q;

	node = (struct pbuf *)memp_malloc(sizeof(struct pbuf));

	node->payload = data;
	node->next = NULL;
	node->flags = 0;
	node->len = len;
	node->tot_len = 16;
	node->type = PBUF_RAM;
	node->ref = ref;

	if (*head == NULL) {
		*head = node;
	} else {
		for (q = *head; q->next != NULL; q = q->next) ;
		q->next = node;
	}
}

/**
* @testcase				: tc_net_pbuf_dechain_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_dechain()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_dechain_p(void)
{
	struct pbuf *head = NULL;
	int pk_count;
	struct pbuf *ret;

	for (pk_count = 0; pk_count < 2; pk_count++) {
		switch (pk_count) {
		case 0:
			create_chain(&head, "packet", strlen("packet"), 1);
			break;
		case 1:
			create_chain(&head, "is", strlen("is"), 2);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_dechain(head);

	TC_ASSERT_NEQ("tc_net_pbuf_dechain_p", ret, NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_pbuf_take_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_take()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_take_n(void)
{
	struct pbuf *buf = NULL;
	const void *dataptr = NULL;
	err_t ret;
	u16_t len = 1;

	ret = pbuf_take(buf, dataptr, len);

	TC_ASSERT_EQ("tc_net_pbuf_take_n", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_take_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_take()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_take_p(void)
{
	err_t ret;
	struct pbuf *buf = NULL;
	void *dataptr = "data";

	buf = (struct pbuf *)memp_malloc(10);
	buf->next = NULL;
	buf->len = 8;
	buf->tot_len = 10;

	ret = pbuf_take(buf, dataptr, strlen("data"));

	TC_ASSERT_EQ("tc_net_pbuf_take_p", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_coalesce_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_coalesce()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_coalesce_n(void)
{
	struct pbuf *ret = NULL;
	struct pbuf *p = NULL;
	pbuf_layer layer;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->next = NULL;
	layer = 4;

	ret = pbuf_coalesce(p, layer);
	memp_free(MEMP_PBUF_POOL, p);
	TC_ASSERT_EQ("tc_net_pbuf_coalesce_n", ret, p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_coalesce_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_coalesce()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_coalesce_p(void)
{
	int pk_count;
	struct pbuf *ret = NULL;
	pbuf_layer layer;
	struct pbuf *head = NULL;

	layer = 3;

	for (pk_count = 0; pk_count < 2; pk_count++) {
		switch (pk_count) {
		case 0:
			create_chain(&head, "packet", strlen("packet"), 1);
			break;
		case 1:
			create_chain(&head, "is", strlen("is"), 2);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_coalesce(head, layer);

	TC_ASSERT_NEQ("tc_net_pbuf_coalesce_p", ret, head);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_get_at_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_get_at()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_get_at_n(void)
{
	struct pbuf *p = NULL;
	u16_t offset = 0;
	u8_t ret;

	ret = pbuf_get_at(p, offset);
	TC_ASSERT_EQ("tc_net_pbuf_get_at_n", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_memcmp_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_memcmp()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_memcmp_n(void)
{
	u16_t ret;
	struct pbuf *p = NULL;
	u16_t offset = 0;
	const void *s2 = NULL;
	u16_t n = 0;

	ret = pbuf_memcmp(p, offset, s2, n);

	TC_ASSERT_EQ("tc_net_pbuf_memcmp_n", ret, 0xffff);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_memfind_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_memfind()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_memfind_n(void)
{
	u16_t ret;
	struct pbuf *p = NULL;
	const void *mem = NULL;
	u16_t mem_len = 1;
	u16_t start_offset = 1;

	ret = pbuf_memfind(p, mem, mem_len, start_offset);

	TC_ASSERT_EQ("tc_net_pbuf_memfind_n", ret, 0xffff);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_pbuf_strstr_n
* @brief				:
* @scenario				:
* @apicovered			: pbuf_strstr()
* @precondition			:
* @postcondition		:
*/
static void tc_net_pbuf_strstr_n(void)
{
	u16_t ret;
	struct pbuf *p = NULL;
	const char *substr = NULL;

	ret = pbuf_strstr(p, substr);

	TC_ASSERT_EQ("pbuf_strstr", ret, 0xFFFF);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_sys_untimeout_n
* @brief				:
* @scenario				:
* @apicovered			: sys_untimeout()
* @precondition			:
* @postcondition		:
*/
static void tc_net_sys_untimeout_n(void)
{
	sys_timeout_handler handler = NULL;
	void *arg = NULL;

	sys_untimeout(handler, arg);
	arg = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_dest_unreach_n
* @brief				:
* @scenario				:
* @apicovered			: icmp_dest_unreach()
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_dest_unreach_n(void)
{
	enum icmp_dur_type t;
	struct pbuf *p = NULL;
	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->payload = NULL;
	t = 0;

	icmp_dest_unreach(p, t);
	memp_free(MEMP_PBUF_POOL, p);
	p = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_time_exceeded_n
* @brief				:
* @scenario				:
* @apicovered			: icmp_time_exceeded()
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_time_exceeded_n(void)
{
	struct pbuf *p;
	enum icmp_dur_type t;
	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);

	p->payload = NULL;
	t = 0;

	icmp_time_exceeded(p, t);
	memp_free(MEMP_PBUF_POOL, p);
	p = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_dest_unreach_p
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_dest_unreach_p(void)
{
	struct pbuf *p = NULL;
	enum icmp_dur_type t;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->payload = "packet";
	t = 0;

	icmp_dest_unreach(p, t);
	memp_free(MEMP_PBUF_POOL, p);
	p = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_time_exceeded_p
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_time_exceeded_p(void)
{
	struct pbuf *p = NULL;
	enum icmp_dur_type t;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->payload = "packet";
	t = 0;

	icmp_time_exceeded(p, t);
	memp_free(MEMP_PBUF_POOL, p);
	p = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_send_response_p
* @brief				:
* @scenario				:
* @apicovered			: icmp_send_response()
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_send_response_p(void)
{
	struct pbuf *p;
	u8_t type;
	u8_t code;

	p->payload = "data";
	type = 3;
	code = 0;

	icmp_send_response(p, type, code);
	p = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_igmp_dump_group_list_n
* @brief				:
* @scenario				:
* @apicovered			: igmp_dump_group_list()
* @precondition			:
* @postcondition		:
*/
static void tc_net_igmp_dump_group_list_n(void)
{
	igmp_dump_group_list();
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_igmp_stop_n
* @brief				:
* @scenario				:
* @apicovered			: igmp_stop()
* @precondition			:
* @postcondition		:
*/
static void tc_net_igmp_stop_n(void)
{
	struct netif *netif;
	err_t ret;
	netif = NULL;

	ret = igmp_stop(netif);

	TC_ASSERT_EQ("tc_net_igmp_stop_n", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: default_netif_linkoutput
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
static err_t default_netif_linkoutput(struct netif *netif, struct pbuf *p)
{
	linkoutput_ctr++;
	return ERR_OK;
}

/**
* @testcase				: default_netif_init
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
static err_t default_netif_init(struct netif *netif)
{
	netif->linkoutput = default_netif_linkoutput;
	netif->output = etharp_output;
	netif->mtu = 1500;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
	netif->hwaddr_len = ETHARP_HWADDR_LEN;
	return 0;
}

/**
* @testcase				: tc_net_igmp_stop_p
* @brief				:
* @scenario				:
* @apicovered			: igmp_stop()
* @precondition			:
* @postcondition		:
*/
static void tc_net_igmp_stop_p(void)
{
	err_t ret;

	IP4_ADDR(&test_gw, 192, 168, 0, 1);
	IP4_ADDR(&test_ipaddr, 192, 168, 0, 1);
	IP4_ADDR(&test_netmask, 255, 255, 0, 0);
	netif_set_default(netif_add(&test_netif, &test_ipaddr, &test_netmask, &test_gw, NULL, default_netif_init, NULL));
	netif_set_up(&test_netif);
	ret = igmp_stop(&test_netif);

	TC_ASSERT_EQ("tc_net_igmp_stop_p", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_input_n
* @brief				:
* @scenario				:
* @apicovered			: icmp_input()
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_input_n(void)
{
	struct pbuf *p = NULL;
	struct netif *inp;
	struct ip_hdr *iphdr;
	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->payload = NULL;
	p->tot_len = 0;
	inp = NULL;
	iphdr = (struct ip_hdr *)p;
	IPH_VHL_SET(iphdr, 4, 0);

	icmp_input(p, inp);
	memp_free(MEMP_PBUF_POOL, p);
	p = NULL;
	inp = NULL;
	iphdr = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_icmp_input_p
* @brief				:
* @scenario				:
* @apicovered			: icmp_input()
* @precondition			:
* @postcondition		:
*/
static void tc_net_icmp_input_p(void)
{
	struct pbuf *p = NULL;
	struct netif *inp = NULL;
	struct ip_hdr *iphdr = NULL;
	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->payload = "data";
	p->tot_len = 4;

	IP4_ADDR(&test_gw, 192, 168, 0, 1);
	IP4_ADDR(&test_ipaddr, 192, 168, 0, 1);
	IP4_ADDR(&test_netmask, 255, 255, 0, 0);

	netif_set_default(netif_add(&test_netif, &test_ipaddr, &test_netmask, &test_gw, NULL, default_netif_init, NULL));
	netif_set_up(&test_netif);

	inp = &test_netif;
	iphdr = (struct ip_hdr *)p;
	IPH_VHL_SET(iphdr, 4, 2);
	icmp_input(p, inp);
	memp_free(MEMP_PBUF_POOL, p);
	p = NULL;
	inp = NULL;
	iphdr = NULL;
	TC_SUCCESS_RESULT();
}

static void *server(void *args)
{
	int i;
	int j = 0;
	int s_fd, sock_tcp;
	int fd[10] = { 0 };
	char buf[20] = { 0 };
	fd_set temp_fd;
	fd_set rfds;
	struct sockaddr_in serveraddr;
	struct sockaddr_in client;
	struct timeval t_Delay;
	int fd_max;
	int newfd;
	socklen_t addrlen;

	FD_ZERO(&temp_fd);
	FD_ZERO(&rfds);

	t_Delay.tv_sec = 1;
	t_Delay.tv_usec = 0;

	memset(&serveraddr, 0, sizeof(serveraddr));
	sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORTNUM);
	serveraddr.sin_addr.s_addr = IPADDR_LOOPBACK;

	bind(sock_tcp, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(sock_tcp, 2);

	fd_max = sock_tcp;

	while (1) {
		rfds = temp_fd;
		if ((s_fd = select(fd_max + 1, &rfds, NULL, NULL, &t_Delay)) == -1) {
			printf("error\n");
		}
		addrlen = sizeof(client);

		if (FD_ISSET(sock_tcp, &rfds)) {
			if ((newfd = accept(sock_tcp, (struct sockaddr *)&client, &addrlen)) == -1) {
				printf("error\n");
			}

			FD_SET(newfd, &temp_fd);
			rfds = temp_fd;
			fd[j++] = newfd;
			if (newfd > fd_max) {
				fd_max = newfd;
			}
		} else {
			for (i = 0; i < j; i++) {
				if (FD_ISSET(fd[i], &rfds)) {
					if (!(read(fd[i], buf, 20))) {
						FD_CLR(fd[i], &temp_fd);
						rfds = temp_fd;
						break;
					}
				}
			}
		}
	}
	return NULL;
}

static void *client(void *args)
{
	char buffer[MAXRCVLEN] = "HELLOWORLD\n";
	int len, sock_tcp;
	struct sockaddr_in dest;

	sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	connect(sock_tcp, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	len = write(sock_tcp, buffer, strlen(buffer));
	buffer[len] = '\0';
	return NULL;
}

/****************************************************************************
 * Name: netcore_api
 ****************************************************************************/
int net_core_main(int sock_tcp)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	tc_net_pbuf_strstr_p();
	tc_net_pbuf_get_at_p();
	tc_net_pbuf_memcmp_p();
	tc_net_get_socket_struct_n();
	tc_net_get_socket_struct_p();
	tc_net_tcp_zero_window_probe_n();
	tc_net_netif_remove_n();
	tc_net_pbuf_alloced_custom_n();
	tc_net_pbuf_alloced_custom_p();
	tc_net_pbuf_dechain_n();
	tc_net_pbuf_dechain_p();
	tc_net_pbuf_take_n();
	tc_net_pbuf_take_p();
	tc_net_pbuf_coalesce_n();
	tc_net_pbuf_coalesce_p();
	tc_net_pbuf_get_at_n();
	tc_net_pbuf_memcmp_n();
	tc_net_pbuf_memfind_n();
	tc_net_pbuf_memfind_p();
	tc_net_pbuf_strstr_n();
	tc_net_pbuf_strstr_p();
	tc_net_sys_untimeout_n();
	tc_net_icmp_dest_unreach_n();
	tc_net_icmp_dest_unreach_p();
	tc_net_icmp_time_exceeded_n();
	tc_net_icmp_time_exceeded_p();
	tc_net_igmp_dump_group_list_n();
	tc_net_igmp_stop_n();
	tc_net_igmp_stop_p();
	tc_net_icmp_input_n();
	tc_net_icmp_input_p();

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
	return 0;
}
