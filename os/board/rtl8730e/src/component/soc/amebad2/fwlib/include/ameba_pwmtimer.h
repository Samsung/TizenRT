#ifndef _AMEBAD2_PWMTIMER_H_
#define _AMEBAD2_PWMTIMER_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PWMTIMER
  * @brief Timer driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup PWMTIMER_Register_Definitions PWMTIMER Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup EN
 * @brief TIM enable register
 * @{
 **/
#define TIM_BIT_CNT_EN       ((u32)0x00000001 << 16)          /*!<R 0x0  counter working status * 0: counter is stoppped * 1: counter is working */
#define TIM_BIT_CEN          ((u32)0x00000001 << 8)          /*!<R 0x0  counter run status * 0: counter is disabled * 1: counter is enabled */
#define TIM_BIT_CNT_STOP     ((u32)0x00000001 << 1)          /*!<WA0 0x0  disable the counter. Poll CNT_RUN to see the counter status. If CNT_RUN is 0, it means that the counter has been disabled internally. */
#define TIM_BIT_CNT_START    ((u32)0x00000001 << 0)          /*!<WA0 0x0  enable the counter. Poll CNT_RUN to see the counter status. If CNT_RUN is 1, it means that the counter has been enabled internally. */
/** @} */

/** @defgroup CR
 * @brief TIM control register
 * @{
 **/
#define TIM_MASK_ETP         ((u32)0x00000003 << 8)          /*!<R/W 0x0  external trigger polarity(TRGI) * 0: positive edge is active * 1: negative edge is active * 2 or 3: both edge is active Note: these bits are only valid in one-pulse mode. */
#define TIM_ETP(x)           ((u32)(((x) & 0x00000003) << 8))
#define TIM_GET_ETP(x)       ((u32)(((x >> 8) & 0x00000003)))
#define TIM_BIT_ARPE         ((u32)0x00000001 << 4)          /*!<R/W 0x0  enable auto-reload preloading * 0: TIM_ARR register is not buffered * 1: TIM_ARR register is buffered */
#define TIM_BIT_OPM          ((u32)0x00000001 << 3)          /*!<R/W 0x0  one-pulse mode * 0: counter is not stopped at UEV * 1: counter stops counting at next UEV */
#define TIM_BIT_URS          ((u32)0x00000001 << 2)          /*!<R/W 0x0  update request source * 0: counter overflow and setting the UG bit * 1: counter overflow */
#define TIM_BIT_UDIS         ((u32)0x00000001 << 1)          /*!<R/W 0x0  disable UEV. Shadow registers keep their values. Otherwise, buffered registers are then loaded with their preload values when UEV happen. */
/** @} */

/** @defgroup DIER
 * @brief TIM interupt enable register
 * @{
 **/
#define TIM_BIT_UIE5         ((u32)0x00000001 << 21)          /*!<R/W 0x0  enable channel 5 update interrupt */
#define TIM_BIT_UIE4         ((u32)0x00000001 << 20)          /*!<R/W 0x0  enable channel 4 update interrupt */
#define TIM_BIT_UIE3         ((u32)0x00000001 << 19)          /*!<R/W 0x0  enable channel 3 update interrupt */
#define TIM_BIT_UIE2         ((u32)0x00000001 << 18)          /*!<R/W 0x0  enable channel 2 update interrupt */
#define TIM_BIT_UIE1         ((u32)0x00000001 << 17)          /*!<R/W 0x0  enable channel 1 update interrupt */
#define TIM_BIT_UIE0         ((u32)0x00000001 << 16)          /*!<R/W 0x0  enable channel 0 update interrupt */
#define TIM_BIT_CC5IE        ((u32)0x00000001 << 6)          /*!<R/W 0x0  enable Capture/Compare 5 interrupt */
#define TIM_BIT_CC4IE        ((u32)0x00000001 << 5)          /*!<R/W 0x0  enable Capture/Compare 4 interrupt */
#define TIM_BIT_CC3IE        ((u32)0x00000001 << 4)          /*!<R/W 0x0  enable Capture/Compare 3 interrupt */
#define TIM_BIT_CC2IE        ((u32)0x00000001 << 3)          /*!<R/W 0x0  enable Capture/Compare 2 interrupt */
#define TIM_BIT_CC1IE        ((u32)0x00000001 << 2)          /*!<R/W 0x0  enable Capture/Compare 1 interrupt */
#define TIM_BIT_CC0IE        ((u32)0x00000001 << 1)          /*!<R/W 0x0  enable Capture/Compare 0 interrupt */
#define TIM_BIT_UIE          ((u32)0x00000001 << 0)          /*!<R/W 0x0  enable counter update interrupt */
/** @} */

/** @defgroup SR
 * @brief TIM status register
 * @{
 **/
#define TIM_BIT_UG_DONE      ((u32)0x00000001 << 31)          /*!<R 0x1  UG operation status This bit is cleared by hardware when the UG bit in the TIMx_EGR register is set. When the UG operation is done, hardware set this bit to '1'. So, software can poll this bit to see the UG operation status. */
#define TIM_BIT_UIF5         ((u32)0x00000001 << 21)          /*!<R/W1C/ES 0x0  refer to UIF0 description */
#define TIM_BIT_UIF4         ((u32)0x00000001 << 20)          /*!<R/W1C/ES 0x0  refer to UIF0 description */
#define TIM_BIT_UIF3         ((u32)0x00000001 << 19)          /*!<R/W1C/ES 0x0  refer to UIF0 description */
#define TIM_BIT_UIF2         ((u32)0x00000001 << 18)          /*!<R/W1C/ES 0x0  refer to UIF0 description */
#define TIM_BIT_UIF1         ((u32)0x00000001 << 17)          /*!<R/W1C/ES 0x0  refer to UIF0 description */
#define TIM_BIT_UIF0         ((u32)0x00000001 << 16)          /*!<R/W1C/ES 0x0  update interrupt flag of channel0 This flag is set by hardware when the UIE0 bit in the TIMx_EGR register is set, and update interrupt of channel0 happens. */
#define TIM_BIT_CC5IF        ((u32)0x00000001 << 6)          /*!<R/W1C/ES 0x0  refer to CC0IF description */
#define TIM_BIT_CC4IF        ((u32)0x00000001 << 5)          /*!<R/W1C/ES 0x0  refer to CC0IF description */
#define TIM_BIT_CC3IF        ((u32)0x00000001 << 4)          /*!<R/W1C/ES 0x0  refer to CC0IF description */
#define TIM_BIT_CC2IF        ((u32)0x00000001 << 3)          /*!<R/W1C/ES 0x0  refer to CC0IF description */
#define TIM_BIT_CC1IF        ((u32)0x00000001 << 2)          /*!<R/W1C/ES 0x0  refer to CC0IF description */
#define TIM_BIT_CC0IF        ((u32)0x00000001 << 1)          /*!<R/W1C/ES 0x0  Capture/Compare 0 interrupt flag channel CC0 configured as output: This flag is set by hardware when the counter TIM_CNT value matches the value of TIM_CCR0 register. It is cleared by software. Channel CC0 configured as input: This flag is set by hardware and the counter value has been captured in CCR0 field of TIM_CCR0 register when an active edge has been detected. It is cleared by software. */
#define TIM_BIT_UIF          ((u32)0x00000001 << 0)          /*!<R/W1C/ES 0x0  update interrupt flag */
/** @} */

/** @defgroup EGR
 * @brief TIM event generation register
 * @{
 **/
#define TIM_BIT_CC5G         ((u32)0x00000001 << 6)          /*!<WA0 0x0  refer to CC0G description */
#define TIM_BIT_CC4G         ((u32)0x00000001 << 5)          /*!<WA0 0x0  refer to CC0G description */
#define TIM_BIT_CC3G         ((u32)0x00000001 << 4)          /*!<WA0 0x0  refer to CC0G description */
#define TIM_BIT_CC2G         ((u32)0x00000001 << 3)          /*!<WA0 0x0  refer to CC0G description */
#define TIM_BIT_CC1G         ((u32)0x00000001 << 2)          /*!<WA0 0x0  refer to CC0G description */
#define TIM_BIT_CC0G         ((u32)0x00000001 << 1)          /*!<WA0 0x0  Capture/Compare 0 generation This bit is set by software and is automatically cleared by hardware. channel CC0 configured as output: The CC0IF flag is set, the corresponding interrupt or request is sent if enabled. Channel CC0 configured as input: The current value of counter is capture in CCR0 field of TIM_CCR0 register. The CC0IF flag is set, the corresponding interrupt or request is sent if enabled. */
#define TIM_BIT_UG           ((u32)0x00000001 << 0)          /*!<WA0 0x0  update generation. Re-initialize the counter and generate an update of registers. Note that the prescaler counter is cleared too.(anyway the prescaler ratio is not affected) */
/** @} */

/** @defgroup CNT
 * @brief TIM counter
 * @{
 **/
#define TIM_MASK_CNT         ((u32)0x0000FFFF << 0)          /*!<R 0x0  counter value */
#define TIM_CNT(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define TIM_GET_CNT(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup PSC
 * @brief TIM prescaler register
 * @{
 **/
#define TIM_MASK_PSC         ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  prescaler value. PSC contains the value to be loaded in the actual prescaler register at each UEV(including when the counter is cleared through UG bit of TIM_EGR register). */
#define TIM_PSC(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define TIM_GET_PSC(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup ARR
 * @brief TIM auto reload register
 * @{
 **/
#define TIM_MASK_ARR         ((u32)0x0000FFFF << 0)          /*!<R/W 0xFFFF  the value to be loaded in the actual auto-reload register. It can be preloaded by setting ARPE bit in TIM_CR. */
#define TIM_ARR(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define TIM_GET_ARR(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup CCRx
 * @brief TIM Capture/Compare register x
 * @{
 **/
#define TIM_BIT_OPM_DLx      ((u32)0x00000001 << 29)          /*!<R/W 0x0  One Pulse Mode default level 0: default level is 0 1: default level is 1 this bit takes effect only in PWM output mode's One-Pulse-Mode */
#define TIM_BIT_CCxM         ((u32)0x00000001 << 27)          /*!<R/W 0x0  CCx working mode * 0: PWM mode * 1: Input capture mode */
#define TIM_BIT_CCxP         ((u32)0x00000001 << 26)          /*!<R/W 0x0  CCx channel configured as output: * 0: OCx active high * 1: OCx ative low CCx channel configured as input: * 0: positive edge of TRGI is active for capture * 1: negative edge of TRGI is active for capture */
#define TIM_BIT_OCxPE        ((u32)0x00000001 << 25)          /*!<R/W 0x0  enable output capture x preload * 0: disable preload register on CCRx. CCRx can be written at any time, the new value is taken into account immediately. * 1: enable preload register on CCRx. Read/Write operations access the preload register. CCRx preload value is loaded in the active register at each UEV. */
#define TIM_BIT_CCxE         ((u32)0x00000001 << 24)          /*!<R/W 0x0  enable CCx */
#define TIM_MASK_CCRx        ((u32)0x0000FFFF << 0)          /*!<R/W/ES 0x0  Capture/Compare x value CCx channel configured as output: CCRx is the value to be loaded in the actual capture/compare x register(preload value). It is loaded permanently if the preload feature is not selected in the OCxPE bit.Else the preload value is copied in the active capture/compare x register when an UEV occurs.The active capture/compare x register contains the value to be compared to the counter TIM_CNT and signalled on OCx output. CCx channel configured as pulse mode 1: CCRx is the counter value transferred by the last input capture event(TRGI) */
#define TIM_CCRx(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define TIM_GET_CCRx(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup PSYNCx
 * @brief TIM Phase Synchronization register x
 * @{
 **/
#define TIM_BIT_SYNCENx      ((u32)0x00000001 << 27)          /*!<R/W 0x0  enable phase sync function in channel x. Phase sync takes effect only in output PWM mode. */
#define TIM_BIT_SYNCDIRx     ((u32)0x00000001 << 26)          /*!<R/W 0x0  phase sync direction to the reference counter: 0:delay to the reference counter 1:ahead to the reference counter */
#define TIM_BIT_SYNCPEx      ((u32)0x00000001 << 25)          /*!<R/W 0x0  enable sync phase parameter preload * 0: disable preload register on PSYNCx. PSYNCx can be written at any time, the new value is taken into account immediately. * 1: enable preload register on PSYNCx. Read/Write operations access the preload register. PSYNCx preload value is loaded in the active register at each UEV. */
#define TIM_MASK_SYNCPHASEx  ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  the deviation value of channelx to the reference counter */
#define TIM_SYNCPHASEx(x)    ((u32)(((x) & 0x0000FFFF) << 0))
#define TIM_GET_SYNCPHASEx(x) ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup PHASECNTx
 * @brief TIM phase counter register x
 * @{
 **/
#define TIM_MASK_CNTx        ((u32)0x0000FFFF << 0)          /*!<R 0x0  phase counter value of channelx */
#define TIM_CNTx(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define TIM_GET_CNTx(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup PWMTIMER_Exported_Types PWMTIMER Exported Types
  * @{
*****************************************************************************/


/**
 * @brief PWMTIMER Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t EN;                                     /*!< TIM ENABLE REGISTER,  Address offset: 0x00 */
	__IO uint32_t CR;                                     /*!< TIM CONTROL REGISTER,  Address offset: 0x04 */
	__IO uint32_t DIER;                                   /*!< TIM INTERUPT ENABLE REGISTER,  Address offset: 0x08 */
	__IO uint32_t SR;                                     /*!< TIM STATUS REGISTER,  Address offset: 0x0C */
	__O  uint32_t EGR;                                    /*!< TIM EVENT GENERATION REGISTER,  Address offset: 0x10 */
	__I  uint32_t CNT;                                    /*!< TIM COUNTER Register,  Address offset: 0x14 */
	__IO uint32_t PSC;                                    /*!< TIM PRESCALER REGISTER,  Address offset: 0x18 */
	__IO uint32_t ARR;                                    /*!< TIM AUTO-RELOAD REGISTER,  Address offset: 0x1C */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x20 */
	__IO uint32_t CCRx[6];                                /*!< TIM CAPTURE/COMPARE REGISTER x Register,  Address offset: 0x24-0x38 */
	__IO uint32_t PSYNCx[6];                              /*!< TIM PHASE SYNCHRONIZATION REGISTER x Register,  Address offset: 0x3C-0x50 */
	__I  uint32_t PHASECNTx[6];                           /*!< TIM PHASE COUNTER REGISTER x Register,  Address offset: 0x54-0x68 */
} RTIM_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup PWMTIMER_Exported_Types PWMTIMER Exported Types
  * @{
  */

/**
  * @brief  TIM Basic Init Structure Definition
  */
typedef struct {
	u32 TIM_Prescaler;		/*!< Specifies the prescaler value used to divide the TIM clock.
							This parameter can be a number between 0x0 and 0xFFFF, basic timer dont care */
	u32 TIM_Period;			/*!< Specifies the period value to be loaded into the active
							Auto-Reload Register at the next update event.
							This parameter is 16bits for TIM8-9, and 32bits for TIM0-TIM7
							you can get it from SourceClock & TIM_Prescaler */
	u32 TIM_UpdateEvent;		/*!< Specifies whether or not to enable update event(UEV).
								This parameter can be ENABLE or DISABLE. ENABLE means UEV Enable, DISABLE means UEV Disable*/
	u32 TIM_UpdateSource;		/*!< Specifies the update request source. This parameter can be
								TIM_UpdateSource_Overflow or TIM_UpdateSource_Global.
								TIM_UpdateSource_Overflow means counter overflow generates an update event(UEV).
								TIM_UpdateSource_Global means both counter overflow and setting the UG bit can generate UEV.*/
	u32 TIM_ARRProtection;	/*!< DISABLE or ENABLE, when ENABLE: period will update when cnt = 0(counter overflow, an UEV happens),
							or  period will update immediatly */

	u8	TIM_Idx;				/*!< 0~9 */

	u32	TIM_SecureTimer;		/*!< Specifies the secure attribute of this timer.
								This parameter is used in secure world of trustzone.*/
} RTIM_TimeBaseInitTypeDef;

/**
  * @brief  TIM Output Compare Init Structure Definition
  */
typedef struct {
	u32 TIM_CCMode;        /*!< Specifies the TIM8 mode. This parameter can be a value of TIM_CCMode_PWM or TIM_CCMode_Inputcapture */
	u32 TIM_CCPolarity;    /*!< Specifies the polarity. This parameter can be TIM_CCPolarity_High/TIM_CCPolarity_Low.
								If CCx channel is configured as output:
									TIM_CCPolarity_High means OCx active high.
									TIM_CCPolarity_Low means OCx active low.
								If CCx channel is configured as input:
									TIM_CCPolarity_High means positive edge of TRGI is active for capture.
									TIM_CCPolarity_Low means negative edge of TRGI is active for capture. */

	u32 TIM_OCProtection; 	/*!< Output Compare value update protection. TIM_OCPreload_Enable/TIM_OCPreload_Disable.
							TIM_OCPreload_Enable means duty cycle will update when UEV happens if write to CCRx field in TIMx_CCRX.
							TIM_OCPreload_Disable means duty cycle will update immediately if write to CCRx field in TIMx_CCRX.*/

	u32 TIM_OCPulse;        /*!< Specifies the output pulse value to be loaded into the CCRx Register, which decides the duty cycle.
							This parameter can be a number between 0x0000 and 0xFFFF */

	u32 TIM_ICPulseMode;	 /*!< Specifies the TIM9 mode, TIM_CCMode_PulseWidth or TIM_CCMode_PulseNumber */
} TIM_CCInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PWMTIMER_Exported_Constants PWMTIMER Exported Constants
  * @{
  */

/** @defgroup SEC_Control_Enable_Definition
  * @{
  */
#define TIM_BIT_SEC        ((u32)0x00000001 << 0)          /*!<R/W 0x0  Secure timer enable * 0: non-security timer * 1: security timer */
/**
  * @}
  */

/** @defgroup TIM_Type
  * @{
  */
#define IS_TIM_ALL_TIM(PERIPH) (((PERIPH) == TIM0) || \
                                   ((PERIPH) == TIM1) || \
                                   ((PERIPH) == TIM2) || \
                                   ((PERIPH) == TIM3) || \
                                   ((PERIPH) == TIM4) || \
                                   ((PERIPH) == TIM5) || \
                                   ((PERIPH) == TIM6) || \
                                   ((PERIPH) == TIM7) || \
                                   ((PERIPH) == TIM8) || \
                                   ((PERIPH) == TIM9) || \
                                   ((PERIPH) == TIM10) || \
                                   ((PERIPH) == TIM11) || \
                                   ((PERIPH) == TIM12) || \
                                   ((PERIPH) == TIM13) || \
                                   ((PERIPH) == TIM0_S) || \
                                   ((PERIPH) == TIM1_S) || \
                                   ((PERIPH) == TIM2_S) || \
                                   ((PERIPH) == TIM3_S) || \
                                   ((PERIPH) == TIM4_S) || \
                                   ((PERIPH) == TIM5_S) || \
                                   ((PERIPH) == TIM6_S) || \
                                   ((PERIPH) == TIM7_S)|| \
                                   ((PERIPH) == TIM8_S)|| \
                                   ((PERIPH) == TIM9_S)|| \
                                   ((PERIPH) == TIM10_S) || \
                                   ((PERIPH) == TIM11_S) || \
                                   ((PERIPH) == TIM12_S) || \
                                   ((PERIPH) == TIM13_S))

#define IS_TIM_SEC_TIM(PERIPH) (((PERIPH) == TIM0_S) || \
                                   ((PERIPH) == TIM1_S) || \
                                   ((PERIPH) == TIM2_S) || \
                                   ((PERIPH) == TIM3_S) || \
                                   ((PERIPH) == TIM4_S) || \
                                   ((PERIPH) == TIM5_S) || \
                                   ((PERIPH) == TIM6_S) || \
                                   ((PERIPH) == TIM7_S)|| \
                                   ((PERIPH) == TIM8_S)|| \
                                   ((PERIPH) == TIM9_S)|| \
                                   ((PERIPH) == TIM10_S)|| \
                                   ((PERIPH) == TIM11_S)|| \
                                   ((PERIPH) == TIM12_S)|| \
                                   ((PERIPH) == TIM13_S))

#define IS_TIM_ONE_PULSE_TIM(PERIPH) (((PERIPH) == TIM8) || \
                                   ((PERIPH) == TIM8_S))

#define IS_TIM_CCM_TIM(PERIPH) (((PERIPH) == TIM8) || \
                                   ((PERIPH) == TIM9) || \
                                   ((PERIPH) == TIM8_S) || \
                                   ((PERIPH) == TIM9_S))

#define IS_TIM_PWM_TIM(PERIPH) (((PERIPH) == TIM8) || \
                                   ((PERIPH) == TIM8_S))

#define IS_TIM_INPULSE_TIM(PERIPH) (((PERIPH) == TIM9) || \
                                   ((PERIPH) == TIM9_S))

#define IS_TIM_40M_TIM(PERIPH) (((PERIPH) == TIM8) || \
                                 ((PERIPH) == TIM9) || \
                                 ((PERIPH) == TIM8_S) || \
                                 ((PERIPH) == TIM9_S))
/**
  * @}
  */

/** @defgroup TIM_Channel
  * @note TIMx: 8~9
  * @{
  */
#define TIM_Channel_0                      ((u16)0x0000)
#define TIM_Channel_1                      ((u16)0x0001)
#define TIM_Channel_2                      ((u16)0x0002)
#define TIM_Channel_3                      ((u16)0x0003)
#define TIM_Channel_4                      ((u16)0x0004)
#define TIM_Channel_5                      ((u16)0x0005)

#define IS_TIM_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_0) || \
                                    ((CHANNEL) == TIM_Channel_1) || \
                                    ((CHANNEL) == TIM_Channel_2) || \
                                    ((CHANNEL) == TIM_Channel_3) || \
                                    ((CHANNEL) == TIM_Channel_4) || \
                                    ((CHANNEL) == TIM_Channel_5))
