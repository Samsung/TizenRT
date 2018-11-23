/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_pwm.c
/// @brief Test Case Example for pwm driver
#include <tinyara/config.h>
#include <tinyara/pwm.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "tc_internal.h"

/**
* @fn                   :tc_driver_pwm_open_close
* @brief                :Test the pwm driver open and close
* @scenario             :Test the pwm driver open and close
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_pwm_open_close(void)
{
	int fd = 0;
	int fd2 = 0;
	int ret = 0;

	fd = open("/dev/pwm0", O_RDWR);
	TC_ASSERT_GT("pwm_open", fd, 0);

	fd2 = open("/dev/pwm0", O_RDWR);
	TC_ASSERT_GT_CLEANUP("pwm_open", fd2, 0, close(fd));

	ret = close(fd);
	TC_ASSERT_GEQ_CLEANUP("pwm_close", ret, 0, close(fd2));

	ret = close(fd2);
	TC_ASSERT_GEQ("pwm_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_pwm_read_write
* @brief                :Test the pwm driver read and write
* @scenario             :Test the pwm driver read and write
* API's covered         :read, write
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_pwm_read_write(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/pwm0", O_RDWR);
	TC_ASSERT_GT("pwm_open", fd, 0);

	ret = read(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("pwm_read", ret, 0, close(fd));

	ret = write(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("pwm_write", ret, 0, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_pwm_ioctl
* @brief                :Test the pwm driver ioctl
* @scenario             :Test the pwm driver ioctl
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_pwm_ioctl(void)
{
	int fd = 0;
	int ret = 0;

	/* Positive test cases */
	fd = open("/dev/pwm0", O_RDWR);
	TC_ASSERT_GT("pwm_open", fd, 0);

	FAR struct pwm_info_s info_s;
	FAR struct pwm_info_s info_r;

	info_s.frequency = 1000;
#ifdef CONFIG_PWM_MULTICHAN
	info_s.channels[1].duty = b16HALF;
	info_s.channels[1].channel = 1;
#else
	info_s.duty = b16HALF;
#ifdef CONFIG_PWM_PULSECOUNT
	info_s.count = 100;
#endif
#endif
	ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)&info_s);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)&info_r);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));
	TC_ASSERT_EQ_CLEANUP("pwm_ioctl", info_r.frequency, 1000, close(fd));
#ifdef CONFIG_PWM_MULTICHAN
	TC_ASSERT_EQ_CLEANUP("pwm_ioctl", info_r.channels[1].duty, b16HALF, close(fd));
	TC_ASSERT_EQ_CLEANUP("pwm_ioctl", info_r.channels[1].channel, 1, close(fd));
#else
	TC_ASSERT_EQ_CLEANUP("pwm_ioctl", info_r.duty, b16HALF, close(fd));
#ifdef CONFIG_PWM_PULSECOUNT
	TC_ASSERT_EQ_CLEANUP("pwm_ioctl", info_r.count, 100, close(fd));
#endif
#endif

	ret = ioctl(fd, PWMIOC_START, 0UL);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, PWMIOC_STOP, 0UL);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	/* Negative test cases */
	ret = ioctl(fd, -1, 0);
	TC_ASSERT_LT_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, PWMIOC_STOP, 0UL);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, PWMIOC_START, 0UL);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, PWMIOC_START, 0UL);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, PWMIOC_STOP, 0UL);
	TC_ASSERT_GEQ_CLEANUP("pwm_ioctl", ret, 0, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: pwm driver test
 ****************************************************************************/
void pwm_main(void)
{
	tc_driver_pwm_open_close();
	tc_driver_pwm_read_write();
	tc_driver_pwm_ioctl();

	return;
}
