/*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     hrs.h
  * @brief    Variables and interfaces for using Heart Rate Service.
  * @details  Heart Rate Service data structs and functions.
  * @author
  * @date     2017-09-21
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _HRS_SERVICE_DEF_H
#define _HRS_SERVICE_DEF_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"


/** @defgroup HRS Heart Rate Service
  * @brief  Heart Rate Service
  * @details

    The Heart Rate Service exposes heart rate and other data related to a heart rate sensor intended for fitness applications.

    Application shall register heart rate service when initialization through @ref hrs_add_service function.

    The Heart Rate Measurement characteristic is used to send a heart rate measurement. Included in the characteristic are a Flags field (for showing the presence of optional fields and features
    supported), a heart rate measurement value field and, depending upon the contents of the Flags field, an Energy Expended field and an RR-Interval field. The RR-Interval represents the time between
    two consecutive R waves in an Electrocardiogram (ECG) waveform.
    Application can send heart rate measurement value through @ref hrs_heart_rate_measurement_value_notify function.

    The Body Sensor Location characteristic of the device is used to describe the intended location of the heart rate measurement for the device.
    The value of the Body Sensor Location characteristic is static while in a connection.

    The Heart Rate Control Point characteristic is used to enable a Client to write control points to a Server to control behavior.
    Support for this characteristic is mandatory if the Server supports the Energy Expended feature.

    Application can set  heart rate measurement parameter and location of the heart rate measurement for the device through @ref hrs_set_parameter function.

  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup HRS_Exported_Macros HRS Exported Macros
  * @brief
  * @{
  */


/** @defgroup HRS_Read_Info HRS Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define HRS_READ_BODY_SENSOR_LOCATION_VALUE                 1
/** @} */

/** @defgroup HRS_Notify_Indicate_Info HRS Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define HRS_NOTIFY_INDICATE_MEASUREMENT_VALUE_ENABLE        1
#define HRS_NOTIFY_INDICATE_MEASUREMENT_VALUE_DISABLE       2
/** @} */



/** @defgroup HRS_Sensor_Location HRS Sensor Location
  * @brief  Body Sensor Location Value
  * @{
  */
#define  BODY_SENSOR_LOCATION_VALUE_OTHER                   0
#define  BODY_SENSOR_LOCATION_VALUE_CHEST                   1
#define  BODY_SENSOR_LOCATION_VALUE_WRIST                   2
#define  BODY_SENSOR_LOCATION_VALUE_FINGER                  3
#define  BODY_SENSOR_LOCATION_VALUE_HAND                    4
#define  BODY_SENSOR_LOCATION_VALUE_EAR_LOBE                5
#define  BODY_SENSOR_LOCATION_VALUE_FOOT                    6
/** @} */

#define HRS_MAX_CTL_PNT_VALUE                               1

/** End of HRS_Exported_Macros
* @}
*/



/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup HRS_Exported_Types HRS Exported Types
  * @brief
  * @{
  */

typedef enum
{
    HRS_HEART_RATE_MEASUREMENT_PARAM_FLAG = 0x01,
    HRS_HEART_RATE_MEASUREMENT_PARAM_MEASUREMENT_VALUE,
    HRS_HEART_RATE_MEASUREMENT_PARAM_ENERGY_EXPENDED,
    HRS_HEART_RATE_MEASUREMENT_PARAM_RR_INTERVAL,
    HRS_BODY_SENSOR_LOCATION_PARAM_VALUE = 0x10,
} T_HRS_PARAM_TYPE;

typedef enum
{
    HRS_HEART_RATE_CP_OPCODE_RESERVED = 0,
    HRS_HEART_RATE_CP_OPCODE_RESET_ENERGY_EXPENDED = 1
} T_HRS_HEART_RATE_CP_OPCODE;


/** notification indification falg */
typedef struct
{
    uint8_t heart_rate_measurement_notify_enable: 1;
    uint8_t rfu: 7;
} HRS_NOTIFY_INDICATE_FLAG;

/** Heart Rate Measurement Value Flag */
typedef struct
{
    uint8_t heart_rate_value_format_bit: 1;
    uint8_t sensor_contact_status_bits: 2;
    uint8_t energy_expended_status_bit: 1;
    uint8_t rr_interval_bit: 1;
    uint8_t rfu: 3;
} T_HEART_RATE_MEASUREMENT_VALUE_FLAG;

/** Heart Rate Measurement Value */
typedef struct
{
    T_HEART_RATE_MEASUREMENT_VALUE_FLAG flag;
    uint16_t heart_rate_measurement_value;
    uint16_t energy_expended;
    uint16_t rr_interval;
} T_HEART_RATE_MEASUREMENT_VALUE;

/**
 * @brief HRS Control Point data, variable length during connection, max can reach 17 bytes.
 *
 * HRS Control Point data used to store the Control Point Command recieved from the client.
*/
typedef struct
{
    uint8_t    cur_length; /**<  length of current CSC Control Point data . */
    uint8_t    value[HRS_MAX_CTL_PNT_VALUE]; /**<  value of current CSC Control Point data . */
} T_HRS_CONTROL_POINT;

typedef struct
{
    T_HRS_HEART_RATE_CP_OPCODE opcode;
} T_HRS_WRITE_MSG;

typedef union
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
    T_HRS_WRITE_MSG write;
} T_HRS_UPSTREAM_MSG_DATA;

typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_HRS_UPSTREAM_MSG_DATA    msg_data;
} T_HRS_CALLBACK_DATA;
/** End of HRS_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup HRS_Exported_Functions HRS Exported Functions
  * @brief
  * @{
  */

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
uint8_t hrs_add_service(void *p_func);


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

bool hrs_set_parameter(T_HRS_PARAM_TYPE param_type, uint8_t len, void *p_value);

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
bool hrs_heart_rate_measurement_value_notify(uint8_t conn_id, T_SERVER_ID service_id);

/** @} End of HRS_Exported_Functions */

/** @} End of HRS */


#ifdef __cplusplus
}
#endif

#endif /* _HRS_SERVICE_DEF_H */

