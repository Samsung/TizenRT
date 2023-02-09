#ifndef _AMEBAD2_SWRCALIB_H_
#define _AMEBAD2_SWRCALIB_H_

/**
  * @}
  */

/** @defgroup Swr_Calib_Info_define
  * @{
  */
enum Swr_Calib_Info {
	Swr_Calib_Twocalib = 0x01,
	Swr_Calib_Onecalib = 0x02,
	Swr_Calib_Nocalib = 0xFF,
};

#define IS_SWR_CALIB_Info(Swr_Calib) (((Swr_Calib) == Swr_Calib_Twocalib) || \
								((Swr_Calib) == Swr_Calib_Onecalib) || \
								((Swr_Calib) == Swr_Calib_Nocalib))
/**
  * @}
  */

/** @defgroup SWR_Calib_Value_define
  * @{
  */
#define SWR_Calib_Twocalib_4Bit_Value(x)		((u8)(((x) & 0xF0) >> 4))
#define SWR_Calib_Onecalib_4Bit_Value(x)		((u8)(((x) & 0x0F) >> 0))
#define AUDIO_Calib_5Bit_Value(x)		((u8)(((x) & 0x1F) >> 0))
/**
  * @}
  */

/** @defgroup VOL_SWR_OTP_Address_Index_define
  * @{
  */
#define		SWR_PFM_125		0
#define		SWR_PWM_135		1
#define		SWR_PFM_09		2
#define		SWR_PWM_10		3
#define		AON_LDO_09		4
#define		CORE_LDO_10		5
#define		ANA_LDO_180		6
#define IS_SWR_CALIB_VOLIDX(VOLIDX) (((VOLIDX) == SWR_PFM_125) || \
								((VOLIDX) == SWR_PWM_135) || \
								((VOLIDX) == SWR_PFM_09) || \
								((VOLIDX) == SWR_PWM_10) || \
								((VOLIDX) == AON_LDO_09) || \
								((VOLIDX) == CORE_LDO_10) || \
								((VOLIDX) == ANA_LDO_180))

/**
  * @}
  */

/** @defgroup SWR_Calib_Vol_Change_Dir_define
  * @{
  */
#define RaiseVol		1
#define ReduceVol		0
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SWR_Calib_Exported_Functions SWR_Calib Exported Functions
  * @{
  */
_LONG_CALL_ u8 SWR_Calib_Get_Value(u8 swr_idx);
_LONG_CALL_ u8 SWR_Calib_Get_Value_AUD(void);
_LONG_CALL_ void SWR_Calib_Load(void);
_LONG_CALL_ void SWR_Calib_RaiseVoltage(void);
_LONG_CALL_ void SWR_Calib_ReduceVoltage(void);
_LONG_CALL_ void SWR_CORE_Vol_Set(u8 vol_type);
_LONG_CALL_ u8 SWR_CORE_Vol_Get(void);
/**
  * @}
  */

#endif
