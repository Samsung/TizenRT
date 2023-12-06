/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CSIS_MGR_H_
#define _CSIS_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "csis_rsi.h"
#include "csis_def.h"
#include "bt_gatt_client.h"

/**
 * \defgroup    LEA_GAF_CSIP_Server Coordinated Set Identification Server
 *
 * \brief   The server role for Coordinated Set Identification Profile.
 */

/**
 * \defgroup    CSIP_Server_Exported_Types Coordinated Set Identification Server Exported Types
 *
 * \ingroup LEA_GAF_CSIP_Server
 * \{
 */

/**
 * csis_mgr.h
 *
 * \brief  CSIS Service Receive Read SIRK Request Parameter.
 *         The message data for LE_AUDIO_MSG_CSIS_READ_SIRK_IND.
 *
 * \ingroup CSIP_Server_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint8_t service_id;
    T_CSIS_SIRK_TYPE sirk_type;
} T_CSIS_READ_SIRK_IND;

/**
 * csis_mgr.h
 *
 * \brief  CSIS Service Set Member Lock State Parameter.
 *         The message data for LE_AUDIO_MSG_CSIS_LOCK_STATE.
 *
 * \ingroup CSIP_Server_Exported_Types
 */
typedef struct
{
    uint8_t service_id;
    T_CSIS_LOCK lock_state;
    uint8_t lock_address[GAP_BD_ADDR_LEN];
    uint8_t lock_address_type;
} T_CSIS_LOCK_STATE;
/**
 * End of CSIP_Server_Exported_Types
 * \}
 */

/**
 * \defgroup    CSIP_Server_Exported_Functions Coordinated Set Identification Server Exported Functions
 *
 * \ingroup LEA_GAF_CSIP_Server
 * \{
 */

/**
 * csis_mgr.h
 *
 * \brief  Update CSIS service SIRK.
 *
 * \param[in]  service_id      Service id.
 * \param[in]  p_sirk          Pointer to SIRK.
 * \param[in]  sirk_type       SIRK type: @ref T_CSIS_SIRK_TYPE.
 *
 * \return         The result of update CSIS service SIRK.
 * \retval true    Update CSIS service SIRK success.
 * \retval false   Update CSIS service SIRK failed.
 *
 * \ingroup CSIP_Server_Exported_Functions
 */
bool csis_update_sirk(uint8_t service_id, uint8_t *p_sirk, T_CSIS_SIRK_TYPE sirk_type);

/**
 * csis_mgr.h
 *
 * \brief  Update Coordinated Set Size characteristic value.
 *
 * \xrefitem Added_API_2_14_1_0 "Added Since 2.14.1.0" "Added API"
 *
 * \param[in]  service_id      Service id.
 * \param[in]  csis_size       Coordinated Set Size characteristic value. Range:0x01 to 0xFF.
 *
 * \return         The result of update Coordinated Set Size.
 * \retval true    Update Coordinated Set Size success.
 * \retval false   Update Coordinated Set Size failed.
 *
 * \ingroup CSIP_Server_Exported_Functions
 */
bool csis_update_size(uint8_t service_id, uint8_t csis_size);

/**
 * csis_mgr.h
 *
 * \brief  Get CSIS service id.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in,out]  p_service_id    Pointer to service id: @ref T_SERVER_ID.
 * \param[in]  p_inc_srv_uuid  Pointer to set member include service uuid: @ref T_ATTR_UUID.
 *
 * \return         The result of get CSIS service id.
 * \retval true    Get CSIS service id success.
 * \retval false   Get CSIS service id failed.
 *
 * \ingroup CSIP_Server_Exported_Functions
 */
bool csis_get_srv_id(uint8_t *p_service_id, T_ATTR_UUID *p_inc_srv_uuid);
/**
 * End of CSIP_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
