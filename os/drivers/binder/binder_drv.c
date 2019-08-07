/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/binder_drv.h>
#include <tinyara/fs/fs.h>
#include <sys/types.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations binder_fops = {
	0, /* open */
	0, /* close */
	0, /* read */
	0, /* write */
	0, /* seek */
	0, /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	0 /* poll */
#endif
};
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: binder_drv_register
 *
 * Description:
 *   Register binder path, BINDER_DRVPATH
 *
 ****************************************************************************/

void binder_drv_register(void)
{
	(void)register_driver(BINDER_DRVPATH, &binder_fops, 0666, NULL);
}
