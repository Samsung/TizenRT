/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <log_service.h>
#include <bt_api_config.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <bt_utils.h>
#include <atcmd_bt_impl.h>
#include <rtk_bt_common.h>

static uint8_t def_adv_data[] = {
	0x02, //AD len
	RTK_BT_LE_GAP_ADTYPE_FLAGS, //AD types
	RTK_BT_LE_GAP_ADTYPE_FLAGS_GENERAL | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED, //AD data
	0x12,
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'R', 'T', 'K', '_', 'B', 'T', '_', 'P', 'E', 'R', 'I', 'P', 'H', 'E', 'R', 'A', 'L',
};

#if RTK_BLE_5_0_AE_ADV_SUPPORT
static uint8_t def_ext_adv_data[] = {
	// Flags
	0x02,
	RTK_BT_LE_GAP_ADTYPE_FLAGS,
	RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
	// Local name
	0x13,
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'B', 'L', 'E', '_', 'B', 'T', '5', '_', 'P', 'e', 'r', 'i', 'p', 'h', 'e', 'r', 'a', 'l',
	// Manufacturer Specific Data
	0xe3,
	RTK_BT_LE_GAP_ADTYPE_MANUFACTURER_SPECIFIC,
	0x5d, 0x00,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
	0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
	0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
	0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
	0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
	0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9,
	0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa,
	0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb,
	0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
	0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd,
};

#define ATCMD_DEFAULT_EXT_ADV_LEN		235

static uint8_t def_ext_scan_data[] = {
	//GAP_ADTYPE_APPEARANCE
	0x3,
	RTK_BT_LE_GAP_ADTYPE_APPEARANCE,
	LO_WORD(RTK_BT_LE_GAP_APPEARANCE_UNKNOWN),
	HI_WORD(RTK_BT_LE_GAP_APPEARANCE_UNKNOWN),
	// Manufacturer Specific Data
	0xf6,
	RTK_BT_LE_GAP_ADTYPE_MANUFACTURER_SPECIFIC,
	0x5d, 0x00,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4,
	0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5,
	0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
	0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7,
	0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
	0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9,
	0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa,
	0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb, 0xb,
	0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
	0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd, 0xd,
	0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe,
	0xf, 0xf, 0xf,
};
#endif

#if RTK_BLE_5_0_PA_ADV_SUPPORT
static uint8_t def_pa_data[] = {
	// Flags
	0x02,
	RTK_BT_LE_GAP_ADTYPE_FLAGS,
	RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED | RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
	// Local name
	0x13,
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE,
	'B', 'L', 'E', '_', 'B', 'T', '_', 'P', 'e', 'r', 'i', 'o', 'd', 'i', 'c', 'A', 'D', 'V',
	// Manufacturer Specific Data
	0xe4,
	RTK_BT_LE_GAP_ADTYPE_MANUFACTURER_SPECIFIC,
	0xe1, 0x00,
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
	0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
	0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
	0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0,
	0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
	0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xf0,
};
#endif

static uint8_t def_scan_rsp_data[] = {
	/* ad_len */
	0x3,
	/* ad_type */
	RTK_BT_LE_GAP_ADTYPE_APPEARANCE,
	/* ad_data */
	LO_WORD(RTK_BT_LE_GAP_APPEARANCE_GENERIC_PHONE),
	HI_WORD(RTK_BT_LE_GAP_APPEARANCE_GENERIC_PHONE),
};

static rtk_bt_le_adv_param_t def_adv_param = {
	.interval_min = 0x30,
	.interval_max = 0x60,
	.type = RTK_BT_LE_ADV_TYPE_IND,
	.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
	.peer_addr = {
		.type = (rtk_bt_le_addr_type_t)0,
		.addr_val = {0},
	},
	.channel_map = RTK_BT_LE_ADV_CHNL_ALL,
	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

#if RTK_BLE_5_0_AE_ADV_SUPPORT
static rtk_bt_le_ext_adv_param_t def_ext_adv_param = {
	.adv_event_prop = RTK_BT_LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED,
	.primary_adv_interval_min = 320,
	.primary_adv_interval_max = 320,
	.primary_adv_channel_map = RTK_BT_LE_ADV_CHNL_ALL,
	.own_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0}},
	.peer_addr = {RTK_BT_LE_ADDR_TYPE_PUBLIC, {0}},//;{0x8A, 0xAA, 0xAA, 0x4C, 0xE0, 0x00},
	.filter_policy = RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
	.tx_power = 0x7F,
	.primary_adv_phy = RTK_BT_LE_PHYS_PRIM_ADV_1M,
	.secondary_adv_max_skip = 0,
	.secondary_adv_phy = RTK_BT_LE_PHYS_CODED,
	.adv_sid = 0,
};
#endif

static rtk_bt_le_scan_param_t def_scan_param = {
	.type          = RTK_BT_LE_SCAN_TYPE_ACTIVE,
	.interval      = 0x60,
	.window        = 0x30,
	.filter_policy = RTK_BT_LE_SCAN_FILTER_ALLOW_ALL,
	.duplicate_opt = 1,
};

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
static rtk_bt_le_ext_scan_param_t def_ext_scan_param = {
	.own_addr_type      = RTK_BT_LE_ADDR_TYPE_PUBLIC,
	.phys               = {true, true},
	.type               = {RTK_BT_LE_SCAN_TYPE_ACTIVE, RTK_BT_LE_SCAN_TYPE_ACTIVE},
	.interval           = {108, 108},
	.window             = {54, 54},

	.duration           = 0,
	.period             = 0,

	.filter_policy      = RTK_BT_LE_SCAN_FILTER_ALLOW_ALL,
	.duplicate_opt      = RTK_BT_LE_SCAN_DUPLICATE_ENABLE,
};
#endif

static rtk_bt_le_create_conn_param_t def_conn_param = {
	.peer_addr = {
		.type = (rtk_bt_le_addr_type_t)0,
		.addr_val = {0},
	},
	.scan_interval = 0x60,
	.scan_window = 0x30,
	.filter_policy = RTK_BT_LE_CONN_FILTER_WITHOUT_WHITELIST,
	.conn_interval_max = 0x60,
	.conn_interval_min = 0x60,
	.conn_latency      = 0,
	.supv_timeout      = 0x100,
	.scan_timeout      = 1000,
};

#if RTK_BLE_5_0_AE_ADV_SUPPORT || RTK_BLE_5_0_AE_SCAN_SUPPORT
static rtk_bt_le_ext_create_conn_param_t def_ext_conn_param = {
	.filter_policy = RTK_BT_LE_CONN_FILTER_WITHOUT_WHITELIST,
	.own_addr_type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
	.peer_addr = {
		.type = RTK_BT_LE_ADDR_TYPE_PUBLIC,
		.addr_val = {0},
	},
	.init_phys = {true, true, true},
	.scan_interval = {0x60, 0x60, 0x60},
	.scan_window = {0x30, 0x30, 0x30},
	.conn_interval_min = {0x60, 0x60, 0x60},
	.conn_interval_max = {0x60, 0x60, 0x60},
	.conn_latency = {0, 0, 0},
	.supv_timeout = {0x100, 0x100, 0x100},
	.scan_timeout = 1000,
};
#endif

static int atcmd_ble_gap_get_version(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	rtk_bt_le_version_info_t version = {0};

	ret = rtk_bt_le_gap_get_version(&version);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get version failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP get version, hci_revision: 0x%x, lmp_subversion: 0x%x, "
			  "btgap_revision: 0x%x, btgap_buildnum: 0x%x ",
			  version.hci_revision, version.lmp_subversion,
			  version.btgap_revision, version.btgap_buildnum);
	return 0;
}

static int atcmd_ble_gap_get_address(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	rtk_bt_le_addr_t addr;
	char addr_str[30] = {0};

	ret = rtk_bt_le_gap_get_address(&addr);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get address failed! err: 0x%x", ret);
		return -1;
	}

	rtk_bt_le_addr_to_str(&addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] GAP get address: %s", addr_str);
	return 0;
}

