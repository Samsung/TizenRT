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
/****************************************************************************
 * examples/nettest/nettest.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * This is a leverage of similar logic from uIP:
 *
 *   Author: Adam Dunkels <adam@sics.se>
 *   Copyright (c) 2003, Adam Dunkels.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 ****************************************************************************/

/**
   * @testcase		   :lwip_nettest_01
   * @brief		   :To establish a connection between tcp server and tcp client
   * @scenario		   :
   * @apicovered	   :
   * @precondition	   : TCP Server(board1)		        TCP Client(board2)
   *			   :1. Connect to Wi-Fi		      1. Connect to Wi-Fi
   *			   :2. Get an IP address	      2. Get an IP address
   *			   :3. Run TCP Server		      3. Run TCP Client
   *			   :TASH> nettest <servip> 5001 1   TASH> nettest <servip> 5001 4
   * @postcondition	   :
   */

/**
   * @testcase		   :lwip_nettest_02
   * @brief		   :To establish a connection between udp server and udp client
   * @scenario		   :
   * @apicovered	   :
   * @precondition	   : UDP Server(board1)		        UDP Client(board2)
   *			   :1. Connect to Wi-Fi		      1. Connect to Wi-Fi
   *			   :2. Get an IP address	      2. Get an IP address
   *			   :3. Run UDP Server		      3. Run UDP Client
   *			   :TASH> nettest <servip> 5001 2   TASH> nettest <servip> 5001 5
   * @postcondition	   :
   */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <debug.h>
#include <sched.h>

#include <tinyara/progmem.h>
#include <tinyara/fs/smart.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/spi/spi.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/select.h>
#include <sys/socket.h>

#define AF_INETX AF_INET
#define PF_INETX PF_INET

/*
 * TCP
 */
#define BUF_SIZE 256

/*
 * UDP
 */
#define APP_MSG_SIZE 256

/*
 * NETWORK
 */

#define MCAST_PORT 5555
#define MCAST_GROUP "225.1.1.1"

#define NUM_PACKETS     50
#define LOCAL_DEVICE "192.168.2.10"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
* Public Data
****************************************************************************/
/* Default localIP, target IP and target port, will be set to appropriate target IP and target Port in netstresstest_main */
char *g_app_target_addr = "192.168.2.6";
int g_app_target_port = 5555;
uint32_t total_data;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void ipmcast_sender_thread(int num_packets, uint32_t sleep_time)
{

	/* ------------------------------------------------------------ */
	/*                                                              */
	/* Send Multicast Datagram code example.                        */
	/*                                                              */
	/* ------------------------------------------------------------ */
	int ret = 0;
	int i = 0;
	int datalen;
	struct in_addr localInterface;
	struct sockaddr_in groupSock;
	int sd;
	char *databuf = "Test Data: IP Multicast from TinyAra Node to Linux";

	socklen_t addrlen = sizeof(struct sockaddr_in);
	printf("\n[MCASTCLIENT] start multicast sender\n");
	/*
	 * Create a datagram socket on which to send.
	 */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		printf("[MCASTCLIENT] [ERR] opening datagram socket");
		exit(1);
	}
	printf("\n[MCASTCLIENT] created socket successfully\n");

	/*
	 * Disable loopback so you do not receive your own datagrams.
	 */

	char loopch = 0;

	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0) {
		printf("\n[MCASTCLIENT] [ERR] Failed setting IP_MULTICAST_LOOP:");
		goto errout_with_socket;
	}
	printf("\n[MCASTCLIENT] setsockopt MULTICAST_LOOP success\n");

	/*
	 * Initialize the group sockaddr structure with a
	 * group address of 225.1.1.1 and port 5555.
	 */
	memset((char *)&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr(g_app_target_addr);
	groupSock.sin_port = htons(g_app_target_port);

	/*
	 * Set local interface for outbound multicast datagrams.
	 * The IP address specified must be associated with a local,
	 * multicast-capable interface.
	 */
	localInterface.s_addr = inet_addr(LOCAL_DEVICE);
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
		printf("\n[MCASTCLIENT] [ERR] Failed setting local interface");
		goto errout_with_socket;
	}
	printf("\n[MCASTCLIENT] setsockopt IP_MULTICAST_IF success\n");

	/*
	 * Send a message to the multicast group specified by the
	 * groupSock sockaddr structure.
	 */

	datalen = strlen(databuf);
	printf("\n[MCASTCLIENT] datalen = %d \n", datalen);

	for (i = 1; i <= num_packets; i++) {
		printf("\n[MCASTCLIENT] sending mcast message (%s) length (%d) number (%d)\n", databuf, datalen, i);
		ret = sendto(sd, databuf, datalen, 0, (struct sockaddr *)&groupSock, addrlen);
		if (ret < 0) {
			printf("\n[MCASTCLIENT] [ERR] sending datagram message");
		}
		sleep(1);
	}

	printf("\n[MCASTCLIENT] Terminate multicast sender after sending sufficient messages (%d)\n", num_packets);
	close(sd);
	return;

