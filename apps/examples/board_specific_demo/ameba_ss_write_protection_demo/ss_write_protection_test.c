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
#include <errno.h>
#include <security/security_common.h>
#include <security/security_api.h>
#include <security/security_ss.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include <tinyara/fs/mtd.h>
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

static char rwbuf[MAX_SS_SIZE];

#ifdef CONFIG_AMEBAD_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x1a000;
static uint32_t SLOT1_START_ADDR = 0x1d000;
#endif
#ifdef CONFIG_AMEBALITE_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x24000;
static uint32_t SLOT1_START_ADDR = 0x27000;
#endif
#ifdef CONFIG_AMEBASMART_TRUSTZONE
/* ToDo: Temporary set need changes based flash addr */
static uint32_t SLOT0_START_ADDR = 0x2C000;
static uint32_t SLOT1_START_ADDR = 0x2F000;
#endif
static uint32_t SLOT_SIZE = 0x1000;
static uint32_t SLOT_AREA_OFFEST = 0x23000;

#ifdef CONFIG_MTD_BYTE_WRITE
static int flash_write(uint32_t slot_index, uint32_t len, uint8_t *data)
{
	uint32_t address;
	if (slot_index == 0) {
		address = SLOT0_START_ADDR;
	} else {
		address = SLOT1_START_ADDR + (SLOT_SIZE * (slot_index - 1));
	}

	int ret = -1;
	FAR uint8_t *buffer;
	FAR struct mtd_dev_s *dev_mtd = NULL;

	if (address == 0 || len <= 0) {
		printf("Invalid arguments, offset 0x%x, write size %d\n", address, len);
		return -1;
	}

	dev_mtd = up_flashinitialize();
	if (!dev_mtd) {
		printf("up_flashinitialize Failed\n");
		return -1;
	}

	buffer = (uint8_t *)malloc(MAX_SS_SIZE);
	if (!buffer) {
		printf("Failed to allocate buffer %d\n", MAX_SS_SIZE);
		return -1;
	}
	memset(buffer, data, len);

	if (dev_mtd->write != NULL) {
		ret = MTD_WRITE(dev_mtd, address, len, buffer);
	}

	if (ret <= 0) {
		printf("Write Failed : %d\n", ret);
		free(buffer);
		return -1;
	} else {
		printf("Write Success\n");
		printf("Length of written data : %d\n\n", ret);
	}
	
	free(buffer);
	
	return 0;
}
#endif

static int flash_read(uint32_t slot_index)
{
	uint32_t address;
	/* The Secure Storage Slot Flash Address , 156 = HEADER 4 + TEST_DATA_LENGTH 4 + DATA LENGTH + TAG 16 + FOOTER 4 , 1 = en_display*/
	uint32_t length = HEAD_LENGTH + DATA_LENGTH + TEST_DATA_LENGTH + TAG_LENGTH + HEAD_LENGTH;
	
	if (slot_index == 0) {
		address = SLOT0_START_ADDR;
	} else {
		address = SLOT1_START_ADDR + (SLOT_SIZE * (slot_index - 1));
	}

	printf("Read Raw data from Slot: %d, Area A\n", slot_index);
	if (!rtl_ss_flash_read(address, length, (uint8_t *)rwbuf, 1)) {
		printf("rtl_ss_flash_read() Failed, %d\n", __LINE__);
		return -1;
	}

	sleep(1);
	address += SLOT_AREA_OFFEST;
	printf("\nRead Raw data from Slot: %d, Area B\n", slot_index);
	if (!rtl_ss_flash_read(address, length, (uint8_t *)rwbuf, 1)) {
		printf("rtl_ss_flash_read() Failed, %d\n", __LINE__);
		return -1;
	}

	return 0;
}

static void print_ss_data(security_data *data)
{
	printf("data.length: %d\n", TEST_DATA_LENGTH);
	char *ptr = (char *)data->data;
	printf("data.data:\n");
	for (int i = 0; i < data->length; i++) {
		printf("%02x ", *(ptr + i));
		if (!((i + 1) % 16)) {
			printf("\n");
		}
	}
}

