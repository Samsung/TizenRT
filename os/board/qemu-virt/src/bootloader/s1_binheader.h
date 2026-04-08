/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
#ifndef __S1_BOOT_BINHEADER_H__
#define __S1_BOOT_BINHEADER_H__


#include <stdint.h>
// +-----------------------------------------------------------------------------------
// | Header size | Binary Version |  Binary Size | Secure Header Size | Secure Header |
// |   (2bytes)  |    (4bytes)    |   (4bytes)   |      (2bytes)      |    (#bytes)   |
// +-----------------------------------------------------------------------------------

struct binheader_s {
	uint32_t crc_hash;
	uint16_t header_size;
	uint32_t version;
	uint32_t bin_size;
	uint16_t secure_header_size;
} __attribute__((__packed__));
typedef struct binheader_s binheader_t;

#endif // __S1_BOOT_BINHEADER_H__
