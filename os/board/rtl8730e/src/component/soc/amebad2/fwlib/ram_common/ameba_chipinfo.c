/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static const char *TAG = "CHIPINFO";
/**
* @brif Memory_Info maintains the chipinfo and corresponding memory information.
*/
const MemoryInfo_TypeDef Memory_Info[] = {
	/*subnum,	pkgnum,		chipinfo,		memory_type,	 		memory_size,		vendor */
	{0x7,		0x0,			0xE0,		DDR_Type_DDR3L,			DDR_Size_2G,		0},			/* 1006(QFN228) */
	{0x6,		0x0,			0xC0,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},/* 1006(QFN228) */
	{0x0,		0x1,			0x01,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},/* 1011(QFN144) */
	{0x0,		0x2,			0x02,		DDR_Type_DDR3L,			DDR_Size_2G,		0},			/* 1012(QFN144) */
	{0x0,		0x4,			0x04,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},/* 1013(QFN100) */
	{0x1,		0x1,			0x21,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},/* 1014(QFN144) */
	{0x1,		0x4,			0x24,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},/* 1015(QFN100) */
	{0x0,		0x3,			0x03,		DDR_Type_DDR2,			DDR_Size_1G,		0},			/* 1016(QFN144) */
	{0x0,		0x5,			0x05,		DDR_Type_DDR2,			DDR_Size_512M,		0},			/* 1018(QFN100) */
	{0x0,		0x6,			0x06,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},			/* 1022(QFN100) psram 64+Nor 64*/
	{0x0,		0x7,			0x07,		DDR_Type_DDR2,			DDR_Size_512M,		0},			/* 1023(QFN100) ddr2 512+Nor 256*/
	{0x0,		0x8,			0x08,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},			/* 1024(QFN144) psram 64+Nor 64*/
	{0x1,		0x8,			0x28,		Memory_Type_PSRAM,		PSRAM_Size_64M,		Vendor_PSRAM_A},			/* 1025(QFN144) psram 64+Nor 64*/
	{0x0,		0x9,			0x09,		DDR_Type_DDR3L,			DDR_Size_2G,		0},			/* 1026(QFN144) ddr3L 2G+nand 2G*/
	{0x1,		0x9,			0x29,		DDR_Type_DDR3L,			DDR_Size_2G,		0},			/* 1027(QFN144) ddr3L 2G+nand 2G*/
	{0x0,		0xA,			0x0A,		DDR_Type_DDR2,			DDR_Size_1G,		0},			/* 1028(QFN144) ddr2 1G+nand 1G*/
	{0x1,		0xA,			0x2A,		DDR_Type_DDR2,			DDR_Size_1G,		0},			/* 1029(QFN144) ddr2 1G+nand 1G*/
	{0x0,		0xB,			0x0B,		Memory_Type_PSRAM,		PSRAM_Size_64M, 	Vendor_PSRAM_A},			/* 1038(QFN100) psram 64Mb+nor 128Mb*/
	{0x0,		0xC,			0x0C,		Memory_Type_PSRAM,		PSRAM_Size_64M, 	Vendor_PSRAM_A},			/* 1037(QFN144) psram 64Mb+nor 64Mb*/
	{0x0,		0xD,			0x0D,		DDR_Type_DDR3L,			DDR_Size_2G, 		0},			/* 1039(QFN144) ddr3L 2G+nand 2G*/
	{0x0,		0xE,			0x0E,		DDR_Type_DDR2,			DDR_Size_1G, 		0},			/* 1040(QFN144) ddr2 1G+nand 1G*/

	{0xFF,		0xFF,		0xFF,		Memory_Type_None,		DDR_Size_None,		Vendor_None}, /* End */
};

/**
  * @brief chipenter endless loop for invalid chipinfo.
  */
static void ChipInfo_Invalid(void)
{
	while (1) {
		// let the program stuck here
		RTK_LOGE(TAG, "Invalid Chininfo! Check OTP\n");
		DelayMs(10000);
	}
}

/**
  * @brief Get chipinfo form OTP.
  * @retval chipinfo
  */
u8 ChipInfo_Get(void)
{
	u32 ChipinfoAddr = 0x7FF;
	static u8 Chipinfo = 0xFF;

	if (Chipinfo != 0xFF) {
		return Chipinfo;
	}

	if (RRAM->CHIP_INFO != 0) { /* RRAM->CHIP_INFO is init in Bootloader */
		Chipinfo = RRAM->CHIP_INFO;
		return Chipinfo;
	}

	OTP_Read8(ChipinfoAddr, &Chipinfo);

	if (Chipinfo == 0xFF) {
		while (1) {
			// let the program stuck here
			RTK_LOGW(TAG, "Please program chipinfo in OTP !\n");
			DelayMs(10000);
		}
	}

	RRAM->CHIP_INFO = Chipinfo;

	return Chipinfo;
}

/**
  * @brief  Get memory type.
  * @retval Memory_Type: Memory_Type_PSRAM/Memory_Type_DDR
  */
