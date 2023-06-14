/**
  ******************************************************************************
  * @file    rtl8721dhp_crypto_rom.c
  * @author
  * @version V1.0.0
  * @date    2017-10-10
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the HW crypto:
  *           - Crypto initialization
  *           - Auth and cipher initialization
  *           - Descriptor setting
  *           - key/IV/PAD setting
  *           - Auth process/update/final
  *           - Cipher encrypt/decrypt
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

HAL_ROM_BSS_SECTION
ALIGNMTO(32) static u8 ipsec_padding[64]  = { 0x0 };

HAL_ROM_BSS_SECTION
HAL_CRYPTO_ADAPTER crypto_engine ALIGNMTO(32);

HAL_ROM_BSS_SECTION
HAL_CRYPTO_ADAPTER crypto_sha_engine ALIGNMTO(32);

HAL_ROM_BSS_SECTION
u32 SHA_OTPKEY = 0;

/**
  * @brief  Sequential hash process.
  * @param  pDigest: Sequential hash result.
  * @note  There exist two buffers : last_message and seq_buf;
  *             last_message : store the previous message pointer.
  *             seq_buf : store the data less than 64.
  * @retval Process status.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_SendSeqBuf(u8 *pDigest, hw_sha_context *ctx)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;
	int ret = SUCCESS;
	const u8 *pIV 	= NULL;
	const u32 ivlen = 0;
	const u32 a2eo 	= 0;
	int rest_bytes = 0;
	int bodylen;
	int restlen;

	int total_len = pIE->hmac_seq_last_msglen;
	int buf_pos = pIE->hmac_seq_buf_is_used_bytes;
	int blocksize = 64;
	if (((ctx->sha2type == SHA2_384) || (ctx->sha2type == SHA2_512)) != 0) {
		blocksize = 128;
	} else {
		blocksize = 64;
	}
	rest_bytes = blocksize - buf_pos;

	if (total_len < rest_bytes) {	//store into seq buf
		_memcpy((void *)(&pIE->hmac_seq_buf[buf_pos]), (const void *)(pIE->hmac_seq_last_message), total_len);
		pIE->hmac_seq_buf_is_used_bytes += total_len;
	} else {
		// send out a seq buf if the seq buf is the last and length is blobk-byte
		_memcpy((void *)(&pIE->hmac_seq_buf[buf_pos]), (const void *)(pIE->hmac_seq_last_message), rest_bytes);
		ret =  CRYPTO_ProcessAD(pIE, (u8 *)(pIE->hmac_seq_buf), blocksize, pIV, ivlen, NULL, a2eo, pDigest, NULL, ctx);
		if (ret != SUCCESS) {
			return ret;
		}

		pIE->hmac_seq_buf_is_used_bytes = 0;
		buf_pos = 0;

		total_len -= rest_bytes;
		pIE->hmac_seq_last_msglen = total_len;
		pIE->hmac_seq_last_message += rest_bytes;

		//send out all blocksize-byte align message
		restlen = total_len & (blocksize - 1);
		bodylen = total_len - restlen;
		if (bodylen > 0) {   // there are 64x messages
			ret =  CRYPTO_ProcessAD(pIE, pIE->hmac_seq_last_message, bodylen, pIV, ivlen, NULL, a2eo, pDigest, NULL, ctx);
			if (ret != SUCCESS) {
				return ret;
			}
			pIE->hmac_seq_last_message += bodylen;
		}

		// backup the rest
		if (restlen > 0) {
			_memcpy((void *)(&pIE->hmac_seq_buf[0]), (const void *)(pIE->hmac_seq_last_message), restlen);
		}
		pIE->hmac_seq_buf_is_used_bytes = restlen;
	}

	return ret;
}

/**
  * @brief  Crypto memory dump.
  * @param  start: Point to start address of memory to be dumped.
  * @param  size: The memory length.
  * @param  strHeader: String at the begining of dump.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_MemDump(const u8 *start, u32 size, char *strHeader)
{
	int row, column, index, index2, max;
	u8 *buf, *line;

	if (!start || (size == 0)) {
		return;
	}

	line = (u8 *)start;

	/*
	16 bytes per line
	*/
	if (strHeader) {
		DBG_8195A("%s", strHeader);
	}

	column = size % 16;
	row = (size / 16) + 1;
	for (index = 0; index < row; index++, line += 16) {
		buf = (u8 *)line;

		max = (index == row - 1) ? column : 16;
		if (max == 0) {
			break;    /* If we need not dump this line, break it. */
		}

		DBG_8195A("\n[%08x] ", line);

		//Hex
		for (index2 = 0; index2 < max; index2++) {
			if (index2 == 8) {
				DBG_8195A("  ");
			}
			DBG_8195A("%02x ", (u8) buf[index2]);
		}

		if (max != 16) {
			if (max < 8) {
				DBG_8195A("  ");
			}
			for (index2 = 16 - max; index2 > 0; index2--) {
				DBG_8195A("   ");
			}
		}

	}

	DBG_8195A("\n");
}

