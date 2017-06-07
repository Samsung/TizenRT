/*****************************************************************************
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
/****************************************************************************
 * examples/tcptest/netstresstest.c
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
//#include <arch/board/board.h>
#include <apps/netutils/netlib.h>

#include <sys/socket.h>


#if defined (CONFIG_BUILD_PROTECTED)
#include <tinyara/kmalloc.h>
#define nettest_alloc(a)  kmm_alloc(a)
#define nettest_free(a)   kmm_free(a)
#else
#include <stdlib.h>
#define nettest_alloc(a) malloc(a)
#define nettest_free(a)  free(a)
#endif


#define AF_INETX AF_INET
#define PF_INETX PF_INET

//#define printf nlldbg

#define NETSTRESSTEST_CMDTHREAD_STACKSIZE (4096)

/////////////////////////////
// TCP
/////////////////////////////
#define BUF_SIZE 256

/////////////////////////////
// UDP
/////////////////////////////
#define APP_MSG_SIZE 256


/////////////////////////////
// NETWORK
/////////////////////////////

/* IP Multicast */
#define MCAST_SERVER_PORT 5551
#define MCAST_CLIENT_PORT 5552
#define MCAST_GROUP "225.1.1.1"

/* UDP */
#define UDP_SERVER_PORT 5553
#define UDP_CLIENT_PORT 5554

/* TCP */
#define TCP_SERVER_PORT 5555
#define TCP_CLIENT_PORT 5556

#define NUM_PACKETS     30000
#define LOCAL_DEVICE "192.168.2.10"


/* Typedef for nettest command's callback
 * @retval 0 success
 **/
typedef int (*NETTEST_CMD_CALLBACK) (int argc, char **args);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static int num_packets = 0;
static uint32_t sleep_time = 0;


/****************************************************************************
* Public Data
****************************************************************************/

uint32_t total_data;

struct nettest_cmd_args_s {
        NETTEST_CMD_CALLBACK cb;
        char **args;
        int argc;
};


/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if 0
/*
* error - wrapper for perror
*/
static void error(char *msg)
{
        perror(msg);
        printf(msg);
        exit(0);
}
#endif

/////////////////////////////
// MCAST
/////////////////////////////

int ipmcast_client_thread_cb(int argc, char *argv[])
{

        /* ------------------------------------------------------------*/
        /*                                                             */
        /* Send Multicast Datagram code example.                       */
        /*                                                             */
        /* ------------------------------------------------------------*/
        int ret = 0;
        int i = 0;
        int datalen;
        struct in_addr        localInterface;
        struct sockaddr_in    groupSock;
        int                   sd;
        char *databuf = "Test Data: IP Multicast from TinyAra Node to Linux";

        char *localip_addr = argv[1];
        char *targetip_addr = argv[2];
        int target_port = atoi(argv[3]);

        socklen_t addrlen = sizeof(struct sockaddr_in);
        printf("\n[MCASTCLIENT] starting ipmcast_client with target address %s and target port %d\n",
               targetip_addr, target_port);
        /*
         * Create a datagram socket on which to send.
         */
        sd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sd < 0) {
                printf("[MCASTCLIENT] [ERR] opening datagram socket");
                return 0;
        }
        printf("\n[MCASTCLIENT] created socket successfully\n");

        /*
         * Disable loopback so you do not receive your own datagrams.
         */

        char loopch = 0;

        if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch,
                       sizeof(loopch)) < 0) {
                printf("\n[MCASTCLIENT] [ERR] Failed setting IP_MULTICAST_LOOP:");
                goto errout_with_socket;
        }
        printf("\n[MCASTCLIENT] setsockopt MULTICAST_LOOP success\n");

        /*
         * Initialize the group sockaddr structure with a
         * group address of 225.1.1.1 and port as specified.
         */
        memset((char *) &groupSock, 0, sizeof(groupSock));
        groupSock.sin_family = AF_INET;
        groupSock.sin_addr.s_addr = inet_addr(targetip_addr);
        groupSock.sin_port = htons(target_port);


        /*
         * Set local interface for outbound multicast datagrams.
         * The IP address specified must be associated with a local,
         * multicast-capable interface.
         */
        localInterface.s_addr = inet_addr(localip_addr);
        if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface,
                       sizeof(localInterface)) < 0) {
                printf("\n[MCASTCLIENT] [ERR] Failed setting local interface");
                goto errout_with_socket;
        }
        printf("\n[MCASTCLIENT] setsockopt IP_MULTICAST_IF success\n");

        /*
         * Send a message to the multicast group specified by the
         * groupSock sockaddr structure.
         */

        datalen = strlen(databuf);
        printf("\n [MCASTCLIENT] datalen = %d \n", datalen);

        for (i = 1; i <= num_packets; i++) {
                printf("\n[MCASTCLIENT] sending mcast message (%s) length (%d) number (%d)\n",
                       databuf, datalen, i);
                ret = sendto(sd, databuf, datalen, 0, (struct sockaddr *)&groupSock,
                             addrlen);
                if (ret < 0) {
                        printf("\n[MCASTCLIENT] [ERR] sending datagram message");
                }
                usleep(sleep_time);
        }

        printf("\n[MCASTCLIENT] Terminate ipmcast_client after sending sufficient messages (%d)\n",
               num_packets);

