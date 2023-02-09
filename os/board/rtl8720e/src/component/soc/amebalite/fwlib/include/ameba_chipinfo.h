#ifndef _AMEBAD2_CHIPINFO_H_
#define _AMEBAD2_CHIPINFO_H_


/** @defgroup CHIPINFO_TypeDef
  * @{
  */
typedef struct {
	u8 sub_num;
	u8 package_num;
	u16 bd_num;
	u32 psram_type;
} CHIPINFO_TypeDef;
/**
  * @}
  */

#define	CHIPINFO_GET_SUBNUM(x)					((u8)(((x >> 5) & 0x00000007)))
#define	CHIPINFO_GET_PKGNUM(x)					((u8)(((x >> 0) & 0x0000001F)))

#define OTP_CHIPVER								0x7F0
#define OTP_GET_CHIP_VERSION(x)					((u32)(((x >> 0) & 0x0000001F)))
#define OTP_GET_ES_FLAG(x)						((u32)(((x >> 5) & 0x00000001)))

#define OTP_CHIPINFO							0x7FF
#define OTP_GET_PACKAGE_NUM(x)					((u32)(((x >> 0) & 0x0000001F)))
#define OTP_GET_INTERNAL_BDNUM(x)				((u32)(((x >> 5) & 0x00000007)))

#define OTP_UUID								0x7F4
#define OTP_LOT_NUM								0x7F8

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

_LONG_CALL_ u16 ChipInfo_BDNum(void);
_LONG_CALL_ u32 ChipInfo_PSRAMType(void);
_LONG_CALL_ u8 ChipInfo_VerNum(void);

/**
  * @}
  */
#endif
