/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <tinyara/ss_slot_index.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <apps/shell/tash.h>
#include <security/security_ss.h>
#include <security/security_common.h>

/********************************************************************************
 * Private Functions
 ********************************************************************************/
static void show_usage(void) 
{
	printf("\nUsage : sec_level [OPTION]...\n");
	printf("Set your security level high or low, or get the current security level status.\n\n");
	printf("Options :\n");
	printf("high		set security level 1(high)\n");
	printf("		You cannot see some secure information.\n\n");
	printf("low		set security level 0(low)\n");
	printf("		You can see all secure information.\n\n");
	printf("status		show current value of security level\n\n");
}

static void tash_security_level(int argc, char **args)
{
        security_handle hnd;
	security_data ss_data = {NULL, 0};
	char ss_path[7];
	security_error ret;

	if (argc < 2) {
		show_usage();
		return;
	}

	/* Show usage */
	if (!strncmp(args[1], "--help", 7)) {
		show_usage();
		return;
	}

	/* Init security handler */
	ret = security_init(&hnd);
        if (ret != SECURITY_OK) {
		printf("[tash_security_level] Failed to initialize security handler\n");
		printf("[security_init] error ret : %d\n", ret);
		free(hnd);
		return;
	}

	/* Form the secure storage path name based on the slot index */
	snprintf(ss_path, 7, "ss/%d", SS_SLOT_INDEX_SECURITY_LEVEL);

	/* Read 'security_level' variable from secure storage */
	if (!strncmp(args[1], "status", 7)) {
		ret = ss_read_secure_storage(hnd, ss_path, 0, &ss_data);
		if (ret != SECURITY_OK) {
			printf("[tash_security_level] Failed to read secure storage\n");
			printf("[ss_read_secure_storage] error ret : %d\n", ret);
			(void)security_deinit(hnd);
			return;
		}

		if (!strncmp(ss_data.data, "1", 2)) {
			printf("security level : high\n");
		} else if (!strncmp(ss_data.data, "0", 2)) {
			printf("security level : low\n");
		} else {
			printf("security level : Unexpected value!!!\n");
			printf("Please set the security level using \"sec_level high\" or \"sec_level low\".\n");
		}
		
		(void)security_deinit(hnd);
		return;
	}

	/* Change security_level according to user input */
	ss_data.length = 2;
	if (!strncmp(args[1], "high", 5)) {
		ss_data.data = "1";
	} else if (!strncmp(args[1], "low", 4)) {
		ss_data.data = "0";
	} else {
		printf("Invalid argument (%s).\n", args[1]);
		show_usage();
		return;
	}
	
        /* Write 'security_level' variable to secure storage  */
	ret = ss_write_secure_storage(hnd, ss_path, 0, &ss_data);
        if (ret != SECURITY_OK) {
		printf("[tash_security_level] Failed to write data in secure storage\n");
		printf("[ss_write_secure_storage] error ret : %d\n", ret);
		(void)security_deinit(hnd);
		return;
	} 
	
	(void)security_deinit(hnd);

	printf("Security level is set\n");

	/* Apply changed value to the kernel variable */
	ret = prctl(PR_SET_SECURITY_LEVEL);
	if (ret != 0) {
		printf("[prctl] Fail to call set_security_level. ret : %d\n", ret);
		return;
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void security_level_register_utilcmds(void)
{
	tash_cmd_install("sec_level", tash_security_level, TASH_EXECMD_SYNC);
}
