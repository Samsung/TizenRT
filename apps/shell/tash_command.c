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
/// @file   tash_command.c
/// @brief  functions to treate commands

#include <pthread.h>
#include <tinyara/config.h>
#include <string.h>
#include <stdio.h>
#ifndef CONFIG_DISABLE_ENIVRON
#include <stdlib.h>
#endif
#include <sys/types.h>
#include <apps/shell/tash.h>
#include "tash_internal.h"

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

/** Following defines are fixed to avoid many configuration variables
for TASH
*/
#define TASH_CMD_MAXSTRLENGTH    (16) /** example: len of ifconfig is 8 */
#ifdef CONFIG_TASH_MAX_COMMANDS
#define TASH_MAX_COMMANDS        CONFIG_TASH_MAX_COMMANDS
#else
#define TASH_MAX_COMMANDS        (32)
#endif
#ifdef CONFIG_TASH_CMDTHREAD_STACKSIZE
#define TASH_CMDTHREAD_STACKSIZE CONFIG_TASH_CMDTHREAD_STACKSIZE
#else
#define TASH_CMDTHREAD_STACKSIZE (4096)
#endif
#ifdef CONFIG_TASH_CMDTHREAD_PRIORITY
#define TASH_CMDTHREAD_PRIORITY  CONFIG_TASH_CMDTHREAD_PRIORITY
#else
#define TASH_CMDTHREAD_PRIORITY  (SCHED_PRIORITY_DEFAULT)
#endif
#define TASH_CMDS_PER_LINE       (4)
#ifndef CONFIG_DISABLE_ENIVRON
#define TASH_ASYNC_CMD_PRI_STR   "CMD_PRI"
#define TASH_ASYNC_CMD_STACK_STR "CMD_STACK"
#endif

/****************************************************************************
 * Global Variables
 ****************************************************************************/

extern int tash_running;

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/**
 * @brief TASH internal data structure to store command strings & callback function
 * pointers
 */
struct tash_cmd_s {
	char str[TASH_CMD_MAXSTRLENGTH];	/* Command strings- eg, ifconfig */
	TASH_CMD_CALLBACK cb;				/* Function pointer for Callback */
	int exec_type;						/* Execution type of this command */
};

struct tash_cmd_info_s {
	pthread_mutex_t tmutex;						/* Mutex for protection */
	struct tash_cmd_s cmd[TASH_MAX_COMMANDS];
	int count;									/* Number of TASH commands */
};

struct tash_cmd_targs_s {
	TASH_CMD_CALLBACK cb;
	char **args;
	int argc;
};

/********************************************************************************
 * Private Function Prototypes
 ********************************************************************************/

static int tash_help(int argc, char **args);
static int tash_exit(int argc, char **args);

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static int is_sorted_list = FALSE;
static struct tash_cmd_info_s tash_cmds_info = {PTHREAD_MUTEX_INITIALIZER};