/**
  * @brief  Set source descriptor.
  * @param  sd1: Source descriptor first word.
  * @param  sdpr: Source descriptor second word.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_SetSrcDesc(u32 sd1, u32 sdpr, u32 engine_type)
{
	u32 timeout;
	IPSEC_TypeDef *IPSEC = NULL;

	if (engine_type == AES_ENGINE) {
		IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) AES_REG_BASE_S;
		}
	} else {
		IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE_S;
		}
	}

	timeout = FIFOCNT_TIMEOUT;
	while (1) {
		if ((IPSEC->SDSR & IPSEC_MASK_SRC_DESC_FIFO_EMPTY_CNT) > 0) {
			DBG_PRINTF(MODULE_IPSEC, LEVEL_INFO, "sd1=0x%x , sdpr=0x%x \r\n", sd1, sdpr);
			IPSEC->SDFWR = sd1;
			IPSEC->SDSWR = sdpr;
			break;
		}
		timeout--;
		if (timeout == 0) {
			DBG_PRINTF(MODULE_IPSEC, LEVEL_ERROR, "Timeout, src fifo is FULL \r\n");
			break;
		}
	}
}

/**
  * @brief  Set destination descriptor.
  * @param  dd1: Destination descriptor first word.
  * @param  ddpr: Destination descriptor second word.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_SetDstDesc(u32 dd1, u32 ddpr, u32 engine_type)
{
	IPSEC_TypeDef *IPSEC = NULL;

	if (engine_type == AES_ENGINE) {
		IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) AES_REG_BASE_S;
		}
	} else {
		IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE_S;
		}
	}

	if ((IPSEC->DDSR & IPSEC_MASK_DES_DES_FIFO_EMPTY_CNT) > 0) {
		DBG_PRINTF(MODULE_IPSEC, LEVEL_INFO, "dd1=0x%x , ddpr=0x%x \r\n", dd1, ddpr);
		IPSEC->DDFWR = dd1;
		IPSEC->DDSWR = ddpr;
	} else {
		DBG_PRINTF(MODULE_IPSEC, LEVEL_ERROR, "dst fifo_cnt is not correct: %d \r\n", (IPSEC->DDSR & IPSEC_MASK_DES_DES_FIFO_EMPTY_CNT));
	}
}

/**
  * @brief  Clear crypto engine command OK interrupt.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_CleanCmdOk(u32 engine_type)
{
	IPSEC_TypeDef *IPSEC = NULL;

	if (engine_type == AES_ENGINE) {
		IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) AES_REG_BASE_S;
		}
	} else {
		IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE_S;
		}
	}

	u32 ok_intr_cnt = 0;
	ok_intr_cnt = IPSEC_GET_OK_INTR_CNT(IPSEC->IPSCSR_RSTEACONFISR);
	IPSEC->IPSCSR_RSTEACONFISR |= IPSEC_CLEAR_OK_INT_NUM(ok_intr_cnt);
	IPSEC->IPSCSR_RSTEACONFISR |= IPSEC_BIT_CMD_OK;
}

/**
  * @brief  Clear command ok and error interrupts.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_ClearAllINT(u32 engine_type)
{
	IPSEC_TypeDef *IPSEC = NULL;

	if (engine_type == AES_ENGINE) {
		IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) AES_REG_BASE_S;
		}
	} else {
		IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE_S;
		}
	}
	IPSEC->IPSCSR_ERR_INT = 0xFFFF;
	//IPSEC->RST_ISR_CON |= CMD_OK;
	CRYPTO_CleanCmdOk(engine_type);
}

/**
  * @brief  Set source descriptor command buffer.
  * @param  pIE: Point to crypto adapter.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_SetCmdBuf(HAL_CRYPTO_ADAPTER *pIE)
{
	rtl_crypto_cl_t *pCL;
	u32 a2eo;
	u32 enl;
	u32 blocksize = 64;

	a2eo = pIE->a2eo;
	enl = pIE->enl;

	pCL = (rtl_crypto_cl_t *)pIE->cl_buffer;

	_memset((u8 *)pCL, 0, sizeof(pIE->cl_buffer));

	if (pIE->auth_type != AUTH_TYPE_NO_AUTH) {	//authentication
		pCL->engine_mode = 1; //hash only
		// Sequential hash setting
		pCL->hmac_seq_hash = 1;
		pCL->hmac_seq_hash_first = 0;
		if (pIE->hmac_seq_hash_first == 1) {
			pCL->hmac_seq_hash_first = 1;
			pIE->hmac_seq_hash_first = 0;
		}

		//Swap settting
		if (pIE->isMD5) {
			pCL->habs = 1;
			pCL->hibs = 1;
			pCL->hobs = 1;
			pCL->hkbs = 1;
		} else if (pIE->isSHA1) {
			pCL->hmac_mode = 1;
		} else if (pIE->sha2type != SHA2_NONE) {
			if (pIE->sha2type == SHA2_224) {
				pCL->hmac_mode = 0x2;
			} else if (pIE->sha2type == SHA2_256) {
				pCL->hmac_mode = 0x3;
			} else if (pIE->sha2type == SHA2_384) {
				pCL->hmac_mode = 0x4;
				blocksize = 128;
			} else if (pIE->sha2type == SHA2_512) {
				pCL->hmac_mode = 0x5;
				blocksize = 128;
			}
		}

		if (pIE->isHMAC) {
			pCL->hmac_en = 1;
		}
		// Use auto padding and sequential hash III
		if (blocksize == 64) {
			// Use auto padding and sequential hash III
			if (pIE->hmac_seq_hash_last == 1) {	//The last block autopadding: 16-byte
				// always using auto padding
				pCL->enl = (enl + 15) / 16 ;
				pCL->enc_last_data_size = pIE->enc_last_data_size;
				pCL->ap0 = pIE->hmac_seq_hash_total_len * 8;
				if (pIE->isHMAC) {
					pCL->ap0 += blocksize * 8;
				}
			} else {	//MD5/SHA1/SHA224/SHA256: 64-byte
				pCL->enl = enl / 64;
			}
		} else if (blocksize == 128) {
			// Use auto padding and sequential hash III
			if (pIE->hmac_seq_hash_last == 1) {	//The last block autopadding: 16-byte
				// always using auto padding
				pCL->enl = (enl + 15) / 16 ;
				pCL->enc_last_data_size = pIE->enc_last_data_size;
				pCL->ap0 = pIE->hmac_seq_hash_total_len * 8;
				if (pIE->isHMAC) {
					pCL->ap0 += blocksize * 8;
				}
			} else {
				pCL->enl = enl / 128;
			}

		}

		pCL->hmac_seq_hash_last = 0;
		pCL->hmac_seq_hash_no_wb = 0;
		if (pIE->hmac_seq_hash_last == 1) {
			pCL->hmac_seq_hash_last = 1;
		}
	} else { // cipher - encryption / decryption
		u32 cipher_type;
		u32 block_mode;
		//cipher mode: 0x0 : ECB, 0x1: CBC, 0x2: CFB , 0x3 : OFB , 0x4 : CTR, 0x5 : GCTR, 0x6: GMAC, 0x7: GHASH, 0x8: GCM
		//0 - chacha20+poly1305, 1 - chacha20, 2 - poly1305 mode 1, 3 - poly1305 mode 2
		cipher_type = pIE->cipher_type;
		block_mode = cipher_type & CIPHER_TYPE_MASK_BLOCK;
		pCL->cipher_mode = block_mode;
		//AES key length
		if (pIE->aes) {
			pCL->cipher_eng_sel = 0;
			switch (pIE->lenCipherKey) {
			case 128/8 :
				pCL->aes_key_sel = 0;
				break;
			case 192/8 :
				pCL->aes_key_sel = 1;
				break;
			case 256/8 :
				pCL->aes_key_sel = 2;
				break;
			default:
				break;
			}

			pCL->enl = (enl + 15) / 16;
			pCL->enc_last_data_size = pIE->enc_last_data_size;

			if (block_mode ==  CIPHER_TYPE_BLOCK_GCM) {
				pCL->header_total_len = (a2eo + 15) / 16;
				pCL->aad_last_data_size = pIE->aad_last_data_size;
			}
		}

		if (pIE->isDecrypt == 0) {
			pCL->cipher_encrypt = 1;
		}

	}

	pCL->icv_total_length = 0x40; // for mix mode, but need to set a value 0x40
}

/**
  * @brief  Set source descriptor command, key, IV, pad array.
  * @param  pIE: Point to crypto adapter.
  * @param  p_iv: IV(initialization vector).
  * @param  ivLen: IV length.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_SrcDescKeyIv(HAL_CRYPTO_ADAPTER *pIE, const u8 *p_iv, u32 ivLen, hw_sha_context *ctx, u32 engine_type)
{
	rtl_crypto_srcdesc_t src_desc;
	//u32 blocksize = 64;

	src_desc.w = 0;

	// FS=1, CL=3
	src_desc.b.rs = 1;
	src_desc.b.fs = 1;
	src_desc.b.cl = 3;

	// auto padding
	if (pIE->hmac_seq_hash_last) {
		src_desc.b.ap = 0x01;
	}

	//Set command buffer array
	CRYPTO_SetCmdBuf(pIE);
	DCache_CleanInvalidate((u32)(&(pIE->cl_buffer)[0]), sizeof(pIE->cl_buffer));
	crypto_dbg_mem_dump((u32)(&(pIE->cl_buffer)[0]), sizeof(pIE->cl_buffer), "Command Setting: ");
	CRYPTO_SetSrcDesc(src_desc.w, (u32)(&(pIE->cl_buffer)[0]), engine_type);

	// Set key array
	if (pIE->cipher_type != CIPHER_TYPE_NO_CIPHER) {	//cipher
		u32 lenCipherKey;
		u8  *pCipherKey;

		lenCipherKey = (u32) pIE->lenCipherKey;
		pCipherKey = (u8 *) pIE->pCipherKey;

		src_desc.w = 0;
		src_desc.b.rs = 1;
		src_desc.b.fs = 1;
		src_desc.b.key_len = lenCipherKey / 4;
		DCache_CleanInvalidate((u32)(pCipherKey), lenCipherKey);
		crypto_dbg_mem_dump((u32)(pCipherKey), lenCipherKey, "KEY: ");
		CRYPTO_SetSrcDesc(src_desc.w, (u32)(pCipherKey), engine_type);
	}

	// Set IV array
	if (p_iv != NULL && ivLen > 0) {
		src_desc.w = 0;
		src_desc.b.rs = 1;
		src_desc.b.fs = 1;
		src_desc.b.iv_len = ivLen / 4;
		DCache_CleanInvalidate((u32)(p_iv), ivLen);
		crypto_dbg_mem_dump((u32)(p_iv), ivLen, "IV: ");
		CRYPTO_SetSrcDesc(src_desc.w, (u32)(p_iv), engine_type);
	}

	// Set Pad array
	if (pIE->isHMAC) {
		src_desc.w = 0;
		src_desc.b.rs = 1;
		src_desc.b.fs = 1;
		src_desc.b.keypad_len = 64;
		DCache_CleanInvalidate((u32)(&(pIE->g_IOPAD[0])), sizeof(pIE->g_IOPAD));
		crypto_dbg_mem_dump((u32)(&(pIE->g_IOPAD[0])), 128, "PAD: ");
		CRYPTO_SetSrcDesc(src_desc.w, (u32)(&(pIE->g_IOPAD[0])), engine_type);
	}

	//set initial value
	if (ctx != NULL && ctx->seq_hash == 1) {
		u32 len = pIE->digestlen;
		if (len == 28) {
			len = 32;
		} else if (len == 48) {
			len = 64;
		}
		if (ctx->initial == 1) {
			ctx->initial = 0;
		} else if (ctx->isMD5 != 1) {
			for (unsigned int i = 0; i < (len / 4); i++) {
				ctx->state[i] = ((u32)(
									 (((u32)(ctx->state[i]) & (u32)0x000000ffUL) << 24) |
									 (((u32)(ctx->state[i]) & (u32)0x0000ff00UL) <<  8) |
									 (((u32)(ctx->state[i]) & (u32)0x00ff0000UL) >>  8) |
									 (((u32)(ctx->state[i]) & (u32)0xff000000UL) >> 24)));
			}
		}

		DCache_CleanInvalidate((u32)(&(ctx->state[0])), len);

		src_desc.w = 0;
		src_desc.b.rs = 1;
		src_desc.b.fs = 1;
		src_desc.b.hash_iv_len = len / 4;
		CRYPTO_SetSrcDesc(src_desc.w, (u32)(&(ctx->state[0])), engine_type);
	}

}

/**
  * @brief  Crypto engine reset.
  * @param  pIE: Point to crypto adapter.
  * @note pIE is no need in Ameba-D reset api, but in order to compatible with AmebaZ reset api, pIE is added.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_Reset(UNUSED_WARN_DIS HAL_CRYPTO_ADAPTER *pIE)
{
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)AES_REG_BASE_S;
	}

	// Crypto engine : Software Reset
	IPSEC->IPSCSR_RSTEACONFISR = IPSEC_BIT_SOFT_RST;
	// register offset 0x030
	IPSEC->IPSCSR_SWAPABURSTR = IPSEC_DMA_BURST_LENGTH(16);
	IPSEC->IPSCSR_SWAPABURSTR |= (IPSEC_BIT_KEY_IV_SWAP | IPSEC_BIT_KEY_PAD_SWAP);
	IPSEC->IPSCSR_SWAPABURSTR |= (IPSEC_BIT_DATA_IN_LITTLE_ENDIAN | IPSEC_BIT_DATA_OUT_LITTLE_ENDIAN | IPSEC_BIT_MAC_OUT_LITTLE_ENDIAN);

	// ++++ register offset 0x018 +++++
	// Crypto Engine : DMA arbiter(detect fifo wasted level) , clock enable
	IPSEC->IPSCSR_DBG = (IPSEC_BIT_ARBITER_MODE | IPSEC_BIT_ENGINE_CLK_EN);
}

/**
  * @brief  SHA engine reset.
  * @param  pIE: Point to crypto adapter.
  * @note pIE is no need in Ameba-D reset api, but in order to compatible with AmebaZ reset api, pIE is added.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void CRYPTO_SHA_Reset(UNUSED_WARN_DIS HAL_CRYPTO_ADAPTER *pIE)
{
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)SHA_REG_BASE_S;
	}

	// Crypto engine : Software Reset
	IPSEC->IPSCSR_RSTEACONFISR = IPSEC_BIT_SOFT_RST;
	// register offset 0x030
	IPSEC->IPSCSR_SWAPABURSTR = IPSEC_DMA_BURST_LENGTH(16);
	IPSEC->IPSCSR_SWAPABURSTR |= (IPSEC_BIT_KEY_IV_SWAP | IPSEC_BIT_KEY_PAD_SWAP);
	IPSEC->IPSCSR_SWAPABURSTR |= (IPSEC_BIT_DATA_IN_LITTLE_ENDIAN | IPSEC_BIT_DATA_OUT_LITTLE_ENDIAN | IPSEC_BIT_MAC_OUT_LITTLE_ENDIAN);

	// ++++ register offset 0x018 +++++
	// Crypto Engine : DMA arbiter(detect fifo wasted level) , clock enable
	IPSEC->IPSCSR_DBG = (IPSEC_BIT_ARBITER_MODE | IPSEC_BIT_ENGINE_CLK_EN);
}

/**
  * @brief  Set crypto adapter parameter.
  * @param  pIE: Point to crypto adapter.
  * @param  cipher_type: Cipher type.
  *		 @arg CIPHER_TYPE_AES_ECB: AES_ECB
  *		 @arg CIPHER_TYPE_AES_CBC: AES_CBC
  *		 @arg CIPHER_TYPE_AES_CFB: AES_CFB
  *		 @arg CIPHER_TYPE_AES_OFB: AES_OFB
  *		 @arg CIPHER_TYPE_AES_CTR: AES_CTR
  *		 @arg CIPHER_TYPE_AES_GCTR: AES_GCTR
  *		 @arg CIPHER_TYPE_AES_GMAC: AES_GMAC
  *		 @arg CIPHER_TYPE_AES_GHASH: AES_GHASH
  *		 @arg CIPHER_TYPE_AES_GCM: AES_GCM
  * @param  auth_type: Authentication algorithm type.
  *		 @arg AUTH_TYPE_MD5: MD5
  *		 @arg AUTH_TYPE_SHA1: SHA1
  *		 @arg AUTH_TYPE_SHA2_224_ALL: SHA2_224
  *		 @arg AUTH_TYPE_SHA2_256_ALL: SHA2_256
  *		 @arg AUTH_TYPE_HMAC_MD5: HMAC MD5
  *		 @arg AUTH_TYPE_HMAC_SHA1: HMAC SHA1
  *		 @arg AUTH_TYPE_HMAC_SHA2_224_ALL: HMAC SHA2_224
  *		 @arg AUTH_TYPE_HMAC_SHA2_256_ALL: HMAC SHA2_256
  * @param  pCipherKey: Point to cipher key.
  * @param  lenCipherKey: Cipher key length.
  * @param  pAuthKey: Point to authentication key.
  * @param  lenAuthKey: Authentication key length.
  * @retval SUCCESS or error status
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_SetSecurityModeAD(HAL_CRYPTO_ADAPTER *pIE,
							 IN const u32 cipher_type, IN const u32 auth_type,
							 IN const void *pCipherKey, IN const u32 lenCipherKey,
							 IN const void *pAuthKey, IN const u32 lenAuthKey)
{
	assert_param(pIE != NULL);
	assert_param(pIE->isInit == _TRUE);
	u32 i;

	if (lenAuthKey > 0) {   // Authentication
		assert_param(lenAuthKey <= CRYPTO_AUTH_PADDING);
	}

	// Cipher
	pIE->aes	 = 0;
	pIE->isDecrypt	 = 0;
	pIE->cipher_type = cipher_type;

	if (cipher_type != CIPHER_TYPE_NO_CIPHER) {
		switch (cipher_type & CIPHER_TYPE_MASK_FUNC) {
		case CIPHER_TYPE_FUNC_AES :
			pIE->aes = 1;
			break;
		}
		pIE->isDecrypt  = (cipher_type & CIPHER_TYPE_MODE_ENCRYPT) ? 0 : 1;
	}

	// Auth
	pIE->auth_type = auth_type;
	if (auth_type != AUTH_TYPE_NO_AUTH) {
		pIE->isHMAC = (auth_type & AUTH_TYPE_MASK_HMAC) ? 1 : 0;
		pIE->isMD5 = ((auth_type & AUTH_TYPE_MASK_FUNC) == AUTH_TYPE_MD5) ? 1 : 0;
		pIE->isSHA1 = ((auth_type & AUTH_TYPE_MASK_FUNC) == AUTH_TYPE_SHA1) ? 1 : 0;
		pIE->isSHA2 = ((auth_type & AUTH_TYPE_MASK_FUNC) == AUTH_TYPE_SHA2) ? 1 : 0;
		if (pIE->isSHA2) {
			switch (auth_type & AUTH_TYPE_MASK_SHA2) {
			case AUTH_TYPE_SHA2_224 :
				pIE->sha2type = SHA2_224;
				break;
			case AUTH_TYPE_SHA2_256 :
				pIE->sha2type = SHA2_256;
				break;
			case AUTH_TYPE_SHA2_384 :
				pIE->sha2type = SHA2_384;
				break;
			case AUTH_TYPE_SHA2_512 :
				pIE->sha2type = SHA2_512;
				break;
			default :
				DBG_PRINTF(MODULE_IPSEC, LEVEL_ERROR, "No this auth_type(%d) for SHA2\n", auth_type);
				pIE->sha2type = SHA2_NONE;
				break;
			}
		} else {
			pIE->sha2type = SHA2_NONE;
		}
	} else {
		pIE->isMD5 = 0;
		pIE->isHMAC = 0;
		pIE->isSHA1 = 0;
		pIE->isSHA2 = 0;
		pIE->sha2type = SHA2_NONE;
	}

	//Set hash algorithm digest length.
	switch (auth_type &  AUTH_TYPE_MASK_FUNC) {
	case AUTH_TYPE_MD5:
		pIE->digestlen = CRYPTO_MD5_DIGEST_LENGTH;
		break;
	case AUTH_TYPE_SHA1:
		pIE->digestlen = CRYPTO_SHA1_DIGEST_LENGTH;
		break;
	case AUTH_TYPE_SHA2:
		pIE->digestlen = pIE->sha2type;
		break;
	default:
		pIE->digestlen = 0;
		break;
	}

	// Set auth key and cipher key.
	pIE->lenCipherKey = lenCipherKey;
	pIE->pCipherKey = pCipherKey;

	pIE->lenAuthKey = lenAuthKey;

	//Authkey is always used in HMAC algorithm to generate auth ipad and opad.
	if (lenAuthKey > 0) {
		if ((pIE->sha2type == SHA2_384) || (pIE->sha2type == SHA2_512)) {
			pIE->ipad = (u8 *)(&(pIE->g_IOPAD[0]));
			pIE->opad = (u8 *)(&(pIE->g_IOPAD[CRYPTO_PADSIZE_512]));
			_memset(pIE->ipad, 0x36, CRYPTO_PADSIZE_512);
			_memset(pIE->opad, 0x5c, CRYPTO_PADSIZE_512);
		} else {
			pIE->ipad = (u8 *)(&(pIE->g_IOPAD[0]));
			pIE->opad = (u8 *)(&(pIE->g_IOPAD[CRYPTO_PADSIZE]));
			_memset(pIE->ipad, 0x36, CRYPTO_PADSIZE);
			_memset(pIE->opad, 0x5c, CRYPTO_PADSIZE);
		}

		if (SHA_OTPKEY == 0) {
			assert_param(pAuthKey != NULL);
			pIE->pAuthKey = pAuthKey;
			for (i = 0; i < lenAuthKey; i++) {
				pIE->ipad[i] ^= ((u8 *) pIE->pAuthKey)[i];
				pIE->opad[i] ^= ((u8 *) pIE->pAuthKey)[i];
			}
		}
	} else {
		pIE->ipad = 0;
		pIE->opad = 0;
	}

	// Sequential hash parameter init
	if (auth_type != AUTH_TYPE_NO_AUTH) {
		pIE->hmac_seq_hash_first = 1;
		pIE->hmac_seq_hash_last = 0;
		pIE->hmac_seq_hash_total_len = 0;
	}
	return SUCCESS;
}

/**
  * @brief  Crypto engine initialize.
  * @param  pIE: Point to crypto adapter, Ameba-D should be NULL.
  * @retval SUCCESS
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_Init(HAL_CRYPTO_ADAPTER *pIE_I)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_engine;

	if (pIE_I != NULL) {
		pIE = pIE_I;
	}

	CRYPTO_Reset(pIE);

	pIE->isInit = 1;
	pIE->isIntMode = 0;
	return SUCCESS;
}

/**
  * @brief  Crypto engine initialize.
  * @param  pIE: Point to crypto adapter, Ameba-D should be NULL.
  * @retval SUCCESS
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_SHA_Init(HAL_CRYPTO_ADAPTER *pIE_I)
{
	HAL_CRYPTO_ADAPTER *pIE = &crypto_sha_engine;

	if (pIE_I != NULL) {
		pIE = pIE_I;
	}

	CRYPTO_SHA_Reset(pIE);

	pIE->isInit = 1;
	pIE->isIntMode = 0;
	return SUCCESS;
}

/**
  * @brief  Crypto set otp key for aes engine.
  * @para keynum: 0 for OTP key1, 1 for OTP key2.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_OTPKey_Init(u32 keynum, u32 status)
{
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)AES_REG_BASE_S;
	}

	if (status != ENABLE) {
		IPSEC->IPSEKCR = 0;
	} else {
		if (keynum > 3) {
			DBG_8195A("Error KeyNum!!\r\n");
			return FAIL;
		}
		/* Choose OTP key1, OTP key2, RSIP key_ecb or RSIP key_ctr. */
		IPSEC->IPSEKCR = IPSEC_BIT_EFUSE_KEY_REQ | IPSEC_BIT_EFUSE_AES_KEY_ID(keynum);
	}
	return SUCCESS;
}

