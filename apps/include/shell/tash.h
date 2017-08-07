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
#ifndef __APPS_INCLUDE_SHELL_TASH_H
#define __APPS_INCLUDE_SHELL_TASH_H

#include <tinyara/config.h>

#define TASH_EXECMD_SYNC  (0)
#define TASH_EXECMD_ASYNC (1)

/* Typedef for tash command's callback
 * @retval 0 success
 **/
typedef int (*TASH_CMD_CALLBACK)(int argc, char **args);

typedef struct {
	const char *str;
	const TASH_CMD_CALLBACK cb;
	const int thread_exec;
} tash_cmdlist_t;

struct tash_taskinfo_s {
	const char *str;
	const int task_prio;
	const int task_stacksize;
};

typedef struct tash_taskinfo_s tash_taskinfo_t;

/** @name tash_cmd_install
 * @brief API to install TASH shell commands
 * @ingroup tash
 * @param[in] str - command's first string(Example: ps, ls, ifconfig etc)
 * @param[in] cb - callback function for the command
 * @param[in] thread_exec: TASH_EXECMD_ASYNC-execute callback as a thread
						   TASH_EXECMD_SYNC -invoked callback directly
 * @retval 0 Success
 * @retval -ve Failure
 *
 * Example Usage:
 * @code
 *
 * @endcode
 **/
int tash_cmd_install(const char *str, TASH_CMD_CALLBACK cb, int thread_exec);

/** @name tash_cmdlist_install
 * @brief API to register TASH shell command list
 * @ingroup tash
 * @param[in] list[] - tash command list structure which has command name, callback fnc, and attribute
 *
 * Example Usage:
 * @code
 *
 * @endcode
 **/
void tash_cmdlist_install(const tash_cmdlist_t list[]);

/** @name tash_start
 * @brief API to start TASH
 * @ingroup tash
 * @retval 0 Success
 * @retval -ve Failure
 *
 * Example Usage:
 * @code
 *
 * @endcode
 **/
int tash_start(void);

#if defined(CONFIG_TASH_COMMAND_INTERFACE)
/** @name tash_get_cmdscount
 * @brief API to get the number of registered tash commands.
 * In protected build, it returns the count of user context commands only.
 * @ingroup tash
 * @ret +ve - Tash commands count
 *     -ve - Error
 *
 * Example Usage:
 * @code
 *
 * @endcode
 **/
int tash_get_cmdscount(void);

/** @name tash_get_cmdpair
 * @brief API to get registered tash commands one at a time
 * @ingroup tash
 * @param[in] str-pointer to get the command string with space for 32 bytes
 * @param[in] cb- Callback function pointer
 * @param[in] index - Tash command index

 * @ret 0 - Successful
 *     -1 - Error
 *
 * Example Usage:
 * @code
	count = tash_get_cmdscount();
	for (i=0; i<count; i++)
		tash_get_cmdpair(str, &cb, i );
 * @endcode
 **/
int tash_get_cmdpair(char *str, TASH_CMD_CALLBACK *cb, int index);
#endif							/*CONFIG_TASH_COMMAND_INTERFACE */

#endif /*__APPS_INCLUDE_SHELL_TASH_H*/
