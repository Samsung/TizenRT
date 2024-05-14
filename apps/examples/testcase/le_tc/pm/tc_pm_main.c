/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/pm/pm.h>
#include <tinyara/clock.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tc_common.h>

#define PM_DRVPATH           "/dev/pm"
#define PM_PROCPATH          "/proc/power/domains/0/pm_suspendcount"
#define SUSPEND_TIME         5000           // It is in millisecond
#define SLEEP_TIME           5000           // It is in millisecond
#define TOTAL_CHECK_TIME     20000          // It is in millisecond
#define MISSING_TICK         5000 
#define DEVIATION_TICK       200

static int pm_drv_fd;
static int pm_proc_fd;

void tc_pm_suspend_resume(void)
{
	int ret_chk;
	int initial_suspendcount, during_suspendcount, final_suspendcount;

	initial_suspendcount = read(pm_proc_fd, NULL, PM_IDLE_DOMAIN);
	TC_ASSERT_GEQ("read suspendcount", initial_suspendcount, OK);

	ret_chk = ioctl(pm_drv_fd, PMIOC_SUSPEND, PM_IDLE_DOMAIN);
	TC_ASSERT_EQ("pm driver suspend", ret_chk, OK);

	during_suspendcount = read(pm_proc_fd, NULL, PM_IDLE_DOMAIN);
	TC_ASSERT_EQ("pm suspend", during_suspendcount, initial_suspendcount + 1);

	// 5 second of lock is added to ensure system is not going to sleep
	// This can be tested by user visually to check if board is not going to sleep
	sleep(5);

	ret_chk = ioctl(pm_drv_fd, PMIOC_RESUME, PM_IDLE_DOMAIN);
	TC_ASSERT_EQ("pm driver resume", ret_chk, OK);

	final_suspendcount = read(pm_proc_fd, NULL, PM_IDLE_DOMAIN);
	TC_ASSERT_EQ("pm resume", final_suspendcount, during_suspendcount - 1);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: tc_pm_timedstay
 *
 * Description:
 *   Testing of pm timedstay operation. This function runs timedstay function
 *   and checks if the pm state's stay count increases during timer running and
 *   goes back to initial value after timer is expired. 
 *
 * Returned Value:
 *   None
 ****************************************************************************/
void tc_pm_timedstay(void)
{
    	int ret_chk;
    	clock_t curr_timetick;
    	clock_t last_timetick;
    	int initial_suspendcount, during_suspendcount, final_suspendcount;

    	/* Storing the stay count before timedstay */
    	initial_suspendcount = read(pm_proc_fd, NULL, PM_IDLE_DOMAIN);
	TC_ASSERT_GEQ("read suspendcount", initial_suspendcount, OK);

    	curr_timetick = clock();
    	last_timetick = curr_timetick;

    	pm_suspend_arg_t arg = {PM_IDLE_DOMAIN, SUSPEND_TIME};
    	ret_chk = ioctl(pm_drv_fd, PMIOC_TIMEDSUSPEND, &arg);
    	TC_ASSERT_EQ("pm_ioctl timedstay", ret_chk, OK);

    	/* Checking if the timedstay is working for 5 sec, 
     	* the stay count should increase by 1 during this time and
     	* it should prevent board from going to sleep */
    	while (curr_timetick - last_timetick < SUSPEND_TIME) {
        	during_suspendcount = read(pm_proc_fd, NULL, PM_IDLE_DOMAIN);
		TC_ASSERT_EQ("read suspendcount", during_suspendcount, initial_suspendcount + 1);
        	sleep(1);
        	curr_timetick = clock();
    	}

    	/* extra 1 sec delay is padded for boundary condition */
    	sleep(1);

    	/* After 5 sec, the staycount should be back to 
     	* initial value */
    	final_suspendcount = read(pm_proc_fd, NULL, PM_IDLE_DOMAIN);
	TC_ASSERT_EQ("read suspendcount", final_suspendcount, initial_suspendcount);

    	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: tc_pm_msleep
 *
 * Description:
 *   Testing of pm_msleep operation. This function checks if pm timer
 *   timer is running properly or not
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void tc_pm_msleep(void)
{
    	int ret_chk;
    	int sleep_cnt = 0;
    	clock_t curr_timetick, last_timetick;

	/* Making board go to sleep as pm idle domain is suspended 
	 * just after booting. It is assumed that testcases will be run
	 * after booting done */
	ret_chk = ioctl(pm_drv_fd, PMIOC_RESUME, PM_IDLE_DOMAIN);
	TC_ASSERT_EQ("pm_ioctl resume", ret_chk, OK);

    	curr_timetick = clock();
    	last_timetick = curr_timetick;

    	/* pm_msleep will be applied for 5 sec continuously inside 
     	* the while loop for 20 sec. So pm_msleep should be run 
     	* 4 times.*/
    	while(curr_timetick - last_timetick < TOTAL_CHECK_TIME) {
        	ret_chk = ioctl(pm_drv_fd, PMIOC_SLEEP, SLEEP_TIME);
        	TC_ASSERT_EQ("pm_ioctl sleep", ret_chk, OK);
        	sleep_cnt++;
        	curr_timetick = clock();
    	}

    	TC_ASSERT_EQ("pm_msleep function", sleep_cnt, (TOTAL_CHECK_TIME/SLEEP_TIME));

	ret_chk = ioctl(pm_drv_fd, PMIOC_SUSPEND, PM_IDLE_DOMAIN);
        TC_ASSERT_EQ("pm_ioctl suspend", ret_chk, OK);
    	
    	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: tc_pm_missing_tick
 *
 * Description:
 *   This function tests if after the board wakes up from the sleep,
 *   if the global system tick count is updated or not because of missing
 *   tick during sleep time.
 *
 * Returned Value:
 *   None
 ****************************************************************************/

void tc_pm_missing_tick(void)
{
    	int ret_chk;
    	clock_t initial_timetick, final_timetick;
    	int staycount, is_resume_applied;

	/* Assuming after booting pm idle domain is suspended by system 
	 * So making the board sleep */
	ret_chk = ioctl(pm_drv_fd, PMIOC_RESUME, PM_IDLE_DOMAIN);
        TC_ASSERT_EQ("pm_ioctl suspend", ret_chk, OK);

    	initial_timetick = clock();

    	ret_chk = ioctl(pm_drv_fd, PMIOC_SLEEP, SLEEP_TIME);
    	TC_ASSERT_EQ("pm_ioctl sleep", ret_chk, OK);

    	final_timetick = clock();

        ret_chk = ioctl(pm_drv_fd, PMIOC_SUSPEND, PM_IDLE_DOMAIN);
        TC_ASSERT_EQ("pm_ioctl suspend", ret_chk, OK);

    	/* Check if difference between final and initial tick is 
     	* within excepted sleep time with slight deviation */
    	TC_ASSERT_GEQ("missing tick after PM wake up", final_timetick - initial_timetick, MISSING_TICK);
    	TC_ASSERT_LEQ("missing tick after PM wake up", final_timetick - initial_timetick, MISSING_TICK + DEVIATION_TICK);

    	TC_SUCCESS_RESULT();
}


/****************************************************************************
 * Public Function
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_pm_main(int argc, char *argv[])
#endif
{
	printf("Testing PM test cases.....\n");

    	pm_drv_fd = open(PM_DRVPATH, O_WRONLY);
	pm_proc_fd = open(PM_PROCPATH, O_RDONLY);
    	/*If FAIL : Failed to open testcase driver*/
	TC_ASSERT_GEQ("pm_drv_fd", pm_drv_fd, OK);
	TC_ASSERT_GEQ_CLEANUP("pm_proc_fd", pm_proc_fd, OK, close(pm_drv_fd));

	tc_pm_suspend_resume();
    	tc_pm_timedstay();
    	tc_pm_msleep();
    	tc_pm_missing_tick();

    	close(pm_drv_fd);
	close(pm_proc_fd);

	return 0;
}
