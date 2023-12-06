/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _AICS_CLIENT_H_
#define _AICS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "aics_def.h"
#include "ble_audio.h"
#include "ble_audio_group.h"

/**
 * \defgroup    LEA_GAF_AICS_Client Audio Input Control Client
 *
 * \brief   The client role for Audio Input Control Service.  Media Control Profile Definition Exported Macros
 */

/**
 * \defgroup    AICS_Client_Exported_Macros Audio Input Control Client Exported Macros
 *
 * \ingroup LEA_GAF_AICS_Client
 * \{
 */

/**
 * aics_client.h
 *
 * \defgroup AICS_CFG_FLAGS AICS Service Characteristic Flags.
 *
 * \brief  Define AICS Service Characteristic Flags.
 *
 * \ingroup AICS_Client_Exported_Macros
 * \{
 */
#define AICS_INPUT_STATE_FLAG       0x01    /**< Audio Input State Flag for read and CCCD configuration. */
#define AICS_INPUT_STATUS_FLAG      0x02    /**< Audio Input Status Flag for read and CCCD configuration. */
#define AICS_INPUT_DES_FLAG         0x04    /**< Audio Input Description Flag for read and CCCD configuration. */
#define AICS_INPUT_TYPE_FLAG        0x08    /**< Audio Input Type Flag for read. */
#define AICS_GAIN_SETTING_PROP_FLAG 0x10    /**< Gain Setting Properties Flag for read. */
/**
 * End of AICS_CFG_FLAGS
 * \}
 */

/**
 * End of AICS_Client_Exported_Macros
 * \}
 */

/**
 * \defgroup    AICS_Client_Exported_Types Audio Input Control Client Exported Types
 *
 * \ingroup LEA_GAF_AICS_Client
 * \{
 */

/**
 * aics_client.h
 *
 * \brief  AICS Characteristic Data.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef union
{
    T_AICS_INPUT_STATE       input_state;    /**< Audio Input State Data. */
    T_AICS_GAIN_SETTING_PROP setting_prop;   /**< Gain Setting Properties Data. */
    uint8_t                  input_type;     /**< Audio Input Type Data. */
    uint8_t                  input_status;   /**< Audio Input Status Data. */
    T_AICS_INPUT_DES         input_des;      /**< Audio Input Description Data. */
} T_AICS_DATA;

/**
 * aics_client.h
 *
 * \brief  AICS Characteristic Type.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef enum
{
    AICS_CHAR_INPUT_STATE,          /**< Audio Input State. */
    AICS_CHAR_GAIN_SETTING_PROP,    /**< Gain Setting Properties. */
    AICS_CHAR_INPUT_TYPE,           /**< Audio Input Type. */
    AICS_CHAR_INPUT_STATUS,         /**< Audio Input Status. */
    AICS_CHAR_INPUT_DES,            /**< Audio Input Description. */
} T_AICS_CHAR_TYPE;

/**
 * aics_client.h
 *
 * \brief  AICS Client Read Characteristic Value Result.
 *         The message data for LE_AUDIO_MSG_AICS_CLIENT_READ_RESULT.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef struct
{
    uint16_t            conn_handle;
    uint8_t             srv_instance_id;   /**< Service instance id. Used when the
                                                number of services is greater than 1. */
    T_AICS_CHAR_TYPE    type;
    T_AICS_DATA         data;
    uint16_t            cause;
} T_AICS_CLIENT_READ_RESULT;

/**
 * aics_client.h
 *
 * \brief  AICS Client Discover AICS Service Result.
 *         The message data for LE_AUDIO_MSG_AICS_CLIENT_DIS_DONE.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool     is_found;          /**< Whether to find service. */
    bool     load_from_ftl;     /**< Whether the service table is loaded from FTL. */
    uint8_t  srv_num;           /**< Service instance number. */
} T_AICS_CLIENT_DIS_DONE;

/**
 * aics_client.h
 *
 * \brief  Audio Input Control Point Characteristic Parameter.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef struct
{
    int8_t  gaining_setting;
} T_AICS_CP_PARAM;

/**
 * aics_client.h
 *
 * \brief  AICS Client Receive Notification Data.
 *         The message data for LE_AUDIO_MSG_AICS_CLIENT_NOTIFY.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef struct
{
    uint16_t            conn_handle;
    uint8_t             srv_instance_id;   /**< Service instance id. Used when the
                                                number of services is greater than 1. */
    T_AICS_CHAR_TYPE    type;
    T_AICS_DATA         data;
} T_AICS_CLIENT_NOTIFY;

/**
 * aics_client.h
 *
 * \brief  AICS Client Write Control Point Result.
 *         The message data for LE_AUDIO_MSG_AICS_CLIENT_CP_RESULT.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef struct
{
    uint16_t        conn_handle;
    uint8_t         srv_instance_id;   /**< Service instance id. Used when the
                                            number of services is greater than 1. */
    uint16_t        cause;
    T_AICS_CP_OP    cp_op;
} T_AICS_CLIENT_CP_RESULT;

/**
 * aics_client.h
 *
 * \brief  AICS Service Data.
 *
 * \ingroup AICS_Client_Exported_Types
 */
