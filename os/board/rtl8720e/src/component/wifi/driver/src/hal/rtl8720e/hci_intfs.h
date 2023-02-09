/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef _HCI_INTFS_H_
#define _HCI_INTFS_H_

#include <autoconf.h>

#define 	HAL_INTERFACE_CMD_LEN			0
#define 	HAL_INTERFACE_CMD_STATUS_LEN   	0
#define 	HAL_INTERFACE_OVERHEAD			(HAL_INTERFACE_CMD_LEN+HAL_INTERFACE_CMD_STATUS_LEN)

#define	GSPI_STATUS_LEN		0

extern void hci_intf_init(_adapter *padapter);
extern void hci_intf_deinit(_adapter *padapter);
extern void hci_intf_request_irq(_adapter *padapter);
extern void hci_intf_free_irq(_adapter *padapter);


#define hci_bus_intf_type RTW_AXI
#define hci_intf_stop	hci_axi_intf_stop
void hci_axi_intf_stop(_adapter *padapter);

#endif //_HCI_INTFS_H_

