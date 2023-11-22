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

#include "ameba_soc.h"

#ifndef MIN
#define MIN(x, y)			(((x) < (y)) ? (x) : (y))
#endif

static LOG_UART_PORT LOG_UART_IDX_FLAG[] = {
	{1, LOGUART_BIT_TP2F_NOT_FULL, LOGUART_BIT_TP2F_EMPTY, 52125, UART_LOG_IRQ},	/* CM0 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{0, LOGUART_BIT_TP1F_NOT_FULL, LOGUART_BIT_TP1F_EMPTY, 781875, UART_LOG_IRQ},		/* CM4 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{3, LOGUART_BIT_TP4F_NOT_FULL, LOGUART_BIT_TP4F_EMPTY, 3127500, UART_LOG_IRQ},	/* CA7 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
};

/**
  *  @brief Pre-Process for Audio pad switch to digital usage
  *  @param PinName: Pin of Audio share PAD.
  *  @retval None
  *  @note Special process for diffrent audio pad type:
			MIC_PAD:		mute MICBST
			MIC_BIAS_PAD:	power down MICBIAS
			AOUT_PAD:		power down HPO
  */
static void APAD_PreProcess(u8 PinName)
{
	if ((PinName < APAD_NAME_START) || (PinName > APAD_NAME_END)) {
		return;
	}

	if ((PinName >= APAD_MIC_PAD__START) && (PinName <= APAD_MIC_PAD_END)) {
		/* keep audio MICBST be mute */
		switch (PinName) {
		case _PA_20:
		case _PA_21:
			/* mute AMIC1 */
			AUDIO_CODEC_SetMicBstChnMute(AMIC1, MICIN, MUTE);
			break;
		case _PA_22:
		case _PA_23:
			/* mute AMIC2 */
			AUDIO_CODEC_SetMicBstChnMute(AMIC2, MICIN, MUTE);
			break;
		case _PA_24:
		case _PA_25:
			/* mute AMIC3 */
			AUDIO_CODEC_SetMicBstChnMute(AMIC3, MICIN, MUTE);
			break;
		case _PA_26:
		case _PA_27:
			/* mute AMIC4 */
			AUDIO_CODEC_SetMicBstChnMute(AMIC4, MICIN, MUTE);
			break;
		case _PA_28:
		case _PA_29:
			/* mute AMIC5 */
			AUDIO_CODEC_SetMicBstChnMute(AMIC5, MICIN, MUTE);
			break;
		default:
			break;
		}

	} else if ((PinName >= APAD_MIC_BIAS_PAD_START) && (PinName <= APAD_MIC_BIAS_PAD_END)) {
		/* keep audio MICBIAS power be power down mode */
		AUDIO_CODEC_SetMicBiasPowerMode(POWER_DOWN);

		switch (PinName) {
		case _PA_30:
			AUDIO_CODEC_SetMicBiasPCUTMode(AMIC1, POWER_DOWN);
			break;
		case _PA_31:
			AUDIO_CODEC_SetMicBiasPCUTMode(AMIC2, POWER_DOWN);
			break;
		case _PB_0:
			AUDIO_CODEC_SetMicBiasPCUTMode(AMIC3, POWER_DOWN);
			break;
		case _PB_1:
			AUDIO_CODEC_SetMicBiasPCUTMode(AMIC4, POWER_DOWN);
			break;
		case _PB_2:
			AUDIO_CODEC_SetMicBiasPCUTMode(AMIC5, POWER_DOWN);
			break;
		default:
			break;
		}

	} else if ((PinName >= APAD_OUT_PAD_START) && (PinName <= APAD_OUT_PAD_END)) {
		/* keep audio HPO power be power down mode */
		switch (PinName) {
		case _PB_3:
		case _PB_4:
			/* AOUT LPLN */
			AUDIO_CODEC_SetHPOPowerMode(CHN_L, POWER_DOWN);
			break;
		case _PB_5:
		case _PB_6:
			/* AOUT RPRN */
			AUDIO_CODEC_SetHPOPowerMode(CHN_R, POWER_DOWN);
			break;
		default:
			break;
		}
	}

}

void Pinmux_Config(u8 PinName, u32 PinFunc)
{
	if ((PinName >= APAD_NAME_START) && (PinName <= APAD_NAME_END)) {
		RCC_PeriphClockCmd(APBPeriph_AUDIO, NULL, ENABLE);

		if (PinFunc != PINMUX_FUNCTION_AUDIO) {
			/* for audio pad switch to digital usage: */

			//step1: pre-process for diffrent audio pad types
			APAD_PreProcess(PinName);

			//step2: enable digital path input
			APAD_InputCtrl(PinName, ENABLE);

			//RTK_LOGW(TAG, "APAD P%s_%d is configured to funcID %lu \n", PORT_NUM(PinName) ? "B" : "A", PIN_NUM(PinName), PinFunc);

		} else {
			APAD_InputCtrl(PinName, DISABLE);
		}

	} else if ((PinName == SWD_DATA) || ((PinName == SWD_CLK))) {
		Pinmux_Swdoff();
		//RTK_LOGW(TAG, "SWD PAD P%s_%d is configured to funcID %lu \n", PORT_NUM(PinName) ? "B" : "A", PIN_NUM(PinName), PinFunc);
	}

	_Pinmux_Config(PinName, PinFunc);

}


int CRYPTO_SetSecurityModeAD(HAL_CRYPTO_ADAPTER *pIE,
							 IN const u32 cipher_type, IN const u32 auth_type,
							 IN const void *pCipherKey, IN const u32 lenCipherKey,
							 IN const void *pAuthKey, IN const u32 lenAuthKey)
{
	assert_param(pIE != NULL);
	assert_param(pIE->isInit == _TRUE);
	u32 i;

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
	if ((sha2type == SHA2_224) || (sha2type == SHA2_256)) {

		assert_param(keylen <= CRYPTO_AUTH_PADDING);
	} else if ((sha2type == SHA2_384) || (sha2type == SHA2_512)) {

		assert_param(keylen <= CRYPTO_AUTH_PADDING_512);
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
  * @brief  LOGUART send char
  * @param  c: send char value
  * @note: remove ConfigDebugClose and KB_ASCII_CR.
  */
void LOGUART_PutChar_RAM(u8 c)
{
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 CounterIndex = 0;
	u32 CPUID = SYS_CPUID();

	while (1) {
		CounterIndex++;
		if (CounterIndex >= LOG_UART_IDX_FLAG[CPUID].tx_timeout) {
			break;
		}

		if (UARTLOG->LOGUART_UART_LSR & LOG_UART_IDX_FLAG[CPUID].not_full) {
			break;
		}
	}
	UARTLOG->LOGUART_UART_THRx[LOG_UART_IDX_FLAG[CPUID].idx] = c;
}

/**
  * @brief Initialize specified GPIO pin output in Open Drain mode.
  * @param PinName: Value of @ref PINMUX_Pin_Name_definitions.
  * @param PUInternal: which can ENABLE or DISABLE.
  * @retval None.
  */
void GPIO_ODInit(u8 PinName, u32 PUInternal)
{
	GPIO_Direction(PinName, GPIO_Mode_IN);

	if (ENABLE == PUInternal) {
		PAD_PullCtrl(PinName, GPIO_PuPd_UP);
	} else {
		PAD_PullCtrl(PinName, GPIO_PuPd_NOPULL);
	}

	GPIO_WriteBit(PinName, 0);
}

/**
  * @brief Write a specified output port pin in Open Drain mode.
  * @param PinName: Value of @ref PINMUX_Pin_Name_definitions.
  * @param PinState: This parameter can be one of the following values:
  * 		@arg GPIO_PIN_LOW: Pin state set to low
  * 		@arg GPIO_PIN_HIGH: Pin state set to high
  * @retval None
  */
void GPIO_ODWriteBit(u8 PinName, u32 PinState)
{
	if (PinState == GPIO_PIN_LOW) {
		GPIO_Direction(PinName, GPIO_Mode_OUT);
	} else {
		GPIO_Direction(PinName, GPIO_Mode_IN);
	}
}

/**
  * @brief  Get CPU clock.
  * @retval  CPU Clk
  */
u32 CPU_ClkGet(void)
{
	u32  CpuClk = 0;//, FreqDown = 0;
	u32  Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	u32 CPUID = SYS_CPUID();
	u32 ChipType = SYSCFG_CHIPType_Get();
	u32 Div;
	PLL_TypeDef *pll = (PLL_TypeDef *)PLL_BASE;

	if (CPUID == LP_CPU_ID) {
		Temp = LSYS_GET_CKSL_LSOC(Temp);

		if (ChipType == CHIP_TYPE_FPGA) {
			CpuClk = 20000000;
		} else {
			if (Temp & 0x2) {
				CpuClk = XTAL_ClkGet();
			} else {
				CpuClk = 4000000;
			}
		}
	} else if (CPUID == NP_CPU_ID) {
		if (ChipType == CHIP_TYPE_FPGA) {
			Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FPGA_CLK_INFO);
			CpuClk = LSYS_GET_FPGA_NP_CLK(Temp);
			if (CpuClk == 0) {
				CpuClk = 50000000;
			} else {
				CpuClk = CpuClk * 1000000;
			}
		} else {
			Temp = LSYS_GET_CKSL_NP(Temp);

			if (Temp != CLK_KM4_XTAL) {
				Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0);
				Temp = LSYS_GET_CKD_NP(Temp);

				Div = PLL_GET_NPLL_DIVN_SDM(pll->PLL_NPPLL_CTRL1) + 2;

				CpuClk = 40000000 * Div / (Temp + 1);

			} else {
				CpuClk = 40000000;
			}

		}
	}
	return CpuClk;
}

/**
  * @brief    Configure sport Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_I2S_PLL98M
  *                            @arg CKSL_I2S_PLL45M
  *                            @arg CKSL_I2S_PLL24M
  *                            @arg CKSL_I2S_XTAL40M
  * @retval   None
  * @note    Used to switch SPORT clock
  */
void RCC_PeriphClockSource_SPORT(int Sportx, u32 Source)
{
	u32 RegValue = 0;
	u32 RegValue2 = 0;
	u32 SPORTClk = 0;

	RegValue = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	RegValue2 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SPORT_CLK);
	SPORTClk = LSYS_GET_CKSL_SPORT(RegValue);

	if (Source == CKSL_I2S_XTAL40M) {
		SPORTClk &= ~(BIT(Sportx));
	} else {
		SPORTClk |= (BIT(Sportx));
		switch (Sportx) {
		case 0:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S0);
			RegValue2 |= LSYS_CKSL_I2S0(Source);
			break;
		case 1:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S1);
			RegValue2 |= LSYS_CKSL_I2S1(Source);
			break;
		case 2:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S2);
			RegValue2 |= LSYS_CKSL_I2S2(Source);
			break;
		case 3:
			RegValue2 &= ~(LSYS_MASK_CKSL_I2S3);
			RegValue2 |= LSYS_CKSL_I2S3(Source);
			break;
		default:
			break;
		}
	}

	RegValue &= ~LSYS_MASK_CKSL_SPORT;
	RegValue |= LSYS_CKSL_SPORT(SPORTClk);

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, RegValue);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SPORT_CLK, RegValue2);

}

