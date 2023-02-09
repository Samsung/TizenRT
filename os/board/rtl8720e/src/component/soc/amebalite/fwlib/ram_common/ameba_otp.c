/**
  ******************************************************************************
  * @file    ameba_otp_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the OTP firmware
  *          library.
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
  * @brief  OTP power cut Enable or Disable.
  * @param  Newstatus :specifies the target power cut status
  * @retval None
  */
static void OTP_PowerCmd(BOOL Enable)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PWC);

	if (Enable) {
		Temp |= AON_BIT_PWC_AON_OTP;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_PWC, Temp);
	} else {
		Temp &= ~(AON_BIT_PWC_AON_OTP);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_PWC, Temp);
	}

}

/**
  * @brief  get OTP power cut status.
  * @param  None
  * @retval specifies the target power cut status
  */
static BOOL OTP_GetPowerState(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_PWC);

	if (Temp & AON_BIT_PWC_AON_OTP) {
		return ENABLE;
	} else {
		return DISABLE;
	}
}

/**
  * @brief  enable or disable access to OTP.
  * @param  specifies the target access permision status
  * @retval None
  */
static void OTP_AccessCmd(BOOL Enable)
{
	OTPC_TypeDef *OTPC  = OTPC_DEV;

	if (Enable) {
		OTPC->OTPC_OTP_CTRL |= OTPC_EF_PG_PWD(OTP_ACCESS_PWD);
	} else {
		OTPC->OTPC_OTP_CTRL &= ~OTPC_MASK_EF_PG_PWD;
	}
}

/**
  * @brief  OTP power switch between read and write.
  * @param  bWrite: write or read
  *          This parameter can be  one of the following values:
  *            @arg 1: write
  *            @arg 0: read
  * @param  PwrState: TRUE/FALSE.
  */
static void OTPPowerSwitch(BOOL BWrite, BOOL PwrState)
{
	if (PwrState == ENABLE) {
		if (OTP_GetPowerState() == DISABLE) {
			OTP_PowerCmd(ENABLE);
		}
	}

	/* otp access protect */
	OTP_AccessCmd(BWrite);
}

/**
  * @brief  Get the CRC of security area 0x200~0x36F.
  * @param None
  * @retval CRC value:
  */
u32 OTPGetCRC(void)
{
	u32 TmpIdx = 0;
	u32 crc;
	OTPC_TypeDef *OTPC  = OTPC_DEV;

	OTPC->OTPC_OTP_CRC_RESULT = OTPC_BIT_CRC_TRIGGER_EN;

	while ((!(OTPC->OTPC_OTP_ALD & OTPC_BIT_LOAD_DONE)) && (TmpIdx < OTP_POLL_TIMES)) {
		DelayUs(1);
		TmpIdx++;
	}

	crc = OTPC_GET_CRC_DATA_ACT(OTPC->OTPC_OTP_CRC_DATA);

	OTPC->OTPC_OTP_CRC_RESULT = ~OTPC_BIT_CRC_TRIGGER_EN;

	return crc;

}

/**
  * @brief  physical map read one byte
  * @param  Addr: otp physical address
  * @param  Data: one byte data buffer for otp data
  * @retval status value:
  *          - _SUCCESS: read ok
  *          - _FAIL: read fail
  */
