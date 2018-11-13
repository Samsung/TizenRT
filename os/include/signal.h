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
/********************************************************************************
 * include/signal.h
 *
 *   Copyright (C) 2007-2009, 2011, 2013-2017 Gregory Nutt. All rights reserved.
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
 ********************************************************************************/
/**
 * @defgroup SIGNAL_KERNEL SIGNAL
 * @brief Provides APIs for Signal
 * @ingroup KERNEL
 *
 * @{
 */

/// @file signal.h
/// @brief Signal APIs

#ifndef __INCLUDE_SIGNAL_H
#define __INCLUDE_SIGNAL_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <stdint.h>
#include <time.h>

/********************************************************************************
 * Pre-processor Definitions
 ********************************************************************************/

/* Signal set management definitions and macros. */

#define NULL_SIGNAL_SET ((sigset_t)0x00000000)
#define ALL_SIGNAL_SET  ((sigset_t)0xffffffff)
#define MIN_SIGNO       0
#define MAX_SIGNO       31
#define GOOD_SIGNO(s)   ((((unsigned)(s)) <= MAX_SIGNO))
#define SIGNO2SET(s)    ((sigset_t)1 << (s))

/* All signals are "real time" signals */

#define SIGRTMIN        MIN_SIGNO	/* First real time signal */
#define SIGRTMAX        MAX_SIGNO	/* Last real time signal */

/* A few of the real time signals are used within the OS.  They have
 * default values that can be overridden from the configuration file. The
 * rest are all user signals.
 *
 * The signal number zero is wasted for the most part.  It is a valid
 * signal number, but has special meaning at many interfaces (e.g., Kill()).
 *
 * These are the semi-standard signal definitions:
 */

#ifndef CONFIG_SIG_SIGUSR1
#define SIGUSR1       1			/* User signal 1 */
#else
#define SIGUSR1       CONFIG_SIG_SIGUSR1
#endif

#ifndef CONFIG_SIG_SIGUSR2
#define SIGUSR2       2			/* User signal 2 */
#else
#define SIGUSR2       CONFIG_SIG_SIGUSR2
#endif

#ifndef CONFIG_SIG_SIGALARM
#define SIGALRM       3			/* Default signal used with POSIX timers (used only */
/* no other signal is provided) */
#else
#define SIGALRM       CONFIG_SIG_SIGALARM
#endif

#ifdef CONFIG_SCHED_HAVE_PARENT
#ifndef CONFIG_SIG_SIGCHLD
#define SIGCHLD     4			/* Used by child threads to signal parent thread */
#else
#define SIGCHLD     CONFIG_SIG_SIGCHLD
#endif
#endif

#ifdef CONFIG_FS_AIO
#ifndef CONFIG_SIG_POLL
#define SIGPOLL     5			/* Sent when an asynchronous I/O event occurs */
#else
#define SIGPOLL     CONFIG_SIG_POLL
#endif
#endif

#ifndef CONFIG_SIG_SIGKILL
#define SIGKILL       9			/* Sent to cause process to terminate */
#else
#define SIGKILL       CONFIG_SIG_SIGKILL
#endif

/* The following are non-standard signal definitions */

#ifndef CONFIG_DISABLE_PTHREAD
#ifndef CONFIG_SIG_SIGCONDTIMEDOUT
#define SIGCONDTIMEDOUT 16		/* Used in the implementation of pthread_cond_timedwait */
#else
#define SIGCONDTIMEDOUT CONFIG_SIG_SIGCONDTIMEDOUT
#endif
#endif

/* SIGWORK is used to wake up various internal, TinyAra worker thread */

#if defined(CONFIG_SCHED_WORKQUEUE) || defined(CONFIG_PAGING)
#ifndef CONFIG_SIG_SIGWORK
#define SIGWORK     17			/* Used to wake up the work queue */
#else
#define SIGWORK     CONFIG_SIG_SIGWORK
#endif
#endif

/* SIGTM is used in Task Manager */
#ifndef CONFIG_SIG_SIGTM_UNICAST
#define SIGTM_UNICAST       18			/* Taskmgt signal */
#else
#define SIGTM_UNICAST       CONFIG_SIG_SIGTM_UNICAST
#endif

/* SIGTM_PAUSE is used in Task Manager */
#ifndef CONFIG_SIG_SIGTM_PAUSE
#define SIGTM_PAUSE       19			/* Taskmgt signal */
#else
#define SIGTM_PAUSE       CONFIG_SIG_SIGTM_PAUSE
#endif

/* SIGTM_RESUME is used in Task Manager */
#ifndef CONFIG_SIG_SIGTM_RESUME
#define SIGTM_RESUME       20			/* Taskmgt signal */
#else
#define SIGTM_RESUME       CONFIG_SIG_SIGTM_RESUME
#endif

