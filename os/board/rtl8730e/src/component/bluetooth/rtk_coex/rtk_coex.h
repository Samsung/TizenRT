/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _RTK_COEX_H_
#define _RTK_COEX_H_

#include <dlist.h>

#define HCI_NONE    0x00
#define HCI_CMD     0x01
#define HCI_ACL     0x02
#define HCI_SCO     0x03
#define HCI_EVT     0x04
#define HCI_ISO     0x05

#define ACL_CONN     0x0
#define SYNC_CONN    0x1
#define LE_CONN      0x2

#define DIR_IN     0
#define DIR_OUT    1

#define A2DP_SIGNAL    0x01
#define A2DP_MEDIA     0x02

#define BT_COEX_MONITOR_INTERVAL    1000
#define BT_LE_BUSY_CONN_INTERVAL    0x10    //20ms, units: 1.25ms
#define BT_HID_BUSY_INTERVAL        60  //units: 1.25ms

#define L2CAP_CONN_REQ       0x02
#define L2CAP_CONN_RSP       0x03
#define L2CAP_DISCONN_REQ    0x06
#define L2CAP_DISCONN_RSP    0x07

#define HCI_VENDOR_SET_PROFILE_REPORT_COMMAND    0xFC1B
#define HCI_VENDOR_SET_BITPOOL                   0xFC51
#define HCI_VENDOR_MAILBOX_CMD                   0xFC8F

#define HCI_EV_CONN_COMPLETE                0x03
#define HCI_EV_CMD_COMPLETE                 0x0E
#define HCI_EV_SYNC_CONN_COMPLETE           0x2c
#define HCI_EV_DISCONN_COMPLETE             0x05
#define HCI_EV_MODE_CHANGE                  0x14
#define HCI_EV_LE_META                      0x3e
#define HCI_EV_LE_CONN_COMPLETE             0x01
#define HCI_EV_LE_ENHANCED_CONN_COMPLETE    0x0a
#define HCI_EV_LE_CONN_UPDATE_COMPLETE      0x03
#define HCI_EV_LE_CIS_EST                   0x19
#define HCI_EV_LE_CREATE_BIG_CPL            0x1b
#define HCI_EV_LE_TERM_BIG_CPL              0x1c
#define HCI_EV_LE_BIG_SYNC_EST              0x1d
#define HCI_EV_LE_BIG_SYNC_LST              0x1e

#define HCI_EV_VENDOR_SPECIFIC              0xFF

#define PSM_SDP     0x0001
#define PSM_RFCOMM  0x0003
#define PSM_PAN     0x000F
#define PSM_HID     0x0011
#define PSM_HID_INT 0x0013
#define PSM_AVCTP   0x0017
#define PSM_AVDTP   0x0019
#define PSM_FTP     0x1001
#define PSM_BIP     0x1003
#define PSM_OPP     0x1005

#define CONFIG_BT_COEX_DEBUG 0

#if defined(CONFIG_BT_COEX_DEBUG) && CONFIG_BT_COEX_DEBUG
#include "bt_debug.h"
#define DBG_BT_COEX(...) BT_LOGA("[BT_COEX] " __VA_ARGS__)
#define DBG_BT_COEX_DUMP(_str, _buf, _len) BT_DUMPA("[BT_COEX] " _str, _buf, _len)
#else
#define DBG_BT_COEX(...) {}
#define DBG_BT_COEX_DUMP(_str, _buf, _len) {}
#endif /* CONFIG_BT_COEX_DEBUG */


