/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_IPC_H_
#define _AMEBA_IPC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup IPC
  * @brief IPC driver modules
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
#define IPC_MASK_TX0_DATA               ((u32)0x000000FF << 8)          /*!<R/W/ES 0x0  After preparing descriptor, data and corresponding memory, 1: Writing 1 to the Tx data register (tx0_data) channel x bit, and the data will be mapped to the CPU0's RX data register (rx0_data) channel x bit. And the corresponding Rx full status bit in REG_ISR_CPU0 will be set. 0: Writing 0 has no effect. If the corresponding Rx full status bit in REG_ISR_CPU0 (eg.isr_rx0_full_status6) is cleared, the corresponding bit in tx0_data will be cleaned automatically. */
#define IPC_TX0_DATA(x)                 (((u32)((x) & 0x000000FF) << 8))
#define IPC_GET_TX0_DATA(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define IPC_MASK_TX1_DATA               ((u32)0x000000FF << 0)          /*!<R/W/ES 0x0  After preparing descriptor, data and corresponding memory, 1: Writing 1 to the Tx data register (tx1_data) channel x bit, and the data will be mapped to the CPU1's Rx data register (rx1_data) channel x bit. And the corresponding Rx full status bit in REG_ISR_CPU1 will be set. 0: Writing 0 has no effect. If the corresponding Rx full status bit in REG_ISR_CPU1 (eg.isr_rx1_full_status6) is cleared, the corresponding bit in tx_data_ln will be cleaned automatically. */
#define IPC_TX1_DATA(x)                 (((u32)((x) & 0x000000FF) << 0))
#define IPC_GET_TX1_DATA(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IPC_RX_DATA
 * @brief Rx Register
 * @{
 **/
#define IPC_MASK_RX0_DATA               ((u32)0x000000FF << 8)          /*!<R 0x0  Rx data automatically maps data from tx0_data of CPU0 */
#define IPC_RX0_DATA(x)                 (((u32)((x) & 0x000000FF) << 8))
#define IPC_GET_RX0_DATA(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define IPC_MASK_RX1_DATA               ((u32)0x000000FF << 0)          /*!<R 0x0  Rx data automatically maps data from tx1_data of CPU1 */
#define IPC_RX1_DATA(x)                 (((u32)((x) & 0x000000FF) << 0))
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
#define IPC_IMR_RX0_FULL_MASK(x)        (((u32)((x) & 0x000000FF) << 24))
#define IPC_GET_IMR_RX0_FULL_MASK(x)    ((u32)(((x >> 24) & 0x000000FF)))
#define IPC_MASK_IMR_RX1_FULL_MASK      ((u32)0x000000FF << 16)          /*!<R/W 0x0  0: Mask Rx Channel x full interrupt of CPU3 received from CPU1 1: Unmask Rx Channel x full interrupt of CPU3 received from CPU1 */
#define IPC_IMR_RX1_FULL_MASK(x)        (((u32)((x) & 0x000000FF) << 16))
#define IPC_GET_IMR_RX1_FULL_MASK(x)    ((u32)(((x >> 16) & 0x000000FF)))
#define IPC_MASK_IMR_TX0_EMPTY_MASK     ((u32)0x000000FF << 8)          /*!<R/W 0x0  0: Mask Tx Channel x empty interrupt of CPU3 transmit to CPU0 1: Unmask Tx Channel x empty interrupt of CPU3 transmit to CPU0 */
#define IPC_IMR_TX0_EMPTY_MASK(x)       (((u32)((x) & 0x000000FF) << 8))
#define IPC_GET_IMR_TX0_EMPTY_MASK(x)   ((u32)(((x >> 8) & 0x000000FF)))
#define IPC_MASK_IMR_TX1_EMPTY_MASK     ((u32)0x000000FF << 0)          /*!<R/W 0x0  0: Mask Tx Channel x empty interrupt of CPU3 transmit to CPU1 1: Unmask Tx Channel x empty interrupt of CPU3 transmit to CPU1 */
#define IPC_IMR_TX1_EMPTY_MASK(x)       (((u32)((x) & 0x000000FF) << 0))
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
typedef void (*IPC_IRQ_FUN)(void *Data, u32 IrqStatus, u32 ChanNum);

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
	void (*Rxfunc)(void *Data, u32 IrqStatus, u32 ChanNum);
	void *RxIrqData;
	void (*Txfunc)(void *Data, u32 IrqStatus, u32 ChanNum);
	void *TxIrqData;
	u32 IPC_Direction;	/* direction of ipc, this parameter is from @IPC_Direction_Mode*/
	u32 IPC_Channel;	/* ipc channel, this parameter is from @IPC_LP_Tx_Channel or @IPC_NP_Tx_Channel or @IPC_AP_Tx_Channel*/
} IPC_INIT_TABLE, *PIPC_INIT_TABLE;


/**
  * @brief IPC SEM IDX
  */
