/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- */
#include <string.h>
#include <stdio.h>
#include <osif.h>
#include <rtk_bt_def.h>
#include <rtk_bt_device.h>
#include <rtk_bt_common.h>
#include <bt_api_config.h>
#include <bt_utils.h>
#include <rtk_bt_br_gap.h>
#include <rtk_bt_sdp.h>
#include <rtk_bt_avrcp.h>
#include <rtk_bt_a2dp.h>
#include <bt_audio_intf.h>
#include <bt_audio_track_api.h>
#include <app_audio_data.h>
#include <sbc_codec_entity.h>
// #include <aac_codec_entity.h>
#include "kv.h"
#include <dlist.h>
#include <app_msg.h>

/* -------------------------------- Defines --------------------------------- */
#define RTK_BT_DEV_NAME                      "RTK_BT_AUDIO_DEVICE"
#define DEFAULT_PAGESCAN_WINDOW              0x48
#define DEFAULT_PAGESCAN_INTERVAL            0x800 //0x800
#define DEFAULT_PAGE_TIMEOUT                 0x2000
#define DEFAULT_INQUIRYSCAN_WINDOW           0x48
#define DEFAULT_INQUIRYSCAN_INTERVAL         0x800 //0x1000
#define DEFAULT_AUDIO_LEFT_VOLUME            0.8
#define DEFAULT_AUDIO_RIGHT_VOLUME           0.8
#define RTK_BT_A2DP_SRC_STREAM_MAX_CREDITS   2
#define RTK_BT_DEMO_RECONNECT_TIMER_INTERVAL 5000
#define RTK_BT_DEMO_RECONNECT_COUNT          3
/* ------------------------------- Company ID ------------------------------- */
#define RTK_COMPANY_ID 0x005D
/* ----------------------------- Avrcp Related ------------------------------ */
#define VOL_MAX     0x7f
#define VOL_MIN     0x00
#define BT_APP_PROCESS(process)                       \
	if (RTK_BT_OK != process)                         \
	{                                                 \
		printf("[APP] %s failed!\r\n", __FUNCTION__); \
		return -1;                                    \
	}

/* ---------------------------- Global Variables ---------------------------- */
static uint8_t curr_volume = RTK_BT_DEFAULT_ABSOLUTE_VOLUME;

/* ---------------------------- Static Variables ---------------------------- */
static rtk_bt_audio_track_t *a2dp_demo_audio_track_hdl = NULL;
static void *a2dp_demo_codec_entity = NULL;
static uint8_t a2dp_demo_role;
static uint8_t src_a2dp_credits = RTK_BT_A2DP_SRC_STREAM_MAX_CREDITS;
static rtk_bt_audio_codec_conf_t audio_codec_conf = {0};
static rtk_bt_sbc_codec_t sbc_codec_t = {
	{
		.sbc_enc_mode = (sbc_channel_mode_t)SBC_MODE_STANDARD,
		.blocks = 16,
		.subbands = 8,
		.alloc_method = SBC_ALLOCATION_METHOD_SNR,
		.sample_rate = 41000,
		.bitpool = 0x21,
		.channel_mode = SBC_CHANNEL_MODE_DUAL_CHANNEL,
	},
	{0},
};
// static rtk_bt_aac_codec_t aac_codec_t = {
// 				{
// 					.bit_rate = 128 * 1024,
// 					.channels = 2,
// 					.object_type = RTK_AAC_AOT_MP2_AAC_LC,
// 					.sample_freq = 44100,
// 					.vbr = 0,
// 				},
// 				{0},
// };

struct a2dp_demo_task_t {
	void* hdl;
	void* sem;
	uint8_t run;
};

static struct a2dp_demo_task_t a2dp_task = {
	.hdl = NULL,
	.sem = NULL,
	.run = 0,
};

static uint8_t remote_bd_addr[6] = {0};
/* ---------------------------- Auto reconnect config ---------------------------- */
static void *reconnect_timer = NULL;
static uint8_t a2dp_demo_auto_reconnect = 1;
static uint8_t a2dp_demo_auto_reconnect_try_count = RTK_BT_DEMO_RECONNECT_COUNT;
/* when sink disconnect due to out of range, this flag where be set up to do reconnecting */
static uint8_t a2dp_demo_disconnection_reconnecting = 0;
static char a2dp_demo_filesystem_key[] = "a2dp_reconnect_data";
/* ---------------------------- Bond Info Manage config -------------------------- */
/* Max bond number is unchangable */
#define A2DP_DEMO_MAX_BOND_NUM 8
#define A2DP_DEMO_BOND_INFO_USED_VAL 0x4E
#define A2DP_DEMO_BOND_INFO_PRIORITY_UNASSIGNED 0x4F
#define A2DP_DEMO_BOND_INFO_FLUSH_EVENT 0x01
#define A2DP_DEMO_BOND_INFO_THREAD_EXIT 0xFF
static char a2dp_demo_bond_info_key[] = "a2dp_bond_table";
static uint8_t a2dp_demo_bond_num = 0;
static uint8_t a2dp_demo_bond_info_flush = 1;
static struct a2dp_demo_task_t bond_info_flush_task = {
	.hdl = NULL,
	.sem = NULL,
	.run = 0,
};
static void *a2dp_demo_bond_info_flush_msg_q = NULL;
typedef struct{
	uint8_t				bd_addr[6];                         /* Bluetooth address */
	uint8_t				name[RTK_BT_GAP_DEVICE_NAME_LEN];   /* name */
	uint8_t				name_contained;                     /* check whether this unit contains name */
	uint8_t				priority;                           /* bond info priority, range from 1~8, 1 is the highest */
	uint8_t				used_check;                         /* check whether this unit is configured */
} a2dp_demo_bond_info_t;
a2dp_demo_bond_info_t a2dp_demo_bond_table[A2DP_DEMO_MAX_BOND_NUM] = {0};
/* ------------------------------------ End -------------------------------------- */
/* class of device */
static uint32_t cod = (0x18 | (0x04 << 8) | (0x24 << 16));
/* supervision_timeout */
static uint16_t superv_time = (0x1f40);

static rtk_bt_br_page_param_t page_param = {
	.pagescan_type = RTK_BT_BR_GAP_PAGE_SCAN_TYPE_INTERLACED,
	.pagescan_interval = DEFAULT_PAGESCAN_INTERVAL,
	.pagescan_window = DEFAULT_PAGESCAN_WINDOW,
	.page_timeout = DEFAULT_PAGE_TIMEOUT,
};

static rtk_bt_br_inquiry_param_t inquiry_param = {
	.inquiryscan_type = RTK_BT_BR_GAP_PAGE_SCAN_TYPE_INTERLACED,
	.inquiryscan_window = DEFAULT_INQUIRYSCAN_WINDOW,
	.inquiryscan_interval = DEFAULT_INQUIRYSCAN_INTERVAL,
	.inquiry_mode = RTK_BT_BR_GAP_INQUIRY_MODE_EXTENDED_RESULT,
};

static const uint8_t did_sdp_record[] = {
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x4D,
	//attribute SDP_ATTR_SRV_CLASS_ID_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PNP_INFORMATION >> 8),
	(uint8_t)(RTK_BT_UUID_PNP_INFORMATION),

	//attribute SDP_ATTR_BROWSE_GROUP_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP >> 8),
	(uint8_t)RTK_BT_UUID_PUBLIC_BROWSE_GROUP,

	//attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x09,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_LANG_ENGLISH >> 8),
	(uint8_t)RTK_BT_SDP_LANG_ENGLISH,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_CHARACTER_UTF8 >> 8),
	(uint8_t)RTK_BT_SDP_CHARACTER_UTF8,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_BASE_LANG_OFFSET >> 8),
	(uint8_t)RTK_BT_SDP_BASE_LANG_OFFSET,

	//attribute SDP_ATTR_PROFILE_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROFILE_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROFILE_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x08,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PNP_INFORMATION >> 8),
	(uint8_t)RTK_BT_UUID_PNP_INFORMATION,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,//version 1.3
	0x03,

	//attribute SDP_ATTR_DIP_SPECIFICATION_ID
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_DIP_SPECIFICATION_ID >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_DIP_SPECIFICATION_ID,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,
	0x03,

	//attribute SDP_ATTR_DIP_VENDOR_ID
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_DIP_VENDOR_ID >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_DIP_VENDOR_ID,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	//0x00,//0x005D : RealTek
	//0x5D,
	(uint8_t)(RTK_COMPANY_ID >> 8),
	(uint8_t)RTK_COMPANY_ID,

	//attribute SDP_ATTR_DIP_PRODUCT_ID
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_DIP_PRODUCT_ID >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_DIP_PRODUCT_ID,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x22,//8763
	0x3B,

	//attribute SDP_ATTR_DIP_PRODUCT_VERSION
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_DIP_PRODUCT_VERSION >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_DIP_PRODUCT_VERSION,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,// 1.0.0
	0x00,

	//attribute SDP_ATTR_DIP_PRIMARY_RECORD
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_DIP_PRIMARY_RECORD >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_DIP_PRIMARY_RECORD,
	RTK_BT_SDP_BOOL_ONE_BYTE,
	true,

	//attribute SDP_ATTR_DIP_VENDOR_ID_SOURCE
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_DIP_VENDOR_ID_SOURCE >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_DIP_VENDOR_ID_SOURCE,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x00,//Bluetooth SIG
	0x01
};

