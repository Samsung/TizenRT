/**
  ******************************************************************************
  * @file    usbd_hal.h
  * @author  Realsil WLAN5 Team
  * @brief   This file is the header file for usbd_hal.c
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBH_HAL_H
#define USBH_HAL_H

/* Includes ------------------------------------------------------------------*/

#include "ameba_soc.h"
#include "usb_ch9.h"
#include "usb_os.h"
#include "usb_hal.h"
#include "usbh.h"

/* Exported defines ----------------------------------------------------------*/

/* USB section defines */
#define USBH_DATA_SECTION
#define USBH_BSS_SECTION
#define USBH_TEXT_SECTION

#define USBH_HCFG_30_60_MHZ                         0U
#define USBH_HCFG_48_MHZ                            1U
#define USBH_HCFG_6_MHZ                             2U

#define USBH_GRXSTS_PKTSTS_IN                       2U
#define USBH_GRXSTS_PKTSTS_IN_XFER_COMP             3U
#define USBH_GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5U
#define USBH_GRXSTS_PKTSTS_CH_HALTED                7U

/* Exported types ------------------------------------------------------------*/

/* Host channel states definition */
typedef enum {
	USBH_PID_DATA0 = 0U,
	USBH_PID_DATA2 = 1U,
	USBH_PID_DATA1 = 2U,
	USBH_PID_SETUP = 3U
} usbh_pid_t;

/* Host channel PID definition */
typedef enum {
	USBH_HC_IDLE = 0U,
	USBH_HC_XFRC,
	USBH_HC_HALTED,
	USBH_HC_NAK,
	USBH_HC_NYET,
	USBH_HC_STALL,
	USBH_HC_XACTERR,
	USBH_HC_BBLERR,
	USBH_HC_DATATGLERR
} usbh_hc_state_t;

typedef struct {
	u8  dev_addr ;          /*!< USB device address, 1~255 */
	u8  ch_num;             /*!< Host channel number, 1~15 */
	u8  ep_num;             /*!< Endpoint number, 1~15 */
	u8  ep_is_in;           /*!< Endpoint direction, 0~1 */
	u8  speed;              /*!< Host speed */
	u8  do_ping;            /*!< Enable or disable the use of the PING protocol for HS mode */
	u8  process_ping;       /*!< Execute the PING protocol for HS mode */
	u8  ep_type;            /*!< Endpoint Type */
	u16 max_packet;         /*!< Endpoint Max packet size, 0~65536 */
	u8  data_pid;           /*!< Initial data PID, 0~1 */
	u8  *xfer_buff;         /*!< Pointer to transfer buffer */
	u32 xfer_len;           /*!< Current transfer length */
	u32 xfer_count;         /*!< Partial transfer length in case of multi packet transfer */
	u8  toggle_in;          /*!< IN transfer current toggle flag, 0~1 */
	u8  toggle_out;         /*!< OUT transfer current toggle flag, 0~1 */
	u32 dma_addr;           /*!< 32 bits aligned transfer buffer address */
	u32 error_cnt;          /*!< Host channel error count.*/

	usbh_urb_state_t  urb_state;  /*!< URB state. */
	usbh_hc_state_t   hc_state;      /*!< Host Channel state. */
} usbh_hc_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbh_hal_host_init(usbh_config_t *cfg);
u8 usbh_hal_init_clock(u8 freq);
u8 usbh_hal_reset_port(void);
u8 usbh_hal_drive_vbus(u8 state);
u32 usbh_hal_get_host_speed(void);
u32 usbh_hal_get_current_frame(void);
u8 usbh_hal_hc_init(u8 ch_num, u8 ep_num, u8 dev_addr, u8 speed, u8 ep_type, u16 mps);
u8 usbh_hal_hc_start_transfer(usbh_hc_t *hc, u8 dma);
u32 usbh_hal_hc_read_interrupt(void);
u8 usbh_hal_hc_halt(u8 hc_num);
u8 usbh_hal_do_ping(u8 ch_num);
u8 usbh_hal_stop(void);

#endif /* USBD_HAL_H */

