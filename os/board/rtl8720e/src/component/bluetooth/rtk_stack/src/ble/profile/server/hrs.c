/*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     hrs.c
* @brief    Heart Rate service source file.
* @details  Interfaces to access Heart Rate service.
* @author
* @date     2017-9-21
* @version  v1.0
*********************************************************************************************************
*/
#include "stdint.h"
#include "gatt.h"
#include <string.h>
#include "trace.h"
#include "profile_server.h"
#include "hrs.h"


/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
#define HRS_BODY_SENSOR_LOCATION_CHAR_SUPPORT
#define HRS_ENERGY_EXPENDED_FEATURE_SUPPORT

#define HRS_HEART_RATE_MEASUREMENT_VALUE_INDEX              2//notify
#define HRS_BODY_SENSOR_LOCATION_VALUE_INDEX                5//read
#define HRS_HEART_RATE_CP_VALUE_INDEX                       7//write

#define HRS_HEART_RATE_MEASUREMENT_CCCD_INDEX               3

#define GATT_UUID_SERVICE_HEART_RATE                        0x180D
#define GATT_UUID_CHAR_HRS_HEART_RATE_MEASUREMENT           0x2A37
#define GATT_UUID_CHAR_HRS_BODY_SENSOR_LOCATION             0x2A38
#define GATT_UUID_CHAR_HRS_HEART_RATE_CP                    0x2A39

#define HRS_CP_RSPCODE_RESERVED                             0x00
#define HRS_CP_RSPCODE_SUCCESS                              0x01
#define HRS_CP_RSPCODE_OPCODE_UNSUPPORT                     0x02
#define HRS_CP_RSPCODE_INVALID_PARAMETER                    0x03
#define HRS_CP_RSPCODE_OPERATION_FAILED                     0x04

#define HRS_CTL_PNT_OPERATE_ACTIVE(x)                     \
    (x == HRS_HEART_RATE_CP_OPCODE_RESET_ENERGY_EXPENDED)

#define Heart_Rate_Value_Format_UINT8                       0
#define Heart_Rate_Value_Format_UINT16                      1

#define HRS_HEART_RATE_MEASUREMENT_VALUE_MAX_LEN            7



HRS_NOTIFY_INDICATE_FLAG hrs_notify_indicate_flag = {0};
T_HEART_RATE_MEASUREMENT_VALUE hrs_heart_rate_measurement_value = {0};
uint8_t hrs_body_sensor_location_value = 0;
T_HRS_CONTROL_POINT hrs_ctl_pnt = {0};


/**<  Function pointer used to send event to application from location and navigation profile. */

static P_FUN_SERVER_GENERAL_CB pfn_hrs_app_cb = NULL;

/**< @brief  profile/service definition.  */
static const T_ATTRIB_APPL hrs_attr_tbl[] =
{
    /*----------------- Heart Rate Service -------------------*/
    /* <<Primary Service>>, .. 0,*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),   /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_SERVICE_HEART_RATE),              /* service UUID */
            HI_WORD(GATT_UUID_SERVICE_HEART_RATE)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, Heart Rate Measurement*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_NOTIFY/* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Temperature Measurement value 2,*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_HRS_HEART_RATE_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_HRS_HEART_RATE_MEASUREMENT)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NOTIF_IND/* wPermissions */
    },

    /* client characteristic configuration */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                   /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    }
#ifdef HRS_BODY_SENSOR_LOCATION_CHAR_SUPPORT
    ,
    /* <<Characteristic>>Body Sensor Location*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ/* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_HRS_BODY_SENSOR_LOCATION),
            HI_WORD(GATT_UUID_CHAR_HRS_BODY_SENSOR_LOCATION)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    }
#endif

#ifdef HRS_ENERGY_EXPENDED_FEATURE_SUPPORT
    ,
    /* <<Characteristic>> Heart Rate Control Point*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE/* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_HRS_HEART_RATE_CP),
            HI_WORD(GATT_UUID_CHAR_HRS_HEART_RATE_CP)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_WRITE/* wPermissions */
    }
