// Copyright 2020-2025 Beken
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

#include <common/bk_include.h>
//#include <os/mem.h>
//#include <os/str.h>
#include "rwnx_config.h"
#include "rwnx_intf.h"
#include "../wpa_supplicant-2.10/bk_patch/fake_socket.h"
#include "../wpa_supplicant-2.10/bk_patch/sk_intf.h"

#if !CONFIG_FULLY_HOSTED
#include "bk_rw.h"
#endif
////#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "bk_wifi_types.h"
#include "rw_msg_rx.h"
#include "bk_wifi.h"
#include "rw_msdu.h"
#include "components/ate.h"

#if CONFIG_DEMO_TEST
#include "demos_start.h"
#endif
#include "bk_hostapd_intf.h"
#include "bk_wifi_private.h"
#include <modules/wifi.h>
#include <components/netif.h>
#include <components/event.h>
#include "rwnx_err.h"
#include "rwnx_defs.h"
#if CONFIG_TX_BUFING
#include "rw_tx_buffering.h"
#endif
#ifdef CONFIG_RWNX_SW_TXQ
#include "rwnx_tx.h"
#include "skbuff.h"
#endif
#include "rwnx_rx.h"
#include "bk_ps.h"
#include "bk_phy_adapter.h"
#include "sys_driver.h"
#include "bk_lwipif_tizenrt.h"
beken_thread_t init_thread_handle;
beken_thread_t app_thread_handle;

beken_semaphore_t app_sema = NULL;
WIFI_CORE_T g_wifi_core = { 0 };
volatile int32_t bmsg_rx_count = 0;

#if (CONFIG_SOC_BK7271)
void bmsg_null_sender(void) __SECTION(".itcm");
void bmsg_tx_handler(BUS_MSG_T *msg) __SECTION(".itcm");
static void core_thread_main(void *arg) __SECTION(".itcm");
#endif

#include "sys_types.h"

#define WIFI_INT_LOW32_BIT      (WIFI_MAC_GEN_INT_BIT | WIFI_HSU_INTERRUPT_CTRL_BIT | WIFI_MAC_WAKEUP_INT_BIT)
#define WIFI_INT_HIGH32_BIT     (WIFI_MODEM_EN | WIFI_MODEM_RC_EN | WIFI_MAC_TX_RX_TIMER_INT_BIT | WIFI_MAC_TX_RX_MISC_INT_BIT | \
                                WIFI_MAC_RX_TRIGGER_INT_BIT | WIFI_MAC_TX_TRIGGER_INT_BIT | WIFI_MAC_PORT_TRIGGER_INT_BIT)

/// os declaration interrupt status
#define WIFI_INT_DECLARATION()   uint32_t int_en_low32, int_en_high32
/// os read interrupt status and disable interrupt
#define WIFI_INT_DISABLE()      int_en_low32 = sys_drv_int_disable(WIFI_INT_LOW32_BIT); \
                                int_en_high32 = sys_drv_int_group2_disable(WIFI_INT_HIGH32_BIT)

/// os restore interrupt status
#define WIFI_INT_RESTORE()      sys_drv_int_enable(int_en_low32); \
                                sys_drv_int_group2_enable(int_en_high32)



extern void mac_sleep_check(void);
extern void mac_wakeup_and_pwr_update(BUS_MSG_T msg);
extern bool ps_close_timer;
int ps_need_pre_process_wrapper( UINT32 arg );
bool power_save_rf_sleep_check_wrapper(void);

__IRAM2 void app_set_sema(void)
{
#if !CONFIG_FULLY_HOSTED
	bk_err_t ret;
	ret = rtos_set_semaphore(&app_sema);

	(void)ret;
#endif
}

#if !CONFIG_FULLY_HOSTED
static void kmsg_bk_thread_main(void *arg)
{
	bk_err_t ret;

	mr_kmsg_init();
	while (1) {
		ret = rtos_get_semaphore(&app_sema, BEKEN_WAIT_FOREVER);
		if (kNoErr != ret) {
			RWNX_LOGE("%s timeout\r\n", __func__);
			continue;
		}

		rwnx_recv_msg();
		ke_evt_none_core_scheduler();
	}
}
#endif /* !CONFIG_FULLY_HOSTED */


