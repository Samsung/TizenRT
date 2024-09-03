/**
  ******************************************************************************
  * @file    usbd_pcd.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbd_pcd.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBD_PCD_H
#define USBD_PCD_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_hal.h"

/* Exported defines ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

#define USB_PCD_MASK_INTERRUPT(__INTERRUPT__)     			((USB_GLOBAL)->GINTMSK &= ~(__INTERRUPT__))
#define USB_PCD_UNMASK_INTERRUPT(__INTERRUPT__)  			((USB_GLOBAL)->GINTMSK |= (__INTERRUPT__))

#define USB_PCD_CLEAR_IN_EP_INTR(__EPNUM__, __INTERRUPT__)  (USB_INEP(__EPNUM__)->DIEPINT = (__INTERRUPT__))
#define USB_PCD_CLEAR_OUT_EP_INTR(__EPNUM__, __INTERRUPT__) (USB_OUTEP(__EPNUM__)->DOEPINT = (__INTERRUPT__))

#define USB_PCD_GET_FLAG(__INTERRUPT__)      				((usb_hal_read_interrupts() & (__INTERRUPT__)) == (__INTERRUPT__))
#define USB_PCD_CLEAR_FLAG(__INTERRUPT__)    				(((USB_GLOBAL)->GINTSTS) &=  (__INTERRUPT__))

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_pcd_init(usb_dev_t *dev, usbd_config_t *config);
u8 usbd_pcd_deinit(usb_dev_t *dev);
u8 usbd_pcd_start(usbd_pcd_t *pcd);
u8 usbd_pcd_stop(usbd_pcd_t *pcd);
u8 usbd_pcd_dev_connected(usbd_pcd_t *pcd);
u8 usbd_pcd_dev_desconnected(usbd_pcd_t *pcd);
u8 usbd_pcd_set_address(usbd_pcd_t *pcd, u8 address);
u8 usbd_pcd_ep_init(usbd_pcd_t *pcd, u8 ep_addr, u16 ep_mps, u8 ep_type);
u8 usbd_pcd_ep_deinit(usbd_pcd_t *pcd, u8 ep_addr);
u8 usbd_pcd_ep_receive(usbd_pcd_t *pcd, u8 ep_addr, u8 *buf, u32 len);
u8 usbd_pcd_ep_transmit(usbd_pcd_t *pcd, u8 ep_addr, u8 *buf, u32 len);
u32 usbd_pcd_ep_get_rx_data_size(usbd_pcd_t *pcd, u8 ep_addr);
u8 usbd_pcd_ep_set_stall(usbd_pcd_t *pcd, u8 ep_addr);
u8 usbd_pcd_ep_clear_stall(usbd_pcd_t *pcd, u8 ep_addr);
u8 usbd_pcd_ep_flush(usbd_pcd_t *pcd, u8 ep_addr);

#endif /* USBD_PCD_H */

