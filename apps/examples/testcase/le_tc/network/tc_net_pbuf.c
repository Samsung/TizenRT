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

/* @file tc_net_pbuf.c
 * @brief Test Case Example for pbuf() API
 */

#include <stdio.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/socket.h>
#include <lwip/pbuf.h>
#include <tinyara/os_api_test_drv.h>
#include <sys/ioctl.h>
#include "tc_internal.h"

#define PBUF_TEST 6

/**
 * @testcase		:tc_net_pbuf_layer_transport_p
 * @brief		:check for the memory allocation for the pbuf by transport layer
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_layer_transport_p(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_TRANSPORT;
	args.len = PBUF_TRANSPORT_HLEN;
	args.type = PBUF_RAM;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_GEQ("pbuf", ret, 1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase		:tc_net_pbuf_layer_ip_p
 * @brief		:check for the memory allocation for the pbuf by ip layer
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_layer_ip_p(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_IP;
	args.len = PBUF_IP_HLEN;
	args.type = PBUF_RAM;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_GEQ("pbuf", ret, 1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase		:tc_net_pbuf_layer_link_p
 * @brief		:check for the memory allocation for the pbuf by link layer
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_layer_link_p(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_LINK;
	args.len = PBUF_LINK_HLEN;
	args.type = PBUF_RAM;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_GEQ("pbuf", ret, 1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase		:tc_net_pbuf_layer_raw_tx_p
 * @brief		:check for the memory allocation for the pbuf by encapsulating link layer headers (e.g. 802.11)
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_layer_raw_tx_p(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_RAW_TX;
	args.len = PBUF_LINK_ENCAPSULATION_HLEN;
	args.type = PBUF_RAM;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_GEQ("pbuf", ret, 1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase		:tc_net_pbuf_layer_raw_p
 * @brief		:check for the memory allocation for the pbuf by RX buffers
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_layer_raw_p(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_RAW;
	args.len = 0;
	args.type = PBUF_RAM;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_GEQ("pbuf", ret, 1);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase		:tc_net_pbuf_layer_n
 * @brief		:passed incorrect layer
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_layer_n(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_TEST;
	args.len = 0;
	args.type = PBUF_RAM;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_EQ("pbuf", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase		:tc_net_pbuf_type_n
 * @brief		:passed incorrect type. type parameter decides how and where the pbuf should be allocated.
 * @scenario		:
 * @apicovered		:pbuf_alloc()
 * @precondition	:
 * @postcondition	:
 */
static void tc_net_pbuf_type_n(int fd)
{
	int ret = 0;
	struct pbuf_test_args args;

	args.layer = PBUF_RAW;
	args.len = 0;
	args.type = PBUF_TEST;

	ret = ioctl(fd, TESTIOC_NET_PBUF, (uint32_t)(&args));
	TC_ASSERT_EQ("pbuf", ret, 0);
	TC_SUCCESS_RESULT();
}

int net_pbuf_main(void)
{
	int fd;
	fd = tc_get_fd();

	tc_net_pbuf_layer_transport_p(fd);
	tc_net_pbuf_layer_ip_p(fd);
	tc_net_pbuf_layer_link_p(fd);
	tc_net_pbuf_layer_raw_tx_p(fd);
	tc_net_pbuf_layer_raw_p(fd);
	tc_net_pbuf_layer_n(fd);
	tc_net_pbuf_type_n(fd);

	return 0;
}
