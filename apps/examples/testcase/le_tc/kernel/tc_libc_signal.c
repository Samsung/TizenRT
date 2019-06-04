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

/// @file tc_libc_signal.c
/// @brief Test Case Example for Libc Signal API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include "tc_internal.h"

#define SIG1  SIGUSR1
#define SIG2  SIGUSR2

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
* @fn                   :tc_libc_signal_sigemptyset
* @brief                :tc_libc_signal_sigemptyset test sigemptyset function.
* @scenario             :sigemptyset() initialize the signal set as NULL_SIGNAL_SET.
* API's covered         :sigemptyset
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/
static void tc_libc_signal_sigemptyset(void)
{
	int ret_chk;
	sigset_t sigset = ALL_SIGNAL_SET;

	ret_chk = sigemptyset(&sigset);
	TC_ASSERT_EQ("sigemptyset", ret_chk, OK);
	TC_ASSERT_EQ("sigemptyset", sigset, NULL_SIGNAL_SET);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_sigfillset
* @brief                :tc_libc_signal_sigfillset test sigfillset function.
* @scenario             :sigfillset() initialize the signal set as NULL_SIGNAL_SET.
* API's covered         :sigfillset
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/
static void tc_libc_signal_sigfillset(void)
{
	int ret_chk;
	sigset_t sigset = NULL_SIGNAL_SET;

	ret_chk = sigfillset(&sigset);
	TC_ASSERT_EQ("sigfillset", ret_chk, OK);
	TC_ASSERT_EQ("sigfillset", sigset, ALL_SIGNAL_SET);

	TC_SUCCESS_RESULT();
}



/**
* @fn                   :tc_libc_signal_sigaddset
* @brief                :tc_libc_signal_sigaddset test sigaddset function
* @scenario             :If signo > MAX_SIGNO, it returns ERROR.
*                        Else, it return OK and set signal.
* API's covered         :sigaddset
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/
static void tc_libc_signal_sigaddset(void)
{
	int ret_chk;
	sigset_t sigset = NULL_SIGNAL_SET;
	sigset_t sigset_expected = NULL_SIGNAL_SET | SIGNO2SET(SIG1);

	ret_chk = sigaddset(&sigset, MAX_SIGNO + 1);
	TC_ASSERT_EQ("sigaddset", ret_chk, ERROR);

	ret_chk = sigaddset(&sigset, SIG1);
	TC_ASSERT_EQ("sigaddset", ret_chk, OK);
	TC_ASSERT_EQ("sigaddset", sigset, sigset_expected);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_sigdelset
* @brief                :tc_libc_signal_sigdelset test sigdelset function
* @scenario             :If signo > MAX_SIGNO, it returns ERROR.
*                        Else, it return OK and set signal.
* API's covered         :sigdelset
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/
static void tc_libc_signal_sigdelset(void)
{
	int ret_chk;
	sigset_t sigset = ALL_SIGNAL_SET;
	sigset_t sigset_expected = ALL_SIGNAL_SET & (~(SIGNO2SET(SIG1)));

	ret_chk = sigdelset(&sigset, MAX_SIGNO + 1);
	TC_ASSERT_EQ("sigdelset", ret_chk, ERROR);

	ret_chk = sigdelset(&sigset, SIG1);
	TC_ASSERT_EQ("sigdelset", ret_chk, OK);
	TC_ASSERT_EQ("sigdelset", sigset, sigset_expected);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_sigismember
* @brief                :tc_libc_signal_sigismember test sigismember function
* @scenario             :If signo > MAX_SIGNO, it returns ERROR.
*                        Else if signo is the member of sigset, it returns 1.
*                        Else it returns 0
* API's covered         :sigismember()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_sigismember(void)
{
	int ret_chk;
	sigset_t sigset = NULL_SIGNAL_SET | SIGNO2SET(SIG1);

	ret_chk = sigismember(&sigset, MAX_SIGNO + 1);
	TC_ASSERT_EQ("sigismember", ret_chk, ERROR);

	ret_chk = sigismember(&sigset, SIG1);
	TC_ASSERT_EQ("sigismember", ret_chk, true);

	ret_chk = sigismember(&sigset, SIG2);
	TC_ASSERT_EQ("sigismember", ret_chk, false);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_sigignore
* @brief                :tc_libc_signal_sigignore test sigignore function
* @scenario             :If sig > MAX_SIGNO, it returns ERROR.
*                        Else if sig is the member of sigset, it will set the disposition of sig to SIG_IGN & return 0
* API's covered         :sigignore()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_sigignore(void)
{
	int ret_chk;

	ret_chk = sigignore(MAX_SIGNO + 1);
	TC_ASSERT_EQ("sigignore", ret_chk, ERROR);

	ret_chk = sigignore(SIG1);
	TC_ASSERT_EQ("sigignore", ret_chk, OK);

	ret_chk = sigignore(SIG2);
	TC_ASSERT_EQ("sigignore", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_DISABLE_POSIX_TIMERS
/**
* @fn                   :tc_libc_signal_sigpause
* @brief                :tc_libc_signal_sigpause test sigpause function
* @scenario             :If sig > MAX_SIGNO, it returns ERROR.
*                        Else if a sigset member, it will remove sig from the calling process' signal mask & suspend the calling process until a signal is received
*                        It will restore the process' signal mask to its original state before returning
* API's covered         :sigpause()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_sigpause(void)
{
	int ret_chk;
	timer_t timer_id;
	clockid_t clockid = 0; /*CLOCK_REALTIME*/
	struct sigevent st_sigevent;
	struct itimerspec st_timer_spec_val;

	ret_chk = sigpause(MAX_SIGNO + 1);
	TC_ASSERT_EQ("sigpause", ret_chk, ERROR);

	/* Set and enable alarm */
	st_sigevent.sigev_notify = 1; /*SIGEV_SIGNAL*/
	st_sigevent.sigev_signo = SIGRTMIN;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);

	st_timer_spec_val.it_value.tv_sec = 2;

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_val, NULL);
	TC_ASSERT_EQ_CLEANUP("timer_settime", ret_chk, OK, timer_delete(timer_id));

	printf("sigpause test : Before Pause\n");
	ret_chk = sigpause(SIG1);
	TC_ASSERT_EQ_CLEANUP("sigpause", ret_chk, ERROR, timer_delete(timer_id));

	printf("sigpause test : 2 sec After Pause\n");

	timer_delete(timer_id);
	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_libc_signal_sigset
