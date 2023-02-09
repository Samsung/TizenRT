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

#ifndef __FREERTOS_INTFS_H_
#define __FREERTOS_INTFS_H_

struct net_device *rtw_init_netdev(_adapter *padapter);
void rtw_os_indicate_disconnect(_adapter *adapter);

extern int rtw_set_wpa_ie(_adapter *padapter, char *pie, unsigned short ielen);
extern void rtw_os_indicate_connect(_adapter *adapter);
extern void netdev_lwip_post_sleep_processing(void);
extern void wireless_send_event(struct net_device *dev, unsigned int event, u8 *buf, int buf_len, int flags);

#ifdef CONFIG_CONCURRENT_MODE
_adapter *rtw_drv_if2_init(_adapter *primary_padapter);
void rtw_if2_deinit(_adapter *padapter);
#endif

extern thread_return rtw_interrupt_thread(thread_context context);

extern thread_return rtw_recv_tasklet(thread_context context);
extern thread_return rtw_xmit_tasklet(thread_context context);
#if defined (CONFIG_WOWLAN) && defined (CONFIG_FW_DRIVER_COEXIST) && CONFIG_FW_DRIVER_COEXIST
extern thread_return rtw_driver_resume_thread(thread_context context);
#endif
extern int rtw_dev_remove(struct net_device *pnetdev);
extern int netdev_close(struct net_device *pnetdev);
extern void rtw_indicate_wx_assoc_event(_adapter *padapter);

#if defined(CONFIG_LX_HCI)
u32 lextra_bus_dma_Interrupt(void *data);
#endif

#endif	//__FREERTOS_INTFS_H_

