/**
  ******************************************************************************
  * @file    usbd_pcd.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the functionalities of USB PCD layer
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "usbd_hal.h"
#include "usbd_core.h"
#include "usbd_pcd.h"

/* Private defines -----------------------------------------------------------*/

/* Enable in token prediction debug */
#define IN_TOKEN_PREDICT_DEBUG_EN              0U

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 usbd_pcd_interrupt_init(usbd_pcd_t *pcd);
static u8 usbd_pcd_interrupt_deinit(usbd_pcd_t *pcd);
static void usbd_pcd_isr_task(void *data);
static void usbd_pcd_irq_handler(void);
static void usbd_pcd_handle_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_reset_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_enum_done_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_suspend_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_ep_out_setup_packet_interrupt(usbd_pcd_t *pcd, u8 ep_num);
static void usbd_pcd_handle_ep_out_transfer_complete_interrupt(usbd_pcd_t *pcd, u8 ep_num);
static void usbd_pcd_handle_out_ep_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_in_ep_disabled_interrupt(usbd_pcd_t *pcd, u8 ep_num);
static void usbd_pcd_handle_in_ep_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_rx_fifo_non_empty_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_get_in_ep_sequence_from_in_token_queue(usbd_pcd_t *pcd);
static u8 usbd_pcd_handle_ep_np_tx_fifo_empty_interrupt(usbd_pcd_t *pcd, u8 ep_num);
static void usbd_pcd_handle_np_tx_fifo_empty_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_ep_mismatch_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_in_nak_effective(usbd_pcd_t *pcd);
static void usbd_pcd_handle_wakeup_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_sof_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_eopf_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_srq_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_otg_interrupt(usbd_pcd_t *pcd);
static void usbd_pcd_handle_interrupt(usbd_pcd_t *pcd);

/* Private variables ---------------------------------------------------------*/

/* PCD handler */
USB_BSS_SECTION
static usbd_pcd_t usbd_pcd;

USB_DATA_SECTION
static const char *const TAG = "USB";

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the PCD interrupt.
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
static u8 usbd_pcd_interrupt_init(usbd_pcd_t *pcd)
{
	int ret;

	if (pcd->isr_initialized) {
		usbd_pcd_interrupt_deinit(pcd);
	}

	rtw_init_sema(&pcd->isr_sema, 0);

	ret = rtw_create_task(&pcd->isr_task,
						  "usbd_isr_task",
						  USBD_ISR_THREAD_STACK_SIZE,
						  pcd->config.isr_priority,
						  usbd_pcd_isr_task,
						  (void *)pcd);
	if (ret != 1) {
		return HAL_ERR_UNKNOWN;
	}

	usb_hal_register_irq_handler((usb_irq_fun_t)usbd_pcd_irq_handler);
	usb_hal_enable_interrupt();

	pcd->isr_initialized = 1;

	return HAL_OK;
}

/**
  * @brief  Deinitialize the PCD interrupt.
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
static u8 usbd_pcd_interrupt_deinit(usbd_pcd_t *pcd)
{
	if (pcd->isr_initialized) {
		usb_hal_disable_interrupt();
		usb_hal_unregister_irq_handler();
		if (pcd->isr_task.task > 0) {
			rtw_delete_task(&pcd->isr_task);
		}
		rtw_free_sema(&pcd->isr_sema);
		pcd->isr_initialized = 0;
	}
	return HAL_OK;
}

/**
  * @brief  PCD ISR task
  * @param  data: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_isr_task(void *data)
{
	usbd_pcd_t *pcd = (usbd_pcd_t *)data;
	u32 gintsts;

	for (;;) {
		rtw_down_timeout_sema(&pcd->isr_sema, 0xFFFFFFFF);

		if (pcd->isr_task.blocked) {
			RTK_LOGW(TAG, "USBD ISR task killed\n");
			break;
		}

		gintsts = usb_hal_read_interrupts();

		usbd_pcd_handle_interrupt(pcd);
		usb_hal_enable_interrupt();

		if ((gintsts != 0U) && (usb_hal_get_otg_mode() == USB_OTG_MODE_DEVICE)) {
			if ((gintsts & USB_OTG_GINTSTS_USBSUSP) != 0) {
				if (pcd->dev->dev_attach_status == USBD_ATTACH_STATUS_ATTACHED) {
					pcd->dev->dev_attach_status = USBD_ATTACH_STATUS_DETACHED;
					if (pcd->dev->driver->status_changed) {
						pcd->dev->driver->status_changed(pcd->dev, pcd->dev->dev_attach_status);
					}
				}
			} else {
				if ((pcd->dev->dev_attach_status != USBD_ATTACH_STATUS_ATTACHED) && ((gintsts & USB_OTG_GINTSTS_ESUSP) == 0)) {
					pcd->dev->dev_attach_status = USBD_ATTACH_STATUS_ATTACHED;
					if (pcd->dev->driver->status_changed) {
						pcd->dev->driver->status_changed(pcd->dev, pcd->dev->dev_attach_status);
					}
				}
			}
		}
	}

	rtw_thread_exit();
}

/**
  * @brief  USB device IRQ handler
  * @param  None
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_irq_handler(void)
{
	usbd_pcd_t *pcd = &usbd_pcd;
	usb_hal_disable_interrupt();
	rtw_up_sema(&pcd->isr_sema);
}

/**
  * @brief  Handle PCD reset interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_reset_interrupt(usbd_pcd_t *pcd)
{
	u32 i;

	USB_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
	/* Set Default Address to 0 */
	USB_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD;

	usb_hal_flush_tx_fifo(0x10U);

	for (i = 0U; i < USB_MAX_ENDPOINTS; i++) {
		USB_INEP(i)->DIEPINT = 0xFFFFU; // 0xFB7FU;
		USB_INEP(i)->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
		USB_OUTEP(i)->DOEPINT = 0xFFFFU; // 0xFB7FU;
		USB_OUTEP(i)->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
	}

	usbd_hal_reset_in_token_queue(pcd);

	USB_DEVICE->DAINTMSK |= 0x10001U;

	USB_DEVICE->DOEPMSK |= USB_OTG_DOEPMSK_STUPM |
						   USB_OTG_DOEPMSK_XFRCM |
						   USB_OTG_DOEPMSK_EPDM |
						   USB_OTG_DOEPMSK_OTEPSPRM;

	USB_DEVICE->DIEPMSK |= USB_OTG_DIEPMSK_TOM |
						   USB_OTG_DIEPMSK_XFRCM |
						   USB_OTG_DIEPMSK_EPDM |
						   USB_OTG_DIEPMSK_INEPNMM;

	/* setup EP0 to receive SETUP packets */
	usbd_hal_ep0_out_start(pcd);

}

/**
  * @brief  Handle PCD enum done interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_enum_done_interrupt(usbd_pcd_t *pcd)
{
	usbd_hal_ep0_setup_activate(pcd);

	pcd->config.speed = usbd_hal_get_device_speed(pcd);

	/* Set USB Turnaround time */
	usbd_hal_set_turnaround_time(pcd);

	usb_os_spinunlock(pcd->lock);
	usbd_core_set_speed(pcd->dev, (usb_speed_type_t)pcd->config.speed);

	/* Reset Device */
	usbd_core_reset(pcd->dev);
	usb_os_spinlock(pcd->lock);
}

/**
  * @brief  Handle PCD suspend interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_suspend_interrupt(usbd_pcd_t *pcd)
{
	if ((USB_DEVICE->DSTS & USB_OTG_DSTS_SUSPSTS) == USB_OTG_DSTS_SUSPSTS) {
		usb_os_spinunlock(pcd->lock);
		usbd_core_suspend(pcd->dev);
		usb_os_spinlock(pcd->lock);
	}
}

/**
  * @brief  process EP OUT setup packet received interrupt.
  * @param  pcd: PCD handle
  * @param  ep_num: Endpoint number
  * @retval HAL status
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_ep_out_setup_packet_interrupt(usbd_pcd_t *pcd, u8 ep_num)
{
	u32 gSNPSiD = USB_GLOBAL->GSNPSID;
	u32 DoepintReg = USB_OUTEP(ep_num)->DOEPINT;

	if (pcd->config.dma_enable) {
		/* StupPktRcvd = 1 pending setup packet int */
		if ((gSNPSiD > USB_OTG_CORE_ID_300A) &&
			((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX)) {
			USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_STPKTRX);
		}
		if (pcd->setup != NULL) {
			DCache_Invalidate((u32)pcd->setup, USBD_SETUP_PACKET_BUF_LEN);
		}
	} else {
		if ((gSNPSiD == USB_OTG_CORE_ID_310A) &&
			((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX)) {
			USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_STPKTRX);
		}
	}

	/* Inform the upper layer that a setup packet is available */
	usb_os_spinunlock(pcd->lock);
	usbd_core_setup_stage(pcd->dev, (u8 *)pcd->setup);
	usb_os_spinlock(pcd->lock);
}

