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
#include <fcntl.h>
#include <crc32.h>
#include <sys/types.h>

#include <binary_manager/binary_manager.h>

#define APP_NAME "micom"

#define EXEC_FINITE                 0
#define EXEC_INFINITE               1

#define DOWNLOAD_VALID_BIN          0
#define DOWNLOAD_INVALID_BIN        1

#define CHECKSUM_SIZE               4
#define CRC_BUFFER_SIZE             512

/* The maximum length of binary name */
#define BIN_NAME_MAX                16

/* The maximum length of version name */
#define BIN_VER_MAX                 16

/* The length of dev name */
#define KERNEL_VER_MAX              8

struct binary_header_s {
	uint16_t header_size;
	uint8_t bin_type;
	uint8_t compression_type;
	uint8_t bin_priority;
	uint32_t bin_size;
	char bin_name[BIN_NAME_MAX];
	char bin_ver[BIN_VER_MAX];
	uint32_t bin_ramsize;
	uint32_t bin_stacksize;
	char kernel_ver[KERNEL_VER_MAX];
	uint32_t jump_addr;
} __attribute__((__packed__));
typedef struct binary_header_s binary_header_t;

static volatile bool is_running;
static volatile bool inf_flag = true;
static int fail_cnt = 0;
static unsigned int new_version = 20190422;

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

static void binary_update_check_test_result(binary_info_t *pre_bin_info, binary_info_t *cur_bin_info, int condition)
{
	printf(" === [%5s] Update info === \n", cur_bin_info->name);
	printf(" %4s | %8s | %s \n", "Con", "Version", "Inactive part");
	printf(" ------------------------- \n");
	printf(" %4s | %8s | %s \n", "Pre", pre_bin_info->version, pre_bin_info->dev_path);
	printf(" %4s | %8s | %s \n", "Cur", cur_bin_info->version, cur_bin_info->dev_path);
	printf(" ========================= \n");

	if (condition == DOWNLOAD_VALID_BIN) {
		if (strncmp(pre_bin_info->dev_path, cur_bin_info->dev_path, sizeof(pre_bin_info->dev_path) == 0)) {
			fail_cnt++;
			printf("Fail to load valid higher version binary.\n");
		} else {
			printf("Success to load valid higher version binary.\n");
		}
	} else { //DOWNLOAD_INVALID_BIN
		if (strncmp(pre_bin_info->dev_path, cur_bin_info->dev_path, sizeof(pre_bin_info->dev_path) != 0)) {
			fail_cnt++;
			printf("Warning! Load invalid binary.\n");
		} else {
			printf("No update with invalid binary.\n");
		}
	}
}

static void binary_update_write_binary(char *devname, int condition, uint32_t crc_hash, binary_header_t *header_data)
{
	int fd;
	int ret;
	fd = open(devname, O_WRONLY);
	if (fd < 0) {
		printf("Failed to open %s: %d, errno: %d\n", devname, ret, get_errno());
		return;
	}

	if (condition == DOWNLOAD_VALID_BIN) {
		/* Write new crc */
		ret = write(fd, (FAR uint8_t *)&crc_hash, CHECKSUM_SIZE);
		if (ret != CHECKSUM_SIZE) {
			printf("Failed to read %s: %d\n", devname, ret);
			close(fd);
			return;
		}
		printf("Download to valid binary\n");
	} else {
		/*Dose not write new crc. jump to header information. */
		ret = lseek(fd, CHECKSUM_SIZE, SEEK_SET);
		if (ret == ERROR) {
			printf("Failed to lseek %s\n", devname);
			close(fd);
			return;
		}
		printf("Download to invalid binary\n");
	}

	/* Write new version. */
	ret = write(fd, (FAR uint8_t *)header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to write %s: %d\n", devname, ret);
		close(fd);
		return;
	}

	close(fd);
}

