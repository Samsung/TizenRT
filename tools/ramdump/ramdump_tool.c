/*******************************************************************
*
* Copyright 2016 Samsung Electronics All Rights Reserved.
*
* File Name : ramdump_tool.c
* Description: Receive Ramdump using UART
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define HANDSHAKE_STRING	"RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX	(7)
#define BINFILE_NAME_SIZE	(40)
#define KB_CHECK_COUNT		(16 * 1024)

#define TTYPATH_LEN		25
#define TTYTYPE_LEN		7

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

/* Ramdump initialization data */
uint32_t  number_of_regions;
typedef struct {
	int rd_regionx_idx;
	uint32_t rd_regionx_start;
	uint32_t rd_regionx_size;
	int rd_regionx_mark;
} rd_regionx;
rd_regionx *mem_info;

static int do_handshake(int dev_fd)
{
	int ret;
	char ack;
	char host_handshake[] = HANDSHAKE_STRING;

	/* Send handshake command to TARGET */
	ret = write(dev_fd, host_handshake, strlen(host_handshake));
	if (ret != strlen(host_handshake)) {
		printf("Sending handshake failed, ret = %d\n", ret);
		return -1;
	}

	/* Read ACK from TARGET */
	ret = read(dev_fd, &ack, sizeof(ack));
	if (ret != 1) {
		printf("Receiving handshake failed, ret = %d\n", ret);
		return -1;
	}

	/* Check ACK */
	if (ack != 'A') {
		printf("Wrong Target Handshake, ack = %c\n", ack);
		return -1;
	}

	printf("Target Handshake successful %s\n", __func__);

	return 0;
}

static int b_read(int fd, uint8_t *buf, int size)
{
	int i;
	int ret;

	for (i = 0; i < size; i++) {
		ret = read(fd, buf + i, 1);
		if (ret != 1) {
			return i;
		}
	}

	return i;
}

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
			printf("Receiving address failed, ret = %d\n", ret);
			return -1;
		}
		mem_info[i].rd_regionx_start = mem_address;

		ret = b_read(dev_fd, (uint8_t *)&mem_size, 4);
		if (ret != 4) {
			printf("Receiving size failed, ret = %d\n", ret);
			return -1;
		}
		mem_info[i].rd_regionx_size = mem_size;

		ret = read(dev_fd, &c, 1);
		if (ret != 1) {
			printf("Receiving size failed, ret = %d\n", ret);
			return -1;
		}
		mem_info[i].rd_regionx_idx = c;
	}

	return 0;
}

static int send_region_info(int dev_fd, char *host_region)
{
	int ret;

	/* Send region info to TARGET */
	ret = write(dev_fd, host_region, 1);
	if (ret != 1) {
		printf("Sending region info failed, ret = %d\n", ret);
		return -1;
	}

	return 0;
}