/**
  * @brief  process EP OUT transfer complete interrupt.
  * @param  pcd: PCD handle
  * @param  ep_num: Endpoint number
  * @retval HAL status
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_ep_out_transfer_complete_interrupt(usbd_pcd_t *pcd, u8 ep_num)
{
	u32 pktcnt;
	u32 xfer_size;
	u32 gSNPSiD = USB_GLOBAL->GSNPSID;
	u32 DoepintReg = USB_OUTEP(ep_num)->DOEPINT;
	usbd_pcd_ep_t *ep = &pcd->out_ep[ep_num];

	if (pcd->config.dma_enable) {
		if ((DoepintReg & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR) { /* Class E */
			USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_OTEPSPR);
		} else if ((DoepintReg & (USB_OTG_DOEPINT_STUP | USB_OTG_DOEPINT_OTEPSPR)) == 0U) {
			/* StupPktRcvd = 1 this is a setup packet */
			if ((gSNPSiD > USB_OTG_CORE_ID_300A) &&
				((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX)) {
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_STPKTRX);
			} else {
				/* out data packet received */
				if (ep_num == 0) {
					xfer_size = USB_OUTEP(ep_num)->DOEPTSIZ & USB_OTG_DOEPTSIZ0_XFRSIZ;
					ep->xfer_count = ep->max_packet_len - xfer_size;
				} else {
					xfer_size = USB_OUTEP(ep_num)->DOEPTSIZ & USB_OTG_DOEPTSIZ_XFRSIZ;
					pktcnt = (ep->xfer_len + ep->max_packet_len - 1U) / ep->max_packet_len;
					ep->xfer_count = ep->max_packet_len * pktcnt - xfer_size;
				}

				if ((ep->xfer_count != 0U) && (ep->xfer_buff != NULL)) {
					DCache_Invalidate((u32)ep->xfer_buff, ep->xfer_count);
				}

				if (ep->xfer_buff) {
					ep->xfer_buff += ep->xfer_count;
				}

				usb_os_spinunlock(pcd->lock);
				usbd_core_data_out_stage(pcd->dev, ep_num, ep->xfer_buff);
				usb_os_spinlock(pcd->lock);

				if ((ep_num == 0U) && (ep->xfer_len == 0U)) {
					/* this is ZLP, so prepare EP0 for next setup */
					usbd_hal_ep0_out_start(pcd);
				}
			}
		} else {
			/* ... */
		}
	} else {
		if (gSNPSiD == USB_OTG_CORE_ID_310A) {
			/* StupPktRcvd = 1 this is a setup packet */
			if ((DoepintReg & USB_OTG_DOEPINT_STPKTRX) == USB_OTG_DOEPINT_STPKTRX) {
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_STPKTRX);
			} else {
				if ((DoepintReg & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR) {
					USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_OTEPSPR);
				}
				usb_os_spinunlock(pcd->lock);
				usbd_core_data_out_stage(pcd->dev, ep_num, ep->xfer_buff);
				usb_os_spinlock(pcd->lock);
			}
		} else {
			usb_os_spinunlock(pcd->lock);
			usbd_core_data_out_stage(pcd->dev, ep_num, ep->xfer_buff);
			usb_os_spinlock(pcd->lock);
		}
	}
}

/**
  * @brief  Handle PCD OUT EP interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_out_ep_interrupt(usbd_pcd_t *pcd)
{
	u32 ep_intr;
	u32 epint;
	u8 ep_num = 0U;
	u32 gSNPSiD = USB_GLOBAL->GSNPSID;

	/* Read in the device interrupt bits */
	ep_intr = usbd_hal_read_all_out_ep_interrupts(pcd);
	while ((ep_intr != 0U) && (ep_num < USB_MAX_ENDPOINTS)) {
		if ((ep_intr & 0x1U) != 0U) {
			epint = usbd_hal_read_out_ep_interrupts(pcd, ep_num);

			RTK_LOGD(TAG, "OUT EP%d 0x%08X\n", ep_num, epint);

			if ((epint & USB_OTG_DOEPINT_XFRC) == USB_OTG_DOEPINT_XFRC) {
				RTK_LOGD(TAG, "OUT EP%d XFRC\n", ep_num);
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_XFRC);
				usbd_pcd_handle_ep_out_transfer_complete_interrupt(pcd, ep_num);
			}

			if ((epint & USB_OTG_DOEPINT_STUP) == USB_OTG_DOEPINT_STUP) {
				RTK_LOGD(TAG, "OUT EP%d STUP\n", ep_num);
				/* Class B setup phase done for previous decoded setup */
				usbd_pcd_handle_ep_out_setup_packet_interrupt(pcd, ep_num);
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_STUP);
			}

			if ((epint & USB_OTG_DOEPINT_OTEPDIS) == USB_OTG_DOEPINT_OTEPDIS) {
				RTK_LOGD(TAG, "OUT EP%d DIS\n", ep_num);
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_OTEPDIS);
			}

			/* Clear Status Phase Received interrupt */
			if ((epint & USB_OTG_DOEPINT_OTEPSPR) == USB_OTG_DOEPINT_OTEPSPR) {
				RTK_LOGD(TAG, "OUT EP%d SPR\n", ep_num);
				if ((pcd->config.dma_enable) && (gSNPSiD == USB_OTG_CORE_ID_310A)) {
					usbd_hal_ep0_out_start(pcd);
				}
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_OTEPSPR);
			}

			/* Clear OUT NAK interrupt */
			if ((epint & USB_OTG_DOEPINT_NAK) == USB_OTG_DOEPINT_NAK) {
				RTK_LOGD(TAG, "OUT EP%d NAK\n", ep_num);
				USB_PCD_CLEAR_OUT_EP_INTR(ep_num, USB_OTG_DOEPINT_NAK);
			}
		}
		ep_num++;
		ep_intr >>= 1U;
	}
}

