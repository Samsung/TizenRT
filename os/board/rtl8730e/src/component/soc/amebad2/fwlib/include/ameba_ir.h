#ifndef _AMEBAD2_IR_H_
#define _AMEBAD2_IR_H_

/** @addtogroup IR
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * IR:
  *		- Base Address: IR_DEV
  *		- IPclk: SCLK, normally is 100MHz
  *		- carrier clock: 25k~500k
  *		- work mode: Half Duplex
  *		- SocPs: SleepMode not support
  *		- accuracy:3%
  *		- IRQ: IR_IRQ
  *		- GDMA support: not support
  *
  *****************************************************************************************
  * PINMUX
  *****************************************************************************************
  */
//1TODO: -to be define:derek
/*
*****************************************************************************************
* How to use IR TX
*****************************************************************************************
* To use the IR tx function, the following steps are mandatory:
*
*		1. Enable peripheral clock and power:
*			RCC_PeriphClockCmd(APBPeriph_IR, APBPeriph_IR_CLOCK, ENABLE);
*
*		2. Configure the IR pinmux
*			Pinmux_Config(Pin_Num, PINMUX_FUNCTION_IR)
*
*		3. Set parameters, change some parameter if needed
*			void IR_StructInit(IR_InitTypeDef *IR_InitStruct)
*
*		4. Init hardware use step3 parameters.
*			void IR_Init(IR_InitTypeDef *IR_InitStruct)
*
*		5. Enable IRQ using following function if needed
*			IR_INTConfig(): IR IRQ Enable set
*			InterruptRegister(): register the IR irq handler
*
*		6. Send TX data to FIFO
*
*****************************************************************************************
* How to use IR RX
*****************************************************************************************
* To use the IR rx function, the following steps are mandatory:
*
*		1. Enable peripheral clock and power:
*			RCC_PeriphClockCmd(APBPeriph_IR, APBPeriph_IR_CLOCK, ENABLE);
*
*		2. Configure the IR pinmux
*			Pinmux_Config(Pin_Num, PINMUX_FUNCTION_IR)
*
*		3. Set parameters, change some parameter if needed
*			void IR_StructInit(IR_InitTypeDef *IR_InitStruct)
*
*		4. Init hardware use step3 parameters.
*			void IR_Init(IR_InitTypeDef *IR_InitStruct)
*
*      5. Enable IRQ using following function if needed
*			IR_INTConfig(): IR IRQ Enable set
*			InterruptRegister(): register the IR irq handler
*
*		6. Recieve Data from Rx fifo
* @endverbatim
*/

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup IR
  * @brief IR driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup IR_Register_Definitions IR Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup IR_CLK_DIV
 * @brief IR clock division register
 * @{
 **/
#define IR_MASK_CLK_DIV                      ((u32)0x00000FFF << 0)          /*!<R/W 0x0  IR_CLK = IO_CLK/(1 + IR_CLK_DIV) *Tx mode: divider number to generate IrDA modulation frequency. For example: sys_clk = 100MHz, modulation_freq = 455kHz, IR_DIV_NUM = (sys_clk/modulation_freq) - 1 *Rx mode: waveform sample clock. IR_DIV_NUM = (sys_clk/sample clock) - 1 For example: sample clock = 100MHz, IR_DIV_NUM = 0; sample clock = 50MHz, IR_DIV_NUM = 1 */
#define IR_CLK_DIV(x)                        ((u32)(((x) & 0x00000FFF) << 0))
#define IR_GET_CLK_DIV(x)                    ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup IR_TX_CONFIG
 * @brief IR Tx configuration register
 * @{
 **/
#define IR_BIT_MODE_SEL                      ((u32)0x00000001 << 31)          /*!<R/W 0x0  *0: Tx mode *1: Rx mode */
#define IR_BIT_TX_START                      ((u32)0x00000001 << 30)          /*!<R/W 0x0  *0: FSM stops at idle state. *1: FSM runs. */
#define IR_MASK_TX_DUTY_NUM                  ((u32)0x00000FFF << 16)          /*!<R/W 0x0  Duty cycle setting for modulation frequency For example: for 1/3 duty cycle, IR_DUTY_NUM = (IR_DIV_NUM+1)/3 Note: Set this value equals to IR_DIV_NUM to generate 100% duty waveform. */
#define IR_TX_DUTY_NUM(x)                    ((u32)(((x) & 0x00000FFF) << 16))
#define IR_GET_TX_DUTY_NUM(x)                ((u32)(((x >> 16) & 0x00000FFF)))
#define IR_BIT_TX_OUTPUT_INVERSE             ((u32)0x00000001 << 14)          /*!<R/W 0x0  *0: Not inverse active output *1: Inverse active output */
#define IR_BIT_TX_DE_INVERSE                 ((u32)0x00000001 << 13)          /*!<R/W 0x0  *0: Not inverse FIFO define *1: Inverse FIFO define */
#define IR_MASK_TX_FIFO_LEVEL_TH             ((u32)0x0000001F << 8)          /*!<R/W 0x0  Tx FIFO interrupt threshold is from 0 to 15. When Tx FIFO depth = < threshold value, interrupt is triggered. */
#define IR_TX_FIFO_LEVEL_TH(x)               ((u32)(((x) & 0x0000001F) << 8))
#define IR_GET_TX_FIFO_LEVEL_TH(x)           ((u32)(((x >> 8) & 0x0000001F)))
#define IR_BIT_TX_IDLE_STATE                 ((u32)0x00000001 << 6)          /*!<R/W 0x0  Tx output state in idle *0: Low *1: High */
#define IR_BIT_TX_FIFO_OVER_INT_MASK         ((u32)0x00000001 << 5)          /*!<R/W 0x0  Tx FIFO overflow interrupt *0: Unmask *1: Mask */
#define IR_BIT_TX_FIFO_OVER_INT_EN           ((u32)0x00000001 << 4)          /*!<R/W 0x0  Tx FIFO overflow interrupt *0: Disable *1: Enable */
#define IR_BIT_TX_FIFO_LEVEL_INT_MASK        ((u32)0x00000001 << 3)          /*!<R/W 0x0  Tx FIFO level interrupt *0: Unmask *1: Mask */
#define IR_BIT_TX_FIFO_EMPTY_INT_MASK        ((u32)0x00000001 << 2)          /*!<R/W 0x0  Tx FIFO empty interrupt *0: Unmask *1: Mask */
#define IR_BIT_TX_FIFO_LEVEL_INT_EN          ((u32)0x00000001 << 1)          /*!<R/W 0x0  Tx FIFO level interrupt When Tx FIFO offset <= threshold value, interrupt is triggered. *0: Disable *1: Enable */
#define IR_BIT_TX_FIFO_EMPTY_INT_EN          ((u32)0x00000001 << 0)          /*!<R/W 0x0  Tx FIFO empty interrupt *0: Disable *1: Enable */
/** @} */

/** @defgroup IR_TX_SR
 * @brief IR Tx FIFO and interrupt status register
 * @{
 **/
#define IR_BIT_TX_FIFO_EMPTY                 ((u32)0x00000001 << 15)          /*!<R 0x0  *0: Not empty *1: Empty */
#define IR_BIT_TX_FIFO_FULL                  ((u32)0x00000001 << 14)          /*!<R 0x0  *0: Not full *1: Full */
#define IR_MASK_TX_FIFO_OFFSET               ((u32)0x0000003F << 8)          /*!<R 0x0  Tx FIFO offset is from 0 to 32. Note: After Tx last packet, hardware can't clear Tx FIFO offset. */
#define IR_TX_FIFO_OFFSET(x)                 ((u32)(((x) & 0x0000003F) << 8))
#define IR_GET_TX_FIFO_OFFSET(x)             ((u32)(((x >> 8) & 0x0000003F)))
#define IR_BIT_TX_STATUS                     ((u32)0x00000001 << 4)          /*!<R 0x0  *0: Idle *1: Run */
#define IR_BIT_TX_FIFO_OVER_INT_STATUS       ((u32)0x00000001 << 2)          /*!<R 0x0  Tx FIFO overflow interrupt *0: Interrupt inactive *1: Interrupt active */
#define IR_BIT_TX_FIFO_LEVEL_INT_STATUS      ((u32)0x00000001 << 1)          /*!<R 0x0  When Tx FIFO offset <= threshold value, interrupt is triggered. *0: Interrupt inactive *1: Interrupt active */
#define IR_BIT_TX_FIFO_EMPTY_INT_STATUS      ((u32)0x00000001 << 0)          /*!<R 0x0  Tx FIFO empty interrupt *0: Interrupt inactive *1: Interrupt active */
/** @} */

/** @defgroup IR_TX_COMPE_DIV
 * @brief IR Tx compensation division register
 * @{
 **/
#define IR_MASK_TX_COMPE_DIV                 ((u32)0x00000FFF << 0)          /*!<R/W 0x0  IR_TX_CLK_Period = SCLK/(TX_COMPE_DIV + 1) */
#define IR_TX_COMPE_DIV(x)                   ((u32)(((x) & 0x00000FFF) << 0))
#define IR_GET_TX_COMPE_DIV(x)               ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup IR_TX_INT_CLR
 * @brief IR Tx FIFO and interrupt clear register
 * @{
 **/
#define IR_BIT_TX_FIFO_OVER_INT_CLR          ((u32)0x00000001 << 3)          /*!<WC   Tx FIFO overflow interrupt Write 1 to clear */
#define IR_BIT_TX_FIFO_LEVEL_INT_CLR         ((u32)0x00000001 << 2)          /*!<WC   When Tx FIFO offset <= threshold value, interrupt is triggered. Write 1 to clear */
#define IR_BIT_TX_FIFO_EMPTY_INT_CLR         ((u32)0x00000001 << 1)          /*!<WC   Tx FIFO empty interrupt Write 1 to clear */
#define IR_BIT_TX_FIFO_CLR                   ((u32)0x00000001 << 0)          /*!<WC   Write 1 to clear Tx FIFO */
/** @} */

/** @defgroup IR_TX_FIFO
 * @brief IR Tx FIFO register
 * @{
 **/
#define IR_BIT_TX_DATA_TYPE                  ((u32)0x00000001 << 31)          /*!<W 0  Data type *0: Inactive carrier (no carrier) *1: Active carrier (carrier) */
#define IR_BIT_TX_DATA_END_FLAG              ((u32)0x00000001 << 30)          /*!<W 0  *0: Normal packet *1: Last packet */
#define IR_MASK_TX_COMPENSATION              ((u32)0x00000003 << 28)          /*!<W 0  *0x0: IR_TX_CLK_Period = Tsys_clk * IR_CLK_DIV *0x1: IR_TX_CLK_Period = (1 + 1/2) Tsys_clk * IR_CLK_DIV *0x2: IR_TX_CLK_Period = (1 + 1/4) Tsys_clk * IR_CLK_DIV *0x3: IR_TX_CLK_Period = Tsys_clk * (IR_TX_COMPE_DIV + 1) */
#define IR_TX_COMPENSATION(x)                ((u32)(((x) & 0x00000003) << 28))
#define IR_GET_TX_COMPENSATION(x)            ((u32)(((x >> 28) & 0x00000003)))
#define IR_MASK_TX_DATA_TIME                 ((u32)0x0FFFFFFF << 0)          /*!<W 0  Real active time = (IR_TX_DATA_TIME + 1) * IR_TX_CLK_Period */
#define IR_TX_DATA_TIME(x)                   ((u32)(((x) & 0x0FFFFFFF) << 0))
#define IR_GET_TX_DATA_TIME(x)               ((u32)(((x >> 0) & 0x0FFFFFFF)))
/** @} */

/** @defgroup IR_RX_CONFIG
 * @brief IR Rx configuration register
 * @{
 **/
#define IR_BIT_RX_START                      ((u32)0x00000001 << 28)          /*!<R/W 0x0  *0: FSM stops at idle state. *1: FSM runs. */
#define IR_BIT_RX_START_MODE                 ((u32)0x00000001 << 27)          /*!<R/W 0x0  *0: Manual mode, IR_RX_MAN_START control *1: Auto-mode, Trigger-mode cont */
#define IR_BIT_RX_MAN_START                  ((u32)0x00000001 << 26)          /*!<R/W 0x0  If IR_RX_TRIGGER_MODE =0, writing 1 means starting to check the waveform. */
#define IR_MASK_RX_TRIGGER_MODE              ((u32)0x00000003 << 24)          /*!<R/W 0x0  *00: High --> low trigger *01: Low -> high trigger *10: High -> low or low ->high trigger */
#define IR_RX_TRIGGER_MODE(x)                ((u32)(((x) & 0x00000003) << 24))
#define IR_GET_RX_TRIGGER_MODE(x)            ((u32)(((x >> 24) & 0x00000003)))
#define IR_MASK_RX_FILTER_STAGETX            ((u32)0x00000007 << 21)          /*!<R/W 0x0  *0x0: Filter <= 20ns glitch *0x1: Filter <= 30ns glitch *0x2: Filter <= 40ns glitch *0x3: Filter <= 50ns glitch … */
#define IR_RX_FILTER_STAGETX(x)              ((u32)(((x) & 0x00000007) << 21))
#define IR_GET_RX_FILTER_STAGETX(x)          ((u32)(((x >> 21) & 0x00000007)))
#define IR_BIT_RX_FIFO_ERROR_INT_MASK        ((u32)0x00000001 << 19)          /*!<R/W 0x0  Rx FIFO error read interrupt When Rx FIFO is empty, read Rx FIFO and trigger interrupt. *0: Unmask *1: Mask */
#define IR_BIT_RX_CNT_THR_INT_MASK           ((u32)0x00000001 << 18)          /*!<R/W 0x0  Rx count threshold interrupt *0: Unmask *1: Mask */
#define IR_BIT_RX_FIFO_OF_INT_MASK           ((u32)0x00000001 << 17)          /*!<R/W 0x0  Rx FIFO overflow *0: Unmask *1: Mask */
#define IR_BIT_RX_CNT_OF_INT_MASK            ((u32)0x00000001 << 16)          /*!<R/W 0x0  RX counter overflow *0: Unmask *1: Mask */
#define IR_BIT_RX_FIFO_LEVEL_INT_MASK        ((u32)0x00000001 << 15)          /*!<R/W 0x0  Rx FIFO level interrupt *0: Unmask *1: Mask When Rx FIFO offset >= threshold value, this interrupt is triggered. */
#define IR_BIT_RX_FIFO_FULL_INT_MASK         ((u32)0x00000001 << 14)          /*!<R/W 0x0  Rx FIFO full interrupt *0: Unmask *1: Mask */
#define IR_BIT_RX_FIFO_DISCARD_SET           ((u32)0x00000001 << 13)          /*!<R/W 0x0  When FIFO is full, new data is send to FIFO. *0: Discard oldest data in FIFO. *1: Reject new data sending to FIFO. */
#define IR_MASK_RX_FIFO_LEVEL_TH             ((u32)0x0000001F << 8)          /*!<R/W 0x0  Rx FIFO interrupt threshold When Rx FIFO depth > threshold value, this interrupt is triggered. */
#define IR_RX_FIFO_LEVEL_TH(x)               ((u32)(((x) & 0x0000001F) << 8))
#define IR_GET_RX_FIFO_LEVEL_TH(x)           ((u32)(((x >> 8) & 0x0000001F)))
#define IR_BIT_RX_FIFO_ERROR_INT_EN          ((u32)0x00000001 << 5)          /*!<R/W 0x0  Rx FIFO error read interrupt When Rx FIFO is empty, reading the Rx FIFO triggers this interrupt. *0: Disable *1: Enable */
#define IR_BIT_RX_CNT_THR_INT_EN             ((u32)0x00000001 << 4)          /*!<R/W 0x0  Rx count threshold interrupt *0: Disable *1: Enable */
#define IR_BIT_RX_FIFO_OF_INT_EN             ((u32)0x00000001 << 3)          /*!<R/W 0x0  RX FIFO overflow *0: Disable *1: Enable */
#define IR_BIT_RX_CNT_OF_INT_EN              ((u32)0x00000001 << 2)          /*!<R/W 0x0  RX counter overflow *0: Disable *1: Enable */
#define IR_BIT_RX_FIFO_LEVEL_INT_EN          ((u32)0x00000001 << 1)          /*!<R/W 0x0  Rx FIFO level interrupt When Rx FIFO offset >= threshold value, this interrupt is triggered. *0: Disable *1: Enable */
#define IR_BIT_RX_FIFO_FULL_INT_EN           ((u32)0x00000001 << 0)          /*!<R/W 0x0  Rx FIFO full interrupt *0: Disable *1: Enable */
/** @} */

/** @defgroup IR_RX_SR
 * @brief IR Rx FIFO and interrupt status register
 * @{
 **/
#define IR_BIT_RX_FIFO_EMPTY                 ((u32)0x00000001 << 17)          /*!<R 0x0  *0: Not empty *1: Empty */
#define IR_BIT_RX_FIFO_FULL                  ((u32)0x00000001 << 16)          /*!<R 0x0  *0: Not full *1: Full */
#define IR_MASK_RX_FIFO_OFFSET               ((u32)0x0000003F << 8)          /*!<R 0x0  Rx FIFO offset */
#define IR_RX_FIFO_OFFSET(x)                 ((u32)(((x) & 0x0000003F) << 8))
#define IR_GET_RX_FIFO_OFFSET(x)             ((u32)(((x >> 8) & 0x0000003F)))
#define IR_BIT_RX_STATE                      ((u32)0x00000001 << 7)          /*!<R 0x0  *0: Idle *1: Run */
#define IR_BIT_RX_FIFO_ERROR_INT_STATUS      ((u32)0x00000001 << 5)          /*!<R 0x0  Rx FIFO error read interrupt status When Rx FIFO is empty, reading the Rx FIFO triggers this interrupt. *0: Interrupt is inactive *1: Interrupt is active */
#define IR_BIT_RX_CNT_THR_INT_STATUS         ((u32)0x00000001 << 4)          /*!<R 0x0  Rx count threshold interrupt status *0: Interrupt is inactive *1: Interrupt is active */
#define IR_BIT_RX_FIFO_OF_INT_STATUS         ((u32)0x00000001 << 3)          /*!<R 0x0  Rx FIFO overflow interrupt status *0: Interrupt is inactive *1: Interrupt is active */
#define IR_BIT_RX_CNT_OF_INT_STATUS          ((u32)0x00000001 << 2)          /*!<R 0x0  Rx counter overflow interrupt status *0: Interrupt is inactive *1: Interrupt is active */
#define IR_BIT_RX_FIFO_LEVEL_INT_STATUS      ((u32)0x00000001 << 1)          /*!<R 0x0  Rx FIFO level interrupt status *0: Interrupt is inactive *1: Interrupt is active */
#define IR_BIT_RX_FIFO_FULL_INT_STATUS       ((u32)0x00000001 << 0)          /*!<R 0x0  Rx FIFO full interrupt status *0: Interrupt is inactive *1: Interrupt is active */
/** @} */

/** @defgroup IR_RX_INT_CLR
 * @brief IR Rx FIFO and interrupt clear register
 * @{
 **/
#define IR_BIT_RX_FIFO_CLR                   ((u32)0x00000001 << 8)          /*!<WC   Write 1 to clear Rx FIFO */
#define IR_BIT_RX_FIFO_ERROR_INT_CLR         ((u32)0x00000001 << 5)          /*!<WC   Rx FIFO error read interrupt Write 1 to clear */
#define IR_BIT_RX_CNT_THR_INT_CLR            ((u32)0x00000001 << 4)          /*!<WC   Rx count threshold interrupt Write 1 to clear */
#define IR_BIT_RX_FIFO_OF_INT_CLR            ((u32)0x00000001 << 3)          /*!<WC   Rx FIFO overflow interrupt Write 1 to clear */
#define IR_BIT_RX_CNT_OF_INT_CLR             ((u32)0x00000001 << 2)          /*!<WC   Rx counter overflow interrupt Write 1 to clear */
#define IR_BIT_RX_FIFO_LEVEL_INT_CLR         ((u32)0x00000001 << 1)          /*!<WC   Rx FIFO level interrupt Write 1 to clear */
#define IR_BIT_RX_FIFO_FULL_INT_CLR          ((u32)0x00000001 << 0)          /*!<WC   Rx FIFO full interrupt Write 1 to clear */
/** @} */

/** @defgroup IR_RX_CNT_INT_SEL
 * @brief IR Rx count threshold configure register
 * @{
 **/
#define IR_BIT_RX_CNT_THR_TRIGGER_LV         ((u32)0x00000001 << 31)          /*!<R/W 0x0  Trigger level *0: When low level counter >= threshold, trigger interrupt *1: When high level counter >= threshold, trigger interrupt */
#define IR_MASK_RX_CNT_THR                   ((u32)0x7FFFFFFF << 0)          /*!<R/W 0x0  31-bits threshold */
#define IR_RX_CNT_THR(x)                     ((u32)(((x) & 0x7FFFFFFF) << 0))
#define IR_GET_RX_CNT_THR(x)                 ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/** @defgroup IR_RX_FIFO
 * @brief IR Rx FIFO register
 * @{
 **/
#define IR_BIT_RX_LEVEL                      ((u32)0x00000001 << 31)          /*!<RO 0x0  Rx Level *1: High level *0: Low level */
#define IR_MASK_RX_CNT                       ((u32)0x7FFFFFFF << 0)          /*!<RO 0x0  31-bits cycle duration */
#define IR_RX_CNT(x)                         ((u32)(((x) & 0x7FFFFFFF) << 0))
#define IR_GET_RX_CNT(x)                     ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/** @defgroup IR_VERSION
 * @brief IR IP version register
 * @{
 **/
#define IR_MASK_VERSION                      ((u32)0xFFFFFFFF << 0)          /*!<R 0x1410150A  IR IP version */
#define IR_VERSION(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IR_GET_VERSION(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup IR_Exported_Types IR Exported Types
  * @{
*****************************************************************************/


/**
 * @brief IR Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t IR_CLK_DIV;                             /*!< IR CLOCK DIVISION REGISTER,  Address offset: 0x000 */
	__IO uint32_t IR_TX_CONFIG;                           /*!< IR TX CONFIGURATION REGISTER,  Address offset: 0x004 */
	__I  uint32_t IR_TX_SR;                               /*!< IR TX FIFO AND INTERRUPT STATUS REGISTER,  Address offset: 0x008 */
	__IO uint32_t IR_TX_COMPE_DIV;                        /*!< IR TX COMPENSATION DIVISION REGISTER,  Address offset: 0x00C */
	__O  uint32_t IR_TX_INT_CLR;                          /*!< IR TX FIFO AND INTERRUPT CLEAR REGISTER,  Address offset: 0x010 */
	__O  uint32_t IR_TX_FIFO;                             /*!< IR TX FIFO REGISTER,  Address offset: 0x014 */
	__IO uint32_t IR_RX_CONFIG;                           /*!< IR RX CONFIGURATION REGISTER,  Address offset: 0x018 */
	__I  uint32_t IR_RX_SR;                               /*!< IR RX FIFO AND INTERRUPT STATUS REGISTER,  Address offset: 0x01C */
	__O  uint32_t IR_RX_INT_CLR;                          /*!< IR RX FIFO AND INTERRUPT CLEAR REGISTER,  Address offset: 0x020 */
	__IO uint32_t IR_RX_CNT_INT_SEL;                      /*!< IR RX COUNT THRESHOLD CONFIGURE REGISTER,  Address offset: 0x024 */
	__I  uint32_t IR_RX_FIFO;                             /*!< IR RX FIFO REGISTER,  Address offset: 0x028 */
	__I  uint32_t IR_VERSION;                             /*!< IR IP VERSION REGISTER,  Address offset: 0x02C */
} IR_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup IR_Exported_Types IR Exported Types
  * @{
  */

/**
 * @brief IR Init Structure Definition
 */
typedef struct {
	u32 IR_Clock;			/*!< Specifies the IR IP core Input clock. */
	u32 IR_Freq;			/*!< Specifies the clock frequency. This parameter is IR carrier freqency whose unit is Hz.
						This parameter can be a value of @ref IR_Carrier_Frequency */
	u32 IR_DutyCycle;		/*!< Specifies the IR duty cycle. */
	u32 IR_Mode;			/*!< Specifies the IR mode.
						This parameter can be a value of @ref IR_Mode */
	u32 IR_TxIdleLevel;		/*!< Specifies the IR output level in Tx mode
						This parameter can be a value of @ref IR_Idle_Level */
	u32 IR_TxInverse;		/*!< Specifies inverse FIFO data or not in TX mode
						This parameter can be a value of @ref IR_Tx_Data_Level */
	u32 IR_TxFIFOThrLevel;	/*!< Specifies TX FIFO interrupt threshold in TX mode. When TX FIFO depth <= threshold value, trigger interrupt.
						This parameter can be a value of @ref IR_Tx_Threshold */
	u32 IR_TxCOMP_CLK;		/*!< Specifies TX compensation clk. This CLK used when @ref IR_Tx_DATA_TYPE = IR_TX_CLK_Self_Def.*/
	u32 IR_RxStartMode;		/*!< Specifies Start mode in RX mode
						This parameter can be a value of @ref IR_Rx_Start_Control */
	u32 IR_RxFIFOThrLevel;	/*!< Specifies RX FIFO interrupt threshold in RX mode. when RX FIFO depth > threshold value, trigger interrupt.
						This parameter can be a value of @ref IR_Rx_Threshold */
	u32 IR_RxFIFOFullCtrl;	/*!< Specifies data discard mode in RX mode when RX FIFO is full and receiving new data
						This parameter can be a value of @ref IR_Rx_FIFO_Discard */
	u32 IR_RxTriggerMode;	/*!< Specifies trigger in RX mode
						This parameter can be a value of @ref IR_Rx_Auto_Trigger_Type */
	u32 IR_RxFilterTime;	/*!< Specifies filter time in RX mode
						This parameter can be a value of @ref IR_Rx_Filter_Time */
	u32 IR_RxCntThrType;	/*!< Specifies counter level type when trigger IR_INT_RX_CNT_THR interrupt in RX mode
						This parameter can be a value of @ref IR_Rx_Count_Level */
	u32 IR_RxCntThr;		/*!< Specifies counter threshold value when trigger IR_INT_RX_CNT_THR interrupt in RX mode */
} IR_InitTypeDef, *PIR_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup IR_Exported_Constants IR Exported Constants
  * @{
  */

/** @defgroup IR_Carrier_Frequency
  * @{
  */
#define IR_TX_FREQ_HZ_MIN			(25000)
#define IS_IR_FREQUENCY(F)			((F) >= IR_TX_FREQ_HZ_MIN)
/**
  * @}
  */

/** @defgroup IR_Mode
  * @{
  */
#define IR_MODE_TX					(0)
#define IR_MODE_RX					(1)

#define IR_MODE(MODE)				((MODE) & IR_BIT_MODE_SEL)
/**
  * @}
  */

/** @defgroup IR_Tx_Threshold
  * @{
  */
#define IR_TX_FIFO_SIZE						(32)
/**
  * @}
  */

/** @defgroup IR_Tx_Data_Level
  * @{
  */
#define IR_TX_DATA_NORMAL_CARRIER_NORMAL					(0)
#define IR_TX_DATA_NORMAL_CARRIER_INVERSE					(1)
#define IR_TX_DATA_INVERSE_CARRIER_NORMAL					(2)
#define IR_TX_DATA_INVERSE_CARRIER_INVERSE					(3)
#define IR_TX_DATA_TYPE_SET(x)				((u32)(((x) << 13) & (IR_BIT_TX_OUTPUT_INVERSE | IR_BIT_TX_DE_INVERSE)))
/**
  * @}
  */

/** @defgroup IR_Idle_Level
  * @{
  */
#define IR_IDLE_OUTPUT_LOW					(0)
#define IR_IDLE_OUTPUT_HIGH					(1)
/**
  * @}
  */

/** @defgroup IR_Tx_INT_Operation
  * @{
  */
#define IR_TX_INT_ALL_MASK					((u32)0x0000002C)
#define IR_TX_INT_ALL_EN					((u32)0x00000013)
#define IR_TX_INT_ALL_CLR					((u32)0x0000000E)
#define IS_TX_INT_MASK(MASK)				(((MASK) & (~IR_TX_INT_ALL_MASK)) == 0)
#define IS_TX_INT(MASK)						(((MASK) & (~IR_TX_INT_ALL_EN)) == 0)
#define IS_TX_INT_CLR(MASK)					(((MASK) & (~IR_TX_INT_ALL_CLR)) == 0)
/**
  * @}
  */

/** @defgroup IR_Rx_Start_Control
  * @{
  */
#define IR_RX_MANUAL_MODE					(0)
#define IR_RX_AUTO_MODE						(1)
/**
  * @}
  */

/** @defgroup IR_Rx_Filter_Time
  * @{
  */
#define IR_RX_FILTER_TIME_20NS					(0)
#define IR_RX_FILTER_TIME_30NS					(1)
#define IR_RX_FILTER_TIME_40NS					(2)
#define IR_RX_FILTER_TIME_50NS					(3)
#define IR_RX_FILTER_TIME_60NS					(4)
#define IR_RX_FILTER_TIME_70NS					(5)
#define IR_RX_FILTER_TIME_80NS					(6)
#define IR_RX_FILTER_TIME_90NS					(7)
/**
  * @}
  */

/** @defgroup IR_Rx_FIFO_Discard
  * @{
  */
#define IR_RX_FIFO_FULL_DISCARD_NEWEST			(1)
#define IR_RX_FIFO_FULL_DISCARD_OLDEST			(0)
/**
  * @}
  */

/** @defgroup IR_Rx_Threshold
  * @{
  */
#define IR_RX_FIFO_SIZE							(32)
/**
  * @}
  */

/** @defgroup IR_Rx_INT_Operation
  * @{
  */
#define IR_RX_INT_ALL_EN						((u32)0x0000003F)
#define IR_RX_INT_ALL_CLR						((u32)0x0000003F)
#define IR_RX_INT_ALL_MASK						((u32)0x000FC000)
#define IS_RX_INT_MASK(MASK)					(((MASK) & (~IR_RX_INT_ALL_MASK)) == 0)
#define IS_RX_INT(MASK)							(((MASK) & (~IR_RX_INT_ALL_EN)) == 0)
#define IS_RX_INT_CLR(MASK)						(((MASK) & (~IR_RX_INT_ALL_CLR)) == 0)
/**
  * @}
  */

/** @defgroup IR_Rx_Count_Level
  * @{
  */
#define IR_RX_COUNT_LOW_LEVEL					(0)
#define IR_RX_COUNT_HIGH_LEVEL					(1)
/**
  * @}
  */

/** @defgroup IR_Rx_Auto_Trigger_Type
  * @{
  */
#define IR_RX_FALL_EDGE							(0)
#define IR_RX_RISING_EDGE						(1)
#define IR_RX_DOUBLE_EDGE						(2)

#define IS_IR_RX_TRIGGER_EDGE(TRIGGER)			(((TRIGGER) == IR_RX_FALL_EDGE) || \
												((TRIGGER) == IR_RX_RISING_EDGE) || \
												((TRIGGER) == IR_RX_DOUBLE_EDGE))
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup IR_Exported_Functions IR Exported Functions
  * @{
  */

void IR_DeInit(void);
void IR_Init(IR_TypeDef *IRx, IR_InitTypeDef *IR_InitStruct);
void IR_StructInit(IR_InitTypeDef *IR_InitStruct);
void IR_Cmd(IR_TypeDef *IRx, u32 mode, u32 NewState);
void IR_SetRxCounterThreshold(IR_TypeDef *IRx, u32 IR_RxCntThrType, u32 IR_RxCntThr);
void IR_SendBuf(IR_TypeDef *IRx, u32 *pBuf, u32 len, u32 IsLastPacket);
void IR_ReceiveBuf(IR_TypeDef *IRx, u32 *pBuf, u32 len);
void IR_INTConfig(IR_TypeDef *IRx, u32 IR_INT, u32 newState);
void IR_MaskINTConfig(IR_TypeDef *IRx, u32 IR_INT, u32 newState);
u32 IR_GetINTStatus(IR_TypeDef *IRx);
u32 IR_GetIMR(IR_TypeDef *IRx);
u32 IR_FSMRunning(IR_TypeDef *IRx);
void IR_ClearINTPendingBit(IR_TypeDef *IRx, u32 IR_CLEAR_INT);
void IR_SetTxThreshold(IR_TypeDef *IRx, uint8_t thd);
void IR_SetRxThreshold(IR_TypeDef *IRx, uint8_t thd);
u32 IR_GetTxFIFOFreeLen(IR_TypeDef *IRx);
u32 IR_GetRxDataLen(IR_TypeDef *IRx);
void IR_SendData(IR_TypeDef *IRx, u32 data);
void IR_StartManualRxTrigger(IR_TypeDef *IRx);
u32 IR_ReceiveData(IR_TypeDef *IRx);
void IR_ClearTxFIFO(IR_TypeDef *IRx);
void IR_ClearRxFIFO(IR_TypeDef *IRx);

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