void macif_rx_app_handler(void);
__IRAM2 void bmsg_rx_handler(BUS_MSG_T *msg)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	if (bmsg_rx_count > 0)
		bmsg_rx_count -= 1;
	GLOBAL_INT_RESTORE();

	macif_rx_app_handler();
}

#if !CONFIG_FULLY_HOSTED
u8 resend_num;
void bmsg_skt_tx_handler(BUS_MSG_T *msg)
{
	int ret = 0;

	if (resend_num >= 3) {
		int dummy = msg->arg;
		S_TYPE_PTR type_ptr = (S_TYPE_PTR)dummy;

		RWNX_LOGW("hapd_intf_ke_rx_handle resend 3 times, drop it!\r\n");
		if (type_ptr->type == HOSTAPD_MGMT ||
			type_ptr->type == HOSTAPD_MGMT_ROBUST)
			ke_mgmt_packet_rx(NULL, 0, type_ptr->vif_index);
		else if ((type_ptr->type == HOSTAPD_DATA) ||
                           (type_ptr->type == HOSTAPD_DATA_SPECIAL))
			ke_l2_packet_rx(NULL, 0, type_ptr->vif_index);
		os_free((void *)(msg->arg));
		resend_num = 0;
		return;
	}
	if (hapd_intf_ke_rx_handle(msg->arg)) {
		RWNX_LOGW("hapd_intf_ke_rx_handle resend\r\n");
		rtos_delay_milliseconds(10);
		ret = rtos_push_to_queue(&g_wifi_core.io_queue, msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			RWNX_LOGW("hapd_intf_ke_rx_handle resend fail!\r\n");
		resend_num++;
	} else {
		resend_num = 0;
	}
}

__IRAM2 void bmsg_tx_handler(BUS_MSG_T *msg)
{
	struct pbuf *p = (struct pbuf *)msg->arg;
	struct pbuf *q = p;
	uint8_t vif_idx = (uint8_t)msg->len;

	if ((!p) || (vif_idx == INVALID_VIF_IDX))
		return;

#if CONFIG_PM_V2
#if CONFIG_SPECIAL_TX_TYPE
	if (BMSG_SPECIAL_TX_TYPE != msg->type)
#endif
	{
		if (ps_close_timer == true) {
			td_start(vif_idx);
			rxu_cntrl_reuse_reord_timer(vif_idx);
			ps_close_timer = false;
		}
	}
#endif


#if !CONFIG_RWNX_SW_TXQ
	rwm_transfer(vif_idx, q, q->payload, q->tot_len, 0, 0
#if CONFIG_SPECIAL_TX_TYPE
		, (BMSG_SPECIAL_TX_TYPE == msg->type)
#endif
	);
#else
	rwnx_start_xmit(vif_idx, q, msg);
#endif /* CONFIG_RWNX_SW_TXQ */
}
#endif

#if !CONFIG_FULLY_HOSTED
void bmsg_tx_raw_cb_handler(BUS_MSG_T *msg)
{
	rwm_raw_frame_with_cb((uint8_t *)msg->arg, msg->len, msg->cb, msg->param);
}

int bmsg_tx_raw_cb_sender(uint8_t *buffer, int length, void *cb, void *param)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_TX_RAW_CB_TYPE;
	msg.arg = (uint32_t)buffer;
	msg.len = length;
	msg.sema = NULL;
	msg.cb = cb;
	msg.param = param;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (ret != kNoErr)
		RWNX_LOGD("bmsg_tx_sender failed\r\n");

	return ret;
}
int bmsg_tx_raw_ex_sender(wifi_raw_tx_info_t *raw_tx, void *cb, void *param)
{
	bk_err_t ret;
	BUS_MSG_T msg;
	uint16_t min_rsv_mem __maybe_unused = 0;

#if CONFIG_RWNX_SW_TXQ
	bk_wifi_get_min_rsv_mem(&min_rsv_mem);

	// if the left free heap size is less than threshold value, the new coming
	// packets will be dropped and a buffer error will be feedbacked to upper layer
	if (rtos_get_free_heap_size() < min_rsv_mem) {
		//RWNX_LOGW("tx failed mem:%d cnt:%d\n", rtos_get_free_heap_size(), skb_get_pending_cnt());
		return ERR_BUF;
	}
#endif

	msg.type = BMSG_TX_RAW_EX_TYPE;
	msg.arg = (uint32_t)raw_tx;
	msg.len = 0;
	msg.sema = NULL;
	msg.cb = cb;
	msg.param = param;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (ret != kNoErr) {
		RWNX_LOGD("bmsg_tx_sender failed\r\n");
		os_free(raw_tx);
	}

	return ret;
}


