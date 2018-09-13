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
 * include/sys/wait.h
 *
 *   Copyright (C) 2011, 2013 Gregory Nutt. All rights reserved.
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
///@file sys/wait.h

#ifndef __INCLUDE_SYS_WAIT_H
#define __INCLUDE_SYS_WAIT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <signal.h>

#ifdef CONFIG_SCHED_WAITPID

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* The following are provided for analysis of returned status values.
 * Encoded is as follows as 2 bytes of info(MS) then two bytes of code (LS).
 * Code:
 *   0 - Child has exited, info is the exit code.
 *   Other values - Not implemented
 */

#define WEXITSTATUS(s)  (((s) >> 8) & 0xff)	/* Return exit status */
#define WIFEXITED(s)    (((s) & 0xff) == 0)	/* True: Child exited normally */

#define WIFCONTINUED(s) (false)	/* True: Child has been continued */
#define WIFSIGNALED(s)  (false)	/* True: Child exited due to uncaught signal */
#define WIFSTOPPED(s)   (false)	/* True: Child is currently stopped */
#define WSTOPSIG(s)     (false)	/* Return signal number that caused process to stop */
#define WTERMSIG(s)     (false)	/* Return signal number that caused process to terminate */

/* The following symbolic constants are possible values for the options
 * argument to waitpid() (1) and/or waitid() (2),
 */

#define WCONTINUED      (1 << 0)	/* Status for child that has been continued (1)(2) */
#define WNOHANG         (1 << 1)	/* Do not wait if status not available (1) (2) */
#define WUNTRACED       (1 << 2)	/* Report status of stopped child process (1) */
#define WEXITED         (1 << 3)	/* Wait for processes that have exited (2) */
#define WSTOPPED        (1 << 4)	/* Status for child stopped on signal (2) */
#define WNOWAIT         (1 << 5)	/* Keep the process in a waitable state (2) */

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/**
 * @ingroup SCHED_KERNEL
 * @brief Enumeration for waitid idtype */
enum idtype_e {
	P_PID = 1,
	P_GID = 2,
	P_ALL = 3
};
typedef enum idtype_e idtype_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
/**
 * @ingroup SCHED_KERNEL
 * @brief suspend execution of the calling thread
 * @details @b #include <sys/wait.h> \n
 * SYSTEM CALL API \n
 *      suspend execution of the calling thread until
 *   status information for one of its terminated child processes is
 *   available, or until delivery of a signal whose action is either to
 *   execute a signal-catching function or to terminate the process. If more
 *   than one thread is suspended in wait() or waitpid() awaiting termination
 *   of the same process, exactly one thread will return the process status
 *   at the time of the target process termination. If status information is
 *   available prior to the call to wait(), return will be immediate.
 *
 *   The waitpid() function will behave identically to wait(), if the pid
 *   argument is (pid_t)-1 and the options argument is 0. Otherwise, its
 *   behaviour will be modified by the values of the pid and options arguments.
 * @param[in] The location to return the exit status
 * @return return a value equal to the process ID of the child process for
 *   which status is reported.
 * @since TizenRT v1.0
 */
EXTERN pid_t wait(FAR int *stat_loc);
/**
 * @ingroup SCHED_KERNEL
 * @brief suspend execution of the calling thread
 * @details @b #include <sys/wait.h> \n
 * SYSTEM CALL API \n
 *      The waitid() function suspends the calling thread until one child of
 *   the process containing the calling thread changes state. It records the
 *   current state of a child in the structure pointed to by 'info'. If a
 *   child process changed state prior to the call to waitid(), waitid()
 *   returns immediately. If more than one thread is suspended in wait() or
 *   waitpid() waiting termination of the same process, exactly one thread
 *   will return the process status at the time of the target process
 *   termination
 *
 *   The idtype and id arguments are used to specify which children waitid()
 *   will wait for.
 *
 *     If idtype is P_PID, waitid() will wait for the child with a process
 *     ID equal to (pid_t)id.
 *
 *     If idtype is P_PGID, waitid() will wait for any child with a process
 *     group ID equal to (pid_t)id.
 *
 *     If idtype is P_ALL, waitid() will wait for any children and id is
 *     ignored.
 *
 *   The options argument is used to specify which state changes waitid()
 *   will will wait for. It is formed by OR-ing together one or more of the
 *   following flags:
 *
 *     WEXITED - Wait for processes that have exited.
 *     WSTOPPED - Status will be returned for any child that has stopped
 *       upon receipt of a signal.
 *     WCONTINUED - Status will be returned for any child that was stopped
 *       and has been continued.
 *     WNOHANG - Return immediately if there are no children to wait for.
 *     WNOWAIT - Keep the process whose status is returned in 'info' in a
 *       waitable state. This will not affect the state of the process; the
 *       process may be waited for again after this call completes.
 *
 *   The 'info' argument must point to a siginfo_t structure. If waitid()
 *   returns because a child process was found that satisfied the conditions
 *   indicated by the arguments idtype and options, then the structure pointed
 *   to by 'info' will be filled in by the system with the status of the
 *   process. The si_signo member will always be equal to SIGCHLD.
 *
 * @param[in] specify which children waitid() will wait for
 * @param[in] specify which children waitid() will wait for
 * @param[in] option to specify which state changes waitid() will wait for
 *
 * @return If waitid() returns due to the change of state of one of its children,
 *   0 is returned. Otherwise, -1 is returned and errno is set to indicate
 *   the error.
 *
 *   The waitid() function will fail if:
 *
 *     ECHILD - The calling process has no existing unwaited-for child
 *       processes.
 *     EINTR - The waitid() function was interrupted by a signal.
 *     EINVAL - An invalid value was specified for options, or idtype and id
 *       specify an invalid set of processes.
 * @since TizenRT v1.0
 */
