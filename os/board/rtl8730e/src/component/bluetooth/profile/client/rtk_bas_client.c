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
#include <rtk_bas_client.h>
#include <rtk_gcs_client.h>

#define BATTERY_SRV_UUID                    	0x180F
#define BATTERY_LEVEL_CHAR_UUID                 0x2A19

#if RTK_BLE_MGR_LIB
rtk_bt_gattc_uuid_t bas_uuid = {
	.is_uuid16 = true,
	.p.uuid16 = BATTERY_SRV_UUID
};
rtk_bt_gattc_uuid_t battery_level_uuid = {
	.is_uuid16 = true,
	.p.uuid16 = BATTERY_LEVEL_CHAR_UUID
};
#endif

typedef struct {
	client_disc_state_t disc_state;
	uint16_t start_handle; /*  battery service start handle */
	uint16_t end_handle; /* battery service end handle */
	uint8_t properties; /* battery level charac properties */
	uint16_t char_val_handle;  /* battery level charac value handle */
	uint16_t cccd_handle;
	uint8_t battery_level;
	bool notify_enable;
} bas_client_db_t;

bas_client_db_t *bas_database[RTK_BLE_GAP_MAX_LINKS] = {0};

#if RTK_BLE_MGR_LIB
static uint16_t bas_client_char_find(uint16_t conn_handle)
{
	uint16_t ret = 0;
	bas_client_db_t *conn_bas_db;
	uint16_t battery_level_handle;
	rtk_bt_gattc_find_param_t find_param = {0};
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db || conn_bas_db->disc_state != DISC_DONE) {
		return RTK_BT_FAIL;
	}

	find_param.conn_handle = conn_handle;
	find_param.type = RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE;
	find_param.find_char.srv_uuid = bas_uuid;
	find_param.find_char.char_uuid = battery_level_uuid;
	find_param.find_char.p_handle = &battery_level_handle;

	ret = rtk_bt_gattc_find(&find_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_bas_db->char_val_handle = battery_level_handle;
	printf("[APP] Battery Level handle is 0x%04x.\r\n", battery_level_handle);
	return RTK_BT_OK;
}

static uint16_t bas_client_cccd_find(uint16_t conn_handle)
{
	uint16_t ret = 0;
	bas_client_db_t *conn_bas_db;
	uint16_t cccd_handle;
	bool support_notify, support_indicate;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_bas_db = bas_database[conn_id];

	rtk_bt_gattc_find_param_t find_param = {0};

	if (!conn_bas_db || conn_bas_db->disc_state != DISC_DONE) {
		return RTK_BT_FAIL;
	}

	find_param.conn_handle = conn_handle;
	find_param.type = RTK_BT_GATT_FIND_CHARACTERISTIC_CCCD_HANDLE;
	find_param.find_char_cccd.srv_uuid = bas_uuid;
	find_param.find_char_cccd.char_uuid = battery_level_uuid;
	find_param.find_char_cccd.p_handle = &cccd_handle;
	find_param.find_char_cccd.p_notify = &support_notify;
	find_param.find_char_cccd.p_indicate = &support_indicate;

	ret = rtk_bt_gattc_find(&find_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_bas_db->cccd_handle = cccd_handle;
	printf("[APP] Battery Level CCCD handle is 0x%04x.\r\n", cccd_handle);
	return RTK_BT_OK;
}

#else /* #if RTK_BLE_MGR_LIB */

static uint16_t bas_client_char_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint16_t battery_level_uuid = BATTERY_LEVEL_CHAR_UUID;
	bas_client_db_t *conn_bas_db;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = BAS_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID;
	disc_param.disc_char_by_uuid.start_handle = conn_bas_db->start_handle;
	disc_param.disc_char_by_uuid.end_handle = conn_bas_db->end_handle;
	disc_param.disc_char_by_uuid.uuid_type = BT_UUID_TYPE_16;
	memcpy(disc_param.disc_char_by_uuid.uuid, &battery_level_uuid, 
													BT_UUID_SIZE_16);

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_bas_db->disc_state = DISC_START;
	return RTK_BT_OK;
}

