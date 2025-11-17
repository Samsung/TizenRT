/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/usb/usbdev_trace.h>

#include <arch/irq.h>
#include <tinyara/serial/serial.h>
#include <tinyara/semaphore.h>
#if defined(CONFIG_AMEBASMART_USBDEVICE)
#include "usbd.h"
#include "usb_os.h"
#include "usbd_hal.h"
#include "usbd_pcd.h"
#include "usbd_core.h"
#include "usbd_cdc_acm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#define CONFIG_AMEBASMART_USBD_CDC_ACM_ASYNC_XFER	0
#define CONFIG_AMEBASMART_CDC_ACM_NOTIFY		0
#define CONFIG_AMEBASMART_USBD_CDC_ACM_HOTPLUG		0

// Asynchronous transfer size
#define CONFIG_CDC_ACM_ASYNC_BUF_SIZE			2048U

// Do not change the settings unless indeed necessary
#define CONFIG_CDC_ACM_BULK_IN_XFER_SIZE		2048U
#define CONFIG_CDC_ACM_BULK_OUT_XFER_SIZE		2048U

#if CONFIG_AMEBASMART_USBD_CDC_ACM_ASYNC_XFER
static u8 cdc_acm_async_xfer_buf[CONFIG_CDC_ACM_ASYNC_BUF_SIZE] __attribute_((aligned(CACHE_LINE_SIZE)));
static u16 cdc_acm_async_xfer_buf_pos = 0;
static volatile int cdc_acm_async_xfer_busy = 0;
static rtos_sema_t cdc_acm_async_xfer_sema;
#endif

#if CONFIG_AMEBASMART_USBD_CDC_ACM_HOTPLUG
static u8 cdc_acm_attach_status;
static rtos_sema_t cdc_acm_attach_status_changed_sema;
#endif

#define CONFIG_TEST_TRANSMIT				0

#endif /* CONFIG_AMEBASMART_USBDEVICE */

extern int cdc_acm_ready_flag;
static uint8_t receive_data;
static int received_flag = 0;
static int attach_status = -1;
/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* The various states of a control pipe */
/* Refer to the usbd_core.h */
/*  EP0 State */
#define USBD_EP0_IDLE                                  0x00U    /* No request in progress */
#define USBD_EP0_SETUP                                 0x01U    /* Setup packet received, preparing for OUT transfer */
#define USBD_EP0_DATA_IN                               0x02U    /* Device ready to send data to host (IN transfer) */
#define USBD_EP0_DATA_OUT                              0x03U    /* Host sending data to device (OUT transfer) */
#define USBD_EP0_STATUS_IN                             0x04U    /* Waiting for host to send status handshake (IN transfer) */
#define USBD_EP0_STATUS_OUT                            0x05U    /* Waiting for device to send status handshake (OUT transfer) */
#define USBD_EP0_STALL                                 0x06U    /* Endpoint stalled due to error or unsupported request */

static char g_usbrxbuffer[CONFIG_CDC_ACM_BULK_OUT_XFER_SIZE];
static char g_usbtxbuffer[CONFIG_CDC_ACM_BULK_IN_XFER_SIZE];

static void rtl8730e_log_usb_shutdown(struct uart_dev_s *dev);
static int rtl8730e_log_usb_attach(struct uart_dev_s *dev);
static void rtl8730e_log_usb_detach(struct uart_dev_s *dev);
static int rtl8730e_log_usb_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int rtl8730e_log_usb_receive(struct uart_dev_s *dev, unsigned int *status);
static void rtl8730e_log_usb_rxint(struct uart_dev_s *dev, bool enable);
static bool rtl8730e_log_usb_rxavailable(struct uart_dev_s *dev);
static void rtl8730e_log_usb_send(struct uart_dev_s *dev, int ch);
static void rtl8730e_log_usb_txint(struct uart_dev_s *dev, bool enable);
static bool rtl8730e_log_usb_txready(struct uart_dev_s *dev);
static bool rtl8730e_log_usb_txempty(struct uart_dev_s *dev);