/**
  * @}
  */

/** @defgroup TIM_Prescaler
  * @{
  */
#define IS_TIM_PSC(VAL) (VAL <= 0xFFFF)
/**
  * @}
  */

/** @defgroup TIM_Control_Bit
  * @{
  */
#define TIM_OPMode_ETP_positive		((u32)0x00000000)
#define TIM_OPMode_ETP_negative		((u32)0x00000100)
#define TIM_OPMode_ETP_bothedge		((u32)0x00000200)
#define IS_TIM_OPM_ETP_MODE(MODE) (((MODE) == TIM_OPMode_ETP_positive) || \
                               ((MODE) == TIM_OPMode_ETP_negative) || \
                               ((MODE) == TIM_OPMode_ETP_bothedge))

#define TIM_OPMode_Single			((u32)0x00000008)
#define TIM_OPMode_Repetitive		((u32)0x00000000) /* repeative is PWM mode */
#define IS_TIM_OPM_MODE(MODE) (((MODE) == TIM_OPMode_Single) || \
                               ((MODE) == TIM_OPMode_Repetitive))

#define TIM_UpdateSource_Global		((u32)0x00000000) /*!< Source of update is the counter overflow or the setting of UG bit. */
#define TIM_UpdateSource_Overflow	((u32)0x00000004) /*!< Source of update is counter overflow. */
#define IS_TIM_UPDATE_SOURCE(SOURCE) (((SOURCE) == TIM_UpdateSource_Global) || \
                                      ((SOURCE) == TIM_UpdateSource_Overflow))