#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
// defined by https://wiki.realtek.com/display/CN3SD9R/Ameba+Coex+MailBox
// for mailbox_id = 0x27
struct rtk_coex_b2w_sw_mailbox_bt_info_report_by_itself {
	// lb0
	uint8_t id;
	// lb1
	uint8_t len;
	// lb2
	uint8_t connect: 1;
	uint8_t SCOeSCO: 1;
	uint8_t inqpage: 1;
	uint8_t aclbusy: 1;
	uint8_t scobusy: 1;
	uint8_t hid: 1;
	uint8_t a2dp: 1;
	uint8_t pan_opp_ftp: 1;
	// lb3
	uint8_t retry_cnt: 4;
	uint8_t bt_fix_2M: 1;
	uint8_t inquiry: 1;
	uint8_t bt_mesh: 1;
	uint8_t page: 1;
	// hb0
	uint8_t rssi;
	// hb1
	uint8_t le_link: 1;
	uint8_t re_link: 1;
	uint8_t setup_link: 1;
	uint8_t ignore_wlan_act: 1;
	uint8_t voice: 1;
	uint8_t ble_scan_en: 1;
	uint8_t role_switch: 1;
	uint8_t multi_link: 1;
	// hb2
	uint8_t opp_exist: 1;
	uint8_t afh_map_update: 1;
	uint8_t a2dp_active: 1;
	uint8_t legacy_slave: 1;
	uint8_t hid_slot: 2;
	uint8_t hid_pair_num: 2;
	//hb3
	uint8_t rsvd: 5;
	uint8_t non_hid_sniff_link: 1;
	uint8_t packet_3M: 1;
	uint8_t a2dp_sink: 1;
};
// for mailbox_id = 0x30
union rtk_coex_b2w_sw_mailbox_bt_mp_report {
	// subid = 0x2D
	struct {
		// lb0
		uint8_t id;
		// lb1
		uint8_t seq;
		// lb2
		uint8_t subid;
		// lb3
		uint8_t status: 4;
		uint8_t len: 4;
		// hb0
		uint8_t le_bg_scan: 1;
		uint8_t le_init_scan: 1;
		uint8_t le_scan: 1;
		uint8_t rsvd: 5;
		// hb1-hb3
		uint8_t rsvd_hb[3];
	} rtk_coex_b2w_sw_mailbox_le_scan_type;
	// subid = 0x2E
	struct {
		// lb0
		uint8_t id;
		// lb1
		uint8_t seq;
		// lb2
		uint8_t subid;
		// lb3
		uint8_t status: 4;
		uint8_t len: 4;
		// hb0
		uint8_t scan_window_low; // unit: 0.625ms
		// hb1
		uint8_t scan_window_high;
		// hb2
		uint8_t scan_intvl_low;
		// hb3
		uint8_t scan_intvl_high;
	} rtk_coex_b2w_sw_mailbox_le_scan_para;
};
// for mailbox_id = 0x47
struct rtk_coex_b2w_sw_mailbox_bt_extra_info_report_by_itself {
	// lb0
	uint8_t  id;
	// lb1
	uint8_t  len;
	// lb2
	u8 rsvd_lb2;
	// lb3
	uint8_t rssi;
	// hb0
	uint8_t rsvd_hb0;
	// hb1
	uint8_t rsvd_hb1_0: 2;
	uint8_t le_setup_link: 1;
	uint8_t rsvd_hb1_1: 1;
	uint8_t seqnum: 4;
	// hb2
	uint8_t iso_intvl_low;//unit:1.25ms
	// hb3
	uint8_t iso_intvl_high;
};
// for mailbox_id = 0x49
struct rtk_coex_b2w_sw_mailbox_bt_le_init_scan_start_end {
	// lb0
	uint8_t id;
	// lb1
	uint8_t len;
	// lb2
	uint8_t scan_window_low; // unit: 0.625ms
	// lb3
	uint8_t scan_window_high;
	// hb0
	uint8_t scan_intvl_low;
	// hb1
	uint8_t scan_intvl_high;
	// hb2
	uint8_t scan_start: 1;
	uint8_t rsvd: 7;
	// hb3
	uint8_t rsvd_hb3;
};
enum LE_SCAN_TYPE_e {
	LE_SCAN_NONE = 0,
	LE_SCAN_INIT = 1,
	LE_SCAN_NORMAL = 2,
};
enum LE_CONNECT_STATUS_e {
	LE_CONNECT_NONE = 0,
	LE_CONNECT_IDLE = 1,
	LE_CONNECT_BUSY = 2,
};
struct rtk_coex_bt_info_t {
	u16 le_scan_init_intvl;
	u16 le_scan_init_window;
	u16 le_scan_intvl;
	u16 le_scan_window;
	u8 le_scan_type: 3;
	u8 le_connect: 3;
	u8 le_setup_link: 1;
	u8 rsvd: 1;
	u8 le_link_cnt;
};

