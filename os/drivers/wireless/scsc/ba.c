/****************************************************************************
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

#include "debug_scsc.h"
#include "dev.h"
#include "ba.h"
#include "mgt.h"

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <tinyara/kmalloc.h>

/* Age value for frames in MPDU reorder buffer */
#define BA_MPDU_FRAME_AGE_TIMEOUT  30	/* 30 milliseconds */
#define BA_WINDOW_BOUNDARY 2048
#define SN_TO_INDEX(__ba_session_rx, __sn) (((__sn - __ba_session_rx->start_sn) & 0xFFF) % __ba_session_rx->buffer_size)

#define ADVANCE_EXPECTED_SN(__ba_session_rx) \
	{ \
		__ba_session_rx->expected_sn++; \
		__ba_session_rx->expected_sn &= 0xFFF; \
	}

#define FREE_BUFFER_SLOT(__ba_session_rx, __index) \
	{ \
		__ba_session_rx->occupied_slots--; \
		__ba_session_rx->buffer[__index].active = false; \
	}

void slsi_rx_ba_init(struct slsi_dev *sdev)
{
	int i;

	for (i = 0; i < SLSI_MAX_RX_BA_SESSIONS; i++) {
		SLSI_MUTEX_INIT(sdev->rx_ba_buffer_pool[i].ba_lock);
	}

	SLSI_MUTEX_INIT(sdev->rx_ba_buffer_pool_lock);
}

static struct slsi_ba_session_rx *slsi_rx_ba_alloc_buffer(struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
	struct slsi_ba_session_rx *buffer = NULL;
	int i;

	SLSI_NET_DBG3(dev, SLSI_RX_BA, "RX BA buffer pool status: %d,%d,%d,%d,%d,%d,%d,%d\n", sdev->rx_ba_buffer_pool[0].used, sdev->rx_ba_buffer_pool[1].used, sdev->rx_ba_buffer_pool[2].used, sdev->rx_ba_buffer_pool[3].used, sdev->rx_ba_buffer_pool[4].used, sdev->rx_ba_buffer_pool[5].used, sdev->rx_ba_buffer_pool[6].used, sdev->rx_ba_buffer_pool[7].used);

	SLSI_MUTEX_LOCK(sdev->rx_ba_buffer_pool_lock);
	for (i = 0; i < SLSI_MAX_RX_BA_SESSIONS; i++) {
		if (!sdev->rx_ba_buffer_pool[i].used) {
			sdev->rx_ba_buffer_pool[i].used = true;
			buffer = &sdev->rx_ba_buffer_pool[i];
			break;
		}
	}
	SLSI_MUTEX_UNLOCK(sdev->rx_ba_buffer_pool_lock);

	if (buffer == NULL) {
		SLSI_NET_ERR(dev, "No free RX BA buffer\n");
	}

	return buffer;
}

static void slsi_rx_ba_free_buffer(struct netif *dev, struct slsi_peer *peer, int tid)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;

	SLSI_MUTEX_LOCK(sdev->rx_ba_buffer_pool_lock);
	if (peer && peer->ba_session_rx[tid]) {
		peer->ba_session_rx[tid]->used = false;
		peer->ba_session_rx[tid] = NULL;
	}
	SLSI_MUTEX_UNLOCK(sdev->rx_ba_buffer_pool_lock);

	SLSI_NET_DBG3(dev, SLSI_RX_BA, "RX BA buffer pool status: %d,%d,%d,%d,%d,%d,%d,%d\n", sdev->rx_ba_buffer_pool[0].used, sdev->rx_ba_buffer_pool[1].used, sdev->rx_ba_buffer_pool[2].used, sdev->rx_ba_buffer_pool[3].used, sdev->rx_ba_buffer_pool[4].used, sdev->rx_ba_buffer_pool[5].used, sdev->rx_ba_buffer_pool[6].used, sdev->rx_ba_buffer_pool[7].used);
}

/* This code - slsi_ba_process_complete()
 * is called in the data workqueue context with the
 * netdev_vif mutex held.
 */
