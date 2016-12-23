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

/// @file libc_unistd.c
/// @brief Test Case Example for Libc Unistd API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "tc_internal.h"

#define ARG_3 3
#define SEC_3 3

#define USLEEP_INTERVAL 3000000
#define BUFFSIZE 32
#define MSG_SIZE 30

sem_t pipe_sem;
int pipe_tc_chk;
int pipe_fd[2];
const char msg[MSG_SIZE + 1] = "012345678901234567890123456789";
char pipe_buf[MSG_SIZE + 1];

static int pipe_tx_func(int argc, char *argv[])
{
	int tx_iter;
	for (tx_iter = 0; tx_iter < 10; tx_iter++) {
		write(pipe_fd[1], msg, MSG_SIZE);
	}
	return 0;
}

static int pipe_rx_func(int argc, char *argv[])
{
	int rx_iter;
	for (rx_iter = 0; rx_iter < 10; rx_iter++) {
		read(pipe_fd[0], pipe_buf, MSG_SIZE);
		if (strcmp(pipe_buf, msg) != 0) {
			pipe_tc_chk = ERROR;
		}
	}
	sem_post(&pipe_sem);

	return 0;
}

/**
* @fn                   :tc_libc_unistd_chdir
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
	if (ret_chk != OK) {
		printf("tc_libc_unistd_chdir_getcwd FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	cwd = getcwd(buff, BUFFSIZE);
	if (strcmp(directory, cwd) != OK) {
		printf("tc_libc_unistd_chdir_getcwd FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_unistd_chdir_getcwd PASS\n");
	total_pass++;
}

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
	int argc = ARG_3;
	char *sz_argv[] = { "-a", "-a", "-b" };

	optind = -1;
	while ((ret = getopt(argc, sz_argv, "ab")) != ERROR)
		switch (ret) {
		case 'a':
			flag_a = 1;
			break;
		case 'b':
			flag_b = 1;
			break;
		default:
			break;
		}
	if (flag_a != 1 || flag_b != 1) {
		printf("tc_libc_unistd_getopt FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_unistd_getopt PASS \n");
	total_pass++;
}

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
	if (ret_chk != OK) {
		printf("tc_libc_unistd_sleep FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	clock_gettime(CLOCK_REALTIME, &st_final_time);
	if ((st_final_time.tv_sec - st_init_time.tv_sec) != SEC_3) {
		printf("tc_libc_unistd_sleep FAIL st_final_time: %lld, st_init_time: %lld \n", st_final_time.tv_sec, st_init_time.tv_sec);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_unistd_sleep PASS \n");
	total_pass++;
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
	if (ret_chk != OK) {
		printf("tc_libc_unistd_chdir FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	clock_gettime(CLOCK_REALTIME, &st_final_time);
	if ((st_final_time.tv_sec - st_init_time.tv_sec) != SEC_3) {
		printf("tc_libc_unistd_usleep FAIL, st_final_time: %lld, st_init_time: %lld \n", st_final_time.tv_sec, st_init_time.tv_sec);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_libc_unistd_usleep PASS\n");
	total_pass++;
}

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
	int pid[2];

	if (pipe(pipe_fd) < 0) {
		printf("tc_libc_unistd_pipe FAIL: pipe error\n");
		total_fail++;
		RETURN_ERR;
	}

	sem_init(&pipe_sem, 0, 0);
	pid[0] = task_create("tx", 99, 1024, pipe_tx_func, NULL);
	if (pid[0] < 0) {
		printf("tc_libc_unistd_pipe FAIL: tx task creation fail\n");
		total_fail++;
		goto close_pipe;
	}

	pid[1] = task_create("rx", 99, 1024, pipe_rx_func, NULL);
	if (pid[1] < 0) {
		printf("tc_libc_unistd_pipe FAIL: rx task creation fail\n");
		total_fail++;
		goto close_pipe;
	}

	sem_wait(&pipe_sem);

	if (pipe_tc_chk == ERROR) {
		printf("tc_libc_unistd_pipe FAIL: tx-rx data not matched\n");
		total_fail++;
		goto close_pipe;
	}

	printf("tc_libc_unistd_pipe PASS\n");
	total_pass++;

close_pipe:

	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

/****************************************************************************
 * Name: libc_unistd
 ****************************************************************************/

int libc_unistd_main(void)
{
	tc_libc_unistd_chdir_getcwd();
	tc_libc_unistd_getopt();
	tc_libc_unistd_sleep();
	tc_libc_unistd_usleep();
	tc_libc_unistd_pipe();

	return 0;
}
