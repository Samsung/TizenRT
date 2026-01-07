/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include <netdb.h>
#include <pthread.h>
#include "PingGenerator.h"
#include <lwip/prot/icmp.h>
#include "netutils/netlib.h"

#define ERR_OK OK
#define PING_TH_NAME "csifw_ping_th"

void *pingThreadFun(void *vargp);
static int setIcmp4Config(struct addrinfo *hints);
static CSIFW_RES ping_generator_close_socket(int *ping_socket, struct addrinfo **addr_info_list);
static CSIFW_RES ping_generator_open_socket(int *ping_socket, struct sockaddr **socketAddr, struct addrinfo **addr_info_list);

void ping_generator_change_interval(unsigned int pingInterval)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return ;
	}	
	CSIFW_LOGD("Changing ping interval from %u ms to %u ms", p_csifw_ctx->ping_Interval/1000, pingInterval);
	p_csifw_ctx->ping_Interval = pingInterval * 1000;	// converting interval in microsecond
	CSIFW_LOGI("New ping interval set to %u microseconds (%.2f ms)", 
		p_csifw_ctx->ping_Interval, (float)pingInterval);
}

static u16_t standard_chksum(void *dataptr, u16_t len)
{
	if (!dataptr) {
		CSIFW_LOGE("Invalid data pointer for checksum");
		return 0;
	}
	if (len == 0) {
		CSIFW_LOGE("Invalid length 0 for checksum");
		return 0;
	}

	u32_t acc = 0;
	u16_t src;
	u8_t *octetptr = (u8_t *) dataptr;
	/* dataptr may be at odd or even addresses */
	while (len > 1) {
		/* declare first octet as most significant
		   thus assume network order, ignoring host order */
		src = (*octetptr) << 8;
		octetptr++;
		/* declare second octet as least significant */
		src |= (*octetptr);
		octetptr++;
		acc += src;
		len -= 2;
	}

	if (len > 0) {
		/* accumulate remaining octet */
		src = (*octetptr) << 8;
		acc += src;
	}
	/* add deferred carry bits */
	acc = (acc >> 16) + (acc & 0x0000ffffUL);
	if ((acc & 0xffff0000UL) != 0) {
		acc = (acc >> 16) + (acc & 0x0000ffffUL);
	}
	/* This maybe a little confusing: reorder sum using htons()
	   instead of ntohs() since it has a little less call overhead.
	   The caller must invert bits for Internet sum ! */
	u16_t checksum = htons((u16_t)acc);
	CSIFW_LOGD("Calculated checksum: 0x%04X", checksum);
	return checksum;
}

static void ping_prepare_echo(struct icmp_echo_hdr *p_iecho, u16_t len)
{
	if (!p_iecho) {
		CSIFW_LOGE("Invalid ICMP echo header pointer");
		return;
	}
	if (len < sizeof(struct icmp_echo_hdr)) {
		CSIFW_LOGE("Invalid length %u (min %zu required)", 
			len, sizeof(struct icmp_echo_hdr));
		return;
	}

	size_t i = 0;
	int icmp_hdrlen = sizeof(struct icmp_echo_hdr);
	ICMPH_CODE_SET(p_iecho, 0);
	p_iecho->id = 0xAFAF;		//PING_ID;
	p_iecho->seqno = htons(1);

	CSIFW_LOGD("Preparing ICMP echo packet (id=0x%X, seq=%d, len=%u)", ntohs(p_iecho->id), ntohs(p_iecho->seqno), len);
	for (i = icmp_hdrlen; i < len; i++) {
		((char *)p_iecho)[i] = (char)i;
	}

	ICMPH_TYPE_SET(p_iecho, ICMP_ECHO);
	p_iecho->chksum = 0;
	p_iecho->chksum = ~standard_chksum(p_iecho, len);
}

