/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <libtuvapi.h>
#include <libtuv/uv.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PORT 5683

/****************************************************************************
 * Private Data
 ****************************************************************************/

int socket_send;
int socket_receive;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void on_interval_send_callback_2s(void *user_data)
{
	printf("%s\n", __func__);

	int send_len = 0;
	struct sockaddr_in address;

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	socket_send = socket(AF_INET, SOCK_DGRAM, 0);
	send_len = sendto(socket_send, "hello", sizeof("hello"), 0, (struct sockaddr *)&address, sizeof(address));

	printf("test_time upsend_len: %d\n", send_len);
}

static void on_interval_send_callback_5s(void *user_data)
{
	printf("%s\n", __func__);

	libtuv_loop_quit();
}

static int os_watch_io_receive_callback(int fd, enum watch_io io, void *user_data)
{
	printf("%s, in para fd: %d, global fd: %d\n", __func__, fd, socket_receive);

	int sin_len;
	char message[256];
	struct sockaddr_in sin;
	int recv_len;

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);

	sin_len = sizeof(sin);
	recv_len = recvfrom(fd, message, sizeof(message), 0, (struct sockaddr *)&sin, (socklen_t *)&sin_len);

	printf("Response from server:%s, len: %d\n", message, recv_len);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int libtuv_main(int argc, FAR char *argv[])
#endif
{
	int fd_watch = -1;
	struct sockaddr_in sin;

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);

	socket_receive = socket(AF_INET, SOCK_DGRAM, 0);
	bind(socket_receive, (struct sockaddr *)&sin, sizeof(sin));
	printf("socket_receive = %d\n", socket_receive);

	libtuv_add_timeout_callback(2000, on_interval_send_callback_2s, NULL);
	libtuv_add_timeout_callback(5000, on_interval_send_callback_5s, NULL);
	libtuv_add_fd_watch(socket_receive, WATCH_IO_IN, os_watch_io_receive_callback, NULL, &fd_watch);

	libtuv_loop_run();

	close(socket_receive);

	return 0;
}
