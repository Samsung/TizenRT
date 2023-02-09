#ifndef _AMEBA_COMP_H_
#define _AMEBA_COMP_H_

/** @addtogroup ADC
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * Comparator:
  * 	- Base Address: Comparator
  * 	- Channel: 4
  * 	- Adjustable internal comparison voltage, 0~3.3v, each step 0.1v
  * 	- Adjustable internal divider resistors
  * 	- Cooperate with ADC comparator-assist mode
  *
  *****************************************************************************************
  * How to use Comparator
  *****************************************************************************************
  * 	  To use Comparator, the following steps are mandatory:
  *
  *      1. Enable the ADC & Comparator interface clock:
  *			RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);
  *
  *      2. Fill the CMP_InitStruct with the desired parameters.
  *			CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct)
  *
  *      3. Init Comparator with the parameters in CMP_InitStruct.
  *			CMP_Init(CMP_InitTypeDef* CMP_InitStruct)
  *
  *      4. Activate the Comparator:
  *			CMP_Cmd(ENABLE).
  *
  *      5.  Enbale specified mode:
  *			CMP_SWTrigCmd(ENABLE)/CMP_AutoCSwCmd(ENABLE)/ \
  *                            CMP_TimerTrigCmd(Tim_Idx, PeriodMs, ENABLE)
  *
  *****************************************************************************************
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @addtogroup ADC
  * @brief Comparator driver modules
  * @{
  */

/* Register Definitions --------------------------------------------------------*/
/** @defgroup COMP_Register_Definitions COMP Register Definitions
 * @{
 */

/**
 * @defgroup COMP_REF_CHx
 * @brief Comparator Channel x Reference Voltage Register
 * @{
 **/