static int atcmd_ble_gap_set_dev_name(int argc, char **argv)
{
	uint16_t ret = 0;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	if (strlen(argv[0]) == 0 || strlen(argv[0]) > 39) {
		AT_PRINTK("[ATBC] GAP set device name failed! too long!");
		return -1;
	}

	ret = rtk_bt_le_gap_set_device_name((const uint8_t *)argv[0]);
	if (ret) {
		AT_PRINTK("[ATBC] GAP set device name failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set device name success");
	return 0;
}

static int atcmd_ble_gap_set_appearance(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t appearance = 0;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	appearance = str_to_int(argv[0]);

	ret = rtk_bt_le_gap_set_appearance(appearance);
	if (ret) {
		AT_PRINTK("[ATBC] GAP set device appearance failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set device appearance success");
	return 0;
}

static int atcmd_ble_gap_set_preferred_conn_param(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_preferred_conn_param_t preferred_conn_param = {0};

	if (argc < 4) {
		AT_PRINTK("[ATBC] GAP set preferred conn param failed! argc number(%d) error!", argc);
		return -1;
	}

	preferred_conn_param.conn_interval_min = str_to_int(argv[0]);
	preferred_conn_param.conn_interval_max = str_to_int(argv[1]);
	preferred_conn_param.slave_latency = str_to_int(argv[2]);
	preferred_conn_param.supv_timeout =  str_to_int(argv[3]);

	AT_PRINTK("[ATBC] conn_interval_min=0x%x,conn_interval_max=0x%x,slave_latency=0x%x,supervision_timeout=0x%x",
			  preferred_conn_param.conn_interval_min,
			  preferred_conn_param.conn_interval_max,
			  preferred_conn_param.slave_latency,
			  preferred_conn_param.supv_timeout);

	ret = rtk_bt_le_gap_set_preferred_conn_param(&preferred_conn_param);
	if (ret) {
		AT_PRINTK("[ATBC] GAP set preferred conn param failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set preferred conn param success");
	return 0;
}

static int atcmd_ble_gap_set_rand_addr(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_rand_addr_type_t type = RTK_BT_LE_RAND_ADDR_STATIC;
	uint8_t addr[RTK_BD_ADDR_LEN] = {0};
	bool auto_generate;

	if (argc != 1 && argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	if (argc == 1) {
		if (hexdata_str_to_bd_addr(argv[0], addr, RTK_BD_ADDR_LEN) == false) {
			return -1;
		}
		auto_generate = false;
	} else if (argc == 2) {
		type = str_to_int(argv[1]);
		auto_generate = true;
	}

	ret = rtk_bt_le_gap_set_rand_addr(auto_generate, type, addr);
	if (ret) {
		AT_PRINTK("[ATBC] GAP set random address failed! err: 0x%x", ret);
		return -1;
	}

	return 0;
}


static int atcmd_ble_gap_set_channel_map(int argc, char **argv)
{
	(void)argc;
	uint16_t ret = 0;
	uint8_t chan_map[RTK_BT_LE_CHAN_MAP_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	if (false == hexnum_str_to_array(argv[0], chan_map, 5) ||
		(ret = rtk_bt_le_gap_set_channels(chan_map)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP set host channel failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set channel map success");
	return 0;
}

static int atcmd_ble_gap_set_adv_data(int argc, char **argv)
{
	uint16_t ret = 0;
	uint8_t adv_data[RTK_BT_LE_MAX_ADV_DATA_LEN] = {0};
	uint16_t adv_len  = 0;

	if (argc != 0 && argc != 1) {
		AT_PRINTK("[ATBC] GAP set adv data failed! wrong args num!");
		return -1;
	}

	if (argc == 0) {
		ret = rtk_bt_le_gap_set_adv_data(def_adv_data, sizeof(def_adv_data));
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP set default adv data failed! err: 0x%x", ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP set default adv data success");
		return 0;
	}

	adv_len = strlen(argv[0]) / 2;
	if (adv_len > 31) {
		AT_PRINTK("[ATBC] GAP set adv data failed! too long!");
		return -1;
	}

	if (false == hexdata_str_to_array(argv[0], adv_data, adv_len) ||
		(ret = rtk_bt_le_gap_set_adv_data(adv_data, adv_len)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP set adv data failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set adv data success");
	return 0;
}

static int atcmd_ble_gap_op_adv(int argc, char **argv)
{
	rtk_bt_le_adv_param_t adv_param;
	uint16_t ret = 0;
	int en;

	if (argc != 1 && argc != 4 && argc != 6 && argc != 9) {
		AT_PRINTK("[ATBC] GAP adv op failed! wrong args num!");
		return -1;
	}

	en = str_to_int(argv[0]);
	if (0 == en) {
		ret = rtk_bt_le_gap_stop_adv();
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP stop adv failed! err: 0x%x", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP stopping adv ...");
			return 0;
		}
	} else if (1 == en) {
		memcpy(&adv_param, &def_adv_param, sizeof(rtk_bt_le_adv_param_t));
		if (argc >= 4) {
			adv_param.type = (rtk_bt_le_adv_type_t)str_to_int(argv[1]);
			adv_param.own_addr_type = (rtk_bt_le_addr_type_t)str_to_int(argv[2]);
			adv_param.filter_policy = (rtk_bt_le_adv_filter_t)str_to_int(argv[3]);
		}
		if (argc >= 6) {
			adv_param.peer_addr.type = (rtk_bt_le_addr_type_t)str_to_int(argv[4]);
			if (false == hexdata_str_to_bd_addr(argv[5], (uint8_t *)adv_param.peer_addr.addr_val, RTK_BD_ADDR_LEN)) {
				return -1;
			}
		}
		if (argc >= 9) {
			adv_param.interval_min = str_to_int(argv[6]);
			adv_param.interval_max = str_to_int(argv[7]);
			adv_param.channel_map = (rtk_bt_le_adv_ch_map_t)str_to_int(argv[8]);
		}
		ret = rtk_bt_le_gap_start_adv(&adv_param);
		if (ret != RTK_BT_OK) {
			AT_PRINTK("[ATBC] GAP start adv failed! err: 0x%x", ret);
			return -1;
		} else {
			/* here, we use [starting] and with [...] means it is on processing(async operation),
			and the result still need to be get from the callback */
			AT_PRINTK("[ATBC] GAP starting adv ...");
		}
	}

	return 0;
}

static int atcmd_ble_gap_set_scan_resp(int argc, char **argv)
{
	(void)argc;
	uint16_t ret = 0;
	uint32_t scan_resp_len = 0;
	uint8_t scan_resp_data[32] = {0};

	if (argc != 0 && argc != 1) {
		AT_PRINTK("[ATBC] GAP set scan resp data failed! wrong args num!");
		return -1;
	}

	if (argc == 0) {
		ret = rtk_bt_le_gap_set_scan_rsp_data(def_scan_rsp_data, sizeof(def_scan_rsp_data));
		if (ret) {
			AT_PRINTK("[ATBC] GAP set default scan resp data failed! err: %d", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP set def scan resp data success");
			return 0;
		}
	}

	scan_resp_len = strlen(argv[0]) / 2;
	if (scan_resp_len > 31) {
		AT_PRINTK("[ATBC] GAP set scan resp data failed! too long!");
		return -1;
	}

	if (false == hexdata_str_to_array(argv[0], scan_resp_data, scan_resp_len) ||
		(ret = rtk_bt_le_gap_set_scan_rsp_data(scan_resp_data, scan_resp_len)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP set scan resp data failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set scan resp data success");
	return 0;
}

static int atcmd_ble_gap_set_ext_adv_data(int argc, char **argv)
{
#if RTK_BLE_5_0_AE_ADV_SUPPORT
	uint16_t ret = 0;
	uint8_t adv_handle = 0;
	uint8_t *pdata = (uint8_t *)def_ext_adv_data;
	uint8_t len = ATCMD_DEFAULT_EXT_ADV_LEN;

	if (argc > 0) {
		adv_handle = str_to_int(argv[0]);
	}

	if (argc == 2) {
		len = str_to_int(argv[1]);
		if (len > sizeof(def_ext_adv_data)) {
			AT_PRINTK("[ATBC] GAP set ext adv data failed! Length too long!");
			return -1;
		}
	}

	ret = rtk_bt_le_gap_set_ext_adv_data(adv_handle, pdata, len);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP set ext adv(%d) data failed! err: 0x%x", adv_handle, ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set ext adv(%d) data success", adv_handle);
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support extended ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_op_ext_adv(int argc, char **argv)
{
#if RTK_BLE_5_0_AE_ADV_SUPPORT
	rtk_bt_le_ext_adv_param_t adv_param;
	uint16_t ret = 0;
	uint8_t operation;
	uint8_t adv_handle = 0;
	uint16_t duration = 0;

	operation = str_to_int(argv[0]);
	if (0 == operation) { //stop ext adv
		if (argc > 1) {
			adv_handle = str_to_int(argv[1]);
		}
		ret = rtk_bt_le_gap_stop_ext_adv(adv_handle);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP stop ext adv(%d) failed! err: 0x%x", adv_handle, ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP stopping ext adv(%d) ...", adv_handle);
		}
	} else if (1 == operation) { //start ext adv
		if (argc > 1) {
			adv_handle = str_to_int(argv[1]);
		}
		if (argc > 2) {
			duration = str_to_int(argv[2]);
		}
		ret = rtk_bt_le_gap_start_ext_adv(adv_handle, duration);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP start ext adv(%d) failed! err: 0x%x", adv_handle, ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP starting ext adv(%d) ...", adv_handle);
		}
	} else if (2 == operation) { //create ext adv handle
		memcpy(&adv_param, &def_ext_adv_param, sizeof(rtk_bt_le_ext_adv_param_t));

		if (argc > 1) {
			adv_param.adv_sid = str_to_int(argv[1]);
		}
		if (argc > 2) {
			adv_param.adv_event_prop = (rtk_bt_le_adv_event_prop_t)str_to_int(argv[2]);
		}
		if (argc > 3) {
			uint8_t phy = str_to_int(argv[3]);
			adv_param.primary_adv_phy = (rtk_bt_le_phy_prim_adv_t)((phy >> 4) & 0xF);
			adv_param.secondary_adv_phy = (rtk_bt_le_phy_type_t)(phy & 0xF);
		}
		if (argc > 4) {
			adv_param.filter_policy = (rtk_bt_le_adv_filter_t)str_to_int(argv[4]);
		}

		if (argc > 5) {
			adv_param.tx_power = (int8_t)str_to_int(argv[5]);
			printf("adv_param.tx_power = %d\r\n", adv_param.tx_power);
		}
		if (argc > 6) {
			adv_param.primary_adv_interval_min = str_to_int(argv[6]);
			adv_param.primary_adv_interval_max = adv_param.primary_adv_interval_min + 10;
		}
		if (argc > 7) {
			adv_param.own_addr.type = (rtk_bt_le_addr_type_t)str_to_int(argv[7]);
			if (adv_param.own_addr.type == RTK_BT_LE_ADDR_TYPE_RANDOM) {
				if (false == hexdata_str_to_bd_addr(argv[8], (uint8_t *)adv_param.own_addr.addr_val, RTK_BD_ADDR_LEN)) {
					return -1;
				}
			}
		}
		if (argc > 9) {
			adv_param.peer_addr.type = (rtk_bt_le_addr_type_t)str_to_int(argv[9]);
			if (false == hexdata_str_to_bd_addr(argv[10], (uint8_t *)adv_param.peer_addr.addr_val, RTK_BD_ADDR_LEN)) {
				return -1;
			}
		}

		ret = rtk_bt_le_gap_create_ext_adv(&adv_param, &adv_handle);

		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP create ext adv failed! err: 0x%x", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP create ext adv success! adv handle: %d", adv_handle);
		}
	} else if (3 == operation) { //delete ext adv handle
		if (argc > 1) {
			adv_handle = str_to_int(argv[1]);
		}
		ret = rtk_bt_le_gap_remove_ext_adv(adv_handle);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP remove ext adv(%d) failed! err: 0x%x", adv_handle, ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP remove ext adv(%d) success!", adv_handle);
		}
	} else {
		AT_PRINTK("[ATBC] GAP ext adv operation failed: wrong operation value: %d!", operation);
		return -1;
	}

	return 0;

#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support extended ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_set_ext_scan_resp(int argc, char **argv)
{
#if RTK_BLE_5_0_AE_ADV_SUPPORT
	uint16_t ret = 0;
	uint8_t adv_handle = 0;
	uint8_t *pdata = (uint8_t *)def_ext_scan_data;
	uint8_t len = sizeof(def_ext_scan_data);

	if (argc > 0) {
		adv_handle = str_to_int(argv[0]);
	}

	if (argc == 2) {
		len = str_to_int(argv[1]);
		if (len > sizeof(def_ext_scan_data)) {
			AT_PRINTK("[ATBC] GAP set ext scan resp data failed! Length too long!");
			return -1;
		}
	}

	ret = rtk_bt_le_gap_set_ext_scan_rsp_data(adv_handle, pdata, len);
	if (ret) {
		AT_PRINTK("[ATBC] GAP set ext scan resp data(%d) failed! err: %d", adv_handle, ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set ext scan resp data(%d) success", adv_handle);
	return 0;

#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support extended ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_pa_adv(int argc, char **argv)
{
#if RTK_BLE_5_0_PA_ADV_SUPPORT
	uint16_t ret = 0;
	uint8_t operation, adv_handle = 0;
	rtk_bt_le_pa_param_t param = {0, 64, 80, RTK_BT_LE_PA_ADV_PROP_INCLUDE_TX_POWER, (uint8_t *)def_pa_data, sizeof(def_pa_data)};

	operation = str_to_int(argv[0]);

	if (operation == 1) { //enable pa
		if (argc > 1) {
			param.adv_handle = str_to_int(argv[1]);
		}

		if (argc > 2) {
			param.pa_interval_min = str_to_int(argv[2]);
			param.pa_interval_max = param.pa_interval_min + 10;
		}

		if (argc > 3) {
			param.pa_prop = str_to_int(argv[3]) ? RTK_BT_LE_PA_ADV_PROP_INCLUDE_TX_POWER : 0;
		}

		if (argc > 4) {
			param.pa_len = str_to_int(argv[4]);
		}

		ret = rtk_bt_le_gap_start_pa(&param);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP start periodic adv(%d) failed! err: 0x%x", param.adv_handle, ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP starting periodic adv(%d) ...", param.adv_handle);

		return 0;
	} else if (operation == 0) { //stop pa
		if (argc > 1) {
			adv_handle = str_to_int(argv[1]);
		}

		ret = rtk_bt_le_gap_stop_pa(adv_handle);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP stop periodic adv(%d) failed! err: 0x%x", adv_handle, ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP stopping periodic adv(%d) ...", adv_handle);

		return 0;
	} else {
		AT_PRINTK("[ATBC] GAP periodic adv operation failed: wrong operation value: %d!", operation);
		return -1;
	}
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Periodic ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_pa_adv_update(int argc, char **argv)
{
#if RTK_BLE_5_0_PA_ADV_SUPPORT
	uint16_t ret = 0;
	uint8_t adv_handle = 0;
	bool update_DID_only = true;
	uint8_t *data = NULL;
	uint16_t len = 0;

	adv_handle = str_to_int(argv[0]);

	if (argc != 1 && argc != 3) {
		AT_PRINTK("[ATBC] GAP set scan paramters failed! wrong args num!");
		return -1;
	}

	if (argc == 3) {
		update_DID_only = false;
		data = (uint8_t *)argv[1];
		len = str_to_int(argv[2]);
	}

	ret = rtk_bt_le_gap_update_pa(adv_handle, update_DID_only, data, len);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP Update periodic adv(%d) failed! err: 0x%x", adv_handle, ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP update periodic adv(%d) success", adv_handle);
	return 0;

#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Periodic ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_pa_sync_get_param(int argc, char **argv)
{
#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	uint16_t ret = 0;
	uint8_t sync_id = 0;
	uint32_t type, value;

	type = str_to_int(argv[0]);

	if (argc > 1) {
		sync_id = str_to_int(argv[1]);
	}

	if (type > RTK_BT_LE_PA_SYNC_PARAM_DEV_STATE) {
		AT_PRINTK("[ATBC] Invalid parameter type!");
		return -1;
	}

	if (type == RTK_BT_LE_PA_SYNC_PARAM_SYNCHRONIZED_PARAM) {
		char le_addr[30];
		rtk_bt_le_addr_t addr;
		rtk_bt_le_pa_sync_sync_param_t param;
		if (argc == 1) {
			AT_PRINTK("[ATBC] Wrong args num!");
			return -1;
		}

		if (rtk_bt_le_gap_pa_sync_get_param(type, &param, sync_id) != RTK_BT_OK) {
			AT_PRINTK("[ATBC] Get PA synchronization parameter failed! err: 0x%x", ret);
			return -1;
		}

		AT_PRINTK("[ATBC] Get PA synchronization parameter Success!");
		addr.type = param.adv_addr_type;
		memcpy(addr.addr_val, param.adv_addr, RTK_BD_ADDR_LEN);
		rtk_bt_le_addr_to_str(&addr, le_addr, sizeof(le_addr));
		printf("sync_handle:%d, adv_sid:%d [Device]: %s, skip:%d, sync_timeout:%d, ",
			   param.sync_handle, param.adv_sid, le_addr, param.skip, param.sync_timeout);
		printf("sync_cte_type:%d, adv_phy:%d, adv_clock_accuracy:0x%x, ",
			   param.sync_cte_type, param.adv_phy, param.adv_clock_accuracy);
		printf("periodic_adv_interval:%d, sync_transfer_received_flag:%d\r\n",
			   param.periodic_adv_interval, param.sync_transfer_received_flag);
		return 0;
	}

	ret = rtk_bt_le_gap_pa_sync_get_param(type, &value, sync_id);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] Get PA synchronization parameter failed! err: 0x%x", ret);
		return -1;
	}
	AT_PRINTK("[ATBC] Get PA synchronization parameter Success! param=0x%x", (unsigned int)value);
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Periodic ADV Sync.");
	return -1;
#endif
}

static int atcmd_ble_gap_pa_sync_modify_list(int argc, char **argv)
{
#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	uint16_t ret = 0;
	uint8_t op;
	rtk_bt_le_addr_t addr;
	uint8_t adv_sid;

	op = str_to_int(argv[0]);

	if (op != RTK_BT_LE_PA_SYNC_ADV_LIST_OP_CLEAR) {
		if (argc != 4) {
			AT_PRINTK("[ATBC] Wrong args num!");
			return -1;
		}

		adv_sid = str_to_int(argv[1]);
		addr.type = str_to_int(argv[2]);
		if (false == hexdata_str_to_bd_addr(argv[3], addr.addr_val, RTK_BD_ADDR_LEN)) {
			return -1;
		}
	}

	ret = rtk_bt_le_gap_pa_sync_modify_adv_list(op, addr, adv_sid);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP modify PA sync adv list failed! err: 0x%x", ret);
		return -1;
	}
	AT_PRINTK("[ATBC] GAP modify PA sync adv list Success!");

	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Periodic ADV Sync.");
	return -1;
#endif
}

static int atcmd_ble_gap_pa_sync_op(int argc, char **argv)
{
#if RTK_BLE_5_0_PA_SYNC_SUPPORT
	uint16_t ret = 0;
	uint8_t op;
	uint8_t sync_id;

	op = str_to_int(argv[0]);
	if (op == 1) { //create sync
		rtk_bt_le_pa_sync_create_t create = {
			.options = RTK_BT_LE_PA_SYNC_CREATE_OPTIONS_REPORT_INITIALLY_DISABLED,
			.sync_cte_type = 0,
			.adv_sid = 0,
			.adv_addr = {0, {0, 0, 0, 0, 0, 0}},
			.skip = 0,
			.sync_timeout = 0x1000,
			.p_sync_id = &sync_id,
		};

		if (argc > 1) {
			create.options = str_to_int(argv[1]);
		}

		if (argc > 2) {
			create.sync_cte_type = str_to_int(argv[2]);
		}

		if (!(create.options & RTK_BT_LE_PA_SYNC_CREATE_OPTIONS_USE_PERIODIC_ADV_LIST)) {
			if (argc > 3) {
				create.adv_sid = str_to_int(argv[3]);
			}

			if (argc > 5) {
				create.adv_addr.type = str_to_int(argv[4]);
				if (false == hexdata_str_to_bd_addr(argv[5], create.adv_addr.addr_val, RTK_BD_ADDR_LEN)) {
					return -1;
				}
			}
		}

		if (argc > 6) {
			create.skip = str_to_int(argv[6]);
		}

		if (argc > 7) {
			create.sync_timeout = str_to_int(argv[7]);
		}

		ret = rtk_bt_le_gap_pa_sync_create(&create);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP PA sync create fail! err: 0x%x", ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP PA sync create success, sync_id = %d!", sync_id);
	} else if (op == 0) {
		if (argc != 2) {
			AT_PRINTK("[ATBC] Wrong args num!");
			return -1;
		}
		sync_id = str_to_int(argv[1]);
		ret = rtk_bt_le_gap_pa_sync_terminate(sync_id);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP PA sync terminate fail! err: 0x%x", ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP PA sync terminate success, sync_id = %d!", sync_id);
	}

	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Periodic ADV Sync.");
	return -1;
