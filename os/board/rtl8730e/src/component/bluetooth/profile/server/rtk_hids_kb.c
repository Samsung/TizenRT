/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <osif.h>

#include <bt_api_config.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_le_gap.h>
#include <rtk_bt_att_defs.h>
#include <rtk_bt_gatts.h>
#include <rtk_service_config.h>
#include <rtk_hids_kb.h>

#define HID_SRV_UUID                            0x1812
#define PROTOCOL_MODE_CHAR_UUID                 0x2A4E
#define REPORT_CHAR_UUID                        0x2A4D
#define REPORT_MAP_CHAR_UUID                    0x2A4B
#define BOOT_KB_IN_REPORT_CHAR_UUID             0x2A22
#define BOOT_KB_OUT_REPORT_CHAR_UUID            0x2A32
#define HID_INFO_CHAR_UUID                      0x2A4A
#define HID_CONTROL_POINT_CHAR_UUID             0x2A4C
#define EXTERNAL_REPORT_REFERENCE_DSCRP_UUID    0x2907
#define REPORT_REFERENCE_DSCRP_UUID             0x2908

#define RTK_BT_UUID_HID_SRV                     BT_UUID_DECLARE_16(HID_SRV_UUID)
#define RTK_BT_UUID_PROTOCOL_MODE_CHAR          BT_UUID_DECLARE_16(PROTOCOL_MODE_CHAR_UUID)
#define RTK_BT_UUID_REPORT_CHAR                 BT_UUID_DECLARE_16(REPORT_CHAR_UUID)
#define RTK_BT_UUID_REPORT_MAP_CHAR             BT_UUID_DECLARE_16(REPORT_MAP_CHAR_UUID)
#define RTK_BT_UUID_BOOT_KB_IN_REPORT_CHAR      BT_UUID_DECLARE_16(BOOT_KB_IN_REPORT_CHAR_UUID)
#define RTK_BT_UUID_BOOT_KB_OUT_REPORT_CHAR     BT_UUID_DECLARE_16(BOOT_KB_OUT_REPORT_CHAR_UUID)
#define RTK_BT_UUID_HID_INFO_CHAR               BT_UUID_DECLARE_16(HID_INFO_CHAR_UUID)
#define RTK_BT_UUID_HID_CONTROL_POINT_CHAR      BT_UUID_DECLARE_16(HID_CONTROL_POINT_CHAR_UUID)
#define RTK_BT_UUID_EXT_REPORT_REF_DSCRP        BT_UUID_DECLARE_16(EXTERNAL_REPORT_REFERENCE_DSCRP_UUID)
#define RTK_BT_UUID_REPORT_REFERENCE_DSCRP      BT_UUID_DECLARE_16(REPORT_REFERENCE_DSCRP_UUID)

/* Report ID for General Keyboard, change: 0x03 to 0x01 */
#define HOGP_KB_REPORT_ID                       0x03
#define HOGP_MM_REPORT_ID                       0x04

#define PROTOCOL_MODE_CHAR_VAL_INDEX            2   /* read, wr_no_rsp */
#define REPORT_INPUT_CHAR_VAL_INDEX             4   /* read, write, notify */
#define REPORT_INPUT_CHAR_CCCD_INDEX            5   /* cccd notify */
#define REPORT_OUTPUT_CHAR_VAL_INDEX            8   /* read, write, wr_no_rsp */
#define REPORT_FEATURE_CHAR_VAL_INDEX           11  /* read, write */
#define REPORT_MAP_CHAR_VAL_INDEX               14  /* read */
#define EXT_REPORT_REF_DSCRP_INDEX              15  /* ref discriptor */
#define BOOT_KB_IN_REPORT_CHAR_VAL_INDEX        17  /* read, write, notify */
#define BOOT_KB_IN_REPORT_CHAR_CCCD_INDEX       18  /* cccd notify */
#define BOOT_KB_OUT_REPORT_CHAR_VAL_INDEX       20  /* read, write, wr_no_rsp */
#define HID_INFO_CHAR_VAL_INDEX                 22  /* read */
#define HID_CONTROL_POINT_CHAR_VAL_INDEX        24  /* wr_no_rsp */
#define MM_REPORT_INPUT_CHAR_VAL_INDEX          26  /* read, write, notify */
#define MM_REPORT_INPUT_CHAR_CCCD_INDEX         27  /* cccd notify */

