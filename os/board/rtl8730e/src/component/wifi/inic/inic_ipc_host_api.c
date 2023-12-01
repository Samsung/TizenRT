/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

/******************************************************************************
 * history *

******************************************************************************/
#include "inic_ipc_api.h"
#include "lwip_netconf.h"
#include "inic_ipc_cfg.h"
#include "wifi_ind.h"

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_INIC_IPC_HOST_API_PRIO 3
#if defined(CONFIG_ENABLE_WPS) && CONFIG_ENABLE_WPS
#define HOST_STACK_SIZE 2048
#else
#define HOST_STACK_SIZE 1024	// for psp overflow when update group key: jira: https://jira.realtek.com/browse/RSWLANQC-1027
#endif
/* ---------------------------- Global Variables ---------------------------- */
_sema  g_host_inic_api_task_wake_sema = NULL;
_sema  g_host_inic_api_message_send_sema = NULL;
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
struct task_struct inic_ipc_api_host_handler;
#endif

//todo:move to non-cache data section
inic_ipc_host_request_message g_host_ipc_api_request_info __attribute__((aligned(64)));
#ifdef IPC_DIR_MSG_TX
IPC_MSG_STRUCT g_host_ipc_api_msg __attribute__((aligned(64)));
#endif

/* -------------------------- Function declaration -------------------------- */
#if CONFIG_AUTO_RECONNECT
extern p_wlan_autoreconnect_hdl_t p_wlan_autoreconnect_hdl;
#endif
#ifdef CONFIG_ENABLE_EAP
extern void eap_autoreconnect_hdl(u8 method_id);
#endif
extern int (*scan_ssid_result_hdl)(char *, int, char *, void *);
extern rtw_result_t (*scan_user_callback_ptr)(unsigned int, void *);
extern rtw_result_t (*scan_each_report_user_callback_ptr)(rtw_scan_result_t *, void *);
extern ap_channel_switch_callback_t p_ap_channel_switch_callback;

extern void (*promisc_user_callback_ptr)(void *);
#ifndef CONFIG_PLATFORM_TIZENRT_OS
extern int dhcps_ip_in_table_check(uint8_t gate, uint8_t d);
#endif
/* ---------------------------- Private Functions --------------------------- */
static void _inic_ipc_api_host_scan_user_callback_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	unsigned int ap_num = p_ipc_msg->param_buf[0];
	void *user_data = (void *)p_ipc_msg->param_buf[1];

	if (scan_user_callback_ptr) {
		scan_user_callback_ptr(ap_num, user_data);
	}
}

static void _inic_ipc_api_host_scan_each_report_callback_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	rtw_scan_result_t *scanned_ap_info = (rtw_scan_result_t *)p_ipc_msg->param_buf[0];
	void *user_data = (void *)p_ipc_msg->param_buf[1];
	DCache_Invalidate((u32)scanned_ap_info, sizeof(rtw_scan_result_t));

	if (scan_each_report_user_callback_ptr) {
		scan_each_report_user_callback_ptr(scanned_ap_info, user_data);
	}
}

static void _inic_ipc_api_host_autoreconnect_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	rtw_security_t security_type = (rtw_security_t)p_ipc_msg->param_buf[0];
	char *ssid = (char *)p_ipc_msg->param_buf[1];
	int ssid_len = (int)p_ipc_msg->param_buf[2];
	char *password = (char *)p_ipc_msg->param_buf[3];
	int password_len = (int)p_ipc_msg->param_buf[4];
	int key_id = (int)p_ipc_msg->param_buf[5];
	char is_wps_trigger = (int)p_ipc_msg->param_buf[6];

	DCache_Invalidate((u32)ssid, ssid_len);
	DCache_Invalidate((u32)password, password_len);
#if CONFIG_AUTO_RECONNECT
	if (p_wlan_autoreconnect_hdl) {
		p_wlan_autoreconnect_hdl(security_type, ssid, ssid_len, password, password_len, key_id, is_wps_trigger);
	}
#endif
}

static void _inic_ipc_api_host_eap_autoreconnect_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
#ifdef CONFIG_ENABLE_EAP
	u8 eap_method = (u8)p_ipc_msg->param_buf[0];
	eap_autoreconnect_hdl(eap_method);
#else
	UNUSED(p_ipc_msg);
