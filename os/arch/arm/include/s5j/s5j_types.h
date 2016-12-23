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
 * arch/arm/include/s5j/s5j_types.h
 *
 *   Copyright (C) 2007-2009, 2011-2012, 2014-2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_INCLUDE_S5J_S5J_TYPES_H
#define __ARCH_ARM_INCLUDE_S5J_S5J_TYPES_H

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <sys/types.h>

#ifndef NULL
#define NULL  (0)
#endif

#ifndef __int64
typedef long long __int64;
#endif

typedef float  f32;				// 32-bit Floating-Point Number
typedef double f64;				// 16-bit Floating-Point Number

#ifndef uint32
typedef unsigned int uint32;
#endif

#ifndef sint32
typedef int sint32;
#endif

#ifdef AARCH_64
typedef unsigned long long uregister;
#else
typedef unsigned int uregister;
#endif

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
#ifndef CONFIG_ENABLE_IOTIVITY
typedef unsigned int __u32;
#endif

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned long ADDRESS;

typedef u64 cycle_t;
typedef unsigned long long cycle_t;

typedef unsigned long phy_addr_t;

#define OK                  0
//#define ERROR               (-1)
//#define TIMEOUT             (-2)
#define REPEATER_NOT_READY  (-3)
#define ILLEGAL_DEVICE      (-4)
//#define FALSE               (0)
//#define TRUE                (1)
#define PASS      (1)
#define FAIL      (0)
#define SEC       (0)
#define NSEC      (1)

#define ENABLE    (1)
#define DISABLE   (0)

#define MHz   1000000

#ifndef __packed
#define __packed    __attribute__((__packed__))
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
	do {\
		const typeof(((type *)0)->member)*__mptr = (ptr);\
		(type *)((char *)__mptr - offsetof(type, member));\
	} while (0)
#endif

#define udelay(usec)    up_udelay(usec)
#define mdelay(msec)    up_mdelay(msec)

#define __irq

#define CONFIG_STACK_SIZE 0x800
#define CONFIG_HEAP_SIZE  0x40000
#define CONFIG_FIN_HZ     26000000

