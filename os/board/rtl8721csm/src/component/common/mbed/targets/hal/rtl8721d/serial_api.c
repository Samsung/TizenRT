/** mbed Microcontroller Library
  ******************************************************************************
  * @file    serial_api.c
  * @author 
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for UART.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */
#include "objects.h"
#include "serial_api.h"
#include "serial_ex_api.h"
#include "pinmap.h"
#include <string.h>
#include <stdbool.h>

/** @addtogroup AmebaD_Mbed_API 
  * @{
  */
  
/** @defgroup MBED_UART
 *  @brief      MBED_UART driver modules.
 *  @{
 */
 
/** @defgroup MBED_UART_Exported_Types MBED_UART Exported Types
  * @{
  */
typedef VOID (*UARTCB_FUN)(VOID *);

typedef struct {
	int	TxCount;     // how many byte to TX
	int	RxCount;     // how many bytes to RX
	u8	*pTxBuf;
	u8	*pRxBuf;
	u8	UartIndex;

	GDMA_InitTypeDef UARTTxGdmaInitStruct;
	GDMA_InitTypeDef UARTRxGdmaInitStruct;
	UART_InitTypeDef UART_InitStruct;
	UART_TypeDef* UARTx;
	IRQn_Type   IrqNum;

	/* for rx DMA timeout */
	u32 last_dma_addr;
	
	VOID (*TxCompCallback)(VOID *para);    // User Tx complete callback function
	VOID (*RxCompCallback)(VOID *para);    // User Rx complete callback function
	VOID *TxCompCbPara; // the pointer argument for TxCompCbPara
	VOID *RxCompCbPara; // the pointer argument for RxCompCallback
}MBED_UART_ADAPTER, *PMBED_UART_ADAPTER;

#define UART_NUM (4)
#define SERIAL_TX_IRQ_EN        0x01
#define SERIAL_RX_IRQ_EN        0x02
#define SERIAL_TX_DMA_EN        0x01
#define SERIAL_RX_DMA_EN        0x02

//#define UART_USE_GTIMER_TO		1
#define UART_TIMER_ID	1
#define UART_TIMER_TO	5000

#define CONFIG_GDMA_EN 1 

static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0, 0};

static uart_irq_handler irq_handler[UART_NUM];
static uint32_t serial_irq_en[UART_NUM]={0, 0, 0, 0};

static int current_baudrate;


#ifdef CONFIG_GDMA_EN 
static uint32_t serial_dma_en[UART_NUM] = {0, 0, 0, 0};
#endif

MBED_UART_ADAPTER uart_adapter[MAX_UART_INDEX+1];

#ifdef CONFIG_MBED_ENABLED
int stdio_uart_inited = 0;
serial_t stdio_uart;
#endif

#ifdef UART_USE_GTIMER_TO
static void uart_gtimer_deinit(void);
#endif

static const PinMap PinMap_UART_TX[] = {
	{PA_12,		UART_3,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PB_1,		UART_3,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_26,		UART_3,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_18,		UART_0,		PIN_DATA(PullUp , PINMUX_FUNCTION_UART)},
	{PA_21,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PB_9,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PB_19,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_7,		UART_2,		PIN_DATA(PullUp, PINMUX_FUNCTION_LOGUART)},
	{NC,		NC,			0}
};

static const PinMap PinMap_UART_RX[] = {
	{PA_13,		UART_3,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PB_2,		UART_3,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_25,		UART_3,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_19,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_22,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PB_8,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PB_18,		UART_0,		PIN_DATA(PullUp, PINMUX_FUNCTION_UART)},
	{PA_8,		UART_2,		PIN_DATA(PullUp, PINMUX_FUNCTION_LOGUART)},
	{NC,		NC,			0}
};

static u32
uart_index_get(PinName tx)
{
	if ((tx == _PA_12) ||(tx == _PB_1)|| (tx == _PA_26)) {
		return 3;
	} else if ((tx == _PA_18) || (tx == _PA_21)||(tx == _PB_9)||(tx == _PB_19)) {
		return 0;
	}else if (tx == _PA_7){
		return 2;
	} else {
		assert_param(0);
	}
        return 3;
}
/** 
  * @}
  */

/** @defgroup MBED_UART_Exported_Functions MBED_UART Exported Functions
  * @{
  */

static u32
uart_dmasend_complete(
        IN VOID *Data
)
{
	PMBED_UART_ADAPTER puart_adapter = (PMBED_UART_ADAPTER) Data;
	PGDMA_InitTypeDef GDMA_InitStruct;

	puart_adapter->TxCount = 0;
	
	GDMA_InitStruct = &puart_adapter->UARTTxGdmaInitStruct;

	// Clean Auto Reload Bit
	GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_DST);

	// Clear Pending ISR
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	/*disable UART TX DMA*/
	UART_TXDMACmd(puart_adapter->UARTx, DISABLE);

	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
	GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	UART_SetTxFlag(puart_adapter->UartIndex, 0);

	// Call user TX complete callback
	if (NULL != puart_adapter->TxCompCallback) {
		puart_adapter->TxCompCallback(puart_adapter->TxCompCbPara);
	}
	
	return 0;
}

static u32
uart_dmarecv_complete(
        IN VOID *Data
)
{
	PMBED_UART_ADAPTER puart_adapter = (PMBED_UART_ADAPTER) Data;
	PGDMA_InitTypeDef GDMA_InitStruct;
	//u8 LineStatus;

	GDMA_InitStruct = &puart_adapter->UARTRxGdmaInitStruct;

#ifdef UART_USE_GTIMER_TO
	RTIM_Cmd(TIMx[UART_TIMER_ID], DISABLE);  

	uart_gtimer_deinit();
#endif

	// Clean Auto Reload Bit
	GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_SRC);

	// Clear Pending ISR
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	/*disable UART RX DMA*/
	UART_RXDMACmd(puart_adapter->UARTx, DISABLE);

	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
	GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	// Check the Line Status
	//LineStatus = (u8)UART_LineStatusGet(puart_adapter->UARTx);

	UART_SetRxFlag(puart_adapter->UartIndex, 0);

	/* close time out */
	UART_INTConfig(puart_adapter->UARTx, RUART_IER_ERBI | RUART_IER_ELSI |RUART_IER_ETOI, DISABLE);
	
	// Call User Rx complete callback
	if (puart_adapter->RxCompCallback != NULL) {
		puart_adapter->RxCompCallback (puart_adapter->RxCompCbPara);
	}
	
	return 0;
}

