/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gls.h
  * @brief    Head file for using glucose service.
  * @details  Glusose data types and external functions declaration.
  * @author   bill
  * @date     2017-6-8
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _GLS_H
#define _GLS_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"
#include "gls_config.h"


/** @defgroup GLS Glucose Service
  * @brief  Glucose service
   * @details

    The Glucose Service exposes glucose and other data related to a personal glucose sensor for
    consumer healthcare applications and is not designed for clinical use.
    Glucose Measurement, Glucose Measurement - Client Characteristic Configuration descriptor,
    Glucose Feature, Record Access Control Point, Record Access Control Point - Client Characteristic Configuration descriptor
    are mandatory exposed in the Glucose Service.
    Glucose Measurement Context is optional, and if the Glucose Measurement Context characteristic is supported,
    Glucose Measurement Context - Client Characteristic Configuration descriptor shall be mandatory.

    The Glucose Measurement characteristic shall be used to send glucose measurements.
    The Glucose Measurement Context characteristic may be used to send additional contextual information relative to
    a Glucose Measurement characteristic.
    The Glucose Feature characteristic shall be used to describe the supported features of the Server.
    For this service to operate, profiles or other applications utilizing this service will need to ensure that
    the Client configures the Record Access Control Point (RACP) characteristic for indications.
    The Client must perform a write to the Record Access Control Point to execute a desired procedure at the Server.

    Application shall register glucose service when initialization through @ref gls_add_service function.

    Application can set the glucose service parameters through @ref gls_set_parameter function.

    Application can send the glucose measurements through @ref gls_glc_measurement_notify function.

  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/

/** @defgroup GLS_Exported_Macros GLS Exported Macros
  * @brief
  * @{
  */

/** @defgroup GLS_Feature GLS Feature
  * @brief  glucose feature bits.
  * @{
  */
#define GLC_FEATURES_LOW_BATTERY                0x0001
#define GLC_FEATURES_SENSOR_MALFUNCTION         0x0002
#define GLC_FEATURES_SENSOR_SAMPLE_SIZE         0x0004
#define GLC_FEATURES_SENSOR_STRIP_INS_ERROR     0x0008
#define GLC_FEATURES_SENSOR_STRIP_TYPE_ERROR    0x0010
#define GLC_FEATURES_SENSOR_RESULT_HIGH_LOW     0x0020
#define GLC_FEATURES_SENSOR_TEMP_HIGH_LOW       0x0040
#define GLC_FEATURES_SENSOR_READ_INTERRUPT      0x0080
#define GLC_FEATURES_GENERAL_DEVICE_FAULT       0x0100
#define GLC_FEATURES_TIME_FAULT                 0x0200
#define GLC_FEATURES_MULTIPLE_BOND              0x0400

/** GLC features supported: */
#define GLC_FEATURES          (GLC_FEATURES_LOW_BATTERY    |  \
                               GLC_FEATURES_SENSOR_STRIP_INS_ERROR)
/** @} */

/**
*  @brief Gulcose service parameter type
*/
typedef enum
{
    /** glucose measurement parameters */
    GLS_PARAM_GLC_MS_FLAG = 0x01,
    GLS_PARAM_GLC_MS_BASE_TIME,
    GLS_PARAM_GLC_MS_TIME_OFFSET,
    GLS_PARAM_GLC_MS_CONCENTRATION,
    GLS_PARAM_GLC_MS_CONCENTRATION_UNITS,
    GLS_PARAM_GLC_MS_TYPE_SAMPLE_LOCATION,
    GLS_PARAM_GLC_MS_SENSOR_STATUS_ANNUNCIATION,

    /** glucose measurement context parameters */
    GLS_PARAM_GLC_MS_CT_FLAG,
    GLS_PARAM_GLC_MS_CT_CARBOHYDRATE_ID,
    GLS_PARAM_GLC_MS_CT_CARBOHYDRATE,
    GLS_PARAM_GLC_MS_CT_MEAL,
    GLS_PARAM_GLC_MS_CT_TESTER_HEALTH,
    GLS_PARAM_GLC_MS_CT_EXERCISE_DURATION,
    GLS_PARAM_GLC_MS_CT_EXERCISE_INTENSITY,
    GLS_PARAM_GLC_MS_CT_MEDICATION_ID,
    GLS_PARAM_GLC_MS_CT_MEDICATION,
    GLS_PARAM_GLC_MS_CT_MEDICATION_UNITS,
    GLS_PARAM_GLC_MS_CT_HbA1c,

    /** Parameters that can be get by application */
    GLS_PARAM_GLC_FEATURES,
    GLS_PARAM_CTL_PNT_PROG_CLR,
    GLS_PARAM_RECORD_NUM,
    GLS_PARAM_RECORD_SEQ_NUM
} T_GLS_PARAM_TYPE;


