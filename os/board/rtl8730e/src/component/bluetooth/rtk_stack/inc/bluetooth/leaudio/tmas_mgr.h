/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _TMAS_MGR_H_
#define _TMAS_MGR_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "tmas_def.h"

/**
 * \defgroup    LEA_USE_CASE_TMAP_Server Telephony and Media Audio Server
 *
 * \brief   The server role for Telephony and Media Audio Profile.
 */

/**
 * \defgroup    TMAP_Server_Exported_Functions Telephony and Media Audio Server Exported Functions
 *
 * \ingroup LEA_USE_CASE_TMAP_Server
 * \{
 */

/**
 * tmas_mgr.h
 *
 * \brief  Initialize TMAS Service.
 *
 * \param[in]  role      TMAP role: @ref TMAP_ROLE_BIT_MASK.
 *
 * \return         The result of initialize TMAS Service.
 * \retval true    Initialize TMAS Service success.
 * \retval false   Initialize TMAS Service failed.
 *
 * \ingroup TMAP_Server_Exported_Functions
 */
bool tmas_init(uint16_t role);
/**
 * End of TMAP_Server_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
