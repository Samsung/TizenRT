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

#ifndef __FREERTOS_INTFS_H_
#define __FREERTOS_INTFS_H_

typedef struct _driver_priv {
	int drv_registered;

	_mutex hw_init_mutex;
	_mutex h2c_fwcmd_mutex;
#if defined(CONFIG_CONCURRENT_MODE) || defined(CONFIG_DUALMAC_CONCURRENT)
	//global variable
	_mutex setch_mutex;
	_mutex setbw_mutex;
#endif
} drv_priv, *pdrv_priv;

struct net_device *rtw_init_netdev(_adapter *padapter);
void rtw_os_indicate_disconnect(_adapter *adapter);

#ifdef CONFIG_PROC_DEBUG
void rtw_proc_init_one(struct net_device *dev);
void rtw_proc_remove_one(struct net_device *dev);
#else
#define rtw_proc_init_one(dev)
#define rtw_proc_remove_one(dev)
#endif

extern int rtw_set_wpa_ie(_adapter *padapter, char *pie, unsigned short ielen);
extern void rtw_os_indicate_connect(_adapter *adapter);
extern void indicate_wx_custom_event(_adapter *padapter, char *msg);
extern int rtw_init_netdev_name(struct net_device *pnetdev, const char *ifname);
extern void netdev_lwip_post_sleep_processing(void);
extern void wireless_send_event(struct net_device *dev, unsigned int cmd, union iwreq_data *wrqu, char *extra);

#ifdef CONFIG_CONCURRENT_MODE
struct _io_ops;
_adapter *rtw_drv_if2_init(_adapter *primary_padapter, char *name, void (*set_intf_ops)(struct _io_ops *pops));
void rtw_drv_if2_free(_adapter *pbuddy_padapter);
#endif

#if defined(CONFIG_ISR_THREAD_MODE_POLLING) || defined(CONFIG_ISR_THREAD_MODE_INTERRUPT)
extern thread_return rtw_interrupt_thread(thread_context context);
#endif

#ifdef CONFIG_RECV_TASKLET_THREAD
extern thread_return rtw_recv_tasklet(thread_context context);
#endif

#ifdef CONFIG_XMIT_TASKLET_THREAD
extern thread_return rtw_xmit_tasklet(thread_context context);
#endif

extern struct net_device *rtw_drv_probe(struct net_device *parent_dev, u32 mode); //Wlan driver init entry
extern void rtw_drv_entry(void);
extern void rtw_drv_halt(void);
extern int rtw_dev_remove(struct net_device *pnetdev);
extern int rtw_ioctl(struct net_device *dev, struct iwreq *rq, int cmd);

#if defined(CONFIG_LX_HCI)
u32 lextra_bus_dma_Interrupt(void *data);
#endif

#endif //__FREERTOS_INTFS_H_
