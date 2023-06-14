/**
  ******************************************************************************
  * @file    rtl8721dhp_sha.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the HW crypto:
  *           - SHA1/SHA2
  *           - HMAC SHA1/SHA2
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

/**
  * @brief  hash save
  * @param 	message : dst buffer
  * @retval	none
  */
__weak HAL_ROM_TEXT_SECTION
void hash_save(hw_sha_context *ctx)
{

	if (ctx != NULL) {
		HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
		ctx->isInit = pIE->isInit;
		ctx->isIntMode = pIE->isIntMode;

		ctx->digestlen = pIE ->digestlen;
		ctx->hmac_seq_buf_is_used_bytes = pIE->hmac_seq_buf_is_used_bytes;
		ctx->hmac_seq_hash_first = pIE->hmac_seq_hash_first;
		ctx->hmac_seq_hash_last = pIE->hmac_seq_hash_last;
		ctx->hmac_seq_hash_total_len = pIE->hmac_seq_hash_total_len;
		ctx->hmac_seq_is_recorded = pIE->hmac_seq_is_recorded;
		ctx->isMD5 = pIE->isMD5;
		ctx->isSHA1 = pIE->isSHA1;
		ctx->isSHA2 = pIE->isSHA2;
		ctx->sha2type = pIE->sha2type;
		ctx->auth_type = pIE->auth_type;

		for (int i = 0; i < 128; i++) {
			ctx->buffer[i] = pIE ->hmac_seq_buf[i]; //message left
		}
	}

}

/**
  * @brief  hash restore
  * @param 	message : src buffer
  * @retval	none
  */
__weak HAL_ROM_TEXT_SECTION
void hash_restore(hw_sha_context *ctx)
{
	if (ctx != NULL) {
		HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

		pIE->isInit = ctx->isInit;
		pIE->isIntMode = ctx->isIntMode;
		pIE->digestlen = ctx ->digestlen;
		pIE->hmac_seq_buf_is_used_bytes = ctx->hmac_seq_buf_is_used_bytes;
		pIE->hmac_seq_hash_first = ctx->hmac_seq_hash_first;
		pIE->hmac_seq_hash_last = ctx->hmac_seq_hash_last;
		pIE->hmac_seq_hash_total_len = ctx->hmac_seq_hash_total_len;
		pIE->hmac_seq_is_recorded = ctx->hmac_seq_is_recorded;
		pIE->isMD5 = ctx->isMD5;
		pIE->isSHA1 = ctx->isSHA1;
		pIE->isSHA2 = ctx->isSHA2;
		pIE->sha2type = ctx->sha2type;
		pIE->auth_type = ctx->auth_type;


		for (int i = 0; i < 128; i++) {
			pIE->hmac_seq_buf[i] = ctx ->buffer[i]; //message left
		}
	}

}

