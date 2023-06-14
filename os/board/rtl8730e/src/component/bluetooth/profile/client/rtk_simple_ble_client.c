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
#include <rtk_bt_gattc.h>
#include <rtk_client_config.h>
#include <rtk_gcs_client.h>
#include <rtk_simple_ble_client.h>

#define SIMPLE_BLE_UUID_SRV                 0xA00A
#define SIMPLE_BLE_UUID_CHAR_VAL_READ       0xB001
#define SIMPLE_BLE_UUID_CHAR_VAL_WRITE      0xB002
#define SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY     0xB003
#define SIMPLE_BLE_UUID_CHAR_VAL_INDICATE   0xB004

#if RTK_BLE_MGR_LIB
rtk_bt_gattc_uuid_t simple_ble_uuid = {
	.is_uuid16 = true,
	.p.uuid16 = SIMPLE_BLE_UUID_SRV
};
#endif

typedef struct {
	uint8_t properties;
	uint16_t char_val_handle;
	void *char_data;
	uint16_t data_len;
	uint16_t cccd_handle;
} simple_ble_charac_db_t;

typedef struct {
	client_disc_state_t disc_state;
	uint16_t start_handle;
	uint16_t end_handle;
	simple_ble_charac_db_t char_db[SIMPLE_BLE_CHAR_NUM];
} simple_ble_client_db_t;

static simple_ble_client_db_t *simple_ble_database[RTK_BLE_GAP_MAX_LINKS] = {0};

#if RTK_BLE_MGR_LIB
static uint16_t simple_ble_client_char_find(uint16_t conn_handle)
{
	uint16_t char_handle;
	rtk_bt_gattc_find_param_t find_param = {0};
	rtk_bt_gattc_uuid_t char_uuid = {
		.is_uuid16 = true,
	};
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	find_param.conn_handle = conn_handle;
	find_param.type = RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE;
	find_param.find_char.srv_uuid = simple_ble_uuid;
	find_param.find_char.p_handle = &char_handle;

	char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_READ;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_val_handle = char_handle;
		printf("[APP] Read V0 handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find read v0 characteristic fail.\r\n");
	}

	char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_WRITE;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_WRITE_V1].char_val_handle = char_handle;
		printf("[APP] Write V1 handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find write v1 characteristic fail.\r\n");
	}

	char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].char_val_handle = char_handle;
		printf("[APP] Notify V2 handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find notify v2 characteristic fail.\r\n");
	}

	char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_INDICATE;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].char_val_handle = char_handle;
		printf("[APP] Indicate V3 handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find indicate v3 characteristic fail.\r\n");
	}

	return RTK_BT_OK;
}

static uint16_t simple_ble_client_cccd_find(uint16_t conn_handle)
{
	uint16_t cccd_handle;
	bool support_notify, support_indicate;
	rtk_bt_gattc_find_param_t find_param = {0};
	rtk_bt_gattc_uuid_t char_uuid = {
		.is_uuid16 = true,
	};
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db || conn_simple_ble_db->disc_state != DISC_DONE) {
		return RTK_BT_FAIL;
	}

	find_param.conn_handle = conn_handle;
	find_param.type = RTK_BT_GATT_FIND_CHARACTERISTIC_CCCD_HANDLE;
	find_param.find_char_cccd.srv_uuid = simple_ble_uuid;
	find_param.find_char_cccd.p_handle = &cccd_handle;
	find_param.find_char_cccd.p_notify = &support_notify;
	find_param.find_char_cccd.p_indicate = &support_indicate;

	char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY;
	find_param.find_char_cccd.char_uuid = char_uuid;
	if( rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].cccd_handle = cccd_handle;
		printf("[APP] Notify V2 CCCD handle is 0x%04x.\r\n", cccd_handle);
	}

	char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_INDICATE;
	find_param.find_char_cccd.char_uuid = char_uuid;
	if( rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].cccd_handle = cccd_handle;
		printf("[APP] Indicate V3 CCCD handle is 0x%04x.\r\n", cccd_handle);
	}
	return RTK_BT_OK;
}

#else /* #if RTK_BLE_MGR_LIB */

/**
 * @brief It is called in gattc callback when service discover is done
 */
static uint16_t simple_ble_client_charac_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL;
	disc_param.disc_char_all.start_handle = conn_simple_ble_db->start_handle;
	disc_param.disc_char_all.end_handle = conn_simple_ble_db->end_handle;

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_simple_ble_db->disc_state = DISC_START;
	return RTK_BT_OK;
}

/**
 * @brief It is called in gattc callback when charac discover is done
 */
