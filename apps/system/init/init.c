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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <tinyara/config.h>
#ifdef CONFIG_LIB_USRWORK
#include <tinyara/wqueue.h>
#endif
#ifdef CONFIG_TASH
#include <apps/shell/tash.h>
#include <apps/system/utils.h>
#endif
#ifdef CONFIG_SYSTEM_INFORMATION
#include <apps/system/sysinfo.h>
#endif
#ifdef CONFIG_BUILD_PROTECTED
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_PROCFS)
#include <sys/mount.h>
#endif
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* In the protected build (only) we also need to start the user work queue */

#if !defined(CONFIG_BUILD_PROTECTED)
#undef CONFIG_LIB_USRWORK
#endif

#ifdef CONFIG_LWM2M_WAKAAMA
extern void dm_cb_register_init(void);
#endif

/****************************************************************************
 * Pravite Functions
 ****************************************************************************/
#ifdef CONFIG_TASH
static void tash_register_cmds(void)
{
#if defined(CONFIG_KERNEL_CMDS) && !defined(CONFIG_BUILD_PROTECTED)
	kernel_register_utilcmds();
#endif
	kernel_register_appcmds();

#ifdef CONFIG_FS_CMDS
	fs_register_utilcmds();
#endif

#ifdef CONFIG_NET_CMDS
	net_register_utilcmds();
	net_register_appcmds();
#endif

	register_examples_cmds();
}
#endif							/* CONFIG_TASH */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int preapp_start(int argc, char *argv[])
#endif
{
#if defined(CONFIG_LIB_USRWORK) || defined(CONFIG_TASH)
	int pid;
#endif

#ifdef CONFIG_BUILD_PROTECTED
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_PROCFS)
	int ret;
	ret = mount(NULL, "/proc", "procfs", 0, NULL);
	if (ret < 0) {
		printf("procfs mount is failed, error code is %d\n", ret);
	}
#endif
#endif

#ifdef CONFIG_SYSTEM_INFORMATION
	sysinfo();
#endif

#ifdef CONFIG_LIB_USRWORK
	/* Start the user-space work queue */

	pid = work_usrstart();
	if (pid <= 0) {
		printf("user work queue is failed to start, error code is %d\n", pid);
		goto error_out;
	}
#endif

#ifdef CONFIG_LWM2M_WAKAAMA
	dm_cb_register_init();
#endif

#ifdef CONFIG_TASH
	tash_register_cmds();

	pid = tash_start();
	if (pid <= 0) {
		printf("TASH is failed to start, error code is %d\n", pid);
		goto error_out;
	}
#endif

#if defined(CONFIG_LIB_USRWORK) || defined(CONFIG_TASH)
error_out:
	return pid;
#else
	return 0;
#endif
}