errout_with_socket:
	close(sd);
	exit(1);
}

void ipmcast_receiver_thread(int num_packets)
{
	int ret = 0;
	int count = 0;
	int datalen;
	char databuf[256];
	int sd;
	struct sockaddr_in localSock;
	struct ip_mreq group;
	printf("\n[MCASTSERV] start multicast receiver\n");
	/* ------------------------------------------------------------ */
	/*                                                              */
	/* Receive Multicast Datagram code example.                     */
	/*                                                              */
	/* ------------------------------------------------------------ */

	/*
	 * Create a datagram socket on which to receive.
	 */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		printf("\n[MCASTSERV] ERR : opening datagram socket\n");
		exit(1);
	}
	printf("\n[MCASTSERV] create socket success\n");
	/*
	 * Enable SO_REUSEADDR to allow multiple instances of this
	 * application to receive copies of the multicast datagrams.
	 */

	int reuse = 1;
	ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("\n[MCASTSERV] ERR: setting SO_REUSEADDR\n");
		goto errout_with_socket;
	}
	printf("\n[MCASTSERV] set reusable success\n");

	/*
	 * Bind to the proper port number with the IP address
	 * specified as INADDR_ANY.
	 */
	memset((char *)&localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_port = htons(g_app_target_port);;
	localSock.sin_addr.s_addr = INADDR_ANY;

	if (bind(sd, (struct sockaddr *)&localSock, sizeof(localSock))) {
		printf("\n[MCASTSERV] ERR: binding datagram socket\n");
		goto errout_with_socket;
	}

	printf("\n[MCASTSERV] bind socket success\n");
	/*
	 * Join the multicast group 225.1.1.1 on the local 192.168.2.10
	 * interface.  Note that this IP_ADD_MEMBERSHIP option must be
	 * called for each local interface over which the multicast
	 * datagrams are to be received.
	 */
	group.imr_multiaddr.s_addr = inet_addr(g_app_target_addr);
	group.imr_interface.s_addr = inet_addr(LOCAL_DEVICE);
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
		printf("\n[MCASTSERV] ERR: adding multicast group\n");
		goto errout_with_socket;
	}
	printf("\n[MCASTSERV] join multicast success sucess success success success success success success\n");
	/*
	 * Read from the socket.
	 */
	while (1) {
		datalen = sizeof(databuf);
		/* any of the below call read / recv is fine */
		ret = read(sd, databuf, datalen);

		if (ret < 0) {
			printf("\n[MCASTSERV] ERR: reading datagram message\n");
		} else {
			databuf[ret] = '\0';
			count++;
			if (count > num_packets) {
				printf("\n[MCASTSERV] - Received Msg # %d] read (%s) (%d) bytes, Terminating MCLIENT as received sufficient packets for testing\n", count, databuf, ret);
				break;
			} else {
				printf("\n[MCASTSERV] - Received Msg # %d] read (%s) (%d) bytes\n", count, databuf, ret);
			}
		}
	}
	close(sd);
	return;

