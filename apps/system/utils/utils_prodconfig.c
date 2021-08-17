/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include <tinyara/prodconfig.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name : utils_prodconfig
 *
 * Description:
 *   It reads the hw configurations from prodconfig drivers
 *   and prints the status like swd enable/disable.
 *
 ****************************************************************************/
int utils_prodconfig(int argc, char **args)
{
	int prodconfig_fd;
	char result = 0;
	prodconfig_fd = open(PRODCONFIG_DRVPATH, O_RDWR);
	if (prodconfig_fd < 0) {
		printf("Fail to open prodconfig, errno %d.\n", get_errno());
		return ERROR;
	}

	(void)read(prodconfig_fd, &result, sizeof(char));
	if (result & SWD_ENABLED) {
		printf("HW Debug : On\n");
	} else {
		printf("HW Debug : Off\n");
	}
	
	if (result & DOWNLOAD_ENABLED) {
		printf("Download : Downloader & Hotfix Available\n");
	} else {
		printf("Download : Hotfix Only Available\n");
	}

	close(prodconfig_fd);

	return OK;
}