void bmsg_tx_raw_ex_handler(BUS_MSG_T *msg)
{
#if !CONFIG_RWNX_SW_TXQ
	wifi_raw_tx_info_t *raw_tx = (wifi_raw_tx_info_t *)msg->arg;
	uint8_t *pkt = raw_tx->pkt;
	uint16_t len = raw_tx->len;
	MSDU_NODE_T *node;

	node = rwm_tx_node_alloc(len, PBUF_RAW);
	if (node == NULL)
		goto exit;

	// XXX: copy
	rwm_tx_mpdu_renew(pkt, len, node->msdu_ptr);
	node->sta_idx = 0xFF;
	node->vif_idx = rwm_get_monitor_vif_idx();  // TODO: BK7236 USE the corrent vif
	node->cb = msg->cb;
	node->args = msg->param;

	rwm_transfer_raw_ex_node(node, &raw_tx->tx_cntrl);

#else /* CONFIG_RWNX_SW_TXQ */
	//RWNX_LOGI("tx_raw_ex interface\n");
	wifi_raw_tx_info_t *raw_tx = (wifi_raw_tx_info_t *)msg->arg;
	struct wifi_mac_hdr *mac_hdr = (struct wifi_mac_hdr *)raw_tx->pkt;
	uint8_t *dest_mac = (uint8_t *)mac_hdr->addr1.array;
	uint8_t *pkt = raw_tx->pkt;
	uint16_t len = raw_tx->len;
	struct sk_buff *skb = NULL;

	skb = alloc_skb(len, PBUF_RAW);

	if (!skb)
		goto exit;

	// XXX: copy
	rwm_tx_mpdu_renew(pkt, len, skb->msdu_ptr);
	if (mac_vif_mgmt_get_used_cnt()) {
		skb->sta_idx = rwm_mgmt_tx_get_staidx(skb->vif_idx, dest_mac);
		skb->vif_idx = rwm_first_vif_idx();  // TODO: BK7236 USE the corrent vif
	} else {
		skb->sta_idx = 0xFF;
		skb->vif_idx = rwm_get_monitor_vif_idx();  // TODO: BK7236 USE the corrent vif
	}
	skb->cb = msg->cb;

#if CONFIG_WIFI_TX_RAW_ENABLE
	if (raw_tx->type == WIFI_TX_RAW_DATA)
	{
		skb->args = skb->msdu_ptr;
	}
	else
#endif
	{
		skb->args = msg->param;
	}

	rwnx_start_xmit_raw_ex(skb,&raw_tx->tx_cntrl);
#endif
  exit:
    os_free(raw_tx);
}

int bmsg_tx_mgmt_sender(struct sk_buff *skb)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_TX_MGMT_SKB;
	msg.arg = (uint32_t)skb;
	msg.len = 0;
	msg.sema = NULL;
	msg.cb = NULL;
	msg.param = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (ret != kNoErr)
		RWNX_LOGW("bmsg_tx_mgmt failed\r\n");

	return ret;
}

void bmsg_tx_raw_handler(BUS_MSG_T *msg)
{
#if !CONFIG_RWNX_SW_TXQ
	uint8_t *pkt = (uint8_t *)msg->arg;
	uint16_t len = msg->len;
	MSDU_NODE_T *node;

	node = rwm_tx_node_alloc(len, PBUF_RAW);
	if (node == NULL)
		goto exit;

	// XXX: copy
	rwm_tx_mpdu_renew(pkt, len, node->msdu_ptr);
	node->sta_idx = 0xFF;
	node->vif_idx = rwm_get_monitor_vif_idx();  // TODO: BK7236 USE the corrent vif
	node->cb = msg->cb;
	node->args = msg->param;
	rwm_transfer_mgmt_node(node);

exit:
	os_free(pkt);
#else /* CONFIG_RWNX_SW_TXQ */
	struct sk_buff *skb = NULL;

	if (msg->type == BMSG_TX_MGMT_SKB) {
		skb = (struct sk_buff *)msg->arg;
	} else {
		uint8_t *pkt = (uint8_t *)msg->arg;
		uint16_t len = msg->len;
		skb = alloc_skb(len, PBUF_RAW);

		if (!skb)
			return;

		// XXX: copy
		rwm_tx_mpdu_renew(pkt, len, skb->msdu_ptr);
		skb->sta_idx = 0xFF;
		skb->vif_idx = rwm_get_monitor_vif_idx();  // TODO: BK7236 USE the corrent vif
		skb->cb = msg->cb;
		skb->args = msg->param;
		os_free(pkt);
	}
	rwnx_start_xmit_mgmt(skb);
#endif
}