static int rtl8730e_usb_setup(struct uart_dev_s *dev);
#define USB_DEV g_usbport
struct amebasmart_usbdev_s {
	/* Common device fields.  This must be the first thing defined in the
	 * structure so that it is possible to simply cast from struct usbdev_s
	 * to struct amebasmart_usbdev_s.
	 */

	struct _usbd_class_driver_t *driver;	/* Class driver */
	u32 ep0_xfer_total_len;			/* The total data length to transfer */
	u32 ep0_xfer_rem_len;			/* The remain data length to transfer */
	u32 ep0_recv_rem_len;			/* The remain data length to receive */
	u8 *ctrl_buf;				/* Buffer for control transfer */
	void *pcd;				/* PCD handle */
	u16 ep0_data_len;			/* EP0 data length */
	u8 ep0_state;				/* EP0 state */
	u8 dev_config;				/* Device config index */
	u8 dev_speed;				/* Device speed, usb_speed_type_t */
	u8 dev_state;				/* Device state, usbd_state_t */
	u8 dev_old_state;			/* Device old state, usbd_state_t */
	u8 dev_attach_status;			/* Device attach status, usbd_attach_status_t */
	u8 test_mode;				/* Test mode */
	u8 self_powered : 1;			/* Self powered or not, 0-bus powered, 1-self powered */
	u8 remote_wakeup_en : 1;		/* Remote wakeup enable or not, 0-disabled, 1-enabled */
	u8 remote_wakeup : 1;			/* Remote wakeup */
	sem_t txsem;
};
static struct amebasmart_usbdev_s g_usbdev;
/* Serial driver UART operations */
static const struct uart_ops_s log_g_usb_ops = {
	.setup = rtl8730e_usb_setup,
	.shutdown = rtl8730e_log_usb_shutdown,
	.attach = rtl8730e_log_usb_attach,
	.detach = rtl8730e_log_usb_detach,
	.ioctl = rtl8730e_log_usb_ioctl,
	.receive = rtl8730e_log_usb_receive,
	.rxint = rtl8730e_log_usb_rxint,
	.rxavailable = rtl8730e_log_usb_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL //Flowcontrol
	.rxflowcontrol = NULL,
#endif
	.send = rtl8730e_log_usb_send,
	.txint = rtl8730e_log_usb_txint,
	.txready = rtl8730e_log_usb_txready,
	.txempty = rtl8730e_log_usb_txempty,
};

struct rtl8730e_up_dev_s {
	uint8_t parity;				/* 0=none, 1=odd, 2=even */
	uint8_t bits;				/* Number of bits (7 or 8) */
	uint8_t stopbit;			/* Number of StopBit (1 or 2) */
	uint32_t baud;				/* Configured baud rate */
	uint32_t irq;				/* IRQ associated with this UART */
	uint8_t FlowControl;
	bool txint_enable;
	bool rxint_enable;
#ifdef CONFIG_SERIAL_IFLOWCONTROL
	uint8_t iflow:1;			/* input flow control (RTS) enabled */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
	uint8_t oflow:1;			/* output flow control (CTS) enabled */
#endif
	uint8_t tx_level;
};
static struct rtl8730e_up_dev_s g_uart4priv = {
	.irq = AMEBASMART_IRQ_USB_OTG,

};

static uart_dev_t g_usbport = {
	.isconsole = false,
	.recv = {
		.size = CONFIG_CDC_ACM_BULK_OUT_XFER_SIZE,
		.buffer = g_usbrxbuffer,
	},
	.xmit = {
		.size = CONFIG_CDC_ACM_BULK_IN_XFER_SIZE,
		.buffer = g_usbtxbuffer,
	},
	.ops = &log_g_usb_ops,
	.priv = &g_uart4priv,
};

static int rtl8730e_usb_setup(struct uart_dev_s *dev)
{

	return OK;
}

static void rtl8730e_log_usb_shutdown(struct uart_dev_s *dev)
{
	/*Do not shut down usb console*/
}


/****************************************************************************
 * Name: usb_attach
 *
 * Related IRQ is done in bsp layer, ignore here
 *
 ****************************************************************************/

static int rtl8730e_log_usb_attach(struct uart_dev_s *dev)
{
	return 0;
}

