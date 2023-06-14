/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

/**
  * @brief  Get the Available Data numbers in FIFO
  * @param  None
  * @retval FIFO LEVEL
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
static inline u32 TRNG_GetFIFOLevel(void)
{
	TRNG_TypeDef *TRNG  = TRNG_DEV;

	if (TrustZone_IsSecure()) {
		TRNG = TRNG_SDEV;
	}

	return TRNG->TRNG_FIFO_STATUS;
}

/**
  * @brief  Get Rand data from TRNG
  * @param  None
  * @retval Rand data
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
u32 _rand(void)
{
	u32 counter = 0;
	u32 level;
	TRNG_TypeDef *TRNG  = TRNG_DEV;

	if (TrustZone_IsSecure()) {
		TRNG = TRNG_SDEV;
	}

	do  {
		level = TRNG_GetFIFOLevel();

		/*APT ERROR*/
		if (level == TRNG_FIFO_LEVEL_INVALID) {
			/*sw reset*/
			TRNG->TRNG_SW_RST = TRNG_BIT_SW_RESET;
		} else if (level != TRNG_FIFO_LEVEL_EMPTY) {
			break;
		}

		/*poling timeout*/
		if (counter++ > TRNG_POLLING_TIMEOUT) {
			counter = 0;
			/*sw reset*/
			TRNG->TRNG_SW_RST = TRNG_BIT_SW_RESET;
		}
	}  while (1);

	/*return TRNG value*/
	return TRNG->TRNG_DATAx[0];
}

/**
  * @brief  wrap for _rand
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
u32 __wrap_rand(void)
{
	return _rand();
}

/**
  * @brief  Get Rand data from TRNG
  * @param  buf: TRNG data target buf
   * @param  Len: request data len
  * @retval successful or not
  */
HAL_ROM_TEXT_SECTION _LONG_CALL_
int RandBytes_Get(void *buf, u32 len)
{
	uint32_t ranbuf;
	uint32_t *lp;
	int i, count;
	u32 level;
	TRNG_TypeDef *TRNG  = TRNG_DEV;

	count = len / sizeof(uint32_t);
	lp = (uint32_t *) buf;

	if (TrustZone_IsSecure()) {
		TRNG = TRNG_SDEV;
	}

	level = TRNG_GetFIFOLevel();

	if (level == TRNG_FIFO_LEVEL_INVALID) {
		return _FAIL;
	}

	for (i = 0; (i < count) && (level > 0); i ++) {
		lp[i] = TRNG->TRNG_DATAx[0];
		len -= sizeof(uint32_t);
		level --;
	}

	/*if FIFO empty and did not get enough data*/
	for (; i < count; i++) {
		lp[i] = _rand();
		len -= sizeof(uint32_t);
	}

	/*if FIFO is not alias by 4 bytes,The rate may slower*/
	if (len > 0) {
		ranbuf = _rand();
		_memcpy(&lp[i], &ranbuf, len);
	}

	return _SUCCESS;
}

