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

#include "dev.h"
#include "debug_scsc.h"
#include "udi.h"
#include "log_clients.h"
#include "unifiio.h"

/* These functions should NOT be called from interrupt context */
/* It is supposed to be called from process context, or
 * NET_TX_SOFTIRQ - with BHs disabled and interrupts disabled
 */
/* Do not sleep */

/* Try to terminate UDI user space users (applications) connected on the cdev (0,1) */
#define SCSC_TERM_UDI_USERS   (1)

void slsi_log_clients_log_signal_safe(struct slsi_dev *sdev, struct max_buff *mbuf, u32 direction)
{
	struct slsi_dlist_head *pos, *n;
	struct slsi_log_client *log_client;
	int dir = (direction == SLSI_LOG_DIRECTION_FROM_HOST) ? UDI_FROM_HOST : UDI_TO_HOST;

	SLSI_MUTEX_LOCK(sdev->log_clients.log_client_lock);
	slsi_dlist_for_each_safe(pos, n, &sdev->log_clients.log_client_list) {
		log_client = slsi_dlist_entry(pos, struct slsi_log_client, q);
		log_client->log_client_cb(log_client, mbuf, dir);
	}
	SLSI_MUTEX_UNLOCK(sdev->log_clients.log_client_lock);
}

void slsi_log_clients_init(struct slsi_dev *sdev)
{
	SLSI_INIT_DLIST_HEAD(&sdev->log_clients.log_client_list);
	SLSI_MUTEX_INIT(sdev->log_clients.log_client_lock);
}

/* The arg called "filter" will eventually be passed to kmm_free().
 * - so pass a NULL if you are not doing any filtering
 */
int slsi_log_client_register(struct slsi_dev *sdev, void *log_client_ctx, int (*log_client_cb)(struct slsi_log_client *, struct max_buff *, int), char *filter, int min_signal_id, int max_signal_id)
{
	struct slsi_log_client *log_client;
	int first_in_list = 0;

	first_in_list = slsi_dlist_empty(&sdev->log_clients.log_client_list);
	log_client = kmm_malloc(sizeof(*log_client));
	if (log_client == NULL) {
		return -ENOMEM;
	}

	log_client->min_signal_id = min_signal_id;
	log_client->max_signal_id = max_signal_id;
	log_client->signal_filter = filter;
	log_client->log_client_ctx = log_client_ctx;
	log_client->log_client_cb = log_client_cb;
	log_client->q.lnext = NULL;
	log_client->q.lprev = NULL;
	/* Add to tail of log queue */
	SLSI_MUTEX_LOCK(sdev->log_clients.log_client_lock);
	slsi_dlist_add_tail(&log_client->q, &sdev->log_clients.log_client_list);
	SLSI_MUTEX_UNLOCK(sdev->log_clients.log_client_lock);

	return 0;
}

void slsi_log_clients_terminate(struct slsi_dev *sdev)
{
	/* If the driver is configured to try and terminate UDI user space
	 * applications, the following will try to do so.
	 */
	if (SCSC_TERM_UDI_USERS) {
		slsi_log_client_msg(sdev, UDI_DRV_UNLOAD_IND, 0, NULL);
	}
}

void slsi_log_client_msg(struct slsi_dev *sdev, u16 event, u32 event_data_length, const u8 *event_data)
{
	struct slsi_dlist_head *pos, *n;
	struct slsi_log_client *log_client;

	SLSI_MUTEX_LOCK(sdev->log_clients.log_client_lock);
	slsi_dlist_for_each_safe(pos, n, &sdev->log_clients.log_client_list) {
		log_client = slsi_dlist_entry(pos, struct slsi_log_client, q);
		if (slsi_kernel_to_user_space_event(log_client, event, event_data_length, event_data)) {
			SLSI_WARN(sdev, "Failed to send event(0x%.4X) to UDI client 0x%p\n", event, log_client);
		}
	}
	SLSI_MUTEX_UNLOCK(sdev->log_clients.log_client_lock);
}

void slsi_log_client_unregister(struct slsi_dev *sdev, void *log_client_ctx)
{
	struct slsi_dlist_head *pos, *n;
	struct slsi_log_client *log_client;

	SLSI_MUTEX_LOCK(sdev->log_clients.log_client_lock);
	slsi_dlist_for_each_safe(pos, n, &sdev->log_clients.log_client_list) {
		log_client = slsi_dlist_entry(pos, struct slsi_log_client, q);
		if (log_client->log_client_ctx == log_client_ctx) {
			kmm_free(log_client->signal_filter);
			slsi_dlist_del(pos);
			kmm_free(log_client);
		}
	}
	SLSI_MUTEX_UNLOCK(sdev->log_clients.log_client_lock);
}