/**
  * @}
  */

/** @defgroup TIM_Interrupt_Control
  * @{
  */
#define TIM_IT_Update			((u32)0x00000001)
#define TIM_IT_CC0				((u32)0x00000002)
#define TIM_IT_CC1				((u32)0x00000004)
#define TIM_IT_CC2				((u32)0x00000008)
#define TIM_IT_CC3				((u32)0x00000010)
#define TIM_IT_CC4				((u32)0x00000020)
#define TIM_IT_CC5				((u32)0x00000040)
#define TIM_IT_UIE0				((u32)0x00010000)
#define TIM_IT_UIE1				((u32)0x00020000)
#define TIM_IT_UIE2				((u32)0x00040000)
#define TIM_IT_UIE3				((u32)0x00080000)
#define TIM_IT_UIE4				((u32)0x00100000)
#define TIM_IT_UIE5				((u32)0x00200000)
#define IS_TIM_IT(IT) ((((IT) & (u32)0xFFC0FF80) == 0x0000) && (((IT) & (u32)0x3F007F) != 0x0000))

#define IS_TIM_GET_IT(IT) (((IT) == TIM_IT_Update) || \
                              ((IT) == TIM_IT_CC0) || \
                              ((IT) == TIM_IT_CC1) || \
                              ((IT) == TIM_IT_CC2) || \
                              ((IT) == TIM_IT_CC3) || \
                              ((IT) == TIM_IT_CC4) || \
                              ((IT) == TIM_IT_CC5) || \
                              ((IT) == TIM_IT_UIE0) || \
                              ((IT) == TIM_IT_UIE1) || \
                              ((IT) == TIM_IT_UIE2) || \
                              ((IT) == TIM_IT_UIE3) || \
                              ((IT) == TIM_IT_UIE4) || \
                              ((IT) == TIM_IT_UIE5))