/* SIGTM_BROADCAST is used in Task Manager */
#ifndef CONFIG_SIG_SIGTM_BROADCAST
#define SIGTM_BROADCAST       21			/* Taskmgt signal */
#else
#define SIGTM_BROADCAST       CONFIG_SIG_SIGTM_BROADCAST
#endif

/* SIG_SIGEL_WAKEUP is used in Event Loop */
#ifndef CONFIG_SIG_SIGEL_WAKEUP
#define SIGEL_WAKEUP       22			/* Eventloop signal */
#else
#define SIGEL_WAKEUP       CONFIG_SIG_SIGEL_WAKEUP
#endif

/* SIG_SIGTM_TERMINATION is used in Task Manager */
#ifndef CONFIG_SIG_SIGTM_TERMINATION
#define SIGTM_TERMINATION       23			/* Taskmgt signal */
#else
#define SIGTM_TERMINATION       CONFIG_SIG_SIGTM_TERMINATION
#endif

/* SIG_SIGEL_EVENT is used for event handling in Event Loop */
#ifndef CONFIG_SIG_SIGEL_EVENT
#define SIGEL_EVENT       24			/* Eventloop signal */
#else
#define SIGEL_EVENT       CONFIG_SIG_SIGEL_EVENT
#endif

/* sigprocmask() "how" definitions. Only one of the following can be specified: */

#define SIG_BLOCK       1		/* Block the given signals */
#define SIG_UNBLOCK     2		/* Unblock the given signals */
#define SIG_SETMASK     3		/* Set the signal mask to the current set */

/* struct sigaction flag values */

#define SA_NOCLDSTOP    (1 << 0)	/* Do not generate SIGCHILD when
									 * children stop (ignored) */
#define SA_SIGINFO      (1 << 1)	/* Invoke the signal-catching function
									 * with 3 args instead of 1
									 * (always assumed) */
#define SA_NOCLDWAIT    (1 << 2)	/* If signo=SIGCHLD, exit status of child
									 * processes will be discarded */

/* These are the possible values of the signfo si_code field */

#define SI_USER         0		/* Signal sent from kill, raise, or abort */
#define SI_QUEUE        1		/* Signal sent from sigqueue */
#define SI_TIMER        2		/* Signal is result of timer expiration */
#define SI_ASYNCIO      3		/* Signal is the result of asynch IO completion */
#define SI_MESGQ        4		/* Signal generated by arrival of a message on an */
/* empty message queue */
#define CLD_EXITED      5		/* Child has exited (SIGCHLD only) */
#define CLD_KILLED      6		/* Child was killed (SIGCHLD only) */
#define CLD_DUMPED      7		/* Child terminated abnormally (SIGCHLD only) */
#define CLD_TRAPPED     8		/* Traced child has trapped (SIGCHLD only) */
#define CLD_STOPPED     9		/* Child has stopped (SIGCHLD only) */
#define CLD_CONTINUED   10		/* Stopped child had continued (SIGCHLD only) */

/* Values for the sigev_notify field of struct sigevent */

#define SIGEV_NONE      0		/* No notification desired */
#define SIGEV_SIGNAL    1		/* Notify via signal */

/* Special values of sigaction (all treated like NULL) */

#define SIG_ERR         ((_sa_handler_t)-1)
#define SIG_DFL         ((_sa_handler_t)0)
#define SIG_IGN         ((_sa_handler_t)0)
#define SIG_HOLD        ((_sa_handler_t)1)   /* Used only with sigset() */

#define COPY_SIGACTION(t, f) \
	{ \
		(t)->sa_sigaction = (f)->sa_sigaction; \
		(t)->sa_mask      = (f)->sa_mask; \
		(t)->sa_flags     = (f)->sa_flags; \
	}

/********************************************************************************
 * Global Type Declarations
 ********************************************************************************/

/* This defines a set of 32 signals (numbered 0 through 31). */

typedef uint32_t sigset_t;		/* Bit set of 32 signals */

/**
 * @ingroup SIGNAL_KERNEL
 * @brief Union for defining the types of the siginfo si_value field
 */
union sigval {
	int sival_int;				/* Integer value */
	FAR void *sival_ptr;		/* Pointer value */
};

/**
 * @ingroup SIGNAL_KERNEL
 * @brief Structure for elements that define a queue signal. The following is
 * used to attach a signal to a message queue to notify a task when a message is
 * available on a queue
 */
struct sigevent {
	uint8_t sigev_notify;		/* Notification method: SIGEV_SIGNAL or SIGEV_NONE */
	uint8_t sigev_signo;		/* Notification signal */
	union sigval sigev_value;	/* Data passed with notification */
};

/**
 * @ingroup SIGNAL_KERNEL
 * @brief Structure for using to pass parameters to/from signal handlers
 */
struct siginfo {
	uint8_t si_signo;			/* Identifies signal */
	uint8_t si_code;			/* Source: SI_USER, SI_QUEUE, SI_TIMER, SI_ASYNCIO, or SI_MESGQ */
	union sigval si_value;		/* Data passed with signal */
#ifdef CONFIG_SCHED_HAVE_PARENT
	pid_t si_pid;				/* Sending task ID */
	int si_status;				/* Exit value or signal (SIGCHLD only). */
#endif
};

