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
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef LWIP_HDR_ARCH_H
#define LWIP_HDR_ARCH_H

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif

#include <net/lwip/arch/cc.h>

#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#define LWIP_RAND() ((u32_t)rand())

/** Platform specific diagnostic output.\n
 * Note the default implementation pulls in printf, which may
 * in turn pull in a lot of standard libary code. In resource-constrained
 * systems, this should be defined to something less resource-consuming.
 */
#ifndef LWIP_PLATFORM_DIAG
#define LWIP_PLATFORM_DIAG(x) printf x
#include <stdio.h>
#include <stdlib.h>
#endif

/** Platform specific assertion handling.\n
 * Note the default implementation pulls in printf, fflush and abort, which may
 * in turn pull in a lot of standard libary code. In resource-constrained
 * systems, this should be defined to something less resource-consuming.
 */
#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFORM_ASSERT(x) do { printf("Assertion \"%s\" failed at line %d in %s\n", \
											 x, __LINE__, __FILE__); fflush(NULL); abort(); } while (0)
#include <stdio.h>
#include <stdlib.h>
#endif

/** Define this to 1 in arch/cc.h of your port if you do not want to
 * include stddef.h header to get size_t. You need to typedef size_t
 * by yourself in this case.
 */
#ifndef LWIP_NO_STDDEF_H
#define LWIP_NO_STDDEF_H 0
#endif

#if !LWIP_NO_STDDEF_H
#include <stddef.h>				/* for size_t */
#endif

/** Define this to 1 in arch/cc.h of your port if your compiler does not provide
 * the stdint.h header. You need to typedef the generic types listed in
 * lwip/arch.h yourself in this case (u8_t, u16_t...).
 */
#ifndef LWIP_NO_STDINT_H
#define LWIP_NO_STDINT_H 0
#endif

/* Define generic types used in lwIP */
#if !LWIP_NO_STDINT_H
#include <stdint.h>
typedef uint8_t u8_t;
typedef int8_t s8_t;
typedef uint16_t u16_t;
typedef int16_t s16_t;
typedef uint32_t u32_t;
typedef int32_t s32_t;
typedef uintptr_t mem_ptr_t;
#endif

/** Define this to 1 in arch/cc.h of your port if your compiler does not provide
 * the inttypes.h header. You need to define the format strings listed in
 * lwip/arch.h yourself in this case (X8_F, U16_F...).
 */
#ifndef LWIP_NO_INTTYPES_H
#define LWIP_NO_INTTYPES_H 0
#endif

/* Define (sn)printf formatters for these lwIP types */
#if !LWIP_NO_INTTYPES_H
#include <inttypes.h>
#ifndef X8_F
#define X8_F  "02"
#endif
#ifndef U16_F
#define U16_F "hu"
#endif
#ifndef S16_F
#define S16_F "d"
#endif
#ifndef X16_F
#define X16_F "hx"
#endif
#ifndef U32_F
#define U32_F "u"
#endif
#ifndef S32_F
#define S32_F "d"
#endif
#ifndef X32_F
#define X32_F "x"
#endif
#ifndef SZT_F
#define SZT_F "uz"
#endif
#endif

/** Define this to 1 in arch/cc.h of your port if your compiler does not provide
 * the limits.h header. You need to define the type limits yourself in this case
 * (e.g. INT_MAX).
 */
#ifndef LWIP_NO_LIMITS_H
#define LWIP_NO_LIMITS_H 0
#endif

/* Include limits.h? */
#if !LWIP_NO_LIMITS_H
#include <limits.h>
#endif

/** C++ const_cast<target_type>(val) equivalent to remove constness from a value (GCC -Wcast-qual) */
#ifndef LWIP_CONST_CAST
#define LWIP_CONST_CAST(target_type, val) ((target_type)((ptrdiff_t)val))
#endif

/** Get rid of alignment cast warnings (GCC -Wcast-align) */
#ifndef LWIP_ALIGNMENT_CAST
#define LWIP_ALIGNMENT_CAST(target_type, val) LWIP_CONST_CAST(target_type, val)
#endif

/** Get rid of warnings related to pointer-to-numeric and vice-versa casts,
 * e.g. "conversion from 'u8_t' to 'void *' of greater size"
 */
#ifndef LWIP_PTR_NUMERIC_CAST
#define LWIP_PTR_NUMERIC_CAST(target_type, val) LWIP_CONST_CAST(target_type, val)
#endif

/** Allocates a memory buffer of specified size that is of sufficient size to align
 * its start address using LWIP_MEM_ALIGN.
 * You can declare your own version here e.g. to enforce alignment without adding
 * trailing padding bytes (see LWIP_MEM_ALIGN_BUFFER) or your own section placement
 * requirements.\n
 * e.g. if you use gcc and need 32 bit alignment:\n
 * \#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) u8_t variable_name[size] \_\_attribute\_\_((aligned(4)))\n
 * or more portable:\n
 * \#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) u32_t variable_name[(size + sizeof(u32_t) - 1) / sizeof(u32_t)]
 */
