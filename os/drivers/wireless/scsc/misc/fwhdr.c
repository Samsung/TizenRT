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

#include "utils_misc.h"
#include "fwhdr.h"
/*
 * The Maxwell Firmware Header Format is defined in SC-505846-SW
 */

#define FWHDR_02_TRAMPOLINE_OFFSET 0
#define FWHDR_02_MAGIC_OFFSET 8
#define FWHDR_02_VERSION_MINOR_OFFSET 12
#define FWHDR_02_VERSION_MAJOR_OFFSET 14
#define FWHDR_02_LENGTH_OFFSET 16
#define FWHDR_02_FIRMWARE_API_VERSION_MINOR_OFFSET 20
#define FWHDR_02_FIRMWARE_API_VERSION_MAJOR_OFFSET 22
#define FWHDR_02_FIRMWARE_CRC_OFFSET 24
#define FWHDR_02_CONST_FW_LENGTH_OFFSET 28
#define FWHDR_02_CONST_CRC_OFFSET 32
#define FWHDR_02_FIRMWARE_RUNTIME_LENGTH_OFFSET 36
#define FWHDR_02_FIRMWARE_ENTRY_POINT_OFFSET 40
#define FWHDR_02_BUILD_ID_OFFSET 48
#define FWHDR_02_R4_PANIC_RECORD_OFFSET_OFFSET 176
#define FWHDR_02_M4_PANIC_RECORD_OFFSET_OFFSET 180
/*
 * Firmware header format for version 1.0 is same as version for 0.2
 */
#define FWHDR_02_TRAMPOLINE(__fw) (*((u32 *)(__fw + FWHDR_02_TRAMPOLINE_OFFSET)))
#define FWHDR_02_HEADER_FIRMWARE_ENTRY_POINT(__fw) (*((u32 *)(__fw + FWHDR_02_FIRMWARE_ENTRY_POINT_OFFSET)))
#define FWHDR_02_HEADER_FIRMWARE_RUNTIME_LENGTH(__fw) (*((u32 *)(__fw + FWHDR_02_FIRMWARE_RUNTIME_LENGTH_OFFSET)))
#define FWHDR_02_HEADER_VERSION_MAJOR(__fw) (*((u16 *)(__fw + FWHDR_02_VERSION_MAJOR_OFFSET)))
#define FWHDR_02_HEADER_VERSION_MINOR(__fw) (*((u16 *)(__fw + FWHDR_02_VERSION_MINOR_OFFSET)))
#define FWHDR_02_HEADER_FIRMWARE_API_VERSION_MINOR(__fw)  (*((u16 *)(__fw + FWHDR_02_FIRMWARE_API_VERSION_MINOR_OFFSET)))
#define FWHDR_02_HEADER_FIRMWARE_API_VERSION_MAJOR(__fw)  (*((u16 *)(__fw + FWHDR_02_FIRMWARE_API_VERSION_MAJOR_OFFSET)))
#define FWHDR_02_FW_CRC32(__fw) (*((u32 *)(__fw + FWHDR_02_FIRMWARE_CRC_OFFSET)))
#define FWHDR_02_HDR_LENGTH(__fw) (*((u32 *)(__fw + FWHDR_02_LENGTH_OFFSET)))
#define FWHDR_02_HEADER_CRC32(__fw) (*((u32 *)(__fw + (FWHDR_02_HDR_LENGTH(__fw)) - sizeof(u32))))
#define FWHDR_02_CONST_CRC32(__fw) (*((u32 *)(__fw + FWHDR_02_CONST_CRC_OFFSET)))
#define FWHDR_02_CONST_FW_LENGTH(__fw) (*((u32 *)(__fw + FWHDR_02_CONST_FW_LENGTH_OFFSET)))
#define FWHDR_02_R4_PANIC_RECORD_OFFSET(__fw) (*((u32 *)(__fw + FWHDR_02_R4_PANIC_RECORD_OFFSET_OFFSET)))
#define FWHDR_02_M4_PANIC_RECORD_OFFSET(__fw) (*((u32 *)(__fw + FWHDR_02_M4_PANIC_RECORD_OFFSET_OFFSET)))

/* firmware header has a panic record if the firmware header length is at least 192 bytes long */
#define MIN_HEADER_LENGTH_WITH_PANIC_RECORD 188

#define FWHDR_MAGIC_STRING "smxf"

static bool fwhdr_parse_v02(char *fw, struct fwhdr *fwhdr)
{
	if (!memcmp(fw + FWHDR_02_MAGIC_OFFSET, FWHDR_MAGIC_STRING, sizeof(FWHDR_MAGIC_STRING) - 1)) {
		fwhdr->firmware_entry_point = FWHDR_02_HEADER_FIRMWARE_ENTRY_POINT(fw);
		fwhdr->hdr_major = FWHDR_02_HEADER_VERSION_MAJOR(fw);
		fwhdr->hdr_minor = FWHDR_02_HEADER_VERSION_MINOR(fw);
		fwhdr->fwapi_major = FWHDR_02_HEADER_FIRMWARE_API_VERSION_MAJOR(fw);
		fwhdr->fwapi_minor = FWHDR_02_HEADER_FIRMWARE_API_VERSION_MINOR(fw);
		fwhdr->fw_crc32 = FWHDR_02_FW_CRC32(fw);
		fwhdr->const_crc32 = FWHDR_02_CONST_CRC32(fw);
		fwhdr->header_crc32 = FWHDR_02_HEADER_CRC32(fw);
		fwhdr->const_fw_length = FWHDR_02_CONST_FW_LENGTH(fw);
		fwhdr->hdr_length = FWHDR_02_HDR_LENGTH(fw);
		fwhdr->fw_runtime_length = FWHDR_02_HEADER_FIRMWARE_RUNTIME_LENGTH(fw);
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
		pr_info("hdr_length=%d\n", fwhdr->hdr_length);
#endif
		fwhdr->r4_panic_record_offset = FWHDR_02_R4_PANIC_RECORD_OFFSET(fw);
		fwhdr->m4_panic_record_offset = FWHDR_02_M4_PANIC_RECORD_OFFSET(fw);
		return true;
	}
	return false;
}

static char *fwhdr_get_build_id_v02(char *fw, struct fwhdr *fwhdr)
{
	if (!memcmp(fw + FWHDR_02_MAGIC_OFFSET, FWHDR_MAGIC_STRING, sizeof(FWHDR_MAGIC_STRING) - 1)) {
		return fw + FWHDR_02_BUILD_ID_OFFSET;
	}
	return NULL;
}

bool fwhdr_parse(char *fw, struct fwhdr *fwhdr)
{
	return fwhdr_parse_v02(fw, fwhdr);
}

char *fwhdr_get_build_id(char *fw, struct fwhdr *fwhdr)
{
	return fwhdr_get_build_id_v02(fw, fwhdr);
}
