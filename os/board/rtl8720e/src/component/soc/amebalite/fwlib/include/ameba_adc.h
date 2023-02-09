#ifndef _AMEBA_ADC_H_
#define _AMEBA_ADC_H_

/** @addtogroup ADC
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * ADC:
  * 	- Base Address: ADC
  * 	- Channel number: 10
  *		- CH7~9: internal ADC channel
  *		- CH0~6: external ADC channel
  * 	- Sample rate: configurable, frequency = CLK_ADC_CORE/divider, in which CLK_ADC_CORE is 2MHz, and can
  *		be divided by 12/16/32/64.
  * 	- Resolution: 12 bit
  * 	- Analog signal sampling: support 0 ~ 3.3V
  * 	- IRQ: ADC_IRQ
  * 	- Support Software-Trigger mode, Automatic mode, Timer-Trigger mode and Comparator-Assist mode.
  *
  *****************************************************************************************
  * How to use ADC in interrupt mode
  *****************************************************************************************
  * 	  To use ADC in interrupt mode, the following steps are mandatory:
  *
  *      1. Enable the ADC interface clock:
  *			RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);
  *
  *      2. Fill the ADC_InitStruct with the desired parameters.
  *
  *      3. Init Hardware with the parameters in ADC_InitStruct.
  *			ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
  *
  *      4. Clear ADC interrupt:
  *			ADC_INTClear()
  *
  *      5. To configure interrupts:
  *			ADC_INTConfig(ADC_IT, ENABLE)
  *
  *      6. Enable the NVIC and the corresponding interrupt using following function.
  *			-InterruptRegister(): register the ADC irq handler
  *			-InterruptEn(): Enable the NVIC interrupt and set irq priority
  *
  *      7. Activate the ADC:
  *			ADC_Cmd(ENABLE).
  *
  *      8. Enbale specified mode:
  *			ADC_SWTrigCmd(ENABLE)/ADC_AutoCSwCmd(ENABLE)/ADC_TimerTrigCmd(Tim_Idx, PeriodMs, ENABLE)
  *
  *      @note	1. If use ADC compare mode, call ADC_SetComp(ADC_channel, CompThresH, CompThresL, CompCtrl) to configure
  *                  2. If use ADC comparator-assist mode, you need to setup and enable comparator.
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup ADC
  * @brief ADC driver modules
  * @{
  */

