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

#ifndef __OSDEP_INTF_H_
#define __OSDEP_INTF_H_

typedef struct net_device *_nic_hdl;
struct iw_request_info {
	u16 cmd;   /* Wireless Extension command */
	u16 flags; /* More to come ;-) */
};
typedef int (*iw_handler)(struct net_device *dev, struct iw_request_info *info,
						  union iwreq_data *wrqu, char *extra);

struct pkt_buff {
	_list list;
	u32 len;
	unsigned char *data;
};

#include "tizenrt/wrapper.h"
#include "tizenrt/tizenrt_intfs.h"
#include "tizenrt/tizenrt_xmit.h"
#include "tizenrt/tizenrt_recv.h"

#ifdef CONFIG_CONCURRENT_MODE
struct _io_ops;
_adapter *rtw_drv_if2_init(_adapter *primary_padapter, char *name, void (*set_intf_ops)(struct _io_ops *pops));
void rtw_drv_if2_free(_adapter *pbuddy_padapter);
#endif

struct net_device *rtw_alloc_etherdev_with_old_priv(int sizeof_priv, void *old_priv);
struct net_device *rtw_alloc_etherdev(int sizeof_priv);
void rtw_free_netdev(struct net_device *netdev);

int rtw_netif_queue_stopped(struct net_device *pnetdev);
void rtw_netif_wake_queue(struct net_device *pnetdev);
void rtw_netif_start_queue(struct net_device *pnetdev);
void rtw_netif_stop_queue(struct net_device *pnetdev);

struct pkt_buff *rtw_alloc_pktbuf(unsigned int size);
void rtw_free_pktbuf(struct pkt_buff *skb);

#endif //_OSDEP_INTF_H_
