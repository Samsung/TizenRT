/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "caifaddrs.h"
#include "oic_malloc.h"
#include "oic_string.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include "logger.h"
#define TAG "OIC_CA_IFADDRS"
#define VERIFY_NON_NULL(arg) { if (!arg) {OIC_LOG(ERROR, TAG, #arg " is NULL"); goto exit;} }

#define NETLINK_MESSAGE_LENGTH  (4096)
#define IFC_LABEL_LOOP          "lo"
#define IFC_ADDR_LOOP_IPV4      "127.0.0.1"
#define IFC_ADDR_LOOP_IPV6      "::1"

typedef struct {
    struct nlmsghdr     msgInfo;
    struct ifaddrmsg    ifaddrInfo;
} CANetlintReq_t;


static bool CASendNetlinkMessage(int netlinkFd, const void* data, size_t len)
{
    ssize_t sentByteCount = TEMP_FAILURE_RETRY(send(netlinkFd, data, len, 0));
    return (sentByteCount == (ssize_t)(len));
}

void CAFreeIfAddrs(struct ifaddrs *ifa)
{
    struct ifaddrs *cur = NULL;
    while (ifa)
    {
        cur = ifa;
        ifa = ifa->ifa_next;
        OICFree(cur->ifa_name);
        OICFree(cur->ifa_addr);
        OICFree(cur);
        cur = NULL;
    }
}

static struct ifaddrs *CAParsingAddr(struct nlmsghdr *recvMsg)
{
    struct ifaddrmsg *ifaddrmsgData = (struct ifaddrmsg*)NLMSG_DATA(recvMsg);
    if (ifaddrmsgData-> ifa_family != AF_INET && ifaddrmsgData-> ifa_family != AF_INET6)
    {
        return NULL;
    }

    struct rtattr *rtattrData = (struct rtattr*)IFA_RTA(ifaddrmsgData);
    int ifaddrmsgLen = IFA_PAYLOAD(recvMsg);

    struct ifaddrs *node = (struct ifaddrs *)OICCalloc(1, sizeof(struct ifaddrs));
    VERIFY_NON_NULL(node);

    char nameBuf[IFNAMSIZ] = { 0 };
    node->ifa_next = NULL;
    if_indextoname(ifaddrmsgData->ifa_index, nameBuf);
    node->ifa_name = (char *)OICCalloc(strlen(nameBuf)+1, sizeof(char));
    VERIFY_NON_NULL(node->ifa_name);

    OICStrcpy(node->ifa_name, strlen(nameBuf)+1, nameBuf);
    node->ifa_flags = ifaddrmsgData->ifa_flags;
    node->ifa_flags |= (IFF_UP|IFF_RUNNING);
    void *dest = NULL;
    struct sockaddr_storage* ss = NULL;

    for (; RTA_OK(rtattrData, ifaddrmsgLen); rtattrData = RTA_NEXT(rtattrData, ifaddrmsgLen))
    {
        switch (rtattrData->rta_type)
        {
            case IFA_ADDRESS:
                ss = (struct sockaddr_storage*)OICCalloc(1, sizeof(struct sockaddr_storage));
                VERIFY_NON_NULL(ss);

                ss->ss_family = ifaddrmsgData-> ifa_family;

                if (ifaddrmsgData-> ifa_family == AF_INET)
                {
                    dest = &((struct sockaddr_in*)ss)->sin_addr;
                    memcpy(dest, RTA_DATA(rtattrData), RTA_PAYLOAD(rtattrData));
                }
                else if (ifaddrmsgData-> ifa_family == AF_INET6)
                {
                    dest = &((struct sockaddr_in6*)ss)->sin6_addr;
                    memcpy(dest, RTA_DATA(rtattrData), RTA_PAYLOAD(rtattrData));
                }

                node->ifa_addr = (struct sockaddr*)ss;
                break;

            default :
                // do nothing
                break;
        }
    }

    return node;
exit:
    CAFreeIfAddrs(node);
    return NULL;
}

CAResult_t CAGetIfaddrsUsingNetlink(struct ifaddrs **ifap)
{
    if (!ifap)
    {
        OIC_LOG(ERROR, TAG, "netlink argument error");
        return CA_STATUS_INVALID_PARAM;
    }
    *ifap = NULL;

    int netlinkFd = socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE);
    CAResult_t state = CA_STATUS_FAILED;
    if (-1 == netlinkFd)
    {
        OIC_LOG_V(ERROR, TAG, "netlink socket failed: %s", strerror(errno));
        return CA_SOCKET_OPERATION_FAILED;
    }

    // send request to kernel
    CANetlintReq_t req;
    memset(&req, 0, sizeof(req));
    req.msgInfo.nlmsg_len = NLMSG_ALIGN(NLMSG_LENGTH(sizeof(req)));
    req.msgInfo.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.msgInfo.nlmsg_type = RTM_GETADDR;
    req.ifaddrInfo.ifa_family = AF_UNSPEC;
    req.ifaddrInfo.ifa_index = 0;

    if (!CASendNetlinkMessage(netlinkFd, &req, req.msgInfo.nlmsg_len))
    {
        OIC_LOG(ERROR, TAG, "netlink send failed");
        state = CA_SOCKET_OPERATION_FAILED;
        goto exit;
    }

    while (1)
    {
        char recvBuf[NETLINK_MESSAGE_LENGTH] = {0};
        struct nlmsghdr *recvMsg = NULL;
        struct ifaddrs *node = NULL;
        struct sockaddr_nl sa = { .nl_family = 0 };
        struct iovec iov = { .iov_base = recvBuf,
                         .iov_len = sizeof (recvBuf) };

        struct msghdr msg = { .msg_name = (void *)&sa,
                          .msg_namelen = sizeof (sa),
                          .msg_iov = &iov,
                          .msg_iovlen = 1 };

        ssize_t len = recvmsg(netlinkFd, &msg, 0);

        for (recvMsg = (struct nlmsghdr *)recvBuf; NLMSG_OK(recvMsg, len);
             recvMsg = NLMSG_NEXT(recvMsg, len))
        {
            switch (recvMsg->nlmsg_type)
            {
                case NLMSG_DONE:
                    OIC_LOG(DEBUG, TAG, "NLMSG_DONE");
                    state = CA_STATUS_OK;
                    goto exit;

                case NLMSG_ERROR:
                    OIC_LOG(ERROR, TAG, "NLMSG is invalid");
                    state = CA_SOCKET_OPERATION_FAILED;
                    goto exit;

                case RTM_NEWADDR:
                    node = CAParsingAddr(recvMsg);
                    state = CA_MEMORY_ALLOC_FAILED;
                    VERIFY_NON_NULL(node);
                    state = CA_STATUS_OK;

                    if (*ifap == NULL)
                    {
                        *ifap = node;
                    }
                    else
                    {
                        node->ifa_next = *ifap;
                        *ifap = node;
                    }
                    node = NULL;

                    break;

                case RTM_NEWLINK:
                default:
                    OIC_LOG(DEBUG, TAG, "ignore unknown NLMSG");
                    break;
            }
        }
    }

exit:
    // release all resources
    close(netlinkFd);
    if (state != CA_STATUS_OK)
    {
        CAFreeIfAddrs(*ifap);
    }

    return state;
}