/**
  * @brief  SHA1 init
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha1_init(hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	const u8 *pCipherKey 	= NULL;
	const u32 lenCipherKey 	= 0;
	const u8 *pAuthKey 		= NULL;
	const u32 lenAuthKey 	= 0;
	u32 ret;

	assert_param(pIE->isInit == _TRUE);

	// for sequential hash
	pIE->hmac_seq_is_recorded = 0;
	pIE->hmac_seq_buf_is_used_bytes = 0;

	ret = CRYPTO_SetSecurityModeAD(pIE, CIPHER_TYPE_NO_CIPHER, AUTH_TYPE_SHA1,
								   pCipherKey, lenCipherKey, pAuthKey, lenAuthKey);
	if (ctx != NULL) {
		hash_save(ctx);
		for (int i = 0; i < 5; i++) {
			ctx->state[0] = 0x67452301;
			ctx->state[1] = 0xEFCDAB89;
			ctx->state[2] = 0x98BADCFE;
			ctx->state[3] = 0x10325476;
			ctx->state[4] = 0xC3D2E1F0;
		}
		ctx->initial = 1;
	}

	return ret;
}

/**
  * @brief  SHA1 process.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of SHA1 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha1_process(
	IN const u8 *message, 	IN const u32 msglen,
	OUT u8 *pDigest)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA1) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	return CRYPTO_ProcessAD(pIE, message, msglen, NULL, 0, NULL, 0, pDigest, NULL, NULL);
}

/**
  * @brief  SHA1 update.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha1_update(IN u8 *message, IN u32 msglen, hw_sha_context *ctx)
{
	int ret = SUCCESS;
	while (msglen > 128 * 1024) {
		ret = rtl_crypto_sha1_update_rom(message, 128 * 1024, ctx);
		msglen = msglen - 128 * 1024;
		message += 128 * 1024;
	}
	if (msglen > 0) {
		ret = rtl_crypto_sha1_update_rom(message, msglen, ctx);
	}

	return ret;

}

/**
  * @brief  SHA1 update rom.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length < 128*1024
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha1_update_rom(IN const u8 *message, IN const u32 msglen, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int ret = SUCCESS;

	ctx->seq_hash = 1;
	hash_restore(ctx);

	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	assert_param(message != NULL);
	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA1) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}
	/*
		if ( pIE->hmac_seq_is_recorded != 0 )  {
			ret = CRYPTO_SendSeqBuf(NULL);
			if ( ret != SUCCESS ) return ret;
		}
	*/
	pIE->hmac_seq_last_message = (u8 *)message;
	pIE->hmac_seq_last_msglen = msglen;
	pIE->hmac_seq_is_recorded = 1;

	ret = CRYPTO_SendSeqBuf((u8 *) & (ctx->state[0]), ctx);
	hash_save(ctx);

	return ret;
}

