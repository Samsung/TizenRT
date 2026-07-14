/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <osif.h>
#include <string.h>
#include <stdbool.h>
#include "ameba_soc.h"
#include "hci_uart.h"
#include "bt_debug.h"
#include "hal_platform.h"
#include "hci_platform.h"
#include "hci_controller.h"

#define HCI_UART_IDX             (3)
#define HCI_UART_DEV             (UART3_DEV)
#define HCI_UART_IRQ             (UART3_BT_IRQ)
#define HCI_UART_IRQ_PRIO        (INT_PRI_LOWEST)

/* Zi Yik Ringbuffer */
static struct hci_uart_t {
	/* UART */
	UART_InitTypeDef UART_InitStruct;

	/* UART TX */
	uint8_t         *tx_buf;
	uint16_t         tx_len;
	void            *tx_done_sem;
} *g_uart = NULL;

uint8_t hci_uart_set_bdrate(uint32_t baudrate)
{
	UART_SetBaud(HCI_UART_DEV, baudrate);
	BT_LOGA("Set baudrate to %d success!\r\n", (int)baudrate);
	return HCI_SUCCESS;
}

static inline void transmit_chars(void)
{
	uint16_t cnt = 0;

	if (g_uart->tx_len == 0) { /* Set TX done after TX FIFO is empty. */
		UART_INTConfig(HCI_UART_DEV, RUART_BIT_ETBEI, DISABLE);
		if (g_uart->tx_done_sem) {
			osif_sem_give(g_uart->tx_done_sem);
		}
		return;
	}

	/* Send data to TX FIFO */
	cnt = (uint16_t)UART_SendDataTO(HCI_UART_DEV, g_uart->tx_buf, g_uart->tx_len, 0);
	g_uart->tx_buf += cnt;
	g_uart->tx_len -= cnt;
}

static inline void receive_chars(void)
{
	hci_uart_rx_irq_handler(true);
}

static uint32_t _uart_irq(void *data)
{
	(void)data;
	uint32_t reg_lsr = UART_LineStatusGet(HCI_UART_DEV);
	uint32_t reg_ier = HCI_UART_DEV->IER;

	if (UART_Readable(HCI_UART_DEV)) {
		receive_chars();
	}

	if ((reg_lsr & RUART_BIT_TX_EMPTY) && (reg_ier & RUART_BIT_ETBEI)) {
		transmit_chars();
	}

	if (reg_lsr & RUART_BIT_RXND_INT) {
		UART_INT_Clear(HCI_UART_DEV, RUART_BIT_RXNDICF);
	}

	if (reg_lsr & RUART_BIT_MODEM_INT) {
		UART_INT_Clear(HCI_UART_DEV, RUART_BIT_MICF);
	}

	if ((reg_lsr & RUART_BIT_TIMEOUT_INT) && (!UART_Readable(HCI_UART_DEV))) {
		UART_INT_Clear(HCI_UART_DEV, RUART_BIT_TOICF);
	}

	if ((reg_lsr & UART_ALL_RX_ERR)) {
		UART_INT_Clear(HCI_UART_DEV, RUART_BIT_RLSICF);
	}

	return 0;
}

uint16_t hci_uart_send(uint8_t *buf, uint16_t len)
{
	if (!g_uart) {
		BT_LOGE("g_uart is NULL!\r\n");
		return 0;
	}

	/* UART_SendData() does not work */
	g_uart->tx_buf = buf;
	g_uart->tx_len = len;

	if (!HCI_BT_KEEP_AWAKE) {
		/* acquire host wake bt */
		set_reg_value(0x42008250, BIT13, 1); /* enable HOST_WAKE_BT */
	}

	UART_INTConfig(HCI_UART_DEV, RUART_BIT_ETBEI, ENABLE);

	if (g_uart->tx_done_sem) {
		if (osif_sem_take(g_uart->tx_done_sem, 0xFFFFFFFF) == false) {
			BT_LOGE("g_uart->tx_done_sem take fail!\r\n");
			len = 0;
		}
	}

	if (!HCI_BT_KEEP_AWAKE) {
		/* release host wake bt */
		set_reg_value(0x42008250, BIT13, 0); /* disable HOST_WAKE_BT */
	}

	/* Trigger TX Empty Interrrupt, so TX done here */
	return len;
}