static uint16_t bas_client_descriptor_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	bas_client_db_t *conn_bas_db;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = BAS_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL;
	disc_param.disc_char_by_uuid.start_handle = conn_bas_db->char_val_handle;
	disc_param.disc_char_by_uuid.end_handle = conn_bas_db->end_handle;

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_bas_db->disc_state = DISC_START;
	return RTK_BT_OK;
}

#endif /* #if RTK_BLE_MGR_LIB */

static void bas_client_discover_res_hdl(void *data)
{
#if RTK_BLE_MGR_LIB
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;

	if(disc_res->is_found) {
		printf("\r\n[APP] BAS client discover all success\r\n");
		bas_client_attach_conn(disc_res->conn_handle);
		bas_client_char_find(disc_res->conn_handle);
		bas_client_cccd_find(disc_res->conn_handle);
	}

#else

	uint16_t uuid = 0;
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;
	rtk_bt_status_t disc_status = disc_res->status;
	bas_client_db_t *conn_bas_db;
	uint16_t conn_handle = disc_res->conn_handle;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db || conn_bas_db->disc_state != DISC_START) {
		return;
	}

	if (RTK_BT_STATUS_CONTINUE == disc_status) {
		switch(disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			conn_bas_db->start_handle = disc_res->disc_primary_all_per.start_handle;
			conn_bas_db->end_handle = disc_res->disc_primary_all_per.end_handle;
			break;
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID:
			conn_bas_db->char_val_handle = disc_res->disc_char_by_uuid_per.value_handle;
			conn_bas_db->properties = disc_res->disc_char_by_uuid_per.properties;
			break;
		case RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL: 
			memcpy(&uuid, disc_res->disc_descriptor_per.uuid, sizeof(uint16_t));
			if (BT_UUID_GATT_CCC_VAL == uuid) {
				conn_bas_db->cccd_handle = disc_res->disc_descriptor_per.handle;
			}
			break;
		default:
			break;
		}
	} else if (RTK_BT_STATUS_DONE == disc_status) {
		switch(disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			if (0 == conn_bas_db->start_handle && 0 == conn_bas_db->end_handle) {
				conn_bas_db->disc_state = DISC_FAILED;
			} else {
				bas_client_char_discover(conn_handle);
			}
			break;
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID:
			if (conn_bas_db->properties & RTK_BT_GATT_CHRC_NOTIFY) {
				bas_client_descriptor_discover(conn_handle);
			}
			break;
		case RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL:
			conn_bas_db->disc_state = DISC_DONE;
			printf("[APP] BAS client discover all success\r\n");
			break;
		default:
			break;
		}
	} else {
		conn_bas_db->disc_state = DISC_FAILED;
	}

#if BAS_CLIENT_SHOW_DETAIL
	general_client_discover_res_hdl(data);
#endif

#endif /* #if RTK_BLE_MGR_LIB */
}

static void bas_client_read_res_hdl(void *data)
{
	rtk_bt_gattc_read_ind_t *read_res = (rtk_bt_gattc_read_ind_t *)data;
	rtk_bt_status_t read_status = read_res->status;
#if RTK_BLE_MGR_LIB
	uint16_t att_handle = read_res->handle;
	uint8_t *value = read_res->value;
#else
	uint16_t att_handle = read_res->by_handle.handle;
	uint8_t *value = read_res->by_handle.value;
#endif
	uint16_t conn_handle = read_res->conn_handle;
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db ||
		(RTK_BT_STATUS_DONE != read_status && RTK_BT_STATUS_CONTINUE != read_status)) {
		return;
	}

	if (att_handle == conn_bas_db->char_val_handle) {
		conn_bas_db->battery_level = *value;
		printf("[APP] BAS client read battery level: %d\r\n", conn_bas_db->battery_level);
	} else if (att_handle == conn_bas_db->cccd_handle) {
		if (*(uint16_t *)value & RTK_BT_GATT_CCC_NOTIFY) {
			conn_bas_db->notify_enable = true;
		} else {
			conn_bas_db->notify_enable = false;
		}
		printf("[APP] BAS client read notify bit: %d\r\n", conn_bas_db->notify_enable);
	}
