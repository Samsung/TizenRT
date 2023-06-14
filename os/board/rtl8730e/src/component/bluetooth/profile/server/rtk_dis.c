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
#include <rtk_service_config.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_dis.h>

#define DEVICE_INFORMATION_SRV_UUID                 0x180A
#define SYSTEM_ID_CHAR_UUID                         0x2A23
#define MODEL_NUMBER_CHAR_UUID                      0x2A24
#define SERIAL_NUMBER_CHAR_UUID                     0x2A25
#define FIRMWARE_REVISION_CHAR_UUID                 0x2A26
#define HARDWARE_REVISION_CHAR_UUID                 0x2A27
#define SOFTWARE_REVISION_CHAR_UUID                 0x2A28
#define MANUFACTURER_NAME_CHAR_UUID                 0x2A29
#define IEEE_CERTIF_DATA_LIST_CHAR_UUID             0x2A2A
#define PNP_ID_CHAR_UUID                            0x2A50

#define RTK_BT_UUID_DEVICE_INFORMATION_SERVICE      BT_UUID_DECLARE_16(DEVICE_INFORMATION_SRV_UUID)
#define RTK_BT_UUID_CHAR_SYSTEM_ID                  BT_UUID_DECLARE_16(SYSTEM_ID_CHAR_UUID)
#define RTK_BT_UUID_CHAR_MODEL_NUMBER               BT_UUID_DECLARE_16(MODEL_NUMBER_CHAR_UUID)
#define RTK_BT_UUID_CHAR_SERIAL_NUMBER              BT_UUID_DECLARE_16(SERIAL_NUMBER_CHAR_UUID)
#define RTK_BT_UUID_CHAR_FIRMWARE_REVISION          BT_UUID_DECLARE_16(FIRMWARE_REVISION_CHAR_UUID)
#define RTK_BT_UUID_CHAR_HARDWARE_REVISION          BT_UUID_DECLARE_16(HARDWARE_REVISION_CHAR_UUID)
#define RTK_BT_UUID_CHAR_SOFTWARE_REVISION          BT_UUID_DECLARE_16(SOFTWARE_REVISION_CHAR_UUID)
#define RTK_BT_UUID_CHAR_MANUFACTURER_NAME          BT_UUID_DECLARE_16(MANUFACTURER_NAME_CHAR_UUID)
#define RTK_BT_UUID_CHAR_IEEE_CERTIF_DATA_LIST      BT_UUID_DECLARE_16(IEEE_CERTIF_DATA_LIST_CHAR_UUID)
#define RTK_BT_UUID_CHAR_PNP_ID                     BT_UUID_DECLARE_16(PNP_ID_CHAR_UUID)

#define INCLUDE_SRVICE_NUM                       1
#define MANUFACTURER_NAME_CHAR_INDEX            (2+INCLUDE_SRVICE_NUM)
#define MODEL_NUMBER_CHAR_INDEX                 (4+INCLUDE_SRVICE_NUM)
#define SERIAL_NUMBER_CHAR_INDEX                (6+INCLUDE_SRVICE_NUM)
#define HARDWARE_REVISION_CHAR_INDEX            (8+INCLUDE_SRVICE_NUM)
#define FIRMWARE_REVISION_CHAR_INDEX            (10+INCLUDE_SRVICE_NUM)
#define SOFTWARE_REVISION_CHAR_INDEX            (12+INCLUDE_SRVICE_NUM)
#define SYSTEM_ID_CHAR_INDEX                    (14+INCLUDE_SRVICE_NUM)
#define IEEE_CERTIF_DATA_LIST_CHAR_INDEX        (16+INCLUDE_SRVICE_NUM)
#define PNP_ID_CHAR_INDEX                       (18+INCLUDE_SRVICE_NUM)

#define MANUFACTURER_NAME_CHAR_INDEX_2            (20+INCLUDE_SRVICE_NUM)
#define MODEL_NUMBER_CHAR_INDEX_2                 (22+INCLUDE_SRVICE_NUM)
#define SERIAL_NUMBER_CHAR_INDEX_2                (24+INCLUDE_SRVICE_NUM)

#define MANUFACTURER_NAME_CHAR_INDEX_3            (26+INCLUDE_SRVICE_NUM)
#define MANUFACTURER_NAME_CHAR_INDEX_4            (28+INCLUDE_SRVICE_NUM)

#define DIS_IEEE_11073_BODY_EXP                 254

static uint8_t dis_manufacturer_name[20] = {'R', 'e', 'a', 'l', 't', 'e', 'k', '\0'};
static uint8_t dis_manufacturer_name_2[20] = {'T', 'e', 'k', 'R', 'e', 'a', 'l','\0'};
static uint8_t dis_manufacturer_name_3[20] = {'R', 'e', 'a', 'l', 's', 'i', 'l','\0'};
static uint8_t dis_manufacturer_name_4[20] = {'S', 'i', 'l', 'R', 'e', 'a', 'l','\0'};

