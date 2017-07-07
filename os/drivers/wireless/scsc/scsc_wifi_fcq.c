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

#include "scsc_wifi_fcq.h"
#include "debug_scsc.h"
#include "dev.h"

#ifdef CONFIG_SCSC_ENABLE_PORT_CONTROL
bool slsi_is_port_blocked(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs)
{
	if (qs->controlled_port_state == SCSC_WIFI_FCQ_8021x_STATE_BLOCKED) {
		return true;
	}

	return false;
}
#endif

static int fcq_transmit(struct netif *dev, struct scsc_wifi_fcq_q_header *queue)
{
	if ((queue->qcod + 1) >= queue->qmod) {
		return ERR_BUF;
	}
	queue->qcod++;

	return 0;
}

#ifdef ENABLE_DYNAMIC_QMOD_ADAPTATION
static void scsc_wifi_fcq_redistribute_qmod(struct scsc_wifi_fcq_data_qset *qs)
{
	int i;
	int num_active_ac = 0;

	/* Get the number of active access categories */
	for (i = 0; i < SLSI_NETIF_Q_PER_PEER; i++) {
		if (qs->ac_inuse & (1 << i)) {
			num_active_ac++;
		}
	}

	if (num_active_ac == 0) {
		SLSI_ERR_NODEV("No active ACs\n");
		return;
	}

	/* Distribute the remaining slots among the active access categories */
	for (i = 0; i < SLSI_NETIF_Q_PER_PEER; i++) {
		qs->ac_q[i].head.qmod = SCSC_WIFI_FCQ_QMOD;

		if (qs->ac_inuse & (1 << i)) {
			qs->ac_q[i].head.qmod += (SCSC_WIFI_FCQ_REMAINING_SLOTS / num_active_ac);
		}
	}
}

static void scsc_wifi_fcq_redistribute_qmod_timer(void *data)
{
	struct scsc_wifi_fcq_data_qset *qs = (struct scsc_wifi_fcq_data_qset *)data;

	SLSI_MUTEX_LOCK(qs->cp_lock);

	if (qs->ac_inuse != qs->ac_active) {
		qs->ac_inuse = qs->ac_active;
		scsc_wifi_fcq_redistribute_qmod(qs);
	}
	qs->ac_active = 0;

	SLSI_MUTEX_UNLOCK(qs->cp_lock);
}
#endif

int scsc_wifi_fcq_transmit_data(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs, u16 priority, bool multicast)
{
	if (WARN_ON(!dev)) {
		return ERR_ARG;
	}

	if (WARN_ON(!qs)) {
		return ERR_ARG;
	}

	if (WARN_ON(priority >= ARRAY_SIZE(qs->ac_q))) {
		return ERR_ARG;
	}

	SLSI_MUTEX_LOCK(qs->cp_lock);

#ifdef CONFIG_SCSC_ENABLE_PORT_CONTROL
	if (qs->controlled_port_state == SCSC_WIFI_FCQ_8021x_STATE_BLOCKED) {
#ifdef CONFIG_SLSI_WLAN_STATS
		struct netdev_vif *ndev_vif = netdev_priv(dev);
		struct slsi_dev *sdev = ndev_vif->sdev;

		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_port_blocked);
#endif
		SLSI_MUTEX_UNLOCK(qs->cp_lock);
		return ERR_CONN;
	}
#endif

#ifdef ENABLE_DYNAMIC_QMOD_ADAPTATION
	if (!multicast) {
		if (!(qs->ac_inuse & (1 << priority))) {
			/* Packet from new access category */
			qs->ac_inuse |= (1 << priority);
			scsc_wifi_fcq_redistribute_qmod(qs);
		}
		if (qs->ac_active == 0) {
			/* Schedule the work to redistribute traffic based on active traffic */
			work_queue(SCSC_WORK, &qs->fcq_work, scsc_wifi_fcq_redistribute_qmod_timer, qs, MSEC2TICK(SCSC_FCQ_REDISTRIBUTE_TIME));
		}
		qs->ac_active |= (1 << priority);
	}
#endif

