#ifndef _TIZENRT_COMPAT_H_
#define _TIZENRT_COMPAT_H_

#include <sys/types.h>
#include <net/lwip/sockets.h>


/*************************************
   access ancillary data
 *************************************/
/*
 *  POSIX 1003.1g - ancillary data object information
 *  Ancillary data consits of a sequence of pairs of
 *  (cmsghdr, cmsg_data[])
 */

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



/*************************************
   fprintf macros
 *************************************/
#define PRIu32          "lu"
#define PRIuPTR         "u"


#endif /* _TIZENRT_COMPAT_H_ */