/**
  * @brief  Handle PCD IN EP disable interrupt.
  * @param  pcd: PCD handle
  * @param  ep_num: Endpoint number
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_in_ep_disabled_interrupt(usbd_pcd_t *pcd, u8 ep_num)
{
	u32 i, j;
	u32 reg;
	u32 xfer_pktcnt;
	u32 pktcnt_mask;
	u32 pktcnt_pos;
	u32 remain_to_transfer = 0;
	u32 dma = pcd->config.dma_enable;
	u8  in_ep_reactive[USB_MAX_ENDPOINTS];
	usbd_pcd_ep_t *pep = &pcd->in_ep[ep_num];

	if ((pcd->start_predict == 0) || (pep->is_ptx)) {   // period
		// not for periodic transfer
		return;
	}

	if (pcd->start_predict > 2)  {  // NP IN EP
		pcd->start_predict--;
		return;
	}

	pcd->start_predict--;

	if (pcd->start_predict == 1)  { // All NP IN Ep's disabled now
		//predict nextep:
		//slave mode, get the in_ep_sequence
		//dma mode, get the nextep_seq
		usbd_pcd_get_in_ep_sequence_from_in_token_queue(pcd);

		if (dma != 0) {
			/* Update all active IN EP's NextEP field based of nextep_seq[] */
			for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
				if (pcd->nextep_seq[i] != 0xff) {   //Active NP IN EP
					reg = USB_INEP(i)->DIEPCTL;
					reg &= ~USB_OTG_DIEPCTL_NEXTEP_Msk;
					reg |= pcd->nextep_seq[i] << USB_OTG_DIEPCTL_NEXTEP_Pos;
					USB_INEP(i)->DIEPCTL = reg;
				}
			}
		}

		/* Flush Shared NP TxFIFO */
		usb_hal_flush_tx_fifo(0x00);

		/*
			Rewind buffers
			check all IN EP list, but not for periodic and unenable EP
		*/
		i = (dma ? pcd->first_in_nextep_seq : 0);
		do {
			if (i >= USB_MAX_ENDPOINTS) {
				break;
			}

			pep = &pcd->in_ep[i];
			if (!(pep->is_ptx) && pep->is_initialized) {
				if (i == 0) {
					pktcnt_mask = USB_OTG_DIEPTSIZ0_PKTCNT;
					pktcnt_pos = USB_OTG_DIEPTSIZ0_PKTCNT_Pos;
				} else {
					pktcnt_mask = USB_OTG_DIEPTSIZ_PKTCNT;
					pktcnt_pos = USB_OTG_DIEPTSIZ_PKTCNT_Pos;
				}
				xfer_pktcnt = (USB_INEP(i)->DIEPTSIZ & pktcnt_mask) >> pktcnt_pos;
				//RTK_LOGD(TAG, "Ep%d,pktcnt(%d)len(%d)xfercnt(%d)zlp(%d)\n", i, xfer_pktcnt, pep->xfer_len, pep->xfer_count, pep->is_zlp);

				if (xfer_pktcnt != 0) {
					//remain_to_transfer is the data length that did not sent success
					//includes not copied to FIFO and copied to FIFO
					if (pep->is_zlp || (0 == pep->xfer_len)) { //zlp
						remain_to_transfer = 0U;
					} else { //not zlp
						//(1)all the trasnfer data has been wrote to the TX FIFO
						//(2)part of the data has been wrote to the TX FIFO
						//and when flush the TX FIFO, the wrote data has been dropped
						if ((pep->xfer_len % pep->max_packet_len) == 0U) { //last packet = 0
							remain_to_transfer = pep->max_packet_len * xfer_pktcnt;
						} else {
							remain_to_transfer = pep->xfer_len % pep->max_packet_len + pep->max_packet_len * (xfer_pktcnt - 1);
						}
					}

					reg = USB_INEP(i)->DIEPTSIZ;
					reg &= ~USB_OTG_DIEPTSIZ_XFRSIZ;
					reg |= USB_OTG_DIEPTSIZ_XFRSIZ & (remain_to_transfer << USB_OTG_DIEPTSIZ_XFRSIZ_Pos);
					USB_INEP(i)->DIEPTSIZ = reg;

					if (dma) {
						reg = pep->dma_addr + (pep->xfer_len - remain_to_transfer);
						USB_INEP(i)->DIEPDMA = reg;
					} else {
						if (remain_to_transfer) {
							pep->xfer_buff -= pep->xfer_count; //rewind to the begin
							pep->xfer_buff += (pep->xfer_len - remain_to_transfer);
						} else {
							pep->xfer_buff = NULL;
						}
					}
					pep->xfer_count = pep->xfer_len - remain_to_transfer;
				}
			}

			if (dma != 0) {
				i = pcd->nextep_seq[i];
				if (i == pcd->first_in_nextep_seq) {
					break;
				}
			} else {
				if (++i >= USB_MAX_ENDPOINTS) {
					break;
				}
			}
		} while (1);

		/* Restart transfers in predicted sequences */
		if (dma == 0) { //slave mode
			for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
				in_ep_reactive[i] = 0xFF;
			}

			j = 0, i = 0;
			//first save in_ep_sequence[INTOKEN] in in_ep_reactive
			do {
				if (pcd->in_ep_sequence[j] < USB_MAX_ENDPOINTS) {  //valid ep
					pep = &pcd->in_ep[pcd->in_ep_sequence[j]];
					if (!(pep->is_ptx) && pep->is_initialized) {
						in_ep_reactive[i++] = pcd->in_ep_sequence[j];
					}
				}
				if (++j >= USB_MAX_ENDPOINTS) {
					break;
				}
			} while (1);

			j = 0, i = 0;
			//second save other IN EPs in in_ep_reactive
			do {
				pep = &pcd->in_ep[j];
				if (!(pep->is_ptx) && pep->is_initialized) {
					do {
						if ((0xFF == in_ep_reactive[i]) || (j == in_ep_reactive[i])) { //find
							break;
						}
						i++;
					} while (1);
					if (0xFF == in_ep_reactive[i]) {
						in_ep_reactive[i++] = j;
					}
				}
				if ((i >= USB_MAX_ENDPOINTS) || (++j >= USB_MAX_ENDPOINTS)) {
					break;
				}
			} while (1);
		}

		if (dma != 0) {
			i = pcd->first_in_nextep_seq;
		} else {
			j = 0;
			i = in_ep_reactive[j];
		}

		do {
			if (i >= USB_MAX_ENDPOINTS) {
				break;
			}

			pep = &pcd->in_ep[i];
			if (!(pep->is_ptx) && pep->is_initialized) {
				//EP0: PktCnt 20:19,XferSize 6:0
				//EPN: PktCnt 28:19,XferSize 18:0
				if (i == 0) {
					pktcnt_mask = USB_OTG_DIEPTSIZ0_PKTCNT;
				} else {
					pktcnt_mask = USB_OTG_DIEPTSIZ_PKTCNT;
				}
				xfer_pktcnt = USB_INEP(i)->DIEPTSIZ & pktcnt_mask;
				if (xfer_pktcnt != 0) {
					/*Re-en IN EP*/
					if ((dma == 0) && (pep->xfer_len > 0U)) {
						if (pcd->ded_tx_fifo_en == 0U) {
							USB_GLOBAL->GINTMSK |= USB_OTG_GINTMSK_NPTXFEM;
						} else {
							USB_DEVICE->DIEPEMPMSK |= 1UL << (ep_num & EP_ADDR_MSK);
						}
					}

					/* Enable the Tx FIFO Empty Interrupt for this EP */
					USB_INEP(i)->DIEPCTL |= (USB_OTG_DIEPCTL_CNAK | USB_OTG_DIEPCTL_EPENA);
				}
			}

			if (dma != 0) {
				i = pcd->nextep_seq[i];
				if (i == pcd->first_in_nextep_seq) {
					break;
				}
			} else {
				if (++j >= USB_MAX_ENDPOINTS) {
					break;
				}
				i = in_ep_reactive[j];
			}
		} while (1);

		/* Clear the global non-periodic IN NAK handshake */
		USB_DEVICE->DCTL |= USB_OTG_DCTL_CGINAK;
		/* Unmask EP Mismatch interrupt */
		USB_GLOBAL->GINTMSK |= USB_OTG_GINTMSK_EPMISM;

		pcd->start_predict = 0;
	}
}

/**
  * @brief  Write TxFIFO when EP TxFIFO emptry interrupt occurs
  * @param  pcd: PCD handle
  * @param  ep_num: Endpoint number
  * @retval HAL status
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_in_ep_tx_fifo_empty_interrupt(usbd_pcd_t *pcd, u8 ep_num)
{
	usbd_pcd_ep_t *ep;
	u32 len;
	u32 len32b;
	u32 msk;

	ep = &pcd->in_ep[ep_num];

	if (ep->xfer_count > ep->xfer_len) {
		RTK_LOGE(TAG, "IN EP%d xfer error\n", ep_num);
		return;
	}

	len = ep->xfer_len - ep->xfer_count;

	if (len > ep->max_packet_len) {
		len = ep->max_packet_len;
	}

	len32b = (len + 3U) / 4U;

	while (((USB_INEP(ep_num)->DTXFSTS & USB_OTG_DTXFSTS_INEPTFSAV) >= len32b) &&
		   (ep->xfer_count < ep->xfer_len) && (ep->xfer_len != 0U)) {
		/* Write the FIFO */
		len = ep->xfer_len - ep->xfer_count;

		if (len > ep->max_packet_len) {
			len = ep->max_packet_len;
		}

		len32b = (len + 3U) / 4U;

		if (pcd->config.dma_enable == 0U) {
			usb_hal_write_packet(ep->xfer_buff, ep_num, (u16)len);
		}

		ep->xfer_buff  += len;
		ep->xfer_count += len;
	}

	if (ep->xfer_len <= ep->xfer_count) {
		msk = 0x1UL << ep_num;
		USB_DEVICE->DIEPEMPMSK &= ~msk;
	}
}

/**
  * @brief  Handle PCD IN EP mismatch interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_in_ep_mismatch_interrupt(usbd_pcd_t *pcd, u32 ep_num)
{
	UNUSED(pcd);
	UNUSED(ep_num);
	//unmask InToken received with EP Mismatch
	USB_DEVICE->DIEPMSK &= ~USB_OTG_DIEPMSK_INEPNMM;
}

/**
  * @brief  Handle PCD IN EP NAK effective interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_in_ep_nak_effective_interrupt(usbd_pcd_t *pcd, u32 ep_num)
{
	UNUSED(pcd);
	UNUSED(ep_num);
	USB_DEVICE->DIEPMSK &= ~USB_OTG_DIEPMSK_INEPNEM;
}

/**
  * @brief  Handle PCD IN ISOC incompISOIN interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_incomplete_in_isoc(usbd_pcd_t *pcd)
{
	u32 i;
	u32 reg;
	u32 regvalue;
	usbd_pcd_ep_t *ep;
	u8 ep_num;

	for (i = 1U; i < USB_MAX_ENDPOINTS; i++) {
		ep = &(pcd->in_ep[i]);  //in
		ep_num = USB_EP_NUM(ep->addr);
		reg = USB_INEP(ep_num)->DIEPCTL;
		if ((ep->type == USB_CH_EP_TYPE_ISOC) && USB_EP_IS_IN(ep->addr) && ((reg & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA)) {
			if ((reg & USB_OTG_DIEPCTL_EONUM_DPID) == 0) {
				reg |= USB_OTG_DIEPCTL_SODDFRM;
			} else {
				reg |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
			}
			/*
				Fix Isoc In Issue:
				if set bit31=1 again, HW will force to send a ZLP in next in token, and packet lost
				while set 0 not change bit31 value, it is still 1
			*/
			reg &= ~(USB_OTG_DIEPCTL_EPENA); //bit31
			USB_INEP(ep_num)->DIEPCTL = reg;
		}
	}
}