/**
  * @brief  SHA1 final process.
  * @param    pDigest	: the result of SHA1 function
  * @note    Use in sequential hash, process the last block
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha1_final(OUT u8 *pDigest, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	hash_restore(ctx);
	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA1) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if (pDigest == NULL) {
		return _ERRNO_CRYPTO_NULL_POINTER;
	}

	if (pIE->hmac_seq_is_recorded == 0) {
		return _ERRNO_CRYPTO_HASH_FINAL_NO_UPDATE;
	}

	ctx->lasthash = 1;
	//return CRYPTO_SendSeqBuf(pDigest);
	return CRYPTO_ProcessAD(pIE, (const uint8_t *)pIE->hmac_seq_buf, pIE->hmac_seq_buf_is_used_bytes, NULL, 0, NULL, 0, pDigest, NULL, ctx);
}

/**
  * @ SHA1 init & process
  *
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of SHA1 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha1(IN const u8 *message, IN const u32 msglen, OUT u8 *pDigest)
{
	int ret;

	ret = rtl_crypto_sha1_init(NULL);
	if (ret != SUCCESS) {
		return ret;
	}

	ret = rtl_crypto_sha1_process(message, msglen, pDigest);

	return ret;
}

/**
  * @brief  SHA2 init
  * @param	sha2type	: This parameter can be one of the following values:
  *		 @arg SHA2_224
  *		 @arg SHA2_256
  *		 @arg SHA2_384
  *		 @arg SHA2_512
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha2_init(IN const SHA2_TYPE sha2type, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int auth_type;
	const u8 *pCipherKey 	= NULL;
	const u32 lenCipherKey 	= 0;
	const u8 *pAuthKey 		= NULL;
	const u32 lenAuthKey 	= 0;
	u32 ret;

	assert_param(pIE->isInit == _TRUE);

	switch (sha2type) {
	case SHA2_224 :
		auth_type = AUTH_TYPE_SHA2_224_ALL;
		if (ctx != NULL) {
			ctx->state[0] = 0xC1059ED8;
			ctx->state[1] = 0x367CD507;
			ctx->state[2] = 0x3070DD17;
			ctx->state[3] = 0xF70E5939;
			ctx->state[4] = 0xFFC00B31;
			ctx->state[5] = 0x68581511;
			ctx->state[6] = 0x64F98FA7;
			ctx->state[7] = 0xBEFA4FA4;
		}
		break;
	case SHA2_256 :
		auth_type = AUTH_TYPE_SHA2_256_ALL;
		if (ctx != NULL) {
			ctx->state[0] = 0x6A09E667;
			ctx->state[1] = 0xBB67AE85;
			ctx->state[2] = 0x3C6EF372;
			ctx->state[3] = 0xA54FF53A;
			ctx->state[4] = 0x510E527F;
			ctx->state[5] = 0x9B05688C;
			ctx->state[6] = 0x1F83D9AB;
			ctx->state[7] = 0x5BE0CD19;
		}
		break;
	case SHA2_384 :
		auth_type = AUTH_TYPE_SHA2_384_ALL;
		if (ctx != NULL) {
			ctx->state[1] = 0xC1059ED8;
			ctx->state[0] = 0xCBBB9D5D;
			ctx->state[3] = 0x367CD507;
			ctx->state[2] = 0x629A292A;
			ctx->state[5] = 0x3070DD17;
			ctx->state[4] = 0x9159015A;
			ctx->state[7] = 0xF70E5939;
			ctx->state[6] = 0x152FECD8;
			ctx->state[9] = 0xFFC00B31;
			ctx->state[8] = 0x67332667;
			ctx->state[11] = 0x68581511;
			ctx->state[10] = 0x8EB44A87;
			ctx->state[13] = 0x64F98FA7;
			ctx->state[12] = 0xDB0C2E0D;
			ctx->state[15] = 0xBEFA4FA4;
			ctx->state[14] = 0x47B5481D;

		}
		break;
	case SHA2_512 :
		auth_type = AUTH_TYPE_SHA2_512_ALL;
		if (ctx != NULL) {
			ctx->state[1] = 0xF3BCC908;
			ctx->state[0] = 0x6A09E667;
			ctx->state[3] = 0x84CAA73B;
			ctx->state[2] = 0xBB67AE85;
			ctx->state[5] = 0xFE94F82B;
			ctx->state[4] = 0x3C6EF372;
			ctx->state[7] = 0x5F1D36F1;
			ctx->state[6] = 0xA54FF53A;
			ctx->state[9] = 0xADE682D1;
			ctx->state[8] = 0x510E527F;
			ctx->state[11] = 0x2B3E6C1F;
			ctx->state[10] = 0x9B05688C;
			ctx->state[13] = 0xFB41BD6B;
			ctx->state[12] = 0x1F83D9AB;
			ctx->state[15] = 0x137E2179;
			ctx->state[14] = 0x5BE0CD19;
		}

		break;
	default:
		return FAIL;
	}

	// for sequential hash
	pIE->hmac_seq_is_recorded = 0;
	pIE->hmac_seq_buf_is_used_bytes = 0;

	ret = CRYPTO_SetSecurityModeAD(pIE, CIPHER_TYPE_NO_CIPHER, auth_type,
								   pCipherKey, lenCipherKey, pAuthKey, lenAuthKey);

	if (ctx != NULL) {
		hash_save(ctx);
		ctx->initial = 1;
	}

	return ret;
}

/**
  * @brief  SHA2 process.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of SHA2 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha2_process(IN const u8 *message, IN const u32 msglen, OUT u8 *pDigest)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA2) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	return CRYPTO_ProcessAD(pIE, message, msglen, NULL, 0, NULL, 0, pDigest, NULL, NULL);
}

/**
  * @brief  SHA2 update.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha2_update(IN u8 *message, IN u32 msglen, hw_sha_context *ctx)
{

	int ret = SUCCESS;
	while (msglen > 128 * 1024) {
		ret = rtl_crypto_sha2_update_rom(message, 128 * 1024, ctx);
		msglen = msglen - 128 * 1024;
		message += 128 * 1024;
	}
	if (msglen > 0) {
		ret = rtl_crypto_sha2_update_rom(message, msglen, ctx);
	}
	return ret;
}

/**
  * @brief  SHA2 update rom.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length < 128 *1024
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha2_update_rom(IN const u8 *message, IN const u32 msglen,  hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int ret = SUCCESS;

	ctx->seq_hash = 1;
	hash_restore(ctx);

	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	assert_param(message != NULL);
	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA2) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}
	/*
		if ( pIE->hmac_seq_is_recorded != 0 )  {
			ret = CRYPTO_SendSeqBuf(NULL);
			if ( ret != SUCCESS ) return ret;
		}
	*/
	pIE->hmac_seq_last_message = (u8 *)message;
	pIE->hmac_seq_last_msglen = msglen;
	pIE->hmac_seq_is_recorded = 1;

	ret = CRYPTO_SendSeqBuf((u8 *) & (ctx->state[0]), ctx);

	hash_save(ctx);

	return ret;
}