/** @defgroup GLS_Upstream_Message GLS Upstream Message
  * @brief  Upstream message used to inform application.
  * @{
  */

/** @defgroup GLS_Read_Info GLS Read Info
  * @brief  Parameter for read characteristic value.
  * @{
  */
#define GLS_EVT_READ_FEATURE                            0x01
/** @} */

/** @defgroup GLS_Notify_Indicate_Info GLS Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define GLS_EVT_GLC_MEASUREMENT_NOTIFY_ENABLE           0x01
#define GLS_EVT_GLC_MEASUREMENT_NOTIFY_DISABLE          0x02
#define GLS_EVT_GLC_MEASUREMENT_CONTEXT_NOTIFY_ENABLE   0x03
#define GLS_EVT_GLC_MEASUREMENT_CONTEXT_NOTIFY_DISABLE  0x04
#define GLS_EVT_GLC_RACP_INDICATE_ENABLE                0x05
#define GLS_EVT_GLC_RACP_INDICATE_DISABLE               0x06
/** @} */

/** @} End of GLS_Upstream_Message */

/** @defgroup GLS_Control_Point GLS Control Point
  * @brief  glucose record access control point
  * @{
  */

/** @brief  Glucose Record Access Control Point OpCodes. */
typedef enum
{
    GLC_RACP_OPCODE_RESERVED = 0x00,
    GLC_RACP_OPCODE_REPORT_RECS = 0x01,
    GLC_RACP_OPCODE_DELETE_RECS = 0x02,
    GLC_RACP_OPCODE_ABORT_OPERATION = 0x03,
    GLC_RACP_OPCODE_REPORT_NBR_OF_RECS = 0x04,
    GLC_RACP_OPCODE_NBR_OF_RECS_RESP = 0x05,
    GLC_RACP_OPCODE_RESP_CODE = 0x06
} T_GLC_CTRL_POINT_OPCODE;

/** @brief  Check glucose Record Access Control Point operation is available or not. */
#define GLC_RACP_OPERATION_ACTIVE(x)        \
    ((x >= GLC_RACP_OPCODE_REPORT_RECS) &&  \
     (x <= GLC_RACP_OPCODE_RESP_CODE))

/** @brief  Glucose Record Access Control Point Operator. */
typedef enum
{
    GLC_RACP_OPERATOR_NULL = 0x00,
    GLC_RACP_OPERATOR_ALL_RECS = 0x01,
    GLC_RACP_OPERATOR_LT_EQ = 0x02,
    GLC_RACP_OPERATOR_GT_EQ = 0x03,
    GLC_RACP_OPERATOR_RANGE = 0x04,
    GLC_RACP_OPERATOR_FIRST = 0x05,
    GLC_RACP_OPERATOR_LAST = 0x06
} T_GLC_CTRL_POINT_OPERATOR;

/** @brief Filter Type values. */
typedef enum
{
    GLC_RACP_FILTER_TYPE_RESERVED = 0x00,
    GLC_RACP_FILTER_TYPE_SEQ_NBR = 0x01,
    GLC_RACP_FILTER_TYPE_TIME = 0x02
} T_GLC_CTRL_POINT_FILTER_TYPE;

