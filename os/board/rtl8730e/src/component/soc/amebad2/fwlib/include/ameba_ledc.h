#ifndef _AMEBAD2_LEDC_H_
#define _AMEBAD2_LEDC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup LEDC
  * @brief LEDC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup LEDC_Register_Definitions LEDC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup LEDC_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_MASK_TOTAL_DATA_LENGTH        ((u32)0x00001FFF << 16)          /*!<R/W 0x0  Total length of transfer data, range from 1 to 8K (unit:32 bits, only low 24-bit is valid). Actual transfer packages is TOTAL_DATA_LENGTH+1. The field is recommended to an integer multiple of (LED_NUM+1). Note: If TOTAL_DATA_LENGTH is greater than (LED_NUM+1) but not integer multiple, the last frame of data will transfer data less than (LED_NUM+1). */
#define LEDC_TOTAL_DATA_LENGTH(x)          ((u32)(((x) & 0x00001FFF) << 16))
#define LEDC_GET_TOTAL_DATA_LENGTH(x)      ((u32)(((x >> 16) & 0x00001FFF)))
#define LEDC_BIT_RESET_LED_EN              ((u32)0x00000001 << 10)          /*!<R/W/ES 0x0  Write operation: When software writes 1 to this bit, LEDC FSM turns to CPU_RESET_LED state, and CPU triggers LEDC to transfer a reset to LED. Note: Only when LEDC is in the IDLE state, can FSM turn to CPU_RESET_LED state. * When LEDC finishs to send RESET to LED, LEDC FSM returns to the IDLE state, and hardware clears RESET_LED_EN. * Or when software sets SOFT_RESET, LEDC FSM returns to IDLE state, and hardware clears RESET_LED_EN. Read operation: 0: LEDC completes the transmission of LED reset operation. 1: LEDC does not complete the transmission of LED reset operation. */
#define LEDC_BIT_LED_POLARITY              ((u32)0x00000001 << 9)          /*!<R/W 0x0  LED DI level when free. 0: Low level 1: High level */
#define LEDC_MASK_LED_RGB_MODE             ((u32)0x00000007 << 6)          /*!<R/W 0x0  LEDC inputs 24 bits data pakage in order of {byte2, byte1, byte0}. 000: Output 24 bits data pakage in order of {byte2, byte1, byte0} 001: Output 24 bits data pakage in order of {byte2, byte0, byte1} 010: Output 24 bits data pakage in order of {byte1, byte2, byte0} 011: Output 24 bits data pakage in order of {byte0, byte2, byte1} 100: Output 24 bits data pakage in order of {byte1, byte0, byte2} 101: Output 24 bits data pakage in order of {byte0, byte1, byte2} */
#define LEDC_LED_RGB_MODE(x)               ((u32)(((x) & 0x00000007) << 6))
#define LEDC_GET_LED_RGB_MODE(x)           ((u32)(((x >> 6) & 0x00000007)))
#define LEDC_BIT_LED_MSB_TOP               ((u32)0x00000001 << 5)          /*!<R/W 0x1  Source RGB data format. 0: LSB 1: MSB */
#define LEDC_BIT_LED_MSB_BYTE2             ((u32)0x00000001 << 4)          /*!<R/W 0x1  LEDC outputs 24 bits data pakage in order of {byte2, byte1, byte0}; 1: Output data byte2 MSB first 0: Output data byte2 LSB first */
#define LEDC_BIT_LED_MSB_BYTE1             ((u32)0x00000001 << 3)          /*!<R/W 0x1  LEDC outputs 24 bits data pakage in order of {byte2, byte1, byte0}; 1: Output data byte1 MSB first 0: Output data byte1 LSB first */
#define LEDC_BIT_LED_MSB_BYTE0             ((u32)0x00000001 << 2)          /*!<R/W 0x1  LEDC outputs 24 bits data pakage in order of {byte2, byte1, byte0}; 1: Output data byte0 MSB first 0: Output data byte0 LSB first */
#define LEDC_BIT_SOFT_RESET                ((u32)0x00000001 << 1)          /*!<R/WA0 0x0  LEDC software reset. If software writes 1 to LEDC_SOFT_RESET, the next cycle hardware will clear LEDC_SOFT_RESET to 0, which will generate a pulse. LEDC soft reset includes all interrupt status register, the control state machine returns to the IDLE state, LEDC FIFO read and write point is cleared to 0, LEDC interrupt is cleared. The related registers are as below: * LEDC_CTRL_REG (LEDC_EN is cleared to 0) * LEDC_DATA_FINISH_CNT_REG (LEDC_DATA_FINISH_CNT is cleared to 0) * LEDC_INT_STS_REG (all interrupt are cleared) Other registers remain unchanged. */
#define LEDC_BIT_EN                        ((u32)0x00000001 << 0)          /*!<R/W/ES 0x0  LEDC enable bit. 0: Disable 1: Enable This bit enable indicates that LEDC can be started when LEDC data finished transmission, or this bit is cleared to 0 by hardware when software sets LEDC_SOFT_RESET. Software clears LEDC_EN when LEDC FSM is not IDLE don't affect data transfer. */
/** @} */

