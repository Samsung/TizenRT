#include "rtw_coex_host_api.h"

/*
 * function extern
*/
extern int rtk_coex_ipc_h2c_info_handler(u16 type, u8 *pdata, u16 data_len);

/*
 * function define
*/
/////////////////////////////////////////////////////////////////////////////
/////// API for h2c (app side)
/////////////////////////////////////////////////////////////////////////////
__weak void rtk_coex_btc_bt_hci_notify(u8 *pdata, u16 len, u8 type)
{
	rtk_coex_ipc_h2c_info_handler(COEX_BT_TYPE(type), pdata, len);
}

__weak int rtk_coex_btc_bt_rfk(void *p_rfk_param, u16 length)
{
	return rtk_coex_ipc_h2c_info_handler(COEX_BT_TYPE(COEX_H2C_BT_RFK), (u8 *)p_rfk_param, length);
}

__weak void rtk_coex_btc_set_bt_ant(u8 bt_ant)
{
	rtk_coex_ipc_h2c_info_handler(COEX_BT_TYPE(COEX_H2C_BT_SET_BT_SEL), &bt_ant, 1);
}

__weak void rtk_coex_btc_set_pta(u8 type, u8 role, u8 process)
{
	struct pta_para_t pta_para = {
		.type = type,
		.role = role,
		.process = process
	};

	rtk_coex_ipc_h2c_info_handler(COEX_BT_TYPE(COEX_H2C_BT_SET_PTA), (u8 *)&pta_para, sizeof(pta_para));
}


/////////////////////////////////////////////////////////////////////////////
/////// API for c2h (app side)
/////////////////////////////////////////////////////////////////////////////
__weak void bt_coex_send_w2b_sw_mailbox(uint8_t *user_data, uint16_t length)
{
	RTK_LOGS(NOTAG, RTK_LOG_ERROR, "weak bt_coex_send_w2b_sw_mailbox, check!!!\r\n");
	(void) user_data;
	(void) length;
}

void rtk_coex_btc_wl_send_sw_mailbox(u8 type, u8 *data, u16 length)
{
	(void) type;
	bt_coex_send_w2b_sw_mailbox(data, length);
}