#endif
}

static void _inic_ipc_api_host_ap_ch_switch_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	unsigned char channel = (unsigned char)p_ipc_msg->param_buf[0];
	rtw_channel_switch_res_t res = (rtw_channel_switch_res_t)p_ipc_msg->param_buf[1];

	if (p_ap_channel_switch_callback) {
		p_ap_channel_switch_callback(channel, res);
	}
}

static void _inic_ipc_api_host_wifi_event_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	rtw_event_indicate_t event = (rtw_event_indicate_t)p_ipc_msg->param_buf[0];
	char *buf = (char *)p_ipc_msg->param_buf[1];
	int buf_len = (int)p_ipc_msg->param_buf[2];
	int flags = (int)p_ipc_msg->param_buf[3];
	DCache_Invalidate((u32)buf, buf_len);

	wifi_indication(event, buf, buf_len, flags);
}

static void _inic_ipc_api_host_lwip_info_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	u32 type = (u32)p_ipc_msg->param_buf[0];
	unsigned char *input = (unsigned char *)p_ipc_msg->param_buf[1];
	int idx = p_ipc_msg->param_buf[2];

	switch (type) {
	case IPC_WLAN_GET_IP:
		p_ipc_msg->ret = (u32)LwIP_GetIP(idx);
		DCache_Clean(p_ipc_msg->ret, 4);
		break;
	case IPC_WLAN_GET_GW:
		p_ipc_msg->ret = (u32)LwIP_GetGW(idx);
		DCache_Clean(p_ipc_msg->ret, 4);
		break;
	case IPC_WLAN_GET_GWMSK:
		p_ipc_msg->ret = (u32)LwIP_GetMASK(idx);
		DCache_Clean(p_ipc_msg->ret, 4);
		break;
	case IPC_WLAN_GET_HW_ADDR:
		p_ipc_msg->ret = (u32)LwIP_GetMAC(idx);
		DCache_Clean(p_ipc_msg->ret, 6);
		break;
	case IPC_WLAN_IS_VALID_IP:
		DCache_Invalidate((u32)input, 4);
		p_ipc_msg->ret = LwIP_netif_is_valid_IP(idx, input);
		break;
	}
}

static void _inic_ipc_api_host_set_netif_info_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	int idx = (u32)p_ipc_msg->param_buf[0];
	unsigned char *dev_addr = (unsigned char *)p_ipc_msg->param_buf[1];
	DCache_Invalidate((u32)dev_addr, ETH_ALEN);

	LwIP_wlan_set_netif_info(idx, NULL, dev_addr);
}

static u32 _inic_ipc_ip_addr_update_in_wowlan(u32 expected_idle_time, void *param)
{
	/* To avoid gcc warnings */
	(void) expected_idle_time;
	(void) param;
	u32 try_cnt = 2500;//wait 5ms

	rtw_memset(&g_host_ipc_api_request_info, 0, sizeof(inic_ipc_host_request_message));

	g_host_ipc_api_request_info.API_ID = IPC_API_WIFI_IP_UPDATE;
	g_host_ipc_api_request_info.param_buf[0] = (u32)LwIP_GetIP(0);
	DCache_Clean(g_host_ipc_api_request_info.param_buf[0], 4);

	DCache_Clean((u32)&g_host_ipc_api_request_info, sizeof(inic_ipc_host_request_message));

	rtw_memset(&g_host_ipc_api_msg, 0, sizeof(IPC_MSG_STRUCT));
	g_host_ipc_api_msg.msg = (u32)&g_host_ipc_api_request_info;
	g_host_ipc_api_msg.msg_type = IPC_USER_POINT;
	g_host_ipc_api_msg.msg_len = sizeof(inic_ipc_host_request_message);
	DCache_Clean((u32)&g_host_ipc_api_msg, sizeof(IPC_MSG_STRUCT));
	ipc_send_message(IPC_DIR_MSG_TX, IPC_H2D_WIFI_API_TRAN, \
					 &g_host_ipc_api_msg);

	while (try_cnt) {
		DCache_Invalidate((u32)&g_host_ipc_api_request_info, sizeof(inic_ipc_host_request_message));
		if (g_host_ipc_api_request_info.API_ID != IPC_WIFI_API_PROCESS_DONE) {
			try_cnt --;
			DelayUs(2);
		} else {
			break;
		}
	}
	if (try_cnt == 0) {
		/* jira: https://jira.realtek.com/browse/RSWLANQC-1036 */
		DBG_ERR("update ip address TO, Driver busy\n");
		g_host_ipc_api_request_info.API_ID = IPC_API_WIFI_MSG_TO;
		DCache_Clean((u32)&g_host_ipc_api_request_info, sizeof(inic_ipc_host_request_message));
		return _FAIL;
	}
	return _SUCCESS;
}

