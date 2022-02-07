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
#ifndef LWIP_HDR_DEBUG_H
#define LWIP_HDR_DEBUG_H

#include "lwip/arch.h"
#include "lwip/opt.h"
/** lower two bits indicate debug level
 * - 0 all
 * - 1 warning
 * - 2 serious
 * - 3 severe
 */
/** Debug level: ALL messages*/
#define LWIP_DBG_LEVEL_ALL     0x00
/** Debug level: Warnings. bad checksums, dropped packets, ... */
#define LWIP_DBG_LEVEL_WARNING 0x01
/** Debug level: Serious. memory allocation failures, ... */
#define LWIP_DBG_LEVEL_SERIOUS 0x02
/** Debug level: Severe */
#define LWIP_DBG_LEVEL_SEVERE  0x03
/**
 * @}
 */

#define LWIP_DBG_MASK_LEVEL    0x03
/* compatibility define only */
#define LWIP_DBG_LEVEL_OFF     LWIP_DBG_LEVEL_ALL

/** flag for LWIP_DEBUGF to enable that debug message */
#define LWIP_DBG_ON            0x80U
/** flag for LWIP_DEBUGF to disable that debug message */
#define LWIP_DBG_OFF           0x00U

/** flag for LWIP_DEBUGF indicating a tracing message (to follow program flow) */
#define LWIP_DBG_TRACE         0x40U
/** flag for LWIP_DEBUGF indicating a state debug message (to follow module states) */
#define LWIP_DBG_STATE         0x20U
/** flag for LWIP_DEBUGF indicating newly added code, not thoroughly tested yet */
#define LWIP_DBG_FRESH         0x10U
/** flag for LWIP_DEBUGF to halt after printing this debug message */
#define LWIP_DBG_HALT          0x08U

#ifdef __DOXYGEN__
#define LWIP_NOASSERT
#undef LWIP_NOASSERT
#endif

#ifndef LWIP_NOASSERT
#define LWIP_ASSERT(message, assertion) do { if (!(assertion)) { \
		LWIP_PLATFORM_ASSERT(message); } } while (0)
#ifndef LWIP_PLATFORM_ASSERT
#error "If you want to use LWIP_ASSERT, LWIP_PLATFORM_ASSERT(message) needs to be defined in your arch/cc.h"
#endif
#else							/* LWIP_NOASSERT */
#define LWIP_ASSERT(message, assertion)
#endif							/* LWIP_NOASSERT */

#ifndef LWIP_ERROR
#ifndef LWIP_NOASSERT
#define LWIP_PLATFORM_ERROR(message) LWIP_PLATFORM_ASSERT(message)
#elif defined LWIP_DEBUG
#define LWIP_PLATFORM_ERROR(message) LWIP_PLATFORM_DIAG((message))
#else
#define LWIP_PLATFORM_ERROR(message)
#endif

/* if "expression" isn't true, then print "message" and execute "handler" expression */
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
		LWIP_PLATFORM_ERROR(message); handler; } } while (0)
#endif							/* LWIP_ERROR */

#ifdef LWIP_DEBUG
#ifdef CONFIG_NET_LWIP_BUILDTIME_DEBUG

#ifndef LWIP_PLATFORM_DIAG
#error "If you want to use LWIP_DEBUG, LWIP_PLATFORM_DIAG(message) needs to be defined in your arch/cc.h"
#endif

#define LWIP_DEBUGF(debug, message) do { \
		if ( \
			((debug) & LWIP_DBG_ON) && \
			((debug) & LWIP_DBG_TYPES_ON) && \
			((s16_t)((debug) & LWIP_DBG_MASK_LEVEL) >= LWIP_DBG_MIN_LEVEL)) { \
				LWIP_PLATFORM_DIAG(message); \
				if ((debug) & LWIP_DBG_HALT) { \
					while (1); \
				} \
		} \
	} while (0)

#else /*  CONFIG_NET_LWIP_BUILDTIME_DEBUG */

#include <tinyara/net/netlogk.h>
#define LWIP_STRIP_PARENS( ... )   __VA_ARGS__
#define LWIP_DEBUGF(debug, message) \
	netlogk_print(NL_MOD_LWIP, debug, __FUNCTION__, __FILE__, __LINE__, LWIP_STRIP_PARENS message)
#endif							/* CONFIG_NET_LWIP_BUILDTIME_DEBUG */
#else /* LWIP_DEBUG  */
#define LWIP_DEBUGF(debug, message)
#endif /* LWIP_DEBUG  */

#endif							/* LWIP_HDR_DEBUG_H */
