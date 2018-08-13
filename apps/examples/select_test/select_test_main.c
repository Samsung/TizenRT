/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <apps/shell/tash.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <debug.h>
#include <arpa/inet.h>

#ifndef CONFIG_NET_LWIP
#include <arpa/inet.h>
#endif

#define ENABLE_MULTIPLE_SOCKET_TEST

#if defined(ENABLE_MULTIPLE_SOCKET_TEST)
#define SOCKET_TEST_PORT        65353
#define SOCKET2_TEST_PORT       65354
#endif

#define ENABLE_STDIN_TEST 1
#define ENABLE_PIPE_TEST  1
#define ENABLE_SELECT_SLEEP_TEST 1
#define MAX(a, b) (a > b ? a : b)

#ifdef ENABLE_MULTIPLE_SOCKET_TEST
static int g_socket_fd;
static int g_socket2_fd;
#endif
static int g_file_fd;
static int g_fifo_wrfd;
static int g_fifo_rdfd;

#ifdef ENABLE_PIPE_TEST

static int g_pipe_handle[2];

static int create_pipe(int handles[2])
{
	return pipe(handles);
}

static int read_pipe(int s, unsigned char *buf, int len)
{
	return read(s, buf, len);
}

static int write_pipe(int s, char *buf, int len)
{
	return write(s, buf, len);
}

static int close_pipe(int *handles)
{
	if (handles[0] > 0) {
		close(handles[0]);
	}

	if (handles[1] > 0) {
		close(handles[2]);
	}

	return 0;
}

#endif

static int create_socket(int port)
{
	int fd;
	struct sockaddr_in serv_addr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd  == -1) {
		return -1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		close(fd);
		return -1;
	}

	return fd;
}

static int read_socket(int fd, unsigned char *buf, int len)
{
	int ret;
	struct sockaddr_in fromaddr;

	socklen_t sockaddr_size = sizeof(struct sockaddr_in);
	ret = recvfrom(fd, buf, len, 0, (struct sockaddr *)&fromaddr, &sockaddr_size);
	if (ret < 0) {
		return -1;
	}

	return ret;
}

static int write_socket(int fd, char *buf, int len, int port)
{
	struct sockaddr_in toaddr;

	memset(&toaddr, 0, sizeof(struct sockaddr_in));
	toaddr.sin_family = AF_INET;
	toaddr.sin_port = htons(port);
	toaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	return sendto(fd, buf, len, 0, (struct sockaddr *)&toaddr, sizeof(struct sockaddr_in));
}


static void main_task(void)
{
	fd_set sockfd_set;
	int max_fd  = 0;
	unsigned char buffer[4];

	struct timeval tv;
	int retval = 0;
#ifdef ENABLE_STDIN_TEST
	int stdin_test_success = 0;
#endif
	int filefd_test_success = 0;
	int fifo_test_success = 0;
#ifdef ENABLE_MULTIPLE_SOCKET_TEST
	int sockfd_test_success = 0;
	int sockfd2_test_success = 0;
#endif
#ifdef ENABLE_PIPE_TEST
	int pipe_test_success = 0;
#endif
	int exit_select = 0;

	/* Wait up to 1 seconds. */

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (exit_select == 0) {
		FD_ZERO(&sockfd_set);

#ifdef ENABLE_STDIN_TEST
		/* Watch stdin to see when it has input. */
		FD_SET(0, &sockfd_set);
#endif

#ifdef ENABLE_MULTIPLE_SOCKET_TEST
		/* Watch g_socket_fd to see when it has input. */
		FD_SET(g_socket_fd, &sockfd_set);
		/* Watch g_socket2_fd to see when it has input. */
		FD_SET(g_socket2_fd, &sockfd_set);
#endif
		/* Watch g_file_fd to see when it has input. */
		FD_SET(g_file_fd, &sockfd_set);
		/* Watch g_fifo_rdfd to see when it has input. */
		FD_SET(g_fifo_rdfd, &sockfd_set);

#ifdef ENABLE_MULTIPLE_SOCKET_TEST
		max_fd = MAX(max_fd, g_socket2_fd);
		max_fd = MAX(max_fd, g_socket_fd);
#endif
		max_fd = MAX(max_fd, g_file_fd);
		max_fd = MAX(max_fd, g_fifo_rdfd);

#ifdef ENABLE_PIPE_TEST
		/* Watch g_pipe_handle[0] to see when it has input. */
		FD_SET(g_pipe_handle[0], &sockfd_set);
		max_fd = MAX(max_fd , g_pipe_handle[0]);
#endif

		//printf("\n max_fd = %d \n",max_fd);

		retval = select(max_fd + 1, &sockfd_set, NULL, NULL, &tv);

		if (retval != -1) {
#ifdef ENABLE_STDIN_TEST
			if (FD_ISSET(0, &sockfd_set)) {
				printf("\nReceived data via stdin fd ------------- \n");
				read(0, buffer, 1);
				printf("\n data received on stdin: %c \n", buffer[0]);
				stdin_test_success = 1;
				printf("\n STDIN select test SUCCESS \n");
			}
#endif
			if (FD_ISSET(g_file_fd, &sockfd_set)) {
				printf("\nReceived data via file fd !!!!!!!!!!!!!! \n");
				read(g_file_fd, buffer, 1);
				printf("\n data received on file fd: %c \n", buffer[0]);
				filefd_test_success = 1;
				printf("\n File FD select test SUCCESS \n");
			}
			if (FD_ISSET(g_fifo_rdfd, &sockfd_set)) {
				printf("\nReceived data via fifo read fd @@@@@@@@@@@@ \n");
				read(g_fifo_rdfd, buffer, 1);
				printf("\n data received on fifo read fd: %c \n", buffer[0]);
				fifo_test_success = 1;
				printf("\n FIFO select test SUCCESS \n");
			}
#ifdef ENABLE_MULTIPLE_SOCKET_TEST
			if (FD_ISSET(g_socket_fd, &sockfd_set)) {
				printf("\nReceived data via socket ############# \n");
				read_socket(g_socket_fd, buffer, 1);
				printf("\n data received on sock fd: %c \n", buffer[0]);
				sockfd_test_success = 1;
				printf("\n Sock FD select test SUCCESS \n");
			}
			if (FD_ISSET(g_socket2_fd, &sockfd_set)) {
				printf("\nReceived data via socket2 ^^^^^^^^^^^^ \n");
				read_socket(g_socket2_fd, buffer, 1);
				printf("\n data received on sock2 fd: %c \n", buffer[0]);
				sockfd2_test_success = 1;
				printf("\n Sock FD2 select test SUCCESS \n");
			}
#endif
#ifdef ENABLE_PIPE_TEST
			if (FD_ISSET(g_pipe_handle[0], &sockfd_set)) {
				printf("\nReceived data via pipe read handle ************* \n");
				read_pipe(g_pipe_handle[0], buffer, 1);
				printf("data received on pipe read hadnle: %c \n", buffer[0]);
				pipe_test_success = 1;
				printf("\n PIPE select test SUCCESS \n");
			}
#endif
		}
		exit_select =   filefd_test_success
						&& fifo_test_success
#ifdef ENABLE_STDIN_TEST
						&& stdin_test_success
#endif
#ifdef ENABLE_MULTIPLE_SOCKET_TEST
						&& sockfd_test_success
						&& sockfd2_test_success
#endif
#ifdef ENABLE_PIPE_TEST
						&& pipe_test_success
#endif
						;

	}
#ifdef ENABLE_MULTIPLE_SOCKET_TEST
	close(g_socket_fd);
	close(g_socket2_fd);
#endif
	close(g_file_fd);
	close(g_fifo_rdfd);
	close(g_fifo_wrfd);
#ifdef ENABLE_PIPE_TEST
	close_pipe(g_pipe_handle);
#endif
	printf("Select Test End \n");
	return;
}

