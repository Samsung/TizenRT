#ifndef _AMEBAD2_SWRCALIB_H_
#define _AMEBAD2_SWRCALIB_H_

/**
  * @}
  */

/** @defgroup Swr_Calib_Info_define
  * @{
  */
enum Swr_Calib_Info {
	Swr_Calib_Nocalib = 0x01,
	Swr_Calib_Onecalib = 0x02,
	Swr_Calib_Twocalib = 0x03,
	Swr_Calib_None = 0xFF,
};

#define IS_SWR_CALIB_Info(Swr_Calib) (((Swr_Calib) == Swr_Calib_Twocalib) || \
								((Swr_Calib) == Swr_Calib_Onecalib) || \
								((Swr_Calib) == Swr_Calib_Nocalib))
/**
  * @}
  */

/** @defgroup Swr_Info_define
  * @{
  */
enum Swr_Type_Info {
	Swr_Core = 0x01,
	Swr_MEM_PWM = 0x02,
	Swr_MEM_PFM = 0x03,
	Swr_AUD_PWM = 0x04,
	Swr_AUD_PFM = 0x05,
	Swr_None = 0xFF,
};

#define IS_SWR_Type(SWR_TYPE) (((SWR_TYPE) == Swr_Core) || \
								((SWR_TYPE) == Swr_MEM_PWM) || \
								((SWR_TYPE) == Swr_MEM_PFM) || \
								((SWR_TYPE) == Swr_AUD_PWM) || \
								((SWR_TYPE) == Swr_AUD_PFM))
/**
  * @}
  */

/** @defgroup SWR_Calib_Value_define
  * @{
  */
#define SWR_Calib_Twocalib_4Bit_Value(x)                     ((u8)(((x) & 0xF0) >> 4))
#define SWR_Calib_Onecalib_4Bit_Value(x)                     ((u8)(((x) & 0x0F) >> 0))

#define SWR_Calib_Twocalib_2Bit_Value(x)                     ((u8)(((x) & 0xC) >> 2))
#define SWR_Calib_Onecalib_2Bit_Value(x)                     ((u8)(((x) & 0x3) >> 0))

#define Width_4Bit	4
#define Width_2Bit	2

#define IS_SWR_CALIB_BitWith(WIDTH) (((WIDTH) == Width_4Bit) || \
								((WIDTH) == Width_2Bit))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup VOL_SWR_OTP_Address_define
  * @{
  */
#define VOL_SWR_DCORE_09_PWM		0x7E0
#define VOL_SWR_DCORE_1V0_PWM		0x7E1
#define VOL_SWR_DCORE_09_PFM		0x7E2
#define VOL_SWR_DCORE_08_PFM		0x7E3
#define VOL_SWR_MEM_PWM		0x7E4
#define VOL_SWR_MEM_PFM		0x7E5
#define VOL_SWR_AUD_PWM		0x7E6
#define VOL_SWR_AUD_PFM		0x7E7
#define VREF_SEL_SWR_MEM		0x7E8
#define VREF_SEL_SWR_AUD		0x7E8
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SWR_Calib_Exported_Functions SWR_Calib Exported Functions
  * @{
  */
_LONG_CALL_ void SWR_Calib_DCore(void);
_LONG_CALL_ void SWR_Calib_MEM(void);
_LONG_CALL_ void SWR_Calib_AUD(void);
/**
  * @}
  */

#endif