void slsi_ba_process_complete(struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *mbuf;

	while ((mbuf = slsi_mbuf_dequeue(&ndev_vif->ba_complete)) != NULL) {
		slsi_rx_data(ndev_vif->sdev, dev, mbuf, true);
	}
}

static void slsi_ba_signal_process_complete(struct netif *dev)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	ndev_vif->ba_flush = 1;
	slsi_mbuf_schedule_work(&ndev_vif->rx_data, slsi_rx_netdev_data_work);
}

static void ba_add_frame_to_ba_complete(struct netif *dev, struct slsi_ba_frame_desc *frame_desc)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);

	slsi_mbuf_queue_tail(&ndev_vif->ba_complete, frame_desc->signal);
}

static void ba_update_expected_sn(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, u16 sn)
{
	u32 i, j;
	u16 gap;

	gap = (sn - ba_session_rx->expected_sn) & 0xFFF;
	SLSI_NET_DBG3(dev, SLSI_RX_BA, "Proccess the frames up to new expected_sn = %d gap = %d\n", sn, gap);

	for (j = 0; j < gap && j < ba_session_rx->buffer_size; j++) {
		i = SN_TO_INDEX(ba_session_rx, ba_session_rx->expected_sn);
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "Proccess the slot index = %d\n", i);
		if (ba_session_rx->buffer[i].active) {
			ba_add_frame_to_ba_complete(dev, &ba_session_rx->buffer[i]);
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "Proccess the frame at index = %d expected_sn = %d\n", i, ba_session_rx->expected_sn);
			FREE_BUFFER_SLOT(ba_session_rx, i);
		} else {
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "Empty slot at index = %d\n", i);
		}
		ADVANCE_EXPECTED_SN(ba_session_rx);
	}
	ba_session_rx->expected_sn = sn;
}

static void ba_complete_ready_sequence(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx)
{
	int i;

	i = SN_TO_INDEX(ba_session_rx, ba_session_rx->expected_sn);
	while (ba_session_rx->buffer[i].active) {
		ba_add_frame_to_ba_complete(dev, &ba_session_rx->buffer[i]);
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "Completed stored frame (expected_sn=%d) at i = %d\n", ba_session_rx->expected_sn, i);
		FREE_BUFFER_SLOT(ba_session_rx, i);
		ADVANCE_EXPECTED_SN(ba_session_rx);
		i = SN_TO_INDEX(ba_session_rx, ba_session_rx->expected_sn);
	}
}

static void ba_scroll_window(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, u16 sn)
{
	if (((sn - ba_session_rx->expected_sn) & 0xFFF) <= BA_WINDOW_BOUNDARY) {
		ba_update_expected_sn(dev, ba_session_rx, sn);
		ba_complete_ready_sequence(dev, ba_session_rx);
	}
}

