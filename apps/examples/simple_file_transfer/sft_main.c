/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <pthread.h>

#define SFT_PORT 5555
#define BUF_SIZE 512
static char g_buf[BUF_SIZE] = {0,};
static char g_name_buf[128];

static int send_data(int sd, char *buf, int buf_size)
{
	printf("Sending Data %d\n", buf_size);
	int sent = 0;
	while (sent < buf_size) {
		int ret = send(sd, buf + sent, buf_size - sent, 0);
		if (ret <= 0) {
			if (ret == 0) {
				printf("connection closed\n");
				return -1;
			}
			if (errno == EWOULDBLOCK) {
				continue;
			}
		}
		sent += ret;
	}
	return 0;
}

static int read_data(int sd, char *buf, int buf_size, int *readed)
{
	printf("Receiving Data %d\n", buf_size);
	int read_size = 0;
	while (read_size < buf_size) {
		int nbytes = recv(sd, buf + read_size, buf_size - read_size, 0);
		if (nbytes == 0) {
			printf("connection closed\n");
			return -1;
		} else if (nbytes < 0) {
			printf("unknown error(%d)\n", errno);
			return -1;
		}
		read_size += nbytes;
		*readed = read_size;
	}
	return 0;
}

static int write_data(FILE *fp, char *buf, int buf_size)
{
	int written = 0;
	while (written < buf_size) {
		int res = fwrite(buf + written, sizeof(char), buf_size - written, fp);
		if (res < 0) {
			printf("file write fail %d\n", errno);
			return -1;
		}
		written += res;
	}
	return 0;
}

static int get_file_size(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);

	printf("file_size (%d)\n", file_size);
	fseek(fp, 0, SEEK_SET);

	return file_size;
}

static void send_file(int connfd)
{
	int readed = 0;
	int file_name_size = 0;
	int ret = read_data(connfd, (char *)&file_name_size, sizeof(int), &readed);
	if (ret < 0) {
		printf("readed %d %d\n", readed, __LINE__);
		return;
	}
	file_name_size = ntohl(file_name_size);
	printf("file name length(%d) %d\n", file_name_size, readed);

	ret = read_data(connfd, g_name_buf, file_name_size, &readed);
	if (ret < 0) {
		printf("fail %d\n", __LINE__);
		return;
	}
	g_name_buf[file_name_size] = 0;
	printf("file name: %s\n", g_name_buf);

	FILE *fp = fopen(g_name_buf, "r");
	if (!fp) {
		printf("fail %d\n", __LINE__);
		return;
	}

	//get file size
	int file_size = get_file_size(fp);
	printf("send file size %d\n", file_size);
	int net_file_size = htonl(file_size);
	ret = send_data(connfd, (void *)&net_file_size, sizeof(int));
	if (ret < 0) {
		printf("error %d\n", __LINE__);
		fclose(fp);
		return;
	}
	printf("file size: %d\n", file_size);

	readed = 0;
	while (readed < file_size) {
		ret = fread(g_buf, sizeof(char), file_size - readed, fp);
		if (ret < 0) {
			fclose(fp);
			return;
		}
		send_data(connfd, g_buf, ret);
		readed += ret;
	}

	printf("[TCPCLIENT] <--send\t%d bytes\n\n", ret);
	fclose(fp);

	return;
}

static void receive_file(int connfd)
{
	int readed = 0;
	int file_name_size = 0;
	int ret = read_data(connfd, (char *)&file_name_size, sizeof(int), &readed);
	if (ret < 0) {
		printf("readed %d %d\n", readed, __LINE__);
		return;
	}
	file_name_size = ntohl(file_name_size);
	printf("file name length(%d) %d\n", file_name_size, readed);

	ret = read_data(connfd, g_name_buf, file_name_size, &readed);
	if (ret < 0) {
		printf("fail %d\n", __LINE__);
		return;
	}
	g_name_buf[file_name_size] = 0;
	printf("file name: %s\n", g_name_buf);

	int file_size = 0;
	ret = read_data(connfd, (char *)&file_size, sizeof(int), &readed);
	if (ret < 0) {
		printf("error %d\n", __LINE__);
		return;
	}
	file_size = ntohl(file_size);
	printf("file size = %d\n", file_size);

	FILE *fp = fopen(g_name_buf, "w");
	if (!fp) {
		printf("fail %d\n", __LINE__);
		return;
	}

	int remain = file_size;
	while (remain > 0) {
		readed = 0;
		int buf_size = remain < BUF_SIZE ? remain : BUF_SIZE;
		ret = read_data(connfd, g_buf, buf_size, &readed);
		if (ret < 0) {
			printf("error %d\n", __LINE__);
			break;
		}
		ret = write_data(fp, g_buf, readed);
		if (ret < 0) {
			printf("error %d\n", __LINE__);
			break;
		}
		remain -= buf_size;
	}
	if (remain == 0) {
		printf("read complete\n");
	}

	fclose(fp);
}

static int tcp_server(void)
{
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	int listenfd = -1;
	int connfd = -1;
	socklen_t clilen;
	int ret = 0;
	int readed = 0;

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("[TCPSERV] TCP socket failure %d\n", errno);
		return -1;
	}

	/*
	 * Enable SO_REUSEADDR to allow multiple instances of this
	 * application to receive copies of the multicast datagrams.
	 */

	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("[TCPSERV] ERR: setting SO_REUSEADDR\n");
		goto out_with_socket;
	}
	printf("[TCPSERV] set reusable success\n");

	/* Connect the socket to the server */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SFT_PORT);

	ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0) {
		perror("[TCPSERV] bind fail\n");
		goto out_with_socket;
	}

	printf("[TCPSERV] Listening... port %d\n", SFT_PORT);

	ret = listen(listenfd, 1024);
	if (ret < 0) {
		perror("[TCPSERV] listen fail\n");
		goto out_with_socket;
	}

	clilen = sizeof(cliaddr);

	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
	if (connfd < 0) {
		perror("[TCPSERV] accept fail\n");
		goto out_with_socket;
	}
	printf("[TCPSERV] Accepted\n");

	int direction = 0;
	ret = read_data(connfd, (char *)&direction, sizeof(int), &readed);
	if (ret < 0) {
		printf("read %d %d\n", readed, __LINE__);
		close(connfd);
		goto out_with_socket;
	}
	direction = ntohl(direction);
	printf("direction %d\n", direction);

	if (direction == 0) {
		printf("send file\n");
		send_file(connfd);
	} else if (direction == 1) {
		printf("receive file\n");
		receive_file(connfd);
	} else {
		printf("invalid direction\n");
	}

	close(connfd);

out_with_socket:
	close(listenfd);
	return 0 ;
}

/****************************************************************************
 * sft_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sft_main(int argc, char *argv[])
#endif
{
	while (1) {
		tcp_server();
	}
	return 0;
}
