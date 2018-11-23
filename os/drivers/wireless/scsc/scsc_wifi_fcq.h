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

#include <sys/types.h>

#include "netif.h"

#ifndef __SCSC_WIFI_FCQ_H
#define __SCSC_WIFI_FCQ_H

/* Setting ENABLE_SCOD will include a station specific flow control
 * #define ENABLE_SCOD 1
 */

/* Setting ENABLE_DYNAMIC_QMOD_ADAPTATION allows the dynamic distribution of qmod */
#define ENABLE_DYNAMIC_QMOD_ADAPTATION

/* Initial value of unicast smod - peer normal */
#define SCSC_WIFI_FCQ_SMOD              (56)

/* Initial value of multicast smod - peer normal */
#define SCSC_WIFI_FCQ_MCAST_SMOD        (4)

/* Initial powersave SMOD value - peer powersave */
#define SCSC_WIFI_FCQ_SMOD_POWER        (4)

/* Initial value of powersave multicast smod - peer normal */
#define SCSC_WIFI_FCQ_MCAST_SMOD_POWER  (4)

/* Initial value of unicast qmod - peer normal */
#ifdef ENABLE_DYNAMIC_QMOD_ADAPTATION
#define SCSC_WIFI_FCQ_QMOD            (2)
#else
#define SCSC_WIFI_FCQ_QMOD            (14)
#endif

/* Remaining slots for qmod */
#define SCSC_WIFI_FCQ_REMAINING_SLOTS   (48)

/* Initial value of multicast qmod - peer normal */
#define SCSC_WIFI_FCQ_MCAST_QMOD        (3)

#define SCSC_FCQ_REDISTRIBUTE_TIME      (5000)	/* 5 seconds */

enum scsc_wifi_fcq_8021x_state {
	SCSC_WIFI_FCQ_8021x_STATE_BLOCKED = 0,
	SCSC_WIFI_FCQ_8021x_STATE_OPEN = 1
};

enum scsc_wifi_fcq_ps_state {
	SCSC_WIFI_FCQ_PS_STATE_POWERSAVE = 1,
	SCSC_WIFI_FCQ_PS_STATE_ACTIVE = 2
};

struct scsc_wifi_fcq_q_stat {
	u32 netq_stops;
	u32 netq_resumes;
};

struct scsc_wifi_fcq_q_header {
	int qmod;
	int qcod;
	u16 netif_queue_id;
	int active;
	struct scsc_wifi_fcq_q_stat stats;
};

struct scsc_wifi_fcq_data_q {
	struct scsc_wifi_fcq_q_header head;
	struct scsc_wifi_fcq_data_qset *qs;
};

struct scsc_wifi_fcq_ctrl_q {
	struct scsc_wifi_fcq_q_header head;
};

enum scsc_wifi_fcq_queue_set_type {
	SCSC_WIFI_FCQ_QUEUE_SET_TYPE_UNICAST = 0,
	SCSC_WIFI_FCQ_QUEUE_SET_TYPE_MULTICAST = 1
};

struct scsc_wifi_fcq_data_qset {
	enum scsc_wifi_fcq_8021x_state controlled_port_state;
	pthread_mutex_t cp_lock;
	struct scsc_wifi_fcq_data_q ac_q[SLSI_NETIF_Q_PER_PEER];
	int smod;
	int scod;
	/* Control AC usage (BE,BK,VI,VO) bitmap */
	u8 ac_inuse;
	u8 ac_active;				/* Current active ACs */
	struct scsc_wifi_fcq_q_stat stats;	/* Stats for smod */
	enum scsc_wifi_fcq_ps_state peer_ps_state;
	u32 peer_ps_state_transitions;
	struct work_s fcq_work;
};

bool slsi_is_port_blocked(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs);
/* Queue and queue set management */
int scsc_wifi_fcq_ctrl_q_init(struct scsc_wifi_fcq_ctrl_q *queue);
void scsc_wifi_fcq_ctrl_q_deinit(struct scsc_wifi_fcq_ctrl_q *queue);

int scsc_wifi_fcq_unicast_qset_init(struct scsc_wifi_fcq_data_qset *qs, u8 qs_num);
int scsc_wifi_fcq_multicast_qset_init(struct scsc_wifi_fcq_data_qset *qs);
void scsc_wifi_fcq_qset_deinit(struct scsc_wifi_fcq_data_qset *qs);	/* Used for deinit both uni and multicast queue sets */

/* Transmit/receive bookkeeping and smod power save changes / 802.1x handling */
int scsc_wifi_fcq_transmit_data(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs, u16 priority, bool multicast);
int scsc_wifi_fcq_receive_data(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs, u16 priority);

int scsc_wifi_fcq_receive_ctrl(struct netif *dev, struct scsc_wifi_fcq_ctrl_q *queue);

int scsc_wifi_fcq_update_smod(struct scsc_wifi_fcq_data_qset *qs, enum scsc_wifi_fcq_ps_state peer_ps_state, enum scsc_wifi_fcq_queue_set_type type);
int scsc_wifi_fcq_8021x_port_state(struct netif *dev, struct scsc_wifi_fcq_data_qset *qs, enum scsc_wifi_fcq_8021x_state state);

/* Statistics */
int scsc_wifi_fcq_stat_queue(struct scsc_wifi_fcq_q_header *queue, struct scsc_wifi_fcq_q_stat *queue_stat, int *qmod, int *qcod);

int scsc_wifi_fcq_stat_queueset(struct scsc_wifi_fcq_data_qset *queue_set, struct scsc_wifi_fcq_q_stat *queue_stat, int *smod, int *scod, enum scsc_wifi_fcq_8021x_state *cp_state, u32 *peer_ps_state_transitions);

#endif							/* #ifndef __SCSC_WIFI_FCQ_H */
