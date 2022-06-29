/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "osif.h"
#include "hci_if_rtk_s.h"
#include "hci/hci_process.h"
#include "hci/hci_transport.h"
#include "hci_platform.h"
#include "hci_dbg.h"

static struct {
	uint8_t* tx_buf;
    uint8_t  status;
} hci_if_rtk = {0, 0};

void hci_tp_open(HCI_OPEN_CB open_cb, HCI_RECV_IND rx_ind)
{
	if (hci_if_rtk.status)
	{
		HCI_ERR("\n\rHci Driver Already Open!");
        goto bailout;
	}

	/* BT Board Init */
	if (HCI_FAIL == hci_platform_init())
        goto bailout;

	/* HCI Transport */
	if (HCI_FAIL == hci_transport_open())
		goto bailout;

	/* HCI Transport Bridge to StandAlone */
	hci_transport_set_get_buf(hci_sa_recv_get_buf);
	hci_transport_set_recv(hci_sa_recv);

	/* HCI UART Bridge to Transport */
	hci_uart_set_rx_ind(hci_transport_recv_ind);
	hci_uart_set_irq(UART_RX, 1);

	if (HCI_FAIL == hci_process())
		goto bailout;

    /* HCI UART Bridge to NULL */
    hci_uart_set_irq(UART_RX, 0);
    hci_uart_set_rx_ind(NULL);

	/* HCI Transport Close and free */
	if (HCI_FAIL == hci_transport_close() || 
		HCI_FAIL == hci_transport_free())
		goto bailout;

	/* HCI Transport Bridge to RTK Stack 
     * (Stop and Start rx_ind for this Moment)
     */
	hci_uart_set_rx_ind(rx_ind);
	hci_uart_set_irq(UART_RX, 1);
	
    hci_if_rtk.status = 1;
    open_cb(true);
    return;

bailout:
    hci_if_rtk.status = 0;
    open_cb(false);
}

void hci_tp_close(void)
{
	if (!hci_if_rtk.status)
		goto bailout;

	/* Platform Deinit First and free */
	if (HCI_FAIL == hci_platform_deinit() || 
		HCI_FAIL == hci_uart_free())
		goto bailout;

    hci_if_rtk.status = 0;
    HCI_PRINT("\n\rHci Driver Closed!");
	return;

bailout:
    hci_if_rtk.status = 1;
    HCI_PRINT("\n\rHci Driver Close Failed!");
}

uint8_t flag_for_hci_trx = 0;
void hci_tp_del(uint8_t param)
{
	if (param == 0) {
		flag_for_hci_trx = 1;
		return ;
	}
	if (param == 1) {
        hci_tp_close();
		flag_for_hci_trx = 0;
		return;
	}
}

bool hci_tp_send(uint8_t *p_buf, uint16_t len, HCI_SEND_CB tx_cb)
{
    if (len != hci_uart_send(p_buf, len))
        return false;

	if (tx_cb)
		tx_cb();

    return true;
}

uint16_t hci_tp_recv(uint8_t *p_buf, uint16_t size)
{
    return hci_uart_read(p_buf, size);
}

void hci_tp_config(uint8_t *p_buf, uint16_t len)
{
    (void)p_buf;
    (void)len;
}
