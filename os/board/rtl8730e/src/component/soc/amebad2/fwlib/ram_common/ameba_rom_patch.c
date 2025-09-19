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

#ifdef ARM_CORE_CA32
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "os_wrapper_critical.h"
#endif //CONFIG_PLATFORM_TIZENRT_OS
#endif //ARM_CORE_CA32

#ifndef MIN
#define MIN(x, y)			(((x) < (y)) ? (x) : (y))
#endif

static const char *TAG = "ROMPATCH";
static LOG_UART_PORT LOG_UART_IDX_FLAG[] = {
	{1, LOGUART_BIT_TP2F_NOT_FULL, LOGUART_BIT_TP2F_EMPTY, 52125, UART_LOG_IRQ},	/* CM0 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{0, LOGUART_BIT_TP1F_NOT_FULL, LOGUART_BIT_TP1F_EMPTY, 781875, UART_LOG_IRQ},		/* CM4 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{3, LOGUART_BIT_TP4F_NOT_FULL, LOGUART_BIT_TP4F_EMPTY, 3127500, UART_LOG_IRQ},	/* CA7 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
};

char num_buf_temp[10];

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
		RCC_PeriphClockCmd(APBPeriph_AUDIO, APBPeriph_CLOCK_NULL, ENABLE);

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

void GPIO_Init(GPIO_InitTypeDef *GPIO_InitStruct)
{
	/* open gpio function and clock */
	//RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

	assert_param(GPIO_InitStruct->GPIO_Mode <= GPIO_Mode_INT);

	/* GPIO Pad shouddown control: Turn on the corresponding GPIO Pad and pinmux to GPIO */
	Pinmux_Config(GPIO_InitStruct->GPIO_Pin, PINMUX_FUNCTION_GPIO);

	if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_INT) {
		GPIO_Direction(GPIO_InitStruct->GPIO_Pin, GPIO_Mode_IN);
		PAD_PullCtrl(GPIO_InitStruct->GPIO_Pin, GPIO_InitStruct->GPIO_PuPd);

		GPIO_INTMode(GPIO_InitStruct->GPIO_Pin, ENABLE, GPIO_InitStruct->GPIO_ITTrigger,
					 GPIO_InitStruct->GPIO_ITPolarity, GPIO_InitStruct->GPIO_ITDebounce);
	} else {
		GPIO_INTMode(GPIO_InitStruct->GPIO_Pin, DISABLE, 0, 0, 0);

		if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT) {
			GPIO_Direction(GPIO_InitStruct->GPIO_Pin, GPIO_Mode_OUT);
		} else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IN) {
			GPIO_Direction(GPIO_InitStruct->GPIO_Pin, GPIO_Mode_IN);
			PAD_PullCtrl(GPIO_InitStruct->GPIO_Pin, GPIO_InitStruct->GPIO_PuPd);
		}
	}
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
  * @brief Initialize the UARTx peripheral according to the specified
  *        parameters in the UART_InitStruct.
  * @param UARTx: where x can be 0/1/2/3.
  * @param UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *        the configuration information for the specified UART peripheral.
  * @retval None
  */
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
	u32 TempFcr;
	u32 TempRxPath;

	/*check the parameters*/
	assert_param(IS_UART_RXFIFO_LEVEL(UART_InitStruct->RxFifoTrigLevel));
	assert_param(IS_UART_RX_ERROR_REPORT(UART_InitStruct->RxErReportCtrl));

	/*disable rx path*/
	UARTx->RX_PATH_CTRL &= ~RUART_BIT_R_RST_NEWRX_N;

	/*clear rx fifo */
	UART_ClearRxFifo(UARTx);

	/*clear tx fifo */
	UART_ClearTxFifo(UARTx);

	/* Disable all interrupts */
	UARTx->IER = 0x00;

	/* Clean Rx break signal interrupt status at initial stage.*/
	UARTx->SCR |= RUART_BIT_SCRATCH_7;

	/*get the FCR register value*/
	TempFcr = UARTx->FCR;

	/*clear the rx fifo level and rx error report feild ( FCR[7:6] & FCR[0])*/
	TempFcr &= ~(RUART_BIT_FIFO_EN | RUART_MASK_RECVTRG);

	/*set the rx fifo trigger level*/
	TempFcr |= UART_InitStruct->RxFifoTrigLevel;

	/*enable or disable rx error report*/
	TempFcr |= UART_InitStruct->RxErReportCtrl;

	UARTx->FCR = TempFcr;

	/* Configure FlowControl */
	if (UART_InitStruct->FlowControl == ENABLE) {
		UARTx->MCR |= RUART_MCL_FLOW_ENABLE;
	} else {
		UARTx->MCR &= ~ RUART_MCL_FLOW_ENABLE;
	}

	/*configure word length, stop bit, parity, parity type, stickparity*/
	UARTx->LCR = ((UART_InitStruct->WordLen) |
				  (UART_InitStruct->StopBit << 2) |
				  (UART_InitStruct->Parity << 3) |
				  (UART_InitStruct->ParityType << 4) |
				  (UART_InitStruct->StickParity << 5));

	/* disable or enable the UART DMA mode */
	if (UART_InitStruct->DmaModeCtrl != DISABLE) {
		UARTx->FCR |= RUART_BIT_DMA_MODE;
	} else {
		UARTx->FCR &= (~ RUART_BIT_DMA_MODE);
	}

	/*configure rx timeout counter*/
	TempRxPath = UARTx->RX_PATH_CTRL;
	TempRxPath &= ~(RUART_MASK_R_RXTO_THRS);
	TempRxPath |= (UART_InitStruct->RxTimeOutCnt << 16);
	UARTx->RX_PATH_CTRL = TempRxPath;
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
		RTK_LOGW(TAG, "irq[%lx] priority %lu shall <= %d\n", irqn, priority, MAX_IRQ_PRIORITY_VALUE);
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