errout_with_socket:
	close(sd);
	exit(1);
}

int udp_server_thread(int num_packets)
{
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	int s;
	int nbytes;
	int count = 0;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[BUF_SIZE];
	fd_set rfds;
	fd_set fds;
	int ret = 0;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("\n[UDPSERV]socket fail");
		return 0;
	}

	printf("\n[UDPSERV] socket created\n");
	printf("\n[UDPSERV] debug %d %d\n", addrlen, sizeof(struct sockaddr));
	memset(&cliaddr, 0, addrlen);
	memset(&servaddr, 0, addrlen);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(g_app_target_port);

	ret = bind(s, (struct sockaddr *)&servaddr, addrlen);
	if (ret < 0) {
		perror("\n[UDPSERV]bind fail\n");
		goto errout_with_socket;
	}
	printf("\n[UDPSERV] socket binded\n");
	printf("\n[UDPSERV] waiting on port %d\n", g_app_target_port);

	FD_ZERO(&fds);
	FD_SET(s, &fds);

	int max_fd = s + 1;
	while (1) {
		rfds = fds;
		printf("[UDPSERV] -->select\n");
		int res = select(max_fd, &rfds, NULL, NULL, NULL);
		printf("[UDPSERV] <--select(%d)\n", res);
		if (res < 0) {
			printf("[UDPSERV] select error(%d)\n", errno);
			goto errout_with_socket;
		}
		if (res > 0) {
			if (FD_ISSET(s, &rfds)) {
				nbytes = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &addrlen);
				if (nbytes < 0) {
					perror("[UDPSERV] recvfrom fail");
					break;
				}
				if (nbytes == 0) {
					printf("[UDPSERV] socket closed from remote\n");
					goto errout_with_socket;
				}
				count++;
				if (count >= num_packets) {
					printf("[UDPSERV] - Received Msg # %d] Received Msg (%s) data size (%d), Exiting UDPSERV as received sufficient packets for testing\n", count, buf, nbytes);
					break;
				} else {
					printf("[UDPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n", count, buf, nbytes);
				}
			}
		} else {
			assert(0);
		}
	}

errout_with_socket:
	close(s);
	return 0;
}

void udp_client_thread(int num_packets, uint32_t sleep_time)
{
	int sockfd;
	int clilen;
	int i = 0;
	char buf[APP_MSG_SIZE];
	fd_set wfds;
	fd_set fds;
	int max_fd = 0;
	struct sockaddr_in serveraddr;

	memset(buf, 0, APP_MSG_SIZE);

	sprintf(buf, "Test Data: UDP send from TinyAra Node to Linux");

	clilen = sizeof(serveraddr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("[UDPCLIENT]socket create error(%d)\n", errno);
		exit(1);
	}
	printf("[UDPCLIENT] socket create(%d)\n", sockfd);

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(g_app_target_addr);
	serveraddr.sin_port = htons(g_app_target_port);

	FD_ZERO(&fds);
	FD_SET(sockfd, &fds);

	max_fd = sockfd + 1;
	for (i = 1; i <= num_packets; i++) {
		wfds = fds;
		printf("[UDPCLIENT] -->select\n");
		int res = select(max_fd, NULL, &wfds, NULL, 0);
		printf("[UDPCLIENT] <--select(%d)\n", res);

		if (res < 0) {
			printf("[UDPCLIENT] select error(%d)\n", errno);
			close(sockfd);
			return;
		}
		if (res > 0) {
			if (FD_ISSET(sockfd, &wfds)) {
				printf("[UDPCLIENT] -->send msg # %d data (%s) nbytes (%d) \n", i, buf, strlen(buf));
				if (sendto(sockfd, (void *)buf, strlen(buf), 0, (struct sockaddr *)&serveraddr, clilen) < 0) {
					printf("[UDPCLIENT] sendto failed\n");
				};
				printf("[UDPCLIENT] <--sent msg\n");
			} else {
				assert(0);
			}
		}
	}
	printf("[UDPCLIENT] Terminating udpclient after sending sufficient messages (%d)\n", num_packets);
	close(sockfd);
	return;
}

