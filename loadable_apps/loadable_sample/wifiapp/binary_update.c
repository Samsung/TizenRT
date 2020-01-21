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
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <crc32.h>

#include <sys/types.h>

#include <binary_manager/binary_manager.h>

/* App binary information for update test */
#define APP_NAME                   "micom"
#define APP_FACTORY_DEV            "/dev/mtdblock2"

#define EXEC_FINITE                 0
#define EXEC_INFINITE               1

#define DOWNLOAD_VALID_BIN          0
#define DOWNLOAD_INVALID_BIN        1

#define CHECKSUM_SIZE               4
#define BUFFER_SIZE                 512

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

static void binary_update_cb(void)
{
	printf(" ==========================================================================\n");
	printf("   The state changed callback is executed in WIFI. %s state is changed. \n", APP_NAME);
	printf(" ========================================================================= \n");
}

static void binary_update_download_binary(binary_update_info_t *binary_info, int condition)
{
	int read_fd;
	int write_fd;
	int new_version;
	int ret;
	int total_size;
	int copy_size;
	int read_size;
	uint32_t crc_hash = 0;
	uint8_t buffer[BUFFER_SIZE];
	binary_header_t header_data;
	char filepath[CONFIG_NAME_MAX];

	snprintf(filepath, CONFIG_PATH_MAX, "%s/%s_%s", BINARY_DIR_PATH, APP_NAME, binary_info->version);
	read_fd = open(filepath, O_RDONLY);
	if (read_fd < 0) {
		if (errno == ENOENT) {
			snprintf(filepath, CONFIG_PATH_MAX, "%s", APP_FACTORY_DEV);
			read_fd = open(APP_FACTORY_DEV, O_RDONLY);
			if (read_fd < 0) {
				fail_cnt++;
				printf("Failed to open %s: %d, errno: %d\n", APP_FACTORY_DEV, read_fd, get_errno());
				return;
			}
		} else {
			fail_cnt++;
			printf("Failed to open %s: %d, errno: %d\n", filepath, read_fd, get_errno());
			return;
		}
	}

	ret = lseek(read_fd, CHECKSUM_SIZE, SEEK_SET);
	if (ret == ERROR) {
		fail_cnt++;
		printf("Failed to lseek %d, errno %d\n", ret, errno);
		close(write_fd);
		return;
	}

	/* Read the binary header. */
	ret = read(read_fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		fail_cnt++;
		printf("Failed to read header %s: %d\n", APP_FACTORY_DEV, ret);
		close(read_fd);
		return;
	}

	new_version = atoi(header_data.bin_ver);
	new_version++;

	write_fd = binary_manager_open_new_entry(APP_NAME, new_version);
	if (write_fd < 0) {
		fail_cnt++;
		printf("Failed to create: version %d, ret %d, errno %d\n", new_version, write_fd, get_errno());
		close(read_fd);
		return;
	}

	/* Version update */
	snprintf(header_data.bin_ver, BIN_VER_MAX, "%u", new_version);

	/* Write to crc information. */
	ret = write(write_fd, (FAR uint8_t *)&crc_hash, CHECKSUM_SIZE);
	if (ret != CHECKSUM_SIZE) {
		printf("Failed to write %d\n", ret);
		goto errout_with_close_fds;
	}

	/* Write the binary header. */
	ret = write(write_fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to write header: %d\n", ret);
		goto errout_with_close_fds;
		return;
	}

	crc_hash = crc32part((uint8_t *)&header_data, header_data.header_size, crc_hash);

	/* Copy binary */
	total_size = header_data.bin_size;
	copy_size = 0;

	while (total_size > copy_size) {
		read_size = ((total_size - copy_size) < BUFFER_SIZE ? (total_size - copy_size) : BUFFER_SIZE);
		ret = read(read_fd, (FAR uint8_t *)buffer, read_size);
		if (ret != read_size) {
			printf("Failed to read buffer : %d\n", ret);
			goto errout_with_close_fds;
		}
		ret = write(write_fd, (FAR uint8_t *)buffer, read_size);
		if (ret != read_size) {
			printf("Failed to write buffer : %d\n", ret);
			goto errout_with_close_fds;
		}
		crc_hash = crc32part(buffer, read_size, crc_hash);
		copy_size += read_size;
		printf("Copy %s binary, %s [%d%%]\r", APP_NAME, filepath, copy_size * 100 / total_size);
	}
	printf("\nCopy SUCCESS\n");

	if (condition == DOWNLOAD_INVALID_BIN) {
		/* Invalid crc value */
		crc_hash++;
	}

	/* Write to crc information. */
	ret = lseek(write_fd, 0, SEEK_SET);
	if (ret == ERROR) {
		printf("Failed to lseek %d, errno %d\n", ret, errno);
		goto errout_with_close_fds;
	}

	/* Write new crc */
	ret = write(write_fd, (FAR uint8_t *)&crc_hash, CHECKSUM_SIZE);
	if (ret != CHECKSUM_SIZE) {
		printf("Failed to write %d\n", ret);
		goto errout_with_close_fds;
	}
	printf("Download binary %s Done!\n", filepath);

errout_with_close_fds:
	close(write_fd);
	close(read_fd);
	if (ret < 0) {
		fail_cnt++;
	}
	return;
}

static void print_binary_info(binary_update_info_t *binary_info)
{
	printf(" ============= binary [%s] info ============== \n", binary_info->name);
	printf(" %8s | %8s\n", "Version", "Available size");
	printf(" ------------------------------------------------ \n");
	printf(" %8s | %8d\n", binary_info->version, binary_info->available_size);
	printf(" ================================================ \n");
}