typedef enum {
	IPC_SEM_IMQ = 0,
	IPC_SEM_FLASH,
	IPC_SEM_OTP,
	IPC_SEM_CRYPTO,
	IPC_SEM_MAX = 16,			/* can't be this value, total 16 ipc semaphores*/
} IPC_SEM_IDX;
/** @} */

/* Exported constants --------------------------------------------------------*/
/** @defgroup IPC_Exported_Constants IPC Exported Constants
  * @{
  */

/** @defgroup IPC_Peripheral_Definition
  * @{
  */
#define IS_IPC_ALL_PERIPH(PERIPH) (((PERIPH) == IPCLP_DEV) || \
										((PERIPH) == IPCNP_DEV)) || \
										((PERIPH) == IPCAP_DEV))
/** @} */

/** @defgroup IPC_Direction_Mode
  * @{
  */
#define IPC_LP_TO_NP			((u32)0x00000000)
#define IPC_LP_TO_AP			((u32)0x00000001)
#define IPC_NP_TO_LP			((u32)0x00000010)
#define IPC_NP_TO_AP			((u32)0x00000011)
#define IPC_AP_TO_LP			((u32)0x00000020)
#define IPC_AP_TO_NP			((u32)0x00000021)
#define IS_IPC_DIR_MODE(MODE) (((MODE) == IPC_LP_TO_NP) || \
									((MODE) == IPC_LP_TO_AP) || \
									((MODE) == IPC_NP_TO_LP) || \
									((MODE) == IPC_NP_TO_AP) || \
									((MODE) == IPC_AP_TO_LP) || \
									((MODE) == IPC_AP_TO_NP))

#define IS_SEND_TO_NP(MODE)		(((MODE) == IPC_LP_TO_NP) || \
								((MODE) == IPC_AP_TO_NP))

#define IS_SEND_TO_AP(MODE)		(((MODE) == IPC_LP_TO_AP) || \
								((MODE) == IPC_NP_TO_AP))

#define IS_SEND_TO_LP(MODE)		(((MODE) == IPC_NP_TO_LP) || \
								((MODE) == IPC_AP_TO_LP))
/** @} */

/** @defgroup IPC_INTR_Mode
  * @{
  */
#define IPC_TX_EMPTY			((u32)0x00000001)
#define IPC_RX_FULL 			((u32)0x00000002)
#define IS_IPC_INTR_MODE(MODE) (((MODE) == IPC_TX_EMPTY) || \
                                   ((MODE) == IPC_RX_FULL))
/** @} */

/** @defgroup IPC_CHANNEL
  * @{
  */
#define IPC_TX_CHANNEL_NUM						16
#define IPC_TX_CHANNEL_SWITCH(x)				((u32)(((x >> 4) & 0x0000000F)))
#define IPC_TX0_CHANNEL_NUM						8
#define IPC_TX0_CHANNEL_SWITCH(x)				((u32)((x) & 0x0000000F))
#define IS_IPC_RX_CHNUM(NUM) 					((NUM) >= 16)
#define IPC_CHANNEL_NUM 						32
/** @} */

/** @defgroup IPC_Valid_CHNUM
  * @{
  */
#define IS_IPC_VALID_CHNUM(NUM) ((NUM) < 8)
/** @} */

/** @defgroup IPC_Valid_SEMID
  * @{
  */
#define IS_IPC_VALID_SEMID(SEM_ID) ((SEM_ID) < 16)
/** @} */


/** @defgroup IPC_Valid_CPUID
  * @{
  */
#define IS_IPC_Valid_CPUID(cpuid)		((cpuid)<=2)
/** @} */

/** @defgroup IPC_LP_Tx_Channel
 * @{
 */
#define IPC_L2N_LOGUART_RX_SWITCH		0	/*!<  LP -->  NP Loguart Rx Switch*/
#define IPC_L2N_WAKE_NP					1
#define IPC_L2N_FLASHPG_REQ				2	/*!<  LP -->  NP Flash Program REQUEST*/
//#define IPC_L2N_Channel3				3
//#define IPC_L2N_Channel4				4
//#define IPC_L2N_Channel5				5
//#define IPC_L2N_Channel6				6
#define IPC_L2N_IMQ_TRX_TRAN					7	/*!<  LP -->  NP IMQ Message Exchange */

#define IPC_L2A_LOGUART_RX_SWITCH				0	/*!<  LP -->  AP Loguart Rx Switch*/
#define IPC_L2A_Channel1				1
//#define IPC_L2A_Channel2				2
//#define IPC_L2A_Channel3				3
//#define IPC_L2A_Channel4				4
//#define IPC_L2A_Channel5				5
//#define IPC_L2A_Channel6				6
#define IPC_L2A_IMQ_TRX_TRAN					7	/*!<  LP -->  AP IMQ Message Exchange */
/** @} */

/** @defgroup IPC_NP_Tx_Channel
 * @{
 */
