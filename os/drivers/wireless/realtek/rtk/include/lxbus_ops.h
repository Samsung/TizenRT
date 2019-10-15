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
#ifndef __LXBUS_OPS_H__
#define __LXBUS_OPS_H__

#if defined(CONFIG_RTL8195A)
//extern u32 TxbdRxbdInitRtl8195a(PADAPTER Adapter);
//extern u32 TxbdRxbdResetRtl8195a(PADAPTER Adapter);

extern VOID InitLxDmaRtl8195a(_adapter *Adapter);
extern u32 rtl8195a_init_desc_ring(_adapter *padapter);
extern u32 rtl8195a_free_desc_ring(_adapter *padapter);
extern void rtl8195a_reset_desc_ring(_adapter *padapter);
extern void EnableDMA8195a(PADAPTER padapter);
extern void EnableInterrupt8195a(PADAPTER padapter);
extern void DisableDMA8195a(PADAPTER padapter);
extern void DisableInterrupt8195a(PADAPTER padapter);
extern s32 InterruptHandle8195a(PADAPTER Adapter);
extern void lxbus_set_intf_ops(struct _io_ops *pops);
extern void rtl8195a_xmit_tasklet(void *priv);
extern void rtl8195a_recv_tasklet(void *priv);
extern void rtl8195a_prepare_bcn_tasklet(void *priv);
extern void rtl8195a_tx_int_handler(_adapter *padapter);
extern void InitInterrupt8195a(PADAPTER padapter);
extern VOID UpdateInterruptMask8195a(PADAPTER Adapter, u32 *pAddMSRB, u32 *pRemoveMSR);

#ifdef CONFIG_WOWLAN
extern void ClearInterrupt8195a(PADAPTER padapter);
#endif

extern void ClearWlPmcInterrupt8195a(PADAPTER padapter);
extern BOOLEAN InterruptRecognized8195a(PADAPTER Adapter);
#elif defined(CONFIG_RTL8711B)

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
#endif

#endif // !__LXBUS_OPS_H__
