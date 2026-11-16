/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

/*
 * Silent Reboot UTC
 *
 * Exercises the user-visible surface of the silent reboot feature: the
 * /dev/silent_reboot driver ioctls and the reboot reason prctl() commands.
 *
 * Safety policy - this feature can reboot the board:
 *   - silent_reboot_unlock() performs an immediate board reset when the
 *     lock count drops to zero while the module is inside its reboot
 *     timezone (00:00-05:00 local time), and SILENTRBIOC_FORCE_REBOOT can
 *     arm the periodic reboot watchdog.  Every testcase that unlocks or
 *     re-arms the watchdog first queries SILENTRBIOC_GETSTATUS and is
 *     skipped (not failed) while reboot_timezone_left == 0, which means
 *     the module is inside the reboot window.  On a freshly booted board
 *     the periodic timer is still armed for DAYS_AFTER_BOOT, so the
 *     guarded testcases normally run.
 *   - utc_silent_reboot_force_rearm_p is additionally skipped when the
 *     reboot timezone begins within 360 seconds, so its 3-second watchdog
 *     can never expire inside the window.
 *   - Every testcase restores the lock count it consumed and never leaves
 *     a written reboot reason behind.
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>

#include <tinyara/silent_reboot.h>
#include <tinyara/fs/ioctl.h>

#include "tc_common.h"

#define SR_DELAY_SEC			5
#define SR_DELAY_WAIT_SEC		7
#define SR_FORCE_SEC			3
#define SR_FORCE_WAIT_SEC		5
#define SR_TZ_MARGIN_SEC		360
/* Seconds in a day; the module defines the same value internally */
#define SR_A_DAY_TO_SEC			86400
#define SR_SEVEN_DAYS_TO_SEC	604800
/* SILENT_RESTART re-arm target is the next midnight plus up to 300s of jitter */
#define SR_TZ_REARM_MAX_SEC		(86400 + 300)

/*
 * Returns true when the module is inside the silent reboot timezone and
 * testcases that may unlock or re-arm the reboot watchdog must be skipped.
 * An unknown state is treated as unsafe.
 */
static bool sr_in_reboot_timezone(int fd)
{
	silent_reboot_status_t st;

	if (ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st) != OK) {
		return true;
	}

	return st.reboot_timezone_left == 0;
}

static int sr_get_count(int fd, int *count)
{
	silent_reboot_status_t st;
	int ret = ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st);

	if (ret == OK) {
		*count = st.lock_count;
	}

	return ret;
}

/**
* @fn                   :utc_silent_reboot_open_p
* @brief                :Open and close the silent reboot driver node
* @Scenario             :The driver registers /dev/silent_reboot at boot
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void utc_silent_reboot_open_p(void)
{
	int fd;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	TC_ASSERT_EQ("close", close(fd), OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_ioctl_n
* @brief                :Unknown ioctl commands are rejected with EINVAL
* @Scenario             :The driver returns -EINVAL for any unknown command
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void utc_silent_reboot_ioctl_n(void)
{
	int fd;
	int ret;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ_CLEANUP("open", fd, OK, close(fd));

	ret = ioctl(fd, SILENTRBIOC_LOCK | 0x4000);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, ERROR, close(fd));
	TC_ASSERT_EQ_CLEANUP("ioctl", errno, EINVAL, close(fd));

	ret = ioctl(fd, 0x1234);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, ERROR, close(fd));
	TC_ASSERT_EQ_CLEANUP("ioctl", errno, EINVAL, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_checkmode_p
* @brief                :Query the boot mode classification
* @Scenario             :SILENTRBIOC_CHECKMODE returns the silent mode flag
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void utc_silent_reboot_checkmode_p(void)
{
	int fd;
	int ret;
	bool mode = false;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	ret = ioctl(fd, SILENTRBIOC_CHECKMODE, (unsigned long)&mode);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	printf("\n silent reboot mode: %s\n", mode ? "silent" : "normal");

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_getstatus_p
* @brief                :Read the silent reboot status
* @Scenario             :SILENTRBIOC_GETSTATUS fills a sane status struct
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void utc_silent_reboot_getstatus_p(void)
{
	int fd;
	int ret;
	silent_reboot_status_t st;

	memset(&st, 0xFF, sizeof(st));

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	ret = ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	/* Periodic reboot timer is armed for at most 7 days + 300s of jitter */
	TC_ASSERT_GEQ("get_status", st.reboot_timezone_left, 0);
	TC_ASSERT_LEQ("get_status", st.reboot_timezone_left, SR_SEVEN_DAYS_TO_SEC + 300);
	TC_ASSERT_GEQ("get_status", st.reboot_delay_left, 0);

	printf("\n lock: %d, timezone left: %d, delay left: %d\n",
			st.lock_count, st.reboot_timezone_left, st.reboot_delay_left);

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_getstatus_n
* @brief                :NULL status pointer is rejected
* @Scenario             :SILENTRBIOC_GETSTATUS with a NULL argument fails
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void utc_silent_reboot_getstatus_n(void)
{
	int fd;
	int ret;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	ret = ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)NULL);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, ERROR, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_delay_n