/**
  * @brief  set iv
  * @param  index: OTF section num, should be 0-7
  * @param  IV: 8 bytes IV for Flash decrypt
  */
void RSIP_IV_Set(u8 index, u8 *IV)
{
	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	/*set iv high 96 bits*/
	RSIP->OTF_IV[index].RSIP_IVxH = ((IV[0] << 24) | (IV[1] << 16) | (IV[2] << 8) | (IV[3]));
	RSIP->OTF_IV[index].RSIP_IVxL = ((IV[4] << 24) | (IV[5] << 16) | (IV[6] << 8) | (IV[7]));
}

/**
  * @brief  ENABLE flash run time decrypt
  * @param  NewStatus: ENABLE/DISABLE
  * @note called in ROM code when boot
  * @note this function can not be called by user.
  */
void RSIP_OTF_Cmd(u32 NewStatus)
{
	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	if (NewStatus == ENABLE) {
		RSIP->RSIP_CR |= RSIP_BIT_AES_EN;
	} else {
		RSIP->RSIP_CR &= (~RSIP_BIT_AES_EN);
	}
}

/**
  * @brief  enable flash run time decrypt in some special FLASH area.
  * @param  EnIdx: 0-7
  * @param BaseAddr: FLASH area base address (should 32B alignment)
  * @param EndAddr: FLASH area end address (should 32B alignment)
  * @param NewStatus This parameter can be one of the following values
  *		 @arg DISABLE close this area run time decypt
  *		 @arg ENABLE enable this area run time decypt (this area will be decrypt when read)
  * @param IVNum: choose the iv for this region , can be 0-7
  * @param Mode: 1 ctr mode, 2 xts mode
  */