T_HID_INFO hid_info = {0, 0,0x0100};
T_HID_PROTOCOL_MODE hid_protocol_mode = BOOT_PROTOCOL_MODE;
uint8_t hid_suspend_mode = 0;
uint16_t external_report_refer = 0;

uint16_t hid_input_kb_report = HID_INPUT_TYPE << 8 | HOGP_KB_REPORT_ID;
uint16_t hid_output_kb_report = HID_OUTPUT_TYPE << 8 | HOGP_KB_REPORT_ID;
uint16_t hid_feature_kb_report = HID_FEATURE_TYPE << 8 | HOGP_KB_REPORT_ID;
uint16_t hid_input_mm_report = HID_INPUT_TYPE << 8 | HOGP_MM_REPORT_ID;

static uint8_t report_input_cccd_ntf_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};
static uint8_t boot_kb_in_cccd_ntf_en_map[RTK_BLE_GAP_MAX_LINKS] = {0};

const uint8_t hids_report_descriptor[] =
{
    0x05, 0x01,     /* USAGE_PAGE       (Generic Desktop) */
    0x09, 0x06,     /* USAGE            (Keyboard) */
    0xa1, 0x01,     /* COLLECTION       (Application) */
    0x85, HOGP_KB_REPORT_ID,    /* REPORT_ID        (3) */
    0x05, 0x07,     /* USAGE_PAGE       (Keyboard) */
    0x19, 0xe0,     /* USAGE_MINIMUM    (Keyboard Left Control) */
    0x29, 0xe7,     /* USAGE_MAXIMUM    (Keyboard Right GUI) */
    0x15, 0x00,     /* LOGICAL_MINIMUM  (0) */
    0x25, 0x01,     /* LOGICAL_MAXIMUM  (1) */
    0x75, 0x01,     /* REPORT_SIZE      (1) */
    0x95, 0x08,     /* REPORT_COUNT     (8) */
    0x81, 0x02,     /* INPUT            (Data,Var,Abs) */
    0x95, 0x01,     /* REPORT_COUNT     (1) */
    0x75, 0x08,     /* REPORT_SIZE      (8) */
    0x81, 0x01,     /* INPUT            (Cnst,Var,Abs) */
    0x95, 0x05,     /* REPORT_COUNT     (5) */
    0x75, 0x01,     /* REPORT_SIZE      (1) */
    0x05, 0x08,     /* USAGE_PAGE       (LEDs) */
    0x19, 0x01,     /* USAGE_MINIMUM    (Num Lock) */
    0x29, 0x05,     /* USAGE_MAXIMUM    (Kana) */
    0x91, 0x02,     /* OUTPUT           (Data,Var,Abs) */
    0x95, 0x01,     /* REPORT_COUNT     (1) */
    0x75, 0x03,     /* REPORT_SIZE      (3) */
    0x91, 0x01,     /* OUTPUT           (Cnst,Var,Abs) */
    0x95, 0x06,     /* REPORT_COUNT     (6) */
    0x75, 0x08,     /* REPORT_SIZE      (8) */
    0x15, 0x00,     /* LOGICAL_MINIMUM  (0) */
    0x25, 0xa4,     /* LOGICAL_MAXIMUM  (164) */    /* Can be 255 */
    0x05, 0x07,     /* USAGE_PAGE       (Keyboard) */
    0x19, 0x00,     /* USAGE_MINIMUM    (Reserved-no event indicated) */
    0x29, 0xa4,     /* USAGE_MAXIMUM    (Keyboard Application) */       /* Can be 255 */
    0x81, 0x00,     /* INPUT            (Data,Ary,Abs) */
    0xc0,           /* END_COLLECTION */
#ifdef MULTIMEDIA_KEYBOARD
    0x05, 0x0c,     /* USAGE_PAGE       (Consumer) */
    0x09, 0x01,     /* USAGE            (Consumer Control) */
    0xa1, 0x01,     /* COLLECTION       (Application) */
    0x85, HOGP_MM_REPORT_ID,     /* REPORT_ID        (4) */
    0x15, 0x00,     /* LOGICAL_MINIMUM  (0) */
    0x25, 0x01,     /* LOGICAL_MAXIMUM  (1) */
    0x75, 0x01,     /* REPORT_SIZE      (1) */
    0x95, 0x18,     /* REPORT_COUNT     (24) */
    0x09, 0xb5,     /* USAGE            (Scan Next Track) */
    0x09, 0xb6,     /* USAGE            (Scan Previous Track) */
    0x09, 0xb7,     /* USAGE            (Stop) */
    0x09, 0xcd,     /* USAGE            (Play/Pause) */
    0x09, 0xe2,     /* USAGE            (Mute) */
    0x09, 0xe5,     /* USAGE            (Bass Boost) */
    0x09, 0xe7,     /* USAGE            (Loudness) */
    0x09, 0xe9,     /* USAGE            (Volume Increment) */
    0x09, 0xea,     /* USAGE            (Volume Decrement) */
    0x0a, 0x52, 0x01,     /* USAGE      (Bass Increment) */
    0x0a, 0x53, 0x01,     /* USAGE      (Bass Decrement) */
    0x0a, 0x54, 0x01,     /* USAGE      (Treble Increment) */
    0x0a, 0x55, 0x01,     /* USAGE      (Treble Decrement) */
    0x0a, 0x83, 0x01,     /* USAGE      (AL Consumer Control Configuration) */
    0x0a, 0x8a, 0x01,     /* USAGE      (AL Email Reader) */
    0x0a, 0x92, 0x01,     /* USAGE      (AL Calculator) */
    0x0a, 0x94, 0x01,     /* USAGE      (AL Local Machine Browser) */
    0x0a, 0x21, 0x02,     /* USAGE      (AC Search) */
    0x0a, 0x23, 0x02,     /* USAGE      (AC Home) */
    0x0a, 0x24, 0x02,     /* USAGE      (AC Back) */
    0x0a, 0x25, 0x02,     /* USAGE      (AC Forward) */
    0x0a, 0x26, 0x02,     /* USAGE      (AC Stop) */
    0x0a, 0x27, 0x02,     /* USAGE      (AC Refresh) */
    0x0a, 0x2a, 0x02,     /* USAGE      (AC Bookmarks) */
    0x81, 0x02,     /* INPUT            (Data,Var,Abs) */
    0xc0            /* END_COLLECTION */
#endif
};

