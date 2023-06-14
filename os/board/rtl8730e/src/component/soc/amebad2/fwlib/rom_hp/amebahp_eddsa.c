#include "ameba_soc.h"

__weak HAL_ROM_TEXT_SECTION
void EDDSA_EngRst(EDDSA_TypeDef *Ed25519)
{
	Ed25519->ENG_CTRLR = 0;

	Ed25519->ENG_INITR |= EDDSA_BIT_ENG_RST;
	Ed25519->ENG_INITR &= ~EDDSA_BIT_ENG_RST;

}

/**
  * @brief  enable the eddsa engine or not
  * @param  status: ENABLE/DISABLE.
  * @retval NONE
  */
__weak HAL_ROM_TEXT_SECTION
void EDDSA_Cmd(EDDSA_TypeDef *Ed25519, u8 NewState)
{
	if (NewState != DISABLE) {
		/* Enable the peripheral */
		Ed25519->ENG_CONF_INTR |= EDDSA_BIT_ENG_START;
	} else {
		/* Disable the peripheral */
		Ed25519->ENG_CONF_INTR &= (~EDDSA_BIT_ENG_START);
	}
}

/**
  * @brief  enable Public key recover or not (for pubkeys which are compressed)
  * @param  status: ENABLE/DISABLE.
  * @retval NONE
  */
__weak HAL_ROM_TEXT_SECTION
void EDDSA_PubRecover_Cmd(EDDSA_TypeDef *Ed25519, u8 NewState)
{
	if (NewState != DISABLE) {
		/* Enable decomp */
		Ed25519->ENG_CTRLR |= EDDSA_BIT_RECOVER_X_EN;
	} else {
		Ed25519->ENG_CTRLR &= ~EDDSA_BIT_RECOVER_X_EN;
	}
}

/**
  * @brief  enable auto mode or not
  * @param  status: ENABLE/DISABLE.
  * @retval NONE
  */
__weak HAL_ROM_TEXT_SECTION
void EDDSA_AutoMode(EDDSA_TypeDef *Ed25519, u8 NewState)
{
	if (NewState != DISABLE) {
		/* Enable decomp */
		Ed25519->ENG_CTRLR |= EDDSA_BIT_AUTO_SP;
	} else {
		Ed25519->ENG_CTRLR &= ~EDDSA_BIT_AUTO_SP;
	}
}

/**
  * @brief select target engine mode
  * @param  mode: selected engine mode
  * @retval NONE
  */
__weak HAL_ROM_TEXT_SECTION
void EDDSA_ModeSel(EDDSA_TypeDef *Ed25519, u32 mode)
{
	Ed25519->ENG_CTRLR &= (~EDDSA_MASK_MOD_SEL);

	switch (mode) {
	case EDDSA_ENG_MODE0:
		Ed25519->ENG_CTRLR |= EDDSA_MOD_SEL(EDDSA_ENG_MODE0);
		break;

	case EDDSA_ENG_MODE1:
		Ed25519->ENG_CTRLR |= EDDSA_MOD_SEL(EDDSA_ENG_MODE1);
		break;

	case EDDSA_ENG_MODE2:
		Ed25519->ENG_CTRLR |= EDDSA_MOD_SEL(EDDSA_ENG_MODE2);
		break;

	case EDDSA_ENG_MODE3:
		Ed25519->ENG_CTRLR |= EDDSA_MOD_SEL(EDDSA_ENG_MODE3);
		break;
	}
}

/**
  * @brief  verify ed25519 sign step by step
  * @param  pk: public key buf
  * @param  sign: input sign buf, shpuld be 64bytes
  * @param  h: input hash buf, shpuld be 64bytes
  * @retval Verify status, 0 verify OK, 1 fail or error.
  */
