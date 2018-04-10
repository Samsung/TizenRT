/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/// @file itc_libc_spawn.c

/// @brief ITC Test Case Example for Libc Spawn API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <spawn.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyara/config.h>
#include "tc_internal.h"
#include <unistd.h>
#include <sys/types.h>


#define ITC_SPAWN_STACKSIZE 2048
#define WAIT_TIME 3

/**
* @fn                   :function_name_spawn
* @description          :Function used for task_spawn
* @return               :int
*/
static int function_name_spawn(int argc, char *argv[])
{
	sleep(WAIT_TIME);
	return 0;
}

/**
* @fn                   :itc_libc_spawn_posix_spawnattr_init_p_reinit
* @brief                :Initializes the given spawn attributes object.
* @scenario             :re-initialization of spawn attr
* @API's covered        :posix_spawnattr_init
* @Preconditions        :none
* @Postconditions       :none
*/
static void itc_libc_spawn_posix_spawnattr_init_p_reinit(void)
{
	int ret_chk = ERROR;
	posix_spawnattr_t st_attr;

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_init", st_attr.flags, 0);
	TC_ASSERT_EQ("posix_spawnattr_init", st_attr.stacksize, CONFIG_TASK_SPAWN_DEFAULT_STACKSIZE);

	ret_chk = posix_spawnattr_init(&st_attr);
	TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);
	TC_ASSERT_EQ("posix_spawnattr_init", st_attr.flags, 0);
	TC_ASSERT_EQ("posix_spawnattr_init", st_attr.stacksize, CONFIG_TASK_SPAWN_DEFAULT_STACKSIZE);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawnattr_setgetflags_n
