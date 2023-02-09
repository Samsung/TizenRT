/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

const CHIPINFO_TypeDef Chip_Info[] = {
	//subnum    pkgnum      bdnumer   psram type
	{0,            2,       1010,     PSRAM_DEVICE_CLK_166 | PSRAM_VENDOR_WB | PSRAM_SIZE_32Mb | PSRAM_PAGE128   }, //QFN48
	{0,            3,       1011,     PSRAM_DEVICE_CLK_250 | PSRAM_VENDOR_WB | PSRAM_SIZE_256Mb | PSRAM_PAGE1024 }, //QFN68
	{0,            1,       1012,     PSRAM_VENDOR_NONE                                                          }, //QFN48
	{0,            0,       1014,     PSRAM_DEVICE_CLK_NotClear | PSRAM_VENDOR_NotClear                          }, //QFN144 debug package
	{1,            2,       1015,     PSRAM_DEVICE_CLK_166 | PSRAM_VENDOR_WB | PSRAM_SIZE_32Mb | PSRAM_PAGE128   }, //QFN48
	{1,            1,       1016,     PSRAM_VENDOR_NONE                                                          }, //QFN48
	{0,            4,       1019,     PSRAM_DEVICE_CLK_250 | PSRAM_VENDOR_WB | PSRAM_SIZE_128Mb | PSRAM_PAGE2048 }, //QFN68
	{0,            5,       1022,     PSRAM_VENDOR_NONE                                                          }, //QFN48

	{0xFF,         0xFF,    0xFFFF,   PSRAM_DEVICE_CLK_NotClear | PSRAM_VENDOR_NotClear                           }, //debug package
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

	OTP_Read8(BDnumAddr, &OTPinfo);

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
  * @retval psram type
  * 01 for APM 32Mb/02 for APM 64Mb/03 for APM 128Mb/04 for APM 256Mb/05 for APM 512Mb
  * 11 for WB 32Mb/12 for WB 64Mb/13 for WB 128Mb/14 for WB 256Mb/15 for WB 512Mb
  */
static u8 ChipInfo_CustomerSelect(void)
{
	static u8 memoryinfo = 0xFF;
	u8 MemoryInfoBuf[4] = {0};

	RRAM_TypeDef *rram = RRAM_DEV;

	if ((memoryinfo == 0xFF) && (rram->PSRAM_TYPE == 0)) {//in KM4 bootloader
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
		rram->PSRAM_TYPE = memoryinfo;

		LOGUART_ClearRxFifo(LOGUART_DEV);
		LOGUART_INTConfig(LOGUART_DEV, LOGUART_BIT_ERBI, ENABLE);
	} else {
		if (memoryinfo == 0xFF) { //in IMG2
			memoryinfo = rram->PSRAM_TYPE;
		}
	}

	return memoryinfo;
}

u32 ChipInfo_PSRAMType(void)
{
	u8 i = 0;
	u16 BDnum = ChipInfo_BDNum();
	u32 memoryinfo = 0;
#if 0 //customer sele todo
#endif
	/* debug package */
	if ((BDnum == 0xFFFF) || (BDnum == 1014)) {
		memoryinfo = ChipInfo_CustomerSelect();

	} else {
		while (Chip_Info[i].bd_num != 0xFFFF) {
			if (Chip_Info[i].bd_num == BDnum) {
				memoryinfo = (u32)(Chip_Info[i].psram_type);
			}

			i++;
		}
	}

	return memoryinfo;
}

/**
  * @brief Get chip version number from OTP.
  * @return Chip version number, which starts from 1.
  */
u8 ChipInfo_VerNum(void)
{
	u32 ChipVerAddr = 0x7F0;
	u8 chip_ver;

	OTP_Read8(ChipVerAddr, &chip_ver);

	return (u8)(chip_ver & 0x1F);
}

/**
  * @brief get package num
  * @retval package information
  */
u8 EFUSE_GetPackageInfo(void)
{

	u8 package_num;

	OTP_Read8(OTP_CHIPINFO, &package_num);
	package_num = OTP_GET_PACKAGE_NUM(package_num);

	return package_num;
}

/**
  * @brief get sub num
  * @retval sub num for same packageinfo
  */
u8 EFUSE_GetBDNum(void)
{

	u8 BD_num;

	OTP_Read8(OTP_CHIPINFO, &BD_num);

	BD_num = OTP_GET_INTERNAL_BDNUM(BD_num);

	return BD_num;
}

/**
  * @brief get chip version
  * @retval chip version, counting from 1(A), 2(B) ...
  */
u8 EFUSE_GetChipVersion(void)
{

	u8 chip_version;

	OTP_Read8(OTP_CHIPVER, &chip_version);
	chip_version = OTP_GET_CHIP_VERSION(chip_version);

	return chip_version;
}

/**
  * @brief get chip ES flag
  * @retval 0: MP Chip, 1: Engineer Sample
  */
u8 EFUSE_GetESFlag(void)
{

	u8 flag;

	OTP_Read8(OTP_CHIPVER, &flag);
	flag = OTP_GET_ES_FLAG(flag);

	return flag;
}

/**
  * @brief get chip UUID & LOT_NUM
  */
void EFUSE_GetUUID(u32 *UUID)
{

	u8 ID[4];
	u8 LOT_num[4];
	int index;

	for (index = 0; index < 4; index ++) {
		OTP_Read8(OTP_UUID + index, &ID[index]);
	}

	for (index = 0; index < 4; index ++) {
		OTP_Read8(OTP_LOT_NUM + index, &LOT_num[index]);
	}

	UUID[0] = (ID[3] << 24) | (ID[2] << 16) | (ID[1] << 8) | ID[0];
	UUID[1] = (LOT_num[3] << 24) | (LOT_num[2] << 16) | (LOT_num[1] << 8) | LOT_num[0];

}