/****************************************************************************
 * secure_storage_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sstorage_main(int argc, char *argv[])
#endif
{
	security_handle hnd;
	security_error res;
	security_data data;
	uint32_t input_data = DEFAULT_TEST_DATA;
	uint32_t slot_index = TEST_SLOT_NUM;
	uint32_t address;
	uint32_t length = TEST_DATA_LENGTH;
	char ss_path[7];
	int option;
	int i;
	char *ptr;

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
	printf("Start Security init!!\n");
	res = security_init(&hnd);
	if (res != SECURITY_OK) {
		printf("security_init() Failed\n, %d", __LINE__);
		return 0;
	}
	printf("Security init complete\n");
	sleep(1);

	data.length = TEST_DATA_LENGTH;

	/* Form the SS path name based on the slot index where we want to read and write */
	snprintf(ss_path, 7, "ss/%d", slot_index);

	printf("Let us read once before writing to the slot\n");
	printf("ss_read_secure_storage() Slot: %d\n\n", slot_index);
	res = ss_read_secure_storage(hnd, ss_path, 0, &data);
	if (res != SECURITY_OK) {
		printf("ss_read_secure_storage() Failed, %d, res = %d\n", __LINE__, res);
		if (res == SECURITY_ERROR) {
			printf("Slot %d is empty\n", slot_index);
		}
	} else {
		print_ss_data(&data);
	}
	sleep(1);

	printf("\nStart Flash Raw data read!!\n");
	if (flash_read(slot_index) < 0){
		printf("flash_read() Failed\n");
		goto exit;
	}
	printf("Read complete\n");
	sleep(1);


	printf("\nStart Secure Storage write!!\n");
	/* Populate the buffer with the user-given data */
	memset(rwbuf, input_data, TEST_DATA_LENGTH);
	data.data = (void *)rwbuf;
	printf("ss_write_secure_storage() Slot: %d\n", slot_index);

	/* Write TEST_DATA_LENGTH bytes from buffer to the desired slot of the SS */
	res = ss_write_secure_storage(hnd, ss_path, 0, &data);
	if (res != SECURITY_OK) {
		printf("ss_write_secure_storage() Failed, %d, res = %d\n", __LINE__, res);
		goto exit;
	}
	printf("Write Complete\n");
	sleep(1);

	printf("\nStart Secure Storage read after ss_write_secure_storage()!!\n");
	printf("ss_read_secure_storage() Slot: %d\n\n", slot_index);
	res = ss_read_secure_storage(hnd, ss_path, 0, &data);
	if (res != SECURITY_OK) {
		printf("ss_read_secure_storage() Failed, %d, res = %d\n", __LINE__, res);
		goto exit;
	}
	print_ss_data(&data);
	sleep(1);

	printf("\nStart Flash Raw data read!!\n");
	if (flash_read(slot_index) < 0){
		printf("flash_read() Failed\n");
		goto exit;
	}
	printf("Read complete\n");
	sleep(1);

#ifdef CONFIG_MTD_BYTE_WRITE
	/* Write data to Secure Storage flash directly */
	printf("\nStart Flash Raw data write!!\n");

	printf("Write Raw data in Slot: %d, Area A\n", slot_index);
	printf("Data will be written in SS if write protection is disabled.\n");
	if (flash_write(address, length, (uint8_t *)rwbuf) < 0) {
		printf("flash_write Failed. %d\n", __LINE__);
		goto exit;
	}

	printf("Read Raw data after flash write\n");
	if (flash_read(slot_index) < 0){
		printf("flash_read() Failed\n");
		goto exit;
	}
	printf("Read Complete\n");
#else
	/* Flash write is not supported if disable MTD_BYTE_WRITE */
	printf("Cannot write data to secure storage directly\n");
	printf("Please enable MTD_BYTE_WRITE\n");
#endif // CONFIG_MTD_BYTE_WRITE
	printf("Ended SE Secure Storage testing!!\n");

exit:
	free(data.data);
	security_deinit(hnd);

	return 0;
}
