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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <security/security_common.h>
#include <security/security_api.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include <security/security_ss.h>
#include <tinyara/os_api_test_drv.h>

#define TAG_LENGTH		16
#define DATA_LENGTH		4
#define HEAD_LENGTH		4
#define TEST_DATA_LENGTH	128
#define TEST_SLOT_NUM		0
#define MAX_SS_SIZE		1024
#define DEFAULT_TEST_DATA	0xAB

static char rwbuf[MAX_SS_SIZE];
static uint32_t SLOT_SIZE = 0x1000;

static void show_usage(void)
{
	printf("Usage:\n");
	printf("  SS API Commands (via seclink driver):\n");
	printf("  sstorage ss_read  [-s SLOT]                    - Read via SS API\n");
	printf("  sstorage ss_write [-s SLOT] [-d DATA]          - Write via SS API\n");
	printf("  sstorage ss_erase [-s SLOT]                    - Erase via SS API\n");
	printf("  sstorage ss_full  [-s SLOT] [-d DATA]          - Full test (read->write->read->erase)\n\n");
	printf("  NS API Commands (via ioctl to os_api_test driver):\n");
	printf("  sstorage ns_read  [-s SLOT]                    - Read via NS API (ioctl)\n");
	printf("  sstorage ns_write [-s SLOT] [-d DATA]          - Write via NS API (ioctl)\n");
	printf("  sstorage ns_erase [-s SLOT]                    - Erase via NS API (ioctl)\n");
	printf("  sstorage ns_full  [-s SLOT] [-d DATA]          - Full test (ioctl)\n\n");
	printf("Options:\n");
	printf("  -s SLOT_INDEX : slot index (0-31), default 0\n");
	printf("  -d DATA : data value to write (hex), default 0xAB\n");
}

