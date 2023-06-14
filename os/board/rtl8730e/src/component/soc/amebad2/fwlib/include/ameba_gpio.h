#ifndef _AMEBAD2_GPIO_H_
#define _AMEBAD2_GPIO_H_

/** @addtogroup GPIO
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * GPIO:
  *		- Base Address: GPIO
  *		- Port number: support A/B two ports
  *		- Pin number: 0 ~ 31(portA),  0 ~ 31(portB)
  *		- IRQ: GPIO_IRQ
  *		- portA/B/C can be configured to interrupt mode
  *
  *****************************************************************************************
  * How to use GPIO in normal mode
  *****************************************************************************************
  * To use GPIO peripheral in normal mode, the following steps are mandatory:
  *
  *		1. Enable the GPIO interface clock using
  *			RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
  *
  *		2. Fill the GPIO_InitStruct with the desired parameters.
  *
  *		3. configure GPIO with the configuration(GPIO mode, pull up/down) of step2:
  *			GPIO_Init(&GPIO_InitStruct)
  *
  *		4. Read or write GPIO pin according to GPIO out/in mode using
  *			GPIO_ReadDataBit() or GPIO_WriteBit()
  *
  *****************************************************************************************
  * How to use GPIO in interrupt mode
  *****************************************************************************************
  * To use GPIO in interrupt mode, the following steps are mandatory:
  *
  *		1. Enable the GPIO interface clock using
  *			RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
  *
  *		2. Fill the GPIO_InitStruct with the desired parameters.
  *
  *		3. configure GPIO with the configuration(GPIO mode, pull up/down) of step2:
  *			GPIO_Init(&GPIO_InitStruct)
  *
  *		4. Register a user interrupt handler:
  *			GPIO_UserRegIrq
  *
  *		5. Configure interrupt mode(trigger, polarity, debounce):
  *			GPIO_INTMode()
  *
  *		6. Enable the interrupt of a specified pin:
  *			GPIO_INTConfig()
  *
  *		@note  PortA/B can configure interrupt mode.
  *
  *****************************************************************************************
  * How to use GPIO port (multiple GPIO pins)
  *****************************************************************************************
  * To use GPIO port, the following steps are mandatory:
  *
  *		1. Enable the GPIO interface clock using
  *			RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
  *
  *		2. Configure GPIO data direction(IN/OUT)
  *			GPIO_PortDirection()
  *
  *		3. Read or write GPIO pin according to GPIO out/in mode using
  *			GPIO_PortRead()
  *			GPIO_PortWrite()
  *
  *****************************************************************************************
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup GPIO
  * @brief GPIO driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup GPIO_Register_Definitions GPIO Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup GPIO_DR
 * @brief Portx data register
 * @{
 **/
