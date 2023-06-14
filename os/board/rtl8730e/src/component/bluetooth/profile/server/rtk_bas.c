/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <osif.h>

#include <bt_api_config.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <rtk_service_config.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_bas.h>
#include <rtk_bt_def.h>

#define BATTERY_UUID_SRV                    	0x180F
#define BATTERY_UUID_CHAR_VAL_LEVEL             0x2A19

#define RTK_BT_UUID_BATTERY                     BT_UUID_DECLARE_16(BATTERY_UUID_SRV)
#define RTK_BT_UUID_BATTERY_VAL_LEVEL           BT_UUID_DECLARE_16(BATTERY_UUID_CHAR_VAL_LEVEL)

#define BAS_READ_INDEX                      2
#define BAS_NOTIFY_VAL_INDEX                2
#define BAS_NOTIFY_CCCD_INDEX               3

static uint8_t battery_level = 0;
static uint8_t battery_cccd_ntf_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};

static rtk_bt_gatt_attr_t battery_attrs[] = {
	/* 0 Primary Service: Battery service */
	RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_BATTERY),

	/* 1, 2 Characteristic: Battery Level */
	RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_BATTERY_VAL_LEVEL,
							   RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_NOTIFY,
							   RTK_BT_GATT_PERM_READ),
	RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),
};

struct rtk_bt_gatt_service battery_srv = RTK_BT_GATT_SERVICE(battery_attrs,BATTERY_SRV_ID);

uint16_t bas_set_params(T_BAS_PARAM_TYPE param_type, uint8_t len, uint8_t *p_value)
{
	uint16_t ret = RTK_BT_OK;

	switch (param_type) {
	case BAS_PARAM_BATTERY_LEVEL:
		if (sizeof(uint8_t) != len) {
			return RTK_BT_FAIL;
		} else {
			battery_level = p_value[0];
		}
		break;

	default:
		ret = RTK_BT_FAIL;
		break;
	}

	return ret;
}


void battery_service_callback(uint8_t event, void *data)
{
	uint32_t i;
	uint16_t ret = 0;

	switch (event)
	{
		case RTK_BT_GATTS_EVT_REGISTER_SERVICE:
		{
			rtk_bt_gatts_reg_ind_t* reg_srv_res = (rtk_bt_gatts_reg_ind_t*)data;
			if (RTK_BT_OK == reg_srv_res->reg_status)
				printf("[APP] BAS register service succeed!\r\n");
			else
				printf("[APP] BAS register service failed, err: 0x%x\r\n", reg_srv_res->reg_status);
			break;
		}
		
		case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND:
		{
			rtk_bt_gatts_ntf_and_ind_ind_t* p_ind_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
			if (RTK_BT_OK == p_ind_ind->err_code)
				printf("[APP] BAS indicate succeed!\r\n");
			else
				printf("[APP] BAS indicate failed, err: 0x%x \r\n", p_ind_ind->err_code);
			break;
		}

		case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND:
		{
			rtk_bt_gatts_ntf_and_ind_ind_t* p_ntf_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
			if (RTK_BT_OK == p_ntf_ind->err_code)
				printf("[APP] BAS notify succeed!\r\n");
			else
				printf("[APP] BAS notify failed, err: 0x%x\r\n", p_ntf_ind->err_code);
			break;
		}
		
		case RTK_BT_GATTS_EVT_READ_IND:
		{
			rtk_bt_gatts_read_ind_t* p_read_ind = (rtk_bt_gatts_read_ind_t*)data;
			rtk_bt_gatts_read_resp_param_t read_resp = {0};
			if (BAS_READ_INDEX == p_read_ind->index)
			{
				read_resp.app_id = p_read_ind->app_id;
				read_resp.conn_handle = p_read_ind->conn_handle;
				read_resp.cid = p_read_ind->cid;
				read_resp.index = p_read_ind->index;
				read_resp.data = &battery_level;
				read_resp.len = 1;
			} 
			else 
			{
				printf("[APP] BAS read event unknown index: %d\r\n", p_read_ind->index);
				break;
			}

			for (i = 0; i < 3; i++)
			{
				ret = rtk_bt_gatts_read_resp(&read_resp);
				if (RTK_BT_OK == ret)
				{
					printf("[APP] BAS response for client read succeed!\r\n");
					break;
				}
				osif_delay(50);
			}
			if (i >= 3)
				printf("[APP] BAS response for client read failed, err: 0x%x\r\n", ret);
			break;
		}
		
		case RTK_BT_GATTS_EVT_WRITE_IND:
		{
			rtk_bt_gatts_write_ind_t* p_write_ind = (rtk_bt_gatts_write_ind_t*)data;
			printf("[APP] BAS write event, while no write attr, so no response, index: %d\r\n", p_write_ind->index);
			break;
		}
		case RTK_BT_GATTS_EVT_CCCD_IND:
		{
			rtk_bt_gatts_cccd_ind_t* p_cccd_ind = (rtk_bt_gatts_cccd_ind_t *)data;
			uint8_t conn_id;

			if (rtk_bt_le_gap_get_conn_id(p_cccd_ind->conn_handle, &conn_id) != RTK_BT_OK)
				break;

			switch (p_cccd_ind->index)
			{
			case BAS_NOTIFY_CCCD_INDEX:
				/* ONLY notify bit can be set! */
				if (p_cccd_ind->value & RTK_BT_GATT_CCC_INDICATE)
					printf("[APP] BAS notify cccd set in error, value: 0x%04x\r\n", p_cccd_ind->value);

				if (p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY)
				{
					battery_cccd_ntf_en_map[conn_id] = 1;
					printf("[APP] BAS notify cccd, notify bit enable\r\n");
				}
				else
				{
					battery_cccd_ntf_en_map[conn_id] = 0;
					printf("[APP] BAS notify cccd, notify bit disable\r\n");
				}
				break;
			default:
				printf("[APP] BAS CCCD event unknown index: %d\r\n", p_cccd_ind->index);
				break;
			}
			break;
		}
		default:
			break;
	}
}

void battery_cccd_notify(uint16_t conn_handle)
{
	rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	if (battery_cccd_ntf_en_map[conn_id]) {
		(100 == battery_level) ? (100) : (battery_level++);

		ntf_param.app_id = BATTERY_SRV_ID;
		ntf_param.conn_handle = conn_handle;
		// ntf_param.cid = 4;	// choose dynamic created L2CAP channel when EATT is enbaled.
		ntf_param.index = BAS_NOTIFY_VAL_INDEX;
		ntf_param.data = &battery_level;
		ntf_param.len = sizeof(battery_level);
		ntf_param.seq = 0;
		rtk_bt_gatts_notify(&ntf_param);
	}
}

uint16_t battery_srv_add(void)
{
	battery_srv.type = GATT_SERVICE_OVER_BLE;
	battery_srv.server_info = 0;
	battery_srv.user_data = NULL;
	battery_srv.register_status = 0;

	return rtk_bt_gatts_register_service(&battery_srv);
}

void bas_disconnect(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	battery_cccd_ntf_en_map[conn_id] = 0;
}

void bas_status_deinit(void)
{
	battery_level = 0;
	memset(battery_cccd_ntf_en_map, 0, sizeof(battery_cccd_ntf_en_map));
}
