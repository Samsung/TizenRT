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
#ifndef __SLSI_STATS_H__
#define __SLSI_STATS_H__
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
#include "misc/fw_panic_record.h"
#endif

#define SLSI_WLAN_STATS_MAGIC_NUM       (12345678)

#ifndef SLSI_NETIF_Q_PER_PEER
#define SLSI_NETIF_Q_PER_PEER           (4)
#endif

#ifndef ETH_ALEN
#define ETH_ALEN                        (6)
#endif

#define SLSI_PEER_VALID                 (0)
#define SLSI_PEER_INVALID               (1)

struct data_path_stats {
	unsigned int magic_number;	/* This is added just to make sure the stats are correct */
	unsigned int tx_linkoutput_packets;
	unsigned int tx_success_packet;
	unsigned int tx_data_hip;
	unsigned int tx_done;
	unsigned int tx_arp_dhcp_mlme;
	unsigned int tx_drop_vif_not_available;
	unsigned int tx_drop_invalid_pbuf;
	unsigned int tx_drop_null_mbuf;
	unsigned int tx_drop_large_pbuf;
	unsigned int tx_drop_wrong_length;
	unsigned int tx_drop_discard_queue;
	unsigned int tx_drop_test_mode_enabled;
	unsigned int tx_drop_vif_not_active;
	unsigned int tx_drop_no_sta_connected;
	unsigned int tx_drop_eapol;
	unsigned int tx_drop_dhcp;
	unsigned int tx_drop_arp;
	unsigned int tx_drop_headroom;
	unsigned int tx_drop_flow_control_mcast;
	unsigned int tx_drop_no_peer_record;
	unsigned int tx_drop_port_blocked;
	unsigned int tx_drop_flow_control_be;
	unsigned int tx_drop_flow_control_bk;
	unsigned int tx_drop_flow_control_vi;
	unsigned int tx_drop_flow_control_vo;
	unsigned int tx_drop_hip4_invalid_param;
	unsigned int tx_drop_hip4_queue_full;
	unsigned int tx_drop_mifptr_to_addr_fail;
	unsigned int tx_drop_fail_to_add_in_hipq;
	unsigned int tx_failed_tx_lifetime;
	unsigned int tx_failed_retry_limit;
	unsigned int tx_failed_cfm_err;

	unsigned int rx_num_interrupts;
	unsigned int rx_num_hip_wq;
	unsigned int rx_num_hip_rx_data_pkts;
	unsigned int rx_num_unit_data_ind;
	unsigned int rx_num_mlme_dhcp;
	unsigned int rx_num_amsdu;
	unsigned int rx_num_msdu_in_amsdu;
	unsigned int rx_num_packets_given_to_lwip;
	unsigned int rx_num_eapol;
	unsigned int rx_num_ba_scroll_window;
	unsigned int rx_mbuf_q_max_len;
	unsigned int rx_drop_mem_alloc_failed;
	unsigned int rx_drop_wrong_signal_id;
	unsigned int rx_drop_unsupported_frame;
	unsigned int rx_drop_peer_not_present;
	unsigned int rx_drop_large_frame;
	unsigned int rx_drop_ba_out_of_order_pkt;
	unsigned int rx_drop_ba_duplicate_pkt;
	unsigned int rx_drop_ba_stop;
	unsigned int rx_drop_mbuf_q_full;

	unsigned int host_triggered_panic;
	unsigned int fw_triggered_panic;
	unsigned int force_panic;
	unsigned int hanged_event;

	int tx_ba;
	int rx_ba;

	int hip4_th_data_idx_r;
	int hip4_th_data_idx_w;
	int hip4_th_ctrl_idx_r;
	int hip4_th_ctrl_idx_w;
	int hip4_th_rfb_idx_r;
	int hip4_th_rfb_idx_w;

	int hip4_fh_data_idx_r;
	int hip4_fh_data_idx_w;
	int hip4_fh_ctrl_idx_r;
	int hip4_fh_ctrl_idx_w;
	int hip4_fh_rfb_idx_r;
	int hip4_fh_rfb_idx_w;

	int mbulk_free_data;
	int mbulk_free_ctrl;
	int ac_q_update_status;
	int ac_q_stats_qcod[SLSI_NETIF_Q_PER_PEER];
	int ac_q_stats_qmod[SLSI_NETIF_Q_PER_PEER];

	struct slsi_dev *sdev;
	u8 peer_mac[ETH_ALEN];
#ifdef CONFIG_SCSC_WLAN_AUTO_RECOVERY
	struct panic_record_stats r4_panic_record_stats[SCSC_NUM_PANIC_RECORD];
	int r4_panic_record_offset;
#endif
};

struct slsi_dev;
void slsi_init_dath_path_stats(struct slsi_dev *sdev);
void *slsi_get_dath_path_stats(void);
void hip4_update_queue_stats(struct slsi_dev *sdev);
void mbulk_pool_stats(struct slsi_dev *sdev);
#endif