static int ba_consume_frame_or_get_buffer_index(struct netif *dev, struct slsi_peer *peer, struct slsi_ba_session_rx *ba_session_rx, u16 sn, struct slsi_ba_frame_desc *frame_desc, bool *stop_timer)
{
#ifdef CONFIG_SLSI_WLAN_STATS
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
#endif
	int i;
	u16 sn_temp;

	*stop_timer = false;

	if (((sn - ba_session_rx->expected_sn) & 0xFFF) <= BA_WINDOW_BOUNDARY) {
		/* Once we are in BA window, set the flag for BA trigger */
		if (!ba_session_rx->trigger_ba_after_ssn) {
			ba_session_rx->trigger_ba_after_ssn = true;
		}

		sn_temp = ba_session_rx->expected_sn + ba_session_rx->buffer_size;
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "New frame: sn=%d\n", sn);

		if (!(((sn - sn_temp) & 0xFFF) > BA_WINDOW_BOUNDARY)) {
			u16 new_expected_sn;

			SLSI_NET_DBG3(dev, SLSI_RX_BA, "Frame is out of window\n");
			sn_temp = (sn - ba_session_rx->buffer_size) & 0xFFF;
			if (ba_session_rx->timer_on) {
				*stop_timer = true;
			}
			new_expected_sn = (sn_temp + 1) & 0xFFF;
			ba_update_expected_sn(dev, ba_session_rx, new_expected_sn);
		}

		i = -1;
		if (sn == ba_session_rx->expected_sn) {
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "sn = ba_session_rx->expected_sn = %d\n", sn);
			if (ba_session_rx->timer_on) {
				*stop_timer = true;
			}
			ADVANCE_EXPECTED_SN(ba_session_rx);
			ba_add_frame_to_ba_complete(dev, frame_desc);
		} else {
			i = SN_TO_INDEX(ba_session_rx, sn);
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "sn (%d) != ba_session_rx->expected_sn(%d), i = %d\n", sn, ba_session_rx->expected_sn, i);
			if (ba_session_rx->buffer[i].active) {
				SLSI_NET_DBG3(dev, SLSI_RX_BA, "free frame at i = %d\n", i);
				i = -1;
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_ba_duplicate_pkt);
				slsi_kfree_mbuf(frame_desc->signal);
			}
		}
	} else {
		i = -1;
		if (!ba_session_rx->trigger_ba_after_ssn) {
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "frame before ssn, pass it up: sn=%d\n", sn);
			ba_add_frame_to_ba_complete(dev, frame_desc);
		} else {
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "old frame, drop: sn=%d, expected_sn=%d\n", sn, ba_session_rx->expected_sn);
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_ba_out_of_order_pkt);
			slsi_kfree_mbuf(frame_desc->signal);
		}
	}
	return i;
}

static void slsi_ba_aging_timeout_handler(void *data)
{
	struct slsi_ba_session_rx *ba_session_rx = (struct slsi_ba_session_rx *)data;
	u8 i, j;
	u8 gap = 1;
	u16 temp_sn;
	struct netif *dev = ba_session_rx->dev;

	SLSI_NET_DBG3(dev, SLSI_RX_BA, "enter\n");

	SLSI_MUTEX_LOCK(ba_session_rx->ba_lock);

	ba_session_rx->timer_on = false;

	if (ba_session_rx->active && ba_session_rx->occupied_slots) {
		/* expected sequence has not arrived so start searching from next
		 * sequence number until a frame is available and determine the gap.
		 * Release all the frames upto next hole from the reorder buffer.
		 */
		temp_sn = (ba_session_rx->expected_sn + 1) & 0xFFF;

		for (j = 0; j < ba_session_rx->buffer_size; j++) {
			i = SN_TO_INDEX(ba_session_rx, temp_sn);

			if (ba_session_rx->buffer[i].active) {
				while (gap--) {
					ADVANCE_EXPECTED_SN(ba_session_rx);
				}

				SLSI_NET_DBG3(dev, SLSI_RX_BA, "Completed stored frame (expected_sn=%d) at i = %d\n", ba_session_rx->expected_sn, i);
				ba_add_frame_to_ba_complete(dev, &ba_session_rx->buffer[i]);
				FREE_BUFFER_SLOT(ba_session_rx, i);
				ADVANCE_EXPECTED_SN(ba_session_rx);
				ba_complete_ready_sequence(dev, ba_session_rx);
				break;
			}
			/* advance temp sequence number and frame gap */
			temp_sn = (temp_sn + 1) & 0xFFF;
			gap++;
		}

		/* Check for next hole in the buffer, if hole exists create the timer for next missing frame */
		if (ba_session_rx->occupied_slots) {
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "Timer start\n");
			work_cancel(SCSC_WORK, &ba_session_rx->work);
			work_queue(SCSC_WORK, &ba_session_rx->work, slsi_ba_aging_timeout_handler, ba_session_rx, MSEC2TICK(BA_MPDU_FRAME_AGE_TIMEOUT));
			ba_session_rx->timer_on = true;
		}
		SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);
		/* Process the data now marked as completed */
		slsi_ba_signal_process_complete(dev);
	} else {
		SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);
	}
}

