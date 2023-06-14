/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __GLS_SERVICE_H__
#define __GLS_SERVICE_H__

#ifdef __cplusplus
extern "C"  {
#endif

#include <rtk_bt_def.h>

#define GLC_MEASUREMENT_CONTEXT_SUPPORT             1

#define GLC_RACP_MAX_NBR_OF_STORED_RECS         10
#define GLC_RACP_DATABASE_SIZE                  (GLC_RACP_MAX_NBR_OF_STORED_RECS + 1)

#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

#define LE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 0) +             \
              ((uint16_t)(*(a + 1)) << 8);              \
    }

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

typedef enum {
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

#define GLC_RACP_OPERATION_ACTIVE(x)        \
    ((x >= GLC_RACP_OPCODE_REPORT_RECS) &&  \
     (x <= GLC_RACP_OPCODE_RESP_CODE))

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

/** GLC measurement flag bits */
typedef struct
{
    uint8_t time_offset: 1;  /**< time offset         */
    uint8_t con_ts_loc: 1;  /**< concentration, time/sample location */
    uint8_t con_units: 1;  /**< 0: kg/L, 1: mol/L        */
    uint8_t ss_annunciation: 1;  /**< sensor status annunciation */
    uint8_t ctxt_info_follows: 1;  /**< context information  */
    uint8_t rfu: 3;
} T_GLC_MEASUREMENT_FLAG;

/** GLC measurement context flag bits */
typedef struct
{
    uint8_t carbohydrate: 1;  /**< Carbohydrates ID and field  */
    uint8_t meal: 1;  /**< Meal ID and field           */
    uint8_t tester_health: 1;  /**< Tester-Health field         */
    uint8_t exercise: 1;  /**< Exercise Duration and Intensity field */
    uint8_t medication: 1; /**< Medication ID and field     */
    uint8_t medication_units: 1;  /**< Medication units 0:kg, 1:liter */
    uint8_t hb_a1c: 1;  /**< hb_a1c field                 */
    uint8_t ext_flags: 1;  /**< extended flag               */
} T_GLC_MSR_CTXT_FLAG;

typedef uint8_t     TIMESTAMP[7];
typedef uint8_t     SFLOAT[2];  

typedef struct {
    T_GLC_MEASUREMENT_FLAG   flags;
    uint16_t    seq_num;
    TIMESTAMP   base_time;
    int16_t     time_offset; //minutes
    SFLOAT      concentration;
    uint8_t     type_sample_location;
    uint16_t    ss_annunciation;
} T_GLC_MEASUREMENT_VALUE;

typedef struct {
    T_GLC_MSR_CTXT_FLAG    flags;
    uint16_t    seq_num;
    uint8_t     ext_flags;
    uint8_t     carbohydrate_ID;
    SFLOAT      carbohydrate;
    uint8_t     meal;
    uint8_t     tester_health;
    uint16_t    exercise_duration;
    uint8_t     exercise_intensity;
    uint8_t     medication_ID;
    SFLOAT      medication;
    SFLOAT      hb_a1c;
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
// #if GLC_MEASUREMENT_CONTEXT_SUPPORT
    T_GLC_MEASUREMENT_CONTEXT glc_measurement_context;
// #endif
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

void glucose_srv_callback(uint8_t event, void *data);

uint16_t gls_set_parameter(T_GLS_PARAM_TYPE param_type, uint8_t len, void *p_value);

uint16_t gls_get_parameter(T_GLS_PARAM_TYPE param_type, uint8_t *len, void *p_value);

bool gls_glc_measurement_notify(uint16_t conn_handle, uint8_t index);

bool gls_glc_measurement_context_notify(uint16_t conn_handle, uint8_t index);

bool gls_racp_response(uint16_t conn_handle, uint8_t rsp_code);

bool gls_racp_num_response(uint16_t conn_handle, uint16_t num);

void gls_prepare_new_record(void);

void gls_push_new_record(void);

bool gls_report_records_task(uint16_t conn_handle);

void user_face_time(TIMESTAMP time_in, int16_t time_offset, TIMESTAMP time_out);

void gls_abort_racp_procedure(void);

uint16_t glucose_srv_add(void);

void gls_disconnect(uint16_t conn_handle);

void gls_status_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __GLS_SERVICE_H__ */