static void read_ss(security_handle hnd, const char *ss_path, uint32_t slot_index)
{
	printf("\nStart Secure Storage read!!\n");
	security_data data;
	security_error res;
	char *ptr;

	data.data = NULL;
	data.length = TEST_DATA_LENGTH;
	printf("ss_read_secure_storage() Slot: %u\n\n", slot_index);
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

static void write_ss(security_handle hnd, const char *ss_path, uint32_t input_data, uint32_t slot_index)
{
	printf("\nStart Secure Storage write!!\n");
	security_data data;
	security_error res;

	/* Populate the buffer with the user-given data */
	memset(rwbuf, input_data, TEST_DATA_LENGTH);
	data.data = (void *)rwbuf;
	data.length = TEST_DATA_LENGTH;

	printf("ss_write_secure_storage() Slot: %u\n", slot_index);
	/* Write TEST_DATA_LENGTH bytes from buffer to the desired slot of the SS */
	res = ss_write_secure_storage(hnd, ss_path, 0, &data);
	if (res != SECURITY_OK) {
		printf("ss_write_secure_storage() Fail, %d, res = %d\n", __LINE__, res);
	}
	printf("Write Complete\n");
}

static int do_ss_read(security_handle hnd, const char *ss_path, uint32_t slot_index)
{
	read_ss(hnd, ss_path, slot_index);
	return 0;
}

static int do_ss_write(security_handle hnd, const char *ss_path, uint32_t input_data, uint32_t slot_index)
{
	write_ss(hnd, ss_path, input_data, slot_index);
	return 0;
}

static int do_ss_erase(security_handle hnd, const char *ss_path, uint32_t slot_index)
{
	printf("\nStart Secure Storage erase!!\n");
	security_error res;
	
	printf("ss_delete_secure_storage() Slot: %u\n", slot_index);
	res = ss_delete_secure_storage(hnd, ss_path);
	if (res != SECURITY_OK) {
		printf("ss_delete_secure_storage() Fail, %d, res = %d\n", __LINE__, res);
		return -1;
	}
	printf("Erase Complete\n");
	return 0;
}

static int do_ss_full(security_handle hnd, const char *ss_path, uint32_t input_data, uint32_t slot_index)
{
	printf("\n=== SS Full Test ===\n");
	
	printf("Step 1: Initial Read\n");
	do_ss_read(hnd, ss_path, slot_index);
	sleep(1);
	
	printf("\nStep 2: Write\n");
	do_ss_write(hnd, ss_path, input_data, slot_index);
	sleep(1);
	
	printf("\nStep 3: Read after Write\n");
	do_ss_read(hnd, ss_path, slot_index);
	sleep(1);
	
	printf("\nStep 4: Erase\n");
	do_ss_erase(hnd, ss_path, slot_index);
	sleep(1);
	
	printf("\nStep 5: Read after Erase\n");
	do_ss_read(hnd, ss_path, slot_index);
	
	return 0;
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
	uint32_t input_data = DEFAULT_TEST_DATA;
	uint32_t slot_index = TEST_SLOT_NUM;
	uint32_t address;
	uint32_t length = HEAD_LENGTH + DATA_LENGTH + TEST_DATA_LENGTH + TAG_LENGTH + HEAD_LENGTH;
	char ss_path[7];


	/* Check if we have at least one argument (the subcommand) */
	if (argc < 2) {
		show_usage();
		return 0;
	}
	
	/* Command type flags */
	int cmd_type = -1;  /* 0=ss_read, 1=ss_write, 2=ss_erase, 3=ss_full */
						/* 4=ns_read, 5=ns_write, 6=ns_erase, 7=ns_full */
	
	/* Parse the subcommand */
	if (strcmp(argv[1], "ss_read") == 0) {
		cmd_type = 0;
	} else if (strcmp(argv[1], "ss_write") == 0) {
		cmd_type = 1;
	} else if (strcmp(argv[1], "ss_erase") == 0) {
		cmd_type = 2;
	} else if (strcmp(argv[1], "ss_full") == 0) {
		cmd_type = 3;
	} else if (strcmp(argv[1], "ns_read") == 0) {
		cmd_type = 4;
	} else if (strcmp(argv[1], "ns_write") == 0) {
		cmd_type = 5;
	} else if (strcmp(argv[1], "ns_erase") == 0) {
		cmd_type = 6;
	} else if (strcmp(argv[1], "ns_full") == 0) {
		cmd_type = 7;
	} else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		show_usage();
		return 0;
	} else {
		printf("Unknown command: %s\n", argv[1]);
		show_usage();
		return -1;
	}
	
	/* Manual argument parsing - getopt not working reliably in TASH */
	for (int i = 2; i < argc; i++) {
		if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
			slot_index = atoi(argv[i + 1]);
			if (slot_index > 31) {
				printf("Slot index cannot be greater than 31, normalizing to 0\n");
				slot_index = 0;
			}
			i++;  /* Skip next argument */
		} else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
			input_data = (uint32_t)strtol(argv[i + 1], NULL, 16);
			i++;  /* Skip next argument */
		}
	}
    
	printf("Start testing SE Secure Storage for Data: %2x, slot: %u\n", input_data, slot_index);

	/* Form the SS path name based on the slot index */
	snprintf(ss_path, 7, "ss/%u", slot_index);
	
	/* Calculate flash address based on slot index */
	address = (slot_index == 0) ? SLOT0_START_ADDR : SLOT1_START_ADDR + (SLOT_SIZE * (slot_index - 1));
	
	/* Execute the requested command */
	switch (cmd_type) {
	case 0:  /* ss_read */
	case 1:  /* ss_write */
	case 2:  /* ss_erase */
	case 3:  /* ss_full */
		/* Initialize security for SS operations */
		printf("Start Security init!!\n");
		res = security_init(&hnd);
		if (res != SECURITY_OK) {
			printf("security_init() Fail\n, %d", __LINE__);
			return -1;
		}
		printf("Security init complete\n");
		sleep(1);
		
		if (cmd_type == 0) {
			/* ss_read */
			do_ss_read(hnd, ss_path, slot_index);
		} else if (cmd_type == 1) {
			/* ss_write */
			do_ss_write(hnd, ss_path, input_data, slot_index);
		} else if (cmd_type == 2) {
			/* ss_erase */
			do_ss_erase(hnd, ss_path, slot_index);
		} else if (cmd_type == 3) {
			/* ss_full: read->write->read->erase */
			do_ss_full(hnd, ss_path, input_data, slot_index);
		}
		
		/* Deinitialize Security */
		printf("Start Security deinit!!\n");
		(void)security_deinit(hnd);
		printf("Security deinit complete\n");
		break;
		
	case 4:  /* ns_read */
	case 5:  /* ns_write */
	case 6:  /* ns_erase */
	case 7:  /* ns_full */
	{
		/* NS operations via unified ioctl to /dev/os_api_test */
		int fd;
		struct ns_flash_op_arg arg;
		int ret;
		
		fd = open(OS_API_TEST_DRVPATH, O_RDWR);
		if (fd < 0) {
			printf("Failed to open %s device, fd=%d\n", OS_API_TEST_DRVPATH, fd);
			break;
		}
		
		switch (cmd_type) {
		case 4:  /* ns_read */
			printf("\n=== NS Read Test (via ioctl) ===\n");
			/* Read Area A */
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_READ;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			printf("Reading Area A (slot=%d, addr=0x%x, len=%d)\n", slot_index, address, length);
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed for Area A: ret=%d\n", ret);
			}
			sleep(1);
			
			/* Read Area B */
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_READ;
			arg.slot_index = slot_index;
			arg.address = address + SLOT_AREA_OFFSET;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			printf("Reading Area B (slot=%d, addr=0x%x, len=%d)\n", slot_index, address + SLOT_AREA_OFFSET, length);
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed for Area B: ret=%d\n", ret);
			}
			break;
			
		case 5:  /* ns_write */
			printf("\n=== NS Write Test (via ioctl) ===\n");
			memset(rwbuf, input_data, length);
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_WRITE;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			arg.write_data = input_data;
			printf("Writing (slot=%d, addr=0x%x, len=%d, data=0x%x)\n", slot_index, address, length, input_data);
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_WRITE failed: ret=%d\n", ret);
			}
			break;
			
		case 6:  /* ns_erase */
			printf("\n=== NS Erase Test (via ioctl) ===\n");
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_ERASE;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			printf("Erasing (slot=%d, addr=0x%x)\n", slot_index, address);
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_ERASE failed: ret=%d\n", ret);
			}
			break;
			
		case 7:  /* ns_full */
			printf("\n ***** Start NS API Full Test (via ioctl) ***** \n");
			
			/* 1. Read flash raw data before write - Area A */
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_READ;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			printf("Step 1a: Read Area A before write\n");
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed for Area A: ret=%d\n", ret);
			}
			sleep(1);
			
			/* Read Area B */
			arg.address = address + SLOT_AREA_OFFSET;
			printf("Step 1b: Read Area B before write\n");
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed for Area B: ret=%d\n", ret);
			}
			sleep(1);
			
			/* 2. Write flash raw data */
			memset(rwbuf, input_data, length);
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_WRITE;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			arg.write_data = input_data;
			printf("Step 2: Write\n");
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_WRITE failed: ret=%d\n", ret);
			}
			sleep(1);
			
			/* 3. Read flash raw data after write */
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_READ;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			printf("Step 3a: Read Area A after write\n");
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed for Area A: ret=%d\n", ret);
			}
			sleep(1);
			
			arg.address = address + SLOT_AREA_OFFSET;
			printf("Step 3b: Read Area B after write\n");
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed for Area B: ret=%d\n", ret);
			}
			sleep(1);
			
			/* 4. Erase with protection verification (handled in driver) */
			printf("\nStep 4: Erase with protection verification\n");
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_ERASE;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			sleep(1);
			
			/* 5. Read after erase */
			printf("\nStep 5: Read after Erase\n");
			memset(&arg, 0, sizeof(arg));
			arg.op_type = NS_FLASH_OP_READ;
			arg.slot_index = slot_index;
			arg.address = address;
			arg.length = length;
			arg.buffer = (uint8_t *)rwbuf;
			ret = ioctl(fd, TESTIOC_NS_FLASH_OP, (unsigned long)&arg);
			if (ret < 0) {
				printf("ioctl NS_FLASH_OP_READ failed after erase: ret=%d\n", ret);
			}
			break;
		}
		
		close(fd);
		break;
	}
	}
	
	printf("Ended SE Secure Storage testing!!\n");
	return 0;
}
