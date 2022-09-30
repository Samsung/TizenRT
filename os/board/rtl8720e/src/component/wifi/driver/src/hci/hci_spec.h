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
#ifndef __HCI_SPEC_H__
#define __HCI_SPEC_H__

/* Add for halbb, It may be wrong if g6 driver add other type for BIT6, */
enum rtw_hci_type {
	RTW_HCI_PCIE = BIT0,
	RTW_HCI_USB = BIT1,
	RTW_HCI_SDIO = BIT2,
	RTW_HCI_GSPI = BIT3,
	RTW_HCI_AXI = BIT6,
	RTW_HCI_MAX,
};

#if defined(CONFIG_GSPI_HCI)
#include "gspi/gspi_spec.h"

// SPI Header Files

#define GSPI_CMD_LEN		4
#define HAL_INTERFACE_CMD_LEN			GSPI_CMD_LEN
#define GSPI_STATUS_LEN		8
#define HAL_INTERFACE_CMD_STATUS_LEN   	GSPI_STATUS_LEN
#define HAL_INTERFACE_OVERHEAD			(HAL_INTERFACE_CMD_LEN+HAL_INTERFACE_OVERHEAD)
//reserve tx headroom in case of softap forwarding unicase packet
#define RX_RESERV_HEADROOM		(SKB_WLAN_TX_EXTRA_LEN>RX_DRIVER_INFO+RXDESC_SIZE)?(SKB_WLAN_TX_EXTRA_LEN-RX_DRIVER_INFO-RXDESC_SIZE):0
typedef struct gspi_data {
	//u8  func_number;

	//u8  tx_block_mode;
	//u8  rx_block_mode;
	u16 block_transfer_len; //u32 block_transfer_len;

#if defined(PLATFORM_FREERTOS)
	_mutex 	spi_mutex;
#endif
} GSPI_DATA, *PGSPI_DATA;

#define INTF_DATA GSPI_DATA

//extern void spi_set_intf_ops(struct _io_ops *pops);
extern void spi_int_hdl(PADAPTER padapter);
#define rtw_hci_interrupt_handler(__adapter) spi_int_hdl(__adapter)

#elif defined(CONFIG_SDIO_HCI)
#include "sdio/sdio_spec.h"

#define GSPI_CMD_LEN		0
#define HAL_INTERFACE_CMD_LEN			GSPI_CMD_LEN
#define GSPI_STATUS_LEN		8
#define HAL_INTERFACE_CMD_STATUS_LEN   	GSPI_STATUS_LEN
#define HAL_INTERFACE_OVERHEAD			(HAL_INTERFACE_CMD_LEN+HAL_INTERFACE_OVERHEAD)
#define RX_RESERV_HEADROOM		(SKB_WLAN_TX_EXTRA_LEN>RX_DRIVER_INFO+RXDESC_SIZE)?(SKB_WLAN_TX_EXTRA_LEN-RX_DRIVER_INFO-RXDESC_SIZE):0

typedef struct gspi_data {
	//u8  func_number;

	//u8  tx_block_mode;
	//u8  rx_block_mode;
	u16 block_transfer_len; //u32 block_transfer_len;

	struct sdio_func	 *func;

#if defined(PLATFORM_FREERTOS)
	_mutex 	spi_mutex;
#endif
} GSPI_DATA, *PGSPI_DATA;
typedef struct rtw_if_operations {
	int (*read)(struct dvobj_priv *d, int addr, void *buf,
				size_t len, bool fixed);
	int (*write)(struct dvobj_priv *d, int addr, void *buf,
				 size_t len, bool fixed);
} RTW_IF_OPS, *PRTW_IF_OPS;

#define INTF_DATA GSPI_DATA
#define INTF_OPS	 PRTW_IF_OPS

//extern void spi_set_intf_ops(struct _io_ops *pops);
extern void spi_int_hdl(PADAPTER padapter);
#define rtw_hci_interrupt_handler(__adapter) spi_int_hdl(__adapter)
#elif defined(CONFIG_LX_HCI)
#define GSPI_CMD_LEN		0
#define GSPI_STATUS_LEN		0
#include "lxbus/lxbus_spec.h"

#elif defined(CONFIG_AXI_HCI)
#define GSPI_CMD_LEN		0
#define GSPI_STATUS_LEN		0
#include "axi/axi_spec.h"

#endif // interface define

#endif //__HCI_SPEC_H__

