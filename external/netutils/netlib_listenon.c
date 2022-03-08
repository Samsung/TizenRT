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
 * netutils/netlib/netlib_listenon.c
 *
 *   Copyright (C) 2007-2009, 2011 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <debug.h>
#include <netutils/netlib.h>
#include <tinyara/net/netlog.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: netlib_listenon
 *
 * Description:
 *   Implement basic server listening
 *
 * Parameters:
 *   portno    The port to listen on (in network byte order)
 *
 * Return:
 *   A valid listening socket or -1 on error.
 *
 ****************************************************************************/

int netlib_listenon(uint16_t portno)
{
	struct sockaddr_in myaddr;
	int listensd;
#ifdef CONFIG_NET_HAVE_REUSEADDR
	int optval;
#endif

	/* Create a new TCP socket to use to listen for connections */

	listensd = socket(PF_INET, SOCK_STREAM, 0);
	if (listensd < 0) {
		NET_LOGE(NL_MOD_NETLIB, "socket failure: %d\n", errno);
		return ERROR;
	}

	/* Set socket to reuse address */

#ifdef CONFIG_NET_HAVE_REUSEADDR
	optval = 1;
	if (setsockopt(listensd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0) {
		NET_LOGE(NL_MOD_NETLIB, "setsockopt SO_REUSEADDR failure: %d\n", errno);
		goto errout_with_socket;
	}
#endif

	/* Bind the socket to a local address */

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = portno;
	myaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listensd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr_in)) < 0) {
		NET_LOGE(NL_MOD_NETLIB, "bind failure: %d\n", errno);
		goto errout_with_socket;
	}

	/* Listen for connections on the bound TCP socket */

	if (listen(listensd, 5) < 0) {
		NET_LOGE(NL_MOD_NETLIB, "listen failure %d\n", errno);
		goto errout_with_socket;
	}

	/* Begin accepting connections */

	NET_LOGV(NL_MOD_NETLIB, "Accepting connections on port %d\n", ntohs(portno));
	return listensd;

errout_with_socket:
	close(listensd);
	return ERROR;
}
