/*  @file tc_net_socket.c
 * @brief Test Case Example for pbuf() API
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>

#include <sys/socket.h>
#include <lwip/pbuf.h>

#include "tc_internal.h"

#define PBUF_TEST 6

/**
* @testcase             tc_net_pkt_layer_transport_p
* @brief		check for the memory allocation for the pbuf by transport layer
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_layer_transport_p(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_TRANSPORT, PBUF_TRANSPORT_HLEN, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             tc_net_pkt_layer_ip_p
* @brief		check for the memory allocation for the pbuf by ip layer
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_layer_ip_p(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_IP, PBUF_IP_HLEN, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             tc_net_pkt_layer_link_p
* @brief		check for the memory allocation for the pbuf by link layer
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_layer_link_p(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_LINK, PBUF_LINK_HLEN, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             tc_net_pkt_layer_raw_tx_p
* @brief		check for the memory allocation for the pbuf by encapsulating link layer headers (e.g. 802.11)
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_layer_raw_tx_p(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_RAW_TX, PBUF_LINK_ENCAPSULATION_HLEN, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             tc_net_pkt_layer_raw_p
* @brief		check for the memory allocation for the pbuf by RX buffers
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_layer_raw_p(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_RAW, 0, PBUF_RAM);
	TC_ASSERT_NEQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             tc_net_pkt_layer_n
* @brief		passed incorrect layer
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_layer_n(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_TEST, 0, PBUF_RAM);
	TC_ASSERT_EQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

/**
* @testcase             tc_net_pkt_type_n
* @brief		passed incorrect type. type parameter decides how and where the pbuf
*                       should be allocated.
* @scenario
* @apicovered           pbuf_alloc()
* @precondition
* @postcondition
*/
void tc_net_pkt_type_n(void) {

	struct pbuf *p = NULL;
	p = pbuf_alloc(PBUF_RAW, 0, PBUF_TEST);
	TC_ASSERT_EQ_CLEANUP("pbuf", p, NULL, pbuf_free(p));
	TC_SUCCESS_RESULT();
}

int net_pkt_main(void)
{
	tc_net_pkt_layer_transport_p();
	tc_net_pkt_layer_ip_p();
	tc_net_pkt_layer_link_p();
	tc_net_pkt_layer_raw_tx_p();
	tc_net_pkt_layer_raw_p();
	tc_net_pkt_layer_n();
	tc_net_pkt_type_n();

	return 0;
}