static uint16_t simple_ble_client_desc_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_simble_db = simple_ble_database[conn_id];

	if (!conn_simble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL;
	/* here we only care about the descriptors of v2 and v3, 
		so discover desc in this two charac */
	disc_param.disc_char_by_uuid.start_handle = 
			conn_simble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].char_val_handle;
	disc_param.disc_char_by_uuid.end_handle = conn_simble_db->end_handle;

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_simble_db->disc_state = DISC_START;
	return RTK_BT_OK;
}

#endif /* #if RTK_BLE_MGR_LIB */

static void simple_ble_client_discover_res_hdl(void *data)
{
#if RTK_BLE_MGR_LIB
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;

	if(disc_res->is_found) {
		printf("\r\n[APP] Simple ble client discover all success\r\n");
		simple_ble_client_attach_conn(disc_res->conn_handle);
		simple_ble_client_char_find(disc_res->conn_handle);
		simple_ble_client_cccd_find(disc_res->conn_handle);
	}
#else

	uint16_t uuid = 0;
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;
	rtk_bt_status_t disc_status = disc_res->status;
	uint16_t conn_handle = disc_res->conn_handle;
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_simble_db = simple_ble_database[conn_id];

	if (!conn_simble_db || conn_simble_db->disc_state != DISC_START) {
		return;
	}

	if (RTK_BT_STATUS_CONTINUE == disc_status) {
		switch (disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			conn_simble_db->start_handle = disc_res->disc_primary_all_per.start_handle;
			conn_simble_db->end_handle = disc_res->disc_primary_all_per.end_handle;
			break;
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL: {
			uint8_t properties = disc_res->disc_char_all_per.properties;
			uint16_t value_handle = disc_res->disc_char_all_per.value_handle;
			if (BT_UUID_TYPE_16 == disc_res->disc_char_all_per.uuid_type) {
				memcpy(&uuid, disc_res->disc_char_all_per.uuid, 
														sizeof(uint16_t));
			}
			switch (uuid) {
			case SIMPLE_BLE_UUID_CHAR_VAL_READ:
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_val_handle = value_handle;
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].properties = properties;
				break;
			case SIMPLE_BLE_UUID_CHAR_VAL_WRITE:
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_WRITE_V1].char_val_handle = value_handle;
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_WRITE_V1].properties = properties;
				break;
			case SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY:
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].char_val_handle = value_handle;
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].properties = properties;
				break;
			case SIMPLE_BLE_UUID_CHAR_VAL_INDICATE:
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].char_val_handle = value_handle;
				conn_simble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].properties = properties;
				break;
			default:
				break;
			}
			break;
		}
		case RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL: {
			uint16_t desc_handle = disc_res->disc_descriptor_per.handle;
			memcpy(&uuid, disc_res->disc_descriptor_per.uuid, sizeof(uint16_t));
			if (BT_UUID_GATT_CCC_VAL == uuid) {
				if (desc_handle > conn_simble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].char_val_handle &&
					desc_handle < conn_simble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].char_val_handle) {
					conn_simble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].cccd_handle = desc_handle;
				} else if (desc_handle > conn_simble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].char_val_handle &&
						   desc_handle <= conn_simble_db->end_handle) {
					conn_simble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].cccd_handle = desc_handle;
				}
				
			}
			break;
		}
		default:
			break;
		}
	} else if (RTK_BT_STATUS_DONE == disc_status) {
		switch (disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			if (0 == conn_simble_db->start_handle && 0 == conn_simble_db->end_handle) {
				conn_simble_db->disc_state = DISC_FAILED;
				printf("[APP] Simple ble client discover service failed\r\n");
			} else {
				simple_ble_client_charac_discover(conn_handle);
			}
			break;
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL:
			simple_ble_client_desc_discover(conn_handle);
			break;
		case RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL:
			conn_simble_db->disc_state = DISC_DONE;
			printf("[APP] Simple ble client discover all success\r\n");
			break;
		default:
			break;
		}
	}

#if SIMPLE_BLE_CLIENT_SHOW_DETAIL
	general_client_discover_res_hdl(data);
#endif

#endif /* #if RTK_BLE_MGR_LIB */
}

static void simple_ble_client_read_res_hdl(void *data)
{
	rtk_bt_gattc_read_ind_t *read_res = (rtk_bt_gattc_read_ind_t *)data;
	rtk_bt_status_t read_status = read_res->status;
	uint16_t conn_handle = read_res->conn_handle;
#if RTK_BLE_MGR_LIB
	uint16_t len = read_res->len;
	uint8_t *value = read_res->value;
	uint16_t att_handle = read_res->handle;
#else
	uint16_t len = read_res->by_handle.len;
	uint8_t *value = read_res->by_handle.value;
	uint16_t att_handle = read_res->by_handle.handle;
#endif
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return;
	}

