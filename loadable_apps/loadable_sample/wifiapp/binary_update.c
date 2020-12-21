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

#define NEW_APP_NAME               "newapp"
#define NEW_APP_VERSION            20200421

#define EXEC_FINITE                 0
#define EXEC_INFINITE               1

#define DOWNLOAD_VALID_BIN          0
#define DOWNLOAD_INVALID_BIN        1

#define CHECKSUM_SIZE               4
#define BUFFER_SIZE                 512

static volatile bool is_running;
static volatile bool inf_flag = true;
static int fail_cnt = 0;

static void binary_update_cb(void)
{
	printf(" ==========================================================================\n");
	printf("   The state changed callback is executed in WIFI. %s state is changed. \n", APP_NAME);
	printf(" ========================================================================= \n");
}

static int binary_update_download_binary(binary_update_info_t *binary_info, int condition)
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
	char filepath[BINARY_PATH_LEN];
	char new_filepath[BINARY_PATH_LEN] = {0,};

	snprintf(filepath, BINARY_PATH_LEN, "%s/%s_%u", BINARY_DIR_PATH, binary_info->name, (uint32_t)binary_info->version);
	read_fd = open(filepath, O_RDONLY);
	if (read_fd < 0) {
		fail_cnt++;
		printf("Failed to open %s: %d, errno: %d\n", filepath, read_fd, get_errno());
		return ERROR;
	}

	/* Read the binary header. */
	ret = read(read_fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to read header %s: %d\n", filepath, ret);
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	new_version = header_data.bin_ver + 1;

	ret = binary_manager_get_download_path(binary_info->name, new_version, new_filepath);
	if (ret < 0) {
		printf("binary_manager_get_download_path FAIL %d,\n", ret);
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	write_fd = open(new_filepath, O_WRONLY);
	if (write_fd < 0) {
		printf("Failed to open file %s: errno %d\n", new_filepath, get_errno());
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	/* Version update */
	header_data.bin_ver = new_version;

	/* Write the binary header. */
	ret = write(write_fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to write header: %d\n", ret);
		ret = ERROR;
		goto errout_with_close_fd2;
	}

	copy_size = 0;
	total_size = header_data.bin_size;
	crc_hash = crc32part((uint8_t *)&header_data + CHECKSUM_SIZE, header_data.header_size, crc_hash);

	/* Copy binary */
	while (total_size > copy_size) {
		read_size = ((total_size - copy_size) < BUFFER_SIZE ? (total_size - copy_size) : BUFFER_SIZE);
		ret = read(read_fd, (FAR uint8_t *)buffer, read_size);
		if (ret != read_size) {
			printf("Failed to read buffer : %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		ret = write(write_fd, (FAR uint8_t *)buffer, read_size);
		if (ret != read_size) {
			printf("Failed to write buffer : %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		crc_hash = crc32part(buffer, read_size, crc_hash);
		copy_size += read_size;
		printf("Copy %s binary, %s [%d%%]\r", binary_info->name, filepath, copy_size * 100 / total_size);
	}
	printf("\nCopy SUCCESS\n");

	if (condition == DOWNLOAD_INVALID_BIN) {
		/* Invalid crc value */
		crc_hash++;
	}

	/* Write new crc value. */
	ret = lseek(write_fd, 0, SEEK_SET);
	if (ret != 0) {
		printf("Failed to lseek %d, errno %d\n", ret, errno);
		ret = ERROR;
		goto errout_with_close_fd2;
	}
	ret = write(write_fd, (FAR uint8_t *)&crc_hash, CHECKSUM_SIZE);
	if (ret != CHECKSUM_SIZE) {
		printf("Failed to write %d\n", ret);
		ret = ERROR;
		goto errout_with_close_fd2;
	}
	printf("Download binary %s version %d Done!\n", binary_info->name, new_version);

	ret = OK;
errout_with_close_fd2:
	close(write_fd);
errout_with_close_fd1:
	close(read_fd);
	if (ret < 0) {
		fail_cnt++;
		unlink(new_filepath);
	}

	return ret;
}

static int binary_update_download_new_binary(void)
{
	int read_fd;
	int write_fd;
	int ret;
	uint32_t total_size;
	uint32_t copy_size;
	uint32_t read_size;
	uint32_t crc_hash = 0;
	uint8_t buffer[BUFFER_SIZE];
	binary_header_t header_data;
	binary_update_info_t bin_info;
	char filepath[BINARY_PATH_LEN];
	char new_filepath[BINARY_PATH_LEN] = {0,};
	int filesize;

	/* Get 'micom' binary info */
	binary_manager_get_update_info(APP_NAME, &bin_info);

	snprintf(filepath, BINARY_PATH_LEN, "%s/%s_%u", BINARY_DIR_PATH, APP_NAME, (uint32_t)bin_info.version);

	read_fd = open(filepath, O_RDONLY);
	if (read_fd < 0) {
		fail_cnt++;
		printf("Failed to open %s: %d, errno: %d\n", filepath, read_fd, get_errno());
		return ERROR;
	}

	/* Get 'micom' binary file size and check it with available size */
	filesize = lseek(read_fd, 0, SEEK_END);
	if (filesize <= 0 || bin_info.available_size <= 0 || filesize >= bin_info.available_size) {
		printf("Can't copy file, size %d, available size %d in fs\n", filesize, bin_info.available_size);
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	/* Set file offset to the start of file */
	ret = lseek(read_fd, 0, SEEK_SET);
	if (ret < 0) {
		printf("Failed to set fileoffset %s: %d\n", filepath, get_errno());
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	/* Read the binary header. */
	ret = read(read_fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to read header %s: %d\n", filepath, ret);
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	ret = binary_manager_get_download_path(NEW_APP_NAME, NEW_APP_VERSION, new_filepath);
	if (ret < 0) {
		printf("binary_manager_get_download_path FAIL %d,\n", ret);
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	write_fd = open(new_filepath, O_WRONLY);
	if (write_fd < 0) {
		printf("Failed to open file %s: errno %d\n", new_filepath, get_errno());
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	/* Update header data : name, version */
	strncpy(header_data.bin_name, NEW_APP_NAME, BIN_NAME_MAX);
	header_data.bin_ver = NEW_APP_VERSION;

	/* Write the binary header. */
	ret = write(write_fd, (FAR uint8_t *)&header_data, sizeof(binary_header_t));
	if (ret != sizeof(binary_header_t)) {
		printf("Failed to write header: %d\n", ret);
		ret = ERROR;
		goto errout_with_close_fd2;
	}

	crc_hash = crc32part((uint8_t *)&header_data + CHECKSUM_SIZE, header_data.header_size, crc_hash);

	/* Copy binary */
	total_size = header_data.bin_size;
	copy_size = 0;

	while (total_size > copy_size) {
		read_size = ((total_size - copy_size) < BUFFER_SIZE ? (total_size - copy_size) : BUFFER_SIZE);
		ret = read(read_fd, (FAR uint8_t *)buffer, read_size);
		if (ret != read_size) {
			printf("Failed to read buffer : %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		ret = write(write_fd, (FAR uint8_t *)buffer, read_size);
		if (ret != read_size) {
			printf("Failed to write buffer : %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		crc_hash = crc32part(buffer, read_size, crc_hash);
		copy_size += read_size;
		printf("Copy %s binary from %s [%d%%]\r", NEW_APP_NAME, filepath, copy_size * 100 / total_size);
	}
	printf("\nCopy SUCCESS\n");

	/* Write to crc information. */
	ret = lseek(write_fd, 0, SEEK_SET);
	if (ret != 0) {
		printf("Failed to lseek %d, errno %d\n", ret, errno);
		ret = ERROR;
		goto errout_with_close_fd2;
	}

	/* Write new crc */
	ret = write(write_fd, (FAR uint8_t *)&crc_hash, CHECKSUM_SIZE);
	if (ret != CHECKSUM_SIZE) {
		printf("Failed to write %d\n", ret);
		ret = ERROR;
		goto errout_with_close_fd2;
	}
	printf("Download binary %s Done!\n", NEW_APP_NAME);

	ret = OK;
errout_with_close_fd2:
	close(write_fd);
errout_with_close_fd1:
	close(read_fd);
	if (ret < 0) {
		fail_cnt++;
		unlink(new_filepath);
	}

	return ret;
}

static void print_binary_info(binary_update_info_t *binary_info)
{
	printf(" =========== binary [%s] info ============ \n", binary_info->name);
	printf(" %10s | %8s\n", "Version", "Available size");
	printf(" -------------------------------------------- \n");
	printf(" %8.1lf | %8d\n", binary_info->version, binary_info->available_size);
	printf(" ============================================ \n");
}

static void print_binary_info_list(binary_update_info_list_t *binary_info_list)
{
	int bin_idx;

	printf(" ============== ALL binary info : %d count ================ \n", binary_info_list->bin_count);
	printf(" %4s | %6s | %10s | %8s\n", "Idx", "Name", "Version", "Available size");
	printf(" -------------------------------------------------------- \n");
	for (bin_idx = 0; bin_idx < binary_info_list->bin_count; bin_idx++) {
		printf(" %4d | %6s | %8.1lf | %8d\n", bin_idx, \
		binary_info_list->bin_info[bin_idx].name, binary_info_list->bin_info[bin_idx].version, \
		binary_info_list->bin_info[bin_idx].available_size);
	}
	printf(" ======================================================== \n");
}

static int binary_update_check_test_result(binary_update_info_t *pre_bin_info, binary_update_info_t *cur_bin_info, int condition)
{
	int ret = ERROR;

	printf(" ========== [%5s] Update info =========== \n", cur_bin_info->name);
	printf(" %4s | %10s \n", "Con", "Version");
	printf(" ----------------------------------------- \n");
	printf(" %4s | %8.1lf \n", "Pre", pre_bin_info->version);
	printf(" %4s | %8.1lf \n", "Cur", cur_bin_info->version);
	printf(" ========================================== \n");

	if (condition == DOWNLOAD_VALID_BIN) {
		if (pre_bin_info->version == cur_bin_info->version) {
			fail_cnt++;
			printf("Fail to load valid higher version binary.\n");
		} else {
			ret = OK;
			printf("Success to load valid higher version binary.\n");
		}
	} else { //DOWNLOAD_INVALID_BIN
		if (pre_bin_info->version != cur_bin_info->version) {
			fail_cnt++;
			printf("Warning! Load invalid binary.\n");
		} else {
			ret = OK;
			printf("No update with invalid binary.\n");
		}
	}

	return ret;	
}

static int binary_update_getinfo_all(void)
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

	return ret;
}

static int binary_update_getinfo(char *name, binary_update_info_t *bin_info)
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

	return ret;
}

static int binary_update_reload(char *name)
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

	return ret;
}

static int binary_update_register_state_changed_callback(void)
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

	return ret;
}

static int binary_update_unregister_state_changed_callback(void)
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

	return ret;
}

static void binary_update_same_version_test(void)
{
	int ret;	
	char filepath[BINARY_PATH_LEN];
	binary_update_info_t bin_info;

	ret = binary_update_getinfo(APP_NAME, &bin_info);
	if (ret != OK) {
		return;
	}

	/* Try to create binary file with old version */
	ret = binary_manager_get_download_path(APP_NAME, (uint32_t)bin_info.version, filepath);
	if (ret == OK) {
		fail_cnt++;
		printf("Get binary info FAIL, ret %d\n", ret);
		close(ret);
	}
}

static void binary_update_new_version_test(void)
{
	int ret;
	char filepath[BINARY_PATH_LEN];
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	ret = binary_update_getinfo(APP_NAME, &pre_bin_info);
	if (ret != OK) {
		return;
	}

	/* Copy current binary and update version. */
	ret = binary_update_download_binary(&pre_bin_info, DOWNLOAD_VALID_BIN);
	if (ret != OK) {
		return;
	}

	ret = binary_update_reload(APP_NAME);
	if (ret != OK) {
		return;
	}

	sleep(2);

	ret = binary_update_getinfo(APP_NAME, &cur_bin_info);
	if (ret != OK) {
		return;
	}

	ret = binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_VALID_BIN);
	if (ret == OK) {
		/* Unlink binary file with old version */
		snprintf(filepath, BINARY_PATH_LEN, "%s/%s_%u", BINARY_DIR_PATH, APP_NAME, (uint32_t)pre_bin_info.version);
		unlink(filepath);
	}
}

static void binary_update_invalid_binary_test(void)
{
	int ret;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	ret = binary_update_getinfo(APP_NAME, &pre_bin_info);
	if (ret != OK) {
		return;
	}

	/* Copy current binary and Write invalid crc. */
	ret = binary_update_download_binary(&pre_bin_info, DOWNLOAD_INVALID_BIN);
	if (ret != OK) {
		return;
	}

	ret = binary_update_reload(APP_NAME);
	if (ret != OK) {
		return;
	}

	sleep(2);

	ret = binary_update_getinfo(APP_NAME, &cur_bin_info);
	if (ret != OK) {
		return;
	}

	binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);
}

static void binary_update_new_binary_test(void)
{
	int ret;
	char filepath[BINARY_PATH_LEN];
	binary_update_info_t bin_info;

	/* Copy current binary and update version. */
	ret = binary_update_download_new_binary();
	if (ret != OK) {
		return;
	}

	ret = binary_update_reload(NEW_APP_NAME);
	if (ret != OK) {
		return;
	}

	sleep(2);

	ret = binary_update_getinfo(NEW_APP_NAME, &bin_info);
	if (ret != OK) {
		return;
	}

	/* Get all binary information */
	ret = binary_update_getinfo_all();
	if (ret != OK) {
		return;
	}

	/* Unlink binary file */
	snprintf(filepath, BINARY_PATH_LEN, "%s/%s_%u", BINARY_DIR_PATH, NEW_APP_NAME, NEW_APP_VERSION);
	unlink(filepath);
}

static void binary_update_run_tests(void)
{
	/* 1. Reload test with same version. */
	binary_update_same_version_test();

	/* 2. Reload test with invalid binary. */
	binary_update_invalid_binary_test();

	/* 3. Register a callack for changed state. */
	binary_update_register_state_changed_callback();

	/* 4. Reload test with new version. */
	binary_update_new_version_test();

	/* 5. Unregister registered callback. */
	binary_update_unregister_state_changed_callback();

	/* 6. Reload test with new binary. */
	binary_update_new_binary_test();
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
		printf("\n** Binary Update Example %d-th Iteration.\n", repetition_num);
		binary_update_run_tests();
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
		binary_update_run_tests();
	}
	binary_update_show_success_ratio(repetition_num);
	is_running = false;
}

#ifdef CONFIG_EXAMPLES_BINARY_UPDATE_TEST
/****************************************************************************
 * binary_update_test
 ****************************************************************************/
void binary_update_test(void)
{
	binary_update_execute_ntimes(1);
}
#endif

#ifdef CONFIG_EXAMPLES_UPDATE_AGING_TEST
/****************************************************************************
 * binary_update_aging_test
 ****************************************************************************/
void binary_update_aging_test(void)
{
	int ret;
	binary_update_info_t bin_info;

	while (1) {
		ret = binary_update_getinfo(APP_NAME, &bin_info);
		if (ret != OK) {
			return;
		}

		/* Copy current binary with a higher version and valid crc. */
		ret = binary_update_download_binary(&bin_info, DOWNLOAD_VALID_BIN);
		if (ret != OK) {
			return;
		}

		sleep(5);

		ret = binary_update_reload(APP_NAME);
		if (ret != OK) {
			return;
		}
	}
}
#endif