static const uint8_t avrcp_ct_sdp_record[] = {
	//Total length
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x3B,//0x49,//0x62,

	//Attribute SDP_ATTR_SRV_CLASS_ID_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Attribute length: 6 bytes
	//Service Class #0: A/V Remote Control
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL >> 8),
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL),
	//Service Class #1: A/V Remote Control Controller
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL_CONTROLLER >> 8),
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL_CONTROLLER),

	//Attribute SDP_ATTR_PROTO_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROTO_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROTO_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x10, //Attribute length: 12 bytes
	//Protocol #0: L2CAP
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Element length: 3 bytes
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_L2CAP >> 8),
	(uint8_t)(RTK_BT_UUID_L2CAP),
	//Parameter #0 for Protocol #0: PSM = AVCTP
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_PSM_AVCTP >> 8),
	(uint8_t)RTK_BT_PSM_AVCTP,
	//Protocol #1: AVCTP
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Element length: 5 bytes
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AVCTP >> 8),
	(uint8_t)(RTK_BT_UUID_AVCTP),
	//Parameter #0 for Protocol #1: 0x0104 (v1.4)
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(0x0104 >> 8),
	(uint8_t)(0x0104),

	//Attribute SDP_ATTR_PROFILE_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROFILE_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROFILE_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x08, //Attribute length: 8 bytes
	//Profile #0: A/V Remote Control
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Element length: 6 bytes
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL >> 8),
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL),
	//Parameter #0 for Profile #0: 0x0106 (v1.6)
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(0x0106 >> 8),
	(uint8_t)(0x0106),

	//Attribute SDP_ATTR_SUPPORTED_FEATURES
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)((RTK_BT_SDP_ATTR_SUPPORTED_FEATURES) >> 8),
	(uint8_t)(RTK_BT_SDP_ATTR_SUPPORTED_FEATURES),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(0x0001 >> 8), //Category 1 Player / Recorder
	(uint8_t)(0x0001),

	//Attribute SDP_ATTR_BROWSE_GROUP_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP >> 8),
	(uint8_t)RTK_BT_UUID_PUBLIC_BROWSE_GROUP
	/*
		//Attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST...it is used for SDP_ATTR_SRV_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
		(uint8_t)RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
		RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
		0x09,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_LANG_ENGLISH >> 8),
		(uint8_t)RTK_BT_SDP_LANG_ENGLISH,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_CHARACTER_UTF8 >> 8),
		(uint8_t)RTK_BT_SDP_CHARACTER_UTF8,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_BASE_LANG_OFFSET >> 8),
		(uint8_t)RTK_BT_SDP_BASE_LANG_OFFSET,

		//Attribute SDP_ATTR_PROVIDER_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)((RTK_BT_SDP_ATTR_PROVIDER_NAME + RTK_BT_SDP_BASE_LANG_OFFSET) >> 8),
		(uint8_t)(RTK_BT_SDP_ATTR_PROVIDER_NAME + RTK_BT_SDP_BASE_LANG_OFFSET),
		RTK_BT_SDP_STRING_HDR,
		0x07, //Attribute length: 7 bytes
		0x52, 0x65, 0x61, 0x6C, 0x54, 0x65, 0x6B, //RealTek

		//Attribute SDP_ATTR_SRV_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)((RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET) >> 8),
		(uint8_t)(RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET),
		RTK_BT_SDP_STRING_HDR,
		0x08, //Attribute length: 8 bytes
		0x41, 0x56, 0x52, 0x43, 0x50, 0x20, 0x43, 0x54, //AVRCP CT
	*/
};

static const uint8_t avrcp_tg_sdp_record[] = {
	//Total length
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x38,//0x46,//0x5F,

	//Attribute SDP_ATTR_SRV_CLASS_ID_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03, //Attribute length: 6 bytes
	//Service Class #0: A/V Remote Control Target
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL_TARGET >> 8),
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL_TARGET),

	//Attribute SDP_ATTR_PROTO_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROTO_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROTO_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x10, //Attribute length: 12 bytes
	//Protocol #0: L2CAP
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Element length: 3 bytes
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_L2CAP >> 8),
	(uint8_t)(RTK_BT_UUID_L2CAP),
	//Parameter #0 for Protocol #0: PSM = AVCTP
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_PSM_AVCTP >> 8),
	(uint8_t)RTK_BT_PSM_AVCTP,
	//Protocol #1: AVCTP
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Element length: 5 bytes
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AVCTP >> 8),
	(uint8_t)(RTK_BT_UUID_AVCTP),
	//Parameter #0 for Protocol #1: 0x0104 (v1.4)
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(0x0104 >> 8),
	(uint8_t)(0x0104),

	//Attribute SDP_ATTR_PROFILE_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROFILE_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROFILE_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x08, //Attribute length: 8 bytes
	//Profile #0: A/V Remote Control
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06, //Element length: 6 bytes
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL >> 8),
	(uint8_t)(RTK_BT_UUID_AV_REMOTE_CONTROL),
	//Parameter #0 for Profile #0: 0x0106 (v1.6)
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(0x0106 >> 8),
	(uint8_t)(0x0106),

	//Attribute SDP_ATTR_SUPPORTED_FEATURES
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)((RTK_BT_SDP_ATTR_SUPPORTED_FEATURES) >> 8),
	(uint8_t)(RTK_BT_SDP_ATTR_SUPPORTED_FEATURES),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(0x0002 >> 8), //Category 2 Amplifier
	(uint8_t)(0x0002),

	//Attribute SDP_ATTR_BROWSE_GROUP_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP >> 8),
	(uint8_t)RTK_BT_UUID_PUBLIC_BROWSE_GROUP
	/*
		//Attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST...it is used for SDP_ATTR_SRV_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
		(uint8_t)RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
		RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
		0x09,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_LANG_ENGLISH >> 8),
		(uint8_t)RTK_BT_SDP_LANG_ENGLISH,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_CHARACTER_UTF8 >> 8),
		(uint8_t)RTK_BT_SDP_CHARACTER_UTF8,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_BASE_LANG_OFFSET >> 8),
		(uint8_t)RTK_BT_SDP_BASE_LANG_OFFSET,

		//Attribute SDP_ATTR_PROVIDER_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)((RTK_BT_SDP_ATTR_PROVIDER_NAME + RTK_BT_SDP_BASE_LANG_OFFSET) >> 8),
		(uint8_t)(RTK_BT_SDP_ATTR_PROVIDER_NAME + RTK_BT_SDP_BASE_LANG_OFFSET),
		RTK_BT_SDP_STRING_HDR,
		0x07, //Attribute length: 7 bytes
		0x52, 0x65, 0x61, 0x6C, 0x54, 0x65, 0x6B, //RealTek

		//Attribute SDP_ATTR_SRV_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)((RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET) >> 8),
		(uint8_t)(RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET),
		SDP_STRING_HDR,
		0x08, //Attribute length: 8 bytes
		0x41, 0x56, 0x52, 0x43, 0x50, 0x20, 0x54, 0x47, //AVRCP TG
	*/
};

static const uint8_t a2dp_sink_sdp_record[] = {
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x39,//0x55,
	//attribute SDP_ATTR_SRV_CLASS_ID_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AUDIO_SINK >> 8),
	(uint8_t)(RTK_BT_UUID_AUDIO_SINK),

	//attribute SDP_ATTR_PROTO_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROTO_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROTO_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x10,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_L2CAP >> 8),
	(uint8_t)(RTK_BT_UUID_L2CAP),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_PSM_AVDTP >> 8),
	(uint8_t)(RTK_BT_PSM_AVDTP),
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AVDTP >> 8),
	(uint8_t)(RTK_BT_UUID_AVDTP),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,
	0x03,

	//attribute SDP_ATTR_BROWSE_GROUP_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP >> 8),
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP),
	/*
		//attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
		(uint8_t)RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
		RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
		0x09,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_LANG_ENGLISH >> 8),
		(uint8_t)RTK_BT_SDP_LANG_ENGLISH,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_CHARACTER_UTF8 >> 8),
		(uint8_t)RTK_BT_SDP_CHARACTER_UTF8,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_BASE_LANG_OFFSET >> 8),
		(uint8_t)RTK_BT_SDP_BASE_LANG_OFFSET,
	*/
	//attribute SDP_ATTR_PROFILE_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROFILE_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROFILE_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x08,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_ADVANCED_AUDIO_DISTRIBUTION >> 8),
	(uint8_t)(RTK_BT_UUID_ADVANCED_AUDIO_DISTRIBUTION),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,//version 1.3
	0x03,

	//attribute SDP_ATTR_SUPPORTED_FEATURES
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SUPPORTED_FEATURES >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SUPPORTED_FEATURES,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x00,
	0x03
	/*
		//attribute SDP_ATTR_SRV_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)((RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET) >> 8),
		(uint8_t)(RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET),
		RTK_BT_SDP_STRING_HDR,
		0x09,
		0x61, 0x32, 0x64, 0x70, 0x5f, 0x73, 0x69, 0x6e, 0x6b //a2dp_sink
	*/
};