/**
  * @brief  Handle PCD IN EP interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_in_ep_interrupt(usbd_pcd_t *pcd)
{
	u32 ep_intr;
	u32 epint;
	u32 msk;
	u8 ep_num = 0U;
	usbd_pcd_ep_t *pep;

	/* Read in the device interrupt bits */
	ep_intr = usbd_hal_read_all_in_ep_interrupts(pcd);
	while ((ep_intr != 0U) && (ep_num < USB_MAX_ENDPOINTS)) {
		if ((ep_intr & 0x1U) != 0U) { /* In ITR */
			pep = &pcd->in_ep[ep_num];
			epint = usbd_hal_read_in_ep_interrupts(pcd, ep_num);
			RTK_LOGD(TAG, "IN EP%d 0x%08X\n", ep_num, epint);

			if ((epint & USB_OTG_DIEPINT_XFRC) == USB_OTG_DIEPINT_XFRC) {
				RTK_LOGD(TAG, "IN EP%d XFRC\n", ep_num);

				msk = 0x1UL << (u32)ep_num;
				USB_DEVICE->DIEPEMPMSK &= ~msk;

				USB_PCD_CLEAR_IN_EP_INTR(ep_num, USB_OTG_DIEPINT_XFRC);

				if (pcd->config.dma_enable) {
					pep->xfer_buff += pep->max_packet_len;
				}

				usb_os_spinunlock(pcd->lock);
				usbd_core_data_in_stage(pcd->dev, ep_num, pep->xfer_buff, HAL_OK);
				usb_os_spinlock(pcd->lock);

				if (pcd->config.dma_enable) {
					/* this is ZLP, so prepare EP0 for next setup */
					if ((ep_num == 0U) && (pep->xfer_len == 0U)) {
						/* prepare to rx more setup packets */
						usbd_hal_ep0_out_start(pcd);
					}
				}
			}
			if ((epint & USB_OTG_DIEPINT_TOC) == USB_OTG_DIEPINT_TOC) {
				RTK_LOGD(TAG, "IN EP%d TOC\n", ep_num);
				USB_PCD_CLEAR_IN_EP_INTR(ep_num, USB_OTG_DIEPINT_TOC);
			}
#if 0
			if ((epint & USB_OTG_DIEPINT_ITTXFE) == USB_OTG_DIEPINT_ITTXFE) {
				RTK_LOGD(TAG, "IN EP%d: ITTXFE\n", ep_num);
				USB_PCD_CLEAR_IN_EP_INTR(ep_num, USB_OTG_DIEPINT_ITTXFE);
			}
#endif
			if ((epint & USB_OTG_DIEPINT_INEPNM) == USB_OTG_DIEPINT_INEPNM) {
				RTK_LOGD(TAG, "IN EP%d INEPNM\n", ep_num);
				usbd_pcd_handle_in_ep_mismatch_interrupt(pcd, ep_num);
				USB_PCD_CLEAR_IN_EP_INTR(ep_num, USB_OTG_DIEPINT_INEPNM);
			}
			if ((epint & USB_OTG_DIEPINT_INEPNE) == USB_OTG_DIEPINT_INEPNE) {
				RTK_LOGD(TAG, "IN EP%d INEPNE\n", ep_num);
				usbd_pcd_handle_in_ep_nak_effective_interrupt(pcd, ep_num);
				USB_PCD_CLEAR_IN_EP_INTR(ep_num, USB_OTG_DIEPINT_INEPNE);
			}
			if ((epint & USB_OTG_DIEPINT_EPDISD) == USB_OTG_DIEPINT_EPDISD) {
				RTK_LOGD(TAG, "IN EP%d EPDISD\n", ep_num);
				usbd_pcd_handle_in_ep_disabled_interrupt(pcd, ep_num);
				USB_PCD_CLEAR_IN_EP_INTR(ep_num, USB_OTG_DIEPINT_EPDISD);
			}
			if ((epint & USB_OTG_DIEPINT_TXFE) == USB_OTG_DIEPINT_TXFE) {
				RTK_LOGD(TAG, "IN EP%d TXFE\n", ep_num);
				usbd_pcd_handle_in_ep_tx_fifo_empty_interrupt(pcd, ep_num);
			}
		}
		ep_num++;
		ep_intr >>= 1U;
	}
}

/**
  * @brief  Handle PCD RxFIFO non-empty interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_rx_fifo_non_empty_interrupt(usbd_pcd_t *pcd)
{
	usbd_pcd_ep_t *ep;
	u32 reg;
	u32 xfer_cnt;
	u8 ep_num;

	USB_PCD_MASK_INTERRUPT(USB_OTG_GINTSTS_RXFLVL);

	reg = USB_GLOBAL->GRXSTSP;
	xfer_cnt = (reg & USB_OTG_GRXSTSP_BCNT) >> USB_OTG_GRXSTSP_BCNT_Pos;

	ep = &pcd->out_ep[reg & USB_OTG_GRXSTSP_EPNUM];
	ep_num = USB_EP_NUM(ep->addr);

	if (((reg & USB_OTG_GRXSTSP_PKTSTS) >> USB_OTG_GRXSTSP_PKTSTS_Pos) ==  STS_DATA_UPDT) {
		if (xfer_cnt != 0U) {
			RTK_LOGD(TAG, "RXFLVL EP%02X STS_DATA_UPDT %d\n", ep->addr, xfer_cnt);
			if ((ep->xfer_buff != NULL) && (ep->xfer_count + xfer_cnt <= ep->xfer_len)) {
				usb_hal_read_packet(ep->xfer_buff, ep_num, (u16)(xfer_cnt & 0xFFFF));
				ep->xfer_buff += xfer_cnt;
				ep->xfer_count += xfer_cnt;
			}
		} else {
			RTK_LOGD(TAG, "RXFLVL EP%02X STS_DATA_UPDT ZLP\n", ep->addr);
		}
	} else if (((reg & USB_OTG_GRXSTSP_PKTSTS) >> USB_OTG_GRXSTSP_PKTSTS_Pos) ==  STS_SETUP_UPDT) {
		RTK_LOGD(TAG, "RXFLVL EP%02X STS_SETUP_UPDT %d\n", ep->addr, xfer_cnt);
		usb_hal_read_packet((u8 *)pcd->setup, ep_num, 8U);
		ep->xfer_count += xfer_cnt;
	} else {
		/* ... */
		RTK_LOGD(TAG, "RXFLVL EP%02X %d\n", ep->addr, xfer_cnt);
	}
	USB_PCD_UNMASK_INTERRUPT(USB_OTG_GINTSTS_RXFLVL);
}

/**
  * @brief  Get IN EP number sequence as per IN Token learn queue
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_get_in_ep_sequence_from_in_token_queue(usbd_pcd_t *pcd)
{
	UNUSED(pcd);
	u32 i;
	u32 idx;
	u32 cnt;
	int start;
	int end;
	int sort_done;
	u32 dtknqr1;
	u32 dtknqr2;
	u8 intkn_seq[USB_IN_TOKEN_QUEUE_DEPTH];
	u8 seq[USB_MAX_ENDPOINTS];
	u8 temp;
	u32 ndx;

	dtknqr1 = USB_DEVICE->DTKNQR1;
	dtknqr2 = USB_DEVICE->DTKNQR2;

	/* Flush in token learning queue */
	USB_GLOBAL->GRSTCTL |= USB_OTG_GRSTCTL_INTKNQFLSH;

	/* Mark ep senquence[] by 0xff */
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		pcd->in_ep_sequence[i] = 0xFF;
	}

	if ((dtknqr1 & USB_OTG_DTKNQR1_WRAPBIT) != 0) {
		start = (int)(dtknqr1 & USB_OTG_DTKNQR1_INTKNWPTR_Msk);
		end = start - 1;
		if (end < 0) {
			end = USB_IN_TOKEN_QUEUE_DEPTH - 1;
		}
		cnt = USB_IN_TOKEN_QUEUE_DEPTH;
	} else {
		start = 0;
		if ((dtknqr1 & USB_OTG_DTKNQR1_INTKNWPTR_Msk) == 0) {
			// No in tokens received
			return;
		} else {
			end = (int)(dtknqr1 & USB_OTG_DTKNQR1_INTKNWPTR_Msk) - 1;
			cnt = end - start + 1;
		}
	}

	/* Fill seqnum[] by initial values: EP number + 31 */
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		seq[i] = i + 31;
	}
	/* Fill intkn_seq[] from in_tkn_epnums[0] */
	for (i = 0; i < 6; i++) {
		intkn_seq[i] = (dtknqr1 >> (8 + i * 4)) & 0xF;
	}

	for (i = 6; i < USB_IN_TOKEN_QUEUE_DEPTH; i++) {
		intkn_seq[i] = (dtknqr2 >> ((i - 6) * 4)) & 0xF;
	}