static void _inic_ipc_api_host_promisc_user_callback_handler(inic_ipc_dev_request_message *p_ipc_msg)
{
	struct rx_pkt_info *ppktinfo = (struct rx_pkt_info *)p_ipc_msg->param_buf[0];

	if (promisc_user_callback_ptr) {
		/* invalidate will be safe if callback read mem only */
		DCache_Invalidate((u32)ppktinfo, sizeof(struct rx_pkt_info));
		DCache_Invalidate((u32)ppktinfo->buf, (u32)ppktinfo->len);
		promisc_user_callback_ptr((void *)p_ipc_msg->param_buf[0]);
	}
}

static void _inic_ipc_api_host_buffered_printf(inic_ipc_dev_request_message *p_ipc_msg)
{
	char *tmp_buffer = (char *)p_ipc_msg->param_buf[0];
	int buf_size = (int)p_ipc_msg->param_buf[1];
	DCache_Invalidate((u32)tmp_buffer, buf_size);

	dbg("%s", tmp_buffer);
	p_ipc_msg->ret = 0;
}

/* ---------------------------- Public Functions ---------------------------- */
/**
 * @brief  process the ipc message.
 * @param  none.
 * @return none.
 */
void inic_ipc_api_host_task(void)
{
	inic_ipc_dev_request_message *p_ipc_msg = NULL;

	do {
		rtw_down_sema(&g_host_inic_api_task_wake_sema);

#ifdef IPC_DIR_MSG_RX
		PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(IPC_DIR_MSG_RX, \
										 IPC_D2H_WIFI_API_TRAN);
		p_ipc_msg = (inic_ipc_dev_request_message *)p_ipc_recv_msg->msg;
#else
		p_ipc_msg = (inic_ipc_dev_request_message *)ipc_get_message(IPC_INT_CHAN_WIFI_API_TRAN);
#endif /* IPC_DIR_MSG_RX */

		DCache_Invalidate((u32)p_ipc_msg, sizeof(inic_ipc_dev_request_message));

		if (p_ipc_msg == NULL) {
			DBG_8195A("Device IPC API message is NULL, invalid!\n\r");
			continue;
		}

		switch (p_ipc_msg->EVENT_ID) {
		/* receive callback indication */
		case IPC_WIFI_EVT_SCAN_USER_CALLBACK:
			_inic_ipc_api_host_scan_user_callback_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_SCAN_EACH_REPORT_USER_CALLBACK:
			_inic_ipc_api_host_scan_each_report_callback_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_AUTO_RECONNECT:
			_inic_ipc_api_host_autoreconnect_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_EAP_AUTO_RECONNECT:
			_inic_ipc_api_host_eap_autoreconnect_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_AP_CH_SWITCH:
			_inic_ipc_api_host_ap_ch_switch_handler(p_ipc_msg);
			break;
		/* receive wifi event indication */
		case IPC_WIFI_EVT_HDL:
			_inic_ipc_api_host_wifi_event_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_GET_LWIP_INFO:
			_inic_ipc_api_host_lwip_info_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_SET_NETIF_INFO:
			_inic_ipc_api_host_set_netif_info_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_PROMISC_CALLBACK:
			_inic_ipc_api_host_promisc_user_callback_handler(p_ipc_msg);
			break;
		case IPC_WIFI_EVT_IP_TABLE_CHK:
#ifndef CONFIG_PLATFORM_TIZENRT_OS
			p_ipc_msg->ret = dhcps_ip_in_table_check(p_ipc_msg->param_buf[0], p_ipc_msg->param_buf[1]);
#endif
			break;
		case IPC_BUFFERED_PRINTF_NP:
			_inic_ipc_api_host_buffered_printf(p_ipc_msg);
			break;
		default:
			DBG_8195A("Host API Unknown event(%d)!\n\r", \
					  p_ipc_msg->EVENT_ID);
			break;
		}
		/*set EVENT_ID to 0 to notify NP that event is finished*/
		p_ipc_msg->EVENT_ID = 0;
		DCache_Clean((u32)p_ipc_msg, sizeof(inic_ipc_dev_request_message));
	} while (1);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	rtw_delete_task(&inic_ipc_api_host_handler);
#endif
}

