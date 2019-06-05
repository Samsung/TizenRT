/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * os/binfmt/binfmt_unregister.c
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

#include <string.h>
#include <sched.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/binfmt/binfmt.h>

#include "binfmt.h"

#ifdef CONFIG_BINFMT_ENABLE

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: unregister_binfmt
 *
 * Description:
 *   Unregister a loader for a binary format
 *
 * Returned Value:
 *   This is a internal function so it follows the convention that
 *   0 (OK) is returned on success and a negated errno is returned on
 *   failure.
 *
 ****************************************************************************/

int unregister_binfmt(FAR struct binfmt_s *binfmt)
{
	FAR struct binfmt_s *curr;
	FAR struct binfmt_s *prev;
	int ret = -EINVAL;

	if (binfmt) {
		/* Disabling pre-emption should be sufficient protection while
		 * accessing the list of registered binary format handlers.
		 */

		sched_lock();

		/* Search the list of registered binary format handlers for the
		 * one to be unregistered.
		 */

		for (prev = NULL, curr = g_binfmts; curr && curr != binfmt; prev = curr, curr = curr->next) ;

		/* Was it in the list? */

		if (curr) {
			/* Yes.. was it at the head of the list? */

			if (!prev) {
				/* Yes.. remove it from the head of the list */

				g_binfmts = binfmt->next;
			} else {
				/* No.. remove it from the middle/end of the list */

				prev->next = binfmt->next;
			}

			binfmt->next = NULL;
			ret = OK;
		}

		sched_unlock();
	}

	return ret;
}

#endif							/* CONFIG_BINFMT_ENABLE */
