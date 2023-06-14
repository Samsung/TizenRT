/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdlib.h>
#include <string.h>
#include <log_service.h>
#include <bt_utils.h>
#include <atcmd_bt_vendor.h>
#include <rtk_bt_vendor.h>
#include <rtk_bt_le_gap.h>
#include <osif.h>
#include <bt_api_config.h>

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

/**
 * @brief  Read ASCII string and convert to uint32_t.
 *
 * @param p
 * @return
*/
static uint32_t cmd_string2uint32(char *p)
{
	uint32_t result = 0;
	char     ch;
	bool     hex = false;

	/* check if value is dec */
	if (p[0] == 'x' || p[0] == 'X') {
		hex = true;
		p = &p[1];
	} else if ((p[0] == '0') && (p[1] == 'x' || p[1] == 'X')) {
		hex = true;
		p = &p[2];
	}

	for (;;) {
		ch = *(p++) | 0x20;                 /* convert to lower case */

		if (hex) {                          /* dec value */
			/* hex value */
			if ((ch >= 'a') && (ch <= 'f')) {
				ch -= ('a' - 10);
			} else if ((ch >= '0') && (ch <= '9')) {
				ch -= '0';
			} else {
				break;
			}
			result = (result << 4);
			result += (ch & 0x0f);
		} else {
			if (ch < '0' || ch > '9') {
				break;    /* end of string reached */
			}
			result = 10 * result + ch - '0';
		}
	}
	return (result);
}

int atcmd_bt_tx_power_gain(int argc, char* argv[])
{
	uint32_t index = 0;
	uint8_t index_le, index_br, index_edr2m, index_edr3m;

	if(argc != 1 && argc != 4) {
		AT_PRINTK("[ATBV] rtk bt set tx power gain fail, wrong parameter number!");
		return 0;
	}

	index_le = (uint8_t)cmd_string2uint32(argv[0]); //le index
	index |= index_le;
	
	if(argc == 4) { // for ameba smart
		index_br = (uint8_t)cmd_string2uint32(argv[1]); //br index
		index_edr2m = (uint8_t)cmd_string2uint32(argv[2]); //edr 2m index
		index_edr3m = (uint8_t)cmd_string2uint32(argv[3]); //edr 3m index

		index |= (index_br << 8);
		index |= (index_edr2m << 16);
		index |= (index_edr3m << 24);
	}

	rtk_bt_set_bt_tx_power_gain_index(index);

	AT_PRINTK("[ATBV] rtk bt set tx power gain OK! 0x%lx", index);
	return 0;
}

_WEAK void bt_hci_debug_enable(void)
{
	printf("HCI debug is not supported on this platform");
}

int atcmd_bt_hci_debug_enable(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	rtk_bt_hci_debug_enable();

	AT_PRINTK("[ATBV]hci debug enable OK!");
	return 0;
}

int atcmd_bt_sleep_mode(int argc, char *argv[])
{
	(void)argc;
	uint8_t data[4] = {0};
	rtk_bt_le_gap_vendor_cmd_param_t param;
	unsigned int mode = (unsigned int)hexnum_str_to_int(argv[0]);
	
	AT_PRINTK("[ATBV]set fw sleep mode to 0x%x.", mode);

	data[0] = mode & 0xFF;			//0:lps, 1:dlps, 2:active
	data[1] = (mode >> 8) & 0xFF;	//0:log off, 1:log on
	data[2] = (mode >> 16) & 0xFF;	//minimum adv slot[LSB] to enter sleep
	data[3] = (mode >> 24) & 0xFF;	//minimum adv slot[MSB] to enter sleep

	param.op = 0xFC74;	//HCI_VENDOR_SLEEP_MODE_CMD
	param.len = 4;
	param.cmd_param = data;

	rtk_bt_le_gap_vendor_cmd_req(&param);

	return 0;
}

int atcmd_bt_ant(int argc, char *argv[])
{
	(void)argc;

	uint8_t ant = atoi(argv[0]);

	if (ant > 1) {
		AT_PRINTK("[ATBV]invalid param!");
		return 0;
	}

	AT_PRINTK("[ATBV]set default bt ant to %s.", ant ? "ANT_S1" : "ANT_S0");
	rtk_bt_set_bt_antenna(ant);

	return 0;
}

int atcmd_bt_read_reg(int argc, char *argv[])
{
	(void)argc;
	uint8_t data[5] = {0};
	rtk_bt_le_gap_vendor_cmd_param_t param;
	unsigned int address = (unsigned int)hexnum_str_to_int(argv[0]);
	
	AT_PRINTK("[ATBV]bt read reg address 0x%x.", address);

	data[0] = 0x10;
	memcpy((void *)&data[1], (void *)&address, 4);

	param.op = 0xFC61;	//read reg of bt controller
	param.len = 5;
	param.cmd_param = data;

	rtk_bt_le_gap_vendor_cmd_req(&param);

	return 0;
}

#if defined(RTK_BLE_TX_SOF_EOF_INDICATION) && RTK_BLE_TX_SOF_EOF_INDICATION
void rtk_bt_le_tx_sof_eof_callback(uint8_t flag)
{
	if(flag == RTK_BT_LE_TX_SOF)
		printf("%s SOF,time=%d\r\n",__func__, (int)osif_sys_time_get());
	else if(flag == RTK_BT_LE_TX_EOF)
		printf("%s EOF,time=%d\r\n",__func__, (int)osif_sys_time_get());
	else 
		printf("%s ERROR\r\n",__func__);
}

int atcmd_bt_sof_eof_ind(int argc, char *argv[])
{
	uint16_t conn_handle = 0;
	uint8_t enable = 0;

	if(argc != 2) {
		AT_PRINTK("[ATBV] atcmd_bt_sof_eof_ind fail, wrong parameter number!");
		return 0;
	}

	conn_handle = (uint16_t)str_to_int(argv[0]);
	enable = (uint8_t)str_to_int(argv[1]);
	
	AT_PRINTK("[ATBV]set conn_handle(0x%x) sof and eof to %x.",conn_handle, enable);

	rtk_bt_le_sof_eof_ind(conn_handle, enable, rtk_bt_le_tx_sof_eof_callback);

	return 0;
}
#else
_WEAK int atcmd_bt_sof_eof_ind(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("BT SOF and EOF indication is not supported on this platform");
	return 0;
}
#endif