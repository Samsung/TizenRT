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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <errno.h>
#include <debug.h>
#include <unistd.h>

#include <tinyara/kernel_test_drv.h>
#include <tinyara/sched.h>

#include "signal/signal.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int test_get_sig_findaction_add(unsigned long arg)
{
	FAR sigactq_t *sigact;
	sigact = sig_findaction(sched_self(), (int)arg);
	return (int)sigact;
}

static int test_signal_pause(unsigned long arg)
{
	int ret;
	ret = pause();              /* pause() always return -1 */
	if (ret == ERROR && get_errno() == EINTR) {
		return OK;
	}
	return ERROR;
}

static int test_get_tcb_sigprocmask(unsigned long arg)
{
	struct tcb_s *tcb;
	tcb = sched_gettcb((pid_t)arg);
	if (tcb == NULL) {
		dbg("sched_gettcb failed. errno : %d\n", get_errno());
		return ERROR;
	}
	return tcb->sigprocmask;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_signal(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
	case TESTIOC_GET_SIG_FINDACTION_ADD:
		ret = test_get_sig_findaction_add(arg);
		break;
	case TESTIOC_SIGNAL_PAUSE:
		ret = test_signal_pause(arg);
		break;
	case TESTIOC_GET_TCB_SIGPROCMASK:
		ret = test_get_tcb_sigprocmask(arg);
		break;
	}
	return ret;
}
