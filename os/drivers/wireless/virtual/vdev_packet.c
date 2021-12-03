/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdint.h>
#include <sys/types.h>
#include <debug.h>
#include <time.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/lwnl/lwnl.h>
#include "vdev_handler.h"

#define VPACK_BEGIN
#define VPACK_END
#define VPACK_STRUCT __attribute__ ((__packed__))

VPACK_BEGIN
struct vwifi_ethhdr {
	uint8_t dest[6];
	uint8_t src[6];
	uint16_t type;
} VPACK_STRUCT;
VPACK_END

VPACK_BEGIN
struct vwifi_iphdr {
	uint8_t v_hl;
	uint8_t tos;
	uint16_t length;
	uint16_t id;
	uint16_t flag;
	uint8_t ttl;
	uint8_t proto;
	uint16_t checksum;
	uint8_t src[4];
	uint8_t dest[4];
} VPACK_STRUCT;
VPACK_END

VPACK_BEGIN
struct vwifi_udphdr {
	uint16_t src;
	uint16_t dest;
	uint16_t len;
	uint16_t checksum;
} VPACK_STRUCT;
VPACK_END

VPACK_BEGIN
struct vwifi_dhcpmsg {
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint8_t xid[4];
	uint16_t secs;
	uint16_t flags;
	uint8_t ciaddr[4];
	uint8_t yiaddr[4];
	uint8_t siaddr[4];
	uint8_t giaddr[4];
	uint8_t chaddr[16];
	uint8_t options[312];
} VPACK_STRUCT;
VPACK_END

static uint8_t g_eth_src[6] = {0,};
static uint8_t g_eth_dest[6] = {0,};

static uint8_t g_ip_src[4] = {0,};
static uint8_t g_ip_dest[4] = {0,};