#endif /* !CONFIG_FULLY_HOSTED */

void bmsg_ioctl_handler(BUS_MSG_T *msg)
{
	ke_msg_send((void *)msg->arg);
}

void bmsg_music_handler(BUS_MSG_T *msg)
{
#if (CONFIG_MP3PLAYER == 1)
	media_msg_sender((void *)msg->arg);
#endif
}

void bmsg_skt_tx_sender(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_SKT_TX_TYPE;
	msg.arg = (uint32_t)arg;
	msg.len = 0;
	msg.sema = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGW("bmsg_skt_tx_sender failed, ret=%d\r\n", ret);
}


__IRAM2 void bmsg_null_sender(void)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_NULL_TYPE;
	msg.arg = 0;
	msg.len = 0;
	msg.sema = NULL;

	if (!rtos_is_queue_empty(&g_wifi_core.io_queue))
		return;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGW("bmsg_null_sender failed, ret=%d\r\n", ret);
}

__IRAM2 void bmsg_rx_sender(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	GLOBAL_INT_DECLARATION();

	msg.type = BMSG_RX_TYPE;
	msg.arg = (uint32_t)arg;
	msg.len = 0;
	msg.sema = NULL;

	GLOBAL_INT_DISABLE();
	if (bmsg_rx_count >= 2) {
		GLOBAL_INT_RESTORE();
		return;
	}

	bmsg_rx_count += 1;
	GLOBAL_INT_RESTORE();

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret) {
		GLOBAL_INT_DISABLE();
		bmsg_rx_count -= 1;
		GLOBAL_INT_RESTORE();
		RWNX_LOGW("bmsg_rx_sender failed, ret=%d\r\n", ret);
	}
}

#if !CONFIG_FULLY_HOSTED
#if BK_SS_WIFI_DP
__IRAM2 int bmsg_tx_sender(struct pbuf *p, uint32_t vif_idx)
{
	bk_err_t ret;
	BUS_MSG_T msg;
	uint16_t min_rsv_mem __maybe_unused = 0;

	#if (CONFIG_RWNX_SW_TXQ && (!BK_SS_WIFI_DP))
	bk_wifi_get_min_rsv_mem(&min_rsv_mem);

	// if the left free heap size is less than threshold value, the new coming
	// packets will be dropped and a buffer error will be feedbacked to upper layer
	if (rtos_get_free_heap_size() < min_rsv_mem) {
		//RWNX_LOGW("tx failed mem:%d cnt:%d\n", rtos_get_free_heap_size(), skb_get_pending_cnt());
		return BK_ERR_NO_MEM;
	}
	#endif

	msg.type = BMSG_TX_TYPE;
	msg.arg = (uint32_t)p;
	msg.len = vif_idx;
	msg.sema = NULL;
	msg.cb = NULL;
	msg.param = NULL;

	pbuf_ref(p);
	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (kNoErr != ret) {
		RWNX_LOGW("bmsg_tx_sender failed, ret=%d\r\n", ret);
#if BK_SS_WIFI_DP
		bk_wlan_buf_free(p);
#else
		pbuf_free(p);
#endif
	}

	return ret;
}
#else
__IRAM2 int bmsg_tx_sender(struct pbuf *p, uint32_t vif_idx)
{
	bk_err_t ret;
	BUS_MSG_T msg;
	uint16_t min_rsv_mem __maybe_unused = 0;

	#if 0//CONFIG_RWNX_SW_TXQ
	bk_wifi_get_min_rsv_mem(&min_rsv_mem);

	// if the left free heap size is less than threshold value, the new coming
	// packets will be dropped and a buffer error will be feedbacked to upper layer
	if (rtos_get_free_heap_size() < min_rsv_mem) {
		//RWNX_LOGW("tx failed mem:%d cnt:%d\n", rtos_get_free_heap_size(), skb_get_pending_cnt());
		return BK_ERR_NO_MEM;
	}
	#endif

	msg.type = BMSG_TX_TYPE;
	msg.arg = (uint32_t)p;
	msg.len = vif_idx;
	msg.sema = NULL;
	msg.cb = NULL;
	msg.param = NULL;

	pbuf_ref(p);
	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (kNoErr != ret) {
		RWNX_LOGW("bmsg_tx_sender failed, ret=%d\r\n", ret);
#if BK_SS_WIFI_DP
		bk_wlan_buf_free(p);
#else
		pbuf_free(p);
#endif
	}

	return ret;
}
#endif
int bmsg_special_tx_sender(struct pbuf *p, uint32_t vif_idx)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_SPECIAL_TX_TYPE;
	msg.arg = (uint32_t)p;
	msg.len = vif_idx;
	msg.sema = NULL;
	msg.cb = NULL;
	msg.param = NULL;

	pbuf_ref(p);
	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (kNoErr != ret) {
		RWNX_LOGW("bmsg_tx_sender failed, ret=%d\r\n", ret);
		pbuf_ref(p);
	}

	return ret;
}

