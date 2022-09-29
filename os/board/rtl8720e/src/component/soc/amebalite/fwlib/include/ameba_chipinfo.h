#ifndef _AMEBAD2_CHIPINFO_H_
#define _AMEBAD2_CHIPINFO_H_

/** @defgroup PSRAM_Size_define
  * @{
  */
enum PSRAM_type {
	PSRAM_A_32Mb = 0x01,
	PSRAM_A_64Mb = 0x02,
	PSRAM_A_128Mb = 0x03,
	PSRAM_A_256Mb = 0x04,
	PSRAM_A_512Mb = 0x05,

	PSRAM_B_32Mb = 0x11,
	PSRAM_B_64Mb = 0x12,
	PSRAM_B_128Mb = 0x13,
	PSRAM_B_256Mb = 0x14,
	PSRAM_B_512Mb = 0x15,
	PSRAM_None = 0xFF,
};
/**
  * @}
  */


/** @defgroup CHIPINFO_TypeDef
  * @{
  */
typedef struct {
	u8 sub_num;
	u8 package_num;
	u16 bd_num;
	u8 psram_type;
} CHIPINFO_TypeDef;
/**
  * @}
  */

#define	CHIPINFO_GET_SUBNUM(x)	((u8)(((x >> 5) & 0x00000007)))
#define	CHIPINFO_GET_PKGNUM(x)	((u8)(((x >> 0) & 0x0000001F)))


/* Exported functions --------------------------------------------------------*/
/** @defgroup HWINFO_Exported_Functions HWINFO Exported Functions
  * @{
  */
_LONG_CALL_ u16 ChipInfo_BDNum(void);
_LONG_CALL_ u8 ChipInfo_PSRAMType(void);
/**
  * @}
  */

#endif
