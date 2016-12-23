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
 * net/socket/net_clone.c
 *
 *   Copyright (C) 2009, 2011-2015 Gregory Nutt. All rights reserved.
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
#ifdef CONFIG_NET

#include <sys/socket.h>

#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/net/net.h>

#include "socket/socket.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: net_clone
 *
 * Description:
 *   Performs the low level, common portion of net_dupsd() and net_dupsd2()
 *
 ****************************************************************************/

int net_clone(FAR struct socket *sock1, FAR struct socket *sock2)
{
	net_lock_t flags;
	int ret = OK;

	/* Parts of this operation need to be atomic */

	flags = net_lock();

	/* Duplicate the socket state */
	sock2->conn = sock1->conn;	/* Netconn callback */
	sock2->lastdata = sock1->lastdata;	/* data that was left from the previous read */
	sock2->lastoffset = sock1->lastoffset;	/* offset in the data that was left from the previous read */
	sock2->rcvevent = sock1->rcvevent;	/*  number of times data was received, set by event_callback(),
										   tested by the receive and select / poll functions */
	sock2->sendevent = sock1->sendevent;	/* number of times data was ACKed (free send buffer), set by event_callback(),
											   tested by select / poll */
	sock2->errevent = sock1->errevent;	/* error happened for this socket, set by event_callback(), tested by select / poll */

	sock2->err = sock1->err;	/* last error that occurred on this socket */

	sock2->select_waiting = sock1->select_waiting;	/* counter of how many threads are waiting for this socket using select */
	sock2->conn->crefs++;
	net_unlock(flags);

	return ret;
}

#endif							/* CONFIG_NET */
