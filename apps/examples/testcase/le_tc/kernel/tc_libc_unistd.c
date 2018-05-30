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

/// @file tc_libc_unistd.c
/// @brief Test Case Example for Libc Unistd API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "tc_internal.h"

#define ARG_COUNT       6
#define BUFFSIZE        32
#ifndef CONFIG_DISABLE_SIGNALS
#define SEC_3           3
#define USLEEP_INTERVAL 3000000
#endif
#ifdef CONFIG_PIPES
#define MSG_SIZE        30

sem_t pipe_sem;
int pipe_tc_chk = OK;
int pipe_fd[2];
const char msg[MSG_SIZE + 1] = "012345678901234567890123456789";
char pipe_buf[MSG_SIZE + 1];

static void *pipe_tx_func(void *arg)
{
	int tx_iter;

	for (tx_iter = 0; tx_iter < 10; tx_iter++) {
		write(pipe_fd[1], msg, MSG_SIZE);
	}
	sem_post(&pipe_sem);

	return NULL;
}

static void *pipe_rx_func(void *arg)
{
	int rx_iter;

	sem_wait(&pipe_sem);
	for (rx_iter = 0; rx_iter < 10; rx_iter++) {
		read(pipe_fd[0], pipe_buf, MSG_SIZE);
		if (strcmp(pipe_buf, msg) != 0) {
			pipe_tc_chk = ERROR;
		}
	}
	sem_post(&pipe_sem);

	return NULL;
}
#endif

#ifndef CONFIG_DISABLE_ENVIRON
/**
* @fn                   :tc_libc_unistd_access
* @brief                :check real user's permissions for a file
* @Scenario             :checks whether the calling process can access the file path‐name
* API's covered         :access
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_unistd_access(void)
{
	int ret_chk;
	char path[BUFFSIZE + 1];

	getcwd(path, BUFFSIZE);
	snprintf(path, BUFFSIZE, "%s/%s", path, __FILE__);

	ret_chk = access(path, F_OK);
	TC_ASSERT_EQ("access", ret_chk, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_unistd_chdir_getcwd
* @brief                :The chdir() function shall cause the directory named by the\
*                        pathname pointed to by the path argument to become the current working directory
* @Scenario             :The chdir() function only affects the working directory of the current process.\
*                        Upon successful completion, 0 shall be returned. Otherwise, -1 shall be returned.
* API's covered         :chdir, getcwd
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_unistd_chdir_getcwd(void)
{

	char *cwd;
	char buff[BUFFSIZE];
	char *directory = "/dev";
	int ret_chk;

	ret_chk = chdir(directory);
	TC_ASSERT_EQ("chdir", ret_chk, OK);

	cwd = getcwd(buff, BUFFSIZE);
	TC_ASSERT_EQ("getcwd", strcmp(directory, cwd), 0);

	/* Failure case: size less than strlen(buff) or invalid size */
	cwd = getcwd(buff, 1);
	TC_ASSERT_EQ("getcwd", cwd, NULL);

	/* Failure case: when "PWD" is not defined*/
	unsetenv("PWD");
	cwd = getcwd(buff, BUFFSIZE);
	TC_ASSERT_NEQ("getcwd", strcmp(directory, cwd), 0);

	directory = NULL;
	ret_chk = chdir(directory);
	TC_ASSERT_EQ("chdir", ret_chk, ERROR);

	/* Failure case: path is not a directory */
	directory = "NOTDIR";
	ret_chk = chdir(directory);
	TC_ASSERT_EQ("chdir", ret_chk, ERROR);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_libc_unistd_getopt
