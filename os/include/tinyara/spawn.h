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
 * include/tinyara/spawn.h
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
///@file tinyara/spawn.h
///@brief SPAWN APIs

#ifndef __INCLUDE_TINYARA_SPAWN_H
#define __INCLUDE_TINYARA_SPAWN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <spawn.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Type Definitions
 ****************************************************************************/
/* This enumerator identifies a file action */

/**
 * @ingroup SPAWN_LIBC
 * @brief enum of spawn_file_action
 */
enum spawn_file_actions_e {
	SPAWN_FILE_ACTION_NONE = 0,
	SPAWN_FILE_ACTION_CLOSE,
	SPAWN_FILE_ACTION_DUP2,
	SPAWN_FILE_ACTION_OPEN
};

/* posix_spawn_file_actions_addclose(), posix_spawn_file_actions_adddup2(),
 * and posix_spawn_file_actions_addopen() will allocate memory and append
 * a new file action to an instance of posix_spawn_file_actions_t.  The
 * internal representation of these structures are defined below:
 */

/**
 * @ingroup SPAWN_LIBC
 * @brief structure for general file action
 */
struct spawn_general_file_action_s {
	FAR struct spawn_general_file_action_s *flink;	/* Supports a singly linked list */
	enum spawn_file_actions_e action;	/* A member of enum spawn_file_actions_e */
};

/**
 * @ingroup SPAWN_LIBC
 * @brief structure for spawn close file action
 */
struct spawn_close_file_action_s {
	FAR struct spawn_general_file_action_s *flink;	/* Supports a singly linked list */
	enum spawn_file_actions_e action;	/* SPAWN_FILE_ACTION_CLOSE */
	int fd;						/* The file descriptor to close */
};

/**
 * @ingroup SPAWN_LIBC
 * @brief structure for spawn dup2 file action
 */
struct spawn_dup2_file_action_s {
	FAR struct spawn_general_file_action_s *flink;	/* Supports a singly linked list */
	enum spawn_file_actions_e action;	/* SPAWN_FILE_ACTION_DUP2 */
	int fd1;					/* The first file descriptor for dup2() */
	int fd2;					/* The second file descriptor for dup2() */
};

/**
 * @ingroup SPAWN_LIBC
 * @brief structure for spawn open file action
 */
struct spawn_open_file_action_s {
	FAR struct spawn_general_file_action_s *flink;	/* Supports a singly linked list */
	enum spawn_file_actions_e action;	/* SPAWN_FILE_ACTION_OPEN */
	int fd;						/* The file descriptor after opening */
	int oflags;					/* Open flags */
	mode_t mode;				/* File creation mode */
	char path[1];				/* Start of the path to be
								 * opened */
};

#define SIZEOF_OPEN_FILE_ACTION_S(n) \
	(sizeof(struct spawn_open_file_action_s) + (n))

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup SPAWN_LIBC
 * @brief Add the file action to the end for the file action list.
 *
 * @details @b #include <tinyara/spawn.h>
 * @param[in] file_action The head of the file action list.
 * @param[in] entry The file action to be added
 * @return none
 * @since Tizen RT v1.0
 */
void add_file_action(FAR posix_spawn_file_actions_t *file_action, FAR struct spawn_general_file_action_s *entry);

#ifdef __cplusplus
}
#endif
#endif							/* __INCLUDE_TINYARA_SPAWN_H */