/** @brief  Glucose Record Access Control Point Response Codes. */
typedef enum
{
    GLC_RACP_RESP_RESERVED = 0x00,
    GLC_RACP_RESP_SUCCESS = 0x01,
    GLC_RACP_RESP_OPCODE_NOT_SUPPORTED = 0x02,
    GLC_RACP_RESP_INVALID_OPERATOR = 0x03,
    GLC_RACP_RESP_OPERATOR_NOT_SUPPORTED = 0x04,
    GLC_RACP_RESP_INVALID_OPERAND = 0x05,
    GLC_RACP_RESP_NO_RECS_FOUND = 0x06,
    GLC_RACP_RESP_ABORT_UNSUCCESSFUL = 0x07,
    GLC_RACP_RESP_PROC_NOT_COMPLETED = 0x08,
    GLC_RACP_RESP_OPERAND_NOT_SUPPORTED = 0x09
} T_GLC_CTRL_POINT_RESP_CODES;

/** @brief profile specific attribute protocol application error codes */
#define GLC_ERR_PROC_ALREADY_IN_PROGRESS                0x80
#define GLC_ERR_CCCD_IMPROPERLY_CONFIGURED              0x81
/** @} */

/** @brief concentration unit */
#define GLC_FLAGS_UNITS_MOL_L_ON                        1
#define GLC_FLAGS_UNITS_KG_L_ON                         0

/** @brief medication unit */
#define GLC_FLAGS_MEDICATION_UNITS_LITERS_ON            1
#define GLC_FLAGS_MEDICATION_UNITS_KG_ON                0

/** GLC sensor status annunciation bits */
#define GLC_SS_ANNUNC_LOW_BATTERY                       0x0001

/** special short float values */
#define SFLOAT_VALUE_NaN                                0x07ff    /**< not a number             */
#define SFLOAT_VALUE_NRes                               0x0800    /**< not at this resolution   */
#define SFLOAT_VALUE_PlusINFINITY                       0x07fe    /**< + INFINITY               */
#define SFLOAT_VALUE_MinusINFINITY                      0x0802    /**< - INFINITY               */
#define SFLOAT_VALUE_RFU                                0x0801    /**< reserved for future use  */

/** @brief RACP database parameters */
#define GLC_RACP_DATABASE_SIZE                          (GLC_RACP_MAX_NBR_OF_STORED_RECS + 1)
#define GLC_RACP_INIT_SEQ_NBR_DEFAULT                   0

/** @} End of GLS_Exported_Macros */



/*============================================================================*
 *                         Types
 *============================================================================*/

/** @defgroup GLS_Exported_Types GLS Exported Types
  * @brief
  * @{
  */

/* Add all public types here */

/** GLC measurement flag bits */
typedef struct
{
    uint8_t time_offset: 1;  /**< time offset         */
    uint8_t con_ts_loc: 1;  /**< concentration, time/sample location */
    uint8_t con_units: 1;  /**< 0: kg/L, 1: mol/L        */
    uint8_t ss_annuciation: 1;  /**< sensor status annunciation */
    uint8_t ctxt_info_follows: 1;  /**< context information  */
    uint8_t rfu: 3;
} T_GLC_MEASUREMENT_FLAG;

/** GLC measurement context flag bits */
typedef struct
{
    uint8_t carbohydrate: 1;  /**< Carbohydrates ID and field  */
    uint8_t meal: 1;  /**< Meal ID and field           */
    uint8_t tester_health: 1;  /**< Tester-Health field         */
    uint8_t excercise: 1;  /**< Exercise Duration and Intensity field */
    uint8_t medication: 1; /**< Medication ID and field     */
    uint8_t medication_units: 1;  /**< Medication units 0:kg, 1:liter */
    uint8_t hb_a1c: 1;  /**< hb_a1c field                 */
    uint8_t ext_flags: 1;  /**< extended flag               */
} T_GLC_MSR_CTXT_FLAG;

typedef uint8_t     TIMESTAMP[7];
typedef uint8_t     SFLOAT[2];         /* 4 bit (MSB) exponent, 12 bit mantissa */

