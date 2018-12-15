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
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "config.h"

#define HANDSHAKE_STRING        "RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX   (7)
#define BINFILE_NAME_SIZE   (40)

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

/* Ramdump initialization data */
int rd_regionx_idx[CONFIG_MM_REGIONS + 2];
int rd_regionx_start[CONFIG_MM_REGIONS + 2];
int rd_regionx_size[CONFIG_MM_REGIONS + 2];

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

static int send_region_info(int dev_fd, char *host_region)
{
	int ret;
	char ack;

	/* Append region info with identification characters */
	host_region[2] = 'r';
	host_region[1] = host_region[0];
	host_region[0] = 'r';

	/* Send region info to TARGET */
	ret = write(dev_fd, host_region, strlen(host_region));
	if (ret != strlen(host_region)) {
		printf("Sending region info failed, ret = %d\n", ret);
		return -1;
	}

	/* Read ACK from TARGET */
	ret = read(dev_fd, &ack, sizeof(ack));
	if (ret != 1) {
		printf("Receiving region info failed, ret = %d\n", ret);
		return -1;
	}

	/* Check ACK */
	if (ack != 'A') {
		printf("Wrong Region info received, ack = %c\n", ack);
		return -1;
	}

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

static int ramdump_parse_info(const char *partition_info_str, int partition_info_array[])
{
	int partition_info = 0;
	int index = 2;

	/* Parse the memory partition information from the Kconfig */

	while (*partition_info_str) {

		/* Extract thresholds from string input in config as integer value */
		partition_info = strtoul(partition_info_str, NULL, 0);

		while (*partition_info_str != ',' && *partition_info_str) {
			partition_info_str++;
		}

		if (*partition_info_str == ',') {
			partition_info_str++;
		}

		/* Update partition_info_array with extracted integer value */
		partition_info_array[index++] = partition_info;
	}

}

void ramdump_info_init(void)
{
	/* Initialize array index with HEAP information */
#if CONFIG_MM_NHEAPS > 1
	ramdump_parse_info(CONFIG_RAM_REGIONx_HEAP_INDEX, rd_regionx_idx);
#endif
	ramdump_parse_info(CONFIG_RAM_REGIONx_START, rd_regionx_start);
	ramdump_parse_info(CONFIG_RAM_REGIONx_SIZE, rd_regionx_size);

#if CONFIG_MM_REGIONS < 2
		rd_regionx_start[1] = rd_regionx_start[2];
		rd_regionx_size[1] = rd_regionx_size[2];
#endif
}

static int ramdump_recv(int dev_fd)
{
	char buf;
	int i;
	int ret;
	int index = 1;
	int count = 0;
	int regions_to_dump = 0;
	uint32_t ramdump_address;
	uint32_t ramdump_size;
	char ramdump_region[3] = { '\0' };
	char bin_file[BINFILE_NAME_SIZE] = { '\0' };
	FILE *bin_fp;
	/* Array to map selected regions to be dumped*/
	int rd_regionx_map[CONFIG_MM_REGIONS + 2] = {0};

	printf("\n=========================================================================\n");
	printf("Ramdump Region Options:\n");
	printf("1. ALL");
#if CONFIG_MM_REGIONS < 2
	printf("\t( Address: %08x, Size: %d )\n",rd_regionx_start[1], rd_regionx_size[1]);
#endif
#if CONFIG_MM_REGIONS > 1
	for (index = 2; index <= CONFIG_MM_REGIONS + 1; index++) {
		printf("\n%d. Region %d:\t( Address: %08x, Size: %d )\t [Heap index = %d]", index, index - 2, rd_regionx_start[index], rd_regionx_size[index], rd_regionx_idx[index]);
	}
#endif
	printf("\n=========================================================================\n");

	printf("Please enter desired ramdump option as below:\n \t1 for ALL\n");
#if CONFIG_MM_REGIONS > 1
	printf(" \t2 for Region 0\n \t25 for Region 0 & 3 ...\n");
#endif
	printf("Please enter your input: ");
	scanf("%d", &index);

	if (index == 1) {
#if CONFIG_MM_REGIONS < 2
		rd_regionx_map[index] = 1;
		regions_to_dump = 1;
#else
		rd_regionx_map[index] = 0;
		for (i = 2; i <= CONFIG_MM_REGIONS + 1; i++) {
			rd_regionx_map[i] = 1;
			regions_to_dump++;
		}
#endif
	}
	else if (index != 1) {
		/* Map index values asked to dump */
		while(index > 0) {
			rd_regionx_map[index%10] = 1;
			index = index/10;
			regions_to_dump++;
		}
	}

	/* Send region index to TARGET */
	snprintf(ramdump_region, 2, "%d", regions_to_dump);
	ret = send_region_info(dev_fd, ramdump_region);
	if (ret != 0) {
		printf("Receiving number of regions to be dumped failed, ret = %d\n", ret);
		return -1;
	}
	printf("\nTarget No. of Regions to be dumped received: %s\n", __func__);
	printf("\nReceiving ramdump......\n\n");

	/* Dump data region wise */
#if CONFIG_MM_REGIONS < 2
	for (index = 1; index <= CONFIG_MM_REGIONS; index++) {
#else
	for (index = 1; index <= CONFIG_MM_REGIONS + 1; index++) {
#endif
		if (rd_regionx_map[index] == 1) {

			/* Send region index to TARGET */
#if CONFIG_MM_REGIONS < 2
			snprintf(ramdump_region, 2, "%d", index - 1);
#else
			snprintf(ramdump_region, 2, "%d", index - 2);
#endif
			ret = send_region_info(dev_fd, ramdump_region);
			if (ret != 0) {
				printf("Receiving region index failed, ret = %d\n", ret);
				return -1;
			}

			/* Receive region address & size from TARGET */
			ret = b_read(dev_fd, (uint8_t *)&ramdump_address, 4);
			if (ret != 4) {
				printf("Receiving address failed, ret = %d\n", ret);
				return -1;
			}

			ret = b_read(dev_fd, (uint8_t *)&ramdump_size, 4);
			if (ret != 4) {
				printf("Receiving size failed, ret = %d\n", ret);
				return -1;
			}

			snprintf(bin_file, BINFILE_NAME_SIZE, "ramdump_0x%08x_0x%08x.bin", ramdump_address, (ramdump_address + ramdump_size));
			printf("=========================================================================\n");
#if CONFIG_MM_REGIONS < 2
			printf("Dumping data, Address: %08x, Size: %dbytes\n", ramdump_address, ramdump_size);
#else
			printf("Dumping Region: %d, Address: %x, Size: %dbytes\n", index - 2, ramdump_address, ramdump_size);
#endif
			printf("=========================================================================\n");

			bin_fp = fopen(bin_file, "w");
			if (bin_fp == NULL) {
				printf("%s create failed\n", bin_file);
				return -1;
			}

			printf("[>");
			fflush(stdout);

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

				if ((count % (16 * 1024)) == 0) {
				//	printf("%s: received %d bytes\n", __func__, count);
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

	ret = 1;

	if (argc < 2) {
		printf("Usage: ./ramdump <device>\n");
		printf("Ex   : ./ramdump /dev/ttyUSB1\n");
		return -1;
	}

	dev_file = argv[1];

	if (access("/var/lock/LCK..ttyUSB1", F_OK) == 0) {
		printf("Error : couldnt lock serial port device %s\n", "/dev/ttyUSB1");
		printf("Please close any other process using this port first\n");
		goto lock_check_err;
	}

	if ((dev_fd = open(dev_file, O_RDWR | O_NOCTTY | O_SYNC)) == -1) {
		printf("%s open failed\n", dev_file);
		goto dev_open_err;
	}

	if (configure_tty(dev_fd) != 0) {
		printf("tty configuration failed\n");
		goto tty_config_err;
	}

	if ((dev_lock_fd = open("/var/lock/LCK..ttyUSB1", O_CREAT, S_IRWXU)) == -1) {
		printf("tty lock  failed\n");
		goto tty_lock_err;
	}

	if (do_handshake(dev_fd) != 0) {
		printf("Target Handshake Failed\n");
		goto handshake_err;
	}

	printf("Target entered to ramdump mode\n");

	ramdump_info_init();

	if (ramdump_recv(dev_fd) != 0) {
		printf("Ramdump receive failed\n");
		goto ramdump_err;
	}

	printf("Ramdump received successfully..!\n");
	ret = 0;

ramdump_err:
handshake_err:
dl_mode_err:
dl_cmd_err:
	remove("/var/lock/LCK..ttyUSB1");
	close(dev_lock_fd);

tty_lock_err:
tty_config_err:
	close(dev_fd);

dev_open_err:
lock_check_err:
bin_type_err:
	return ret;
}