static uint8_t dis_model_number[20] = {'8', '8', '8', '5', '5', '5', '\0'};
static uint8_t dis_model_number_2[20] = {'5', '5', '5', '8', '8', '8', '\0'};

static uint8_t dis_serial_number[20] = {'6', '5', '4', '3', '2', '1', '\0'};
static uint8_t dis_serial_number_2[20] = {'1', '2', '3', '4', '5', '6', '\0'};

static uint8_t dis_hardware_rev[20] = {'v', '1', '.', '1', '\0'};
static uint8_t dis_firmware_rev[20] = {'v', '2', '.', '1', '\0'};
static uint8_t dis_software_rev[20] = {'v', '3', '.', '2', '\0'};
static uint8_t dis_sys_id[8] = {0, 1, 2, 0, 0, 3, 4, 5};
static uint8_t dis_ieee_data_list[30] = {
    DIS_IEEE_11073_BODY_EXP,      // Authoritative body type
    0x00,                         // Authoritative body structure type
    // Authoritative body data follows below:
    'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l'
};
static uint8_t dis_ieee_data_list_len = 14;
static uint8_t dis_pnp_id[7] = {
    1,
    0x5D, 0x00,
    0x00, 0x00,
    0x00, 0x01,
};

extern struct rtk_bt_gatt_service battery_srv;


static rtk_bt_gatt_attr_t device_info_attrs[] = {
    RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_DEVICE_INFORMATION_SERVICE),
    RTK_BT_GATT_INCLUDE_SERVICE(&battery_srv),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_MANUFACTURER_NAME,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_MODEL_NUMBER,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_SERIAL_NUMBER,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_HARDWARE_REVISION,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_FIRMWARE_REVISION,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_SOFTWARE_REVISION,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_SYSTEM_ID,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_IEEE_CERTIF_DATA_LIST,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_PNP_ID,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    /* test for multiple same characteristic */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_MANUFACTURER_NAME,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_MODEL_NUMBER,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_SERIAL_NUMBER,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    /* test for more than 2 same characteristic */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_MANUFACTURER_NAME,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),

    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_CHAR_MANUFACTURER_NAME,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ),
};
static struct rtk_bt_gatt_service device_info_srv = RTK_BT_GATT_SERVICE(device_info_attrs,DEVICE_INFO_SRV_ID);

uint16_t dis_set_params(dis_param_type_t param_type, uint8_t len, void *p_value)
{
    uint16_t ret = RTK_BT_OK;

    switch (param_type) {
    case DIS_PARAM_MANUFACTURER_NAME:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_manufacturer_name, p_value, len);
        }
        break;
    case DIS_PARAM_MODEL_NUMBER:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_model_number, p_value, len);
        }
        break;
    case DIS_PARAM_SERIAL_NUMBER:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_serial_number, p_value, len);
        }
        break;

    case DIS_PARAM_HARDWARE_REVISION:
        if (len > 8) {
            len = 8;
        } else {
            memcpy(dis_sys_id, p_value, len);
        }
        break;

    case DIS_PARAM_FIRMWARE_REVISION:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_firmware_rev, p_value, len);
        }
        break;

    case DIS_PARAM_SOFTWARE_REVISION:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_software_rev, p_value, len);
        }
        break;

    case DIS_PARAM_SYSTEM_ID:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_hardware_rev, p_value, len);
        }
        break;

    case DIS_PARAM_IEEE_DATA_LIST:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_ieee_data_list, p_value, len);
        }
        break;

    case DIS_PARAM_PNP_ID:
        if (len > 20) {
            len = 20;
        } else {
            memcpy(dis_pnp_id, p_value, len);
        }
        break;

    default:
        ret = RTK_BT_FAIL;
        break;
    }

    return ret;
}

