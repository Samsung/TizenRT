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

#include <tinyara/kernel_test_drv.h>
#include <tinyara/sched.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct tcb_s *tcb;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int test_get_self_pid(unsigned long arg)
{
	tcb = sched_self();
	if (tcb == NULL) {
		return ERROR;
	}
	return tcb->pid;
}

static int test_is_alive_thread(unsigned long arg)
{
	tcb = sched_gettcb((pid_t)arg);
	if (tcb == NULL) {
		dbg("sched_gettcb failed. errno : %d\n", get_errno());
		return ERROR;
	}
	return OK;
}

static int test_get_tcb_adj_stack_size(unsigned long arg)
{
	tcb = sched_gettcb((pid_t)arg);
	if (tcb == NULL) {
		dbg("sched_gettcb failed. errno : %d\n", get_errno());
		return ERROR;
	}
	return tcb->adj_stack_size;
}

static void test_sched_foreach(unsigned long arg)
{
	sched_foreach((void *)arg, NULL);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_sched(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
	case TESTIOC_GET_SELF_PID:
		ret = test_get_self_pid(arg);
		break;
	case TESTIOC_IS_ALIVE_THREAD:
		ret = test_is_alive_thread(arg);
		break;
	case TESTIOC_GET_TCB_ADJ_STACK_SIZE:
		ret = test_get_tcb_adj_stack_size(arg);
		break;
	case TESTIOC_SCHED_FOREACH:
		test_sched_foreach(arg);
		break;
	}
	return ret;
}