__weak HAL_ROM_TEXT_SECTION
int EDDSA_Verify_StepMode(EDDSA_TypeDef *Ed25519, u8 *pk, u8 *sign, u8 *h)
{
	int i;
	u8 *rcheck;
	u32 EDDSA_TimeOut = 20000000;

	EDDSA_EngRst(Ed25519);

	/* enable x recover */
	EDDSA_PubRecover_Cmd(Ed25519, ENABLE);
	EDDSA_ModeSel(Ed25519, EDDSA_ENG_MODE0);

	/* S * B & A decomp */
	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_Y2_Px[i]), &(pk[4 * i]), 4);
	}

	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_X1_Px[i]), &(sign[4 * i + 32]), 4);
	}

	EDDSA_Cmd(Ed25519, ENABLE);

	while ((Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_IDLE)) {
		EDDSA_TimeOut--;
		if (EDDSA_TimeOut == 0) {
			return EDDSA_ERR_TIMEOUT_STEP1;
		} else if (Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_DECODE_FAIL) {
			return EDDSA_ERR_ERROR_STEP1;
		}
	}

	/* hash value */
	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_X1_Px[i]), &(h[4 * i]), 4);
		_memcpy((char *) & (Ed25519->ENG_Y1_Px[i]), &(h[4 * i + 32]), 4);
	}

	/* pub key */
	for (i = 0; i < 8; i++) {
		Ed25519->ENG_X2_Px[i] = Ed25519->ENG_XO2_Px[i];
		Ed25519->ENG_Y2_Px[i] = Ed25519->ENG_YO2_Px[i];
	}

	EDDSA_PubRecover_Cmd(Ed25519, DISABLE);
	EDDSA_ModeSel(Ed25519, EDDSA_ENG_MODE1);

	EDDSA_Cmd(Ed25519, ENABLE);

	EDDSA_TimeOut = 20000000;
	while ((Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_IDLE)) {
		EDDSA_TimeOut--;
		if (EDDSA_TimeOut == 0) {
			return EDDSA_ERR_TIMEOUT_STEP2;
		} else if (Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_DECODE_FAIL) {
			return EDDSA_ERR_ERROR_STEP2;
		}
	}

	EDDSA_ModeSel(Ed25519, EDDSA_ENG_MODE2);
	EDDSA_Cmd(Ed25519, ENABLE);

	EDDSA_TimeOut = 20000000;
	while ((Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_IDLE)) {
		EDDSA_TimeOut--;
		if (EDDSA_TimeOut == 0) {
			return EDDSA_ERR_TIMEOUT_STEP3;
		} else if (Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_DECODE_FAIL) {
			return EDDSA_ERR_ERROR_STEP3;
		}
	}

	rcheck = (u8 *)Ed25519->ENG_YO2_Px;
	for (i = 0; i < 32; i++) {
		if (rcheck[i] != sign[i]) {
			return EDDSA_ERR_VERIFY_FAIL;
		}
	}

	return 0;
}

/**
  * @brief  verify ed25519 sign in half auto mode
  * @param  pk: public key buf
  * @param  sign: input sign buf, should be 64bytes
  * @param  h: input hash buf, should be 64bytes
  * @note  half auto mode is recommanded, cause S and hash value need the same reg in auto mode,
  *        but the time to write S and hash is hard to control in auto mode.
  * @retval Verify status, 0 verify OK, 1 fail or error.
  */
