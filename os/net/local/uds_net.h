/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef __NET_LOCAL_UDS_NET_H
#define __NET_LOCAL_UDS_NET_H

#include <tinyara/net/net.h>

void uds_net_initlist(FAR struct socketlist *list);
void uds_net_releaselist(FAR struct socketlist *list);
int uds_checksd(int fd, int oflags);

int uds_socket(int domain, int type, int protocol);
int uds_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int uds_connect(int sockfd, FAR const struct sockaddr *addr, socklen_t addrlen);
int uds_listen(int sockfd, int backlog);
int uds_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t uds_send(int sockfd, FAR const void *buf, size_t len, int flags);
ssize_t uds_sendto(int sockfd, FAR const void *buf, size_t len, int flags, FAR const struct sockaddr *to, socklen_t tolen);
ssize_t uds_recv(int sockfd, FAR void *buf, size_t len, int flags);
ssize_t uds_recvfrom(int sockfd, FAR void *buf, size_t len, int flags, FAR struct sockaddr *from, FAR socklen_t *fromlen);
int uds_getsockname(int sockfd, FAR struct sockaddr *addr, FAR socklen_t *addrlen);
int uds_getpeername(int sockfd, FAR struct sockaddr *addr, FAR socklen_t *addrlen);

#endif /* __NET_LOCAL_UDS_NET_H */
