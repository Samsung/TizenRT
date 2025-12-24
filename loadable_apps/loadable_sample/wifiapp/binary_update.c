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
#ifdef CONFIG_MMINFO
#include <tinyara/fs/ioctl.h>
#include <tinyara/mminfo.h>
#endif
#include <tinyara/binary_manager.h>

#include <binary_manager/binary_manager.h>

/* Kernel, common binary information for update test */
#define KERNEL                  "kernel"
#define COMMON                  "common"

/* App binary information for update test */

#ifdef CONFIG_APP1_BIN_NAME
#define APP1_NAME                  CONFIG_APP1_BIN_NAME
#endif

#if (CONFIG_NUM_APPS > 1)
#ifdef CONFIG_APP2_BIN_NAME
#define APP2_NAME                  CONFIG_APP2_BIN_NAME
#endif
#endif

#define EXEC_FINITE                 0
#define EXEC_INFINITE               1

#define DOWNLOAD_VALID_BIN          0
#define DOWNLOAD_INVALID_BIN        1

#define CHECKSUM_SIZE               4
#define BUFFER_SIZE                 512

static volatile bool is_running;
static volatile bool inf_flag = true;
static int fail_cnt = 0;
static int update_type_flag = 0;

static void binary_update_cb(void)
{
	printf(" ==========================================================================\n");
	printf("   The state changed callback is executed in WIFI. %s state is changed. \n", APP1_NAME);
	printf(" ========================================================================= \n");
}

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
	if (strcmp(binary_info->name, KERNEL) == 0) {
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
	} else if (strcmp(binary_info->name, COMMON) == 0) {
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

static int binary_update_same_version_test(char *bin_name)
{
	int ret;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;
	update_type_flag = 0;

	printf("\n** Binary Update Same Version Test. **\n");

	/* Get pre-update info for app binary */
	ret = binary_update_getinfo(bin_name, &pre_bin_info);
	if (ret != OK) {
		printf("Failed to get pre-update info for %s\n", bin_name);
		return ret;
	}

	/* Download and set bootparam for all binaries with same version */
	printf("\n** Download and set bootparam for APP1 binary. (same version) **\n");
	ret = download_binary_and_set_bootparam(bin_name, false, DOWNLOAD_VALID_BIN, BINARY_USERAPP);
	if (ret != OK) {
		printf("Failed to download and set bootparam for %s\n", bin_name);
		return ret;
	}

	ret = binary_update_reload();
	if (ret != OK) {
		printf("Failed to reload binaries\n");
		return ret;
	}

	/* Get post-update info and check results for all binaries */
	ret = binary_update_getinfo(bin_name, &cur_bin_info);
	if (ret != OK) {
		printf("Failed to get post-update info for %s\n", bin_name);
		return ret;
	}

	ret = binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);
	if (ret != OK) {
		printf("Same version test failed for %s\n", bin_name);
		return ret;
	}

	printf("Same version test passed for all binaries\n");
	return OK;
}

static int binary_update_new_version_test(char *bin_name)
{
	int ret;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;
	update_type_flag = 0;
	bool version_up;

	printf("\n** Binary Update New Version Test. **\n");

	/* Get pre-update info for all binaries */
	ret = binary_update_getinfo(bin_name, &pre_bin_info);
	if (ret != OK) {
		printf("Failed to get pre-update info for %s\n", bin_name);
		return ret;
	}

	/* Download and set bootparam for all binaries with new version */
	printf("\n** Download and set bootparam for app binary. (new version) **\n");
	version_up = true;
	ret = download_binary_and_set_bootparam(bin_name, version_up, DOWNLOAD_VALID_BIN, BINARY_USERAPP);
	if (ret != OK) {
		printf("Failed to download and set bootparam for %s\n", bin_name);
		return ret;
	}

	ret = binary_update_reload();
	if (ret != OK) {
		return ret;
	}

	/* Get post-update info and check results for all binaries */
	ret = binary_update_getinfo(bin_name, &cur_bin_info);
	if (ret != OK) {
		printf("Failed to get post-update info for %s\n", bin_name);
		return ret;
	}

	ret = binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_VALID_BIN);
	if (ret != OK) {
		printf("New version test failed for %s\n", bin_name);
		return ret;
	}

	printf("New version test passed for all binaries\n");
	return OK;
}