void
INT_I2C1(void)
{
	INT_Entry_Patch(I2C1_IRQ);
}

void
INT_I2C2(void)
{
	INT_Entry_Patch(I2C2_IRQ);
}

void VectorTableAdd(void)
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

#define get_num_va_args(_args, _lcount)				\
	(((_lcount) > 1)  ? va_arg(_args, long long int) :	\
	(((_lcount) == 1) ? va_arg(_args, long int) :		\
			    va_arg(_args, int)))

#define get_unum_va_args(_args, _lcount)				\
	(((_lcount) > 1)  ? va_arg(_args, unsigned long long int) :	\
	(((_lcount) == 1) ? va_arg(_args, unsigned long int) :		\
			    va_arg(_args, unsigned int)))

#define get_char_upper_lower(_upper, _rem)				\
	(((_rem) < 0xa)   ?  ('0' + _rem) : 					\
	(((_upper) == 1)   ?  ('A' + (_rem - 0xa)) : 			\
	('a' + (_rem - 0xa))))

#define is_digit(c) ((c >= '0') && (c <= '9'))

static int print_string(const char *str)
{
	int count = 0;

	//assert_param(str != NULL);
	if (str == NULL) {
		return -1;
	}
	for (; *str != '\0'; str++) {
		DiagPutChar(*str);
		count++;
	}
	return count;
}

static int print_unsigned_num(unsigned long long int unum, unsigned int radix,
							  char padc, int padn, int upper, int sign)
{
	/* Just need enough space to store 64 bit decimal integer */
	char num_buf[20];
	int i = 0, count = 0;
	unsigned int rem;
	/* 1. convert num to str*/
	do {
		rem = unum % radix;
		num_buf[i] = get_char_upper_lower(upper, rem);
		i++;
		unum /= radix;
	} while (unum > 0U);
	/* 2. If there is a sign bit, subtract the padn by 1. */
	if (sign) {
		count++;
		padn--;
	}

	/* 3. If fill 0, need to print the sign first, and then fill in 0 */
	if (sign && (padc == '0')) {
		DiagPutChar('-');
	}
	/* 4. Fill the character padc */
	if (padn > 0) {
		while (i < padn) {
			DiagPutChar(padc);
			count++;
			padn--;
		}
	}
	/* 5.If fill a space, need to wait for step 4 to complete before filling the sign. */
	if (sign && (padc == ' ' || padc == '\0')) {
		DiagPutChar('-');
	}
	/* 6. Print the numbers*/
	while (--i >= 0) {
		DiagPutChar(num_buf[i]);
		count++;
	}

	return count;
}

static inline int pad_char_control(int padn, char padc)
{
	int count = 0;
	while (padn > 0) {
		DiagPutChar(padc);
		padn--;
		count++;
	}
	return count;
}

