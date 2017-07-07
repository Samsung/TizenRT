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

#ifndef FW_PANIC_RECORD_H__
#define FW_PANIC_RECORD_H__

#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY

#define SCSC_NUM_PANIC_RECORD (5)

struct panic_record_stats {
	u32 panic_id;
	u32 panic_arg;
};
extern struct panic_record_stats __r4_panic_record_stats[];
extern int __r4_panic_record_offset;
#endif

bool fw_parse_r4_panic_record(u32 *r4_panic_record);
void fw_parse_m4_panic_record(u32 *m4_panic_record);

#endif							/* FW_PANIC_RECORD_H__ */