void tcp_server_thread(int num_packets, int infpkt)
{
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	int listenfd = -1;
	int connfd = -1;
	socklen_t clilen;

	int ret = 0;
	int recv_len = 0;
	int nbytes = 0;
	int count = 0;

	char msg[BUF_SIZE];

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("\n[TCPSERV] TCP socket failure %d\n", errno);
		exit(1);
	}

	/*
	 * Enable SO_REUSEADDR to allow multiple instances of this
	 * application to receive copies of the multicast datagrams.
	 */

	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("\n[TCPSERV] ERR: setting SO_REUSEADDR\n");
		goto errout_with_socket;
	}
	printf("\n[TCPSERV] set reusable success\n");

	/* Connect the socket to the server */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = HTONS(g_app_target_port);

	ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0) {
		perror("\n[TCPSERV] bind fail\n");
		goto errout_with_socket;
	}

	printf("\n[TCPSERV] Listening... port %d\n", g_app_target_port);

	ret = listen(listenfd, 1024);
	if (ret < 0) {
		perror("\n[TCPSERV] listen fail\n");
		goto errout_with_socket;
	}

	clilen = sizeof(cliaddr);

	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
	if (connfd < 0) {
		perror("\n[TCPSERV] accept fail\n");
		goto errout_with_socket;
	}
	printf("\n[TCPSERV] Accepted\n");

	recv_len = sizeof(msg);
	while (1) {
		nbytes = recv(connfd, msg, recv_len, 0);
		if (nbytes <= 0) {
			/* connection closed */
			printf("\n[TCPSERV] selectserver: socket hung up err\n");
			break;
		}
		count++;
		if (infpkt == 0) {
			if (count > num_packets) {
				printf("\n[TCPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n Exiting tcp_server as received sufficient packets for testing\n", count, msg, nbytes);
				break;
			} else {
				printf("[TCPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n", count, msg, nbytes);
			}
		} else if (infpkt == 1) {
			if (count >= num_packets) {
				printf("Count %d\n", count);
				num_packets += num_packets;
			} else {
				printf("[TCPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n", count, msg, nbytes);
			}
		}
	}
	if (connfd > 0) {
		close(connfd);
		printf("\n[TCPSERV] Closed connfd successfully \n");
	}
	close(listenfd);
	printf("\n[TCPSERV] Closed listenfd successfully \n");
	return;

errout_with_socket:
	close(listenfd);
	exit(1);
}