#define EPERM   1				/* Not super-user */
#define ENOENT  2				/* No such file or directory */
#define ESRCH   3				/* No such process */
#define EINTR   4				/* Interrupted system call */
#define EIO     5				/* I/O error */
#define ENXIO   6				/* No such device or address */
#define E2BIG   7				/* Arg list too long */
#define ENOEXEC 8				/* Exec format error */
#define EBADF   9				/* Bad file number */
#define ECHILD  10				/* No children */
#define EAGAIN  11				/* No more processes */
#define ENOMEM  12				/* Not enough core */
#define EACCES  13				/* Permission denied */
#define EFAULT  14				/* Bad address */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOTBLK 15				/* Block device required */
#endif
#define EBUSY   16				/* Mount device busy */
#define EEXIST  17				/* File exists */
#define EXDEV   18				/* Cross-device link */
#define ENODEV  19				/* No such device */
#define ENOTDIR 20				/* Not a directory */
#define EISDIR  21				/* Is a directory */
#define EINVAL  22				/* Invalid argument */
#define ENFILE  23				/* Too many open files in system */
#define EMFILE  24				/* Too many open files */
#define ENOTTY  25				/* Not a typewriter */
#define ETXTBSY 26				/* Text file busy */
#define EFBIG   27				/* File too large */
#define ENOSPC  28				/* No space left on device */
#define ESPIPE  29				/* Illegal seek */
#define EROFS   30				/* Read only file system */
#define EMLINK  31				/* Too many links */
#define EPIPE   32				/* Broken pipe */
#define EDOM    33				/* Math arg out of domain of func */
#define ERANGE  34				/* Math result not representable */
#define ENOMSG  35				/* No message of desired type */
#define EIDRM   36				/* Identifier removed */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ECHRNG  37				/* Channel number out of range */
#define EL2NSYNC 38				/* Level 2 not synchronized */
#define EL3HLT  39				/* Level 3 halted */
#define EL3RST  40				/* Level 3 reset */
#define ELNRNG  41				/* Link number out of range */
#define EUNATCH 42				/* Protocol driver not attached */
#define ENOCSI  43				/* No CSI structure available */
#define EL2HLT  44				/* Level 2 halted */
#endif
#define EDEADLK 45				/* Deadlock condition */
#define ENOLCK  46				/* No record locks available */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EBADE   50				/* Invalid exchange */
#define EBADR   51				/* Invalid request descriptor */
#define EXFULL  52				/* Exchange full */
#define ENOANO  53				/* No anode */
#define EBADRQC 54				/* Invalid request code */
#define EBADSLT 55				/* Invalid slot */
#define EDEADLOCK 56			/* File locking deadlock error */
#define EBFONT  57				/* Bad font file fmt */
#endif
#define ENOSTR  60				/* Device not a stream */
#define ENODATA 61				/* No data (for no delay io) */
#define ETIME   62				/* Timer expired */
#define ENOSR   63				/* Out of streams resources */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENONET  64				/* Machine is not on the network */
#define ENOPKG  65				/* Package not installed */
#define EREMOTE 66				/* The object is remote */
#endif
#define ENOLINK 67				/* The link has been severed */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EADV    68				/* Advertise error */
#define ESRMNT  69				/* Srmount error */
#define ECOMM   70				/* Communication error on send */
#endif
#define EPROTO  71				/* Protocol error */
#define EMULTIHOP 74			/* Multihop attempted */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ELBIN   75				/* Inode is remote (not really error) */
#define EDOTDOT 76				/* Cross mount point (not really error) */
#endif
#define EBADMSG 77				/* Trying to read unreadable message */
#define EFTYPE  79				/* Inappropriate file type or format */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOTUNIQ 80				/* Given log. name not unique */
#define EBADFD  81				/* f.d. invalid for this operation */
#define EREMCHG 82				/* Remote address changed */
#define ELIBACC 83				/* Can't access a needed shared lib */
#define ELIBBAD 84				/* Accessing a corrupted shared lib */
#define ELIBSCN 85				/* .lib section in a.out corrupted */
#define ELIBMAX 86				/* Attempting to link in too many libs */
#define ELIBEXEC 87				/* Attempting to exec a shared library */
#endif
#define ENOSYS  88				/* Function not implemented */
#ifdef __CYGWIN__
#define ENMFILE 89				/* No more files */
#endif
#define ENOTEMPTY 90			/* Directory not empty */
#define ENAMETOOLONG 91			/* File or path name too long */
#define ELOOP   92				/* Too many symbolic links */
#define EOPNOTSUPP 95			/* Operation not supported on transport endpoint */
#define EPFNOSUPPORT 96			/* Protocol family not supported */
#define ECONNRESET 104			/* Connection reset by peer */
#define ENOBUFS 105				/* No buffer space available */
#define EAFNOSUPPORT 106		/* Address family not supported by protocol family */
#define EPROTOTYPE 107			/* Protocol wrong type for socket */
#define ENOTSOCK 108			/* Socket operation on non-socket */
#define ENOPROTOOPT 109			/* Protocol not available */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESHUTDOWN 110			/* Can't send after socket shutdown */
#endif
#define ECONNREFUSED 111		/* Connection refused */
#define EADDRINUSE 112			/* Address already in use */
#define ECONNABORTED 113		/* Connection aborted */
#define ENETUNREACH 114			/* Network is unreachable */
#define ENETDOWN 115			/* Network interface is not configured */
#define ETIMEDOUT 116			/* Connection timed out */
#define EHOSTDOWN 117			/* Host is down */
#define EHOSTUNREACH 118		/* Host is unreachable */
#define EINPROGRESS 119			/* Connection already in progress */
#define EALREADY 120			/* Socket already connected */
#define EDESTADDRREQ 121		/* Destination address required */
#define EMSGSIZE 122			/* Message too long */
#define EPROTONOSUPPORT 123		/* Unknown protocol */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESOCKTNOSUPPORT 124		/* Socket type not supported */
#endif
#define EADDRNOTAVAIL 125		/* Address not available */
#define ENETRESET 126
#define EISCONN 127				/* Socket is already connected */
#define ENOTCONN 128			/* Socket is not connected */
#define ETOOMANYREFS 129
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EPROCLIM 130
#define EUSERS 131
#endif
#define EDQUOT 132
#define ESTALE 133
#define ENOTSUP 134				/* Not supported */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOMEDIUM 135			/* No medium (in tape drive) */
#endif
#ifdef __CYGWIN__
#define ENOSHARE 136			/* No such host or network path */
#define ECASECLASH 137			/* Filename exists with different case */
#endif
#define EILSEQ 138
#define EOVERFLOW 139			/* Value too large for defined data type */
#define ECANCELED 140			/* Operation canceled */
#define ENOTRECOVERABLE 141		/* State not recoverable */
#define EOWNERDEAD 142			/* Previous owner died */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESTRPIPE 143			/* Streams pipe error */
#endif
#define EWOULDBLOCK EAGAIN		/* Operation would block */

