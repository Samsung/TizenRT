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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <apps/system/binary_update.h>

#define RELOAD_CMD             0
#define GETINFO_CMD            1
#define GETINFO_ALL_CMD        2

static void print_binary_info(binary_info_t *binary_info)
{
	printf(" ========= binary [%s] info ========== \n", binary_info->name);
	printf(" %8s | %8s | %s \n", "Version", "Partsize", "Inactive part");
	printf(" --------------------------------------- \n");
	printf(" %8s | %8d | %s\n", binary_info->version, binary_info->part_size, binary_info->dev_path);
	printf(" ======================================== \n");
}

static void print_binary_info_list(binary_info_list_t *binary_info_list)
{
	int bin_idx;

	printf(" ============ ALL binary info : %d count ============== \n", binary_info_list->bin_count);
	printf(" %4s | %6s | %8s | %8s | %s \n", "Idx", "Name", "Version", "Partsize", "Inactive part");
	printf(" ---------------------------------------------------- \n");
	for (bin_idx = 0; bin_idx < binary_info_list->bin_count; bin_idx++) {
		printf(" %4d | %6s | %8s | %8d | %s\n", bin_idx, \
		binary_info_list->bin_info[bin_idx].name, binary_info_list->bin_info[bin_idx].version, \
		binary_info_list->bin_info[bin_idx].part_size, binary_info_list->bin_info[bin_idx].dev_path);
	}
	printf(" ==================================================== \n");
}


static void binary_update_getinfo_all(void)
{
	int ret;
	binary_info_list_t bin_info_list;

	ret = binary_update_get_binary_info_all(&bin_info_list);
	if (ret == OK) {
		print_binary_info_list(&bin_info_list);
	} else {
		printf("Get binary info all FAIL %d\n", ret);
	}
}

static void binary_update_getinfo(char *name)
{
	int ret;
	binary_info_t bin_info;

	ret = binary_update_get_binary_info(name, &bin_info);
	if (ret == OK) {
		print_binary_info(&bin_info);
	} else {
		printf("Get binary info FAIL, ret %d\n", ret);
	}
}

static void binary_update_reload(char *name)
{
	int ret;

	ret = binary_update_reload_binary(name);
	if (ret == OK) {
		printf("RELOAD [%s] SUCCESS\n", name);
	} else {
		printf("Reload binary %s FAIL, ret %d\n", name, ret);
	}
}


/****************************************************************************
 * binary_update_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int binary_update_main(int argc, char *argv[])
#endif
{
	int type;

	printf("Binary update!!\n");

	if (argc <= 1) {
		goto usage;
	}

	type = (pid_t)atoi(argv[1]);

	if (type == GETINFO_ALL_CMD && argc == 2) {
		binary_update_getinfo_all();
	} else if (type == GETINFO_CMD && argc == 3) {
		binary_update_getinfo(argv[2]);
	} else if (type == RELOAD_CMD && argc == 3) {
		binary_update_reload(argv[2]);
	} else {
		goto usage;
	}

	return 0;
usage:
	printf("\nUsage: binary_update CMD [BINARY_NAME]\n");
	printf("CMD \n");
	printf("   0 : RELOAD, 1 : GETINFO, 2 : GETINFO ALL \n");

	return -1;
}