int bmsg_tx_sender_ext(struct pbuf *p, uint32_t vif_idx, void *cb, void *param)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_TX_TYPE;
	msg.arg = (uint32_t)p;
	msg.len = vif_idx;
	msg.sema = NULL;
	msg.cb = cb;
	msg.param = param;

	if (p)
		pbuf_ref(p);

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (kNoErr != ret) {
		RWNX_LOGW("bmsg_tx_sender_ext failed\r\n");
		if (p)
#if BK_SS_WIFI_DP
		    bk_wlan_buf_free(p);
#else
		    pbuf_free(p);
#endif
	}

	return ret;
}

int bmsg_special_tx_sender_ext(struct pbuf *p, uint32_t vif_idx, void *cb, void *param)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_SPECIAL_TX_TYPE;
	msg.arg = (uint32_t)p;
	msg.len = vif_idx;
	msg.sema = NULL;
	msg.cb = cb;
	msg.param = param;

	if (p)
		pbuf_ref(p);

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
	if (kNoErr != ret) {
		RWNX_LOGW("bmsg_tx_sender_ext failed\r\n");
		if (p)
#if BK_SS_WIFI_DP
		bk_wlan_buf_free(p);
#else
		pbuf_free(p);
#endif
		}

	return ret;
}
#endif

int bmsg_tx_raw_sender(uint8_t *payload, uint16_t length)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_TX_RAW_TYPE;
	msg.arg = (uint32_t)payload;
	msg.len = length;
	msg.sema = NULL;
	msg.cb = NULL;
	msg.param = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);

	if (ret != kNoErr) {
		RWNX_LOGW("bmsg_tx_raw_sender failed, ret=%d\r\n", ret);
		os_free(payload);
	}

	return ret;
}

#if (SUPPORT_LSIG_MONITOR)
void bmsg_rx_lsig_handler(BUS_MSG_T *msg)
{
	lsig_input((msg->arg & 0xFFFF0000) >> 16, msg->arg & 0xFF, msg->len);
}

void bmsg_rx_lsig(uint16_t len, uint8_t rssi)
{
	BUS_MSG_T msg;

	msg.type = BMSG_RX_LSIG;
	msg.arg = (uint32_t)((len << 16) | rssi);
	msg.len = rtos_get_time();
	msg.sema = NULL;
	rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
}
#endif

int bmsg_ioctl_sender(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_IOCTL_TYPE;
	msg.arg = (uint32_t)arg;
	msg.len = 0;
	msg.sema = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGD("bmsg_ioctl_sender_failed\r\n");
	else
		RWNX_LOGD("bmsg_ioctl_sender\r\n");

	return ret;
}

int bmsg_software_para_ioctl_sender(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_SOFTWARE_IOCTL_TYPE;
	msg.arg = (uint32_t)arg;
	msg.len = 0;
	msg.sema = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGD("bmsg_software_ioctl_sender_failed\r\n");
	else
		RWNX_LOGD("bmsg_software_ioctl_sender\r\n");

	return ret;
}