errout_with_socket:
        close(sd);
        return 0;
}

int ipmcast_server_thread_cb(int argc, char *argv[])
{
        int ret  = 0;
        int count = 0;
        int datalen;
        char databuf[256];
        int                   sd;
        struct sockaddr_in    localSock;
        struct ip_mreq  group;

        char *localip_addr = argv[1];
        char *targetip_addr = argv[2];
        int target_port = atoi(argv[3]);

        printf("\n[MCASTSERV] starting ipmcast_server with listner address %s and listner port %d\n",
               targetip_addr, target_port);
        /* ------------------------------------------------------------*/
        /*                                                             */
        /* Receive Multicast Datagram code example.                    */
        /*                                                             */
        /* ------------------------------------------------------------*/

        /*
         * Create a datagram socket on which to receive.
         */
        sd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sd < 0) {
                printf("\n[MCASTSERV] ERR : opening datagram socket\n");
                return 0;
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
        memset((char *) &localSock, 0, sizeof(localSock));
        localSock.sin_family = AF_INET;
        localSock.sin_port = htons(target_port);;
        localSock.sin_addr.s_addr  = INADDR_ANY;

        if (bind(sd, (struct sockaddr *)&localSock, sizeof(localSock))) {
                printf("\n[MCASTSERV] ERR: binding datagram socket\n");
                goto errout_with_socket;
        }

        printf("\n[MCASTSERV] bind socket success\n");
        /*
         * Join the multicast group 225.1.1.1 on the local LOCAL_DEVICE
         * interface.  Note that this IP_ADD_MEMBERSHIP option must be
         * called for each local interface over which the multicast
         * datagrams are to be received.
         */
        group.imr_multiaddr.s_addr = inet_addr(targetip_addr);
        group.imr_interface.s_addr = inet_addr(localip_addr);
        if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group,
                       sizeof(group)) < 0) {
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
                ret = recv(sd, databuf, datalen, 0);
                if (ret < 0) {
                        printf("\n[MCASTSERV] ERR: reading datagram message\n");
                        //goto errout_with_socket;
                }
                databuf[ret] = '\0';
                count++;
                if (count > num_packets) {
                        printf("\n[MCASTSERV] - Received Msg # %d] read (%s) (%d) bytes, Terminating MCLIENT as received sufficient packets for testing\n",
                               count, databuf, ret);
                        break;
                } else {
                        printf("\n[MCASTSERV] - Received Msg # %d] read (%s) (%d) bytes\n", count,
                               databuf,
                               ret);
                }
        }

        printf("\n[MCASTSERV] Terminate ipmcast_client after sending sufficient messages (%d)\n",
               num_packets);