uint16_t hci_uart_read(uint8_t *buf, uint16_t len)
{
	if (len) {
		return UART_ReceiveDataTO(HCI_UART_DEV, buf, len, 0);
	}

	return 0;
}

void hci_uart_rx_irq(bool enable)
{
	UART_INTConfig(HCI_UART_DEV, RUART_BIT_ERBI | RUART_BIT_ETOI, enable ? ENABLE : DISABLE);
}

uint8_t hci_uart_open(void)
{
	/* Init g_uart */
	if (!g_uart) {
		g_uart = (struct hci_uart_t *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct hci_uart_t));
		if (!g_uart) {
			BT_LOGE("g_uart is NULL!\r\n");
			return HCI_FAIL;
		}
		memset(g_uart, 0, sizeof(struct hci_uart_t));
	}

	if (osif_sem_create(&g_uart->tx_done_sem, 0, 1) == false) {
		BT_LOGE("g_uart->tx_done_sem create fail!\r\n");
		osif_mem_free(g_uart);
		return HCI_FAIL;
	}

	/* Enable Clock */
	RCC_PeriphClockCmd(APBPeriph_UARTx[HCI_UART_IDX], APBPeriph_UARTx_CLOCK[HCI_UART_IDX], ENABLE);

	/* Enable UART
	 * Use Flow Control (When rx FIFO reaches level, RTS will be pulled high)
	 * Use Baudrate 115200 (Default)
	 */
	UART_InitTypeDef *pUARTStruct = &g_uart->UART_InitStruct;
	UART_StructInit(pUARTStruct);
	pUARTStruct->WordLen = RUART_WLS_8BITS;
	pUARTStruct->StopBit = RUART_STOP_BIT_1;
	pUARTStruct->Parity = RUART_PARITY_DISABLE;
	pUARTStruct->ParityType = RUART_EVEN_PARITY;
	pUARTStruct->StickParity = RUART_STICK_PARITY_DISABLE;
	pUARTStruct->RxFifoTrigLevel = UART_RX_FIFOTRIG_LEVEL_32BYTES;  /* 16Bytes actually, FIFO depth of Uart3 is only half of Uart0/1/2. */
	pUARTStruct->FlowControl = ENABLE;
	UART_Init(HCI_UART_DEV, pUARTStruct);
	UART_SetBaud(HCI_UART_DEV, 115200);

	/* Disable and Enable UART Interrupt */
	InterruptDis(HCI_UART_IRQ);
	InterruptUnRegister(HCI_UART_IRQ);
	InterruptRegister((IRQ_FUN)_uart_irq, HCI_UART_IRQ, NULL, HCI_UART_IRQ_PRIO);
	InterruptEn(HCI_UART_IRQ, HCI_UART_IRQ_PRIO);

	UART_INTConfig(HCI_UART_DEV, RUART_BIT_ETBEI, DISABLE);
	UART_INTConfig(HCI_UART_DEV, RUART_BIT_ERBI | RUART_BIT_ELSI | RUART_BIT_ETOI, ENABLE);

	/* Enable Uart High Rate Rx Path */
	UART_RxCmd(HCI_UART_DEV, ENABLE);

	return HCI_SUCCESS;
}

uint8_t hci_uart_close(void)
{
	if (!g_uart) {
		return HCI_FAIL;
	}

	/* Disable UART Interrupt and UART */
	InterruptDis(HCI_UART_IRQ);
	InterruptUnRegister(HCI_UART_IRQ);
	UART_DeInit(HCI_UART_DEV);

	return HCI_SUCCESS;
}

uint8_t hci_uart_free(void)
{
	if (!g_uart) {
		return HCI_FAIL;
	}

	if (g_uart->tx_done_sem) {
		osif_sem_delete(g_uart->tx_done_sem);
		g_uart->tx_done_sem = NULL;
	}

	osif_mem_free(g_uart);
	g_uart = NULL;

	return HCI_SUCCESS;
}