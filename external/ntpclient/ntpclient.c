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
/****************************************************************************
 * netutils/ntpclient/ntpclient.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @file ntpclient.c
 * @brief ntpclient core and API
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/socket.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <debug.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <protocols/ntpclient.h>
#include <netdb.h>
#include "ntpv3.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* NTP Time is seconds since 1900. Convert to Unix time which is seconds
 * since 1970
 */
#define NTP2UNIX_TRANLSLATION 2208988800u
#define NTP_VERSION           3

#ifndef CONFIG_NETUTILS_NTPCLIENT_DEBUG
#define CONFIG_NETUTILS_NTPCLIENT_DEBUG		0
#endif

#ifndef CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER
#define CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER		"time.google.com"
#endif

#ifndef CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT
#define CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT		123
#endif

#ifndef CONFIG_NETUTILS_NTPCLIENT_DEFAULT_INTERVAL_SECONDS
#define CONFIG_NETUTILS_NTPCLIENT_DEFAULT_INTERVAL_SECONDS		86400
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This type describes the state of the NTP client daemon.  Only once
 * instance of the NTP daemon is permitted in this implementation.
 */

struct ntpc_daemon_s {
	volatile uint8_t state;		/* See enum ntpc_daemon_e */
	sem_t interlock;			/* Used to synchronize start and stop events */
	pid_t pid;					/* Task ID of the NTP daemon */
};

struct ntpc_server_info_s {
	int num_of_servers;
	struct ntpc_server_s server[MAX_NTP_SERVER_NUM];
	int interval_secs;
};

void (*ntp_link_err_cb)(void);
/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This type describes the state of the NTP client daemon.  Only once
 * instance of the NTP daemon is permitted in this implementation.  This
 * limitation is due only to this global data structure.
 */

static struct ntpc_daemon_s g_ntpc_daemon;
static struct ntpc_server_info_s g_ntps;
static int g_debug = CONFIG_NETUTILS_NTPCLIENT_DEBUG;

/****************************************************************************
 * Private Functions Prototype
 ****************************************************************************/
static void ntpc_destroy_server_info(void);

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: ntpc_getuint32
 *
 * Description:
 *   Return the big-endian, 4-byte value in network (big-endian) order.
 *
 ****************************************************************************/

static inline uint32_t ntpc_getuint32(FAR uint8_t *ptr)
{
	/* Network order is big-endian; host order is irrelevant */

	return (uint32_t)ptr[3] |	/* MS byte appears first in data stream */
		   ((uint32_t)ptr[2] << 8) | ((uint32_t)ptr[1] << 16) | ((uint32_t)ptr[0] << 24);
}

/****************************************************************************
 * Name: ntpc_settime
 *
 * Description:
 *   Given the NTP time in seconds, set the system time
 *
 ****************************************************************************/

