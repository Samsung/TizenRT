/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include "usb_io_realtek.h"
#include <drv_types.h>

void usbhost_rtl8723d_initialize(void);
int usbhost_ctrl_req(void *priv, unsigned char bdir_in, unsigned int wvalue, unsigned char *buf, unsigned int len);
int usbhost_get_speed(void *priv);
int usbhost_get_in_ep_info(void *priv, unsigned char *ep_addr_array);
int usbhost_get_out_ep_info(void *priv, unsigned char *ep_addr_array);
unsigned char usbhost_get_bulk_in_pipe(void *priv, unsigned char en_addr);
unsigned char usbhost_get_bulk_out_pipe(void *priv, unsigned char en_addr);
int usbhost_bulk_in(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usbhost_asynch_t callback, void *arg);
int usbhost_bulk_out(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usbhost_asynch_t callback, void *arg);
int usbhost_cancel_bulk_in(void *priv);
int usbhost_cancel_bulk_out(void *priv);

extern void *g_rtk_wifi_usb;
extern unsigned char g_rtk_wifi_connect;

static void rtw_usb_probe(void)
{
	nvdbg("rtw_usb_probe -------> \n");

	if (!g_rtk_wifi_connect)
		usbhost_rtl8723d_initialize();

	while (1) {

		if (g_rtk_wifi_connect)
			break;
		else
			usleep(10000);
	}

	nvdbg("rtw_usb_probe <------- \n");
}

static int rtw_usb_ctrl_req(void *priv, unsigned char bdir_in, unsigned int wvalue, unsigned char *buf, unsigned int len)
{
	int ret = 0;
	if (usbhost_ctrl_req(priv, bdir_in, wvalue, buf, len) == OK) {
		ret = 1;
	}
	return ret;
}

static int rtw_usb_get_speed_info(void *priv)
{
	u8 usb_speed = 0;
	u8 ret = RTW_USB_SPEED_UNKNOWN;

	usb_speed = usbhost_get_speed(priv);

	switch (usb_speed) {
	case 1:
		nvdbg("Low Speed Case \n");
		ret = RTW_USB_SPEED_1_1;
		break;
	case 2:
		nvdbg("full speed Case \n");
		ret = RTW_USB_SPEED_1_1;
		break;
	case 3:
		nvdbg("high speed Case \n");
		ret = RTW_USB_SPEED_2;
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

static int rtw_usb_get_in_ep_info(void *priv, unsigned char *ep_addr_array)
{
	return usbhost_get_in_ep_info(priv, ep_addr_array);
}

static int rtw_usb_get_out_ep_info(void *priv, unsigned char *ep_addr_array)
{
	return usbhost_get_out_ep_info(priv, ep_addr_array);
}

static unsigned char rtw_usb_get_bulk_in_pipe(void *priv, unsigned char ep_addr)
{
	return usbhost_get_bulk_in_pipe(priv, ep_addr);
}

static unsigned char rtw_usb_get_bulk_out_pipe(void *priv, unsigned char ep_addr)
{
	return usbhost_get_bulk_out_pipe(priv, ep_addr);
}

static int rtw_usb_bulk_in(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usb_complete callback, void *arg)
{
	return usbhost_bulk_in(priv, pipe, buf, len, callback, arg);
}

static int rtw_usb_bulk_out(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usb_complete callback, void *arg)
{
	return usbhost_bulk_out(priv, pipe, buf, len, callback, arg);
}

static int rtw_usb_cancel_bulk_in(void *priv)
{
	return usbhost_cancel_bulk_in(priv);
}

static int rtw_usb_cancel_bulk_out(void *priv)
{
	return usbhost_cancel_bulk_out(priv);
}

extern USB_BUS_OPS rtw_usb_bus_ops = {
	rtw_usb_probe,
	NULL,
	rtw_usb_ctrl_req,
	rtw_usb_get_speed_info,
	rtw_usb_get_in_ep_info,
	rtw_usb_get_out_ep_info,
	rtw_usb_get_bulk_in_pipe,
	rtw_usb_get_bulk_out_pipe,
	rtw_usb_bulk_in,
	rtw_usb_bulk_out,
	rtw_usb_cancel_bulk_in,
	rtw_usb_cancel_bulk_out
};