#define GPIO_MASK_PORTx_DATA            ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Values written to this register are output on the I/O signals for Portx if the corresponding data direction bits for Portx are set to Output mode and the corresponding control bit for Portx is set to software mode. The value read back is equal to the last value written to this register. */
#define GPIO_PORTx_DATA(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_PORTx_DATA(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_DDR
 * @brief Portx data direction register
 * @{
 **/
#define GPIO_MASK_PORTx_DATA_DIRECTION  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Values written to this register independently control the direction of the corresponding data bit in Portx. The default direction can be configured as input or output after system reset through the GPIO_DFLT_DIR_x parameter. 0: Input (default) 1: Output */
#define GPIO_PORTx_DATA_DIRECTION(x)    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_PORTx_DATA_DIRECTION(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_EN
 * @brief PortA/B/C interrupt enable register
 * @{
 **/
#define GPIO_MASK_INT_EN                ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Allows each bit of Portx to be configured for interrupts. By default, the generation of interrupts is disabled. Whenever a 1 is written to a bit of this register, it configures the corresponding bit on Portx to become an interrupt; otherwise, Portx operates as a normal GPIO signal. Interrupts are disabled on the corresponding bits of Portx if the corresponding data direction register is set to Output or if Portx mode is set to Hardware. 0: Configure Portx bit as normal GPIO signal (default) 1: Configure Portx bit as interrupt */
#define GPIO_INT_EN(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_EN(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_MASK
 * @brief PortA/B/C interrupt mask register
 * @{
 **/
#define GPIO_MASK_INT_MASK              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Controls whether an interrupt on Portx can create an interrupt for the interrupt controller by not masking it. By default, all interrupts bits are unmasked. Whenever a 1 is written to a bit in this register, it masks the interrupt generation capability for this signal; otherwise interrupts are allowed through. The unmasked status can be read as well as the resultant status after masking. 0: Interrupt bits are unmasked (default) 1: Mask interrupt */
#define GPIO_INT_MASK(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_MASK(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_TYPE
 * @brief PortA interrupt trigger type register
 * @{
 **/
#define GPIO_MASK_INT_TYPE              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Controls the type of interrupt that can occur on Portx. Whenever a 0 is written to a bit of this register, it configures the interrupt type to be level-sensitive; otherwise, it is edge-sensitive. 0: Level-sensitive (default) 1: Edge-sensitive */
#define GPIO_INT_TYPE(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_TYPE(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_POLARITY
 * @brief PortA interrupt polarity register
 * @{
 **/
#define GPIO_MASK_INT_POLARITY          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Controls the polarity of edge or level sensitivity that can occur on input of Portx. Whenever a 0 is written to a bit of this register, it configures the interrupt type to falling-edge or active-low sensitive; otherwise, it is rising-edge or active-high sensitive. 0: Active-low (default) 1: Active-high */
#define GPIO_INT_POLARITY(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_POLARITY(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_STATUS
 * @brief PortA/B/C interrupt status register
 * @{
 **/
#define GPIO_MASK_INT_STATUS            ((u32)0xFFFFFFFF << 0)          /*!<R 0  Interrupt status of Portx. */
#define GPIO_INT_STATUS(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_STATUS(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_STATUS_RAW
 * @brief PortA/B/C raw interrupt status register
 * @{
 **/
#define GPIO_MASK_RAW_INT_STATUS        ((u32)0xFFFFFFFF << 0)          /*!<R 0  Raw interrupt status of Portx (premasking bits). */
#define GPIO_RAW_INT_STATUS(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_RAW_INT_STATUS(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_DEBOUNCE
 * @brief PortA/B/C interrupt debounce register
 * @{
 **/
#define GPIO_MASK_DEBOUNCE_EN           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Controls whether an external signal that is the source of an interrupt needs to be debounced to remove any spurious glitches. Writing a 1 to a bit in this register enables the debouncing circuitry. A signal must be valid for two periods of an external clock before it is internally processed. 0: No debounce (default) 1: Enable debounce This register is available only if Portx is configured to generate interrupts (GPIO_PORTx_INTR = Include (1)) and when the debounce logic is included (GPIO_DEBOUNCE = Include (1)). */
#define GPIO_DEBOUNCE_EN(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_DEBOUNCE_EN(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_EOI
 * @brief PortA/B/C edge type interrupt clear register
 * @{
 **/
#define GPIO_MASK_INT_EOI               ((u32)0xFFFFFFFF << 0)          /*!<WA0 0  Controls the clearing of edge type interrupts from Portx. When a 1 is written into a corresponding bit of this register, the interrupt is cleared. All interrupts are cleared when Portx is not configured for interrupts. 0: No interrupt clear (default) 1: Clear interrupt */
#define GPIO_INT_EOI(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_EOI(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_EXT_PORT
 * @brief External PortA/B/C data register
 * @{
 **/
#define GPIO_MASK_EXT_PORTx             ((u32)0xFFFFFFFF << 0)          /*!<R 0  When Portx is configured as Input, then reading this location reads the values on the signal. When the data direction of Portx is set as Output, reading this location reads the data register for Portx. */
#define GPIO_EXT_PORTx(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_EXT_PORTx(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_ITN_LS_SYNC
 * @brief PortA/B/C level interrupt synchronization
 * @{
 **/
#define GPIO_BIT_LEVEL_SYNC             ((u32)0x00000001 << 0)          /*!<R/W 0  Writing a 1 to this register results in all level-sensitive interrupts being synchronized to pclk_intr. 0: No synchronization to pclk_intr (default) 1: Synchronize to pclk_intr */
/** @} */

/** @defgroup GPIO_ID_CODE
 * @brief PortA/B/C GPIO ID code register
 * @{
 **/
#define GPIO_MASK_ID_CODE               ((u32)0xFFFFFFFF << 0)          /*!<R 32'h1  This register is available only if Portx/B/C is configured to generate interrupts (GPIO_PORTx_INTR = Include (1)) and interrupt detection is configured to generate on both rising and falling edges of external input signal (GPIO_INT_BOTH_EDGE=Include (1)). */
#define GPIO_ID_CODE(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_ID_CODE(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_INT_BOTHEDGE
 * @brief PortA/B/C both edge interrupt register
 * @{
 **/
#define GPIO_MASK_INT_BOTHEDGE          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Controls the edge types of interrupt that can occur on Portx. Whenever a particular bit is programmed to 1, it enables the generation of interrupt on both the rising edge and falling edge of an external input signal corresponding to that bit on PortA. The values programmed in the registers gpio_intype_level and gpio_int_polarity for this particular bit are not considered when the corresponding bit of this register is set to 1. Whenever a particular bit is programmed to 0, the interrupt type depends on the value of the corresponding bits in the gpio_inttype_level and gpio_int_polarity registers. 0: Disabled. Single edge sensitive 1: Enabled. Both edge sensitive */
#define GPIO_INT_BOTHEDGE(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_INT_BOTHEDGE(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_VER_ID_CODE
 * @brief PortA/B/C GPIO component version register
 * @{
 **/
#define GPIO_MASK_VER_ID_CODE           ((u32)0xFFFFFFFF << 0)          /*!<R 32'h3231302A  ASCII value for each number in the version, followed by *. For example 32_30_31_2A represents the version 2.01*. Reset Value: See the releases table in the Release Notes. */
#define GPIO_VER_ID_CODE(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GPIO_GET_VER_ID_CODE(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup GPIO_DB_DIV_CONFIG
 * @brief
 * @{
 **/
#define GPIO_MASK_EXT_DB_DIV_CNT        ((u32)0x0000007F << 0)          /*!<R/W 0  Debounce interrupt prescaler of 32.768KHz. de-bounce time = (EXT_DB_DIV_CNT +1) * 2*32μs. */
#define GPIO_EXT_DB_DIV_CNT(x)          ((u32)(((x) & 0x0000007F) << 0))
#define GPIO_GET_EXT_DB_DIV_CNT(x)      ((u32)(((x >> 0) & 0x0000007F)))
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Types GPIO Exported Types
  * @{
*****************************************************************************/

/** @brief GPIO port
*****************************************************************************/
typedef struct {

	__IO uint32_t GPIO_DR;                                /*!< PORTA DATA REGISTER */
	__IO uint32_t GPIO_DDR;                               /*!< PORTA DATA DIRECTION REGISTER */
	__IO uint32_t GPIO_CTL;                               /*!< PORTA DATA SOURCE REGISTER */
} GPIO_Port_TypeDef;





/**
 * @brief GPIO Register Declaration
 *****************************************************************************/
typedef struct {
	GPIO_Port_TypeDef PORT[1];                            /*!< GPIO IP have 1 ports,  Address offset: 0x000-0x008 */
	__IO uint32_t RSVD0[9];                               /*!<  Reserved,  Address offset:0xC-0x2C */
	__IO uint32_t GPIO_INT_EN;                            /*!< PORTA/B/C INTERRUPT ENABLE REGISTER,  Address offset: 0x030 */
	__IO uint32_t GPIO_INT_MASK;                          /*!< PORTA/B/C INTERRUPT MASK REGISTER,  Address offset: 0x034 */
	__IO uint32_t GPIO_INT_TYPE;                          /*!< PORTA INTERRUPT TRIGGER TYPE REGISTER,  Address offset: 0x038 */
	__IO uint32_t GPIO_INT_POLARITY;                      /*!< PORTA INTERRUPT POLARITY REGISTER,  Address offset: 0x03C */
	__I  uint32_t GPIO_INT_STATUS;                        /*!< PORTA/B/C INTERRUPT STATUS REGISTER,  Address offset: 0x040 */
	__I  uint32_t GPIO_INT_STATUS_RAW;                    /*!< PORTA/B/C RAW INTERRUPT STATUS REGISTER,  Address offset: 0x044 */
	__IO uint32_t GPIO_DEBOUNCE;                          /*!< PORTA/B/C INTERRUPT DEBOUNCE REGISTER,  Address offset: 0x048 */
	__O  uint32_t GPIO_INT_EOI;                           /*!< PORTA/B/C EDGE TYPE INTERRUPT CLEAR REGISTER,  Address offset: 0x04C */
	__I  uint32_t GPIO_EXT_PORT[1];                       /*!< EXTERNAL PORTA/B/C DATA REGISTER,  Address offset: 0x050-0x050 */
	__IO uint32_t RSVD1[3];                               /*!<  Reserved,  Address offset:0x54-0x5C */
	__IO uint32_t GPIO_ITN_LS_SYNC;                       /*!< PORTA/B/C LEVEL INTERRUPT SYNCHRONIZATION Register,  Address offset: 0x060 */
	__I  uint32_t GPIO_ID_CODE;                           /*!< PORTA/B/C GPIO ID CODE REGISTER,  Address offset: 0x064 */
	__IO uint32_t GPIO_INT_BOTHEDGE;                      /*!< PORTA/B/C BOTH EDGE INTERRUPT REGISTER,  Address offset: 0x068 */
	__I  uint32_t GPIO_VER_ID_CODE;                       /*!< PORTA/B/C GPIO COMPONENT VERSION REGISTER,  Address offset: 0x06C */
	__IO uint32_t RSVD2[2];                               /*!<  Reserved,  Address offset:0x70-0x74 */
	__IO uint32_t GPIO_DB_DIV_CONFIG;                     /*!<  Register,  Address offset: 0x078 */
} GPIO_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup GPIO_Exported_Types GPIO Exported Types
  * @{
  */

/**
  * @brief  GPIO Init Structure Definition
  */
typedef struct {
	u32 GPIO_Mode;			/*!< Specifies the operating mode for the selected pins.
								This parameter can be a value of @ref GPIO_Mode */

	u32 GPIO_PuPd;			/*!< Specifies the operating Pull-up/Pull down for the selected pins.
								This parameter can be a value of @ref GPIO_Pull_Type */

	u32 GPIO_ITTrigger;		/*!< Specifies interrupt mode is level or edge trigger
								This parameter can be a value of @ref GPIO_INT_Trigger_Type */

	u32 GPIO_ITPolarity;	/*!< Specifies interrupt mode is high or low active trigger
								This parameter can be a value of @ref GPIO_INT_Polarity_Type */

	u32 GPIO_ITDebounce;	/*!< Specifies enable or disable de-bounce for interrupt
								This parameter can be a value of @ref GPIO_INT_Debounce_Type*/

	u32 GPIO_Pin;			/*!< Specifies the selected pins.
								This parameter contains two parts: Pin[7:5]: port number; Pin[4:0]: pin number */
} GPIO_InitTypeDef;

/**
  * @brief  GPIO IRQ Function Definition
  */
typedef void (*GPIO_IRQ_FUN)(VOID *Data, u32 Id);

/**
  * @brief  GPIO USER IRQ Function Definition
  */
typedef void (*GPIO_USER_IRQ_FUN)(u32 Id);

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup GPIO_Exported_Constants GPIO Exported Constants
  * @{
  */

/** @defgroup GPIO_Mode
  * @{
  */
#define GPIO_Mode_IN			0x0 /*!< GPIO Input Mode */
#define GPIO_Mode_OUT			0x1 /*!< GPIO Output Mode */
#define GPIO_Mode_INT			0x2 /*!< GPIO Interrupt Mode */
/**
  * @}
  */

/** @defgroup GPIO_Pull_Type
  * @{
  */
#define GPIO_PuPd_NOPULL		0x0 /*!< GPIO Interrnal HIGHZ */
#define GPIO_PuPd_DOWN			0x1 /*!< GPIO Interrnal Pull DOWN */
#define GPIO_PuPd_UP			0x2 /*!< GPIO Interrnal Pull UP */
#define GPIO_PuPd_SHUTDOWN		0x3 /*!< GPIO Interrnal PAD shutdown */
/**
  * @}
  */

/** @defgroup GPIO_INT_Trigger_Type
  * @{
  */
#define GPIO_INT_Trigger_LEVEL		0x0 /*!< This interrupt is level trigger */
#define GPIO_INT_Trigger_EDGE		0x1 /*!< This interrupt is edge trigger */
#define GPIO_INT_Trigger_BOTHEDGE	0x2 /*!< This interrupt is both-edge trigger */

#define IS_GPIOIT_LEVEL_TYPE(TYPE)		(((TYPE) == GPIO_INT_Trigger_LEVEL) || \
										((TYPE) == GPIO_INT_Trigger_EDGE) || \
										((TYPE) == GPIO_INT_Trigger_BOTHEDGE))
/**
  * @}
  */

/** @defgroup GPIO_INT_Polarity_Type
  * @{
  */
#define GPIO_INT_POLARITY_ACTIVE_LOW		0x0 /*!< Setting interrupt to low active: falling edge or low level */
#define GPIO_INT_POLARITY_ACTIVE_HIGH		0x1 /*!< Setting interrupt to high active: rising edge or high level */

#define IS_GPIOIT_POLARITY_TYPE(TYPE)		(((TYPE) == GPIO_INT_POLARITY_ACTIVE_LOW) || \
											((TYPE) == GPIO_INT_POLARITY_ACTIVE_HIGH))
/**
  * @}
  */

/** @defgroup GPIO_INT_Debounce_Type
  * @{
  */
#define GPIO_INT_DEBOUNCE_DISABLE			0x0 /*!< Disable interrupt debounce */
#define GPIO_INT_DEBOUNCE_ENABLE			0x1 /*!< Enable interrupt debounce */

#define IS_GPIOIT_DEBOUNCE_TYPE(TYPE)		(((TYPE) == GPIO_INT_DEBOUNCE_DISABLE) || \
											((TYPE) == GPIO_INT_DEBOUNCE_ENABLE))
/**
  * @}
  */

/** @defgroup GPIO_Pin_State
  * @{
  */
#define GPIO_PIN_LOW			0x0 /*!< Pin state is low */
#define GPIO_PIN_HIGH			0x1 /*!< Pin state is high */
/**
  * @}
  */

/** @defgroup GPIO_Port_Type
  * @{
  */
#define GPIO_PORT_A				0x0 /*!< Port number A */
#define GPIO_PORT_B				0x1 /*!< Port number B */
#define GPIO_PORT_C				0x2 /*!< Port number C */

#define IS_GPIO_PORT_NUM(PORT)	((PORT) == GPIO_PORT_A || \
								(PORT) == GPIO_PORT_B || \
								(PORT) == GPIO_PORT_C)
/**
  * @}
  */

/** @defgroup GPIO_IRQ_Event
  * @{
  */
#define HAL_IRQ_NONE			0x0 /*!< No interrupt event  */
#define HAL_IRQ_RISE			0x1 /*!< Rising edge or high level interrupt event */
#define HAL_IRQ_FALL			0x2 /*!< Falling edge or low level interrupt event */
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup GPIO_Exported_Functions GPIO Exported Functions
  * @{
  */

_LONG_CALL_ void GPIO_WriteBit(u32 GPIO_Pin, u32 BitVal);
_LONG_CALL_ u32 GPIO_ReadDataBit(u32 GPIO_Pin);
_LONG_CALL_ void GPIO_DeInit(u32 GPIO_Pin);
_LONG_CALL_ void GPIO_UserRegIrq(u32 GPIO_Pin, VOID *IrqHandler, VOID *IrqData);
_LONG_CALL_ void GPIO_INTMode(u32 GPIO_Pin, u32 NewState, u32 GPIO_ITTrigger, u32 GPIO_ITPolarity, u32 GPIO_ITDebounce);
_LONG_CALL_ void GPIO_INTConfig(u32 GPIO_Pin, u32 NewState);
_LONG_CALL_ void GPIO_Init(GPIO_InitTypeDef  *GPIO_InitStruct);
_LONG_CALL_ u32 GPIO_INTHandler(IN VOID *pData);
_LONG_CALL_ void GPIO_Direction(u32 GPIO_Pin, u32 data_direction);
_LONG_CALL_ u32 GPIO_PortRead(u32 GPIO_Port, u32 GPIO_Mask);
_LONG_CALL_ void GPIO_PortWrite(u32 GPIO_Port, u32 GPIO_Mask, u32 Port_State);
_LONG_CALL_ void GPIO_PortDirection(u32 GPIO_Port, u32 GPIO_Mask, u32 data_direction);
_LONG_CALL_ void GPIO_DebounceClock(u32 GPIO_Port, u32 DivideCount);
_LONG_CALL_ void GPIO_LevelSync(u32 GPIO_Port, u32 NewState);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
