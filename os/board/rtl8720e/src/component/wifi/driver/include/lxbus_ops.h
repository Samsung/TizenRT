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


#if defined(CONFIG_RTL8711B)

extern u32 rtl8711b_init_desc_ring(_adapter *padapter);
extern u32 rtl8711b_free_desc_ring(_adapter *padapter);
extern void rtl8711b_reset_desc_ring(_adapter *padapter);
extern void EnableDMA8711b(PADAPTER padapter);
extern void EnableInterrupt8711b(PADAPTER padapter);
extern void DisableDMA8711b(PADAPTER padapter);
extern void DisableInterrupt8711b(PADAPTER padapter);
extern s32 InterruptHandle8711b(PADAPTER Adapter);
extern void lxbus_set_intf_ops(struct _io_ops *pops);
extern void rtl8711b_xmit_tasklet(void *priv);
extern void rtl8711b_recv_tasklet(void *priv);
extern void rtl8711b_prepare_bcn_tasklet(void *priv);
extern void rtl8711b_tx_int_handler(_adapter *padapter);
extern void InitInterrupt8711b(PADAPTER padapter);
extern VOID UpdateInterruptMask8711b(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);

#ifdef CONFIG_WOWLAN
extern void ClearInterrupt8711b(PADAPTER padapter);
extern void DisableInterruptButCpwm28711b(PADAPTER padapter);
#endif

extern void ClearWlPmcInterrupt8711b(PADAPTER padapter);
extern BOOLEAN InterruptRecognized8711b(PADAPTER Adapter);

#elif defined(CONFIG_RTL8721D)
extern u32 rtl8721d_init_desc_ring(_adapter *padapter);
extern u32 rtl8721d_free_desc_ring(_adapter *padapter);
extern void rtl8721d_reset_desc_ring(_adapter *padapter);
extern void EnableDMA8721d(PADAPTER padapter);
extern void EnableInterrupt8721d(PADAPTER padapter);
extern void DisableDMA8721d(PADAPTER padapter);
extern void DisableInterrupt8721d(PADAPTER padapter);
extern s32 InterruptHandle8721d(PADAPTER Adapter);
extern void lxbus_set_intf_ops(struct _io_ops *pops);
extern void rtl8721d_xmit_tasklet(void *priv);
extern void rtl8721d_recv_tasklet(void *priv);
extern void rtl8721d_prepare_bcn_tasklet(void *priv);
extern void rtl8721d_tx_int_handler(_adapter *padapter);
extern void InitInterrupt8721d(PADAPTER padapter);
extern VOID UpdateInterruptMask8721d(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);

#ifdef CONFIG_WOWLAN
extern void ClearInterrupt8721d(PADAPTER padapter);
extern void DisableInterruptButCpwm28721d(PADAPTER padapter);
#endif

extern void ClearWlPmcInterrupt8721d(PADAPTER padapter);
extern BOOLEAN InterruptRecognized8721d(PADAPTER Adapter);

#elif defined(CONFIG_RTL8730E)
extern u32 rtl8730e_init_desc_ring(_adapter *padapter);
extern u32 rtl8730e_free_desc_ring(_adapter *padapter);
extern void rtl8730e_reset_desc_ring(_adapter *padapter);
extern void EnableDMA8730e(PADAPTER padapter);
extern void EnableInterrupt8730e(PADAPTER padapter);
extern void DisableDMA8730e(PADAPTER padapter);
extern void DisableInterrupt8730e(PADAPTER padapter);
extern s32 InterruptHandle8730e(PADAPTER Adapter);
extern void lxbus_set_intf_ops(struct _io_ops *pops);
extern void rtl8730e_xmit_tasklet(void *priv);
extern void rtl8730e_recv_tasklet(void *priv);
extern void rtl8730e_prepare_bcn_tasklet(void *priv);
extern void rtl8730e_tx_int_handler(_adapter *padapter);
extern void InitInterrupt8730e(PADAPTER padapter);
extern VOID UpdateInterruptMask8730e(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);
extern void ClearWlPmcInterrupt8730e(PADAPTER padapter);
extern BOOLEAN InterruptRecognized8730e(PADAPTER Adapter);

#elif defined(CONFIG_RTL8730A)
extern u32 rtl8730a_init_desc_ring(_adapter *padapter);
extern u32 rtl8730a_free_desc_ring(_adapter *padapter);
extern void rtl8730a_reset_desc_ring(_adapter *padapter);
extern void EnableDMA8730a(PADAPTER padapter);
extern void EnableInterrupt8730a(PADAPTER padapter);
extern void DisableDMA8730a(PADAPTER padapter);
extern void DisableInterrupt8730a(PADAPTER padapter);
extern s32 InterruptHandle8730a(PADAPTER Adapter);
extern void lxbus_set_intf_ops(struct _io_ops *pops);
extern void rtl8730a_xmit_tasklet(void *priv);
extern void rtl8730a_recv_tasklet(void *priv);
extern void rtl8730a_prepare_bcn_tasklet(void *priv);
extern void rtl8730a_tx_int_handler(_adapter *padapter);
extern void InitInterrupt8730a(PADAPTER padapter);
extern VOID UpdateInterruptMask8730a(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);
extern void ClearWlPmcInterrupt8730a(PADAPTER padapter);
extern BOOLEAN InterruptRecognized8730a(PADAPTER Adapter);

#elif defined(CONFIG_RTL8720E)
extern u32 rtl8720e_init_desc_ring(_adapter *padapter);
extern u32 rtl8720e_free_desc_ring(_adapter *padapter);
extern void rtl8720e_reset_desc_ring(_adapter *padapter);
extern void EnableDMA8720e(PADAPTER padapter);
extern void EnableInterrupt8720e(PADAPTER padapter);
extern void DisableDMA8720e(PADAPTER padapter);
extern void DisableInterrupt8720e(PADAPTER padapter);
extern s32 InterruptHandle8720e(PADAPTER Adapter);
extern void lxbus_set_intf_ops(struct _io_ops *pops);
extern void rtl8720e_xmit_tasklet(void *priv);
extern void rtl8720e_recv_tasklet(void *priv);
extern void rtl8720e_prepare_bcn_tasklet(void *priv);
extern void rtl8720e_tx_int_handler(_adapter *padapter);
extern void InitInterrupt8720e(PADAPTER padapter);
extern VOID UpdateInterruptMask8720e(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);
extern void ClearWlPmcInterrupt8720e(PADAPTER padapter);
extern BOOLEAN InterruptRecognized8720e(PADAPTER Adapter);

#endif

#endif // !__LXBUS_OPS_H__