static u32
uart_dmarecv_irqhandler(
        IN VOID *Data
)
{
	PMBED_UART_ADAPTER puart_adapter = (PMBED_UART_ADAPTER) Data;

	DCache_Invalidate((u32)puart_adapter->pRxBuf, puart_adapter->RxCount);
	puart_adapter->RxCount = 0;
	uart_dmarecv_complete(puart_adapter);

	return 0;
}

static void uart_txdone_callback(VOID *pAdapter)
{
	PMBED_UART_ADAPTER puart_adapter = pAdapter;
	u8 uart_idx = puart_adapter->UartIndex;

	// Mask UART TX FIFI empty
	UART_INTConfig(puart_adapter->UARTx, RUART_IER_ETBEI, DISABLE);

	if (irq_handler[uart_idx] != NULL) {
		irq_handler[uart_idx](serial_irq_ids[uart_idx], TxIrq);
	}
}

static void uart_rxdone_callback(VOID *pAdapter)
{
	PMBED_UART_ADAPTER puart_adapter = pAdapter;
	u8 uart_idx = puart_adapter->UartIndex;

	if (irq_handler[uart_idx] != NULL) {
		irq_handler[uart_idx](serial_irq_ids[uart_idx], RxIrq);
	}
}

static VOID
uart_intrecv_complete(
        IN PMBED_UART_ADAPTER puart_adapter
        )
{
	//volatile u8 LineStatus;

	// Disable RX Interrupt
	UART_INTConfig(puart_adapter->UARTx, (RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI), DISABLE);

	// Check the Line Status
	//LineStatus = (u8)UART_LineStatusGet(puart_adapter->UARTx);

	UART_SetRxFlag(puart_adapter->UartIndex, 0);

	// Call User Rx complete callback
	if (puart_adapter->RxCompCallback != NULL) {
		puart_adapter->RxCompCallback (puart_adapter->RxCompCbPara);
	}
}

static VOID
uart_intsend_complete(
        IN PMBED_UART_ADAPTER puart_adapter
        )
{
	// Disable Tx FIFO empty interrupt
	UART_INTConfig(puart_adapter->UARTx, RUART_IER_ETBEI, DISABLE);

	UART_SetTxFlag(puart_adapter->UartIndex, 0);

	// Call user TX complete callback
	if (NULL != puart_adapter->TxCompCallback) {
		puart_adapter->TxCompCallback(puart_adapter->TxCompCbPara);
	}
}

static u32
uart_irqhandler(
        IN VOID *Data
)
{
	volatile u8 reg_iir;
	u8 IntId;
	u32 RegValue;
	PMBED_UART_ADAPTER puart_adapter = (PMBED_UART_ADAPTER) Data;

	reg_iir = UART_IntStatus(puart_adapter->UARTx);
	if ((reg_iir & RUART_IIR_INT_PEND) != 0) {
		// No pending IRQ
		return 0;
	}

	IntId = (reg_iir & RUART_IIR_INT_ID) >> 1;

	switch (IntId) {
	case RUART_LP_RX_MONITOR_DONE:
		RegValue = UART_RxMonitorSatusGet(puart_adapter->UARTx);
	break;
	
	case RUART_MODEM_STATUS:
		RegValue =  UART_ModemStatusGet(puart_adapter->UARTx);
	break;
		
	case RUART_RECEIVE_LINE_STATUS:
		RegValue = UART_LineStatusGet(puart_adapter->UARTx);
	break;

	case RUART_TX_FIFO_EMPTY:
		if (UART_GetTxFlag(puart_adapter->UartIndex)) {
			int cnt=16;
			while(cnt>0&&puart_adapter->TxCount>0){
				UART_CharPut(puart_adapter->UARTx, *puart_adapter->pTxBuf);
				puart_adapter->TxCount --;
				puart_adapter->pTxBuf ++;
				cnt--;
			}
			
			if (0 == puart_adapter->TxCount) {
				uart_intsend_complete(puart_adapter);
			}
		} else {
			// Call Tx done callback
			uart_txdone_callback(puart_adapter);
		}
	break;

	case RUART_RECEIVER_DATA_AVAILABLE:
	case RUART_TIME_OUT_INDICATION:
		if (UART_GetRxFlag(puart_adapter->UartIndex) == STATERX_INT) {
			u32 TransCnt = 0;
			
			TransCnt = UART_ReceiveDataTO(puart_adapter->UARTx, puart_adapter->pRxBuf,
				puart_adapter->RxCount, 1);
			puart_adapter->RxCount -= TransCnt;
			puart_adapter->pRxBuf += TransCnt;
		
			if (puart_adapter->RxCount == 0) {
				uart_intrecv_complete(puart_adapter);
			}
		} else {
			// Call Rx data ready callback
			RegValue = (UART_LineStatusGet(puart_adapter->UARTx));
			if (RegValue & RUART_LINE_STATUS_REG_DR) {
				uart_rxdone_callback(puart_adapter);
			}
		}
	break;

	default:
		DBG_PRINTF(MODULE_UART, LEVEL_ERROR, "Unknown Interrupt \n");
	break;
	}

	return 0;
}