#endif
};

/**< @brief  Heart Rate service size definition.  */
const uint16_t hrs_attr_tbl_size = sizeof(hrs_attr_tbl);

/**
 * @brief       Set a heart rate service parameter.
 *
 *              NOTE: You can call this function with a heart rate service parameter type and it will set the
 *                      heart rate service parameter.  Heart rate service parameters are defined
 *                      in @ref T_HTS_TEMPERATURE_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Health thermometer service parameter type: @ref T_HRS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_HEART_RATE_MEASUREMENT_VALUE_FLAG flag;
        flag.heart_rate_value_format_bit = 1;
        flag.sensor_contact_status_bits = 3;
        if (p_parse_value->param_count >= 1)
        {
            flag.sensor_contact_status_bits = p_parse_value->dw_param[1];
        }

        flag.energy_expended_status_bit = 1;
        flag.rr_interval_bit = 1;
        flag.rfu = 0;

        hrs_set_parameter(HRS_HEART_RATE_MEASUREMENT_PARAM_FLAG, 1, &flag);
    }
 * \endcode
 */

bool hrs_set_parameter(T_HRS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR0("hrs_set_parameter failed\n");
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_FLAG:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&hrs_heart_rate_measurement_value.flag, p_value, len);
            }
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_MEASUREMENT_VALUE:
        {
            if (len != sizeof(uint16_t) && len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&hrs_heart_rate_measurement_value.heart_rate_measurement_value, p_value, len);

            }
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_ENERGY_EXPENDED:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&hrs_heart_rate_measurement_value.energy_expended, p_value, len);

            }
        }
        break;

    case HRS_HEART_RATE_MEASUREMENT_PARAM_RR_INTERVAL:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&hrs_heart_rate_measurement_value.rr_interval, p_value, len);

            }
        }
        break;

    case HRS_BODY_SENSOR_LOCATION_PARAM_VALUE:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&hrs_body_sensor_location_value, p_value, len);
            }
        }
        break;

    }

    return ret;
}

/**
 * @brief       Send heart rate measurement value notification data.
 *              Application shall call @ref hrs_set_parameter to set heart rate measurement value first,
 *              and the call this api to send the notication value.
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 *
 * @return service id @ref T_SERVER_ID.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool op_result;

        T_HEART_RATE_MEASUREMENT_VALUE_FLAG flag;
        flag.heart_rate_value_format_bit = 1;
        flag.sensor_contact_status_bits = 3;
        if (p_parse_value->param_count >= 1)
        {
            flag.sensor_contact_status_bits = p_parse_value->dw_param[1];
        }

        flag.energy_expended_status_bit = 1;
        flag.rr_interval_bit = 1;
        flag.rfu = 0;

        hrs_set_parameter(HRS_HEART_RATE_MEASUREMENT_PARAM_FLAG, 1, &flag);

        op_result = hrs_heart_rate_measurement_value_notify(p_parse_value->dw_param[0], hrs_id);
    }
 * \endcode
 */
