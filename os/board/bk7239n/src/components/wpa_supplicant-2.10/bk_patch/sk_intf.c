#include <common/bk_include.h>
#include "sk_intf.h"
#include "../hostapd/main_none.h"
#include "../src/utils/eloop.h"

//#include "bk_wifi_types.h"
//#include "bk_rw.h"

extern uint32_t wpa_hostapd_queue_poll(uint32_t param);

SOCKET mgmt_get_socket_num(u8 vif_idx)
{
	return (SK_INTF_MGMT_SOCKET_NUM + vif_idx);
}

SOCKET ioctl_get_socket_num(u8 vif_idx)
{
	return (SK_INTF_IOCTL_SOCKET_NUM + vif_idx);
}

SOCKET data_get_socket_num(u8 vif_idx)
{
	return (SK_INTF_DATA_SOCKET_NUM + vif_idx);
}

void handle_dummy_read(int sock, void *eloop_ctx, void *sock_ctx)
{
    int len;
    unsigned char *buf;

#ifndef TMP_BUF_LEN
#define TMP_BUF_LEN    (512)
#endif

    buf = os_malloc(TMP_BUF_LEN);
    if(!buf) {
        WPA_LOGE("dummy_read no mem\r\n");
        goto dummy_exit;
    }

    len = fsocket_recv(sock, buf, TMP_BUF_LEN, 0);
    if (len < 0)
    {
        wpa_printf(MSG_ERROR, "recv: %s", strerror(errno));
        goto dummy_exit;
    }

dummy_exit:
    if(buf)
    {
        os_free(buf);
    }

    return;
}

/*
 * send to wpad or hapd.
 */
int ke_mgmt_packet_tx(unsigned char *buf, int len, int flag)
{
	int ret, poll_flag = 0;
	SOCKET sk = mgmt_get_socket_num(flag);

	ret = ke_sk_send(sk, buf, len, flag);
	if(ret)
	{
    	poll_flag = wpa_hostapd_queue_poll((uint32_t)flag);
	}

	if(poll_flag)
	{
		handle_dummy_read(sk, NULL, NULL);
	}

	return ret;
}

int ke_mgmt_packet_rx(unsigned char *buf, int len, int flag)
{
	SOCKET sk = mgmt_get_socket_num(flag);

	return ke_sk_recv(sk, buf, len, flag);
}

int ke_mgmt_peek_txed_next_payload_size(int flag)
{
	SOCKET sk = mgmt_get_socket_num(flag);

	return ke_sk_send_peek_next_payload_size(sk);
}

int ke_mgmt_peek_rxed_next_payload_size(int flag)
{
	SOCKET sk = mgmt_get_socket_num(flag);

	return ke_sk_recv_peek_next_payload_size(sk);
}

int ke_l2_packet_tx(unsigned char *buf, int len, int flag)
{
	int ret, poll_flag = 0;
	SOCKET sk = data_get_socket_num(flag);

	ret = ke_sk_send(sk, buf, len, flag);
	if(0 != ret)
	{
    	poll_flag = wpa_hostapd_queue_poll((uint32_t)flag);
	}

	if(poll_flag)
	{
		handle_dummy_read(sk, NULL, NULL);
	}

	return ret;
}

int ke_l2_packet_rx(unsigned char *buf, int len, int flag)
{
	SOCKET sk = data_get_socket_num(flag);

	return ke_sk_recv(sk, buf, len, flag);
}

int ke_data_peek_txed_next_payload_size(int flag)
{
	SOCKET sk = data_get_socket_num(flag);

	return ke_sk_send_peek_next_payload_size(sk);
}

int ke_data_peek_rxed_next_payload_size(int flag)
{
	SOCKET sk = data_get_socket_num(flag);

	return ke_sk_recv_peek_next_payload_size(sk);
}

int ws_mgmt_peek_rxed_next_payload_size(int flag)
{
	SOCKET sk = mgmt_get_socket_num(flag);

	return fsocket_peek_recv_next_payload_size(sk);
}

int ws_get_mgmt_packet(unsigned char *buf, int len, int flag)
{
	SOCKET sk;

	sk = mgmt_get_socket_num(flag);

	return fsocket_recv(sk, buf, len, flag);
}

int ws_data_peek_rxed_next_payload_size(int flag)
{
	SOCKET sk = data_get_socket_num(flag);

	return fsocket_peek_recv_next_payload_size(sk);
}

int ws_get_data_packet(unsigned char *buf, int len, int flag)
{
	SOCKET sk;

	sk = data_get_socket_num(flag);

	return fsocket_recv(sk, buf, len, flag);
}

// eof

