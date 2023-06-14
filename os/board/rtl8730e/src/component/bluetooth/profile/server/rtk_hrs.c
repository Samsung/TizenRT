/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <osif.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_gatts.h>
#include <rtk_bt_att_defs.h>
#include <bt_api_config.h>
#include <rtk_service_config.h>
#include <rtk_hrs.h>

#define Heart_Rate_Value_Format_UINT8       0
#define Heart_Rate_Value_Format_UINT16      1

#define HEART_RATE_SENSOR_CONTACT_FEAT_SUPPORT      0

#define HEART_RATE_SRV_UUID                     0x180D
#define HEART_RATE_MEASUREMENT_CHAR_UUID        0x2A37
#define HRS_BODY_SENSOR_LOCATION_CHAR_UUID      0x2A38  
#define HEART_RATE_CP_CHAR_UUID                 0x2A39

#define RTK_BT_UUID_HEART_RATE_SRV                  BT_UUID_DECLARE_16(HEART_RATE_SRV_UUID) 
#define RTK_BT_UUID_HEART_RATE_MEASUREMENT_CHAR     BT_UUID_DECLARE_16(HEART_RATE_MEASUREMENT_CHAR_UUID)
#define RTK_BT_UUID_HRS_BODY_SENSOR_LOCATION_CHAR   BT_UUID_DECLARE_16(HRS_BODY_SENSOR_LOCATION_CHAR_UUID)
#define RTK_BT_UUID_HEART_RATE_CP_CHAR              BT_UUID_DECLARE_16(HEART_RATE_CP_CHAR_UUID)

#define HEART_RATE_MEASUREMENT_CHAR_VAL_INDEX       (2)   /* read, notify */
#define HEART_RATE_MEASUREMENT_CHAR_CCCD_INDEX      (3)   /* cccd notify */
#define HRS_BODY_SENSOR_LOCATION_CHAR_VAL_INDEX     (5)   /* read */
#define HEART_RATE_CP_CHAR_VAL_INDEX                (7)   /* write */


static heart_rate_measurement_val_t heart_rate_measurement_val = {
    .flags = {
        .heart_rate_value_format_bit = 1,
        .energy_expended_status_bit = 1,
        .rr_interval_bit = 1,
    },
    .measurement_val = 110,
    .energy_expended = 0,
    .rr_interval = {0x11, 0x11, 0x11, 0x11},
};

static uint8_t hrs_sensor_location_val = 0x33;
static uint8_t hrs_ctrl_point_val = 0x55;
static uint8_t hr_msmnt_cccd_ntf_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};
static uint32_t energy_expended_reset_time = 0;

static rtk_bt_gatt_attr_t hrs_attrs[] = {
    RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_HEART_RATE_SRV),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_HEART_RATE_MEASUREMENT_CHAR,
                               RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_NOTIFY,
                               RTK_BT_GATT_PERM_READ),
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_HRS_BODY_SENSOR_LOCATION_CHAR,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_HEART_RATE_CP_CHAR,
                               RTK_BT_GATT_CHRC_WRITE,
                               RTK_BT_GATT_PERM_WRITE),
};
static struct rtk_bt_gatt_service heart_rate_srv = RTK_BT_GATT_SERVICE(hrs_attrs,HEART_RATE_SRV_ID);

