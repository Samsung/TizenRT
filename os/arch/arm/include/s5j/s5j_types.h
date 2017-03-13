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
