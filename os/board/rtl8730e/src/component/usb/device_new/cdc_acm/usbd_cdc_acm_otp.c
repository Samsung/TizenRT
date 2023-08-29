/**
  ******************************************************************************
  * @file    usbd_cdc_acm_otp.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the OTP functionalities of the USB CDC Class
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

#include "ameba.h"
#include "usbd_cdc_acm.h"
#include "usbd.h"

#ifdef CONFIG_USB_USE_OTP_DESC

/* Private defines -----------------------------------------------------------*/

#define USB_OTP_OFFSET_START		0x2A0U
#define USB_OTP_OFFSET_END			0x2E0U
#define USB_OTP_LEN					68U // USB_OTP_OFFSET_END - USB_OTP_OFFSET_START, aligned to 4 bytes
#define USB_OTP_OFFSET_VID			0x2A0U
#define USB_OTP_OFFSET_PID			0x2A2U
#define USB_OTP_OFFSET_STR			0x2A7U
#define USB_OTP_STR_LEN				(USB_OTP_OFFSET_END - USB_OTP_OFFSET_STR + 1U)

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static u8 usbd_otp_map[OTP_LMAP_LEN];

/* Private functions ---------------------------------------------------------*/

static u8 usbd_otp_get_strlen(u8 *buf)
{
	u8 len = 0U;

	while (*buf != '\0') {
		len++;
		buf++;
	}

	return len;
}

/**
  * @brief  Get string descriptor from ASCII string buffer
  * @param  desc - String descriptor in UNICODE
  * @param  buf - String buffer in ASCII
  * @param  len - String descriptor length
  * @retval void
  */
static void usbd_otp_get_str_desc(u8 *desc, u8 *buf, u8 *len)
{
	u8 idx = 0U;

	if (buf != NULL) {
		*len = usbd_otp_get_strlen(buf) * 2U + 2U;
		desc[idx++] = *len;
		desc[idx++] = USB_DESC_TYPE_STRING;

		while (*buf != '\0') {
			desc[idx++] = *buf++;
			desc[idx++] =  0U;
		}
	}
}

/* Exported functions --------------------------------------------------------*/

u8 usbd_otp_init(usbd_otp_t *otp)
{
	u32 status;
	u32 reg;
	u8 len;
	u8 sn_len;
	u8 mfg_len;
	u8 prod_len;
	u8 buf[USBD_OTP_MAX_STR_LEN];

	otp->otp_sn = 0U;
	otp->otp_param = 0U;

	otp->mfg_str = rtw_zmalloc(USBD_CDC_MAX_STR_LEN);
	if (otp->mfg_str == NULL) {
		return HAL_ERR_MEM;
	}

	otp->prod_str = rtw_zmalloc(USBD_CDC_MAX_STR_LEN);
	if (otp->prod_str == NULL) {
		rtw_free(otp->mfg_str);
		otp->mfg_str = NULL;
		return HAL_ERR_MEM;
	}

	otp->sn_str = rtw_zmalloc(USBD_CDC_MAX_STR_LEN);
	if (otp->sn_str == NULL) {
		rtw_free(otp->mfg_str);
		otp->mfg_str = NULL;
		rtw_free(otp->prod_str);
		otp->prod_str = NULL;
		return HAL_ERR_MEM;
	}

	reg = HAL_READ32(USB_REG_BASE, USB_OTG_ADDON_REG_AUTOLOAD_CTRL);
	otp->self_powered = ((reg & USB_OTG_ADDON_REG_AUTOLOAD_CTRL_BIT_SELF_POWER_EN) == 0) ? 0 : 1;
	otp->remote_wakeup_en = ((reg & USB_OTG_ADDON_REG_AUTOLOAD_CTRL_BIT_REMOTE_WAKEUP) == 0) ? 0 : 1;
	if (((reg & USB_OTG_ADDON_REG_AUTOLOAD_CTRL_BIT_AUTOLOAD_DESC_EN) != 0U) ||
		((reg & USB_OTG_ADDON_REG_AUTOLOAD_CTRL_BIT_SQNUM_ROM) == 0U)) {
		status = OTP_LogicalMap_Read(usbd_otp_map, USB_OTP_OFFSET_START, USB_OTP_LEN);
		if (status != _SUCCESS) {
			return HAL_ERR_HW;
		}

		if ((reg & USB_OTG_ADDON_REG_AUTOLOAD_CTRL_BIT_SQNUM_ROM) == 0U) {
			// Mfg string
			mfg_len = usbd_otp_map[USB_OTP_OFFSET_STR];
			if ((mfg_len < 2) || (mfg_len > USB_OTP_STR_LEN)) {
				return HAL_ERR_PARA;
			}
			// Product string
			prod_len = usbd_otp_map[USB_OTP_OFFSET_STR + mfg_len];
			if ((prod_len < 2) || ((u16)(mfg_len + prod_len) > USB_OTP_STR_LEN)) {
				return HAL_ERR_PARA;
			}
			// SN string
			sn_len = usbd_otp_map[USB_OTP_OFFSET_STR + mfg_len + prod_len];
			if ((sn_len < 2U) || ((u16)(sn_len + mfg_len + prod_len) > (u16)USB_OTP_STR_LEN)) {
				return HAL_ERR_PARA;
			}
			rtw_memcpy((void *)buf, (void *)&usbd_otp_map[USB_OTP_OFFSET_STR + mfg_len + prod_len + 2], sn_len - 2);
			buf[sn_len - 2] = '\0';
			usbd_otp_get_str_desc(otp->sn_str, buf, &otp->sn_str_len);
			otp->otp_sn = 1U;
		}

		if ((reg & USB_OTG_ADDON_REG_AUTOLOAD_CTRL_BIT_AUTOLOAD_DESC_EN) != 0U) {
			// VID
			otp->vid = usbd_otp_map[USB_OTP_OFFSET_VID] | ((u16)usbd_otp_map[USB_OTP_OFFSET_VID + 1] << 8);
			// PID
			otp->pid = usbd_otp_map[USB_OTP_OFFSET_PID] | ((u16)usbd_otp_map[USB_OTP_OFFSET_PID + 1] << 8);
			// Mfg string
			len = usbd_otp_map[USB_OTP_OFFSET_STR];
			if ((len < 2) || (len > USB_OTP_STR_LEN)) {
				return HAL_ERR_PARA;
			}
			rtw_memcpy((void *)buf, (void *)&usbd_otp_map[USB_OTP_OFFSET_STR + 2], len - 2);
			buf[len - 2] = '\0';
			usbd_otp_get_str_desc(otp->mfg_str, buf, &otp->mfg_str_len);
			// Product string
			len = usbd_otp_map[USB_OTP_OFFSET_STR + otp->mfg_str_len];
			if ((len < 2) || ((u16)(otp->mfg_str_len + len) > USB_OTP_STR_LEN)) {
				return HAL_ERR_PARA;
			}
			rtw_memcpy((void *)buf, (void *)&usbd_otp_map[USB_OTP_OFFSET_STR + otp->mfg_str_len + 2], len - 2);
			buf[len - 2] = '\0';
			usbd_otp_get_str_desc(otp->prod_str, buf, &otp->prod_str_len);
			otp->otp_param = 1U;
		}
	}

	return HAL_OK;
}

#endif // CONFIG_USB_USE_OTP_DESC

