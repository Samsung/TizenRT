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
#ifndef __LXBUS_OPS_H__
#define __LXBUS_OPS_H__

extern const u32 tx_buf_addr_table[16];

extern void wifi_hal_desc_ring_init(_adapter *padapter);
extern u32 wifi_hal_desc_ring_free(_adapter *padapter);
extern void wifi_hal_desc_ring_reset(_adapter *padapter);
extern void wifi_xmit_tasklet(void *priv);
extern void wifi_recv_tasklet(void *priv);
extern void wifi_hal_tx_int_handler(_adapter *padapter);
extern void wifi_hal_interrupt_enable(PADAPTER padapter);
extern void wifi_hal_interrupt_disable(PADAPTER padapter);
extern s32 wifi_hal_interrupt_handle(PADAPTER Adapter);
extern void wifi_hal_interrupt_init(PADAPTER padapter);

#if defined(CONFIG_RTL8721D)
extern VOID UpdateInterruptMask8721d(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);
#ifdef CONFIG_WOWLAN
extern void DisableInterruptButCpwm28721d(PADAPTER padapter);
#endif
#endif

#endif // !__LXBUS_OPS_H__

