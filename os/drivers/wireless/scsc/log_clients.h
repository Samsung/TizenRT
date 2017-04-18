/*****************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef __SLSI_LOG_CLIENTS_H__
#define __SLSI_LOG_CLIENTS_H__

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