static int binary_update_invalid_binary_test(char *bin_name)
{
	int ret;
	binary_update_info_t pre_bin_info;
	binary_update_info_t cur_bin_info;
	update_type_flag = 0;
	int condition;

	printf("\n** Binary Update Invalid binary update Test. **\n");

	/* Get pre-update info for all binaries */
	ret = binary_update_getinfo(bin_name, &pre_bin_info);
	if (ret != OK) {
		printf("Failed to get pre-update info for %s\n", bin_name);
		return ret;
	}

	/* Download and set bootparam for all binaries with invalid CRC (Only app1 or app2 binary is invalid) */
	printf("\n** Download and set bootparam for app binary. (invalid CRC) **\n");
	condition = DOWNLOAD_INVALID_BIN;
	ret = download_binary_and_set_bootparam(bin_name, true, condition, BINARY_USERAPP);
	if (ret != OK) {
		printf("Failed to download and set bootparam for %s\n", bin_name);
		return ret;
	}

	ret = binary_update_reload();
	if (ret != OK) {
		printf("Failed to reload binaries\n");
		return ret;
	}

	/* Get post-update info and check results for all binaries */
	ret = binary_update_getinfo(bin_name, &cur_bin_info);
	if (ret != OK) {
		printf("Failed to get post-update info for %s\n", bin_name);
		return ret;
	}

	ret = binary_update_check_test_result(&pre_bin_info, &cur_bin_info, DOWNLOAD_INVALID_BIN);
	if (ret != OK) {
		printf("Invalid binary test failed for %s\n", bin_name);
		return ret;
	}

	printf("Invalid binary test passed for all binaries\n");
	return OK;
}

static void binary_update_run_tests(void)
{
	int ret;

	/* 1. Get info all test. */
	binary_update_getinfo_all();

	/* 2. Reload test with same version. */
	ret = binary_update_same_version_test(APP1_NAME);
	if (ret != OK) {
		printf("Same version test failed\n");
		return ret;
	}

	/* 3. Reload test with invalid binary. */
	ret = binary_update_invalid_binary_test(APP1_NAME);
	if (ret != OK) {
		printf("Invalid binary test failed\n");
		return ret;
	}

	/* 4. Register a callack for changed state. */
	binary_update_register_state_changed_callback();

	/* 5. Reload test with new version. */
	ret = binary_update_new_version_test(APP1_NAME);
	if (ret != OK) {
		printf("New version test failed\n");
		return ret;
	}

	/* 6. Unregister registered callback. */
	binary_update_unregister_state_changed_callback();
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

/****************************************************************************
 * binary_update_test_with_type
 ****************************************************************************/
void binary_update_test_with_type(const char *test_type)
{
	if (test_type == NULL) {
		/* Default: run all tests */
		binary_update_run_tests();
		return;
	}

	if (strcmp(test_type, "same_version") == 0) {
		binary_update_same_version_test(APP1_NAME);
	} else if (strcmp(test_type, "new_version") == 0) {
		binary_update_new_version_test(APP1_NAME);
	} else if (strcmp(test_type, "invalid_binary") == 0) {
		binary_update_invalid_binary_test(APP1_NAME);
	} else if (strcmp(test_type, "all_tests") == 0) {
		binary_update_run_tests();
	} else {
		printf("Unknown test type: %s\n", test_type);
		printf("Available test types: same_version, new_version, invalid_binary, all_tests\n");
	}
}
#endif

#ifdef CONFIG_EXAMPLES_UPDATE_AGING_TEST
/****************************************************************************
 * binary_update_aging_test
 ****************************************************************************/
void binary_update_aging_test(void)
{
	int ret;
	int randval;
	binary_update_info_t bin_info;
#ifdef CONFIG_MMINFO
	int fd;
	struct mallinfo data;

	fd = open(MMINFO_DRVPATH, O_RDWR);
	if (fd < 0) {
		printf("Open %s Fail, %d errno %d\n", MMINFO_DRVPATH, fd, get_errno());
	}
#endif

	while (1) {
#ifdef CONFIG_MMINFO
		if (fd >= 0) {
			ret = ioctl(fd, MMINFOIOC_HEAP, (int)&data);
			if (ret == OK) {
				printf("=================== Memory Information ===================\n");
				printf("              total       used       free    largest\n");
				printf(" Mem:	%11d%11d%11d%11d\n", data.arena, data.uordblks, data.fordblks, data.mxordblk);
				printf("===========================================================\n");
			}
		}
#endif
		/* Iterate update test with random binary */
		randval = rand();
		if (randval % 2 == 0) {
			ret = binary_update_new_version_test(APP1_NAME);
		} else {
			ret = binary_update_new_version_test(APP2_NAME);
		}
		if (ret != OK) {
			break;
		}
	}

	printf(" Binary update aging Test EXIT!! \n");
#ifdef CONFIG_MMINFO
	if (fd >= 0) {
		close(fd);
	}
#endif
}
#endif
