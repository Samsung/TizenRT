#include <common/bk_include.h>
#include "../wpa_supplicant-2.10/src/utils/common.h"
#include "fake_socket.h"
#include <os/mem.h>
#include "bk_wifi_private.h"
#include "../src/utils/wpa_debug.h"

/*
 * RWIP use ke_sk_xx to recv/send mesages, while non-RWIP use fsocket_xxxx.
 */

static SOCKET_ENTITY socket_entity = {
	DL_LIST_HEAD_INIT(socket_entity.sk_head)
};

SOCKET_ENTITY *get_fsocket_entity(void)
{
	return &socket_entity;
}

extern void bmsg_skt_tx_sender(void *arg);

/**
 * append @buf to socket->sk_rx_msg list
 */
int ke_sk_send(SOCKET sk, const unsigned char *buf, int len, int flag)
{
	int ret = 0;
	unsigned char *data_buf;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg;

	WPA_LOGD("ke_tx:%d,buf:0x%x, len:%d\r\n", sk, buf, len);
	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto tx_exit;
	}

	sk_msg = (SOCKET_MSG *)os_malloc(sizeof(SOCKET_MSG));
	if(0 == sk_msg)
	{
		goto tx_exit;
	}
	data_buf = (unsigned char *)os_malloc(len);
	if(0 == data_buf)
	{
		goto malloc_buf_exit;
	}
	ret = len;
	sk_msg->len = len;
	sk_msg->msg = data_buf;

	os_memcpy(sk_msg->msg, buf, len);

	dl_list_add_tail(&element->sk_rx_msg, &sk_msg->data);
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;

malloc_buf_exit:
	os_free(sk_msg);
	sk_msg = NULL;

tx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
}

/**
 * recv buf from socket->sk_tx_msg list.
 */
int ke_sk_recv(SOCKET sk, const unsigned char *buf, int len, int flag)
{
	int count;
	int ret = 0;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg, *tmp;

	WPA_LOGD("ke_rx:%d,buf:0x%x, len:%d\r\n", sk, buf, len);
	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto rx_exit;
	}
	dl_list_for_each_safe(sk_msg, tmp, &element->sk_tx_msg, SOCKET_MSG, data)
	{
		if (len != 0 && buf != NULL) {
			count = MIN(sk_msg->len, len);

			BK_ASSERT(count); /* BK_ASSERT VERIFIED */
			BK_ASSERT(sk_msg); /* BK_ASSERT VERIFIED */
			WPA_LOGD("r1:%d,buf:0x%x, len:%d\r\n", sk, buf, count);
			os_memcpy((void *)buf, (void *)sk_msg->msg, count);

			ret = count;
		} else {
			WPA_LOGI("drop sk_tx_msg\r\n");
		}

		os_free(sk_msg->msg);
		sk_msg->msg = 0;
		sk_msg->len = 0;

		dl_list_del(&sk_msg->data);
		os_free(sk_msg);
		sk_msg = 0;

		break;
	}

rx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
}

int ke_sk_recv_peek_next_payload_size(SOCKET sk)
{
	int ret = 0;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg, *tmp;

	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto rx_exit;
	}

	dl_list_for_each_safe(sk_msg, tmp, &element->sk_tx_msg, SOCKET_MSG, data)
	{
		ret = sk_msg->len;
		break;
	}

rx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
}

int ke_sk_send_peek_next_payload_size(SOCKET sk)
{
	int ret = 0;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg, *tmp;

	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto rx_exit;
	}

	dl_list_for_each_safe(sk_msg, tmp, &element->sk_rx_msg, SOCKET_MSG, data)
	{
		ret = sk_msg->len;
		break;
	}

rx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
}

BK_SOCKET *sk_get_sk_element(SOCKET sk)
{
	BK_SOCKET *tmp;
	BK_SOCKET *bk_sk_ptr;

	dl_list_for_each_safe(bk_sk_ptr, tmp, &socket_entity.sk_head, BK_SOCKET, sk_element)
	{
		if(sk == bk_sk_ptr->sk)
		{
			return bk_sk_ptr;
		}
	}

	return 0;
}

