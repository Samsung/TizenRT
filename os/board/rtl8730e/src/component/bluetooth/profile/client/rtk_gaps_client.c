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
#include <rtk_gaps_client.h>
#include <rtk_gcs_client.h>

#define GAP_SRV_UUID                    0x1800
#define DEVICE_NAME_CHAR_UUID           0x2A00
#define APPEARANCE_CHAR_UUID            0x2A01
/* peripheral preferred connection parameter charac */
#define PPCP_CHAR_UUID                  0x2A04
#define CENTRAL_ADDR_RES_CHAR_UUID      0x2AA6

#if RTK_BLE_MGR_LIB
rtk_bt_gattc_uuid_t gaps_uuid = {
	.is_uuid16 = true,
	.p.uuid16 = GAP_SRV_UUID
};
#endif

typedef struct {
	uint8_t properties;
	uint16_t char_val_handle;
	void *char_data;
	uint16_t data_len;
} gaps_charac_db_t;

typedef struct {
	client_disc_state_t disc_state;
	uint16_t start_handle;
	uint16_t end_handle;
	gaps_charac_db_t char_db[GAPS_CHAR_NUM];
} gaps_client_db_t;

static gaps_client_db_t *gaps_database[RTK_BLE_GAP_MAX_LINKS] = {0};

#if !RTK_BLE_MGR_LIB
uint16_t gaps_client_srv_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint16_t gaps_uuid = GAP_SRV_UUID;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;
	gaps_client_db_t *conn_gaps_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_gaps_db = gaps_database[conn_id];

	if (!conn_gaps_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = GAPS_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID;
	disc_param.disc_primary_by_uuid.uuid_type = BT_UUID_TYPE_16;
	memcpy(disc_param.disc_primary_by_uuid.uuid, &gaps_uuid, BT_UUID_SIZE_16);


	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_gaps_db->disc_state = DISC_START;
	return RTK_BT_OK;
}
#endif

#if RTK_BLE_MGR_LIB
static uint16_t gaps_client_char_find(uint16_t conn_handle)
{
	uint8_t conn_id;
	gaps_client_db_t *conn_gaps_db;
	uint16_t char_handle;
	rtk_bt_gattc_find_param_t find_param = {0};
	rtk_bt_gattc_uuid_t char_uuid = {
		.is_uuid16 = true,
	};

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_gaps_db = gaps_database[conn_id];

	find_param.conn_handle = conn_handle;
	find_param.type = RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE;
	find_param.find_char.srv_uuid = gaps_uuid;
	find_param.find_char.p_handle = &char_handle;

	char_uuid.p.uuid16 = DEVICE_NAME_CHAR_UUID;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].char_val_handle = char_handle;
		printf("[APP] device name handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find device name characteristic fail.\r\n");
	}

	char_uuid.p.uuid16 = APPEARANCE_CHAR_UUID;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_val_handle = char_handle;
		printf("[APP] apperance handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find apperance characteristic fail.\r\n");
	}

	char_uuid.p.uuid16 = PPCP_CHAR_UUID;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_val_handle = char_handle;
		printf("[APP] PPCP handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find PPCP characteristic fail.\r\n");
	}

	char_uuid.p.uuid16 = CENTRAL_ADDR_RES_CHAR_UUID;
	find_param.find_char.char_uuid = char_uuid;
	if (rtk_bt_gattc_find(&find_param) == RTK_BT_OK) {
		conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].char_val_handle = char_handle;
		printf("[APP] central addr resolution handle is 0x%04x.\r\n", char_handle);
	} else {
		printf("Find central addr resolution characteristic fail.\r\n");
	}

	return RTK_BT_OK;
}

#else /* #if RTK_BLE_MGR_LIB */

