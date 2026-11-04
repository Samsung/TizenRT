// Copyright 2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common/bk_include.h"
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#ifdef CONFIG_IPV6
#include <lwip/ethip6.h>
#endif
#include <bk_ethernetif.h>
#include <stdio.h>
#include <string.h>
#include "lwip/netif/etharp.h"
#include "bk_drv_model.h"
#include <os/mem.h>
#include <os/os.h>
#include "bk_uart.h"
#include "bk_wifi.h"
#include "bk_private/bk_wifi.h"
#include "rwnx_rx.h"
#include "bk_lwipif_tizenrt.h"
#include "wifi_config.h"
extern size_t rtos_get_free_heap_size_by_index(int heap_index);
struct pbuf *bk_wlan_buf_alloc(uint32_t len)
{
#ifdef CONFIG_QUICK_TRACK
	struct pbuf *p = mem_malloc(sizeof(struct pbuf) + len + 96 + MEM_ALIGNMENT);
	if (!p)
		return 0;
	memset(p, 0, sizeof(struct pbuf) + len + 96 + MEM_ALIGNMENT);
	p->next = 0;
	p->len = len;
	p->tot_len = len;
	p->ref = 1;
	p->type = PBUF_RAM;
	p->flags = 0;
#ifdef CONFIG_NET_NETMGR_ZEROCOPY
    p->payload = LWIP_MEM_ALIGN((uint8_t*)p + sizeof(struct pbuf) + 96);//DP_TODO: check 96
#else
    p->payload = LWIP_MEM_ALIGN((uint8_t*)p + sizeof(struct pbuf) + 96);//DP_TODO: check 96
#endif
#else
	struct pbuf *p = os_zalloc_sram(len + sizeof(struct pbuf) + 96);
	if (!p)
		return 0;
	p->next = 0;
	p->len = len;
	p->tot_len = len;
#ifdef CONFIG_NET_NETMGR_ZEROCOPY
    p->payload = (uint8_t*)p + sizeof(struct pbuf) + 96;//DP_TODO: check 96
#else
    p->payload = (uint8_t*)p + sizeof(struct pbuf) + 96;//DP_TODO: check 96
#endif
#endif
    return p;
}

void bk_wlan_buf_free(struct pbuf *p)
{
#ifdef CONFIG_QUICK_TRACK
	pbuf_free(p);
#else
	os_free(p);
#endif
}

err_t bk_wlan_txdata_send(uint8_t * data,uint16_t len, uint32_t vif_idx)
{
    struct pbuf* p = NULL;
    err_t ret = ERR_OK;

    size_t free_heap_size = rtos_get_free_heap_size_by_index(SRAM_INDEX);
    if (free_heap_size < WIFI_CFG_DFT_MIN_RSV_MEM) {
        return ERR_MEM;
    }
#ifdef CONFIG_QUICK_TRACK
#ifdef CONFIG_NET_NETMGR_ZEROCOPY
    struct pbuf *src = (struct pbuf *)data;
    struct pbuf *q = NULL;
    uint16_t offset = 0;

    p = bk_wlan_buf_alloc(src->tot_len);
    if(p)
    {
        for (q = src; q != NULL; q = q->next) {
            memcpy((uint8_t *)p->payload + offset, q->payload, q->len);
            offset += q->len;
        }
    }
    else
    {
        return ERR_MEM;
    }
#else
    p = bk_wlan_buf_alloc(len);
    if(p)
    {
        memcpy(p->payload,data,len);
    }
    else
    {
        return ERR_MEM;
    }
#endif
#else
#ifdef CONFIG_NET_NETMGR_ZEROCOPY
#if BK_SS_WIFI_DP
    p = bk_wlan_buf_alloc(((struct pbuf*)data)->len);   
    if(p)
    {
        //TODO: Use dma copy instead of cpu copy
        memcpy(p->payload,((struct pbuf*)data)->payload,((struct pbuf*)data)->len);
    }
    else
    {
        return ERR_MEM;
    }
#else
    p = (struct pbuf*)data;
#endif
#else
    p = bk_wlan_buf_alloc(len);
    if(p)
    {
        memcpy(p->payload,data,len);
    }
    else
    {
        return ERR_MEM;
    }
#endif
#endif

    ret = bmsg_tx_sender(p, (uint32_t)vif_idx);
#ifdef CONFIG_QUICK_TRACK
    pbuf_free(p);
#endif

    if(ERR_OK != ret)
    {
        ret = ERR_TIMEOUT;
    }

    return ret;
}