/**
 * @brief  to haddle the ipc message interrupt, wakeup event task to process.
 * @param  Data[inout]: IPC data.
 * @param  IrqStatus[in]: interrupt status.
 * @param  ChanNum[in]: IPC channel number.
 * @return none.
 */
void inic_ipc_api_host_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	/* wakeup task */
	rtw_up_sema_from_isr(&g_host_inic_api_task_wake_sema);
}

/**
 * @brief  to send a ipc message to device and wait result.
 * @param  ID[in]: API_ID.
 * @param  param_buf[in]: pointer to API parameter.
 * @return result of API.
 */
int inic_ipc_api_host_message_send(u32 id, u32 *param_buf, u32 buf_len)
{
	int ret = 0;
	rtw_down_sema(&g_host_inic_api_message_send_sema);

	rtw_memset(&g_host_ipc_api_request_info, 0, sizeof(inic_ipc_host_request_message));

	g_host_ipc_api_request_info.API_ID = id;
	if (param_buf != NULL) {
		rtw_memcpy(g_host_ipc_api_request_info.param_buf, param_buf, buf_len * sizeof(u32));
	}
	DCache_Clean((u32)&g_host_ipc_api_request_info, sizeof(inic_ipc_host_request_message));
#ifdef IPC_DIR_MSG_TX
	rtw_memset(&g_host_ipc_api_msg, 0, sizeof(IPC_MSG_STRUCT));
	g_host_ipc_api_msg.msg = (u32)&g_host_ipc_api_request_info;
	g_host_ipc_api_msg.msg_type = IPC_USER_POINT;
	g_host_ipc_api_msg.msg_len = sizeof(inic_ipc_host_request_message);
	DCache_Clean((u32)&g_host_ipc_api_msg, sizeof(IPC_MSG_STRUCT));

	ipc_send_message(IPC_DIR_MSG_TX, IPC_H2D_WIFI_API_TRAN, \
					 &g_host_ipc_api_msg);
#else
	ipc_send_message(IPC_INT_CHAN_WIFI_API_TRAN, &g_host_ipc_api_request_info);
#endif /* IPC_DIR_MSG_TX */

	while (1) {
		if (g_host_ipc_api_request_info.API_ID != IPC_WIFI_API_PROCESS_DONE) {
			rtw_mdelay_os(1);
			DCache_Invalidate((u32)&g_host_ipc_api_request_info, sizeof(inic_ipc_host_request_message));
		} else {
			break;
		}
	}
	ret = g_host_ipc_api_request_info.ret;
	rtw_up_sema(&g_host_inic_api_message_send_sema);
	return ret;
}

/**
 * @brief  to initialize the ipc host for WIFI api.
 * @param  none.
 * @return none.
 */
void inic_ipc_api_init_host(VOID)
{
	/* initialize the semaphores */
	rtw_init_sema(&g_host_inic_api_task_wake_sema, 0);
	rtw_init_sema(&g_host_inic_api_message_send_sema, 0);
	rtw_up_sema(&g_host_inic_api_message_send_sema);

	/*for updating ip address before sleep*/
	pmu_register_sleep_callback(PMU_WLAN_DEVICE, (PSM_HOOK_FUN)_inic_ipc_ip_addr_update_in_wowlan, NULL, NULL, NULL);

	/* Initialize the event task */
	if (rtw_create_task(&inic_ipc_api_host_handler, (const char *const)"inic_ipc_api_host_task", HOST_STACK_SIZE, (0 + CONFIG_INIC_IPC_HOST_API_PRIO), (void*)inic_ipc_api_host_task, NULL) != 1) {
			DBG_8195A("Create inic_ipc_api_host_task Err!!\n");
		}
}

