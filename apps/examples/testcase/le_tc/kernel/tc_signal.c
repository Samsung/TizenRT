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

/// @file signal.c
/// @brief Test Case Example for Signal API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include "../../os/kernel/signal/signal.h"
#include "tc_internal.h"

#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#ifndef SIGCHLD
#define SIGCHLD 4
#endif
#define SIGKILL 9
#define SIGTERM 15
#define SEC_1 1
#define SEC_2 2
#define SEC_3 3
#define SEC_5 5
#define VAL_100 100

pid_t g_sig_pid;

static bool g_sig_handle = false;

/**
* @fn                   :sigquit_handler
* @brief                :utility function used in tc_signal_sig_pending_procmask_emptyset_addset
* @return               :int
*/
static void sigquit_handler(int signo)
{
	g_sig_handle = true;
	TC_ASSERT_EQ("sigquit_handler", signo, SIGQUIT);
}

/**
* @fn                   :sigint_handler
* @brief                :utility function used in tc_signal_sigqueue
* @return               :void
*/
static void sigint_handler(int signo, siginfo_t *info, void *ctx)
{
	g_sig_handle = true;
	TC_ASSERT_EQ("sigint handler", info->si_value.sival_int, VAL_100);
}

/**
* @fn                   :sigusr1_func
* @brief                :utility function used in tc_signal_sigwaitinfo
* @return               :void*
*/
static void *sigusr1_func(void *arg)
{
	unsigned int remain = SEC_2;
	while (remain > 0) {
		remain = sleep(remain);
	}
	/* Kill sends the signal */
	kill(g_sig_pid, SIGUSR1);
	return NULL;
}

/**
* @fn                   :sigusr2_func
* @brief                :utility function used in tc_signal_sigwaitinfo
* @return               :void*
*/
static void *sigusr2_func(void *arg)
{
	unsigned int remain = SEC_5;
	while (remain > 0) {
		remain = sleep(remain);
	}

	/* Kill sends the signal */
	kill(g_sig_pid, SIGUSR2);
	return NULL;
}

/**
* @fn                   :sigaction_handler
* @brief                :utility function used in tc_signal_sigaction
* @return               :void
*/
void sigaction_handler(int signo)
{
}

/**
* @fn                   :tc_signal_sigwaitinfo
* @brief                :Create a task with called function sigusr1_func.
* @Scenario             :Create a task with called function sigusr1_func.
* API's covered         :sigwaitinfo
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_signal_sigwaitinfo(void)
{
	pthread_t pid;
	struct siginfo value;
	sigset_t sigset;
	int ret_chk;

	g_sig_pid = getpid();

	ret_chk = pthread_create(&pid, NULL, sigusr1_func, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	/* Wait for a signal */

	(void)sigemptyset(&sigset);
	(void)sigaddset(&sigset, SIGUSR1);
	value.si_value.sival_int = -1;
	ret_chk = sigwaitinfo(&sigset, &value);
	TC_ASSERT_EQ_CLEANUP("sigwaitinfo", ret_chk, SIGUSR1, goto errout);
	TC_ASSERT_GEQ_CLEANUP("sigwaitinfo", value.si_value.sival_int, 0, goto errout);

	TC_SUCCESS_RESULT();
errout:
	pthread_join(pid, NULL);
}

/**
* @fn                   :tc_signal_sigaction
* @brief                :This function calls sigaction with signal SIGINT.
* @Scenario             :This function calls sigaction with signal SIGINT.
* API's covered         :sigaction
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_signal_sigaction(void)
{
	int ret_chk = ERROR;
	struct sigaction st_act;
	struct sigaction st_oact;
	FAR sigactq_t *sigact_before;
	FAR sigactq_t *sigact_after;

	/* save orginal action */
	sigact_before = sig_findaction(sched_self(), SIGINT);

	st_act.sa_handler = sigaction_handler;
	st_act.sa_flags = 0;
	sigemptyset(&st_act.sa_mask);

	ret_chk = sigaction(SIGINT, &st_act, &st_oact);
	TC_ASSERT_GEQ("sigaction", ret_chk, 0);

	st_act.sa_handler = NULL;

	ret_chk = sigaction(SIGINT, &st_oact, &st_act);
	TC_ASSERT_GEQ("sigaction", ret_chk, 0);

	TC_ASSERT_EQ("sigaction", st_act.sa_handler, sigaction_handler);

	/* make sure action is not changed */
	sigact_after = sig_findaction(sched_self(), SIGINT);
	TC_ASSERT_EQ("sig_findaction", sigact_before, sigact_after);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_signal_kill
