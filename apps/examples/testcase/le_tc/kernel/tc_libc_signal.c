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

/// @file libc_signal.c
/// @brief Test Case Example for Libc Signal API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <signal.h>
#include "tc_internal.h"

#define SIG1  SIGUSR1
#define SIG2  SIGUSR2
#define TRUE  1
#define FALSE 0

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
	TC_ASSERT_EQ("sigismember", ret_chk, TRUE);

	ret_chk = sigismember(&sigset, SIG2);
	TC_ASSERT_EQ("sigismember", ret_chk, FALSE);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_signal
 ****************************************************************************/

int libc_signal_main(void)
{
	tc_libc_signal_sigemptyset();
	tc_libc_signal_sigfillset();
	tc_libc_signal_sigaddset();
	tc_libc_signal_sigdelset();
	tc_libc_signal_sigismember();

	return 0;
}