/**
  * @}
  */

/** @defgroup TIM_PSC_Reload_Mode
  * @{
  */
#define TIM_PSCReloadMode_Update		((u32)0x00000000)
#define TIM_PSCReloadMode_Immediate	((u32)0x00000001)
#define IS_TIM_PRESCALER_RELOAD(RELOAD) (((RELOAD) == TIM_PSCReloadMode_Update) || \
                                         ((RELOAD) == TIM_PSCReloadMode_Immediate))
/**
  * @}
  */

/** @defgroup TIM_Event_Source
  * @{
  */
#define TIM_EventSource_Update		((u32)0x00000001)
#define TIM_EventSource_CC0			((u32)0x00000002)
#define TIM_EventSource_CC1			((u32)0x00000004)
#define TIM_EventSource_CC2			((u32)0x00000008)
#define TIM_EventSource_CC3			((u32)0x00000010)
#define TIM_EventSource_CC4			((u32)0x00000020)
#define TIM_EventSource_CC5			((u32)0x00000040)
#define IS_LP_TIM_EVENT_SOURCE(SOURCE) ((((SOURCE) & 0xFFFFFFFE) == 0x0000) && \
						(((SOURCE) & 0x1) != 0x0000))
#define IS_HP_TIM_EVENT_SOURCE(SOURCE) ((((SOURCE) & 0xFFFFFF80) == 0x0000) && \
						(((SOURCE) & 0x7F) != 0x0000))