/**
  * @brief  SHA2 final process.
  * @param    pDigest	: the result of SHA2 function
  * @note    Use in sequential hash, process the last block
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha2_final(OUT u8 *pDigest, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	hash_restore(ctx);
	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA2) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if (pDigest == NULL) {
		return _ERRNO_CRYPTO_NULL_POINTER;
	}

	if (pIE->hmac_seq_is_recorded == 0) {
		return _ERRNO_CRYPTO_HASH_FINAL_NO_UPDATE;
	}

	ctx->lasthash = 1;
	//return CRYPTO_SendSeqBuf(pDigest);
	return CRYPTO_ProcessAD(pIE, (const uint8_t *)pIE->hmac_seq_buf, pIE->hmac_seq_buf_is_used_bytes, NULL, 0, NULL, 0, pDigest, NULL, ctx);
}

/**
  * @brief  SHA2 init & process
  * @param	sha2type	: This parameter can be one of the following values:
  *		 @arg SHA2_224
  *		 @arg SHA2_256
  *		 @arg SHA2_384
  *		 @arg SHA2_512
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of SHA2 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_sha2(IN const SHA2_TYPE sha2type, IN const u8 *message, IN const u32 msglen,
					OUT u8 *pDigest)
{
	int ret;

	ret = rtl_crypto_sha2_init(sha2type, NULL);
	if (ret != SUCCESS) {
		return ret;
	}

	ret = rtl_crypto_sha2_process(message, msglen, pDigest);

	return ret;
}

/**
  * @brief  HMAC-SHA1 init, set key
  * @param	key		: need to be 4 byte alignment
  * @param	keylen	: key length
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha1_init(IN const u8 *key, IN const u32 keylen, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	u8 *pCipherKey = NULL;
	u32 lenCipherKey = 0;
	u32 ret;
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)SHA_REG_BASE_S;
	}

	if (((IPSEC->IPSEKCR) & IPSEC_BIT_EFUSE_KEY_REQ) == 0) {
		if (key == NULL) {
			return _ERRNO_CRYPTO_NULL_POINTER;
		}
		if ((u32)(key) & 0x3) {
			return _ERRNO_CRYPTO_ADDR_NOT_4Byte_Aligned;    // need to be 4 byte alignment
		}
		if (keylen > CRYPTO_AUTH_PADDING) {
			return _ERRNO_CRYPTO_KEY_OutRange;
		}
	}

	assert_param(pIE->isInit == _TRUE);

	// for sequential hash
	pIE->hmac_seq_is_recorded = 0;
	pIE->hmac_seq_buf_is_used_bytes = 0;

	ret = CRYPTO_SetSecurityModeAD(pIE, CIPHER_TYPE_NO_CIPHER, AUTH_TYPE_HMAC_SHA1,
								   pCipherKey, lenCipherKey, key, keylen);

	if (ctx != NULL) {
		hash_save(ctx);
		for (int i = 0; i < 5; i++) {
			ctx->state[0] = 0x67452301;
			ctx->state[1] = 0xEFCDAB89;
			ctx->state[2] = 0x98BADCFE;
			ctx->state[3] = 0x10325476;
			ctx->state[4] = 0xC3D2E1F0;
		}

		ctx->initial = 1;
	}

	return ret;
}

/**
  * @brief  HMAC-SHA1 process.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of HMAC-SHA1 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha1_process(
	IN const u8 *message, 	IN const u32 msglen,
	OUT u8 *pDigest)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	if ((pIE->auth_type & AUTH_TYPE_MASK_HMAC) == 0) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA1) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	return CRYPTO_ProcessAD(pIE, message, msglen, NULL, 0, NULL, 0, pDigest, NULL, NULL);
}

/**
  * @brief  HMAC-SHA1 update.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha1_update(IN u8 *message, IN u32 msglen, hw_sha_context *ctx)
{
	int ret = SUCCESS;
	while (msglen > 128 * 1024) {
		ret = rtl_crypto_hmac_sha1_update_rom(message, 128 * 1024, ctx);
		msglen = msglen - 128 * 1024;
		message += 128 * 1024;
	}

	if (msglen > 0) {
		ret = rtl_crypto_hmac_sha1_update_rom(message, msglen, ctx);
	}
	return ret;

}

/**
  * @brief  HMAC-SHA1 update rom.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length < 128*1024
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha1_update_rom(IN const u8 *message, IN const u32 msglen, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int ret = SUCCESS;

	ctx->seq_hash = 1;
	hash_restore(ctx);
	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	assert_param(message != NULL);
	if ((pIE->auth_type & AUTH_TYPE_MASK_HMAC) == 0) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA1) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}
	/*
		if ( pIE->hmac_seq_is_recorded != 0 )  {
			ret = CRYPTO_SendSeqBuf(NULL);
			if ( ret != SUCCESS ) return ret;
		}
	*/
	pIE->hmac_seq_last_message = (u8 *)message;
	pIE->hmac_seq_last_msglen = msglen;
	pIE->hmac_seq_is_recorded = 1;
	ret = CRYPTO_SendSeqBuf((u8 *) & (ctx->state[0]), ctx);
	hash_save(ctx);

	return ret;
}

