#ifndef _AMEBAD2_CHIPINFO_H_
#define _AMEBAD2_CHIPINFO_H_

/** @defgroup Memory_Type_define
  * @{
  */
enum Memory_Type {
	Memory_Type_DDR = 0x01,
	Memory_Type_PSRAM = 0x02,
	Memory_Type_None = 0xFF,
};

/**
  * @}
  */

/** @defgroup Chip_Package_define
  * @{
  */
enum Package_Type {
	Package_Type_QFN228 = 0x0,
	Package_Type_QFN144 = 0x1,
	Package_Type_QFN100 = 0x2,
	Package_Type_Unknown = 0xFF,
};

/** @defgroup DDR_Type_define
  * @{
  */
enum DDR_Type {
	DDR_Type_DDR2 = 0x01,
	DDR_Type_DDR3 = 0x02,
	DDR_Type_DDR3L = 0x03,
	DDR_Type_LPDDR1 = 0x04,
	DDR_Type_None = 0xFF,
};
/**
  * @}
  */

/** @defgroup PSRAM_Size_define
  * @{
  */
enum PSRAM_Size {
	PSRAM_Size_32M = 0x01,
	PSRAM_Size_64M = 0x02,
	PSRAM_Size_128M = 0x03,
	PSRAM_Size_256M = 0x04,
	PSRAM_Size_None = 0xFF,
};
/**
  * @}
  */

/** @defgroup DDR_Size_define
  * @{
  */
enum DDR_Size {
	DDR_Size_256M = 0x01,
	DDR_Size_512M = 0x02,
	DDR_Size_1G = 0x03,
	DDR_Size_2G = 0x04,
	DDR_Size_None = 0xFF,
};
/**
  * @}
  */

/** @defgroup Memory_vendor_define
  * @{
  */
enum Memory_vendor {
	Vendor_PSRAM_A = 0x01,
	Vendor_PSRAM_B = 0x02,
	Vendor_None = 0xFF,
};
/**
  * @}
  */

/** @defgroup MemoryInfo_TypeDef
  * @{
  */
typedef struct {
	u8 sub_num;
	u8 package_num;
	u8 chip_info;
	u8 memory_type;
	u8 memory_size;
	u8 memory_vendor;
} MemoryInfo_TypeDef;
/**
  * @}
  */

#define OTP_CHIPVER								0x7F0
#define OTP_GET_CHIP_VERSION(x)					((u32)(((x >> 0) & 0x0000001F)))
#define OTP_GET_ES_FLAG(x)						((u32)(((x >> 5) & 0x00000001)))

#define OTP_CHIPINFO							0x7FF
#define OTP_GET_PACKAGE_NUM(x)					((u32)(((x >> 0) & 0x0000001F)))
#define OTP_GET_INTERNAL_BDNUM(x)				((u32)(((x >> 5) & 0x00000007)))

#define OTP_UUID								0x7F4
#define OTP_LOT_NUM								0x7F8
#define OTP_INFO								0x7FC

enum OTP_Info {
	OTP_CpuCoreNum = 0,
	OTP_CpuClkRate = 1,
	OTP_WIFI5GSupport = 2,
	OTP_802Protocol = 3,
	OTP_MemorySizeOrigin = 4,
	OTP_RSVD0 = 5,
};

#define OTP_GET_CPU_CORENUM(x)					((u32)(((x >> 0) & 0x0000000F)))
#define OTP_GET_CPU_CLK(x)						((u32)(((x >> 4) & 0x0000000F)))

#define OTP_GET_WIFI5G_SUPPORT(x)				((u32)(((x >> 0) & 0x00000003)))
#define OTP_GET_802PROTOCOL(x)					((u32)(((x >> 2) & 0x00000003)))
#define OTP_GET_MEMORYSIZE_ORIGIN(x)			((u32)(((x >> 4) & 0x00000003)))
#define OTP_GET_RSVD0(x)						((u32)(((x >> 6) & 0x00000003)))


/**
  * @}
  */


/** @defgroup EFUSE_Physical_Address_functions
  * @{
  */
_LONG_CALL_ u8 EFUSE_GetPackageInfo(void);
_LONG_CALL_ u8 EFUSE_GetBDNum(void);
_LONG_CALL_ u8 EFUSE_GetChipVersion(void);
_LONG_CALL_ u8 EFUSE_GetESFlag(void);
_LONG_CALL_ void EFUSE_GetUUID(u32 *UUID);

_LONG_CALL_ u8 ChipInfo_Get(void);
_LONG_CALL_ u8 ChipInfo_MemoryType(void);
_LONG_CALL_ u8 ChipInfo_DDRType(void);
_LONG_CALL_ u8 ChipInfo_MemorySize(void);
_LONG_CALL_ u8 ChipInfo_MemoryVendor(void);
_LONG_CALL_ u8 ChipInfo_ChipPackage(void);
_LONG_CALL_ u8 EFUSE_Get_Info(u32 FuncID);

/**
  * @}
  */
/* MANUAL_GEN_END */
#endif