static inline int pad_num_control(unsigned long long int unum, unsigned int radix, char padc,
								  int padn, int upper, int sign, int pad_on_right)
{
	int count = 0;
	/* 1. if '-' left align was set, pad character in the right of num*/
	if (pad_on_right) {
		int width = print_unsigned_num(unum, radix, padc, 0, upper, sign);
		count += width;
		//if padc is equal to '\space' and pad number is great than width of num, execute pad opration. Otherwise, no operation.
		if ((padn > width) && (padc  == ' ')) {
			count += pad_char_control(padn - width, padc);
		}
		/* 2. Normally, pad operation is executed on the left of num.*/
	} else {
		count += print_unsigned_num(unum, radix, padc, padn, upper, sign);
	}
	return count;
}
/*******************************************************************
 * Reduced format print for Trusted firmware.
 * The following type specifiers are supported by this print
 * %lx - hexadecimal format
 * %s - string format
 * %lu or %i - signed decimal format
 * %u - unsigned decimal format
 * %p - pointer format
 *
 * The following length specifiers are supported by this print
 * %l - long int (64-bit on AArch64)
 * %ll - long long int (64-bit on AArch64)
 * %z - size_t sized integer formats (64 bit on AArch64)
 *
 * The following padding specifiers are supported by this print
 * %0NN - Left-pad the number with 0s (NN is a decimal number)
 * %NN - Left-pad the number with spaces
 * %-NN - Right-pad the number with spaces
 *
 * The print exits on all other formats specifiers other than valid
 * combinations of the above specifiers.
 *******************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
int DiagVprintf(const char *fmt, va_list args)
{
	int l_count;
	long long int num;
	unsigned long long int unum;
	char *str;

	int count = 0; 		/* Number of printed characters */
	int upper = 0;

	// pad control
	char padc = '\0'; 	/* Padding character */
	int padn; 			/* Number of characters to pad */
	int pad_on_right;
	int sign = 0;
	int flag = 0;

	char c = '\0';
	int percent_flag = 0;

	while (*fmt != '\0') {
		l_count = 0;
		percent_flag = pad_on_right = padn = sign = flag = 0;
		padc = '\0';

		if (*fmt == '%') {
			percent_flag = 1;
			fmt++;
			/* Check the format specifier */
loop:
			switch (*fmt) {
			case 'i': /* Fall through to next one */
			case 'd':
				num = get_num_va_args(args, l_count);
				if (num < 0) {
					sign = 1;
					unum = (unsigned long long int) - num;
				} else {
					unum = (unsigned long long int)num;
				}
				count += pad_num_control(unum, 10, padc, padn, upper, sign, pad_on_right);
				break;
			case 'u':
				unum = get_unum_va_args(args, l_count);
				count += pad_num_control(unum, 10, padc, padn, upper, sign, pad_on_right);
				break;
			case 'B':
			case 'b':
				unum = get_unum_va_args(args, l_count);
				count += pad_num_control(unum, 2, padc, padn, upper, sign, pad_on_right);
				break;
			case 'O':
			case 'o':
				unum = get_unum_va_args(args, l_count);
				count += pad_num_control(unum, 8, padc, padn, upper, sign, pad_on_right);
				break;
			case 'X':
				upper = 1;
			case 'x':
				unum = get_unum_va_args(args, l_count);
				count += pad_num_control(unum, 16, padc, padn, upper, sign, pad_on_right);
				break;
			case 'p':
				upper = 1;
				unum = (uintptr_t)va_arg(args, void *);
				if (sizeof(uintptr_t) == 4U) {
					padc = '0';
					padn = 8;
				} else {
					padc = '0';
					padn = 16;
				}
				count += pad_num_control(unum, 16, padc, padn, upper, sign, pad_on_right);
				break;
			case 's':
				str = va_arg(args, char *);
				char *tempstr = str;
				int tempcount = 0;
				while (*tempstr != '\0') {
					tempstr++;
					tempcount++;
				}
				// left pad
				if (!pad_on_right && (padn - tempcount > 0)) {
					count += pad_char_control(padn - tempcount, padc);
				}
				// print string
				count += print_string(str);
				// right pad
				if (pad_on_right && (padn - tempcount > 0)) {
					count += pad_char_control(padn - tempcount, padc);
				}
				break;
			case 'c':
			case 'C':
				c = va_arg(args, int);
				// left pad
				if (!pad_on_right && (padn > 1)) {
					count += pad_char_control(padn - 1, padc);
				}
				DiagPutChar(c);
				count++;
				//right pad
				if (pad_on_right && (padn > 1)) {
					count += pad_char_control(padn - 1, padc);
				}
				break;
			case 'z':
				if (sizeof(size_t) == 8U) {
					goto not_support;
				}
				fmt++;
				goto loop;
			case 'L':
			case 'l':
				l_count++;
				fmt++;
				goto loop;
			case '-':
				fmt++;
				pad_on_right++;
				goto loop;
			/*% is followed by a %. */
			case '%':
				if (percent_flag) {
					DiagPutChar('%');
				}
				count++;
				break;
			case '0':
				padc = '0';
				padn = 0;
				fmt++;
				goto pad_count;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				padc = ' ';
				padn = 0;
				goto pad_count;
			/*% is followed by a spaces, so padc = \space. */
			case ' ':
				padc = ' ';
				padn = 0;
				fmt++;
pad_count:
				for (;;) {
					char ch = *fmt;
					if (ch == '\0') { // deal with case '% \0'.
						goto exit;
					} else if (!is_digit(ch)) {
						goto loop;
					}
					padn = (padn * 10) + (ch - '0');
					fmt++;
				}
				//assert_param(0); /* Unreachable */
				return -1;
			case '\0':
				break;
			default:
not_support:
				/* Exit on any other format specifier */
				return -1;
			}
			fmt++;
			continue;
		}
		DiagPutChar(*fmt);
		fmt++;
		count++;
	}