#if RTK_BLE_MGR_LIB
	if (RTK_BT_STATUS_DONE != read_status) {
		printf("[APP] Simple ble client read fail\r\n");
		return;
	}
#else
	if (RTK_BT_STATUS_CONTINUE != read_status)
		return;
#endif

	if (att_handle == conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_val_handle) {
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_data =
						(void *)osif_mem_alloc(RAM_TYPE_DATA_ON, len);
		memcpy(conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_data,
				value, len);
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].data_len = len;
		printf("[APP] Simple ble client read charac v0: %s\r\n ",
				(char *)conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_data);
	}
}

static void simple_ble_client_write_res_hdl(void *data)
{
	rtk_bt_gattc_write_ind_t *write_res = (rtk_bt_gattc_write_ind_t *)data;
	rtk_bt_status_t write_status = write_res->status;
	uint16_t att_handle = write_res->handle;
	uint16_t conn_handle = write_res->conn_handle;
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return;
	}
	if (RTK_BT_STATUS_DONE == write_status) {
		if (att_handle == conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_WRITE_V1].char_val_handle) {
			printf("[APP] Simple ble client write charac v1 success\r\n");
		}
	}
}

static void simple_ble_client_notify_hdl(void *data)
{
	rtk_bt_gattc_cccd_value_ind_t *ntf_ind = (rtk_bt_gattc_cccd_value_ind_t *)data;
	uint16_t att_handle = ntf_ind->value_handle;
	uint16_t conn_handle = ntf_ind->conn_handle;
	simple_ble_client_db_t *conn_simple_ble_db;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	conn_simple_ble_db = simple_ble_database[conn_id];
	
	if (!conn_simple_ble_db ||
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].char_val_handle != att_handle) {
		return;
	}

	printf("[APP] Simple ble client charac v2 notify received\r\n");
	gattc_dump(ntf_ind->len, ntf_ind->value, (uint8_t*)"notify event");
}

static void simple_ble_client_indicate_hdl(void *data)
{
	uint16_t ret = 0;
	rtk_bt_gattc_cfm_param_t cfm_param = {0};
	rtk_bt_gattc_cccd_value_ind_t *indicate_ind = 
										(rtk_bt_gattc_cccd_value_ind_t *)data;
	uint16_t att_handle = indicate_ind->value_handle;
	uint16_t conn_handle = indicate_ind->conn_handle;
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db ||
		conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].char_val_handle != att_handle) {
		return;
	}

	printf("[APP] Simple ble client charac v3 indicate received\r\n");
	gattc_dump(indicate_ind->len, indicate_ind->value, (uint8_t*)"indicate event");

	cfm_param.profile_id = indicate_ind->profile_id;
	cfm_param.conn_handle = indicate_ind->conn_handle;
#if RTK_BLE_MGR_LIB
	cfm_param.cid = indicate_ind->cid;
#endif
	ret = rtk_bt_gattc_confirm(&cfm_param);
	if (RTK_BT_OK != ret) {
		printf("[APP] Simple ble client confirm for indication failed! err: 0x%x\r\n", ret);
	}
}

static void simple_ble_client_cccd_enable_hdl(void *data)
{
	rtk_bt_gattc_cccd_update_ind_t *cccd_update = 
									(rtk_bt_gattc_cccd_update_ind_t *)data;
	rtk_bt_status_t status = cccd_update->status;;
	uint16_t conn_handle = cccd_update->conn_handle;
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db || RTK_BT_STATUS_DONE != status) {
		return;
	}

#if RTK_BLE_MGR_LIB
	if (cccd_update->uuid.p.uuid16 == SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY) {
		printf("[APP] Simple ble client enable char v2 notify succeed\r\n");
	} else if (cccd_update->uuid.p.uuid16 == SIMPLE_BLE_UUID_CHAR_VAL_INDICATE) {
		printf("[APP] Simple ble client enable char v3 indicate succeed\r\n");
	}
#else
	if (conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].cccd_handle
												== cccd_update->cccd_handle) {
		printf("[APP] Simple ble client enable char v2 notify succeed\r\n");
	} else if (conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].cccd_handle
												== cccd_update->cccd_handle) {
		printf("[APP] Simple ble client enable char v3 indicate succeed\r\n");
	}