static const uint8_t a2dp_src_sdp_record[] =
{
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x39,//0x55,
	//attribute SDP_ATTR_SRV_CLASS_ID_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AUDIO_SOURCE >> 8),
	(uint8_t)(RTK_BT_UUID_AUDIO_SOURCE),

	//attribute SDP_ATTR_PROTO_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROTO_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROTO_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x10,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_L2CAP >> 8),
	(uint8_t)(RTK_BT_UUID_L2CAP),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_PSM_AVDTP >> 8),
	(uint8_t)(RTK_BT_PSM_AVDTP),
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_AVDTP >> 8),
	(uint8_t)(RTK_BT_UUID_AVDTP),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,
	0x03,

	//attribute SDP_ATTR_BROWSE_GROUP_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x03,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP >> 8),
	(uint8_t)(RTK_BT_UUID_PUBLIC_BROWSE_GROUP),
	/*
		//attribute SDP_ATTR_LANG_BASE_ATTR_ID_LIST
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST >> 8),
		(uint8_t)RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST,
		RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
		0x09,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_LANG_ENGLISH >> 8),
		(uint8_t)RTK_BT_SDP_LANG_ENGLISH,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_CHARACTER_UTF8 >> 8),
		(uint8_t)RTK_BT_SDP_CHARACTER_UTF8,
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)(RTK_BT_SDP_BASE_LANG_OFFSET >> 8),
		(uint8_t)RTK_BT_SDP_BASE_LANG_OFFSET,
	*/
	//attribute SDP_ATTR_PROFILE_DESC_LIST
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_PROFILE_DESC_LIST >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_PROFILE_DESC_LIST,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x08,
	RTK_BT_SDP_DATA_ELEM_SEQ_HDR,
	0x06,
	RTK_BT_SDP_UUID16_HDR,
	(uint8_t)(RTK_BT_UUID_ADVANCED_AUDIO_DISTRIBUTION >> 8),
	(uint8_t)(RTK_BT_UUID_ADVANCED_AUDIO_DISTRIBUTION),
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x01,//version 1.3
	0x03,

	//attribute SDP_ATTR_SUPPORTED_FEATURES
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	(uint8_t)(RTK_BT_SDP_ATTR_SUPPORTED_FEATURES >> 8),
	(uint8_t)RTK_BT_SDP_ATTR_SUPPORTED_FEATURES,
	RTK_BT_SDP_UNSIGNED_TWO_BYTE,
	0x00,
	0x03
	/*
		//attribute SDP_ATTR_SRV_NAME
		RTK_BT_SDP_UNSIGNED_TWO_BYTE,
		(uint8_t)((RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET) >> 8),
		(uint8_t)(RTK_BT_SDP_ATTR_SRV_NAME + RTK_BT_SDP_BASE_LANG_OFFSET),
		RTK_BT_SDP_STRING_HDR,
		0x09,
		0x61, 0x32, 0x64, 0x70, 0x5f, 0x73, 0x69, 0x6e, 0x6b //a2dp_sink
	*/
};

static rtk_bt_a2dp_media_codec_sbc_t codec_sbc = {
	.sampling_frequency_mask = 0xf0,
	.channel_mode_mask = 0x0f,
	.block_length_mask = 0xf0,
	.subbands_mask = 0x0C,
	.allocation_method_mask = 0x03,
	.min_bitpool = 2,
	.max_bitpool = 53,
};

// static rtk_bt_a2dp_media_codec_aac_t codec_aac = {
// 	.object_type_mask = 0x80,
// 	.sampling_frequency_mask = 0x0180,
// 	.channel_number_mask = 0x0c,
// 	.vbr_supported = 1,
// 	.bit_rate = 0,
// };

static uint32_t pcm_offset = 0;

static void app_a2dp_src_send_data(void)
{
	rtk_bt_a2dp_stream_data_send_t data_send_t = {0};
	struct enc_codec_buffer *penc_codec_buffer_t = NULL;
	short *pdata = NULL;

	if (src_a2dp_credits) {
		if (pcm_offset < (birds_sing_size / 2)) {
			if (pcm_offset + 512 < birds_sing_size / 2) {
				pdata = (short *)(birds_sing + pcm_offset);
				penc_codec_buffer_t = rtk_bt_audio_data_encode(RTK_BT_AUDIO_CODEC_SBC, a2dp_demo_codec_entity, pdata, 1024);
				if (penc_codec_buffer_t) {
					memset((void *)&data_send_t, 0, sizeof(rtk_bt_a2dp_stream_data_send_t));
					memcpy((void *)data_send_t.bd_addr, (void *)remote_bd_addr, 6);
					data_send_t.frame_buf = (uint8_t *)penc_codec_buffer_t->pbuffer;
					data_send_t.frame_num = (uint8_t)penc_codec_buffer_t->frame_num;
					data_send_t.len = (uint16_t)(penc_codec_buffer_t->frame_num * penc_codec_buffer_t->frame_size);
					if (rtk_bt_a2dp_data_send(&data_send_t)) {
						printf("[A2DP] data send fail \r\n");
					} else {
						src_a2dp_credits --;
					}
					rtk_bt_audio_free_encode_buffer(RTK_BT_AUDIO_CODEC_SBC, a2dp_demo_codec_entity, penc_codec_buffer_t);
				} else {
					printf("[A2DP SRC Demo]: Encode fail \r\n");
				}
			} else {
				pdata = (short *)(birds_sing + pcm_offset);
				penc_codec_buffer_t = rtk_bt_audio_data_encode(RTK_BT_AUDIO_CODEC_SBC, a2dp_demo_codec_entity, pdata, ((birds_sing_size / 2) - pcm_offset) * 2);
				if (penc_codec_buffer_t) {
					memset((void *)&data_send_t, 0, sizeof(rtk_bt_a2dp_stream_data_send_t));
					memcpy((void *)data_send_t.bd_addr, (void *)remote_bd_addr, 6);
					data_send_t.frame_buf = (uint8_t *)penc_codec_buffer_t->pbuffer;
					data_send_t.frame_num = (uint8_t)penc_codec_buffer_t->frame_num;
					data_send_t.len = (uint16_t)(penc_codec_buffer_t->frame_num * penc_codec_buffer_t->frame_size);
					if (rtk_bt_a2dp_data_send(&data_send_t)) {
						printf("[A2DP] data send fail \r\n");
					} else {
						src_a2dp_credits --;
					}
					rtk_bt_audio_free_encode_buffer(RTK_BT_AUDIO_CODEC_SBC, a2dp_demo_codec_entity, penc_codec_buffer_t);
				} else {
					printf("[A2DP SRC Demo]: Encode fail \r\n");
				}
			}
			pcm_offset += 512;
		} else {
			pcm_offset = 0;
		}
	} else {
		// printf("[A2DP] waiting src_a2dp_credits \r\n");
	}
}

static void a2dp_task_entry(void *ctx)
{
	(void)ctx;
	osif_sem_give(a2dp_task.sem);

	while(a2dp_task.run) {
		app_a2dp_src_send_data();
		osif_delay(3);
	}

	osif_sem_give(a2dp_task.sem);
	osif_task_delete(NULL);
}

static a2dp_demo_bond_info_t* a2dp_demo_find_bond_info_by_mac_addr(uint8_t *bd_addr)
{
	a2dp_demo_bond_info_t *pbond_info = NULL;

	if (!bd_addr) {
		return NULL;
	}
	for (uint8_t i = 0; i < A2DP_DEMO_MAX_BOND_NUM; i ++) {
		/* check bond info unit meaningful */
		if (A2DP_DEMO_BOND_INFO_USED_VAL == a2dp_demo_bond_table[i].used_check) {
			if (!memcmp((void *)a2dp_demo_bond_table[i].bd_addr, (void *)bd_addr, 6)) {
				pbond_info = &a2dp_demo_bond_table[i];
				break;
			}
		}
	}

	return pbond_info;
}

static a2dp_demo_bond_info_t *a2dp_demo_find_bond_info_by_priority(uint8_t bond_priority)
{
	a2dp_demo_bond_info_t *pbond_info = NULL;

	if ((bond_priority < 1) || (bond_priority > 8)) {
		return NULL;
	}
	for (uint8_t i = 0; i < A2DP_DEMO_MAX_BOND_NUM; i ++) {
		/* check bond info unit meaningful */
		if (A2DP_DEMO_BOND_INFO_USED_VAL == a2dp_demo_bond_table[i].used_check) {
			if (bond_priority == a2dp_demo_bond_table[i].priority) {
				pbond_info = &a2dp_demo_bond_table[i];
				break;
			}
		}
	}

	return pbond_info;
}

