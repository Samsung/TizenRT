/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include "include/PingGenerator.h"
#include <netdb.h>
#include <lwip/prot/icmp.h>
#include <pthread.h>

extern COLLECT_STATE g_service_state;
static unsigned int gPingInterval = 0;
static pthread_t gPingThread;
static g_ping_count;

void* pingThreadFun(void *vargp);
static CSIFW_RES ping_generator_open_socket(int *ping_socket, struct sockaddr **socketAddr, struct addrinfo **result);
static CSIFW_RES ping_generator_close_socket(int *ping_socket, struct addrinfo **result);
static int setIcmp4Config(struct addrinfo *hints);
static bool gStopPingThread;

#define PING_TH_NAME "csifw_ping"

void ping_generator_change_interval(unsigned int pingInterval)
{
	CSIFW_LOGI("Ping interval set to %u MS", pingInterval);  // interval in ms 
	gPingInterval = pingInterval * 1000;              // converting interval in microsecond
}

static u16_t standard_chksum(void *dataptr, u16_t len)
{
	u32_t acc;
	u16_t src;
	u8_t *octetptr;

	acc = 0;
	/* dataptr may be at odd or even addresses */
	octetptr = (u8_t *) dataptr;
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
	return htons((u16_t) acc);
}

static void ping_prepare_echo(struct icmp_echo_hdr *p_iecho, u16_t len)
{
	size_t i = 0;
	int icmp_hdrlen = sizeof(struct icmp_echo_hdr);
	ICMPH_CODE_SET(p_iecho, 0);
	p_iecho->id = 0xAFAF; //PING_ID;
	p_iecho->seqno = htons(1);

	/* fill the additional data buffer with some data */
	for (i = icmp_hdrlen; i < len; i++) {
		((char *)p_iecho)[i] = (char)i;
	}

	ICMPH_TYPE_SET(p_iecho, ICMP_ECHO);
	p_iecho->chksum = 0;
	p_iecho->chksum = ~standard_chksum(p_iecho, len);

}


static CSIFW_RES ping_send(int ping_socket, struct icmp_echo_hdr *p_iecho, struct sockaddr *socketAddr)
{
	g_ping_count++;
	if(!(g_ping_count % 1000)) CSIFW_LOGD("ping_send(): ping sent %d times", g_ping_count);
    
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	size_t icmplen = sizeof(struct icmp_echo_hdr) + 8;

	ret = sendto(ping_socket, p_iecho, icmplen, 0, socketAddr, addrlen);
	if (ret <= 0) {
		CSIFW_LOGE("sendto() return val is %d, errno: %d", ret, errno);
		perror("sendto");
		return CSIFW_ERROR;
	}

	return CSIFW_OK;
}

void* pingThreadFun(void *vargp)
{
	int ping_socket = -1;
	struct addrinfo *result = NULL;
	struct sockaddr *socketAddr = NULL;

	if (ping_generator_open_socket(&ping_socket, &socketAddr, &result) != CSIFW_OK) {
		CSIFW_LOGE("Start collect failed as socket open failed");
		if (ping_generator_close_socket(&ping_socket, &result) != CSIFW_OK) {
			CSIFW_LOGE("socket close failed");
		}
		return NULL;
	}

	size_t icmplen = sizeof(struct icmp_echo_hdr) + 8;
	struct icmp_echo_hdr *p_iecho = NULL;

	/*	ToDo: Check this repetitive malloc */
	p_iecho = (struct icmp_echo_hdr *)malloc(icmplen);
	if (!p_iecho) {
		CSIFW_LOGE("fail to allocate memory %zu", icmplen);
		ping_generator_close_socket(&ping_socket, &result);
		return NULL;
	}
	ping_prepare_echo(p_iecho, (u16_t)icmplen);

    while (!gStopPingThread) {
		usleep(gPingInterval);
		if (ping_send(ping_socket, p_iecho, socketAddr) == CSIFW_OK) {
		} else {
			CSIFW_LOGE("[T] ping_process: sendto error(%d)", errno);
		}
	}
	if (p_iecho) {
		free(p_iecho);
	}
	if (ping_generator_close_socket(&ping_socket, &result) != CSIFW_OK) {
		CSIFW_LOGE("socket close failed");
	}
	CSIFW_LOGD("[PING Thread] THREAD EXIT");
	return NULL;
}