#ifdef UART_USE_GTIMER_TO
static void
uart_gtimer_handle(
    IN  VOID        *Data
) 
{
	PMBED_UART_ADAPTER puart_adapter = (PMBED_UART_ADAPTER) Data;
	PGDMA_InitTypeDef GDMA_InitStruct;
	u32 TransCnt = 0;
				
	GDMA_InitStruct = &puart_adapter->UARTRxGdmaInitStruct;

	RTIM_INTClear(TIMx[UART_TIMER_ID]);
	
	if (UART_GetRxFlag(puart_adapter->UartIndex) == STATERX_DMA) {
		u32 Current_Addr = GDMA_GetDstAddr(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
		u32 data_in_fifo = UART_Readable(puart_adapter->UARTx);

		/* have Rx some data */
		if ((Current_Addr != (u32)(puart_adapter->pRxBuf)) || data_in_fifo) {
			/* not increase for 5ms */
			if (puart_adapter->last_dma_addr == Current_Addr) {
				/* rx stop 5ms, packet complete */
				RTIM_Cmd(TIMx[UART_TIMER_ID], DISABLE);
				
				//DBG_8195A("%s:UART DMA TO Current_Addr:%x start_addr:%x RxCount: %d\n",
				//	__func__, Current_Addr, puart_adapter->pRxBuf, puart_adapter->RxCount);

				puart_adapter->RxCount = puart_adapter->RxCount - (Current_Addr - (u32)puart_adapter->pRxBuf);
				puart_adapter->pRxBuf = (u8*)Current_Addr;

				TransCnt = UART_ReceiveDataTO(puart_adapter->UARTx, puart_adapter->pRxBuf,
					puart_adapter->RxCount, 1);
				puart_adapter->RxCount -= TransCnt;
				puart_adapter->pRxBuf += TransCnt;
				
				uart_dmarecv_complete(puart_adapter);
				
				GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
	
				//DBG_8195A("UART DMA TO RxCount: %d\n", puart_adapter->RxCount);
			} else {
				puart_adapter->last_dma_addr = Current_Addr;
			}
		} else { /* rx not start */
			puart_adapter->last_dma_addr = (u32)(puart_adapter->pRxBuf);
		}
	}
}

  /**
  * @brief    Initialize the timer used for the Uart Rx Dma time out.
  * @param  puart_adapter:  a PRUART_VERIFY_PARAMETER pointer.
  * @param  Period: the desired timeout value (Unit:  microsecond).
  *
  * @retval   none
  */
static void
uart_gtimer_init(PMBED_UART_ADAPTER puart_adapter, u32 PeriodUs)
{
	RTIM_TimeBaseInitTypeDef TIM_InitStructTmp;

	RTIM_TimeBaseStructInit(&TIM_InitStructTmp);
	TIM_InitStructTmp.TIM_Idx = UART_TIMER_ID;
	TIM_InitStructTmp.TIM_Prescaler = 0x00;
	TIM_InitStructTmp.TIM_Period = (PeriodUs / 31 - 1);
	TIM_InitStructTmp.TIM_UpdateEvent = ENABLE; /* UEV enable */
	TIM_InitStructTmp.TIM_UpdateSource = TIM_UpdateSource_Overflow;
	TIM_InitStructTmp.TIM_ARRProtection = DISABLE;

	RTIM_TimeBaseInit(TIMx[UART_TIMER_ID], &TIM_InitStructTmp,
		TIMx_irq[UART_TIMER_ID], (IRQ_FUN) uart_gtimer_handle,
		(u32)puart_adapter);
	RTIM_INTConfig(TIMx[UART_TIMER_ID], TIM_IT_Update, ENABLE);	
}

static void
uart_gtimer_deinit(void)
{
	InterruptDis(TIMx_irq[UART_TIMER_ID]);
	InterruptUnRegister(TIMx_irq[UART_TIMER_ID]);
	
	RTIM_DeInit(TIMx[UART_TIMER_ID]);
}
#endif

/**
  * @brief  Initializes the UART device, include clock/function/interrupt/UART registers.
  * @param  obj: uart object define in application software.
  * @param  tx: Tx PinName according to pinmux spec.
  * @param  rx: Rx PinName according to pinmux spec.
  * @retval none  
  */
void serial_init(serial_t *obj, PinName tx, PinName rx) 
{
	uint8_t uart_idx = uart_index_get(tx);
	UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
	UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
	UARTName UARTx = (UARTName)pinmap_merge(uart_tx, uart_rx);
	PMBED_UART_ADAPTER puart_adapter = NULL;

	assert_param(UARTx != (UARTName)NC);

	obj->uart_idx = uart_idx;	
	
	puart_adapter = &(uart_adapter[obj->uart_idx]);

	puart_adapter->UartIndex = uart_idx;
	puart_adapter->UARTx = UART_DEV_TABLE[uart_idx].UARTx;
	puart_adapter->IrqNum = UART_DEV_TABLE[uart_idx].IrqNum;

	/* Configure the UART pins */
	pinmap_pinout(tx, PinMap_UART_TX);
	pinmap_pinout(rx, PinMap_UART_RX);
	pin_mode(tx, PullUp);
	pin_mode(rx, PullUp);

	UART_StructInit(&puart_adapter->UART_InitStruct);
	UART_Init(puart_adapter->UARTx, &puart_adapter->UART_InitStruct);
	InterruptRegister((IRQ_FUN)uart_irqhandler, puart_adapter->IrqNum, (u32)puart_adapter, 5);
	InterruptEn(puart_adapter->IrqNum, 5);

#ifdef CONFIG_MBED_ENABLED
	// For stdio management
	if (uart_idx == STDIO_UART) {
		stdio_uart_inited = 1;
		memcpy(&stdio_uart, obj, sizeof(serial_t));
	}
#endif
}

void serial_pin_init(PinName tx, PinName rx) 
{
	pinmap_pinout(tx, PinMap_UART_TX);
	pinmap_pinout(rx, PinMap_UART_RX);
	pin_mode(tx, PullUp);
	pin_mode(rx, PullUp);
}

/**
  * @brief  Deinitializes the UART device, include clock/function/interrupt/UART registers.
  * @param  obj: uart object define in application software.
  * @retval none  
  */
void serial_free(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter = &(uart_adapter[obj->uart_idx]);

	UART_DeInit(puart_adapter->UARTx);

	InterruptDis(puart_adapter->IrqNum);
	InterruptUnRegister(puart_adapter->IrqNum);
	
#ifdef UART_USE_GTIMER_TO
	uart_gtimer_deinit();
#endif

#ifdef CONFIG_GDMA_EN
	if (serial_dma_en[obj->uart_idx] & SERIAL_RX_DMA_EN) {
		GDMA_ChnlFree(puart_adapter->UARTRxGdmaInitStruct.GDMA_Index,
		puart_adapter->UARTRxGdmaInitStruct.GDMA_ChNum);	
		serial_dma_en[obj->uart_idx] &= ~SERIAL_RX_DMA_EN;
	}

	if (serial_dma_en[obj->uart_idx] & SERIAL_TX_DMA_EN) {
		GDMA_ChnlFree(puart_adapter->UARTTxGdmaInitStruct.GDMA_Index,
		puart_adapter->UARTTxGdmaInitStruct.GDMA_ChNum);
		serial_dma_en[obj->uart_idx] &= ~SERIAL_TX_DMA_EN;
	}    
#endif
	// TODO: recovery Pin Mux

}

/**
  * @brief  Set UART device baudrate.
  * @param  obj: uart object define in application software.
  * @param  baudrate: Baud Rate Val, like 115200 (unit is HZ).
  * @retval none  
  */

void serial_baud(serial_t *obj, int baudrate)
{
	current_baudrate=baudrate;
	PMBED_UART_ADAPTER puart_adapter = &(uart_adapter[obj->uart_idx]);
	
	UART_SetBaud(puart_adapter->UARTx, baudrate);
	UART_RxCmd(puart_adapter->UARTx, ENABLE);

	if (baudrate <= 500000) {
		if (uart_config[obj->uart_idx].LOW_POWER_RX_ENABLE) {
			UART_MonitorParaConfig(puart_adapter->UARTx, 100, ENABLE);
			UART_RxMonitorCmd(puart_adapter->UARTx, ENABLE);
			RCC_PeriphClockSource_UART(puart_adapter->UARTx, UART_RX_CLK_OSC_LP);
			UART_LPRxBaudSet(puart_adapter->UARTx, baudrate, 2000000);
			UART_RxCmd(puart_adapter->UARTx, ENABLE);
		}
	}
}

/**
  * @brief  Set UART format.
  * @param  obj: uart object define in application software.
  * @param  data_bits: data bits, this parameter can be one of the following values:
  *		@arg 7
  *		@arg 8
  * @param  parity: this parameter can be one of the following values:
  *		@arg ParityNone
  *		@arg ParityOdd
  *		@arg ParityEven
  *		@arg ParityForced1: same action with ParityOdd
  *		@arg ParityForced0: same action with ParityEven
  * @param  stop_bits: this parameter can be one of the following values:
  *		@arg 2
  *		@arg 1
  * @retval none  
  */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) 
{
	PMBED_UART_ADAPTER puart_adapter = &(uart_adapter[obj->uart_idx]);
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;

	UART_RxCmd(puart_adapter->UARTx, DISABLE);
    
	if (data_bits == 8) {
		puart_adapter->UART_InitStruct.WordLen = RUART_WLS_8BITS;
	} else {
		puart_adapter->UART_InitStruct.WordLen = RUART_WLS_7BITS;
	}

	switch (parity) {
	case ParityOdd:
	case ParityForced0:
		puart_adapter->UART_InitStruct.Parity = RUART_PARITY_ENABLE;
		puart_adapter->UART_InitStruct.ParityType = RUART_ODD_PARITY;
	break;
	case ParityEven:
	case ParityForced1:
		puart_adapter->UART_InitStruct.Parity = RUART_PARITY_ENABLE;
		puart_adapter->UART_InitStruct.ParityType = RUART_EVEN_PARITY;
	break;
	default: // ParityNone
		puart_adapter->UART_InitStruct.Parity = RUART_PARITY_DISABLE;
	break;
	}

	if (stop_bits == 2) {
		puart_adapter->UART_InitStruct.StopBit = RUART_STOP_BIT_2;
	} else {
		puart_adapter->UART_InitStruct.StopBit = RUART_STOP_BIT_1;
	}

	UARTx->LCR = ((puart_adapter->UART_InitStruct.WordLen) |
		(puart_adapter->UART_InitStruct.StopBit << 2) |
		(puart_adapter->UART_InitStruct.Parity << 3) |
		(puart_adapter->UART_InitStruct.ParityType << 4) |
		(puart_adapter->UART_InitStruct.StickParity << 5));

	UART_RxCmd(puart_adapter->UARTx, ENABLE);
}

