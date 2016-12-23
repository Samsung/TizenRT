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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include "tc_internal.h"

#define PRIORITY_NUM 8
#define MAX_SYSLOG_MSG 32

int g_prioidx[PRIORITY_NUM] = { LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG };
const char *g_priostr[PRIORITY_NUM] = { "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Information", "Debug" };

char g_syslogmsg[MAX_SYSLOG_MSG];

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

	//The initial value of logmask is LOG_ALL, 0xff
	oldmask = LOG_ALL;
	newmask = LOG_MASK(LOG_WARNING);
	mask_chk = -1;

	//It sets the logmask and returns the previous mask.
	mask_chk = setlogmask(newmask);
	if (oldmask != mask_chk) {
		printf("tc_syslog_setlogmask FAIL, %d, %d\n", oldmask, mask_chk);
		total_fail++;
		RETURN_ERR;
	}

	oldmask = newmask;
	newmask = LOG_UPTO(LOG_INFO);

	mask_chk = setlogmask(newmask);
	if (oldmask != mask_chk) {
		printf("tc_syslog_setlogmask FAIL, %d, %d\n", oldmask, mask_chk);
		total_fail++;
		RETURN_ERR;
	}
	setlogmask(LOG_ALL);
	printf("tc_libc_syslog_setlogmask PASS\n");
	total_pass++;
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
	int i;
	int ret;

	for (i = 0; i < PRIORITY_NUM; i++) {
		sprintf(g_syslogmsg, "%s message\n", g_priostr[i]);
		ret = syslog(g_prioidx[i], g_syslogmsg);
		if (ret != strlen(g_syslogmsg)) {
			printf("tc_libc_syslog_syslog FAIL\n");
			total_fail++;
			RETURN_ERR;
		}
	}

	printf("tc_libc_syslog_syslog PASS\n");
	total_pass++;
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
	int i;
	int ret;

	for (i = 0; i < PRIORITY_NUM; i++) {
		sprintf(g_syslogmsg, "%s message\n", g_priostr[i]);
		ret = vsyslogFunc(g_prioidx[i], g_syslogmsg);
		if (ret != strlen(g_syslogmsg)) {
			printf("tc_libc_syslog_vsyslog FAIL\n");
			total_fail++;
			RETURN_ERR;
		}
	}
	printf("tc_libc_syslog_vsyslog PASS\n");
	total_pass++;
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
	int i;
	int ret;

	for (i = 0; i < PRIORITY_NUM; i++) {
		sprintf(g_syslogmsg, "%s message\n", g_priostr[i]);
		ret = lowsyslog(g_prioidx[i], g_syslogmsg);
		if (ret != strlen(g_syslogmsg)) {
			printf("tc_libc_syslog_lowsyslog FAIL\n");
			total_fail++;
			RETURN_ERR;
		}
	}
	printf("tc_libc_syslog_lowsyslog PASS\n");
	total_pass++;
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
	int i;
	int ret;

	for (i = 0; i < PRIORITY_NUM; i++) {
		sprintf(g_syslogmsg, "%s message\n", g_priostr[i]);
		ret = lowvsyslogFunc(g_prioidx[i], g_syslogmsg);
		if (ret != strlen(g_syslogmsg)) {
			printf("tc_libc_syslog_lowvsyslog FAIL\n");
			total_fail++;
			RETURN_ERR;
		}
	}
	printf("tc_libc_syslog_lowvsyslog PASS\n");
	total_pass++;
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