int slsi_ba_process_frame(struct netif *dev, struct slsi_peer *peer, struct max_buff *mbuf, u16 sequence_number, u16 tid)
{
	int i;
	struct slsi_ba_session_rx *ba_session_rx = peer->ba_session_rx[tid];
	struct slsi_ba_frame_desc frame_desc;
	bool stop_timer = false;

	SLSI_NET_DBG3(dev, SLSI_RX_BA, "Got frame(sn=%d)\n", sequence_number);

	if (WARN_ON(tid > FAPI_PRIORITY_QOS_UP7)) {
		SLSI_NET_ERR(dev, "tid=%d\n", tid);
		return -EINVAL;
	}

	if (ba_session_rx == NULL) {
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ba_session_rx->ba_lock);

	if (!ba_session_rx->active) {
		SLSI_NET_ERR(dev, "No BA session exists\n");
		SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);
		return -EINVAL;
	}

	frame_desc.signal = mbuf;
	frame_desc.sn = sequence_number;
	frame_desc.active = true;

	i = ba_consume_frame_or_get_buffer_index(dev, peer, ba_session_rx, sequence_number, &frame_desc, &stop_timer);
	if (i >= 0) {
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "Store frame(sn=%d) at i = %d\n", sequence_number, i);
		ba_session_rx->buffer[i] = frame_desc;
		ba_session_rx->occupied_slots++;
	} else {
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "Frame consumed - sn = %d\n", sequence_number);
	}

	ba_complete_ready_sequence(dev, ba_session_rx);

	/* Timer decision:
	 *
	 * If the timer is not running (timer_on=false)
	 *    Start the timer if there are holes (occupied_slots!=0)
	 *
	 * If the timer is running (timer_on=true)
	 *    Stop the timer if
	 *       There are no holes (occupied_slots=0)
	 *    Restart the timer if
	 *       stop_timer=true and there are holes (occupied_slots!=0)
	 *    Leave the timer running (do nothing) if
	 *       stop_timer=false and there are holes (occupied_slots!=0)
	 */

	if (!ba_session_rx->timer_on) {
		if (ba_session_rx->occupied_slots) {
			stop_timer = false;
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "Timer start\n");
			work_cancel(SCSC_WORK, &ba_session_rx->work);
			work_queue(SCSC_WORK, &ba_session_rx->work, slsi_ba_aging_timeout_handler, ba_session_rx, MSEC2TICK(BA_MPDU_FRAME_AGE_TIMEOUT));
			ba_session_rx->timer_on = true;
		} else {
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "No slots occupied\n");
		}
	} else if (!ba_session_rx->occupied_slots) {
		stop_timer = true;
	} else if (stop_timer) {
		stop_timer = false;
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "Timer restart\n");
		work_cancel(SCSC_WORK, &ba_session_rx->work);
		work_queue(SCSC_WORK, &ba_session_rx->work, slsi_ba_aging_timeout_handler, ba_session_rx, MSEC2TICK(BA_MPDU_FRAME_AGE_TIMEOUT));
		ba_session_rx->timer_on = true;
	} else {
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "timer_on: %d, occupied_slots: %d, stop_timer: %d\n", ba_session_rx->timer_on, ba_session_rx->occupied_slots, stop_timer);
	}

	if (stop_timer) {
		ba_session_rx->timer_on = false;
		SLSI_NET_DBG3(dev, SLSI_RX_BA, "Timer stop\n");
		work_cancel(SCSC_WORK, &ba_session_rx->work);
	}
	SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);

	slsi_ba_signal_process_complete(dev);
	return 0;
}

bool slsi_ba_check(struct slsi_peer *peer, u16 tid)
{
	if (tid > FAPI_PRIORITY_QOS_UP7) {
		return false;
	}
	if (peer->ba_session_rx[tid] == NULL) {
		return false;
	}

	return peer->ba_session_rx[tid]->active;
}