/* AUTO_GEN_START */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup ADC_Register_Definitions ADC Register Definitions
 * @{
 *****************************************************************************/

/**
 * @defgroup ADC_CONF
 * @brief ADC Configuration Register
 * @{
 **/
#define ADC_BIT_ENABLE                      ((u32)0x00000001 << 9)          /*!<R/W 0h  This bit is for ADC enable control */
#define ADC_MASK_CVLIST_LEN                 ((u32)0x0000000F << 4)          /*!<R/W 0h  This field defines the number of items in the ADC conversion channel list. */
#define ADC_CVLIST_LEN(x)                   ((u32)(((x) & 0x0000000F) << 4))
#define ADC_GET_CVLIST_LEN(x)               ((u32)(((x >> 4) & 0x0000000F)))
#define ADC_MASK_OP_MOD                     ((u32)0x00000007 << 1)          /*!<R/W 0h  These bits selects ADC operation mode. 0: software trigger mode. 1:automatic mode. 2:timer-trigger mode. 3:comparator-assist mode */
#define ADC_OP_MOD(x)                       ((u32)(((x) & 0x00000007) << 1))
#define ADC_GET_OP_MOD(x)                   ((u32)(((x >> 1) & 0x00000007)))
#define ADC_BIT_REF_IN_SEL                  ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit selects ADC reference voltage input. 0: ADC's reference voltage is from internal source. 1:ADC's reference voltage is from external pin. */
/** @} */

/**
 * @defgroup ADC_IN_TYPE
 * @brief ADC Input Type Register
 * @{
 **/
#define ADC_BIT_IN_TYPE_CH9                 ((u32)0x00000001 << 9)          /*!<R 0  This bit reflects which kind of input type channel 9 is. Since channel 9 is a fixed internal channel. */
#define ADC_BIT_IN_TYPE_CH8                 ((u32)0x00000001 << 8)          /*!<R 0  This bit reflects which kind of input type channel 8 is. Since channel 8 is a fixed internal channel. */
#define ADC_BIT_IN_TYPE_CH7                 ((u32)0x00000001 << 7)          /*!<R 0  This bit reflects which kind of input type channel 7 is. Since channel 7 is a fixed internal channel. */
#define ADC_BIT_IN_TYPE_CH6                 ((u32)0x00000001 << 6)          /*!<R 0  This bit reflects which kind of input type channel 6 is. Since channel 6 is a fixed internal channel. */
#define ADC_BIT_IN_TYPE_CH5                 ((u32)0x00000001 << 5)          /*!<R/W 0  This bit controls channel 5 input type. If it's set to differential type(value of this bit is 1), channel 4 should also be set to differential type since they are hardware-fixed differential pair. 0: ADC channel 5 is single-end input. 1:ADC channel 5 is differential input which is paired with channel 4. */
#define ADC_BIT_IN_TYPE_CH4                 ((u32)0x00000001 << 4)          /*!<R/W 0  This bit controls channel 4 input type. If it's set to differential type(value of this bit is 1), channel 5 should also be set to differential type since they are hardware-fixed differential pair. 0: ADC channel 4 is single-end input. 1:ADC channel 4 is differential input which is paired with channel 5. */
#define ADC_BIT_IN_TYPE_CH3                 ((u32)0x00000001 << 3)          /*!<R/W 0  This bit controls channel 3 input type. If it's set to differential type(value of this bit is 1), channel 2 should also be set to differential type since they are hardware-fixed differential pair. 0: ADC channel 3 is single-end input. 1:ADC channel 3 is differential input which is paired with channel 2. */
#define ADC_BIT_IN_TYPE_CH2                 ((u32)0x00000001 << 2)          /*!<R/W 0  This bit controls channel 2 input type. If it's set to differential type(value of this bit is 1), channel 3 should also be set to differential type since they are hardware-fixed differential pair. 0: ADC channel 2 is single-end input. 1:ADC channel 2 is differential input which is paired with channel 3. */
#define ADC_BIT_IN_TYPE_CH1                 ((u32)0x00000001 << 1)          /*!<R/W 0  This bit controls channel 1 input type. If it's set to differential type(value of this bit is 1), channel 0 should also be set to differential type since they are hardware-fixed differential pair. 0: ADC channel 1 is single-end input. 1:ADC channel 1 is differential input which is paired with channel 0. */
#define ADC_BIT_IN_TYPE_CH0                 ((u32)0x00000001 << 0)          /*!<R/W 0  This bit controls channel 0 input type. If it's set to differential type(value of this bit is 1), channel 1 should also be set to differential type since they are hardware-fixed differential pair. 0: ADC channel 0 is single-end input. 1:ADC channel 0 is differential input which is paired with channel 1. */
/** @} */

/**
 * @defgroup ADC_COMP_TH_CHx
 * @brief ADC Channel x Comparison Threshold Register
 * @{
 **/
#define ADC_MASK_COMP_TH_H_CHx              ((u32)0x00000FFF << 16)          /*!<R/W 0h  This field defines the higher threshold of channel x for ADC automatic comparison. */
#define ADC_COMP_TH_H_CHx(x)                ((u32)(((x) & 0x00000FFF) << 16))
#define ADC_GET_COMP_TH_H_CHx(x)            ((u32)(((x >> 16) & 0x00000FFF)))
#define ADC_MASK_COMP_TH_L_CHx              ((u32)0x00000FFF << 0)          /*!<R/W 0h  This field defines the lower threshold of channel x for ADC automatic comparison. */
#define ADC_COMP_TH_L_CHx(x)                ((u32)(((x) & 0x00000FFF) << 0))
#define ADC_GET_COMP_TH_L_CHx(x)            ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**
 * @defgroup ADC_COMP_CTRL
 * @brief ADC Comparison Control Register
 * @{
 **/
#define ADC_MASK_COMP_CTRL_CH9              ((u32)0x00000003 << 18)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH9(x)                ((u32)(((x) & 0x00000003) << 18))
#define ADC_GET_COMP_CTRL_CH9(x)            ((u32)(((x >> 18) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH8              ((u32)0x00000003 << 16)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH8(x)                ((u32)(((x) & 0x00000003) << 16))
#define ADC_GET_COMP_CTRL_CH8(x)            ((u32)(((x >> 16) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH7              ((u32)0x00000003 << 14)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH7(x)                ((u32)(((x) & 0x00000003) << 14))
#define ADC_GET_COMP_CTRL_CH7(x)            ((u32)(((x >> 14) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH6              ((u32)0x00000003 << 12)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH6(x)                ((u32)(((x) & 0x00000003) << 12))
#define ADC_GET_COMP_CTRL_CH6(x)            ((u32)(((x >> 12) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH5              ((u32)0x00000003 << 10)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH5(x)                ((u32)(((x) & 0x00000003) << 10))
#define ADC_GET_COMP_CTRL_CH5(x)            ((u32)(((x >> 10) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH4              ((u32)0x00000003 << 8)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH4(x)                ((u32)(((x) & 0x00000003) << 8))
#define ADC_GET_COMP_CTRL_CH4(x)            ((u32)(((x >> 8) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH3              ((u32)0x00000003 << 6)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH3(x)                ((u32)(((x) & 0x00000003) << 6))
#define ADC_GET_COMP_CTRL_CH3(x)            ((u32)(((x >> 6) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH2              ((u32)0x00000003 << 4)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH2(x)                ((u32)(((x) & 0x00000003) << 4))
#define ADC_GET_COMP_CTRL_CH2(x)            ((u32)(((x >> 4) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH1              ((u32)0x00000003 << 2)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH1(x)                ((u32)(((x) & 0x00000003) << 2))
#define ADC_GET_COMP_CTRL_CH1(x)            ((u32)(((x >> 2) & 0x00000003)))
#define ADC_MASK_COMP_CTRL_CH0              ((u32)0x00000003 << 0)          /*!<R/W 0h  This field defines ADC channel comparison criteria which would notify system by interrupt when the criterion matches. Once a criterion matches, ADC should send an interrupt signal to system if the related interrupt mask is unmask. ADC also updates comparison results in reg_adc_comp_sts. 0: When Vin < bit_adc_comp_th_l_chx, comparison criterion matches. 1: When Vin > bit_adc_comp_th_h_chx, comparison criterion matches. 2: When Vin >= bit_adc_comp_th_l_chx and Vin <= bit_adc_comp_th_h_chx, comparison criterion matches. 3: When Vin < bit_adc_comp_th_l_chx or Vin > bit_adc_comp_th_h_chx, comparison criterion matches. */
#define ADC_COMP_CTRL_CH0(x)                ((u32)(((x) & 0x00000003) << 0))
#define ADC_GET_COMP_CTRL_CH0(x)            ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup ADC_COMP_STS
 * @brief ADC Comparison Status Register
 * @{
 **/
#define ADC_MASK_COMP_STS_CH9               ((u32)0x00000003 << 18)          /*!<R 0h  this field reflects the comparison result of channel 9 immediately */
#define ADC_COMP_STS_CH9(x)                 ((u32)(((x) & 0x00000003) << 18))
#define ADC_GET_COMP_STS_CH9(x)             ((u32)(((x >> 18) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH8               ((u32)0x00000003 << 16)          /*!<R 0h  this field reflects the comparison result of channel 8 immediately */
#define ADC_COMP_STS_CH8(x)                 ((u32)(((x) & 0x00000003) << 16))
#define ADC_GET_COMP_STS_CH8(x)             ((u32)(((x >> 16) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH7               ((u32)0x00000003 << 14)          /*!<R 0h  this field reflects the comparison result of channel 7 immediately */
#define ADC_COMP_STS_CH7(x)                 ((u32)(((x) & 0x00000003) << 14))
#define ADC_GET_COMP_STS_CH7(x)             ((u32)(((x >> 14) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH6               ((u32)0x00000003 << 12)          /*!<R 0h  this field reflects the comparison result of channel 6 immediately */
#define ADC_COMP_STS_CH6(x)                 ((u32)(((x) & 0x00000003) << 12))
#define ADC_GET_COMP_STS_CH6(x)             ((u32)(((x >> 12) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH5               ((u32)0x00000003 << 10)          /*!<R 0h  this field reflects the comparison result of channel 5 immediately */
#define ADC_COMP_STS_CH5(x)                 ((u32)(((x) & 0x00000003) << 10))
#define ADC_GET_COMP_STS_CH5(x)             ((u32)(((x >> 10) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH4               ((u32)0x00000003 << 8)          /*!<R 0h  this field reflects the comparison result of channel 4 immediately */
#define ADC_COMP_STS_CH4(x)                 ((u32)(((x) & 0x00000003) << 8))
#define ADC_GET_COMP_STS_CH4(x)             ((u32)(((x >> 8) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH3               ((u32)0x00000003 << 6)          /*!<R 0h  this field reflects the comparison result of channel 3 immediately */
#define ADC_COMP_STS_CH3(x)                 ((u32)(((x) & 0x00000003) << 6))
#define ADC_GET_COMP_STS_CH3(x)             ((u32)(((x >> 6) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH2               ((u32)0x00000003 << 4)          /*!<R 0h  this field reflects the comparison result of channel 2 immediately */
#define ADC_COMP_STS_CH2(x)                 ((u32)(((x) & 0x00000003) << 4))
#define ADC_GET_COMP_STS_CH2(x)             ((u32)(((x >> 4) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH1               ((u32)0x00000003 << 2)          /*!<R 0h  this field reflects the comparison result of channel 1 immediately */
#define ADC_COMP_STS_CH1(x)                 ((u32)(((x) & 0x00000003) << 2))
#define ADC_GET_COMP_STS_CH1(x)             ((u32)(((x >> 2) & 0x00000003)))
#define ADC_MASK_COMP_STS_CH0               ((u32)0x00000003 << 0)          /*!<R 0h  this field reflects the comparison result of channel 0 immediately */
#define ADC_COMP_STS_CH0(x)                 ((u32)(((x) & 0x00000003) << 0))
#define ADC_GET_COMP_STS_CH0(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup ADC_CHSW_LIST_0
 * @brief ADC Channel Switch List 0 Register
 * @{
 **/
#define ADC_MASK_CHSW_7                     ((u32)0x0000000F << 28)          /*!<R/W 0h  This field defines the number 7 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_7(x)                       ((u32)(((x) & 0x0000000F) << 28))
#define ADC_GET_CHSW_7(x)                   ((u32)(((x >> 28) & 0x0000000F)))
#define ADC_MASK_CHSW_6                     ((u32)0x0000000F << 24)          /*!<R/W 0h  This field defines the number 6 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_6(x)                       ((u32)(((x) & 0x0000000F) << 24))
#define ADC_GET_CHSW_6(x)                   ((u32)(((x >> 24) & 0x0000000F)))
#define ADC_MASK_CHSW_5                     ((u32)0x0000000F << 20)          /*!<R/W 0h  This field defines the number 5 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_5(x)                       ((u32)(((x) & 0x0000000F) << 20))
#define ADC_GET_CHSW_5(x)                   ((u32)(((x >> 20) & 0x0000000F)))
#define ADC_MASK_CHSW_4                     ((u32)0x0000000F << 16)          /*!<R/W 0h  This field defines the number 4 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_4(x)                       ((u32)(((x) & 0x0000000F) << 16))
#define ADC_GET_CHSW_4(x)                   ((u32)(((x >> 16) & 0x0000000F)))
#define ADC_MASK_CHSW_3                     ((u32)0x0000000F << 12)          /*!<R/W 0h  This field defines the number 3 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_3(x)                       ((u32)(((x) & 0x0000000F) << 12))
#define ADC_GET_CHSW_3(x)                   ((u32)(((x >> 12) & 0x0000000F)))
#define ADC_MASK_CHSW_2                     ((u32)0x0000000F << 8)          /*!<R/W 0h  This field defines the number 2 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_2(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define ADC_GET_CHSW_2(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define ADC_MASK_CHSW_1                     ((u32)0x0000000F << 4)          /*!<R/W 0h  This field defines the number 1 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_1(x)                       ((u32)(((x) & 0x0000000F) << 4))
#define ADC_GET_CHSW_1(x)                   ((u32)(((x >> 4) & 0x0000000F)))
#define ADC_MASK_CHSW_0                     ((u32)0x0000000F << 0)          /*!<R/W 0h  This field defines the number 0 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_0(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define ADC_GET_CHSW_0(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup ADC_CHSW_LIST_1
 * @brief ADC Channel Switch List 1 Register
 * @{
 **/
#define ADC_MASK_CHSW_15                    ((u32)0x0000000F << 28)          /*!<R/W 0h  This field defines the number 15 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_15(x)                      ((u32)(((x) & 0x0000000F) << 28))
#define ADC_GET_CHSW_15(x)                  ((u32)(((x >> 28) & 0x0000000F)))
#define ADC_MASK_CHSW_14                    ((u32)0x0000000F << 24)          /*!<R/W 0h  This field defines the number 14 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_14(x)                      ((u32)(((x) & 0x0000000F) << 24))
#define ADC_GET_CHSW_14(x)                  ((u32)(((x >> 24) & 0x0000000F)))
#define ADC_MASK_CHSW_13                    ((u32)0x0000000F << 20)          /*!<R/W 0h  This field defines the number 13 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_13(x)                      ((u32)(((x) & 0x0000000F) << 20))
#define ADC_GET_CHSW_13(x)                  ((u32)(((x >> 20) & 0x0000000F)))
#define ADC_MASK_CHSW_12                    ((u32)0x0000000F << 16)          /*!<R/W 0h  This field defines the number 12 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_12(x)                      ((u32)(((x) & 0x0000000F) << 16))
#define ADC_GET_CHSW_12(x)                  ((u32)(((x >> 16) & 0x0000000F)))
#define ADC_MASK_CHSW_11                    ((u32)0x0000000F << 12)          /*!<R/W 0h  This field defines the number 11 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_11(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define ADC_GET_CHSW_11(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define ADC_MASK_CHSW_10                    ((u32)0x0000000F << 8)          /*!<R/W 0h  This field defines the number 10 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_10(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define ADC_GET_CHSW_10(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define ADC_MASK_CHSW_9                     ((u32)0x0000000F << 4)          /*!<R/W 0h  This field defines the number 9 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_9(x)                       ((u32)(((x) & 0x0000000F) << 4))
#define ADC_GET_CHSW_9(x)                   ((u32)(((x >> 4) & 0x0000000F)))
#define ADC_MASK_CHSW_8                     ((u32)0x0000000F << 0)          /*!<R/W 0h  This field defines the number 8 channel to do a conversion when a conversion event takes place. */
#define ADC_CHSW_8(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define ADC_GET_CHSW_8(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup ADC_RST_LIST
 * @brief ADC Reset Channel List Register
 * @{
 **/
#define ADC_BIT_RST_LIST                    ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls to reset the channel switch to default state or not. Once this bit is set, the auto/manual channel switch goes to the first channel as a default state. 0: Do Not reset conversion list to default state. 1: Reset conversion list to default state. */
/** @} */

/**
 * @defgroup ADC_AUTO_CSW_CTRL
 * @brief ADC Automatic Channel Switch Control Register
 * @{
 **/
#define ADC_BIT_AUTO_CSW_EN                 ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls the automatic channel switch enabled or disabled. 0: Disable the automatic channel switch.If an automatic channel switch is in process, writing 0 will terminate the automatic channel switch. 1: Enable the automatic channel switch. When setting this bit, an automatic channel switch starts from the first channel in the channel switch list. */
/** @} */

/**
 * @defgroup ADC_SW_TRIG
 * @brief ADC Software Trigger Register
 * @{
 **/
#define ADC_BIT_SW_TRIG                     ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls the ADC module to do a conversion. Every time this bit is set to 1, ADC module would switch to a new channel and do one conversion. Therefore, this bit could be used as a start-convert event which is controlled by software. Every time a conversion is done, software MUST clear this bit manually. 0: disable the analog module and analog mux. 1: enable the analog module and analog mux. And then start a new channel conversion. */
/** @} */

/**
 * @defgroup ADC_LAST_CH
 * @brief ADC Last Channel Register
 * @{
 **/
#define ADC_MASK_LAST_CH                    ((u32)0x0000000F << 0)          /*!<R 0h  This field reflects the last used channel */
#define ADC_LAST_CH(x)                      ((u32)(((x) & 0x0000000F) << 0))
#define ADC_GET_LAST_CH(x)                  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup ADC_BUSY_STS
 * @brief ADC Busy Status Register
 * @{
 **/
#define ADC_BIT_FIFO_EMPTY                  ((u32)0x00000001 << 2)          /*!<R 1  0: FIFO in ADC not empty; 1: FIFO in ADC empty */
#define ADC_BIT_FIFO_FULL_REAL              ((u32)0x00000001 << 1)          /*!<R 0  0: FIFO in ADC not full real; 1: FIFO in ADC full real */
#define ADC_BIT_BUSY_STS                    ((u32)0x00000001 << 0)          /*!<R 0  This bit reflects the ADC is busy or not. If the ADC is processing a conversion of a channel, this bit remains 1 which indicates it's busy. Once a conversion is done, this bit becomes 0 which indicates it's ready to do another conversion. 0: The ADC is ready. 1: The ADC is busy. */
/** @} */

/**
 * @defgroup ADC_INTR_CTRL
 * @brief ADC Interrupt Control Register
 * @{
 **/
#define ADC_BIT_IT_COMPRE_CH9_EN            ((u32)0x00000001 << 28)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 9 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH8_EN            ((u32)0x00000001 << 27)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 8 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH7_EN            ((u32)0x00000001 << 26)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 7 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH6_EN            ((u32)0x00000001 << 25)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 6 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH5_EN            ((u32)0x00000001 << 24)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 5 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH4_EN            ((u32)0x00000001 << 23)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 4 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH3_EN            ((u32)0x00000001 << 22)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 3 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH2_EN            ((u32)0x00000001 << 21)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 2 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH1_EN            ((u32)0x00000001 << 20)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 1 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMPRE_CH0_EN            ((u32)0x00000001 << 19)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 0 comparison criterion matching is rising edge. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH9_EN              ((u32)0x00000001 << 17)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 9 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH8_EN              ((u32)0x00000001 << 16)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 8 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH7_EN              ((u32)0x00000001 << 15)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 7 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH6_EN              ((u32)0x00000001 << 14)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 6 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH5_EN              ((u32)0x00000001 << 13)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 5 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH4_EN              ((u32)0x00000001 << 12)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 4 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH3_EN              ((u32)0x00000001 << 11)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 3 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH2_EN              ((u32)0x00000001 << 10)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 2 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH1_EN              ((u32)0x00000001 << 9)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 1 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_COMP_CH0_EN              ((u32)0x00000001 << 8)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when channel 0 comparison criterion matches. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_ERR_EN                   ((u32)0x00000001 << 7)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when an error state takes place. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_DAT_OVW_EN               ((u32)0x00000001 << 6)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when a data overwritten situation takes place. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_FIFO_EMPTY_EN            ((u32)0x00000001 << 5)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when a FIFO empty state takes place. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_FIFO_OVER_EN             ((u32)0x00000001 << 4)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when a FIFO overflow state takes place. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_FIFO_FULL_EN             ((u32)0x00000001 << 3)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when a FIFO full state takes place. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_CHCV_END_EN              ((u32)0x00000001 << 2)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when a particular channel conversion is done. Please refer to reg_adc_it_chno_con 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_CV_END_EN                ((u32)0x00000001 << 1)          /*!<R/W 0h  This bit controls the interrupt is enabled or not every time a conversion is done. No matter ADC module is in what kind of operation mode. Every time a conversion is executed, ADC module would notify system if This bit is set. 0: This interrupt is disabled. 1: This interrupt is enabled. */
#define ADC_BIT_IT_CVLIST_END_EN            ((u32)0x00000001 << 0)          /*!<R/W 0h  This bit controls the interrupt is enabled or not when a conversion of the last channel in the list is done. For example, in automatic mode conversions would be executed continuously. Every time the last channel conversion is done, which means all channel conversions in the list is done, ADC could notify system if This bit is set. 0: This interrupt is disabled. 1: This interrupt is enabled. */
/** @} */

/**
 * @defgroup ADC_INTR_RAW_STS
 * @brief ADC Interrupt Raw Status Register
 * @{
 **/
#define ADC_BIT_IT_COMPRE_CH9_RAW_STS       ((u32)0x00000001 << 28)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH8_RAW_STS       ((u32)0x00000001 << 27)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH7_RAW_STS       ((u32)0x00000001 << 26)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH6_RAW_STS       ((u32)0x00000001 << 25)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH5_RAW_STS       ((u32)0x00000001 << 24)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH4_RAW_STS       ((u32)0x00000001 << 23)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH3_RAW_STS       ((u32)0x00000001 << 22)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH2_RAW_STS       ((u32)0x00000001 << 21)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH1_RAW_STS       ((u32)0x00000001 << 20)          /*!<R 0h   */
#define ADC_BIT_IT_COMPRE_CH0_RAW_STS       ((u32)0x00000001 << 19)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH9_RAW_STS         ((u32)0x00000001 << 17)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH8_RAW_STS         ((u32)0x00000001 << 16)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH7_RAW_STS         ((u32)0x00000001 << 15)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH6_RAW_STS         ((u32)0x00000001 << 14)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH5_RAW_STS         ((u32)0x00000001 << 13)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH4_RAW_STS         ((u32)0x00000001 << 12)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH3_RAW_STS         ((u32)0x00000001 << 11)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH2_RAW_STS         ((u32)0x00000001 << 10)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH1_RAW_STS         ((u32)0x00000001 << 9)          /*!<R 0h   */
#define ADC_BIT_IT_COMP_CH0_RAW_STS         ((u32)0x00000001 << 8)          /*!<R 0h   */
#define ADC_BIT_IT_ERR_RAW_STS              ((u32)0x00000001 << 7)          /*!<R 0h   */
#define ADC_BIT_IT_DAT_OVW_RAW_STS          ((u32)0x00000001 << 6)          /*!<R 0h   */
#define ADC_BIT_IT_FIFO_EMPTY_RAW_STS       ((u32)0x00000001 << 5)          /*!<R 1h   */
#define ADC_BIT_IT_FIFO_OVER_RAW_STS        ((u32)0x00000001 << 4)          /*!<R 0h   */
#define ADC_BIT_IT_FIFO_FULL_RAW_STS        ((u32)0x00000001 << 3)          /*!<R 0h   */
#define ADC_BIT_IT_CHCV_END_RAW_STS         ((u32)0x00000001 << 2)          /*!<R 0h   */
#define ADC_BIT_IT_CV_END_RAW_STS           ((u32)0x00000001 << 1)          /*!<R 0h   */
#define ADC_BIT_IT_CVLIST_END_RAW_STS       ((u32)0x00000001 << 0)          /*!<R 0h   */
/** @} */

/**
 * @defgroup ADC_INTR_STS
 * @brief ADC Interrupt Status Register
 * @{
 **/
#define ADC_BIT_IT_COMPRE_CH9_STS           ((u32)0x00000001 << 28)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH8_STS           ((u32)0x00000001 << 27)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH7_STS           ((u32)0x00000001 << 26)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH6_STS           ((u32)0x00000001 << 25)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH5_STS           ((u32)0x00000001 << 24)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH4_STS           ((u32)0x00000001 << 23)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH3_STS           ((u32)0x00000001 << 22)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH2_STS           ((u32)0x00000001 << 21)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH1_STS           ((u32)0x00000001 << 20)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMPRE_CH0_STS           ((u32)0x00000001 << 19)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH9_STS             ((u32)0x00000001 << 17)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH8_STS             ((u32)0x00000001 << 16)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH7_STS             ((u32)0x00000001 << 15)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH6_STS             ((u32)0x00000001 << 14)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH5_STS             ((u32)0x00000001 << 13)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH4_STS             ((u32)0x00000001 << 12)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH3_STS             ((u32)0x00000001 << 11)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH2_STS             ((u32)0x00000001 << 10)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH1_STS             ((u32)0x00000001 << 9)          /*!<RW1C 0h   */
#define ADC_BIT_IT_COMP_CH0_STS             ((u32)0x00000001 << 8)          /*!<RW1C 0h   */
#define ADC_BIT_IT_ERR_STS                  ((u32)0x00000001 << 7)          /*!<RW1C 0h   */
#define ADC_BIT_IT_DAT_OVW_STS              ((u32)0x00000001 << 6)          /*!<RW1C 0h   */
#define ADC_BIT_IT_FIFO_EMPTY_STS           ((u32)0x00000001 << 5)          /*!<RW1C 0h   */
#define ADC_BIT_FIFO_OVER_STS               ((u32)0x00000001 << 4)          /*!<RW1C 0h   */
#define ADC_BIT_FIFO_FULL_STS               ((u32)0x00000001 << 3)          /*!<RW1C 0h   */
#define ADC_BIT_CHCV_END_STS                ((u32)0x00000001 << 2)          /*!<RW1C 0h   */
#define ADC_BIT_CV_END_STS                  ((u32)0x00000001 << 1)          /*!<RW1C 0h   */
#define ADC_BIT_CVLIST_END_STS              ((u32)0x00000001 << 0)          /*!<RW1C 0h   */
/** @} */

/**
 * @defgroup ADC_IT_CHNO_CON
 * @brief ADC Interrupt Channel Number Configuration Register
 * @{
 **/
#define ADC_MASK_IT_CHNO_CON                ((u32)0x0000000F << 0)          /*!<R/W 0h  This field defines that ADC module should send interrupt signal to system when a conversion which of channel number is the same as this field. This register is only valid when bit_adc_it_chcv_end_en is set. bit_adc_it_chno_con: channel number */
#define ADC_IT_CHNO_CON(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define ADC_GET_IT_CHNO_CON(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**
 * @defgroup ADC_FULL_LVL
 * @brief ADC FIFO Full Threshold Register
 * @{
 **/
#define ADC_MASK_FULL_LVL                   ((u32)0x0000003F << 0)          /*!<R/W 0h  This field defines the FIFO full entry thredshold level and it's used for FIFO full interrupt. It should be set according to ADC FIFO depth. A value of 0 sets the threshold for 1 entry, and a value of 15 sets the threshold for 16 entries. */
#define ADC_FULL_LVL(x)                     ((u32)(((x) & 0x0000003F) << 0))
#define ADC_GET_FULL_LVL(x)                 ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**
 * @defgroup ADC_EXT_TRIG_TIMER_SEL
 * @brief ADC External Trigger Timer Select Register
 * @{
 **/
#define ADC_MASK_EXT_TRIG_TIMER_SEL         ((u32)0x00000007 << 0)          /*!<R/W 0h  These three bits defines which timer channel would be used to make ADC module do a conversion. 0: Timer module 0 is used as the comparator external trigger source. 1: Timer module 1 is used as the comparator external trigger source. .. 7: Timer module 7 is used as the comparator external trigger source. */
#define ADC_EXT_TRIG_TIMER_SEL(x)           ((u32)(((x) & 0x00000007) << 0))
#define ADC_GET_EXT_TRIG_TIMER_SEL(x)       ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**
 * @defgroup ADC_DATA_CHx
 * @brief ADC Channel x Data Register
 * @{
 **/
#define ADC_BIT_DAT_RDY_CHx                 ((u32)0x00000001 << 17)          /*!<R 0  This bit indicates that a conversion is done. Every time a conversion is done, this bit should be set to 1 and it would be cleared to 0 when a read operation of reg_adc_data_chx. */
#define ADC_BIT_DAT_OVW_CHx                 ((u32)0x00000001 << 16)          /*!<R 0  This bit indicates that there is a data overwritten situation in bit_adc_data_global takeing place. A data overwritten situation is that a former conversion data is NOT read before a new conversion is written to data field. It would be cleared to 0 when a read operation of reg_adc_data_chx. 0: There is no data overwritten case. 1: There is a data overwritten case. */
#define ADC_MASK_DATA_CHx                   ((u32)0x0000FFFF << 0)          /*!<R 0  This field contains the newsest conversion data of channel x [15:12]: which channel in channel list the data belongs to, only valid when bit_adc_ch_unmask=1 [11:0]: newest data */
#define ADC_DATA_CHx(x)                     ((u32)(((x) & 0x0000FFFF) << 0))
#define ADC_GET_DATA_CHx(x)                 ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup ADC_DATA_GLOBAL
 * @brief ADC Global Data Register
 * @{
 **/
#define ADC_MASK_DAT_CH                     ((u32)0x0000000F << 18)          /*!<R 0  This field indicates which channel data is procedd right now */
#define ADC_DAT_CH(x)                       ((u32)(((x) & 0x0000000F) << 18))
#define ADC_GET_DAT_CH(x)                   ((u32)(((x >> 18) & 0x0000000F)))
#define ADC_BIT_DAT_RDY_GLOBAL              ((u32)0x00000001 << 17)          /*!<R 0  This bit indicates that a conversion is done. Every time a conversion is done, this bit should be set to 1 and it would be cleared to 0 when a read operation of reg_adc_data_global */
#define ADC_BIT_DAT_OVW_GLOBAL              ((u32)0x00000001 << 16)          /*!<R 0  This bit indicates that there is a data overwritten situation in bit_adc_data_global takes place. A data overwritten situation is that a former conversion data is NOT read before a new conversion is written to data field. 0: there is no data overwritten case. 1: there is a data overwritten case. */
#define ADC_MASK_DATA_GLOBAL                ((u32)0x0000FFFF << 0)          /*!<R 0  This field contains the newsest conversion data of channel in the list. [15:12]: which channel in channel list the data belongs to, only valid when bit_adc_ch_unmask=1 [11:0]: newest data */
#define ADC_DATA_GLOBAL(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define ADC_GET_DATA_GLOBAL(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**
 * @defgroup ADC_FLR
 * @brief ADC FIFO Level Register
 * @{
 **/
#define ADC_MASK_FLR                        ((u32)0x0000001F << 0)          /*!<R 0  This field records the current ADC FIFO entry number. */
#define ADC_FLR(x)                          ((u32)(((x) & 0x0000001F) << 0))
#define ADC_GET_FLR(x)                      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**
 * @defgroup ADC_CLR_FIFO
 * @brief ADC Clear FIFO Register
 * @{
 **/
#define ADC_BIT_CLR_FIFO                    ((u32)0x00000001 << 0)          /*!<R/W 0  This bit is used to clear FIFO contents and makes it to the default status. Every time this bit is set to 1. FIFO would be cleared to the default status. Software should set this bit to 0 after FIFO is cleared. 0: No effect 1: Clear FIFO */
/** @} */

/**
 * @defgroup ADC_CLK_DIV
 * @brief ADC Clock Divider Register
 * @{
 **/
#define ADC_MASK_CLK_DIV                    ((u32)0x00000007 << 0)          /*!<R/W 3h  This field defines clock driver level of ADC module. A value of 0 is for clock divided by 2. A value of 1 is for clock divided by 4. A value of 2 is for clock divided by 8. A value of 3 is for clock divided by 12. A value of 4 is for clock divided by 16. A value of 5 is for clock divided by 32. A value of 6 is for clock divided by 64. */
#define ADC_CLK_DIV(x)                      ((u32)(((x) & 0x00000007) << 0))
#define ADC_GET_CLK_DIV(x)                  ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**
 * @defgroup ADC_DELAY_CNT
 * @brief ADC Delay Count Register
 * @{
 **/
#define ADC_BIT_CH_UNMASK                   ((u32)0x00000001 << 31)          /*!<R/W 0  Enable BIT_ADC_DAT_CHID */
#define ADC_MASK_CLK_SEL                    ((u32)0x00000003 << 8)          /*!<R/W 0  Just config to 00 */
#define ADC_CLK_SEL(x)                      ((u32)(((x) & 0x00000003) << 8))
#define ADC_GET_CLK_SEL(x)                  ((u32)(((x >> 8) & 0x00000003)))
#define ADC_MASK_DELAY                      ((u32)0x00000003 << 2)          /*!<R/W 0  This field defines channel switch data delay count. A value of 0 is for using default count of hardware. */
#define ADC_DELAY(x)                        ((u32)(((x) & 0x00000003) << 2))
#define ADC_GET_DELAY(x)                    ((u32)(((x >> 2) & 0x00000003)))
#define ADC_MASK_CTRL_DELAY                 ((u32)0x00000003 << 0)          /*!<R/W 0  This field defines channel switch ctrl delay count. A value of 0 is for using default count of hardware. */
#define ADC_CTRL_DELAY(x)                   ((u32)(((x) & 0x00000003) << 0))
#define ADC_GET_CTRL_DELAY(x)               ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**
 * @defgroup ADC_PWR_CTRL
 * @brief ADC Power Control Register
 * @{
 **/
#define ADC_BIT_DBG_EN                      ((u32)0x00000001 << 3)          /*!<R/W 0  Debug port enable */
#define ADC_MASK_DBG_SEL                    ((u32)0x00000003 << 1)          /*!<R/W 0  Debug port select */
#define ADC_DBG_SEL(x)                      ((u32)(((x) & 0x00000003) << 1))
#define ADC_GET_DBG_SEL(x)                  ((u32)(((x >> 1) & 0x00000003)))
#define ADC_BIT_PWR_CTRL                    ((u32)0x00000001 << 0)          /*!<R/W 0  This field is used to power control */
/** @} */

/**
 * @defgroup ADC_SW_TMODE
 * @brief ADC Software Test Mode
 * @{
 **/
#define ADC_BIT_SW_TMODE                    ((u32)0x00000001 << 0)          /*!<R/W 0  This bit is used to change the high 4 bits in FIFO. 0: normal mode, the high 4 bits will be null or the channel number 1: software test mode, the high 4 bits will be the counter number of sample points. (loop from 0 to 15) */
/** @} */
/** @} */

/* Exported types ------------------------------------------------------------*/

/**************************************************************************//**
 * @defgroup ADC_Exported_Types ADC Exported Types
 * @{
 * @brief ADC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t ADC_CONF;                               /*!< ADC CONFIGURATION REGISTER,  Address offset: 0x000 */
	__IO uint32_t ADC_IN_TYPE;                            /*!< ADC INPUT TYPE REGISTER,  Address offset: 0x004 */
	__IO uint32_t ADC_COMP_TH_CHx[10];                    /*!< ADC CHANNEL x COMPARISON THRESHOLD REGISTER,  Address offset: 0x008-0x02C */
	__IO uint32_t ADC_COMP_CTRL;                          /*!< ADC COMPARISON CONTROL REGISTER,  Address offset: 0x030 */
	__I  uint32_t ADC_COMP_STS;                           /*!< ADC COMPARISON STATUS REGISTER,  Address offset: 0x034 */
	__IO uint32_t ADC_CHSW_LIST_0;                        /*!< ADC CHANNEL SWITCH LIST 0 REGISTER,  Address offset: 0x038 */
	__IO uint32_t ADC_CHSW_LIST_1;                        /*!< ADC CHANNEL SWITCH LIST 1 REGISTER,  Address offset: 0x03C */
	__IO uint32_t ADC_RST_LIST;                           /*!< ADC RESET CHANNEL LIST REGISTER,  Address offset: 0x040 */
	__IO uint32_t ADC_AUTO_CSW_CTRL;                      /*!< ADC AUTOMATIC CHANNEL SWITCH CONTROL REGISTER,  Address offset: 0x044 */
	__IO uint32_t ADC_SW_TRIG;                            /*!< ADC SOFTWARE TRIGGER REGISTER,  Address offset: 0x048 */
	__I  uint32_t ADC_LAST_CH;                            /*!< ADC LAST CHANNEL REGISTER,  Address offset: 0x04C */
	__I  uint32_t ADC_BUSY_STS;                           /*!< ADC BUSY STATUS REGISTER,  Address offset: 0x050 */
	__IO uint32_t ADC_INTR_CTRL;                          /*!< ADC INTERRUPT CONTROL REGISTER,  Address offset: 0x054 */
	__I  uint32_t ADC_INTR_RAW_STS;                       /*!< ADC INTERRUPT RAW STATUS REGISTER,  Address offset: 0x058 */
	__IO uint32_t ADC_INTR_STS;                           /*!< ADC INTERRUPT STATUS REGISTER,  Address offset: 0x05C */
	__IO uint32_t ADC_IT_CHNO_CON;                        /*!< ADC INTERRUPT CHANNEL NUMBER CONFIGURATION REGISTER,  Address offset: 0x060 */
	__IO uint32_t ADC_FULL_LVL;                           /*!< ADC FIFO FULL THRESHOLD REGISTER,  Address offset: 0x064 */
	__IO uint32_t ADC_EXT_TRIG_TIMER_SEL;                 /*!< ADC EXTERNAL TRIGGER TIMER SELECT REGISTER,  Address offset: 0x068 */
	__I  uint32_t ADC_DATA_CHx[10];                       /*!< ADC CHANNEL x DATA REGISTER,  Address offset: 0x06C-0x090 */
	__I  uint32_t ADC_DATA_GLOBAL;                        /*!< ADC GLOBAL DATA REGISTER,  Address offset: 0x094 */
	__I  uint32_t ADC_FLR;                                /*!< ADC FIFO LEVEL REGISTER,  Address offset: 0x098 */
	__IO uint32_t ADC_CLR_FIFO;                           /*!< ADC CLEAR FIFO REGISTER,  Address offset: 0x09C */
	__IO uint32_t ADC_CLK_DIV;                            /*!< ADC CLOCK DIVIDER REGISTER,  Address offset: 0x0A0 */
	__IO uint32_t ADC_DELAY_CNT;                          /*!< ADC DELAY COUNT REGISTER,  Address offset: 0x0A4 */
	__IO uint32_t ADC_PWR_CTRL;                           /*!< ADC POWER CONTROL REGISTER,  Address offset: 0x0A8 */
	__IO uint32_t ADC_SW_TMODE;                           /*!< ADC SOFTWARE TEST MODE Register,  Address offset: 0x0AC */
	__IO uint32_t ADC_DUMMY;                              /*!< ADC DUMMY REGISTER,  Address offset: 0x0B0 */
} ADC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup ADC_Exported_Types ADC Exported Types
  * @{
  */

/**
  * @brief  ADC Init Structure Definition
  */
typedef struct {
	u8 ADC_OpMode;				/*!< Specifies ADC operation mode.
									This parameter can be a value of @ref ADC_Operation_Mode */

	u8 ADC_CvlistLen;				/*!< The number of valid items in the ADC conversion channel list is (ADC_CvlistLen + 1).
									This parameter can be set to 0~15 */

	u8 ADC_Cvlist[16];				/*!< Specifies the ADC channel conversion order. Each member should be
									the channel index */

	u8 ADC_ClkDiv;				/*!< Specifies ADC clock divider.
									This parameter can be a value of @ref ADC_Clk_Divider */

	u8 ADC_RxThresholdLevel;		/*!< Specifies the receive FIFO threshold level.
							    		When the number of rx FIFO entries is greater than or equal to this
							     		value +1, the receive FIFO full interrupt is triggered. */

	u8  ADC_SpecialCh;			/*!< Specifies ADC particular channel. This parameter defines that ADC module
									should send interrupt signal to system when a conversion which of channel
									number is the same as this parameter. Default 0xFF means there is no need
									to set particular channel. */

	u32 ADC_ChanInType; 		/*!< Specifies CH0~5 input type. Default all channels are in single-end mode.
									If some channels need to be set to differential mode, use a value or
									combination of @ref ADC_CH_Input_Type. */

	u8 ADC_ChIDEn;				/*!< Specifies whether ADC enables BIT_ADC_DAT_CHID or not. */

} ADC_InitTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup ADC_Exported_Constants ADC Exported Constants
  * @{
  */

/** @defgroup ADC_Chn_Selection
  * @{
  */
#define ADC_CH0						((u8)0x00)
#define ADC_CH1						((u8)0x01)
#define ADC_CH2						((u8)0x02)
#define ADC_CH3						((u8)0x03)
#define ADC_CH4						((u8)0x04)
#define ADC_CH5						((u8)0x05)
#define ADC_CH6						((u8)0x06)
#define ADC_CH7						((u8)0x07)	/*!< ADC internal channel */
#define ADC_CH8						((u8)0x08)	/*!< ADC internal channel */
#define ADC_CH9						((u8)0x09)	/*!< ADC internal channel */
#define ADC_CH_NUM					(10)
#define ADC_GLOBAL					((u8)0xFF)

#define IS_ADC_CHN_SEL(SEL)			(((SEL) == ADC_CH0) || \
									((SEL) == ADC_CH1) || \
									((SEL) == ADC_CH2) || \
									((SEL) == ADC_CH3) || \
									((SEL) == ADC_CH4) || \
									((SEL) == ADC_CH5) || \
									((SEL) == ADC_CH6) || \
									((SEL) == ADC_CH7) || \
									((SEL) == ADC_CH8) || \
									((SEL) == ADC_CH9))
/**
  * @}
  */

/** @defgroup ADC_Clk_Divider
  * @{
  */
#define ADC_CLK_DIV_2				((u8)0x00)
#define ADC_CLK_DIV_4				((u8)0x01)
#define ADC_CLK_DIV_8				((u8)0x02)
#define ADC_CLK_DIV_12				((u8)0x03)
#define ADC_CLK_DIV_16				((u8)0x04)
#define ADC_CLK_DIV_32				((u8)0x05)
#define ADC_CLK_DIV_64				((u8)0x06)

#define IS_ADC_SAMPLE_CLK(CLK)		(((CLK) == ADC_CLK_DIV_2) || \
									((CLK) == ADC_CLK_DIV_4) || \
									((CLK) == ADC_CLK_DIV_8) || \
									((CLK) == ADC_CLK_DIV_12) || \
									((CLK) == ADC_CLK_DIV_16) || \
									((CLK) == ADC_CLK_DIV_32) || \
									((CLK) == ADC_CLK_DIV_64))
/**
  * @}
  */

/** @defgroup ADC_Operation_Mode
  * @{
  */
#define ADC_SW_TRI_MODE				((u8)0x00)	/*!< ADC software-trigger mode */
#define ADC_AUTO_MODE				((u8)0x01)	/*!< ADC automatic mode */
#define ADC_TIM_TRI_MODE			((u8)0x02)	/*!< ADC timer-trigger mode */
#define ADC_COMP_ASSIST_MODE		((u8)0x03)	/*!< ADC comparator-assist mode */

#define IS_ADC_MODE(mode)		(((mode) == ADC_SW_TRI_MODE) || \
								((mode) == ADC_AUTO_MODE) || \
								((mode) == ADC_TIM_TRI_MODE) || \
								((mode) == ADC_COMP_ASSIST_MODE))
/**
  * @}
  */

/** @defgroup ADC_CH_Input_Type
  * @{
  */
#define ADC_DIFFERENTIAL_CH(x)			((u32)0x00000001 << (x))
/**
  * @}
  */

/** @defgroup ADC_Compare_Control
  * @{
  */
#define ADC_COMP_SMALLER_THAN_THL			((u8)0x00)	/*!< Vin < ADC_COMP_TH_L */
#define ADC_COMP_GREATER_THAN_THH			((u8)0x01)	/*!< Vin > ADC_COMP_TH_H */
#define ADC_COMP_WITHIN_THL_AND_THH			((u8)0x02)	/*!< Vin >= ADC_COMP_TH_L &&  Vin <= ADC_COMP_TH_H */
#define ADC_COMP_OUTSIDE_THL_AND_THH		((u8)0x03)	/*!< Vin < ADC_COMP_TH_L  || Vin > ADC_COMP_TH_H */
#define IS_ADC_COMP_CRITERIA(rule)			(((rule) == ADC_COMP_SMALLER_THAN_THL) || \
											((rule) == ADC_COMP_GREATER_THAN_THH) || \
											((rule) == ADC_COMP_WITHIN_THL_AND_THH) || \
											((rule) == ADC_COMP_OUTSIDE_THL_AND_THH))
/**
  * @}
  */

/** @defgroup ADC_Compare_Threshold
  * @{
  */
#define IS_ADC_VALID_COMP_TH(x)				((x) < 0x1000)
/**
  * @}
  */

/** @defgroup ADC_Valid_Timer
  * @{
  */
#define IS_ADC_VALID_TIM(idx)				((idx) < 8)
/**
  * @}
  */

/** @defgroup ADC_Interrupt_Status
  * @{
  */
#define ADC_BIT_IT_COMPRE_ALL_STS			(ADC_BIT_IT_COMPRE_CH0_STS | \
											ADC_BIT_IT_COMPRE_CH1_STS | \
											ADC_BIT_IT_COMPRE_CH2_STS | \
											ADC_BIT_IT_COMPRE_CH3_STS | \
											ADC_BIT_IT_COMPRE_CH4_STS | \
											ADC_BIT_IT_COMPRE_CH5_STS | \
											ADC_BIT_IT_COMPRE_CH6_STS | \
											ADC_BIT_IT_COMPRE_CH7_STS | \
											ADC_BIT_IT_COMPRE_CH8_STS | \
											ADC_BIT_IT_COMPRE_CH9_STS)

#define ADC_BIT_IT_COMP_ALL_STS				(ADC_BIT_IT_COMP_CH0_STS | \
											ADC_BIT_IT_COMP_CH1_STS | \
											ADC_BIT_IT_COMP_CH2_STS | \
											ADC_BIT_IT_COMP_CH3_STS | \
											ADC_BIT_IT_COMP_CH4_STS | \
											ADC_BIT_IT_COMP_CH5_STS | \
											ADC_BIT_IT_COMP_CH6_STS | \
											ADC_BIT_IT_COMP_CH7_STS | \
											ADC_BIT_IT_COMP_CH8_STS | \
											ADC_BIT_IT_COMP_CH9_STS)

#define ADC_BIT_IT_ALL_STS					(ADC_BIT_IT_COMPRE_ALL_STS | \
											ADC_BIT_IT_COMP_ALL_STS | \
											ADC_BIT_IT_ERR_STS | \
											ADC_BIT_IT_DAT_OVW_STS |\
											ADC_BIT_IT_FIFO_EMPTY_STS |\
											ADC_BIT_FIFO_OVER_STS |\
											ADC_BIT_FIFO_FULL_STS |\
											ADC_BIT_CHCV_END_STS |\
											ADC_BIT_CV_END_STS |\
											ADC_BIT_CVLIST_END_STS)
/**
  * @}
  */

/** @defgroup ADC_CHSW_List
  * @{
  */
#define ADC_SHIFT_CHSW0(x)						(4*x)
#define ADC_SHIFT_CHSW1(x)						(4*(x - 8))
/**
  * @}
  */

/** @defgroup ADC_Comparison_Setting
  * @{
  */
#define ADC_SHIFT_COMP_CTRL_CH(x)				(2*x)
#define ADC_MASK_COMP_CTRL_CH(x)				(u32)(0x00000003 << ADC_SHIFT_COMP_CTRL_CH(x))

#define ADC_SHIFT_COMP_STS_CH(x)				(2*x)
#define ADC_MASK_COMP_STS_CH(x)					(u32)(0x00000003 << ADC_SHIFT_COMP_STS_CH(x))

#define ADC_IT_COMP_CH_EN(x)					((u32)0x00000001 << ((8+x)))
#define ADC_IT_COMPRE_CH_EN(x)					((u32)0x00000001 << ((19+x)))

/**
  * @}
  */

/** @defgroup ADC_Data_Setting
  * @{
  */
#define ADC_GET_DAT_CHID(x)						((u32)(((x >> 12) & 0x0000000F)))
#define ADC_CH_DATA(x)							((u32)((x) & 0x00000FFF))
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */

_LONG_CALL_ void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct);
_LONG_CALL_ void ADC_Init(ADC_InitTypeDef *ADC_InitStruct);
_LONG_CALL_ void ADC_Cmd(u32 NewState);
_LONG_CALL_ void ADC_INTConfig(u32 ADC_IT, u32 NewState);
_LONG_CALL_ void ADC_INTClear(void);
_LONG_CALL_ void ADC_INTClearPendingBits(u32 ADC_IT);
_LONG_CALL_ u32 ADC_GetISR(void);
_LONG_CALL_ u32 ADC_GetRawISR(void);
_LONG_CALL_ u32 ADC_GetCompStatus(u8 ADC_Channel);
_LONG_CALL_ u32 ADC_GetRxCount(void);
_LONG_CALL_ u32 ADC_GetLastChan(void);
_LONG_CALL_ void ADC_SetComp(u8 ADC_channel, u16 CompThresH, u16 CompThresL, u8 CompCtrl);
_LONG_CALL_ void ADC_ResetCSwList(void);
_LONG_CALL_ u32 ADC_Readable(void);
_LONG_CALL_ u16 ADC_Read(void);
_LONG_CALL_ void ADC_ReceiveBuf(u16 *pBuf, u32 len);
_LONG_CALL_ void ADC_ClearFIFO(void);
_LONG_CALL_ u32 ADC_GetStatus(void);
_LONG_CALL_ void ADC_SWTrigCmd(u32 NewState);
_LONG_CALL_ void ADC_AutoCSwCmd(u32 NewState);
_LONG_CALL_ void ADC_TimerTrigCmd(u8 Tim_Idx, u32 PeriodMs, u32 NewState);
_LONG_CALL_ void ADC_SetDmaEnable(u32 newState);
_LONG_CALL_ u32 ADC_RXGDMA_Init(GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData, IRQ_FUN CallbackFunc,
								u8 *pDataBuf, u32 DataLen);
_LONG_CALL_ s32 ADC_GetVoltage(u16 chan_data);
_LONG_CALL_ u32 ADC_GetInterR(void);

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
