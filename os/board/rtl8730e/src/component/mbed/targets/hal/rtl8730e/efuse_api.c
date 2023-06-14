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
int efuse_get_remaining_length(void)
{
	return OTP_RemainLength();
}

/**
  * @brief  Read eFuse content of specified user logical OTP.
  * @param  data: buffer address to save the readback data.
  * @retval none
  * @note Read user logical OTP(0x170~0x19F).
  */
void efuse_mtp_read(uint8_t *data)
{
	u8 ret = _FAIL;

	/*0xff will be eFuse default value instead of 0x00. */
	_memset(data, 0xFF, OTP_MTP_USER_LEN);

	ret = OTP_LogicalMap_Read(data, OTP_MTP_USER_START, OTP_MTP_USER_LEN);

	if (ret == _FAIL) {
		DBG_8195A("ERR: efuse_mtp_read fail \n");
	}

}

/**
  * @brief  Write user's content to user logical OTP.
  * @param  offset: Address offset of the programmed user logical OTP(base address: 0x170).
  * @param  data: Data buffer to be programmed.
  * @param  len: Data length to be programmed.
  *          - 0~32: Success
  *          - -1: Failure
  * @note Write user logical OTP(0x170~0x19F).
  */
int efuse_mtp_write(uint32_t offset, uint32_t len, uint8_t *data)
{
	u32 bResult;

	if ((offset + len) > OTP_MTP_USER_LEN) {
		DBG_8195A("ERR: Write MTP out of boundary\n");
		return -1;
	}

	bResult = OTP_LogicalMap_Write((OTP_MTP_USER_START + offset), len, data);

	if (!bResult) {
		DBG_8195A("write fail \n");
		return -1;
	}

	return 1;
}

/**
  * @brief  Read OTP User-defined Physical content.
  * @param  Offset: Offset of the physical OTP (base address: 0x380)
  * @param  len: Length of readback data.
  * @param  buf: Address to save the readback data.
  * @return Read result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  * @note Read user OTP(0x380~0x4FF)
          0x380~0x3BF:UDF1 Secure Optional
          0x3C0~0x3FF:UDF2 Secure Optional
          0x400~0x4FF:Non-Secure.
  */
int efuse_otp_read(uint32_t offset, uint32_t len, uint8_t *buf)
{
	u32 index;
	u32 bResult;

	if ((offset + len) > OTP_USER_LEN) {
		DBG_8195A("ERR: Read Out of boundary\n");
		return -1;
	}

	for (index = 0; index < len; index++) {
		bResult = OTP_Read8((OTP_USER_START + offset + index), (u8 *)(buf + index));

		if (!bResult) {
			DBG_8195A("ERR: Read User defined physical OTP fail\n");
			return -1;
		}
	}

	return 0;
}

/**
  * @brief  Write user's content to OTP User-defined Physical zone.
  * @param  offset: Offset of the programmed physical OTP(base address: 0x380).
  * @param  len: Data length of programmed data.
  * @param  buf: Data to be programmed.
  * @return Write result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  * @note Read user OTP(0x380~0x4FF)
          0x380~0x3BF:UDF1 Secure Optional
          0x3C0~0x3FF:UDF2 Secure Optional
          0x400~0x4FF:Non-Secure
  */
int efuse_otp_write(uint32_t offset, uint32_t len, uint8_t *buf)
{
	u8 index;
	u32 bResult;

	if ((offset + len) > OTP_USER_LEN) {
		DBG_8195A("ERR: Write Out of boundary\n");
		return -1;
	}

	for (index = 0; index < len; index++) {
		bResult = OTP_Write8((OTP_USER_START + offset + index), buf[index]);

		if (!bResult) {
			DBG_8195A("ERR: Write User defined physical OTP fail\n");
			return -1;
		}
	}
	return 0;
}

/**
  * @brief  Check user's content to OTP efuse.
  * @param  len: Data length of programmed data.
  * @param  buf: Data to be programmed.
  * @return Check result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  */
int efuse_otp_chk(u8 len, u8 *buf)
{
	/* To avoid gcc warnings */
	(void) len;
	(void) buf;
	DBG_8195A("Ameba-D2 not support efuse_otp_chk function!\n");
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