#ifdef ENABLE_SCOD
	if ((qs->scod + 1) >= qs->smod) {
		/* Drop the packet */
		SLSI_MUTEX_UNLOCK(qs->cp_lock);
		SLSI_NET_DBG3(dev, SLSI_TX, "scod (%d) >= smod (%d)\n", qs->scod, qs->smod);
		return ERR_BUF;
	}
	qs->scod++;
#endif

	if (fcq_transmit(dev, &qs->ac_q[priority].head) < 0) {
#ifdef ENABLE_SCOD
		/* Queue is full and was not active, so decrement scod since
		 * this packet won't be transmitted, but the overall scod
		 * resource is still available. This situation should never
		 * happen if flow control works as expected.
		 */
		qs->scod--;
#endif
		SLSI_MUTEX_UNLOCK(qs->cp_lock);
		SLSI_NET_DBG3(dev, SLSI_TX, "scsc_wifi_fcq_transmit_data: Flow control not respected. Packet will be dropped.\n");
		return ERR_BUF;
	}

	SLSI_MUTEX_UNLOCK(qs->cp_lock);

	return 0;
}

static int fcq_receive(struct netif *dev, struct scsc_wifi_fcq_q_header *queue)
{
	queue->qcod--;
	if (queue->qcod < 0) {
		queue->qcod = 0;
		SLSI_NET_DBG3(dev, SLSI_TX, "fcq_receive: qcod is negative. Has been fixed\n");
	}

	return 0;
}

int scsc_wifi_fcq_receive_ctrl(struct netif *dev, struct scsc_wifi_fcq_ctrl_q *queue)
{
	if (WARN_ON(!dev)) {
		return -EINVAL;
	}

	if (WARN_ON(!queue)) {
		return -EINVAL;
	}

	return fcq_receive(dev, &queue->head);
}

int scsc_wifi_fcq_receive_data(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs, u16 priority)
{
	int rc = 0;

	if (WARN_ON(!dev)) {
		return -EINVAL;
	}

	if (WARN_ON(!qs)) {
		return -EINVAL;
	}

	if (WARN_ON(priority >= ARRAY_SIZE(qs->ac_q))) {
		return -EINVAL;
	}

	/* The read/modify/write of the scod here needs synchronisation. */
	SLSI_MUTEX_LOCK(qs->cp_lock);
#ifdef ENABLE_SCOD
	qs->scod--;
	SLSI_NET_DBG3(dev, SLSI_TX, "scod (%d) smod (%d)\n", qs->scod, qs->smod);
	if (qs->scod < 0) {
		qs->scod = 0;
		SLSI_NET_DBG3(dev, SLSI_TX, "scsc_wifi_fcq_receive: scod is negative. Has been fixed\n");
	}
#endif
	rc = fcq_receive(dev, &qs->ac_q[priority].head);

	SLSI_MUTEX_UNLOCK(qs->cp_lock);
	return rc;
}

int scsc_wifi_fcq_update_smod(struct scsc_wifi_fcq_data_qset *qs, enum scsc_wifi_fcq_ps_state peer_ps_state, enum scsc_wifi_fcq_queue_set_type type)
{
	if (WARN_ON(!qs)) {
		return -EINVAL;
	}

	if (peer_ps_state == SCSC_WIFI_FCQ_PS_STATE_POWERSAVE) {
		if (type == SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST) {
			qs->smod = SCSC_WIFI_FCQ_SMOD_POWER;
		} else {
			qs->smod = SCSC_WIFI_FCQ_MCAST_SMOD_POWER;
		}
		qs->peer_ps_state = peer_ps_state;
		qs->peer_ps_state_transitions++;
	} else if (peer_ps_state == SCSC_WIFI_FCQ_PS_STATE_ACTIVE) {
		if (type == SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST) {
			qs->smod = SCSC_WIFI_FCQ_SMOD;
		} else {
			qs->smod = SCSC_WIFI_FCQ_MCAST_SMOD;
		}
		qs->peer_ps_state = peer_ps_state;
		qs->peer_ps_state_transitions++;
	} else {
		SLSI_NET_DBG3(dev, SLSI_TX, "Unknown sta_state %d\n", peer_ps_state);
	}

	return 0;
}

