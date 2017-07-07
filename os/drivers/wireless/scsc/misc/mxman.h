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

#ifndef _MAXWELL_MANAGER_H
#define _MAXWELL_MANAGER_H
#include <tinyara/wqueue.h>
#include "fwhdr.h"
#include "mxmgmt_transport.h"
#include "tinyara_wrapper.h"

struct mxman;

void mxman_init(struct mxman *mxman, struct scsc_mx *mx);
void mxman_deinit(struct mxman *mxman);
int mxman_open(struct mxman *mxman);
void mxman_close(struct mxman *mxman);
void mxman_fail(struct mxman *mxman);
void mxman_freeze(struct mxman *mxman);
int mxman_force_panic(struct mxman *mxman);
int mxman_suspend(struct mxman *mxman);
void mxman_resume(struct mxman *mxman);

enum mxman_state {
	MXMAN_STATE_STOPPED,
	MXMAN_STATE_STARTED,
	MXMAN_STATE_FAILED,
	MXMAN_STATE_FREEZED,
};

// simple list structure for timer-list
struct hlist_node {
	struct hlist_node *next, * *pprev;
};
struct timer_list {
	/*
	 * All fields that change during normal runtime grouped to the
	 * same cacheline
	 */
	struct hlist_node entry;
	unsigned long expires;
	void (*function)(unsigned long);
	unsigned long data;
	unsigned int flags;
	signed int slack;
};
struct delayed_work {
	struct work_s work;
	struct timer_list timer;

	/* target workqueue and CPU ->timer uses to queue ->work */
	struct workqueue_struct *wq;
	int cpu;
};

struct mxman {
	struct scsc_mx *mx;
	int users;
	void *start_sram;
	struct work_s fw_crc_work;
	struct work_s failure_work;
	struct work_s mm_msg_start_ind_work;
	char *fw;
	u32 fw_image_size;
	struct completion mm_msg_start_ind_completion;
	struct fwhdr fwhdr;
	struct mxconf *mxconf;
	enum mxman_state mxman_state;
	enum mxman_state mxman_next_state;
	pthread_mutex_t mxman_mutex;
	struct completion recovery_completion;
	int suspended;
	bool check_crc;
	u16 scsc_panic_code;
	char fw_build_id[64];
};

void mxman_register_gdb_channel(struct scsc_mx *mx, mxmgmt_channel_handler handler, void *data);
void mxman_send_gdb_channel(struct scsc_mx *mx, void *data, size_t length);

#endif