errout_with_socket:
        close(sd);
        return 0;
}

/////////////////////////////
// TCP
/////////////////////////////

int tcp_client_thread_cb(int argc, char *argv[])
{
        struct sockaddr_in myaddr;
        int sockfd;
        int ret = 0;
        int i = 0;
        uint32_t sbuf_size = 0;
        uint32_t send_try = 0;
        char buf[BUF_SIZE];
        socklen_t addrlen;

        //char *localip_addr = argv[1];
        char *targetip_addr = argv[2];
        int target_port = atoi(argv[3]);

        printf("\n[TCPCLIENT] starting tcp_client with target address %s and target port %d\n",
               targetip_addr, target_port);

        sockfd = socket(PF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
                printf("\n[TCPCLIENT] TCP socket failure %d\n", errno);
                return 0;
        }

        /* Connect the socket to the server */
        myaddr.sin_family      = AF_INET;
        myaddr.sin_port        = HTONS(target_port);
        inet_pton(AF_INET, targetip_addr, &(myaddr.sin_addr));

        addrlen = sizeof(struct sockaddr_in);

        printf("\n[TCPCLIENT] Connecting...\n");
        if (connect( sockfd, (struct sockaddr *)&myaddr, addrlen) < 0) {
                printf("\n[TCPCLIENT] connect fail: %d\n", errno);
                goto errout_with_socket;
        }
        printf("\n[TCPCLIENT] Connected\n");

        /* Then send num_packets number of messages */

        sprintf(buf, "Test Data: TCP send from TinyAra Node to Linux");

        for (i = 1; i <= num_packets; i++) {
                sbuf_size = strlen(buf);
                printf("\n\n\n[TCPCLIENT] -->send msg # %d data (%s) nbytes (%d) \n", i, buf,
                       strlen(buf));

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

                        printf("\n[TCPCLIENT] socket error ret(%d) err(%d) EAGAIN(%d)\n", ret, errno,
                               EWOULDBLOCK);
                        break;
                }
                send_try = 0;
                printf("\n[TCPCLIENT] <--send\t%d bytes\n\n", ret);
                usleep(sleep_time);

        }

        printf("\n[TCPCLIENT] Terminating tcp_client after sending sufficient messages (%d)\n",
               num_packets);

errout_with_socket:
        close(sockfd);
        return 0;
}


int tcp_server_thread_cb(int argc, char *argv[])
{
        struct sockaddr_in servaddr, cliaddr;
        int listenfd = -1;
        int connfd = -1;
        socklen_t clilen;

        int ret = 0;
        int recv_len = 0;
        int nbytes = 0;
        int count = 0;

        char msg[BUF_SIZE];

        //char *localip_addr = argv[1];
        //char *targetip_addr = argv[2];
        int target_port = atoi(argv[3]);

        printf("\n[TCPSERV] starting tcp_server with listner address as INADDR_ANY %s and listner port %d\n",
               INADDR_ANY, target_port);

        listenfd = socket(PF_INET, SOCK_STREAM, 0);
        if (listenfd < 0) {
                printf("\n[TCPSERV] TCP socket failure %d\n", errno);
                return 0;
        }

        /*
        * Enable SO_REUSEADDR to allow multiple instances of this
        * application to receive copies of the multicast datagrams.
        */

        int reuse = 1;
        ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                         sizeof(reuse));
        if (ret < 0) {
                printf("\n[TCPSERV] ERR: setting SO_REUSEADDR\n");
                goto errout_with_socket;
        }
        printf("\n[TCPSERV] set reusable success\n");

        /* Connect the socket to the server */
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = PF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = HTONS(target_port);

        ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        if (ret < 0) {
                perror("\n[TCPSERV] bind fail\n");
                goto errout_with_socket;
        }

        printf("\n[TCPSERV] Listening... port %d\n", target_port);

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
                        // connection closed
                        printf("\n[TCPSERV] selectserver: socket hung up err\n");
                        break;
                }
                count++;
                if (count > num_packets) {
                        printf("\n[TCPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n Exiting tcp_server as received sufficient packets for testing\n",
                               count, msg, nbytes);
                        break;
                } else {
                        printf("\n[TCPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n",
                               count, msg, nbytes);
                }
        }
        if (connfd > 0) {
                close(connfd);
                printf("\n[TCPSERV] Closed connfd successfully \n");
        }

        printf("\n[TCPSERV] Terminating tcp_server after receiving sufficient messages (%d)\n",
               num_packets);

