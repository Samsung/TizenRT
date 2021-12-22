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
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include "connect_test_data.h"
#include "connect_test_server.h"
#include "connect_test_log.h"

#define TAG "[CT_TCP]"

static char g_buf[BUF_SIZE];
static int data_transferred = 0;
static int server_recv_send_data = 0;
static int client_send_recv_data = 0;

extern volatile bool g_broadcast_flag;

extern bool g_tcp_server_recvd_data;
extern bool g_tcp_server_sentack_data;

extern bool g_tcp_server_sent_data;
extern bool g_tcp_server_recvdack_data;

extern bool g_tcp_client_sent_data;
extern bool g_tcp_client_recvdack_data;

extern bool g_tcp_client_recvd_data;
extern bool g_tcp_client_sentack_data;

static int _recv_data(int fd, int size)
{
	int nbytes = recv(fd, g_buf, size, 0);	
	DATA_TRANSFER_ERROR
	return nbytes;
}

static int _send_data(int fd, int size)
{
	int nbytes = send(fd, g_buf, size, 0);	
	DATA_TRANSFER_ERROR
	return nbytes;
}

static int _data_transfer(int transfer_mode, int fd, int size)
{
	int remain = size, done = 0, nbytes;
	bool isError = false;	
	while (remain > 0) {
		int packet_size = remain > BUF_SIZE ? BUF_SIZE : remain;
		nbytes = transfer_mode ? _recv_data(fd, packet_size) : _send_data(fd, packet_size);
		if (nbytes == -1) {
			isError = true;
			break;
		}
		done += nbytes;
		remain -= nbytes;
	}
	data_transferred = done;	
	CT_LOG(TAG, "%s: %d", (transfer_mode == 1 ? "Rcvd_Till_Now" : "Sent_Till_Now"), done);
	if (isError) {
		CT_LOGE(TAG, "error ");
		return -1;
	}
	return 0;
}

int _tcp_server(int msg_size, long *sum)
{
	struct timeval start, end;
	struct sockaddr_in saddr, caddr;
	int listenfd, sockfd = 0, ret = 0;
	char buf[10];		

	/* Socket Creation */
	CT_LOG(TAG, "create socket");
	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		CT_LOGE(TAG, "create socket fail %d", errno);
		return -1;
	}

	g_listenfd = listenfd;

	CT_LOG(TAG, "set reuseaddr");
	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		CT_LOGE(TAG, "set reuse addr fail %d", errno);
		close(listenfd);
		return -1;
	}

	/* Set Timeout for Sockets data transfer */
	DECLARE_TIMEOUT

	ret = setsockopt(listenfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	if (ret < 0) {
		CT_LOGE(TAG, "set SO_RCVTIMEO failed %d", errno);
		close(listenfd);
		return -1;
	}
	int flag = 1;
	ret = setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
	if (ret < 0) {
		CT_LOGE(TAG, "set TCP_NODELAY failed %d", errno);
		close(listenfd);
		return -1;
	}

	/* Socket binding process */
	CT_LOG(TAG, "bind INADDR_ANY port:%d", PORT);
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = PF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(PORT);
	ret = bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (ret < 0) {
		CT_LOGE(TAG, "bind fail %d", errno);
		close(listenfd);
		return -1;
	}

	CT_LOG(TAG, "listen");
	ret = listen(listenfd, 1024);
	if (ret < 0) {
		CT_LOGE(TAG, "listen fail %d", errno);		
		close(listenfd);
		return -1;
	}

	/* Accept connection from the client */
	CT_LOG(TAG, "wait connection");
	socklen_t clilen = sizeof(caddr);				
	sockfd = accept(listenfd, (struct sockaddr*)&caddr, &clilen);
	if (sockfd < 0) {
		CT_LOGE(TAG, "accept fail %d", errno);	
		close(listenfd);
		return -1;
	}
	g_broadcast_flag = false;

	SET_START_TIME(TCP Data transfer);
	/* Receive data through tcp socket */
	if (!g_tcp_server_recvd_data) {
		memset(buf, '\0', 10);
		snprintf(buf, 10, "%d", data_transferred);
		send(sockfd, buf, strlen(buf), 0);
		server_recv_send_data = server_recv_send_data + data_transferred;
		CT_LOG(TAG, "SendHowMuchReceviedbyServer_%d", server_recv_send_data);		
		
		ret = _data_transfer(1, sockfd, msg_size - server_recv_send_data);
		TERMINATE
		CT_LOG(TAG, "TCP receive OK");
		CT_LOG(TAG, "TOTAL_BYTES_RECEIVED:_%d", server_recv_send_data + data_transferred);
		g_tcp_server_recvd_data = true;
		server_recv_send_data = 0;
		data_transferred = 0;
	}

	/* Send ack data through tcp socket */
	if (!g_tcp_server_sentack_data) {		
		CT_LOG(TAG, "send ACK data size %d", ACK_SIZE);
		ret = _data_transfer(0, sockfd, ACK_SIZE);
		TERMINATE
		CT_LOG(TAG, "TCP send ACK OK");
		g_tcp_server_sentack_data = true;		
		data_transferred = 0;
	}

	/* Send data through tcp socket */
	if (!g_tcp_server_sent_data) {
		memset(buf, '\0', 10);
		read(sockfd, buf, 10);
		server_recv_send_data = server_recv_send_data + atoi(buf);
		CT_LOG(TAG, "GetHowmuchReceivedByClientBeforeConnectionBreak_%d", server_recv_send_data);
		
		ret = _data_transfer(0, sockfd, msg_size - server_recv_send_data);
		TERMINATE
		CT_LOG(TAG, "TCP data sent OK");
		CT_LOG(TAG, "TOTAL_BYTES_SEND:_%d", server_recv_send_data + data_transferred);
		g_tcp_server_sent_data = true;
		server_recv_send_data = 0;
		data_transferred = 0;
	}

	/* Recv ack data through tcp socket */
	if (!g_tcp_server_recvdack_data) {		
		CT_LOG(TAG, "receive ACK data size %d", ACK_SIZE);		
		ret = _data_transfer(1, sockfd, ACK_SIZE);
		TERMINATE
		CT_LOG(TAG, "TCP ACK receive OK");
		g_tcp_server_recvdack_data = true;		
		data_transferred = 0;
	}

	CALCULATE_ELAPSED_TIME(TCP Data Transfer, *sum);

	close(sockfd);
	close(listenfd);
	CT_LOG(TAG, "tcp data transmission finished");
	return 0;
}

