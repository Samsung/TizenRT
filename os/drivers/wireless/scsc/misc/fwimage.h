/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef FWIMAGE_H
#define FWIMAGE_H

int fwimage_check_fw_header_crc(char *fw, u32 hdr_length, u32 header_crc32);
int fwimage_check_fw_const_section_crc(char *fw, u32 const_crc32, u32 const_fw_length, u32 hdr_length);
int fwimage_check_fw_crc(char *fw, u32 fw_runtime_length, u32 hdr_length, u32 fw_crc32);

#endif /* FWIMAGE_H */