void RSIP_OTF_Enable(u32 EnIdx, u32 BaseAddr, u32 EndAddr, u32 NewStatus, u32 IVNum, u32 Mode)
{
	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	if (NewStatus != DISABLE) {
		assert_param(IS_OTF_IVNUM(IVNum));
		assert_param(IS_OTF_MODE(Mode));
		/* disable region first */
		RSIP->OTF_REGION[EnIdx].RSIP_RxEAR &= ~RSIP_BIT_REGION_x_ENABLE;

		RSIP->OTF_REGION[EnIdx].RSIP_RxBAR = BaseAddr | RSIP_REGION_x_IV_NUM(IVNum);
		RSIP->OTF_REGION[EnIdx].RSIP_RxEAR = EndAddr | RSIP_REGION_x_MODE(Mode) | RSIP_BIT_REGION_x_ENABLE ;
	} else {
		RSIP->OTF_REGION[EnIdx].RSIP_RxEAR &= ~RSIP_BIT_REGION_x_ENABLE;
	}
}

/**
  * @brief  Config flash mmu in some special FLASH area.
  * @param  MMUIdx: 0~3
  * @param AddrStart: FLASH area logical address start (should 32B alignment)
  * @param AddrEnd: FLASH area logical address end (should 32B alignment)
  * @param AddrOffset: FLASH area physical address start (should 32B alignment)
  * @note output addr = Access address - AddrStart + AddrOffset.
  */
