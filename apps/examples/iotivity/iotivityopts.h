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
#ifndef __IOTIVITY_IOTIVITYOPTS_H__
#define __IOTIVITY_IOTIVITYOPTS_H__

/*******************************************************************************************
 * Included Files
 *******************************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>

#ifdef CONFIG_ENABLE_IOTIVITY

#define __TINYARA__ 1

#ifdef CONFIG_TB_LOG
#define TB_LOG 1
#endif
#ifdef CONFIG_TCP_ADAPTER
#define TCP_ADAPTER 1
#endif
#ifdef CONFIG_WITH_TCP
#define WITH_TCP 1
#endif
#ifdef CONFIG_WITH_DTLS
#define __WITH_DTLS__ 1
#endif
#ifdef CONFIG_WITH_X509
#define __WITH_X509__ 1
#endif
#ifdef CONFIG_WITH_SHA256
#define WITH_SHA256 1
#endif
#ifdef CONFIG_ROUTING_EP
#define ROUTING_EP 1
#endif


#define RTM_NEWLINK 16
#define NETLINK_ROUTE 0

struct sockaddr_nl
{
  __kernel_sa_family_t    nl_family;  /* AF_NETLINK   */
  unsigned short  nl_pad;     /* zero    */
  __u32 nl_pid;     /* port ID  */
  __u32 nl_groups;  /* multicast groups mask */
};


struct nlmsghdr
{
  __u32       nlmsg_len;  /* Length of message including header */
  __u16       nlmsg_type; /* Message content */
  __u16       nlmsg_flags;    /* Additional flags */
  __u32       nlmsg_seq;  /* Sequence number */
  __u32       nlmsg_pid;  /* Sending process port ID */
};

#define NLMSG_ALIGNTO   4U
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
#define NLMSG_HDRLEN     ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
#define NLMSG_NEXT(nlh,len)  ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
                  (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && \
               (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
               (nlh)->nlmsg_len <= (len))


#endif


#endif /* __IOTIVITY_IOTIVITYOPTS_H__ */