#ifndef BT_HCI_OP_LE_SET_SCAN_PARAM
#define BT_HCI_OP_LE_SET_SCAN_PARAM     0x200b
#endif
#ifndef BT_HCI_OP_LE_SET_SCAN_ENABLE
#define BT_HCI_OP_LE_SET_SCAN_ENABLE    0x200c
#endif
#ifndef BT_HCI_OP_LE_CREATE_CONN
#define BT_HCI_OP_LE_CREATE_CONN    0x200d
#endif
#ifndef BT_HCI_OP_LE_CREATE_CONN_CANCEL
#define BT_HCI_OP_LE_CREATE_CONN_CANCEL 0x200e
#endif
#endif

enum __hci_conn_type {
	HCI_CONN_TYPE_L2CAP = 0,
	HCI_CONN_TYPE_SCO_ESCO  = 1,
	HCI_CONN_TYPE_LE    = 2,
	HCI_CONN_TYPE_CIS   = 4,
	HCI_CONN_TYPE_BIS   = 5,
};

enum __profile_type {
	PROFILE_SCO = 0,
	PROFILE_HID = 1,
	PROFILE_A2DP = 2,
	PROFILE_PAN = 3,
	PROFILE_HID_INTERVAL = 4, /* hid interval */
	PROFILE_HOGP = 5,
	PROFILE_VOICE = 6,
	PROFILE_SINK = 7,
	PROFILE_LE_AUDIO = 8,
	PROFILE_MOUSE = 9,
	PROFILE_MAX = 10
};


enum rtk_coex_mailbox_cmd {
	RTK_COEX_MAILBOX_BT_FW_PATCH                        = 0x0a,
	RTK_COEX_MAILBOX_WL_OP_MODE                         = 0x11,
	RTK_COEX_MAILBOX_WIFI_RF_CALIBRATION_EN             = 0x15,
	RTK_COEX_MAILBOX_FORCE_BT_TXPWR                     = 0x17,
	RTK_COEX_MAILBOX_BT_IGNORE_WLAN_ACT                 = 0x1b,
	RTK_COEX_MAILBOX_PSD_CONTROL                        = 0x21,
	RTK_COEX_MAILBOX_BT_INFO_REPORT                     = 0x23,
	RTK_COEX_MAILBOX_BT_PSD_REPORT                      = 0x24,
	RTK_COEX_MAILBOX_BT_INFO_REPORT_BY_ITSELF           = 0x27,
	RTK_COEX_MAILBOX_BT_INFO_REPORT_CONTROL             = 0x28,
	RTK_COEX_MAILBOX_BT_SET_TXRETRY_REPORT_PARAMETER    = 0x29,
	RTK_COEX_MAILBOX_BT_SET_PTA_TABLE                   = 0x2A,
	RTK_COEX_MAILBOX_BT_MP_REPORT                       = 0x30,
	RTK_COEX_MAILBOX_BT_PSD_MODE                        = 0x31,
	RTK_COEX_MAILBOX_BT_SET_BT_LNA_CONSTRAINT           = 0x32,
	RTK_COEX_MAILBOX_BT_LOOPBACK                        = 0x33,
	RTK_COEX_MAILBOX_BT_CALIBRATION                     = 0x34,
	RTK_COEX_MAILBOX_INTERNAL_SWITCH                    = 0x35,
	RTK_COEX_MAILBOX_BT_CHECK_WLAN_STATE                = 0x36,
	RTK_COEX_MAILBOX_CHANGE_WIFI_RF_STATE               = 0x37,
	RTK_COEX_MAILBOX_BT_PAGE_SCAN_INTERVAL              = 0x38,
	RTK_COEX_MAILBOX_BT_AUTO_SLOT                       = 0x39,
	RTK_COEX_MAILBOX_MAILBOX_EN                         = 0x40,
	RTK_COEX_MAILBOX_BT_CALIBRATION_FAIL                = 0x41,
	RTK_COEX_MAILBOX_WIFI_TDMA_IN_BT_SLOT               = 0x42,
	RTK_COEX_MAILBOX_WIFI_RPT_TDMA                      = 0x43,
	RTK_COEX_MAILBOX_BT_LE_EXTRA_INFO_BY_ITSELF         = 0x46,
	RTK_COEX_MAILBOX_BT_LE_EXTRA_INFO                   = 0x47,
	RTK_COEX_MAILBOX_BT_SLOT_CTRL                       = 0x48,
	RTK_COEX_MAILBOX_BT_SCAN_START_END                  = 0x49,
};