static void ntpc_settime(FAR uint8_t *timestamp)
{
	struct timespec tp;
	time_t seconds;
	uint32_t frac;
	uint32_t nsec;
#ifdef CONFIG_HAVE_LONG_LONG
	uint64_t tmp;
#else
	uint32_t a16;
	uint32_t b0;
	uint32_t t32;
	uint32_t t16;
	uint32_t t0;
#endif

	/* NTP timestamps are represented as a 64-bit fixed-point number, in
	 * seconds relative to 0000 UT on 1 January 1900.  The integer part is
	 * in the first 32 bits and the fraction part in the last 32 bits, as
	 * shown in the following diagram.
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Integer Part                          |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Fraction Part                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 */

	seconds = ntpc_getuint32(timestamp);

	/* Translate seconds to account for the difference in the origin time */

	if (seconds > NTP2UNIX_TRANLSLATION) {
		seconds -= NTP2UNIX_TRANLSLATION;
	}

	/* Conversion of the fractional part to nanoseconds:
	 *
	 *  NSec = (f * 1,000,000,000) / 4,294,967,296
	 *       = (f * (5**9 * 2**9) / (2**32)
	 *       = (f * 5**9) / (2**23)
	 *       = (f * 1,953,125) / 8,388,608
	 */

	frac = ntpc_getuint32(timestamp + 4);
#ifdef CONFIG_HAVE_LONG_LONG
	/* if we have 64-bit long long values, then the computation is easy */

	tmp = ((uint64_t)frac * 1953125) >> 23;
	nsec = (uint32_t)tmp;

#else
	/* If we don't have 64 bit integer types, then the calculation is a little
	 * more complex:
	 *
	 * Let f         = a    << 16 + b
	 *     1,953,125 = 0x1d << 16 + 0xcd65
	 * NSec << 23 =  ((a << 16) + b) * ((0x1d << 16) + 0xcd65)
	 *            = (a << 16) * 0x1d << 16) +
	 *              (a << 16) * 0xcd65 +
	 *              b         * 0x1d << 16) +
	 *              b         * 0xcd65;
	 */

	/* Break the fractional part up into two values */

	a16 = frac >> 16;
	b0 = frac & 0xffff;

	/* Get the b32 and b0 terms
	 *
	 * t32 = (a << 16) * 0x1d << 16)
	 * t0  = b * 0xcd65
	 */

	t32 = 0x001d * a16;
	t0 = 0xcd65 * b0;

	/* Get the first b16 term
	 *
	 * (a << 16) * 0xcd65
	 */

	t16 = 0xcd65 * a16;

	/* Add the upper 16-bits to the b32 accumulator */

	t32 += (t16 >> 16);

	/* Add the lower 16-bits to the b0 accumulator, handling carry to the b32
	 * accumulator
	 */

	t16 <<= 16;
	if (t0 > (0xffffffff - t16)) {
		t32++;
	}

	t0 += t16;

	/* Get the second b16 term
	 *
	 * b * (0x1d << 16)
	 */

	t16 = 0x001d * b0;

	/* Add the upper 16-bits to the b32 accumulator */

	t32 += (t16 >> 16);

	/* Add the lower 16-bits to the b0 accumulator, handling carry to the b32
	 * accumulator
	 */

	t16 <<= 16;
	if (t0 > (0xffffffff - t16)) {
		t32++;
	}

	t0 += t16;

	/* t32 and t0 represent the 64 bit product.  Now shift right by 23 bits to
	 * accomplish the divide by by 2**23.
	 */

	nsec = (t32 << (32 - 23)) + (t0 >> 23);
#endif

	/* Set the system time */

	tp.tv_sec = seconds;
	tp.tv_nsec = nsec;
	clock_settime(CLOCK_REALTIME, &tp);

	ndbg("Set time to %lu seconds\n", (unsigned long)tp.tv_sec);
}

/****************************************************************************
 * Name: ntpc_daemon
 *
 * Description:
 *   This the the NTP client daemon.  This is a *very* minimal
 *   implementation! An NTP request is and the system clock is set when the
 *   response is received
 *
 ****************************************************************************/

