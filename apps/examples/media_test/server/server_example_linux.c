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
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SEND_BUFFER_SIZE (1 << 16)
#define RECV_BUFFER_SIZE (1 << 11)

#define INST_PLAY         0x00
#define INST_STOP         0x01
#define INST_PAUSE        0x02
#define INST_RESUME       0x03
#define INST_RECORD       0x04
#define INST_RECORD_DATA  0x05
#define INST_QUIT         0x06

#define SEND_STATE_NONE         0x00
#define SEND_STATE_PAYLOAD_SIZE 0x01
#define SEND_STATE_PLAYING      0x02
#define SEND_STATE_STOP         0x03
#define SEND_STATE_PAUSE        0x04

#define SEND_STATE_LOCK()   do { pthread_mutex_lock(&g_sc.mutex_state); } while (false)
#define SEND_STATE_UNLOCK() do { pthread_mutex_unlock(&g_sc.mutex_state); } while (false)

struct server_context_s {
	pthread_t		pth_recv;
	pthread_t		pth_send;
	bool			recv_thread_running;
	bool			send_thread_running;
	pthread_mutexattr_t	mutex_attr_state;
	pthread_mutex_t		mutex_state;
	sem_t			sem_pause;
	int			server_sock;
	int			client_sock;
	int			send_state;
};

struct server_context_s g_sc;
char g_send_buffer[SEND_BUFFER_SIZE];
char g_receive_buffer[RECV_BUFFER_SIZE];

bool recv_data(void *ptr, size_t size)
{
	int ret;
	size_t recvd = 0;
	while (recvd < size) {
		ret = recv(g_sc.client_sock, (char *)ptr + recvd, size - recvd, 0);
		if (ret < 0) {
			printf("Client disconnected\n");
			return false;
		}
		recvd += ret;
	}

	return true;
}

bool send_data(void *ptr, size_t size)
{
	int ret;
	size_t sent = 0;
	while (sent < size) {
		ret = send(g_sc.client_sock, (char *)ptr + sent, size - sent, 0);
		if (ret < 0) {
			printf("Client disconnected\n");
			return false;
		}
		printf("Sent: %d bytes\n", ret);
		sent += ret;
	}

	return true;
}

void *recv_thread(void *arg)
{
	int packet_size = 0;
	char inst = 0;
	FILE *fp = NULL;

	while (g_sc.recv_thread_running) {
		recv(g_sc.client_sock, &inst, 1, 0);
		printf("Instruction : %d\n", inst);
		switch (inst) {
		case INST_PLAY:
			SEND_STATE_LOCK();
			g_sc.send_state = SEND_STATE_PAYLOAD_SIZE;
			SEND_STATE_UNLOCK();
			break;

		case INST_STOP:
			SEND_STATE_LOCK();
			g_sc.send_state = SEND_STATE_STOP;
			close(g_sc.client_sock);
			g_sc.recv_thread_running = false;
			g_sc.send_thread_running = false;
			g_sc.client_sock = -1;
			SEND_STATE_UNLOCK();
			break;

		case INST_PAUSE:
			SEND_STATE_LOCK();
			g_sc.send_state = SEND_STATE_PAUSE;
			SEND_STATE_UNLOCK();
			break;

		case INST_RESUME:
			SEND_STATE_LOCK();
			g_sc.send_state = SEND_STATE_PLAYING;
			sem_post(&g_sc.sem_pause);
			SEND_STATE_UNLOCK();
			break;

		case INST_RECORD:
			fp = fopen("./data.raw", "wb");
			if (!fp) {
				printf("Error: fopen()\n");
				close(g_sc.server_sock);
				exit(1);
			}
			break;

		case INST_RECORD_DATA:
			recv_data(&packet_size, sizeof(int));
			recv_data(g_receive_buffer., packet_size);
			fwrite(g_receive_buffer., 1, packet_size, fp);
			break;

		case INST_QUIT:
			SEND_STATE_LOCK();
			g_sc.send_state = SEND_STATE_NONE;
			close(g_sc.client_sock);
			close(g_sc.server_sock);
			g_sc.recv_thread_running = false;
			g_sc.send_thread_running = false;
			g_sc.server_sock = -1;
			g_sc.client_sock = -1;
			SEND_STATE_UNLOCK();
			break;
		}
	}

	if (fp) {
		fclose(fp);
		fp = NULL;
	}
	printf("Recv thread finished.\n");

	return NULL;
}