static uint8_t g_tx_packet[1500] = {0,};
static uint8_t g_eth_broad[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static const uint8_t g_ip_null_addr[4] = {0x0, 0x0, 0x0, 0x0};
static const uint8_t g_ip_broadaddr[4] = {0xff, 0xff, 0xff, 0xff};
static const uint8_t g_dhcp_magic[4] = {0x63, 0x82, 0x53, 0x63};
static const uint8_t g_dhcp_lease_addr[4] = {0xc0, 0xa8, 0x01, 0x03};
static const uint8_t g_dhcp_server_addr[4] = {0xc0, 0xa8, 0x1, 0x1};
static const uint8_t g_dhcp_subnetmask[4] = {0xff, 0xff, 0xff, 0x0};
static const uint8_t g_dhcp_broadaddr[4] = {0xc0, 0xa8, 0x1, 0xff};
static uint8_t g_dhcp_discover[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0e, 0x04,
	0x96, 0x1d, 0xb3, 0xb0, 0x08, 0x00, 0x45, 0x00,
	0x01, 0x1c, 0x00, 0x00, 0x00, 0x00, 0xff, 0x11,
	0xba, 0xd1, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
	0xff, 0xff, 0x00, 0x44, 0x00, 0x43, 0x01, 0x08,
	0xcf, 0x04, 0x01, 0x01, 0x06, 0x00, 0x3c, 0x2f,
	0xb9, 0xd7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x04,
	0x96, 0x1d, 0xb3, 0xb0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82,
	0x53, 0x63, 0x35, 0x01, 0x01, 0x39, 0x02, 0x05,
	0xdc, 0x37, 0x04, 0x01, 0x03, 0x1c, 0x06, 0xff,
	0x00, 0x00};

extern void vwifi_send_packet(uint8_t *buf, uint32_t len);
// lwip function
extern uint16_t inet_chksum(const void *dataptr, uint16_t len);

static uint8_t *g_dhcp_buf = NULL;
static uint32_t g_dhcp_buf_len = 0;
static uint32_t g_dhcps_assigned_ip = 0;
/*  parsing a packet */
static int _vwifi_handle_eth(uint8_t *buf, uint32_t len, int *next_pos);
static int _vwifi_handle_ip(uint8_t *buf, uint32_t len, int *next_pos);
static int _vwifi_handle_udp(uint8_t *buf, uint32_t len, int *next_pos);
static int _vwifi_handle_dhcp(uint8_t *buf, uint32_t blen, int *msg_type);
/*  dhcp option handler */
static int _vwifi_handle_option_message_type(uint8_t *buf);
static void _vwifi_handle_option_parameter(uint8_t *buf);
static void _vwifi_handle_option_maxsize(uint8_t *buf);
/*  generate a packet */
static void _vwifi_create_stoc(uint8_t *buf, uint32_t len);
static void _vwifi_create_ctos(uint8_t *buf, uint32_t len);
static void _vwifi_send_dhcpoffer(uint8_t *buf, uint32_t len);
static void _vwifi_send_dhcpack(uint8_t *buf, uint32_t len);
static void _vwifi_send_dhcp_request(uint8_t *buf, uint32_t len);

int _vwifi_handle_eth(uint8_t *buf, uint32_t len, int *next_pos)
{
	(void)len;
	VWIFI_LOG("ETHERNET\n");

	struct vwifi_ethhdr *ethhdr = (struct vwifi_ethhdr *)buf;
	uint8_t *addr = ethhdr->dest;
	VWIFI_LOG("dest %02x:%02x:%02x:%02x:%02x:%02x\n",
			  addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	memcpy(g_eth_dest, addr, 6);

	addr = ethhdr->src;
	VWIFI_LOG("src %02x:%02x:%02x:%02x:%02x:%02x\n",
			  addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	memcpy(g_eth_src, addr, 6);

	VWIFI_LOG("type %x\n", ethhdr->type);
	*next_pos = sizeof(struct vwifi_ethhdr);

	return 0;
}

int _vwifi_handle_ip(uint8_t *buf, uint32_t len, int *next_pos)
{
	struct vwifi_iphdr *iphdr = (struct vwifi_iphdr *)buf;
	uint8_t version = iphdr->v_hl >> 4;
	uint8_t ip_hdrlen = (iphdr->v_hl & 0xf) * 4;
	VWIFI_LOG("IP\n");
	VWIFI_LOG("version %d %d\n", version, ip_hdrlen);
	if (version != 4) {
		return -1;
	}
	VWIFI_LOG("packet length %d\n", ntohs(iphdr->length));
	VWIFI_LOG("SRC: %d.%d.%d.%d\n",
			  iphdr->src[0], iphdr->src[1],
			  iphdr->src[2], iphdr->src[3]);
	memcpy(g_ip_src, iphdr->src, 4);

	VWIFI_LOG("DEST: %d.%d.%d.%d\n",
			  iphdr->dest[0], iphdr->dest[1],
			  iphdr->dest[2], iphdr->dest[3]);
	memcpy(g_ip_dest, iphdr->dest, 4);

	VWIFI_LOG("protocol %d\n", iphdr->proto);
	if (iphdr->proto != 17) {
		return -1;
	}
	*next_pos = ip_hdrlen;
	return 0;
}

int _vwifi_handle_udp(uint8_t *buf, uint32_t len, int *next_pos)
{
	struct vwifi_udphdr *udphdr = (struct vwifi_udphdr *)buf;
	VWIFI_LOG("UDP\n");
	VWIFI_LOG("src %d\n", ntohs(udphdr->src));
	VWIFI_LOG("dest %d\n", ntohs(udphdr->dest));
	if (ntohs(udphdr->dest) != 67 && ntohs(udphdr->dest) != 68) {
		VWIFI_LOG("next protocol is not dhcp\n");
		return -1;
	}
	*next_pos = sizeof(struct vwifi_udphdr);
	return 0;
}

int _vwifi_handle_option_message_type(uint8_t *buf)
{
	int type = buf[2];
	switch (type) {
	case 1:
		VWIFI_LOG("DHCP Discover\n");
		break;
	case 2:
		VWIFI_LOG("DHCP Offer\n");
		break;
	case 3:
		VWIFI_LOG("DHCP Request\n");
		break;
	case 5:
		VWIFI_LOG("DHCP Ack\n");
		break;
	default:
		VWIFI_LOG("Unknown %d\n", type);
		break;
	}
	return type;
}

void _vwifi_handle_option_parameter(uint8_t *buf)
{
	int len = buf[1];
	VWIFI_LOG("requested list\n");
	for (int i = 0; i < len; i++) {
		VWIFI_LOG("list item %d\n", buf[2 + i]);
	}
	VWIFI_LOG("\n");
}

void _vwifi_handle_option_maxsize(uint8_t *buf)
{
	uint16_t len = 0;
	memcpy(&len, &buf[2], 2);
	VWIFI_LOG("max size %u\n", ntohs(len));
}

int _vwifi_handle_dhcp(uint8_t *buf, uint32_t blen, int *msg_type)
{
	struct vwifi_dhcpmsg *dhcpmsg = (struct vwifi_dhcpmsg *)buf;
	VWIFI_LOG("\n\nDHCP\n");
	VWIFI_LOG("op %d\n", dhcpmsg->op);
	VWIFI_LOG("type %d\n", dhcpmsg->htype);
	VWIFI_LOG("length %d\n", dhcpmsg->hlen);
	VWIFI_LOG("ops %d\n", dhcpmsg->hops);
	uint32_t *addr = (uint32_t *)dhcpmsg->ciaddr;
	VWIFI_LOG("client addr %08x\n", *addr);
	addr = (uint32_t *)dhcpmsg->yiaddr;
	VWIFI_LOG("your addr %08x\n", *addr);
	g_dhcps_assigned_ip = *addr;
	addr = (uint32_t *)dhcpmsg->siaddr;
	VWIFI_LOG("next server addr %08x\n", *addr);
	addr = (uint32_t *)dhcpmsg->giaddr;
	VWIFI_LOG("relay ageint addr %08x\n", *addr);
	// boot file name 64
	// server host name 16 * 8 = 128
	VWIFI_LOG("option %d length %d type %d\n",
			  dhcpmsg->options[0],
			  dhcpmsg->options[1],
			  dhcpmsg->options[2]);
	VWIFI_LOG("dhcp magic %02x %02x %02x %02x\n",
			  dhcpmsg->options[192],
			  dhcpmsg->options[193],
			  dhcpmsg->options[194],
			  dhcpmsg->options[195]);

	int idx = 196;
	while (dhcpmsg->options[idx] != 0xff) {
		int option = dhcpmsg->options[idx];
		int len = dhcpmsg->options[idx + 1];
		VWIFI_LOG("option %d length %d\n", option, len);

		if (option == 53) {
			*msg_type = _vwifi_handle_option_message_type(&dhcpmsg->options[idx]);
		} else if (option == 55) {
			_vwifi_handle_option_parameter(&dhcpmsg->options[idx]);
		} else if (option == 57) {
			_vwifi_handle_option_maxsize(&dhcpmsg->options[idx]);
		}
		idx += (len + 2);
	}
	return 0;
}

// STA mode
void _vwifi_create_stoc(uint8_t *buf, uint32_t len)
{
	// udp header
	struct vwifi_udphdr *udp = (struct vwifi_udphdr *)(&g_tx_packet[34]);
	udp->src = htons(67);
	udp->dest = htons(68);
	udp->len = ntohs(sizeof(struct vwifi_dhcpmsg) + 8);
	// checksum is 0 to skip checksum

	// ethernet header
	struct vwifi_ethhdr *eth = (struct vwifi_ethhdr *)g_tx_packet;
	memcpy(eth->dest, g_eth_broad, 6);
	memcpy(eth->src, g_eth_src, 6);
	eth->src[5] = g_eth_src[5] + 1;
	eth->type = htons(0x0800);

	// ip header
	struct vwifi_iphdr *ip = (struct vwifi_iphdr *)(&g_tx_packet[14]);
	struct vwifi_iphdr *in_ip = (struct vwifi_iphdr *)(&buf[14]);
	ip->v_hl = in_ip->v_hl;
	ip->tos = in_ip->tos;
	ip->length = ntohs(sizeof(struct vwifi_dhcpmsg) + 8 + 20);
	ip->id = in_ip->id;
	ip->flag = in_ip->flag;
	ip->ttl = in_ip->ttl;
	ip->proto = in_ip->proto;
	memcpy(ip->src, g_dhcp_server_addr, 4);
	memcpy(ip->dest, g_ip_broadaddr, 4);
	ip->checksum = inet_chksum(ip, 20);
}

// softap mode
void _vwifi_create_ctos(uint8_t *buf, uint32_t len)
{
	// udp header
	struct vwifi_udphdr *udp = (struct vwifi_udphdr *)(&g_tx_packet[34]);
	udp->src = htons(68);
	udp->dest = htons(67);
	udp->len = ntohs(sizeof(struct vwifi_dhcpmsg) + 8);
	// checksum is 0 to skip checksum

	// ethernet header
	struct vwifi_ethhdr *eth = (struct vwifi_ethhdr *)g_tx_packet;
	memcpy(eth->dest, g_eth_src, 6);
	//memcpy(eth->src, g_eth_dest, 6);
	eth->src[5] = g_eth_src[5] + 1;
	eth->type = htons(0x0800);

	// ip header
	struct vwifi_iphdr *ip = (struct vwifi_iphdr *)(&g_tx_packet[14]);
	struct vwifi_iphdr *in_ip = (struct vwifi_iphdr *)(&buf[14]);
	ip->v_hl = in_ip->v_hl;
	ip->tos = in_ip->tos;
	ip->length = ntohs(sizeof(struct vwifi_dhcpmsg) + 8 + 20);
	ip->id = in_ip->id;
	ip->flag = in_ip->flag;
	ip->ttl = in_ip->ttl;
	ip->proto = in_ip->proto;
	uint8_t  client_ip[] = {0x02, 0x2f, 0xa8, 0xc0};
	memcpy(ip->src, client_ip, 4);
	memcpy(ip->dest, g_ip_broadaddr, 4);
	ip->checksum = inet_chksum(ip, 20);
}

void _vwifi_send_dhcpoffer(uint8_t *buf, uint32_t len)
{
	memset(g_tx_packet, 0, 1500);
	// dhcp offer create
	// offset eth(14) + ip(20) + udp(8)
	struct vwifi_dhcpmsg *dhcpmsg = (struct vwifi_dhcpmsg *)(&g_tx_packet[42]);
	struct vwifi_dhcpmsg *in_dhcpmsg = (struct vwifi_dhcpmsg *)(&buf[42]);
	dhcpmsg->op = 2;
	dhcpmsg->htype = 0x01;
	dhcpmsg->hlen = 6;
	dhcpmsg->hops = 0;
	memcpy(dhcpmsg->xid, in_dhcpmsg->xid, 4);
	dhcpmsg->secs = 0;
	dhcpmsg->flags = htons(0x8000);
	memcpy(dhcpmsg->ciaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->yiaddr, g_dhcp_lease_addr, 4);
	memcpy(dhcpmsg->siaddr, g_dhcp_server_addr, 4);
	memcpy(dhcpmsg->giaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->chaddr, g_eth_src, 6);

	memcpy(&dhcpmsg->options[192], g_dhcp_magic, 4);

	dhcpmsg->options[196] = 53;
	dhcpmsg->options[197] = 1;
	dhcpmsg->options[198] = 2;

	dhcpmsg->options[199] = 54;
	dhcpmsg->options[200] = 4;
	memcpy(&dhcpmsg->options[201], g_dhcp_server_addr, 4);

	dhcpmsg->options[205] = 51;
	dhcpmsg->options[206] = 4;
	dhcpmsg->options[210] = 0xff; // 255 sec

	dhcpmsg->options[211] = 1; // subnetmask
	dhcpmsg->options[212] = 4;
	memcpy(&dhcpmsg->options[213], g_dhcp_subnetmask, 4);

	dhcpmsg->options[217] = 3; // router
	dhcpmsg->options[218] = 4;
	memcpy(&dhcpmsg->options[219], g_dhcp_server_addr, 4);

	dhcpmsg->options[223] = 6; // domain name server;
	dhcpmsg->options[224] = 4;
	memcpy(&dhcpmsg->options[225], g_dhcp_server_addr, 4);

	dhcpmsg->options[229] = 0xff;

	_vwifi_create_stoc(buf, len);

	vwifi_send_packet(g_tx_packet, 14 + 20 + 8 + sizeof(struct vwifi_dhcpmsg));
}

void _vwifi_send_dhcpack(uint8_t *buf, uint32_t len)
{
	memset(g_tx_packet, 0, 1500);
	// dhcp ACK create
	// offset eth(14) + ip(20) + udp(8)
	struct vwifi_dhcpmsg *dhcpmsg = (struct vwifi_dhcpmsg *)(&g_tx_packet[42]);
	struct vwifi_dhcpmsg *in_dhcpmsg = (struct vwifi_dhcpmsg *)(&buf[42]);
	dhcpmsg->op = 2;
	dhcpmsg->htype = 0x01;
	dhcpmsg->hlen = 6;
	dhcpmsg->hops = 0;
	memcpy(dhcpmsg->xid, in_dhcpmsg->xid, 4);
	dhcpmsg->secs = 0;
	dhcpmsg->flags = 0x8000;
	memcpy(dhcpmsg->ciaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->yiaddr, g_dhcp_lease_addr, 4);
	memcpy(dhcpmsg->siaddr, g_dhcp_server_addr, 4);
	memcpy(dhcpmsg->giaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->chaddr, g_eth_src, 6);

	memcpy(&dhcpmsg->options[192], g_dhcp_magic, 4);

	dhcpmsg->options[196] = 53;
	dhcpmsg->options[197] = 1;
	dhcpmsg->options[198] = 5;

	dhcpmsg->options[199] = 54;
	dhcpmsg->options[200] = 4;
	memcpy(&dhcpmsg->options[201], g_dhcp_server_addr, 4);

	uint32_t dhcp_time = 86400; // seconds
	dhcpmsg->options[205] = 51; // lease time
	dhcpmsg->options[206] = 4;
	memcpy(&dhcpmsg->options[207], &dhcp_time, 4);

	dhcp_time = 43200; // seconds 12 hour
	dhcpmsg->options[211] = 58; // rebinding
	dhcpmsg->options[212] = 4;
	memcpy(&dhcpmsg->options[213], &dhcp_time, 4);

	dhcp_time = 75600;
	dhcpmsg->options[217] = 59;
	dhcpmsg->options[218] = 4;
	memcpy(&dhcpmsg->options[219], &dhcp_time, 4);

	dhcpmsg->options[223] = 1;
	dhcpmsg->options[224] = 4;
	memcpy(&dhcpmsg->options[225], &g_dhcp_subnetmask, 4);

	dhcpmsg->options[229] = 28;
	dhcpmsg->options[230] = 4;
	memcpy(&dhcpmsg->options[231], &g_dhcp_broadaddr, 4);

	dhcpmsg->options[235] = 6;
	dhcpmsg->options[236] = 4;
	memcpy(&dhcpmsg->options[237], &g_dhcp_server_addr, 4);

	dhcpmsg->options[241] = 3;
	dhcpmsg->options[242] = 4;
	memcpy(&dhcpmsg->options[243], &g_dhcp_server_addr, 4);

	dhcpmsg->options[247] = 0xff;

	_vwifi_create_stoc(buf, len);
	vwifi_send_packet(g_tx_packet, 14 + 20 + 8 + sizeof(struct vwifi_dhcpmsg));
}

void _vwifi_send_dhcp_request(uint8_t *buf, uint32_t len)
{
	memset(g_tx_packet, 0, 1500);
	// dhcp offer create
	// offset eth(14) + ip(20) + udp(8)
	struct vwifi_dhcpmsg *dhcpmsg = (struct vwifi_dhcpmsg *)(&g_tx_packet[42]);
	struct vwifi_dhcpmsg *in_dhcpmsg = (struct vwifi_dhcpmsg *)(&buf[42]);
	dhcpmsg->op = 1; // boot request
	dhcpmsg->htype = 0x01;
	dhcpmsg->hlen = 6;
	dhcpmsg->hops = 0;
	memcpy(dhcpmsg->xid, in_dhcpmsg->xid, 4);
	dhcpmsg->secs = 0;
	dhcpmsg->flags = htons(0x8000); // braodcast
	memcpy(dhcpmsg->ciaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->yiaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->siaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->giaddr, g_ip_null_addr, 4);
	memcpy(dhcpmsg->chaddr, g_eth_src, 6);

	memcpy(&dhcpmsg->options[192], g_dhcp_magic, 4);

	dhcpmsg->options[196] = 53;
	dhcpmsg->options[197] = 1;
	dhcpmsg->options[198] = 3; //dhcp message type request

	// option max size 1500B
	dhcpmsg->options[199] = 57;
	dhcpmsg->options[200] = 2;
	dhcpmsg->options[201] = 0x05;
	dhcpmsg->options[202] = 0xdc;

	// option requested ip address
	dhcpmsg->options[203] = 50;
	dhcpmsg->options[204] = 4;
	memcpy(&dhcpmsg->options[205], &g_dhcps_assigned_ip, 4);

	// option 55 parameter request list
	uint8_t param_list[] = {1, 3, 28, 6};
	dhcpmsg->options[209] = 55;
	dhcpmsg->options[210] = 4;
	memcpy(&dhcpmsg->options[211], &param_list, 4);

	dhcpmsg->options[215] = 0xff;

	_vwifi_create_ctos(buf, len);
	vwifi_send_packet(g_tx_packet, 14 + 20 + 8 + sizeof(struct vwifi_dhcpmsg));
}

void vwifi_handle_packet(uint8_t *buf, uint32_t len)
{
	VWIFI_LOG("%p, len(%d)\n", buf, len);
	// print ethernet header
	int next = 0, sum = 0;
	g_dhcp_buf = buf;
	g_dhcp_buf_len = len;
	int res = _vwifi_handle_eth(buf, 0, &next);
	if (res < 0) {
		return;
	}
	sum += next;

	res = _vwifi_handle_ip(buf + sum, 0, &next);
	if (res < 0) {
		return;
	}
	sum += next;

	res = _vwifi_handle_udp(buf + sum, 0, &next);
	if (res < 0) {
		return;
	}
	sum += next;

	res = _vwifi_handle_dhcp(buf + sum, 0, &next);
	if (next == 1) { // receive dhcp discover
		VWIFI_LOG("send offer message\n");
		_vwifi_send_dhcpoffer(buf, len);
	} else if (next == 3) { // receive dhcp request
		VWIFI_LOG("send ACK message\n");
		_vwifi_send_dhcpack(buf, len);
	} else if (next == 2) { // receive dhcp offer
		VWIFI_LOG("send REQUEST message\n");
		_vwifi_send_dhcp_request(buf, len);
	}
}

void vwifi_generate_dhcp_discover(void)
{
	VWIFI_LOG("dhcp discover len (%d)\n", sizeof(g_dhcp_discover));
	vwifi_send_packet(g_dhcp_discover, sizeof(g_dhcp_discover));
}