int scsc_wifi_fcq_8021x_port_state(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs, enum scsc_wifi_fcq_8021x_state state)
{
	if (WARN_ON(!dev)) {
		return -EINTR;
	}

	if (WARN_ON(!qs)) {
		return -EINVAL;
	}

	SLSI_MUTEX_LOCK(qs->cp_lock);
	qs->controlled_port_state = state;
	SLSI_MUTEX_UNLOCK(qs->cp_lock);
	SLSI_NET_DBG3(dev, SLSI_TX, "802.1x: Queue set 0x%p is %s\n", qs, state == SCSC_WIFI_FCQ_8021x_STATE_OPEN ? "Open" : "Blocked");
	return 0;
}

/**
 * Statistics
 */
int scsc_wifi_fcq_stat_queue(struct scsc_wifi_fcq_q_header *queue, struct scsc_wifi_fcq_q_stat *queue_stat, int *qmod, int *qcod)
{
	if (WARN_ON(!queue) || WARN_ON(!queue_stat) || WARN_ON(!qmod) || WARN_ON(!qmod)) {
		return -EINTR;
	}

	memcpy(queue_stat, &queue->stats, sizeof(struct scsc_wifi_fcq_q_stat));
	*qmod = queue->qmod;
	*qcod = queue->qcod;
	return 0;
}

int scsc_wifi_fcq_stat_queueset(struct scsc_wifi_fcq_data_qset *queue_set, struct scsc_wifi_fcq_q_stat *queue_stat, int *smod, int *scod, enum scsc_wifi_fcq_8021x_state *cp_state, u32 *peer_ps_state_transitions)
{
	if (WARN_ON(!queue_set) || WARN_ON(!queue_stat) || WARN_ON(!smod) || WARN_ON(!scod) || WARN_ON(!cp_state) || WARN_ON(!peer_ps_state_transitions)) {
		return -EINTR;
	}

	memcpy(queue_stat, &queue_set->stats, sizeof(struct scsc_wifi_fcq_q_stat));
	*peer_ps_state_transitions = queue_set->peer_ps_state_transitions;
	*cp_state = queue_set->controlled_port_state;
	*smod = queue_set->smod;
	*scod = queue_set->scod;
	return 0;
}

/**
 * Queue and Queue Set init/deinit
 */
int scsc_wifi_fcq_ctrl_q_init(struct scsc_wifi_fcq_ctrl_q *queue)
{
	if (WARN_ON(!queue)) {
		return -EINVAL;
	}

	/* Ensure that default qmod doesn't exceed 24 bit */
	if (WARN_ON(SCSC_WIFI_FCQ_QMOD >= 0x1000000)) {
		return -EINVAL;
	}

	queue->head.qmod = SCSC_WIFI_FCQ_QMOD;
	queue->head.qcod = 0;
	queue->head.netif_queue_id = 0;
	queue->head.stats.netq_stops = 0;
	queue->head.stats.netq_resumes = 0;
	queue->head.active = 1;

	return 0;
}

void scsc_wifi_fcq_ctrl_q_deinit(struct scsc_wifi_fcq_ctrl_q *queue)
{
	WARN_ON(!queue);

	if (queue->head.qcod != 0) {
		SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Ctrl queue (0x%p) deinit: qcod is %d, netif queue %d\n", queue, queue->head.qcod, queue->head.netif_queue_id);
	}
}

static int fcq_data_q_init(enum scsc_wifi_fcq_queue_set_type type, struct scsc_wifi_fcq_data_q *queue, struct scsc_wifi_fcq_data_qset *qs, u8 qs_num, s16 ac)
{
	if (WARN_ON(!queue)) {
		return -EINVAL;
	}

	if (WARN_ON(!qs)) {
		return -EINVAL;
	}

	/* Ensure that default qmods don't exceed 24 bit */
	if (WARN_ON(SCSC_WIFI_FCQ_QMOD >= 0x1000000) || WARN_ON(SCSC_WIFI_FCQ_MCAST_QMOD >= 0x1000000)) {
		return -EINVAL;
	}

	if (type == SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST) {
		queue->head.qmod = SCSC_WIFI_FCQ_QMOD;
	} else {
		queue->head.qmod = SCSC_WIFI_FCQ_MCAST_QMOD;
	}

	queue->head.qcod = 0;
	queue->qs = qs;
	queue->head.netif_queue_id = type == SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST ? slsi_netif_get_peer_queue(qs_num, ac) : slsi_netif_get_multicast_queue(ac);
	queue->head.stats.netq_stops = 0;
	queue->head.stats.netq_resumes = 0;
	queue->head.active = 1;

	return 0;
}

