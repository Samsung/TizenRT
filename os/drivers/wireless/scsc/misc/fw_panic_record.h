/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
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

#endif /* FW_PANIC_RECORD_H__ */
