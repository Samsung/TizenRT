/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <rtk_service_config.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_simple_ble_service.h>

#define SIMPLE_BLE_UUID_SRV                 0xA00A
#define SIMPLE_BLE_UUID_CHAR_VAL_READ       0xB001
#define SIMPLE_BLE_UUID_CHAR_VAL_WRITE      0xB002
#define SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY     0xB003
#define SIMPLE_BLE_UUID_CHAR_VAL_INDICATE   0xB004

#define RTK_BT_UUID_SIMPLE_BLE              BT_UUID_DECLARE_16(SIMPLE_BLE_UUID_SRV)
#define RTK_BT_UUID_SIMPLE_BLE_VAL_READ     BT_UUID_DECLARE_16(SIMPLE_BLE_UUID_CHAR_VAL_READ)
#define RTK_BT_UUID_SIMPLE_BLE_VAL_WRITE    BT_UUID_DECLARE_16(SIMPLE_BLE_UUID_CHAR_VAL_WRITE)
#define RTK_BT_UUID_SIMPLE_BLE_VAL_NOTIFY   BT_UUID_DECLARE_16(SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY)
#define RTK_BT_UUID_SIMPLE_BLE_VAL_INDICATE BT_UUID_DECLARE_16(SIMPLE_BLE_UUID_CHAR_VAL_INDICATE)

#define SIMPLE_BLE_READ_MAX_LEN             60
#define SIMPLE_BLE_READ_INDEX               2
#define SIMPLE_BLE_WRITE_INDEX              5
#define SIMPLE_BLE_NOTIFY_VAL_INDEX         7
#define SIMPLE_BLE_NOTIFY_CCCD_INDEX        8
#define SIMPLE_BLE_INDICATE_VAL_INDEX       10
#define SIMPLE_BLE_INDICATE_CCCD_INDEX      11

static uint8_t simple_ble_read_val[SIMPLE_BLE_READ_MAX_LEN] = {'D', 'e', 'm', 'o', '\0'};
// static uint16_t simple_ble_read_len           = 5;
static uint8_t simple_ble_read_cud[]          = "Simple BLE Read User Descriptor";
static uint8_t simple_ble_cccd_ntf_val[]      = "Simple BLE notification data";
static uint8_t simple_ble_cccd_ntf_en_map[10] = {0};
static uint8_t simple_ble_cccd_ind_val[]      = "Simple BLE indication data";
static uint8_t simple_ble_cccd_ind_en_map[10] = {0};

static rtk_bt_gatt_attr_t simple_ble_attrs[] = {
	/* Primary Service: simple BLE */
	RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_SIMPLE_BLE),

	/* Characteristic: demo for read */
	RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_SIMPLE_BLE_VAL_READ,
							   RTK_BT_GATT_CHRC_READ,
							   RTK_BT_GATT_PERM_READ),
	RTK_BT_GATT_CUD(simple_ble_read_cud, RTK_BT_GATT_PERM_READ),

	/* Characteristic: demo for write */
	RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_SIMPLE_BLE_VAL_WRITE,
							   RTK_BT_GATT_CHRC_WRITE | RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP,
							   RTK_BT_GATT_PERM_WRITE),

	/* Characteristic: demo for notify */
	RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_SIMPLE_BLE_VAL_NOTIFY,
							   RTK_BT_GATT_CHRC_NOTIFY,
							   RTK_BT_GATT_PERM_NONE),
	RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),

	/* Characteristic: demo for indicate */
	RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_SIMPLE_BLE_VAL_INDICATE,
							   RTK_BT_GATT_CHRC_INDICATE,
							   RTK_BT_GATT_PERM_NONE),
	RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),
};

static struct rtk_bt_gatt_service simple_ble_srv = RTK_BT_GATT_SERVICE(simple_ble_attrs,SIMPLE_BLE_SRV_ID);

