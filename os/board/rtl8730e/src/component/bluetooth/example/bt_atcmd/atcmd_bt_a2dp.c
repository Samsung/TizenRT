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
#include <rtk_bt_a2dp.h>
#include <atcmd_bt_impl.h>
#include <rtk_bt_br_gap.h>

_WEAK uint16_t rtk_bt_a2dp_connect(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] A2dp not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_a2dp_disconnect(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] A2dp not support disconnect");
	return -1;
}

_WEAK uint16_t rtk_bt_a2dp_start(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] A2dp not support start");
	return -1;
}

_WEAK uint16_t rtk_bt_a2dp_suspend(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] A2dp not support suspend");
	return -1;
}

static int atcmd_bt_a2dp_connect(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] A2DP connect op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_a2dp_connect(bd_addr)) {
		AT_PRINTK("[ATBC] A2DP connect fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] A2DP connecting to device %s ...", addr_str);

	return 0;
}

static int atcmd_bt_a2dp_disconnect(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] A2DP disconnect op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_a2dp_disconnect(bd_addr)) {
		AT_PRINTK("[ATBC] A2DP disconnect fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] A2DP disconnecting to device %s ...", addr_str);

	return 0;
}

static int atcmd_bt_a2dp_start(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] A2DP start op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_a2dp_start(bd_addr)) {
		AT_PRINTK("[ATBC] A2DP start fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] A2DP starting to device %s ...", addr_str);

	return 0;
}

static int atcmd_bt_a2dp_suspend(int argc, char **argv)
{
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] A2DP suspend op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_a2dp_suspend(bd_addr)) {
		AT_PRINTK("[ATBC] A2DP suspend fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] A2DP suspending to device %s ...", addr_str);

	return 0;
}

static const cmd_table_t a2dp_cmd_table[] = {
	{"conn",       atcmd_bt_a2dp_connect,        1, 2},
	{"disconn",    atcmd_bt_a2dp_disconnect,     1, 2},
	{"start",      atcmd_bt_a2dp_start,          1, 2},
	{"suspend",    atcmd_bt_a2dp_suspend,        1, 2},
	{NULL,},
};

int atcmd_bt_a2dp_cmd(int argc, char* argv[])
{
	atcmd_bt_excute(argc, argv, a2dp_cmd_table, "[ATBC][a2dp]");
	return 0;
}