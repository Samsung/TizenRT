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

#ifndef __OSDEP_INTF_H_
#define __OSDEP_INTF_H_

typedef	struct	net_device 	*_nic_hdl;

struct pkt_buff {
	_list list;
	u32 len;
	unsigned char *data;
};

#include "freertos/wrapper.h"
#include "freertos/freertos_intfs.h"

#ifdef CONFIG_CONCURRENT_MODE
_adapter *rtw_drv_if2_init(_adapter *primary_padapter);
void rtw_drv_if2_free(_adapter *pbuddy_padapter);
#endif

struct pkt_buff *rtw_alloc_pktbuf(unsigned int size);
void rtw_free_pktbuf(struct pkt_buff *skb);

extern int rtw_xmit_entry(_pkt *pkt, _nic_hdl pnetdev);
#endif	//_OSDEP_INTF_H_