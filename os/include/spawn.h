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
 * include/spawn.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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
 * @defgroup SPAWN_LIBC SPAWN
 * @brief Provides APIs for Spawn
 * @ingroup KERNEL
 *
 * @{
 */

/// @file spawn.h
/// @brief Spawn APIs

#ifndef __INCLUDE_SPAWN_H
#define __INCLUDE_SPAWN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <sched.h>
#include <signal.h>
#include <errno.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_TASK_SPAWN_DEFAULT_STACKSIZE
#define CONFIG_TASK_SPAWN_DEFAULT_STACKSIZE 2048
#endif

/* "The spawn.h header shall define the flags that may be set in a
 * posix_spawnattr_t object using the posix_spawnattr_setflags() function:"
 */

#define POSIX_SPAWN_RESETIDS      (1 << 0)	/* 1: Reset effective user ID */
#define POSIX_SPAWN_SETPGROUP     (1 << 1)	/* 1: Set process group */
#define POSIX_SPAWN_SETSCHEDPARAM (1 << 2)	/* 1: Set task's priority */
#define POSIX_SPAWN_SETSCHEDULER  (1 << 3)	/* 1: Set task's scheduler policy */
#define POSIX_SPAWN_SETSIGDEF     (1 << 4)	/* 1: Set default signal actions */
#define POSIX_SPAWN_SETSIGMASK    (1 << 5)	/* 1: Set sigmask */

/****************************************************************************
 * Type Definitions
 ****************************************************************************/
/* "The spawn.h header shall define the posix_spawnattr_t and
 * posix_spawn_file_actions_t types used in performing spawn operations.
 *
 * The internal structure underlying the posix_spawnattr_t is exposed here
 * because the user will be required to allocate this memory.
 */
/**
 * @brief Structure of posix_spawn attributes
 */
struct posix_spawnattr_s {
	/* Used by posix_spawn, posix_spawnp, and task_spawn */

	uint8_t flags;				/* See POSIX_SPAWN_ definitions */
	uint8_t priority;			/* Task scheduling priority */
	uint8_t policy;				/* Task scheduling policy */
#ifndef CONFIG_DISABLE_SIGNALS
	sigset_t sigmask;			/* Signals to be masked */
#endif

#ifndef CONFIG_BUILD_KERNEL
	/* Used only by task_spawn (non-standard) */

	size_t stacksize;			/* Task stack size */
#endif
};

typedef struct posix_spawnattr_s posix_spawnattr_t;

/* posix_spawn_file_actions_addclose(), posix_spawn_file_actions_adddup2(),
 * and posix_spawn_file_actions_addopen() will allocate memory and append
 * a new file action to an instance of posix_spawn_file_actions_t.  The
 * internal representation of these structures is not exposed to the user.
 * The user need only know that the size sizeof(posix_spawn_file_actions_t)
 * will hold a pointer to data.
 */

typedef FAR void *posix_spawn_file_actions_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/* "The following shall be declared as functions and may also be defined as
 * macros. Function prototypes shall be provided."
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Spawn interfaces *********************************************************/
/* Standard posix_spawn[p] interfaces.  These functions execute files in the
 * file system at 'path'
 */

/**
 * @cond
 * @internal
 */
int posix_spawn(FAR pid_t *pid, FAR const char *path, FAR const posix_spawn_file_actions_t *file_actions, FAR const posix_spawnattr_t *attr, FAR char *const argv[], FAR char *const envp[]);
#define posix_spawnp(pid, path, file_actions, attr, argv, envp) \
	posix_spawn(pid, path, file_actions, attr, argv, envp)
/**
 * @endcond
 */
#ifndef CONFIG_BUILD_KERNEL
/* Non-standard task_spawn interface.  This function uses the same
 * semantics to execute a file in memory at 'entry', giving it the name
 * 'name'.
 */
/**
 * @cond
 * @internal
 */
int task_spawn(FAR pid_t *pid, FAR const char *name, main_t entry, FAR const posix_spawn_file_actions_t *file_actions, FAR const posix_spawnattr_t *attr, FAR char *const argv[], FAR char *const envp[]);
/**
 * @endcond
 */
#endif

/* File action interfaces ***************************************************/
/* File action initialization and destruction */
/**
 * @brief initialize spawn file actions object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawn_file_actions_init(FAR posix_spawn_file_actions_t *file_actions);
/**
 * @brief destroy spawn file actions object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawn_file_actions_destroy(FAR posix_spawn_file_actions_t *file_actions);

/* Add file action interfaces */
/**
 * @brief add close or open action to spawn file actions object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawn_file_actions_addclose(FAR posix_spawn_file_actions_t *file_actions, int fd);
/**
 * @brief add dup2 action to spawn file actions object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.1
 */
int posix_spawn_file_actions_adddup2(FAR posix_spawn_file_actions_t *file_actions, int fd1, int fd2);
/**
 * @brief add close or open action to spawn file actions object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawn_file_actions_addopen(FAR posix_spawn_file_actions_t *file_actions, int fd, FAR const char *path, int oflags, mode_t mode);

/* Spawn attributes interfaces **********************************************/
/* Spawn attributes initialization and destruction */
/**
 * @brief initialize spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_init(FAR posix_spawnattr_t *attr);

/* int posix_spawnattr_destroy(FAR posix_spawnattr_t *); */
#ifdef CONFIG_DEBUG
#define posix_spawnattr_destroy(attr) (attr ? 0 : EINVAL)
#else
#define posix_spawnattr_destroy(attr) (0)
#endif

/* Get spawn attributes interfaces */
/**
 * @brief get and set the spawn-flags attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_getflags(FAR const posix_spawnattr_t *attr, FAR short *flags);
#define posix_spawnattr_getpgroup(attr, group) (ENOSYS)
/**
 * @brief get the spawn-schedparam attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_getschedparam(FAR const posix_spawnattr_t *attr, FAR struct sched_param *param);
/**
 * @brief get the spawn-schedpolicy attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_getschedpolicy(FAR const posix_spawnattr_t *attr, FAR int *policy);
#define posix_spawnattr_getsigdefault(attr, sigdefault) (ENOSYS)
#ifndef CONFIG_DISABLE_SIGNALS
/**
 * @brief get the spawn-sigmask attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_getsigmask(FAR const posix_spawnattr_t *attr, FAR sigset_t *sigmask);
#else
#define posix_spawnattr_getsigmask(attr, sigmask) (ENOSYS)
#endif

/* Set spawn attributes interfaces */
/**
 * @brief set the spawn-flags attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_setflags(FAR posix_spawnattr_t *attr, short flags);
#define posix_spawnattr_setpgroup(attr, group) (ENOSYS)
/**
 * @brief set the spawn-schedparam attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_setschedparam(FAR posix_spawnattr_t *attr, FAR const struct sched_param *param);
/**
 * @brief set the spawn-schedpolicy attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_setschedpolicy(FAR posix_spawnattr_t *attr, int policy);
#define posix_spawnattr_setsigdefault(attr, sigdefault) (ENOSYS)
#ifndef CONFIG_DISABLE_SIGNALS
/**
 * @brief set the spawn-sigmask attribute of a spawn attributes object
 * @details @b #include <spawn.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since Tizen RT v1.0
 */
int posix_spawnattr_setsigmask(FAR posix_spawnattr_t *attr, FAR const sigset_t *sigmask);
#else
#define posix_spawnattr_setsigmask(attr, sigmask) (ENOSYS)
#endif

/* Non-standard get/set spawn attributes interfaces for use only with
 * task_spawn()
 */
/**
 * @brief The task_spawnattr_getstacksize() function will obtain the value of
 *   the spawn-stacksize attribute from the attributes object referenced
 *   by attr.
 *
 * @details @b #include <spawn.h>
 * @param[in] attr The address spawn attributes to be queried.
 * @param[in] stacksize The location to return the spawn-stacksize value.
 * @return On success, these functions return 0; on failure they return an errno
 * @since Tizen RT v1.0
 */
int task_spawnattr_getstacksize(FAR const posix_spawnattr_t *attr, size_t *stacksize);
/**
 * @brief The task_spawnattr_setstacksize() function shall set the spawn-
 *   stacksize attribute in an initialized attributes object referenced
 *   by attr.
 *
 * @details @b #include <spawn.h>
 * @param[in] attr The address spawn attributes to be used.
 * @param[in] stacksize The new stacksize to set.
 * @return On success, these functions return 0; on failure they return an errno
 * @since Tizen RT v1.0
 */
int task_spawnattr_setstacksize(FAR posix_spawnattr_t *attr, size_t stacksize);

/* Non standard debug functions */

#ifdef CONFIG_DEBUG
/**
 * @brief Show the entryent file actions.
 * @details @b #include <spawn.h> \n
 * @param[in] file_actions The address of the file_actions to be dumped.
 * @since Tizen RT v1.0
 */
void posix_spawn_file_actions_dump(FAR posix_spawn_file_actions_t *file_actions);
/**
 * @brief Show the current attributes
 * @details @b #include <spawn.h> \n
 * @param[in] attr The address of the spawn attributes to be dumped.
 * @since Tizen RT v1.0
 */
void posix_spawnattr_dump(FAR posix_spawnattr_t *attr);
#else
#define posix_spawn_file_actions_dump(fa)
#define posix_spawnattr_dump(a)
#endif

#ifdef __cplusplus
}
#endif
#endif							/* __INCLUDE_SPAWN_H */
/**
 * @}
 */