int _tcp_client(char *ipaddr, int msg_size)
{
	struct sockaddr_in saddr;
	int ret = 0, sockfd = 0;
	char buf[10];	

	CT_LOG(TAG, "create socket");
	int retrycount = 0;
retry_connection:
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		CT_LOGE(TAG, "create socket fail %d", errno);
		return -1;
	}

	/* Set Timeout for Sockets data transfer */
	DECLARE_TIMEOUT	
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		CT_LOGE(TAG, "Set time out failed for RECV");
		close(sockfd);
		return -1;
	}
	
	int flag = 1;
	
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag)) < 0) {
		CT_LOGE(TAG, "Set time out failed for TCP_NODELAY");
		close(sockfd);
		return -1;
	}

	CT_LOG(TAG, "connect to %s:%d", ipaddr, PORT);	
	/* Connect the socket to the server */
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);
	inet_pton(AF_INET, ipaddr, &(saddr.sin_addr));
	unsigned int addrlen = sizeof(struct sockaddr_in);
	bool is_connected = false;
	retrycount++;
	if (connect(sockfd, (struct sockaddr *)&saddr, addrlen) < 0) {
		CT_LOGE(TAG, "connect fail %d", errno);		
		sleep(1);		
	} else {
		CT_LOG(TAG, "connection OK");
		is_connected = true;
	}
	if (!is_connected) {
		if (retrycount <= 5) {
			CT_LOGE(TAG, "connect try again");
			close(sockfd);
			goto retry_connection;
		} else {
			CT_LOGE(TAG, "retry connect failed even after %d retry", retrycount);
			close(sockfd);
			return -1;
		}
	} else {
		/* Send data through tcp socket */
		if (!g_tcp_client_sent_data) {
			memset(buf, '\0', 10);
			read(sockfd, buf, 10);
			client_send_recv_data = client_send_recv_data + atoi(buf);
			CT_LOG(TAG, "GetHowmuchReceivedByServerBeforeConnectionBreak_%d\n", client_send_recv_data);			
						
			ret = _data_transfer(0, sockfd, msg_size - client_send_recv_data);
			TERMINATE
			CT_LOG(TAG, "TCP data sent OK");
			CT_LOG(TAG, "TOTAL_BYTES_SEND: %d", client_send_recv_data + data_transferred);
			g_tcp_client_sent_data = true;
			client_send_recv_data = 0;
			data_transferred = 0;
		}

		/* Recv ack data through tcp socket */
		if (!g_tcp_client_recvdack_data) {						
			CT_LOG(TAG, "receive ACK data size %d", ACK_SIZE);			
			ret = _data_transfer(1, sockfd, ACK_SIZE);
			TERMINATE
			CT_LOG(TAG, "TCP ACK receive OK");
			g_tcp_client_recvdack_data = true;			
			data_transferred = 0;
		}

		/* Receive data through tcp socket */
		if (!g_tcp_client_recvd_data) {
			memset(buf, '\0', 10);
			snprintf(buf, 10, "%d", data_transferred);
			send(sockfd, buf, strlen(buf), 0);
			client_send_recv_data = client_send_recv_data + data_transferred;
			CT_LOG(TAG, "SendHowMuchReceviedbyClient_%d", client_send_recv_data);			

			ret = _data_transfer(1, sockfd, msg_size - client_send_recv_data);
			TERMINATE
			CT_LOG(TAG, "TCP receive OK");
			CT_LOG(TAG, "TOTAL_BYTES_RECEIVED: %d", client_send_recv_data + data_transferred);
			g_tcp_client_recvd_data = true;
			client_send_recv_data = 0;
			data_transferred = 0;
		}

		/* Send ack data through tcp socket */
		if (!g_tcp_client_sentack_data) {			
			CT_LOG(TAG, "send ACK data size %d", ACK_SIZE);			
			ret = _data_transfer(0, sockfd, ACK_SIZE);
			TERMINATE
			CT_LOG(TAG, "TCP send ACK OK");
			g_tcp_client_sentack_data = true;			
			data_transferred = 0;
		}

		CT_LOG(TAG, "sleep 500ms -> before closing connection, allows receiving of data");
		usleep(500000);
		close(sockfd);
		CT_LOG(TAG, "tcp data send finish");
	}
	return 0;
}