* @brief                :The getopt() function returns the next option character specified on the command line.
* @Scenario             :The getopt() function is a command-line parser returns the next option character (if one is found)\
*                        from argv that matches a character in optstring, if there is one that matches.
* API's covered         :getopt
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_unistd_getopt(void)
{
	int flag_a = 0;
	int flag_b = 0;
	int ret = -1;
	int argc = ARG_COUNT;
	char *sz_argv[] = { "arg0", "-a2", "-b", "-c", "-a", "-:" };
	char **arg;
	int *opt;
	int *idx;

	optind = -1;
	while ((ret = getopt(argc, sz_argv, ":a:b")) != ERROR)
		switch (ret) {
		case 'a':
			flag_a = 1;
			arg = getoptargp();
			vdbg("getoptargp: optarg = %s\n", *arg);
			break;
		case 'b':
			flag_b = 1;
			idx = getoptindp();
			vdbg("getoptindp: optind = %d\n", *idx);
			break;
		default:
			opt = getoptoptp();
			vdbg("getoptoptp: optopt = %c\n", *opt);
			break;
		}
	TC_ASSERT_EQ("getopt", flag_a, 1);
	TC_ASSERT_EQ("getopt", flag_b, 1);

	ret = getopt(argc, sz_argv, NULL);
	TC_ASSERT_EQ("getopt", ret, ERROR);

	sz_argv[1] = "a";
	ret = getopt(argc, sz_argv, ":a:b");
	TC_ASSERT_EQ("getopt", ret, ERROR);

	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_SIGNALS
/**
* @fn                   :tc_libc_unistd_sleep
* @brief                :The sleep() function shall cause the calling thread to be suspended from execution until either\
*                        the number of realtime seconds specified by the argument seconds has elapsed
* @Scenario             :If sleep() returns because the requested time has elapsed, the value returned shall be 0.
* API's covered         :sleep
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_unistd_sleep(void)
{
	int ret_chk;
	struct timespec st_init_time;
	struct timespec st_final_time;

	clock_gettime(CLOCK_REALTIME, &st_init_time);

	ret_chk = sleep(SEC_3);
	TC_ASSERT_EQ("sleep", ret_chk, 0);

	clock_gettime(CLOCK_REALTIME, &st_final_time);
	TC_ASSERT_EQ("sleep", st_final_time.tv_sec - st_init_time.tv_sec, SEC_3);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_unistd_usleep
* @brief                :suspend execution for microsecond intervals.
* @Scenario             :The usleep() function suspends execution of the calling thread for(at least) usec microseconds.
* API's covered         :usleep
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_unistd_usleep(void)
{
	int ret_chk;
	struct timespec st_init_time;
	struct timespec st_final_time;

	clock_gettime(CLOCK_REALTIME, &st_init_time);
	ret_chk = usleep(USLEEP_INTERVAL);
	TC_ASSERT_EQ("usleep", ret_chk, 0);

	clock_gettime(CLOCK_REALTIME, &st_final_time);
	TC_ASSERT_EQ("usleep", st_final_time.tv_sec - st_init_time.tv_sec, SEC_3);

	TC_SUCCESS_RESULT();
}
#endif

#ifdef CONFIG_PIPES
/**
* @fn                   :tc_libc_unistd_pipe
* @brief                :creates a pair of file descriptor
* @Scenario             :send and receive data between two threads through pipe
* API's covered         :pipe
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_unistd_pipe(void)
{
	int ret_chk;
	pthread_t th_id[2];

	ret_chk = pipe(pipe_fd);
	TC_ASSERT_NEQ("pipe", ret_chk, ERROR);

	sem_init(&pipe_sem, 0, 0);

	ret_chk = pthread_create(&th_id[0], NULL, pipe_tx_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto cleanup_pipe);

	ret_chk = pthread_create(&th_id[1], NULL, pipe_rx_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto cleanup_pipe);

	ret_chk = pthread_join(th_id[0], NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	ret_chk = pthread_join(th_id[1], NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_ASSERT_EQ_CLEANUP("pipe", pipe_tc_chk, OK, goto cleanup_pipe);

	TC_SUCCESS_RESULT();

cleanup_pipe:
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
#endif

/****************************************************************************
 * Name: libc_unistd
 ****************************************************************************/

int libc_unistd_main(void)
{
#ifndef CONFIG_DISABLE_ENVIRON
	tc_libc_unistd_access();
	tc_libc_unistd_chdir_getcwd();
#endif
	tc_libc_unistd_getopt();
#ifdef CONFIG_PIPES
	tc_libc_unistd_pipe();
#endif
#ifndef CONFIG_DISABLE_SIGNALS
	tc_libc_unistd_sleep();
	tc_libc_unistd_usleep();
#endif

	return 0;
}