#if BAS_CLIENT_SHOW_DETAIL && !RTK_BLE_MGR_LIB
	general_client_read_res_hdl(data);
#endif

}

static void bas_client_write_res_hdl(void *data)
{
	rtk_bt_gattc_write_ind_t *write_res = (rtk_bt_gattc_write_ind_t *)data;
	rtk_bt_status_t write_status = write_res->status;
	uint16_t conn_handle = write_res->conn_handle;
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db || RTK_BT_STATUS_DONE != write_status) {
		return;
	}

	if (write_res->handle == conn_bas_db->cccd_handle) {
		printf("[APP] BAS client set notify success\r\n");
	}

#if BAS_CLIENT_SHOW_DETAIL && !RTK_BLE_MGR_LIB
	general_client_write_res_hdl(data);
#endif

}

static void bas_client_notify_hdl(void *data)
{
	rtk_bt_gattc_cccd_value_ind_t *notify_ind = (rtk_bt_gattc_cccd_value_ind_t *)data;
	uint16_t conn_handle = notify_ind->conn_handle;
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return;
	}
	if (notify_ind->value_handle == conn_bas_db->char_val_handle) {
		printf("[APP] BAS client notify battery level: %d\r\n", *notify_ind->value);
	}

	return;
}

static void bas_client_cccd_enable_hdl(void *data)
{
	rtk_bt_gattc_cccd_update_ind_t *cccd_update = (rtk_bt_gattc_cccd_update_ind_t *)data;
	uint16_t conn_handle = cccd_update->conn_handle;
	rtk_bt_status_t status = cccd_update->status;
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db)
		return;

#if RTK_BLE_MGR_LIB
	if (cccd_update->uuid.p.uuid16 != BATTERY_LEVEL_CHAR_UUID)
		return;
#else
	if (cccd_update->cccd_handle != conn_bas_db->cccd_handle)
		return;
#endif

	if (RTK_BT_STATUS_DONE == status && cccd_update->bnotify) {
		printf("[APP] BAS client enable notify succeed\r\n");
	} else {
		printf("[APP] BAS client enable notify failed\r\n");
	}
}

static void bas_client_cccd_disable_hdl(void *data)
{
	rtk_bt_gattc_cccd_update_ind_t *cccd_update = (rtk_bt_gattc_cccd_update_ind_t *)data;
	uint16_t conn_handle = cccd_update->conn_handle;
	rtk_bt_status_t status = cccd_update->status;
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db)
		return;

#if RTK_BLE_MGR_LIB
	if (cccd_update->uuid.p.uuid16 != BATTERY_LEVEL_CHAR_UUID)
		return;
#else
	if (cccd_update->cccd_handle != conn_bas_db->cccd_handle)
		return;
#endif

	if (RTK_BT_STATUS_DONE == status && cccd_update->bnotify) {
		printf("[APP] BAS client disable notify succeed\r\n");
	} else {
		printf("[APP] BAS client disable notify failed\r\n");
	}
}