#define COMP_MASK_REF1_CHx               ((u32)0x0000001F << 16)          /*!<R/W 0h  This field controls the comparator channel x internal reference voltage 1. Vref1 is equal to (the value of bit_comp_ref1)*0.1V. Therefore, when bit_comp_ref1 is 5, Vref1 is 0.5V. Vref1 is used as high level thredshold, so user must keep Vref1 >= Vref0. */
#define COMP_REF1_CHx(x)                 ((u32)(((x) & 0x0000001F) << 16))
#define COMP_GET_REF1_CHx(x)             ((u32)(((x >> 16) & 0x0000001F)))
#define COMP_MASK_REF0_CHx               ((u32)0x0000001F << 0)          /*!<R/W 0h  This field controls the comparator channel x internal reference voltage 0. Vref0 is equal to (the value of bit_comp_ref0)*0.1V. Therefore, when bit_comp_ref0 is 5, Vref0 is 0.5V. Vref0 is used as low level thredshold, so user must keep Vref1 >= Vref0. */
#define COMP_REF0_CHx(x)                 ((u32)(((x) & 0x0000001F) << 0))
#define COMP_GET_REF0_CHx(x)             ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**
 * @defgroup COMP_INTR_CTRL
 * @brief Comparator Interrupt Control Register
 * @{
 **/
#define COMP_BIT_CONTI_VS_EN             ((u32)0x00000001 << 30)          /*!<R/W 0h  At continuous mode, channel Vref hysteresis level rising interrupt. 0:disable; 1:enable */
#define COMP_BIT_CONTI_VH_EN             ((u32)0x00000001 << 29)          /*!<R/W 0h  At continuous mode, channel Vref high level rising interrupt. 0:disable; 1:enable */
#define COMP_BIT_CONTI_VL_EN             ((u32)0x00000001 << 28)          /*!<R/W 0h  At continuous mode, channel Vref low level rising interrupt. 0:disable; 1:enable */
#define COMP_MASK_WK_SYS_CTRL_3          ((u32)0x00000003 << 26)          /*!<R/W 0h  This field defines the criterion of when comparator channel 3 should wake up system. The comparator should send wakeup signal only when bit_comp_wk_sys_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up system. 1: When Vin >= Vref1, the comparator wakes up system. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up system. */
#define COMP_WK_SYS_CTRL_3(x)            ((u32)(((x) & 0x00000003) << 26))
#define COMP_GET_WK_SYS_CTRL_3(x)        ((u32)(((x >> 26) & 0x00000003)))
#define COMP_BIT_WK_SYS_EN_3             ((u32)0x00000001 << 25)          /*!<R/W 0h  This bit controls whether the comparator channel 3 sends wakeup signal or not when the criterion of bit_comp_wk_sys_ctrl matches. 0: Disable comparator wakes up system. 1: Enable comparator wakes up system. */
#define COMP_MASK_WK_SYS_CTRL_2          ((u32)0x00000003 << 23)          /*!<R/W 0h  This field defines the criterion of when comparator channel 2 should wake up system. The comparator should send wakeup signal only when bit_comp_wk_sys_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up system. 1: When Vin >= Vref1, the comparator wakes up system. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up system. */
#define COMP_WK_SYS_CTRL_2(x)            ((u32)(((x) & 0x00000003) << 23))
#define COMP_GET_WK_SYS_CTRL_2(x)        ((u32)(((x >> 23) & 0x00000003)))
#define COMP_BIT_WK_SYS_EN_2             ((u32)0x00000001 << 22)          /*!<R/W 0h  This bit controls whether the comparator channel 2 sends wakeup signal or not when the criterion of bit_comp_wk_sys_ctrl matches. 0: Disable comparator wakes up system. 1: Enable comparator wakes up system. */
#define COMP_MASK_WK_SYS_CTRL_1          ((u32)0x00000003 << 20)          /*!<R/W 0h  This field defines the criterion of when comparator channel 1 should wake up system. The comparator should send wakeup signal only when bit_comp_wk_sys_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up system. 1: When Vin >= Vref1, the comparator wakes up system. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up system. */
#define COMP_WK_SYS_CTRL_1(x)            ((u32)(((x) & 0x00000003) << 20))
#define COMP_GET_WK_SYS_CTRL_1(x)        ((u32)(((x >> 20) & 0x00000003)))
#define COMP_BIT_WK_SYS_EN_1             ((u32)0x00000001 << 19)          /*!<R/W 0h  This bit controls whether the comparator channel 1 sends wakeup signal or not when the criterion of bit_comp_wk_sys_ctrl matches. 0: Disable comparator wakes up system. 1: Enable comparator wakes up system. */
#define COMP_MASK_WK_SYS_CTRL_0          ((u32)0x00000003 << 17)          /*!<R/W 0h  This field defines the criterion of when comparator channel 0 should wake up system. The comparator should send wakeup signal only when bit_comp_wk_sys_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up system. 1: When Vin >= Vref1, the comparator wakes up system. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up system. */
#define COMP_WK_SYS_CTRL_0(x)            ((u32)(((x) & 0x00000003) << 17))
#define COMP_GET_WK_SYS_CTRL_0(x)        ((u32)(((x >> 17) & 0x00000003)))
#define COMP_BIT_WK_SYS_EN_0             ((u32)0x00000001 << 16)          /*!<R/W 0h  This bit controls whether the comparator channel 0 sends wakeup signal or not when the criterion of bit_comp_wk_sys_ctrl matches. 0: Disable comparator wakes up system. 1: Enable comparator wakes up system. */
#define COMP_MASK_WK_ADC_CTRL_3          ((u32)0x00000003 << 10)          /*!<R/W 0h  This field defines the criterion of when comparator channel 3 should wake up ADC. The comparator should send wakeup signal only when bit_comp_wk_adc_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up ADC. 1: When Vin >= Vref1, the comparator wakes up ADC. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up ADC. */
#define COMP_WK_ADC_CTRL_3(x)            ((u32)(((x) & 0x00000003) << 10))
#define COMP_GET_WK_ADC_CTRL_3(x)        ((u32)(((x >> 10) & 0x00000003)))
#define COMP_BIT_WK_ADC_EN_3             ((u32)0x00000001 << 9)          /*!<R/W 0h  This bit controls whether the comparator channel 3 sends wakeup signal or not when the criterion of bit_comp_wk_adc_ctrl matches. 0: Disable comparator wakes up ADC. 1: Enable comparator wakes up ADC. */
#define COMP_MASK_WK_ADC_CTRL_2          ((u32)0x00000003 << 7)          /*!<R/W 0h  This field defines the criterion of when comparator channel 2 should wake up ADC. The comparator should send wakeup signal only when bit_comp_wk_adc_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up ADC. 1: When Vin >= Vref1, the comparator wakes up ADC. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up ADC. */
#define COMP_WK_ADC_CTRL_2(x)            ((u32)(((x) & 0x00000003) << 7))
#define COMP_GET_WK_ADC_CTRL_2(x)        ((u32)(((x >> 7) & 0x00000003)))
#define COMP_BIT_WK_ADC_EN_2             ((u32)0x00000001 << 6)          /*!<R/W 0h  This bit controls whether the comparator channel 2 sends wakeup signal or not when the criterion of bit_comp_wk_adc_ctrl matches. 0: Disable comparator wakes up ADC. 1: Enable comparator wakes up ADC. */
#define COMP_MASK_WK_ADC_CTRL_1          ((u32)0x00000003 << 4)          /*!<R/W 0h  This field defines the criterion of when comparator channel 1 should wake up ADC. The comparator should send wakeup signal only when bit_comp_wk_adc_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up ADC. 1: When Vin >= Vref1, the comparator wakes up ADC. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up ADC. */
#define COMP_WK_ADC_CTRL_1(x)            ((u32)(((x) & 0x00000003) << 4))
#define COMP_GET_WK_ADC_CTRL_1(x)        ((u32)(((x >> 4) & 0x00000003)))
#define COMP_BIT_WK_ADC_EN_1             ((u32)0x00000001 << 3)          /*!<R/W 0h  This bit controls whether the comparator channel 1 sends wakeup signal or not when the criterion of bit_comp_wk_adc_ctrl matches. 0: Disable comparator wakes up ADC. 1: Enable comparator wakes up ADC. */
#define COMP_MASK_WK_ADC_CTRL_0          ((u32)0x00000003 << 1)          /*!<R/W 0h  This field defines the criterion of when comparator channel 0 should wake up ADC. The comparator should send wakeup signal only when bit_comp_wk_adc_en is 1, if the criterion matches. 0: When Vin < Vref0, the comparator wakes up ADC. 1: When Vin >= Vref1, the comparator wakes up ADC. 2: When Vin > Vref0 and Vin < Vref1, the comparator wakes up ADC. */
#define COMP_WK_ADC_CTRL_0(x)            ((u32)(((x) & 0x00000003) << 1))
#define COMP_GET_WK_ADC_CTRL_0(x)        ((u32)(((x >> 1) & 0x00000003)))
#define COMP_BIT_WK_ADC_EN_0             ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls whether the comparator channel 0 sends wakeup signal or not when the criterion of bit_comp_wk_adc_ctrl matches. 0: Disable comparator wakes up ADC. 1: Enable comparator wakes up ADC. */
/** @} */

/**
 * @defgroup COMP_WK_STS
 * @brief Comparator Wakeup ADC/SYS Status Register
 * @{
 **/
#define COMP_BIT_CONTI_VS_STS            ((u32)0x00000001 << 30)          /*!<RW1C 0h  At continuous mode, channel Vref hysteresis level rising interrupt status */
#define COMP_BIT_CONTI_VH_STS            ((u32)0x00000001 << 29)          /*!<RW1C 0h  At continuous mode, channel Vref high level rising interrupt status */
#define COMP_BIT_CONTI_VL_STS            ((u32)0x00000001 << 28)          /*!<RW1C 0h  At continuous mode, channel Vref low level rising interrupt status */
#define COMP_BIT_WK_SYS_STS_3            ((u32)0x00000001 << 25)          /*!<RW1C 0h  Channel 3 system wakeup status */
#define COMP_BIT_WK_SYS_STS_2            ((u32)0x00000001 << 22)          /*!<RW1C 0h  Channel 2 system wakeup status */
#define COMP_BIT_WK_SYS_STS_1            ((u32)0x00000001 << 19)          /*!<RW1C 0h  Channel 1 system wakeup status */
#define COMP_BIT_WK_SYS_STS_0            ((u32)0x00000001 << 16)          /*!<RW1C 0h  Channel 0 system wakeup status */
#define COMP_BIT_WK_ADC_STS_3            ((u32)0x00000001 << 9)          /*!<RW1C 0h  Channel 3 ADC wakeup status */
#define COMP_BIT_WK_ADC_STS_2            ((u32)0x00000001 << 6)          /*!<RW1C 0h  Channel 2 ADC wakeup status */
#define COMP_BIT_WK_ADC_STS_1            ((u32)0x00000001 << 3)          /*!<RW1C 0h  Channel 1 ADC wakeup status */
#define COMP_BIT_WK_ADC_STS_0            ((u32)0x00000001 << 0)          /*!<RW1C 0h  Channel 0 ADC wakeup status */
/** @} */

/**
 * @defgroup COMP_WK_STS_RAW
 * @brief Comparator Wakeup Register
 * @{
 **/
#define COMP_BIT_CONTI_VS_RAW            ((u32)0x00000001 << 30)          /*!<R 0h  At continuous mode, channel Vref hysteresis level rising interrupt status raw */
#define COMP_BIT_CONTI_VH_RAW            ((u32)0x00000001 << 29)          /*!<R 0h  At continuous mode, channel Vref high level rising interrupt status raw */
#define COMP_BIT_CONTI_VL_RAW            ((u32)0x00000001 << 28)          /*!<R 0h  At continuous mode, channel Vref low level rising interrupt status raw */
#define COMP_BIT_SYS_WAKE_3              ((u32)0x00000001 << 25)          /*!<R 0h  Channle 3 system wakeup status raw */
#define COMP_BIT_SYS_WAKE_2              ((u32)0x00000001 << 22)          /*!<R 0h  Channel 2 system wakeup status raw */
#define COMP_BIT_SYS_WAKE_1              ((u32)0x00000001 << 19)          /*!<R 0h  Channle 1 system wakeup status raw */
#define COMP_BIT_SYS_WAKE_0              ((u32)0x00000001 << 16)          /*!<R 0h  Channle 0 system wakeup status raw */
#define COMP_BIT_ADC_WAKE_3_RAW          ((u32)0x00000001 << 9)          /*!<R 0h  Channle 3 ADC wakeup status raw */
#define COMP_BIT_ADC_WAKE_2_RAW          ((u32)0x00000001 << 6)          /*!<R 0h  Channel 2 ADC wakeup status raw */
#define COMP_BIT_ADC_WAKE_1_RAW          ((u32)0x00000001 << 3)          /*!<R 0h  Channle 1 ADC wakeup status raw */
#define COMP_BIT_ADC_WAKE_0_RAW          ((u32)0x00000001 << 0)          /*!<R 0h  Channle 0 ADC wakeup status raw */
/** @} */

/**
 * @defgroup COMP_CHSW_LIST
 * @brief Comparator Channel Switch List Register
 * @{
 **/
#define COMP_MASK_CHSW_3                 ((u32)0x0000000F << 12)          /*!<R/W 0h  This field defines the channel 3 to do a comparison when the start-compare event takes place. */
#define COMP_CHSW_3(x)                   ((u32)(((x) & 0x0000000F) << 12))
#define COMP_GET_CHSW_3(x)               ((u32)(((x >> 12) & 0x0000000F)))
#define COMP_MASK_CHSW_2                 ((u32)0x0000000F << 8)          /*!<R/W 0h  This field defines the channel 2 to do a comparison when the start-compare event takes place. */
#define COMP_CHSW_2(x)                   ((u32)(((x) & 0x0000000F) << 8))
#define COMP_GET_CHSW_2(x)               ((u32)(((x >> 8) & 0x0000000F)))
#define COMP_MASK_CHSW_1                 ((u32)0x0000000F << 4)          /*!<R/W 0h  This field defines the channel 1 to do a comparison when the start-compare event takes place. */
#define COMP_CHSW_1(x)                   ((u32)(((x) & 0x0000000F) << 4))
#define COMP_GET_CHSW_1(x)               ((u32)(((x >> 4) & 0x0000000F)))
#define COMP_MASK_CHSW_0                 ((u32)0x0000000F << 0)          /*!<R/W 0h  This field defines the channel 0 to do a comparison when the start-compare event takes place. */
#define COMP_CHSW_0(x)                   ((u32)(((x) & 0x0000000F) << 0))
#define COMP_GET_CHSW_0(x)               ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup COMP_LAST_CH
 * @brief Comparator Last Channel Register
 * @{
 **/
#define COMP_MASK_LAST_CH                ((u32)0x00000003 << 0)          /*!<R/W 0h  Last channel used */
#define COMP_LAST_CH(x)                  ((u32)(((x) & 0x00000003) << 0))
#define COMP_GET_LAST_CH(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup COMP_BUSY_STS
 * @brief Comparator Busy Status Register
 * @{
 **/
#define COMP_BIT_BUSY_STS                ((u32)0x00000001 << 0)          /*!<R 0h  This bit reflects whether the comparatos is busy or not. 0: The comparatos is ready. 1: The comparator is busy. */
/** @} */

/**
 * @defgroup COMP_CH_STS
 * @brief Comparator Channel Status Register
 * @{
 **/
#define COMP_MASK_CH_STS_3               ((u32)0x00000003 << 6)          /*!<R 0h  This field reflects the channel 3 comparison result. 0: When Vin < Vref0. 1: When Vin >= Vref1. 2: When Vin > Vref0 and Vin < Vref1. */
#define COMP_CH_STS_3(x)                 ((u32)(((x) & 0x00000003) << 6))
#define COMP_GET_CH_STS_3(x)             ((u32)(((x >> 6) & 0x00000003)))
#define COMP_MASK_CH_STS_2               ((u32)0x00000003 << 4)          /*!<R 0h  This field reflects the channel 2 comparison result. 0: When Vin < Vref0. 1: When Vin >= Vref1. 2: When Vin > Vref0 and Vin < Vref1. */
#define COMP_CH_STS_2(x)                 ((u32)(((x) & 0x00000003) << 4))
#define COMP_GET_CH_STS_2(x)             ((u32)(((x >> 4) & 0x00000003)))
#define COMP_MASK_CH_STS_1               ((u32)0x00000003 << 2)          /*!<R 0h  This field reflects the channel 1 comparison result. 0: When Vin < Vref0. 1: When Vin >= Vref1. 2: When Vin > Vref0 and Vin < Vref1. */
#define COMP_CH_STS_1(x)                 ((u32)(((x) & 0x00000003) << 2))
#define COMP_GET_CH_STS_1(x)             ((u32)(((x >> 2) & 0x00000003)))
#define COMP_MASK_CH_STS_0               ((u32)0x00000003 << 0)          /*!<R 0h  This field reflects the channel 0 comparison result. 0: When Vin < Vref0. 1: When Vin >= Vref1. 2: When Vin > Vref0 and Vin < Vref1. */
#define COMP_CH_STS_0(x)                 ((u32)(((x) & 0x00000003) << 0))
#define COMP_GET_CH_STS_0(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup COMP_AUTO_SHUT
 * @brief Comparator Automatic Shut Register
 * @{
 **/
#define COMP_BIT_AUTO_SHUT               ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls the comparator to disable the analog module and mux automatically or not after the analog conversion is done. 0: The analog module and mux will NOT be disabled. 1: The analog module and mux will be disabled automatically after the analog conversion is done. */
/** @} */

/**
 * @defgroup COMP_EXT_TRIG_CTRL
 * @brief Comparator External Trigger Control Register
 * @{
 **/
#define COMP_BIT_EXT_WK_TIMER            ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls the external trigger source of general timer enabled or not. If it's enabled, the comparator would execute comparison process when the timer event sends to comparator module. 0: Disable external timer trigger. 1: Enable external timer trigger. */
/** @} */

/**
 * @defgroup COMP_EXT_TRIG_TIMER_SEL
 * @brief Comparator External Trigger Timer Select Register
 * @{
 **/
#define COMP_MASK_EXT_WK_TIMER_SEL       ((u32)0x00000007 << 0)          /*!<R/W 0h  This field defines which timer channel would be used to wake up the comparator. 0: timer module 0 is used as the comparator external trigger source. 1:timer module 1 is used as the comparator external trigger source. ...so on... 7:timer module 7 is used as the comparator external trigger source. */
#define COMP_EXT_WK_TIMER_SEL(x)         ((u32)(((x) & 0x00000007) << 0))
#define COMP_GET_EXT_WK_TIMER_SEL(x)     ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**
 * @defgroup COMP_RST_LIST
 * @brief Comparator Reset Channel List Register
 * @{
 **/
#define COMP_BIT_RST_LIST                ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls to reset the channel switch to default state or not. Once this bit is set, the auto/manual channel switch goes to the first channel as a default state. 0: Do Not reset conversion list to default state. 1:Reset conversion list to default state. */
/** @} */

/**
 * @defgroup COMP_AUTOSW_EN
 * @brief Comparator Automatic Channel Switch Enable Register
 * @{
 **/
#define COMP_BIT_AUTOSW_EN               ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls the automatic channel switch enabled or disabled. 0: Disable the automatic channel switch. If an automatic channel switch is in process, writing 0 will terminate the automatic channel switch. 1: Enable the automatic channel switch. When setting this bit, an automatic channel switch starts from the first channel in the channel switch list. */
/** @} */

/**
 * @defgroup COMP_EN_TRIG
 * @brief Comparator Enable Trigger Register
 * @{
 **/
#define COMP_BIT_DBG_EN                  ((u32)0x00000001 << 2)          /*!<R/W 0h  The comparator IP debug port enable. */
#define COMP_BIT_EN_TRIG                 ((u32)0x00000001 << 1)          /*!<R/W 0h  This bit controls the analog module and the analog mux of comparator to be enabled or not. Every time this bit is written, the comparator switches to a new channel and does one comparison. After this, it shuts down the analog module according to REG_COMP_AUTO_SHUT. Therefore, this bit can be used as a start-compare event which is controlled by software. Every time a comparison is done, software must clear this bit manually. 0: Disable the analog module and analog mux. 1: Enable the analog module and analog mux, then start a new channel comparison. */
#define COMP_BIT_ENABLE                  ((u32)0x00000001 << 0)          /*!<R/W 0h  The comparator IP internal enable. */
/** @} */

/**
 * @defgroup COMP_CTRL_CNT
 * @brief Comparator External Wake Shut Count Register
 * @{
 **/
#define COMP_MASK_POWON_CNT              ((u32)0x000000FF << 24)          /*!<R/W A0h  This field defines how long the comparator analog module can work after the comparator is enabled, the value will be multipled 2 in hardware circuit. Therefore, this is about 160us. */
#define COMP_POWON_CNT(x)                ((u32)(((x) & 0x000000FF) << 24))
#define COMP_GET_POWON_CNT(x)            ((u32)(((x >> 24) & 0x000000FF)))
#define COMP_MASK_DATA_VALID_CNT         ((u32)0x000000FF << 16)          /*!<R/W 6Eh  This field defines how long the comparator analog module data is valid for digital module after analog module works. It's about 55us. */
#define COMP_DATA_VALID_CNT(x)           ((u32)(((x) & 0x000000FF) << 16))
#define COMP_GET_DATA_VALID_CNT(x)       ((u32)(((x >> 16) & 0x000000FF)))
#define COMP_MASK_CHSW_CNT               ((u32)0x000000FF << 8)          /*!<R/W 6Eh  This field defines how long the channel switch can jump to next channel at automatic channel switch mode. It's about 55us. */
#define COMP_CHSW_CNT(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define COMP_GET_CHSW_CNT(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define COMP_MASK_EXT_WK_SHUT_CNT        ((u32)0x000000FF << 0)          /*!<R/W 96h  This field defines how long to disable comparator analog module when an external trigger event is sent to the comparator. The count must include the comparator response time to get a correct compare result.It's about 75us. */
#define COMP_EXT_WK_SHUT_CNT(x)          ((u32)(((x) & 0x000000FF) << 0))
#define COMP_GET_EXT_WK_SHUT_CNT(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**
 * @defgroup COMP_LPSD
 * @brief Comparator Analog Register
 * @{
 **/
#define COMP_BIT_BG                      ((u32)0x00000001 << 31)          /*!<R 0h  comparator's POW_BG, power on BG */
#define COMP_MASK_LPSD_DUMMY             ((u32)0x0001FFFF << 14)          /*!<R/W 0h  reserved */
#define COMP_LPSD_DUMMY(x)               ((u32)(((x) & 0x0001FFFF) << 14))
#define COMP_GET_LPSD_DUMMY(x)           ((u32)(((x >> 14) & 0x0001FFFF)))
#define COMP_BIT_EN_VREF0L               ((u32)0x00000001 << 13)          /*!<R/W 0h   */
#define COMP_BIT_EN_VREF0H               ((u32)0x00000001 << 12)          /*!<R/W 0h   */
#define COMP_MASK_LPSD_11_2_FW           ((u32)0x000003FF << 2)          /*!<R/WP 0h  comp_lpsd[11:2], for every bit: [11:7]: SEL_VREFL, Vref low voltage 0000: 25mV, 25mV/step [6:2]: SEL_VREFH, Vref high voltage 0000: 25mV, 25 mV/step */
#define COMP_LPSD_11_2_FW(x)             ((u32)(((x) & 0x000003FF) << 2))
#define COMP_GET_LPSD_11_2_FW(x)         ((u32)(((x >> 2) & 0x000003FF)))
#define COMP_BIT_LPSD_1                  ((u32)0x00000001 << 1)          /*!<R/W 0h  SD_POSEDGE, 1: Vin > Vref output high */
#define COMP_BIT_LPSD_0_FW               ((u32)0x00000001 << 0)          /*!<R/WP 0h  POW SD, power on SD */
/** @} */

/**
 * @defgroup COMP_CONTINUOUS_CTRL_0
 * @brief Comparator Continuous Control Register 0
 * @{
 **/
#define COMP_BIT_HYST_EN                 ((u32)0x00000001 << 31)          /*!<R/W 0h  enable comparator hysteresis circuit function. hysteresis circuit function can only take effect in continuous function. */
#define COMP_BIT_CONTINUOUS_EN           ((u32)0x00000001 << 30)          /*!<R/W 0h  enable comparator continuous sample function. continuous sample function can only take effect in autoswitch mode, and 4 channels in list are must configured into same channel */
#define COMP_MASK_ZCD_SEL                ((u32)0x00000003 << 28)          /*!<R/W 0h  select output ZCD signals source: 00: none ; 01: comp out Vref1; 10: comp out Vref0; 11: comp hysteresis */
#define COMP_ZCD_SEL(x)                  ((u32)(((x) & 0x00000003) << 28))
#define COMP_GET_ZCD_SEL(x)              ((u32)(((x >> 28) & 0x00000003)))
#define COMP_MASK_VS_DBC_CNT             ((u32)0x00007FFF << 0)          /*!<R/W 0h  hysteresis debounce counter for ZCD */
#define COMP_VS_DBC_CNT(x)               ((u32)(((x) & 0x00007FFF) << 0))
#define COMP_GET_VS_DBC_CNT(x)           ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/**
 * @defgroup COMP_CONTINUOUS_CTRL_1
 * @brief Comparator Continuous Control Register 1
 * @{
 **/
#define COMP_MASK_VH_DBC_CNT             ((u32)0x00007FFF << 16)          /*!<R/W 0h  Vref high level debounce counter for ZCD */
#define COMP_VH_DBC_CNT(x)               ((u32)(((x) & 0x00007FFF) << 16))
#define COMP_GET_VH_DBC_CNT(x)           ((u32)(((x >> 16) & 0x00007FFF)))
#define COMP_MASK_VL_DBC_CNT             ((u32)0x00007FFF << 0)          /*!<R/W 0h  Vref low level debounce counter for ZCD */
#define COMP_VL_DBC_CNT(x)               ((u32)(((x) & 0x00007FFF) << 0))
#define COMP_GET_VL_DBC_CNT(x)           ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */
/** @} */


/* Exported types --------------------------------------------------------*/

/** @defgroup COMP_Exported_Types COMP Exported Types
  * @{
*****************************************************************************/

/**
 * @brief COMP Register Declaration
 */
typedef struct {
	__IO uint32_t COMP_REF_CHx[4];                        /*!< COMPARATOR CHANNEL x REFERENCE VOLTAGE REGISTER,  Address offset: 0x000-0x00C */
	__IO uint32_t COMP_INTR_CTRL;                         /*!< COMPARATOR INTERRUPT CONTROL REGISTER,  Address offset: 0x010 */
	__IO uint32_t COMP_WK_STS;                            /*!< COMPARATOR WAKEUP ADC/SYS STATUS REGISTER,  Address offset: 0x014 */
	__I  uint32_t COMP_WK_STS_RAW;                        /*!< COMPARATOR WAKEUP REGISTER,  Address offset: 0x018 */
	__IO uint32_t COMP_CHSW_LIST;                         /*!< COMPARATOR CHANNEL SWITCH LIST REGISTER,  Address offset: 0x01C */
	__IO uint32_t COMP_LAST_CH;                           /*!< COMPARATOR LAST CHANNEL REGISTER,  Address offset: 0x020 */
	__I  uint32_t COMP_BUSY_STS;                          /*!< COMPARATOR BUSY STATUS REGISTER,  Address offset: 0x024 */
	__I  uint32_t COMP_CH_STS;                            /*!< COMPARATOR CHANNEL STATUS REGISTER,  Address offset: 0x028 */
	__IO uint32_t COMP_AUTO_SHUT;                         /*!< COMPARATOR AUTOMATIC SHUT REGISTER,  Address offset: 0x02C */
	__IO uint32_t COMP_EXT_TRIG_CTRL;                     /*!< COMPARATOR EXTERNAL TRIGGER CONTROL REGISTER,  Address offset: 0x030 */
	__IO uint32_t COMP_EXT_TRIG_TIMER_SEL;                /*!< COMPARATOR EXTERNAL TRIGGER TIMER SELECT REGISTER,  Address offset: 0x034 */
	__IO uint32_t COMP_RST_LIST;                          /*!< COMPARATOR RESET CHANNEL LIST REGISTER,  Address offset: 0x038 */
	__IO uint32_t COMP_AUTOSW_EN;                         /*!< COMPARATOR AUTOMATIC CHANNEL SWITCH ENABLE REGISTER,  Address offset: 0x03C */
	__IO uint32_t COMP_EN_TRIG;                           /*!< COMPARATOR ENABLE TRIGGER REGISTER,  Address offset: 0x040 */
	__IO uint32_t COMP_CTRL_CNT;                          /*!< COMPARATOR EXTERNAL WAKE SHUT COUNT REGISTER,  Address offset: 0x044 */
	__IO uint32_t COMP_LPSD;                              /*!< COMPARATOR ANALOG REGISTER,  Address offset: 0x048 */
	__IO uint32_t COMP_CONTINUOUS_CTRL_0;                 /*!< COMPARATOR CONTINUOUS CONTROL REGISTER 0 Register,  Address offset: 0x04C */
	__IO uint32_t COMP_CONTINUOUS_CTRL_1;                 /*!< COMPARATOR CONTINUOUS CONTROL REGISTER 1 Register,  Address offset: 0x050 */
	__IO uint32_t COMP_DUMMY;                             /*!< COMPARATOR DUMMY REGISTER,  Address offset: 0x054 */
} CMP_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup COMP_Exported_Types COMP Exported Types
  * @{
  */

/**
  * @brief	Comparator Channel Init Structure Definition
  */
typedef struct {
	u8 CMP_ChIndex;			/*!< Specifies the channel index */

	u8 CMP_Ref0;			/*!< Specifies the internal reference voltage0, the value can be 0~31,
									Vref0 = CMP_Ref0*0.1v */
	u8 CMP_Ref1;			/*!< Specifies the internal reference voltage1, the value can be 0~31,
									Vref1 = CMP_Ref1*0.1v */
	u8 CMP_WakeType;		/*!< Specifies this channel wakeup system or ADC when criterion matches.
									This parameter can be a value or combination of
									@ref COMP_Compare_Wakeup_Type */
	u8 CMP_WakeSysCtrl;		/*!< Specifies the criteria of when comparator channel should wakeup
									system, which can be a value of
									@ref COMP_Wakeup_Control */
	u8 CMP_WakeADCCtrl;		/*!< Specifies the criteria of when comparator channel should wakeup
									ADC, which can be a value of
									@ref COMP_Wakeup_Control */
} CMP_CHTypeDef;

/**
  * @brief Comparator Init Structure Definition
  */
typedef struct {

	CMP_CHTypeDef CMP_ChanCtrl[4];  /*!< Specifies the comparator channel control parameters */

} CMP_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup COMP_Exported_Constants COMP Exported Constants
  * @{
  */

/** @defgroup COMP_Channel
  * @{
  */
#define COMP_CH0						((u8)0x00)
#define COMP_CH1						((u8)0x01)
#define COMP_CH2						((u8)0x02)
#define COMP_CH3						((u8)0x03)
#define COMP_CH_NUM						(4)

#define IS_COMP_CHN_SEL(SEL)           (((SEL) == COMP_CH0) || \
										((SEL) == COMP_CH1) || \
										((SEL) == COMP_CH2) || \
										((SEL) == COMP_CH3))
/**
  * @}
  */

/** @defgroup COMP_Compare_Control
  * @{
  */
#define COMP_SMALLER_THAN_REF0			((u8)0x00)	/*!< Vin < Vref0 */
#define COMP_GREATER_THAN_REF1			((u8)0x01)	/*!< Vin >= Vref1 */
#define COMP_WITHIN_REF0_AND_REF1		((u8)0x02)	/*!< Vin > Vref0 &&  Vin < Vref1 */
/**
  * @}
  */

/** @defgroup COMP_Compare_Wakeup_Type
  * @{
  */
#define COMP_WK_SYS						((u8)0x02)
#define COMP_WK_ADC						((u8)0x01)
#define COMP_WK_NONE					((u8)0x00)
/**
  * @}
  */

/** @defgroup COMP_Channel_List_Shift
  * @{
  */
#define COMP_SHIFT_CHSW(x)				(4*x)
/**
  * @}
  */

/** @defgroup COMP_Channel_Status
  * @{
  */
#define COMP_SHIFT_CH_STS(x)			(2*x)
#define COMP_MASK_CH_STS(x)				(u32)(0x3 << COMP_SHIFT_CH_STS(x))
/**
  * @}
  */

/** @defgroup COMP_Interrupt_Type
  * @{
  */
#define COMP_IT_CTN_VS					((u32)0x00000004)
#define COMP_IT_CTN_VH					((u32)0x00000002)
#define COMP_IT_CTN_VL					((u32)0x00000001)

#define COMP_IT_CONFIG(IT)      ((u32)(((IT) & 0x7) << 28))

#define IS_COMP_IT(IT)        ((((IT) & (u32)0xFFFFFF8) == 0X0) && \
								(((IT) & (u32)0x7) != 0X0))
/**
  * @}
  */

/** @defgroup COMP_Wakeup_Control
  * @{
  */
#define COMP_SHIFT_WK_SYS_CTRL(x)			(17 + 3*x)
#define COMP_MASK_WK_SYS_CTRL(x)			(u32)(0x00000003 << COMP_SHIFT_WK_SYS_CTRL(x))
#define COMP_SHIFT_WK_SYS_EN(x)				(16 + 3*x)
#define COMP_WK_SYS_EN(x)					(u32)(0x00000001 << COMP_SHIFT_WK_SYS_EN(x))

#define COMP_SHIFT_WK_ADC_CTRL(x)			(1 + 3*x)
#define COMP_MASK_WK_ADC_CTRL(x)			(u32)(0x00000003 << COMP_SHIFT_WK_ADC_CTRL(x))
#define COMP_SHIFT_WK_ADC_EN(x)				(3*x)
#define COMP_WK_ADC_EN(x)					(u32)(0x00000001 << COMP_SHIFT_WK_ADC_EN(x))
/**
  * @}
  */

/** @defgroup COMP_ZCD_Source
  * @{
  */
#define COMP_ZCD_SRC_NONE					((u8)0x00)
#define COMP_ZCD_SRC_VHOUT					((u8)0x01)
#define COMP_ZCD_SRC_VLOUT					((u8)0x02)
#define COMP_ZCD_SRC_VSOUT					((u8)0x03)

#define IS_COMP_ZCD_SRC_TYPE(SEL)      (((SEL) == COMP_ZCD_SRC_NONE) || \
										((SEL) == COMP_ZCD_SRC_VHOUT) || \
										((SEL) == COMP_ZCD_SRC_VLOUT) || \
										((SEL) == COMP_ZCD_SRC_VSOUT))
/**
  * @}
  */

/** @defgroup COMP_Debounce_Control
  * @{
  */
#define COMP_DBC_VS     ((u8)0x0)
#define COMP_DBC_VH     ((u8)0x1)
#define COMP_DBC_VL     ((u8)0x2)
#define IS_COMP_DBC_TYPE(x)    (((x) == COMP_DBC_VS) || \
								((x) == COMP_DBC_VH) || \
								((x) == COMP_DBC_VL))

#define COMP_VS_DBC_CNT_MAX     ((u16)0x7FFF)
#define COMP_VH_DBC_CNT_MAX     ((u16)0x7FFF)
#define COMP_VL_DBC_CNT_MAX     ((u16)0x7FFF)

#define IS_COMP_VS_BC_CNT(x)    ((x) <= COMP_VS_DBC_CNT_MAX)
#define IS_COMP_VH_BC_CNT(x)    ((x) <= COMP_VH_DBC_CNT_MAX)
#define IS_COMP_VL_BC_CNT(x)    ((x) <= COMP_VL_DBC_CNT_MAX)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup COMP_Exported_Functions COMP Exported Functions
  * @{
  */

_LONG_CALL_ void CMP_StructInit(CMP_InitTypeDef *CMP_InitStruct);
_LONG_CALL_ void CMP_Init(CMP_InitTypeDef *CMP_InitStruct);
_LONG_CALL_ void CMP_Cmd(u32 NewState);
_LONG_CALL_ u32 CMP_Busy(void);
_LONG_CALL_ void CMP_INTConfig(u32 Cmp_IT, u32 NewState);
_LONG_CALL_ u32 CMP_GetISR(void);
_LONG_CALL_ void CMP_INTClearPendingBit(u32 Cmp_IT);
_LONG_CALL_ u32 CMP_GetCompStatus(u8 channel);
_LONG_CALL_ void CMP_ResetCSwList(void);
_LONG_CALL_ void CMP_AutoShutCmd(u32 NewState);
_LONG_CALL_ void CMP_AutoCSwCmd(u32 NewState);
_LONG_CALL_ void CMP_TimerTrigCmd(u8 Tim_Idx, u32 PeriodMs, u32 NewState);
_LONG_CALL_ void CMP_ContinousCmd(u32 NewState);
_LONG_CALL_ void CMP_HystCmd(u32 NewState);
_LONG_CALL_ void CMP_SetZcdSrc(u32 Src_Type);
_LONG_CALL_ void CMP_SetDbcCnt(u8 Dbc_Type, u32 Dbc_Cnt);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif
