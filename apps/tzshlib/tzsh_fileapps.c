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
 * apps/tzshlib/tzsh_fileapps.c
 *
 *   Copyright (C) 2013 Gregory Nutt.  All rights reserved.
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

#ifdef CONFIG_SCHED_WAITPID
#include <sys/ioctl.h>
#include <sys/wait.h>
#endif

#include <stdbool.h>
#include <spawn.h>
#include <errno.h>
#include <string.h>

#include "tzsh.h"
#include "tzsh_console.h"

#ifdef CONFIG_TZSH_FILE_APPS

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_fileapp
 *
 * Description:
 *    Attempt to execute the application task whose name is 'cmd'
 *
 * Returned Value:
 *   <0          If posix_spawn() fails, then the negated errno value
 *               is returned.
 *   -1 (ERROR)  if the application task corresponding to 'cmd' could not
 *               be started (possibly because it doesn not exist).
 *    0 (OK)     if the application task corresponding to 'cmd' was
 *               and successfully started.  If CONFIG_SCHED_WAITPID is
 *               defined, this return value also indicates that the
 *               application returned successful status (EXIT_SUCCESS)
 *    1          If CONFIG_SCHED_WAITPID is defined, then this return value
 *               indicates that the application task was spawned successfully
 *               but returned failure exit status.
 *
 ****************************************************************************/