exit:
	return count;
}

static inline int print_decimal_num(unsigned int unum, int radix, char padc, int padn, int sign)
{
	/* Just need enough space to store 31 bit decimal integer */
	char *num_buf = num_buf_temp;
	int i = 0, count = 0;
	unsigned int rem;
	/* 1. convert num to str*/
	do {
		rem = unum % radix;
		num_buf[i] = get_char_upper_lower(0, rem);
		i++;
		unum /= radix;
	} while (unum > 0U);

	/* 2. Fill the character padc */
	if (padn > 0) {
		while (i < padn) {
			DiagPutChar(padc);
			count++;
			padn--;
		}
	}

	if (sign) {
		DiagPutChar('-');
	}

	/* 3. Print the numbers*/
	while (--i >= 0) {
		DiagPutChar(num_buf[i]);
		count++;
	}

	return count;
}

static int DiagVSprintfStringNum(const char *fmt, va_list args)
{
	char *str;

	int count = 0; 		/* Number of printed characters */
	int sign = 0, num = 0;
	// pad control
	char padc = '\0'; 	/* Padding character */
	int padn; 			/* Number of characters to pad */
	int pad_on_right;

	char c = '\0';
	int percent_flag = 0;

	unsigned int unum;
	while (*fmt != '\0') {
		percent_flag = pad_on_right = padn = 0;
		padc = '\0';
		sign = 0;
		if (*fmt == '%') {
			percent_flag = 1;
			fmt++;
loop:
			/* Check the format specifier */
			switch (*fmt) {
			case 'i':
			case 'd':
				num = get_num_va_args(args, 0);
				if (num < 0) {
					sign = 1;
					unum = (unsigned int) - num;
				} else {
					unum = (unsigned int)num;
				}
				count += print_decimal_num(unum, 10, padc, padn, sign);
				break;
			case 'u':
				unum = (unsigned int)get_unum_va_args(args, 0);
				count += print_decimal_num(unum, 10, padc, padn, 0);
				break;
			case 'x':
				unum = (unsigned int)get_unum_va_args(args, 0);
				count += print_decimal_num(unum, 16, padc, padn, 0);
				break;
			case 's':
				str = va_arg(args, char *);
				char *tempstr = str;
				int tempcount = 0;
				while (*tempstr != '\0') {
					tempstr++;
					tempcount++;
				}
				// left pad
				if (!pad_on_right && (padn - tempcount > 0)) {
					count += pad_char_control(padn - tempcount, padc);
				}
				// print string
				count += print_string(str);
				// right pad
				if (pad_on_right && (padn - tempcount > 0)) {
					count += pad_char_control(padn - tempcount, padc);
				}
				break;
			case 'c':
			case 'C':
				c = va_arg(args, int);
				// left pad
				if (!pad_on_right && (padn > 1)) {
					count += pad_char_control(padn - 1, padc);
				}
				DiagPutChar(c);
				count++;
				//right pad
				if (pad_on_right && (padn > 1)) {
					count += pad_char_control(padn - 1, padc);
				}
				break;
			/*% is followed by a %. */
			case '%':
				if (percent_flag) {
					DiagPutChar('%');
				}
				count++;
				break;
			case '0':
				padc = '0';
				padn = 0;
				fmt++;
				goto pad_count;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				padc = ' ';
				padn = 0;
				goto pad_count;
			/*% is followed by a spaces, so padc = \space. */
			case ' ':
				padc = ' ';
				padn = 0;
				fmt++;
pad_count:
				for (;;) {
					char ch = *fmt;
					if (ch == '\0') { // deal with case '% \0'.
						return count;
					} else if (!is_digit(ch)) {
						goto loop;
					}
					padn = (padn * 10) + (ch - '0');
					fmt++;
				}
			default:
				/* Exit on any other format specifier */
				return -1;
			}
			fmt++;
			continue;
		}
		DiagPutChar(*fmt);
		fmt++;
		count++;
	}

	return count;
}