* @brief                :This Tc kill the system call.
* @Scenario             :This Tc kill the system call.
* API's covered         :getpid, kill
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_signal_kill(void)
{
	pid_t pid;
	int ret_chk = ERROR;

	pid = getpid();

	if (pid != 0) {
		ret_chk = kill(pid, SIGHUP);
		sleep(SEC_1);
		TC_ASSERT_NEQ("kill", ret_chk, ERROR);

		ret_chk = kill(pid, SIGINT);
		sleep(SEC_1);
		TC_ASSERT_NEQ("kill", ret_chk, ERROR);

		ret_chk = kill(pid, SIGQUIT);
		sleep(SEC_1);
		TC_ASSERT_NEQ("kill", ret_chk, ERROR);
	}
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_signal_nanosleep
* @brief                :Suspend the process for the time specified.
* @Scenario             :Suspend the process for the time specified.
* API's covered         :nanosleep
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_signal_nanosleep(void)
{
	struct timespec st_timespec;
	struct timespec st_init_timespec;
	struct timespec st_final_timespec;
	clockid_t clock_id = CLOCK_REALTIME;
	int ret_chk;

	st_timespec.tv_sec = SEC_2;
	st_timespec.tv_nsec = 0;

	/* Set start time. do while statement prevents nanotime carry */

	do {
		clock_gettime(clock_id, &st_init_timespec);
	} while (st_init_timespec.tv_nsec > 900000000);

	ret_chk = nanosleep(&st_timespec, NULL);
	TC_ASSERT_NEQ("nanosleep", ret_chk, ERROR);

	clock_gettime(clock_id, &st_final_timespec);
	TC_ASSERT_EQ("clock_gettime", st_final_timespec.tv_sec - st_init_timespec.tv_sec, st_timespec.tv_sec);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_signal_pause
* @brief                :Create two different signal sender tasks, then pause.
* @Scenario             :Create two different signal sender tasks, then pause.
* API's covered         :pause
* Preconditions         :sigprocmask
* Postconditions        :none
* @return               :void
*/
static void tc_signal_pause(void)
{
	pthread_t pid1;
	pthread_t pid2;
	sigset_t saved;
	sigset_t newmask;
	struct timespec st_init_timespec;
	struct timespec st_final_timespec;
	clockid_t clock_id = CLOCK_REALTIME;
	int ret_chk;

	g_sig_pid = getpid();

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	/* Save the current sigprocmask */

	ret_chk = sigprocmask(SIG_SETMASK, &newmask, &saved);
	TC_ASSERT_EQ("sigprocmask", ret_chk, OK);

	clock_gettime(clock_id, &st_init_timespec);
	ret_chk = pthread_create(&pid1, NULL, sigusr1_func, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);
	ret_chk = pthread_create(&pid2, NULL, sigusr2_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto errout);

	/* Wait for a signal */

	ret_chk = pause();
	TC_ASSERT_EQ_CLEANUP("pause", ret_chk, ERROR, goto errout);
	TC_ASSERT_EQ_CLEANUP("pause", get_errno(), EINTR, goto errout);

	clock_gettime(clock_id, &st_final_timespec);
	if (st_final_timespec.tv_sec - st_init_timespec.tv_sec < SEC_5) {
		ret_chk = sigprocmask(SIG_SETMASK, &saved, NULL);
		TC_ASSERT_EQ_CLEANUP("sigprocmask", ret_chk, OK, goto errout);
	}

	/* Restore sigprocmask */

	ret_chk = sigprocmask(SIG_SETMASK, &saved, NULL);
	TC_ASSERT_EQ_CLEANUP("sigprocmask", ret_chk, OK, goto errout);

	TC_SUCCESS_RESULT();
errout:
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
}

/**
* @fn                   :tc_signal_sigsuspend
* @brief                :Create two different signal sender tasks, then sigsuspend.
* @Scenario             :Create two different signal sender tasks, then sigsuspend.
* API's covered         :sigsuspend
* Preconditions         :sigprocmask
* Postconditions        :none
* @return               :void
*/
static void tc_signal_sigsuspend(void)
{
	pthread_t pid1;
	pthread_t pid2;
	sigset_t saved;
	sigset_t newmask;
	struct timespec st_init_timespec;
	struct timespec st_final_timespec;
	clockid_t clock_id = CLOCK_REALTIME;
	int ret_chk;

	g_sig_pid = getpid();

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGCHLD);
	/* Save the current sigprocmask */

	ret_chk = sigprocmask(SIG_SETMASK, &newmask, &saved);
	TC_ASSERT_EQ("sigprocmask", ret_chk, OK);

	clock_gettime(clock_id, &st_init_timespec);
	ret_chk = pthread_create(&pid1, NULL, sigusr1_func, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);
	ret_chk = pthread_create(&pid2, NULL, sigusr2_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto errout);

	/* Wait for a signal */

	ret_chk = sigsuspend(&newmask);
	TC_ASSERT_EQ_CLEANUP("sigsuspend", ret_chk, ERROR, goto errout);

	clock_gettime(clock_id, &st_final_timespec);
	TC_ASSERT_GEQ_CLEANUP("clock_gettime", st_final_timespec.tv_sec - st_init_timespec.tv_sec, SEC_5, goto errout);

	TC_SUCCESS_RESULT();

errout:
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	/* Restore sigprocmask */

	sigprocmask(SIG_SETMASK, &saved, NULL);
}

/**
* @fn                   :tc_signal_sig_pending_procmask_emptyset_addset
* @brief                :Change the signal mask for the calling thread and retrieve the\
*                        signals that are pending for delivery by calling sigpending.
* @Scenario             :Change the signal mask for the calling thread and retrieve the\
*                        signals that are pending for delivery by calling sigpending.
* API's covered         :sigprocmask,sigemptyset,sigaddset,sigpending
* Preconditions         :sigaction
* Postconditions        :none
* @return               :void
*/
static void tc_signal_sig_pending_procmask_emptyset_addset(void)
{
	int ret_chk = ERROR;
	pid_t pid = getpid();

	sigset_t st_newmask;
	sigset_t st_oldmask;
	sigset_t st_pendmask;
	struct timespec st_timespec;
	struct sigaction st_act;
	struct sigaction st_oact;

	g_sig_handle = false;

	st_timespec.tv_sec = SEC_5;
	st_timespec.tv_nsec = 0;

	memset(&st_act, 0, sizeof(st_act));
	st_act.sa_handler = sigquit_handler;

	TC_ASSERT("sigaction", !(sigaction(SIGQUIT, &st_act, &st_oact)));

	sigemptyset(&st_newmask);
	sigaddset(&st_newmask, SIGQUIT);

	ret_chk = sigprocmask(SIG_BLOCK, &st_newmask, &st_oldmask);
	TC_ASSERT_GEQ("sigprocmask", ret_chk, 0);

	nanosleep(&st_timespec, NULL);

	kill(pid, SIGQUIT);
	/* to call the handler function for verifying the sigpromask */
	ret_chk = sigprocmask(SIG_SETMASK, &st_oldmask, NULL);
	TC_ASSERT_GEQ("sigprocmask", ret_chk, 0);

	st_timespec.tv_sec = 1;

	nanosleep(&st_timespec, NULL);

	ret_chk = sigprocmask(SIG_UNBLOCK, &st_oldmask, NULL);
	TC_ASSERT_GEQ("sigprocmask", ret_chk, 0);

	ret_chk = sigpending(&st_pendmask);
	TC_ASSERT_GEQ("sigpending", ret_chk, 0);

	nanosleep(&st_timespec, NULL);

	TC_ASSERT("nanosleep", g_sig_handle);

	TC_ASSERT("sigaction", !sigaction(SIGQUIT, &st_oact, NULL));

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_signal_sigqueue
* @brief                :Sends the signal SIGINT to the process whose PID is fetched by getpid.
* @Scenario             :Sends the signal SIGINT to the process whose PID is fetched by getpid.
* API's covered         :sigqueue,getpid
* Preconditions         :sigaction
* Postconditions        :none
* @return               :void
*/

static void tc_signal_sigqueue(void)
{
	struct sigaction st_act;
	struct sigaction st_oact;
	g_sig_handle = false;

	st_act.sa_sigaction = sigint_handler;
	sigemptyset(&st_act.sa_mask);
	/*information transfer switch */
	st_act.sa_flags = SA_SIGINFO;
	TC_ASSERT_NEQ("sigaction", sigaction(SIGINT, &st_act, &st_oact), ERROR);

	sleep(SEC_2);

	union sigval mysigval;
	mysigval.sival_int = VAL_100;
	TC_ASSERT_NEQ("sigqueue", sigqueue(getpid(), SIGINT, mysigval), ERROR);

	sleep(SEC_1);

	TC_ASSERT("sigqueue", g_sig_handle);

	TC_ASSERT_NEQ("sigaction", sigaction(SIGINT, &st_oact, NULL), ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_signal_sigtimedwait
* @brief                :Create a task and  suspends execution of the calling thread until one of the\
*                        signals in set (sigset)is pending.
* @Scenario             :Create a task and  suspends execution of the calling thread until one of the\
*                        signals in set (sigset)is pending.
* API's covered         :task_create, sigtimedwait
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_signal_sigtimedwait(void)
{
	pthread_t pid;
	struct siginfo value;
	sigset_t sigset;
	struct timespec st_timeout;
	struct timespec st_init_timespec;
	struct timespec st_final_timespec;
	clockid_t clock_id = CLOCK_REALTIME;
	int ret_chk;

	g_sig_pid = getpid();
	st_timeout.tv_sec = SEC_5;
	st_timeout.tv_nsec = 0;
	value.si_value.sival_int = -1;

	/* Set signal set */

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);

	ret_chk = pthread_create(&pid, NULL, sigusr1_func, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	clock_gettime(clock_id, &st_init_timespec);
	ret_chk = sigtimedwait(&sigset, &value, &st_timeout);
	TC_ASSERT_NEQ_CLEANUP("sigtimedwait", ret_chk, ERROR, goto errout);

	clock_gettime(clock_id, &st_final_timespec);
	TC_ASSERT_LEQ_CLEANUP("clock_gettime", st_final_timespec.tv_sec - st_init_timespec.tv_sec, SEC_3, goto errout);
	TC_ASSERT_GEQ_CLEANUP("sigtimedwait", value.si_value.sival_int, 0, goto errout);

	TC_SUCCESS_RESULT();
errout:
	pthread_join(pid, NULL);
}

/**
* @fn                   :tc_signal_sighold_sigrelse
* @brief                :set and unset a specific signal
* API's covered         :sighold, sigrelse
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_signal_sighold_sigrelse(void)
{
	sigset_t org_set;
	sigset_t hold_set;
	sigset_t relse_set;
	struct tcb_s *cur_tcb;
	int ret_chk;

	cur_tcb = sched_self();
	org_set = cur_tcb->sigprocmask;

	ret_chk = sighold(SIGUSR1);

	TC_ASSERT_EQ("sighold", ret_chk, OK);

	hold_set = cur_tcb->sigprocmask;
	TC_ASSERT_NEQ("sighold", org_set, hold_set);

	ret_chk = sigrelse(SIGUSR1);
	TC_ASSERT_EQ("sigrelease", ret_chk, OK);

	relse_set = cur_tcb->sigprocmask;
	TC_ASSERT_NEQ("sighold", hold_set, relse_set);
	TC_ASSERT_EQ("Sigrelease", org_set, relse_set);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: signal
 ****************************************************************************/

int signal_main(void)
{
	tc_signal_kill();
	tc_signal_nanosleep();
	tc_signal_pause();
	tc_signal_sig_pending_procmask_emptyset_addset();
	tc_signal_sigaction();
	tc_signal_sigqueue();
	tc_signal_sigsuspend();
	tc_signal_sigtimedwait();
	tc_signal_sigwaitinfo();
	tc_signal_sighold_sigrelse();

	return 0;
}