static int start_task(void)
{
	int result = -1;

	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	if (pthread_create(&tid, &attr, (void *(*)(void *))main_task, NULL) != 0) {
		goto done;
	}

	/* success */
	result = 0;

done:
	return result;

}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int select_test_main(int argc, char *argv[])
#endif
{
	int result = -1;

	if (argc < 6) {
		printf("\n[USAGE] select_test a b c d e\n");
		return 0;
	}


#ifdef ENABLE_MULTIPLE_SOCKET_TEST
	g_socket_fd = create_socket(SOCKET_TEST_PORT);
	g_socket2_fd = create_socket(SOCKET2_TEST_PORT);
#endif
	mkfifo("/dev/fifo0", 0666);
	g_file_fd = open("/dev/fifo0", O_RDWR);
	mkfifo("/dev/fifo1", 0666);
	g_fifo_wrfd = open("/dev/fifo1", O_WRONLY);
	g_fifo_rdfd = open("/dev/fifo1", O_RDONLY);
#ifdef ENABLE_PIPE_TEST
	create_pipe(g_pipe_handle);
	printf("Test Start : pipe_handle[0]=%d, pipe_handle[1]=%d\n", g_pipe_handle[0], g_pipe_handle[1]);
#endif
#ifdef ENABLE_MULTIPLE_SOCKET_TEST
	printf("Test Start :socket_fd=%d socket2_fd=%d  file_fd = %d fifo_wrfd = %d fifo_rdfd = %d \n", g_socket_fd, g_socket2_fd, g_file_fd, g_fifo_wrfd, g_fifo_rdfd);
#else
	printf("Test Start :stdin = %d file_fd = %d fifo_wrfd = %d fifo_rdfd = %d \n", 0, g_file_fd, g_fifo_wrfd, g_fifo_rdfd);
#endif

#ifdef ENABLE_SELECT_SLEEP_TEST
	struct timeval tv;
	/* Wait up to 3 seconds. */

	tv.tv_sec = 3;
	tv.tv_usec = 0;
	/* test select sleep functionality */
	select(0, NULL, NULL, NULL, &tv);
#else
	usleep(50000);
#endif

	start_task();

	{
		int param_idx = 1;

		printf("\nWriting data(%s) to g_fifo_wrfd = %d\n", argv[param_idx], g_fifo_wrfd);
		write(g_fifo_wrfd, argv[param_idx], 1);
		param_idx++;
		usleep(2000000);
#ifdef ENABLE_PIPE_TEST
		printf("\nWriting data(%s) to g_pipe_handle = %d\n", argv[param_idx], g_pipe_handle[1]);
		write_pipe(g_pipe_handle[1], argv[param_idx], 1);
		param_idx++;
		usleep(2000000);
#endif
		printf("\nWriting data(%s) to g_file_fd = %d\n", argv[param_idx], g_file_fd);
		write(g_file_fd, argv[param_idx], 1);
		param_idx++;
		usleep(2000000);
#ifdef ENABLE_MULTIPLE_SOCKET_TEST
		printf("\nWriting data(%s) to g_socket_fd = %d\n", argv[param_idx], g_socket_fd);
		write_socket(g_socket_fd, argv[param_idx], 1, SOCKET_TEST_PORT);
		param_idx++;
		usleep(2000000);
		printf("\nWriting data(%s) to g_socket2_fd = %d\n", argv[param_idx], g_socket2_fd);
		write_socket(g_socket2_fd, argv[param_idx], 1, SOCKET2_TEST_PORT);
#endif
	}

	/* success */
	result = 0;
	return result;
}

