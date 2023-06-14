/**
  ******************************************************************************
  * @file    usb_hal.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usb_hal.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USB_HAL_H
#define USB_HAL_H

/* Includes ------------------------------------------------------------------*/

#include "basic_types.h"
#include "usb_ch9.h"
#include "usb_os.h"

/* Exported defines ----------------------------------------------------------*/

/* USB section defines */
#define USB_DATA_SECTION
#define USB_BSS_SECTION
#define USB_TEXT_SECTION

/* USB Core ID */
#define USB_OTG_CORE_ID_300A                   0x4F54300AU
#define USB_OTG_CORE_ID_310A                   0x4F54310AU

/* Exported types ------------------------------------------------------------*/

/* Unaligned 32 bit type for DFIFO usage */
typedef struct {
	u32 data __PACKED;
} usb_unaligned_u32_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usb_hal_core_init(u8 dma_enable);
void usb_hal_enable_global_interrupt(void);
void usb_hal_disable_global_interrupt(void);
u32 usb_hal_read_interrupts(void);
void usb_hal_clear_interrupts(u32 interrupt);
u8 usb_hal_set_otg_mode(usb_otg_mode_t mode);
usb_otg_mode_t usb_hal_get_otg_mode(void);
u8 usb_hal_flush_tx_fifo(u32 num);
u8 usb_hal_flush_rx_fifo(void);
u8 usb_hal_write_packet(u8 *src, u8 ep_ch_num, u16 len);
u8 usb_hal_read_packet(u8 *dest, u16 len);

#endif /* USB_HAL_H */