errout_with_socket:
        close(listenfd);
        printf("\n[TCPSERV] Closed listenfd successfully \n");
        return 0;
}

/////////////////////////////
// UDP
/////////////////////////////

int udp_client_thread_cb(int argc, char *argv[])
{
        int sockfd;
        int clilen;
        int i = 0;
        char buf[APP_MSG_SIZE];

        struct sockaddr_in serveraddr;

        //char *localip_addr = argv[1];
        char *targetip_addr = argv[2];
        int target_port = atoi(argv[3]);

        printf("\n[UDPCLIENT] starting udp_client with target address %s and target port %d\n",
               targetip_addr, target_port);

        memset(buf, 0, APP_MSG_SIZE);

        sprintf(buf, "Test Data: UDP send from TinyAra Node to Linux");

        clilen = sizeof(serveraddr);

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
                printf("\n[UDPCLIENT]socket create error\n");
                return 0;
        }
        printf("\n[UDPCLIENT] socket create\n");

        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr(targetip_addr);
        serveraddr.sin_port = htons(target_port);

        for (i = 1; i <= num_packets; i++) {
                printf("\n\n\n[UDPCLIENT] -->send msg # %d data (%s) nbytes (%d) \n", i, buf,
                       strlen(buf));
                sendto(sockfd, (void *)buf, strlen(buf), 0, (struct sockaddr *)&serveraddr,
                       clilen);
                printf("\n[UDPCLIENT] <--sent msg\n\n\n\n");
                usleep(sleep_time);
        }
        printf("\n[UDPCLIENT] Terminating udp_client after sending sufficient messages (%d)\n",
               num_packets);

        close(sockfd);
        return 0;
}


int udp_server_thread_cb(int argc, char *argv[])
{
        struct sockaddr_in servaddr;
        struct sockaddr_in cliaddr;
        int s;
        int nbytes;
        int count = 0;
        socklen_t addrlen = sizeof(struct sockaddr_in);
        char buf[BUF_SIZE];
        int ret = 0;
        //num_packets = 35000;

        //char *localip_addr = argv[1];
        //char *targetip_addr = argv[2];
        int target_port = atoi(argv[3]);

        printf("\n[UDPSERV] starting udp_server with listner address as INADDR_ANY %s and listner port %d\n",
               INADDR_ANY, target_port);

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
        servaddr.sin_port = htons(target_port);

        ret = bind(s, (struct sockaddr *)&servaddr, addrlen);
        if (ret < 0) {
                perror("\n[UDPSERV]bind fail\n");
                goto errout_with_socket;
        }
        printf("\n[UDPSERV] socket binded\n");
        printf("\n[UDPSERV] waiting on port %d\n", target_port);
        while (1) {
                //printf("\n[UDPSERV] Waiting request\n");
                nbytes = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *)&cliaddr,
                                  &addrlen);
                if (nbytes < 0) {
                        perror("\n[UDPSERV] recvfrom fail");
                        break;
                }
                if (nbytes == 0) {
                        printf("\n[UDPSERV] socket closed from remote\n");
                        break;
                }
                count++;
                if (count > num_packets) {
                        printf("\n[UDPSERV] - Received Msg # %d] Received Msg (%s) data size (%d), Exiting UDPSERV as received sufficient packets for testing\n",
                               count, buf, nbytes);
                        break;
                } else {
                        printf("\n[UDPSERV] - Received Msg # %d] Received Msg (%s) data size (%d)\n",
                               count, buf, nbytes);
                }

        }

        printf("\n[UDPSERV] Terminating udp_server after receiving sufficient messages (%d)\n",
               num_packets);

