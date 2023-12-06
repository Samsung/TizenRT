/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _MICS_MGR_H_
#define _MICS_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "codec_def.h"
#include "mics_def.h"

/**
 * \defgroup    LEA_GAF_MICP_Server Microphone Control Server
 *
 * \brief   The server role for Microphone Control Profile.
 */

/**
 * \defgroup    MICP_Server_Exported_Types Microphone Control Server Exported Types
 *
 * \ingroup LEA_GAF_MICP_Server
 * \{
 */

/**
 * mics_mgr.h
 *
 * \brief  MICS Service parameter.
 *
 * \ingroup MICP_Server_Exported_Types
 */
typedef struct
{
    T_MICS_MUTE mic_mute;
} T_MICS_PARAM;

/**
 * mics_mgr.h
 *
 * \brief  MICS Service Write Mute Indication parameter.
 *         The message data for LE_AUDIO_MSG_MICS_WRITE_MUTE_IND.
 *
 * \ingroup MICP_Server_Exported_Types
 */
typedef struct
{
    T_MICS_MUTE mic_mute;
} T_MICS_WRITE_MUTE_IND;
/**
 * End of MICP_Server_Exported_Types
 * \}
 */

/**
 * \defgroup    MICP_Server_Exported_Functions Microphone Control Server Exported Functions
 *
 * \ingroup LEA_GAF_MICP_Server
 * \{
 */

/**
 * mics_mgr.h
 *
 * \brief  Set MICS service parameters.
 *
 * \param[in]  p_param   Pointer to MICS service parameter: @ref T_MICS_PARAM.
 *
 * \return         The result of set MICS service parameter.
 * \retval true    Set MICS service parameter success.
 * \retval false   Set MICS service parameter failed.
 *
 * \ingroup MICP_Server_Exported_Functions
 */
bool mics_set_param(T_MICS_PARAM *p_param);

/**
 * mics_mgr.h
 *
 * \brief  Get MICS service parameters.
 *
 * \param[in,out]  p_param      Pointer to MICS service parameter: @ref T_MICS_PARAM.
 *
 * \return         The result of get MICS service parameter.
 * \retval true    Get MICS service parameter success.
 * \retval false   Get MICS service parameter failed.
 *
 * \ingroup MICP_Server_Exported_Functions
 */
bool mics_get_param(T_MICS_PARAM *p_param);
/**
 * End of MICP_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
