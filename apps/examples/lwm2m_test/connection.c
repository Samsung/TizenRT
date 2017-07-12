/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Gregory Lemercier, Samsung Semiconductor - support for TCP/TLS
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Pascal Rieux - Please refer to git log
 *
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "connection.h"

// from commandline.c
void output_buffer(FILE *stream, uint8_t *buffer, int length, int indent);

int create_socket(coap_protocol_t protocol, const char *portStr, int addressFamily)
{
	int s = -1;
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = addressFamily;
	switch (protocol) {
	case COAP_TCP:
	case COAP_TCP_TLS:
		hints.ai_socktype = SOCK_STREAM;
		break;
	case COAP_UDP:
	case COAP_UDP_DTLS:
		hints.ai_socktype = SOCK_DGRAM;
		break;
	default:
		break;
	}
	hints.ai_flags = AI_PASSIVE;

	if (0 != getaddrinfo(NULL, portStr, &hints, &res)) {
		return -1;
	}

	for (p = res ; p != NULL; p = p->ai_next) {
		s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (s >= 0) {
			int reuse;
			if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
				close(s);
				s = -1;
				continue;
			}

			if (-1 == bind(s, p->ai_addr, p->ai_addrlen)) {
				close(s);
				s = -1;
				continue;
			}
			break;
		}
	}

	freeaddrinfo(res);

	return s;
}

connection_t *create_session(int sockfd, struct sockaddr_storage *caddr, socklen_t *caddrLen, coap_protocol_t proto, void *tls_context, void *tls_option)
{
	int ret;
	int buf[1] = {0, };
	int newsock = -1;
	struct timeval tv;
	connection_t *connP = NULL;

#ifdef WITH_MBEDTLS
	tls_session *session = NULL;
	tls_ctx *ctx = tls_context;
	tls_opt *opt = tls_option;
#endif

	switch (proto) {
	case COAP_TCP:
		if (listen(sockfd, TCP_LISTEN_BACKLOG) != 0) {
			return NULL;
		}
		newsock = accept(sockfd, (struct sockaddr *)caddr, caddrLen);
		break;
	case COAP_TCP_TLS:
#ifdef WITH_MBEDTLS
		if (listen(sockfd, TCP_LISTEN_BACKLOG) != 0) {
			return NULL;
		}
#endif
	case COAP_UDP_DTLS:
#ifdef WITH_MBEDTLS
		session = TLSSession(sockfd, ctx, opt);
		if (session == NULL) {
			printf("Error: Create Secure session\n");
			return NULL;
		}
		newsock = session->net.fd;
#endif
		break;
	case COAP_UDP:
		ret = recvfrom(sockfd, buf, sizeof(buf), MSG_PEEK, (struct sockaddr *)caddr, caddrLen);
		if (ret > 0) {
			newsock = sockfd;
		}
		break;
	default:
		break;
	}

	if (newsock < 0) {
		printf("create_session : Failed to accept, errno %d\n", errno);
	} else {
		connP = connection_new_incoming(NULL, newsock, (struct sockaddr *)caddr, *caddrLen);
#ifdef WITH_MBEDTLS
		connP->session = session;
#endif

		tv.tv_sec = 3;
		tv.tv_usec = 0;
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) {

		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv))) {
		}
	}
	return connP;
}

connection_t *connection_new_incoming(connection_t *connList,
									  int sock,
									  struct sockaddr *addr,
									  size_t addrLen)
{
	connection_t *connP;

	connP = (connection_t *)malloc(sizeof(connection_t));
	if (connP != NULL) {
		connP->sock = sock;
		memcpy(&(connP->addr), addr, addrLen);
		connP->addrLen = addrLen;
		connP->next = connList;
#ifdef WITH_MBEDTLS
		connP->session = NULL;
#endif
	}

	return connP;
}