EXTERN int waitid(idtype_t idtype, id_t id, FAR siginfo_t *info, int options);
/**
 * @ingroup SCHED_KERNEL
 * @brief wait for a specific thread to end
 * @details @b #include <sys/wait.h> \n
 * SYSTEM CALL API \n
 *     obtain status information pertaining to one
 *   of the caller's child processes. The waitpid() function will suspend
 *   execution of the calling thread until status information for one of the
 *   terminated child processes of the calling process is available, or until
 *   delivery of a signal whose action is either to execute a signal-catching
 *   function or to terminate the process. If more than one thread is suspended
 *   in waitpid() awaiting termination of the same process, exactly one thread
 *   will return the process status at the time of the target process
 *   termination. If status information is available prior to the call to
 *   waitpid(), return will be immediate.
 *
 *   The pid argument specifies a set of child processes for which status is
 *   requested. The waitpid() function will only return the status of a child
 *   process from this set:
 *
 *   - If pid is equal to (pid_t)-1, status is requested for any child process.
 *     In this respect, waitpid() is then equivalent to wait().
 *   - If pid is greater than 0, it specifies the process ID of a single child
 *     process for which status is requested.
 *   - If pid is 0, status is requested for any child process whose process
 *     group ID is equal to that of the calling process.(Not supported yet)
 *   - If pid is less than (pid_t)-1, status is requested for any child process
 *     whose process group ID is equal to the absolute value of pid.(Not supported yet)
 *
 *   The options argument is constructed from the bitwise-inclusive OR of zero
 *   or more of the following flags, defined in the <sys/wait.h> header:
 *
 *   WCONTINUED - The waitpid() function will report the status of any
 *     continued child process specified by pid whose status has not been
 *     reported since it continued from a job control stop.
 *   WNOHANG - The waitpid() function will not suspend execution of the
 *    calling thread if status is not immediately available for one of the
 *    child processes specified by pid.
 *   WUNTRACED - The status of any child processes specified by pid that are
 *    stopped, and whose status has not yet been reported since they stopped,
 *    will also be reported to the requesting process.
 *
 *   If the calling process has SA_NOCLDWAIT set or has SIGCHLD set to
 *   SIG_IGN, and the process has no unwaited-for children that were
 *   transformed into zombie processes, the calling thread will block until all
 *   of the children of the process containing the calling thread terminate, and
 *   waitpid() will fail and set errno to ECHILD.
 *
 *   If waitpid() returns because the status of a child process is available,
 *   these functions will return a value equal to the process ID of the child
 *   process. In this case, if the value of the argument stat_loc is not a
 *   null pointer, information will be stored in the location pointed to by
 *   stat_loc. The value stored at the location pointed to by stat_loc will be
 *   0 if and only if the status returned is from a terminated child process
 *   that terminated by one of the following means:
 *
 *   1. The process returned 0 from main().
 *   2. The process called _exit() or exit() with a status argument of 0.
 *   3. The process was terminated because the last thread in the process terminated.
 *
 *   Regardless of its value, this information may be interpreted using the
 *   following macros, which are defined in <sys/wait.h> and evaluate to
 *   integral expressions; the stat_val argument is the integer value pointed
 *   to by stat_loc.
 *
 *   WIFEXITED(stat_val) - Evaluates to a non-zero value if status was
 *     returned for a child process that terminated normally.
 *   WEXITSTATUS(stat_val) - If the value of WIFEXITED(stat_val) is non-zero,
 *     this macro evaluates to the low-order 8 bits of the status argument
 *     that the child process passed to _exit() or exit(), or the value the
 *     child process returned from main().
 *   WIFSIGNALED(stat_val) - Evaluates to a non-zero value if status was
 *     returned for a child process that terminated due to the receipt of a
 *     signal that was not caught (see <signal.h>).
 *   WTERMSIG(stat_val)  - If the value of WIFSIGNALED(stat_val) is non-zero,
 *     this macro evaluates to the number of the signal that caused the
 *     termination of the child process.
 *   WIFSTOPPED(stat_val) - Evaluates to a non-zero value if status was
 *     returned for a child process that is currently stopped.
 *   WSTOPSIG(stat_val) - If the value of WIFSTOPPED(stat_val) is non-zero,
 *     this macro evaluates to the number of the signal that caused the child
 *     process to stop.
 *   WIFCONTINUED(stat_val) - Evaluates to a non-zero value if status was
 *    returned for a child process that has continued from a job control stop.
 * @param[in] The task ID of the thread to waid for
 * @param[in] The location to return the exit status
 * @param[in] ignored
 * @return If waitpid() returns because the status of a child process is available,
 *   it will return a value equal to the process ID of the child process for
 *   which status is reported.
 *
 *   If waitpid() returns due to the delivery of a signal to the calling
 *   process, -1 will be returned and errno set to EINTR.
 *
 *   If waitpid() was invoked with WNOHANG set in options, it has at least
 *   one child process specified by pid for which status is not available, and
 *   status is not available for any process specified by pid, 0 is returned.
 *
 *   Otherwise, (pid_t)-1 will be returned, and errno set to indicate the error:
 *
 *   ECHILD - The process specified by pid does not exist or is not a child of
 *     the calling process, or the process group specified by pid does not exist
 *     or does not have any member process that is a child of the calling process.
 *   EINTR - The function was interrupted by a signal. The value of the location
 *     pointed to by stat_loc is undefined.
 *   EINVAL - The options argument is not valid.
 * @since TizenRT v1.0
 */
EXTERN pid_t waitpid(pid_t pid, FAR int *stat_loc, int options);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* CONFIG_SCHED_WAITPID */
#endif							/* __INCLUDE_SYS_WAIT_H */
