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

#ifndef __SLSI_HIP_H__
#define __SLSI_HIP_H__
#include <sys/types.h>
#include <pthread.h>
struct slsi_dev;

/* This structure describes the chip and HIP core lib
 * information that exposed to the OS layer.
 */
struct slsi_card_info {
	u16 chip_id;
	u32 fw_build;
	u16 fw_hip_version;
	u32 sdio_block_size;
};

/* HIP States:
 *     STOPPED  : (default) state, avoid running the HIP
 *     STARTING : HIP is being initialised, avoid running the HIP
 *     STARTED  : HIP cycles can run
 *     STOPPING : HIP is being de-initialised, avoid running the HIP
 *     BLOCKED  : HIP TX CMD53 failure or WLAN subsystem crashed indication from Hydra,
 *                                                                avoid running the HIP
 */
enum slsi_hip_state {
	SLSI_HIP_STATE_STOPPED,
	SLSI_HIP_STATE_STARTING,
	SLSI_HIP_STATE_STARTED,
	SLSI_HIP_STATE_STOPPING,
	SLSI_HIP_STATE_BLOCKED
};

struct slsi_hip {
	struct slsi_dev *sdev;
	struct slsi_card_info card_info;
	/* a std mutex */
	pthread_mutex_t hip_mutex;

	/* refer to enum slsi_hip_state */
	int hip_state;
};

#define SLSI_PS_MAX_TID_PRI 8

#define SLSI_HIP_PARAM_SLOT_COUNT 2

/****************************** Configuration ********************************/
struct slsi_hip_card_params {
	u32 zero_copy;
	u32 traffic_qlen;
	u32 th_max_slots;
	u32 fh_max_slots;
	u32 led_mask;
	u32 poll_period;
	u32 tx_window_segment_size;
	u32 tx_force_pull_mode;
	s32 mparam_slot_count[SLSI_HIP_PARAM_SLOT_COUNT];
	u32 minVifDurationEapol;
	u32 minVifDurationDhcp;
	u32 minVifDurationArp;
	u16 minVifDurationPacketFilter;
};

/***************************** Stats and debug *******************************/
#define SLSI_HIP2_STATS_PKTS_PER_CYCLE_MAX 10

/* Total number of FH buffers */
#define SLSI_HIP_FH_BUF_MIN_INDEX 0
#define SLSI_HIP_FH_BUF_MAX       2

/* configs shared with firwmare */
struct slsi_hip_shared_configs {
	u16 magic;
	u16 init_id;
	u16 ctrl_h;
	u16 ctrl_poll_h;

	u16 fh_h[SLSI_HIP_FH_BUF_MAX];

	u16 th_sig_h;
	u16 th_bulk_h;
	u32 ctrl_buf_sz;

	u32 fh_buf_sz[SLSI_HIP_FH_BUF_MAX];

	u32 th_sig_buf_sz;
	u32 th_bulk_buf_sz;
	u16 ext_buf_h[3];
	u32 ext_buf_sz[3];
};

struct slsi_hip_fh_buf_configs {
	u32 seg_size;
	u32 tcb_min_room;
	u16 tcb_max_retries;
	u16 proc_segs;
};

/* Driver's HIP parameters */
struct slsi_hip_drv_configs {
	u8 fh_buf_i;
	struct slsi_hip_fh_buf_configs fh_c[SLSI_HIP_FH_BUF_MAX];
};

struct slsi_proc_hip_configs {
	struct slsi_hip_drv_configs drv_c;
	struct slsi_hip_shared_configs shared_c;
};

struct slsi_hip_fh_stats {
	/* Values specific to a particular FH buffer */
	u32 segs;
	u32 room[16];
	u32 writes[16];
	u32 tcb_reads;
	u32 tcb_failsafe;
	u32 no_room;
	u32 none;
	u32 sigdata;
	u32 bytes;
	u32 pkts[5];
#ifdef CONFIG_SCSC_WLAN_HIP_SUPPORT_SCATTER_GATHER_API
	u32 sg_pad;
	u32 sg_mcount;
	u32 sg_per_cmd53[3];
	u32 sg_limit;
#endif
};

int slsi_hip_init(struct slsi_dev *sdev);
void slsi_hip_deinit(struct slsi_dev *sdev);

int slsi_hip_start(struct slsi_dev *sdev, struct slsi_hip_card_params *hip_param);
int slsi_hip_setup(struct slsi_dev *sdev);
int slsi_hip_stop(struct slsi_dev *sdev);
bool slsi_hip_is_sdio_ready(struct slsi_dev *sdev);
int slsi_wlan_clear_interrupt(struct slsi_dev *sdev);
void slsi_hydra_hip_run(struct slsi_hip *priv);

/* Forward declaration */
struct sap_api;
struct max_buff;

/* Register SAP with HIP layer */
int slsi_hip_sap_register(struct sap_api *sap_api);
/* Unregister SAP with HIP layer */
int slsi_hip_sap_unregister(struct sap_api *sap_api);
/* SAP rx proxy */
int slsi_hip_rx(struct slsi_dev *sdev, struct max_buff *mbuf);
/* SAP setup once we receive SAP versions */
int slsi_hip_sap_setup(struct slsi_dev *sdev);
/* Allow the SAP to act on a buffer in the free list. */
int slsi_hip_tx_done(struct slsi_dev *sdev, u16 colour);

#endif