#define __ELASTERROR 2000		/* Users can add values starting here */

#define Assert(b) ({if (!(b)) {printf("\n %s(line %d)\n", __FILE__, __LINE__); do {} while (1); } })

#define __iomem

#define __raw_writel(d, b)  (*(volatile unsigned int *)(b) = (d))
#define __raw_readl(b)      (*(volatile unsigned int *)(b))

#define writel(v, c)  (*(volatile unsigned int *)(c) = (v))
#define readl(c)      (*(volatile unsigned int *)(c))

#define Outp32(addr, data)  (*(volatile u32 *)(addr) = (data))
#define Outp16(addr, data)  (*(volatile u16 *)(addr) = (data))
#define Outp8(addr, data)   (*(volatile u8 *)(addr) = (data))
#define Inp32(addr)   (*(volatile u32 *)(addr))
#define Inp16(addr)   (*(volatile u16 *)(addr))
#define Inp8(addr)    (*(volatile u8 *)(addr))

#define SetBits(uAddr, uBaseBit, uMaskValue, uSetValue) \
	Outp32(uAddr, (Inp32(uAddr) & ~((uMaskValue) << (uBaseBit))) | (((uMaskValue) & (uSetValue)) << (uBaseBit)))

#define ClearBits(uAddr, uBaseBit, uMaskValue, uSetValue) \
	Outp32(uAddr, (Inp32(uAddr) & ~((uMaskValue) << (uBaseBit))) & ~(((uMaskValue) & (uSetValue)) << (uBaseBit)))

#define GetBits(uAddr, uBaseBit, uMaskValue) ((Inp32(uAddr) >> (uBaseBit)) & (uMaskValue))

#define SET_REG(reg, bitfield, val) \
	do {\
		int __temp = Inp32(reg);\
		__temp &= ~(bitfield);\
		__temp |= ((bitfield) & (val));\
		Outp32(reg, __temp);\
	} while (0)

#define HW_REG32(base, offset)  (*(volatile u32 *)((unsigned int)base + ((unsigned int)offset)))
#define HW_REG16(base, offset)  (*(volatile u16 *)((unsigned int)base + ((unsigned int)offset)))
#define HW_REG8(base, offset)   (*(volatile u8 *)((unsigned int)base + ((unsigned int)offset)))

#define cprintf printf

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#ifndef BUG
#define BUG() \
	do {\
		printf("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __FUNCTION__); \
	} while (0)
#define BUG_ON(condition) do { if (unlikely((condition) != 0)) BUG(); } while (0)
#endif							/* BUG */

#define setbit(addr, bit, value)  __raw_writel((__raw_readl(addr) & ~(1 << bit)) | ((value & 0x1) << bit), addr)

#endif