#endif
}

static void simple_ble_client_cccd_disable_hdl(void *data)
{
	rtk_bt_gattc_cccd_update_ind_t *cccd_update = 
									(rtk_bt_gattc_cccd_update_ind_t *)data;
	rtk_bt_status_t status = cccd_update->status;
	uint16_t conn_handle = cccd_update->conn_handle;
	uint8_t conn_id;
	simple_ble_client_db_t *conn_simple_ble_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db || RTK_BT_STATUS_DONE != status) {
		return;
	}

#if RTK_BLE_MGR_LIB
	if (cccd_update->uuid.p.uuid16 == SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY) {
		printf("[APP] Simple ble client disable char v2 notify succeed\r\n");
	} else if (cccd_update->uuid.p.uuid16 == SIMPLE_BLE_UUID_CHAR_VAL_INDICATE) {
		printf("[APP] Simple ble client disable char v3 indicate succeed\r\n");
	}
#else
	if (conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].cccd_handle
												== cccd_update->cccd_handle) {
		printf("[APP] Simple ble client disable char v2 notify succeed\r\n");
	} else if (conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].cccd_handle
												== cccd_update->cccd_handle) {
		printf("[APP] Simple ble client disable char v3 indicate succeed\r\n");
	}
#endif
}

rtk_bt_evt_cb_ret_t simple_ble_client_app_callback(uint8_t event, void *data)
{
	switch (event) {
	case RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND:
		simple_ble_client_discover_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		simple_ble_client_read_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_WRITE_RESULT_IND:
		simple_ble_client_write_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_NOTIFY_IND:
		simple_ble_client_notify_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		simple_ble_client_indicate_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_CCCD_ENABLE_IND:
		simple_ble_client_cccd_enable_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_CCCD_DISABLE_IND:
		simple_ble_client_cccd_disable_hdl(data);
		break;
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

static uint16_t simple_ble_client_database_free(uint8_t conn_id)
{
	uint32_t i = 0;
	void *p_char_data;

	if (!simple_ble_database[conn_id]) {
		return RTK_BT_OK;
	}

	for (i = 0; i < SIMPLE_BLE_CHAR_NUM; i++) {
		p_char_data = simple_ble_database[conn_id]->char_db[i].char_data;
		if (p_char_data != NULL) {
			osif_mem_free(p_char_data);
		}
	}
	osif_mem_free(simple_ble_database[conn_id]);
	simple_ble_database[conn_id] = NULL;

	return RTK_BT_OK;
}

uint16_t simple_ble_client_attach_conn(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	simple_ble_database[conn_id] = (simple_ble_client_db_t *)
				osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(simple_ble_client_db_t));
	memset(simple_ble_database[conn_id], 0, sizeof(simple_ble_client_db_t));

#if RTK_BLE_MGR_LIB
	simple_ble_database[conn_id]->disc_state = DISC_DONE;
	return RTK_BT_OK;
#else
	return rtk_bt_gattc_attach_connect(SIMPLE_BLE_CLIENT_PROFILE_ID, conn_handle);
#endif
}

uint16_t simple_ble_client_detach_conn(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	ret = simple_ble_client_database_free(conn_id);
	if (ret) {
		return ret;
	}

#if RTK_BLE_MGR_LIB
	return RTK_BT_OK;
#else
	return rtk_bt_gattc_detach_connect(SIMPLE_BLE_CLIENT_PROFILE_ID, conn_handle);
#endif
}

uint16_t simple_ble_client_add(void)
{
#if RTK_BLE_MGR_LIB
	return rtk_bt_gattc_register_profile(SIMPLE_BLE_CLIENT_PROFILE_ID, simple_ble_uuid);
#else
	return rtk_bt_gattc_register_profile(SIMPLE_BLE_CLIENT_PROFILE_ID);
#endif
}

uint16_t simple_ble_client_delete(void)
{
	int i = 0;
	uint16_t ret = 0;

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		ret = simple_ble_client_database_free(i);
		if (ret) {
			return ret;
		}
	}

#if RTK_BLE_MGR_LIB
	return rtk_bt_gattc_unregister_profile(SIMPLE_BLE_CLIENT_PROFILE_ID, simple_ble_uuid);
#else
	return rtk_bt_gattc_unregister_profile(SIMPLE_BLE_CLIENT_PROFILE_ID);
#endif
}

