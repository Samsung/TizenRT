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
#include "panic_record_r4_defs.h"
#include "utils_misc.h"
#include "debug_scsc.h"

#define PANIC_RECORD_CKSUM_SEED 0xa5a5a5a5
/*
 * version 2 r4 panic record defs
 */
#define R4_PANIC_RECORD_VERSION_2 2
#define R4_PANIC_RECORD_LENGTH_INDEX_V2 1
#define R4_PANIC_RECORD_PANIC_CODE_INDEX_V2 3
#define R4_PANIC_RECORD_PANIC_ARG_INDEX_V2 4
#define R4_PANIC_RECORD_MAX_LENGTH_V2 256

/*
 * version 1 mr4 panic record defs
 */
#define M4_PANIC_RECORD_VERSION_1 1
#define M4_PANIC_RECORD_VERSION_INDEX 0
#define M4_PANIC_RECORD_LENGTH_INDEX 1
#define M4_PANIC_RECORD_MAX_LENGTH 256

#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
struct panic_record_stats __r4_panic_record_stats[SCSC_NUM_PANIC_RECORD];
int __r4_panic_record_offset = 0;
#endif

/**
 * Compute 32bit xor of specified seed value and data.
 *
 * @param   seed    Initial seed value.
 * @param   data    Array of uint32s to be xored
 * @param   len     Number of uint32s to be xored
 *
 * @return  Computed 32bit xor of specified seed value and data.
 */
static u32 xor32(uint32_t seed, const u32 data[], size_t len)
{
	const u32 *i;

	u32 xor = seed;

	for (i = data; i != data + len; ++i) {
		xor ^= *i;
	}
	return xor;
}

static void panic_record_dump(u32 *panic_record, u32 panic_record_length, bool r4)
{
	int i;

	SLSI_INFO_NODEV("%s panic record dump(length=%d):\n", r4 ? "R4" : "M4", panic_record_length);
	for (i = 0; i < panic_record_length; i++) {
		SLSI_INFO_NODEV("%s_panic_record[%d] = %08x\n", r4 ? "r4" : "m4", i, panic_record[i]);
	}
	if (panic_record_length > R4_PANIC_RECORD_PANIC_ARG_INDEX_V2) {
		SLSI_WARN_NODEV("FW PANIC: |cpu r4|id 0x%04X|arg 0x%08X|\n", panic_record[R4_PANIC_RECORD_PANIC_CODE_INDEX_V2], panic_record[R4_PANIC_RECORD_PANIC_ARG_INDEX_V2]);
	}
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	/* Store the initial SCSC_NUM_PANIC_RECORD number of panic records (ID and argument) */
	if (__r4_panic_record_offset < SCSC_NUM_PANIC_RECORD) {
		__r4_panic_record_stats[__r4_panic_record_offset].panic_id = panic_record[R4_PANIC_RECORD_PANIC_CODE_INDEX_V2];
		__r4_panic_record_stats[__r4_panic_record_offset].panic_arg = panic_record[R4_PANIC_RECORD_PANIC_ARG_INDEX_V2];

		__r4_panic_record_offset++;
	}
#endif
}

static void fw_parse_r4_panic_record_v1(u32 *r4_panic_record)
{
	u32 panic_record_cksum = *(r4_panic_record + PANIC_RECORD_R4_CKSUM_INDEX);
	u32 calculated_cksum;

	calculated_cksum = xor32(PANIC_RECORD_CKSUM_SEED, r4_panic_record, PANIC_RECORD_R4_LEN - 1);
	if (calculated_cksum == panic_record_cksum) {
		SLSI_INFO_NODEV("panic_record_cksum OK: %08x\n", calculated_cksum);
		panic_record_dump(r4_panic_record, PANIC_RECORD_R4_LEN, true);
	} else {
		SLSI_ERR_NODEV("BAD panic_record_cksum: 0x%x calculated_cksum: 0x%x\n", panic_record_cksum, calculated_cksum);
	}
}

static bool fw_parse_r4_panic_record_v2(u32 *r4_panic_record)
{
	u32 panic_record_cksum;
	u32 calculated_cksum;
	u32 panic_record_length = *(r4_panic_record + R4_PANIC_RECORD_LENGTH_INDEX_V2) / 4;

	SLSI_INFO_NODEV("panic_record_length: %d\n", panic_record_length);

	if (panic_record_length < R4_PANIC_RECORD_MAX_LENGTH_V2) {
		panic_record_cksum = *(r4_panic_record + panic_record_length - 1);
		calculated_cksum = xor32(PANIC_RECORD_CKSUM_SEED, r4_panic_record, panic_record_length - 1);
		if (calculated_cksum == panic_record_cksum) {
			SLSI_INFO_NODEV("panic_record_cksum OK: %08x\n", calculated_cksum);
			panic_record_dump(r4_panic_record, panic_record_length, true);
			return true;
		} else {
			SLSI_ERR_NODEV("BAD panic_record_cksum: 0x%x calculated_cksum: 0x%x\n", panic_record_cksum, calculated_cksum);
		}
	} else {
		SLSI_ERR_NODEV("BAD panic_record_length: %d\n", panic_record_length);
	}
	return false;
}

static void fw_parse_m4_panic_record_v1(u32 *m4_panic_record)
{
	u32 panic_record_cksum;
	u32 calculated_cksum;
	u32 panic_record_length = *(m4_panic_record + M4_PANIC_RECORD_LENGTH_INDEX) / 4;

	SLSI_INFO_NODEV("panic_record_length: %d\n", panic_record_length);
	if (panic_record_length < M4_PANIC_RECORD_MAX_LENGTH) {
		panic_record_cksum = *(m4_panic_record + panic_record_length - 1);
		calculated_cksum = xor32(PANIC_RECORD_CKSUM_SEED, m4_panic_record, panic_record_length - 1);
		if (calculated_cksum == panic_record_cksum) {
			SLSI_INFO_NODEV("panic_record_cksum OK: %08x\n", calculated_cksum);
			panic_record_dump(m4_panic_record, panic_record_length, false);
		} else {
			SLSI_ERR_NODEV("BAD panic_record_cksum: 0x%x calculated_cksum: 0x%x\n", panic_record_cksum, calculated_cksum);
		}
	} else {
		SLSI_ERR_NODEV("BAD panic_record_length: %d\n", panic_record_length);
	}
}

bool fw_parse_r4_panic_record(u32 *r4_panic_record)
{
	u32 panic_record_version = *(r4_panic_record + PANIC_RECORD_R4_VERSION_INDEX);

	SLSI_INFO_NODEV("panic_record_version: %d\n", panic_record_version);
	switch (panic_record_version) {
	default:
		SLSI_ERR_NODEV("BAD panic_record_version: %d\n", panic_record_version);
		break;
	case R4_PANIC_RECORD_VERSION_2:
		return fw_parse_r4_panic_record_v2(r4_panic_record);
	}
	return false;
}

void fw_parse_m4_panic_record(u32 *m4_panic_record)
{
	u32 panic_record_version = *(m4_panic_record + M4_PANIC_RECORD_VERSION_INDEX);

	SLSI_INFO_NODEV("panic_record_version: %d\n", panic_record_version);
	switch (panic_record_version) {
	default:
		SLSI_ERR_NODEV("BAD panic_record_version: %d\n", panic_record_version);
		return;
	case M4_PANIC_RECORD_VERSION_1:
		fw_parse_m4_panic_record_v1(m4_panic_record);
		break;
	}
}