void *send_thread(void *arg)
{
	int len = 0;
	FILE *fp = NULL;

	while (g_sc.send_thread_running) {
		SEND_STATE_LOCK();
		switch (g_sc.send_state) {
		case SEND_STATE_NONE:
			usleep(100);
			break;

		case SEND_STATE_PAYLOAD_SIZE: {
			fp = fopen("./music.wav", "rb");
			if (!fp) {
				printf("Error: fopen()\n");
				close(g_sc.server_sock);
				exit(1);
			}
			fseek(fp, 44L, SEEK_SET);
			g_sc.send_state = SEND_STATE_PLAYING;
		}
		break;

		case SEND_STATE_PLAYING:
			if (feof(fp)) {
				close(g_sc.client_sock);
				g_sc.client_sock = -1;
				g_sc.send_thread_running = false;
				g_sc.recv_thread_running = false;
			} else {
				len = fread(g_send_buffer, 1, SEND_BUFFER_SIZE, fp);
				if (!send_data(g_send_buffer, len)) {
					g_sc.send_thread_running = false;
					close(g_sc.client_sock);
					g_sc.client_sock = -1;
				}
			}
			break;

		case SEND_STATE_STOP:
			g_sc.send_thread_running = false;
			break;

		case SEND_STATE_PAUSE:
			SEND_STATE_UNLOCK();
			sem_wait(&g_sc.sem_pause);
			SEND_STATE_LOCK();
			break;
		}
		SEND_STATE_UNLOCK();
	}
	if (fp) {
		fclose(fp);
		fp = NULL;
	}
	printf("Send thread finished.\n");
	g_sc.send_state = SEND_STATE_NONE;
	return NULL;
}

void server_init()
{
	int client_addr_size;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	memset(&g_sc, 0, sizeof(g_sc));
	g_sc.send_state = SEND_STATE_NONE;

	g_sc.server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (g_sc.server_sock < 0) {
		printf("Error: socket()\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.213"); // INADDR_ANY;

	if (bind(g_sc.server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Error: bind()\n");
		close(g_sc.server_sock);
		exit(1);
	}

	if (listen(g_sc.server_sock, 50) == -1) {
		printf("Error: listen()\n");
		close(g_sc.server_sock);
		exit(1);
	}

	sem_init(&g_sc.sem_pause, 0, 0);
}

int main()
{
	socklen_t client_addr_size;
	struct sockaddr_in client_addr;

	server_init();
	while (true) {
		client_addr_size = sizeof(client_addr);
		g_sc.client_sock = accept(g_sc.server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
		if (g_sc.client_sock < 0) {
			printf("Error: accept()\n");
			close(g_sc.server_sock);
			exit(1);
		}
		printf("Client connected!!\n");

		pthread_mutexattr_init(&g_sc.mutex_attr_state);
		pthread_mutexattr_settype(&g_sc.mutex_attr_state, PTHREAD_MUTEX_NORMAL);
		pthread_mutex_init(&g_sc.mutex_state, &g_sc.mutex_attr_state);

		g_sc.recv_thread_running = true;
		if (pthread_create(&g_sc.pth_recv, NULL, recv_thread, NULL) < 0) {
			printf("Error: pthread_create(recv)\n");
			close(g_sc.client_sock);
			close(g_sc.server_sock);
			exit(1);
		}

		g_sc.send_thread_running = true;
		if (pthread_create(&g_sc.pth_send, NULL, send_thread, NULL) < 0) {
			printf("Error: pthread_create(send)\n");
			close(g_sc.client_sock);
			close(g_sc.server_sock);
			exit(1);
		}
	}

	return 0;
}