/**
  * @brief  Crypto set otp key for sha engine.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_OTPKey_SHA_Init(u32 keynum, u32 status)
{
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)SHA_REG_BASE_S;
	}

	if (status != ENABLE) {
		SHA_OTPKEY = 0;
		IPSEC->IPSEKCR = 0;
	} else {
		SHA_OTPKEY = 1;
		switch (keynum) {
		case 0:
			IPSEC->IPSEKCR = IPSEC_BIT_EFUSE_KEY_REQ ;
			break;
		case 1:
			IPSEC->IPSEKCR = IPSEC_BIT_EFUSE_KEY_REQ | IPSEC_BIT_EFUSE_HMAC_KEY_ID;
			break;
		default:
			DBG_8195A("Error KeyNum!!\r\n");
			return FAIL;
			break;
		}
	}
	return SUCCESS;

}

__weak HAL_ROM_TEXT_SECTION
void CRYPTO_Cache_Sync(u8 *p_buf, u32 len)
{
	u32 start_addr = (u32)(p_buf);
	u32 end_addr = start_addr + len;
	u8 cache_buf[CACHE_LINE_SIZE] = {0};
	u8 header_offset = 0, tail_offset = 0;
	//S1: enter critical, should be initialized before use
	//if(driver_call_os_func_map.driver_enter_critical) {
	//driver_call_os_func_map.driver_enter_critical();
	//}

	header_offset = (start_addr & (~CACHE_LINE_ADDR_MSK));
	tail_offset = (end_addr & (~CACHE_LINE_ADDR_MSK));

	if (!IS_CACHE_LINE_ALIGNED_ADDR(start_addr)) {
		//S2: cache store
		_memcpy(cache_buf, (u8 *)(start_addr & CACHE_LINE_ADDR_MSK), CACHE_LINE_SIZE);
		//S3: cache flush
		DCache_Invalidate((start_addr & CACHE_LINE_ADDR_MSK), CACHE_LINE_SIZE);
		//S4: memory store
		if ((end_addr & CACHE_LINE_ADDR_MSK) == (start_addr & CACHE_LINE_ADDR_MSK)) {
			_memcpy((cache_buf + header_offset), (u8 *)(start_addr), (tail_offset - header_offset));
		} else {
			_memcpy((cache_buf + header_offset), (u8 *)(start_addr), (CACHE_LINE_SIZE - header_offset));
		}
		//S5: write back
		_memcpy((u8 *)(start_addr & CACHE_LINE_ADDR_MSK), cache_buf, CACHE_LINE_SIZE);
		//S6: cache clean
		DCache_Clean((start_addr & CACHE_LINE_ADDR_MSK), CACHE_LINE_SIZE);
	}

	if ((!IS_CACHE_LINE_ALIGNED_ADDR(end_addr)) && ((end_addr & CACHE_LINE_ADDR_MSK) != (start_addr & CACHE_LINE_ADDR_MSK))) {
		//S2: cache store
		_memcpy(cache_buf, (u8 *)(end_addr & CACHE_LINE_ADDR_MSK), CACHE_LINE_SIZE);
		//S3: cache flush
		DCache_Invalidate((end_addr & CACHE_LINE_ADDR_MSK), CACHE_LINE_SIZE);
		//S4: memory store
		_memcpy(cache_buf, (u8 *)(end_addr & CACHE_LINE_ADDR_MSK), tail_offset);
		//S5: write back
		_memcpy((u8 *)(end_addr & CACHE_LINE_ADDR_MSK), cache_buf, CACHE_LINE_SIZE);
		//S6: cache clean
		DCache_Clean((end_addr & CACHE_LINE_ADDR_MSK), CACHE_LINE_SIZE);
	}

	//S7: cache flush
	DCache_Invalidate(start_addr, len);

	//S8: exit critical
	//if(driver_call_os_func_map.driver_exit_critical) {
	//driver_call_os_func_map.driver_exit_critical();
	//}
}

/**
  * @brief  Crypto engine process.
  * @param  pIE: Point to crypto adapter.
  * @param  message: Point to message.
  * @param  msglen: Message length.
  * @param  pIv: Point to IV(initialize vector).
  * @param  ivlen: IV length.
  * @param  paad: Point to AAD(additional authentication data).
  * @param  aadlen: AAD length.
  * @param  pResult: Point to auth or cipher result.
  * @param  pTag: Point to MAC(Message Authentication Code) .
  * @retval Result of process.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_ProcessAD(HAL_CRYPTO_ADAPTER *pIE,
					 IN const u8 *message, 	IN const u32 msglen,
					 IN const u8 *pIv, 		IN const u32 ivlen,
					 IN const u8 *paad, 		IN const u32 aadlen,
					 OUT u8 *pResult, OUT u8 *pTag, hw_sha_context *ctx)
{
	rtl_crypto_srcdesc_t srcdesc;
	u32 a2eo;
	u32 enl;
	u32 digestlen;
	volatile u32 ips_err;
	u32 loopWait;
	u32 engine_type = 0;
	IPSEC_TypeDef *IPSEC = NULL;

	if (pIE->aes) {
		IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) AES_REG_BASE_S;
		}
		engine_type = 0;
	} else {
		IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

		if (TrustZone_IsSecure()) {
			IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE_S;
		}
		engine_type = 1;
	}

	assert_param(message != NULL);
	//assert_param(msglen >= 0 && msglen <= CRYPTO_MAX_MSG_LENGTH);
	//assert_param(pIE != NULL);
	assert_param(pIE->isInit == _TRUE);

	// Use only one scatter
	a2eo = aadlen;
	enl = msglen;

	CRYPTO_ClearAllINT(engine_type);

	//   Set relative length data
	/* ** Calculate message and a2eo auto padding length ** */
	pIE->a2eo = a2eo;
	pIE->enl = enl;


	pIE->enc_last_data_size = enl % 16;
	pIE->apl = (16 - pIE->enc_last_data_size) % 16;

	pIE->aad_last_data_size = a2eo % 16;
	pIE->apl_aad = (16 - pIE->aad_last_data_size) % 16;


	digestlen = pIE->digestlen;

	if (pIE->auth_type != AUTH_TYPE_NO_AUTH) {
		pIE->hmac_seq_hash_total_len += msglen;

		if ((ctx != NULL && ctx->lasthash) || ctx == NULL) {
			pIE->hmac_seq_hash_last = 1;
		}
	}

	/********************************************
	 * step 1: Setup desitination descriptor
	 * Auth descriptor (1):
	 	-----------------
		|digest			|
		-----------------
	 * Cipher descriptor:
		-----------------
		|Data			|
		-----------------
		|Hash Data(Tag)	|
		-----------------
	 ********************************************/
	if (pIE->auth_type != AUTH_TYPE_NO_AUTH) {	//auth
		rtl_crypto_dstdesc_t dst_desc;

		if (pResult != NULL) {
			if (ctx != NULL && ctx->seq_hash == 1 && ctx->lasthash != 1) {
				if (digestlen == 28) {
					digestlen = 32;
				} else if (digestlen == 48) {
					digestlen = 64;
				}
			}
			dst_desc.w = 0;
			dst_desc.auth.ws = 1;
			dst_desc.auth.fs = 1;
			dst_desc.auth.ls = 1;
			dst_desc.auth.adl = digestlen;
			DCache_CleanInvalidate(((u32)pResult), (digestlen));
			CRYPTO_SetDstDesc(dst_desc.w, (u32)pResult, engine_type);
		}
	} else { // cipher
		rtl_crypto_dstdesc_t  dst_auth_desc;
		rtl_crypto_dstdesc_t  dst_cipher_desc;

		if (pResult != NULL) {
			dst_cipher_desc.w = 0;
			dst_cipher_desc.cipher.ws = 1;
			dst_cipher_desc.cipher.fs = 1;
			dst_cipher_desc.cipher.enc = 1;
			dst_cipher_desc.cipher.enl = pIE->enl;
			if (pTag == NULL) {
				dst_cipher_desc.cipher.ls = 1;
			}
			DCache_CleanInvalidate(((u32)pResult), (pIE->enl));
			CRYPTO_SetDstDesc(dst_cipher_desc.w, (u32)pResult, engine_type);// Data
		}

		if (pTag != NULL) {	// Tag
			dst_auth_desc.w = 0;
			dst_auth_desc.auth.ws = 1;
			dst_auth_desc.auth.enc = 0;
			dst_auth_desc.auth.adl = 16;
			dst_auth_desc.auth.ls = 1;
			if (pResult == NULL) {
				dst_auth_desc.auth.fs = 1;
			}
			DCache_CleanInvalidate(((u32)pTag), (16));
			CRYPTO_SetDstDesc(dst_auth_desc.w, (u32)pTag, engine_type);
		}
	}

	/********************************************
	 * step 2: Setup source descriptor
			 ----------
			|Cmd buffer |
			----------
			|Key array   |
			----------
			|IV array     |
			----------
			|PAD array  |
			----------
			|Data 1       |
			|   .             |
			|   .             |
			|   .             |
			|Data N       |
			----------
			|HMAC APL  |
			----------
	 ********************************************/
	/********************************************
	  * step 2-1: prepare Cmd & Key & IV array & Pad array:
	  ********************************************/

	CRYPTO_SrcDescKeyIv(pIE, pIv, ivlen, ctx, engine_type);

	/********************************************
	 * step 2-2: prepare Data1 ~ DataN
	 ********************************************/
	srcdesc.w = 0;
	srcdesc.d.rs = 1;

	if (paad != NULL) {
		DCache_CleanInvalidate((u32)paad, aadlen);
		while (a2eo > 16) {	//AAD, 16-byte per src desc
			srcdesc.d.a2eo = 16;

			crypto_dbg_mem_dump(paad, 16, "AAD: ");
			CRYPTO_SetSrcDesc(srcdesc.w, (u32)paad, engine_type);

			paad += 16;
			a2eo -= 16;
			srcdesc.w = 0;
			srcdesc.d.rs = 1;
		}

		if (a2eo > 0) {	//AAD, last block
			srcdesc.d.a2eo = a2eo;
			crypto_dbg_mem_dump(paad, a2eo, "AAD: ");
			CRYPTO_SetSrcDesc(srcdesc.w, (u32)paad, engine_type);
			srcdesc.w = 0;
			srcdesc.d.rs = 1;
		}

		if (pIE->apl_aad > 0) {	//AAD padding, last block
			srcdesc.d.a2eo = pIE->apl_aad;
			DCache_CleanInvalidate((u32)ipsec_padding, sizeof(ipsec_padding));
			crypto_dbg_mem_dump(ipsec_padding, pIE->apl_aad, "AAD padding ");
			CRYPTO_SetSrcDesc(srcdesc.w, (u32)ipsec_padding, engine_type);
			srcdesc.w = 0;
			srcdesc.d.rs = 1;
		}
	}

	if (message != NULL) {
		DCache_CleanInvalidate(((u32)message), enl);
	}

	while (enl > 16368) {	// Data body 16352-byte per src desc
		srcdesc.d.enl = 16368;
		CRYPTO_SetSrcDesc(srcdesc.w, (u32)message, engine_type);

		message += 16368;
		enl -= 16368;
		srcdesc.w = 0;
		srcdesc.d.rs = 1;
	}

	// Data body msglen < 16352
	if (pIE->apl == 0) {
		srcdesc.d.ls = 1;
	}
	srcdesc.d.enl = enl;
	CRYPTO_SetSrcDesc(srcdesc.w, (u32)message, engine_type);
	crypto_dbg_mem_dump(message, enl, "message: ");

	// data padding, regard as enl
	if (pIE->apl != 0) {
		srcdesc.w = 0;
		srcdesc.d.rs = 1;
		srcdesc.d.enl = pIE->apl;
		srcdesc.d.ls = 1;
		DCache_CleanInvalidate((u32)ipsec_padding, sizeof(ipsec_padding));
		crypto_dbg_mem_dump(ipsec_padding, srcdesc.d.enl, "data padding ");
		CRYPTO_SetSrcDesc(srcdesc.w, (u32)ipsec_padding, engine_type);
	}

	/********************************************
	 * step 3: Wait until ipsec engine stop
	 *Polling mode, intr_mode = 0
	 ********************************************/
	loopWait = 1000000; /* hope long enough */
	while (1) {
		if (IPSEC->IPSCSR_RSTEACONFISR & IPSEC_BIT_CMD_OK) {
			break;
		}

		ips_err = IPSEC->IPSCSR_ERR_INT;
		if (ips_err) {
			DBG_PRINTF(MODULE_IPSEC, LEVEL_ERROR, "ips 0x1C err = 0x%08x\r\n", ips_err);
			return FAIL;
		}

		loopWait--;
		if (loopWait == 0) {
			DBG_PRINTF(MODULE_IPSEC, LEVEL_ERROR, "Wait Timeout\r\n");
			return FAIL; /* error occurs */
		}
	}

	if (pResult != NULL) {
		if (pIE->auth_type != AUTH_TYPE_NO_AUTH) {	//auth
			CRYPTO_Cache_Sync(pResult, digestlen);
		} else {
			CRYPTO_Cache_Sync(pResult, pIE->enl);
		}
	}

	if (pTag != NULL) {
		CRYPTO_Cache_Sync(pTag, 16);
	}

	//CRYPTO_CleanCmdOk();
	return SUCCESS;
}

