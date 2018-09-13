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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/testcase_drv.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int testdrv_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t testdrv_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t testdrv_write(FAR struct file *filep, FAR const char *buffer, size_t len);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations testdrv_fops = {
	0,							/* open */
	0,							/* close */
	testdrv_read,						/* read */
	testdrv_write,						/* write */
	0,							/* seek */
	testdrv_ioctl						/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0							/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/************************************************************************************
 * Name: testdrv_ioctl
 *
 * Description:  The standard ioctl method.
 *
 ************************************************************************************/

static int testdrv_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -EINVAL;

	vdbg("cmd: %d arg: %ld\n", cmd, arg);

	/* Handle built-in ioctl commands */

	switch (cmd) {
	/* TESTFWIOC_DRIVER_ANALOG - Run the test case for /os/driver/analog module
	 *
	 *   ioctl argument:  An integer value indicating the particular test to be run
	 */

	case TESTIOC_DRIVER_ANALOG: {

	}
	break;

	default: {
		vdbg("Unrecognized cmd: %d arg: %ld\n", cmd, arg);
	}
	break;

	}

	return ret;
}


static ssize_t testdrv_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;                                       /* Return EOF */
}

static ssize_t testdrv_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return len;                                     /* Say that everything was written */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: test_drv_register
 *
 * Description:
 *   Register /dev/testcase
 *
 ****************************************************************************/

void test_drv_register(void)
{
	(void)register_driver("/dev/testcase", &testdrv_fops, 0666, NULL);
}
