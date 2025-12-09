/*
 * WPA Supplicant - Layer2 packet handling example with stub functions
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file can be used as a starting point for layer2 packet implementation.
 */

#include "../utils/includes.h"

#include "../utils/common.h"
#include "../utils/eloop.h"
#include "l2_packet.h"
#include "../../bk_patch/fake_socket.h"
#include "rw_msdu.h"
#include "fhost_msg.h"

struct l2_packet_data {
	char ifname[17];
	u8 own_addr[ETH_ALEN];
	u8 vif_index;
	void (*rx_callback)(void *ctx, const u8 *src_addr,
			    const u8 *buf, size_t len);
	void *rx_callback_ctx;
	int l2_hdr; /* whether to include layer 2 (Ethernet) header data
		     * buffers */
	int fd;
};


int l2_packet_get_own_addr(struct l2_packet_data *l2, u8 *addr)
{
	os_memcpy(addr, l2->own_addr, ETH_ALEN);
	return 0;
}

struct l2_packet_head l2_packet = {
 	DL_LIST_HEAD_INIT(l2_packet.head_list),
	.num = 0,
};

struct l2_packet_head *get_l2_packet_entity(void)
{
	return &l2_packet;
}

static void l2_packet_tx_cb(struct ieee80211_tx_cb *cb, bool success)
{
 	struct l2_packet_node *item, *n;
 	struct l2_packet_head *l2_packet_head = get_l2_packet_entity();

	if(dl_list_empty(&l2_packet_head->head_list)) {
		return;
	}

	//if semaphore is null, do nothing
	if (cb->sema == NULL)
	{
		WPA_LOGI("l2_packet_tx_cb, semaphore is null\n");
		return;
	}

 	rtos_lock_mutex(&l2_packet_head->l2_mutex);
 	dl_list_for_each_safe(item, n, &l2_packet_head->head_list, struct l2_packet_node, list)
 	{
 		if(item->cb.l2_tag == cb->l2_tag)
 		{
			cb->result = success ? 0 : -1;
 			rtos_set_semaphore(&(item->cb.sema));
 			break;
 		}
 	}
	rtos_unlock_mutex(&l2_packet_head->l2_mutex);
}

/**
 * send L2 packet
 * return: <0 if error, else 0.
 */
int __l2_packet_send(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto,
		   const u8 *buf, size_t len, int sync)
{
	int data_len, ret = 0;
	struct l2_ethhdr *eth;
    S_TYPE_PTR type_ptr;
	unsigned char *data_buf = NULL;
	struct l2_packet_head *l2_packet_head = get_l2_packet_entity();
	struct l2_packet_node *item, *n;
	struct l2_packet_node *node = NULL; //control block

	if (sync) {
		node = os_zalloc(sizeof(struct l2_packet_node));
		if(!node) {
			ret = -1;
			goto send_exit;
		}
		(l2_packet_head->num)++;
	}
	type_ptr = os_zalloc(sizeof(S_TYPE_ST));
    if (!type_ptr) {
		ret = -1;
        if (type_ptr) {
            os_free(type_ptr);
        }
		goto send_exit;
    }

	data_len = len + sizeof(struct l2_ethhdr);
	data_buf = os_zalloc(data_len);
	if(data_buf == NULL){
		ret = -1;
		goto send_exit;
	}

	eth = (struct l2_ethhdr *) data_buf;
	os_memcpy(eth->h_dest, dst_addr, ETH_ALEN);
	os_memcpy(eth->h_source, l2->own_addr, ETH_ALEN);
	eth->h_proto = host_to_be16(proto);
	os_memcpy(eth+1, buf, len);

	extern bool wpa_tkip_key_set_flag;
	if (!wpa_tkip_key_set_flag) {
		type_ptr->type = HOSTAPD_DATA;
	} else {
		type_ptr->type = HOSTAPD_DATA_SPECIAL;
		wpa_tkip_key_set_flag = false;
	}

	type_ptr->vif_index = l2->vif_index;

	if (sync) {
		node->cb.result = -1;
		node->cb.l2_tag = l2_packet_head->num;
		ret = rtos_init_semaphore(&(node->cb.sema), 1);
		type_ptr->cb = l2_packet_tx_cb;
		type_ptr->args = (void *)(&node->cb);
		rtos_lock_mutex(&l2_packet_head->l2_mutex);
		dl_list_add_tail(&l2_packet_head->head_list, &node->list);
		rtos_unlock_mutex(&l2_packet_head->l2_mutex);
	}

	fsocket_send(l2->fd, data_buf, data_len, type_ptr);
	if (sync) {
		ret = rtos_get_semaphore(&(node->cb.sema), 1*1000 /*BEKEN_NEVER_TIMEOUT*/);
		if (ret != kNoErr) {
			WPA_LOGE("%s: send timeout\r\n", __func__);
		} else {
			ret = node->cb.result;
			if (ret < 0)
				WPA_LOGE("%s: send failed\r\n", __func__);
		}
		rtos_lock_mutex(&l2_packet_head->l2_mutex);
		dl_list_for_each_safe(item, n, &l2_packet_head->head_list, struct l2_packet_node, list)
		{
			if(item->cb.l2_tag == node->cb.l2_tag)
			{
				dl_list_del(&item->list);
				rtos_deinit_semaphore(&(item->cb.sema));
				break;
			}
		}
		rtos_unlock_mutex(&l2_packet_head->l2_mutex);
		if(node) {
			os_free(node);
			node = 0;
		}
	}
send_exit:
	if(data_buf){
		os_free(data_buf);
	}

	return ret;
}


