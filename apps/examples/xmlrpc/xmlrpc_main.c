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
 * apps/examples/xmlrpc/xmlrpc_main.c
 *
 *   Copyright (C) 2012 Max Holtzberg. All rights reserved.
 *   Author: Max Holtzberg <mh@uvc.de>
 *
 * Based on the embeddable lightweight XML-RPC server code discussed
 * in the article at: http://www.drdobbs.com/web-development/\
 *    an-embeddable-lightweight-xml-rpc-server/184405364
 *
 *  Copyright (c) 2002 Cogito LLC.  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or
 *  without modification, is hereby granted without fee provided
 *  that the following conditions are met:
 *
 *    1.  Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the
 *        following disclaimer.
 *    2.  Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the
 *        following disclaimer in the documentation and/or other
 *        materials provided with the distribution.
 *    3.  Neither the name of Cogito LLC nor the names of its
 *        contributors may be used to endorse or promote products
 *        derived from this software without specific prior
 *        written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COGITO LLC AND CONTRIBUTERS 'AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COGITO LLC
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARAY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/

/*
 *  Lightweight Embedded XML-RPC Server main
 *
 *  mtj@cogitollc.com
 *
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <debug.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <netinet/in.h>

#include <netutils/netlib.h>
#include <protocols/xmlrpc.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char *notimplemented = { "HTTP/1.1 501 Not Implemented\n\n" };
static const char *separator = { "\015\012\015\012" };
#define XMLRPC_BUFFERSIZE 1024
#define XMLRPC_STRINGSIZE 64


/****************************************************************************
 * External Function Prototypes
 ****************************************************************************/

extern void calls_register(void);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: xmlrpc_findbody
 *
 * Description:
 *   Find the message body of an HTTP Request Message
 *
 ****************************************************************************/

static char *xmlrpc_findbody(char *buf)
{
	char *temp;

	temp = strstr(buf, separator);

	if (temp == NULL) {
		return NULL;
	} else {
		return temp + 4;
	}
}

/****************************************************************************
 * Name: xmlrpc_getheader
 *
 * Description:
 *   Find the HTTP header and return it's value.
 *
 ****************************************************************************/

static int xmlrpc_getheader(char *buffer, char *header, char *value, int size)
{
	char *temp;
	int i = 0;

	temp = strstr(buffer, header);
	if (temp) {
		/* Skip the header element */

		temp += strlen(header);

		/* Skip any white-space */

		while (*temp == ' ') {
			temp++;
		}

		/* Copy the rest to the value parameter */

		while ((*temp != ' ') && (*temp != '\n') && (i < size)) {
			value[i++] = *temp++;
		}

		value[i] = 0;
		return i;
	}

	return -1;
}

/****************************************************************************
 * Name: xmlrpc_handler
 *
 * Description:
 *    Parse and handle the current HTTP request message.
 *
 ****************************************************************************/

static void xmlrpc_handler(int fd)
{
	fd_set rfds;
	struct timeval tv;
	int ret, len, max = 0, loadlen = -1;
	char buffer[XMLRPC_BUFFERSIZE] = { 0 };
	char value[XMLRPC_STRINGSIZE + 1];
	char *temp;

	/* Read in the Request Header */

	do {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		ndbg("[%d] select...\n", fd);
		ret = select(fd + 1, &rfds, NULL, NULL, &tv);
		ndbg("[%d] data ready\n", fd);

		if (ret > 0) {
			if (FD_ISSET(fd, &rfds)) {
				len = recv(fd, &buffer[max], 1024, 0);
				ndbg("[%d] %d bytes received\n", fd, len);

				if (len > 0) {
					max += len;
					buffer[max] = 0;

					ret = xmlrpc_getheader(buffer, "Content-Length:", value,
										   XMLRPC_BUFFERSIZE);
					if (ret > 0) {
						loadlen = atoi(value);
					}
				} else {
					ret = -1;
					break;
				}
			}
		} else {
			/* Timeout... */

			ndbg("[%d] timeout\n", fd);
			ret = -1;
			break;
		}

		temp = strstr(buffer, separator);

		if (temp) {
			if (strlen(temp) - 4 == loadlen) {
				break;
			}
		}

	} while (1);

	/* Determine request */

	if (!strncmp(buffer, "POST", 4)) {
		temp = xmlrpc_findbody(buffer);
		xmlrpc_parse(fd, temp);
	} else {
		// send 501 error
		write(fd, notimplemented, strlen(notimplemented));
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: xmlrpc_main
 *
 * Description:
 *   The embedded HTTP server main
 *
 ****************************************************************************/
int xmlrpc_main(int argc, char *argv[])
{
	int listenfd, connfd, on = 1;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	/* Register RPC functions. */

	calls_register();

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	bzero((void *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(80);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 5);

	for (;;) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		if (connfd <= 0) {
			break;
		}
		printf("Connection accepted: %d\n", connfd);

		xmlrpc_handler(connfd);
		close(connfd);
		printf("[%d] connection closed\n", connfd);
	}

	close(listenfd);
	return (0);
}
