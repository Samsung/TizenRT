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
#include <rtk_bt_spp.h>
#include <atcmd_bt_impl.h>
#include <rtk_bt_br_gap.h>

_WEAK uint16_t rtk_bt_spp_connect(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] SPP not support connect");
	return -1;
}

_WEAK uint16_t rtk_bt_spp_disconnect(rtk_bt_spp_disconn_req_t *p_disconn_req_t)
{
	(void)p_disconn_req_t;
	AT_PRINTK("[ATBE] SPP not support disconnect");
	return -1;
}

_WEAK uint16_t rtk_bt_spp_disconnect_all(uint8_t *bd_addr)
{
	(void)bd_addr;
	AT_PRINTK("[ATBE] SPP not support disconnect all");
	return -1;
}

_WEAK uint16_t rtk_bt_spp_send_data(rtk_bt_spp_send_data_t *send_data_t)
{
	(void)send_data_t;
	AT_PRINTK("[ATBE] SPP not support send data");
	return -1;
}

_WEAK uint16_t rtk_bt_spp_give_credits(rtk_bt_spp_credits_give_t *p_credits_give_t)
{
	(void)p_credits_give_t;
	AT_PRINTK("[ATBE] SPP not support give credits");
	return -1;
}

static int atcmd_bt_spp_connect(int argc, char **argv)
{
	// ATBC=spp_cmd,conn,bd_addr
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] SPP connect op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_spp_connect(bd_addr)) {
		AT_PRINTK("[ATBC] SPP connect fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] SPP connecting to device %s ...", addr_str);

	return 0;
}

static int atcmd_bt_spp_disconnect(int argc, char ** argv)
{
	// ATBC=spp_cmd,disconn,bd_addr,server_chann
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};
	uint8_t server_chann = 0;
	char addr_str[30] = {0};

	if (argc != 2) {
		AT_PRINTK("[ATBC] SPP send data op failed! wrong args num!");
		return -1;
	}

	if (hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN) == false) {
		AT_PRINTK("[ATBC] SPP send data op failed! wrong bd addr!");
		return -1;
	}

	server_chann = (uint8_t)(str_to_int(argv[1]));

	rtk_bt_spp_disconn_req_t p_disconn_req_t = {0};

	memcpy((void *)p_disconn_req_t.bd_addr, bd_addr, 6);
	p_disconn_req_t.local_server_chann = server_chann;

	if (rtk_bt_spp_disconnect(&p_disconn_req_t)) {
		AT_PRINTK("[ATBC] SPP disconnect channel 0x%xfail \r\n", p_disconn_req_t.local_server_chann);
		return -1;
	}

	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] SPP send disconnect request to device %s ,server channel 0x%x \r\n", addr_str, server_chann);

	return 0;
}

static int atcmd_bt_spp_disconnect_all(int argc, char **argv)
{
	// ATBC=spp_cmd,disconn_all,bd_addr
	char addr_str[30] = {0};
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};

	if (argc != 1) {
		AT_PRINTK("[ATBC] SPP disconnect op failed! wrong args num!");
		return -1;
	}
	hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN);
	if (rtk_bt_spp_disconnect_all(bd_addr)) {
		AT_PRINTK("[ATBC] SPP disconnect fail \r\n");
		return -1;
	}
	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] SPP disconnecting to device %s ...", addr_str);

	return 0;
}

static int atcmd_bt_spp_send_data(int argc, char ** argv)
{
	// ATBC=spp_cmd,send_data,bd_addr,server_chann,xxxxxxx
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};
	uint8_t server_chann = 0;
	uint8_t data_send_buf[RTK_BT_SPP_MAX_DATA_IND_LENGTH] = {0};
	char addr_str[30] = {0};

	if (argc != 3) {
		AT_PRINTK("[ATBC] SPP send data op failed! wrong args num!");
		return -1;
	}

	if (hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN) == false) {
		AT_PRINTK("[ATBC] SPP send data op failed! wrong bd addr!");
		return -1;
	}

	uint16_t data_send_len = strlen(argv[2])/2;

	if (hexdata_str_to_array(argv[2], data_send_buf, data_send_len) == false) {
		AT_PRINTK("[ATBC] SPP send data op failed! wrong hex data!");
		return -1;
	}

	server_chann = (uint8_t)(str_to_int(argv[1]));

	rtk_bt_spp_send_data_t send_data_t = {0};
	memcpy((void *)send_data_t.bd_addr, bd_addr, 6);
	send_data_t.local_server_chann = server_chann;
	send_data_t.data = data_send_buf;
	send_data_t.len = data_send_len;
	if (rtk_bt_spp_send_data(&send_data_t)) {
		AT_PRINTK("[ATBC] SPP send data fail \r\n");
		return -1;
	}

	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] SPP send data to device %s ,server channel 0x%x success!", addr_str, server_chann);

	return 0;
}

static int atcmd_bt_spp_give_credits(int argc, char ** argv)
{
	// ATBC=spp_cmd,give_credits,bd_addr,server_chann,xx
	uint8_t bd_addr[RTK_BD_ADDR_LEN] = {0};
	uint8_t server_chann = 0;
	uint8_t credits = 0;
	char addr_str[30] = {0};

	if (argc != 3) {
		AT_PRINTK("[ATBC] SPP give credits op failed! wrong args num!");
		return -1;
	}

	if (hexdata_str_to_bd_addr(argv[0], bd_addr, RTK_BD_ADDR_LEN) == false) {
		AT_PRINTK("[ATBC] SPP give credits op failed! wrong bd addr!");
		return -1;
	}

	server_chann = (uint8_t)(str_to_int(argv[1]));
	credits = (uint8_t)(str_to_int(argv[2]));

	rtk_bt_spp_credits_give_t give_credits_t = {0};
	memcpy((void *)give_credits_t.bd_addr, bd_addr, 6);
	give_credits_t.local_server_chann = server_chann;
	give_credits_t.credits = credits;

	if (rtk_bt_spp_credits_give(&give_credits_t)) {
		AT_PRINTK("[ATBC] SPP send data fail \r\n");
		return -1;
	}

	rtk_bt_br_addr_to_str(bd_addr, addr_str, sizeof(addr_str));
	AT_PRINTK("[ATBC] SPP give %d credits to device %s ,server channel 0x%x success!", credits, addr_str, server_chann);

	return 0;
}

static const cmd_table_t spp_cmd_table[] = {
	{"conn",        atcmd_bt_spp_connect,            1, 2},
	{"disconn",     atcmd_bt_spp_disconnect,         3, 3},
	{"disconn_all", atcmd_bt_spp_disconnect_all,     1, 2},
	{"send_data",   atcmd_bt_spp_send_data,          4, 4},
	{"give_credits",atcmd_bt_spp_give_credits,       4, 4},
	{NULL,},
};

int atcmd_bt_spp_cmd(int argc, char* argv[])
{
	atcmd_bt_excute(argc, argv, spp_cmd_table, "[ATBC][spp]");
	return 0;
}