/**
  * @brief  HMAC-SHA1 final process.
  * @param    pDigest	: the result of HMAC-SHA1 function
  * @note    Use in sequential hash, process the last block
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha1_final(OUT u8 *pDigest, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	hash_restore(ctx);
	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	if ((pIE->auth_type & AUTH_TYPE_MASK_HMAC) == 0) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA1) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if (pDigest == NULL) {
		return _ERRNO_CRYPTO_NULL_POINTER;
	}

	if (pIE->hmac_seq_is_recorded == 0) {
		return _ERRNO_CRYPTO_HASH_FINAL_NO_UPDATE;
	}
	ctx->lasthash = 1;
	//return CRYPTO_SendSeqBuf(pDigest);
	return CRYPTO_ProcessAD(pIE, (const uint8_t *)pIE->hmac_seq_buf, pIE->hmac_seq_buf_is_used_bytes, NULL, 0, NULL, 0, pDigest, NULL, NULL);
}

/**
  * @brief  HMAC-SHA1 init & process
  * @param	key		: need to be 4 byte alignment
  * @param	keylen	: key length
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of HMAC-SHA1 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha1(
	IN const u8 *message, 	IN const u32 msglen,
	IN const u8 *key, 		IN const u32 keylen,
	OUT u8 *pDigest)
{
	int ret;

	ret = rtl_crypto_hmac_sha1_init(key, keylen, NULL);
	if (ret != SUCCESS) {
		return ret;
	}

	ret = rtl_crypto_hmac_sha1_process(message, msglen, pDigest);

	return ret;
}

/**
  * @brief  HMAC-SHA2 init, set key
  * @param	key		: need to be 4 byte alignment
  * @param	keylen	: key length
  * @param	sha2type	: This parameter can be one of the following values:
  *		 @arg SHA2_224
  *		 @arg SHA2_256
  *		 @arg SHA2_384
  *		 @arg SHA2_512
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha2_init(
	IN const SHA2_TYPE sha2type,
	IN const u8 *key, 	IN const u32 keylen, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int auth_type;
	u8 *pCipherKey = NULL;
	u32 lenCipherKey = 0;
	u32 ret;
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)SHA_REG_BASE_S;
	}

	if (((IPSEC->IPSEKCR) & IPSEC_BIT_EFUSE_KEY_REQ) == 0) {
		if (key == NULL) {
			return _ERRNO_CRYPTO_NULL_POINTER;
		}
		if ((u32)(key) & 0x3) {
			return _ERRNO_CRYPTO_ADDR_NOT_4Byte_Aligned;    // need to be 4 byte alignment
		}
	}

	if (keylen > CRYPTO_AUTH_PADDING) {
		return _ERRNO_CRYPTO_KEY_OutRange;
	}

	assert_param(pIE->isInit == _TRUE);

	switch (sha2type) {
	case SHA2_224 :
		auth_type = AUTH_TYPE_HMAC_SHA2_224_ALL;
		if (ctx != NULL) {
			ctx->state[0] = 0xC1059ED8;
			ctx->state[1] = 0x367CD507;
			ctx->state[2] = 0x3070DD17;
			ctx->state[3] = 0xF70E5939;
			ctx->state[4] = 0xFFC00B31;
			ctx->state[5] = 0x68581511;
			ctx->state[6] = 0x64F98FA7;
			ctx->state[7] = 0xBEFA4FA4;
		}
		break;
	case SHA2_256 :
		auth_type = AUTH_TYPE_HMAC_SHA2_256_ALL;
		if (ctx != NULL) {
			ctx->state[0] = 0x6A09E667;
			ctx->state[1] = 0xBB67AE85;
			ctx->state[2] = 0x3C6EF372;
			ctx->state[3] = 0xA54FF53A;
			ctx->state[4] = 0x510E527F;
			ctx->state[5] = 0x9B05688C;
			ctx->state[6] = 0x1F83D9AB;
			ctx->state[7] = 0x5BE0CD19;
		}
		break;
	case SHA2_384 :
		auth_type = AUTH_TYPE_HMAC_SHA2_384_ALL;
		if (ctx != NULL) {
			ctx->state[1] = 0xC1059ED8;
			ctx->state[0] = 0xCBBB9D5D;
			ctx->state[3] = 0x367CD507;
			ctx->state[2] = 0x629A292A;
			ctx->state[5] = 0x3070DD17;
			ctx->state[4] = 0x9159015A;
			ctx->state[7] = 0xF70E5939;
			ctx->state[6] = 0x152FECD8;
			ctx->state[9] = 0xFFC00B31;
			ctx->state[8] = 0x67332667;
			ctx->state[11] = 0x68581511;
			ctx->state[10] = 0x8EB44A87;
			ctx->state[13] = 0x64F98FA7;
			ctx->state[12] = 0xDB0C2E0D;
			ctx->state[15] = 0xBEFA4FA4;
			ctx->state[14] = 0x47B5481D;

		}
		break;
	case SHA2_512 :
		auth_type = AUTH_TYPE_HMAC_SHA2_512_ALL;
		if (ctx != NULL) {
			ctx->state[1] = 0xF3BCC908;
			ctx->state[0] = 0x6A09E667;
			ctx->state[3] = 0x84CAA73B;
			ctx->state[2] = 0xBB67AE85;
			ctx->state[5] = 0xFE94F82B;
			ctx->state[4] = 0x3C6EF372;
			ctx->state[7] = 0x5F1D36F1;
			ctx->state[6] = 0xA54FF53A;
			ctx->state[9] = 0xADE682D1;
			ctx->state[8] = 0x510E527F;
			ctx->state[11] = 0x2B3E6C1F;
			ctx->state[10] = 0x9B05688C;
			ctx->state[13] = 0xFB41BD6B;
			ctx->state[12] = 0x1F83D9AB;
			ctx->state[15] = 0x137E2179;
			ctx->state[14] = 0x5BE0CD19;
		}
		break;

	default:
		return FAIL;
	}

	// for sequential hash
	pIE->hmac_seq_is_recorded = 0;
	pIE->hmac_seq_buf_is_used_bytes = 0;
	ret = CRYPTO_SetSecurityModeAD(pIE, CIPHER_TYPE_NO_CIPHER, auth_type,
								   pCipherKey, lenCipherKey, key, keylen);

	if (ctx != NULL) {
		hash_save(ctx);
		ctx->initial = 1;
	}

	return ret;
}

/**
  * @brief  HMAC-SHA2 process.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of HMAC-SHA2 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha2_process(
	IN const u8 *message, 	IN const u32 msglen,
	OUT u8 *pDigest)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	if ((pIE->auth_type & AUTH_TYPE_MASK_HMAC) == 0) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA2) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	return CRYPTO_ProcessAD(pIE, message, msglen, NULL, 0, NULL, 0, pDigest, NULL, NULL);
}

/**
  * @brief  HMAC-SHA2 update.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha2_update(IN u8 *message, IN u32 msglen, hw_sha_context *ctx)
{
	int ret = SUCCESS;

	while (msglen > 128 * 1024) {
		ret = rtl_crypto_hmac_sha2_update_rom(message, 128 * 1024, ctx);
		msglen = msglen - 128 * 1024;
		message += 128 * 1024;
	}
	if (msglen > 0) {
		ret = rtl_crypto_hmac_sha2_update_rom(message, msglen, ctx);
	}
	return ret;
}
/**
  * @brief  HMAC-SHA2 update rom.
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length < 128*1024
  * @note    Use in sequential hash
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha2_update_rom(IN const u8 *message, IN const u32 msglen, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int ret = SUCCESS;

	ctx->seq_hash = 1;
	hash_restore(ctx);
	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	assert_param(message != NULL);
	if ((pIE->auth_type & AUTH_TYPE_MASK_HMAC) == 0) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA2) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}
	/*
		if ( pIE->hmac_seq_is_recorded != 0 )  {
			ret = CRYPTO_SendSeqBuf(NULL);
			if ( ret != SUCCESS ) return ret;
		}
	*/
	pIE->hmac_seq_last_message = (u8 *)message;
	pIE->hmac_seq_last_msglen = msglen;
	pIE->hmac_seq_is_recorded = 1;
	ret = CRYPTO_SendSeqBuf((u8 *) & (ctx->state[0]), ctx);
	hash_save(ctx);

	return ret;
}

