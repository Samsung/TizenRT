/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DIAGNOSE_FORMAT_H_
#define _AMEBA_DIAGNOSE_FORMAT_H_

#include "ameba_soc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************************
 *                                   Protocols
 ****************************************************************************************************/

#define RTK_DIAG_FRAME_HEADER ((u16)('r' | ('t' << 8))) //header of RtkDiagDataFrame_t.header

struct tagRtkDiagDataFrame {
	u16 header;
	u16 size; //as spec: == sizeof(cmd) + lengthof(payload) - sizeof(crc)
	u8 cmd;
	u8 payload[0]; //WARNING: Here payload includes crc's one byte(last byte), different from spec
	//1. for event req/res: payload is offset(u16) + event + crc, so size is: 2 + event + 1
	//2. for others: depends cmd
} __PACKED;
typedef struct tagRtkDiagDataFrame RtkDiagDataFrame_t;

// Event structure
struct tagRtkDiagEvent {
	u16 evt_type;
	u16 evt_len;    //length of evt_info
	u32 evt_time;
	u8 evt_level;
	u8 rsvd[3];
	u8 evt_info[0];  // Flexible array member for event data
} __PACKED;
typedef struct tagRtkDiagEvent RtkDiagEvent_t;

enum rtk_event_level {
	RTK_EVENT_LEVEL_ALWAYS = 0,
	RTK_EVENT_LEVEL_ERROR,
	RTK_EVENT_LEVEL_WARNING,
	RTK_EVENT_LEVEL_INFO,
	RTK_EVENT_LEVEL_DEBUG,
};

enum rtk_event_type {
	RTK_EVENT_TYPE_INTERNAL = 0,
	//soc module continue add
	RTK_EVENT_TYPE_DBG_DEMO_1,
	RTK_EVENT_TYPE_DBG_DEMO_2,

	RTK_EVENT_TYPE_WIFI_BASE = 40000, // wifi event in [40000, 50000-1]
	//wifi module continue add

	RTK_EVENT_TYPE_CUSTOMER = 50000,
	//customer add

	RTK_EVENT_TYPE_MAX  = 60000,
};

/*****************************************************************************************************
 *                                   Events Define
 ****************************************************************************************************/

/******************* INTERNAL **********************/

struct diag_evt_demo_1 {
	u8 u8_data;
	u16 u16_data;
	float f_data;
	char ch_data;
	char str_data[32];
} __PACKED;

struct diag_evt_demo_2 {
	struct diag_evt_demo_1 dm1_data;
	int i_data;
} __PACKED;

/********************* WIFI ************************/
enum diag_evt_wifi {
	DIAG_EVT_WIFI_DISCONN = RTK_EVENT_TYPE_WIFI_BASE + 0x01,
	DIAG_EVT_WIFI_NO_RX = RTK_EVENT_TYPE_WIFI_BASE + 0x02,
	DIAG_EVT_WIFI_NO_TX = RTK_EVENT_TYPE_WIFI_BASE + 0x03,
	DIAG_EVT_WIFI_JOIN_FAIL = RTK_EVENT_TYPE_WIFI_BASE + 0x04,
};

struct diag_evt_wifi_disconn { /*DIAG_EVT_WIFI_DISCONN*/
	u16 reason;  /* <!-- DIAG: --> enum rtw_disconn_reason in wifi_api_types.h*/
} __PACKED;

struct diag_evt_wifi_join_fail { /*DIAG_EVT_WIFI_JOIN_FAIL*/
	u16 reason;  /* <!-- DIAG: --> enum rtk_error_code in rtk_status.h*/
	u16 reason_code;
} __PACKED;

struct diag_evt_wifi_rxbd_t {
	u8 sw_index : 4;
	u8 hw_index : 4;
} __PACKED;

struct diag_evt_wifi_no_rx { /*DIAG_EVT_WIFI_NO_RX*/
	u8 bb_cca_cck[5];
	u8 bb_cca_ofdm[5];
	u8 bb_rx_cck[5];
	u8 bb_rx_ofdm[5];
	u8 mac_rx_cck[5];
	u8 mac_rx_ofdm[5];
	struct diag_evt_wifi_rxbd_t rxbd[5];
	u8 cca_th;
} __PACKED;


/******************* CUSTOMER **********************/

#ifdef __cplusplus
}
#endif

#endif