* @brief                :Non-positive delay values are rejected
* @Scenario             :SILENTRBIOC_DELAY(0) and DELAY(-10) fail before any
*                       timer is touched
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void utc_silent_reboot_delay_n(void)
{
	int fd;
	int ret;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	ret = ioctl(fd, SILENTRBIOC_DELAY, (unsigned long)0);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, ERROR, close(fd));

	ret = ioctl(fd, SILENTRBIOC_DELAY, (unsigned long)(-10));
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, ERROR, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_reboot_reason_p
* @brief                :Reboot reason prctl round trip
* @Scenario             :Clear, write and read the reboot reason.  The value
*                       written before a real reboot only reflects after it,
*                       so the read is asserted as a non-error value, same as
*                       the kernel le_tc task testcase.
* API's covered         :prctl
* Preconditions         :none
* Postconditions        :Reboot reason is cleared again
* @return               :void
*/
static void utc_silent_reboot_reboot_reason_p(void)
{
	int ret;

	ret = prctl(PR_REBOOT_REASON_CLEAR);
	TC_ASSERT_EQ("prctl", ret, OK);

	ret = prctl(PR_REBOOT_REASON_WRITE, 100);
	TC_ASSERT_EQ("prctl", ret, OK);

	ret = prctl(PR_REBOOT_REASON_READ);
	TC_ASSERT_GEQ("prctl", ret, OK);

	ret = prctl(PR_REBOOT_REASON_CLEAR);
	TC_ASSERT_GEQ("prctl", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_lock_unlock_p
* @brief                :Lock and unlock track the lock count
* @Scenario             :Each LOCK increments and each UNLOCK decrements the
*                       module lock count.  Skipped inside the reboot
*                       timezone: an unlock that returns the count to zero
*                       there reboots the board.
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :Lock count restored
* @return               :void
*/
static void utc_silent_reboot_lock_unlock_p(void)
{
	int fd;
	int ret;
	int base;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	if (sr_in_reboot_timezone(fd)) {
		printf("\n[%s] SKIP, inside silent reboot timezone\n", __func__);
		close(fd);
		return;
	}

	TC_ASSERT_EQ_CLEANUP("get_status", sr_get_count(fd, &base), OK, close(fd));

	ret = ioctl(fd, SILENTRBIOC_LOCK);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("get_status", sr_get_count(fd, &ret), OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("lock", ret, base + 1, close(fd));

	ret = ioctl(fd, SILENTRBIOC_LOCK);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	ret = ioctl(fd, SILENTRBIOC_UNLOCK);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("get_status", sr_get_count(fd, &ret), OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("unlock", ret, base + 1, close(fd));

	ret = ioctl(fd, SILENTRBIOC_UNLOCK);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("get_status", sr_get_count(fd, &ret), OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("unlock", ret, base, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_delay_hold_p
* @brief                :Delay takes a lock and releases it on expiry
* @Scenario             :SILENTRBIOC_DELAY(n) locks the module for n seconds
*                       and the timer callback unlocks it afterwards.
*                       Skipped inside the reboot timezone (auto unlock).
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :Lock count restored
* @return               :void
*/
static void utc_silent_reboot_delay_hold_p(void)
{
	int fd;
	int ret;
	int base;
	silent_reboot_status_t st;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	if (sr_in_reboot_timezone(fd)) {
		printf("\n[%s] SKIP, inside silent reboot timezone\n", __func__);
		close(fd);
		return;
	}

	TC_ASSERT_EQ_CLEANUP("get_status", sr_get_count(fd, &base), OK, close(fd));

	ret = ioctl(fd, SILENTRBIOC_DELAY, (unsigned long)SR_DELAY_SEC);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	TC_ASSERT_EQ_CLEANUP("getstatus", ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st), OK, close(fd));
	TC_ASSERT_GEQ_CLEANUP("delay", st.reboot_delay_left, 1, close(fd));
	TC_ASSERT_LEQ_CLEANUP("delay", st.reboot_delay_left, SR_DELAY_SEC, close(fd));
	TC_ASSERT_EQ_CLEANUP("delay lock", st.lock_count, base + 1, close(fd));

	sleep(SR_DELAY_WAIT_SEC);

	/* The delay timer must have unlocked the module again */
	TC_ASSERT_EQ_CLEANUP("getstatus", ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st), OK, close(fd));
	TC_ASSERT_EQ_CLEANUP("delay left", st.reboot_delay_left, 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("delay unlock", st.lock_count, base, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_force_noop_p
* @brief                :A force timeout longer than the armed timer is a no-op
* @Scenario             :SILENTRBIOC_FORCE_REBOOT returns OK without touching
*                       the periodic watchdog when the requested timeout is
*                       larger than the remaining time.
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :Periodic timer untouched
* @return               :void
*/
static void utc_silent_reboot_force_noop_p(void)
{
	int fd;
	int ret;
	int left;
	silent_reboot_status_t st;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	if (sr_in_reboot_timezone(fd)) {
		printf("\n[%s] SKIP, inside silent reboot timezone\n", __func__);
		close(fd);
		return;
	}

	TC_ASSERT_EQ_CLEANUP("getstatus", ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st), OK, close(fd));
	left = st.reboot_timezone_left;

	ret = ioctl(fd, SILENTRBIOC_FORCE_REBOOT, (unsigned long)(left + SR_TZ_MARGIN_SEC * 3));
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	/* The watchdog must only have moved on with the elapsed wall time */
	TC_ASSERT_EQ_CLEANUP("getstatus", ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st), OK, close(fd));
	TC_ASSERT_LEQ_CLEANUP("force noop", st.reboot_timezone_left, left + 2, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :utc_silent_reboot_force_rearm_p
* @brief                :Force timeout shortens the watchdog and re-arms it
* @Scenario             :SILENTRBIOC_FORCE_REBOOT(3) re-arms the periodic
*                       watchdog to fire in 3 seconds.  Outside the reboot
*                       timezone the callback only re-arms the timer for the
*                       next reboot window instead of rebooting the board.
*                       Also skipped when the window begins within 360
*                       seconds so the short watchdog can never expire
*                       inside it.
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :Periodic watchdog re-armed for the next window
* @return               :void
*/
static void utc_silent_reboot_force_rearm_p(void)
{
	int fd;
	int ret;
	silent_reboot_status_t st;

	fd = open(SILENT_REBOOT_DRVPATH, O_RDWR);
	TC_ASSERT_GEQ("open", fd, OK);

	if (sr_in_reboot_timezone(fd)) {
		printf("\n[%s] SKIP, inside silent reboot timezone\n", __func__);
		close(fd);
		return;
	}

	TC_ASSERT_EQ_CLEANUP("getstatus", ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st), OK, close(fd));
	if (st.reboot_timezone_left < SR_TZ_MARGIN_SEC) {
		printf("\n[%s] SKIP, reboot timezone starts within %d seconds\n",
				__func__, SR_TZ_MARGIN_SEC);
		close(fd);
		return;
	}

	ret = ioctl(fd, SILENTRBIOC_FORCE_REBOOT, (unsigned long)SR_FORCE_SEC);
	TC_ASSERT_EQ_CLEANUP("ioctl", ret, OK, close(fd));

	sleep(SR_FORCE_WAIT_SEC);

	/* The callback ran without rebooting and re-armed the next window */
	TC_ASSERT_EQ_CLEANUP("getstatus", ioctl(fd, SILENTRBIOC_GETSTATUS, (unsigned long)&st), OK, close(fd));
	TC_ASSERT_GT_CLEANUP("rearm", st.reboot_timezone_left, 0, close(fd));
	TC_ASSERT_LEQ_CLEANUP("rearm", st.reboot_timezone_left, SR_TZ_REARM_MAX_SEC, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_silent_reboot_main(int argc, char *argv[])
#endif
{
	if (testcase_state_handler(TC_START, "Silent Reboot UTC") == ERROR) {
		return ERROR;
	}

	utc_silent_reboot_open_p();
	utc_silent_reboot_ioctl_n();
	utc_silent_reboot_checkmode_p();
	utc_silent_reboot_getstatus_p();
	utc_silent_reboot_getstatus_n();
	utc_silent_reboot_delay_n();
	utc_silent_reboot_reboot_reason_p();
	utc_silent_reboot_force_noop_p();
	utc_silent_reboot_lock_unlock_p();
	utc_silent_reboot_delay_hold_p();
	utc_silent_reboot_force_rearm_p();

	(void)testcase_state_handler(TC_END, "Silent Reboot UTC");

	printf("\nTOTAL : %d, FAIL : %d\n", total_pass + total_fail, total_fail);

	return (total_fail > 0) ? ERROR : OK;
}