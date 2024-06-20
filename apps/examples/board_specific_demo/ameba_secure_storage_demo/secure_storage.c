/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <security/security_common.h>
#include <security/security_api.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include <security/security_ss.h>
#define TAG_LENGTH		16
#define DATA_LENGTH		4
#define HEAD_LENGTH		4
#define TEST_DATA_LENGTH	128
#define TEST_SLOT_NUM		0
#define MAX_SS_SIZE		1024
#define DEFAULT_TEST_DATA	0xAB

#define SHOW_USAGE printf("Usage: sstorage -s SLOT_INDEX -d DATA\n	\
	SLOT_INDEX : slot index to save the DATA, valid range 0 ~ 31\n	\
	DATA : value to be saved in byte\n");

extern int rtl_ss_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display);
static char rwbuf[MAX_SS_SIZE];
static uint32_t SLOT_SIZE = 0x1000;

#ifdef CONFIG_AMEBAD_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x1a000;
static uint32_t SLOT1_START_ADDR = 0x1d000;
static uint32_t SLOT_AREA_OFFSET = 0x23000;
#endif
#ifdef CONFIG_AMEBALITE_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x24000;
static uint32_t SLOT1_START_ADDR = 0x27000;
static uint32_t SLOT_AREA_OFFSET = 0x23000;
#endif
#ifdef CONFIG_AMEBASMART_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x2C000;
static uint32_t SLOT1_START_ADDR = 0x2F000;
static uint32_t SLOT_AREA_OFFSET = 0x2A000;
#endif


/****************************************************************************
 * secure_storage_main
 ****************************************************************************/
int check_flash_data_empty(uint8_t *data, uint32_t length)
{
	for (uint32_t i = 0; i < length; ++i) {
		if (data[i] != 0xFF) {
			return 0; /* Not empty */
		}
	}
	return 1; /* Empty */
}

void read_flash_data(const char *label, int slot_index, uint32_t address, uint32_t length)
{
	printf("\nStart Flash Raw data read!!\n");
	printf("\nRead Raw data from Slot: %d, %s\n", slot_index, label);
	if (!rtl_ss_flash_read(address, length, (uint8_t *)rwbuf, 1)) {
		printf("rtl_ss_flash_read() Fail, %d\n", __LINE__);
	}
	printf("Read complete\n");
}

void read_ss(security_handle hnd, const char *ss_path, uint32_t slot_index)
{
	printf("\nStart Secure Storage read!!\n");
	security_data data;
	security_error res;
	char *ptr;

	data.data = NULL;
	data.length = TEST_DATA_LENGTH;
	printf("ss_read_secure_storage() Slot: %d\n\n", slot_index);
	res = ss_read_secure_storage(hnd, ss_path, 0, &data);
	if (res != SECURITY_OK) {
		printf("ss_read_secure_storage() Fail, %d, res = %d\n", __LINE__, res);
	} else {
		printf("data.length: %d\n", data.length);
		ptr = (char *)data.data;
		printf("data.data:\n");
		for (int i = 0; i < data.length; i++) {
			printf("%02x ", *(ptr + i));
			if (!((i + 1) % 16)) {
				printf("\n");
			}
		}
	}
	/* Free the read buffer space allocated by the ss_read_secure_storage() function */
	free(data.data);
}