/**
  * @brief  Set UART interrupt handler if needed.
  * @param  obj: uart object define in application software.
  * @param  handler: interrupt callback function
  * @param  id: interrupt callback parameter
  * @retval none  
  */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) 
{
	PMBED_UART_ADAPTER puart_adapter;
	u8 uart_idx;

	puart_adapter = &(uart_adapter[obj->uart_idx]);

	uart_idx = puart_adapter->UartIndex;

	irq_handler[uart_idx] = handler;
	serial_irq_ids[uart_idx] = id;        
}

/**
  * @brief  Enable/Disable UART interrupt.
  * @param  obj: uart object define in application software.
  * @param  irq: Tx or Rx interrupt, this parameter can be one of the following values:
  *		@arg RxIrq
  *		@arg TxIrq
  * @param  enable: this parameter can be one of the following values:
  *		@arg 0 disable
  *		@arg 1 enable
  * @retval none  
  */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) 
{
	PMBED_UART_ADAPTER puart_adapter;
	u8 uart_idx;

	puart_adapter = &(uart_adapter[obj->uart_idx]);
	uart_idx = puart_adapter->UartIndex;

	if (enable) {
		if (irq == RxIrq) {
			serial_irq_en[uart_idx] |= SERIAL_RX_IRQ_EN;
			UART_INTConfig(puart_adapter->UARTx, RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI, ENABLE);
		}
		else {
			serial_irq_en[uart_idx] |= SERIAL_TX_IRQ_EN;
		}
	} 
	else { // disable
		if (irq == RxIrq) {
			serial_irq_en[uart_idx] &= ~SERIAL_RX_IRQ_EN;
			UART_INTConfig(puart_adapter->UARTx, (RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI), DISABLE);
		}
		else {
			serial_irq_en[uart_idx] &= ~SERIAL_TX_IRQ_EN;
			UART_INTConfig(puart_adapter->UARTx, RUART_IER_ETBEI, DISABLE);
		}
	}
}

/**
  * @brief  Enable/Disable UART loopback mode.
  * @param  obj: uart object define in application software.
  * @param  enable: enable/disable the loopback mode
  *             @arg 0 disable
  *             @arg 1 enable
  * @retval none
  */
void serial_control_loopback(serial_t *obj, bool enable)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	if (enable) {
		UARTx->MCR |= BIT(4);
	} else {
		UARTx->MCR &= ~BIT(4);
	}
}
/**
  * @brief  get one byte from UART.
  * @param  obj: uart object define in application software.
  * @retval : received character
  * @note this function is asynchronous API.
  */
int serial_getc(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	u8 RxByte = 0;

	UART_CharGet(puart_adapter->UARTx, &RxByte);

	return (int)RxByte;
}

/**
  * @brief  send one byte use UART.
  * @param  obj: uart object define in application software.
  * @param  c: the data to transmit.
  * @retval none
  * @note this function is asynchronous API.
  */
void serial_putc(serial_t *obj, int c) 
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);

	UART_CharPut(puart_adapter->UARTx, (c & 0xFF));

	if (serial_irq_en[obj->uart_idx] & SERIAL_TX_IRQ_EN) {
		// UnMask TX FIFO empty IRQ
		UART_INTConfig(puart_adapter->UARTx, RUART_IER_ETBEI, ENABLE);
	}
}