void simple_ble_service_callback(uint8_t event, void *data)
{
	uint32_t i = 0;
	uint16_t ret = 0;

	// printf("simple_ble_service_callback: event = %d \r\n",event);
	switch(event){
	case RTK_BT_GATTS_EVT_REGISTER_SERVICE:
		{
			rtk_bt_gatts_reg_ind_t *p_gatts_reg_ind = (rtk_bt_gatts_reg_ind_t *)data;
			if(p_gatts_reg_ind->reg_status == RTK_BT_OK)
				printf("[APP] simple ble service register succeed!\r\n");
			else
				printf("[APP] simple ble service register failed, err: 0x%x\r\n", 
														p_gatts_reg_ind->reg_status);
			break;
		}
	case RTK_BT_GATTS_EVT_READ_IND:
		{
			rtk_bt_gatts_read_ind_t *p_read_ind = (rtk_bt_gatts_read_ind_t *)data;
			uint16_t offset = p_read_ind->offset;
			uint16_t actual_len = SIMPLE_BLE_READ_MAX_LEN - p_read_ind->offset;
			if (SIMPLE_BLE_READ_INDEX == p_read_ind->index) 
			{
				rtk_bt_gatts_read_resp_param_t read_resp = {
					.app_id   = p_read_ind->app_id,
					.conn_handle  = p_read_ind->conn_handle,
					.cid      = p_read_ind->cid,
					.index    = p_read_ind->index,
					.data     = &simple_ble_read_val[offset],
					.len      = actual_len,
					.err_code = 0,
					.seq      = 0,
				};
				for (i = 0; i < 3; i++)
				{
					ret = rtk_bt_gatts_read_resp(&read_resp);
					if (RTK_BT_OK == ret)
					{
						printf("[APP] Simple BLE respond for client read success, offset: %d\r\n", offset);
						break;
					}
					osif_delay(50);
				}
				if (i >= 3)
					printf("[APP] Simple BLE respond for client read failed, err: 0x%x\r\n", ret);
			} 
			else 
			{
				printf("[APP] Simple BLE read event unknown index: %d\r\n", p_read_ind->index);
			}
			
			break;
		}
	case RTK_BT_GATTS_EVT_WRITE_IND:
		{
			rtk_bt_gatts_write_ind_t *p_write_ind = (rtk_bt_gatts_write_ind_t *)data;
			rtk_bt_gatts_write_resp_param_t write_resp = {0};
			if (SIMPLE_BLE_WRITE_INDEX == p_write_ind->index)
			{
				printf("[APP] Simple BLE write event, len: %d, type: %d, data: ",
											p_write_ind->len, p_write_ind->type);
				for (i = 0; i < p_write_ind->len; i++)
				{
					if (0 == i%16)
						printf("\n\r");
					printf("%02x ", *(p_write_ind->value+i));
				}
				printf("\r\n");

				write_resp.app_id = p_write_ind->app_id;
				write_resp.conn_handle = p_write_ind->conn_handle;
				write_resp.cid = p_write_ind->cid,
				write_resp.index = p_write_ind->index;
				write_resp.type = p_write_ind->type;
				write_resp.err_code = 0;
				write_resp.seq = 0;
				
				for (i = 0; i < 3; i++)
				{
					ret = rtk_bt_gatts_write_resp(&write_resp);
					if (RTK_BT_OK == ret)
					{
						printf("[APP] Simple BLE response for client write success!\r\n");
						break;
					}
					osif_delay(10);
				}
				if (i >= 3)
					printf("[APP] Simple BLE response for client write failed, err: 0x%x\r\n", ret);
			}
			else
				printf("[APP] Simple BLE write event unknown index: %d\r\n", p_write_ind->index);
			break;
		}
	case RTK_BT_GATTS_EVT_CCCD_IND:
		{
			rtk_bt_gatts_cccd_ind_t *p_cccd_ind = (rtk_bt_gatts_cccd_ind_t *)data;
			uint8_t conn_id;

			if (rtk_bt_le_gap_get_conn_id(p_cccd_ind->conn_handle, &conn_id) != RTK_BT_OK)
				break;

			switch(p_cccd_ind->index) {
			case SIMPLE_BLE_NOTIFY_CCCD_INDEX:
				if (p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY)
				{
					simple_ble_cccd_ntf_en_map[conn_id] = 1;
					printf("[APP] Simple BLE notify cccd, notify bit enable\r\n");
				}
				else
				{
					simple_ble_cccd_ntf_en_map[conn_id] = 0;
					printf("[APP] Simple BLE notify cccd, notify bit disable\r\n");
				}
				break;
			case SIMPLE_BLE_INDICATE_CCCD_INDEX:
				if (p_cccd_ind->value & RTK_BT_GATT_CCC_INDICATE)
				{
					simple_ble_cccd_ind_en_map[conn_id] = 1;
					printf("[APP] Simple BLE indicate cccd, indicate bit enable\r\n");
				}
				else
				{
					simple_ble_cccd_ind_en_map[conn_id] = 0;
					printf("[APP] Simple BLE indicate cccd, indicate bit disable\r\n");
				}
				break;
			default:
				break;
			}
			break;
		}
	
	case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND:
	{
		rtk_bt_gatts_ntf_and_ind_ind_t *p_ntf_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
		if (RTK_BT_OK == p_ntf_ind->err_code)
			printf("[APP] Simple BLE notify succeed!\r\n");
		else
			printf("[APP] Simple BLE notify failed, err: 0x%x\r\n", p_ntf_ind->err_code);
		break;
	}

	case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND:
	{
		rtk_bt_gatts_ntf_and_ind_ind_t *p_ind_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
		if (RTK_BT_OK == p_ind_ind->err_code)
			printf("[APP] Simple BLE indicate succeed!\r\n");
		else
			printf("[APP] Simple BLE indicate failed, err: 0x%x\r\n", p_ind_ind->err_code);
		break;
	}
	default:
		break;
	}
}

