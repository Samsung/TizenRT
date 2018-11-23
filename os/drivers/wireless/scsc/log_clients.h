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

#ifndef __SLSI_LOG_CLIENTS_H__
#define __SLSI_LOG_CLIENTS_H__

#include <sys/types.h>

struct slsi_dev;

struct slsi_log_client {
	struct slsi_dlist_head q;
	void *log_client_ctx;
	int (*log_client_cb)(struct slsi_log_client *, struct max_buff *, int);
	int min_signal_id;
	int max_signal_id;
	char *signal_filter;
};

struct slsi_log_clients {
	pthread_mutex_t log_client_lock;
	struct slsi_dlist_head log_client_list;
};

#define SLSI_LOG_DIRECTION_FROM_HOST 0
#define SLSI_LOG_DIRECTION_TO_HOST   1

void slsi_log_clients_init(struct slsi_dev *sdev);
int slsi_log_client_register(struct slsi_dev *sdev, void *log_client_ctx, int (*log_client_cb)(struct slsi_log_client *, struct max_buff *, int), char *filter, int min_signal_id, int max_signal_id);

void slsi_log_client_unregister(struct slsi_dev *sdev, void *log_client_ctx);
void slsi_log_client_msg(struct slsi_dev *sdev, u16 event, u32 event_data_length, const u8 *event_data);
void slsi_log_clients_log_signal_safe(struct slsi_dev *sdev, struct max_buff *mbuf, u32 direction);
static inline void slsi_log_clients_log_signal_fast(struct slsi_dev *sdev, struct slsi_log_clients *log_clients, struct max_buff *mbuf, u32 direction)
{
	/* list_empty() is unsafe but fast
	 * slsi_log_clients_log_signal_safe() will make sure all is good
	 */
	if (!slsi_dlist_empty(&log_clients->log_client_list)) {
		slsi_log_clients_log_signal_safe(sdev, mbuf, direction);
	}
}

void slsi_log_clients_terminate(struct slsi_dev *sdev);

#endif /*__SLSI_LOG_CLIENTS_H__*/