/**
  * @brief  check if there is data in rx fifo.
  * @param  obj: uart object define in application software.
  * @retval status value:
  *          - 1: TRUE
  *          - 0: FALSE   
  */
int serial_readable(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);

	if (UART_Readable(puart_adapter->UARTx)) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  check if write data to tx fifo is permitted.
  * @param  obj: uart object define in application software.
  * @retval status value:
  *          - 1: TRUE
  *          - 0: FALSE   
  */
int serial_writable(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);

	if (UART_Writable(puart_adapter->UARTx)) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  check if transmit fifo is empty
  * @param  obj: uart object define in application software.
  * @retval status value:
  *          - 1: TRUE
  *          - 0: FALSE   
  */
int serial_tx_empty(serial_t *obj)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);

	if (puart_adapter->UARTx->LSR & RUART_LINE_STATUS_REG_TEMT) {
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief  Clear Rx fifo.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_clear(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter;

	puart_adapter = &(uart_adapter[obj->uart_idx]);

	UART_ClearRxFifo(puart_adapter->UARTx);
}

/**
  * @brief  set tx pinmux.
  * @param  tx: Tx PinName according to pinmux spec.
  * @retval none
  */
void serial_pinout_tx(PinName tx) 
{
	Pinmux_Config(tx, PINMUX_FUNCTION_UART);
}

/**
  * @brief  enable UART break control function.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_break_set(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);

	UART_BreakCtl(puart_adapter->UARTx, 1);
}

/**
  * @brief  disable UART break control function.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_break_clear(serial_t *obj) 
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	
	UART_BreakCtl(puart_adapter->UARTx, 0);
}

/**
  * @brief  set TX complete handler.
  * @param  obj: uart object define in application software.
  * @param  handler: TX complete callback function.
  * @param  id: TX complete callback function parameter.
  * @retval none
  * @note this function is used when asynchronous API is used.
  */
void serial_send_comp_handler(serial_t *obj, void *handler, uint32_t id) 
{
    PMBED_UART_ADAPTER puart_adapter;

    puart_adapter = &(uart_adapter[obj->uart_idx]);
    puart_adapter->TxCompCallback = (UARTCB_FUN)handler;
    puart_adapter->TxCompCbPara = (void*)id;
}

/**
  * @brief  set RX complete handler.
  * @param  obj: uart object define in application software.
  * @param  handler: RX complete callback function.
  * @param  id: RX complete callback function parameter.
  * @retval none
  * @note this function is used when asynchronous API is used.
  */
void serial_recv_comp_handler(serial_t *obj, void *handler, uint32_t id) 
{
    PMBED_UART_ADAPTER puart_adapter;

    puart_adapter = &(uart_adapter[obj->uart_idx]);
    puart_adapter->RxCompCallback = (UARTCB_FUN)handler;
    puart_adapter->RxCompCbPara = (void*)id;
}

/**
  * @brief  recv target length data use interrupt mode.
  * @param  obj: uart object define in application software.
  * @param  prxbuf: buffer to save data read from UART FIFO.
  * @param  len: number of data to be read.
  * @retval HAL_Status
  * @note this function is asynchronous API.
  */
int32_t serial_recv_stream (serial_t *obj, char *prxbuf, uint32_t len)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	int ret = HAL_OK;
	u32 TransCnt = 0;
	
	assert_param(prxbuf != NULL);
	assert_param(len != 0);
	
	if (UART_GetRxFlag(puart_adapter->UartIndex)) {
		DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart int rx: busy\n");
		return HAL_BUSY;
	}

	obj->rx_len = len;
	puart_adapter->pRxBuf = (uint8_t*)prxbuf;
	puart_adapter->RxCount = len;

	UART_SetRxFlag(puart_adapter->UartIndex, STATERX_INT);

	// Could be the RX FIFO has some data already
	TransCnt = UART_ReceiveDataTO(puart_adapter->UARTx, puart_adapter->pRxBuf,
		puart_adapter->RxCount, 1);
	puart_adapter->RxCount -= TransCnt;
	puart_adapter->pRxBuf += TransCnt;

	if (puart_adapter->RxCount == 0) {
		uart_intrecv_complete(puart_adapter);
	} else {
		// Enable RX Interrupt
		UART_INTConfig(puart_adapter->UARTx, RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI, ENABLE);
	}

	return (ret);
}

/**
  * @brief  send target length data use interrupt mode.
  * @param  obj: uart object define in application software.
  * @param  ptxbuf: buffer to be written to Tx FIFO.
  * @param  len: number of data to be send.
  * @retval HAL_Status
  * @note this function is asynchronous API.
  */
int32_t serial_send_stream (serial_t *obj, char *ptxbuf, uint32_t len)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	int ret = 0;
	int cnt=16;

	assert_param(ptxbuf != NULL);
	assert_param(len != 0);
	
	if (UART_GetTxFlag(puart_adapter->UartIndex)) {
		DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart int tx: busy\n");
		return HAL_BUSY;
	}

	obj->tx_len = len;
	puart_adapter->pTxBuf = (uint8_t*)ptxbuf;
	puart_adapter->TxCount = len;

	UART_SetTxFlag(puart_adapter->UartIndex, STATETX_INT);

	while(cnt>0&&puart_adapter->TxCount>0){
		UART_CharPut(puart_adapter->UARTx, *puart_adapter->pTxBuf);
		puart_adapter->TxCount --;
		puart_adapter->pTxBuf ++;
		cnt--;
	}
	
	if (0 == puart_adapter->TxCount) {
		uart_intsend_complete(puart_adapter);
	} else {
		// Enable Tx FIFO empty interrupt
		UART_INTConfig(puart_adapter->UARTx, RUART_IER_ETBEI, ENABLE);
	}
	
	return (ret);
}

/**
  * @brief  recv target length data use DMA mode.
  * @param  obj: uart object define in application software.
  * @param  prxbuf: buffer to save data read from UART FIFO.
  * @param  len: number of data to be read.
  * @retval HAL_Status
  * @note this function is asynchronous API.
  */