uint16_t gaps_client_char_discover(uint16_t conn_handle)
{
	uint16_t ret = 0;
	rtk_bt_gattc_discover_param_t disc_param = {0};
	uint8_t conn_id;
	gaps_client_db_t *conn_gaps_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_gaps_db = gaps_database[conn_id];

	if (!conn_gaps_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	disc_param.profile_id = GAPS_CLIENT_PROFILE_ID;
	disc_param.conn_handle = conn_handle;
	disc_param.type = RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL;
	disc_param.disc_char_all.start_handle = conn_gaps_db->start_handle;
	disc_param.disc_char_all.end_handle = conn_gaps_db->end_handle;

	ret = rtk_bt_gattc_discover(&disc_param);
	if (RTK_BT_OK != ret) {
		return ret;
	}

	conn_gaps_db->disc_state = DISC_START;
	return RTK_BT_OK;
}

#endif /* #if RTK_BLE_MGR_LIB */

static void gaps_client_discover_res_hdl(void *data)
{
#if RTK_BLE_MGR_LIB
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;

	if(disc_res->is_found) {
		printf("\r\n[APP] GAPS client discover all success\r\n");
		gaps_client_attach_conn(disc_res->conn_handle);
		gaps_client_char_find(disc_res->conn_handle);
	}
#else

	uint16_t uuid = 0;
	rtk_bt_gattc_discover_ind_t *disc_res = (rtk_bt_gattc_discover_ind_t *)data;
	rtk_bt_status_t disc_status = disc_res->status;
	uint16_t conn_handle = disc_res->conn_handle;
	uint8_t conn_id;
	gaps_client_db_t *conn_gaps_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_gaps_db = gaps_database[conn_id];

	if (!conn_gaps_db || conn_gaps_db->disc_state != DISC_START) {
		return;
	}

	if (RTK_BT_STATUS_CONTINUE == disc_status) {
		switch (disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			conn_gaps_db->start_handle = disc_res->disc_primary_by_uuid_per.start_handle;
			conn_gaps_db->end_handle = disc_res->disc_primary_by_uuid_per.end_handle;
			break;
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL: {
			uint8_t properties = disc_res->disc_char_all_per.properties;
			uint16_t value_handle = disc_res->disc_char_all_per.value_handle;
			if (BT_UUID_TYPE_16 == disc_res->disc_char_all_per.uuid_type) {
				memcpy(&uuid, disc_res->disc_char_all_per.uuid, 
														sizeof(uint16_t));
			}
			switch (uuid) {
			case DEVICE_NAME_CHAR_UUID:
				conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].char_val_handle = value_handle;
				conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].properties = properties;
				break;
			case APPEARANCE_CHAR_UUID:
				conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_val_handle = value_handle;
				conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].properties = properties;
				break;
			case PPCP_CHAR_UUID:
				conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_val_handle = value_handle;
				conn_gaps_db->char_db[GAPS_CHAR_PPCP].properties = properties;
				break;
			case CENTRAL_ADDR_RES_CHAR_UUID:
				conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].char_val_handle = value_handle;
				conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].properties = properties;
				break;
			default:
				break;
			}
			break;
		}
		default:
			break; 
		}
	} else if (RTK_BT_STATUS_DONE == disc_status) {
		switch (disc_res->type) {
		case RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID:
			if (0 == conn_gaps_db->start_handle && 0 == conn_gaps_db->end_handle) {
				conn_gaps_db->disc_state = DISC_FAILED;
				printf("[APP] GAPS client discover service failed\r\n");
			} else {
				gaps_client_char_discover(conn_handle);
			}
			break;
		case RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL:
			conn_gaps_db->disc_state = DISC_DONE;
			printf("[APP] GAPS client discover all success\r\n");
			break;
		default:
			break;
		}
	}

#if GAPS_CLIENT_SHOW_DETAIL
	general_client_discover_res_hdl(data);
#endif

#endif /* #if RTK_BLE_MGR_LIB */
}

static void gaps_client_read_res_hdl(void *data)
{
	rtk_bt_gattc_read_ind_t *read_res = (rtk_bt_gattc_read_ind_t *)data;
	rtk_bt_status_t read_status = read_res->status;
#if RTK_BLE_MGR_LIB
	uint16_t len = read_res->len;
	uint8_t *value = read_res->value;
	uint16_t att_handle = read_res->handle;
#else
	uint16_t len = read_res->by_handle.len;
	uint8_t *value = read_res->by_handle.value;
	uint16_t att_handle = read_res->by_handle.handle;
#endif
	uint16_t conn_handle = read_res->conn_handle;
	uint8_t conn_id;
	gaps_client_db_t *conn_gaps_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;
	
	conn_gaps_db = gaps_database[conn_id];

	if (!conn_gaps_db) {
		return;
	}

#if RTK_BLE_MGR_LIB
	if (RTK_BT_STATUS_DONE != read_status) {
		printf("[APP] GAPS client read fail.\r\n");
		return;
	}
#else
	if (RTK_BT_STATUS_CONTINUE != read_status)
		return;
#endif

	if (att_handle == conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].char_val_handle) {
		conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].char_data = 
							(void *)osif_mem_alloc(RAM_TYPE_DATA_ON, len);
		memcpy(conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].char_data,
				value, len);
		conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].data_len = len;
		printf("[APP] GAPS client read device name: %s\r\n", 
				(char *)conn_gaps_db->char_db[GAPS_CHAR_DEVICE_NAME].char_data);

	} else if (att_handle == conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_val_handle) {
		conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_data = 
							(void *)osif_mem_alloc(RAM_TYPE_DATA_ON, len);
		memcpy(conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_data,
				value, len);
		conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].data_len = len;
		printf("[APP] GAPS client read appearance: 0x%x\r\n", 
				*(uint16_t *)conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_data);

	} else if (att_handle == conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_val_handle) {
		conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_data = 
							(void *)osif_mem_alloc(RAM_TYPE_DATA_ON, len);
		memcpy(conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_data,
				value, len);
		conn_gaps_db->char_db[GAPS_CHAR_PPCP].data_len = len;
		printf("[APP] GAPS client read peripheral preferred connection parameters, "
				"conn_interval_max: 0x%x, slave_latency: 0x%x, supervision_timeout: 0x%x\r\n", 
				*(uint16_t *)conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_data,
				*(uint16_t *)((char *)conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_data + 2),
				*(uint16_t *)((char *)conn_gaps_db->char_db[GAPS_CHAR_PPCP].char_data + 4));

	} else if (att_handle == 
				conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].char_val_handle) {
		conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].char_data = 
							(void *)osif_mem_alloc(RAM_TYPE_DATA_ON, len);
		memcpy(conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].char_data,
				value, len);
		conn_gaps_db->char_db[GAPS_CHAR_CEN_ADDR_RES].data_len = len;
		printf("[APP] GAPS client read central address resolution: %d\r\n", 
				*(uint16_t *)conn_gaps_db->char_db[GAPS_CHAR_APPEARANCE].char_data);
	}

