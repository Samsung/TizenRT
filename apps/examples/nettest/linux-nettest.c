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

/* nettest sample application only for Linux */

#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 256

#define NUM_PACKETS 100

#define NETTEST_IPV(x) ((x) ? "_V6" : "_V4")

char *g_app_netif_addr;
char *g_app_target_addr;
int g_app_target_port;

void ipmcast_sender_thread(int num_packets)
{
	int ret = 0;
	int i = 0;
	int datalen;
	struct in_addr localInterface;
	struct sockaddr_in groupSock;
	int sd;
	char *databuf = "Test Data: IP Multicast from TinyAra Node to Linux";

	socklen_t addrlen = sizeof(struct sockaddr_in);
	printf("\n[MCASTSENDER] start multicast sender\n");
	 
	/* Create a datagram socket on which to send. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		printf("[MCASTSENDER] [ERR] opening datagram socket");
		exit(1);
	}
	printf("\n[MCASTSENDER] created socket successfully\n");

	/* Disable loopback so you do not receive your own datagrams. */
	char loopch = 0;
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0) {
		printf("\n[MCASTSENDER] [ERR] Failed setting IP_MULTICAST_LOOP:");
		goto errout_with_socket;
	}
	printf("\n[MCASTSENDER] setsockopt MULTICAST_LOOP success\n");

	/* initialize socket information */
	memset((char *)&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr(g_app_target_addr);
	groupSock.sin_port = htons(g_app_target_port);

	/*
	 * Set local interface for outbound multicast datagrams.
	 * The IP address specified must be associated with a local,
	 * multicast-capable interface.
	 */
	localInterface.s_addr = inet_addr(g_app_netif_addr);
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
		printf("\n[MCASTSENDER] [ERR] Failed setting local interface");
		goto errout_with_socket;
	}
	printf("\n[MCASTSENDER] setsockopt IP_MULTICAST_IF success\n");

	/*
	 * Send a message to the multicast group specified by the
	 * groupSock sockaddr structure.
	 */
	datalen = strlen(databuf);
	printf("\n[MCASTSENDER] datalen = %d \n", datalen);

	for (i = 1; i <= num_packets; i++) {
		printf("\n[MCASTSENDER] sending mcast message (%s) length (%d) number (%d)\n", databuf, datalen, i);
		ret = sendto(sd, databuf, datalen, 0, (struct sockaddr *)&groupSock, addrlen);
		if (ret < 0) {
			printf("\n[MCASTSENDER] [ERR] sending datagram message");
		}
		sleep(1);
	}

	printf("\n[MCASTSENDER] Terminate multicast sender after sending sufficient messages (%d)\n", num_packets);
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

	/* Create a datagram socket on which to receive. */
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

	/* initialize socket information and bind the socket */
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
	group.imr_interface.s_addr = inet_addr(g_app_netif_addr);
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
		printf("\n[MCASTSERV] ERR: adding multicast group\n");
		goto errout_with_socket;
	}
	printf("\n[MCASTSERV] join multicast success sucess success success success success success success\n");

	/* Read from the socket. */
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
				printf("\n[MCASTSERV] - Received Msg # %d] read (%s) (%d) bytes, Terminating mcast serv as received sufficient packets for testing\n", count, databuf, ret);
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