rtk_bt_evt_cb_ret_t bas_client_app_callback(uint8_t event, void *data)
{
	switch (event) {
	case RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND:
		bas_client_discover_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		bas_client_read_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_WRITE_RESULT_IND:
		bas_client_write_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_NOTIFY_IND:
		bas_client_notify_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_INDICATE_IND:
		// bas_client_indicate_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_CCCD_ENABLE_IND:
		bas_client_cccd_enable_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_CCCD_DISABLE_IND:
		bas_client_cccd_disable_hdl(data);
		break;
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

uint16_t bas_client_attach_conn(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	bas_database[conn_id] = 
		(bas_client_db_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(bas_client_db_t));
	memset(bas_database[conn_id], 0, sizeof(bas_client_db_t));

#if RTK_BLE_MGR_LIB
	bas_database[conn_id]->disc_state = DISC_DONE;
	return RTK_BT_OK;
#else
	return rtk_bt_gattc_attach_connect(BAS_CLIENT_PROFILE_ID, conn_handle);
#endif
}

uint16_t bas_client_detach_conn(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	osif_mem_free(bas_database[conn_id]);
	bas_database[conn_id] = NULL;
#if RTK_BLE_MGR_LIB
	return RTK_BT_OK;
#else
	return rtk_bt_gattc_detach_connect(BAS_CLIENT_PROFILE_ID, conn_handle);
#endif
}

uint16_t bas_client_add(void)
{
#if RTK_BLE_MGR_LIB
	return rtk_bt_gattc_register_profile(BAS_CLIENT_PROFILE_ID, bas_uuid);
#else
	return rtk_bt_gattc_register_profile(BAS_CLIENT_PROFILE_ID);
#endif
}

uint16_t bas_client_delete(void)
{
	int i = 0;

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		if (bas_database[i]) {
			osif_mem_free(bas_database[i]);
			bas_database[i] = NULL;
		}
	}

#if RTK_BLE_MGR_LIB
	return rtk_bt_gattc_unregister_profile(BAS_CLIENT_PROFILE_ID, bas_uuid);
#else
	return rtk_bt_gattc_unregister_profile(BAS_CLIENT_PROFILE_ID);
#endif
}

#if !RTK_BLE_MGR_LIB
uint16_t bas_client_srv_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint16_t bas_uuid = BATTERY_SRV_UUID;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = BAS_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID;
	disc_param.disc_primary_by_uuid.uuid_type = BT_UUID_TYPE_16;
	memcpy(disc_param.disc_primary_by_uuid.uuid, &bas_uuid, BT_UUID_SIZE_16);

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_bas_db->disc_state = DISC_START;
	return RTK_BT_OK;
}
#endif

uint16_t bas_client_read_battery_level(uint16_t conn_handle)
{
	rtk_bt_gattc_read_param_t read_param = {0};
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (conn_bas_db->disc_state != DISC_DONE) {
		printf("[APP] BAS client need discover service before read battery level !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

	read_param.conn_handle = conn_handle;
	read_param.profile_id = BAS_CLIENT_PROFILE_ID;
	read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
	read_param.by_handle.handle = conn_bas_db->char_val_handle;

	return rtk_bt_gattc_read(&read_param);
}

uint16_t bas_client_set_notify(uint16_t conn_handle, bool enable)
{
	uint16_t ret = 0;
	rtk_bt_gattc_update_cccd_param_t cccd_param = {0};
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}
	if (conn_bas_db->disc_state != DISC_DONE) {
		printf("[APP] BAS client need discover service before set notify !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

#if RTK_BLE_MGR_LIB
	cccd_param.srv_cfg = false;
	cccd_param.srv_uuid = bas_uuid;
	cccd_param.char_uuid = battery_level_uuid;
#else
	cccd_param.char_val_handle = conn_bas_db->char_val_handle;
	cccd_param.cccd_handle = conn_bas_db->cccd_handle;
#endif

	cccd_param.profile_id = BAS_CLIENT_PROFILE_ID;
	cccd_param.conn_handle = conn_handle;
	cccd_param.bnotify = true;
	
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

uint16_t bas_client_read_notify(uint16_t conn_handle)
{
	rtk_bt_gattc_read_param_t read_param = {0};
	uint8_t conn_id;
	bas_client_db_t *conn_bas_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_bas_db = bas_database[conn_id];

	if (!conn_bas_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}
	if (conn_bas_db->disc_state != DISC_DONE) {
		printf("[APP] BAS client need discover service before read notify !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}
	
	read_param.conn_handle = conn_handle;
	read_param.profile_id = BAS_CLIENT_PROFILE_ID;
	read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
	read_param.by_handle.handle = conn_bas_db->cccd_handle;

	return rtk_bt_gattc_read(&read_param);
}
