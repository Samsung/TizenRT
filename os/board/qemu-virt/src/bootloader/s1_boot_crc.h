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
#ifndef __S1_BOOT_CRC_H__
#define __S1_BOOT_CRC_H__

#include <stdint.h>
#include <stddef.h> // For size_t
#include "s1_bootparam.h" // Include for bpdata_t
#include "s1_binheader.h" // Include for binheader_t

// The crc32part function
uint32_t crc32part(const uint8_t *src, size_t len, uint32_t crc32val);

// Function to calculate the CRC for the boot parameter header
uint32_t calculate_bootparam_crc(const bpdata_t *bp_data);
uint32_t calculate_kernel_crc(const binheader_t *bh_data);

#endif // __S1_BOOT_CRC_H__