static int ntpc_daemon(int argc, char **argv)
{
	struct sockaddr_in server;
	struct ntp_datagram_s xmit;
	struct ntp_datagram_s recv;
	struct timeval tv;
	socklen_t socklen;
	ssize_t nbytes;
	int exitcode = EXIT_SUCCESS;
	int ret = 0;
	int sd;
	fd_set sockfd_set;
	int srv_index;

	/* Indicate that we have started */

	g_ntpc_daemon.state = NTP_RUNNING;
	sem_post(&g_ntpc_daemon.interlock);

	/* Create a datagram socket  */

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		ndbg("ERROR: socket failed: %d\n", errno);

		g_ntpc_daemon.state = NTP_STOPPED;
		sem_post(&g_ntpc_daemon.interlock);
		return EXIT_FAILURE;
	}

	/* Setup a receive timeout on the socket */
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	srv_index = 0;

	/* Here we do the communication with the NTP server.  This is a very simple
	 * client architecture.  A request is sent and then a NTP packet is received
	 * and used to set the current time.
	 *
	 * NOTE that the scheduler is locked whenever this loop runs.  That
	 * assures both:  (1) that there are no asynchronous stop requests and
	 * (2) that we are not suspended while in critical moments when we about
	 * to set the new time.  This sounds harsh, but this function is suspended
	 * most of the time either: (1) sending a datagram, (2) receiving a datagram,
	 * or (3) waiting for the next poll cycle.
	 *
	 * TODO: The first datagram that is sent is usually lost.  That is because
	 * the MAC address of the NTP server is not in the ARP table.  This is
	 * particularly bad here because the request will not be sent again until
	 * the long delay expires leaving the system with bad time for a long time
	 * initially.  Solutions:
	 *
	 * 1. Fix send logic so that it assures that the ARP request has been
	 *    sent and the entry is in the ARP table before sending the packet
	 *    (best).
	 * 2. Add some ad hoc logic here so that there is no delay until at least
	 *    one good time is received.
	 */

	while (g_ntpc_daemon.state != NTP_STOP_REQUESTED) {
		/* Format the transmit datagram */

		memset(&xmit, 0, sizeof(xmit));
		xmit.lvm = MKLVM(0, 3, NTP_VERSION);

		svdbg("Sending a NTP packet\n");

		if (g_debug) {
			ndbg("server : %s (%d.%d.%d.%d:%d)\n", g_ntps.server[srv_index].conn.hostname, (g_ntps.server[srv_index].ipaddr >> 0) & 0xFF, (g_ntps.server[srv_index].ipaddr >> 8) & 0xFF, (g_ntps.server[srv_index].ipaddr >> 16) & 0xFF, (g_ntps.server[srv_index].ipaddr >> 24) & 0xFF, g_ntps.server[srv_index].conn.port);
		}

		/* Setup or sockaddr_in struct with information about the server we are
		 * going to ask the the time from.
		 */
		memset(&server, 0, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_port = htons(g_ntps.server[srv_index].conn.port);
		server.sin_addr.s_addr = g_ntps.server[srv_index].ipaddr;

		ret = sendto(sd, &xmit, sizeof(struct ntp_datagram_s), 0, (FAR struct sockaddr *)&server, sizeof(struct sockaddr_in));

		if (ret < 0) {
			/* Check if we received a signal.  That is not an error but
			 * other error events will terminate the client.
			 */

			int errval = errno;
			if (errval != EINTR) {
				ndbg("ERROR: sendto() failed (errno: %d)\n", errval);
			}

			/* Go back to the top of the loop if we were interrupted
			 * by a signal.  The signal might mean that we were
			 * requested to stop(?)
			 */

			g_ntps.server[srv_index].link = NTP_LINK_DOWN;	/* current server's link is down */
			srv_index++;		/* increase server index */
			if (srv_index >= g_ntps.num_of_servers) {
				srv_index = 0;
				if (ntp_link_err_cb) {
					ntp_link_err_cb();
				}
			}

			continue;
		}

		/* Attempt to receive a packet (with a timeout that was set up via
		 * setsockopt() above)
		 */

		nbytes = -1;
		FD_ZERO(&sockfd_set);
		FD_SET(sd, &sockfd_set);
		ret = select(sd + 1, &sockfd_set, NULL, NULL, &tv);
		if ((ret > 0) && FD_ISSET(sd, &sockfd_set)) {
			socklen = sizeof(struct sockaddr_in);
			nbytes = recvfrom(sd, (void *)&recv, sizeof(struct ntp_datagram_s), 0, (FAR struct sockaddr *)&server, &socklen);
		}

		/* Check if the received message was long enough to be a valid NTP
		 * datagram.
		 */

		if (nbytes >= (ssize_t)NTP_DATAGRAM_MINSIZE) {
			svdbg("Setting time\n");
			ntpc_settime(recv.recvtimestamp);
			g_ntps.server[srv_index].link = NTP_LINK_UP;
		}

		/* Check for errors.  Note that properly received, short datagrams
		 * are simply ignored.
		 */

		else if (nbytes < 0) {
			/* Check if we received a signal.  That is not an error but
			 * other error events will terminate the client.
			 */

			int errval = errno;
			if (errval != EINTR) {
				ndbg("ERROR: recvfrom() failed (errno: %d)\n", errval);
				if (g_debug) {
					ndbg("ntp client cannot receive time information from ntp server.\n");
					ndbg("maybe, ntp server is not connected.\n");
				}
			}

			g_ntps.server[srv_index].link = NTP_LINK_DOWN;	/* current server's link is down */
			srv_index++;		/* increase server index */
			if (srv_index >= g_ntps.num_of_servers) {
				srv_index = 0;
				if (ntp_link_err_cb) {
					ntp_link_err_cb();
				}
			}
			continue;

		}

		/* A full implementation of an NTP client would require much more.  I
		 * think we can skip most of that here.
		 */

		if (g_ntpc_daemon.state == NTP_RUNNING) {

			if (g_debug) {
				ndbg("Waiting for %d seconds\n", g_ntps.interval_secs);
			}

			(void)sleep(g_ntps.interval_secs);
		}
	}

	/* The NTP client is terminating */

	if (g_debug) {
		ndbg("daemon stop\n");
	}

	close(sd);

	/* destroy server information */
	ntpc_destroy_server_info();

	g_ntpc_daemon.state = NTP_STOPPED;
	sem_post(&g_ntpc_daemon.interlock);
	return exitcode;
}