int32_t serial_recv_stream_dma (serial_t *obj, char *prxbuf, uint32_t len)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	HAL_Status ret = HAL_OK;
	u32 ret1;

	assert_param(prxbuf != NULL);
	assert_param(len != 0);
	
	if (UART_GetRxFlag(puart_adapter->UartIndex)) {
		DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart dma rx: busy\n");
		return HAL_BUSY;
	}

	obj->rx_len = len;
	puart_adapter->pRxBuf = (uint8_t*)prxbuf;
	puart_adapter->RxCount = len;
	puart_adapter->last_dma_addr = (u32)prxbuf;

	UART_SetRxFlag(puart_adapter->UartIndex, STATERX_DMA);

	// Disable Rx interrupt
	UART_INTConfig(puart_adapter->UARTx, (RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI), DISABLE);
	UART_RXDMAConfig(puart_adapter->UARTx, 4);
	UART_RXDMACmd(puart_adapter->UARTx, ENABLE);

	ret1 = UART_RXGDMA_Init(puart_adapter->UartIndex, &puart_adapter->UARTRxGdmaInitStruct,
		puart_adapter, uart_dmarecv_irqhandler,
		puart_adapter->pRxBuf, puart_adapter->RxCount);

	NVIC_SetPriority(GDMA_GetIrqNum(0, puart_adapter->UARTRxGdmaInitStruct.GDMA_ChNum), 12);	
	
	if ((serial_dma_en[obj->uart_idx] & SERIAL_RX_DMA_EN)==0) {
		if(ret1 == _TRUE) {
			serial_dma_en[obj->uart_idx] |= SERIAL_RX_DMA_EN;
		} else {
			return HAL_BUSY;
		}
	}
	
	#ifdef UART_USE_GTIMER_TO
	uart_gtimer_init(puart_adapter, UART_TIMER_TO);
	RTIM_Cmd(TIMx[UART_TIMER_ID], ENABLE);
	#endif
	
	return (ret);
}

/**
  * @brief  send target length data use DMA mode.
  * @param  obj: uart object define in application software.
  * @param  ptxbuf: buffer to be written to Tx FIFO.
  * @param  len: number of data to be send.
  * @retval : HAL_Status
  * @note this function is asynchronous API.
  */
int32_t serial_send_stream_dma (serial_t *obj, char *ptxbuf, uint32_t len)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	int32_t ret = HAL_OK;
	u32 ret1;

	assert_param(ptxbuf != NULL);
	assert_param(len != 0);

	if (UART_GetTxFlag(puart_adapter->UartIndex)) {
		DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart dma tx: busy\n");
		return HAL_BUSY;
	}

	obj->tx_len = len;
	puart_adapter->pTxBuf = (uint8_t*)ptxbuf;
	puart_adapter->TxCount = len;

	UART_SetTxFlag(puart_adapter->UartIndex, STATETX_DMA);

	UART_TXDMAConfig(puart_adapter->UARTx, 8);
	UART_TXDMACmd(puart_adapter->UARTx, ENABLE);

	ret1 = UART_TXGDMA_Init(puart_adapter->UartIndex, &puart_adapter->UARTTxGdmaInitStruct,
		puart_adapter, uart_dmasend_complete,
		puart_adapter->pTxBuf, puart_adapter->TxCount);

	NVIC_SetPriority(GDMA_GetIrqNum(0, puart_adapter->UARTTxGdmaInitStruct.GDMA_ChNum), 12);	

	if ((serial_dma_en[obj->uart_idx] & SERIAL_TX_DMA_EN)==0) {
		if(ret1 == _TRUE) {
			serial_dma_en[obj->uart_idx] |= SERIAL_TX_DMA_EN;
		} else {
			return HAL_BUSY;
		}
	}
	
	return (ret);
}

/**
  * @brief  stop the stream or stream_dma TX.
  * @param  obj: uart object define in application software.
  * @retval : number of bytes sent before stop
  */
int32_t serial_send_stream_abort (serial_t *obj)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	int ret = 0;

	if (!UART_GetTxFlag(puart_adapter->UartIndex)) {
		//DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart tx abort: Not in TX state \n");
		return HAL_OK;
	}

	// Disable Tx FIFO empty interrupt
	UART_INTConfig(puart_adapter->UARTx, RUART_IER_ETBEI, DISABLE);
	
	if(UART_GetTxFlag(puart_adapter->UartIndex) == STATERX_DMA) {
		if (uart_config[obj->uart_idx].LOW_POWER_RX_ENABLE == DISABLE) {
			u32 Current_Addr;
			PGDMA_InitTypeDef GDMA_InitStruct = &puart_adapter->UARTTxGdmaInitStruct;

			Current_Addr = GDMA_GetSrcAddr(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
			GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);

			puart_adapter->TxCount = puart_adapter->TxCount - (Current_Addr - (u32)puart_adapter->pTxBuf);
			puart_adapter->pTxBuf = (u8*)Current_Addr;
			
			// Clean Auto Reload Bit
			GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_DST);
			// Clear Pending ISR
			GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
			//GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
			GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

			UART_TXDMACmd(puart_adapter->UARTx, DISABLE);
		}
	}

	UART_SetTxFlag(puart_adapter->UartIndex, 0);

	UART_ClearTxFifo(puart_adapter->UARTx);

	ret = obj->tx_len - puart_adapter->TxCount;
	
	return (ret);
}

/**
  * @brief  stop the stream or stream_dma RX.
  * @param  obj: uart object define in application software.
  * @retval : number of bytes received before stop
  */
int32_t serial_recv_stream_abort (serial_t *obj)
{
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	int ret = 0;

	if (!UART_GetRxFlag(puart_adapter->UartIndex)) {
		//DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart rx abort: Not in TX state \n");
		return HAL_OK;
	}

	// Disable Rx interrupt
	UART_INTConfig(puart_adapter->UARTx, (RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI), DISABLE);

	if(UART_GetRxFlag(puart_adapter->UartIndex) == STATERX_DMA) {
		if (uart_config[obj->uart_idx].LOW_POWER_RX_ENABLE == DISABLE) {
			u32 Current_Addr;
			u32 TransCnt;
			PGDMA_InitTypeDef GDMA_InitStruct = &puart_adapter->UARTRxGdmaInitStruct;

			/*when stream DMA mode used, some data may be in uart rx fifo, get it if transmission aborted*/
			Current_Addr = GDMA_GetDstAddr(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
			GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
			
			puart_adapter->RxCount = puart_adapter->RxCount - (Current_Addr - (u32)puart_adapter->pRxBuf);
			puart_adapter->pRxBuf = (u8*)Current_Addr;

			TransCnt = UART_ReceiveDataTO(puart_adapter->UARTx, puart_adapter->pRxBuf,
				puart_adapter->RxCount, 1);
			puart_adapter->RxCount -= TransCnt;
			puart_adapter->pRxBuf += TransCnt;
					
			// Clean Auto Reload Bit
			GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_DST);
			// Clear Pending ISR
			GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
			//GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
			GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
			UART_RXDMACmd(puart_adapter->UARTx, DISABLE);		
		}
	}
	
	UART_SetRxFlag(puart_adapter->UartIndex, 0);

	UART_ClearRxFifo(puart_adapter->UARTx);

	ret = obj->rx_len - puart_adapter->RxCount;

	return (ret);
}

