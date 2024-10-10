/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include "include/CSILogsDumper.h"
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>

#define PORT 5000
#define CONTROL_COMMAND_BUFFER_SIZE 128
#define CSI_DATA_DUMP_BUFFER_SIZE 1024

unsigned char gCSIDataDumpBuffer[CSI_DATA_DUMP_BUFFER_SIZE];
char gControlCommandBuffer[CONTROL_COMMAND_BUFFER_SIZE];

csiDataDumpListener gListener = NULL;
static pthread_t gThreadId = -1;
static int gSockFd = -1, gFifoFd = -1;
struct sockaddr_in gServerAddr, gClientAddr;
socklen_t gClientLen = sizeof(gClientAddr);
fd_set readfds;
int maxfd;

static int gWaitingForData = 0;
static int gRemainingBytes = 0;
static int gBytesReadSofar = 0;

static int recv_message(int fd, void *buf, int buflen)
{
	int received = 0;
	while (1) {
		int res = read(fd, buf + received, buflen - received);
		if (res < 0) {
			int err_no = errno;
			if (err_no == EAGAIN || err_no == EINTR) {
				continue;
			}
			CSIFW_LOGE("read error %d\n", err_no);
			return -1;
		}
		received += res;
		if (received == buflen) {
			break;
		}
	}
	return received;
}

static void print_buf(const unsigned char* buf, int len)
{
	printf("\n-------------------------------------------RAW DATA- %d ----------------------------------------------------\n\n", len);
	unsigned long long *buff_tmp = (u64 *)buf;
	int buff_len = (len / 8) + 1;
	for (int i = 0; i < buff_len; i++) {
		printf("[%02d]0x%016llx\n", i, buff_tmp[i]);
	}
}

void set_event_listener(csiDataDumpListener listener)
{
	gListener = listener;
}

void *dataTransmitter(void *arg)
{
	int len;
	while (1) {
		FD_ZERO(&readfds);
		FD_SET(gSockFd, &readfds);
		FD_SET(gFifoFd, &readfds);
		maxfd = (gSockFd > gFifoFd) ? gSockFd : gFifoFd;

		CSIFW_LOGI("Wait for an activity on one of the file descriptors");
		int ret = select(maxfd + 1, &readfds, NULL, NULL, NULL);
		if (ret < 0) {
			if (errno == EBADF){
				CSIFW_LOGE("select operation failed due to file descripter close: %d", errno);
				return;
			}
			else if(errno == EINTR){
				CSIFW_LOGE("select operation failed due to Temporary Interrupt: %d", errno);
				continue;;
			}
			CSIFW_LOGE("select operation failed over file descripters: %d", errno);
			return;
		}

		// Check if there's data on the UDP socket
		if (FD_ISSET(gSockFd, &readfds)) {
			CSIFW_LOGI("Received an activity on sockfd descriptors");
			len = recvfrom(gSockFd, gControlCommandBuffer, CONTROL_COMMAND_BUFFER_SIZE-1, 0, (struct sockaddr *)&gClientAddr, &gClientLen);
			if (len < 0) {
				continue;
			}
			gControlCommandBuffer[len] = '\0';
			if (strncmp(gControlCommandBuffer, "START", 6) == 0) {
				gListener(START_DUMP);
				CSIFW_LOGI("Received START command. Starting to send data to client from FIFO");
			} else if (strncmp(gControlCommandBuffer, "STOP", 5) == 0) {
				gListener(STOP_DUMP);
				CSIFW_LOGI("Received STOP command. Stopping data transfer to client from FIFO");
			} else {
				CSIFW_LOGI("Received unknown command: %s", gControlCommandBuffer);
				continue;
			}
		}

		// Check if there's data on the FIFO
		if (FD_ISSET(gFifoFd, &readfds)) {
			CSIFW_LOGI("Received an activity on Fifo descriptors");
			if (!gWaitingForData) {
				// Get the length from the first 2 bytes
				len = recv_message(gFifoFd, (void *)gCSIDataDumpBuffer, 2);
				if (len <= 0) {
					CSIFW_LOGE("operation to read length of data from FIFO Failed");
					continue;
				}
				uint16_t length;
				memcpy(&length , gCSIDataDumpBuffer, sizeof(length));
				CSIFW_LOGI("Read %d length to client:", length);
				gRemainingBytes = length;
				gWaitingForData = 1;
				gBytesReadSofar = 0;
			}
			if (gWaitingForData) {
				// We are expecting to read the actual data
				ssize_t bytes_read = read(gFifoFd, gCSIDataDumpBuffer + gBytesReadSofar, gRemainingBytes);
				if (bytes_read < 0) {
					int err_no = errno;
					if (err_no == EAGAIN || err_no == EINTR) {
						continue;
					}
					CSIFW_LOGE("read error %d\n", err_no);
					continue; // or return
				}

				gBytesReadSofar += bytes_read;
				gRemainingBytes -= bytes_read;

				if (gRemainingBytes == 0) {
					// Complete data received
					gWaitingForData = 0;
					// Send the data to the client
					len = sendto(gSockFd, gCSIDataDumpBuffer, gBytesReadSofar, 0, (struct sockaddr *)&gClientAddr, sizeof(gClientAddr));
					if (len == -1) {
						CSIFW_LOGE("sendto error:%d", errno);
						continue;
					}
					CSIFW_LOGI("Sent %d bytes to client", len);
					print_buf(gCSIDataDumpBuffer,len);
				}
			}
		}
	}
}

CSIFW_RES csi_logs_dumper_init(void)
{
	if ((gSockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		CSIFW_LOGE("failed to create a socket fd");
		return CSIFW_ERROR;
	}

	memset(&gServerAddr, 0, sizeof(gServerAddr));
	gServerAddr.sin_family = AF_INET;
	gServerAddr.sin_addr.s_addr = INADDR_ANY;
	gServerAddr.sin_port = htons(PORT);

	if (bind(gSockFd, (struct sockaddr*)&gServerAddr, sizeof(gServerAddr)) < 0) {
		CSIFW_LOGE("binding socket failed");
		close(gSockFd);
		return CSIFW_ERROR;
	}
	printf("server Socket Created\n");
	gFifoFd = open(CSI_DUMP_DATA_QUEUE_NAME, O_RDONLY | O_NONBLOCK);
	if (gFifoFd < 0) {
		CSIFW_LOGE("open CSI_DUMP_DATA_QUEUE fail %d", errno);
		close(gSockFd);
		return CSIFW_ERROR;
	}

	if (pthread_create(&gThreadId, NULL, dataTransmitter, NULL) != 0) {
		CSIFW_LOGE("Failed to create dataTransmitter thread %d", errno);
		close(gSockFd);
		close(gFifoFd);
		return CSIFW_ERROR;
	}

	if (pthread_setname_np(gThreadId, "CSI_DataDumpOverNetwork") != 0) {
		CSIFW_LOGE("Error in setting dataTransmitter thread name, error_no: %d", errno);
	}
	CSIFW_LOGI("CSI_DataDumpOverNetwork created");
	return CSIFW_OK;
}

CSIFW_RES csi_logs_dumper_deinit()
{
	gListener = NULL;
	close(gSockFd);
	close(gFifoFd);
	pthread_join(gThreadId, NULL);
	return CSIFW_OK;
}