/* for RTK_COEX_MAILBOX_BT_MP_REPORT 0x30*/
enum rtk_coex_mailbox_mp_report_opcode {
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_VERSION        = 0x0,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_WRITE_REG_ADDR        = 0xC,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_WRITE_REG_VALUE       = 0xD,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_READ_REG              = 0x11,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_IND_SPEC_ACL_PKT      = 0x21,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_COEX_SUPPORT_VERSION  = 0x2B,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_TYPE   = 0x2D,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_GET_BT_LE_SCAN_PARA   = 0x2E,
	RTK_COEX_MAILBOX_MP_REPORT_OPCODE_SET_BT_LANCONS_LVL    = 0x32,
};
struct rtk_bt_coex_profile_info_t {
	struct list_head list;
	uint16_t psm;
	uint16_t dcid;
	uint16_t scid;
	uint16_t idx;
	uint8_t  flags;
};

struct rtk_bt_coex_conn_t {
	struct list_head list;
	uint16_t conn_handle;
	uint8_t type;       /* __hci_conn_type：0:l2cap, 1:sco/esco, 2:le */
	uint16_t connect_interval;
	uint16_t profile_bitmap;
	uint16_t profile_status_bitmap;
	uint8_t  profile_refcount[PROFILE_MAX];
	struct list_head profile_list;
	uint32_t a2dp_cnt;
	uint32_t a2dp_pre_cnt;
	uint32_t pan_cnt;
	uint32_t pan_pre_cnt;
};

struct rtk_bt_coex_monitor_node_t {
	struct list_head list;
	struct rtk_bt_coex_conn_t *p_conn;
	uint16_t profile_idx;
	uint8_t b_first_add;
};

struct rtk_bt_coex_priv_t {
	struct list_head conn_list;
	struct list_head monitor_list;
	void *monitor_mutex;
	void *monitor_timer;
#if defined(HCI_BT_COEX_SW_MAILBOX) && HCI_BT_COEX_SW_MAILBOX
	struct rtk_coex_bt_info_t bt_info_cur;
	struct rtk_coex_bt_info_t bt_info_prev;
	void *info_paras_mutex;
	void *setup_link_timer;
#endif
};



/* little endian */
struct sbc_frame_hdr {
	uint8_t syncword: 8;        /* Sync word */
	uint8_t subbands: 1;        /* Subbands, 0:4;1:8 */
	uint8_t allocation_method: 1;   /* Allocation method, 0:LOUDNESS;1:SNR */
	uint8_t channel_mode: 2;        /* Channel mode, 00:MONO(1channel);01:DUAL(2channels);10:STEREO(2channels);11:JOINT_STEREO(2channels) */
	uint8_t blocks: 2;      /* Blocks */
	uint8_t sampling_frequency: 2;  /* Sampling frequency, 00:16K;01:32K;10:44.1K;11:48K */
	uint8_t bitpool: 8;     /* Bitpool */
	uint8_t crc_check: 8;       /* CRC check */
};

struct rtp_header {
	unsigned cc: 4;
	unsigned x: 1;
	unsigned p: 1;
	unsigned v: 2;

	unsigned pt: 7;
	unsigned m: 1;

	uint16_t sequence_number;
	uint32_t timestamp;
	uint32_t ssrc;
	uint32_t csrc[0];
};


void bt_coex_init(void);
void bt_coex_process_rx_frame(uint8_t type, uint8_t *pdata, uint16_t len);
void bt_coex_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len);
void bt_coex_deinit(void);
#endif  /* _RTK_COEX_H_ */
