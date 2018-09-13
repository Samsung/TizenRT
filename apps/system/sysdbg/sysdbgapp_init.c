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

/***************************************************************************
 * Included Files
 ***************************************************************************/

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <apps/system/sysdbgapp_init.h>
#include <apps/shell/tash.h>

/* Global variable to hold the handle of sysdbg device */
static int g_sysdbg_handle = -1;

/****************************************************************************
 * Name:  sysdbg_dev_open
 *
 * Description:
 *   This function opens sysdbg device, so that user space can talk to it
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   int - Negative value indicates that device open failure, non negative
 *   indicates the handle of the device
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
static int sysdbg_dev_open(void)
{
	if (g_sysdbg_handle < 0) {
		g_sysdbg_handle = open("/dev/sysdbg", O_WRONLY);
		if (g_sysdbg_handle < 0) {
			printf("sysdbgapp: can not open /dev/sysdbg (errno = %d)\n", get_errno());
		}
	}
	return g_sysdbg_handle;
}

/****************************************************************************
 * Name: sysdbg_dev_close
 *
 * Description:
 *   This function closes sysdbg device and initialize the device handle to -1
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *  None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
static void sysdbg_dev_close(void)
{
	int ret = -1;
	if (g_sysdbg_handle < 0) {
		printf("sysdbgapp:  /dev/sysdbg device is not yet open\n");
	} else {
		ret = close(g_sysdbg_handle);
		if (ret < 0) {
			printf("sysdbgapp: failed to close /dev/sysdbg (handle = %d  errno = %d)\n", g_sysdbg_handle, get_errno());
		} else {
			g_sysdbg_handle = -1;
		}
	}
}

/****************************************************************************
 * Name:  show_usage
 *
 * Description:
 *   This function prints the usage of sysdbg tash command
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void show_usage(FAR const char *progname)
{
	printf("USAGE: \n");
	printf("kindly provide 2 arguments\n");
	printf("For help, %s help\n", progname);
}

/****************************************************************************
 * Name:  sysdbgapp_main
 *
 * Description:
 *   This function handles the user inputs and writes it to sysdbg device
 *
 * Inputs:
 *   argc - Number of arguments passed
 *   argv - Array of character pointers
 *
 * Return Value:
 *   int - zero indicates successful execution
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

int sysdbgapp_main(int argc, char **args)
{
	int nbytes;
	int ret = 0;
	int arg1_len = 0;
	int arg2_len = 0;
	char arg_str[64] = { '\0' };

	if (argc < 2 || argc > 3) {
		show_usage(args[0]);
	} else {
		/* Open system debug device created in kernel/debug/sysdbg.c */
		if (sysdbg_dev_open() < 0) {
			printf("failed to open /dev/sysdbg (handle = %d & errono = %d)\n", g_sysdbg_handle, get_errno());
			ret = -1;
			goto end;
		}
		/* Second and Third arguments length should not be greater than 32 [ 31 + '\0' ] */
		arg1_len = strlen(args[1]);
		if ((arg1_len + 1) > 32) {
			printf("sysdbgapp: args[1]:%s exceeds max argument length of 31\n", args[1]);
			ret = -1;
			goto end;
		}
		if (argc == 3) {
			arg2_len = strlen(args[2]);
			if ((arg2_len + 1) > 32) {
				printf("sysdbgapp: args[2]:%s exceeds max argument length of 31\n", args[2]);
				ret = -1;
				goto end;
			}
		}
		snprintf(arg_str, sizeof(arg_str), "%s %s", args[1], (argc == 3) ? args[2] : NULL);
		nbytes = write(g_sysdbg_handle, arg_str, strlen(arg_str));

		if (nbytes < 0) {
			printf("sysdbgapp: failed to write to sysdbg device (errno = %d) \n", get_errno());
			ret = -1;
		}
	}
end:
	/* Close the device while exiting the main */
	if (g_sysdbg_handle >= 0) {
		sysdbg_dev_close();
	}
	return ret;
}

/****************************************************************************
 * Name:  sysdbgapp_init
 *
 * Description:
 *   This function registers the sysdbg command to TASH
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

void sysdbgapp_init(void)
{
	tash_cmd_install("sysdbg", sysdbgapp_main, TASH_EXECMD_ASYNC);
}