NON_DRAM_TEXT_SECTION
u32 OTP_Read8(u32 Addr, u8 *Data)
{
	u32 TmpIdx = 0;
	u32 bResult = _FAIL;
	volatile u32 *AccessReg = NULL;
	OTPC_TypeDef *OTPC  = OTPC_DEV;

	if (Addr >= OTP_REAL_CONTENT_LEN) {
		*Data = 0xff;
		return bResult;
	}

	if (TrustZone_IsSecure()) {
		OTPC = OTPC_DEV_S;
		AccessReg = &(OTPC->OTPC_OTP_AS_SEC);//Secure
	} else {
		AccessReg = &(OTPC->OTPC_OTP_AS);//No Secure
	}

	OTPPowerSwitch(DISABLE, ENABLE);

	*AccessReg = OTPC_EF_ADDR_NS(Addr);

	/* 10~20us is needed */
	while ((!(OTPC_BIT_EF_RD_WR_NS & (*AccessReg))) && (TmpIdx < OTP_POLL_TIMES)) {
		DelayUs(5);
		TmpIdx++;
	}

	if (TmpIdx < OTP_POLL_TIMES) {
		*Data = (u8)((*AccessReg) & OTPC_MASK_EF_DATA_NS);
		bResult = _SUCCESS;
	} else {
		*Data = 0xff;
		bResult = _FAIL;
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP_Read8 Fail %x \n", Addr);
	}

	OTPPowerSwitch(DISABLE, DISABLE);

	return bResult;
}

/**
  * @brief  physical map read one byte
  * @param  Addr: otp physical address
  * @param  Data: one byte data buffer for otp data
  * @retval status value:
  *          - _SUCCESS: read ok
  *          - _FAIL: read fail
  */
static BOOL OTP_ProgramMarginRead8(u32 Addr, u8 *Data)
{
	u32 TmpIdx = 0;
	u32 bResult = _FAIL;
	volatile u32 *AccessReg = NULL;
	OTPC_TypeDef *OTPC  = OTPC_DEV;

	if (Addr >= OTP_REAL_CONTENT_LEN) {
		*Data = 0xff;
		return bResult;
	}

	if (TrustZone_IsSecure()) {
		OTPC = OTPC_DEV_S;
		AccessReg = &(OTPC->OTPC_OTP_AS_SEC);//Secure
	} else {
		AccessReg = &(OTPC->OTPC_OTP_AS);//No Secure
	}

	OTPPowerSwitch(DISABLE, ENABLE);

	*AccessReg = OTPC_EF_MODE_SEL_NS(OTP_PGR_MODE) | OTPC_EF_ADDR_NS(Addr);

	/* 10~20us is needed */
	while ((!(OTPC_BIT_EF_RD_WR_NS & (*AccessReg))) && (TmpIdx < OTP_POLL_TIMES)) {
		DelayUs(5);
		TmpIdx++;
	}

	if (TmpIdx < OTP_POLL_TIMES) {
		*Data = (u8)((*AccessReg) & OTPC_MASK_EF_DATA_NS);
		bResult = _SUCCESS;
	} else {
		*Data = 0xff;
		bResult = _FAIL;
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP_Read8 Fail %x \n", Addr);
	}

	OTPPowerSwitch(DISABLE,  DISABLE);

	return bResult;
}


/**
  * @brief  physical map write one byte
  * @param  Addr: otp physical address
  * @param  Data: one byte data to write
  * @retval status value:
  *          - _SUCCESS: write ok
  *          - _FAIL: write fail
  */
static u32 _OTP_Write8(u32 Addr, u8 Data)
{
	u32 TmpIdx = 0;
	u32 bResult = _FAIL;
	volatile u32 *AccessReg = NULL;
	OTPC_TypeDef *OTPC  = OTPC_DEV;

	if (Addr >= OTP_REAL_CONTENT_LEN) {
		return bResult;
	}

	if (Data == 0xFF) {
		return _SUCCESS;
	}

	if (TrustZone_IsSecure()) {
		OTPC = OTPC_DEV_S;
		AccessReg = &(OTPC->OTPC_OTP_AS_SEC);//Secure
	} else {
		AccessReg = &(OTPC->OTPC_OTP_AS);//No Secure
	}


	OTPPowerSwitch(ENABLE, ENABLE);

	*AccessReg = OTPC_BIT_EF_RD_WR_NS | OTPC_EF_ADDR_NS(Addr) | Data;

	while ((OTPC_BIT_EF_RD_WR_NS & (*AccessReg)) && (TmpIdx < OTP_POLL_TIMES)) {
		DelayUs(5);
		TmpIdx++;
	}

	if (TmpIdx < OTP_POLL_TIMES) {
		bResult = _SUCCESS;
	} else {
		bResult = _FAIL;
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP_Write8 Fail %x \n", Addr);
	}

	OTPPowerSwitch(DISABLE, DISABLE);

	return bResult;
}


