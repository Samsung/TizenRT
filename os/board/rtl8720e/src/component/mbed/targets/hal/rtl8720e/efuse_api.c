/** mbed Microcontroller Library
  ******************************************************************************
  * @file    efuse_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for EFUSE.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"
#include "rom_aes.h"
//#ifdef CONFIG_EFUSE_EN

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_OTPC
 *  @brief    MBED_OTPC driver modules.
 *  @{
 */

/** @defgroup MBED_OTPC_Exported_Functions MBED_OTPC Exported Functions
  * @{
  */

/**
  * @brief  Get remaining eFuse length.
  * @return Remaining eFuse length.
  */
int  efuse_get_remaining_length(void)
{
	return EFUSE_RemainLength();
}

/**
  * @brief  Read eFuse content of specified user logical OTP.
  * @param  data: Address to save the readback data.
  * @retval none
  * @note Read user logical OTP(0x2D0~0x2FF).
  */
void  efuse_mtp_read(uint8_t *data)
{
	u8 EfuseBuf[OTP_LMAP_LEN];
	u8 ret;

	/*0xff will be eFuse default value instead of 0x00. */
	_memset(data, 0xFF, OTP_MTP_USER_LEN);

	ret = EFUSE_LMAP_READ(EfuseBuf);

	if (ret == _FAIL) {
		DBG_8195A("EFUSE_LogicalMap_Read fail \n");
	}

	_memcpy(data, EfuseBuf + OTP_MTP_USER_START, OTP_MTP_USER_LEN);

}

/**
  * @brief  Write user's content to user logical OTP.
  * @param  data: Address to save the data.
  * @param  len: Data length of programmed data.
  * @return Write result. It can be one of the following values:
  * @retval 1~32: Success.
  * @retval 0/-1: Error.
  * @note Read user logical OTP(0x2D0~0x2FF).
  */
int  efuse_mtp_write(uint8_t *data, uint8_t len)
{
	u32 bResult;

	if (len > OTP_MTP_USER_LEN)	{
		DBG_8195A("string length should be smaller than OTP_MTP_USER_LEN\n");
		return -1;
	}


	bResult = EFUSE_LMAP_WRITE(OTP_MTP_USER_START, len, data);
	if (!bResult) {
		DBG_8195A("write fail \n");
		return -1;
	} else {
		return len;
	}
}

/**
  * @brief  Read OTP physical content.
  * @param  address: Offset of the physical OTP.
  * @param  len: Length of readback data.
  * @param  buf: Address to save the readback data.
  * @return Read result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  * @note Read user OTP(0x380~0x4FF).
  */
int efuse_otp_read(u8 address, u8 len, u8 *buf)
{
	u8 content[OTP_USER_LEN];	// the OTP max length is 256
	u32 index;
	u32 bResult;
	if ((address + len) > OTP_USER_LEN) {
		return -1;
	}
	for (index = 0; index < OTP_USER_LEN; index++) {
		bResult = EFUSE_PMAP_READ8(0, OTP_USER_START + index, content + index, L25EOUTVOLTAGE);
		if (!bResult) {
			return -1;
		}
	}

	_memcpy(buf, content + address, len);
	return 0;

}

/**
  * @brief  Write user's content to user physical OTP.
  * @param  address: Offset of the programmed OTP.
  * @param  len: Data length of programmed data.
  * @param  buf: Data to be programmed.
  * @return Write result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  * @note Read user OTP(0x380~0x4FF).
  */
int efuse_otp_write(u8 address, u8 len, u8 *buf)
{
	u8 index;
	u32 bResult;

	if ((address + len) > OTP_USER_LEN) {
		return -1;
	}

	for (index = 0; index < len; index++) {
		bResult = EFUSE_PMAP_WRITE8(0, address + OTP_USER_START + index, buf[index], L25EOUTVOLTAGE);
		if (!bResult) {
			return -1;
		}
	}
	return 0;
}

/**
  * @brief  Check user's content to OTP efuse.
  * @param  len: Data length of programmed data.
  * @param  buf: Poniter to the data to be programmed.
  * @return Check result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  */
int efuse_otp_chk(u8 len, u8 *buf)
{
	/* To avoid gcc warnings */
	(void) len;
	(void) buf;
	DBG_8195A("AmebaLite not support efuse_otp_chk function!\n");
	return 0;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
//#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
