/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <unistd.h>
#include "aft_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>
#include <string>
using namespace std;
#define SFT_PORT 5555
#define BUF_SIZE 2048
static char g_buf[BUF_SIZE] = {0,};
static char g_name_buf[128];
static pthread_mutex_t g_stream_lock = PTHREAD_MUTEX_INITIALIZER;
static sem_t g_stream_request;
static sem_t g_stream_request_done;
static bool g_stream_started = false;
static bool g_stream_request_pending = false;
static bool g_stream_end_requested = false;
static const uint8_t *g_stream_buffer = NULL;
static uint32_t g_stream_length = 0;
static int g_stream_result = 0;
int g_aft_soc = -1;

static int send_data(int sd, const void *buffer, size_t buf_size)
{
	const uint8_t *buf = static_cast<const uint8_t *>(buffer);
	size_t sent = 0;
	while (sent < buf_size) {
		ssize_t ret = send(sd, buf + sent, buf_size - sent, 0);
		if (ret <= 0) {
			if (ret == 0) {
				printf("[AFT] connection closed\n");
				return -1;
			}
			if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
				continue;
			}
			printf("[AFT] send failed (%d)\n", errno);
			return -1;
		}
		sent += static_cast<size_t>(ret);
	}
	return 0;
}

static int read_data(int sd, char *buf, size_t buf_size, size_t *readed)
{
	ssize_t nbytes;
	size_t read_size = 0;
	while (read_size < buf_size) {
		nbytes = recv(sd, buf + read_size, buf_size - read_size, 0);
		if (nbytes == 0) {
			printf("[AFT] connection closed\n");
			return -1;
		} else if (nbytes < 0) {
			printf("[AFT] unknown error(%d)\n", errno);
			return -1;
		}
		// Check for potential overflow before addition
		if (read_size > SIZE_MAX - nbytes) {
			printf("[AFT] integer overflow detected\n");
			return -1;
		}
		read_size += nbytes;
		*readed = read_size;
	}
	return 0;
}

static size_t get_file_size(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	size_t file_size = static_cast<size_t>(ftell(fp));

	printf("[AFT] file_size (%zu)\n", file_size);
	fseek(fp, 0, SEEK_SET);

	return file_size;
}

static void send_file(int connfd)
{
	int ret;
	size_t readed = 0;
	uint32_t net_file_name_size = 0;
	uint32_t file_name_size = 0;
	ret = read_data(connfd, (char *)&net_file_name_size, sizeof(int), &readed);
	if (ret < 0) {
		printf("[AFT] readed %zu %d\n", readed, __LINE__);
		return;
	}
	file_name_size = ntohl(net_file_name_size);
	// Validate file_name_size before use
	if (file_name_size == 0 || file_name_size > sizeof(g_name_buf) - 1) {
		printf("[AFT] invalid file name size: %u\n", file_name_size);
		return;
	}
	printf("[AFT] file name length(%u) %zu\n", file_name_size, readed);

	ret = read_data(connfd, g_name_buf, static_cast<size_t>(file_name_size), &readed);
	if (ret < 0) {
		printf("[AFT] fail %d\n", ret);
		return;
	}
	g_name_buf[readed] = 0;
	printf("[AFT] file name: %s\n", g_name_buf);
	
	FILE *fp = fopen(g_name_buf, "rb");
	if (!fp) {
		printf("[AFT] fail %d\n", __LINE__);
		return;
	}

	//get file size
	size_t file_size = get_file_size(fp);
	printf("[AFT] send file size %zu\n", file_size);
	int net_file_size = htonl(static_cast<uint32_t>(file_size));
	ret = send_data(connfd, (char *)&net_file_size, sizeof(int));
	
	if (ret < 0) {
		printf("[AFT] error %d\n", errno);
		fclose(fp);
		return;
	}
	printf("[AFT] file size: %zu\n", file_size);

	readed = 0;
	while (readed < file_size) {
		ret = fread(g_buf, sizeof(char), BUF_SIZE, fp);
		if (ret < 0) {
			fclose(fp);
			return;
		}
		send_data(connfd, g_buf, ret);
		readed += ret;
		printf("[AFT] Successfully Sent %zu bytes until now.\n", readed);
	}

	printf("[AFT] <--send : %zu bytes\n", readed);
	fclose(fp);
	return;
}

/****************************************************************************
 * aft_main
 ****************************************************************************/