static rtk_bt_gatt_attr_t hid_srv_attrs[] = {
    /* 0 */
    RTK_BT_GATT_PRIMARY_SERVICE(RTK_BT_UUID_HID_SRV),

    /* 1, 2 */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_PROTOCOL_MODE_CHAR,
                    RTK_BT_GATT_CHRC_READ |RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                    RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_READ_AUTHEN),

    /* 3, 4 input report */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_REPORT_CHAR,
        RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_WRITE | RTK_BT_GATT_CHRC_NOTIFY,
                    RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),
    /* 5 */
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),
    /* 6 */
    RTK_BT_GATT_DESCRIPTOR(RTK_BT_UUID_REPORT_REFERENCE_DSCRP,
        RTK_BT_GATT_PERM_READ_AUTHEN, &hid_input_kb_report, 2, RTK_BT_GATT_INTERNAL),

    /* 7, 8 output report */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_REPORT_CHAR,
        RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_WRITE | RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                            RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),
    /* 9 */
    RTK_BT_GATT_DESCRIPTOR(RTK_BT_UUID_REPORT_REFERENCE_DSCRP,
            RTK_BT_GATT_PERM_READ_AUTHEN, &hid_output_kb_report, 2, RTK_BT_GATT_INTERNAL),

    /* 10, 11 feature report */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_REPORT_CHAR,
                               RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_WRITE,
                    RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),
    /* 12 */
    RTK_BT_GATT_DESCRIPTOR(RTK_BT_UUID_REPORT_REFERENCE_DSCRP,
            RTK_BT_GATT_PERM_READ_AUTHEN, &hid_feature_kb_report, 2, RTK_BT_GATT_INTERNAL),

    /* 13, 14 */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_REPORT_MAP_CHAR,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ_AUTHEN),
    /* 15 */
    RTK_BT_GATT_DESCRIPTOR(RTK_BT_UUID_EXT_REPORT_REF_DSCRP,
                           RTK_BT_GATT_PERM_READ_AUTHEN, NULL, 0, RTK_BT_GATT_APP),

    /* 16, 17 */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_BOOT_KB_IN_REPORT_CHAR,
            RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_WRITE | RTK_BT_GATT_CHRC_NOTIFY,
                        RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),
    /* 18 */
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),

    /* 19, 20 */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_BOOT_KB_OUT_REPORT_CHAR,
        RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_WRITE | RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                                RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),

    // /* 21, 22 */
    // RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_BOOT_MS_IN_REPORT_CHAR,
    //                                         RTK_BT_GATT_CHRC_READ,
    //                                 RTK_BT_GATT_PERM_READ_AUTHEN),
    // /* 23 */
    // RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),

    /* 21, 22 */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_HID_INFO_CHAR,
                               RTK_BT_GATT_CHRC_READ,
                               RTK_BT_GATT_PERM_READ_AUTHEN),

    /* 23, 24 */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_HID_CONTROL_POINT_CHAR,
                               RTK_BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                               RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),

    /* 25, 26 multimedia keyboard Input */
    RTK_BT_GATT_CHARACTERISTIC(RTK_BT_UUID_REPORT_CHAR,
        RTK_BT_GATT_CHRC_READ | RTK_BT_GATT_CHRC_WRITE | RTK_BT_GATT_CHRC_NOTIFY,
                        RTK_BT_GATT_PERM_READ_AUTHEN | RTK_BT_GATT_PERM_WRITE_AUTHEN),
    /* 27 */
    RTK_BT_GATT_CCC(RTK_BT_GATT_PERM_READ | RTK_BT_GATT_PERM_WRITE),
    /* 28 */
    RTK_BT_GATT_DESCRIPTOR(RTK_BT_UUID_REPORT_REFERENCE_DSCRP,
            RTK_BT_GATT_PERM_READ_AUTHEN, &hid_input_mm_report, 2, RTK_BT_GATT_INTERNAL),
};
static struct rtk_bt_gatt_service hid_srv = RTK_BT_GATT_SERVICE(hid_srv_attrs,HID_SRV_ID);

