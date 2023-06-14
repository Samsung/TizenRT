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
#include <rtk_bt_br_gap.h>
#include <bt_utils.h>
#include <atcmd_bt_impl.h>
#include <rtk_bt_common.h>

_WEAK uint16_t rtk_bt_br_gap_start_inquiry(rtk_bt_br_inquiry_start_t *inquiry_start_param)
{
	(void)inquiry_start_param;
	AT_PRINTK("[ATBC] BR GAP not support inquiry");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_disconnect(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] BR GAP not support disconnect");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_cfg_page_param(uint8_t type, uint16_t interval, uint16_t window)
{
	(void)type;
	(void)interval;
	(void)window;
	AT_PRINTK("[ATBE] BR GAP not support cfg page param");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_cfg_inquiry_param(uint8_t type, uint16_t interval, uint16_t window)
{
	(void)type;
	(void)interval;
	(void)window;
	AT_PRINTK("[ATBE] BR GAP not support cfg inquiry param");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_bond_clear(void)
{
	AT_PRINTK("[ATBC] BR GAP not support bond clear");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_set_radio_mode(uint8_t radio_mode)
{
	(void)radio_mode;
	AT_PRINTK("[ATBC] BR GAP not support radio mode");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_max_bond_num_get(uint8_t *pmax_bond_num)
{
	(void)pmax_bond_num;
	AT_PRINTK("[ATBC] BR GAP not support max bond num get");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_bond_num_get(uint8_t *pbond_num)
{
	(void)pbond_num;
	AT_PRINTK("[ATBC] BR GAP not support bond num get");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_bond_addr_get(uint8_t priority, uint8_t *bd_addr)
{
	(void)priority;
	(void)bd_addr;
	AT_PRINTK("[ATBC] BR GAP not support bond addr get");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_bond_index_get(uint8_t *pindex, uint8_t *bd_addr)
{
	(void)pindex;
	(void)bd_addr;
	AT_PRINTK("[ATBC] BR GAP not support bond index get");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_bond_key_get(uint8_t *bd_addr, uint8_t *key_type, uint8_t *bond_key)
{
	(void)bd_addr;
	(void)key_type;
	(void)bond_key;
	AT_PRINTK("[ATBC] BR GAP not support bond key get");
	return -1;
}

_WEAK uint16_t rtk_bt_br_gap_bond_delete(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBC] BR GAP not support bond key delete");
	return -1;
}

static int atcmd_br_gap_inquiry_start(int argc, char **argv)
{
	uint16_t ret = 1;
	rtk_bt_br_inquiry_start_t inquiry_start_param = {0};

	if (argc != 2) {
		AT_PRINTK("[ATBC] BR GAP inquiry start op failed! wrong args num!");
		return -1;
	}

	inquiry_start_param.limited_inquiry = (bool)atoi(argv[0]);
	inquiry_start_param.timeout = (uint8_t)atoi(argv[1]);

	ret = rtk_bt_br_gap_start_inquiry(&inquiry_start_param);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP start inquiry failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP starting inquiry...");
	return 0;
}

static int atcmd_br_gap_disconnect(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] BR GAP disconnect op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_br_gap_disconnect(bd_addr)) {
		AT_PRINTK("[ATBC] BR GAP disconnect fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] BR GAP disconnecting to device %s ...", addr_str);

	return 0;
}

static int atcmd_br_gap_cfg_page_param(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t type = 0;
	uint16_t interval = 0;
	uint16_t window = 0;

	if (argc != 3) {
		AT_PRINTK("[ATBC] BR GAP cfg page param failed! wrong args num!");
		return -1;
	}

	type = (uint8_t)atoi(argv[0]);
	interval = (uint16_t)atoi(argv[1]);
	window = (uint16_t)atoi(argv[2]);
	printf("[ATBC] cfg page type 0x%02x, interval 0x%04x, window 0x%04x \r\n", type, interval, window);
	ret = rtk_bt_br_gap_cfg_page_param(type, interval, window);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP cfg page param failed! err: 0x%x", ret);
		return -1;
	}

	return 0;
}

static int atcmd_br_gap_cfg_inquiry_param(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t type = 0;
	uint16_t interval = 0;
	uint16_t window = 0;

	if (argc != 3) {
		AT_PRINTK("[ATBC] BR GAP cfg inquiry param failed! wrong args num!");
		return -1;
	}

	type = (uint8_t)atoi(argv[0]);
	interval = (uint16_t)atoi(argv[1]);
	window = (uint16_t)atoi(argv[2]);
	printf("[ATBC] cfg inquiry type 0x%02x, interval 0x%04x, window 0x%04x \r\n", type, interval, window);
	ret = rtk_bt_br_gap_cfg_inquiry_param(type, interval, window);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP cfg inquiry param failed! err: 0x%x", ret);
		return -1;
	}

	return 0;
}

static int atcmd_br_gap_bond_clear(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;

	ret = rtk_bt_br_gap_bond_clear();

	if (ret) {
		AT_PRINTK("[ATBC] BR GAP clear bond storage failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP clearing bond info...");
	return 0;
}

static int atcmd_br_gap_radio_mode(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t mode = 0;

	if (argc != 1) {
		AT_PRINTK("[ATBC] BR GAP radio mode failed! wrong args num!");
		return -1;
	}

	mode = (uint8_t)atoi(argv[0]);

	ret = rtk_bt_br_gap_set_radio_mode(mode);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP radio mode failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP seting radio mode ...");
	return 0;
}

static int atcmd_br_gap_bond_max_num(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	uint8_t bond_max_num = 0;

	ret = rtk_bt_br_gap_max_bond_num_get(&bond_max_num);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP bond max num failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] Max bond num %d ...", bond_max_num);
	return 0;
}

static int atcmd_br_gap_bond_num(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	uint16_t ret = 0;
	uint8_t bond_num = 0;

	ret = rtk_bt_br_gap_bond_num_get(&bond_num);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP bond num failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] Bond num %d ...", bond_num);
	return 0;
}

static int atcmd_br_gap_bond_addr_get(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t priority = 0;
	uint8_t bd_addr[6] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] BR GAP Bond addr get failed! wrong args num!");
		return -1;
	}

	priority = (uint8_t)atoi(argv[0]);

	ret = rtk_bt_br_gap_bond_addr_get(priority, bd_addr);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP get bond addr failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP Bond addr %x: %x: %x: %x: %x: %x ",
							bd_addr[5], bd_addr[4], bd_addr[3], bd_addr[2], bd_addr[1], bd_addr[0]);
	return 0;
}

static int atcmd_br_gap_bond_index_get(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t bd_addr[6] = {0};
	uint8_t index = 0;

	if (argc != 1) {
		AT_PRINTK("[ATBC] BR GAP Bond index get failed! wrong args num!");
		return -1;
	}

	hexdata_str_to_bd_addr(argv[0], bd_addr, 6);
	ret = rtk_bt_br_gap_bond_index_get(&index, bd_addr);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP get bond index failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP Bond index 0x%x", index);
	return 0;
}

static int atcmd_br_gap_bond_key_get(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t bd_addr[6] = {0};
	uint8_t key_type = 0;
	uint8_t key[16] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] BR GAP Bond key get failed! wrong args num!");
		return -1;
	}

	hexdata_str_to_bd_addr(argv[0], bd_addr, 6);
	ret = rtk_bt_br_gap_bond_key_get(bd_addr, &key_type, key);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP get bond key failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP Bond key: \r\n");
	for (uint8_t i = 0; i < 16; i ++) {
		printf("%02x ", key[i]);
	}
	return 0;
}