#pragma GCC diagnostic pop

u32 DiagPrintf(const char *fmt, ...)
{
	int ret;
	va_list va;

	va_start(va, fmt);
	ret = DiagVprintf(fmt, va);
	va_end(va);
	return ret;
}

/* support %s %c */
/* and num %d %x, format like %08.. is supported too, num must within 32bits */
u32 DiagPrintf_minimal(const char *fmt, ...)
{
	int ret;
	va_list va;

#ifdef ARM_CORE_CA32
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	irqstate_t flags = tizenrt_critical_enter();
#else
	u32 isr_status = spin_lock_irqsave(&print_lock);
#endif //CONFIG_PLATFORM_TIZENRT_OS
#endif //ARM_CORE_CA32

	va_start(va, fmt);
	ret = DiagVSprintfStringNum(fmt, va);
	va_end(va);

#ifdef ARM_CORE_CA32
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	tizenrt_critical_exit(flags);
#else
	spin_unlock_irqrestore(&print_lock, isr_status);
#endif //CONFIG_PLATFORM_TIZENRT_OS
#endif //ARM_CORE_CA32
	return ret;
}
/* support %s %d %x %c, %08x.*/
int DiagSnPrintf(char *buf, size_t size, const char *fmt, ...)
{
	va_list     ap;
	char *p, *s, *buf_end = NULL;
	char padc = ' ';
	int padn = 0;

	if (buf == NULL) {
		return 0;
	}

	va_start(ap, fmt);
	s = buf;
	buf_end = size ? (buf + size) : (char *)~0;
	for (; *fmt != '\0'; ++fmt) {
		padn = 0;
		padc = ' ';
		if (*fmt != '%') {
			*s++ = *fmt;

			if (s >= buf_end) {
				goto Exit;
			}

			continue;
		}
		if (*++fmt == 's') {
			p = va_arg(ap, char *);
			while (*p != '\0') {
				*s++ = *p++;
				if (s >= buf_end) {
					goto Exit;
				}
			}
		} else {	/* Length of item is bounded */

			char tmp[20], *q = tmp;
			int shift = 0;// = 12;

			int lpforchk = 0;
			if ((*fmt == 'p') || (*fmt == 'P')) {
				lpforchk = (uintptr_t)va_arg(ap, void *);
				if (sizeof(uintptr_t) == 4U) {
					padc = '0';
					padn = 8;
				} else {
					padc = '0';
					padn = 16;
				}
			} else {
				lpforchk = va_arg(ap, int);
			}
			if ((lpforchk) < 0x10) {
				shift = 0;
			} else if (((lpforchk) >= 0x10) && ((lpforchk) < 0x100)) {
				shift = 4;
			} else if (((lpforchk) >= 0x100) && ((lpforchk) < 0x1000)) {
				shift = 8;
			} else if (((lpforchk) >= 0x1000) && ((lpforchk) < 0x10000)) {
				shift = 12;
			} else if (((lpforchk) >= 0x10000) && ((lpforchk) < 0x100000)) {
				shift = 16;
			} else if (((lpforchk) >= 0x100000) && ((lpforchk) < 0x1000000)) {
				shift = 20;
			} else if (((lpforchk) >= 0x1000000) && ((lpforchk) < 0x10000000)) {
				shift = 24;
			} else if ((lpforchk) >= 0x10000000) {
				shift = 28;
			} else {
				shift = 28;
			}

			if (*fmt == '0') {
				fmt++;
				padc = '0';
			}

			if ((*fmt  > '0') && (*fmt  <= '9')) {
				int width;
				unsigned char fch = *fmt;
				for (width = 0; (fch >= '0') && (fch <= '9'); fch = *++fmt) {
					width = width * 10 + fch - '0';
				}
				padn = width;
			}
			/*
			 * Before each format q points to tmp buffer
			 * After each format q points past end of item
			 * Only %p and %x can be filled with digits 0 or spaces.
			 */
			if ((*fmt == 'x') || (*fmt == 'X') || (*fmt == 'p') || (*fmt == 'P')) {
				/* With x86 gcc, sizeof(long) == sizeof(int) */
				const int lp = lpforchk;
				int h = lp;
				int hex_count = 0;
				unsigned int h_back = h;
				int ncase = (*fmt & 0x20);

				if ((*fmt == 'p') || (*fmt == 'P')) {
					*q++ = '0';
					*q++ = 'X' | ncase;
				}

				while (h_back) {
					hex_count += 1;
					h_back  = h_back >> 4;
				}
				while (padn-- > hex_count) {
					*q++ = padc;
				}

				for (; shift >= 0; shift -= 4) {
					*q++ = "0123456789ABCDEF"[(h >> shift) & 0xF] | ncase;
				}
			} else if (*fmt == 'd') {
				int i = lpforchk;
				char *r;
				int digit_space = 0;

				if (i < 0) {
					*q++ = '-';
					i = -i;
					digit_space++;
				}

				p = q;		/* save beginning of digits */

				do {
					*q++ = '0' + (i % 10);
					i /= 10;
					digit_space++;
				} while (i);


				for (; shift >= 0; shift -= 4) {

					if (digit_space-- > 0) {
						; //do nothing
					} else {
						*q++ = '0';
					}
				}

				/* reverse digits, stop in middle */
				r = q;		/* don't alter q */

				while (--r > p) {
					i = *r;
					*r = *p;
					*p++ = i;
				}
			} else if (*fmt == 'c') {
				*q++ = lpforchk;
			} else {
				*q++ = *fmt;
			}

			/* now output the saved string */
			for (p = tmp; p < q; ++p) {
				*s++ = *p;

				if (s >= buf_end) {
					goto Exit;
				}
			}
		}
	}
Exit:

	if (buf) {
		*s = '\0';
	}
	va_end(ap);
	return (s - buf);
}

