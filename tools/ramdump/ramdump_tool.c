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

#define HANDSHAKE_STRING        "RAMDUMP"
#define HANDSHAKE_STR_LEN_MAX   (7)
#define BINFILE_NAME_SIZE   (40)

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

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

static int ramdump_recv(int dev_fd)
{
	char buf;
	int ret;
	int count = 0;
	uint32_t ramdump_address;
	uint32_t ramdump_size;
	FILE *bin_fp;
	char bin_file[BINFILE_NAME_SIZE] = { '\0' };

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

	printf("%s: ramdump_address = %08x, ramdump_size = %d\n", __func__, ramdump_address, ramdump_size);

	snprintf(bin_file, BINFILE_NAME_SIZE, "ramdump_0x%08x_0x%08x.bin", ramdump_address, (ramdump_address + ramdump_size));

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
			// printf("%s: received %d bytes\n", __func__, count);
			printf("\b=>");
			fflush(stdout);
		}
	}
	printf("]\n");
	fclose(bin_fp);

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
	printf("Receiving ramdump......\n");

	if (ramdump_recv(dev_fd) != 0) {
		printf("Ramdump receive failed\n");
		goto ramdump_err;
	}

	printf("Ramdump received successfully\n");
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