/****************************************************************************
 * Name: usb_detach
 *
 * Description:
 *   Detach USB interrupts.  This method is called when the serial port is
 *   closed normally just before the shutdown method is called.  The exception is
 *   the serial console which is never shutdown.
 *
 ****************************************************************************/

static void rtl8730e_log_usb_detach(struct uart_dev_s *dev)
{

}

/****************************************************************************
 * Name: usb_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int rtl8730e_log_usb_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
	int ret = OK;
	return ret;
}

/****************************************************************************
 * Name: usb_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the UART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

static int rtl8730e_log_usb_receive(struct uart_dev_s *dev, unsigned int *status)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	uint8_t rxd;
	
	DEBUGASSERT(priv);
	while(!received_flag){

	}
	rxd = receive_data;
	*status = rxd;
	received_flag = 0;
	return rxd & 0xff;
}


/****************************************************************************
 * Name: usb_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/
static void rtl8730e_log_usb_rxint(struct uart_dev_s *dev, bool enable)
{

}

/****************************************************************************
 * Name: usb_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 ****************************************************************************/

static bool rtl8730e_log_usb_rxavailable(struct uart_dev_s *dev)
{
	if(received_flag) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/****************************************************************************
 * Name: usb_send
 *
 * Description:
 *   This method will send one byte on the USB
 *
 ****************************************************************************/
static void rtl8730e_log_usb_send(struct uart_dev_s *dev, int ch)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	int fd;
	
	usbd_cdc_acm_transmit(&ch, 1);
	while (sem_wait(&g_usbdev.txsem) != OK) {
		/* The only case that an error should occur here is if the wait was awakened
		 * by a signal.
		 */
		ASSERT(errno == EINTR);
	}
	priv->tx_level--;

}


static void usb_xmitchars(FAR uart_dev_t *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	uint16_t nbytes = 0;
	uint8_t *buffer = NULL;
	buffer = (uint8_t *)rtos_mem_zmalloc(CONFIG_CDC_ACM_BULK_IN_XFER_SIZE);
	if (buffer == NULL) {
		udbg("malloc failed\n");
	}
#ifdef CONFIG_SMP
	irqstate_t flags = enter_critical_section();
#endif

	/* Send while we still have data in the TX buffer & room in the fifo */

	while (dev->xmit.head != dev->xmit.tail && uart_txready(dev)) {
		/* Send the next byte */
		buffer[nbytes] = dev->xmit.buffer[dev->xmit.tail];
		// uart_send(dev, dev->xmit.buffer[dev->xmit.tail]);
		nbytes++;

		/* Increment the tail index */

		if (++(dev->xmit.tail) >= dev->xmit.size) {
			dev->xmit.tail = 0;
		}
		if (nbytes == CONFIG_CDC_ACM_BULK_IN_XFER_SIZE) {
			nbytes = 0;
			/*we should only call usb TX when device is attached and ready, else abandon the data*/
			if (attach_status == USBD_ATTACH_STATUS_ATTACHED && cdc_acm_ready_flag) { 
				usbd_cdc_acm_transmit(buffer, CONFIG_CDC_ACM_BULK_IN_XFER_SIZE);
				while (sem_wait(&g_usbdev.txsem) != OK) {
					/* The only case that an error should occur here is if the wait was awakened
					* by a signal.
					*/
					ASSERT(errno == EINTR);
				}
			}
			priv->tx_level-=CONFIG_CDC_ACM_BULK_IN_XFER_SIZE;
		}
	}
	/*we should only call usb TX when device is attached and ready, else abandon the data*/
	if (attach_status == USBD_ATTACH_STATUS_ATTACHED && cdc_acm_ready_flag) { 
		usbd_cdc_acm_transmit(buffer, nbytes);
		while (sem_wait(&g_usbdev.txsem) != OK) {
			/* The only case that an error should occur here is if the wait was awakened
			* by a signal.
			*/
			ASSERT(errno == EINTR);
		}
	}
	priv->tx_level-=nbytes;
	rtos_mem_free(buffer);
	/* When all of the characters have been sent from the buffer disable the TX
	 * interrupt.
	 *
	 * Potential bug?  If nbytes == 0 && (dev->xmit.head == dev->xmit.tail) &&
	 * dev->xmitwaiting == true, then disabling the TX interrupt will leave
	 * the uart_write() logic waiting to TX to complete with no TX interrupts.
	 * Can that happen?
	 */

	if (dev->xmit.head == dev->xmit.tail) {
		uart_disabletxint(dev);
	}

	/* If any bytes were removed from the buffer, inform any waiters there there is
	 * space available.
	 */

	if (nbytes) {
		dev->sent(dev);
	}
#ifdef CONFIG_SMP
	leave_critical_section(flags);
#endif
}