u64 inic_ipc_host_get_wifi_tsf(unsigned char port_id)
{
	u64 ret = 0;

	if ((HAL_READ32(WIFI_REG_BASE, 0xA4) & 0x7F00) == BIT13) {
		/* in ips flow, it will return 0 or will be hang, thus need additional check*/
		if (port_id == 0) {
			ret = (((u64) HAL_READ32(WIFI_REG_BASE, 0x564)) << 32) | HAL_READ32(WIFI_REG_BASE, 0x560); //REG_P0_TSFTR
		} else if (port_id == 1) {
			ret = (((u64) HAL_READ32(WIFI_REG_BASE, 0x56C)) << 32) | HAL_READ32(WIFI_REG_BASE, 0x568); //REG_P1_TSFTR
		}
	} else {
		ret = 0; /* !pon state */
	}

	return ret;
}

int inic_ipc_host_get_txbuf_pkt_num(void)
{
	int ret = 0;
#ifdef CONFIG_PLATFORM_AMEBAD2
	u16 queue0_info = (HAL_READ16(WIFI_REG_BASE, 0x400) & 0x7F00) >> 8;//REG_Q0_INFO
	u16 queue1_info = (HAL_READ16(WIFI_REG_BASE, 0x404) & 0x7F00) >> 8;//REG_Q1_INFO
	u16 queue2_info = (HAL_READ16(WIFI_REG_BASE, 0x408) & 0x7F00) >> 8;//REG_Q2_INFO
	u16 queue3_info = (HAL_READ16(WIFI_REG_BASE, 0x40C) & 0x7F00) >> 8;//REG_Q3_INFO
	u16 mgnt_queue_info = (HAL_READ16(WIFI_REG_BASE, 0x410) & 0x7F00) >> 8;//REG_MGQ_INFO
	u16 high_queue_info = (HAL_READ16(WIFI_REG_BASE, 0x414) & 0x7F00) >> 8;//REG_HIQ_INFO
	ret = queue0_info + queue1_info + queue2_info + queue3_info + mgnt_queue_info + high_queue_info;
#endif
	return ret;

}

int inic_ipc_iwpriv_command(char *cmd, unsigned int cmd_len, int show_msg)
{
	int ret = 0;
	u32 param_buf[3];

	param_buf[0] = (u32)cmd;
	param_buf[1] = (u32)cmd_len;
	param_buf[2] = (u32)show_msg;
	DCache_Clean((u32)cmd, cmd_len);

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IWPRIV_INFO, param_buf, 3);
	return ret;
}

void inic_ipc_buffered_printf_set_np_enable(u8 enable)
{
	u32 param_buf[1];
	param_buf[0] = (u32)enable;
	inic_ipc_api_host_message_send(IPC_API_BUFFERED_PRINTF_SET_NP_EN, param_buf, 1);
}

#ifdef CONFIG_MP_INCLUDED
void inic_ipc_mp_command(char *token, unsigned int cmd_len, int show_msg)
{
	u32 param_buf[4];
	char *user_buf = (char *)rtw_zmalloc(INIC_MP_MSG_BUF_SIZE); //support max buf for PSD
	if (user_buf == NULL) {
		printf("user buf for mp malloc failed\n");
		return;
	}

	param_buf[0] = (u32)token;
	param_buf[1] = (u32)cmd_len;
	param_buf[2] = (u32)show_msg;
	param_buf[3] = (u32)user_buf;
	DCache_Clean((u32)user_buf, INIC_MP_MSG_BUF_SIZE);
	DCache_Clean((u32)token, cmd_len);

	inic_ipc_api_host_message_send(IPC_API_WIFI_MP_CMD, param_buf, 4);
	/* user_buf contains mp command result(in string format) from NP core,
	Dcache_Invalidate user_buf before use it */
	if (show_msg) {
		DCache_Invalidate((u32)user_buf, INIC_MP_MSG_BUF_SIZE);
		printf("\n\rPrivate Message: %s\n", user_buf);
	}
	rtw_mfree((u8 *)user_buf, INIC_MP_MSG_BUF_SIZE);
}
#endif

/* ---------------------------- Global Variables ---------------------------- */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_RTL8720F)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_api_host_table = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = inic_ipc_api_host_int_hdl,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_DIR_MSG_RX,
	.IPC_Channel = IPC_D2H_WIFI_API_TRAN
};
#endif
