/**
  ******************************************************************************
  * The header file for usbd_cdc_acm_otp.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_CDC_ACM_OTP_H
#define USBD_CDC_ACM_OTP_H

#ifdef CONFIG_USB_USE_OTP_DESC

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

#define USBD_OTP_MAX_STR_LEN	58U
#define USBD_CDC_MAX_STR_LEN	(USBD_OTP_MAX_STR_LEN * 2 + 2)

/* Exported types ------------------------------------------------------------*/

typedef struct {
	u8 otp_sn;
	u8 otp_param;
	u16 vid;
	u16 pid;
	u8 self_powered;
	u8 remote_wakeup_en;
	u8 mfg_str_len;
	u8 prod_str_len;
	u8 sn_str_len;
	u8 *mfg_str;
	u8 *prod_str;
	u8 *sn_str;
} usbd_otp_t;

/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

u8 usbd_otp_init(usbd_otp_t *otp);

#endif // CONFIG_USB_USE_OTP_DESC

#endif  /* USBD_CDC_ACM_OTP_H */