/** @defgroup LEDC_LED_T01_Timing_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_MASK_T1H_CNT                  ((u32)0x000000FF << 24)          /*!<R/W 0x20  LED modulate logical "1" high level time (T1H_TIME). Unit: cycle (40MHz), T1H_TIME = 25ns * T1H_CNT , where T1H_CNT>0 T1H_TIME default value is 800ns. */
#define LEDC_T1H_CNT(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define LEDC_GET_T1H_CNT(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define LEDC_MASK_T1L_CNT                  ((u32)0x000000FF << 16)          /*!<R/W 0xc  LED modulate logical "1" low level time. Unit: cycle (40MHz), T1L_TIME = 25ns *T1L_CNT, where T1L_CNT>0 T1L_TIME default value is 300ns. */
#define LEDC_T1L_CNT(x)                    ((u32)(((x) & 0x000000FF) << 16))
#define LEDC_GET_T1L_CNT(x)                ((u32)(((x >> 16) & 0x000000FF)))
#define LEDC_MASK_T0H_CNT                  ((u32)0x000000FF << 8)          /*!<R/W 0xc  LED modulate logical "0" hgh level time. Unit: cycle (40MHz), T0H_TIME = 25ns * T0H_CNT , where T0H_CNT>0 T0H_TIME default value is 300ns. */
#define LEDC_T0H_CNT(x)                    ((u32)(((x) & 0x000000FF) << 8))
#define LEDC_GET_T0H_CNT(x)                ((u32)(((x >> 8) & 0x000000FF)))
#define LEDC_MASK_T0L_CNT                  ((u32)0x000000FF << 0)          /*!<R/W 0x20  LED modulate logical "0" low level time. Unit: cycle (40MHz), T0L_TIME = 25ns *T0L_CNT, where T0L_CNT>0 T0L_TIME default value is 800ns. */
#define LEDC_T0L_CNT(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define LEDC_GET_T0L_CNT(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup LEDC_DATA_FINISH_CNT_REG
 * @brief
 * @{
 **/
#define LEDC_MASK_LED_WAIT_DATA_TIME       ((u32)0x00007FFF << 16)          /*!<R/W 0x2edf  The time that internal FIFO of LEDC is waiting for data. The default value is 300us by default. To avoid insert LED RESET between two pixel data, hardware will send WAITDATA_TIMEOUT_INT when wait_time>= min (RESET_TIME, LED_WAIT_DATA_TIME). The adjust range is from 25ns ~ 819us, LED_WAIT_DATA_TIME = 25ns *(N+1) where N is from 0 to 0x7FFF. When the setting time is exceeded, LEDC will send WAITDATA_TIMEOUT_INT interrupt. Under this abnormal situation, software should reset LEDC. */
#define LEDC_LED_WAIT_DATA_TIME(x)         ((u32)(((x) & 0x00007FFF) << 16))
#define LEDC_GET_LED_WAIT_DATA_TIME(x)     ((u32)(((x >> 16) & 0x00007FFF)))
#define LEDC_MASK_LED_DATA_FINISH_CNT      ((u32)0x00001FFF << 0)          /*!<R 0x0  The total LED data have been sent. (range: 0 ~ 8K). When LEDC transfer finishs normally, this register is cleared by hardware when generate trans_finish_int. Software knows LED_DATA_FINISH_CNTequals to TOTAL_DATA_LENGTH. When LEDC transfer interrupt abnormally,this register will clear to 0 by software by setting LEDC_SOFT_RESET. */
#define LEDC_LED_DATA_FINISH_CNT(x)        ((u32)(((x) & 0x00001FFF) << 0))
#define LEDC_GET_LED_DATA_FINISH_CNT(x)    ((u32)(((x >> 0) & 0x00001FFF)))
/** @} */

/** @defgroup LEDC_LED_RESET_TIMING_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_MASK_RESET_TIME               ((u32)0x00003FFF << 16)          /*!<R/W 0x2edf  Reset time control of LED lamp. The default reset time is 300us. The adjust range is from 25ns to 409.6us. RESET_TIME = 25ns *(N+1) where N is from 0 to 0x3FFF. */
#define LEDC_RESET_TIME(x)                 ((u32)(((x) & 0x00003FFF) << 16))
#define LEDC_GET_RESET_TIME(x)             ((u32)(((x >> 16) & 0x00003FFF)))
#define LEDC_MASK_LED_NUM                  ((u32)0x000003FF << 0)          /*!<R/W 0x0  The number of external LED lamp. Maximum up to 1024. The default value is 0, which indicate that 1 LED lamp is connected. The range is from 0 to 1023. */
#define LEDC_LED_NUM(x)                    ((u32)(((x) & 0x000003FF) << 0))
#define LEDC_GET_LED_NUM(x)                ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup LEDC_WAIT_TIME0_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_BIT_WAIT_TIME0_EN             ((u32)0x00000001 << 9)          /*!<R/W 0  Wait time0 enable. When set this bit, the controller automatically insert waiting time before next LED package data. 0: Disable 1: Enable */
#define LEDC_MASK_TOTAL_WAIT_TIME0         ((u32)0x000001FF << 0)          /*!<R/W 0x1FF  Waiting time between 2 LED data, and LEDC output Low level during waiting time0. The adjust range is from 25ns to 12.5us, WAIT_TIME0 = 25ns *(N+1) where N is from 0 to 0x1FF. */
#define LEDC_TOTAL_WAIT_TIME0(x)           ((u32)(((x) & 0x000001FF) << 0))
#define LEDC_GET_TOTAL_WAIT_TIME0(x)       ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup LEDC_WAIT_TIME1_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_BIT_WAIT_TIME1_EN             ((u32)0x00000001 << 31)          /*!<R/W 0  Wait time1 enable. When set this bit, the controller automatically insert waiting time before next LED frame data. 0: Disable 1: Enable */
#define LEDC_MASK_TOTAL_WAIT_TIME1         ((u32)0x7FFFFFFF << 0)          /*!<R/W 0x1FFFFFF  Waiting time between 2 frame data, and LEDC output Low level during waiting time1. The adjust range is from 25ns to 53s, WAIT_TIME0 = 25ns *(N+1) where N is from 0 to 0x7FFFFFFF. */
#define LEDC_TOTAL_WAIT_TIME1(x)           ((u32)(((x) & 0x7FFFFFFF) << 0))
#define LEDC_GET_TOTAL_WAIT_TIME1(x)       ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/** @defgroup LEDC_DATA_REG
 * @brief
 * @{
 **/
#define LEDC_MASK_DATA                     ((u32)0x00FFFFFF << 0)          /*!<WPD 0  LEDC display data, the lower 24-bit is valid. Write this register means push data to LEDC FIFO. Register LEDC_EN toggle from 0->1 will clear LEDC FIFO. Thus, only data pushed when LEDC_EN=1 will be send to LED. */
#define LEDC_DATA(x)                       ((u32)(((x) & 0x00FFFFFF) << 0))
#define LEDC_GET_DATA(x)                   ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup LEDC_DMA_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_BIT_DMA_EN                    ((u32)0x00000001 << 5)          /*!<R/W 0x1  LEDC DMA request enable. 0: Disable request of DMA transfer data 1: Enable request of DMA transfer data */
#define LEDC_MASK_FIFO_TRIG_LEVEL          ((u32)0x0000001F << 0)          /*!<R/W 0xF  The remaining space of internal FIFO in LEDC When the remain space of internal LEDC FIFO is larger than LEDC_FIFO_TRIG_LEVEL,will generate dma request or cpu_req_int. Note : The default value is 15. The adjust range is from 0 to 31, and the recommended cofiguration is 7 or 15. */
#define LEDC_FIFO_TRIG_LEVEL(x)            ((u32)(((x) & 0x0000001F) << 0))
#define LEDC_GET_FIFO_TRIG_LEVEL(x)        ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup LEDC_LED_INTERRUPT_CTRL_REG
 * @brief
 * @{
 **/
#define LEDC_BIT_GLOBAL_INT_EN             ((u32)0x00000001 << 4)          /*!<R/W 0  Global interrupt enable. 0: Disable 1: Enable */
#define LEDC_BIT_FIFO_OVERFLOW_INT_EN      ((u32)0x00000001 << 3)          /*!<R/W 0  FIFO overflow interrupt enable. When the data wriiten by software is more than internal FIFO level of LEDC, LEDC is in data loss state. 0: Disable 1: Enable */
#define LEDC_BIT_WAITDATA_TIMEOUT_INT_EN   ((u32)0x00000001 << 2)          /*!<R/W 0  The internal FIFO in LEDC can`t get data after the LED_WAIT_DATA_TIME, the interrupt will be enable. 0: Disable 1: Enable */
#define LEDC_BIT_FIFO_CPUREQ_INT_EN        ((u32)0x00000001 << 1)          /*!<R/W 0  FIFO request CPU data interrupt enable. 0: Disable 1: Enable */
#define LEDC_BIT_LED_TRANS_FINISH_INT_EN   ((u32)0x00000001 << 0)          /*!<R/W 0  Data transmission complete interrupt enable. 0: Disable 1: Enable */
/** @} */

/** @defgroup LEDC_LED_INT_STS_REG
 * @brief
 * @{
 **/
#define LEDC_BIT_FIFO_EMPTY                ((u32)0x00000001 << 17)          /*!<R 0x1  FIFO empty status flag. When LEDC_EN is 0, FIFO_EMPTY equal 1. This flush residual data when LED_NUM is not integer multiples of burst length. */
#define LEDC_BIT_FIFO_FULL                 ((u32)0x00000001 << 16)          /*!<R 0  FIFO full status flag. */
#define LEDC_BIT_FIFO_OVERFLOW_INT         ((u32)0x00000001 << 3)          /*!<RW1C 0  FIFO overflow interrupt Clear 0 when software set LEDC_SOFT_RESET. The data written by external is more than the maximum storage space of LEDC FIFO. At this time, software needs to deal with the abnormal situation.LEDC performs soft_reset operation to refresh all data. 0: FIFO not overflow 1: FIFO overflow */
#define LEDC_BIT_WAITDATA_TIMEOUT_INT      ((u32)0x00000001 << 2)          /*!<RW1C 0  Before transfer a frame: When the internal FIFO in LEDC can`t get data after the LED_WAIT_DATA_TIME,this bit is set. During transfer of a frame (between two pixel of data): To avoid insert LED RESET between two pixel data, hardware will send WAITDATA_TIMEOUT_INT when wait_time>= min(RESET_TIME,LED_WAIT_DATA_TIME). Clear 0 when software set LEDC_SOFT_RESET. When new data arrives at WAIT_DATA state, LEDC will continue sending data. Note : software should notice if the waiting time exceeds the operation of reset time, LEDC may enter refresh state, new data has not been sent. 0: LEDC not timeout 1: LEDC timeout */
#define LEDC_BIT_FIFO_CPUREQ_INT           ((u32)0x00000001 << 1)          /*!<RW1C 0  When FIFO data available is less than the threshold left, the interrupt is reported to CPU. Clear 0 when software set LEDC_SOFT_RESET. 0: FIFO doesn`t request CPU for data 1: FIFO request CPU to transfer data Note: when get FIFO_CPUREQ_INT, CPU write 1 data to LEDC Buffer. It is inefficiently compares to DMA mode. */
#define LEDC_BIT_LED_TRANS_FINISH_INT      ((u32)0x00000001 << 0)          /*!<RW1C 0  When the data configured as TOTAL_DATA_LENGTH has been transferred complete, this bit is set. Clear 0 when software set LEDC_SOFT_RESET. 0: Data transfer has not complete 1: Data transfer complete */
/** @} */

/** @defgroup LEDC_DEBUG_SEL_REG
 * @brief
 * @{
 **/
#define LEDC_MASK_DEBUG_SEL_DRIVE          ((u32)0x00000003 << 7)          /*!<R/W 0  drive module debug out select. */
#define LEDC_DEBUG_SEL_DRIVE(x)            ((u32)(((x) & 0x00000003) << 7))
#define LEDC_GET_DEBUG_SEL_DRIVE(x)        ((u32)(((x >> 7) & 0x00000003)))
#define LEDC_MASK_DEBUG_SEL_FIFO           ((u32)0x0000000F << 3)          /*!<R/W 0  fifo module debug out select. [6:5] select debug_data of ledc_txfifo, valid range 0,1 [4:3] select output data byte when [6:5] is 0. 2'b00: byte0 2'b01: byte1 2'b1x: byte2 */
#define LEDC_DEBUG_SEL_FIFO(x)             ((u32)(((x) & 0x0000000F) << 3))
#define LEDC_GET_DEBUG_SEL_FIFO(x)         ((u32)(((x >> 3) & 0x0000000F)))
#define LEDC_MASK_DEBUG_SEL_TOP            ((u32)0x00000007 << 0)          /*!<R/W 0  ledc top module debug out select. */
#define LEDC_DEBUG_SEL_TOP(x)              ((u32)(((x) & 0x00000007) << 0))
#define LEDC_GET_DEBUG_SEL_TOP(x)          ((u32)(((x >> 0) & 0x00000007)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup LEDC_Exported_Types LEDC Exported Types
  * @{
*****************************************************************************/


/**
 * @brief LEDC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t LEDC_CTRL_REG;                          /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t LEDC_LED_T01_TIMING_CTRL_REG;           /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t LEDC_DATA_FINISH_CNT_REG;               /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t LEDC_LED_RESET_TIMING_CTRL_REG;         /*!<  Register,  Address offset: 0x00C */
	__IO uint32_t LEDC_WAIT_TIME0_CTRL_REG;               /*!<  Register,  Address offset: 0x010 */
	__IO uint32_t LEDC_WAIT_TIME1_CTRL_REG;               /*!<  Register,  Address offset: 0x014 */
	__O  uint32_t LEDC_DATA_REG;                          /*!<  Register,  Address offset: 0x018 */
	__IO uint32_t LEDC_DMA_CTRL_REG;                      /*!<  Register,  Address offset: 0x01C */
	__IO uint32_t LEDC_LED_INTERRUPT_CTRL_REG;            /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t LEDC_LED_INT_STS_REG;                   /*!<  Register,  Address offset: 0x024 */
	__IO uint32_t RSVD0[2];                               /*!<  Reserved,  Address offset:0x28-0x2C */
	__IO uint32_t LEDC_DEBUG_SEL_REG;                     /*!<  Register,  Address offset: 0x030 */
} LEDC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types ------------------------------------------------------------*/
/** @addtogroup LEDC_Exported_Types LEDC Exported Types
  * @{
  */

/**
  * @brief  LEDC Init Structure Definition
  */
typedef struct {
	u32 led_count;
	u32 data_length;
	u32 t1h_ns;
	u32 t1l_ns;
	u32 t0h_ns;
	u32 t0l_ns;
	u32 reset_ns;
	u32 wait_data_time_ns;
	u32 wait_time0_en;
	u32 wait_time1_en;
	u32 wait_time0_ns;
	u32 wait_time1_ns;
	u32 output_RGB_mode;
	u32 ledc_polarity;
	u32 ledc_trans_mode;
	u32 ledc_fifo_level;
} LEDC_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup LEDC_Exported_Constants LEDC Exported Constants
  * @{
  */

/** @defgroup LEDC_Transfer_Mode
  * @{
  */
#define LEDC_CPU_MODE				(0)
#define LEDC_DMA_MODE				(1)
#define IS_LEDC_TRANS_MODE(MODE)	(((MODE) == LEDC_CPU_MODE ||\
									(MODE) == LEDC_DMA_MODE))
/**
  * @}
  */

/** @defgroup LEDC_FIFO_Level
  * @{
  */
#define LEDC_FIFO_DEPTH					(32)
#define LEDC_DEFAULT_FIFO_TRIG_LEVEL	(15)
/**
  * @}
  */

/** @defgroup LEDC_Idle_Level
  * @{
  */
#define LEDC_IDLE_POLARITY_LOW			(0)
#define LEDC_IDLE_POLARITY_HIGH			(1)
/**
  * @}
  */

/** @defgroup LEDC_RGB_Mode
  * @{
  */
#define LEDC_OUTPUT_GRB				(0)
#define LEDC_OUTPUT_GBR				(1)
#define LEDC_OUTPUT_RGB				(2)
#define LEDC_OUTPUT_BGR				(3)
#define LEDC_OUTPUT_RBG				(4)
#define LEDC_OUTPUT_BRG				(5)

#define IS_LEDC_OUTPUT_MODE(MODE)	((MODE) == LEDC_OUTPUT_GRB || \
									(MODE) == LEDC_OUTPUT_GBR || \
									(MODE) == LEDC_OUTPUT_RGB || \
									(MODE) == LEDC_OUTPUT_BGR || \
									(MODE) == LEDC_OUTPUT_RBG || \
									(MODE) == LEDC_OUTPUT_BRG)
/**
  * @}
  */

/** @defgroup LEDC_Input_Order
  * @{
  */
#define LEDC_INPUT_LSB				(0)
#define LEDC_INPUT_MSB				(1)
#define IS_LEDC_INPUT_ORDER(ORDER)	((ORDER) == LEDC_INPUT_LSB || \
									(ORDER) == LEDC_INPUT_MSB)
