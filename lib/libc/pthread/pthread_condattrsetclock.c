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
 * libc/pthread/pthread_condattrinit.c
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

#include <sys/types.h>
#include <pthread.h>
#include <debug.h>
#include <errno.h>

/****************************************************************************
 * Global Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  pthread_condattr_setclock
 *
 * Description:
 *   Operations on condition variable attributes
 *
 * Parameters:
 *   attr
 *        A pointer to the pthread_condattr_t object for which you want to
 *        set the clock.
 *   clock_id
 *        A clockid_t object that specifies the ID of the clock that you want
 *        to use for the condition variable; one of:CLOCK_REALTIME
 *
 * Return Value:
 *   0 on successful. Appropriate error value otherwise.
 *
 * Assumptions:
 *
 ****************************************************************************/

#define COND_NWAITERS_SHIFT 1

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id)
{
	/* Only a few clocks are allowed.  */
	if (clock_id != CLOCK_MONOTONIC && clock_id != CLOCK_REALTIME)
		/* If more clocks are allowed some day the storing of the clock ID
		   in the pthread_cond_t structure needs to be adjusted.  */
	{
		return EINVAL;
	}

	/* Make sure the value fits in the bits we reserved.  */
	//assert (clock_id < (1 << COND_NWAITERS_SHIFT));

	// int *valuep = &((struct pthread_condattr *)attr)->value;

	/*
	 *valuep = ((*valuep & ~(((1 << COND_NWAITERS_SHIFT) - 1) << 1)) | (clock_id << 1));
	 */
	return 0;
}