static int ramdump_recv(int dev_fd)
{
	char buf;
	int i;
	int ret;
	int index;
	int count = 0;
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
			printf("\n%d. Region %d:\t( Address: 0x%08x, Size: %d )\t [Heap index = %d]", index, index - 2, mem_info[index].rd_regionx_start, mem_info[index].rd_regionx_size, mem_info[index].rd_regionx_idx);
		}
	}
	printf("\n=========================================================================\n");
	printf("Please enter desired ramdump option as below:\n \t1 for ALL\n");
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
	printf("\nNo. of Regions to be dumped received: %s\n", __func__);
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

			snprintf(bin_file, BINFILE_NAME_SIZE, "ramdump_0x%08x_0x%08x.bin",  mem_info[index].rd_regionx_start, (mem_info[index].rd_regionx_start +  mem_info[index].rd_regionx_size));
			printf("=========================================================================\n");
			if (number_of_regions == 1) {
				printf("Dumping data, Address: 0x%08x, Size: %dbytes\n",  mem_info[index].rd_regionx_start,  mem_info[index].rd_regionx_size);
			} else {
				printf("Dumping Region: %d, Address: 0x%08x, Size: %dbytes\n", index - 2,  mem_info[index].rd_regionx_start,  mem_info[index].rd_regionx_size);
			}
			printf("=========================================================================\n");

			bin_fp = fopen(bin_file, "w");
			if (bin_fp == NULL) {
				printf("%s create failed\n", bin_file);
				return -1;
			}

			printf("[>");
			fflush(stdout);

			/* Dump data of Memory REGIONx  */
			ramdump_size =  mem_info[index].rd_regionx_size;
			while (ramdump_size) {
				ret = read(dev_fd, &buf, 1);
				if (ret != 1) {
					printf("Receiving ramdump %dTH byte failed, ret = %d\n", count, ret);
					fclose(bin_fp);
					return -1;
				}

				ret = fwrite(&buf, 1, 1, bin_fp);
				if (ret != 1) {
					printf("Writing ramdump %dTH byte failed, ret = %d\n", count, ret);
					fclose(bin_fp);
					return -1;
				}

				count++;
				ramdump_size--;

				if ((count % (KB_CHECK_COUNT)) == 0) {
					printf("\b=>");
					fflush(stdout);
				}
			}
			printf("]\n");
			fclose(bin_fp);
		}
	}
	return 0;

}

static int configure_tty(int tty_fd)
{
	struct termios ttyio;
	memset(&ttyio, 0, sizeof(ttyio));

	fcntl(tty_fd, F_SETFL, 0);
	tcgetattr(tty_fd, &ttyio);

	/* set baudrate */
	cfsetispeed(&ttyio, B115200);
	cfsetospeed(&ttyio, B115200);
	ttyio.c_cflag |= (CLOCAL | CREAD);

	cfmakeraw(&ttyio);

	ttyio.c_cflag &= ~CSTOPB;
	ttyio.c_cflag &= ~(PARENB | PARODD);
	ttyio.c_cflag &= ~CSIZE;
	ttyio.c_cflag &= ~CRTSCTS;
	ttyio.c_cflag |= CS8;

	ttyio.c_iflag &= ~ICRNL;
	ttyio.c_iflag &= ~INLCR;
	ttyio.c_oflag &= ~OCRNL;
	ttyio.c_oflag &= ~ONLCR;
	ttyio.c_lflag &= ~ICANON;
	ttyio.c_lflag &= ~ECHO;

	ttyio.c_cc[VMIN] = 1;
	ttyio.c_cc[VTIME] = 5;

	tcflush(tty_fd, TCIOFLUSH);

	tcsetattr(tty_fd, TCSANOW, &ttyio);
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

	ret = 1;

	if (argc < 2) {
		printf("Usage: ./ramdump <device>\n");
		printf("Ex   : ./ramdump /dev/ttyUSB1   or\n");
		printf("       ./ramdump /dev/ttyACM0\n");
		return -1;
	}

	dev_file = argv[1];

	/* Get the tty type  */
	if (!strcmp(dev_file, "/dev/ttyUSB1")) {
		strncpy(tty_type, "ttyUSB1", TTYTYPE_LEN);
	} else if (!strcmp(dev_file, "/dev/ttyACM0")) {
		strncpy(tty_type, "ttyACM0", TTYTYPE_LEN);
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

	if (do_handshake(dev_fd) != 0) {
		printf("Target Handshake Failed\n");
		goto handshake_err;
	}

	printf("Target entered to ramdump mode\n");

	if (ramdump_info_init(dev_fd) != 0) {
		printf("Ramdump initialization failed\n");
		goto init_err;
	}

	if (ramdump_recv(dev_fd) != 0) {
		printf("Ramdump receive failed\n");
		goto ramdump_err;
	}

	printf("Ramdump received successfully..!\n");
	ret = 0;

init_err:
	free(mem_info);
ramdump_err:
handshake_err:
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