bool hrs_heart_rate_measurement_value_notify(uint8_t conn_id, T_SERVER_ID service_id)
{
    uint8_t heart_rate_measurement_value[HRS_HEART_RATE_MEASUREMENT_VALUE_MAX_LEN];
    uint8_t cur_offset = 0;

    memcpy(&heart_rate_measurement_value[cur_offset], &hrs_heart_rate_measurement_value.flag, 1);
    cur_offset += 1;

    if (hrs_heart_rate_measurement_value.flag.heart_rate_value_format_bit ==
        Heart_Rate_Value_Format_UINT8)
    {
        memcpy(&heart_rate_measurement_value[cur_offset],
               &hrs_heart_rate_measurement_value.heart_rate_measurement_value, 1);
        cur_offset += 1;
    }
    else
    {
        memcpy(&heart_rate_measurement_value[cur_offset],
               &hrs_heart_rate_measurement_value.heart_rate_measurement_value, 2);
        cur_offset += 2;
    }

    if (hrs_heart_rate_measurement_value.flag.energy_expended_status_bit)
    {
        memcpy(&heart_rate_measurement_value[cur_offset], &hrs_heart_rate_measurement_value.energy_expended,
               2);
        cur_offset += 2;
    }

    if (hrs_heart_rate_measurement_value.flag.rr_interval_bit)
    {
        memcpy(&heart_rate_measurement_value[cur_offset], &hrs_heart_rate_measurement_value.rr_interval, 2);
        cur_offset += 2;
    }

    PROFILE_PRINT_INFO0("hrs_heart_rate_measurement_value_notify");
    return server_send_data(conn_id, service_id, HRS_HEART_RATE_MEASUREMENT_VALUE_INDEX,
                            heart_rate_measurement_value, cur_offset, GATT_PDU_TYPE_NOTIFICATION);
}

/**
 * @brief  handle control point write (request).
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id    Service ID.
 * @param write_length      Write request data length.
 * @param value_ptr         Pointer to write request data.
 * @return none
 * @retval  void
*/
static T_APP_RESULT hrs_hanlde_ctl_pnt_proc(uint8_t conn_id, T_SERVER_ID service_id,
                                            uint16_t write_length, uint8_t *p_value)
{
    T_HRS_CALLBACK_DATA callback_data;
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;
    uint16_t parameter_length = 0;
    memcpy(hrs_ctl_pnt.value, p_value, write_length);
    if (write_length >= 1)
    {
        parameter_length = write_length - 1;
    }

    PROFILE_PRINT_INFO1("hrs_hanlde_ctl_pnt_proc request: OpCode=0x%x", hrs_ctl_pnt.value[0]);

    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.opcode = (T_HRS_HEART_RATE_CP_OPCODE)hrs_ctl_pnt.value[0];


    switch (hrs_ctl_pnt.value[0])
    {
    case HRS_HEART_RATE_CP_OPCODE_RESET_ENERGY_EXPENDED:
        {
            if (!hrs_notify_indicate_flag.heart_rate_measurement_notify_enable)
            {
                cause = APP_RESULT_PROC_ALREADY_IN_PROGRESS;
            }
            else if (parameter_length != 0)
            {
                cause = APP_RESULT_INVALID_PDU;
            }
        }
        break;


    default:
        {
            cause = APP_RESULT_APP_ERR;
        }
        break;
    }

    pfn_hrs_app_cb(service_id, (void *)&callback_data);
    return cause;
}


/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id    Service ID.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                Used for Blob Read.
 * @param[in,out] p_length            length of getting characteristic data.
 * @param[in,out] pp_value            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT hrs_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;

    PROFILE_PRINT_INFO2("hrs_attr_read_cb iAttribIndex = %d iOffset %x", attrib_index, offset);

    *p_length = 0;

    switch (attrib_index)
    {
    case HRS_BODY_SENSOR_LOCATION_VALUE_INDEX:
        {
            T_HRS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = HRS_READ_BODY_SENSOR_LOCATION_VALUE;
            pfn_hrs_app_cb(service_id, (void *)&callback_data);

            *p_length = sizeof(hrs_body_sensor_location_value);
            *pp_value = (uint8_t *) &hrs_body_sensor_location_value;
        }
        break;

    default:
        {
            PROFILE_PRINT_ERROR1("hrs_attr_read_cb iAttribIndex = %d not found", attrib_index);
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }

    return cause;
}


