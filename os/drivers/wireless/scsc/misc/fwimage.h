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

#ifndef FWIMAGE_H
#define FWIMAGE_H

int fwimage_check_fw_header_crc(char *fw, u32 hdr_length, u32 header_crc32);
int fwimage_check_fw_const_section_crc(char *fw, u32 const_crc32, u32 const_fw_length, u32 hdr_length);
int fwimage_check_fw_crc(char *fw, u32 fw_runtime_length, u32 hdr_length, u32 fw_crc32);

#endif							/* FWIMAGE_H */
