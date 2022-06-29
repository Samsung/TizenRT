/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "osif.h"
#include "hci_config.h"
#include "hci/hci_common.h"
#include "hci/hci_if_zephyr.h"
#include "hci/hci_process.h"
#include "hci/hci_transport.h"
#include "hci_uart.h"
#include "hci_platform.h"
#include "hci_dbg.h"

static uint8_t hci_drv_inited = 0;

int hci_drv_init(void)
{
	if (hci_drv_inited)
	{
		HCI_WARN("Hci Driver Already Open!");
		return 0;
	}

	/* BT Board Init */
	if (HCI_FAIL == hci_platform_init())
		return -1;

	/* HCI Transport */
	if (HCI_FAIL == hci_transport_open())
		return -1;

	/* HCI Transport Bridge to StandAlone */
	hci_transport_set_get_buf(hci_sa_recv_get_buf);
	hci_transport_set_recv(hci_sa_recv);

	/* HCI UART Bridge to Transport */
	hci_uart_set_rx_ind(hci_transport_recv_ind);
	hci_uart_set_irq(UART_RX, 1);

	if (HCI_FAIL == hci_process())
		return -1;

    /* HCI UART Bridge to NULL */
    hci_uart_set_irq(UART_RX, 0);
    hci_uart_set_rx_ind(NULL);

    /* Stop HCI Driver Transport and Uart will 
     * be bridged to Zephyr stack Tranport.
     */
    hci_transport_close();
    hci_transport_free();

	hci_drv_inited = 1;
    
    return 0;
}

void hci_drv_stop(void)
{
    /* Stop UART RX and TX */
    /* RX Stop */
    hci_uart_set_irq(UART_RX, 0);
    hci_uart_set_rx_ind(NULL);

    /* TX Stop */
    //hci_uart_set_irq(UART_TX, 0);
}

int hci_drv_deinit(void)
{
    if (!hci_drv_inited)
        return 0;

    hci_platform_deinit();
    hci_uart_free();

    hci_drv_inited = 0;
    return 0;
}

/* Just For Compiler for 8710c (software_reset) */
void hci_tp_close(void)
{
    hci_drv_deinit();
}

