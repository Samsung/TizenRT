/*
 * WPA Supplicant - Layer2 packet handling with NuttX packet sockets
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "includes.h"

#include "common.h"
#include "eloop.h"
#include "l2_packet.h"
#include <netpacket/packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <wpa_supplicant/wpa_supplicant_i.h>

struct l2_packet_data {
	char ifname[17];
	u8 own_addr[ETH_ALEN];
	void (*rx_callback)(void *ctx, const u8 *src_addr, const u8 *buf, size_t len);
	void *rx_callback_ctx;
	int l2_hdr;					/* whether to include layer 2 (Ethernet) header data
								 * buffers */
	int ifindex;
};

struct wpa_eapol_key_work {
	struct l2_packet_data *l2;
	u8 *addr;
	u8 *buf;
	u16 len;

};

extern int eth_send_eapol(const u8 *src, const u8 *dst, const u8 *buf, u16 len, u16 proto);

int l2_packet_get_own_addr(struct l2_packet_data *l2, u8 *addr)
{
	os_memcpy(addr, l2->own_addr, ETH_ALEN);
	return 0;
}

int l2_packet_send(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto, const u8 *buf, size_t len)
{
	if (l2 == NULL) {
		return -1;
	}
	if (l2->l2_hdr) {
		wpa_printf(MSG_ERROR, "l2_packet_send - received with header ");
	} else {
		eth_send_eapol(l2->own_addr, dst_addr, buf, len, proto);
	}
	return 0;
}

static void wpa_receive_eapol_cb(struct wpa_supplicant *wpa_s, struct wpa_eapol_key_work *cwork)
{
	wpa_printf(MSG_DEBUG, "Post EAPOL Rx");

	cwork->l2->rx_callback(wpa_s->l2->rx_callback_ctx, cwork->addr, cwork->buf, cwork->len);

	os_free(cwork->addr);
	os_free(cwork->buf);
	os_free(cwork);
	return;
}

void l2_packet_receive(void *ctx, const u8 *src_addr, u8 *ptr, u16 len)
{
	struct wpa_supplicant *wpa_s = ctx;
	u8 *buf;
	struct wpa_eapol_key_work *cwork;

	wpa_printf(MSG_DEBUG, "%s: recvd EAPOL src=" MACSTR " len=%d", __func__, MAC2STR(src_addr), (int)len);

	buf = os_zalloc(len);
	if (buf == NULL) {
		wpa_printf(MSG_ERROR, "EAPOL Rx: Buffer allocation failed for len = %d", len);
		return;
	}
	os_memcpy(buf, ptr, len);

	cwork = os_zalloc(sizeof(*cwork));
	if (cwork == NULL) {
		wpa_printf(MSG_ERROR, "EAPOL Rx: cwork allocation failed");
		os_free(buf);
		return;
	}

	cwork->addr = os_zalloc(ETH_ALEN);
	if (cwork->addr == NULL) {
		wpa_printf(MSG_ERROR, "EAPOL Rx: MAC addr allocation failed");
		os_free(cwork);
		os_free(buf);
		return;
	}

	os_memcpy(cwork->addr, src_addr, ETH_ALEN);
	cwork->l2 = wpa_s->l2;
	cwork->buf = buf;
	cwork->len = len;

	eloop_register_timeout(0, 0, (eloop_timeout_handler) wpa_receive_eapol_cb, wpa_s, cwork);
	/* Interrupt select/poll to process the event immediately */
	kill(eloop_get_pid(), SIGUSR2);

	return;
}

struct l2_packet_data *l2_packet_init(const char *ifname, const u8 *own_addr, unsigned short protocol, void (*rx_callback)(void *ctx, const u8 *src_addr, const u8 *buf, size_t len), void *rx_callback_ctx, int l2_hdr)
{
	struct l2_packet_data *l2;

	l2 = os_zalloc(sizeof(struct l2_packet_data));
	if (l2 == NULL) {
		return NULL;
	}
	os_strlcpy(l2->ifname, ifname, sizeof(l2->ifname));
	l2->rx_callback = rx_callback;
	l2->rx_callback_ctx = rx_callback_ctx;
	l2->l2_hdr = l2_hdr;
	os_memcpy(l2->own_addr, own_addr, ETH_ALEN);

	return l2;
}

struct l2_packet_data *l2_packet_init_bridge(const char *br_ifname, const char *ifname, const u8 *own_addr, unsigned short protocol, void (*rx_callback)(void *ctx, const u8 *src_addr, const u8 *buf, size_t len), void *rx_callback_ctx, int l2_hdr)
{
	return l2_packet_init(br_ifname, own_addr, protocol, rx_callback, rx_callback_ctx, l2_hdr);
}

void l2_packet_deinit(struct l2_packet_data *l2)
{
	if (l2 == NULL) {
		return;
	}

	os_free(l2);
}

int l2_packet_get_ip_addr(struct l2_packet_data *l2, char *buf, size_t len)
{
	int s;
	struct ifreq ifr;
	struct sockaddr_in *saddr;
	size_t res;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		wpa_printf(MSG_ERROR, "%s: socket: %s", __func__, strerror(errno));
		return -1;
	}
	os_memset(&ifr, 0, sizeof(ifr));
	os_strlcpy(ifr.ifr_name, l2->ifname, sizeof(ifr.ifr_name));
	if (ioctl(s, SIOCGIFADDR, (unsigned long)&ifr) < 0) {
		if (errno != EADDRNOTAVAIL) {
			wpa_printf(MSG_ERROR, "%s: ioctl[SIOCGIFADDR]: %s", __func__, strerror(errno));
		}
		close(s);
		return -1;
	}
	close(s);
	saddr = aliasing_hide_typecast(&ifr.ifr_addr, struct sockaddr_in);
	if (saddr->sin_family != AF_INET) {
		return -1;
	}
	res = os_strlcpy(buf, inet_ntoa(saddr->sin_addr), len);
	if (res >= len) {
		return -1;
	}
	return 0;
}

void l2_packet_notify_auth_start(struct l2_packet_data *l2)
{
	/* This function can be left empty */
}

int l2_packet_set_packet_filter(struct l2_packet_data *l2, enum l2_packet_filter_type type)
{
	return -1;
}