static void binary_update_download_binary(char *devname, int condition)
{
	int fd;
	int ret;
	int file_size;
	int read_size;
	uint32_t crc_hash = 0;
	uint8_t crc_buffer[CRC_BUFFER_SIZE];
	binary_header_t header_data;

	printf("\n** Binary Update Download [%s] %s case test.\n", devname, condition == DOWNLOAD_VALID_BIN ? "Valid" : "Invalid");

	fd = open(devname, O_RDONLY);
	if (fd < 0) {
		fail_cnt++;
		printf("Failed to open %s: %d, errno: %d\n", devname, ret, get_errno());
		return;
	}

	/* Jump to header information. */
	ret = lseek(fd, CHECKSUM_SIZE, SEEK_SET);
	if (ret == ERROR) {
		printf("Failed to lseek %s\n", devname);
		close(fd);
		return;
	}

	/* Read the binary header. */
	ret = read(fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to read %s: %d\n", devname, ret);
		close(fd);
		return;
	}

	/* Version update */
	snprintf(header_data.bin_ver, BIN_VER_MAX, "%u", new_version);

	/* Version is updated to create a new crc. */
	if (condition == DOWNLOAD_VALID_BIN) {
		crc_hash = crc32part((uint8_t *)&header_data, header_data.header_size, crc_hash);
		file_size = header_data.bin_size;

		while (file_size > 0) {
			read_size = file_size < CRC_BUFFER_SIZE ? file_size : CRC_BUFFER_SIZE;
			ret = read(fd, (FAR uint8_t *)crc_buffer, read_size);
			if (ret != read_size) {
				printf("Failed to read %s: %d\n", devname, ret);
				close(fd);
				return;
			}
			crc_hash = crc32part(crc_buffer, read_size, crc_hash);
			file_size -= read_size;
		}
	}

	close(fd);

	binary_update_write_binary(devname, condition, crc_hash, &header_data);
}

static void binary_update_getinfo_all(void)
{
	int ret;
	binary_info_list_t bin_info_list;

	printf("\n** Binary Update GETINFO_ALL test.\n");
	ret = binary_manager_get_update_info_all(&bin_info_list);
	if (ret == OK) {
		print_binary_info_list(&bin_info_list);
	} else {
		fail_cnt++;
		printf("Get binary info all FAIL %d\n", ret);
	}
}

static void binary_update_getinfo(char *name, binary_info_t *bin_info)
{
	int ret;

	printf("\n** Binary Update GETINFO [%s] test.\n", name);
	ret = binary_manager_get_update_info(name, bin_info);
	if (ret == OK) {
		print_binary_info(bin_info);
	} else {
		printf("Get binary info FAIL, ret %d\n", ret);
	}
}

static void binary_update_reload(char *name)
{
	int ret;

	printf("\n** Binary Update RELOAD [%s] test.\n", name);
	ret = binary_manager_update_binary(name);
	if (ret == OK) {
		printf("RELOAD [%s] SUCCESS\n", name);
	} else {
		printf("Reload binary %s FAIL, ret %d\n", name, ret);
	}
}

static void binary_update_run_tests(int repetition_num)
{
	new_version++;

	binary_info_t pre_bin_info;
	binary_info_t cur_bin_info;
	printf("\n** Binary Update Example %d-th Iteration.\n", repetition_num);

	binary_update_getinfo(APP_NAME, &pre_bin_info);
	/* Test invalid APP binary download. */
	binary_update_download_binary(&pre_bin_info.dev_path, DOWNLOAD_INVALID_BIN);
	binary_update_reload(APP_NAME);
	binary_update_getinfo(APP_NAME, &cur_bin_info);

	binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);

	/* Test valid APP binary download. */
	binary_update_download_binary(&cur_bin_info.dev_path, DOWNLOAD_VALID_BIN);
	binary_update_reload(APP_NAME);
	binary_update_getinfo(APP_NAME, &cur_bin_info);

	binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_VALID_BIN);

	binary_update_getinfo_all();

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
	char *cmd_arg;
	char *cnt_arg;
	int execution_type;

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
			execution_type = EXEC_FINITE;
			cnt_arg = optarg;
			break;
		case '?':
		default:
			goto usage;
		}
	}

	/* Execute the binary update example. */
	if (execution_type == EXEC_INFINITE) {
		if (strncmp(cmd_arg, "start", strlen("start") + 1) == 0) {
			if (is_running) {
				goto already_running;
			}
			inf_flag = true;
			binary_update_execute_infinitely();
		} else if (strncmp(cmd_arg, "stop", strlen("stop") + 1) == 0) {
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