/**
  * @brief  physical map write one byte
  * @param  Addr: otp physical address
  * @param  Data: one byte data to write
  * @retval status value:
  *          - _SUCCESS: write ok
  *          - _FAIL: write fail
  */
u32 OTP_Write8(u32 Addr, u8 Data)
{
	u8 Temp;
	u8 Target;
	u8 retry = 0;

	Target = Data;

	if (OTP_ProgramMarginRead8(Addr, &Temp) == _FAIL) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "PMR Read error!\n");
		goto exit;
	}

retry:

	/*do not need program bits include originally do not need program
		(bits equals 1 in data) and already prgoramed bits(bits euqals 0 in Temp) */
	Data |= ~Temp;

	/*program*/
	if (_OTP_Write8(Addr, Data) == _FAIL) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP program error!\n");
		goto exit;
	}

	/*Read after program*/
	if (OTP_ProgramMarginRead8(Addr, &Temp) == _FAIL)  {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "PMR2 Read error!\n");
		goto exit;
	}

	/*program do not get desired value,the OTP can be programmed at most 3 times
		here only try once.*/
	if (Temp != Target) {
		if ((retry++) < 1) {
			goto retry;
		} else {
			goto exit;
		}
	}

	return _SUCCESS;

exit:
	return _FAIL;

}


/**
  * @brief  physical map write four byte
  * @param  Addr: otp physical address
  * @param  Data: four byte data to Read
  * @retval status value:
  *          - _SUCCESS: write ok
  *          - _FAIL: write fail
  */
static u32 OTP_Read32(u32 Addr, u32 *Data)
{
	u8 Temp;
	u8 i;

	*Data = 0;

	for (i = 0; i < 4; i++) {
		if (OTP_Read8(Addr++, &Temp) == _FAIL) {
			goto exit;
		}

		*Data |= (((u32)Temp) << (8 * i));
	}

	return _SUCCESS;
exit:
	return _FAIL;
}


/**
  * @brief  PG one logical map OTP packet in Byte format
  * @param  offset: offsetlogical addr
  * @param  Contant: packet data
  * @retval status value:
  *          - _SUCCESS: write ok
  *          - _FAIL: write fail
  */
static u32 OTP_PG_Packet_Byte(u16 offset, u8 Contant)
{
	u32 Idx = 0;
	u32 OTPData;

	/* Make sure the offset is correct*/
	if (offset > OTP_LMAP_LEN) {
		DBG_PRINTF(MODULE_OTP, LEVEL_WARN, "Make sure OTP logical area  :%x  defined\n", offset);
	}

	DBG_PRINTF(MODULE_OTP, LEVEL_INFO, "OTP_PG_Packet Byte  [%x] %x \n", offset, Contant);

	//count the physical written num of word
	while (Idx < LOGICAL_MAP_SECTION_LEN) {

		OTP_Read32(Idx, &OTPData);

		if (OTPData == 0xFFFFFFFF) {
			break;
		}

		Idx += 4;

		if (OTP_GET_LTYP(OTPData) == OTP_LTYP2) {
			Idx += OTP_GET_LTYP2_LEN(OTPData);
		}

		if (OTP_GET_LTYP(OTPData) == OTP_LTYP3) {
			Idx += 4;
		}
	}

	if (Idx  > LOGICAL_MAP_SECTION_LEN) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP_PG_Packet no enough space %x \n");
		return _FAIL;
	}

	OTP_Write8(Idx++, offset  & 0xFF); //header[7:0]
	OTP_Write8(Idx++, (OTP_LBASE_EFUSE << 4) | ((offset >> 8) & 0x0F)); //header[15:8]
	OTP_Write8(Idx++, Contant);
	OTP_Write8(Idx++, ((OTP_LTYP1 << 4) | 0x0F)); //header[31:24]

	return _SUCCESS;
}