CSIFW_RES ping_generator_start(void)
{
	if (gPingInterval == 0) {
		CSIFW_LOGD("Ping interval is 0 ==> NO PING START", gPingInterval);
		return CSIFW_OK;
	}
	g_ping_count = 0;
	gPingThread = 0;
	// create thread and values init
	gStopPingThread = false;
	if (pthread_create(&gPingThread, NULL, pingThreadFun, NULL) != 0) {
		CSIFW_LOGE("Failed to create csi data collect thread");
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(gPingThread, PING_TH_NAME) != 0) {
		CSIFW_LOGE("Error in setting ping thread name, error_no: %d", get_errno());
	}
	CSIFW_LOGI("Thread created and values initialized");
	return CSIFW_OK;
}

CSIFW_RES ping_generator_stop(void)
{
	if (gPingInterval == 0) { // means ping never started
		CSIFW_LOGI("Ping interval is 0 ==> PING STOP NOT REQUIRED", gPingInterval);
		return CSIFW_OK;
	}
	gStopPingThread = true;
	pthread_join(gPingThread, NULL);
	CSIFW_LOGI("Ping thread stopped");
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

static CSIFW_RES ping_generator_open_socket(int *ping_socket, struct sockaddr **socketAddr, struct addrinfo **result)
{
	char ipv4_address[4];
	char ipv4_buf[16];
	const char *tAddr;
	if (netlib_get_ipv4_gateway_addr("wlan0", ipv4_address) == 0) {
			snprintf(ipv4_buf, 16, "%d.%d.%d.%d", ipv4_address[0], ipv4_address[1], ipv4_address[2], ipv4_address[3]);
	}
	
	struct timeval tv;
	struct timespec ping_time;
	struct addrinfo hints;
	struct addrinfo *rp = NULL;

	/* write information for getaddrinfo() */
	if (setIcmp4Config(&hints) != CSIFW_OK) {
		CSIFW_LOGE("ping_process: invalid IP address");
		return CSIFW_ERROR;
	}
	tAddr = ipv4_buf;
	CSIFW_LOGI("PING %s (%s)bytes of data.", tAddr, tAddr);

	/* get address information */
	if (getaddrinfo(tAddr, NULL, &hints, result) != 0) {
		CSIFW_LOGE("ping_process: fail to get addrinfo");
		return CSIFW_ERROR;
	}
	/* try to find valid socket with address information */
	for (rp = *result; rp != NULL; rp = rp->ai_next) {
		if ((*ping_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
			/* continue if open is failed */
			continue;
		} else {
			/* success */
			CSIFW_LOGI("SOCKET SET!!!");
			break;
		}
	}
	CSIFW_LOGI("ping_socket: %d", *ping_socket);
	if (rp == NULL) {
		/* opening socket is totally failed */
		CSIFW_LOGE("ping_process: fail to create raw socket");
		return CSIFW_ERROR;
	}

	/* copy the socket pointer we found */
	*socketAddr = rp->ai_addr;
	if (!(*socketAddr))
	{
		CSIFW_LOGE("ping_send: invalid rp->ai_addr");
		return CSIFW_ERROR;
	}

	if ((*socketAddr)->sa_family != AF_INET) {
		CSIFW_LOGE("ping_send: invalid family");
		return CSIFW_ERROR;
	}

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	if (setsockopt(*ping_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) != ERR_OK) {
		CSIFW_LOGE("ping_process: setsockopt error");
		return CSIFW_ERROR;
	}
	CSIFW_LOGI("setsockopt Done");
	return CSIFW_OK;
}

static CSIFW_RES ping_generator_close_socket(int *ping_socket, struct addrinfo **result)
{
	CSIFW_RES res = CSIFW_OK;
	if (*result) {
		freeaddrinfo(*result);
		CSIFW_LOGI("freeaddrinfo Done");
		}

	if (*ping_socket >= 0) {
		if (close(*ping_socket) < 0) {
			CSIFW_LOGE("close Socket failed");
			res = CSIFW_ERROR;
		}
		*ping_socket = -1;
	}
	CSIFW_LOGI("Ping generator socket closed");
	return res;
}