SOCKET fsocket_init(int af, int type, int protocol)
{
	SOCKET sk;
	BK_SOCKET *sk_ptr;

	rtos_lock_mutex(&socket_entity.fs_mutex);
	sk = af + type + protocol;
	if (0 != sk_get_sk_element(sk))
	{
	    WPA_LOGE("%s sk=%d exist\n", __FUNCTION__, sk);
	    rtos_unlock_mutex(&socket_entity.fs_mutex);
	    return sk;
	}

	sk_ptr = (BK_SOCKET *)os_malloc(sizeof(BK_SOCKET));
	if(0 == sk_ptr)
	{
		WPA_LOGE("fsocket_init oom\r\n");
		rtos_unlock_mutex(&socket_entity.fs_mutex);
		return 0;
	}

	sk_ptr->sk = sk;

	dl_list_init(&sk_ptr->sk_rx_msg);
	dl_list_init(&sk_ptr->sk_tx_msg);

	dl_list_add(&socket_entity.sk_head, &sk_ptr->sk_element);
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	WPA_LOGD("create fsocket_init:%d\r\n", sk);
	return sk;
}

/*
 *
 */
int fsocket_send(SOCKET sk, const unsigned char *buf, int len, S_TYPE_PTR type)
{
	int ret = 0;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg;
	unsigned char *data_buf;

	WPA_LOGD("hapd_tx:%d,buf:0x%x, len:%d\r\n", sk, buf, len);
	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto tx_exit;
	}

	sk_msg = (SOCKET_MSG *)os_malloc(sizeof(SOCKET_MSG));
	if(0 == sk_msg)
	{
		goto tx_exit;
	}

	data_buf = (unsigned char *)os_malloc(len);
	if(0 == data_buf)
	{
		goto malloc_buf_exit;
	}
	ret = len;
	sk_msg->len = len;
	sk_msg->msg = data_buf;

	os_memcpy(sk_msg->msg, buf, len);

	dl_list_add_tail(&element->sk_tx_msg, &sk_msg->data);
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	bmsg_skt_tx_sender(type);

	return ret;

malloc_buf_exit:
	os_free(sk_msg);

tx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
}

int fsocket_recv(SOCKET sk, const unsigned char *buf, int len, int flag)
{
	int count;
	int ret = 0;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg, *tmp;

	WPA_LOGD("hapd_rx:%d,buf:0x%x, len:%d\r\n", sk, buf, len);
	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto rx_exit;
	}

	dl_list_for_each_safe(sk_msg, tmp, &element->sk_rx_msg, SOCKET_MSG, data)
	{
		if(sk_msg->len > len)
		{
			WPA_LOGW("recv_buf_small:%d:%d\r\n", sk_msg->len, len);
		}

		count = MIN(sk_msg->len, len);

		BK_ASSERT(count); /* BK_ASSERT VERIFIED */
		BK_ASSERT(sk_msg); /* BK_ASSERT VERIFIED */

		os_memcpy((void *)buf, (void *)sk_msg->msg, count);
		ret = count;

		os_free(sk_msg->msg);
		sk_msg->msg = 0;
		sk_msg->len = 0;

		dl_list_del(&sk_msg->data);
		os_free(sk_msg);
		sk_msg = 0;

		break;
	}

rx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
;
}

void fsocket_close(SOCKET sk)
{
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg, *tmp;

	WPA_LOGD("close_sk:%d\r\n", sk);
	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		rtos_unlock_mutex(&socket_entity.fs_mutex);
		return;
	}

	dl_list_for_each_safe(sk_msg, tmp, &element->sk_tx_msg, SOCKET_MSG, data)
	{
		dl_list_del(&sk_msg->data);

		if(sk_msg->msg)
		{
			os_free(sk_msg->msg);
			sk_msg->msg = 0;
			sk_msg->len = 0;
		}

		os_free(sk_msg);
	}

	dl_list_for_each_safe(sk_msg, tmp, &element->sk_rx_msg, SOCKET_MSG, data)
	{
		dl_list_del(&sk_msg->data);

		if(sk_msg->msg)
		{
			os_free(sk_msg->msg);
			sk_msg->msg = 0;
			sk_msg->len = 0;
		}

		os_free(sk_msg);
	}

	dl_list_del(&element->sk_element);

	os_free(element);
	element = 0;

	rtos_unlock_mutex(&socket_entity.fs_mutex);
}

// same as ke_sk_send_peek_next_payload_size
int fsocket_peek_recv_next_payload_size(SOCKET sk)
{
	int ret = 0;
	BK_SOCKET *element;
	SOCKET_MSG *sk_msg, *tmp;

	rtos_lock_mutex(&socket_entity.fs_mutex);
	element = sk_get_sk_element(sk);
	if(0 == element)
	{
		goto rx_exit;
	}

	dl_list_for_each_safe(sk_msg, tmp, &element->sk_rx_msg, SOCKET_MSG, data)
	{
		ret = sk_msg->len;
		break;
	}

rx_exit:
	rtos_unlock_mutex(&socket_entity.fs_mutex);

	return ret;
}
// eof