/****************************************************************************
 * Name: ntpc_init_server_info
 *
 * Description:
 *   Initialize NTP server information.
 *
 ****************************************************************************/

static int ntpc_init_server_info(struct ntpc_server_conn_s *server_list, uint32_t num_of_servers, int interval_secs)
{
	int result = -1;
	struct hostent *he;
	int i;
	int server_count;

	if (num_of_servers > MAX_NTP_SERVER_NUM) {
		ndbg("ERROR: too many servers \n");
		goto done;
	}

	if ((interval_secs < MIN_NTP_POLL_INTERVAL_SECS) || (interval_secs > MAX_NTP_POLL_INTERVAL_SECS)) {
		ndbg("ERROR: invalid poll interval. poll interval should be %d ~ %d secs. \n", MIN_NTP_POLL_INTERVAL_SECS, MAX_NTP_POLL_INTERVAL_SECS);
		goto done;
	}

	server_count = 0;
	memset(&g_ntps, 0x0, sizeof(g_ntps));

	for (i = 0; i < num_of_servers; i++) {
		he = gethostbyname(server_list[i].hostname);
		if (!he) {
			continue;
		}

		g_ntps.server[server_count].conn.hostname = strdup(server_list[i].hostname);
		g_ntps.server[server_count].conn.port = server_list[i].port;
		g_ntps.server[server_count].link = NTP_LINK_NOT_SET;
		memcpy(&g_ntps.server[server_count].ipaddr, he->h_addr, sizeof(in_addr_t));
		ndbg("ntpc_init_server_info : %s %d\n", g_ntps.server[server_count].conn.hostname, g_ntps.server[server_count].conn.port);
		server_count++;
	}

	g_ntps.num_of_servers = server_count;
	g_ntps.interval_secs = interval_secs;

	if (g_ntps.num_of_servers == 0) {
		ndbg("ERROR: there is no valid server. \n");
		goto done;
	}

	/* success */
	result = 0;

done:
	return result;

}

/****************************************************************************
 * Name: ntpc_destroy_server_info
 *
 * Description:
 *   Destroy NTP server information.
 *
 ****************************************************************************/

