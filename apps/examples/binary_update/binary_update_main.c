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
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <apps/system/binary_update.h>

#define APP1_NAME "micom"
#define APP2_NAME "wifi"

#define EXEC_NORMAL   0
#define EXEC_INFINITE 1

static volatile bool is_running;
static volatile bool inf_flag = true;
static int fail_cnt = 0;

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

	printf("\n** Binary Update GETINFO_ALL test.\n");
	ret = binary_update_get_binary_info_all(&bin_info_list);
	if (ret == OK) {
		print_binary_info_list(&bin_info_list);
	} else {
		fail_cnt++;
		printf("Get binary info all FAIL %d\n", ret);
	}
}

static void binary_update_getinfo(char *name)
{
	int ret;
	binary_info_t bin_info;

	printf("\n** Binary Update GETINFO [%s] test.\n", name);
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

	printf("\n** Binary Update RELOAD [%s] test.\n", name);
	ret = binary_update_reload_binary(name);
	if (ret == OK) {
		printf("RELOAD [%s] SUCCESS\n", name);
	} else {
		printf("Reload binary %s FAIL, ret %d\n", name, ret);
	}
}

static void binary_update_run_tests(int repetition_num)
{
	printf("\n** Binary Update Example %d-th Iteration.\n", repetition_num);
	binary_update_getinfo(APP1_NAME);
	binary_update_getinfo(APP2_NAME);
	binary_update_getinfo_all();
	binary_update_reload(APP1_NAME);
	binary_update_reload(APP2_NAME);

	/* Wait for finishing previous test. */
	sleep(1);
}

static void binary_update_show_success_ratio(int rep_cnt)
{
	printf("\n*** Binary Update Example is finished. (run %d-times)\n", rep_cnt);
	printf(" - success : %d, fail %d\n", rep_cnt - fail_cnt, fail_cnt);
	fail_cnt = 0;
}

static void binary_update_execute_infinitely(void)
{
	int repetition_num;

	printf("\n** Start Binary Update Example : executes infinitely. ===\n");
	repetition_num = 0;
	is_running = true;
	while (inf_flag) {
		repetition_num++;
		binary_update_run_tests(repetition_num);
	}
	binary_update_show_success_ratio(repetition_num);
	is_running = false;
}

static void binary_update_execute_ntimes(int repetition_num)
{
	int loop_idx;
	is_running = true;
	/* binary_update example executes multiple-times. */
	printf("\n** Binary Update example : executes %d-times.\n", repetition_num);
	for (loop_idx = 1; loop_idx <= repetition_num; loop_idx++) {
		binary_update_run_tests(loop_idx);
	}
	binary_update_show_success_ratio(repetition_num);
	is_running = false;
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
	int repetition_num = 1;
	int option;
	char *cmd_arg = NULL;
	char *cnt_arg = NULL;
	int execution_type = EXEC_NORMAL;

	if (argc >= 4 || argc == 2) {
		goto usage;
	}

	while ((option = getopt(argc, argv, "r:n:")) != ERROR) {
		switch (option) {
		case 'r':
			execution_type = EXEC_INFINITE;
			cmd_arg = optarg;
			break;
		case 'n':
			execution_type = EXEC_NORMAL;
			cnt_arg = optarg;
			break;
		case '?':
		default:
			goto usage;
		}
	}

	/* Execute the binary update example. */
	if (execution_type == EXEC_INFINITE) {
		if (cmd_arg && strncmp(cmd_arg, "start", strlen("start") + 1) == 0) {
			if (is_running) {
				goto already_running;
			}
			inf_flag = true;
			binary_update_execute_infinitely();
		} else if (cmd_arg && strncmp(cmd_arg, "stop", strlen("stop") + 1) == 0) {
			if (inf_flag == false) {
				printf("There is no infinite running Binary Update example. Cannot stop the sample.\n");
				return -1;
			}
			inf_flag = false;
		} else {
			goto usage;
		}

	} else {
		if (is_running) {
			goto already_running;
		}

		if (cnt_arg != NULL) {
			repetition_num = atoi(cnt_arg);
			if (repetition_num <= 0) {
				goto usage;
			}
		} else {
			repetition_num = 1;
		}

		binary_update_execute_ntimes(repetition_num);
	}

	return 0;
usage:
	printf("\nUsage : binary_update [OPTIONS]\n");
	printf("Options:\n");
	printf(" -r start : Execute Binary Update Example infinitely until stop cmd.\n");
	printf("    stop  : Stop the Binary Update Example infinite execution.\n");
	printf(" -n COUNT : Execute Binary Update Example COUNT-iterations.\n");
	return -1;
already_running:
	printf("There is already running Binary Update Example.\n");
	printf("New sample can run after that previous example is finished.\n");
	return -1;
}
