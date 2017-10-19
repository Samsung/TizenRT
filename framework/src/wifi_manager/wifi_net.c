/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <tinyara/config.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <errno.h>
#include <debug.h>
#include <netinet/in.h>
#include <netdb.h>
#include "wifi_utils.h"
#include "wifi_net.h"

#define ptr2uint32(ptr) (uint32_t)ptr[3] |     \
				((uint32_t)ptr[2] << 8)  |     \
				((uint32_t)ptr[1] << 16) |     \
				((uint32_t)ptr[0] << 24)

#define NTP2UNIX_TRANLSLATION 2208988800u
#define NTP_VERSION           3
static void convert_ntp_time(FAR uint8_t *timestamp, unsigned int *sec, unsigned int *usec)
{
	time_t seconds;
	uint32_t frac;
	uint32_t nsec;
#ifdef CONFIG_HAVE_LONG_LONG
	uint64_t tmp;
#else
	uint32_t a16;
	uint32_t b0;
	uint32_t t32;
	uint32_t t16;
	uint32_t t0;
#endif

	/* NTP timestamps are represented as a 64-bit fixed-point number, in
	 * seconds relative to 0000 UT on 1 January 1900.  The integer part is
	 * in the first 32 bits and the fraction part in the last 32 bits, as
	 * shown in the following diagram.
	 *
	 *    0                   1                   2                   3
	 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Integer Part                          |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *   |                         Fraction Part                         |
	 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 */

	seconds = ptr2uint32(timestamp);

	/* Translate seconds to account for the difference in the origin time */

	if (seconds > NTP2UNIX_TRANLSLATION) {
		seconds -= NTP2UNIX_TRANLSLATION;
	}

	/* Conversion of the fractional part to nanoseconds:
	 *
	 *  NSec = (f * 1,000,000,000) / 4,294,967,296
	 *       = (f * (5**9 * 2**9) / (2**32)
	 *       = (f * 5**9) / (2**23)
	 *       = (f * 1,953,125) / 8,388,608
	 */

	frac = ptr2uint32((timestamp + 4));
#ifdef CONFIG_HAVE_LONG_LONG
	/* if we have 64-bit long long values, then the computation is easy */

	tmp = ((uint64_t)frac * 1953125) >> 23;
	nsec = (uint32_t)tmp;

#else
	/* If we don't have 64 bit integer types, then the calculation is a little
	 * more complex:
	 *
	 * Let f         = a    << 16 + b
	 *     1,953,125 = 0x1d << 16 + 0xcd65
	 * NSec << 23 =  ((a << 16) + b) * ((0x1d << 16) + 0xcd65)
	 *            = (a << 16) * 0x1d << 16) +
	 *              (a << 16) * 0xcd65 +
	 *              b         * 0x1d << 16) +
	 *              b         * 0xcd65;
	 */

	/* Break the fractional part up into two values */

	a16 = frac >> 16;
	b0 = frac & 0xffff;

	/* Get the b32 and b0 terms
	 *
	 * t32 = (a << 16) * 0x1d << 16)
	 * t0  = b * 0xcd65
	 */

	t32 = 0x001d * a16;
	t0 = 0xcd65 * b0;

	/* Get the first b16 term
	 *
	 * (a << 16) * 0xcd65
	 */

	t16 = 0xcd65 * a16;

	/* Add the upper 16-bits to the b32 accumulator */

	t32 += (t16 >> 16);

	/* Add the lower 16-bits to the b0 accumulator, handling carry to the b32
	 * accumulator
	 */

	t16 <<= 16;
	if (t0 > (0xffffffff - t16)) {
		t32++;
	}

	t0 += t16;

	/* Get the second b16 term
	 *
	 * b * (0x1d << 16)
	 */

	t16 = 0x001d * b0;

	/* Add the upper 16-bits to the b32 accumulator */

	t32 += (t16 >> 16);

	/* Add the lower 16-bits to the b0 accumulator, handling carry to the b32
	 * accumulator
	 */

	t16 <<= 16;
	if (t0 > (0xffffffff - t16)) {
		t32++;
	}

	t0 += t16;

	/* t32 and t0 represent the 64 bit product.  Now shift right by 23 bits to
	 * accomplish the divide by by 2**23.
	 */

	nsec = (t32 << (32 - 23)) + (t0 >> 23);
#endif
	*sec = seconds;
	*usec = nsec / 1000;
	ndbg(" %lu seconds, %lu usec\n", (uint32_t)seconds, (uint32_t)nsec / 1000);

}

wifi_utils_result_e wifi_net_hostname_to_ip4(char *hostname, unsigned int *ip4_address)
{
	if (hostname == NULL || ip4_address == NULL) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	struct hostent *shost;

	shost = gethostbyname((const char *)hostname);
	if (shost == NULL) {
		ndbg("gethostbyname fail %d\n", shost);
		return WIFI_UTILS_FAIL;
	}

	memcpy(ip4_address, shost->h_addr, sizeof(in_addr_t));

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_net_ip4_addr_to_ip4_str(unsigned int ip4_addr, char ip4_str[18])
{
	if (ip4_addr <= 0 || ip4_str == NULL) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	unsigned char bytes[4];
	bytes[0] = ip4_addr & 0xFF;
	bytes[1] = (ip4_addr >> 8) & 0xFF;
	bytes[2] = (ip4_addr >> 16) & 0xFF;
	bytes[3] = (ip4_addr >> 24) & 0xFF;

	sprintf(ip4_str, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_net_mac_addr_to_mac_str(unsigned char mac_addr[6], char mac_str[20])
{
	if (!mac_addr || !mac_str) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	snprintf(mac_str, 18, "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
	return WIFI_UTILS_SUCCESS;
}