/**
  * @}
  */

/** @defgroup TIM_Capture_Compare_Control
  * @{
  */
#define TIM_CCx_Enable				((u32)0x01000000)
#define TIM_CCx_Disable				((u32)0x00000000)
#define IS_TIM_CCX(CCX) (((CCX) == TIM_CCx_Enable) || ((CCX) == TIM_CCx_Disable))

#define TIM_OCPreload_Enable		((u32)0x02000000)
#define TIM_OCPreload_Disable		((u32)0x00000000)
#define IS_TIM_OCPRELOAD_STATE(STATE) (((STATE) == TIM_OCPreload_Enable) || \
                                       ((STATE) == TIM_OCPreload_Disable))

#define TIM_CCPolarity_High			((u32)0x00000000) /*!< if input is set :  Positive edge of TRGI is active for capture */
#define TIM_CCPolarity_Low			((u32)0x04000000) /*!< if input is set :  negative edge of TRGI is active for capture */
#define IS_TIM_CC_POLARITY(POLARITY) (((POLARITY) == TIM_CCPolarity_High) || \
                                      ((POLARITY) == TIM_CCPolarity_Low))

/* TIM8 PWM or Inputcapture mode */
#define TIM_CCMode_PWM			((u32)0x00000000)
#define TIM_CCMode_Inputcapture		((u32)0x08000000)
#define IS_TIM_CC_MODE(MODE) (((MODE) == TIM_CCMode_PWM) || \
                              ((MODE) == TIM_CCMode_Inputcapture))

