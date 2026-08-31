/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * apps/examples/ltp/config.h
 *
 * Feature detection macros for LTP on TizenRT.
 *
 * Adapted from NuttX apps/testing/ltp/config.h for TizenRT.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");

 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __APPS_EXAMPLES_LTP_CONFIG_H
#define __APPS_EXAMPLES_LTP_CONFIG_H

/* Standard headers needed by many LTP test support files (e.g. testfrmw.c)
 * that omit them. Since config.h is force-included via -include, adding
 * them here ensures all test files have access to pthread_mutex_t,
 * PTHREAD_MUTEX_INITIALIZER, va_list, and printf/vprintf.
 */
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>

/* TizenRT does not define SEM_FAILED. sem_open() returns ERROR (-1) on failure. */
#ifndef SEM_FAILED
#define SEM_FAILED ((sem_t *) -1)
#endif

/* TizenRT's signal.h does not define sig_atomic_t (standard C type for
 * signal-safe atomic variables). Use int as the base type.
 */
#ifndef _SIG_ATOMIC_T
#define _SIG_ATOMIC_T
typedef int sig_atomic_t;
#endif


/* TizenRT does not implement sysconf(). All tests calling sysconf() are
 * blacklisted in the Makefile and ltp_register.sh (BLACKWORDS += "sysconf").
 * These _SC_* constants are defined here for completeness — some test files
 * may reference them as preprocessor constants or in comparisons without
 * calling sysconf(). Keeping them prevents unexpected compilation errors.
 * Values match NuttX's nuttx/include/unistd.h definitions.
 */

#ifndef _SC_THREAD_PROCESS_SHARED
#define _SC_THREAD_PROCESS_SHARED 0x0055
#endif
#ifndef _SC_SEM_NSEMS_MAX
#define _SC_SEM_NSEMS_MAX        0x0040
#endif
#ifndef _SC_SEM_VALUE_MAX
#define _SC_SEM_VALUE_MAX        0x0041
#endif
#ifndef _SC_SEMAPHORES
#define _SC_SEMAPHORES            0x0042
#endif
#ifndef _SC_THREADS
#define _SC_THREADS               0x005a
#endif
#ifndef _SC_THREAD_KEYS_MAX
#define _SC_THREAD_KEYS_MAX       0x0051
#endif
#ifndef _SC_THREAD_STACK_MIN
#define _SC_THREAD_STACK_MIN      0x0058
#endif
#ifndef _SC_THREAD_THREADS_MAX
#define _SC_THREAD_THREADS_MAX    0x0059
#endif
#ifndef _SC_OPEN_MAX
#define _SC_OPEN_MAX              0x0035
#endif
#ifndef _SC_PAGESIZE
#define _SC_PAGESIZE              0x0036
#endif
#ifndef _SC_PAGE_SIZE
#define _SC_PAGE_SIZE             _SC_PAGESIZE
#endif
#ifndef _SC_MONOTONIC_CLOCK
#define _SC_MONOTONIC_CLOCK       0x0031
#endif
#ifndef _SC_CLOCK_SELECTION
#define _SC_CLOCK_SELECTION       0x0030
#endif
#ifndef _SC_REALTIME_SIGNALS
#define _SC_REALTIME_SIGNALS      0x003c
#endif

/* TizenRT does not define STDOUT_FILENO (POSIX standard file descriptor for stdout) */
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif






/* TizenRT does not define SIGTERM in its signal.h. It uses non-standard
 * signal numbers (SIGUSR1=1, SIGUSR2=2, SIGALRM=3, SIGKILL=9, etc.).
 * Define SIGTERM as SIGKILL so that LTP tests using it can compile.
 */
#ifndef SIGTERM
#define SIGTERM SIGKILL
#endif

/* TizenRT does not define all sigaction flags. Define the missing ones
 * as 0 (no-op) so that LTP sigaction tests can compile. TizenRT's signal
 * implementation is simplified and does not support these features.
 */