uint16_t heart_rate_srv_set_params(hrs_param_type_t param_type, uint8_t len, void *p_value)
{
    uint16_t ret = RTK_BT_OK;

    switch (param_type) {
    case HRS_HEART_RATE_MEASUREMENT_PARAM_FLAG: 
        if (len != sizeof(uint8_t)) {
            ret = RTK_BT_FAIL;
        } else {
            memcpy(&heart_rate_measurement_val.flags, p_value, sizeof(uint8_t));
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_MEASUREMENT_VALUE:
        if (len != sizeof(uint8_t) && len != sizeof(uint16_t)) {
            ret = RTK_BT_FAIL;
        } else {
            memcpy(&heart_rate_measurement_val.measurement_val, p_value, len);
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_ENERGY_EXPENDED:
        if (len != sizeof(uint16_t)) {
            ret = RTK_BT_FAIL;
        } else {
            memcpy(&heart_rate_measurement_val.energy_expended, p_value, len);
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_RR_INTERVAL:
        if (len > sizeof(uint16_t)*HRS_HEART_RATE_MEASUREMENT_RR_INTERVAL_NUM) {
            ret = RTK_BT_FAIL;
        } else {
            memcpy(&heart_rate_measurement_val.rr_interval, p_value, len);
        }
        break;

    case HRS_BODY_SENSOR_LOCATION_PARAM_VALUE:
        if (len != sizeof(uint8_t)) {
            ret = RTK_BT_FAIL;
        } else {
            memcpy(&hrs_sensor_location_val, p_value, len);
        }
        break;

    default:
        ret = RTK_BT_FAIL;
        break;
    }

    return ret;
}


uint16_t get_heart_rate_val_from_sensor(void)
{
    uint16_t base = 60;
    uint16_t range = 120;
    uint16_t val = 0;

    val = base + rand() % (range + 1);
    return val;
}

/* simulate the accumulated energey since the energy expended reset time */
uint32_t get_heart_rate_energy_expended_val(void)
{
    uint32_t current_time = osif_sys_time_get();
    uint32_t energy_expended_val = 
            (current_time - energy_expended_reset_time)/500;
    return energy_expended_val;
}

void update_heart_rate_measurement_val(void)
{
    heart_rate_measurement_val_t *val = &heart_rate_measurement_val;
    uint32_t energy_expended_val = 0;

    val->measurement_val = get_heart_rate_val_from_sensor();
    energy_expended_val = get_heart_rate_energy_expended_val();
    val->energy_expended = 
            (uint16_t) (energy_expended_val > 0xFFFF ? 0xFFFF: energy_expended_val);
}

uint8_t heart_rate_measurement_value_regroup(uint8_t *dst)
{
    uint8_t actual_len = 0;
    heart_rate_measurement_val_t *src = &heart_rate_measurement_val;
    memcpy(&dst[actual_len], &src->flags, 1);
    actual_len += 1;

    if (Heart_Rate_Value_Format_UINT8 == src->flags.heart_rate_value_format_bit) {
        memcpy(&dst[actual_len], &src->measurement_val, sizeof(uint8_t));
        actual_len += sizeof(uint8_t);
    } else {
        memcpy(&dst[actual_len], &src->measurement_val, sizeof(uint16_t));
        actual_len += sizeof(uint16_t);
    }

    if (src->flags.energy_expended_status_bit) {
        memcpy(&dst[actual_len], &src->energy_expended, sizeof(uint16_t));
        actual_len +=  sizeof(uint16_t);
    }

    if (src->flags.rr_interval_bit) {
        memcpy(&dst[actual_len], &src->rr_interval, sizeof(src->rr_interval));
        actual_len +=  sizeof(src->rr_interval);
    }

    return actual_len;
}

void hrs_read_hdl(void *data)
{
    uint32_t i;
    uint16_t ret = 0;
    rtk_bt_gatts_read_ind_t *p_read_ind = (rtk_bt_gatts_read_ind_t*)data;
    rtk_bt_gatts_read_resp_param_t read_resp = {0};
    uint8_t heart_rate_measurement_value_buf[HRS_HEART_RATE_MEASUREMENT_VALUE_MAX_LEN] = {0};
    uint8_t actual_len = 0;

    read_resp.app_id = p_read_ind->app_id;
    read_resp.conn_handle = p_read_ind->conn_handle;
    read_resp.cid = p_read_ind->cid;
    read_resp.index = p_read_ind->index;

    if (HEART_RATE_MEASUREMENT_CHAR_VAL_INDEX == p_read_ind->index) {
        update_heart_rate_measurement_val();
        actual_len = heart_rate_measurement_value_regroup(
                                            heart_rate_measurement_value_buf);
        read_resp.data = heart_rate_measurement_value_buf;
        read_resp.len = actual_len;
    } else if (HRS_BODY_SENSOR_LOCATION_CHAR_VAL_INDEX == p_read_ind->index) {
        read_resp.data = &hrs_sensor_location_val;
        read_resp.len = sizeof(hrs_sensor_location_val);
    } else {
        printf("[APP] HRS read event unknown index: %d\r\n", p_read_ind->index);
        return;
    }

    for (i = 0; i < 3; i++) {
        ret = rtk_bt_gatts_read_resp(&read_resp);
        if (RTK_BT_OK == ret) {
            printf("[APP] HRS response for client read succeed, index: %d\r\n", p_read_ind->index);
            break;
        }
        osif_delay(50);
    }
    if (i >= 3) {
        printf("[APP] HRS response for client read failed, err: 0x%x\r\n", ret);
    }
}

void hrs_write_hdl(void *data)
{
    uint32_t i;
    rtk_bt_gatts_write_ind_t* p_write_ind = (rtk_bt_gatts_write_ind_t*)data;
    rtk_bt_gatts_write_resp_param_t write_resp = {0};
    write_resp.app_id = p_write_ind->app_id;
    write_resp.conn_handle = p_write_ind->conn_handle;
    write_resp.cid = p_write_ind->cid;
    write_resp.index = p_write_ind->index;
    write_resp.type = p_write_ind->type;

    if (HEART_RATE_CP_CHAR_VAL_INDEX == p_write_ind->index) {
        hrs_ctrl_point_val = *(uint8_t *)p_write_ind->value;

        printf("[APP] HRS write event value: %d, type: %d\r\n", hrs_ctrl_point_val,
                p_write_ind->type);
        if (hrs_ctrl_point_val) {
            heart_rate_measurement_val.energy_expended = 0;
            energy_expended_reset_time = osif_sys_time_get();
        }
    } else {
        printf("[APP] HRS write event unknown index: %d\r\n", p_write_ind->index);
        return;
    }

    if (RTK_BT_GATTS_WRITE_NO_RESP == p_write_ind->type  ||
        RTK_BT_GATTS_WRITE_NO_RESP_SIGNED == p_write_ind->type) {
        return;
    }

    for (i = 0; i < 3; i++) {
        if (RTK_BT_OK == rtk_bt_gatts_write_resp(&write_resp)) {
            printf("[APP] HRS response for client write succeed!\r\n");
            break;
        }
        osif_delay(50);
    }
    if (i >= 3) {
        printf("[APP] HRS response for client write failed!\r\n");
    }

}

void hrs_cccd_update_hdl(void *data)
{
    rtk_bt_gatts_cccd_ind_t * p_cccd_ind = (rtk_bt_gatts_cccd_ind_t*)data;
    uint8_t cccd_ntf = p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(p_cccd_ind->conn_handle, &conn_id) != RTK_BT_OK)
		return;

    if (HEART_RATE_MEASUREMENT_CHAR_CCCD_INDEX == p_cccd_ind->index) {
        hr_msmnt_cccd_ntf_en_map[conn_id] = cccd_ntf;
        printf("[APP] HRS measurement value cccd, notify bit: %d\r\n", cccd_ntf);
    } else {
        printf("[APP] HRS CCCD event unknown index: %d\r\n", p_cccd_ind->index);
    }
}


void heart_rate_srv_callback(uint8_t event, void *data)
{
    switch (event) {
	    case RTK_BT_GATTS_EVT_REGISTER_SERVICE: {
	        rtk_bt_gatts_reg_ind_t *reg_srv_res = (rtk_bt_gatts_reg_ind_t*)data;
	        if (RTK_BT_OK == reg_srv_res->reg_status) {
	            printf("[APP] HRS register service succeed!\r\n");
	        } else {
	            printf("[APP] HRS register service failed: %d\r\n", reg_srv_res->reg_status);
	        }
	        
	        break;
	    } 
		case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND: {
	        rtk_bt_gatts_ntf_and_ind_ind_t* p_ind_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
	        if (RTK_BT_OK == p_ind_ind->err_code) {
	            printf("[APP] HRS indicate succeed!\r\n");
	        } else {
	            printf("[APP] HRS indicate failed \r\n");
	        }

	        break;
    	}
		case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND: {
	        rtk_bt_gatts_ntf_and_ind_ind_t* p_ntf_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
	        if (RTK_BT_OK == p_ntf_ind->err_code) {
	            printf("[APP] HRS notify succeed!\r\n");
	        } else {
	            printf("[APP] HRS notify failed, local error \r\n");
	        }

	        break;
    	}
		
		case RTK_BT_GATTS_EVT_READ_IND:
			hrs_read_hdl(data);
			break;
		
		case RTK_BT_GATTS_EVT_WRITE_IND: 
			hrs_write_hdl(data);
			break;
		
		case RTK_BT_GATTS_EVT_CCCD_IND:
			hrs_cccd_update_hdl(data);
			break;
			
		default:
			break;
    }
}

void heart_rate_measurement_cccd_notify(uint16_t conn_handle)
{
    rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};
    uint8_t heart_rate_measurement_value_buf[HRS_HEART_RATE_MEASUREMENT_VALUE_MAX_LEN] = {0};
    uint8_t actual_len = 0;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

    if (hr_msmnt_cccd_ntf_en_map[conn_id]) {
        update_heart_rate_measurement_val();
        actual_len = heart_rate_measurement_value_regroup(
                                            heart_rate_measurement_value_buf);
        ntf_param.app_id = HEART_RATE_SRV_ID;
        ntf_param.conn_handle = conn_handle;
		// ntf_param.cid = 4;	// choose dynamic created L2CAP channel when EATT is enbaled.
        ntf_param.index = HEART_RATE_MEASUREMENT_CHAR_VAL_INDEX;
        ntf_param.data = heart_rate_measurement_value_buf;
        ntf_param.len = actual_len;
        ntf_param.seq = 0;
        rtk_bt_gatts_notify(&ntf_param);
    }
}


uint16_t heart_rate_srv_add(void)
{
    heart_rate_srv.type = GATT_SERVICE_OVER_BLE;
    heart_rate_srv.server_info = 0;
    heart_rate_srv.user_data = NULL;
    heart_rate_srv.register_status = 0;

    return rtk_bt_gatts_register_service(&heart_rate_srv);
}

void hrs_disconnect(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

    hr_msmnt_cccd_ntf_en_map[conn_id] = 0;
}


void hrs_status_deinit(void)
{
    memset(hr_msmnt_cccd_ntf_en_map, 0, sizeof(hr_msmnt_cccd_ntf_en_map));
}