void RSIP_MMU_Config(u32 MMUIdx, u32 AddrStart, u32 AddrEnd, u32 AddrOffset)
{

	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	/* disable the region first */
	RSIP->FLASH_MMU[MMUIdx].RSIP_REMAPxOR &= ~RSIP_BIT_REMAP_x_ENABLE;

	RSIP->FLASH_MMU[MMUIdx].RSIP_REMAPxBAR = ((AddrStart + 0x1f) >> 5) << 9;
	RSIP->FLASH_MMU[MMUIdx].RSIP_REMAPxEAR = ((AddrEnd + 0x1f) >> 5) << 9;
	RSIP->FLASH_MMU[MMUIdx].RSIP_REMAPxOR = ((AddrOffset + 0x1f) >> 5) << 9;
}

/**
  * @brief  ENABLE flash MMU Entry
  * @param  MMUIdx: 0~3
  * @param  NewStatus: ENABLE/DISABLE
  */
void RSIP_MMU_Cmd(u32 MMUIdx, u32 NewStatus)
{
	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	if (NewStatus != DISABLE) {
		RSIP->FLASH_MMU[MMUIdx].RSIP_REMAPxOR |= RSIP_BIT_REMAP_x_ENABLE;
	} else {
		RSIP->FLASH_MMU[MMUIdx].RSIP_REMAPxOR &= ~RSIP_BIT_REMAP_x_ENABLE;
	}
}

/**
  * @brief Clean MMU Cache
  */
void RSIP_MMU_Cache_Clean(void)
{
	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);

	RSIP->RSIP_MOR |= RSIP_BIT_CLEAN_CACHE_LINE;

}

void NAND_UserModeEn(u8 enable)
{
	SPIC_TypeDef *spi_flash = SPIC;

	/* Wait spic busy done before switch mode */
	NAND_WaitBusy(WAIT_SPIC_BUSY);

	if (enable) {
		spi_flash->CTRLR0 |= BIT_USER_MODE;
	} else {
		spi_flash->CTRLR0 &= ~BIT_USER_MODE;
	}
}