/**
  * @brief  Cipher initialize.
  * @param  pIE: Point to crypto adapter.
  * @param  cipher_type: Cipher type.
  *		 @arg CIPHER_TYPE_AES_ECB: AES_ECB
  *		 @arg CIPHER_TYPE_AES_CBC: AES_CBC
  *		 @arg CIPHER_TYPE_AES_CFB: AES_CFB
  *		 @arg CIPHER_TYPE_AES_OFB: AES_OFB
  *		 @arg CIPHER_TYPE_AES_CTR: AES_CTR
  * @param  key: Cipher key.
  * @param  keylen: Key length.
  * @retval Function execute status.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_CipherInit(HAL_CRYPTO_ADAPTER *pIE,
					  IN const u32 cipher_type,
					  IN const u8 *key, IN const u32 keylen)
{
	u8 *pAuthKey = NULL;
	u32 lenAuthKey = 0;

	return CRYPTO_SetSecurityModeAD(pIE,
									cipher_type, AUTH_TYPE_NO_AUTH,
									key, keylen, pAuthKey, lenAuthKey);

}

/**
  * @brief  Cipher encryption.
  * @param  pIE: Point to crypto adapter.
  * @param  message: Point to message.
  * @param  msglen: Message length.
  * @param  pIv: Point to IV(initialize vector).
  * @param  ivlen: IV length.
  * @param  paad: Point to AAD(additional authentication data).
  * @param  aadlen: AAD length.
  * @param  pResult: Point to auth or cipher result.
  * @param  pTag: Point to MAC(Message Authentication Code) .
  * @retval Result of process.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_CipherEncryptAD(HAL_CRYPTO_ADAPTER *pIE,
						   IN const u8 *message, 	IN const u32 msglen,
						   IN const u8 *piv, 		IN const u32 ivlen,
						   IN const u8 *paad, 		IN const u32 aadlen,
						   OUT u8 *pResult, OUT u8 *pTag)
{
	int ret;

	pIE->cipher_type |= CIPHER_TYPE_MODE_ENCRYPT;
	pIE->isDecrypt = _FALSE;

	ret = CRYPTO_ProcessAD(pIE, message, msglen,
						   piv, ivlen, paad, aadlen, pResult, pTag, NULL);

	return ret;
}

/**
  * @brief  Cipher decryption.
  * @param  pIE: Point to crypto adapter.
  * @param  message: Point to message.
  * @param  msglen: Message length.
  * @param  pIv: Point to IV(initialize vector).
  * @param  ivlen: IV length.
  * @param  paad: Point to AAD(additional authentication data).
  * @param  aadlen: AAD length.
  * @param  pResult: Point to auth or cipher result.
  * @param  pTag: Point to MAC(Message Authentication Code) .
  * @retval Result of process.
  */
__weak HAL_ROM_TEXT_SECTION
int CRYPTO_CipherDecryptAD(
	HAL_CRYPTO_ADAPTER *pIE,
	IN const u8 *message, 	IN const u32 msglen,
	IN const u8 *piv, 			IN const u32 ivlen,
	IN const u8 *paad,			IN const u32 aadlen,
	OUT u8 *pResult, OUT u8 *pTag)
{
	int ret;

	pIE->cipher_type ^= CIPHER_TYPE_MODE_ENCRYPT;
	pIE->isDecrypt = _TRUE;

	ret = CRYPTO_ProcessAD(pIE, message, msglen, piv, ivlen, paad, aadlen, pResult, pTag, NULL);

	return ret;
}

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor *****END OF FILE****/
