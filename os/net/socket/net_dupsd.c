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
 * net/socket/net_dupsd.c
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/socket.h>

#include <sched.h>
#include <errno.h>
#include <debug.h>

#include "socket/socket.h"

#if defined(CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: net_dupsd
 *
 * Description:
 *   Clone a socket descriptor to an arbitray descriptor number.  If file
 *   descriptors are implemented, then this is called by dup() for the case
 *   of socket file descriptors.  If file descriptors are not implemented,
 *   then this function IS dup().
 *
 ****************************************************************************/

int net_dupsd(int sockfd)
{
	struct socket *sock1 = NULL;
	struct socket *sock2 = NULL;
	struct netconn *conn = NULL;
	int sockfd2;
	int err;
	int ret;

	/* Lock the scheduler throughout the following */

	sched_lock();

	/* Get the socket structure underlying sockfd */

	sock1 = get_socket(sockfd);

	/* Verify that the sockfd corresponds to valid, allocated socket */

	if (!sock1) {
		err = EBADF;
		goto errout;
	}

	/* Allocate a new netconn with same type as that of sock1 */

	conn = netconn_new(sock1->conn->type);

	sockfd2 = alloc_socket(conn, 0);
	if (sockfd2 < 0) {
		err = ENFILE;
		goto errout;
	}

	/* Get the socket structure underlying the new descriptor */

	sock2 = (struct socket *)get_socket(sockfd2);
	if (!sock2) {
		err = ENOSYS;			/* should not happen */
		goto errout;
	}

	/* Duplicate the socket state */

	ret = net_clone(sock1, sock2);
	if (ret < 0) {
		err = -ret;
		goto errout;

	}

	sched_unlock();
	return sockfd2;

errout:
	sched_unlock();
	errno = err;
	return ERROR;
}

#endif							/* defined(CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0 */