errout_with_socket:
        close(s);
        return 0;
}


/** @brief Duplicate args to be passed to pthread
 *  @ingroup netstresstest
 */
static struct nettest_cmd_args_s *netstresstest_dupargs(int argc, char *args[],
                NETTEST_CMD_CALLBACK cb)
{
        struct nettest_cmd_args_s *new_arg_s = NULL;
        int dup_idx, cancel_idx, dup_done = TRUE;

        new_arg_s = (struct nettest_cmd_args_s *)nettest_alloc(sizeof(
                                struct nettest_cmd_args_s));
        if (new_arg_s != NULL) {
                new_arg_s->argc = argc;
                new_arg_s->cb = cb;
                new_arg_s->args = (char **)nettest_alloc(argc * sizeof(char *));
                if (new_arg_s->args != NULL) {
                        for (dup_idx = 0; dup_idx < argc; dup_idx++) {
                                new_arg_s->args[dup_idx] = strdup(args[dup_idx]);
                                if (new_arg_s->args[dup_idx] == NULL) {
                                        for (cancel_idx = 0; cancel_idx < dup_idx; cancel_idx++) {
                                                nettest_free(new_arg_s->args[cancel_idx]);
                                        }
                                        nettest_free(new_arg_s->args);
                                        nettest_free(new_arg_s);
                                        dup_done = FALSE;
                                        break;
                                }
                        }
                } else {
                        nettest_free(new_arg_s);
                        dup_done = FALSE;
                }
        }

        if (dup_done == TRUE) {
                return new_arg_s;
        } else {
                return NULL;
        }
}

/** @brief Thread Entry point for commands to execute in a seperate thread
 *  @ingroup netstresstest
 */
static void *netstresstest_cmdthread_entry(void *arg)
{
        int args_idx;
        struct nettest_cmd_args_s *nettest_cmdthread_s;

        nettest_cmdthread_s = (struct nettest_cmd_args_s *)arg;

        /* excute a callback function */
        nettest_cmdthread_s->cb(nettest_cmdthread_s->argc, nettest_cmdthread_s->args);

        /* Free up the memory */
        for (args_idx = 0; args_idx < nettest_cmdthread_s->argc; args_idx++)
                nettest_free(nettest_cmdthread_s->args[args_idx]);
        nettest_free(nettest_cmdthread_s->args);
        nettest_free(nettest_cmdthread_s);

        return NULL;
}

/** @brief Launch a pthread to run tash cmd asynchronously
 *  @ingroup netstresstest
 */
static int netstresstest_launch_cmdthread(struct nettest_cmd_args_s *arg,
                const char *name)
{
        int ret = 0;
        pthread_attr_t attr;
        pthread_t tid;

        /* Initialize the attribute variable */
        pthread_attr_init(&attr);
        struct sched_param sparam;

        /* 1. set a priority */
        sparam.sched_priority = SCHED_PRIORITY_DEFAULT;
        (void)pthread_attr_setschedparam(&attr, &sparam);

        /* 2. set a stacksize */
        (void)pthread_attr_setstacksize(&attr, NETSTRESSTEST_CMDTHREAD_STACKSIZE);

        /* 3. create pthread with entry function */
        ret = pthread_create(&tid, &attr, &netstresstest_cmdthread_entry, (void *)arg);
        if (ret == OK) {
                pthread_setname_np(tid, name);
        }

        pthread_detach(tid);

        return ret;
}