static void print_binary_info_list(binary_update_info_list_t *binary_info_list)
{
	int bin_idx;

	printf(" ================== ALL binary info : %d count ==================== \n", binary_info_list->bin_count);
	printf(" %4s | %6s | %8s | %8s\n", "Idx", "Name", "Version", "Available size");
	printf(" ---------------------------------------------------------------- \n");
	for (bin_idx = 0; bin_idx < binary_info_list->bin_count; bin_idx++) {
		printf(" %4d | %6s | %8s | %8d\n", bin_idx, \
		binary_info_list->bin_info[bin_idx].name, binary_info_list->bin_info[bin_idx].version, \
		binary_info_list->bin_info[bin_idx].available_size);
	}
	printf(" ================================================================ \n");
}

static void binary_update_check_test_result(binary_update_info_t *pre_bin_info, binary_update_info_t *cur_bin_info, int condition)
{
	printf(" ============ [%5s] Update info ============= \n", cur_bin_info->name);
	printf(" %4s | %8s \n", "Con", "Version");
	printf(" --------------------------------------------- \n");
	printf(" %4s | %8s \n", "Pre", pre_bin_info->version);
	printf(" %4s | %8s \n", "Cur", cur_bin_info->version);
	printf(" ============================================== \n");

	if (condition == DOWNLOAD_VALID_BIN) {
		if (strncmp(pre_bin_info->version, cur_bin_info->version, sizeof(pre_bin_info->version)) == 0) {
			fail_cnt++;
			printf("Fail to load valid higher version binary.\n");
		} else {
			printf("Success to load valid higher version binary.\n");
		}
	} else { //DOWNLOAD_INVALID_BIN
		if (strncmp(pre_bin_info->version, cur_bin_info->version, sizeof(pre_bin_info->version)) != 0) {
			fail_cnt++;
			printf("Warning! Load invalid binary.\n");
		} else {
			printf("No update with invalid binary.\n");
		}
	}
}

static void binary_update_getinfo_all(void)
{
	int ret;
	binary_update_info_list_t bin_info_list;

	printf("\n** Binary Update GETINFO_ALL test.\n");
	ret = binary_manager_get_update_info_all(&bin_info_list);
	if (ret == OK) {
		print_binary_info_list(&bin_info_list);
	} else {
		fail_cnt++;
		printf("Get binary info all FAIL %d\n", ret);
	}
}

static void binary_update_getinfo(char *name, binary_update_info_t *bin_info)
{
	int ret;

	printf("\n** Binary Update GETINFO [%s] test.\n", name);
	ret = binary_manager_get_update_info(name, bin_info);
	if (ret == OK) {
		print_binary_info(bin_info);
	} else {
		fail_cnt++;
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
		fail_cnt++;
		printf("Reload binary %s FAIL, ret %d\n", name, ret);
	}
}

static void binary_update_register_state_changed_callback(void)
{
	int ret;

	printf("\n** Binary Update Register state changed callback test.\n");
	ret = binary_manager_register_state_changed_callback((binmgr_statecb_t)binary_update_cb, NULL);
	if (ret == OK) {
		printf("Register state changed callback SUCCESS\n");
	} else {
		fail_cnt++;
		printf("Register state changed callback FAIL, ret %d\n", ret);
	}
}

static void binary_update_unregister_state_changed_callback(void)
{
	int ret;

	printf("\n** Binary Update Unregister state changed callback test.\n");
	ret = binary_manager_unregister_state_changed_callback();
	if (ret == OK) {
		printf("Unregister state changed callback SUCCESS\n");
	} else {
		fail_cnt++;
		printf("Unregister state changed callback FAIL, ret %d\n", ret);
	}
}

static void binary_update_same_version_test(void)
{
	int ret;
	int version;
	binary_update_info_t bin_info;

	binary_update_getinfo(APP_NAME, &bin_info);

	version = atoi(bin_info.version);

	/* Try to create binary file with old version */
	ret = binary_manager_open_new_entry(APP_NAME, version);
	if (ret == OK) {
		fail_cnt++;
		printf("Get binary info FAIL, ret %d\n", ret);
		close(ret);
	}
}

static void binary_update_new_version_test(void)
{
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	binary_update_getinfo(APP_NAME, &pre_bin_info);

	/* Copy current binary and update version. */
	binary_update_download_binary(&pre_bin_info, DOWNLOAD_VALID_BIN);

	binary_update_reload(APP_NAME);

	sleep(3);

	binary_update_getinfo(APP_NAME, &cur_bin_info);

	binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_VALID_BIN);
}

static void binary_update_invalid_binary_test(void)
{
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	binary_update_getinfo(APP_NAME, &pre_bin_info);

	/* Copy current binary and Write invalid crc. */
	binary_update_download_binary(&pre_bin_info, DOWNLOAD_INVALID_BIN);

	binary_update_reload(APP_NAME);

	sleep(3);

	binary_update_getinfo(APP_NAME, &cur_bin_info);

	binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);
}

static void binary_update_run_tests(int repetition_num)
{
	printf("\n** Binary Update Example %d-th Iteration.\n", repetition_num);

	/* Test old APP binary download. */
	binary_update_same_version_test();

	/* Test invalid APP binary download. */
	binary_update_invalid_binary_test();

	/* Register a callack for changed state */
	binary_update_register_state_changed_callback();

	/* Test valid APP binary download. */
	binary_update_new_version_test();

	/* Unregister registered callback */
	binary_update_unregister_state_changed_callback();

	/* Get information of all binaries */
	binary_update_getinfo_all();
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
 * binary_update_test
 ****************************************************************************/

void binary_update_test(void)
{
	binary_update_execute_ntimes(1);
}
