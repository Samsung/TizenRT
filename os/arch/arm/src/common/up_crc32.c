/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/common/up_crc32.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>

#include <arm_acle.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef __ARM_FEATURE_CRC32
#error CONFIG_ARCH_CRC32 requires compiler support for ARM CRC32 instructions
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: up_crc32part
 ****************************************************************************/

uint32_t up_crc32part(FAR const uint8_t *src, size_t len, uint32_t crc32val)
{
	uint32_t crc_val = ~crc32val;

	while (len >= sizeof(uint32_t)) {
		uint32_t word = ((uint32_t)src[0]) |
						((uint32_t)src[1] << 8) |
						((uint32_t)src[2] << 16) |
						((uint32_t)src[3] << 24);

		crc_val = __crc32w(crc_val, word);
		src += sizeof(uint32_t);
		len -= sizeof(uint32_t);
	}

	if (len >= sizeof(uint16_t)) {
		uint16_t halfword = ((uint16_t)src[0]) |
							((uint16_t)src[1] << 8);

		crc_val = __crc32h(crc_val, halfword);
		src += sizeof(uint16_t);
		len -= sizeof(uint16_t);
	}

	if (len > 0) {
		crc_val = __crc32b(crc_val, *src);
	}

	return ~crc_val;
}
