/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>

#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_service_config.h>
#include <rtk_ias.h>

#define IMMEDIATE_ALERT_SRV_UUID            0x1802
#define ALERT_LEVEL_CHAR_UUID               0x2A06

#define RTK_BT_UUID_IMMEDIATE_ALERT_SRV     BT_UUID_DECLARE_16(IMMEDIATE_ALERT_SRV_UUID)
#define RTK_BT_UUID_ALERT_LEVEL_CHAR        BT_UUID_DECLARE_16(ALERT_LEVEL_CHAR_UUID)

#define ALERT_LEVEL_CHAR_VAL_INDEX          2   /* write */

static rtk_bt_gatt_attr_t immediate_alert_attrs[] = {
    /* Primary Service: immediate_alert */
    RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_IMMEDIATE_ALERT_SRV),

    /* Characteristic: alert level */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_ALERT_LEVEL_CHAR,
                               RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                               RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),
};

static struct rtk_bt_gatt_service immediate_alert_srv = RTK_BT_GATT_SERVICE(immediate_alert_attrs,IMMEDIATE_ALERT_SRV_ID);


void immediate_alert_srv_callback(uint8_t event, void *data)
{
    switch (event) {
        case RTK_BT_GATTS_EVT_REGISTER_SERVICE: {
            rtk_bt_gatts_reg_ind_t* reg_srv_res = (rtk_bt_gatts_reg_ind_t*)data;
            if (RTK_BT_OK == reg_srv_res->reg_status) {
                printf("[APP] IAS register service succeed!\r\n");
            } else {
                printf("[APP] IAS register service failed, err: 0x%x\r\n", reg_srv_res->reg_status);
            }

            break;
        }
        
        case RTK_BT_GATTS_EVT_WRITE_IND: {
            rtk_bt_gatts_write_ind_t* p_write_ind = (rtk_bt_gatts_write_ind_t*)data;
            uint8_t *value = (uint8_t *)(p_write_ind->value);
            if (ALERT_LEVEL_CHAR_VAL_INDEX == p_write_ind->index) {
                printf("[APP] IAS write by remote, value: %d, type: %d\r\n", 
                                                        *value, p_write_ind->type);
            }
        
            break;
        }
        
        default:
            break;
    }

}


uint16_t immediate_alert_srv_add(void)
{
    immediate_alert_srv.type = GATT_SERVICE_OVER_BLE;
    immediate_alert_srv.server_info = 0;
    immediate_alert_srv.user_data = NULL;
    immediate_alert_srv.register_status = 0;

    return rtk_bt_gatts_register_service(&immediate_alert_srv);
}

