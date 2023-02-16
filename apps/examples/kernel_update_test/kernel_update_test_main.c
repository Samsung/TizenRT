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
#ifdef CONFIG_MMINFO
#include <tinyara/fs/ioctl.h>
#include <tinyara/mminfo.h>
#endif
#include <tinyara/binary_manager.h>

#include <binary_manager/binary_manager.h>

/* Kernel binary information for update test */
#define KERNEL                  "kernel"

#define DOWNLOAD_VALID_BIN          0
#define DOWNLOAD_INVALID_BIN        1

#define CHECKSUM_SIZE               4
#define BUFFER_SIZE                 512

static int fail_cnt = 0;

static int binary_update_download_binary(binary_update_info_t *binary_info, bool version_up, int condition)
{
	int read_fd;
	int write_fd;
	int ret;
	int total_size;
	int copy_size;
	int read_size;
	uint32_t crc_hash = 0;
	uint8_t buffer[BUFFER_SIZE];
	char origin_path[BINARY_PATH_LEN];
	char download_path[BINARY_PATH_LEN];
	kernel_binary_header_t header_data;

	ret = binary_manager_get_current_path(binary_info->name, origin_path);
	if (ret < 0) {
		printf("binary_manager_get_current_path FAIL, ret %d\n", ret);
		ret = ERROR;
		goto errout;
	} else {
		printf("path to %s's currently used partition %s\n", binary_info->name, origin_path);
	}

	read_fd = open(origin_path, O_RDONLY);
	if (read_fd < 0) {
		fail_cnt++;
		printf("Failed to open %s: %d, errno: %d\n", origin_path, read_fd, get_errno());
		return ERROR;
	}

	/* Read the binary header. */
	ret = read(read_fd, (FAR uint8_t *)&header_data, sizeof(kernel_binary_header_t));
	if (ret != sizeof(kernel_binary_header_t)) {
		printf("Failed to read header %s: %d\n", origin_path, ret);
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	ret = binary_manager_get_download_path(binary_info->name, download_path);
	if (ret < 0) {
		printf("binary_manager_get_download_path FAIL, ret %d\n", ret);
		ret = ERROR;
		goto errout_with_close_fd1;
	} else {
		printf("path to %s's inactive partition %s\n", binary_info->name, download_path);
	}

	write_fd = open(download_path, O_WRONLY);
	if (write_fd < 0) {
		printf("Failed to open %s: errno %d\n", download_path, get_errno());
		ret = ERROR;
		goto errout_with_close_fd1;
	}

	/* Version update */
	if (version_up) {
		header_data.version = binary_info->version + 1;
	}

	/* Write the binary header. */
	ret = write(write_fd, (FAR uint8_t *)&header_data, sizeof(kernel_binary_header_t));
	if (ret != sizeof(kernel_binary_header_t)) {
		printf("Failed to write header: %d\n", ret);
		ret = ERROR;
		goto errout_with_close_fd2;
	}

	copy_size = 0;
	total_size = header_data.binary_size;
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
		printf("Copy %s binary to %s [%d%%]\r", binary_info->name, download_path, copy_size * 100 / total_size);
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
	printf("Download binary %s version %d Done!\n", binary_info->name, header_data.version);

	ret = OK;
errout_with_close_fd2:
	close(write_fd);
errout_with_close_fd1:
	close(read_fd);
errout:
	if (ret < 0) {
		fail_cnt++;
	}

	return ret;
}

static void print_binary_info(binary_update_info_t *binary_info)
{
	printf(" =========== binary [%s] info ============ \n", binary_info->name);
	printf(" %10s | %8s\n", "Version", "Available size");
	printf(" -------------------------------------------- \n");
	printf(" %8.1u | %8d\n", binary_info->version, binary_info->available_size);
	printf(" ============================================ \n");
}

static void print_binary_info_list(binary_update_info_list_t *binary_info_list)
{
	int bin_idx;

	printf(" ============== ALL binary info : %d count ================ \n", binary_info_list->bin_count);
	printf(" %4s | %6s | %10s | %8s\n", "Idx", "Name", "Version", "Available size");
	printf(" -------------------------------------------------------- \n");
	for (bin_idx = 0; bin_idx < binary_info_list->bin_count; bin_idx++) {
		printf(" %4d | %6s | %8.1u | %8d\n", bin_idx, \
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
	printf(" %4s | %8.1u \n", "Pre", pre_bin_info->version);
	printf(" %4s | %8.1u \n", "Cur", cur_bin_info->version);
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

static int binary_update_reload(void)
{
	int ret;

	printf("\n** Binary Update RELOAD test.\n");
	ret = binary_manager_update_binary();
	if (ret == OK) {
		printf("RELOAD SUCCESS\n");
	} else {
		fail_cnt++;
		printf("Reload binary FAIL, ret %d\n", ret);
	}

	return ret;
}

static int binary_update_same_version_test(void)
{
	int ret;
	uint8_t type = 0;
	binary_setbp_result_t result;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	printf("\n** Binary Update Same Version Test. **\n");

	ret = binary_update_getinfo(KERNEL, &pre_bin_info);
	if (ret != OK) {
		return ret;
	}

	/* Copy current binary with no version update */
	ret = binary_update_download_binary(&pre_bin_info, false, DOWNLOAD_VALID_BIN);
	if (ret != OK) {
		printf("\nfailed to update same binary\n");
		return ret;
	}

	sleep(2);

	BM_SET_GROUP(type, BINARY_KERNEL);
	ret = binary_manager_set_bootparam(type, &result);
	if (ret != OK) {
		if (ret == BINMGR_ALREADY_UPDATED) {
			int idx;
			for (idx = 0; idx < BINARY_TYPE_MAX; idx++) {
				printf("[%d] result %d\n", idx, result.result[idx]);
			}
		}
		return ret;
	}

	ret = binary_update_reload();
	if (ret != OK) {
		return ret;
	}

	ret = binary_update_getinfo(KERNEL, &cur_bin_info);
	if (ret != OK) {
		return ret;
	}

	return binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);
}

static int binary_update_new_version_test(char *bin_name)
{
	int ret;
	uint8_t type = 0;
	binary_setbp_result_t result;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	printf("\n** Binary Update New Version Test. **\n");

	ret = binary_update_getinfo(bin_name, &pre_bin_info);
	if (ret != OK) {
		return ret;
	}

	/* Copy current binary and update version. */
	ret = binary_update_download_binary(&pre_bin_info, true, DOWNLOAD_VALID_BIN);
	if (ret != OK) {
		return ret;
	}

	BM_SET_GROUP(type, BINARY_KERNEL);
	ret = binary_manager_set_bootparam(type, &result);
	printf("binary_manager_set_bootparam %d\n", ret);
	if (ret != OK) {
		if (ret == BINMGR_ALREADY_UPDATED) {
			int idx;
			for (idx = 0; idx < BINARY_TYPE_MAX; idx++) {
				printf("[%d] result %d\n", idx, result.result[idx]);
			}
		}
		return ret;
	}

	ret = binary_update_reload();
	if (ret != OK) {
		return ret;
	}

	sleep(2);

	ret = binary_update_getinfo(bin_name, &cur_bin_info);
	if (ret != OK) {
		return ret;
	}

	return binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_VALID_BIN);
}

static void binary_update_invalid_binary_test(void)
{
	int ret;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;

	printf("\n** Binary Update Invalid binary update Test. **\n");

	ret = binary_update_getinfo(KERNEL, &pre_bin_info);
	if (ret != OK) {
		return;
	}

	/* Copy current binary and Write invalid crc. */
	ret = binary_update_download_binary(&pre_bin_info, true, DOWNLOAD_INVALID_BIN);
	if (ret != OK) {
		return;
	}

	ret = binary_update_reload();
	if (ret != OK) {
		return;
	}

	sleep(2);

	ret = binary_update_getinfo(KERNEL, &cur_bin_info);
	if (ret != OK) {
		return;
	}

	binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);
}

static void binary_update_run_tests(void)
{
	/* 1. Get info all test. */
	binary_update_getinfo_all();

	/* 2. Reload test with same version. */
	binary_update_same_version_test();

	/* 3. Reload test with invalid binary. */
	binary_update_invalid_binary_test();

	/* 4. Reload test with new version. */
	binary_update_new_version_test(KERNEL);

}

/****************************************************************************
 * binary_update_aging_test
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int kernel_update_main(int argc, char *argv[])
#endif
{
	binary_update_run_tests();	
}