__weak HAL_ROM_TEXT_SECTION
int EDDSA_Verify_HalfAutoMode(EDDSA_TypeDef *Ed25519, u8 *pk, u8 *sign, u8 *h)
{
	int i;
	u8 *rcheck;
	u32 EDDSA_TimeOut = 20000000;

	EDDSA_EngRst(Ed25519);

	/* enable x recover */
	EDDSA_PubRecover_Cmd(Ed25519, ENABLE);
	EDDSA_ModeSel(Ed25519, EDDSA_ENG_MODE0);

	/* S*B & A decomp */
	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_Y2_Px[i]), &(pk[4 * i]), 4);

	}

	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_X1_Px[i]), &(sign[4 * i + 32]), 4);
	}

	EDDSA_Cmd(Ed25519, ENABLE);

	while ((Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_IDLE)) {
		EDDSA_TimeOut--;
		if (EDDSA_TimeOut == 0) {
			return EDDSA_ERR_TIMEOUT_STEP1;
		} else if (Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_DECODE_FAIL) {
			return EDDSA_ERR_ERROR_STEP1;
		}
	}

	/* hash value */
	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_X1_Px[i]), &(h[4 * i]), 4);
		_memcpy((char *) & (Ed25519->ENG_Y1_Px[i]), &(h[4 * i + 32]), 4);
	}

	/* pub key */
	for (i = 0; i < 8; i++) {
		Ed25519->ENG_X2_Px[i] = Ed25519->ENG_XO2_Px[i];
		Ed25519->ENG_Y2_Px[i] = Ed25519->ENG_YO2_Px[i];
	}

	Ed25519->ENG_CONF_INTR |= EDDSA_BIT_HASH_OK;

	EDDSA_PubRecover_Cmd(Ed25519, DISABLE);
	EDDSA_ModeSel(Ed25519, EDDSA_ENG_MODE1);
	EDDSA_AutoMode(Ed25519, ENABLE);

	EDDSA_Cmd(Ed25519, ENABLE);

	EDDSA_TimeOut = 20000000;
	while ((Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_IDLE)) {
		EDDSA_TimeOut--;
		if (EDDSA_TimeOut == 0) {
			return EDDSA_ERR_TIMEOUT_STEP2;
		} else if (Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_DECODE_FAIL) {
			return EDDSA_ERR_ERROR_STEP2;
		}
	}

	rcheck = (u8 *)Ed25519->ENG_YO2_Px;
	for (i = 0; i < 32; i++) {
		if (rcheck[i] != sign[i]) {
			return EDDSA_ERR_VERIFY_FAIL;
		}
	}

	return 0;
}

/**
  * @brief  verify ed25519 sign in auto mode
  * @param  pk: public key buf
  * @param  sign: input sign buf, shpuld be 64bytes
  * @param  h: input hash buf, shpuld be 64bytes
  * @retval Verify status, 0 verify OK, 1 fail or error.
  */
__weak HAL_ROM_TEXT_SECTION
int EDDSA_Verify_AutoMode(EDDSA_TypeDef *Ed25519, u8 *pk, u8 *sign, u8 *h)
{
	int i;
	u8 *rcheck;
	u32 EDDSA_TimeOut = 20000000;

	EDDSA_EngRst(Ed25519);

	/* enable x recover */
	EDDSA_PubRecover_Cmd(Ed25519, ENABLE);
	EDDSA_ModeSel(Ed25519, EDDSA_ENG_MODE0);
	EDDSA_AutoMode(Ed25519, ENABLE);

	/* S*B & A decomp */
	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_Y2_Px[i]), &(pk[4 * i]), 4); //compressed A pubkey

	}

	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_X1_Px[i]), &(sign[4 * i + 32]), 4); //S sign last 32 bytes
	}

	EDDSA_Cmd(Ed25519, ENABLE);

	/* hash value */
	for (i = 0; i < 8; i++) {
		_memcpy((char *) & (Ed25519->ENG_X1_Px[i]), &(h[4 * i]), 4);
		_memcpy((char *) & (Ed25519->ENG_Y1_Px[i]), &(h[4 * i + 32]), 4);
	}

	Ed25519->ENG_CONF_INTR |= EDDSA_BIT_HASH_OK;

	while ((Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_IDLE)) {
		EDDSA_TimeOut--;
		if (EDDSA_TimeOut == 0) {
			return EDDSA_ERR_TIMEOUT_STEP1;
		} else if (Ed25519->ENG_CONF_INTR & EDDSA_BIT_ENG_DECODE_FAIL) {
			return EDDSA_ERR_ERROR_STEP1;
		}
	}

	rcheck = (u8 *)Ed25519->ENG_YO2_Px;
	for (i = 0; i < 32; i++) {
		if (rcheck[i] != sign[i]) {
			return EDDSA_ERR_VERIFY_FAIL;
		}
	}

	return 0;
}