int bmsg_hardware_para_ioctl_sender(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_HARDWARE_IOCTL_TYPE;
	msg.arg = (uint32_t)arg;
	msg.len = 0;
	msg.sema = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGD("bmsg_hardware_ioctl_sender_failed\r\n");
	else
		RWNX_LOGD("bmsg_hardware_ioctl_sender\r\n");

	return ret;
}

void bmsg_music_sender(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	msg.type = BMSG_MEDIA_TYPE;
	msg.arg = (uint32_t)arg;
	msg.len = 0;
	msg.sema = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGD("bmsg_media_sender_failed\r\n");
}

#if CONFIG_TX_BUFING
void bmsg_tx_bufing_sender(uint32_t cid_and_flag)
{
	int ret;
	BUS_MSG_T msg;

	msg.type = BMSG_TX_BUFING_TYPE;
	msg.arg = cid_and_flag;
	msg.len = 0;
	msg.sema = NULL;

	ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		RWNX_LOGD("bmsg_txing_sender failed\r\n");
}


void bmsg_tx_bufing_handler(BUS_MSG_T *msg)
{
	uint32_t cid_and_flag = msg->arg;

	rwm_tx_bufing_restore_data(cid_and_flag);
}
#endif

void bmsg_tx_drop_traffic_sender(uint8_t arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	if (g_wifi_core.io_queue) {
		msg.type = BMSG_TX_DROP_TRAFFIC;
		msg.arg = (uint32_t)arg;
		msg.len = 0;
		msg.sema = NULL;

		ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			RWNX_LOGE("bmsg_tx_drop_traffic_sender failed\r\n");
	} else {
		RWNX_LOGE("g_wifi_core.io_queue null\r\n");
	}
}

void bmsg_ps_sender(uint8_t arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	if (g_wifi_core.io_queue) {
		msg.type = BMSG_STA_PS_TYPE;
		msg.arg = (uint32_t)arg;
		msg.len = 0;
		msg.sema = NULL;

		ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			RWNX_LOGE("bmsg_ps_sender failed\r\n");
	} else {
		RWNX_LOGE("g_wifi_core.io_queue null\r\n");
	}
}

void ps_send_lpo_msg(uint8_t arg,uint8_t lpo_src)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	if (g_wifi_core.io_queue) {
		msg.type = BMSG_STA_PS_TYPE;
		msg.arg = (uint32_t)arg;
		msg.len = (uint32_t)lpo_src;;
		msg.sema = NULL;

		ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			RWNX_LOGE("ps_send_msg failed\r\n");
	} else {
		RWNX_LOGE("ps_send_msg.io_queue null\r\n");
	}
}

void ps_msg_process(BUS_MSG_T *msg);
void bmsg_ps_handler(BUS_MSG_T *msg)
{
	ps_msg_process(msg);
}