int TRNG_get_random_bytes(void *dst, u32 size)
{
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = size / sizeof(unsigned int);
	lp = (unsigned int *) dst;

	for (i = 0; i < count; i ++) {
		lp[i] = _rand();
		size -= sizeof(unsigned int);
	}

	if (size > 0) {
		ranbuf = _rand();
		_memcpy(&lp[i], &ranbuf, size);
	}
	return 0;
}

int TRNG_get_random_bytes_f_rng(void *p_rng, unsigned char *output, size_t output_size)
{
	UNUSED(p_rng);
	return TRNG_get_random_bytes(output, output_size);
}

__NO_RETURN
void io_assert_failed(uint8_t *file, uint32_t line)
{
	RTK_LOGS(NOTAG, "io driver parameters error! file_name: %s, line: %d", file, line);

	for (;;);
}

void CRYPTO_Reset(UNUSED_WARN_DIS HAL_CRYPTO_ADAPTER *pIE)
{
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) AES_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)AES_REG_BASE_S;
	}
	/*take sema to obtain the right to crypto engine*/
	while (IPC_SEMTake(IPC_SEM_CRYPTO, 0xFFFFFFFF) != _TRUE) {
		RTK_LOGE(TAG, "ipsec get hw sema fail\n");
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

	/*free sema to release the right to crypto engine*/
	IPC_SEMFree(IPC_SEM_CRYPTO);
}

void CRYPTO_SHA_Reset(UNUSED_WARN_DIS HAL_CRYPTO_ADAPTER *pIE)
{
	IPSEC_TypeDef *IPSEC = (IPSEC_TypeDef *) SHA_REG_BASE;

	if (TrustZone_IsSecure()) {
		IPSEC = (IPSEC_TypeDef *)SHA_REG_BASE_S;
	}

	/*take sema to obtain the right to crypto engine*/
	while (IPC_SEMTake(IPC_SEM_CRYPTO, 0xFFFFFFFF) != _TRUE) {
		RTK_LOGE(TAG, "ipsec get hw sema fail\n");
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

	/*free sema to release the right to crypto engine*/
	IPC_SEMFree(IPC_SEM_CRYPTO);
}

/******************* (C) COPYRIGHT 2022 Realtek Semiconductor *****END OF FILE****/
