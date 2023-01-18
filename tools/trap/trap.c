/*******************************************************************
*
* Copyright 2016 Samsung Electronics All Rights Reserved.
*
* File Name : trap.c
* Description: Receive RAM and/or Userfs dump using UART
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
******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "trap.h"
#include "../../os/include/tinyara/config.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_SMARTFS_MAXNAMLEN
#define USERFS_MAXNAMLEN 32
#endif

/****************************************************************************
 * Private Variables
 ****************************************************************************/

rd_regionx *mem_info = NULL;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

int ramdump_info_init(int dev_fd)
{
	char c;
	int i;
	int ret;
	uint32_t mem_address;
	uint32_t mem_size;

	/* Receive number of memory regions from TARGET */
	ret = read(dev_fd, &c, 1);
	if (ret != 1) {
		printf("Receiving number of regions failed, ret = %d\n", ret);
		return -1;
	}
	number_of_regions = c;

	/* Allocate memory to ramdump info structure */
	mem_info = (rd_regionx *)malloc((number_of_regions + 2) * sizeof(rd_regionx));
	if (!mem_info) {
		return -1;
	}

	/* Receive memory address, size & heap index for memory regions from TARGET */
	for (i = 2; i <= number_of_regions + 1; i++) {
		ret = b_read(dev_fd, (uint8_t *)&mem_address, 4);
		if (ret != 4) {
			printf("Receiving ram region address failed, ret = %d\n", ret);
			return -1;
		}
		mem_info[i].rd_regionx_start = mem_address;

		ret = b_read(dev_fd, (uint8_t *)&mem_size, 4);
		if (ret != 4) {
			printf("Receiving ram region size failed, ret = %d\n", ret);
			return -1;
		}
		mem_info[i].rd_regionx_size = mem_size;

		ret = read(dev_fd, &c, 1);
		if (ret != 1) {
			printf("Receiving ram region index failed, ret = %d\n", ret);
			return -1;
		}
		mem_info[i].rd_regionx_idx = c;
	}

	return 0;
}