/**
  * @brief  Clear TX fifo.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_clear_tx(serial_t *obj)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	
	UART_ClearTxFifo(UARTx);
}

/**
  * @brief  Clear RX fifo.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_clear_rx(serial_t *obj)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	
	UART_ClearRxFifo(UARTx);
}

/**
  * @brief  recv target length data use poll mode, with time out.
  * @param  obj: uart object define in application software.
  * @param  ptxbuf: buffer to be written to Tx FIFO.
  * @param  len: number of data to be recv.
  * @param  timeout_ms: polling time before timeout.
  * @retval : return received bytes count
  * @note this function is synchronous API.
  */
int32_t serial_recv_blocked (serial_t *obj, char *prxbuf, uint32_t len, uint32_t timeout_ms)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	uint32_t cnt = 0;
	uint32_t startcount = SYSTIMER_TickGet();

	obj->rx_len = len;
	while (1) {
		while (!serial_readable(obj));
		UART_CharGet(UARTx, (u8 *)prxbuf);

		prxbuf++;
		cnt++;

		if (cnt == len) {
			break;
		}

		if (SYSTIMER_GetPassTime(startcount) > timeout_ms) {
			break;
		}
	}

	return cnt;
}

/**
  * @brief  send target length data use poll mode, with time out.
  * @param  obj: uart object define in application software.
  * @param  ptxbuf: buffer to be written to Tx FIFO.
  * @param  len: number of data to be send.
  * @param  timeout_ms: polling time before timeout.
  * @retval : transmitted bytes count
  * @note this function is synchronous API.
  */
int32_t serial_send_blocked (serial_t *obj, char *ptxbuf, uint32_t len, uint32_t timeout_ms)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	uint32_t cnt = 0;
	uint32_t startcount = SYSTIMER_TickGet();

	obj->tx_len = len;
	while (1) {
		UART_CharPut(UARTx, *ptxbuf);

		ptxbuf++;
		cnt++;

		if (cnt == len) {
			break;
		}

		if (SYSTIMER_GetPassTime(startcount) > timeout_ms) {
			break;
		}
	}

	return cnt;
}

/**
  * @brief  disable uart clock and function.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_disable (serial_t *obj)
{
	switch (obj->uart_idx) {
	case 0:
		/* UART 0 */
		RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, DISABLE);
	break;

	case 1:
		/* UART 1 */
		RCC_PeriphClockCmd(APBPeriph_UART1, APBPeriph_UART1_CLOCK, DISABLE);
	break;
	default:
	break;
	}

}

/**
  * @brief  enable uart clock and function.
  * @param  obj: uart object define in application software.
  * @retval none
  */
void serial_enable (serial_t *obj)
{
	switch (obj->uart_idx) {
	case 0:
		/* UART 0 */
		RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
	break;

	case 1:
		/* UART 1 */
		RCC_PeriphClockCmd(APBPeriph_UART1, APBPeriph_UART1_CLOCK, ENABLE);
	break;
	default:
	break;
	}
}


/**
  * @brief  recv target length data use interrupt mode.
  * @param  obj: uart object define in application software.
  * @param  prxbuf: buffer to save data read from UART FIFO.
  * @param  len: number of data to be recv.
  * @param  timeout_ms: polling time before timeout.
  * @param  force_cs: forcing context switch function.
  * @retval : the byte count received before timeout, or error(<0)
  * @note this function is asynchronous API.
  */
int32_t serial_recv_stream_timeout (serial_t *obj, 
	char *prxbuf, 
	uint32_t len,
	uint32_t timeout_ms,
	void *force_cs)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	u32 TransCnt = 0;
	void (*task_yield)(void);
	uint32_t startcount = SYSTIMER_TickGet();
	
	assert_param(prxbuf != NULL);
	assert_param(len != 0);
	assert_param(timeout_ms > 0);
	
	//ret = HalRuartRecv(puart_adapter, (u8*)prxbuf, len, INT_BASED);

	if (UART_GetRxFlag(obj->uart_idx)) {
		DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart int rx: busy\n");
		return -1;
	}

	puart_adapter->pRxBuf = (uint8_t*)prxbuf;
	puart_adapter->RxCount = len;

	UART_SetRxFlag(obj->uart_idx, STATERX_INT);

	// Could be the RX FIFO has some data already
	TransCnt = UART_ReceiveDataTO(UARTx, puart_adapter->pRxBuf,
		puart_adapter->RxCount, 1);
	puart_adapter->RxCount -= TransCnt;
	puart_adapter->pRxBuf += TransCnt;

	if (puart_adapter->RxCount == 0) {
		uart_intrecv_complete(puart_adapter);
		return len;
	} else {
		// Enable RX Interrupt
		UART_INTConfig(UARTx, RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI, ENABLE);
	}

	if (puart_adapter->RxCount > 0) {
		task_yield = (void (*)(void))force_cs;
		while (1) {
			/* complete */
			if (UART_GetRxFlag(obj->uart_idx) != STATERX_INT) {
				break;
			}

			/* time out */
			if (SYSTIMER_GetPassTime(startcount) > timeout_ms) {
				serial_recv_stream_abort(obj);
			}

			if (NULL != task_yield) {
				task_yield();
			}
		}
	}

	return (len - puart_adapter->RxCount);
}

/**
  * @brief  recv data use DMA mode with hardware timeout and uart is flow controller.
  * @param  obj: uart object define in application software.
  * @param  prxbuf: buffer to save data read from UART FIFO.
  * @param  len: not use, reserved for compatibility.
  * @param  timeout_ms: hardware timeout for rx.
  * @param  force_cs: not use, reserved for compatibility,it should be NULL.
  * @retval HAL_Status
  * @note this function is asynchronous API.
  */