const static tash_cmdlist_t tash_basic_cmds[] = {
	{"exit",  tash_exit,   TASH_EXECMD_SYNC},
	{"help",  tash_help,   TASH_EXECMD_SYNC},
	{"sh",    tash_script, TASH_EXECMD_SYNC},
#ifndef CONFIG_DISABLE_SIGNALS
	{"sleep", tash_sleep,  TASH_EXECMD_SYNC},
#endif
	{NULL,    NULL,        0}
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void tash_sort_cmds(void)
{
	int i;
	int j;
	int length = tash_cmds_info.count;
	int gap = tash_cmds_info.count;
	struct tash_cmd_s temp;
	int tash_cmd_size = sizeof(struct tash_cmd_s);

	while (gap > 1) {
		/* gap = max(floor(gap * 5 / 11), 1) */

		gap = (gap * 5 / 11) > 1 ? (gap * 5 / 11) : 1;
		for (i = gap; i < length; i++) {
			memcpy(&temp, &tash_cmds_info.cmd[i], tash_cmd_size);
			for (j = i; j >= gap && (strcmp(temp.str, tash_cmds_info.cmd[j - gap].str) < 0); j -= gap) {
				memcpy(&tash_cmds_info.cmd[j], &tash_cmds_info.cmd[j - gap], tash_cmd_size);
			}
			memcpy(&tash_cmds_info.cmd[j], &temp, tash_cmd_size);
		}
	}
}

/** @brief Help function in TASH to list all available commands
 *  @ingroup tash
 */
static int tash_help(int argc, char **args)
{
	int cmd_idx;

	printf("\t TASH command list \n");
	printf("\t --------------------\n");

	if (is_sorted_list == FALSE) {
		tash_sort_cmds();
		is_sorted_list = TRUE;
	}

	for (cmd_idx = 0; cmd_idx < tash_cmds_info.count; cmd_idx++) {
		printf("%-16s ", tash_cmds_info.cmd[cmd_idx].str);
		if (cmd_idx % TASH_CMDS_PER_LINE == (TASH_CMDS_PER_LINE - 1)) {
			printf("\n");
		}
	}
	printf("\n");
#ifndef CONFIG_DISABLE_ENIVRON
	printf("\nIf you want to run an ASYNC command with specific priority and stacksize\n");
	printf("use \"setenv %s\" or \"%s\"\n", TASH_ASYNC_CMD_PRI_STR, TASH_ASYNC_CMD_STACK_STR);
#endif
	return 0;
}

/** @brief TASH exit function. It cannot be re-launched.
 *  @ingroup tash
 */
static int tash_exit(int argc, char **args)
{
	printf("TASH: Good bye!!\n");
	tash_running = FALSE;
	exit(0);
}

/** @brief Thread Entry point for commands to execute in a seperate thread
 *  @ingroup tash
 */
static void *tash_cmdthread_entry(void *arg)
{
	int args_idx;
	struct tash_cmd_targs_s *tash_cmdthread_s;

	tash_cmdthread_s = (struct tash_cmd_targs_s *)arg;

	/* excute a callback function */
	tash_cmdthread_s->cb(tash_cmdthread_s->argc, tash_cmdthread_s->args);

	/* Free up the memory */
	for (args_idx = 0; args_idx < tash_cmdthread_s->argc; args_idx++) {
		tash_free(tash_cmdthread_s->args[args_idx]);
	}
	tash_free(tash_cmdthread_s->args);
	tash_free(tash_cmdthread_s);

	return NULL;
}

/** @brief Launch a pthread to run tash cmd asynchronously
 *  @ingroup tash
 */
static int tash_launch_cmdthread(struct tash_cmd_targs_s *arg)
{
	int ret = 0;
	pthread_attr_t attr;
	pthread_t tid;
	pthread_attr_init(&attr);
	struct sched_param sparam;
	int pri = TASH_CMDTHREAD_PRIORITY;
	long stack_size = TASH_CMDTHREAD_STACKSIZE;
#ifndef CONFIG_DISABLE_ENIVRON
	char *env_pri;
	char *env_stack;
	int is_setenv = false;

	env_pri = getenv(TASH_ASYNC_CMD_PRI_STR);
	if (env_pri != NULL) {
		pri = atoi(env_pri);
		is_setenv = true;
	}

	env_stack = getenv(TASH_ASYNC_CMD_STACK_STR);
	if (env_stack != NULL) {
		stack_size = atoi(env_stack);
		is_setenv = true;
	}

	if (is_setenv) {
		printf("Priority and Stack size for TASH ASYNC cmd are changed\n");
		printf("Command will be launched with pri (%d), stack size(%d)\n", pri, stack_size);
	}
#endif
	sparam.sched_priority = pri;
	(void)pthread_attr_setschedparam(&attr, &sparam);
	(void)pthread_attr_setstacksize(&attr, stack_size);

	ret = pthread_create(&tid, &attr, &tash_cmdthread_entry, (void *)arg);
	if (ret == OK) {
		pthread_setname_np(tid, arg->args[0]);
	}

	pthread_detach(tid);

	return ret;
}

/** @brief Duplicate args to be passed to pthread
 *  @ingroup tash
 */
static struct tash_cmd_targs_s *tash_dupargs(int argc, char *args[], TASH_CMD_CALLBACK cb)
{
	struct tash_cmd_targs_s *new_arg_s = NULL;
	int dup_idx;
	int cancel_idx;

	new_arg_s = (struct tash_cmd_targs_s *)tash_alloc(sizeof(struct tash_cmd_targs_s));
	if (new_arg_s != NULL) {
		new_arg_s->argc = argc;
		new_arg_s->cb = cb;
		new_arg_s->args = (char **)tash_alloc(argc * sizeof(char *));
		if (new_arg_s->args != NULL) {
			for (dup_idx = 0; dup_idx < argc; dup_idx++) {
				new_arg_s->args[dup_idx] = strdup(args[dup_idx]);
				if (new_arg_s->args[dup_idx] == NULL) {
					for (cancel_idx = 0; cancel_idx < dup_idx; cancel_idx++) {
						tash_free(new_arg_s->args[cancel_idx]);
					}
					tash_free(new_arg_s->args);
					tash_free(new_arg_s);
					return NULL;
				}
			}
		} else {
			tash_free(new_arg_s);
			return NULL;
		}
	}

	return new_arg_s;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/** @brief TASH command execution in tash task context
 *  @ingroup tash
 */
int tash_execute_cmd(char **args, int argc)
{
	int cmd_idx;
	int cmd_found = 0;

	/* lock mutex */
	pthread_mutex_lock(&tash_cmds_info.tmutex);

	for (cmd_idx = 0; cmd_idx < tash_cmds_info.count; cmd_idx++) {
		cmd_found = (strncmp(args[0], tash_cmds_info.cmd[cmd_idx].str, TASH_CMD_MAXSTRLENGTH - 1) == 0) ? 1 : 0;
		if (cmd_found) {
			if (tash_cmds_info.cmd[cmd_idx].exec_type == TASH_EXECMD_SYNC) {
				/* unlock mutex before executing */
				pthread_mutex_unlock(&tash_cmds_info.tmutex);

				/* function call to execute SYNC command */
				(*tash_cmds_info.cmd[cmd_idx].cb) (argc, args);
			} else if (tash_cmds_info.cmd[cmd_idx].exec_type == TASH_EXECMD_ASYNC) {
				/* Save arguments to launch a pthread */
				struct tash_cmd_targs_s *targs = NULL;
				targs = tash_dupargs(argc, args, tash_cmds_info.cmd[cmd_idx].cb);

				/* unlock mutex before executing */
				pthread_mutex_unlock(&tash_cmds_info.tmutex);

				if (targs != NULL) {
					/* launch a pthread to execute ASYNC command */
					if (tash_launch_cmdthread(targs)) {
						shdbg("TASH: error in command thread launch \n");
					}
				}
			} else {
				/* unlock mutex */
				pthread_mutex_unlock(&tash_cmds_info.tmutex);

				shdbg("TASH: cmd (%s) has wrong value on exec type\n", args[0]);
			}
			break;
		}						/* cmd_found */
	}							/* for */

	if (!cmd_found) {
		/* unlock mutex */
		pthread_mutex_unlock(&tash_cmds_info.tmutex);

		printf("TASH: cmd (%s) not registered\n", args[0]);
	}

	return 0;					/* Need to pass the appropriate error later */
}

/** @name tash_cmd_install
 * @brief API to install TASH commands
 * @ingroup tash
 * @param[in] str - command's first string(Example: ps, ls, ifconfig etc)
 * @param[in] cb - callback function for the command
 * @param[in] thread_exec: TASH_EXECMD_ASYNC -execute callback as a thread
						   TASH_EXECMD_SYNC- invoke callback directly
 * @retval 0 Success
 * @retval -ve Failure
 *
 * Example Usage:
 * @code
 *
 * @endcode
 */
int tash_cmd_install(const char *str, TASH_CMD_CALLBACK cb, int thread_exec)
{
	int cmd_idx;

	if (TASH_MAX_COMMANDS == tash_cmds_info.count) {
		return -1;				/* MAX cmd count reached */
	}

	/* Lock mutex */
	pthread_mutex_lock(&tash_cmds_info.tmutex);

	/* CHeck if cmd is already installed */
	for (cmd_idx = 0; cmd_idx < tash_cmds_info.count; cmd_idx++) {
		if (strncmp(str, tash_cmds_info.cmd[cmd_idx].str, TASH_CMD_MAXSTRLENGTH - 1) == 0) {
			pthread_mutex_unlock(&tash_cmds_info.tmutex);
			return -2;			/* CMD already installed */
		}
	}

	/* store command string - no need of explicit NULL termination */
	strncpy(tash_cmds_info.cmd[tash_cmds_info.count].str, str, TASH_CMD_MAXSTRLENGTH - 1);
	/* store callback */
	tash_cmds_info.cmd[tash_cmds_info.count].cb = cb;
	/* store thread_exec flags */
	tash_cmds_info.cmd[tash_cmds_info.count].exec_type = thread_exec;
	/* Increment command count value */
	tash_cmds_info.count++;
	pthread_mutex_unlock(&tash_cmds_info.tmutex);

	is_sorted_list = FALSE;

	return 0;
}

/** @name tash_cmdlist_install
 * @brief API to register TASH command list
 * @ingroup tash
 * @param[in] list[] - tash command list structure which has command name, callback fnc, and attribute
 *
 * Example Usage:
 * @code
 *
 * @endcode
 **/
void tash_cmdlist_install(const tash_cmdlist_t list[])
{
	const tash_cmdlist_t *map;

	for (map = list; map->cb; map++) {
		tash_cmd_install(map->str, map->cb, map->thread_exec);
	}
}

void tash_register_basic_cmds(void)
{
	tash_cmdlist_install(tash_basic_cmds);
}

#if defined(CONFIG_TASH_TELNET_INTERFACE)
/** @name tash_get_cmdscount
 * @brief API to get the number of registered tash commands.
 * In protected build, it returns the count of user context commands only.
 * @ingroup tash
 * @ret +ve - Tash commands count
 *     -ve - Error
 *
 * Example Usage:
 * @code
 * @endcode
 **/
int tash_get_cmdscount(void)
{
	int count = 0;

	pthread_mutex_lock(&tash_cmds_info.tmutex);
	count = tash_cmds_info.count;
	pthread_mutex_unlock(&tash_cmds_info.tmutex);

	return count;
}

/** @name tash_get_cmdpair
 * @brief API to get registered tash commands one at a time
 * @ingroup tash
 * @param[in] str- Pointer to get the command string with space for 32 bytes
 * @param[in] cb- Callback function pointer
 * @param[in] index- Tash command index

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
int tash_get_cmdpair(char *str, TASH_CMD_CALLBACK *cb, int index)
{
	int count = 0;
	int ret = -1;

	pthread_mutex_lock(&tash_cmds_info.tmutex);
	count = tash_cmds_info.count;
	/* NOTE: This has to be modified for protected build */
	if (index <= count) {
		/* Copy string */
		strncpy(str, tash_cmds_info.cmd[index].str, TASH_CMD_MAXSTRLENGTH - 1);
		/* Copy function ptr */
		*cb = tash_cmds_info.cmd[index].cb;
		ret = OK;
	}
	pthread_mutex_unlock(&tash_cmds_info.tmutex);

	return ret;
}
#endif