/**
  * @brief  PG one logical map OTP packet in DWord format
  * @param  offset: offset logical addr
  * @param  len: the size will be wirte,should less than 16
  * @param  pContant: packet data
  * @retval status value:
  *          - _SUCCESS: write ok
  *          - _FAIL: write fail
  */
static u32 OTP_PG_Packet_Word(u16 offset, u8 len, u8 *pContant)
{
	u32 Idx = 0;
	u32 OTPData;
	u32  IdxTemp;

	/* 4byte align and size should less than 16bytes*/
	if ((len > OTP_LPGPKT_SIZE)  || ((len & 0x03) != 0)) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP_PG_Packet_Word size error :%x len:%x \n", offset, len);
		return _FAIL;
	}

	/* Make sure the offset is correct*/
	if (offset > OTP_LMAP_LEN) {
		DBG_PRINTF(MODULE_OTP, LEVEL_WARN, "Make sure OTP logical area  :%x  defined\n", offset);
	}

	for (IdxTemp = 0; IdxTemp < len; IdxTemp++) {
		DBG_PRINTF(MODULE_OTP, LEVEL_INFO, "OTP_PG_Packet [%x] %x \n", IdxTemp, *(pContant + IdxTemp));
	}

	//count the physical written num of word
	while (Idx < LOGICAL_MAP_SECTION_LEN) {

		OTP_Read32(Idx, &OTPData);

		if (OTPData == 0xFFFFFFFF) {
			break;
		}

		Idx += 4;

		if (OTP_GET_LTYP(OTPData) == OTP_LTYP2) {
			Idx += OTP_GET_LTYP2_LEN(OTPData);
		}

		if (OTP_GET_LTYP(OTPData) == OTP_LTYP3) {
			Idx += 4;
		}
	}


	if (Idx + len > LOGICAL_MAP_SECTION_LEN) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "OTP_PG_Packet no enough space %x \n", Idx);
		return _FAIL;
	}

	OTP_Write8(Idx++, offset & 0xFF); //header[7:0]
	OTP_Write8(Idx++, (OTP_LBASE_EFUSE << 4) | ((offset >> 8) & 0x0F)); //header[15:8]
	Idx++; //header[23:16] reserved
	OTP_Write8(Idx++, ((OTP_LTYP2 << 4) | OTP_SET_LTYP2_LEN(len))); //header[31:24]

	for (IdxTemp = 0; IdxTemp < len; IdxTemp++) {
		OTP_Write8(Idx++, pContant[IdxTemp]);
	}

	return _SUCCESS;
}


/**
  * @brief  read OTP logical map
  * @param  pbuf: OTP_LMAP_LEN len buffer used for OTP Logical map
  * @retval status value:
  *          - _SUCCESS: read ok
  *          - _FAIL: read fail
  */