static int atcmd_br_gap_bond_delete(int argc, char **argv)
{
	uint16_t ret = 1;
	uint8_t bd_addr[6] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] BR GAP Bond delete failed! wrong args num!");
		return -1;
	}

	hexdata_str_to_bd_addr(argv[0], bd_addr, 6);
	ret = rtk_bt_br_gap_bond_delete(bd_addr);
	if (ret) {
		AT_PRINTK("[ATBC] BR GAP Bond delete failed! err: 0x%x", ret);
		return -1;
	}

	AT_PRINTK("[ATBC] BR GAP Bond delete success");

	return 0;
}

static const cmd_table_t br_gap_cmd_table[] = {
	{"inquiry_start",      atcmd_br_gap_inquiry_start,        1, 3},
	{"disc",               atcmd_br_gap_disconnect,           1, 2},
	{"cfg_page",           atcmd_br_gap_cfg_page_param,       1, 4},
	{"cfg_inquiry",        atcmd_br_gap_cfg_inquiry_param,    1, 4},
	{"bond_clear",         atcmd_br_gap_bond_clear,           1, 1},
	{"radio_mode",         atcmd_br_gap_radio_mode,           1, 2},
	{"bond_max_num",       atcmd_br_gap_bond_max_num,         1, 1},
	{"bond_num",           atcmd_br_gap_bond_num,             1, 1},
	{"bond_addr_get",      atcmd_br_gap_bond_addr_get,        1, 2},
	{"bond_index_get",     atcmd_br_gap_bond_index_get,       1, 2},
	{"bond_key_get",       atcmd_br_gap_bond_key_get,         1, 2},
	{"bond_delete",        atcmd_br_gap_bond_delete,          1, 2},
	{NULL,},
};

int atcmd_bt_br_gap(int argc, char* argv[])
{
	atcmd_bt_excute(argc, argv, br_gap_cmd_table, "[ATBC][br_gap]");
	return 0;
}