/**
  * @}
  */

/** @defgroup LEDC_Output_Order
  * @{
  */
#define LEDC_OUTPUT_ORDER_MASK		((u32)0x0000001C)
#define LEDC_OUTPUT_ORDER(x)		((u32)((x) & 0x00000007) << 2)
/**
  * @}
  */

/** @defgroup LEDC_Interrupt
  * @{
  */
#define LEDC_INT_ALL			((u32)0x0000001F)
#define LEDC_INT_EXT_EN			((u32)0x0000001D)

#define IS_LEDC_INTERRUPT(INT)	((INT) <= LEDC_INT_ALL)
/**
  * @}
  */

/** @defgroup LEDC_Data_Length
  * @{
  */
#define LEDC_MAX_DATA_LENGTH			(0x2000)

#define IS_LEDC_DATA_LENGTH(LENGTH)		((LENGTH > 0) && ((LENGTH) <= LEDC_MAX_DATA_LENGTH))
/**
  * @}
  */

/** @defgroup LEDC_LED_Number
  * @{
  */
#define LEDC_DEFAULT_LED_NUM	(32)
#define LEDC_MAX_LED_NUM		(1024)

#define IS_LEDC_LED_NUM(NUM)	((NUM > 0) && (NUM <= LEDC_MAX_LED_NUM))
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup LEDC_Exported_Functions LEDC Exported Functions
  * @{
  */

/** @defgroup LEDC_Exported_Normal_Functions LEDC Exported Normal Functions
  * @{
  */
_LONG_CALL_ void LEDC_StructInit(LEDC_InitTypeDef *LEDC_InitStruct);
_LONG_CALL_ void LEDC_Init(LEDC_TypeDef *LEDCx, LEDC_InitTypeDef *LEDC_InitStruct);
_LONG_CALL_ void LEDC_Cmd(LEDC_TypeDef *LEDCx, u8 NewState);
_LONG_CALL_ void LEDC_INTConfig(LEDC_TypeDef *LEDCx, u32 LEDC_IT, u32 NewState);
_LONG_CALL_ void LEDC_ClearINT(LEDC_TypeDef *LEDCx, u32 LEDC_IT);
_LONG_CALL_ u32 LEDC_GetINT(LEDC_TypeDef *LEDCx);
_LONG_CALL_ void LEDC_SetTransferMode(LEDC_TypeDef *LEDCx, u32 mode);
_LONG_CALL_ void LEDC_SoftReset(LEDC_TypeDef *LEDCx);
_LONG_CALL_ void LEDC_WriteData(LEDC_TypeDef *LEDCx, u32 data);
_LONG_CALL_ u32 LEDC_SendData(LEDC_TypeDef *LEDCx, void *data, u32 Len);
_LONG_CALL_ void LEDC_SetFIFOLevel(LEDC_TypeDef *LEDCx, u8 FifoLevel);
_LONG_CALL_ BOOL LEDC_TXGDMA_Init(LEDC_TypeDef *LEDCx, PGDMA_InitTypeDef GDMA_InitStruct, void *CallbackData,
								  IRQ_FUN CallbackFunc, u32 *pTxData, u32 Length);
/**
  * @}
  */

/** @defgroup LEDC_Exported_Timing_Configure_Functions LEDC Exported Timing Configure Functions
  * @{
  */
_LONG_CALL_ void LEDC_SetTotalLength(LEDC_TypeDef *LEDCx, u32 TotalData);
_LONG_CALL_ void LEDC_SetLEDNum(LEDC_TypeDef *LEDCx, u32 Num);
_LONG_CALL_ void LEDC_SetPolarity(LEDC_TypeDef *LEDCx, u32 Pol);
_LONG_CALL_ void LEDC_SetOutputMode(LEDC_TypeDef *LEDCx, u32 mode);
_LONG_CALL_ void LEDC_SetReset_val(LEDC_TypeDef *LEDCx, u32 RstVal);
_LONG_CALL_ void LEDC_SetT1H_val(LEDC_TypeDef *LEDCx, u32 T1hVal);
_LONG_CALL_ void LEDC_SetT1L_val(LEDC_TypeDef *LEDCx, u32 T1lVal);
_LONG_CALL_ void LEDC_SetT0L_val(LEDC_TypeDef *LEDCx, u32 T0lVal);
_LONG_CALL_ void LEDC_SetT0H_val(LEDC_TypeDef *LEDCx, u32 T0hVal);
_LONG_CALL_ void LEDC_SetWaitDataTime_val(LEDC_TypeDef *LEDCx, u32 WaitDataVal);
_LONG_CALL_ void LEDC_SetWaitTime0_val(LEDC_TypeDef *LEDCx, u32 WaitTime0, u8 NewState);
_LONG_CALL_ void LEDC_SetWaitTime1_val(LEDC_TypeDef *LEDCx, u32 WaitTime1, u8 NewState);

_LONG_CALL_ void LEDC_SetOutputOrder(LEDC_TypeDef *LEDCx, u8 order);
_LONG_CALL_ void LEDC_SetInputMode(LEDC_TypeDef *LEDCx, u8 order);
_LONG_CALL_ void LEDC_LEDReset(LEDC_TypeDef *LEDCx);
_LONG_CALL_ u32 LEDC_GetTransferMode(LEDC_TypeDef *LEDCx);
_LONG_CALL_ u32 LEDC_GetFIFOLevel(LEDC_TypeDef *LEDCx);
/**
  * @}
  */

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