#if IN_TOKEN_PREDICT_DEBUG_EN
	for (i = 0; i < USB_IN_TOKEN_QUEUE_DEPTH; i++) {
		RTK_LOGD(TAG, "Raw INTKN[%d]=EP%d\n", i, intkn_seq[i]);
	}

	RTK_LOGD(TAG, "Start=%d end=%d\n", start, end);
#endif

	/* Update seqnum based on intkn_seq[] */
	i = 0;
	idx = start;
	do {
		seq[intkn_seq[idx]] = i;
		idx++;
		i++;
		if (idx == USB_IN_TOKEN_QUEUE_DEPTH) {
			idx = 0;
		}
	} while (i < cnt);

#if IN_TOKEN_PREDICT_DEBUG_EN
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		RTK_LOGD(TAG, "Unsorted SEQ[%d]=%d\n", i, seq[i]);
	}
#endif

	/* Sort seqnum[] */
	sort_done = 0;
	while (!sort_done) {
		sort_done = 1;
		for (i = 0U; i < USB_MAX_ENDPOINTS - 1U; i++) {
			if (seq[i] > seq[i + 1]) {
				temp = seq[i];
				seq[i] = seq[i + 1];
				seq[i + 1] = temp;
				sort_done = 0;
			}
		}
	}

	ndx = start + seq[0];
	if (ndx >= USB_IN_TOKEN_QUEUE_DEPTH) {
		ndx = ndx % USB_IN_TOKEN_QUEUE_DEPTH;
	}
	pcd->first_in_nextep_seq = intkn_seq[ndx];

#if IN_TOKEN_PREDICT_DEBUG_EN
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		RTK_LOGD(TAG, "Sorted SEQ[%d]=%d\n", i, seq[i]);
	}
#endif

	/* Update nextep_seq[] by EP numbers */
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		idx = start + i;
		if (seq[i] < 31) {
			idx = start + seq[i];
			if (idx >= USB_IN_TOKEN_QUEUE_DEPTH) {
				idx = idx % USB_IN_TOKEN_QUEUE_DEPTH;
			}
			pcd->in_ep_sequence[i] = intkn_seq[idx];
		}
	}

#if IN_TOKEN_PREDICT_DEBUG_EN
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		RTK_LOGD(TAG, "INTKN[%d]=%d\n", i, pcd->in_ep_sequence[i]);
	}
#endif
}

/**
  * @brief  Handle PCD EP non-periodical TxFIFO empty interrupt.
  * @param  pcd: PCD handle
  * @param  ep_num: Endpoint number
  * @retval void
  */
USB_TEXT_SECTION
static u8 usbd_pcd_handle_ep_np_tx_fifo_empty_interrupt(usbd_pcd_t *pcd, u8 ep_num)
{
	usbd_pcd_ep_t *ep;
	u32 len;
	u32 reg;
	u8 status = 0;

	ep = &pcd->in_ep[ep_num];

	if ((ep->xfer_len == 0) && (ep->is_zlp == 0)) {
		RTK_LOGD(TAG, "NPTxFEmp: token received for EP%d when TxFIFO is not ready\n", ep_num);
		return 0;
	}

	if ((ep->xfer_count > ep->xfer_len) || ((ep->xfer_len > 0) && (ep->xfer_buff == NULL))) {
		RTK_LOGE(TAG, "NPTxFEmp: invalid parameter\n");
		return 0;
	}

	if ((ep->xfer_len == 0) || (ep->xfer_count == ep->xfer_len)) {
		RTK_LOGD(TAG, "NPTxFEmp: EP%d false alarm\n", ep_num);
		return 0;
	}

	len = ep->xfer_len - ep->xfer_count;
	if (len > ep->max_packet_len) {
		len = ep->max_packet_len;
	}

	RTK_LOGD(TAG, "NPTxFEmp EP%d xfer_len=%d xfer_count=%d len=%d\n", ep_num, ep->xfer_len, ep->xfer_count, len);

	/* While there is space in the queue and space in the FIFO and
	 * More data to tranfer, Write packets to the Tx FIFO */
	reg = USB_GLOBAL->GNPTXSTS;

	while (((reg & USB_OTG_GNPTXSTS_NPTQXSAV_Msk) > 0)
		   && ((reg & USB_OTG_GNPTXSTS_NPTXFSAV_Msk) >= ((len + 3U) / 4U))
		   && (ep->xfer_count < ep->xfer_len)) {
			usb_hal_write_packet(ep->xfer_buff, ep_num, (u16)len);
		ep->xfer_count += len;
		ep->xfer_buff += len;
		len = ep->xfer_len - ep->xfer_count;
		if (len > ep->max_packet_len) {
			len = ep->max_packet_len;
		}
		reg = USB_GLOBAL->GNPTXSTS;
	}

	if (ep->xfer_len > ep->xfer_count) {
		status = 1;
	}

	return status;
}

/**
  * @brief  Handle PCD non-periodical TxFIFO empty interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_np_tx_fifo_empty_interrupt(usbd_pcd_t *pcd)
{
	u8 ep_num;
	u32 i;
	u8 status;
#if IN_TOKEN_PREDICT_WA_EN
	usbd_pcd_ep_t *ep;
#endif

	USB_GLOBAL->GINTMSK &= ~USB_OTG_GINTMSK_NPTXFEM;

	if (pcd->config.dma_enable) {
		return;
	}

	usbd_pcd_get_in_ep_sequence_from_in_token_queue(pcd);

	/* TX as per the prediction result of in token learn queue. */
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		ep_num = pcd->in_ep_sequence[i];
		if (ep_num != 0xFF) {
			if (ep_num < USB_MAX_ENDPOINTS) {
				status = usbd_pcd_handle_ep_np_tx_fifo_empty_interrupt(pcd, ep_num);
				if (status) {
					USB_GLOBAL->GINTMSK |= USB_OTG_GINTMSK_NPTXFEM;
				}
			} else {
				RTK_LOGE(TAG, "Invalid EP num %d in IN token learn queue\n", ep_num);
			}
		}
	}

#if IN_TOKEN_PREDICT_WA_EN
	/* Workaround: Sometimes in tokens cannot be caught by SW ISR after NPTXFE interrupt especially when multiple NP IN endpoints are enabled.
	   However, this will result in infinite interrupts if usbd_ep_transmit/usbd_ep0_transmit improperly called without in tokens on the bus. */
	if ((USB_GLOBAL->GINTMSK & USB_OTG_GINTMSK_NPTXFEM) == 0) {
		for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
			ep = &pcd->in_ep[i];
			if ((ep->tx_fifo_num == 0) && (ep->xfer_len > ep->xfer_count) && ((USB_INEP(i)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) != 0)) {
#ifdef USBD_XFER_ERROR_DETECT_EN
				if (ep->nptx_err_cnt < pcd->config.nptx_max_err_cnt[i]) {
					if (++ep->nptx_err_cnt == pcd->config.nptx_max_err_cnt[i]) {
						RTK_LOGW(TAG, "EP%d TX timeout %d: xfer_len=%d, xfer_count=%d\n", i, ep->nptx_err_cnt, ep->xfer_len, ep->xfer_count);
						usb_os_spinunlock(pcd->lock);
						usbd_core_data_in_stage(pcd->dev, (u8)i, ep->xfer_buff, HAL_TIMEOUT);
						usb_os_spinlock(pcd->lock);
					} else {
						USB_GLOBAL->GINTMSK |= USB_OTG_GINTMSK_NPTXFEM;
						RTK_LOGD(TAG, "EP%d TXE miss %d: xfer_len=%d, xfer_count=%d\n", i, ep->nptx_err_cnt, ep->xfer_len, ep->xfer_count);
					}
				}
#else
				USB_GLOBAL->GINTMSK |= USB_OTG_GINTMSK_NPTXFEM;
				RTK_LOGD(TAG, "EP%d TXE miss: xfer_len=%d, xfer_count=%d\n", i, ep->xfer_len, ep->xfer_count);
#endif
				break;
			}
		}
	}
#endif
}