uint16_t hids_set_parameter(T_HIDS_PARAM_TYPE param_type, uint8_t length, void *value_ptr)
{
    uint16_t ret = RTK_BT_OK;

    switch (param_type) {
    case HID_PROTOCOL_MODE:
        hid_protocol_mode = (T_HID_PROTOCOL_MODE) * ((uint8_t *)value_ptr);
        break;

    case HID_REPORT_INPUT:
        break;

    case HID_REPORT_OUTPUT:
        break;

    case HID_REPORT_FEATURE:
        break;

    case HID_REPORT_MAP:
        break;

    case HID_EXTERNAL_REPORT_REFER:
        external_report_refer = *(uint16_t *)value_ptr;
        break;

    case HID_BOOT_KB_IN_REPORT:
        break;

    case HID_BOOT_KB_OUT_REPORT:
        break;

    case HID_INFO:
        memcpy((void *)&hid_info, value_ptr, length);
        break;

    case HID_CONTROL_POINT:
        hid_suspend_mode = *((uint8_t *)value_ptr);
        break;

    default:
        ret = RTK_BT_FAIL;
        break;
    }
    return ret;
}

static void hids_read_hdl(void *data)
{
    uint32_t i;
    uint16_t ret = 0;
    rtk_bt_gatts_read_ind_t* p_read_ind = (rtk_bt_gatts_read_ind_t*)data;
    rtk_bt_gatts_read_resp_param_t read_resp = {0};
    read_resp.app_id = p_read_ind->app_id;
    read_resp.conn_handle = p_read_ind->conn_handle;
    read_resp.cid = p_read_ind->cid;
    read_resp.index = p_read_ind->index;

    switch (p_read_ind->index) {
    case PROTOCOL_MODE_CHAR_VAL_INDEX:
        read_resp.data = (void *)&hid_protocol_mode;
        read_resp.len = sizeof(hid_protocol_mode);
        break;

    case REPORT_INPUT_CHAR_VAL_INDEX:
        break;

    case REPORT_OUTPUT_CHAR_VAL_INDEX:
        break;

    case REPORT_FEATURE_CHAR_VAL_INDEX:
        break;

    case REPORT_MAP_CHAR_VAL_INDEX: {
        uint16_t total_len = sizeof(hids_report_descriptor);
        uint16_t offset = p_read_ind->offset;
        uint16_t actual_len = total_len - p_read_ind->offset;

        // if (total_len - evt_read->offset < (mtu_size -1)) {
        //     actual_len = total_len - evt_read->offset;
        // } else {
        //     actual_len = mtu_size - 1;
        // }
        read_resp.data = (void *)((uint8_t *)&hids_report_descriptor[0] + offset);
        read_resp.len = actual_len;
        break;
    }

    case EXT_REPORT_REF_DSCRP_INDEX:
        read_resp.data = (void *)&external_report_refer;
        read_resp.len = sizeof(external_report_refer);
        break;

    case BOOT_KB_IN_REPORT_CHAR_VAL_INDEX:
        break;

    case BOOT_KB_OUT_REPORT_CHAR_VAL_INDEX:
        break;


    case HID_INFO_CHAR_VAL_INDEX:
        read_resp.data = (uint8_t *)&hid_info;
        read_resp.len = sizeof(hid_info);
        break;

    case HID_CONTROL_POINT_CHAR_VAL_INDEX:
        break;

    default:
        break;
    }

    for (i = 0; i < 3; i++) {
        ret = rtk_bt_gatts_read_resp(&read_resp);
        if (RTK_BT_OK == ret) {
            printf("[APP] HIDS response for client read succeed, index: %d\r\n", read_resp.index);
            break;
        }
        osif_delay(50);
    }
    if (i >= 3)
        printf("[APP] HIDS response for client read failed, err: 0x%x\r\n", ret);
}


