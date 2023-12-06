/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _TMAS_CLIENT_H_
#define _TMAS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "tmas_def.h"

/**
 * \defgroup    LEA_USE_CASE_TMAP_Client Telephony and Media Audio Client
 *
 * \brief   The client role for Telephony and Media Audio Profile.
 */

/**
 * \defgroup    TMAP_Client_Exported_Types Telephony and Media Audio Client Exported Types
 *
 * \ingroup LEA_USE_CASE_TMAP_Client
 * \{
 */

/**
 * tmas_client.h
 *
 * \brief  TMAS Client Discover Service Result.
 *         The message data for LE_AUDIO_MSG_TMAS_CLIENT_DIS_DONE.
 *
 * \ingroup TMAP_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;      /**< Whether to find service. */
    bool    load_from_ftl; /**< Whether the service table is loaded from FTL. */
} T_TMAS_CLIENT_DIS_DONE;

/**
 * tmas_client.h
 *
 * \brief  TMAS Client Read TMAP Role Result.
 *         The message data for LE_AUDIO_MSG_TMAS_CLIENT_READ_ROLE_RESULT.
 *
 * \ingroup TMAP_Client_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    uint16_t role;
    uint16_t cause;
} T_TMAS_CLIENT_READ_ROLE_RESULT;
/**
 * End of TMAP_Client_Exported_Types
 * \}
 */

/**
 * \defgroup    TMAP_Client_Exported_Functions Telephony and Media Audio Client Exported Functions
 *
 * \ingroup LEA_USE_CASE_TMAP_Client
 * \{
 */

/**
 * tmas_client.h
 *
 * \brief  TMAS client read TMAP role.
 *         If this API is called successfully, the Application will
 *         receive LE_AUDIO_MSG_TMAS_CLIENT_READ_ROLE_RESULT.
 *
 * \param[in]  conn_handle      Connection handle.
 *
 * \return         The result of TMAS client read TMAP role.
 * \retval true    TMAS client read TMAP role request success.
 * \retval false   TMAS client read TMAP role request failed.
 *
 * \ingroup TMAP_Client_Exported_Functions
 */
bool tmas_read_role(uint16_t conn_handle);

/**
 * tmas_client.h
 *
 * \brief  Initialize TMAS client.
 *
 * \return         The result of initialize TMAS client.
 * \retval true    Initialize TMAS client success.
 * \retval false   Initialize TMAS client failed.
 *
 * \ingroup TMAP_Client_Exported_Functions
 */
bool tmas_client_init(void);
/**
 * End of TMAP_Client_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
