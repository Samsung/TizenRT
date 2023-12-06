/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _AICS_MGR_H_
#define _AICS_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "codec_def.h"
#include "aics_def.h"

/**
 * \defgroup    LEA_GAF_AICS_Server Audio Input Control Server
 *
 * \brief   The server role for Audio Input Control Service.
 */

/**
 * \defgroup    AICS_Server_Exported_Types Audio Input Control Server Exported Types
 *
 * \ingroup LEA_GAF_AICS_Server
 * \{
 */

/**
 * aics_mgr.h
 *
 * \brief  AICS Control Point Operation Parameter.
 *         The message data for LE_AUDIO_MSG_AICS_CP_IND.
 *
 * \ingroup AICS_Server_Exported_Types
 */
typedef struct
{
    uint16_t     conn_handle;
    uint8_t      srv_instance_id; /**< Service instance id. Used when the
                                       number of services is greater than 1. */
    T_AICS_CP_OP cp_op;
    int8_t       gain_setting;
} T_AICS_CP_IND;

/**
 * aics_mgr.h
 *
 * \brief  AICS Audio Input Description Parameter.
 *         The message data for LE_AUDIO_MSG_AICS_WRITE_INPUT_DES_IND.
 *
 * \ingroup AICS_Server_Exported_Types
 */
typedef struct
{
    uint16_t         conn_handle;
    uint8_t          srv_instance_id; /**< Service instance id. Used when the
                                           number of services is greater than 1. */
    T_AICS_INPUT_DES input_des;
} T_AICS_WRITE_INPUT_DES_IND;

/**
 * aics_mgr.h
 *
 * \brief  AICS Parameter Type.
 *
 * \ingroup AICS_Server_Exported_Types
 */
typedef enum
{
    AICS_PARAM_INPUT_STATE       = 0x01,   /**< Parameter Data @ref T_AICS_INPUT_STATE */
    AICS_PARAM_GAIN_SETTING_PROP = 0x02,   /**< Parameter Data @ref T_AICS_GAIN_SETTING_PROP */
    AICS_PARAM_INPUT_TYPE        = 0x03,   /**< Parameter Data length is 1 byte */
    AICS_PARAM_INPUT_STATUS      = 0x04,   /**< Parameter Data length is 1 byte */
    AICS_PARAM_INPUT_DES         = 0x05,   /**< Parameter Data @ref T_AICS_INPUT_DES */
} T_AICS_PARAM_TYPE;
/**
 * End of AICS_Server_Exported_Types
 * \}
 */

/**
 * \defgroup    AICS_Server_Exported_Functions Audio Input Control Server Exported Functions
 *
 * \ingroup LEA_GAF_AICS_Server
 * \{
 */

/**
 * aics_mgr.h
 *
 * \brief  Set AICS service parameter.
 *
 * \param[in]  srv_instance_id     Service instance id.
 * \param[in]  param_type          AICS service parameter type: @ref T_AICS_PARAM_TYPE.
 * \param[in]  value_len           The value length of p_value.
 * \arg    param_type @ref AICS_PARAM_INPUT_STATE, value_len is size of @ref T_AICS_INPUT_STATE.
 * \arg    param_type @ref AICS_PARAM_GAIN_SETTING_PROP, value_len is size of @ref T_AICS_GAIN_SETTING_PROP.
 * \arg    param_type @ref AICS_PARAM_INPUT_TYPE, value_len is uint8_t.
 * \arg    param_type @ref AICS_PARAM_INPUT_STATUS, value_len is uint8_t.
 * \arg    param_type @ref AICS_PARAM_INPUT_DES, value_len is the length of audio input description.
 * \param[in]  p_value             Pointer to the AICS parameter data.
 * \param[in]  set_change_counter  Whether to set change counter.
 * \arg    true    Set change counter when param_type is AICS_PARAM_INPUT_STATE.
 * \arg    false   Not set change counter when param_type is AICS_PARAM_INPUT_STATE.
 *
 * \return         The result of set AICS service parameter.
 * \retval true    Set AICS service parameter success.
 * \retval false   Set AICS service parameter failed.
 *
 * \ingroup AICS_Server_Exported_Functions
 */
bool aics_set_param(uint8_t srv_instance_id, T_AICS_PARAM_TYPE param_type, uint8_t value_len,
                    uint8_t *p_value, bool set_change_counter);

/**
 * aics_mgr.h
 *
 * \brief  Get AICS service parameter.
 *
 * \param[in]  srv_instance_id   Service instance id.
 * \param[in]  param_type        AICS service parameter type: @ref T_AICS_PARAM_TYPE, include
 *                               @ref AICS_PARAM_INPUT_STATE, @ref AICS_PARAM_GAIN_SETTING_PROP,
 *                               @ref AICS_PARAM_INPUT_TYPE, @ref AICS_PARAM_INPUT_STATUS.
 * \param[in,out]  p_value       Pointer to the parameter value.
 *
 * \return         The result of get AICS parameter.
 * \retval true    Get AICS service parameter success.
 * \retval false   Get AICS parameter failed.
 *
 * \ingroup AICS_Server_Exported_Functions
 */
bool aics_get_param(uint8_t srv_instance_id, T_AICS_PARAM_TYPE param_type, uint8_t *p_value);
/**
 * End of AICS_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