u8 ChipInfo_MemoryType(void)
{
	u32 i = 0;
	u8 Chipinfo;

	/* check environment */
	if ((SYSCFG_CHIPType_Get() == CHIP_TYPE_PALADIUM) || (SYSCFG_CHIPType_Get() == CHIP_TYPE_RTLSIM)) {
		return Memory_Type_PSRAM;
	}

	Chipinfo = ChipInfo_Get();

	while (Memory_Info[i].chip_info != 0xFF) {
		if (Memory_Info[i].chip_info == Chipinfo) {
			if (Memory_Info[i].memory_type == Memory_Type_PSRAM) {
				return Memory_Type_PSRAM;
			} else {
				return Memory_Type_DDR;
			}
		}

		i++;
	}

	/* wrong chipinfo, enter endless loop */
	ChipInfo_Invalid();

	/* avoid warning, never reach here */
	return 0;
}

/**
  * @brief  Check DDR type.
  * @retval DDR_Type: DDR_Type_DDR2/DDR_Type_DDR3/DDR_Type_DDR3L/DDR_Type_LPDDR1/DDR_Type_None
  */
u8 ChipInfo_DDRType(void)
{
	u32 i = 0;
	u8 Chipinfo = ChipInfo_Get();

	if (ChipInfo_MemoryType() == Memory_Type_DDR) {
		while (Memory_Info[i].chip_info != 0xFF) {
			if (Memory_Info[i].chip_info == Chipinfo) {
				return Memory_Info[i].memory_type;
			}

			i++;
		}
	} else {
		/* 0xFF for psram case, can't change to Memory_Type_PSRAM*/
		return DDR_Type_None;
	}

	/* avoid warning, never reach here */
	return 0;
}

/**
  * @brief  Check Memory size.
  * @retval
  */
u8 ChipInfo_MemorySize(void)
{
	u32 i = 0;
	u8 Chipinfo = ChipInfo_Get();

	while (Memory_Info[i].chip_info != 0xFF) {
		if (Memory_Info[i].chip_info == Chipinfo) {
			return Memory_Info[i].memory_size;
		}

		i++;
	}

	/* for wrong chipinf, enter endless loop */
	ChipInfo_Invalid();

	/* avoid warning, never reach here */
	return 0;
}

/**
  * @brief  Check memory vendor.
  * @retval Memory_Vendor
  */
u8 ChipInfo_MemoryVendor(void)
{
	u32 i = 0;
	u8 Chipinfo;

	/* check environment */
	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_RTLSIM) {
		return Vendor_PSRAM_A;
	} else if (SYSCFG_CHIPType_Get() == CHIP_TYPE_PALADIUM) {
		return Vendor_PSRAM_B;
	}

	Chipinfo = ChipInfo_Get();

	while (Memory_Info[i].chip_info != 0xFF) {
		if (Memory_Info[i].chip_info == Chipinfo) {
			return Memory_Info[i].memory_vendor;
		}

		i++;
	}

	/* wrong chipinfo, enter endless loop */
	ChipInfo_Invalid();
	/* avoid warning, never reach here */
	return 0;
}

/**
  * @brief  Check Chip Package.
  * @retval Memory_Vendor
  */
u8 ChipInfo_ChipPackage(void)
{
	u32 i = 0;
	u8 Chipinfo = ChipInfo_Get();
	u8 Package;

	while (Memory_Info[i].chip_info != 0xFF) {
		if (Memory_Info[i].chip_info == Chipinfo) {
			Package = Memory_Info[i].package_num;
			if (Package == 0) {
				return Package_Type_QFN228;
			} else if (Package > 0 && Package < 4) {
				return Package_Type_QFN144;
			} else if (Package >= 4 && Package <= 5) {
				return Package_Type_QFN100;
			} else {
				return Package_Type_Unknown;
			}
		}

		i++;
	}

	/* wrong chipinfo, enter endless loop */
	ChipInfo_Invalid();
	/* avoid warning, never reach here */
	return 0xFF;


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
  * @retval chip version, counting from 0(A), 1(B) ...
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

/**
  * @brief get chip CPU & WIFI
  * @note retval need to be parsed
  */
u8 EFUSE_Get_Info(u32 FuncID)
{
	u8 remark_info[2];
	u8 info;

	OTP_Read8(OTP_INFO, &remark_info[0]);
	OTP_Read8(OTP_INFO + 1, &remark_info[1]);

	switch (FuncID) {
	case OTP_CpuCoreNum:
		info = OTP_GET_CPU_CORENUM(remark_info[0]);
		break;
	case OTP_CpuClkRate:
		info = OTP_GET_CPU_CLK(remark_info[0]);
		break;
	case OTP_WIFI5GSupport:
		info = OTP_GET_WIFI5G_SUPPORT(remark_info[1]);
		break;
	case OTP_802Protocol:
		info = OTP_GET_802PROTOCOL(remark_info[1]);
		break;
	case OTP_MemorySizeOrigin:
		info = OTP_GET_MEMORYSIZE_ORIGIN(remark_info[1]);
		break;
	case OTP_RSVD0:
		info = OTP_GET_RSVD0(remark_info[1]);
		break;
	default:
		info = _FAIL;
		break;
	}

	return info;
}