#endif
}

static int atcmd_ble_gap_past_send(int argc, char **argv)
{
	(void)argc;
#if RTK_BLE_5_1_PAST_SENDER_SUPPORT
	(void)argc;
	uint16_t ret = 0;
	uint16_t conn_handle;
	uint8_t service_data, use_sync_id, idx;

	conn_handle = str_to_int(argv[0]);
	service_data = str_to_int(argv[1]);
	use_sync_id = str_to_int(argv[2]);
	idx = str_to_int(argv[3]);

	ret = rtk_bt_le_gap_past_send(conn_handle, service_data, use_sync_id ? true : false, idx);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP PAST transfer fail! err: 0x%x", ret);
		return -1;
	}
	AT_PRINTK("[ATBC] GAP PAST transfer success!");

	return 0;
#else
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support PAST.");
	return -1;
#endif
}

static int atcmd_ble_gap_past_recv(int argc, char **argv)
{
#if RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_past_recv_param_t param = {
		.conn_handle = 0,
		.mode = RTK_BT_LE_PAST_RECV_MODE_PA_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER,
		.cte_type = 0,
		.skip = 0,
		.sync_timeout = 0x1000,
	};

	if (argc > 0) {
		param.conn_handle = str_to_int(argv[0]);
	}

	if (argc > 1) {
		param.mode = str_to_int(argv[1]);
	}

	if (argc > 2) {
		param.cte_type = str_to_int(argv[2]);
	}

	if (argc > 3) {
		param.skip = str_to_int(argv[3]);
	}

	if (argc > 4) {
		param.sync_timeout = str_to_int(argv[4]);
	}

	ret = rtk_bt_le_gap_past_recipient_set(&param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP PAST recipient set params fail! conn_handle: %d, err: 0x%x", param.conn_handle, ret);
		return -1;
	}
	AT_PRINTK("[ATBC] GAP PAST recipient set params success! conn_handle: %d", param.conn_handle);

	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support PAST.");
	return -1;