static void hids_write_hdl(void *data)
{
    uint32_t i;
    uint16_t ret = 0;
    rtk_bt_gatts_write_ind_t *p_write_ind = (rtk_bt_gatts_write_ind_t*)data;
    rtk_bt_gatts_write_resp_param_t write_resp = {0};
    uint16_t index = p_write_ind->index;
    void *p_value = p_write_ind->value;
    uint16_t len = p_write_ind->len;

    switch (index) {
    case PROTOCOL_MODE_CHAR_VAL_INDEX:
        hids_set_parameter(HID_PROTOCOL_MODE, len, p_value);
        break;

    case REPORT_INPUT_CHAR_VAL_INDEX:
        break;

    case REPORT_OUTPUT_CHAR_VAL_INDEX:
        break;

    case REPORT_FEATURE_CHAR_VAL_INDEX:
        break;

    case REPORT_MAP_CHAR_VAL_INDEX:
        break;

    case BOOT_KB_IN_REPORT_CHAR_VAL_INDEX:
        break;

    case BOOT_KB_OUT_REPORT_CHAR_VAL_INDEX:
        break;

    case HID_INFO_CHAR_VAL_INDEX:
        break;

    case HID_CONTROL_POINT_CHAR_VAL_INDEX:
        break;

    default:
        break;
    }

    printf("[APP] HIDS write event data(index: %d): ", index);
    for (i = 0; i < p_write_ind->len; i++)
    {
        if (0 == i%16)
            printf("\r\n");
        printf("%02x ", *(p_write_ind->value+i));
    }
    printf("\r\n");

    if (RTK_BT_GATTS_WRITE_NO_RESP == p_write_ind->type  ||
        RTK_BT_GATTS_WRITE_NO_RESP_SIGNED == p_write_ind->type) {
        return;
    }
    
    write_resp.app_id = p_write_ind->app_id;
    write_resp.conn_handle = p_write_ind->conn_handle;
    write_resp.index = p_write_ind->index;
    write_resp.cid = p_write_ind->cid;
    write_resp.type = p_write_ind->type;

    for (i = 0; i < 3; i++) {
        ret = rtk_bt_gatts_write_resp(&write_resp);
        if (RTK_BT_OK == ret) {
            printf("[APP] HIDS response for client write succeed, index: %d\r\n", index);
            break;
        }
        osif_delay(50);
    }
    if (i >= 3)
        printf("[APP] HIDS response for client write failed, err: 0x%x\r\n", ret);
}