static int ramdump_recv(int dev_fd)
{
	int i;
	int ret;
	int index;
	int regions_to_dump = 0;
	uint32_t ramdump_size;
	char ramdump_region[2] = { '\0' };
	char bin_file[BINFILE_NAME_SIZE] = { '\0' };
	FILE *bin_fp;

	/* Display memory region options for user to dump */
	printf("\n=========================================================================\n");
	printf("Ramdump Region Options:\n");
	printf("1. ALL");
	if (number_of_regions == 1) {
		printf("\t( Address: %08x, Size: %d )\n", mem_info[2].rd_regionx_start, mem_info[2].rd_regionx_size);
	} else {
		for (index = 2; index <= number_of_regions + 1; index++) {
			printf("\n%d. Region %d:\t( Address: 0x%08x, Size: %d )\t [Heap index = %d]", \
				index, index - 2, mem_info[index].rd_regionx_start, \
					mem_info[index].rd_regionx_size, mem_info[index].rd_regionx_idx);
		}
	}
	printf("\n=========================================================================\n");
	printf("\nPlease enter desired ramdump option as below:\n \t1 for ALL\n");
	if (number_of_regions > 1) {
	printf(" \t2 for Region 0\n \t25 for Region 0 & 3 ...\n");
	}

	/* Take user's input for desired ramdump region */
	printf("Please enter your input: ");
	scanf("%d", &index);

scan_input:
	/* Check if user's input is valid */
	while (index < 0 || index == EOF || (number_of_regions == 1 && index > 1)) {
		printf("Please enter correct input: ");
		scanf("%d", &index);
	}

	/* Mark regions to be dumped */
	while (index > 0) {
		if ((index % 10) > (number_of_regions + 1)) {
			index = -1;
			goto scan_input;
		}
		mem_info[index % 10].rd_regionx_mark = 1;
		index = index / 10;
		regions_to_dump++;
	}

	/* If any digit in input integer is 1, dump all regions */
	if (mem_info[1].rd_regionx_mark) {
		mem_info[1].rd_regionx_mark = 0;
		regions_to_dump = number_of_regions;
		for (i = 2; i <= number_of_regions + 1; i++) {
			mem_info[i].rd_regionx_mark = 1;
		}
	}

	/* Send number of regions to dump to TARGET */
	snprintf(ramdump_region, 2, "%d", regions_to_dump);
	ret = send_region_info(dev_fd, ramdump_region);
	if (ret != 0) {
		printf("Receiving number of regions to be dumped failed, ret = %d\n", ret);
		return -1;
	}
	printf("\n%s: No. of Regions to be dumped received\n", __func__);
	printf("\nReceiving ramdump......\n\n");

	/* Dump data region wise */
	for (index = 2; index <= number_of_regions + 1; index++) {
		if (mem_info[index].rd_regionx_mark == 1) {

			/* Send region index to TARGET */
			snprintf(ramdump_region, 2, "%d", index - 2);
			ret = send_region_info(dev_fd, ramdump_region);
			if (ret != 0) {
				printf("Receiving region index failed, ret = %d\n", ret);
				return -1;
			}

			snprintf(bin_file, BINFILE_NAME_SIZE, "ramdump_0x%08x_0x%08x.bin", \
				mem_info[index].rd_regionx_start, (mem_info[index].rd_regionx_start +  mem_info[index].rd_regionx_size));
			printf("=========================================================================\n");
			if (number_of_regions == 1) {
				printf("Dumping data, Address: 0x%08x, Size: %dbytes\n", \
					mem_info[index].rd_regionx_start,  mem_info[index].rd_regionx_size);
			} else {
				printf("Dumping Region: %d, Address: 0x%08x, Size: %dbytes\n", \
					index - 2,  mem_info[index].rd_regionx_start,  mem_info[index].rd_regionx_size);
			}
			printf("=========================================================================\n");

			bin_fp = fopen(bin_file, "w");
			if (bin_fp == NULL) {
				printf("%s create failed\n", bin_file);
				return -1;
			}

			ramdump_size = mem_info[index].rd_regionx_size;
			ret = get_dump(dev_fd, bin_fp, ramdump_size);
			fclose(bin_fp);
			if (ret != 0) {
				printf("Unable to receive complete ramdump!!!\n");
				return -1;
			}
		}
	}
	return 0;

}

static int fsdump_recv(int dev_fd)
{
	int ret;
	uint32_t smartfs_addr;
	uint32_t smartfs_size;
	char fs_bin[BINFILE_NAME_SIZE] = { '\0' };
	FILE *fs_fp;

	/* Receive Userfs flash partition start address and size */
	ret = b_read(dev_fd, (uint8_t *)&smartfs_addr, 4);
	if (ret != 4) {
		printf("Receiving Userfs start address failed, ret = %d\n", ret);
		return -1;
	}

	ret = b_read(dev_fd, (uint8_t *)&smartfs_size, 4);
	if (ret != 4) {
		printf("Receiving Userfs size failed, ret = %d\n", ret);
		return -1;
	}

	printf("\n=========================================================================\n");
	printf("Filesystem start address = %08x, filesystem size = %d\n", smartfs_addr, smartfs_size);
	printf("=========================================================================\n");

	/* Create filesystem dump binary file name */
	snprintf(fs_bin, BINFILE_NAME_SIZE, "fsdump_0x%08x_0x%08x.bin", smartfs_addr, (smartfs_addr + smartfs_size));

	printf("\nReceiving file system dump.....\n");

	fs_fp = fopen(fs_bin, "w");
	if (fs_fp == NULL) {
		printf("%s create failed\n", fs_bin);
		return -1;
	}

	ret = get_dump(dev_fd, fs_fp, smartfs_size);
	fclose(fs_fp);
	if (ret != 0) {
		printf("Unable to receive complete FS Dump!!!\n");
		return -1;
	}

	return 0;
}