u32 OTP_LogicalMap_Read(u8 *pbuf, u32 addr, u32 len)
{
	u32 OTP_Addr = 0;
	u32 offset;
	u32 OTPData;
	u8 data, plen, type;

	if ((addr + len) > OTP_LMAP_LEN) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "LogicalMap Read error %x+%x  exceed limit\n", addr, len);
		return _FAIL;
	}

	/*  0xff will be OTP default value instead of 0x00. */
	_memset(pbuf, 0xFF, len);

	while (OTP_Addr < LOGICAL_MAP_SECTION_LEN) {
		OTP_Read32(OTP_Addr, &OTPData);

		if (OTPData == 0xFFFFFFFF) {/* not write */
			DBG_PRINTF(MODULE_OTP, LEVEL_INFO, "OTP_LogicalMap_Read: data end at address=%x\n", OTP_Addr);
			break;
		}

		OTP_Addr += 4;

		switch (OTP_GET_LTYP(OTPData)) {
		case OTP_LTYP0:
			/*empty entry shift to next entry*/
			break;

		case OTP_LTYP1:
			if (OTP_GET_LTYP1_BASE(OTPData) == OTP_LBASE_EFUSE) {
				offset = OTP_GET_LTYP1_OFFSET(OTPData);
				data = OTP_GET_LTYP1_DATA(OTPData);

				if ((offset >= addr) && (offset < addr + len)) {
					pbuf[offset - addr] = data;
				}
			}

			break;

		case OTP_LTYP2:
			plen = OTP_GET_LTYP2_LEN(OTPData);
			offset = OTP_GET_LTYP2_OFFSET(OTPData);
			type = OTP_GET_LTYP2_BASE(OTPData);

			if (type == OTP_LBASE_EFUSE) {
				while (plen-- > 0) {
					OTP_Read8(OTP_Addr++, &data);

					if ((offset >= addr) && (offset < addr + len)) {
						pbuf[offset++ -addr] = data;
					}
				}
			} else {
				OTP_Addr += plen;
			}

			break;

		default :
			break;

		}

		if ((OTP_Addr & 0x03) != 0) {
			DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "alignment error %x %x \n", OTP_Addr, OTPData);
		}

	}

	return _SUCCESS;
}


/**
  * @brief  write OTP logical address
  * @param  addr: logical addr
  * @param  cnts: byte number, should be even
  * @param  data: data buffer to be write
  * @retval status value:
  *          - _SUCCESS: write ok
  *          - _FAIL: write fail
  */
u32 OTP_LogicalMap_Write(u32 addr, u32 cnts, u8 *data)
{
	u32	base, offset;
	u32 bytemap = 0, byte_change = 0, wordmap = 0, word_change = 0;
	u8 word_start = 0, word_end = 0, word_offset;
	u8 *pcontant = data;
	u32	i, j;
	u32	ret = _SUCCESS;
	u8	newdata[OTP_LPGPKT_SIZE];
	s32 left_cnts = cnts;
	u8 write_pkt;

	if ((addr + cnts) > OTP_LMAP_LEN) {
		DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "LogicalMap Write error %x+%x  exceed limit\n", addr, cnts);
		return _FAIL;
	}

	/* 4bytes one section */
	base = addr & 0xFFFFFFF0;
	offset = addr & 0x0000000F;

	while (left_cnts > 0) {

		write_pkt = left_cnts + offset > OTP_LPGPKT_SIZE ? OTP_LPGPKT_SIZE : left_cnts + offset;
		ret = OTP_LogicalMap_Read(newdata, base, OTP_LPGPKT_SIZE);

		if (ret == _FAIL) {
			DBG_PRINTF(MODULE_OTP, LEVEL_ERROR, "LogicalMap Read error when write @ %x \n", base);
			return _FAIL;
		}

		/*compare and record changed data*/
		for (i = offset, j = 0; i < write_pkt; i++, j++) {
			if (pcontant[j] != newdata[i]) {
				newdata[i] = pcontant[j];
				bytemap |= BIT(i);
				wordmap |= BIT(i >> 2);
				byte_change++;
				DBG_PRINTF(MODULE_OTP, LEVEL_INFO, "newdata[%x]= %x\n", i, newdata[i]);
			}
		}

		/*no changes in the PKT,go and try next*/
		if (byte_change == 0) {
			goto next;
		}


		/*find start word and end word*/
		word_start = OTP_LPGPKT_SIZE;
		word_end = 0;

		for (i = offset / 4 ; i < OTP_LPGPKT_SIZE / 4 ; i++) {
			if ((wordmap >> i) & BIT0) {
				/*choose a small one as start word*/
				word_start = (word_start < i) ? word_start : i;
				/*choose a big one as end word*/
				word_end = (word_end > i) ? word_end : i;
			}
		}


		/*calculate word change*/
		word_change = word_end - word_start + 1;
		word_offset = word_start * 4;

		/*choose use witch type to write the logical efuse*/
		if ((word_change + 1) < byte_change) {
			ret = OTP_PG_Packet_Word(base + word_offset, word_change * 4, &newdata[word_offset]);
		} else {
			for (i = 0; i < OTP_LPGPKT_SIZE; i++) {
				if (bytemap & BIT(i)) {
					ret = OTP_PG_Packet_Byte(base + i, newdata[i]);
				}
			}
		}

next:
		left_cnts -= (OTP_LPGPKT_SIZE - offset);
		pcontant += (OTP_LPGPKT_SIZE - offset);
		base += OTP_LPGPKT_SIZE;
		offset = 0;

		bytemap = 0;
		wordmap = 0;
		byte_change = 0;

		DBG_PRINTF(MODULE_OTP, LEVEL_INFO, "next write cycle Base %x cnts %x \n", base, left_cnts);
	}

	return ret;
}