void NAND_RxData(u8 cmd, u32 StartAddr, u32 read_len, u8 *read_data)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u8 rd_cmd = cmd;
	u32 rx_num = 0;
	u32 temp, idx;
	u32 *aligned_buf;

	if (rd_cmd == 0) {
		rd_cmd = flash_init_para.FLASH_cur_cmd;
	}

	/* Enter User Mode */
	NAND_UserModeEn(ENABLE);

	/* set CTRLR0: RX_mode */
	spi_flash->CTRLR0 |= TMOD(3);

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	spi_flash->RX_NDF = RX_NDF(read_len);
	spi_flash->TX_NDF = 0;

	/* Set ADDR length, and dummy byte is set by USER_LENGTH.USER_RD_DUMMY_LENGTH*/
	temp = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = temp | USER_ADDR_LENGTH(flash_init_para.FLASH_addr_phase_len);

	/* Set target slave */
	spi_flash->SER = 1;

	/* set flash_cmd: write cmd & address to fifo & addr is MSB */
	spi_flash->DR[0].BYTE = rd_cmd;

	/*read from cache of GigaDevice Nand flash need dummy data after cmd phase.*/
	if (flash_init_para.FLASH_addr_phase_len == NAND_COL_ADDR_3_BYTE) {
		spi_flash->DR[0].BYTE = 0x0; 	/*put dummy data after cmd for GigaDevice*/
	}
	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF00) >> 8);
	spi_flash->DR[0].BYTE = (u8)(StartAddr & 0xFF);

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;
	rx_num = 0;

	temp = UNALIGNED32(read_data);
	if (temp) { /* if buf is not aligned */
		while (rx_num < read_len) {
			if (spi_flash->SR & BIT_RFNE) {
				read_data[rx_num] = spi_flash->DR[0].BYTE;
				rx_num += 1;
			}
			if (rx_num == U32BLOCKSIZE - temp) {
				break;
			}
		}
	}

	aligned_buf = (u32 *)&read_data[rx_num];
	while (rx_num < read_len) {
		temp = GET_RXFLR(spi_flash->RXFLR) / U32BLOCKSIZE;
		for (idx = 0; idx < temp; idx++) {
			aligned_buf[idx] = spi_flash->DR[0].WORD;
		}
		aligned_buf += idx;
		rx_num += idx * U32BLOCKSIZE;

		if (read_len - rx_num < U32BLOCKSIZE) {
			break;
		}
	}

	while (rx_num < read_len) {
		if (spi_flash->SR & BIT_RFNE) {
			read_data[rx_num] = spi_flash->DR[0].BYTE;
			rx_num += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	NAND_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Exit user mode */
	NAND_UserModeEn(DISABLE);
}

u8 NAND_Page_Read(u32 PageAddr, u32 ByteAddr, u32 ByteLen, u8 *pData)
{
#ifdef ARM_CORE_CA32
	if (SYSCFG_CUT_VERSION_A == SYSCFG_RLVersion()) {
		arm_gic_freq_switch();
	}
#endif

	u8 status = NAND_Page_Read_ArrayToCache(PageAddr);

	NAND_RxData(flash_init_para.FLASH_cur_cmd, ByteAddr, ByteLen, pData);

#ifdef ARM_CORE_CA32
	if (SYSCFG_CUT_VERSION_A == SYSCFG_RLVersion()) {
		arm_gic_freq_restore();
	}
#endif

	/*if status is 0 means NAND_STATUS_ECC_NO_BITFLIPS, ortherwise need confirm spi nand datasheet.*/
	return status;
}

void NAND_TxData(u8 cmd, u32 StartAddr, u32 ByteLen, u8 *pData)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 tx_num = 0;
	u32 addr_length = spi_flash->USER_LENGTH;
	u32 ctrl0 = spi_flash->CTRLR0;
	u32 temp, idx;
	u32 *aligned_buf;

	/* Enter user mode */
	NAND_UserModeEn(ENABLE);

	/* set CTRLR0: TX mode and one bit mode */
	spi_flash->CTRLR0 &= ~(TMOD(3) | CMD_CH(3) | ADDR_CH(3) | DATA_CH(3));

	/* if cur bitmode is 4O/4IO, and cmd is X4 Data Xfer, use 1-1-4 mode, otherwise use 1-1-1 mode to xfer data */
	if ((cmd == NAND_CMD_QPP) || (cmd == NAND_CMD_QPP_RANDOM)) {
		spi_flash->CTRLR0 |= DATA_CH(2);
	}

	/* Set ADDR length */
	temp = spi_flash->USER_LENGTH & ~MASK_USER_ADDR_LENGTH;
	spi_flash->USER_LENGTH = temp | USER_ADDR_LENGTH(2);

	/* Set TX_NDF: frame number of Tx data. */
	spi_flash->RX_NDF = 0;
	spi_flash->TX_NDF = TX_NDF(ByteLen);

	/* set flash_cmd: write cmd & address to fifo & addr is MSB */
	spi_flash->DR[0].BYTE = cmd;
	spi_flash->DR[0].BYTE = (u8)((StartAddr & 0xFF00) >> 8);
	spi_flash->DR[0].BYTE = (u8)(StartAddr & 0xFF);


	/* we can not set ssienr=1 when FIFO empty */
	/* Pre-load data before enabling, but there are just 16 - 4 = 12 bytes fifo afer cmd+addr */
	tx_num = 0;

	temp = UNALIGNED32(pData);
	if (temp) { /* if buf is not aligned */
		while (tx_num < ByteLen) {
			spi_flash->DR[0].BYTE = pData[tx_num];
			tx_num += 1;
			if (tx_num == U32BLOCKSIZE - temp) {
				break;
			}
		}
	}

	/* Enable SSIENR to start the transfer */
	spi_flash->SSIENR = BIT_SPIC_EN;

	/* Put the remain data into transmit FIFO */
	aligned_buf = (u32 *)&pData[tx_num];
	while (tx_num < ByteLen) {
		temp = SPIC_DR_DEPTH - GET_TXFLR(spi_flash->TXFLR);
		temp = MIN(ByteLen - tx_num, temp) / U32BLOCKSIZE;
		for (idx = 0; idx < temp; idx++) {
			spi_flash->DR[0].WORD = aligned_buf[idx];
		}
		aligned_buf += idx;
		tx_num += idx * U32BLOCKSIZE;

		if (ByteLen - tx_num < U32BLOCKSIZE) {
			break;
		}
	}

	while (tx_num < ByteLen) {
		if (spi_flash->SR & BIT_TFNF) {
			spi_flash->DR[0].BYTE = pData[tx_num];
			tx_num += 1;
		}
	}

	/* Wait transfer complete. When complete, SSIENR.SPIC_EN are cleared by HW automatically. */
	NAND_WaitBusy(WAIT_TRANS_COMPLETE);

	/* Restore bitmode & addr_len */
	spi_flash->USER_LENGTH = addr_length;
	spi_flash->CTRLR0 = ctrl0;
}