#if GAPS_CLIENT_SHOW_DETAIL && !RTK_BLE_MGR_LIB
	general_client_read_res_hdl(data);
#endif
	return;
}

rtk_bt_evt_cb_ret_t gaps_client_app_callback(uint8_t event, void *data)
{
	switch (event) {
	case RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND:
		gaps_client_discover_res_hdl(data);
		break;
	case RTK_BT_GATTC_EVT_READ_RESULT_IND:
		gaps_client_read_res_hdl(data);
		break;
	// case RTK_BT_GATTC_EVT_WRITE_RESULT_IND:
	//     gaps_client_write_res_hdl(data);
	//     break;
	// case RTK_BT_GATTC_EVT_NOTIFY_IND:
	//     gaps_client_notify_hdl(data);
	//     break;
	default:
		break;
	}

	return RTK_BT_EVT_CB_OK;
}

static uint16_t gaps_client_database_free(uint8_t conn_id)
{
	uint32_t i = 0;
	void *p_char_data;
	
	if (!gaps_database[conn_id]) {
		return RTK_BT_OK;
	}

	for (i = 0; i < GAPS_CHAR_NUM; i++) {
		p_char_data = gaps_database[conn_id]->char_db[i].char_data;
		if (p_char_data != NULL) {
			osif_mem_free(p_char_data);
		}
	}
	osif_mem_free(gaps_database[conn_id]);
	gaps_database[conn_id] = NULL;

	return RTK_BT_OK;
}

uint16_t gaps_client_attach_conn(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	gaps_database[conn_id] = (gaps_client_db_t *)
					osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(gaps_client_db_t));
	memset(gaps_database[conn_id], 0, sizeof(gaps_client_db_t));

#if RTK_BLE_MGR_LIB
	gaps_database[conn_id]->disc_state = DISC_DONE;
	return RTK_BT_OK;
#else
	return rtk_bt_gattc_attach_connect(GAPS_CLIENT_PROFILE_ID, conn_handle);
#endif
}

uint16_t gaps_client_detach_conn(uint16_t conn_handle)
{
	uint16_t ret = 0;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;

	ret = gaps_client_database_free(conn_id);
	if (ret) {
		return ret;
	}

#if RTK_BLE_MGR_LIB
	return RTK_BT_OK;
#else
	return rtk_bt_gattc_detach_connect(GAPS_CLIENT_PROFILE_ID, conn_handle);
#endif
}

uint16_t gaps_client_add(void)
{
#if RTK_BLE_MGR_LIB
	return rtk_bt_gattc_register_profile(GAPS_CLIENT_PROFILE_ID, gaps_uuid);
#else
	return rtk_bt_gattc_register_profile(GAPS_CLIENT_PROFILE_ID);
#endif
}

uint16_t gaps_client_delete(void)
{
	int i = 0;
	uint16_t ret = 0;

	for (i = 0; i < RTK_BLE_GAP_MAX_LINKS; i++) {
		ret = gaps_client_database_free(i);
		if (ret) {
			return ret;
		}
	}

#if RTK_BLE_MGR_LIB
	return rtk_bt_gattc_unregister_profile(GAPS_CLIENT_PROFILE_ID, gaps_uuid);
#else
	return rtk_bt_gattc_unregister_profile(GAPS_CLIENT_PROFILE_ID);
#endif
}

uint16_t gaps_client_char_read(uint16_t conn_handle, gaps_charac_index_t char_index)
{
	rtk_bt_gattc_read_param_t read_param = {0};
	uint8_t conn_id;
	gaps_client_db_t *conn_gaps_db;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return RTK_BT_FAIL;
	
	conn_gaps_db = gaps_database[conn_id];

	if (!conn_gaps_db) {
		return RTK_BT_ERR_NO_ENTRY;
	}

	if (conn_gaps_db->disc_state != DISC_DONE) {
		printf("[APP] GAPS client need discover service before read charac !!!\r\n");
		return RTK_BT_ERR_STATE_INVALID;
	}

	read_param.conn_handle = conn_handle;
	read_param.profile_id = GAPS_CLIENT_PROFILE_ID;
	read_param.type = RTK_BT_GATT_CHAR_READ_BY_HANDLE;
	read_param.by_handle.handle = 
						conn_gaps_db->char_db[char_index].char_val_handle;

	return rtk_bt_gattc_read(&read_param);
}