static void fcq_data_q_deinit(struct scsc_wifi_fcq_data_q *queue)
{
	WARN_ON(!queue);

	if (queue->head.qcod != 0) {
		SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Data queue (0x%p) deinit: qcod is %d, netif queue %d\n", queue, queue->head.qcod, queue->head.netif_queue_id);
	}
}

static void fcq_qset_init(enum scsc_wifi_fcq_queue_set_type type, struct scsc_wifi_fcq_data_qset *qs, u8 qs_num)
{
	int i;
	struct scsc_wifi_fcq_data_q *queue;

	memset(qs, 0, sizeof(struct scsc_wifi_fcq_data_qset));

	pthread_mutex_init(&qs->cp_lock, NULL);
	if (type == SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST) {
		qs->smod = SCSC_WIFI_FCQ_SMOD;
	} else {
		qs->smod = SCSC_WIFI_FCQ_MCAST_SMOD;
	}
	qs->scod = 0;

	qs->peer_ps_state = SCSC_WIFI_FCQ_PS_STATE_ACTIVE;
	qs->controlled_port_state = SCSC_WIFI_FCQ_8021x_STATE_BLOCKED;

	/* Queues */
	for (i = 0; i < SLSI_NETIF_Q_PER_PEER; i++) {
		queue = &qs->ac_q[i];
		fcq_data_q_init(type, queue, qs, qs_num, i);
	}

#ifdef ENABLE_DYNAMIC_QMOD_ADAPTATION
	/* Give all the remaining slots to BE */
	qs->ac_q[SLSI_TRAFFIC_Q_BE].head.qmod += SCSC_WIFI_FCQ_REMAINING_SLOTS;
	qs->ac_inuse = (1 << SLSI_TRAFFIC_Q_BE);
#endif
}

int scsc_wifi_fcq_unicast_qset_init(struct scsc_wifi_fcq_data_qset *qs, u8 qs_num)
{
	if (WARN_ON(!qs)) {
		return -EINVAL;
	}

	/* Ensure that default smod doesn't exceed 24 bit */
	if (WARN_ON(SCSC_WIFI_FCQ_SMOD >= 0x1000000)) {
		return -EINVAL;
	}

	SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Init unicast queue set 0x%p\n", qs);
	fcq_qset_init(SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST, qs, qs_num);
	SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Unicast queue set inited\n");
	return 0;
}

int scsc_wifi_fcq_multicast_qset_init(struct scsc_wifi_fcq_data_qset *qs)
{
	if (WARN_ON(!qs)) {
		return -EINVAL;
	}

	/* Ensure that default smod doesn't exceed 24 bit */
	if (WARN_ON(SCSC_WIFI_FCQ_MCAST_SMOD >= 0x1000000)) {
		return -EINVAL;
	}

	SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Init multicast queue set 0x%p\n", qs);
	fcq_qset_init(SCSC_WIFI_FCQ_QUEUE_SET_TYPE_MULTICAST, qs, 0);
	SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Multicast queue set inited\n");
	return 0;
}

void scsc_wifi_fcq_qset_deinit(struct scsc_wifi_fcq_data_qset *qs)
{
	int i;

	WARN_ON(!qs);

	if (qs->scod != 0) {
		SLSI_DBG1_NODEV(SLSI_WIFI_FCQ, "Data set (0x%p) deinit: scod is %d\n", qs, qs->scod);
	}

	for (i = 0; i < SLSI_NETIF_Q_PER_PEER; i++) {
		fcq_data_q_deinit(&qs->ac_q[i]);
	}

	work_cancel(SCSC_WORK, &qs->fcq_work);
}