#define IPC_N2L_TICKLESS_INDICATION			0	/*!<  NP -->  LP Tickless indicate */
#define IPC_N2L_WIFI_FW_INFO					1	/*!<  NP -->  LP FW Info*/
#define IPC_N2L_FLASHPG_REQ					2	/*!<  NP -->  LP Flash Program Request*/
#define IPC_N2L_UARTBRIDGE						3
//#define IPC_N2L_Channel4						4
//#define IPC_N2L_Channel5						5
//#define IPC_N2L_Channel6						6
#define IPC_N2L_IMQ_TRX_TRAN					7	/*!<  NP -->  LP IMQ Message Exchange */

#define IPC_N2A_WIFI_TRX_TRAN					0	/*!<  NP -->  AP WIFI Message Exchange */
#define IPC_N2A_WIFI_API_TRAN					1	/*!<  NP -->  AP API WIFI Message Exchange */
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define IPC_N2A_NP_LOG_CHN						2	/*!<  NP -->  AP Send NP log to AP */
#else
//#define IPC_N2A_Channel2						2	/*!<  NP -->  AP */
#endif //CONFIG_PLATFORM_TIZENRT_OS
#define IPC_N2A_BT_API_TRAN						3	/*!<  NP -->  AP BT API Exchange */
#define IPC_N2A_BT_DRC_TRAN						4	/*!<  NP -->  AP BT DATA Message Exchange */
#define IPC_N2A_802154_TRAN						5
#define IPC_N2A_OTP_TX_TRAN						6
#define IPC_N2A_IMQ_TRX_TRAN					7	/*!<  NP -->  AP IMQ Message Exchange */
/** @} */

/** @defgroup IPC_AP_Tx_Channel
 * @{
 */
#define IPC_A2L_TICKLESS_INDICATION			0	/*!<  AP -->  LP Tickless Indicate */
//#define IPC_A2L_Channel1						1
#define IPC_A2L_UARTBRIDGE						2
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#define IPC_A2L_DISLOGUART						3
#define IPC_A2L_WIFI_FW_INFO					4 /*!<  AP -->  LP Get stats info from WIFI FW */
#else
//#define IPC_A2L_Channel3						3
//#define IPC_A2L_Channel4						4
#endif //CONFIG_PLATFORM_TIZENRT_OS
//#define IPC_A2L_Channel5						5
//#define IPC_A2L_Channel6						6
#define IPC_A2L_IMQ_TRX_TRAN					7	/*!<  AP -->  LP IMQ Message Exchange */

#define IPC_A2N_WIFI_TRX_TRAN					0	/*!<  AP -->  NP WIFI Message Exchange */
#define IPC_A2N_WIFI_API_TRAN					1	/*!<  AP -->  NP WIFI API Message Exchange */
#define IPC_A2N_FLASHPG_REQ						2	/*!<  AP -->  NP Flash Program Request*/
#define IPC_A2N_BT_API_TRAN						3	/*!<  AP -->  NP BT API Exchange */
#define IPC_A2N_BT_DRC_TRAN						4	/*!<  AP -->  NP BT DATA Message Exchange */
#define IPC_A2N_802154_TRAN						5
#define IPC_A2N_OTP_RX_TRAN						6
#define IPC_A2N_LOGUART_RX_SWITCH				7	/*!<  AP -->  NP Loguart Message Exchange for Linux*/
#define IPC_A2N_IMQ_TRX_TRAN					7	/*!<  AP -->  NP IMQ Message Exchange for RTOS*/
#if (defined(ARM_CORE_CA32) && defined(CONFIG_AS_AP)) || \
	(defined(ARM_CORE_CM4) && defined(CONFIG_AS_NP))

#define IPC_CH_WIFI_FW_CTRL		IPC_N2L_Channel1
#define IPC_DIR_WIFI_FW_CTRL		IPC_NP_TO_LP

#elif (defined(ARM_CORE_CM4) && defined(CONFIG_AS_AP)) || \
	(defined(ARM_CORE_CA32) && defined(CONFIG_AS_NP))

#define IPC_CH_WIFI_FW_CTRL		IPC_A2L_Channel1
#define IPC_DIR_WIFI_FW_CTRL		IPC_AP_TO_LP

#elif (defined(ARM_CORE_CM0))
#define IPC_CH_WIFI_FW_CTRL			1
#define IPC_FW_CA2LP_CHNUM			17
#define IPC_FW_KM2LP_CHNUM			25

#endif
/** @} */

/** @} */

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
void IPC_INTUserHandler(IPC_TypeDef *IPCx, u8 IPC_Shiftbit, void *IrqHandler, void *IrqData);
IPC_TypeDef *IPC_GetDevById(u32 cpu_id);
u32 IPC_SEMTake(u32 SEM_Idx, u32 timeout);
u32 IPC_SEMFree(u32 SEM_Idx);
void IPC_SEMDelayStub(void (*pfunc)(uint32_t));

/** @} */

/** @} */

/** @} */

/* Other definitions --------------------------------------------------------*/

extern const IPC_INIT_TABLE  ipc_init_config[];

/* MANUAL_GEN_END */

#endif

