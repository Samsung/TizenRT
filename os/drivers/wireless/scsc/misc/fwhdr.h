/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
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
