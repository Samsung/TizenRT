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

#ifndef _USB_IO_REALTEK_H
#define _USB_IO_REALTEK_H

#include "tizenrt_service.h"
#include "tinyara/usb/usb.h"
#include "tinyara/usb/usbhost.h"

typedef struct USB_DATA_S {
	void *usb_intf;
} USB_DATA, *PUSB_DATA;

typedef void (*usb_complete)(void *arg, ssize_t result);

typedef struct _USB_BUS_OPS {
	void (*usb_probe)();
	int (*usb_remove)();
	int (*usb_ctrl_req)(void *priv, unsigned char bdir_in, unsigned int wvalue, unsigned char *buf, unsigned int len);
	int (*usb_get_speed_info)(void *priv);
	int (*usb_get_in_ep_info)(void *priv, unsigned char *pipe_num_array);
	int (*usb_get_out_ep_info)(void *priv, unsigned char *pipe_num_array);
	unsigned char (*usb_get_bulk_in_pipe)(void *priv, unsigned char ep_addr);
	unsigned char (*usb_get_bulk_out_pipe)(void *priv, unsigned char ep_addr);
	int (*usb_bulk_in)(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usb_complete callback, void *arg);
	int (*usb_bulk_out)(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usb_complete callback, void *arg);
	int (*usb_cancel_bulk_in)(void *priv);
	int (*usb_cancel_bulk_out)(void *priv);
} USB_BUS_OPS;

extern USB_BUS_OPS rtw_usb_bus_ops;
#endif
