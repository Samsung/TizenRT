/**
  ******************************************************************************
  * @file    usbd_core.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbd_core.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __USBD_CORE_H
#define __USBD_CORE_H

/* Includes ------------------------------------------------------------------*/

#include "usbd_hal.h"

/* Exported defines ----------------------------------------------------------*/

/*  EP0 State */
#define USBD_EP0_IDLE                                  0x00U
#define USBD_EP0_SETUP                                 0x01U
#define USBD_EP0_DATA_IN                               0x02U
#define USBD_EP0_DATA_OUT                              0x03U
#define USBD_EP0_STATUS_IN                             0x04U
#define USBD_EP0_STATUS_OUT                            0x05U
#define USBD_EP0_STALL                                 0x06U

/* Exported types ------------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_core_setup_stage(usb_dev_t *dev, u8 *setup);
u8 usbd_core_data_out_stage(usb_dev_t *dev, u8 ep_num, u8 *buf);
u8 usbd_core_data_in_stage(usb_dev_t *dev, u8 ep_num, u8 *buf, u8 status);
u8 usbd_core_reset(usb_dev_t  *dev);
u8 usbd_core_set_speed(usb_dev_t  *dev, usb_speed_type_t speed);
u8 usbd_core_suspend(usb_dev_t  *dev);
u8 usbd_core_resume(usb_dev_t  *dev);
u8 usbd_core_sof(usb_dev_t  *dev);
u8 usbd_core_connected(usb_dev_t  *dev);
u8 usbd_core_disconnected(usb_dev_t  *dev);
u8 usbd_core_ep_set_stall(usb_dev_t *dev, u8 ep_addr);
u8 usbd_core_ep_clear_stall(usb_dev_t *dev, u8 ep_addr);
u8 usbd_core_ep_is_stall(usb_dev_t *dev, u8 ep_addr);
u8 usbd_core_ep0_set_stall(usb_dev_t *dev);
u32 usbd_core_get_rx_data_size(usb_dev_t *dev, u8  ep_addr);
u8 usbd_core_ep0_transmit_status(usb_dev_t *dev);
u8 usbd_core_ep0_receive_status(usb_dev_t *dev);

#endif /* __USBD_CORE_H */