static void __slsi_rx_ba_stop(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, bool flush)
{
#ifdef CONFIG_SLSI_WLAN_STATS
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
#endif
	u8 i, j;

	SLSI_NET_DBG1(dev, SLSI_RX_BA, "Stopping BA session: tid = %d\n", ba_session_rx->tid);

	if (WARN_ON(!ba_session_rx->active)) {
		SLSI_NET_ERR(dev, "No BA session exists\n");
		return;
	}

	for (i = SN_TO_INDEX(ba_session_rx, ba_session_rx->expected_sn), j = 0; j < ba_session_rx->buffer_size; i++, j++) {
		i %= ba_session_rx->buffer_size;
		if (ba_session_rx->buffer[i].active) {
			if (flush) {
				/* Drop the pending packet if the BA stop is triggered because of disconnection */
				SLSI_NET_DBG1(dev, SLSI_RX_BA, "Drop pending reorder frame, len = %d\n", ba_session_rx->buffer[i].signal->data_len);
				slsi_kfree_mbuf(ba_session_rx->buffer[i].signal);
				SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_ba_stop);
			} else {
				/* Forward the packet to the network stack if BA stop is triggered by DEL-BA */
				ba_add_frame_to_ba_complete(dev, &ba_session_rx->buffer[i]);
			}
			SLSI_NET_DBG3(dev, SLSI_RX_BA, "Completed stored frame at i = %d\n", i);
			FREE_BUFFER_SLOT(ba_session_rx, i);
		}
	}

	if (!flush) {
		slsi_ba_signal_process_complete(dev);
	}

	ba_session_rx->active = false;
}

static void slsi_rx_ba_stop_lock_held(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, bool flush)
{
	__slsi_rx_ba_stop(dev, ba_session_rx, flush);
	if (ba_session_rx->timer_on) {
		ba_session_rx->timer_on = false;
		work_cancel(SCSC_WORK, &ba_session_rx->work);
	}
}

static void slsi_rx_ba_stop_lock_unheld(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, bool flush)
{
	SLSI_MUTEX_LOCK(ba_session_rx->ba_lock);
	__slsi_rx_ba_stop(dev, ba_session_rx, flush);
	if (ba_session_rx->timer_on) {
		ba_session_rx->timer_on = false;
		work_cancel(SCSC_WORK, &ba_session_rx->work);
	}
	SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);
}

void slsi_rx_ba_stop_all(struct netif *dev, struct slsi_peer *peer)
{
	int i;

	for (i = 0; i < NUM_BA_SESSIONS_PER_PEER; i++)
		if (peer->ba_session_rx[i] && peer->ba_session_rx[i]->active) {
			slsi_rx_ba_stop_lock_unheld(dev, peer->ba_session_rx[i], true);
			slsi_rx_ba_free_buffer(dev, peer, i);
		}
}

static int slsi_rx_ba_start(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, u16 tid, u16 buffer_size, u16 start_sn)
{
	SLSI_NET_DBG1(dev, SLSI_RX_BA, "Request to start a new BA session tid=%d buffer_size=%d start_sn=%d\n", tid, buffer_size, start_sn);

	if (WARN_ON((0 == buffer_size) || (buffer_size > SLSI_BA_BUFFER_SIZE_MAX))) {
		SLSI_NET_ERR(dev, "Invalid window size: buffer_size=%d\n", buffer_size);
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(ba_session_rx->ba_lock);

	if (ba_session_rx->active) {
		SLSI_NET_DBG1(dev, SLSI_RX_BA, "BA session already exists\n");

		if ((ba_session_rx->buffer_size == buffer_size) && (ba_session_rx->expected_sn == start_sn)) {
			SLSI_NET_DBG1(dev, SLSI_RX_BA, "BA session tid=%d already exists. The parameters match so keep the existing session\n", tid);

			SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);

			return 0;
		}
		SLSI_NET_WARN(dev, "Parameters don't match so stop the existing BA session: tid=%d\n", tid);
		slsi_rx_ba_stop_lock_held(dev, ba_session_rx, false);
	}

	ba_session_rx->dev = dev;
	ba_session_rx->buffer_size = buffer_size;
	ba_session_rx->start_sn = start_sn;
	ba_session_rx->expected_sn = start_sn;
	ba_session_rx->trigger_ba_after_ssn = false;
	ba_session_rx->tid = tid;
	ba_session_rx->active = true;
	ba_session_rx->timer_on = false;
	ba_session_rx->occupied_slots = 0;

	SLSI_NET_DBG1(dev, SLSI_RX_BA, "Started a new BA session tid=%d buffer_size=%d start_sn=%d\n", tid, buffer_size, start_sn);

	SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);
	slsi_ba_signal_process_complete(dev);

	return 0;
}