#if CONFIG_PM_V2
////////7256 powersave V2.0
__IRAM2 static void core_thread_main(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;

	while (1) {
		ret = rtos_pop_from_queue(&g_wifi_core.io_queue, &msg, BEKEN_WAIT_FOREVER);

		////check if mac need wakeup and pwr update
		mac_wakeup_and_pwr_update(msg);

		if (kNoErr == ret) {
			switch (msg.type) {
			case BMSG_STA_PS_TYPE:
				RWNX_LOGD("bmsg_ioctl_handler\r\n");
				bmsg_ps_handler(&msg);
				break;

			case BMSG_RX_TYPE:
				/* ieee 802.11 rx handler */
				RWNX_LOGD("bmsg_rx_handler\r\n");
				bmsg_rx_handler(&msg);
				break;

#if !CONFIG_FULLY_HOSTED
			case BMSG_SPECIAL_TX_TYPE:
			case BMSG_TX_TYPE:
				/* ieee 802.3 data tx handler */
				RWNX_LOGD("bmsg_tx_handler\r\n");
				bmsg_tx_handler(&msg);
#if CONFIG_RWNX_SW_TXQ
				rwnx_hwq_process_all(0);
#endif
				break;
#endif

#if CONFIG_RWNX_SW_TXQ
			case BMSG_TX_DROP_TRAFFIC:
				rwnx_txq_cleanup_timer_cb(NULL);
				break;
#endif

#if !CONFIG_FULLY_HOSTED
			case BMSG_SKT_TX_TYPE:
				/* ieee 802.11 mlme frame tx handler, used by wpa_s/hapd */
				RWNX_LOGD("bmsg_skt_tx_handler\r\n");
				bmsg_skt_tx_handler(&msg);
				break;

			case BMSG_TX_MGMT_SKB:
				/* fallthru */
			case BMSG_TX_RAW_CB_TYPE:
				/* fallthru */
			case BMSG_TX_RAW_TYPE:
				/* ieee 802.11 mgmt frame tx handler */
				bmsg_tx_raw_handler(&msg);
				break;
#endif
			case BMSG_TX_RAW_EX_TYPE:
				/* extended raw tx handler */
				bmsg_tx_raw_ex_handler(&msg);
				break;
#if CONFIG_TX_BUFING
			case BMSG_TX_BUFING_TYPE:
				bmsg_tx_bufing_handler(&msg);
				break;
#endif

			case BMSG_IOCTL_TYPE:
				/* ke_msg tx handler */
				RWNX_LOGD("bmsg_ioctl_handler\r\n");
				bmsg_ioctl_handler(&msg);

				break;

			case BMSG_SOFTWARE_IOCTL_TYPE:
				RWNX_LOGD("bmsg_software_ioctl_handler\r\n");
				bmsg_ioctl_handler(&msg);
				break;

			case BMSG_HARDWARE_IOCTL_TYPE:
				RWNX_LOGD("bmsg_hardware_ioctl_handler\r\n");
				bmsg_ioctl_handler(&msg);
				break;
#if 0
			case BMSG_MEDIA_TYPE:
				bmsg_music_handler(&msg);
				break;
#endif

#if (SUPPORT_LSIG_MONITOR)
			case BMSG_RX_LSIG:
				bmsg_rx_lsig_handler(&msg);
				break;
#endif
			default:
				break;
			}

			if (msg.sema != NULL)
				rtos_set_semaphore(&msg.sema);

			ke_evt_core_scheduler();
		}
		////check if mac need sleep
		mac_sleep_check();
	}
}


#else
static void core_thread_main(void *arg)
{
	bk_err_t ret;
	BUS_MSG_T msg;
	uint8_t ke_skip = 0;

#if CONFIG_STA_PS
	uint8_t ps_flag = 0;
#endif

	while (1) {
		ret = rtos_pop_from_queue(&g_wifi_core.io_queue, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.type) {
#if CONFIG_STA_PS
			case BMSG_STA_PS_TYPE:
				if (ps_need_pre_process_wrapper(msg.arg))
					bmsg_ps_handler(&msg);
				else
					ps_flag = 1;
				break;
#endif

			case BMSG_RX_TYPE:
				/* ieee 802.11 rx handler */
				RWNX_LOGD("bmsg_rx_handler\r\n");
				bmsg_rx_handler(&msg);
				break;

#if !CONFIG_FULLY_HOSTED
			case BMSG_TX_TYPE:
				/* ieee 802.3 data tx handler */
				RWNX_LOGD("bmsg_tx_handler\r\n");
				bmsg_tx_handler(&msg);
#if CONFIG_RWNX_SW_TXQ
				rwnx_hwq_process_all(0);
#endif
				break;
#endif

#if CONFIG_RWNX_SW_TXQ
			case BMSG_TX_DROP_TRAFFIC:
				rwnx_txq_cleanup_timer_cb(NULL);
				break;
#endif

#if !CONFIG_FULLY_HOSTED
			case BMSG_SKT_TX_TYPE:
				/* ieee 802.11 mlme frame tx handler, used by wpa_s/hapd */
				RWNX_LOGD("bmsg_skt_tx_handler\r\n");
				bmsg_skt_tx_handler(&msg);
				break;

			case BMSG_TX_MGMT_SKB:
				/* fallthru */
			case BMSG_TX_RAW_CB_TYPE:
				/* fallthru */
			case BMSG_TX_RAW_TYPE:
				/* ieee 802.11 mgmt frame tx handler */
				bmsg_tx_raw_handler(&msg);
				break;
#endif

#if CONFIG_TX_BUFING
			case BMSG_TX_BUFING_TYPE:
				bmsg_tx_bufing_handler(&msg);
				break;
#endif

			case BMSG_IOCTL_TYPE:
				/* ke_msg tx handler */
				RWNX_LOGD("bmsg_ioctl_handler\r\n");
				bmsg_ioctl_handler(&msg);
				break;

			case BMSG_MEDIA_TYPE:
				ke_skip = 1;
				bmsg_music_handler(&msg);
				break;


#if (SUPPORT_LSIG_MONITOR)
			case BMSG_RX_LSIG:
				bmsg_rx_lsig_handler(&msg);
				break;
#endif
			default:
				RWNX_LOGD("unknown_msg\r\n");
				break;
			}

			if (msg.sema != NULL)
				rtos_set_semaphore(&msg.sema);

			if (!ke_skip)
				ke_evt_core_scheduler();
			else
				ke_skip = 0;
		}

#if CONFIG_STA_PS
		if (ps_flag == 1) {
			bmsg_ps_handler(&msg);
			ps_flag = 0;
		}
		power_save_rf_sleep_check_wrapper();
#endif
	}
}