u8 NAND_Page_Write(u32 PageAddr, u32 ByteAddr, u32 ByteLen, u8 *pData)
{
	u8 status;

#ifdef ARM_CORE_CA32
	if (SYSCFG_CUT_VERSION_A == SYSCFG_RLVersion()) {
		arm_gic_freq_switch();
	}
#endif

	NAND_WriteEn();
	/* Use program laod X4 (reset buffer) commond if cur bitmode is 4IO/4O */
	if (flash_init_para.FLASH_cur_bitmode >= SpicQuadOBitMode) {
		NAND_TxData(NAND_CMD_QPP, ByteAddr, ByteLen, pData);
	} else {
		NAND_TxData(NAND_CMD_PP, ByteAddr, ByteLen, pData);
	}

	status = NAND_Page_Write_Program_Execute(PageAddr);

#ifdef ARM_CORE_CA32
	if (SYSCFG_CUT_VERSION_A == SYSCFG_RLVersion()) {
		arm_gic_freq_restore();
	}
#endif

	return status;
}


#ifndef ARM_CORE_CA32
void irq_set_priority(IRQn_Type irqn, uint32_t priority)
{
	if (priority > MAX_IRQ_PRIORITY_VALUE) {
		DBG_8195A("irq[%d] priority %d shall <= %d\n", irqn, priority, MAX_IRQ_PRIORITY_VALUE);
		assert_param(0);
	}
	__NVIC_SetPriority(irqn, priority);
}

