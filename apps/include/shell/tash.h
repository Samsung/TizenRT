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
///@cond
/**
 * @defgroup TASH SHELL
 * @brief Provides APIs for shell (TASH - TinyAra SHell)
 */

/// @file  tash.h
/// @brief Shell APIs to register commands and to get command informations

#ifndef __APPS_INCLUDE_SHELL_TASH_H
#define __APPS_INCLUDE_SHELL_TASH_H

#include <tinyara/config.h>

/**
 * @brief   Definitions of command execution types
 * @details @b #include <apps/shell/tash.h> \n
 *          The type, SYNC (0) is running command in TASH task.
 *                   ASYNC (1) is running command in seperated thread.
 */
#define TASH_EXECMD_SYNC  (0)
#define TASH_EXECMD_ASYNC (1)

/**
 * @brief tash command's callback function prototype
 * @details @b #include <apps/shell/tash.h>
 */
typedef int (*TASH_CMD_CALLBACK)(int argc, char **args);

/**
 * @brief   Structure of TASH command list
 * @details @b #include <apps/shell/tash.h> \n
 *          To register TASH command, these elements are needed.
 */
struct tash_cmdlist_s {
	const char *name;
	const TASH_CMD_CALLBACK entry;
	const int exectype;
};
typedef struct tash_cmdlist_s tash_cmdlist_t;

/**
 * @brief   API to install a TASH command
 * @details @b #include <apps/shell/tash.h>
 * @param[in] str          Command string(Example: ps, ls, ifconfig etc)
 * @param[in] cb           Callback function for the command
 * @param[in] thread_exec  TASH_EXECMD_ASYNC-execute callback as a thread
						   TASH_EXECMD_SYNC -invoked callback directly
 * @retval 0 Success
 * @retval -ve Failure
 */
int tash_cmd_install(const char *str, TASH_CMD_CALLBACK cb, int thread_exec);

/**
 * @brief   API to register multiple TASH commands at once
 * @details @b #include <apps/shell/tash.h> \n
 *          The difference with tash_cmd_install is supporting command list.
 *          When you want to register several commands at once, you can use this.
 * @param[in] list[] tash command list structure which has command name, callback fnc, and attribute
 */
void tash_cmdlist_install(const tash_cmdlist_t list[]);

/**
 * @internal
 */
/**
 * @brief   API to start TASH
 * @details @b #include <apps/shell/tash.h>
 * @return  On success, 0 is returned. On failure, negative value is returned.
 */
int tash_start(void);

#if defined(CONFIG_TASH_COMMAND_INTERFACE)
/**
 * @brief   API to get the number of registered tash commands.
 * @details @b #include <apps/shell/tash.h> \n
 *          In protected build, it returns the count of user context commands only.
 * @return  On success, positive value is returned. On failure, negative value is returned.
 */
int tash_get_cmdscount(void);

/**
 * @brief   API to get registered tash commands one at a time
 * @details @b #include <apps/shell/tash.h>
 * @param[in] str   Pointer to get the command string with space for 32 bytes
 * @param[in] cb    Callback function pointer
 * @param[in] index Tash command index
 * @return  On success, 0 is returned. On failure, -1 is returned.
 *
 * Example Usage:
 * @code
 *	count = tash_get_cmdscount();
 *	for (i=0; i<count; i++)
 *		tash_get_cmdpair(str, &cb, i );
 * @endcode
 */
int tash_get_cmdpair(char *str, TASH_CMD_CALLBACK *cb, int index);
#endif							/*CONFIG_TASH_COMMAND_INTERFACE */

#endif /*__APPS_INCLUDE_SHELL_TASH_H*/
///@endcond
