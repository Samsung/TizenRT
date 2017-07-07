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

#ifndef __HIP4_H__
#define __HIP4_H__

/**
 * This header file is the public HIP4 interface, which will be accessible by
 * Wi-Fi service driver components.
 *
 * All struct and internal HIP functions shall be moved to a private header
 * file.
 */

#include <scsc/scsc_mifram.h>
#include <scsc/scsc_mx.h>
#include "mbulk.h"
#include <sys/types.h>
#include "utils_scsc.h"
#include <tinyara/wqueue.h>
#include "max_buf.h"

#define MIF_HIP_COMPAT_FLAG_NEED_MLME_RESET     (1 << 0)
#define MIF_HIP_COMPAT_FLAG_MIB_DAT_BY_FAPI     (1 << 1)

#define HIP4_SLOTS_SIZE                        (2048)
#define HIP4_DAT_SLOTS                         (IMG_MGR_SEC_WLAN_TX_DAT_SIZE / HIP4_SLOTS_SIZE)
#define HIP4_CTL_SLOTS                         (IMG_MGR_SEC_WLAN_TX_CTL_SIZE / HIP4_SLOTS_SIZE)

#define MIF_HIP_CFG_Q_NUM       6

/* Current versions supported by this HIP */
#define HIP4_SUPPORTED_V1       3
#define HIP4_SUPPORTED_V2       4

enum hip4_hip_q_conf {
	HIP4_MIF_Q_FH_CTRL,
	HIP4_MIF_Q_FH_DAT,
	HIP4_MIF_Q_FH_RFB,
	HIP4_MIF_Q_TH_CTRL,
	HIP4_MIF_Q_TH_DAT,
	HIP4_MIF_Q_TH_RFB
};

struct hip4_hip_config_version_4 {
	u32 magic_number;
	u16 hip_config_ver;			/* Version of this configuration structure = 2 */
	u16 config_len;				/* Size of this configuration structure */

	/* FW owned */
	u32 compat_flag;			/* flag of the expected driver's behaviours */

	u16 sap_mlme_ver;			/* Fapi SAP_MLME version */
	u16 sap_ma_ver;				/* Fapi SAP_MA version */
	u16 sap_debug_ver;			/* Fapi SAP_DEBUG version */
	u16 sap_test_ver;			/* Fapi SAP_TEST version */

	u32 fw_build_id;			/* Firmware Build Id */
	u32 fw_patch_id;			/* Firmware Patch Id */

	u8 unidat_req_headroom;
	u8 unidat_req_tailroom;
	u8 bulk_buffer_align;
	u8 host_cache_line;

	u32 host_buf_loc;			/* location of the host buffer */
	u32 host_buf_sz;			/* in byte, size of the host buffer */
	u32 fw_buf_loc;				/* location of the firmware buffer */
	u32 fw_buf_sz;				/* in byte, size of the firmware buffer */
	u32 mib_loc;				/* MIB location in MIF_ADDR */
	u32 mib_sz;					/* MIB size */
	u32 log_config_loc;			/* Logging Configuration Location in MIF_ADDR */
	u32 log_config_sz;			/* Logging Configuration Size in MIF_ADDR */

	u8 mif_fh_int_n;			/* MIF from-host interrupt bit position */
	u8 mif_th_int_n;			/* MIF to-host interrpt bit position */
	u8 reserved[2];

	u32 scbrd_loc;

	u16 q_num;
	u16 q_len;
	u16 q_idx_sz;
	u8 reserved2[2];

	u32 q_loc[MIF_HIP_CFG_Q_NUM];

	u8 reserved3[16];
} STRUCT_PACKED;

struct hip4_hip_config_version_3 {
	/* Host owned */
	u32 magic_number;			/* 0xcaba0401 */
	u16 hip_config_ver;			/* Version of this configuration structure = 2 */
	u16 config_len;				/* Size of this configuration structure */

	/* FW owned */
	u32 compat_flag;			/* flag of the expected driver's behaviours */

	u16 sap_mlme_ver;			/* Fapi SAP_MLME version */
	u16 sap_ma_ver;				/* Fapi SAP_MA version */
	u16 sap_debug_ver;			/* Fapi SAP_DEBUG version */
	u16 sap_test_ver;			/* Fapi SAP_TEST version */

	u32 fw_build_id;			/* Firmware Build Id */
	u32 fw_patch_id;			/* Firmware Patch Id */

	u8 unidat_req_headroom;		/* Headroom the host shall reserve in mbulk for MA-UNITDATA.REQ signal */
	u8 unidat_req_tailroom;		/* Tailroom the host shall reserve in mbulk for MA-UNITDATA.REQ signal */
	u8 bulk_buffer_align;		/* 4 */

	/* Host owned */
	u8 host_cache_line;			/* 64 */

	u32 host_buf_loc;			/* location of the host buffer in MIF_ADDR */
	u32 host_buf_sz;			/* in byte, size of the host buffer */
	u32 fw_buf_loc;				/* location of the firmware buffer in MIF_ADDR */
	u32 fw_buf_sz;				/* in byte, size of the firmware buffer */
	u32 mib_loc;				/* MIB location in MIF_ADDR */
	u32 mib_sz;					/* MIB size */
	u32 log_config_loc;			/* Logging Configuration Location in MIF_ADDR */
	u32 log_config_sz;			/* Logging Configuration Size in MIF_ADDR */

	u8 mif_fh_int_n;			/* MIF from-host interrupt bit position */
	u8 mif_th_int_n;			/* MIF to-host interrpt bit position */
	u8 reserved[2];

