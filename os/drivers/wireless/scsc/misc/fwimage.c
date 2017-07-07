/*****************************************************************************
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
#include <crc32.h>
#include <errno.h>
#include "fwimage.h"
#include "utils_misc.h"

int fwimage_check_fw_header_crc(char *fw, u32 hdr_length, u32 header_crc32)
{
	u32 header_crc32_calculated;

	/*
	 * The last 4-bytes are header CRC
	 */
	/*VD: need to check this crc32 */
	header_crc32_calculated = crc32((const uint8_t *)fw, hdr_length - sizeof(u32));
	if (header_crc32_calculated != header_crc32) {
		pr_err("%s: CRC32 doesn't match: header_crc32_calculated=%d header_crc32=%d\n", __func__, header_crc32_calculated, header_crc32);
		return -ENOENT;
	}
	return 0;
}

int fwimage_check_fw_const_section_crc(char *fw, u32 const_crc32, u32 const_fw_length, u32 hdr_length)
{
	u32 const_crc32_calculated;

	const_crc32_calculated = crc32(((const uint8_t *)(fw + hdr_length)), const_fw_length - hdr_length);
	if (const_crc32_calculated != const_crc32) {
		pr_err("%s: CRC32 doesn't match: const_crc32_calculated=%d const_crc32=%d\n", __func__, const_crc32_calculated, const_crc32);
		return -ENOENT;
	}
	return 0;
}

int fwimage_check_fw_crc(char *fw, u32 fw_image_length, u32 hdr_length, u32 fw_crc32)
{
	u32 fw_crc32_calculated;

	fw_crc32_calculated = crc32(((const uint8_t *)(fw + hdr_length)), fw_image_length - hdr_length);
	if (fw_crc32_calculated != fw_crc32) {
		pr_err("%s: CRC32 doesn't match: fw_crc32_calculated=%d fw_crc32=%d\n", __func__, fw_crc32_calculated, fw_crc32);
		return -ENOENT;
	}
	return 0;
}