static void hids_cccd_update_hdl(void *data)
{
    rtk_bt_gatts_cccd_ind_t* p_cccd_ind = (rtk_bt_gatts_cccd_ind_t*)data;
    uint16_t index = p_cccd_ind->index;
    uint16_t conn_handle = p_cccd_ind->conn_handle;
    uint8_t cccd_ntf = p_cccd_ind->value & RTK_BT_GATT_CCC_NOTIFY;
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

    switch (index) {
    case REPORT_INPUT_CHAR_CCCD_INDEX:
        report_input_cccd_ntf_en_map[conn_id] = cccd_ntf;
        printf("[APP] HIDS report input charac cccd bit, notify: %d\r\n", cccd_ntf);
        break;

    case BOOT_KB_IN_REPORT_CHAR_CCCD_INDEX:
        printf("[APP] HIDS boot keyboard input charac cccd bit, notify: %d\r\n", cccd_ntf);
        boot_kb_in_cccd_ntf_en_map[conn_id] = cccd_ntf;
        break;

    default:
        break;
    }
}


void hid_srv_callback(uint8_t event, void *data)
{
    switch (event) {
        case RTK_BT_GATTS_EVT_REGISTER_SERVICE: {
            rtk_bt_gatts_reg_ind_t* reg_srv_res = (rtk_bt_gatts_reg_ind_t*)data;
            if (RTK_BT_OK == reg_srv_res->reg_status) {
                printf("[APP] HIDS register service succeed!\r\n");
            } else {
                printf("[APP] HIDS register service failed, err: 0x%x\r\n", reg_srv_res->reg_status);
            }
            
            break;
        }
        
        case RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND:{
            rtk_bt_gatts_ntf_and_ind_ind_t* p_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
            if (RTK_BT_OK == p_ind->err_code)
                printf("[APP] HIDS indicate succeed!\r\n");
            else
                printf("[APP] HIDS indicate failed, err: 0x%x\r\n", p_ind->err_code);

            break;
        }

    case RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND: {
            rtk_bt_gatts_ntf_and_ind_ind_t* p_ntf_ind = (rtk_bt_gatts_ntf_and_ind_ind_t*)data;
            if (RTK_BT_OK == p_ntf_ind->err_code)
                printf("[APP] HIDS notify succeed!\r\n");
            else
                printf("[APP] HIDS notify failed, err: 0x%x\r\n", p_ntf_ind->err_code);

            break;
        }

    case RTK_BT_GATTS_EVT_READ_IND:
        hids_read_hdl(data);
        break;

    case RTK_BT_GATTS_EVT_WRITE_IND:
        hids_write_hdl(data);
        break;

    case RTK_BT_GATTS_EVT_CCCD_IND:
        hids_cccd_update_hdl(data);
        break;
    default:
        break;
    }
}

void hids_cccd_notify(uint16_t conn_handle, uint16_t char_index, void *data, uint16_t len)
{
    rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};
    ntf_param.app_id = HID_SRV_ID;
    ntf_param.conn_handle = conn_handle;
	// ntf_param.cid = 4;	// choose dynamic created L2CAP channel when EATT is enbaled.
    ntf_param.data = data;
    ntf_param.len = len;
    ntf_param.index = char_index;

    rtk_bt_gatts_notify(&ntf_param);
}

uint16_t hid_srv_add(void)
{
    hid_srv.type = GATT_SERVICE_OVER_BLE;
    hid_srv.server_info = 0;
    hid_srv.user_data = NULL;
    hid_srv.register_status = 0;

    return rtk_bt_gatts_register_service(&hid_srv);
}



void hids_disconnect(uint16_t conn_handle)
{
	uint8_t conn_id;

	if (rtk_bt_le_gap_get_conn_id(conn_handle, &conn_id) != RTK_BT_OK)
		return;

    report_input_cccd_ntf_en_map[conn_id] = 0;
    boot_kb_in_cccd_ntf_en_map[conn_id] = 0;
}

void hids_status_deinit(void)
{
    memset(report_input_cccd_ntf_en_map, 0, sizeof(report_input_cccd_ntf_en_map));
    memset(boot_kb_in_cccd_ntf_en_map, 0, sizeof(boot_kb_in_cccd_ntf_en_map));
}


