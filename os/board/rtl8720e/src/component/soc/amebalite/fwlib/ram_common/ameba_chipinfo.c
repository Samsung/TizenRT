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
* @brif CHIPINFO_TypeDef maintains the BD number and corresponding OTP information.
*/
const CHIPINFO_TypeDef Chip_Info[] = {
	//subnum	pkgnum		bdnumer		psram type
	{0,			2,			1010,		PSRAM_B_32Mb}, //QFN48
	{0,			3,			1011,		PSRAM_B_256Mb}, //QFN68
	{0,			1,			1012,		PSRAM_None}, //QFN48
	{0,			0,			1014,		PSRAM_None}, //QFN144
	{1,			2,			1015,		PSRAM_B_32Mb}, //QFN48
	{1,			1,			1016,		PSRAM_None}, //QFN48

	{0xFF,		0xFF,		0xFFFF,		PSRAM_None},
};

/**
  * @brief Get BD numer form OTP.
  * @retval BD number
  */
u16 ChipInfo_BDNum(void)
{
	u8 OTPinfo, subnum, pkgnum;
	u16 BDnum = 0;
	u32 BDnumAddr = 0x7FF;
	u32 i;

	EFUSE_PMAP_READ8(0, BDnumAddr, &OTPinfo, L25EOUTVOLTAGE);

	if (OTPinfo == 0xFF) {
		return 0xFFFF; //debug package
	}

	subnum = CHIPINFO_GET_SUBNUM(OTPinfo);
	pkgnum = CHIPINFO_GET_PKGNUM(OTPinfo);


	for (i = 0;;) {
		/*  Check if search to end */
		if (Chip_Info[i].bd_num == 0xFFFF) {
			DBG_8195A("Invalid BD number!\n");
			return 1014; //default 144 package
		}

		if ((Chip_Info[i].sub_num == subnum) && (Chip_Info[i].package_num == pkgnum)) {
			BDnum = Chip_Info[i].bd_num;
			break;
		}

		i++;
	}

	return BDnum;
}

/**
  * @brief Customer select memory information flow.
  * @retval TRUE or FALSE
  * TRUE: memory information is in flash system data
  * FLASE: memory information is in Chip_Info[]
  */
static void ChipInfo_CustomerSelect(void)
{
	u8 memoryinfo;
	u32 sysdata;
	u8 MemoryInfoBuf[4] = {0};


	/* check if memory info in system data is empty*/
	sysdata = HAL_READ32(SPI_FLASH_BASE + FLASH_SYSTEM_DATA_ADDR, 0x0);
	DCache_Invalidate((u32)(SPI_FLASH_BASE + FLASH_SYSTEM_DATA_ADDR), sizeof(MemoryInfoBuf));


	if (sysdata == 0xFFFFFFFF) {

		DBG_8195A("\nFor debug package. Please select psram type according to your chip\n");
		DBG_8195A("01 for APM 32Mb\n02 for APM 64Mb\n03 for APM 128Mb\n04 for APM 256Mb\n05 for APM 512Mb\n\n");
		DBG_8195A("11 for WB 32Mb\n12 for WB 64Mb\n13 for WB 128Mb\n14 for WB 256Mb\n15 for WB 512Mb\n\n");

		LOGUART_ClearRxFifo(LOGUART_DEV);
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, DISABLE);

		while (!(LOGUART_Readable()));
		MemoryInfoBuf[0] = LOGUART_GetChar(_FALSE);
		while (!(LOGUART_Readable()));
		MemoryInfoBuf[1] = LOGUART_GetChar(_FALSE);

		memoryinfo = (u8)(((MemoryInfoBuf[0] & 0x0F) << 4) | (MemoryInfoBuf[1] & 0x0F));

		LOGUART_ClearRxFifo(LOGUART_DEV);
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, ENABLE);

		FLASH_TxData(FLASH_SYSTEM_DATA_ADDR, 1, (u8 *)&memoryinfo);
	}
}

u8 ChipInfo_PSRAMType(void)
{
	u8 i = 0;
	u16 BDnum = ChipInfo_BDNum();
	u32 sysdata;
	u8 memoryinfo = 0;

	/* debug package */
	if ((BDnum == 0xFFFF) || (BDnum == 1014)) {
		ChipInfo_CustomerSelect();

		sysdata = HAL_READ32(SPI_FLASH_BASE + FLASH_SYSTEM_DATA_ADDR, 0x0);
		memoryinfo = (u8)(sysdata & 0xFF);

	} else {
		while (Chip_Info[i].bd_num != 0xFFFF) {
			if (Chip_Info[i].bd_num == BDnum) {
				memoryinfo = (u8)(Chip_Info[i].psram_type);
			}

			i++;
		}
	}

	return memoryinfo;
}
