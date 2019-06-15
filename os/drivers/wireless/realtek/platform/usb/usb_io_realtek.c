#include "usb_io_realtek.h"
#include <drv_types.h>

void usbhost_rtl8723d_initialize(void);
int usbhost_ctrl_req(void *priv, unsigned char bdir_in, unsigned int wvalue, unsigned char *buf, unsigned int len);
int usbhost_get_speed(void *priv);
int usbhost_get_in_ep_info(void *priv, unsigned char *ep_addr_array);
int usbhost_get_out_ep_info(void *priv, unsigned char *ep_addr_array);
unsigned char usbhost_get_bulk_in_pipe(void *priv, unsigned char en_addr);
unsigned char usbhost_get_bulk_out_pipe(void *priv, unsigned char en_addr);
int usbhost_bulk_in(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len);
int usbhost_bulk_out(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len);
int usbhost_cancel_bulk_in(void *priv);
int usbhost_cancel_bulk_out(void *priv);

extern void *g_rtk_wifi_usb;
extern unsigned char g_rtk_wifi_connect;
static unsigned char rtk_usb_running = 0;
extern unsigned char usb_thread_active;
extern _mutex usb_host_mutex;

static void rtw_usb_probe(void)
{
	nvdbg("rtw_usb_probe -------> \n");

	if (!g_rtk_wifi_connect)
		usbhost_rtl8723d_initialize();

	while (1) {

		if (g_rtk_wifi_connect)
			break;
		else
			usleep(10000);
	}

	nvdbg("rtw_usb_probe <------- \n");
}

int usb_thread(int argc, char *argv[])
{
	extern PADAPTER padapter_for_Tizenrt;
	extern ssize_t nread_bytes;
	unsigned int count = 0;
	PADAPTER padapter = NULL;
	struct recv_buf *precvbuf;
	struct recv_priv *precvpriv;
	int i = 0;
	pthread_detach(pthread_self());

	_func_enter_;

	while (1) {
		if (!usb_thread_active) {
			break;
		}
		if (rtk_usb_running) {
			if ((padapter == NULL) && (padapter_for_Tizenrt != NULL)) {
				padapter = padapter_for_Tizenrt;
				precvpriv = &(padapter->recvpriv);
				precvpriv->ff_hwaddr = RECV_BULK_IN_ADDR;
				precvbuf = (struct recv_buf *)precvpriv->precv_buf;
			}
			count = rtw_read8(padapter, 0x287);
			if (count != 0) {
				for (i = 0; i < count; i++) {
					rtw_read_port(padapter, precvpriv->ff_hwaddr, 0, (unsigned char *)precvbuf, 0);
					usb_read_port_complete((void *)precvbuf, nread_bytes);
					if (i == 5) {
						continue;
					}
				}
			} else {
				rtw_msleep_os(1);
			}
		} else {
			if (padapter) {
				padapter = NULL;
			}
			rtw_msleep_os(2000);
		}
	}

	_func_exit_;

	nvdbg("exit usb read port thread \n");
	return 0;
}

main_t _usb_thread = &usb_thread;

static int rtw_usb_ctrl_req(void *priv, unsigned char bdir_in, unsigned int wvalue, unsigned char *buf, unsigned int len)
{
	return usbhost_ctrl_req(priv, bdir_in, wvalue, buf, len);
}

static int rtw_usb_get_speed_info(void *priv)
{
	u8 usb_speed = 0;
	u8 ret = RTW_USB_SPEED_UNKNOWN;

	usb_speed = usbhost_get_speed(priv);

	switch (usb_speed) {
	case 1:
		nvdbg("Low Speed Case \n");
		ret = RTW_USB_SPEED_1_1;
		break;
	case 2:
		nvdbg("full speed Case \n");
		ret = RTW_USB_SPEED_1_1;
		break;
	case 3:
		nvdbg("high speed Case \n");
		ret = RTW_USB_SPEED_2;
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

static int rtw_usb_get_in_ep_info(void *priv, unsigned char *ep_addr_array)
{
	return usbhost_get_in_ep_info(priv, ep_addr_array);
}

static int rtw_usb_get_out_ep_info(void *priv, unsigned char *ep_addr_array)
{
	return usbhost_get_out_ep_info(priv, ep_addr_array);
}

static unsigned char rtw_usb_get_bulk_in_pipe(void *priv, unsigned char ep_addr)
{
	return usbhost_get_bulk_in_pipe(priv, ep_addr);
}

static unsigned char rtw_usb_get_bulk_out_pipe(void *priv, unsigned char ep_addr)
{
	return usbhost_get_bulk_out_pipe(priv, ep_addr);
}

ssize_t nread_bytes = 0;

static int rtw_usb_bulk_in(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usb_complete callback, void *arg)
{
	int ret = 0;

	nread_bytes = 0;

	if (!rtk_usb_running) {
		rtk_usb_running = 1;
		ndbg("\r\n[rtw_usb_bulk_in]:rtk_usb_running enabled");
	}

	nread_bytes = usbhost_bulk_in(priv, pipe, buf, len);

	if (nread_bytes > 0) {
		ret = 0;
	} else {
		ndbg("usb bulk in fail: %d \n", nread_bytes);
		nread_bytes = 0;
	}

	return ret;
}

static int rtw_usb_bulk_out(void *priv, unsigned char pipe, unsigned char *buf, unsigned int len, usb_complete callback, void *arg)
{
	ssize_t nwritten_bytes = 0;
	int ret = 0;

	nwritten_bytes = usbhost_bulk_out(priv, pipe, buf, len);

	if (nwritten_bytes > 0) {
		ret = 0;
		callback(arg, nwritten_bytes);
	} else {
		ndbg("usb bulk out fail: %d \n", nwritten_bytes);
		nwritten_bytes = 0;
	}

	return ret;
}

static int rtw_usb_cancel_bulk_in(void *priv)
{
	if (rtk_usb_running) {
		rtk_usb_running = 0;
		ndbg("\r\n[rtw_usb_cancel_bulk_in]:rtk_usb_running disabled");
	}

	return usbhost_cancel_bulk_in(priv);
}

static int rtw_usb_cancel_bulk_out(void *priv)
{
	return usbhost_cancel_bulk_out(priv);
}

extern USB_BUS_OPS rtw_usb_bus_ops = {
	rtw_usb_probe,
	NULL,
	rtw_usb_ctrl_req,
	rtw_usb_get_speed_info,
	rtw_usb_get_in_ep_info,
	rtw_usb_get_out_ep_info,
	rtw_usb_get_bulk_in_pipe,
	rtw_usb_get_bulk_out_pipe,
	rtw_usb_bulk_in,
	rtw_usb_bulk_out,
	rtw_usb_cancel_bulk_in,
	rtw_usb_cancel_bulk_out
};