void tcp_client_thread(int num_packets, uint32_t sleep_time)
{
	struct sockaddr_in myaddr;
	int sockfd;
	int ret = 0;
	int i = 0;
	uint32_t sbuf_size = 0;
	uint32_t send_try = 0;
	char buf[BUF_SIZE];
	socklen_t addrlen;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("\n[TCPCLIENT] TCP socket failure %d\n", errno);
		exit(1);
	}

	/* Connect the socket to the server */
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = HTONS(g_app_target_port);
	inet_pton(AF_INET, g_app_target_addr, &(myaddr.sin_addr));

	addrlen = sizeof(struct sockaddr_in);

	printf("\n[TCPCLIENT] Connecting...\n");
	if (connect(sockfd, (struct sockaddr *)&myaddr, addrlen) < 0) {
		printf("\n[TCPCLIENT] connect fail: %d\n", errno);
		goto errout_with_socket;
	}
	printf("\n[TCPCLIENT] Connected\n");

	/* Then send num_packets number of messages */

	sprintf(buf, "Test Data: TCP send from TinyAra Node to Linux");

	for (i = 1; i <= num_packets; i++) {
		sbuf_size = strlen(buf);
		printf("\n\n\n[TCPCLIENT] -->send msg # %d data (%s) nbytes (%d) \n", i, buf, strlen(buf));

		ret = send(sockfd, buf, sbuf_size, 0);

		if (ret <= 0) {

			if (ret == 0) {
				printf("\n[TCPCLIENT] connection closed\n");
				break;
			}

			if (errno == EWOULDBLOCK) {
				if (send_try++ > 100) {
					printf("\n[TCPCLIENT] Sending try is more than LIMIT(%d)\n", 100);
					break;
				}
				printf("\n[TCPCLIENT] wouldblock, retry delay 200ms!!\n");
				usleep(200000);
				continue;
			}

			printf("\n[TCPCLIENT] socket error ret(%d) err(%d) EAGAIN(%d)\n", ret, errno, EWOULDBLOCK);
			break;
		}
		send_try = 0;
		printf("\n[TCPCLIENT] <--send\t%d bytes\n\n", ret);
		sleep(1);

	}

	printf("\n[TCPCLIENT] Terminating tcpclient after sending sufficient messages (%d)\n", num_packets);

	close(sockfd);
	return;

errout_with_socket:
	close(sockfd);
	exit(1);
}

/* Sample App to test Transport Layer (TCP / UDP) / IP Multicast Functionality */
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int nettest_main(int argc, char *argv[])
#endif
{
	nlldbg("Running nettest_main \n");
	int proto = 0;
	int num_packets_to_process = NUM_PACKETS;
	uint32_t sleep_time = 0;
	/* pps - packet per second, default value 1 */
	uint32_t pps = 1;
	if (argc < 4) {
		printf("\n\nUsage1: nettest target_addr target_port [0(tcpserv -infinite pkt receive), 1(tcpserv), 2(udpserv), 3(ipmulticastreceiver)]\n\n");
		printf("\n\nSample Command1 (for ipmulticastreceiver): nettest 225.1.1.1 5555 3\n\n");
		printf("\n\n");
		printf("\n\nUsage2: nettest target_addr target_port [4(tcpclient), 5(udpclient), 6(ipmulticastsender)] pps(optional, number of packets transmitted per second - default value 1)\n\n");
		printf("\n\nSample Command2 (for udpclient): nettest 192.168.2.5 5555 5 4\n\n");
		printf("\n\n");
		return 0;
	}

	g_app_target_addr = argv[1];
	g_app_target_port = atoi(argv[2]);
	proto = atoi(argv[3]);

	if (argc == 5) {
		pps = atoi(argv[4]);
	}

	sleep_time = 1000000ul / pps;

	printf("\n[NETTEST APP]Target addr : %s Target port %d protocol_under_test(%d) pps(%d)\n", g_app_target_addr, g_app_target_port, proto, pps);

	switch (proto) {
	case 0:
		/* TCP Receive infinite pkts */
		tcp_server_thread(num_packets_to_process, 1);
		break;
	case 1:
		/* TCP receive test */
		tcp_server_thread(num_packets_to_process, 0);
		break;
	case 2:
		/* UDP receive test */
		udp_server_thread(num_packets_to_process);
		break;
	case 3:
		/* ipmulticast_receiver */
		ipmcast_receiver_thread(num_packets_to_process);
		break;
	case 4:
		/* TCP send test */
		tcp_client_thread(num_packets_to_process, sleep_time);
		break;
	case 5:
		/* UDP send test */
		udp_client_thread(num_packets_to_process, sleep_time);
		break;
	case 6:
		/* ipmulticast_sender */
		ipmcast_sender_thread(num_packets_to_process, sleep_time);
		break;
	default:
		printf("[NETTEST APP] Invalid proto type\n");
		break;
	}
	printf("\nExiting nettest_main thread, job finished\n");
	return 0;
}