static int extfsdump_recv(int dev_fd)
{
        int ret;
        uint32_t ext_smartfs_size;
        char ext_fs_bin[BINFILE_NAME_SIZE] = { '\0' };
        FILE *fs_fp;

        /* Receive Userfs flash partition size */
        ret = b_read(dev_fd, (uint8_t *)&ext_smartfs_size, 4);
        if (ret != 4) {
                printf("Receiving Userfs size failed, ret = %d\n", ret);
                return -1;
        }

	if (ext_smartfs_size == 0) {
		printf("No external userfs partition found. Please check config\n");
		return 0;
	}

        printf("\n=========================================================================\n");
        printf("External filesystem size = %u\n", ext_smartfs_size);
        printf("=========================================================================\n");

        /* Create filesystem dump binary file name */
        snprintf(ext_fs_bin, BINFILE_NAME_SIZE, "ext_fsdump_%08u.bin", ext_smartfs_size);

        printf("\nReceiving external file system dump.....\n");

        fs_fp = fopen(ext_fs_bin, "w");
        if (fs_fp == NULL) {
                printf("%s create failed\n", ext_fs_bin);
                return -1;
        }

        ret = get_dump(dev_fd, fs_fp, ext_smartfs_size);
        fclose(fs_fp);
        if (ret != 0) {
                printf("Unable to receive complete FS Dump!!!\n");
                return -1;
        }

        return 0;
}

