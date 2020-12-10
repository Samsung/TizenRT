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
 * net/socket/net_sockets.c
 *
 *   Copyright (C) 2007-2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#include <string.h>
#include <semaphore.h>
#include <assert.h>
#include <sched.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/net/net.h>
#include <tinyara/kmalloc.h>

#include <sys/socket.h>
#include "socket/socket.h"

#if CONFIG_NSOCKET_DESCRIPTORS > 0

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: net_initlist
 *
 * Description:
 *   Initialize a list of sockets for a new task
 *
 * Input Parameters:
 *   list -- A reference to the pre-allocated socket list to be initialized.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_initlist(FAR struct socketlist *list)
{
	for (int i = 0; i < CONFIG_NSOCKET_DESCRIPTORS; ++i) {
		list->sl_sockets[i].sock = NULL;
	}

	/* Initialize the list access mutex */
	(void)sem_init(&list->sl_sem, 0, 1);
}

/****************************************************************************
 * Name: net_releaselist
 *
 * Description:
 *   Release resources held by the socket list
 *
 * Input Parameters:
 *   list -- A reference to the pre-allocated socket list to be un-initialized.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void net_releaselist(FAR struct socketlist *list)
{
	int i;
	int ret;
	struct lwip_sock *psock;

	for (i = 0; i < CONFIG_NSOCKET_DESCRIPTORS; ++i) {
		if (list->sl_sockets[i].sock) {

			psock = (struct lwip_sock *)list->sl_sockets[i].sock;

			if (list->sl_sockets[i].s_crefs == 1) {
				ret = lwip_sock_close(psock);
				if (ret) {
					ndbg("socket could not close properly\n");
					list->sl_sockets[i].sock = NULL;
					(void)sem_destroy(&list->sl_sem);
					return;
				}
			}

			else if (list->sl_sockets[i].s_crefs > 1) {
				list->sl_sockets[i].s_crefs--;
			}

			else {
				memset(&list->sl_sockets[i], 0, sizeof(struct socket));
			}
		}
		list->sl_sockets[i].sock = NULL;
	}

	/* Destroy the semaphore */
	(void)sem_destroy(&list->sl_sem);
}

#endif							/* CONFIG_NSOCKET_DESCRIPTORS > 0 */