/**
  * @brief  Handle PCD EP mismatch interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_ep_mismatch_interrupt(usbd_pcd_t *pcd)
{
	u32 reg;
	u32 gintsts = usb_hal_read_interrupts();

	if ((++pcd->nptx_epmis_cnt >= pcd->config.nptx_max_epmis_cnt) && (!(gintsts & (USB_OTG_GINTSTS_GINAKEFF)))) {
		RTK_LOGD(TAG, "EPMIS count overflow: %d\n", pcd->config.nptx_max_epmis_cnt);

		pcd->nptx_epmis_cnt = 0;
		pcd->start_predict = 1;

		/* Disable EP Mismatch interrupt */
		/* Enable the Global IN NAK Effective Interrupt */
		reg = USB_GLOBAL->GINTMSK;
		reg &= ~(USB_OTG_GINTMSK_EPMISM);
		reg |= USB_OTG_GINTMSK_GINAKEFFM;
		USB_GLOBAL->GINTMSK = reg;

		/* Set the global non-periodic IN NAK handshake */
		USB_DEVICE->DCTL |= USB_OTG_DCTL_SGINAK;
	}
}
/**
  * @brief  Handle PCD EP in nak effective interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_in_nak_effective(usbd_pcd_t *pcd)
{
	usbd_pcd_ep_t *ep;
	u32 reg;
	u8 i;
	u8 ep_num;

	/* Disable all active IN EPs(use non-periodic shared TxFifo) */
	for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
		ep = &(pcd->in_ep[i]);
		ep_num = USB_EP_NUM(ep->addr);
		reg = USB_INEP(ep_num)->DIEPCTL;

		if ((!(ep->is_ptx)) && ((reg & USB_OTG_DIEPCTL_EPENA) == USB_OTG_DIEPCTL_EPENA)) {
			//count for how many np eps
			if (pcd->start_predict > 0) {
				pcd->start_predict++;
			}
			reg |= (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK);
			USB_INEP(ep_num)->DIEPCTL = reg;
		}
	}

	/* Disable the Global IN NAK Effective Interrupt */
	USB_GLOBAL->GINTMSK &= ~(USB_OTG_GINTMSK_GINAKEFFM);
}

/**
  * @brief  Handle PCD wakeup interrupt.
  * @param  pcd: PCD handle
  * @retval void
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_wakeup_interrupt(usbd_pcd_t *pcd)
{
	/* Clear the Remote Wake-up Signaling */
	USB_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

	if (pcd->lpm_state == LPM_L1) {
		pcd->lpm_state = LPM_L0;
		// FIXME: Do nothing
	} else {
		usb_os_spinunlock(pcd->lock);
		usbd_core_resume(pcd->dev);
		usb_os_spinlock(pcd->lock);
	}
}

/**
  * @brief  Handle SOF interrupt
  * @param  pcd: PCD handle
  * @retval None
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_sof_interrupt(usbd_pcd_t *pcd)
{
#if USBD_SWITCH_TO_ACTIVE_BY_SOF_INTR
	/* if the config do not config the sof,so should disable it */
	if ((pcd->config.ext_intr_en & USBD_SOF_INTR) == 0) {
		USB_GLOBAL->GINTMSK &= ~(USB_OTG_GINTMSK_SOFM);
	}
#endif

	usb_os_spinunlock(pcd->lock);
	usbd_core_sof(pcd->dev);
	usb_os_spinlock(pcd->lock);
}

/**
  * @brief  Handle EOPF interrupt
  * @param  pcd: PCD handle
  * @retval None
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_eopf_interrupt(usbd_pcd_t *pcd)
{
	usb_dev_t *dev = pcd->dev;
	u8 ep_num;
	if (dev->dev_state == USBD_STATE_CONFIGURED) {
		usbd_pcd_ep_t *ep;

		for (ep_num = 1U; ep_num < USB_MAX_ENDPOINTS; ep_num++) {
			ep = &(pcd->out_ep[ep_num]);
			if ((USB_CH_EP_TYPE_ISOC == ep->type) && USB_EP_IS_OUT(ep->addr) && ((USB_OUTEP(ep_num)->DOEPCTL & USB_OTG_DOEPCTL_EPENA) == USB_OTG_DOEPCTL_EPENA)) {
				if ((USB_DEVICE->DSTS & BIT8) == 0U) { //work in next SOF
					USB_OUTEP(ep_num)->DOEPCTL |= USB_OTG_DOEPCTL_SODDFRM;
				} else {
					USB_OUTEP(ep_num)->DOEPCTL |= USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
				}
			}
		}
	}
}

/**
  * @brief  Handle SRQ interrupt
  * @param  pcd: PCD handle
  * @retval None
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_srq_interrupt(usbd_pcd_t *pcd)
{
	usb_os_spinunlock(pcd->lock);
	usbd_core_connected(pcd->dev);
	usb_os_spinlock(pcd->lock);
}

/**
  * @brief  Handle OTG interrupt
  * @param  pcd: PCD handle
  * @retval None
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_otg_interrupt(usbd_pcd_t *pcd)
{
	u32 reg = USB_GLOBAL->GOTGINT;

	if ((reg & USB_OTG_GOTGINT_SEDET) == USB_OTG_GOTGINT_SEDET) {
		RTK_LOGD(TAG, "GOTGINT_SEDET\n");
		usb_os_spinunlock(pcd->lock);
		usbd_core_disconnected(pcd->dev);
		usb_os_spinlock(pcd->lock);
	}

	USB_GLOBAL->GOTGINT |= reg;
}

/**
  * @brief  Handles PCD interrupt request.
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
static void usbd_pcd_handle_interrupt(usbd_pcd_t *pcd)
{
	/* ensure that we are in device mode */
	if (usb_hal_get_otg_mode() == USB_OTG_MODE_DEVICE) {

		usb_os_spinlock(pcd->lock);

		u32  gintsts =  usb_hal_read_interrupts();

		/* avoid spurious interrupt */
		if (gintsts == 0U) {
			usb_os_spinunlock(pcd->lock);
			return;
		}

		//RTK_LOGD(TAG, "====== IRQ 0x%08X =======\n", usb_hal_read_interrupts());

		if (gintsts & (USB_OTG_GINTSTS_IEPINT)) {
			RTK_LOGD(TAG, "IEPINT\n");
			usbd_pcd_handle_in_ep_interrupt(pcd);
		}

		if (gintsts & (USB_OTG_GINTSTS_OEPINT)) {
			RTK_LOGD(TAG, "OEPINT\n");
			usbd_pcd_handle_out_ep_interrupt(pcd);
		}

		if (gintsts & (USB_OTG_GINTSTS_IISOIXFR)) {
			RTK_LOGD(TAG, "incompISOIN\n");
			usbd_pcd_handle_incomplete_in_isoc(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_IISOIXFR);
		}

		/* Handle Resume Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_WKUINT)) {
			RTK_LOGD(TAG, "WKUINT\n");
			usbd_pcd_handle_wakeup_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_WKUINT);
		}

		/* Handle Suspend Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_USBSUSP)) {
			RTK_LOGD(TAG, "USBSUSP\n");
			usbd_pcd_handle_suspend_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_USBSUSP);
		}
		/* Handle Reset Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_USBRST)) {
			RTK_LOGD(TAG, "USBRST\n");
			usbd_pcd_handle_reset_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_USBRST);
		}

		/* Handle Enumeration done Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_ENUMDNE)) {
			RTK_LOGD(TAG, "ENUMDNE\n");
			usbd_pcd_handle_enum_done_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_ENUMDNE);
		}

		/* Handle RxQLevel Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_RXFLVL)) {
			RTK_LOGD(TAG, "RXFLVL\n");
			usbd_pcd_handle_rx_fifo_non_empty_interrupt(pcd);
		}

		/* Handle NP TxFIFO Empty Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_NPTXFE)) {
			RTK_LOGD(TAG, "NPTXFE\n");
			usbd_pcd_handle_np_tx_fifo_empty_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_NPTXFE);
		}

		/* Handle EPMIS Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_EPMIS)) {
			RTK_LOGD(TAG, "EPMIS\n");
			usbd_pcd_handle_ep_mismatch_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_EPMIS);
		}

		/* Handle GINAKEFF Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_GINAKEFF)) {
			RTK_LOGD(TAG, "GINAKEFF\n");
			usbd_pcd_handle_in_nak_effective(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_GINAKEFF);
		}

		/* Handle SOF Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_SOF)) {
			RTK_LOGD(TAG, "SOF\n");
#if USBD_SWITCH_TO_ACTIVE_BY_SOF_INTR
			/*
				if the device wakes up the host remotely,
				WKUINT intr will not be triggered
				depend on sof intr to run the wakeup callback
			*/
			usbd_pcd_handle_wakeup_interrupt(pcd);
#endif
			usbd_pcd_handle_sof_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_SOF);
		}

		/* Handle EOPF Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_EOPF)) {
			//RTK_LOGD(TAG, "EOPF\n");
			usbd_pcd_handle_eopf_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_EOPF);
		}

		/* Handle Connection event Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_SRQINT)) {
			RTK_LOGD(TAG, "SRQINT\n");
			usbd_pcd_handle_srq_interrupt(pcd);
			USB_PCD_CLEAR_FLAG(USB_OTG_GINTSTS_SRQINT);
		}

		/* Handle Disconnection event Interrupt */
		if (gintsts & (USB_OTG_GINTSTS_OTGINT)) {
			RTK_LOGD(TAG, "OTGINT\n");
			usbd_pcd_handle_otg_interrupt(pcd);
		}

		usb_os_spinunlock(pcd->lock);
	}
}