static void slsi_ba_process_error(struct netif *dev, struct slsi_ba_session_rx *ba_session_rx, u16 sequence_number)
{
	SLSI_MUTEX_LOCK(ba_session_rx->ba_lock);

	if (WARN_ON(!ba_session_rx->active)) {
		SLSI_NET_ERR(dev, "No BA session exists\n");
		SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);
		return;
	}

	ba_scroll_window(dev, ba_session_rx, sequence_number);

	SLSI_MUTEX_UNLOCK(ba_session_rx->ba_lock);

	slsi_ba_signal_process_complete(dev);
}

void slsi_handle_blockack(struct netif *dev, struct slsi_peer *peer, u16 vif, u8 *peer_qsta_address, u16 parameter_set, u16 sequence_number, u16 reason_code, u16 direction)
{
	struct slsi_ba_session_rx *ba_session_rx;
	u16 user_priority = (parameter_set >> 2) & 0x000F;
	u16 buffer_size = (parameter_set >> 6) & 0x03FF;
#ifdef CONFIG_SLSI_WLAN_STATS
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
#endif

	SLSI_UNUSED_PARAMETER(vif);
	SLSI_UNUSED_PARAMETER(peer_qsta_address);

	if (WARN_ON(user_priority > FAPI_PRIORITY_QOS_UP7)) {
		SLSI_NET_ERR(dev, "Invalid user_priority=%d\n", user_priority);
		return;
	}

	switch (direction) {
	case FAPI_DIRECTION_TRANSMIT:
#ifdef CONFIG_SLSI_WLAN_STATS
		switch (reason_code) {
		case FAPI_REASONCODE_START:
			SLSI_INCR_DATA_PATH_STATS_SET_VAL(sdev->dp_stats.tx_ba, 1);
			break;
		case FAPI_REASONCODE_END:
			SLSI_INCR_DATA_PATH_STATS_SET_VAL(sdev->dp_stats.tx_ba, 0);
			break;
		}
#endif
		break;
	case FAPI_DIRECTION_RECEIVE:
		ba_session_rx = peer->ba_session_rx[user_priority];

		switch (reason_code) {
		case FAPI_REASONCODE_START:
			SLSI_INCR_DATA_PATH_STATS_SET_VAL(sdev->dp_stats.rx_ba, 1);
			if (peer->ba_session_rx[user_priority] == NULL) {
				peer->ba_session_rx[user_priority] = slsi_rx_ba_alloc_buffer(dev);
			}

			if (peer->ba_session_rx[user_priority])
				if (slsi_rx_ba_start(dev, peer->ba_session_rx[user_priority], user_priority, buffer_size, sequence_number) != 0) {
					slsi_rx_ba_free_buffer(dev, peer, user_priority);
				}
			break;
		case FAPI_REASONCODE_END:
			SLSI_INCR_DATA_PATH_STATS_SET_VAL(sdev->dp_stats.rx_ba, 0);
			if (ba_session_rx) {
				slsi_rx_ba_stop_lock_unheld(dev, ba_session_rx, false);
				slsi_rx_ba_free_buffer(dev, peer, user_priority);
			}
			break;
		case FAPI_REASONCODE_UNSPECIFIED_REASON:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_ba_scroll_window);
			if (ba_session_rx) {
				slsi_ba_process_error(dev, ba_session_rx, sequence_number);
			}
			break;
		default:
			SLSI_NET_ERR(dev, "Invalid value: reason_code=%d\n", reason_code);
			break;
		}
		break;
	default:
		SLSI_NET_ERR(dev, "Invalid value: direction=%d\n", direction);
		break;
	}
}