BOOL irq_register(IRQ_FUN IrqFun, IRQn_Type IrqNum, u32 Data,  u32 Priority)
{
	UserIrqFunTable[IrqNum] = (IRQ_FUN)((u32)IrqFun | 0x1);
	UserIrqDataTable[IrqNum] = (u32)(Data);

	irq_set_priority(IrqNum, Priority);

	return _TRUE;
}
#endif

/**
  * @brief Check Boot from nor or nand
  * TRUE: Boot from Nor
  * FALSE: Boot from Nand
  */
u32 SYSCFG_BootFromNor(void)
{
	u32 IsTestMode = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0) & LSYS_BIT_PTRP_TMODE;
	u32 TestMode = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG1) & LSYS_MASK_PTRP_ICFG;

	/* Boot Select decided by Trap pin if in Test mode14 */
	if (IsTestMode && (TestMode  == BIT_FORCE_USE_S0_ICFG)) {
		goto BYTRAP;
	}

	if (SYSCFG_OTP_BootSelect() == SYSCFG_OTP_BOOTNOR) {
		return TRUE;
	}

	if (SYSCFG_OTP_BootSelect() == SYSCFG_OTP_BOOTNAND) {
		return FALSE;
	}

BYTRAP:
	/*Boot Select decided by Trap pin*/
	if (SYSCFG_TRP_BootSel()) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
  * @brief Get RL version.
  * @return RL version.
  */
u32 SYSCFG_RLVersion(void)
{
	u32 value;
	u32 value32;

	/* Set LSYS_CHIP_INFO_EN register to get ChipInfo */
	value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL);
	value &= ~(LSYS_MASK_CHIP_INFO_EN);
	value |= LSYS_CHIP_INFO_EN(0xA);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL, value);

	value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL);

	/* Clear LSYS_CHIP_INFO_EN register */
	value &= ~(LSYS_MASK_CHIP_INFO_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL, value);

	return LSYS_GET_RL_VER(value32);
}

/**
  * @brief Get RL number.
  * @return RL number.
  */
u32 SYSCFG_GetRLNum(void)
{
	u32 value;
	u32 value32;

	/* Set LSYS_CHIP_INFO_EN register to get ChipInfo */
	value = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL);
	value &= ~(LSYS_MASK_CHIP_INFO_EN);
	value |= LSYS_CHIP_INFO_EN(0xA);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL, value);

	value32 = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL);

	/* Clear LSYS_CHIP_INFO_EN register */
	value &= ~(LSYS_MASK_CHIP_INFO_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SCAN_CTRL, value);

	return LSYS_GET_RL_NO(value32);
}


#if defined (ARM_CORE_CM0)

VOID INT_Entry_Patch(u32 IntIndex)
{
	extern IRQ_FUN UserIrqFunTable[MAX_PERIPHERAL_IRQ_NUM];
	//Clear ARM Cortex-M3 IRQ
	NVIC_ClearPendingIRQ(IntIndex);

	if (UserIrqFunTable[IntIndex] != NULL) {
		UserIrqFunTable[IntIndex]((VOID *)(UserIrqDataTable[IntIndex]));
	} else {
		DBG_8195A("INT_Entry Irq %d Fun Not Assign!!!!!", IntIndex);
	}
}


VOID
INT_I2C1(VOID)
{
	INT_Entry_Patch(I2C1_IRQ);
}

VOID
INT_I2C2(VOID)
{
	INT_Entry_Patch(I2C2_IRQ);
}

VOID VectorTableAdd(VOID)
{
	extern HAL_VECTOR_FUN NewVectorTable[MAX_VECTOR_TABLE_NUM];

	NewVectorTable[16 + I2C1_IRQ] = (HAL_VECTOR_FUN) INT_I2C1;
	NewVectorTable[16 + I2C2_IRQ] = (HAL_VECTOR_FUN) INT_I2C2;
}
#endif