void write_ss(security_handle hnd, const char *ss_path, uint32_t input_data, uint32_t slot_index)
{
	printf("\nStart Secure Storage write!!\n");
	security_data data;
	security_error res;

	/* Populate the buffer with the user-given data */
	memset(rwbuf, input_data, TEST_DATA_LENGTH);
	data.data = (void *)rwbuf;
	data.length = TEST_DATA_LENGTH;

	printf("ss_write_secure_storage() Slot: %d\n", slot_index);
	/* Write TEST_DATA_LENGTH bytes from buffer to the desired slot of the SS */
	res = ss_write_secure_storage(hnd, ss_path, 0, &data);
	if (res != SECURITY_OK) {
		printf("ss_write_secure_storage() Fail, %d, res = %d\n", __LINE__, res);
	}
	printf("Write Complete\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sstorage_main(int argc, char *argv[])
#endif
{
	security_handle hnd;
	security_error res;
	uint32_t input_data = DEFAULT_TEST_DATA;
	uint32_t slot_index = TEST_SLOT_NUM;
	uint32_t address;
	uint32_t length = HEAD_LENGTH + DATA_LENGTH + TEST_DATA_LENGTH + TAG_LENGTH + HEAD_LENGTH;
	char ss_path[7];
	int area_a_empty;
	int area_b_empty;
	int empty;
	int flash_protected;
	int option;

	while ((option = getopt(argc, argv, "d:s:h")) > 0) {
		switch (option) {
		case 'd':
			sscanf(optarg, "%x", &input_data);
			break;
		case 's':
			slot_index = atoi(optarg);
			if (slot_index > 31) {
				printf("Slot index cannot be greater than 31, normalizing to 0\n");
				slot_index = 0;
			}
			break;
		default:
			SHOW_USAGE
			return 0;
		}
	}

	printf("Start testing SE Secure Storage!!\n");
	printf("Data: %2x, slot: %d\n", input_data, slot_index);

	/* 1. Initialize security */
	printf("Start Security init!!\n");
	res = security_init(&hnd);
	if (res != SECURITY_OK) {
		printf("security_init() Fail\n, %d", __LINE__);
		sleep(1);
	}
	printf("Security init complete\n");
	sleep(1);

	/* Form the SS path name based on the slot index where we want to read and write */
	snprintf(ss_path, 7, "ss/%d", slot_index);

	/* 2. Read Secure Storage flash data before perform write */
	printf("Let us read once before writing to the slot\n");
	read_ss(hnd, ss_path, slot_index);
	sleep(1);

	/* 3. Read flash raw data before perform write */
	address = (slot_index == 0) ? SLOT0_START_ADDR : SLOT1_START_ADDR + (SLOT_SIZE * (slot_index - 1));
	read_flash_data("Area A", slot_index, address, length);
	sleep(1);
	read_flash_data("Area B", slot_index, address + SLOT_AREA_OFFSET, length);
	sleep(1);

	/* 4. Write Secure Storage */
	write_ss(hnd, ss_path, input_data, slot_index);
	sleep(1);

	/* 5. Read Secure Storage flash data after perform write */
	read_ss(hnd, ss_path, slot_index);
	sleep(1);

	/* 6. Read flash raw data and Identify data exists Area after perform write */
	read_flash_data("Area A", slot_index, address, length);
	area_a_empty = check_flash_data_empty((uint8_t *)rwbuf, length);
	sleep(1);
	read_flash_data("Area B", slot_index, address + SLOT_AREA_OFFSET, length);
	area_b_empty = check_flash_data_empty((uint8_t *)rwbuf, length);

	printf("\n ***** Start Verify Secure Storage Area Protected.***** \n");
	/* 7. Retrieve the Flash Status Bit */
	flash_protected = rtl_verify_flash_protect();

	/* 8. Erase the existing data Area and Read flash raw data to verify the protection status */
	if (!area_a_empty) {
		printf("\nErase Area A where data existed.\n");
		ns_flash_erase(address);
		read_flash_data("Area A", slot_index, address, length);
		empty = check_flash_data_empty((uint8_t *)rwbuf, length);
	} else if (!area_b_empty) {
		printf("\nErase Area B where data existed.\n");
		ns_flash_erase(address + SLOT_AREA_OFFSET);
		read_flash_data("Area B", slot_index, address + SLOT_AREA_OFFSET, length);
		empty = check_flash_data_empty((uint8_t *)rwbuf, length);
	}

	if (flash_protected) {
		if (empty) {
			printf("ERROR: Erase success, but flash block protection detected and original data corrupted.\n");
		} else {
			printf("EXPECTED: Erase failed, flash block protection detected and original data retained.\n");
		}
	} else {
		if (empty) {
			printf("EXPECTED: Erase success, no flash block protection detected.\n");
		} else {
			printf("ERROR: Erase failed, no flash block protection detected.\n");
		}
	}

	/* 9. Deinitialize Security */
	printf("Start Security deinit!!\n");
	(void)security_deinit(hnd);

	printf("Ended SE Secure Storage testing!!\n");
	return 0;
}
