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
#include <errno.h>
#include <signal.h>
#include "tc_internal.h"

#define SIGINT  2				/* interrupt */
#define SIGQUIT 3				/* quit */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
* @fn                   :tc_libc_signal_sigaddset_sigdelset
* @brief                :sigaddset() adds the signal specified by the signo to the signal set pointed to by set.\
*                        sigdelset() deletes the signal specified by signo from the signal set pointed to by set.
* @scenario             :The sigaddset() adds the signal specified by the signo\
*                        sigdelset() deletes the signal specified by signo and  to verify we use sigismember.
* API's covered         :sigaddset, sigdelset
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/
static void tc_libc_signal_sigaddset_sigdelset(void)
{
	int ret_chk = 0;
	sigset_t sigset;

	ret_chk = sigemptyset(&sigset);
	TC_ASSERT_EQ("sigemptyset", ret_chk, OK);

	ret_chk = sigaddset(&sigset, SIGQUIT);
	TC_ASSERT_EQ("sigaddset", ret_chk, OK);

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_EQ("sigismember", ret_chk, 1);

	ret_chk = sigdelset(&sigset, SIGQUIT);
	TC_ASSERT_EQ("sigdelset", ret_chk, OK);

	/* should not find that signal as it has already deleted */

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_NEQ("sigismember", ret_chk, 1);
	TC_ASSERT_NEQ("sigismember", ret_chk, ERROR);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_sigemptyset_sigfillset
* @brief                :sigemptyset() and sigfillset() initialize the signal set pointed to by set,\
*                        such that all signals defined are included and excluded respectively.
* @scenario             :sigemptyset() and sigfillset() initialize the signal set pointed to by set,\
*                        such that all signals defined are included and excluded respectively. To verify we use sigismember.
* API's covered         :sigemptyset, sigfillset
* Preconditions         :none
* Preconditions         :none
* @return               :void
*/
static void tc_libc_signal_sigemptyset_sigfillset(void)
{
	int ret_chk = 0;
	sigset_t sigset;

	ret_chk = sigfillset(&sigset);
	TC_ASSERT_EQ("sigfillset", ret_chk, OK);

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_EQ("sigismember", ret_chk, 1);

	ret_chk = sigismember(&sigset, SIGINT);
	TC_ASSERT_EQ("sigismember", ret_chk, 1);

	ret_chk = sigemptyset(&sigset);
	TC_ASSERT_EQ("sigemptyset", ret_chk, OK);

	/* sigismember should not find any signal as all signals are set empty */

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_NEQ("sigemptyset", ret_chk, 1);
	TC_ASSERT_NEQ("sigismember", ret_chk, ERROR);

	/* sigfillset will fill all signals to signal set */

	ret_chk = sigfillset(&sigset);
	TC_ASSERT_EQ("sigfillset", ret_chk, OK);

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_EQ("sigismember", ret_chk, 1);

	ret_chk = sigismember(&sigset, SIGINT);
	TC_ASSERT_EQ("sigismember", ret_chk, 1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_signal_sigismember
* @brief                :sigismember() function shall test whether the signal specified by signo is a member of the set pointed to by set.
* @scenario             :empty the sigset using sigemptyset() ,then verify it with sigismember() function which\
*                        shall test whether the signal specified by signo is a member
* API's covered         :sigismember()
* Preconditions         :sigemptyset()
* Preconditions         :none
* @return               :void
*/

static void tc_libc_signal_sigismember(void)
{
	int ret_chk = 0;
	sigset_t sigset;

	ret_chk = sigemptyset(&sigset);
	TC_ASSERT_EQ("sigemptyset", ret_chk, OK);

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_NEQ("sigismember", ret_chk, 1);
	TC_ASSERT_NEQ("sigismember", ret_chk, ERROR);

	/* sigfillset will fill all signals to signal set */

	ret_chk = sigfillset(&sigset);
	TC_ASSERT_EQ("sigfillset", ret_chk, OK);

	/* sigismember will find the signals which were filled by sigfillset */

	ret_chk = sigismember(&sigset, SIGQUIT);
	TC_ASSERT_EQ("sigismember", ret_chk, 1);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_signal
 ****************************************************************************/

int libc_signal_main(void)
{
	tc_libc_signal_sigaddset_sigdelset();
	tc_libc_signal_sigemptyset_sigfillset();
	tc_libc_signal_sigismember();

	return 0;
}
