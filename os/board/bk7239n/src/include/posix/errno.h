/*
 * Amazon FreeRTOS POSIX V1.1.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file errno.h
 * @brief System error numbers.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 *
 * The values defined in this file may not be compatible with the strerror
 * function provided by this system.
 */

#ifndef _FREERTOS_POSIX_ERRNO_H_
#define _FREERTOS_POSIX_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_AGORA_IOT_SDK
extern int errno;
#endif

/* Undefine all errnos to avoid redefinition errors with system errnos. */
#undef EPERM
#undef ENOENT
#undef EBADF
#undef EAGAIN
#undef ENOMEM
#undef EEXIST
#undef EBUSY
#undef EINVAL
#undef ENOSPC
#undef ERANGE
#undef ENAMETOOLONG
#undef EDEADLK
#undef EOVERFLOW
#undef ENOSYS
#undef EMSGSIZE
#undef ENOTSUP
#undef ETIMEDOUT
#ifdef CONFIG_AGORA_IOT_SDK
#undef EWOULDBLOCK
#undef EINTR
#undef ENOBUFS
#undef EHOSTUNREACH
#undef EIO
#endif

/**
 * @name Definition of POSIX errnos.
 */
/**@{ */
#define EPERM           1   /**< Operation not permitted. */
#define ENOENT          2   /**< No such file or directory. */
#define EBADF           9   /**< Bad file descriptor. */
#define EAGAIN          11  /**< Resource unavailable, try again. */
#define ENOMEM          12  /**< Not enough space. */
#define EEXIST          17  /**< File exists. */
#define EBUSY           16  /**< Device or resource busy. */
#define EINVAL          22  /**< Invalid argument. */
#define ENOSPC          28  /**< No space left on device. */
#define ERANGE          34  /**< Result too large. */
#define ENAMETOOLONG    36  /**< File name too long. */
#define EDEADLK         45  /**< Resource deadlock would occur. */
#define EOVERFLOW       75  /**< Value too large to be stored in data type. */
#define ENOSYS          88  /**< Function not supported. */
#define EMSGSIZE        90  /**< Message too long. */
#define ENOTSUP         95  /**< Operation not supported. */
#ifdef CONFIG_AGORA_IOT_SDK
#define EOPNOTSUPP      95  /* Operation not supported on transport endpoint */
#endif
#define ETIMEDOUT       116 /**< Connection timed out. */
#ifdef CONFIG_AGORA_IOT_SDK
#define EWOULDBLOCK EAGAIN /* Operation would block */
#define EINTR 4		/* Interrupted system call */
#define  ENOBUFS        105  /* No buffer space available */
#define  EHOSTUNREACH   113  /* No route to host */
#define  EALREADY       114  /* Operation already in progress */
#define  EINPROGRESS    115  /* Operation now in progress */
#define  EADDRINUSE      98  /* Address already in use */
#define  EISCONN        106  /* Transport endpoint is already connected */
#define  ENOTCONN       107  /* Transport endpoint is not connected */
#define  ECONNABORTED   103  /* Software caused connection abort */
#define  ECONNRESET     104  /* Connection reset by peer */
#define  EIO              5  /* I/O error */
#define  ENFILE          23  /* File table overflow */
#define  EAFNOSUPPORT    97  /* Address family not supported by protocol */
#define  EADDRNOTAVAIL   99  /* Cannot assign requested address */
#define  ENODEV          19  /* No such device */
#define  ENOPROTOOPT     92  /* Protocol not available */
#define  EFAULT          14  /* Bad address */
#define  EPIPE           32  /* Broken pipe */
#endif
/**@} */

#ifndef CONFIG_AGORA_IOT_SDK
/**
 * @name System Variable
 *
 * @brief Define FreeRTOS+POSIX errno, if enabled.
 * Set configUSE_POSIX_ERRNO to enable, and clear to disable. See FreeRTOS.h.
 *
 * @{
 */
#if ( configUSE_POSIX_ERRNO == 1 )
    extern int FreeRTOS_errno;
    #define errno    FreeRTOS_errno
#endif
/**@} */
#endif /* CONFIG_AGORA_IOT_SDK */

#ifdef __cplusplus
}
#endif

#endif /* ifndef _FREERTOS_POSIX_ERRNO_H_ */
