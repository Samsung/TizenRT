#ifndef __IOTIVITY_CONFIG_H__
#define __IOTIVITY_CONFIG_H__

//resource/c_common/octhread/src/posix/octhread.c
#define HAVE_PTHREAD_H

//resource/c_common/oic_platform/src/oic_platform.c
#define HAVE_NETDB_H

//resource/c_common/octhread/src/posix/octhread.c
#define HAVE_SYS_TIME_H

//resource/csdk/connectivity/inc/caadapterutils.h
#define HAVE_SYS_SOCKET_H

// resource/csdk/connectivity/src/ip_adapter/caipserver.c
#define HAVE_NET_IF_H


#if 1 /* from sys/sock_internal.h */

#include <sys/types.h>
#include <net/lwip/sockets.h>

struct cmsghdr {
        __kernel_size_t cmsg_len;       /* data byte count, including hdr */
        int cmsg_level;                         /* originating protocol */
        int cmsg_type;                          /* protocol-specific type */
};

#define CMSG_NXTHDR(mhdr, cmsg) cmsg_nxthdr((mhdr), (cmsg))

#define CMSG_ALIGN(len) (((len)+sizeof(long)-1) & ~(sizeof(long)-1))

#define CMSG_DATA(cmsg) ((void *)((char *)(cmsg) + CMSG_ALIGN(sizeof(struct cmsghdr))))
#define CMSG_SPACE(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#define CMSG_LEN(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))

#define __CMSG_FIRSTHDR(ctl, len) ((len) >= sizeof(struct cmsghdr) ? \
                                                                  (struct cmsghdr *)(ctl) : \
                                                                  (struct cmsghdr *)NULL)
#define CMSG_FIRSTHDR(msg)  __CMSG_FIRSTHDR((msg)->msg_control, (msg)->msg_controllen)

static inline struct cmsghdr *__cmsg_nxthdr(void *__ctl, __kernel_size_t __size, struct cmsghdr *__cmsg)
{
        struct cmsghdr *__ptr;

        __ptr = (struct cmsghdr *)(((unsigned char *)__cmsg) + CMSG_ALIGN(__cmsg->cmsg_len));
        if ((unsigned long)((char *)(__ptr + 1) - (char *)__ctl) > __size) {
                return (struct cmsghdr *)0;
        }

        return __ptr;
}

static inline struct cmsghdr *cmsg_nxthdr(struct msghdr *__msg, struct cmsghdr *__cmsg)
{
        return __cmsg_nxthdr(__msg->msg_control, __msg->msg_controllen, __cmsg);
}

#endif

#include "platform_features.h"

#endif /* __IOTIVITY_CONFIG_H__ */
