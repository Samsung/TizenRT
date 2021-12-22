/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include "connect_test_data.h"
#include "connect_test_server.h"
#include "connect_test_log.h"

#define TAG "[CT_UDP]"

static char g_buf[BUF_SIZE_UDP];
extern unsigned char g_mac_buf_server[MAC_BUF_SIZE];
extern unsigned char g_recv_mac_buf_server[MAC_BUF_SIZE];
extern unsigned char g_mac_buf_client[MAC_BUF_SIZE];
extern unsigned char g_recv_mac_buf_client[MAC_BUF_SIZE];

fd_set rfds, ofds, sfds;
static bool for_device_mac = true;

static int _recv_data(int fd, int size, struct sockaddr_in* addr)
{
	int re_try_select = 0;
	int nbytes = 0;
	unsigned int len = sizeof(struct sockaddr);
	/* Set Timeout for Sockets data transfer */
	DECLARE_TIMEOUT
	while (true) {
		int res = select(fd + 1, &rfds, NULL, NULL, &tv);
		if (res > 0) {
			break;
		}		
		if (re_try_select > 6) {			
			CT_LOGE(TAG, "Select got failed multiple times, so return");
			return -1;
		}		
		if (res == 0) {			
			CT_LOGE(TAG, "Select recv timeout error %d, try again", errno);
		} else if (res < 0) {			
			CT_LOGE(TAG, "Select recv error %d, try again", errno);			
		}
		re_try_select++;
	}
	if (FD_ISSET(fd, &rfds)) {		
		nbytes = recvfrom(fd, g_buf, size, 0, (struct sockaddr*)addr, &len);
		if (-1 == nbytes) {
			CT_LOGE(TAG, "recvfrom error %s_%d", strerror(errno), errno);
		}		
	}
	return nbytes;
}

static int _send_data(int fd, int size, struct sockaddr_in* addr)
{
	int re_try_select = 0;
	int nbytes = 0;
	unsigned int len = sizeof(struct sockaddr);
	/* Set Timeout for Sockets data transfer */
	DECLARE_TIMEOUT
	while (true) {
		int res = select(fd + 1, NULL, &sfds, NULL, &tv);
		if (res > 0) {
			break;
		}
		if (re_try_select > 6) {
			CT_LOGE(TAG,  "Select got failed multiple times, so return");			
			return -1;
		}		
		if (res == 0) {			
			CT_LOGE(TAG,  "Select send timeout error %d, try again", errno);
		} else if (res < 0) {			
			CT_LOGE(TAG,  "Select send error %d, try again", errno);			
		}
		re_try_select++;
	}
	if (FD_ISSET(fd, &sfds)) {		
		nbytes = sendto(fd, g_buf, size, 0, (struct sockaddr*)addr, len);
		if (-1 == nbytes) {
			CT_LOGE(TAG, "sendto error %s_%d", strerror(errno), errno);
		}
	}
	return nbytes;
}

static int _data_transfer(int transfer_mode, int fd, int size, struct sockaddr_in* addr)
{
	int remain = size, done = 0, nbytes;
	FD_ZERO(&ofds);
	FD_SET(fd, &ofds);	
	bool isError = false;
	DECLARE_TIMEOUT
	CT_LOG(TAG, "DATA TRANSFER START");	
	int ack  = for_device_mac ? size :0;	
	while (remain > 0) {
		sfds = ofds;
		rfds = ofds;		
		int packet_size = remain > BUF_SIZE_UDP ? BUF_SIZE_UDP : remain;
		nbytes = transfer_mode ? _recv_data(fd, packet_size, addr) : _send_data(fd, packet_size, addr);		
		if (nbytes == -1) {
			isError = true;
			break;
		}
		done += nbytes;
		if(for_device_mac) {			
			remain -= nbytes;		
		} else {			
			memset(g_buf,'\0', BUF_SIZE_UDP);			
			if(transfer_mode == 1) {				
				// Send acknowledgement for recived				
				snprintf(g_buf, ACK_SIZE_UDP, "%d", done);
				_send_data(fd, ACK_SIZE_UDP, addr);
				remain -= nbytes;
			} else {
				// Get acknowledgement for sent					
				_recv_data(fd, ACK_SIZE_UDP, addr);
				ack = atoi(g_buf);
				remain = size - ack;
			}			
		}		
	}	
	if(!for_device_mac) {	
		if(transfer_mode == 1) {						
			CT_LOG(TAG, "TOTAL_ACK_SENT_FOR_RECVD:%s", g_buf);
		} else {			
			CT_LOG(TAG, "TOTAL_ACK_RECVD_FOR_SENT:%s", g_buf);
		}
	}	
	if (isError) {
		CT_LOGE(TAG, "error");
		return -1;
	}	
	return 0;
}

