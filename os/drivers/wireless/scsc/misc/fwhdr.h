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

#ifndef FWHDR_H
#define FWHDR_H
#include <stdbool.h>

struct fwhdr {
	u16 hdr_major;
	u16 hdr_minor;

	u16 fwapi_major;
	u16 fwapi_minor;

	u32 firmware_entry_point;
	u32 fw_runtime_length;

	u32 fw_crc32;
	u32 const_crc32;
	u32 header_crc32;

	u32 const_fw_length;
	u32 hdr_length;
	u32 r4_panic_record_offset;
	u32 m4_panic_record_offset;
};

bool fwhdr_parse(char *fw, struct fwhdr *fwhdr);
char *fwhdr_get_build_id(char *fw, struct fwhdr *fwhdr);

#endif							/* FWHDR_H */