* @brief                : set and get the spawn-flags attribute in an initialized attributes object referenced by st_attr
* @scenario             : Passing spawn-flags with in-valid attributes in setter as well as in getter.
* @API's covered        : posix_spawnattr_init,posix_spawnattr_setschedparam, posix_spawnattr_setflags, posix_spawnattr_getflags
* @Preconditions        : posix_spawnattr_init, posix_spawnattr_setschedparam
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawnattr_setgetflags_n(void)
{
	int ret_chk = ERROR;
	short int flags;

	ret_chk = posix_spawnattr_setflags(NULL, POSIX_SPAWN_SETSCHEDPARAM);
	TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, EINVAL);

	ret_chk = posix_spawnattr_getflags(NULL, &flags);
	TC_ASSERT_EQ("posix_spawnattr_getflags", ret_chk, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawnattr_SetGetflags_p_all_flag
* @brief                : set and get the spawn-flags attribute in an initialized attributes object referenced by st_attr
* @scenario             : The spawn-flags attribute is used to indicate which process attributes are to be changed in the new process image.
* @API's covered        : posix_spawnattr_init,posix_spawnattr_setschedparam, posix_spawnattr_setflags, posix_spawnattr_getflags
* @Preconditions        : posix_spawnattr_init, posix_spawnattr_setschedparam
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawnattr_setgetflags_p_all_flag(void)
{
	int ret_chk = ERROR;
	pid_t pid;
	posix_spawnattr_t st_attr;
	short int flags;
	int exec_index;
	int status;
	int spawn_attr[] = { POSIX_SPAWN_RESETIDS, POSIX_SPAWN_SETPGROUP, POSIX_SPAWN_SETSCHEDPARAM,
		POSIX_SPAWN_SETSCHEDULER, POSIX_SPAWN_SETSIGDEF, POSIX_SPAWN_SETSIGMASK };

	int cnt = sizeof(spawn_attr) / sizeof(spawn_attr[0]);

	for (exec_index = 0; exec_index < cnt; exec_index++) {
		ret_chk = posix_spawnattr_init(&st_attr);
		TC_ASSERT_EQ("posix_spawnattr_init", ret_chk, OK);

		ret_chk = posix_spawnattr_setflags(&st_attr, spawn_attr[exec_index]);
		TC_ASSERT_EQ("posix_spawnattr_setflags", ret_chk, OK);

		ret_chk = task_spawn(&pid, "spawn_flag", function_name_spawn, NULL, &st_attr, (char *const *)NULL, (char *const *)NULL);

		TC_ASSERT_EQ("task_spawn", ret_chk, OK);
		int ret_pid = waitpid(pid, &status, 0);
		TC_ASSERT_NEQ("waitpid", ret_pid, -1);

		ret_chk = posix_spawnattr_getflags(&st_attr, &flags);
		TC_ASSERT_EQ("posix_spawnattr_getflags", ret_chk, OK);
		TC_ASSERT_EQ("posix_spawnattr_getflags", flags, spawn_attr[exec_index]);
	}
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawnattr_setgetschedparam_n
* @brief                : sets and gets the value of the scheduling parameters attribute
* @scenario             : posix_spawnattr_setschedparam invalid params in setter as well as in getter.
* @API's covered        : posix_spawnattr_setschedparam, posix_spawnattr_getschedparam
* @Preconditions        : none
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawnattr_setgetschedparam_n(void)
{
	int ret_chk = ERROR;
	struct sched_param setParam;
	struct sched_param getParam;

	ret_chk = posix_spawnattr_setschedparam(NULL, &setParam);
	TC_ASSERT_EQ("posix_spawnattr_setschedparam", ret_chk, EINVAL);

	ret_chk = posix_spawnattr_getschedparam(NULL, &getParam);
	TC_ASSERT_EQ("posix_spawnattr_getschedparam", ret_chk, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawnattr_setgetschedpolicy_n
* @brief                : Set and get the spawn-sched policy attribute in an initialized attributes object referenced by st_attr.
* @scenario             : posix_spawnattr_setschedpolicy with invalid params in setter as well as in getter.
* @API's covered        : posix_spawnattr_setschedpolicy, posix_spawnattr_getschedpolicy
* @Preconditions        : none
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawnattr_setgetschedpolicy_n(void)
{
	int ret_chk = ERROR;
	int schedpolicy;

	ret_chk = posix_spawnattr_setschedpolicy(NULL, SCHED_RR);
	TC_ASSERT_EQ("posix_spawnattr_setschedpolicy", ret_chk, EINVAL);

	ret_chk = posix_spawnattr_getschedpolicy(NULL, &schedpolicy);
	TC_ASSERT_EQ("posix_spawnattr_getschedpolicy", ret_chk, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawnattr_setgetsigmask_n
* @brief                : sets and gets the spawn-sigmask attribute in an initialized attributes object referenced by st_attr
* @scenario             : posix_spawnattr_setsigmask with invalid params in setter as well as in getter.
* @API's covered        : posix_spawnattr_setsigmask, posix_spawnattr_getsigmask
* @Preconditions        : none
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawnattr_setgetsigmask_n(void)
{
	int ret_chk = ERROR;
	sigset_t setMask = MIN_SIGNO;
	sigset_t getMask;

	ret_chk = posix_spawnattr_setsigmask(NULL, &setMask);
	TC_ASSERT_EQ("posix_spawnattr_setsigmask", ret_chk, EINVAL);

	ret_chk = posix_spawnattr_getsigmask(NULL, &getMask);
	TC_ASSERT_EQ("posix_spawnattr_getsigmask", ret_chk, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_libc_spawn_task_spawnattr_setgetstacksize_n
* @brief                :sets and gets the stack size attribute of the thread attributes object referred to by st_attr
* @scenario             :task_spawnattr_setstacksize with invalid params in setter as well as in getter.
* @API's covered        :posix_spawnattr_init, task_spawnattr_setstacksize,task_spawnattr_getstacksize
* @Preconditions        :posix_spawnattr_init
* @Postconditions       :none
*/
static void itc_libc_spawn_task_spawnattr_setgetstacksize_n(void)
{
	int ret_chk = ERROR;
	size_t stacksize;

	ret_chk = task_spawnattr_setstacksize(NULL, ITC_SPAWN_STACKSIZE);
	TC_ASSERT_EQ("task_spawnattr_setstacksize", ret_chk, EINVAL);

	ret_chk = task_spawnattr_getstacksize(NULL, &stacksize);
	TC_ASSERT_EQ("task_spawnattr_getstacksize", ret_chk, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawn_file_actions_addopen_p_reopen
* @brief                : adds an open action to the object referenced by fact_p that causes the file named by path to be opened
* @scenario             : re addopen action once the object is open succesfully
* @API's covered        : posix_spawn_file_actions_init,posix_spawn_file_actions_addopen
* @Preconditions        : none
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawn_file_actions_addopen_p_reopen(void)
{
	const char szfilepath[] = "./testdata.txt";
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 0, szfilepath, O_RDONLY, 0644);
	TC_ASSERT_EQ("posix_spawn_file_actions_addopen", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 0, szfilepath, O_RDONLY, 0644);
	TC_ASSERT_EQ("posix_spawn_file_actions_addopen", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addclose(&st_fileactions, 0);
	TC_ASSERT_EQ("posix_spawn_file_actions_addclose", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   : itc_libc_spawn_posix_spawn_file_actions_addclose_p_reclose
* @brief                : adds an open action to the object referenced by fact_p that causes the file named by path to be opened
* @scenario             : re addclose action once the object is close succesfully
* @API's covered        : posix_spawn_file_actions_init,posix_spawn_file_actions_addopen
* @Preconditions        : none
* @Postconditions       : none
*/
static void itc_libc_spawn_posix_spawn_file_actions_addclose_p_reclose(void)
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

	ret_chk = posix_spawn_file_actions_addclose(&st_fileactions, 0);
	TC_ASSERT_EQ("posix_spawn_file_actions_addclose", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}


/**
* @fn                   : itc_libc_spawn_file_actions_addclose_n_after_destroy
* @brief                : adds an open action to the object referenced by fact_p that causes the file named by path to be opened
* @scenario             : addclose action once the object is destroyed succesfully
* @API's covered        : posix_spawn_file_actions_init,posix_spawn_file_actions_addopen
* @Preconditions        : none
* @Postconditions       : none
*/
static void itc_libc_spawn_file_actions_addclose_n_after_destroy(void)
{
	const char szfilepath[] = "./testdata.txt";
	posix_spawn_file_actions_t st_fileactions;
	int ret_chk = ERROR;

	ret_chk = posix_spawn_file_actions_init(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_init", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addopen(&st_fileactions, 0, szfilepath, O_RDONLY, 0644);
	TC_ASSERT_EQ("posix_spawn_file_actions_addopen", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_destroy(&st_fileactions);
	TC_ASSERT_EQ("posix_spawn_file_actions_destroy", ret_chk, OK);

	ret_chk = posix_spawn_file_actions_addclose(&st_fileactions, 0);
	TC_ASSERT_EQ("posix_spawn_file_actions_addclose", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_spawn
 ****************************************************************************/

int itc_libc_spawn_main(void)
{
	itc_libc_spawn_posix_spawnattr_init_p_reinit();
	itc_libc_spawn_posix_spawnattr_setgetflags_p_all_flag();
	itc_libc_spawn_posix_spawn_file_actions_addopen_p_reopen();
	itc_libc_spawn_posix_spawn_file_actions_addclose_p_reclose();
	itc_libc_spawn_file_actions_addclose_n_after_destroy();
	/* System crash for below itc's */
	itc_libc_spawn_posix_spawnattr_setgetflags_n();
	itc_libc_spawn_posix_spawnattr_setgetschedparam_n();
	itc_libc_spawn_posix_spawnattr_setgetschedpolicy_n();
	itc_libc_spawn_posix_spawnattr_setgetsigmask_n();
	itc_libc_spawn_task_spawnattr_setgetstacksize_n();
	return 0;
}

