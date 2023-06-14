#ifndef _AMEBAD2_THERMAL_H_
#define _AMEBAD2_THERMAL_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup THERMAL
  * @brief THERMAL driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup THERMAL_Register_Definitions THERMAL Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup TM_GAIN
 * @brief Thermal Meter Gain Register
 * @{
 **/
#define TM_MASK_A                 ((u32)0x1FFFFFFF << 0)          /*!<R/W 0x07F80000  This bit defines thermal meter gain. * Bit[28:14]: Integer part * Bit[13:0]: Decimal part */
#define TM_A(x)                   ((u32)(((x) & 0x1FFFFFFF) << 0))
#define TM_GET_A(x)               ((u32)(((x >> 0) & 0x1FFFFFFF)))
/** @} */

/** @defgroup TM_OFFSET
 * @brief Thermal Meter Offset Register
 * @{
 **/
#define TM_MASK_B                 ((u32)0x003FFFFF << 0)          /*!<R/W 0x374000  This bit defines thermal meter offset. * Bit21: Signed bit * Bit[20:11]: Integer part * Bit[10:0]: Decimal part */
#define TM_B(x)                   ((u32)(((x) & 0x003FFFFF) << 0))
#define TM_GET_B(x)               ((u32)(((x >> 0) & 0x003FFFFF)))
/** @} */

/** @defgroup TM_CTRL
 * @brief Thermal Meter Control Register
 * @{
 **/
#define TM_MASK_PWR               ((u32)0x000000FF << 24)          /*!<R/W 0x69  thm protect password. tm_pow,tm_powcut,tm_rstb,tm_highcmp_pt_en can be programed only when this value is 8'h69. */
#define TM_PWR(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define TM_GET_PWR(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define TM_BIT_POW                ((u32)0x00000001 << 21)          /*!<R/W 0x1  thermal poweron signal * 0:power down * 1:power on */
#define TM_BIT_POWCUT             ((u32)0x00000001 << 20)          /*!<R/W 0x1  Power cut * 0: disable power cut * 1: enable power cut */
#define TM_BIT_CLK_INV_SEL        ((u32)0x00000001 << 19)          /*!<R/W 0x0  Clock phase invert select when read temperature. * 0: not invert (sample data in rising edge) * 1: invert (sample data in falling edge) */
#define TM_BIT_RSTB               ((u32)0x00000001 << 18)          /*!<R/W 0x1  Thermal reset signal. * 0: disable thermal * 1: enable thermal */
#define TM_MASK_CHOPFREQSEL       ((u32)0x0000000F << 14)          /*!<R/W 0x1  Chop frequecy select * 0: fs/64 * 1: fs/128 * 3: fs/256 * 7: fs/512 * 15: fs/1024 */
#define TM_CHOPFREQSEL(x)         ((u32)(((x) & 0x0000000F) << 14))
#define TM_GET_CHOPFREQSEL(x)     ((u32)(((x >> 14) & 0x0000000F)))
#define TM_MASK_OSR               ((u32)0x00000007 << 11)          /*!<R/W 0x0  Deci filter DSR select * 0: 128 * 1: 256 * 2: 512 * 3: 1024 */
#define TM_OSR(x)                 ((u32)(((x) & 0x00000007) << 11))
#define TM_GET_OSR(x)             ((u32)(((x >> 11) & 0x00000007)))
#define TM_BIT_HOLD_EN            ((u32)0x00000001 << 10)          /*!<R/W 0x0  Hold output */
#define TM_MASK_HOLD_DLY          ((u32)0x00000003 << 8)          /*!<R/W 0x1  Hold delay, adjust TM_CLK_OUT width */
#define TM_HOLD_DLY(x)            ((u32)(((x) & 0x00000003) << 8))
#define TM_GET_HOLD_DLY(x)        ((u32)(((x >> 8) & 0x00000003)))
#define TM_BIT_CHOP_EN            ((u32)0x00000001 << 7)          /*!<R/W 0x1  Chop enable */
#define TM_BIT_EN_LATCH           ((u32)0x00000001 << 6)          /*!<R/W/ES 0x1  Latch thermal sensor output immediately after power on for more accurate test */
#define TM_MASK_BIAS_SEL          ((u32)0x00000003 << 4)          /*!<R/W 0x1  Select ADC bias current * 0: 8μA * 1: 10μA * 2: 12μA * 3: 14μA */
#define TM_BIAS_SEL(x)            ((u32)(((x) & 0x00000003) << 4))
#define TM_GET_BIAS_SEL(x)        ((u32)(((x >> 4) & 0x00000003)))
#define TM_BIT_BIASDEM_EN         ((u32)0x00000001 << 3)          /*!<R/W 0x1  Bias DEM enable */
#define TM_BIT_ADCCKSEL           ((u32)0x00000001 << 0)          /*!<R/W 0x1  Selelct ADC fs * 0: clk_src/256 * 1: clk_src/128 */
/** @} */

/** @defgroup TM_TH_CTRL
 * @brief Thermal Meter Threshold Control Register
 * @{
 **/
#define TM_BIT_HIGHCMP_PT_EN      ((u32)0x00000001 << 29)          /*!<R/W 0x0  To enable compare tm_out with over temperature protect threshold: When tm_out[18:10] > tm_high_pt_thr, it will set aon reset. * 1: Enable the compare; * 0: Disable the compare. */
#define TM_MASK_HIGH_PT_THR       ((u32)0x000001FF << 20)          /*!<R/W 0x07D  Set over temperature protection threshold for comparison with TEMP_OUT. Only between 0x046 (70°C) and 0x08C (140°C) are valid. * Bit28: Signed bit * Bit[27:20]: Integer part */
#define TM_HIGH_PT_THR(x)         ((u32)(((x) & 0x000001FF) << 20))
#define TM_GET_HIGH_PT_THR(x)     ((u32)(((x >> 20) & 0x000001FF)))
#define TM_BIT_HIGHCMP_WT_EN      ((u32)0x00000001 << 19)          /*!<R/W 0x0  To enable compare tm_out with over temperature warning threshold: When tm_high_thr <= tm_out[18:10] < tm_high_pt_thr, it will set Interrupt pending flag ISR_TM_HIGH. * 1: Enable the compare; * 0: Disable the compare. */
#define TM_MASK_HIGH_WT_THR       ((u32)0x000001FF << 10)          /*!<R/W 0x069  Set the over temperature warning threshold for comparison with TEMP_OUT.Only values greater than 0 are supported * Bit18: Signed bit * Bit[17:10]: Integer part */
#define TM_HIGH_WT_THR(x)         ((u32)(((x) & 0x000001FF) << 10))
#define TM_GET_HIGH_WT_THR(x)     ((u32)(((x >> 10) & 0x000001FF)))
#define TM_BIT_LOWCMP_WT_EN       ((u32)0x00000001 << 9)          /*!<R/W 0x0  To enable compare TM_OUT with set Low threshold: When tm_out[18:10] <= tm_low_thr, it will set Interrupt pending flag ISR_TM_LOW. * 1: Enable the compare * 0: Disable the compare */
#define TM_MASK_LOW_WT_THR        ((u32)0x000001FF << 0)          /*!<R/W 0x1D8  Set the Low threshold for comparison with TEMP_OUT. * Bit8: Signed bit * Bit[7:0]: Integer part */
#define TM_LOW_WT_THR(x)          ((u32)(((x) & 0x000001FF) << 0))
#define TM_GET_LOW_WT_THR(x)      ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup TM_RESULT
 * @brief Thermal Meter Temperature Result Register
 * @{
 **/
#define TM_MASK_OUT               ((u32)0x0007FFFF << 0)          /*!<R 0x0  This bit defines real temperature. The result refreshes in real time. * Bit18: Signed bit * Bit[17:10]: Integer part * Bit[9:0]: Decimal part */
#define TM_OUT(x)                 ((u32)(((x) & 0x0007FFFF) << 0))
#define TM_GET_OUT(x)             ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup TM_ADC_RESULT
 * @brief Thermal Meter ADC Result Register
 * @{
 **/
#define TM_MASK_ADC_OUT           ((u32)0x003FFFFF << 0)          /*!<R 0x0  This bit defines thermal meter ADC output. The result refreshes in real time. * ADC output is a positive decimal number, real output=tm_out/2^22 */
#define TM_ADC_OUT(x)             ((u32)(((x) & 0x003FFFFF) << 0))
#define TM_GET_ADC_OUT(x)         ((u32)(((x >> 0) & 0x003FFFFF)))
/** @} */

/** @defgroup TM_MAX_CTRL
 * @brief Thermal Meter Max Temperature Control Register
 * @{
 **/
#define TM_BIT_MAX_CLR            ((u32)0x00000001 << 19)          /*!<WA0 0x0  This bit defines clear max temperature * 0: not clear * 1: clear tm_max value */
#define TM_MASK_MAX               ((u32)0x0007FFFF << 0)          /*!<R 0x40000  These bits define max temperature. */
#define TM_MAX(x)                 ((u32)(((x) & 0x0007FFFF) << 0))
#define TM_GET_MAX(x)             ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup TM_MIN_CTRL
 * @brief Thermal Meter Min Temperature Control Register
 * @{
 **/
#define TM_BIT_MIN_CLR            ((u32)0x00000001 << 19)          /*!<WA0 0x0  This bit defines clear min temperature * 0: not clear * 1: clear tm_min value */
#define TM_MASK_MIN               ((u32)0x0007FFFF << 0)          /*!<R 0x3FFFF  These bits define min temperature. */
#define TM_MIN(x)                 ((u32)(((x) & 0x0007FFFF) << 0))
#define TM_GET_MIN(x)             ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup TM_OUT_PWR_ON
 * @brief Thermal Meter Power On Temperature Register
 * @{
 **/
#define TM_MASK_TEMP_OUT_POWERON  ((u32)0x0007FFFF << 0)          /*!<R 0x0  This bit defines output temperature when power on reset. * Bit18: Signed bit * Bit[17:10]: Integer part * Bit[9:0]: Decimal part */
#define TM_TEMP_OUT_POWERON(x)    ((u32)(((x) & 0x0007FFFF) << 0))
#define TM_GET_TEMP_OUT_POWERON(x) ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/** @defgroup TM_INTR_CTRL
 * @brief Thermal Meter Interrupt Control Register
 * @{
 **/
#define TM_BIT_IMR_TM_LOW_WT      ((u32)0x00000001 << 1)          /*!<R/W 0x0  Interrupt control for thermal meter detect low temperature. * 0: Disable low temperature detect interrupt * 1: Enable low temperature detect interrupt */
#define TM_BIT_IMR_TM_HIGH_WT     ((u32)0x00000001 << 0)          /*!<R/W 0x0  Interrupt control for thermal meter detect over temperature warning. * 0: Disable over temperature warning detect interrupt * 1: Enable over temperature warning detect interrupt */
/** @} */

/** @defgroup TM_INTR_STS
 * @brief Thermal Meter Interrupt Status Register
 * @{
 **/
#define TM_BIT_ISR_TM_LOW_WT      ((u32)0x00000001 << 1)          /*!<RW1C 0x0  Interrupt pending flag for thermal meter detect low temperature. * 0: normal * 1: interrupt pending */
#define TM_BIT_ISR_TM_HIGH_WT     ((u32)0x00000001 << 0)          /*!<RW1C 0x0  Interrupt pending flag for thermal meter detect over temperature warning. * 0: normal * 1: interrupt pending */
/** @} */

/** @defgroup TM_TIMER
 * @brief Thermal Meter Timer Register
 * @{
 **/
#define TM_MASK_TIME_PERIOD       ((u32)0x000000FF << 0)          /*!<R/W 0x0F  Time period of auto power on */
#define TM_TIME_PERIOD(x)         ((u32)(((x) & 0x000000FF) << 0))
#define TM_GET_TIME_PERIOD(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup THERMAL_Exported_Types THERMAL Exported Types
  * @{
*****************************************************************************/


/**
 * @brief THERMAL Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t TM_GAIN;                                /*!< THERMAL METER GAIN REGISTER,  Address offset: 0x0000 */
	__IO uint32_t TM_OFFSET;                              /*!< THERMAL METER OFFSET REGISTER,  Address offset: 0x0004 */
	__IO uint32_t TM_CTRL;                                /*!< THERMAL METER CONTROL REGISTER,  Address offset: 0x0008 */
	__IO uint32_t TM_TH_CTRL;                             /*!< THERMAL METER THRESHOLD CONTROL REGISTER,  Address offset: 0x000C */
	__I  uint32_t TM_RESULT;                              /*!< THERMAL METER TEMPERATURE RESULT REGISTER,  Address offset: 0x0010 */
	__I  uint32_t TM_ADC_RESULT;                          /*!< THERMAL METER ADC RESULT REGISTER,  Address offset: 0x0014 */
	__IO uint32_t TM_MAX_CTRL;                            /*!< THERMAL METER MAX TEMPERATURE CONTROL REGISTER,  Address offset: 0x0018 */
	__IO uint32_t TM_MIN_CTRL;                            /*!< THERMAL METER MIN TEMPERATURE CONTROL REGISTER,  Address offset: 0x001C */
	__I  uint32_t TM_OUT_PWR_ON;                          /*!< THERMAL METER POWER ON TEMPERATURE REGISTER,  Address offset: 0x0020 */
	__IO uint32_t TM_INTR_CTRL;                           /*!< THERMAL METER INTERRUPT CONTROL REGISTER,  Address offset: 0x0024 */
	__IO uint32_t TM_INTR_STS;                            /*!< THERMAL METER INTERRUPT STATUS REGISTER,  Address offset: 0x0028 */
	__IO uint32_t TM_TIMER;                               /*!< THERMAL METER TIMER REGISTER,  Address offset: 0x002C */
} THERMAL_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup THERMAL_Exported_Types THERMAL Exported Types
  * @{
  */

/**
  * @brief  Thermal Init Structure Definition
  */
typedef struct {
	u8 TM_DownSampRate;			/*!< Specifies thermal decimation filter down sample rate.
									This parameter can be a value of @ref TM_Down_Sample_Rate */

	u8 TM_AdcClkDiv;				/*!< Specifies thermal ADC clock divider.
									This parameter can be a value of @ref TM_ADC_CLK_Divider */

	u16 TM_HighProtectThreshold;	/*!< Specifies thermal high protection threshold.
							    		When the measured temperature is greater than or equal to this value,
							     		HW power down protection is triggered.
							     		@note This parameter only between 0x046 and 0x08C is valid. */

	u16 TM_HighWarnThreshold;		/*!< Specifies thermal high warning threshold.
									The high warning interrupt is generated when the measured temperature is greater
									than or equal to this value, and less than TM_HighProtectThreshold,
									and tm_highcmp_wt_en=1, imr_tm_high_wt=1. */

	u16  TM_LowWarnThreshold;		/*!< Specifies thermal low warning threshold.
									The low warning interrupt is generated when the measured temperature is less
									than or equal to this value, and tm_lowcmp_wt_en=1, imr_tm_low_wt=1. */

	u8 TM_TimePeriod; 				/*!< Specifies timer period between HW power down and restart.
									When the measured temperature is greater than or equal to TM_HighProtectThreshold,
									HW auto power down, the system will auto restart after this value time period*/

} TM_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup THERMAL_Exported_Constants THERMAL Exported Constants
  * @{
  */

/** @defgroup TM_Down_Sample_Rate
  * @{
  */
#define TM_DSR_128						((u8)0x00)
#define TM_DSR_256						((u8)0x01)
#define TM_DSR_512						((u8)0x02)
#define TM_DSR_1024					((u8)0x03)

#define IS_TM_DOWN_SAMPLE_RATE(sel)     (((sel) == TM_DSR_128) || \
										((sel) == TM_DSR_256) || \
										((sel) == TM_DSR_512) || \
										((sel) == TM_DSR_1024))
/**
  * @}
  */

/** @defgroup TM_ADC_CLK_Divider
  * @{
  */
#define TM_ADC_CLK_DIV_256			((u8)0x00)
#define TM_ADC_CLK_DIV_128			((u8)0x01)

#define IS_TM_ADC_CLK_DIV(div)     (((div) == TM_ADC_CLK_DIV_256) || \
									((div) == TM_ADC_CLK_DIV_128))
/**
  * @}
  */

/** @defgroup TM_DECIMAL_OUT
  * @{
  */
#define TEMP_DECIMAL_OUT(x)		((u32)((x) & 0x000003FF))

/**
  * @}
  */


/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup THERMAL_Exported_Functions THERMAL Exported Functions
  * @{
  */

_LONG_CALL_ void TM_StructInit(TM_InitTypeDef *TM_InitStruct);
_LONG_CALL_ void TM_Init(TM_InitTypeDef *TM_InitStruct);
_LONG_CALL_ void TM_Cmd(u32 NewState);
_LONG_CALL_ void TM_INTConfig(u32 TM_IT, u32 NewState);
_LONG_CALL_ void TM_INTClear(void);
_LONG_CALL_ void TM_INTClearPendingBits(u32 TM_IT);
_LONG_CALL_ u32 TM_GetISR(void);
_LONG_CALL_ u32 TM_GetTempResult(void);
_LONG_CALL_ u32 TM_GetPowOnTemp(void);
_LONG_CALL_ u32 TM_GetMaxTemp(void);
_LONG_CALL_ u32 TM_GetMinTemp(void);
_LONG_CALL_ void TM_MaxTempClr(void);
_LONG_CALL_ void TM_MinTempClr(void);
_LONG_CALL_ void TM_SetLatch(u32 NewState);
_LONG_CALL_ void TM_SetHighPT(u32 NewState);
_LONG_CALL_ float TM_GetCdegree(u32 Data);
_LONG_CALL_ float TM_GetFdegree(u32 Data);

/**
  * @}
  */
/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