void dis_read_hdl(void *data)
{
    int i = 0;
    uint16_t ret = 0;
    rtk_bt_gatts_read_ind_t *evt_read = (rtk_bt_gatts_read_ind_t*)data;
    rtk_bt_gatts_read_resp_param_t read_resp = {0};
    read_resp.app_id = evt_read->app_id;
    read_resp.conn_handle = evt_read->conn_handle;
    read_resp.index = evt_read->index;
    read_resp.cid = evt_read->cid;

    switch (evt_read->index) {
    case MANUFACTURER_NAME_CHAR_INDEX:
        read_resp.data = dis_manufacturer_name;
        read_resp.len = strlen((const char*)dis_manufacturer_name) ;//+ 1;
        break;

    case MODEL_NUMBER_CHAR_INDEX:
        read_resp.data = dis_model_number;
        read_resp.len = strlen((const char*)dis_model_number) ;//+ 1;
        break;

    case SERIAL_NUMBER_CHAR_INDEX:
        read_resp.data = dis_serial_number;
        read_resp.len = strlen((const char*)dis_serial_number) ;//+ 1;
        break;

    case HARDWARE_REVISION_CHAR_INDEX:
        read_resp.data = dis_hardware_rev;
        read_resp.len = strlen((const char*)dis_hardware_rev) ;//+ 1;
        break;

    case FIRMWARE_REVISION_CHAR_INDEX:
        read_resp.data = dis_firmware_rev;
        read_resp.len = strlen((const char*)dis_firmware_rev) ;//+ 1;
        break;

    case SOFTWARE_REVISION_CHAR_INDEX:
        read_resp.data = dis_software_rev;
        read_resp.len = strlen((const char*)dis_software_rev) ;//+ 1;
        break;

    case SYSTEM_ID_CHAR_INDEX:
        read_resp.data = dis_sys_id;
        read_resp.len = sizeof(dis_sys_id);
        break;

    case IEEE_CERTIF_DATA_LIST_CHAR_INDEX:
        read_resp.data = dis_ieee_data_list;
        read_resp.len = dis_ieee_data_list_len;
        break;

    case PNP_ID_CHAR_INDEX:
        read_resp.data = dis_pnp_id;
        read_resp.len = sizeof(dis_pnp_id);
        break;

    /* test for multiple same charac */
    case MANUFACTURER_NAME_CHAR_INDEX_2:
        read_resp.data = dis_manufacturer_name_2;
        read_resp.len = strlen((const char*)dis_manufacturer_name_2) ;//+ 1;
        break;

    case MODEL_NUMBER_CHAR_INDEX_2:
        read_resp.data = dis_model_number_2;
        read_resp.len = strlen((const char*)dis_model_number_2) ;//+ 1;
        break;

    case SERIAL_NUMBER_CHAR_INDEX_2:
        read_resp.data = dis_serial_number_2;
        read_resp.len = strlen((const char*)dis_serial_number_2) ;//+ 1;
        break;

    case MANUFACTURER_NAME_CHAR_INDEX_3:
        read_resp.data = dis_manufacturer_name_3;
        read_resp.len = strlen((const char*)dis_manufacturer_name_3) ;//+ 1;
        break;

    case MANUFACTURER_NAME_CHAR_INDEX_4:
        read_resp.data = dis_manufacturer_name_4;
        read_resp.len = strlen((const char*)dis_manufacturer_name_4) ;//+ 1;
        break;

    default:
        printf("[APP] DIS read event unknown index: %d\r\n", evt_read->index);
        return;
    }

    for (i = 0; i < 3; i++) {
        ret = rtk_bt_gatts_read_resp(&read_resp);
        if (RTK_BT_OK == ret) {
            printf("[APP] DIS response for client read succeed!\r\n");
            break;
        }
        osif_delay(50);
    }
    if (i >= 3) {
        printf("[APP] DIS response for client read failed, err: 0x%x\r\n", ret);
    }
}

void device_info_srv_callback(uint8_t event, void *data)
{
    switch (event)
    {
        case RTK_BT_GATTS_EVT_REGISTER_SERVICE: {
            rtk_bt_gatts_reg_ind_t* reg_srv_res = (rtk_bt_gatts_reg_ind_t*)data;
            if (RTK_BT_OK == reg_srv_res->reg_status)
                printf("[APP] DIS register service succeed!\r\n");
            else
                printf("[APP] DIS register service failed, err: 0x%x\r\n", reg_srv_res->reg_status);
            break;
        }

        case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND: {
            rtk_bt_gatts_ntf_and_ind_ind_t* p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
            if (RTK_BT_OK == p_ind->err_code)
                printf("[APP] DIS indicate succeed!\r\n");
            else
                printf("[APP] DIS indicate failed, err: 0x%x\r\n", p_ind->err_code);
            break;
        }

        case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND: {
            rtk_bt_gatts_ntf_and_ind_ind_t* p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
            if (RTK_BT_OK == p_ind->err_code)
                printf("[APP] DIS notify succeed!\r\n");
            else
                printf("[APP] DIS notify failed, err: 0x%x\r\n", p_ind->err_code);
            break;
        }

        case RTK_BT_GATTS_EVT_READ_IND: {
            dis_read_hdl(data);
            break;
        }

        case RTK_BT_GATTS_EVT_WRITE_IND: {
            rtk_bt_gatts_write_ind_t* p_write_ind = (rtk_bt_gatts_write_ind_t*)data;
            printf("[APP] DIS write event, while no write attr, so no response, index: %d\r\n", p_write_ind->index);
            break;
        }

        case RTK_BT_GATTS_EVT_CCCD_IND: {
            rtk_bt_gatts_cccd_ind_t* p_cccd_ind = (rtk_bt_gatts_cccd_ind_t*)data;
            printf("[APP] DIS CCCD event unknown index: %d\r\n", p_cccd_ind->index);
            break;
        }

        default:
            printf("[APP] DIS NO corresponding event: %d\r\n", event);
            break;
    }
}



uint16_t device_info_srv_add(void)
{
    device_info_srv.type = GATT_SERVICE_OVER_BLE;
    device_info_srv.server_info = 0;
    device_info_srv.user_data = NULL;
    device_info_srv.register_status = 0;

    return rtk_bt_gatts_register_service(&device_info_srv);
}

