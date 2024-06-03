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

#include <errno.h>
#include <debug.h>
#include <assert.h>

#define PM_DRVPATH           "/dev/pm"

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

static ssize_t pm_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t pm_write(FAR struct file *filep, FAR const char *buffer, size_t len);
static int pm_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

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
 *   PMIOC_SUSPEND - for locking a specific PM state  
 *   PMIOC_RESUME - for unlocking a specific PM state
 *   PMIOC_SLEEP - to make board sleep for given time
 *   PMIOC_TIMEDSUSPEND - to suspend a pm state for given time duration
 *   PMIOC_TUNEFREQ - for changing the operating frequency of the core to save power
 * 
 * Arguments:
 *   filep is ioctl fd, cmd is required command, arg is required argument for
 *   the command. 
 *   for PMIOC_SUSPEND, arg is an enum representing PM DOMAIN
 *   for PMIOC_RESUME, arg is an enum representing PM DOMAIN
 *   for PMIOC_SLEEP, arg should be an int.(user should input time in millisecond)
 *   for PMIOC_TIMEDSUSPEND, arg should be a pointer to pm_suspend_arg_t 
 *   for PMIOC_TUNEFREQ, arg should be an int type.
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
    	case PMIOC_SUSPEND:
	        ret = pm_suspend((enum pm_domain_e)arg);
		pmvdbg("State locked!\n");
        	break;
        case PMIOC_RESUME:
		ret = pm_resume((enum pm_domain_e)arg);
		pmvdbg("State unlocked!\n");
		break;
	case PMIOC_SLEEP:
		if (arg > 0) {
			ret = pm_sleep((int)arg);
		} else {
			pmvdbg("Please input positive timer interval\n");
		}
		break;
	case PMIOC_TIMEDSUSPEND:
		if ((pm_suspend_arg_t *)arg == NULL) {
			pmvdbg("Please input correct arguments\n");
		} else {
			ret = pm_timedsuspend(((pm_suspend_arg_t *)arg)->domain, ((pm_suspend_arg_t *)arg)->timer_interval);
		}
		break;
#ifdef CONFIG_PM_DVFS
        case PMIOC_TUNEFREQ:
        	pm_dvfs(arg);
        	ret = OK;
        	break;
#endif
	default:
		pmvdbg("Invalid command passed!\n");
		break;
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
