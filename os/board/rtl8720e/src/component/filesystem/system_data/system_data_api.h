#ifndef __SYSTEM_DATA_API_H__
#define __SYSTEM_DATA_API_H__


/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
#define SYS_DATA_SGNT			"SYSd"
#define SYS_DATA_SGNT_LEN		4
#define SYS_DATA_VERSION		"V001"
#define SYS_DATA_VERSION_ADDR		0x04
#define SYS_DATA_BT_DATA		"BT01"
#define SYS_DATA_BT_DATA_ADDR		0x10
#define SYS_DATA_WIFI_DATA_ADDR		0x20
#define WIFI_FAST_CON_PARTION_SIZE 	0x400
#define TOTAL_PARTION_SIZE 		0x1000
#define SYS_DATA_NAND_START		12
#define SYS_DATA_NAND_END		15
#define SYS_DATA_NOR_ADDR		4095

//Function
int sys_write_wlan_data_to_flash(u8 *data, uint32_t len);
int sys_write_bt_data_to_flash(u8 *data, uint32_t len);
int sys_read_wlan_data_from_flash(u8 *data, uint32_t len);
int sys_read_bt_data_from_flash(u8 *data);
int sys_erase_system_data(void);

#endif //#ifndef __SYSTEM_DATA_API__
