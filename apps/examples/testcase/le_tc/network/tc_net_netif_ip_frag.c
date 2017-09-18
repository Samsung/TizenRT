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
#include <net/lwip/ipv4/ip_frag.h>

#include "tc_internal.h"

/**
* @testcase				: tc_ip_frag_n 
* @brief				:
* @scenario				:
* @apicovered			: ip_frag()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_frag_n(void)
{
	struct pbuf *p = NULL;
	struct netif *n = NULL;
	ip_addr_t *dest = NULL;
	int result = 0;

	result = ip_frag(p, n, dest);
	TC_ASSERT_NEQ("ip_frag", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_ip_reass_free_complete_datagram_p
* @brief				:
* @scenario				:
* @apicovered			: ip_reass_free_complete_datagram()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_reass_free_complete_datagram_p(void)
{
	struct ip_reassdata *ipr = malloc(sizeof(struct ip_reassdata));
	ipr->p = malloc(sizeof(struct pbuf));
	int result = 0;

	result = ip_reass_free_complete_datagram(ipr, NULL);
	/* ipr -> p is freed in ip_reass_free_complete_datagram */
	free(ipr);	
	ipr = NULL;
	TC_ASSERT_NEQ("ip_frag_free_pbuf_custom_ref", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_ip_reass_remove_oldest_datagram_p
* @brief				:
* @scenario				:
* @apicovered			: ip_reass_remove_oldest_datagram()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_reass_remove_oldest_datagram_p(void)
{
	struct ip_hdr *fraghdr = malloc(sizeof(struct ip_hdr));
	int result = 0;

	result = ip_reass_remove_oldest_datagram(fraghdr, 1);
	free(fraghdr);
	fraghdr = NULL;
	TC_ASSERT_NEQ("ip_reass_remove_oldest_datagram", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_ip_reass_remove_oldest_datagram_n
* @brief				:
* @scenario				:
* @apicovered			: ip_reass_remove_oldest_datagram()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_reass_remove_oldest_datagram_n(void)
{
	struct ip_hdr *fraghdr = NULL;
	int result = 0;

	result = ip_reass_remove_oldest_datagram(fraghdr, 1);
	TC_ASSERT_EQ("ip_reass_remove_oldest_datagram", result, 0);
	TC_SUCCESS_RESULT();
}
  
/**
* @testcase				: tc_ip_frag_free_pbuf_custom_ref_p
* @brief				:
* @scenario				:
* @apicovered			: ip_frag_free_pbuf_custom_ref()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_frag_free_pbuf_custom_ref_p(struct pbuf_custom_ref *p)
{
	int result = 1;

	ip_frag_free_pbuf_custom_ref(p);
	TC_ASSERT_NEQ("ip_frag_free_pbuf_custom_ref", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_ip_frag_alloc_pbuf_custom_ref_p
* @brief				:
* @scenario				:
* @apicovered			: ip_frag_alloc_pbuf_custom_ref()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_frag_alloc_pbuf_custom_ref_p(void)
{
	struct pbuf_custom_ref *buf = NULL;
	int result = 0;

	buf = (struct pbuf_custom_ref *)ip_frag_alloc_pbuf_custom_ref();
	if (buf) {
		result = 1;
		tc_ip_frag_free_pbuf_custom_ref_p(buf);
	}
	
	TC_ASSERT_NEQ("ip_frag_alloc_pbuf_custom_ref", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_ip_frag_free_pbuf_custom_n
* @brief				:
* @scenario				:
* @apicovered			: ipfrag_free_pbuf_custom()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_frag_free_pbuf_custom_n(void)
{
	ipfrag_free_pbuf_custom(NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_ip_frag_free_pbuf_custom_p
* @brief				:
* @scenario				:
* @apicovered			: ipfrag_free_pbuf_custom()
* @precondition			:
* @postcondition		:
*/
static void tc_ip_frag_free_pbuf_custom_p(void)
{
	struct pbuf *n = malloc(sizeof(struct pbuf));

	ipfrag_free_pbuf_custom(n);
	TC_SUCCESS_RESULT();
}
/****************************************************************************
 * Name: lwip_read_write()
 ****************************************************************************/
int tc_ip_frag_main(void)
{
	tc_ip_frag_n();
	tc_ip_frag_alloc_pbuf_custom_ref_p();
	tc_ip_frag_free_pbuf_custom_n();	
	tc_ip_frag_free_pbuf_custom_p();
	tc_ip_reass_free_complete_datagram_p();
	tc_ip_reass_remove_oldest_datagram_p();
	tc_ip_reass_remove_oldest_datagram_n();
	return 0;
}