#if !RTK_BLE_MGR_LIB
uint16_t simple_ble_client_srv_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint16_t simple_ble_uuid = SIMPLE_BLE_UUID_SRV;
	simple_ble_client_db_t *conn_simple_ble_db;
	uint8_t conn_id;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	disc_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID;
	disc_param.disc_primary_by_uuid.uuid_type = BT_UUID_TYPE_16;
	memcpy(disc_param.disc_primary_by_uuid.uuid, &simple_ble_uuid, BT_UUID_SIZE_16);

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_simple_ble_db->disc_state = DISC_START;
	return RTK_BT_OK;
}
#endif

uint16_t simple_ble_client_read_charac_v0(uint16_t conn_handle)
{
	rtk_bt_gattc_read_param_t read_param = {0};
	simple_ble_client_db_t *conn_simple_ble_db;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (conn_simple_ble_db->disc_state != DISC_DONE) {
		printf("[APP] Simple ble client need discover service before read charac !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

	read_param.conn_handle = conn_handle;
	read_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
	read_param.by_handle.handle = 
				conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_READ_V0].char_val_handle;

	return rtk_bt_gattc_read(&read_param);
}

uint16_t simple_ble_client_write_charac_v1(uint16_t conn_handle, uint8_t *data, uint16_t len)
{
	rtk_bt_gattc_write_param_t write_param = {0};
	simple_ble_client_db_t *conn_simple_ble_db;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (conn_simple_ble_db->disc_state != DISC_DONE) {
		printf("[APP] Simple ble client need discover service before write charac !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

	write_param.conn_handle = conn_handle;
	write_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	write_param.type = RTK_BT_GATT_CHAR_WRITE_REQ;
	write_param.handle = 
			conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_WRITE_V1].char_val_handle;
	write_param.length = len;
	write_param.data = data;

	return rtk_bt_gattc_write(&write_param);
}

uint16_t simple_ble_client_set_notify_charac_v2(uint16_t conn_handle, bool enable)
{
	uint16_t ret = 0;
	rtk_bt_gattc_update_cccd_param_t cccd_param = {0};
	simple_ble_client_db_t *conn_simple_ble_db;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (conn_simple_ble_db->disc_state != DISC_DONE) {
		printf("[APP] Simple ble client need discover service before wrtie cccd !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

	cccd_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	cccd_param.conn_handle = conn_handle;
	cccd_param.bnotify = true;
#if RTK_BLE_MGR_LIB
	cccd_param.srv_cfg = false;
	cccd_param.srv_uuid = simple_ble_uuid;
	cccd_param.char_uuid.is_uuid16 = true;
	cccd_param.char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_NOTIFY;
#else
	cccd_param.char_val_handle = 
			conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].char_val_handle;
	cccd_param.cccd_handle = 
			conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_NOTIFY_V2].cccd_handle;
#endif

	if (enable) {
		ret = rtk_bt_gattc_enable_notify_or_indicate(&cccd_param);
	} else {
		ret = rtk_bt_gattc_disable_notify_or_indicate(&cccd_param);
	}
	if (RTK_BT_OK != ret) {
		return ret;
	}

	return RTK_BT_OK;
}

uint16_t simple_ble_client_set_indicate_charac_v3(uint16_t conn_handle, bool enable)
{
	uint16_t ret = 0;
	rtk_bt_gattc_update_cccd_param_t cccd_param = {0};
	simple_ble_client_db_t *conn_simple_ble_db;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_simple_ble_db = simple_ble_database[conn_id];

	if (!conn_simple_ble_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (conn_simple_ble_db->disc_state != DISC_DONE) {
		printf("[APP] Simple ble client need discover service before wrtie cccd !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

	cccd_param.profile_id = SIMPLE_BLE_CLIENT_PROFILE_ID;
	cccd_param.conn_handle = conn_handle;
	cccd_param.bindicate = true;
#if RTK_BLE_MGR_LIB
	cccd_param.srv_cfg = false;
	cccd_param.srv_uuid = simple_ble_uuid;
	cccd_param.char_uuid.is_uuid16 = true;
	cccd_param.char_uuid.p.uuid16 = SIMPLE_BLE_UUID_CHAR_VAL_INDICATE;
#else
	cccd_param.char_val_handle = 
			conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].char_val_handle;
	cccd_param.cccd_handle = 
			conn_simple_ble_db->char_db[SIMPLE_BLE_CHAR_INDICATE_V3].cccd_handle;
#endif

	if (enable) {
		ret = rtk_bt_gattc_enable_notify_or_indicate(&cccd_param);
	} else {
		ret = rtk_bt_gattc_disable_notify_or_indicate(&cccd_param);
	}
	if (RTK_BT_OK != ret) {
		return ret;
	}

	return RTK_BT_OK;
}