/****************************************************************************
 * Name: usb_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

static void rtl8730e_log_usb_txint(struct uart_dev_s *dev, bool enable)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);
	priv->txint_enable = enable;
	if (enable) {
		// uart_xmitchars(dev);
		usb_xmitchars(dev);
	}
}

/****************************************************************************
 * Name: usb_txready
 *
 * Description:
 *   Return true if the tranmsit fifo is not full
 *
 ****************************************************************************/

static bool rtl8730e_log_usb_txready(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	return 1;

}

/****************************************************************************
 * Name: usb_txempty
 *
 * Description:
 *   Return true if the transmit fifo is empty
 *
 ****************************************************************************/

static bool rtl8730e_log_usb_txempty(struct uart_dev_s *dev)
{
	struct rtl8730e_up_dev_s *priv = (struct rtl8730e_up_dev_s *)dev->priv;
	DEBUGASSERT(priv);

	return 1;
}

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register operations ******************************************************/

#if defined(CONFIG_DEBUG_USBDEVICE)
static void amebasmart_usb_hal_dump_registers(void);
#endif

/* Low-Level Helpers ********************************************************/

/* Suspend/Resume Helpers ***************************************************/

/* Request Helpers **********************************************************/

/* Interrupt level processing ***********************************************/

/* Endpoint helpers *********************************************************/

/* Endpoint operations ******************************************************/

/* USB device controller operations *****************************************/

/* Initialization/Reset *****************************************************/

/*usbd related APIs*/
static int amebasmart_usbd_init(struct amebasmart_usbdev_s *priv, usbd_config_t *cfg);
static int amebasmart_usbd_cdc_acm_init(struct amebasmart_usbdev_s *priv, usbd_cdc_acm_cb_t *cb);
static amebasmart_usbd_cdc_acm_deinit(struct amebasmart_usbdev_s *priv);

/*CDC ACM CALLBACK related*/
static int amebasmart_cdc_acm_cb_init(void);
static int amebasmart_cdc_acm_cb_deinit(void);
static int amebasmart_cdc_acm_cb_setup(usb_setup_req_t *req, u8 *buf);
static int amebasmart_cdc_acm_cb_received(u8 *buf, u16 len);
static void amebasmart_cdc_acm_cb_status_changed(u8 status);
static void amebasmart_cdc_acm_cb_transmitted(u8 status);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static usbd_cdc_acm_cb_t amebasmart_cdc_acm_cb = {
	.init 		= amebasmart_cdc_acm_cb_init,
	.deinit 	= amebasmart_cdc_acm_cb_deinit,
	.setup 		= amebasmart_cdc_acm_cb_setup,
	.received 	= amebasmart_cdc_acm_cb_received,
	.transmitted = amebasmart_cdc_acm_cb_transmitted,
	.status_changed = amebasmart_cdc_acm_cb_status_changed
};


/*CDC ACM related*/
static usbd_config_t amebasmart_cdc_acm_cfg = {
	.speed = USB_SPEED_FULL,
	.dma_enable   = 1U,
	.isr_priority = 25U,
	.intr_use_ptx_fifo  = 0U,
	.nptx_max_epmis_cnt = 10U,
	.ext_intr_en        = USBD_EPMIS_INTR,
	.nptx_max_err_cnt   = {0U, 0U, 0U, 2000U, },
};

static usbd_cdc_acm_line_coding_t amebasmart_cdc_acm_line_coding;

static uint16_t cdc_acm_ctrl_line_state;

/**
  * @brief  Initializes the CDC media layer
  * @param  None
  * @retval Status
  */