* @brief                :tc_libc_signal_sigset test sigset function
* @scenario             :If sig is the member of sigset, it will modify signal dispositions
*                        It will return SIG_HOLD if signal had been blocked & the signal's previous disposition if it had not been blocked else SIG_ERR will be returned
* API's covered         :sigset()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_sigset(void)
{
	int ret_chk;

	ret_chk = (int)sigset(MAX_SIGNO + 1, SIG_DFL);
	TC_ASSERT_EQ("sigset", ret_chk, ERROR);

	ret_chk = (int)sigset(SIG1, SIG_DFL);
	TC_ASSERT_EQ("sigset", ret_chk, OK);

	ret_chk = (int)sigset(SIG2, SIG_IGN);
	TC_ASSERT_EQ("sigset", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_signal
* @brief                :tc_libc_signal_signal test signal function
* @scenario             :If sig is the member of sigset, it will modify signal dispositions
*                        It will return the previous disposition of the signal handling else SIG_ERR will be returned
* API's covered         :signal()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_signal(void)
{
	int ret_chk;

	ret_chk = (int)signal(MAX_SIGNO + 1, SIG_DFL);
	TC_ASSERT_EQ("signal", ret_chk, ERROR);

	ret_chk = (int)signal(SIG1, SIG_DFL);
	TC_ASSERT_EQ("signal", ret_chk, OK);

	ret_chk = (int)signal(SIG2, SIG_IGN);
	TC_ASSERT_EQ("signal", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_raise
* @brief                :tc_libc_signal_raise test sigraise function
* @scenario             :If sig > MAX_SIGNO, it returns ERROR.
*                        Else if sig is the member of sigset, it will send the signal sig to the executing thread or process
* API's covered         :raise()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_raise(void)
{
	int ret_chk;

	ret_chk = raise(MAX_SIGNO + 1);
	TC_ASSERT_EQ("raise", ret_chk, ERROR);

	ret_chk = raise(SIG1);
	TC_ASSERT_EQ("raise", ret_chk, OK);

	ret_chk = raise(SIG2);
	TC_ASSERT_EQ("raise", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_psignal
* @brief                :tc_libc_signal_psignal test sigpsignal function
* @scenario             :If pinfo == NULL, it set errno EINVAL.
*                        Else if message is not a null pointer and is not the empty string, the string pointed to by the message argument will be written, followed by a colon and a space.
*                        Then the signal description string associated with signum will be written, followed by a newline.
* API's covered         :psiginfo()
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_psignal(void)
{
	siginfo_t siginfo;
	siginfo.si_signo = SIG1;

	psiginfo(NULL, NULL);
	TC_ASSERT_EQ("psignal", get_errno(), EINVAL);

	/* "User-defined signal 1 (POSIX): SIGUSR1" will be printed to stderr */
	psiginfo(&siginfo, "User-defined signal 1 (POSIX)");

	/* "SIGUSR1" will be printed to stderr */
	psiginfo(&siginfo, NULL);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_signal
 ****************************************************************************/

int libc_signal_main(void)
{
	tc_libc_signal_psignal();
	tc_libc_signal_sigemptyset();
	tc_libc_signal_sigfillset();
	tc_libc_signal_sigaddset();
	tc_libc_signal_sigdelset();
	tc_libc_signal_sigismember();
	tc_libc_signal_sigignore();
	tc_libc_signal_raise();
	tc_libc_signal_sigset();
	tc_libc_signal_signal();
#ifndef CONFIG_DISABLE_POSIX_TIMERS
	tc_libc_signal_sigpause();
#endif

	return 0;
}
