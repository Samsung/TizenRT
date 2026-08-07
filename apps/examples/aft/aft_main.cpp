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
#define STREAM_QUEUE_MAX_ITEMS 256U
#define STREAM_QUEUE_MAX_BYTES (1024U * 1024U)

struct stream_queue_item_s {
	struct stream_queue_item_s *next;
	uint64_t sequence;
	uint32_t length;
	uint8_t data[0];
};

static char g_buf[BUF_SIZE] = {0,};
static char g_name_buf[128];
static pthread_mutex_t g_stream_lock = PTHREAD_MUTEX_INITIALIZER;
static sem_t g_stream_request;
static sem_t g_stream_request_done;
static bool g_stream_started = false;
static bool g_stream_end_requested = false;
static bool g_stream_failed = false;
static struct stream_queue_item_s *g_stream_queue_head = NULL;
static struct stream_queue_item_s *g_stream_queue_tail = NULL;
static uint32_t g_stream_queue_items = 0;
static size_t g_stream_queue_bytes = 0;
static uint64_t g_stream_next_sequence = 1;
static int g_stream_result = 0;
int g_aft_soc = -1;

static void stream_queue_clear_locked(void)
{
	struct stream_queue_item_s *item = g_stream_queue_head;

	while (item) {
		struct stream_queue_item_s *next = item->next;
		free(item);
		item = next;
	}

	g_stream_queue_head = NULL;
	g_stream_queue_tail = NULL;
	g_stream_queue_items = 0;
	g_stream_queue_bytes = 0;
}

static struct stream_queue_item_s *stream_queue_dequeue_locked(void)
{
	struct stream_queue_item_s *item = g_stream_queue_head;

	if (item == NULL) {
		return NULL;
	}

	g_stream_queue_head = item->next;
	if (g_stream_queue_head == NULL) {
		g_stream_queue_tail = NULL;
	}
	g_stream_queue_items--;
	g_stream_queue_bytes -= item->length;
	item->next = NULL;
	return item;
}

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
		g_stream_end_requested = false;
		g_stream_failed = false;
		stream_queue_clear_locked();
		g_stream_next_sequence = 1;
		g_stream_result = 0;
		pthread_mutex_unlock(&g_stream_lock);
		close(listenfd);
		printf("[AFT] Stream connection is ready\n");

		int wait_ret = 0;
		while (true) {
			struct stream_queue_item_s *item;
			bool close_stream;
			bool stream_failed;
			uint32_t queue_items = 0;
			size_t queue_bytes = 0;

			do {
				wait_ret = sem_wait(&g_stream_request);
			} while (wait_ret < 0 && errno == EINTR);
			if (wait_ret < 0) {
				break;
			}

			pthread_mutex_lock(&g_stream_lock);
			item = stream_queue_dequeue_locked();
			queue_items = g_stream_queue_items;
			queue_bytes = g_stream_queue_bytes;
			close_stream = (item == NULL && g_stream_end_requested);
			stream_failed = g_stream_failed;
			pthread_mutex_unlock(&g_stream_lock);

			if (item) {
				printf("[AFT][stream] dequeue seq=%llu bytes=%u queued_items=%u queued_bytes=%zu\n",
					(unsigned long long)item->sequence, item->length,
					queue_items, queue_bytes);
				int send_ret = stream_failed ? -1 : send_data(connfd, item->data, item->length);
				if (send_ret == 0) {
					printf("[AFT][stream] sent seq=%llu bytes=%u\n",
						(unsigned long long)item->sequence, item->length);
				} else {
					printf("[AFT][stream] send failed seq=%llu bytes=%u\n",
						(unsigned long long)item->sequence, item->length);
					pthread_mutex_lock(&g_stream_lock);
					g_stream_failed = true;
					g_stream_result = -1;
					pthread_mutex_unlock(&g_stream_lock);
				}
				free(item);
				continue;
			}

			if (close_stream) {
				close(connfd);
				pthread_mutex_lock(&g_stream_lock);
				g_aft_soc = -1;
				g_stream_started = false;
				g_stream_result = g_stream_failed ? -1 : 0;
				stream_queue_clear_locked();
				pthread_mutex_unlock(&g_stream_lock);
				sem_post(&g_stream_request_done);
				break;
			}
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

/* Raw stream protocol: send_stream_buf copies DSP bytes into a FIFO; socket close is EOF. */
int send_stream_start(void)
{
	pthread_mutex_lock(&g_stream_lock);
	if (g_aft_soc < 0 || g_stream_started || g_stream_end_requested) {
		pthread_mutex_unlock(&g_stream_lock);
		return -EINVAL;
	}

	g_stream_started = true;
	pthread_mutex_unlock(&g_stream_lock);
	return 0;
}

int send_stream_buf(const uint8_t *buffer, uint32_t length)
{
	struct stream_queue_item_s *item;
	uint32_t queue_items;
	uint64_t sequence;
	size_t queue_bytes;

	if (buffer == NULL || length == 0 || length > STREAM_QUEUE_MAX_BYTES ||
		length > SIZE_MAX - sizeof(*item)) {
		return -EINVAL;
	}

	item = (struct stream_queue_item_s *)malloc(sizeof(*item) + length);
	if (item == NULL) {
		printf("[AFT][stream] enqueue failed: allocation bytes=%u\n", length);
		return -ENOMEM;
	}
	item->next = NULL;
	item->length = length;
	memcpy(item->data, buffer, length);

	pthread_mutex_lock(&g_stream_lock);
	if (g_aft_soc < 0 || !g_stream_started || g_stream_end_requested || g_stream_failed) {
		pthread_mutex_unlock(&g_stream_lock);
		free(item);
		return -EINVAL;
	}
	if (g_stream_queue_items >= STREAM_QUEUE_MAX_ITEMS ||
		g_stream_queue_bytes > STREAM_QUEUE_MAX_BYTES - length) {
		printf("[AFT][stream] enqueue full: bytes=%u queued_items=%u queued_bytes=%zu\n",
			length, g_stream_queue_items, g_stream_queue_bytes);
		pthread_mutex_unlock(&g_stream_lock);
		free(item);
		return -ENOSPC;
	}

	sequence = g_stream_next_sequence++;
	item->sequence = sequence;
	if (g_stream_queue_tail) {
		g_stream_queue_tail->next = item;
	} else {
		g_stream_queue_head = item;
	}
	g_stream_queue_tail = item;
	g_stream_queue_items++;
	g_stream_queue_bytes += length;
	queue_items = g_stream_queue_items;
	queue_bytes = g_stream_queue_bytes;
	pthread_mutex_unlock(&g_stream_lock);
	printf("[AFT][stream] enqueue seq=%llu bytes=%u queued_items=%u queued_bytes=%zu\n",
		(unsigned long long)sequence, length, queue_items, queue_bytes);
	if (sem_post(&g_stream_request) != 0) {
		printf("[AFT][stream] enqueue notification failed (%d)\n", errno);
		return -1;
	}
	return 0;
}

int send_stream_end(void)
{
	pthread_mutex_lock(&g_stream_lock);
	if (g_aft_soc < 0 || !g_stream_started || g_stream_end_requested) {
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

	pthread_mutex_lock(&g_stream_lock);
	int ret = g_stream_result;
	pthread_mutex_unlock(&g_stream_lock);
	return ret;
}
}
