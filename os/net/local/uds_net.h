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

#endif /* __NET_LOCAL_UDS_NET_H */