/* TIM9 pulse mode */
#define TIM_CCMode_PulseWidth		((u32)0x00000000)
#define TIM_CCMode_PulseNumber	((u32)0x10000000)
#define IS_TIM_CC_PULSEMODE(MODE) (((MODE) == TIM_CCMode_PulseWidth) || \
                              ((MODE) == TIM_CCMode_PulseNumber))

#define TIM_CCMode_CCR				((u32)0x0000FFFF)
#define IS_TIM_CC_PULSEWIDTH(Compare) ((Compare) <= TIM_CCMode_CCR)
/**
  * @}
  */

/** @defgroup TIM9_CC0_Pulse_Mode
  * @{
  */
#define TIM9_GET_CC0PM(x)                             ((u32)(((x) & 0x10000000) >> 28)) /*!<R/W 0x0  CC0 pulse mode * 0: pulse mode 0 * 1: pulse mode 1 */
#define TIM9_CC0PM(x)                                    ((u32)(((x) & 0x00000001) << 28))
/**
  * @}
  */

/** @defgroup TIMBasic_ARR
  * @{
  */
#define TIMBasic_GET_ARR(x)                            ((u32)(((x) & 0xFFFFFFFF) >> 0)) /*!<R/W 0xFFFFFFFF  the value to be loaded in the actual auto-reload register. It can be preloaded by setting ARPE bit in TIM_CR. */
#define TIMBasic_ARR(x)                                   ((u32)(((x) & 0xFFFFFFFF) << 0))
/**
  * @}
  */