/* Sample App to test Transport Layer (TCP / UDP) / IP Multicast Functionality */
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int netstresstest_main(int argc, char *argv[])
#endif
{
        nlldbg("\n[NETSTRESSTEST APP] Running netstresstest_main \n");

        if (argc < 4) {
                printf("\n\nUsage1: netstresstest localip_addr targetip_addr target_port [0 - to use predfined ports]\n\n");
                return 0;
        }

        num_packets = NUM_PACKETS;
        // pps - packet per second, default value 1
        uint32_t pps = 20;
        sleep_time = 1000000ul / pps;
        int port = atoi(argv[3]);
        struct nettest_cmd_args_s *nargs = NULL;

        printf("\n[NETSTRESSTEST APP] LocalIP: %s addr TargetIP addr : %s Target Port: %s\n",
               argv[1], argv[2], argv[3]);

        // Launch TCP Server thread
        nargs = NULL;
        if (port == 0)
                itoa(TCP_SERVER_PORT, argv[3], 10);
        nargs = netstresstest_dupargs(argc, argv, tcp_server_thread_cb);
        if (nargs != NULL) {
                if (netstresstest_launch_cmdthread(nargs, "nst_tcp_server")) {
                        fprintf(stderr, "\n [NETSTRESSTEST APP] error in tcp_server_thread launch \n");
                }
        }

        // Launch TCP Client thread
        nargs = NULL;
        if (port == 0)
                itoa(TCP_CLIENT_PORT, argv[3], 10);
        nargs = netstresstest_dupargs(argc, argv, tcp_client_thread_cb);
        if (nargs != NULL) {
                if (netstresstest_launch_cmdthread(nargs, "nst_tcp_client")) {
                        fprintf(stderr, "\n [NETSTRESSTEST APP] error in tcp_client_thread launch \n");
                }
        }

        // Launch UDP Server thread
        nargs = NULL;
        if (port == 0)
                itoa(UDP_SERVER_PORT, argv[3], 10);
        nargs = netstresstest_dupargs(argc, argv, udp_server_thread_cb);
        if (nargs != NULL) {
                if (netstresstest_launch_cmdthread(nargs, "nst_udp_server")) {
                        fprintf(stderr, "\n [NETSTRESSTEST APP] error in udp_server_thread launch \n");
                }
        }

        // Launch UDP Client thread
        nargs = NULL;
        if (port == 0)
                itoa(UDP_CLIENT_PORT, argv[3], 10);
        nargs = netstresstest_dupargs(argc, argv, udp_client_thread_cb);
        if (nargs != NULL) {
                if (netstresstest_launch_cmdthread(nargs, "nst_udp_client")) {
                        fprintf(stderr, "\n [NETSTRESSTEST APP] error in udp_client_thread launch \n");
                }
        }

        // Launch IPMCAST Server thread
        nargs = NULL;
        strcpy(argv[2], MCAST_GROUP);
        if (port == 0)
                itoa(MCAST_SERVER_PORT, argv[3], 10);
        nargs = netstresstest_dupargs(argc, argv, ipmcast_server_thread_cb);
        if (nargs != NULL) {
                if (netstresstest_launch_cmdthread(nargs, "nst_ipmcast_server")) {
                        fprintf(stderr,
                                "\n [NETSTRESSTEST APP] error in ipmcast_server_thread launch \n");
                }
        }

        // Launch IPMCAST Client thread
        nargs = NULL;
        strcpy(argv[2], MCAST_GROUP);
        if (port == 0)
                itoa(MCAST_CLIENT_PORT, argv[3], 10);
        nargs = netstresstest_dupargs(argc, argv, ipmcast_client_thread_cb);
        if (nargs != NULL) {
                if (netstresstest_launch_cmdthread(nargs, "nst_ipmcast_client")) {
                        fprintf(stderr,
                                "\n [NETSTRESSTEST APP] error in ipmcast_client_thread launch \n");
                }
        }

        printf("\nExiting netstresstest_main thread, job finished, launched all test threads successfully\n");
        return 0;
}