int l2_packet_send(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto,
		   const u8 *buf, size_t len)
{
	return __l2_packet_send(l2, dst_addr, proto, buf, len, 0);
}


int l2_packet_send_sync(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto,
		   const u8 *buf, size_t len)
{
	return __l2_packet_send(l2, dst_addr, proto, buf, len, 1);
}


static void l2_packet_receive(int sock, void *eloop_ctx, void *sock_ctx)
{
#if CONFIG_WPA2_ENTERPRISE || defined(CONFIG_P2P)
#define TMP_BUF_LEN     1500
#else
#define TMP_BUF_LEN     512
#endif

	int len;
	unsigned char *buf;
	struct l2_ethhdr *hdr;
	struct l2_packet_data *l2 = eloop_ctx;

	buf = os_malloc(TMP_BUF_LEN);
	BK_ASSERT(buf);

	len = fsocket_recv(sock, buf, TMP_BUF_LEN, 0);
	if (len < 0) {
		wpa_printf(MSG_ERROR, "fsocket_recv_len_err");
		goto recv_exit;
	}

	hdr = (struct l2_ethhdr *) buf;
	l2->rx_callback(l2->rx_callback_ctx,
						hdr->h_source,
						buf + sizeof(struct l2_ethhdr),
						len - sizeof(struct l2_ethhdr));

recv_exit:
	if(buf)
		os_free(buf);

	return;
}

extern UINT8 rwm_mgmt_vif_mac2idx(void *mac);
struct l2_packet_data * l2_packet_init(
	const char *ifname, const u8 *own_addr, unsigned short protocol,
	void (*rx_callback)(void *ctx, const u8 *src_addr,
			    const u8 *buf, size_t len),
	void *rx_callback_ctx, int l2_hdr)
{
	struct l2_packet_data *l2;

	l2 = os_zalloc(sizeof(struct l2_packet_data));
	if (l2 == NULL)
		return NULL;
	os_strlcpy(l2->ifname, ifname, sizeof(l2->ifname));
	os_memcpy(l2->own_addr, own_addr, ETH_ALEN);

	l2->vif_index = rwm_mgmt_vif_mac2idx((void *)own_addr);
	if (l2->vif_index == 0xff)
		WPA_LOGE("not found vif_index in l2_packet_init\r\n");

	l2->rx_callback = rx_callback;
	l2->rx_callback_ctx = rx_callback_ctx;
	l2->l2_hdr = l2_hdr;

	/*
	 * TODO: open connection for receiving frames
	 */
	protocol += l2->vif_index;
	l2->fd = fsocket_init(PF_PACKET, SOCK_RAW, protocol);
	if (l2->fd >= 0)
		eloop_register_read_sock(l2->fd, l2_packet_receive, l2, NULL);

	return l2;
}


struct l2_packet_data * l2_packet_init_bridge(
	const char *br_ifname, const char *ifname, const u8 *own_addr,
	unsigned short protocol,
	void (*rx_callback)(void *ctx, const u8 *src_addr,
			    const u8 *buf, size_t len),
	void *rx_callback_ctx, int l2_hdr)
{
	return l2_packet_init(br_ifname, own_addr, protocol, rx_callback,
			      rx_callback_ctx, l2_hdr);
}


void l2_packet_deinit(struct l2_packet_data *l2)
{
	if (l2 == NULL)
		return;

	if (l2->fd >= 0) {
		eloop_unregister_read_sock(l2->fd);

		/* TODO: close connection */
		fsocket_close(l2->fd);
	}

	os_free(l2);
	l2 = NULL;
}


int l2_packet_get_ip_addr(struct l2_packet_data *l2, char *buf, size_t len)
{
	/* TODO: get interface IP address */
	return -1;
}


void l2_packet_notify_auth_start(struct l2_packet_data *l2)
{
	/* This function can be left empty */
}


int l2_packet_set_packet_filter(struct l2_packet_data *l2,
				enum l2_packet_filter_type type)
{
	return -1;
}
