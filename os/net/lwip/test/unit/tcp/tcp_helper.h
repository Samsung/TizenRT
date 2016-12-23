/****************************************************************************
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

#ifndef __TCP_HELPER_H__
#define __TCP_HELPER_H__

#include "../lwip_check.h"
#include <net/lwip/arch.h>
#include <net/lwip/tcp.h>
#include <net/lwip/netif.h>

/* counters used for test_tcp_counters_* callback functions */
struct test_tcp_counters {
	u32_t recv_calls;
	u32_t recved_bytes;
	u32_t recv_calls_after_close;
	u32_t recved_bytes_after_close;
	u32_t close_calls;
	u32_t err_calls;
	err_t last_err;
	char *expected_data;
	u32_t expected_data_len;
};

struct test_tcp_txcounters {
	u32_t num_tx_calls;
	u32_t num_tx_bytes;
	u8_t copy_tx_packets;
	struct pbuf *tx_packets;
};

/* Helper functions */
void tcp_remove_all(void);

struct pbuf *tcp_create_segment(ip_addr_t *src_ip, ip_addr_t *dst_ip, u16_t src_port, u16_t dst_port, void *data, size_t data_len, u32_t seqno, u32_t ackno, u8_t headerflags);
struct pbuf *tcp_create_rx_segment(struct tcp_pcb *pcb, void *data, size_t data_len, u32_t seqno_offset, u32_t ackno_offset, u8_t headerflags);
struct pbuf *tcp_create_rx_segment_wnd(struct tcp_pcb *pcb, void *data, size_t data_len, u32_t seqno_offset, u32_t ackno_offset, u8_t headerflags, u16_t wnd);
void tcp_set_state(struct tcp_pcb *pcb, enum tcp_state state, ip_addr_t *local_ip, ip_addr_t *remote_ip, u16_t local_port, u16_t remote_port);
void test_tcp_counters_err(void *arg, err_t err);
err_t test_tcp_counters_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

struct tcp_pcb *test_tcp_new_counters_pcb(struct test_tcp_counters *counters);

void test_tcp_input(struct pbuf *p, struct netif *inp);

void test_tcp_init_netif(struct netif *netif, struct test_tcp_txcounters *txcounters, ip_addr_t *ip_addr, ip_addr_t *netmask);

#endif