typedef struct siginfo siginfo_t;

/* Non-standard convenience definition of signal handling function types.
 * These should be used only internally within the TinyAra signal logic.
 */

typedef CODE void (*_sa_handler_t)(int);
typedef CODE void (*_sa_sigaction_t)(int, FAR siginfo_t *, FAR void *);

/**
 * @ingroup SIGNAL_KERNEL
 * @brief Structure for defining the action to take for given signal
 */
struct sigaction {
	union {
		_sa_handler_t _sa_handler;
		_sa_sigaction_t _sa_sigaction;
	} sa_u;
	sigset_t sa_mask;
	int sa_flags;
};

/* Definitions that adjust the non-standard naming */

#define sa_handler   sa_u._sa_handler
#define sa_sigaction sa_u._sa_sigaction

/**
 * @}
 */
/********************************************************************************
 * Global Variables
 ********************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 ********************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
/**
 * @ingroup SIGNAL_KERNEL
 * @brief send a signal to a process or a group of processes
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int kill(pid_t pid, int sig);

/**
 * @cond
 * @internal
 * @brief write signal information to standard error
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.1 PRE
 */
void psignal(int signum, FAR const char *message);
/**
 * @endcond
 */

/**
 * @cond
 * @internal
 * @brief write signal information to standard error
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.1 PRE
 */
void psiginfo(const siginfo_t *pinfo, const char *message);
/**
 * @endcond
 */

/* Pthread signal management APIs */
/**
 * @ingroup SIGNAL_KERNEL
 * @brief send a signal to a pthread
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int pthread_kill(pthread_t thread, int sig);

/**
 * @ingroup SIGNAL_KERNEL
 * @brief examine and change blocked signals
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int pthread_sigmask(int how, FAR const sigset_t *set, FAR sigset_t *oset);

/**
 * @addtogroup SIGNAL_KERNEL
 * @{
 */
/**
 * @brief initialize and empty a signal set
 * @details @b #include <signal.h> \n
 * POSIX APIs (refer to : http://pubs.pengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigemptyset(FAR sigset_t *set);
/**
 * @brief initialize and fill a signal set
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigfillset(FAR sigset_t *set);
/**
 * @brief add a signal to a signal set
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigaddset(FAR sigset_t *set, int signo);
/**
 * @brief delete a signal from a signal set
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigdelset(FAR sigset_t *set, int signo);
/**
 * @brief test for a signal in a signal set
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigismember(FAR const sigset_t *set, int signo);
/**
 * @brief signal management
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sighold(int sig);
/**
 * @brief signal management
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
int sigignore(int sig);
/**
 * @brief signal management
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
int sigpause(int sig);
/**
 * @brief send a signal to the executing process
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
int raise(int sig);
/**
 * @brief signal management
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigrelse(int sig);
/**
 * @brief signal management
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
CODE void (*sigset(int sig, CODE void (*func)(int sig)))(int sig);
/**
 * @brief signal management
 * @details @b #include <signal.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
CODE void (*signal(int sig, CODE void (*func)(int sig)))(int sig);
/**
 * @}
 */

/**
 * @ingroup SIGNAL_KERNEL
 * @brief examine and change a signal action
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigaction(int sig, FAR const struct sigaction *act, FAR struct sigaction *oact);
/**
 * @ingroup SIGNAL_KERNEL
 * @brief examine and change blocked signals
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigprocmask(int how, FAR const sigset_t *set, FAR sigset_t *oset);
/**
 * @ingroup SIGNAL_KERNEL
 * @brief examine pending signals
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigpending(FAR sigset_t *set);
/**
 * @ingroup SIGNAL_KERNEL
 * @brief wait for a signal
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigsuspend(FAR const sigset_t *sigmask);
/**
 * @ingroup SIGNAL_KERNEL
 * @brief wait for queued signals
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigwaitinfo(FAR const sigset_t *set, FAR struct siginfo *value);

/**
 * @cond
 * @internal
 * @ingroup SIGNAL_KERNEL
 * @brief wait for queued signals
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.1 PRE
 */
int sigwait(FAR const sigset_t *set, FAR int *sig);
/**
 * @endcond
 */

/**
 * @ingroup SIGNAL_KERNEL
 * @brief wait for queued signals
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sigtimedwait(FAR const sigset_t *set, FAR struct siginfo *value, FAR const struct timespec *timeout);
/**
 * @ingroup SIGNAL_KERNEL
 * @brief queue a signal to a process
 * @details @b #include <signal.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#ifdef CONFIG_CAN_PASS_STRUCTS
int sigqueue(int pid, int signo, union sigval value);
#else
int sigqueue(int pid, int signo, FAR void *sival_ptr);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_SIGNAL_H */