#ifndef SA_RESTART
#define SA_RESTART 0
#endif
#ifndef SA_NODEFER
#define SA_NODEFER 0
#endif
#ifndef SA_RESETHAND
#define SA_RESETHAND 0
#endif
#ifndef SA_ONSTACK
#define SA_ONSTACK 0
#endif

/* TizenRT only defines a small subset of POSIX signals (SIGUSR1, SIGUSR2,
 * SIGALRM, SIGCHLD, SIGKILL). Define the missing standard POSIX signals
 * with their conventional values so that LTP signal tests can compile.
 * Note: These signals may not be deliverable at runtime on TizenRT.
 */
#ifndef SIGHUP
#define SIGHUP 1
#endif
#ifndef SIGINT
#define SIGINT 2
#endif
#ifndef SIGQUIT
#define SIGQUIT 3
#endif
#ifndef SIGILL
#define SIGILL 4
#endif
#ifndef SIGABRT
#define SIGABRT 6
#endif
#ifndef SIGFPE
#define SIGFPE 8
#endif
#ifndef SIGSEGV
#define SIGSEGV 11
#endif
#ifndef SIGPIPE
#define SIGPIPE 13
#endif
#ifndef SIGBUS
#define SIGBUS 7
#endif
#ifndef SIGTRAP
#define SIGTRAP 5
#endif
#ifndef SIGURG
#define SIGURG 23
#endif
#ifndef SIGTSTP
#define SIGTSTP 20
#endif
#ifndef SIGCONT
#define SIGCONT 18
#endif
#ifndef SIGSTOP
#define SIGSTOP 17
#endif
#ifndef SIGTTIN
#define SIGTTIN 21
#endif
#ifndef SIGTTOU
#define SIGTTOU 22
#endif
#ifndef SIGIO
#define SIGIO 29
#endif
#ifndef SIGXCPU
#define SIGXCPU 24
#endif
#ifndef SIGXFSZ
#define SIGXFSZ 25
#endif
#ifndef SIGVTALRM
#define SIGVTALRM 26
#endif
#ifndef SIGPROF
#define SIGPROF 27
#endif
#ifndef SIGWINCH
#define SIGWINCH 28
#endif
#ifndef SIGSYS
#define SIGSYS 31
#endif

/* TizenRT does not define SIGEV_THREAD. Define it as a no-op value. */
#ifndef SIGEV_THREAD
#define SIGEV_THREAD 2
#endif

/* TizenRT does not define _NSIG. Use 32 (max signal number + 1). */
#ifndef _NSIG
#define _NSIG 32
#endif
#ifndef NSIG
#define NSIG _NSIG
#endif

/* Define to 1 if you have __atomic_* compiler builtins */

#define HAVE_ATOMIC_MEMORY_MODEL 1


/* Define to 1 if you have __builtin___clear_cache */
#define HAVE_BUILTIN_CLEAR_CACHE 1

/* Define to 1 if you have the `daemon' function. */
/* #undef HAVE_DAEMON */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if `sa_sigaction' is a member of `struct sigaction'. */
#define HAVE_STRUCT_SIGACTION_SA_SIGACTION 1

/* Define to 1 if the system has the type `struct iovec'. */
#define HAVE_STRUCT_IOVEC 1

/* Define to 1 if you have __sync_add_and_fetch */
#define HAVE_SYNC_ADD_AND_FETCH 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vfork' function. */
/* #undef HAVE_VFORK */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Target is running without an MMU */
#define UCLINUX 1

/* Name of package */
#define PACKAGE "ltp"

/* Define to the full name of this package. */
#define PACKAGE_NAME "ltp"

/* Define to the version of this package. */
#define PACKAGE_VERSION "20230516"

/* Version number of package */
#define VERSION "20230516"

#endif /* __APPS_EXAMPLES_LTP_CONFIG_H */
