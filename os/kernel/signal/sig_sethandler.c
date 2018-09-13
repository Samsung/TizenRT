/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <signal.h>
#include <queue.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <tinyara/signal.h>

#include "signal/signal.h"
/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
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
int sig_sethandler(struct tcb_s *tcb, int signo, struct sigaction *act)
{
	sigactq_t *sigact;

	sched_lock();

	/* No.. Then we need to allocate one for the new action. */
	sigact = (sigactq_t *)sig_allocateaction();

	/* An error has occurred if we could not allocate the sigaction */
	if (!sigact) {
		set_errno(ENOMEM);
		return ERROR;
	}

	/* Put the signal number in the queue entry */
	sigact->signo = (uint8_t)signo;

	/* Add the new sigaction to sigactionq */
	sq_addlast((FAR sq_entry_t *)sigact, &tcb->sigactionq);
	COPY_SIGACTION(&sigact->act, act);

	sched_unlock();
	return OK;
}