connection_t *connection_create(coap_protocol_t protocol,
								connection_t *connList,
								int sock,
								char *host,
								char *port,
								int addressFamily)
{
	struct addrinfo hints;
	struct addrinfo *servinfo = NULL;
	struct addrinfo *p;
	struct sockaddr *sa = NULL;
	struct timeval tv;
	socklen_t sl = 0;
	connection_t *connP = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = addressFamily;

	switch (protocol) {
	case COAP_UDP:
	case COAP_UDP_DTLS:
		hints.ai_socktype = SOCK_DGRAM;
		break;
	case COAP_TCP:
	case COAP_TCP_TLS:
		hints.ai_socktype = SOCK_STREAM;
		break;
	default:
		break;
	}

	if (0 != getaddrinfo(host, port, &hints, &servinfo) || servinfo == NULL) {
		return NULL;
	}

	// TODO : How to support various addresses
	for (p = servinfo; p != NULL; p = p->ai_next) {
		sa = p->ai_addr;
#ifdef CONFIG_NET_LWIP
		sa->sa_len = p->ai_addrlen;
#endif
		sl = p->ai_addrlen;

		/* Set send and receive timeout values */
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
			printf("connect: setsockopt fail\n");
		}
		if (-1 == connect(sock, sa, sl)) {
			fprintf(stderr, "connection_create : failed to connect, errno %d\r\n", errno);
			return NULL;
		} else {
			break;
		}
	}
	connP = connection_new_incoming(connList, sock, sa, sl);

	if (NULL != servinfo) {
#ifdef CONFIG_NET_LWIP
		freeaddrinfo(servinfo);
#else
		if (servinfo->ai_addr) {
			free(servinfo->ai_addr);
		}
		if (servinfo->ai_canonname) {
			free(servinfo->ai_canonname);
		}
		free(servinfo);
#endif
	}

	return connP;
}

void connection_free(connection_t *connList)
{
	while (connList != NULL) {
		connection_t *nextP;

		nextP = connList->next;
#ifdef WITH_MBEDTLS
		if (connList->session) {
			if (TLSSession_free(connList->session)) {
				printf("Error: fail to free tls session\n");
			}
			connList->session = NULL;
		}
#endif
		free(connList);
		connList = nextP;
	}
}

int connection_send(connection_t *connP,
					uint8_t *buffer,
					size_t length,
					coap_protocol_t proto)
{
	int nbSent;
	size_t offset;

	char s[INET6_ADDRSTRLEN];
	in_port_t port = 0;

	s[0] = 0;

	if (AF_INET == connP->addr.sin6_family) {
		struct sockaddr_in *saddr = (struct sockaddr_in *)&connP->addr;
		inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
		port = saddr->sin_port;
	} else if (AF_INET6 == connP->addr.sin6_family) {
		struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&connP->addr;
		inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
		port = saddr->sin6_port;
	}

	printf("Sending %d bytes to [%s]:%hu\n", length, s, ntohs(port));
	output_buffer(stderr, buffer, length, 0);

	offset = 0;
	while (offset != length) {
		nbSent = 0;
		switch (proto) {
		case COAP_UDP_DTLS:
		case COAP_TCP_TLS:
#ifdef WITH_MBEDTLS
			nbSent = mbedtls_ssl_write(connP->session->ssl, buffer + offset, length - offset);
#endif
			break;
		case COAP_TCP:
			nbSent = send(connP->sock, buffer + offset, length - offset, 0);
			break;
		case COAP_UDP:
			nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
			break;
		default:
			break;
		}
		if (nbSent < 0) {
			printf("Send fail nbSent : %d , error: %s\n", nbSent, strerror(errno));
			return -1;
		}

		offset += nbSent;
	}
	return 0;
}

uint8_t lwm2m_buffer_send(void *sessionH,
						  uint8_t *buffer,
						  size_t length,
						  void *userdata,
						  coap_protocol_t proto)
{
	connection_t *connP = (connection_t *) sessionH;

	if (connP == NULL) {
		printf("#> failed sending %lu bytes, missing connection\n", length);
		return COAP_500_INTERNAL_SERVER_ERROR ;
	}

	if (-1 == connection_send(connP, buffer, length, proto)) {
		printf("#> failed sending %lu bytes\n", length);
		return COAP_500_INTERNAL_SERVER_ERROR ;
	}

	return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void *session1,
							void *session2,
							void *userData)
{
	return (session1 == session2);
}

int connection_read(coap_protocol_t protocol,
					connection_t *connP,
					int sock,
					uint8_t *buffer,
					size_t len,
					struct sockaddr_storage *from,
					socklen_t *fromLen)
{
	int numBytes = -1;

	switch (protocol) {
	case COAP_TCP_TLS:
	case COAP_UDP_DTLS:
#ifdef WITH_MBEDTLS
		numBytes = mbedtls_ssl_read(connP->session->ssl, buffer, len);
#endif
		break;
	case COAP_UDP:
		numBytes = recvfrom(sock, buffer, len, 0, (struct sockaddr *)from, fromLen);
		break;
	case COAP_TCP:
		numBytes = recv(sock, buffer, len, 0);
		break;
	default:
		printf("connection_read : unsupported protocol type : %d\n", protocol);
		break;
	}

	return numBytes;
}
