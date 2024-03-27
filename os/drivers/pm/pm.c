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

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/pm/pm.h>
#include <tinyara/clock.h>

#include <errno.h>
#include <debug.h>
#include <assert.h>

#define PM_DRVPATH     "/dev/pm"

/* Added to record timer countdown interrupt */
struct pm_timer_s g_pm_timer = {0, 0, 0};

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

static int pm_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t pm_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t pm_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Data
 ****************************************************************************/
static const struct file_operations pm_fops = {
	0,                          /* open */
	0,                          /* close */
	pm_read,                    /* read */
	pm_write,                   /* write */
	0,                          /* seek */
	pm_ioctl                    /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t pm_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t pm_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}


/************************************************************************************
 * Name: pm_ioctl
 *
 * Command description:
 *   PMIOC_LOCK - for locking PM transition (both permanent and timer)
 *   PMIOC_UNLOCK - for unlocking PM transiton
 *   PMIOC_WAKEUP_TIMER - for waking up the system after a given time
 *   PMIOC_SET_WIFI_WAKEUP - to set wakeup timer for next wifi keep alive signal sending
 *   PMIOC_TIMETICK - for getting current time tick
 *   PMIOC_TUNEFREQ - for changing the operating frequency of the core to save power
 * 
 * Arguments:
 *   filep is ioctl fd, cmd is required command, arg is required argument for
 *   the command. 
 *   for LOCK, WAKEUP_TIMER, SET_WIFI_WAKEUP, arg should be a long unsigned type.
 *   for TIMETICK , arg should be a pointer to a size_t type.
 *   for TUNEFREQ, arg should be an int type.
 *
 * Description:
 *   This api can be used to perform PM operation.
 *
 ************************************************************************************/
static int pm_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -ENOSYS;
    
	/* Handle built-in ioctl commands */
	switch (cmd) {
    
     	/****************************************************************************
     	* Description: Lock PM in PM_NORMAL state by invoking pm_stay().
     	****************************************************************************/
    	case PMIOC_LOCK:
        	/* Always lock PM_NORMAL state, check again whether this implementation
        	is enough to handle, if not, consider below methods...
        	1a. PM_ACTIVITY is invoked in some of the driver, if application utlized it, it should draw state back to PM_NORMAL
        	1b. If 1a cannot cover all condition, add one more syscall api, as a bridge for application to call pm_activity
        	2a. Else, we can consider to lock/unlock for all state other than PM_SLEEP, ie. fetch state: pm_querystate(PM_IDLE_DOMAIN)
        	2b. Need to verify if the transition is going to happen, but lock state appears suddenly, what will happen?
        	*/
        	pm_stay(PM_IDLE_DOMAIN, PM_NORMAL);
        	pmvdbg("State locked!\n");
        	if (arg > 0) {
            		pm_set_timer(PM_LOCK_TIMER, arg);
        	} else {
            		g_pm_timer.timer_type = PM_NO_TIMER;
        	}
        	ret = OK;
        	break;
        /****************************************************************************
         * Description:
         *  Unlock PM from PM_NORMAL state by invoking pm_relax(). 
         ****************************************************************************/
        case PMIOC_UNLOCK:
            	pm_relax(PM_IDLE_DOMAIN, PM_NORMAL);
            	pmvdbg("State unlocked!\n");
            	g_pm_timer.timer_type = PM_NO_TIMER;
            	ret = OK;
            	break;
        /****************************************************************************
         * Description:
         *  Make the board wake up after given time (arg) when board sleeps.
         ****************************************************************************/
        case PMIOC_WAKEUP_TIMER:
        	if (g_pm_timer.timer_type == PM_NO_TIMER && arg > 0) {
        		pm_set_timer(PM_WAKEUP_TIMER, arg);
        	}	 
        	ret = OK;
        	break;
        /****************************************************************************
         * Description:
         *  Sets timer interval for next sleep cycle after sending of wifi keep 
         *  alive signal from UART. It also stores the current time tick 
         *  in g_pm_timer.last_wifi_alive_send_time that will be used to calculate the 
         *  duration of required sleep later just before sleep. If another
         *  interrupt occurs during this interval, the system will still
         *  wake up.
         *  Also we dont want to do pm_set_timer to PM_WAKEUP_TIMER if the
         *  system is already in PM_LOCK_TIMER state. If system is in 
         *  PM_LOCK_TIMER state, then power_unlock_write can only change that state.
         *
         ****************************************************************************/
        case PMIOC_SET_WIFI_WAKEUP:
        	g_pm_timer.last_wifi_alive_send_time = clock_systimer();
        	if (g_pm_timer.timer_type == PM_NO_TIMER && arg > 0) {
        		pm_set_timer(PM_WAKEUP_TIMER, arg);
        	}
        	ret = OK;
        	break;
        /****************************************************************************
         * Description:
         *   apps can use this method to get the current time tick.
         *
         * Returned Value:
         *  Time tick value of current time
         ****************************************************************************/
        case PMIOC_TIMETICK:
        	*((size_t *)arg) = clock_systimer();
		ret = OK;
        	break;

#ifdef CONFIG_PM_DVFS
        /****************************************************************************
        * Description:
        *  Change the operating frequency of the core to save power
        *  consumption during active mode.
        ****************************************************************************/
        case PMIOC_TUNEFREQ:
        	/* Buffer string need not to be considered, just div level is enough */
        	/* Available frequencies for changing mode of power consumption
        	0 -> 1.2GHz
        	1 -> 600MHz
        	2 -> 400Mhz
        	3 -> 300Mhz
        	*/
        	pm_dvfs(arg);
        	ret = OK;
        	break;
#endif
        }
        return ret;
}

/****************************************************************************
 * Name: pm_register
 *
 * Description:
 *   Register pm driver path, PM_DRVPATH
 *
 ****************************************************************************/
void pm_driver_register(void)
{
	(void)register_driver(PM_DRVPATH, &pm_fops, 0666, NULL);
}
