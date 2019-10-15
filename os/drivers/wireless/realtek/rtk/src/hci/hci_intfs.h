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
#ifndef _HCI_INTFS_H_
#define _HCI_INTFS_H_

#include <autoconf.h>

struct host_ctrl_intf_ops {
	struct dvobj_priv *(*dvobj_init)(void);
	void (*dvobj_deinit)(struct dvobj_priv *dvobj);
	void (*dvobj_request_irq)(struct dvobj_priv *dvobj);
	void (*dvobj_free_irq)(struct dvobj_priv *dvobj);
};

extern struct dvobj_priv *hci_dvobj_init(void);
extern void hci_dvobj_deinit(struct dvobj_priv *dvobj);
extern void hci_dvobj_request_irq(struct dvobj_priv *dvobj);
extern void hci_dvobj_free_irq(struct dvobj_priv *dvobj);

#if defined(CONFIG_GSPI_HCI)
#define hci_bus_intf_type RTW_GSPI
#define hci_set_intf_ops spi_set_intf_ops
#define hci_intf_start rtw_hal_enable_interrupt
#define hci_intf_stop rtw_hal_disable_interrupt
extern s32 gspi_dvobj_xmit_mgnt(_adapter *padapter, struct xmit_frame *pmgntframe);
extern s32 gspi_dvobj_xmit_data(_adapter *padapter, struct xmit_frame *pxmitframe);
#endif

#if defined(CONFIG_SDIO_HCI)
#define hci_bus_intf_type RTW_SDIO
#define hci_set_intf_ops sdio_set_intf_ops
#define hci_intf_start rtw_hal_enable_interrupt
#define hci_intf_stop rtw_hal_disable_interrupt
extern s32 sdio_dvobj_xmit_mgnt(_adapter *padapter, struct xmit_frame *pmgntframe);
extern s32 sdio_dvobj_xmit_data(_adapter *padapter, struct xmit_frame *pxmitframe);
#endif

#if defined(CONFIG_USB_HCI)
#define hci_bus_intf_type RTW_USB
#define hci_set_intf_ops usb_set_intf_ops
struct dvobj_priv *usb_dvobj_init(void);
void usb_dvobj_deinit(struct dvobj_priv *dvobj);
void usb_intf_start(_adapter *padapter);
void usb_intf_stop(_adapter *padapter);

#define hci_intf_start usb_intf_start
#define hci_intf_stop usb_intf_stop
#endif

#if defined(CONFIG_LX_HCI)
#define hci_bus_intf_type RTW_LXBUS
#define hci_set_intf_ops lxbus_set_intf_ops
#define hci_intf_start rtw_hal_enable_interrupt
#define hci_intf_stop hci_lxbus_intf_stop
void hci_lxbus_intf_stop(_adapter *padapter);
u32 lextra_bus_dma_Interrupt(void *data);
#endif

#if defined(CONFIG_AXI_HCI)
#define hci_bus_intf_type RTW_AXI
#define hci_set_intf_ops axi_set_intf_ops
#define hci_intf_start rtw_hal_enable_interrupt
#define hci_intf_stop hci_axi_intf_stop
void hci_axi_intf_stop(_adapter *padapter);
#endif

#endif //_HCI_INTFS_H_