#endif

void core_thread_deinit(void)
{
	if (g_wifi_core.handle) {
		rtos_delete_thread(&g_wifi_core.handle);
		g_wifi_core.handle = 0;
	}

	if (g_wifi_core.io_queue) {
		rtos_deinit_queue(&g_wifi_core.io_queue);
		g_wifi_core.io_queue = 0;
	}

	g_wifi_core.queue_item_count = 0;
}

int core_thread_init(void)
{
	bk_err_t os_ret;
	int ret = BK_OK;

	g_wifi_core.queue_item_count = CORE_QITEM_COUNT;

	os_ret = rtos_init_queue(&g_wifi_core.io_queue,
							 "core_queue",
							 sizeof(BUS_MSG_T),
							 g_wifi_core.queue_item_count);
	if (kNoErr != os_ret) {
		ret = BK_ERR_RWNX_INIT_IO_QUEUE;
		goto _core_thread_init_fail;
	}

	os_ret = rtos_create_thread(&g_wifi_core.handle,
								CONFIG_WIFI_CORE_TASK_PRIO,
								"core_thread",
								(beken_thread_function_t)core_thread_main,
								CONFIG_WIFI_CORE_TASK_STACK_SIZE,
								(beken_thread_arg_t)0);
	if (kNoErr != os_ret) {
		ret = BK_ERR_RWNX_INIT_IO_TASK;
		goto _core_thread_init_fail;
	}

	return ret;

_core_thread_init_fail:
	core_thread_deinit();
	return ret;
}

extern beken_mutex_t sr_mutex;
int rwnx_intf_deinit(void)
{
	core_thread_deinit();
	if (app_sema) {
		rtos_deinit_semaphore(&app_sema);
		app_sema = NULL;
	}

	if (app_thread_handle) {
		rtos_delete_thread(&app_thread_handle);
		app_thread_handle = NULL;
	}
	rtos_deinit_mutex(&sr_mutex);

	return BK_OK;
}

int rwnx_intf_init(void)
{
	bk_err_t os_ret;
	int ret = BK_OK;

	rtos_init_mutex(&sr_mutex);
	os_ret = rtos_init_semaphore(&app_sema, 1);
	if (os_ret != kNoErr) {
		ret = BK_ERR_RWNX_INIT_SEMA;
		goto exit;
	}

#if !CONFIG_FULLY_HOSTED
	os_ret = rtos_create_thread(&app_thread_handle,
								CONFIG_WIFI_KMSG_TASK_PRIO,
								"kmsgbk",
								(beken_thread_function_t)kmsg_bk_thread_main,
								(unsigned short)CONFIG_WIFI_KMSG_TASK_STACK_SIZE,
								(beken_thread_arg_t)0);
	if (os_ret != kNoErr) {
		ret = BK_ERR_RWNX_INIT_KMSG_TASK;
		goto exit;
	}
#endif

	ret = core_thread_init();
	if (ret != BK_OK)
		goto exit;

#if CONFIG_PM_V2 && CONFIG_STA_PS && CONFIG_SOC_BK7239XX
	if (!ate_is_enabled())
	{
		rwnxl_sleep();
		ps_env_set_ps_on(true);
		ps_env_set_cmd_ps_on(true);
	}
#endif

#if CONFIG_TX_BUFING
	rwm_msdu_init();
#endif

	return BK_OK;

exit:
	rwnx_intf_deinit();
	return ret;
}

int bmsg_is_empty(void)
{
	if (!rtos_is_queue_empty(&g_wifi_core.io_queue))
		return 0;
	else
		return 1;
}

