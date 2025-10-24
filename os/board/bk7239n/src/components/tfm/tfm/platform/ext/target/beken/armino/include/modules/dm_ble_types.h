// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#pragma once

#include <common/bk_typedef.h>
#include "modules/ble_types.h"

#ifdef __cplusplus
extern"C" {
#endif



#ifndef _H_GATT_DEFINES_
#define _H_GATT_DEFINES_

/**
    @brief GATT Attributes
 */
enum
{
    GATT_PRIMARY_SERVICE =            0x2800U,
    GATT_SECONDARY_SERVICE =          0x2801U,
    GATT_INCLUDE =                    0x2802U,
    GATT_CHARACTERISTIC =             0x2803U,
    GATT_EXTENDED_PROPERTIES =        0x2900U,
    GATT_USER_DESCRIPTION =           0x2901U,
    GATT_CLIENT_CONFIG =              0x2902U,
    GATT_SERVER_CONFIG =              0x2903U,
    GATT_FORMAT =                     0x2904U,
    GATT_AGGREGATE_FORMAT =           0x2905U,
    GATT_VALID_RANGE =                0x2906U,
    GATT_EXTERNAL_REPORT_REF =        0x2907U,
    GATT_IP_OP_FEATURE_REPORT_REF =   0x2908U,

};

#if 0
/** GATT Characteristic Properties Bit Field */

/** Broadcast */
#define GATT_CH_PROP_BIT_BROADCAST         0x01U

/** Read */
#define GATT_CH_PROP_BIT_READ              0x02U

/** Write Without Response */
#define GATT_CH_PROP_BIT_WRITE_WO_RSP      0x04U

/** Write */
#define GATT_CH_PROP_BIT_WRITE             0x08U

/** Notify */
#define GATT_CH_PROP_BIT_NOTIFY            0x10U

/** Indicate */
#define GATT_CH_PROP_BIT_INDICATE          0x20U

/** Authenticated Signed Write */
#define GATT_CH_PROP_BIT_SIGN_WRITE        0x40U

/** Extended Properties */
#define GATT_CH_PROP_BIT_EXT_PROPERTY      0x80U
#endif

/**
    @brief GATT Configuration
 */
enum
{
    /** GATT Client Configuration values */
    GATT_CLI_CNFG_NOTIFICATION =         0x0001U,
    GATT_CLI_CNFG_INDICATION =           0x0002U,
    GATT_CLI_CNFG_DEFAULT =              0x0000U,

    /** GATT Server Configuration values */
    GATT_SER_CNFG_BROADCAST =            0x0001U,
    GATT_SER_CNFG_DEFAULT =              0x0000U,
};


/**
    @brief GATT Services
 */
enum
{
    GATT_GAP_SERVICE =                       0x1800U,
    GATT_GATT_SERVICE =                      0x1801U,
    GATT_IMMEDIATE_ALERT_SERVICE =           0x1802U,
    GATT_LINK_LOSS_SERVICE =                 0x1803U,
    GATT_TX_POWER_SERVICE =                  0x1804U,
    GATT_CURRENT_TIME_SERVICE =              0x1805U,
    GATT_REF_TIME_UPDATE_SERVICE =           0x1806U,
    GATT_NEXT_DST_CHANGE_SERVICE =           0x1807U,
    GATT_GLUCOSE_SERVICE =                   0x1808U,
    GATT_HEALTH_THERMOMETER_SERVICE =        0x1809U,
    GATT_DEVICE_INFO_SERVICE =               0x180AU,
    GATT_NWA_SERVICE =                       0x180BU,
    GATT_WATCH_DOG_SERVICE =                 0x180CU,
    GATT_HEART_RATE_SERVICE =                0x180DU,
    GATT_PHONE_ALERT_STATUS_SERVICE =        0x180EU,
    GATT_BATTERY_SERVICE =                   0x180FU,
    GATT_BLOOD_PRESSURE_SERVICE =            0x1810U,
    GATT_ALERT_NOTIFICATION_SERVICE =        0x1811U,
    GATT_HID_SERVICE =                       0x1812U,
    GATT_SCAN_PARAM_SERVICE =                0x1813U,
    GATT_RUNNING_SPEED_AND_CADENCE_SERVICE = 0x1814U,
    GATT_AIOS_SERVICE =                      0x1815U,
    GATT_CYCLING_SPEED_AND_CADENCE_SERVICE = 0x1816U,
    GATT_CPM_SERVICE =                       0x1818U,
    GATT_LOCATION_AND_NAVIGATION_SERVICE =   0x1819U,
    GATT_ENVIRONMENTAL_SENSING_SERVICE =     0x181AU,
    GATT_BODY_COMPOSITION_SERVICE =          0x181BU,
    GATT_USER_DATA_SERVICE =                 0x181CU,
    GATT_WEIGHT_SCALE_SERVICE =              0x181DU,
    GATT_BM_SERVICE =                        0x181EU,
    GATT_CGM_SERVICE =                       0x181FU,
    GATT_INT_PS_SERVICE =                    0x1820U,
    GATT_IPS_SERVICE =                       0x1821U,
    GATT_PULSE_OXIMETER_SERVICE =            0x1822U,
    GATT_HPS_SERVICE =                       0x1823U,
    GATT_TDS_SERVICE =                       0x1824U,
    GATT_OBJECT_TRANSFER_SERVICE =           0x1825U,
    GATT_FTMS_SERVICE =                      0x1826U,
    GATT_RCS_SERVICE =                       0x1829U,
    GATT_INSULIN_DELIVERY_SERVICE =          0x183AU,

    /** Dont find this on .org */
    GATT_FIND_ME_SERVICE =                   0x18A3U,

    /** Still not adopted, may need updation */
    GATT_TPMS_SERVICE =                      0x7FD0U,
};


/**
    @brief GATT Characteristic
 */
enum
{
    GATT_DEVICE_NAME_CHARACTERISTIC =          0x2A00U,
    GATT_APPEARANCE_CHARACTERISTIC =           0x2A01U,
    GATT_PERIPHRL_PRIVCY_FLG_CHARACTERISTIC =  0x2A02U,
    GATT_RECONNECT_ADDR_CHARACTERISTIC =       0x2A03U,
    GATT_PRFRRD_CNXN_PARAM_CHARACTERISTIC =    0x2A04U,
    GATT_SERVICE_CHANGED_CHARACTERISTIC =      0x2A05U,
    GATT_ALERT_LEVEL_CHARACTERISTIC =          0x2A06U,
    GATT_TX_POWER_LEVEL_CHARACTERISTIC =       0x2A07U,
    GATT_DATE_TIME_CHARACTERISTIC =            0x2A08U,
    GATT_DAY_OF_WEEK_CHARACTERISTIC =          0x2A09U,
    GATT_DAY_DATE_TIME_CHARACTERISTIC =        0x2A0AU,
    GATT_EXACT_TIME_100_CHARACTERISTIC =       0x2A0BU,
    GATT_EXACT_TIME_256_CHARACTERISTIC =       0x2A0CU,
    GATT_DST_OFFSET_CHARACTERISTIC =           0x2A0DU,
    GATT_TIME_ZONE_CHARACTERISTIC =            0x2A0EU,
    GATT_LOCAL_TIME_INFO_CHARACTERISTIC =      0x2A0FU,
    GATT_SEC_TIME_ZONE_CHARACTERISTIC =        0x2A10U,
    GATT_TIME_WITH_DST_CHARACTERISTIC =        0x2A11U,
    GATT_TIME_ACCURACY_CHARACTERISTIC =        0x2A12U,
    GATT_TIME_SOURCE_CHARACTERISTIC =          0x2A13U,
    GATT_REF_TIME_INFO_CHARACTERISTIC =        0x2A14U,
    GATT_TIME_BROADCAST_CHARACTERISTIC =       0x2A15U,
    GATT_TIME_UPDATE_CONTROL_POINT =           0x2A16U,
    GATT_TIME_UPDATE_STATE_CHARACTERISTIC =    0x2A17U,
    GATT_GLUCOSE_MSRMNT_CHARACTERISTIC =       0x2A18U,
    GATT_BATTERY_LEVEL_CHARACTERISTIC =        0x2A19U,
    GATT_BATTERY_PWR_ST_CHARCTERISTIC =        0x2A1AU,
    GATT_BATTERY_LEVEL_ST_CHARACTERISTIC =     0x2A1BU,
    GATT_TEMPERATURE_MSMNT_CHARACTERISTIC =    0x2A1CU,
    GATT_TEMPERATURE_TYPE_CHARACTERISTIC =     0x2A1DU,
    GATT_INTERMEDIATE_TEMP_CHARACTERISTIC =    0x2A1EU,
    GATT_MSMNT_INTERVAL_CHARATACTERISTIC =     0x2A21U,
    GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC =    0x2A22U,
    GATT_SYSTEM_ID_CHARACTERISTIC =            0x2A23U,
    GATT_MODEL_NUMBER_CHARACTERISTIC =         0x2A24U,
    GATT_SERIAL_NUMBER_CHARACTERISTIC =        0x2A25U,
    GATT_FIRMWARE_REV_CHARACTERISTIC =         0x2A26U,
    GATT_HARDWARE_REV_CHARACTERISTIC =         0x2A27U,
    GATT_SOFTWARE_REV_CHARACTERISTIC =         0x2A28U,
    GATT_MANUFACTURER_NAME_CHARACTERISTIC =    0x2A29U,
    GATT_REG_CERT_DATA_CHARACTERISTIC =        0x2A2AU,
    GATT_CURRENT_TIME_CHARACTERISTIC =         0x2A2BU,
    GATT_SCAN_REFRESH_CHARACTERISTIC =         0x2A31U,
    GATT_BOOT_KYBRD_OP_CHARACTERISTIC =        0x2A32U,
    GATT_MOUSE_IP_CHARACTERISTIC =             0x2A33U,
    GATT_GL_MSRMT_CTX_CHARACTERISTIC =         0x2A34U,
    GATT_BP_MSRMT_CHARACTERISTIC =             0x2A35U,
    GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC =    0x2A36U,
    GATT_HR_MSRMT_CHARACTERISTIC =             0x2A37U,
    GATT_BODY_SENSOR_LOC_CHARACTERISTIC =      0x2A38U,
    GATT_HR_CNTRL_POINT =                      0x2A39U,
    GATT_NWA_CHARACTERISTIC =                  0x2A3EU,
    GATT_ALERT_STATUS_CHARACTERISTIC =         0x2A3FU,
    GATT_RINGER_CONTROL_POINT_CHARACTERISTIC = 0x2A40U,
    GATT_RINGER_SETTING_CHARACTERISTIC =       0x2A41U,
    GATT_ALERT_ID_BIT_MASK_CHARACTERISTIC =    0x2A42U,
    GATT_ALERT_ID_CHARACTERISTIC =             0x2A43U,
    GATT_ALERT_NTF_CONTROL_POINT =             0x2A44U,
    GATT_UNREAD_ALRT_STATUS_CHARACTERISTIC =   0x2A45U,
    GATT_NEW_ALERT_CHARACTERISTIC =            0x2A46U,
    GATT_SPRTD_NEW_ALRT_CTGRY_CHARACTERISTIC = 0x2A47U,
    GATT_SPRT_UNRD_ALRT_CTGRY_CHARACTERISTIC = 0x2A48U,
    GATT_BP_FEATURE_CHARACTERISTIC =           0x2A49U,
    GATT_HID_INFO_CHARACTERISTIC =             0x2A4AU,
    GATT_HID_RPT_MAP_CHARACTERISTIC =          0x2A4BU,
    GATT_HID_CP_CHARACTERISTIC =               0x2A4CU,
    GATT_HID_RPT_CHARACTERISTIC =              0x2A4DU,
    GATT_HID_PROTO_MODE_CHARACTERISTIC =       0x2A4EU,
    GATT_SCAN_WINDOW_CHARACTERISTIC =          0x2A4FU,
    GATT_PNP_ID_CHARACTERISTIC =               0x2A50U,
    GATT_GL_FEATURE_CHARACTERISTIC =           0x2A51U,
    GATT_RACP_CHARACTERISTIC =                 0x2A52U,
    GATT_RSC_MSRMT_CHARACTERISTIC =            0x2A53U,
    GATT_RSC_FEATURE_CHARACTERISTIC =          0x2A54U,
    GATT_SC_CONTROL_POINT_CHARACTERISTIC =     0x2A55U,
    GATT_CSC_MSRMT_CHARACTERISTIC =            0x2A5BU,
    GATT_CSC_FEATURE_CHARACTERISTIC =          0x2A5CU,
    GATT_SENSOR_LOCATION_CHARACTERISTIC =      0x2A5DU,

    /** Still Not Adopted, may need updation
        Pulse Oximeter Characteristics defines */
    GATT_POX_SPOT_CHK_MSRMT_CHARACTERISTIC =   0x2A5EU,
    GATT_POX_CONTINUOUS_MSRMT_CHARACTERISTIC = 0x2A5FU,
    GATT_POX_PULSATILE_EVENT_CHARACTERISTIC =  0x2A61U,
    GATT_POX_FEATURES_CHARACTERISTIC =         0x2A60U,
    GATT_POX_CONTROL_POINT_CHARACTERISTIC =    0x2A62U,

    /** CPM Characteristics Measurments */
    GATT_CPM_MSRMT_CHARACTERISTIC =            0x2A63U,
    GATT_CPM_VECTOR_CHARACTERISTIC =           0x2A64U,
    GATT_CPM_FEATURE_CHARACTERISTIC =          0x2A65U,
    GATT_CPM_CNTRL_PNT_CHARACTERISTIC =        0x2A66U,
    GATT_CPM_SENSOR_LOCATION_CHARACTERISTIC =  0x2A5DU,

    /** Location and Navigation Characteristics */
    GATT_LOCATION_AND_SPEED_CHARACTERISTIC =   0x2A67U,
    GATT_NAVIGATION_CHARACTERISTIC =           0x2A68U,
    GATT_LN_POSITION_QUALITY_CHARACTERISTIC =  0x2A69U,
    GATT_LN_FEATURE_CHARACTERISTIC =           0x2A6AU,
    GATT_LN_CNTRL_PNT_CHARACTERISTIC =         0x2A6BU,

    /** CGM Characteristics defines */
    GATT_CGM_MSRMT_CHARACTERISTIC =            0x2AA7U,
    GATT_CGM_FEATURES_CHARACTERISTIC =         0x2AA8U,
    GATT_CGM_STATUS_CHARACTERISTIC =           0x2AA9U,
    GATT_CGM_SSN_STRT_TIME_CHARACTERISTIC =    0x2AAAU,
    GATT_CGM_SSN_RUN_TIME_CHARACTERISTIC =     0x2AABU,
    GATT_CGM_SPECIFIC_OPS_CP_CHARACTERISTIC =  0x2AACU,

    GATT_BATTERY_REMOVABLE_CHARACTERISTIC =    0x2A3AU,
    GATT_BATTERY_SERV_REQRD_CHARACTERISTIC =   0x2A3BU,

    /** Weight Scale Characteristics define */
    GATT_WS_MSRMT_CHARACTERISTIC =             0x2A9DU,
    GATT_WS_FEATURE_CHARACTERISTIC =           0x2A9EU,

    /** Body Composition Characteristics define */
    GATT_BC_MSRMT_CHARACTERISTIC =             0x2A9CU,
    GATT_BC_FEATURE_CHARACTERISTIC =           0x2A9BU,

    /** User Name Characterictics define */
    GATT_UD_FIRST_NAME_CHARACTERISTIC =        0x2A8AU,
    GATT_UD_LAST_NAME_CHARACTERISTIC =         0x2A90U,
    GATT_UD_EMAIL_CHARACTERISTIC =             0x2A87U,
    GATT_UD_AGE_CHARACTERISTIC =               0x2A80U,
    GATT_UD_GENDER_CHARACTERISTIC =            0x2A8CU,
    GATT_UD_DB_CHNG_INC_CHARACTERISTIC =       0x2A99U,
    GATT_UD_DOB_CHARACTERISTIC =               0x2A85U,
    GATT_UD_WEIGHT_CHARACTERISTIC =            0x2A98U,
    GATT_UD_HEIGHT_CHARACTERISTIC =            0x2A8EU,
    GATT_UD_VO2MAX_CHARACTERISTIC =            0x2A96U,
    GATT_UD_HRMAX_CHARACTERISTIC =             0x2A8DU,
    GATT_UD_RHR_CHARACTERISTIC =               0x2A92U,
    GATT_UD_MAXRHR_CHARACTERISTIC =            0x2A91U,
    GATT_UD_AEROTH_CHARACTERISTIC =            0x2A7FU,
    GATT_UD_ANAETH_CHARACTERISTIC =            0x2A83U,
    GATT_UD_SPORTTYPE_CHARACTERISTIC =         0x2A93U,
    GATT_UD_DATETHASS_CHARACTERISTIC =         0x2A86U,
    GATT_UD_WAISTCIR_CHARACTERISTIC =          0x2A97U,
    GATT_UD_HIPCIR_CHARACTERISTIC =            0x2A8FU,
    GATT_UD_FATBURN_LOW_CHARACTERISTIC =       0x2A88U,
    GATT_UD_FATBURN_UPL_CHARACTERISTIC =       0x2A89U,
    GATT_UD_AERO_LOW_CHARACTERISTIC =          0x2A7EU,
    GATT_UD_AERO_UPL_CHARACTERISTIC =          0x2A84U,
    GATT_UD_ANAE_LOW_CHARACTERISTIC =          0x2A81U,
    GATT_UD_ANAE_UPL_CHARACTERISTIC =          0x2A82U,
    GATT_UD_FIVEZONE_HRL_CHARACTERISTIC =      0x2A8BU,
    GATT_UD_THREEZONE_HRL_CHARACTERISTIC =     0x2A94U,
    GATT_UD_TWOZONE_HRL_CHARACTERISTIC =       0x2A95U,
    GATT_UD_USER_INDEX_CHARACTERISTIC =        0x2A9AU,
    GATT_UD_USER_CNTRL_PNT_CHARACTERISTIC =    0x2A9FU,
    GATT_UD_LANGUAGE_CHARACTERISTIC =          0x2AA2U,
    GATT_UD_REGISTERED_USER_CHARACTERISTIC =   0x2B37U,

    /** Environmental Sensing Characterisitcs define */
    GATT_ESS_DVC_CHARACTERISTIC =              0x2A7DU,
    GATT_ESS_TEMP_CHARACTERISTIC =             0x2A6EU,
    GATT_ES_MEASURMENT_DESCRIPTOR =            0x290CU,
    GATT_ES_TRIGGER_SETTING_DESCRIPTOR =       0x290DU,
    GATT_ES_CONFIGURATION_DESCRIPTOR =         0x290BU,
    GATT_ES_MEASUREMENT_DESCRIPTOR =           0x290CU,

    /** Bond Management Characteritiscs defines */
    GATT_BM_FEATURES_CHARACTERISTIC =          0x2AA5U,
    GATT_BM_CNTRL_PNT_CHARACTERISTIC =         0x2AA4U,

    /** Automation IO Service Characteritiscs defines */
    GATT_AIO_DIGITAL_CHARACTERISTIC =          0x2A56U,
    GATT_AIO_ANALOG_CHARACTERISTIC =           0x2A58U,
    GATT_AIO_AGGREGATE_CHARACTERISTIC =        0x2A5AU,

    /** Indoor Positioning Characteritiscs defines */
    GATT_IP_INDOOR_POS_CONFIG_CHARACTERISTIC = 0x2AADU,
    GATT_IP_LATITUDE_CHARACTERISTIC =          0x2AAEU,
    GATT_IP_LONGITUDE_CHARACTERISTIC =         0x2AAFU,
    GATT_IP_LOCAL_NORTH_CHARACTERISTIC =       0x2AB0U,
    GATT_IP_LOCAL_EAST_CHARACTERISTIC =        0x2AB1U,
    GATT_IP_FLOOR_NUM_CHARACTERISTIC =         0x2AB2U,
    GATT_IP_ALTITUDE_CHARACTERISTIC =          0x2AB3U,
    GATT_IP_UNCERTAINTY_CHARACTERISTIC =       0x2AB4U,
    GATT_IP_LOCAL_NAME_CHARACTERISTIC =        0x2AB5U,

    /** Object Transfer Characteritics defines */
    GATT_OTS_FEATURE_CHARACTERISTIC =          0x2ABDU,
    GATT_OTS_OBJ_NAME_CHARACTERISTIC =         0x2ABEU,
    GATT_OTS_OBJ_TYPE_CHARACTERISTIC =         0x2ABFU,
    GATT_OTS_OBJ_SIZE_CHARACTERISTIC =         0x2AC0U,
    GATT_OTS_OBJ_FIRST_CRTD_CHARACTERISTIC =   0x2AC1U,
    GATT_OTS_OBJ_LAST_MODFD_CHARACTERISTIC =   0x2AC2U,
    GATT_OTS_OBJ_ID_CHARACTERISTIC =           0x2AC3U,
    GATT_OTS_OBJ_PRPTY_CHARACTERISTIC =        0x2AC4U,
    GATT_OTS_OACP_CHARACTERISTIC =             0x2AC5U,
    GATT_OTS_OLCP_CHARACTERISTIC =             0x2AC6U,
    GATT_OTS_OBJ_LIST_FILTER_CHARACTERISTIC =  0x2AC7U,
    GATT_OTS_OBJ_CHANGED_CHARACTERISTIC =      0x2AC8U,
    GATT_OTS_LE_PSM_CHARACTERISTIC =           0x7FBDU,
    GATT_OTS_OBJ_LAST_ACCSD_CHARACTERISTIC =   0x7FE3U,
    GATT_OTS_OBJ_CHECKSUM_CHARACTERISTIC =     0x7FE2U,
    GATT_OTS_OCTECT_OFFSET_CHARACTERISTIC =    0x7FE1U,

    /** Tire Pressure Monitoring Characteritiscs defines
    Needs to update the opcode once TPMS spec is adopted  */
    GATT_TPM_TIRE_PRESSURE_CHARACTERISTIC =    0x1111U,
    GATT_TPM_TIRE_TEMP_CHARACTERISTIC =        0x2222U,
    GATT_TPM_PRESSURE_CALIBN_CHARACTERISTIC =  0x3333U,
    GATT_TPM_TEMP_CALIBN_CHARACTERISTIC =      0x4444U,
    GATT_TPM_DISRY_CONN_MGMNT_CHARACTERISTIC = 0xFFFFU,
    GATT_TPM_ADV_PERIOD_CHARACTERISTIC =       0xAAA1U,

    /** Transport Discovery Characteristics defines */
    GATT_TD_CONTROL_POINT_CHARACTERISTIC =     0x2ABCU,

    /** HTTP Proxy Characteristics defines */
    GATT_HPC_URI_CHARACTERISTIC =              0x2AB6U,
    GATT_HPC_HTTP_HEADERS_CHARACTERISTIC =     0x2AB7U,
    GATT_HPC_HTTP_ENTITY_BODY_CHARACTERISTIC = 0x2AB9U,
    GATT_HPC_HTTP_CP_CHARACTERISTIC =          0x2ABAU,
    GATT_HPC_HTTP_STATUS_CODE_CHARACTERISTIC = 0x2AB8U,
    GATT_HPC_HTTPS_SECURITY_CHARACTERISTIC =   0x2ABBU,

    /** Fitness Machine Characteristics defines */
    GATT_FTM_FEATURE_CHARACTERISTIC =          0x2ACCU,
    GATT_FTM_TREADMILL_CHARACTERISTIC =        0x2ACDU,
    GATT_FTM_CROSS_TRAINER_CHARACTERISTIC =    0x2ACEU,
    GATT_FTM_STEP_CLIMBER_CHARACTERISTIC =     0x2ACFU,
    GATT_FTM_STAIR_CLIMBER_CHARACTERISTIC =    0x2AD0U,
    GATT_FTM_ROWER_DATA_CHARACTERISTIC =       0x2AD1U,
    GATT_FTM_INDOOR_BIKE_CHARACTERISTIC =      0x2AD2U,
    GATT_FTM_TRAINING_STATUS_CHARACTERISTIC =  0x2AD3U,
    GATT_FTM_SUPPD_SPEED_RNG_CHARACTERISTIC =  0x2AD4U,
    GATT_FTM_SUPPD_INCLN_RNG_CHARACTERISTIC =  0x2AD5U,
    GATT_FTM_SUPPD_RESIS_RNG_CHARACTERISTIC =  0x2AD6U,
    GATT_FTM_SUPPD_POWER_RNG_CHARACTERISTIC =  0x2AD8U,
    GATT_FTM_SUPPD_HRTRE_RNG_CHARACTERISTIC =  0x2AD7U,
    GATT_FTM_CONTROL_POINT_CHARACTERISTIC =    0x2AD9U,
    GATT_FTM_MACHINE_STATUS_CHARACTERISTIC =   0x2ADAU,

    /** Reconnection Configuration Characteristic defines */
    GATT_RC_FEATURE_CHARACTERISTIC =           0x2B1DU,
    GATT_RC_SETTING_CHARACTERISTIC =           0x2B1EU,
    GATT_RC_CP_CHARACTERISTIC =                0x2B1FU,

    /** Insulin Delivery Characteristics defines */
    GATT_IDD_STATUS_CHANGED_CHARACTERISTIC =   0x2B20U,
    GATT_IDD_STATUS_CHARACTERISTIC =           0x2B21U,
    GATT_IDD_ANNUN_STATUS_CHARACTERISTIC =     0x2B22U,
    GATT_IDD_FEATURE_CHARACTERISTIC =          0x2B23U,
    GATT_IDD_STATUS_READER_CP_CHARACTERISTIC = 0x2B24U,
    GATT_IDD_COMMAND_CP_CHARACTERISTIC =       0x2B25U,
    GATT_IDD_COMMAND_DATA_CHARACTERISTIC =     0x2B26U,
    GATT_IDD_RACP_CHARACTERISTIC =             0x2B27U,
    GATT_IDD_HISTORY_DATA_CHARACTERISTIC =     0x2B28U,

};

#define ATT_APPL_PROCEDURE_ALREADY_IN_PROGRESS          0x80U
#define ATT_APPL_CCD_IMPROPERLY_CONFIGURED              0x81U


#endif /**_H_GATT_DEFINES_ */






#ifndef _H_BT_ERROR_
#define _H_BT_ERROR_

typedef uint16_t API_RESULT;

#define GATT_DB_ERR_ID                          0x6B00U
/**Not an error code. More of a status code */
#define GATT_DB_DELAYED_RESPONSE                 (0x0060U | GATT_DB_ERR_ID)
#define GATT_DB_DONOT_RESPOND                    (0x0061U | GATT_DB_ERR_ID)
#define GATT_DB_ALREADY_RESPONDED                (0x0062U | GATT_DB_ERR_ID)

#endif

#ifndef _H_BT_DEVICE_QUEUE_
#define _H_BT_DEVICE_QUEUE_

typedef uint8_t DEVICE_HANDLE;

#endif


#ifndef _H_BT_ATT_API_
#define _H_BT_ATT_API_

typedef uint8_t ATT_CON_ID;

/**
 * @brief ble att protocol handle
 * */
typedef uint16_t ATT_ATTR_HANDLE;

/** Abstracts 16 Bit UUID */
typedef uint16_t ATT_UUID16;

/** ATT Default MTU */
//#define ATT_DEFAULT_MTU                   23U

/** ATT Signature Size in Signed Write */
#define ATT_AUTH_SIGNATURE_SIZE           12U

/** ATT Execute Write Cancel Flag */
#define ATT_EXECUTE_WRITE_CANCEL_FLAG   0x00U

/** ATT Execute Write Execute Flag */
#define ATT_EXECUTE_WRITE_EXEC_FLAG     0x01U

/** ATT Identification for 16-bit UUID Format */
#define ATT_16_BIT_UUID_FORMAT          0x01U

/** ATT Identification for 128-bit UUID Format */
#define ATT_128_BIT_UUID_FORMAT         0x02U

/** ATT 16-bit UUID Size */
#define ATT_16_BIT_UUID_SIZE               2U

/** ATT 128-bit UUID Size */
#define ATT_128_BIT_UUID_SIZE             16U

/** ATT Connection Instance Initialization Value. */
#define ATT_CON_ID_INIT_VAL             0xFFU

/** ATT Application Callback Initialization Value. */
#define ATT_APPL_CB_INIT_VAL               NULL

/** ATT Invalid Attribute Handle Value */
#define ATT_INVALID_ATTR_HANDLE_VAL      0x0000U

/** ATT Attribute Handle Start Range */
#define ATT_ATTR_HANDLE_START_RANGE      0x0001U

/** ATT Attribute Handle End Range */
#define ATT_ATTR_HANDLE_END_RANGE        0xFFFFU

/**
    @brief Abstracts 128 Bit UUID
 */
typedef struct
{
    uint8_t   value[ATT_128_BIT_UUID_SIZE];
} ATT_UUID128;


/**
    @brief att uuid struct
 */
typedef union
{
    ATT_UUID16    uuid_16;
    ATT_UUID128   uuid_128;

} ATT_UUID;

/**
    @brief att value struct
 */
typedef struct
{
    /** Value to be packed */
    uint8_t     *val;

    /** Length of Value */
    uint16_t   len;

    /** Out Parameter Indicating Actual Length Packed */
    uint16_t   actual_len;

} ATT_VALUE;

/**
 *  @brief Abstracts Handle Value Pair
 *  This is used in multiple PDUs - see \ref ATT_WRITE_REQ_PARAM,
 *  \ref ATT_WRITE_CMD_PARAM etc.
 */
typedef struct
{
    /** Attribute Value  */
    ATT_VALUE          value;

    /** Attribute Handle */
    ATT_ATTR_HANDLE    handle;

} ATT_HANDLE_VALUE_PAIR;


/**
    @brief ATT Handle
 */
typedef struct
{
    /** Identifies the peer instance */
    DEVICE_HANDLE     device_id;

    /** Identifies the ATT Instance */
    ATT_CON_ID        att_id;

} ATT_HANDLE;


/**
    @brief ATT range
 */
typedef struct
{
    /** Start Handle */
    ATT_ATTR_HANDLE    start_handle;

    /** End Handle */
    ATT_ATTR_HANDLE    end_handle;

} ATT_HANDLE_RANGE;



#endif



#ifndef _H_BT_GATT_DB_API_
#define _H_BT_GATT_DB_API_

/**
 * such as GATT_DB_CHAR_VALUE_READ_REQ
 * */
typedef uint8_t GATT_DB_OPERATION;

/**
    @brief only use for @ref gatt_db_operations
 */
enum
{
    /** Read operation for Attribute  */
    GATT_DB_READ =                             0x01U,

    /** Write operation for Attribute */
    GATT_DB_WRITE =                            0x02U,

    /** Read blob operation for Attribute */
    GATT_DB_READ_BLOB =                        0x03U,

    /** Write Without Response operation for Attribute */
    GATT_DB_WRITE_WITHOUT_RSP =                0x04U,

    /** Read by UUID/Type operation for Attribute */
    GATT_DB_READ_BY_TYPE =                     0x05U,

    /** Signed Write operation for Attribute */
    GATT_DB_SIGNED_WRITE =                     0x06U,

    /** Execute Write operation for Attribute */
    GATT_DB_EXECUTE =                          0x07U,

    /** Prepare Write operation for Attribute */
    GATT_DB_PREPARE =                          0x08U,

    /**
     *  Local update of a Readable Attribute Value using \ref GATT_DB_HANDLE instead
     *  of Attribute Handle. See \ref BT_gatt_db_set_char_val for more details.
     */
    GATT_DB_UPDATE =                           0x10U,

    /**128 Bit Attribute UUID  */
    GATT_DB_128_BIT_UUID_FORMAT =              0x20U,

    /**
     *  Peer initiated operation, needed when Peer is Reading or Writing Values
     *  either using GATT Read Procedures for Value and Descriptors.
     */
    GATT_DB_PEER_INITIATED =                   0x80U,

    /**
     *  Locally initiated operation, needed for local updates based on Attribute
     *  Handles. In case \ref GATT_DB_HANDLE is known, use of access through
     *  \ref GATT_DB_UPDATE is recommended.
     */
    GATT_DB_LOCALLY_INITIATED =                0x00U,
};


/**
 *  @brief Operations Notified In Characteristic Callback
 *  \defgroup gatt_db_operations Operations Notified In Characteristic Callback
 *  \{
 *  This section describes the operations notified by the module to the
 *  application through the callback registered by the application.
 */

enum
{
    /** Characteristic Value Local Read Operation */
    GATT_DB_CHAR_VALUE_READ_REQ =             0x01U,

    /** Characteristic Value Local Write Operation */
    GATT_DB_CHAR_VALUE_WRITE_REQ =            0x02U,

    /** Characteristic Value Local Write Without Response Operation */
    GATT_DB_CHAR_VALUE_WRITE_WITHOUT_REQ =    0x03U,

    /** Characteristic Client Configuration Local Read Operation */
    GATT_DB_CHAR_CLI_CNFG_READ_REQ =          0x11U,

    /** Characteristic Client Configuration Local Write Operation */
    GATT_DB_CHAR_CLI_CNFG_WRITE_REQ =         0x12U,

    /** Characteristic Server Configuration Local Read Operation */
    GATT_DB_CHAR_SER_CNFG_READ_REQ =          0x21U,

    /** Characteristic Server Configuration Local Write Operation */
    GATT_DB_CHAR_SER_CNFG_WRITE_REQ =         0x22U,

    /** Characteristic Value Peer Read Operation */
    GATT_DB_CHAR_PEER_READ_REQ =              (GATT_DB_PEER_INITIATED | GATT_DB_READ),

    /** Characteristic Value Peer Write Operation */
    GATT_DB_CHAR_PEER_WRITE_REQ =             (GATT_DB_PEER_INITIATED | GATT_DB_WRITE),

    /** Characteristic Value Peer Read Blob Operation */
    GATT_DB_CHAR_PEER_READ_BLOB_REQ =         (GATT_DB_PEER_INITIATED | GATT_DB_READ_BLOB),

    /** Characteristic Value Peer Write Command */
    GATT_DB_CHAR_PEER_WRITE_CMD =             (GATT_DB_PEER_INITIATED | GATT_DB_WRITE_WITHOUT_RSP),

    /** Characteristic Value Peer Read by Type Operation */
    GATT_DB_CHAR_PEER_READ_BY_TYPE_REQ =      (GATT_DB_PEER_INITIATED | GATT_DB_READ_BY_TYPE),

    /** Characteristic Value Peer Signed Write Operation */
    GATT_DB_CHAR_PEER_SIGNED_WRITE_CMD =      (GATT_DB_PEER_INITIATED | GATT_DB_SIGNED_WRITE),

    /** Characteristic Value Peer Execute Write Req */
    GATT_DB_CHAR_PEER_EXECUTE_WRITE_REQ =     (GATT_DB_PEER_INITIATED | GATT_DB_EXECUTE),

    /** Characteristic Value Peer Prepare Write Req */
    GATT_DB_CHAR_PEER_PREPARE_WRITE_REQ =     (GATT_DB_PEER_INITIATED | GATT_DB_PREPARE),

    /** Characteristic Client Configuration Peer Read Operation */
    GATT_DB_CHAR_PEER_CLI_CNFG_READ_REQ =     0x91U,

    /** Characteristic Client Configuration Peer Write Operation */
    GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ =    0x92U,

    /** Characteristic Server Configuration Peer Read Operation */
    GATT_DB_CHAR_PEER_SER_CNFG_READ_REQ =     0xA1U,

    /** Characteristic Server Configuration Peer Write Operation */
    GATT_DB_CHAR_PEER_SER_CNFG_WRITE_REQ =    0xA2U,

    /** Characteristic User Description Peer Read Operation */
    GATT_DB_CHAR_PEER_USR_DESC_READ_REQ =     0xB1U,

    /** Characteristic User Description Peer Write Operation */
    GATT_DB_CHAR_PEER_USR_DESC_WRITE_REQ =    0xB2U,

    /** Characteristic Higher Layer Defined Descriptor Peer Read Operation */
    GATT_DB_CHAR_PEER_HLD_DESC_READ_REQ =     0xF1U,

    /** Characteristic Higher Layer Defined Descriptor Peer Write Operation */
    GATT_DB_CHAR_PEER_HLD_DESC_WRITE_REQ =    0xF2U,
};

/** \} */

/**
 * @brief Characteristic Properties
 *  \defgroup gatt_characteristic_properties Characteristic Properties
 *  \{
 *  Characteristic Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Characteristic. Also
 *  see \ref GATT_DB_ATTR_LIST.
 */
enum
{
    /**
     *  Characteristic support Broadcast of its value to the peer.
     *  Setting this property automatically includes Characteristic Server
     *  Configuration Descriptor to the Characteristic
     */
    GATT_DB_CHAR_BROADCAST_PROPERTY =                0x00000001U,

    /** Characteristic support Reading its value by peer */
    GATT_DB_CHAR_READ_PROPERTY =                     0x00000002U,

    /** Characteristic support Writing its value by peer Without Response  */
    GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY =        0x00000004U,

    /** Characteristic supports Writing its value by peer */
    GATT_DB_CHAR_WRITE_PROPERTY =                    0x00000008U,

    /**
     *  Characteristic supports Notifying its value to the peer.
     *  Setting this property automatically includes Characteristic Server
     *  Configuration Descriptor to the Characteristic
     */
    GATT_DB_CHAR_NOTIFY_PROPERTY =                   0x00000010U,

    /** Characteristic supports Indicating its value to the peer */
    GATT_DB_CHAR_INDICATE_PROPERTY =                 0x00000020U,

    /** Characteristic supports Signed Write on its value */
    GATT_DB_CHAR_SIGNED_WRITE_PROPERTY =             0x00000040U,

    /** Characteristic supports write on its User Description Descriptor */
    GATT_DB_CHAR_WRIEABLE_AUX_PROPERTY =             0x00000200U,

    /** No Auxillary Property */
    GATT_DB_NO_AUXILLARY_PROPERTY =                  0x00U,

    /** Characteristic Value is Fixed Length */
    GATT_DB_FIXED_LENGTH_PROPERTY =                  0x01U,

    GATT_DB_AUTHORIZATION_PROPERTY =                 0x02U,

    GATT_DB_PEER_SPECIFIC_VAL_PROPERTY =             0x40U,

    GATT_DB_CONST_ATTR_VAL_PROPERTY =                0x80U,
};

/** \} */

/**
 * @brief GATT Characteristic and Descriptor permissions
 * */
enum
{
    GATT_DB_PERM_NONE =                    0x0000U,
    GATT_DB_PERM_READ =                    0x0001U,
    GATT_DB_PERM_READ_ENCRYPTED =          0x0002U,
    GATT_DB_PERM_READ_ENCRYPTED_MITM =     0x0004U,
    GATT_DB_PERM_WRITE =                   0x0010U,
    GATT_DB_PERM_WRITE_ENCRYPTED =         0x0020U,
    GATT_DB_PERM_WRITE_ENCRYPTED_MIMT =    0x0040U,
    GATT_DB_PERM_WRITE_SIGNED =            0x0080U,
    GATT_DB_PERM_WRITE_SIGNED_MITM =       0x0100U,
};


/**
 * @brief Service Properties
 *  \defgroup gatt_service_properties Service Properties
 *  \{
 *  Service Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Service. Also
 *  see \ref GATT_DB_SERVICE_DESC.
 */
enum
{
    /**
     *  Security related - combination of Level, Mode and Encryption Key Size
     *  (if applicable) describes complete security needs
     */
    /** Service employs Level 0 */
    GATT_DB_SER_SECURITY_LEVEL0 =                    0x00000000U,

    /** Service employs Level 1 */
    GATT_DB_SER_SECURITY_LEVEL1 =                    0x00000001U,

    /** Service employs Level 2 */
    GATT_DB_SER_SECURITY_LEVEL2 =                    0x00000002U,

    /** Service employs Level 3 */
    GATT_DB_SER_SECURITY_LEVEL3 =                    0x00000004U,

    /** Service employs Mode 1 */
    GATT_DB_SER_SECURITY_MODE1 =                     0x00000010U,

    /** Service employs Mode 2 */
    GATT_DB_SER_SECURITY_MODE2 =                     0x00000020U,

    /** ignore_this Unused right now */
    GATT_DB_SER_MULTIPLE_CLIENTS_SUPPORT =           0x00000040U,

    /**
     *  Service is a Secondary Service. If this not used, Service is by default
     *  considered to be Primary
     */
    GATT_DB_SER_SECONDARY_SERVICE_PROPERTY =         0x00000080U,

#if 1//def GATT_DB_SUPPORT_128_BIT_UUID

    /** Service UUID is 128-bit */
    GATT_DB_SER_128_BIT_UUID_FORMAT =                0x00008000U,

#endif //GATT_DB_SUPPORT_128_BIT_UUID

    /** Set this property to relax all Security on the Service */
    GATT_DB_SER_NO_SECURITY_PROPERTY =               0x00000000U,

    /**
     *  This is employed to describe Service is not rigid on any Encryption
     *  Key Size. Any Size used by other Services or on the whole by the device
     *  will do
     */
    GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE =             0x00000000U,

    /** Encyrption Key Size 7 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_7 =                 0x01000000U,

    /** Encyrption Key Size 8 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_8 =                 0x02000000U,

    /** Encyrption Key Size 9 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_9 =                 0x03000000U,

    /** Encyrption Key Size 10 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_10 =                0x04000000U,

    /** Encyrption Key Size 11 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_11 =                0x05000000U,

    /** Encryption Key Size 12 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_12 =                0x06000000U,

    /** Encryption Key Size 13 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_13 =                0x07000000U,

    /** Encryption Key Size 14 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_14 =                0x08000000U,

    /** Encryption Key Size 15 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_15 =                0x09000000U,

    /** Encryption Key Size 16 Needed for the Service */
    GATT_DB_SER_ENCRYPT_KEY_SIZE_16 =                0x0A000000U,

    /** \cond ignore_this unused */
    GATT_DB_SER_NO_ENCRYPT_PROPERTY =                0x00000000U,

    /**
     *  Macros to describe the Transport access of the Service over BR/EDR only,
     *  LE only or Any Transport.
     */
    /** Service accessible over BR/EDR Link Only */
    GATT_DB_SER_SUPPORT_BR_LINK_TYPE =               0x10000000U,

    /** Service accessible over LE Link Only */
    GATT_DB_SER_SUPPORT_LE_LINK_TYPE =               0x20000000U,

    /** Service accessible over Any Link */
    GATT_DB_SER_SUPPORT_ANY_LINK_TYPE =              0xF0000000U,
    /** \endcond */
};

/** \} */


/**
 * @brief such as @ref GATT_DB_SER_NO_SECURITY_PROPERTY @ref GATT_DB_SER_SUPPORT_ANY_LINK_TYPE
 * */
typedef uint32_t GATT_DB_SERVICE_DESC;


/**
    @brief gatt db uuid type
 */
typedef struct
{
    /// Format indicating, 16 bit or 128 bit UUIDs, see @ref ATT_16_BIT_UUID_FORMAT @ref ATT_128_BIT_UUID_FORMAT
    uint8_t              uuid_format;

    /// Attribute UUID
    ATT_UUID           uuid;

} GATT_DB_UUID_TYPE;

/**
    @brief used in @ref bk_ble_gatt_db_add_service
 */
typedef struct
{
    /** UUID, see @ref GATT_DB_UUID_TYPE */
    GATT_DB_UUID_TYPE    uuid;

    /** If this service is primary or not */
    uint8_t                is_primary;

    /**
     * Security Requrirements for the Service
     * Logical OR of desired combinations of
     * 1. Security Mode
     * 2. Security Level
     * 3. Encryption Key Size
     *
     * Acceptable values for Security Mode are:
     * \ref GATT_DB_SER_SECURITY_MODE1
     * \ref GATT_DB_SER_SECURITY_MODE2
     *
     * Acceptable values for Security Level are:
     * \ref GATT_DB_SER_SECURITY_LEVEL1
     * \ref GATT_DB_SER_SECURITY_LEVEL2
     * \ref GATT_DB_SER_SECURITY_LEVEL3
     *
     * Acceptable values for Encryption Key Size are:
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_7
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_8
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_9
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_10
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_11
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_12
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_13
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_14
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_15
     * \ref GATT_DB_SER_ENCRYPT_KEY_SIZE_16
     * When the Service does not mandate any specific Key size
     * \ref GATT_DB_SER_ENC_KEY_SIZE_DONT_CARE shall be used.
     *
     * When No Security is mandated for the service
     * Value \ref GATT_DB_SER_NO_SECURITY_PROPERTY shall be used.
     */
    GATT_DB_SERVICE_DESC sec_req;

    /**
     * Transport Requrirements for the Service.
     * This describes the Transport on which this required
     * to be operational.
     * \ref GATT_DB_SER_SUPPORT_ANY_LINK_TYPE
     * \ref GATT_DB_SER_SUPPORT_LE_LINK_TYPE
     * \ref GATT_DB_SER_SUPPORT_BR_LINK_TYPE
     */
    GATT_DB_SERVICE_DESC link_req;

} GATT_DB_SERVICE_INFO;



/**
 * @brief GATT db handle
 **/
typedef struct
{
    /** Identifies the peer accessing the database */
    DEVICE_HANDLE    device_id;

    /** Identifies the Service being Accessed */
    uint8_t            service_id;

    /** Identifies the Characteristic being Accessed */
    uint8_t            char_id;

} GATT_DB_HANDLE;

/**
 * @brief GATT db param
 **/
typedef struct
{
    /** Abstracts Value to get/set the Attribute Value */
    ATT_VALUE            value;

    /** Abstracts the handle information */
    ATT_ATTR_HANDLE      handle;

    /** Abstracts the Access Operation, such as @ref GATT_DB_CHAR_PEER_READ_REQ
     * @attention when recv GATT_DB_CHAR_PEER_READ_REQ and you dont want response immediately,  return GATT_DB_DELAYED_RESPONSE in @ref ble_gatt_db_callback_t
     *
     * */
    GATT_DB_OPERATION    db_op;

#if 1//def GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT
    uint16_t               offset;
#endif //GATT_DB_HAVE_OFFSET_IN_PARAMS_SUPPORT

} GATT_DB_PARAMS;



#endif


#ifndef _H_GATT_
#define _H_GATT_

#define GATT_MAX_CHAR_DESCRIPTORS 6U //todo: remove, use drnay array

/**
    @brief gatt Service param
 */
typedef struct
{
    /**Service UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /**Service UUID Type */
    uint8_t uuid_type;
#endif //ATT_128_BIT_UUID_FORMAT

    /**Service Range */
    ATT_HANDLE_RANGE range;

} GATT_SERVICE_PARAM;

/**
    @brief gatt Characteristic Descriptor Information
 */
typedef struct
{
    /**Descriptor handle */
    ATT_ATTR_HANDLE handle;

    /**Descriptor UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /**Descriptor UUID Type */
    uint8_t uuid_type;
#endif //ATT_128_BIT_UUID_FORMAT

} GATT_CHAR_DESC_PARAM;


/**
    @brief gatt Characteristic Information
 */
typedef struct
{
    /**Characteristic Range */
    ATT_HANDLE_RANGE range;

    /**Characteristic Property */
    uint8_t cproperty;

    /**Characteristic Value Handle */
    ATT_ATTR_HANDLE value_handle;

    /**Characteristic UUID */
    ATT_UUID uuid;

#if 1//def ATT_128_BIT_UUID_FORMAT
    /**Characteristic UUID Type */
    uint8_t uuid_type;
#endif /**ATT_128_BIT_UUID_FORMAT */

    /**Characteristic desciptor index*/
    uint8_t desc_index;

    /**Characteristics Value Offset */
    uint16_t val_offset;

    /**Characteristic Value Length */
    uint16_t val_length;

    /**Characteristic descriptor array */
    GATT_CHAR_DESC_PARAM descriptor[GATT_MAX_CHAR_DESCRIPTORS];
} GATT_CHARACTERISTIC_PARAM;

#endif




/*
 * @brief used in bk_ble_set_event_callback, this enum show as "event", you must analyse param in same time
 */
typedef enum
{
    /// ble stack init ok, param null
    BK_DM_BLE_EVENT_STACK_OK,             //BLE_5_STACK_OK

    /// scan peer adv report, param ble_adv_report_t
    BK_DM_BLE_EVENT_REPORT_ADV,             //BLE_5_REPORT_ADV

    /// mtu change event, param ble_mtu_change_t
    BK_DM_BLE_EVENT_MTU_CHANGE,             //BLE_5_MTU_CHANGE

    /// recv connnect event, param ble_conn_att_t
    BK_DM_BLE_EVENT_CONNECT,             //BLE_5_CONNECT_EVENT

    /// recv disconnect event, param ble_conn_att_t
    BK_DM_BLE_EVENT_DISCONNECT,             //BLE_5_DISCONNECT_EVENT

    /// create db event, param ble_create_db_t, param null
    BK_DM_BLE_EVENT_CREATE_DB,             //BLE_5_CREATE_DB

    /// tx complete event, param ble_att_tx_compl_t
    BK_DM_BLE_EVENT_TX_DONE,             //BLE_5_TX_DONE


    /// get current conn phy result, param ble_read_phy_t
    BK_DM_BLE_EVENT_READ_PHY,             //BLE_5_READ_PHY_EVENT

    /// recv ble conn update event result, param ble_conn_update_param_compl_ind_t
    BK_DM_BLE_EVENT_CONN_UPDATA,             //BLE_5_CONN_UPDATA_EVENT

    /// discover peer primary service result, param ble_discovery_primary_service_t
    BK_DM_BLE_EVENT_DISCOVERY_PRIMARY_SERVICE,             //BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT

    /// discover peer characteristic result, param ble_discovery_char_t
    BK_DM_BLE_EVENT_DISCOVERY_CHAR,             //BLE_5_DISCOVERY_CHAR_EVENT

    ///recv gatt client notify, param ble_att_notify_t
    BK_DM_BLE_EVENT_RECV_NOTIFY,             //BLE_5_RECV_NOTIFY_EVENT

    ///recv gatt read response, param ble_att_rw_t
    BK_DM_BLE_EVENT_ATT_READ_RESPONSE,             //BLE_5_ATT_READ_RESPONSE

    ///recv ble connection param update req, param ble_conn_update_param_ind_t
    BK_DM_BLE_EVENT_CONN_UPD_PAR_ASK,             //BLE_5_CONN_UPD_PAR_ASK
} ble_event_enum_t;




/**
    @brief use in @ref bk_ble_set_advertising_params
 */
enum advertise_filter_policy
{
    /// both scan and connect
    ADV_FILTER_POLICY_ALLOW_SCAN_ANY_CONNECT_ANY,
    /// scan only in white list, connect any
    ADV_FILTER_POLICY_ALLOW_SCAN_WLST_CONNECT_ANY,
    /// scan any, connect only in white list
    ADV_FILTER_POLICY_ALLOW_SCAN_ANY_CONNECT_WLIT,
    /// both scan connect only in white list
    ADV_FILTER_POLICY_ALLOW_SCAN_WLIT_CONNECT_WLIT,
};

/**
    @brief use in @ref bk_ble_set_advertising_params
 */
enum advertise_legacy_type
{
    /// adv that can be scan and connect
    ADV_LEGACY_TYPE_ADV_IND,
    /// adv that can be connect by special device(white list). This send adv more frequently.
    ADV_LEGACY_TYPE_ADV_DIRECT_IND_HIGH,
    /// adv that can be scan
    ADV_LEGACY_TYPE_ADV_SCAN_IND,
    /// adv that can't be scan and connect
    ADV_LEGACY_TYPE_ADV_NON_IND,
    /// adv that can be connect by special device(white list). This send adv more few.
    ADV_LEGACY_TYPE_ADV_DIRECT_IND_LOW,
};

/**
    @brief use in @ref bk_ble_set_advertising_params
 */
typedef struct
{
    /// Minimum advertising interval (in unit of 625us). Must be greater than or equal to 20ms
    uint32_t adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than or equal to 20ms
    uint32_t adv_intv_max;
    /// Advertising type, see adv_legacy_type
    uint8_t adv_type;
    /// Own address type:  public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3
    uint8_t own_addr_type;
    /// Peer address
    bd_addr_t peer_address;
    /// Peer address type:  public or public identity addr =0 / RDP or random Identity Addr =1
    uint8_t peer_addr_type;
    /// Bit field indicating the channel mapping (@see enum adv_chnl_map)
    uint8_t chnl_map;
    /// advertising policy see advertise_filter_policy
    uint16_t filter_policy;
} ble_adv_parameter_t;


/**
    @brief use in BK_DM_BLE_EVENT_CONNECT BK_DM_BLE_EVENT_DISCONNECT
 */
typedef struct
{
    API_RESULT event_result;
    //ATT_HANDLE att_handle;
    uint8_t conn_handle;
    /// Peer address type
    uint8_t peer_addr_type;
    /// Peer BT address
    uint8_t peer_addr[6];
} ble_conn_att_t;

/**
    @brief use in BK_DM_BLE_EVENT_TX_DONE
 */
typedef struct
{
    API_RESULT event_result;
    //    ATT_HANDLE att_handle;
    uint8_t conn_handle;
    ATT_ATTR_HANDLE attr_handle;
} ble_att_tx_compl_t;


/**
    @brief use in BK_DM_BLE_EVENT_RECV_NOTIFY
 */
typedef struct
{
    API_RESULT event_result;
    //    ATT_HANDLE att_handle;
    uint8_t conn_handle;
    ATT_ATTR_HANDLE attr_handle;

    uint8_t *data;
    uint16_t len;
} ble_att_notify_t;

/**
    @brief use in BK_DM_BLE_EVENT_CONN_UPD_PAR_ASK
 */
typedef struct
{
    bd_addr_t peer_address;

    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;

    uint8_t is_agree;   ///0:is not agree,1:is agree
} ble_conn_update_param_ind_t;

/**
    @brief use in BK_DM_BLE_EVENT_CONN_UPDATA
 */
typedef struct
{
    bd_addr_t peer_address;

    uint8_t status;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} ble_conn_update_param_compl_ind_t;

/**
    @brief use in BK_DM_BLE_EVENT_ATT_READ_RESPONSE
 */
typedef struct
{
    API_RESULT event_result;
    //ATT_HANDLE att_handle;
    uint8_t conn_handle;
    ATT_ATTR_HANDLE attr_handle;

    uint8_t *data;
    uint16_t len;
} ble_att_rw_t;

/**
 * GATT DB callback extension, GATT DB Handler Callback.
 * The GATT DB PL extension calls the registered callback
 * to indicate any ongoing GATT DB Operation.
 * The Upper Layer can implement specific handling for any Characterisitc
 * or Characteritistic Descriptor depending on its requirement.
 *
 * \param [in] conn_handle connection handle, see @ref BK_DM_BLE_EVENT_CONNECT
 * \param [in] handle Pointer to GATT DB identifier \ref GATT_DB_HANDLE
 * \param [in] param  Pointer to GATT DB Parameters \ref GATT_DB_PARAMS
 *
 *
 * \return API_SUCCESS if ATT/GATT response for the incoming request to be sent
 *         from the below layer
 *         GATT_DB_DELAYED_RESPONSE if ATT/GATT response will be sent at a
 *         later point by the application
 *         GATT_DB_DONOT_RESPOND if ATT/GATT response will be sent at a
 *         later point by the application
 *         GATT_DB_ALREADY_RESPONDED if ATT/GATT response is already sent
 *         the application
 *         else and Error code describing cause of failure.
 */
typedef bk_err_t (*ble_gatt_db_callback_t)(
    uint8_t conn_handle,
    /*IN */ GATT_DB_HANDLE *handle,
    /*IN */ GATT_DB_PARAMS *param);


/**
 * @brief for async dm_ble api event.
 *
 * ble event report.
 *
 * @param
 * - event: event id. see ble_event_enum_t
 * - param: param
 *
**/
typedef uint32_t (*ble_event_cb_t)(ble_event_enum_t event, void *param);

#ifdef __cplusplus
}
#endif