/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize PCD
  * @param  dev: USB device instance
  * @param  config: USB device configuration
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_init(usb_dev_t *dev, usbd_config_t *config)
{
	u8 i;
	u8 ret = HAL_OK;
	usbd_pcd_ep_t *ep;
	usbd_pcd_t *pcd = &usbd_pcd;

	dev->pcd = &usbd_pcd;
	pcd->dev = dev;

	RTK_LOGI(TAG, "USB configurations:\n");
	RTK_LOGI(TAG, "* speed: %d\n", config->speed);
	RTK_LOGI(TAG, "* dma_enable: %d\n", config->dma_enable);
	RTK_LOGI(TAG, "* isr_priority: %d\n", config->isr_priority);
	RTK_LOGI(TAG, "* intr_use_ptx_fifo: %d\n", config->intr_use_ptx_fifo);
	RTK_LOGI(TAG, "* ptx_fifo_first: %d\n", config->ptx_fifo_first);
	RTK_LOGI(TAG, "* ext_intr_en: 0x%08X\n", config->ext_intr_en);
	RTK_LOGI(TAG, "* nptx_max_epmis_cnt: %d\n", config->nptx_max_epmis_cnt);
	for (i = 0U; i < USB_MAX_ENDPOINTS; i++) {
		RTK_LOGI(TAG, "* nptx_max_err_cnt[%d]: %d\n", i, config->nptx_max_err_cnt[i]);
	}

	memcpy((void *)&pcd->config, (void *)config, sizeof(usbd_config_t));

	for (i = 0U; i < USB_MAX_ENDPOINTS; i++) {
		if (pcd->config.nptx_max_err_cnt[i] == 0U) {
			pcd->config.nptx_max_err_cnt[i] = USBD_NPTX_DEF_ERR_CNT;
		}
	}

	if (pcd->pcd_state == HAL_PCD_STATE_RESET) {
		/* Allocate lock resource and initialize it */
		pcd->lock = usb_os_spinlock_alloc();
		if (pcd->lock == NULL) {
			RTK_LOGE(TAG, "PCD lock init fail\n");
			pcd->pcd_state = HAL_PCD_STATE_ERROR;
			ret = HAL_ERR_MEM;
			goto errexit;
		}

		/* Init the low level hardware: GPIO, CLOCK, PHY... */
		ret = usb_chip_init();
		if (ret != HAL_OK) {
			RTK_LOGE(TAG, "USB low-level init fail\n");
			pcd->pcd_state = HAL_PCD_STATE_ERROR;
			goto errexit;
		}
	}

	pcd->pcd_state = HAL_PCD_STATE_BUSY;
	pcd->phy_initialized = 0;
	pcd->isr_initialized = 0;
	pcd->setup = (u8 *)usb_os_mem_alloc(USBD_SETUP_PACKET_BUF_LEN);
	if (pcd->setup == NULL) {
		ret = HAL_ERR_MEM;
		goto errexit;
	}
	if (config->dma_enable && (!USB_IS_MEM_DMA_ALIGNED(pcd->setup))) {
		RTK_LOGE(TAG, "Setup buffer alignment error!\n");
		ret = HAL_ERR_MEM;
		goto errexit;
	}

	/* Disable the Interrupts */
	usb_hal_disable_global_interrupt();

	/* Init the Core (common init.) */
	ret = usb_hal_core_init(config->dma_enable);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Core init fail\n");
		pcd->pcd_state = HAL_PCD_STATE_ERROR;
		goto errexit;
	}

	/* Force Device Mode*/
	usb_hal_set_otg_mode((usb_otg_mode_t)USB_OTG_MODE_DEVICE);

	/* Init Device */
	ret = usbd_hal_device_init(pcd);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Device init fail\n");
		pcd->pcd_state = HAL_PCD_STATE_ERROR;
		goto errexit;
	}

	/* Init endpoints structures */
	for (i = 0U; i < USB_MAX_ENDPOINTS; i++) {
		ep = &pcd->in_ep[i];
		/* Init ep structure */
		ep->addr = i | USB_D2H;
		if (pcd->ded_tx_fifo_en) {
			ep->tx_fifo_num = i;
		} else {
			ep->tx_fifo_num = 0U;
		}
		/* Control until ep is activated */
		ep->type = USB_CH_EP_TYPE_CTRL;
		ep->max_packet_len = 0U;
		ep->xfer_buff = 0U;
		ep->xfer_len = 0U;
		ep->is_zlp = 0U;
		ep->is_ptx = 0U;
		ep->is_initialized = 0U;
	}

	for (i = 0U; i < USB_MAX_ENDPOINTS; i++) {
		ep = &pcd->out_ep[i];
		ep->addr = i | USB_H2D;
		/* Control until ep is activated */
		ep->type = USB_CH_EP_TYPE_CTRL;
		ep->max_packet_len = 0U;
		ep->xfer_buff = 0U;
		ep->xfer_len = 0U;
		ep->is_zlp = 0U;
		ep->is_initialized = 0U;
	}

	pcd->address = 0U;
	pcd->pcd_state = HAL_PCD_STATE_READY;

	usbd_hal_disconnect(pcd);

#if CONFIG_USB_PHY
#if CONFIG_USB_OTG
	ret = usb_hal_enable_otg();
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "OTG enable fail\n");
		pcd->pcd_state = HAL_PCD_STATE_ERROR;
		goto errexit;
	}
#endif

	ret = usb_hal_calibrate(USB_OTG_MODE_DEVICE);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "PHY calibration fail\n");
		pcd->pcd_state = HAL_PCD_STATE_ERROR;
		goto errexit;
	}
#endif

	ret = usbd_hal_config_dfifo(pcd);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "DFIFO config fail\n");
		pcd->pcd_state = HAL_PCD_STATE_ERROR;
		goto errexit;
	}

	ret = usbd_pcd_interrupt_init(pcd);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Interrupt init fail\n");
		pcd->pcd_state = HAL_PCD_STATE_ERROR;
		goto errexit;
	}

	return HAL_OK;

errexit:

	if (pcd->lock) {
		usb_os_spinlock_free(pcd->lock);
	}
	if (pcd->setup) {
		usb_os_mem_free(pcd->setup);
	}
	return ret;
}

/**
  * @brief  DeInitialize PCD
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_deinit(usb_dev_t *dev)
{
	u8 result = HAL_OK;

	usbd_pcd_t *pcd = dev->pcd;

	if (pcd == NULL) {
		return HAL_OK;
	}

	usbd_pcd_interrupt_deinit(pcd);

	pcd->pcd_state = HAL_PCD_STATE_BUSY;

	/* Stop Device */
	result = usbd_pcd_stop(pcd);

	usb_chip_deinit();

	usb_os_spinlock_free(pcd->lock);

	if (pcd->setup != NULL) {
		usb_os_mem_free(pcd->setup);
		pcd->setup = NULL;
	}

	pcd->lock = NULL;
	dev->pcd = NULL;

	pcd->pcd_state = HAL_PCD_STATE_RESET;

	return result;
}

