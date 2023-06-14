/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <osif.h>

#include <platform_opts_bt.h>
#include <log_service.h>
#include <bt_utils.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_avrcp.h>
#include <atcmd_bt_impl.h>
#include <rtk_bt_br_gap.h>

_WEAK uint16_t rtk_bt_avrcp_play(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_pause(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_stop(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_forward(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_backward(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_rewind_start(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_rewind_stop(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_fast_forward_start(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_fast_forward_stop(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_absolute_volume_set(uint8_t *bd_addr, uint8_t volume)
{
	(void)bd_addr;
	(void)volume;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_avrcp_volume_change_req(uint8_t *bd_addr, uint8_t volume)
{
	(void)bd_addr;
	(void)volume;
	AT_PRINTK("[ATBE] avrcp not support connect");
	return -1;
}

static int atcmd_bt_avrcp_play(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP play op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_play(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP play fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP play %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_pause(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP pause op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_pause(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP pause fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP pause %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_stop(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP stop op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_stop(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP stop fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP stop %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_forward(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP forward op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_forward(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP forward fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP forward %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_backward(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP backward op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_backward(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP backward fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP backward %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_rewind_start(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP rewind start op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_rewind_start(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP rewind start fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP rewind start %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_rewind_stop(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP rewind stop op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_rewind_stop(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP rewind stop fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP rewind stop %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_fast_forward_start(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP fast forward start op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_fast_forward_start(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP fast forward start fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP fast forward start %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_fast_forward_stop(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] AVRCP fast forward stop op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_avrcp_fast_forward_stop(bd_addr)) {
		AT_PRINTK("[ATBC] AVRCP fast forward stop fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP fast forward stop %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_volume_set(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};
	uint8_t volume = 0;

	if (argc != 2) {
		AT_PRINTK("[ATBC] AVRCP volume set op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	volume = (uint8_t)atoi(argv[1]);
	if (rtk_bt_avrcp_absolute_volume_set(bd_addr, volume)) {
		AT_PRINTK("[ATBC] AVRCP volume set fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP volume set %s ...", addr_str);

	return 0;
}

static int atcmd_bt_avrcp_volume_change_req(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};
	uint8_t volume = 0;

	if (argc != 2) {
		AT_PRINTK("[ATBC] AVRCP volume set op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	volume = (uint8_t)atoi(argv[1]);
	if (rtk_bt_avrcp_volume_change_req(bd_addr, volume)) {
		AT_PRINTK("[ATBC] AVRCP volume set fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] AVRCP volume set %s ...", addr_str);

	return 0;
}

static const cmd_table_t avrcp_cmd_table[] = {
	{"play",                 atcmd_bt_avrcp_play,                  1, 2},
	{"pause",                atcmd_bt_avrcp_pause,                 1, 2},
	{"stop",                 atcmd_bt_avrcp_stop,                  1, 2},
	{"forward",              atcmd_bt_avrcp_forward,               1, 2},
	{"backward",             atcmd_bt_avrcp_backward,              1, 2},
	{"rewind_start",         atcmd_bt_avrcp_rewind_start,          1, 2},
	{"rewind_stop",          atcmd_bt_avrcp_rewind_stop,           1, 2},
	{"fast_forward_start",   atcmd_bt_avrcp_fast_forward_start,    1, 2},
	{"fast_forward_stop",    atcmd_bt_avrcp_fast_forward_stop,     1, 2},
	{"volume_set",           atcmd_bt_avrcp_volume_set,            1, 3},
	{"volume_change_req",    atcmd_bt_avrcp_volume_change_req,     1, 3},
	{NULL,},
};

int atcmd_bt_avrcp_cmd(int argc, char* argv[])
{
	atcmd_bt_excute(argc, argv, avrcp_cmd_table, "[ATBC][avrcp]");
	return 0;
}