/** @defgroup TIMBasic_CNT
  * @{
  */
#define TIMBasic_GET_CNT(x)                            ((u32)(((x) & 0xFFFFFFFF) >> 0)) /*!<R 0x0  counter value */
#define TIMBasic_CNT(x)                                   ((u32)(((x) & 0xFFFFFFFF) << 0))
/**
  * @}
  */

/** @defgroup TIM_Phase_Sync_Control
  * @{
  */

#define TIMPWM_PSync_Delay				((u32)0x00000000)
#define TIMPWM_PSync_Ahead				((u32)0x00000001)
#define IS_TIMPWM_PSync_Dir(DIR) (((DIR) == TIMPWM_PSync_Delay) || ((DIR) == TIMPWM_PSync_Ahead))

#define TIMPWM_PSyncPreload_Enable		((u32)0x00000000)
#define TIMPWM_PSyncPreload_Disable		((u32)0x00000001)
#define IS_TIMPWM_PPRELOAD_STATE(STATE) (((STATE) == TIMPWM_PSyncPreload_Enable) || \
                                       ((STATE) == TIMPWM_PSyncPreload_Disable))
/**
  * @}
  */

/** @defgroup TIM_Default_Level
  * @{
  */

#define TIMPWM_DefaultLevel_High				((u32)0x00000000)
#define TIMPWM_DefaultLevel_Low				((u32)0x00000001)
#define IS_TIMPWM_DefaultLevel(LEVEL) (((LEVEL) == TIMPWM_DefaultLevel_High) || ((LEVEL) == TIMPWM_DefaultLevel_Low))
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PWMTIMER_Exported_Functions PWMTIMER Exported Functions
  * @{
  */

/** @defgroup TimeBase_Management_Functions TimeBase Management Functions
* @{
*/
_LONG_CALL_ void RTIM_TimeBaseStructInit(RTIM_TimeBaseInitTypeDef *TIM_InitStruct);
_LONG_CALL_ void RTIM_TimeBaseInit(RTIM_TypeDef *TIMx, RTIM_TimeBaseInitTypeDef *TIM_InitStruct, IRQn_Type   IrqNum,
								   IRQ_FUN UserCB, u32 UserCBData);
_LONG_CALL_ void RTIM_Cmd(RTIM_TypeDef *TIMx, u32 NewState);
_LONG_CALL_ void RTIM_DeInit(RTIM_TypeDef *TIMx);
_LONG_CALL_ u32 RTIM_GetCount(RTIM_TypeDef *TIMx);
_LONG_CALL_ void RTIM_UpdateDisableConfig(RTIM_TypeDef *TIMx, u32 NewState);
_LONG_CALL_ void RTIM_ARRPreloadConfig(RTIM_TypeDef *TIMx, u32 NewState);
_LONG_CALL_ void RTIM_UpdateRequestConfig(RTIM_TypeDef *TIMx, u32 TIM_UpdateSource);
_LONG_CALL_ void RTIM_PrescalerConfig(RTIM_TypeDef *TIMx, u32 Prescaler, u32 TIM_PSCReloadMode);
_LONG_CALL_ void RTIM_GenerateEvent(RTIM_TypeDef *TIMx, u32 TIM_EventSource);
_LONG_CALL_ void RTIM_ChangePeriod(RTIM_TypeDef *TIMx, u32 Autoreload);
_LONG_CALL_ void RTIM_ChangePeriodImmediate(RTIM_TypeDef *TIMx, u32 Autoreload);
_LONG_CALL_ void RTIM_Reset(RTIM_TypeDef *TIMx);
/**
  * @}
  */

/** @defgroup Capture_Compare_Management_Functions Capture Compare Management Functions
* @{
*/
_LONG_CALL_ void RTIM_CCStructInit(TIM_CCInitTypeDef *TIM_CCInitStruct);
_LONG_CALL_ void RTIM_CCxInit(RTIM_TypeDef *TIMx, TIM_CCInitTypeDef *TIM_CCInitStruct, u16 TIM_Channel);
_LONG_CALL_ void RTIM_CCRxMode(RTIM_TypeDef *TIMx, u16 TIM_Channel, u32 TIM_CCMode);
_LONG_CALL_ void RTIM_CCRxSet(RTIM_TypeDef *TIMx, u32 Compare, u16 TIM_Channel);
_LONG_CALL_ u32 RTIM_CCRxGet(RTIM_TypeDef *TIMx, u16 TIM_Channel);
_LONG_CALL_ void RTIM_OCxPreloadConfig(RTIM_TypeDef *TIMx, u32 TIM_OCProtection, u16 TIM_Channel);
_LONG_CALL_ void RTIM_CCxPolarityConfig(RTIM_TypeDef *TIMx, u32 TIM_OCPolarity, u16 TIM_Channel);
_LONG_CALL_ void RTIM_CCxCmd(RTIM_TypeDef *TIMx, u16 TIM_Channel, u32 TIM_CCx);
_LONG_CALL_ void RTIM_SetOnePulseOutputMode(RTIM_TypeDef *TIMx, u32 TIM_OPMode, u32 TrigerPolarity);
_LONG_CALL_ void RTIM_SetOnePulseDefaultLevel(RTIM_TypeDef *TIMx, u16 TIM_Channel, u32 DefaultLevel);
/**
  * @}
  */

/** @defgroup Interrupt_Management_Functions Interrupt Management Functions
* @{
*/
_LONG_CALL_ void RTIM_INTConfig(RTIM_TypeDef *TIMx, u32 TIM_IT, u32 NewState);
_LONG_CALL_ void RTIM_INTClear(RTIM_TypeDef *TIMx);
_LONG_CALL_ void RTIM_INTClearPendingBit(RTIM_TypeDef *TIMx, u32 TIM_IT);
_LONG_CALL_ u32 RTIM_GetFlagStatus(RTIM_TypeDef *TIMx, u32 TIM_FLAG);
_LONG_CALL_ u32 RTIM_GetINTStatus(RTIM_TypeDef *TIMx, u32 TIM_IT);
/**
  * @}
  */

/** @defgroup Phase_Sync_Functions Phase Sync Functions
* @{
*/
_LONG_CALL_ void RTIM_PSyncxSet(RTIM_TypeDef *TIMx, u16 TIM_Channel, u32 TIM_SyncPhase);
_LONG_CALL_ u32 RTIM_PSyncxGet(RTIM_TypeDef *TIMx, u16 TIM_Channel);
_LONG_CALL_ void RTIM_PSyncxDir(RTIM_TypeDef *TIMx, u16 TIM_Channel, u8 TIM_SyncDir);
_LONG_CALL_ void RTIM_PSyncxPreloadConfig(RTIM_TypeDef *TIMx, u16 TIM_Channel, u32 TIM_PSyncProtection);
_LONG_CALL_ void RTIM_PSyncxCmd(RTIM_TypeDef *TIMx, u16 TIM_Channel, u8 NewState);
_LONG_CALL_ u32 RTIM_GetChannelCountx(RTIM_TypeDef *TIMx, u16 TIM_Channel);
/**
  * @}
  */

/**
  * @}
  */

/** @} */

/** @} */

/* Other Definitions -------------------------------------------------------------------*/

typedef struct {
	u32 PWM_CHANNEL;
	u32 KM0_CHAN_STATUS;
	u32 KM4_CHAN_STATUS;
} PWMCHANCFG_TypeDef;

#define TimerNum		14
#define PWM_CHAN_MAX	6

extern PWMCHANCFG_TypeDef  pwmchannel_config[];
extern int TIMx_irq[TimerNum];
extern RTIM_TypeDef *TIMx[TimerNum];
extern RTIM_TypeDef *TIMx_S[TimerNum];
extern u32 TIM_IT_CCx[PWM_CHAN_MAX];
extern u32 APBPeriph_TIMx[TimerNum];
extern u32 APBPeriph_TIMx_CLOCK[TimerNum];

#define TIMER_TICK_US					31
#define TIMER_TICK_US_X4				(4*1000000/32000) //32k clock, 31.25us every timer_tick

/* MANUAL_GEN_END */

#endif