#endif
}

static int atcmd_ble_gap_default_past_recv(int argc, char **argv)
{
#if RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_past_recv_param_t param = {
		.conn_handle = 0,
		.mode = RTK_BT_LE_PAST_RECV_MODE_PA_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER,
		.cte_type = 0,
		.skip = 0,
		.sync_timeout = 0x1000,
	};

	// if(argc > 0)
	//     param.conn_handle = str_to_int(argv[0]);

	if (argc > 0) {
		param.mode = str_to_int(argv[0]);
	}

	if (argc > 1) {
		param.cte_type = str_to_int(argv[1]);
	}

	if (argc > 2) {
		param.skip = str_to_int(argv[2]);
	}

	if (argc > 3) {
		param.sync_timeout = str_to_int(argv[3]);
	}

	ret = rtk_bt_le_gap_default_past_recipient_set(&param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP PAST recipient set default params fail! err: 0x%x", ret);
		return -1;
	}
	AT_PRINTK("[ATBC] GAP PAST recipient set default params success!");

	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support PAST.");
	return -1;
#endif
}

static int atcmd_ble_gap_set_scan_param(int argc, char **argv)
{
	(void)argc;
	uint16_t ret = 0;
	rtk_bt_le_scan_param_t scan_param;

	if (argc != 0 && argc != 4 && argc != 6) {
		AT_PRINTK("[ATBC] GAP set scan paramters failed! wrong args num!");
		return -1;
	}

	memcpy(&scan_param, &def_scan_param, sizeof(rtk_bt_le_scan_param_t));
	if (argc == 0) {
		; /* do nothing, just use default def_scan_param */
	}
	if (argc >= 4) {
		scan_param.type = (rtk_bt_le_scan_type_t)str_to_int(argv[0]);
		scan_param.own_addr_type = (rtk_bt_le_addr_type_t)str_to_int(argv[1]);
		scan_param.filter_policy = (rtk_bt_le_scan_filter_t)str_to_int(argv[2]);
		scan_param.duplicate_opt = (uint8_t)str_to_int(argv[3]);
	}
	if (argc >= 6) {
		scan_param.interval = str_to_int(argv[4]);
		scan_param.window = str_to_int(argv[5]);
	}
	ret = rtk_bt_le_gap_set_scan_param(&scan_param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP set scan param failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set scan param success");
	return 0;
}

static int atcmd_ble_gap_op_scan(int argc, char **argv)
{
	uint16_t ret = 0;
	if (argc != 1) {
		AT_PRINTK("[ATBC] GAP scan op failed! wrong args num!");
		return -1;
	}

	if (1 == str_to_int(argv[0])) {
		ret = rtk_bt_le_gap_start_scan();
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP start scan failed! err: 0x%x", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP starting scan ...");
		}
	} else if (0 == str_to_int(argv[0])) {
		ret = rtk_bt_le_gap_stop_scan();
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP stop scan failed! err: 0x%x", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP stopping scan ...");
		}
	} else {
		AT_PRINTK("[ATBC] GAP scan op failed! wrong args!");
		return -1;
	}

	return 0;
}