/**
  * @brief  Get OTP logical address remain length.
  * @param  none
  * @retval OTP logical address remain length
  */
static u32 OTP_RemainLength(void)
{
	u32 Idx = 0;
	u32 OTPData;

	//find start addr
	while (Idx < LOGICAL_MAP_SECTION_LEN) {

		OTP_Read32(Idx, &OTPData);

		if (OTPData == 0xFFFFFFFF) {
			break;
		}

		Idx += 4;

		if (OTP_GET_LTYP(OTPData) == OTP_LTYP2) {
			Idx += OTP_GET_LTYP2_LEN(OTPData);
		}

		if (OTP_GET_LTYP(OTPData) == OTP_LTYP3) {
			Idx += 4;
		}
	}

	return (LOGICAL_MAP_SECTION_LEN - Idx);
}

/**
  * @brief  define alias name to compatible with old API
  */
u32 EFUSERead8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 *Data, UNUSED_WARN_DIS u8 L25OutVoltage)
{
	return  OTP_Read8(Addr, Data);
}

/**
  * @brief  define alias name to compatible with old API
  */
u32 EFUSEWrite8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 Data, UNUSED_WARN_DIS u8 L25OutVoltage)
{
	return OTP_Write8(Addr, Data);
}

/**
  * @brief  define alias name to compatible with old API
  */
NON_DRAM_TEXT_SECTION
u32 EFUSE_PMAP_READ8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 *Data, UNUSED_WARN_DIS u8 L25OutVoltage)
{
	return OTP_Read8(Addr, Data);
}

/**
  * @brief  define alias name to compatible with old API
  */
u32 EFUSE_PMAP_WRITE8(UNUSED_WARN_DIS u32 CtrlSetting, u32 Addr, u8 Data, UNUSED_WARN_DIS u8 L25OutVoltage)
{
	return OTP_Write8(Addr, Data);
}

/**
  * @brief  define alias name to compatible with old API
  */
u32 EFUSE_LMAP_READ(u8 *pbuf)
{
	return OTP_LogicalMap_Read(pbuf, 0, OTP_LMAP_LEN);
}

/**
  * @brief  define alias name to compatible with old API
  */
u32 EFUSE_LMAP_WRITE(u32 addr, u32 cnts, u8 *data)
{
	if (((cnts % 4) != 0) && (cnts != 1)) {
		DBG_8195A("Warning Logical write should 4 bytes alignment\n");
		DBG_8195A("Writing....\n");
	}
	return OTP_LogicalMap_Write(addr, cnts, data);
}


/**
  * @brief  define alias name to compatible with old API
  */
u32 EFUSE_RemainLength(void)
{
	return OTP_RemainLength();
}

u32 EFUSE_GetCRC(void)
{

	return OTPGetCRC();
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