/**
  * @brief  Start the PCD
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_start(usbd_pcd_t *pcd)
{
	usb_os_spinlock(pcd->lock);
	usbd_hal_connect(pcd);
	usb_hal_enable_global_interrupt();
	usb_os_spinunlock(pcd->lock);
	return HAL_OK;
}

/**
  * @brief  Stop the PCD
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_stop(usbd_pcd_t *pcd)
{
	usb_os_spinlock(pcd->lock);

	if (pcd->pcd_state != HAL_PCD_STATE_STOP) {
		usb_hal_disable_global_interrupt();
		if (usbd_hal_device_stop(pcd) != HAL_OK) {
			usb_os_spinunlock(pcd->lock);
			return HAL_ERR_HW;
		}
		usbd_hal_disconnect(pcd);
		pcd->pcd_state = HAL_PCD_STATE_STOP;
	}

	usb_os_spinunlock(pcd->lock);

	return HAL_OK;
}

/**
  * @brief  Connect the USB device
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_dev_connected(usbd_pcd_t *pcd)
{
	usb_os_spinlock(pcd->lock);
	usbd_hal_connect(pcd);
	usb_os_spinunlock(pcd->lock);
	return HAL_OK;
}

/**
  * @brief  Disconnect the USB device.
  * @param  pcd: PCD handle
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_dev_desconnected(usbd_pcd_t *pcd)
{
	usb_os_spinlock(pcd->lock);
	usbd_hal_disconnect(pcd);
	usb_os_spinunlock(pcd->lock);
	return HAL_OK;
}

/**
  * @brief  Set the USB Device address.
  * @param  pcd: PCD handle
  * @param  address: New device address
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_set_address(usbd_pcd_t *pcd, u8 address)
{
	usb_os_spinlock(pcd->lock);
	pcd->address = address;
	usbd_hal_set_device_address(pcd, address);
	usb_os_spinunlock(pcd->lock);
	return HAL_OK;
}

/**
  * @brief  Open and configure an endpoint.
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @param  ep_mps: Endpoint max packet size
  * @param  ep_type: Endpoint type
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_init(usbd_pcd_t *pcd, u8 ep_addr, u16 ep_mps, u8 ep_type)
{
	u8 is_ptx = 0;
	u8 ret = HAL_OK;
	usbd_pcd_ep_t *ep;
	u8 ep_num = USB_EP_NUM(ep_addr);

	if (USB_EP_IS_IN(ep_addr)) {
		ep = &pcd->in_ep[ep_num];
		is_ptx = (ep_type == USB_CH_EP_TYPE_ISOC) ||
				 ((ep_type == USB_CH_EP_TYPE_INTR) && (pcd->config.intr_use_ptx_fifo != 0U) && (pcd->ded_tx_fifo_en == 0U));
	} else {
		ep = &pcd->out_ep[ep_addr & EP_ADDR_MSK];
	}

	ep->addr = ep_addr;
	ep->max_packet_len = ep_mps;
	ep->type = ep_type;
	ep->is_initialized = 1U;
	ep->is_ptx = is_ptx;
	if (USB_EP_IS_IN(ep_addr)) {
		if (pcd->ded_tx_fifo_en) {
			ep->tx_fifo_num = ep_num;
		} else {
			if ((ep_type == USB_CH_EP_TYPE_ISOC) ||
				((ep_type == USB_CH_EP_TYPE_INTR) && (pcd->config.intr_use_ptx_fifo != 0U))) {
				ep->tx_fifo_num = 1;
			} else {
				ep->tx_fifo_num = 0;
			}
		}
	}

	usb_os_spinlock(pcd->lock);
	usbd_hal_ep_activate(pcd, ep);
	usb_os_spinunlock(pcd->lock);

	return ret;
}

/**
  * @brief  Deactivate an endpoint.
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_deinit(usbd_pcd_t *pcd, u8 ep_addr)
{
	usbd_pcd_ep_t *ep;
	u8 ep_num = USB_EP_NUM(ep_addr);

	if (USB_EP_IS_IN(ep_addr)) {
		ep = &pcd->in_ep[ep_num];
	} else {
		ep = &pcd->out_ep[ep_num];
	}
	ep->addr = ep_addr;
	ep->is_initialized = 0U;
	ep->is_ptx = 0U;

	usb_os_spinlock(pcd->lock);
	usbd_hal_ep_deactivate(pcd, ep);
	usb_os_spinunlock(pcd->lock);
	return HAL_OK;
}

/**
  * @brief  Receive an amount of data.
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @param  buf: Pointer to the reception buffer
  * @param  len: Amount of data to be received
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_receive(usbd_pcd_t *pcd, u8 ep_addr, u8 *buf, u32 len)
{
	usbd_pcd_ep_t *ep;
	u32 dma = pcd->config.dma_enable;
	u8 ep_num = USB_EP_NUM(ep_addr);

	if ((dma != 0) && (buf != NULL) && (len != 0)) {
		if (USB_IS_MEM_DMA_ALIGNED(buf)) {
			DCache_Clean((u32)buf, len);
		} else {
			RTK_LOGE(TAG, "EP%02X RX buffer alignment error!\n", ep_addr);
			return HAL_ERR_MEM;
		}
	}

	ep = &pcd->out_ep[ep_num];

	/*setup and start the Xfer */
	ep->xfer_buff = buf;
	ep->xfer_len = len;
	ep->xfer_count = 0U;
	ep->is_zlp = (len == 0) ? 1 : 0;
	ep->addr = ep_addr;

	if (dma) {
		ep->dma_addr = (u32)buf;
	}

	usb_os_spinlock_safe(pcd->lock);

	if (ep_num == 0U) {
		usbd_hal_ep0_start_transfer(pcd, ep);
	} else {
		usbd_hal_ep_start_transfer(pcd, ep);
	}

	usb_os_spinunlock_safe(pcd->lock);

	return HAL_OK;
}

/**
  * @brief  Get Received Data Size
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @retval Data Size
  */
USB_TEXT_SECTION
u32 usbd_pcd_ep_get_rx_data_size(usbd_pcd_t *pcd, u8 ep_num)
{
	return pcd->out_ep[ep_num].xfer_count;
}

/**
  * @brief  Send an amount of data
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @param  buf: Pointer to the transmission buffer
  * @param  len: Amount of data to be sent
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_transmit(usbd_pcd_t *pcd, u8 ep_addr, u8 *buf, u32 len)
{
	usbd_pcd_ep_t *ep;
	u32 dma = pcd->config.dma_enable;
	u8 ep_num = USB_EP_NUM(ep_addr);

	if ((dma != 0) && (buf != NULL) && (len != 0)) {
		if (USB_IS_MEM_DMA_ALIGNED(buf)) {
			DCache_Clean((u32)buf, len);
		} else {
			RTK_LOGE(TAG, "EP%02X TX buffer alignment error!\n", ep_addr);
			return HAL_ERR_MEM;
		}
	}

	ep = &pcd->in_ep[ep_num];

	/* Setup and start the transfer */
	ep->xfer_buff = buf;
	ep->xfer_len = len;
	ep->xfer_count = 0U;
	ep->addr = ep_addr;
	ep->is_zlp = (len == 0) ? 1 : 0;

	if (dma) {
		ep->dma_addr = (u32)buf;
	}

#ifdef USBD_XFER_ERROR_DETECT_EN
	ep->nptx_err_cnt = 0;
#endif

	usb_os_spinlock_safe(pcd->lock);

	if (ep_num == 0U) {
		usbd_hal_ep0_start_transfer(pcd, ep);
	} else {
		usbd_hal_ep_start_transfer(pcd, ep);
	}

	usb_os_spinunlock_safe(pcd->lock);

	return HAL_OK;
}

/**
  * @brief  Set a STALL condition over an endpoint
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_set_stall(usbd_pcd_t *pcd, u8 ep_addr)
{
	usbd_pcd_ep_t *ep;
	u8 ep_num = USB_EP_NUM(ep_addr);

	if (ep_num >= USB_MAX_ENDPOINTS) {
		return HAL_ERR_PARA;
	}

	if (USB_EP_IS_IN(ep_addr)) {
		ep = &pcd->in_ep[ep_num];
	} else {
		ep = &pcd->out_ep[ep_num];
	}

	ep->is_stall = 1U;
	ep->addr = ep_addr;

	usb_os_spinlock_safe(pcd->lock);

	usbd_hal_ep_set_stall(pcd, ep);
	if (ep_num == 0U) {
		usbd_hal_ep0_out_start(pcd);
	}

	usb_os_spinunlock_safe(pcd->lock);

	return HAL_OK;
}

/**
  * @brief  Clear a STALL condition over in an endpoint
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_clear_stall(usbd_pcd_t *pcd, u8 ep_addr)
{
	usbd_pcd_ep_t *ep;
	u8 ep_num = USB_EP_NUM(ep_addr);

	if (ep_num >= USB_MAX_ENDPOINTS) {
		return HAL_ERR_PARA;
	}

	if (USB_EP_IS_IN(ep_addr)) {
		ep = &pcd->in_ep[ep_num];
	} else {
		ep = &pcd->out_ep[ep_num];
	}

	ep->is_stall = 0U;
	ep->addr = ep_addr;

	usb_os_spinlock_safe(pcd->lock);
	usbd_hal_ep_clear_stall(pcd, ep);
	usb_os_spinunlock_safe(pcd->lock);

	return HAL_OK;
}

/**
  * @brief  Flush an endpoint
  * @param  pcd: PCD handle
  * @param  ep_addr: Endpoint address
  * @retval HAL status
  */
USB_TEXT_SECTION
u8 usbd_pcd_ep_flush(usbd_pcd_t *pcd, u8 ep_addr)
{
	u32 fifo_num;
	usbd_pcd_ep_t *ep;

	usb_os_spinlock(pcd->lock);

	if (USB_EP_IS_IN(ep_addr)) {
		ep = &pcd->in_ep[USB_EP_NUM(ep_addr)];
		fifo_num = usbd_hal_get_tx_fifo_num(pcd, ep);
		usb_hal_flush_tx_fifo(fifo_num);
	} else {
		usb_hal_flush_rx_fifo();
	}

	usb_os_spinunlock(pcd->lock);

	return HAL_OK;
}



