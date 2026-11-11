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
 * sched/task/task_prctl.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/prctl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/sched.h>

#include "sched/sched.h"
#include "task/task.h"

#ifdef CONFIG_PREFERENCE
#  include <tinyara/preference.h>
#  include "preference/preference.h"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: prctl
 *
 * Description:
 *   prctl() is called with a first argument describing what to do (with
 *   values PR_* defined above) and with additional arguments depending on
 *   the specific command.
 *
 * Returned Value:
 *   The returned value may depend on the specific command.  For PR_SET_NAME
 *   and PR_GET_NAME, the returned value of 0 indicates successful operation.
 *   On any failure, -1 is retruend and the errno value is set appropriately.
 *
 *     EINVAL The value of 'option' is not recognized.
 *     EFAULT optional arg1 is not a valid address.
 *     ESRCH  No task/thread can be found corresponding to that specified
 *       by optional arg1.
 *
 ****************************************************************************/

int prctl(int option, ...)
{
	va_list ap;
	int errcode;

	va_start(ap, option);
	switch (option) {
	case PR_SET_NAME:
	case PR_GET_NAME:
	case PR_SET_NAME_EXT:
	case PR_GET_NAME_EXT:
#if CONFIG_TASK_NAME_SIZE > 0
	{
		/* Get the prctl arguments */

		FAR char *name = va_arg(ap, FAR char *);
		FAR struct tcb_s *tcb;
		int pid = 0;

		if (option == PR_SET_NAME_EXT ||
		    option == PR_GET_NAME_EXT) {
			pid = va_arg(ap, int);
		}

		/* Get the TCB associated with the PID (handling the special case
		 * of pid==0 meaning "this thread")
		 */

		if (pid == 0) {
			tcb = this_task();
		} else {
			tcb = nxsched_get_tcb(pid);
		}

		/* An invalid pid will be indicated by a NULL TCB returned from
		 * nxsched_get_tcb()
		 */

		if (tcb == NULL) {
			serr("ERROR: Pid does not correspond to a task: %d\n", pid);
			errcode = ESRCH;
			goto errout;
		}

		/* A pointer to the task name storage must also be provided */

		if (name == NULL) {
			serr("ERROR: No name provide\n");
			errcode = EFAULT;
			goto errout;
		}

		/* Now get or set the task name */

		if (option == PR_SET_NAME || option == PR_SET_NAME_EXT) {
			/* Ensure that tcb->name will be null-terminated, truncating if
			 * necessary.
			 */

			strlcpy(tcb->name, name, sizeof(tcb->name));
			tcb->name[CONFIG_TASK_NAME_SIZE] = '\0';
		} else {
			/* The returned value will be null-terminated, truncating if
			 * necessary.
			 */

			strlcpy(name, tcb->name, sizeof(tcb->name));
			name[CONFIG_TASK_NAME_SIZE - 1] = '\0';
		}
	}
	break;
#else
		serr("ERROR: Option not enabled: %d\n", option);
		errcode = ENOSYS;
		goto errout;
#endif

#ifdef CONFIG_PREFERENCE
	case PR_SET_PREFERENCE:
	{
		int ret;
		preference_data_t *data;

		data = va_arg(ap, preference_data_t *);
		ret = preference_write_key(data);
		va_end(ap);
		return ret;
	}

	case PR_GET_PREFERENCE:
	{
		int ret;
		preference_data_t *data;

		data = va_arg(ap, preference_data_t *);
		ret = preference_read_key(data);
		va_end(ap);
		return ret;
	}

	case PR_REMOVE_PREFERENCE:
	{
		int ret;
		int type;
		char *key;

		type = va_arg(ap, int);
		key = va_arg(ap, char *);
		ret = preference_remove_key(type, key);
		va_end(ap);
		return ret;
	}

	case PR_REMOVEALL_PREFERENCE:
	{
		int ret;
		int type;
		char *path;

		type = va_arg(ap, int);
		path = va_arg(ap, char *);
		ret = preference_remove_all_key(type, path);
		va_end(ap);
		return ret;
	}

	case PR_CHECK_PREFERENCE:
	{
		int ret;
		int type;
		char *key;
		bool *result;

		type = va_arg(ap, int);
		key = va_arg(ap, char *);
		result = va_arg(ap, bool *);
		ret = preference_check_key(type, key, result);
		va_end(ap);
		return ret;
	}

	case PR_SET_PREFERENCE_CB:
	{
		int ret;
		preference_callback_t *data;

		data = va_arg(ap, preference_callback_t *);
		ret = preference_register_callback(data);
		va_end(ap);
		return ret;
	}

	case PR_UNSET_PREFERENCE_CB:
	{
		int ret;
		char *key;
		int type;

		type = va_arg(ap, int);
		key = va_arg(ap, char *);
		ret = preference_unregister_callback(key, type);
		va_end(ap);
		return ret;
	}
#endif

	default:
		serr("ERROR: Unrecognized option: %d\n", option);
		errcode = EINVAL;
		goto errout;
	}

	/* Not reachable unless CONFIG_TASK_NAME_SIZE is > 0.  NOTE: This might
	 * change if additional commands are supported.
	 */

#if CONFIG_TASK_NAME_SIZE > 0
	va_end(ap);
	return OK;
#endif

errout:
	va_end(ap);
	set_errno(errcode);
	return ERROR;
}
