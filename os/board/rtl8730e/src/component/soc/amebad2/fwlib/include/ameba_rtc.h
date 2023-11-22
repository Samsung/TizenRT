#ifndef _AMEBAD2_RTC_H_
#define _AMEBAD2_RTC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup RTC
  * @brief RTC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup RTC_Register_Definitions RTC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup RTC_TR
 * @brief RTC Time Reg
 * @{
 **/
#define RTC_MASK_DAY        ((u32)0x000001FF << 23)          /*!<R/W 0  DAY in binary format */
#define RTC_DAY(x)          ((u32)(((x) & 0x000001FF) << 23))
#define RTC_GET_DAY(x)      ((u32)(((x >> 23) & 0x000001FF)))
#define RTC_BIT_PM          ((u32)0x00000001 << 22)          /*!<R/W 0  PM:AM/PM notation *0: AM or 24-hour foramt *1: PM */
#define RTC_PM(x)           ((u32)(((x) & 0x00000001) << 22))
#define RTC_MASK_HT         ((u32)0x00000003 << 20)          /*!<R/W 0  Hour tens in BCD format */
#define RTC_HT(x)           ((u32)(((x) & 0x00000003) << 20))
#define RTC_GET_HT(x)       ((u32)(((x >> 20) & 0x00000003)))
#define RTC_MASK_HU         ((u32)0x0000000F << 16)          /*!<R/W 0  Hour units in BCD format */
#define RTC_HU(x)           ((u32)(((x) & 0x0000000F) << 16))
#define RTC_GET_HU(x)       ((u32)(((x >> 16) & 0x0000000F)))
#define RTC_MASK_MNT        ((u32)0x00000007 << 12)          /*!<R/W 0  Minute tens in BCD format */
#define RTC_MNT(x)          ((u32)(((x) & 0x00000007) << 12))
#define RTC_GET_MNT(x)      ((u32)(((x >> 12) & 0x00000007)))
#define RTC_MASK_MNU        ((u32)0x0000000F << 8)          /*!<R/W 0  Minute units in BCD format */
#define RTC_MNU(x)          ((u32)(((x) & 0x0000000F) << 8))
#define RTC_GET_MNU(x)      ((u32)(((x >> 8) & 0x0000000F)))
#define RTC_MASK_ST         ((u32)0x00000007 << 4)          /*!<R/W 0  Second tens in BCD format */
#define RTC_ST(x)           ((u32)(((x) & 0x00000007) << 4))
#define RTC_GET_ST(x)       ((u32)(((x >> 4) & 0x00000007)))
#define RTC_MASK_SU         ((u32)0x0000000F << 0)          /*!<R/W 0  Second units in BCD format */
#define RTC_SU(x)           ((u32)(((x) & 0x0000000F) << 0))
#define RTC_GET_SU(x)       ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup RTC_CR
 * @brief RTC Ctrl Reg
 * @{
 **/
#define RTC_MASK_DAY_THRES  ((u32)0x000001FF << 23)          /*!<R 1FFh  Day threshold in binary format */
#define RTC_DAY_THRES(x)    ((u32)(((x) & 0x000001FF) << 23))
#define RTC_GET_DAY_THRES(x) ((u32)(((x >> 23) & 0x000001FF)))
#define RTC_BIT_DOVTHIE     ((u32)0x00000001 << 16)          /*!<R/W 0  Day over threshold interrupt enable *0: Day over threshold interrupt disabled *1: Day over threshold interrupt enabled */
#define RTC_DOVTHIE(x)      ((u32)(((x) & 0x00000001) << 16))
#define RTC_BIT_WUTIE       ((u32)0x00000001 << 14)          /*!<R/W 0  Wakeup timer interrupt enable *0: Wakeup timer interrupt disabled *1: Wakeup timer interrupt enabled */
#define RTC_WUTIE(x)        ((u32)(((x) & 0x00000001) << 14))
#define RTC_BIT_ALMIE       ((u32)0x00000001 << 12)          /*!<R/W 0  Alarm interrupt enable *0: Alarm interrupt disabled *1: Alarm interrupt enabled */
#define RTC_ALMIE(x)        ((u32)(((x) & 0x00000001) << 12))
#define RTC_BIT_WUTE        ((u32)0x00000001 << 10)          /*!<R/W 0  Wakeup timer enable *0: Wakeup timer disabled *1: Wakeup timer enabled */
#define RTC_WUTE(x)         ((u32)(((x) & 0x00000001) << 10))
#define RTC_BIT_ALME        ((u32)0x00000001 << 8)          /*!<R/W 0  Alarm enable *0: Alarm disabled *1: Alarm enabled */
#define RTC_ALME(x)         ((u32)(((x) & 0x00000001) << 8))
#define RTC_BIT_FMT         ((u32)0x00000001 << 7)          /*!<R/W 0  Hour format *0: 24 hour/day format *1: AM/PM hour format */
#define RTC_FMT(x)          ((u32)(((x) & 0x00000001) << 7))
#define RTC_MASK_OSEL       ((u32)0x00000003 << 5)          /*!<R/W 0  Output selection.There bits are used to select the flag to be routed to RTC_OUT output. *00: Output disabled *01: Alarm output enabled *10: Clock output is clk_spre (default:1Hz) *11: Clock output is clk_apre (default:512Hz) */
#define RTC_OSEL(x)         ((u32)(((x) & 0x00000003) << 5))
#define RTC_GET_OSEL(x)     ((u32)(((x >> 5) & 0x00000003)))
#define RTC_BIT_BYPSHAD     ((u32)0x00000001 << 3)          /*!<R/W 0  Bypass the shadow registers. *0: Calendar values (when reading from RTC_TR) are taken from the shadow register, which are updated once every two RTCCLK cycles. *1: Calendar values (when reading from RTC_TR) are taken directly from the calendar counters. */
#define RTC_BYPSHAD(x)      ((u32)(((x) & 0x00000001) << 3))
#define RTC_BIT_BKP         ((u32)0x00000001 << 2)          /*!<R/W 0  Backup This bit can be written by the user to memorize whether the daylight saving time change has been performed or not. */
#define RTC_BKP(x)          ((u32)(((x) & 0x00000001) << 2))
#define RTC_BIT_SUB1H       ((u32)0x00000001 << 1)          /*!<W 0  Subtract one hour(winter time change),when this bit is set outside initialization mode,1 hour is subtracted to the calendar time if the current hour is not 0,this bit is always read as 0. *0:No effect *1:subtract one hour to the current time */
#define RTC_SUB1H(x)        ((u32)(((x) & 0x00000001) << 1))
#define RTC_BIT_ADD1H       ((u32)0x00000001 << 0)          /*!<W 0  Add one hour(summer time change),when this bit is set outside initialization mode,1 hour is add to the calendar time,this bit is always read as 0. *0:No effect *1:add one hour to the current time */
#define RTC_ADD1H(x)        ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup RTC_ISR
 * @brief RTC Init And Status Reg
 * @{
 **/
#define RTC_BIT_RECALPF     ((u32)0x00000001 << 16)          /*!<R 0  Recalibration pending flag The RECALPF status flag is automatically set to '1' when software writes to the RTC_CALIBR register, indicating that the RTCCALIBR register is blocked. When the new calibration settings are taken into account, this bit returns to '0'. Refer to Re-calibration on-the-fly. */
#define RTC_RECALPF(x)      ((u32)(((x) & 0x00000001) << 16))
#define RTC_BIT_DOVTHF      ((u32)0x00000001 << 15)          /*!<R/W1C 0  Day over threshold flag This flag is set by hardware when the Day in RTC_TR over the DAY_THRES set in RTC_CR register. */
#define RTC_DOVTHF(x)       ((u32)(((x) & 0x00000001) << 15))
#define RTC_BIT_WUTF        ((u32)0x00000001 << 10)          /*!<R/W1C 0  Wakeup timer flag This flag is set by hardware when the wakeup auto-reload counter reaches to. This flag must be cleared by software at least 2 RTCCLK periods before WUTF is set to '1' again. */
#define RTC_WUTF(x)         ((u32)(((x) & 0x00000001) << 10))
#define RTC_BIT_ALMF        ((u32)0x00000001 << 8)          /*!<R/W1C 0  Alarm flag This flag is set by hardware when the time register (RTC_TR) matches the Alarm registers (RTC_ALMR1L and RTC_ALMR1H). */
#define RTC_ALMF(x)         ((u32)(((x) & 0x00000001) << 8))
#define RTC_BIT_INIT        ((u32)0x00000001 << 7)          /*!<R/W 0  Initialization mode *0: Free running mode. *1: Initialization mode used to program time and date register (RTC_TR), and prescaler register (RTC_PRER). Counters are stoped and start counting from the new value when INIT is set. */
#define RTC_INIT(x)         ((u32)(((x) & 0x00000001) << 7))
#define RTC_BIT_INITF       ((u32)0x00000001 << 6)          /*!<R 0  Initialization flag. When this bit is set to '1', the RTC is in initialization state, and the time, date, and prescaler registers can be updated. *0: Calendar registers update is not allowed. *1: Calendar registers update is allowed. */
#define RTC_INITF(x)        ((u32)(((x) & 0x00000001) << 6))
#define RTC_BIT_RSF         ((u32)0x00000001 << 5)          /*!<R/W1C 0  Registers synchronization flag. This bit is set by hardware each time the calendar registers are copied into the shadow registers (RTC_TR). This bit is cleared by hardware in initialization mode or when in bypass shadow register mode (BYPSHAD=1). This bit can also be cleared by software. It is cleared either by software or by hardware in initialization mode. *0: Calendar shadow registers are not yet synchronized. *1: Calendar shadow registers are synchronized. */
#define RTC_RSF(x)          ((u32)(((x) & 0x00000001) << 5))
#define RTC_BIT_INITS       ((u32)0x00000001 << 4)          /*!<R 0  This bit is set by hardware when the calendar day field is different from 0 (RTC domain reset state). *0: Calendar has not been initialized. *1: Calendar has been initialized. */
#define RTC_INITS(x)        ((u32)(((x) & 0x00000001) << 4))
#define RTC_BIT_WUTWF       ((u32)0x00000001 << 2)          /*!<R 0  Wakeup timer write flag This flag is set by hardware when WUT value can be changed, after the WUTE bit has been set to '0' in RTC_CR. *0: Wakeup timer configuration update not allowd except in initialization mode *1: Wakeup timer configuration update allowed */
#define RTC_WUTWF(x)        ((u32)(((x) & 0x00000001) << 2))
#define RTC_BIT_WUTRSF      ((u32)0x00000001 << 1)          /*!<R/W1C 0  This bit is set by hardware each time the WUTE bit is copied into the shadow register. This bit is cleared by hardware in initialization mode. This bit can also be cleared by software. It is cleared either by software or by hardware in initialization mode. *0: WUTE is not yet synchronized. *1: WUTE is synchronized. */
#define RTC_WUTRSF(x)       ((u32)(((x) & 0x00000001) << 1))
#define RTC_BIT_ALMWF       ((u32)0x00000001 << 0)          /*!<R 0  Alarm write flag This bit is set by hardware when Alarm values can be changed, after the ALME bit has been set to '0' in RTC_CR. It is cleared by hardware when ALME bit has been set to '1' in RTC_CR. *0: Alarm update not allowed *1: Alarm update allowed */
#define RTC_ALMWF(x)        ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup RTC_PRER
 * @brief RTC Prescaler Reg
 * @{
 **/
#define RTC_MASK_PREDIV_A   ((u32)0x000001FF << 16)          /*!<R/W 7Fh  Asynchronous prescaler factor This is the asynchronous division factor: clk_apre freq = RTCCLK freq/(PREDIV_A + 1) */
#define RTC_PREDIV_A(x)     ((u32)(((x) & 0x000001FF) << 16))
#define RTC_GET_PREDIV_A(x) ((u32)(((x >> 16) & 0x000001FF)))
#define RTC_MASK_PREDIV_S   ((u32)0x000001FF << 0)          /*!<R/W FFh  Synchronous prescaler factor This is the synchronous division factor: clk_spre freq = clk_apre freq/(PREDIV_S + 1) */
#define RTC_PREDIV_S(x)     ((u32)(((x) & 0x000001FF) << 0))
#define RTC_GET_PREDIV_S(x) ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup RTC_CALIBR
 * @brief RTC Calibration Reg
 * @{
 **/
#define RTC_MASK_CALP       ((u32)0x00000007 << 16)          /*!<R/W 0  Calibration period */
#define RTC_CALP(x)         ((u32)(((x) & 0x00000007) << 16))
#define RTC_GET_CALP(x)     ((u32)(((x >> 16) & 0x00000007)))
#define RTC_BIT_DCE         ((u32)0x00000001 << 15)          /*!<R/W 0  Digital calibration enable *0: Digital calibration disabled *1: Digital calibration enabled */
#define RTC_DCE(x)          ((u32)(((x) & 0x00000001) << 15))
#define RTC_BIT_DCS         ((u32)0x00000001 << 14)          /*!<R/W 0  Digital calibration signal *0: Positive calibration, time update frequency is increased. *1: Negative calibration, time update frequency is decreased. */
#define RTC_DCS(x)          ((u32)(((x) & 0x00000001) << 14))
#define RTC_MASK_DC         ((u32)0x0000007F << 0)          /*!<R/W 0  Digital calibration */
#define RTC_DC(x)           ((u32)(((x) & 0x0000007F) << 0))
#define RTC_GET_DC(x)       ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup RTC_ALMR1L
 * @brief RTC Alarm 1 Reg Low
 * @{
 **/
#define RTC_BIT_MSK2        ((u32)0x00000001 << 23)          /*!<R/W 0  Alarm hour mask *0: Alarm set if the hours match *1: Hours don't care in Alarm comparison */
#define RTC_MSK2(x)         ((u32)(((x) & 0x00000001) << 23))
#define RTC_BIT_ALR_PM      ((u32)0x00000001 << 22)          /*!<R/W 0  AM/PM notation. *0: AM or 24-hour format *1: PM */
#define RTC_ALR_PM(x)       ((u32)(((x) & 0x00000001) << 22))
#define RTC_MASK_ALR_HT     ((u32)0x00000003 << 20)          /*!<R/W 0  Hour tens in BCD format */
#define RTC_ALR_HT(x)       ((u32)(((x) & 0x00000003) << 20))
#define RTC_GET_ALR_HT(x)   ((u32)(((x >> 20) & 0x00000003)))
#define RTC_MASK_ALR_HU     ((u32)0x0000000F << 16)          /*!<R/W 0  Hour units in BCD format */
#define RTC_ALR_HU(x)       ((u32)(((x) & 0x0000000F) << 16))
#define RTC_GET_ALR_HU(x)   ((u32)(((x >> 16) & 0x0000000F)))
#define RTC_BIT_MSK1        ((u32)0x00000001 << 15)          /*!<R/W 0  Alarm minutes mask. *0: Alarm set if the minutes match *1: Minutes don't care in Alarm comprison */
#define RTC_MSK1(x)         ((u32)(((x) & 0x00000001) << 15))
#define RTC_MASK_ALR_MNT    ((u32)0x00000007 << 12)          /*!<R/W 0  Minute tens in BCD format */
#define RTC_ALR_MNT(x)      ((u32)(((x) & 0x00000007) << 12))
#define RTC_GET_ALR_MNT(x)  ((u32)(((x >> 12) & 0x00000007)))
#define RTC_MASK_ALR_MNU    ((u32)0x0000000F << 8)          /*!<R/W 0  Minute units in BCD format */
#define RTC_ALR_MNU(x)      ((u32)(((x) & 0x0000000F) << 8))
#define RTC_GET_ALR_MNU(x)  ((u32)(((x >> 8) & 0x0000000F)))
#define RTC_BIT_MSK0        ((u32)0x00000001 << 7)          /*!<R/W 0  Alarm seconds mask *0: Alarm set if the seconds match *1: Seconds don't care in Alarm comprison */
#define RTC_MSK0(x)         ((u32)(((x) & 0x00000001) << 7))
#define RTC_MASK_ALR_ST     ((u32)0x00000007 << 4)          /*!<R/W 0  Second tens in BCD format */
#define RTC_ALR_ST(x)       ((u32)(((x) & 0x00000007) << 4))
#define RTC_GET_ALR_ST(x)   ((u32)(((x >> 4) & 0x00000007)))
#define RTC_MASK_ALR_SU     ((u32)0x0000000F << 0)          /*!<R/W 0  Second units in BCD format */
#define RTC_ALR_SU(x)       ((u32)(((x) & 0x0000000F) << 0))
#define RTC_GET_ALR_SU(x)   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup RTC_ALMR1H
 * @brief
 * @{
 **/
#define RTC_BIT_MSK3        ((u32)0x00000001 << 9)          /*!<R/W 0  Alarm day mask *0: Alarm set if the days match *1: Days don't care in Alarm comprison */
#define RTC_MSK3(x)         ((u32)(((x) & 0x00000001) << 9))
#define RTC_MASK_ALR_DAY    ((u32)0x000001FF << 0)          /*!<R/W 0  DAY in binary format */
#define RTC_ALR_DAY(x)      ((u32)(((x) & 0x000001FF) << 0))
#define RTC_GET_ALR_DAY(x)  ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup RTC_WPR
 * @brief RTC Write Protect Reg
 * @{
 **/
#define RTC_MASK_KEY        ((u32)0x000000FF << 0)          /*!<R/W 0  Write protection key This byte is written by software. Refer to RTC register write protection for a description of how to unlock RTC register write protection. */
#define RTC_KEY(x)          ((u32)(((x) & 0x000000FF) << 0))
#define RTC_GET_KEY(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup RTC_YEAR
 * @brief RTC Year Reg
 * @{
 **/
#define RTC_BIT_RESTORE     ((u32)0x00000001 << 31)          /*!<R/W 0  Reset Flag. Indicates whether any reset conditions have occurred (except POR/PDR/BOD), so that the bit is set to '1' before reset, and then the global variable that determines whether the recovery time information is needed depending on whether the bit is '1'. */
#define RTC_RESTORE(x)      ((u32)(((x) & 0x00000001) << 31))
#define RTC_MASK_YEAR       ((u32)0x000000FF << 0)          /*!<R/W 0  Year in binary format */
#define RTC_YEAR(x)         ((u32)(((x) & 0x000000FF) << 0))
#define RTC_GET_YEAR(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup RTC_WUTR
 * @brief RTC Wakeup Timer Reg
 * @{
 **/
#define RTC_MASK_WUT        ((u32)0x0001FFFF << 0)          /*!<R/W 0  Wakeup auto-reload value bits When the wakeup timer is enabled (WUTE is set to '1'), the WUTF flag is set every (WUT[16:0]+1) cycles. The first assertion of WUTF occurs between WUT and (WUT+1) cycles after WUTE is set. */
#define RTC_WUT(x)          ((u32)(((x) & 0x0001FFFF) << 0))
#define RTC_GET_WUT(x)      ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup RTC_Exported_Types RTC Exported Types
  * @{
*****************************************************************************/


/**
 * @brief RTC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t RTC_TR;                                 /*!< RTC TIME REG Register,  Address offset: 0x000 */
	__IO uint32_t RTC_CR;                                 /*!< RTC CTRL REG Register,  Address offset: 0x004 */
	__IO uint32_t RTC_ISR;                                /*!< RTC INIT AND STATUS REG Register,  Address offset: 0x008 */
	__IO uint32_t RTC_PRER;                               /*!< RTC PRESCALER REG Register,  Address offset: 0x00C */
	__IO uint32_t RTC_CALIBR;                             /*!< RTC CALIBRATION REG Register,  Address offset: 0x010 */
	__IO uint32_t RTC_ALMR1L;                             /*!< RTC ALARM 1 REG LOW Register,  Address offset: 0x014 */
	__IO uint32_t RTC_ALMR1H;                             /*!< - RTC ALARM 1 REG HIGH Register,  Address offset: 0x018 */
	__IO uint32_t RTC_WPR;                                /*!< RTC WRITE PROTECT REG Register,  Address offset: 0x01C */
	__IO uint32_t RTC_YEAR;                               /*!< RTC YEAR REG Register,  Address offset: 0x020 */
	__IO uint32_t RTC_WUTR;                               /*!< RTC WAKEUP TIMER REG Register,  Address offset: 0x024 */
} RTC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup RTC_Exported_Types RTC Exported Types
  * @{
  */

/**
  * @brief  RTC Init Structure Definition
  */
typedef struct {
	u32 RTC_HourFormat;   /*!< Specifies the RTC Hour Format.
					This parameter can be a value of @ref RTC_Hour_Format */

	u32 RTC_AsynchPrediv; /*!< Specifies the RTC Asynchronous Predivider value.
					This parameter must be a value of @ref RTC_Asynchronous_Predivider */

	u32 RTC_SynchPrediv;  /*!< Specifies the RTC Synchronous Predivider value.
					This parameter must be a value of @ref RTC_Synchronous_Predivider */

	u32 RTC_DayThreshold;  /*!< Specifies the RTC Day Threshold value.
					This parameter must be a value of @ref RTC_Day_Threshold */
} RTC_InitTypeDef;

/**
  * @brief  RTC Time Structure Definition
  */
typedef struct {
	u16 RTC_Year;	/*!< Year in binary format 16bits 1900~2155 */

	u16 RTC_Days;	/*!< Day in binary format 9bits 0~0x1FF */

	u8 RTC_Hours;    /*!< Specifies the RTC Time Hour.
				This parameter must be set to a value in the 1-12 range
				if the RTC_HourFormat_12 is selected or 0-23 range if
				the RTC_HourFormat_24 is selected. */

	u8 RTC_Minutes;  /*!< Specifies the RTC Time Minutes.
				This parameter must be set to a value in the 0-59 range. */

	u8 RTC_Seconds;  /*!< Specifies the RTC Time Seconds.
				This parameter must be set to a value in the 0-59 range. */

	u8 RTC_H12_PMAM;      /*!< Specifies the RTC AM/PM Time.
				This parameter can be a value of @ref RTC_AM_PM */
} RTC_TimeTypeDef;

/**
  * @brief  RTC Alarm Structure Definition
  */
typedef struct {
	RTC_TimeTypeDef RTC_AlarmTime;     /*!< Specifies the RTC Alarm Time members. */

	u32 RTC_AlarmMask;            /*!< Specifies the RTC Alarm1 Masks(H:M:S).
					This parameter can be a value of @ref RTC_AlarmMask1 */
	u32 RTC_Alarm2Mask;            /*!< Specifies the RTC Alarm2 Masks Day).
					This parameter can be a value of @ref RTC_AlarmMask2 */
} RTC_AlarmTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RTC_Exported_Constants RTC Exported Constants
  * @{
  */

/** @defgroup RTC_Input_Parameter_Format
  * @{
  */
#define RTC_Format_BIN			((u32)0x000000000)
#define RTC_Format_BCD			((u32)0x000000001)
#define IS_RTC_FORMAT(FORMAT)	(((FORMAT) == RTC_Format_BIN) || ((FORMAT) == RTC_Format_BCD))
/**
  * @}
  */

/** @defgroup RTC_Hour_Format
  * @{
  */
#define RTC_HourFormat_24					((u32)0x00000000)
#define RTC_HourFormat_12					((u32)0x00000080)
#define IS_RTC_HOUR_FORMAT(FORMAT)			(((FORMAT) == RTC_HourFormat_12) || \
                                        	((FORMAT) == RTC_HourFormat_24))
#define IS_RTC_TIME_DAY_THRES(TIME_DAYS)	((TIME_DAYS) <= 0x1FF)

/**
  * @}
  */

/** @defgroup RTC_Asynchronous_Predivider
  * @{
  */
#define IS_RTC_ASYNCH_PREDIV(PREDIV)   ((PREDIV) <= 0x1FF)
/**
  * @}
  */

/** @defgroup RTC_Synchronous_Predivider
  * @{
  */
#define IS_RTC_SYNCH_PREDIV(PREDIV)    ((PREDIV) <= 0x1FF)
/**
  * @}
  */

/** @defgroup RTC_Day_Threshold
  * @{
  */
#define RTC_DAYTHRES_MSK			((u32)0xFF800000)
#define IS_RTC_DAY_THRES(DAYS)		((DAYS) <= 0x1FF)
/**
  * @}
  */

/** @defgroup RTC_Base_Year
  * @{
  */
#define RTC_BASE_YEAR		((u16)1900)
/**
  * @}
  */

/** @defgroup RTC_Year_Threshold
  * @{
  */
#define IS_RTC_YEAR_THRES(YEAR)		((YEAR >= RTC_BASE_YEAR) && \
									((YEAR - RTC_BASE_YEAR) <= 0xFF))
/**
  * @}
  */

/** @defgroup RTC_Time_Definitions
  * @{
  */
#define IS_RTC_HOUR12(HOUR)		(((HOUR) > 0) && ((HOUR) <= 12))
#define IS_RTC_HOUR24(HOUR)		((HOUR) <= 23)
#define IS_RTC_MINUTES(MINUTES)	((MINUTES) <= 59)
#define IS_RTC_SECONDS(SECONDS)	((SECONDS) <= 59)
/**
  * @}
  */

/** @defgroup RTC_AM_PM
  * @{
  */
#define RTC_H12_AM				((u8)0x00) //AM or 24-hour format
#define RTC_H12_PM				((u8)0x01) //PM
#define IS_RTC_H12_AMPM(PM)		(((PM) == RTC_H12_AM) || ((PM) == RTC_H12_PM))
/**
  * @}
  */

/** @defgroup RTC_AlarmMask1
  * @{
  */
#define RTC_AlarmMask_None		((u32)0x00000000)
#define RTC_AlarmMask_Hours		((u32)0x00800000)
#define RTC_AlarmMask_Minutes	((u32)0x00008000)
#define RTC_AlarmMask_Seconds	((u32)0x00000080)
#define RTC_AlarmMask_All		((u32)0x00808080)
#define IS_ALARM_MASK(MASK)		(((MASK) & 0x7F7F7F) == 0)
/**
  * @}
  */

/** @defgroup RTC_AlarmMask2
  * @{
  */
#define RTC_Alarm2Mask_None		((u32)0x00000000)
#define RTC_Alarm2Mask_Days		((u32)0x00000200)
#define IS_ALARM2_MASK(MASK)	(((MASK) & ~RTC_Alarm2Mask_Days) == 0)
/**
  * @}
  */

/** @defgroup RTC_Alarm_Control
  * @{
  */
#define RTC_Alarm				((u32)0x00000100)
#define IS_RTC_ALARM(ALARM)		((ALARM) == RTC_Alarm)
#define IS_RTC_CMD_ALARM(ALARM)	(((ALARM) & RTC_Alarm) != (u32)0)
/**
  * @}
  */

/** @defgroup RTC_Alarm_Interrupt_Control
  * @{
  */
#define RTC_Alarm_IntEn			((u32)0x00001000)
/**
  * @}
  */

/** @defgroup RTC_DayLightSaving_Control
  * @{
  */
#define RTC_DayLightSaving_SUB1H		((u32)0x00000002)
#define RTC_DayLightSaving_ADD1H		((u32)0x00000001)
#define IS_RTC_DAYLIGHT_SAVING(SAVE)	(((SAVE) == RTC_DayLightSaving_SUB1H) || \
											((SAVE) == RTC_DayLightSaving_ADD1H))

#define RTC_StoreOperation_Reset		((u32)0x00000000)
#define RTC_StoreOperation_Set			((u32)0x00000004)
#define IS_RTC_STORE_OPERATION(OPERATION)	(((OPERATION) == RTC_StoreOperation_Reset) || \
											((OPERATION) == RTC_StoreOperation_Set))
/**
  * @}
  */

/** @defgroup RTC_Output_Control
  * @{
  */
#define RTC_Output_Disable		((u32)0x00000000)
#define RTC_Output_Alarm		((u32)0x00000020) ////wakeup
#define RTC_Output_clkspre		((u32)0x00000040) ////1Hz
#define RTC_Output_clkapre		((u32)0x00000060) ////256Hz

#define IS_RTC_OUTPUT(OUTPUT) (((OUTPUT) == RTC_Output_Disable) || \
                               ((OUTPUT) == RTC_Output_Alarm) || \
                               ((OUTPUT) == RTC_Output_clkspre) || \
                               ((OUTPUT) == RTC_Output_clkapre))
/**
  * @}
  */

/** @defgroup RTC_Smooth_Calibration_Control
  * @{
  */
#define RTC_CalibPeriod_1MIN		((u32)0x00000000)
#define RTC_CalibPeriod_2MIN		((u32)0x00010000)
#define RTC_CalibPeriod_3MIN		((u32)0x00020000)
#define RTC_CalibPeriod_4MIN		((u32)0x00030000)
#define RTC_CalibPeriod_5MIN		((u32)0x00040000)
#define RTC_CalibPeriod_6MIN		((u32)0x00050000)
#define RTC_CalibPeriod_7MIN		((u32)0x00060000)
#define RTC_CalibPeriod_8MIN		((u32)0x00070000)

#define IS_RTC_CALIB_PERIOD(DCP) (((DCP) == RTC_CalibPeriod_1MIN) || \
                                 ((DCP) == RTC_CalibPeriod_2MIN) || \
                                 ((DCP) == RTC_CalibPeriod_3MIN) || \
                                 ((DCP) == RTC_CalibPeriod_4MIN) || \
                                 ((DCP) == RTC_CalibPeriod_5MIN) || \
                                 ((DCP) == RTC_CalibPeriod_6MIN) || \
                                 ((DCP) == RTC_CalibPeriod_7MIN) || \
                                 ((DCP) == RTC_CalibPeriod_8MIN))

#define RTC_Calib_Disable		((u32)0x00000000)
#define RTC_Calib_Enable		((u32)0x00008000)

#define IS_RTC_CALIB_ENABLE(DCE) (((DCE) == RTC_Calib_Disable) || \
                                 ((DCE) == RTC_Calib_Enable))

#define RTC_CalibSign_Positive	((u32)0x00000000)
#define RTC_CalibSign_Negative	((u32)0x00004000)

#define IS_RTC_CALIB_SIGN(SIGN) (((SIGN) == RTC_CalibSign_Positive) || \
                                 ((SIGN) == RTC_CalibSign_Negative))

#define IS_RTC_CALIB_VALUE(VALUE) ((VALUE) <= 0x7F)
/**
  * @}
  */

/** @defgroup RTC_TR_Mask
  * @{
  */
#define RTC_TR_RESERVED_MASK	((u32)0xFFFF7F7F)
/**
  * @}
  */

/** @defgroup RTC_Timeout_Control
  * @{
  */
#define INITMODE_TIMEOUT	((u32) 0x00010000)
#define SYNCHRO_TIMEOUT		((u32) 0x00020000)
#define RECALPF_TIMEOUT		((u32) 0x00020000)
#define ALARMDIS_TIMEOUT	((u32) 0x00020000)
#define WUTDIS_TIMEOUT		((u32) 0x00020000)
/**
  * @}
  */

/** @defgroup RTC_Shift_Control
  * @{
  */
#define RTC_SHIFT_ALR_HU	16
#define RTC_SHIFT_ALR_MNU	8
#define RTC_SHIFT_ALR_PM	22
#define RTC_SHIFT_HU		16
#define RTC_SHIFT_MNU		8
#define RTC_SHIFT_PM		22
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RTC_Exported_Functions RTC Exported Functions
  * @{
  */

_LONG_CALL_ u32 RTC_BypassShadowCmd(u32 NewState);
_LONG_CALL_ void RTC_StructInit(RTC_InitTypeDef *RTC_InitStruct);
_LONG_CALL_ u32 RTC_Init(RTC_InitTypeDef *RTC_InitStruct);
_LONG_CALL_ void RTC_TimeStructInit(RTC_TimeTypeDef *RTC_TimeStruct);
_LONG_CALL_ u32 RTC_SetTime(u32 RTC_Format, RTC_TimeTypeDef *RTC_TimeStruct);
_LONG_CALL_ void RTC_GetTime(u32 RTC_Format, RTC_TimeTypeDef *RTC_TimeStruct);
_LONG_CALL_ u32 RTC_SetTimeDayYear(u32 RTC_TimeDay, u32 RTC_TimeYear);
_LONG_CALL_ void RTC_DayIntClear(void);
_LONG_CALL_ u32 RTC_DayIntCmd(u32 NewState);
_LONG_CALL_ u32 RTC_DayThresSet(u32 DayThres);
_LONG_CALL_ u32 RTC_DayThresGet(void);
_LONG_CALL_ u32 RTC_SetAlarm(u32 RTC_Format, RTC_AlarmTypeDef *RTC_AlarmStruct);
_LONG_CALL_ void RTC_AlarmStructInit(RTC_AlarmTypeDef *RTC_AlarmStruct);
_LONG_CALL_ void RTC_GetAlarm(u32 RTC_Format, RTC_AlarmTypeDef *RTC_AlarmStruct);
_LONG_CALL_ void RTC_AlarmCmd(u32 NewState);
_LONG_CALL_ void RTC_AlarmClear(void);
_LONG_CALL_ u32 RTC_WaitForWUTSynchro(void);
_LONG_CALL_ u32 RTC_SetWakeup(u32 RTC_WakeupRange);
_LONG_CALL_ u32 RTC_GetWakeup(void);
_LONG_CALL_ void RTC_WakeupCmd(u32 NewState);
_LONG_CALL_ void RTC_WakeupClear(void);
_LONG_CALL_ u32 RTC_DayLightSavingConfig(u32 RTC_DayLightSaving, u32 RTC_StoreOperation);
_LONG_CALL_ u32 RTC_GetStoreOperation(void);
_LONG_CALL_ u32 RTC_OutputConfig(u32 RTC_Output);
_LONG_CALL_ u32 RTC_SmoothCalibConfig(u32 CalibSign, u32 Value, u32 CalibPeriod, u32 Calib_Enable);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