int dump_file_from_fs(int dev_fd, char *filename, int maxnamelen)
{
	int ret;
	int fd;
	int i, last_subdir;
	char buf;
	FILE *fp;
	u_int64_t file_size;
	char char_filesize[8];
	char local_filename[maxnamelen + 9];

	printf("Attempting to Dump File %s from Target Device\n", filename);

	ret = write(dev_fd, filename, maxnamelen);
	if (ret != maxnamelen) {
		printf("Unable to send Dump File name to Target Device, ret = %d\n", ret);
		return -1;
	}

	file_size = 0;
	for (i = 0; i < 8; i++) {
		ret = read(dev_fd, &char_filesize[i], sizeof(char_filesize[i]));
		if (ret != sizeof(char_filesize[i])) {
			printf("Unable to get file size from Target Device, ret = %d\n", ret);
			return -1;
		}
	}

	for (i = 7; i >= 0; i--) {
		file_size += ((u_int64_t)char_filesize[8 - i - 1] << (i * 8)) & ((u_int64_t)0xFF << (i * 8));
	}

	printf("\nReceiving file %s of size %lu from Target Device\n\n", filename, file_size);

	i = 0;
	last_subdir = -1;
	while (filename[i] != '\0') {
		/* We dont want the file to be saved in the root folder, we want it in the current folder */
		if (filename[i] == '/') {
			last_subdir = i;
		}
		i++;
	}

	filename = filename + last_subdir + 1;
	snprintf(local_filename, maxnamelen + 9, "%s%s", "DumpFile_", filename);

	fp = fopen(local_filename, "w");
	if (fp == NULL) {
		printf("Unable to open file %s to save logs chain dump\n", filename);
		return -1;
	}

	ret = get_dump(dev_fd, fp, file_size);
	fclose(fp);
	if (ret != 0) {
		printf("Unable to receive logs chain dump file from target device, ret = %d\n", ret);
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int ret;
	int dev_fd;
	int dev_lock_fd;
	char *dev_file;
	char tty_path[TTYPATH_LEN] = {0, };
	char tty_type[TTYTYPE_LEN] = {0, };
	char choice[4] = {'\0'};
	char c;
	char handshake_string[HANDSHAKE_STR_LEN_MAX];
	int DUMP_FLAGS;
	int choice_index = 0;

	ret = 1;

	if (argc < 2) {
		printf("Usage: ./trap.sh <device>\n");
		printf("Ex   : ./trap.sh /dev/ttyUSB1   or\n");
		printf("       ./trap.sh /dev/ttyACM0\n");
		return -1;
	}

	dev_file = argv[1];

	/* Get the tty type  */
	if (!strncmp(dev_file, "/dev/ttyUSB", 11)) {
		strncpy(tty_type, &dev_file[5], TTYTYPE_LEN);
	} else if (!strncmp(dev_file, "/dev/ttyACM", 11)) {
		strncpy(tty_type, &dev_file[5], TTYTYPE_LEN);
	} else {
		printf("Undefined tty %s\n", dev_file);
		return -1;
	}

	/* Compose tty path  */
	snprintf(tty_path, TTYPATH_LEN, "/var/lock/LCK..%s", tty_type);
	if (access(tty_path, F_OK) == 0) {
		printf("Error : couldnt lock serial port device %s\n", dev_file);
		printf("Please close any other process using this port first\n");
		goto lock_check_err;
	}

	if ((dev_fd = open(dev_file, O_RDWR | O_NOCTTY | O_SYNC)) == -1) {
		printf("%s open failed!!\nPlease enter correct device port\n", dev_file);
		goto dev_open_err;
	}

	if (configure_tty(dev_fd) != 0) {
		printf("tty configuration failed\n");
		goto tty_config_err;
	}

	if ((dev_lock_fd = open(tty_path, O_CREAT, S_IRWXU)) == -1) {
		printf("tty lock  failed\n");
		goto tty_lock_err;
	}

	printf("Target device locked and ready to DUMP!!!\n");

	while (1) {
		printf("\nChoose from the following options:-\n1. RAM Dump\n2. Userfs Dump\n3. Both RAM and Userfs dumps\n4. External Userfs Partition Dump\n5. Dump specific file from target device\nr. Reboot TARGET Device\nx. Exit TRAP tool\n");
		printf("\nYou may chose mutiple dump options together by entering the option numbers consectuively\nFor Example - \'14\' to dump both RAM and External USERFS partition contents OR \'35\' to print RAM, USERFS and FILE contents\n");
		fflush(stdout);
		scanf("%s", choice);
		getchar();

		DUMP_FLAGS = CLEAR_DUMP_FLAGS;

		while (choice_index < 4 && choice[choice_index] != '\0') {
			switch (choice[choice_index]) {
			case '1':
				DUMP_FLAGS |= RAMDUMP_FLAG;
				break;
			case '2':
				DUMP_FLAGS |= USERFSDUMP_FLAG;
				break;
			case '3':
				DUMP_FLAGS |= RAMDUMP_FLAG;
				DUMP_FLAGS |= USERFSDUMP_FLAG;
				break;
			case '4':
				DUMP_FLAGS |= EXTUSERFS_DUMP_FLAG;
				break;
			case '5':
				DUMP_FLAGS |= FILE_DUMP_FLAG;
				break;
			case 'r':
				DUMP_FLAGS |= REBOOT_DEVICE_FLAG;
				break;
			case 'x':
				break;
			default:
				printf("Invalid Input\n");
				continue;
			}

			choice_index++;
		}

		if (DUMP_FLAGS & REBOOT_DEVICE_FLAG) {
			printf("NOTE: - CONFIG_BOARD_ASSERT_AUTORESET needs to be enabled to reboot TARGET Device after a crash\n");
			printf("Handsake for rebooting device may fail since device reboots before sending acknowledgement\n");

			/* Send handshake signal for rebooting TARGET device */
			strncpy(handshake_string, TARGET_REBOOT_STRING, HANDSHAKE_STR_LEN_MAX);
			if (do_handshake(dev_fd, handshake_string) != 0) {
				printf("Target Handshake Failed\n");
			}
			break;
		}

		/* If no flag is set, then exit the TRAP tool */
		if (DUMP_FLAGS == CLEAR_DUMP_FLAGS) {
			break;
		}

		if (DUMP_FLAGS & RAMDUMP_FLAG) {
			printf("DUMPING RAM CONTENTS\n");
			fflush(stdout);

			/* Handshake for synchronization before RAMDUMP */
			strncpy(handshake_string, RAMDUMP_HANDSHAKE_STRING, HANDSHAKE_STR_LEN_MAX);
			if (do_handshake(dev_fd, handshake_string) != 0) {
				printf("Target Handshake Failed\n");
				goto handshake_err;
			}

			if (ramdump_info_init(dev_fd) != 0) {
				printf("Ramdump initialization failed\n");
				goto init_err;
			}

			if (ramdump_recv(dev_fd) != 0) {
				printf("Ramdump receive failed\n");
				goto dump_err;
			}

			printf("Ramdump received successfully..!\n");
			fflush(stdout);
		}

		if (DUMP_FLAGS & USERFSDUMP_FLAG) {
			printf("\nDUMPING USERFS CONTENTS\n");
			fflush(stdout);

			/* Handshake for synchronization before FS Dump */
			strncpy(handshake_string, FSDUMP_HANDSHAKE_STRING, HANDSHAKE_STR_LEN_MAX);
			if (do_handshake(dev_fd, handshake_string) != 0) {
				printf("Target Handshake Failed\n");
				goto handshake_err;
			}

			ret = fsdump_recv(dev_fd);
			if (ret != 0) {
				printf("Filesystem dump reception failed, ret: %d\n", ret);
				goto dump_err;
			}
			printf("\nFilesystem Dump received successfully\n");
			fflush(stdout);
		}

		if (DUMP_FLAGS & EXTUSERFS_DUMP_FLAG) {
			printf("\nMake sure to enable BCH Driver for External UserFS Partition Dump\n");
			printf("\nDUMPING EXTERNAL USERFS DUMP PARTITION\n");
			fflush(stdout);

			/* Handshake for synchronization before External Userfs Dump */
			strncpy(handshake_string, EXTFSDUMP_HANDSHAKE_STRING, HANDSHAKE_STR_LEN_MAX);
			if (do_handshake(dev_fd, handshake_string) != 0) {
				printf("Target handshake failed\n");
				goto handshake_err;
			}

			ret = extfsdump_recv(dev_fd);
			if (ret != 0) {
				printf("External Userfs partition dump reception failed, ret: %d\n", ret);
				goto dump_err;
			}
			printf("\nExternal Userfs partition dump received successfully\n");
			fflush(stdout);
		}

		if (DUMP_FLAGS & FILE_DUMP_FLAG) {
#ifdef CONFIG_SMARTFS_MAXNAMLEN
			int userfs_maxnamelength = CONFIG_SMARTFS_MAXNAMLEN + 1;
			char filename[CONFIG_SMARTFS_MAXNAMLEN + 1];
#else
			int userfs_maxnamelength;
			char filename[USERFS_MAXNAMLEN + 1];

			printf("No Config found for maximum possible file name length, please enter manually (usually = 32)\n");
			printf("Else, the arbitrarily set default value will be used (enter '0' to continue with the default\n");
			scanf("%d\n", userfs_maxnamelength);

			if (userfs_maxnamlen == 0) {
				userfs_maxnamelength = USERFS_MAXNAMLEN + 1;
			} else {
				userfs_maxnamelength += 1;
			}
#endif
			printf("This utility will allow you to enter a file name that you want to be dumped from the device\n");
			printf("It is the responsibility of the user to ensure that the correct file name/path is provided\n");

			strncpy(handshake_string, USERFS_FDUMP_HANDSHAKE_STRING, HANDSHAKE_STR_LEN_MAX);
			if (do_handshake(dev_fd, handshake_string) != 0) {
				printf("Target handshake failed\n");
				goto handshake_err;
			}

			memset(filename, '\0', userfs_maxnamelength);
			printf("Enter the absolute path of the file that you want to be dumped from the Filesystem: -");
			fflush(stdout);
			scanf("%s", filename);
			getchar();

			ret = dump_file_from_fs(dev_fd, filename, userfs_maxnamelength);
			if (ret != 0) {
				printf("Dump Tool failed to dump file %s from the filesystem.\n", filename);
				goto dump_err;
			}

			printf("\nDumped file %s from filesystem successfully\n", filename);
			fflush(stdout);
		}
	}

	printf("Dump tool exits after successful operation\n");
	ret = 0;

init_err:
dump_err:
handshake_err:
	if (mem_info) {
		free(mem_info);
	}
dl_mode_err:
dl_cmd_err:
	remove(tty_path);
	close(dev_lock_fd);

tty_lock_err:
tty_config_err:
	close(dev_fd);

dev_open_err:
lock_check_err:
bin_type_err:
	return ret;
}