static int amebasmart_cdc_acm_cb_init(void)
{
	// struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	// DEBUGASSERT(dev);

	usbd_cdc_acm_line_coding_t *lc = &amebasmart_cdc_acm_line_coding;

	lc->bitrate = 1500000;
	lc->format = 0x00;
	lc->parity_type = 0x00;
	lc->data_type = 0x08;

#if CONFIG_AMEBASMART_USBD_CDC_ACM_ASYNC_XFER
	cdc_acm_async_xfer_buf_pos = 0;
	cdc_acm_async_xfer_busy = 0;
#endif
	return 0;
}

/**
  * @brief  DeInitializes the CDC media layer
  * @param  None
  * @retval Status
  */
static int amebasmart_cdc_acm_cb_deinit(void)
{

	// struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	// DEBUGASSERT(dev);

#if CONFIG_AMEBASMART_USBD_CDC_ACM_ASYNC_XFER
	cdc_acm_async_xfer_buf_pos = 0;
	cdc_acm_async_xfer_busy = 0;
#endif
	return HAL_OK;
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface through this function.
  * @param  Buf: RX buffer
  * @param  Len: RX data length (in bytes)
  * @retval Status
  */
static int amebasmart_cdc_acm_cb_received(u8 *buf, u16 len)
{

#if CONFIG_AMEBASMART_USBD_CDC_ACM_ASYNC_XFER
	u8 ret = HAL_OK;
	if (0 == cdc_acm_async_xfer_busy) {
		if ((cdc_acm_async_xfer_buf_pos + len) > CONFIG_CDC_ACM_ASYNC_BUF_SIZE) {
			len = CONFIG_CDC_ACM_ASYNC_BUF_SIZE - cdc_acm_async_xfer_buf_pos;  // extra data discarded
		}

		memcpy((void *)((u32)cdc_acm_async_xfer_buf + cdc_acm_async_xfer_buf_pos), buf, len);
		cdc_acm_async_xfer_buf_pos += len;
		if (cdc_acm_async_xfer_buf_pos >= CONFIG_CDC_ACM_ASYNC_BUF_SIZE) {
			cdc_acm_async_xfer_buf_pos = 0;
			rtos_sema_give(cdc_acm_async_xfer_sema);
		}
	} else {
		printf("\n[CDC] Busy, discarded %d bytes\n", len);
		ret = HAL_BUSY;
	}

	return ret;
#else
	receive_data = (uint8_t)*buf;
	received_flag = 1;
	uart_recvchars(&USB_DEV);
	return receive_data;
#endif
}

static void amebasmart_cdc_acm_cb_transmitted(u8 status)
{
	(void)sem_post(&g_usbdev.txsem);
}
/**
  * @brief  Handle the CDC class control requests
  * @param  cmd: Command code
  * @param  buf: Buffer containing command data (request parameters)
  * @param  len: Number of data to be sent (in bytes)
  * @retval Status
  */
static int amebasmart_cdc_acm_cb_setup(usb_setup_req_t *req, u8 *buf)
{
	// struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	// DEBUGASSERT(dev);

	usbd_cdc_acm_line_coding_t *lc = &amebasmart_cdc_acm_line_coding;

	switch (req->bRequest) {
	case CDC_SEND_ENCAPSULATED_COMMAND:
		/* Do nothing */
		break;

	case CDC_GET_ENCAPSULATED_RESPONSE:
		/* Do nothing */
		break;

	case CDC_SET_COMM_FEATURE:
		/* Do nothing */
		break;

	case CDC_GET_COMM_FEATURE:
		/* Do nothing */
		break;

	case CDC_CLEAR_COMM_FEATURE:
		/* Do nothing */
		break;

	case CDC_SET_LINE_CODING:
		if (req->wLength == CDC_ACM_LINE_CODING_SIZE) {
			lc->bitrate = (u32)(buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
			lc->format = buf[4];
			lc->parity_type = buf[5];
			lc->data_type = buf[6];
		}

		break;

	case CDC_GET_LINE_CODING:
		buf[0] = (u8)(lc->bitrate & 0xFF);
		buf[1] = (u8)((lc->bitrate >> 8) & 0xFF);
		buf[2] = (u8)((lc->bitrate >> 16) & 0xFF);
		buf[3] = (u8)((lc->bitrate >> 24) & 0xFF);
		buf[4] = lc->format;
		buf[5] = lc->parity_type;
		buf[6] = lc->data_type;
		break;

	case CDC_SET_CONTROL_LINE_STATE:
		/*
		wValue:	wValue, Control Signal Bitmap
				D2-15:	Reserved, 0
				D1:	RTS, 0 - Deactivate, 1 - Activate
				D0:	DTR, 0 - Not Present, 1 - Present
		*/
		cdc_acm_ctrl_line_state = req->wValue;
		if (cdc_acm_ctrl_line_state & 0x01) {
#if CONFIG_AMEBASMART_CDC_ACM_NOTIFY
			usbd_cdc_acm_notify_serial_state(CDC_ACM_CTRL_DSR | CDC_ACM_CTRL_DCD);
#endif
		}
		break;

	case CDC_SEND_BREAK:
		/* Do nothing */
		break;

	default:
		break;
	}

	return HAL_OK;
}

static void amebasmart_cdc_acm_cb_status_changed(uint8_t status)
{
	// struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	// DEBUGASSERT(dev);
	/*when detached, we need to clear the rdy flag*/
	attach_status = status;
	if (status == USBD_ATTACH_STATUS_DETACHED) {
		cdc_acm_ready_flag = -1;
	}
#if CONFIG_AMEBASMART_USBD_CDC_ACM_HOTPLUG
	cdc_acm_attach_status = status;
	rtos_sema_give(cdc_acm_attach_status_changed_sema);
#endif
}


static int amebasmart_usbd_cdc_acm_init(struct amebasmart_usbdev_s *priv, usbd_cdc_acm_cb_t *cb)
{
	struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	int ret = -1;

	ret = usbd_cdc_acm_init(CONFIG_CDC_ACM_BULK_OUT_XFER_SIZE, CONFIG_CDC_ACM_BULK_IN_XFER_SIZE, &amebasmart_cdc_acm_cb);
	if (ret != 0) {
		udbg("USB ACM init failed\n");
		amebasmart_usbd_cdc_acm_deinit(dev);
	}
	return ret;
}

static amebasmart_usbd_cdc_acm_deinit(struct amebasmart_usbdev_s *priv)
{
	struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	DEBUGASSERT(dev);

	usbd_cdc_acm_deinit();
}

static int amebasmart_usbd_init(struct amebasmart_usbdev_s *priv, usbd_config_t *cfg)
{
	struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;

	int ret = -1;
	ret = usbd_init(cfg);

	return ret;
}

/****************************************************************************
 * Name: amebasmart_up_usbuninitialize
 * Description: Initialize the USB driver
 * Input Parameters: None
 * Returned Value: None
 ****************************************************************************/
int amebasmart_up_usbinitialize(struct amebasmart_usbdev_s *priv)
{
	struct amebasmart_usbdev_s *dev = (struct amebasmart_usbdev_s *)priv;
	DEBUGASSERT(dev);
	
	int ret = -1;

	ret = amebasmart_usbd_init(dev, &amebasmart_cdc_acm_cfg);
	if (ret != 0) {
		udbg("usbd init fail\n");
		return ret;
	}

	ret = amebasmart_usbd_cdc_acm_init(dev, &amebasmart_cdc_acm_cb);
	if (ret != 0) {
		udbg("usbd acm init fail\n");
		return ret;
	}
	sem_init(&g_usbdev.txsem, 0, 0);

	return ret;
}

void register_usb(void)
{
	USB_DEV.isconsole = true;
	uart_register("/dev/ttyACM0", &USB_DEV);
	/*unregister /dev/console because serial was sharing the same fd during serial initialization*/
	unregister_driver("/dev/console");
	uart_register("/dev/console", &USB_DEV);
}
void usb_initialize(void)
{
	struct amebasmart_usbdev_s *priv = NULL;
	priv = &g_usbdev;

	int ret = -1;

	ret = amebasmart_up_usbinitialize(priv);
	if (ret != 0) {
		udbg("amebasmart usb init fail\n");
	}
}