typedef struct
{
    T_GLC_MEASUREMENT_FLAG   flags;
    uint16_t     seq_num;
    TIMESTAMP  base_time;
#if (GLC_INCLUDE_TIME_OFFSET)
    int16_t      time_offset; //minutes
#endif
#if (GLC_INCLUDE_CONC_TS_LOC)
    SFLOAT     concentration;
    uint8_t    type_sample_location;
#endif
#if (GLC_INCLUDE_SS_ANNUNC)
    uint16_t    ss_annunciation;
#endif
} T_GLC_MEASUREMENT_VALUE;

typedef struct
{
    T_GLC_MSR_CTXT_FLAG    flags;
    uint16_t   seq_num;
#if (GLC_INCLUDE_EXT_FLAGS)
    uint8_t    ext_flags;
#endif
#if (GLC_INCLUDE_CARBOHYDRATE)
    uint8_t    carbohydrate_ID;
    SFLOAT     carbohydrate;
#endif
#if (GLC_INCLUDE_MEAL)
    uint8_t    meal;
#endif
#if (GLC_INCLUDE_TESTER_HEALTH)
    uint8_t    tester_health;
#endif
#if (GLC_INCLUDE_EXCERCISE)
    uint16_t   exercise_duration;
    uint8_t    exercise_intensity;
#endif
#if (GLC_INCLUDE_MEDICATION)
    uint8_t    medication_ID;
    SFLOAT     medication;
#endif
#if (GLC_INCLUDE_HbA1c)
    SFLOAT     hb_a1c;
#endif
} T_GLC_MEASUREMENT_CONTEXT;

typedef struct
{
    T_GLC_CTRL_POINT_OPCODE op_code;
    T_GLC_CTRL_POINT_OPERATOR op; /**< operator */
    uint8_t operand[18];
} T_GLC_CONTROL_POINT;

typedef struct
{
    T_GLC_MEASUREMENT_VALUE glc_measurement_value;
#if GLC_MEASUREMENT_CONTEXT_SUPPORT
    T_GLC_MEASUREMENT_CONTEXT glc_measurement_context;
#endif
} T_PATIENT_RECORD;

typedef struct
{
    T_PATIENT_RECORD records[GLC_RACP_DATABASE_SIZE];
    uint8_t record_num;
    /**
        The tail points to an empty record who plays a role as a guard.
        1. empty tail == head
        2. one   (tail - head + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE == 1
        3. full (head - tail + GLC_RACP_DATABASE_SIZE) % GLC_RACP_DATABASE_SIZE == 1
    */
    uint8_t head;
    uint8_t tail;
    uint16_t seq_num; /**< sequence number of latest record */
} T_RECORD_DATA_BASE;

typedef struct
{
    T_GLC_CONTROL_POINT ctrl_point;
    uint8_t cp_length; /**< length of current operand of control point */
    T_RECORD_DATA_BASE record_db;
} T_GLC_RACP;

/** Flags indicate CCCD */
typedef struct
{
    uint8_t GLC_measurement_notify_enable: 1;
    uint8_t GLC_measurement_context_notify_enable: 1;
    uint8_t GLC_RACP_indicate_enable: 1;
    uint8_t rfu: 5;
} T_GLC_NOTIFY_INDICATE_FLAG;

/** @defgroup GLS_Callback_Data GLS Callback Data
  * @brief GLS data struct for notification data to application.
  * @{
  */
typedef union
{
    uint8_t notify_indicate_index;
    uint8_t read_value_index;
    T_GLC_CONTROL_POINT write;
} T_GLS_UPSTREAM_MSG_DATA;

typedef struct
{
    T_SERVICE_CALLBACK_TYPE    msg_type;
    T_GLS_UPSTREAM_MSG_DATA    msg_data;
} T_GLS_CALLBACK_DATA;
/** @} */

/** @} End of GLS_Exported_Types */


