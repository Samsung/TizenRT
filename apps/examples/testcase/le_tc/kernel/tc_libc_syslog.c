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

/// @file tc_libc_syslog.c

/// @brief Test Case Example for Syslog API
#include <tinyara/config.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include "tc_internal.h"

#define PRIORITY_NUM 8
#define MAX_SYSLOG_MSG 32
#define USEC_100 100

int g_prioidx[PRIORITY_NUM] = { LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG };
const char *g_priostr[PRIORITY_NUM] = { "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Information", "Debug" };

char g_syslogmsg[MAX_SYSLOG_MSG + 1];

/**
 * @fn                  :tc_libc_syslog_setlogmask
 * @brief               :set syslog logging level. It is used with bit operation macros LOG_MASK, LOG_UPTO
 * @scenario            :set logging level
 * API's covered        :setlogmask, syslog
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_libc_syslog_setlogmask(void)
{
	int oldmask, newmask, mask_chk;
	setlogmask(LOG_ALL);

	/* The initial value of logmask is LOG_ALL, 0xff */

	oldmask = LOG_ALL;
	newmask = LOG_MASK(LOG_WARNING);
	mask_chk = -1;

	/* It sets the logmask and returns the previous mask. */

	mask_chk = setlogmask(newmask);
	TC_ASSERT_EQ("setlogmask", oldmask, mask_chk);

	oldmask = newmask;
	newmask = LOG_UPTO(LOG_INFO);

	mask_chk = setlogmask(newmask);
	TC_ASSERT_EQ("setlogmask", oldmask, mask_chk);
	setlogmask(LOG_ALL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_syslog_syslog
 * @brief               :Print log messages corresponding with logging level.
 * @scenario            :Compare values between length of log messages and returned value by syslog
 * API's covered        :syslog
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_libc_syslog_syslog(void)
{
	int ret_chk;
	int i;

	for (i = 0; i < PRIORITY_NUM; i++) {
		snprintf(g_syslogmsg, MAX_SYSLOG_MSG, "%s message\n", g_priostr[i]);
		usleep(USEC_100);
		ret_chk = syslog(g_prioidx[i], g_syslogmsg);
		TC_ASSERT_EQ("syslog", ret_chk, strlen(g_syslogmsg));
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :lowvsyslogFunc
* @description          :function for tc_libc_syslog_vsyslog
* @return               :int
*/
static int vsyslogFunc(int priority, FAR const char *msg, ...)
{
	va_list ap;
	int ret;

	va_start(ap, msg);
	ret = vsyslog(priority, msg, ap);
	va_end(ap);

	return ret;
}

/**
 * @fn                  :tc_libc_syslog_vsyslog
 * @brief               :Print log messages corresponding with logging level.
 * @scenario            :Compare values between length of log messages and returned value by vsyslog
 * API's covered        :vsyslog
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_libc_syslog_vsyslog(void)
{
	int ret_chk;
	int i;

	for (i = 0; i < PRIORITY_NUM; i++) {
		snprintf(g_syslogmsg, MAX_SYSLOG_MSG, "%s message\n", g_priostr[i]);
		usleep(USEC_100);
		ret_chk = vsyslogFunc(g_prioidx[i], g_syslogmsg);
		TC_ASSERT_EQ("vsyslog", ret_chk, strlen(g_syslogmsg));
	}

	TC_SUCCESS_RESULT();
}

#if defined(CONFIG_ARCH_LOWPUTC) || defined(CONFIG_SYSLOG)
/**
 * @fn                  :tc_libc_syslog_lowsyslog
 * @brief               :Print log messages corresponding with logging level using lower level interface.
 * @scenario            :Compare values between length of log messages and returned value by lowsyslog
 * API's covered        :lowsyslog
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_libc_syslog_lowsyslog(void)
{
	int ret_chk;
	int i;

	for (i = 0; i < PRIORITY_NUM; i++) {
		snprintf(g_syslogmsg, MAX_SYSLOG_MSG, "%s message\n", g_priostr[i]);
		usleep(USEC_100);
		ret_chk = lowsyslog(g_prioidx[i], g_syslogmsg);
		TC_ASSERT_EQ("lowsyslog", ret_chk, strlen(g_syslogmsg));
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :lowvsyslogFunc
* @description          :function for tc_libc_syslog_lowvsyslog
* @return               :int
*/
static int lowvsyslogFunc(int priority, FAR const char *msg, ...)
{
	va_list ap;
	int ret;

	va_start(ap, msg);
	ret = lowvsyslog(priority, msg, ap);
	va_end(ap);

	return ret;
}

/**
 * @fn                  :tc_libc_syslog_lowvsyslog
 * @brief               :Print log messages corresponding with logging level using lower level interface.
 * @scenario            :Compare values between of length of log messages and returned value by lowvsyslog
 * API's covered        :lowvsyslog
 * Preconditions        :none
 * Postconditions       :none
 * @return              :void
 */
static void tc_libc_syslog_lowvsyslog(void)
{
	int ret_chk;
	int i;

	for (i = 0; i < PRIORITY_NUM; i++) {
		snprintf(g_syslogmsg, MAX_SYSLOG_MSG, "%s message\n", g_priostr[i]);
		usleep(USEC_100);
		ret_chk = lowvsyslogFunc(g_prioidx[i], g_syslogmsg);
		TC_ASSERT_EQ("lowvsyslog", ret_chk, strlen(g_syslogmsg));
	}

	TC_SUCCESS_RESULT();
}

#endif

/****************************************************************************
 * Name: libc_syslog
 ****************************************************************************/

int libc_syslog_main(void)
{
	tc_libc_syslog_setlogmask();
	tc_libc_syslog_syslog();
	tc_libc_syslog_vsyslog();
#if defined(CONFIG_ARCH_LOWPUTC) || defined(CONFIG_SYSLOG)
	tc_libc_syslog_lowsyslog();
	tc_libc_syslog_lowvsyslog();
#endif
	return 0;
}