int _udp_server(int msg_size, long* sum) {	
	struct timeval start, end;
	struct sockaddr_in saddr, caddr;
	int sockfd = 0, ret = 0;
	for_device_mac = true;
	/* Socket Creation */
	CT_LOG(TAG, "create_socket");
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		CT_LOGE(TAG, "create_socket fail %d", errno);
		return -1;
	}
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
		CT_LOGE(TAG, "socket_non_blocking_mode_done_failed");
		close(sockfd);
		return -1;
	}
	/* Socket binding process */
	CT_LOG(TAG, "bind_INADDR_ANY port:%d", PORT);
	memset(&saddr, 0, sizeof(saddr));
	memset(&caddr, 0, sizeof(caddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(PORT);
	ret = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret < 0) {		
		CT_LOGE(TAG, "bind_fail %d", errno);
		close(sockfd);
		return -1;
	}
	
	CT_LOG(TAG, "Receiving MAC from peer");
	ret = _data_transfer(1, sockfd, MAC_BUF_SIZE, &caddr);
	TERMINATE
	strncpy((char*)g_recv_mac_buf_server, g_buf, MAC_BUF_SIZE);	
	CT_LOG(TAG, "Peer MAC is: %s", g_recv_mac_buf_server);	
			
	CT_LOG(TAG, "Sending Device MAC to peer. Server_mac %s", g_mac_buf_server);
	strncpy(g_buf, (char*)g_mac_buf_server, MAC_BUF_SIZE);
	ret = _data_transfer(0, sockfd, MAC_BUF_SIZE, &caddr);
	TERMINATE
	
	for_device_mac = false;
	SET_START_TIME(UDP Data transfer);

	/* Receive data through udp socket */
	memset(g_buf, '\0', BUF_SIZE_UDP);
	ret = _data_transfer(1, sockfd, msg_size, &caddr);
	TERMINATE
	CT_LOG(TAG, "_recv_data_OK");
	
	/* Send data through udp socket */
	memset(g_buf, '\0', BUF_SIZE_UDP);
	ret = _data_transfer(0, sockfd, msg_size, &caddr);	
	TERMINATE	
	CT_LOG(TAG, "_send_data_OK");
	CALCULATE_ELAPSED_TIME(UDP Data Transfer, *sum);
	close(sockfd);
	CT_LOG(TAG, "terminate_udp_data_transfer");
	return 0;
}

int _udp_client(char* server_addr, int msg_size)
{
	struct sockaddr_in saddr;	
	int sockfd = 0, ret = 0;
	for_device_mac = true;

	/* Socket Creation */
	CT_LOG(TAG, "create_socket");
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		CT_LOGE(TAG, "create socket fail %d", errno);
		return -1;
	}
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
		CT_LOGE(TAG, "socket_non_blocking_mode_failed");
		close(sockfd);
		return -1;
	}	
	/* Connect the socket to the server */
	CT_LOG(TAG, "connect_to %s:%d", server_addr, PORT);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);
	inet_aton(server_addr, &(saddr.sin_addr));
		
	CT_LOG(TAG, "Sending Device MAC to peer, Client_mac: %s", g_mac_buf_client);	
	strncpy(g_buf, (char*)g_mac_buf_client, MAC_BUF_SIZE);
	ret = _data_transfer(0, sockfd, MAC_BUF_SIZE, &saddr);
	TERMINATE
	CT_LOG(TAG, "Receiving MAC from peer");	
	ret = _data_transfer(1, sockfd, MAC_BUF_SIZE, &saddr);
	TERMINATE
	strncpy((char*)g_recv_mac_buf_client, g_buf, MAC_BUF_SIZE);	
	CT_LOG(TAG, "Peer MAC is: %s", g_recv_mac_buf_client);
	for_device_mac = false;	
	
	/* Send data through udp socket */
	memset(g_buf, '\0', BUF_SIZE_UDP);
	ret = _data_transfer(0, sockfd, msg_size, &saddr);
	TERMINATE
	CT_LOG(TAG, "_send_data_OK");

	/* Receive data through udp socket */
	memset(g_buf, '\0', BUF_SIZE_UDP);
	ret = _data_transfer(1, sockfd, msg_size, &saddr);
	TERMINATE
	CT_LOG(TAG, "_recv_data_OK");
	close(sockfd);
	CT_LOG(TAG, "udp_data_transmission_finished");	
	return 0;
}