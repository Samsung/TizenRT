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
 * include/unistd.h
 *
 *   Copyright (C) 2007-2009, 2013-2014 Gregory Nutt. All rights reserved.
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
/**
 * @defgroup UNISTD_KERNEL UNISTD
 * @ingroup KERNEL
 *
 * @{
 */

///@file unistd.h
///@brief POSIX operating system APIs

#ifndef __INCLUDE_UNISTD_H
#define __INCLUDE_UNISTD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <tinyara/compiler.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The number of functions that may be registered to be called
 * at program exit.
 */

#define ATEXIT_MAX 1

/* Values for seeking */

#define SEEK_SET    0			/* From the start of the file */
#define SEEK_CUR    1			/* From the current file offset */
#define SEEK_END    2			/* From the end of the file */

/* Bit values for the second argument to access */

#define F_OK        0			/* Test existence */
#define R_OK        1			/* Test read permission */
#define W_OK        2			/* Test write permission */
#define X_OK        4			/* Test execute permission */

/* POSIX feature set macros */

#define  POSIX_VERSION
#undef  _POSIX_SAVED_IDS
#undef  _POSIX_JOB_CONTROL
#define _POSIX_REALTIME_SIGNALS 1
#define _POSIX_MESSAGE_PASSING 1
#undef  _POSIX_MAPPED_FILES
#undef  _POSIX_SHARED_MEMORY_OBJECTS
#define _POSIX_PRIORITY_SCHEDULING 1
#define _POSIX_TIMERS 1
#undef  _POSIX_MEMLOCK
#undef  _POSIX_MEMLOCK_RANGE
#undef  _POSIX_FSYNC
#define _POSIX_SYNCHRONIZED_IO 1
#undef  _POSIX_ASYNCHRONOUS_IO
#undef  _POSIX_PRIORITIZED_IO

/* Execution time constants (not supported) */

#undef  _POSIX_CHOWN_RESTRICTED
#undef  _POSIX_NO_TRUNC
#undef  _POSIX_VDISABLE

#define _POSIX_SYNC_IO 1
#undef  _POSIX_ASYNC_IO
#undef  _POSIX_PRIO_IO

#define fdatasync(f) fsync(f)

/****************************************************************************
 * Global Variables
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Used by getopt (obviously NOT thread safe!).  These variables cannot be
 * accessed directly by an external NXFLAT module.  In that case, accessor
 * functions must be used.
 */

#ifndef __NXFLAT__
EXTERN FAR char *optarg;		/* Optional argument following option */
EXTERN int optind;				/* Index into argv */
EXTERN int optopt;				/* unrecognized option character */
#else
#define optarg  (*(getoptargp()))
#define optind  (*(getoptindp()))
#define optopt  (*(getoptoptp()))
#endif

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

/* Task Control Interfaces */

/**
 * @ingroup UNISTD_KERNEL
 * @brief The vfork() function has the same effect as fork(), except that the behavior is
 *   undefined if the process created by vfork() either modifies any data other than
 *   a variable of type pid_t used to store the return value from vfork(), or returns
 *   from the function in which vfork() was called, or calls any other function before
 *   successfully calling _exit() or one of the exec family of functions.
 *
 * @details This thin layer implements vfork by simply calling up_vfork() with the vfork()
 *   context as an argument.  The overall sequence is:
 *
 *   1) User code calls vfork().  vfork() collects context information and
 *      transfers control up up_vfork().
 *   2) up_vfork()and calls task_vforksetup().
 *   3) task_vforksetup() allocates and configures the child task's TCB.  This
 *      consists of:
 *      - Allocation of the child task's TCB.
 *      - Initialization of file descriptors and streams
 *      - Configuration of environment variables
 *      - Setup the intput parameters for the task.
 *      - Initialization of the TCB (including call to up_initial_state()
 *   4) up_vfork() provides any additional operating context. up_vfork must:
 *      - Allocate and initialize the stack
 *      - Initialize special values in any CPU registers that were not
 *        already configured by up_initial_state()
 *   5) up_vfork() then calls task_vforkstart()
 *   6) task_vforkstart() then executes the child thread.
 *
 * @return Upon successful completion, vfork() returns 0 to the child process and returns
 *   the process ID of the child process to the parent process. Otherwise, -1 is
 *   returned to the parent, no child process is created, and errno is set to
 *   indicate the error.
 * @since Tizen RT v1.0
 */
pid_t vfork(void);
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
pid_t getpid(void);
/**
 * @cond
 * @internal
 */
void _exit(int status) noreturn_function;
/**
 * @endcond
 */
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
unsigned int sleep(unsigned int seconds);
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int usleep(useconds_t usec);
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int pause(void);

/* File descriptor operations */

/**
 * @addtogroup UNISTD_KERNEL
 * @{
 */
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int close(int fd);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int dup(int fd);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int dup2(int fd1, int fd2);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int fsync(int fd);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
off_t lseek(int fd, off_t offset, int whence);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
ssize_t read(int fd, FAR void *buf, size_t nbytes);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
ssize_t write(int fd, FAR const void *buf, size_t nbytes);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
ssize_t pread(int fd, FAR void *buf, size_t nbytes, off_t offset);
/**
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
ssize_t pwrite(int fd, FAR const void *buf, size_t nbytes, off_t offset);

/**
		  * @} *///end for UNISTD_KERNEL
/* Memory management */

#if defined(CONFIG_ARCH_ADDRENV) && defined(CONFIG_MM_PGALLOC) && \
	defined(CONFIG_ARCH_USE_MMU)
/**
 * @cond
 * @internal
 */
FAR void *sbrk(intptr_t incr);
/**
 * @endcond
 */
#endif

/* Special devices */

/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int pipe(int fd[2]);

/* Working directory operations */

/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int chdir(FAR const char *path);
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
FAR char *getcwd(FAR char *buf, size_t size);

/* File path operations */

/**
* @cond
* @internal
*/
int access(FAR const char *path, int amode);
/**
 * @endcond
 */
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int rmdir(FAR const char *pathname);
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @details [SYSTEM CALL API]
 * @since Tizen RT v1.0
 */
int unlink(FAR const char *pathname);

/* Execution of programs from files */

#ifdef CONFIG_LIBC_EXECFUNCS
/**
 * @cond
 * @internal
 * @{
 */
int execl(FAR const char *path, ...);
int execv(FAR const char *path, FAR char *const argv[]);
/**
 * @endcond
 * @} */
#endif

/* Other */
/**
 * @ingroup UNISTD_KERNEL
 * @brief  POSIX APIs (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int getopt(int argc, FAR char *const argv[], FAR const char *optstring);

/* Accessor functions intended for use only by external NXFLAT
 * modules.  The global variables optarg, optind, and optopt cannot
 * be referenced directly from external modules.
 */
/**
 * @cond
 * @internal
 * @{
 */
FAR char **getoptargp(void);	/* Optional argument following option */
int *getoptindp(void);			/* Index into argv */
int *getoptoptp(void);			/* unrecognized option character */
/**
 * @endcond
 * @} */
#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_UNISTD_H */
/**
 * @} */