void simple_ble_srv_cccd_notify(uint16_t conn_handle)
{
	rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	if (simple_ble_cccd_ntf_en_map[conn_id]) {
		ntf_param.app_id = SIMPLE_BLE_SRV_ID;
		ntf_param.conn_handle = conn_handle;
		// ntf_param.cid = 4;	// choose dynamic created L2CAP channel when EATT is enbaled.
		ntf_param.index = SIMPLE_BLE_NOTIFY_VAL_INDEX;
		ntf_param.data = simple_ble_cccd_ntf_val;
		ntf_param.len = sizeof(simple_ble_cccd_ntf_val)/sizeof(uint8_t);
		ntf_param.seq = 0;
		rtk_bt_gatts_notify(&ntf_param);
	}
}

void simple_ble_srv_cccd_indicate(uint16_t conn_handle)
{
	rtk_bt_gatts_ntf_and_ind_param_t ind_param = {0};
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	if (simple_ble_cccd_ind_en_map[conn_id]) {
		ind_param.app_id = SIMPLE_BLE_SRV_ID;
		ind_param.conn_handle = conn_handle;
		// ind_param.cid = 4;	// choose dynamic created L2CAP channel when EATT is enbaled.
		ind_param.index = SIMPLE_BLE_INDICATE_VAL_INDEX;
		ind_param.data = simple_ble_cccd_ind_val;
		ind_param.len = sizeof(simple_ble_cccd_ind_val)/sizeof(uint8_t);
		ind_param.seq = 0;
		rtk_bt_gatts_indicate(&ind_param);
	}
}

uint16_t simple_ble_srv_add(void)
{
	simple_ble_srv.type = GATT_SERVICE_OVER_BLE;
	simple_ble_srv.server_info = 0;
	simple_ble_srv.user_data = NULL;
	simple_ble_srv.register_status = 0;

	return rtk_bt_gatts_register_service(&simple_ble_srv);
}

void simple_ble_srv_disconnect(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	simple_ble_cccd_ntf_en_map[conn_id] = 0;
	simple_ble_cccd_ind_en_map[conn_id] = 0;
}

void simple_ble_srv_status_deinit(void)
{
	memset(simple_ble_cccd_ind_en_map, 0, sizeof(simple_ble_cccd_ind_en_map));
	memset(simple_ble_cccd_ntf_en_map, 0, sizeof(simple_ble_cccd_ntf_en_map));
}

