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
#include <security/security_common.h>
#include <security/security_api.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include <security/security_ss.h>
#define SE_IOTIVITY_SVR_DB 0
#define TAG_LENGTH 16
#define DATA_LENGTH 4
#define HEAD_LENGTH 4
#define TEST_DATA_LENGTH 128;
#define TEST_SLOT_NUM 4
#define MAX_SS_SIZE 8192

extern int rtl_ss_flash_read(uint32_t address, uint32_t len, uint8_t *data, int en_display);
static char a[MAX_SS_SIZE];
static char b[MAX_SS_SIZE];

#ifdef CONFIG_AMEBAD_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x1a000;
static uint32_t SLOT1_START_ADDR = 0x1d000;
#endif
#ifdef CONFIG_AMEBALITE_TRUSTZONE
static uint32_t SLOT0_START_ADDR = 0x24000;
static uint32_t SLOT1_START_ADDR = 0x27000;
#endif
static uint32_t SLOT_SIZE = 0x1000;
static uint32_t SLOT_AREA_OFFEST = 0x23000;

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
	uint32_t slot;
	uint32_t address;
	uint32_t length = TEST_DATA_LENGTH;
	char ss_path[7];

	printf("Start testing SE Secure Storage!!\n");
	printf("Start Security init!!\n");
	sleep(1);
	res = security_init(&hnd);
	if (res != SECURITY_OK) {
		printf("security_init() Fail\n", __LINE__);
		sleep(1);
	}

	sleep(1);
	data.data = (void *)a;
	data.length = length;

	printf("Start Secure Storage write!!\n");
	for (slot = 0; slot < TEST_SLOT_NUM; slot++) {
		snprintf(ss_path, 7, "ss/%d", slot);
		memset(a, (0xA0 + slot), length);
		a[length] = '\0';
		printf("ss_write_secure_storage() Slot: %d\n", slot);
		sleep(1);
		res = ss_write_secure_storage(hnd, ss_path, 0, &data);
		if (res != SECURITY_OK) {
			printf("ss_write_secure_storage() Fail\n", __LINE__);
		}
	}

	printf("Start Secure Storage read!!\n");
	sleep(1);
	for (slot = 0; slot < TEST_SLOT_NUM; slot++) {
		snprintf(ss_path, 7, "ss/%d", slot);
		printf("ss_read_secure_storage() Slot: %d\n", slot);
		sleep(1);
		res = ss_read_secure_storage(hnd, ss_path, 0, &data);
		if (res != SECURITY_OK) {
			printf("ss_read_secure_storage() Fail\n", __LINE__);
		} else {
			printf("data.length: %d\n", data.length);
			printf("data.data:\n");
			memcpy(b, data.data, data.length);
			for (int i = 0; i < data.length; i++) {
				printf("%02x ", b[i]);
				if (!((i + 1) % 16)) {
					printf("\n");
				}
			}
		}
	}

	printf("Start Flash Raw data read!!\n");
	for (slot = 0; slot < TEST_SLOT_NUM; slot++) {
		/* The Secure Storage Slot Flash Address , 156 = HEADER 4 + TEST_DATA_LENGTH 4 + DATA LENGTH + TAG 16 + FOOTER 4 , 1 = en_display*/
		length = HEAD_LENGTH + DATA_LENGTH + TEST_DATA_LENGTH + TAG_LENGTH + HEAD_LENGTH;
		if (slot == 0) {
			address = SLOT0_START_ADDR;
		} else {
			address = SLOT1_START_ADDR + (SLOT_SIZE * (slot - 1));
		}

		sleep(1);
		printf("Read Raw data from Slot: %d, Area A\n", slot);
		if (!rtl_ss_flash_read(address, length, a, 1)) {
			printf("rtl_ss_flash_read() Fail\n", __LINE__);
		}

		sleep(1);
		address += SLOT_AREA_OFFEST;
		printf("Read Raw data from Slot: %d, Area B\n", slot);
		if (!rtl_ss_flash_read(address, length, a, 1)) {
			printf("rtl_ss_flash_read() Fail\n", __LINE__);
		}
	}

	free(data.data);

	printf("Start Security deinit!!\n");
	(void)security_deinit(hnd);
	sleep(1);

	printf("Ended SE Secure Storage testing!!\n");
	return 0;
}
