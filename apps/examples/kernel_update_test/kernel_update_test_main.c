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

/* For ERROR and OK definitions */
#include <errno.h>
#include <sys/types.h>

/* Kernel binary information for update test */
#define KERNEL                  "kernel"

/* Common/app1 binary names for testing */
/* Kernel / Common / App1 binary must be set at once */
#define COMMON                   "common"
#define APP1                     "app1"

#define DOWNLOAD_VALID_BIN          0
#define DOWNLOAD_INVALID_BIN        1

#define CHECKSUM_SIZE               4
#define BUFFER_SIZE                 512

static int fail_cnt = 0;
static int update_type_flag = 0;

static void print_binary_info(binary_update_info_t *binary_info)
{
	printf(" =========== binary [%s] info ============ \n", binary_info->name);
	printf(" %10s | %8s\n", "Version", "Available size");
	printf(" -------------------------------------------- \n");
	printf(" %8.1u | %8d\n", binary_info->version, binary_info->available_size);
	printf(" ============================================ \n");
}

static int binary_update_getinfo(char *name, binary_update_info_t *bin_info)
{
	int ret;

	printf("\n** Binary Update GETINFO [%s]\n", name);
	ret = binary_manager_get_update_info(name, bin_info);
	if (ret == OK) {
		print_binary_info(bin_info);
	} else {
		fail_cnt++;
		printf("Get binary info FAIL, ret %d\n", ret);
	}

	return ret;
}

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
	
	/* Union to handle different binary header types */
	union {
		user_binary_header_t user_header;
		kernel_binary_header_t kernel_header;
		common_binary_header_t common_header;
	} header_data;
	
	int header_size;
	uint32_t binary_size;
	uint32_t version;

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

	/* Determine binary type and read appropriate header */
	if (strcmp(binary_info->name, "kernel") == 0) {
		/* Kernel binary */
		ret = read(read_fd, (FAR uint8_t *)&header_data.kernel_header, sizeof(kernel_binary_header_t));
		if (ret != sizeof(kernel_binary_header_t)) {
			printf("Failed to read kernel header %s: %d\n", origin_path, ret);
			ret = ERROR;
			goto errout_with_close_fd1;
		}
		header_size = sizeof(kernel_binary_header_t);
		binary_size = header_data.kernel_header.binary_size;
		version = header_data.kernel_header.version;
		
		if (version_up) {
			printf("current version: %d\n", header_data.kernel_header.version);
			header_data.kernel_header.version = binary_info->version + 1;
			printf("new version: %d\n", header_data.kernel_header.version);
		}
	} else if (strcmp(binary_info->name, "common") == 0) {
		/* Common binary */
		ret = read(read_fd, (FAR uint8_t *)&header_data.common_header, sizeof(common_binary_header_t));
		if (ret != sizeof(common_binary_header_t)) {
			printf("Failed to read common header %s: %d\n", origin_path, ret);
			ret = ERROR;
			goto errout_with_close_fd1;
		}
		header_size = sizeof(common_binary_header_t);
		binary_size = header_data.common_header.bin_size;
		version = header_data.common_header.version;
		
		if (version_up) {
			printf("current version: %d\n", header_data.common_header.version);
			header_data.common_header.version = binary_info->version + 1;
			printf("new version: %d\n", header_data.common_header.version);
		}
	} else {
		/* User application binary (app1, app2, etc.) */
		ret = read(read_fd, (FAR uint8_t *)&header_data.user_header, sizeof(user_binary_header_t));
		if (ret != sizeof(user_binary_header_t)) {
			printf("Failed to read user header %s: %d\n", origin_path, ret);
			ret = ERROR;
			goto errout_with_close_fd1;
		}
		header_size = sizeof(user_binary_header_t);
		binary_size = header_data.user_header.bin_size;
		version = header_data.user_header.bin_ver;
		
		if (version_up) {
			printf("current version: %d\n", header_data.user_header.bin_ver);
			header_data.user_header.bin_ver = binary_info->version + 1;
			printf("new version: %d\n", header_data.user_header.bin_ver);
		}
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

	/* Write the binary header based on type */
	if (strcmp(binary_info->name, "kernel") == 0) {
		ret = write(write_fd, (FAR uint8_t *)&header_data.kernel_header, sizeof(kernel_binary_header_t));
		if (ret != sizeof(kernel_binary_header_t)) {
			printf("Failed to write kernel header: %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		crc_hash = crc32part((uint8_t *)&header_data.kernel_header + CHECKSUM_SIZE, header_data.kernel_header.header_size, crc_hash);
	} else if (strcmp(binary_info->name, "common") == 0) {
		ret = write(write_fd, (FAR uint8_t *)&header_data.common_header, sizeof(common_binary_header_t));
		if (ret != sizeof(common_binary_header_t)) {
			printf("Failed to write common header: %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		crc_hash = crc32part((uint8_t *)&header_data.common_header + CHECKSUM_SIZE, header_data.common_header.header_size, crc_hash);
	} else {
		ret = write(write_fd, (FAR uint8_t *)&header_data.user_header, sizeof(user_binary_header_t));
		if (ret != sizeof(user_binary_header_t)) {
			printf("Failed to write user header: %d\n", ret);
			ret = ERROR;
			goto errout_with_close_fd2;
		}
		crc_hash = crc32part((uint8_t *)&header_data.user_header + CHECKSUM_SIZE, header_data.user_header.header_size, crc_hash);
	}

	copy_size = 0;
	total_size = binary_size;

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
	
	/* Print version based on binary type */
	if (strcmp(binary_info->name, "kernel") == 0) {
		printf("Download binary %s version %d Done!\n", binary_info->name, header_data.kernel_header.version);
	} else if (strcmp(binary_info->name, "common") == 0) {
		printf("Download binary %s version %d Done!\n", binary_info->name, header_data.common_header.version);
	} else {
		printf("Download binary %s version %d Done!\n", binary_info->name, header_data.user_header.bin_ver);
	}

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

static int download_binary_and_set_bootparam(char *bin_name, bool version_up, int condition, uint8_t binary_type)
{
	int ret;
	binary_setbp_result_t result;
	binary_update_info_t bin_info;

	ret = binary_update_getinfo(bin_name, &bin_info);
	if (ret != OK) {
		printf("Failed to get info for %s\n", bin_name);
		return ret;
	}

	/* Download binary */
	ret = binary_update_download_binary(&bin_info, version_up, condition);
	if (ret != OK) {
		printf("Failed to download %s binary\n", bin_name);
		return ret;
	}

	/* Wait a bit more to ensure the binary is properly written */
	sleep(2);

	/* Set boot parameter */
	BM_SET_GROUP(update_type_flag, binary_type);
	ret = binary_manager_set_bootparam(update_type_flag, &result);
	if (ret != OK) {
		if (ret == BINMGR_ALREADY_UPDATED) {
			int idx;
			for (idx = 0; idx < BINARY_TYPE_MAX; idx++) {
				printf("[%d] result %d\n", idx, result.result[idx]);
			}
		} else {
			printf("binary_manager_set_bootparam FAIL for %s, ret %d\n", bin_name, ret);
		}
		return ret;
	}

	printf("Successfully downloaded and set bootparam for %s\n", bin_name);
	return OK;
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
	binary_update_info_t pre_bin_info[3];
	binary_update_info_t cur_bin_info[3];
	char *bin_names[] = {KERNEL, COMMON, APP1};
	uint8_t bin_types[] = {BINARY_KERNEL, BINARY_COMMON, BINARY_USERAPP};
	int bin_count = sizeof(bin_names) / sizeof(bin_names[0]);
	int i;
	update_type_flag = 0;

	printf("\n** Binary Update Same Version Test. **\n");

	/* Get pre-update info for all binaries */
	for (i = 0; i < bin_count; i++) {
		ret = binary_update_getinfo(bin_names[i], &pre_bin_info[i]);
		if (ret != OK) {
			printf("Failed to get pre-update info for %s\n", bin_names[i]);
			return ret;
		}
	}

	/* Download and set bootparam for all binaries with same version */
	printf("\n** Download and set bootparam for kernel binary. (same version) **\n");
	printf("** (Both common and app1 binary will be updated with same version) **\n");
	for (i = 0; i < bin_count; i++) {
		ret = download_binary_and_set_bootparam(bin_names[i], false, DOWNLOAD_VALID_BIN, bin_types[i]);
		if (ret != OK) {
			printf("Failed to download and set bootparam for %s\n", bin_names[i]);
			return ret;
		}
	}

	ret = binary_update_reload();
	if (ret != OK) {
		printf("Failed to reload binaries\n");
		return ret;
	}

	/* Get post-update info and check results for all binaries */
	for (i = 0; i < bin_count; i++) {
		ret = binary_update_getinfo(bin_names[i], &cur_bin_info[i]);
		if (ret != OK) {
			printf("Failed to get post-update info for %s\n", bin_names[i]);
			return ret;
		}

		ret = binary_update_check_test_result(&pre_bin_info[i], &cur_bin_info[i], DOWNLOAD_INVALID_BIN);
		if (ret != OK) {
			printf("Same version test failed for %s\n", bin_names[i]);
			return ret;
		}
	}

	printf("Same version test passed for all binaries\n");
	return OK;
}

static int binary_update_new_version_test(void)
{
	int ret;
	binary_update_info_t pre_bin_info[3];
	binary_update_info_t cur_bin_info[3];
	char *bin_names[] = {KERNEL, COMMON, APP1};
	uint8_t bin_types[] = {BINARY_KERNEL, BINARY_COMMON, BINARY_USERAPP};
	int bin_count = sizeof(bin_names) / sizeof(bin_names[0]);
	int i;
	update_type_flag = 0;

	printf("\n** Binary Update New Version Test. **\n");

	/* Get pre-update info for all binaries */
	for (i = 0; i < bin_count; i++) {
		ret = binary_update_getinfo(bin_names[i], &pre_bin_info[i]);
		if (ret != OK) {
			printf("Failed to get pre-update info for %s\n", bin_names[i]);
			return ret;
		}
	}

	/* Download and set bootparam for all binaries with new version */
	printf("\n** Download and set bootparam for kernel binary. (new version) **\n");
	printf("** (Both common and app1 binary will be updated with same version) **\n");
	for (i = 0; i < bin_count; i++) {
		int version_up = false;
		if (bin_names[i] == KERNEL) {
			version_up = true;
		}
		ret = download_binary_and_set_bootparam(bin_names[i], version_up, DOWNLOAD_VALID_BIN, bin_types[i]);
		if (ret != OK) {
			printf("Failed to download and set bootparam for %s\n", bin_names[i]);
			return ret;
		}
	}

	ret = binary_update_reload();
	if (ret != OK) {
		printf("Failed to reload binaries\n");
		return ret;
	}

	/* Get post-update info and check results for all binaries */
	for (i = 0; i < bin_count; i++) {
		ret = binary_update_getinfo(bin_names[i], &cur_bin_info[i]);
		if (ret != OK) {
			printf("Failed to get post-update info for %s\n", bin_names[i]);
			return ret;
		}

		ret = binary_update_check_test_result(&pre_bin_info[i], &cur_bin_info[i], DOWNLOAD_VALID_BIN);
		if (ret != OK) {
			printf("New version test failed for %s\n", bin_names[i]);
			return ret;
		}
	}

	printf("New version test passed for all binaries\n");
	return OK;
}

static int binary_update_invalid_binary_test(void)
{
	int ret;
	binary_update_info_t pre_bin_info[3];
	binary_update_info_t cur_bin_info[3];
	char *bin_names[] = {KERNEL, COMMON, APP1};
	uint8_t bin_types[] = {BINARY_KERNEL, BINARY_COMMON, BINARY_USERAPP};
	int bin_count = sizeof(bin_names) / sizeof(bin_names[0]);
	int i;
	update_type_flag = 0;

	printf("\n** Binary Update Invalid binary update Test. **\n");

	/* Get pre-update info for all binaries */
	for (i = 0; i < bin_count; i++) {
		ret = binary_update_getinfo(bin_names[i], &pre_bin_info[i]);
		if (ret != OK) {
			printf("Failed to get pre-update info for %s\n", bin_names[i]);
			return ret;
		}
	}

	/* Download and set bootparam for all binaries with invalid CRC (Only kernel binary is invalid) */
	printf("\n** Download and set bootparam for kernel binary. (invalid CRC) **\n");
	printf("** (Both common and app1 binary will be updated with same version & valid CRC) **\n");
	for (i = 0; i < bin_count; i++) {
		int condition = DOWNLOAD_VALID_BIN;
		if (bin_names[i] == KERNEL) {
			condition = DOWNLOAD_INVALID_BIN;
		}
		ret = download_binary_and_set_bootparam(bin_names[i], true, condition, bin_types[i]);
		if (ret != OK) {
			printf("Failed to download and set bootparam for %s\n", bin_names[i]);
			return ret;
		}
	}

	ret = binary_update_reload();
	if (ret != OK) {
		printf("Failed to reload binaries\n");
		return ret;
	}

	/* Get post-update info and check results for all binaries */
	for (i = 0; i < bin_count; i++) {
		ret = binary_update_getinfo(bin_names[i], &cur_bin_info[i]);
		if (ret != OK) {
			printf("Failed to get post-update info for %s\n", bin_names[i]);
			return ret;
		}

		ret = binary_update_check_test_result(&pre_bin_info[i], &cur_bin_info[i], DOWNLOAD_INVALID_BIN);
		if (ret != OK) {
			printf("Invalid binary test failed for %s\n", bin_names[i]);
			return ret;
		}
	}

	printf("Invalid binary test passed for all binaries\n");
	return OK;
}

static int binary_update_run_tests(void)
{
	int ret;

	/* 1. Get info all test. */
	binary_update_getinfo_all();

	/* 2. Reload test with same version. */
	ret = binary_update_same_version_test();
	if (ret != OK) {
		printf("Same version test failed\n");
		return ret;
	}

	/* 3. Reload test with invalid binary. */
	ret = binary_update_invalid_binary_test();
	if (ret != OK) {
		printf("Invalid binary test failed\n");
		return ret;
	}

	/* 4. Reload test with new version. */
	ret = binary_update_new_version_test();
	if (ret != OK) {
		printf("New version test failed\n");
		return ret;
	}

	printf("All tests completed successfully\n");
	return OK;
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