#ifndef LWIP_DECLARE_MEMORY_ALIGNED
#define LWIP_DECLARE_MEMORY_ALIGNED(variable_name, size) u8_t variable_name[LWIP_MEM_ALIGN_BUFFER(size)]
#endif

/** Calculate memory size for an aligned buffer - returns the next highest
 * multiple of MEM_ALIGNMENT (e.g. LWIP_MEM_ALIGN_SIZE(3) and
 * LWIP_MEM_ALIGN_SIZE(4) will both yield 4 for MEM_ALIGNMENT == 4).
 */
#ifndef LWIP_MEM_ALIGN_SIZE
#define LWIP_MEM_ALIGN_SIZE(size) (((size) + MEM_ALIGNMENT - 1U) & ~(MEM_ALIGNMENT-1U))
#endif

/** Calculate safe memory size for an aligned buffer when using an unaligned
 * type as storage. This includes a safety-margin on (MEM_ALIGNMENT - 1) at the
 * start (e.g. if buffer is u8_t[] and actual data will be u32_t*)
 */
#ifndef LWIP_MEM_ALIGN_BUFFER
#define LWIP_MEM_ALIGN_BUFFER(size) (((size) + MEM_ALIGNMENT - 1U))
#endif

/** Align a memory pointer to the alignment defined by MEM_ALIGNMENT
 * so that ADDR % MEM_ALIGNMENT == 0
 */
#ifndef LWIP_MEM_ALIGN
#define LWIP_MEM_ALIGN(addr) ((void *)(((mem_ptr_t)(addr) + MEM_ALIGNMENT - 1) & ~(mem_ptr_t)(MEM_ALIGNMENT-1)))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Packed structs support.
  * Placed BEFORE declaration of a packed struct.\n
  * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
  * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
  */
#ifndef PACK_STRUCT_BEGIN
#define PACK_STRUCT_BEGIN
#endif							/* PACK_STRUCT_BEGIN */

/** Packed structs support.
  * Placed AFTER declaration of a packed struct.\n
  * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
  * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
  */
#ifndef PACK_STRUCT_END
#define PACK_STRUCT_END
#endif							/* PACK_STRUCT_END */

/** Packed structs support.
  * Placed between end of declaration of a packed struct and trailing semicolon.\n
  * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
  * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
  */
#ifndef PACK_STRUCT_STRUCT
#if defined(__GNUC__) || defined(__clang__)
#define PACK_STRUCT_STRUCT __attribute__((packed))
#else
#define PACK_STRUCT_STRUCT
#endif
#endif							/* PACK_STRUCT_STRUCT */

/** Packed structs support.
  * Wraps u32_t and u16_t members.\n
  * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
  * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
  */
#ifndef PACK_STRUCT_FIELD
#define PACK_STRUCT_FIELD(x) x
#endif							/* PACK_STRUCT_FIELD */

/** Packed structs support.
  * Wraps u8_t members, where some compilers warn that packing is not necessary.\n
  * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
  * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
  */
#ifndef PACK_STRUCT_FLD_8
#define PACK_STRUCT_FLD_8(x) PACK_STRUCT_FIELD(x)
#endif							/* PACK_STRUCT_FLD_8 */

/** Packed structs support.
  * Wraps members that are packed structs themselves, where some compilers warn that packing is not necessary.\n
  * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
  * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
  */
#ifndef PACK_STRUCT_FLD_S
#define PACK_STRUCT_FLD_S(x) PACK_STRUCT_FIELD(x)
#endif							/* PACK_STRUCT_FLD_S */

/** Packed structs support using \#include files before and after struct to be packed.\n
 * The file included BEFORE the struct is "arch/bpstruct.h".\n
 * The file included AFTER the struct is "arch/epstruct.h".\n
 * This can be used to implement struct packing on MS Visual C compilers, see
 * the Win32 port in the lwIP contrib repository for reference.
 * For examples of packed struct declarations, see include/lwip/prot/ subfolder.\n
 * A port to GCC/clang is included in lwIP, if you use these compilers there is nothing to do here.
 */
#ifdef __DOXYGEN__
#define PACK_STRUCT_USE_INCLUDES
#endif

/** Eliminates compiler warning about unused arguments (GCC -Wextra -Wunused). */
#ifndef LWIP_UNUSED_ARG
#define LWIP_UNUSED_ARG(x) (void)x
#endif							/* LWIP_UNUSED_ARG */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif							/* LWIP_HDR_ARCH_H */
