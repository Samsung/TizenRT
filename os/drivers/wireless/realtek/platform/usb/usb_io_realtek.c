#include "usb_io_realtek.h"
#include <drv_types.h>

void usbhost_rtl8723d_initialize(void);
int usbhost_ctrl_req(void *priv,unsigned char bdir_in,unsigned int wvalue,unsigned char *buf,unsigned int len);
int usbhost_get_speed(void *priv);
int usbhost_get_in_ep_info(void *priv, unsigned char *ep_addr_array);
int usbhost_get_out_ep_info(void *priv, unsigned char *ep_addr_array);
unsigned char usbhost_get_bulk_in_pipe(void *priv,unsigned char en_addr);
unsigned char usbhost_get_bulk_out_pipe(void *priv,unsigned char en_addr);
int usbhost_bulk_in(void *priv,unsigned char pipe,unsigned char *buf,unsigned int len);
int usbhost_bulk_out(void *priv,unsigned char pipe,unsigned char *buf,unsigned int len);
int usbhost_cancel_bulk_in(void *priv);
int usbhost_cancel_bulk_out(void *priv);

extern void *g_rtk_wifi_usb;
unsigned char g_rtk_wifi_connect;

static void rtw_usb_probe(void)
{
	printf("rtw_usb_probe -------> \n");

	if(!g_rtk_wifi_connect)
		usbhost_rtl8723d_initialize();

	while(1){
		
		if(g_rtk_wifi_connect)
			break;
		else
			usleep(10000);
	}

	
	printf("rtw_usb_probe <------- \n");
}

static int rtw_usb_ctrl_req(void *priv,unsigned char bdir_in,unsigned int wvalue,unsigned char *buf,unsigned int len)
{
	return usbhost_ctrl_req(priv,bdir_in,wvalue,buf,len);
}


static int rtw_usb_get_speed_info(void *priv)
{
	u8 usb_speed = 	0;
	u8 ret = RTW_USB_SPEED_UNKNOWN;

	usb_speed = usbhost_get_speed(priv);

	switch (usb_speed) {
	case 1:
		 printf("Low Speed Case \n");
		 ret = RTW_USB_SPEED_1_1;
		 break;
	case 2:
		 printf("full speed Case \n");
		 ret = RTW_USB_SPEED_1_1;
		 break;
	case 3:
		 printf("high speed Case \n");
		 ret = RTW_USB_SPEED_2;
		 break;
	default:
		ret = 0;
		break;
	}

	return ret;

}

static int rtw_usb_get_in_ep_info(void *priv,unsigned char *ep_addr_array)
{
	return usbhost_get_in_ep_info(priv,ep_addr_array);
}

static int rtw_usb_get_out_ep_info(void *priv,unsigned char *ep_addr_array)
{
	return usbhost_get_out_ep_info(priv,ep_addr_array);
}

static unsigned char rtw_usb_get_bulk_in_pipe(void *priv,unsigned char ep_addr)
{
	return usbhost_get_bulk_in_pipe(priv,ep_addr);
}

static unsigned char rtw_usb_get_bulk_out_pipe(void *priv,unsigned char ep_addr)
{
	return usbhost_get_bulk_out_pipe(priv,ep_addr);
}

ssize_t  nread_bytes = 0;

static int rtw_usb_bulk_in(void *priv,unsigned char pipe,unsigned char *buf,unsigned int len,usb_complete callback,void *arg)
{
	int ret = 0;

	nread_bytes = 0;

	//printf("rtw_usb_bulk_in:  buf = %x,len = %d \n",buf,len);
	
	nread_bytes = usbhost_bulk_in(priv,pipe,buf,len);
	
	if(nread_bytes > 0){
		ret = 0;
		//printf("usb bulk in: nbytes = %d \n",nread_bytes);
		//callback(arg,nread_bytes);
	}
	else{
		printf("usb bulk in fail: %d \n",nread_bytes);
		nread_bytes = 0;
	}

	return ret;
}

static int rtw_usb_bulk_out(void *priv,unsigned char pipe,unsigned char *buf,unsigned int len,usb_complete callback,void *arg)
{
	ssize_t nwritten_bytes = 0;
	int ret = 0;

	//printf("usb bulk out : pipe = %d \n",pipe);

	nwritten_bytes = usbhost_bulk_out(priv,pipe,buf,len);

	//printf("nwritten_bytes = %d \n",nwritten_bytes);

	if(nwritten_bytes > 0){
		ret = 0;
		callback(arg,nwritten_bytes);
	}
	else{
		printf("usb bulk out fail: %d \n",nwritten_bytes);
		nwritten_bytes = 0;
	}

	return ret;
}

static int rtw_usb_cancel_bulk_in(void *priv)
{
	return usbhost_cancel_bulk_in(priv);
}

static int rtw_usb_cancel_bulk_out(void *priv)
{
	return usbhost_cancel_bulk_out(priv);
}

extern USB_BUS_OPS rtw_usb_bus_ops= {
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