/**
  * @brief  HMAC-SHA2 final process.
  * @param    pDigest	: the result of HMAC-SHA2 function
  * @note    Use in sequential hash, process the last block
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha2_final(OUT u8 *pDigest, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	hash_restore(ctx);
	assert_param((pIE != NULL) && (pIE->isInit == _TRUE));
	if ((pIE->auth_type & AUTH_TYPE_MASK_HMAC) == 0) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if ((pIE->auth_type & AUTH_TYPE_MASK_FUNC) != AUTH_TYPE_SHA2) {
		return _ERRNO_CRYPTO_AUTH_TYPE_NOT_MATCH;
	}

	if (pDigest == NULL) {
		return _ERRNO_CRYPTO_NULL_POINTER;
	}

	if (pIE->hmac_seq_is_recorded == 0) {
		return _ERRNO_CRYPTO_HASH_FINAL_NO_UPDATE;
	}

	ctx->lasthash = 1;

	return CRYPTO_ProcessAD(pIE, (const uint8_t *)pIE->hmac_seq_buf, pIE->hmac_seq_buf_is_used_bytes, NULL, 0, NULL, 0, pDigest, NULL, ctx);
}

/**
  * @brief  HMAC-SHA2 init & process
  * @param	key		: need to be 4 byte alignment
  * @param	keylen	: key length
  * @param	sha2type	: This parameter can be one of the following values:
  *		 @arg SHA2_224
  *		 @arg SHA2_256
  *		 @arg SHA2_384
  *		 @arg SHA2_512
  * @param 	message : input buffer
  * @param    msglen 	: input buffer length
  * @param    pDigest	: the result of HMAC-SHA2 function
  * @retval	0		: SUCCESS <br>
  *			others	: fail, refer to ERRNO
  */
__weak HAL_ROM_TEXT_SECTION
int rtl_crypto_hmac_sha2(
	IN const SHA2_TYPE sha2type,
	IN const u8 *message, 			IN const u32 msglen,
	IN const u8 *key, 				IN const u32 keylen,
	OUT u8 *pDigest)
{
	int ret;

	ret = rtl_crypto_hmac_sha2_init(sha2type, key, keylen, NULL);
	if (ret != SUCCESS) {
		return ret;
	}

	ret = rtl_crypto_hmac_sha2_process(message, msglen, pDigest);

	return ret;
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
