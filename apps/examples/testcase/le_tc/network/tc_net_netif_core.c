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

#define TWO             2
#define CNT             2
#define VAL             6
#define MTUSIZE         1500
#define TOTLEN          16
#define PBUF_SIZE       10

struct netif loop_netif;
struct netif test_netif;
static int linkoutput_ctr;
extern err_t netif_loopif_init(struct netif *netif);

/**
* @testcase             : tc_net_pbuf_get_at_p
* @brief                : get one byte from specified
* @scenario             : take the pbuf and offset
* @apicovered           : pbuf_get_at()
* @precondition         : none
* @postcondition        : none
* @return               : void
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
	TC_ASSERT_NEQ_CLEANUP("pbuf_get_at", ret, ZERO, memp_free(MEMP_PBUF_POOL, p));
	memp_free(MEMP_PBUF_POOL, p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : create_packet
* @brief                : helper function
* @scenario             : Initealize the default packet
* @apicovered           : none
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
void create_packet(struct pbuf **head, char *data, u16_t len, u16_t ref)
{
	struct pbuf *node;
	struct pbuf *q;

	node = (struct pbuf *)mem_malloc(sizeof(struct pbuf));
	TC_ASSERT_NEQ("mem_malloc", node, NULL);
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
* @testcase             : tc_net_pbuf_memcmp_p
* @brief                : Compare pbuf contents at specified offset with memory
* @scenario             : positive scenario for pbuf_memcmp
* @apicovered           : create_packet pbuf_memcmp
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_memcmp_p(void)
{
	u16_t ret;
	struct pbuf *head = NULL;
	struct pbuf *temp = NULL;
	int pk_count;
	u16_t offset = VAL;
	const void *s2 = "is";
	u16_t n = TWO;

	for (pk_count = 0; pk_count < CNT; pk_count++) {
		switch (pk_count) {
		case 0:
			create_packet(&head, "packet", strlen("packet"), ONE);
			break;
		case 1:
			create_packet(&head, "is", (strlen("is") + strlen("packet")), TWO);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_memcmp(head, offset, s2, n);
	while (head) {
		temp = head;
		head = head->next;
		mem_free(temp);
	}
	TC_ASSERT_EQ("pbuf_memcmp", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_memfind_p
* @brief                : Find occurrence of mem in pbuf p, starting at offset
* @scenario             : positive scenario for pbuf_memfind
* @apicovered           : pbuf_memfind
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_memfind_p(void)
{
	int ret;
	struct pbuf *p = (struct pbuf *)"packet not found";
	const void *mem = "packet";
	u16_t mem_len = 1;
	u16_t start_offset = 0;
	p->tot_len = 16;
	p->len = 16;

	ret = pbuf_memfind(p, mem, mem_len, start_offset);

	TC_ASSERT_NEQ("pbuf_memfind", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase             : tc_net_get_socket_struct_n
* @brief                : finding the socket through sockfd.
* @scenario             : If there is no wrong sockfd.
* @apicovered           : get_socket_struct()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_get_socket_struct_n(void)
{
	struct socket *ret = (struct socket *)get_socket_struct(NEG_VAL);

	TC_ASSERT_EQ("net_get_socket_struct_n", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_get_socket_struct_p
* @brief                : Finding the socket through sockfd
* @scenario             : sending fd and geting structure
* @apicovered           : get_socket_struct()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_get_socket_struct_p(void)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	struct socket *ret = (struct socket *)get_socket_struct(s);

	TC_ASSERT_NEQ("net_get_socket_struct_p", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_alloced_custom_n
* @brief                : Initialize a custom pbuf
* @scenario             : none
* @apicovered           : pbuf_alloced_custom()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_alloced_custom_n(void)
{
	struct pbuf *ret;
	pbuf_layer l = 4;
	u16_t length = TWO;
	pbuf_type type = 4;
	struct pbuf_custom *p = NULL;
	void *payload_mem = NULL;
	u16_t payload_mem_len = ONE;

	ret = pbuf_alloced_custom(l, length, type, p, payload_mem, payload_mem_len);

	TC_ASSERT_EQ("net_pbuf_alloced_custom", ret, NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase             : tc_net_pbuf_alloced_custom_p
* @brief                : Initialize a custom pbuf
* @scenario             : none
* @apicovered           : pbuf_alloced_custom()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_alloced_custom_p(void)
{
	struct pbuf *ret;
	struct pbuf_custom *p;
	u16_t length = 5;
	pbuf_type type = 0;
	void *payload_mem = "payload big";
	u16_t payload_mem_len = 11;

	p = (struct pbuf_custom *)mem_malloc(sizeof(struct pbuf_custom));
	TC_ASSERT_NEQ("mem_malloc", p, NULL);
	ret = pbuf_alloced_custom(0, length, type, p, payload_mem, payload_mem_len);
	mem_free(p);

	TC_ASSERT_EQ("net_pbuf_alloced_custom", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_dechain_n
* @brief                : Dechains the first pbuf from its succeeding pbufs in the chain
* @scenario             : detching the pbuf
* @apicovered           : memp_malloc, pbuf_dechain
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_dechain_n(void)
{
	struct pbuf *ret;
	struct pbuf *p;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	p->next = NULL;

	ret = pbuf_dechain(p);
	memp_free(MEMP_PBUF_POOL, p);
	TC_ASSERT_EQ("net_pbuf_dechain_n", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : create_chain
* @brief                : helper function
* @scenario             : creating chain of pbuf
* @apicovered           : none
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
void create_chain(struct pbuf **head, char *data, u16_t len, u16_t ref)
{
	struct pbuf *node;
	struct pbuf *q;

	node = (struct pbuf *)mem_malloc(sizeof(struct pbuf));
	TC_ASSERT_NEQ("mem_malloc", node, NULL);

	node->payload = data;
	node->next = NULL;
	node->flags = 0;
	node->len = len;
	node->tot_len = TOTLEN;
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
* @testcase             : tc_net_pbuf_dechain_p
* @brief                : Dechains the first pbuf from its succeeding pbufs in the chain
* @scenario             : none
* @apicovered           : create_chain, pbuf_dechain()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_dechain_p(void)
{
	struct pbuf *head = NULL;
	struct pbuf *temp = NULL;
	int pk_count;
	struct pbuf *ret;

	for (pk_count = 0; pk_count < 2; pk_count++) {
		switch (pk_count) {
		case 0:
			create_chain(&head, "packet", strlen("packet"), ONE);
			break;
		case 1:
			create_chain(&head, "is", strlen("is"), TWO);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_dechain(head);
	while (head) {
		temp = head;
		head = head->next;
		mem_free(temp);
	}

	TC_ASSERT_NEQ("net_pbuf_dechain", ret, NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase             : tc_net_pbuf_take_n
* @brief                : Create PBUF_RAM copies of pbufs
* @scenario             : none
* @apicovered           : pbuf_take()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_take_n(void)
{
	struct pbuf *buf = NULL;
	const void *dataptr = NULL;
	err_t ret;
	u16_t len = 1;

	ret = pbuf_take(buf, dataptr, len);

	TC_ASSERT_EQ("net_pbuf_take", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_take_p
* @brief                : Create PBUF_RAM copies of pbufs
* @scenario             : none
* @apicovered           : pbuf_take()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_take_p(void)
{
	err_t ret;
	struct pbuf *buf = NULL;
	void *dataptr = "data";

	buf = (struct pbuf *)memp_malloc(PBUF_SIZE);
	buf->next = NULL;
	buf->len = 8;
	buf->tot_len = 10;

	ret = pbuf_take(buf, dataptr, strlen("data"));
	memp_free(MEMP_PBUF_POOL, buf);
	TC_ASSERT_EQ("net_pbuf_take", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_coalesce_n
* @brief                : Creates a single pbuf out of a queue of pbufs
* @scenario             : none
* @apicovered           : pbuf_coalesce()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_coalesce_n(void)
{
	struct pbuf *ret = NULL;
	struct pbuf *p = NULL;
	pbuf_layer layer;

	p = (struct pbuf *)memp_malloc(MEMP_PBUF_POOL);
	TC_ASSERT_NEQ("memp_malloc", p, NULL);
	p->next = NULL;
	layer = 4;

	ret = pbuf_coalesce(p, layer);
	memp_free(MEMP_PBUF_POOL, p);
	TC_ASSERT_EQ("net_pbuf_coalesce", ret, p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_coalesce_p
* @brief                : Creates a single pbuf out of a queue of pbufs
* @scenario             : none
* @apicovered           : pbuf_coalesce()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_coalesce_p(void)
{
	int pk_count;
	struct pbuf *ret = NULL;
	pbuf_layer layer;
	struct pbuf *head = NULL;
	struct pbuf *temp = NULL;

	layer = 3;

	for (pk_count = 0; pk_count < CNT; pk_count++) {
		switch (pk_count) {
		case 0:
			create_chain(&head, "packet", strlen("packet"), ONE);
			break;
		case 1:
			create_chain(&head, "is", strlen("is"), TWO);
			break;
		default:
			printf("\n invalid parameter\n");
		}
	}

	ret = pbuf_coalesce(head, layer);
	while (head) {
		temp = head;
		head = head->next;
		mem_free(temp);
	}

	TC_ASSERT_NEQ_CLEANUP("net_pbuf_coalesce", ret, head, pbuf_free(head));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_get_at_n
* @brief                : Get one byte from the specified position in a pbuf
* @scenario             : none
* @apicovered           : pbuf_get_at()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_get_at_n(void)
{
	struct pbuf *p = NULL;
	u16_t offset = 0;
	u8_t ret;

	ret = pbuf_get_at(p, offset);
	TC_ASSERT_EQ("net_pbuf_get_at", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_memcmp_n
* @brief                : Get one byte from the specified position in a pbuf
* @scenario             : negative scenario for pbuf_memcmp
* @apicovered           : pbuf_memcmp()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_memcmp_n(void)
{
	u16_t ret;
	struct pbuf *p = NULL;
	u16_t offset = ZERO;
	const void *s2 = NULL;
	u16_t n = ZERO;

	ret = pbuf_memcmp(p, offset, s2, n);

	TC_ASSERT_NEQ("net_pbuf_memcmp", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_pbuf_memfind_n
* @brief                : Find occurrence of mem in pbuf p, starting at offset
* @scenario             : none
* @apicovered           : pbuf_memfind()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
static void tc_net_pbuf_memfind_n(void)
{
	u16_t ret;
	struct pbuf *p = NULL;
	const void *mem = NULL;
	u16_t mem_len = ONE;
	u16_t start_offset = ONE;

	ret = pbuf_memfind(p, mem, mem_len, start_offset);

	TC_ASSERT_EQ("net_pbuf_memfind", ret, 0xffff);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : default_netif_linkoutput
* @brief                : none
* @scenario             : none
* @apicovered           : default_netif_linkoutput
* @precondition         : network interface must be created
* @postcondition        : none
* @return               : void
*/
static err_t default_netif_linkoutput(struct netif *netif, struct pbuf *p)
{
	linkoutput_ctr++;
	return ERR_OK;
}

/****************************************************************************
 * Name: netcore_api
 ****************************************************************************/
int net_core_main(int sock_tcp)
{
	tc_net_get_socket_struct_n();
	tc_net_get_socket_struct_p();
	tc_net_pbuf_get_at_p();
	tc_net_pbuf_get_at_n();
	tc_net_pbuf_memcmp_p();
	tc_net_pbuf_memcmp_n();
	tc_net_pbuf_alloced_custom_n();
	tc_net_pbuf_alloced_custom_p();
	tc_net_pbuf_dechain_n();
	tc_net_pbuf_dechain_p();
	tc_net_pbuf_take_n();
	tc_net_pbuf_take_p();
	tc_net_pbuf_coalesce_n();
	tc_net_pbuf_coalesce_p();
	tc_net_pbuf_memfind_n();
	tc_net_pbuf_memfind_p();
	return 0;
}
