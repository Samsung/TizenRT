#ifndef _AMEBA_IPC_H_
#define _AMEBA_IPC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup IPC
  * @brief  IPC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup IPC_Register_Definitions IPC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup IPC_TX_DATA
 * @brief Tx Register
 * @{
 **/
#define IPC_MASK_TX0_DATA               ((u32)0x000000FF << 8)          /*!<R/WH/ES 0x0  After preparing descriptor, data and corresponding memory, 1: Writing 1 to the Tx data register (tx0_data) channel x bit, and the data will be mapped to the CPU0's RX data register (rx0_data) channel x bit. And the corresponding Rx full status bit in REG_ISR_CPU0 will be set. 0: Writing 0 has no effect. If the corresponding Rx full status bit in REG_ISR_CPU0 (eg.isr_rx0_full_status6) is cleared, the corresponding bit in tx0_data will be cleaned automatically. */
#define IPC_TX0_DATA(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define IPC_GET_TX0_DATA(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define IPC_MASK_TX1_DATA               ((u32)0x000000FF << 0)          /*!<R/WH/ES 0x0  After preparing descriptor, data and corresponding memory, 1: Writing 1 to the Tx data register (tx1_data) channel x bit, and the data will be mapped to the CPU1's Rx data register (rx1_data) channel x bit. And the corresponding Rx full status bit in REG_ISR_CPU1 will be set. 0: Writing 0 has no effect. If the corresponding Rx full status bit in REG_ISR_CPU1 (eg.isr_rx1_full_status6) is cleared, the corresponding bit in tx_data_ln will be cleaned automatically. */
#define IPC_TX1_DATA(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define IPC_GET_TX1_DATA(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IPC_RX_DATA
 * @brief Rx Register
 * @{
 **/
#define IPC_MASK_RX0_DATA               ((u32)0x000000FF << 8)          /*!<R 0x0  Rx data automatically maps data from tx0_data of CPU0 */
#define IPC_RX0_DATA(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define IPC_GET_RX0_DATA(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define IPC_MASK_RX1_DATA               ((u32)0x000000FF << 0)          /*!<R 0x0  Rx data automatically maps data from tx1_data of CPU1 */
#define IPC_RX1_DATA(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define IPC_GET_RX1_DATA(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IPC_ISR
 * @brief Interrupt Status Register
 * @{
 **/
#define IPC_BIT_ISR_RX0_FULL_STATUS7    ((u32)0x00000001 << 31)          /*!<RW1CB 0x0  Rx channel 7 full interrupt status of CPU3. The corresponding Rx full status bit will be set by CPU0's Tx data register (tx0_data) channel 7. It will be cleared by software writing 1. */
#define IPC_BIT_ISR_RX0_FULL_STATUS6    ((u32)0x00000001 << 30)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX0_FULL_STATUS5    ((u32)0x00000001 << 29)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX0_FULL_STATUS4    ((u32)0x00000001 << 28)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX0_FULL_STATUS3    ((u32)0x00000001 << 27)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX0_FULL_STATUS2    ((u32)0x00000001 << 26)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX0_FULL_STATUS1    ((u32)0x00000001 << 25)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX0_FULL_STATUS0    ((u32)0x00000001 << 24)          /*!<RW1CB 0x0  Refer to the description of the isr_rx0_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS7    ((u32)0x00000001 << 23)          /*!<RW1CB 0x0  Rx channel 7 full interrupt status of CPU3. The corresponding Rx full status bit will be set by CPU1's Tx data register (tx1_data) channel 7. It will be cleared by software writing 1. */
#define IPC_BIT_ISR_RX1_FULL_STATUS6    ((u32)0x00000001 << 22)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS5    ((u32)0x00000001 << 21)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS4    ((u32)0x00000001 << 20)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS3    ((u32)0x00000001 << 19)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS2    ((u32)0x00000001 << 18)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS1    ((u32)0x00000001 << 17)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_RX1_FULL_STATUS0    ((u32)0x00000001 << 16)          /*!<RW1CB 0x0  Refer to the description of the isr_rx1_full_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS7   ((u32)0x00000001 << 15)          /*!<RW1CB 0x0  Tx channel 7 empty interrupt status of CPU3 transmit to CPU0. If the corresponding Rx full status bit in REG_ISR_CPU0 (eg.isr_rx0_full_status7) is cleared, the corresponding Tx empty status bit will be set automatically. It will be cleared by software writing 1. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS6   ((u32)0x00000001 << 14)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS5   ((u32)0x00000001 << 13)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS4   ((u32)0x00000001 << 12)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS3   ((u32)0x00000001 << 11)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS2   ((u32)0x00000001 << 10)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS1   ((u32)0x00000001 << 9)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX0_EMPTY_STATUS0   ((u32)0x00000001 << 8)          /*!<RW1CB 0x0  Refer to the description of the isr_tx0_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS7   ((u32)0x00000001 << 7)          /*!<RW1CB 0x0  Tx channel 7 empty interrupt status of CPU3 transmit to CPU1. If the corresponding Rx full status bit in REG_ISR_CPU1 (eg.isr_rx1_full_status7) is cleared, the corresponding Tx empty status bit will be set automatically. It will be cleared by software writing 1. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS6   ((u32)0x00000001 << 6)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS5   ((u32)0x00000001 << 5)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS4   ((u32)0x00000001 << 4)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS3   ((u32)0x00000001 << 3)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS2   ((u32)0x00000001 << 2)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS1   ((u32)0x00000001 << 1)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
#define IPC_BIT_ISR_TX1_EMPTY_STATUS0   ((u32)0x00000001 << 0)          /*!<RW1CB 0x0  Refer to the description of the isr_tx1_empty_status7. */
/** @} */

/** @defgroup IPC_IMR
 * @brief Interrupt Mask Register
 * @{
 **/
#define IPC_MASK_IMR_RX0_FULL_MASK      ((u32)0x000000FF << 24)          /*!<R/W 0x0  0: Mask Rx Channel x full interrupt of CPU3 received from CPU0 1: Unmask Rx Channel x full interrupt of CPU3 received from CPU0 */
#define IPC_IMR_RX0_FULL_MASK(x)        ((u32)(((x) & 0x000000FF) << 24))
#define IPC_GET_IMR_RX0_FULL_MASK(x)    ((u32)(((x >> 24) & 0x000000FF)))
#define IPC_MASK_IMR_RX1_FULL_MASK      ((u32)0x000000FF << 16)          /*!<R/W 0x0  0: Mask Rx Channel x full interrupt of CPU3 received from CPU1 1: Unmask Rx Channel x full interrupt of CPU3 received from CPU1 */
#define IPC_IMR_RX1_FULL_MASK(x)        ((u32)(((x) & 0x000000FF) << 16))
#define IPC_GET_IMR_RX1_FULL_MASK(x)    ((u32)(((x >> 16) & 0x000000FF)))
#define IPC_MASK_IMR_TX0_EMPTY_MASK     ((u32)0x000000FF << 8)          /*!<R/W 0x0  0: Mask Tx Channel x empty interrupt of CPU3 transmit to CPU0 1: Unmask Tx Channel x empty interrupt of CPU3 transmit to CPU0 */
#define IPC_IMR_TX0_EMPTY_MASK(x)       ((u32)(((x) & 0x000000FF) << 8))
#define IPC_GET_IMR_TX0_EMPTY_MASK(x)   ((u32)(((x >> 8) & 0x000000FF)))
#define IPC_MASK_IMR_TX1_EMPTY_MASK     ((u32)0x000000FF << 0)          /*!<R/W 0x0  0: Mask Tx Channel x empty interrupt of CPU3 transmit to CPU1 1: Unmask Tx Channel x empty interrupt of CPU3 transmit to CPU1 */
#define IPC_IMR_TX1_EMPTY_MASK(x)       ((u32)(((x) & 0x000000FF) << 0))
#define IPC_GET_IMR_TX1_EMPTY_MASK(x)   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IPC_ICR
 * @brief Clear Tx Register
 * @{
 **/
#define IPC_BIT_ICR_TX0_DATA_CLEAR7     ((u32)0x00000001 << 15)          /*!<RW1CB 0x0  0: Writing 0 has no effect. 1: Clear the Tx data register (tx0_data) channel 7 bit. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR6     ((u32)0x00000001 << 14)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR5     ((u32)0x00000001 << 13)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR4     ((u32)0x00000001 << 12)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR3     ((u32)0x00000001 << 11)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR2     ((u32)0x00000001 << 10)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR1     ((u32)0x00000001 << 9)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX0_DATA_CLEAR0     ((u32)0x00000001 << 8)          /*!<RW1CB 0x0  Refer to the description of the icr_tx0_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR7     ((u32)0x00000001 << 7)          /*!<RW1CB 0x0  0: Writing 0 has no effect. 1: Clear the Tx data register (tx1_data) channel 7 bit. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR6     ((u32)0x00000001 << 6)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR5     ((u32)0x00000001 << 5)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR4     ((u32)0x00000001 << 4)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR3     ((u32)0x00000001 << 3)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR2     ((u32)0x00000001 << 2)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR1     ((u32)0x00000001 << 1)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
#define IPC_BIT_ICR_TX1_DATA_CLEAR0     ((u32)0x00000001 << 0)          /*!<RW1CB 0x0  Refer to the description of the icr_tx1_data_clear7. */
/** @} */

/** @defgroup IPC_SEMx
 * @brief
 * @{
 **/
#define IPC_BIT_SEMx_CLR                ((u32)0x00000001 << 11)          /*!<WA0 0x0  The bit can clear 'CPUID ' & 'PXID' when owner CPU write 1. */
#define IPC_BIT_SEMx_SET_CPUID          ((u32)0x00000001 << 10)          /*!<WA0 0x0  The bit can set cpuid, It must be written at the same time as sem_pxid. */
#define IPC_MASK_SEMx_CPUID             ((u32)0x00000003 << 8)          /*!<R 0x0  ‘CPUID’ bit is a read-only bit. 00：Currently semaphore can be occupied by CPU. When writing PXID, hardware sets CPUID. 01: Currently semaphore is occupied by Kr4. 10: Currently semaphore is occupied by Km4. 11: Currently semaphore is occupied by hifi. */
#define IPC_SEMx_CPUID(x)               ((u32)(((x) & 0x00000003) << 8))
#define IPC_GET_SEMx_CPUID(x)           ((u32)(((x >> 8) & 0x00000003)))
#define IPC_MASK_SEMx_PXID              ((u32)0x000000FF << 0)          /*!<R/WE/ES 0x0  CPUID_PXID, which is used to indicate who owns this semaphore when the CPUID is the same. */
#define IPC_SEMx_PXID(x)                ((u32)(((x) & 0x000000FF) << 0))
#define IPC_GET_SEMx_PXID(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup IPC_Exported_Types IPC Exported Types
  * @{
*****************************************************************************/


/**
 * @brief IPC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t IPC_TX_DATA;                            /*!< TX REGISTER,  Address offset: 0x0000 */
	__I  uint32_t IPC_RX_DATA;                            /*!< RX REGISTER,  Address offset: 0x0004 */
	__IO uint32_t IPC_ISR;                                /*!< INTERRUPT STATUS REGISTER,  Address offset: 0x0008 */
	__IO uint32_t IPC_IMR;                                /*!< INTERRUPT MASK REGISTER,  Address offset: 0x000C */
	__IO uint32_t IPC_ICR;                                /*!< CLEAR TX REGISTER,  Address offset: 0x0010 */
	__IO uint32_t IPC_SEMx[4];                            /*!<  Register,  Address offset: 0x0014-0x0020 */
	__IO uint32_t IPC_DUMMY;                              /*!<  Register,  Address offset: 0x0024 */
} IPC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup IPC_Exported_Types IPC Exported Types
  * @{
  */

/**
  * @brief IPC IRQ Function Definition
  */
typedef void (*IPC_IRQ_FUN)(VOID *Data, u32 IrqStatus, u32 ChanNum);

/**
  * @brief IPC User Message Type Definition
  */
typedef enum {
	IPC_USER_POINT = 0,
	IPC_USER_DATA	= 1
} USER_MSG_TYP_DEF;

/**
  * @brief IPC Init Table Definition
  */
typedef struct _IPC_INIT_TABLE_ {
	u32 USER_MSG_TYPE;
	void (*func)(VOID *Data, u32 IrqStatus, u32 ChanNum);
	VOID *IrqData;
	u32 IPC_Direction;	/* direction of ipc, this parameter is from @IPC_Direction_Mode*/
	u32 IPC_Channel;	/* ipc channel, this parameter is from @IPC_KM4_Tx_Channel or @IPC_KR4_Tx_Channel or @IPC_DSP_Tx_Channel*/
	u32 IPC_Intr_Mode;	/*Tx or Rx Intr, this parameter is from @IPC_INTR_Mode */
} IPC_INIT_TABLE, *PIPC_INIT_TABLE;

/**
  * @brief IPC SEM IDX
  */
typedef enum {
	IPC_SEM_IMQ = 0,
	IPC_SEM_FLASH,
	IPC_SEM_2,		/* reserved for futural use*/
	IPC_SEM_3,		/* reserved for futural use*/
	IPC_SEM_4,		/* reserved for futural use*/
} IPC_SEM_IDX;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup IPC_Exported_Constants IPC Exported Constants
  * @{
  */

/** @defgroup IPC_Peripheral_Definition
  * @{
  */
#define IS_IPC_ALL_PERIPH(PERIPH) (((PERIPH) == IPCKR4_DEV) || \
										((PERIPH) == IPCKM4_DEV)) || \
										((PERIPH) == IPCDSP_DEV))
/**
  * @}
  */

/** @defgroup IPC_Direction_Mode
  * @{
  */
#define IPC_KR4_TO_KM4			((u32)0x00000000)
#define IPC_KR4_TO_DSP			((u32)0x00000001)
#define IPC_KM4_TO_KR4			((u32)0x00000010)
#define IPC_KM4_TO_DSP			((u32)0x00000011)
#define IPC_DSP_TO_KR4			((u32)0x00000020)
#define IPC_DSP_TO_KM4			((u32)0x00000021)
#define IS_IPC_DIR_MODE(MODE) (((MODE) == IPC_KR4_TO_KM4) || \
									((MODE) == IPC_KR4_TO_DSP) || \
									((MODE) == IPC_KM4_TO_KR4) || \
									((MODE) == IPC_KM4_TO_DSP) || \
									((MODE) == IPC_DSP_TO_KR4) || \
									((MODE) == IPC_DSP_TO_KM4))
/**
  * @}
  */

/** @defgroup IPC_INTR_Mode
  * @{
  */
#define IPC_TX_EMPTY			((u32)0x00000001)
#define IPC_RX_FULL 			((u32)0x00000002)
#define IS_IPC_INTR_MODE(MODE) (((MODE) == IPC_TX_EMPTY) || \
                                   ((MODE) == IPC_RX_FULL))
/**
  * @}
  */

/** @defgroup IPC_Valid_PXID
  * @{
  */
#define IS_IPC_VALID_PXID(PXID) ((PXID) > 0)
/**
  * @}
  */

/** @defgroup IPC_TX_CHANNEL
  * @{
  */
#define IPC_TX_CHANNEL_NUM						16
#define IPC_TX_CHANNEL_SWITCH(x)				((u32)(((x >> 4) & 0x0000000F)))
#define IPC_TX0_CHANNEL_NUM						8
#define IPC_TX0_CHANNEL_SWITCH(x)				((u32)((x) & 0x0000000F))

/**
  * @}
  */

/** @defgroup IPC_KR4_Tx_Channel
  * @{
  */
#define IPC_R2M_TICKLESS_INDICATION		0	/*!<  KR4 -->  KM4 Tickless indicate */
#define IPC_R2M_WAKE_AP					1	/*!<  KR4 -->  KM4 Wakeup*/
#define IPC_R2M_WIFI_FW_INFO				2	/*!<  KR4 -->  KM4 FW Info*/
#define IPC_R2M_FLASHPG_REQ				3	/*!<  KR4 -->  KM4 Flash Program REQUEST*/
#define IPC_R2M_IMQ_TRX_TRAN				4	/*!<  KR4 -->  KM4 IMQ Message Exchange */
//#define IPC_R2M_Channel4				4
//#define IPC_R2M_Channel5				5
#define IPC_R2M_BT_API_TRAN					5	/*!<  KR4 -->  KM4 BT API Exchange */
// #define IPC_R2M_BT_DATA_TRAN				5	/*!<  KR4 -->  KM4 BT DATA Exchange */
#define IPC_R2M_WIFI_TRX_TRAN				6	/*!<  KR4 -->  KM4 WIFI Message Exchange */
#define IPC_R2M_WIFI_API_TRAN				7	/*!<  KR4 -->  KM4 API WIFI Message Exchange */

#define IPC_R2D_WAKE_DSP				0	/*!<  KR4 -->  DSP Tickless indicate */
//#define IPC_R2D_Channel1				1
//#define IPC_R2D_Channel2				2
//#define IPC_R2D_Channel3				3
#define IPC_R2D_IMQ_TRX_TRAN			4	/*!<  KR4 -->  DSP IMQ Message Exchange */
//#define IPC_R2D_Channel4				4
//#define IPC_R2D_Channel5				5
//#define IPC_R2D_Channel6				6
//#define IPC_R2D_Channel7				7
/**
  * @}
  */

/** @defgroup IPC_KM4_Tx_Channel
  * @{
  */
#define IPC_M2R_TICKLESS_INDICATION				0	/*!<  KM4 -->  KR4 Tickless indicate */
#define IPC_M2R_WIFI_FW_INFO						1	/*!<  KM4 -->  KR4 FW Info*/
#define IPC_M2R_WAKE_AP							2	/*!<  KM4 -->  KR4 Wakeup*/
#define IPC_M2R_LOGUART_RX_SWITCH				3	/*!<  KM4 -->  KR4 Loguart Rx Switch*/
#define IPC_M2R_IMQ_TRX_TRAN					4	/*!<  KM4 -->  KR4 IMQ Message Exchange */
//#define IPC_M2R_Channel5						5
#define IPC_M2R_BT_API_TRAN						5	/*!<  KM4 -->  KR4 BT API Exchange */
// #define IPC_M2R_BT_DATA_TRAN					5	/*!<  KM4 -->  KR4 BT DATA Exchange */
#define IPC_M2R_WIFI_TRX_TRAN						6	/*!<  KM4 -->  KR4 WIFI Message Exchange */
#define IPC_M2R_WIFI_API_TRAN						7	/*!<  KM4 -->  KR4 WIFI API Message Exchange */

#define IPC_M2D_WAKE_DSP						0	/*!<  KM4 -->  DSP Wakeup */
//#define IPC_M2D_Channel1							1
#define IPC_M2D_LOGUART_RX_SWITCH				2	/*!<  KM4 -->  DSP Loguart Rx Switch*/
#define IPC_M2D_802154_TRAN						3
#define IPC_M2D_IMQ_TRX_TRAN					4	/*!<  KM4 -->  DSP IMQ Message Exchange */
//#define IPC_M2D_Channel4						4
//#define IPC_M2D_Channel5						5
//#define IPC_M2D_Channel6						6
//#define IPC_M2D_Channel7						7
/**
  * @}
  */

/** @defgroup IPC_DSP_Tx_Channel
  * @{
  */
#define IPC_D2R_TICKLESS_INDICATION				0	/*!<  DSP -->  KR4 Tickless Indicate */
//#define IPC_D2R_Channel1						1
//#define IPC_D2R_Channel2						2
//#define IPC_D2R_Channel3						3
#define IPC_D2R_IMQ_TRX_TRAN					4	/*!<  DSP -->  KR4 IMQ Message Exchange */
//#define IPC_D2R_Channel4						4
//#define IPC_D2R_Channel5						5
//#define IPC_D2R_Channel6						6
//#define IPC_D2R_Channel7						7

#define IPC_D2M_TICKLESS_INDICATION				0	/*!<  DSP -->  KM4 Tickless Indicate */
//#define IPC_D2M_WIFI_TRX_TRAN						1	/*!<  DSP -->  KM4 WIFI Message Exchange */
//#define IPC_D2M_WIFI_API_TRAN						2	/*!<  DSP -->  KM4 WIFI API Message Exchange */
#define IPC_D2M_802154_TRAN						3
#define IPC_D2M_IMQ_TRX_TRAN					4	/*!<  DSP -->  KM4 IMQ Message Exchange */
//#define IPC_D2M_Channel4						4
//#define IPC_D2M_Channel5						5
//#define IPC_D2M_Channel6						6
//#define IPC_D2M_Channel7						7
/**
  * @}
  */

/** @defgroup IPC_SEM_PXID
  * @{
  */
#define KM4_IPC_PXID BIT(0)
#define KR4_IPC_PXID BIT(1)
#define DSP_IPC_PXID BIT(2)
#define CA7_IPC_PXID BIT(3)

#if defined(ARM_CORE_CM4)
#define IPC_FLASH_LOCK  KM4_IPC_PXID
#elif defined(ARM_CORE_CA32)
#define IPC_FLASH_LOCK  KR4_IPC_PXID
#elif defined(RSICV_CORE_KR4)
#define IPC_FLASH_LOCK  DSP_IPC_PXID
#else
#define IPC_FLASH_LOCK  CA7_IPC_PXID
#endif
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup IPC_Exported_Functions IPC Exported Functions
  * @{
  */

void IPC_INTConfig(IPC_TypeDef *IPCx, u8 IPC_Shiftbit, u32 NewState);
void IPC_IERSet(IPC_TypeDef *IPCx, u32 IPC_Chs);
u32 IPC_IERGet(IPC_TypeDef *IPCx);
u32 IPC_INTRequest(IPC_TypeDef *IPCx, u32 IPC_Dir, u8 IPC_ChNum);
u32 IPC_INTGet(IPC_TypeDef *IPCx);
void IPC_INTClear(IPC_TypeDef *IPCx, u8 IPC_Shiftbit);
u32 IPC_INTHandler(void *Data);
void IPC_INTUserHandler(IPC_TypeDef *IPCx, u8 IPC_Shiftbit, VOID *IrqHandler, VOID *IrqData);
u32 IPC_SEMTake(IPC_SEM_IDX SEM_Idx, u32 PXID_Idx);
u32 IPC_SEMFree(IPC_SEM_IDX SEM_Idx, u32 PXID_Idx);

/**
  * @}
  */

/** @} */

/** @} */

/* Other definitions --------------------------------------------------------*/

extern const IPC_INIT_TABLE  ipc_init_config[];

/* MANUAL_GEN_END */

#endif