static CSIFW_RES ping_send(int ping_socket, struct icmp_echo_hdr *p_iecho, struct sockaddr *socketAddr)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	p_csifw_ctx->ping_count++;
	if (!(p_csifw_ctx->ping_count % 1000)) {
		CSIFW_LOGD("Ping #%d - Sending ICMP echo (socket=%d, seq=%d, id=%d)",
			p_csifw_ctx->ping_count, ping_socket, 
			ntohs(p_iecho->seqno), ntohs(p_iecho->id));
	}

	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	size_t icmplen = sizeof(struct icmp_echo_hdr) + 8;

	ret = sendto(ping_socket, p_iecho, icmplen, 0, socketAddr, addrlen);
	if (ret <= 0) {
		CSIFW_LOGE("sendto() failed - ret=%d, errno=%d (%s)", ret, errno, strerror(errno));
		return CSIFW_ERROR;
	}
	return CSIFW_OK;
}

void *pingThreadFun(void *vargp)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return NULL;
	}

	int ping_socket = p_csifw_ctx->ping_socket;
	struct icmp_echo_hdr *p_iecho = p_csifw_ctx->p_iecho;
	struct sockaddr *socketAddr = p_csifw_ctx->socketAddr;
	while (!p_csifw_ctx->ping_thread_stop) {
		usleep(p_csifw_ctx->ping_Interval);
		if (ping_send(ping_socket, p_iecho, socketAddr) != CSIFW_OK) {
			CSIFW_LOGE("Ping send failed :%d", CSIFW_ERROR);
		}
	}
	CSIFW_LOGD("[THREAD] : EXIT");
	return NULL;
}

CSIFW_RES ping_generator_start(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	if (p_csifw_ctx->ping_Interval == 0) {
		CSIFW_LOGD("Ping interval is %d ==> NO PING START", p_csifw_ctx->ping_Interval);
		return CSIFW_OK;
	}
	p_csifw_ctx->ping_count = 0;
	p_csifw_ctx->csi_ping_thread = 0;
	p_csifw_ctx->ping_thread_stop = false;
	if (pthread_create(&p_csifw_ctx->csi_ping_thread, NULL, pingThreadFun, NULL) != 0) {
		CSIFW_LOGE("Failed to create Ping thread - errno=%d (%s)", errno, strerror(errno));
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(p_csifw_ctx->csi_ping_thread, PING_TH_NAME) != 0) {
		CSIFW_LOGE("Failed to set thread name '%s' - errno=%d (%s)", PING_TH_NAME, errno, strerror(errno));
	}
	CSIFW_LOGD("[PING Thread] created and initialized successfully");
	return CSIFW_OK;
}