static a2dp_demo_bond_info_t *get_free_bond_table_unit(void)
{
	a2dp_demo_bond_info_t *pbond_info = NULL;

	for (uint8_t i = 0; i < A2DP_DEMO_MAX_BOND_NUM; i ++) {
		/* check free unit */
		if (A2DP_DEMO_BOND_INFO_USED_VAL != a2dp_demo_bond_table[i].used_check) {
			pbond_info = &a2dp_demo_bond_table[i];
			pbond_info->used_check = A2DP_DEMO_BOND_INFO_USED_VAL;
			break;
		}
	}

	/* if bond units are full, get the lowest priority bond info for overwriting */
	if (!pbond_info) {
		pbond_info = a2dp_demo_find_bond_info_by_priority(8);
	}

	return pbond_info;
}

/* once application modify bond info, this api should bt invoke to sync bond info table */
uint16_t a2dp_demo_flush_bond_info(uint8_t type)
{
	T_IO_MSG io_msg;
	io_msg.type = type;
	
	if (false == osif_msg_send(a2dp_demo_bond_info_flush_msg_q, &io_msg, 0)) {
		return 1;
	}
	
	return 0;
}

static void a2dp_demo_bond_flush_thread(void *ctx)
{
	(void)ctx;
	T_IO_MSG io_msg;
	uint8_t bd_addr[6] = {0};
	a2dp_demo_bond_info_t *pbond_info = NULL;
	uint8_t temp_bond_num = 0;
	uint8_t need_save_bond_info = 0;
	uint16_t ret = 1;

	osif_sem_give(bond_info_flush_task.sem);
	while(bond_info_flush_task.run) {
		if (osif_msg_recv(a2dp_demo_bond_info_flush_msg_q, &io_msg, 0xFFFFFFFF) == true) {
			if (A2DP_DEMO_BOND_INFO_THREAD_EXIT == io_msg.type) {
				bond_info_flush_task.run = 0;
				continue;
			}
			/* do flush action */
			/* 1. get bond number */
			if (rtk_bt_br_gap_bond_num_get(&temp_bond_num)) {
				printf("[A2DP Demo] Get Bond Number fail\r\n");
				continue;
			}
			printf("[A2DP Demo] Get Bond Number %d \r\n", temp_bond_num);
			/* 2. get bond mac address accord to bond number */
			for (uint8_t i = 0; i < temp_bond_num; i ++) {
				/* gap bond priority start frmo 1, so i + 1 */
				ret = rtk_bt_br_gap_bond_addr_get(i + 1, bd_addr);
				if (ret) {
					printf("[A2DP Demo] Get bond addr failed! err: 0x%x", ret);
					continue;
				}
				printf("[A2DP Demo] Get Bond addr %x: %x: %x: %x: %x: %x \r\n",
										bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
				/* 3. sync with a2dp_demo_bond_table */
				pbond_info = a2dp_demo_find_bond_info_by_mac_addr(bd_addr);
				if (!pbond_info) {
					/* allocate new bond info unit */
					pbond_info = get_free_bond_table_unit();
					if (!pbond_info) {
						printf("[A2DP Demo] Get free bond table unit fail \r\n");
						continue;
					}
					memcpy((void *)pbond_info->bd_addr, (void *)bd_addr, 6);
					pbond_info->name_contained = 0;
					pbond_info->priority = i + 1;
					memset((void *)pbond_info->name, 0, RTK_BT_GAP_DEVICE_NAME_LEN);
					need_save_bond_info = 1;
					/* 4. try to fetch remote device name */
					rtk_bt_br_gap_get_remote_name(bd_addr);
				} else {
					/* update bond info unit */
					if (pbond_info->priority != (i + 1)) {
						if (A2DP_DEMO_BOND_INFO_PRIORITY_UNASSIGNED != pbond_info->priority) {
							a2dp_demo_bond_info_t *ptemp_bond_info = NULL;
							/* get previous priority bond info unit */
							ptemp_bond_info = a2dp_demo_find_bond_info_by_priority(i + 1);
							if (ptemp_bond_info) {
								/* this bond info should be mask for further configuration */
								ptemp_bond_info->priority = A2DP_DEMO_BOND_INFO_PRIORITY_UNASSIGNED;
							}
						}
						need_save_bond_info = 1;
						pbond_info->priority = i + 1;
					}
					if (!pbond_info->name_contained) {
						/* 4. try to fetch remote device name */
						rtk_bt_br_gap_get_remote_name(bd_addr);
					}
				}
			}
			/* 5. Maybe the loweset priority bond unit is delete, which cannot be detected by step3 */
			if (temp_bond_num < a2dp_demo_bond_num) {
				for (uint8_t i = 0; i < (a2dp_demo_bond_num - temp_bond_num); i ++) {
					pbond_info = a2dp_demo_find_bond_info_by_priority(temp_bond_num + i + 1);
					if (pbond_info) {
						memset((void *)pbond_info, 0, sizeof(a2dp_demo_bond_info_t));
						need_save_bond_info = 1;
					}
				}
			}
			/* 6. check whether need save a2dp_demo_bond_table */
			if (need_save_bond_info) {
				/* foreach table to check whether there is A2DP_DEMO_BOND_INFO_PRIORITY_UNASSIGNED bond unit(need to be deleted) */
				for (uint8_t i = 0; i < A2DP_DEMO_MAX_BOND_NUM; i ++) {
					if ((A2DP_DEMO_BOND_INFO_USED_VAL == a2dp_demo_bond_table[i].used_check) &&
					(A2DP_DEMO_BOND_INFO_PRIORITY_UNASSIGNED == a2dp_demo_bond_table[i].priority)) {
						memset((void *)&a2dp_demo_bond_table[i], 0, sizeof(a2dp_demo_bond_info_t));
					}
				}
				if (rt_kv_set(a2dp_demo_bond_info_key, (void *)a2dp_demo_bond_table, sizeof(a2dp_demo_bond_table)) == 1) {
					printf("[A2DP Demo] Save a2dp demo bond info table success \r\n");
				} else {
					printf("[A2DP Demo] Fail to save a2dp demo bond info table \r\n");
				}
			}
			a2dp_demo_bond_num = temp_bond_num;
		} else {
			printf("[A2DP Demo] Bond info flush thread recv msg fail\r\n");
		}
		osif_delay(3);
	}

	osif_sem_give(bond_info_flush_task.sem);
	osif_task_delete(NULL);
}

static void a2dp_demo_bond_info_dump(void)
{
	a2dp_demo_bond_info_t *pbond_info = NULL;

	printf("[A2DP Demo] Bond Info List\r\n");
	for (uint8_t i = 0; i < a2dp_demo_bond_num; i ++) {
		pbond_info = a2dp_demo_find_bond_info_by_priority(i + 1);
		if (pbond_info) {
			printf(">> %d. ", (int)(i + 1));
			printf(" mac addr %x: %x: %x: %x: %x: %x  ", pbond_info->bd_addr[5],
														pbond_info->bd_addr[4],
														pbond_info->bd_addr[3],
														pbond_info->bd_addr[2],
														pbond_info->bd_addr[1],
														pbond_info->bd_addr[0]);
			printf(" name ");
			if (pbond_info->name_contained) {
				printf(" %s", pbond_info->name);
			}
			printf(" \r\n");
		}
	}
}

static rtk_bt_evt_cb_ret_t br_gap_app_callback(uint8_t evt_code, void* param, uint32_t len)
{
	(void)len;
	(void)param;

	switch (evt_code) {
	
	/* RTK_BT_BR_GAP_ACT_SET_DEVICE_NAME */
	/* RTK_BT_BR_GAP_ACT_SET_PAGE_PARAM */
	/* RTK_BT_BR_GAP_ACT_SET_INQUIRY_PARAM */
	/* are Sync */

	case RTK_BT_BR_GAP_INQUIRY_RESULT: {
		rtk_bt_br_inquiry_result_t *p_result = (rtk_bt_br_inquiry_result_t *)param;
		printf("[BR GAP] Scan %x: %x: %x: %x: %x: %x Name %s \r\n", 
				p_result->bd_addr[5], p_result->bd_addr[4], p_result->bd_addr[3], p_result->bd_addr[2], p_result->bd_addr[1], p_result->bd_addr[0], 
				p_result->name);
		break;
	}

	case RTK_BT_BR_GAP_REMOTE_NAME_RSP: {
		rtk_bt_br_remote_name_rsp_t *p_name_rsp = (rtk_bt_br_remote_name_rsp_t *)param;
		if (a2dp_demo_bond_info_flush) {
			a2dp_demo_bond_info_t *pbond_info = NULL;
			pbond_info = a2dp_demo_find_bond_info_by_mac_addr(p_name_rsp->bd_addr);
			if (pbond_info) {
				memcpy((void *)pbond_info->name, (void *)p_name_rsp->name, RTK_BT_GAP_DEVICE_NAME_LEN);
				pbond_info->name_contained = 1;
				if (rt_kv_set(a2dp_demo_bond_info_key, (void *)a2dp_demo_bond_table, sizeof(a2dp_demo_bond_table)) == 1) {
					printf("[A2DP Demo] Save a2dp demo bond info table success \r\n");
					a2dp_demo_bond_info_dump();
				} else {
					printf("[A2DP Demo] Fail to save a2dp demo bond info table \r\n");
				}
			}
		}
		break;
	}

	case RTK_BT_BR_GAP_ACL_CONN_IND: {
		uint8_t *bd_addr = (uint8_t *)param;
		printf("[BR GAP] ACL connection indication %x: %x: %x: %x: %x: %x \r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
		break;
	}

	case RKT_BT_BR_GAP_ACL_CONN_SUCCESS: {
		uint8_t *bd_addr = (uint8_t *)param;
		if (a2dp_demo_auto_reconnect) {
			/* write reconnect info to file system */
			if (rt_kv_set(a2dp_demo_filesystem_key, (void *)bd_addr, 6) == 1) {
				printf("[A2DP Demo] Save a2dp demo reconnect data success \r\n");
			} else {
				printf("[A2DP Demo] Fail to save a2dp reconnect data \r\n");
			}
			a2dp_demo_auto_reconnect_try_count = 0;
			osif_timer_stop(&reconnect_timer);
		}
		printf("[BR GAP] ACL connection success %x: %x: %x: %x: %x: %x \r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
		break;
	}

	case RTK_BT_BR_GAP_LINK_KEY_REQ: {
		uint8_t found = *(uint8_t *)param;
		if (a2dp_demo_bond_info_flush && found) {
			a2dp_demo_flush_bond_info(A2DP_DEMO_BOND_INFO_FLUSH_EVENT);
		}
		printf("[BR GAP] Link Key Request received and found is 0x%x \r\n", found);
		break;
	}

	case RTK_BT_BR_GAP_LINK_KEY_INFO: {
		rtk_bt_br_bond_key_t *pbond_key_t = (rtk_bt_br_bond_key_t *)param;
		uint8_t *bd_addr = pbond_key_t->bd_addr;
		if (a2dp_demo_bond_info_flush) {
			a2dp_demo_flush_bond_info(A2DP_DEMO_BOND_INFO_FLUSH_EVENT);
		}
		printf("[BR GAP] Set link key of %x: %x: %x: %x: %x: %x \r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
		break;
	}

	case RTK_BT_BR_GAP_ACL_DISCONN: {
		rtk_bt_br_acl_disc_t *p_acl_disc_event = (rtk_bt_br_acl_disc_t *)param;
		printf("[BR GAP] ACL disconnection %x: %x: %x: %x: %x: %x \r\n", 
				p_acl_disc_event->bd_addr[5], p_acl_disc_event->bd_addr[4],
				p_acl_disc_event->bd_addr[3], p_acl_disc_event->bd_addr[2],
				p_acl_disc_event->bd_addr[1], p_acl_disc_event->bd_addr[0]);
		printf("[BR GAP] ACL disc cause 0x%x \r\n", p_acl_disc_event->cause);
		/* cause 0x113 -> Remote User Terminate*/
		/* cause 0x108 -> Conn Timeout*/
		if (a2dp_demo_auto_reconnect) {
			if ((RTK_BT_ERR_HCI_GROUP | RTK_BT_HCI_ERR_REMOTE_USER_TERMINATE) == p_acl_disc_event->cause) {
				printf("[A2DP Demo] Deleting a2dp demo reconnect config \r\n");
				if (rt_kv_delete(a2dp_demo_filesystem_key) == 0) {
					printf("[A2DP Demo] Delete a2dp demo reconnect config success \r\n");
				}
			} else if ((RTK_BT_ERR_HCI_GROUP | RTK_BT_HCI_ERR_CONN_TIMEOUT) == p_acl_disc_event->cause) {
				printf("[A2DP Demo] Start reconnect \r\n");
				a2dp_demo_auto_reconnect_try_count = RTK_BT_DEMO_RECONNECT_COUNT;
				a2dp_demo_disconnection_reconnecting = 1;
				/* avoid another device inquiry req and paging our dut */
				rtk_bt_br_gap_set_radio_mode(RTK_BT_BR_GAP_RADIO_MODE_CONNECTABLE);
				osif_timer_start(&reconnect_timer);
			}
		}
		if (a2dp_demo_bond_info_flush) {
			a2dp_demo_flush_bond_info(A2DP_DEMO_BOND_INFO_FLUSH_EVENT);
		}
		break;
	}
	
	default:
		printf("%s, Unknown GAP RTK_BLE_EVT: %d\r\n", __func__, evt_code);
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

static rtk_bt_evt_cb_ret_t rtk_bt_avrcp_app_callback(uint8_t evt_code, void* param, uint32_t len)
{
	(void)len;
	uint8_t bd_addr[6];

	switch (evt_code) {
	case RTK_BT_AVRCP_EVT_CONN_IND: {
		memcpy((void *)bd_addr, param, 6);
		printf("[AVRCP] Receive AVRCP connection from %x: %x: %x: %x: %x: %x\r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
		break;
	}

	case RTK_BT_AVRCP_EVT_ABSOLUTE_VOLUME_SET: {
		rtk_bt_avrcp_absolute_volume_set_t *p_avrcp_absolute_volume_set_t = (rtk_bt_avrcp_absolute_volume_set_t *)param;
		uint8_t volume = p_avrcp_absolute_volume_set_t->volume;
		float audio_track_volume = 0;
		
		curr_volume = volume;
		audio_track_volume = 1.0 * curr_volume / VOL_MAX;
		rtk_bt_audio_track_set_volume(audio_track_volume, audio_track_volume);
		printf("[AVRCP] absolute volume set %.2f \r\n", audio_track_volume);
		break;
	}

	case RTK_BT_AVRCP_EVT_VOLUME_CHANGED: {
		rtk_bt_avrcp_volume_t *pvolume_t = (rtk_bt_avrcp_volume_t *)param;
		uint8_t volume = pvolume_t->volume;

		curr_volume = volume;
		printf("[AVRCP] Volume changed %d \r\n", curr_volume);
		break;
	}

	case RTK_BT_AVRCP_EVT_VOLUME_UP: {
		if (curr_volume < VOL_MAX) {
			curr_volume++;
		} else {
			curr_volume = VOL_MAX;
		}
		printf("[AVRCP] Volume up to %d \r\n", curr_volume);
		break;
	}

	case RTK_BT_AVRCP_EVT_VOLUME_DOWN: {
		if (curr_volume > VOL_MIN) {
			curr_volume--;
		} else {
			curr_volume = VOL_MIN;
		}
		printf("[AVRCP] Volume down to %d \r\n", curr_volume);
		break;
	}

	case RTK_BT_AVRCP_EVT_REG_VOLUME_CHANGED: {
		rtk_bt_avrcp_volume_change_req_t *p_volume_t = (rtk_bt_avrcp_volume_change_req_t *)param;
		uint8_t volume = p_volume_t->volume;
		float audio_track_volume = 0;
		
		curr_volume = volume;
		audio_track_volume = 1.0 * curr_volume / VOL_MAX;
		rtk_bt_audio_track_set_volume(audio_track_volume, audio_track_volume);
		printf("[AVRCP] volume request set %.2f \r\n", audio_track_volume);
		break;
	}

	case RTK_BT_AVRCP_EVT_CONN_CMPL: {
		printf("[AVRCP] Connection Completion \r\n");
		break;
	}

	case RTK_BT_AVRCP_EVT_PLAY_STATUS_CHANGED_REG_REQ: {
		rtk_bt_avrcp_sub_event_t *psub_event = (rtk_bt_avrcp_sub_event_t *)param;
		switch (psub_event->avrcp_play_status) {
		case 0x00: {
			printf("[AVRCP]: Stopped \r\n");
			break;
		}

		case 0x01: {
			printf("[AVRCP]: Playing \r\n");
			break;
		}

		case 0x02: {
			printf("[AVRCP]: Paused \r\n");
			if (a2dp_demo_disconnection_reconnecting) {
				rtk_bt_avrcp_play(remote_bd_addr);
				a2dp_demo_disconnection_reconnecting = 0;
			}
			break;
		}

		case 0x03: {
			printf("[AVRCP]: FWD_SEEK \r\n");
			break;
		}

		case 0x04: {
			printf("[AVRCP]: REV_SEEK \r\n");
			break;
		}

		case 0xFF: {
			printf("[AVRCP]: ERROR \r\n");
		}
		break; 

		default: {
			printf("[AVRCP]: Default 0x%x \r\n", psub_event->avrcp_play_status);
		}
		break;
		}
		break;
	}

	case RTK_BT_AVRCP_EVT_PLAY_REQ_EVENT: {
		printf("[AVRCP]: Play req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_PAUSE_REQ_EVENT: {
		printf("[AVRCP]: Pause req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_STOP_REQ_EVENT: {
		printf("[AVRCP]: Stop req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_FORWARD_REQ_EVENT: {
		printf("[AVRCP]: Forward req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_BACKWARD_REQ_EVENT: {
		printf("[AVRCP]: Backward req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_FAST_FORWARD_START_REQ_EVENT: {
		printf("[AVRCP]: Fast forward start req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_FAST_FORWARD_STOP_REQ_EVENT: {
		printf("[AVRCP]: Fast forward stop req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_REWIND_START_REQ_EVENT: {
		printf("[AVRCP]: Rewind start req \r\n");
		/* TODO */
		break;
	}

	case RTK_BT_AVRCP_EVT_REWIND_STOP_REQ_EVENT: {
		printf("[AVRCP]: Rewind stop req \r\n");
		/* TODO */
		break;
	}

	default: {
		printf("%s, Unknown GAP RTK_BLE_EVT: %d\r\n", __func__, evt_code);
		break;
	}
	}

	return RTK_BT_EVT_CB_OK;
}

static uint16_t rtk_bt_a2dp_sbc_parse_decoder_struct(rtk_bt_a2dp_codec_t *pa2dp_codec, rtk_bt_sbc_decode_t *psbc_decoder_t)
{
	if (pa2dp_codec->sbc.channel_mode == RTK_BT_A2DP_SBC_CHANNEL_MODE_MONO) {
		psbc_decoder_t->channel_mode = 1;
	} else {
		psbc_decoder_t->channel_mode = 2;
	}
	switch (pa2dp_codec->sbc.sampling_frequency) {
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_16KHZ:
			psbc_decoder_t->sampling_frequency = (uint32_t)16000;
			break;
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_32KHZ:
			psbc_decoder_t->sampling_frequency = (uint32_t)32000;
			break;
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_44_1KHZ:
			psbc_decoder_t->sampling_frequency = (uint32_t)44100;
			break;
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_48KHZ:
			psbc_decoder_t->sampling_frequency = (uint32_t)48000;
			break;
		default:
			printf("[A2DP] RTK Audio not support frequency %x \r\n", pa2dp_codec->sbc.sampling_frequency);
			return 1;
	}
	psbc_decoder_t->allocation_method = pa2dp_codec->sbc.allocation_method;
	psbc_decoder_t->block_length = pa2dp_codec->sbc.block_length;
	psbc_decoder_t->subbands = pa2dp_codec->sbc.subbands;
	psbc_decoder_t->min_bitpool = pa2dp_codec->sbc.min_bitpool;
	psbc_decoder_t->max_bitpool = pa2dp_codec->sbc.max_bitpool;
	psbc_decoder_t->sbc_dec_mode = (sbc_channel_mode_t)SBC_MODE_STANDARD;
	a2dp_demo_audio_track_hdl = rtk_bt_audio_track_add(RTK_BT_AUDIO_CODEC_SBC, (float)DEFAULT_AUDIO_LEFT_VOLUME, (float)DEFAULT_AUDIO_RIGHT_VOLUME, psbc_decoder_t->channel_mode, psbc_decoder_t->sampling_frequency, 0);
	if (!a2dp_demo_audio_track_hdl) {
		printf("[A2DP] bt audio track add fail \r\n");
		return 1;
	}

	return 0;
}

static uint16_t rtk_bt_a2dp_sbc_parse_encoder_struct(rtk_bt_a2dp_codec_t *pa2dp_codec, rtk_bt_sbc_encode_t *psbc_encoder_t)
{
	if (pa2dp_codec->sbc.channel_mode == RTK_BT_A2DP_SBC_CHANNEL_MODE_MONO) {
		psbc_encoder_t->channel_mode = SBC_CHANNEL_MODE_MONO;
	} else {
		psbc_encoder_t->channel_mode = SBC_CHANNEL_MODE_DUAL_CHANNEL;
	}
	switch (pa2dp_codec->sbc.sampling_frequency) {
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_16KHZ:
			psbc_encoder_t->sample_rate = (uint32_t)16000;
			break;
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_32KHZ:
			psbc_encoder_t->sample_rate = (uint32_t)32000;
			break;
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_44_1KHZ:
			psbc_encoder_t->sample_rate = (uint32_t)44100;
			break;
		case RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_48KHZ:
			psbc_encoder_t->sample_rate = (uint32_t)48000;
			break;
		default:
			printf("[A2DP] RTK Audio not support frequency %x \r\n", pa2dp_codec->sbc.sampling_frequency);
			return 1;
	}
	psbc_encoder_t->alloc_method = SBC_ALLOCATION_METHOD_SNR;
	psbc_encoder_t->blocks = 16;
	psbc_encoder_t->subbands = 8;
	psbc_encoder_t->bitpool = 0x35;
	psbc_encoder_t->sbc_enc_mode = (sbc_channel_mode_t)SBC_MODE_STANDARD;

	return 0;
}

// static uint16_t rtk_bt_a2dp_aac_parse_decoder_struct(rtk_bt_a2dp_codec_t *pa2dp_codec, rtk_bt_aac_decode_t *paac_decoder_t)
// {
// 	if (pa2dp_codec->aac.channel_number == RTK_BT_A2DP_AAC_CHANNEL_NUMBER_1) {
// 		paac_decoder_t->channels = 1;
// 	} else {
// 		paac_decoder_t->channels = 2;
// 	}
// 	switch (pa2dp_codec->aac.sampling_frequency) {
// 		case RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_8KHZ:
// 			paac_decoder_t->sample_freq = 8000;
// 			break;
// 		case RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_16KHZ:
// 			paac_decoder_t->sample_freq = 16000;
// 			break;
// 		case RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_32KHZ:
// 			paac_decoder_t->sample_freq = 32000;
// 			break;
// 		case RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_44_1KHZ:
// 			paac_decoder_t->sample_freq = 44100;
// 			break;
// 		case RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_48KHZ:
// 			paac_decoder_t->sample_freq = 48000;
// 			break;
// 		default:
// 			printf("[A2DP] RTK Audio not support frequency 11.025, 12, 22.05, 24, 64, 88.2, 96KHz \r\n");
// 			return 1;
// 	}
// 	switch (pa2dp_codec->aac.object_type) {
// 		case RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_2_AAC_LC:
// 			paac_decoder_t->object_type = RTK_AAC_AOT_MP2_AAC_LC;
// 			break;
		
// 		case RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_LC:
// 			paac_decoder_t->object_type = RTK_AAC_AOT_AAC_LC;
// 			break;
		
// 		case RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_LTP:
// 			paac_decoder_t->object_type = RTK_AAC_AOT_AAC_LTP;
// 			break;
		
// 		case RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_SCALABLE:
// 			paac_decoder_t->object_type = RTK_AAC_AOT_AAC_SCAL;
// 			break;

// 		default:
// 			printf("[A2DP] RTK Audio only support object type MP2_AAC_LC/MP4_AAC_LC/MP4_AAC_LTP/MP4_AAC_SCALABLE \r\n");
// 			return 1;
// 	}
// 	paac_decoder_t->bit_rate = pa2dp_codec->aac.bit_rate;
// 	paac_decoder_t->vbr = pa2dp_codec->aac.vbr_supported;
// 	rtk_bt_audio_track_add((float)DEFAULT_AUDIO_LEFT_VOLUME, (float)DEFAULT_AUDIO_RIGHT_VOLUME, paac_decoder_t->channels, paac_decoder_t->sample_freq, 0);
	
// 	return 0;
// }

static rtk_bt_evt_cb_ret_t rtk_bt_a2dp_app_callback(uint8_t evt_code, void* param, uint32_t len)
{
	(void)len;
	uint8_t bd_addr[6];

	switch (evt_code) {
	
	case RTK_BT_A2DP_EVT_SDP_ATTR_INFO: {
		rtk_bt_a2dp_sdp_attr_info_t *p_info = (rtk_bt_a2dp_sdp_attr_info_t *)param;
		printf("[A2DP] SDP Scan %x: %x: %x: %x: %x: %x\r\n", 
			p_info->bd_addr[5], p_info->bd_addr[4],
			p_info->bd_addr[3], p_info->bd_addr[2],
			p_info->bd_addr[1], p_info->bd_addr[0]);
	}
	break;

	case RTK_BT_A2DP_EVT_CONN_IND: {
		rtk_bt_a2dp_conn_ind_t *conn_ind = (rtk_bt_a2dp_conn_ind_t *)param;
		memcpy((void *)bd_addr, conn_ind->bd_addr, 6);
		printf("[A2DP] Receive A2DP connection from %x: %x: %x: %x: %x: %x\r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	}
	break;

	case RTK_BT_A2DP_EVT_CONN_CMPL: {
		rtk_bt_a2dp_conn_ind_t *conn_ind = (rtk_bt_a2dp_conn_ind_t *)param;
		src_a2dp_credits = RTK_BT_A2DP_SRC_STREAM_MAX_CREDITS;
		memcpy((void *)remote_bd_addr, conn_ind->bd_addr, 6);
		printf("[A2DP] A2DP connection completion with %x: %x: %x: %x: %x: %x\r\n", 
				remote_bd_addr[5], remote_bd_addr[4], remote_bd_addr[3], remote_bd_addr[2], remote_bd_addr[1], remote_bd_addr[0]);
	}
	break;

	case RTK_BT_A2DP_EVT_DISCONN_CMPL: {
		rtk_bt_a2dp_disconn_ind_t *disconn_ind = (rtk_bt_a2dp_disconn_ind_t *)param;
		rtk_bt_audio_codec_remove(audio_codec_conf.codec_index, a2dp_demo_codec_entity);
		memset((void *)&audio_codec_conf, 0, sizeof(rtk_bt_audio_codec_conf_t));
		rtk_bt_audio_deinit();
		a2dp_demo_audio_track_hdl = NULL;
		a2dp_demo_codec_entity = NULL;
		memcpy((void *)bd_addr, disconn_ind->bd_addr, 6);
		printf("[A2DP] A2DP disconnection completion with %x: %x: %x: %x: %x: %x\r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	}
	break;

	case RTK_BT_A2DP_EVT_CONFIG_CMPL: {
		uint16_t ret = 1;
		rtk_bt_a2dp_codec_t *pa2dp_codec = (rtk_bt_a2dp_codec_t*)param;
		if ((pa2dp_codec->codec_type & RTK_BT_AUDIO_CODEC_SBC/* | RTK_BT_AUDIO_CODEC_AAC */) == 0) {
			printf("[A2DP] Not support codec %d \r\n", pa2dp_codec->codec_type);
			break;
		}
		if (rtk_bt_audio_init()) {
			printf("[A2DP] rtk_bt_audio_init fail \r\n");
			break;
		}
		if (a2dp_demo_role == RTK_BT_A2DP_ROLE_SNK) {
			if (pa2dp_codec->codec_type == RTK_BT_AUDIO_CODEC_SBC) {
				ret = rtk_bt_a2dp_sbc_parse_decoder_struct(pa2dp_codec, &sbc_codec_t.decoder_t);
				audio_codec_conf.codec_index = RTK_BT_AUDIO_CODEC_SBC;
				audio_codec_conf.param = (void *)&sbc_codec_t;
			} /* else if (pa2dp_codec->codec_type == RTK_BT_AUDIO_CODEC_AAC) {
				ret = rtk_bt_a2dp_aac_parse_decoder_struct(pa2dp_codec, &aac_codec_t.decoder_t);
				audio_codec_conf.codec_index = RTK_BT_AUDIO_CODEC_AAC;
				audio_codec_conf.param = (void *)&aac_codec_t;
			} */
			if (ret) {
				printf("[A2DP] RTK_BT_A2DP_EVT_CONFIG_CMPL Fail \r\n");
				rtk_bt_audio_deinit();
				a2dp_demo_audio_track_hdl = NULL;
				a2dp_demo_codec_entity = NULL;
				break;
			}
		} else {
			if (pa2dp_codec->codec_type == RTK_BT_AUDIO_CODEC_SBC) {
				ret = rtk_bt_a2dp_sbc_parse_encoder_struct(pa2dp_codec, &sbc_codec_t.encoder_t);
				audio_codec_conf.codec_index = RTK_BT_AUDIO_CODEC_SBC;
				audio_codec_conf.param = (void *)&sbc_codec_t;
			}
		}
		a2dp_demo_codec_entity = rtk_bt_audio_codec_add(&audio_codec_conf);
		// bt_a2dp_sink_event_handle(A2DP_SINK_STREAM_DATA_HANDLE_INIT, A2DP_SBC_CODEC, 0, NULL);
		printf("[A2DP] Configure Complete CODEC %d \r\n", pa2dp_codec->codec_type);
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_OPEN: {
		printf("[A2DP] A2DP STREAM is opened \r\n");
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_START_IND: {
		rtk_bt_a2dp_stream_start_t *pa2dp_stream = (rtk_bt_a2dp_stream_start_t *)param;

		printf("[A2DP] BT_EVENT_A2DP_STREAM_START_IND active_a2dp_idx %d, streaming_fg %d \r\n", 
				pa2dp_stream->active_a2dp_link_index, pa2dp_stream->stream_cfg);
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_START_RSP: {
		rtk_bt_a2dp_stream_start_t *pa2dp_stream = (rtk_bt_a2dp_stream_start_t *)param;

		printf("[A2DP] BT_EVENT_A2DP_STREAM_START_IND active_a2dp_idx %d, streaming_fg %d \r\n", 
				pa2dp_stream->active_a2dp_link_index, pa2dp_stream->stream_cfg);
		
		{
			printf("[A2DP Demo] Create Test Demo \r\n");
			if (false == osif_sem_create(&a2dp_task.sem, 0, 1))
				return -1;

			a2dp_task.run = 1;
			if (false == osif_task_create(&a2dp_task.hdl, "a2dp_task",
											a2dp_task_entry, NULL,
											2048, 4)) {
				osif_sem_delete(a2dp_task.sem);
				return -1;
			}
			osif_sem_take(a2dp_task.sem, 0xffffffff);
		}
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_STOP: {
		rtk_bt_a2dp_conn_ind_t *conn_ind = (rtk_bt_a2dp_conn_ind_t *)param;
		memcpy((void *)bd_addr, conn_ind->bd_addr, 6);
		printf("[A2DP] Stream stop from %x: %x: %x: %x: %x: %x\r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_CLOSE: {
		rtk_bt_a2dp_conn_ind_t *conn_ind = (rtk_bt_a2dp_conn_ind_t *)param;
		memcpy((void *)bd_addr, conn_ind->bd_addr, 6);
		printf("[A2DP] Stream close from %x: %x: %x: %x: %x: %x\r\n", 
				bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_DATA_IND: { //BT sync api shall not be called here
		rtk_bt_a2dp_stream_data_ind_t *pdata_in = (rtk_bt_a2dp_stream_data_ind_t *)param;

		if (rtk_bt_audio_recvd_data_in(RTK_BT_AUDIO_CODEC_SBC, a2dp_demo_audio_track_hdl, a2dp_demo_codec_entity, pdata_in->data, pdata_in->length)) {
			printf("[A2DP] Stream Data Receiving FAIL %d \r\n", RTK_BT_AUDIO_CODEC_SBC);
		}
	}
	break;

	case RTK_BT_A2DP_EVT_STREAM_DATA_RSP: {
		if (a2dp_demo_role == RTK_BT_A2DP_ROLE_SRC) {
			src_a2dp_credits++;
			if (src_a2dp_credits > RTK_BT_A2DP_SRC_STREAM_MAX_CREDITS) {
				src_a2dp_credits = RTK_BT_A2DP_SRC_STREAM_MAX_CREDITS;
			}
		}
	}
	break;

	default: {
		printf("[A2DP]: default evt_code 0x%04x \r\n", evt_code);
	}
	break;
	}

	return RTK_BT_EVT_CB_OK;
}

static void a2dp_demo_reconnect_timer_handle(void *arg)
{
	(void)arg;

	if (a2dp_demo_auto_reconnect_try_count --) {
		printf("[A2DP]: try reconnect to %x: %x: %x: %x: %x: %x\r\n", remote_bd_addr[5], remote_bd_addr[4], remote_bd_addr[3], remote_bd_addr[2], remote_bd_addr[1], remote_bd_addr[0]);
		rtk_bt_a2dp_connect(remote_bd_addr);
		osif_timer_restart(&reconnect_timer, RTK_BT_DEMO_RECONNECT_TIMER_INTERVAL);
	} else {
		/* auto reconnect fail and enable inquiry scan and page scan */
		rtk_bt_br_gap_set_radio_mode(RTK_BT_BR_GAP_RADIO_MODE_VISIBLE_CONNECTABLE);
	}
}

extern bool rtk_bt_pre_enable(void);
/**
 * @brief  process a2dp main init/ deinit.
 * @param  enable[in]: 1: init. 0 deinit
 * @return none.
 */
int bt_a2dp_main(uint8_t role, uint8_t enable)
{
	rtk_bt_app_conf_t bt_app_conf = {0};
	rtk_bt_br_bd_addr_t bd_addr = {
		.type = (rtk_bt_br_addr_type_t)0,
		.bd_addr = {{0}},
	};
	char addr_str[30] = {0};
	char dev_name[30] = {0};

	if (1 == enable) {
		if (rtk_bt_pre_enable() == false) {
			printf("%s fail!\r\n", __func__);
			return -1;
		}

		//set GAP configuration
		bt_app_conf.app_profile_support = 	RTK_BT_PROFILE_A2DP | \
											RTK_BT_PROFILE_AVRCP | \
											RTK_BT_PROFILE_SDP;
		bt_app_conf.mtu_size = 180;
		bt_app_conf.prefer_all_phy = 0;
		bt_app_conf.prefer_tx_phy = 1 | 1<<1 | 1<<2;
		bt_app_conf.prefer_rx_phy = 1 | 1<<1 | 1<<2;
		bt_app_conf.max_tx_octets = 0x40;
		bt_app_conf.max_tx_time = 0x200;
		bt_app_conf.a2dp_role = role;

		rtk_bt_br_gap_default_param_t dft_param;

		dft_param.mtu_size = 180;
		BT_APP_PROCESS(rtk_bt_br_gap_set_default_param(&dft_param));
		/* set class of device */
		BT_APP_PROCESS(rtk_bt_br_gap_set_cod(&cod));
		/* set supervision_timeout */
		BT_APP_PROCESS(rtk_bt_br_gap_set_supvisiontimeout(&superv_time));
		BT_APP_PROCESS(rtk_bt_br_gap_set_inquiry_param(&inquiry_param));
		BT_APP_PROCESS(rtk_bt_br_gap_set_page_param(&page_param));
		/* sdp record add (must be invoked before rtk_bt_enable) */
		BT_APP_PROCESS(rtk_sdp_record_add((void *)did_sdp_record, sizeof(did_sdp_record)));
		if (RTK_BT_A2DP_ROLE_SNK == role) {
			BT_APP_PROCESS(rtk_sdp_record_add((void *)a2dp_sink_sdp_record, sizeof(a2dp_sink_sdp_record)));
			a2dp_demo_role = RTK_BT_A2DP_ROLE_SNK;
		} else if (RTK_BT_A2DP_ROLE_SRC == role) {
			BT_APP_PROCESS(rtk_sdp_record_add((void *)a2dp_src_sdp_record, sizeof(a2dp_src_sdp_record)));
			a2dp_demo_role = RTK_BT_A2DP_ROLE_SRC;
		} else {
			printf("[APP] Wrong A2DP role %d \r\n", role);
			return 1;
		}
		BT_APP_PROCESS(rtk_sdp_record_add((void *)avrcp_ct_sdp_record, sizeof(avrcp_ct_sdp_record)));
		BT_APP_PROCESS(rtk_sdp_record_add((void *)avrcp_tg_sdp_record, sizeof(avrcp_tg_sdp_record)));

		/* a2dp codec add (must be invoked before rtk_bt_enable) */
		BT_APP_PROCESS(rtk_bt_a2dp_codec_cfg((uint8_t)RTK_BT_AUDIO_CODEC_SBC, (void *)&codec_sbc, (uint32_t)sizeof(rtk_bt_a2dp_media_codec_sbc_t)));
		// BT_APP_PROCESS(rtk_bt_a2dp_codec_cfg((uint8_t)RTK_BT_AUDIO_CODEC_AAC, (void *)&codec_aac, (uint32_t)sizeof(rtk_bt_a2dp_media_codec_sbc_t)));

		/* Enable BT */
		BT_APP_PROCESS(rtk_bt_enable(&bt_app_conf));

		BT_APP_PROCESS(rtk_bt_br_gap_get_address(&bd_addr));
		rtk_bt_le_addr_to_str(&bd_addr, addr_str, sizeof(addr_str));
		printf("[APP] BD_ADDR: %s\r\n", addr_str);

		/* Initilize GAP part */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_BR_GP_GAP, br_gap_app_callback));
		/* mix RTK_BT_DEV_NAME with bt mac address */
		strcpy(dev_name, RTK_BT_DEV_NAME);
		snprintf(&dev_name[strlen(RTK_BT_DEV_NAME)], 7, "(%02X%02X)", bd_addr.bd_addr.addr[1], bd_addr.bd_addr.addr[0]);
		BT_APP_PROCESS(rtk_bt_br_gap_set_device_name((const uint8_t*)dev_name));

		/* Initilize avrcp part */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_BR_GP_AVRCP, rtk_bt_avrcp_app_callback));

		/* Initilize a2dp part */
		BT_APP_PROCESS(rtk_bt_evt_register_callback(RTK_BT_BR_GP_A2DP, rtk_bt_a2dp_app_callback));
		/* auto reconnect init */
		if (a2dp_demo_auto_reconnect) {
			printf("[A2DP Demo] Create auto reaconnect timer \r\n");
			if (false == osif_timer_create(&reconnect_timer, "a2dp_demo_reconnect_timer", NULL,
															RTK_BT_DEMO_RECONNECT_TIMER_INTERVAL,
															false,
															a2dp_demo_reconnect_timer_handle)) {
				printf("[APP Demo] Create reconnect timer fail \r\n");
				a2dp_demo_auto_reconnect = 0;
				return 1;
			} else {
				a2dp_demo_auto_reconnect_try_count = RTK_BT_DEMO_RECONNECT_COUNT;
			}
			/* Load reconnect info from file system */
			if (rt_kv_get(a2dp_demo_filesystem_key, (void *)remote_bd_addr, 6) == 1) {
				printf("[A2DP Demo] Load a2dp demo reconnect data success \r\n");
				/* avoid another device inquiry req and paging our dut */
				rtk_bt_br_gap_set_radio_mode(RTK_BT_BR_GAP_RADIO_MODE_CONNECTABLE);
				osif_timer_start(&reconnect_timer);
			} else {
				printf("[A2DP Demo] Fail to load a2dp reconnect data \r\n");
			}
		}
		/* bond info flush thread init */
		if (a2dp_demo_bond_info_flush) {
			if (rtk_bt_br_gap_bond_num_get(&a2dp_demo_bond_num)) {
				printf("[A2DP Demo] Get Bond Number fail\r\n");
				goto failed;
			}
			/* Load reconnect info from file system */
			if (rt_kv_get(a2dp_demo_bond_info_key, (void *)a2dp_demo_bond_table, sizeof(a2dp_demo_bond_table)) == 1) {
				printf("[A2DP Demo] Load a2dp demo bond info table success \r\n");
				/* dump bond info */
				a2dp_demo_bond_info_dump();
			} else {
				printf("[A2DP Demo] Fail to load a2dp bond table \r\n");
			}
			printf("[A2DP Demo] Create bond info flush thread \r\n");
			if (false == osif_sem_create(&bond_info_flush_task.sem, 0, 1))
				goto failed;
			if (false == osif_msg_queue_create(&a2dp_demo_bond_info_flush_msg_q, 0x0F, sizeof(T_IO_MSG))) {
				goto failed;
			}
			bond_info_flush_task.run = 1;
			if (false == osif_task_create(&bond_info_flush_task.hdl, "a2dp_demo_bond_info_flush_thread",
											a2dp_demo_bond_flush_thread, NULL,
											2048, 4)) {
				osif_sem_delete(bond_info_flush_task.sem);
				goto failed;
			}
			/* Wait until event task is running */
			if (false == osif_sem_take(bond_info_flush_task.sem, 0xffffffff)) {
				goto failed;
			}
		}
	} else if (0 == enable) {
		/* auto reconnect deinit */
		if (a2dp_demo_auto_reconnect) {
			osif_timer_stop(&reconnect_timer);
			osif_timer_delete(&reconnect_timer);
			a2dp_demo_auto_reconnect_try_count = RTK_BT_DEMO_RECONNECT_COUNT;
			a2dp_demo_disconnection_reconnecting = 0;
		}
		if (a2dp_demo_bond_info_flush) {
			/* indicate a2dp demo bond info flush thread to kill itself */
			a2dp_demo_flush_bond_info(A2DP_DEMO_BOND_INFO_THREAD_EXIT);
			if (false == osif_sem_take(bond_info_flush_task.sem, 0xffffffff)) {
				return -1;
			}
			osif_sem_delete(bond_info_flush_task.sem);
			osif_msg_queue_delete(a2dp_demo_bond_info_flush_msg_q);
			bond_info_flush_task.sem = NULL;
			bond_info_flush_task.hdl = NULL;
			a2dp_demo_bond_info_flush_msg_q = NULL;
		}
		/* audio related resources release */
		rtk_bt_audio_deinit();
		a2dp_demo_audio_track_hdl = NULL;
		a2dp_demo_codec_entity = NULL;
		memset((void *)&audio_codec_conf, 0, sizeof(rtk_bt_audio_codec_conf_t));
		BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_BR_GP_GAP));
		BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_BR_GP_AVRCP));
		BT_APP_PROCESS(rtk_bt_evt_unregister_callback(RTK_BT_BR_GP_A2DP));
		/* Disable BT */
		BT_APP_PROCESS(rtk_bt_disable());
	}

	return 0;

failed:
	if (bond_info_flush_task.hdl) {
		osif_task_delete(bond_info_flush_task.hdl);
	}
	if (bond_info_flush_task.sem) {
		osif_sem_delete(bond_info_flush_task.sem);
	}
	if (a2dp_demo_bond_info_flush_msg_q) {
		osif_msg_queue_delete(a2dp_demo_bond_info_flush_msg_q);
	}

	return -1;
}