#ifdef ARM_CORE_CM4
u32 Boot_Cut_Ver;
// for security
int SBOOT_GetMdType(u8 HashId, SHA2_TYPE *MdType, u8 *IsHmac)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return SBOOT_GetMdType_A(HashId, MdType, IsHmac);
	} else {
		return SBOOT_GetMdType_B(HashId, MdType, IsHmac);

	}
}

int CRYPTO_OTPKey_SHA_Init(u32 keynum, u32 status)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		/* SHA engine auto-load HMAC key */
		return CRYPTO_OTPKey_SHA_Init_A(keynum, status);
	} else {
		return CRYPTO_OTPKey_SHA_Init_B(keynum, status);
	}
}

int rtl_crypto_hmac_sha2_update(IN u8 *message, IN u32 msglen, hw_sha_context *ctx)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return rtl_crypto_hmac_sha2_update_A(message, msglen, ctx);
	} else {
		return rtl_crypto_hmac_sha2_update_B(message, msglen, ctx);
	}

}

int rtl_crypto_hmac_sha2_final(OUT u8 *pDigest, hw_sha_context *ctx)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return rtl_crypto_hmac_sha2_final_A(pDigest, ctx);
	} else {
		return rtl_crypto_hmac_sha2_final_B(pDigest, ctx);

	}
}

int rtl_crypto_sha2_init(IN const SHA2_TYPE sha2type, hw_sha_context *ctx)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return rtl_crypto_sha2_init_A(sha2type, ctx);
	} else {
		return rtl_crypto_sha2_init_B(sha2type, ctx);

	}

}

int rtl_crypto_sha2_update(IN u8 *message, IN u32 msglen, hw_sha_context *ctx)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return rtl_crypto_sha2_update_A(message, msglen, ctx);
	} else {
		return rtl_crypto_sha2_update_B(message, msglen, ctx);
	}
}

int rtl_crypto_sha2_final(OUT u8 *pDigest, hw_sha_context *ctx)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return rtl_crypto_sha2_final_A(pDigest, ctx);
	} else {
		return rtl_crypto_sha2_final_B(pDigest, ctx);

	}

}

int SBOOT_Validate_PubKey(u8 AuthAlg, u8 *Pk, u8 *Hash)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {

		return SBOOT_Validate_PubKey_A(AuthAlg, Pk, Hash);
	} else {
		return SBOOT_Validate_PubKey_B(AuthAlg, Pk, Hash);

	}

}

int SBOOT_Validate_Signature(u8 AuthAlg, u8 HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return SBOOT_Validate_Signature_A(AuthAlg, HashAlg, Pk, Msg, Len, Sig);
	} else {
		return SBOOT_Validate_Signature_B(AuthAlg, HashAlg, Pk, Msg, Len, Sig);

	}

}

int SBOOT_Validate_ImgHash(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return SBOOT_Validate_ImgHash_A(HashAlg, ImgHash, SubImgInfo, Num);
	} else {
		return SBOOT_Validate_ImgHash_B(HashAlg, ImgHash, SubImgInfo, Num);
	}

}

int CRYPTO_SHA_Init(HAL_CRYPTO_ADAPTER *pIE_I)
{
	UNUSED(pIE_I);
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return CRYPTO_SHA_Init_A(NULL);
	} else {
		return CRYPTO_SHA_Init_B(NULL);
	}

}

int SBOOT_Validate_Algorithm(u8 *AuthAlg, u8 *HashAlg, u8 ManiAuth, u8 ManiHash)
{
	Boot_Cut_Ver = SYSCFG_RLVersion();

	if (Boot_Cut_Ver == SYSCFG_CUT_VERSION_A) {
		return SBOOT_Validate_Algorithm_A(AuthAlg, HashAlg, ManiAuth, ManiHash);
	} else {
		return SBOOT_Validate_Algorithm_B(AuthAlg, HashAlg, ManiAuth, ManiHash);
	}
}
#endif
/******************* (C) COPYRIGHT 2022 Realtek Semiconductor *****END OF FILE****/