int32_t serial_recv_stream_dma_timeout(serial_t *obj, 
	char *prxbuf,
	uint32_t len,
	uint32_t timeout_ms,
	void *force_cs)
{
	/* To avoid gcc warnings */
	( void ) force_cs;
	( void ) len;
	
	PMBED_UART_ADAPTER puart_adapter=&(uart_adapter[obj->uart_idx]);
	HAL_Status ret = HAL_OK;
	u32 ret1;
	u32 timeout_count;

	timeout_count=timeout_ms*current_baudrate/1000;
	
	assert_param(prxbuf != NULL);
	assert_param(timeout_count>=64&&timeout_count<65536);

	if (UART_GetRxFlag(puart_adapter->UartIndex)) {
		DBG_PRINTF(MODULE_UART, LEVEL_WARN, "uart dma rx: busy\n");
		return HAL_BUSY;
	}

	puart_adapter->pRxBuf = (uint8_t*)prxbuf;
	puart_adapter->last_dma_addr = (u32)prxbuf;
	
	if(0!=timeout_count){
		int regval=puart_adapter->UARTx->RX_PATH;
		regval=(timeout_count<<16)|(0xffff&regval);
		puart_adapter->UARTx->RX_PATH=regval;
	}
	
	UART_SetRxFlag(puart_adapter->UartIndex, STATERX_DMA);


	// Disable Rx interrupt
	UART_INTConfig(puart_adapter->UARTx, (RUART_IER_ERBI | RUART_IER_ELSI | RUART_IER_ETOI), DISABLE);
	UART_RXDMAConfig(puart_adapter->UARTx, 4);
	UART_RXDMACmd(puart_adapter->UARTx, ENABLE);


	ret1 = UART_RXGDMA_Init(puart_adapter->UartIndex, &puart_adapter->UARTRxGdmaInitStruct,
		puart_adapter, uart_dmarecv_irqhandler,
		puart_adapter->pRxBuf, 0);

	NVIC_SetPriority(GDMA_GetIrqNum(0, puart_adapter->UARTRxGdmaInitStruct.GDMA_ChNum), 12);	

	if ((serial_dma_en[obj->uart_idx] & SERIAL_RX_DMA_EN)==0) {
		if(ret1 == _TRUE) {
			serial_dma_en[obj->uart_idx] |= SERIAL_RX_DMA_EN;
		} else {
			return HAL_BUSY;
		}
	}	
	return (ret);
}

/**
  * @brief  set uart rx fifo trigger level.
  * @param  obj: uart object define in application software.
  * @param  FifoLv: FIFO level enum index.
  * @retval   none
  */
void serial_rx_fifo_level(serial_t *obj, SerialFifoLevel FifoLv)
{
	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	u32 Temp = 0;

	switch(FifoLv) {
		case FifoLv1Byte:
			Temp = UART_RX_FIFOTRIG_LEVEL_1BYTES;
		break;
		case FifoLvQuarter:
			Temp = UART_RX_FIFOTRIG_LEVEL_4BYTES;
		break;
		case FifoLvHalf:
			Temp = UART_RX_FIFOTRIG_LEVEL_8BYTES;
		break;
		case FifoLvFull:
			Temp = UART_RX_FIFOTRIG_LEVEL_14BYTES;
		break;		
		default:
			assert_param(0);
		break;
	}

	/*set rx fifo level*/
	UART_SetRxLevel(UARTx, Temp);
}

/**
  * @brief  uart autoflow control setting.
  * @param  obj: uart object define in application software.
  * @param  type: autoflow control type.
  * @param  rxflow: RTS pin.
  * @param  txflow: CTS pin.
  * @retval   none
  */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
	/* To avoid gcc warnings */
	( void ) txflow;

	UART_TypeDef* UARTx = UART_DEV_TABLE[obj->uart_idx].UARTx;
	u32 Temp;

	/*autoflow control configuration*/
	if(type != FlowControlNone) {
		/*config UART0 pinmux*/
		if(rxflow == PA_14) {
			/*KM0 UART "S0"  */
			Pinmux_Config(_PA_14, PINMUX_FUNCTION_UART_RTSCTS);	/*RTS*/
			Pinmux_Config(_PA_15, PINMUX_FUNCTION_UART_RTSCTS);	/*CTS*/
		} else if (rxflow == PA_16) {
			/*KM4 UART "S0"  */
			Pinmux_Config(_PA_16, PINMUX_FUNCTION_UART_RTSCTS); 	/*RTS*/
			Pinmux_Config(_PA_17, PINMUX_FUNCTION_UART_RTSCTS); 	/*CTS*/
		} else if (rxflow == PA_27) {
			/*if using PA27, SWD should be closed first*/
			Temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_SWD_PMUX_EN);
			Temp &= (~BIT_LSYS_SWD_PMUX_EN);
			HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_SWD_PMUX_EN, Temp);
			Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_SWD_PMUX_EN);	
			Temp &= (~BIT_LSYS_SWD_PMUX_EN);	
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_SWD_PMUX_EN, Temp);
			
			/*KM0 UART  "S1"  */
			Pinmux_Config(_PA_27, PINMUX_FUNCTION_UART_RTSCTS); 	/*RTS*/
			Pinmux_Config(_PA_28, PINMUX_FUNCTION_UART_RTSCTS); 	/*CTS*/
		} else if (rxflow == PA_10) {
			/*KM4 UART  "S1"  */
			Pinmux_Config(_PA_23, PINMUX_FUNCTION_UART_RTSCTS); 	/*RTS*/
			Pinmux_Config(_PA_24, PINMUX_FUNCTION_UART_RTSCTS); 	/*CTS*/
		} else if (rxflow == PB_11) {
			/*KM4 UART  "S2"  */
			Pinmux_Config(_PB_11, PINMUX_FUNCTION_UART_RTSCTS); 	/*RTS*/
			Pinmux_Config(_PB_10, PINMUX_FUNCTION_UART_RTSCTS); 	/*CTS*/
		} else if (rxflow == PB_21) {
			/*KM4 UART "S3"  */
			Pinmux_Config(_PB_21, PINMUX_FUNCTION_UART_RTSCTS); 	/*RTS*/
			Pinmux_Config(_PB_20, PINMUX_FUNCTION_UART_RTSCTS); 	/*CTS*/
		} else {
			assert_param(0);
		}
		/*enable auto flow control*/
		UARTx->MCR |= BIT(5);
	} else {
		/*disable auto flow control*/
		UARTx->MCR &= ~ BIT(5);
	}

	/*no matter auto flow control is enabled or disabled,
	RTS pin should be always Low, and thus peer can send data*/
	UARTx->MCR |= BIT(1);

}
/** 
  * @}
  */

/** 
  * @}
  */

/** 
  * @}
  */

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