/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GLS_Exported_Functions GLS Exported Functions
  * @brief
  * @{
  */

/**
  * @brief       Add gulcose service to the BLE stack database.
  *
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval Others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void profile_init()
     {
         server_init(1);
         gls_id = gls_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID gls_add_service(void *p_func);


/**
 * @brief       Set a GLS parameter.
 *
 *              NOTE: You can call this function with a gulcose service parameter type and it will set the
 *                      gulcose service parameter.  Gulcose service parameters are defined in @ref T_GLS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Gulcose service parameter type: @ref T_GLS_PARAM_TYPE
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
        T_GLC_MEASUREMENT_FLAG ms_flag =
        {
            1, //time_offset
            1, //con_ts_loc
            GLC_FLAGS_UNITS_MOL_L_ON, //con_units
            1, //ss_annuciation
            0, //ctxt_info_follows
            0  //rfu
        };
        gls_set_parameter(GLS_PARAM_GLC_MS_FLAG, 1, &ms_flag);
    }
 * \endcode
 */
bool gls_set_parameter(T_GLS_PARAM_TYPE param_type, uint8_t len, void *p_value);


/**
 * @brief   Get a GLS parameter.
 *
 *          NOTE: You can call this function with a gulcose parameter type and it will get a
 *          gulcose parameter. Gulcose parameters are defined in @ref T_GLS_PARAM_TYPE.
 *
 * @param[in]   param_type Gulcose parameter type: @ref T_GLS_PARAM_TYPE
 * @param[in,out]   len Pointer to the location to get the length of data.
 * @param[in,out]  p_value Pointer to the location to get the parameter value.  This is dependent on
 *                      the parameter type and will be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        int record_num;
        gls_get_parameter(GLS_PARAM_RECORD_NUM, &len, &record_num);
    }
 * \endcode
 */
bool gls_get_parameter(T_GLS_PARAM_TYPE param_type, uint8_t *len, void *p_value);


/**
 * @brief       Send measurement notification data .
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   index  Index.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t battery_level = 90;
        bas_battery_level_value_notify(conn_id, bas_id, battery_level);
    }
 * \endcode
 */
bool gls_glc_measurement_notify(uint8_t conn_id, T_SERVER_ID service_id, uint8_t index);

#if GLC_MEASUREMENT_CONTEXT_SUPPORT

/**
 * @brief       Send measurement context notification data .
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   index  Index.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t battery_level = 90;
        bas_battery_level_value_notify(conn_id, bas_id, battery_level);
    }
 * \endcode
 */
bool gls_glc_measurement_context_notify(uint8_t conn_id, T_SERVER_ID service_id, uint8_t index);
#endif


/**
 * @brief       Indicate control point data response to client from application
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   rsp_code  Response code.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool gls_racp_response(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code);

/**
 * @brief       Indicate control point data number response to client from application
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   num  Number of glucose records.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool gls_racp_num_response(uint8_t conn_id, T_SERVER_ID service_i, uint16_t num);

/**
 * @brief       Prepare a new record in database.
 * @return void.
 *
 */
void gls_prepare_new_record(void);


/**
 * @brief       Push a new record into database.
 * @return void.
 *
 */
void gls_push_new_record(void);

/**
 * @brief       Report records sub procedure.
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t battery_level = 90;
        bas_battery_level_value_notify(conn_id, bas_id, battery_level);
    }
 * \endcode
 */
bool gls_report_records_task(uint8_t conn_id, T_SERVER_ID service_id);

/**
 * @brief       Calculate user facing time by adding basetime and timeoffset.
 *
 *
 * @param[in]   time_in  Connection id.
 * @param[in]   time_offset  Service id.
 * @param[in]   time_out  Battery level value.
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        user_face_time(time, 30, time);
    }
 * \endcode
 */

void user_face_time(TIMESTAMP time_in, int16_t time_offset, TIMESTAMP time_out);

/**
 * @brief       Abort RACP procedure by app.
 *
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        gls_abort_racp_procedure();
    }
 * \endcode
 */
void gls_abort_racp_procedure(void);

/** @} End of GLS_Exported_Functions */

/** @} End of GLS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GLUCOSE_SERVICE_H */