int tzsh_fileapp(FAR struct tzsh_vtbl_s *vtbl, FAR const char *cmd, FAR char **argv, FAR const char *redirfile, int oflags)
{
	posix_spawn_file_actions_t file_actions;
	posix_spawnattr_t attr;
	pid_t pid;
	int ret;

	/* Initialize the attributes file actions structure */

	ret = posix_spawn_file_actions_init(&file_actions);
	if (ret != 0) {
		/* posix_spawn_file_actions_init returns a positive errno value on
		 * failure.
		 */

		tzsh_output(vtbl, g_fmtcmdfailed, cmd, "posix_spawn_file_actions_init", TZSH_ERRNO_OF(ret));
		goto errout;
	}

	ret = posix_spawnattr_init(&attr);
	if (ret != 0) {
		/* posix_spawnattr_init returns a positive errno value on failure. */

		tzsh_output(vtbl, g_fmtcmdfailed, cmd, "posix_spawnattr_init", TZSH_ERRNO);
		goto errout_with_actions;
	}

	/* Handle re-direction of output */

	if (redirfile) {
		ret = posix_spawn_file_actions_addopen(&file_actions, 1, redirfile, oflags, 0644);
		if (ret != 0) {
			/* posix_spawn_file_actions_addopen returns a positive errno
			 * value on failure.
			 */

			tzsh_output(vtbl, g_fmtcmdfailed, cmd, "posix_spawn_file_actions_addopen", TZSH_ERRNO);
			goto errout_with_attrs;
		}
	}

	/* Lock the scheduler in an attempt to prevent the application from
	 * running until waitpid() has been called.
	 */

	sched_lock();

	/* Execute the program. posix_spawnp returns a positive errno value on
	 * failure.
	 */

	ret = posix_spawnp(&pid, cmd, &file_actions, &attr, &argv[1], NULL);
	if (ret == OK) {
		/* The application was successfully started with pre-emption disabled.
		 * In the simplest cases, the application will not have run because the
		 * the scheduler is locked.  But in the case where I/O was redirected, a
		 * proxy task ran and broke our lock.  As result, the application may
		 * have aso ran if its priority was higher than than the priority of
		 * this thread.
		 *
		 * If the application did not run to completion and if the application
		 * was not backgrounded, then we need to wait here for the application
		 * to exit.  This only works works with the following options:
		 *
		 * - CONFIG_TZSH_DISABLEBG - Do not run commands in background
		 * - CONFIG_SCHED_WAITPID - Required to run external commands in
		 *     foreground
		 */

#ifdef CONFIG_SCHED_WAITPID

		/* CONFIG_SCHED_WAITPID is selected, so we may run the command in
		 * foreground unless we were specifically requested to run the command
		 * in background (and running commands in background is enabled).
		 */

#ifndef CONFIG_TZSH_DISABLEBG
		if (vtbl->np.np_bg == false)
#endif							/* CONFIG_TZSH_DISABLEBG */
		{
			int rc = 0;

			/* Setup up to receive SIGINT if control-C entered.  The return
			 * value is ignored because this console device may not support
			 * SIGINT.
			 */

			(void)ioctl(stdout->fs_fd, TIOCSCTTY, pid);

			/* Wait for the application to exit.  We did lock the scheduler
			 * above, but that does not guarantee that the application did not
			 * already run to completion in the case where I/O was redirected.
			 * Here the scheduler will be unlocked while waitpid is waiting
			 * and if the application has not yet run, it will now be able to
			 * do so.
			 *
			 * NOTE: WUNTRACED does nothing in the default case, but in the
			 * case the where CONFIG_SIG_SIGSTOP_ACTION=y, the file app
			 * may also be stopped.  In that case WUNTRACED will force
			 * waitpid() to return with ECHILD.
			 */

			ret = waitpid(pid, &rc, WUNTRACED);
			if (ret < 0) {
				/* If the child thread does not exist, waitpid() will return
				 * the error ECHLD.  Since we know that the task was successfully
				 * started, this must be one of the cases described above; we
				 * have to assume that the task already exit'ed.  In this case,
				 * we have no idea if the application ran successfully or not
				 * (because NuttX does not retain exit status of child tasks).
				 * Let's assume that is did run successfully.
				 */

				int errcode = errno;
				if (errcode == ECHILD) {
					ret = OK;
				} else {
					tzsh_output(vtbl, g_fmtcmdfailed, cmd, "waitpid", TZSH_ERRNO_OF(errcode));
				}
			}

			/* Waitpid completed the wait successfully */

			else {
				/* We can't return the exact status (tzsh has nowhere to put it)
				 * so just pass back zero/nonzero in a fashion that doesn't look
				 * like an error.
				 */

				ret = (rc == 0) ? OK : 1;

				/* TODO:  Set the environment variable '?' to a string corresponding
				 * to WEXITSTATUS(rc) so that $? will expand to the exit status of
				 * the most recently executed task.
				 */
			}

			(void)ioctl(stdout->fs_fd, TIOCSCTTY, -1);
		}
#ifndef CONFIG_TZSH_DISABLEBG
		else
#endif							/* CONFIG_TZSH_DISABLEBG */
#endif							/* CONFIG_SCHED_WAITPID */

			/* We get here if either:
			 *
			 * - CONFIG_SCHED_WAITPID is not selected meaning that all commands
			 *   have to be run in background, or
			 * - CONFIG_SCHED_WAITPID and CONFIG_TZSH_DISABLEBG are both selected, but the
			 *   user requested to run the command in background.
			 *
			 * NOTE that the case of a) CONFIG_SCHED_WAITPID is not selected and
			 * b) CONFIG_TZSH_DISABLEBG selected cannot be supported.  In that event, all
			 * commands will have to run in background.  The waitpid() API must be
			 * available to support running the command in foreground.
			 */

#if !defined(CONFIG_SCHED_WAITPID) || !defined(CONFIG_TZSH_DISABLEBG)
		{
			struct sched_param param;
			sched_getparam(ret, &param);
			tzsh_output(vtbl, "%s [%d:%d]\n", cmd, ret, param.sched_priority);

			/* Backgrounded commands always 'succeed' as long as we can start
			 * them.
			 */

			ret = OK;
		}
#endif							/* !CONFIG_SCHED_WAITPID || !CONFIG_TZSH_DISABLEBG */
	}

	sched_unlock();

	/* Free attibutes and file actions.  Ignoring return values in the case
	 * of an error.
	 */

errout_with_actions:
	(void)posix_spawn_file_actions_destroy(&file_actions);

errout_with_attrs:
	(void)posix_spawnattr_destroy(&attr);

errout:
	/* Most posix_spawn interfaces return a positive errno value on failure
	 * and do not set the errno variable.
	 */

	if (ret > 0) {
		/* Set the errno value and return -1 */

		set_errno(ret);
		ret = ERROR;
	} else if (ret < 0) {
		/* Return -1 on failure.  errno should have been set. */

		ret = ERROR;
	}

	return ret;
}

#endif							/* CONFIG_TZSH_FILE_APPS */
