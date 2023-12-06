/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CSIS_RSI_H_
#define _CSIS_RSI_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

/**
 * \defgroup    LEA_GAF_CSIP_RSI Coordinated Set Identification RSI
 *
 * \brief   Coordinated Set Identification Profile resolvable set identifier.
 */

/**
 * \defgroup    CSIP_RSI_Exported_Macros Coordinated Set Identification RSI Exported Macros
 *
 * \ingroup LEA_GAF_CSIP_RSI
 * \{
 */

/**
 * csis_rsi.h
 *
 * \brief  Define CSIS Service RSI Length
 *
 * \ingroup CSIP_RSI_Exported_Macros
 */
#define CSI_RSI_LEN           6
/**
 * End of CSIP_RSI_Exported_Macros
 * \}
 */

/**
 * \defgroup    CSIP_RSI_Exported_Functions Coordinated Set Identification RSI Exported Functions
 *
 * \ingroup LEA_GAF_CSIP_RSI
 * \{
 */

/**
 * csis_rsi.h
 *
 * \brief  Generate resolvable set identifier.
 *
 * \param[in]  p_sirk      Pointer to set identity resolving key.
 * \param[in,out]  p_rsi   Pointer to resolvable set identifier.
 *
 * \return         The result of generate resolvable set identifier.
 * \retval true    Generate resolvable set identifier success.
 * \retval false   Generate resolvable set identifier failed.
 *
 * \ingroup CSIP_RSI_Exported_Functions
 */
bool csis_gen_rsi(const uint8_t *p_sirk, uint8_t *p_rsi);

/**
 * csis_rsi.h
 *
 * \brief  Resolve resolvable set identifier.
 *
 * \param[in]  p_sirk      Pointer to set identity resolving key.
 * \param[in]  p_rsi      Pointer to resolvable set identifier.
 *
 * \return         The result of resolve resolvable set identifier.
 * \retval true    SIRK and resolvable set identifier are matched.
 * \retval false   SIRK and resolvable set identifier are not matched.
 *
 * \ingroup CSIP_RSI_Exported_Functions
 */
bool csis_check_rsi(const uint8_t *p_sirk, uint8_t *p_rsi);
/**
 * End of CSIP_RSI_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