static int atcmd_ble_gap_ext_scan_set_param(int argc, char **argv)
{
#if RTK_BLE_5_0_AE_SCAN_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_ext_scan_param_t scan_param;
	uint8_t scan_phys;

	if (argc != 0 && argc != 3 && argc != 6 && argc != 9 && argc != 12) {
		AT_PRINTK("[ATBC] GAP set scan paramters failed! wrong args num!");
		return -1;
	}

	if (argc == 0) {
		ret = rtk_bt_le_gap_ext_scan_set_param(&def_ext_scan_param);
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP set default ext scan paramters failed! err: 0x%x", ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP set default ext scan paramters success");
		return 0;
	}

	memcpy(&scan_param, &def_ext_scan_param, sizeof(rtk_bt_le_ext_scan_param_t));

	scan_param.own_addr_type = (rtk_bt_le_addr_type_t)str_to_int(argv[0]);
	scan_param.filter_policy = (rtk_bt_le_scan_filter_t)str_to_int(argv[1]);
	scan_param.duplicate_opt = (uint8_t)str_to_int(argv[2]);

	if (argc > 3) {
		scan_param.duration = (uint16_t)str_to_int(argv[3]);
		scan_param.period = str_to_int(argv[4]);
		scan_phys = str_to_int(argv[5]);
		scan_param.phys[0] = (scan_phys & 1 << 0) ? true : false;
		scan_param.phys[1] = (scan_phys & 1 << 2) ? true : false;
	}

	if (argc > 6 && scan_param.phys[0]) {
		scan_param.type[0] = (rtk_bt_le_scan_type_t)str_to_int(argv[6]);
		scan_param.interval[0] = str_to_int(argv[7]);
		scan_param.window[0] = str_to_int(argv[8]);
	}

	if (argc > 9 && scan_param.phys[1]) {
		scan_param.type[1] = (rtk_bt_le_scan_type_t)str_to_int(argv[9]);
		scan_param.interval[1] = str_to_int(argv[10]);
		scan_param.window[1] = str_to_int(argv[11]);
	}
	ret = rtk_bt_le_gap_ext_scan_set_param(&scan_param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP set scan param failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set scan param success");
	return 0;

#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support extended ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_op_ext_scan(int argc, char **argv)
{
	(void)argc;
#if RTK_BLE_5_0_AE_SCAN_SUPPORT
	uint16_t ret = 0;

	if (1 == str_to_int(argv[0])) {
		ret = rtk_bt_le_gap_ext_scan_start();
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP start ext scan failed! err: 0x%x", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP starting ext scan ...");
		}
	} else if (0 == str_to_int(argv[0])) {
		ret = rtk_bt_le_gap_ext_scan_stop();
		if (RTK_BT_OK != ret) {
			AT_PRINTK("[ATBC] GAP stop ext scan failed! err: 0x%x", ret);
			return -1;
		} else {
			AT_PRINTK("[ATBC] GAP stopping ext scan ...");
		}
	} else {
		AT_PRINTK("[ATBC] GAP ext scan op failed! wrong args!");
		return -1;
	}

	return 0;
#else
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support extended ADV.");
	return -1;
#endif
}

static int atcmd_ble_gap_connect(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint16_t ret = 0;
	rtk_bt_le_create_conn_param_t conn_param;

	if (argc != 2 && argc != 4 && argc != 10) {
		AT_PRINTK("[ATBC] GAP connect op failed! wrong args num!");
		return -1;
	}
	memcpy(&conn_param, &def_conn_param, sizeof(rtk_bt_le_create_conn_param_t));
	if (argc >= 2) {
		conn_param.peer_addr.type = (rtk_bt_le_addr_type_t)str_to_int(argv[0]);
		if (false == hexdata_str_to_bd_addr(argv[1], conn_param.peer_addr.addr_val, RTK_BD_ADDR_LEN)) {
			return -1;
		}
	}
	if (argc >= 4) {
		conn_param.own_addr_type = (rtk_bt_le_addr_type_t)str_to_int(argv[2]);
		conn_param.filter_policy = (rtk_bt_le_conn_filter_t)str_to_int(argv[3]);
	}
	if (argc >= 10) {
		conn_param.scan_interval = str_to_int(argv[4]);
		conn_param.scan_window = str_to_int(argv[5]);
		conn_param.conn_interval_min = str_to_int(argv[6]);
		conn_param.conn_interval_max = str_to_int(argv[7]);
		conn_param.conn_latency = str_to_int(argv[8]);
		conn_param.supv_timeout = str_to_int(argv[9]);
	}
	ret = rtk_bt_le_gap_connect(&conn_param);
	if (ret != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP connect ops failed! err: 0x%x", ret);
		return -1;
	}

	rtk_bt_le_addr_to_str(&conn_param.peer_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] GAP connecting to device %s ...", addr_str);
	return 0;
}