typedef struct
{
    uint8_t                  srv_instance_id;   /**< Service instance id. Used when the
                                                     number of services is greater than 1. */
    uint8_t                  type_exist;        /**< AICS service type: @ref AICS_CFG_FLAGS. */
    T_AICS_INPUT_STATE       input_state;
    T_AICS_GAIN_SETTING_PROP setting_prop;
    uint8_t                  input_type;
    uint8_t                  input_status;
    T_AICS_INPUT_DES         input_des;
} T_AICS_SRV_DATA;

/**
 * End of AICS_Client_Exported_Types
 * \}
 */

/**
 * \defgroup AICS_Client_Exported_Functions Audio Input Control Client Exported Functions
 *
 * \ingroup LEA_GAF_AICS_Client
 * \{
 */

/**
 * aics_client.h
 *
 * \brief  Get AICS service data.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_instance_id  Service instance id.
 * \param[in,out]  p_value      Pointer to AICS service data: @ref T_AICS_SRV_DATA.
 *
 * \return         The result of get AICS service data.
 * \retval true    Get AICS service data success.
 * \retval false   Get AICS service data failed.
 *
 * \ingroup AICS_Client_Exported_Functions
 */
bool aics_get_srv_data(uint16_t conn_handle, uint8_t srv_instance_id, T_AICS_SRV_DATA *p_value);

/**
 * aics_client.h
 *
 * \brief  Config the AICS service CCCD data.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_instance_id  Service instance id.
 * \param[in]  cfg_flags        AICS CCCD Flags: @ref AICS_CFG_FLAGS.
 * \param[in]  enable           Whether to enable CCCD.
 * \arg    true    Enable CCCD.
 * \arg    false   Disable CCCD.
 *
 * \ingroup AICS_Client_Exported_Functions
 */
void aics_cfg_cccd(uint16_t conn_handle, uint8_t srv_instance_id, uint8_t cfg_flags, bool enable);

/**
 * aics_client.h
 *
 * \brief  Read AICS characteristic value.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_instance_id  Service instance id.
 * \param[in]  type             AICS characteristic type: @ref T_AICS_CHAR_TYPE.
 *
 * \return         The result of read AICS characteristic value.
 * \retval true    Read AICS characteristic value success.
 * \retval false   Read AICS characteristic value failed.
 *
 * \ingroup AICS_Client_Exported_Functions
 */
bool aics_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, T_AICS_CHAR_TYPE type);

/**
 * aics_client.h
 *
 * \brief  AICS client write audio input control point.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_instance_id  Service instance id.
 * \param[in]  op               Audio input control point opcode: @ref T_AICS_CP_OP.
 * \param[in]  p_param          Point to audio input control point parameter: @ref T_AICS_CP_PARAM.
 *
 * \return         The result of AICS client write audio input control point.
 * \retval true    AICS client write audio input control point success.
 * \retval false   AICS client write audio input control point failed.
 *
 * \ingroup AICS_Client_Exported_Functions
 */
bool aics_write_cp(uint16_t conn_handle, uint8_t srv_instance_id, T_AICS_CP_OP op,
                   T_AICS_CP_PARAM *p_param);

/**
 * aics_client.h
 *
 * \brief  AICS client write audio input control point by ble audio group.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  group_handle     BLE audio group handle: @ref T_BLE_AUDIO_GROUP_HANDLE.
 * \param[in]  srv_instance_id  Service instance id.
 * \param[in]  op               Audio input control point opcode: @ref T_AICS_CP_OP.
 * \param[in]  p_param          Point to audio input control point parameter: @ref T_AICS_CP_PARAM.
 *
 * \return Operation result        The result of AICS client write audio input control point.
 * \retval LE_AUDIO_CAUSE_SUCCESS  AICS client write audio input control point success.
 * \retval Others                  AICS client write audio input control point failed.
 *
 * \ingroup AICS_Client_Exported_Functions
 */
T_LE_AUDIO_CAUSE aics_write_cp_by_group(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                        uint8_t srv_instance_id,
                                        T_AICS_CP_OP op,
                                        T_AICS_CP_PARAM *p_param);

/**
 * aics_client.h
 *
 * \brief  Write the Audio Input Description characteristic.
 *         If the audio input description characteristic changes, the Application will receive
 *         LE_AUDIO_MSG_AICS_CLIENT_NOTIFY.
 *
 * \xrefitem Added_API_2_14_1_0 "Added Since 2.14.1.0" "Added API"
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  srv_instance_id  AICS service index.
 * \param[in]  p_input_des      Point to the input descriptor value.
 * \param[in]  input_des_len    The length of the input descriptor value.
 *
 * \return         The result of AICS client send write without response packet.
 * \retval true    AICS client send write without response packet success.
 * \retval false   AICS client send write without response packet failed.
 *
 * \ingroup AICS_Client_Exported_Functions
 */
bool aics_write_input_des(uint16_t conn_handle, uint8_t srv_instance_id,
                          uint8_t *p_input_des, uint16_t input_des_len);
/**
 * End of AICS_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