/**
 * @brief write characteristic data from service.
 *
 * @param[in] conn_id          Connection id.
 * @param[in] service_id          ServiceID to be written.
 * @param[in] attrib_index          Attribute index of characteristic.
 * @param[in] length            length of value to be written.
 * @param[in] p_value            value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT hrs_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;
    PROFILE_PRINT_INFO2("hrs_attr_write_cb  iAttribIndex = %d wLength %x", attrib_index, length);

    switch (attrib_index)
    {
    case HRS_HEART_RATE_CP_VALUE_INDEX:
        {
            /* Attribute value has variable size, make sure written value size is valid. */
            if ((length > HRS_MAX_CTL_PNT_VALUE) || (p_value == NULL))
            {
                cause  = APP_RESULT_INVALID_VALUE_SIZE;
            }
            /* Make sure Control Point is not "Process already in progress". */
            else if (HRS_CTL_PNT_OPERATE_ACTIVE(hrs_ctl_pnt.value[0]))
            {
                cause  = APP_RESULT_PROC_ALREADY_IN_PROGRESS;
            }
            /* Make sure Control Point is configured indication enable. */
            else if (!hrs_notify_indicate_flag.heart_rate_measurement_notify_enable)
            {
                cause = APP_RESULT_CCCD_IMPROPERLY_CONFIGURED;
            }
            else
            {
                cause = hrs_hanlde_ctl_pnt_proc(conn_id, service_id, length, p_value);
            }

        }
        break;

    default:
        {
            PROFILE_PRINT_ERROR1("hrs_attr_write_cb  iAttribIndex = %d not found", attrib_index);
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }
    hrs_ctl_pnt.value[0] = HRS_HEART_RATE_CP_OPCODE_RESERVED;

    return cause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id          Service ID.
 * @param[in] index          Attribute index of characteristic data.
 * @param[in] ccc_bits         CCCD bits from stack.
 * @return None
*/
void hrs_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_HRS_CALLBACK_DATA hrs_callback_data;
    bool bHandle = true;

    PROFILE_PRINT_INFO2("hrs_cccd_update_cb  Index = %d wCCCDBits %x", index, ccc_bits);

    switch (index)
    {
    case HRS_HEART_RATE_MEASUREMENT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                // Enable Notification
                hrs_notify_indicate_flag.heart_rate_measurement_notify_enable = 1;
                hrs_callback_data.msg_data.notification_indification_index =
                    HRS_NOTIFY_INDICATE_MEASUREMENT_VALUE_ENABLE;
            }
            else
            {
                hrs_notify_indicate_flag.heart_rate_measurement_notify_enable = 0;
                hrs_callback_data.msg_data.notification_indification_index =
                    HRS_NOTIFY_INDICATE_MEASUREMENT_VALUE_DISABLE;
            }
        }
        break;


    default:
        {
            bHandle = false;
            PROFILE_PRINT_ERROR1("hrs_cccd_update_cb  Index = %d not found", index);
        }
        break;

    }
    /* Notify Application. */
    if (pfn_hrs_app_cb && (bHandle == true))
    {
        pfn_hrs_app_cb(service_id, (void *)&hrs_callback_data);
    }

    return;
}

/**
 * @brief Heart Rate Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS hrs_cbs =
{
    hrs_attr_read_cb,  // Read callback function pointer
    hrs_attr_write_cb, // Write callback function pointer
    hrs_cccd_update_cb  // CCCD update callback function pointer
};

/**
 * @brief       Add heart rate service to the BLE stack database.
 *
 *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
 *
 * <b>Example usage</b>
 * \code{.c}
    void profile_init()
    {
        server_init(1);
        hrs_id = hrs_add_service(app_handle_profile_message);
    }
 * \endcode
 */
T_SERVER_ID hrs_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)hrs_attr_tbl,
                                    hrs_attr_tbl_size,
                                    hrs_cbs))
    {
        PROFILE_PRINT_ERROR1("hrs_add_service: ServiceId %d", service_id);
        service_id = 0xff;
    }
    pfn_hrs_app_cb = (P_FUN_SERVER_GENERAL_CB)p_func;

    return service_id;
}