static int atcmd_ble_gap_ext_connect(int argc, char **argv)
{
#if RTK_BLE_5_0_AE_ADV_SUPPORT || RTK_BLE_5_0_AE_SCAN_SUPPORT
	char addr_str[30] = {0};
	rtk_bt_le_ext_create_conn_param_t ext_conn_param;
	int i = 0;
	uint8_t phys_bit_mask = 0;
	uint16_t ret = 0;

	if (argc != 3 && argc != 5 && argc != 11) {
		AT_PRINTK("[ATBC] GAP connect op failed! wrong args num!");
		return -1;
	}
	memcpy(&ext_conn_param, &def_ext_conn_param,
		   sizeof(rtk_bt_le_ext_create_conn_param_t));

	if (argc >= 3) {
		ext_conn_param.peer_addr.type = (rtk_bt_le_addr_type_t)str_to_int(argv[0]);
		if (false == hexdata_str_to_bd_addr(argv[1], ext_conn_param.peer_addr.addr_val, RTK_BD_ADDR_LEN)) {
			return -1;
		}
		phys_bit_mask = (uint8_t)str_to_int(argv[2]);
		ext_conn_param.init_phys[0] = (phys_bit_mask & 1 << 0) ? true : false;
		ext_conn_param.init_phys[1] = (phys_bit_mask & 1 << 1) ? true : false;
		ext_conn_param.init_phys[2] = (phys_bit_mask & 1 << 2) ? true : false;
		;
	}
	if (argc >= 5) {
		ext_conn_param.own_addr_type = (rtk_bt_le_addr_type_t)str_to_int(argv[3]);
		ext_conn_param.filter_policy = (rtk_bt_le_conn_filter_t)str_to_int(argv[4]);
	}
	if (argc >= 11) {
		for (i = 0; i < 3; i++) {
			if (0 == ext_conn_param.init_phys[i]) {
				continue;
			}
			ext_conn_param.scan_interval[i] = str_to_int(argv[5]);
			ext_conn_param.scan_window[i] = str_to_int(argv[6]);
			ext_conn_param.conn_interval_min[i] = str_to_int(argv[7]);
			ext_conn_param.conn_interval_max[i] = str_to_int(argv[8]);
			ext_conn_param.conn_latency[i] = str_to_int(argv[9]);
			ext_conn_param.supv_timeout[i] = str_to_int(argv[10]);
		}
	}
	/* atcmd has limitation with argc num, so we have no choice but let
	    all phys use a same set of params */
#if 0
	uint8_t init_phy_num = 0;
	int j = 0;
	else if (9 == argc || 15 == argc || 21 == argc) {
		init_phy_num = ((ext_conn_param.init_phys[0] == true) ? 1 : 0) +
					   ((ext_conn_param.init_phys[1] == true) ? 1 : 0) +
					   ((ext_conn_param.init_phys[2] == true) ? 1 : 0);
		if (argc != (init_phy_num) * 6 + 3) {
			AT_PRINTK("[ATBC] GAP ext connection failed! wrong args num: %d, %d init phys "
					  "need %d args!", argc, init_phy_num, (init_phy_num) * 6 + 3);
			return -1;
		}

		j = 3;
		for (i = 0; i < 3; i++) {
			if (0 == ext_conn_param.init_phys[i]) {
				continue;
			}
			ext_conn_param.scan_interval[i] = str_to_int(argv[j++]);
			ext_conn_param.scan_window[i] = str_to_int(argv[j++]);
			ext_conn_param.conn_interval_min[i] = str_to_int(argv[j++]);
			ext_conn_param.conn_interval_max[i] = str_to_int(argv[j++]);
			ext_conn_param.conn_latency[i] = str_to_int(argv[j++]);
			ext_conn_param.sup_timeout[i] = str_to_int(argv[j++]);
		}
	}
#endif

	if ((ret = rtk_bt_le_gap_ext_connect(&ext_conn_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP ext connect failed! err: 0x%x", ret);
		return -1;
	}
	rtk_bt_le_addr_to_str(&ext_conn_param.peer_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] GAP extended connecting to device %s ...", addr_str);
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support extended connection.");
	return -1;
#endif
}

static int atcmd_ble_gap_get_conn_rssi(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle = 0;
	int8_t rssi_val = 0;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_handle = str_to_int(argv[0]);
	if ((ret = rtk_bt_le_gap_read_rssi(conn_handle, &rssi_val)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP read rssi failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP read rssi, conn_hanlde: %d, value: %d", conn_handle, rssi_val);
	return 0;
}

static int atcmd_ble_gap_get_dev_state(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	rtk_bt_le_gap_dev_state_t dev_state = {0};

	ret = rtk_bt_le_gap_get_dev_state(&dev_state);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP get dev state failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP dev state, init_state: %d, adv_state: %d, scan_state: %d, conn_state: %d",
			  dev_state.gap_init_state, dev_state.gap_adv_state,
			  dev_state.gap_scan_state, dev_state.gap_conn_state);
	return 0;
}

static int atcmd_ble_gap_get_active_conn(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	uint16_t i = 0;
	rtk_bt_le_get_active_conn_t active_conn = {0};

	ret = rtk_bt_le_gap_get_active_conn(&active_conn);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get active conn num failed! err: 0x%x", ret);
		return -1;
	}

	printf("[ATBC] GAP get active conn, num: %d, conn_handle:", active_conn.conn_num);
	for (i = 0; i < active_conn.conn_num; i++) {
		printf(" %d,", active_conn.conn_handle[i]);
	}
	printf("\r\n");
	return 0;
}

static int atcmd_ble_gap_get_conn_handle_by_addr(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle = 0xFF;
	rtk_bt_le_addr_t addr = {0};

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	addr.type = str_to_int(argv[0]);

	if (false == hexdata_str_to_bd_addr(argv[1], addr.addr_val, RTK_BD_ADDR_LEN) ||
		(ret = rtk_bt_le_gap_get_conn_handle_by_addr(&addr, &conn_handle)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP get conn handle by addr failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP get conn handle by addr: %d", conn_handle);
	return 0;
}

static int atcmd_ble_gap_get_conn_info(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle;
	char bd_addr_str[30] = {0};
	rtk_bt_le_conn_info_t conn_info;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_handle = str_to_int(argv[0]);
	if ((ret = rtk_bt_le_gap_get_conn_info(conn_handle, &conn_info)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP get connection info failed! err: 0x%x", ret);
		return -1;
	}

	rtk_bt_le_addr_to_str(&conn_info.remote, bd_addr_str, sizeof(bd_addr_str));
	AT_PRINTK("[ATBC] GAP get connection info: \n\r"
			  "conn_handle: %d, local role: %s, peer bd_addr: (%s), \n\r"
			  "interval: 0x%x, latency: 0x%x, supv_timeout: 0x%x, tx_phy: 0x%x, rx_phy: 0x%x",
			  conn_handle, (conn_info.role == RTK_BT_LE_ROLE_MASTER) ?
			  "master" : ((conn_info.role == RTK_BT_LE_ROLE_SLAVE) ? "slave" : "unknown"),
			  bd_addr_str, conn_info.interval, conn_info.latency,
			  conn_info.supv_timeout, conn_info.tx_phy, conn_info.rx_phy);
	return 0;
}

static int atcmd_ble_gap_get_mtu_size(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle;
	uint16_t mtu_size = 0;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_handle = str_to_int(argv[0]);
	if ((ret = rtk_bt_le_gap_get_mtu_size(conn_handle, &mtu_size)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP get mtu size failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP get mtu size, conn_handle: %d, mtu_size: %d", conn_handle, mtu_size);
	return 0;
}

static int atcmd_ble_gap_update_conn(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_update_conn_param_t up_conn_param;

	if (argc != 5) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	up_conn_param.conn_handle = str_to_int(argv[0]);
	up_conn_param.conn_interval_min = str_to_int(argv[1]);
	up_conn_param.conn_interval_max = str_to_int(argv[2]);
	up_conn_param.conn_latency = str_to_int(argv[3]);
	up_conn_param.supv_timeout = str_to_int(argv[4]);

	ret = rtk_bt_le_gap_update_conn_param(&up_conn_param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP update connect parameters ops failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP updating connection parameters ...");
	return 0;
}

static int atcmd_ble_gap_set_conn_data_len(int argc, char **argv)
{
#if RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_set_datalen_param_t data_len_param;

	if (argc != 3) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	data_len_param.conn_handle = str_to_int(argv[0]);
	data_len_param.max_tx_octets = str_to_int(argv[1]);
	data_len_param.max_tx_time = str_to_int(argv[2]);
	ret = rtk_bt_le_gap_set_data_len(&data_len_param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP set connection data len ops failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP setting connection data len ...");
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support set data len.");
	return -1;
#endif
}

static int atcmd_ble_gap_set_conn_phy(int argc, char **argv)
{
#if RTK_BLE_5_0_SET_PHYS_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_set_phy_param_t conn_phy_param = {0};

	if (argc != 5) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_phy_param.conn_handle = str_to_int(argv[0]);
	conn_phy_param.all_phys = str_to_int(argv[1]);
	conn_phy_param.tx_phys = str_to_int(argv[2]);
	conn_phy_param.rx_phys = str_to_int(argv[3]);
	conn_phy_param.phy_options = str_to_int(argv[4]);

	ret = rtk_bt_le_gap_set_phy(&conn_phy_param);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] GAP set connection PHY ops failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP setting connection PHY ...");
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support set PHY.");
	return -1;
#endif
}

static int atcmd_ble_gap_disconnect(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_handle = str_to_int(argv[0]);
	if ((ret = rtk_bt_le_gap_disconnect(conn_handle)) != 0) {
		AT_PRINTK("[ATBC] GAP disconnect ops failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP disconnecting ...");
	return 0;
}

static int atcmd_ble_gap_privacy_enable(int argc, char **argv)
{
#if RTK_BLE_PRIVACY_SUPPORT
	uint16_t ret = 0;
	bool whitelist = true;

	(void)argc;
	if (argc > 0) {
		whitelist = str_to_int(argv[0]);
	}

	ret = rtk_bt_le_gap_privacy_init(whitelist);

	if (ret) {
		AT_PRINTK("[ATBC] GAP privacy init failed! whitelist: %d err: 0x%x", whitelist, ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP privacy init success! whitelist: %d", whitelist);
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support privacy.");
	return -1;
#endif
}

static int atcmd_ble_gap_privacy_list(int argc, char **argv)
{
#if RTK_BLE_PRIVACY_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_modify_resolv_list_t param = {0};

	param.op = str_to_int(argv[0]);
	if (param.op != RTK_BT_LE_RESOLV_LIST_OP_CLEAR) {
		param.entry.addr_type = str_to_int(argv[1]);
		if (false == hexdata_str_to_bd_addr(argv[2], param.entry.addr, RTK_BD_ADDR_LEN)) {
			return -1;
		}
		if (argc > 3) {
			param.entry.device_mode = str_to_int(argv[3]);
		}
	}

	ret = rtk_bt_le_gap_privacy_modify_resolving_list(&param);
	if (ret) {
		AT_PRINTK("[ATBC] GAP privacy modify resolving list failed! op: %d err: 0x%x", param.op, ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP privacy modify resolving list success! op: %d", param.op);
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support privacy.");
	return -1;
#endif
}

static int atcmd_ble_gap_add_whilelist(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_modify_wl_param_t wl_op_param;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	wl_op_param.op = RTK_BT_LE_WHITELIST_ADD;
	wl_op_param.addr.type = str_to_int(argv[0]);

	if (false == hexdata_str_to_bd_addr(argv[1], (uint8_t *)wl_op_param.addr.addr_val, RTK_BD_ADDR_LEN) ||
		(ret = rtk_bt_le_gap_modify_whitelist(&wl_op_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP whitelist add failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP whitelist add success");
	return 0;
}

static int atcmd_ble_gap_remove_whilelist(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_modify_wl_param_t wl_op_param;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	wl_op_param.op = RTK_BT_LE_WHITELIST_REMOVE;
	wl_op_param.addr.type = str_to_int(argv[0]);

	if (false == hexdata_str_to_bd_addr(argv[1], (uint8_t *)wl_op_param.addr.addr_val, RTK_BD_ADDR_LEN) ||
		(ret = rtk_bt_le_gap_modify_whitelist(&wl_op_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP whitelist remove failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP whitelist remove success");
	return 0;
}

static int atcmd_ble_gap_clear_whilelist(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	rtk_bt_le_modify_wl_param_t wl_op_param;
	wl_op_param.op = RTK_BT_LE_WHITELIST_CLEAR;

	ret = rtk_bt_le_gap_modify_whitelist(&wl_op_param);
	if (ret) {
		AT_PRINTK("[ATBC] GAP whitelist clear failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP get whitelist clear success");
	return 0;
}

static int atcmd_ble_gap_set_security_param(int argc, char **argv)
{
	uint16_t ret = 0;
	if (argc != 0 && argc != 7) {
		AT_PRINTK("[ATBC] GAP set security paramters failed! wrong args num!");
		return -1;
	}

	if (argc == 0) {
		rtk_bt_le_security_param_t def_sec_param = {
			.io_cap = RTK_IO_CAP_KEYBOARD_DISPALY,
			.oob_data_flag = 0,
			.bond_flag = 1,
			.mitm_flag = 0,
			.sec_pair_flag = 1,
			.use_fixed_key = 0,
			.fixed_key = 000000,
		};
		ret = rtk_bt_le_sm_set_security_param(&def_sec_param);
		if (ret) {
			AT_PRINTK("[ATBC] GAP set default security paramters failed! err: 0x%x", ret);
			return -1;
		}
		AT_PRINTK("[ATBC] GAP set def security paramters sucess");
		return 0;
	}

	rtk_bt_le_security_param_t sec_param;
	sec_param.io_cap = (rtk_bt_le_io_cap_t)str_to_int(argv[0]);
	sec_param.oob_data_flag = (uint8_t)str_to_int(argv[1]);
	sec_param.bond_flag = (uint8_t)str_to_int(argv[2]);
	sec_param.mitm_flag = (uint8_t)str_to_int(argv[3]);
	sec_param.sec_pair_flag = (uint8_t)str_to_int(argv[4]);
	sec_param.use_fixed_key = (uint8_t)str_to_int(argv[5]);
	sec_param.fixed_key = atoi(argv[6]);

	if (RTK_BT_FAIL == rtk_bt_le_sm_set_security_param(&sec_param)) {
		AT_PRINTK("[ATBC] GAP set security paramters failed!");
		return -1;
	}

	AT_PRINTK("[ATBC] GAP set security paramters sucess");
	return 0;
}

static int atcmd_ble_gap_security(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_handle = str_to_int(argv[0]);
	if ((ret = rtk_bt_le_sm_start_security(conn_handle)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP start security flow failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP starting security flow ...");
	return 0;
}

static int atcmd_ble_gap_confirm_pair(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_pair_cfm_t pair_cfm_param;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	pair_cfm_param.conn_handle = str_to_int(argv[0]);
	if ((pair_cfm_param.confirm = str_to_int(argv[1])) > 1 ||
		(ret = rtk_bt_le_sm_pairing_confirm(&pair_cfm_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP pair confirm failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP pair confirm OK!");
	return 0;
}

static int atcmd_ble_gap_input_auth_key(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_auth_key_input_t auth_key_param;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	auth_key_param.conn_handle = str_to_int(argv[0]);
	if ((auth_key_param.passkey = atoi(argv[1])) > 999999 ||
		(ret = rtk_bt_le_sm_passkey_entry(&auth_key_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP input auth key failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP input auth key OK!");
	return 0;
}

static int atcmd_ble_gap_confirm_auth_key(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_auth_key_confirm_t auth_keycfm_param;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	auth_keycfm_param.conn_handle = str_to_int(argv[0]);
	if ((auth_keycfm_param.confirm = str_to_int(argv[1])) > 1 ||
		(ret = rtk_bt_le_sm_passkey_confirm(&auth_keycfm_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP auth key confirm failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP auth key confirm OK!");
	return 0;
}

static int atcmd_ble_gap_input_auth_oob(int argc, char **argv)
{
#if RTK_BLE_SMP_OOB_SUPPORT
	uint16_t ret = 0;
	rtk_bt_le_set_oob_key_t auth_oob_param;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	auth_oob_param.conn_handle = str_to_int(argv[0]);
	if (false == hexnum_str_to_array(argv[1], (uint8_t *)auth_oob_param.oob_key, RTK_BT_LE_SM_OOB_KEY_LEN) ||
		(ret = rtk_bt_le_sm_set_oob_tk(&auth_oob_param)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP input auth OOB TK failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP input auth OOB TK OK!");
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support OOB TK.");
	return -1;
#endif
}

static int atcmd_ble_gap_get_bond_num(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	uint8_t bond_num;

	ret = rtk_bt_le_sm_get_bond_num(&bond_num);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get bond number failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP get bond number: %d", bond_num);
	return 0;
}

static int atcmd_ble_gap_get_bond_info(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	char addr_str[30] = {0};
	char ident_addr_str[30] = {0};
	uint8_t bond_size;

	ret = rtk_bt_le_sm_get_bond_num(&bond_size);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get bond number failed! err: 0x%x", ret);
		return -1;
	}

	if (!bond_size) {
		AT_PRINTK("[ATBC] GAP no bond device!");
		return 0;
	}

	rtk_bt_le_bond_info_t *bond_info =
		(rtk_bt_le_bond_info_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,
				bond_size * sizeof(rtk_bt_le_bond_info_t));
	memset(bond_info, 0, bond_size * sizeof(rtk_bt_le_bond_info_t));
	ret = rtk_bt_le_sm_get_bond_info(bond_info, &bond_size);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get bond info failed! err:0x%x", ret);
		goto out;
	}

	AT_PRINTK("[ATBC] GAP get bond info: \n\r");
	for (int i = 0; i < bond_size; i++) {
		rtk_bt_le_addr_to_str(&bond_info[i].remote_addr, addr_str, sizeof(addr_str));
		rtk_bt_le_addr_to_str(&bond_info[i].ident_addr, ident_addr_str, sizeof(ident_addr_str));
		AT_PRINTK("[%d] %s, resolved: %s", i, addr_str, ident_addr_str);
	}

out:
	if (bond_info) {
		osif_mem_free(bond_info);
	}

	return ret;
}

static int atcmd_ble_gap_delete_bond(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_addr_t addr;

	if (argc != 2) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	addr.type = (rtk_bt_le_addr_type_t)str_to_int(argv[0]);

	if (false == hexdata_str_to_bd_addr(argv[1], (uint8_t *)addr.addr_val, RTK_BD_ADDR_LEN) ||
		(ret = rtk_bt_le_sm_delete_bond_device(&addr)) != RTK_BT_OK) {
		AT_PRINTK("[ATBC] GAP remove bond device failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP removing bond device ...");
	return 0;
}

static int atcmd_ble_gap_clear_bond(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;

	ret = rtk_bt_le_sm_clear_bond_list();
	if (ret) {
		AT_PRINTK("[ATBC] GAP clear bond info failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP clearing bond info ...");
	return 0;
}

static int atcmd_ble_gap_tx_pending_num(int argc, char **argv)
{
	uint16_t ret = 0;
	uint16_t conn_handle = 0, tx_pending_num = 0;

	if (argc != 1) {
		AT_PRINTK("%s, wrong args num: %d", __func__, argc);
		return -1;
	}

	conn_handle = (uint16_t)str_to_int(argv[0]);
	ret = rtk_bt_le_gap_get_tx_pending_num(conn_handle, &tx_pending_num);
	if (ret) {
		AT_PRINTK("[ATBC] GAP get tx pending number failed! err: 0x%x", ret);
		return -1;
	}
	AT_PRINTK("[ATBC] GAP get tx pending number, conn_handle: %d tx_pending_num: %d", conn_handle, tx_pending_num);

	return 0;
}

static int atcmd_ble_gap_vendor_cmd_req(int argc, char **argv)
{
	uint16_t ret = 0;
	rtk_bt_le_gap_vendor_cmd_param_t vendor_param = {0};

	if (argc != 3) {
		AT_PRINTK("[ATBC] GAP vendor cmd request failed, wrong args num");
		return -1;
	}

	vendor_param.op = (uint16_t)str_to_int(argv[0]);

	vendor_param.len = (uint8_t)str_to_int(argv[1]);
	vendor_param.cmd_param = (uint8_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,
							 vendor_param.len);
	hexnum_str_to_array(argv[2], vendor_param.cmd_param, vendor_param.len);
	ret = rtk_bt_le_gap_vendor_cmd_req(&vendor_param);
	if (ret) {
		AT_PRINTK("[ATBC] GAP vendor cmd reqeust failed, err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] GAP vendor cmd reqeust success");
	return 0;
}

static int atcmd_ble_gap_txpower_read(int argc, char **argv)
{
#if RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	(void)argc;
	uint16_t conn_handle, ret = 0;
	uint8_t type, phy;
	int8_t cur_txpower = 0, max_txpower = 0;

	type = str_to_int(argv[0]);
	conn_handle = str_to_int(argv[1]);
	phy = str_to_int(argv[2]);

	if (type == 0) { // read local
		ret = rtk_bt_le_gap_read_local_tx_power(conn_handle, (rtk_bt_le_txpower_phy_t)phy, &cur_txpower, &max_txpower);
	} else { // read remote
		ret = rtk_bt_le_gap_read_remote_tx_power(conn_handle, (rtk_bt_le_txpower_phy_t)phy);
	}

	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] Read tx power failed! err: 0x%x", ret);
		return -1;
	}
	if (type == 0) {
		AT_PRINTK("[ATBC] Read local tx power Success! curr: %d, max: %d", cur_txpower, max_txpower);
	} else {
		AT_PRINTK("[ATBC] Read remote tx power Success!");
	}

	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Power Control.");
	return -1;
#endif
}

static int atcmd_ble_gap_txpower_report_set(int argc, char **argv)
{
#if RTK_BLE_5_2_POWER_CONTROL_SUPPORT
	(void)argc;
	uint16_t conn_handle, ret = 0;
	bool local_enable, remote_enable;

	conn_handle = str_to_int(argv[0]);
	local_enable = str_to_int(argv[1]);
	remote_enable = str_to_int(argv[2]);

	ret = rtk_bt_le_gap_tx_power_report_set(conn_handle, local_enable, remote_enable);
	if (RTK_BT_OK != ret) {
		AT_PRINTK("[ATBC] Tx power reporting set failed! err: 0x%x", ret);
		return -1;
	}
	AT_PRINTK("[ATBC] Tx power reporting set Success!");
	return 0;
#else
	(void)argc;
	(void)argv;
	AT_PRINTK("[ATBC] Platform not support Power Control.");
	return -1;
#endif
}

static const cmd_table_t le_gap_cmd_table[] = {
	{"version",      atcmd_ble_gap_get_version,        1, 1},
	{"addr",         atcmd_ble_gap_get_address,        1, 1},
	{"name",         atcmd_ble_gap_set_dev_name,       2, 2},
	{"appear",       atcmd_ble_gap_set_appearance,     2, 2},
	{"pre_conn_param", atcmd_ble_gap_set_preferred_conn_param,     5, 5},
	{"rand_addr",    atcmd_ble_gap_set_rand_addr,      2, 3},
	{"channel",      atcmd_ble_gap_set_channel_map,    2, 2},
	{"adv_data",     atcmd_ble_gap_set_adv_data,       1, 2},
	{"adv",          atcmd_ble_gap_op_adv,             2, 10},
	{"scan_rsp",     atcmd_ble_gap_set_scan_resp,      1, 2},
	{"eadv",         atcmd_ble_gap_op_ext_adv,         2, 13},
	{"eadv_data",    atcmd_ble_gap_set_ext_adv_data,   1, 3},
	{"escan_rsp",    atcmd_ble_gap_set_ext_scan_resp,  1, 3},
	{"pa_adv",       atcmd_ble_gap_pa_adv,             2, 6},
	{"pa_update",    atcmd_ble_gap_pa_adv_update,      2, 4},
	{"pa_sync_param", atcmd_ble_gap_pa_sync_get_param, 2, 3},
	{"pa_sync_list", atcmd_ble_gap_pa_sync_modify_list, 2, 5},
	{"pa_sync",      atcmd_ble_gap_pa_sync_op,         2, 9},
	{"past_send",    atcmd_ble_gap_past_send,          5, 5},
	{"past_recv",    atcmd_ble_gap_past_recv,          1, 6},
	{"def_past_recv",    atcmd_ble_gap_default_past_recv,          1, 5},
	{"scan_param",   atcmd_ble_gap_set_scan_param,     1, 7},
	{"scan",         atcmd_ble_gap_op_scan,            2, 2},
	{"escan_param",  atcmd_ble_gap_ext_scan_set_param, 1, 13},
	{"escan",        atcmd_ble_gap_op_ext_scan,        2, 2},
	{"conn",         atcmd_ble_gap_connect,            3, 11},
	{"econn",        atcmd_ble_gap_ext_connect,        4, 12},
	{"conn_rssi",    atcmd_ble_gap_get_conn_rssi,      2, 2},
	{"dev_state",    atcmd_ble_gap_get_dev_state,      1, 1},
	{"get_active_conn", atcmd_ble_gap_get_active_conn, 1, 1},
	{"get_conn_handle", atcmd_ble_gap_get_conn_handle_by_addr, 3, 3},
	{"conn_info",    atcmd_ble_gap_get_conn_info,      2, 2},
	{"mtu_size",     atcmd_ble_gap_get_mtu_size,       2, 2},
	{"conn_update",  atcmd_ble_gap_update_conn,        6, 6},
	{"conn_datalen", atcmd_ble_gap_set_conn_data_len,  4, 4},
	{"conn_phy",     atcmd_ble_gap_set_conn_phy,       6, 6},
	{"disconn",      atcmd_ble_gap_disconnect,         2, 2},
	{"privacy",      atcmd_ble_gap_privacy_enable,     1, 2},
	{"privacy_list", atcmd_ble_gap_privacy_list,       2, 5},
	{"wl_add",       atcmd_ble_gap_add_whilelist,      3, 3},
	{"wl_remove",    atcmd_ble_gap_remove_whilelist,   3, 3},
	{"wl_clear",     atcmd_ble_gap_clear_whilelist,    1, 1},
	{"sec_param",    atcmd_ble_gap_set_security_param, 1, 8},
	{"sec",          atcmd_ble_gap_security,           2, 2},
	{"pair_cfm",     atcmd_ble_gap_confirm_pair,       3, 3},
	{"auth_key",     atcmd_ble_gap_input_auth_key,     3, 3},
	{"auth_keycfm",  atcmd_ble_gap_confirm_auth_key,   3, 3},
	{"auth_oob",     atcmd_ble_gap_input_auth_oob,     3, 3},
	{"bond_num",     atcmd_ble_gap_get_bond_num,       1, 1},
	{"bond_info",    atcmd_ble_gap_get_bond_info,      1, 1},
	{"bond_del",     atcmd_ble_gap_delete_bond,        3, 3},
	{"bond_clear",   atcmd_ble_gap_clear_bond,         1, 1},
	{"tx_pending_num", atcmd_ble_gap_tx_pending_num,   2, 2},
	{"vendor_cmd",   atcmd_ble_gap_vendor_cmd_req,     4, 4},
	{"txpower_read", atcmd_ble_gap_txpower_read,       4, 4},
	{"txpower_report", atcmd_ble_gap_txpower_report_set, 4, 4},
	{NULL,},
};

int atcmd_bt_le_gap(int argc, char *argv[])
{
	atcmd_bt_excute(argc, argv, le_gap_cmd_table, "[ATBC][le_gap]");
	return 0;
}