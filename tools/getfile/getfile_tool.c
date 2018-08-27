/*******************************************************************
*
* Copyright 2018 Samsung Electronics All Rights Reserved.
*
* File Name : getfile_tool.c
* Description: Get file from board using UART
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

#define HANDSHAKE_STRING        "GETFILE"
#define HANDSHAKE_STR_LEN_MAX   (7)
#define BINFILE_NAME_SIZE   (128)

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

	printf("\nTarget Handshake successful\n\n");

	return 0;
}

static int send_file_path(int dev_fd, char *path)
{
	int ret;
	char ack;
	uint8_t size;

	size = strlen(path);
	ret = write(dev_fd, &size, 1);
	if (ret != 1) {
		printf("Sending file path size failed, ret = %d\n", ret);
		return -1;
	}

	/* Send file path to TARGET */
	ret = write(dev_fd, path, strlen(path));
	if (ret != strlen(path)) {
		printf("Sending file path failed, ret = %d\n", ret);
		return -1;
	}
	printf("\nTarget recieve file path successful\n");

	return 0;
}

static int b_read(int fd, uint8_t * buf, int size)
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

static int start_recv(int dev_fd, char *filename)
{
	char buf;
	int ret;
	int count = 0;
	uint32_t file_address;
	uint32_t file_size;
	FILE *bin_fp;
	char bin_file[BINFILE_NAME_SIZE] = { '\0' };

	ret = b_read(dev_fd, (uint8_t *) & file_address, 4);
	if (ret != 4) {
		printf("Receiving address failed, ret = %d\n", ret);
		return -1;
	}

	ret = b_read(dev_fd, (uint8_t *) & file_size, 4);
	if (ret != 4) {
		printf("Receiving size failed, ret = %d\n", ret);
		return -1;
	}

	printf("%s: file buf address = 0x%08x, file size = %d\n", __func__,
	       file_address, file_size);

	strcpy(bin_file, filename);

	bin_fp = fopen(bin_file, "w");
	if (bin_fp == NULL) {
		printf("%s create failed\n", bin_file);
		return -1;
	}

	printf("[>");
	fflush(stdout);

	while (file_size) {
		ret = read(dev_fd, &buf, 1);
		if (ret != 1) {
			printf("Receiving file %dTH byte failed, ret = %d\n",
			       count, ret);
			fclose(bin_fp);
			return -1;
		}

		ret = fwrite(&buf, 1, 1, bin_fp);
		if (ret != 1) {
			printf("Writing file %dTH byte failed, ret = %d\n",
			       count, ret);
			fclose(bin_fp);
			return -1;
		}

		count++;
		file_size--;

		if ((count % (16 * 1024)) == 0) {
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

	char *file_path[10];
	char fname[30];
	char *store_path;
	char st_tmp_path[50];
	int filenum = 1;
	int findex = 1;
	char *p;
	char end = 'S';

	ret = 1;
	if (argc < 3) {
		printf
		    ("Usage: ./getfile <device>  <filenum> <remote file path>  <store path> \n");
		printf
		    ("Ex   : ./getfile /dev/ttyUSB1  5 /tmp/file ....  /home/ \n");
		return -1;
	}

	dev_file = argv[1];
	filenum = argc;
	if (filenum == 4) {
		file_path[1] = argv[2];
		store_path = argv[3];
	} else {
		for (findex = 2; findex <= filenum - 2; findex++)
			file_path[findex - 1] = argv[findex];
		store_path = argv[findex];
		strcpy(st_tmp_path, store_path);
	}

	if (access("/var/lock/LCK..ttyUSB1", F_OK) == 0) {
		printf("Error : couldnt lock serial port device %s\n",
		       "/dev/ttyUSB1");
		printf
		    ("Please close any other process using this port first\n");
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

	if ((dev_lock_fd =
	     open("/var/lock/LCK..ttyUSB1", O_CREAT, S_IRWXU)) == -1) {
		printf("tty lock  failed\n");
		goto tty_lock_err;
	}
	if (do_handshake(dev_fd) != 0) {
		printf("Target Handshake Failed\n");
		goto handshake_err;
	}

	if (filenum == 4) {
		if (send_file_path(dev_fd, file_path[1]) != 0) {
			printf("Send file  Failed\n");
			goto handshake_err;
		}
		printf("\nStarting pull file via Uart from :%s......\n",
		       file_path[1]);

		strcpy(fname,
		       ((p =
			 strrchr(file_path[1], '/')) ? (p + 1) : file_path[1]));
		strcat(store_path, fname);
		if (start_recv(dev_fd, store_path) != 0) {
			printf("start recv file failed\n");
			goto getfile_err;
		}
		ret = write(dev_fd, &end, 1);
		if (ret != 1) {
			printf("Sending end failed, ret = %d\n", ret);
			goto getfile_err;
		}
	} else {
		for (findex = 1; findex <= filenum - 3; findex++) {
			if (send_file_path(dev_fd, file_path[findex]) != 0) {
				printf("Send file  Failed\n");
				goto handshake_err;
			}
			printf
			    ("Starting pull %d file via Uart from :%s ......\n",
			     findex, file_path[findex]);
			strcpy(store_path, st_tmp_path);
			strcpy(fname,
			       ((p =
				 strrchr(file_path[findex],
					 '/')) ? (p + 1) : file_path[findex]));
			strcat(store_path, fname);
			if (start_recv(dev_fd, store_path) != 0) {
				printf("start recv file failed\n");
				goto getfile_err;
			}
		}
		ret = write(dev_fd, &end, 1);
		if (ret != 1) {
			printf("Sending end failed, ret = %d\n", ret);
			goto getfile_err;
		}
	}

	printf("Successfully! Now can continue reconnect UART at terminal\n");
	ret = 0;

 getfile_err:
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