void udp_client_thread(int num_packets, int ipver)
{
	struct sockaddr_in cliaddr;

	struct addrinfo *cliaddr6;

	int fd;
	int count = 1;
	int nbytes = 0;
	socklen_t addrlen;
	char sbuf[BUF_SIZE];
	char rbuf[BUF_SIZE];

	if (ipver == 1) {

		char addr6_str[50];
		char port_str[10];
		struct addrinfo hints;
		struct addrinfo *rp;

		memset(addr6_str, 0, 50);
		snprintf(addr6_str, 50, "%s", g_app_target_addr);
		memset(port_str, 0, 10);
		snprintf(port_str, 10, "%d", g_app_target_port);

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET6;
		hints.ai_socktype = SOCK_DGRAM;
		inet_pton(AF_INET6, addr6_str, &hints.ai_addr);

		if (getaddrinfo(addr6_str, port_str, &hints, &cliaddr6) != 0) {
			printf("[UDPCLI%s] getaddrinfo err: %d\n", NETTEST_IPV(ipver), errno);
			exit(1);
		}

		for (rp = cliaddr6; rp != NULL; rp = rp->ai_next) {
			memset(addr6_str, 0, 50);
			inet_ntop(AF_INET6, rp->ai_addr, addr6_str, 50);
			printf("[UDPCLI%s] addr: %s port:%s family: %d type: %d protocol: %d\n", NETTEST_IPV(ipver), addr6_str, port_str, rp->ai_family, rp->ai_socktype, rp->ai_protocol);

			fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if (fd < 0) {
				continue;
			}

			if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
				// connect success
				break;
			}
			close(fd);
		}
		if (rp == NULL) {
			printf("[UDPCLI%s] fail to open socket\n", NETTEST_IPV(ipver));
			freeaddrinfo(cliaddr6);
			exit(1);
		}
		freeaddrinfo(cliaddr6);
	} else {
		char addr6_str[40];
		fd = socket(AF_INET, SOCK_DGRAM, 0);

		cliaddr.sin_family = AF_INET;
		cliaddr.sin_port = htons(g_app_target_port);
		inet_pton(AF_INET, g_app_target_addr, &cliaddr.sin_addr);
		addrlen = sizeof(struct sockaddr_in);

		inet_ntop(AF_INET, &cliaddr.sin_addr, addr6_str, 40);
		printf("[UDPCLI%s] addr: %s port: %d\n", NETTEST_IPV(ipver), addr6_str, htons(cliaddr.sin_port));
	}

	memset(sbuf, 0, BUF_SIZE);
	snprintf(sbuf, sizeof("hello, UDP_VX"), "hello, UDP%s", NETTEST_IPV(ipver));
	memset(rbuf, 0, BUF_SIZE);

	while (1) {
		if (ipver == 1) {

			nbytes = write(fd, sbuf, BUF_SIZE);
			if (nbytes <= 0) {
				printf("[UDPCLI%s] write err: %d\n", NETTEST_IPV(ipver), errno);
				break;
			}

			nbytes = read(fd, rbuf, BUF_SIZE);

		} else {
			nbytes = sendto(fd, sbuf, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, addrlen);
			if (nbytes <= 0) {
				printf("[UDPCLI%s] sendto err: %d\n", NETTEST_IPV(ipver), errno);
				break;
			}

			nbytes = recvfrom(fd, rbuf, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &addrlen);
		}
		if (nbytes <= 0) {
			printf("[UDPCLI%s] %s err: %d\n", NETTEST_IPV(ipver), ipver ? "read" : "recvfrom", errno);
			break;
		} else {
			if (num_packets == 0) {
				/* infinite test */
				if (!(count % 1000)) {
					printf("[UDPCLI%s infinite test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, nbytes);
				}
			} else {
				printf("[UDPCLI%s test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, nbytes);
				if (count >= num_packets) {
					printf("[UDPCLI%s test done #%d]\n", NETTEST_IPV(ipver), count);
					break;
				}
			}
			count++;
		}
	}

	close(fd);
	return;
}

void udp_server_thread(int num_packets, int ipver)
{
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;

	struct sockaddr_in6 servaddr6;
	struct sockaddr_in6 cliaddr6;

	char msg[BUF_SIZE];
	int fd;
	int maxfd;
	int nbytes;
	int ret = 0;
	int count = 1;
	int have_received = 0;
	socklen_t addrlen;
	fd_set fds;
	fd_set rfds;
	fd_set sfds;

	if (ipver == 1) {
		/* create IPV6 udp socket */

		fd = socket(AF_INET6, SOCK_DGRAM, 0);
		if (fd < 0) {
			printf("[UDPSERV%s] socket err: %d\n", NETTEST_IPV(ipver), errno);
			exit(1);
		}

		/* initialize socket information */
		addrlen = sizeof(struct sockaddr_in6);
		memset(&servaddr6, 0, addrlen);

		servaddr6.sin6_family = AF_INET6;
		servaddr6.sin6_addr = in6addr_any;
		servaddr6.sin6_port = htons(g_app_target_port);

		/* bind to the in6addr_any */
		ret = bind(fd, (struct sockaddr *)&servaddr6, addrlen);
	} else {
		/* create IPV4 udp socket */
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		if (fd < 0) {
			printf("[UDPSERV%s] socket err: %d\n", NETTEST_IPV(ipver), errno);
			exit(1);
		}

		/* initialize socket information */
		addrlen = sizeof(struct sockaddr_in);
		memset(&servaddr, 0, addrlen);

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(g_app_target_port);

		/* bind to the INADDR_ANY */
		ret = bind(fd, (struct sockaddr *)&servaddr, addrlen);
	}
	if (ret < 0) {
		printf("[UDPSERV%s] bind err: %d\n", NETTEST_IPV(ipver), errno);
		goto errout_with_socket;
	}
	printf("[UDPSERV%s] waiting on port %d\n", NETTEST_IPV(ipver), g_app_target_port);

	/* fds is used to initialize rfds and sfds */
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	maxfd = fd + 1;
	while (1) {
		rfds = fds;
		FD_ZERO(&sfds);
		if (have_received) {
			/* sfds should be set when it has something to send */
			sfds = fds;
		}

		ret = select(maxfd, &rfds, &sfds, NULL, NULL);
		if (ret < 0) {
			printf("[UDPSERV%s] select err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}
		if (ret > 0) {
			if (FD_ISSET(fd, &sfds)) {
				if (ipver == 1) {

					nbytes = sendto(fd, msg, BUF_SIZE, 0, (struct sockaddr *)&cliaddr6, addrlen);

				} else {
					nbytes = sendto(fd, msg, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, addrlen);
				}
				if (nbytes <= 0) {
					printf("[UDPSERV%s] sendto err: %d\n", NETTEST_IPV(ipver), errno);
					break;
				} else {
					have_received--;
					if (num_packets == 0) {
						/* infinite test */
						if (!(count % 1000)) {
							printf("[UDPSERV%s infinite test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, msg, nbytes);
						}
					} else {
						printf("[UDPSERV%s test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, msg, nbytes);
						if (count >= num_packets) {
							printf("[UDPSERV%s test done #%d]\n", NETTEST_IPV(ipver), count);
							break;
						}
					}
					count++;
				}
			}
			if (FD_ISSET(fd, &rfds)) {
				if (ipver == 1) {

					nbytes = recvfrom(fd, msg, BUF_SIZE, 0, (struct sockaddr *)&cliaddr6, &addrlen);

				} else {
					nbytes = recvfrom(fd, msg, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &addrlen);
				}
				if (nbytes <= 0) {
					printf("[UDPSERV%s] recvfrom err: %d\n", NETTEST_IPV(ipver), errno);
					break;
				}
				have_received++;
			}
		} else {
			printf("[UDPSERV%s] select err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}
	}

	close(fd);
	return;

errout_with_socket:
	close(fd);
	exit(1);
}

void tcp_client_thread(int num_packets, int ipver)
{
	struct sockaddr_in cliaddr;

	struct addrinfo *cliaddr6;

	int ret;
	int fd;
	int count = 1;
	int nbytes = 0;
	socklen_t addrlen;
	char sbuf[BUF_SIZE];
	char rbuf[BUF_SIZE];

	if (ipver == 1) {

		char addr6_str[50];
		char port_str[10];
		struct addrinfo hints;
		struct addrinfo *rp;

		memset(addr6_str, 0, 50);
		snprintf(addr6_str, 50, "%s", g_app_target_addr);
		memset(port_str, 0, 10);
		snprintf(port_str, 10, "%d", g_app_target_port);

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET6;
		hints.ai_socktype = SOCK_STREAM;
		inet_pton(AF_INET6, addr6_str, &hints.ai_addr);

		ret = getaddrinfo(addr6_str, port_str, &hints, &cliaddr6);
		if (ret != 0) {
			printf("[TCPCLI%s] getaddrinfo err: %d\n", NETTEST_IPV(ipver), ret);
			exit(1);
		}

		for (rp = cliaddr6; rp != NULL; rp = rp->ai_next) {
			memset(addr6_str, 0, 50);
			inet_ntop(AF_INET6, rp->ai_addr, addr6_str, 50);
			printf("[TCPCLI%s] addr: %s port:%s family: %d type: %d protocol: %d\n", NETTEST_IPV(ipver), addr6_str, port_str, rp->ai_family, rp->ai_socktype, rp->ai_protocol);

			fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if (fd < 0) {
				continue;
			}

			if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
				/* success */
				break;
			}
			close(fd);
		}
		if (rp == NULL) {
			printf("[TCPCLI%s] connection fail\n", NETTEST_IPV(ipver));
			freeaddrinfo(cliaddr6);
			exit(1);
		}
		freeaddrinfo(cliaddr6);
	} else {
		char addr6_str[40];
		fd = socket(AF_INET, SOCK_STREAM, 0);

		cliaddr.sin_family = AF_INET;
		cliaddr.sin_port = htons(g_app_target_port);
		inet_pton(AF_INET, g_app_target_addr, &cliaddr.sin_addr);
		addrlen = sizeof(struct sockaddr_in);

		inet_ntop(AF_INET, &cliaddr.sin_addr, addr6_str, 40);
		printf("[TCPCLI%s] addr: %s port: %d\n", NETTEST_IPV(ipver), addr6_str, htons(cliaddr.sin_port));

		ret = connect(fd, (struct sockaddr *)&cliaddr, addrlen);
	}
	if (ret < 0) {
		printf("[TCPCLI%s] connect err: %d\n", NETTEST_IPV(ipver), errno);
		close(fd);
		exit(1);
	}

	memset(sbuf, 0, BUF_SIZE);
	snprintf(sbuf, sizeof("hello, TCP_VX"), "hello, TCP%s", NETTEST_IPV(ipver));
	memset(rbuf, 0, BUF_SIZE);

	while (1) {
		nbytes = send(fd, sbuf, BUF_SIZE, 0);
		if (nbytes <= 0) {
			printf("[TCPCLI%s] send err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		nbytes = recv(fd, rbuf, BUF_SIZE, 0);
		if (nbytes <= 0) {
			printf("[TCPCLI%s] recv err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		if (num_packets == 0) {
			if (!(count % 1000)) {
				printf("[TCPCLI%s infinite test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, nbytes);
			}
		} else {
			printf("[TCPCLI%s test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, nbytes);
			if (count >= num_packets) {
				printf("[TCPCLI%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
	}

	close(fd);
	return;
}

void tcp_server_thread(int num_packets, int ipver)
{
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;

	struct sockaddr_in6 servaddr6;
	struct sockaddr_in6 cliaddr6;

	char msg[BUF_SIZE];
	int listenfd = -1;
	int connfd = -1;
	int ret = 0;
	int nbytes = 0;
	int count = 1;
	socklen_t addrlen;

	if (ipver == 1) {
		/* create tcp ipv6 socket */

		listenfd = socket(AF_INET6, SOCK_STREAM, 0);
	} else {
		/* create tcp ipv4 socket */
		listenfd = socket(AF_INET, SOCK_STREAM, 0);
	}
	if (listenfd < 0) {
		printf("[TCPSERV%s] socket err: %d\n", NETTEST_IPV(ipver), errno);
		exit(1);
	}

	/* Enable SO_REUSEADDR to allow multiple instances */
	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("[TCPSERV%s] setsockopt err: %d\n", NETTEST_IPV(ipver), errno);
		goto errout_with_socket;
	}

	if (ipver == 1) {

		char addr6_str[40];

		/* initialize ipv6 socket information */
		addrlen = sizeof(struct sockaddr_in6);
		memset(&servaddr6, 0, addrlen);
		servaddr6.sin6_family = AF_INET6;
		servaddr6.sin6_addr = in6addr_any;
		servaddr6.sin6_port = htons(g_app_target_port);

		inet_ntop(AF_INET6, (void *)&servaddr6.sin6_addr, addr6_str, 40);
		printf("[TCPSERV%s] addr6_str: %s, port: %d\n", NETTEST_IPV(ipver), addr6_str, htons(servaddr6.sin6_port));

		/* bind to in6addr_any */
		ret = bind(listenfd, (struct sockaddr *)&servaddr6, addrlen);

	} else {
		char addr_str[40];

		/* initialize ipv4 socket information */
		addrlen = sizeof(struct sockaddr_in);
		memset(&servaddr, 0, addrlen);
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(g_app_target_port);

		inet_ntop(AF_INET, (void *)&servaddr.sin_addr, addr_str, 40);
		printf("[TCP_SERV%s] addr_str: %s, port: %d\n", NETTEST_IPV(ipver), addr_str, htons(servaddr.sin_port));

		/* bind to in6addr_any */
		ret = bind(listenfd, (struct sockaddr *)&servaddr, addrlen);
	}
	if (ret < 0) {
		printf("[TCPSERV%s] bind err: %d\n", NETTEST_IPV(ipver), errno);
		goto errout_with_socket;
	}

	/* start listening socket fd */
	ret = listen(listenfd, 1024);
	if (ret < 0) {
		printf("[TCPSERV%s] listen err: %d\n", NETTEST_IPV(ipver), errno);
		goto errout_with_socket;
	}

	if (ipver == 1) {

		/* accept ipv6 socket */
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr6, &addrlen);

	} else {
		/* accept ipv4 socket */
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &addrlen);
	}
	if (connfd < 0) {
		printf("[TCPSERV%s] accept err: %d\n", NETTEST_IPV(ipver), errno);
		goto errout_with_socket;
	}

	while (1) {
		/* echo back loop */
		nbytes = recv(connfd, msg, BUF_SIZE, 0);
		if (nbytes <= 0) {
			printf("[TCPSERV%s] recv err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		nbytes = send(connfd, msg, BUF_SIZE, 0);
		if (nbytes <= 0) {
			printf("[TCPSERV%s] send err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		if (num_packets == 0) {
			/* Infinite echo back */
			if (!(count % 1000)) {
				printf("[TCPSERV%s infinite test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, msg, nbytes);
			}
		} else {
			printf("[TCPSERV%s test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, msg, nbytes);
			if (count >= num_packets) {
				printf("[TCPSERV%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
	}

	if (connfd > 0) {
		close(connfd);
	}
	close(listenfd);
	return;

errout_with_socket:
	close(listenfd);
	exit(1);
}

/* Sample App to test Transport Layer (TCP / UDP) / IP Multicast Functionality */
int main(int argc, char *argv[])
{
	int proto;
	int num_packets_to_process;

	if (argc < 3) {
		printf("\nUsage1: %s proto target_port option1\n", argv[0]);
		printf("\tUsage1 is for echoback SERVER\n");
		printf("Parameter:\n");
		printf("\tproto - 0(tcp ipv4 echoback), 1(tcp ipv6 echoback)\n");
		printf("\t        2(udp ipv4 echoback), 3(udp ipv6 echoback)\n");
		printf("\ttarget_port - port address to bind\n");
		printf("\toption1 - number of pakcets to test, default: %d\n", NUM_PACKETS);
		printf("Sample Command(tcp ipv4 echoback server): nettest 0 8888 1000(option1)\n\n");

		printf("Usage2: %s proto target_addr target_port option1\n", argv[0]);
		printf("\tUsage2 is for CLIENT that expects echoing back from the server\n");
		printf("Parameter:\n");
		printf("\tproto - 10(tcp ipv4 client), 11(tcp ipv6 client)\n");
		printf("\t        12(udp ipv4 client), 13(udp ipv6 client)\n");
		printf("\ttarget_addr - target IP address to send or connect\n");
		printf("\ttarget_port - target port address to send or connect\n");
		printf("\toption1 - number of packets to test, default: %d\n\n", NUM_PACKETS);
		printf("Sample Command(udp ipv6 client): nettest 13 fe80::dcc3:3084:cb9b:38ce%%wl1 8888 1000(option1)\n\n");

		return 0;
	}

	proto = atoi(argv[1]);
	num_packets_to_process = NUM_PACKETS;

	if (proto >= 0 && proto < 10) {
		g_app_target_port = atoi(argv[2]);

		if (argc >= 4) {
			num_packets_to_process = atoi(argv[3]);
		}
	}

	if (proto >= 10 && proto < 20) {
		g_app_target_addr = argv[2];
		g_app_target_port = atoi(argv[3]);

		if (argc >= 5) {
			num_packets_to_process = atoi(argv[4]);
		}
	}

	if (proto >= 20 && proto < 30) {
		g_app_target_addr = argv[2];
		g_app_target_port = atoi(argv[3]);

		if (argc >= 5) {
			num_packets_to_process = atoi(argv[4]);
		}

		if (argc >= 6) {
			g_app_netif_addr = argv[5];
		} else {
			g_app_netif_addr = INADDR_ANY;
		}
	}

	switch (proto) {
	case 0:
		/* IPV4 TCP echo back server test */
		tcp_server_thread(num_packets_to_process, 0);
		break;
	case 1:
		/* IPV6 TCP echo back server test */
		tcp_server_thread(num_packets_to_process, 1);
		break;
	case 2:
		/* IPV4 UDP echo back server test */
		udp_server_thread(num_packets_to_process, 0);
		break;
	case 3:
		/* IPV6 UDP echo back server test */
		udp_server_thread(num_packets_to_process, 1);
		break;

	case 10:
		/* IPV4 TCP client test */
		tcp_client_thread(num_packets_to_process, 0);
		break;
	case 11:
		/* IPV6 TCP client test */
		tcp_client_thread(num_packets_to_process, 1);
		break;
	case 12:
		/* IPV4 UDP client test */
		udp_client_thread(num_packets_to_process, 0);
		break;
	case 13:
		/* IPV6 UDP client test */
		udp_client_thread(num_packets_to_process, 1);
		break;

	case 20:
		/* ipmulticast send */
		ipmcast_sender_thread(num_packets_to_process);
		break;
	case 21:
		/* ipmulticast receive */
		ipmcast_receiver_thread(num_packets_to_process);
		break;
	default:
		printf("[NETTEST APP] Invalid proto type\n");
		break;
	}
	printf("\nExiting nettest_main thread, job finished\n");
	return 0;
}
