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

/// @file libc_spawn.c

/// @brief Test Case Example for Libc Spawn API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <tinyara/spawn.h>
#include <spawn.h>
#include <sys/types.h>
#include <fcntl.h>
#include "tc_internal.h"

#define HIGH_PRIORITY 5
#define SEC_3 3
#define SEC_4 4
#define SEC_10000 10000
#define MIN_SIGNO 0
#define FILEPATH "/dev/smart1"
#define SPAWN_STACKSIZE 2048

/**
* @fn                   :function_name_spawn
* @description          :Function used for task_spawn
* @return               :int
*/
static int function_name_spawn(int argc, char *argv[])
{
	usleep(SEC_10000);
	return 0;
}

/**
* @fn                   :tc_libc_spawn_posix_spawnattr_init
* @brief                :Initializes the given spawn attributes object.
* @scenario             :Initializes the given spawn attributes object with the default value for all the attributes
* @API's covered        :posix_spawnattr_init
* @Preconditions        :none
* @Postconditions       :none
* @Return               :void
*/
static void tc_libc_spawn_posix_spawnattr_init(void)
{
	int ret_chk = ERROR;
	posix_spawnattr_t st_attr;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_init", st_attr.flags, 0);
	TC_ASSERT_EQ("posix_spawnattr_init", st_attr.stacksize, CONFIG_TASK_SPAWN_DEFAULT_STACKSIZE);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawnattr_SetGetschedparam
* @brief                : sets and gets the value of the scheduling parameters attribute
* @scenario             : The scheduling parameters attribute governs the parameter assigned to a new process image in a spawn operation
* @API's covered        : posix_spawnattr_init, posix_spawnattr_setschedparam, posix_spawnattr_getschedparam
* @Preconditions        : posix_spawnattr_init
* @Postconditions       : none
* @Return               : void
*/
static void tc_libc_spawn_posix_spawnattr_setgetschedparam(void)
{
	int ret_chk = ERROR;
	pid_t pid;
	struct sched_param setParam;
	struct sched_param getParam;
	posix_spawnattr_t st_attr;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

	setParam.sched_priority = HIGH_PRIORITY;
	ret_chk = posix_spawnattr_setschedparam(&st_attr, &setParam);
	TC_ASSERT_EQ("posix_spawnattr_setschedparam", ret_chk, OK);

	ret_chk = task_spawn(&pid, "spawn", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);
	sleep(SEC_4);
	TC_ASSERT_EQ("task_spawn", ret_chk, OK);

	ret_chk = posix_spawnattr_getschedparam(&st_attr, &getParam);
	TC_ASSERT_EQ("posix_spawnattr_getschedparam", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_getschedparam", setParam.sched_priority, getParam.sched_priority);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawnattr_SetGetflags
* @brief                : set and get the spawn-flags attribute in an initialized attributes object referenced by st_attr
* @scenario             : The spawn-flags attribute is used to indicate which process attributes are to be changed in the new process image.
* @API's covered        : posix_spawnattr_init,posix_spawnattr_setschedparam, posix_spawnattr_setflags, posix_spawnattr_getflags
* @Preconditions        : posix_spawnattr_init, posix_spawnattr_setschedparam
* @Postconditions       : none
* @Return               : void
*/

static void tc_libc_spawn_posix_spawnattr_setgetflags(void)
{
	int ret_chk = ERROR;
	pid_t pid;
	posix_spawnattr_t st_attr;
	short int flags;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_SETSCHEDPARAM);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);

	ret_chk = task_spawn(&pid, "spawn_flag", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);
	sleep(SEC_3);
	TC_ASSERT_EQ("task_spawn", ret_chk, OK);

	ret_chk = posix_spawnattr_getflags(&st_attr, &flags);
	TC_ASSERT_EQ("posix_spawnattr_getflags", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_getflags", flags, POSIX_SPAWN_SETSCHEDPARAM);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawnattr_SetGetschedpolicy
* @brief                : Set and get the spawn-sched policy attribute in an initialized attributes object referenced by st_attr.
* @scenario             : The spawn-sched policy attribute represents the scheduling policy to be assigned to the new process image in a spawn operation
* @API's covered        : posix_spawnattr_init, posix_spawnattr_setschedpolicy, posix_spawnattr_getschedpolicy
* @Preconditions        : posix_spawnattr_init
* @Postconditions       : none
* @Return               : void
*/

static void tc_libc_spawn_posix_spawnattr_setgetschedpolicy(void)
{
	int ret_chk = ERROR;
	pid_t pid;
	posix_spawnattr_t st_attr;
	int schedpolicy;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

	ret_chk = posix_spawnattr_setschedpolicy(&st_attr, SCHED_RR);
	TC_ASSERT_EQ("posix_spawnattr_setschedpolicy", ret_chk, OK);

	ret_chk = task_spawn(&pid, "spawn_policy", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);
	sleep(SEC_4);
	TC_ASSERT_EQ("task_spawn", ret_chk, OK);

	ret_chk = posix_spawnattr_getschedpolicy(&st_attr, &schedpolicy);
	TC_ASSERT_EQ("posix_spawnattr_getschedpolicy", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_getschedpolicy", schedpolicy, SCHED_RR);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_spawn_task_spawnattr_SetGetstacksize
* @brief                :sets and gets the stack size attribute of the thread attributes object referred to by st_attr
* @scenario             :The stack size attribute determines the minimum size (in bytes) that will be allocated for
*                        threads created using the thread attributes object st_attr.
* @API's covered        :posix_spawnattr_init, task_spawnattr_setstacksize,task_spawnattr_getstacksize
* @Preconditions        :posix_spawnattr_init
* @Postconditions       :none
* @Return               :void
*/
static void tc_libc_spawn_task_spawnattr_setgetstacksize(void)
{
	int ret_chk = ERROR;
	pid_t pid;
	posix_spawnattr_t st_attr;
	size_t stacksize;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

	ret_chk = task_spawnattr_setstacksize(&st_attr, SPAWN_STACKSIZE);
	TC_ASSERT_EQ("task_spawnattr_setstacksize", ret_chk, OK);

	ret_chk = task_spawn(&pid, "spawn_stacksize", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);
	sleep(SEC_4);
	TC_ASSERT_EQ("task_spawn", ret_chk, OK);

	ret_chk = task_spawnattr_getstacksize(&st_attr, &stacksize);
	TC_ASSERT_EQ("task_spawnattr_getstacksize", ret_chk, OK);
	TC_ASSERT_EQ("task_spawnattr_getstacksize", stacksize, SPAWN_STACKSIZE);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawnattr_SetGetsigmask
* @brief                : sets and gets the spawn-sigmask attribute in an initialized attributes object referenced by st_attr
* @scenario             : The spawn-sigmask attribute represents the signal mask in effect in the new process image of a spawn operation
* @API's covered        : posix_spawnattr_init, posix_spawnattr_setsigmask, posix_spawnattr_getsigmask
* @Preconditions        : posix_spawnattr_init
* @Postconditions       : none
* @Return               : void
*/
static void tc_libc_spawn_posix_spawnattr_setgetsigmask(void)
{
	posix_spawnattr_t st_attr;
	int ret_chk = ERROR;
	sigset_t setMask = MIN_SIGNO;
	sigset_t getMask;
	pid_t pid;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

	ret_chk = posix_spawnattr_setsigmask(&st_attr, &setMask);
	TC_ASSERT_EQ("posix_spawnattr_setsigmask", ret_chk, OK);

	ret_chk = task_spawn(&pid, "spawn_sigmask", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);
	sleep(SEC_4);
	TC_ASSERT_EQ("task_spawn", ret_chk, OK);

	ret_chk = posix_spawnattr_getsigmask(&st_attr, &getMask);
	TC_ASSERT_EQ("posix_spawnattr_getsigmask", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_getsigmask", setMask, getMask);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_spawn_posix_spawnattr_dump
* @brief                :Show the current attributes
* @scenario             :It dumps the spawn attributes whose address is passed
* @API's covered        :posix_spawnattr_init, posix_spawnattr_setschedparam, posix_spawnattr_setflags, posix_spawnattr_setschedpolicy, posix_spawnattr_dump
* @Preconditions        :posix_spawnattr_init, posix_spawnattr_setschedparam, posix_spawnattr_setflags, posix_spawnattr_setschedpolicy
* @Postconditions       :none
* @Return               :void
*/
static void tc_libc_spawn_posix_spawnattr_dump(void)
{
	int ret_chk = ERROR;
	pid_t pid;
	posix_spawnattr_t st_attr;

	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

	ret_chk = posix_spawnattr_setschedpolicy(&st_attr, SCHED_RR);
	TC_ASSERT_EQ("posix_spawnattr_setschedpolicy", ret_chk, OK);

	ret_chk = task_spawn(&pid, "spawn_dump", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);
	sleep(SEC_4);
	TC_ASSERT_EQ("task_spawn", ret_chk, OK);

	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setschedpolicy(&st_attr, SCHED_FIFO);
	TC_ASSERT_EQ("posix_spawnattr_setschedpolicy", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_SETSCHEDPARAM);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_RESETIDS);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_SETPGROUP);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_SETSCHEDULER);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_SETSIGDEF);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	ret_chk = posix_spawnattr_setflags(&st_attr, POSIX_SPAWN_SETSIGMASK);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);
	posix_spawnattr_dump(&st_attr);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawn_file_actions_init
* @brief                : Initializes the object referenced by file_action
* @scenario             : initializes the object referenced by st_fileactions, to an empty set of file actions for subsequent use
* @API's covered        : posix_spawn_file_actions_init, posix_spawn_file_actions_destroy
* @Preconditions        : none
* @Postconditions       : posix_spawn_file_actions_destroy
* @Return               : void
*/
static void tc_libc_spawn_posix_spawn_file_actions_init(void)
{
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", st_fileactions, NULL);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   : tc_libc_spawn_posix_spawn_file_actions_adddup2()
 * @brief                : adds a dup2 operation to the object referenced by file_actions, for subsequent use in a call to posix_spawn()/posix_spawnp()
 * @scenario             : The descriptor referred to by fd2 is created as if dup2() had been called on fd1 prior to the new child process starting execution.
 *                         On success, these functions return 0; on failure they return an error
 * @API's covered        : posix_spawn_file_actions_adddup2
 * @Preconditions        : posix_spawn_file_actions_init
 * @Postconditions       : posix_spawn_file_actions_destroy
 * @Return               : void
 */
static void tc_libc_spawn_posix_spawn_file_actions_adddup2(void)
{
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_adddup2(&st_fileactions, 1, 2);
	TC_ASSERT_EQ("posix_spawn_file_actions_adddup2", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawn_file_actions_addOpenClose
* @brief                : adds an open action to the object referenced by fact_p that causes the file named by path to be opened
* @scenario             : Files that need to be opened for use by the spawned process can alternatively be handled either by having the
*                         calling process open or by passing filenames to the spawned process
* @API's covered        : posix_spawn_file_actions_init,posix_spawn_file_actions_addopen
* @Preconditions        : posix_spawn_file_actions_init
* @Postconditions       : posix_spawn_file_actions_destroy
* @Return               : void
*/
static void tc_libc_spawn_posix_spawn_file_actions_addopenclose(void)
{
	const char szfilepath[] = "./testdata.txt";
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 0, szfilepath, O_RDONLY, 0644);
	TC_ASSERT_EQ("posix_spawn_file_actions_addopen", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addclose(&st_fileactions, 0);
	TC_ASSERT_EQ("posix_spawn_file_actions_addclose", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawn_file_actions_destroy
* @brief                : destroy the object referenced by st_fileactions
* @scenario             : destroys the object referenced by st_fileactions which was previously intialized
* @API's covered        : posix_spawn_file_actions_init, posix_spawn_file_actions_addopen ,posix_spawn_file_actions_destroy
* @Preconditions        : posix_spawn_file_actions_init ,posix_spawn_file_actions_addopen
* @Postconditions       : none
* @Return               : void
*/
static void tc_libc_spawn_posix_spawn_file_actions_destroy(void)
{
	const char szfilepath[] = "./testdata.txt";
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 1, szfilepath, O_WRONLY, 0644);
	TC_ASSERT_EQ("posix_spawn_file_actions_addopen", ret_chk, OK);
	TC_ASSERT_NEQ("posix_spawn_file_actions_addopen", st_fileactions, NULL);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", st_fileactions, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_libc_spawn_posix_spawn_file_actions_dump
* @brief                : Dump the file action whose address is passed
* @scenario             : Dump the file action whose address is passed
* @API's covered        : posix_spawn_file_actions_init, posix_spawn_file_actions_addopen, posix_spawn_file_actions_dump, posix_spawn_file_actions_destroy
* @Preconditions        : posix_spawn_file_actions_init, posix_spawn_file_actions_addopen
* @Postconditions       : posix_spawn_file_actions_destroy
* @Return               : void
*/
static void tc_libc_spawn_posix_spawn_file_actions_dump(void)
{
	const char szfilepath[] = "./testdata.txt";
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	posix_spawn_file_actions_dump(&st_fileactions);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 1, szfilepath, O_WRONLY, 0644);
	TC_ASSERT_EQ_CLEANUP("posix_spawn_file_actions_addopen", ret_chk, OK, goto errout);

	/* posix_spawn_file_actions_dump returns (void) */

	posix_spawn_file_actions_dump(&st_fileactions);

	ret_chk = posix_spawn_file_actions_adddup2(&st_fileactions, 1, 2);
	TC_ASSERT_EQ_CLEANUP("posix_spawn_file_actions_adddup2", ret_chk, OK, goto errout);

	posix_spawn_file_actions_dump(&st_fileactions);

	ret_chk = posix_spawn_file_actions_addclose(&st_fileactions, 1);
	TC_ASSERT_EQ_CLEANUP("posix_spawn_file_actions_addclose", ret_chk, OK, goto errout);

	posix_spawn_file_actions_dump(&st_fileactions);

	/* Destroy each file action, one at a time */

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();

errout:
	posix_spawn_file_actions_destroy(&st_fileactions);

}

/**
* @fn                   : tc_libc_spawn_add_file_action
* @brief                : Add the file action
* @scenario             : Add the file action to the end for the file action list
* @API's covered        : posix_spawn_file_actions_init, posix_spawn_file_actions_addopen, add_file_action
* @Preconditions        : posix_spawn_file_actions_init,posix_spawn_file_actions_addopen
* @Postconditions       : posix_spawn_file_actions_destroy
* @Return               : void
*/
static void tc_libc_spawn_add_file_action(void)
{
	const char szfilepath[] = "./testdata.txt";
	posix_spawn_file_actions_t st_fileactions;
	struct spawn_open_file_action_s *entry1;
	struct spawn_open_file_action_s *entry2;
	size_t length;
	size_t alloc_num;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 1, szfilepath, O_WRONLY, 0644);
	TC_ASSERT_EQ("posix_spawn_file_actions_addopen", ret_chk, OK);

	length = strlen(szfilepath);
	alloc_num = SIZEOF_OPEN_FILE_ACTION_S(length);

	/* Allocate the action list entry of this size */

	entry1 = (struct spawn_open_file_action_s *)zalloc(alloc_num);
	TC_ASSERT_NEQ("zalloc", entry1, NULL);

	/* And add it to the file action list */

	add_file_action(st_fileactions, (struct spawn_general_file_action_s *)entry1);

	entry2 = (struct spawn_open_file_action_s *)zalloc(alloc_num);
	TC_ASSERT_NEQ("zalloc", entry2, NULL);

	/* And add it to the file action list */

	add_file_action(st_fileactions, (struct spawn_general_file_action_s *)entry2);
	TC_ASSERT_EQ("add_file_action", entry1->flink, (struct spawn_general_file_action_s *)entry2);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_spawn
 ****************************************************************************/

int libc_spawn_main(void)
{
	tc_libc_spawn_posix_spawnattr_dump();
	tc_libc_spawn_posix_spawnattr_init();
	tc_libc_spawn_posix_spawnattr_setgetflags();
	tc_libc_spawn_posix_spawnattr_setgetschedparam();
	tc_libc_spawn_posix_spawnattr_setgetschedpolicy();
	tc_libc_spawn_posix_spawnattr_setgetsigmask();
	tc_libc_spawn_task_spawnattr_setgetstacksize();
	tc_libc_spawn_posix_spawn_file_actions_init();
	tc_libc_spawn_add_file_action();
	tc_libc_spawn_posix_spawn_file_actions_adddup2();
	tc_libc_spawn_posix_spawn_file_actions_addopenclose();
	tc_libc_spawn_posix_spawn_file_actions_destroy();
	tc_libc_spawn_posix_spawn_file_actions_dump();

	return 0;
}