	u32 scbrd_loc;				/* Scoreboard locatin in MIF_ADDR */

	u16 q_num;					/* 6 */
	u16 q_len;					/* 256 */
	u16 q_idx_sz;				/* 1 */
	u8 reserved2[2];

	u32 q_loc[MIF_HIP_CFG_Q_NUM];

	u8 reserved3[16];
} STRUCT_PACKED;

struct hip4_hip_init {
	/* Host owned */
	u32 magic_number;			/* 0xcaaa0400 */
	/* FW owned */
	u32 conf_hip4_ver;
	/* Host owned */
	u32 version_a_ref;			/* Location of Config structure A (old) */
	u32 version_b_ref;			/* Location of Config structure B (new) */
} STRUCT_PACKED;

#define MAX_NUM 256
struct hip4_hip_q {
	u32 array[MAX_NUM];
	u8 idx_read;				/* To keep track */
	u8 idx_write;				/* To keep track */
	u8 total;
} __aligned(64);

struct hip4_hip_control {
	struct hip4_hip_init init;
	struct hip4_hip_config_version_3 config_v3 __aligned(32);
	struct hip4_hip_config_version_4 config_v4 __aligned(32);
	u32 scoreboard[256] __aligned(64);
	struct hip4_hip_q q[MIF_HIP_CFG_Q_NUM] __aligned(64);
} __aligned(4096);

struct hip_q_index {
	u8 read;
	u8 write;
} STRUCT_PACKED;

struct hip_q_indice {
	struct hip_q_index q[6];
} STRUCT_PACKED;

struct slsi_hip4;

/* This struct is private to the HIP implementation */
struct hip4_priv {
	volatile struct hip_q_indice *q_indice;
	struct work_s intr_wq;
	/* Interrupts cache */
	/* TOHOST */
	u32 rx_intr_tohost;
	/* FROMHOST */
	u32 rx_intr_fromhost;

	/* For workqueue */
	struct slsi_hip4 *hip;

	/* Pool for data frames */
	u8 host_pool_id_dat;
	/* Pool for ctl frames */
	u8 host_pool_id_ctl;

	/* Device lock */
	pthread_mutex_t rx_lock;

	/* Scoreboard update mutex lock */
	pthread_mutex_t rw_scoreboard;

	/* Control the hip4 deinit */
	int closing;

	u8 unidat_req_headroom;		/* Headroom the host shall reserve in mbulk for MA-UNITDATA.REQ signal */
	u8 unidat_req_tailroom;		/* Tailroom the host shall reserve in mbulk for MA-UNITDATA.REQ signal */
	u32 version;				/* Version of the running FW */
	void *scbrd_base;			/* Scbrd_base pointer */

#ifdef CONFIG_ARCH_CHIP_S5JT200
	/* Allocated memory */
	scsc_mifram_ref hip_control;
	scsc_mifram_ref from_host_pool;
	scsc_mifram_ref to_host_pool;
#endif
};

struct scsc_service;

struct slsi_hip4 {
	struct hip4_priv *hip_priv;
	struct hip4_hip_control *hip_control;
};

/* Public functions */
int hip4_init(struct slsi_hip4 *hip);
int hip4_setup(struct slsi_hip4 *hip);
void hip4_freeze(struct slsi_hip4 *hip);
void hip4_deinit(struct slsi_hip4 *hip);

int scsc_wifi_transmit_frame(struct slsi_hip4 *hip, bool ctrl_packet, struct max_buff *mbuf);

/* Macros for accessing information stored in the hip_config struct */
#define scsc_wifi_get_hip_config_version_4_u8(buff_ptr, member) le16_to_cpu((((struct hip4_hip_config_version_4 *)(buff_ptr))->member))
#define scsc_wifi_get_hip_config_version_4_u16(buff_ptr, member) le16_to_cpu((((struct hip4_hip_config_version_4 *)(buff_ptr))->member))
#define scsc_wifi_get_hip_config_version_4_u32(buff_ptr, member) le32_to_cpu((((struct hip4_hip_config_version_4 *)(buff_ptr))->member))
#define scsc_wifi_get_hip_config_version_3_u8(buff_ptr, member) le16_to_cpu((((struct hip4_hip_config_version_3 *)(buff_ptr))->member))
#define scsc_wifi_get_hip_config_version_3_u16(buff_ptr, member) le16_to_cpu((((struct hip4_hip_config_version_3 *)(buff_ptr))->member))
#define scsc_wifi_get_hip_config_version_3_u32(buff_ptr, member) le32_to_cpu((((struct hip4_hip_config_version_3 *)(buff_ptr))->member))
#define scsc_wifi_get_hip_config_u8(buff_ptr, member, ver) le16_to_cpu((((struct hip4_hip_config_version_##ver *)(buff_ptr->config_v##ver))->member))
#define scsc_wifi_get_hip_config_u16(buff_ptr, member, ver) le16_to_cpu((((struct hip4_hip_config_version_##ver *)(buff_ptr->config_v##ver))->member))
#define scsc_wifi_get_hip_config_u32(buff_ptr, member, ver) le32_to_cpu((((struct hip4_hip_config_version_##ver *)(buff_ptr->config_v##ver))->member))
#define scsc_wifi_get_hip_config_version(buff_ptr) le32_to_cpu((((struct hip4_hip_init *)(buff_ptr))->conf_hip4_ver))

#define QR(hip_q, qname)       ((hip_q)->q_indice->q[qname].read)
#define QW(hip_q, qname)       ((hip_q)->q_indice->q[qname].write)

#endif
