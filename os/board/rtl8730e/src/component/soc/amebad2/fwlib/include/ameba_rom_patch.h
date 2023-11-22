/**
  ******************************************************************************
  * @file    ameba_rom_patch.c
  * @author
  * @version V1.0.0
  * @date    2022-05-31
  * @brief   This file contains all the patch functions for rom code.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBA_ROM_PATCH_H_
#define _AMEBA_ROM_PATCH_H_

/* Exported constants --------------------------------------------------------*/
/** @defgroup ROM_PATCH_Exported_Constants ROM_PATCH Exported Constants
  * @{
  */

// add your constants here

/**
  * @}
  */

/* Exported functions -------------------------------------------------------- */
/** @defgroup ROM_PATCH_Exported_Functions ROM_PATCH Exported Functions
  * @{
  */

// add your function statements here
_LONG_CALL_ void LOGUART_PutChar_RAM(u8 c);
_LONG_CALL_ void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
_LONG_CALL_ void Pinmux_Config(u8 PinName, u32 PinFunc);
_LONG_CALL_ void GPIO_ODInit(u8 PinName, u32 PUInternal);
_LONG_CALL_ void GPIO_ODWriteBit(u8 PinName, u32 PinState);
_LONG_CALL_ u32 SYSCFG_RLVersion(void);
_LONG_CALL_ u32 SYSCFG_GetRLNum(void);
_LONG_CALL_ VOID VectorTableAdd(VOID);

#ifdef ARM_CORE_CM4
#include "amebahp_secure_boot.h"

int SBOOT_GetMdType_B(u8 HashId, SHA2_TYPE *MdType, u8 *IsHmac);
int SBOOT_Validate_Algorithm_B(u8 *AuthAlg, u8 *HashAlg, u8 ManiAuth, u8 ManiHash);
int SBOOT_Validate_PubKey_B(u8 AuthAlg, u8 *Pk, u8 *Hash);
int SBOOT_Validate_Signature_B(u8 AuthAlg, u8 HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig);
int SBOOT_Validate_ImgHash_B(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num);
int SBOOT_SignatureVerify_B(u8 *PubKeyHash, Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgCnt);

int CRYPTO_SHA_Init_B(HAL_CRYPTO_ADAPTER *pIE);
int CRYPTO_OTPKey_SHA_Init_B(u32 keynum, u32 status);
int rtl_crypto_sha2_init_B(IN const SHA2_TYPE sha2type, hw_sha_context *ctx);
int rtl_crypto_sha2_update_B(IN u8 *message, IN u32 msglen, hw_sha_context *ctx);
int rtl_crypto_sha2_final_B(OUT u8 *pDigest, hw_sha_context *ctx);
int rtl_crypto_hmac_sha2_update_B(IN u8 *message, IN u32 msglen, hw_sha_context *ctx);
int rtl_crypto_hmac_sha2_final_B(OUT u8 *pDigest, hw_sha_context *ctx);

int SBOOT_GetMdType_A(u8 HashId, SHA2_TYPE *MdType, u8 *IsHmac);
int SBOOT_Validate_Algorithm_A(u8 *AuthAlg, u8 *HashAlg, u8 ManiAuth, u8 ManiHash);
int SBOOT_Validate_PubKey_A(u8 AuthAlg, u8 *Pk, u8 *Hash);
int SBOOT_Validate_Signature_A(u8 AuthAlg, u8 HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig);
int SBOOT_Validate_ImgHash_A(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num);
int SBOOT_SignatureVerify_A(u8 *PubKeyHash, Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgCnt);

int CRYPTO_SHA_Init_A(HAL_CRYPTO_ADAPTER *pIE);
int CRYPTO_OTPKey_SHA_Init_A(u32 keynum, u32 staAtus);
int rtl_crypto_sha2_init_A(IN const SHA2_TYPE sha2type, hw_sha_context *ctx);
int rtl_crypto_sha2_update_A(IN u8 *message, IN u32 msglen, hw_sha_context *ctx);
int rtl_crypto_sha2_final_A(OUT u8 *pDigest, hw_sha_context *ctx);
int rtl_crypto_hmac_sha2_update_A(IN u8 *message, IN u32 msglen, hw_sha_context *ctx);
int rtl_crypto_hmac_sha2_final_A(OUT u8 *pDigest, hw_sha_context *ctx);
#endif
/**
  * @}
  */

#endif /* _AMEBA_ROM_PATCH_H_ */
/******************* (C) COPYRIGHT 2022 Realtek Semiconductor *****END OF FILE****/