extern "C"
{
int aft_main(int argc, char *argv[])
{
	printf("[AFT] Main called");
	bool stream_mode = false;
	if (argc == 2 && strcmp(argv[1], "stream") == 0) {
		stream_mode = true;
	} else if (argc != 1) {
		printf("\nUsage: aft [stream]\n");
		return -EINVAL;
	}

	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	int listenfd = -1;
	int connfd = -1;
	socklen_t clilen;
	int ret = 0;

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("[AFT] TCP socket failure %d\n", errno);
		return -1;
	}

	/*
	 * Enable SO_REUSEADDR to allow multiple instances of this
	 * application to receive copies of the multicast datagrams.
	 */

	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("[AFT] ERR: setting SO_REUSEADDR\n");
		close(listenfd);
		return 0 ;
	}
	printf("[AFT] set reusable success\n");

	/* Connect the socket to the server */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SFT_PORT);

	ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0) {
		perror("[AFT] bind fail\n");
		close(listenfd);
		return 0;
	}

	printf("[AFT] Listening... port %d\n", SFT_PORT);

	ret = listen(listenfd, 1024);
	if (ret < 0) {
		perror("[AFT] listen fail\n");
		close(listenfd);
		return 0;
	}

	clilen = sizeof(cliaddr);

	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
	if (connfd < 0) {
		perror("[AFT] accept fail\n");
		close(listenfd);
		return 0;
	}
	printf("[AFT] Accepted\n");
	if (stream_mode) {
		if (sem_init(&g_stream_request, 0, 0) != 0) {
			printf("[AFT] failed to initialize stream semaphore (%d)\n", errno);
			close(connfd);
			close(listenfd);
			return -1;
		}

		if (sem_init(&g_stream_request_done, 0, 0) != 0) {
			printf("[AFT] failed to initialize stream request semaphore (%d)\n", errno);
			sem_destroy(&g_stream_request);
			close(connfd);
			close(listenfd);
			return -1;
		}

		pthread_mutex_lock(&g_stream_lock);
		g_aft_soc = connfd;
		g_stream_started = false;
		g_stream_request_pending = false;
		g_stream_end_requested = false;
		g_stream_buffer = NULL;
		g_stream_length = 0;
		g_stream_result = 0;
		pthread_mutex_unlock(&g_stream_lock);
		close(listenfd);
		printf("[AFT] Stream connection is ready\n");

		int wait_ret = 0;
		while (true) {
			do {
				wait_ret = sem_wait(&g_stream_request);
			} while (wait_ret < 0 && errno == EINTR);
			if (wait_ret < 0) {
				break;
			}

			pthread_mutex_lock(&g_stream_lock);
			bool end_requested = g_stream_end_requested;
			const uint8_t *buffer = g_stream_buffer;
			uint32_t length = g_stream_length;
			pthread_mutex_unlock(&g_stream_lock);

			if (end_requested) {
				close(connfd);
				pthread_mutex_lock(&g_stream_lock);
				g_aft_soc = -1;
				g_stream_started = false;
				g_stream_request_pending = false;
				g_stream_result = 0;
				pthread_mutex_unlock(&g_stream_lock);
				sem_post(&g_stream_request_done);
				break;
			}

			int send_ret = send_data(connfd, buffer, length);
			pthread_mutex_lock(&g_stream_lock);
			g_stream_result = send_ret;
			g_stream_request_pending = false;
			g_stream_buffer = NULL;
			g_stream_length = 0;
			pthread_mutex_unlock(&g_stream_lock);
			sem_post(&g_stream_request_done);
		}

		sem_destroy(&g_stream_request_done);
		sem_destroy(&g_stream_request);
		return wait_ret;
	}

	send_file(connfd);
	close(connfd);
	close(listenfd);
	printf("[AFT] Stopping Perfectly\n");
	return 0;
}

/* Raw stream protocol: send_stream_buf transmits only DSP bytes; socket close is EOF. */
int send_stream_start(void)
{
	pthread_mutex_lock(&g_stream_lock);
	if (g_aft_soc < 0 || g_stream_started) {
		pthread_mutex_unlock(&g_stream_lock);
		return -EINVAL;
	}

	g_stream_started = true;
	pthread_mutex_unlock(&g_stream_lock);
	return 0;
}

int send_stream_buf(const uint8_t *buffer, uint32_t length)
{
	pthread_mutex_lock(&g_stream_lock);
	if (g_aft_soc < 0 || !g_stream_started || g_stream_request_pending ||
		buffer == NULL || length == 0) {
		pthread_mutex_unlock(&g_stream_lock);
		return -EINVAL;
	}

	g_stream_buffer = buffer;
	g_stream_length = length;
	g_stream_request_pending = true;
	pthread_mutex_unlock(&g_stream_lock);

	if (sem_post(&g_stream_request) != 0) {
		return -1;
	}

	int wait_ret;
	do {
		wait_ret = sem_wait(&g_stream_request_done);
	} while (wait_ret < 0 && errno == EINTR);
	if (wait_ret < 0) {
		return -1;
	}

	pthread_mutex_lock(&g_stream_lock);
	int ret = g_stream_result;
	pthread_mutex_unlock(&g_stream_lock);
	return ret;
}

int send_stream_end(void)
{
	pthread_mutex_lock(&g_stream_lock);
	if (g_aft_soc < 0 || !g_stream_started) {
		pthread_mutex_unlock(&g_stream_lock);
		return -EINVAL;
	}

	g_stream_end_requested = true;
	pthread_mutex_unlock(&g_stream_lock);

	if (sem_post(&g_stream_request) != 0) {
		return -1;
	}

	int wait_ret;
	do {
		wait_ret = sem_wait(&g_stream_request_done);
	} while (wait_ret < 0 && errno == EINTR);
	if (wait_ret < 0) {
		return -1;
	}

	int ret;
	pthread_mutex_lock(&g_stream_lock);
	ret = g_stream_result;
	pthread_mutex_unlock(&g_stream_lock);
	return ret;
}
}