CSIFW_RES csi_ping_generator_initialize(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	CSIFW_LOGD("Starting ping thread (ID: %lu)", pthread_self());
	int ping_socket = -1;
	struct addrinfo *addr_info_list = NULL;
	struct sockaddr *socketAddr = NULL;

	CSIFW_LOGD("Opening ping socket");
	if (ping_generator_open_socket(&ping_socket, &socketAddr, &addr_info_list) != CSIFW_OK) {
		CSIFW_LOGE("Failed to open ping socket :%d", CSIFW_ERROR);
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("Ping socket opened successfully (fd=%d)", ping_socket);

	size_t icmplen = sizeof(struct icmp_echo_hdr) + 8;
	struct icmp_echo_hdr *p_iecho = NULL;

	CSIFW_LOGD("Allocating ICMP echo buffer (%zu bytes) for socket %d", icmplen, ping_socket);
	p_iecho = (struct icmp_echo_hdr *)malloc(icmplen);
	if (!p_iecho) {
		CSIFW_LOGE("Failed to allocate %zu bytes for ICMP echo (errno=%d: %s)", 
			icmplen, errno, strerror(errno));
		ping_generator_close_socket(&ping_socket, &addr_info_list);
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("ICMP echo buffer allocated successfully at %p (%zu bytes)", p_iecho, icmplen);
	ping_prepare_echo(p_iecho, (u16_t) icmplen);

	p_csifw_ctx->addr_info = addr_info_list;
	p_csifw_ctx->p_iecho = p_iecho;
	p_csifw_ctx->socketAddr = socketAddr;
	p_csifw_ctx->ping_socket = ping_socket;
    
	return CSIFW_OK;
}

CSIFW_RES csi_ping_generator_cleanup(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	if (p_csifw_ctx->p_iecho) {
		free(p_csifw_ctx->p_iecho);
	}
	if (ping_generator_close_socket(&p_csifw_ctx->ping_socket, &p_csifw_ctx->addr_info) != CSIFW_OK) {
		CSIFW_LOGE("Failed to close ping socket :%d", CSIFW_ERROR);
		return CSIFW_ERROR;
	}
	return CSIFW_OK;
}

CSIFW_RES ping_generator_stop(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	if (p_csifw_ctx->ping_Interval == 0) {
		CSIFW_LOGI("Ping interval is 0 ==> PING STOP NOT REQUIRED", p_csifw_ctx->ping_Interval);
		return CSIFW_OK;
	}
	p_csifw_ctx->ping_thread_stop = true;
	pthread_join(p_csifw_ctx->csi_ping_thread, NULL);
	CSIFW_LOGD("[PING Thread] stopped");
	return CSIFW_OK;
}

static int setIcmp4Config(struct addrinfo *hints)
{
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
	return CSIFW_OK;
}

static CSIFW_RES ping_generator_open_socket(int *ping_socket, struct sockaddr **socketAddr, struct addrinfo **addr_info_list)
{
	const char *tAddr;
	char ipv4_buf[16];
	char ipv4_address[4];
	if (netlib_get_ipv4_gateway_addr("wlan0", (struct in_addr *)ipv4_address) == 0) {
		snprintf(ipv4_buf, 16, "%d.%d.%d.%d", ipv4_address[0], ipv4_address[1], ipv4_address[2], ipv4_address[3]);
	}

	struct timeval tv;
	struct addrinfo hints;
	struct addrinfo *rp = NULL;

	if (setIcmp4Config(&hints) != CSIFW_OK) {
		CSIFW_LOGE("Failed to set ICMP4 config: invalid IP address");
		return CSIFW_ERROR;
	}
	tAddr = ipv4_buf;
	CSIFW_LOGI("PING %s (%s)bytes of data.", tAddr, tAddr);

	if (getaddrinfo(tAddr, NULL, &hints, addr_info_list) != 0) {
		CSIFW_LOGE("Failed to get address info for %s - errno=%d (%s)", tAddr, errno, strerror(errno));
		return CSIFW_ERROR;
	}
	for (rp = *addr_info_list; rp != NULL; rp = rp->ai_next) {
		if ((*ping_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
			continue;
		} else {
			CSIFW_LOGD("Socket created successfully (fd=%d, family=%d, type=%d, proto=%d)",
						*ping_socket, rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			break;
		}
	}
	CSIFW_LOGD("Using socket %d for ping operations", *ping_socket);
	if (rp == NULL) {
		CSIFW_LOGE("Failed to create raw socket for %s - errno=%d (%s)",
					tAddr, errno, strerror(errno));
		return CSIFW_ERROR;
	}

	*socketAddr = rp->ai_addr;
	if (!(*socketAddr)) {
		CSIFW_LOGE("Invalid socket address for %s",socketAddr);
		return CSIFW_ERROR;
	}
	if ((*socketAddr)->sa_family != AF_INET) {
		CSIFW_LOGE("Invalid socket family %d (expected AF_INET)",
				(*socketAddr)->sa_family);
		return CSIFW_ERROR;
	}

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	if (setsockopt(*ping_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) != ERR_OK) {
		CSIFW_LOGE("setsockopt failed for socket %d - errno=%d (%s)",
				*ping_socket, errno, strerror(errno));
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("Socket options set successfully (fd=%d, timeout=%d sec)", 
			*ping_socket, tv.tv_sec);
	return CSIFW_OK;
}

static CSIFW_RES ping_generator_close_socket(int *ping_socket, struct addrinfo **addr_info_list)
{
	CSIFW_RES res = CSIFW_OK;
	if (*addr_info_list) {
		freeaddrinfo(*addr_info_list);
		CSIFW_LOGD("Freed address info resources");
	}

	if (*ping_socket >= 0) {
		if (close(*ping_socket) < 0) {
			CSIFW_LOGE("Failed to close socket %d - errno=%d (%s)",
					*ping_socket, errno, strerror(errno));
			res = CSIFW_ERROR;
		}
		CSIFW_LOGD("Ping generator socket %d closed successfully", *ping_socket);
		*ping_socket = -1;
	}
	return res;
}