static void ntpc_destroy_server_info(void)
{
	int i;

	for (i = 0; i < g_ntps.num_of_servers; i++) {
		if (g_ntps.server[i].conn.hostname) {
			free(g_ntps.server[i].conn.hostname);
			g_ntps.server[i].conn.hostname = NULL;
		}
	}

	memset(&g_ntps, 0x0, sizeof(g_ntps));
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: ntpc_start
 *
 * Description:
 *   Start the NTP daemon
 *
 * Returned Value:
 *   On success, the non-negative task ID of the NTPC daemon is returned;
 *   On failure, a negated errno value is returned.
 *
 ****************************************************************************/

int ntpc_start(struct ntpc_server_conn_s *server_list, uint32_t num_of_servers, uint32_t interval_secs, void *link_error_cb)
{
	int result = ERROR;

	/* Is the NTP in a non-running state? */

	struct ntpc_server_conn_s default_server_conn;
	if (server_list == NULL || num_of_servers == 0) {
		num_of_servers = 1;
		default_server_conn.hostname = CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER;
		default_server_conn.port = CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT;
		server_list = &default_server_conn;
		ndbg("hostname = %s port = %d\n", default_server_conn.hostname, default_server_conn.port);
	}

	if (interval_secs == 0) {
		interval_secs = CONFIG_NETUTILS_NTPCLIENT_DEFAULT_INTERVAL_SECONDS;
		ndbg("interval = %d\n", interval_secs);
	}

	if (num_of_servers > MAX_NTP_SERVER_NUM) {
		ndbg("ERROR: too many servers \n");
		return -EINVAL;
	}

	sched_lock();
	if (g_ntpc_daemon.state == NTP_NOT_RUNNING || g_ntpc_daemon.state == NTP_STOPPED) {
		/* Is this the first time that the NTP daemon has been started? */

		if (g_ntpc_daemon.state == NTP_NOT_RUNNING) {
			/* Yes... then we will need to initialize the state structure */

			sem_init(&g_ntpc_daemon.interlock, 0, 0);
		}

		/* Start the NTP daemon */

		/* initialize ntp server information */
		if (ntpc_init_server_info(server_list, num_of_servers, interval_secs) != 0) {
			ndbg("ERROR: Failed to initialize NTP Server information \n");
			goto done;
		}
		ntp_link_err_cb = link_error_cb;
		g_ntpc_daemon.state = NTP_STARTED;
		g_ntpc_daemon.pid = task_create("NTP daemon", CONFIG_NETUTILS_NTPCLIENT_SERVERPRIO, CONFIG_NETUTILS_NTPCLIENT_STACKSIZE, ntpc_daemon, NULL);

		/* Handle failures to start the NTP daemon */

		if (g_ntpc_daemon.pid < 0) {
			int errval = errno;
			DEBUGASSERT(errval > 0);

			g_ntpc_daemon.state = NTP_STOPPED;
			ndbg("ERROR: Failed to start the NTP daemon\n", errval);
			result = -errval;
			goto done;
		}

		/* Wait for any daemon state change */

		do {
			(void)sem_wait(&g_ntpc_daemon.interlock);
		} while (g_ntpc_daemon.state == NTP_STARTED);
	} else {
		ndbg("ntpclient is already running.\n");
		goto done;
	}

	result = g_ntpc_daemon.pid;

done:
	sched_unlock();

	return result;
}

/****************************************************************************
 * Name: ntpc_stop
 *
 * Description:
 *   Stop the NTP daemon
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_SIGNALS
int ntpc_stop(void)
{
	int result = ERROR;
	int ret;

	/* Is the NTP in a running state? */

	sched_lock();
	if (g_ntpc_daemon.state == NTP_STARTED || g_ntpc_daemon.state == NTP_RUNNING) {

		/* Yes.. request that the daemon stop. */
		g_ntpc_daemon.state = NTP_STOP_REQUESTED;

		/* wait ntpc daemon's response only in case of ntpc_stop() is called outside ntpc_daemon */
		if (g_ntpc_daemon.pid != getpid()) {
			/* Wait for any daemon state change */
			do {
				/* Signal the NTP client */

				ret = kill(g_ntpc_daemon.pid, CONFIG_NETUTILS_NTPCLIENT_SIGWAKEUP);

				if (ret < 0) {
					ndbg("ERROR: kill pid %d failed: %d\n", g_ntpc_daemon.pid, errno);
					break;
				}

				/* Wait for the NTP client to respond to the stop request */

				(void)sem_wait(&g_ntpc_daemon.interlock);
			} while (g_ntpc_daemon.state == NTP_STOP_REQUESTED);
		}
	} else {
		ndbg("ntpclient has already been stopped.\n");
		goto done;
	}

	result = OK;

done:
	sched_unlock();
	return result;
}
#endif

/****************************************************************************
 * Name: ntpc_get_status
 *
 * Description:
 *   Get NTP daemon's status
 *
 * Returned Value:
 *   ntpc daemon's status value (please, refer to enum ntpc_daemon_e definition)
 *
 *
 ****************************************************************************/
int ntpc_get_status(void)
{
	return g_ntpc_daemon.state;
}

/****************************************************************************
 * Name: ntpc_get_link_status
 *
 * Description:
 *   Check if link is up
 *
 * Returned Value:
 *   ntpc's link status (please, refer to enum ntpc_link_status_e definition)
 *
 *
 ****************************************************************************/
int ntpc_get_link_status(void)
{
	int status;

	if (g_ntpc_daemon.state == NTP_STARTED || g_ntpc_daemon.state == NTP_RUNNING) {
		int i;

		int link_up_flag = 0;
		int link_not_set_flag = 0;

		/* check servers' link status  */
		for (i = 0; i < g_ntps.num_of_servers; i++) {
			if (g_ntps.server[i].link == NTP_LINK_UP) {
				link_up_flag = 1;
			} else if (g_ntps.server[i].link == NTP_LINK_NOT_SET) {
				link_not_set_flag = 1;
			}
		}

		if (link_up_flag) {
			/* if link_up_flag is on, ntpc link status is NTP_LINK_UP */
			status = NTP_LINK_UP;
		} else if (link_not_set_flag) {
			/* if link_up_flag is off & link_not_set_flag is on, ntpc link status is NTP_LINK_NOT_SET */
			status = NTP_LINK_NOT_SET;
		} else {
			/* all servers' link are down */
			status = NTP_LINK_DOWN;
		}
	} else {
		status = NTP_LINK_DOWN;
	}

	return status;
}

